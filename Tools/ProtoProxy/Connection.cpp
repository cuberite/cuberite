
// Connection.cpp

// Interfaces to the cConnection class representing a single pair of connected sockets

#include "Globals.h"
#include "Connection.h"
#include "Server.h"
#include <iostream>

#ifdef _WIN32
	#include <direct.h>  // For _mkdir()
#endif




/// When defined, the following macro causes a sleep after each parsed packet (DEBUG-mode only)
// #define SLEEP_AFTER_PACKET





#if defined(_DEBUG) && defined(SLEEP_AFTER_PACKET)
	#define DebugSleep Sleep
#else
	#define DebugSleep(X)
#endif  // else _DEBUG





#define HANDLE_CLIENT_PACKET_READ(Proc, Type, Var) \
	Type Var; \
	{ \
		if (!m_ClientBuffer.Proc(Var)) \
		{ \
			return false; \
		} \
	}

#define HANDLE_SERVER_PACKET_READ(Proc, Type, Var) \
	Type Var; \
	{ \
		if (!m_ServerBuffer.Proc(Var)) \
		{ \
			return false; \
		} \
	}

#define CLIENTSEND(...) SendData(m_ClientSocket, __VA_ARGS__, "Client")
#define SERVERSEND(...) SendData(m_ServerSocket, __VA_ARGS__, "Server")
#define CLIENTENCRYPTSEND(...) SendData(m_ClientSocket, __VA_ARGS__, "Client")  // The client conn is always unencrypted
#define SERVERENCRYPTSEND(...) SendEncryptedData(m_ServerSocket, m_ServerEncryptor, __VA_ARGS__, "Server")

#define COPY_TO_SERVER() \
	{ \
		AString ToServer; \
		m_ClientBuffer.ReadAgain(ToServer); \
		switch (m_ServerState) \
		{ \
			case csUnencrypted: \
			{ \
				SERVERSEND(ToServer.data(), ToServer.size()); \
				break; \
			} \
			case csEncryptedUnderstood: \
			case csEncryptedUnknown: \
			{ \
				SERVERENCRYPTSEND(ToServer.data(), ToServer.size()); \
				break; \
			} \
			case csWaitingForEncryption: \
			{ \
				Log("Waiting for server encryption, queued %u bytes", ToServer.size()); \
				m_ServerEncryptionBuffer.append(ToServer.data(), ToServer.size()); \
				break; \
			} \
		} \
		DebugSleep(50); \
	}

#define COPY_TO_CLIENT() \
	{ \
		AString ToClient; \
		m_ServerBuffer.ReadAgain(ToClient); \
		switch (m_ClientState) \
		{ \
			case csUnencrypted: \
			{ \
				CLIENTSEND(ToClient.data(), ToClient.size()); \
				break; \
			} \
			case csEncryptedUnderstood: \
			case csEncryptedUnknown: \
			{ \
				CLIENTENCRYPTSEND(ToClient.data(), ToClient.size()); \
				break; \
			} \
			/* case csWaitingForEncryption: \
			{ \
				Log("Waiting for client encryption, queued %u bytes", ToClient.size()); \
				m_ClientEncryptionBuffer.append(ToClient.data(), ToClient.size()); \
				break; \
			} \
			*/ \
		} \
		DebugSleep(50); \
	}

#define HANDLE_CLIENT_READ(Proc) \
	{ \
		if (!Proc) \
		{ \
			AString Leftover; \
			m_ClientBuffer.ReadAgain(Leftover); \
			DataLog(Leftover.data(), Leftover.size(), "Leftover data after client packet parsing, %d bytes:", Leftover.size()); \
			m_ClientBuffer.ResetRead(); \
			return true; \
		} \
	}
	
#define HANDLE_SERVER_READ(Proc) \
	{ \
		if (!Proc) \
		{ \
			m_ServerBuffer.ResetRead(); \
			return true; \
		} \
	}
	
	
#define MAX_ENC_LEN 1024




typedef unsigned char Byte;





AString PrintableAbsIntTriplet(int a_X, int a_Y, int a_Z, double a_Divisor = 32)
{
	return Printf("<%d, %d, %d> ~ {%.02f, %.02f, %.02f}",
		a_X, a_Y, a_Z,
		(double)a_X / a_Divisor, (double)a_Y / a_Divisor, (double)a_Z / a_Divisor
	);
}





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cConnection:

cConnection::cConnection(SOCKET a_ClientSocket, cServer & a_Server) :
	m_ItemIdx(0),
	m_LogFile(NULL),
	m_Server(a_Server),
	m_ClientSocket(a_ClientSocket),
	m_ServerSocket(-1),
	m_BeginTick(m_Timer.GetNowTime()),
	m_ClientState(csUnencrypted),
	m_ServerState(csUnencrypted),
	m_Nonce(0),
	m_ClientBuffer(1024 KiB),
	m_ServerBuffer(1024 KiB),
	m_HasClientPinged(false),
	m_ServerProtocolState(-1),
	m_ClientProtocolState(-1),
	m_IsServerEncrypted(false)
{
	// Create the Logs subfolder, if not already created:
	#if defined(_WIN32)
		_mkdir("Logs");
	#else
		mkdir("Logs", 0777);
	#endif

	Printf(m_LogNameBase, "Logs/Log_%d_%d", (int)time(NULL), a_ClientSocket);
	AString fnam(m_LogNameBase);
	fnam.append(".log");
	m_LogFile = fopen(fnam.c_str(), "w");
	Log("Log file created");
	printf("Connection is logged to file \"%s\"\n", fnam.c_str());
}





cConnection::~cConnection()
{
	fclose(m_LogFile);
}





void cConnection::Run(void)
{
	if (!ConnectToServer())
	{
		Log("Cannot connect to server; aborting");
		return;
	}

	while (true)
	{
		fd_set ReadFDs;
		FD_ZERO(&ReadFDs);
		FD_SET(m_ServerSocket, &ReadFDs);
		FD_SET(m_ClientSocket, &ReadFDs);
		int res = select(2, &ReadFDs, NULL, NULL, NULL);
		if (res <= 0)
		{
			printf("select() failed: %d; aborting client", WSAGetLastError());
			break;
		}
		if (FD_ISSET(m_ServerSocket, &ReadFDs))
		{
			if (!RelayFromServer())
			{
				break;
			}
		}
		if (FD_ISSET(m_ClientSocket, &ReadFDs))
		{
			if (!RelayFromClient())
			{
				break;
			}
		}
	}
	Log("Relaying ended, closing sockets");
	closesocket(m_ServerSocket);
	closesocket(m_ClientSocket);
}





void cConnection::Log(const char * a_Format, ...)
{
	va_list args;
	va_start(args, a_Format);
	AString msg;
	AppendVPrintf(msg, a_Format, args);
	va_end(args);
	AString FullMsg;
	Printf(FullMsg, "[%5.3f] %s\n", GetRelativeTime(), msg.c_str());
	
	// Log to file:
	cCSLock Lock(m_CSLog);
	fputs(FullMsg.c_str(), m_LogFile);
	#ifdef _DEBUG
		fflush(m_LogFile);
	#endif  // _DEBUG
	
	// Log to screen:
	// std::cout << FullMsg;
}





void cConnection::DataLog(const void * a_Data, int a_Size, const char * a_Format, ...)
{
	va_list args;
	va_start(args, a_Format);
	AString msg;
	AppendVPrintf(msg, a_Format, args);
	va_end(args);
	AString FullMsg;
	AString Hex;
	Printf(FullMsg, "[%5.3f] %s\n%s\n", GetRelativeTime(), msg.c_str(), CreateHexDump(Hex, a_Data, a_Size, 16).c_str());
	
	// Log to file:
	cCSLock Lock(m_CSLog);
	fputs(FullMsg.c_str(), m_LogFile);
	
	/*
	// Log to screen:
	std::cout << FullMsg;
	//*/
}





void cConnection::LogFlush(void)
{
	fflush(m_LogFile);
}





bool cConnection::ConnectToServer(void)
{
	m_ServerSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (m_ServerSocket == INVALID_SOCKET)
	{
		return false;
	}
	sockaddr_in localhost;
	localhost.sin_family = AF_INET;
	localhost.sin_port = htons(m_Server.GetConnectPort());
	localhost.sin_addr.s_addr = htonl(0x7f000001);  // localhost
	if (connect(m_ServerSocket, (sockaddr *)&localhost, sizeof(localhost)) != 0)
	{
		printf("connection to server failed: %d\n", WSAGetLastError());
		return false;
	}
	Log("Connected to SERVER");
	return true;
}





bool cConnection::RelayFromServer(void)
{
	char Buffer[64 KiB];
	int res = recv(m_ServerSocket, Buffer, sizeof(Buffer), 0);
	if (res <= 0)
	{
		Log("Server closed the socket: %d; %d; aborting connection", res, WSAGetLastError());
		return false;
	}
	
	DataLog(Buffer, res, "Received %d bytes from the SERVER", res);
	
	switch (m_ServerState)
	{
		case csUnencrypted:
		case csWaitingForEncryption:
		{
			return DecodeServersPackets(Buffer, res);
		}
		case csEncryptedUnderstood:
		{
			m_ServerDecryptor.ProcessData((byte *)Buffer, (byte *)Buffer, res);
			DataLog(Buffer, res, "Decrypted %d bytes from the SERVER", res);
			return DecodeServersPackets(Buffer, res);
		}
		case csEncryptedUnknown:
		{
			m_ServerDecryptor.ProcessData((byte *)Buffer, (byte *)Buffer, res);
			DataLog(Buffer, res, "Decrypted %d bytes from the SERVER", res);
			return CLIENTSEND(Buffer, res);
		}
	}
	
	return true;
}





bool cConnection::RelayFromClient(void)
{
	char Buffer[64 KiB];
	int res = recv(m_ClientSocket, Buffer, sizeof(Buffer), 0);
	if (res <= 0)
	{
		Log("Client closed the socket: %d; %d; aborting connection", res, WSAGetLastError());
		return false;
	}
	
	DataLog(Buffer, res, "Received %d bytes from the CLIENT", res);
	
	switch (m_ClientState)
	{
		case csUnencrypted:
		case csWaitingForEncryption:
		{
			return DecodeClientsPackets(Buffer, res);
		}
		case csEncryptedUnderstood:
		{
			return DecodeClientsPackets(Buffer, res);
		}
		case csEncryptedUnknown:
		{
			DataLog(Buffer, res, "Decrypted %d bytes from the CLIENT", res);
			m_ServerEncryptor.ProcessData((byte *)Buffer, (byte *)Buffer, res);
			return SERVERSEND(Buffer, res);
		}
	}
	
	return true;
}





double cConnection::GetRelativeTime(void)
{
	return (double)(m_Timer.GetNowTime() - m_BeginTick) / 1000;
}





bool cConnection::SendData(SOCKET a_Socket, const char * a_Data, int a_Size, const char * a_Peer)
{
	DataLog(a_Data, a_Size, "Sending data to %s, %d bytes", a_Peer, a_Size);
	
	int res = send(a_Socket, a_Data, a_Size, 0);
	if (res <= 0)
	{
		Log("%s closed the socket: %d, %d; aborting connection", a_Peer, res, WSAGetLastError());
		return false;
	}
	return true;
}





bool cConnection::SendData(SOCKET a_Socket, cByteBuffer & a_Data, const char * a_Peer)
{
	AString All;
	a_Data.ReadAll(All);
	a_Data.CommitRead();
	return SendData(a_Socket, All.data(), All.size(), a_Peer);
}





bool cConnection::SendEncryptedData(SOCKET a_Socket, Encryptor & a_Encryptor, const char * a_Data, int a_Size, const char * a_Peer)
{
	DataLog(a_Data, a_Size, "Encrypting %d bytes to %s", a_Size, a_Peer);
	const byte * Data = (const byte *)a_Data;
	while (a_Size > 0)
	{
		byte Buffer[64 KiB];
		int NumBytes = (a_Size > sizeof(Buffer)) ? sizeof(Buffer) : a_Size;
		a_Encryptor.ProcessData(Buffer, Data, NumBytes);
		bool res = SendData(a_Socket, (const char *)Buffer, NumBytes, a_Peer);
		if (!res)
		{
			return false;
		}
		Data += NumBytes;
		a_Size -= NumBytes;
	}
	return true;
}





bool cConnection::SendEncryptedData(SOCKET a_Socket, Encryptor & a_Encryptor, cByteBuffer & a_Data, const char * a_Peer)
{
	AString All;
	a_Data.ReadAll(All);
	a_Data.CommitRead();
	return SendEncryptedData(a_Socket, a_Encryptor, All.data(), All.size(), a_Peer);
}





