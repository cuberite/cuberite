
// Connection.cpp

// Interfaces to the cConnection class representing a single pair of connected sockets

#include "Globals.h"
#include "Connection.h"
#include "Server.h"
#include <iostream>





#ifdef _DEBUG
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
#define CLIENTENCRYPTSEND(...) SendEncryptedData(m_ClientSocket, m_ClientEncryptor, __VA_ARGS__, "Client")
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
			case csWaitingForEncryption: \
			{ \
				Log("Waiting for client encryption, queued %u bytes", ToClient.size()); \
				m_ClientEncryptionBuffer.append(ToClient.data(), ToClient.size()); \
				break; \
			} \
		} \
		DebugSleep(50); \
	}

#define HANDLE_CLIENT_READ(Proc) \
	{ \
		if (!Proc()) \
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
		if (!Proc()) \
		{ \
			m_ServerBuffer.ResetRead(); \
			return true; \
		} \
	}
	
	
#define MAX_ENC_LEN 1024




typedef unsigned char Byte;





enum
{
	PACKET_KEEPALIVE                 = 0x00,
	PACKET_LOGIN                     = 0x01,
	PACKET_HANDSHAKE                 = 0x02,
	PACKET_CHAT_MESSAGE              = 0x03,
	PACKET_TIME_UPDATE               = 0x04,
	PACKET_ENTITY_EQUIPMENT          = 0x05,
	PACKET_COMPASS                   = 0x06,
	PACKET_USE_ENTITY                = 0x07,
	PACKET_UPDATE_HEALTH             = 0x08,
	PACKET_PLAYER_ON_GROUND          = 0x0a,
	PACKET_PLAYER_POSITION           = 0x0b,
	PACKET_PLAYER_LOOK               = 0x0c,
	PACKET_PLAYER_POSITION_LOOK      = 0x0d,
	PACKET_BLOCK_DIG                 = 0x0e,
	PACKET_BLOCK_PLACE               = 0x0f,
	PACKET_SLOT_SELECT               = 0x10,
	PACKET_PLAYER_ANIMATION          = 0x12,
	PACKET_ENTITY_ACTION             = 0x13,
	PACKET_SPAWN_NAMED_ENTITY        = 0x14,
	PACKET_SPAWN_PICKUP              = 0x15,
	PACKET_COLLECT_PICKUP            = 0x16,
	PACKET_SPAWN_OBJECT_VEHICLE      = 0x17,
	PACKET_SPAWN_MOB                 = 0x18,
	PACKET_SPAWN_PAINTING            = 0x19,
	PACKET_SPAWN_EXPERIENCE_ORB      = 0x1a,
	PACKET_ENTITY_VELOCITY           = 0x1c,
	PACKET_DESTROY_ENTITIES          = 0x1d,
	PACKET_ENTITY                    = 0x1e,
	PACKET_ENTITY_RELATIVE_MOVE      = 0x1f,
	PACKET_ENTITY_LOOK               = 0x20,
	PACKET_ENTITY_RELATIVE_MOVE_LOOK = 0x21,
	PACKET_ENTITY_TELEPORT           = 0x22,
	PACKET_ENTITY_HEAD_LOOK          = 0x23,
	PACKET_ENTITY_STATUS             = 0x26,
	PACKET_ATTACH_ENTITY             = 0x27,
	PACKET_ENTITY_METADATA           = 0x28,
	PACKET_ENTITY_EFFECT             = 0x29,
	PACKET_ENTITY_EFFECT_REMOVE      = 0x2a,
	PACKET_SET_EXPERIENCE            = 0x2b,
	PACKET_ENTITY_PROPERTIES         = 0x2c,
	PACKET_MAP_CHUNK                 = 0x33,
	PACKET_MULTI_BLOCK_CHANGE        = 0x34,
	PACKET_BLOCK_CHANGE              = 0x35,
	PACKET_BLOCK_ACTION              = 0x36,
	PACKET_MAP_CHUNK_BULK            = 0x38,
	PACKET_SOUND_EFFECT              = 0x3d,
	PACKET_NAMED_SOUND_EFFECT        = 0x3e,
	PACKET_CHANGE_GAME_STATE         = 0x46,
	PACKET_WINDOW_OPEN               = 0x64,
	PACKET_WINDOW_CLOSE              = 0x65,
	PACKET_WINDOW_CLICK              = 0x66,
	PACKET_SET_SLOT                  = 0x67,
	PACKET_WINDOW_CONTENTS           = 0x68,
	PACKET_CREATIVE_INVENTORY_ACTION = 0x6b,
	PACKET_UPDATE_SIGN               = 0x82,
	PACKET_UPDATE_TILE_ENTITY        = 0x84,
	PACKET_PLAYER_LIST_ITEM          = 0xc9,
	PACKET_PLAYER_ABILITIES          = 0xca,
	PACKET_INCREMENT_STATISTIC       = 0xc8,
	PACKET_TAB_COMPLETION            = 0xcb,
	PACKET_LOCALE_AND_VIEW           = 0xcc,
	PACKET_CLIENT_STATUSES           = 0xcd,
	PACKET_PLUGIN_MESSAGE            = 0xfa,
	PACKET_ENCRYPTION_KEY_RESPONSE   = 0xfc,
	PACKET_ENCRYPTION_KEY_REQUEST    = 0xfd,
	PACKET_PING                      = 0xfe,
	PACKET_KICK                      = 0xff,
} ;




enum
{
	OBJECT_BOAT             = 1,
	OBJECT_MINECART         = 10,
	OBJECT_MINECART_STORAGE = 11,
	OBJECT_MINECART_POWERED = 12,
	OBJECT_TNT              = 50,
	OBJECT_ENDERCRYSTAL     = 51,
	OBJECT_ARROW            = 60,
	OBJECT_SNOWBALL         = 61,
	OBJECT_EGG              = 62,
	OBJECT_FALLING_BLOCK    = 70,
	OBJECT_EYE_OF_ENDER     = 72,
	OBJECT_DRAGON_EGG       = 74,
	OBJECT_FISHING_FLOAT    = 90,
} ;





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cConnection:

