
// Protocol17x.h

/*
Declares the 1.7.x protocol classes:
	- cProtocol172
		- release 1.7.2 protocol (#4)
	- cProtocol176
		- release 1.7.6 protocol (#5)
*/





#pragma once

#include "Protocol.h"
#include "LengthenedProtocol.h"
#include "../ByteBuffer.h"

#ifdef _MSC_VER
	#pragma warning(push)
	#pragma warning(disable:4127)
	#pragma warning(disable:4244)
	#pragma warning(disable:4231)
	#pragma warning(disable:4189)
	#pragma warning(disable:4702)
#endif

#ifdef _MSC_VER
	#pragma warning(pop)
#endif





// fwd:
namespace Json
{
	class Value;
}





class cProtocol172 :
	public cLengthenedProtocol
{
	typedef cLengthenedProtocol super;
	
public:

	cProtocol172(const AString a_LogID);

	/** Sending stuff to clients (alphabetically sorted): */
	virtual void SendAttachEntity               (const cEntity & a_Entity, const cEntity * a_Vehicle) override;
	virtual void SendBlockAction                (int a_BlockX, int a_BlockY, int a_BlockZ, char a_Byte1, char a_Byte2, BLOCKTYPE a_BlockType) override;
	virtual void SendBlockBreakAnim	            (UInt32 a_EntityID, int a_BlockX, int a_BlockY, int a_BlockZ, char a_Stage) override;
	virtual void SendBlockChange                (int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta) override;
	virtual void SendBlockChanges               (int a_ChunkX, int a_ChunkZ, const sSetBlockVector & a_Changes) override;
	virtual void SendChat                       (const AString & a_Message, eChatType a_Type) override;
	virtual void SendChat                       (const cCompositeChat & a_Message, eChatType a_Type, bool a_ShouldUseChatPrefixes) override;
	virtual void SendChunkData                  (int a_ChunkX, int a_ChunkZ, cChunkDataSerializer & a_Serializer) override;
	virtual void SendCollectEntity              (const cEntity & a_Entity, const cPlayer & a_Player) override;
	virtual void SendDestroyEntity              (const cEntity & a_Entity) override;
	virtual void SendDisconnect                 (AString & a_ByteBuffer, const AString & a_Reason) override;
	virtual void SendDisplayObjective           (const AString & a_Objective, cScoreboard::eDisplaySlot a_Display) override;
	virtual void SendEditSign                   (int a_BlockX, int a_BlockY, int a_BlockZ) override;  ///< Request the client to open up the sign editor for the sign (1.6+)
	virtual void SendEntityAnimation            (const cEntity & a_Entity, char a_Animation) override;
	virtual void SendEntityEffect               (const cEntity & a_Entity, int a_EffectID, int a_Amplifier, short a_Duration) override;
	virtual void SendEntityEquipment            (const cEntity & a_Entity, short a_SlotNum, const cItem & a_Item) override;
	virtual void SendEntityHeadLook             (const cEntity & a_Entity) override;
	virtual void SendEntityLook                 (const cEntity & a_Entity) override;
	virtual void SendEntityMetadata             (const cEntity & a_Entity) override;
	virtual void SendEntityProperties           (const cEntity & a_Entity) override;
	virtual void SendEntityRelMove              (const cEntity & a_Entity, char a_RelX, char a_RelY, char a_RelZ) override;
	virtual void SendEntityRelMoveLook          (const cEntity & a_Entity, char a_RelX, char a_RelY, char a_RelZ) override;
	virtual void SendEntityStatus               (const cEntity & a_Entity, char a_Status) override;
	virtual void SendEntityVelocity             (const cEntity & a_Entity) override;
	virtual void SendExperience                 (const cPlayer & a_Player) override;
	virtual void SendExperienceOrb              (const cExpOrb & a_ExpOrb) override;
	virtual void SendExplosion                  (double a_BlockX, double a_BlockY, double a_BlockZ, float a_Radius, const cVector3iArray & a_BlocksAffected, const Vector3d & a_PlayerMotion) override;
	virtual void SendGameMode                   (eGameMode a_GameMode) override;
	virtual void SendHealth                     (int a_Health, int a_FoodLevel, double a_FoodSaturationLevel) override;
	virtual void SendHideTitle                  (void) override;
	virtual void SendInventorySlot              (char a_WindowID, short a_SlotNum, const cItem & a_Item) override;
	virtual void SendKeepAlive                  (UInt32 a_PingID) override;
	virtual void SendLogin                      (const cPlayer & a_Player, const cWorld & a_World) override;
	virtual void SendLoginSuccess               (const AString & a_UUID, const AString & a_Username) override;
	virtual void SendMapData                    (const cMap & a_Map, int a_DataStartX, int a_DataStartY) override;
	virtual void SendPaintingSpawn              (const cPainting & a_Painting) override;
	virtual void SendParticleEffect             (const AString & a_ParticleName, float a_SrcX, float a_SrcY, float a_SrcZ, float a_OffsetX, float a_OffsetY, float a_OffsetZ, float a_ParticleData, int a_ParticleAmount) override;
	virtual void SendParticleEffect             (const AString & a_ParticleName, Vector3f a_Src, Vector3f a_Offset, float a_ParticleData, int a_ParticleAmount, std::array<int, 2> a_Data) override;
	virtual void SendPickupSpawn                (const cPickup & a_Pickup) override;
	virtual void SendPlayerAbilities            (const cPlayer & a_Player) override;
	virtual void SendPlayerListAddPlayer        (const cPlayer & a_Player) override;
	virtual void SendPlayerListRemovePlayer     (const cPlayer & a_Player) override;
	virtual void SendPlayerListUpdateGameMode   (const cPlayer & a_Player) override;
	virtual void SendPlayerListUpdatePing       (const cPlayer & a_Player) override;
	virtual void SendPlayerListUpdateDisplayName(const cPlayer & a_Player, const AString & a_CustomName) override;
	virtual void SendPlayerMaxSpeed             (const cPlayer & a_Player) override;
	virtual void SendPlayerMoveLook             (const cPlayer & a_Player) override;
	virtual void SendPlayerPosition             (const cPlayer & a_Player) override;
	virtual void SendPlayerSpawn                (const cPlayer & a_Player) override;
	virtual void SendPluginMessage              (const AString & a_Channel, const AString & a_Message) override;
	virtual void SendRemoveEntityEffect         (const cEntity & a_Entity, int a_EffectID) override;
	virtual void SendResetTitle                 (void) override;
	virtual void SendRespawn                    (eGameMode a_GameMode, eDimension a_Dimension, bool a_ShouldIgnoreDimensionChecks) override;
	virtual void SendScoreUpdate                (const AString & a_Objective, const AString & a_Player, cObjective::Score a_Score, Byte a_Mode) override;
	virtual void SendScoreboardObjective        (const AString & a_Name, const AString & a_DisplayName, Byte a_Mode) override;
	virtual void SendSetSubTitle                (const cCompositeChat & a_SubTitle) override;
	virtual void SendSetRawSubTitle             (const AString & a_SubTitle) override;
	virtual void SendSetTitle                   (const cCompositeChat & a_Title) override;
	virtual void SendSetRawTitle                (const AString & a_Title) override;
	virtual void SendSoundEffect                (const AString & a_SoundName, double a_X, double a_Y, double a_Z, float a_Volume, float a_Pitch) override;
	virtual void SendSoundParticleEffect        (const EffectID a_EffectID, int a_SrcX, int a_SrcY, int a_SrcZ, int a_Data) override;
	virtual void SendSpawnFallingBlock          (const cFallingBlock & a_FallingBlock) override;
	virtual void SendSpawnMob                   (const cMonster & a_Mob) override;
	virtual void SendSpawnObject                (const cEntity & a_Entity, char a_ObjectType, int a_ObjectData, Byte a_Yaw, Byte a_Pitch) override;
	virtual void SendSpawnVehicle               (const cEntity & a_Vehicle, char a_VehicleType, char a_VehicleSubType) override;
	virtual void SendStatistics                 (const cStatManager & a_Manager) override;
	virtual void SendTabCompletionResults       (const AStringVector & a_Results) override;
	virtual void SendTeleportEntity             (const cEntity & a_Entity) override;
	virtual void SendThunderbolt                (int a_BlockX, int a_BlockY, int a_BlockZ) override;
	virtual void SendTitleTimes                 (int a_FadeInTicks, int a_DisplayTicks, int a_FadeOutTicks) override;
	virtual void SendTimeUpdate                 (Int64 a_WorldAge, Int64 a_TimeOfDay, bool a_DoDaylightCycle) override;
	virtual void SendUnloadChunk                (int a_ChunkX, int a_ChunkZ) override;
	virtual void SendUpdateBlockEntity          (cBlockEntity & a_BlockEntity) override;
	virtual void SendUpdateSign                 (int a_BlockX, int a_BlockY, int a_BlockZ, const AString & a_Line1, const AString & a_Line2, const AString & a_Line3, const AString & a_Line4) override;
	virtual void SendUseBed                     (const cEntity & a_Entity, int a_BlockX, int a_BlockY, int a_BlockZ) override;
	virtual void SendWeather                    (eWeather a_Weather) override;
	virtual void SendWholeInventory             (const cPlayer & a_Player, const cWindow & a_Window) override;
	virtual void SendWindowClose                (const cWindow & a_Window) override;
	virtual void SendWindowOpen                 (const cWindow & a_Window) override;
	virtual void SendWindowProperty             (const cWindow & a_Window, short a_Property, short a_Value) override;

	virtual AString GetAuthServerID(void) override { return m_AuthServerID; }


protected:

	typedef std::vector<std::unique_ptr<cClientAction>> ActionList;

	AString m_ServerAddress;
	
	UInt16 m_ServerPort;
	
	AString m_AuthServerID;
	
	/** State of the protocol. 1 = status, 2 = login, 3 = game */
	UInt32 m_State;
	
	/** The dimension that was last sent to a player in a Respawn or Login packet.
	Used to avoid Respawning into the same dimension, which confuses the client. */
	eDimension m_LastSentDimension;

	/** Reads and handles the packet. The packet length and type have already been read.
	Returns true if the packet was understood, false if it was an unknown packet
	*/
	cProtocolError HandlePacket(cByteBuffer & a_ByteBuffer, UInt32 a_PacketType, ActionList & a_Action) WARN_UNUSED;

	// Packet handlers while in the Status state (m_State == 1):
	cProtocolError HandlePacketStatusPing(cByteBuffer & a_ByteBuffer, ActionList & a_Action) WARN_UNUSED;
	virtual cProtocolError HandlePacketStatusRequest(cByteBuffer & a_ByteBuffer, ActionList & a_Action) WARN_UNUSED;

	// Packet handlers while in the Login state (m_State == 2):
	cProtocolError HandlePacketLoginEncryptionResponse(cByteBuffer & a_ByteBuffer, ActionList & a_Action) WARN_UNUSED;
	cProtocolError HandlePacketLoginStart(cByteBuffer & a_ByteBuffer, ActionList & a_Action) WARN_UNUSED;

	// Packet handlers while in the Game state (m_State == 3):
	cProtocolError HandlePacketAnimation              (cByteBuffer & a_ByteBuffer, ActionList & a_Action) WARN_UNUSED;
	cProtocolError HandlePacketBlockDig               (cByteBuffer & a_ByteBuffer, ActionList & a_Action) WARN_UNUSED;
	cProtocolError HandlePacketBlockPlace             (cByteBuffer & a_ByteBuffer, ActionList & a_Action) WARN_UNUSED;
	cProtocolError HandlePacketChatMessage            (cByteBuffer & a_ByteBuffer, ActionList & a_Action) WARN_UNUSED;
	cProtocolError HandlePacketClientSettings         (cByteBuffer & a_ByteBuffer, ActionList & a_Action) WARN_UNUSED;
	cProtocolError HandlePacketClientStatus           (cByteBuffer & a_ByteBuffer, ActionList & a_Action) WARN_UNUSED;
	cProtocolError HandlePacketCreativeInventoryAction(cByteBuffer & a_ByteBuffer, ActionList & a_Action) WARN_UNUSED;
	cProtocolError HandlePacketEntityAction           (cByteBuffer & a_ByteBuffer, ActionList & a_Action) WARN_UNUSED;
	cProtocolError HandlePacketKeepAlive              (cByteBuffer & a_ByteBuffer, ActionList & a_Action) WARN_UNUSED;
	cProtocolError HandlePacketPlayer                 (cByteBuffer & a_ByteBuffer, ActionList & a_Action) WARN_UNUSED;
	cProtocolError HandlePacketPlayerAbilities        (cByteBuffer & a_ByteBuffer, ActionList & a_Action) WARN_UNUSED;
	cProtocolError HandlePacketPlayerLook             (cByteBuffer & a_ByteBuffer, ActionList & a_Action) WARN_UNUSED;
	cProtocolError HandlePacketPlayerPos              (cByteBuffer & a_ByteBuffer, ActionList & a_Action) WARN_UNUSED;
	cProtocolError HandlePacketPlayerPosLook          (cByteBuffer & a_ByteBuffer, ActionList & a_Action) WARN_UNUSED;
	cProtocolError HandlePacketPluginMessage          (cByteBuffer & a_ByteBuffer, ActionList & a_Action) WARN_UNUSED;
	cProtocolError HandlePacketSlotSelect             (cByteBuffer & a_ByteBuffer, ActionList & a_Action) WARN_UNUSED;
	cProtocolError HandlePacketSteerVehicle           (cByteBuffer & a_ByteBuffer, ActionList & a_Action) WARN_UNUSED;
	cProtocolError HandlePacketTabComplete            (cByteBuffer & a_ByteBuffer, ActionList & a_Action) WARN_UNUSED;
	cProtocolError HandlePacketUpdateSign             (cByteBuffer & a_ByteBuffer, ActionList & a_Action) WARN_UNUSED;
	cProtocolError HandlePacketUseEntity              (cByteBuffer & a_ByteBuffer, ActionList & a_Action) WARN_UNUSED;
	cProtocolError HandlePacketEnchantItem            (cByteBuffer & a_ByteBuffer, ActionList & a_Action) WARN_UNUSED;
	cProtocolError HandlePacketWindowClick            (cByteBuffer & a_ByteBuffer, ActionList & a_Action) WARN_UNUSED;
	cProtocolError HandlePacketWindowClose            (cByteBuffer & a_ByteBuffer, ActionList & a_Action) WARN_UNUSED;
	
	/** Parses Vanilla plugin messages into specific ClientHandle calls.
	The message payload is still in the bytebuffer, to be read by this function. */
	void HandleVanillaPluginMessage(cByteBuffer & a_ByteBuffer, const AString & a_Channel, UInt16 a_PayloadLength);
	
	/** Sends the data to the client, encrypting them if needed. */
	//virtual void SendData(cByteBuffer & a_Buffer, const char * a_Data, size_t a_Size) override;

	/** Sends the packet to the client. Called by the cPacketizer's destructor. */
	virtual void SendPacket(cPacketizer & a_Packet) override;

	void SendCompass(const cWorld & a_World);
	
	/** Reads an item out of the received data, sets a_Item to the values read. Returns false if not enough received data */
	virtual cProtocolError ReadItem(cByteBuffer & a_ByteBuffer, cItem & a_Item);
	
	/** Parses item metadata as read by ReadItem(), into the item enchantments. */
	void ParseItemMetadata(cItem & a_Item, const AString & a_Metadata);
	
	void StartEncryption(const Byte * a_Key);

	/** Converts the BlockFace received by the protocol into eBlockFace constants.
	If the received value doesn't match any of our eBlockFace constants, BLOCK_FACE_NONE is returned. */
	eBlockFace FaceIntToBlockFace(Int8 a_FaceInt);

	/** Writes the item data into a packet. */
	void WriteItem(cPacketizer & a_Pkt, const cItem & a_Item);

	/** Writes the metadata for the specified entity, not including the terminating 0x7f. */
	void WriteEntityMetadata(cPacketizer & a_Pkt, const cEntity & a_Entity);

	/** Writes the mob-specific metadata for the specified mob */
	void WriteMobMetadata(cPacketizer & a_Pkt, const cMonster & a_Mob);

	/** Writes the entity properties for the specified entity, including the Count field. */
	void WriteEntityProperties(cPacketizer & a_Pkt, const cEntity & a_Entity);

	/** Writes the block entity data for the specified block entity into the packet. */
	void WriteBlockEntity(cPacketizer & a_Pkt, const cBlockEntity & a_BlockEntity);

	virtual cProtocolError OnDataAddedToBuffer(cByteBuffer & a_Buffer, std::vector<std::unique_ptr<cClientAction>> & a_Action) override WARN_UNUSED;
} ;





/** The version 5 lengthed protocol, used by 1.7.6 through 1.7.9. */
class cProtocol176 :
	public cProtocol172
{
	typedef cProtocol172 super;
	
public:
	cProtocol176(const AString a_LogID);
	
	// cProtocol172 overrides:
	virtual void SendPlayerSpawn(const cPlayer & a_Player) override;
	virtual cProtocolError HandlePacketStatusRequest(cByteBuffer & a_ByteBuffer, ActionList & a_Action) override WARN_UNUSED;

} ;