bool cConnection::DecodeClientsPackets(const char * a_Data, int a_Size)
{
	if (!m_ClientBuffer.Write(a_Data, a_Size))
	{
		Log("Too much queued data for the server, aborting connection");
		return false;
	}
	
	while (m_ClientBuffer.CanReadBytes(1))
	{
		UInt32 PacketLen;
		if (
			!m_ClientBuffer.ReadVarInt(PacketLen) ||
			!m_ClientBuffer.CanReadBytes(PacketLen)
		)
		{
			// Not a complete packet yet
			break;
		}
		UInt32 PacketType, PacketReadSoFar;
		PacketReadSoFar = m_ClientBuffer.GetReadableSpace();
		VERIFY(m_ClientBuffer.ReadVarInt(PacketType));
		PacketReadSoFar -= m_ClientBuffer.GetReadableSpace();
		Log("Decoding client's packets, there are now %d bytes in the queue; next packet is 0x%0x, %u bytes long", m_ClientBuffer.GetReadableSpace(), PacketType, PacketLen);
		switch (m_ClientProtocolState)
		{
			case -1:
			{
				// No initial handshake received yet
				switch (PacketType)
				{
					case 0x00: HANDLE_CLIENT_READ(HandleClientHandshake()); break;
					default:   HANDLE_CLIENT_READ(HandleClientUnknownPacket(PacketType, PacketLen, PacketReadSoFar)); break;
				}
				break;
			}  // case -1
			
			case 1:
			{
				// Status query
				switch (PacketType)
				{
					case 0x00: HANDLE_CLIENT_READ(HandleClientStatusRequest()); break;
					case 0x01: HANDLE_CLIENT_READ(HandleClientStatusPing()); break;
					default:   HANDLE_CLIENT_READ(HandleClientUnknownPacket(PacketType, PacketLen, PacketReadSoFar)); break;
				}
				break;
			}
			
			case 2:
			{
				// Login
				switch (PacketType)
				{
					case 0x00: HANDLE_CLIENT_READ(HandleClientLoginStart()); break;
					case 0x01: HANDLE_CLIENT_READ(HandleClientLoginEncryptionKeyResponse()); break;
					default:   HANDLE_CLIENT_READ(HandleClientUnknownPacket(PacketType, PacketLen, PacketReadSoFar)); break;
				}
				break;
			}
			
			case 3:
			{
				// Game:
				switch (PacketType)
				{
					case 0x00: HANDLE_CLIENT_READ(HandleClientKeepAlive()); break;
					case 0x01: HANDLE_CLIENT_READ(HandleClientChatMessage()); break;
					case 0x02: HANDLE_CLIENT_READ(HandleClientUseEntity()); break;
					case 0x03: HANDLE_CLIENT_READ(HandleClientPlayerOnGround()); break;
					case 0x04: HANDLE_CLIENT_READ(HandleClientPlayerPosition()); break;
					case 0x05: HANDLE_CLIENT_READ(HandleClientPlayerLook()); break;
					case 0x06: HANDLE_CLIENT_READ(HandleClientPlayerPositionLook()); break;
					case 0x07: HANDLE_CLIENT_READ(HandleClientBlockDig()); break;
					case 0x08: HANDLE_CLIENT_READ(HandleClientBlockPlace()); break;
					case 0x09: HANDLE_CLIENT_READ(HandleClientSlotSelect()); break;
					case 0x0a: HANDLE_CLIENT_READ(HandleClientAnimation()); break;
					case 0x0b: HANDLE_CLIENT_READ(HandleClientEntityAction()); break;
					case 0x0d: HANDLE_CLIENT_READ(HandleClientWindowClose()); break;
					case 0x0e: HANDLE_CLIENT_READ(HandleClientWindowClick()); break;
					case 0x10: HANDLE_CLIENT_READ(HandleClientCreativeInventoryAction()); break;
					case 0x12: HANDLE_CLIENT_READ(HandleClientUpdateSign()); break;
					case 0x13: HANDLE_CLIENT_READ(HandleClientPlayerAbilities()); break;
					case 0x14: HANDLE_CLIENT_READ(HandleClientTabCompletion()); break;
					case 0x15: HANDLE_CLIENT_READ(HandleClientLocaleAndView()); break;
					case 0x16: HANDLE_CLIENT_READ(HandleClientClientStatuses()); break;
					case 0x17: HANDLE_CLIENT_READ(HandleClientPluginMessage()); break;
					default:   HANDLE_CLIENT_READ(HandleClientUnknownPacket(PacketType, PacketLen, PacketReadSoFar)); break;
				}
				break;
			}  // case 3 - Game
			
			default:
			{
				Log("Receiving server packets while in an unknown protocol state (%d)!", m_ClientProtocolState);
				HANDLE_CLIENT_READ(HandleClientUnknownPacket(PacketType, PacketLen, PacketReadSoFar));
				break;
			}
		}  // switch (m_ProtocolState)
		m_ClientBuffer.CommitRead();
	}  // while (true)
	return true;
}





bool cConnection::DecodeServersPackets(const char * a_Data, int a_Size)
{
	if (!m_ServerBuffer.Write(a_Data, a_Size))
	{
		Log("Too much queued data for the client, aborting connection");
		return false;
	}
	
	if (
		(m_ServerState == csEncryptedUnderstood) &&
		(m_ClientState == csUnencrypted)
	)
	{
		// Client hasn't finished encryption handshake yet, don't send them any data yet
	}
	
	while (true)
	{
		UInt32 PacketLen;
		if (
			!m_ServerBuffer.ReadVarInt(PacketLen) ||
			!m_ServerBuffer.CanReadBytes(PacketLen)
		)
		{
			// Not a complete packet yet
			m_ServerBuffer.ResetRead();
			break;
		}
		if (PacketLen == 0)
		{
			m_ServerBuffer.ResetRead();
			AString All;
			m_ServerBuffer.ReadAll(All);
			DataLog(All.data(), All.size(), "====== Received a bad packet length? Inspect the contents below ======");
			m_ServerBuffer.CommitRead();  // Try to recover by marking everything as read
			break;
		}
		UInt32 PacketType, PacketReadSoFar;
		PacketReadSoFar = m_ServerBuffer.GetReadableSpace();
		VERIFY(m_ServerBuffer.ReadVarInt(PacketType));
		PacketReadSoFar -= m_ServerBuffer.GetReadableSpace();
		Log("Decoding server's packets, there are now %d bytes in the queue; next packet is 0x%0x, %u bytes long", m_ServerBuffer.GetReadableSpace(), PacketType, PacketLen);
		LogFlush();
		switch (m_ServerProtocolState)
		{
			case -1:
			{
				Log("Receiving data from the server without an initial handshake message!");
				HANDLE_SERVER_READ(HandleServerUnknownPacket(PacketType, PacketLen, PacketReadSoFar));
				break;
			}
			
			case 1:
			{
				// Status query:
				switch (PacketType)
				{
					case 0x00: HANDLE_SERVER_READ(HandleServerStatusResponse()); break;
					case 0x01: HANDLE_SERVER_READ(HandleServerStatusPing());     break;
					default:   HANDLE_SERVER_READ(HandleServerUnknownPacket(PacketType, PacketLen, PacketReadSoFar)); break;
				}
				break;
			}
			
			case 2:
			{
				// Login:
				switch (PacketType)
				{
					case 0x00: HANDLE_SERVER_READ(HandleServerLoginDisconnect()); break;
					case 0x01: HANDLE_SERVER_READ(HandleServerLoginEncryptionKeyRequest()); break;
					case 0x02: HANDLE_SERVER_READ(HandleServerLoginSuccess()); break;
					default:   HANDLE_SERVER_READ(HandleServerUnknownPacket(PacketType, PacketLen, PacketReadSoFar)); break;
				}
				break;
			}
			
			case 3:
			{
				// Game:
				switch (PacketType)
				{
					case 0x00: HANDLE_SERVER_READ(HandleServerKeepAlive()); break;
					case 0x01: HANDLE_SERVER_READ(HandleServerJoinGame()); break;
					case 0x02: HANDLE_SERVER_READ(HandleServerChatMessage()); break;
					case 0x03: HANDLE_SERVER_READ(HandleServerTimeUpdate()); break;
					case 0x04: HANDLE_SERVER_READ(HandleServerEntityEquipment()); break;
					case 0x05: HANDLE_SERVER_READ(HandleServerCompass()); break;
					case 0x06: HANDLE_SERVER_READ(HandleServerUpdateHealth()); break;
					case 0x07: HANDLE_SERVER_READ(HandleServerRespawn()); break;
					case 0x08: HANDLE_SERVER_READ(HandleServerPlayerPositionLook()); break;
					case 0x09: HANDLE_SERVER_READ(HandleServerSlotSelect()); break;
					case 0x0a: HANDLE_SERVER_READ(HandleServerUseBed()); break;
					case 0x0b: HANDLE_SERVER_READ(HandleServerPlayerAnimation()); break;
					case 0x0c: HANDLE_SERVER_READ(HandleServerSpawnNamedEntity()); break;
					case 0x0d: HANDLE_SERVER_READ(HandleServerCollectPickup()); break;
					case 0x0e: HANDLE_SERVER_READ(HandleServerSpawnObjectVehicle()); break;
					case 0x0f: HANDLE_SERVER_READ(HandleServerSpawnMob()); break;
					case 0x10: HANDLE_SERVER_READ(HandleServerSpawnPainting()); break;
					case 0x11: HANDLE_SERVER_READ(HandleServerSpawnExperienceOrbs()); break;
					case 0x12: HANDLE_SERVER_READ(HandleServerEntityVelocity()); break;
					case 0x13: HANDLE_SERVER_READ(HandleServerDestroyEntities()); break;
					case 0x14: HANDLE_SERVER_READ(HandleServerEntity()); break;
					case 0x15: HANDLE_SERVER_READ(HandleServerEntityRelativeMove()); break;
					case 0x16: HANDLE_SERVER_READ(HandleServerEntityLook()); break;
					case 0x17: HANDLE_SERVER_READ(HandleServerEntityRelativeMoveLook()); break;
					case 0x18: HANDLE_SERVER_READ(HandleServerEntityTeleport()); break;
					case 0x19: HANDLE_SERVER_READ(HandleServerEntityHeadLook()); break;
					case 0x1a: HANDLE_SERVER_READ(HandleServerEntityStatus()); break;
					case 0x1b: HANDLE_SERVER_READ(HandleServerAttachEntity()); break;
					case 0x1c: HANDLE_SERVER_READ(HandleServerEntityMetadata()); break;
					case 0x1f: HANDLE_SERVER_READ(HandleServerSetExperience()); break;
					case 0x20: HANDLE_SERVER_READ(HandleServerEntityProperties()); break;
					case 0x21: HANDLE_SERVER_READ(HandleServerMapChunk()); break;
					case 0x22: HANDLE_SERVER_READ(HandleServerMultiBlockChange()); break;
					case 0x23: HANDLE_SERVER_READ(HandleServerBlockChange()); break;
					case 0x24: HANDLE_SERVER_READ(HandleServerBlockAction()); break;
					case 0x26: HANDLE_SERVER_READ(HandleServerMapChunkBulk()); break;
					case 0x27: HANDLE_SERVER_READ(HandleServerExplosion()); break;
					case 0x28: HANDLE_SERVER_READ(HandleServerSoundEffect()); break;
					case 0x29: HANDLE_SERVER_READ(HandleServerNamedSoundEffect()); break;
					case 0x2b: HANDLE_SERVER_READ(HandleServerChangeGameState()); break;
					case 0x2d: HANDLE_SERVER_READ(HandleServerWindowOpen()); break;
					case 0x2e: HANDLE_SERVER_READ(HandleServerWindowClose()); break;
					case 0x2f: HANDLE_SERVER_READ(HandleServerSetSlot()); break;
					case 0x30: HANDLE_SERVER_READ(HandleServerWindowContents()); break;
					case 0x33: HANDLE_SERVER_READ(HandleServerUpdateSign()); break;
					case 0x35: HANDLE_SERVER_READ(HandleServerUpdateTileEntity()); break;
					case 0x37: HANDLE_SERVER_READ(HandleServerStatistics()); break;
					case 0x38: HANDLE_SERVER_READ(HandleServerPlayerListItem()); break;
					case 0x39: HANDLE_SERVER_READ(HandleServerPlayerAbilities()); break;
					case 0x3a: HANDLE_SERVER_READ(HandleServerTabCompletion()); break;
					case 0x3f: HANDLE_SERVER_READ(HandleServerPluginMessage()); break;
					case 0x40: HANDLE_SERVER_READ(HandleServerKick()); break;
					default:   HANDLE_SERVER_READ(HandleServerUnknownPacket(PacketType, PacketLen, PacketReadSoFar)); break;
				}  // switch (PacketType)
				break;
			}  // case 3 - Game
			
			// TODO: Move this elsewhere
			default:
			{
				Log("Received a packet from the server while in an unknown state: %d", m_ServerProtocolState);
				HANDLE_SERVER_READ(HandleServerUnknownPacket(PacketType, PacketLen, PacketReadSoFar));
				break;
			}
		}  // switch (m_ProtocolState)
		
		m_ServerBuffer.CommitRead();
	}  // while (CanReadBytes(1))
	return true;
}





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// packet handling, client-side, initial handshake:

