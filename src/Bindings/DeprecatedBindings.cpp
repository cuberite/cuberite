
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "DeprecatedBindings.h"
#undef TOLUA_TEMPLATE_BIND
#include "tolua++/include/tolua++.h"

#include "Plugin.h"
#include "PluginLua.h"
#include "PluginManager.h"
#include "LuaWindow.h"
#include "LuaChunkStay.h"

#include "../BlockInfo.h"





/* get function: g_BlockLightValue */
#ifndef TOLUA_DISABLE_tolua_get_AllToLua_g_BlockLightValue
static int tolua_get_AllToLua_g_BlockLightValue(lua_State* tolua_S)
{
	int BlockType;
	#ifndef TOLUA_RELEASE
	{
		tolua_Error tolua_err;
		if (!tolua_isnumber(tolua_S, 2, 0, &tolua_err))
		{
			tolua_error(tolua_S, "#vinvalid type in array indexing.", &tolua_err);
		}
	}
	#endif
	BlockType = (int)tolua_tonumber(tolua_S, 2, 0);
	if ((BlockType < 0) || (BlockType > E_BLOCK_MAX_TYPE_ID))
	{
		tolua_error(tolua_S, "array indexing out of range.", NULL);
	}
	tolua_pushnumber(tolua_S, (lua_Number)cBlockInfo::GetLightValue((BLOCKTYPE)BlockType));
	return 1;
}
#endif  // #ifndef TOLUA_DISABLE





/* get function: g_BlockSpreadLightFalloff */
#ifndef TOLUA_DISABLE_tolua_get_AllToLua_g_BlockSpreadLightFalloff
static int tolua_get_AllToLua_g_BlockSpreadLightFalloff(lua_State* tolua_S)
{
	int BlockType;
	#ifndef TOLUA_RELEASE
	{
		tolua_Error tolua_err;
		if (!tolua_isnumber(tolua_S, 2, 0, &tolua_err))
			tolua_error(tolua_S, "#vinvalid type in array indexing.", &tolua_err);
	}
	#endif
	BlockType = (int)tolua_tonumber(tolua_S, 2, 0);
	if ((BlockType < 0) || (BlockType > E_BLOCK_MAX_TYPE_ID))
	{
		tolua_error(tolua_S, "array indexing out of range.", NULL);
	}
	tolua_pushnumber(tolua_S, (lua_Number)cBlockInfo::GetSpreadLightFalloff((BLOCKTYPE)BlockType));
	return 1;
}
#endif  // #ifndef TOLUA_DISABLE





/* get function: g_BlockTransparent */
#ifndef TOLUA_DISABLE_tolua_get_AllToLua_g_BlockTransparent
static int tolua_get_AllToLua_g_BlockTransparent(lua_State* tolua_S)
{
	int BlockType;
	#ifndef TOLUA_RELEASE
	{
		tolua_Error tolua_err;
		if (!tolua_isnumber(tolua_S, 2, 0, &tolua_err))
			tolua_error(tolua_S, "#vinvalid type in array indexing.", &tolua_err);
	}
	#endif
	BlockType = (int)tolua_tonumber(tolua_S, 2, 0);
	if ((BlockType < 0) || (BlockType > E_BLOCK_MAX_TYPE_ID))
	{
		tolua_error(tolua_S, "array indexing out of range.", NULL);
	}
	tolua_pushboolean(tolua_S, cBlockInfo::IsTransparent((BLOCKTYPE)BlockType));
	return 1;
}
#endif  // #ifndef TOLUA_DISABLE





/* get function: g_BlockOneHitDig */
#ifndef TOLUA_DISABLE_tolua_get_AllToLua_g_BlockOneHitDig
static int tolua_get_AllToLua_g_BlockOneHitDig(lua_State* tolua_S)
{
	int BlockType;
	#ifndef TOLUA_RELEASE
	{
		tolua_Error tolua_err;
		if (!tolua_isnumber(tolua_S, 2, 0, &tolua_err))
			tolua_error(tolua_S, "#vinvalid type in array indexing.", &tolua_err);
	}
	#endif
	BlockType = (int)tolua_tonumber(tolua_S, 2, 0);
	if ((BlockType < 0) || (BlockType > E_BLOCK_MAX_TYPE_ID))
	{
		tolua_error(tolua_S, "array indexing out of range.", NULL);
	}
	tolua_pushboolean(tolua_S, cBlockInfo::IsOneHitDig((BLOCKTYPE)BlockType));
	return 1;
}
#endif  // #ifndef TOLUA_DISABLE





