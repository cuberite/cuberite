#pragma once

class cSocket;
class cSemaphore;
class cEvent;
class Game;
class cPacket;
class cChunk;
class cPlayer;
class cRedstone;
class cClientHandle							// tolua_export
{											// tolua_export
public:
	enum ENUM_PRIORITY
	{
		E_PRIORITY_LOW,
		E_PRIORITY_NORMAL
	};

	static const int MAXBLOCKCHANGEINTERACTIONS = 10; // 5 didn't help, 10 seems to have done the trick

	cClientHandle(const cSocket & a_Socket);
	~cClientHandle();

	static const int VIEWDISTANCE = 13; // MUST be odd number or CRASH!
	static const int GENERATEDISTANCE = 2; // Server generates this many chunks AHEAD of player sight.

	const cSocket & GetSocket();
	cPlayer* GetPlayer() { return m_Player; }	// tolua_export

	void Kick( const char* a_Reason ); //tolua_export

	void AddPacket( cPacket * a_Packet );
	void HandlePendingPackets();

	void StreamChunks();
	void StreamChunksSmart( cChunk** a_Chunks, unsigned int a_NumChunks );
	void RemoveFromAllChunks();

	inline void SetLoggedIn( bool a_bLoggedIn ) { m_bLoggedIn = a_bLoggedIn; }
	inline bool IsLoggedIn() { return m_bLoggedIn; }

	void Tick(float a_Dt);

	bool IsDestroyed() { return m_bDestroyed; }
	void Destroy();

	cChunk* m_LoadedChunks[VIEWDISTANCE*VIEWDISTANCE];

	void Send( const cPacket & a_Packet, ENUM_PRIORITY a_Priority = E_PRIORITY_NORMAL );

	static void SendThread( void *lpParam );
	static void ReceiveThread( void *lpParam );
	static void AuthenticateThread( void* a_Param );

	const char* GetUsername();
	
	inline short GetPing() { return m_Ping; }
private:
	void HandlePacket( cPacket* a_Packet );
	void RemovePacket( cPacket * a_Packet );

	void SendLoginResponse();

	struct sClientHandleState;
	sClientHandleState* m_pState;

	bool m_bDestroyed;
	cPlayer* m_Player;
	bool m_bKicking;

	float m_TimeLastPacket;

	short m_Ping;
	int m_PingID;
	long long m_PingStartTime;
	long long m_LastPingTime;
	static const unsigned short PING_TIME_MS = 1000; //minecraft sends 1 per 20 ticks (1 second or every 1000 ms)

	bool m_bLoggedIn;
	bool m_bPositionConfirmed;
	bool m_bSendLoginResponse;

	bool m_bKeepThreadGoing;
};										// tolua_export
