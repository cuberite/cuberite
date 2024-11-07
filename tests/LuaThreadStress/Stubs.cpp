
// Stubs.cpp

// Implements stubs of various Cuberite methods that are needed for linking but not for runtime
// This is required so that we don't bring in the entire Cuberite via dependencies

#include "Globals.h"
#include "BlockInfo.h"
#include "Bindings.h"
#include "DeadlockDetect.h"
#include "UUID.h"
#include "Bindings/DeprecatedBindings.h"
#include "Bindings/LuaJson.h"
#include "Bindings/ManualBindings.h"
#include "BlockEntities/BlockEntity.h"
#include "Blocks/BlockHandler.h"
#include "Generating/ChunkDesc.h"





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





bool cBlockInfo::IsSolid(BLOCKTYPE)
{
	return false;
}





cBoundingBox::cBoundingBox(double, double, double, double, double, double)
{
}





cBoundingBox cBlockHandler::GetPlacementCollisionBox(BLOCKTYPE a_XM, BLOCKTYPE a_XP, BLOCKTYPE a_YM, BLOCKTYPE a_YP, BLOCKTYPE a_ZM, BLOCKTYPE a_ZP) const
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





cItems cBlockHandler::ConvertToPickups(const NIBBLETYPE a_BlockMeta, const cItem * const a_Tool) const
{
	return cItems();
}





bool cBlockHandler::CanBeAt(const cChunk & a_Chunk, const Vector3i a_Position, const NIBBLETYPE a_Meta) const
{
	return true;
}





bool cBlockHandler::IsUseable() const
{
	return false;
}





bool cBlockHandler::DoesIgnoreBuildCollision(const cWorld & a_World, const cItem & a_HeldItem, Vector3i a_Position, NIBBLETYPE a_Meta, eBlockFace a_ClickedBlockFace, bool a_ClickedDirectly) const
{
	return m_BlockType == E_BLOCK_AIR;
}





void cBlockHandler::Check(cChunkInterface & a_ChunkInterface, cBlockPluginInterface & a_PluginInterface, Vector3i a_RelPos, cChunk & a_Chunk) const
{
}





ColourID cBlockHandler::GetMapBaseColourID(NIBBLETYPE a_Meta) const
{
	return 0;
}





bool cBlockHandler::IsInsideBlock(Vector3d a_Position, const NIBBLETYPE a_BlockMeta) const
{
	return true;
}





const cBlockHandler & cBlockHandler::For(BLOCKTYPE a_BlockType)
{
	// Dummy handler.
	static cBlockHandler Handler(E_BLOCK_AIR);
	return Handler;
}





OwnedBlockEntity cBlockEntity::CreateByBlockType(BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, Vector3i a_Pos, cWorld * a_World)
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





OwnedBlockEntity cBlockEntity::Clone(Vector3i a_Pos)
{
	return nullptr;
}





bool cLuaState::GetStackValue(int, cUUID *&)
{
	return false;
}





bool cUUID::FromString(const AString &)
{
	return true;
}





int BlockStringToType(const AString &)
{
	return 0;
}
