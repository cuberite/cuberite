
#pragma once

#include "Defines.h"
#include "Scoreboard.h"

//fwd:
class cClientHandle;
class cCompositeChat;
class cPlayer;
class cWorld;
enum class EffectID : Int32;

class cBroadcaster
{
public:

	cBroadcaster(cWorld * a_World);

	// Broadcast respective packets to all clients of the chunk where the event is taking place
	// (Please keep these alpha-sorted)
	void BroadcastAttachEntity               (const cEntity & a_Entity, const cEntity & a_Vehicle);
	void BroadcastBlockAction                (Vector3i a_BlockPos, Byte a_Byte1, Byte a_Byte2, BLOCKTYPE a_BlockType, const cClientHandle * a_Exclude = nullptr);  // tolua_export
	void BroadcastBlockBreakAnimation        (UInt32 a_EntityID, Vector3i a_BlockPos, char a_Stage, const cClientHandle * a_Exclude = nullptr);
	void BroadcastBlockEntity                (Vector3i a_BlockPos, const cClientHandle * a_Exclude = nullptr);  ///< If there is a block entity at the specified coods, sends it to all clients except a_Exclude
	void BroadcastChat                       (const AString & a_Message, const cClientHandle * a_Exclude = nullptr, eMessageType a_ChatPrefix = mtCustom);
	void BroadcastChatInfo                   (const AString & a_Message, const cClientHandle * a_Exclude = nullptr) { BroadcastChat(a_Message, a_Exclude, mtInformation); }
	void BroadcastChatFailure                (const AString & a_Message, const cClientHandle * a_Exclude = nullptr) { BroadcastChat(a_Message, a_Exclude, mtFailure); }
	void BroadcastChatSuccess                (const AString & a_Message, const cClientHandle * a_Exclude = nullptr) { BroadcastChat(a_Message, a_Exclude, mtSuccess); }
	void BroadcastChatWarning                (const AString & a_Message, const cClientHandle * a_Exclude = nullptr) { BroadcastChat(a_Message, a_Exclude, mtWarning); }
	void BroadcastChatFatal                  (const AString & a_Message, const cClientHandle * a_Exclude = nullptr) { BroadcastChat(a_Message, a_Exclude, mtFailure); }
	void BroadcastChatDeath                  (const AString & a_Message, const cClientHandle * a_Exclude = nullptr) { BroadcastChat(a_Message, a_Exclude, mtDeath); }
	void BroadcastChat                       (const cCompositeChat & a_Message, const cClientHandle * a_Exclude = nullptr);
	void BroadcastCollectEntity              (const cEntity & a_Pickup, const cPlayer & a_Player, int a_Count, const cClientHandle * a_Exclude = nullptr);
	void BroadcastDestroyEntity              (const cEntity & a_Entity, const cClientHandle * a_Exclude = nullptr);
	void BroadcastDetachEntity               (const cEntity & a_Entity, const cEntity & a_PreviousVehicle);
	void BroadcastEntityEffect               (const cEntity & a_Entity, int a_EffectID, int a_Amplifier, short a_Duration, const cClientHandle * a_Exclude = nullptr);
	void BroadcastEntityEquipment            (const cEntity & a_Entity, short a_SlotNum, const cItem & a_Item, const cClientHandle * a_Exclude = nullptr);
	void BroadcastEntityHeadLook             (const cEntity & a_Entity, const cClientHandle * a_Exclude = nullptr);
	void BroadcastEntityLook                 (const cEntity & a_Entity, const cClientHandle * a_Exclude = nullptr);
	void BroadcastEntityMetadata             (const cEntity & a_Entity, const cClientHandle * a_Exclude = nullptr);
	void BroadcastEntityRelMove              (const cEntity & a_Entity, Vector3<char> a_RelMove, const cClientHandle * a_Exclude = nullptr);
	void BroadcastEntityRelMoveLook          (const cEntity & a_Entity, Vector3<char> a_RelMove, const cClientHandle * a_Exclude = nullptr);
	void BroadcastEntityStatus               (const cEntity & a_Entity, char a_Status, const cClientHandle * a_Exclude = nullptr);
	void BroadcastEntityVelocity             (const cEntity & a_Entity, const cClientHandle * a_Exclude = nullptr);
	void BroadcastEntityAnimation            (const cEntity & a_Entity, char a_Animation, const cClientHandle * a_Exclude = nullptr);
	void BroadcastLeashEntity                (const cEntity & a_Entity, const cEntity & a_EntityLeashedTo);
	void BroadcastParticleEffect             (const AString & a_ParticleName, const Vector3f a_Src, const Vector3f a_Offset, float a_ParticleData, int a_ParticleAmount, cClientHandle * a_Exclude = nullptr);
	void BroadcastParticleEffect             (const AString & a_ParticleName, const Vector3f a_Src, const Vector3f a_Offset, float a_ParticleData, int a_ParticleAmount, std::array<int, 2> a_Data, cClientHandle * a_Exclude = nullptr);
	void BroadcastPlayerListAddPlayer        (const cPlayer & a_Player, const cClientHandle * a_Exclude = nullptr);
	void BroadcastPlayerListRemovePlayer     (const cPlayer & a_Player, const cClientHandle * a_Exclude = nullptr);
	void BroadcastPlayerListUpdateGameMode   (const cPlayer & a_Player, const cClientHandle * a_Exclude = nullptr);
	void BroadcastPlayerListUpdatePing       (const cPlayer & a_Player, const cClientHandle * a_Exclude = nullptr);
	void BroadcastPlayerListUpdateDisplayName(const cPlayer & a_Player, const AString & a_CustomName, const cClientHandle * a_Exclude = nullptr);
	void BroadcastRemoveEntityEffect         (const cEntity & a_Entity, int a_EffectID, const cClientHandle * a_Exclude = nullptr);
	void BroadcastScoreboardObjective        (const AString & a_Name, const AString & a_DisplayName, Byte a_Mode);
	void BroadcastScoreUpdate                (const AString & a_Objective, const AString & a_PlayerName, cObjective::Score a_Score, Byte a_Mode);
	void BroadcastDisplayObjective           (const AString & a_Objective, cScoreboard::eDisplaySlot a_Display);
	void BroadcastSoundEffect                (const AString & a_SoundName, Vector3d a_Position, float a_Volume, float a_Pitch, const cClientHandle * a_Exclude = nullptr);
	void BroadcastSoundParticleEffect        (const EffectID a_EffectID, Vector3i a_SrcPos, int a_Data, const cClientHandle * a_Exclude = nullptr);
	void BroadcastSpawnEntity                (cEntity & a_Entity, const cClientHandle * a_Exclude = nullptr);
	void BroadcastTeleportEntity             (const cEntity & a_Entity, const cClientHandle * a_Exclude = nullptr);
	void BroadcastThunderbolt                (Vector3i a_BlockPos, const cClientHandle * a_Exclude = nullptr);
	void BroadcastTimeUpdate                 (const cClientHandle * a_Exclude = nullptr);
	void BroadcastUnleashEntity              (const cEntity & a_Entity);
	void BroadcastUseBed                     (const cEntity & a_Entity, Vector3i a_BedPos);
	void BroadcastWeather                    (eWeather a_Weather, const cClientHandle * a_Exclude = nullptr);

private:
	cWorld * m_World;

};
