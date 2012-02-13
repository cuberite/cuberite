
// cServer.h

// Interfaces to the cServer object representing the network server





#pragma once
#ifndef CSERVER_H_INCLUDED
#define CSERVER_H_INCLUDED

#include "cSocketThreads.h"





class cPlayer;
class cClientHandle;
class cPacket;





class cServer										//tolua_export
{													//tolua_export
public:												//tolua_export
	static cServer * GetServer();					//tolua_export

	bool InitServer( int a_Port = 25565 );

	int GetPort() { return m_iServerPort; }
	bool IsConnected(){return m_bIsConnected;} // returns connection status
	void StartListenClient(); // Listen to client

	void Broadcast(const cPacket & a_Packet, cClientHandle* a_Exclude = NULL) { Broadcast(&a_Packet, a_Exclude); }
	void Broadcast(const cPacket * a_Packet, cClientHandle* a_Exclude = NULL);

	bool Tick(float a_Dt);

	void StartListenThread();

	bool Command( cClientHandle & a_Client, const char* a_Cmd );
	void ServerCommand( const char* a_Cmd );								//tolua_export
	void Shutdown();

	void SendMessage( const char* a_Message, cPlayer* a_Player = 0, bool a_bExclude = false ); //tolua_export
	
	void KickUser(const AString & iUserName, const AString & iReason);
	void AuthenticateUser(const AString & iUserName);  // Called by cAuthenticator to auth the specified user

	static void ServerListenThread( void* a_Args );

	const AString & GetServerID(void) const;
	
	void ClientDestroying(const cClientHandle * a_Client);  // Called by cClientHandle::Destroy(); removes the client from m_SocketThreads
	
private:

	friend class cRoot; // so cRoot can create and destroy cServer
	
	cServer();
	~cServer();

	struct sServerState;
	sServerState* m_pState;
	
	cSocketThreads m_SocketThreads;

	// Time since server was started
	float m_Millisecondsf;
	unsigned int m_Milliseconds;

	bool m_bIsConnected; // true - connected false - not connected
	int m_iServerPort;

	bool m_bRestarting;
}; //tolua_export





#endif  // CSERVER_H_INCLUDED




