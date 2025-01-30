
// Protocol.h

// Interfaces to the cProtocol class representing the generic interface that a protocol
// parser and serializer must implement





#pragma once

#include "../Defines.h"
#include "../Scoreboard.h"
#include "../ByteBuffer.h"
#include "../EffectID.h"
#include "../World.h"
#include "../WorldStorage/FastNBT.h"
//#include "../ClientHandle.h"





class cMap;
class cExpOrb;
class cPlayer;
class cEntity;
class cWindow;
class cPainting;
class cWorld;
class cMonster;
class cCompositeChat;
class cPacketizer;

struct StatisticsManager;


static const int CompressionThreshold = 128;  // After how large a packet should we compress it.


class cProtocol
{
public:

	cProtocol(cClientHandle * a_Client) :
		m_Client(a_Client),
		m_OutPacketBuffer(64 KiB),
		m_OutPacketLenBuffer(20)  // 20 bytes is more than enough for one VarInt
	{
	}

	virtual ~cProtocol() {}

	/** Logical types of outgoing packets.
	These values get translated to on-wire packet IDs in GetPacketID(), specific for each protocol.
	This is mainly useful for protocol sub-versions that re-number the packets while using mostly the same packet layout. */
	enum ePacketType
	{
		pktAttachEntity = 0,
		pktCustomPayload,
		pktCommnadTree,
		pktLightUpdate,
		pktPlayerActionResponse,
		pktRenderDistanceCenter,
		pktBlockAction,
		pktBlockBreakAnim,
		pktBlockChange,
		pktBlockChanges,
		pktBossBar,
		pktWorldBorder,
		pktCameraSetTo,
		pktChatRaw,
		pktCollectEntity,
		pktConfigurationReady,
		pktConfigurationCustomPayload,
		pktConfigurationDynamicRegistries,
		pktConfigurationTags,
		pktDestroyEntity,
		pktDifficulty,
		pktDisconnectDuringLogin,
		pktDisconnectDuringGame,
		pktDisplayObjective,
		pktEditSign,
		pktEncryptionRequest,
		pktEntityAnimation,
		pktEntityEffect,
		pktEntityEquipment,
		pktEntityHeadLook,
		pktEntityLook,
		pktEntityMeta,
		pktEntityProperties,
		pktEntityRelMove,
		pktEntityRelMoveLook,
		pktEntityStatus,
		pktEntityVelocity,
		pktExperience,
		pktExplosion,
		pktGameMode,
		pktHeldItemChange,
		pktHorseWindowOpen,
		pktInventorySlot,
		pktJoinGame,
		pktKeepAlive,
		pktLeashEntity,
		pktLoginSuccess,
		pktMapData,
		pktParticleEffect,
		pktPingResponse,
		pktPlayerAbilities,
		pktPlayerList,
		pktPlayerListHeaderFooter,
		pktPlayerLstRemove,
		pktPlayerMoveLook,
		pktPluginMessage,
		pktRemoveEntityEffect,
		pktResourcePack,
		pktRespawn,
		pktScoreboardObjective,
		pktSpawnObject,
		pktSoundEffect,
		pktSetCursorItem,
		pktSelectKnownPacks,
		pktSoundParticleEffect,
		pktSpawnExperienceOrb,
		pktSpawnGlobalEntity,
		pktSpawnMob,
		pktSpawnOtherPlayer,
		pktSpawnPainting,
		pktSpawnPosition,
		pktStartCompression,
		pktStatistics,
		pktStatusResponse,
		pktTabCompletionResults,
		pktTeleportEntity,
		pktTimeUpdate,
		pktTitle,
		pktUnloadChunk,
		pktUnlockRecipe,
		pktUpdateBlockEntity,
		pktUpdateHealth,
		pktUpdateScore,
		pktUpdateSign,
		pktUseBed,
		pktWeather,
		pktWindowItems,
		pktWindowClose,
		pktWindowOpen,
		pktWindowProperty
	};