bool cConnection::HandleClientHandshake(void)
{
	// Read the packet from the client:
	HANDLE_CLIENT_PACKET_READ(ReadVarInt,        UInt32,  ProtocolVersion);
	HANDLE_CLIENT_PACKET_READ(ReadVarUTF8String, AString, ServerHost);
	HANDLE_CLIENT_PACKET_READ(ReadBEShort,       short,   ServerPort);
	HANDLE_CLIENT_PACKET_READ(ReadVarInt,        UInt32,  NextState);
	m_ClientBuffer.CommitRead();
	
	Log("Received an initial handshake packet from the client:");
	Log("  ProtocolVersion = %u", ProtocolVersion);
	Log("  ServerHost = \"%s\"", ServerHost.c_str());
	Log("  ServerPort = %d", ServerPort);
	Log("  NextState = %u", NextState);

	// Send the same packet to the server, but with our port:
	cByteBuffer Packet(512);
	Packet.WriteVarInt(0);  // Packet type - initial handshake
	Packet.WriteVarInt(ProtocolVersion);
	Packet.WriteVarUTF8String(ServerHost);
	Packet.WriteBEShort(m_Server.GetConnectPort());
	Packet.WriteVarInt(NextState);
	AString Pkt;
	Packet.ReadAll(Pkt);
	cByteBuffer ToServer(512);
	ToServer.WriteVarUTF8String(Pkt);
	SERVERSEND(ToServer);
	
	m_ClientProtocolState = (int)NextState;
	m_ServerProtocolState = (int)NextState;
	
	return true;
}





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// packet handling, client-side, login:

bool cConnection::HandleClientLoginEncryptionKeyResponse(void)
{
	Log("Client: Unexpected packet: encryption key response");
	return true;
}





bool cConnection::HandleClientLoginStart(void)
{
	HANDLE_CLIENT_PACKET_READ(ReadVarUTF8String, AString, UserName);
	Log("Received a login start packet from the client:");
	Log("  Username = \"%s\"", UserName.c_str());
	COPY_TO_SERVER();
	return true;
}





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// packet handling, client-side, game:

bool cConnection::HandleClientAnimation(void)
{
	HANDLE_CLIENT_PACKET_READ(ReadBEInt, int,  EntityID);
	HANDLE_CLIENT_PACKET_READ(ReadChar,  char, Animation);
	Log("Received a PACKET_ANIMATION from the client:");
	Log("  EntityID: %d", EntityID);
	Log("  Animation: %d", Animation);
	COPY_TO_SERVER();
	return true;
}





bool cConnection::HandleClientBlockDig(void)
{
	HANDLE_CLIENT_PACKET_READ(ReadByte,  Byte, Status);
	HANDLE_CLIENT_PACKET_READ(ReadBEInt, int,  BlockX);
	HANDLE_CLIENT_PACKET_READ(ReadByte,  Byte, BlockY);
	HANDLE_CLIENT_PACKET_READ(ReadBEInt, int,  BlockZ);
	HANDLE_CLIENT_PACKET_READ(ReadByte,  Byte, BlockFace);
	Log("Received a PACKET_BLOCK_DIG from the client:");
	Log("  Status = %d", Status);
	Log("  Pos = <%d, %d, %d>", BlockX, BlockY, BlockZ);
	Log("  BlockFace = %d", BlockFace);
	COPY_TO_SERVER();
	return true;
}





bool cConnection::HandleClientBlockPlace(void)
{
	HANDLE_CLIENT_PACKET_READ(ReadBEInt, int,  BlockX);
	HANDLE_CLIENT_PACKET_READ(ReadByte,  Byte, BlockY);
	HANDLE_CLIENT_PACKET_READ(ReadBEInt, int,  BlockZ);
	HANDLE_CLIENT_PACKET_READ(ReadChar,  char, Face);
	AString Desc;
	if (!ParseSlot(m_ClientBuffer, Desc))
	{
		return false;
	}
	HANDLE_CLIENT_PACKET_READ(ReadChar,  char, CursorX);
	HANDLE_CLIENT_PACKET_READ(ReadChar,  char, CursorY);
	HANDLE_CLIENT_PACKET_READ(ReadChar,  char, CursorZ);
	Log("Received a PACKET_BLOCK_PLACE from the client:");
	Log("  Block = {%d, %d, %d}", BlockX, BlockY, BlockZ);
	Log("  Face = %d", Face);
	Log("  Item = %s", Desc.c_str());
	Log("  Cursor = <%d, %d, %d>", CursorX, CursorY, CursorZ);
	COPY_TO_SERVER();
	return true;
}





bool cConnection::HandleClientChatMessage(void)
{
	HANDLE_CLIENT_PACKET_READ(ReadVarUTF8String, AString, Message);
	Log("Received a PACKET_CHAT_MESSAGE from the client:");
	Log("  Message = \"%s\"", Message.c_str());
	COPY_TO_SERVER();
	return true;
}





bool cConnection::HandleClientClientStatuses(void)
{
	HANDLE_CLIENT_PACKET_READ(ReadChar, char, Statuses);
	Log("Received a PACKET_CLIENT_STATUSES from the CLIENT:");
	Log("  Statuses = %d", Statuses);
	
	COPY_TO_SERVER();
	return true;
}





bool cConnection::HandleClientCreativeInventoryAction(void)
{
	HANDLE_CLIENT_PACKET_READ(ReadBEShort, short, SlotNum);
	AString Item;
	if (!ParseSlot(m_ClientBuffer, Item))
	{
		return false;
	}
	Log("Received a PACKET_CREATIVE_INVENTORY_ACTION from the client:");
	Log("  SlotNum = %d", SlotNum);
	Log("  Item = %s", Item.c_str());
	COPY_TO_SERVER();
	return true;
}





bool cConnection::HandleClientDisconnect(void)
{
	HANDLE_CLIENT_PACKET_READ(ReadVarUTF8String, AString, Reason);
	Log("Received a PACKET_DISCONNECT from the client:");
	Log("  Reason = \"%s\"", Reason.c_str());
	COPY_TO_SERVER();
	return true;
}





bool cConnection::HandleClientEntityAction(void)
{
	HANDLE_CLIENT_PACKET_READ(ReadBEInt, int,  PlayerID);
	HANDLE_CLIENT_PACKET_READ(ReadByte,  Byte, ActionType);
	HANDLE_CLIENT_PACKET_READ(ReadBEInt, int,  HorseJumpBoost);
	Log("Received a PACKET_ENTITY_ACTION from the client:");
	Log("  PlayerID = %d", PlayerID);
	Log("  ActionType = %d", ActionType);
	Log("  HorseJumpBoost = %d (0x%08x)", HorseJumpBoost, HorseJumpBoost);
	COPY_TO_SERVER();
	return true;
}





bool cConnection::HandleClientKeepAlive(void)
{
	HANDLE_CLIENT_PACKET_READ(ReadBEInt, int, ID);
	Log("Received a PACKET_KEEPALIVE from the client");
	COPY_TO_SERVER();
	return true;
}





bool cConnection::HandleClientLocaleAndView(void)
{
	HANDLE_CLIENT_PACKET_READ(ReadVarUTF8String, AString, Locale);
	HANDLE_CLIENT_PACKET_READ(ReadChar,          char,    ViewDistance);
	HANDLE_CLIENT_PACKET_READ(ReadChar,          char,    ChatFlags);
	HANDLE_CLIENT_PACKET_READ(ReadChar,          char,    Unused);
	HANDLE_CLIENT_PACKET_READ(ReadChar,          char,    Difficulty);
	HANDLE_CLIENT_PACKET_READ(ReadChar,          char,    ShowCape);
	Log("Received a PACKET_LOCALE_AND_VIEW from the client");
	COPY_TO_SERVER();
	return true;
}





bool cConnection::HandleClientPing(void)
{
	m_HasClientPinged = true;
	Log("Received a PACKET_PING from the client");
	m_ClientBuffer.ResetRead();
	SERVERSEND(m_ClientBuffer);
	return true;
}





bool cConnection::HandleClientPlayerAbilities(void)
{
	HANDLE_CLIENT_PACKET_READ(ReadChar,    char, Flags);
	HANDLE_CLIENT_PACKET_READ(ReadBEFloat, float, FlyingSpeed);
	HANDLE_CLIENT_PACKET_READ(ReadBEFloat, float, WalkingSpeed);
	Log("Receives a PACKET_PLAYER_ABILITIES from the client:");
	Log("  Flags = %d (0x%02x)", Flags, Flags);
	Log("  FlyingSpeed = %f", FlyingSpeed);
	Log("  WalkingSpeed = %f", WalkingSpeed);
	COPY_TO_SERVER();
	return true;
}





bool cConnection::HandleClientPlayerLook(void)
{
	HANDLE_CLIENT_PACKET_READ(ReadBEFloat, float, Yaw);
	HANDLE_CLIENT_PACKET_READ(ReadBEFloat, float, Pitch);
	HANDLE_CLIENT_PACKET_READ(ReadChar,    char,  OnGround);
	Log("Received a PACKET_PLAYER_LOOK from the client");
	COPY_TO_SERVER();
	return true;
}





bool cConnection::HandleClientPlayerOnGround(void)
{
	HANDLE_CLIENT_PACKET_READ(ReadChar, char, OnGround);
	Log("Received a PACKET_PLAYER_ON_GROUND from the client");
	COPY_TO_SERVER();
	return true;
}





bool cConnection::HandleClientPlayerPosition(void)
{
	HANDLE_CLIENT_PACKET_READ(ReadBEDouble, double, PosX);
	HANDLE_CLIENT_PACKET_READ(ReadBEDouble, double, PosY);
	HANDLE_CLIENT_PACKET_READ(ReadBEDouble, double, Stance);
	HANDLE_CLIENT_PACKET_READ(ReadBEDouble, double, PosZ);
	HANDLE_CLIENT_PACKET_READ(ReadChar,     char,   IsOnGround);
	Log("Received a PACKET_PLAYER_POSITION from the client");

	// TODO: list packet contents
	
	COPY_TO_SERVER();
	return true;
}





bool cConnection::HandleClientPlayerPositionLook(void)
{
	HANDLE_CLIENT_PACKET_READ(ReadBEDouble, double, PosX);
	HANDLE_CLIENT_PACKET_READ(ReadBEDouble, double, PosY);
	HANDLE_CLIENT_PACKET_READ(ReadBEDouble, double, Stance);
	HANDLE_CLIENT_PACKET_READ(ReadBEDouble, double, PosZ);
	HANDLE_CLIENT_PACKET_READ(ReadBEFloat,  float,  Yaw);
	HANDLE_CLIENT_PACKET_READ(ReadBEFloat,  float,  Pitch);
	HANDLE_CLIENT_PACKET_READ(ReadChar,     char,   IsOnGround);
	Log("Received a PACKET_PLAYER_POSITION_LOOK from the client");
	Log("  Pos = {%.03f, %.03f, %.03f}", PosX, PosY, PosZ);
	Log("  Stance = %.03f", Stance);
	Log("  Yaw, Pitch = <%.03f, %.03f>", Yaw, Pitch);
	Log("  IsOnGround = %s", IsOnGround ? "true" : "false");
	
	COPY_TO_SERVER();
	return true;
}





bool cConnection::HandleClientPluginMessage(void)
{
	HANDLE_CLIENT_PACKET_READ(ReadVarUTF8String, AString, ChannelName);
	HANDLE_CLIENT_PACKET_READ(ReadBEShort,         short,   Length);
	AString Data;
	if (!m_ClientBuffer.ReadString(Data, Length))
	{
		return false;
	}
	Log("Received a PACKET_PLUGIN_MESSAGE from the client");
	Log("  ChannelName = \"%s\"", ChannelName.c_str());
	DataLog(Data.data(), Length, "  Data: %d bytes", Length);
	COPY_TO_SERVER();
	return true;
}





bool cConnection::HandleClientSlotSelect(void)
{
	HANDLE_CLIENT_PACKET_READ(ReadBEShort, short, SlotNum);
	Log("Received a PACKET_SLOT_SELECT from the client");
	Log("  SlotNum = %d", SlotNum);
	COPY_TO_SERVER();
	return true;
}





bool cConnection::HandleClientStatusPing(void)
{
	HANDLE_CLIENT_PACKET_READ(ReadBEInt64, Int64, Time);
	Log("Received the status ping packet from the client:");
	Log("  Time = %lld", Time);
	COPY_TO_SERVER();
	return true;
}





bool cConnection::HandleClientStatusRequest(void)
{
	Log("Received the status request packet from the client");
	COPY_TO_SERVER();
	return true;
}





bool cConnection::HandleClientTabCompletion(void)
{
	HANDLE_CLIENT_PACKET_READ(ReadVarUTF8String, AString, Query);
	Log("Received a PACKET_TAB_COMPLETION query from the client");
	Log("  Query = \"%s\"", Query.c_str());
	COPY_TO_SERVER();
	return true;
}





bool cConnection::HandleClientUpdateSign(void)
{
	HANDLE_CLIENT_PACKET_READ(ReadBEInt,         int,     BlockX);
	HANDLE_CLIENT_PACKET_READ(ReadBEShort,       short,   BlockY);
	HANDLE_CLIENT_PACKET_READ(ReadBEInt,         int,     BlockZ);
	HANDLE_CLIENT_PACKET_READ(ReadVarUTF8String, AString, Line1);
	HANDLE_CLIENT_PACKET_READ(ReadVarUTF8String, AString, Line2);
	HANDLE_CLIENT_PACKET_READ(ReadVarUTF8String, AString, Line3);
	HANDLE_CLIENT_PACKET_READ(ReadVarUTF8String, AString, Line4);
	Log("Received a PACKET_UPDATE_SIGN from the client:");
	Log("  Block = {%d, %d, %d}", BlockX, BlockY, BlockZ);
	Log("  Lines = \"%s\", \"%s\", \"%s\", \"%s\"", Line1.c_str(), Line2.c_str(), Line3.c_str(), Line4.c_str());
	COPY_TO_SERVER();
	return true;
}





