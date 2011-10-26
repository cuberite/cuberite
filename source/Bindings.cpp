/*
** Lua binding: AllToLua
** Generated automatically by tolua++-1.0.92 on 07/15/11 01:36:28.
*/

#ifndef __cplusplus
#include "stdlib.h"
#endif
#include "string.h"

#include "tolua++.h"

/* Exported function */
TOLUA_API int  tolua_AllToLua_open (lua_State* tolua_S);

#include "tolua_base.h"
#include "cTorch.h"
#include "cStairs.h"
#include "cLadder.h"
#include "../iniFile/iniFile.h"
#include "BlockID.h"
#include "PacketID.h"
#include "Defines.h"
#include "LuaFunctions.h"
#include "cStringMap.h"
#include "cChatColor.h"
#include "cClientHandle.h"
#include "cEntity.h"
#include "cPawn.h"
#include "cPlayer.h"
#include "cPluginManager.h"
#include "cPlugin.h"
#include "cPlugin_Lua.h"
#include "cServer.h"
#include "cWorld.h"
#include "cInventory.h"
#include "cItem.h"
#include "cWebAdmin.h"
#include "cWebPlugin.h"
#include "cPickup.h"
#include "cRoot.h"
#include "cTCPLink.h"
#include "Vector3f.h"
#include "Vector3d.h"
#include "Vector3i.h"
#include "Matrix4f.h"
#include "cCuboid.h"
#include "cMCLogger.h"
#include "cTracer.h"
#include "cGroup.h"
#include "packets/cPacket_Login.h"
#include "packets/cPacket_BlockDig.h"
#include "packets/cPacket_BlockPlace.h"

/* function to release collected object via destructor */
#ifdef __cplusplus

static int tolua_collect_cItem (lua_State* tolua_S)
{
 cItem* self = (cItem*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_Vector3f (lua_State* tolua_S)
{
 Vector3f* self = (Vector3f*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_cIniFile (lua_State* tolua_S)
{
 cIniFile* self = (cIniFile*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_cPickup (lua_State* tolua_S)
{
 cPickup* self = (cPickup*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_cPacket_BlockDig (lua_State* tolua_S)
{
 cPacket_BlockDig* self = (cPacket_BlockDig*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_cTracer (lua_State* tolua_S)
{
 cTracer* self = (cTracer*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_cPlugin (lua_State* tolua_S)
{
 cPlugin* self = (cPlugin*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_cMCLogger (lua_State* tolua_S)
{
 cMCLogger* self = (cMCLogger*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_cCuboid (lua_State* tolua_S)
{
 cCuboid* self = (cCuboid*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_cWebPlugin (lua_State* tolua_S)
{
 cWebPlugin* self = (cWebPlugin*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_Vector3i (lua_State* tolua_S)
{
 Vector3i* self = (Vector3i*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_cEntity (lua_State* tolua_S)
{
 cEntity* self = (cEntity*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_cTCPLink (lua_State* tolua_S)
{
 cTCPLink* self = (cTCPLink*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_Vector3d (lua_State* tolua_S)
{
 Vector3d* self = (Vector3d*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}
#endif


/* function to register type */
static void tolua_reg_types (lua_State* tolua_S)
{
 tolua_usertype(tolua_S,"TakeDamageInfo");
 tolua_usertype(tolua_S,"cPlugin");
 tolua_usertype(tolua_S,"cStringMap");
 tolua_usertype(tolua_S,"cServer");
 tolua_usertype(tolua_S,"cRoot");
 tolua_usertype(tolua_S,"cPlugin::CommandStruct");
 tolua_usertype(tolua_S,"cPickup");
 tolua_usertype(tolua_S,"cRecipeChecker");
 tolua_usertype(tolua_S,"cPacket_Login");
 tolua_usertype(tolua_S,"cClientHandle");
 tolua_usertype(tolua_S,"Lua__cPacket_BlockDig");
 tolua_usertype(tolua_S,"cFurnaceRecipe");
 tolua_usertype(tolua_S,"cCuboid");
 tolua_usertype(tolua_S,"cChatColor");
 tolua_usertype(tolua_S,"cMCLogger");
 tolua_usertype(tolua_S,"cPacket_PickupSpawn");
 tolua_usertype(tolua_S,"Lua__cWebPlugin");
 tolua_usertype(tolua_S,"Lua__cPawn");
 tolua_usertype(tolua_S,"cGroup");
 tolua_usertype(tolua_S,"cItem");
 tolua_usertype(tolua_S,"Vector3f");
 tolua_usertype(tolua_S,"cPlugin_Lua");
 tolua_usertype(tolua_S,"cTracer");
 tolua_usertype(tolua_S,"Lua__cPlayer");
 tolua_usertype(tolua_S,"cPacket");
 tolua_usertype(tolua_S,"cPacket_BlockDig");
 tolua_usertype(tolua_S,"cWebAdmin");
 tolua_usertype(tolua_S,"Vector3i");
 tolua_usertype(tolua_S,"cBlockEntity");
 tolua_usertype(tolua_S,"cWorld");
 tolua_usertype(tolua_S,"Lua__cTCPLink");
 tolua_usertype(tolua_S,"Lua__cPlugin");
 tolua_usertype(tolua_S,"cTCPLink");
 tolua_usertype(tolua_S,"cPacket_BlockPlace");
 tolua_usertype(tolua_S,"cLadder");
 tolua_usertype(tolua_S,"cGroupManager");
 tolua_usertype(tolua_S,"cPluginManager");
 tolua_usertype(tolua_S,"cIniFile");
 tolua_usertype(tolua_S,"cWebPlugin");
 tolua_usertype(tolua_S,"HTTPRequest");
 tolua_usertype(tolua_S,"cPawn");
 tolua_usertype(tolua_S,"cPlayer");
 tolua_usertype(tolua_S,"cTorch");
 tolua_usertype(tolua_S,"Lua__cPickup");
 tolua_usertype(tolua_S,"cEntity");
 tolua_usertype(tolua_S,"cInventory");
 tolua_usertype(tolua_S,"Lua__cEntity");
 tolua_usertype(tolua_S,"cStairs");
 tolua_usertype(tolua_S,"Vector3d");
}

/* method: DirectionToMetaData of class  cTorch */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cTorch_DirectionToMetaData00
static int tolua_AllToLua_cTorch_DirectionToMetaData00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"cTorch",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  char a_Direction = ((char)  tolua_tonumber(tolua_S,2,0));
  {
   char tolua_ret = (char)  cTorch::DirectionToMetaData(a_Direction);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'DirectionToMetaData'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: MetaDataToDirection of class  cTorch */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cTorch_MetaDataToDirection00
static int tolua_AllToLua_cTorch_MetaDataToDirection00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"cTorch",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  char a_MetaData = ((char)  tolua_tonumber(tolua_S,2,0));
  {
   char tolua_ret = (char)  cTorch::MetaDataToDirection(a_MetaData);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'MetaDataToDirection'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: RotationToMetaData of class  cStairs */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cStairs_RotationToMetaData00
static int tolua_AllToLua_cStairs_RotationToMetaData00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"cStairs",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  float a_Rotation = ((float)  tolua_tonumber(tolua_S,2,0));
  {
   char tolua_ret = (char)  cStairs::RotationToMetaData(a_Rotation);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'RotationToMetaData'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: DirectionToMetaData of class  cLadder */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cLadder_DirectionToMetaData00
static int tolua_AllToLua_cLadder_DirectionToMetaData00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"cLadder",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  char a_Direction = ((char)  tolua_tonumber(tolua_S,2,0));
  {
   char tolua_ret = (char)  cLadder::DirectionToMetaData(a_Direction);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'DirectionToMetaData'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: MetaDataToDirection of class  cLadder */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cLadder_MetaDataToDirection00
static int tolua_AllToLua_cLadder_MetaDataToDirection00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"cLadder",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  char a_MetaData = ((char)  tolua_tonumber(tolua_S,2,0));
  {
   char tolua_ret = (char)  cLadder::MetaDataToDirection(a_MetaData);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'MetaDataToDirection'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  cIniFile */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cIniFile_new00
static int tolua_AllToLua_cIniFile_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"cIniFile",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const std::string iniPath = ((const std::string)  tolua_tocppstring(tolua_S,2,""));
  {
   cIniFile* tolua_ret = (cIniFile*)  Mtolua_new((cIniFile)(iniPath));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"cIniFile");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  cIniFile */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cIniFile_new00_local
static int tolua_AllToLua_cIniFile_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"cIniFile",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const std::string iniPath = ((const std::string)  tolua_tocppstring(tolua_S,2,""));
  {
   cIniFile* tolua_ret = (cIniFile*)  Mtolua_new((cIniFile)(iniPath));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"cIniFile");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: CaseSensitive of class  cIniFile */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cIniFile_CaseSensitive00
static int tolua_AllToLua_cIniFile_CaseSensitive00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cIniFile",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cIniFile* self = (cIniFile*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'CaseSensitive'", NULL);
#endif
  {
   self->CaseSensitive();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'CaseSensitive'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: CaseInsensitive of class  cIniFile */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cIniFile_CaseInsensitive00
static int tolua_AllToLua_cIniFile_CaseInsensitive00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cIniFile",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cIniFile* self = (cIniFile*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'CaseInsensitive'", NULL);
#endif
  {
   self->CaseInsensitive();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'CaseInsensitive'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: Path of class  cIniFile */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cIniFile_Path00
static int tolua_AllToLua_cIniFile_Path00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cIniFile",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cIniFile* self = (cIniFile*)  tolua_tousertype(tolua_S,1,0);
  const std::string newPath = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'Path'", NULL);
#endif
  {
   self->Path(newPath);
   tolua_pushcppstring(tolua_S,(const char*)newPath);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Path'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: Path of class  cIniFile */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cIniFile_Path01
static int tolua_AllToLua_cIniFile_Path01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cIniFile",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  const cIniFile* self = (const cIniFile*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'Path'", NULL);
#endif
  {
   std::string tolua_ret = (std::string)  self->Path();
   tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
tolua_lerror:
 return tolua_AllToLua_cIniFile_Path00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetPath of class  cIniFile */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cIniFile_SetPath00
static int tolua_AllToLua_cIniFile_SetPath00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cIniFile",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cIniFile* self = (cIniFile*)  tolua_tousertype(tolua_S,1,0);
  const std::string newPath = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetPath'", NULL);
#endif
  {
   self->SetPath(newPath);
   tolua_pushcppstring(tolua_S,(const char*)newPath);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetPath'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: ReadFile of class  cIniFile */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cIniFile_ReadFile00
static int tolua_AllToLua_cIniFile_ReadFile00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cIniFile",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cIniFile* self = (cIniFile*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'ReadFile'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->ReadFile();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'ReadFile'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: WriteFile of class  cIniFile */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cIniFile_WriteFile00
static int tolua_AllToLua_cIniFile_WriteFile00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cIniFile",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cIniFile* self = (cIniFile*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'WriteFile'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->WriteFile();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'WriteFile'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: Erase of class  cIniFile */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cIniFile_Erase00
static int tolua_AllToLua_cIniFile_Erase00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cIniFile",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cIniFile* self = (cIniFile*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'Erase'", NULL);
#endif
  {
   self->Erase();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Erase'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: Clear of class  cIniFile */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cIniFile_Clear00
static int tolua_AllToLua_cIniFile_Clear00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cIniFile",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cIniFile* self = (cIniFile*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'Clear'", NULL);
#endif
  {
   self->Clear();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Clear'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: Reset of class  cIniFile */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cIniFile_Reset00
static int tolua_AllToLua_cIniFile_Reset00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cIniFile",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cIniFile* self = (cIniFile*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'Reset'", NULL);
#endif
  {
   self->Reset();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Reset'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: FindKey of class  cIniFile */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cIniFile_FindKey00
static int tolua_AllToLua_cIniFile_FindKey00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cIniFile",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cIniFile* self = (const cIniFile*)  tolua_tousertype(tolua_S,1,0);
  const std::string keyname = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'FindKey'", NULL);
#endif
  {
   long tolua_ret = (long)  self->FindKey(keyname);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
   tolua_pushcppstring(tolua_S,(const char*)keyname);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'FindKey'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: FindValue of class  cIniFile */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cIniFile_FindValue00
static int tolua_AllToLua_cIniFile_FindValue00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cIniFile",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cIniFile* self = (const cIniFile*)  tolua_tousertype(tolua_S,1,0);
  const unsigned keyID = ((const unsigned)  tolua_tonumber(tolua_S,2,0));
  const std::string valuename = ((const std::string)  tolua_tocppstring(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'FindValue'", NULL);
#endif
  {
   long tolua_ret = (long)  self->FindValue(keyID,valuename);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
   tolua_pushcppstring(tolua_S,(const char*)valuename);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'FindValue'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: NumKeys of class  cIniFile */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cIniFile_NumKeys00
static int tolua_AllToLua_cIniFile_NumKeys00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cIniFile",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cIniFile* self = (const cIniFile*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'NumKeys'", NULL);
#endif
  {
   unsigned tolua_ret = (unsigned)  self->NumKeys();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'NumKeys'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetNumKeys of class  cIniFile */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cIniFile_GetNumKeys00
static int tolua_AllToLua_cIniFile_GetNumKeys00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cIniFile",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cIniFile* self = (const cIniFile*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetNumKeys'", NULL);
#endif
  {
   unsigned tolua_ret = (unsigned)  self->GetNumKeys();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetNumKeys'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: AddKeyName of class  cIniFile */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cIniFile_AddKeyName00
static int tolua_AllToLua_cIniFile_AddKeyName00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cIniFile",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cIniFile* self = (cIniFile*)  tolua_tousertype(tolua_S,1,0);
  const std::string keyname = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'AddKeyName'", NULL);
#endif
  {
   unsigned tolua_ret = (unsigned)  self->AddKeyName(keyname);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
   tolua_pushcppstring(tolua_S,(const char*)keyname);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'AddKeyName'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: KeyName of class  cIniFile */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cIniFile_KeyName00
static int tolua_AllToLua_cIniFile_KeyName00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cIniFile",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cIniFile* self = (const cIniFile*)  tolua_tousertype(tolua_S,1,0);
  const unsigned keyID = ((const unsigned)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'KeyName'", NULL);
#endif
  {
   std::string tolua_ret = (std::string)  self->KeyName(keyID);
   tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'KeyName'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetKeyName of class  cIniFile */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cIniFile_GetKeyName00
static int tolua_AllToLua_cIniFile_GetKeyName00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cIniFile",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cIniFile* self = (const cIniFile*)  tolua_tousertype(tolua_S,1,0);
  const unsigned keyID = ((const unsigned)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetKeyName'", NULL);
#endif
  {
   std::string tolua_ret = (std::string)  self->GetKeyName(keyID);
   tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetKeyName'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: NumValues of class  cIniFile */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cIniFile_NumValues00
static int tolua_AllToLua_cIniFile_NumValues00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cIniFile",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cIniFile* self = (cIniFile*)  tolua_tousertype(tolua_S,1,0);
  const std::string keyname = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'NumValues'", NULL);
#endif
  {
   unsigned tolua_ret = (unsigned)  self->NumValues(keyname);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
   tolua_pushcppstring(tolua_S,(const char*)keyname);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'NumValues'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetNumValues of class  cIniFile */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cIniFile_GetNumValues00
static int tolua_AllToLua_cIniFile_GetNumValues00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cIniFile",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cIniFile* self = (cIniFile*)  tolua_tousertype(tolua_S,1,0);
  const std::string keyname = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetNumValues'", NULL);
#endif
  {
   unsigned tolua_ret = (unsigned)  self->GetNumValues(keyname);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
   tolua_pushcppstring(tolua_S,(const char*)keyname);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetNumValues'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: NumValues of class  cIniFile */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cIniFile_NumValues01
static int tolua_AllToLua_cIniFile_NumValues01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cIniFile",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  cIniFile* self = (cIniFile*)  tolua_tousertype(tolua_S,1,0);
  const unsigned keyID = ((const unsigned)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'NumValues'", NULL);
#endif
  {
   unsigned tolua_ret = (unsigned)  self->NumValues(keyID);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
tolua_lerror:
 return tolua_AllToLua_cIniFile_NumValues00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetNumValues of class  cIniFile */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cIniFile_GetNumValues01
static int tolua_AllToLua_cIniFile_GetNumValues01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cIniFile",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  cIniFile* self = (cIniFile*)  tolua_tousertype(tolua_S,1,0);
  const unsigned keyID = ((const unsigned)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetNumValues'", NULL);
#endif
  {
   unsigned tolua_ret = (unsigned)  self->GetNumValues(keyID);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
tolua_lerror:
 return tolua_AllToLua_cIniFile_GetNumValues00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: ValueName of class  cIniFile */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cIniFile_ValueName00
static int tolua_AllToLua_cIniFile_ValueName00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cIniFile",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cIniFile* self = (const cIniFile*)  tolua_tousertype(tolua_S,1,0);
  const std::string keyname = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
  const unsigned valueID = ((const unsigned)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'ValueName'", NULL);
#endif
  {
   std::string tolua_ret = (std::string)  self->ValueName(keyname,valueID);
   tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
   tolua_pushcppstring(tolua_S,(const char*)keyname);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'ValueName'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetValueName of class  cIniFile */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cIniFile_GetValueName00
static int tolua_AllToLua_cIniFile_GetValueName00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cIniFile",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cIniFile* self = (const cIniFile*)  tolua_tousertype(tolua_S,1,0);
  const std::string keyname = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
  const unsigned valueID = ((const unsigned)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetValueName'", NULL);
#endif
  {
   std::string tolua_ret = (std::string)  self->GetValueName(keyname,valueID);
   tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
   tolua_pushcppstring(tolua_S,(const char*)keyname);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetValueName'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: ValueName of class  cIniFile */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cIniFile_ValueName01
static int tolua_AllToLua_cIniFile_ValueName01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cIniFile",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  const cIniFile* self = (const cIniFile*)  tolua_tousertype(tolua_S,1,0);
  const unsigned keyID = ((const unsigned)  tolua_tonumber(tolua_S,2,0));
  const unsigned valueID = ((const unsigned)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'ValueName'", NULL);
#endif
  {
   std::string tolua_ret = (std::string)  self->ValueName(keyID,valueID);
   tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
tolua_lerror:
 return tolua_AllToLua_cIniFile_ValueName00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetValueName of class  cIniFile */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cIniFile_GetValueName01
static int tolua_AllToLua_cIniFile_GetValueName01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cIniFile",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  const cIniFile* self = (const cIniFile*)  tolua_tousertype(tolua_S,1,0);
  const unsigned keyID = ((const unsigned)  tolua_tonumber(tolua_S,2,0));
  const unsigned valueID = ((const unsigned)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetValueName'", NULL);
#endif
  {
   std::string tolua_ret = (std::string)  self->GetValueName(keyID,valueID);
   tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
tolua_lerror:
 return tolua_AllToLua_cIniFile_GetValueName00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetValue of class  cIniFile */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cIniFile_GetValue00
static int tolua_AllToLua_cIniFile_GetValue00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cIniFile",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cIniFile* self = (const cIniFile*)  tolua_tousertype(tolua_S,1,0);
  const std::string keyname = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
  const std::string valuename = ((const std::string)  tolua_tocppstring(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetValue'", NULL);
#endif
  {
   std::string tolua_ret = (std::string)  self->GetValue(keyname,valuename);
   tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
   tolua_pushcppstring(tolua_S,(const char*)keyname);
   tolua_pushcppstring(tolua_S,(const char*)valuename);
  }
 }
 return 3;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetValue'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetValue of class  cIniFile */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cIniFile_GetValue01
static int tolua_AllToLua_cIniFile_GetValue01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cIniFile",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,3,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  const cIniFile* self = (const cIniFile*)  tolua_tousertype(tolua_S,1,0);
  const std::string keyname = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
  const std::string valuename = ((const std::string)  tolua_tocppstring(tolua_S,3,0));
  const std::string defValue = ((const std::string)  tolua_tocppstring(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetValue'", NULL);
#endif
  {
   std::string tolua_ret = (std::string)  self->GetValue(keyname,valuename,defValue);
   tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
   tolua_pushcppstring(tolua_S,(const char*)keyname);
   tolua_pushcppstring(tolua_S,(const char*)valuename);
   tolua_pushcppstring(tolua_S,(const char*)defValue);
  }
 }
 return 4;
tolua_lerror:
 return tolua_AllToLua_cIniFile_GetValue00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetValue of class  cIniFile */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cIniFile_GetValue02
static int tolua_AllToLua_cIniFile_GetValue02(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cIniFile",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  const cIniFile* self = (const cIniFile*)  tolua_tousertype(tolua_S,1,0);
  const unsigned keyID = ((const unsigned)  tolua_tonumber(tolua_S,2,0));
  const unsigned valueID = ((const unsigned)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetValue'", NULL);
#endif
  {
   std::string tolua_ret = (std::string)  self->GetValue(keyID,valueID);
   tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
tolua_lerror:
 return tolua_AllToLua_cIniFile_GetValue01(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetValue of class  cIniFile */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cIniFile_GetValue03
static int tolua_AllToLua_cIniFile_GetValue03(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cIniFile",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  const cIniFile* self = (const cIniFile*)  tolua_tousertype(tolua_S,1,0);
  const unsigned keyID = ((const unsigned)  tolua_tonumber(tolua_S,2,0));
  const unsigned valueID = ((const unsigned)  tolua_tonumber(tolua_S,3,0));
  const std::string defValue = ((const std::string)  tolua_tocppstring(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetValue'", NULL);
#endif
  {
   std::string tolua_ret = (std::string)  self->GetValue(keyID,valueID,defValue);
   tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
   tolua_pushcppstring(tolua_S,(const char*)defValue);
  }
 }
 return 2;
tolua_lerror:
 return tolua_AllToLua_cIniFile_GetValue02(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetValueI of class  cIniFile */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cIniFile_GetValueI00
static int tolua_AllToLua_cIniFile_GetValueI00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cIniFile",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cIniFile* self = (const cIniFile*)  tolua_tousertype(tolua_S,1,0);
  const std::string keyname = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
  const std::string valuename = ((const std::string)  tolua_tocppstring(tolua_S,3,0));
  const int defValue = ((const int)  tolua_tonumber(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetValueI'", NULL);
#endif
  {
   int tolua_ret = (int)  self->GetValueI(keyname,valuename,defValue);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
   tolua_pushcppstring(tolua_S,(const char*)keyname);
   tolua_pushcppstring(tolua_S,(const char*)valuename);
  }
 }
 return 3;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetValueI'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetValueB of class  cIniFile */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cIniFile_GetValueB00
static int tolua_AllToLua_cIniFile_GetValueB00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cIniFile",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isboolean(tolua_S,4,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cIniFile* self = (const cIniFile*)  tolua_tousertype(tolua_S,1,0);
  const std::string keyname = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
  const std::string valuename = ((const std::string)  tolua_tocppstring(tolua_S,3,0));
  const bool defValue = ((const bool)  tolua_toboolean(tolua_S,4,false));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetValueB'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->GetValueB(keyname,valuename,defValue);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
   tolua_pushcppstring(tolua_S,(const char*)keyname);
   tolua_pushcppstring(tolua_S,(const char*)valuename);
  }
 }
 return 3;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetValueB'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetValueF of class  cIniFile */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cIniFile_GetValueF00
static int tolua_AllToLua_cIniFile_GetValueF00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cIniFile",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cIniFile* self = (const cIniFile*)  tolua_tousertype(tolua_S,1,0);
  const std::string keyname = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
  const std::string valuename = ((const std::string)  tolua_tocppstring(tolua_S,3,0));
  const double defValue = ((const double)  tolua_tonumber(tolua_S,4,0.0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetValueF'", NULL);
#endif
  {
   double tolua_ret = (double)  self->GetValueF(keyname,valuename,defValue);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
   tolua_pushcppstring(tolua_S,(const char*)keyname);
   tolua_pushcppstring(tolua_S,(const char*)valuename);
  }
 }
 return 3;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetValueF'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetValue of class  cIniFile */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cIniFile_SetValue00
static int tolua_AllToLua_cIniFile_SetValue00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cIniFile",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cIniFile* self = (cIniFile*)  tolua_tousertype(tolua_S,1,0);
  const unsigned keyID = ((const unsigned)  tolua_tonumber(tolua_S,2,0));
  const unsigned valueID = ((const unsigned)  tolua_tonumber(tolua_S,3,0));
  const std::string value = ((const std::string)  tolua_tocppstring(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetValue'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->SetValue(keyID,valueID,value);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
   tolua_pushcppstring(tolua_S,(const char*)value);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetValue'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetValue of class  cIniFile */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cIniFile_SetValue01
static int tolua_AllToLua_cIniFile_SetValue01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cIniFile",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,3,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,4,0,&tolua_err) ||
     !tolua_isboolean(tolua_S,5,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  cIniFile* self = (cIniFile*)  tolua_tousertype(tolua_S,1,0);
  const std::string keyname = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
  const std::string valuename = ((const std::string)  tolua_tocppstring(tolua_S,3,0));
  const std::string value = ((const std::string)  tolua_tocppstring(tolua_S,4,0));
  const bool create = ((const bool)  tolua_toboolean(tolua_S,5,true));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetValue'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->SetValue(keyname,valuename,value,create);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
   tolua_pushcppstring(tolua_S,(const char*)keyname);
   tolua_pushcppstring(tolua_S,(const char*)valuename);
   tolua_pushcppstring(tolua_S,(const char*)value);
  }
 }
 return 4;
tolua_lerror:
 return tolua_AllToLua_cIniFile_SetValue00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetValueI of class  cIniFile */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cIniFile_SetValueI00
static int tolua_AllToLua_cIniFile_SetValueI00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cIniFile",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isboolean(tolua_S,5,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cIniFile* self = (cIniFile*)  tolua_tousertype(tolua_S,1,0);
  const std::string keyname = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
  const std::string valuename = ((const std::string)  tolua_tocppstring(tolua_S,3,0));
  const int value = ((const int)  tolua_tonumber(tolua_S,4,0));
  const bool create = ((const bool)  tolua_toboolean(tolua_S,5,true));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetValueI'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->SetValueI(keyname,valuename,value,create);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
   tolua_pushcppstring(tolua_S,(const char*)keyname);
   tolua_pushcppstring(tolua_S,(const char*)valuename);
  }
 }
 return 3;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetValueI'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetValueB of class  cIniFile */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cIniFile_SetValueB00
static int tolua_AllToLua_cIniFile_SetValueB00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cIniFile",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isboolean(tolua_S,4,0,&tolua_err) ||
     !tolua_isboolean(tolua_S,5,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cIniFile* self = (cIniFile*)  tolua_tousertype(tolua_S,1,0);
  const std::string keyname = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
  const std::string valuename = ((const std::string)  tolua_tocppstring(tolua_S,3,0));
  const bool value = ((const bool)  tolua_toboolean(tolua_S,4,0));
  const bool create = ((const bool)  tolua_toboolean(tolua_S,5,true));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetValueB'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->SetValueB(keyname,valuename,value,create);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
   tolua_pushcppstring(tolua_S,(const char*)keyname);
   tolua_pushcppstring(tolua_S,(const char*)valuename);
  }
 }
 return 3;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetValueB'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetValueF of class  cIniFile */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cIniFile_SetValueF00
static int tolua_AllToLua_cIniFile_SetValueF00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cIniFile",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isboolean(tolua_S,5,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cIniFile* self = (cIniFile*)  tolua_tousertype(tolua_S,1,0);
  const std::string keyname = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
  const std::string valuename = ((const std::string)  tolua_tocppstring(tolua_S,3,0));
  const double value = ((const double)  tolua_tonumber(tolua_S,4,0));
  const bool create = ((const bool)  tolua_toboolean(tolua_S,5,true));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetValueF'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->SetValueF(keyname,valuename,value,create);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
   tolua_pushcppstring(tolua_S,(const char*)keyname);
   tolua_pushcppstring(tolua_S,(const char*)valuename);
  }
 }
 return 3;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetValueF'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: DeleteValue of class  cIniFile */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cIniFile_DeleteValue00
static int tolua_AllToLua_cIniFile_DeleteValue00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cIniFile",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cIniFile* self = (cIniFile*)  tolua_tousertype(tolua_S,1,0);
  const std::string keyname = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
  const std::string valuename = ((const std::string)  tolua_tocppstring(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'DeleteValue'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->DeleteValue(keyname,valuename);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
   tolua_pushcppstring(tolua_S,(const char*)keyname);
   tolua_pushcppstring(tolua_S,(const char*)valuename);
  }
 }
 return 3;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'DeleteValue'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: DeleteKey of class  cIniFile */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cIniFile_DeleteKey00
static int tolua_AllToLua_cIniFile_DeleteKey00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cIniFile",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cIniFile* self = (cIniFile*)  tolua_tousertype(tolua_S,1,0);
  const std::string keyname = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'DeleteKey'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->DeleteKey(keyname);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
   tolua_pushcppstring(tolua_S,(const char*)keyname);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'DeleteKey'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: NumHeaderComments of class  cIniFile */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cIniFile_NumHeaderComments00
static int tolua_AllToLua_cIniFile_NumHeaderComments00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cIniFile",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cIniFile* self = (cIniFile*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'NumHeaderComments'", NULL);
#endif
  {
   unsigned tolua_ret = (unsigned)  self->NumHeaderComments();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'NumHeaderComments'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: HeaderComment of class  cIniFile */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cIniFile_HeaderComment00
static int tolua_AllToLua_cIniFile_HeaderComment00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cIniFile",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cIniFile* self = (cIniFile*)  tolua_tousertype(tolua_S,1,0);
  const std::string comment = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'HeaderComment'", NULL);
#endif
  {
   self->HeaderComment(comment);
   tolua_pushcppstring(tolua_S,(const char*)comment);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'HeaderComment'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: HeaderComment of class  cIniFile */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cIniFile_HeaderComment01
static int tolua_AllToLua_cIniFile_HeaderComment01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cIniFile",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  const cIniFile* self = (const cIniFile*)  tolua_tousertype(tolua_S,1,0);
  const unsigned commentID = ((const unsigned)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'HeaderComment'", NULL);
#endif
  {
   std::string tolua_ret = (std::string)  self->HeaderComment(commentID);
   tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
tolua_lerror:
 return tolua_AllToLua_cIniFile_HeaderComment00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: DeleteHeaderComment of class  cIniFile */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cIniFile_DeleteHeaderComment00
static int tolua_AllToLua_cIniFile_DeleteHeaderComment00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cIniFile",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cIniFile* self = (cIniFile*)  tolua_tousertype(tolua_S,1,0);
  unsigned commentID = ((unsigned)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'DeleteHeaderComment'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->DeleteHeaderComment(commentID);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'DeleteHeaderComment'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: DeleteHeaderComments of class  cIniFile */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cIniFile_DeleteHeaderComments00
static int tolua_AllToLua_cIniFile_DeleteHeaderComments00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cIniFile",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cIniFile* self = (cIniFile*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'DeleteHeaderComments'", NULL);
#endif
  {
   self->DeleteHeaderComments();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'DeleteHeaderComments'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: NumKeyComments of class  cIniFile */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cIniFile_NumKeyComments00
static int tolua_AllToLua_cIniFile_NumKeyComments00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cIniFile",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cIniFile* self = (const cIniFile*)  tolua_tousertype(tolua_S,1,0);
  const unsigned keyID = ((const unsigned)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'NumKeyComments'", NULL);
#endif
  {
   unsigned tolua_ret = (unsigned)  self->NumKeyComments(keyID);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'NumKeyComments'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: NumKeyComments of class  cIniFile */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cIniFile_NumKeyComments01
static int tolua_AllToLua_cIniFile_NumKeyComments01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cIniFile",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  const cIniFile* self = (const cIniFile*)  tolua_tousertype(tolua_S,1,0);
  const std::string keyname = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'NumKeyComments'", NULL);
#endif
  {
   unsigned tolua_ret = (unsigned)  self->NumKeyComments(keyname);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
   tolua_pushcppstring(tolua_S,(const char*)keyname);
  }
 }
 return 2;
tolua_lerror:
 return tolua_AllToLua_cIniFile_NumKeyComments00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: KeyComment of class  cIniFile */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cIniFile_KeyComment00
static int tolua_AllToLua_cIniFile_KeyComment00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cIniFile",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cIniFile* self = (cIniFile*)  tolua_tousertype(tolua_S,1,0);
  const unsigned keyID = ((const unsigned)  tolua_tonumber(tolua_S,2,0));
  const std::string comment = ((const std::string)  tolua_tocppstring(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'KeyComment'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->KeyComment(keyID,comment);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
   tolua_pushcppstring(tolua_S,(const char*)comment);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'KeyComment'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: KeyComment of class  cIniFile */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cIniFile_KeyComment01
static int tolua_AllToLua_cIniFile_KeyComment01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cIniFile",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  cIniFile* self = (cIniFile*)  tolua_tousertype(tolua_S,1,0);
  const std::string keyname = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
  const std::string comment = ((const std::string)  tolua_tocppstring(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'KeyComment'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->KeyComment(keyname,comment);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
   tolua_pushcppstring(tolua_S,(const char*)keyname);
   tolua_pushcppstring(tolua_S,(const char*)comment);
  }
 }
 return 3;
tolua_lerror:
 return tolua_AllToLua_cIniFile_KeyComment00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: KeyComment of class  cIniFile */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cIniFile_KeyComment02
static int tolua_AllToLua_cIniFile_KeyComment02(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cIniFile",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  const cIniFile* self = (const cIniFile*)  tolua_tousertype(tolua_S,1,0);
  const unsigned keyID = ((const unsigned)  tolua_tonumber(tolua_S,2,0));
  const unsigned commentID = ((const unsigned)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'KeyComment'", NULL);
#endif
  {
   std::string tolua_ret = (std::string)  self->KeyComment(keyID,commentID);
   tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
tolua_lerror:
 return tolua_AllToLua_cIniFile_KeyComment01(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: KeyComment of class  cIniFile */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cIniFile_KeyComment03
static int tolua_AllToLua_cIniFile_KeyComment03(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cIniFile",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  const cIniFile* self = (const cIniFile*)  tolua_tousertype(tolua_S,1,0);
  const std::string keyname = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
  const unsigned commentID = ((const unsigned)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'KeyComment'", NULL);
#endif
  {
   std::string tolua_ret = (std::string)  self->KeyComment(keyname,commentID);
   tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
   tolua_pushcppstring(tolua_S,(const char*)keyname);
  }
 }
 return 2;
tolua_lerror:
 return tolua_AllToLua_cIniFile_KeyComment02(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: DeleteKeyComment of class  cIniFile */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cIniFile_DeleteKeyComment00
static int tolua_AllToLua_cIniFile_DeleteKeyComment00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cIniFile",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cIniFile* self = (cIniFile*)  tolua_tousertype(tolua_S,1,0);
  const unsigned keyID = ((const unsigned)  tolua_tonumber(tolua_S,2,0));
  const unsigned commentID = ((const unsigned)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'DeleteKeyComment'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->DeleteKeyComment(keyID,commentID);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'DeleteKeyComment'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: DeleteKeyComment of class  cIniFile */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cIniFile_DeleteKeyComment01
static int tolua_AllToLua_cIniFile_DeleteKeyComment01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cIniFile",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  cIniFile* self = (cIniFile*)  tolua_tousertype(tolua_S,1,0);
  const std::string keyname = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
  const unsigned commentID = ((const unsigned)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'DeleteKeyComment'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->DeleteKeyComment(keyname,commentID);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
   tolua_pushcppstring(tolua_S,(const char*)keyname);
  }
 }
 return 2;
tolua_lerror:
 return tolua_AllToLua_cIniFile_DeleteKeyComment00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: DeleteKeyComments of class  cIniFile */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cIniFile_DeleteKeyComments00
static int tolua_AllToLua_cIniFile_DeleteKeyComments00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cIniFile",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cIniFile* self = (cIniFile*)  tolua_tousertype(tolua_S,1,0);
  const unsigned keyID = ((const unsigned)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'DeleteKeyComments'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->DeleteKeyComments(keyID);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'DeleteKeyComments'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: DeleteKeyComments of class  cIniFile */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cIniFile_DeleteKeyComments01
static int tolua_AllToLua_cIniFile_DeleteKeyComments01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cIniFile",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  cIniFile* self = (cIniFile*)  tolua_tousertype(tolua_S,1,0);
  const std::string keyname = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'DeleteKeyComments'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->DeleteKeyComments(keyname);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
   tolua_pushcppstring(tolua_S,(const char*)keyname);
  }
 }
 return 2;
tolua_lerror:
 return tolua_AllToLua_cIniFile_DeleteKeyComments00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

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
 tolua_pushnumber(tolua_S,(lua_Number)g_BlockLightValue[tolua_index]);
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
  g_BlockLightValue[tolua_index] = ((char)  tolua_tonumber(tolua_S,3,0));
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
 tolua_pushnumber(tolua_S,(lua_Number)g_BlockSpreadLightFalloff[tolua_index]);
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
  g_BlockSpreadLightFalloff[tolua_index] = ((char)  tolua_tonumber(tolua_S,3,0));
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
 tolua_pushboolean(tolua_S,(bool)g_BlockTransparent[tolua_index]);
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
  g_BlockTransparent[tolua_index] = ((bool)  tolua_toboolean(tolua_S,3,0));
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
 tolua_pushboolean(tolua_S,(bool)g_BlockOneHitDig[tolua_index]);
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
  g_BlockOneHitDig[tolua_index] = ((bool)  tolua_toboolean(tolua_S,3,0));
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* function: IsValidBlock */
#ifndef TOLUA_DISABLE_tolua_AllToLua_IsValidBlock00
static int tolua_AllToLua_IsValidBlock00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnumber(tolua_S,1,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  int a_BlockID = ((int)  tolua_tonumber(tolua_S,1,0));
  {
   bool tolua_ret = (bool)  IsValidBlock(a_BlockID);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'IsValidBlock'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: isValidItem */
#ifndef TOLUA_DISABLE_tolua_AllToLua_isValidItem00
static int tolua_AllToLua_isValidItem00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnumber(tolua_S,1,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  int a_ItemID = ((int)  tolua_tonumber(tolua_S,1,0));
  {
   bool tolua_ret = (bool)  isValidItem(a_ItemID);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'isValidItem'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: AddDirection */
#ifndef TOLUA_DISABLE_tolua_AllToLua_AddDirection00
static int tolua_AllToLua_AddDirection00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnumber(tolua_S,1,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isboolean(tolua_S,5,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  int a_X = ((int)  tolua_tonumber(tolua_S,1,0));
  char a_Y = ((char)  tolua_tonumber(tolua_S,2,0));
  int a_Z = ((int)  tolua_tonumber(tolua_S,3,0));
  char a_Direction = ((char)  tolua_tonumber(tolua_S,4,0));
  bool a_bInverse = ((bool)  tolua_toboolean(tolua_S,5,false));
  {
   AddDirection(a_X,a_Y,a_Z,a_Direction,a_bInverse);
   tolua_pushnumber(tolua_S,(lua_Number)a_X);
   tolua_pushnumber(tolua_S,(lua_Number)a_Y);
   tolua_pushnumber(tolua_S,(lua_Number)a_Z);
  }
 }
 return 3;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'AddDirection'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: GetTime */
#ifndef TOLUA_DISABLE_tolua_AllToLua_GetTime00
static int tolua_AllToLua_GetTime00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnoobj(tolua_S,1,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   unsigned int tolua_ret = (unsigned int)  GetTime();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetTime'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: GetChar */
#ifndef TOLUA_DISABLE_tolua_AllToLua_GetChar00
static int tolua_AllToLua_GetChar00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_iscppstring(tolua_S,1,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  std::string a_Str = ((std::string)  tolua_tocppstring(tolua_S,1,0));
  unsigned int a_Idx = ((unsigned int)  tolua_tonumber(tolua_S,2,0));
  {
   std::string tolua_ret = (std::string)  GetChar(a_Str,a_Idx);
   tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
   tolua_pushcppstring(tolua_S,(const char*)a_Str);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetChar'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: clear of class  cStringMap */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cStringMap_clear00
static int tolua_AllToLua_cStringMap_clear00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cStringMap",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cStringMap* self = (cStringMap*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'clear'", NULL);
#endif
  {
   self->clear();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'clear'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: size of class  cStringMap */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cStringMap_size00
static int tolua_AllToLua_cStringMap_size00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cStringMap",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cStringMap* self = (const cStringMap*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'size'", NULL);
#endif
  {
   unsigned int tolua_ret = (unsigned int)  self->size();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'size'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: get of class  cStringMap */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cStringMap_get00
static int tolua_AllToLua_cStringMap_get00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cStringMap",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cStringMap* self = (cStringMap*)  tolua_tousertype(tolua_S,1,0);
  const std::string index = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'get'", NULL);
#endif
  {
   std::string tolua_ret = (std::string)  self->get(index);
   tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
   tolua_pushcppstring(tolua_S,(const char*)index);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* get function: Color of class  cChatColor */
#ifndef TOLUA_DISABLE_tolua_get_cChatColor_Color
static int tolua_get_cChatColor_Color(lua_State* tolua_S)
{
  tolua_pushcppstring(tolua_S,(const char*)cChatColor::Color);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: Black of class  cChatColor */
#ifndef TOLUA_DISABLE_tolua_get_cChatColor_Black
static int tolua_get_cChatColor_Black(lua_State* tolua_S)
{
  tolua_pushcppstring(tolua_S,(const char*)cChatColor::Black);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: Navy of class  cChatColor */
#ifndef TOLUA_DISABLE_tolua_get_cChatColor_Navy
static int tolua_get_cChatColor_Navy(lua_State* tolua_S)
{
  tolua_pushcppstring(tolua_S,(const char*)cChatColor::Navy);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: Green of class  cChatColor */
#ifndef TOLUA_DISABLE_tolua_get_cChatColor_Green
static int tolua_get_cChatColor_Green(lua_State* tolua_S)
{
  tolua_pushcppstring(tolua_S,(const char*)cChatColor::Green);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: Blue of class  cChatColor */
#ifndef TOLUA_DISABLE_tolua_get_cChatColor_Blue
static int tolua_get_cChatColor_Blue(lua_State* tolua_S)
{
  tolua_pushcppstring(tolua_S,(const char*)cChatColor::Blue);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: Red of class  cChatColor */
#ifndef TOLUA_DISABLE_tolua_get_cChatColor_Red
static int tolua_get_cChatColor_Red(lua_State* tolua_S)
{
  tolua_pushcppstring(tolua_S,(const char*)cChatColor::Red);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: Purple of class  cChatColor */
#ifndef TOLUA_DISABLE_tolua_get_cChatColor_Purple
static int tolua_get_cChatColor_Purple(lua_State* tolua_S)
{
  tolua_pushcppstring(tolua_S,(const char*)cChatColor::Purple);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: Gold of class  cChatColor */
#ifndef TOLUA_DISABLE_tolua_get_cChatColor_Gold
static int tolua_get_cChatColor_Gold(lua_State* tolua_S)
{
  tolua_pushcppstring(tolua_S,(const char*)cChatColor::Gold);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: LightGray of class  cChatColor */
#ifndef TOLUA_DISABLE_tolua_get_cChatColor_LightGray
static int tolua_get_cChatColor_LightGray(lua_State* tolua_S)
{
  tolua_pushcppstring(tolua_S,(const char*)cChatColor::LightGray);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: Gray of class  cChatColor */
#ifndef TOLUA_DISABLE_tolua_get_cChatColor_Gray
static int tolua_get_cChatColor_Gray(lua_State* tolua_S)
{
  tolua_pushcppstring(tolua_S,(const char*)cChatColor::Gray);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: DarkPurple of class  cChatColor */
#ifndef TOLUA_DISABLE_tolua_get_cChatColor_DarkPurple
static int tolua_get_cChatColor_DarkPurple(lua_State* tolua_S)
{
  tolua_pushcppstring(tolua_S,(const char*)cChatColor::DarkPurple);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: LightGreen of class  cChatColor */
#ifndef TOLUA_DISABLE_tolua_get_cChatColor_LightGreen
static int tolua_get_cChatColor_LightGreen(lua_State* tolua_S)
{
  tolua_pushcppstring(tolua_S,(const char*)cChatColor::LightGreen);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: LightBlue of class  cChatColor */
#ifndef TOLUA_DISABLE_tolua_get_cChatColor_LightBlue
static int tolua_get_cChatColor_LightBlue(lua_State* tolua_S)
{
  tolua_pushcppstring(tolua_S,(const char*)cChatColor::LightBlue);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: Rose of class  cChatColor */
#ifndef TOLUA_DISABLE_tolua_get_cChatColor_Rose
static int tolua_get_cChatColor_Rose(lua_State* tolua_S)
{
  tolua_pushcppstring(tolua_S,(const char*)cChatColor::Rose);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: LightPurple of class  cChatColor */
#ifndef TOLUA_DISABLE_tolua_get_cChatColor_LightPurple
static int tolua_get_cChatColor_LightPurple(lua_State* tolua_S)
{
  tolua_pushcppstring(tolua_S,(const char*)cChatColor::LightPurple);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: Yellow of class  cChatColor */
#ifndef TOLUA_DISABLE_tolua_get_cChatColor_Yellow
static int tolua_get_cChatColor_Yellow(lua_State* tolua_S)
{
  tolua_pushcppstring(tolua_S,(const char*)cChatColor::Yellow);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: White of class  cChatColor */
#ifndef TOLUA_DISABLE_tolua_get_cChatColor_White
static int tolua_get_cChatColor_White(lua_State* tolua_S)
{
  tolua_pushcppstring(tolua_S,(const char*)cChatColor::White);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* method: MakeColor of class  cChatColor */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cChatColor_MakeColor00
static int tolua_AllToLua_cChatColor_MakeColor00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"cChatColor",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  char a_Color = ((char)  tolua_tonumber(tolua_S,2,0));
  {
   const std::string tolua_ret = (const std::string)  cChatColor::MakeColor(a_Color);
   tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'MakeColor'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetPlayer of class  cClientHandle */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cClientHandle_GetPlayer00
static int tolua_AllToLua_cClientHandle_GetPlayer00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cClientHandle",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cClientHandle* self = (cClientHandle*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetPlayer'", NULL);
#endif
  {
   cPlayer* tolua_ret = (cPlayer*)  self->GetPlayer();
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"cPlayer");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetPlayer'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: Kick of class  cClientHandle */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cClientHandle_Kick00
static int tolua_AllToLua_cClientHandle_Kick00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cClientHandle",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cClientHandle* self = (cClientHandle*)  tolua_tousertype(tolua_S,1,0);
  const char* a_Reason = ((const char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'Kick'", NULL);
#endif
  {
   self->Kick(a_Reason);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Kick'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  cEntity */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cEntity_delete00
static int tolua_AllToLua_cEntity_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cEntity",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cEntity* self = (cEntity*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'", NULL);
#endif
  Mtolua_delete(self);
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: Initialize of class  cEntity */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cEntity_Initialize00
static int tolua_AllToLua_cEntity_Initialize00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cEntity",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cEntity* self = (cEntity*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'Initialize'", NULL);
#endif
  {
   self->Initialize();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Initialize'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetEntityType of class  cEntity */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cEntity_GetEntityType00
static int tolua_AllToLua_cEntity_GetEntityType00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cEntity",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cEntity* self = (cEntity*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetEntityType'", NULL);
#endif
  {
   unsigned int tolua_ret = (unsigned int)  self->GetEntityType();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetEntityType'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: IsA of class  cEntity */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cEntity_IsA00
static int tolua_AllToLua_cEntity_IsA00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cEntity",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cEntity* self = (cEntity*)  tolua_tousertype(tolua_S,1,0);
  const char* a_EntityType = ((const char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'IsA'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->IsA(a_EntityType);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'IsA'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetClass of class  cEntity */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cEntity_GetClass00
static int tolua_AllToLua_cEntity_GetClass00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cEntity",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cEntity* self = (cEntity*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetClass'", NULL);
#endif
  {
   const char* tolua_ret = (const char*)  self->GetClass();
   tolua_pushstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetClass'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetPosition of class  cEntity */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cEntity_GetPosition00
static int tolua_AllToLua_cEntity_GetPosition00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cEntity",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cEntity* self = (cEntity*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetPosition'", NULL);
#endif
  {
   const Vector3d& tolua_ret = (const Vector3d&)  self->GetPosition();
    tolua_pushusertype(tolua_S,(void*)&tolua_ret,"const Vector3d");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetPosition'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetPosX of class  cEntity */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cEntity_GetPosX00
static int tolua_AllToLua_cEntity_GetPosX00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cEntity",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cEntity* self = (cEntity*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetPosX'", NULL);
#endif
  {
   const double tolua_ret = (const double)  self->GetPosX();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetPosX'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetPosY of class  cEntity */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cEntity_GetPosY00
static int tolua_AllToLua_cEntity_GetPosY00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cEntity",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cEntity* self = (cEntity*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetPosY'", NULL);
#endif
  {
   const double tolua_ret = (const double)  self->GetPosY();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetPosY'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetPosZ of class  cEntity */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cEntity_GetPosZ00
static int tolua_AllToLua_cEntity_GetPosZ00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cEntity",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cEntity* self = (cEntity*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetPosZ'", NULL);
#endif
  {
   const double tolua_ret = (const double)  self->GetPosZ();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetPosZ'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetRot of class  cEntity */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cEntity_GetRot00
static int tolua_AllToLua_cEntity_GetRot00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cEntity",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cEntity* self = (cEntity*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetRot'", NULL);
#endif
  {
   const Vector3f& tolua_ret = (const Vector3f&)  self->GetRot();
    tolua_pushusertype(tolua_S,(void*)&tolua_ret,"const Vector3f");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetRot'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetRotation of class  cEntity */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cEntity_GetRotation00
static int tolua_AllToLua_cEntity_GetRotation00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cEntity",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cEntity* self = (cEntity*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetRotation'", NULL);
#endif
  {
   float tolua_ret = (float)  self->GetRotation();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetRotation'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetPitch of class  cEntity */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cEntity_GetPitch00
static int tolua_AllToLua_cEntity_GetPitch00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cEntity",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cEntity* self = (cEntity*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetPitch'", NULL);
#endif
  {
   float tolua_ret = (float)  self->GetPitch();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetPitch'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetRoll of class  cEntity */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cEntity_GetRoll00
static int tolua_AllToLua_cEntity_GetRoll00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cEntity",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cEntity* self = (cEntity*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetRoll'", NULL);
#endif
  {
   float tolua_ret = (float)  self->GetRoll();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetRoll'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetLookVector of class  cEntity */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cEntity_GetLookVector00
static int tolua_AllToLua_cEntity_GetLookVector00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cEntity",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cEntity* self = (cEntity*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetLookVector'", NULL);
#endif
  {
   Vector3f tolua_ret = (Vector3f)  self->GetLookVector();
   {
#ifdef __cplusplus
    void* tolua_obj = Mtolua_new((Vector3f)(tolua_ret));
     tolua_pushusertype(tolua_S,tolua_obj,"Vector3f");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(Vector3f));
     tolua_pushusertype(tolua_S,tolua_obj,"Vector3f");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetLookVector'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetPosX of class  cEntity */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cEntity_SetPosX00
static int tolua_AllToLua_cEntity_SetPosX00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cEntity",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cEntity* self = (cEntity*)  tolua_tousertype(tolua_S,1,0);
  const double a_PosX = ((const double)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetPosX'", NULL);
#endif
  {
   self->SetPosX(a_PosX);
   tolua_pushnumber(tolua_S,(lua_Number)a_PosX);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetPosX'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetPosY of class  cEntity */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cEntity_SetPosY00
static int tolua_AllToLua_cEntity_SetPosY00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cEntity",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cEntity* self = (cEntity*)  tolua_tousertype(tolua_S,1,0);
  const double a_PosY = ((const double)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetPosY'", NULL);
#endif
  {
   self->SetPosY(a_PosY);
   tolua_pushnumber(tolua_S,(lua_Number)a_PosY);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetPosY'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetPosZ of class  cEntity */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cEntity_SetPosZ00
static int tolua_AllToLua_cEntity_SetPosZ00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cEntity",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cEntity* self = (cEntity*)  tolua_tousertype(tolua_S,1,0);
  const double a_PosZ = ((const double)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetPosZ'", NULL);
#endif
  {
   self->SetPosZ(a_PosZ);
   tolua_pushnumber(tolua_S,(lua_Number)a_PosZ);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetPosZ'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetPosition of class  cEntity */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cEntity_SetPosition00
static int tolua_AllToLua_cEntity_SetPosition00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cEntity",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cEntity* self = (cEntity*)  tolua_tousertype(tolua_S,1,0);
  const double a_PosX = ((const double)  tolua_tonumber(tolua_S,2,0));
  const double a_PosY = ((const double)  tolua_tonumber(tolua_S,3,0));
  const double a_PosZ = ((const double)  tolua_tonumber(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetPosition'", NULL);
#endif
  {
   self->SetPosition(a_PosX,a_PosY,a_PosZ);
   tolua_pushnumber(tolua_S,(lua_Number)a_PosX);
   tolua_pushnumber(tolua_S,(lua_Number)a_PosY);
   tolua_pushnumber(tolua_S,(lua_Number)a_PosZ);
  }
 }
 return 3;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetPosition'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetPosition of class  cEntity */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cEntity_SetPosition01
static int tolua_AllToLua_cEntity_SetPosition01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cEntity",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const Vector3d",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  cEntity* self = (cEntity*)  tolua_tousertype(tolua_S,1,0);
  const Vector3d* a_Pos = ((const Vector3d*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetPosition'", NULL);
#endif
  {
   self->SetPosition(*a_Pos);
  }
 }
 return 0;
tolua_lerror:
 return tolua_AllToLua_cEntity_SetPosition00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetRot of class  cEntity */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cEntity_SetRot00
static int tolua_AllToLua_cEntity_SetRot00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cEntity",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const Vector3f",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cEntity* self = (cEntity*)  tolua_tousertype(tolua_S,1,0);
  const Vector3f* a_Rot = ((const Vector3f*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetRot'", NULL);
#endif
  {
   self->SetRot(*a_Rot);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetRot'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetRotation of class  cEntity */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cEntity_SetRotation00
static int tolua_AllToLua_cEntity_SetRotation00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cEntity",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cEntity* self = (cEntity*)  tolua_tousertype(tolua_S,1,0);
  float a_Rotation = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetRotation'", NULL);
#endif
  {
   self->SetRotation(a_Rotation);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetRotation'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetPitch of class  cEntity */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cEntity_SetPitch00
static int tolua_AllToLua_cEntity_SetPitch00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cEntity",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cEntity* self = (cEntity*)  tolua_tousertype(tolua_S,1,0);
  float a_Pitch = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetPitch'", NULL);
#endif
  {
   self->SetPitch(a_Pitch);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetPitch'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetRoll of class  cEntity */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cEntity_SetRoll00
static int tolua_AllToLua_cEntity_SetRoll00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cEntity",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cEntity* self = (cEntity*)  tolua_tousertype(tolua_S,1,0);
  float a_Roll = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetRoll'", NULL);
#endif
  {
   self->SetRoll(a_Roll);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetRoll'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetUniqueID of class  cEntity */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cEntity_GetUniqueID00
static int tolua_AllToLua_cEntity_GetUniqueID00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cEntity",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cEntity* self = (cEntity*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetUniqueID'", NULL);
#endif
  {
   int tolua_ret = (int)  self->GetUniqueID();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetUniqueID'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: IsDestroyed of class  cEntity */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cEntity_IsDestroyed00
static int tolua_AllToLua_cEntity_IsDestroyed00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cEntity",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cEntity* self = (cEntity*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'IsDestroyed'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->IsDestroyed();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'IsDestroyed'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: Destroy of class  cEntity */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cEntity_Destroy00
static int tolua_AllToLua_cEntity_Destroy00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cEntity",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cEntity* self = (cEntity*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'Destroy'", NULL);
#endif
  {
   self->Destroy();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Destroy'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: Tick of class  cEntity */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cEntity_Tick00
static int tolua_AllToLua_cEntity_Tick00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cEntity",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cEntity* self = (cEntity*)  tolua_tousertype(tolua_S,1,0);
  float a_Dt = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'Tick'", NULL);
#endif
  {
   self->Tick(a_Dt);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Tick'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SpawnOn of class  cEntity */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cEntity_SpawnOn00
static int tolua_AllToLua_cEntity_SpawnOn00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cEntity",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"cClientHandle",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cEntity* self = (cEntity*)  tolua_tousertype(tolua_S,1,0);
  cClientHandle* a_Target = ((cClientHandle*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SpawnOn'", NULL);
#endif
  {
   self->SpawnOn(a_Target);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SpawnOn'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

 class Lua__cEntity : public cEntity, public ToluaBase {
public:
	 void  Initialize( void ) {
		if (push_method("Initialize",  tolua_AllToLua_cEntity_Initialize00)) {
			ToluaBase::dbcall(lua_state, 1, 0);
		} else {
			return ( void ) cEntity:: Initialize();
		};
	};
	unsigned int  GetEntityType( void ) {
		if (push_method("GetEntityType",  tolua_AllToLua_cEntity_GetEntityType00)) {
			ToluaBase::dbcall(lua_state, 1, 1);
			 unsigned int  tolua_ret = (unsigned int )tolua_tonumber(lua_state, -1, 0);
			lua_pop(lua_state, 1);
			return tolua_ret;
		} else {
			return (unsigned int ) cEntity:: GetEntityType();
		};
	};
	 bool  IsA( const char* a_EntityType) {
		if (push_method("IsA",  tolua_AllToLua_cEntity_IsA00)) {
			tolua_pushstring(lua_state, (const char*)a_EntityType);
			ToluaBase::dbcall(lua_state, 2, 1);
			 bool  tolua_ret = ( bool )tolua_toboolean(lua_state, -1, 0);
			lua_pop(lua_state, 1);
			return tolua_ret;
		} else {
			return ( bool ) cEntity:: IsA(a_EntityType);
		};
	};
	 const char*  GetClass( void ) {
		if (push_method("GetClass",  tolua_AllToLua_cEntity_GetClass00)) {
			ToluaBase::dbcall(lua_state, 1, 1);
			 const char*  tolua_ret = ( const char* )tolua_tostring(lua_state, -1, 0);
			lua_pop(lua_state, 1);
			return tolua_ret;
		} else {
			return ( const char* ) cEntity:: GetClass();
		};
	};
	 void  Tick( float a_Dt) {
		if (push_method("Tick",  tolua_AllToLua_cEntity_Tick00)) {
			tolua_pushnumber(lua_state, (lua_Number)a_Dt);
			ToluaBase::dbcall(lua_state, 2, 0);
		} else {
			if (lua_state)
				LOG("pure-virtual method cEntity::Tick not implemented.");
			else {
				LOG("pure-virtual method cEntity::Tick called with no lua_state. Aborting");
				::abort();
			};
			return ( void )0;
		};
	};
	 void  SpawnOn( cClientHandle* a_Target) {
		if (push_method("SpawnOn",  tolua_AllToLua_cEntity_SpawnOn00)) {
			tolua_pushusertype(lua_state, (void*)a_Target, "cClientHandle");
			ToluaBase::dbcall(lua_state, 2, 0);
		} else {
			if (lua_state)
				LOG("pure-virtual method cEntity::SpawnOn not implemented.");
			else {
				LOG("pure-virtual method cEntity::SpawnOn called with no lua_state. Aborting");
				::abort();
			};
			return ( void )0;
		};
	};

	 void cEntity__Initialize( void ) {
		return ( void )cEntity::Initialize();
	};
	unsigned int cEntity__GetEntityType( void ) {
		return (unsigned int )cEntity::GetEntityType();
	};
	 bool cEntity__IsA( const char* a_EntityType) {
		return ( bool )cEntity::IsA(a_EntityType);
	};
	 const char* cEntity__GetClass( void ) {
		return ( const char* )cEntity::GetClass();
	};
	 Lua__cEntity( const double& a_X, const double& a_Y, const double& a_Z): cEntity(a_X,a_Y,a_Z){};
};

/* method: tolua__set_instance of class  Lua__cEntity */
#ifndef TOLUA_DISABLE_tolua_AllToLua_Lua__cEntity_tolua__set_instance00
static int tolua_AllToLua_Lua__cEntity_tolua__set_instance00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Lua__cEntity",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Lua__cEntity* self = (Lua__cEntity*)  tolua_tousertype(tolua_S,1,0);
  lua_State* L =  tolua_S;
  lua_Object lo = ((lua_Object)  tolua_tovalue(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'tolua__set_instance'", NULL);
#endif
  {
   self->tolua__set_instance(L,lo);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'tolua__set_instance'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: cEntity__Initialize of class  Lua__cEntity */
#ifndef TOLUA_DISABLE_tolua_AllToLua_Lua__cEntity_cEntity__Initialize00
static int tolua_AllToLua_Lua__cEntity_cEntity__Initialize00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Lua__cEntity",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Lua__cEntity* self = (Lua__cEntity*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'cEntity__Initialize'", NULL);
#endif
  {
   self->cEntity__Initialize();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'cEntity__Initialize'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: cEntity__GetEntityType of class  Lua__cEntity */
#ifndef TOLUA_DISABLE_tolua_AllToLua_Lua__cEntity_cEntity__GetEntityType00
static int tolua_AllToLua_Lua__cEntity_cEntity__GetEntityType00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Lua__cEntity",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Lua__cEntity* self = (Lua__cEntity*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'cEntity__GetEntityType'", NULL);
#endif
  {
   unsigned int tolua_ret = (unsigned int)  self->cEntity__GetEntityType();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'cEntity__GetEntityType'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: cEntity__IsA of class  Lua__cEntity */
#ifndef TOLUA_DISABLE_tolua_AllToLua_Lua__cEntity_cEntity__IsA00
static int tolua_AllToLua_Lua__cEntity_cEntity__IsA00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Lua__cEntity",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Lua__cEntity* self = (Lua__cEntity*)  tolua_tousertype(tolua_S,1,0);
  const char* a_EntityType = ((const char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'cEntity__IsA'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->cEntity__IsA(a_EntityType);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'cEntity__IsA'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: cEntity__GetClass of class  Lua__cEntity */
#ifndef TOLUA_DISABLE_tolua_AllToLua_Lua__cEntity_cEntity__GetClass00
static int tolua_AllToLua_Lua__cEntity_cEntity__GetClass00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Lua__cEntity",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Lua__cEntity* self = (Lua__cEntity*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'cEntity__GetClass'", NULL);
#endif
  {
   const char* tolua_ret = (const char*)  self->cEntity__GetClass();
   tolua_pushstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'cEntity__GetClass'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  Lua__cEntity */
#ifndef TOLUA_DISABLE_tolua_AllToLua_Lua__cEntity_new00
static int tolua_AllToLua_Lua__cEntity_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Lua__cEntity",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const double a_X = ((const double)  tolua_tonumber(tolua_S,2,0));
  const double a_Y = ((const double)  tolua_tonumber(tolua_S,3,0));
  const double a_Z = ((const double)  tolua_tonumber(tolua_S,4,0));
  {
   Lua__cEntity* tolua_ret = (Lua__cEntity*)  Mtolua_new((Lua__cEntity)(a_X,a_Y,a_Z));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"Lua__cEntity");
   tolua_pushnumber(tolua_S,(lua_Number)a_X);
   tolua_pushnumber(tolua_S,(lua_Number)a_Y);
   tolua_pushnumber(tolua_S,(lua_Number)a_Z);
  }
 }
 return 4;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  Lua__cEntity */
#ifndef TOLUA_DISABLE_tolua_AllToLua_Lua__cEntity_new00_local
static int tolua_AllToLua_Lua__cEntity_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Lua__cEntity",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const double a_X = ((const double)  tolua_tonumber(tolua_S,2,0));
  const double a_Y = ((const double)  tolua_tonumber(tolua_S,3,0));
  const double a_Z = ((const double)  tolua_tonumber(tolua_S,4,0));
  {
   Lua__cEntity* tolua_ret = (Lua__cEntity*)  Mtolua_new((Lua__cEntity)(a_X,a_Y,a_Z));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"Lua__cEntity");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
   tolua_pushnumber(tolua_S,(lua_Number)a_X);
   tolua_pushnumber(tolua_S,(lua_Number)a_Y);
   tolua_pushnumber(tolua_S,(lua_Number)a_Z);
  }
 }
 return 4;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  Lua__cEntity */
#ifndef TOLUA_DISABLE_tolua_AllToLua_Lua__cEntity_delete00
static int tolua_AllToLua_Lua__cEntity_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Lua__cEntity",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Lua__cEntity* self = (Lua__cEntity*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'", NULL);
#endif
  Mtolua_delete(self);
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE


/* function to release collected object via destructor */
#ifdef __cplusplus

static int tolua_collect_Lua__cEntity (lua_State* tolua_S)
{
 Lua__cEntity* self = (Lua__cEntity*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}
#endif

/* get function: Damage of class  TakeDamageInfo */
#ifndef TOLUA_DISABLE_tolua_get_TakeDamageInfo_Damage
static int tolua_get_TakeDamageInfo_Damage(lua_State* tolua_S)
{
  TakeDamageInfo* self = (TakeDamageInfo*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'Damage'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->Damage);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: Damage of class  TakeDamageInfo */
#ifndef TOLUA_DISABLE_tolua_set_TakeDamageInfo_Damage
static int tolua_set_TakeDamageInfo_Damage(lua_State* tolua_S)
{
  TakeDamageInfo* self = (TakeDamageInfo*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'Damage'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->Damage = ((int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: Instigator of class  TakeDamageInfo */
#ifndef TOLUA_DISABLE_tolua_get_TakeDamageInfo_Instigator_ptr
static int tolua_get_TakeDamageInfo_Instigator_ptr(lua_State* tolua_S)
{
  TakeDamageInfo* self = (TakeDamageInfo*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'Instigator'",NULL);
#endif
   tolua_pushusertype(tolua_S,(void*)self->Instigator,"cEntity");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: Instigator of class  TakeDamageInfo */
#ifndef TOLUA_DISABLE_tolua_set_TakeDamageInfo_Instigator_ptr
static int tolua_set_TakeDamageInfo_Instigator_ptr(lua_State* tolua_S)
{
  TakeDamageInfo* self = (TakeDamageInfo*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'Instigator'",NULL);
  if (!tolua_isusertype(tolua_S,2,"cEntity",0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->Instigator = ((cEntity*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: TeleportTo of class  cPawn */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cPawn_TeleportTo00
static int tolua_AllToLua_cPawn_TeleportTo00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cPawn",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"cEntity",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cPawn* self = (cPawn*)  tolua_tousertype(tolua_S,1,0);
  cEntity* a_Entity = ((cEntity*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'TeleportTo'", NULL);
#endif
  {
   self->TeleportTo(a_Entity);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'TeleportTo'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: TeleportTo of class  cPawn */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cPawn_TeleportTo01
static int tolua_AllToLua_cPawn_TeleportTo01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cPawn",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  cPawn* self = (cPawn*)  tolua_tousertype(tolua_S,1,0);
  const double a_PosX = ((const double)  tolua_tonumber(tolua_S,2,0));
  const double a_PosY = ((const double)  tolua_tonumber(tolua_S,3,0));
  const double a_PosZ = ((const double)  tolua_tonumber(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'TeleportTo'", NULL);
#endif
  {
   self->TeleportTo(a_PosX,a_PosY,a_PosZ);
   tolua_pushnumber(tolua_S,(lua_Number)a_PosX);
   tolua_pushnumber(tolua_S,(lua_Number)a_PosY);
   tolua_pushnumber(tolua_S,(lua_Number)a_PosZ);
  }
 }
 return 3;
tolua_lerror:
 return tolua_AllToLua_cPawn_TeleportTo00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: Heal of class  cPawn */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cPawn_Heal00
static int tolua_AllToLua_cPawn_Heal00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cPawn",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cPawn* self = (cPawn*)  tolua_tousertype(tolua_S,1,0);
  int a_Health = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'Heal'", NULL);
#endif
  {
   self->Heal(a_Health);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Heal'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: TakeDamage of class  cPawn */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cPawn_TakeDamage00
static int tolua_AllToLua_cPawn_TakeDamage00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cPawn",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"cEntity",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cPawn* self = (cPawn*)  tolua_tousertype(tolua_S,1,0);
  int a_Damage = ((int)  tolua_tonumber(tolua_S,2,0));
  cEntity* a_Instigator = ((cEntity*)  tolua_tousertype(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'TakeDamage'", NULL);
#endif
  {
   self->TakeDamage(a_Damage,a_Instigator);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'TakeDamage'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: KilledBy of class  cPawn */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cPawn_KilledBy00
static int tolua_AllToLua_cPawn_KilledBy00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cPawn",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"cEntity",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cPawn* self = (cPawn*)  tolua_tousertype(tolua_S,1,0);
  cEntity* a_Killer = ((cEntity*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'KilledBy'", NULL);
#endif
  {
   self->KilledBy(a_Killer);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'KilledBy'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetHealth of class  cPawn */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cPawn_GetHealth00
static int tolua_AllToLua_cPawn_GetHealth00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cPawn",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cPawn* self = (cPawn*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetHealth'", NULL);
#endif
  {
   int tolua_ret = (int)  self->GetHealth();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetHealth'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

 class Lua__cPawn : public cPawn, public ToluaBase {
public:
	 void  TeleportTo( cEntity* a_Entity) {
		if (push_method("TeleportTo",  tolua_AllToLua_cPawn_TeleportTo00)) {
			tolua_pushusertype(lua_state, (void*)a_Entity, "cEntity");
			ToluaBase::dbcall(lua_state, 2, 0);
		} else {
			return ( void ) cPawn:: TeleportTo(a_Entity);
		};
	};
	 void  TeleportTo( const double& a_PosX, const double& a_PosY, const double& a_PosZ) {
		if (push_method("TeleportTo",  tolua_AllToLua_cPawn_TeleportTo01)) {
			tolua_pushnumber(lua_state, (lua_Number)a_PosX);
			tolua_pushnumber(lua_state, (lua_Number)a_PosY);
			tolua_pushnumber(lua_state, (lua_Number)a_PosZ);
			ToluaBase::dbcall(lua_state, 4, 0);
		} else {
			return ( void ) cPawn:: TeleportTo(a_PosX,a_PosY,a_PosZ);
		};
	};
	 void  TakeDamage( int a_Damage, cEntity* a_Instigator) {
		if (push_method("TakeDamage",  tolua_AllToLua_cPawn_TakeDamage00)) {
			tolua_pushnumber(lua_state, (lua_Number)a_Damage);
			tolua_pushusertype(lua_state, (void*)a_Instigator, "cEntity");
			ToluaBase::dbcall(lua_state, 3, 0);
		} else {
			return ( void ) cPawn:: TakeDamage(a_Damage,a_Instigator);
		};
	};
	 void  KilledBy( cEntity* a_Killer) {
		if (push_method("KilledBy",  tolua_AllToLua_cPawn_KilledBy00)) {
			tolua_pushusertype(lua_state, (void*)a_Killer, "cEntity");
			ToluaBase::dbcall(lua_state, 2, 0);
		} else {
			return ( void ) cPawn:: KilledBy(a_Killer);
		};
	};
	 void  Initialize( void ) {
		if (push_method("Initialize",  tolua_AllToLua_cEntity_Initialize00)) {
			ToluaBase::dbcall(lua_state, 1, 0);
		} else {
			return ( void ) cPawn:: Initialize();
		};
	};
	unsigned int  GetEntityType( void ) {
		if (push_method("GetEntityType",  tolua_AllToLua_cEntity_GetEntityType00)) {
			ToluaBase::dbcall(lua_state, 1, 1);
			 unsigned int  tolua_ret = (unsigned int )tolua_tonumber(lua_state, -1, 0);
			lua_pop(lua_state, 1);
			return tolua_ret;
		} else {
			return (unsigned int ) cPawn:: GetEntityType();
		};
	};
	 bool  IsA( const char* a_EntityType) {
		if (push_method("IsA",  tolua_AllToLua_cEntity_IsA00)) {
			tolua_pushstring(lua_state, (const char*)a_EntityType);
			ToluaBase::dbcall(lua_state, 2, 1);
			 bool  tolua_ret = ( bool )tolua_toboolean(lua_state, -1, 0);
			lua_pop(lua_state, 1);
			return tolua_ret;
		} else {
			return ( bool ) cPawn:: IsA(a_EntityType);
		};
	};
	 const char*  GetClass( void ) {
		if (push_method("GetClass",  tolua_AllToLua_cEntity_GetClass00)) {
			ToluaBase::dbcall(lua_state, 1, 1);
			 const char*  tolua_ret = ( const char* )tolua_tostring(lua_state, -1, 0);
			lua_pop(lua_state, 1);
			return tolua_ret;
		} else {
			return ( const char* ) cPawn:: GetClass();
		};
	};
	 void  Tick( float a_Dt) {
		if (push_method("Tick",  tolua_AllToLua_cEntity_Tick00)) {
			tolua_pushnumber(lua_state, (lua_Number)a_Dt);
			ToluaBase::dbcall(lua_state, 2, 0);
		} else {
			if (lua_state)
				LOG("pure-virtual method cPawn::Tick not implemented.");
			else {
				LOG("pure-virtual method cPawn::Tick called with no lua_state. Aborting");
				::abort();
			};
			return ( void )0;
		};
	};
	 void  SpawnOn( cClientHandle* a_Target) {
		if (push_method("SpawnOn",  tolua_AllToLua_cEntity_SpawnOn00)) {
			tolua_pushusertype(lua_state, (void*)a_Target, "cClientHandle");
			ToluaBase::dbcall(lua_state, 2, 0);
		} else {
			if (lua_state)
				LOG("pure-virtual method cPawn::SpawnOn not implemented.");
			else {
				LOG("pure-virtual method cPawn::SpawnOn called with no lua_state. Aborting");
				::abort();
			};
			return ( void )0;
		};
	};

	 void cPawn__TeleportTo( cEntity* a_Entity) {
		return ( void )cPawn::TeleportTo(a_Entity);
	};
	 void cPawn__TeleportTo( const double& a_PosX, const double& a_PosY, const double& a_PosZ) {
		return ( void )cPawn::TeleportTo(a_PosX,a_PosY,a_PosZ);
	};
	 void cPawn__TakeDamage( int a_Damage, cEntity* a_Instigator) {
		return ( void )cPawn::TakeDamage(a_Damage,a_Instigator);
	};
	 void cPawn__KilledBy( cEntity* a_Killer) {
		return ( void )cPawn::KilledBy(a_Killer);
	};
	 void cPawn__Initialize( void ) {
		return ( void )cPawn::Initialize();
	};
	unsigned int cPawn__GetEntityType( void ) {
		return (unsigned int )cPawn::GetEntityType();
	};
	 bool cPawn__IsA( const char* a_EntityType) {
		return ( bool )cPawn::IsA(a_EntityType);
	};
	 const char* cPawn__GetClass( void ) {
		return ( const char* )cPawn::GetClass();
	};
};

/* method: tolua__set_instance of class  Lua__cPawn */
#ifndef TOLUA_DISABLE_tolua_AllToLua_Lua__cPawn_tolua__set_instance00
static int tolua_AllToLua_Lua__cPawn_tolua__set_instance00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Lua__cPawn",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Lua__cPawn* self = (Lua__cPawn*)  tolua_tousertype(tolua_S,1,0);
  lua_State* L =  tolua_S;
  lua_Object lo = ((lua_Object)  tolua_tovalue(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'tolua__set_instance'", NULL);
#endif
  {
   self->tolua__set_instance(L,lo);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'tolua__set_instance'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: cPawn__TeleportTo of class  Lua__cPawn */
#ifndef TOLUA_DISABLE_tolua_AllToLua_Lua__cPawn_cPawn__TeleportTo00
static int tolua_AllToLua_Lua__cPawn_cPawn__TeleportTo00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Lua__cPawn",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"cEntity",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Lua__cPawn* self = (Lua__cPawn*)  tolua_tousertype(tolua_S,1,0);
  cEntity* a_Entity = ((cEntity*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'cPawn__TeleportTo'", NULL);
#endif
  {
   self->cPawn__TeleportTo(a_Entity);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'cPawn__TeleportTo'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: cPawn__TeleportTo of class  Lua__cPawn */
#ifndef TOLUA_DISABLE_tolua_AllToLua_Lua__cPawn_cPawn__TeleportTo01
static int tolua_AllToLua_Lua__cPawn_cPawn__TeleportTo01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Lua__cPawn",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  Lua__cPawn* self = (Lua__cPawn*)  tolua_tousertype(tolua_S,1,0);
  const double a_PosX = ((const double)  tolua_tonumber(tolua_S,2,0));
  const double a_PosY = ((const double)  tolua_tonumber(tolua_S,3,0));
  const double a_PosZ = ((const double)  tolua_tonumber(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'cPawn__TeleportTo'", NULL);
#endif
  {
   self->cPawn__TeleportTo(a_PosX,a_PosY,a_PosZ);
   tolua_pushnumber(tolua_S,(lua_Number)a_PosX);
   tolua_pushnumber(tolua_S,(lua_Number)a_PosY);
   tolua_pushnumber(tolua_S,(lua_Number)a_PosZ);
  }
 }
 return 3;
tolua_lerror:
 return tolua_AllToLua_Lua__cPawn_cPawn__TeleportTo00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: cPawn__TakeDamage of class  Lua__cPawn */
#ifndef TOLUA_DISABLE_tolua_AllToLua_Lua__cPawn_cPawn__TakeDamage00
static int tolua_AllToLua_Lua__cPawn_cPawn__TakeDamage00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Lua__cPawn",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"cEntity",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Lua__cPawn* self = (Lua__cPawn*)  tolua_tousertype(tolua_S,1,0);
  int a_Damage = ((int)  tolua_tonumber(tolua_S,2,0));
  cEntity* a_Instigator = ((cEntity*)  tolua_tousertype(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'cPawn__TakeDamage'", NULL);
#endif
  {
   self->cPawn__TakeDamage(a_Damage,a_Instigator);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'cPawn__TakeDamage'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: cPawn__KilledBy of class  Lua__cPawn */
#ifndef TOLUA_DISABLE_tolua_AllToLua_Lua__cPawn_cPawn__KilledBy00
static int tolua_AllToLua_Lua__cPawn_cPawn__KilledBy00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Lua__cPawn",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"cEntity",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Lua__cPawn* self = (Lua__cPawn*)  tolua_tousertype(tolua_S,1,0);
  cEntity* a_Killer = ((cEntity*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'cPawn__KilledBy'", NULL);
#endif
  {
   self->cPawn__KilledBy(a_Killer);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'cPawn__KilledBy'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetEyeHeight of class  cPlayer */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cPlayer_GetEyeHeight00
static int tolua_AllToLua_cPlayer_GetEyeHeight00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cPlayer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cPlayer* self = (cPlayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetEyeHeight'", NULL);
#endif
  {
   double tolua_ret = (double)  self->GetEyeHeight();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetEyeHeight'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetEyePosition of class  cPlayer */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cPlayer_GetEyePosition00
static int tolua_AllToLua_cPlayer_GetEyePosition00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cPlayer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cPlayer* self = (cPlayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetEyePosition'", NULL);
#endif
  {
   Vector3d tolua_ret = (Vector3d)  self->GetEyePosition();
   {
#ifdef __cplusplus
    void* tolua_obj = Mtolua_new((Vector3d)(tolua_ret));
     tolua_pushusertype(tolua_S,tolua_obj,"Vector3d");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(Vector3d));
     tolua_pushusertype(tolua_S,tolua_obj,"Vector3d");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetEyePosition'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetFlying of class  cPlayer */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cPlayer_GetFlying00
static int tolua_AllToLua_cPlayer_GetFlying00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cPlayer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cPlayer* self = (cPlayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetFlying'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->GetFlying();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetFlying'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetStance of class  cPlayer */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cPlayer_GetStance00
static int tolua_AllToLua_cPlayer_GetStance00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cPlayer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cPlayer* self = (cPlayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetStance'", NULL);
#endif
  {
   const double tolua_ret = (const double)  self->GetStance();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetStance'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetInventory of class  cPlayer */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cPlayer_GetInventory00
static int tolua_AllToLua_cPlayer_GetInventory00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cPlayer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cPlayer* self = (cPlayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetInventory'", NULL);
#endif
  {
   cInventory& tolua_ret = (cInventory&)  self->GetInventory();
    tolua_pushusertype(tolua_S,(void*)&tolua_ret,"cInventory");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetInventory'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: TeleportTo of class  cPlayer */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cPlayer_TeleportTo00
static int tolua_AllToLua_cPlayer_TeleportTo00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cPlayer",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"cEntity",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cPlayer* self = (cPlayer*)  tolua_tousertype(tolua_S,1,0);
  cEntity* a_Entity = ((cEntity*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'TeleportTo'", NULL);
#endif
  {
   self->TeleportTo(a_Entity);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'TeleportTo'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: TeleportTo of class  cPlayer */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cPlayer_TeleportTo01
static int tolua_AllToLua_cPlayer_TeleportTo01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cPlayer",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  cPlayer* self = (cPlayer*)  tolua_tousertype(tolua_S,1,0);
  const double a_PosX = ((const double)  tolua_tonumber(tolua_S,2,0));
  const double a_PosY = ((const double)  tolua_tonumber(tolua_S,3,0));
  const double a_PosZ = ((const double)  tolua_tonumber(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'TeleportTo'", NULL);
#endif
  {
   self->TeleportTo(a_PosX,a_PosY,a_PosZ);
   tolua_pushnumber(tolua_S,(lua_Number)a_PosX);
   tolua_pushnumber(tolua_S,(lua_Number)a_PosY);
   tolua_pushnumber(tolua_S,(lua_Number)a_PosZ);
  }
 }
 return 3;
tolua_lerror:
 return tolua_AllToLua_cPlayer_TeleportTo00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: MoveTo of class  cPlayer */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cPlayer_MoveTo00
static int tolua_AllToLua_cPlayer_MoveTo00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cPlayer",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const Vector3d",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cPlayer* self = (cPlayer*)  tolua_tousertype(tolua_S,1,0);
  const Vector3d* a_NewPos = ((const Vector3d*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'MoveTo'", NULL);
#endif
  {
   self->MoveTo(*a_NewPos);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'MoveTo'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetClientHandle of class  cPlayer */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cPlayer_GetClientHandle00
static int tolua_AllToLua_cPlayer_GetClientHandle00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cPlayer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cPlayer* self = (cPlayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetClientHandle'", NULL);
#endif
  {
   cClientHandle* tolua_ret = (cClientHandle*)  self->GetClientHandle();
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"cClientHandle");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetClientHandle'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SendMessage of class  cPlayer */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cPlayer_SendMessage00
static int tolua_AllToLua_cPlayer_SendMessage00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cPlayer",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cPlayer* self = (cPlayer*)  tolua_tousertype(tolua_S,1,0);
  const char* a_Message = ((const char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SendMessage'", NULL);
#endif
  {
   self->SendMessage(a_Message);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SendMessage'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetName of class  cPlayer */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cPlayer_GetName00
static int tolua_AllToLua_cPlayer_GetName00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cPlayer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cPlayer* self = (cPlayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetName'", NULL);
#endif
  {
   const char* tolua_ret = (const char*)  self->GetName();
   tolua_pushstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetName'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetName of class  cPlayer */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cPlayer_SetName00
static int tolua_AllToLua_cPlayer_SetName00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cPlayer",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cPlayer* self = (cPlayer*)  tolua_tousertype(tolua_S,1,0);
  const char* a_Name = ((const char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetName'", NULL);
#endif
  {
   self->SetName(a_Name);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetName'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: AddToGroup of class  cPlayer */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cPlayer_AddToGroup00
static int tolua_AllToLua_cPlayer_AddToGroup00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cPlayer",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cPlayer* self = (cPlayer*)  tolua_tousertype(tolua_S,1,0);
  const char* a_GroupName = ((const char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'AddToGroup'", NULL);
#endif
  {
   self->AddToGroup(a_GroupName);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'AddToGroup'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: CanUseCommand of class  cPlayer */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cPlayer_CanUseCommand00
static int tolua_AllToLua_cPlayer_CanUseCommand00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cPlayer",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cPlayer* self = (cPlayer*)  tolua_tousertype(tolua_S,1,0);
  const char* a_Command = ((const char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'CanUseCommand'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->CanUseCommand(a_Command);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'CanUseCommand'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: HasPermission of class  cPlayer */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cPlayer_HasPermission00
static int tolua_AllToLua_cPlayer_HasPermission00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cPlayer",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cPlayer* self = (cPlayer*)  tolua_tousertype(tolua_S,1,0);
  const char* a_Permission = ((const char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'HasPermission'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->HasPermission(a_Permission);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'HasPermission'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: IsInGroup of class  cPlayer */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cPlayer_IsInGroup00
static int tolua_AllToLua_cPlayer_IsInGroup00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cPlayer",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cPlayer* self = (cPlayer*)  tolua_tousertype(tolua_S,1,0);
  const char* a_Group = ((const char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'IsInGroup'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->IsInGroup(a_Group);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'IsInGroup'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetColor of class  cPlayer */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cPlayer_GetColor00
static int tolua_AllToLua_cPlayer_GetColor00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cPlayer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cPlayer* self = (cPlayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetColor'", NULL);
#endif
  {
   std::string tolua_ret = (std::string)  self->GetColor();
   tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetColor'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: TossItem of class  cPlayer */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cPlayer_TossItem00
static int tolua_AllToLua_cPlayer_TossItem00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cPlayer",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cPlayer* self = (cPlayer*)  tolua_tousertype(tolua_S,1,0);
  bool a_bDraggingItem = ((bool)  tolua_toboolean(tolua_S,2,0));
  int a_Amount = ((int)  tolua_tonumber(tolua_S,3,1));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'TossItem'", NULL);
#endif
  {
   self->TossItem(a_bDraggingItem,a_Amount);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'TossItem'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: Heal of class  cPlayer */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cPlayer_Heal00
static int tolua_AllToLua_cPlayer_Heal00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cPlayer",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cPlayer* self = (cPlayer*)  tolua_tousertype(tolua_S,1,0);
  int a_Health = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'Heal'", NULL);
#endif
  {
   self->Heal(a_Health);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Heal'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: TakeDamage of class  cPlayer */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cPlayer_TakeDamage00
static int tolua_AllToLua_cPlayer_TakeDamage00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cPlayer",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"cEntity",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cPlayer* self = (cPlayer*)  tolua_tousertype(tolua_S,1,0);
  int a_Damage = ((int)  tolua_tonumber(tolua_S,2,0));
  cEntity* a_Instigator = ((cEntity*)  tolua_tousertype(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'TakeDamage'", NULL);
#endif
  {
   self->TakeDamage(a_Damage,a_Instigator);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'TakeDamage'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: KilledBy of class  cPlayer */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cPlayer_KilledBy00
static int tolua_AllToLua_cPlayer_KilledBy00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cPlayer",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"cEntity",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cPlayer* self = (cPlayer*)  tolua_tousertype(tolua_S,1,0);
  cEntity* a_Killer = ((cEntity*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'KilledBy'", NULL);
#endif
  {
   self->KilledBy(a_Killer);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'KilledBy'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: Respawn of class  cPlayer */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cPlayer_Respawn00
static int tolua_AllToLua_cPlayer_Respawn00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cPlayer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cPlayer* self = (cPlayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'Respawn'", NULL);
#endif
  {
   self->Respawn();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Respawn'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetVisible of class  cPlayer */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cPlayer_SetVisible00
static int tolua_AllToLua_cPlayer_SetVisible00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cPlayer",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cPlayer* self = (cPlayer*)  tolua_tousertype(tolua_S,1,0);
  bool a_bVisible = ((bool)  tolua_toboolean(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetVisible'", NULL);
#endif
  {
   self->SetVisible(a_bVisible);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetVisible'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: IsVisible of class  cPlayer */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cPlayer_IsVisible00
static int tolua_AllToLua_cPlayer_IsVisible00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cPlayer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cPlayer* self = (cPlayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'IsVisible'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->IsVisible();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'IsVisible'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

 class Lua__cPlayer : public cPlayer, public ToluaBase {
public:
	 void  TeleportTo( cEntity* a_Entity) {
		if (push_method("TeleportTo",  tolua_AllToLua_cPlayer_TeleportTo00)) {
			tolua_pushusertype(lua_state, (void*)a_Entity, "cEntity");
			ToluaBase::dbcall(lua_state, 2, 0);
		} else {
			return ( void ) cPlayer:: TeleportTo(a_Entity);
		};
	};
	 void  TeleportTo( const double& a_PosX, const double& a_PosY, const double& a_PosZ) {
		if (push_method("TeleportTo",  tolua_AllToLua_cPlayer_TeleportTo01)) {
			tolua_pushnumber(lua_state, (lua_Number)a_PosX);
			tolua_pushnumber(lua_state, (lua_Number)a_PosY);
			tolua_pushnumber(lua_state, (lua_Number)a_PosZ);
			ToluaBase::dbcall(lua_state, 4, 0);
		} else {
			return ( void ) cPlayer:: TeleportTo(a_PosX,a_PosY,a_PosZ);
		};
	};
	 void  MoveTo( const Vector3d& a_NewPos) {
		if (push_method("MoveTo",  tolua_AllToLua_cPlayer_MoveTo00)) {
			tolua_pushusertype(lua_state, (void*)&a_NewPos, "const Vector3d");
			ToluaBase::dbcall(lua_state, 2, 0);
		} else {
			return ( void ) cPlayer:: MoveTo(a_NewPos);
		};
	};
	 void  TakeDamage( int a_Damage, cEntity* a_Instigator) {
		if (push_method("TakeDamage",  tolua_AllToLua_cPawn_TakeDamage00)) {
			tolua_pushnumber(lua_state, (lua_Number)a_Damage);
			tolua_pushusertype(lua_state, (void*)a_Instigator, "cEntity");
			ToluaBase::dbcall(lua_state, 3, 0);
		} else {
			return ( void ) cPlayer:: TakeDamage(a_Damage,a_Instigator);
		};
	};
	 void  KilledBy( cEntity* a_Killer) {
		if (push_method("KilledBy",  tolua_AllToLua_cPawn_KilledBy00)) {
			tolua_pushusertype(lua_state, (void*)a_Killer, "cEntity");
			ToluaBase::dbcall(lua_state, 2, 0);
		} else {
			return ( void ) cPlayer:: KilledBy(a_Killer);
		};
	};
	 void  Initialize( void ) {
		if (push_method("Initialize",  tolua_AllToLua_cEntity_Initialize00)) {
			ToluaBase::dbcall(lua_state, 1, 0);
		} else {
			return ( void ) cPlayer:: Initialize();
		};
	};
	unsigned int  GetEntityType( void ) {
		if (push_method("GetEntityType",  tolua_AllToLua_cEntity_GetEntityType00)) {
			ToluaBase::dbcall(lua_state, 1, 1);
			 unsigned int  tolua_ret = (unsigned int )tolua_tonumber(lua_state, -1, 0);
			lua_pop(lua_state, 1);
			return tolua_ret;
		} else {
			return (unsigned int ) cPlayer:: GetEntityType();
		};
	};
	 bool  IsA( const char* a_EntityType) {
		if (push_method("IsA",  tolua_AllToLua_cEntity_IsA00)) {
			tolua_pushstring(lua_state, (const char*)a_EntityType);
			ToluaBase::dbcall(lua_state, 2, 1);
			 bool  tolua_ret = ( bool )tolua_toboolean(lua_state, -1, 0);
			lua_pop(lua_state, 1);
			return tolua_ret;
		} else {
			return ( bool ) cPlayer:: IsA(a_EntityType);
		};
	};
	 const char*  GetClass( void ) {
		if (push_method("GetClass",  tolua_AllToLua_cEntity_GetClass00)) {
			ToluaBase::dbcall(lua_state, 1, 1);
			 const char*  tolua_ret = ( const char* )tolua_tostring(lua_state, -1, 0);
			lua_pop(lua_state, 1);
			return tolua_ret;
		} else {
			return ( const char* ) cPlayer:: GetClass();
		};
	};
	 void  Tick( float a_Dt) {
		if (push_method("Tick",  tolua_AllToLua_cEntity_Tick00)) {
			tolua_pushnumber(lua_state, (lua_Number)a_Dt);
			ToluaBase::dbcall(lua_state, 2, 0);
		} else {
			if (lua_state)
				LOG("pure-virtual method cPlayer::Tick not implemented.");
			else {
				LOG("pure-virtual method cPlayer::Tick called with no lua_state. Aborting");
				::abort();
			};
			return ( void )0;
		};
	};
	 void  SpawnOn( cClientHandle* a_Target) {
		if (push_method("SpawnOn",  tolua_AllToLua_cEntity_SpawnOn00)) {
			tolua_pushusertype(lua_state, (void*)a_Target, "cClientHandle");
			ToluaBase::dbcall(lua_state, 2, 0);
		} else {
			if (lua_state)
				LOG("pure-virtual method cPlayer::SpawnOn not implemented.");
			else {
				LOG("pure-virtual method cPlayer::SpawnOn called with no lua_state. Aborting");
				::abort();
			};
			return ( void )0;
		};
	};

	 void cPlayer__TeleportTo( cEntity* a_Entity) {
		return ( void )cPlayer::TeleportTo(a_Entity);
	};
	 void cPlayer__TeleportTo( const double& a_PosX, const double& a_PosY, const double& a_PosZ) {
		return ( void )cPlayer::TeleportTo(a_PosX,a_PosY,a_PosZ);
	};
	 void cPlayer__MoveTo( const Vector3d& a_NewPos) {
		return ( void )cPlayer::MoveTo(a_NewPos);
	};
	 void cPlayer__TakeDamage( int a_Damage, cEntity* a_Instigator) {
		return ( void )cPlayer::TakeDamage(a_Damage,a_Instigator);
	};
	 void cPlayer__KilledBy( cEntity* a_Killer) {
		return ( void )cPlayer::KilledBy(a_Killer);
	};
	 void cPlayer__Initialize( void ) {
		return ( void )cPlayer::Initialize();
	};
	unsigned int cPlayer__GetEntityType( void ) {
		return (unsigned int )cPlayer::GetEntityType();
	};
	 bool cPlayer__IsA( const char* a_EntityType) {
		return ( bool )cPlayer::IsA(a_EntityType);
	};
	 const char* cPlayer__GetClass( void ) {
		return ( const char* )cPlayer::GetClass();
	};
};

/* method: tolua__set_instance of class  Lua__cPlayer */
#ifndef TOLUA_DISABLE_tolua_AllToLua_Lua__cPlayer_tolua__set_instance00
static int tolua_AllToLua_Lua__cPlayer_tolua__set_instance00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Lua__cPlayer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Lua__cPlayer* self = (Lua__cPlayer*)  tolua_tousertype(tolua_S,1,0);
  lua_State* L =  tolua_S;
  lua_Object lo = ((lua_Object)  tolua_tovalue(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'tolua__set_instance'", NULL);
#endif
  {
   self->tolua__set_instance(L,lo);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'tolua__set_instance'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: cPlayer__TeleportTo of class  Lua__cPlayer */
#ifndef TOLUA_DISABLE_tolua_AllToLua_Lua__cPlayer_cPlayer__TeleportTo00
static int tolua_AllToLua_Lua__cPlayer_cPlayer__TeleportTo00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Lua__cPlayer",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"cEntity",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Lua__cPlayer* self = (Lua__cPlayer*)  tolua_tousertype(tolua_S,1,0);
  cEntity* a_Entity = ((cEntity*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'cPlayer__TeleportTo'", NULL);
#endif
  {
   self->cPlayer__TeleportTo(a_Entity);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'cPlayer__TeleportTo'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: cPlayer__TeleportTo of class  Lua__cPlayer */
#ifndef TOLUA_DISABLE_tolua_AllToLua_Lua__cPlayer_cPlayer__TeleportTo01
static int tolua_AllToLua_Lua__cPlayer_cPlayer__TeleportTo01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Lua__cPlayer",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  Lua__cPlayer* self = (Lua__cPlayer*)  tolua_tousertype(tolua_S,1,0);
  const double a_PosX = ((const double)  tolua_tonumber(tolua_S,2,0));
  const double a_PosY = ((const double)  tolua_tonumber(tolua_S,3,0));
  const double a_PosZ = ((const double)  tolua_tonumber(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'cPlayer__TeleportTo'", NULL);
#endif
  {
   self->cPlayer__TeleportTo(a_PosX,a_PosY,a_PosZ);
   tolua_pushnumber(tolua_S,(lua_Number)a_PosX);
   tolua_pushnumber(tolua_S,(lua_Number)a_PosY);
   tolua_pushnumber(tolua_S,(lua_Number)a_PosZ);
  }
 }
 return 3;
tolua_lerror:
 return tolua_AllToLua_Lua__cPlayer_cPlayer__TeleportTo00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: cPlayer__MoveTo of class  Lua__cPlayer */
#ifndef TOLUA_DISABLE_tolua_AllToLua_Lua__cPlayer_cPlayer__MoveTo00
static int tolua_AllToLua_Lua__cPlayer_cPlayer__MoveTo00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Lua__cPlayer",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const Vector3d",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Lua__cPlayer* self = (Lua__cPlayer*)  tolua_tousertype(tolua_S,1,0);
  const Vector3d* a_NewPos = ((const Vector3d*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'cPlayer__MoveTo'", NULL);
#endif
  {
   self->cPlayer__MoveTo(*a_NewPos);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'cPlayer__MoveTo'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetPluginManager of class  cPluginManager */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cPluginManager_GetPluginManager00
static int tolua_AllToLua_cPluginManager_GetPluginManager00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"cPluginManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   cPluginManager* tolua_ret = (cPluginManager*)  cPluginManager::GetPluginManager();
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"cPluginManager");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetPluginManager'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetPlugin of class  cPluginManager */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cPluginManager_GetPlugin00
static int tolua_AllToLua_cPluginManager_GetPlugin00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cPluginManager",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cPluginManager* self = (cPluginManager*)  tolua_tousertype(tolua_S,1,0);
  std::string a_Plugin = ((std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetPlugin'", NULL);
#endif
  {
   cPlugin* tolua_ret = (cPlugin*)  self->GetPlugin(a_Plugin);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"cPlugin");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetPlugin'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: ReloadPlugins of class  cPluginManager */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cPluginManager_ReloadPlugins00
static int tolua_AllToLua_cPluginManager_ReloadPlugins00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cPluginManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cPluginManager* self = (cPluginManager*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'ReloadPlugins'", NULL);
#endif
  {
   self->ReloadPlugins();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'ReloadPlugins'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: AddPlugin of class  cPluginManager */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cPluginManager_AddPlugin00
static int tolua_AllToLua_cPluginManager_AddPlugin00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cPluginManager",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"cPlugin",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cPluginManager* self = (cPluginManager*)  tolua_tousertype(tolua_S,1,0);
  lua_State* a_LuaState =  tolua_S;
  cPlugin* a_Plugin = ((cPlugin*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'AddPlugin'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->AddPlugin(a_LuaState,a_Plugin);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'AddPlugin'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: AddHook of class  cPluginManager */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cPluginManager_AddHook00
static int tolua_AllToLua_cPluginManager_AddHook00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cPluginManager",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"cPlugin",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cPluginManager* self = (cPluginManager*)  tolua_tousertype(tolua_S,1,0);
  cPlugin* a_Plugin = ((cPlugin*)  tolua_tousertype(tolua_S,2,0));
  cPluginManager::PluginHook a_Hook = ((cPluginManager::PluginHook) (int)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'AddHook'", NULL);
#endif
  {
   self->AddHook(a_Plugin,a_Hook);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'AddHook'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetNumPlugins of class  cPluginManager */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cPluginManager_GetNumPlugins00
static int tolua_AllToLua_cPluginManager_GetNumPlugins00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cPluginManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cPluginManager* self = (cPluginManager*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetNumPlugins'", NULL);
#endif
  {
   unsigned int tolua_ret = (unsigned int)  self->GetNumPlugins();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetNumPlugins'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: RemovePlugin of class  cPluginManager */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cPluginManager_RemovePlugin00
static int tolua_AllToLua_cPluginManager_RemovePlugin00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cPluginManager",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"cPlugin",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,3,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cPluginManager* self = (cPluginManager*)  tolua_tousertype(tolua_S,1,0);
  cPlugin* a_Plugin = ((cPlugin*)  tolua_tousertype(tolua_S,2,0));
  bool a_bDelete = ((bool)  tolua_toboolean(tolua_S,3,false));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'RemovePlugin'", NULL);
#endif
  {
   self->RemovePlugin(a_Plugin,a_bDelete);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'RemovePlugin'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: RemoveLuaPlugin of class  cPluginManager */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cPluginManager_RemoveLuaPlugin00
static int tolua_AllToLua_cPluginManager_RemoveLuaPlugin00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cPluginManager",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cPluginManager* self = (cPluginManager*)  tolua_tousertype(tolua_S,1,0);
  std::string a_FileName = ((std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'RemoveLuaPlugin'", NULL);
#endif
  {
   self->RemoveLuaPlugin(a_FileName);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'RemoveLuaPlugin'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetLuaPlugin of class  cPluginManager */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cPluginManager_GetLuaPlugin00
static int tolua_AllToLua_cPluginManager_GetLuaPlugin00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cPluginManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cPluginManager* self = (cPluginManager*)  tolua_tousertype(tolua_S,1,0);
  lua_State* a_State =  tolua_S;
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetLuaPlugin'", NULL);
#endif
  {
   cPlugin_Lua* tolua_ret = (cPlugin_Lua*)  self->GetLuaPlugin(a_State);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"cPlugin_Lua");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetLuaPlugin'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  cPlugin */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cPlugin_delete00
static int tolua_AllToLua_cPlugin_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cPlugin",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cPlugin* self = (cPlugin*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'", NULL);
#endif
  Mtolua_delete(self);
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: OnDisable of class  cPlugin */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cPlugin_OnDisable00
static int tolua_AllToLua_cPlugin_OnDisable00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cPlugin",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cPlugin* self = (cPlugin*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'OnDisable'", NULL);
#endif
  {
   self->OnDisable();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'OnDisable'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: Initialize of class  cPlugin */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cPlugin_Initialize00
static int tolua_AllToLua_cPlugin_Initialize00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cPlugin",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cPlugin* self = (cPlugin*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'Initialize'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->Initialize();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Initialize'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: Tick of class  cPlugin */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cPlugin_Tick00
static int tolua_AllToLua_cPlugin_Tick00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cPlugin",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cPlugin* self = (cPlugin*)  tolua_tousertype(tolua_S,1,0);
  float a_Dt = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'Tick'", NULL);
#endif
  {
   self->Tick(a_Dt);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Tick'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: OnCollectItem of class  cPlugin */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cPlugin_OnCollectItem00
static int tolua_AllToLua_cPlugin_OnCollectItem00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cPlugin",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"cPickup",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"cPlayer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cPlugin* self = (cPlugin*)  tolua_tousertype(tolua_S,1,0);
  cPickup* a_Pickup = ((cPickup*)  tolua_tousertype(tolua_S,2,0));
  cPlayer* a_Player = ((cPlayer*)  tolua_tousertype(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'OnCollectItem'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->OnCollectItem(a_Pickup,a_Player);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'OnCollectItem'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: OnDisconnect of class  cPlugin */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cPlugin_OnDisconnect00
static int tolua_AllToLua_cPlugin_OnDisconnect00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cPlugin",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"cPlayer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cPlugin* self = (cPlugin*)  tolua_tousertype(tolua_S,1,0);
  std::string a_Reason = ((std::string)  tolua_tocppstring(tolua_S,2,0));
  cPlayer* a_Player = ((cPlayer*)  tolua_tousertype(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'OnDisconnect'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->OnDisconnect(a_Reason,a_Player);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'OnDisconnect'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: OnBlockPlace of class  cPlugin */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cPlugin_OnBlockPlace00
static int tolua_AllToLua_cPlugin_OnBlockPlace00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cPlugin",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"cPacket_BlockPlace",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"cPlayer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cPlugin* self = (cPlugin*)  tolua_tousertype(tolua_S,1,0);
  cPacket_BlockPlace* a_PacketData = ((cPacket_BlockPlace*)  tolua_tousertype(tolua_S,2,0));
  cPlayer* a_Player = ((cPlayer*)  tolua_tousertype(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'OnBlockPlace'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->OnBlockPlace(a_PacketData,a_Player);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'OnBlockPlace'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: OnBlockDig of class  cPlugin */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cPlugin_OnBlockDig00
static int tolua_AllToLua_cPlugin_OnBlockDig00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cPlugin",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"cPacket_BlockDig",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"cPlayer",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,4,"cItem",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cPlugin* self = (cPlugin*)  tolua_tousertype(tolua_S,1,0);
  cPacket_BlockDig* a_PacketData = ((cPacket_BlockDig*)  tolua_tousertype(tolua_S,2,0));
  cPlayer* a_Player = ((cPlayer*)  tolua_tousertype(tolua_S,3,0));
  cItem* a_PickupItem = ((cItem*)  tolua_tousertype(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'OnBlockDig'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->OnBlockDig(a_PacketData,a_Player,a_PickupItem);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'OnBlockDig'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: OnChat of class  cPlugin */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cPlugin_OnChat00
static int tolua_AllToLua_cPlugin_OnChat00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cPlugin",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"cPlayer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cPlugin* self = (cPlugin*)  tolua_tousertype(tolua_S,1,0);
  std::string a_Chat = ((std::string)  tolua_tocppstring(tolua_S,2,0));
  cPlayer* a_Player = ((cPlayer*)  tolua_tousertype(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'OnChat'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->OnChat(a_Chat,a_Player);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'OnChat'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: OnLogin of class  cPlugin */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cPlugin_OnLogin00
static int tolua_AllToLua_cPlugin_OnLogin00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cPlugin",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"cPacket_Login",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cPlugin* self = (cPlugin*)  tolua_tousertype(tolua_S,1,0);
  cPacket_Login* a_PacketData = ((cPacket_Login*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'OnLogin'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->OnLogin(a_PacketData);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'OnLogin'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: OnPlayerSpawn of class  cPlugin */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cPlugin_OnPlayerSpawn00
static int tolua_AllToLua_cPlugin_OnPlayerSpawn00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cPlugin",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"cPlayer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cPlugin* self = (cPlugin*)  tolua_tousertype(tolua_S,1,0);
  cPlayer* a_Player = ((cPlayer*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'OnPlayerSpawn'", NULL);
#endif
  {
   self->OnPlayerSpawn(a_Player);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'OnPlayerSpawn'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: OnPlayerJoin of class  cPlugin */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cPlugin_OnPlayerJoin00
static int tolua_AllToLua_cPlugin_OnPlayerJoin00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cPlugin",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"cPlayer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cPlugin* self = (cPlugin*)  tolua_tousertype(tolua_S,1,0);
  cPlayer* a_Player = ((cPlayer*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'OnPlayerJoin'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->OnPlayerJoin(a_Player);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'OnPlayerJoin'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: OnPlayerMove of class  cPlugin */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cPlugin_OnPlayerMove00
static int tolua_AllToLua_cPlugin_OnPlayerMove00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cPlugin",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"cPlayer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cPlugin* self = (cPlugin*)  tolua_tousertype(tolua_S,1,0);
  cPlayer* a_Player = ((cPlayer*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'OnPlayerMove'", NULL);
#endif
  {
   self->OnPlayerMove(a_Player);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'OnPlayerMove'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: OnTakeDamage of class  cPlugin */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cPlugin_OnTakeDamage00
static int tolua_AllToLua_cPlugin_OnTakeDamage00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cPlugin",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"cPawn",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"TakeDamageInfo",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cPlugin* self = (cPlugin*)  tolua_tousertype(tolua_S,1,0);
  cPawn* a_Pawn = ((cPawn*)  tolua_tousertype(tolua_S,2,0));
  TakeDamageInfo* a_TakeDamageInfo = ((TakeDamageInfo*)  tolua_tousertype(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'OnTakeDamage'", NULL);
#endif
  {
   self->OnTakeDamage(a_Pawn,a_TakeDamageInfo);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'OnTakeDamage'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: OnKilled of class  cPlugin */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cPlugin_OnKilled00
static int tolua_AllToLua_cPlugin_OnKilled00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cPlugin",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"cPawn",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"cEntity",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cPlugin* self = (cPlugin*)  tolua_tousertype(tolua_S,1,0);
  cPawn* a_Killed = ((cPawn*)  tolua_tousertype(tolua_S,2,0));
  cEntity* a_Killer = ((cEntity*)  tolua_tousertype(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'OnKilled'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->OnKilled(a_Killed,a_Killer);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'OnKilled'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetName of class  cPlugin */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cPlugin_GetName00
static int tolua_AllToLua_cPlugin_GetName00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cPlugin",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cPlugin* self = (const cPlugin*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetName'", NULL);
#endif
  {
   std::string tolua_ret = (std::string)  self->GetName();
   tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetName'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetName of class  cPlugin */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cPlugin_SetName00
static int tolua_AllToLua_cPlugin_SetName00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cPlugin",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cPlugin* self = (cPlugin*)  tolua_tousertype(tolua_S,1,0);
  std::string a_Name = ((std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetName'", NULL);
#endif
  {
   self->SetName(a_Name);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetName'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetVersion of class  cPlugin */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cPlugin_GetVersion00
static int tolua_AllToLua_cPlugin_GetVersion00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cPlugin",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cPlugin* self = (const cPlugin*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetVersion'", NULL);
#endif
  {
   int tolua_ret = (int)  self->GetVersion();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetVersion'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetVersion of class  cPlugin */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cPlugin_SetVersion00
static int tolua_AllToLua_cPlugin_SetVersion00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cPlugin",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cPlugin* self = (cPlugin*)  tolua_tousertype(tolua_S,1,0);
  int a_Version = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetVersion'", NULL);
#endif
  {
   self->SetVersion(a_Version);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetVersion'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* get function: Command of class  CommandStruct */
#ifndef TOLUA_DISABLE_tolua_get_cPlugin__CommandStruct_Command
static int tolua_get_cPlugin__CommandStruct_Command(lua_State* tolua_S)
{
  cPlugin::CommandStruct* self = (cPlugin::CommandStruct*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'Command'",NULL);
#endif
  tolua_pushcppstring(tolua_S,(const char*)self->Command);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: Command of class  CommandStruct */
#ifndef TOLUA_DISABLE_tolua_set_cPlugin__CommandStruct_Command
static int tolua_set_cPlugin__CommandStruct_Command(lua_State* tolua_S)
{
  cPlugin::CommandStruct* self = (cPlugin::CommandStruct*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'Command'",NULL);
  if (!tolua_iscppstring(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->Command = ((std::string)  tolua_tocppstring(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: Description of class  CommandStruct */
#ifndef TOLUA_DISABLE_tolua_get_cPlugin__CommandStruct_Description
static int tolua_get_cPlugin__CommandStruct_Description(lua_State* tolua_S)
{
  cPlugin::CommandStruct* self = (cPlugin::CommandStruct*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'Description'",NULL);
#endif
  tolua_pushcppstring(tolua_S,(const char*)self->Description);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: Description of class  CommandStruct */
#ifndef TOLUA_DISABLE_tolua_set_cPlugin__CommandStruct_Description
static int tolua_set_cPlugin__CommandStruct_Description(lua_State* tolua_S)
{
  cPlugin::CommandStruct* self = (cPlugin::CommandStruct*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'Description'",NULL);
  if (!tolua_iscppstring(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->Description = ((std::string)  tolua_tocppstring(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: Permission of class  CommandStruct */
#ifndef TOLUA_DISABLE_tolua_get_cPlugin__CommandStruct_Permission
static int tolua_get_cPlugin__CommandStruct_Permission(lua_State* tolua_S)
{
  cPlugin::CommandStruct* self = (cPlugin::CommandStruct*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'Permission'",NULL);
#endif
  tolua_pushcppstring(tolua_S,(const char*)self->Permission);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: Permission of class  CommandStruct */
#ifndef TOLUA_DISABLE_tolua_set_cPlugin__CommandStruct_Permission
static int tolua_set_cPlugin__CommandStruct_Permission(lua_State* tolua_S)
{
  cPlugin::CommandStruct* self = (cPlugin::CommandStruct*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'Permission'",NULL);
  if (!tolua_iscppstring(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->Permission = ((std::string)  tolua_tocppstring(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: AddCommand of class  cPlugin */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cPlugin_AddCommand00
static int tolua_AllToLua_cPlugin_AddCommand00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cPlugin",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,3,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cPlugin* self = (cPlugin*)  tolua_tousertype(tolua_S,1,0);
  std::string a_Command = ((std::string)  tolua_tocppstring(tolua_S,2,0));
  std::string a_Description = ((std::string)  tolua_tocppstring(tolua_S,3,0));
  std::string a_Permission = ((std::string)  tolua_tocppstring(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'AddCommand'", NULL);
#endif
  {
   self->AddCommand(a_Command,a_Description,a_Permission);
   tolua_pushcppstring(tolua_S,(const char*)a_Command);
   tolua_pushcppstring(tolua_S,(const char*)a_Description);
   tolua_pushcppstring(tolua_S,(const char*)a_Permission);
  }
 }
 return 3;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'AddCommand'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

 class Lua__cPlugin : public cPlugin, public ToluaBase {
public:
	 void  OnDisable( void ) {
		if (push_method("OnDisable",  tolua_AllToLua_cPlugin_OnDisable00)) {
			ToluaBase::dbcall(lua_state, 1, 0);
		} else {
			return ( void ) cPlugin:: OnDisable();
		};
	};
	 bool  Initialize( void ) {
		if (push_method("Initialize",  tolua_AllToLua_cPlugin_Initialize00)) {
			ToluaBase::dbcall(lua_state, 1, 1);
			 bool  tolua_ret = ( bool )tolua_toboolean(lua_state, -1, 0);
			lua_pop(lua_state, 1);
			return tolua_ret;
		} else {
			if (lua_state)
				LOG("pure-virtual method cPlugin::Initialize not implemented.");
			else {
				LOG("pure-virtual method cPlugin::Initialize called with no lua_state. Aborting");
				::abort();
			};
			return ( bool )0;
		};
	};
	 void  Tick( float a_Dt) {
		if (push_method("Tick",  tolua_AllToLua_cPlugin_Tick00)) {
			tolua_pushnumber(lua_state, (lua_Number)a_Dt);
			ToluaBase::dbcall(lua_state, 2, 0);
		} else {
			return ( void ) cPlugin:: Tick(a_Dt);
		};
	};
	 bool  OnCollectItem( cPickup* a_Pickup, cPlayer* a_Player) {
		if (push_method("OnCollectItem",  tolua_AllToLua_cPlugin_OnCollectItem00)) {
			tolua_pushusertype(lua_state, (void*)a_Pickup, "cPickup");
			tolua_pushusertype(lua_state, (void*)a_Player, "cPlayer");
			ToluaBase::dbcall(lua_state, 3, 1);
			 bool  tolua_ret = ( bool )tolua_toboolean(lua_state, -1, 0);
			lua_pop(lua_state, 1);
			return tolua_ret;
		} else {
			return ( bool ) cPlugin:: OnCollectItem(a_Pickup,a_Player);
		};
	};
	 bool  OnDisconnect( std::string a_Reason, cPlayer* a_Player) {
		if (push_method("OnDisconnect",  tolua_AllToLua_cPlugin_OnDisconnect00)) {
			tolua_pushcppstring(lua_state, (const char*)a_Reason);
			tolua_pushusertype(lua_state, (void*)a_Player, "cPlayer");
			ToluaBase::dbcall(lua_state, 3, 1);
			 bool  tolua_ret = ( bool )tolua_toboolean(lua_state, -1, 0);
			lua_pop(lua_state, 1);
			return tolua_ret;
		} else {
			return ( bool ) cPlugin:: OnDisconnect(a_Reason,a_Player);
		};
	};
	 bool  OnBlockPlace( cPacket_BlockPlace* a_PacketData, cPlayer* a_Player) {
		if (push_method("OnBlockPlace",  tolua_AllToLua_cPlugin_OnBlockPlace00)) {
			tolua_pushusertype(lua_state, (void*)a_PacketData, "cPacket_BlockPlace");
			tolua_pushusertype(lua_state, (void*)a_Player, "cPlayer");
			ToluaBase::dbcall(lua_state, 3, 1);
			 bool  tolua_ret = ( bool )tolua_toboolean(lua_state, -1, 0);
			lua_pop(lua_state, 1);
			return tolua_ret;
		} else {
			return ( bool ) cPlugin:: OnBlockPlace(a_PacketData,a_Player);
		};
	};
	 bool  OnBlockDig( cPacket_BlockDig* a_PacketData, cPlayer* a_Player, cItem* a_PickupItem) {
		if (push_method("OnBlockDig",  tolua_AllToLua_cPlugin_OnBlockDig00)) {
			tolua_pushusertype(lua_state, (void*)a_PacketData, "cPacket_BlockDig");
			tolua_pushusertype(lua_state, (void*)a_Player, "cPlayer");
			tolua_pushusertype(lua_state, (void*)a_PickupItem, "cItem");
			ToluaBase::dbcall(lua_state, 4, 1);
			 bool  tolua_ret = ( bool )tolua_toboolean(lua_state, -1, 0);
			lua_pop(lua_state, 1);
			return tolua_ret;
		} else {
			return ( bool ) cPlugin:: OnBlockDig(a_PacketData,a_Player,a_PickupItem);
		};
	};
	 bool  OnChat( std::string a_Chat, cPlayer* a_Player) {
		if (push_method("OnChat",  tolua_AllToLua_cPlugin_OnChat00)) {
			tolua_pushcppstring(lua_state, (const char*)a_Chat);
			tolua_pushusertype(lua_state, (void*)a_Player, "cPlayer");
			ToluaBase::dbcall(lua_state, 3, 1);
			 bool  tolua_ret = ( bool )tolua_toboolean(lua_state, -1, 0);
			lua_pop(lua_state, 1);
			return tolua_ret;
		} else {
			return ( bool ) cPlugin:: OnChat(a_Chat,a_Player);
		};
	};
	 bool  OnLogin( cPacket_Login* a_PacketData) {
		if (push_method("OnLogin",  tolua_AllToLua_cPlugin_OnLogin00)) {
			tolua_pushusertype(lua_state, (void*)a_PacketData, "cPacket_Login");
			ToluaBase::dbcall(lua_state, 2, 1);
			 bool  tolua_ret = ( bool )tolua_toboolean(lua_state, -1, 0);
			lua_pop(lua_state, 1);
			return tolua_ret;
		} else {
			return ( bool ) cPlugin:: OnLogin(a_PacketData);
		};
	};
	 void  OnPlayerSpawn( cPlayer* a_Player) {
		if (push_method("OnPlayerSpawn",  tolua_AllToLua_cPlugin_OnPlayerSpawn00)) {
			tolua_pushusertype(lua_state, (void*)a_Player, "cPlayer");
			ToluaBase::dbcall(lua_state, 2, 0);
		} else {
			return ( void ) cPlugin:: OnPlayerSpawn(a_Player);
		};
	};
	 bool  OnPlayerJoin( cPlayer* a_Player) {
		if (push_method("OnPlayerJoin",  tolua_AllToLua_cPlugin_OnPlayerJoin00)) {
			tolua_pushusertype(lua_state, (void*)a_Player, "cPlayer");
			ToluaBase::dbcall(lua_state, 2, 1);
			 bool  tolua_ret = ( bool )tolua_toboolean(lua_state, -1, 0);
			lua_pop(lua_state, 1);
			return tolua_ret;
		} else {
			return ( bool ) cPlugin:: OnPlayerJoin(a_Player);
		};
	};
	 void  OnPlayerMove( cPlayer* a_Player) {
		if (push_method("OnPlayerMove",  tolua_AllToLua_cPlugin_OnPlayerMove00)) {
			tolua_pushusertype(lua_state, (void*)a_Player, "cPlayer");
			ToluaBase::dbcall(lua_state, 2, 0);
		} else {
			return ( void ) cPlugin:: OnPlayerMove(a_Player);
		};
	};
	 void  OnTakeDamage( cPawn* a_Pawn, TakeDamageInfo* a_TakeDamageInfo) {
		if (push_method("OnTakeDamage",  tolua_AllToLua_cPlugin_OnTakeDamage00)) {
			tolua_pushusertype(lua_state, (void*)a_Pawn, "cPawn");
			tolua_pushusertype(lua_state, (void*)a_TakeDamageInfo, "TakeDamageInfo");
			ToluaBase::dbcall(lua_state, 3, 0);
		} else {
			return ( void ) cPlugin:: OnTakeDamage(a_Pawn,a_TakeDamageInfo);
		};
	};
	 bool  OnKilled( cPawn* a_Killed, cEntity* a_Killer) {
		if (push_method("OnKilled",  tolua_AllToLua_cPlugin_OnKilled00)) {
			tolua_pushusertype(lua_state, (void*)a_Killed, "cPawn");
			tolua_pushusertype(lua_state, (void*)a_Killer, "cEntity");
			ToluaBase::dbcall(lua_state, 3, 1);
			 bool  tolua_ret = ( bool )tolua_toboolean(lua_state, -1, 0);
			lua_pop(lua_state, 1);
			return tolua_ret;
		} else {
			return ( bool ) cPlugin:: OnKilled(a_Killed,a_Killer);
		};
	};

	 void cPlugin__OnDisable( void ) {
		return ( void )cPlugin::OnDisable();
	};
	 void cPlugin__Tick( float a_Dt) {
		return ( void )cPlugin::Tick(a_Dt);
	};
	 bool cPlugin__OnCollectItem( cPickup* a_Pickup, cPlayer* a_Player) {
		return ( bool )cPlugin::OnCollectItem(a_Pickup,a_Player);
	};
	 bool cPlugin__OnDisconnect( std::string a_Reason, cPlayer* a_Player) {
		return ( bool )cPlugin::OnDisconnect(a_Reason,a_Player);
	};
	 bool cPlugin__OnBlockPlace( cPacket_BlockPlace* a_PacketData, cPlayer* a_Player) {
		return ( bool )cPlugin::OnBlockPlace(a_PacketData,a_Player);
	};
	 bool cPlugin__OnBlockDig( cPacket_BlockDig* a_PacketData, cPlayer* a_Player, cItem* a_PickupItem) {
		return ( bool )cPlugin::OnBlockDig(a_PacketData,a_Player,a_PickupItem);
	};
	 bool cPlugin__OnChat( std::string a_Chat, cPlayer* a_Player) {
		return ( bool )cPlugin::OnChat(a_Chat,a_Player);
	};
	 bool cPlugin__OnLogin( cPacket_Login* a_PacketData) {
		return ( bool )cPlugin::OnLogin(a_PacketData);
	};
	 void cPlugin__OnPlayerSpawn( cPlayer* a_Player) {
		return ( void )cPlugin::OnPlayerSpawn(a_Player);
	};
	 bool cPlugin__OnPlayerJoin( cPlayer* a_Player) {
		return ( bool )cPlugin::OnPlayerJoin(a_Player);
	};
	 void cPlugin__OnPlayerMove( cPlayer* a_Player) {
		return ( void )cPlugin::OnPlayerMove(a_Player);
	};
	 void cPlugin__OnTakeDamage( cPawn* a_Pawn, TakeDamageInfo* a_TakeDamageInfo) {
		return ( void )cPlugin::OnTakeDamage(a_Pawn,a_TakeDamageInfo);
	};
	 bool cPlugin__OnKilled( cPawn* a_Killed, cEntity* a_Killer) {
		return ( bool )cPlugin::OnKilled(a_Killed,a_Killer);
	};
	 Lua__cPlugin( void ): cPlugin(){};
};

/* method: tolua__set_instance of class  Lua__cPlugin */
#ifndef TOLUA_DISABLE_tolua_AllToLua_Lua__cPlugin_tolua__set_instance00
static int tolua_AllToLua_Lua__cPlugin_tolua__set_instance00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Lua__cPlugin",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Lua__cPlugin* self = (Lua__cPlugin*)  tolua_tousertype(tolua_S,1,0);
  lua_State* L =  tolua_S;
  lua_Object lo = ((lua_Object)  tolua_tovalue(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'tolua__set_instance'", NULL);
#endif
  {
   self->tolua__set_instance(L,lo);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'tolua__set_instance'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: cPlugin__OnDisable of class  Lua__cPlugin */
#ifndef TOLUA_DISABLE_tolua_AllToLua_Lua__cPlugin_cPlugin__OnDisable00
static int tolua_AllToLua_Lua__cPlugin_cPlugin__OnDisable00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Lua__cPlugin",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Lua__cPlugin* self = (Lua__cPlugin*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'cPlugin__OnDisable'", NULL);
#endif
  {
   self->cPlugin__OnDisable();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'cPlugin__OnDisable'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: cPlugin__Tick of class  Lua__cPlugin */
#ifndef TOLUA_DISABLE_tolua_AllToLua_Lua__cPlugin_cPlugin__Tick00
static int tolua_AllToLua_Lua__cPlugin_cPlugin__Tick00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Lua__cPlugin",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Lua__cPlugin* self = (Lua__cPlugin*)  tolua_tousertype(tolua_S,1,0);
  float a_Dt = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'cPlugin__Tick'", NULL);
#endif
  {
   self->cPlugin__Tick(a_Dt);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'cPlugin__Tick'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: cPlugin__OnCollectItem of class  Lua__cPlugin */
#ifndef TOLUA_DISABLE_tolua_AllToLua_Lua__cPlugin_cPlugin__OnCollectItem00
static int tolua_AllToLua_Lua__cPlugin_cPlugin__OnCollectItem00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Lua__cPlugin",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"cPickup",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"cPlayer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Lua__cPlugin* self = (Lua__cPlugin*)  tolua_tousertype(tolua_S,1,0);
  cPickup* a_Pickup = ((cPickup*)  tolua_tousertype(tolua_S,2,0));
  cPlayer* a_Player = ((cPlayer*)  tolua_tousertype(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'cPlugin__OnCollectItem'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->cPlugin__OnCollectItem(a_Pickup,a_Player);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'cPlugin__OnCollectItem'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: cPlugin__OnDisconnect of class  Lua__cPlugin */
#ifndef TOLUA_DISABLE_tolua_AllToLua_Lua__cPlugin_cPlugin__OnDisconnect00
static int tolua_AllToLua_Lua__cPlugin_cPlugin__OnDisconnect00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Lua__cPlugin",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"cPlayer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Lua__cPlugin* self = (Lua__cPlugin*)  tolua_tousertype(tolua_S,1,0);
  std::string a_Reason = ((std::string)  tolua_tocppstring(tolua_S,2,0));
  cPlayer* a_Player = ((cPlayer*)  tolua_tousertype(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'cPlugin__OnDisconnect'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->cPlugin__OnDisconnect(a_Reason,a_Player);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'cPlugin__OnDisconnect'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: cPlugin__OnBlockPlace of class  Lua__cPlugin */
#ifndef TOLUA_DISABLE_tolua_AllToLua_Lua__cPlugin_cPlugin__OnBlockPlace00
static int tolua_AllToLua_Lua__cPlugin_cPlugin__OnBlockPlace00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Lua__cPlugin",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"cPacket_BlockPlace",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"cPlayer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Lua__cPlugin* self = (Lua__cPlugin*)  tolua_tousertype(tolua_S,1,0);
  cPacket_BlockPlace* a_PacketData = ((cPacket_BlockPlace*)  tolua_tousertype(tolua_S,2,0));
  cPlayer* a_Player = ((cPlayer*)  tolua_tousertype(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'cPlugin__OnBlockPlace'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->cPlugin__OnBlockPlace(a_PacketData,a_Player);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'cPlugin__OnBlockPlace'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: cPlugin__OnBlockDig of class  Lua__cPlugin */
#ifndef TOLUA_DISABLE_tolua_AllToLua_Lua__cPlugin_cPlugin__OnBlockDig00
static int tolua_AllToLua_Lua__cPlugin_cPlugin__OnBlockDig00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Lua__cPlugin",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"cPacket_BlockDig",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"cPlayer",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,4,"cItem",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Lua__cPlugin* self = (Lua__cPlugin*)  tolua_tousertype(tolua_S,1,0);
  cPacket_BlockDig* a_PacketData = ((cPacket_BlockDig*)  tolua_tousertype(tolua_S,2,0));
  cPlayer* a_Player = ((cPlayer*)  tolua_tousertype(tolua_S,3,0));
  cItem* a_PickupItem = ((cItem*)  tolua_tousertype(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'cPlugin__OnBlockDig'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->cPlugin__OnBlockDig(a_PacketData,a_Player,a_PickupItem);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'cPlugin__OnBlockDig'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: cPlugin__OnChat of class  Lua__cPlugin */
#ifndef TOLUA_DISABLE_tolua_AllToLua_Lua__cPlugin_cPlugin__OnChat00
static int tolua_AllToLua_Lua__cPlugin_cPlugin__OnChat00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Lua__cPlugin",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"cPlayer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Lua__cPlugin* self = (Lua__cPlugin*)  tolua_tousertype(tolua_S,1,0);
  std::string a_Chat = ((std::string)  tolua_tocppstring(tolua_S,2,0));
  cPlayer* a_Player = ((cPlayer*)  tolua_tousertype(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'cPlugin__OnChat'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->cPlugin__OnChat(a_Chat,a_Player);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'cPlugin__OnChat'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: cPlugin__OnLogin of class  Lua__cPlugin */
#ifndef TOLUA_DISABLE_tolua_AllToLua_Lua__cPlugin_cPlugin__OnLogin00
static int tolua_AllToLua_Lua__cPlugin_cPlugin__OnLogin00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Lua__cPlugin",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"cPacket_Login",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Lua__cPlugin* self = (Lua__cPlugin*)  tolua_tousertype(tolua_S,1,0);
  cPacket_Login* a_PacketData = ((cPacket_Login*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'cPlugin__OnLogin'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->cPlugin__OnLogin(a_PacketData);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'cPlugin__OnLogin'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: cPlugin__OnPlayerSpawn of class  Lua__cPlugin */
#ifndef TOLUA_DISABLE_tolua_AllToLua_Lua__cPlugin_cPlugin__OnPlayerSpawn00
static int tolua_AllToLua_Lua__cPlugin_cPlugin__OnPlayerSpawn00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Lua__cPlugin",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"cPlayer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Lua__cPlugin* self = (Lua__cPlugin*)  tolua_tousertype(tolua_S,1,0);
  cPlayer* a_Player = ((cPlayer*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'cPlugin__OnPlayerSpawn'", NULL);
#endif
  {
   self->cPlugin__OnPlayerSpawn(a_Player);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'cPlugin__OnPlayerSpawn'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: cPlugin__OnPlayerJoin of class  Lua__cPlugin */
#ifndef TOLUA_DISABLE_tolua_AllToLua_Lua__cPlugin_cPlugin__OnPlayerJoin00
static int tolua_AllToLua_Lua__cPlugin_cPlugin__OnPlayerJoin00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Lua__cPlugin",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"cPlayer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Lua__cPlugin* self = (Lua__cPlugin*)  tolua_tousertype(tolua_S,1,0);
  cPlayer* a_Player = ((cPlayer*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'cPlugin__OnPlayerJoin'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->cPlugin__OnPlayerJoin(a_Player);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'cPlugin__OnPlayerJoin'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: cPlugin__OnPlayerMove of class  Lua__cPlugin */
#ifndef TOLUA_DISABLE_tolua_AllToLua_Lua__cPlugin_cPlugin__OnPlayerMove00
static int tolua_AllToLua_Lua__cPlugin_cPlugin__OnPlayerMove00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Lua__cPlugin",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"cPlayer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Lua__cPlugin* self = (Lua__cPlugin*)  tolua_tousertype(tolua_S,1,0);
  cPlayer* a_Player = ((cPlayer*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'cPlugin__OnPlayerMove'", NULL);
#endif
  {
   self->cPlugin__OnPlayerMove(a_Player);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'cPlugin__OnPlayerMove'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: cPlugin__OnTakeDamage of class  Lua__cPlugin */
#ifndef TOLUA_DISABLE_tolua_AllToLua_Lua__cPlugin_cPlugin__OnTakeDamage00
static int tolua_AllToLua_Lua__cPlugin_cPlugin__OnTakeDamage00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Lua__cPlugin",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"cPawn",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"TakeDamageInfo",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Lua__cPlugin* self = (Lua__cPlugin*)  tolua_tousertype(tolua_S,1,0);
  cPawn* a_Pawn = ((cPawn*)  tolua_tousertype(tolua_S,2,0));
  TakeDamageInfo* a_TakeDamageInfo = ((TakeDamageInfo*)  tolua_tousertype(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'cPlugin__OnTakeDamage'", NULL);
#endif
  {
   self->cPlugin__OnTakeDamage(a_Pawn,a_TakeDamageInfo);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'cPlugin__OnTakeDamage'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: cPlugin__OnKilled of class  Lua__cPlugin */
#ifndef TOLUA_DISABLE_tolua_AllToLua_Lua__cPlugin_cPlugin__OnKilled00
static int tolua_AllToLua_Lua__cPlugin_cPlugin__OnKilled00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Lua__cPlugin",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"cPawn",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"cEntity",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Lua__cPlugin* self = (Lua__cPlugin*)  tolua_tousertype(tolua_S,1,0);
  cPawn* a_Killed = ((cPawn*)  tolua_tousertype(tolua_S,2,0));
  cEntity* a_Killer = ((cEntity*)  tolua_tousertype(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'cPlugin__OnKilled'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->cPlugin__OnKilled(a_Killed,a_Killer);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'cPlugin__OnKilled'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  Lua__cPlugin */
#ifndef TOLUA_DISABLE_tolua_AllToLua_Lua__cPlugin_new00
static int tolua_AllToLua_Lua__cPlugin_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Lua__cPlugin",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   Lua__cPlugin* tolua_ret = (Lua__cPlugin*)  Mtolua_new((Lua__cPlugin)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"Lua__cPlugin");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  Lua__cPlugin */
#ifndef TOLUA_DISABLE_tolua_AllToLua_Lua__cPlugin_new00_local
static int tolua_AllToLua_Lua__cPlugin_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Lua__cPlugin",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   Lua__cPlugin* tolua_ret = (Lua__cPlugin*)  Mtolua_new((Lua__cPlugin)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"Lua__cPlugin");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  Lua__cPlugin */
#ifndef TOLUA_DISABLE_tolua_AllToLua_Lua__cPlugin_delete00
static int tolua_AllToLua_Lua__cPlugin_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Lua__cPlugin",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Lua__cPlugin* self = (Lua__cPlugin*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'", NULL);
#endif
  Mtolua_delete(self);
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE


/* function to release collected object via destructor */
#ifdef __cplusplus

static int tolua_collect_Lua__cPlugin (lua_State* tolua_S)
{
 Lua__cPlugin* self = (Lua__cPlugin*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}
#endif

/* method: GetFileName of class  cPlugin_Lua */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cPlugin_Lua_GetFileName00
static int tolua_AllToLua_cPlugin_Lua_GetFileName00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cPlugin_Lua",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cPlugin_Lua* self = (cPlugin_Lua*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetFileName'", NULL);
#endif
  {
   std::string tolua_ret = (std::string)  self->GetFileName();
   tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetFileName'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetServer of class  cServer */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cServer_GetServer00
static int tolua_AllToLua_cServer_GetServer00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"cServer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   cServer* tolua_ret = (cServer*)  cServer::GetServer();
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"cServer");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetServer'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: ServerCommand of class  cServer */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cServer_ServerCommand00
static int tolua_AllToLua_cServer_ServerCommand00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cServer",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cServer* self = (cServer*)  tolua_tousertype(tolua_S,1,0);
  const char* a_Cmd = ((const char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'ServerCommand'", NULL);
#endif
  {
   self->ServerCommand(a_Cmd);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'ServerCommand'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SendMessage of class  cServer */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cServer_SendMessage00
static int tolua_AllToLua_cServer_SendMessage00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cServer",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"cPlayer",1,&tolua_err) ||
     !tolua_isboolean(tolua_S,4,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cServer* self = (cServer*)  tolua_tousertype(tolua_S,1,0);
  const char* a_Message = ((const char*)  tolua_tostring(tolua_S,2,0));
  cPlayer* a_Player = ((cPlayer*)  tolua_tousertype(tolua_S,3,0));
  bool a_bExclude = ((bool)  tolua_toboolean(tolua_S,4,false));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SendMessage'", NULL);
#endif
  {
   self->SendMessage(a_Message,a_Player,a_bExclude);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SendMessage'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetWorld of class  cWorld */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cWorld_GetWorld00
static int tolua_AllToLua_cWorld_GetWorld00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"cWorld",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   cWorld* tolua_ret = (cWorld*)  cWorld::GetWorld();
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"cWorld");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetWorld'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetTime of class  cWorld */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cWorld_GetTime00
static int tolua_AllToLua_cWorld_GetTime00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"cWorld",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   float tolua_ret = (float)  cWorld::GetTime();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetTime'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetWorldTime of class  cWorld */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cWorld_SetWorldTime00
static int tolua_AllToLua_cWorld_SetWorldTime00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cWorld",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cWorld* self = (cWorld*)  tolua_tousertype(tolua_S,1,0);
  long long a_WorldTime = ((long long)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetWorldTime'", NULL);
#endif
  {
   self->SetWorldTime(a_WorldTime);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetWorldTime'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetHeight of class  cWorld */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cWorld_GetHeight00
static int tolua_AllToLua_cWorld_GetHeight00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cWorld",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cWorld* self = (cWorld*)  tolua_tousertype(tolua_S,1,0);
  int a_X = ((int)  tolua_tonumber(tolua_S,2,0));
  int a_Z = ((int)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetHeight'", NULL);
#endif
  {
   char tolua_ret = (char)  self->GetHeight(a_X,a_Z);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetHeight'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetNumPlayers of class  cWorld */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cWorld_GetNumPlayers00
static int tolua_AllToLua_cWorld_GetNumPlayers00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cWorld",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cWorld* self = (cWorld*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetNumPlayers'", NULL);
#endif
  {
   unsigned int tolua_ret = (unsigned int)  self->GetNumPlayers();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetNumPlayers'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetPlayer of class  cWorld */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cWorld_GetPlayer00
static int tolua_AllToLua_cWorld_GetPlayer00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cWorld",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cWorld* self = (cWorld*)  tolua_tousertype(tolua_S,1,0);
  const char* a_PlayerName = ((const char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetPlayer'", NULL);
#endif
  {
   cPlayer* tolua_ret = (cPlayer*)  self->GetPlayer(a_PlayerName);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"cPlayer");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetPlayer'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetEntity of class  cWorld */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cWorld_GetEntity00
static int tolua_AllToLua_cWorld_GetEntity00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cWorld",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cWorld* self = (cWorld*)  tolua_tousertype(tolua_S,1,0);
  int a_UniqueID = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetEntity'", NULL);
#endif
  {
   cEntity* tolua_ret = (cEntity*)  self->GetEntity(a_UniqueID);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"cEntity");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetEntity'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetBlock of class  cWorld */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cWorld_SetBlock00
static int tolua_AllToLua_cWorld_SetBlock00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cWorld",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,6,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,7,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cWorld* self = (cWorld*)  tolua_tousertype(tolua_S,1,0);
  int a_X = ((int)  tolua_tonumber(tolua_S,2,0));
  int a_Y = ((int)  tolua_tonumber(tolua_S,3,0));
  int a_Z = ((int)  tolua_tonumber(tolua_S,4,0));
  char a_BlockType = ((char)  tolua_tonumber(tolua_S,5,0));
  char a_BlockMeta = ((char)  tolua_tonumber(tolua_S,6,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetBlock'", NULL);
#endif
  {
   self->SetBlock(a_X,a_Y,a_Z,a_BlockType,a_BlockMeta);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetBlock'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: FastSetBlock of class  cWorld */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cWorld_FastSetBlock00
static int tolua_AllToLua_cWorld_FastSetBlock00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cWorld",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,6,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,7,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cWorld* self = (cWorld*)  tolua_tousertype(tolua_S,1,0);
  int a_X = ((int)  tolua_tonumber(tolua_S,2,0));
  int a_Y = ((int)  tolua_tonumber(tolua_S,3,0));
  int a_Z = ((int)  tolua_tonumber(tolua_S,4,0));
  char a_BlockType = ((char)  tolua_tonumber(tolua_S,5,0));
  char a_BlockMeta = ((char)  tolua_tonumber(tolua_S,6,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'FastSetBlock'", NULL);
#endif
  {
   self->FastSetBlock(a_X,a_Y,a_Z,a_BlockType,a_BlockMeta);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'FastSetBlock'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetBlock of class  cWorld */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cWorld_GetBlock00
static int tolua_AllToLua_cWorld_GetBlock00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cWorld",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cWorld* self = (cWorld*)  tolua_tousertype(tolua_S,1,0);
  int a_X = ((int)  tolua_tonumber(tolua_S,2,0));
  int a_Y = ((int)  tolua_tonumber(tolua_S,3,0));
  int a_Z = ((int)  tolua_tonumber(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetBlock'", NULL);
#endif
  {
   char tolua_ret = (char)  self->GetBlock(a_X,a_Y,a_Z);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetBlock'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetBlockMeta of class  cWorld */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cWorld_GetBlockMeta00
static int tolua_AllToLua_cWorld_GetBlockMeta00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cWorld",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cWorld* self = (cWorld*)  tolua_tousertype(tolua_S,1,0);
  int a_X = ((int)  tolua_tonumber(tolua_S,2,0));
  int a_Y = ((int)  tolua_tonumber(tolua_S,3,0));
  int a_Z = ((int)  tolua_tonumber(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetBlockMeta'", NULL);
#endif
  {
   char tolua_ret = (char)  self->GetBlockMeta(a_X,a_Y,a_Z);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetBlockMeta'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetBlockMeta of class  cWorld */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cWorld_SetBlockMeta00
static int tolua_AllToLua_cWorld_SetBlockMeta00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cWorld",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cWorld* self = (cWorld*)  tolua_tousertype(tolua_S,1,0);
  int a_X = ((int)  tolua_tonumber(tolua_S,2,0));
  int a_Y = ((int)  tolua_tonumber(tolua_S,3,0));
  int a_Z = ((int)  tolua_tonumber(tolua_S,4,0));
  char a_MetaData = ((char)  tolua_tonumber(tolua_S,5,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetBlockMeta'", NULL);
#endif
  {
   self->SetBlockMeta(a_X,a_Y,a_Z,a_MetaData);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetBlockMeta'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: DigBlock of class  cWorld */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cWorld_DigBlock00
static int tolua_AllToLua_cWorld_DigBlock00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cWorld",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,5,&tolua_err) || !tolua_isusertype(tolua_S,5,"cItem",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cWorld* self = (cWorld*)  tolua_tousertype(tolua_S,1,0);
  int a_X = ((int)  tolua_tonumber(tolua_S,2,0));
  int a_Y = ((int)  tolua_tonumber(tolua_S,3,0));
  int a_Z = ((int)  tolua_tonumber(tolua_S,4,0));
  cItem* a_PickupItem = ((cItem*)  tolua_tousertype(tolua_S,5,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'DigBlock'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->DigBlock(a_X,a_Y,a_Z,*a_PickupItem);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'DigBlock'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SendBlockTo of class  cWorld */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cWorld_SendBlockTo00
static int tolua_AllToLua_cWorld_SendBlockTo00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cWorld",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isusertype(tolua_S,5,"cPlayer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cWorld* self = (cWorld*)  tolua_tousertype(tolua_S,1,0);
  int a_X = ((int)  tolua_tonumber(tolua_S,2,0));
  int a_Y = ((int)  tolua_tonumber(tolua_S,3,0));
  int a_Z = ((int)  tolua_tonumber(tolua_S,4,0));
  cPlayer* a_Player = ((cPlayer*)  tolua_tousertype(tolua_S,5,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SendBlockTo'", NULL);
#endif
  {
   self->SendBlockTo(a_X,a_Y,a_Z,a_Player);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SendBlockTo'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetSpawnX of class  cWorld */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cWorld_GetSpawnX00
static int tolua_AllToLua_cWorld_GetSpawnX00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cWorld",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cWorld* self = (cWorld*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetSpawnX'", NULL);
#endif
  {
   const double tolua_ret = (const double)  self->GetSpawnX();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetSpawnX'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetSpawnY of class  cWorld */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cWorld_GetSpawnY00
static int tolua_AllToLua_cWorld_GetSpawnY00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cWorld",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cWorld* self = (cWorld*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetSpawnY'", NULL);
#endif
  {
   const double tolua_ret = (const double)  self->GetSpawnY();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetSpawnY'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetSpawnZ of class  cWorld */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cWorld_GetSpawnZ00
static int tolua_AllToLua_cWorld_GetSpawnZ00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cWorld",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cWorld* self = (cWorld*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetSpawnZ'", NULL);
#endif
  {
   const double tolua_ret = (const double)  self->GetSpawnZ();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetSpawnZ'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetBlockEntity of class  cWorld */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cWorld_GetBlockEntity00
static int tolua_AllToLua_cWorld_GetBlockEntity00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cWorld",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cWorld* self = (cWorld*)  tolua_tousertype(tolua_S,1,0);
  int a_X = ((int)  tolua_tonumber(tolua_S,2,0));
  int a_Y = ((int)  tolua_tonumber(tolua_S,3,0));
  int a_Z = ((int)  tolua_tonumber(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetBlockEntity'", NULL);
#endif
  {
   cBlockEntity* tolua_ret = (cBlockEntity*)  self->GetBlockEntity(a_X,a_Y,a_Z);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"cBlockEntity");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetBlockEntity'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GrowTree of class  cWorld */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cWorld_GrowTree00
static int tolua_AllToLua_cWorld_GrowTree00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cWorld",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cWorld* self = (cWorld*)  tolua_tousertype(tolua_S,1,0);
  int a_X = ((int)  tolua_tonumber(tolua_S,2,0));
  int a_Y = ((int)  tolua_tonumber(tolua_S,3,0));
  int a_Z = ((int)  tolua_tonumber(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GrowTree'", NULL);
#endif
  {
   self->GrowTree(a_X,a_Y,a_Z);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GrowTree'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetWorldSeed of class  cWorld */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cWorld_GetWorldSeed00
static int tolua_AllToLua_cWorld_GetWorldSeed00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cWorld",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cWorld* self = (cWorld*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetWorldSeed'", NULL);
#endif
  {
   unsigned int tolua_ret = (unsigned int)  self->GetWorldSeed();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetWorldSeed'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: Clear of class  cInventory */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cInventory_Clear00
static int tolua_AllToLua_cInventory_Clear00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cInventory",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cInventory* self = (cInventory*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'Clear'", NULL);
#endif
  {
   self->Clear();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Clear'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: AddItem of class  cInventory */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cInventory_AddItem00
static int tolua_AllToLua_cInventory_AddItem00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cInventory",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"cItem",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cInventory* self = (cInventory*)  tolua_tousertype(tolua_S,1,0);
  cItem* a_Item = ((cItem*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'AddItem'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->AddItem(*a_Item);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'AddItem'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: RemoveItem of class  cInventory */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cInventory_RemoveItem00
static int tolua_AllToLua_cInventory_RemoveItem00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cInventory",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"cItem",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cInventory* self = (cInventory*)  tolua_tousertype(tolua_S,1,0);
  cItem* a_Item = ((cItem*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'RemoveItem'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->RemoveItem(*a_Item);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'RemoveItem'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetSlot of class  cInventory */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cInventory_GetSlot00
static int tolua_AllToLua_cInventory_GetSlot00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cInventory",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cInventory* self = (cInventory*)  tolua_tousertype(tolua_S,1,0);
  int a_SlotNum = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetSlot'", NULL);
#endif
  {
   cItem* tolua_ret = (cItem*)  self->GetSlot(a_SlotNum);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"cItem");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetSlot'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetFromHotBar of class  cInventory */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cInventory_GetFromHotBar00
static int tolua_AllToLua_cInventory_GetFromHotBar00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cInventory",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cInventory* self = (cInventory*)  tolua_tousertype(tolua_S,1,0);
  int a_SlotNum = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetFromHotBar'", NULL);
#endif
  {
   cItem* tolua_ret = (cItem*)  self->GetFromHotBar(a_SlotNum);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"cItem");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetFromHotBar'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetEquippedItem of class  cInventory */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cInventory_GetEquippedItem00
static int tolua_AllToLua_cInventory_GetEquippedItem00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cInventory",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cInventory* self = (cInventory*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetEquippedItem'", NULL);
#endif
  {
   cItem& tolua_ret = (cItem&)  self->GetEquippedItem();
    tolua_pushusertype(tolua_S,(void*)&tolua_ret,"cItem");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetEquippedItem'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetEquippedSlot of class  cInventory */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cInventory_SetEquippedSlot00
static int tolua_AllToLua_cInventory_SetEquippedSlot00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cInventory",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cInventory* self = (cInventory*)  tolua_tousertype(tolua_S,1,0);
  int a_SlotNum = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetEquippedSlot'", NULL);
#endif
  {
   self->SetEquippedSlot(a_SlotNum);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetEquippedSlot'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SendSlot of class  cInventory */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cInventory_SendSlot00
static int tolua_AllToLua_cInventory_SendSlot00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cInventory",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cInventory* self = (cInventory*)  tolua_tousertype(tolua_S,1,0);
  int a_SlotNum = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SendSlot'", NULL);
#endif
  {
   self->SendSlot(a_SlotNum);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SendSlot'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  cItem */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cItem_new00
static int tolua_AllToLua_cItem_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"cItem",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,1,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,1,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ENUM_ITEM_ID a_ItemID = ((ENUM_ITEM_ID) (int)  tolua_tonumber(tolua_S,2,E_ITEM_EMPTY));
  char a_ItemCount = ((char)  tolua_tonumber(tolua_S,3,0));
  short a_ItemHealth = ((short)  tolua_tonumber(tolua_S,4,0));
  {
   cItem* tolua_ret = (cItem*)  Mtolua_new((cItem)(a_ItemID,a_ItemCount,a_ItemHealth));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"cItem");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  cItem */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cItem_new00_local
static int tolua_AllToLua_cItem_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"cItem",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,1,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,1,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ENUM_ITEM_ID a_ItemID = ((ENUM_ITEM_ID) (int)  tolua_tonumber(tolua_S,2,E_ITEM_EMPTY));
  char a_ItemCount = ((char)  tolua_tonumber(tolua_S,3,0));
  short a_ItemHealth = ((short)  tolua_tonumber(tolua_S,4,0));
  {
   cItem* tolua_ret = (cItem*)  Mtolua_new((cItem)(a_ItemID,a_ItemCount,a_ItemHealth));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"cItem");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: Empty of class  cItem */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cItem_Empty00
static int tolua_AllToLua_cItem_Empty00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cItem",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cItem* self = (cItem*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'Empty'", NULL);
#endif
  {
   self->Empty();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Empty'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: IsEmpty of class  cItem */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cItem_IsEmpty00
static int tolua_AllToLua_cItem_IsEmpty00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cItem",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cItem* self = (cItem*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'IsEmpty'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->IsEmpty();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'IsEmpty'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: Equals of class  cItem */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cItem_Equals00
static int tolua_AllToLua_cItem_Equals00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cItem",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"cItem",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cItem* self = (cItem*)  tolua_tousertype(tolua_S,1,0);
  cItem* a_Item = ((cItem*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'Equals'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->Equals(*a_Item);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Equals'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* get function: m_ItemID of class  cItem */
#ifndef TOLUA_DISABLE_tolua_get_cItem_m_ItemID
static int tolua_get_cItem_m_ItemID(lua_State* tolua_S)
{
  cItem* self = (cItem*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_ItemID'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->m_ItemID);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: m_ItemID of class  cItem */
#ifndef TOLUA_DISABLE_tolua_set_cItem_m_ItemID
static int tolua_set_cItem_m_ItemID(lua_State* tolua_S)
{
  cItem* self = (cItem*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_ItemID'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_ItemID = ((ENUM_ITEM_ID) (int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: m_ItemCount of class  cItem */
#ifndef TOLUA_DISABLE_tolua_get_cItem_m_ItemCount
static int tolua_get_cItem_m_ItemCount(lua_State* tolua_S)
{
  cItem* self = (cItem*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_ItemCount'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->m_ItemCount);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: m_ItemCount of class  cItem */
#ifndef TOLUA_DISABLE_tolua_set_cItem_m_ItemCount
static int tolua_set_cItem_m_ItemCount(lua_State* tolua_S)
{
  cItem* self = (cItem*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_ItemCount'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_ItemCount = ((char)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: m_ItemHealth of class  cItem */
#ifndef TOLUA_DISABLE_tolua_get_cItem_m_ItemHealth
static int tolua_get_cItem_m_ItemHealth(lua_State* tolua_S)
{
  cItem* self = (cItem*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_ItemHealth'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->m_ItemHealth);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: m_ItemHealth of class  cItem */
#ifndef TOLUA_DISABLE_tolua_set_cItem_m_ItemHealth
static int tolua_set_cItem_m_ItemHealth(lua_State* tolua_S)
{
  cItem* self = (cItem*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_ItemHealth'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_ItemHealth = ((short)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: Method of class  HTTPRequest */
#ifndef TOLUA_DISABLE_tolua_get_HTTPRequest_Method
static int tolua_get_HTTPRequest_Method(lua_State* tolua_S)
{
  HTTPRequest* self = (HTTPRequest*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'Method'",NULL);
#endif
  tolua_pushcppstring(tolua_S,(const char*)self->Method);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: Method of class  HTTPRequest */
#ifndef TOLUA_DISABLE_tolua_set_HTTPRequest_Method
static int tolua_set_HTTPRequest_Method(lua_State* tolua_S)
{
  HTTPRequest* self = (HTTPRequest*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'Method'",NULL);
  if (!tolua_iscppstring(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->Method = ((std::string)  tolua_tocppstring(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: Path of class  HTTPRequest */
#ifndef TOLUA_DISABLE_tolua_get_HTTPRequest_Path
static int tolua_get_HTTPRequest_Path(lua_State* tolua_S)
{
  HTTPRequest* self = (HTTPRequest*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'Path'",NULL);
#endif
  tolua_pushcppstring(tolua_S,(const char*)self->Path);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: Path of class  HTTPRequest */
#ifndef TOLUA_DISABLE_tolua_set_HTTPRequest_Path
static int tolua_set_HTTPRequest_Path(lua_State* tolua_S)
{
  HTTPRequest* self = (HTTPRequest*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'Path'",NULL);
  if (!tolua_iscppstring(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->Path = ((std::string)  tolua_tocppstring(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: Params of class  HTTPRequest */
#ifndef TOLUA_DISABLE_tolua_get_HTTPRequest_Params_ptr
static int tolua_get_HTTPRequest_Params_ptr(lua_State* tolua_S)
{
  HTTPRequest* self = (HTTPRequest*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'Params'",NULL);
#endif
   tolua_pushusertype(tolua_S,(void*)self->Params,"cStringMap");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: Params of class  HTTPRequest */
#ifndef TOLUA_DISABLE_tolua_set_HTTPRequest_Params_ptr
static int tolua_set_HTTPRequest_Params_ptr(lua_State* tolua_S)
{
  HTTPRequest* self = (HTTPRequest*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'Params'",NULL);
  if (!tolua_isusertype(tolua_S,2,"cStringMap",0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->Params = ((cStringMap*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: Username of class  HTTPRequest */
#ifndef TOLUA_DISABLE_tolua_get_HTTPRequest_Username
static int tolua_get_HTTPRequest_Username(lua_State* tolua_S)
{
  HTTPRequest* self = (HTTPRequest*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'Username'",NULL);
#endif
  tolua_pushcppstring(tolua_S,(const char*)self->Username);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: Username of class  HTTPRequest */
#ifndef TOLUA_DISABLE_tolua_set_HTTPRequest_Username
static int tolua_set_HTTPRequest_Username(lua_State* tolua_S)
{
  HTTPRequest* self = (HTTPRequest*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'Username'",NULL);
  if (!tolua_iscppstring(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->Username = ((std::string)  tolua_tocppstring(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  cWebPlugin */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cWebPlugin_delete00
static int tolua_AllToLua_cWebPlugin_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cWebPlugin",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cWebPlugin* self = (cWebPlugin*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'", NULL);
#endif
  Mtolua_delete(self);
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetName of class  cWebPlugin */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cWebPlugin_SetName00
static int tolua_AllToLua_cWebPlugin_SetName00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cWebPlugin",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cWebPlugin* self = (cWebPlugin*)  tolua_tousertype(tolua_S,1,0);
  std::string a_Name = ((std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetName'", NULL);
#endif
  {
   self->SetName(a_Name);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetName'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetName of class  cWebPlugin */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cWebPlugin_GetName00
static int tolua_AllToLua_cWebPlugin_GetName00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cWebPlugin",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cWebPlugin* self = (cWebPlugin*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetName'", NULL);
#endif
  {
   std::string tolua_ret = (std::string)  self->GetName();
   tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetName'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: HandleRequest of class  cWebPlugin */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cWebPlugin_HandleRequest00
static int tolua_AllToLua_cWebPlugin_HandleRequest00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cWebPlugin",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"HTTPRequest",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cWebPlugin* self = (cWebPlugin*)  tolua_tousertype(tolua_S,1,0);
  HTTPRequest* a_Request = ((HTTPRequest*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'HandleRequest'", NULL);
#endif
  {
   std::string tolua_ret = (std::string)  self->HandleRequest(a_Request);
   tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'HandleRequest'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: Initialize of class  cWebPlugin */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cWebPlugin_Initialize00
static int tolua_AllToLua_cWebPlugin_Initialize00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cWebPlugin",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cWebPlugin* self = (cWebPlugin*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'Initialize'", NULL);
#endif
  {
   self->Initialize();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Initialize'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

 class Lua__cWebPlugin : public cWebPlugin, public ToluaBase {
public:
	 std::string  HandleRequest( HTTPRequest* a_Request) {
		if (push_method("HandleRequest",  tolua_AllToLua_cWebPlugin_HandleRequest00)) {
			tolua_pushusertype(lua_state, (void*)a_Request, "HTTPRequest");
			ToluaBase::dbcall(lua_state, 2, 1);
			 std::string  tolua_ret = ( std::string )tolua_tocppstring(lua_state, -1, 0);
			lua_pop(lua_state, 1);
			return tolua_ret;
		} else {
			if (lua_state)
				LOG("pure-virtual method cWebPlugin::HandleRequest not implemented.");
			else {
				LOG("pure-virtual method cWebPlugin::HandleRequest called with no lua_state. Aborting");
				::abort();
			};
			return "";
		};
	};
	 void  Initialize( void ) {
		if (push_method("Initialize",  tolua_AllToLua_cWebPlugin_Initialize00)) {
			ToluaBase::dbcall(lua_state, 1, 0);
		} else {
			if (lua_state)
				LOG("pure-virtual method cWebPlugin::Initialize not implemented.");
			else {
				LOG("pure-virtual method cWebPlugin::Initialize called with no lua_state. Aborting");
				::abort();
			};
			return ( void )0;
		};
	};

	 Lua__cWebPlugin( lua_State* L): cWebPlugin(L){};
};

/* method: tolua__set_instance of class  Lua__cWebPlugin */
#ifndef TOLUA_DISABLE_tolua_AllToLua_Lua__cWebPlugin_tolua__set_instance00
static int tolua_AllToLua_Lua__cWebPlugin_tolua__set_instance00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Lua__cWebPlugin",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Lua__cWebPlugin* self = (Lua__cWebPlugin*)  tolua_tousertype(tolua_S,1,0);
  lua_State* L =  tolua_S;
  lua_Object lo = ((lua_Object)  tolua_tovalue(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'tolua__set_instance'", NULL);
#endif
  {
   self->tolua__set_instance(L,lo);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'tolua__set_instance'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  Lua__cWebPlugin */
#ifndef TOLUA_DISABLE_tolua_AllToLua_Lua__cWebPlugin_new00
static int tolua_AllToLua_Lua__cWebPlugin_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Lua__cWebPlugin",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  lua_State* L =  tolua_S;
  {
   Lua__cWebPlugin* tolua_ret = (Lua__cWebPlugin*)  Mtolua_new((Lua__cWebPlugin)(L));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"Lua__cWebPlugin");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  Lua__cWebPlugin */
#ifndef TOLUA_DISABLE_tolua_AllToLua_Lua__cWebPlugin_new00_local
static int tolua_AllToLua_Lua__cWebPlugin_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Lua__cWebPlugin",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  lua_State* L =  tolua_S;
  {
   Lua__cWebPlugin* tolua_ret = (Lua__cWebPlugin*)  Mtolua_new((Lua__cWebPlugin)(L));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"Lua__cWebPlugin");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  Lua__cWebPlugin */
#ifndef TOLUA_DISABLE_tolua_AllToLua_Lua__cWebPlugin_delete00
static int tolua_AllToLua_Lua__cWebPlugin_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Lua__cWebPlugin",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Lua__cWebPlugin* self = (Lua__cWebPlugin*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'", NULL);
#endif
  Mtolua_delete(self);
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE


/* function to release collected object via destructor */
#ifdef __cplusplus

static int tolua_collect_Lua__cWebPlugin (lua_State* tolua_S)
{
 Lua__cWebPlugin* self = (Lua__cWebPlugin*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}
#endif

/* method: new of class  cPickup */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cPickup_new00
static int tolua_AllToLua_cPickup_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"cPickup",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,5,&tolua_err) || !tolua_isusertype(tolua_S,5,"const cItem",0,&tolua_err)) ||
     !tolua_isnumber(tolua_S,6,1,&tolua_err) ||
     !tolua_isnumber(tolua_S,7,1,&tolua_err) ||
     !tolua_isnumber(tolua_S,8,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,9,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  int a_X = ((int)  tolua_tonumber(tolua_S,2,0));
  int a_Y = ((int)  tolua_tonumber(tolua_S,3,0));
  int a_Z = ((int)  tolua_tonumber(tolua_S,4,0));
  const cItem* a_Item = ((const cItem*)  tolua_tousertype(tolua_S,5,0));
  float a_SpeedX = ((float)  tolua_tonumber(tolua_S,6,0.f));
  float a_SpeedY = ((float)  tolua_tonumber(tolua_S,7,0.f));
  float a_SpeedZ = ((float)  tolua_tonumber(tolua_S,8,0.f));
  {
   cPickup* tolua_ret = (cPickup*)  Mtolua_new((cPickup)(a_X,a_Y,a_Z,*a_Item,a_SpeedX,a_SpeedY,a_SpeedZ));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"cPickup");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  cPickup */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cPickup_new00_local
static int tolua_AllToLua_cPickup_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"cPickup",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,5,&tolua_err) || !tolua_isusertype(tolua_S,5,"const cItem",0,&tolua_err)) ||
     !tolua_isnumber(tolua_S,6,1,&tolua_err) ||
     !tolua_isnumber(tolua_S,7,1,&tolua_err) ||
     !tolua_isnumber(tolua_S,8,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,9,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  int a_X = ((int)  tolua_tonumber(tolua_S,2,0));
  int a_Y = ((int)  tolua_tonumber(tolua_S,3,0));
  int a_Z = ((int)  tolua_tonumber(tolua_S,4,0));
  const cItem* a_Item = ((const cItem*)  tolua_tousertype(tolua_S,5,0));
  float a_SpeedX = ((float)  tolua_tonumber(tolua_S,6,0.f));
  float a_SpeedY = ((float)  tolua_tonumber(tolua_S,7,0.f));
  float a_SpeedZ = ((float)  tolua_tonumber(tolua_S,8,0.f));
  {
   cPickup* tolua_ret = (cPickup*)  Mtolua_new((cPickup)(a_X,a_Y,a_Z,*a_Item,a_SpeedX,a_SpeedY,a_SpeedZ));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"cPickup");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  cPickup */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cPickup_new01
static int tolua_AllToLua_cPickup_new01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"cPickup",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"cPacket_PickupSpawn",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  cPacket_PickupSpawn* a_PickupSpawnPacket = ((cPacket_PickupSpawn*)  tolua_tousertype(tolua_S,2,0));
  {
   cPickup* tolua_ret = (cPickup*)  Mtolua_new((cPickup)(a_PickupSpawnPacket));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"cPickup");
  }
 }
 return 1;
tolua_lerror:
 return tolua_AllToLua_cPickup_new00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  cPickup */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cPickup_new01_local
static int tolua_AllToLua_cPickup_new01_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"cPickup",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"cPacket_PickupSpawn",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  cPacket_PickupSpawn* a_PickupSpawnPacket = ((cPacket_PickupSpawn*)  tolua_tousertype(tolua_S,2,0));
  {
   cPickup* tolua_ret = (cPickup*)  Mtolua_new((cPickup)(a_PickupSpawnPacket));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"cPickup");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
  }
 }
 return 1;
tolua_lerror:
 return tolua_AllToLua_cPickup_new00_local(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  cPickup */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cPickup_delete00
static int tolua_AllToLua_cPickup_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cPickup",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cPickup* self = (cPickup*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'", NULL);
#endif
  Mtolua_delete(self);
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetItem of class  cPickup */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cPickup_GetItem00
static int tolua_AllToLua_cPickup_GetItem00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cPickup",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cPickup* self = (cPickup*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetItem'", NULL);
#endif
  {
   cItem* tolua_ret = (cItem*)  self->GetItem();
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"cItem");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetItem'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: CollectedBy of class  cPickup */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cPickup_CollectedBy00
static int tolua_AllToLua_cPickup_CollectedBy00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cPickup",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"cPlayer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cPickup* self = (cPickup*)  tolua_tousertype(tolua_S,1,0);
  cPlayer* a_Dest = ((cPlayer*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'CollectedBy'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->CollectedBy(a_Dest);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'CollectedBy'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

 class Lua__cPickup : public cPickup, public ToluaBase {
public:
	 bool  CollectedBy( cPlayer* a_Dest) {
		if (push_method("CollectedBy",  tolua_AllToLua_cPickup_CollectedBy00)) {
			tolua_pushusertype(lua_state, (void*)a_Dest, "cPlayer");
			ToluaBase::dbcall(lua_state, 2, 1);
			 bool  tolua_ret = ( bool )tolua_toboolean(lua_state, -1, 0);
			lua_pop(lua_state, 1);
			return tolua_ret;
		} else {
			return ( bool ) cPickup:: CollectedBy(a_Dest);
		};
	};
	 void  Initialize( void ) {
		if (push_method("Initialize",  tolua_AllToLua_cEntity_Initialize00)) {
			ToluaBase::dbcall(lua_state, 1, 0);
		} else {
			return ( void ) cPickup:: Initialize();
		};
	};
	unsigned int  GetEntityType( void ) {
		if (push_method("GetEntityType",  tolua_AllToLua_cEntity_GetEntityType00)) {
			ToluaBase::dbcall(lua_state, 1, 1);
			 unsigned int  tolua_ret = (unsigned int )tolua_tonumber(lua_state, -1, 0);
			lua_pop(lua_state, 1);
			return tolua_ret;
		} else {
			return (unsigned int ) cPickup:: GetEntityType();
		};
	};
	 bool  IsA( const char* a_EntityType) {
		if (push_method("IsA",  tolua_AllToLua_cEntity_IsA00)) {
			tolua_pushstring(lua_state, (const char*)a_EntityType);
			ToluaBase::dbcall(lua_state, 2, 1);
			 bool  tolua_ret = ( bool )tolua_toboolean(lua_state, -1, 0);
			lua_pop(lua_state, 1);
			return tolua_ret;
		} else {
			return ( bool ) cPickup:: IsA(a_EntityType);
		};
	};
	 const char*  GetClass( void ) {
		if (push_method("GetClass",  tolua_AllToLua_cEntity_GetClass00)) {
			ToluaBase::dbcall(lua_state, 1, 1);
			 const char*  tolua_ret = ( const char* )tolua_tostring(lua_state, -1, 0);
			lua_pop(lua_state, 1);
			return tolua_ret;
		} else {
			return ( const char* ) cPickup:: GetClass();
		};
	};
	 void  Tick( float a_Dt) {
		if (push_method("Tick",  tolua_AllToLua_cEntity_Tick00)) {
			tolua_pushnumber(lua_state, (lua_Number)a_Dt);
			ToluaBase::dbcall(lua_state, 2, 0);
		} else {
			if (lua_state)
				LOG("pure-virtual method cPickup::Tick not implemented.");
			else {
				LOG("pure-virtual method cPickup::Tick called with no lua_state. Aborting");
				::abort();
			};
			return ( void )0;
		};
	};
	 void  SpawnOn( cClientHandle* a_Target) {
		if (push_method("SpawnOn",  tolua_AllToLua_cEntity_SpawnOn00)) {
			tolua_pushusertype(lua_state, (void*)a_Target, "cClientHandle");
			ToluaBase::dbcall(lua_state, 2, 0);
		} else {
			if (lua_state)
				LOG("pure-virtual method cPickup::SpawnOn not implemented.");
			else {
				LOG("pure-virtual method cPickup::SpawnOn called with no lua_state. Aborting");
				::abort();
			};
			return ( void )0;
		};
	};

	 bool cPickup__CollectedBy( cPlayer* a_Dest) {
		return ( bool )cPickup::CollectedBy(a_Dest);
	};
	 void cPickup__Initialize( void ) {
		return ( void )cPickup::Initialize();
	};
	unsigned int cPickup__GetEntityType( void ) {
		return (unsigned int )cPickup::GetEntityType();
	};
	 bool cPickup__IsA( const char* a_EntityType) {
		return ( bool )cPickup::IsA(a_EntityType);
	};
	 const char* cPickup__GetClass( void ) {
		return ( const char* )cPickup::GetClass();
	};
	 Lua__cPickup( int a_X, int a_Y, int a_Z, const cItem& a_Item, float a_SpeedX = 0.f, float a_SpeedY = 0.f, float a_SpeedZ = 0.f): cPickup(a_X,a_Y,a_Z,a_Item,a_SpeedX,a_SpeedY,a_SpeedZ){};
	 Lua__cPickup( cPacket_PickupSpawn* a_PickupSpawnPacket): cPickup(a_PickupSpawnPacket){};
};

/* method: tolua__set_instance of class  Lua__cPickup */
#ifndef TOLUA_DISABLE_tolua_AllToLua_Lua__cPickup_tolua__set_instance00
static int tolua_AllToLua_Lua__cPickup_tolua__set_instance00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Lua__cPickup",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Lua__cPickup* self = (Lua__cPickup*)  tolua_tousertype(tolua_S,1,0);
  lua_State* L =  tolua_S;
  lua_Object lo = ((lua_Object)  tolua_tovalue(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'tolua__set_instance'", NULL);
#endif
  {
   self->tolua__set_instance(L,lo);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'tolua__set_instance'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: cPickup__CollectedBy of class  Lua__cPickup */
#ifndef TOLUA_DISABLE_tolua_AllToLua_Lua__cPickup_cPickup__CollectedBy00
static int tolua_AllToLua_Lua__cPickup_cPickup__CollectedBy00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Lua__cPickup",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"cPlayer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Lua__cPickup* self = (Lua__cPickup*)  tolua_tousertype(tolua_S,1,0);
  cPlayer* a_Dest = ((cPlayer*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'cPickup__CollectedBy'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->cPickup__CollectedBy(a_Dest);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'cPickup__CollectedBy'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  Lua__cPickup */
#ifndef TOLUA_DISABLE_tolua_AllToLua_Lua__cPickup_new00
static int tolua_AllToLua_Lua__cPickup_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Lua__cPickup",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,5,&tolua_err) || !tolua_isusertype(tolua_S,5,"cItem",0,&tolua_err)) ||
     !tolua_isnumber(tolua_S,6,1,&tolua_err) ||
     !tolua_isnumber(tolua_S,7,1,&tolua_err) ||
     !tolua_isnumber(tolua_S,8,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,9,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  int a_X = ((int)  tolua_tonumber(tolua_S,2,0));
  int a_Y = ((int)  tolua_tonumber(tolua_S,3,0));
  int a_Z = ((int)  tolua_tonumber(tolua_S,4,0));
  const cItem* a_Item = ((const cItem*)  tolua_tousertype(tolua_S,5,0));
  float a_SpeedX = ((float)  tolua_tonumber(tolua_S,6,0.f));
  float a_SpeedY = ((float)  tolua_tonumber(tolua_S,7,0.f));
  float a_SpeedZ = ((float)  tolua_tonumber(tolua_S,8,0.f));
  {
   Lua__cPickup* tolua_ret = (Lua__cPickup*)  Mtolua_new((Lua__cPickup)(a_X,a_Y,a_Z,*a_Item,a_SpeedX,a_SpeedY,a_SpeedZ));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"Lua__cPickup");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  Lua__cPickup */
#ifndef TOLUA_DISABLE_tolua_AllToLua_Lua__cPickup_new00_local
static int tolua_AllToLua_Lua__cPickup_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Lua__cPickup",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,5,&tolua_err) || !tolua_isusertype(tolua_S,5,"cItem",0,&tolua_err)) ||
     !tolua_isnumber(tolua_S,6,1,&tolua_err) ||
     !tolua_isnumber(tolua_S,7,1,&tolua_err) ||
     !tolua_isnumber(tolua_S,8,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,9,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  int a_X = ((int)  tolua_tonumber(tolua_S,2,0));
  int a_Y = ((int)  tolua_tonumber(tolua_S,3,0));
  int a_Z = ((int)  tolua_tonumber(tolua_S,4,0));
  const cItem* a_Item = ((const cItem*)  tolua_tousertype(tolua_S,5,0));
  float a_SpeedX = ((float)  tolua_tonumber(tolua_S,6,0.f));
  float a_SpeedY = ((float)  tolua_tonumber(tolua_S,7,0.f));
  float a_SpeedZ = ((float)  tolua_tonumber(tolua_S,8,0.f));
  {
   Lua__cPickup* tolua_ret = (Lua__cPickup*)  Mtolua_new((Lua__cPickup)(a_X,a_Y,a_Z,*a_Item,a_SpeedX,a_SpeedY,a_SpeedZ));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"Lua__cPickup");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  Lua__cPickup */
#ifndef TOLUA_DISABLE_tolua_AllToLua_Lua__cPickup_new01
static int tolua_AllToLua_Lua__cPickup_new01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Lua__cPickup",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"cPacket_PickupSpawn",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  cPacket_PickupSpawn* a_PickupSpawnPacket = ((cPacket_PickupSpawn*)  tolua_tousertype(tolua_S,2,0));
  {
   Lua__cPickup* tolua_ret = (Lua__cPickup*)  Mtolua_new((Lua__cPickup)(a_PickupSpawnPacket));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"Lua__cPickup");
  }
 }
 return 1;
tolua_lerror:
 return tolua_AllToLua_Lua__cPickup_new00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  Lua__cPickup */
#ifndef TOLUA_DISABLE_tolua_AllToLua_Lua__cPickup_new01_local
static int tolua_AllToLua_Lua__cPickup_new01_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Lua__cPickup",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"cPacket_PickupSpawn",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  cPacket_PickupSpawn* a_PickupSpawnPacket = ((cPacket_PickupSpawn*)  tolua_tousertype(tolua_S,2,0));
  {
   Lua__cPickup* tolua_ret = (Lua__cPickup*)  Mtolua_new((Lua__cPickup)(a_PickupSpawnPacket));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"Lua__cPickup");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
  }
 }
 return 1;
tolua_lerror:
 return tolua_AllToLua_Lua__cPickup_new00_local(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  Lua__cPickup */
#ifndef TOLUA_DISABLE_tolua_AllToLua_Lua__cPickup_delete00
static int tolua_AllToLua_Lua__cPickup_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Lua__cPickup",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Lua__cPickup* self = (Lua__cPickup*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'", NULL);
#endif
  Mtolua_delete(self);
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE


/* function to release collected object via destructor */
#ifdef __cplusplus

static int tolua_collect_Lua__cPickup (lua_State* tolua_S)
{
 Lua__cPickup* self = (Lua__cPickup*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}
#endif

/* method: Get of class  cRoot */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cRoot_Get00
static int tolua_AllToLua_cRoot_Get00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"cRoot",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   cRoot* tolua_ret = (cRoot*)  cRoot::Get();
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"cRoot");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Get'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetServer of class  cRoot */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cRoot_GetServer00
static int tolua_AllToLua_cRoot_GetServer00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cRoot",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cRoot* self = (cRoot*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetServer'", NULL);
#endif
  {
   cServer* tolua_ret = (cServer*)  self->GetServer();
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"cServer");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetServer'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetWorld of class  cRoot */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cRoot_GetWorld00
static int tolua_AllToLua_cRoot_GetWorld00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cRoot",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cRoot* self = (cRoot*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetWorld'", NULL);
#endif
  {
   cWorld* tolua_ret = (cWorld*)  self->GetWorld();
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"cWorld");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetWorld'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetGroupManager of class  cRoot */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cRoot_GetGroupManager00
static int tolua_AllToLua_cRoot_GetGroupManager00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cRoot",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cRoot* self = (cRoot*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetGroupManager'", NULL);
#endif
  {
   cGroupManager* tolua_ret = (cGroupManager*)  self->GetGroupManager();
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"cGroupManager");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetGroupManager'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetRecipeChecker of class  cRoot */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cRoot_GetRecipeChecker00
static int tolua_AllToLua_cRoot_GetRecipeChecker00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cRoot",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cRoot* self = (cRoot*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetRecipeChecker'", NULL);
#endif
  {
   cRecipeChecker* tolua_ret = (cRecipeChecker*)  self->GetRecipeChecker();
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"cRecipeChecker");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetRecipeChecker'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetFurnaceRecipe of class  cRoot */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cRoot_GetFurnaceRecipe00
static int tolua_AllToLua_cRoot_GetFurnaceRecipe00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cRoot",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cRoot* self = (cRoot*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetFurnaceRecipe'", NULL);
#endif
  {
   cFurnaceRecipe* tolua_ret = (cFurnaceRecipe*)  self->GetFurnaceRecipe();
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"cFurnaceRecipe");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetFurnaceRecipe'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetWebAdmin of class  cRoot */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cRoot_GetWebAdmin00
static int tolua_AllToLua_cRoot_GetWebAdmin00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cRoot",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cRoot* self = (cRoot*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetWebAdmin'", NULL);
#endif
  {
   cWebAdmin* tolua_ret = (cWebAdmin*)  self->GetWebAdmin();
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"cWebAdmin");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetWebAdmin'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetPluginManager of class  cRoot */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cRoot_GetPluginManager00
static int tolua_AllToLua_cRoot_GetPluginManager00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cRoot",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cRoot* self = (cRoot*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetPluginManager'", NULL);
#endif
  {
   cPluginManager* tolua_ret = (cPluginManager*)  self->GetPluginManager();
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"cPluginManager");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetPluginManager'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: ServerCommand of class  cRoot */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cRoot_ServerCommand00
static int tolua_AllToLua_cRoot_ServerCommand00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cRoot",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cRoot* self = (cRoot*)  tolua_tousertype(tolua_S,1,0);
  const char* a_Cmd = ((const char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'ServerCommand'", NULL);
#endif
  {
   self->ServerCommand(a_Cmd);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'ServerCommand'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  cTCPLink */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cTCPLink_delete00
static int tolua_AllToLua_cTCPLink_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cTCPLink",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cTCPLink* self = (cTCPLink*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'", NULL);
#endif
  Mtolua_delete(self);
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: Connect of class  cTCPLink */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cTCPLink_Connect00
static int tolua_AllToLua_cTCPLink_Connect00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cTCPLink",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cTCPLink* self = (cTCPLink*)  tolua_tousertype(tolua_S,1,0);
  const char* a_Address = ((const char*)  tolua_tostring(tolua_S,2,0));
  unsigned int a_Port = ((unsigned int)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'Connect'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->Connect(a_Address,a_Port);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Connect'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: Send of class  cTCPLink */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cTCPLink_Send00
static int tolua_AllToLua_cTCPLink_Send00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cTCPLink",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cTCPLink* self = (cTCPLink*)  tolua_tousertype(tolua_S,1,0);
  char* a_Data = ((char*)  tolua_tostring(tolua_S,2,0));
  unsigned int a_Size = ((unsigned int)  tolua_tonumber(tolua_S,3,0));
  int a_Flags = ((int)  tolua_tonumber(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'Send'", NULL);
#endif
  {
   int tolua_ret = (int)  self->Send(a_Data,a_Size,a_Flags);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Send'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SendMessage of class  cTCPLink */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cTCPLink_SendMessage00
static int tolua_AllToLua_cTCPLink_SendMessage00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cTCPLink",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cTCPLink* self = (cTCPLink*)  tolua_tousertype(tolua_S,1,0);
  const char* a_Message = ((const char*)  tolua_tostring(tolua_S,2,0));
  int a_Flags = ((int)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SendMessage'", NULL);
#endif
  {
   int tolua_ret = (int)  self->SendMessage(a_Message,a_Flags);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SendMessage'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: CloseSocket of class  cTCPLink */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cTCPLink_CloseSocket00
static int tolua_AllToLua_cTCPLink_CloseSocket00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cTCPLink",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cTCPLink* self = (cTCPLink*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'CloseSocket'", NULL);
#endif
  {
   self->CloseSocket();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'CloseSocket'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

 class Lua__cTCPLink : public cTCPLink, public ToluaBase {
public:
	 void  ReceivedData( char* a_Data, int a_Size) {
		if (push_method("ReceivedData",  NULL)) {
			tolua_pushstring(lua_state, (const char*)a_Data);
			tolua_pushnumber(lua_state, (lua_Number)a_Size);
			ToluaBase::dbcall(lua_state, 3, 0);
		} else {
			if (lua_state)
				LOG("pure-virtual method cTCPLink::ReceivedData not implemented.");
			else {
				LOG("pure-virtual method cTCPLink::ReceivedData called with no lua_state. Aborting");
				::abort();
			};
			return ( void )0;
		};
	};

	 Lua__cTCPLink( void ): cTCPLink(){};
};

/* method: tolua__set_instance of class  Lua__cTCPLink */
#ifndef TOLUA_DISABLE_tolua_AllToLua_Lua__cTCPLink_tolua__set_instance00
static int tolua_AllToLua_Lua__cTCPLink_tolua__set_instance00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Lua__cTCPLink",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Lua__cTCPLink* self = (Lua__cTCPLink*)  tolua_tousertype(tolua_S,1,0);
  lua_State* L =  tolua_S;
  lua_Object lo = ((lua_Object)  tolua_tovalue(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'tolua__set_instance'", NULL);
#endif
  {
   self->tolua__set_instance(L,lo);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'tolua__set_instance'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  Lua__cTCPLink */
#ifndef TOLUA_DISABLE_tolua_AllToLua_Lua__cTCPLink_new00
static int tolua_AllToLua_Lua__cTCPLink_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Lua__cTCPLink",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   Lua__cTCPLink* tolua_ret = (Lua__cTCPLink*)  Mtolua_new((Lua__cTCPLink)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"Lua__cTCPLink");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  Lua__cTCPLink */
#ifndef TOLUA_DISABLE_tolua_AllToLua_Lua__cTCPLink_new00_local
static int tolua_AllToLua_Lua__cTCPLink_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Lua__cTCPLink",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   Lua__cTCPLink* tolua_ret = (Lua__cTCPLink*)  Mtolua_new((Lua__cTCPLink)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"Lua__cTCPLink");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  Lua__cTCPLink */
#ifndef TOLUA_DISABLE_tolua_AllToLua_Lua__cTCPLink_delete00
static int tolua_AllToLua_Lua__cTCPLink_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Lua__cTCPLink",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Lua__cTCPLink* self = (Lua__cTCPLink*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'", NULL);
#endif
  Mtolua_delete(self);
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE


/* function to release collected object via destructor */
#ifdef __cplusplus

static int tolua_collect_Lua__cTCPLink (lua_State* tolua_S)
{
 Lua__cTCPLink* self = (Lua__cTCPLink*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}
#endif

/* method: new of class  Vector3f */
#ifndef TOLUA_DISABLE_tolua_AllToLua_Vector3f_new00
static int tolua_AllToLua_Vector3f_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Vector3f",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const Vector3d",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Vector3d* v = ((const Vector3d*)  tolua_tousertype(tolua_S,2,0));
  {
   Vector3f* tolua_ret = (Vector3f*)  Mtolua_new((Vector3f)(*v));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"Vector3f");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  Vector3f */
#ifndef TOLUA_DISABLE_tolua_AllToLua_Vector3f_new00_local
static int tolua_AllToLua_Vector3f_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Vector3f",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const Vector3d",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Vector3d* v = ((const Vector3d*)  tolua_tousertype(tolua_S,2,0));
  {
   Vector3f* tolua_ret = (Vector3f*)  Mtolua_new((Vector3f)(*v));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"Vector3f");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  Vector3f */
#ifndef TOLUA_DISABLE_tolua_AllToLua_Vector3f_new01
static int tolua_AllToLua_Vector3f_new01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Vector3f",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Vector3d",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  const Vector3d* v = ((const Vector3d*)  tolua_tousertype(tolua_S,2,0));
  {
   Vector3f* tolua_ret = (Vector3f*)  Mtolua_new((Vector3f)(v));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"Vector3f");
  }
 }
 return 1;
tolua_lerror:
 return tolua_AllToLua_Vector3f_new00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  Vector3f */
#ifndef TOLUA_DISABLE_tolua_AllToLua_Vector3f_new01_local
static int tolua_AllToLua_Vector3f_new01_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Vector3f",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Vector3d",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  const Vector3d* v = ((const Vector3d*)  tolua_tousertype(tolua_S,2,0));
  {
   Vector3f* tolua_ret = (Vector3f*)  Mtolua_new((Vector3f)(v));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"Vector3f");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
  }
 }
 return 1;
tolua_lerror:
 return tolua_AllToLua_Vector3f_new00_local(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  Vector3f */
#ifndef TOLUA_DISABLE_tolua_AllToLua_Vector3f_new02
static int tolua_AllToLua_Vector3f_new02(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Vector3f",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const Vector3i",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  const Vector3i* v = ((const Vector3i*)  tolua_tousertype(tolua_S,2,0));
  {
   Vector3f* tolua_ret = (Vector3f*)  Mtolua_new((Vector3f)(*v));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"Vector3f");
  }
 }
 return 1;
tolua_lerror:
 return tolua_AllToLua_Vector3f_new01(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  Vector3f */
#ifndef TOLUA_DISABLE_tolua_AllToLua_Vector3f_new02_local
static int tolua_AllToLua_Vector3f_new02_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Vector3f",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const Vector3i",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  const Vector3i* v = ((const Vector3i*)  tolua_tousertype(tolua_S,2,0));
  {
   Vector3f* tolua_ret = (Vector3f*)  Mtolua_new((Vector3f)(*v));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"Vector3f");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
  }
 }
 return 1;
tolua_lerror:
 return tolua_AllToLua_Vector3f_new01_local(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  Vector3f */
#ifndef TOLUA_DISABLE_tolua_AllToLua_Vector3f_new03
static int tolua_AllToLua_Vector3f_new03(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Vector3f",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Vector3i",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  const Vector3i* v = ((const Vector3i*)  tolua_tousertype(tolua_S,2,0));
  {
   Vector3f* tolua_ret = (Vector3f*)  Mtolua_new((Vector3f)(v));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"Vector3f");
  }
 }
 return 1;
tolua_lerror:
 return tolua_AllToLua_Vector3f_new02(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  Vector3f */
#ifndef TOLUA_DISABLE_tolua_AllToLua_Vector3f_new03_local
static int tolua_AllToLua_Vector3f_new03_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Vector3f",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Vector3i",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  const Vector3i* v = ((const Vector3i*)  tolua_tousertype(tolua_S,2,0));
  {
   Vector3f* tolua_ret = (Vector3f*)  Mtolua_new((Vector3f)(v));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"Vector3f");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
  }
 }
 return 1;
tolua_lerror:
 return tolua_AllToLua_Vector3f_new02_local(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  Vector3f */
#ifndef TOLUA_DISABLE_tolua_AllToLua_Vector3f_new04
static int tolua_AllToLua_Vector3f_new04(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Vector3f",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  {
   Vector3f* tolua_ret = (Vector3f*)  Mtolua_new((Vector3f)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"Vector3f");
  }
 }
 return 1;
tolua_lerror:
 return tolua_AllToLua_Vector3f_new03(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  Vector3f */
#ifndef TOLUA_DISABLE_tolua_AllToLua_Vector3f_new04_local
static int tolua_AllToLua_Vector3f_new04_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Vector3f",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  {
   Vector3f* tolua_ret = (Vector3f*)  Mtolua_new((Vector3f)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"Vector3f");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
  }
 }
 return 1;
tolua_lerror:
 return tolua_AllToLua_Vector3f_new03_local(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  Vector3f */
#ifndef TOLUA_DISABLE_tolua_AllToLua_Vector3f_new05
static int tolua_AllToLua_Vector3f_new05(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Vector3f",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  float a_x = ((float)  tolua_tonumber(tolua_S,2,0));
  float a_y = ((float)  tolua_tonumber(tolua_S,3,0));
  float a_z = ((float)  tolua_tonumber(tolua_S,4,0));
  {
   Vector3f* tolua_ret = (Vector3f*)  Mtolua_new((Vector3f)(a_x,a_y,a_z));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"Vector3f");
  }
 }
 return 1;
tolua_lerror:
 return tolua_AllToLua_Vector3f_new04(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  Vector3f */
#ifndef TOLUA_DISABLE_tolua_AllToLua_Vector3f_new05_local
static int tolua_AllToLua_Vector3f_new05_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Vector3f",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  float a_x = ((float)  tolua_tonumber(tolua_S,2,0));
  float a_y = ((float)  tolua_tonumber(tolua_S,3,0));
  float a_z = ((float)  tolua_tonumber(tolua_S,4,0));
  {
   Vector3f* tolua_ret = (Vector3f*)  Mtolua_new((Vector3f)(a_x,a_y,a_z));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"Vector3f");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
  }
 }
 return 1;
tolua_lerror:
 return tolua_AllToLua_Vector3f_new04_local(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: Set of class  Vector3f */
#ifndef TOLUA_DISABLE_tolua_AllToLua_Vector3f_Set00
static int tolua_AllToLua_Vector3f_Set00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Vector3f",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Vector3f* self = (Vector3f*)  tolua_tousertype(tolua_S,1,0);
  float a_x = ((float)  tolua_tonumber(tolua_S,2,0));
  float a_y = ((float)  tolua_tonumber(tolua_S,3,0));
  float a_z = ((float)  tolua_tonumber(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'Set'", NULL);
#endif
  {
   self->Set(a_x,a_y,a_z);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Set'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: Normalize of class  Vector3f */
#ifndef TOLUA_DISABLE_tolua_AllToLua_Vector3f_Normalize00
static int tolua_AllToLua_Vector3f_Normalize00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Vector3f",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Vector3f* self = (Vector3f*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'Normalize'", NULL);
#endif
  {
   self->Normalize();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Normalize'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: NormalizeCopy of class  Vector3f */
#ifndef TOLUA_DISABLE_tolua_AllToLua_Vector3f_NormalizeCopy00
static int tolua_AllToLua_Vector3f_NormalizeCopy00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Vector3f",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Vector3f* self = (const Vector3f*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'NormalizeCopy'", NULL);
#endif
  {
   Vector3f tolua_ret = (Vector3f)  self->NormalizeCopy();
   {
#ifdef __cplusplus
    void* tolua_obj = Mtolua_new((Vector3f)(tolua_ret));
     tolua_pushusertype(tolua_S,tolua_obj,"Vector3f");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(Vector3f));
     tolua_pushusertype(tolua_S,tolua_obj,"Vector3f");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'NormalizeCopy'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: NormalizeCopy of class  Vector3f */
#ifndef TOLUA_DISABLE_tolua_AllToLua_Vector3f_NormalizeCopy01
static int tolua_AllToLua_Vector3f_NormalizeCopy01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Vector3f",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"Vector3f",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  const Vector3f* self = (const Vector3f*)  tolua_tousertype(tolua_S,1,0);
  Vector3f* a_V = ((Vector3f*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'NormalizeCopy'", NULL);
#endif
  {
   self->NormalizeCopy(*a_V);
  }
 }
 return 0;
tolua_lerror:
 return tolua_AllToLua_Vector3f_NormalizeCopy00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: Length of class  Vector3f */
#ifndef TOLUA_DISABLE_tolua_AllToLua_Vector3f_Length00
static int tolua_AllToLua_Vector3f_Length00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Vector3f",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Vector3f* self = (const Vector3f*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'Length'", NULL);
#endif
  {
   float tolua_ret = (float)  self->Length();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Length'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SqrLength of class  Vector3f */
#ifndef TOLUA_DISABLE_tolua_AllToLua_Vector3f_SqrLength00
static int tolua_AllToLua_Vector3f_SqrLength00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Vector3f",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Vector3f* self = (const Vector3f*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SqrLength'", NULL);
#endif
  {
   float tolua_ret = (float)  self->SqrLength();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SqrLength'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: Dot of class  Vector3f */
#ifndef TOLUA_DISABLE_tolua_AllToLua_Vector3f_Dot00
static int tolua_AllToLua_Vector3f_Dot00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Vector3f",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const Vector3f",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Vector3f* self = (const Vector3f*)  tolua_tousertype(tolua_S,1,0);
  const Vector3f* a_V = ((const Vector3f*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'Dot'", NULL);
#endif
  {
   float tolua_ret = (float)  self->Dot(*a_V);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Dot'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: Cross of class  Vector3f */
#ifndef TOLUA_DISABLE_tolua_AllToLua_Vector3f_Cross00
static int tolua_AllToLua_Vector3f_Cross00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Vector3f",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const Vector3f",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Vector3f* self = (const Vector3f*)  tolua_tousertype(tolua_S,1,0);
  const Vector3f* v = ((const Vector3f*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'Cross'", NULL);
#endif
  {
   Vector3f tolua_ret = (Vector3f)  self->Cross(*v);
   {
#ifdef __cplusplus
    void* tolua_obj = Mtolua_new((Vector3f)(tolua_ret));
     tolua_pushusertype(tolua_S,tolua_obj,"Vector3f");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(Vector3f));
     tolua_pushusertype(tolua_S,tolua_obj,"Vector3f");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Cross'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: Equals of class  Vector3f */
#ifndef TOLUA_DISABLE_tolua_AllToLua_Vector3f_Equals00
static int tolua_AllToLua_Vector3f_Equals00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Vector3f",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const Vector3f",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Vector3f* self = (const Vector3f*)  tolua_tousertype(tolua_S,1,0);
  const Vector3f* v = ((const Vector3f*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'Equals'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->Equals(*v);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Equals'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: operator+ of class  Vector3f */
#ifndef TOLUA_DISABLE_tolua_AllToLua_Vector3f__add00
static int tolua_AllToLua_Vector3f__add00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Vector3f",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const Vector3f",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Vector3f* self = (const Vector3f*)  tolua_tousertype(tolua_S,1,0);
  const Vector3f* v2 = ((const Vector3f*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'operator+'", NULL);
#endif
  {
   Vector3f tolua_ret = (Vector3f)  self->operator+(*v2);
   {
#ifdef __cplusplus
    void* tolua_obj = Mtolua_new((Vector3f)(tolua_ret));
     tolua_pushusertype(tolua_S,tolua_obj,"Vector3f");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(Vector3f));
     tolua_pushusertype(tolua_S,tolua_obj,"Vector3f");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function '.add'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: operator+ of class  Vector3f */
#ifndef TOLUA_DISABLE_tolua_AllToLua_Vector3f__add01
static int tolua_AllToLua_Vector3f__add01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Vector3f",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Vector3f",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  const Vector3f* self = (const Vector3f*)  tolua_tousertype(tolua_S,1,0);
  const Vector3f* v2 = ((const Vector3f*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'operator+'", NULL);
#endif
  {
   Vector3f tolua_ret = (Vector3f)  self->operator+(v2);
   {
#ifdef __cplusplus
    void* tolua_obj = Mtolua_new((Vector3f)(tolua_ret));
     tolua_pushusertype(tolua_S,tolua_obj,"Vector3f");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(Vector3f));
     tolua_pushusertype(tolua_S,tolua_obj,"Vector3f");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#endif
   }
  }
 }
 return 1;
tolua_lerror:
 return tolua_AllToLua_Vector3f__add00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: operator- of class  Vector3f */
#ifndef TOLUA_DISABLE_tolua_AllToLua_Vector3f__sub00
static int tolua_AllToLua_Vector3f__sub00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Vector3f",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const Vector3f",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Vector3f* self = (const Vector3f*)  tolua_tousertype(tolua_S,1,0);
  const Vector3f* v2 = ((const Vector3f*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'operator-'", NULL);
#endif
  {
   Vector3f tolua_ret = (Vector3f)  self->operator-(*v2);
   {
#ifdef __cplusplus
    void* tolua_obj = Mtolua_new((Vector3f)(tolua_ret));
     tolua_pushusertype(tolua_S,tolua_obj,"Vector3f");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(Vector3f));
     tolua_pushusertype(tolua_S,tolua_obj,"Vector3f");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function '.sub'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: operator- of class  Vector3f */
#ifndef TOLUA_DISABLE_tolua_AllToLua_Vector3f__sub01
static int tolua_AllToLua_Vector3f__sub01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Vector3f",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Vector3f",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  const Vector3f* self = (const Vector3f*)  tolua_tousertype(tolua_S,1,0);
  const Vector3f* v2 = ((const Vector3f*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'operator-'", NULL);
#endif
  {
   Vector3f tolua_ret = (Vector3f)  self->operator-(v2);
   {
#ifdef __cplusplus
    void* tolua_obj = Mtolua_new((Vector3f)(tolua_ret));
     tolua_pushusertype(tolua_S,tolua_obj,"Vector3f");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(Vector3f));
     tolua_pushusertype(tolua_S,tolua_obj,"Vector3f");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#endif
   }
  }
 }
 return 1;
tolua_lerror:
 return tolua_AllToLua_Vector3f__sub00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: operator* of class  Vector3f */
#ifndef TOLUA_DISABLE_tolua_AllToLua_Vector3f__mul00
static int tolua_AllToLua_Vector3f__mul00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Vector3f",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Vector3f* self = (const Vector3f*)  tolua_tousertype(tolua_S,1,0);
  const float f = ((const float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'operator*'", NULL);
#endif
  {
   Vector3f tolua_ret = (Vector3f)  self->operator*(f);
   {
#ifdef __cplusplus
    void* tolua_obj = Mtolua_new((Vector3f)(tolua_ret));
     tolua_pushusertype(tolua_S,tolua_obj,"Vector3f");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(Vector3f));
     tolua_pushusertype(tolua_S,tolua_obj,"Vector3f");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function '.mul'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: operator* of class  Vector3f */
#ifndef TOLUA_DISABLE_tolua_AllToLua_Vector3f__mul01
static int tolua_AllToLua_Vector3f__mul01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Vector3f",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const Vector3f",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  const Vector3f* self = (const Vector3f*)  tolua_tousertype(tolua_S,1,0);
  const Vector3f* v2 = ((const Vector3f*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'operator*'", NULL);
#endif
  {
   Vector3f tolua_ret = (Vector3f)  self->operator*(*v2);
   {
#ifdef __cplusplus
    void* tolua_obj = Mtolua_new((Vector3f)(tolua_ret));
     tolua_pushusertype(tolua_S,tolua_obj,"Vector3f");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(Vector3f));
     tolua_pushusertype(tolua_S,tolua_obj,"Vector3f");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#endif
   }
  }
 }
 return 1;
tolua_lerror:
 return tolua_AllToLua_Vector3f__mul00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* get function: x of class  Vector3f */
#ifndef TOLUA_DISABLE_tolua_get_Vector3f_x
static int tolua_get_Vector3f_x(lua_State* tolua_S)
{
  Vector3f* self = (Vector3f*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'x'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->x);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: x of class  Vector3f */
#ifndef TOLUA_DISABLE_tolua_set_Vector3f_x
static int tolua_set_Vector3f_x(lua_State* tolua_S)
{
  Vector3f* self = (Vector3f*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'x'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->x = ((float)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: y of class  Vector3f */
#ifndef TOLUA_DISABLE_tolua_get_Vector3f_y
static int tolua_get_Vector3f_y(lua_State* tolua_S)
{
  Vector3f* self = (Vector3f*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'y'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->y);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: y of class  Vector3f */
#ifndef TOLUA_DISABLE_tolua_set_Vector3f_y
static int tolua_set_Vector3f_y(lua_State* tolua_S)
{
  Vector3f* self = (Vector3f*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'y'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->y = ((float)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: z of class  Vector3f */
#ifndef TOLUA_DISABLE_tolua_get_Vector3f_z
static int tolua_get_Vector3f_z(lua_State* tolua_S)
{
  Vector3f* self = (Vector3f*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'z'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->z);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: z of class  Vector3f */
#ifndef TOLUA_DISABLE_tolua_set_Vector3f_z
static int tolua_set_Vector3f_z(lua_State* tolua_S)
{
  Vector3f* self = (Vector3f*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'z'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->z = ((float)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  Vector3d */
#ifndef TOLUA_DISABLE_tolua_AllToLua_Vector3d_new00
static int tolua_AllToLua_Vector3d_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Vector3d",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const Vector3f",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Vector3f* v = ((const Vector3f*)  tolua_tousertype(tolua_S,2,0));
  {
   Vector3d* tolua_ret = (Vector3d*)  Mtolua_new((Vector3d)(*v));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"Vector3d");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  Vector3d */
#ifndef TOLUA_DISABLE_tolua_AllToLua_Vector3d_new00_local
static int tolua_AllToLua_Vector3d_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Vector3d",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const Vector3f",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Vector3f* v = ((const Vector3f*)  tolua_tousertype(tolua_S,2,0));
  {
   Vector3d* tolua_ret = (Vector3d*)  Mtolua_new((Vector3d)(*v));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"Vector3d");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  Vector3d */
#ifndef TOLUA_DISABLE_tolua_AllToLua_Vector3d_new01
static int tolua_AllToLua_Vector3d_new01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Vector3d",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Vector3f",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  const Vector3f* v = ((const Vector3f*)  tolua_tousertype(tolua_S,2,0));
  {
   Vector3d* tolua_ret = (Vector3d*)  Mtolua_new((Vector3d)(v));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"Vector3d");
  }
 }
 return 1;
tolua_lerror:
 return tolua_AllToLua_Vector3d_new00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  Vector3d */
#ifndef TOLUA_DISABLE_tolua_AllToLua_Vector3d_new01_local
static int tolua_AllToLua_Vector3d_new01_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Vector3d",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Vector3f",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  const Vector3f* v = ((const Vector3f*)  tolua_tousertype(tolua_S,2,0));
  {
   Vector3d* tolua_ret = (Vector3d*)  Mtolua_new((Vector3d)(v));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"Vector3d");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
  }
 }
 return 1;
tolua_lerror:
 return tolua_AllToLua_Vector3d_new00_local(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  Vector3d */
#ifndef TOLUA_DISABLE_tolua_AllToLua_Vector3d_new02
static int tolua_AllToLua_Vector3d_new02(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Vector3d",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  {
   Vector3d* tolua_ret = (Vector3d*)  Mtolua_new((Vector3d)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"Vector3d");
  }
 }
 return 1;
tolua_lerror:
 return tolua_AllToLua_Vector3d_new01(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  Vector3d */
#ifndef TOLUA_DISABLE_tolua_AllToLua_Vector3d_new02_local
static int tolua_AllToLua_Vector3d_new02_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Vector3d",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  {
   Vector3d* tolua_ret = (Vector3d*)  Mtolua_new((Vector3d)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"Vector3d");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
  }
 }
 return 1;
tolua_lerror:
 return tolua_AllToLua_Vector3d_new01_local(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  Vector3d */
#ifndef TOLUA_DISABLE_tolua_AllToLua_Vector3d_new03
static int tolua_AllToLua_Vector3d_new03(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Vector3d",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  double a_x = ((double)  tolua_tonumber(tolua_S,2,0));
  double a_y = ((double)  tolua_tonumber(tolua_S,3,0));
  double a_z = ((double)  tolua_tonumber(tolua_S,4,0));
  {
   Vector3d* tolua_ret = (Vector3d*)  Mtolua_new((Vector3d)(a_x,a_y,a_z));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"Vector3d");
  }
 }
 return 1;
tolua_lerror:
 return tolua_AllToLua_Vector3d_new02(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  Vector3d */
#ifndef TOLUA_DISABLE_tolua_AllToLua_Vector3d_new03_local
static int tolua_AllToLua_Vector3d_new03_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Vector3d",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  double a_x = ((double)  tolua_tonumber(tolua_S,2,0));
  double a_y = ((double)  tolua_tonumber(tolua_S,3,0));
  double a_z = ((double)  tolua_tonumber(tolua_S,4,0));
  {
   Vector3d* tolua_ret = (Vector3d*)  Mtolua_new((Vector3d)(a_x,a_y,a_z));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"Vector3d");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
  }
 }
 return 1;
tolua_lerror:
 return tolua_AllToLua_Vector3d_new02_local(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: Set of class  Vector3d */
#ifndef TOLUA_DISABLE_tolua_AllToLua_Vector3d_Set00
static int tolua_AllToLua_Vector3d_Set00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Vector3d",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Vector3d* self = (Vector3d*)  tolua_tousertype(tolua_S,1,0);
  double a_x = ((double)  tolua_tonumber(tolua_S,2,0));
  double a_y = ((double)  tolua_tonumber(tolua_S,3,0));
  double a_z = ((double)  tolua_tonumber(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'Set'", NULL);
#endif
  {
   self->Set(a_x,a_y,a_z);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Set'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: Normalize of class  Vector3d */
#ifndef TOLUA_DISABLE_tolua_AllToLua_Vector3d_Normalize00
static int tolua_AllToLua_Vector3d_Normalize00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Vector3d",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Vector3d* self = (Vector3d*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'Normalize'", NULL);
#endif
  {
   self->Normalize();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Normalize'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: NormalizeCopy of class  Vector3d */
#ifndef TOLUA_DISABLE_tolua_AllToLua_Vector3d_NormalizeCopy00
static int tolua_AllToLua_Vector3d_NormalizeCopy00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Vector3d",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Vector3d* self = (Vector3d*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'NormalizeCopy'", NULL);
#endif
  {
   Vector3d tolua_ret = (Vector3d)  self->NormalizeCopy();
   {
#ifdef __cplusplus
    void* tolua_obj = Mtolua_new((Vector3d)(tolua_ret));
     tolua_pushusertype(tolua_S,tolua_obj,"Vector3d");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(Vector3d));
     tolua_pushusertype(tolua_S,tolua_obj,"Vector3d");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'NormalizeCopy'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: NormalizeCopy of class  Vector3d */
#ifndef TOLUA_DISABLE_tolua_AllToLua_Vector3d_NormalizeCopy01
static int tolua_AllToLua_Vector3d_NormalizeCopy01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Vector3d",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"Vector3d",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  Vector3d* self = (Vector3d*)  tolua_tousertype(tolua_S,1,0);
  Vector3d* a_V = ((Vector3d*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'NormalizeCopy'", NULL);
#endif
  {
   self->NormalizeCopy(*a_V);
  }
 }
 return 0;
tolua_lerror:
 return tolua_AllToLua_Vector3d_NormalizeCopy00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: Length of class  Vector3d */
#ifndef TOLUA_DISABLE_tolua_AllToLua_Vector3d_Length00
static int tolua_AllToLua_Vector3d_Length00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Vector3d",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Vector3d* self = (const Vector3d*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'Length'", NULL);
#endif
  {
   double tolua_ret = (double)  self->Length();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Length'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SqrLength of class  Vector3d */
#ifndef TOLUA_DISABLE_tolua_AllToLua_Vector3d_SqrLength00
static int tolua_AllToLua_Vector3d_SqrLength00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Vector3d",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Vector3d* self = (const Vector3d*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SqrLength'", NULL);
#endif
  {
   double tolua_ret = (double)  self->SqrLength();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SqrLength'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: Dot of class  Vector3d */
#ifndef TOLUA_DISABLE_tolua_AllToLua_Vector3d_Dot00
static int tolua_AllToLua_Vector3d_Dot00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Vector3d",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const Vector3d",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Vector3d* self = (const Vector3d*)  tolua_tousertype(tolua_S,1,0);
  const Vector3d* a_V = ((const Vector3d*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'Dot'", NULL);
#endif
  {
   double tolua_ret = (double)  self->Dot(*a_V);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Dot'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: Cross of class  Vector3d */
#ifndef TOLUA_DISABLE_tolua_AllToLua_Vector3d_Cross00
static int tolua_AllToLua_Vector3d_Cross00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Vector3d",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const Vector3d",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Vector3d* self = (const Vector3d*)  tolua_tousertype(tolua_S,1,0);
  const Vector3d* v = ((const Vector3d*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'Cross'", NULL);
#endif
  {
   Vector3d tolua_ret = (Vector3d)  self->Cross(*v);
   {
#ifdef __cplusplus
    void* tolua_obj = Mtolua_new((Vector3d)(tolua_ret));
     tolua_pushusertype(tolua_S,tolua_obj,"Vector3d");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(Vector3d));
     tolua_pushusertype(tolua_S,tolua_obj,"Vector3d");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Cross'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: operator+ of class  Vector3d */
#ifndef TOLUA_DISABLE_tolua_AllToLua_Vector3d__add00
static int tolua_AllToLua_Vector3d__add00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Vector3d",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const Vector3d",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Vector3d* self = (const Vector3d*)  tolua_tousertype(tolua_S,1,0);
  const Vector3d* v2 = ((const Vector3d*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'operator+'", NULL);
#endif
  {
   Vector3d tolua_ret = (Vector3d)  self->operator+(*v2);
   {
#ifdef __cplusplus
    void* tolua_obj = Mtolua_new((Vector3d)(tolua_ret));
     tolua_pushusertype(tolua_S,tolua_obj,"Vector3d");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(Vector3d));
     tolua_pushusertype(tolua_S,tolua_obj,"Vector3d");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function '.add'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: operator+ of class  Vector3d */
#ifndef TOLUA_DISABLE_tolua_AllToLua_Vector3d__add01
static int tolua_AllToLua_Vector3d__add01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Vector3d",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Vector3d",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  const Vector3d* self = (const Vector3d*)  tolua_tousertype(tolua_S,1,0);
  const Vector3d* v2 = ((const Vector3d*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'operator+'", NULL);
#endif
  {
   Vector3d tolua_ret = (Vector3d)  self->operator+(v2);
   {
#ifdef __cplusplus
    void* tolua_obj = Mtolua_new((Vector3d)(tolua_ret));
     tolua_pushusertype(tolua_S,tolua_obj,"Vector3d");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(Vector3d));
     tolua_pushusertype(tolua_S,tolua_obj,"Vector3d");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#endif
   }
  }
 }
 return 1;
tolua_lerror:
 return tolua_AllToLua_Vector3d__add00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: operator- of class  Vector3d */
#ifndef TOLUA_DISABLE_tolua_AllToLua_Vector3d__sub00
static int tolua_AllToLua_Vector3d__sub00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Vector3d",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const Vector3d",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Vector3d* self = (const Vector3d*)  tolua_tousertype(tolua_S,1,0);
  const Vector3d* v2 = ((const Vector3d*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'operator-'", NULL);
#endif
  {
   Vector3d tolua_ret = (Vector3d)  self->operator-(*v2);
   {
#ifdef __cplusplus
    void* tolua_obj = Mtolua_new((Vector3d)(tolua_ret));
     tolua_pushusertype(tolua_S,tolua_obj,"Vector3d");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(Vector3d));
     tolua_pushusertype(tolua_S,tolua_obj,"Vector3d");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function '.sub'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: operator- of class  Vector3d */
#ifndef TOLUA_DISABLE_tolua_AllToLua_Vector3d__sub01
static int tolua_AllToLua_Vector3d__sub01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Vector3d",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Vector3d",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  const Vector3d* self = (const Vector3d*)  tolua_tousertype(tolua_S,1,0);
  const Vector3d* v2 = ((const Vector3d*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'operator-'", NULL);
#endif
  {
   Vector3d tolua_ret = (Vector3d)  self->operator-(v2);
   {
#ifdef __cplusplus
    void* tolua_obj = Mtolua_new((Vector3d)(tolua_ret));
     tolua_pushusertype(tolua_S,tolua_obj,"Vector3d");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(Vector3d));
     tolua_pushusertype(tolua_S,tolua_obj,"Vector3d");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#endif
   }
  }
 }
 return 1;
tolua_lerror:
 return tolua_AllToLua_Vector3d__sub00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: operator* of class  Vector3d */
#ifndef TOLUA_DISABLE_tolua_AllToLua_Vector3d__mul00
static int tolua_AllToLua_Vector3d__mul00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Vector3d",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Vector3d* self = (const Vector3d*)  tolua_tousertype(tolua_S,1,0);
  const double f = ((const double)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'operator*'", NULL);
#endif
  {
   Vector3d tolua_ret = (Vector3d)  self->operator*(f);
   {
#ifdef __cplusplus
    void* tolua_obj = Mtolua_new((Vector3d)(tolua_ret));
     tolua_pushusertype(tolua_S,tolua_obj,"Vector3d");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(Vector3d));
     tolua_pushusertype(tolua_S,tolua_obj,"Vector3d");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function '.mul'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: operator* of class  Vector3d */
#ifndef TOLUA_DISABLE_tolua_AllToLua_Vector3d__mul01
static int tolua_AllToLua_Vector3d__mul01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Vector3d",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const Vector3d",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  const Vector3d* self = (const Vector3d*)  tolua_tousertype(tolua_S,1,0);
  const Vector3d* v2 = ((const Vector3d*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'operator*'", NULL);
#endif
  {
   Vector3d tolua_ret = (Vector3d)  self->operator*(*v2);
   {
#ifdef __cplusplus
    void* tolua_obj = Mtolua_new((Vector3d)(tolua_ret));
     tolua_pushusertype(tolua_S,tolua_obj,"Vector3d");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(Vector3d));
     tolua_pushusertype(tolua_S,tolua_obj,"Vector3d");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#endif
   }
  }
 }
 return 1;
tolua_lerror:
 return tolua_AllToLua_Vector3d__mul00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* get function: x of class  Vector3d */
#ifndef TOLUA_DISABLE_tolua_get_Vector3d_x
static int tolua_get_Vector3d_x(lua_State* tolua_S)
{
  Vector3d* self = (Vector3d*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'x'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->x);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: x of class  Vector3d */
#ifndef TOLUA_DISABLE_tolua_set_Vector3d_x
static int tolua_set_Vector3d_x(lua_State* tolua_S)
{
  Vector3d* self = (Vector3d*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'x'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->x = ((double)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: y of class  Vector3d */
#ifndef TOLUA_DISABLE_tolua_get_Vector3d_y
static int tolua_get_Vector3d_y(lua_State* tolua_S)
{
  Vector3d* self = (Vector3d*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'y'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->y);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: y of class  Vector3d */
#ifndef TOLUA_DISABLE_tolua_set_Vector3d_y
static int tolua_set_Vector3d_y(lua_State* tolua_S)
{
  Vector3d* self = (Vector3d*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'y'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->y = ((double)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: z of class  Vector3d */
#ifndef TOLUA_DISABLE_tolua_get_Vector3d_z
static int tolua_get_Vector3d_z(lua_State* tolua_S)
{
  Vector3d* self = (Vector3d*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'z'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->z);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: z of class  Vector3d */
#ifndef TOLUA_DISABLE_tolua_set_Vector3d_z
static int tolua_set_Vector3d_z(lua_State* tolua_S)
{
  Vector3d* self = (Vector3d*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'z'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->z = ((double)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  Vector3i */
#ifndef TOLUA_DISABLE_tolua_AllToLua_Vector3i_new00
static int tolua_AllToLua_Vector3i_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Vector3i",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const Vector3d",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Vector3d* v = ((const Vector3d*)  tolua_tousertype(tolua_S,2,0));
  {
   Vector3i* tolua_ret = (Vector3i*)  Mtolua_new((Vector3i)(*v));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"Vector3i");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  Vector3i */
#ifndef TOLUA_DISABLE_tolua_AllToLua_Vector3i_new00_local
static int tolua_AllToLua_Vector3i_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Vector3i",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const Vector3d",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Vector3d* v = ((const Vector3d*)  tolua_tousertype(tolua_S,2,0));
  {
   Vector3i* tolua_ret = (Vector3i*)  Mtolua_new((Vector3i)(*v));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"Vector3i");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  Vector3i */
#ifndef TOLUA_DISABLE_tolua_AllToLua_Vector3i_new01
static int tolua_AllToLua_Vector3i_new01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Vector3i",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  {
   Vector3i* tolua_ret = (Vector3i*)  Mtolua_new((Vector3i)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"Vector3i");
  }
 }
 return 1;
tolua_lerror:
 return tolua_AllToLua_Vector3i_new00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  Vector3i */
#ifndef TOLUA_DISABLE_tolua_AllToLua_Vector3i_new01_local
static int tolua_AllToLua_Vector3i_new01_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Vector3i",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  {
   Vector3i* tolua_ret = (Vector3i*)  Mtolua_new((Vector3i)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"Vector3i");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
  }
 }
 return 1;
tolua_lerror:
 return tolua_AllToLua_Vector3i_new00_local(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  Vector3i */
#ifndef TOLUA_DISABLE_tolua_AllToLua_Vector3i_new02
static int tolua_AllToLua_Vector3i_new02(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Vector3i",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  int a_x = ((int)  tolua_tonumber(tolua_S,2,0));
  int a_y = ((int)  tolua_tonumber(tolua_S,3,0));
  int a_z = ((int)  tolua_tonumber(tolua_S,4,0));
  {
   Vector3i* tolua_ret = (Vector3i*)  Mtolua_new((Vector3i)(a_x,a_y,a_z));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"Vector3i");
  }
 }
 return 1;
tolua_lerror:
 return tolua_AllToLua_Vector3i_new01(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  Vector3i */
#ifndef TOLUA_DISABLE_tolua_AllToLua_Vector3i_new02_local
static int tolua_AllToLua_Vector3i_new02_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Vector3i",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  int a_x = ((int)  tolua_tonumber(tolua_S,2,0));
  int a_y = ((int)  tolua_tonumber(tolua_S,3,0));
  int a_z = ((int)  tolua_tonumber(tolua_S,4,0));
  {
   Vector3i* tolua_ret = (Vector3i*)  Mtolua_new((Vector3i)(a_x,a_y,a_z));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"Vector3i");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
  }
 }
 return 1;
tolua_lerror:
 return tolua_AllToLua_Vector3i_new01_local(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: Set of class  Vector3i */
#ifndef TOLUA_DISABLE_tolua_AllToLua_Vector3i_Set00
static int tolua_AllToLua_Vector3i_Set00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Vector3i",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Vector3i* self = (Vector3i*)  tolua_tousertype(tolua_S,1,0);
  int a_x = ((int)  tolua_tonumber(tolua_S,2,0));
  int a_y = ((int)  tolua_tonumber(tolua_S,3,0));
  int a_z = ((int)  tolua_tonumber(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'Set'", NULL);
#endif
  {
   self->Set(a_x,a_y,a_z);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Set'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: Length of class  Vector3i */
#ifndef TOLUA_DISABLE_tolua_AllToLua_Vector3i_Length00
static int tolua_AllToLua_Vector3i_Length00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Vector3i",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Vector3i* self = (const Vector3i*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'Length'", NULL);
#endif
  {
   float tolua_ret = (float)  self->Length();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Length'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SqrLength of class  Vector3i */
#ifndef TOLUA_DISABLE_tolua_AllToLua_Vector3i_SqrLength00
static int tolua_AllToLua_Vector3i_SqrLength00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Vector3i",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Vector3i* self = (const Vector3i*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SqrLength'", NULL);
#endif
  {
   int tolua_ret = (int)  self->SqrLength();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SqrLength'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: Equals of class  Vector3i */
#ifndef TOLUA_DISABLE_tolua_AllToLua_Vector3i_Equals00
static int tolua_AllToLua_Vector3i_Equals00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Vector3i",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const Vector3i",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Vector3i* self = (const Vector3i*)  tolua_tousertype(tolua_S,1,0);
  const Vector3i* v = ((const Vector3i*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'Equals'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->Equals(*v);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Equals'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: Equals of class  Vector3i */
#ifndef TOLUA_DISABLE_tolua_AllToLua_Vector3i_Equals01
static int tolua_AllToLua_Vector3i_Equals01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Vector3i",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Vector3i",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  const Vector3i* self = (const Vector3i*)  tolua_tousertype(tolua_S,1,0);
  const Vector3i* v = ((const Vector3i*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'Equals'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->Equals(v);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
tolua_lerror:
 return tolua_AllToLua_Vector3i_Equals00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* get function: x of class  Vector3i */
#ifndef TOLUA_DISABLE_tolua_get_Vector3i_x
static int tolua_get_Vector3i_x(lua_State* tolua_S)
{
  Vector3i* self = (Vector3i*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'x'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->x);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: x of class  Vector3i */
#ifndef TOLUA_DISABLE_tolua_set_Vector3i_x
static int tolua_set_Vector3i_x(lua_State* tolua_S)
{
  Vector3i* self = (Vector3i*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'x'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->x = ((int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: y of class  Vector3i */
#ifndef TOLUA_DISABLE_tolua_get_Vector3i_y
static int tolua_get_Vector3i_y(lua_State* tolua_S)
{
  Vector3i* self = (Vector3i*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'y'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->y);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: y of class  Vector3i */
#ifndef TOLUA_DISABLE_tolua_set_Vector3i_y
static int tolua_set_Vector3i_y(lua_State* tolua_S)
{
  Vector3i* self = (Vector3i*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'y'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->y = ((int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: z of class  Vector3i */
#ifndef TOLUA_DISABLE_tolua_get_Vector3i_z
static int tolua_get_Vector3i_z(lua_State* tolua_S)
{
  Vector3i* self = (Vector3i*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'z'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->z);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: z of class  Vector3i */
#ifndef TOLUA_DISABLE_tolua_set_Vector3i_z
static int tolua_set_Vector3i_z(lua_State* tolua_S)
{
  Vector3i* self = (Vector3i*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'z'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->z = ((int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  cCuboid */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cCuboid_new00
static int tolua_AllToLua_cCuboid_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"cCuboid",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   cCuboid* tolua_ret = (cCuboid*)  Mtolua_new((cCuboid)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"cCuboid");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  cCuboid */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cCuboid_new00_local
static int tolua_AllToLua_cCuboid_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"cCuboid",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   cCuboid* tolua_ret = (cCuboid*)  Mtolua_new((cCuboid)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"cCuboid");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  cCuboid */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cCuboid_new01
static int tolua_AllToLua_cCuboid_new01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"cCuboid",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const cCuboid",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  const cCuboid* a_Cuboid = ((const cCuboid*)  tolua_tousertype(tolua_S,2,0));
  {
   cCuboid* tolua_ret = (cCuboid*)  Mtolua_new((cCuboid)(*a_Cuboid));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"cCuboid");
  }
 }
 return 1;
tolua_lerror:
 return tolua_AllToLua_cCuboid_new00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  cCuboid */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cCuboid_new01_local
static int tolua_AllToLua_cCuboid_new01_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"cCuboid",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const cCuboid",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  const cCuboid* a_Cuboid = ((const cCuboid*)  tolua_tousertype(tolua_S,2,0));
  {
   cCuboid* tolua_ret = (cCuboid*)  Mtolua_new((cCuboid)(*a_Cuboid));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"cCuboid");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
  }
 }
 return 1;
tolua_lerror:
 return tolua_AllToLua_cCuboid_new00_local(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  cCuboid */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cCuboid_new02
static int tolua_AllToLua_cCuboid_new02(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"cCuboid",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const Vector3i",0,&tolua_err)) ||
     (tolua_isvaluenil(tolua_S,3,&tolua_err) || !tolua_isusertype(tolua_S,3,"const Vector3i",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  const Vector3i* a_p1 = ((const Vector3i*)  tolua_tousertype(tolua_S,2,0));
  const Vector3i* a_p2 = ((const Vector3i*)  tolua_tousertype(tolua_S,3,0));
  {
   cCuboid* tolua_ret = (cCuboid*)  Mtolua_new((cCuboid)(*a_p1,*a_p2));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"cCuboid");
  }
 }
 return 1;
tolua_lerror:
 return tolua_AllToLua_cCuboid_new01(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  cCuboid */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cCuboid_new02_local
static int tolua_AllToLua_cCuboid_new02_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"cCuboid",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const Vector3i",0,&tolua_err)) ||
     (tolua_isvaluenil(tolua_S,3,&tolua_err) || !tolua_isusertype(tolua_S,3,"const Vector3i",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  const Vector3i* a_p1 = ((const Vector3i*)  tolua_tousertype(tolua_S,2,0));
  const Vector3i* a_p2 = ((const Vector3i*)  tolua_tousertype(tolua_S,3,0));
  {
   cCuboid* tolua_ret = (cCuboid*)  Mtolua_new((cCuboid)(*a_p1,*a_p2));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"cCuboid");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
  }
 }
 return 1;
tolua_lerror:
 return tolua_AllToLua_cCuboid_new01_local(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* get function: p1 of class  cCuboid */
#ifndef TOLUA_DISABLE_tolua_get_cCuboid_p1
static int tolua_get_cCuboid_p1(lua_State* tolua_S)
{
  cCuboid* self = (cCuboid*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'p1'",NULL);
#endif
   tolua_pushusertype(tolua_S,(void*)&self->p1,"Vector3i");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: p1 of class  cCuboid */
#ifndef TOLUA_DISABLE_tolua_set_cCuboid_p1
static int tolua_set_cCuboid_p1(lua_State* tolua_S)
{
  cCuboid* self = (cCuboid*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'p1'",NULL);
  if ((tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"Vector3i",0,&tolua_err)))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->p1 = *((Vector3i*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: p2 of class  cCuboid */
#ifndef TOLUA_DISABLE_tolua_get_cCuboid_p2
static int tolua_get_cCuboid_p2(lua_State* tolua_S)
{
  cCuboid* self = (cCuboid*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'p2'",NULL);
#endif
   tolua_pushusertype(tolua_S,(void*)&self->p2,"Vector3i");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: p2 of class  cCuboid */
#ifndef TOLUA_DISABLE_tolua_set_cCuboid_p2
static int tolua_set_cCuboid_p2(lua_State* tolua_S)
{
  cCuboid* self = (cCuboid*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'p2'",NULL);
  if ((tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"Vector3i",0,&tolua_err)))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->p2 = *((Vector3i*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: Sort of class  cCuboid */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cCuboid_Sort00
static int tolua_AllToLua_cCuboid_Sort00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cCuboid",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cCuboid* self = (cCuboid*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'Sort'", NULL);
#endif
  {
   self->Sort();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Sort'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: IsInside of class  cCuboid */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cCuboid_IsInside00
static int tolua_AllToLua_cCuboid_IsInside00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cCuboid",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const Vector3i",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cCuboid* self = (const cCuboid*)  tolua_tousertype(tolua_S,1,0);
  const Vector3i* v = ((const Vector3i*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'IsInside'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->IsInside(*v);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'IsInside'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: IsInside of class  cCuboid */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cCuboid_IsInside01
static int tolua_AllToLua_cCuboid_IsInside01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cCuboid",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const Vector3d",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  const cCuboid* self = (const cCuboid*)  tolua_tousertype(tolua_S,1,0);
  const Vector3d* v = ((const Vector3d*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'IsInside'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->IsInside(*v);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
tolua_lerror:
 return tolua_AllToLua_cCuboid_IsInside00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  cMCLogger */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cMCLogger_new00
static int tolua_AllToLua_cMCLogger_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"cMCLogger",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  char* a_File = ((char*)  tolua_tostring(tolua_S,2,0));
  {
   cMCLogger* tolua_ret = (cMCLogger*)  Mtolua_new((cMCLogger)(a_File));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"cMCLogger");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  cMCLogger */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cMCLogger_new00_local
static int tolua_AllToLua_cMCLogger_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"cMCLogger",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  char* a_File = ((char*)  tolua_tostring(tolua_S,2,0));
  {
   cMCLogger* tolua_ret = (cMCLogger*)  Mtolua_new((cMCLogger)(a_File));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"cMCLogger");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  cMCLogger */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cMCLogger_delete00
static int tolua_AllToLua_cMCLogger_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cMCLogger",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cMCLogger* self = (cMCLogger*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'", NULL);
#endif
  Mtolua_delete(self);
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: LogSimple of class  cMCLogger */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cMCLogger_LogSimple00
static int tolua_AllToLua_cMCLogger_LogSimple00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cMCLogger",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cMCLogger* self = (cMCLogger*)  tolua_tousertype(tolua_S,1,0);
  const char* a_Text = ((const char*)  tolua_tostring(tolua_S,2,0));
  int a_LogType = ((int)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'LogSimple'", NULL);
#endif
  {
   self->LogSimple(a_Text,a_LogType);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'LogSimple'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  cTracer */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cTracer_new00
static int tolua_AllToLua_cTracer_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"cTracer",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"cWorld",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cWorld* a_World = ((cWorld*)  tolua_tousertype(tolua_S,2,0));
  {
   cTracer* tolua_ret = (cTracer*)  Mtolua_new((cTracer)(a_World));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"cTracer");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  cTracer */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cTracer_new00_local
static int tolua_AllToLua_cTracer_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"cTracer",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"cWorld",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cWorld* a_World = ((cWorld*)  tolua_tousertype(tolua_S,2,0));
  {
   cTracer* tolua_ret = (cTracer*)  Mtolua_new((cTracer)(a_World));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"cTracer");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  cTracer */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cTracer_delete00
static int tolua_AllToLua_cTracer_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cTracer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cTracer* self = (cTracer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'", NULL);
#endif
  Mtolua_delete(self);
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: Trace of class  cTracer */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cTracer_Trace00
static int tolua_AllToLua_cTracer_Trace00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cTracer",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const Vector3f",0,&tolua_err)) ||
     (tolua_isvaluenil(tolua_S,3,&tolua_err) || !tolua_isusertype(tolua_S,3,"const Vector3f",0,&tolua_err)) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cTracer* self = (cTracer*)  tolua_tousertype(tolua_S,1,0);
  const Vector3f* a_Start = ((const Vector3f*)  tolua_tousertype(tolua_S,2,0));
  const Vector3f* a_Direction = ((const Vector3f*)  tolua_tousertype(tolua_S,3,0));
  int a_Distance = ((int)  tolua_tonumber(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'Trace'", NULL);
#endif
  {
   int tolua_ret = (int)  self->Trace(*a_Start,*a_Direction,a_Distance);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Trace'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetValues of class  cTracer */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cTracer_SetValues00
static int tolua_AllToLua_cTracer_SetValues00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cTracer",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const Vector3f",0,&tolua_err)) ||
     (tolua_isvaluenil(tolua_S,3,&tolua_err) || !tolua_isusertype(tolua_S,3,"const Vector3f",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cTracer* self = (cTracer*)  tolua_tousertype(tolua_S,1,0);
  const Vector3f* a_Start = ((const Vector3f*)  tolua_tousertype(tolua_S,2,0));
  const Vector3f* a_Direction = ((const Vector3f*)  tolua_tousertype(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetValues'", NULL);
#endif
  {
   self->SetValues(*a_Start,*a_Direction);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetValues'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* get function: BlockHitPosition of class  cTracer */
#ifndef TOLUA_DISABLE_tolua_get_cTracer_BlockHitPosition_ptr
static int tolua_get_cTracer_BlockHitPosition_ptr(lua_State* tolua_S)
{
  cTracer* self = (cTracer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'BlockHitPosition'",NULL);
#endif
   tolua_pushusertype(tolua_S,(void*)self->BlockHitPosition,"Vector3f");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: BlockHitPosition of class  cTracer */
#ifndef TOLUA_DISABLE_tolua_set_cTracer_BlockHitPosition_ptr
static int tolua_set_cTracer_BlockHitPosition_ptr(lua_State* tolua_S)
{
  cTracer* self = (cTracer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'BlockHitPosition'",NULL);
  if (!tolua_isusertype(tolua_S,2,"Vector3f",0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->BlockHitPosition = ((Vector3f*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: HitNormal of class  cTracer */
#ifndef TOLUA_DISABLE_tolua_get_cTracer_HitNormal_ptr
static int tolua_get_cTracer_HitNormal_ptr(lua_State* tolua_S)
{
  cTracer* self = (cTracer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'HitNormal'",NULL);
#endif
   tolua_pushusertype(tolua_S,(void*)self->HitNormal,"Vector3f");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: HitNormal of class  cTracer */
#ifndef TOLUA_DISABLE_tolua_set_cTracer_HitNormal_ptr
static int tolua_set_cTracer_HitNormal_ptr(lua_State* tolua_S)
{
  cTracer* self = (cTracer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'HitNormal'",NULL);
  if (!tolua_isusertype(tolua_S,2,"Vector3f",0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->HitNormal = ((Vector3f*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: RealHit of class  cTracer */
#ifndef TOLUA_DISABLE_tolua_get_cTracer_RealHit_ptr
static int tolua_get_cTracer_RealHit_ptr(lua_State* tolua_S)
{
  cTracer* self = (cTracer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'RealHit'",NULL);
#endif
   tolua_pushusertype(tolua_S,(void*)self->RealHit,"Vector3f");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: RealHit of class  cTracer */
#ifndef TOLUA_DISABLE_tolua_set_cTracer_RealHit_ptr
static int tolua_set_cTracer_RealHit_ptr(lua_State* tolua_S)
{
  cTracer* self = (cTracer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'RealHit'",NULL);
  if (!tolua_isusertype(tolua_S,2,"Vector3f",0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->RealHit = ((Vector3f*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetName of class  cGroup */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cGroup_SetName00
static int tolua_AllToLua_cGroup_SetName00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cGroup",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cGroup* self = (cGroup*)  tolua_tousertype(tolua_S,1,0);
  std::string a_Name = ((std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetName'", NULL);
#endif
  {
   self->SetName(a_Name);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetName'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetName of class  cGroup */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cGroup_GetName00
static int tolua_AllToLua_cGroup_GetName00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cGroup",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cGroup* self = (const cGroup*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetName'", NULL);
#endif
  {
   const std::string tolua_ret = (const std::string)  self->GetName();
   tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetName'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetColor of class  cGroup */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cGroup_SetColor00
static int tolua_AllToLua_cGroup_SetColor00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cGroup",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cGroup* self = (cGroup*)  tolua_tousertype(tolua_S,1,0);
  std::string a_Color = ((std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetColor'", NULL);
#endif
  {
   self->SetColor(a_Color);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetColor'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: AddCommand of class  cGroup */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cGroup_AddCommand00
static int tolua_AllToLua_cGroup_AddCommand00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cGroup",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cGroup* self = (cGroup*)  tolua_tousertype(tolua_S,1,0);
  std::string a_Command = ((std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'AddCommand'", NULL);
#endif
  {
   self->AddCommand(a_Command);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'AddCommand'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: AddPermission of class  cGroup */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cGroup_AddPermission00
static int tolua_AllToLua_cGroup_AddPermission00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cGroup",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cGroup* self = (cGroup*)  tolua_tousertype(tolua_S,1,0);
  std::string a_Permission = ((std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'AddPermission'", NULL);
#endif
  {
   self->AddPermission(a_Permission);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'AddPermission'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: InheritFrom of class  cGroup */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cGroup_InheritFrom00
static int tolua_AllToLua_cGroup_InheritFrom00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cGroup",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"cGroup",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cGroup* self = (cGroup*)  tolua_tousertype(tolua_S,1,0);
  cGroup* a_Group = ((cGroup*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'InheritFrom'", NULL);
#endif
  {
   self->InheritFrom(a_Group);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'InheritFrom'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: HasCommand of class  cGroup */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cGroup_HasCommand00
static int tolua_AllToLua_cGroup_HasCommand00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cGroup",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cGroup* self = (cGroup*)  tolua_tousertype(tolua_S,1,0);
  std::string a_Command = ((std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'HasCommand'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->HasCommand(a_Command);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'HasCommand'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetColor of class  cGroup */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cGroup_GetColor00
static int tolua_AllToLua_cGroup_GetColor00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cGroup",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cGroup* self = (const cGroup*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetColor'", NULL);
#endif
  {
   std::string tolua_ret = (std::string)  self->GetColor();
   tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetColor'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* get function: m_ProtocolVersion of class  cPacket_Login */
#ifndef TOLUA_DISABLE_tolua_get_cPacket_Login_m_ProtocolVersion
static int tolua_get_cPacket_Login_m_ProtocolVersion(lua_State* tolua_S)
{
  cPacket_Login* self = (cPacket_Login*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_ProtocolVersion'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->m_ProtocolVersion);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: m_ProtocolVersion of class  cPacket_Login */
#ifndef TOLUA_DISABLE_tolua_set_cPacket_Login_m_ProtocolVersion
static int tolua_set_cPacket_Login_m_ProtocolVersion(lua_State* tolua_S)
{
  cPacket_Login* self = (cPacket_Login*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_ProtocolVersion'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_ProtocolVersion = ((int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: m_Username of class  cPacket_Login */
#ifndef TOLUA_DISABLE_tolua_get_cPacket_Login_m_Username
static int tolua_get_cPacket_Login_m_Username(lua_State* tolua_S)
{
  cPacket_Login* self = (cPacket_Login*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_Username'",NULL);
#endif
  tolua_pushcppstring(tolua_S,(const char*)self->m_Username);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: m_Username of class  cPacket_Login */
#ifndef TOLUA_DISABLE_tolua_set_cPacket_Login_m_Username
static int tolua_set_cPacket_Login_m_Username(lua_State* tolua_S)
{
  cPacket_Login* self = (cPacket_Login*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_Username'",NULL);
  if (!tolua_iscppstring(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_Username = ((std::string)  tolua_tocppstring(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: m_MapSeed of class  cPacket_Login */
#ifndef TOLUA_DISABLE_tolua_get_cPacket_Login_m_MapSeed
static int tolua_get_cPacket_Login_m_MapSeed(lua_State* tolua_S)
{
  cPacket_Login* self = (cPacket_Login*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_MapSeed'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->m_MapSeed);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: m_MapSeed of class  cPacket_Login */
#ifndef TOLUA_DISABLE_tolua_set_cPacket_Login_m_MapSeed
static int tolua_set_cPacket_Login_m_MapSeed(lua_State* tolua_S)
{
  cPacket_Login* self = (cPacket_Login*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_MapSeed'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_MapSeed = ((long long)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: m_Dimension of class  cPacket_Login */
#ifndef TOLUA_DISABLE_tolua_get_cPacket_Login_m_Dimension
static int tolua_get_cPacket_Login_m_Dimension(lua_State* tolua_S)
{
  cPacket_Login* self = (cPacket_Login*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_Dimension'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->m_Dimension);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: m_Dimension of class  cPacket_Login */
#ifndef TOLUA_DISABLE_tolua_set_cPacket_Login_m_Dimension
static int tolua_set_cPacket_Login_m_Dimension(lua_State* tolua_S)
{
  cPacket_Login* self = (cPacket_Login*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_Dimension'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_Dimension = ((char)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  cPacket_BlockDig */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cPacket_BlockDig_new00
static int tolua_AllToLua_cPacket_BlockDig_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"cPacket_BlockDig",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   cPacket_BlockDig* tolua_ret = (cPacket_BlockDig*)  Mtolua_new((cPacket_BlockDig)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"cPacket_BlockDig");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  cPacket_BlockDig */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cPacket_BlockDig_new00_local
static int tolua_AllToLua_cPacket_BlockDig_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"cPacket_BlockDig",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   cPacket_BlockDig* tolua_ret = (cPacket_BlockDig*)  Mtolua_new((cPacket_BlockDig)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"cPacket_BlockDig");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: Clone of class  cPacket_BlockDig */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cPacket_BlockDig_Clone00
static int tolua_AllToLua_cPacket_BlockDig_Clone00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cPacket_BlockDig",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cPacket_BlockDig* self = (const cPacket_BlockDig*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'Clone'", NULL);
#endif
  {
   cPacket* tolua_ret = (cPacket*)  self->Clone();
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"cPacket");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Clone'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* get function: m_Status of class  cPacket_BlockDig */
#ifndef TOLUA_DISABLE_tolua_get_cPacket_BlockDig_m_Status
static int tolua_get_cPacket_BlockDig_m_Status(lua_State* tolua_S)
{
  cPacket_BlockDig* self = (cPacket_BlockDig*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_Status'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->m_Status);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: m_Status of class  cPacket_BlockDig */
#ifndef TOLUA_DISABLE_tolua_set_cPacket_BlockDig_m_Status
static int tolua_set_cPacket_BlockDig_m_Status(lua_State* tolua_S)
{
  cPacket_BlockDig* self = (cPacket_BlockDig*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_Status'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_Status = ((char)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: m_PosX of class  cPacket_BlockDig */
#ifndef TOLUA_DISABLE_tolua_get_cPacket_BlockDig_m_PosX
static int tolua_get_cPacket_BlockDig_m_PosX(lua_State* tolua_S)
{
  cPacket_BlockDig* self = (cPacket_BlockDig*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_PosX'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->m_PosX);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: m_PosX of class  cPacket_BlockDig */
#ifndef TOLUA_DISABLE_tolua_set_cPacket_BlockDig_m_PosX
static int tolua_set_cPacket_BlockDig_m_PosX(lua_State* tolua_S)
{
  cPacket_BlockDig* self = (cPacket_BlockDig*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_PosX'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_PosX = ((int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: m_PosY of class  cPacket_BlockDig */
#ifndef TOLUA_DISABLE_tolua_get_cPacket_BlockDig_m_PosY
static int tolua_get_cPacket_BlockDig_m_PosY(lua_State* tolua_S)
{
  cPacket_BlockDig* self = (cPacket_BlockDig*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_PosY'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->m_PosY);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: m_PosY of class  cPacket_BlockDig */
#ifndef TOLUA_DISABLE_tolua_set_cPacket_BlockDig_m_PosY
static int tolua_set_cPacket_BlockDig_m_PosY(lua_State* tolua_S)
{
  cPacket_BlockDig* self = (cPacket_BlockDig*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_PosY'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_PosY = ((char)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: m_PosZ of class  cPacket_BlockDig */
#ifndef TOLUA_DISABLE_tolua_get_cPacket_BlockDig_m_PosZ
static int tolua_get_cPacket_BlockDig_m_PosZ(lua_State* tolua_S)
{
  cPacket_BlockDig* self = (cPacket_BlockDig*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_PosZ'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->m_PosZ);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: m_PosZ of class  cPacket_BlockDig */
#ifndef TOLUA_DISABLE_tolua_set_cPacket_BlockDig_m_PosZ
static int tolua_set_cPacket_BlockDig_m_PosZ(lua_State* tolua_S)
{
  cPacket_BlockDig* self = (cPacket_BlockDig*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_PosZ'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_PosZ = ((int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: m_Direction of class  cPacket_BlockDig */
#ifndef TOLUA_DISABLE_tolua_get_cPacket_BlockDig_m_Direction
static int tolua_get_cPacket_BlockDig_m_Direction(lua_State* tolua_S)
{
  cPacket_BlockDig* self = (cPacket_BlockDig*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_Direction'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->m_Direction);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: m_Direction of class  cPacket_BlockDig */
#ifndef TOLUA_DISABLE_tolua_set_cPacket_BlockDig_m_Direction
static int tolua_set_cPacket_BlockDig_m_Direction(lua_State* tolua_S)
{
  cPacket_BlockDig* self = (cPacket_BlockDig*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_Direction'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_Direction = ((char)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

 class Lua__cPacket_BlockDig : public cPacket_BlockDig, public ToluaBase {
public:
	 cPacket*  Clone( void )const {
		if (push_method("Clone",  tolua_AllToLua_cPacket_BlockDig_Clone00)) {
			ToluaBase::dbcall(lua_state, 1, 1);
			 cPacket*  tolua_ret = ( cPacket* )tolua_tousertype(lua_state, -1, 0);
			lua_pop(lua_state, 1);
			return tolua_ret;
		} else {
			return ( cPacket* ) cPacket_BlockDig:: Clone();
		};
	};

	 cPacket* cPacket_BlockDig__Clone( void ) {
		return ( cPacket* )cPacket_BlockDig::Clone();
	};
	 Lua__cPacket_BlockDig( void ): cPacket_BlockDig(){};
};

/* method: tolua__set_instance of class  Lua__cPacket_BlockDig */
#ifndef TOLUA_DISABLE_tolua_AllToLua_Lua__cPacket_BlockDig_tolua__set_instance00
static int tolua_AllToLua_Lua__cPacket_BlockDig_tolua__set_instance00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Lua__cPacket_BlockDig",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Lua__cPacket_BlockDig* self = (Lua__cPacket_BlockDig*)  tolua_tousertype(tolua_S,1,0);
  lua_State* L =  tolua_S;
  lua_Object lo = ((lua_Object)  tolua_tovalue(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'tolua__set_instance'", NULL);
#endif
  {
   self->tolua__set_instance(L,lo);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'tolua__set_instance'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: cPacket_BlockDig__Clone of class  Lua__cPacket_BlockDig */
#ifndef TOLUA_DISABLE_tolua_AllToLua_Lua__cPacket_BlockDig_cPacket_BlockDig__Clone00
static int tolua_AllToLua_Lua__cPacket_BlockDig_cPacket_BlockDig__Clone00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Lua__cPacket_BlockDig",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Lua__cPacket_BlockDig* self = (Lua__cPacket_BlockDig*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'cPacket_BlockDig__Clone'", NULL);
#endif
  {
   cPacket* tolua_ret = (cPacket*)  self->cPacket_BlockDig__Clone();
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"cPacket");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'cPacket_BlockDig__Clone'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  Lua__cPacket_BlockDig */
#ifndef TOLUA_DISABLE_tolua_AllToLua_Lua__cPacket_BlockDig_new00
static int tolua_AllToLua_Lua__cPacket_BlockDig_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Lua__cPacket_BlockDig",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   Lua__cPacket_BlockDig* tolua_ret = (Lua__cPacket_BlockDig*)  Mtolua_new((Lua__cPacket_BlockDig)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"Lua__cPacket_BlockDig");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  Lua__cPacket_BlockDig */
#ifndef TOLUA_DISABLE_tolua_AllToLua_Lua__cPacket_BlockDig_new00_local
static int tolua_AllToLua_Lua__cPacket_BlockDig_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Lua__cPacket_BlockDig",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   Lua__cPacket_BlockDig* tolua_ret = (Lua__cPacket_BlockDig*)  Mtolua_new((Lua__cPacket_BlockDig)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"Lua__cPacket_BlockDig");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE


/* function to release collected object via destructor */
#ifdef __cplusplus

static int tolua_collect_Lua__cPacket_BlockDig (lua_State* tolua_S)
{
 Lua__cPacket_BlockDig* self = (Lua__cPacket_BlockDig*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}
#endif

/* get function: m_PosX of class  cPacket_BlockPlace */
#ifndef TOLUA_DISABLE_tolua_get_cPacket_BlockPlace_m_PosX
static int tolua_get_cPacket_BlockPlace_m_PosX(lua_State* tolua_S)
{
  cPacket_BlockPlace* self = (cPacket_BlockPlace*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_PosX'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->m_PosX);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: m_PosX of class  cPacket_BlockPlace */
#ifndef TOLUA_DISABLE_tolua_set_cPacket_BlockPlace_m_PosX
static int tolua_set_cPacket_BlockPlace_m_PosX(lua_State* tolua_S)
{
  cPacket_BlockPlace* self = (cPacket_BlockPlace*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_PosX'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_PosX = ((int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: m_PosY of class  cPacket_BlockPlace */
#ifndef TOLUA_DISABLE_tolua_get_cPacket_BlockPlace_m_PosY
static int tolua_get_cPacket_BlockPlace_m_PosY(lua_State* tolua_S)
{
  cPacket_BlockPlace* self = (cPacket_BlockPlace*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_PosY'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->m_PosY);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: m_PosY of class  cPacket_BlockPlace */
#ifndef TOLUA_DISABLE_tolua_set_cPacket_BlockPlace_m_PosY
static int tolua_set_cPacket_BlockPlace_m_PosY(lua_State* tolua_S)
{
  cPacket_BlockPlace* self = (cPacket_BlockPlace*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_PosY'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_PosY = ((char)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: m_PosZ of class  cPacket_BlockPlace */
#ifndef TOLUA_DISABLE_tolua_get_cPacket_BlockPlace_m_PosZ
static int tolua_get_cPacket_BlockPlace_m_PosZ(lua_State* tolua_S)
{
  cPacket_BlockPlace* self = (cPacket_BlockPlace*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_PosZ'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->m_PosZ);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: m_PosZ of class  cPacket_BlockPlace */
#ifndef TOLUA_DISABLE_tolua_set_cPacket_BlockPlace_m_PosZ
static int tolua_set_cPacket_BlockPlace_m_PosZ(lua_State* tolua_S)
{
  cPacket_BlockPlace* self = (cPacket_BlockPlace*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_PosZ'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_PosZ = ((int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: m_Direction of class  cPacket_BlockPlace */
#ifndef TOLUA_DISABLE_tolua_get_cPacket_BlockPlace_m_Direction
static int tolua_get_cPacket_BlockPlace_m_Direction(lua_State* tolua_S)
{
  cPacket_BlockPlace* self = (cPacket_BlockPlace*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_Direction'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->m_Direction);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: m_Direction of class  cPacket_BlockPlace */
#ifndef TOLUA_DISABLE_tolua_set_cPacket_BlockPlace_m_Direction
static int tolua_set_cPacket_BlockPlace_m_Direction(lua_State* tolua_S)
{
  cPacket_BlockPlace* self = (cPacket_BlockPlace*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_Direction'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_Direction = ((char)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: m_ItemType of class  cPacket_BlockPlace */
#ifndef TOLUA_DISABLE_tolua_get_cPacket_BlockPlace_m_ItemType
static int tolua_get_cPacket_BlockPlace_m_ItemType(lua_State* tolua_S)
{
  cPacket_BlockPlace* self = (cPacket_BlockPlace*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_ItemType'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->m_ItemType);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: m_ItemType of class  cPacket_BlockPlace */
#ifndef TOLUA_DISABLE_tolua_set_cPacket_BlockPlace_m_ItemType
static int tolua_set_cPacket_BlockPlace_m_ItemType(lua_State* tolua_S)
{
  cPacket_BlockPlace* self = (cPacket_BlockPlace*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_ItemType'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_ItemType = ((short)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: m_Count of class  cPacket_BlockPlace */
#ifndef TOLUA_DISABLE_tolua_get_cPacket_BlockPlace_m_Count
static int tolua_get_cPacket_BlockPlace_m_Count(lua_State* tolua_S)
{
  cPacket_BlockPlace* self = (cPacket_BlockPlace*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_Count'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->m_Count);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: m_Count of class  cPacket_BlockPlace */
#ifndef TOLUA_DISABLE_tolua_set_cPacket_BlockPlace_m_Count
static int tolua_set_cPacket_BlockPlace_m_Count(lua_State* tolua_S)
{
  cPacket_BlockPlace* self = (cPacket_BlockPlace*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_Count'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_Count = ((char)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: m_Uses of class  cPacket_BlockPlace */
#ifndef TOLUA_DISABLE_tolua_get_cPacket_BlockPlace_m_Uses
static int tolua_get_cPacket_BlockPlace_m_Uses(lua_State* tolua_S)
{
  cPacket_BlockPlace* self = (cPacket_BlockPlace*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_Uses'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->m_Uses);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: m_Uses of class  cPacket_BlockPlace */
#ifndef TOLUA_DISABLE_tolua_set_cPacket_BlockPlace_m_Uses
static int tolua_set_cPacket_BlockPlace_m_Uses(lua_State* tolua_S)
{
  cPacket_BlockPlace* self = (cPacket_BlockPlace*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_Uses'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_Uses = ((short)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* Open function */
TOLUA_API int tolua_AllToLua_open (lua_State* tolua_S)
{
 tolua_open(tolua_S);
 tolua_reg_types(tolua_S);
 tolua_module(tolua_S,NULL,1);
 tolua_beginmodule(tolua_S,NULL);
  tolua_cclass(tolua_S,"cTorch","cTorch","",NULL);
  tolua_beginmodule(tolua_S,"cTorch");
   tolua_function(tolua_S,"DirectionToMetaData",tolua_AllToLua_cTorch_DirectionToMetaData00);
   tolua_function(tolua_S,"MetaDataToDirection",tolua_AllToLua_cTorch_MetaDataToDirection00);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"cStairs","cStairs","",NULL);
  tolua_beginmodule(tolua_S,"cStairs");
   tolua_function(tolua_S,"RotationToMetaData",tolua_AllToLua_cStairs_RotationToMetaData00);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"cLadder","cLadder","",NULL);
  tolua_beginmodule(tolua_S,"cLadder");
   tolua_function(tolua_S,"DirectionToMetaData",tolua_AllToLua_cLadder_DirectionToMetaData00);
   tolua_function(tolua_S,"MetaDataToDirection",tolua_AllToLua_cLadder_MetaDataToDirection00);
  tolua_endmodule(tolua_S);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"cIniFile","cIniFile","",tolua_collect_cIniFile);
  #else
  tolua_cclass(tolua_S,"cIniFile","cIniFile","",NULL);
  #endif
  tolua_beginmodule(tolua_S,"cIniFile");
   tolua_function(tolua_S,"new",tolua_AllToLua_cIniFile_new00);
   tolua_function(tolua_S,"new_local",tolua_AllToLua_cIniFile_new00_local);
   tolua_function(tolua_S,".call",tolua_AllToLua_cIniFile_new00_local);
   tolua_function(tolua_S,"CaseSensitive",tolua_AllToLua_cIniFile_CaseSensitive00);
   tolua_function(tolua_S,"CaseInsensitive",tolua_AllToLua_cIniFile_CaseInsensitive00);
   tolua_function(tolua_S,"Path",tolua_AllToLua_cIniFile_Path00);
   tolua_function(tolua_S,"Path",tolua_AllToLua_cIniFile_Path01);
   tolua_function(tolua_S,"SetPath",tolua_AllToLua_cIniFile_SetPath00);
   tolua_function(tolua_S,"ReadFile",tolua_AllToLua_cIniFile_ReadFile00);
   tolua_function(tolua_S,"WriteFile",tolua_AllToLua_cIniFile_WriteFile00);
   tolua_function(tolua_S,"Erase",tolua_AllToLua_cIniFile_Erase00);
   tolua_function(tolua_S,"Clear",tolua_AllToLua_cIniFile_Clear00);
   tolua_function(tolua_S,"Reset",tolua_AllToLua_cIniFile_Reset00);
   tolua_function(tolua_S,"FindKey",tolua_AllToLua_cIniFile_FindKey00);
   tolua_function(tolua_S,"FindValue",tolua_AllToLua_cIniFile_FindValue00);
   tolua_function(tolua_S,"NumKeys",tolua_AllToLua_cIniFile_NumKeys00);
   tolua_function(tolua_S,"GetNumKeys",tolua_AllToLua_cIniFile_GetNumKeys00);
   tolua_function(tolua_S,"AddKeyName",tolua_AllToLua_cIniFile_AddKeyName00);
   tolua_function(tolua_S,"KeyName",tolua_AllToLua_cIniFile_KeyName00);
   tolua_function(tolua_S,"GetKeyName",tolua_AllToLua_cIniFile_GetKeyName00);
   tolua_function(tolua_S,"NumValues",tolua_AllToLua_cIniFile_NumValues00);
   tolua_function(tolua_S,"GetNumValues",tolua_AllToLua_cIniFile_GetNumValues00);
   tolua_function(tolua_S,"NumValues",tolua_AllToLua_cIniFile_NumValues01);
   tolua_function(tolua_S,"GetNumValues",tolua_AllToLua_cIniFile_GetNumValues01);
   tolua_function(tolua_S,"ValueName",tolua_AllToLua_cIniFile_ValueName00);
   tolua_function(tolua_S,"GetValueName",tolua_AllToLua_cIniFile_GetValueName00);
   tolua_function(tolua_S,"ValueName",tolua_AllToLua_cIniFile_ValueName01);
   tolua_function(tolua_S,"GetValueName",tolua_AllToLua_cIniFile_GetValueName01);
   tolua_function(tolua_S,"GetValue",tolua_AllToLua_cIniFile_GetValue00);
   tolua_function(tolua_S,"GetValue",tolua_AllToLua_cIniFile_GetValue01);
   tolua_function(tolua_S,"GetValue",tolua_AllToLua_cIniFile_GetValue02);
   tolua_function(tolua_S,"GetValue",tolua_AllToLua_cIniFile_GetValue03);
   tolua_function(tolua_S,"GetValueI",tolua_AllToLua_cIniFile_GetValueI00);
   tolua_function(tolua_S,"GetValueB",tolua_AllToLua_cIniFile_GetValueB00);
   tolua_function(tolua_S,"GetValueF",tolua_AllToLua_cIniFile_GetValueF00);
   tolua_function(tolua_S,"SetValue",tolua_AllToLua_cIniFile_SetValue00);
   tolua_function(tolua_S,"SetValue",tolua_AllToLua_cIniFile_SetValue01);
   tolua_function(tolua_S,"SetValueI",tolua_AllToLua_cIniFile_SetValueI00);
   tolua_function(tolua_S,"SetValueB",tolua_AllToLua_cIniFile_SetValueB00);
   tolua_function(tolua_S,"SetValueF",tolua_AllToLua_cIniFile_SetValueF00);
   tolua_function(tolua_S,"DeleteValue",tolua_AllToLua_cIniFile_DeleteValue00);
   tolua_function(tolua_S,"DeleteKey",tolua_AllToLua_cIniFile_DeleteKey00);
   tolua_function(tolua_S,"NumHeaderComments",tolua_AllToLua_cIniFile_NumHeaderComments00);
   tolua_function(tolua_S,"HeaderComment",tolua_AllToLua_cIniFile_HeaderComment00);
   tolua_function(tolua_S,"HeaderComment",tolua_AllToLua_cIniFile_HeaderComment01);
   tolua_function(tolua_S,"DeleteHeaderComment",tolua_AllToLua_cIniFile_DeleteHeaderComment00);
   tolua_function(tolua_S,"DeleteHeaderComments",tolua_AllToLua_cIniFile_DeleteHeaderComments00);
   tolua_function(tolua_S,"NumKeyComments",tolua_AllToLua_cIniFile_NumKeyComments00);
   tolua_function(tolua_S,"NumKeyComments",tolua_AllToLua_cIniFile_NumKeyComments01);
   tolua_function(tolua_S,"KeyComment",tolua_AllToLua_cIniFile_KeyComment00);
   tolua_function(tolua_S,"KeyComment",tolua_AllToLua_cIniFile_KeyComment01);
   tolua_function(tolua_S,"KeyComment",tolua_AllToLua_cIniFile_KeyComment02);
   tolua_function(tolua_S,"KeyComment",tolua_AllToLua_cIniFile_KeyComment03);
   tolua_function(tolua_S,"DeleteKeyComment",tolua_AllToLua_cIniFile_DeleteKeyComment00);
   tolua_function(tolua_S,"DeleteKeyComment",tolua_AllToLua_cIniFile_DeleteKeyComment01);
   tolua_function(tolua_S,"DeleteKeyComments",tolua_AllToLua_cIniFile_DeleteKeyComments00);
   tolua_function(tolua_S,"DeleteKeyComments",tolua_AllToLua_cIniFile_DeleteKeyComments01);
  tolua_endmodule(tolua_S);
  tolua_constant(tolua_S,"E_BLOCK_AIR",E_BLOCK_AIR);
  tolua_constant(tolua_S,"E_BLOCK_STONE",E_BLOCK_STONE);
  tolua_constant(tolua_S,"E_BLOCK_GRASS",E_BLOCK_GRASS);
  tolua_constant(tolua_S,"E_BLOCK_DIRT",E_BLOCK_DIRT);
  tolua_constant(tolua_S,"E_BLOCK_COBBLESTONE",E_BLOCK_COBBLESTONE);
  tolua_constant(tolua_S,"E_BLOCK_WOOD",E_BLOCK_WOOD);
  tolua_constant(tolua_S,"E_BLOCK_SAPLING",E_BLOCK_SAPLING);
  tolua_constant(tolua_S,"E_BLOCK_BEDROCK",E_BLOCK_BEDROCK);
  tolua_constant(tolua_S,"E_BLOCK_WATER",E_BLOCK_WATER);
  tolua_constant(tolua_S,"E_BLOCK_STATIONARY_WATER",E_BLOCK_STATIONARY_WATER);
  tolua_constant(tolua_S,"E_BLOCK_LAVA",E_BLOCK_LAVA);
  tolua_constant(tolua_S,"E_BLOCK_STATIONARY_LAVA",E_BLOCK_STATIONARY_LAVA);
  tolua_constant(tolua_S,"E_BLOCK_SAND",E_BLOCK_SAND);
  tolua_constant(tolua_S,"E_BLOCK_GRAVEL",E_BLOCK_GRAVEL);
  tolua_constant(tolua_S,"E_BLOCK_GOLD_ORE",E_BLOCK_GOLD_ORE);
  tolua_constant(tolua_S,"E_BLOCK_IRON_ORE",E_BLOCK_IRON_ORE);
  tolua_constant(tolua_S,"E_BLOCK_COAL_ORE",E_BLOCK_COAL_ORE);
  tolua_constant(tolua_S,"E_BLOCK_LOG",E_BLOCK_LOG);
  tolua_constant(tolua_S,"E_BLOCK_LEAVES",E_BLOCK_LEAVES);
  tolua_constant(tolua_S,"E_BLOCK_SPONGE",E_BLOCK_SPONGE);
  tolua_constant(tolua_S,"E_BLOCK_GLASS",E_BLOCK_GLASS);
  tolua_constant(tolua_S,"E_BLOCK_WHITE_CLOTH",E_BLOCK_WHITE_CLOTH);
  tolua_constant(tolua_S,"E_BLOCK_YELLOW_FLOWER",E_BLOCK_YELLOW_FLOWER);
  tolua_constant(tolua_S,"E_BLOCK_RED_ROSE",E_BLOCK_RED_ROSE);
  tolua_constant(tolua_S,"E_BLOCK_BROWN_MUSHROOM",E_BLOCK_BROWN_MUSHROOM);
  tolua_constant(tolua_S,"E_BLOCK_RED_MUSHROOM",E_BLOCK_RED_MUSHROOM);
  tolua_constant(tolua_S,"E_BLOCK_GOLD_BLOCK",E_BLOCK_GOLD_BLOCK);
  tolua_constant(tolua_S,"E_BLOCK_IRON_BLOCK",E_BLOCK_IRON_BLOCK);
  tolua_constant(tolua_S,"E_BLOCK_DOUBLE_STEP",E_BLOCK_DOUBLE_STEP);
  tolua_constant(tolua_S,"E_BLOCK_STEP",E_BLOCK_STEP);
  tolua_constant(tolua_S,"E_BLOCK_BRICK",E_BLOCK_BRICK);
  tolua_constant(tolua_S,"E_BLOCK_TNT",E_BLOCK_TNT);
  tolua_constant(tolua_S,"E_BLOCK_BOOKCASE",E_BLOCK_BOOKCASE);
  tolua_constant(tolua_S,"E_BLOCK_MOSSY_COBBLESTONE",E_BLOCK_MOSSY_COBBLESTONE);
  tolua_constant(tolua_S,"E_BLOCK_OBSIDIAN",E_BLOCK_OBSIDIAN);
  tolua_constant(tolua_S,"E_BLOCK_TORCH",E_BLOCK_TORCH);
  tolua_constant(tolua_S,"E_BLOCK_FIRE",E_BLOCK_FIRE);
  tolua_constant(tolua_S,"E_BLOCK_MOB_SPAWNER",E_BLOCK_MOB_SPAWNER);
  tolua_constant(tolua_S,"E_BLOCK_WOODEN_STAIRS",E_BLOCK_WOODEN_STAIRS);
  tolua_constant(tolua_S,"E_BLOCK_CHEST",E_BLOCK_CHEST);
  tolua_constant(tolua_S,"E_BLOCK_REDSTONE_WIRE",E_BLOCK_REDSTONE_WIRE);
  tolua_constant(tolua_S,"E_BLOCK_DIAMOND_ORE",E_BLOCK_DIAMOND_ORE);
  tolua_constant(tolua_S,"E_BLOCK_DIAMOND_BLOCK",E_BLOCK_DIAMOND_BLOCK);
  tolua_constant(tolua_S,"E_BLOCK_WORKBENCH",E_BLOCK_WORKBENCH);
  tolua_constant(tolua_S,"E_BLOCK_CROPS",E_BLOCK_CROPS);
  tolua_constant(tolua_S,"E_BLOCK_SOIL",E_BLOCK_SOIL);
  tolua_constant(tolua_S,"E_BLOCK_FURNACE",E_BLOCK_FURNACE);
  tolua_constant(tolua_S,"E_BLOCK_BURNING_FURNACE",E_BLOCK_BURNING_FURNACE);
  tolua_constant(tolua_S,"E_BLOCK_SIGN_POST",E_BLOCK_SIGN_POST);
  tolua_constant(tolua_S,"E_BLOCK_WOODEN_DOOR",E_BLOCK_WOODEN_DOOR);
  tolua_constant(tolua_S,"E_BLOCK_LADDER",E_BLOCK_LADDER);
  tolua_constant(tolua_S,"E_BLOCK_MINECART_TRACKS",E_BLOCK_MINECART_TRACKS);
  tolua_constant(tolua_S,"E_BLOCK_COBBLESTONE_STAIRS",E_BLOCK_COBBLESTONE_STAIRS);
  tolua_constant(tolua_S,"E_BLOCK_WALLSIGN",E_BLOCK_WALLSIGN);
  tolua_constant(tolua_S,"E_BLOCK_LEVER",E_BLOCK_LEVER);
  tolua_constant(tolua_S,"E_BLOCK_STONE_PRESSURE_PLATE",E_BLOCK_STONE_PRESSURE_PLATE);
  tolua_constant(tolua_S,"E_BLOCK_IRON_DOOR",E_BLOCK_IRON_DOOR);
  tolua_constant(tolua_S,"E_BLOCK_WOODEN_PRESSURE_PLATE",E_BLOCK_WOODEN_PRESSURE_PLATE);
  tolua_constant(tolua_S,"E_BLOCK_REDSTONE_ORE",E_BLOCK_REDSTONE_ORE);
  tolua_constant(tolua_S,"E_BLOCK_REDSTONE_ORE_GLOWING",E_BLOCK_REDSTONE_ORE_GLOWING);
  tolua_constant(tolua_S,"E_BLOCK_REDSTONE_TORCH_ON",E_BLOCK_REDSTONE_TORCH_ON);
  tolua_constant(tolua_S,"E_BLOCK_REDSTONE_TORCH_OFF",E_BLOCK_REDSTONE_TORCH_OFF);
  tolua_constant(tolua_S,"E_BLOCK_SNOW",E_BLOCK_SNOW);
  tolua_constant(tolua_S,"E_BLOCK_ICE",E_BLOCK_ICE);
  tolua_constant(tolua_S,"E_BLOCK_SNOW_BLOCK",E_BLOCK_SNOW_BLOCK);
  tolua_constant(tolua_S,"E_BLOCK_CACTUS",E_BLOCK_CACTUS);
  tolua_constant(tolua_S,"E_BLOCK_CLAY",E_BLOCK_CLAY);
  tolua_constant(tolua_S,"E_BLOCK_REEDS",E_BLOCK_REEDS);
  tolua_constant(tolua_S,"E_BLOCK_BLOODSTONE",E_BLOCK_BLOODSTONE);
  tolua_constant(tolua_S,"E_BLOCK_SOULSAND",E_BLOCK_SOULSAND);
  tolua_constant(tolua_S,"E_BLOCK_GLOWSTONE",E_BLOCK_GLOWSTONE);
  tolua_constant(tolua_S,"E_BLOCK_PORT",E_BLOCK_PORT);
  tolua_constant(tolua_S,"E_BLOCK_JACK_O_LANTERN",E_BLOCK_JACK_O_LANTERN);
  tolua_constant(tolua_S,"E_BLOCK_CAKE",E_BLOCK_CAKE);
  tolua_constant(tolua_S,"E_BLOCK_REDSTONE_REPEATER_OFF",E_BLOCK_REDSTONE_REPEATER_OFF);
  tolua_constant(tolua_S,"E_BLOCK_REDSTONE_REPEATER_ON",E_BLOCK_REDSTONE_REPEATER_ON);
  tolua_constant(tolua_S,"E_BLOCK_LOCKED_CHEST",E_BLOCK_LOCKED_CHEST);
  tolua_constant(tolua_S,"E_BLOCK_TRAPDOOR",E_BLOCK_TRAPDOOR);
  tolua_constant(tolua_S,"E_ITEM_EMPTY",E_ITEM_EMPTY);
  tolua_constant(tolua_S,"E_ITEM_STONE",E_ITEM_STONE);
  tolua_constant(tolua_S,"E_ITEM_GRASS",E_ITEM_GRASS);
  tolua_constant(tolua_S,"E_ITEM_DIRT",E_ITEM_DIRT);
  tolua_constant(tolua_S,"E_ITEM_COBBLESTONE",E_ITEM_COBBLESTONE);
  tolua_constant(tolua_S,"E_ITEM_WOOD",E_ITEM_WOOD);
  tolua_constant(tolua_S,"E_ITEM_SAPLING",E_ITEM_SAPLING);
  tolua_constant(tolua_S,"E_ITEM_BEDROCK",E_ITEM_BEDROCK);
  tolua_constant(tolua_S,"E_ITEM_WATER",E_ITEM_WATER);
  tolua_constant(tolua_S,"E_ITEM_STATIONARY_WATER",E_ITEM_STATIONARY_WATER);
  tolua_constant(tolua_S,"E_ITEM_LAVA",E_ITEM_LAVA);
  tolua_constant(tolua_S,"E_ITEM_STATIONARY_LAVA",E_ITEM_STATIONARY_LAVA);
  tolua_constant(tolua_S,"E_ITEM_SAND",E_ITEM_SAND);
  tolua_constant(tolua_S,"E_ITEM_GRAVEL",E_ITEM_GRAVEL);
  tolua_constant(tolua_S,"E_ITEM_GOLD_ORE",E_ITEM_GOLD_ORE);
  tolua_constant(tolua_S,"E_ITEM_IRON_ORE",E_ITEM_IRON_ORE);
  tolua_constant(tolua_S,"E_ITEM_COAL_ORE",E_ITEM_COAL_ORE);
  tolua_constant(tolua_S,"E_ITEM_LOG",E_ITEM_LOG);
  tolua_constant(tolua_S,"E_ITEM_LEAVES",E_ITEM_LEAVES);
  tolua_constant(tolua_S,"E_ITEM_GOLD_BLOCK",E_ITEM_GOLD_BLOCK);
  tolua_constant(tolua_S,"E_ITEM_IRON_BLOCK",E_ITEM_IRON_BLOCK);
  tolua_constant(tolua_S,"E_ITEM_DIAMOND_BLOCK",E_ITEM_DIAMOND_BLOCK);
  tolua_constant(tolua_S,"E_ITEM_FLINT_AND_STEEL",E_ITEM_FLINT_AND_STEEL);
  tolua_constant(tolua_S,"E_ITEM_APPLE",E_ITEM_APPLE);
  tolua_constant(tolua_S,"E_ITEM_BOW",E_ITEM_BOW);
  tolua_constant(tolua_S,"E_ITEM_ARROW",E_ITEM_ARROW);
  tolua_constant(tolua_S,"E_ITEM_COAL",E_ITEM_COAL);
  tolua_constant(tolua_S,"E_ITEM_DIAMOND",E_ITEM_DIAMOND);
  tolua_constant(tolua_S,"E_ITEM_IRON",E_ITEM_IRON);
  tolua_constant(tolua_S,"E_ITEM_GOLD",E_ITEM_GOLD);
  tolua_constant(tolua_S,"E_ITEM_MUSHROOM_SOUP",E_ITEM_MUSHROOM_SOUP);
  tolua_constant(tolua_S,"E_ITEM_GOLD_SWORD",E_ITEM_GOLD_SWORD);
  tolua_constant(tolua_S,"E_ITEM_GOLD_SHOVEL",E_ITEM_GOLD_SHOVEL);
  tolua_constant(tolua_S,"E_ITEM_GOLD_PICKAXE",E_ITEM_GOLD_PICKAXE);
  tolua_constant(tolua_S,"E_ITEM_GOLD_AXE",E_ITEM_GOLD_AXE);
  tolua_constant(tolua_S,"E_ITEM_STRING",E_ITEM_STRING);
  tolua_constant(tolua_S,"E_ITEM_FEATHER",E_ITEM_FEATHER);
  tolua_constant(tolua_S,"E_ITEM_BREAD",E_ITEM_BREAD);
  tolua_constant(tolua_S,"E_ITEM_RAW_MEAT",E_ITEM_RAW_MEAT);
  tolua_constant(tolua_S,"E_ITEM_COOKED_MEAT",E_ITEM_COOKED_MEAT);
  tolua_constant(tolua_S,"E_ITEM_GOLDEN_APPLE",E_ITEM_GOLDEN_APPLE);
  tolua_constant(tolua_S,"E_ITEM_SIGN",E_ITEM_SIGN);
  tolua_constant(tolua_S,"E_ITEM_MILK",E_ITEM_MILK);
  tolua_constant(tolua_S,"E_ITEM_EGG",E_ITEM_EGG);
  tolua_constant(tolua_S,"E_ITEM_COMPASS",E_ITEM_COMPASS);
  tolua_constant(tolua_S,"E_ITEM_FISHING_ROD",E_ITEM_FISHING_ROD);
  tolua_constant(tolua_S,"E_ITEM_CLOCK",E_ITEM_CLOCK);
  tolua_constant(tolua_S,"E_ITEM_GLOWSTONE_DUST",E_ITEM_GLOWSTONE_DUST);
  tolua_constant(tolua_S,"E_ITEM_RAW_FISH",E_ITEM_RAW_FISH);
  tolua_constant(tolua_S,"E_ITEM_COOKED_FISH",E_ITEM_COOKED_FISH);
  tolua_constant(tolua_S,"E_ITEM_CAKE",E_ITEM_CAKE);
  tolua_constant(tolua_S,"E_KEEP_ALIVE",E_KEEP_ALIVE);
  tolua_constant(tolua_S,"E_LOGIN",E_LOGIN);
  tolua_constant(tolua_S,"E_HANDSHAKE",E_HANDSHAKE);
  tolua_constant(tolua_S,"E_CHAT",E_CHAT);
  tolua_constant(tolua_S,"E_UPDATE_TIME",E_UPDATE_TIME);
  tolua_constant(tolua_S,"E_ENTITY_EQUIPMENT",E_ENTITY_EQUIPMENT);
  tolua_constant(tolua_S,"E_CREATE_INVENTORY_ACTION",E_CREATE_INVENTORY_ACTION);
  tolua_constant(tolua_S,"E_USE_ENTITY",E_USE_ENTITY);
  tolua_constant(tolua_S,"E_UPDATE_HEALTH",E_UPDATE_HEALTH);
  tolua_constant(tolua_S,"E_RESPAWN",E_RESPAWN);
  tolua_constant(tolua_S,"E_FLYING",E_FLYING);
  tolua_constant(tolua_S,"E_PLAYERPOS",E_PLAYERPOS);
  tolua_constant(tolua_S,"E_PLAYERLOOK",E_PLAYERLOOK);
  tolua_constant(tolua_S,"E_PLAYERMOVELOOK",E_PLAYERMOVELOOK);
  tolua_constant(tolua_S,"E_BLOCK_DIG",E_BLOCK_DIG);
  tolua_constant(tolua_S,"E_BLOCK_PLACE",E_BLOCK_PLACE);
  tolua_constant(tolua_S,"E_ITEM_SWITCH",E_ITEM_SWITCH);
  tolua_constant(tolua_S,"E_ADD_TO_INV",E_ADD_TO_INV);
  tolua_constant(tolua_S,"E_ANIMATION",E_ANIMATION);
  tolua_constant(tolua_S,"E_PACKET_13",E_PACKET_13);
  tolua_constant(tolua_S,"E_NAMED_ENTITY_SPAWN",E_NAMED_ENTITY_SPAWN);
  tolua_constant(tolua_S,"E_PICKUP_SPAWN",E_PICKUP_SPAWN);
  tolua_constant(tolua_S,"E_COLLECT_ITEM",E_COLLECT_ITEM);
  tolua_constant(tolua_S,"E_ADD_VEHICLE",E_ADD_VEHICLE);
  tolua_constant(tolua_S,"E_SPAWN_MOB",E_SPAWN_MOB);
  tolua_constant(tolua_S,"E_DESTROY_ENT",E_DESTROY_ENT);
  tolua_constant(tolua_S,"E_ENTITY",E_ENTITY);
  tolua_constant(tolua_S,"E_REL_ENT_MOVE",E_REL_ENT_MOVE);
  tolua_constant(tolua_S,"E_ENT_LOOK",E_ENT_LOOK);
  tolua_constant(tolua_S,"E_REL_ENT_MOVE_LOOK",E_REL_ENT_MOVE_LOOK);
  tolua_constant(tolua_S,"E_ENT_TELEPORT",E_ENT_TELEPORT);
  tolua_constant(tolua_S,"E_ENT_STATUS",E_ENT_STATUS);
  tolua_constant(tolua_S,"E_METADATA",E_METADATA);
  tolua_constant(tolua_S,"E_PRE_CHUNK",E_PRE_CHUNK);
  tolua_constant(tolua_S,"E_MAP_CHUNK",E_MAP_CHUNK);
  tolua_constant(tolua_S,"E_MULTI_BLOCK",E_MULTI_BLOCK);
  tolua_constant(tolua_S,"E_BLOCK_CHANGE",E_BLOCK_CHANGE);
  tolua_constant(tolua_S,"E_WINDOW_OPEN",E_WINDOW_OPEN);
  tolua_constant(tolua_S,"E_WINDOW_CLOSE",E_WINDOW_CLOSE);
  tolua_constant(tolua_S,"E_WINDOW_CLICK",E_WINDOW_CLICK);
  tolua_constant(tolua_S,"E_INVENTORY_SLOT",E_INVENTORY_SLOT);
  tolua_constant(tolua_S,"E_INVENTORY_WHOLE",E_INVENTORY_WHOLE);
  tolua_constant(tolua_S,"E_INVENTORY_PROGRESS",E_INVENTORY_PROGRESS);
  tolua_constant(tolua_S,"E_UPDATE_SIGN",E_UPDATE_SIGN);
  tolua_constant(tolua_S,"E_DISCONNECT",E_DISCONNECT);
  tolua_array(tolua_S,"g_BlockLightValue",tolua_get_AllToLua_g_BlockLightValue,tolua_set_AllToLua_g_BlockLightValue);
  tolua_array(tolua_S,"g_BlockSpreadLightFalloff",tolua_get_AllToLua_g_BlockSpreadLightFalloff,tolua_set_AllToLua_g_BlockSpreadLightFalloff);
  tolua_array(tolua_S,"g_BlockTransparent",tolua_get_AllToLua_g_BlockTransparent,tolua_set_AllToLua_g_BlockTransparent);
  tolua_array(tolua_S,"g_BlockOneHitDig",tolua_get_AllToLua_g_BlockOneHitDig,tolua_set_AllToLua_g_BlockOneHitDig);
  tolua_function(tolua_S,"IsValidBlock",tolua_AllToLua_IsValidBlock00);
  tolua_function(tolua_S,"isValidItem",tolua_AllToLua_isValidItem00);
  tolua_function(tolua_S,"AddDirection",tolua_AllToLua_AddDirection00);
  tolua_function(tolua_S,"GetTime",tolua_AllToLua_GetTime00);
  tolua_function(tolua_S,"GetChar",tolua_AllToLua_GetChar00);
  tolua_cclass(tolua_S,"cStringMap","cStringMap","",NULL);
  tolua_beginmodule(tolua_S,"cStringMap");
   tolua_function(tolua_S,"clear",tolua_AllToLua_cStringMap_clear00);
   tolua_function(tolua_S,"size",tolua_AllToLua_cStringMap_size00);
   tolua_function(tolua_S,"get",tolua_AllToLua_cStringMap_get00);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"cChatColor","cChatColor","",NULL);
  tolua_beginmodule(tolua_S,"cChatColor");
   tolua_variable(tolua_S,"Color",tolua_get_cChatColor_Color,NULL);
   tolua_variable(tolua_S,"Black",tolua_get_cChatColor_Black,NULL);
   tolua_variable(tolua_S,"Navy",tolua_get_cChatColor_Navy,NULL);
   tolua_variable(tolua_S,"Green",tolua_get_cChatColor_Green,NULL);
   tolua_variable(tolua_S,"Blue",tolua_get_cChatColor_Blue,NULL);
   tolua_variable(tolua_S,"Red",tolua_get_cChatColor_Red,NULL);
   tolua_variable(tolua_S,"Purple",tolua_get_cChatColor_Purple,NULL);
   tolua_variable(tolua_S,"Gold",tolua_get_cChatColor_Gold,NULL);
   tolua_variable(tolua_S,"LightGray",tolua_get_cChatColor_LightGray,NULL);
   tolua_variable(tolua_S,"Gray",tolua_get_cChatColor_Gray,NULL);
   tolua_variable(tolua_S,"DarkPurple",tolua_get_cChatColor_DarkPurple,NULL);
   tolua_variable(tolua_S,"LightGreen",tolua_get_cChatColor_LightGreen,NULL);
   tolua_variable(tolua_S,"LightBlue",tolua_get_cChatColor_LightBlue,NULL);
   tolua_variable(tolua_S,"Rose",tolua_get_cChatColor_Rose,NULL);
   tolua_variable(tolua_S,"LightPurple",tolua_get_cChatColor_LightPurple,NULL);
   tolua_variable(tolua_S,"Yellow",tolua_get_cChatColor_Yellow,NULL);
   tolua_variable(tolua_S,"White",tolua_get_cChatColor_White,NULL);
   tolua_function(tolua_S,"MakeColor",tolua_AllToLua_cChatColor_MakeColor00);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"cClientHandle","cClientHandle","",NULL);
  tolua_beginmodule(tolua_S,"cClientHandle");
   tolua_function(tolua_S,"GetPlayer",tolua_AllToLua_cClientHandle_GetPlayer00);
   tolua_function(tolua_S,"Kick",tolua_AllToLua_cClientHandle_Kick00);
  tolua_endmodule(tolua_S);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"cEntity","cEntity","",tolua_collect_cEntity);
  #else
  tolua_cclass(tolua_S,"cEntity","cEntity","",NULL);
  #endif
  tolua_beginmodule(tolua_S,"cEntity");
   tolua_function(tolua_S,"delete",tolua_AllToLua_cEntity_delete00);
   tolua_function(tolua_S,"Initialize",tolua_AllToLua_cEntity_Initialize00);
   tolua_constant(tolua_S,"E_ENTITY",cEntity::E_ENTITY);
   tolua_constant(tolua_S,"E_PLAYER",cEntity::E_PLAYER);
   tolua_constant(tolua_S,"E_PICKUP",cEntity::E_PICKUP);
   tolua_function(tolua_S,"GetEntityType",tolua_AllToLua_cEntity_GetEntityType00);
   tolua_function(tolua_S,"IsA",tolua_AllToLua_cEntity_IsA00);
   tolua_function(tolua_S,"GetClass",tolua_AllToLua_cEntity_GetClass00);
   tolua_function(tolua_S,"GetPosition",tolua_AllToLua_cEntity_GetPosition00);
   tolua_function(tolua_S,"GetPosX",tolua_AllToLua_cEntity_GetPosX00);
   tolua_function(tolua_S,"GetPosY",tolua_AllToLua_cEntity_GetPosY00);
   tolua_function(tolua_S,"GetPosZ",tolua_AllToLua_cEntity_GetPosZ00);
   tolua_function(tolua_S,"GetRot",tolua_AllToLua_cEntity_GetRot00);
   tolua_function(tolua_S,"GetRotation",tolua_AllToLua_cEntity_GetRotation00);
   tolua_function(tolua_S,"GetPitch",tolua_AllToLua_cEntity_GetPitch00);
   tolua_function(tolua_S,"GetRoll",tolua_AllToLua_cEntity_GetRoll00);
   tolua_function(tolua_S,"GetLookVector",tolua_AllToLua_cEntity_GetLookVector00);
   tolua_function(tolua_S,"SetPosX",tolua_AllToLua_cEntity_SetPosX00);
   tolua_function(tolua_S,"SetPosY",tolua_AllToLua_cEntity_SetPosY00);
   tolua_function(tolua_S,"SetPosZ",tolua_AllToLua_cEntity_SetPosZ00);
   tolua_function(tolua_S,"SetPosition",tolua_AllToLua_cEntity_SetPosition00);
   tolua_function(tolua_S,"SetPosition",tolua_AllToLua_cEntity_SetPosition01);
   tolua_function(tolua_S,"SetRot",tolua_AllToLua_cEntity_SetRot00);
   tolua_function(tolua_S,"SetRotation",tolua_AllToLua_cEntity_SetRotation00);
   tolua_function(tolua_S,"SetPitch",tolua_AllToLua_cEntity_SetPitch00);
   tolua_function(tolua_S,"SetRoll",tolua_AllToLua_cEntity_SetRoll00);
   tolua_function(tolua_S,"GetUniqueID",tolua_AllToLua_cEntity_GetUniqueID00);
   tolua_function(tolua_S,"IsDestroyed",tolua_AllToLua_cEntity_IsDestroyed00);
   tolua_function(tolua_S,"Destroy",tolua_AllToLua_cEntity_Destroy00);
   tolua_function(tolua_S,"Tick",tolua_AllToLua_cEntity_Tick00);
   tolua_function(tolua_S,"SpawnOn",tolua_AllToLua_cEntity_SpawnOn00);
  tolua_endmodule(tolua_S);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"Lua__cEntity","Lua__cEntity","cEntity",tolua_collect_Lua__cEntity);
  #else
  tolua_cclass(tolua_S,"Lua__cEntity","Lua__cEntity","cEntity",NULL);
  #endif
  tolua_beginmodule(tolua_S,"Lua__cEntity");
   tolua_function(tolua_S,"tolua__set_instance",tolua_AllToLua_Lua__cEntity_tolua__set_instance00);
   tolua_function(tolua_S,"cEntity__Initialize",tolua_AllToLua_Lua__cEntity_cEntity__Initialize00);
   tolua_function(tolua_S,"cEntity__GetEntityType",tolua_AllToLua_Lua__cEntity_cEntity__GetEntityType00);
   tolua_function(tolua_S,"cEntity__IsA",tolua_AllToLua_Lua__cEntity_cEntity__IsA00);
   tolua_function(tolua_S,"cEntity__GetClass",tolua_AllToLua_Lua__cEntity_cEntity__GetClass00);
   tolua_function(tolua_S,"new",tolua_AllToLua_Lua__cEntity_new00);
   tolua_function(tolua_S,"new_local",tolua_AllToLua_Lua__cEntity_new00_local);
   tolua_function(tolua_S,".call",tolua_AllToLua_Lua__cEntity_new00_local);
   tolua_function(tolua_S,"delete",tolua_AllToLua_Lua__cEntity_delete00);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"TakeDamageInfo","TakeDamageInfo","",NULL);
  tolua_beginmodule(tolua_S,"TakeDamageInfo");
   tolua_variable(tolua_S,"Damage",tolua_get_TakeDamageInfo_Damage,tolua_set_TakeDamageInfo_Damage);
   tolua_variable(tolua_S,"Instigator",tolua_get_TakeDamageInfo_Instigator_ptr,tolua_set_TakeDamageInfo_Instigator_ptr);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"cPawn","cPawn","cEntity",NULL);
  tolua_beginmodule(tolua_S,"cPawn");
   tolua_function(tolua_S,"TeleportTo",tolua_AllToLua_cPawn_TeleportTo00);
   tolua_function(tolua_S,"TeleportTo",tolua_AllToLua_cPawn_TeleportTo01);
   tolua_function(tolua_S,"Heal",tolua_AllToLua_cPawn_Heal00);
   tolua_function(tolua_S,"TakeDamage",tolua_AllToLua_cPawn_TakeDamage00);
   tolua_function(tolua_S,"KilledBy",tolua_AllToLua_cPawn_KilledBy00);
   tolua_function(tolua_S,"GetHealth",tolua_AllToLua_cPawn_GetHealth00);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"Lua__cPawn","Lua__cPawn","cPawn",NULL);
  tolua_beginmodule(tolua_S,"Lua__cPawn");
   tolua_function(tolua_S,"tolua__set_instance",tolua_AllToLua_Lua__cPawn_tolua__set_instance00);
   tolua_function(tolua_S,"cPawn__TeleportTo",tolua_AllToLua_Lua__cPawn_cPawn__TeleportTo00);
   tolua_function(tolua_S,"cPawn__TeleportTo",tolua_AllToLua_Lua__cPawn_cPawn__TeleportTo01);
   tolua_function(tolua_S,"cPawn__TakeDamage",tolua_AllToLua_Lua__cPawn_cPawn__TakeDamage00);
   tolua_function(tolua_S,"cPawn__KilledBy",tolua_AllToLua_Lua__cPawn_cPawn__KilledBy00);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"cPlayer","cPlayer","cPawn",NULL);
  tolua_beginmodule(tolua_S,"cPlayer");
   tolua_function(tolua_S,"GetEyeHeight",tolua_AllToLua_cPlayer_GetEyeHeight00);
   tolua_function(tolua_S,"GetEyePosition",tolua_AllToLua_cPlayer_GetEyePosition00);
   tolua_function(tolua_S,"GetFlying",tolua_AllToLua_cPlayer_GetFlying00);
   tolua_function(tolua_S,"GetStance",tolua_AllToLua_cPlayer_GetStance00);
   tolua_function(tolua_S,"GetInventory",tolua_AllToLua_cPlayer_GetInventory00);
   tolua_function(tolua_S,"TeleportTo",tolua_AllToLua_cPlayer_TeleportTo00);
   tolua_function(tolua_S,"TeleportTo",tolua_AllToLua_cPlayer_TeleportTo01);
   tolua_function(tolua_S,"MoveTo",tolua_AllToLua_cPlayer_MoveTo00);
   tolua_function(tolua_S,"GetClientHandle",tolua_AllToLua_cPlayer_GetClientHandle00);
   tolua_function(tolua_S,"SendMessage",tolua_AllToLua_cPlayer_SendMessage00);
   tolua_function(tolua_S,"GetName",tolua_AllToLua_cPlayer_GetName00);
   tolua_function(tolua_S,"SetName",tolua_AllToLua_cPlayer_SetName00);
   tolua_function(tolua_S,"AddToGroup",tolua_AllToLua_cPlayer_AddToGroup00);
   tolua_function(tolua_S,"CanUseCommand",tolua_AllToLua_cPlayer_CanUseCommand00);
   tolua_function(tolua_S,"HasPermission",tolua_AllToLua_cPlayer_HasPermission00);
   tolua_function(tolua_S,"IsInGroup",tolua_AllToLua_cPlayer_IsInGroup00);
   tolua_function(tolua_S,"GetColor",tolua_AllToLua_cPlayer_GetColor00);
   tolua_function(tolua_S,"TossItem",tolua_AllToLua_cPlayer_TossItem00);
   tolua_function(tolua_S,"Heal",tolua_AllToLua_cPlayer_Heal00);
   tolua_function(tolua_S,"TakeDamage",tolua_AllToLua_cPlayer_TakeDamage00);
   tolua_function(tolua_S,"KilledBy",tolua_AllToLua_cPlayer_KilledBy00);
   tolua_function(tolua_S,"Respawn",tolua_AllToLua_cPlayer_Respawn00);
   tolua_function(tolua_S,"SetVisible",tolua_AllToLua_cPlayer_SetVisible00);
   tolua_function(tolua_S,"IsVisible",tolua_AllToLua_cPlayer_IsVisible00);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"Lua__cPlayer","Lua__cPlayer","cPlayer",NULL);
  tolua_beginmodule(tolua_S,"Lua__cPlayer");
   tolua_function(tolua_S,"tolua__set_instance",tolua_AllToLua_Lua__cPlayer_tolua__set_instance00);
   tolua_function(tolua_S,"cPlayer__TeleportTo",tolua_AllToLua_Lua__cPlayer_cPlayer__TeleportTo00);
   tolua_function(tolua_S,"cPlayer__TeleportTo",tolua_AllToLua_Lua__cPlayer_cPlayer__TeleportTo01);
   tolua_function(tolua_S,"cPlayer__MoveTo",tolua_AllToLua_Lua__cPlayer_cPlayer__MoveTo00);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"cPluginManager","cPluginManager","",NULL);
  tolua_beginmodule(tolua_S,"cPluginManager");
   tolua_constant(tolua_S,"E_PLUGIN_TICK",cPluginManager::E_PLUGIN_TICK);
   tolua_constant(tolua_S,"E_PLUGIN_CHAT",cPluginManager::E_PLUGIN_CHAT);
   tolua_constant(tolua_S,"E_PLUGIN_COLLECT_ITEM",cPluginManager::E_PLUGIN_COLLECT_ITEM);
   tolua_constant(tolua_S,"E_PLUGIN_BLOCK_DIG",cPluginManager::E_PLUGIN_BLOCK_DIG);
   tolua_constant(tolua_S,"E_PLUGIN_BLOCK_PLACE",cPluginManager::E_PLUGIN_BLOCK_PLACE);
   tolua_constant(tolua_S,"E_PLUGIN_DISCONNECT",cPluginManager::E_PLUGIN_DISCONNECT);
   tolua_constant(tolua_S,"E_PLUGIN_HANDSHAKE",cPluginManager::E_PLUGIN_HANDSHAKE);
   tolua_constant(tolua_S,"E_PLUGIN_LOGIN",cPluginManager::E_PLUGIN_LOGIN);
   tolua_constant(tolua_S,"E_PLUGIN_PLAYER_SPAWN",cPluginManager::E_PLUGIN_PLAYER_SPAWN);
   tolua_constant(tolua_S,"E_PLUGIN_PLAYER_JOIN",cPluginManager::E_PLUGIN_PLAYER_JOIN);
   tolua_constant(tolua_S,"E_PLUGIN_PLAYER_MOVE",cPluginManager::E_PLUGIN_PLAYER_MOVE);
   tolua_constant(tolua_S,"E_PLUGIN_TAKE_DAMAGE",cPluginManager::E_PLUGIN_TAKE_DAMAGE);
   tolua_constant(tolua_S,"E_PLUGIN_KILLED",cPluginManager::E_PLUGIN_KILLED);
   tolua_function(tolua_S,"GetPluginManager",tolua_AllToLua_cPluginManager_GetPluginManager00);
   tolua_function(tolua_S,"GetPlugin",tolua_AllToLua_cPluginManager_GetPlugin00);
   tolua_function(tolua_S,"ReloadPlugins",tolua_AllToLua_cPluginManager_ReloadPlugins00);
   tolua_function(tolua_S,"AddPlugin",tolua_AllToLua_cPluginManager_AddPlugin00);
   tolua_function(tolua_S,"AddHook",tolua_AllToLua_cPluginManager_AddHook00);
   tolua_function(tolua_S,"GetNumPlugins",tolua_AllToLua_cPluginManager_GetNumPlugins00);
   tolua_function(tolua_S,"RemovePlugin",tolua_AllToLua_cPluginManager_RemovePlugin00);
   tolua_function(tolua_S,"RemoveLuaPlugin",tolua_AllToLua_cPluginManager_RemoveLuaPlugin00);
   tolua_function(tolua_S,"GetLuaPlugin",tolua_AllToLua_cPluginManager_GetLuaPlugin00);
  tolua_endmodule(tolua_S);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"cPlugin","cPlugin","",tolua_collect_cPlugin);
  #else
  tolua_cclass(tolua_S,"cPlugin","cPlugin","",NULL);
  #endif
  tolua_beginmodule(tolua_S,"cPlugin");
   tolua_function(tolua_S,"delete",tolua_AllToLua_cPlugin_delete00);
   tolua_function(tolua_S,"OnDisable",tolua_AllToLua_cPlugin_OnDisable00);
   tolua_function(tolua_S,"Initialize",tolua_AllToLua_cPlugin_Initialize00);
   tolua_function(tolua_S,"Tick",tolua_AllToLua_cPlugin_Tick00);
   tolua_function(tolua_S,"OnCollectItem",tolua_AllToLua_cPlugin_OnCollectItem00);
   tolua_function(tolua_S,"OnDisconnect",tolua_AllToLua_cPlugin_OnDisconnect00);
   tolua_function(tolua_S,"OnBlockPlace",tolua_AllToLua_cPlugin_OnBlockPlace00);
   tolua_function(tolua_S,"OnBlockDig",tolua_AllToLua_cPlugin_OnBlockDig00);
   tolua_function(tolua_S,"OnChat",tolua_AllToLua_cPlugin_OnChat00);
   tolua_function(tolua_S,"OnLogin",tolua_AllToLua_cPlugin_OnLogin00);
   tolua_function(tolua_S,"OnPlayerSpawn",tolua_AllToLua_cPlugin_OnPlayerSpawn00);
   tolua_function(tolua_S,"OnPlayerJoin",tolua_AllToLua_cPlugin_OnPlayerJoin00);
   tolua_function(tolua_S,"OnPlayerMove",tolua_AllToLua_cPlugin_OnPlayerMove00);
   tolua_function(tolua_S,"OnTakeDamage",tolua_AllToLua_cPlugin_OnTakeDamage00);
   tolua_function(tolua_S,"OnKilled",tolua_AllToLua_cPlugin_OnKilled00);
   tolua_function(tolua_S,"GetName",tolua_AllToLua_cPlugin_GetName00);
   tolua_function(tolua_S,"SetName",tolua_AllToLua_cPlugin_SetName00);
   tolua_function(tolua_S,"GetVersion",tolua_AllToLua_cPlugin_GetVersion00);
   tolua_function(tolua_S,"SetVersion",tolua_AllToLua_cPlugin_SetVersion00);
   tolua_cclass(tolua_S,"CommandStruct","cPlugin::CommandStruct","",NULL);
   tolua_beginmodule(tolua_S,"CommandStruct");
    tolua_variable(tolua_S,"Command",tolua_get_cPlugin__CommandStruct_Command,tolua_set_cPlugin__CommandStruct_Command);
    tolua_variable(tolua_S,"Description",tolua_get_cPlugin__CommandStruct_Description,tolua_set_cPlugin__CommandStruct_Description);
    tolua_variable(tolua_S,"Permission",tolua_get_cPlugin__CommandStruct_Permission,tolua_set_cPlugin__CommandStruct_Permission);
   tolua_endmodule(tolua_S);
   tolua_function(tolua_S,"AddCommand",tolua_AllToLua_cPlugin_AddCommand00);
  tolua_endmodule(tolua_S);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"Lua__cPlugin","Lua__cPlugin","cPlugin",tolua_collect_Lua__cPlugin);
  #else
  tolua_cclass(tolua_S,"Lua__cPlugin","Lua__cPlugin","cPlugin",NULL);
  #endif
  tolua_beginmodule(tolua_S,"Lua__cPlugin");
   tolua_function(tolua_S,"tolua__set_instance",tolua_AllToLua_Lua__cPlugin_tolua__set_instance00);
   tolua_function(tolua_S,"cPlugin__OnDisable",tolua_AllToLua_Lua__cPlugin_cPlugin__OnDisable00);
   tolua_function(tolua_S,"cPlugin__Tick",tolua_AllToLua_Lua__cPlugin_cPlugin__Tick00);
   tolua_function(tolua_S,"cPlugin__OnCollectItem",tolua_AllToLua_Lua__cPlugin_cPlugin__OnCollectItem00);
   tolua_function(tolua_S,"cPlugin__OnDisconnect",tolua_AllToLua_Lua__cPlugin_cPlugin__OnDisconnect00);
   tolua_function(tolua_S,"cPlugin__OnBlockPlace",tolua_AllToLua_Lua__cPlugin_cPlugin__OnBlockPlace00);
   tolua_function(tolua_S,"cPlugin__OnBlockDig",tolua_AllToLua_Lua__cPlugin_cPlugin__OnBlockDig00);
   tolua_function(tolua_S,"cPlugin__OnChat",tolua_AllToLua_Lua__cPlugin_cPlugin__OnChat00);
   tolua_function(tolua_S,"cPlugin__OnLogin",tolua_AllToLua_Lua__cPlugin_cPlugin__OnLogin00);
   tolua_function(tolua_S,"cPlugin__OnPlayerSpawn",tolua_AllToLua_Lua__cPlugin_cPlugin__OnPlayerSpawn00);
   tolua_function(tolua_S,"cPlugin__OnPlayerJoin",tolua_AllToLua_Lua__cPlugin_cPlugin__OnPlayerJoin00);
   tolua_function(tolua_S,"cPlugin__OnPlayerMove",tolua_AllToLua_Lua__cPlugin_cPlugin__OnPlayerMove00);
   tolua_function(tolua_S,"cPlugin__OnTakeDamage",tolua_AllToLua_Lua__cPlugin_cPlugin__OnTakeDamage00);
   tolua_function(tolua_S,"cPlugin__OnKilled",tolua_AllToLua_Lua__cPlugin_cPlugin__OnKilled00);
   tolua_function(tolua_S,"new",tolua_AllToLua_Lua__cPlugin_new00);
   tolua_function(tolua_S,"new_local",tolua_AllToLua_Lua__cPlugin_new00_local);
   tolua_function(tolua_S,".call",tolua_AllToLua_Lua__cPlugin_new00_local);
   tolua_function(tolua_S,"delete",tolua_AllToLua_Lua__cPlugin_delete00);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"cPlugin_Lua","cPlugin_Lua","",NULL);
  tolua_beginmodule(tolua_S,"cPlugin_Lua");
   tolua_function(tolua_S,"GetFileName",tolua_AllToLua_cPlugin_Lua_GetFileName00);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"cServer","cServer","",NULL);
  tolua_beginmodule(tolua_S,"cServer");
   tolua_function(tolua_S,"GetServer",tolua_AllToLua_cServer_GetServer00);
   tolua_function(tolua_S,"ServerCommand",tolua_AllToLua_cServer_ServerCommand00);
   tolua_function(tolua_S,"SendMessage",tolua_AllToLua_cServer_SendMessage00);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"cWorld","cWorld","",NULL);
  tolua_beginmodule(tolua_S,"cWorld");
   tolua_function(tolua_S,"GetWorld",tolua_AllToLua_cWorld_GetWorld00);
   tolua_function(tolua_S,"GetTime",tolua_AllToLua_cWorld_GetTime00);
   tolua_function(tolua_S,"SetWorldTime",tolua_AllToLua_cWorld_SetWorldTime00);
   tolua_function(tolua_S,"GetHeight",tolua_AllToLua_cWorld_GetHeight00);
   tolua_function(tolua_S,"GetNumPlayers",tolua_AllToLua_cWorld_GetNumPlayers00);
   tolua_function(tolua_S,"GetPlayer",tolua_AllToLua_cWorld_GetPlayer00);
   tolua_function(tolua_S,"GetEntity",tolua_AllToLua_cWorld_GetEntity00);
   tolua_function(tolua_S,"SetBlock",tolua_AllToLua_cWorld_SetBlock00);
   tolua_function(tolua_S,"FastSetBlock",tolua_AllToLua_cWorld_FastSetBlock00);
   tolua_function(tolua_S,"GetBlock",tolua_AllToLua_cWorld_GetBlock00);
   tolua_function(tolua_S,"GetBlockMeta",tolua_AllToLua_cWorld_GetBlockMeta00);
   tolua_function(tolua_S,"SetBlockMeta",tolua_AllToLua_cWorld_SetBlockMeta00);
   tolua_function(tolua_S,"DigBlock",tolua_AllToLua_cWorld_DigBlock00);
   tolua_function(tolua_S,"SendBlockTo",tolua_AllToLua_cWorld_SendBlockTo00);
   tolua_function(tolua_S,"GetSpawnX",tolua_AllToLua_cWorld_GetSpawnX00);
   tolua_function(tolua_S,"GetSpawnY",tolua_AllToLua_cWorld_GetSpawnY00);
   tolua_function(tolua_S,"GetSpawnZ",tolua_AllToLua_cWorld_GetSpawnZ00);
   tolua_function(tolua_S,"GetBlockEntity",tolua_AllToLua_cWorld_GetBlockEntity00);
   tolua_function(tolua_S,"GrowTree",tolua_AllToLua_cWorld_GrowTree00);
   tolua_function(tolua_S,"GetWorldSeed",tolua_AllToLua_cWorld_GetWorldSeed00);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"cInventory","cInventory","",NULL);
  tolua_beginmodule(tolua_S,"cInventory");
   tolua_function(tolua_S,"Clear",tolua_AllToLua_cInventory_Clear00);
   tolua_function(tolua_S,"AddItem",tolua_AllToLua_cInventory_AddItem00);
   tolua_function(tolua_S,"RemoveItem",tolua_AllToLua_cInventory_RemoveItem00);
   tolua_function(tolua_S,"GetSlot",tolua_AllToLua_cInventory_GetSlot00);
   tolua_function(tolua_S,"GetFromHotBar",tolua_AllToLua_cInventory_GetFromHotBar00);
   tolua_function(tolua_S,"GetEquippedItem",tolua_AllToLua_cInventory_GetEquippedItem00);
   tolua_function(tolua_S,"SetEquippedSlot",tolua_AllToLua_cInventory_SetEquippedSlot00);
   tolua_function(tolua_S,"SendSlot",tolua_AllToLua_cInventory_SendSlot00);
  tolua_endmodule(tolua_S);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"cItem","cItem","",tolua_collect_cItem);
  #else
  tolua_cclass(tolua_S,"cItem","cItem","",NULL);
  #endif
  tolua_beginmodule(tolua_S,"cItem");
   tolua_function(tolua_S,"new",tolua_AllToLua_cItem_new00);
   tolua_function(tolua_S,"new_local",tolua_AllToLua_cItem_new00_local);
   tolua_function(tolua_S,".call",tolua_AllToLua_cItem_new00_local);
   tolua_function(tolua_S,"Empty",tolua_AllToLua_cItem_Empty00);
   tolua_function(tolua_S,"IsEmpty",tolua_AllToLua_cItem_IsEmpty00);
   tolua_function(tolua_S,"Equals",tolua_AllToLua_cItem_Equals00);
   tolua_variable(tolua_S,"m_ItemID",tolua_get_cItem_m_ItemID,tolua_set_cItem_m_ItemID);
   tolua_variable(tolua_S,"m_ItemCount",tolua_get_cItem_m_ItemCount,tolua_set_cItem_m_ItemCount);
   tolua_variable(tolua_S,"m_ItemHealth",tolua_get_cItem_m_ItemHealth,tolua_set_cItem_m_ItemHealth);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"HTTPRequest","HTTPRequest","",NULL);
  tolua_beginmodule(tolua_S,"HTTPRequest");
   tolua_variable(tolua_S,"Method",tolua_get_HTTPRequest_Method,tolua_set_HTTPRequest_Method);
   tolua_variable(tolua_S,"Path",tolua_get_HTTPRequest_Path,tolua_set_HTTPRequest_Path);
   tolua_variable(tolua_S,"Params",tolua_get_HTTPRequest_Params_ptr,tolua_set_HTTPRequest_Params_ptr);
   tolua_variable(tolua_S,"Username",tolua_get_HTTPRequest_Username,tolua_set_HTTPRequest_Username);
  tolua_endmodule(tolua_S);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"cWebPlugin","cWebPlugin","",tolua_collect_cWebPlugin);
  #else
  tolua_cclass(tolua_S,"cWebPlugin","cWebPlugin","",NULL);
  #endif
  tolua_beginmodule(tolua_S,"cWebPlugin");
   tolua_function(tolua_S,"delete",tolua_AllToLua_cWebPlugin_delete00);
   tolua_function(tolua_S,"SetName",tolua_AllToLua_cWebPlugin_SetName00);
   tolua_function(tolua_S,"GetName",tolua_AllToLua_cWebPlugin_GetName00);
   tolua_function(tolua_S,"HandleRequest",tolua_AllToLua_cWebPlugin_HandleRequest00);
   tolua_function(tolua_S,"Initialize",tolua_AllToLua_cWebPlugin_Initialize00);
  tolua_endmodule(tolua_S);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"Lua__cWebPlugin","Lua__cWebPlugin","cWebPlugin",tolua_collect_Lua__cWebPlugin);
  #else
  tolua_cclass(tolua_S,"Lua__cWebPlugin","Lua__cWebPlugin","cWebPlugin",NULL);
  #endif
  tolua_beginmodule(tolua_S,"Lua__cWebPlugin");
   tolua_function(tolua_S,"tolua__set_instance",tolua_AllToLua_Lua__cWebPlugin_tolua__set_instance00);
   tolua_function(tolua_S,"new",tolua_AllToLua_Lua__cWebPlugin_new00);
   tolua_function(tolua_S,"new_local",tolua_AllToLua_Lua__cWebPlugin_new00_local);
   tolua_function(tolua_S,".call",tolua_AllToLua_Lua__cWebPlugin_new00_local);
   tolua_function(tolua_S,"delete",tolua_AllToLua_Lua__cWebPlugin_delete00);
  tolua_endmodule(tolua_S);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"cPickup","cPickup","cEntity",tolua_collect_cPickup);
  #else
  tolua_cclass(tolua_S,"cPickup","cPickup","cEntity",NULL);
  #endif
  tolua_beginmodule(tolua_S,"cPickup");
   tolua_function(tolua_S,"new",tolua_AllToLua_cPickup_new00);
   tolua_function(tolua_S,"new_local",tolua_AllToLua_cPickup_new00_local);
   tolua_function(tolua_S,".call",tolua_AllToLua_cPickup_new00_local);
   tolua_function(tolua_S,"new",tolua_AllToLua_cPickup_new01);
   tolua_function(tolua_S,"new_local",tolua_AllToLua_cPickup_new01_local);
   tolua_function(tolua_S,".call",tolua_AllToLua_cPickup_new01_local);
   tolua_function(tolua_S,"delete",tolua_AllToLua_cPickup_delete00);
   tolua_function(tolua_S,"GetItem",tolua_AllToLua_cPickup_GetItem00);
   tolua_function(tolua_S,"CollectedBy",tolua_AllToLua_cPickup_CollectedBy00);
  tolua_endmodule(tolua_S);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"Lua__cPickup","Lua__cPickup","cPickup",tolua_collect_Lua__cPickup);
  #else
  tolua_cclass(tolua_S,"Lua__cPickup","Lua__cPickup","cPickup",NULL);
  #endif
  tolua_beginmodule(tolua_S,"Lua__cPickup");
   tolua_function(tolua_S,"tolua__set_instance",tolua_AllToLua_Lua__cPickup_tolua__set_instance00);
   tolua_function(tolua_S,"cPickup__CollectedBy",tolua_AllToLua_Lua__cPickup_cPickup__CollectedBy00);
   tolua_function(tolua_S,"new",tolua_AllToLua_Lua__cPickup_new00);
   tolua_function(tolua_S,"new_local",tolua_AllToLua_Lua__cPickup_new00_local);
   tolua_function(tolua_S,".call",tolua_AllToLua_Lua__cPickup_new00_local);
   tolua_function(tolua_S,"new",tolua_AllToLua_Lua__cPickup_new01);
   tolua_function(tolua_S,"new_local",tolua_AllToLua_Lua__cPickup_new01_local);
   tolua_function(tolua_S,".call",tolua_AllToLua_Lua__cPickup_new01_local);
   tolua_function(tolua_S,"delete",tolua_AllToLua_Lua__cPickup_delete00);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"cRoot","cRoot","",NULL);
  tolua_beginmodule(tolua_S,"cRoot");
   tolua_function(tolua_S,"Get",tolua_AllToLua_cRoot_Get00);
   tolua_function(tolua_S,"GetServer",tolua_AllToLua_cRoot_GetServer00);
   tolua_function(tolua_S,"GetWorld",tolua_AllToLua_cRoot_GetWorld00);
   tolua_function(tolua_S,"GetGroupManager",tolua_AllToLua_cRoot_GetGroupManager00);
   tolua_function(tolua_S,"GetRecipeChecker",tolua_AllToLua_cRoot_GetRecipeChecker00);
   tolua_function(tolua_S,"GetFurnaceRecipe",tolua_AllToLua_cRoot_GetFurnaceRecipe00);
   tolua_function(tolua_S,"GetWebAdmin",tolua_AllToLua_cRoot_GetWebAdmin00);
   tolua_function(tolua_S,"GetPluginManager",tolua_AllToLua_cRoot_GetPluginManager00);
   tolua_function(tolua_S,"ServerCommand",tolua_AllToLua_cRoot_ServerCommand00);
  tolua_endmodule(tolua_S);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"cTCPLink","cTCPLink","",tolua_collect_cTCPLink);
  #else
  tolua_cclass(tolua_S,"cTCPLink","cTCPLink","",NULL);
  #endif
  tolua_beginmodule(tolua_S,"cTCPLink");
   tolua_function(tolua_S,"delete",tolua_AllToLua_cTCPLink_delete00);
   tolua_function(tolua_S,"Connect",tolua_AllToLua_cTCPLink_Connect00);
   tolua_function(tolua_S,"Send",tolua_AllToLua_cTCPLink_Send00);
   tolua_function(tolua_S,"SendMessage",tolua_AllToLua_cTCPLink_SendMessage00);
   tolua_function(tolua_S,"CloseSocket",tolua_AllToLua_cTCPLink_CloseSocket00);
  tolua_endmodule(tolua_S);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"Lua__cTCPLink","Lua__cTCPLink","cTCPLink",tolua_collect_Lua__cTCPLink);
  #else
  tolua_cclass(tolua_S,"Lua__cTCPLink","Lua__cTCPLink","cTCPLink",NULL);
  #endif
  tolua_beginmodule(tolua_S,"Lua__cTCPLink");
   tolua_function(tolua_S,"tolua__set_instance",tolua_AllToLua_Lua__cTCPLink_tolua__set_instance00);
   tolua_function(tolua_S,"new",tolua_AllToLua_Lua__cTCPLink_new00);
   tolua_function(tolua_S,"new_local",tolua_AllToLua_Lua__cTCPLink_new00_local);
   tolua_function(tolua_S,".call",tolua_AllToLua_Lua__cTCPLink_new00_local);
   tolua_function(tolua_S,"delete",tolua_AllToLua_Lua__cTCPLink_delete00);
  tolua_endmodule(tolua_S);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"Vector3f","Vector3f","",tolua_collect_Vector3f);
  #else
  tolua_cclass(tolua_S,"Vector3f","Vector3f","",NULL);
  #endif
  tolua_beginmodule(tolua_S,"Vector3f");
   tolua_function(tolua_S,"new",tolua_AllToLua_Vector3f_new00);
   tolua_function(tolua_S,"new_local",tolua_AllToLua_Vector3f_new00_local);
   tolua_function(tolua_S,".call",tolua_AllToLua_Vector3f_new00_local);
   tolua_function(tolua_S,"new",tolua_AllToLua_Vector3f_new01);
   tolua_function(tolua_S,"new_local",tolua_AllToLua_Vector3f_new01_local);
   tolua_function(tolua_S,".call",tolua_AllToLua_Vector3f_new01_local);
   tolua_function(tolua_S,"new",tolua_AllToLua_Vector3f_new02);
   tolua_function(tolua_S,"new_local",tolua_AllToLua_Vector3f_new02_local);
   tolua_function(tolua_S,".call",tolua_AllToLua_Vector3f_new02_local);
   tolua_function(tolua_S,"new",tolua_AllToLua_Vector3f_new03);
   tolua_function(tolua_S,"new_local",tolua_AllToLua_Vector3f_new03_local);
   tolua_function(tolua_S,".call",tolua_AllToLua_Vector3f_new03_local);
   tolua_function(tolua_S,"new",tolua_AllToLua_Vector3f_new04);
   tolua_function(tolua_S,"new_local",tolua_AllToLua_Vector3f_new04_local);
   tolua_function(tolua_S,".call",tolua_AllToLua_Vector3f_new04_local);
   tolua_function(tolua_S,"new",tolua_AllToLua_Vector3f_new05);
   tolua_function(tolua_S,"new_local",tolua_AllToLua_Vector3f_new05_local);
   tolua_function(tolua_S,".call",tolua_AllToLua_Vector3f_new05_local);
   tolua_function(tolua_S,"Set",tolua_AllToLua_Vector3f_Set00);
   tolua_function(tolua_S,"Normalize",tolua_AllToLua_Vector3f_Normalize00);
   tolua_function(tolua_S,"NormalizeCopy",tolua_AllToLua_Vector3f_NormalizeCopy00);
   tolua_function(tolua_S,"NormalizeCopy",tolua_AllToLua_Vector3f_NormalizeCopy01);
   tolua_function(tolua_S,"Length",tolua_AllToLua_Vector3f_Length00);
   tolua_function(tolua_S,"SqrLength",tolua_AllToLua_Vector3f_SqrLength00);
   tolua_function(tolua_S,"Dot",tolua_AllToLua_Vector3f_Dot00);
   tolua_function(tolua_S,"Cross",tolua_AllToLua_Vector3f_Cross00);
   tolua_function(tolua_S,"Equals",tolua_AllToLua_Vector3f_Equals00);
   tolua_function(tolua_S,".add",tolua_AllToLua_Vector3f__add00);
   tolua_function(tolua_S,".add",tolua_AllToLua_Vector3f__add01);
   tolua_function(tolua_S,".sub",tolua_AllToLua_Vector3f__sub00);
   tolua_function(tolua_S,".sub",tolua_AllToLua_Vector3f__sub01);
   tolua_function(tolua_S,".mul",tolua_AllToLua_Vector3f__mul00);
   tolua_function(tolua_S,".mul",tolua_AllToLua_Vector3f__mul01);
   tolua_variable(tolua_S,"x",tolua_get_Vector3f_x,tolua_set_Vector3f_x);
   tolua_variable(tolua_S,"y",tolua_get_Vector3f_y,tolua_set_Vector3f_y);
   tolua_variable(tolua_S,"z",tolua_get_Vector3f_z,tolua_set_Vector3f_z);
  tolua_endmodule(tolua_S);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"Vector3d","Vector3d","",tolua_collect_Vector3d);
  #else
  tolua_cclass(tolua_S,"Vector3d","Vector3d","",NULL);
  #endif
  tolua_beginmodule(tolua_S,"Vector3d");
   tolua_function(tolua_S,"new",tolua_AllToLua_Vector3d_new00);
   tolua_function(tolua_S,"new_local",tolua_AllToLua_Vector3d_new00_local);
   tolua_function(tolua_S,".call",tolua_AllToLua_Vector3d_new00_local);
   tolua_function(tolua_S,"new",tolua_AllToLua_Vector3d_new01);
   tolua_function(tolua_S,"new_local",tolua_AllToLua_Vector3d_new01_local);
   tolua_function(tolua_S,".call",tolua_AllToLua_Vector3d_new01_local);
   tolua_function(tolua_S,"new",tolua_AllToLua_Vector3d_new02);
   tolua_function(tolua_S,"new_local",tolua_AllToLua_Vector3d_new02_local);
   tolua_function(tolua_S,".call",tolua_AllToLua_Vector3d_new02_local);
   tolua_function(tolua_S,"new",tolua_AllToLua_Vector3d_new03);
   tolua_function(tolua_S,"new_local",tolua_AllToLua_Vector3d_new03_local);
   tolua_function(tolua_S,".call",tolua_AllToLua_Vector3d_new03_local);
   tolua_function(tolua_S,"Set",tolua_AllToLua_Vector3d_Set00);
   tolua_function(tolua_S,"Normalize",tolua_AllToLua_Vector3d_Normalize00);
   tolua_function(tolua_S,"NormalizeCopy",tolua_AllToLua_Vector3d_NormalizeCopy00);
   tolua_function(tolua_S,"NormalizeCopy",tolua_AllToLua_Vector3d_NormalizeCopy01);
   tolua_function(tolua_S,"Length",tolua_AllToLua_Vector3d_Length00);
   tolua_function(tolua_S,"SqrLength",tolua_AllToLua_Vector3d_SqrLength00);
   tolua_function(tolua_S,"Dot",tolua_AllToLua_Vector3d_Dot00);
   tolua_function(tolua_S,"Cross",tolua_AllToLua_Vector3d_Cross00);
   tolua_function(tolua_S,".add",tolua_AllToLua_Vector3d__add00);
   tolua_function(tolua_S,".add",tolua_AllToLua_Vector3d__add01);
   tolua_function(tolua_S,".sub",tolua_AllToLua_Vector3d__sub00);
   tolua_function(tolua_S,".sub",tolua_AllToLua_Vector3d__sub01);
   tolua_function(tolua_S,".mul",tolua_AllToLua_Vector3d__mul00);
   tolua_function(tolua_S,".mul",tolua_AllToLua_Vector3d__mul01);
   tolua_variable(tolua_S,"x",tolua_get_Vector3d_x,tolua_set_Vector3d_x);
   tolua_variable(tolua_S,"y",tolua_get_Vector3d_y,tolua_set_Vector3d_y);
   tolua_variable(tolua_S,"z",tolua_get_Vector3d_z,tolua_set_Vector3d_z);
  tolua_endmodule(tolua_S);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"Vector3i","Vector3i","",tolua_collect_Vector3i);
  #else
  tolua_cclass(tolua_S,"Vector3i","Vector3i","",NULL);
  #endif
  tolua_beginmodule(tolua_S,"Vector3i");
   tolua_function(tolua_S,"new",tolua_AllToLua_Vector3i_new00);
   tolua_function(tolua_S,"new_local",tolua_AllToLua_Vector3i_new00_local);
   tolua_function(tolua_S,".call",tolua_AllToLua_Vector3i_new00_local);
   tolua_function(tolua_S,"new",tolua_AllToLua_Vector3i_new01);
   tolua_function(tolua_S,"new_local",tolua_AllToLua_Vector3i_new01_local);
   tolua_function(tolua_S,".call",tolua_AllToLua_Vector3i_new01_local);
   tolua_function(tolua_S,"new",tolua_AllToLua_Vector3i_new02);
   tolua_function(tolua_S,"new_local",tolua_AllToLua_Vector3i_new02_local);
   tolua_function(tolua_S,".call",tolua_AllToLua_Vector3i_new02_local);
   tolua_function(tolua_S,"Set",tolua_AllToLua_Vector3i_Set00);
   tolua_function(tolua_S,"Length",tolua_AllToLua_Vector3i_Length00);
   tolua_function(tolua_S,"SqrLength",tolua_AllToLua_Vector3i_SqrLength00);
   tolua_function(tolua_S,"Equals",tolua_AllToLua_Vector3i_Equals00);
   tolua_function(tolua_S,"Equals",tolua_AllToLua_Vector3i_Equals01);
   tolua_variable(tolua_S,"x",tolua_get_Vector3i_x,tolua_set_Vector3i_x);
   tolua_variable(tolua_S,"y",tolua_get_Vector3i_y,tolua_set_Vector3i_y);
   tolua_variable(tolua_S,"z",tolua_get_Vector3i_z,tolua_set_Vector3i_z);
  tolua_endmodule(tolua_S);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"cCuboid","cCuboid","",tolua_collect_cCuboid);
  #else
  tolua_cclass(tolua_S,"cCuboid","cCuboid","",NULL);
  #endif
  tolua_beginmodule(tolua_S,"cCuboid");
   tolua_function(tolua_S,"new",tolua_AllToLua_cCuboid_new00);
   tolua_function(tolua_S,"new_local",tolua_AllToLua_cCuboid_new00_local);
   tolua_function(tolua_S,".call",tolua_AllToLua_cCuboid_new00_local);
   tolua_function(tolua_S,"new",tolua_AllToLua_cCuboid_new01);
   tolua_function(tolua_S,"new_local",tolua_AllToLua_cCuboid_new01_local);
   tolua_function(tolua_S,".call",tolua_AllToLua_cCuboid_new01_local);
   tolua_function(tolua_S,"new",tolua_AllToLua_cCuboid_new02);
   tolua_function(tolua_S,"new_local",tolua_AllToLua_cCuboid_new02_local);
   tolua_function(tolua_S,".call",tolua_AllToLua_cCuboid_new02_local);
   tolua_variable(tolua_S,"p1",tolua_get_cCuboid_p1,tolua_set_cCuboid_p1);
   tolua_variable(tolua_S,"p2",tolua_get_cCuboid_p2,tolua_set_cCuboid_p2);
   tolua_function(tolua_S,"Sort",tolua_AllToLua_cCuboid_Sort00);
   tolua_function(tolua_S,"IsInside",tolua_AllToLua_cCuboid_IsInside00);
   tolua_function(tolua_S,"IsInside",tolua_AllToLua_cCuboid_IsInside01);
  tolua_endmodule(tolua_S);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"cMCLogger","cMCLogger","",tolua_collect_cMCLogger);
  #else
  tolua_cclass(tolua_S,"cMCLogger","cMCLogger","",NULL);
  #endif
  tolua_beginmodule(tolua_S,"cMCLogger");
   tolua_function(tolua_S,"new",tolua_AllToLua_cMCLogger_new00);
   tolua_function(tolua_S,"new_local",tolua_AllToLua_cMCLogger_new00_local);
   tolua_function(tolua_S,".call",tolua_AllToLua_cMCLogger_new00_local);
   tolua_function(tolua_S,"delete",tolua_AllToLua_cMCLogger_delete00);
   tolua_function(tolua_S,"LogSimple",tolua_AllToLua_cMCLogger_LogSimple00);
  tolua_endmodule(tolua_S);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"cTracer","cTracer","",tolua_collect_cTracer);
  #else
  tolua_cclass(tolua_S,"cTracer","cTracer","",NULL);
  #endif
  tolua_beginmodule(tolua_S,"cTracer");
   tolua_function(tolua_S,"new",tolua_AllToLua_cTracer_new00);
   tolua_function(tolua_S,"new_local",tolua_AllToLua_cTracer_new00_local);
   tolua_function(tolua_S,".call",tolua_AllToLua_cTracer_new00_local);
   tolua_function(tolua_S,"delete",tolua_AllToLua_cTracer_delete00);
   tolua_function(tolua_S,"Trace",tolua_AllToLua_cTracer_Trace00);
   tolua_function(tolua_S,"SetValues",tolua_AllToLua_cTracer_SetValues00);
   tolua_variable(tolua_S,"BlockHitPosition",tolua_get_cTracer_BlockHitPosition_ptr,tolua_set_cTracer_BlockHitPosition_ptr);
   tolua_variable(tolua_S,"HitNormal",tolua_get_cTracer_HitNormal_ptr,tolua_set_cTracer_HitNormal_ptr);
   tolua_variable(tolua_S,"RealHit",tolua_get_cTracer_RealHit_ptr,tolua_set_cTracer_RealHit_ptr);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"cGroup","cGroup","",NULL);
  tolua_beginmodule(tolua_S,"cGroup");
   tolua_function(tolua_S,"SetName",tolua_AllToLua_cGroup_SetName00);
   tolua_function(tolua_S,"GetName",tolua_AllToLua_cGroup_GetName00);
   tolua_function(tolua_S,"SetColor",tolua_AllToLua_cGroup_SetColor00);
   tolua_function(tolua_S,"AddCommand",tolua_AllToLua_cGroup_AddCommand00);
   tolua_function(tolua_S,"AddPermission",tolua_AllToLua_cGroup_AddPermission00);
   tolua_function(tolua_S,"InheritFrom",tolua_AllToLua_cGroup_InheritFrom00);
   tolua_function(tolua_S,"HasCommand",tolua_AllToLua_cGroup_HasCommand00);
   tolua_function(tolua_S,"GetColor",tolua_AllToLua_cGroup_GetColor00);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"cPacket_Login","cPacket_Login","cPacket",NULL);
  tolua_beginmodule(tolua_S,"cPacket_Login");
   tolua_variable(tolua_S,"m_ProtocolVersion",tolua_get_cPacket_Login_m_ProtocolVersion,tolua_set_cPacket_Login_m_ProtocolVersion);
   tolua_variable(tolua_S,"m_Username",tolua_get_cPacket_Login_m_Username,tolua_set_cPacket_Login_m_Username);
   tolua_variable(tolua_S,"m_MapSeed",tolua_get_cPacket_Login_m_MapSeed,tolua_set_cPacket_Login_m_MapSeed);
   tolua_variable(tolua_S,"m_Dimension",tolua_get_cPacket_Login_m_Dimension,tolua_set_cPacket_Login_m_Dimension);
  tolua_endmodule(tolua_S);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"cPacket_BlockDig","cPacket_BlockDig","cPacket",tolua_collect_cPacket_BlockDig);
  #else
  tolua_cclass(tolua_S,"cPacket_BlockDig","cPacket_BlockDig","cPacket",NULL);
  #endif
  tolua_beginmodule(tolua_S,"cPacket_BlockDig");
   tolua_function(tolua_S,"new",tolua_AllToLua_cPacket_BlockDig_new00);
   tolua_function(tolua_S,"new_local",tolua_AllToLua_cPacket_BlockDig_new00_local);
   tolua_function(tolua_S,".call",tolua_AllToLua_cPacket_BlockDig_new00_local);
   tolua_function(tolua_S,"Clone",tolua_AllToLua_cPacket_BlockDig_Clone00);
   tolua_variable(tolua_S,"m_Status",tolua_get_cPacket_BlockDig_m_Status,tolua_set_cPacket_BlockDig_m_Status);
   tolua_variable(tolua_S,"m_PosX",tolua_get_cPacket_BlockDig_m_PosX,tolua_set_cPacket_BlockDig_m_PosX);
   tolua_variable(tolua_S,"m_PosY",tolua_get_cPacket_BlockDig_m_PosY,tolua_set_cPacket_BlockDig_m_PosY);
   tolua_variable(tolua_S,"m_PosZ",tolua_get_cPacket_BlockDig_m_PosZ,tolua_set_cPacket_BlockDig_m_PosZ);
   tolua_variable(tolua_S,"m_Direction",tolua_get_cPacket_BlockDig_m_Direction,tolua_set_cPacket_BlockDig_m_Direction);
  tolua_endmodule(tolua_S);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"Lua__cPacket_BlockDig","Lua__cPacket_BlockDig","cPacket_BlockDig",tolua_collect_Lua__cPacket_BlockDig);
  #else
  tolua_cclass(tolua_S,"Lua__cPacket_BlockDig","Lua__cPacket_BlockDig","cPacket_BlockDig",NULL);
  #endif
  tolua_beginmodule(tolua_S,"Lua__cPacket_BlockDig");
   tolua_function(tolua_S,"tolua__set_instance",tolua_AllToLua_Lua__cPacket_BlockDig_tolua__set_instance00);
   tolua_function(tolua_S,"cPacket_BlockDig__Clone",tolua_AllToLua_Lua__cPacket_BlockDig_cPacket_BlockDig__Clone00);
   tolua_function(tolua_S,"new",tolua_AllToLua_Lua__cPacket_BlockDig_new00);
   tolua_function(tolua_S,"new_local",tolua_AllToLua_Lua__cPacket_BlockDig_new00_local);
   tolua_function(tolua_S,".call",tolua_AllToLua_Lua__cPacket_BlockDig_new00_local);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"cPacket_BlockPlace","cPacket_BlockPlace","cPacket",NULL);
  tolua_beginmodule(tolua_S,"cPacket_BlockPlace");
   tolua_variable(tolua_S,"m_PosX",tolua_get_cPacket_BlockPlace_m_PosX,tolua_set_cPacket_BlockPlace_m_PosX);
   tolua_variable(tolua_S,"m_PosY",tolua_get_cPacket_BlockPlace_m_PosY,tolua_set_cPacket_BlockPlace_m_PosY);
   tolua_variable(tolua_S,"m_PosZ",tolua_get_cPacket_BlockPlace_m_PosZ,tolua_set_cPacket_BlockPlace_m_PosZ);
   tolua_variable(tolua_S,"m_Direction",tolua_get_cPacket_BlockPlace_m_Direction,tolua_set_cPacket_BlockPlace_m_Direction);
   tolua_variable(tolua_S,"m_ItemType",tolua_get_cPacket_BlockPlace_m_ItemType,tolua_set_cPacket_BlockPlace_m_ItemType);
   tolua_variable(tolua_S,"m_Count",tolua_get_cPacket_BlockPlace_m_Count,tolua_set_cPacket_BlockPlace_m_Count);
   tolua_variable(tolua_S,"m_Uses",tolua_get_cPacket_BlockPlace_m_Uses,tolua_set_cPacket_BlockPlace_m_Uses);
  tolua_endmodule(tolua_S);
 tolua_endmodule(tolua_S);
 return 1;
}


#if defined(LUA_VERSION_NUM) && LUA_VERSION_NUM >= 501
 TOLUA_API int luaopen_AllToLua (lua_State* tolua_S) {
 return tolua_AllToLua_open(tolua_S);
};
#endif