	enum class EntityMetadata
	{
		EntityFlags,
		EntityAir,
		EntityCustomName,
		EntityCustomNameVisible,
		EntitySilent,
		EntityNoGravity,
		EntityPose,

		PotionThrown,

		FallingBlockPosition,

		AreaEffectCloudRadius,
		AreaEffectCloudColor,
		AreaEffectCloudSinglePointEffect,
		AreaEffectCloudParticleId,
		AreaEffectCloudParticleParameter1,
		AreaEffectCloudParticleParameter2,

		ArrowFlags,
		TippedArrowColor,
		PiercingLevel,

		BoatLastHitTime,
		BoatForwardDirection,
		BoatDamageTaken,
		BoatType,
		BoatLeftPaddleTurning,
		BoatRightPaddleTurning,
		BoatSplashTimer,

		EnderCrystalBeamTarget,
		EnderCrystalShowBottom,

		WitherSkullInvulnerable,

		FireworkInfo,
		FireworkBoostedEntityId,
		FireworkFromCrossbow,
		FrozenTicks,

		ItemFrameItem,
		ItemFrameRotation,

		ItemItem,

		LivingActiveHand,
		LivingHealth,
		LivingPotionEffectColor,
		LivingPotionEffectAmbient,
		LivingNumberOfArrows,

		PlayerAdditionalHearts,
		PlayerScore,
		PlayerDisplayedSkinParts,
		PlayerMainHand,

		ArmorStandStatus,
		ArmorStandHeadRotation,
		ArmorStandBodyRotation,
		ArmorStandLeftArmRotation,
		ArmorStandRightArmRotation,
		ArmorStandLeftLegRotation,
		ArmorStandRightLegRotation,

		InsentientFlags,

		BatHanging,

		AgeableIsBaby,

		AbstractHorseFlags,
		AbstractHorseOwner,

		HorseVariant,
		HorseArmour,

		ChestedHorseChested,

		LlamaStrength,
		LlamaCarpetColor,
		LlamaVariant,

		PigHasSaddle,
		PigTotalCarrotOnAStickBoost,

		RabbitType,

		PolarBearStanding,

		SheepFlags,

		TameableAnimalFlags,
		TameableAnimalOwner,

		ThrowableItem,

		TridentLoyaltyLevel,

		OcelotType,

		WolfDamageTaken,
		WolfBegging,
		WolfCollarColour,

		VillagerProfession,

		IronGolemPlayerCreated,

		ShulkerFacingDirection,
		ShulkerAttachmentFallingBlockPosition,
		ShulkerShieldHeight,

		BlazeOnFire,

		CreeperState,
		CreeperPowered,
		CreeperIgnited,

		GuardianStatus,
		GuardianTarget,

		IllagerFlags,
		SpeIlagerSpell,

		VexFlags,

		AbstractSkeletonArmsSwinging,

		SpiderClimbing,

		WitchAggresive,
		WitchDrinking,

		WitherFirstHeadTarget,
		WitherSecondHeadTarget,
		WitherThirdHeadTarget,
		WitherInvulnerableTimer,

		ZombieIsBaby,
		ZombieUnusedWasType,
		ZombieHandsRisedUp,

		ZombieVillagerConverting,
		ZombieVillagerProfession,

		EndermanCarriedBlock,
		EndermanScreaming,

		EnderDragonDragonPhase,

		GhastAttacking,

		SlimeSize,

		MinecartShakingPower,
		MinecartShakingDirection,
		MinecartShakingMultiplier,
		MinecartBlockIDMeta,
		MinecartBlockY,
		MinecartShowBlock,

		MinecartCommandBlockCommand,
		MinecartCommandBlockLastOutput,

		MinecartFurnacePowered,

		MooshroomType,

