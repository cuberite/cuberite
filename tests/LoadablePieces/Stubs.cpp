
// Stubs.cpp

// Implements stubs of various MCServer methods that are needed for linking but not for runtime
// This is required so that we don't bring in the entire MCServer via dependencies

#include "Globals.h"
#include "BlockInfo.h"
#include "SelfTests.h"
#include "Bindings.h"
#include "Bindings/DeprecatedBindings.h"
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





cBlockInfo::~cBlockInfo()
{
}





void cBlockInfo::Initialize(cBlockInfo::cBlockInfoArray & a_BlockInfos)
{
	// The piece-loading code uses the handlers for rotations, so we need valid handlers
	// Insert dummy handlers:
	for (size_t i = 0; i < ARRAYCOUNT(a_BlockInfos); i++)
	{
		a_BlockInfos[i].m_Handler = new cBlockHandler(static_cast<BLOCKTYPE>(i));
	}
}





cBlockHandler::cBlockHandler(BLOCKTYPE a_BlockType)
{
}





bool cBlockHandler::GetPlacementBlockTypeMeta(
	cChunkInterface & a_ChunkInterface, cPlayer * a_Player,
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





void cBlockHandler::OnPlacedByPlayer(cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface, cPlayer * a_Player, const sSetBlock & a_BlockChange)
{
}





void cBlockHandler::OnDestroyedByPlayer(cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface, cPlayer * a_Player, int a_BlockX, int a_BlockY, int a_BlockZ)
{
}





void cBlockHandler::OnPlaced(cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface, int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta)
{
}





void cBlockHandler::OnDestroyed(cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface, int a_BlockX, int a_BlockY, int a_BlockZ)
{
}





void cBlockHandler::NeighborChanged(cChunkInterface & a_ChunkInterface, int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_WhichNeighbor)
{
}





void cBlockHandler::ConvertToPickups(cItems & a_Pickups, NIBBLETYPE a_BlockMeta)
{
}





void cBlockHandler::DropBlock(cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface, cBlockPluginInterface & a_BlockPluginInterface, cEntity * a_Digger, int a_BlockX, int a_BlockY, int a_BlockZ, bool a_CanDrop)
{
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





bool cBlockHandler::DoesIgnoreBuildCollision(void)
{
	return (m_BlockType == E_BLOCK_AIR);
}





bool cBlockHandler::DoesDropOnUnsuitable(void)
{
	return true;
}





void cBlockHandler::Check(cChunkInterface & a_ChunkInterface, cBlockPluginInterface & a_PluginInterface, int a_RelX, int a_RelY, int a_RelZ, cChunk & a_Chunk)
{
}





ColourID cBlockHandler::GetMapBaseColourID(NIBBLETYPE a_Meta)
{
	return 0;
}





cBlockEntity * cBlockEntity::CreateByBlockType(BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, int a_BlockX, int a_BlockY, int a_BlockZ, cWorld * a_World)
{
	return nullptr;
}





cSelfTests::cSelfTests(void):
	m_AllowRegistering(true)
{
}





cSelfTests & cSelfTests::Get(void)
{
	static cSelfTests singleton;
	return singleton;
}





void cSelfTests::Register(cSelfTests::SelfTestFunction a_TestFn, const AString & a_TestName)
{
}




