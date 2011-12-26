#pragma once

class cEvent;
class cSemaphore;
class cCriticalSection;
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
	int RecClient(cClientHandle *sRecSocket); // receive message for a particular socket

	void Broadcast( const cPacket & a_Packet, cClientHandle* a_Exclude = 0 );

	bool Tick(float a_Dt);

	void StartListenThread();

	bool Command( cClientHandle & a_Client, const char* a_Cmd );
	void ServerCommand( const char* a_Cmd );								//tolua_export
	void Shutdown();

	void SendMessage( const char* a_Message, cPlayer* a_Player = 0, bool a_bExclude = false ); //tolua_export

	static void ServerListenThread( void* a_Args );

	const char* GetServerID();
private:
	friend class cRoot; // so cRoot can create and destroy cServer
	cServer();
	~cServer();

	struct sServerState;
	sServerState* m_pState;

	// Time since server was started
	float m_Millisecondsf;
	unsigned int m_Milliseconds;

	bool m_bIsConnected; // true - connected false - not connected
	int m_iServerPort;

	bool m_bRestarting;
}; //tolua_export
