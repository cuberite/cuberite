
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "DeprecatedBindings.h"
#undef TOLUA_TEMPLATE_BIND
#include "tolua++/include/tolua++.h"


#include "../BlockInfo.h"
#include "../World.h"
#include "../Entities/Player.h"
#include "LuaState.h"





/* get function: g_BlockLightValue */
#ifndef TOLUA_DISABLE_tolua_get_AllToLua_g_BlockLightValue
static int tolua_get_AllToLua_g_BlockLightValue(lua_State* tolua_S)
{
	BLOCKTYPE BlockType;
	#ifndef TOLUA_RELEASE
	{
		tolua_Error tolua_err;
		if (!tolua_isnumber(tolua_S, 2, 0, &tolua_err))
		{
			tolua_error(tolua_S, "#vinvalid type in array indexing.", &tolua_err);
		}
	}
	#endif
	BlockType = static_cast<BLOCKTYPE>(tolua_tonumber(tolua_S, 2, 0));
	if ((BlockType < 0) || (BlockType > E_BLOCK_MAX_TYPE_ID))
	{
		tolua_error(tolua_S, "array indexing out of range.", nullptr);
	}
	tolua_pushnumber(tolua_S, static_cast<lua_Number>(cBlockInfo::GetLightValue(BlockType)));
	return 1;
}
#endif  // #ifndef TOLUA_DISABLE





/* get function: g_BlockSpreadLightFalloff */
#ifndef TOLUA_DISABLE_tolua_get_AllToLua_g_BlockSpreadLightFalloff
static int tolua_get_AllToLua_g_BlockSpreadLightFalloff(lua_State* tolua_S)
{
	BLOCKTYPE BlockType;
	#ifndef TOLUA_RELEASE
	{
		tolua_Error tolua_err;
		if (!tolua_isnumber(tolua_S, 2, 0, &tolua_err))
		{
			tolua_error(tolua_S, "#vinvalid type in array indexing.", &tolua_err);
		}
	}
	#endif
	BlockType = static_cast<BLOCKTYPE>(tolua_tonumber(tolua_S, 2, 0));
	if ((BlockType < 0) || (BlockType > E_BLOCK_MAX_TYPE_ID))
	{
		tolua_error(tolua_S, "array indexing out of range.", nullptr);
	}
	tolua_pushnumber(tolua_S, static_cast<lua_Number>(cBlockInfo::GetSpreadLightFalloff(BlockType)));
	return 1;
}
#endif  // #ifndef TOLUA_DISABLE





/* get function: g_BlockTransparent */
#ifndef TOLUA_DISABLE_tolua_get_AllToLua_g_BlockTransparent
static int tolua_get_AllToLua_g_BlockTransparent(lua_State* tolua_S)
{
	BLOCKTYPE BlockType;
	#ifndef TOLUA_RELEASE
	{
		tolua_Error tolua_err;
		if (!tolua_isnumber(tolua_S, 2, 0, &tolua_err))
		{
			tolua_error(tolua_S, "#vinvalid type in array indexing.", &tolua_err);
		}
	}
	#endif
	BlockType = static_cast<BLOCKTYPE>(tolua_tonumber(tolua_S, 2, 0));
	if ((BlockType < 0) || (BlockType > E_BLOCK_MAX_TYPE_ID))
	{
		tolua_error(tolua_S, "array indexing out of range.", nullptr);
	}
	tolua_pushboolean(tolua_S, cBlockInfo::IsTransparent(BlockType));
	return 1;
}
#endif  // #ifndef TOLUA_DISABLE





/* get function: g_BlockOneHitDig */
#ifndef TOLUA_DISABLE_tolua_get_AllToLua_g_BlockOneHitDig
static int tolua_get_AllToLua_g_BlockOneHitDig(lua_State* tolua_S)
{
	BLOCKTYPE BlockType;
	#ifndef TOLUA_RELEASE
	{
		tolua_Error tolua_err;
		if (!tolua_isnumber(tolua_S, 2, 0, &tolua_err))
		{
			tolua_error(tolua_S, "#vinvalid type in array indexing.", &tolua_err);
		}
	}
	#endif
	BlockType = static_cast<BLOCKTYPE>(tolua_tonumber(tolua_S, 2, 0));
	if ((BlockType < 0) || (BlockType > E_BLOCK_MAX_TYPE_ID))
	{
		tolua_error(tolua_S, "array indexing out of range.", nullptr);
	}
	tolua_pushboolean(tolua_S, cBlockInfo::IsOneHitDig(BlockType));
	return 1;
}
#endif  // #ifndef TOLUA_DISABLE





