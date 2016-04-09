
// Protocol19x.h

/*
Declares the 1.9.x protocol classes:
	- cProtocol190
		- release 1.9.0 protocol (#107)
	- cProtocol191
		- release 1.9.1 protocol (#108)
	- cProtocol192
		- release 1.9.2 protocol (#109)
(others may be added later in the future for the 1.9 release series)
*/





#pragma once

#include "Protocol.h"
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

#include "PolarSSL++/AesCfb128Decryptor.h"
#include "PolarSSL++/AesCfb128Encryptor.h"





// fwd:
namespace Json
{
	class Value;
}





class cProtocol190 :
	public cProtocol
{
	typedef cProtocol super;

public:

	cProtocol190(cClientHandle * a_Client, const AString & a_ServerAddress, UInt16 a_ServerPort, UInt32 a_State);

	/** Called when client sends some data: */
	virtual void DataReceived(const char * a_Data, size_t a_Size) override;

	/** Sending stuff to clients (alphabetically sorted): */
	virtual void SendAttachEntity               (const cEntity & a_Entity, const cEntity & a_Vehicle) override;
	virtual void SendBlockAction                (int a_BlockX, int a_BlockY, int a_BlockZ, char a_Byte1, char a_Byte2, BLOCKTYPE a_BlockType) override;
	virtual void SendBlockBreakAnim	            (UInt32 a_EntityID, int a_BlockX, int a_BlockY, int a_BlockZ, char a_Stage) override;
	virtual void SendBlockChange                (int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta) override;
	virtual void SendBlockChanges               (int a_ChunkX, int a_ChunkZ, const sSetBlockVector & a_Changes) override;
	virtual void SendChat                       (const AString & a_Message, eChatType a_Type) override;
	virtual void SendChat                       (const cCompositeChat & a_Message, eChatType a_Type, bool a_ShouldUseChatPrefixes) override;
	virtual void SendChunkData                  (int a_ChunkX, int a_ChunkZ, cChunkDataSerializer & a_Serializer) override;
	virtual void SendCollectEntity              (const cEntity & a_Entity, const cPlayer & a_Player) override;
	virtual void SendDestroyEntity              (const cEntity & a_Entity) override;
	virtual void SendDetachEntity               (const cEntity & a_Entity, const cEntity & a_PreviousVehicle) override;
	virtual void SendDisconnect                 (const AString & a_Reason) override;
	virtual void SendEditSign                   (int a_BlockX, int a_BlockY, int a_BlockZ) override;  ///< Request the client to open up the sign editor for the sign (1.6+)
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
	virtual void SendExplosion                  (double a_BlockX, double a_BlockY, double a_BlockZ, float a_Radius, const cVector3iArray & a_BlocksAffected, const Vector3d & a_PlayerMotion) override;
	virtual void SendGameMode                   (eGameMode a_GameMode) override;
	virtual void SendHealth                     (void) override;
	virtual void SendHideTitle                  (void) override;
	virtual void SendInventorySlot              (char a_WindowID, short a_SlotNum, const cItem & a_Item) override;
	virtual void SendKeepAlive                  (UInt32 a_PingID) override;
	virtual void SendLogin                      (const cPlayer & a_Player, const cWorld & a_World) override;
	virtual void SendLoginSuccess               (void) override;
	virtual void SendMapData                    (const cMap & a_Map, int a_DataStartX, int a_DataStartY) override;
	virtual void SendPaintingSpawn              (const cPainting & a_Painting) override;
	virtual void SendPickupSpawn                (const cPickup & a_Pickup) override;
	virtual void SendPlayerAbilities            (void) override;
	virtual void SendEntityAnimation            (const cEntity & a_Entity, char a_Animation) override;
	virtual void SendParticleEffect             (const AString & a_ParticleName, float a_SrcX, float a_SrcY, float a_SrcZ, float a_OffsetX, float a_OffsetY, float a_OffsetZ, float a_ParticleData, int a_ParticleAmount) override;
	virtual void SendParticleEffect             (const AString & a_ParticleName, Vector3f a_Src, Vector3f a_Offset, float a_ParticleData, int a_ParticleAmount, std::array<int, 2> a_Data) override;
	virtual void SendPlayerListAddPlayer        (const cPlayer & a_Player) override;
	virtual void SendPlayerListRemovePlayer     (const cPlayer & a_Player) override;
	virtual void SendPlayerListUpdateGameMode   (const cPlayer & a_Player) override;
	virtual void SendPlayerListUpdatePing       (const cPlayer & a_Player) override;
	virtual void SendPlayerListUpdateDisplayName(const cPlayer & a_Player, const AString & a_CustomName) override;
	virtual void SendPlayerMaxSpeed             (void) override;
	virtual void SendPlayerMoveLook             (void) override;
	virtual void SendPlayerPosition             (void) override;
	virtual void SendPlayerSpawn                (const cPlayer & a_Player) override;
	virtual void SendPluginMessage              (const AString & a_Channel, const AString & a_Message) override;
	virtual void SendRemoveEntityEffect         (const cEntity & a_Entity, int a_EffectID) override;
	virtual void SendResetTitle                 (void) override;
	virtual void SendRespawn                    (eDimension a_Dimension, bool a_ShouldIgnoreDimensionChecks) override;
	virtual void SendSoundEffect                (const AString & a_SoundName, double a_X, double a_Y, double a_Z, float a_Volume, float a_Pitch) override;
	virtual void SendExperience                 (void) override;
	virtual void SendExperienceOrb              (const cExpOrb & a_ExpOrb) override;
	virtual void SendScoreboardObjective        (const AString & a_Name, const AString & a_DisplayName, Byte a_Mode) override;
	virtual void SendScoreUpdate                (const AString & a_Objective, const AString & a_Player, cObjective::Score a_Score, Byte a_Mode) override;
	virtual void SendDisplayObjective           (const AString & a_Objective, cScoreboard::eDisplaySlot a_Display) override;
	virtual void SendSetSubTitle                (const cCompositeChat & a_SubTitle) override;
	virtual void SendSetRawSubTitle             (const AString & a_SubTitle) override;
	virtual void SendSetTitle                   (const cCompositeChat & a_Title) override;
	virtual void SendSetRawTitle                (const AString & a_Title) override;
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
	virtual void SendWholeInventory             (const cWindow & a_Window) override;
	virtual void SendWindowClose                (const cWindow & a_Window) override;
	virtual void SendWindowOpen                 (const cWindow & a_Window) override;
	virtual void SendWindowProperty             (const cWindow & a_Window, short a_Property, short a_Value) override;

	virtual AString GetAuthServerID(void) override { return m_AuthServerID; }

	/** Compress the packet. a_Packet must be without packet length.
	a_Compressed will be set to the compressed packet includes packet length and data length.
	If compression fails, the function returns false. */
	static bool CompressPacket(const AString & a_Packet, AString & a_Compressed);

	/** The 1.8 protocol use a particle id instead of a string. This function converts the name to the id. If the name is incorrect, it returns 0. */
	static int GetParticleID(const AString & a_ParticleName);

	/** Minecraft 1.8 use other locations to spawn the item frame. This function converts the 1.7 positions to 1.8 positions. */
	static void FixItemFramePositions(int a_ObjectData, double & a_PosX, double & a_PosZ, double & a_Yaw);

protected:

	AString m_ServerAddress;

	UInt16 m_ServerPort;

	AString m_AuthServerID;

	/** State of the protocol. 1 = status, 2 = login, 3 = game */
	UInt32 m_State;

	/** Buffer for the received data */
	cByteBuffer m_ReceivedData;

	bool m_IsEncrypted;

	cAesCfb128Decryptor m_Decryptor;
	cAesCfb128Encryptor m_Encryptor;

	/** The logfile where the comm is logged, when g_ShouldLogComm is true */
	cFile m_CommLogFile;

	/** The dimension that was last sent to a player in a Respawn or Login packet.
	Used to avoid Respawning into the same dimension, which confuses the client. */
	eDimension m_LastSentDimension;


	/** Adds the received (unencrypted) data to m_ReceivedData, parses complete packets */
	void AddReceivedData(const char * a_Data, size_t a_Size);

	/** Reads and handles the packet. The packet length and type have already been read.
	Returns true if the packet was understood, false if it was an unknown packet
	*/
	bool HandlePacket(cByteBuffer & a_ByteBuffer, UInt32 a_PacketType);

	// Packet handlers while in the Status state (m_State == 1):
	virtual void HandlePacketStatusPing(cByteBuffer & a_ByteBuffer);
	virtual void HandlePacketStatusRequest(cByteBuffer & a_ByteBuffer);

	// Packet handlers while in the Login state (m_State == 2):
	void HandlePacketLoginEncryptionResponse(cByteBuffer & a_ByteBuffer);
	void HandlePacketLoginStart(cByteBuffer & a_ByteBuffer);

	// Packet handlers while in the Game state (m_State == 3):
	void HandlePacketAnimation              (cByteBuffer & a_ByteBuffer);
	void HandlePacketBlockDig               (cByteBuffer & a_ByteBuffer);
	void HandlePacketBlockPlace             (cByteBuffer & a_ByteBuffer);
	void HandlePacketChatMessage            (cByteBuffer & a_ByteBuffer);
	void HandlePacketClientSettings         (cByteBuffer & a_ByteBuffer);
	void HandlePacketClientStatus           (cByteBuffer & a_ByteBuffer);
	void HandleConfirmTeleport              (cByteBuffer & a_ByteBuffer);
	void HandlePacketCreativeInventoryAction(cByteBuffer & a_ByteBuffer);
	void HandlePacketEntityAction           (cByteBuffer & a_ByteBuffer);
	void HandlePacketKeepAlive              (cByteBuffer & a_ByteBuffer);
	void HandlePacketPlayer                 (cByteBuffer & a_ByteBuffer);
	void HandlePacketPlayerAbilities        (cByteBuffer & a_ByteBuffer);
	void HandlePacketPlayerLook             (cByteBuffer & a_ByteBuffer);
	void HandlePacketPlayerPos              (cByteBuffer & a_ByteBuffer);
	void HandlePacketPlayerPosLook          (cByteBuffer & a_ByteBuffer);
	void HandlePacketPluginMessage          (cByteBuffer & a_ByteBuffer);
	void HandlePacketSlotSelect             (cByteBuffer & a_ByteBuffer);
	void HandlePacketSteerVehicle           (cByteBuffer & a_ByteBuffer);
	void HandlePacketTabComplete            (cByteBuffer & a_ByteBuffer);
	void HandlePacketUpdateSign             (cByteBuffer & a_ByteBuffer);
	void HandlePacketUseEntity              (cByteBuffer & a_ByteBuffer);
	void HandlePacketUseItem                (cByteBuffer & a_ByteBuffer);
	void HandlePacketEnchantItem            (cByteBuffer & a_ByteBuffer);
	void HandlePacketWindowClick            (cByteBuffer & a_ByteBuffer);
	void HandlePacketWindowClose            (cByteBuffer & a_ByteBuffer);

	/** Parses Vanilla plugin messages into specific ClientHandle calls.
	The message payload is still in the bytebuffer, the handler reads it specifically for each handled channel */
	void HandleVanillaPluginMessage(cByteBuffer & a_ByteBuffer, const AString & a_Channel);


	/** Sends the data to the client, encrypting them if needed. */
	virtual void SendData(const char * a_Data, size_t a_Size) override;

	/** Sends the packet to the client. Called by the cPacketizer's destructor. */
	virtual void SendPacket(cPacketizer & a_Packet) override;

	void SendCompass(const cWorld & a_World);

	/** Reads an item out of the received data, sets a_Item to the values read.
	Returns false if not enough received data.
	a_KeepRemainingBytes tells the function to keep that many bytes at the end of the buffer. */
	virtual bool ReadItem(cByteBuffer & a_ByteBuffer, cItem & a_Item, size_t a_KeepRemainingBytes = 0);

	/** Parses item metadata as read by ReadItem(), into the item enchantments. */
	void ParseItemMetadata(cItem & a_Item, const AString & a_Metadata);

	void StartEncryption(const Byte * a_Key);

	/** Converts the BlockFace received by the protocol into eBlockFace constants.
	If the received value doesn't match any of our eBlockFace constants, BLOCK_FACE_NONE is returned. */
	eBlockFace FaceIntToBlockFace(UInt32 a_FaceInt);

	/** Writes the item data into a packet. */
	void WriteItem(cPacketizer & a_Pkt, const cItem & a_Item);

	/** Writes the metadata for the specified entity, not including the terminating 0xff. */
	void WriteEntityMetadata(cPacketizer & a_Pkt, const cEntity & a_Entity);

	/** Writes the mob-specific metadata for the specified mob */
	void WriteMobMetadata(cPacketizer & a_Pkt, const cMonster & a_Mob);

	/** Writes the entity properties for the specified entity, including the Count field. */
	void WriteEntityProperties(cPacketizer & a_Pkt, const cEntity & a_Entity);

	/** Writes the block entity data for the specified block entity into the packet. */
	void WriteBlockEntity(cPacketizer & a_Pkt, const cBlockEntity & a_BlockEntity);

	/** Types used within metadata */
	enum eMetadataType
	{
		METADATA_TYPE_BYTE              = 0,
		METADATA_TYPE_VARINT            = 1,
		METADATA_TYPE_FLOAT             = 2,
		METADATA_TYPE_STRING            = 3,
		METADATA_TYPE_CHAT              = 4,
		METADATA_TYPE_ITEM              = 5,
		METADATA_TYPE_BOOL              = 6,
		METADATA_TYPE_ROTATION          = 7,
		METADATA_TYPE_POSITION          = 8,
		METADATA_TYPE_OPTIONAL_POSITION = 9,
		METADATA_TYPE_DIRECTION         = 10,
		METADATA_TYPE_OPTIONAL_UUID     = 11,
		METADATA_TYPE_BLOCKID           = 12
	} ;
} ;





/** The version 108 protocol, used by 1.9.1.  Uses an int rather than a byte for dimension in join game. */
class cProtocol191 :
	public cProtocol190
{
	typedef cProtocol190 super;

public:
	cProtocol191(cClientHandle * a_Client, const AString & a_ServerAddress, UInt16 a_ServerPort, UInt32 a_State);

	// cProtocol190 overrides:
	virtual void SendLogin(const cPlayer & a_Player, const cWorld & a_World) override;
	virtual void HandlePacketStatusRequest(cByteBuffer & a_ByteBuffer) override;

} ;





/** The version 109 protocol, used by 1.9.2.  Same as 1.9.1, except the server list ping version number changed with the protocol number. */
class cProtocol192 :
	public cProtocol191
{
	typedef cProtocol191 super;

public:
	cProtocol192(cClientHandle * a_Client, const AString & a_ServerAddress, UInt16 a_ServerPort, UInt32 a_State);

	// cProtocol190 overrides:
	virtual void HandlePacketStatusRequest(cByteBuffer & a_ByteBuffer) override;

} ;




