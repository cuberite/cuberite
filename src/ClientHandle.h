
// cClientHandle.h

// Interfaces to the cClientHandle class representing a client connected to this server. The client need not be a player yet





#pragma once
#ifndef CCLIENTHANDLE_H_INCLUDED
#define CCLIENTHANDLE_H_INCLUDED

#include "Defines.h"
#include "Vector3.h"
#include "OSSupport/SocketThreads.h"
#include "ChunkDef.h"
#include "ByteBuffer.h"
#include "Scoreboard.h"
#include "Map.h"
#include "Enchantments.h"
#include "UI/SlotArea.h"





class cChunkDataSerializer;
class cInventory;
class cMonster;
class cPawn;
class cExpOrb;
class cPainting;
class cPickup;
class cPlayer;
class cProtocol;
class cWindow;
class cFallingBlock;
class cItemHandler;
class cWorld;
class cCompositeChat;





class cClientHandle :  // tolua_export
	public cSocketThreads::cCallback
{											// tolua_export
public:
	
#if defined(ANDROID_NDK)
	static const int DEFAULT_VIEW_DISTANCE = 4;  // The default ViewDistance (used when no value is set in Settings.ini)
#else
	static const int DEFAULT_VIEW_DISTANCE = 10;
#endif
	static const int MAX_VIEW_DISTANCE = 15;
	static const int MIN_VIEW_DISTANCE = 3;
	
	cClientHandle(const cSocket * a_Socket, int a_ViewDistance);
	virtual ~cClientHandle();

	const AString & GetIPString(void) const { return m_IPString; }
	
	cPlayer* GetPlayer() { return m_Player; }	// tolua_export

	const AString & GetUUID(void) const { return m_UUID; } // tolua_export
	void SetUUID(const AString & a_UUID) { m_UUID = a_UUID; }
	
	/** Generates an UUID based on the username stored for this client, and stores it in the m_UUID member.
	This is used for the offline (non-auth) mode, when there's no UUID source.
	Each username generates a unique and constant UUID, so that when the player reconnects with the same name, their UUID is the same.
	Internally calls the GenerateOfflineUUID static function. */
	void GenerateOfflineUUID(void);
	
	/** Generates an UUID based on the player name provided.
	This is used for the offline (non-auth) mode, when there's no UUID source.
	Each username generates a unique and constant UUID, so that when the player reconnects with the same name, their UUID is the same. */
	static AString GenerateOfflineUUID(const AString & a_Username);  // tolua_export
	
	/** Formats the type of message with the proper color and prefix for sending to the client. **/
	AString FormatMessageType(bool ShouldAppendChatPrefixes, eMessageType a_ChatPrefix, const AString & a_AdditionalData);
	
	AString FormatChatPrefix(bool ShouldAppendChatPrefixes, AString a_ChatPrefixS, AString m_Color1, AString m_Color2);

	void Kick(const AString & a_Reason);		// tolua_export
	void Authenticate(const AString & a_Name, const AString & a_UUID);  // Called by cAuthenticator when the user passes authentication

	void StreamChunks(void);
	
	// Removes the client from all chunks. Used when switching worlds or destroying the player
	void RemoveFromAllChunks(void);
	
	inline bool IsLoggedIn(void) const { return (m_State >= csAuthenticating); }

	/** Called while the client is being ticked from the world via its cPlayer object */
	void Tick(float a_Dt);
	
	/** Called while the client is being ticked from the cServer object */
	void ServerTick(float a_Dt);

	void Destroy(void);
	
	bool IsPlaying   (void) const { return (m_State == csPlaying); }
	bool IsDestroyed (void) const { return (m_State == csDestroyed); }
	bool IsDestroying(void) const { return (m_State == csDestroying); }

	// The following functions send the various packets:
	// (Please keep these alpha-sorted)
	void SendAttachEntity        (const cEntity & a_Entity, const cEntity * a_Vehicle);
	void SendBlockAction         (int a_BlockX, int a_BlockY, int a_BlockZ, char a_Byte1, char a_Byte2, BLOCKTYPE a_BlockType);
	void SendBlockBreakAnim      (int a_EntityID, int a_BlockX, int a_BlockY, int a_BlockZ, char a_Stage);
	void SendBlockChange         (int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta); // tolua_export
	void SendBlockChanges        (int a_ChunkX, int a_ChunkZ, const sSetBlockVector & a_Changes);
	void SendChat                (const AString & a_Message, eMessageType a_ChatPrefix, const AString & a_AdditionalData = "");
	void SendChat                (const cCompositeChat & a_Message);
	void SendChunkData           (int a_ChunkX, int a_ChunkZ, cChunkDataSerializer & a_Serializer);
	void SendCollectPickup       (const cPickup & a_Pickup, const cPlayer & a_Player);
	void SendDestroyEntity       (const cEntity & a_Entity);
	void SendDisconnect          (const AString & a_Reason);
	void SendEditSign            (int a_BlockX, int a_BlockY, int a_BlockZ);
	void SendEntityEffect        (const cEntity & a_Entity, int a_EffectID, int a_Amplifier, short a_Duration);
	void SendEntityEquipment     (const cEntity & a_Entity, short a_SlotNum, const cItem & a_Item);
	void SendEntityHeadLook      (const cEntity & a_Entity);
	void SendEntityLook          (const cEntity & a_Entity);
	void SendEntityMetadata      (const cEntity & a_Entity);
	void SendEntityProperties    (const cEntity & a_Entity);
	void SendEntityRelMove       (const cEntity & a_Entity, char a_RelX, char a_RelY, char a_RelZ);
	void SendEntityRelMoveLook   (const cEntity & a_Entity, char a_RelX, char a_RelY, char a_RelZ);
	void SendEntityStatus        (const cEntity & a_Entity, char a_Status);
	void SendEntityVelocity      (const cEntity & a_Entity);
	void SendExplosion           (double a_BlockX, double a_BlockY, double a_BlockZ, float a_Radius, const cVector3iArray & a_BlocksAffected, const Vector3d & a_PlayerMotion);
	void SendGameMode            (eGameMode a_GameMode);
	void SendHealth              (void);
	void SendInventorySlot       (char a_WindowID, short a_SlotNum, const cItem & a_Item);
	void SendMapColumn           (int a_ID, int a_X, int a_Y, const Byte * a_Colors, unsigned int a_Length);
	void SendMapDecorators       (int a_ID, const cMapDecoratorList & a_Decorators);
	void SendMapInfo             (int a_ID, unsigned int a_Scale);
	void SendPaintingSpawn       (const cPainting & a_Painting);
	void SendPickupSpawn         (const cPickup & a_Pickup);
	void SendEntityAnimation     (const cEntity & a_Entity, char a_Animation);
	void SendParticleEffect      (const AString & a_ParticleName, float a_SrcX, float a_SrcY, float a_SrcZ, float a_OffsetX, float a_OffsetY, float a_OffsetZ, float a_ParticleData, int a_ParticleAmmount);
	void SendPlayerAbilities     (void);
	void SendPlayerListItem      (const cPlayer & a_Player, bool a_IsOnline);
	void SendPlayerMaxSpeed      (void);  ///< Informs the client of the maximum player speed (1.6.1+)
	void SendPlayerMoveLook      (void);
	void SendPlayerPosition      (void);
	void SendPlayerSpawn         (const cPlayer & a_Player);
	void SendPluginMessage       (const AString & a_Channel, const AString & a_Message);  // Exported in ManualBindings.cpp
	void SendRemoveEntityEffect  (const cEntity & a_Entity, int a_EffectID);
	void SendRespawn             (void);
	void SendExperience          (void);
	void SendExperienceOrb       (const cExpOrb & a_ExpOrb);
	void SendScoreboardObjective (const AString & a_Name, const AString & a_DisplayName, Byte a_Mode);
	void SendScoreUpdate         (const AString & a_Objective, const AString & a_Player, cObjective::Score a_Score, Byte a_Mode);
	void SendDisplayObjective    (const AString & a_Objective, cScoreboard::eDisplaySlot a_Display);
	void SendSoundEffect         (const AString & a_SoundName, int a_SrcX, int a_SrcY, int a_SrcZ, float a_Volume, float a_Pitch);  // a_Src coords are Block * 8
	void SendSoundParticleEffect (int a_EffectID, int a_SrcX, int a_SrcY, int a_SrcZ, int a_Data);
	void SendSpawnFallingBlock   (const cFallingBlock & a_FallingBlock);
	void SendSpawnMob            (const cMonster & a_Mob);
	void SendSpawnObject         (const cEntity & a_Entity, char a_ObjectType, int a_ObjectData, Byte a_Yaw, Byte a_Pitch);
	void SendSpawnVehicle        (const cEntity & a_Vehicle, char a_VehicleType, char a_VehicleSubType = 0);
	void SendTabCompletionResults(const AStringVector & a_Results);
	void SendTeleportEntity      (const cEntity & a_Entity);
	void SendThunderbolt         (int a_BlockX, int a_BlockY, int a_BlockZ);
	void SendTimeUpdate          (Int64 a_WorldAge, Int64 a_TimeOfDay);
	void SendUnloadChunk         (int a_ChunkX, int a_ChunkZ);
	void SendUpdateBlockEntity   (cBlockEntity & a_BlockEntity);
	void SendUpdateSign          (int a_BlockX, int a_BlockY, int a_BlockZ, const AString & a_Line1, const AString & a_Line2, const AString & a_Line3, const AString & a_Line4);
	void SendUseBed              (const cEntity & a_Entity, int a_BlockX, int a_BlockY, int a_BlockZ );
	void SendWeather             (eWeather a_Weather);
	void SendWholeInventory      (const cWindow & a_Window);
	void SendWindowClose         (const cWindow & a_Window);
	void SendWindowOpen          (const cWindow & a_Window);
	void SendWindowProperty      (const cWindow & a_Window, int a_Property, int a_Value);

	// tolua_begin
	const AString & GetUsername(void) const;
	void SetUsername( const AString & a_Username );
	
	inline short GetPing(void) const { return m_Ping; }
	
	void SetViewDistance(int a_ViewDistance);
	int  GetViewDistance(void) const { return m_ViewDistance; }
	
	void SetLocale(AString & a_Locale) { m_Locale = a_Locale; }
	AString GetLocale(void) const { return m_Locale; }

	int GetUniqueID(void) const { return m_UniqueID; }
	
	bool HasPluginChannel(const AString & a_PluginChannel);
	
	// tolua_end
	
	/** Returns true if the client wants the chunk specified to be sent (in m_ChunksToSend) */
	bool WantsSendChunk(int a_ChunkX, int a_ChunkY, int a_ChunkZ);
	
	/** Adds the chunk specified to the list of chunks wanted for sending (m_ChunksToSend) */
	void AddWantedChunk(int a_ChunkX, int a_ChunkZ);
	
	// Calls that cProtocol descendants use to report state:
	void PacketBufferFull(void);
	void PacketUnknown(UInt32 a_PacketType);
	void PacketError(unsigned char a_PacketType);

	// Calls that cProtocol descendants use for handling packets:
	void HandleAnimation        (char a_Animation);
	void HandleChat             (const AString & a_Message);
	void HandleCreativeInventory(short a_SlotNum, const cItem & a_HeldItem);
	void HandleDisconnect       (const AString & a_Reason);
	void HandleEntityCrouch     (int a_EntityID, bool a_IsCrouching);
	void HandleEntityLeaveBed   (int a_EntityID);
	void HandleEntitySprinting  (int a_EntityID, bool a_IsSprinting);
	
	/** Called when the protocol handshake has been received (for protocol versions that support it;
	otherwise the first instant when a username is received).
	Returns true if the player is to be let in, false if they were disconnected
	*/
	bool HandleHandshake        (const AString & a_Username);
	
	void HandleKeepAlive        (int a_KeepAliveID);
	void HandleLeftClick        (int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace, char a_Status);
	void HandlePing             (void);
	void HandlePlayerAbilities  (bool a_CanFly, bool a_IsFlying, float FlyingSpeed, float WalkingSpeed);
	void HandlePlayerLook       (float a_Rotation, float a_Pitch, bool a_IsOnGround);
	void HandlePlayerMoveLook   (double a_PosX, double a_PosY, double a_PosZ, double a_Stance, float a_Rotation, float a_Pitch, bool a_IsOnGround);  // While m_bPositionConfirmed (normal gameplay)
	void HandlePlayerPos        (double a_PosX, double a_PosY, double a_PosZ, double a_Stance, bool a_IsOnGround);
	void HandlePluginMessage    (const AString & a_Channel, const AString & a_Message);
	void HandleRespawn          (void);
	void HandleRightClick       (int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace, int a_CursorX, int a_CursorY, int a_CursorZ, const cItem & a_HeldItem);
	void HandleSlotSelected     (short a_SlotNum);
	void HandleSteerVehicle     (float Forward, float Sideways);
	void HandleTabCompletion    (const AString & a_Text);
	void HandleUpdateSign       (
		int a_BlockX, int a_BlockY, int a_BlockZ, 
		const AString & a_Line1, const AString & a_Line2, 
		const AString & a_Line3, const AString & a_Line4
	);
	void HandleUnmount          (void);
	void HandleUseEntity        (int a_TargetEntityID, bool a_IsLeftClick);
	void HandleWindowClick      (char a_WindowID, short a_SlotNum, eClickAction a_ClickAction, const cItem & a_HeldItem);
	void HandleWindowClose      (char a_WindowID);

	/** Called when the protocol has finished logging the user in.
	Return true to allow the user in; false to kick them.
	*/
	bool HandleLogin(int a_ProtocolVersion, const AString & a_Username);
	
	void SendData(const char * a_Data, size_t a_Size);
	
	/** Called when the player moves into a different world; queues sreaming the new chunks */
	void MoveToWorld(cWorld & a_World, bool a_SendRespawnPacket);
	
	/** Called when the player will enchant a Item */
	void HandleEnchantItem(Byte & WindowID, Byte & Enchantment);
	
private:

	/** Handles the block placing packet when it is a real block placement (not block-using, item-using or eating) */
	void HandlePlaceBlock(int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace, int a_CursorX, int a_CursorY, int a_CursorZ, cItemHandler & a_ItemHandler);

	/** The type used for storing the names of registered plugin channels. */
	typedef std::set<AString> cChannels;

	int m_ViewDistance;  // Number of chunks the player can see in each direction; 4 is the minimum ( http://wiki.vg/Protocol_FAQ#.E2.80.A6all_connecting_clients_spasm_and_jerk_uncontrollably.21 )
	
	static const int GENERATEDISTANCE = 2; // Server generates this many chunks AHEAD of player sight. 2 is the minimum, since foliage is generated 1 step behind chunk terrain generation
	
	AString m_IPString;

	AString m_Username;
	AString m_Password;

	cCriticalSection m_CSChunkLists;
	cChunkCoordsList m_LoadedChunks;  // Chunks that the player belongs to
	cChunkCoordsList m_ChunksToSend;  // Chunks that need to be sent to the player (queued because they weren't generated yet or there's not enough time to send them)

	cProtocol * m_Protocol;
	
	cCriticalSection m_CSIncomingData;
	AString          m_IncomingData;
	
	cCriticalSection m_CSOutgoingData;
	cByteBuffer      m_OutgoingData;
	AString          m_OutgoingDataOverflow;  ///< For data that didn't fit into the m_OutgoingData ringbuffer temporarily

	Vector3d m_ConfirmPosition;

	cPlayer * m_Player;
	
	bool m_HasSentDC;  ///< True if a D/C packet has been sent in either direction
	
	// Chunk position when the last StreamChunks() was called; used to avoid re-streaming while in the same chunk
	int m_LastStreamedChunkX;
	int m_LastStreamedChunkZ;

	/** Seconds since the last packet data was received (updated in Tick(), reset in DataReceived()) */
	float m_TimeSinceLastPacket;
	
	short m_Ping;
	int   m_PingID;
	long long m_PingStartTime;
	long long m_LastPingTime;
	static const unsigned short PING_TIME_MS = 1000; //minecraft sends 1 per 20 ticks (1 second or every 1000 ms)
	
	// Values required for block dig animation
	int m_BlockDigAnimStage;  // Current stage of the animation; -1 if not digging
	int m_BlockDigAnimSpeed;  // Current speed of the animation (units ???)
	int m_BlockDigAnimX;
	int m_BlockDigAnimY;
	int m_BlockDigAnimZ;

	// To avoid dig/aim bug in the client, store the last position given in a DIG_START packet and compare to that when processing the DIG_FINISH packet:
	bool m_HasStartedDigging;
	int m_LastDigBlockX;
	int m_LastDigBlockY;
	int m_LastDigBlockZ;
	
	/** Used while csDestroyedWaiting for counting the ticks until the connection is closed */
	int m_TicksSinceDestruction;

	enum eState
	{
		csConnected,         ///< The client has just connected, waiting for their handshake / login
		csAuthenticating,    ///< The client has logged in, waiting for external authentication
		csAuthenticated,     ///< The client has been authenticated, will start streaming chunks in the next tick
		csDownloadingWorld,  ///< The client is waiting for chunks, we're waiting for the loader to provide and send them
 		csConfirmingPos,     ///< The client has been sent the position packet, waiting for them to repeat the position back
		csPlaying,           ///< Normal gameplay
		csDestroying,        ///< The client is being destroyed, don't queue any more packets / don't add to chunks
		csDestroyedWaiting,  ///< The client has been destroyed, but is still kept so that the Kick packet is delivered (#31)
		csDestroyed,         ///< The client has been destroyed, the destructor is to be called from the owner thread
		
		// TODO: Add Kicking here as well
	} ;
	
	eState m_State;
	
	/** m_State needs to be locked in the Destroy() function so that the destruction code doesn't run twice on two different threads */
	cCriticalSection m_CSDestroyingState;

	/** If set to true during csDownloadingWorld, the tick thread calls CheckIfWorldDownloaded() */
	bool m_ShouldCheckDownloaded;

	/** Number of explosions sent this tick */
	int m_NumExplosionsThisTick;

	/** Number of place or break interactions this tick */
	int m_NumBlockChangeInteractionsThisTick;
	
	static int s_ClientCount;
	int m_UniqueID;
	AString m_UUID;
	
	/** Set to true when the chunk where the player is is sent to the client. Used for spawning the player */
	bool m_HasSentPlayerChunk;

	/** Client Settings */
	AString m_Locale;
	
	/** The plugin channels that the client has registered. */
	cChannels m_PluginChannels;


	/** Returns true if the rate block interactions is within a reasonable limit (bot protection) */
	bool CheckBlockInteractionsRate(void);
	
	/** Adds a single chunk to be streamed to the client; used by StreamChunks() */
	void StreamChunk(int a_ChunkX, int a_ChunkZ);
	
	/** Handles the DIG_STARTED dig packet: */
	void HandleBlockDigStarted (int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace, BLOCKTYPE a_OldBlock, NIBBLETYPE a_OldMeta);
	
	/** Handles the DIG_FINISHED dig packet: */
	void HandleBlockDigFinished(int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace, BLOCKTYPE a_OldBlock, NIBBLETYPE a_OldMeta);

	/** Converts the protocol-formatted channel list (NUL-separated) into a proper string vector. */
	AStringVector BreakApartPluginChannels(const AString & a_PluginChannels);
	
	/** Adds all of the channels to the list of current plugin channels. Handles duplicates gracefully. */
	void RegisterPluginChannels(const AStringVector & a_ChannelList);
	
	/** Removes all of the channels from the list of current plugin channels. Ignores channels that are not found. */
	void UnregisterPluginChannels(const AStringVector & a_ChannelList);
	
	/** Handles the "MC|AdvCdm" plugin message */
	void HandleCommandBlockMessage(const char * a_Data, size_t a_Length);

	/** Handles the "MC|ItemName" plugin message */
	void HandleAnvilItemName(const char * a_Data, size_t a_Length);
	
	// cSocketThreads::cCallback overrides:
	virtual void DataReceived   (const char * a_Data, size_t a_Size) override;  // Data is received from the client
	virtual void GetOutgoingData(AString & a_Data) override;  // Data can be sent to client
	virtual void SocketClosed   (void) override;  // The socket has been closed for any reason
};										// tolua_export




#endif  // CCLIENTHANDLE_H_INCLUDED




