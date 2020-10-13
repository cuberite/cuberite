
// Protocol_1_8.h

/*
Declares the 1.8 protocol classes:
	- cProtocol_1_8_0
		- release 1.8 protocol (#47), also used by 1.8.1 to 1.8.9
*/





#pragma once

#include "Protocol.h"
#include "../ByteBuffer.h"
#include "../Registries/Statistics.h"

#include "../mbedTLS++/AesCfb128Decryptor.h"
#include "../mbedTLS++/AesCfb128Encryptor.h"





class cProtocol_1_8_0:
	public cProtocol
{
	using Super = cProtocol;

public:

	cProtocol_1_8_0(cClientHandle * a_Client, const AString & a_ServerAddress, State a_State);

	/** Called when client sends some data: */
	virtual void DataReceived(cByteBuffer & a_Buffer, const char * a_Data, size_t a_Size) override;

	/** Sending stuff to clients (alphabetically sorted): */
	virtual void SendAttachEntity               (const cEntity & a_Entity, const cEntity & a_Vehicle) override;
	virtual void SendBlockAction                (int a_BlockX, int a_BlockY, int a_BlockZ, char a_Byte1, char a_Byte2, BLOCKTYPE a_BlockType) override;
	virtual void SendBlockBreakAnim	            (UInt32 a_EntityID, int a_BlockX, int a_BlockY, int a_BlockZ, char a_Stage) override;
	virtual void SendBlockChange                (int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta) override;
	virtual void SendBlockChanges               (int a_ChunkX, int a_ChunkZ, const sSetBlockVector & a_Changes) override;
	virtual void SendCameraSetTo                (const cEntity & a_Entity) override;
	virtual void SendChat                       (const AString & a_Message, eChatType a_Type) override;
	virtual void SendChat                       (const cCompositeChat & a_Message, eChatType a_Type, bool a_ShouldUseChatPrefixes) override;
	virtual void SendChatRaw                    (const AString & a_MessageRaw, eChatType a_Type) override;
	virtual void SendChunkData                  (const std::string_view a_ChunkData) override;
	virtual void SendCollectEntity              (const cEntity & a_Collected, const cEntity & a_Collector, unsigned a_Count) override;
	virtual void SendDestroyEntity              (const cEntity & a_Entity) override;
	virtual void SendDetachEntity               (const cEntity & a_Entity, const cEntity & a_PreviousVehicle) override;
	virtual void SendDisconnect                 (const AString & a_Reason) override;
	virtual void SendEditSign                   (int a_BlockX, int a_BlockY, int a_BlockZ) override;  ///< Request the client to open up the sign editor for the sign (1.6+)
	virtual void SendEntityAnimation            (const cEntity & a_Entity, char a_Animation) override;
	virtual void SendEntityEffect               (const cEntity & a_Entity, int a_EffectID, int a_Amplifier, int a_Duration) override;
	virtual void SendEntityEquipment            (const cEntity & a_Entity, short a_SlotNum, const cItem & a_Item) override;
	virtual void SendEntityHeadLook             (const cEntity & a_Entity) override;
	virtual void SendEntityLook                 (const cEntity & a_Entity) override;
	virtual void SendEntityMetadata             (const cEntity & a_Entity) override;
	virtual void SendEntityPosition             (const cEntity & a_Entity) override;
	virtual void SendEntityProperties           (const cEntity & a_Entity) override;
	virtual void SendEntityStatus               (const cEntity & a_Entity, char a_Status) override;
	virtual void SendEntityVelocity             (const cEntity & a_Entity) override;
	virtual void SendExperience                 (void) override;
	virtual void SendExperienceOrb              (const cExpOrb & a_ExpOrb) override;
	virtual void SendExplosion                  (Vector3f a_Position, float a_Power) override;
	virtual void SendGameMode                   (eGameMode a_GameMode) override;
	virtual void SendHealth                     (void) override;
	virtual void SendHeldItemChange             (int a_ItemIndex) override;
	virtual void SendHideTitle                  (void) override;
	virtual void SendInventorySlot              (char a_WindowID, short a_SlotNum, const cItem & a_Item) override;
	virtual void SendKeepAlive                  (UInt32 a_PingID) override;
	virtual void SendLeashEntity                (const cEntity & a_Entity, const cEntity & a_EntityLeashedTo) override;
	virtual void SendLogin                      (const cPlayer & a_Player, const cWorld & a_World) override;
	virtual void SendLoginSuccess               (void) override;
	virtual void SendMapData                    (const cMap & a_Map, int a_DataStartX, int a_DataStartY) override;
	virtual void SendPaintingSpawn              (const cPainting & a_Painting) override;
	virtual void SendPlayerAbilities            (void) override;
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
	virtual void SendResourcePack               (const AString & a_ResourcePackUrl) override;
	virtual void SendRespawn                    (eDimension a_Dimension) override;
	virtual void SendSoundEffect                (const AString & a_SoundName, double a_X, double a_Y, double a_Z, float a_Volume, float a_Pitch) override;
	virtual void SendScoreboardObjective        (const AString & a_Name, const AString & a_DisplayName, Byte a_Mode) override;
	virtual void SendScoreUpdate                (const AString & a_Objective, const AString & a_Player, cObjective::Score a_Score, Byte a_Mode) override;
	virtual void SendDisplayObjective           (const AString & a_Objective, cScoreboard::eDisplaySlot a_Display) override;
	virtual void SendSetSubTitle                (const cCompositeChat & a_SubTitle) override;
	virtual void SendSetRawSubTitle             (const AString & a_SubTitle) override;
	virtual void SendSetTitle                   (const cCompositeChat & a_Title) override;
	virtual void SendSetRawTitle                (const AString & a_Title) override;
	virtual void SendSoundParticleEffect        (const EffectID a_EffectID, int a_SrcX, int a_SrcY, int a_SrcZ, int a_Data) override;
	virtual void SendSpawnEntity                (const cEntity & a_Entity) override;
	virtual void SendSpawnMob                   (const cMonster & a_Mob) override;
	virtual void SendStatistics                 (const cStatManager & a_Manager) override;
	virtual void SendTabCompletionResults       (const AStringVector & a_Results) override;
	virtual void SendThunderbolt                (int a_BlockX, int a_BlockY, int a_BlockZ) override;
	virtual void SendTitleTimes                 (int a_FadeInTicks, int a_DisplayTicks, int a_FadeOutTicks) override;
	virtual void SendTimeUpdate                 (Int64 a_WorldAge, Int64 a_TimeOfDay, bool a_DoDaylightCycle) override;
	virtual void SendUnleashEntity              (const cEntity & a_Entity) override;
	virtual void SendUnloadChunk                (int a_ChunkX, int a_ChunkZ) override;
	virtual void SendUpdateBlockEntity          (cBlockEntity & a_BlockEntity) override;
	virtual void SendUpdateSign                 (int a_BlockX, int a_BlockY, int a_BlockZ, const AString & a_Line1, const AString & a_Line2, const AString & a_Line3, const AString & a_Line4) override;
	virtual void SendUseBed                     (const cEntity & a_Entity, int a_BlockX, int a_BlockY, int a_BlockZ) override;
	virtual void SendUnlockRecipe               (UInt32 a_RecipeID) override;
	virtual void SendInitRecipes                (UInt32 a_RecipeID) override;
	virtual void SendWeather                    (eWeather a_Weather) override;
	virtual void SendWholeInventory             (const cWindow & a_Window) override;
	virtual void SendWindowClose                (const cWindow & a_Window) override;
	virtual void SendWindowOpen                 (const cWindow & a_Window) override;
	virtual void SendWindowProperty             (const cWindow & a_Window, size_t a_Property, short a_Value) override;

	virtual AString GetAuthServerID(void) override { return m_AuthServerID; }

	/** Compress the packet. a_Packet must be without packet length.
	a_Compressed will be set to the compressed packet includes packet length and data length.
	If compression fails, the function returns false. */
	static bool CompressPacket(const AString & a_Packet, AString & a_Compressed);

	/** The 1.8 protocol use a particle id instead of a string. This function converts the name to the id. If the name is incorrect, it returns 0. */
	static int GetParticleID(const AString & a_ParticleName);

protected:

	/** State of the protocol. */
	State m_State;

	/** Adds the received (unencrypted) data to m_ReceivedData, parses complete packets */
	virtual void AddReceivedData(cByteBuffer & a_Buffer, const char * a_Data, size_t a_Size);

	/** Nobody inherits 1.8, so it doesn't use this method */
	virtual UInt32 GetPacketID(ePacketType a_Packet) override;

	/** Returns 1.8. */
	virtual Version GetProtocolVersion() override;

	/** Converts eMonsterType to protocol-specific mob types */
	virtual UInt32 GetProtocolMobType(eMonsterType a_MobType);

	/** Reads and handles the packet. The packet length and type have already been read.
	Returns true if the packet was understood, false if it was an unknown packet
	*/
	virtual bool HandlePacket(cByteBuffer & a_ByteBuffer, UInt32 a_PacketType);

	// Packet handlers while in the Status state (m_State == 1):
	virtual void HandlePacketStatusPing(cByteBuffer & a_ByteBuffer);
	virtual void HandlePacketStatusRequest(cByteBuffer & a_ByteBuffer);

	// Packet handlers while in the Login state (m_State == 2):
	virtual void HandlePacketLoginEncryptionResponse(cByteBuffer & a_ByteBuffer);
	virtual void HandlePacketLoginStart(cByteBuffer & a_ByteBuffer);

	// Packet handlers while in the Game state (m_State == 3):
	virtual void HandlePacketAnimation              (cByteBuffer & a_ByteBuffer);
	virtual void HandlePacketBlockDig               (cByteBuffer & a_ByteBuffer);
	virtual void HandlePacketBlockPlace             (cByteBuffer & a_ByteBuffer);
	virtual void HandlePacketChatMessage            (cByteBuffer & a_ByteBuffer);
	virtual void HandlePacketClientSettings         (cByteBuffer & a_ByteBuffer);
	virtual void HandlePacketClientStatus           (cByteBuffer & a_ByteBuffer);
	virtual void HandlePacketCreativeInventoryAction(cByteBuffer & a_ByteBuffer);
	virtual void HandlePacketEntityAction           (cByteBuffer & a_ByteBuffer);
	virtual void HandlePacketKeepAlive              (cByteBuffer & a_ByteBuffer);
	virtual void HandlePacketPlayer                 (cByteBuffer & a_ByteBuffer);
	virtual void HandlePacketPlayerAbilities        (cByteBuffer & a_ByteBuffer);
	virtual void HandlePacketPlayerLook             (cByteBuffer & a_ByteBuffer);
	virtual void HandlePacketPlayerPos              (cByteBuffer & a_ByteBuffer);
	virtual void HandlePacketPlayerPosLook          (cByteBuffer & a_ByteBuffer);
	virtual void HandlePacketPluginMessage          (cByteBuffer & a_ByteBuffer);
	virtual void HandlePacketResourcePackStatus     (cByteBuffer & a_ByteBuffer);
	virtual void HandlePacketSlotSelect             (cByteBuffer & a_ByteBuffer);
	virtual void HandlePacketSpectate               (cByteBuffer & a_ByteBuffer);
	virtual void HandlePacketSteerVehicle           (cByteBuffer & a_ByteBuffer);
	virtual void HandlePacketTabComplete            (cByteBuffer & a_ByteBuffer);
	virtual void HandlePacketUpdateSign             (cByteBuffer & a_ByteBuffer);
	virtual void HandlePacketUseEntity              (cByteBuffer & a_ByteBuffer);
	virtual void HandlePacketEnchantItem            (cByteBuffer & a_ByteBuffer);
	virtual void HandlePacketWindowClick            (cByteBuffer & a_ByteBuffer);
	virtual void HandlePacketWindowClose            (cByteBuffer & a_ByteBuffer);

	/** Parses Vanilla plugin messages into specific ClientHandle calls.
	The message payload is still in the bytebuffer, the handler reads it specifically for each handled channel */
	virtual void HandleVanillaPluginMessage(cByteBuffer & a_ByteBuffer, const AString & a_Channel);

	/** Sends the data to the client, encrypting them if needed. */
	virtual void SendData(const char * a_Data, size_t a_Size) override;

	/** Sends the packet to the client. Called by the cPacketizer's destructor. */
	virtual void SendPacket(cPacketizer & a_Packet) override;

	/** Reads an item out of the received data, sets a_Item to the values read.
	Returns false if not enough received data.
	a_KeepRemainingBytes tells the function to keep that many bytes at the end of the buffer. */
	virtual bool ReadItem(cByteBuffer & a_ByteBuffer, cItem & a_Item, size_t a_KeepRemainingBytes = 0);

	/** Parses item metadata as read by ReadItem(), into the item enchantments. */
	virtual void ParseItemMetadata(cItem & a_Item, const AString & a_Metadata);

	virtual void StartEncryption(const Byte * a_Key);

	/** Converts the BlockFace received by the protocol into eBlockFace constants.
	If the received value doesn't match any of our eBlockFace constants, BLOCK_FACE_NONE is returned. */
	static eBlockFace FaceIntToBlockFace(Int32 a_FaceInt);

	/** Sends the entity type and entity-dependent data required for the entity to initially spawn. */
	virtual void SendEntitySpawn(const cEntity & a_Entity, const UInt8 a_ObjectType, const Int32 a_ObjectData);

	/** Writes the item data into a packet. */
	virtual void WriteItem(cPacketizer & a_Pkt, const cItem & a_Item);

	/** Writes the metadata for the specified entity, not including the terminating 0x7f. */
	virtual void WriteEntityMetadata(cPacketizer & a_Pkt, const cEntity & a_Entity);

	/** Writes the mob-specific metadata for the specified mob */
	virtual void WriteMobMetadata(cPacketizer & a_Pkt, const cMonster & a_Mob);

	/** Writes the entity properties for the specified entity, including the Count field. */
	virtual void WriteEntityProperties(cPacketizer & a_Pkt, const cEntity & a_Entity);

	/** Writes the block entity data for the specified block entity into the packet. */
	virtual void WriteBlockEntity(cPacketizer & a_Pkt, const cBlockEntity & a_BlockEntity);

private:

	AString m_ServerAddress;

	AString m_AuthServerID;

	bool m_IsEncrypted;

	cAesCfb128Decryptor m_Decryptor;
	cAesCfb128Encryptor m_Encryptor;

	/** The logfile where the comm is logged, when g_ShouldLogComm is true */
	cFile m_CommLogFile;

	/** Sends an entity teleport packet.
	Mitigates a 1.8 bug where the position in the entity spawn packet is ignored,
	and so entities don't show up until a teleport is sent. */
	void SendEntityTeleport(const cEntity & a_Entity);

	/** Converts an entity to a protocol-specific entity type.
	Only entities that the Send Spawn Entity packet supports are valid inputs to this method */
	static UInt8 GetProtocolEntityType(const cEntity & a_Entity);

	/** Converts a statistic to a protocol-specific string.
	Protocols <= 1.12 use strings, hence this is a static as the string-mapping was append-only for the versions that used it.
	Returns an empty string, handled correctly by the client, for newer, unsupported statistics. */
	static const char * GetProtocolStatisticName(Statistic a_Statistic);
} ;