cConnection::cConnection(SOCKET a_ClientSocket, cServer & a_Server) :
	m_ItemIdx(0),
	m_LogFile(NULL),
	m_Server(a_Server),
	m_ClientSocket(a_ClientSocket),
	m_ServerSocket(-1),
	m_BeginTick(clock()),
	m_ClientState(csUnencrypted),
	m_ServerState(csUnencrypted),
	m_Nonce(0),
	m_ClientBuffer(1024 KiB),
	m_ServerBuffer(1024 KiB),
	m_HasClientPinged(false)
{
	Printf(m_LogNameBase, "Log_%d", (int)time(NULL));
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
			m_ClientEncryptor.ProcessData((byte *)Buffer, (byte *)Buffer, res);
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
			m_ClientDecryptor.ProcessData((byte *)Buffer, (byte *)Buffer, res);
			DataLog(Buffer, res, "Decrypted %d bytes from the CLIENT", res);
			return DecodeClientsPackets(Buffer, res);
		}
		case csEncryptedUnknown:
		{
			m_ClientDecryptor.ProcessData((byte *)Buffer, (byte *)Buffer, res);
			DataLog(Buffer, res, "Decrypted %d bytes from the CLIENT", res);
			m_ServerEncryptor.ProcessData((byte *)Buffer, (byte *)Buffer, res);
			return SERVERSEND(Buffer, res);
		}
	}
	
	return true;
}





double cConnection::GetRelativeTime(void)
{
	return (double)(clock() - m_BeginTick) / CLOCKS_PER_SEC;
	
}





