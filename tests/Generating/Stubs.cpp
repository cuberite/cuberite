
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
#include "Mobs/Monster.h"
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





void cBlockInfo::sHandlerDeleter::operator () (cBlockHandler * a_Handler)
{
	delete a_Handler;
}





cBlockInfo::cBlockInfoArray::cBlockInfoArray()
{
	cBlockInfoArray & BlockInfos = *this;
	// The piece-loading code uses the handlers for rotations, so we need valid handlers
	// Insert dummy handlers:
	for (size_t i = 0; i < BlockInfos.size(); i++)
	{
		BlockInfos[i].m_Handler.reset(new cBlockHandler(static_cast<BLOCKTYPE>(i)));
	}
}





cBoundingBox::cBoundingBox(double, double, double, double, double, double)
{
}





cBlockHandler::cBlockHandler(BLOCKTYPE a_BlockType)
{
}





cBoundingBox cBlockHandler::GetPlacementCollisionBox(BLOCKTYPE a_XM, BLOCKTYPE a_XP, BLOCKTYPE a_YM, BLOCKTYPE a_YP, BLOCKTYPE a_ZM, BLOCKTYPE a_ZP)
{
	return cBoundingBox(0, 0, 0, 0, 0, 0);
}





bool cBlockHandler::GetPlacementBlockTypeMeta(
	cChunkInterface & a_ChunkInterface, cPlayer & a_Player,
	int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace,
	int a_CursorX, int a_CursorY, int a_CursorZ,
	BLOCKTYPE & a_BlockType, NIBBLETYPE & a_BlockMeta
)
{
	return true;
}





void cBlockHandler::OnUpdate(cChunkInterface & cChunkInterface, cWorldInterface & a_WorldInterface, cBlockPluginInterface & a_PluginInterface, cChunk & a_Chunk, int a_BlockX, int a_BlockY, int a_BlockZ)
{
}





void cBlockHandler::OnPlacedByPlayer(cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface, cPlayer & a_Player, const sSetBlock & a_BlockChange)
{
}





void cBlockHandler::OnPlaced(cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface, Vector3i a_BlockPos, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta)
{
}





void cBlockHandler::OnBroken(cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface, Vector3i a_BlockPos, BLOCKTYPE a_OldBlockType, NIBBLETYPE a_OldBlockMeta)
{
}





void cBlockHandler::NeighborChanged(cChunkInterface & a_ChunkInterface, Vector3i a_BlockPos, eBlockFace a_WhichNeighbor)
{
}





cItems cBlockHandler::ConvertToPickups(NIBBLETYPE a_BlockMeta, cBlockEntity * a_BlockEntity, const cEntity * a_Digger, const cItem * a_Tool)
{
	return cItems();
}





bool cBlockHandler::CanBeAt(cChunkInterface & a_ChunkInterface, int a_BlockX, int a_BlockY, int a_BlockZ, const cChunk & a_Chunk)
{
	return true;
}





bool cBlockHandler::IsUseable()
{
	return false;
}





bool cBlockHandler::IsClickedThrough(void)
{
	return false;
}





bool cBlockHandler::DoesIgnoreBuildCollision(cChunkInterface & a_ChunkInterface, Vector3i a_Pos, cPlayer & a_Player, NIBBLETYPE a_Meta)
{
	return (m_BlockType == E_BLOCK_AIR);
}





bool cBlockHandler::DoesDropOnUnsuitable(void)
{
	return true;
}





void cBlockHandler::Check(cChunkInterface & a_ChunkInterface, cBlockPluginInterface & a_PluginInterface, Vector3i a_RelPos, cChunk & a_Chunk)
{
}





ColourID cBlockHandler::GetMapBaseColourID(NIBBLETYPE a_Meta)
{
	return 0;
}





bool cBlockHandler::IsInsideBlock(Vector3d a_Position, const BLOCKTYPE a_BlockType, const NIBBLETYPE a_BlockMeta)
{
	return true;
}





cBlockEntity * cBlockEntity::CreateByBlockType(BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, Vector3i a_Pos, cWorld * a_World)
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





bool cBlockEntity::IsBlockEntityBlockType(BLOCKTYPE a_BlockType)
{
	return false;
}





cBlockEntity * cBlockEntity::Clone(Vector3i a_Pos)
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





bool cFluidSimulator::CanWashAway(BLOCKTYPE a_BlockType)
{
	return false;
}





bool cFireSimulator::DoesBurnForever(BLOCKTYPE a_BlockType)
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
