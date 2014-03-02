
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "DeprecatedBindings.h"
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
	int tolua_index;
	#ifndef TOLUA_RELEASE
	{
		tolua_Error tolua_err;
		if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
			tolua_error(tolua_S,"#vinvalid type in array indexing.",&tolua_err);
	}
	#endif
	tolua_index = (int)tolua_tonumber(tolua_S,2,0);
	#ifndef TOLUA_RELEASE
	if (tolua_index<0)
		tolua_error(tolua_S,"array indexing out of range.",NULL);
	#endif
	tolua_pushnumber(tolua_S,(lua_Number)cBlockInfo::GetLightValue(tolua_index));
	return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: g_BlockLightValue */
#ifndef TOLUA_DISABLE_tolua_set_AllToLua_g_BlockLightValue
static int tolua_set_AllToLua_g_BlockLightValue(lua_State* tolua_S)
{
	int tolua_index;
	#ifndef TOLUA_RELEASE
	{
		tolua_Error tolua_err;
		if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
			tolua_error(tolua_S,"#vinvalid type in array indexing.",&tolua_err);
	}
	#endif
	tolua_index = (int)tolua_tonumber(tolua_S,2,0);
	#ifndef TOLUA_RELEASE
	if (tolua_index<0)
		tolua_error(tolua_S,"array indexing out of range.",NULL);
	#endif
	cBlockInfo::GetById(tolua_index).m_LightValue = ((unsigned char)  tolua_tonumber(tolua_S,3,0));
	return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: g_BlockSpreadLightFalloff */
#ifndef TOLUA_DISABLE_tolua_get_AllToLua_g_BlockSpreadLightFalloff
static int tolua_get_AllToLua_g_BlockSpreadLightFalloff(lua_State* tolua_S)
{
	int tolua_index;
	#ifndef TOLUA_RELEASE
	{
		tolua_Error tolua_err;
		if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
			tolua_error(tolua_S,"#vinvalid type in array indexing.",&tolua_err);
	}
	#endif
	tolua_index = (int)tolua_tonumber(tolua_S,2,0);
	#ifndef TOLUA_RELEASE
	if (tolua_index<0)
		tolua_error(tolua_S,"array indexing out of range.",NULL);
	#endif
	tolua_pushnumber(tolua_S,(lua_Number)cBlockInfo::GetSpreadLightFalloff(tolua_index));
	return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: g_BlockSpreadLightFalloff */
#ifndef TOLUA_DISABLE_tolua_set_AllToLua_g_BlockSpreadLightFalloff
static int tolua_set_AllToLua_g_BlockSpreadLightFalloff(lua_State* tolua_S)
{
	int tolua_index;
	#ifndef TOLUA_RELEASE
	{
		tolua_Error tolua_err;
		if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
			tolua_error(tolua_S,"#vinvalid type in array indexing.",&tolua_err);
	}
	#endif
	tolua_index = (int)tolua_tonumber(tolua_S,2,0);
	#ifndef TOLUA_RELEASE
	if (tolua_index<0)
		tolua_error(tolua_S,"array indexing out of range.",NULL);
	#endif
	cBlockInfo::GetById(tolua_index).m_SpreadLightFalloff = ((unsigned char)  tolua_tonumber(tolua_S,3,0));
	return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: g_BlockTransparent */
#ifndef TOLUA_DISABLE_tolua_get_AllToLua_g_BlockTransparent
static int tolua_get_AllToLua_g_BlockTransparent(lua_State* tolua_S)
{
	int tolua_index;
	#ifndef TOLUA_RELEASE
	{
		tolua_Error tolua_err;
		if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
			tolua_error(tolua_S,"#vinvalid type in array indexing.",&tolua_err);
	}
	#endif
	tolua_index = (int)tolua_tonumber(tolua_S,2,0);
	#ifndef TOLUA_RELEASE
	if (tolua_index<0)
		tolua_error(tolua_S,"array indexing out of range.",NULL);
	#endif
	tolua_pushboolean(tolua_S,(bool)cBlockInfo::IsTransparent(tolua_index));
	return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: g_BlockTransparent */
#ifndef TOLUA_DISABLE_tolua_set_AllToLua_g_BlockTransparent
static int tolua_set_AllToLua_g_BlockTransparent(lua_State* tolua_S)
{
	int tolua_index;
	#ifndef TOLUA_RELEASE
	{
		tolua_Error tolua_err;
		if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
			tolua_error(tolua_S,"#vinvalid type in array indexing.",&tolua_err);
	}
	#endif
	tolua_index = (int)tolua_tonumber(tolua_S,2,0);
	#ifndef TOLUA_RELEASE
	if (tolua_index<0)
		tolua_error(tolua_S,"array indexing out of range.",NULL);
	#endif
	cBlockInfo::GetById(tolua_index).m_Transparent = ((bool)  tolua_toboolean(tolua_S,3,0));
	return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: g_BlockOneHitDig */
#ifndef TOLUA_DISABLE_tolua_get_AllToLua_g_BlockOneHitDig
static int tolua_get_AllToLua_g_BlockOneHitDig(lua_State* tolua_S)
{
	int tolua_index;
	#ifndef TOLUA_RELEASE
	{
		tolua_Error tolua_err;
		if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
			tolua_error(tolua_S,"#vinvalid type in array indexing.",&tolua_err);
	}
	#endif
	tolua_index = (int)tolua_tonumber(tolua_S,2,0);
	#ifndef TOLUA_RELEASE
	if (tolua_index<0)
		tolua_error(tolua_S,"array indexing out of range.",NULL);
	#endif
	tolua_pushboolean(tolua_S,(bool)cBlockInfo::IsOneHitDig(tolua_index));
	return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: g_BlockOneHitDig */
#ifndef TOLUA_DISABLE_tolua_set_AllToLua_g_BlockOneHitDig
static int tolua_set_AllToLua_g_BlockOneHitDig(lua_State* tolua_S)
{
	int tolua_index;
	#ifndef TOLUA_RELEASE
	{
		tolua_Error tolua_err;
		if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
			tolua_error(tolua_S,"#vinvalid type in array indexing.",&tolua_err);
	}
	#endif
	tolua_index = (int)tolua_tonumber(tolua_S,2,0);
	#ifndef TOLUA_RELEASE
	if (tolua_index<0)
		tolua_error(tolua_S,"array indexing out of range.",NULL);
	#endif
	cBlockInfo::GetById(tolua_index).m_OneHitDig = ((bool)  tolua_toboolean(tolua_S,3,0));
	return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: g_BlockPistonBreakable */
#ifndef TOLUA_DISABLE_tolua_get_AllToLua_g_BlockPistonBreakable
static int tolua_get_AllToLua_g_BlockPistonBreakable(lua_State* tolua_S)
{
	int tolua_index;
	#ifndef TOLUA_RELEASE
	{
		tolua_Error tolua_err;
		if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
			tolua_error(tolua_S,"#vinvalid type in array indexing.",&tolua_err);
	}
	#endif
	tolua_index = (int)tolua_tonumber(tolua_S,2,0);
	#ifndef TOLUA_RELEASE
	if (tolua_index<0 || tolua_index>=256)
		tolua_error(tolua_S,"array indexing out of range.",NULL);
	#endif
	tolua_pushboolean(tolua_S,(bool)cBlockInfo::IsPistonBreakable(tolua_index));
	return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: g_BlockPistonBreakable */
#ifndef TOLUA_DISABLE_tolua_set_AllToLua_g_BlockPistonBreakable
static int tolua_set_AllToLua_g_BlockPistonBreakable(lua_State* tolua_S)
{
	int tolua_index;
	#ifndef TOLUA_RELEASE
	{
		tolua_Error tolua_err;
		if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
			tolua_error(tolua_S,"#vinvalid type in array indexing.",&tolua_err);
	}
	#endif
	tolua_index = (int)tolua_tonumber(tolua_S,2,0);
	#ifndef TOLUA_RELEASE
	if (tolua_index<0 || tolua_index>=256)
		tolua_error(tolua_S,"array indexing out of range.",NULL);
	#endif
	cBlockInfo::GetById(tolua_index).m_PistonBreakable = ((bool)  tolua_toboolean(tolua_S,3,0));
	return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: g_BlockIsSnowable */
#ifndef TOLUA_DISABLE_tolua_get_AllToLua_g_BlockIsSnowable
static int tolua_get_AllToLua_g_BlockIsSnowable(lua_State* tolua_S)
{
	int tolua_index;
	#ifndef TOLUA_RELEASE
	{
		tolua_Error tolua_err;
		if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
			tolua_error(tolua_S,"#vinvalid type in array indexing.",&tolua_err);
	}
	#endif
	tolua_index = (int)tolua_tonumber(tolua_S,2,0);
	#ifndef TOLUA_RELEASE
	if (tolua_index<0 || tolua_index>=256)
		tolua_error(tolua_S,"array indexing out of range.",NULL);
	#endif
	tolua_pushboolean(tolua_S,(bool)cBlockInfo::IsSnowable(tolua_index));
	return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: g_BlockIsSnowable */
#ifndef TOLUA_DISABLE_tolua_set_AllToLua_g_BlockIsSnowable
static int tolua_set_AllToLua_g_BlockIsSnowable(lua_State* tolua_S)
{
	int tolua_index;
	#ifndef TOLUA_RELEASE
	{
		tolua_Error tolua_err;
		if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
			tolua_error(tolua_S,"#vinvalid type in array indexing.",&tolua_err);
	}
	#endif
	tolua_index = (int)tolua_tonumber(tolua_S,2,0);
	#ifndef TOLUA_RELEASE
	if (tolua_index<0 || tolua_index>=256)
		tolua_error(tolua_S,"array indexing out of range.",NULL);
	#endif
	cBlockInfo::GetById(tolua_index).m_IsSnowable = ((bool)  tolua_toboolean(tolua_S,3,0));
	return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: g_BlockRequiresSpecialTool */
#ifndef TOLUA_DISABLE_tolua_get_AllToLua_g_BlockRequiresSpecialTool
static int tolua_get_AllToLua_g_BlockRequiresSpecialTool(lua_State* tolua_S)
{
	int tolua_index;
	#ifndef TOLUA_RELEASE
	{
		tolua_Error tolua_err;
		if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
			tolua_error(tolua_S,"#vinvalid type in array indexing.",&tolua_err);
	}
	#endif
	tolua_index = (int)tolua_tonumber(tolua_S,2,0);
	#ifndef TOLUA_RELEASE
	if (tolua_index<0 || tolua_index>=256)
		tolua_error(tolua_S,"array indexing out of range.",NULL);
	#endif
	tolua_pushboolean(tolua_S,(bool)cBlockInfo::RequiresSpecialTool(tolua_index));
	return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: g_BlockRequiresSpecialTool */
#ifndef TOLUA_DISABLE_tolua_set_AllToLua_g_BlockRequiresSpecialTool
static int tolua_set_AllToLua_g_BlockRequiresSpecialTool(lua_State* tolua_S)
{
	int tolua_index;
	#ifndef TOLUA_RELEASE
	{
		tolua_Error tolua_err;
		if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
			tolua_error(tolua_S,"#vinvalid type in array indexing.",&tolua_err);
	}
	#endif
	tolua_index = (int)tolua_tonumber(tolua_S,2,0);
	#ifndef TOLUA_RELEASE
	if (tolua_index<0 || tolua_index>=256)
		tolua_error(tolua_S,"array indexing out of range.",NULL);
	#endif
	cBlockInfo::GetById(tolua_index).m_RequiresSpecialTool = ((bool)  tolua_toboolean(tolua_S,3,0));
	return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: g_BlockIsSolid */
#ifndef TOLUA_DISABLE_tolua_get_AllToLua_g_BlockIsSolid
static int tolua_get_AllToLua_g_BlockIsSolid(lua_State* tolua_S)
{
	int tolua_index;
	#ifndef TOLUA_RELEASE
	{
		tolua_Error tolua_err;
		if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
			tolua_error(tolua_S,"#vinvalid type in array indexing.",&tolua_err);
	}
	#endif
	tolua_index = (int)tolua_tonumber(tolua_S,2,0);
	#ifndef TOLUA_RELEASE
	if (tolua_index<0 || tolua_index>=256)
		tolua_error(tolua_S,"array indexing out of range.",NULL);
	#endif
	tolua_pushboolean(tolua_S,(bool)cBlockInfo::IsSolid(tolua_index));
	return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: g_BlockIsSolid */
#ifndef TOLUA_DISABLE_tolua_set_AllToLua_g_BlockIsSolid
static int tolua_set_AllToLua_g_BlockIsSolid(lua_State* tolua_S)
{
	int tolua_index;
	#ifndef TOLUA_RELEASE
	{
		tolua_Error tolua_err;
		if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
			tolua_error(tolua_S,"#vinvalid type in array indexing.",&tolua_err);
	}
	#endif
	tolua_index = (int)tolua_tonumber(tolua_S,2,0);
	#ifndef TOLUA_RELEASE
	if (tolua_index<0 || tolua_index>=256)
		tolua_error(tolua_S,"array indexing out of range.",NULL);
	#endif
	cBlockInfo::GetById(tolua_index).m_IsSolid = ((bool)  tolua_toboolean(tolua_S,3,0));
	return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: g_BlockFullyOccupiesVoxel */
#ifndef TOLUA_DISABLE_tolua_get_AllToLua_g_BlockFullyOccupiesVoxel
static int tolua_get_AllToLua_g_BlockFullyOccupiesVoxel(lua_State* tolua_S)
{
	int tolua_index;
	#ifndef TOLUA_RELEASE
	{
		tolua_Error tolua_err;
		if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
			tolua_error(tolua_S,"#vinvalid type in array indexing.",&tolua_err);
	}
	#endif
	tolua_index = (int)tolua_tonumber(tolua_S,2,0);
	#ifndef TOLUA_RELEASE
	if (tolua_index<0 || tolua_index>=256)
		tolua_error(tolua_S,"array indexing out of range.",NULL);
	#endif
	tolua_pushboolean(tolua_S,(bool)cBlockInfo::FullyOccupiesVoxel(tolua_index));
	return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: g_BlockFullyOccupiesVoxel */
#ifndef TOLUA_DISABLE_tolua_set_AllToLua_g_BlockFullyOccupiesVoxel
static int tolua_set_AllToLua_g_BlockFullyOccupiesVoxel(lua_State* tolua_S)
{
	int tolua_index;
	#ifndef TOLUA_RELEASE
	{
		tolua_Error tolua_err;
		if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
			tolua_error(tolua_S,"#vinvalid type in array indexing.",&tolua_err);
	}
	#endif
	tolua_index = (int)tolua_tonumber(tolua_S,2,0);
	#ifndef TOLUA_RELEASE
	if (tolua_index<0 || tolua_index>=256)
		tolua_error(tolua_S,"array indexing out of range.",NULL);
	#endif
	cBlockInfo::GetById(tolua_index).m_FullyOccupiesVoxel = ((bool)  tolua_toboolean(tolua_S,3,0));
	return 0;
}
#endif //#ifndef TOLUA_DISABLE





void DeprecatedBindings::Bind(lua_State * tolua_S)
{
	tolua_array(tolua_S, "g_BlockLightValue",          tolua_get_AllToLua_g_BlockLightValue,          tolua_set_AllToLua_g_BlockLightValue);
	tolua_array(tolua_S, "g_BlockSpreadLightFalloff",  tolua_get_AllToLua_g_BlockSpreadLightFalloff,  tolua_set_AllToLua_g_BlockSpreadLightFalloff);
	tolua_array(tolua_S, "g_BlockTransparent",         tolua_get_AllToLua_g_BlockTransparent,         tolua_set_AllToLua_g_BlockTransparent);
	tolua_array(tolua_S, "g_BlockOneHitDig",           tolua_get_AllToLua_g_BlockOneHitDig,           tolua_set_AllToLua_g_BlockOneHitDig);
	tolua_array(tolua_S, "g_BlockPistonBreakable",     tolua_get_AllToLua_g_BlockPistonBreakable,     tolua_set_AllToLua_g_BlockPistonBreakable);
	tolua_array(tolua_S, "g_BlockIsSnowable",          tolua_get_AllToLua_g_BlockIsSnowable,          tolua_set_AllToLua_g_BlockIsSnowable);
	tolua_array(tolua_S, "g_BlockRequiresSpecialTool", tolua_get_AllToLua_g_BlockRequiresSpecialTool, tolua_set_AllToLua_g_BlockRequiresSpecialTool);
	tolua_array(tolua_S, "g_BlockIsSolid",             tolua_get_AllToLua_g_BlockIsSolid,             tolua_set_AllToLua_g_BlockIsSolid);
	tolua_array(tolua_S, "g_BlockFullyOccupiesVoxel",  tolua_get_AllToLua_g_BlockFullyOccupiesVoxel,  tolua_set_AllToLua_g_BlockFullyOccupiesVoxel);
}