bool cConnection::SendData(SOCKET a_Socket, const char * a_Data, int a_Size, const char * a_Peer)
{
	DataLog(a_Data, a_Size, "Sending data to %s", a_Peer);
	
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
		Log("Decoding client's packets, there are now %d bytes in the queue", m_ClientBuffer.GetReadableSpace());
		unsigned char PacketType;
		m_ClientBuffer.ReadByte(PacketType);
		switch (PacketType)
		{
			case PACKET_BLOCK_DIG:                 HANDLE_CLIENT_READ(HandleClientBlockDig); break;
			case PACKET_BLOCK_PLACE:               HANDLE_CLIENT_READ(HandleClientBlockPlace); break;
			case PACKET_CHAT_MESSAGE:              HANDLE_CLIENT_READ(HandleClientChatMessage); break;
			case PACKET_CLIENT_STATUSES:           HANDLE_CLIENT_READ(HandleClientClientStatuses); break;
			case PACKET_CREATIVE_INVENTORY_ACTION: HANDLE_CLIENT_READ(HandleClientCreativeInventoryAction); break;
			case PACKET_ENCRYPTION_KEY_RESPONSE:   HANDLE_CLIENT_READ(HandleClientEncryptionKeyResponse); break;
			case PACKET_ENTITY_ACTION:             HANDLE_CLIENT_READ(HandleClientEntityAction); break;
			case PACKET_HANDSHAKE:                 HANDLE_CLIENT_READ(HandleClientHandshake); break;
			case PACKET_KEEPALIVE:                 HANDLE_CLIENT_READ(HandleClientKeepAlive); break;
			case PACKET_LOCALE_AND_VIEW:           HANDLE_CLIENT_READ(HandleClientLocaleAndView); break;
			case PACKET_PING:                      HANDLE_CLIENT_READ(HandleClientPing); break;
			case PACKET_PLAYER_ABILITIES:          HANDLE_CLIENT_READ(HandleClientPlayerAbilities); break;
			case PACKET_PLAYER_ANIMATION:          HANDLE_CLIENT_READ(HandleClientAnimation); break;
			case PACKET_PLAYER_LOOK:               HANDLE_CLIENT_READ(HandleClientPlayerLook); break;
			case PACKET_PLAYER_ON_GROUND:          HANDLE_CLIENT_READ(HandleClientPlayerOnGround); break;
			case PACKET_PLAYER_POSITION:           HANDLE_CLIENT_READ(HandleClientPlayerPosition); break;
			case PACKET_PLAYER_POSITION_LOOK:      HANDLE_CLIENT_READ(HandleClientPlayerPositionLook); break;
			case PACKET_PLUGIN_MESSAGE:            HANDLE_CLIENT_READ(HandleClientPluginMessage); break;
			case PACKET_SLOT_SELECT:               HANDLE_CLIENT_READ(HandleClientSlotSelect); break;
			case PACKET_TAB_COMPLETION:            HANDLE_CLIENT_READ(HandleClientTabCompletion); break;
			case PACKET_UPDATE_SIGN:               HANDLE_CLIENT_READ(HandleClientUpdateSign); break;
			case PACKET_USE_ENTITY:                HANDLE_CLIENT_READ(HandleClientUseEntity); break;
			case PACKET_WINDOW_CLICK:              HANDLE_CLIENT_READ(HandleClientWindowClick); break;
			case PACKET_WINDOW_CLOSE:              HANDLE_CLIENT_READ(HandleClientWindowClose); break;
			default:
			{
				if (m_ClientState == csEncryptedUnderstood)
				{
					Log("****************** Unknown packet 0x%02x from the client while encrypted; continuing to relay blind only", PacketType);
					AString Data;
					m_ClientBuffer.ResetRead();
					m_ClientBuffer.ReadAll(Data);
					DataLog(Data.data(), Data.size(), "Current data in the client packet queue: %d bytes", Data.size());
					m_ClientState = csEncryptedUnknown;
					m_ClientBuffer.ResetRead();
					if (m_ServerState == csUnencrypted)
					{
						SERVERSEND(m_ClientBuffer);
					}
					else
					{
						SERVERENCRYPTSEND(m_ClientBuffer);
					}
					return true;
				}
				else
				{
					Log("Unknown packet 0x%02x from the client while unencrypted; aborting connection", PacketType);
					return false;
				}
			}
		}  // switch (PacketType)
		m_ClientBuffer.CommitRead();
	}  // while (CanReadBytes(1))
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
	
	while (m_ServerBuffer.CanReadBytes(1))
	{
		unsigned char PacketType;
		m_ServerBuffer.ReadByte(PacketType);
		Log("Decoding server's packets, there are now %d bytes in the queue; next packet is 0x%x", m_ServerBuffer.GetReadableSpace(), PacketType);
		LogFlush();
		switch (PacketType)
		{
			case PACKET_ATTACH_ENTITY:             HANDLE_SERVER_READ(HandleServerAttachEntity); break;
			case PACKET_BLOCK_ACTION:              HANDLE_SERVER_READ(HandleServerBlockAction); break;
			case PACKET_BLOCK_CHANGE:              HANDLE_SERVER_READ(HandleServerBlockChange); break;
			case PACKET_CHANGE_GAME_STATE:         HANDLE_SERVER_READ(HandleServerChangeGameState); break;
			case PACKET_CHAT_MESSAGE:              HANDLE_SERVER_READ(HandleServerChatMessage); break;
			case PACKET_COLLECT_PICKUP:            HANDLE_SERVER_READ(HandleServerCollectPickup); break;
			case PACKET_COMPASS:                   HANDLE_SERVER_READ(HandleServerCompass); break;
			case PACKET_DESTROY_ENTITIES:          HANDLE_SERVER_READ(HandleServerDestroyEntities); break;
			case PACKET_ENCRYPTION_KEY_REQUEST:    HANDLE_SERVER_READ(HandleServerEncryptionKeyRequest); break;
			case PACKET_ENCRYPTION_KEY_RESPONSE:   HANDLE_SERVER_READ(HandleServerEncryptionKeyResponse); break;
			case PACKET_ENTITY:                    HANDLE_SERVER_READ(HandleServerEntity); break;
			case PACKET_ENTITY_EQUIPMENT:          HANDLE_SERVER_READ(HandleServerEntityEquipment); break;
			case PACKET_ENTITY_HEAD_LOOK:          HANDLE_SERVER_READ(HandleServerEntityHeadLook); break;
			case PACKET_ENTITY_LOOK:               HANDLE_SERVER_READ(HandleServerEntityLook); break;
			case PACKET_ENTITY_METADATA:           HANDLE_SERVER_READ(HandleServerEntityMetadata); break;
			case PACKET_ENTITY_PROPERTIES:         HANDLE_SERVER_READ(HandleServerEntityProperties); break;
			case PACKET_ENTITY_RELATIVE_MOVE:      HANDLE_SERVER_READ(HandleServerEntityRelativeMove); break;
			case PACKET_ENTITY_RELATIVE_MOVE_LOOK: HANDLE_SERVER_READ(HandleServerEntityRelativeMoveLook); break;
			case PACKET_ENTITY_STATUS:             HANDLE_SERVER_READ(HandleServerEntityStatus); break;
			case PACKET_ENTITY_TELEPORT:           HANDLE_SERVER_READ(HandleServerEntityTeleport); break;
			case PACKET_ENTITY_VELOCITY:           HANDLE_SERVER_READ(HandleServerEntityVelocity); break;
			case PACKET_INCREMENT_STATISTIC:       HANDLE_SERVER_READ(HandleServerIncrementStatistic); break;
			case PACKET_KEEPALIVE:                 HANDLE_SERVER_READ(HandleServerKeepAlive); break;
			case PACKET_KICK:                      HANDLE_SERVER_READ(HandleServerKick); break;
			case PACKET_LOGIN:                     HANDLE_SERVER_READ(HandleServerLogin); break;
			case PACKET_MAP_CHUNK:                 HANDLE_SERVER_READ(HandleServerMapChunk); break;
			case PACKET_MAP_CHUNK_BULK:            HANDLE_SERVER_READ(HandleServerMapChunkBulk); break;
			case PACKET_MULTI_BLOCK_CHANGE:        HANDLE_SERVER_READ(HandleServerMultiBlockChange); break;
			case PACKET_NAMED_SOUND_EFFECT:        HANDLE_SERVER_READ(HandleServerNamedSoundEffect); break;
			case PACKET_PLAYER_ABILITIES:          HANDLE_SERVER_READ(HandleServerPlayerAbilities); break;
			case PACKET_PLAYER_ANIMATION:          HANDLE_SERVER_READ(HandleServerPlayerAnimation); break;
			case PACKET_PLAYER_LIST_ITEM:          HANDLE_SERVER_READ(HandleServerPlayerListItem); break;
			case PACKET_PLAYER_POSITION_LOOK:      HANDLE_SERVER_READ(HandleServerPlayerPositionLook); break;
			case PACKET_PLUGIN_MESSAGE:            HANDLE_SERVER_READ(HandleServerPluginMessage); break;
			case PACKET_SET_EXPERIENCE:            HANDLE_SERVER_READ(HandleServerSetExperience); break;
			case PACKET_SET_SLOT:                  HANDLE_SERVER_READ(HandleServerSetSlot); break;
			case PACKET_SLOT_SELECT:               HANDLE_SERVER_READ(HandleServerSlotSelect); break;
			case PACKET_SOUND_EFFECT:              HANDLE_SERVER_READ(HandleServerSoundEffect); break;
			case PACKET_SPAWN_MOB:                 HANDLE_SERVER_READ(HandleServerSpawnMob); break;
			case PACKET_SPAWN_NAMED_ENTITY:        HANDLE_SERVER_READ(HandleServerSpawnNamedEntity); break;
			case PACKET_SPAWN_OBJECT_VEHICLE:      HANDLE_SERVER_READ(HandleServerSpawnObjectVehicle); break;
			case PACKET_SPAWN_PAINTING:            HANDLE_SERVER_READ(HandleServerSpawnPainting); break;
			case PACKET_SPAWN_PICKUP:              HANDLE_SERVER_READ(HandleServerSpawnPickup); break;
			case PACKET_TAB_COMPLETION:            HANDLE_SERVER_READ(HandleServerTabCompletion); break;
			case PACKET_TIME_UPDATE:               HANDLE_SERVER_READ(HandleServerTimeUpdate); break;
			case PACKET_UPDATE_HEALTH:             HANDLE_SERVER_READ(HandleServerUpdateHealth); break;
			case PACKET_UPDATE_SIGN:               HANDLE_SERVER_READ(HandleServerUpdateSign); break;
			case PACKET_UPDATE_TILE_ENTITY:        HANDLE_SERVER_READ(HandleServerUpdateTileEntity); break;
			case PACKET_WINDOW_CLOSE:              HANDLE_SERVER_READ(HandleServerWindowClose); break;
			case PACKET_WINDOW_CONTENTS:           HANDLE_SERVER_READ(HandleServerWindowContents); break;
			case PACKET_WINDOW_OPEN:               HANDLE_SERVER_READ(HandleServerWindowOpen); break;
			default:
			{
				if (m_ServerState == csEncryptedUnderstood)
				{
					Log("********************** Unknown packet 0x%02x from the server while encrypted; continuing to relay blind only", PacketType);
					AString Data;
					m_ServerBuffer.ResetRead();
					m_ServerBuffer.ReadAll(Data);
					DataLog(Data.data(), Data.size(), "Current data in the server packet queue: %d bytes", Data.size());
					m_ServerState = csEncryptedUnknown;
					m_ServerBuffer.ResetRead();
					if (m_ClientState == csUnencrypted)
					{
						CLIENTSEND(m_ServerBuffer);
					}
					else
					{
						CLIENTENCRYPTSEND(m_ServerBuffer);
					}
					return true;
				}
				else
				{
					Log("Unknown packet 0x%02x from the server while unencrypted; aborting connection", PacketType);
					return false;
				}
			}
		}  // switch (PacketType)
		m_ServerBuffer.CommitRead();
	}  // while (CanReadBytes(1))
	return true;
}





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
	HANDLE_CLIENT_PACKET_READ(ReadBEUTF16String16, AString, Message);
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





