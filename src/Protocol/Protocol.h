
// Protocol.h

// Interfaces to the cProtocol class representing the generic interface that a protocol
// parser and serializer must implement





#pragma once

#include "../Defines.h"
#include "../Scoreboard.h"
#include "../ByteBuffer.h"
#include "../EffectID.h"
#include "../World.h"





class cMap;
class cExpOrb;
class cPlayer;
class cEntity;
class cWindow;
class cPickup;
class cPainting;
class cWorld;
class cMonster;
class cChunkDataSerializer;
class cFallingBlock;
class cCompositeChat;
class cStatManager;
class cPacketizer;





typedef unsigned char Byte;





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
		pktBlockAction,
		pktBlockBreakAnim,
		pktBlockChange,
		pktBlockChanges,
		pktCameraSetTo,
		pktChatRaw,
		pktCollectEntity,
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
		pktPlayerMaxSpeed,
		pktPlayerMoveLook,
		pktPluginMessage,
		pktRemoveEntityEffect,
		pktResourcePack,
		pktRespawn,
		pktScoreboardObjective,
		pktSpawnObject,
		pktSoundEffect,
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

		TNTPrimedFuseTime
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
		v1_13_2 = 404,  // TODO: this constant should be in WebServer
		v1_14   = 477
	};

	enum State
	{
		Status = 1,
		Login = 2,
		Game = 3,
	};

	/** Called when client sends some data */
	virtual void DataReceived(cByteBuffer & a_Buffer, const char * a_Data, size_t a_Size) = 0;

	// Sending stuff to clients (alphabetically sorted):
	virtual void SendAttachEntity               (const cEntity & a_Entity, const cEntity & a_Vehicle) = 0;
	virtual void SendBlockAction                (int a_BlockX, int a_BlockY, int a_BlockZ, char a_Byte1, char a_Byte2, BLOCKTYPE a_BlockType) = 0;
	virtual void SendBlockBreakAnim             (UInt32 a_EntityID, int a_BlockX, int a_BlockY, int a_BlockZ, char a_Stage) = 0;
	virtual void SendBlockChange                (int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta) = 0;
	virtual void SendBlockChanges               (int a_ChunkX, int a_ChunkZ, const sSetBlockVector & a_Changes) = 0;
	virtual void SendCameraSetTo                (const cEntity & a_Entity) = 0;
	virtual void SendChat                       (const AString & a_Message, eChatType a_Type) = 0;
	virtual void SendChat                       (const cCompositeChat & a_Message, eChatType a_Type, bool a_ShouldUseChatPrefixes) = 0;
	virtual void SendChatRaw                    (const AString & a_MessageRaw, eChatType a_Type) = 0;
	virtual void SendChunkData                  (const std::string_view a_ChunkData) = 0;
	virtual void SendCollectEntity              (const cEntity & a_Collected, const cEntity & a_Collector, unsigned a_Count) = 0;
	virtual void SendDestroyEntity              (const cEntity & a_Entity) = 0;
	virtual void SendDetachEntity               (const cEntity & a_Entity, const cEntity & a_PreviousVehicle) = 0;
	virtual void SendDisconnect                 (const AString & a_Reason) = 0;
	virtual void SendEditSign                   (int a_BlockX, int a_BlockY, int a_BlockZ) = 0;  ///< Request the client to open up the sign editor for the sign (1.6+)
	virtual void SendEntityEffect               (const cEntity & a_Entity, int a_EffectID, int a_Amplifier, int a_Duration) = 0;
	virtual void SendEntityAnimation            (const cEntity & a_Entity, char a_Animation) = 0;
	virtual void SendEntityEquipment            (const cEntity & a_Entity, short a_SlotNum, const cItem & a_Item) = 0;
	virtual void SendEntityHeadLook             (const cEntity & a_Entity) = 0;
	virtual void SendEntityLook                 (const cEntity & a_Entity) = 0;
	virtual void SendEntityMetadata             (const cEntity & a_Entity) = 0;
	virtual void SendEntityPosition             (const cEntity & a_Entity) = 0;
	virtual void SendEntityProperties           (const cEntity & a_Entity) = 0;
	virtual void SendEntityStatus               (const cEntity & a_Entity, char a_Status) = 0;
	virtual void SendEntityVelocity             (const cEntity & a_Entity) = 0;
	virtual void SendExplosion                  (Vector3f a_Position, float a_Power) = 0;
	virtual void SendGameMode                   (eGameMode a_GameMode) = 0;
	virtual void SendHealth                     (void) = 0;
	virtual void SendHeldItemChange             (int a_ItemIndex) = 0;
	virtual void SendHideTitle                  (void) = 0;
	virtual void SendInventorySlot              (char a_WindowID, short a_SlotNum, const cItem & a_Item) = 0;
	virtual void SendKeepAlive                  (UInt32 a_PingID) = 0;
	virtual void SendLeashEntity                (const cEntity & a_Entity, const cEntity & a_EntityLeashedTo) = 0;
	virtual void SendLogin                      (const cPlayer & a_Player, const cWorld & a_World) = 0;
	virtual void SendLoginSuccess               (void) = 0;
	virtual void SendMapData                    (const cMap & a_Map, int a_DataStartX, int a_DataStartY) = 0;
	virtual void SendPaintingSpawn              (const cPainting & a_Painting) = 0;
	virtual void SendPlayerAbilities            (void) = 0;
	virtual void SendParticleEffect             (const AString & a_SoundName, float a_SrcX, float a_SrcY, float a_SrcZ, float a_OffsetX, float a_OffsetY, float a_OffsetZ, float a_ParticleData, int a_ParticleAmount) = 0;
	virtual void SendParticleEffect             (const AString & a_SoundName, Vector3f a_Src, Vector3f a_Offset, float a_ParticleData, int a_ParticleAmount, std::array<int, 2> a_Data) = 0;
	virtual void SendPlayerListAddPlayer        (const cPlayer & a_Player) = 0;
	virtual void SendPlayerListRemovePlayer     (const cPlayer & a_Player) = 0;
	virtual void SendPlayerListUpdateGameMode   (const cPlayer & a_Player) = 0;
	virtual void SendPlayerListUpdatePing       (const cPlayer & a_Player) = 0;
	virtual void SendPlayerListUpdateDisplayName(const cPlayer & a_Player, const AString & a_CustomName) = 0;
	virtual void SendPlayerMaxSpeed             (void) = 0;  ///< Informs the client of the maximum player speed (1.6.1+)
	virtual void SendPlayerMoveLook             (void) = 0;
	virtual void SendPlayerPosition             (void) = 0;
	virtual void SendPlayerSpawn                (const cPlayer & a_Player) = 0;
	virtual void SendPluginMessage              (const AString & a_Channel, const AString & a_Message) = 0;
	virtual void SendRemoveEntityEffect         (const cEntity & a_Entity, int a_EffectID) = 0;
	virtual void SendResetTitle                 (void) = 0;
	virtual void SendResourcePack               (const AString & a_ResourcePackUrl) = 0;
	virtual void SendRespawn                    (eDimension a_Dimension) = 0;
	virtual void SendExperience                 (void) = 0;
	virtual void SendExperienceOrb              (const cExpOrb & a_ExpOrb) = 0;
	virtual void SendScoreboardObjective        (const AString & a_Name, const AString & a_DisplayName, Byte a_Mode) = 0;
	virtual void SendScoreUpdate                (const AString & a_Objective, const AString & a_Player, cObjective::Score a_Score, Byte a_Mode) = 0;
	virtual void SendDisplayObjective           (const AString & a_Objective, cScoreboard::eDisplaySlot a_Display) = 0;
	virtual void SendSetSubTitle                (const cCompositeChat & a_SubTitle) = 0;
	virtual void SendSetRawSubTitle             (const AString & a_SubTitle) = 0;
	virtual void SendSetTitle                   (const cCompositeChat & a_Title) = 0;
	virtual void SendSetRawTitle                (const AString & a_Title) = 0;
	virtual void SendSoundEffect                (const AString & a_SoundName, double a_X, double a_Y, double a_Z, float a_Volume, float a_Pitch) = 0;
	virtual void SendSoundParticleEffect        (const EffectID a_EffectID, int a_SrcX, int a_SrcY, int a_SrcZ, int a_Data) = 0;
	virtual void SendSpawnEntity                (const cEntity & a_Entity) = 0;
	virtual void SendSpawnMob                   (const cMonster & a_Mob) = 0;
	virtual void SendStatistics                 (const cStatManager & a_Manager) = 0;
	virtual void SendTabCompletionResults       (const AStringVector & a_Results) = 0;
	virtual void SendThunderbolt                (int a_BlockX, int a_BlockY, int a_BlockZ) = 0;
	virtual void SendTitleTimes                 (int a_FadeInTicks, int a_DisplayTicks, int a_FadeOutTicks) = 0;
	virtual void SendTimeUpdate                 (Int64 a_WorldAge, Int64 a_TimeOfDay, bool a_DoDaylightCycle) = 0;
	virtual void SendUnleashEntity              (const cEntity & a_Entity) = 0;
	virtual void SendUnloadChunk                (int a_ChunkX, int a_ChunkZ) = 0;
	virtual void SendUpdateBlockEntity          (cBlockEntity & a_BlockEntity) = 0;
	virtual void SendUpdateSign                 (int a_BlockX, int a_BlockY, int a_BlockZ, const AString & a_Line1, const AString & a_Line2, const AString & a_Line3, const AString & a_Line4) = 0;
	virtual void SendUseBed                     (const cEntity & a_Entity, int a_BlockX, int a_BlockY, int a_BlockZ) = 0;
	virtual void SendUnlockRecipe               (UInt32 a_RecipeID) = 0;
	virtual void SendInitRecipes                (UInt32 a_RecipeID) = 0;
	virtual void SendWeather                    (eWeather a_Weather) = 0;
	virtual void SendWholeInventory             (const cWindow    & a_Window) = 0;
	virtual void SendWindowClose                (const cWindow    & a_Window) = 0;
	virtual void SendWindowOpen                 (const cWindow & a_Window) = 0;
	virtual void SendWindowProperty             (const cWindow & a_Window, size_t a_Property, short a_Value) = 0;

	/** Returns the ServerID used for authentication through session.minecraft.net */
	virtual AString GetAuthServerID(void) = 0;