bool cConnection::HandleClientUseEntity(void)
{
	HANDLE_CLIENT_PACKET_READ(ReadBEInt, int,  EntityID);
	HANDLE_CLIENT_PACKET_READ(ReadChar,  char, MouseButton);
	Log("Received a PACKET_USE_ENTITY from the client:");
	Log("  EntityID = %d", EntityID);
	Log("  MouseButton = %d", MouseButton);
	COPY_TO_SERVER();
	return true;
}





bool cConnection::HandleClientWindowClick(void)
{
	HANDLE_CLIENT_PACKET_READ(ReadChar,    char,  WindowID);
	HANDLE_CLIENT_PACKET_READ(ReadBEShort, short, SlotNum);
	HANDLE_CLIENT_PACKET_READ(ReadChar,    char,  Button);
	HANDLE_CLIENT_PACKET_READ(ReadBEShort, short, TransactionID);
	HANDLE_CLIENT_PACKET_READ(ReadChar,    char,  Mode);
	AString Item;
	if (!ParseSlot(m_ClientBuffer, Item))
	{
		return false;
	}
	Log("Received a PACKET_WINDOW_CLICK from the client");
	Log("  WindowID = %d", WindowID);
	Log("  SlotNum = %d", SlotNum);
	Log("  Button = %d, Mode = %d", Button, Mode);
	Log("  TransactionID = 0x%x", TransactionID);
	Log("  ClickedItem = %s", Item.c_str());
	COPY_TO_SERVER();
	return true;
}





bool cConnection::HandleClientWindowClose(void)
{
	HANDLE_CLIENT_PACKET_READ(ReadChar, char, WindowID);
	Log("Received a PACKET_WINDOW_CLOSE from the client:");
	Log("  WindowID = %d", WindowID);
	COPY_TO_SERVER();
	return true;
}





bool cConnection::HandleClientUnknownPacket(UInt32 a_PacketType, UInt32 a_PacketLen, UInt32 a_PacketReadSoFar)
{
	AString Data;
	if (!m_ClientBuffer.ReadString(Data, a_PacketLen - a_PacketReadSoFar))
	{
		return false;
	}
	DataLog(Data.data(), Data.size(), "****************** Unknown packet 0x%x from the client; relaying and ignoring", a_PacketType);
	COPY_TO_SERVER();
	return true;
}





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// packet handling, server-side, login:

bool cConnection::HandleServerLoginDisconnect(void)
{
	HANDLE_SERVER_PACKET_READ(ReadVarUTF8String, AString, Reason);
	Log("Received a login-disconnect packet from the server:");
	Log("  Reason = \"%s\"", Reason.c_str());
	COPY_TO_SERVER();
	return true;
}





bool cConnection::HandleServerLoginEncryptionKeyRequest(void)
{
	// Read the packet from the server:
	HANDLE_SERVER_PACKET_READ(ReadVarUTF8String, AString, ServerID);
	HANDLE_SERVER_PACKET_READ(ReadBEShort,       short,   PublicKeyLength);
	AString PublicKey;
	if (!m_ServerBuffer.ReadString(PublicKey, PublicKeyLength))
	{
		return false;
	}
	HANDLE_SERVER_PACKET_READ(ReadBEShort,       short,   NonceLength);
	AString Nonce;
	if (!m_ServerBuffer.ReadString(Nonce, NonceLength))
	{
		return false;
	}
	Log("Got PACKET_ENCRYPTION_KEY_REQUEST from the SERVER:");
	Log("  ServerID = %s", ServerID.c_str());
	
	// Reply to the server:
	SendEncryptionKeyResponse(PublicKey, Nonce);
	
	// Do not send to client - we want the client connection open
	return true;
}





bool cConnection::HandleServerLoginSuccess(void)
{
	HANDLE_SERVER_PACKET_READ(ReadVarUTF8String, AString, UUID);
	HANDLE_SERVER_PACKET_READ(ReadVarUTF8String, AString, Username);
	Log("Received a login success packet from the server:");
	Log("  UUID = \"%s\"", UUID.c_str());
	Log("  Username = \"%s\"", Username.c_str());
	
	Log("Server is now in protocol state Game.");
	m_ServerProtocolState = 3;
	
	if (m_IsServerEncrypted)
	{
		Log("Server communication is now encrypted");
		m_ServerState = csEncryptedUnderstood;
		DataLog(m_ServerEncryptionBuffer.data(), m_ServerEncryptionBuffer.size(), "Sending the queued data to server (%u bytes):", m_ServerEncryptionBuffer.size());
		SERVERENCRYPTSEND(m_ServerEncryptionBuffer.data(), m_ServerEncryptionBuffer.size());
		m_ServerEncryptionBuffer.clear();
	}
	COPY_TO_CLIENT();
	Log("Client is now in protocol state Game.");
	m_ClientProtocolState = 3;
	return true;
}





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// packet handling, server-side, game:

bool cConnection::HandleServerAttachEntity(void)
{
	HANDLE_SERVER_PACKET_READ(ReadBEInt, int,  EntityID);
	HANDLE_SERVER_PACKET_READ(ReadBEInt, int,  VehicleID);
	HANDLE_SERVER_PACKET_READ(ReadBool,  bool, Leash);
	Log("Received a PACKET_ATTACH_ENTITY from the server:");
	Log("  EntityID = %d (0x%x)", EntityID, EntityID);
	Log("  VehicleID = %d (0x%x)", VehicleID, VehicleID);
	Log("  Leash = %s", Leash ? "true" : "false");
	COPY_TO_CLIENT();
	return true;
}





bool cConnection::HandleServerBlockAction(void)
{
	HANDLE_SERVER_PACKET_READ(ReadBEInt,   int,    BlockX);
	HANDLE_SERVER_PACKET_READ(ReadBEShort, short,  BlockY);
	HANDLE_SERVER_PACKET_READ(ReadBEInt,   int,    BlockZ);
	HANDLE_SERVER_PACKET_READ(ReadByte,    Byte,   Byte1);
	HANDLE_SERVER_PACKET_READ(ReadByte,    Byte,   Byte2);
	HANDLE_SERVER_PACKET_READ(ReadVarInt,  UInt32, BlockID);
	Log("Received a PACKET_BLOCK_ACTION from the server:");
	Log("  Pos = {%d, %d, %d}", BlockX, BlockY, BlockZ);
	Log("  Bytes = (%d, %d) == (0x%x, 0x%x)", Byte1, Byte2, Byte1, Byte2);
	Log("  BlockID = %u", BlockID);
	COPY_TO_CLIENT();
	return true;
}





bool cConnection::HandleServerBlockChange(void)
{
	HANDLE_SERVER_PACKET_READ(ReadBEInt,   int,    BlockX);
	HANDLE_SERVER_PACKET_READ(ReadByte,    Byte,   BlockY);
	HANDLE_SERVER_PACKET_READ(ReadBEInt,   int,    BlockZ);
	HANDLE_SERVER_PACKET_READ(ReadVarInt,  UInt32, BlockType);
	HANDLE_SERVER_PACKET_READ(ReadChar,    char,   BlockMeta);
	Log("Received a PACKET_BLOCK_CHANGE from the server");
	COPY_TO_CLIENT();
	return true;
}





bool cConnection::HandleServerChangeGameState(void)
{
	HANDLE_SERVER_PACKET_READ(ReadChar,    char,  Reason);
	HANDLE_SERVER_PACKET_READ(ReadBEFloat, float, Data);
	Log("Received a PACKET_CHANGE_GAME_STATE from the server:");
	Log("  Reason = %d", Reason);
	Log("  Data = %f", Data);
	COPY_TO_CLIENT();
	return true;
}





bool cConnection::HandleServerChatMessage(void)
{
	HANDLE_SERVER_PACKET_READ(ReadVarUTF8String, AString, Message);
	Log("Received a PACKET_CHAT_MESSAGE from the server:");
	Log("  Message = \"%s\"", Message.c_str());
	COPY_TO_CLIENT();
	return true;
}





bool cConnection::HandleServerCollectPickup(void)
{
	HANDLE_SERVER_PACKET_READ(ReadBEInt, int, CollectedID);
	HANDLE_SERVER_PACKET_READ(ReadBEInt, int, CollectorID);
	Log("Received a PACKET_COLLECT_PICKUP from the server:");
	Log("  CollectedID = %d", CollectedID);
	Log("  CollectorID = %d", CollectorID);
	COPY_TO_CLIENT();
	return true;
}





bool cConnection::HandleServerCompass(void)
{
	HANDLE_SERVER_PACKET_READ(ReadBEInt, int, SpawnX);
	HANDLE_SERVER_PACKET_READ(ReadBEInt, int, SpawnY);
	HANDLE_SERVER_PACKET_READ(ReadBEInt, int, SpawnZ);
	Log("Received PACKET_COMPASS from the server:");
	Log("  Spawn = {%d, %d, %d}", SpawnX, SpawnY, SpawnZ);
	COPY_TO_CLIENT();
	return true;
}





bool cConnection::HandleServerDestroyEntities(void)
{
	HANDLE_SERVER_PACKET_READ(ReadByte, Byte, NumEntities);
	if (!m_ServerBuffer.SkipRead((int)NumEntities * 4))
	{
		return false;
	}
	Log("Received PACKET_DESTROY_ENTITIES from the server:");
	Log("  NumEntities = %d", NumEntities);
	COPY_TO_CLIENT();
	return true;
}





bool cConnection::HandleServerEntity(void)
{
	HANDLE_SERVER_PACKET_READ(ReadBEInt, int, EntityID);
	Log("Received a PACKET_ENTITY from the server:");
	Log("  EntityID = %d", EntityID);
	COPY_TO_CLIENT();
	return true;
}





bool cConnection::HandleServerEntityEquipment(void)
{
	HANDLE_SERVER_PACKET_READ(ReadBEInt,   int,   EntityID);
	HANDLE_SERVER_PACKET_READ(ReadBEShort, short, SlotNum);
	AString Item;
	if (!ParseSlot(m_ServerBuffer, Item))
	{
		return false;
	}
	Log("Received a PACKET_ENTITY_EQUIPMENT from the server:");
	Log("  EntityID = %d", EntityID);
	Log("  SlotNum = %d", SlotNum);
	Log("  Item = %s", Item.c_str());
	COPY_TO_CLIENT();
	return true;
}





bool cConnection::HandleServerEntityHeadLook(void)
{
	HANDLE_SERVER_PACKET_READ(ReadBEInt, int,  EntityID);
	HANDLE_SERVER_PACKET_READ(ReadByte,  Byte, HeadYaw);
	Log("Received a PACKET_ENTITY_HEAD_LOOK from the server:");
	Log("  EntityID = %d", EntityID);
	Log("  HeadYaw = %d", HeadYaw);
	COPY_TO_CLIENT();
	return true;
}





bool cConnection::HandleServerEntityLook(void)
{
	HANDLE_SERVER_PACKET_READ(ReadBEInt, int,  EntityID);
	HANDLE_SERVER_PACKET_READ(ReadByte,  Byte, Yaw);
	HANDLE_SERVER_PACKET_READ(ReadByte,  Byte, Pitch);
	Log("Received a PACKET_ENTITY_LOOK from the server:");
	Log("  EntityID = %d", EntityID);
	Log("  Yaw = %d", Yaw);
	Log("  Pitch = %d", Pitch);
	COPY_TO_CLIENT();
	return true;
}





bool cConnection::HandleServerEntityMetadata(void)
{
	HANDLE_SERVER_PACKET_READ(ReadBEInt, int, EntityID);
	AString Metadata;
	if (!ParseMetadata(m_ServerBuffer, Metadata))
	{
		return false;
	}
	AString HexDump;
	CreateHexDump(HexDump, Metadata.data(), Metadata.size(), 32);
	Log("Received a PACKET_ENTITY_METADATA from the server:");
	Log("  EntityID = %d", EntityID);
	Log("  Metadata, length = %d (0x%x):\n%s", Metadata.length(), Metadata.length(), HexDump.c_str());
	LogMetadata(Metadata, 4);
	COPY_TO_CLIENT();
	return true;
}





bool cConnection::HandleServerEntityProperties(void)
{
	HANDLE_SERVER_PACKET_READ(ReadBEInt, int, EntityID);
	HANDLE_SERVER_PACKET_READ(ReadBEInt, int, Count);
	Log("Received a PACKET_ENTITY_PROPERTIES from the server:");
	Log("  EntityID = %d", EntityID);
	Log("  Count = %d", Count);
	
	for (int i = 0; i < Count; i++)
	{
		HANDLE_SERVER_PACKET_READ(ReadVarUTF8String, AString, Key);
		HANDLE_SERVER_PACKET_READ(ReadBEDouble,      double,  Value);
		HANDLE_SERVER_PACKET_READ(ReadBEShort, short, ListLength);
		Log(" \"%s\" = %f; %d modifiers", Key.c_str(), Value, ListLength);
		for (short j = 0; j < ListLength; j++)
		{
			HANDLE_SERVER_PACKET_READ(ReadBEInt64,  Int64,  UUIDHi);
			HANDLE_SERVER_PACKET_READ(ReadBEInt64,  Int64,  UUIDLo);
			HANDLE_SERVER_PACKET_READ(ReadBEDouble, double, DblVal);
			HANDLE_SERVER_PACKET_READ(ReadByte,     Byte,   ByteVal);
			Log("  [%d] = {0x%08llx%08llx, %f, %u}", j, UUIDHi, UUIDLo, DblVal, ByteVal);
		}
	}  // for i
	COPY_TO_CLIENT();
	return true;
}





