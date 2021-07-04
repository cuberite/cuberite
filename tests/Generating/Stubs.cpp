
// Stubs.cpp

// Implements stubs of various Cuberite methods that are needed for linking but not for runtime
// This is required so that we don't bring in the entire Cuberite via dependencies

#include "Globals.h"
#include "BlockInfo.h"
#include "UUID.h"
#include "Bindings.h"
#include "Bindings/DeprecatedBindings.h"
#include "Bindings/LuaJson.h"
#include "Bindings/LuaState.h"
#include "Bindings/ManualBindings.h"
#include "BlockEntities/BlockEntity.h"
#include "Blocks/BlockHandler.h"
#include "Generating/ChunkDesc.h"
#include "DeadlockDetect.h"
#include "Entities/Entity.h"
#include "Entities/EnderCrystal.h"
#include "Mobs/Monster.h"
#include "Mobs/EnderDragon.h"
#include "Simulator/FluidSimulator.h"
#include "Simulator/FireSimulator.h"
#include "MobSpawner.h"
#include "ItemGrid.h"





// fwd:
struct lua_State;





// Prototypes, needed by clang:
extern "C" int luaopen_lsqlite3(lua_State * a_LuaState);
extern "C" int luaopen_lxp(lua_State * a_LuaState);





void cManualBindings::Bind(lua_State * a_LuaState)
{
}





void DeprecatedBindings::Bind(lua_State * a_LuaState)
{
}





void cLuaJson::Bind(cLuaState & a_LuaState)
{
}





int tolua_AllToLua_open(lua_State * a_LuaState)
{
	return 0;
}





extern "C" int luaopen_lsqlite3(lua_State * a_LuaState)
{
	return 0;
}





extern "C" int luaopen_lxp(lua_State * a_LuaState)
{
	return 0;
}





cBoundingBox::cBoundingBox(double, double, double, double, double, double)
{
}





cBoundingBox cBlockHandler::GetPlacementCollisionBox(BlockState a_XM, BlockState a_XP, BlockState a_YM, BlockState a_YP, BlockState a_ZM, BlockState a_ZP) const
{
	return cBoundingBox(0, 0, 0, 0, 0, 0);
}





void cBlockHandler::OnUpdate(cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface, cBlockPluginInterface & a_PluginInterface, cChunk & a_Chunk, const Vector3i a_RelPos) const
{
}





void cBlockHandler::OnNeighborChanged(cChunkInterface & a_ChunkInterface, Vector3i a_BlockPos, eBlockFace a_WhichNeighbor) const
{
}





void cBlockHandler::NeighborChanged(cChunkInterface & a_ChunkInterface, Vector3i a_BlockPos, eBlockFace a_WhichNeighbor)
{
}





cItems cBlockHandler::ConvertToPickups(BlockState a_Block, const cItem * a_Tool) const
{
	return cItems();
}





bool cBlockHandler::CanBeAt(const cChunk & a_Chunk, Vector3i a_Position, BlockState a_Self) const
{
	return true;
}





bool cBlockHandler::IsUseable() const
{
	return false;
}





bool cBlockHandler::DoesIgnoreBuildCollision(const cWorld & a_World, const cItem & a_HeldItem, Vector3i a_Position, BlockState a_ClickedBlock, eBlockFace a_ClickedBlockFace, bool a_ClickedDirectly) const
{
	return m_BlockType == BlockType::Air;
}





void cBlockHandler::Check(cChunkInterface & a_ChunkInterface, cBlockPluginInterface & a_PluginInterface, Vector3i a_RelPos, cChunk & a_Chunk) const
{
}





ColourID cBlockHandler::GetMapBaseColourID() const
{
	return 0;
}





bool cBlockHandler::IsInsideBlock(const Vector3d a_RelPosition, const BlockState a_Block) const
{
	return true;
}





const cBlockHandler & cBlockHandler::For(BlockType a_BlockType)
{
	// Dummy handler.
	static cBlockHandler Handler(BlockType::Air);
	return Handler;
}





OwnedBlockEntity cBlockEntity::CreateByBlockType(BlockState a_Block, Vector3i a_Pos, cWorld * a_World)
{
	return nullptr;
}





void cDeadlockDetect::TrackCriticalSection(cCriticalSection & a_CS, const AString & a_Name)
{
}





void cDeadlockDetect::UntrackCriticalSection(cCriticalSection & a_CS)
{
}





void cBlockEntity::SetPos(Vector3i a_NewPos)
{
}





bool cBlockEntity::IsBlockEntityBlockType(BlockState a_Block)
{
	return false;
}





