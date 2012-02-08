
// cClientHandle.h

// Interfaces to the cClientHandle class representing a client connected to this server. The client need not be a player yet





#pragma once
#ifndef CCLIENTHANDLE_H_INCLUDED
#define CCLIENTHANDLE_H_INCLUDED

#include "packets/cPacket.h"
#include "Vector3d.h"
#include "cSocketThreads.h"

#include "packets/cPacket_KeepAlive.h"
#include "packets/cPacket_PlayerPosition.h"
#include "packets/cPacket_Respawn.h"
#include "packets/cPacket_RelativeEntityMoveLook.h"
#include "packets/cPacket_Chat.h"
#include "packets/cPacket_Login.h"
#include "packets/cPacket_WindowClick.h"
#include "packets/cPacket_PlayerMoveLook.h"
#include "packets/cPacket_TimeUpdate.h"
#include "packets/cPacket_BlockDig.h"
#include "packets/cPacket_Handshake.h"
#include "packets/cPacket_PlayerLook.h"
#include "packets/cPacket_ArmAnim.h"
#include "packets/cPacket_BlockPlace.h"
#include "packets/cPacket_Flying.h"
#include "packets/cPacket_Disconnect.h"
#include "packets/cPacket_PickupSpawn.h"
#include "packets/cPacket_ItemSwitch.h"
#include "packets/cPacket_EntityEquipment.h"
#include "packets/cPacket_CreativeInventoryAction.h"
#include "packets/cPacket_NewInvalidState.h"
#include "packets/cPacket_UseEntity.h"
#include "packets/cPacket_WindowClose.h"
#include "packets/cPacket_UpdateSign.h"
#include "packets/cPacket_Ping.h"
#include "packets/cPacket_PlayerListItem.h"





// class Game;
class cChunk;
class cPlayer;
class cRedstone;





class cClientHandle :  // tolua_export
	public cSocketThreads::cCallback
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

	const cSocket & GetSocket(void) const {return m_Socket; }
	cSocket &       GetSocket(void)       {return m_Socket; }
	
	cPlayer* GetPlayer() { return m_Player; }	// tolua_export

	void Kick(const AString & a_Reason); //tolua_export
	void Authenticate(void);  // Called by cAuthenticator when the user passes authentication

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

	const AString & GetUsername(void) const;
	
	inline short GetPing() { return m_Ping; }

private:

	int m_ProtocolVersion;
	AString m_Username;
	AString m_Password;
	
	AString m_ReceivedData;  // Accumulator for the data received from the socket, waiting to be parsed; accessed from the cSocketThreads' thread only!

	PacketList m_PendingNrmSendPackets;
	PacketList m_PendingLowSendPackets;

	cThread * m_pSendThread;

	cSocket m_Socket;

	cCriticalSection m_CriticalSection;
	cCriticalSection m_SendCriticalSection;
	// cCriticalSection m_SocketCriticalSection;
	cSemaphore       m_Semaphore;

	Vector3d m_ConfirmPosition;

	cPacket * m_PacketMap[256];

	bool      m_bDestroyed;
	cPlayer * m_Player;
	bool      m_bKicking;

	float m_TimeLastPacket;

	short m_Ping;
	int   m_PingID;
	long long m_PingStartTime;
	long long m_LastPingTime;
	static const unsigned short PING_TIME_MS = 1000; //minecraft sends 1 per 20 ticks (1 second or every 1000 ms)

	bool m_bLoggedIn;
	bool m_bPositionConfirmed;
	bool m_bSendLoginResponse;

	bool m_bKeepThreadGoing;

	void HandlePacket(cPacket * a_Packet);
	
	// Packets handled while !m_bLoggedIn:
	void HandlePing         (void);
	void HandleHandshake    (cPacket_Handshake *      a_Packet);
	void HandleLogin        (cPacket_Login *          a_Packet);
	void HandleMoveLookLogin(cPacket_PlayerMoveLook * a_Packet);  // While !m_bLoggedIn
	void HandleDefaultLogin (cPacket *                a_Packet);  // the default case
	
	// Packets handled while !m_bPositionConfirmed:
	void HandleMoveLookConfirm(cPacket_PlayerMoveLook * a_Packet);  // While !m_bPositionConfirmed
	
	// Packets handled while m_bPositionConfirmed (normal gameplay):
	void HandleCreativeInventory(cPacket_CreativeInventoryAction * a_Packet);
	void HandlePlayerPos        (cPacket_PlayerPosition *          a_Packet);
	void HandleBlockDig         (cPacket_BlockDig *                a_Packet);
	void HandleBlockPlace       (cPacket_BlockPlace *              a_Packet);
	void HandlePickupSpawn      (cPacket_PickupSpawn *             a_Packet);
	void HandleChat             (cPacket_Chat *                    a_Packet);
	void HandlePlayerLook       (cPacket_PlayerLook *              a_Packet);
	void HandlePlayerMoveLook   (cPacket_PlayerMoveLook *          a_Packet);  // While m_bPositionConfirmed (normal gameplay)
	void HandleAnimation        (cPacket_ArmAnim *                 a_Packet);
	void HandleItemSwitch       (cPacket_ItemSwitch *              a_Packet);
	void HandleWindowClose      (cPacket_WindowClose *             a_Packet);
	void HandleWindowClick      (cPacket_WindowClick *             a_Packet);
	void HandleUpdateSign       (cPacket_UpdateSign *              a_Packet);
	void HandleUseEntity        (cPacket_UseEntity *               a_Packet);
	void HandleRespawn          (void);
	void HandleDisconnect       (cPacket_Disconnect *              a_Packet);
	void HandleKeepAlive        (cPacket_KeepAlive *               a_Packet);

	/// Returns true if the rate block interactions is within a reasonable limit (bot protection)
	bool CheckBlockInteractionsRate(void);
	
	void SendLoginResponse();
	
	// cSocketThreads::cCallback overrides:
	virtual void DataReceived   (const char * a_Data, int a_Size) override;  // Data is received from the client
	virtual void GetOutgoingData(AString & a_Data) override;  // Data can be sent to client
	virtual void SocketClosed   (void) override;  // The socket has been closed for any reason

};										// tolua_export




#endif  // CCLIENTHANDLE_H_INCLUDED




