
// Protocol_1_8.h

/*
Declares the 1.8 protocol classes:
	- cProtocol_1_8_0
		- release 1.8 protocol (#47), also used by 1.8.1 to 1.8.9
*/





#pragma once

#include "Protocol.h"
#include "../ByteBuffer.h"
#include "../Registries/CustomStatistics.h"

#include "../mbedTLS++/AesCfb128Decryptor.h"
#include "../mbedTLS++/AesCfb128Encryptor.h"

#include "CircularBufferCompressor.h"
#include "StringCompression.h"





class cProtocol_1_8_0:
	public cProtocol
{
	using Super = cProtocol;

public:

	cProtocol_1_8_0(cClientHandle * a_Client, const AString & a_ServerAddress, State a_State);

	virtual void DataReceived(cByteBuffer & a_Buffer, ContiguousByteBuffer & a_Data) override;
	virtual void DataPrepared(ContiguousByteBuffer & a_Data) override;

	// Sending stuff to clients (alphabetically sorted):
	virtual void SendAcknowledgeBlockChange     (int a_SequenceId) override;
	virtual void SendAttachEntity               (const cEntity & a_Entity, const cEntity & a_Vehicle) override;
	virtual void SendBlockAction                (Vector3i a_BlockPos, char a_Byte1, char a_Byte2, BlockState a_Block) override;
	virtual void SendBlockBreakAnim	            (UInt32 a_EntityID, Vector3i a_BlockPos, char a_Stage) override;
	virtual void SendRenderDistanceCenter       (cChunkCoords a_chunk) override;
	virtual void SendBlockChange                (Vector3i a_BlockPos, BlockState a_Block) override;
	virtual void SendBlockChanges               (int a_ChunkX, int a_ChunkZ, const sSetBlockVector & a_Changes) override;
	virtual void SendBossBarAdd                 (UInt32 a_UniqueID, const cCompositeChat & a_Title, float a_FractionFilled, BossBarColor a_Color, BossBarDivisionType a_DivisionType, bool a_DarkenSky, bool a_PlayEndMusic, bool a_CreateFog) override;
	virtual void SendBossBarRemove              (UInt32 a_UniqueID) override;
	virtual void SendBossBarUpdateFlags         (UInt32 a_UniqueID, bool a_DarkenSky, bool a_PlayEndMusic, bool a_CreateFog) override;
	virtual void SendBossBarUpdateHealth        (UInt32 a_UniqueID, float a_FractionFilled) override;
	virtual void SendBossBarUpdateStyle         (UInt32 a_UniqueID, BossBarColor a_Color, BossBarDivisionType a_DivisionType) override;
	virtual void SendBossBarUpdateTitle         (UInt32 a_UniqueID, const cCompositeChat & a_Title) override;
	virtual void SendCameraSetTo                (const cEntity & a_Entity) override;
	virtual void SendChat                       (const AString & a_Message, eChatType a_Type) override;
	virtual void SendChat                       (const cCompositeChat & a_Message, eChatType a_Type, bool a_ShouldUseChatPrefixes) override;
	virtual void SendChatRaw                    (const AString & a_MessageRaw, eChatType a_Type) override;
	virtual void SendChunkData                  (ContiguousByteBufferView a_ChunkData) override;
	virtual void SendCollectEntity              (const cEntity & a_Collected, const cEntity & a_Collector, unsigned a_Count) override;
	virtual void SendCommandTree                (void) override;
	virtual void SendDestroyEntity              (const cEntity & a_Entity) override;
	virtual void SendDetachEntity               (const cEntity & a_Entity, const cEntity & a_PreviousVehicle) override;
	virtual void SendDisconnect                 (const AString & a_Reason) override;
	virtual void SendDynamicRegistries          (void) override;
	virtual void SendEditSign                   (Vector3i a_BlockPos) override;  ///< Request the client to open up the sign editor for the sign (1.6+)
	virtual void SendEntityAnimation            (const cEntity & a_Entity, EntityAnimation a_Animation) override;
	virtual void SendEntityEffect               (const cEntity & a_Entity, int a_EffectID, int a_Amplifier, int a_Duration) override;
	virtual void SendEntityEquipment            (const cEntity & a_Entity, short a_SlotNum, const cItem & a_Item) override;
	virtual void SendEntityHeadLook             (const cEntity & a_Entity) override;
	virtual void SendEntityLook                 (const cEntity & a_Entity) override;
	virtual void SendEntityMetadata             (const cEntity & a_Entity) override;
	virtual void SendEntityPosition             (const cEntity & a_Entity) override;
	virtual void SendEntityProperties           (const cEntity & a_Entity) override;
	virtual void SendEntityVelocity             (const cEntity & a_Entity) override;
	virtual void SendExperience                 (void) override;
	virtual void SendExperienceOrb              (const cExpOrb & a_ExpOrb) override;
	virtual void SendExplosion                  (Vector3f a_Position, float a_Power) override;
	virtual void SendFinishConfiguration        (void) override;
	virtual void SendGameMode                   (eGameMode a_GameMode) override;
	virtual void SendHealth                     (void) override;
	virtual void SendHeldItemChange             (int a_ItemIndex) override;
	virtual void SendHideTitle                  (void) override;
	virtual void SendInventorySlot              (char a_WindowID, short a_SlotNum, const cItem & a_Item) override;
	virtual void SendKeepAlive                  (UInt32 a_PingID) override;
	virtual void SendSelectKnownPacks           (void) override;
	virtual void SendInitialChunksComing        () override;
	virtual void SendLeashEntity                (const cEntity & a_Entity, const cEntity & a_EntityLeashedTo) override;
	virtual void SendLogin                      (const cPlayer & a_Player, const cWorld & a_World) override;
	virtual void SendLoginSuccess               (void) override;
	virtual void SendMapData                    (const cMap & a_Map, int a_DataStartX, int a_DataStartY) override;
	virtual void SendPaintingSpawn              (const cPainting & a_Painting) override;
	virtual void SendPlayerAbilities            (void) override;
	virtual void SendParticleEffect             (const AString & a_ParticleName, Vector3f a_Src, Vector3f a_Offset, float a_ParticleData, int a_ParticleAmount) override;
	virtual void SendParticleEffect             (const AString & a_ParticleName, Vector3f a_Src, Vector3f a_Offset, float a_ParticleData, int a_ParticleAmount, std::array<int, 2> a_Data) override;
	virtual void SendPlayerActionResponse       (Vector3i a_blockpos, int a_state_id, cProtocol::PlayerActionResponses a_action, bool a_IsApproved) override;
	virtual void SendPlayerListAddPlayer        (const cPlayer & a_Player) override;
	virtual void SendPlayerListInitChat         (const cPlayer & a_Player) override;
	virtual void SendPlayerListHeaderFooter     (const cCompositeChat & a_Header, const cCompositeChat & a_Footer) override;
	virtual void SendPlayerListRemovePlayer     (const cPlayer & a_Player) override;
	virtual void SendPlayerListUpdateDisplayName(const cPlayer & a_Player, const AString & a_CustomName) override;
	virtual void SendPlayerListUpdateGameMode   (const cPlayer & a_Player) override;
	virtual void SendPlayerListUpdatePing       () override;
	virtual void SendPlayerMoveLook             (Vector3d a_Pos, float a_Yaw, float a_Pitch, bool a_IsRelative) override;
	virtual void SendPlayerMoveLook             (void) override;
	virtual void SendPlayerPermissionLevel      (void) override;
	virtual void SendPlayerPosition             (void) override;
	virtual void SendPlayerSpawn                (const cPlayer & a_Player) override;
	virtual void SendPluginMessage              (const AString & a_Channel, ContiguousByteBufferView a_Message) override;
	virtual void SendRemoveEntityEffect         (const cEntity & a_Entity, int a_EffectID) override;
	virtual void SendResetTitle                 (void) override;
	virtual void SendResourcePack               (const AString & a_ResourcePackUrl) override;
	virtual void SendRespawn                    (eDimension a_Dimension) override;
	virtual void SendSoundEffect                (const AString & a_SoundName, Vector3d a_Origin, float a_Volume, float a_Pitch) override;
	virtual void SendScoreboardObjective        (const AString & a_Name, const AString & a_DisplayName, Byte a_Mode) override;
	virtual void SendScoreUpdate                (const AString & a_Objective, const AString & a_Player, cObjective::Score a_Score, Byte a_Mode) override;
	virtual void SendDisplayObjective           (const AString & a_Objective, cScoreboard::eDisplaySlot a_Display) override;
	virtual void SendSetSubTitle                (const cCompositeChat & a_SubTitle) override;
	virtual void SendSetRawSubTitle             (const AString & a_SubTitle) override;
	virtual void SendSetTitle                   (const cCompositeChat & a_Title) override;
	virtual void SendSetRawTitle                (const AString & a_Title) override;
	virtual void SendSoundParticleEffect        (const EffectID a_EffectID, Vector3i a_Origin, int a_Data) override;
	virtual void SendSpawnEntity                (const cEntity & a_Entity) override;
	virtual void SendSpawnMob                   (const cMonster & a_Mob) override;
	virtual void SendStatistics                 (const StatisticsManager & a_Manager) override;
	virtual void SendTabCompletionResults       (const AStringVector & a_Results, UInt32 CompletionId) override;
	virtual void SendTags                       (void) override;
	virtual void SendThunderbolt                (Vector3i a_BlockPos) override;
	virtual void SendTitleTimes                 (int a_FadeInTicks, int a_DisplayTicks, int a_FadeOutTicks) override;
	virtual void SendTimeUpdate                 (cTickTimeLong a_WorldAge, cTickTimeLong a_WorldDate, bool a_DoDaylightCycle) override;
	virtual void SendUnleashEntity              (const cEntity & a_Entity) override;
	virtual void SendUnloadChunk                (int a_ChunkX, int a_ChunkZ) override;
	virtual void SendUpdateBlockEntity          (cBlockEntity & a_BlockEntity) override;
	virtual void SendUpdateSign                 (Vector3i a_BlockPos, const AString & a_Line1, const AString & a_Line2, const AString & a_Line3, const AString & a_Line4) override;
	virtual void SendUnlockRecipe               (UInt32 a_RecipeID) override;
	virtual void SendInitRecipes                (UInt32 a_RecipeID) override;
	virtual void SendWeather                    (eWeather a_Weather) override;
	virtual void SendGameStateChange            (eGameStateReason a_Reason, float a_Value) override;
	virtual void SendWholeInventory             (const cWindow & a_Window, const cItem & a_CursorStack) override;
	virtual void SendWindowClose                (const cWindow & a_Window) override;
	virtual void SendWindowOpen                 (const cWindow & a_Window) override;
	virtual void SendWindowProperty             (const cWindow & a_Window, size_t a_Property, short a_Value) override;

	virtual AString GetAuthServerID(void) override { return m_AuthServerID; }

	/** Compress the packet. a_Packet must be without packet length.
	a_Compressed will be set to the compressed packet includes packet length and data length. */
	static void CompressPacket(CircularBufferCompressor & a_Packet, ContiguousByteBuffer & a_Compressed);

	virtual State GetCurrentState(void) const override { return m_State; }

protected:

	/** State of the protocol. */
	State m_State;

	/** Converts the BlockFace received by the protocol into eBlockFace constants.
	If the received value doesn't match any of our eBlockFace constants, BLOCK_FACE_NONE is returned. */
	static eBlockFace FaceIntToBlockFace(Int32 a_FaceInt);

	/** Get the packet ID for a given packet. */
	virtual UInt32 GetPacketID(ePacketType a_Packet) const override;

	/** Converts an animation into an ID suitable for use with the Entity Animation packet.
	Returns (uchar)-1 if the protocol version doesn't support this animation. */
	virtual unsigned char GetProtocolEntityAnimation(EntityAnimation a_Animation) const;

	/** Converts an animation into an ID suitable for use with the Entity Status packet.
	Returns -1 if the protocol version doesn't support this animation. */
	virtual signed char GetProtocolEntityStatus(EntityAnimation a_Animation) const;

	/** Converts an entity to a protocol-specific entity type.
	Only entities that the Send Spawn Entity packet supports are valid inputs to this method */
	virtual UInt8 GetProtocolEntityType(const cEntity & a_Entity) const;

	/** Converts eMonsterType to protocol-specific mob types */
	virtual UInt32 GetProtocolMobType(eMonsterType a_MobType) const;

	/** The 1.8 protocol use a particle id instead of a string. This function converts the name to the id. If the name is incorrect, it returns 0. */
	virtual int GetProtocolParticleID(const AString & a_ParticleName) const;

	/** Returns the protocol version. */
	virtual Version GetProtocolVersion() const override;

	/** Reads and handles the packet. The packet length and type have already been read.
	Returns true if the packet was understood, false if it was an unknown packet. */
	virtual bool HandlePacket(cByteBuffer & a_ByteBuffer, UInt32 a_PacketType);

	// Packet handlers while in the Status state (m_State == 1):
	virtual void HandlePacketStatusPing(cByteBuffer & a_ByteBuffer);
	virtual void HandlePacketStatusRequest(cByteBuffer & a_ByteBuffer);

	// Packet handlers while in the Login state (m_State == 2):
	virtual void HandlePacketLoginEncryptionResponse(cByteBuffer & a_ByteBuffer);
	virtual void HandlePacketLoginStart(cByteBuffer & a_ByteBuffer);
	virtual void HandlePacketEnterConfiguration(cByteBuffer & a_ByteBuffer);

	// Packet handlers while in the Configuration state (m_State == 4):  Only used in 1.20.2+
	virtual void HandlePacketReady(cByteBuffer & a_ByteBuffer);

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
	virtual void HandlePacketPlayerSession          (cByteBuffer & a_ByteBuffer);
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
	virtual void HandlePacketBookUpdate             (cByteBuffer & a_ByteBuffer);
	virtual void HandlePacketCommandExecution       (cByteBuffer & a_ByteBuffer);
	/** Parses Vanilla plugin messages into specific ClientHandle calls.
	The message payload is still in the bytebuffer, the handler reads it specifically for each handled channel. */
	virtual void HandleVanillaPluginMessage(cByteBuffer & a_ByteBuffer, std::string_view a_Channel);

	/** Parses item metadata as read by ReadItem(), into the item enchantments. */
	virtual void ParseItemMetadata(cItem & a_Item, ContiguousByteBufferView a_Metadata) const;

	/** Reads an item out of the received data, sets a_Item to the values read.
	Returns false if not enough received data.
	a_KeepRemainingBytes tells the function to keep that many bytes at the end of the buffer. */
	virtual bool ReadItem(cByteBuffer & a_ByteBuffer, cItem & a_Item, size_t a_KeepRemainingBytes = 0) const;

	/** Sends the entity type and entity-dependent data required for the entity to initially spawn. */
	virtual void SendEntitySpawn(const cEntity & a_Entity, const UInt8 a_ObjectType, const Int32 a_ObjectData);

	/** Sends the packet to the client. Called by the cPacketizer's destructor. */
	virtual void SendPacket(cPacketizer & a_Packet) override;

	/** Writes the block entity data for the specified block entity into the packet. */
	virtual void WriteBlockEntity(cFastNBTWriter & a_Writer, const cBlockEntity & a_BlockEntity) const;

	/** Writes the metadata for the specified entity, not including the terminating 0x7f. */
	virtual void WriteEntityMetadata(cPacketizer & a_Pkt, const cEntity & a_Entity) const;

	/** Writes the entity properties for the specified entity, including the Count field. */
	virtual void WriteEntityProperties(cPacketizer & a_Pkt, const cEntity & a_Entity) const;

	/** Writes the item data into a packet. */
	virtual void WriteItem(cPacketizer & a_Pkt, const cItem & a_Item) const;

	/** Writes the mob-specific metadata for the specified mob */
	virtual void WriteMobMetadata(cPacketizer & a_Pkt, const cMonster & a_Mob) const;

	void StartEncryption(const Byte * a_Key);

private:

	AString m_ServerAddress;

	AString m_AuthServerID;

	bool m_IsEncrypted;

	cAesCfb128Decryptor m_Decryptor;
	cAesCfb128Encryptor m_Encryptor;

	CircularBufferCompressor m_Compressor;
	CircularBufferExtractor m_Extractor;

	/** The logfile where the comm is logged, when g_ShouldLogComm is true */
	cFile m_CommLogFile;

	/** Adds the received (unencrypted) data to m_ReceivedData, parses complete packets */
	void AddReceivedData(cByteBuffer & a_Buffer, ContiguousByteBufferView a_Data);

	/** Converts a statistic to a protocol-specific string.
	Protocols <= 1.12 use strings, hence this is a static as the string-mapping was append-only for the versions that used it.
	Returns an empty string, handled correctly by the client, for newer, unsupported statistics. */
	static const char * GetProtocolStatisticName(CustomStatistic a_Statistic);

	/** Handle a complete packet stored in the given buffer. */
	void HandlePacket(cByteBuffer & a_Buffer);
} ;