bool cConnection::HandleClientEncryptionKeyResponse(void)
{
	HANDLE_CLIENT_PACKET_READ(ReadBEShort, short, EncKeyLength);
	AString EncKey;
	if (!m_ClientBuffer.ReadString(EncKey, EncKeyLength))
	{
		return true;
	}
	HANDLE_CLIENT_PACKET_READ(ReadBEShort, short, EncNonceLength);
	AString EncNonce;
	if (!m_ClientBuffer.ReadString(EncNonce, EncNonceLength))
	{
		return true;
	}
	if ((EncKeyLength > MAX_ENC_LEN) || (EncNonceLength > MAX_ENC_LEN))
	{
		Log("Client: Too long encryption params");
		return true;
	}
	StartClientEncryption(EncKey, EncNonce);
	return true;
}





bool cConnection::HandleClientEntityAction(void)
{
	HANDLE_CLIENT_PACKET_READ(ReadBEInt, int,  PlayerID);
	HANDLE_CLIENT_PACKET_READ(ReadByte,  Byte, ActionType);
	HANDLE_CLIENT_PACKET_READ(ReadBEInt, int,  UnknownHorseVal);
	Log("Received a PACKET_ENTITY_ACTION from the client:");
	Log("  PlayerID = %d", PlayerID);
	Log("  ActionType = %d", ActionType);
	Log("  UnknownHorseVal = %d (0x%08x)", UnknownHorseVal, UnknownHorseVal);
	COPY_TO_SERVER();
	return true;
}





bool cConnection::HandleClientHandshake(void)
{
	// Read the packet from the client:
	HANDLE_CLIENT_PACKET_READ(ReadByte,            Byte,    ProtocolVersion);
	HANDLE_CLIENT_PACKET_READ(ReadBEUTF16String16, AString, Username);
	HANDLE_CLIENT_PACKET_READ(ReadBEUTF16String16, AString, ServerHost);
	HANDLE_CLIENT_PACKET_READ(ReadBEInt,           int,     ServerPort);
	m_ClientBuffer.CommitRead();
	
	Log("Received a PACKET_HANDSHAKE from the client:");
	Log("  ProtocolVersion = %d", ProtocolVersion);
	Log("  Username = \"%s\"", Username.c_str());
	Log("  ServerHost = \"%s\"", ServerHost.c_str());
	Log("  ServerPort = %d", ServerPort);

	// Send the same packet to the server, but with our port:
	cByteBuffer ToServer(512);
	ToServer.WriteByte           (PACKET_HANDSHAKE);
	ToServer.WriteByte           (ProtocolVersion);
	ToServer.WriteBEUTF16String16(Username);
	ToServer.WriteBEUTF16String16(ServerHost);
	ToServer.WriteBEInt          (m_Server.GetConnectPort());
	SERVERSEND(ToServer);
	
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
	HANDLE_CLIENT_PACKET_READ(ReadBEUTF16String16, AString, Locale);
	HANDLE_CLIENT_PACKET_READ(ReadChar,            char,    ViewDistance);
	HANDLE_CLIENT_PACKET_READ(ReadChar,            char,    ChatFlags);
	HANDLE_CLIENT_PACKET_READ(ReadChar,            char,    Difficulty);
	HANDLE_CLIENT_PACKET_READ(ReadChar,            char,    ShowCape);
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
	HANDLE_CLIENT_PACKET_READ(ReadChar, char, Flags);
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
	HANDLE_CLIENT_PACKET_READ(ReadChar,    char,  OnGround);
	Log("Received a PACKET_PLAYER_ON_GROUND from the client");
	COPY_TO_SERVER();
	return true;
}





bool cConnection::HandleClientPlayerPosition(void)
{
	HANDLE_CLIENT_PACKET_READ(ReadBEDouble, double, PosX);
	HANDLE_CLIENT_PACKET_READ(ReadBEDouble, double, Stance);
	HANDLE_CLIENT_PACKET_READ(ReadBEDouble, double, PosY);
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
	HANDLE_CLIENT_PACKET_READ(ReadBEDouble, double, Stance);
	HANDLE_CLIENT_PACKET_READ(ReadBEDouble, double, PosY);
	HANDLE_CLIENT_PACKET_READ(ReadBEDouble, double, PosZ);
	HANDLE_CLIENT_PACKET_READ(ReadBEFloat,  float,  Yaw);
	HANDLE_CLIENT_PACKET_READ(ReadBEFloat,  float,  Pitch);
	HANDLE_CLIENT_PACKET_READ(ReadChar,     char,   IsOnGround);
	Log("Received a PACKET_PLAYER_POSITION_LOOK from the client");
	Log("  Pos = {%.03f, %.03f, %.03f}", PosX, PosY, PosZ);
	Log("  Stance = %.03f", Stance);
	Log("  Y, P = %.03f, %.03f", Yaw, Pitch);
	Log("  IsOnGround = %s", IsOnGround ? "true" : "false");
	
	COPY_TO_SERVER();
	return true;
}