		TNTPrimedFuseTime
	};
	enum class Hand	 //  Used by the book update packet  //  A better, more uniqie name should be used instead
	{
		MAIN_HAND,
		OFF_HAND
	};
	enum class PlayerActionResponses
	{
		START_DESTROY_BLOCK,
		ABORT_DESTROY_BLOCK,
		STOP_DESTROY_BLOCK,
		DROP_ALL_ITEMS,
		DROP_ITEM,
		RELEASE_USE_ITEM,
		SWAP_HELD_ITEMS
	};
	enum class EntityMetadataType
	{
		Byte,
		VarInt,
		Float,
		String,
		Chat,
		OptChat,
		Item,
		Boolean,
		Rotation,
		Position,
		OptPosition,
		Direction,
		OptUUID,
		OptBlockID,
		NBT,
		Particle,
		VillagerData,
		OptVarInt,
		Pose
	};


	/** The protocol version number, received from the client in the Handshake packet. */
	enum class Version
	{
		v1_8_0  = 47,
		v1_9_0  = 107,
		v1_9_1  = 108,
		v1_9_2  = 109,
		v1_9_4  = 110,
		v1_10_0 = 210,
		v1_11_0 = 315,
		v1_11_1 = 316,
		v1_12   = 335,
		v1_12_1 = 338,
		v1_12_2 = 340,
		v1_13   = 393,
		v1_13_1 = 401,
		v1_13_2 = 404,
		v1_14   = 477,
		v1_14_1 = 480,
		v1_14_2 = 485,
		v1_14_3 = 490,
		v1_14_4 = 498,
		v1_15   = 573,
		v1_15_1 = 575,
		v1_15_2 = 578,
		v1_16   = 735,
		v1_16_1 = 736,
		v1_16_2 = 751,
		v1_16_3 = 753,
		v1_16_4 = 754,
		// v1_16_5 = 754,
		v1_17   = 755,
		v1_17_1 = 756,
		v1_18   = 757,
		// v1_18_1 = 757,
		v1_18_2 = 758,
		v1_19   = 759,
		v1_19_1 = 760,
		// v1_19_2 = 760,
		v1_19_3 = 761,
		v1_19_4 = 762,
		v1_20   = 763,
		// v1_20_1 = 763,
		v1_20_2 = 764,
		v1_20_3 = 765,
		// v1_20_4 = 765,
		v1_20_5 = 766,
		//  v1_20_6 = 766,
		v1_21 = 767,
		// v1_21_1 = 767,
		v1_21_2 = 768,
		// v1_21_3 = 768
		v1_21_4 = 769,
		Latest = v1_21_4,
	};

	enum State
	{
		Status = 1,
		Login = 2,
		Game = 3,
		Configuration = 4,
	};

	enum class PlayerListAction : std::uint8_t
	{
		AddPlayer = 1,
		InitializeChat = 2,
		UpdateGameMode = 4,
		UpdateListed = 8,
		UpdateLatency = 16,
		UpdateDisplayName = 32,
	};

	/** Called by cClientHandle to process data, when the client sends some.
	The protocol uses the provided buffers for storage and processing, and must have exclusive access to them. */
	virtual void DataReceived(cByteBuffer & a_Buffer, ContiguousByteBuffer & a_Data) = 0;

	/** Called by cClientHandle to finalise a buffer of prepared data before they are sent to the client.
	Descendants may for example, encrypt the data if needed.
	The protocol modifies the provided buffer in-place. */
	virtual void DataPrepared(ContiguousByteBuffer & a_Data) = 0;

