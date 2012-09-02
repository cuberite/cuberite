
// Connection.h

// Interfaces to the cConnection class representing a single pair of connected sockets





#pragma once

#include <time.h>
#include "ByteBuffer.h"





class cServer;





class cConnection
{
	cCriticalSection m_CSLog;
	FILE * m_LogFile;
	
	cServer & m_Server;
	SOCKET m_ClientSocket;
	SOCKET m_ServerSocket;
	
	clock_t m_BeginTick;  // Tick when the relative time was first retrieved (used for GetRelativeTime())
	
	enum eConnectionState
	{
		csUnencrypted,          // The connection is not encrypted. Packets must be decoded in order to be able to start decryption.
		csEncryptedUnderstood,  // The communication is encrypted and so far all packets have been understood, so they can be still decoded
		csEncryptedUnknown,     // The communication is encrypted, but an unknown packet has been received, so packets cannot be decoded anymore
	};
	
	eConnectionState m_ClientState;
	eConnectionState m_ServerState;
	
	int m_Nonce;
	
public:
	cConnection(SOCKET a_ClientSocket, cServer & a_Server);
	~cConnection();
	
	void Run(void);
	
	void Log(const char * a_Format, ...);
	void DataLog(const void * a_Data, int a_Size, const char * a_Format, ...);
	
protected:
	typedef CFB_Mode<AES>::Encryption Encryptor;
	typedef CFB_Mode<AES>::Decryption Decryptor;
	
	cByteBuffer m_ClientBuffer;
	cByteBuffer m_ServerBuffer;
	
	Decryptor m_ServerDecryptor;
	Encryptor m_ServerEncryptor;

	Decryptor m_ClientDecryptor;
	Encryptor m_ClientEncryptor;

	bool ConnectToServer(void);
	
	/// Relays data from server to client; returns false if connection aborted
	bool RelayFromServer(void);
	
	/// Relays data from client to server; returns false if connection aborted
	bool RelayFromClient(void);
	
	/// Returns the time relative to the first call of this function, in the fractional seconds elapsed
	double GetRelativeTime(void);
	
	/// Sends data to the specified socket. If sending fails, prints a fail message using a_Peer and returns false.
	bool SendData(SOCKET a_Socket, const char * a_Data, int a_Size, const char * a_Peer);
	
	/// Sends data to the specified socket. If sending fails, prints a fail message using a_Peer and returns false.
	bool SendData(SOCKET a_Socket, cByteBuffer & a_Data, const char * a_Peer);
	
	/// Sends data to the specfied socket, after encrypting it using a_Encryptor. If sending fails, prints a fail message using a_Peer and returns false
	bool SendEncryptedData(SOCKET a_Socket, Encryptor & a_Encryptor, const char * a_Data, int a_Size, const char * a_Peer);
	
	/// Sends data to the specfied socket, after encrypting it using a_Encryptor. If sending fails, prints a fail message using a_Peer and returns false
	bool SendEncryptedData(SOCKET a_Socket, Encryptor & a_Encryptor, cByteBuffer & a_Data, const char * a_Peer);
	
	/// Decodes packets coming from the client, sends appropriate counterparts to the server; returns false if the connection is to be dropped
	bool DecodeClientsPackets(const char * a_Data, int a_Size);

	/// Decodes packets coming from the server, sends appropriate counterparts to the client; returns false if the connection is to be dropped
	bool DecodeServersPackets(const char * a_Data, int a_Size);
	
	// Packet handling, client-side:
	void HandleClientEncryptionKeyResponse(void);
	void HandleClientHandshake(void);
	void HandleClientPing(void);

	// Packet handling, server-side:
	void HandleServerEncryptionKeyRequest(void);
	void HandleServerEncryptionKeyResponse(void);
	void HandleServerKick(void);
	
	/// Send EKResp to the server:
	void SendEncryptionKeyResponse(const AString & a_ServerPublicKey, const AString & a_Nonce);
	
	/// Starts client encryption based on the parameters received
	void StartClientEncryption(const AString & a_EncryptedSecret, const AString & a_EncryptedNonce);
} ;