OwnedBlockEntity cBlockEntity::Clone(Vector3i a_Pos)
{
	return nullptr;
}





bool cLuaState::GetStackValue(int, cUUID *&)
{
	return false;
}





bool cUUID::FromString(const AString&)
{
	return false;
}





void cEntity::SetPosition(const Vector3d & a_Position)
{
}





void cEntity::SetHealth(float a_NewHealth)
{
}





cMonster::eFamily cMonster::FamilyFromType(eMonsterType a_Type)
{
	return cMonster::mfAmbient;
}





std::unique_ptr<cMonster> cMonster::NewMonsterFromType(eMonsterType a_Type)
{
	return nullptr;
}





void cMonster::CheckEventLostPlayer(std::chrono::milliseconds a_Dt)
{

}





bool cFluidSimulator::CanWashAway(BlockState a_BlockType)
{
	return false;
}





bool cFireSimulator::DoesBurnForever(BlockState a_BlockType)
{
	return false;
}





void cItemGrid::GenerateRandomLootWithBooks(const cLootProbab * a_LootProbabs, size_t a_CountLootProbabs, int a_NumSlots, int a_Seed)
{
}





std::set<eMonsterType> cMobSpawner::GetAllowedMobTypes(EMCSBiome a_Biome)
{
	return {};
}





cItem::cItem()
{
}





cItem::cItem(
		enum Item a_ItemType,
		char a_ItemCount,
		short a_ItemDamage,
		const AString & a_Enchantments,
		const AString & a_CustomName,
		const AStringVector & a_LoreTable
)
{
}





void cItem::Empty()
{
}





cEnderCrystal::cEnderCrystal(Vector3d a_Pos, bool a_ShowBottom):
	Super(etEnderCrystal, a_Pos, 1.0, 1.0)
{
}





void cEnderCrystal::SpawnOn(class cClientHandle & a_ClientHandle)
{
}






void cEnderCrystal::Tick(std::chrono::milliseconds a_Dt, class cChunk & a_Chunk)
{
}






void cEnderCrystal::KilledBy(struct TakeDamageInfo & a_TakeDamageInfo)
{
}





cEntity::cEntity(enum cEntity::eEntityType a_EntityType, class Vector3<double> a_Pos, float a_Height, float a_Width)
{
}





bool cEntity::Initialize(class std::unique_ptr<class cEntity,struct std::default_delete<class cEntity> > a_Entity,class cWorld & a_World)
{
	return true;
}





void cEntity::OnAddToWorld(class cWorld & a_World)
{
}





void cEntity::OnRemoveFromWorld(class cWorld & a_World)
{
}





bool cEntity::IsA(char const * a_Type)const
{
	return true;
}





char const * cEntity::GetParentClass(void) const
{
	return "";
}





void cEntity::HandleSpeedFromAttachee(float a_SpeedX,float a_SpeedZ)
{
}






void cEntity::Destroy(void)
{
}






bool cEntity::DoTakeDamage(struct TakeDamageInfo & a_TakeDamageInfo)
{
	return true;
}






int cEntity::GetRawDamageAgainst(class cEntity const & a_Entity)
{
	return 0;
}






bool cEntity::ArmorCoversAgainst(enum eDamageType a_DamageType)
{
	return true;
}






float cEntity::GetArmorCoverAgainst(class cEntity const * a_Entity,enum eDamageType a_DamageType,int a_Value)
{
	return 0.0f;
}





char const * cEntity::GetClass(void) const
{
	return "";
}





float cEntity::GetEnchantmentCoverAgainst(class cEntity const * a_Entity,enum eDamageType a_DamageType,int a_Value)
{
	return 0.0f;
}






float cEntity::GetEnchantmentBlastKnockbackReduction(void)
{
	return 0.0f;
}






double cEntity::GetKnockbackAmountAgainst(class cEntity const & a_Entity)
{
	return 0.0f;
}






void cEntity::ApplyArmorDamage(int)
{
}






void cEntity::KilledBy(struct TakeDamageInfo & a_TakeDamageInfo)
{
}






void cEntity::Heal(int a_Value)
{
}






void cEntity::Tick(std::chrono::milliseconds a_Dt, class cChunk & a_Chunk)
{
}






void cEntity::HandlePhysics(std::chrono::milliseconds a_Dt, class cChunk & a_Chunk)
{
}






void cEntity::TickBurning(class cChunk & a_Chunk)
{
}






void cEntity::DetectCacti(void)
{
}






bool cEntity::DetectPortal(void)
{
	return true;
}






void cEntity::TickInVoid(class cChunk & a_Chunk)
{
}