	// Sending stuff to clients (alphabetically sorted):
	virtual void SendAcknowledgeBlockChange     (int a_SequenceId) = 0;
	virtual void SendAttachEntity               (const cEntity & a_Entity, const cEntity & a_Vehicle) = 0;
	virtual void SendBlockAction                (Vector3i a_BlockPos, char a_Byte1, char a_Byte2, BlockState a_Block) = 0;
	virtual void SendBlockBreakAnim             (UInt32 a_EntityID, Vector3i a_BlockPos, char a_Stage) = 0;
	virtual void SendBlockChange                (Vector3i a_BlockPos, BlockState a_Block) = 0;
	virtual void SendBlockChanges               (int a_ChunkX, int a_ChunkZ, const sSetBlockVector & a_Changes) = 0;
	virtual void SendBossBarAdd                 (UInt32 a_UniqueID, const cCompositeChat & a_Title, float a_FractionFilled, BossBarColor a_Color, BossBarDivisionType a_DivisionType, bool a_DarkenSky, bool a_PlayEndMusic, bool a_CreateFog) = 0;
	virtual void SendBossBarRemove              (UInt32 a_UniqueID) = 0;
	virtual void SendBossBarUpdateHealth        (UInt32 a_UniqueID, float a_FractionFilled) = 0;
	virtual void SendBossBarUpdateFlags         (UInt32 a_UniqueID, bool a_DarkenSky, bool a_PlayEndMusic, bool a_CreateFog) = 0;
	virtual void SendBossBarUpdateStyle         (UInt32 a_UniqueID, BossBarColor a_Color, BossBarDivisionType a_DivisionType) = 0;
	virtual void SendBossBarUpdateTitle         (UInt32 a_UniqueID, const cCompositeChat & a_Title) = 0;
	virtual void SendCameraSetTo                (const cEntity & a_Entity) = 0;
	virtual void SendChat                       (const AString & a_Message, eChatType a_Type) = 0;
	virtual void SendChat                       (const cCompositeChat & a_Message, eChatType a_Type, bool a_ShouldUseChatPrefixes) = 0;
	virtual void SendChatRaw                    (const AString & a_MessageRaw, eChatType a_Type) = 0;
	virtual void SendChunkData                  (ContiguousByteBufferView a_ChunkData) = 0;
	virtual void SendCollectEntity              (const cEntity & a_Collected, const cEntity & a_Collector, unsigned a_Count) = 0;
	virtual void SendCommandTree                (void) = 0;
	virtual void SendDestroyEntity              (const cEntity & a_Entity) = 0;
	virtual void SendDetachEntity               (const cEntity & a_Entity, const cEntity & a_PreviousVehicle) = 0;
	virtual void SendDisconnect                 (const AString & a_Reason) = 0;
	virtual void SendDynamicRegistries          (void) = 0;
	virtual void SendEditSign                   (Vector3i a_BlockPos) = 0;  ///< Request the client to open up the sign editor for the sign (1.6+)
	virtual void SendEntityEffect               (const cEntity & a_Entity, int a_EffectID, int a_Amplifier, int a_Duration) = 0;
	virtual void SendEntityAnimation            (const cEntity & a_Entity, EntityAnimation a_Animation) = 0;
	virtual void SendEntityEquipment            (const cEntity & a_Entity, short a_SlotNum, const cItem & a_Item) = 0;
	virtual void SendEntityHeadLook             (const cEntity & a_Entity) = 0;
	virtual void SendEntityLook                 (const cEntity & a_Entity) = 0;
	virtual void SendEntityMetadata             (const cEntity & a_Entity) = 0;
	virtual void SendEntityPosition             (const cEntity & a_Entity) = 0;
	virtual void SendEntityProperties           (const cEntity & a_Entity) = 0;
	virtual void SendEntityVelocity             (const cEntity & a_Entity) = 0;
	virtual void SendExplosion                  (Vector3f a_Position, float a_Power) = 0;
	virtual void SendFinishConfiguration        (void) = 0;
	virtual void SendGameMode                   (eGameMode a_GameMode) = 0;
	virtual void SendHealth                     (void) = 0;
	virtual void SendHeldItemChange             (int a_ItemIndex) = 0;
	virtual void SendHideTitle                  (void) = 0;
	virtual void SendInventorySlot              (char a_WindowID, short a_SlotNum, const cItem & a_Item) = 0;
	virtual void SendKeepAlive                  (UInt32 a_PingID) = 0;
	virtual void SendSelectKnownPacks           (void) = 0;
	virtual void SendLeashEntity                (const cEntity & a_Entity, const cEntity & a_EntityLeashedTo) = 0;
	virtual void SendLogin                      (const cPlayer & a_Player, const cWorld & a_World) = 0;
	virtual void SendLoginSuccess               (void) = 0;
	virtual void SendMapData                    (const cMap & a_Map, int a_DataStartX, int a_DataStartY) = 0;
	virtual void SendPaintingSpawn              (const cPainting & a_Painting) = 0;
	virtual void SendPlayerAbilities            (void) = 0;
	virtual void SendParticleEffect             (const AString & a_SoundName, Vector3f a_Src, Vector3f a_Offset, float a_ParticleData, int a_ParticleAmount) = 0;
	virtual void SendParticleEffect             (const AString & a_SoundName, Vector3f a_Src, Vector3f a_Offset, float a_ParticleData, int a_ParticleAmount, std::array<int, 2> a_Data) = 0;
	virtual void SendPlayerActionResponse       (Vector3i a_blockpos, int a_state_id, cProtocol::PlayerActionResponses a_action, bool a_IsApproved) = 0;
	virtual void SendPlayerListAddPlayer        (const cPlayer & a_Player) = 0;
	virtual void SendPlayerListInitChat         (const cPlayer & a_Player) = 0;
	virtual void SendPlayerListHeaderFooter     (const cCompositeChat & a_Header, const cCompositeChat & a_Footer) = 0;
	virtual void SendPlayerListRemovePlayer     (const cPlayer & a_Player) = 0;
	virtual void SendPlayerListUpdateGameMode   (const cPlayer & a_Player) = 0;
	virtual void SendPlayerListUpdatePing       () = 0;
	virtual void SendPlayerListUpdateDisplayName(const cPlayer & a_Player, const AString & a_CustomName) = 0;
	virtual void SendPlayerMoveLook             (Vector3d a_Pos, float a_Yaw, float a_Pitch, bool a_IsRelative) = 0;
	virtual void SendPlayerMoveLook             (void) = 0;
	virtual void SendPlayerPermissionLevel      (void) = 0;
	virtual void SendPlayerPosition             (void) = 0;
	virtual void SendPlayerSpawn                (const cPlayer & a_Player) = 0;
	virtual void SendPluginMessage              (const AString & a_Channel, ContiguousByteBufferView a_Message) = 0;
	virtual void SendRemoveEntityEffect         (const cEntity & a_Entity, int a_EffectID) = 0;
	virtual void SendResetTitle                 (void) = 0;
	virtual void SendResourcePack               (const AString & a_ResourcePackUrl) = 0;
	virtual void SendRespawn                    (eDimension a_Dimension) = 0;
	virtual void SendRenderDistanceCenter       (cChunkCoords a_chunk) = 0;
	virtual void SendExperience                 (void) = 0;
	virtual void SendExperienceOrb              (const cExpOrb & a_ExpOrb) = 0;
	virtual void SendScoreboardObjective        (const AString & a_Name, const AString & a_DisplayName, Byte a_Mode) = 0;
	virtual void SendScoreUpdate                (const AString & a_Objective, const AString & a_Player, cObjective::Score a_Score, Byte a_Mode) = 0;
	virtual void SendDisplayObjective           (const AString & a_Objective, cScoreboard::eDisplaySlot a_Display) = 0;
	virtual void SendSetSubTitle                (const cCompositeChat & a_SubTitle) = 0;
	virtual void SendSetRawSubTitle             (const AString & a_SubTitle) = 0;
	virtual void SendSetTitle                   (const cCompositeChat & a_Title) = 0;
	virtual void SendSetRawTitle                (const AString & a_Title) = 0;
	virtual void SendSoundEffect                (const AString & a_SoundName, Vector3d a_Origin, float a_Volume, float a_Pitch) = 0;
	virtual void SendSoundParticleEffect        (const EffectID a_EffectID, Vector3i a_Origin, int a_Data) = 0;
	virtual void SendSpawnEntity                (const cEntity & a_Entity) = 0;
	virtual void SendSpawnMob                   (const cMonster & a_Mob) = 0;
	virtual void SendStatistics                 (const StatisticsManager & a_Manager) = 0;
	virtual void SendTabCompletionResults       (const AStringVector & a_Results, UInt32 CompletionId) = 0;
	virtual void SendTags                       (void) = 0;
	virtual void SendThunderbolt                (Vector3i a_Origin) = 0;
	virtual void SendTitleTimes                 (int a_FadeInTicks, int a_DisplayTicks, int a_FadeOutTicks) = 0;
	virtual void SendTimeUpdate                 (cTickTimeLong a_WorldAge, cTickTimeLong a_WorldDate, bool a_DoDaylightCycle) = 0;
	virtual void SendUnleashEntity              (const cEntity & a_Entity) = 0;
	virtual void SendUnloadChunk                (int a_ChunkX, int a_ChunkZ) = 0;
	virtual void SendUpdateBlockEntity          (cBlockEntity & a_BlockEntity) = 0;
	virtual void SendUpdateSign                 (Vector3i a_BlockPos, const AString & a_Line1, const AString & a_Line2, const AString & a_Line3, const AString & a_Line4) = 0;
	virtual void SendUnlockRecipe               (UInt32 a_RecipeID) = 0;
	virtual void SendInitRecipes                (UInt32 a_RecipeID) = 0;
	virtual void SendInitialChunksComing        () = 0;
	virtual void SendWeather                    (eWeather a_Weather) = 0;
	virtual void SendGameStateChange            (eGameStateReason a_Reason, float a_Value) = 0;
	virtual void SendWholeInventory             (const cWindow    & a_Window, const cItem & a_CursorStack) = 0;
	virtual void SendWindowClose                (const cWindow    & a_Window) = 0;
	virtual void SendWindowOpen                 (const cWindow & a_Window) = 0;
	virtual void SendWindowProperty             (const cWindow & a_Window, size_t a_Property, short a_Value) = 0;

