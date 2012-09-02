
// Connection.cpp

// Interfaces to the cConnection class representing a single pair of connected sockets

#include "Globals.h"
#include "Connection.h"
#include "Server.h"





#define HANDLE_CLIENT_PACKET_READ(Proc, Type, Var) \
	Type Var; \
	{ \
		if (!m_ClientBuffer.Proc(Var)) \
		{ \
			return; \
		} \
	}

#define HANDLE_SERVER_PACKET_READ(Proc, Type, Var) \
	Type Var; \
	{ \
		if (!m_ServerBuffer.Proc(Var)) \
		{ \
			return; \
		} \
	}

#define CLIENTSEND(...) SendData(m_ClientSocket, __VA_ARGS__, "Client")
#define SERVERSEND(...) SendData(m_ServerSocket, __VA_ARGS__, "Server")
#define CLIENTENCRYPTSEND(...) SendEncryptedData(m_ClientSocket, m_ClientEncryptor, __VA_ARGS__, "Client")
#define SERVERENCRYPTSEND(...) SendEncryptedData(m_ServerSocket, m_ServerEncryptor, __VA_ARGS__, "Server")

#define MAX_ENC_LEN 1024



typedef unsigned char Byte;





enum
{
	PACKET_HANDSHAKE               = 0x02,
	PACKET_ENCRYPTION_KEY_RESPONSE = 0xfc,
	PACKET_ENCRYPTION_KEY_REQUEST  = 0xfd,
	PACKET_PING                    = 0xfe,
	PACKET_KICK                    = 0xff,
} ;





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cConnection:

cConnection::cConnection(SOCKET a_ClientSocket, cServer & a_Server) :
	m_Server(a_Server),
	m_LogFile(NULL),
	m_ClientSocket(a_ClientSocket),
	m_ServerSocket(-1),
	m_BeginTick(clock()),
	m_ClientState(csUnencrypted),
	m_ServerState(csUnencrypted),
	m_ClientBuffer(64 KiB),
	m_ServerBuffer(64 KiB),
	m_Nonce(0)
{
	AString fnam;
	Printf(fnam, "Log_%d.log", (int)time(NULL));
	m_LogFile = fopen(fnam.c_str(), "w");
	Log("Log file created");
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
	puts(FullMsg.c_str());
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
	Printf(FullMsg, "[%5.3f] %s\n%s", GetRelativeTime(), msg.c_str(), CreateHexDump(Hex, a_Data, a_Size, 16).c_str());
	
	// Log to file:
	cCSLock Lock(m_CSLog);
	fputs(FullMsg.c_str(), m_LogFile);
	
	// Log to screen:
	puts(FullMsg.c_str());
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
	char Buffer[1024];
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
	char Buffer[1024];
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
	DataLog(a_Data, a_Size, "Sending encrypted %d bytes to %s", a_Size, a_Peer);
	const byte * Data = (const byte *)a_Data;
	while (a_Size > 0)
	{
		byte Buffer[512];
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
		unsigned char PacketType;
		m_ClientBuffer.ReadByte(PacketType);
		switch (PacketType)
		{
			case PACKET_ENCRYPTION_KEY_RESPONSE: HandleClientEncryptionKeyResponse(); break;
			case PACKET_HANDSHAKE:               HandleClientHandshake(); break;
			case PACKET_PING:                    HandleClientPing(); break;
			default:
			{
				if (m_ClientState == csEncryptedUnderstood)
				{
					Log("Unknown packet 0x%02x from the client while encrypted; continuing to relay blind only", PacketType);
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
		switch (PacketType)
		{
			case PACKET_ENCRYPTION_KEY_REQUEST:  HandleServerEncryptionKeyRequest(); break;
			case PACKET_ENCRYPTION_KEY_RESPONSE: HandleServerEncryptionKeyResponse(); break;
			case PACKET_KICK:                    HandleServerKick(); break;
			default:
			{
				if (m_ServerState == csEncryptedUnderstood)
				{
					Log("Unknown packet 0x%02x from the server while encrypted; continuing to relay blind only", PacketType);
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





void cConnection::HandleClientEncryptionKeyResponse(void)
{
	HANDLE_CLIENT_PACKET_READ(ReadBEShort, short, EncKeyLength);
	AString EncKey;
	if (!m_ClientBuffer.ReadString(EncKey, EncKeyLength))
	{
		return;
	}
	HANDLE_CLIENT_PACKET_READ(ReadBEShort, short, EncNonceLength);
	AString EncNonce;
	if (!m_ClientBuffer.ReadString(EncNonce, EncNonceLength))
	{
		return;
	}
	if ((EncKeyLength > MAX_ENC_LEN) || (EncNonceLength > MAX_ENC_LEN))
	{
		Log("Client: Too long encryption params");
		return;
	}
	StartClientEncryption(EncKey, EncNonce);
}





void cConnection::HandleClientHandshake(void)
{
	// Read the packet from the client:
	HANDLE_CLIENT_PACKET_READ(ReadByte,            Byte,    ProtocolVersion);
	HANDLE_CLIENT_PACKET_READ(ReadBEUTF16String16, AString, Username);
	HANDLE_CLIENT_PACKET_READ(ReadBEUTF16String16, AString, ServerHost);
	HANDLE_CLIENT_PACKET_READ(ReadBEInt,           int,     ServerPort);
	m_ClientBuffer.CommitRead();
	
	// Send the same packet to the server:
	cByteBuffer ToServer(512);
	ToServer.WriteByte           (PACKET_HANDSHAKE);
	ToServer.WriteByte           (ProtocolVersion);
	ToServer.WriteBEUTF16String16(Username);
	ToServer.WriteBEUTF16String16(ServerHost);
	ToServer.WriteBEInt          (m_Server.GetConnectPort());
	SERVERSEND(ToServer);
}





void cConnection::HandleClientPing(void)
{
	Log("Received a PACKET_PING from the CLIENT");
	m_ClientBuffer.ResetRead();
	SERVERSEND(m_ClientBuffer);
}





void cConnection::HandleServerEncryptionKeyRequest(void)
{
	// Read the packet from the server:
	HANDLE_SERVER_PACKET_READ(ReadBEUTF16String16, AString, ServerID);
	HANDLE_SERVER_PACKET_READ(ReadBEShort,         short,   PublicKeyLength);
	AString PublicKey;
	if (!m_ServerBuffer.ReadString(PublicKey, PublicKeyLength))
	{
		return;
	}
	HANDLE_SERVER_PACKET_READ(ReadBEShort,         short,   NonceLength);
	AString Nonce;
	if (!m_ServerBuffer.ReadString(Nonce, NonceLength))
	{
		return;
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
}





void cConnection::HandleServerEncryptionKeyResponse(void)
{
	HANDLE_SERVER_PACKET_READ(ReadBEInt, int, Lengths);
	if (Lengths != 0)
	{
		Log("Lengths are not zero!");
		return;
	}
	m_ServerState = csEncryptedUnderstood;
}





void cConnection::HandleServerKick(void)
{
	HANDLE_SERVER_PACKET_READ(ReadBEUTF16String16, AString, Reason);
	Log("Received PACKET_KICK from the SERVER:");
	Log("  Reason = \"%s\"", Reason.c_str());
	
	cByteBuffer ToClient(Reason.size() * 2 + 4);
	ToClient.WriteByte(PACKET_KICK);
	ToClient.WriteBEUTF16String16(Reason);
	CLIENTSEND(ToClient);
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
	AutoSeededRandomPool rng;
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
}





void cConnection::StartClientEncryption(const AString & a_EncKey, const AString & a_EncNonce)
{
	// Decrypt EncNonce using privkey
	RSAES<PKCS1v15>::Decryptor rsaDecryptor(m_Server.GetPrivateKey());
	AutoSeededRandomPool rng;
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
	m_ClientState = csEncryptedUnderstood;
	
	// Handle all postponed server data
	DecodeServersPackets(NULL, 0);
}