bool cConnection::HandleServerEntityRelativeMove(void)
{
	HANDLE_SERVER_PACKET_READ(ReadBEInt, int,  EntityID);
	HANDLE_SERVER_PACKET_READ(ReadByte,  Byte, dx);
	HANDLE_SERVER_PACKET_READ(ReadByte,  Byte, dy);
	HANDLE_SERVER_PACKET_READ(ReadByte,  Byte, dz);
	Log("Received a PACKET_ENTITY_RELATIVE_MOVE from the server:");
	Log("  EntityID = %d", EntityID);
	Log("  RelMove = %s", PrintableAbsIntTriplet(dx, dy, dz).c_str());
	COPY_TO_CLIENT();
	return true;
}





bool cConnection::HandleServerEntityRelativeMoveLook(void)
{
	HANDLE_SERVER_PACKET_READ(ReadBEInt, int, EntityID);
	HANDLE_SERVER_PACKET_READ(ReadByte,  Byte, dx);
	HANDLE_SERVER_PACKET_READ(ReadByte,  Byte, dy);
	HANDLE_SERVER_PACKET_READ(ReadByte,  Byte, dz);
	HANDLE_SERVER_PACKET_READ(ReadByte,  Byte, Yaw);
	HANDLE_SERVER_PACKET_READ(ReadByte,  Byte, Pitch);
	Log("Received a PACKET_ENTITY_RELATIVE_MOVE_LOOK from the server:");
	Log("  EntityID = %d", EntityID);
	Log("  RelMove = %s", PrintableAbsIntTriplet(dx, dy, dz).c_str());
	Log("  Yaw = %d", Yaw);
	Log("  Pitch = %d", Pitch);
	COPY_TO_CLIENT();
	return true;
}





bool cConnection::HandleServerEntityStatus(void)
{
	HANDLE_SERVER_PACKET_READ(ReadBEInt, int,  EntityID);
	HANDLE_SERVER_PACKET_READ(ReadByte,  Byte, Status);
	Log("Received a PACKET_ENTITY_STATUS from the server:");
	Log("  EntityID = %d", EntityID);
	Log("  Status = %d", Status);
	COPY_TO_CLIENT();
	return true;
}





bool cConnection::HandleServerEntityTeleport(void)
{
	HANDLE_SERVER_PACKET_READ(ReadBEInt, int,  EntityID);
	HANDLE_SERVER_PACKET_READ(ReadBEInt, int,  AbsX);
	HANDLE_SERVER_PACKET_READ(ReadBEInt, int,  AbsY);
	HANDLE_SERVER_PACKET_READ(ReadBEInt, int,  AbsZ);
	HANDLE_SERVER_PACKET_READ(ReadByte,  Byte, Yaw);
	HANDLE_SERVER_PACKET_READ(ReadByte,  Byte, Pitch);
	Log("Received a PACKET_ENTITY_TELEPORT from the server:");
	Log("  EntityID = %d", EntityID);
	Log("  Pos = %s", PrintableAbsIntTriplet(AbsX, AbsY, AbsZ).c_str());
	Log("  Yaw = %d", Yaw);
	Log("  Pitch = %d", Pitch);
	COPY_TO_CLIENT();
	return true;
}





bool cConnection::HandleServerEntityVelocity(void)
{
	HANDLE_SERVER_PACKET_READ(ReadBEInt,   int,   EntityID);
	HANDLE_SERVER_PACKET_READ(ReadBEShort, short, VelocityX);
	HANDLE_SERVER_PACKET_READ(ReadBEShort, short, VelocityY);
	HANDLE_SERVER_PACKET_READ(ReadBEShort, short, VelocityZ);
	Log("Received a PACKET_ENTITY_VELOCITY from the server:");
	Log("  EntityID = %d", EntityID);
	Log("  Velocity = %s", PrintableAbsIntTriplet(VelocityX, VelocityY, VelocityZ, 8000).c_str());
	COPY_TO_CLIENT();
	return true;
}





bool cConnection::HandleServerExplosion(void)
{
	HANDLE_SERVER_PACKET_READ(ReadBEFloat, float, PosX);
	HANDLE_SERVER_PACKET_READ(ReadBEFloat, float, PosY);
	HANDLE_SERVER_PACKET_READ(ReadBEFloat, float, PosZ);
	HANDLE_SERVER_PACKET_READ(ReadBEFloat, float, Force);
	HANDLE_SERVER_PACKET_READ(ReadBEInt,   int,   NumRecords);
	struct sCoords
	{
		int x, y, z;
		
		sCoords(int a_X, int a_Y, int a_Z) : x(a_X), y(a_Y), z(a_Z) {}
	} ;
	std::vector<sCoords> Records;
	Records.reserve(NumRecords);
	int PosXI = (int)PosX, PosYI = (int)PosY, PosZI = (int)PosZ;
	for (int i = 0; i < NumRecords; i++)
	{
		HANDLE_SERVER_PACKET_READ(ReadChar, char, rx);
		HANDLE_SERVER_PACKET_READ(ReadChar, char, ry);
		HANDLE_SERVER_PACKET_READ(ReadChar, char, rz);
		Records.push_back(sCoords(PosXI + rx, PosYI + ry, PosZI + rz));
	}
	HANDLE_SERVER_PACKET_READ(ReadBEFloat, float, PlayerMotionX);
	HANDLE_SERVER_PACKET_READ(ReadBEFloat, float, PlayerMotionY);
	HANDLE_SERVER_PACKET_READ(ReadBEFloat, float, PlayerMotionZ);
	Log("Received a PACKET_EXPLOSION from the server:");
	Log("  Pos = {%.02f, %.02f, %.02f}", PosX, PosY, PosZ);
	Log("  Force = %.02f", Force);
	Log("  NumRecords = %d", NumRecords);
	for (int i = 0; i < NumRecords; i++)
	{
		Log("    Records[%d] = {%d, %d, %d}", i, Records[i].x, Records[i].y, Records[i].z);
	}
	Log("  Player motion = <%.02f, %.02f, %.02f>", PlayerMotionX, PlayerMotionY, PlayerMotionZ);
	COPY_TO_CLIENT();
	return true;
}





bool cConnection::HandleServerIncrementStatistic(void)
{
	// 0xc8
	HANDLE_SERVER_PACKET_READ(ReadBEInt, int, StatisticID);
	HANDLE_SERVER_PACKET_READ(ReadBEInt, int, Amount);
	Log("Received a PACKET_INCREMENT_STATISTIC from the server:");
	Log("  StatisticID = %d (0x%x)", StatisticID, StatisticID);
	Log("  Amount = %d", Amount);
	COPY_TO_CLIENT();
	return true;
}





bool cConnection::HandleServerJoinGame(void)
{
	HANDLE_SERVER_PACKET_READ(ReadBEInt,         int,     EntityID);
	HANDLE_SERVER_PACKET_READ(ReadChar,          char,    GameMode);
	HANDLE_SERVER_PACKET_READ(ReadChar,          char,    Dimension);
	HANDLE_SERVER_PACKET_READ(ReadChar,          char,    Difficulty);
	HANDLE_SERVER_PACKET_READ(ReadChar,          char,    MaxPlayers);
	HANDLE_SERVER_PACKET_READ(ReadVarUTF8String, AString, LevelType);
	Log("Received a PACKET_LOGIN from the server:");
	Log("  EntityID = %d",      EntityID);
	Log("  GameMode = %d",      GameMode);
	Log("  Dimension = %d",     Dimension);
	Log("  Difficulty = %d",    Difficulty);
	Log("  MaxPlayers = %d",    MaxPlayers);
	Log("  LevelType = \"%s\"", LevelType.c_str());
	COPY_TO_CLIENT();
	return true;
}





bool cConnection::HandleServerKeepAlive(void)
{
	HANDLE_SERVER_PACKET_READ(ReadBEInt, int, PingID);
	Log("Received a PACKET_KEEP_ALIVE from the server:");
	Log("  ID = %d", PingID);
	COPY_TO_CLIENT()
	return true;
}





bool cConnection::HandleServerKick(void)
{
	HANDLE_SERVER_PACKET_READ(ReadVarUTF8String, AString, Reason);
	Log("Received PACKET_KICK from the SERVER:");
	if (m_HasClientPinged)
	{
		Log("  This was a std reply to client's PING");
		AStringVector Split;
		
		// Split by NULL chars (StringSplit() won't work here):
		size_t Last = 0;
		size_t Len = Reason.size();
		for (size_t i = 0; i < Len; i++)
		{
			if (Reason[i] == 0)
			{
				Split.push_back(Reason.substr(Last, i - Last));
				Last = i + 1;
			}
		}
		if (Last < Len)
		{
			Split.push_back(Reason.substr(Last));
		}
		
		if (Split.size() == 6)
		{
			Log("  Preamble: \"%s\"", Split[0].c_str());
			Log("  Protocol version: \"%s\"", Split[1].c_str());
			Log("  Server version: \"%s\"", Split[2].c_str());
			Log("  MOTD: \"%s\"", Split[3].c_str());
			Log("  Cur players: \"%s\"", Split[4].c_str());
			Log("  Max players: \"%s\"", Split[5].c_str());
			
			// Modify the MOTD to show that it's being ProtoProxied:
			Reason.assign(Split[0]);
			Reason.push_back(0);
			Reason.append(Split[1]);
			Reason.push_back(0);
			Reason.append(Split[2]);
			Reason.push_back(0);
			Reason.append(Printf("ProtoProxy: %s", Split[3].c_str()));
			Reason.push_back(0);
			Reason.append(Split[4]);
			Reason.push_back(0);
			Reason.append(Split[5]);
			AString ReasonBE16;
			UTF8ToRawBEUTF16(Reason.data(), Reason.size(), ReasonBE16);
			AString PacketStart("\xff");
			PacketStart.push_back((ReasonBE16.size() / 2) / 256);
			PacketStart.push_back((ReasonBE16.size() / 2) % 256);
			CLIENTSEND(PacketStart.data(), PacketStart.size());
			CLIENTSEND(ReasonBE16.data(), ReasonBE16.size());
			return true;
		}
		else
		{
			DataLog(Reason.data(), Reason.size(), "  Unknown reply format, dumping hex:");
		}
	}
	else
	{
		Log("  Reason = \"%s\"", Reason.c_str());
	}
	COPY_TO_CLIENT();
	return true;
}





bool cConnection::HandleServerMapChunk(void)
{
	HANDLE_SERVER_PACKET_READ(ReadBEInt,   int,   ChunkX);
	HANDLE_SERVER_PACKET_READ(ReadBEInt,   int,   ChunkZ);
	HANDLE_SERVER_PACKET_READ(ReadChar,    char,  IsContiguous);
	HANDLE_SERVER_PACKET_READ(ReadBEShort, short, PrimaryBitmap);
	HANDLE_SERVER_PACKET_READ(ReadBEShort, short, AdditionalBitmap);
	HANDLE_SERVER_PACKET_READ(ReadBEInt,   int,   CompressedSize);
	AString CompressedData;
	if (!m_ServerBuffer.ReadString(CompressedData, CompressedSize))
	{
		return false;
	}
	Log("Received a PACKET_MAP_CHUNK from the server:");
	Log("  ChunkPos = [%d, %d]", ChunkX, ChunkZ);
	Log("  Compressed size = %d (0x%x)", CompressedSize, CompressedSize);
	
	// TODO: Save the compressed data into a file for later analysis
	
	COPY_TO_CLIENT()
	return true;
}





