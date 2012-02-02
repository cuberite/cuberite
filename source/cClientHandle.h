
// cClientHandle.h

// Interfaces to the cClientHandle class representing a client connected to this server. The client need not be a player yet





#pragma once
#ifndef CCLIENTHANDLE_H_INCLUDED
#define CCLIENTHANDLE_H_INCLUDED

#include "Packets/cPacket.h"
#include "Vector3d.h"





// class Game;
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

	static const int VIEWDISTANCE = 17; // MUST be odd number or CRASH!
	static const int GENERATEDISTANCE = 2; // Server generates this many chunks AHEAD of player sight.

	const cSocket & GetSocket();
	cPlayer* GetPlayer() { return m_Player; }	// tolua_export

	void Kick(const AString & a_Reason); //tolua_export
	void Authenticate(void);  // Called by cAuthenticator when the user passes authentication

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

	const AString & GetUsername(void) const;
	
	inline short GetPing() { return m_Ping; }
private:
	void HandlePacket( cPacket* a_Packet );
	void RemovePacket( cPacket * a_Packet );

	void SendLoginResponse();

	int mProtocolVersion;
	AString mUsername;
	AString mPassword;

	PacketList mPendingParsePackets;
	PacketList mPendingNrmSendPackets;
	PacketList mPendingLowSendPackets;

	cThread* pReceiveThread;
	cThread* pSendThread;

	cSocket mSocket;

	cCriticalSection mCriticalSection;
	cCriticalSection mSendCriticalSection;
	cCriticalSection mSocketCriticalSection;
	cSemaphore       mSemaphore;

	Vector3d mConfirmPosition;

	cPacket * mPacketMap[256];

	bool      m_bDestroyed;
	cPlayer * m_Player;
	bool      m_bKicking;

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




#endif  // CCLIENTHANDLE_H_INCLUDED




