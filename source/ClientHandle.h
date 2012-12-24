
// cClientHandle.h

// Interfaces to the cClientHandle class representing a client connected to this server. The client need not be a player yet





#pragma once
#ifndef CCLIENTHANDLE_H_INCLUDED
#define CCLIENTHANDLE_H_INCLUDED

#include "Defines.h"
#include "Vector3d.h"
#include "OSSupport/SocketThreads.h"
#include "ChunkDef.h"
#include "ByteBuffer.h"





class cChunkDataSerializer;
class cInventory;
class cMonster;
class cPawn;
class cPickup;
class cPlayer;
class cProtocol;
class cRedstone;
class cWindow;





class cClientHandle :  // tolua_export
	public cSocketThreads::cCallback
{											// tolua_export
public:
	enum ENUM_PRIORITY
	{
		E_PRIORITY_LOW,
		E_PRIORITY_NORMAL
	};

	static const int MAXBLOCKCHANGEINTERACTIONS = 20; // 5 didn't help, 10 still doesn't work in Creative, 20 seems to have done the trick
	
#if defined(ANDROID_NDK)
	static const int DEFAULT_VIEW_DISTANCE = 4;  // The default ViewDistance (used when no value is set in Settings.ini)
#else
	static const int DEFAULT_VIEW_DISTANCE = 9;
#endif
	static const int MAX_VIEW_DISTANCE = 10;
	static const int MIN_VIEW_DISTANCE = 4;

	cClientHandle(const cSocket * a_Socket, int a_ViewDistance);
	virtual ~cClientHandle();

	const AString & GetIPString(void) const { return m_IPString; }
	
	cPlayer* GetPlayer() { return m_Player; }	// tolua_export

	void Kick(const AString & a_Reason);		//tolua_export
	void Authenticate(void);  // Called by cAuthenticator when the user passes authentication

	void StreamChunks(void);
	
	// Removes the client from all chunks. Used when switching worlds or destroying the player
	void RemoveFromAllChunks(void);
	
	inline bool IsLoggedIn(void) const { return (m_State >= csAuthenticating); }

	void Tick(float a_Dt);

	void Destroy(void);
	
	bool IsPlaying   (void) const { return (m_State == csPlaying); }
	bool IsDestroyed (void) const { return (m_State == csDestroyed); }
	bool IsDestroying(void) const { return (m_State == csDestroying); }

	void SendBlockAction        (int a_BlockX, int a_BlockY, int a_BlockZ, char a_Byte1, char a_Byte2, BLOCKTYPE a_BlockType);
	void SendBlockChange        (int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta);
	void SendBlockChanges       (int a_ChunkX, int a_ChunkZ, const sSetBlockVector & a_Changes);
	void SendChat               (const AString & a_Message);
	void SendChunkData          (int a_ChunkX, int a_ChunkZ, cChunkDataSerializer & a_Serializer);
	void SendCollectPickup      (const cPickup & a_Pickup, const cPlayer & a_Player);
	void SendDestroyEntity      (const cEntity & a_Entity);
	void SendDisconnect         (const AString & a_Reason);
	void SendEntHeadLook        (const cEntity & a_Entity);
	void SendEntLook            (const cEntity & a_Entity);
	void SendEntRelMove         (const cEntity & a_Entity, char a_RelX, char a_RelY, char a_RelZ);
	void SendEntRelMoveLook     (const cEntity & a_Entity, char a_RelX, char a_RelY, char a_RelZ);
	void SendEntityEquipment    (const cEntity & a_Entity, short a_SlotNum, const cItem & a_Item);
	void SendEntityStatus       (const cEntity & a_Entity, char a_Status);
	void SendGameMode           (eGameMode a_GameMode);
	void SendHealth             (void);
	void SendInventoryProgress  (char a_WindowID, short a_Progressbar, short a_Value);
	void SendInventorySlot      (char a_WindowID, short a_SlotNum, const cItem & a_Item);
	void SendMetadata           (const cPawn & a_Entity);
	void SendPickupSpawn        (const cPickup & a_Pickup);
	void SendPlayerAnimation    (const cPlayer & a_Player, char a_Animation);
	void SendPlayerListItem     (const cPlayer & a_Player, bool a_IsOnline);
	void SendPlayerMoveLook     (void);
	void SendPlayerPosition     (void);
	void SendPlayerSpawn        (const cPlayer & a_Player);
	void SendRespawn            (void);
	void SendSoundEffect        (const AString & a_SoundName, int a_SrcX, int a_SrcY, int a_SrcZ, float a_Volume, float a_Pitch);  // a_Src coords are Block * 8
	void SendSoundParticleEffect(int a_EffectID, int a_SrcX, int a_SrcY, int a_SrcZ, int a_Data);
	void SendBlockBreakAnim     (int a_entityID, int a_blockX, int a_blockY, int a_blockZ, char a_stage);
	void SendSpawnMob           (const cMonster & a_Mob);
	void SendTeleportEntity     (const cEntity & a_Entity);
	void SendThunderbolt        (int a_BlockX, int a_BlockY, int a_BlockZ);
	void SendTimeUpdate         (Int64 a_WorldAge, Int64 a_TimeOfDay);
	void SendUnloadChunk        (int a_ChunkX, int a_ChunkZ);
	void SendUpdateSign         (int a_BlockX, int a_BlockY, int a_BlockZ, const AString & a_Line1, const AString & a_Line2, const AString & a_Line3, const AString & a_Line4);
	void SendWeather            (eWeather a_Weather);
	void SendWholeInventory     (const cInventory & a_Inventory);
	void SendWholeInventory     (const cWindow    & a_Window);
	void SendWindowClose        (char a_WindowID);
	void SendWindowOpen         (char a_WindowID, char a_WindowType, const AString & a_WindowTitle, char a_NumSlots);
	void SendUseBed             (const cEntity & a_Entity, int a_BlockX, int a_BlockY, int a_BlockZ );
	void SendSpawnObject        (const cEntity & a_Entity, char a_ObjectType, int a_ObjectData, short a_SpeedX, short a_SpeedY, short a_SpeedZ, Byte a_Yaw, Byte a_Pitch);

	const AString & GetUsername(void) const;		//tolua_export
	void SetUsername( const AString & a_Username );	//tolua_export
	
	inline short GetPing(void) const { return m_Ping; }	//tolua_export
	
	void SetViewDistance(int a_ViewDistance);		// tolua_export
	int  GetViewDistance(void) const { return m_ViewDistance; }  // tolua_export

	int GetUniqueID() const { return m_UniqueID; }	//tolua_export
	
	/// Returns true if the client wants the chunk specified to be sent (in m_ChunksToSend)
	bool WantsSendChunk(int a_ChunkX, int a_ChunkY, int a_ChunkZ);
	
	/// Adds the chunk specified to the list of chunks wanted for sending (m_ChunksToSend)
	void AddWantedChunk(int a_ChunkX, int a_ChunkZ);
	
	// Calls that cProtocol descendants use to report state:
	void PacketBufferFull(void);
	void PacketUnknown(unsigned char a_PacketType);
	void PacketError(unsigned char a_PacketType);

	// Calls that cProtocol descendants use for handling packets:
	void HandlePing             (void);
	void HandleCreativeInventory(short a_SlotNum, const cItem & a_HeldItem);
	void HandlePlayerPos        (double a_PosX, double a_PosY, double a_PosZ, double a_Stance, bool a_IsOnGround);
	void HandleBlockDig         (int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace, char a_Status);
	void HandleBlockPlace       (int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace, const cItem & a_HeldItem);
	void HandleChat             (const AString & a_Message);
	void HandlePlayerLook       (float a_Rotation, float a_Pitch, bool a_IsOnGround);
	void HandlePlayerMoveLook   (double a_PosX, double a_PosY, double a_PosZ, double a_Stance, float a_Rotation, float a_Pitch, bool a_IsOnGround);  // While m_bPositionConfirmed (normal gameplay)
	void HandleAnimation        (char a_Animation);
	void HandleSlotSelected     (short a_SlotNum);
	void HandleWindowClose      (char a_WindowID);
	void HandleWindowClick      (char a_WindowID, short a_SlotNum, bool a_IsRightClick, bool a_IsShiftPressed, const cItem & a_HeldItem);
	void HandleUpdateSign       (
		int a_BlockX, int a_BlockY, int a_BlockZ, 
		const AString & a_Line1, const AString & a_Line2, 
		const AString & a_Line3, const AString & a_Line4
	);
	void HandleUseEntity        (int a_TargetEntityID, bool a_IsLeftClick);
	void HandleRespawn          (void);
	void HandleDisconnect       (const AString & a_Reason);
	void HandleKeepAlive        (int a_KeepAliveID);
	bool HandleHandshake        (const AString & a_Username);
	void HandleEntityAction     (int a_EntityID, char a_ActionID);
	
	/** Called when the protocol has finished logging the user in.
	Return true to allow the user in; false to kick them.
	*/
	bool HandleLogin(int a_ProtocolVersion, const AString & a_Username);
	
	void SendData(const char * a_Data, int a_Size);
private:

	int m_ViewDistance;  // Number of chunks the player can see in each direction; 4 is the minimum ( http://wiki.vg/Protocol_FAQ#.E2.80.A6all_connecting_clients_spasm_and_jerk_uncontrollably.21 )
	
	static const int GENERATEDISTANCE = 2; // Server generates this many chunks AHEAD of player sight. 2 is the minimum, since foliage is generated 1 step behind chunk terrain generation
	
	AString m_IPString;

	int     m_ProtocolVersion;
	AString m_Username;
	AString m_Password;

	cCriticalSection m_CSChunkLists;
	cChunkCoordsList m_LoadedChunks;  // Chunks that the player belongs to
	cChunkCoordsList m_ChunksToSend;  // Chunks that need to be sent to the player (queued because they weren't generated yet or there's not enough time to send them)

	cProtocol * m_Protocol;
	
	cCriticalSection m_CSOutgoingData;
	cByteBuffer      m_OutgoingData;
	AString          m_OutgoingDataOverflow;  //< For data that didn't fit into the m_OutgoingData ringbuffer temporarily

	cCriticalSection m_CriticalSection;

	Vector3d m_ConfirmPosition;

	cPlayer * m_Player;
	bool      m_bKicking;
	
	// Chunk position when the last StreamChunks() was called; used to avoid re-streaming while in the same chunk
	int m_LastStreamedChunkX;
	int m_LastStreamedChunkZ;

	/// Seconds since the last packet data was received (updated in Tick(), reset in DataReceived())
	float m_TimeSinceLastPacket;
	
	short m_Ping;
	int   m_PingID;
	long long m_PingStartTime;
	long long m_LastPingTime;
	static const unsigned short PING_TIME_MS = 1000; //minecraft sends 1 per 20 ticks (1 second or every 1000 ms)
	
	// Values required for block dig animation
	int m_BlockDigAnim;  // Current stage of the animation; -1 if not digging
	int m_BlockDigAnimSpeed;  // Current speed of the animation (units ???)
	int m_BlockDigX;
	int m_BlockDigY;
	int m_BlockDigZ;

	char m_LastDigStatus;
	int m_LastDigX;
	int m_LastDigY;
	int m_LastDigZ;

	enum eState
	{
		csConnected,         // The client has just connected, waiting for their handshake / login
		csAuthenticating,    // The client has logged in, waiting for external authentication
		csDownloadingWorld,  // The client is waiting for chunks, we're waiting for the loader to provide and send them
 		csConfirmingPos,     // The client has been sent the position packet, waiting for them to repeat the position back
		csPlaying,           // Normal gameplay
		csDestroying,        // The client is being destroyed, don't queue any more packets / don't add to chunks
		csDestroyed,         // The client has been destroyed, the destructor is to be called from the owner thread
		
		// TODO: Add Kicking here as well
	} ;
	
	eState m_State;

	bool m_bKeepThreadGoing;
	
	/// If set to true during csDownloadingWorld, the tick thread calls CheckIfWorldDownloaded()
	bool m_ShouldCheckDownloaded;

	/// Returns true if the rate block interactions is within a reasonable limit (bot protection)
	bool CheckBlockInteractionsRate(void);
	
	/// Checks whether all loaded chunks have been sent to the client; if so, sends the position to confirm
	void CheckIfWorldDownloaded(void);
	
	/// Sends the PlayerMoveLook packet that the client needs to reply to for the game to start
	void SendConfirmPosition(void);
	
	/// Adds a single chunk to be streamed to the client; used by StreamChunks()
	void StreamChunk(int a_ChunkX, int a_ChunkY, int a_ChunkZ);
	
	// cSocketThreads::cCallback overrides:
	virtual void DataReceived   (const char * a_Data, int a_Size) override;  // Data is received from the client
	virtual void GetOutgoingData(AString & a_Data) override;  // Data can be sent to client
	virtual void SocketClosed   (void) override;  // The socket has been closed for any reason

	static int s_ClientCount;
	int m_UniqueID;
};										// tolua_export




#endif  // CCLIENTHANDLE_H_INCLUDED