bool cConnection::HandleServerMapChunkBulk(void)
{
	HANDLE_SERVER_PACKET_READ(ReadBEShort, short, ChunkCount);
	HANDLE_SERVER_PACKET_READ(ReadBEInt,   int,   CompressedSize);
	HANDLE_SERVER_PACKET_READ(ReadBool,    bool,  IsSkyLightSent);
	AString CompressedData;
	if (!m_ServerBuffer.ReadString(CompressedData, CompressedSize))
	{
		return false;
	}
	
	// Read individual chunk metas.
	// Need to read them first and only then start logging (in case we don't have the full packet yet)
	struct sChunkMeta
	{
		int m_ChunkX, m_ChunkZ;
		short m_PrimaryBitmap;
		short m_AddBitmap;
		sChunkMeta(int a_ChunkX, int a_ChunkZ, short a_PrimaryBitmap, short a_AddBitmap) :
			m_ChunkX(a_ChunkX), m_ChunkZ(a_ChunkZ), m_PrimaryBitmap(a_PrimaryBitmap), m_AddBitmap(a_AddBitmap)
		{
		}
	} ;
	typedef std::vector<sChunkMeta> sChunkMetas;
	sChunkMetas ChunkMetas;
	ChunkMetas.reserve(ChunkCount);
	for (short i = 0; i < ChunkCount; i++)
	{
		HANDLE_SERVER_PACKET_READ(ReadBEInt,   int,   ChunkX);
		HANDLE_SERVER_PACKET_READ(ReadBEInt,   int,   ChunkZ);
		HANDLE_SERVER_PACKET_READ(ReadBEShort, short, PrimaryBitmap);
		HANDLE_SERVER_PACKET_READ(ReadBEShort, short, AddBitmap);
		ChunkMetas.push_back(sChunkMeta(ChunkX, ChunkZ, PrimaryBitmap, AddBitmap));
	}
	
	Log("Received a PACKET_MAP_CHUNK_BULK from the server:");
	Log("  ChunkCount = %d", ChunkCount);
	Log("  Compressed size = %d (0x%x)", CompressedSize, CompressedSize);
	Log("  IsSkyLightSent = %s", IsSkyLightSent ? "true" : "false");
	
	// Log individual chunk coords:
	int idx = 0;
	for (sChunkMetas::iterator itr = ChunkMetas.begin(), end = ChunkMetas.end(); itr != end; ++itr, ++idx)
	{
		Log("  [%d]: [%d, %d], primary bitmap 0x%02x, add bitmap 0x%02x",
			idx, itr->m_ChunkX, itr->m_ChunkZ, itr->m_PrimaryBitmap, itr->m_AddBitmap
		);
	}  // for itr - ChunkMetas[]

	// TODO: Save the compressed data into a file for later analysis
	
	COPY_TO_CLIENT();
	Sleep(50);
	return true;
}





bool cConnection::HandleServerMultiBlockChange(void)
{
	HANDLE_SERVER_PACKET_READ(ReadBEInt,   int,   ChunkX);
	HANDLE_SERVER_PACKET_READ(ReadBEInt,   int,   ChunkZ);
	HANDLE_SERVER_PACKET_READ(ReadBEShort, short, NumBlocks);
	HANDLE_SERVER_PACKET_READ(ReadBEInt,   int,   DataSize);
	AString BlockChangeData;
	if (!m_ServerBuffer.ReadString(BlockChangeData, DataSize))
	{
		return false;
	}
	Log("Received a PACKET_MULTI_BLOCK_CHANGE packet from the server:");
	Log("  Chunk = [%d, %d]", ChunkX, ChunkZ);
	Log("  NumBlocks = %d", NumBlocks);
	COPY_TO_CLIENT();
	return true;
}





bool cConnection::HandleServerNamedSoundEffect(void)
{
	HANDLE_SERVER_PACKET_READ(ReadVarUTF8String, AString, SoundName);
	HANDLE_SERVER_PACKET_READ(ReadBEInt,         int,     PosX);
	HANDLE_SERVER_PACKET_READ(ReadBEInt,         int,     PosY);
	HANDLE_SERVER_PACKET_READ(ReadBEInt,         int,     PosZ);
	HANDLE_SERVER_PACKET_READ(ReadBEFloat,       float,   Volume);
	HANDLE_SERVER_PACKET_READ(ReadByte,          Byte,    Pitch);
	Log("Received a PACKET_NAMED_SOUND_EFFECT from the server:");
	Log("  SoundName = \"%s\"", SoundName.c_str());
	Log("  Pos = %s", PrintableAbsIntTriplet(PosX, PosY, PosZ, 8).c_str());
	Log("  Volume = %f", Volume);
	Log("  Pitch = %d", Pitch);
	COPY_TO_CLIENT();
	return true;
}





bool cConnection::HandleServerPlayerAbilities(void)
{
	HANDLE_SERVER_PACKET_READ(ReadChar, char, Flags);
	HANDLE_SERVER_PACKET_READ(ReadBEFloat, float, FlyingSpeed);
	HANDLE_SERVER_PACKET_READ(ReadBEFloat, float, WalkingSpeed);
	Log("Received a PACKET_PLAYER_ABILITIES from the server:");
	Log("  Flags = %d (0x%02x)", Flags, Flags);
	Log("  FlyingSpeed = %f", FlyingSpeed);
	Log("  WalkingSpeed = %f", WalkingSpeed);
	COPY_TO_CLIENT();
	return true;
}





bool cConnection::HandleServerPlayerAnimation(void)
{
	HANDLE_SERVER_PACKET_READ(ReadVarInt, UInt32, PlayerID);
	HANDLE_SERVER_PACKET_READ(ReadByte,   Byte,   AnimationID);
	Log("Received a PACKET_PLAYER_ANIMATION from the server:");
	Log("  PlayerID: %u (0x%x)", PlayerID, PlayerID);
	Log("  Animation: %d", AnimationID);
	COPY_TO_CLIENT();
	return true;
}





bool cConnection::HandleServerPlayerListItem(void)
{
	HANDLE_SERVER_PACKET_READ(ReadVarUTF8String, AString, PlayerName);
	HANDLE_SERVER_PACKET_READ(ReadChar,            char,    IsOnline);
	HANDLE_SERVER_PACKET_READ(ReadBEShort,         short,   Ping);
	Log("Received a PACKET_PLAYERLIST_ITEM from the server:");
	Log("  PlayerName = \"%s\"", PlayerName.c_str());
	Log("  Ping = %d", Ping);
	COPY_TO_CLIENT();
	return true;
}





bool cConnection::HandleServerPlayerPositionLook(void)
{
	HANDLE_SERVER_PACKET_READ(ReadBEDouble, double, PosX);
	HANDLE_SERVER_PACKET_READ(ReadBEDouble, double, PosY);
	HANDLE_SERVER_PACKET_READ(ReadBEDouble, double, PosZ);
	HANDLE_SERVER_PACKET_READ(ReadBEFloat,  float,  Yaw);
	HANDLE_SERVER_PACKET_READ(ReadBEFloat,  float,  Pitch);
	HANDLE_SERVER_PACKET_READ(ReadChar,     char,   IsOnGround);
	Log("Received a PACKET_PLAYER_POSITION_LOOK from the server");

	// TODO: list packet contents
	
	COPY_TO_CLIENT();
	return true;
}





bool cConnection::HandleServerPluginMessage(void)
{
	HANDLE_SERVER_PACKET_READ(ReadVarUTF8String, AString, ChannelName);
	HANDLE_SERVER_PACKET_READ(ReadBEShort,         short,   Length);
	AString Data;
	if (!m_ServerBuffer.ReadString(Data, Length))
	{
		return false;
	}
	Log("Received a PACKET_PLUGIN_MESSAGE from the server");
	Log("  ChannelName = \"%s\"", ChannelName.c_str());
	DataLog(Data.data(), Length, "  Data: %d bytes", Length);
	COPY_TO_CLIENT();
	return true;
}





bool cConnection::HandleServerRespawn(void)
{
	HANDLE_SERVER_PACKET_READ(ReadBEInt,         int,     Dimension);
	HANDLE_SERVER_PACKET_READ(ReadChar,          char,    Difficulty);
	HANDLE_SERVER_PACKET_READ(ReadChar,          char,    GameMode);
	HANDLE_SERVER_PACKET_READ(ReadVarUTF8String, AString, LevelType);
	Log("Received a respawn packet from the server:");
	Log("  Dimension = %d", Dimension);
	Log("  Difficulty = %d", Difficulty);
	Log("  GameMode = %d", GameMode);
	Log("  LevelType = \"%s\"", LevelType.c_str());
	COPY_TO_CLIENT();
	return true;
}





bool cConnection::HandleServerSetExperience(void)
{
	HANDLE_SERVER_PACKET_READ(ReadBEFloat, float, ExperienceBar);
	HANDLE_SERVER_PACKET_READ(ReadBEShort, short, Level);
	HANDLE_SERVER_PACKET_READ(ReadBEShort, short, TotalExperience);
	Log("Received a PACKET_SET_EXPERIENCE from the server:");
	Log("  ExperienceBar = %.05f", ExperienceBar);
	Log("  Level = %d", Level);
	Log("  TotalExperience = %d", TotalExperience);
	COPY_TO_CLIENT();
	return true;
}





bool cConnection::HandleServerSetSlot(void)
{
	HANDLE_SERVER_PACKET_READ(ReadChar,    char,  WindowID);
	HANDLE_SERVER_PACKET_READ(ReadBEShort, short, SlotNum);
	AString Item;
	if (!ParseSlot(m_ServerBuffer, Item))
	{
		return false;
	}
	Log("Received a PACKET_SET_SLOT from the server:");
	Log("  WindowID = %d", WindowID);
	Log("  SlotNum = %d", SlotNum);
	Log("  Item = %s", Item.c_str());
	COPY_TO_CLIENT();
	return true;
}





bool cConnection::HandleServerSlotSelect(void)
{
	HANDLE_SERVER_PACKET_READ(ReadByte, Byte, SlotNum);
	Log("Received a PACKET_SLOT_SELECT from the server:");
	Log("  SlotNum = %d", SlotNum);
	COPY_TO_CLIENT();
	return true;
}





bool cConnection::HandleServerSoundEffect(void)
{
	HANDLE_SERVER_PACKET_READ(ReadBEInt,   int,   EffectID);
	HANDLE_SERVER_PACKET_READ(ReadBEInt,   int,   PosX);
	HANDLE_SERVER_PACKET_READ(ReadByte,    Byte,  PosY);
	HANDLE_SERVER_PACKET_READ(ReadBEInt,   int,   PosZ);
	HANDLE_SERVER_PACKET_READ(ReadBEInt,   int,   Data);
	HANDLE_SERVER_PACKET_READ(ReadByte,    Byte,  NoVolumeDecrease);
	Log("Received a PACKET_SOUND_EFFECT from the server:");
	Log("  EffectID = %d", EffectID);
	Log("  Pos = {%d, %d, %d}", PosX, PosY, PosZ);
	Log("  Data = %d", Data);
	Log("  NoVolumeDecrease = %d", NoVolumeDecrease);
	COPY_TO_CLIENT();
	return true;
}





bool cConnection::HandleServerSpawnExperienceOrbs(void)
{
	HANDLE_SERVER_PACKET_READ(ReadVarInt,  UInt32, EntityID);
	HANDLE_SERVER_PACKET_READ(ReadBEInt,   int,    PosX);
	HANDLE_SERVER_PACKET_READ(ReadBEInt,   int,    PosY);
	HANDLE_SERVER_PACKET_READ(ReadBEInt,   int,    PosZ);
	HANDLE_SERVER_PACKET_READ(ReadBEShort, short,  Count);
	Log("Received a SPAWN_EXPERIENCE_ORBS packet from the server:");
	Log("  EntityID = %u (0x%x)", EntityID, EntityID);
	Log("  Pos = %s", PrintableAbsIntTriplet(PosX, PosY, PosZ).c_str());
	Log("  Count = %d", Count);
	COPY_TO_CLIENT();
	return true;
}





bool cConnection::HandleServerSpawnMob(void)
{
	HANDLE_SERVER_PACKET_READ(ReadVarInt,  UInt32, EntityID);
	HANDLE_SERVER_PACKET_READ(ReadChar,    char,   MobType);
	HANDLE_SERVER_PACKET_READ(ReadBEInt,   int,    PosX);
	HANDLE_SERVER_PACKET_READ(ReadBEInt,   int,    PosY);
	HANDLE_SERVER_PACKET_READ(ReadBEInt,   int,    PosZ);
	HANDLE_SERVER_PACKET_READ(ReadByte,    Byte,   Yaw);
	HANDLE_SERVER_PACKET_READ(ReadByte,    Byte,   Pitch);
	HANDLE_SERVER_PACKET_READ(ReadByte,    Byte,   HeadYaw);
	HANDLE_SERVER_PACKET_READ(ReadBEShort, short,  VelocityX);
	HANDLE_SERVER_PACKET_READ(ReadBEShort, short,  VelocityY);
	HANDLE_SERVER_PACKET_READ(ReadBEShort, short,  VelocityZ);
	AString Metadata;
	if (!ParseMetadata(m_ServerBuffer, Metadata))
	{
		return false;
	}
	AString HexDump;
	CreateHexDump(HexDump, Metadata.data(), Metadata.size(), 32);
	Log("Received a PACKET_SPAWN_MOB from the server:");
	Log("  EntityID = %u (0x%x)", EntityID, EntityID);
	Log("  MobType = %d", MobType);
	Log("  Pos = %s", PrintableAbsIntTriplet(PosX, PosY, PosZ).c_str());
	Log("  Angles = [%d, %d, %d]", Yaw, Pitch, HeadYaw);
	Log("  Velocity = %s", PrintableAbsIntTriplet(VelocityX, VelocityY, VelocityZ, 8000).c_str());
	Log("  Metadata, length = %d (0x%x):\n%s", Metadata.length(), Metadata.length(), HexDump.c_str());
	LogMetadata(Metadata, 4);
	COPY_TO_CLIENT();
	return true;
}





