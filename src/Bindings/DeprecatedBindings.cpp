
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "DeprecatedBindings.h"
#undef TOLUA_TEMPLATE_BIND
#include "tolua++/include/tolua++.h"


#include "../BlockInfo.h"
#include "../World.h"
#include "../Entities/Player.h"





static void lua_do_warning(lua_State* L, const char * a_pFormat, ...)
{
	// Retrieve current function name
	lua_Debug entry;
	VERIFY(lua_getstack(L, 0, &entry));
	VERIFY(lua_getinfo(L, "n", &entry));

	// Insert function name into error msg
	AString msg(a_pFormat);
	ReplaceString(msg, "#funcname#", entry.name?entry.name:"?");

	// Copied from luaL_error and modified
	va_list argp;
	va_start(argp, a_pFormat);
	luaL_where(L, 1);
	lua_pushvfstring(L, msg.c_str(), argp);
	va_end(argp);
	lua_concat(L, 2);
	lua_error(L);
}





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
		tolua_error(tolua_S, "array indexing out of range.", nullptr);
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
		tolua_error(tolua_S, "array indexing out of range.", nullptr);
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
		tolua_error(tolua_S, "array indexing out of range.", nullptr);
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
		tolua_error(tolua_S, "array indexing out of range.", nullptr);
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
		tolua_error(tolua_S, "array indexing out of range.", nullptr);
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
		tolua_error(tolua_S, "array indexing out of range.", nullptr);
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
		tolua_error(tolua_S, "array indexing out of range.", nullptr);
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
		tolua_error(tolua_S, "array indexing out of range.", nullptr);
	}
	tolua_pushboolean(tolua_S, (bool)cBlockInfo::FullyOccupiesVoxel((BLOCKTYPE)BlockType));
	return 1;
}
#endif  // #ifndef TOLUA_DISABLE





/** function: cWorld:SetSignLines */
static int tolua_cWorld_SetSignLines(lua_State * tolua_S)
{
	#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype (tolua_S, 1, "cWorld", 0, &tolua_err) ||
		!tolua_isnumber   (tolua_S, 2, 0, &tolua_err) ||
		!tolua_isnumber   (tolua_S, 3, 0, &tolua_err) ||
		!tolua_isnumber   (tolua_S, 4, 0, &tolua_err) ||
		!tolua_iscppstring(tolua_S, 5, 0, &tolua_err) ||
		!tolua_iscppstring(tolua_S, 6, 0, &tolua_err) ||
		!tolua_iscppstring(tolua_S, 7, 0, &tolua_err) ||
		!tolua_iscppstring(tolua_S, 8, 0, &tolua_err) ||
		!tolua_isusertype (tolua_S, 9, "cPlayer", 1, &tolua_err) ||
		!tolua_isnoobj    (tolua_S, 10, &tolua_err)
		)
		goto tolua_lerror;
	else
	#endif
	{
		cWorld * self       = (cWorld *) tolua_tousertype (tolua_S, 1, nullptr);
		int BlockX          = (int)      tolua_tonumber   (tolua_S, 2, 0);
		int BlockY          = (int)      tolua_tonumber   (tolua_S, 3, 0);
		int BlockZ          = (int)      tolua_tonumber   (tolua_S, 4, 0);
		const AString Line1 =            tolua_tocppstring(tolua_S, 5, 0);
		const AString Line2 =            tolua_tocppstring(tolua_S, 6, 0);
		const AString Line3 =            tolua_tocppstring(tolua_S, 7, 0);
		const AString Line4 =            tolua_tocppstring(tolua_S, 8, 0);
		cPlayer * Player    = (cPlayer *)tolua_tousertype (tolua_S, 9, nullptr);
		#ifndef TOLUA_RELEASE
		if (self == nullptr)
		{
			tolua_error(tolua_S, "invalid 'self' in function 'UpdateSign'", nullptr);
		}
		#endif
		{
			bool res = self->SetSignLines(BlockX, BlockY, BlockZ, Line1, Line2, Line3, Line4, Player);
			tolua_pushboolean(tolua_S, res ? 1 : 0);
		}
	}
	lua_do_warning(tolua_S, "Warning in function call '#funcname#': UpdateSign() is deprecated. Please use SetSignLines()");
	return 1;
	
	#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S, "#ferror in function 'UpdateSign'.", &tolua_err);
	return 0;
	#endif
}





void DeprecatedBindings::Bind(lua_State * tolua_S)
{
	tolua_beginmodule(tolua_S, nullptr);

	tolua_array(tolua_S, "g_BlockLightValue",          tolua_get_AllToLua_g_BlockLightValue,          nullptr);
	tolua_array(tolua_S, "g_BlockSpreadLightFalloff",  tolua_get_AllToLua_g_BlockSpreadLightFalloff,  nullptr);
	tolua_array(tolua_S, "g_BlockTransparent",         tolua_get_AllToLua_g_BlockTransparent,         nullptr);
	tolua_array(tolua_S, "g_BlockOneHitDig",           tolua_get_AllToLua_g_BlockOneHitDig,           nullptr);
	tolua_array(tolua_S, "g_BlockPistonBreakable",     tolua_get_AllToLua_g_BlockPistonBreakable,     nullptr);
	tolua_array(tolua_S, "g_BlockIsSnowable",          tolua_get_AllToLua_g_BlockIsSnowable,          nullptr);
	tolua_array(tolua_S, "g_BlockIsSolid",             tolua_get_AllToLua_g_BlockIsSolid,             nullptr);
	tolua_array(tolua_S, "g_BlockFullyOccupiesVoxel",  tolua_get_AllToLua_g_BlockFullyOccupiesVoxel,  nullptr);

	tolua_beginmodule(tolua_S, "cWorld");
		tolua_function(tolua_S, "UpdateSign", tolua_cWorld_SetSignLines);
	tolua_endmodule(tolua_S);

	tolua_endmodule(tolua_S);
}