protected:

	friend class cPacketizer;

	cClientHandle * m_Client;

	/** Provides synchronization for sending the entire packet at once.
	Each SendXYZ() function must acquire this CS in order to send the whole packet at once.
	Automated via cPacketizer class. */
	cCriticalSection m_CSPacket;

	/** Buffer for composing the outgoing packets, through cPacketizer */
	cByteBuffer m_OutPacketBuffer;

	/** Buffer for composing packet length (so that each cPacketizer instance doesn't allocate a new cPacketBuffer) */
	cByteBuffer m_OutPacketLenBuffer;

	/** Returns the protocol-specific packet ID given the protocol-agnostic packet enum. */
	virtual UInt32 GetPacketID(ePacketType a_Packet) = 0;

	/** Returns the current protocol's version, for handling status requests. */
	virtual Version GetProtocolVersion() = 0;

	/** A generic data-sending routine, all outgoing packet data needs to be routed through this so that descendants may override it. */
	virtual void SendData(const char * a_Data, size_t a_Size) = 0;

	/** Sends a single packet contained within the cPacketizer class.
	The cPacketizer's destructor calls this to send the contained packet; protocol may transform the data (compression in 1.8 etc). */
	virtual void SendPacket(cPacketizer & a_Packet) = 0;
} ;