/* get function: g_BlockPistonBreakable */
#ifndef TOLUA_DISABLE_tolua_get_AllToLua_g_BlockPistonBreakable
static int tolua_get_AllToLua_g_BlockPistonBreakable(lua_State* tolua_S)
{
	BLOCKTYPE BlockType;
	#ifndef TOLUA_RELEASE
	{
		tolua_Error tolua_err;
		if (!tolua_isnumber(tolua_S, 2, 0, &tolua_err))
		{
			tolua_error(tolua_S, "#vinvalid type in array indexing.", &tolua_err);
		}
	}
	#endif
	BlockType = static_cast<BLOCKTYPE>(tolua_tonumber(tolua_S, 2, 0));
	if ((BlockType < 0) || (BlockType > E_BLOCK_MAX_TYPE_ID))
	{
		tolua_error(tolua_S, "array indexing out of range.", nullptr);
	}
	tolua_pushboolean(tolua_S, cBlockInfo::IsPistonBreakable(BlockType));
	return 1;
}
#endif  // #ifndef TOLUA_DISABLE





/* get function: g_BlockIsSnowable */
#ifndef TOLUA_DISABLE_tolua_get_AllToLua_g_BlockIsSnowable
static int tolua_get_AllToLua_g_BlockIsSnowable(lua_State* tolua_S)
{
	BLOCKTYPE BlockType;
	#ifndef TOLUA_RELEASE
	{
		tolua_Error tolua_err;
		if (!tolua_isnumber(tolua_S, 2, 0, &tolua_err))
		{
			tolua_error(tolua_S, "#vinvalid type in array indexing.", &tolua_err);
		}
	}
	#endif
	BlockType = static_cast<BLOCKTYPE>(tolua_tonumber(tolua_S, 2, 0));
	if ((BlockType < 0) || (BlockType > E_BLOCK_MAX_TYPE_ID))
	{
		tolua_error(tolua_S, "array indexing out of range.", nullptr);
	}
	tolua_pushboolean(tolua_S, cBlockInfo::IsSnowable(BlockType));
	return 1;
}
#endif  // #ifndef TOLUA_DISABLE





/* get function: g_BlockIsSolid */
#ifndef TOLUA_DISABLE_tolua_get_AllToLua_g_BlockIsSolid
static int tolua_get_AllToLua_g_BlockIsSolid(lua_State* tolua_S)
{
	BLOCKTYPE BlockType;
	#ifndef TOLUA_RELEASE
	{
		tolua_Error tolua_err;
		if (!tolua_isnumber(tolua_S, 2, 0, &tolua_err))
		{
			tolua_error(tolua_S, "#vinvalid type in array indexing.", &tolua_err);
		}
	}
	#endif
	BlockType = static_cast<BLOCKTYPE>(tolua_tonumber(tolua_S, 2, 0));
	if ((BlockType < 0) || (BlockType > E_BLOCK_MAX_TYPE_ID))
	{
		tolua_error(tolua_S, "array indexing out of range.", nullptr);
	}
	tolua_pushboolean(tolua_S, static_cast<bool>(cBlockInfo::IsSolid(BlockType)));
	return 1;
}
#endif  // #ifndef TOLUA_DISABLE





/* get function: g_BlockFullyOccupiesVoxel */
#ifndef TOLUA_DISABLE_tolua_get_AllToLua_g_BlockFullyOccupiesVoxel
static int tolua_get_AllToLua_g_BlockFullyOccupiesVoxel(lua_State* tolua_S)
{
	BLOCKTYPE BlockType;
	#ifndef TOLUA_RELEASE
	{
		tolua_Error tolua_err;
		if (!tolua_isnumber(tolua_S, 2, 0, &tolua_err))
		{
			tolua_error(tolua_S, "#vinvalid type in array indexing.", &tolua_err);
		}
	}
	#endif
	BlockType = static_cast<BLOCKTYPE>(tolua_tonumber(tolua_S, 2, 0));
	if ((BlockType < 0) || (BlockType > E_BLOCK_MAX_TYPE_ID))
	{
		tolua_error(tolua_S, "array indexing out of range.", nullptr);
	}
	tolua_pushboolean(tolua_S, static_cast<bool>(cBlockInfo::FullyOccupiesVoxel(BlockType)));
	return 1;
}
#endif  // #ifndef TOLUA_DISABLE





