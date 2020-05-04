
#pragma once

#include "../Defines.h"
#include "../Scoreboard.h"

// fwd:
class cClientHandle;
class cCompositeChat;
class cPlayer;
class cWorld;
enum class EffectID : Int32;



class cBroadcastInterface
{
public:
	virtual ~cBroadcastInterface() {}

	// Broadcast respective packets to all clients of the chunk where the event is taking place
	// (Please keep these alpha-sorted)
	virtual void BroadcastAttachEntity               (const cEntity & a_Entity, const cEntity & a_Vehicle) = 0;
	virtual void BroadcastBlockAction                (Vector3i a_BlockPos, Byte a_Byte1, Byte a_Byte2, BLOCKTYPE a_BlockType, const cClientHandle * a_Exclude = nullptr) = 0;
	virtual void BroadcastBlockBreakAnimation        (UInt32 a_EntityID, Vector3i a_BlockPos, Int8 a_Stage, const cClientHandle * a_Exclude = nullptr) = 0;
	virtual void BroadcastBlockEntity                (Vector3i a_BlockPos, const cClientHandle * a_Exclude = nullptr) = 0;
	virtual void BroadcastChat                       (const AString & a_Message, const cClientHandle * a_Exclude = nullptr, eMessageType a_ChatPrefix = mtCustom) = 0;
	virtual void BroadcastChatInfo                   (const AString & a_Message, const cClientHandle * a_Exclude = nullptr) = 0;
	virtual void BroadcastChatFailure                (const AString & a_Message, const cClientHandle * a_Exclude = nullptr) = 0;
	virtual void BroadcastChatSuccess                (const AString & a_Message, const cClientHandle * a_Exclude = nullptr) = 0;
	virtual void BroadcastChatWarning                (const AString & a_Message, const cClientHandle * a_Exclude = nullptr) = 0;
	virtual void BroadcastChatFatal                  (const AString & a_Message, const cClientHandle * a_Exclude = nullptr) = 0;
	virtual void BroadcastChatDeath                  (const AString & a_Message, const cClientHandle * a_Exclude = nullptr) = 0;
	virtual void BroadcastChat                       (const cCompositeChat & a_Message, const cClientHandle * a_Exclude = nullptr) = 0;
	virtual void BroadcastCollectEntity              (const cEntity & a_Collected, const cEntity & a_Collector, unsigned a_Count, const cClientHandle * a_Exclude = nullptr) = 0;
	virtual void BroadcastDestroyEntity              (const cEntity & a_Entity, const cClientHandle * a_Exclude = nullptr) = 0;
	virtual void BroadcastDetachEntity               (const cEntity & a_Entity, const cEntity & a_PreviousVehicle) = 0;
	virtual void BroadcastEntityEffect               (const cEntity & a_Entity, int a_EffectID, int a_Amplifier, int a_Duration, const cClientHandle * a_Exclude = nullptr) = 0;
	virtual void BroadcastEntityEquipment            (const cEntity & a_Entity, short a_SlotNum, const cItem & a_Item, const cClientHandle * a_Exclude = nullptr) = 0;
	virtual void BroadcastEntityHeadLook             (const cEntity & a_Entity, const cClientHandle * a_Exclude = nullptr) = 0;
	virtual void BroadcastEntityLook                 (const cEntity & a_Entity, const cClientHandle * a_Exclude = nullptr) = 0;
	virtual void BroadcastEntityMetadata             (const cEntity & a_Entity, const cClientHandle * a_Exclude = nullptr) = 0;
	virtual void BroadcastEntityPosition             (const cEntity & a_Entity, const cClientHandle * a_Exclude = nullptr) = 0;
	virtual void BroadcastEntityStatus               (const cEntity & a_Entity, Int8 a_Status, const cClientHandle * a_Exclude = nullptr) = 0;
	virtual void BroadcastEntityVelocity             (const cEntity & a_Entity, const cClientHandle * a_Exclude = nullptr) = 0;
	virtual void BroadcastEntityAnimation            (const cEntity & a_Entity, Int8 a_Animation, const cClientHandle * a_Exclude = nullptr) = 0;
	virtual void BroadcastLeashEntity                (const cEntity & a_Entity, const cEntity & a_EntityLeashedTo) = 0;
	virtual void BroadcastParticleEffect             (const AString & a_ParticleName, Vector3f a_Src, Vector3f a_Offset, float a_ParticleData, int a_ParticleAmount, const cClientHandle * a_Exclude = nullptr) = 0;
	virtual void BroadcastParticleEffect             (const AString & a_ParticleName, Vector3f a_Src, Vector3f a_Offset, float a_ParticleData, int a_ParticleAmount, std::array<int, 2> a_Data, const cClientHandle * a_Exclude = nullptr) = 0;
	virtual void BroadcastPlayerListAddPlayer        (const cPlayer & a_Player, const cClientHandle * a_Exclude = nullptr) = 0;
	virtual void BroadcastPlayerListRemovePlayer     (const cPlayer & a_Player, const cClientHandle * a_Exclude = nullptr) = 0;
	virtual void BroadcastPlayerListUpdateGameMode   (const cPlayer & a_Player, const cClientHandle * a_Exclude = nullptr) = 0;
	virtual void BroadcastPlayerListUpdatePing       (const cPlayer & a_Player, const cClientHandle * a_Exclude = nullptr) = 0;
	virtual void BroadcastPlayerListUpdateDisplayName(const cPlayer & a_Player, const AString & a_CustomName, const cClientHandle * a_Exclude = nullptr) = 0;
	virtual void BroadcastRemoveEntityEffect         (const cEntity & a_Entity, int a_EffectID, const cClientHandle * a_Exclude = nullptr) = 0;
	virtual void BroadcastScoreboardObjective        (const AString & a_Name, const AString & a_DisplayName, Byte a_Mode) = 0;
	virtual void BroadcastScoreUpdate                (const AString & a_Objective, const AString & a_PlayerName, cObjective::Score a_Score, Byte a_Mode) = 0;
	virtual void BroadcastDisplayObjective           (const AString & a_Objective, cScoreboard::eDisplaySlot a_Display) = 0;
	virtual void BroadcastSoundEffect                (const AString & a_SoundName, Vector3d a_Position, float a_Volume, float a_Pitch, const cClientHandle * a_Exclude = nullptr) = 0;
	virtual void BroadcastSoundParticleEffect        (const EffectID a_EffectID, Vector3i a_SrcPos, int a_Data, const cClientHandle * a_Exclude = nullptr) = 0;
	virtual void BroadcastSpawnEntity                (cEntity & a_Entity, const cClientHandle * a_Exclude = nullptr) = 0;
	virtual void BroadcastThunderbolt                (Vector3i a_BlockPos, const cClientHandle * a_Exclude = nullptr) = 0;
	virtual void BroadcastTimeUpdate                 (const cClientHandle * a_Exclude = nullptr) = 0;
	virtual void BroadcastUnleashEntity              (const cEntity & a_Entity) = 0;
	virtual void BroadcastUseBed                     (const cEntity & a_Entity, Vector3i a_BedPos) = 0;
	virtual void BroadcastWeather                    (eWeather a_Weather, const cClientHandle * a_Exclude = nullptr) = 0;
};