void cEntity::OnStartedBurning(void)
{
}






void cEntity::OnFinishedBurning(void)
{
}






void cEntity::TeleportToEntity(class cEntity & a_Entity)
{
}






void cEntity::TeleportToCoords(double a_XPos, double a_YPos, double a_ZPos)
{
}






void cEntity::BroadcastMovementUpdate(class cClientHandle const * a_ClientHandle)
{
}






void cEntity::AttachTo(class cEntity & a_Entity)
{
}






void cEntity::Detach(void)
{
}






void cEntity::SetSpeed(double a_XSpeed, double a_YSpeed, double a_ZSpeed)
{
}





void cEntity::DetectMagma(void)
{
}





bool cEntity::MoveToWorld(cWorld & a_World, Vector3d a_NewPosition, bool a_SetPortalCooldown, bool a_ShouldSendRespawn)
{
    return false;
}






void cEntity::HandleAir(void)
{
}






void cEntity::SetSwimState(class cChunk & a_Chunk)
{
}






void cEntity::ResetPosition(class Vector3<double> a_Pos)
{
}





void cEntity::OnDetach()
{

}





cPawn::cPawn(enum cEntity::eEntityType, float a_Width, float a_Height) :
	cEntity(etMonster, Vector3d(), a_Height, a_Width)
{
}





bool cPawn::IsFireproof(void) const
{
	return true;
}





bool cPawn::IsInvisible(void) const
{
	return true;
}





void cPawn::HandleAir(void)
{
}




void cPawn::HandleFalling()
{
}





void cPawn::KilledBy(struct TakeDamageInfo & a_TakeDamageInfo)
{
}



void cPawn::OnRemoveFromWorld(cWorld & a_World)
{
}





void cPawn::ResetPosition(class Vector3<double>)
{
}





void cPawn::Tick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk)
{
}





cMonster::cMonster(const AString & a_StringA, enum eMonsterType a_MonsterType, const AString & a_StringB, const AString & a_StringC, const AString & a_StringD, float a_Width, float a_Height) :
	cPawn(etMonster, a_Width, a_Height),
	m_PathFinder(a_Width, a_Height)
{
}



cPathFinder::cPathFinder(float a_Width, float a_Height)
{

}





void cMonster::OnRemoveFromWorld(class cWorld & a_World)
{
}






void cMonster::Tick(std::chrono::milliseconds a_Dt, class cChunk & a_Chunk)
{
}





void cMonster::EventSeePlayer(cPlayer * a_Player, cChunk & a_Chunk)
{
}





void cMonster::InStateChasing(std::chrono::milliseconds a_Dt, class cChunk & a_Chunk)
{
}





void cMonster::SpawnOn(class cClientHandle & a_ClientHandle)
{
}





bool cMonster::DoTakeDamage(struct TakeDamageInfo & a_TakeDamageInfo)
{
	return true;
}





void cMonster::KilledBy(struct TakeDamageInfo & a_TakeDamageInfo)
{
}





void cMonster::OnRightClicked(class cPlayer & a_Player)
{
}





void cMonster::HandleFalling(void)
{
}





void cMonster::MoveToPosition(class Vector3<double> const & a_Pos)
{
}





void cMonster::CheckEventSeePlayer(class cChunk & a_Chunk)
{
}





bool cMonster::IsNetherNative(void)
{
	return false;
}





bool cMonster::IsUndead(void)
{
	return false;
}





void cMonster::EventLosePlayer(void)
{
}





void cMonster::InStateIdle(std::chrono::milliseconds a_Dt ,class cChunk & a_Chunk)
{
}





void cMonster::InStateEscaping(std::chrono::milliseconds a_Dt ,class cChunk & a_Chunk)
{
}





cAggressiveMonster::cAggressiveMonster(const AString & a_StringA, enum eMonsterType a_MonsterType, const AString & a_StringB, const AString & a_StringC, const AString & a_StringD, float a_Width, float a_Height) :
	cMonster(a_StringA, a_MonsterType, a_StringB, a_StringC, a_StringD, a_Width, a_Height)
{
}





void cAggressiveMonster::Tick(std::chrono::milliseconds a_Dt, class cChunk & a_Chunk)
{
}





void cAggressiveMonster::InStateChasing(std::chrono::milliseconds a_Dt, class cChunk & a_Chunk)
{
}






void cAggressiveMonster::EventSeePlayer(class cPlayer *,class cChunk &)
{
}





bool cAggressiveMonster::Attack(std::chrono::milliseconds a_Dt)
{
	return true;
}