bool cConnection::HandleClientPluginMessage(void)
{
	HANDLE_CLIENT_PACKET_READ(ReadBEUTF16String16, AString, ChannelName);
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





bool cConnection::HandleClientTabCompletion(void)
{
	HANDLE_CLIENT_PACKET_READ(ReadBEUTF16String16, AString, Query);
	Log("Received a PACKET_TAB_COMPLETION query from the client");
	Log("  Query = \"%s\"", Query.c_str());
	COPY_TO_SERVER();
	return true;
}





bool cConnection::HandleClientUpdateSign(void)
{
	HANDLE_CLIENT_PACKET_READ(ReadBEInt,           int,   BlockX);
	HANDLE_CLIENT_PACKET_READ(ReadBEShort,         short, BlockY);
	HANDLE_CLIENT_PACKET_READ(ReadBEInt,           int,   BlockZ);
	HANDLE_CLIENT_PACKET_READ(ReadBEUTF16String16, AString, Line1);
	HANDLE_CLIENT_PACKET_READ(ReadBEUTF16String16, AString, Line2);
	HANDLE_CLIENT_PACKET_READ(ReadBEUTF16String16, AString, Line3);
	HANDLE_CLIENT_PACKET_READ(ReadBEUTF16String16, AString, Line4);
	Log("Received a PACKET_UPDATE_SIGN from the client:");
	Log("  Block = {%d, %d, %d}", BlockX, BlockY, BlockZ);
	Log("  Lines = \"%s\", \"%s\", \"%s\", \"%s\"", Line1.c_str(), Line2.c_str(), Line3.c_str(), Line4.c_str());
	COPY_TO_SERVER();
	return true;
}





bool cConnection::HandleClientUseEntity(void)
{
	HANDLE_CLIENT_PACKET_READ(ReadBEInt, int,  PlayerID);
	HANDLE_CLIENT_PACKET_READ(ReadBEInt, int,  EntityID);
	HANDLE_CLIENT_PACKET_READ(ReadChar,  char, MouseButton);
	Log("Received a PACKET_USE_ENTITY from the client:");
	Log("  PlayerID = %d", PlayerID);
	Log("  EntityID = %d", EntityID);
	Log("  MouseButton = %d", MouseButton);
	COPY_TO_SERVER();
	return true;
}





bool cConnection::HandleClientWindowClick(void)
{
	HANDLE_CLIENT_PACKET_READ(ReadChar,    char,  WindowID);
	HANDLE_CLIENT_PACKET_READ(ReadBEShort, short, SlotNum);
	HANDLE_CLIENT_PACKET_READ(ReadChar,    char,  IsRightClick);
	HANDLE_CLIENT_PACKET_READ(ReadBEShort, short, TransactionID);
	HANDLE_CLIENT_PACKET_READ(ReadChar,    char,  IsShiftClick);
	AString Item;
	if (!ParseSlot(m_ClientBuffer, Item))
	{
		return false;
	}
	Log("Received a PACKET_WINDOW_CLICK from the client");
	Log("  WindowID = %d", WindowID);
	Log("  SlotNum = %d", SlotNum);
	Log("  IsRclk = %d, IsShift = %d", IsRightClick, IsShiftClick);
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
	HANDLE_SERVER_PACKET_READ(ReadBEInt,   int,   BlockX);
	HANDLE_SERVER_PACKET_READ(ReadBEShort, short, BlockY);
	HANDLE_SERVER_PACKET_READ(ReadBEInt,   int,   BlockZ);
	HANDLE_SERVER_PACKET_READ(ReadByte,    Byte,  Byte1);
	HANDLE_SERVER_PACKET_READ(ReadByte,    Byte,  Byte2);
	HANDLE_SERVER_PACKET_READ(ReadBEShort, short, BlockID);
	Log("Received a PACKET_BLOCK_ACTION from the server:");
	Log("  Pos = {%d, %d, %d}", BlockX, BlockY, BlockZ);
	Log("  Bytes = (%d, %d) == (0x%x, 0x%x)", Byte1, Byte2, Byte1, Byte2);
	Log("  BlockID = %d", BlockID);
	COPY_TO_CLIENT();
	return true;
}





bool cConnection::HandleServerBlockChange(void)
{
	HANDLE_SERVER_PACKET_READ(ReadBEInt,   int,   BlockX);
	HANDLE_SERVER_PACKET_READ(ReadByte,    Byte,  BlockY);
	HANDLE_SERVER_PACKET_READ(ReadBEInt,   int,   BlockZ);
	HANDLE_SERVER_PACKET_READ(ReadBEShort, short, BlockType);
	HANDLE_SERVER_PACKET_READ(ReadChar,    char,  BlockMeta);
	Log("Received a PACKET_BLOCK_CHANGE from the server");
	COPY_TO_CLIENT();
	return true;
}





bool cConnection::HandleServerChangeGameState(void)
{
	HANDLE_SERVER_PACKET_READ(ReadChar, char,  Reason);
	HANDLE_SERVER_PACKET_READ(ReadChar, char,  Data);
	Log("Received a PACKET_CHANGE_GAME_STATE from the server:");
	Log("  Reason = %d", Reason);
	Log("  Data = %d", Data);
	COPY_TO_CLIENT();
	return true;
}





bool cConnection::HandleServerChatMessage(void)
{
	HANDLE_SERVER_PACKET_READ(ReadBEUTF16String16, AString, Message);
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





bool cConnection::HandleServerEncryptionKeyRequest(void)
{
	// Read the packet from the server:
	HANDLE_SERVER_PACKET_READ(ReadBEUTF16String16, AString, ServerID);
	HANDLE_SERVER_PACKET_READ(ReadBEShort,         short,   PublicKeyLength);
	AString PublicKey;
	if (!m_ServerBuffer.ReadString(PublicKey, PublicKeyLength))
	{
		return false;
	}
	HANDLE_SERVER_PACKET_READ(ReadBEShort,         short,   NonceLength);
	AString Nonce;
	if (!m_ServerBuffer.ReadString(Nonce, NonceLength))
	{
		return false;
	}
	Log("Got PACKET_ENCRYPTION_KEY_REQUEST from the SERVER:");
	Log("  ServerID = %s", ServerID.c_str());
	
	// Reply to the server:
	SendEncryptionKeyResponse(PublicKey, Nonce);
	
	// Send a 0xFD Encryption Key Request http://wiki.vg/Protocol#0xFD to the client, using our own key:
	Log("Sending PACKET_ENCRYPTION_KEY_REQUEST to the CLIENT");
	AString key;
	StringSink sink(key);  // GCC won't allow inline instantiation in the following line, damned temporary refs
	m_Server.GetPublicKey().Save(sink);
	cByteBuffer ToClient(512);
	ToClient.WriteByte           (PACKET_ENCRYPTION_KEY_REQUEST);
	ToClient.WriteBEUTF16String16(ServerID);
	ToClient.WriteBEShort        ((short)key.size());
	ToClient.WriteBuf            (key.data(), key.size());
	ToClient.WriteBEShort        (4);
	ToClient.WriteBEInt          (m_Nonce);  // Using 'this' as the cryptographic nonce, so that we don't have to generate one each time :)
	CLIENTSEND(ToClient);
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
		HANDLE_SERVER_PACKET_READ(ReadBEUTF16String16, AString, Key);
		HANDLE_SERVER_PACKET_READ(ReadBEDouble,        double,  Value);
		Log(" \"%s\" = %f", Key.c_str(), Value);
	}  // for i
	
	HANDLE_SERVER_PACKET_READ(ReadBEShort, short, ListLength);
	Log("  ListLength = %d", ListLength);
	for (int i = 0; i < ListLength; i++)
	{
		HANDLE_SERVER_PACKET_READ(ReadBEInt64,  Int64,  UUIDHi);
		HANDLE_SERVER_PACKET_READ(ReadBEInt64,  Int64,  UUIDLo);
		HANDLE_SERVER_PACKET_READ(ReadBEDouble, double, DblVal);
		HANDLE_SERVER_PACKET_READ(ReadByte,     Byte,   ByteVal);
		Log("  [%d] = {0x%08llx%08llx, %f, %i}", i, UUIDHi, UUIDLo, DblVal, ByteVal);
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
	Log("  RelMove = <%d, %d, %d>", dx, dy, dz);
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
	Log("  RelMove = <%d, %d, %d>", dx, dy, dz);
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
	HANDLE_SERVER_PACKET_READ(ReadBEInt, int, EntityID);
	HANDLE_SERVER_PACKET_READ(ReadBEInt, int,  BlockX);
	HANDLE_SERVER_PACKET_READ(ReadBEInt, int,  BlockY);
	HANDLE_SERVER_PACKET_READ(ReadBEInt, int,  BlockZ);
	HANDLE_SERVER_PACKET_READ(ReadByte,  Byte, Yaw);
	HANDLE_SERVER_PACKET_READ(ReadByte,  Byte, Pitch);
	Log("Received a PACKET_ENTITY_TELEPORT from the server:");
	Log("  EntityID = %d", EntityID);
	Log("  Pos = {%d, %d, %d}", BlockX, BlockY, BlockZ);
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
	Log("  Velocity = <%d, %d, %d>", VelocityX, VelocityY, VelocityZ);
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





bool cConnection::HandleServerKeepAlive(void)
{
	HANDLE_SERVER_PACKET_READ(ReadBEInt, int, PingID);
	Log("Received a PACKET_KEEP_ALIVE from the server:");
	Log("  ID = %d", PingID);
	COPY_TO_CLIENT()
	return true;
}





bool cConnection::HandleServerEncryptionKeyResponse(void)
{
	HANDLE_SERVER_PACKET_READ(ReadBEInt, int, Lengths);
	if (Lengths != 0)
	{
		Log("Lengths are not zero!");
		return true;
	}
	Log("Server communication is now encrypted");
	m_ServerState = csEncryptedUnderstood;
	DataLog(m_ServerEncryptionBuffer.data(), m_ServerEncryptionBuffer.size(), "Sending the queued data to server (%u bytes):", m_ServerEncryptionBuffer.size());
	SERVERENCRYPTSEND(m_ServerEncryptionBuffer.data(), m_ServerEncryptionBuffer.size());
	m_ServerEncryptionBuffer.clear();
	return true;
}





bool cConnection::HandleServerKick(void)
{
	HANDLE_SERVER_PACKET_READ(ReadBEUTF16String16, AString, Reason);
	Log("Received PACKET_KICK from the SERVER:");
	if (m_HasClientPinged)
	{
		Log("  This was a std reply to client's PING");
		AStringVector Split;
		
		// Split by NULL chars (StringSplit() won't work here):
		size_t Last = 0;
		for (size_t i = 0; i < Reason.size(); i++)
		{
			if (Reason[i] == 0)
			{
				Split.push_back(Reason.substr(Last, i - Last));
				Last = i + 1;
			}
		}
		
		if (Split.size() == 5)
		{
			Log("  Protocol version: \"%s\"", Split[0].c_str());
			Log("  Server version: \"%s\"", Split[1].c_str());
			Log("  MOTD: \"%s\"", Split[2].c_str());
			Log("  Cur players: \"%s\"", Split[3].c_str());
			Log("  Max players: \"%s\"", Split[4].c_str());
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





bool cConnection::HandleServerLogin(void)
{
	HANDLE_SERVER_PACKET_READ(ReadBEInt,           int,     EntityID);
	HANDLE_SERVER_PACKET_READ(ReadBEUTF16String16, AString, LevelType);
	HANDLE_SERVER_PACKET_READ(ReadChar,            char,    GameMode);
	HANDLE_SERVER_PACKET_READ(ReadChar,            char,    Dimension);
	HANDLE_SERVER_PACKET_READ(ReadChar,            char,    Difficulty);
	HANDLE_SERVER_PACKET_READ(ReadChar,            char,    Unused);
	HANDLE_SERVER_PACKET_READ(ReadChar,            char,    MaxPlayers);
	Log("Received a PACKET_LOGIN from the server:");
	Log("  EntityID = %d",      EntityID);
	Log("  LevelType = \"%s\"", LevelType.c_str());
	Log("  GameMode = %d",      GameMode);
	Log("  Dimension = %d",     Dimension);
	Log("  Difficulty = %d",    Difficulty);
	Log("  Unused = %d",        Unused);
	Log("  MaxPlayers = %d",    MaxPlayers);
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
	AString Meta;
	if (!m_ServerBuffer.ReadString(Meta, ChunkCount * 12))
	{
		return false;
	}
	Log("Received a PACKET_MAP_CHUNK_BULK from the server:");
	Log("  ChunkCount = %d", ChunkCount);
	Log("  Compressed size = %d (0x%x)", CompressedSize, CompressedSize);
	Log("  IsSkyLightSent = %s", IsSkyLightSent ? "true" : "false");
	
	// TODO: Save the compressed data into a file for later analysis
	
	COPY_TO_CLIENT();
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
	HANDLE_SERVER_PACKET_READ(ReadBEUTF16String16, AString, SoundName);
	HANDLE_SERVER_PACKET_READ(ReadBEInt,           int,     PosX);
	HANDLE_SERVER_PACKET_READ(ReadBEInt,           int,     PosY);
	HANDLE_SERVER_PACKET_READ(ReadBEInt,           int,     PosZ);
	HANDLE_SERVER_PACKET_READ(ReadBEFloat,         float,   Volume);
	HANDLE_SERVER_PACKET_READ(ReadByte,            Byte,    Pitch);
	Log("Received a PACKET_NAMED_SOUND_EFFECT from the server:");
	Log("  SoundName = \"%s\"", SoundName.c_str());
	Log("  Pos = (%d, %d, %d) ~ {%d, %d, %d}", PosX, PosY, PosZ, PosX / 8, PosY / 8, PosZ / 8);
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
	HANDLE_SERVER_PACKET_READ(ReadBEInt, int,  PlayerID);
	HANDLE_SERVER_PACKET_READ(ReadByte,  Byte, AnimationID);
	Log("Received a PACKET_PLAYER_ANIMATION from the server:");
	Log("  PlayerID: %d (0x%x)", PlayerID, PlayerID);
	Log("  Animation: %d", AnimationID);
	COPY_TO_CLIENT();
	return true;
}





bool cConnection::HandleServerPlayerListItem(void)
{
	HANDLE_SERVER_PACKET_READ(ReadBEUTF16String16, AString, PlayerName);
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
	HANDLE_SERVER_PACKET_READ(ReadBEDouble, double, Stance);
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
	HANDLE_SERVER_PACKET_READ(ReadBEUTF16String16, AString, ChannelName);
	HANDLE_SERVER_PACKET_READ(ReadBEShort,         short,   Length);
	AString Data;
	if (!m_ServerBuffer.ReadString(Data, Length))
	{
		return false;
	}
	Log("Received a PACKET_PLUGIN_MESSAGE from the server");
	Log("  ChannelName = \"%s\"", ChannelName.c_str());
	DataLog(Data.data(), Length, "  Data: %d bytes", Length);
	COPY_TO_SERVER();
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
	HANDLE_SERVER_PACKET_READ(ReadBEShort, short, SlotNum);
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





bool cConnection::HandleServerSpawnMob(void)
{
	HANDLE_SERVER_PACKET_READ(ReadBEInt,   int,   EntityID);
	HANDLE_SERVER_PACKET_READ(ReadChar,    char,  MobType);
	HANDLE_SERVER_PACKET_READ(ReadBEInt,   int,   PosX);
	HANDLE_SERVER_PACKET_READ(ReadBEInt,   int,   PosY);
	HANDLE_SERVER_PACKET_READ(ReadBEInt,   int,   PosZ);
	HANDLE_SERVER_PACKET_READ(ReadByte,    Byte,  Yaw);
	HANDLE_SERVER_PACKET_READ(ReadByte,    Byte,  Pitch);
	HANDLE_SERVER_PACKET_READ(ReadByte,    Byte,  HeadYaw);
	HANDLE_SERVER_PACKET_READ(ReadBEShort, short, VelocityX);
	HANDLE_SERVER_PACKET_READ(ReadBEShort, short, VelocityY);
	HANDLE_SERVER_PACKET_READ(ReadBEShort, short, VelocityZ);
	AString Metadata;
	if (!ParseMetadata(m_ServerBuffer, Metadata))
	{
		return false;
	}
	AString HexDump;
	CreateHexDump(HexDump, Metadata.data(), Metadata.size(), 32);
	Log("Received a PACKET_SPAWN_MOB from the server:");
	Log("  EntityID = %d", EntityID);
	Log("  MobType = %d", MobType);
	Log("  Pos = <%d, %d, %d> ~ {%d, %d, %d}", PosX, PosY, PosZ, PosX / 32, PosY / 32, PosZ / 32);
	Log("  Angles = [%d, %d, %d]", Yaw, Pitch, HeadYaw);
	Log("  Velocity = <%d, %d, %d>", VelocityX, VelocityY, VelocityZ);
	Log("  Metadata, length = %d (0x%x):\n%s", Metadata.length(), Metadata.length(), HexDump.c_str());
	LogMetadata(Metadata, 4);
	COPY_TO_CLIENT();
	return true;
}





bool cConnection::HandleServerSpawnNamedEntity(void)
{
	HANDLE_SERVER_PACKET_READ(ReadBEInt,           int,     EntityID);
	HANDLE_SERVER_PACKET_READ(ReadBEUTF16String16, AString, EntityName);
	HANDLE_SERVER_PACKET_READ(ReadBEInt,           int,     PosX);
	HANDLE_SERVER_PACKET_READ(ReadBEInt,           int,     PosY);
	HANDLE_SERVER_PACKET_READ(ReadBEInt,           int,     PosZ);
	HANDLE_SERVER_PACKET_READ(ReadByte,            Byte,    Yaw);
	HANDLE_SERVER_PACKET_READ(ReadByte,            Byte,    Pitch);
	HANDLE_SERVER_PACKET_READ(ReadBEShort,         short,   CurrentItem);
	AString Metadata;
	if (!ParseMetadata(m_ServerBuffer, Metadata))
	{
		return false;
	}
	AString HexDump;
	CreateHexDump(HexDump, Metadata.data(), Metadata.size(), 32);
	Log("Received a PACKET_SPAWN_NAMED_ENTITY from the server:");
	Log("  EntityID = %d (0x%x)", EntityID, EntityID);
	Log("  Name = %s", EntityName.c_str());
	Log("  Pos = <%d, %d, %d> ~ {%d, %d, %d}", PosX, PosY, PosZ, PosX / 32, PosY / 32, PosZ / 32);
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
	m_ServerBuffer.SkipRead(1);
	if (Buffer.size() > 128)
	{
		// Only log up to 128 bytes
		Buffer.erase(128, AString::npos);
	}
	DataLog(Buffer.data(), Buffer.size(), "Buffer while parsing the PACKET_SPAWN_OBJECT_VEHICLE packet (%d bytes):", Buffer.size());
	#endif  // _DEBUG
	
	HANDLE_SERVER_PACKET_READ(ReadBEInt,   int,   EntityID);
	HANDLE_SERVER_PACKET_READ(ReadChar,    char,  ObjType);
	HANDLE_SERVER_PACKET_READ(ReadBEInt,   int,   PosX);
	HANDLE_SERVER_PACKET_READ(ReadBEInt,   int,   PosY);
	HANDLE_SERVER_PACKET_READ(ReadBEInt,   int,   PosZ);
	HANDLE_SERVER_PACKET_READ(ReadByte,    Byte,  Yaw);
	HANDLE_SERVER_PACKET_READ(ReadByte,    Byte,  Pitch);
	HANDLE_SERVER_PACKET_READ(ReadBEInt,   int,   DataIndicator);
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
	Log("  EntityID = %d (0x%x)", EntityID, EntityID);
	Log("  ObjType = %d (0x%x)", ObjType, ObjType);
	Log("  Pos = <%d, %d, %d> ~ {%d, %d, %d}", PosX, PosY, PosZ, PosX / 32, PosY / 32, PosZ / 32);
	Log("  Rotation = <yaw %d, pitch %d>", Yaw, Pitch);
	Log("  DataIndicator = %d (0x%x)", DataIndicator, DataIndicator);
	if (DataIndicator != 0)
	{
		Log("  Velocity = <%d, %d, %d>", VelocityX, VelocityY, VelocityZ);
		DataLog(ExtraData.data(), ExtraData.size(), "  ExtraData size = %d:", ExtraData.size());
	}
	COPY_TO_CLIENT();
	return true;
}





bool cConnection::HandleServerSpawnPainting(void)
{
	HANDLE_SERVER_PACKET_READ(ReadBEInt,           int,     EntityID);
	HANDLE_SERVER_PACKET_READ(ReadBEUTF16String16, AString, ImageName);
	HANDLE_SERVER_PACKET_READ(ReadBEInt,           int,     PosX);
	HANDLE_SERVER_PACKET_READ(ReadBEInt,           int,     PosY);
	HANDLE_SERVER_PACKET_READ(ReadBEInt,           int,     PosZ);
	HANDLE_SERVER_PACKET_READ(ReadBEInt,           int,     Direction);
	Log("Received a PACKET_SPAWN_PAINTING from the server:");
	Log("  EntityID = %d", EntityID);
	Log("  ImageName = \"%s\"", ImageName.c_str());
	Log("  Pos = <%d, %d, %d> ~ {%d, %d, %d}", PosX, PosY, PosZ, PosX / 32, PosY / 32, PosZ / 32);
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
	Log("  Pos = <%d, %d, %d> ~ {%d, %d, %d}", PosX, PosY, PosZ, PosX / 32, PosY / 32, PosZ / 32);
	Log("  Angles = [%d, %d, %d]", Rotation, Pitch, Roll);
	COPY_TO_CLIENT();
	return true;
}





bool cConnection::HandleServerTabCompletion(void)
{
	HANDLE_SERVER_PACKET_READ(ReadBEUTF16String16, AString, Results);
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
	HANDLE_SERVER_PACKET_READ(ReadBEUTF16String16, AString, Line1);
	HANDLE_SERVER_PACKET_READ(ReadBEUTF16String16, AString, Line2);
	HANDLE_SERVER_PACKET_READ(ReadBEUTF16String16, AString, Line3);
	HANDLE_SERVER_PACKET_READ(ReadBEUTF16String16, AString, Line4);
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
	DataLog(Data.data(), Data.size(), "  Data (%d bytes)", Data.size());
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
	HANDLE_SERVER_PACKET_READ(ReadBEUTF16String16, AString, Title);
	HANDLE_SERVER_PACKET_READ(ReadByte,            Byte,    NumSlots);
	HANDLE_SERVER_PACKET_READ(ReadByte,            Byte,    UseProvidedTitle);
	int HorseInt = 0;
	if (WindowType == 11)  // Horse / Donkey / Mule
	{
		HANDLE_SERVER_PACKET_READ(ReadBEInt, int, intHorseInt);
		HorseInt = intHorseInt;
	}
	Log("Received a PACKET_WINDOW_OPEN from the server:");
	Log("  WindowID = %d", WindowID);
	Log("  WindowType = %d", WindowType);
	Log("  Title = \"%s\", Use = %d", Title.c_str(), UseProvidedTitle);
	Log("  NumSlots = %d", NumSlots);
	if (WindowType == 11)
	{
		Log("  HorseInt = %d (0x%08x)", HorseInt, HorseInt);
	}
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
			case 4:  // string16
			{
				short Len = 0;
				if (!a_Buffer.ReadBEShort(Len))
				{
					return false;
				}
				short NetLen = htons(Len);
				a_Metadata.append((char *)&NetLen, 2);
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
			case 4:  // string16
			{
				short Length = (a_Metadata[pos + 1] << 8) | a_Metadata[pos + 2];
				Log("%sstring[%d] = \"%*s\"", Indent.c_str(), Index, Length, a_Metadata.c_str() + pos + 3);
				pos += Length + 2;
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
	ToServer.WriteByte(PACKET_ENCRYPTION_KEY_RESPONSE);
	ToServer.WriteBEShort(EncryptedLength);
	ToServer.WriteBuf(EncryptedSecret, EncryptedLength);
	ToServer.WriteBEShort(EncryptedLength);
	ToServer.WriteBuf(EncryptedNonce, EncryptedLength);
	SERVERSEND(ToServer);
	m_ServerState = csWaitingForEncryption;
}





void cConnection::StartClientEncryption(const AString & a_EncKey, const AString & a_EncNonce)
{
	// Decrypt EncNonce using privkey
	RSAES<PKCS1v15>::Decryptor rsaDecryptor(m_Server.GetPrivateKey());
	time_t CurTime = time(NULL);
	RandomPool rng;
	rng.Put((const byte *)&CurTime, sizeof(CurTime));
	byte DecryptedNonce[MAX_ENC_LEN];
	DecodingResult res = rsaDecryptor.Decrypt(rng, (const byte *)a_EncNonce.data(), a_EncNonce.size(), DecryptedNonce);
	if (!res.isValidCoding || (res.messageLength != 4))
	{
		Log("Client: Bad nonce length");
		return;
	}
	if (ntohl(*((int *)DecryptedNonce)) != m_Nonce)
	{
		Log("Bad nonce value");
		return;
	}
	
	// Decrypt the symmetric encryption key using privkey:
	byte SharedSecret[MAX_ENC_LEN];
	res = rsaDecryptor.Decrypt(rng, (const byte *)a_EncKey.data(), a_EncKey.size(), SharedSecret);
	if (!res.isValidCoding || (res.messageLength != 16))
	{
		Log("Bad key length");
		return;
	}
	
	// Send encryption key response:
	cByteBuffer ToClient(6);
	ToClient.WriteByte((char)0xfc);
	ToClient.WriteBEShort(0);
	ToClient.WriteBEShort(0);
	CLIENTSEND(ToClient);
	
	// Start the encryption:
	m_ClientEncryptor.SetKey(SharedSecret, 16, MakeParameters(Name::IV(), ConstByteArrayParameter(SharedSecret, 16))(Name::FeedbackSize(), 1));
	m_ClientDecryptor.SetKey(SharedSecret, 16, MakeParameters(Name::IV(), ConstByteArrayParameter(SharedSecret, 16))(Name::FeedbackSize(), 1));
	Log("Client connection is now encrypted");
	m_ClientState = csEncryptedUnderstood;
	
	// Send the queued data:
	DataLog(m_ClientEncryptionBuffer.data(), m_ClientEncryptionBuffer.size(), "Sending the queued data to client (%u bytes):", m_ClientEncryptionBuffer.size());
	CLIENTENCRYPTSEND(m_ClientEncryptionBuffer.data(), m_ClientEncryptionBuffer.size());
	m_ClientEncryptionBuffer.clear();
	
	// Handle all postponed server data
	DecodeServersPackets(NULL, 0);
}