bool cConnection::HandleServerSpawnNamedEntity(void)
{
	HANDLE_SERVER_PACKET_READ(ReadVarInt,        UInt32,  EntityID);
	HANDLE_SERVER_PACKET_READ(ReadVarUTF8String, AString, EntityUUID);
	HANDLE_SERVER_PACKET_READ(ReadVarUTF8String, AString, EntityName);
	HANDLE_SERVER_PACKET_READ(ReadBEInt,         int,     PosX);
	HANDLE_SERVER_PACKET_READ(ReadBEInt,         int,     PosY);
	HANDLE_SERVER_PACKET_READ(ReadBEInt,         int,     PosZ);
	HANDLE_SERVER_PACKET_READ(ReadByte,          Byte,    Yaw);
	HANDLE_SERVER_PACKET_READ(ReadByte,          Byte,    Pitch);
	HANDLE_SERVER_PACKET_READ(ReadBEShort,       short,   CurrentItem);
	AString Metadata;
	if (!ParseMetadata(m_ServerBuffer, Metadata))
	{
		return false;
	}
	AString HexDump;
	CreateHexDump(HexDump, Metadata.data(), Metadata.size(), 32);
	Log("Received a PACKET_SPAWN_NAMED_ENTITY from the server:");
	Log("  EntityID = %u (0x%x)", EntityID, EntityID);
	Log("  UUID = \"%s\"", EntityUUID.c_str());
	Log("  Name = \"%s\"", EntityName.c_str());
	Log("  Pos = %s", PrintableAbsIntTriplet(PosX, PosY, PosZ).c_str());
	Log("  Rotation = <yaw %d, pitch %d>", Yaw, Pitch);
	Log("  CurrentItem = %d", CurrentItem);
	Log("  Metadata, length = %d (0x%x):\n%s", Metadata.length(), Metadata.length(), HexDump.c_str());
	LogMetadata(Metadata, 4);
	COPY_TO_CLIENT();
	return true;
}





bool cConnection::HandleServerSpawnObjectVehicle(void)
{
	#ifdef _DEBUG
	// DEBUG:
	// This packet is still troublesome when DataIndicator != 0
	AString Buffer;
	m_ServerBuffer.ResetRead();
	m_ServerBuffer.ReadAll(Buffer);
	m_ServerBuffer.ResetRead();
	UInt32 PacketLen, PacketType;
	m_ServerBuffer.ReadVarInt(PacketLen);
	m_ServerBuffer.ReadVarInt(PacketType);
	if (Buffer.size() > 128)
	{
		// Only log up to 128 bytes
		Buffer.erase(128, AString::npos);
	}
	DataLog(Buffer.data(), Buffer.size(), "Buffer while parsing the PACKET_SPAWN_OBJECT_VEHICLE packet (%d bytes):", Buffer.size());
	#endif  // _DEBUG
	
	HANDLE_SERVER_PACKET_READ(ReadVarInt,  UInt32, EntityID);
	HANDLE_SERVER_PACKET_READ(ReadByte,    Byte,   ObjType);
	HANDLE_SERVER_PACKET_READ(ReadBEInt,   int,    PosX);
	HANDLE_SERVER_PACKET_READ(ReadBEInt,   int,    PosY);
	HANDLE_SERVER_PACKET_READ(ReadBEInt,   int,    PosZ);
	HANDLE_SERVER_PACKET_READ(ReadByte,    Byte,   Pitch);
	HANDLE_SERVER_PACKET_READ(ReadByte,    Byte,   Yaw);
	HANDLE_SERVER_PACKET_READ(ReadBEInt,   int,    DataIndicator);
	AString ExtraData;
	short VelocityX, VelocityY, VelocityZ;
	if (DataIndicator != 0)
	{
		HANDLE_SERVER_PACKET_READ(ReadBEShort, short, SpeedX);
		HANDLE_SERVER_PACKET_READ(ReadBEShort, short, SpeedY);
		HANDLE_SERVER_PACKET_READ(ReadBEShort, short, SpeedZ);
		VelocityX = SpeedX; VelocityY = SpeedY; VelocityZ = SpeedZ;  // Speed vars are local to this scope, but we need them available later
		/*
		// This doesn't seem to work - for a falling block I'm getting no extra data at all
		int ExtraLen = 0;
		switch (ObjType)
		{
			case OBJECT_FALLING_BLOCK: ExtraLen = 4; break;  // int: BlockType | (BlockMeta << 12)
			case OBJECT_ARROW:
			case OBJECT_SNOWBALL:
			case OBJECT_EGG:
			case OBJECT_EYE_OF_ENDER:
			case OBJECT_DRAGON_EGG:
			case OBJECT_FISHING_FLOAT:
			{
				ExtraLen = 4; break;  // int: EntityID of the thrower
			}
			// TODO: Splash potions
		}
		if ((ExtraLen > 0) && !m_ServerBuffer.ReadString(ExtraData, ExtraLen))
		{
			return false;
		}
		*/
	}
	Log("Received a PACKET_SPAWN_OBJECT_VEHICLE from the server:");
	Log("  EntityID = %u (0x%x)", EntityID, EntityID);
	Log("  ObjType = %d (0x%x)", ObjType, ObjType);
	Log("  Pos = %s", PrintableAbsIntTriplet(PosX, PosY, PosZ).c_str());
	Log("  Rotation = <yaw %d, pitch %d>", Yaw, Pitch);
	Log("  DataIndicator = %d (0x%x)", DataIndicator, DataIndicator);
	if (DataIndicator != 0)
	{
		Log("  Velocity = %s", PrintableAbsIntTriplet(VelocityX, VelocityY, VelocityZ, 8000).c_str());
		DataLog(ExtraData.data(), ExtraData.size(), "  ExtraData size = %d:", ExtraData.size());
	}
	COPY_TO_CLIENT();
	return true;
}





bool cConnection::HandleServerSpawnPainting(void)
{
	HANDLE_SERVER_PACKET_READ(ReadVarInt,        UInt32,  EntityID);
	HANDLE_SERVER_PACKET_READ(ReadVarUTF8String, AString, ImageName);
	HANDLE_SERVER_PACKET_READ(ReadBEInt,         int,     PosX);
	HANDLE_SERVER_PACKET_READ(ReadBEInt,         int,     PosY);
	HANDLE_SERVER_PACKET_READ(ReadBEInt,         int,     PosZ);
	HANDLE_SERVER_PACKET_READ(ReadBEInt,         int,     Direction);
	Log("Received a PACKET_SPAWN_PAINTING from the server:");
	Log("  EntityID = %u", EntityID);
	Log("  ImageName = \"%s\"", ImageName.c_str());
	Log("  Pos = %s", PrintableAbsIntTriplet(PosX, PosY, PosZ).c_str());
	Log("  Direction = %d", Direction);
	COPY_TO_CLIENT();
	return true;
}





bool cConnection::HandleServerSpawnPickup(void)
{
	HANDLE_SERVER_PACKET_READ(ReadBEInt,   int,   EntityID);
	AString ItemDesc;
	if (!ParseSlot(m_ServerBuffer, ItemDesc))
	{
		return false;
	}
	HANDLE_SERVER_PACKET_READ(ReadBEInt,   int,   PosX);
	HANDLE_SERVER_PACKET_READ(ReadBEInt,   int,   PosY);
	HANDLE_SERVER_PACKET_READ(ReadBEInt,   int,   PosZ);
	HANDLE_SERVER_PACKET_READ(ReadByte,    Byte,  Rotation);
	HANDLE_SERVER_PACKET_READ(ReadByte,    Byte,  Pitch);
	HANDLE_SERVER_PACKET_READ(ReadByte,    Byte,  Roll);
	Log("Received a PACKET_SPAWN_PICKUP from the server:");
	Log("  EntityID = %d", EntityID);
	Log("  Item = %s", ItemDesc.c_str());
	Log("  Pos = %s", PrintableAbsIntTriplet(PosX, PosY, PosZ).c_str());
	Log("  Angles = [%d, %d, %d]", Rotation, Pitch, Roll);
	COPY_TO_CLIENT();
	return true;
}





bool cConnection::HandleServerStatistics(void)
{
	HANDLE_SERVER_PACKET_READ(ReadVarInt, UInt32, NumEntries);
	for (UInt32 i = 0; i < NumEntries; i++)
	{
		HANDLE_SERVER_PACKET_READ(ReadVarUTF8String, AString, StatName);
		HANDLE_SERVER_PACKET_READ(ReadVarInt,        UInt32,  StatValue);
	}
	COPY_TO_CLIENT();
	return true;
}




bool cConnection::HandleServerStatusPing(void)
{
	HANDLE_SERVER_PACKET_READ(ReadBEInt64, Int64, Time);
	Log("Received server's ping response:");
	Log("  Time = %lld", Time);
	COPY_TO_CLIENT();
	return true;
}





bool cConnection::HandleServerStatusResponse(void)
{
	HANDLE_SERVER_PACKET_READ(ReadVarUTF8String, AString, Response);
	Log("Received server's status response:");
	Log("  Response: %s", Response.c_str());
	
	// Modify the response to show that it's being proto-proxied:
	const char DescSearch[] = "\"description\":{\"text\":\"";
	size_t idx = Response.find(DescSearch);
	if (idx != AString::npos)
	{
		Response.assign(Response.substr(0, idx + sizeof(DescSearch) - 1) + "ProtoProxy: " + Response.substr(idx + sizeof(DescSearch) - 1));
	}
	cByteBuffer Packet(Response.size() + 50);
	Packet.WriteVarInt(0);  // Packet type - status response
	Packet.WriteVarUTF8String(Response);
	AString Pkt;
	Packet.ReadAll(Pkt);
	cByteBuffer ToClient(Response.size() + 50);
	ToClient.WriteVarUTF8String(Pkt);
	CLIENTSEND(ToClient);
	return true;
}





bool cConnection::HandleServerTabCompletion(void)
{
	HANDLE_SERVER_PACKET_READ(ReadVarInt,        UInt32,  NumResults);
	HANDLE_SERVER_PACKET_READ(ReadVarUTF8String, AString, Results);
	Log("Received a PACKET_TAB_COMPLETION from the server, results given:");

	// Parse the zero-terminated list of results:
	size_t len = Results.size();
	size_t last = 0;
	for (size_t i = 0; i < len; i++)
	{
		if (Results[i] == 0)
		{
			Log("  \"%s\"", Results.substr(last, i - last).c_str());
			last = i + 1;
		}
	}
	if (last < len)
	{
		Log("  \"%s\"", Results.substr(last, len - last).c_str());
	}
	COPY_TO_CLIENT();
	return true;
}





bool cConnection::HandleServerTimeUpdate(void)
{
	HANDLE_SERVER_PACKET_READ(ReadBEInt64, Int64, WorldAge);
	HANDLE_SERVER_PACKET_READ(ReadBEInt64, Int64, TimeOfDay);
	Log("Received a PACKET_TIME_UPDATE from the server");
	COPY_TO_CLIENT();
	return true;
}





bool cConnection::HandleServerUpdateHealth(void)
{
	HANDLE_SERVER_PACKET_READ(ReadBEFloat, float, Health);
	HANDLE_SERVER_PACKET_READ(ReadBEShort, short, Food);
	HANDLE_SERVER_PACKET_READ(ReadBEFloat, float, Saturation);
	Log("Received a PACKET_UPDATE_HEALTH from the server");
	COPY_TO_CLIENT();
	return true;
}





bool cConnection::HandleServerUpdateSign(void)
{
	HANDLE_SERVER_PACKET_READ(ReadBEInt,           int,   BlockX);
	HANDLE_SERVER_PACKET_READ(ReadBEShort,         short, BlockY);
	HANDLE_SERVER_PACKET_READ(ReadBEInt,           int,   BlockZ);
	HANDLE_SERVER_PACKET_READ(ReadVarUTF8String, AString, Line1);
	HANDLE_SERVER_PACKET_READ(ReadVarUTF8String, AString, Line2);
	HANDLE_SERVER_PACKET_READ(ReadVarUTF8String, AString, Line3);
	HANDLE_SERVER_PACKET_READ(ReadVarUTF8String, AString, Line4);
	Log("Received a PACKET_UPDATE_SIGN from the server:");
	Log("  Block = {%d, %d, %d}", BlockX, BlockY, BlockZ);
	Log("  Lines = \"%s\", \"%s\", \"%s\", \"%s\"", Line1.c_str(), Line2.c_str(), Line3.c_str(), Line4.c_str());
	COPY_TO_CLIENT();
	return true;
}





bool cConnection::HandleServerUpdateTileEntity(void)
{
	HANDLE_SERVER_PACKET_READ(ReadBEInt,   int,   BlockX);
	HANDLE_SERVER_PACKET_READ(ReadBEShort, short, BlockY);
	HANDLE_SERVER_PACKET_READ(ReadBEInt,   int,   BlockZ);
	HANDLE_SERVER_PACKET_READ(ReadByte,    Byte,  Action);
	HANDLE_SERVER_PACKET_READ(ReadBEShort, short, DataLength);
	AString Data;
	if ((DataLength > 0) && !m_ServerBuffer.ReadString(Data, DataLength))
	{
		return false;
	}
	Log("Received a PACKET_UPDATE_TILE_ENTITY from the server:");
	Log("  Block = {%d, %d, %d}", BlockX, BlockY, BlockZ);
	Log("  Action = %d", Action);
	DataLog(Data.data(), Data.size(), "  Data (%u bytes)", Data.size());
	COPY_TO_CLIENT();
	return true;
}





