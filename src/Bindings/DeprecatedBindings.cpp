
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "DeprecatedBindings.h"
#undef TOLUA_TEMPLATE_BIND
#include "tolua++/include/tolua++.h"


#include "../World.h"
#include "../Entities/Player.h"
#include "LuaState.h"
#include "../BlockInfo.h"
#include "../BlockEntities/NoteEntity.h"





/* get function: g_BlockLightValue */
#ifndef TOLUA_DISABLE_tolua_get_AllToLua_g_BlockLightValue
static int tolua_get_AllToLua_g_BlockLightValue(lua_State* tolua_S)
{
	cLuaState LuaState(tolua_S);

	int BlockType = 0;
	#ifndef TOLUA_RELEASE
	{
		tolua_Error tolua_err;
		if (!tolua_isnumber(tolua_S, 2, 0, &tolua_err))
		{
			tolua_error(tolua_S, "#vinvalid type in array indexing.", &tolua_err);
		}
	}
	#endif
	LuaState.GetStackValue(2, BlockType);
	if ((BlockType < 0) || (BlockType > E_BLOCK_MAX_TYPE_ID))
	{
		tolua_error(tolua_S, "array indexing out of range.", nullptr);
	}
	LuaState.Push(cBlockInfo::GetLightValue(static_cast<BLOCKTYPE>(BlockType)));
	return 1;
}
#endif  // #ifndef TOLUA_DISABLE





/* get function: g_BlockSpreadLightFalloff */
#ifndef TOLUA_DISABLE_tolua_get_AllToLua_g_BlockSpreadLightFalloff
static int tolua_get_AllToLua_g_BlockSpreadLightFalloff(lua_State* tolua_S)
{
	cLuaState LuaState(tolua_S);

	int BlockType = 0;
	#ifndef TOLUA_RELEASE
	{
		tolua_Error tolua_err;
		if (!tolua_isnumber(tolua_S, 2, 0, &tolua_err))
		{
			tolua_error(tolua_S, "#vinvalid type in array indexing.", &tolua_err);
		}
	}
	#endif
	LuaState.GetStackValue(2, BlockType);
	if ((BlockType < 0) || (BlockType > E_BLOCK_MAX_TYPE_ID))
	{
		tolua_error(tolua_S, "array indexing out of range.", nullptr);
	}
	LuaState.Push(cBlockInfo::GetSpreadLightFalloff(static_cast<BLOCKTYPE>(BlockType)));
	return 1;
}
#endif  // #ifndef TOLUA_DISABLE





/* get function: g_BlockTransparent */
#ifndef TOLUA_DISABLE_tolua_get_AllToLua_g_BlockTransparent
static int tolua_get_AllToLua_g_BlockTransparent(lua_State* tolua_S)
{
	cLuaState LuaState(tolua_S);

	int BlockType = 0;
	#ifndef TOLUA_RELEASE
	{
		tolua_Error tolua_err;
		if (!tolua_isnumber(tolua_S, 2, 0, &tolua_err))
		{
			tolua_error(tolua_S, "#vinvalid type in array indexing.", &tolua_err);
		}
	}
	#endif
	LuaState.GetStackValue(2, BlockType);
	if ((BlockType < 0) || (BlockType > E_BLOCK_MAX_TYPE_ID))
	{
		tolua_error(tolua_S, "array indexing out of range.", nullptr);
	}
	LuaState.Push(cBlockInfo::IsTransparent(static_cast<BLOCKTYPE>(BlockType)));
	return 1;
}
#endif  // #ifndef TOLUA_DISABLE





/* get function: g_BlockOneHitDig */
#ifndef TOLUA_DISABLE_tolua_get_AllToLua_g_BlockOneHitDig
static int tolua_get_AllToLua_g_BlockOneHitDig(lua_State* tolua_S)
{
	cLuaState LuaState(tolua_S);

	int BlockType = 0;
	#ifndef TOLUA_RELEASE
	{
		tolua_Error tolua_err;
		if (!tolua_isnumber(tolua_S, 2, 0, &tolua_err))
		{
			tolua_error(tolua_S, "#vinvalid type in array indexing.", &tolua_err);
		}
	}
	#endif
	LuaState.GetStackValue(2, BlockType);
	if ((BlockType < 0) || (BlockType > E_BLOCK_MAX_TYPE_ID))
	{
		tolua_error(tolua_S, "array indexing out of range.", nullptr);
	}
	LuaState.Push(cBlockInfo::IsOneHitDig(static_cast<BLOCKTYPE>(BlockType)));
	return 1;
}
#endif  // #ifndef TOLUA_DISABLE