	/** Writes the block entity data for the specified block entity into the packet.
	*   Needs to be here so ChunkDataSerializer can access it. */
	virtual void WriteBlockEntity(cFastNBTWriter & a_Writer, const cBlockEntity & a_BlockEntity) const = 0;

	/** Returns the ServerID used for authentication through session.minecraft.net */
	virtual AString GetAuthServerID(void) = 0;

	virtual State GetCurrentState(void) const = 0;

	/** In case the protocol does not support the given parser it returns -1 */
	virtual Int32 GetProtocolCommandArgumentID(eCommandParserType a_ParserType) const { return -1;}

	virtual UInt32 GetProtocolSoundID(const AString & a_SoundName) const { return 1;}

	virtual UInt32 GetBlockEntityID(const cBlockEntity & a_BlockEntity) const { return 0;}
protected:

	friend class cPacketizer;

	cClientHandle * m_Client;

	bool m_CompressionEnabled = false;

	bool m_IsConfigReadySent = false;  // only used in 1.20.2+ Basically used as a hack

	/** Provides synchronization for sending the entire packet at once.
	Each SendXYZ() function must acquire this CS in order to send the whole packet at once.
	Automated via cPacketizer class. */
	cCriticalSection m_CSPacket;

	/** Buffer for composing the outgoing packets, through cPacketizer */
	cByteBuffer m_OutPacketBuffer;

	/** Buffer for composing packet length (so that each cPacketizer instance doesn't allocate a new cPacketBuffer) */
	cByteBuffer m_OutPacketLenBuffer;

	/** Returns the protocol-specific packet ID given the protocol-agnostic packet enum. */
	virtual UInt32 GetPacketID(ePacketType a_Packet) const = 0;

	/** Returns the current protocol's version, for handling status requests. */
	virtual Version GetProtocolVersion() const = 0;

	/** Sends a single packet contained within the cPacketizer class.
	The cPacketizer's destructor calls this to send the contained packet; protocol may transform the data (compression in 1.8 etc). */
	virtual void SendPacket(cPacketizer & a_Packet) = 0;
} ;