bool cConnection::HandleServerUseBed(void)
{
	HANDLE_SERVER_PACKET_READ(ReadBEInt, int,  EntityID);
	HANDLE_SERVER_PACKET_READ(ReadBEInt, int,  BedX);
	HANDLE_SERVER_PACKET_READ(ReadByte,  Byte, BedY);
	HANDLE_SERVER_PACKET_READ(ReadBEInt, int,  BedZ);
	Log("Received a use bed packet from the server:");
	Log("  EntityID = %d", EntityID);
	Log("  Bed = {%d, %d, %d}", BedX, BedY, BedZ);
	COPY_TO_CLIENT();
	return true;
}





bool cConnection::HandleServerWindowClose(void)
{
	HANDLE_SERVER_PACKET_READ(ReadChar, char, WindowID);
	Log("Received a PACKET_WINDOW_CLOSE from the server:");
	Log("  WindowID = %d", WindowID);
	COPY_TO_CLIENT();
	return true;
}





bool cConnection::HandleServerWindowContents(void)
{
	HANDLE_SERVER_PACKET_READ(ReadChar, char, WindowID);
	HANDLE_SERVER_PACKET_READ(ReadBEShort, short, NumSlots);
	Log("Received a PACKET_WINDOW_CONTENTS from the server:");
	Log("  WindowID = %d", WindowID);
	Log("  NumSlots = %d", NumSlots);
	AStringVector Items;
	for (short i = 0; i < NumSlots; i++)
	{
		AString Item;
		if (!ParseSlot(m_ServerBuffer, Item))
		{
			return false;
		}
		Log("    %d: %s", i, Item.c_str());
	}

	COPY_TO_CLIENT();
	return true;
}





bool cConnection::HandleServerWindowOpen(void)
{
	HANDLE_SERVER_PACKET_READ(ReadChar,            char,    WindowID);
	HANDLE_SERVER_PACKET_READ(ReadChar,            char,    WindowType);
	HANDLE_SERVER_PACKET_READ(ReadVarUTF8String, AString, Title);
	HANDLE_SERVER_PACKET_READ(ReadByte,            Byte,    NumSlots);
	HANDLE_SERVER_PACKET_READ(ReadByte,            Byte,    UseProvidedTitle);
	int HorseEntityID = 0;
	if (WindowType == 11)  // Horse / Donkey / Mule
	{
		HANDLE_SERVER_PACKET_READ(ReadBEInt, int, intHorseInt);
		HorseEntityID = intHorseInt;
	}
	Log("Received a PACKET_WINDOW_OPEN from the server:");
	Log("  WindowID = %d", WindowID);
	Log("  WindowType = %d", WindowType);
	Log("  Title = \"%s\", Use = %d", Title.c_str(), UseProvidedTitle);
	Log("  NumSlots = %d", NumSlots);
	if (WindowType == 11)
	{
		Log("  HorseEntityID = %d (0x%08x)", HorseEntityID, HorseEntityID);
	}
	COPY_TO_CLIENT();
	return true;
}





bool cConnection::HandleServerUnknownPacket(UInt32 a_PacketType, UInt32 a_PacketLen, UInt32 a_PacketReadSoFar)
{
	AString Data;
	ASSERT(a_PacketLen >= a_PacketReadSoFar);
	if (!m_ServerBuffer.ReadString(Data, a_PacketLen - a_PacketReadSoFar))
	{
		return false;
	}
	DataLog(Data.data(), Data.size(), "****************** Unknown packet 0x%x from the server; relaying and ignoring", a_PacketType);
	COPY_TO_CLIENT();
	return true;
}





bool cConnection::ParseSlot(cByteBuffer & a_Buffer, AString & a_ItemDesc)
{
	short ItemType;
	if (!a_Buffer.ReadBEShort(ItemType))
	{
		return false;
	}
	if (ItemType <= 0)
	{
		a_ItemDesc = "<empty>";
		return true;
	}
	if (!a_Buffer.CanReadBytes(5))
	{
		return false;
	}
	char ItemCount;
	short ItemDamage;
	short MetadataLength;
	a_Buffer.ReadChar(ItemCount);
	a_Buffer.ReadBEShort(ItemDamage);
	a_Buffer.ReadBEShort(MetadataLength);
	Printf(a_ItemDesc, "%d:%d * %d", ItemType, ItemDamage, ItemCount);
	if (MetadataLength <= 0)
	{
		return true;
	}
	AString Metadata;
	Metadata.resize(MetadataLength);
	if (!a_Buffer.ReadBuf((void *)Metadata.data(), MetadataLength))
	{
		return false;
	}
	AString MetaHex;
	CreateHexDump(MetaHex, Metadata.data(), Metadata.size(), 16);
	AppendPrintf(a_ItemDesc, "; %d bytes of meta:\n%s", MetadataLength, MetaHex.c_str());

	// Save metadata to a file:
	AString fnam;
	Printf(fnam, "%s_item_%08x.nbt", m_LogNameBase.c_str(), m_ItemIdx++);
	FILE * f = fopen(fnam.c_str(), "wb");
	if (f != NULL)
	{
		fwrite(Metadata.data(), 1, Metadata.size(), f);
		fclose(f);
		AppendPrintf(a_ItemDesc, "\n    (saved to file \"%s\")", fnam.c_str());
	}
	
	return true;
}





bool cConnection::ParseMetadata(cByteBuffer & a_Buffer, AString & a_Metadata)
{
	char x;
	if (!a_Buffer.ReadChar(x))
	{
		return false;
	}
	a_Metadata.push_back(x);
	while (x != 0x7f)
	{
		int Index = ((unsigned)((unsigned char)x)) & 0x1f;  // Lower 5 bits = index
		int Type  = ((unsigned)((unsigned char)x)) >> 5;    // Upper 3 bits = type
		int Length = 0;
		switch (Type)
		{
			case 0: Length = 1; break;  // byte
			case 1: Length = 2; break;  // short
			case 2: Length = 4; break;  // int
			case 3: Length = 4; break;  // float
			case 4:  // UTF-8 string with VarInt length
			{
				UInt32 Len;
				int rs = a_Buffer.GetReadableSpace();
				if (!a_Buffer.ReadVarInt(Len))
				{
					return false;
				}
				rs = rs - a_Buffer.GetReadableSpace();
				cByteBuffer LenBuf(8);
				LenBuf.WriteVarInt(Len);
				AString VarLen;
				LenBuf.ReadAll(VarLen);
				a_Metadata.append(VarLen);
				Length = Len;
				break;
			}
			case 5:
			{
				int Before = a_Buffer.GetReadableSpace();
				AString ItemDesc;
				if (!ParseSlot(a_Buffer, ItemDesc))
				{
					return false;
				}
				int After = a_Buffer.GetReadableSpace();
				a_Buffer.ResetRead();
				a_Buffer.SkipRead(a_Buffer.GetReadableSpace() - Before);
				Length = Before - After;
				break;
			}
			case 6: Length = 12; break;  // 3 * int
			case 7:
			default:
			{
				ASSERT(!"Unknown metadata type");
				break;
			}
		}  // switch (Type)
		AString data;
		if (!a_Buffer.ReadString(data, Length))
		{
			return false;
		}
		a_Metadata.append(data);
		if (!a_Buffer.ReadChar(x))
		{
			return false;
		}
		a_Metadata.push_back(x);
	}  // while (x != 0x7f)
	return true;
}





void cConnection::LogMetadata(const AString & a_Metadata, size_t a_IndentCount)
{
	AString Indent(a_IndentCount, ' ');
	int pos = 0;
	while (a_Metadata[pos] != 0x7f)
	{
		int Index = ((unsigned)((unsigned char)a_Metadata[pos])) & 0x1f;  // Lower 5 bits = index
		int Type  = ((unsigned)((unsigned char)a_Metadata[pos])) >> 5;    // Upper 3 bits = type
		int Length = 0;
		switch (Type)
		{
			case 0:
			{
				Log("%sbyte[%d] = %d", Indent.c_str(), Index, a_Metadata[pos + 1]);
				pos += 1;
				break;
			}
			case 1:
			{
				Log("%sshort[%d] = %d", Indent.c_str(), Index, (a_Metadata[pos + 1] << 8) | a_Metadata[pos + 2]);
				pos += 2;
				break;
			}
			case 2:
			{
				Log("%sint[%d] = %d", Indent.c_str(), Index, (a_Metadata[pos + 1] << 24) | (a_Metadata[pos + 2] << 16) | (a_Metadata[pos + 3] << 8) | a_Metadata[pos + 4]);
				pos += 4;
				break;
			}
			case 3:
			{
				Log("%sfloat[%d] = 0x%x", Indent.c_str(), Index, (a_Metadata[pos + 1] << 24) | (a_Metadata[pos + 2] << 16) | (a_Metadata[pos + 3] << 8) | a_Metadata[pos + 4]);
				pos += 4;
				break;
			}
			case 4:  // UTF-8 string with VarInt length
			{
				cByteBuffer bb(10);
				int RestLen = (int)a_Metadata.size() - pos - 1;
				if (RestLen > 8)
				{
					RestLen = 8;
				}
				bb.Write(a_Metadata.data() + pos + 1, RestLen);
				UInt32 Length;
				int rs = bb.GetReadableSpace();
				bb.ReadVarInt(Length);
				rs = rs - bb.GetReadableSpace();
				Log("%sstring[%d] = \"%*s\"", Indent.c_str(), Index, Length, a_Metadata.c_str() + pos + rs + 1);
				pos += Length + rs + 2;
				break;
			}
			case 5:
			{
				int BytesLeft = a_Metadata.size() - pos - 1;
				cByteBuffer bb(BytesLeft);
				bb.Write(a_Metadata.data() + pos + 1, BytesLeft);
				AString ItemDesc;
				if (!ParseSlot(bb, ItemDesc))
				{
					ASSERT(!"Cannot parse item description from metadata");
					return;
				}
				int After = bb.GetReadableSpace();
				int BytesConsumed = BytesLeft - bb.GetReadableSpace();

				Log("%sslot[%d] = %s (%d bytes)", Indent.c_str(), Index, ItemDesc.c_str(), BytesConsumed);
				pos += BytesConsumed;
				break;
			}
			case 6:
			{
				Log("%spos[%d] = <%d, %d, %d>", Indent.c_str(), Index, 
					(a_Metadata[pos + 1] << 24) | (a_Metadata[pos + 2]  << 16) | (a_Metadata[pos + 3]  << 8) | a_Metadata[pos + 4],
					(a_Metadata[pos + 5] << 24) | (a_Metadata[pos + 6]  << 16) | (a_Metadata[pos + 7]  << 8) | a_Metadata[pos + 8],
					(a_Metadata[pos + 9] << 24) | (a_Metadata[pos + 10] << 16) | (a_Metadata[pos + 11] << 8) | a_Metadata[pos + 12]
				);
				pos += 12;
				break;
			}
			default:
			{
				ASSERT(!"Unknown metadata type");
				break;
			}
		}  // switch (Type)
		pos += 1;
	}  // while (x != 0x7f)
}





void cConnection::SendEncryptionKeyResponse(const AString & a_ServerPublicKey, const AString & a_Nonce)
{
	// Generate the shared secret and encrypt using the server's public key
	byte SharedSecret[16];
	byte EncryptedSecret[128];
	memset(SharedSecret, 0, sizeof(SharedSecret));  // Use all zeroes for the initial secret
	RSA::PublicKey pk;
	CryptoPP::StringSource src(a_ServerPublicKey, true);
	ByteQueue bq;
	src.TransferTo(bq);
	bq.MessageEnd();
	pk.Load(bq);
	RSAES<PKCS1v15>::Encryptor rsaEncryptor(pk);
	RandomPool rng;
	time_t CurTime = time(NULL);
	rng.Put((const byte *)&CurTime, sizeof(CurTime));
	int EncryptedLength = rsaEncryptor.FixedCiphertextLength();
	ASSERT(EncryptedLength <= sizeof(EncryptedSecret));
	rsaEncryptor.Encrypt(rng, SharedSecret, sizeof(SharedSecret), EncryptedSecret);
	m_ServerEncryptor.SetKey(SharedSecret, 16, MakeParameters(Name::IV(), ConstByteArrayParameter(SharedSecret, 16))(Name::FeedbackSize(), 1));
	m_ServerDecryptor.SetKey(SharedSecret, 16, MakeParameters(Name::IV(), ConstByteArrayParameter(SharedSecret, 16))(Name::FeedbackSize(), 1));
	
	// Encrypt the nonce:
	byte EncryptedNonce[128];
	rsaEncryptor.Encrypt(rng, (const byte *)(a_Nonce.data()), a_Nonce.size(), EncryptedNonce);
	
	// Send the packet to the server:
	Log("Sending PACKET_ENCRYPTION_KEY_RESPONSE to the SERVER");
	cByteBuffer ToServer(1024);
	ToServer.WriteByte(0x01);  // To server: Encryption key response
	ToServer.WriteBEShort(EncryptedLength);
	ToServer.WriteBuf(EncryptedSecret, EncryptedLength);
	ToServer.WriteBEShort(EncryptedLength);
	ToServer.WriteBuf(EncryptedNonce, EncryptedLength);
	SERVERSEND(ToServer);
	m_ServerState = csEncryptedUnderstood;
	m_IsServerEncrypted = true;
}