/* get function: g_BlockPistonBreakable */
#ifndef TOLUA_DISABLE_tolua_get_AllToLua_g_BlockPistonBreakable
static int tolua_get_AllToLua_g_BlockPistonBreakable(lua_State* tolua_S)
{
	int BlockType;
	#ifndef TOLUA_RELEASE
	{
		tolua_Error tolua_err;
		if (!tolua_isnumber(tolua_S, 2, 0, &tolua_err))
			tolua_error(tolua_S, "#vinvalid type in array indexing.", &tolua_err);
	}
	#endif
	BlockType = (int)tolua_tonumber(tolua_S, 2, 0);
	if ((BlockType < 0) || (BlockType > E_BLOCK_MAX_TYPE_ID))
	{
		tolua_error(tolua_S, "array indexing out of range.", NULL);
	}
	tolua_pushboolean(tolua_S, cBlockInfo::IsPistonBreakable((BLOCKTYPE)BlockType));
	return 1;
}
#endif  // #ifndef TOLUA_DISABLE





/* get function: g_BlockIsSnowable */
#ifndef TOLUA_DISABLE_tolua_get_AllToLua_g_BlockIsSnowable
static int tolua_get_AllToLua_g_BlockIsSnowable(lua_State* tolua_S)
{
	int BlockType;
	#ifndef TOLUA_RELEASE
	{
		tolua_Error tolua_err;
		if (!tolua_isnumber(tolua_S, 2, 0, &tolua_err))
			tolua_error(tolua_S, "#vinvalid type in array indexing.", &tolua_err);
	}
	#endif
	BlockType = (int)tolua_tonumber(tolua_S, 2, 0);
	if ((BlockType < 0) || (BlockType > E_BLOCK_MAX_TYPE_ID))
	{
		tolua_error(tolua_S, "array indexing out of range.", NULL);
	}
	tolua_pushboolean(tolua_S, cBlockInfo::IsSnowable((BLOCKTYPE)BlockType));
	return 1;
}
#endif  // #ifndef TOLUA_DISABLE





/* get function: g_BlockIsSolid */
#ifndef TOLUA_DISABLE_tolua_get_AllToLua_g_BlockIsSolid
static int tolua_get_AllToLua_g_BlockIsSolid(lua_State* tolua_S)
{
	int BlockType;
	#ifndef TOLUA_RELEASE
	{
		tolua_Error tolua_err;
		if (!tolua_isnumber(tolua_S, 2, 0, &tolua_err))
			tolua_error(tolua_S, "#vinvalid type in array indexing.", &tolua_err);
	}
	#endif
	BlockType = (int)tolua_tonumber(tolua_S, 2, 0);
	if ((BlockType < 0) || (BlockType > E_BLOCK_MAX_TYPE_ID))
	{
		tolua_error(tolua_S, "array indexing out of range.", NULL);
	}
	tolua_pushboolean(tolua_S, (bool)cBlockInfo::IsSolid((BLOCKTYPE)BlockType));
	return 1;
}
#endif  // #ifndef TOLUA_DISABLE





/* get function: g_BlockFullyOccupiesVoxel */
#ifndef TOLUA_DISABLE_tolua_get_AllToLua_g_BlockFullyOccupiesVoxel
static int tolua_get_AllToLua_g_BlockFullyOccupiesVoxel(lua_State* tolua_S)
{
	int BlockType;
	#ifndef TOLUA_RELEASE
	{
		tolua_Error tolua_err;
		if (!tolua_isnumber(tolua_S, 2, 0, &tolua_err))
			tolua_error(tolua_S, "#vinvalid type in array indexing.", &tolua_err);
	}
	#endif
	BlockType = (int)tolua_tonumber(tolua_S, 2, 0);
	if ((BlockType < 0) || (BlockType > E_BLOCK_MAX_TYPE_ID))
	{
		tolua_error(tolua_S, "array indexing out of range.", NULL);
	}
	tolua_pushboolean(tolua_S, (bool)cBlockInfo::FullyOccupiesVoxel((BLOCKTYPE)BlockType));
	return 1;
}
#endif  // #ifndef TOLUA_DISABLE





void DeprecatedBindings::Bind(lua_State * tolua_S)
{
	tolua_beginmodule(tolua_S, NULL);

	tolua_array(tolua_S, "g_BlockLightValue",          tolua_get_AllToLua_g_BlockLightValue,          NULL);
	tolua_array(tolua_S, "g_BlockSpreadLightFalloff",  tolua_get_AllToLua_g_BlockSpreadLightFalloff,  NULL);
	tolua_array(tolua_S, "g_BlockTransparent",         tolua_get_AllToLua_g_BlockTransparent,         NULL);
	tolua_array(tolua_S, "g_BlockOneHitDig",           tolua_get_AllToLua_g_BlockOneHitDig,           NULL);
	tolua_array(tolua_S, "g_BlockPistonBreakable",     tolua_get_AllToLua_g_BlockPistonBreakable,     NULL);
	tolua_array(tolua_S, "g_BlockIsSnowable",          tolua_get_AllToLua_g_BlockIsSnowable,          NULL);
	tolua_array(tolua_S, "g_BlockIsSolid",             tolua_get_AllToLua_g_BlockIsSolid,             NULL);
	tolua_array(tolua_S, "g_BlockFullyOccupiesVoxel",  tolua_get_AllToLua_g_BlockFullyOccupiesVoxel,  NULL);

	tolua_endmodule(tolua_S);
}