/* function: StringToMobType */
static int tolua_AllToLua_StringToMobType00(lua_State* tolua_S)
{
	cLuaState LuaState(tolua_S);

	#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_iscppstring(tolua_S, 1, 0, &tolua_err) ||
		!tolua_isnoobj(tolua_S, 2, &tolua_err)
		)
		goto tolua_lerror;
	else
	#endif
	{
		const AString a_MobString = tolua_tocppstring(LuaState, 1, 0);
		eMonsterType MobType = cMonster::StringToMobType(a_MobString);
		tolua_pushnumber(LuaState, static_cast<lua_Number>(MobType));
		tolua_pushcppstring(LuaState, a_MobString);
	}

	LOGWARNING("Warning in function call 'StringToMobType': StringToMobType() is deprecated. Please use cMonster:StringToMobType()");
	LuaState.LogStackTrace(0);
	return 2;

	#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(LuaState, "#ferror in function 'StringToMobType'.", &tolua_err);
	return 0;
	#endif
}





/** function: cWorld:SetSignLines */
static int tolua_cWorld_SetSignLines(lua_State * tolua_S)
{
	cLuaState LuaState(tolua_S);

	#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype (LuaState, 1, "cWorld", 0, &tolua_err) ||
		!tolua_isnumber   (LuaState, 2, 0, &tolua_err) ||
		!tolua_isnumber   (LuaState, 3, 0, &tolua_err) ||
		!tolua_isnumber   (LuaState, 4, 0, &tolua_err) ||
		!tolua_iscppstring(LuaState, 5, 0, &tolua_err) ||
		!tolua_iscppstring(LuaState, 6, 0, &tolua_err) ||
		!tolua_iscppstring(LuaState, 7, 0, &tolua_err) ||
		!tolua_iscppstring(LuaState, 8, 0, &tolua_err) ||
		!tolua_isusertype (LuaState, 9, "cPlayer", 1, &tolua_err) ||
		!tolua_isnoobj    (LuaState, 10, &tolua_err)
		)
		goto tolua_lerror;
	else
	#endif
	{
		cWorld * self       = reinterpret_cast<cWorld *>(tolua_tousertype (LuaState, 1, nullptr));
		cPlayer * Player    = reinterpret_cast<cPlayer*>(tolua_tousertype (LuaState, 9, nullptr));
		int BlockX          = static_cast<int>(tolua_tonumber(LuaState, 2, 0));
		int BlockY          = static_cast<int>(tolua_tonumber(LuaState, 3, 0));
		int BlockZ          = static_cast<int>(tolua_tonumber(LuaState, 4, 0));
		const AString Line1 = tolua_tocppstring(LuaState, 5, 0);
		const AString Line2 = tolua_tocppstring(LuaState, 6, 0);
		const AString Line3 = tolua_tocppstring(LuaState, 7, 0);
		const AString Line4 = tolua_tocppstring(LuaState, 8, 0);
		#ifndef TOLUA_RELEASE
		if (self == nullptr)
		{
			tolua_error(LuaState, "invalid 'self' in function 'UpdateSign'", nullptr);
		}
		#endif
		{
			bool res = self->SetSignLines(BlockX, BlockY, BlockZ, Line1, Line2, Line3, Line4, Player);
			tolua_pushboolean(LuaState, res ? 1 : 0);
		}
	}
	LOGWARNING("Warning in function call 'UpdateSign': UpdateSign() is deprecated. Please use SetSignLines()");
	LuaState.LogStackTrace(0);
	return 1;
	
	#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(LuaState, "#ferror in function 'UpdateSign'.", &tolua_err);
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

	tolua_function(tolua_S, "StringToMobType", tolua_AllToLua_StringToMobType00);

	tolua_beginmodule(tolua_S, "cWorld");
		tolua_function(tolua_S, "UpdateSign", tolua_cWorld_SetSignLines);
	tolua_endmodule(tolua_S);

	tolua_endmodule(tolua_S);
}