/* get function: g_BlockPistonBreakable */
#ifndef TOLUA_DISABLE_tolua_get_AllToLua_g_BlockPistonBreakable
static int tolua_get_AllToLua_g_BlockPistonBreakable(lua_State* tolua_S)
{
	cLuaState LuaState(tolua_S);

	int BlockType = 0;
	#ifndef TOLUA_RELEASE
	{
		tolua_Error tolua_err;
		if (!tolua_isnumber(tolua_S, 2, 0, &tolua_err))
		{
			tolua_error(tolua_S, "#vinvalid type in array indexing.", &tolua_err);
		}
	}
	#endif
	LuaState.GetStackValue(2, BlockType);
	if ((BlockType < 0) || (BlockType > E_BLOCK_MAX_TYPE_ID))
	{
		tolua_error(tolua_S, "array indexing out of range.", nullptr);
	}
	LuaState.Push(cBlockInfo::IsPistonBreakable(static_cast<BLOCKTYPE>(BlockType)));
	return 1;
}
#endif  // #ifndef TOLUA_DISABLE





/* get function: g_BlockIsSnowable */
#ifndef TOLUA_DISABLE_tolua_get_AllToLua_g_BlockIsSnowable
static int tolua_get_AllToLua_g_BlockIsSnowable(lua_State* tolua_S)
{
	cLuaState LuaState(tolua_S);

	int BlockType = 0;
	#ifndef TOLUA_RELEASE
	{
		tolua_Error tolua_err;
		if (!tolua_isnumber(tolua_S, 2, 0, &tolua_err))
		{
			tolua_error(tolua_S, "#vinvalid type in array indexing.", &tolua_err);
		}
	}
	#endif
	LuaState.GetStackValue(2, BlockType);
	if ((BlockType < 0) || (BlockType > E_BLOCK_MAX_TYPE_ID))
	{
		tolua_error(tolua_S, "array indexing out of range.", nullptr);
	}
	LuaState.Push(cBlockInfo::IsSnowable(static_cast<BLOCKTYPE>(BlockType)));
	return 1;
}
#endif  // #ifndef TOLUA_DISABLE





/* get function: g_BlockIsSolid */
#ifndef TOLUA_DISABLE_tolua_get_AllToLua_g_BlockIsSolid
static int tolua_get_AllToLua_g_BlockIsSolid(lua_State* tolua_S)
{
	cLuaState LuaState(tolua_S);

	int BlockType = 0;
	#ifndef TOLUA_RELEASE
	{
		tolua_Error tolua_err;
		if (!tolua_isnumber(tolua_S, 2, 0, &tolua_err))
		{
			tolua_error(tolua_S, "#vinvalid type in array indexing.", &tolua_err);
		}
	}
	#endif
	LuaState.GetStackValue(2, BlockType);
	if ((BlockType < 0) || (BlockType > E_BLOCK_MAX_TYPE_ID))
	{
		tolua_error(tolua_S, "array indexing out of range.", nullptr);
	}
	LuaState.Push(cBlockInfo::IsSolid(static_cast<BLOCKTYPE>(BlockType)));
	return 1;
}
#endif  // #ifndef TOLUA_DISABLE





/* get function: g_BlockFullyOccupiesVoxel */
#ifndef TOLUA_DISABLE_tolua_get_AllToLua_g_BlockFullyOccupiesVoxel
static int tolua_get_AllToLua_g_BlockFullyOccupiesVoxel(lua_State* tolua_S)
{
	cLuaState LuaState(tolua_S);

	int BlockType = 0;
	#ifndef TOLUA_RELEASE
	{
		tolua_Error tolua_err;
		if (!tolua_isnumber(tolua_S, 2, 0, &tolua_err))
		{
			tolua_error(tolua_S, "#vinvalid type in array indexing.", &tolua_err);
		}
	}
	#endif
	LuaState.GetStackValue(2, BlockType);
	if ((BlockType < 0) || (BlockType > E_BLOCK_MAX_TYPE_ID))
	{
		tolua_error(tolua_S, "array indexing out of range.", nullptr);
	}
	LuaState.Push(cBlockInfo::FullyOccupiesVoxel(static_cast<BLOCKTYPE>(BlockType)));
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
		const AString a_MobString = tolua_tocppstring(LuaState, 1, nullptr);
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





static int tolua_cBlockInfo_GetPlaceSound(lua_State * tolua_S)
{
	cLuaState L(tolua_S);
	if (
		!L.CheckParamStaticSelf("cBlockInfo") ||
		!L.CheckParamNumber(2)
	)
	{
		return 0;
	}

	L.Push("");
	LOGWARNING("cBlockInfo:GetPlaceSound() is deprecated");
	L.LogStackTrace(0);
	return 1;
}





static int tolua_get_cItem_m_Lore(lua_State * tolua_S)
{
	// Maintain legacy m_Lore variable as Lore table split by ` (grave-accent)
	cLuaState L(tolua_S);
	if (!L.CheckParamSelf("const cItem"))
	{
		return 0;
	}

	const cItem * Self = nullptr;
	L.GetStackValue(1, Self);

	AString LoreString = StringJoin(Self->m_LoreTable, "`");

	L.Push(LoreString);

	LOGWARNING("cItem.m_Lore is deprecated, use cItem.m_LoreTable instead");
	L.LogStackTrace(0);
	return 1;
}





static int tolua_set_cItem_m_Lore(lua_State * tolua_S)
{
	// Maintain legacy m_Lore variable as Lore table split by ` (grave-accent)
	cLuaState L(tolua_S);
	if (
		!L.CheckParamSelf("cItem") ||
		!L.CheckParamString(2)
	)
	{
		return 0;
	}

	cItem * Self = nullptr;
	AString LoreString;
	L.GetStackValues(1, Self, LoreString);

	Self->m_LoreTable = StringSplit(LoreString, "`");

	LOGWARNING("cItem.m_Lore is deprecated, use cItem.m_LoreTable instead");
	L.LogStackTrace(0);
	return 0;
}




/** function: cNoteEntity: GetNote */
static int tolua_cNoteEntity_GetPitch(lua_State * tolua_S)
{
	cLuaState LuaState(tolua_S);

	if (
		!LuaState.CheckParamUserType(1, "cNoteEntity") ||
		!LuaState.CheckParamEnd(2)
	)
	{
		return 0;
	}

	cNoteEntity * Self = nullptr;

	if (!LuaState.GetStackValues(1, Self))
	{
		tolua_error(LuaState, "Failed to read parameters", nullptr);
	}
	if (Self == nullptr)
	{
		tolua_error(LuaState, "invalid 'self' in function 'GetPitch'", nullptr);
	}
	LuaState.Push(Self->GetNote());
	LOGWARNING("Warning: 'cNoteEntity:GetPitch' function is deprecated. Please use 'cNoteEntity:GetNote' instead.");
	LuaState.LogStackTrace(0);
	return 1;
}




/** function: cNoteEntity: IncrementNote */
static int tolua_cNoteEntity_IncrementPitch(lua_State * tolua_S)
{
	cLuaState LuaState(tolua_S);

	if (
		!LuaState.CheckParamUserType(1, "cNoteEntity") ||
		!LuaState.CheckParamEnd(2)
	)
	{
		return 0;
	}

	cNoteEntity * Self = nullptr;

	if (!LuaState.GetStackValues(1, Self))
	{
		tolua_error(LuaState, "Failed to read parameters", nullptr);
	}
	if (Self == nullptr)
	{
		tolua_error(LuaState, "invalid 'self' in function 'SetPitch'", nullptr);
	}

	Self->IncrementNote();
	LOGWARNING("Warning: 'cNoteEntity:IncrementPitch' function is deprecated. Please use 'cNoteEntity:IncrementNote' instead.");
	LuaState.LogStackTrace(0);
	return 1;
}




/** function: cNoteEntity: SetNote */
static int tolua_cNoteEntity_SetPitch(lua_State * tolua_S)
{
	cLuaState LuaState(tolua_S);

	if (
		!LuaState.CheckParamUserType(1, "cNoteEntity") ||
		!LuaState.CheckParamNumber(2) ||
		!LuaState.CheckParamEnd(3)
	)
	{
		return 0;
	}

	cNoteEntity * Self = nullptr;
	int Note = -1;

	if (!LuaState.GetStackValues(1, Self, Note))
	{
		tolua_error(LuaState, "Failed to read parameters", nullptr);
	}
	if (Self == nullptr)
	{
		tolua_error(LuaState, "invalid 'self' in function 'SetPitch'", nullptr);
	}

	Self->SetNote(Note % 25);
	LOGWARNING("Warning: 'cNoteEntity:SetPitch' function is deprecated. Please use 'cNoteEntity:SetNote' instead.");
	LuaState.LogStackTrace(0);
	return 1;
}




/** function: cWorld:SetSignLines */
static int tolua_cWorld_SetSignLines(lua_State * tolua_S)
{
	cLuaState LuaState(tolua_S);

	#ifndef TOLUA_RELEASE
	if (
		!LuaState.CheckParamUserType(1, "cWorld") ||
		!LuaState.CheckParamNumber(2, 4) ||
		!LuaState.CheckParamString(5, 8) ||
		!LuaState.CheckParamUserType(9, "cPlayer") ||
		!LuaState.CheckParamEnd(10)
		)
		return 0;
	else
	#endif
	{
		cWorld * self = nullptr;
		cPlayer * Player = nullptr;
		int BlockX = 0;
		int BlockY = 0;
		int BlockZ = 0;
		AString Line1;
		AString Line2;
		AString Line3;
		AString Line4;

		LuaState.GetStackValues(1, self, BlockX, BlockY, BlockZ, Line1, Line2, Line3, Line4, Player);

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
}





/** Function: cWorld:GrowTree.
Exported manually because of the obsolete int-based overload.
When removing from DeprecatedBindings, make sure the function is exported automatically. */
static int tolua_cWorld_GrowTree(lua_State * a_LuaState)
{
	cLuaState LuaState(a_LuaState);
	if (lua_isnumber(LuaState, 2))
	{
		// This is the obsolete signature, warn and translate:
		LOGWARNING("Warning: cWorld:GrowTree function expects Vector3i-based coords rather than int-based coords. Emulating old-style call.");
		LuaState.LogStackTrace(0);
		cWorld * Self = nullptr;
		int BlockX, BlockY, BlockZ;
		if (!LuaState.GetStackValues(1, Self, BlockX, BlockY, BlockZ))
		{
			return LuaState.ApiParamError("Failed to read int-based coord parameters");
		}
		LuaState.Push(Self->GrowTree({BlockX, BlockY, BlockZ}));
		return 1;
	}

	// This is the correct signature, execute:
	cWorld * Self = nullptr;
	Vector3i BlockPos;
	if (!LuaState.GetStackValues(1, Self, BlockPos))
	{
		return LuaState.ApiParamError("Failed to read Vector3i-based coord parameters");
	}
	LuaState.Push(Self->GrowTree(BlockPos));
	return 1;
}





/** Function: cWorld:GrowTreeByBiome.
Exported manually because of the obsolete int-based overload.
When removing from DeprecatedBindings, make sure the function is exported automatically. */
static int tolua_cWorld_GrowTreeByBiome(lua_State * a_LuaState)
{
	cLuaState LuaState(a_LuaState);
	if (lua_isnumber(LuaState, 2))
	{
		// This is the obsolete signature, warn and translate:
		LOGWARNING("Warning: cWorld:GrowTreeByBiome function expects Vector3i-based coords rather than int-based coords. Emulating old-style call.");
		LuaState.LogStackTrace(0);
		cWorld * Self = nullptr;
		int BlockX, BlockY, BlockZ;
		if (!LuaState.GetStackValues(1, Self, BlockX, BlockY, BlockZ))
		{
			return LuaState.ApiParamError("Failed to read int-based coord parameters");
		}
		LuaState.Push(Self->GrowTreeByBiome({BlockX, BlockY, BlockZ}));
		return 1;
	}

	// This is the correct signature, execute:
	cWorld * Self = nullptr;
	Vector3i BlockPos;
	if (!LuaState.GetStackValues(1, Self, BlockPos))
	{
		return LuaState.ApiParamError("Failed to read Vector3i-based coord parameters");
	}
	LuaState.Push(Self->GrowTreeByBiome(BlockPos));
	return 1;
}





/** Function: cWorld:GrowTreeFromSapling.
Exported manually because of the obsolete int-based overload and obsolete SaplingMeta parameter.
When removing from DeprecatedBindings, make sure the function is exported automatically. */
static int tolua_cWorld_GrowTreeFromSapling(lua_State * a_LuaState)
{
	cLuaState LuaState(a_LuaState);
	if (lua_isnumber(LuaState, 2))
	{
		// This is the obsolete signature, warn and translate:
		LOGWARNING("Warning: cWorld:GrowTreeFromSapling function expects Vector3i-based coords rather than int-based coords. Emulating old-style call.");
		LuaState.LogStackTrace(0);
		cWorld * Self = nullptr;
		int BlockX, BlockY, BlockZ;
		if (!LuaState.GetStackValues(1, Self, BlockX, BlockY, BlockZ))
		{
			return LuaState.ApiParamError("Failed to read int-based coord parameters");
		}
		LuaState.Push(Self->GrowTreeFromSapling({BlockX, BlockY, BlockZ}));
		return 1;
	}

	// This is the correct signature, execute:
	cWorld * Self = nullptr;
	Vector3i BlockPos;
	if (!LuaState.GetStackValues(1, Self, BlockPos))
	{
		return LuaState.ApiParamError("Failed to read Vector3i-based coord parameters");
	}
	if (lua_isnumber(LuaState, 3))
	{
		// There's an extra parameter, the obsolete SaplingMeta
		LOGWARNING("Warning: cWorld:GrowTreeFromSapling function no longer has the SaplingMeta parameter. Ignoring it now.");
		LuaState.LogStackTrace(0);
	}
	LuaState.Push(Self->GrowTreeFromSapling(BlockPos));
	return 1;
}





/** function: cWorld:SetNextBlockTick */
static int tolua_cWorld_SetNextBlockTick(lua_State * tolua_S)
{
	cLuaState LuaState(tolua_S);

	if (
		!LuaState.CheckParamUserType(1, "cWorld") ||
		!LuaState.CheckParamNumber(2, 4) ||
		!LuaState.CheckParamEnd(5)
	)
	{
		return 0;
	}

	cWorld * Self = nullptr;
	int BlockX = 0;
	int BlockY = 0;
	int BlockZ = 0;

	if (!LuaState.GetStackValues(1, Self, BlockX, BlockY, BlockZ))
	{
		tolua_error(LuaState, "Failed to read parameters", nullptr);
	}
	if (Self == nullptr)
	{
		tolua_error(LuaState, "invalid 'self' in function 'SetNextBlockTick'", nullptr);
	}
	Self->SetNextBlockToTick({BlockX, BlockY, BlockZ});
	LOGWARNING("Warning: 'cWorld:SetNextBlockTick' function is deprecated. Please use 'cWorld:SetNextBlockToTick' instead.");
	LuaState.LogStackTrace(0);
	return 1;
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

	tolua_beginmodule(tolua_S, "cBlockInfo");
		tolua_function(tolua_S, "GetPlaceSound",          tolua_cBlockInfo_GetPlaceSound);
	tolua_endmodule(tolua_S);

	tolua_beginmodule(tolua_S, "cItem");
		tolua_variable(tolua_S, "m_Lore", tolua_get_cItem_m_Lore, tolua_set_cItem_m_Lore);
	tolua_endmodule(tolua_S);

	tolua_beginmodule(tolua_S, "cNoteEntity");
		tolua_function(tolua_S, "GetPitch", tolua_cNoteEntity_GetPitch);
		tolua_function(tolua_S, "IncrementPitch", tolua_cNoteEntity_IncrementPitch);
		tolua_function(tolua_S, "SetPitch", tolua_cNoteEntity_SetPitch);
	tolua_endmodule(tolua_S);

	tolua_beginmodule(tolua_S, "cWorld");
		tolua_function(tolua_S, "GrowTree",            tolua_cWorld_GrowTree);
		tolua_function(tolua_S, "GrowTreeByBiome",     tolua_cWorld_GrowTreeByBiome);
		tolua_function(tolua_S, "GrowTreeFromSapling", tolua_cWorld_GrowTreeFromSapling);
		tolua_function(tolua_S, "SetNextBlockTick",    tolua_cWorld_SetNextBlockTick);
		tolua_function(tolua_S, "UpdateSign",          tolua_cWorld_SetSignLines);
	tolua_endmodule(tolua_S);

	tolua_endmodule(tolua_S);
}




