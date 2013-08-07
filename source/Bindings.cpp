/*
** Lua binding: AllToLua
** Generated automatically by tolua++-1.0.92 on 08/07/13 12:06:03.
*/

#ifndef __cplusplus
#include "stdlib.h"
#endif
#include "string.h"

#include "tolua++.h"

/* Exported function */
TOLUA_API int  tolua_AllToLua_open (lua_State* tolua_S);

#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules
#include "tolua_base.h"
#include "ChunkDef.h"
#include "../iniFile/iniFile.h"
#include "BlockID.h"
#include "StringUtils.h"
#include "Defines.h"
#include "LuaFunctions.h"
#include "StringMap.h"
#include "ChatColor.h"
#include "ClientHandle.h"
#include "Entity.h"
#include "Pawn.h"
#include "Player.h"
#include "PluginManager.h"
#include "Plugin.h"
#include "Plugin_NewLua.h"
#include "Server.h"
#include "World.h"
#include "Inventory.h"
#include "Enchantments.h"
#include "Item.h"
#include "ItemGrid.h"
#include "BlockEntities/BlockEntity.h"
#include "BlockEntities/BlockEntityWithItems.h"
#include "BlockEntities/ChestEntity.h"
#include "BlockEntities/DropSpenserEntity.h"
#include "BlockEntities/DispenserEntity.h"
#include "BlockEntities/DropperEntity.h"
#include "BlockEntities/FurnaceEntity.h"
#include "WebAdmin.h"
#include "WebPlugin.h"
#include "Pickup.h"
#include "Root.h"
#include "Vector3f.h"
#include "Vector3d.h"
#include "Vector3i.h"
#include "Matrix4f.h"
#include "Cuboid.h"
#include "Tracer.h"
#include "Group.h"
#include "BlockArea.h"
#include "Generating/ChunkDesc.h"
#include "CraftingRecipes.h"
#include "UI/Window.h"
#include "LuaWindow.h"

/* function to release collected object via destructor */
#ifdef __cplusplus

static int tolua_collect_cItem (lua_State* tolua_S)
{
 cItem* self = (cItem*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_cFurnaceEntity (lua_State* tolua_S)
{
 cFurnaceEntity* self = (cFurnaceEntity*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_cChestEntity (lua_State* tolua_S)
{
 cChestEntity* self = (cChestEntity*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_cDispenserEntity (lua_State* tolua_S)
{
 cDispenserEntity* self = (cDispenserEntity*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_cCuboid (lua_State* tolua_S)
{
 cCuboid* self = (cCuboid*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_cBlockEntity (lua_State* tolua_S)
{
 cBlockEntity* self = (cBlockEntity*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_cBlockArea (lua_State* tolua_S)
{
 cBlockArea* self = (cBlockArea*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_cEnchantments (lua_State* tolua_S)
{
 cEnchantments* self = (cEnchantments*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_cLuaWindow (lua_State* tolua_S)
{
 cLuaWindow* self = (cLuaWindow*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_cDropperEntity (lua_State* tolua_S)
{
 cDropperEntity* self = (cDropperEntity*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_cPickup (lua_State* tolua_S)
{
 cPickup* self = (cPickup*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_sWebAdminPage (lua_State* tolua_S)
{
 sWebAdminPage* self = (sWebAdminPage*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_cCraftingGrid (lua_State* tolua_S)
{
 cCraftingGrid* self = (cCraftingGrid*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_cTracer (lua_State* tolua_S)
{
 cTracer* self = (cTracer*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_Vector3f (lua_State* tolua_S)
{
 Vector3f* self = (Vector3f*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_Vector3i (lua_State* tolua_S)
{
 Vector3i* self = (Vector3i*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_cIniFile (lua_State* tolua_S)
{
 cIniFile* self = (cIniFile*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_cItems (lua_State* tolua_S)
{
 cItems* self = (cItems*) tolua_tousertype(tolua_S,1,0);
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
 tolua_usertype(tolua_S,"cPlugin_NewLua");
 tolua_usertype(tolua_S,"cCraftingGrid");
 tolua_usertype(tolua_S,"cCraftingRecipe");
 tolua_usertype(tolua_S,"cPlugin");
 tolua_usertype(tolua_S,"cWindow");
 tolua_usertype(tolua_S,"cStringMap");
 tolua_usertype(tolua_S,"cItemGrid");
 tolua_usertype(tolua_S,"cBlockArea");
 tolua_usertype(tolua_S,"cEnchantments");
 tolua_usertype(tolua_S,"cLuaWindow");
 tolua_usertype(tolua_S,"cInventory");
 tolua_usertype(tolua_S,"cRoot");
 tolua_usertype(tolua_S,"cCuboid");
 tolua_usertype(tolua_S,"std::vector<cIniFile::key>");
 tolua_usertype(tolua_S,"cGroup");
 tolua_usertype(tolua_S,"cPickup");
 tolua_usertype(tolua_S,"std::vector<std::string>");
 tolua_usertype(tolua_S,"cTracer");
 tolua_usertype(tolua_S,"cClientHandle");
 tolua_usertype(tolua_S,"cChunkDesc");
 tolua_usertype(tolua_S,"cFurnaceRecipe");
 tolua_usertype(tolua_S,"Vector3i");
 tolua_usertype(tolua_S,"cChatColor");
 tolua_usertype(tolua_S,"cWorld");
 tolua_usertype(tolua_S,"Lua__cPickup");
 tolua_usertype(tolua_S,"Lua__cWebPlugin");
 tolua_usertype(tolua_S,"cCraftingRecipes");
 tolua_usertype(tolua_S,"cEntity");
 tolua_usertype(tolua_S,"cItem");
 tolua_usertype(tolua_S,"Vector3f");
 tolua_usertype(tolua_S,"cWebPlugin");
 tolua_usertype(tolua_S,"cDropSpenserEntity");
 tolua_usertype(tolua_S,"Lua__cPlayer");
 tolua_usertype(tolua_S,"cWebAdmin");
 tolua_usertype(tolua_S,"cChestEntity");
 tolua_usertype(tolua_S,"cDispenserEntity");
 tolua_usertype(tolua_S,"sWebAdminPage");
 tolua_usertype(tolua_S,"cBlockEntity");
 tolua_usertype(tolua_S,"cCriticalSection");
 tolua_usertype(tolua_S,"HTTPTemplateRequest");
 tolua_usertype(tolua_S,"HTTPRequest");
 tolua_usertype(tolua_S,"HTTPFormData");
 tolua_usertype(tolua_S,"cFurnaceEntity");
 tolua_usertype(tolua_S,"cDropperEntity");
 tolua_usertype(tolua_S,"cLineBlockTracer");
 tolua_usertype(tolua_S,"cPluginManager");
 tolua_usertype(tolua_S,"cIniFile");
 tolua_usertype(tolua_S,"cBlockEntityWithItems");
 tolua_usertype(tolua_S,"cListeners");
 tolua_usertype(tolua_S,"cPawn");
 tolua_usertype(tolua_S,"cPlayer");
 tolua_usertype(tolua_S,"cGroupManager");
 tolua_usertype(tolua_S,"cBlockEntityWindowOwner");
 tolua_usertype(tolua_S,"cItemGrid::cListener");
 tolua_usertype(tolua_S,"cServer");
 tolua_usertype(tolua_S,"cItems");
 tolua_usertype(tolua_S,"Lua__cEntity");
 tolua_usertype(tolua_S,"Vector3d");
}

/* method: new of class  cIniFile */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cIniFile_new00
static int tolua_AllToLua_cIniFile_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"cIniFile",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   cIniFile* tolua_ret = (cIniFile*)  Mtolua_new((cIniFile)());
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
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   cIniFile* tolua_ret = (cIniFile*)  Mtolua_new((cIniFile)());
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

/* method: new of class  cIniFile */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cIniFile_new01
static int tolua_AllToLua_cIniFile_new01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"cIniFile",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  const std::string a_Path = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
  {
   cIniFile* tolua_ret = (cIniFile*)  Mtolua_new((cIniFile)(a_Path));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"cIniFile");
   tolua_pushcppstring(tolua_S,(const char*)a_Path);
  }
 }
 return 2;
tolua_lerror:
 return tolua_AllToLua_cIniFile_new00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  cIniFile */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cIniFile_new01_local
static int tolua_AllToLua_cIniFile_new01_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"cIniFile",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  const std::string a_Path = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
  {
   cIniFile* tolua_ret = (cIniFile*)  Mtolua_new((cIniFile)(a_Path));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"cIniFile");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
   tolua_pushcppstring(tolua_S,(const char*)a_Path);
  }
 }
 return 2;
tolua_lerror:
 return tolua_AllToLua_cIniFile_new00_local(tolua_S);
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
   const std::string tolua_ret = (const std::string)  self->Path();
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
     !tolua_isboolean(tolua_S,2,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cIniFile* self = (cIniFile*)  tolua_tousertype(tolua_S,1,0);
  bool a_AllowExampleRedirect = ((bool)  tolua_toboolean(tolua_S,2,true));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'ReadFile'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->ReadFile(a_AllowExampleRedirect);
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
     !tolua_isusertype(tolua_S,1,"const cIniFile",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cIniFile* self = (const cIniFile*)  tolua_tousertype(tolua_S,1,0);
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
  const AString keyname = ((const AString)  tolua_tocppstring(tolua_S,2,0));
  const AString valuename = ((const AString)  tolua_tocppstring(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetValue'", NULL);
#endif
  {
   AString tolua_ret = (AString)  self->GetValue(keyname,valuename);
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
  const AString keyname = ((const AString)  tolua_tocppstring(tolua_S,2,0));
  const AString valuename = ((const AString)  tolua_tocppstring(tolua_S,3,0));
  const AString defValue = ((const AString)  tolua_tocppstring(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetValue'", NULL);
#endif
  {
   AString tolua_ret = (AString)  self->GetValue(keyname,valuename,defValue);
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
   AString tolua_ret = (AString)  self->GetValue(keyID,valueID);
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
  const AString defValue = ((const AString)  tolua_tocppstring(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetValue'", NULL);
#endif
  {
   AString tolua_ret = (AString)  self->GetValue(keyID,valueID,defValue);
   tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
   tolua_pushcppstring(tolua_S,(const char*)defValue);
  }
 }
 return 2;
tolua_lerror:
 return tolua_AllToLua_cIniFile_GetValue02(tolua_S);
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
  const AString keyname = ((const AString)  tolua_tocppstring(tolua_S,2,0));
  const AString valuename = ((const AString)  tolua_tocppstring(tolua_S,3,0));
  const double defValue = ((const double)  tolua_tonumber(tolua_S,4,0));
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
  const AString keyname = ((const AString)  tolua_tocppstring(tolua_S,2,0));
  const AString valuename = ((const AString)  tolua_tocppstring(tolua_S,3,0));
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
  const AString keyname = ((const AString)  tolua_tocppstring(tolua_S,2,0));
  const AString valuename = ((const AString)  tolua_tocppstring(tolua_S,3,0));
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

/* method: GetValueSet of class  cIniFile */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cIniFile_GetValueSet00
static int tolua_AllToLua_cIniFile_GetValueSet00(lua_State* tolua_S)
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
  const AString keyname = ((const AString)  tolua_tocppstring(tolua_S,2,0));
  const AString valuename = ((const AString)  tolua_tocppstring(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetValueSet'", NULL);
#endif
  {
   AString tolua_ret = (AString)  self->GetValueSet(keyname,valuename);
   tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
   tolua_pushcppstring(tolua_S,(const char*)keyname);
   tolua_pushcppstring(tolua_S,(const char*)valuename);
  }
 }
 return 3;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetValueSet'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetValueSet of class  cIniFile */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cIniFile_GetValueSet01
static int tolua_AllToLua_cIniFile_GetValueSet01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cIniFile",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,3,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  cIniFile* self = (cIniFile*)  tolua_tousertype(tolua_S,1,0);
  const AString keyname = ((const AString)  tolua_tocppstring(tolua_S,2,0));
  const AString valuename = ((const AString)  tolua_tocppstring(tolua_S,3,0));
  const AString defValue = ((const AString)  tolua_tocppstring(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetValueSet'", NULL);
#endif
  {
   AString tolua_ret = (AString)  self->GetValueSet(keyname,valuename,defValue);
   tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
   tolua_pushcppstring(tolua_S,(const char*)keyname);
   tolua_pushcppstring(tolua_S,(const char*)valuename);
   tolua_pushcppstring(tolua_S,(const char*)defValue);
  }
 }
 return 4;
tolua_lerror:
 return tolua_AllToLua_cIniFile_GetValueSet00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetValueSetF of class  cIniFile */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cIniFile_GetValueSetF00
static int tolua_AllToLua_cIniFile_GetValueSetF00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cIniFile",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cIniFile* self = (cIniFile*)  tolua_tousertype(tolua_S,1,0);
  const AString keyname = ((const AString)  tolua_tocppstring(tolua_S,2,0));
  const AString valuename = ((const AString)  tolua_tocppstring(tolua_S,3,0));
  const double defValue = ((const double)  tolua_tonumber(tolua_S,4,0.0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetValueSetF'", NULL);
#endif
  {
   double tolua_ret = (double)  self->GetValueSetF(keyname,valuename,defValue);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
   tolua_pushcppstring(tolua_S,(const char*)keyname);
   tolua_pushcppstring(tolua_S,(const char*)valuename);
  }
 }
 return 3;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetValueSetF'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetValueSetI of class  cIniFile */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cIniFile_GetValueSetI00
static int tolua_AllToLua_cIniFile_GetValueSetI00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cIniFile",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cIniFile* self = (cIniFile*)  tolua_tousertype(tolua_S,1,0);
  const AString keyname = ((const AString)  tolua_tocppstring(tolua_S,2,0));
  const AString valuename = ((const AString)  tolua_tocppstring(tolua_S,3,0));
  const int defValue = ((const int)  tolua_tonumber(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetValueSetI'", NULL);
#endif
  {
   int tolua_ret = (int)  self->GetValueSetI(keyname,valuename,defValue);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
   tolua_pushcppstring(tolua_S,(const char*)keyname);
   tolua_pushcppstring(tolua_S,(const char*)valuename);
  }
 }
 return 3;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetValueSetI'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetValueSetB of class  cIniFile */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cIniFile_GetValueSetB00
static int tolua_AllToLua_cIniFile_GetValueSetB00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cIniFile",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isboolean(tolua_S,4,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cIniFile* self = (cIniFile*)  tolua_tousertype(tolua_S,1,0);
  const AString keyname = ((const AString)  tolua_tocppstring(tolua_S,2,0));
  const AString valuename = ((const AString)  tolua_tocppstring(tolua_S,3,0));
  const bool defValue = ((const bool)  tolua_toboolean(tolua_S,4,false));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetValueSetB'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->GetValueSetB(keyname,valuename,defValue);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
   tolua_pushcppstring(tolua_S,(const char*)keyname);
   tolua_pushcppstring(tolua_S,(const char*)valuename);
  }
 }
 return 3;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetValueSetB'.",&tolua_err);
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

/* method: DeleteValueByID of class  cIniFile */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cIniFile_DeleteValueByID00
static int tolua_AllToLua_cIniFile_DeleteValueByID00(lua_State* tolua_S)
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
  const unsigned valueID = ((const unsigned)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'DeleteValueByID'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->DeleteValueByID(keyID,valueID);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'DeleteValueByID'.",&tolua_err);
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

/* function: BlockStringToType */
#ifndef TOLUA_DISABLE_tolua_AllToLua_BlockStringToType00
static int tolua_AllToLua_BlockStringToType00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_iscppstring(tolua_S,1,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const AString a_BlockTypeString = ((const AString)  tolua_tocppstring(tolua_S,1,0));
  {
   unsigned char tolua_ret = ( unsigned char)  BlockStringToType(a_BlockTypeString);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
   tolua_pushcppstring(tolua_S,(const char*)a_BlockTypeString);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'BlockStringToType'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: StringToItem */
#ifndef TOLUA_DISABLE_tolua_AllToLua_StringToItem00
static int tolua_AllToLua_StringToItem00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_iscppstring(tolua_S,1,0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"cItem",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const AString a_ItemTypeString = ((const AString)  tolua_tocppstring(tolua_S,1,0));
  cItem* a_Item = ((cItem*)  tolua_tousertype(tolua_S,2,0));
  {
   bool tolua_ret = (bool)  StringToItem(a_ItemTypeString,*a_Item);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
   tolua_pushcppstring(tolua_S,(const char*)a_ItemTypeString);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'StringToItem'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: ItemToString */
#ifndef TOLUA_DISABLE_tolua_AllToLua_ItemToString00
static int tolua_AllToLua_ItemToString00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     (tolua_isvaluenil(tolua_S,1,&tolua_err) || !tolua_isusertype(tolua_S,1,"const cItem",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cItem* a_Item = ((const cItem*)  tolua_tousertype(tolua_S,1,0));
  {
   AString tolua_ret = (AString)  ItemToString(*a_Item);
   tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'ItemToString'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: ItemTypeToString */
#ifndef TOLUA_DISABLE_tolua_AllToLua_ItemTypeToString00
static int tolua_AllToLua_ItemTypeToString00(lua_State* tolua_S)
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
  short a_ItemType = ((short)  tolua_tonumber(tolua_S,1,0));
  {
   AString tolua_ret = (AString)  ItemTypeToString(a_ItemType);
   tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'ItemTypeToString'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: ItemToFullString */
#ifndef TOLUA_DISABLE_tolua_AllToLua_ItemToFullString00
static int tolua_AllToLua_ItemToFullString00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     (tolua_isvaluenil(tolua_S,1,&tolua_err) || !tolua_isusertype(tolua_S,1,"const cItem",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cItem* a_Item = ((const cItem*)  tolua_tousertype(tolua_S,1,0));
  {
   AString tolua_ret = (AString)  ItemToFullString(*a_Item);
   tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'ItemToFullString'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: StringToBiome */
#ifndef TOLUA_DISABLE_tolua_AllToLua_StringToBiome00
static int tolua_AllToLua_StringToBiome00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_iscppstring(tolua_S,1,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const AString a_BiomeString = ((const AString)  tolua_tocppstring(tolua_S,1,0));
  {
   EMCSBiome tolua_ret = (EMCSBiome)  StringToBiome(a_BiomeString);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
   tolua_pushcppstring(tolua_S,(const char*)a_BiomeString);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'StringToBiome'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: StringToMobType */
#ifndef TOLUA_DISABLE_tolua_AllToLua_StringToMobType00
static int tolua_AllToLua_StringToMobType00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_iscppstring(tolua_S,1,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const AString a_MobString = ((const AString)  tolua_tocppstring(tolua_S,1,0));
  {
   int tolua_ret = (int)  StringToMobType(a_MobString);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
   tolua_pushcppstring(tolua_S,(const char*)a_MobString);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'StringToMobType'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: StringToDimension */
#ifndef TOLUA_DISABLE_tolua_AllToLua_StringToDimension00
static int tolua_AllToLua_StringToDimension00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_iscppstring(tolua_S,1,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const AString a_DimensionString = ((const AString)  tolua_tocppstring(tolua_S,1,0));
  {
   eDimension tolua_ret = (eDimension)  StringToDimension(a_DimensionString);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
   tolua_pushcppstring(tolua_S,(const char*)a_DimensionString);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'StringToDimension'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: DamageTypeToString */
#ifndef TOLUA_DISABLE_tolua_AllToLua_DamageTypeToString00
static int tolua_AllToLua_DamageTypeToString00(lua_State* tolua_S)
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
  eDamageType a_DamageType = ((eDamageType) (int)  tolua_tonumber(tolua_S,1,0));
  {
   AString tolua_ret = (AString)  DamageTypeToString(a_DamageType);
   tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'DamageTypeToString'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: StringToDamageType */
#ifndef TOLUA_DISABLE_tolua_AllToLua_StringToDamageType00
static int tolua_AllToLua_StringToDamageType00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_iscppstring(tolua_S,1,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const AString a_DamageString = ((const AString)  tolua_tocppstring(tolua_S,1,0));
  {
   eDamageType tolua_ret = (eDamageType)  StringToDamageType(a_DamageString);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
   tolua_pushcppstring(tolua_S,(const char*)a_DamageString);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'StringToDamageType'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: TrimString */
#ifndef TOLUA_DISABLE_tolua_AllToLua_TrimString00
static int tolua_AllToLua_TrimString00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_iscppstring(tolua_S,1,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const AString str = ((const AString)  tolua_tocppstring(tolua_S,1,0));
  {
   AString tolua_ret = (AString)  TrimString(str);
   tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
   tolua_pushcppstring(tolua_S,(const char*)str);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'TrimString'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: NoCaseCompare */
#ifndef TOLUA_DISABLE_tolua_AllToLua_NoCaseCompare00
static int tolua_AllToLua_NoCaseCompare00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_iscppstring(tolua_S,1,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const AString s1 = ((const AString)  tolua_tocppstring(tolua_S,1,0));
  const AString s2 = ((const AString)  tolua_tocppstring(tolua_S,2,0));
  {
   int tolua_ret = (int)  NoCaseCompare(s1,s2);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
   tolua_pushcppstring(tolua_S,(const char*)s1);
   tolua_pushcppstring(tolua_S,(const char*)s2);
  }
 }
 return 3;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'NoCaseCompare'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: ReplaceString */
#ifndef TOLUA_DISABLE_tolua_AllToLua_ReplaceString00
static int tolua_AllToLua_ReplaceString00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_iscppstring(tolua_S,1,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  AString iHayStack = ((AString)  tolua_tocppstring(tolua_S,1,0));
  const AString iNeedle = ((const AString)  tolua_tocppstring(tolua_S,2,0));
  const AString iReplaceWith = ((const AString)  tolua_tocppstring(tolua_S,3,0));
  {
   ReplaceString(iHayStack,iNeedle,iReplaceWith);
   tolua_pushcppstring(tolua_S,(const char*)iHayStack);
   tolua_pushcppstring(tolua_S,(const char*)iNeedle);
   tolua_pushcppstring(tolua_S,(const char*)iReplaceWith);
  }
 }
 return 3;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'ReplaceString'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: EscapeString */
#ifndef TOLUA_DISABLE_tolua_AllToLua_EscapeString00
static int tolua_AllToLua_EscapeString00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_iscppstring(tolua_S,1,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const AString a_Message = ((const AString)  tolua_tocppstring(tolua_S,1,0));
  {
   AString tolua_ret = (AString)  EscapeString(a_Message);
   tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
   tolua_pushcppstring(tolua_S,(const char*)a_Message);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'EscapeString'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: StripColorCodes */
#ifndef TOLUA_DISABLE_tolua_AllToLua_StripColorCodes00
static int tolua_AllToLua_StripColorCodes00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_iscppstring(tolua_S,1,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const AString a_Message = ((const AString)  tolua_tocppstring(tolua_S,1,0));
  {
   AString tolua_ret = (AString)  StripColorCodes(a_Message);
   tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
   tolua_pushcppstring(tolua_S,(const char*)a_Message);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'StripColorCodes'.",&tolua_err);
 return 0;
#endif
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
  g_BlockLightValue[tolua_index] = ((unsigned char)  tolua_tonumber(tolua_S,3,0));
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
  g_BlockSpreadLightFalloff[tolua_index] = ((unsigned char)  tolua_tonumber(tolua_S,3,0));
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

/* function: ClickActionToString */
#ifndef TOLUA_DISABLE_tolua_AllToLua_ClickActionToString00
static int tolua_AllToLua_ClickActionToString00(lua_State* tolua_S)
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
  eClickAction a_ClickAction = ((eClickAction) (int)  tolua_tonumber(tolua_S,1,0));
  {
   const char* tolua_ret = (const char*)  ClickActionToString(a_ClickAction);
   tolua_pushstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'ClickActionToString'.",&tolua_err);
 return 0;
#endif
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
  int a_BlockType = ((int)  tolua_tonumber(tolua_S,1,0));
  {
   bool tolua_ret = (bool)  IsValidBlock(a_BlockType);
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

/* function: IsValidItem */
#ifndef TOLUA_DISABLE_tolua_AllToLua_IsValidItem00
static int tolua_AllToLua_IsValidItem00(lua_State* tolua_S)
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
  int a_ItemType = ((int)  tolua_tonumber(tolua_S,1,0));
  {
   bool tolua_ret = (bool)  IsValidItem(a_ItemType);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'IsValidItem'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: AddFaceDirection */
#ifndef TOLUA_DISABLE_tolua_AllToLua_AddFaceDirection00
static int tolua_AllToLua_AddFaceDirection00(lua_State* tolua_S)
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
  int a_BlockX = ((int)  tolua_tonumber(tolua_S,1,0));
  int a_BlockY = ((int)  tolua_tonumber(tolua_S,2,0));
  int a_BlockZ = ((int)  tolua_tonumber(tolua_S,3,0));
  char a_BlockFace = ((char)  tolua_tonumber(tolua_S,4,0));
  bool a_bInverse = ((bool)  tolua_toboolean(tolua_S,5,false));
  {
   AddFaceDirection(a_BlockX,a_BlockY,a_BlockZ,a_BlockFace,a_bInverse);
   tolua_pushnumber(tolua_S,(lua_Number)a_BlockX);
   tolua_pushnumber(tolua_S,(lua_Number)a_BlockY);
   tolua_pushnumber(tolua_S,(lua_Number)a_BlockZ);
  }
 }
 return 3;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'AddFaceDirection'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: ItemCategory::IsPickaxe */
#ifndef TOLUA_DISABLE_tolua_AllToLua_ItemCategory_IsPickaxe00
static int tolua_AllToLua_ItemCategory_IsPickaxe00(lua_State* tolua_S)
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
  short a_ItemID = ((short)  tolua_tonumber(tolua_S,1,0));
  {
   bool tolua_ret = (bool)  ItemCategory::IsPickaxe(a_ItemID);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'IsPickaxe'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: ItemCategory::IsAxe */
#ifndef TOLUA_DISABLE_tolua_AllToLua_ItemCategory_IsAxe00
static int tolua_AllToLua_ItemCategory_IsAxe00(lua_State* tolua_S)
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
  short a_ItemID = ((short)  tolua_tonumber(tolua_S,1,0));
  {
   bool tolua_ret = (bool)  ItemCategory::IsAxe(a_ItemID);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'IsAxe'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: ItemCategory::IsSword */
#ifndef TOLUA_DISABLE_tolua_AllToLua_ItemCategory_IsSword00
static int tolua_AllToLua_ItemCategory_IsSword00(lua_State* tolua_S)
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
  short a_ItemID = ((short)  tolua_tonumber(tolua_S,1,0));
  {
   bool tolua_ret = (bool)  ItemCategory::IsSword(a_ItemID);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'IsSword'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: ItemCategory::IsHoe */
#ifndef TOLUA_DISABLE_tolua_AllToLua_ItemCategory_IsHoe00
static int tolua_AllToLua_ItemCategory_IsHoe00(lua_State* tolua_S)
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
  short a_ItemID = ((short)  tolua_tonumber(tolua_S,1,0));
  {
   bool tolua_ret = (bool)  ItemCategory::IsHoe(a_ItemID);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'IsHoe'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: ItemCategory::IsShovel */
#ifndef TOLUA_DISABLE_tolua_AllToLua_ItemCategory_IsShovel00
static int tolua_AllToLua_ItemCategory_IsShovel00(lua_State* tolua_S)
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
  short a_ItemID = ((short)  tolua_tonumber(tolua_S,1,0));
  {
   bool tolua_ret = (bool)  ItemCategory::IsShovel(a_ItemID);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'IsShovel'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: ItemCategory::IsTool */
#ifndef TOLUA_DISABLE_tolua_AllToLua_ItemCategory_IsTool00
static int tolua_AllToLua_ItemCategory_IsTool00(lua_State* tolua_S)
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
  short a_ItemID = ((short)  tolua_tonumber(tolua_S,1,0));
  {
   bool tolua_ret = (bool)  ItemCategory::IsTool(a_ItemID);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'IsTool'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: ItemCategory::IsHelmet */
#ifndef TOLUA_DISABLE_tolua_AllToLua_ItemCategory_IsHelmet00
static int tolua_AllToLua_ItemCategory_IsHelmet00(lua_State* tolua_S)
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
  short a_ItemType = ((short)  tolua_tonumber(tolua_S,1,0));
  {
   bool tolua_ret = (bool)  ItemCategory::IsHelmet(a_ItemType);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'IsHelmet'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: ItemCategory::IsChestPlate */
#ifndef TOLUA_DISABLE_tolua_AllToLua_ItemCategory_IsChestPlate00
static int tolua_AllToLua_ItemCategory_IsChestPlate00(lua_State* tolua_S)
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
  short a_ItemType = ((short)  tolua_tonumber(tolua_S,1,0));
  {
   bool tolua_ret = (bool)  ItemCategory::IsChestPlate(a_ItemType);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'IsChestPlate'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: ItemCategory::IsLeggings */
#ifndef TOLUA_DISABLE_tolua_AllToLua_ItemCategory_IsLeggings00
static int tolua_AllToLua_ItemCategory_IsLeggings00(lua_State* tolua_S)
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
  short a_ItemType = ((short)  tolua_tonumber(tolua_S,1,0));
  {
   bool tolua_ret = (bool)  ItemCategory::IsLeggings(a_ItemType);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'IsLeggings'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: ItemCategory::IsBoots */
#ifndef TOLUA_DISABLE_tolua_AllToLua_ItemCategory_IsBoots00
static int tolua_AllToLua_ItemCategory_IsBoots00(lua_State* tolua_S)
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
  short a_ItemType = ((short)  tolua_tonumber(tolua_S,1,0));
  {
   bool tolua_ret = (bool)  ItemCategory::IsBoots(a_ItemType);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'IsBoots'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: ItemCategory::IsArmor */
#ifndef TOLUA_DISABLE_tolua_AllToLua_ItemCategory_IsArmor00
static int tolua_AllToLua_ItemCategory_IsArmor00(lua_State* tolua_S)
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
  short a_ItemType = ((short)  tolua_tonumber(tolua_S,1,0));
  {
   bool tolua_ret = (bool)  ItemCategory::IsArmor(a_ItemType);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'IsArmor'.",&tolua_err);
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

/* get function: Delimiter of class  cChatColor */
#ifndef TOLUA_DISABLE_tolua_get_cChatColor_Delimiter
static int tolua_get_cChatColor_Delimiter(lua_State* tolua_S)
{
  tolua_pushcppstring(tolua_S,(const char*)cChatColor::Delimiter);
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

/* get function: Random of class  cChatColor */
#ifndef TOLUA_DISABLE_tolua_get_cChatColor_Random
static int tolua_get_cChatColor_Random(lua_State* tolua_S)
{
  tolua_pushcppstring(tolua_S,(const char*)cChatColor::Random);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: Bold of class  cChatColor */
#ifndef TOLUA_DISABLE_tolua_get_cChatColor_Bold
static int tolua_get_cChatColor_Bold(lua_State* tolua_S)
{
  tolua_pushcppstring(tolua_S,(const char*)cChatColor::Bold);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: Strikethrough of class  cChatColor */
#ifndef TOLUA_DISABLE_tolua_get_cChatColor_Strikethrough
static int tolua_get_cChatColor_Strikethrough(lua_State* tolua_S)
{
  tolua_pushcppstring(tolua_S,(const char*)cChatColor::Strikethrough);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: Underlined of class  cChatColor */
#ifndef TOLUA_DISABLE_tolua_get_cChatColor_Underlined
static int tolua_get_cChatColor_Underlined(lua_State* tolua_S)
{
  tolua_pushcppstring(tolua_S,(const char*)cChatColor::Underlined);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: Italic of class  cChatColor */
#ifndef TOLUA_DISABLE_tolua_get_cChatColor_Italic
static int tolua_get_cChatColor_Italic(lua_State* tolua_S)
{
  tolua_pushcppstring(tolua_S,(const char*)cChatColor::Italic);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: Plain of class  cChatColor */
#ifndef TOLUA_DISABLE_tolua_get_cChatColor_Plain
static int tolua_get_cChatColor_Plain(lua_State* tolua_S)
{
  tolua_pushcppstring(tolua_S,(const char*)cChatColor::Plain);
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
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cClientHandle* self = (cClientHandle*)  tolua_tousertype(tolua_S,1,0);
  const AString a_Reason = ((const AString)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'Kick'", NULL);
#endif
  {
   self->Kick(a_Reason);
   tolua_pushcppstring(tolua_S,(const char*)a_Reason);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Kick'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SendBlockChange of class  cClientHandle */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cClientHandle_SendBlockChange00
static int tolua_AllToLua_cClientHandle_SendBlockChange00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cClientHandle",0,&tolua_err) ||
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
  cClientHandle* self = (cClientHandle*)  tolua_tousertype(tolua_S,1,0);
  int a_BlockX = ((int)  tolua_tonumber(tolua_S,2,0));
  int a_BlockY = ((int)  tolua_tonumber(tolua_S,3,0));
  int a_BlockZ = ((int)  tolua_tonumber(tolua_S,4,0));
  unsigned char a_BlockType = (( unsigned char)  tolua_tonumber(tolua_S,5,0));
  unsigned char a_BlockMeta = (( unsigned char)  tolua_tonumber(tolua_S,6,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SendBlockChange'", NULL);
#endif
  {
   self->SendBlockChange(a_BlockX,a_BlockY,a_BlockZ,a_BlockType,a_BlockMeta);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SendBlockChange'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetUsername of class  cClientHandle */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cClientHandle_GetUsername00
static int tolua_AllToLua_cClientHandle_GetUsername00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cClientHandle",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cClientHandle* self = (const cClientHandle*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetUsername'", NULL);
#endif
  {
   const AString tolua_ret = (const AString)  self->GetUsername();
   tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetUsername'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetUsername of class  cClientHandle */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cClientHandle_SetUsername00
static int tolua_AllToLua_cClientHandle_SetUsername00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cClientHandle",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cClientHandle* self = (cClientHandle*)  tolua_tousertype(tolua_S,1,0);
  const AString a_Username = ((const AString)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetUsername'", NULL);
#endif
  {
   self->SetUsername(a_Username);
   tolua_pushcppstring(tolua_S,(const char*)a_Username);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetUsername'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetPing of class  cClientHandle */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cClientHandle_GetPing00
static int tolua_AllToLua_cClientHandle_GetPing00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cClientHandle",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cClientHandle* self = (const cClientHandle*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetPing'", NULL);
#endif
  {
   short tolua_ret = (short)  self->GetPing();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetPing'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetViewDistance of class  cClientHandle */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cClientHandle_SetViewDistance00
static int tolua_AllToLua_cClientHandle_SetViewDistance00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cClientHandle",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cClientHandle* self = (cClientHandle*)  tolua_tousertype(tolua_S,1,0);
  int a_ViewDistance = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetViewDistance'", NULL);
#endif
  {
   self->SetViewDistance(a_ViewDistance);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetViewDistance'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetViewDistance of class  cClientHandle */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cClientHandle_GetViewDistance00
static int tolua_AllToLua_cClientHandle_GetViewDistance00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cClientHandle",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cClientHandle* self = (const cClientHandle*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetViewDistance'", NULL);
#endif
  {
   int tolua_ret = (int)  self->GetViewDistance();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetViewDistance'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetUniqueID of class  cClientHandle */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cClientHandle_GetUniqueID00
static int tolua_AllToLua_cClientHandle_GetUniqueID00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cClientHandle",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cClientHandle* self = (const cClientHandle*)  tolua_tousertype(tolua_S,1,0);
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

/* get function: DamageType of class  TakeDamageInfo */
#ifndef TOLUA_DISABLE_tolua_get_TakeDamageInfo_DamageType
static int tolua_get_TakeDamageInfo_DamageType(lua_State* tolua_S)
{
  TakeDamageInfo* self = (TakeDamageInfo*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'DamageType'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->DamageType);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: DamageType of class  TakeDamageInfo */
#ifndef TOLUA_DISABLE_tolua_set_TakeDamageInfo_DamageType
static int tolua_set_TakeDamageInfo_DamageType(lua_State* tolua_S)
{
  TakeDamageInfo* self = (TakeDamageInfo*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'DamageType'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->DamageType = ((eDamageType) (int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: Attacker of class  TakeDamageInfo */
#ifndef TOLUA_DISABLE_tolua_get_TakeDamageInfo_Attacker_ptr
static int tolua_get_TakeDamageInfo_Attacker_ptr(lua_State* tolua_S)
{
  TakeDamageInfo* self = (TakeDamageInfo*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'Attacker'",NULL);
#endif
   tolua_pushusertype(tolua_S,(void*)self->Attacker,"cEntity");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: Attacker of class  TakeDamageInfo */
#ifndef TOLUA_DISABLE_tolua_set_TakeDamageInfo_Attacker_ptr
static int tolua_set_TakeDamageInfo_Attacker_ptr(lua_State* tolua_S)
{
  TakeDamageInfo* self = (TakeDamageInfo*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'Attacker'",NULL);
  if (!tolua_isusertype(tolua_S,2,"cEntity",0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->Attacker = ((cEntity*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: RawDamage of class  TakeDamageInfo */
#ifndef TOLUA_DISABLE_tolua_get_TakeDamageInfo_RawDamage
static int tolua_get_TakeDamageInfo_RawDamage(lua_State* tolua_S)
{
  TakeDamageInfo* self = (TakeDamageInfo*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'RawDamage'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->RawDamage);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: RawDamage of class  TakeDamageInfo */
#ifndef TOLUA_DISABLE_tolua_set_TakeDamageInfo_RawDamage
static int tolua_set_TakeDamageInfo_RawDamage(lua_State* tolua_S)
{
  TakeDamageInfo* self = (TakeDamageInfo*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'RawDamage'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->RawDamage = ((int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: FinalDamage of class  TakeDamageInfo */
#ifndef TOLUA_DISABLE_tolua_get_TakeDamageInfo_FinalDamage
static int tolua_get_TakeDamageInfo_FinalDamage(lua_State* tolua_S)
{
  TakeDamageInfo* self = (TakeDamageInfo*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'FinalDamage'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->FinalDamage);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: FinalDamage of class  TakeDamageInfo */
#ifndef TOLUA_DISABLE_tolua_set_TakeDamageInfo_FinalDamage
static int tolua_set_TakeDamageInfo_FinalDamage(lua_State* tolua_S)
{
  TakeDamageInfo* self = (TakeDamageInfo*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'FinalDamage'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->FinalDamage = ((int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: Knockback of class  TakeDamageInfo */
#ifndef TOLUA_DISABLE_tolua_get_TakeDamageInfo_Knockback
static int tolua_get_TakeDamageInfo_Knockback(lua_State* tolua_S)
{
  TakeDamageInfo* self = (TakeDamageInfo*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'Knockback'",NULL);
#endif
   tolua_pushusertype(tolua_S,(void*)&self->Knockback,"Vector3d");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: Knockback of class  TakeDamageInfo */
#ifndef TOLUA_DISABLE_tolua_set_TakeDamageInfo_Knockback
static int tolua_set_TakeDamageInfo_Knockback(lua_State* tolua_S)
{
  TakeDamageInfo* self = (TakeDamageInfo*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'Knockback'",NULL);
  if ((tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"Vector3d",0,&tolua_err)))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->Knockback = *((Vector3d*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetEntityType of class  cEntity */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cEntity_GetEntityType00
static int tolua_AllToLua_cEntity_GetEntityType00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cEntity",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cEntity* self = (const cEntity*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetEntityType'", NULL);
#endif
  {
   cEntity::eEntityType tolua_ret = (cEntity::eEntityType)  self->GetEntityType();
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

/* method: IsPlayer of class  cEntity */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cEntity_IsPlayer00
static int tolua_AllToLua_cEntity_IsPlayer00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cEntity",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cEntity* self = (const cEntity*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'IsPlayer'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->IsPlayer();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'IsPlayer'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: IsPickup of class  cEntity */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cEntity_IsPickup00
static int tolua_AllToLua_cEntity_IsPickup00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cEntity",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cEntity* self = (const cEntity*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'IsPickup'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->IsPickup();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'IsPickup'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: IsMob of class  cEntity */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cEntity_IsMob00
static int tolua_AllToLua_cEntity_IsMob00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cEntity",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cEntity* self = (const cEntity*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'IsMob'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->IsMob();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'IsMob'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: IsMinecart of class  cEntity */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cEntity_IsMinecart00
static int tolua_AllToLua_cEntity_IsMinecart00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cEntity",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cEntity* self = (const cEntity*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'IsMinecart'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->IsMinecart();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'IsMinecart'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: IsTNT of class  cEntity */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cEntity_IsTNT00
static int tolua_AllToLua_cEntity_IsTNT00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cEntity",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cEntity* self = (const cEntity*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'IsTNT'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->IsTNT();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'IsTNT'.",&tolua_err);
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
     !tolua_isusertype(tolua_S,1,"const cEntity",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cEntity* self = (const cEntity*)  tolua_tousertype(tolua_S,1,0);
  const char* a_ClassName = ((const char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'IsA'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->IsA(a_ClassName);
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
     !tolua_isusertype(tolua_S,1,"const cEntity",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cEntity* self = (const cEntity*)  tolua_tousertype(tolua_S,1,0);
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

/* method: GetClassStatic of class  cEntity */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cEntity_GetClassStatic00
static int tolua_AllToLua_cEntity_GetClassStatic00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"cEntity",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   const char* tolua_ret = (const char*)  cEntity::GetClassStatic();
   tolua_pushstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetClassStatic'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetParentClass of class  cEntity */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cEntity_GetParentClass00
static int tolua_AllToLua_cEntity_GetParentClass00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cEntity",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cEntity* self = (const cEntity*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetParentClass'", NULL);
#endif
  {
   const char* tolua_ret = (const char*)  self->GetParentClass();
   tolua_pushstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetParentClass'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetWorld of class  cEntity */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cEntity_GetWorld00
static int tolua_AllToLua_cEntity_GetWorld00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cEntity",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cEntity* self = (const cEntity*)  tolua_tousertype(tolua_S,1,0);
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

/* method: GetHeadYaw of class  cEntity */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cEntity_GetHeadYaw00
static int tolua_AllToLua_cEntity_GetHeadYaw00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cEntity",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cEntity* self = (const cEntity*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetHeadYaw'", NULL);
#endif
  {
   double tolua_ret = (double)  self->GetHeadYaw();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetHeadYaw'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetHeight of class  cEntity */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cEntity_GetHeight00
static int tolua_AllToLua_cEntity_GetHeight00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cEntity",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cEntity* self = (const cEntity*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetHeight'", NULL);
#endif
  {
   double tolua_ret = (double)  self->GetHeight();
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

/* method: GetMass of class  cEntity */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cEntity_GetMass00
static int tolua_AllToLua_cEntity_GetMass00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cEntity",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cEntity* self = (const cEntity*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetMass'", NULL);
#endif
  {
   double tolua_ret = (double)  self->GetMass();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetMass'.",&tolua_err);
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
     !tolua_isusertype(tolua_S,1,"const cEntity",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cEntity* self = (const cEntity*)  tolua_tousertype(tolua_S,1,0);
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
     !tolua_isusertype(tolua_S,1,"const cEntity",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cEntity* self = (const cEntity*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetPosX'", NULL);
#endif
  {
   double tolua_ret = (double)  self->GetPosX();
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
     !tolua_isusertype(tolua_S,1,"const cEntity",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cEntity* self = (const cEntity*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetPosY'", NULL);
#endif
  {
   double tolua_ret = (double)  self->GetPosY();
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
     !tolua_isusertype(tolua_S,1,"const cEntity",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cEntity* self = (const cEntity*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetPosZ'", NULL);
#endif
  {
   double tolua_ret = (double)  self->GetPosZ();
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
     !tolua_isusertype(tolua_S,1,"const cEntity",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cEntity* self = (const cEntity*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetRot'", NULL);
#endif
  {
   const Vector3d& tolua_ret = (const Vector3d&)  self->GetRot();
    tolua_pushusertype(tolua_S,(void*)&tolua_ret,"const Vector3d");
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
     !tolua_isusertype(tolua_S,1,"const cEntity",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cEntity* self = (const cEntity*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetRotation'", NULL);
#endif
  {
   double tolua_ret = (double)  self->GetRotation();
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
     !tolua_isusertype(tolua_S,1,"const cEntity",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cEntity* self = (const cEntity*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetPitch'", NULL);
#endif
  {
   double tolua_ret = (double)  self->GetPitch();
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
     !tolua_isusertype(tolua_S,1,"const cEntity",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cEntity* self = (const cEntity*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetRoll'", NULL);
#endif
  {
   double tolua_ret = (double)  self->GetRoll();
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
     !tolua_isusertype(tolua_S,1,"const cEntity",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cEntity* self = (const cEntity*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetLookVector'", NULL);
#endif
  {
   Vector3d tolua_ret = (Vector3d)  self->GetLookVector();
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
 tolua_error(tolua_S,"#ferror in function 'GetLookVector'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetSpeed of class  cEntity */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cEntity_GetSpeed00
static int tolua_AllToLua_cEntity_GetSpeed00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cEntity",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cEntity* self = (const cEntity*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetSpeed'", NULL);
#endif
  {
   const Vector3d& tolua_ret = (const Vector3d&)  self->GetSpeed();
    tolua_pushusertype(tolua_S,(void*)&tolua_ret,"const Vector3d");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetSpeed'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetSpeedX of class  cEntity */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cEntity_GetSpeedX00
static int tolua_AllToLua_cEntity_GetSpeedX00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cEntity",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cEntity* self = (const cEntity*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetSpeedX'", NULL);
#endif
  {
   double tolua_ret = (double)  self->GetSpeedX();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetSpeedX'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetSpeedY of class  cEntity */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cEntity_GetSpeedY00
static int tolua_AllToLua_cEntity_GetSpeedY00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cEntity",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cEntity* self = (const cEntity*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetSpeedY'", NULL);
#endif
  {
   double tolua_ret = (double)  self->GetSpeedY();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetSpeedY'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetSpeedZ of class  cEntity */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cEntity_GetSpeedZ00
static int tolua_AllToLua_cEntity_GetSpeedZ00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cEntity",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cEntity* self = (const cEntity*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetSpeedZ'", NULL);
#endif
  {
   double tolua_ret = (double)  self->GetSpeedZ();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetSpeedZ'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetWidth of class  cEntity */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cEntity_GetWidth00
static int tolua_AllToLua_cEntity_GetWidth00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cEntity",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cEntity* self = (const cEntity*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetWidth'", NULL);
#endif
  {
   double tolua_ret = (double)  self->GetWidth();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetWidth'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetChunkX of class  cEntity */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cEntity_GetChunkX00
static int tolua_AllToLua_cEntity_GetChunkX00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cEntity",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cEntity* self = (const cEntity*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetChunkX'", NULL);
#endif
  {
   int tolua_ret = (int)  self->GetChunkX();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetChunkX'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetChunkZ of class  cEntity */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cEntity_GetChunkZ00
static int tolua_AllToLua_cEntity_GetChunkZ00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cEntity",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cEntity* self = (const cEntity*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetChunkZ'", NULL);
#endif
  {
   int tolua_ret = (int)  self->GetChunkZ();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetChunkZ'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetHeadYaw of class  cEntity */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cEntity_SetHeadYaw00
static int tolua_AllToLua_cEntity_SetHeadYaw00(lua_State* tolua_S)
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
  double a_HeadYaw = ((double)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetHeadYaw'", NULL);
#endif
  {
   self->SetHeadYaw(a_HeadYaw);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetHeadYaw'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetHeight of class  cEntity */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cEntity_SetHeight00
static int tolua_AllToLua_cEntity_SetHeight00(lua_State* tolua_S)
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
  double a_Height = ((double)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetHeight'", NULL);
#endif
  {
   self->SetHeight(a_Height);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetHeight'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetMass of class  cEntity */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cEntity_SetMass00
static int tolua_AllToLua_cEntity_SetMass00(lua_State* tolua_S)
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
  double a_Mass = ((double)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetMass'", NULL);
#endif
  {
   self->SetMass(a_Mass);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetMass'.",&tolua_err);
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
  double a_PosX = ((double)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetPosX'", NULL);
#endif
  {
   self->SetPosX(a_PosX);
  }
 }
 return 0;
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
  double a_PosY = ((double)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetPosY'", NULL);
#endif
  {
   self->SetPosY(a_PosY);
  }
 }
 return 0;
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
  double a_PosZ = ((double)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetPosZ'", NULL);
#endif
  {
   self->SetPosZ(a_PosZ);
  }
 }
 return 0;
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
  double a_PosX = ((double)  tolua_tonumber(tolua_S,2,0));
  double a_PosY = ((double)  tolua_tonumber(tolua_S,3,0));
  double a_PosZ = ((double)  tolua_tonumber(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetPosition'", NULL);
#endif
  {
   self->SetPosition(a_PosX,a_PosY,a_PosZ);
  }
 }
 return 0;
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
  double a_Rotation = ((double)  tolua_tonumber(tolua_S,2,0));
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
  double a_Pitch = ((double)  tolua_tonumber(tolua_S,2,0));
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
  double a_Roll = ((double)  tolua_tonumber(tolua_S,2,0));
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

/* method: SetSpeed of class  cEntity */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cEntity_SetSpeed00
static int tolua_AllToLua_cEntity_SetSpeed00(lua_State* tolua_S)
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
  double a_SpeedX = ((double)  tolua_tonumber(tolua_S,2,0));
  double a_SpeedY = ((double)  tolua_tonumber(tolua_S,3,0));
  double a_SpeedZ = ((double)  tolua_tonumber(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetSpeed'", NULL);
#endif
  {
   self->SetSpeed(a_SpeedX,a_SpeedY,a_SpeedZ);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetSpeed'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetSpeed of class  cEntity */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cEntity_SetSpeed01
static int tolua_AllToLua_cEntity_SetSpeed01(lua_State* tolua_S)
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
  const Vector3d* a_Speed = ((const Vector3d*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetSpeed'", NULL);
#endif
  {
   self->SetSpeed(*a_Speed);
  }
 }
 return 0;
tolua_lerror:
 return tolua_AllToLua_cEntity_SetSpeed00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetSpeedX of class  cEntity */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cEntity_SetSpeedX00
static int tolua_AllToLua_cEntity_SetSpeedX00(lua_State* tolua_S)
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
  double a_SpeedX = ((double)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetSpeedX'", NULL);
#endif
  {
   self->SetSpeedX(a_SpeedX);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetSpeedX'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetSpeedY of class  cEntity */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cEntity_SetSpeedY00
static int tolua_AllToLua_cEntity_SetSpeedY00(lua_State* tolua_S)
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
  double a_SpeedY = ((double)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetSpeedY'", NULL);
#endif
  {
   self->SetSpeedY(a_SpeedY);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetSpeedY'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetSpeedZ of class  cEntity */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cEntity_SetSpeedZ00
static int tolua_AllToLua_cEntity_SetSpeedZ00(lua_State* tolua_S)
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
  double a_SpeedZ = ((double)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetSpeedZ'", NULL);
#endif
  {
   self->SetSpeedZ(a_SpeedZ);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetSpeedZ'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetWidth of class  cEntity */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cEntity_SetWidth00
static int tolua_AllToLua_cEntity_SetWidth00(lua_State* tolua_S)
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
  double a_Width = ((double)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetWidth'", NULL);
#endif
  {
   self->SetWidth(a_Width);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetWidth'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: AddPosX of class  cEntity */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cEntity_AddPosX00
static int tolua_AllToLua_cEntity_AddPosX00(lua_State* tolua_S)
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
  double a_AddPosX = ((double)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'AddPosX'", NULL);
#endif
  {
   self->AddPosX(a_AddPosX);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'AddPosX'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: AddPosY of class  cEntity */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cEntity_AddPosY00
static int tolua_AllToLua_cEntity_AddPosY00(lua_State* tolua_S)
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
  double a_AddPosY = ((double)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'AddPosY'", NULL);
#endif
  {
   self->AddPosY(a_AddPosY);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'AddPosY'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: AddPosZ of class  cEntity */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cEntity_AddPosZ00
static int tolua_AllToLua_cEntity_AddPosZ00(lua_State* tolua_S)
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
  double a_AddPosZ = ((double)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'AddPosZ'", NULL);
#endif
  {
   self->AddPosZ(a_AddPosZ);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'AddPosZ'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: AddPosition of class  cEntity */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cEntity_AddPosition00
static int tolua_AllToLua_cEntity_AddPosition00(lua_State* tolua_S)
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
  double a_AddPosX = ((double)  tolua_tonumber(tolua_S,2,0));
  double a_AddPosY = ((double)  tolua_tonumber(tolua_S,3,0));
  double a_AddPosZ = ((double)  tolua_tonumber(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'AddPosition'", NULL);
#endif
  {
   self->AddPosition(a_AddPosX,a_AddPosY,a_AddPosZ);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'AddPosition'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: AddPosition of class  cEntity */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cEntity_AddPosition01
static int tolua_AllToLua_cEntity_AddPosition01(lua_State* tolua_S)
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
  const Vector3d* a_AddPos = ((const Vector3d*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'AddPosition'", NULL);
#endif
  {
   self->AddPosition(*a_AddPos);
  }
 }
 return 0;
tolua_lerror:
 return tolua_AllToLua_cEntity_AddPosition00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: AddSpeed of class  cEntity */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cEntity_AddSpeed00
static int tolua_AllToLua_cEntity_AddSpeed00(lua_State* tolua_S)
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
  double a_AddSpeedX = ((double)  tolua_tonumber(tolua_S,2,0));
  double a_AddSpeedY = ((double)  tolua_tonumber(tolua_S,3,0));
  double a_AddSpeedZ = ((double)  tolua_tonumber(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'AddSpeed'", NULL);
#endif
  {
   self->AddSpeed(a_AddSpeedX,a_AddSpeedY,a_AddSpeedZ);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'AddSpeed'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: AddSpeed of class  cEntity */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cEntity_AddSpeed01
static int tolua_AllToLua_cEntity_AddSpeed01(lua_State* tolua_S)
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
  const Vector3d* a_AddSpeed = ((const Vector3d*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'AddSpeed'", NULL);
#endif
  {
   self->AddSpeed(*a_AddSpeed);
  }
 }
 return 0;
tolua_lerror:
 return tolua_AllToLua_cEntity_AddSpeed00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: AddSpeedX of class  cEntity */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cEntity_AddSpeedX00
static int tolua_AllToLua_cEntity_AddSpeedX00(lua_State* tolua_S)
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
  double a_AddSpeedX = ((double)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'AddSpeedX'", NULL);
#endif
  {
   self->AddSpeedX(a_AddSpeedX);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'AddSpeedX'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: AddSpeedY of class  cEntity */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cEntity_AddSpeedY00
static int tolua_AllToLua_cEntity_AddSpeedY00(lua_State* tolua_S)
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
  double a_AddSpeedY = ((double)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'AddSpeedY'", NULL);
#endif
  {
   self->AddSpeedY(a_AddSpeedY);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'AddSpeedY'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: AddSpeedZ of class  cEntity */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cEntity_AddSpeedZ00
static int tolua_AllToLua_cEntity_AddSpeedZ00(lua_State* tolua_S)
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
  double a_AddSpeedZ = ((double)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'AddSpeedZ'", NULL);
#endif
  {
   self->AddSpeedZ(a_AddSpeedZ);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'AddSpeedZ'.",&tolua_err);
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
     !tolua_isusertype(tolua_S,1,"const cEntity",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cEntity* self = (const cEntity*)  tolua_tousertype(tolua_S,1,0);
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
     !tolua_isusertype(tolua_S,1,"const cEntity",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cEntity* self = (const cEntity*)  tolua_tousertype(tolua_S,1,0);
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
     !tolua_isboolean(tolua_S,2,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cEntity* self = (cEntity*)  tolua_tousertype(tolua_S,1,0);
  bool a_ShouldBroadcast = ((bool)  tolua_toboolean(tolua_S,2,true));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'Destroy'", NULL);
#endif
  {
   self->Destroy(a_ShouldBroadcast);
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

/* method: TakeDamage of class  cEntity */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cEntity_TakeDamage00
static int tolua_AllToLua_cEntity_TakeDamage00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cEntity",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"cEntity",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cEntity* self = (cEntity*)  tolua_tousertype(tolua_S,1,0);
  cEntity* a_Attacker = ((cEntity*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'TakeDamage'", NULL);
#endif
  {
   self->TakeDamage(*a_Attacker);
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

/* method: TakeDamage of class  cEntity */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cEntity_TakeDamage01
static int tolua_AllToLua_cEntity_TakeDamage01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cEntity",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"cEntity",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  cEntity* self = (cEntity*)  tolua_tousertype(tolua_S,1,0);
  eDamageType a_DamageType = ((eDamageType) (int)  tolua_tonumber(tolua_S,2,0));
  cEntity* a_Attacker = ((cEntity*)  tolua_tousertype(tolua_S,3,0));
  int a_RawDamage = ((int)  tolua_tonumber(tolua_S,4,0));
  double a_KnockbackAmount = ((double)  tolua_tonumber(tolua_S,5,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'TakeDamage'", NULL);
#endif
  {
   self->TakeDamage(a_DamageType,a_Attacker,a_RawDamage,a_KnockbackAmount);
  }
 }
 return 0;
tolua_lerror:
 return tolua_AllToLua_cEntity_TakeDamage00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: TakeDamage of class  cEntity */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cEntity_TakeDamage02
static int tolua_AllToLua_cEntity_TakeDamage02(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cEntity",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"cEntity",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,6,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,7,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  cEntity* self = (cEntity*)  tolua_tousertype(tolua_S,1,0);
  eDamageType a_DamageType = ((eDamageType) (int)  tolua_tonumber(tolua_S,2,0));
  cEntity* a_Attacker = ((cEntity*)  tolua_tousertype(tolua_S,3,0));
  int a_RawDamage = ((int)  tolua_tonumber(tolua_S,4,0));
  int a_FinalDamage = ((int)  tolua_tonumber(tolua_S,5,0));
  double a_KnockbackAmount = ((double)  tolua_tonumber(tolua_S,6,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'TakeDamage'", NULL);
#endif
  {
   self->TakeDamage(a_DamageType,a_Attacker,a_RawDamage,a_FinalDamage,a_KnockbackAmount);
  }
 }
 return 0;
tolua_lerror:
 return tolua_AllToLua_cEntity_TakeDamage01(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetRawDamageAgainst of class  cEntity */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cEntity_GetRawDamageAgainst00
static int tolua_AllToLua_cEntity_GetRawDamageAgainst00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cEntity",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const cEntity",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cEntity* self = (cEntity*)  tolua_tousertype(tolua_S,1,0);
  const cEntity* a_Receiver = ((const cEntity*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetRawDamageAgainst'", NULL);
#endif
  {
   int tolua_ret = (int)  self->GetRawDamageAgainst(*a_Receiver);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetRawDamageAgainst'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetArmorCoverAgainst of class  cEntity */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cEntity_GetArmorCoverAgainst00
static int tolua_AllToLua_cEntity_GetArmorCoverAgainst00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cEntity",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const cEntity",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cEntity* self = (cEntity*)  tolua_tousertype(tolua_S,1,0);
  const cEntity* a_Attacker = ((const cEntity*)  tolua_tousertype(tolua_S,2,0));
  eDamageType a_DamageType = ((eDamageType) (int)  tolua_tonumber(tolua_S,3,0));
  int a_RawDamage = ((int)  tolua_tonumber(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetArmorCoverAgainst'", NULL);
#endif
  {
   int tolua_ret = (int)  self->GetArmorCoverAgainst(a_Attacker,a_DamageType,a_RawDamage);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetArmorCoverAgainst'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetKnockbackAmountAgainst of class  cEntity */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cEntity_GetKnockbackAmountAgainst00
static int tolua_AllToLua_cEntity_GetKnockbackAmountAgainst00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cEntity",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const cEntity",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cEntity* self = (cEntity*)  tolua_tousertype(tolua_S,1,0);
  const cEntity* a_Receiver = ((const cEntity*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetKnockbackAmountAgainst'", NULL);
#endif
  {
   double tolua_ret = (double)  self->GetKnockbackAmountAgainst(*a_Receiver);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetKnockbackAmountAgainst'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetEquippedWeapon of class  cEntity */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cEntity_GetEquippedWeapon00
static int tolua_AllToLua_cEntity_GetEquippedWeapon00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cEntity",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cEntity* self = (const cEntity*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetEquippedWeapon'", NULL);
#endif
  {
   cItem tolua_ret = (cItem)  self->GetEquippedWeapon();
   {
#ifdef __cplusplus
    void* tolua_obj = Mtolua_new((cItem)(tolua_ret));
     tolua_pushusertype(tolua_S,tolua_obj,"cItem");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(cItem));
     tolua_pushusertype(tolua_S,tolua_obj,"cItem");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetEquippedWeapon'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetEquippedHelmet of class  cEntity */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cEntity_GetEquippedHelmet00
static int tolua_AllToLua_cEntity_GetEquippedHelmet00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cEntity",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cEntity* self = (const cEntity*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetEquippedHelmet'", NULL);
#endif
  {
   cItem tolua_ret = (cItem)  self->GetEquippedHelmet();
   {
#ifdef __cplusplus
    void* tolua_obj = Mtolua_new((cItem)(tolua_ret));
     tolua_pushusertype(tolua_S,tolua_obj,"cItem");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(cItem));
     tolua_pushusertype(tolua_S,tolua_obj,"cItem");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetEquippedHelmet'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetEquippedChestplate of class  cEntity */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cEntity_GetEquippedChestplate00
static int tolua_AllToLua_cEntity_GetEquippedChestplate00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cEntity",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cEntity* self = (const cEntity*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetEquippedChestplate'", NULL);
#endif
  {
   cItem tolua_ret = (cItem)  self->GetEquippedChestplate();
   {
#ifdef __cplusplus
    void* tolua_obj = Mtolua_new((cItem)(tolua_ret));
     tolua_pushusertype(tolua_S,tolua_obj,"cItem");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(cItem));
     tolua_pushusertype(tolua_S,tolua_obj,"cItem");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetEquippedChestplate'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetEquippedLeggings of class  cEntity */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cEntity_GetEquippedLeggings00
static int tolua_AllToLua_cEntity_GetEquippedLeggings00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cEntity",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cEntity* self = (const cEntity*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetEquippedLeggings'", NULL);
#endif
  {
   cItem tolua_ret = (cItem)  self->GetEquippedLeggings();
   {
#ifdef __cplusplus
    void* tolua_obj = Mtolua_new((cItem)(tolua_ret));
     tolua_pushusertype(tolua_S,tolua_obj,"cItem");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(cItem));
     tolua_pushusertype(tolua_S,tolua_obj,"cItem");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetEquippedLeggings'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetEquippedBoots of class  cEntity */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cEntity_GetEquippedBoots00
static int tolua_AllToLua_cEntity_GetEquippedBoots00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cEntity",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cEntity* self = (const cEntity*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetEquippedBoots'", NULL);
#endif
  {
   cItem tolua_ret = (cItem)  self->GetEquippedBoots();
   {
#ifdef __cplusplus
    void* tolua_obj = Mtolua_new((cItem)(tolua_ret));
     tolua_pushusertype(tolua_S,tolua_obj,"cItem");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(cItem));
     tolua_pushusertype(tolua_S,tolua_obj,"cItem");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetEquippedBoots'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: KilledBy of class  cEntity */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cEntity_KilledBy00
static int tolua_AllToLua_cEntity_KilledBy00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cEntity",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"cEntity",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cEntity* self = (cEntity*)  tolua_tousertype(tolua_S,1,0);
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

/* method: Heal of class  cEntity */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cEntity_Heal00
static int tolua_AllToLua_cEntity_Heal00(lua_State* tolua_S)
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
  int a_HitPoints = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'Heal'", NULL);
#endif
  {
   self->Heal(a_HitPoints);
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

/* method: GetHealth of class  cEntity */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cEntity_GetHealth00
static int tolua_AllToLua_cEntity_GetHealth00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cEntity",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cEntity* self = (const cEntity*)  tolua_tousertype(tolua_S,1,0);
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

/* method: SetHealth of class  cEntity */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cEntity_SetHealth00
static int tolua_AllToLua_cEntity_SetHealth00(lua_State* tolua_S)
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
  int a_Health = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetHealth'", NULL);
#endif
  {
   self->SetHealth(a_Health);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetHealth'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetMaxHealth of class  cEntity */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cEntity_SetMaxHealth00
static int tolua_AllToLua_cEntity_SetMaxHealth00(lua_State* tolua_S)
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
  int a_MaxHealth = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetMaxHealth'", NULL);
#endif
  {
   self->SetMaxHealth(a_MaxHealth);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetMaxHealth'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetMaxHealth of class  cEntity */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cEntity_GetMaxHealth00
static int tolua_AllToLua_cEntity_GetMaxHealth00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cEntity",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cEntity* self = (const cEntity*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetMaxHealth'", NULL);
#endif
  {
   int tolua_ret = (int)  self->GetMaxHealth();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetMaxHealth'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: StartBurning of class  cEntity */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cEntity_StartBurning00
static int tolua_AllToLua_cEntity_StartBurning00(lua_State* tolua_S)
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
  int a_TicksLeftBurning = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'StartBurning'", NULL);
#endif
  {
   self->StartBurning(a_TicksLeftBurning);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'StartBurning'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: StopBurning of class  cEntity */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cEntity_StopBurning00
static int tolua_AllToLua_cEntity_StopBurning00(lua_State* tolua_S)
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'StopBurning'", NULL);
#endif
  {
   self->StopBurning();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'StopBurning'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: TeleportToEntity of class  cEntity */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cEntity_TeleportToEntity00
static int tolua_AllToLua_cEntity_TeleportToEntity00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cEntity",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"cEntity",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cEntity* self = (cEntity*)  tolua_tousertype(tolua_S,1,0);
  cEntity* a_Entity = ((cEntity*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'TeleportToEntity'", NULL);
#endif
  {
   self->TeleportToEntity(*a_Entity);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'TeleportToEntity'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: TeleportToCoords of class  cEntity */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cEntity_TeleportToCoords00
static int tolua_AllToLua_cEntity_TeleportToCoords00(lua_State* tolua_S)
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
  double a_PosX = ((double)  tolua_tonumber(tolua_S,2,0));
  double a_PosY = ((double)  tolua_tonumber(tolua_S,3,0));
  double a_PosZ = ((double)  tolua_tonumber(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'TeleportToCoords'", NULL);
#endif
  {
   self->TeleportToCoords(a_PosX,a_PosY,a_PosZ);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'TeleportToCoords'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: IsOnFire of class  cEntity */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cEntity_IsOnFire00
static int tolua_AllToLua_cEntity_IsOnFire00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cEntity",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cEntity* self = (const cEntity*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'IsOnFire'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->IsOnFire();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'IsOnFire'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: IsCrouched of class  cEntity */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cEntity_IsCrouched00
static int tolua_AllToLua_cEntity_IsCrouched00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cEntity",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cEntity* self = (const cEntity*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'IsCrouched'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->IsCrouched();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'IsCrouched'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: IsRiding of class  cEntity */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cEntity_IsRiding00
static int tolua_AllToLua_cEntity_IsRiding00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cEntity",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cEntity* self = (const cEntity*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'IsRiding'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->IsRiding();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'IsRiding'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: IsSprinting of class  cEntity */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cEntity_IsSprinting00
static int tolua_AllToLua_cEntity_IsSprinting00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cEntity",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cEntity* self = (const cEntity*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'IsSprinting'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->IsSprinting();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'IsSprinting'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: IsRclking of class  cEntity */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cEntity_IsRclking00
static int tolua_AllToLua_cEntity_IsRclking00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cEntity",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cEntity* self = (const cEntity*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'IsRclking'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->IsRclking();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'IsRclking'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

 class Lua__cEntity : public cEntity, public ToluaBase {
public:
	 bool  IsA( const char* a_ClassName)const {
		if (push_method("IsA",  tolua_AllToLua_cEntity_IsA00)) {
			tolua_pushstring(lua_state, (const char*)a_ClassName);
			ToluaBase::dbcall(lua_state, 2, 1);
			 bool  tolua_ret = ( bool )tolua_toboolean(lua_state, -1, 0);
			lua_pop(lua_state, 1);
			return tolua_ret;
		} else {
			return ( bool ) cEntity:: IsA(a_ClassName);
		};
	};
	 const char*  GetClass( void )const {
		if (push_method("GetClass",  tolua_AllToLua_cEntity_GetClass00)) {
			ToluaBase::dbcall(lua_state, 1, 1);
			 const char*  tolua_ret = ( const char* )tolua_tostring(lua_state, -1, 0);
			lua_pop(lua_state, 1);
			return tolua_ret;
		} else {
			return ( const char* ) cEntity:: GetClass();
		};
	};
	 const char*  GetParentClass( void )const {
		if (push_method("GetParentClass",  tolua_AllToLua_cEntity_GetParentClass00)) {
			ToluaBase::dbcall(lua_state, 1, 1);
			 const char*  tolua_ret = ( const char* )tolua_tostring(lua_state, -1, 0);
			lua_pop(lua_state, 1);
			return tolua_ret;
		} else {
			return ( const char* ) cEntity:: GetParentClass();
		};
	};
	 int  GetRawDamageAgainst( const cEntity& a_Receiver) {
		if (push_method("GetRawDamageAgainst",  tolua_AllToLua_cEntity_GetRawDamageAgainst00)) {
			tolua_pushusertype(lua_state, (void*)&a_Receiver, "const cEntity");
			ToluaBase::dbcall(lua_state, 2, 1);
			 int  tolua_ret = ( int )tolua_tonumber(lua_state, -1, 0);
			lua_pop(lua_state, 1);
			return tolua_ret;
		} else {
			return ( int ) cEntity:: GetRawDamageAgainst(a_Receiver);
		};
	};
	 int  GetArmorCoverAgainst( const cEntity* a_Attacker, eDamageType a_DamageType, int a_RawDamage) {
		if (push_method("GetArmorCoverAgainst",  tolua_AllToLua_cEntity_GetArmorCoverAgainst00)) {
			tolua_pushusertype(lua_state, (void*)a_Attacker, "const cEntity");
			tolua_pushnumber(lua_state, (lua_Number)a_DamageType);
			tolua_pushnumber(lua_state, (lua_Number)a_RawDamage);
			ToluaBase::dbcall(lua_state, 4, 1);
			 int  tolua_ret = ( int )tolua_tonumber(lua_state, -1, 0);
			lua_pop(lua_state, 1);
			return tolua_ret;
		} else {
			return ( int ) cEntity:: GetArmorCoverAgainst(a_Attacker,a_DamageType,a_RawDamage);
		};
	};
	 double  GetKnockbackAmountAgainst( const cEntity& a_Receiver) {
		if (push_method("GetKnockbackAmountAgainst",  tolua_AllToLua_cEntity_GetKnockbackAmountAgainst00)) {
			tolua_pushusertype(lua_state, (void*)&a_Receiver, "const cEntity");
			ToluaBase::dbcall(lua_state, 2, 1);
			 double  tolua_ret = ( double )tolua_tonumber(lua_state, -1, 0);
			lua_pop(lua_state, 1);
			return tolua_ret;
		} else {
			return ( double ) cEntity:: GetKnockbackAmountAgainst(a_Receiver);
		};
	};
	 cItem  GetEquippedWeapon( void )const {
		if (push_method("GetEquippedWeapon",  tolua_AllToLua_cEntity_GetEquippedWeapon00)) {
			ToluaBase::dbcall(lua_state, 1, 1);
			 cItem  tolua_ret = ( cItem )*(cItem*)tolua_tousertype(lua_state, -1, 0);
			lua_pop(lua_state, 1);
			return tolua_ret;
		} else {
			return ( cItem ) cEntity:: GetEquippedWeapon();
		};
	};
	 cItem  GetEquippedHelmet( void )const {
		if (push_method("GetEquippedHelmet",  tolua_AllToLua_cEntity_GetEquippedHelmet00)) {
			ToluaBase::dbcall(lua_state, 1, 1);
			 cItem  tolua_ret = ( cItem )*(cItem*)tolua_tousertype(lua_state, -1, 0);
			lua_pop(lua_state, 1);
			return tolua_ret;
		} else {
			return ( cItem ) cEntity:: GetEquippedHelmet();
		};
	};
	 cItem  GetEquippedChestplate( void )const {
		if (push_method("GetEquippedChestplate",  tolua_AllToLua_cEntity_GetEquippedChestplate00)) {
			ToluaBase::dbcall(lua_state, 1, 1);
			 cItem  tolua_ret = ( cItem )*(cItem*)tolua_tousertype(lua_state, -1, 0);
			lua_pop(lua_state, 1);
			return tolua_ret;
		} else {
			return ( cItem ) cEntity:: GetEquippedChestplate();
		};
	};
	 cItem  GetEquippedLeggings( void )const {
		if (push_method("GetEquippedLeggings",  tolua_AllToLua_cEntity_GetEquippedLeggings00)) {
			ToluaBase::dbcall(lua_state, 1, 1);
			 cItem  tolua_ret = ( cItem )*(cItem*)tolua_tousertype(lua_state, -1, 0);
			lua_pop(lua_state, 1);
			return tolua_ret;
		} else {
			return ( cItem ) cEntity:: GetEquippedLeggings();
		};
	};
	 cItem  GetEquippedBoots( void )const {
		if (push_method("GetEquippedBoots",  tolua_AllToLua_cEntity_GetEquippedBoots00)) {
			ToluaBase::dbcall(lua_state, 1, 1);
			 cItem  tolua_ret = ( cItem )*(cItem*)tolua_tousertype(lua_state, -1, 0);
			lua_pop(lua_state, 1);
			return tolua_ret;
		} else {
			return ( cItem ) cEntity:: GetEquippedBoots();
		};
	};
	 void  KilledBy( cEntity* a_Killer) {
		if (push_method("KilledBy",  tolua_AllToLua_cEntity_KilledBy00)) {
			tolua_pushusertype(lua_state, (void*)a_Killer, "cEntity");
			ToluaBase::dbcall(lua_state, 2, 0);
		} else {
			return ( void ) cEntity:: KilledBy(a_Killer);
		};
	};
	 void  TeleportToEntity( cEntity& a_Entity) {
		if (push_method("TeleportToEntity",  tolua_AllToLua_cEntity_TeleportToEntity00)) {
			tolua_pushusertype(lua_state, (void*)&a_Entity, "cEntity");
			ToluaBase::dbcall(lua_state, 2, 0);
		} else {
			return ( void ) cEntity:: TeleportToEntity(a_Entity);
		};
	};
	 void  TeleportToCoords( double a_PosX, double a_PosY, double a_PosZ) {
		if (push_method("TeleportToCoords",  tolua_AllToLua_cEntity_TeleportToCoords00)) {
			tolua_pushnumber(lua_state, (lua_Number)a_PosX);
			tolua_pushnumber(lua_state, (lua_Number)a_PosY);
			tolua_pushnumber(lua_state, (lua_Number)a_PosZ);
			ToluaBase::dbcall(lua_state, 4, 0);
		} else {
			return ( void ) cEntity:: TeleportToCoords(a_PosX,a_PosY,a_PosZ);
		};
	};
	 bool  IsOnFire( void )const {
		if (push_method("IsOnFire",  tolua_AllToLua_cEntity_IsOnFire00)) {
			ToluaBase::dbcall(lua_state, 1, 1);
			 bool  tolua_ret = ( bool )tolua_toboolean(lua_state, -1, 0);
			lua_pop(lua_state, 1);
			return tolua_ret;
		} else {
			return ( bool ) cEntity:: IsOnFire();
		};
	};
	 bool  IsCrouched( void )const {
		if (push_method("IsCrouched",  tolua_AllToLua_cEntity_IsCrouched00)) {
			ToluaBase::dbcall(lua_state, 1, 1);
			 bool  tolua_ret = ( bool )tolua_toboolean(lua_state, -1, 0);
			lua_pop(lua_state, 1);
			return tolua_ret;
		} else {
			return ( bool ) cEntity:: IsCrouched();
		};
	};
	 bool  IsRiding( void )const {
		if (push_method("IsRiding",  tolua_AllToLua_cEntity_IsRiding00)) {
			ToluaBase::dbcall(lua_state, 1, 1);
			 bool  tolua_ret = ( bool )tolua_toboolean(lua_state, -1, 0);
			lua_pop(lua_state, 1);
			return tolua_ret;
		} else {
			return ( bool ) cEntity:: IsRiding();
		};
	};
	 bool  IsSprinting( void )const {
		if (push_method("IsSprinting",  tolua_AllToLua_cEntity_IsSprinting00)) {
			ToluaBase::dbcall(lua_state, 1, 1);
			 bool  tolua_ret = ( bool )tolua_toboolean(lua_state, -1, 0);
			lua_pop(lua_state, 1);
			return tolua_ret;
		} else {
			return ( bool ) cEntity:: IsSprinting();
		};
	};
	 bool  IsRclking( void )const {
		if (push_method("IsRclking",  tolua_AllToLua_cEntity_IsRclking00)) {
			ToluaBase::dbcall(lua_state, 1, 1);
			 bool  tolua_ret = ( bool )tolua_toboolean(lua_state, -1, 0);
			lua_pop(lua_state, 1);
			return tolua_ret;
		} else {
			return ( bool ) cEntity:: IsRclking();
		};
	};

	 bool cEntity__IsA( const char* a_ClassName) {
		return ( bool )cEntity::IsA(a_ClassName);
	};
	 const char* cEntity__GetClass( void ) {
		return ( const char* )cEntity::GetClass();
	};
	 const char* cEntity__GetParentClass( void ) {
		return ( const char* )cEntity::GetParentClass();
	};
	 int cEntity__GetRawDamageAgainst( const cEntity& a_Receiver) {
		return ( int )cEntity::GetRawDamageAgainst(a_Receiver);
	};
	 int cEntity__GetArmorCoverAgainst( const cEntity* a_Attacker, eDamageType a_DamageType, int a_RawDamage) {
		return ( int )cEntity::GetArmorCoverAgainst(a_Attacker,a_DamageType,a_RawDamage);
	};
	 double cEntity__GetKnockbackAmountAgainst( const cEntity& a_Receiver) {
		return ( double )cEntity::GetKnockbackAmountAgainst(a_Receiver);
	};
	 cItem cEntity__GetEquippedWeapon( void ) {
		return ( cItem )cEntity::GetEquippedWeapon();
	};
	 cItem cEntity__GetEquippedHelmet( void ) {
		return ( cItem )cEntity::GetEquippedHelmet();
	};
	 cItem cEntity__GetEquippedChestplate( void ) {
		return ( cItem )cEntity::GetEquippedChestplate();
	};
	 cItem cEntity__GetEquippedLeggings( void ) {
		return ( cItem )cEntity::GetEquippedLeggings();
	};
	 cItem cEntity__GetEquippedBoots( void ) {
		return ( cItem )cEntity::GetEquippedBoots();
	};
	 void cEntity__KilledBy( cEntity* a_Killer) {
		return ( void )cEntity::KilledBy(a_Killer);
	};
	 void cEntity__TeleportToEntity( cEntity& a_Entity) {
		return ( void )cEntity::TeleportToEntity(a_Entity);
	};
	 void cEntity__TeleportToCoords( double a_PosX, double a_PosY, double a_PosZ) {
		return ( void )cEntity::TeleportToCoords(a_PosX,a_PosY,a_PosZ);
	};
	 bool cEntity__IsOnFire( void ) {
		return ( bool )cEntity::IsOnFire();
	};
	 bool cEntity__IsCrouched( void ) {
		return ( bool )cEntity::IsCrouched();
	};
	 bool cEntity__IsRiding( void ) {
		return ( bool )cEntity::IsRiding();
	};
	 bool cEntity__IsSprinting( void ) {
		return ( bool )cEntity::IsSprinting();
	};
	 bool cEntity__IsRclking( void ) {
		return ( bool )cEntity::IsRclking();
	};
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
  const char* a_ClassName = ((const char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'cEntity__IsA'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->cEntity__IsA(a_ClassName);
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

/* method: cEntity__GetParentClass of class  Lua__cEntity */
#ifndef TOLUA_DISABLE_tolua_AllToLua_Lua__cEntity_cEntity__GetParentClass00
static int tolua_AllToLua_Lua__cEntity_cEntity__GetParentClass00(lua_State* tolua_S)
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'cEntity__GetParentClass'", NULL);
#endif
  {
   const char* tolua_ret = (const char*)  self->cEntity__GetParentClass();
   tolua_pushstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'cEntity__GetParentClass'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: cEntity__GetRawDamageAgainst of class  Lua__cEntity */
#ifndef TOLUA_DISABLE_tolua_AllToLua_Lua__cEntity_cEntity__GetRawDamageAgainst00
static int tolua_AllToLua_Lua__cEntity_cEntity__GetRawDamageAgainst00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Lua__cEntity",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const cEntity",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Lua__cEntity* self = (Lua__cEntity*)  tolua_tousertype(tolua_S,1,0);
  const cEntity* a_Receiver = ((const cEntity*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'cEntity__GetRawDamageAgainst'", NULL);
#endif
  {
   int tolua_ret = (int)  self->cEntity__GetRawDamageAgainst(*a_Receiver);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'cEntity__GetRawDamageAgainst'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: cEntity__GetArmorCoverAgainst of class  Lua__cEntity */
#ifndef TOLUA_DISABLE_tolua_AllToLua_Lua__cEntity_cEntity__GetArmorCoverAgainst00
static int tolua_AllToLua_Lua__cEntity_cEntity__GetArmorCoverAgainst00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Lua__cEntity",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const cEntity",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Lua__cEntity* self = (Lua__cEntity*)  tolua_tousertype(tolua_S,1,0);
  const cEntity* a_Attacker = ((const cEntity*)  tolua_tousertype(tolua_S,2,0));
  eDamageType a_DamageType = ((eDamageType) (int)  tolua_tonumber(tolua_S,3,0));
  int a_RawDamage = ((int)  tolua_tonumber(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'cEntity__GetArmorCoverAgainst'", NULL);
#endif
  {
   int tolua_ret = (int)  self->cEntity__GetArmorCoverAgainst(a_Attacker,a_DamageType,a_RawDamage);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'cEntity__GetArmorCoverAgainst'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: cEntity__GetKnockbackAmountAgainst of class  Lua__cEntity */
#ifndef TOLUA_DISABLE_tolua_AllToLua_Lua__cEntity_cEntity__GetKnockbackAmountAgainst00
static int tolua_AllToLua_Lua__cEntity_cEntity__GetKnockbackAmountAgainst00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Lua__cEntity",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const cEntity",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Lua__cEntity* self = (Lua__cEntity*)  tolua_tousertype(tolua_S,1,0);
  const cEntity* a_Receiver = ((const cEntity*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'cEntity__GetKnockbackAmountAgainst'", NULL);
#endif
  {
   double tolua_ret = (double)  self->cEntity__GetKnockbackAmountAgainst(*a_Receiver);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'cEntity__GetKnockbackAmountAgainst'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: cEntity__GetEquippedWeapon of class  Lua__cEntity */
#ifndef TOLUA_DISABLE_tolua_AllToLua_Lua__cEntity_cEntity__GetEquippedWeapon00
static int tolua_AllToLua_Lua__cEntity_cEntity__GetEquippedWeapon00(lua_State* tolua_S)
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'cEntity__GetEquippedWeapon'", NULL);
#endif
  {
   cItem tolua_ret = (cItem)  self->cEntity__GetEquippedWeapon();
   {
#ifdef __cplusplus
    void* tolua_obj = Mtolua_new((cItem)(tolua_ret));
     tolua_pushusertype(tolua_S,tolua_obj,"cItem");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(cItem));
     tolua_pushusertype(tolua_S,tolua_obj,"cItem");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'cEntity__GetEquippedWeapon'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: cEntity__GetEquippedHelmet of class  Lua__cEntity */
#ifndef TOLUA_DISABLE_tolua_AllToLua_Lua__cEntity_cEntity__GetEquippedHelmet00
static int tolua_AllToLua_Lua__cEntity_cEntity__GetEquippedHelmet00(lua_State* tolua_S)
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'cEntity__GetEquippedHelmet'", NULL);
#endif
  {
   cItem tolua_ret = (cItem)  self->cEntity__GetEquippedHelmet();
   {
#ifdef __cplusplus
    void* tolua_obj = Mtolua_new((cItem)(tolua_ret));
     tolua_pushusertype(tolua_S,tolua_obj,"cItem");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(cItem));
     tolua_pushusertype(tolua_S,tolua_obj,"cItem");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'cEntity__GetEquippedHelmet'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: cEntity__GetEquippedChestplate of class  Lua__cEntity */
#ifndef TOLUA_DISABLE_tolua_AllToLua_Lua__cEntity_cEntity__GetEquippedChestplate00
static int tolua_AllToLua_Lua__cEntity_cEntity__GetEquippedChestplate00(lua_State* tolua_S)
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'cEntity__GetEquippedChestplate'", NULL);
#endif
  {
   cItem tolua_ret = (cItem)  self->cEntity__GetEquippedChestplate();
   {
#ifdef __cplusplus
    void* tolua_obj = Mtolua_new((cItem)(tolua_ret));
     tolua_pushusertype(tolua_S,tolua_obj,"cItem");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(cItem));
     tolua_pushusertype(tolua_S,tolua_obj,"cItem");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'cEntity__GetEquippedChestplate'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: cEntity__GetEquippedLeggings of class  Lua__cEntity */
#ifndef TOLUA_DISABLE_tolua_AllToLua_Lua__cEntity_cEntity__GetEquippedLeggings00
static int tolua_AllToLua_Lua__cEntity_cEntity__GetEquippedLeggings00(lua_State* tolua_S)
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'cEntity__GetEquippedLeggings'", NULL);
#endif
  {
   cItem tolua_ret = (cItem)  self->cEntity__GetEquippedLeggings();
   {
#ifdef __cplusplus
    void* tolua_obj = Mtolua_new((cItem)(tolua_ret));
     tolua_pushusertype(tolua_S,tolua_obj,"cItem");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(cItem));
     tolua_pushusertype(tolua_S,tolua_obj,"cItem");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'cEntity__GetEquippedLeggings'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: cEntity__GetEquippedBoots of class  Lua__cEntity */
#ifndef TOLUA_DISABLE_tolua_AllToLua_Lua__cEntity_cEntity__GetEquippedBoots00
static int tolua_AllToLua_Lua__cEntity_cEntity__GetEquippedBoots00(lua_State* tolua_S)
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'cEntity__GetEquippedBoots'", NULL);
#endif
  {
   cItem tolua_ret = (cItem)  self->cEntity__GetEquippedBoots();
   {
#ifdef __cplusplus
    void* tolua_obj = Mtolua_new((cItem)(tolua_ret));
     tolua_pushusertype(tolua_S,tolua_obj,"cItem");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(cItem));
     tolua_pushusertype(tolua_S,tolua_obj,"cItem");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'cEntity__GetEquippedBoots'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: cEntity__KilledBy of class  Lua__cEntity */
#ifndef TOLUA_DISABLE_tolua_AllToLua_Lua__cEntity_cEntity__KilledBy00
static int tolua_AllToLua_Lua__cEntity_cEntity__KilledBy00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Lua__cEntity",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"cEntity",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Lua__cEntity* self = (Lua__cEntity*)  tolua_tousertype(tolua_S,1,0);
  cEntity* a_Killer = ((cEntity*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'cEntity__KilledBy'", NULL);
#endif
  {
   self->cEntity__KilledBy(a_Killer);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'cEntity__KilledBy'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: cEntity__TeleportToEntity of class  Lua__cEntity */
#ifndef TOLUA_DISABLE_tolua_AllToLua_Lua__cEntity_cEntity__TeleportToEntity00
static int tolua_AllToLua_Lua__cEntity_cEntity__TeleportToEntity00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Lua__cEntity",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"cEntity",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Lua__cEntity* self = (Lua__cEntity*)  tolua_tousertype(tolua_S,1,0);
  cEntity* a_Entity = ((cEntity*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'cEntity__TeleportToEntity'", NULL);
#endif
  {
   self->cEntity__TeleportToEntity(*a_Entity);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'cEntity__TeleportToEntity'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: cEntity__TeleportToCoords of class  Lua__cEntity */
#ifndef TOLUA_DISABLE_tolua_AllToLua_Lua__cEntity_cEntity__TeleportToCoords00
static int tolua_AllToLua_Lua__cEntity_cEntity__TeleportToCoords00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Lua__cEntity",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Lua__cEntity* self = (Lua__cEntity*)  tolua_tousertype(tolua_S,1,0);
  double a_PosX = ((double)  tolua_tonumber(tolua_S,2,0));
  double a_PosY = ((double)  tolua_tonumber(tolua_S,3,0));
  double a_PosZ = ((double)  tolua_tonumber(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'cEntity__TeleportToCoords'", NULL);
#endif
  {
   self->cEntity__TeleportToCoords(a_PosX,a_PosY,a_PosZ);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'cEntity__TeleportToCoords'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: cEntity__IsOnFire of class  Lua__cEntity */
#ifndef TOLUA_DISABLE_tolua_AllToLua_Lua__cEntity_cEntity__IsOnFire00
static int tolua_AllToLua_Lua__cEntity_cEntity__IsOnFire00(lua_State* tolua_S)
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'cEntity__IsOnFire'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->cEntity__IsOnFire();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'cEntity__IsOnFire'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: cEntity__IsCrouched of class  Lua__cEntity */
#ifndef TOLUA_DISABLE_tolua_AllToLua_Lua__cEntity_cEntity__IsCrouched00
static int tolua_AllToLua_Lua__cEntity_cEntity__IsCrouched00(lua_State* tolua_S)
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'cEntity__IsCrouched'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->cEntity__IsCrouched();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'cEntity__IsCrouched'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: cEntity__IsRiding of class  Lua__cEntity */
#ifndef TOLUA_DISABLE_tolua_AllToLua_Lua__cEntity_cEntity__IsRiding00
static int tolua_AllToLua_Lua__cEntity_cEntity__IsRiding00(lua_State* tolua_S)
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'cEntity__IsRiding'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->cEntity__IsRiding();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'cEntity__IsRiding'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: cEntity__IsSprinting of class  Lua__cEntity */
#ifndef TOLUA_DISABLE_tolua_AllToLua_Lua__cEntity_cEntity__IsSprinting00
static int tolua_AllToLua_Lua__cEntity_cEntity__IsSprinting00(lua_State* tolua_S)
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'cEntity__IsSprinting'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->cEntity__IsSprinting();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'cEntity__IsSprinting'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: cEntity__IsRclking of class  Lua__cEntity */
#ifndef TOLUA_DISABLE_tolua_AllToLua_Lua__cEntity_cEntity__IsRclking00
static int tolua_AllToLua_Lua__cEntity_cEntity__IsRclking00(lua_State* tolua_S)
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'cEntity__IsRclking'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->cEntity__IsRclking();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'cEntity__IsRclking'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: Initialize of class  cPlayer */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cPlayer_Initialize00
static int tolua_AllToLua_cPlayer_Initialize00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cPlayer",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"cWorld",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cPlayer* self = (cPlayer*)  tolua_tousertype(tolua_S,1,0);
  cWorld* a_World = ((cWorld*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'Initialize'", NULL);
#endif
  {
   self->Initialize(a_World);
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

/* method: GetEyeHeight of class  cPlayer */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cPlayer_GetEyeHeight00
static int tolua_AllToLua_cPlayer_GetEyeHeight00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cPlayer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cPlayer* self = (const cPlayer*)  tolua_tousertype(tolua_S,1,0);
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
     !tolua_isusertype(tolua_S,1,"const cPlayer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cPlayer* self = (const cPlayer*)  tolua_tousertype(tolua_S,1,0);
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

/* method: IsOnGround of class  cPlayer */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cPlayer_IsOnGround00
static int tolua_AllToLua_cPlayer_IsOnGround00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cPlayer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cPlayer* self = (const cPlayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'IsOnGround'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->IsOnGround();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'IsOnGround'.",&tolua_err);
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
     !tolua_isusertype(tolua_S,1,"const cPlayer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cPlayer* self = (const cPlayer*)  tolua_tousertype(tolua_S,1,0);
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

/* method: GetEquippedItem of class  cPlayer */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cPlayer_GetEquippedItem00
static int tolua_AllToLua_cPlayer_GetEquippedItem00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cPlayer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cPlayer* self = (const cPlayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetEquippedItem'", NULL);
#endif
  {
   const cItem& tolua_ret = (const cItem&)  self->GetEquippedItem();
    tolua_pushusertype(tolua_S,(void*)&tolua_ret,"const cItem");
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

/* method: GetGameMode of class  cPlayer */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cPlayer_GetGameMode00
static int tolua_AllToLua_cPlayer_GetGameMode00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cPlayer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cPlayer* self = (const cPlayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetGameMode'", NULL);
#endif
  {
   eGameMode tolua_ret = (eGameMode)  self->GetGameMode();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetGameMode'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetGameMode of class  cPlayer */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cPlayer_SetGameMode00
static int tolua_AllToLua_cPlayer_SetGameMode00(lua_State* tolua_S)
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
  eGameMode a_GameMode = ((eGameMode) (int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetGameMode'", NULL);
#endif
  {
   self->SetGameMode(a_GameMode);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetGameMode'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: IsGameModeCreative of class  cPlayer */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cPlayer_IsGameModeCreative00
static int tolua_AllToLua_cPlayer_IsGameModeCreative00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cPlayer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cPlayer* self = (const cPlayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'IsGameModeCreative'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->IsGameModeCreative();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'IsGameModeCreative'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: IsGameModeSurvival of class  cPlayer */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cPlayer_IsGameModeSurvival00
static int tolua_AllToLua_cPlayer_IsGameModeSurvival00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cPlayer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cPlayer* self = (const cPlayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'IsGameModeSurvival'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->IsGameModeSurvival();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'IsGameModeSurvival'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: IsGameModeAdventure of class  cPlayer */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cPlayer_IsGameModeAdventure00
static int tolua_AllToLua_cPlayer_IsGameModeAdventure00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cPlayer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cPlayer* self = (const cPlayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'IsGameModeAdventure'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->IsGameModeAdventure();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'IsGameModeAdventure'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetIP of class  cPlayer */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cPlayer_GetIP00
static int tolua_AllToLua_cPlayer_GetIP00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cPlayer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cPlayer* self = (const cPlayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetIP'", NULL);
#endif
  {
   AString tolua_ret = (AString)  self->GetIP();
   tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetIP'.",&tolua_err);
 return 0;
#endif
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

/* method: GetWindow of class  cPlayer */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cPlayer_GetWindow00
static int tolua_AllToLua_cPlayer_GetWindow00(lua_State* tolua_S)
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetWindow'", NULL);
#endif
  {
   cWindow* tolua_ret = (cWindow*)  self->GetWindow();
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"cWindow");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetWindow'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: CloseWindow of class  cPlayer */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cPlayer_CloseWindow00
static int tolua_AllToLua_cPlayer_CloseWindow00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cPlayer",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,2,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cPlayer* self = (cPlayer*)  tolua_tousertype(tolua_S,1,0);
  bool a_CanRefuse = ((bool)  tolua_toboolean(tolua_S,2,true));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'CloseWindow'", NULL);
#endif
  {
   self->CloseWindow(a_CanRefuse);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'CloseWindow'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: CloseWindowIfID of class  cPlayer */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cPlayer_CloseWindowIfID00
static int tolua_AllToLua_cPlayer_CloseWindowIfID00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cPlayer",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isboolean(tolua_S,3,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cPlayer* self = (cPlayer*)  tolua_tousertype(tolua_S,1,0);
  char a_WindowID = ((char)  tolua_tonumber(tolua_S,2,0));
  bool a_CanRefuse = ((bool)  tolua_toboolean(tolua_S,3,true));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'CloseWindowIfID'", NULL);
#endif
  {
   self->CloseWindowIfID(a_WindowID,a_CanRefuse);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'CloseWindowIfID'.",&tolua_err);
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
     !tolua_isusertype(tolua_S,1,"const cPlayer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cPlayer* self = (const cPlayer*)  tolua_tousertype(tolua_S,1,0);
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
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cPlayer* self = (cPlayer*)  tolua_tousertype(tolua_S,1,0);
  const AString a_Message = ((const AString)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SendMessage'", NULL);
#endif
  {
   self->SendMessage(a_Message);
   tolua_pushcppstring(tolua_S,(const char*)a_Message);
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

/* method: GetName of class  cPlayer */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cPlayer_GetName00
static int tolua_AllToLua_cPlayer_GetName00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cPlayer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cPlayer* self = (const cPlayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetName'", NULL);
#endif
  {
   const AString tolua_ret = (const AString)  self->GetName();
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

/* method: SetName of class  cPlayer */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cPlayer_SetName00
static int tolua_AllToLua_cPlayer_SetName00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cPlayer",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cPlayer* self = (cPlayer*)  tolua_tousertype(tolua_S,1,0);
  const AString a_Name = ((const AString)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetName'", NULL);
#endif
  {
   self->SetName(a_Name);
   tolua_pushcppstring(tolua_S,(const char*)a_Name);
  }
 }
 return 1;
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
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cPlayer* self = (cPlayer*)  tolua_tousertype(tolua_S,1,0);
  const AString a_GroupName = ((const AString)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'AddToGroup'", NULL);
#endif
  {
   self->AddToGroup(a_GroupName);
   tolua_pushcppstring(tolua_S,(const char*)a_GroupName);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'AddToGroup'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: RemoveFromGroup of class  cPlayer */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cPlayer_RemoveFromGroup00
static int tolua_AllToLua_cPlayer_RemoveFromGroup00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cPlayer",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cPlayer* self = (cPlayer*)  tolua_tousertype(tolua_S,1,0);
  const AString a_GroupName = ((const AString)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'RemoveFromGroup'", NULL);
#endif
  {
   self->RemoveFromGroup(a_GroupName);
   tolua_pushcppstring(tolua_S,(const char*)a_GroupName);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'RemoveFromGroup'.",&tolua_err);
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
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cPlayer* self = (cPlayer*)  tolua_tousertype(tolua_S,1,0);
  const AString a_Command = ((const AString)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'CanUseCommand'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->CanUseCommand(a_Command);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
   tolua_pushcppstring(tolua_S,(const char*)a_Command);
  }
 }
 return 2;
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
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cPlayer* self = (cPlayer*)  tolua_tousertype(tolua_S,1,0);
  const AString a_Permission = ((const AString)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'HasPermission'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->HasPermission(a_Permission);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
   tolua_pushcppstring(tolua_S,(const char*)a_Permission);
  }
 }
 return 2;
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
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cPlayer* self = (cPlayer*)  tolua_tousertype(tolua_S,1,0);
  const AString a_Group = ((const AString)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'IsInGroup'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->IsInGroup(a_Group);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
   tolua_pushcppstring(tolua_S,(const char*)a_Group);
  }
 }
 return 2;
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
     !tolua_isusertype(tolua_S,1,"const cPlayer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cPlayer* self = (const cPlayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetColor'", NULL);
#endif
  {
   AString tolua_ret = (AString)  self->GetColor();
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
     !tolua_isnumber(tolua_S,4,1,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cPlayer* self = (cPlayer*)  tolua_tousertype(tolua_S,1,0);
  bool a_bDraggingItem = ((bool)  tolua_toboolean(tolua_S,2,0));
  char a_Amount = ((char)  tolua_tonumber(tolua_S,3,1));
  short a_CreateType = ((short)  tolua_tonumber(tolua_S,4,0));
  short a_CreateHealth = ((short)  tolua_tonumber(tolua_S,5,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'TossItem'", NULL);
#endif
  {
   self->TossItem(a_bDraggingItem,a_Amount,a_CreateType,a_CreateHealth);
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

/* method: GetFoodLevel of class  cPlayer */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cPlayer_GetFoodLevel00
static int tolua_AllToLua_cPlayer_GetFoodLevel00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cPlayer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cPlayer* self = (const cPlayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetFoodLevel'", NULL);
#endif
  {
   int tolua_ret = (int)  self->GetFoodLevel();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetFoodLevel'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetFoodSaturationLevel of class  cPlayer */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cPlayer_GetFoodSaturationLevel00
static int tolua_AllToLua_cPlayer_GetFoodSaturationLevel00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cPlayer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cPlayer* self = (const cPlayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetFoodSaturationLevel'", NULL);
#endif
  {
   double tolua_ret = (double)  self->GetFoodSaturationLevel();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetFoodSaturationLevel'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetFoodTickTimer of class  cPlayer */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cPlayer_GetFoodTickTimer00
static int tolua_AllToLua_cPlayer_GetFoodTickTimer00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cPlayer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cPlayer* self = (const cPlayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetFoodTickTimer'", NULL);
#endif
  {
   int tolua_ret = (int)  self->GetFoodTickTimer();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetFoodTickTimer'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetFoodExhaustionLevel of class  cPlayer */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cPlayer_GetFoodExhaustionLevel00
static int tolua_AllToLua_cPlayer_GetFoodExhaustionLevel00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cPlayer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cPlayer* self = (const cPlayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetFoodExhaustionLevel'", NULL);
#endif
  {
   double tolua_ret = (double)  self->GetFoodExhaustionLevel();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetFoodExhaustionLevel'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetFoodPoisonedTicksRemaining of class  cPlayer */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cPlayer_GetFoodPoisonedTicksRemaining00
static int tolua_AllToLua_cPlayer_GetFoodPoisonedTicksRemaining00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cPlayer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cPlayer* self = (const cPlayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetFoodPoisonedTicksRemaining'", NULL);
#endif
  {
   int tolua_ret = (int)  self->GetFoodPoisonedTicksRemaining();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetFoodPoisonedTicksRemaining'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: IsSatiated of class  cPlayer */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cPlayer_IsSatiated00
static int tolua_AllToLua_cPlayer_IsSatiated00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cPlayer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cPlayer* self = (const cPlayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'IsSatiated'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->IsSatiated();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'IsSatiated'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetFoodLevel of class  cPlayer */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cPlayer_SetFoodLevel00
static int tolua_AllToLua_cPlayer_SetFoodLevel00(lua_State* tolua_S)
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
  int a_FoodLevel = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetFoodLevel'", NULL);
#endif
  {
   self->SetFoodLevel(a_FoodLevel);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetFoodLevel'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetFoodSaturationLevel of class  cPlayer */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cPlayer_SetFoodSaturationLevel00
static int tolua_AllToLua_cPlayer_SetFoodSaturationLevel00(lua_State* tolua_S)
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
  double a_FoodSaturationLevel = ((double)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetFoodSaturationLevel'", NULL);
#endif
  {
   self->SetFoodSaturationLevel(a_FoodSaturationLevel);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetFoodSaturationLevel'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetFoodTickTimer of class  cPlayer */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cPlayer_SetFoodTickTimer00
static int tolua_AllToLua_cPlayer_SetFoodTickTimer00(lua_State* tolua_S)
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
  int a_FoodTickTimer = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetFoodTickTimer'", NULL);
#endif
  {
   self->SetFoodTickTimer(a_FoodTickTimer);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetFoodTickTimer'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetFoodExhaustionLevel of class  cPlayer */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cPlayer_SetFoodExhaustionLevel00
static int tolua_AllToLua_cPlayer_SetFoodExhaustionLevel00(lua_State* tolua_S)
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
  double a_FoodSaturationLevel = ((double)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetFoodExhaustionLevel'", NULL);
#endif
  {
   self->SetFoodExhaustionLevel(a_FoodSaturationLevel);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetFoodExhaustionLevel'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetFoodPoisonedTicksRemaining of class  cPlayer */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cPlayer_SetFoodPoisonedTicksRemaining00
static int tolua_AllToLua_cPlayer_SetFoodPoisonedTicksRemaining00(lua_State* tolua_S)
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
  int a_FoodPoisonedTicksRemaining = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetFoodPoisonedTicksRemaining'", NULL);
#endif
  {
   self->SetFoodPoisonedTicksRemaining(a_FoodPoisonedTicksRemaining);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetFoodPoisonedTicksRemaining'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: Feed of class  cPlayer */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cPlayer_Feed00
static int tolua_AllToLua_cPlayer_Feed00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cPlayer",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cPlayer* self = (cPlayer*)  tolua_tousertype(tolua_S,1,0);
  int a_Food = ((int)  tolua_tonumber(tolua_S,2,0));
  double a_Saturation = ((double)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'Feed'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->Feed(a_Food,a_Saturation);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Feed'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: AddFoodExhaustion of class  cPlayer */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cPlayer_AddFoodExhaustion00
static int tolua_AllToLua_cPlayer_AddFoodExhaustion00(lua_State* tolua_S)
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
  double a_Exhaustion = ((double)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'AddFoodExhaustion'", NULL);
#endif
  {
   self->AddFoodExhaustion(a_Exhaustion);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'AddFoodExhaustion'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: FoodPoison of class  cPlayer */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cPlayer_FoodPoison00
static int tolua_AllToLua_cPlayer_FoodPoison00(lua_State* tolua_S)
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
  int a_NumTicks = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'FoodPoison'", NULL);
#endif
  {
   self->FoodPoison(a_NumTicks);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'FoodPoison'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: IsEating of class  cPlayer */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cPlayer_IsEating00
static int tolua_AllToLua_cPlayer_IsEating00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cPlayer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cPlayer* self = (const cPlayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'IsEating'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->IsEating();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'IsEating'.",&tolua_err);
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
     !tolua_isusertype(tolua_S,1,"const cPlayer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cPlayer* self = (const cPlayer*)  tolua_tousertype(tolua_S,1,0);
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

/* method: MoveToWorld of class  cPlayer */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cPlayer_MoveToWorld00
static int tolua_AllToLua_cPlayer_MoveToWorld00(lua_State* tolua_S)
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
  const char* a_WorldName = ((const char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'MoveToWorld'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->MoveToWorld(a_WorldName);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'MoveToWorld'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: LoadPermissionsFromDisk of class  cPlayer */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cPlayer_LoadPermissionsFromDisk00
static int tolua_AllToLua_cPlayer_LoadPermissionsFromDisk00(lua_State* tolua_S)
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'LoadPermissionsFromDisk'", NULL);
#endif
  {
   self->LoadPermissionsFromDisk();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'LoadPermissionsFromDisk'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetMaxSpeed of class  cPlayer */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cPlayer_GetMaxSpeed00
static int tolua_AllToLua_cPlayer_GetMaxSpeed00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cPlayer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cPlayer* self = (const cPlayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetMaxSpeed'", NULL);
#endif
  {
   double tolua_ret = (double)  self->GetMaxSpeed();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetMaxSpeed'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetNormalMaxSpeed of class  cPlayer */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cPlayer_GetNormalMaxSpeed00
static int tolua_AllToLua_cPlayer_GetNormalMaxSpeed00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cPlayer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cPlayer* self = (const cPlayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetNormalMaxSpeed'", NULL);
#endif
  {
   double tolua_ret = (double)  self->GetNormalMaxSpeed();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetNormalMaxSpeed'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetSprintingMaxSpeed of class  cPlayer */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cPlayer_GetSprintingMaxSpeed00
static int tolua_AllToLua_cPlayer_GetSprintingMaxSpeed00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cPlayer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cPlayer* self = (const cPlayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetSprintingMaxSpeed'", NULL);
#endif
  {
   double tolua_ret = (double)  self->GetSprintingMaxSpeed();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetSprintingMaxSpeed'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetNormalMaxSpeed of class  cPlayer */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cPlayer_SetNormalMaxSpeed00
static int tolua_AllToLua_cPlayer_SetNormalMaxSpeed00(lua_State* tolua_S)
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
  double a_Speed = ((double)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetNormalMaxSpeed'", NULL);
#endif
  {
   self->SetNormalMaxSpeed(a_Speed);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetNormalMaxSpeed'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetSprintingMaxSpeed of class  cPlayer */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cPlayer_SetSprintingMaxSpeed00
static int tolua_AllToLua_cPlayer_SetSprintingMaxSpeed00(lua_State* tolua_S)
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
  double a_Speed = ((double)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetSprintingMaxSpeed'", NULL);
#endif
  {
   self->SetSprintingMaxSpeed(a_Speed);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetSprintingMaxSpeed'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetCrouch of class  cPlayer */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cPlayer_SetCrouch00
static int tolua_AllToLua_cPlayer_SetCrouch00(lua_State* tolua_S)
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
  bool a_IsCrouched = ((bool)  tolua_toboolean(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetCrouch'", NULL);
#endif
  {
   self->SetCrouch(a_IsCrouched);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetCrouch'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetSprint of class  cPlayer */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cPlayer_SetSprint00
static int tolua_AllToLua_cPlayer_SetSprint00(lua_State* tolua_S)
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
  bool a_IsSprinting = ((bool)  tolua_toboolean(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetSprint'", NULL);
#endif
  {
   self->SetSprint(a_IsSprinting);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetSprint'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

 class Lua__cPlayer : public cPlayer, public ToluaBase {
public:
	 void  Initialize( cWorld* a_World) {
		if (push_method("Initialize",  tolua_AllToLua_cPlayer_Initialize00)) {
			tolua_pushusertype(lua_state, (void*)a_World, "cWorld");
			ToluaBase::dbcall(lua_state, 2, 0);
		} else {
			return ( void ) cPlayer:: Initialize(a_World);
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
	 bool  IsA( const char* a_ClassName)const {
		if (push_method("IsA",  tolua_AllToLua_cEntity_IsA00)) {
			tolua_pushstring(lua_state, (const char*)a_ClassName);
			ToluaBase::dbcall(lua_state, 2, 1);
			 bool  tolua_ret = ( bool )tolua_toboolean(lua_state, -1, 0);
			lua_pop(lua_state, 1);
			return tolua_ret;
		} else {
			return ( bool ) cPlayer:: IsA(a_ClassName);
		};
	};
	 const char*  GetClass( void )const {
		if (push_method("GetClass",  tolua_AllToLua_cEntity_GetClass00)) {
			ToluaBase::dbcall(lua_state, 1, 1);
			 const char*  tolua_ret = ( const char* )tolua_tostring(lua_state, -1, 0);
			lua_pop(lua_state, 1);
			return tolua_ret;
		} else {
			return ( const char* ) cPlayer:: GetClass();
		};
	};
	 const char*  GetParentClass( void )const {
		if (push_method("GetParentClass",  tolua_AllToLua_cEntity_GetParentClass00)) {
			ToluaBase::dbcall(lua_state, 1, 1);
			 const char*  tolua_ret = ( const char* )tolua_tostring(lua_state, -1, 0);
			lua_pop(lua_state, 1);
			return tolua_ret;
		} else {
			return ( const char* ) cPlayer:: GetParentClass();
		};
	};
	 int  GetRawDamageAgainst( const cEntity& a_Receiver) {
		if (push_method("GetRawDamageAgainst",  tolua_AllToLua_cEntity_GetRawDamageAgainst00)) {
			tolua_pushusertype(lua_state, (void*)&a_Receiver, "const cEntity");
			ToluaBase::dbcall(lua_state, 2, 1);
			 int  tolua_ret = ( int )tolua_tonumber(lua_state, -1, 0);
			lua_pop(lua_state, 1);
			return tolua_ret;
		} else {
			return ( int ) cPlayer:: GetRawDamageAgainst(a_Receiver);
		};
	};
	 int  GetArmorCoverAgainst( const cEntity* a_Attacker, eDamageType a_DamageType, int a_RawDamage) {
		if (push_method("GetArmorCoverAgainst",  tolua_AllToLua_cEntity_GetArmorCoverAgainst00)) {
			tolua_pushusertype(lua_state, (void*)a_Attacker, "const cEntity");
			tolua_pushnumber(lua_state, (lua_Number)a_DamageType);
			tolua_pushnumber(lua_state, (lua_Number)a_RawDamage);
			ToluaBase::dbcall(lua_state, 4, 1);
			 int  tolua_ret = ( int )tolua_tonumber(lua_state, -1, 0);
			lua_pop(lua_state, 1);
			return tolua_ret;
		} else {
			return ( int ) cPlayer:: GetArmorCoverAgainst(a_Attacker,a_DamageType,a_RawDamage);
		};
	};
	 double  GetKnockbackAmountAgainst( const cEntity& a_Receiver) {
		if (push_method("GetKnockbackAmountAgainst",  tolua_AllToLua_cEntity_GetKnockbackAmountAgainst00)) {
			tolua_pushusertype(lua_state, (void*)&a_Receiver, "const cEntity");
			ToluaBase::dbcall(lua_state, 2, 1);
			 double  tolua_ret = ( double )tolua_tonumber(lua_state, -1, 0);
			lua_pop(lua_state, 1);
			return tolua_ret;
		} else {
			return ( double ) cPlayer:: GetKnockbackAmountAgainst(a_Receiver);
		};
	};
	 cItem  GetEquippedWeapon( void )const {
		if (push_method("GetEquippedWeapon",  tolua_AllToLua_cEntity_GetEquippedWeapon00)) {
			ToluaBase::dbcall(lua_state, 1, 1);
			 cItem  tolua_ret = ( cItem )*(cItem*)tolua_tousertype(lua_state, -1, 0);
			lua_pop(lua_state, 1);
			return tolua_ret;
		} else {
			return ( cItem ) cPlayer:: GetEquippedWeapon();
		};
	};
	 cItem  GetEquippedHelmet( void )const {
		if (push_method("GetEquippedHelmet",  tolua_AllToLua_cEntity_GetEquippedHelmet00)) {
			ToluaBase::dbcall(lua_state, 1, 1);
			 cItem  tolua_ret = ( cItem )*(cItem*)tolua_tousertype(lua_state, -1, 0);
			lua_pop(lua_state, 1);
			return tolua_ret;
		} else {
			return ( cItem ) cPlayer:: GetEquippedHelmet();
		};
	};
	 cItem  GetEquippedChestplate( void )const {
		if (push_method("GetEquippedChestplate",  tolua_AllToLua_cEntity_GetEquippedChestplate00)) {
			ToluaBase::dbcall(lua_state, 1, 1);
			 cItem  tolua_ret = ( cItem )*(cItem*)tolua_tousertype(lua_state, -1, 0);
			lua_pop(lua_state, 1);
			return tolua_ret;
		} else {
			return ( cItem ) cPlayer:: GetEquippedChestplate();
		};
	};
	 cItem  GetEquippedLeggings( void )const {
		if (push_method("GetEquippedLeggings",  tolua_AllToLua_cEntity_GetEquippedLeggings00)) {
			ToluaBase::dbcall(lua_state, 1, 1);
			 cItem  tolua_ret = ( cItem )*(cItem*)tolua_tousertype(lua_state, -1, 0);
			lua_pop(lua_state, 1);
			return tolua_ret;
		} else {
			return ( cItem ) cPlayer:: GetEquippedLeggings();
		};
	};
	 cItem  GetEquippedBoots( void )const {
		if (push_method("GetEquippedBoots",  tolua_AllToLua_cEntity_GetEquippedBoots00)) {
			ToluaBase::dbcall(lua_state, 1, 1);
			 cItem  tolua_ret = ( cItem )*(cItem*)tolua_tousertype(lua_state, -1, 0);
			lua_pop(lua_state, 1);
			return tolua_ret;
		} else {
			return ( cItem ) cPlayer:: GetEquippedBoots();
		};
	};
	 void  KilledBy( cEntity* a_Killer) {
		if (push_method("KilledBy",  tolua_AllToLua_cEntity_KilledBy00)) {
			tolua_pushusertype(lua_state, (void*)a_Killer, "cEntity");
			ToluaBase::dbcall(lua_state, 2, 0);
		} else {
			return ( void ) cPlayer:: KilledBy(a_Killer);
		};
	};
	 void  TeleportToEntity( cEntity& a_Entity) {
		if (push_method("TeleportToEntity",  tolua_AllToLua_cEntity_TeleportToEntity00)) {
			tolua_pushusertype(lua_state, (void*)&a_Entity, "cEntity");
			ToluaBase::dbcall(lua_state, 2, 0);
		} else {
			return ( void ) cPlayer:: TeleportToEntity(a_Entity);
		};
	};
	 void  TeleportToCoords( double a_PosX, double a_PosY, double a_PosZ) {
		if (push_method("TeleportToCoords",  tolua_AllToLua_cEntity_TeleportToCoords00)) {
			tolua_pushnumber(lua_state, (lua_Number)a_PosX);
			tolua_pushnumber(lua_state, (lua_Number)a_PosY);
			tolua_pushnumber(lua_state, (lua_Number)a_PosZ);
			ToluaBase::dbcall(lua_state, 4, 0);
		} else {
			return ( void ) cPlayer:: TeleportToCoords(a_PosX,a_PosY,a_PosZ);
		};
	};
	 bool  IsOnFire( void )const {
		if (push_method("IsOnFire",  tolua_AllToLua_cEntity_IsOnFire00)) {
			ToluaBase::dbcall(lua_state, 1, 1);
			 bool  tolua_ret = ( bool )tolua_toboolean(lua_state, -1, 0);
			lua_pop(lua_state, 1);
			return tolua_ret;
		} else {
			return ( bool ) cPlayer:: IsOnFire();
		};
	};
	 bool  IsCrouched( void )const {
		if (push_method("IsCrouched",  tolua_AllToLua_cEntity_IsCrouched00)) {
			ToluaBase::dbcall(lua_state, 1, 1);
			 bool  tolua_ret = ( bool )tolua_toboolean(lua_state, -1, 0);
			lua_pop(lua_state, 1);
			return tolua_ret;
		} else {
			return ( bool ) cPlayer:: IsCrouched();
		};
	};
	 bool  IsRiding( void )const {
		if (push_method("IsRiding",  tolua_AllToLua_cEntity_IsRiding00)) {
			ToluaBase::dbcall(lua_state, 1, 1);
			 bool  tolua_ret = ( bool )tolua_toboolean(lua_state, -1, 0);
			lua_pop(lua_state, 1);
			return tolua_ret;
		} else {
			return ( bool ) cPlayer:: IsRiding();
		};
	};
	 bool  IsSprinting( void )const {
		if (push_method("IsSprinting",  tolua_AllToLua_cEntity_IsSprinting00)) {
			ToluaBase::dbcall(lua_state, 1, 1);
			 bool  tolua_ret = ( bool )tolua_toboolean(lua_state, -1, 0);
			lua_pop(lua_state, 1);
			return tolua_ret;
		} else {
			return ( bool ) cPlayer:: IsSprinting();
		};
	};
	 bool  IsRclking( void )const {
		if (push_method("IsRclking",  tolua_AllToLua_cEntity_IsRclking00)) {
			ToluaBase::dbcall(lua_state, 1, 1);
			 bool  tolua_ret = ( bool )tolua_toboolean(lua_state, -1, 0);
			lua_pop(lua_state, 1);
			return tolua_ret;
		} else {
			return ( bool ) cPlayer:: IsRclking();
		};
	};

	 void cPlayer__Initialize( cWorld* a_World) {
		return ( void )cPlayer::Initialize(a_World);
	};
	 void cPlayer__MoveTo( const Vector3d& a_NewPos) {
		return ( void )cPlayer::MoveTo(a_NewPos);
	};
	 bool cPlayer__IsA( const char* a_ClassName) {
		return ( bool )cPlayer::IsA(a_ClassName);
	};
	 const char* cPlayer__GetClass( void ) {
		return ( const char* )cPlayer::GetClass();
	};
	 const char* cPlayer__GetParentClass( void ) {
		return ( const char* )cPlayer::GetParentClass();
	};
	 int cPlayer__GetRawDamageAgainst( const cEntity& a_Receiver) {
		return ( int )cPlayer::GetRawDamageAgainst(a_Receiver);
	};
	 int cPlayer__GetArmorCoverAgainst( const cEntity* a_Attacker, eDamageType a_DamageType, int a_RawDamage) {
		return ( int )cPlayer::GetArmorCoverAgainst(a_Attacker,a_DamageType,a_RawDamage);
	};
	 double cPlayer__GetKnockbackAmountAgainst( const cEntity& a_Receiver) {
		return ( double )cPlayer::GetKnockbackAmountAgainst(a_Receiver);
	};
	 cItem cPlayer__GetEquippedWeapon( void ) {
		return ( cItem )cPlayer::GetEquippedWeapon();
	};
	 cItem cPlayer__GetEquippedHelmet( void ) {
		return ( cItem )cPlayer::GetEquippedHelmet();
	};
	 cItem cPlayer__GetEquippedChestplate( void ) {
		return ( cItem )cPlayer::GetEquippedChestplate();
	};
	 cItem cPlayer__GetEquippedLeggings( void ) {
		return ( cItem )cPlayer::GetEquippedLeggings();
	};
	 cItem cPlayer__GetEquippedBoots( void ) {
		return ( cItem )cPlayer::GetEquippedBoots();
	};
	 void cPlayer__KilledBy( cEntity* a_Killer) {
		return ( void )cPlayer::KilledBy(a_Killer);
	};
	 void cPlayer__TeleportToEntity( cEntity& a_Entity) {
		return ( void )cPlayer::TeleportToEntity(a_Entity);
	};
	 void cPlayer__TeleportToCoords( double a_PosX, double a_PosY, double a_PosZ) {
		return ( void )cPlayer::TeleportToCoords(a_PosX,a_PosY,a_PosZ);
	};
	 bool cPlayer__IsOnFire( void ) {
		return ( bool )cPlayer::IsOnFire();
	};
	 bool cPlayer__IsCrouched( void ) {
		return ( bool )cPlayer::IsCrouched();
	};
	 bool cPlayer__IsRiding( void ) {
		return ( bool )cPlayer::IsRiding();
	};
	 bool cPlayer__IsSprinting( void ) {
		return ( bool )cPlayer::IsSprinting();
	};
	 bool cPlayer__IsRclking( void ) {
		return ( bool )cPlayer::IsRclking();
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

/* method: cPlayer__Initialize of class  Lua__cPlayer */
#ifndef TOLUA_DISABLE_tolua_AllToLua_Lua__cPlayer_cPlayer__Initialize00
static int tolua_AllToLua_Lua__cPlayer_cPlayer__Initialize00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Lua__cPlayer",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"cWorld",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Lua__cPlayer* self = (Lua__cPlayer*)  tolua_tousertype(tolua_S,1,0);
  cWorld* a_World = ((cWorld*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'cPlayer__Initialize'", NULL);
#endif
  {
   self->cPlayer__Initialize(a_World);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'cPlayer__Initialize'.",&tolua_err);
 return 0;
#endif
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

/* method: Get of class  cPluginManager */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cPluginManager_Get00
static int tolua_AllToLua_cPluginManager_Get00(lua_State* tolua_S)
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
   cPluginManager* tolua_ret = (cPluginManager*)  cPluginManager::Get();
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"cPluginManager");
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

/* method: GetPlugin of class  cPluginManager */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cPluginManager_GetPlugin00
static int tolua_AllToLua_cPluginManager_GetPlugin00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cPluginManager",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cPluginManager* self = (const cPluginManager*)  tolua_tousertype(tolua_S,1,0);
  const AString a_Plugin = ((const AString)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetPlugin'", NULL);
#endif
  {
   cPlugin* tolua_ret = (cPlugin*)  self->GetPlugin(a_Plugin);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"cPlugin");
   tolua_pushcppstring(tolua_S,(const char*)a_Plugin);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetPlugin'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: FindPlugins of class  cPluginManager */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cPluginManager_FindPlugins00
static int tolua_AllToLua_cPluginManager_FindPlugins00(lua_State* tolua_S)
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'FindPlugins'", NULL);
#endif
  {
   self->FindPlugins();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'FindPlugins'.",&tolua_err);
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
     !tolua_isusertype(tolua_S,1,"const cPluginManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cPluginManager* self = (const cPluginManager*)  tolua_tousertype(tolua_S,1,0);
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

/* method: DisablePlugin of class  cPluginManager */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cPluginManager_DisablePlugin00
static int tolua_AllToLua_cPluginManager_DisablePlugin00(lua_State* tolua_S)
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
  const AString a_PluginName = ((const AString)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'DisablePlugin'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->DisablePlugin(a_PluginName);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
   tolua_pushcppstring(tolua_S,(const char*)a_PluginName);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'DisablePlugin'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: LoadPlugin of class  cPluginManager */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cPluginManager_LoadPlugin00
static int tolua_AllToLua_cPluginManager_LoadPlugin00(lua_State* tolua_S)
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
  const AString a_PluginName = ((const AString)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'LoadPlugin'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->LoadPlugin(a_PluginName);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
   tolua_pushcppstring(tolua_S,(const char*)a_PluginName);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'LoadPlugin'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: IsCommandBound of class  cPluginManager */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cPluginManager_IsCommandBound00
static int tolua_AllToLua_cPluginManager_IsCommandBound00(lua_State* tolua_S)
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
  const AString a_Command = ((const AString)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'IsCommandBound'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->IsCommandBound(a_Command);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
   tolua_pushcppstring(tolua_S,(const char*)a_Command);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'IsCommandBound'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetCommandPermission of class  cPluginManager */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cPluginManager_GetCommandPermission00
static int tolua_AllToLua_cPluginManager_GetCommandPermission00(lua_State* tolua_S)
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
  const AString a_Command = ((const AString)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetCommandPermission'", NULL);
#endif
  {
   AString tolua_ret = (AString)  self->GetCommandPermission(a_Command);
   tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
   tolua_pushcppstring(tolua_S,(const char*)a_Command);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetCommandPermission'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: ExecuteCommand of class  cPluginManager */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cPluginManager_ExecuteCommand00
static int tolua_AllToLua_cPluginManager_ExecuteCommand00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cPluginManager",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"cPlayer",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cPluginManager* self = (cPluginManager*)  tolua_tousertype(tolua_S,1,0);
  cPlayer* a_Player = ((cPlayer*)  tolua_tousertype(tolua_S,2,0));
  const AString a_Command = ((const AString)  tolua_tocppstring(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'ExecuteCommand'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->ExecuteCommand(a_Player,a_Command);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
   tolua_pushcppstring(tolua_S,(const char*)a_Command);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'ExecuteCommand'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: ForceExecuteCommand of class  cPluginManager */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cPluginManager_ForceExecuteCommand00
static int tolua_AllToLua_cPluginManager_ForceExecuteCommand00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cPluginManager",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"cPlayer",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cPluginManager* self = (cPluginManager*)  tolua_tousertype(tolua_S,1,0);
  cPlayer* a_Player = ((cPlayer*)  tolua_tousertype(tolua_S,2,0));
  const AString a_Command = ((const AString)  tolua_tocppstring(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'ForceExecuteCommand'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->ForceExecuteCommand(a_Player,a_Command);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
   tolua_pushcppstring(tolua_S,(const char*)a_Command);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'ForceExecuteCommand'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: IsConsoleCommandBound of class  cPluginManager */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cPluginManager_IsConsoleCommandBound00
static int tolua_AllToLua_cPluginManager_IsConsoleCommandBound00(lua_State* tolua_S)
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
  const AString a_Command = ((const AString)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'IsConsoleCommandBound'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->IsConsoleCommandBound(a_Command);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
   tolua_pushcppstring(tolua_S,(const char*)a_Command);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'IsConsoleCommandBound'.",&tolua_err);
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
   const AString tolua_ret = (const AString)  self->GetName();
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
  const AString a_Name = ((const AString)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetName'", NULL);
#endif
  {
   self->SetName(a_Name);
   tolua_pushcppstring(tolua_S,(const char*)a_Name);
  }
 }
 return 1;
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

/* method: GetDirectory of class  cPlugin */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cPlugin_GetDirectory00
static int tolua_AllToLua_cPlugin_GetDirectory00(lua_State* tolua_S)
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetDirectory'", NULL);
#endif
  {
   const AString tolua_ret = (const AString)  self->GetDirectory();
   tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetDirectory'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetLocalDirectory of class  cPlugin */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cPlugin_GetLocalDirectory00
static int tolua_AllToLua_cPlugin_GetLocalDirectory00(lua_State* tolua_S)
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetLocalDirectory'", NULL);
#endif
  {
   AString tolua_ret = (AString)  self->GetLocalDirectory();
   tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetLocalDirectory'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* get function: __cWebPlugin__ of class  cPlugin_NewLua */
#ifndef TOLUA_DISABLE_tolua_get_cPlugin_NewLua___cWebPlugin__
static int tolua_get_cPlugin_NewLua___cWebPlugin__(lua_State* tolua_S)
{
  cPlugin_NewLua* self = (cPlugin_NewLua*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable '__cWebPlugin__'",NULL);
#endif
#ifdef __cplusplus
   tolua_pushusertype(tolua_S,(void*)static_cast<cWebPlugin*>(self), "cWebPlugin");
#else
   tolua_pushusertype(tolua_S,(void*)((cWebPlugin*)self), "cWebPlugin");
#endif
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* method: BroadcastChat of class  cServer */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cServer_BroadcastChat00
static int tolua_AllToLua_cServer_BroadcastChat00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cServer",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"const cClientHandle",1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cServer* self = (cServer*)  tolua_tousertype(tolua_S,1,0);
  const AString a_Message = ((const AString)  tolua_tocppstring(tolua_S,2,0));
  const cClientHandle* a_Exclude = ((const cClientHandle*)  tolua_tousertype(tolua_S,3,NULL));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'BroadcastChat'", NULL);
#endif
  {
   self->BroadcastChat(a_Message,a_Exclude);
   tolua_pushcppstring(tolua_S,(const char*)a_Message);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'BroadcastChat'.",&tolua_err);
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
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"cPlayer",1,&tolua_err) ||
     !tolua_isboolean(tolua_S,4,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cServer* self = (cServer*)  tolua_tousertype(tolua_S,1,0);
  const AString a_Message = ((const AString)  tolua_tocppstring(tolua_S,2,0));
  cPlayer* a_Player = ((cPlayer*)  tolua_tousertype(tolua_S,3,NULL));
  bool a_bExclude = ((bool)  tolua_toboolean(tolua_S,4,false));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SendMessage'", NULL);
#endif
  {
   self->SendMessage(a_Message,a_Player,a_bExclude);
   tolua_pushcppstring(tolua_S,(const char*)a_Message);
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

/* method: GetServerID of class  cServer */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cServer_GetServerID00
static int tolua_AllToLua_cServer_GetServerID00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cServer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cServer* self = (const cServer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetServerID'", NULL);
#endif
  {
   const AString tolua_ret = (const AString)  self->GetServerID();
   tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetServerID'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetClassStatic of class  cWorld */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cWorld_GetClassStatic00
static int tolua_AllToLua_cWorld_GetClassStatic00(lua_State* tolua_S)
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
   const char* tolua_ret = (const char*)  cWorld::GetClassStatic();
   tolua_pushstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetClassStatic'.",&tolua_err);
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

/* method: GetTicksUntilWeatherChange of class  cWorld */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cWorld_GetTicksUntilWeatherChange00
static int tolua_AllToLua_cWorld_GetTicksUntilWeatherChange00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cWorld",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cWorld* self = (const cWorld*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetTicksUntilWeatherChange'", NULL);
#endif
  {
   int tolua_ret = (int)  self->GetTicksUntilWeatherChange();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetTicksUntilWeatherChange'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetWorldAge of class  cWorld */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cWorld_GetWorldAge00
static int tolua_AllToLua_cWorld_GetWorldAge00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cWorld",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cWorld* self = (const cWorld*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetWorldAge'", NULL);
#endif
  {
   long long tolua_ret = ( long long)  self->GetWorldAge();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetWorldAge'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetTimeOfDay of class  cWorld */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cWorld_GetTimeOfDay00
static int tolua_AllToLua_cWorld_GetTimeOfDay00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cWorld",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cWorld* self = (const cWorld*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetTimeOfDay'", NULL);
#endif
  {
   long long tolua_ret = ( long long)  self->GetTimeOfDay();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetTimeOfDay'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetTicksUntilWeatherChange of class  cWorld */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cWorld_SetTicksUntilWeatherChange00
static int tolua_AllToLua_cWorld_SetTicksUntilWeatherChange00(lua_State* tolua_S)
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
  int a_WeatherInterval = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetTicksUntilWeatherChange'", NULL);
#endif
  {
   self->SetTicksUntilWeatherChange(a_WeatherInterval);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetTicksUntilWeatherChange'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetTimeOfDay of class  cWorld */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cWorld_SetTimeOfDay00
static int tolua_AllToLua_cWorld_SetTimeOfDay00(lua_State* tolua_S)
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
  long long a_TimeOfDay = (( long long)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetTimeOfDay'", NULL);
#endif
  {
   self->SetTimeOfDay(a_TimeOfDay);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetTimeOfDay'.",&tolua_err);
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
  long long a_TimeOfDay = (( long long)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetWorldTime'", NULL);
#endif
  {
   self->SetWorldTime(a_TimeOfDay);
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

/* method: GetGameMode of class  cWorld */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cWorld_GetGameMode00
static int tolua_AllToLua_cWorld_GetGameMode00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cWorld",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cWorld* self = (const cWorld*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetGameMode'", NULL);
#endif
  {
   eGameMode tolua_ret = (eGameMode)  self->GetGameMode();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetGameMode'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: IsGameModeCreative of class  cWorld */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cWorld_IsGameModeCreative00
static int tolua_AllToLua_cWorld_IsGameModeCreative00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cWorld",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cWorld* self = (const cWorld*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'IsGameModeCreative'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->IsGameModeCreative();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'IsGameModeCreative'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: IsGameModeSurvival of class  cWorld */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cWorld_IsGameModeSurvival00
static int tolua_AllToLua_cWorld_IsGameModeSurvival00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cWorld",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cWorld* self = (const cWorld*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'IsGameModeSurvival'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->IsGameModeSurvival();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'IsGameModeSurvival'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: IsGameModeAdventure of class  cWorld */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cWorld_IsGameModeAdventure00
static int tolua_AllToLua_cWorld_IsGameModeAdventure00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cWorld",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cWorld* self = (const cWorld*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'IsGameModeAdventure'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->IsGameModeAdventure();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'IsGameModeAdventure'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: IsPVPEnabled of class  cWorld */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cWorld_IsPVPEnabled00
static int tolua_AllToLua_cWorld_IsPVPEnabled00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cWorld",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cWorld* self = (const cWorld*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'IsPVPEnabled'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->IsPVPEnabled();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'IsPVPEnabled'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: IsDeepSnowEnabled of class  cWorld */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cWorld_IsDeepSnowEnabled00
static int tolua_AllToLua_cWorld_IsDeepSnowEnabled00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cWorld",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cWorld* self = (const cWorld*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'IsDeepSnowEnabled'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->IsDeepSnowEnabled();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'IsDeepSnowEnabled'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetDimension of class  cWorld */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cWorld_GetDimension00
static int tolua_AllToLua_cWorld_GetDimension00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cWorld",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cWorld* self = (const cWorld*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetDimension'", NULL);
#endif
  {
   eDimension tolua_ret = (eDimension)  self->GetDimension();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetDimension'.",&tolua_err);
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
  int a_BlockX = ((int)  tolua_tonumber(tolua_S,2,0));
  int a_BlockZ = ((int)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetHeight'", NULL);
#endif
  {
   int tolua_ret = (int)  self->GetHeight(a_BlockX,a_BlockZ);
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

/* method: UnloadUnusedChunks of class  cWorld */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cWorld_UnloadUnusedChunks00
static int tolua_AllToLua_cWorld_UnloadUnusedChunks00(lua_State* tolua_S)
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'UnloadUnusedChunks'", NULL);
#endif
  {
   self->UnloadUnusedChunks();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'UnloadUnusedChunks'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetMaxPlayers of class  cWorld */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cWorld_GetMaxPlayers00
static int tolua_AllToLua_cWorld_GetMaxPlayers00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cWorld",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cWorld* self = (const cWorld*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetMaxPlayers'", NULL);
#endif
  {
   unsigned int tolua_ret = (unsigned int)  self->GetMaxPlayers();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetMaxPlayers'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetMaxPlayers of class  cWorld */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cWorld_SetMaxPlayers00
static int tolua_AllToLua_cWorld_SetMaxPlayers00(lua_State* tolua_S)
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
  int iMax = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetMaxPlayers'", NULL);
#endif
  {
   self->SetMaxPlayers(iMax);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetMaxPlayers'.",&tolua_err);
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

/* method: RegenerateChunk of class  cWorld */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cWorld_RegenerateChunk00
static int tolua_AllToLua_cWorld_RegenerateChunk00(lua_State* tolua_S)
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
  int a_ChunkX = ((int)  tolua_tonumber(tolua_S,2,0));
  int a_ChunkZ = ((int)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'RegenerateChunk'", NULL);
#endif
  {
   self->RegenerateChunk(a_ChunkX,a_ChunkZ);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'RegenerateChunk'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GenerateChunk of class  cWorld */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cWorld_GenerateChunk00
static int tolua_AllToLua_cWorld_GenerateChunk00(lua_State* tolua_S)
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
  int a_ChunkX = ((int)  tolua_tonumber(tolua_S,2,0));
  int a_ChunkZ = ((int)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GenerateChunk'", NULL);
#endif
  {
   self->GenerateChunk(a_ChunkX,a_ChunkZ);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GenerateChunk'.",&tolua_err);
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
  int a_BlockX = ((int)  tolua_tonumber(tolua_S,2,0));
  int a_BlockY = ((int)  tolua_tonumber(tolua_S,3,0));
  int a_BlockZ = ((int)  tolua_tonumber(tolua_S,4,0));
  unsigned char a_BlockType = (( unsigned char)  tolua_tonumber(tolua_S,5,0));
  unsigned char a_BlockMeta = (( unsigned char)  tolua_tonumber(tolua_S,6,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetBlock'", NULL);
#endif
  {
   self->SetBlock(a_BlockX,a_BlockY,a_BlockZ,a_BlockType,a_BlockMeta);
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
  int a_BlockX = ((int)  tolua_tonumber(tolua_S,2,0));
  int a_BlockY = ((int)  tolua_tonumber(tolua_S,3,0));
  int a_BlockZ = ((int)  tolua_tonumber(tolua_S,4,0));
  unsigned char a_BlockType = (( unsigned char)  tolua_tonumber(tolua_S,5,0));
  unsigned char a_BlockMeta = (( unsigned char)  tolua_tonumber(tolua_S,6,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'FastSetBlock'", NULL);
#endif
  {
   self->FastSetBlock(a_BlockX,a_BlockY,a_BlockZ,a_BlockType,a_BlockMeta);
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
  int a_BlockX = ((int)  tolua_tonumber(tolua_S,2,0));
  int a_BlockY = ((int)  tolua_tonumber(tolua_S,3,0));
  int a_BlockZ = ((int)  tolua_tonumber(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetBlock'", NULL);
#endif
  {
   unsigned char tolua_ret = ( unsigned char)  self->GetBlock(a_BlockX,a_BlockY,a_BlockZ);
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
  int a_BlockX = ((int)  tolua_tonumber(tolua_S,2,0));
  int a_BlockY = ((int)  tolua_tonumber(tolua_S,3,0));
  int a_BlockZ = ((int)  tolua_tonumber(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetBlockMeta'", NULL);
#endif
  {
   unsigned char tolua_ret = ( unsigned char)  self->GetBlockMeta(a_BlockX,a_BlockY,a_BlockZ);
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
  int a_BlockX = ((int)  tolua_tonumber(tolua_S,2,0));
  int a_BlockY = ((int)  tolua_tonumber(tolua_S,3,0));
  int a_BlockZ = ((int)  tolua_tonumber(tolua_S,4,0));
  unsigned char a_MetaData = (( unsigned char)  tolua_tonumber(tolua_S,5,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetBlockMeta'", NULL);
#endif
  {
   self->SetBlockMeta(a_BlockX,a_BlockY,a_BlockZ,a_MetaData);
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

/* method: GetBlockSkyLight of class  cWorld */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cWorld_GetBlockSkyLight00
static int tolua_AllToLua_cWorld_GetBlockSkyLight00(lua_State* tolua_S)
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
  int a_BlockX = ((int)  tolua_tonumber(tolua_S,2,0));
  int a_BlockY = ((int)  tolua_tonumber(tolua_S,3,0));
  int a_BlockZ = ((int)  tolua_tonumber(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetBlockSkyLight'", NULL);
#endif
  {
   unsigned char tolua_ret = ( unsigned char)  self->GetBlockSkyLight(a_BlockX,a_BlockY,a_BlockZ);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetBlockSkyLight'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetBlockBlockLight of class  cWorld */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cWorld_GetBlockBlockLight00
static int tolua_AllToLua_cWorld_GetBlockBlockLight00(lua_State* tolua_S)
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
  int a_BlockX = ((int)  tolua_tonumber(tolua_S,2,0));
  int a_BlockY = ((int)  tolua_tonumber(tolua_S,3,0));
  int a_BlockZ = ((int)  tolua_tonumber(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetBlockBlockLight'", NULL);
#endif
  {
   unsigned char tolua_ret = ( unsigned char)  self->GetBlockBlockLight(a_BlockX,a_BlockY,a_BlockZ);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetBlockBlockLight'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetBlockTypeMeta of class  cWorld */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cWorld_GetBlockTypeMeta00
static int tolua_AllToLua_cWorld_GetBlockTypeMeta00(lua_State* tolua_S)
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
  int a_BlockX = ((int)  tolua_tonumber(tolua_S,2,0));
  int a_BlockY = ((int)  tolua_tonumber(tolua_S,3,0));
  int a_BlockZ = ((int)  tolua_tonumber(tolua_S,4,0));
  unsigned char a_BlockType = (( unsigned char)  tolua_tonumber(tolua_S,5,0));
  unsigned char a_BlockMeta = (( unsigned char)  tolua_tonumber(tolua_S,6,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetBlockTypeMeta'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->GetBlockTypeMeta(a_BlockX,a_BlockY,a_BlockZ,a_BlockType,a_BlockMeta);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
   tolua_pushnumber(tolua_S,(lua_Number)a_BlockType);
   tolua_pushnumber(tolua_S,(lua_Number)a_BlockMeta);
  }
 }
 return 3;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetBlockTypeMeta'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetBlockInfo of class  cWorld */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cWorld_GetBlockInfo00
static int tolua_AllToLua_cWorld_GetBlockInfo00(lua_State* tolua_S)
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
     !tolua_isnumber(tolua_S,7,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,8,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,9,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cWorld* self = (cWorld*)  tolua_tousertype(tolua_S,1,0);
  int a_BlockX = ((int)  tolua_tonumber(tolua_S,2,0));
  int a_BlockY = ((int)  tolua_tonumber(tolua_S,3,0));
  int a_BlockZ = ((int)  tolua_tonumber(tolua_S,4,0));
  unsigned char a_BlockType = (( unsigned char)  tolua_tonumber(tolua_S,5,0));
  unsigned char a_Meta = (( unsigned char)  tolua_tonumber(tolua_S,6,0));
  unsigned char a_SkyLight = (( unsigned char)  tolua_tonumber(tolua_S,7,0));
  unsigned char a_BlockLight = (( unsigned char)  tolua_tonumber(tolua_S,8,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetBlockInfo'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->GetBlockInfo(a_BlockX,a_BlockY,a_BlockZ,a_BlockType,a_Meta,a_SkyLight,a_BlockLight);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
   tolua_pushnumber(tolua_S,(lua_Number)a_BlockType);
   tolua_pushnumber(tolua_S,(lua_Number)a_Meta);
   tolua_pushnumber(tolua_S,(lua_Number)a_SkyLight);
   tolua_pushnumber(tolua_S,(lua_Number)a_BlockLight);
  }
 }
 return 5;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetBlockInfo'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: FastSetBlock of class  cWorld */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cWorld_FastSetBlock01
static int tolua_AllToLua_cWorld_FastSetBlock01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cWorld",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const Vector3i",0,&tolua_err)) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  cWorld* self = (cWorld*)  tolua_tousertype(tolua_S,1,0);
  const Vector3i* a_Pos = ((const Vector3i*)  tolua_tousertype(tolua_S,2,0));
  unsigned char a_BlockType = (( unsigned char)  tolua_tonumber(tolua_S,3,0));
  unsigned char a_BlockMeta = (( unsigned char)  tolua_tonumber(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'FastSetBlock'", NULL);
#endif
  {
   self->FastSetBlock(*a_Pos,a_BlockType,a_BlockMeta);
  }
 }
 return 0;
tolua_lerror:
 return tolua_AllToLua_cWorld_FastSetBlock00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetBlock of class  cWorld */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cWorld_GetBlock01
static int tolua_AllToLua_cWorld_GetBlock01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cWorld",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const Vector3i",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  cWorld* self = (cWorld*)  tolua_tousertype(tolua_S,1,0);
  const Vector3i* a_Pos = ((const Vector3i*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetBlock'", NULL);
#endif
  {
   unsigned char tolua_ret = ( unsigned char)  self->GetBlock(*a_Pos);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
tolua_lerror:
 return tolua_AllToLua_cWorld_GetBlock00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetBlockMeta of class  cWorld */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cWorld_GetBlockMeta01
static int tolua_AllToLua_cWorld_GetBlockMeta01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cWorld",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const Vector3i",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  cWorld* self = (cWorld*)  tolua_tousertype(tolua_S,1,0);
  const Vector3i* a_Pos = ((const Vector3i*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetBlockMeta'", NULL);
#endif
  {
   unsigned char tolua_ret = ( unsigned char)  self->GetBlockMeta(*a_Pos);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
tolua_lerror:
 return tolua_AllToLua_cWorld_GetBlockMeta00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetBlockMeta of class  cWorld */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cWorld_SetBlockMeta01
static int tolua_AllToLua_cWorld_SetBlockMeta01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cWorld",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const Vector3i",0,&tolua_err)) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  cWorld* self = (cWorld*)  tolua_tousertype(tolua_S,1,0);
  const Vector3i* a_Pos = ((const Vector3i*)  tolua_tousertype(tolua_S,2,0));
  unsigned char a_MetaData = (( unsigned char)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetBlockMeta'", NULL);
#endif
  {
   self->SetBlockMeta(*a_Pos,a_MetaData);
  }
 }
 return 0;
tolua_lerror:
 return tolua_AllToLua_cWorld_SetBlockMeta00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: SpawnItemPickups of class  cWorld */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cWorld_SpawnItemPickups00
static int tolua_AllToLua_cWorld_SpawnItemPickups00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cWorld",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const cItems",0,&tolua_err)) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,6,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,7,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cWorld* self = (cWorld*)  tolua_tousertype(tolua_S,1,0);
  const cItems* a_Pickups = ((const cItems*)  tolua_tousertype(tolua_S,2,0));
  double a_BlockX = ((double)  tolua_tonumber(tolua_S,3,0));
  double a_BlockY = ((double)  tolua_tonumber(tolua_S,4,0));
  double a_BlockZ = ((double)  tolua_tonumber(tolua_S,5,0));
  double a_FlyAwaySpeed = ((double)  tolua_tonumber(tolua_S,6,1.0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SpawnItemPickups'", NULL);
#endif
  {
   self->SpawnItemPickups(*a_Pickups,a_BlockX,a_BlockY,a_BlockZ,a_FlyAwaySpeed);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SpawnItemPickups'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SpawnItemPickups of class  cWorld */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cWorld_SpawnItemPickups01
static int tolua_AllToLua_cWorld_SpawnItemPickups01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cWorld",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const cItems",0,&tolua_err)) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,6,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,7,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,8,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,9,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  cWorld* self = (cWorld*)  tolua_tousertype(tolua_S,1,0);
  const cItems* a_Pickups = ((const cItems*)  tolua_tousertype(tolua_S,2,0));
  double a_BlockX = ((double)  tolua_tonumber(tolua_S,3,0));
  double a_BlockY = ((double)  tolua_tonumber(tolua_S,4,0));
  double a_BlockZ = ((double)  tolua_tonumber(tolua_S,5,0));
  double a_SpeedX = ((double)  tolua_tonumber(tolua_S,6,0));
  double a_SpeedY = ((double)  tolua_tonumber(tolua_S,7,0));
  double a_SpeedZ = ((double)  tolua_tonumber(tolua_S,8,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SpawnItemPickups'", NULL);
#endif
  {
   self->SpawnItemPickups(*a_Pickups,a_BlockX,a_BlockY,a_BlockZ,a_SpeedX,a_SpeedY,a_SpeedZ);
  }
 }
 return 0;
tolua_lerror:
 return tolua_AllToLua_cWorld_SpawnItemPickups00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: SpawnPrimedTNT of class  cWorld */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cWorld_SpawnPrimedTNT00
static int tolua_AllToLua_cWorld_SpawnPrimedTNT00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cWorld",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,6,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,7,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cWorld* self = (cWorld*)  tolua_tousertype(tolua_S,1,0);
  double a_X = ((double)  tolua_tonumber(tolua_S,2,0));
  double a_Y = ((double)  tolua_tonumber(tolua_S,3,0));
  double a_Z = ((double)  tolua_tonumber(tolua_S,4,0));
  float a_FuseTimeInSec = ((float)  tolua_tonumber(tolua_S,5,0));
  double a_InitialVelocityCoeff = ((double)  tolua_tonumber(tolua_S,6,1));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SpawnPrimedTNT'", NULL);
#endif
  {
   self->SpawnPrimedTNT(a_X,a_Y,a_Z,a_FuseTimeInSec,a_InitialVelocityCoeff);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SpawnPrimedTNT'.",&tolua_err);
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'DigBlock'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->DigBlock(a_X,a_Y,a_Z);
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
     !tolua_isusertype(tolua_S,1,"const cWorld",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cWorld* self = (const cWorld*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetSpawnX'", NULL);
#endif
  {
   double tolua_ret = (double)  self->GetSpawnX();
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
     !tolua_isusertype(tolua_S,1,"const cWorld",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cWorld* self = (const cWorld*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetSpawnY'", NULL);
#endif
  {
   double tolua_ret = (double)  self->GetSpawnY();
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
     !tolua_isusertype(tolua_S,1,"const cWorld",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cWorld* self = (const cWorld*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetSpawnZ'", NULL);
#endif
  {
   double tolua_ret = (double)  self->GetSpawnZ();
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

/* method: WakeUpSimulators of class  cWorld */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cWorld_WakeUpSimulators00
static int tolua_AllToLua_cWorld_WakeUpSimulators00(lua_State* tolua_S)
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
  int a_BlockX = ((int)  tolua_tonumber(tolua_S,2,0));
  int a_BlockY = ((int)  tolua_tonumber(tolua_S,3,0));
  int a_BlockZ = ((int)  tolua_tonumber(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'WakeUpSimulators'", NULL);
#endif
  {
   self->WakeUpSimulators(a_BlockX,a_BlockY,a_BlockZ);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'WakeUpSimulators'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: WakeUpSimulatorsInArea of class  cWorld */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cWorld_WakeUpSimulatorsInArea00
static int tolua_AllToLua_cWorld_WakeUpSimulatorsInArea00(lua_State* tolua_S)
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
     !tolua_isnumber(tolua_S,7,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,8,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cWorld* self = (cWorld*)  tolua_tousertype(tolua_S,1,0);
  int a_MinBlockX = ((int)  tolua_tonumber(tolua_S,2,0));
  int a_MaxBlockX = ((int)  tolua_tonumber(tolua_S,3,0));
  int a_MinBlockY = ((int)  tolua_tonumber(tolua_S,4,0));
  int a_MaxBlockY = ((int)  tolua_tonumber(tolua_S,5,0));
  int a_MinBlockZ = ((int)  tolua_tonumber(tolua_S,6,0));
  int a_MaxBlockZ = ((int)  tolua_tonumber(tolua_S,7,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'WakeUpSimulatorsInArea'", NULL);
#endif
  {
   self->WakeUpSimulatorsInArea(a_MinBlockX,a_MaxBlockX,a_MinBlockY,a_MaxBlockY,a_MinBlockZ,a_MaxBlockZ);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'WakeUpSimulatorsInArea'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetSignLines of class  cWorld */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cWorld_GetSignLines00
static int tolua_AllToLua_cWorld_GetSignLines00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cWorld",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,5,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,6,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,7,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,8,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,9,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cWorld* self = (cWorld*)  tolua_tousertype(tolua_S,1,0);
  int a_BlockX = ((int)  tolua_tonumber(tolua_S,2,0));
  int a_BlockY = ((int)  tolua_tonumber(tolua_S,3,0));
  int a_BlockZ = ((int)  tolua_tonumber(tolua_S,4,0));
  AString a_Line1 = ((AString)  tolua_tocppstring(tolua_S,5,0));
  AString a_Line2 = ((AString)  tolua_tocppstring(tolua_S,6,0));
  AString a_Line3 = ((AString)  tolua_tocppstring(tolua_S,7,0));
  AString a_Line4 = ((AString)  tolua_tocppstring(tolua_S,8,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetSignLines'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->GetSignLines(a_BlockX,a_BlockY,a_BlockZ,a_Line1,a_Line2,a_Line3,a_Line4);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
   tolua_pushcppstring(tolua_S,(const char*)a_Line1);
   tolua_pushcppstring(tolua_S,(const char*)a_Line2);
   tolua_pushcppstring(tolua_S,(const char*)a_Line3);
   tolua_pushcppstring(tolua_S,(const char*)a_Line4);
  }
 }
 return 5;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetSignLines'.",&tolua_err);
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
  int a_BlockX = ((int)  tolua_tonumber(tolua_S,2,0));
  int a_BlockY = ((int)  tolua_tonumber(tolua_S,3,0));
  int a_BlockZ = ((int)  tolua_tonumber(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GrowTree'", NULL);
#endif
  {
   self->GrowTree(a_BlockX,a_BlockY,a_BlockZ);
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

/* method: GrowTreeFromSapling of class  cWorld */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cWorld_GrowTreeFromSapling00
static int tolua_AllToLua_cWorld_GrowTreeFromSapling00(lua_State* tolua_S)
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
  int a_BlockX = ((int)  tolua_tonumber(tolua_S,2,0));
  int a_BlockY = ((int)  tolua_tonumber(tolua_S,3,0));
  int a_BlockZ = ((int)  tolua_tonumber(tolua_S,4,0));
  char a_SaplingMeta = ((char)  tolua_tonumber(tolua_S,5,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GrowTreeFromSapling'", NULL);
#endif
  {
   self->GrowTreeFromSapling(a_BlockX,a_BlockY,a_BlockZ,a_SaplingMeta);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GrowTreeFromSapling'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GrowTreeByBiome of class  cWorld */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cWorld_GrowTreeByBiome00
static int tolua_AllToLua_cWorld_GrowTreeByBiome00(lua_State* tolua_S)
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
  int a_BlockX = ((int)  tolua_tonumber(tolua_S,2,0));
  int a_BlockY = ((int)  tolua_tonumber(tolua_S,3,0));
  int a_BlockZ = ((int)  tolua_tonumber(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GrowTreeByBiome'", NULL);
#endif
  {
   self->GrowTreeByBiome(a_BlockX,a_BlockY,a_BlockZ);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GrowTreeByBiome'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GrowRipePlant of class  cWorld */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cWorld_GrowRipePlant00
static int tolua_AllToLua_cWorld_GrowRipePlant00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cWorld",0,&tolua_err) ||
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
  cWorld* self = (cWorld*)  tolua_tousertype(tolua_S,1,0);
  int a_BlockX = ((int)  tolua_tonumber(tolua_S,2,0));
  int a_BlockY = ((int)  tolua_tonumber(tolua_S,3,0));
  int a_BlockZ = ((int)  tolua_tonumber(tolua_S,4,0));
  bool a_IsByBonemeal = ((bool)  tolua_toboolean(tolua_S,5,false));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GrowRipePlant'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->GrowRipePlant(a_BlockX,a_BlockY,a_BlockZ,a_IsByBonemeal);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GrowRipePlant'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GrowCactus of class  cWorld */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cWorld_GrowCactus00
static int tolua_AllToLua_cWorld_GrowCactus00(lua_State* tolua_S)
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
  int a_BlockX = ((int)  tolua_tonumber(tolua_S,2,0));
  int a_BlockY = ((int)  tolua_tonumber(tolua_S,3,0));
  int a_BlockZ = ((int)  tolua_tonumber(tolua_S,4,0));
  int a_NumBlocksToGrow = ((int)  tolua_tonumber(tolua_S,5,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GrowCactus'", NULL);
#endif
  {
   self->GrowCactus(a_BlockX,a_BlockY,a_BlockZ,a_NumBlocksToGrow);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GrowCactus'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GrowMelonPumpkin of class  cWorld */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cWorld_GrowMelonPumpkin00
static int tolua_AllToLua_cWorld_GrowMelonPumpkin00(lua_State* tolua_S)
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
  int a_BlockX = ((int)  tolua_tonumber(tolua_S,2,0));
  int a_BlockY = ((int)  tolua_tonumber(tolua_S,3,0));
  int a_BlockZ = ((int)  tolua_tonumber(tolua_S,4,0));
  char a_BlockType = ((char)  tolua_tonumber(tolua_S,5,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GrowMelonPumpkin'", NULL);
#endif
  {
   self->GrowMelonPumpkin(a_BlockX,a_BlockY,a_BlockZ,a_BlockType);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GrowMelonPumpkin'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GrowSugarcane of class  cWorld */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cWorld_GrowSugarcane00
static int tolua_AllToLua_cWorld_GrowSugarcane00(lua_State* tolua_S)
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
  int a_BlockX = ((int)  tolua_tonumber(tolua_S,2,0));
  int a_BlockY = ((int)  tolua_tonumber(tolua_S,3,0));
  int a_BlockZ = ((int)  tolua_tonumber(tolua_S,4,0));
  int a_NumBlocksToGrow = ((int)  tolua_tonumber(tolua_S,5,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GrowSugarcane'", NULL);
#endif
  {
   self->GrowSugarcane(a_BlockX,a_BlockY,a_BlockZ,a_NumBlocksToGrow);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GrowSugarcane'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetBiomeAt of class  cWorld */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cWorld_GetBiomeAt00
static int tolua_AllToLua_cWorld_GetBiomeAt00(lua_State* tolua_S)
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
  int a_BlockX = ((int)  tolua_tonumber(tolua_S,2,0));
  int a_BlockZ = ((int)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetBiomeAt'", NULL);
#endif
  {
   int tolua_ret = (int)  self->GetBiomeAt(a_BlockX,a_BlockZ);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetBiomeAt'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetName of class  cWorld */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cWorld_GetName00
static int tolua_AllToLua_cWorld_GetName00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cWorld",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cWorld* self = (const cWorld*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetName'", NULL);
#endif
  {
   const AString tolua_ret = (const AString)  self->GetName();
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

/* method: SaveAllChunks of class  cWorld */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cWorld_SaveAllChunks00
static int tolua_AllToLua_cWorld_SaveAllChunks00(lua_State* tolua_S)
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SaveAllChunks'", NULL);
#endif
  {
   self->SaveAllChunks();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SaveAllChunks'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetNumChunks of class  cWorld */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cWorld_GetNumChunks00
static int tolua_AllToLua_cWorld_GetNumChunks00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cWorld",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cWorld* self = (const cWorld*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetNumChunks'", NULL);
#endif
  {
   int tolua_ret = (int)  self->GetNumChunks();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetNumChunks'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetGeneratorQueueLength of class  cWorld */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cWorld_GetGeneratorQueueLength00
static int tolua_AllToLua_cWorld_GetGeneratorQueueLength00(lua_State* tolua_S)
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetGeneratorQueueLength'", NULL);
#endif
  {
   int tolua_ret = (int)  self->GetGeneratorQueueLength();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetGeneratorQueueLength'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetLightingQueueLength of class  cWorld */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cWorld_GetLightingQueueLength00
static int tolua_AllToLua_cWorld_GetLightingQueueLength00(lua_State* tolua_S)
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetLightingQueueLength'", NULL);
#endif
  {
   int tolua_ret = (int)  self->GetLightingQueueLength();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetLightingQueueLength'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetStorageLoadQueueLength of class  cWorld */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cWorld_GetStorageLoadQueueLength00
static int tolua_AllToLua_cWorld_GetStorageLoadQueueLength00(lua_State* tolua_S)
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetStorageLoadQueueLength'", NULL);
#endif
  {
   int tolua_ret = (int)  self->GetStorageLoadQueueLength();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetStorageLoadQueueLength'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetStorageSaveQueueLength of class  cWorld */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cWorld_GetStorageSaveQueueLength00
static int tolua_AllToLua_cWorld_GetStorageSaveQueueLength00(lua_State* tolua_S)
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetStorageSaveQueueLength'", NULL);
#endif
  {
   int tolua_ret = (int)  self->GetStorageSaveQueueLength();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetStorageSaveQueueLength'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: QueueBlockForTick of class  cWorld */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cWorld_QueueBlockForTick00
static int tolua_AllToLua_cWorld_QueueBlockForTick00(lua_State* tolua_S)
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
  int a_BlockX = ((int)  tolua_tonumber(tolua_S,2,0));
  int a_BlockY = ((int)  tolua_tonumber(tolua_S,3,0));
  int a_BlockZ = ((int)  tolua_tonumber(tolua_S,4,0));
  float a_TimeToWait = ((float)  tolua_tonumber(tolua_S,5,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'QueueBlockForTick'", NULL);
#endif
  {
   self->QueueBlockForTick(a_BlockX,a_BlockY,a_BlockZ,a_TimeToWait);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'QueueBlockForTick'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: CastThunderbolt of class  cWorld */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cWorld_CastThunderbolt00
static int tolua_AllToLua_cWorld_CastThunderbolt00(lua_State* tolua_S)
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
  int a_BlockX = ((int)  tolua_tonumber(tolua_S,2,0));
  int a_BlockY = ((int)  tolua_tonumber(tolua_S,3,0));
  int a_BlockZ = ((int)  tolua_tonumber(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'CastThunderbolt'", NULL);
#endif
  {
   self->CastThunderbolt(a_BlockX,a_BlockY,a_BlockZ);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'CastThunderbolt'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetWeather of class  cWorld */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cWorld_SetWeather00
static int tolua_AllToLua_cWorld_SetWeather00(lua_State* tolua_S)
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
  eWeather a_NewWeather = ((eWeather) (int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetWeather'", NULL);
#endif
  {
   self->SetWeather(a_NewWeather);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetWeather'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: ChangeWeather of class  cWorld */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cWorld_ChangeWeather00
static int tolua_AllToLua_cWorld_ChangeWeather00(lua_State* tolua_S)
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'ChangeWeather'", NULL);
#endif
  {
   self->ChangeWeather();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'ChangeWeather'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetWeather of class  cWorld */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cWorld_GetWeather00
static int tolua_AllToLua_cWorld_GetWeather00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cWorld",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cWorld* self = (const cWorld*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetWeather'", NULL);
#endif
  {
   eWeather tolua_ret = (eWeather)  self->GetWeather();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetWeather'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetNextBlockTick of class  cWorld */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cWorld_SetNextBlockTick00
static int tolua_AllToLua_cWorld_SetNextBlockTick00(lua_State* tolua_S)
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
  int a_BlockX = ((int)  tolua_tonumber(tolua_S,2,0));
  int a_BlockY = ((int)  tolua_tonumber(tolua_S,3,0));
  int a_BlockZ = ((int)  tolua_tonumber(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetNextBlockTick'", NULL);
#endif
  {
   self->SetNextBlockTick(a_BlockX,a_BlockY,a_BlockZ);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetNextBlockTick'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetMaxSugarcaneHeight of class  cWorld */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cWorld_GetMaxSugarcaneHeight00
static int tolua_AllToLua_cWorld_GetMaxSugarcaneHeight00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cWorld",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cWorld* self = (const cWorld*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetMaxSugarcaneHeight'", NULL);
#endif
  {
   int tolua_ret = (int)  self->GetMaxSugarcaneHeight();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetMaxSugarcaneHeight'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetMaxCactusHeight of class  cWorld */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cWorld_GetMaxCactusHeight00
static int tolua_AllToLua_cWorld_GetMaxCactusHeight00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cWorld",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cWorld* self = (const cWorld*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetMaxCactusHeight'", NULL);
#endif
  {
   int tolua_ret = (int)  self->GetMaxCactusHeight();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetMaxCactusHeight'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: IsBlockDirectlyWatered of class  cWorld */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cWorld_IsBlockDirectlyWatered00
static int tolua_AllToLua_cWorld_IsBlockDirectlyWatered00(lua_State* tolua_S)
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
  int a_BlockX = ((int)  tolua_tonumber(tolua_S,2,0));
  int a_BlockY = ((int)  tolua_tonumber(tolua_S,3,0));
  int a_BlockZ = ((int)  tolua_tonumber(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'IsBlockDirectlyWatered'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->IsBlockDirectlyWatered(a_BlockX,a_BlockY,a_BlockZ);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'IsBlockDirectlyWatered'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SpawnMob of class  cWorld */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cWorld_SpawnMob00
static int tolua_AllToLua_cWorld_SpawnMob00(lua_State* tolua_S)
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
  double a_PosX = ((double)  tolua_tonumber(tolua_S,2,0));
  double a_PosY = ((double)  tolua_tonumber(tolua_S,3,0));
  double a_PosZ = ((double)  tolua_tonumber(tolua_S,4,0));
  int a_EntityType = ((int)  tolua_tonumber(tolua_S,5,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SpawnMob'", NULL);
#endif
  {
   int tolua_ret = (int)  self->SpawnMob(a_PosX,a_PosY,a_PosZ,a_EntityType);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SpawnMob'.",&tolua_err);
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

/* method: HowManyCanFit of class  cInventory */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cInventory_HowManyCanFit00
static int tolua_AllToLua_cInventory_HowManyCanFit00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cInventory",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const cItem",0,&tolua_err)) ||
     !tolua_isboolean(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cInventory* self = (cInventory*)  tolua_tousertype(tolua_S,1,0);
  const cItem* a_ItemStack = ((const cItem*)  tolua_tousertype(tolua_S,2,0));
  bool a_ConsiderEmptySlots = ((bool)  tolua_toboolean(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'HowManyCanFit'", NULL);
#endif
  {
   int tolua_ret = (int)  self->HowManyCanFit(*a_ItemStack,a_ConsiderEmptySlots);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'HowManyCanFit'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: HowManyCanFit of class  cInventory */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cInventory_HowManyCanFit01
static int tolua_AllToLua_cInventory_HowManyCanFit01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cInventory",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const cItem",0,&tolua_err)) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isboolean(tolua_S,5,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  cInventory* self = (cInventory*)  tolua_tousertype(tolua_S,1,0);
  const cItem* a_ItemStack = ((const cItem*)  tolua_tousertype(tolua_S,2,0));
  int a_BeginSlotNum = ((int)  tolua_tonumber(tolua_S,3,0));
  int a_EndSlotNum = ((int)  tolua_tonumber(tolua_S,4,0));
  bool a_ConsiderEmptySlots = ((bool)  tolua_toboolean(tolua_S,5,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'HowManyCanFit'", NULL);
#endif
  {
   int tolua_ret = (int)  self->HowManyCanFit(*a_ItemStack,a_BeginSlotNum,a_EndSlotNum,a_ConsiderEmptySlots);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
tolua_lerror:
 return tolua_AllToLua_cInventory_HowManyCanFit00(tolua_S);
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
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const cItem",0,&tolua_err)) ||
     !tolua_isboolean(tolua_S,3,1,&tolua_err) ||
     !tolua_isboolean(tolua_S,4,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cInventory* self = (cInventory*)  tolua_tousertype(tolua_S,1,0);
  const cItem* a_ItemStack = ((const cItem*)  tolua_tousertype(tolua_S,2,0));
  bool a_AllowNewStacks = ((bool)  tolua_toboolean(tolua_S,3,true));
  bool a_tryToFillEquippedFirst = ((bool)  tolua_toboolean(tolua_S,4,false));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'AddItem'", NULL);
#endif
  {
   int tolua_ret = (int)  self->AddItem(*a_ItemStack,a_AllowNewStacks,a_tryToFillEquippedFirst);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
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

/* method: AddItems of class  cInventory */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cInventory_AddItems00
static int tolua_AllToLua_cInventory_AddItems00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cInventory",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"cItems",0,&tolua_err)) ||
     !tolua_isboolean(tolua_S,3,0,&tolua_err) ||
     !tolua_isboolean(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cInventory* self = (cInventory*)  tolua_tousertype(tolua_S,1,0);
  cItems* a_ItemStackList = ((cItems*)  tolua_tousertype(tolua_S,2,0));
  bool a_AllowNewStacks = ((bool)  tolua_toboolean(tolua_S,3,0));
  bool a_tryToFillEquippedFirst = ((bool)  tolua_toboolean(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'AddItems'", NULL);
#endif
  {
   int tolua_ret = (int)  self->AddItems(*a_ItemStackList,a_AllowNewStacks,a_tryToFillEquippedFirst);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'AddItems'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: RemoveOneEquippedItem of class  cInventory */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cInventory_RemoveOneEquippedItem00
static int tolua_AllToLua_cInventory_RemoveOneEquippedItem00(lua_State* tolua_S)
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'RemoveOneEquippedItem'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->RemoveOneEquippedItem();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'RemoveOneEquippedItem'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: HowManyItems of class  cInventory */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cInventory_HowManyItems00
static int tolua_AllToLua_cInventory_HowManyItems00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cInventory",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const cItem",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cInventory* self = (cInventory*)  tolua_tousertype(tolua_S,1,0);
  const cItem* a_Item = ((const cItem*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'HowManyItems'", NULL);
#endif
  {
   int tolua_ret = (int)  self->HowManyItems(*a_Item);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'HowManyItems'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: HasItems of class  cInventory */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cInventory_HasItems00
static int tolua_AllToLua_cInventory_HasItems00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cInventory",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const cItem",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cInventory* self = (cInventory*)  tolua_tousertype(tolua_S,1,0);
  const cItem* a_ItemStack = ((const cItem*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'HasItems'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->HasItems(*a_ItemStack);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'HasItems'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetArmorGrid of class  cInventory */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cInventory_GetArmorGrid00
static int tolua_AllToLua_cInventory_GetArmorGrid00(lua_State* tolua_S)
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetArmorGrid'", NULL);
#endif
  {
   cItemGrid& tolua_ret = (cItemGrid&)  self->GetArmorGrid();
    tolua_pushusertype(tolua_S,(void*)&tolua_ret,"cItemGrid");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetArmorGrid'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetInventoryGrid of class  cInventory */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cInventory_GetInventoryGrid00
static int tolua_AllToLua_cInventory_GetInventoryGrid00(lua_State* tolua_S)
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetInventoryGrid'", NULL);
#endif
  {
   cItemGrid& tolua_ret = (cItemGrid&)  self->GetInventoryGrid();
    tolua_pushusertype(tolua_S,(void*)&tolua_ret,"cItemGrid");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetInventoryGrid'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetHotbarGrid of class  cInventory */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cInventory_GetHotbarGrid00
static int tolua_AllToLua_cInventory_GetHotbarGrid00(lua_State* tolua_S)
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetHotbarGrid'", NULL);
#endif
  {
   cItemGrid& tolua_ret = (cItemGrid&)  self->GetHotbarGrid();
    tolua_pushusertype(tolua_S,(void*)&tolua_ret,"cItemGrid");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetHotbarGrid'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetOwner of class  cInventory */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cInventory_GetOwner00
static int tolua_AllToLua_cInventory_GetOwner00(lua_State* tolua_S)
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetOwner'", NULL);
#endif
  {
   cPlayer& tolua_ret = (cPlayer&)  self->GetOwner();
    tolua_pushusertype(tolua_S,(void*)&tolua_ret,"cPlayer");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetOwner'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: CopyToItems of class  cInventory */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cInventory_CopyToItems00
static int tolua_AllToLua_cInventory_CopyToItems00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cInventory",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"cItems",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cInventory* self = (cInventory*)  tolua_tousertype(tolua_S,1,0);
  cItems* a_Items = ((cItems*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'CopyToItems'", NULL);
#endif
  {
   self->CopyToItems(*a_Items);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'CopyToItems'.",&tolua_err);
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
     !tolua_isusertype(tolua_S,1,"const cInventory",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cInventory* self = (const cInventory*)  tolua_tousertype(tolua_S,1,0);
  int a_SlotNum = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetSlot'", NULL);
#endif
  {
   const cItem& tolua_ret = (const cItem&)  self->GetSlot(a_SlotNum);
    tolua_pushusertype(tolua_S,(void*)&tolua_ret,"const cItem");
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

/* method: GetArmorSlot of class  cInventory */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cInventory_GetArmorSlot00
static int tolua_AllToLua_cInventory_GetArmorSlot00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cInventory",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cInventory* self = (const cInventory*)  tolua_tousertype(tolua_S,1,0);
  int a_ArmorSlotNum = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetArmorSlot'", NULL);
#endif
  {
   const cItem& tolua_ret = (const cItem&)  self->GetArmorSlot(a_ArmorSlotNum);
    tolua_pushusertype(tolua_S,(void*)&tolua_ret,"const cItem");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetArmorSlot'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetInventorySlot of class  cInventory */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cInventory_GetInventorySlot00
static int tolua_AllToLua_cInventory_GetInventorySlot00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cInventory",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cInventory* self = (const cInventory*)  tolua_tousertype(tolua_S,1,0);
  int a_InventorySlotNum = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetInventorySlot'", NULL);
#endif
  {
   const cItem& tolua_ret = (const cItem&)  self->GetInventorySlot(a_InventorySlotNum);
    tolua_pushusertype(tolua_S,(void*)&tolua_ret,"const cItem");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetInventorySlot'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetHotbarSlot of class  cInventory */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cInventory_GetHotbarSlot00
static int tolua_AllToLua_cInventory_GetHotbarSlot00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cInventory",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cInventory* self = (const cInventory*)  tolua_tousertype(tolua_S,1,0);
  int a_HotBarSlotNum = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetHotbarSlot'", NULL);
#endif
  {
   const cItem& tolua_ret = (const cItem&)  self->GetHotbarSlot(a_HotBarSlotNum);
    tolua_pushusertype(tolua_S,(void*)&tolua_ret,"const cItem");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetHotbarSlot'.",&tolua_err);
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
     !tolua_isusertype(tolua_S,1,"const cInventory",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cInventory* self = (const cInventory*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetEquippedItem'", NULL);
#endif
  {
   const cItem& tolua_ret = (const cItem&)  self->GetEquippedItem();
    tolua_pushusertype(tolua_S,(void*)&tolua_ret,"const cItem");
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

/* method: SetSlot of class  cInventory */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cInventory_SetSlot00
static int tolua_AllToLua_cInventory_SetSlot00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cInventory",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,3,&tolua_err) || !tolua_isusertype(tolua_S,3,"const cItem",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cInventory* self = (cInventory*)  tolua_tousertype(tolua_S,1,0);
  int a_SlotNum = ((int)  tolua_tonumber(tolua_S,2,0));
  const cItem* a_Item = ((const cItem*)  tolua_tousertype(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetSlot'", NULL);
#endif
  {
   self->SetSlot(a_SlotNum,*a_Item);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetSlot'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetArmorSlot of class  cInventory */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cInventory_SetArmorSlot00
static int tolua_AllToLua_cInventory_SetArmorSlot00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cInventory",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,3,&tolua_err) || !tolua_isusertype(tolua_S,3,"const cItem",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cInventory* self = (cInventory*)  tolua_tousertype(tolua_S,1,0);
  int a_ArmorSlotNum = ((int)  tolua_tonumber(tolua_S,2,0));
  const cItem* a_Item = ((const cItem*)  tolua_tousertype(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetArmorSlot'", NULL);
#endif
  {
   self->SetArmorSlot(a_ArmorSlotNum,*a_Item);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetArmorSlot'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetInventorySlot of class  cInventory */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cInventory_SetInventorySlot00
static int tolua_AllToLua_cInventory_SetInventorySlot00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cInventory",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,3,&tolua_err) || !tolua_isusertype(tolua_S,3,"const cItem",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cInventory* self = (cInventory*)  tolua_tousertype(tolua_S,1,0);
  int a_InventorySlotNum = ((int)  tolua_tonumber(tolua_S,2,0));
  const cItem* a_Item = ((const cItem*)  tolua_tousertype(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetInventorySlot'", NULL);
#endif
  {
   self->SetInventorySlot(a_InventorySlotNum,*a_Item);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetInventorySlot'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetHotbarSlot of class  cInventory */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cInventory_SetHotbarSlot00
static int tolua_AllToLua_cInventory_SetHotbarSlot00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cInventory",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,3,&tolua_err) || !tolua_isusertype(tolua_S,3,"const cItem",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cInventory* self = (cInventory*)  tolua_tousertype(tolua_S,1,0);
  int a_HotBarSlotNum = ((int)  tolua_tonumber(tolua_S,2,0));
  const cItem* a_Item = ((const cItem*)  tolua_tousertype(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetHotbarSlot'", NULL);
#endif
  {
   self->SetHotbarSlot(a_HotBarSlotNum,*a_Item);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetHotbarSlot'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetEquippedSlotNum of class  cInventory */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cInventory_SetEquippedSlotNum00
static int tolua_AllToLua_cInventory_SetEquippedSlotNum00(lua_State* tolua_S)
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetEquippedSlotNum'", NULL);
#endif
  {
   self->SetEquippedSlotNum(a_SlotNum);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetEquippedSlotNum'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetEquippedSlotNum of class  cInventory */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cInventory_GetEquippedSlotNum00
static int tolua_AllToLua_cInventory_GetEquippedSlotNum00(lua_State* tolua_S)
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetEquippedSlotNum'", NULL);
#endif
  {
   int tolua_ret = (int)  self->GetEquippedSlotNum();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetEquippedSlotNum'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: ChangeSlotCount of class  cInventory */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cInventory_ChangeSlotCount00
static int tolua_AllToLua_cInventory_ChangeSlotCount00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cInventory",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cInventory* self = (cInventory*)  tolua_tousertype(tolua_S,1,0);
  int a_SlotNum = ((int)  tolua_tonumber(tolua_S,2,0));
  int a_AddToCount = ((int)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'ChangeSlotCount'", NULL);
#endif
  {
   int tolua_ret = (int)  self->ChangeSlotCount(a_SlotNum,a_AddToCount);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'ChangeSlotCount'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: DamageItem of class  cInventory */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cInventory_DamageItem00
static int tolua_AllToLua_cInventory_DamageItem00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cInventory",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cInventory* self = (cInventory*)  tolua_tousertype(tolua_S,1,0);
  int a_SlotNum = ((int)  tolua_tonumber(tolua_S,2,0));
  short a_Amount = ((short)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'DamageItem'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->DamageItem(a_SlotNum,a_Amount);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'DamageItem'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: DamageEquippedItem of class  cInventory */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cInventory_DamageEquippedItem00
static int tolua_AllToLua_cInventory_DamageEquippedItem00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cInventory",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cInventory* self = (cInventory*)  tolua_tousertype(tolua_S,1,0);
  short a_Amount = ((short)  tolua_tonumber(tolua_S,2,1));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'DamageEquippedItem'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->DamageEquippedItem(a_Amount);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'DamageEquippedItem'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetEquippedHelmet of class  cInventory */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cInventory_GetEquippedHelmet00
static int tolua_AllToLua_cInventory_GetEquippedHelmet00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cInventory",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cInventory* self = (const cInventory*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetEquippedHelmet'", NULL);
#endif
  {
   const cItem& tolua_ret = (const cItem&)  self->GetEquippedHelmet();
    tolua_pushusertype(tolua_S,(void*)&tolua_ret,"const cItem");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetEquippedHelmet'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetEquippedChestplate of class  cInventory */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cInventory_GetEquippedChestplate00
static int tolua_AllToLua_cInventory_GetEquippedChestplate00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cInventory",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cInventory* self = (const cInventory*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetEquippedChestplate'", NULL);
#endif
  {
   const cItem& tolua_ret = (const cItem&)  self->GetEquippedChestplate();
    tolua_pushusertype(tolua_S,(void*)&tolua_ret,"const cItem");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetEquippedChestplate'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetEquippedLeggings of class  cInventory */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cInventory_GetEquippedLeggings00
static int tolua_AllToLua_cInventory_GetEquippedLeggings00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cInventory",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cInventory* self = (const cInventory*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetEquippedLeggings'", NULL);
#endif
  {
   const cItem& tolua_ret = (const cItem&)  self->GetEquippedLeggings();
    tolua_pushusertype(tolua_S,(void*)&tolua_ret,"const cItem");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetEquippedLeggings'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetEquippedBoots of class  cInventory */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cInventory_GetEquippedBoots00
static int tolua_AllToLua_cInventory_GetEquippedBoots00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cInventory",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cInventory* self = (const cInventory*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetEquippedBoots'", NULL);
#endif
  {
   const cItem& tolua_ret = (const cItem&)  self->GetEquippedBoots();
    tolua_pushusertype(tolua_S,(void*)&tolua_ret,"const cItem");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetEquippedBoots'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  cEnchantments */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cEnchantments_new00
static int tolua_AllToLua_cEnchantments_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"cEnchantments",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   cEnchantments* tolua_ret = (cEnchantments*)  Mtolua_new((cEnchantments)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"cEnchantments");
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

/* method: new_local of class  cEnchantments */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cEnchantments_new00_local
static int tolua_AllToLua_cEnchantments_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"cEnchantments",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   cEnchantments* tolua_ret = (cEnchantments*)  Mtolua_new((cEnchantments)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"cEnchantments");
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

/* method: new of class  cEnchantments */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cEnchantments_new01
static int tolua_AllToLua_cEnchantments_new01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"cEnchantments",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  const AString a_StringSpec = ((const AString)  tolua_tocppstring(tolua_S,2,0));
  {
   cEnchantments* tolua_ret = (cEnchantments*)  Mtolua_new((cEnchantments)(a_StringSpec));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"cEnchantments");
   tolua_pushcppstring(tolua_S,(const char*)a_StringSpec);
  }
 }
 return 2;
tolua_lerror:
 return tolua_AllToLua_cEnchantments_new00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  cEnchantments */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cEnchantments_new01_local
static int tolua_AllToLua_cEnchantments_new01_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"cEnchantments",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  const AString a_StringSpec = ((const AString)  tolua_tocppstring(tolua_S,2,0));
  {
   cEnchantments* tolua_ret = (cEnchantments*)  Mtolua_new((cEnchantments)(a_StringSpec));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"cEnchantments");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
   tolua_pushcppstring(tolua_S,(const char*)a_StringSpec);
  }
 }
 return 2;
tolua_lerror:
 return tolua_AllToLua_cEnchantments_new00_local(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: AddFromString of class  cEnchantments */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cEnchantments_AddFromString00
static int tolua_AllToLua_cEnchantments_AddFromString00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cEnchantments",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cEnchantments* self = (cEnchantments*)  tolua_tousertype(tolua_S,1,0);
  const AString a_StringSpec = ((const AString)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'AddFromString'", NULL);
#endif
  {
   self->AddFromString(a_StringSpec);
   tolua_pushcppstring(tolua_S,(const char*)a_StringSpec);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'AddFromString'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: ToString of class  cEnchantments */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cEnchantments_ToString00
static int tolua_AllToLua_cEnchantments_ToString00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cEnchantments",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cEnchantments* self = (const cEnchantments*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'ToString'", NULL);
#endif
  {
   AString tolua_ret = (AString)  self->ToString();
   tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'ToString'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetLevel of class  cEnchantments */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cEnchantments_GetLevel00
static int tolua_AllToLua_cEnchantments_GetLevel00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cEnchantments",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cEnchantments* self = (const cEnchantments*)  tolua_tousertype(tolua_S,1,0);
  int a_EnchantmentID = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetLevel'", NULL);
#endif
  {
   int tolua_ret = (int)  self->GetLevel(a_EnchantmentID);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetLevel'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetLevel of class  cEnchantments */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cEnchantments_SetLevel00
static int tolua_AllToLua_cEnchantments_SetLevel00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cEnchantments",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cEnchantments* self = (cEnchantments*)  tolua_tousertype(tolua_S,1,0);
  int a_EnchantmentID = ((int)  tolua_tonumber(tolua_S,2,0));
  int a_Level = ((int)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetLevel'", NULL);
#endif
  {
   self->SetLevel(a_EnchantmentID,a_Level);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetLevel'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: Clear of class  cEnchantments */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cEnchantments_Clear00
static int tolua_AllToLua_cEnchantments_Clear00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cEnchantments",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cEnchantments* self = (cEnchantments*)  tolua_tousertype(tolua_S,1,0);
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

/* method: IsEmpty of class  cEnchantments */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cEnchantments_IsEmpty00
static int tolua_AllToLua_cEnchantments_IsEmpty00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cEnchantments",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cEnchantments* self = (const cEnchantments*)  tolua_tousertype(tolua_S,1,0);
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

/* method: StringToEnchantmentID of class  cEnchantments */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cEnchantments_StringToEnchantmentID00
static int tolua_AllToLua_cEnchantments_StringToEnchantmentID00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"cEnchantments",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const AString a_EnchantmentName = ((const AString)  tolua_tocppstring(tolua_S,2,0));
  {
   int tolua_ret = (int)  cEnchantments::StringToEnchantmentID(a_EnchantmentName);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
   tolua_pushcppstring(tolua_S,(const char*)a_EnchantmentName);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'StringToEnchantmentID'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: operator== of class  cEnchantments */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cEnchantments__eq00
static int tolua_AllToLua_cEnchantments__eq00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cEnchantments",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const cEnchantments",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cEnchantments* self = (const cEnchantments*)  tolua_tousertype(tolua_S,1,0);
  const cEnchantments* a_Other = ((const cEnchantments*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'operator=='", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->operator==(*a_Other);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function '.eq'.",&tolua_err);
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
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   cItem* tolua_ret = (cItem*)  Mtolua_new((cItem)());
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
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   cItem* tolua_ret = (cItem*)  Mtolua_new((cItem)());
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

/* method: new of class  cItem */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cItem_new01
static int tolua_AllToLua_cItem_new01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"cItem",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,1,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  short a_ItemType = ((short)  tolua_tonumber(tolua_S,2,0));
  char a_ItemCount = ((char)  tolua_tonumber(tolua_S,3,1));
  short a_ItemDamage = ((short)  tolua_tonumber(tolua_S,4,0));
  {
   cItem* tolua_ret = (cItem*)  Mtolua_new((cItem)(a_ItemType,a_ItemCount,a_ItemDamage));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"cItem");
  }
 }
 return 1;
tolua_lerror:
 return tolua_AllToLua_cItem_new00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  cItem */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cItem_new01_local
static int tolua_AllToLua_cItem_new01_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"cItem",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,1,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  short a_ItemType = ((short)  tolua_tonumber(tolua_S,2,0));
  char a_ItemCount = ((char)  tolua_tonumber(tolua_S,3,1));
  short a_ItemDamage = ((short)  tolua_tonumber(tolua_S,4,0));
  {
   cItem* tolua_ret = (cItem*)  Mtolua_new((cItem)(a_ItemType,a_ItemCount,a_ItemDamage));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"cItem");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
  }
 }
 return 1;
tolua_lerror:
 return tolua_AllToLua_cItem_new00_local(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  cItem */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cItem_new02
static int tolua_AllToLua_cItem_new02(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"cItem",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,5,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  short a_ItemType = ((short)  tolua_tonumber(tolua_S,2,0));
  char a_ItemCount = ((char)  tolua_tonumber(tolua_S,3,0));
  short a_ItemDamage = ((short)  tolua_tonumber(tolua_S,4,0));
  const AString a_Enchantments = ((const AString)  tolua_tocppstring(tolua_S,5,0));
  {
   cItem* tolua_ret = (cItem*)  Mtolua_new((cItem)(a_ItemType,a_ItemCount,a_ItemDamage,a_Enchantments));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"cItem");
   tolua_pushcppstring(tolua_S,(const char*)a_Enchantments);
  }
 }
 return 2;
tolua_lerror:
 return tolua_AllToLua_cItem_new01(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  cItem */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cItem_new02_local
static int tolua_AllToLua_cItem_new02_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"cItem",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,5,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  short a_ItemType = ((short)  tolua_tonumber(tolua_S,2,0));
  char a_ItemCount = ((char)  tolua_tonumber(tolua_S,3,0));
  short a_ItemDamage = ((short)  tolua_tonumber(tolua_S,4,0));
  const AString a_Enchantments = ((const AString)  tolua_tocppstring(tolua_S,5,0));
  {
   cItem* tolua_ret = (cItem*)  Mtolua_new((cItem)(a_ItemType,a_ItemCount,a_ItemDamage,a_Enchantments));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"cItem");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
   tolua_pushcppstring(tolua_S,(const char*)a_Enchantments);
  }
 }
 return 2;
tolua_lerror:
 return tolua_AllToLua_cItem_new01_local(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  cItem */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cItem_new03
static int tolua_AllToLua_cItem_new03(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"cItem",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const cItem",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  const cItem* a_CopyFrom = ((const cItem*)  tolua_tousertype(tolua_S,2,0));
  {
   cItem* tolua_ret = (cItem*)  Mtolua_new((cItem)(*a_CopyFrom));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"cItem");
  }
 }
 return 1;
tolua_lerror:
 return tolua_AllToLua_cItem_new02(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  cItem */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cItem_new03_local
static int tolua_AllToLua_cItem_new03_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"cItem",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const cItem",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  const cItem* a_CopyFrom = ((const cItem*)  tolua_tousertype(tolua_S,2,0));
  {
   cItem* tolua_ret = (cItem*)  Mtolua_new((cItem)(*a_CopyFrom));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"cItem");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
  }
 }
 return 1;
tolua_lerror:
 return tolua_AllToLua_cItem_new02_local(tolua_S);
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

/* method: Clear of class  cItem */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cItem_Clear00
static int tolua_AllToLua_cItem_Clear00(lua_State* tolua_S)
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

/* method: IsEmpty of class  cItem */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cItem_IsEmpty00
static int tolua_AllToLua_cItem_IsEmpty00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cItem",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cItem* self = (const cItem*)  tolua_tousertype(tolua_S,1,0);
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

/* method: IsEqual of class  cItem */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cItem_IsEqual00
static int tolua_AllToLua_cItem_IsEqual00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cItem",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const cItem",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cItem* self = (const cItem*)  tolua_tousertype(tolua_S,1,0);
  const cItem* a_Item = ((const cItem*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'IsEqual'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->IsEqual(*a_Item);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'IsEqual'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: IsSameType of class  cItem */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cItem_IsSameType00
static int tolua_AllToLua_cItem_IsSameType00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cItem",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const cItem",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cItem* self = (const cItem*)  tolua_tousertype(tolua_S,1,0);
  const cItem* a_Item = ((const cItem*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'IsSameType'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->IsSameType(*a_Item);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'IsSameType'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: CopyOne of class  cItem */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cItem_CopyOne00
static int tolua_AllToLua_cItem_CopyOne00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cItem",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cItem* self = (const cItem*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'CopyOne'", NULL);
#endif
  {
   cItem tolua_ret = (cItem)  self->CopyOne();
   {
#ifdef __cplusplus
    void* tolua_obj = Mtolua_new((cItem)(tolua_ret));
     tolua_pushusertype(tolua_S,tolua_obj,"cItem");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(cItem));
     tolua_pushusertype(tolua_S,tolua_obj,"cItem");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'CopyOne'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: AddCount of class  cItem */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cItem_AddCount00
static int tolua_AllToLua_cItem_AddCount00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cItem",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cItem* self = (cItem*)  tolua_tousertype(tolua_S,1,0);
  char a_AmountToAdd = ((char)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'AddCount'", NULL);
#endif
  {
   cItem& tolua_ret = (cItem&)  self->AddCount(a_AmountToAdd);
    tolua_pushusertype(tolua_S,(void*)&tolua_ret,"cItem");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'AddCount'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetMaxDamage of class  cItem */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cItem_GetMaxDamage00
static int tolua_AllToLua_cItem_GetMaxDamage00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cItem",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cItem* self = (const cItem*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetMaxDamage'", NULL);
#endif
  {
   short tolua_ret = (short)  self->GetMaxDamage();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetMaxDamage'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: DamageItem of class  cItem */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cItem_DamageItem00
static int tolua_AllToLua_cItem_DamageItem00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cItem",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cItem* self = (cItem*)  tolua_tousertype(tolua_S,1,0);
  short a_Amount = ((short)  tolua_tonumber(tolua_S,2,1));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'DamageItem'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->DamageItem(a_Amount);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'DamageItem'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: IsDamageable of class  cItem */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cItem_IsDamageable00
static int tolua_AllToLua_cItem_IsDamageable00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cItem",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cItem* self = (const cItem*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'IsDamageable'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->IsDamageable();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'IsDamageable'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: IsStackableWith of class  cItem */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cItem_IsStackableWith00
static int tolua_AllToLua_cItem_IsStackableWith00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cItem",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const cItem",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cItem* self = (const cItem*)  tolua_tousertype(tolua_S,1,0);
  const cItem* a_OtherStack = ((const cItem*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'IsStackableWith'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->IsStackableWith(*a_OtherStack);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'IsStackableWith'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: IsFullStack of class  cItem */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cItem_IsFullStack00
static int tolua_AllToLua_cItem_IsFullStack00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cItem",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cItem* self = (const cItem*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'IsFullStack'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->IsFullStack();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'IsFullStack'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* get function: m_ItemType of class  cItem */
#ifndef TOLUA_DISABLE_tolua_get_cItem_m_ItemType
static int tolua_get_cItem_m_ItemType(lua_State* tolua_S)
{
  cItem* self = (cItem*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_ItemType'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->m_ItemType);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: m_ItemType of class  cItem */
#ifndef TOLUA_DISABLE_tolua_set_cItem_m_ItemType
static int tolua_set_cItem_m_ItemType(lua_State* tolua_S)
{
  cItem* self = (cItem*)  tolua_tousertype(tolua_S,1,0);
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

/* get function: m_ItemDamage of class  cItem */
#ifndef TOLUA_DISABLE_tolua_get_cItem_m_ItemDamage
static int tolua_get_cItem_m_ItemDamage(lua_State* tolua_S)
{
  cItem* self = (cItem*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_ItemDamage'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->m_ItemDamage);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: m_ItemDamage of class  cItem */
#ifndef TOLUA_DISABLE_tolua_set_cItem_m_ItemDamage
static int tolua_set_cItem_m_ItemDamage(lua_State* tolua_S)
{
  cItem* self = (cItem*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_ItemDamage'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_ItemDamage = ((short)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: m_Enchantments of class  cItem */
#ifndef TOLUA_DISABLE_tolua_get_cItem_m_Enchantments
static int tolua_get_cItem_m_Enchantments(lua_State* tolua_S)
{
  cItem* self = (cItem*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_Enchantments'",NULL);
#endif
   tolua_pushusertype(tolua_S,(void*)&self->m_Enchantments,"cEnchantments");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: m_Enchantments of class  cItem */
#ifndef TOLUA_DISABLE_tolua_set_cItem_m_Enchantments
static int tolua_set_cItem_m_Enchantments(lua_State* tolua_S)
{
  cItem* self = (cItem*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_Enchantments'",NULL);
  if ((tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"cEnchantments",0,&tolua_err)))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_Enchantments = *((cEnchantments*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  cItems */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cItems_new00
static int tolua_AllToLua_cItems_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"cItems",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   cItems* tolua_ret = (cItems*)  Mtolua_new((cItems)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"cItems");
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

/* method: new_local of class  cItems */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cItems_new00_local
static int tolua_AllToLua_cItems_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"cItems",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   cItems* tolua_ret = (cItems*)  Mtolua_new((cItems)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"cItems");
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

/* method: Get of class  cItems */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cItems_Get00
static int tolua_AllToLua_cItems_Get00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cItems",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cItems* self = (cItems*)  tolua_tousertype(tolua_S,1,0);
  int a_Idx = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'Get'", NULL);
#endif
  {
   cItem* tolua_ret = (cItem*)  self->Get(a_Idx);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"cItem");
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

/* method: Set of class  cItems */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cItems_Set00
static int tolua_AllToLua_cItems_Set00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cItems",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,3,&tolua_err) || !tolua_isusertype(tolua_S,3,"const cItem",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cItems* self = (cItems*)  tolua_tousertype(tolua_S,1,0);
  int a_Idx = ((int)  tolua_tonumber(tolua_S,2,0));
  const cItem* a_Item = ((const cItem*)  tolua_tousertype(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'Set'", NULL);
#endif
  {
   self->Set(a_Idx,*a_Item);
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

/* method: Add of class  cItems */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cItems_Add00
static int tolua_AllToLua_cItems_Add00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cItems",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const cItem",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cItems* self = (cItems*)  tolua_tousertype(tolua_S,1,0);
  const cItem* a_Item = ((const cItem*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'Add'", NULL);
#endif
  {
   self->Add(*a_Item);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Add'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: Delete of class  cItems */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cItems_Delete00
static int tolua_AllToLua_cItems_Delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cItems",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cItems* self = (cItems*)  tolua_tousertype(tolua_S,1,0);
  int a_Idx = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'Delete'", NULL);
#endif
  {
   self->Delete(a_Idx);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Delete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: Clear of class  cItems */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cItems_Clear00
static int tolua_AllToLua_cItems_Clear00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cItems",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cItems* self = (cItems*)  tolua_tousertype(tolua_S,1,0);
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

/* method: Size of class  cItems */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cItems_Size00
static int tolua_AllToLua_cItems_Size00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cItems",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cItems* self = (cItems*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'Size'", NULL);
#endif
  {
   int tolua_ret = (int)  self->Size();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Size'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: Set of class  cItems */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cItems_Set01
static int tolua_AllToLua_cItems_Set01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cItems",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  cItems* self = (cItems*)  tolua_tousertype(tolua_S,1,0);
  int a_Idx = ((int)  tolua_tonumber(tolua_S,2,0));
  ENUM_ITEM_ID a_ItemType = ((ENUM_ITEM_ID) (int)  tolua_tonumber(tolua_S,3,0));
  char a_ItemCount = ((char)  tolua_tonumber(tolua_S,4,0));
  short a_ItemDamage = ((short)  tolua_tonumber(tolua_S,5,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'Set'", NULL);
#endif
  {
   self->Set(a_Idx,a_ItemType,a_ItemCount,a_ItemDamage);
  }
 }
 return 0;
tolua_lerror:
 return tolua_AllToLua_cItems_Set00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: Add of class  cItems */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cItems_Add01
static int tolua_AllToLua_cItems_Add01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cItems",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  cItems* self = (cItems*)  tolua_tousertype(tolua_S,1,0);
  ENUM_ITEM_ID a_ItemType = ((ENUM_ITEM_ID) (int)  tolua_tonumber(tolua_S,2,0));
  char a_ItemCount = ((char)  tolua_tonumber(tolua_S,3,0));
  short a_ItemDamage = ((short)  tolua_tonumber(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'Add'", NULL);
#endif
  {
   self->Add(a_ItemType,a_ItemCount,a_ItemDamage);
  }
 }
 return 0;
tolua_lerror:
 return tolua_AllToLua_cItems_Add00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetWidth of class  cItemGrid */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cItemGrid_GetWidth00
static int tolua_AllToLua_cItemGrid_GetWidth00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cItemGrid",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cItemGrid* self = (const cItemGrid*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetWidth'", NULL);
#endif
  {
   int tolua_ret = (int)  self->GetWidth();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetWidth'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetHeight of class  cItemGrid */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cItemGrid_GetHeight00
static int tolua_AllToLua_cItemGrid_GetHeight00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cItemGrid",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cItemGrid* self = (const cItemGrid*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetHeight'", NULL);
#endif
  {
   int tolua_ret = (int)  self->GetHeight();
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

/* method: GetNumSlots of class  cItemGrid */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cItemGrid_GetNumSlots00
static int tolua_AllToLua_cItemGrid_GetNumSlots00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cItemGrid",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cItemGrid* self = (const cItemGrid*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetNumSlots'", NULL);
#endif
  {
   int tolua_ret = (int)  self->GetNumSlots();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetNumSlots'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetSlotNum of class  cItemGrid */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cItemGrid_GetSlotNum00
static int tolua_AllToLua_cItemGrid_GetSlotNum00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cItemGrid",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cItemGrid* self = (const cItemGrid*)  tolua_tousertype(tolua_S,1,0);
  int a_X = ((int)  tolua_tonumber(tolua_S,2,0));
  int a_Y = ((int)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetSlotNum'", NULL);
#endif
  {
   int tolua_ret = (int)  self->GetSlotNum(a_X,a_Y);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetSlotNum'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetSlot of class  cItemGrid */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cItemGrid_GetSlot00
static int tolua_AllToLua_cItemGrid_GetSlot00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cItemGrid",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cItemGrid* self = (const cItemGrid*)  tolua_tousertype(tolua_S,1,0);
  int a_X = ((int)  tolua_tonumber(tolua_S,2,0));
  int a_Y = ((int)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetSlot'", NULL);
#endif
  {
   const cItem& tolua_ret = (const cItem&)  self->GetSlot(a_X,a_Y);
    tolua_pushusertype(tolua_S,(void*)&tolua_ret,"const cItem");
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

/* method: GetSlot of class  cItemGrid */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cItemGrid_GetSlot01
static int tolua_AllToLua_cItemGrid_GetSlot01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cItemGrid",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  const cItemGrid* self = (const cItemGrid*)  tolua_tousertype(tolua_S,1,0);
  int a_SlotNum = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetSlot'", NULL);
#endif
  {
   const cItem& tolua_ret = (const cItem&)  self->GetSlot(a_SlotNum);
    tolua_pushusertype(tolua_S,(void*)&tolua_ret,"const cItem");
  }
 }
 return 1;
tolua_lerror:
 return tolua_AllToLua_cItemGrid_GetSlot00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetSlot of class  cItemGrid */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cItemGrid_SetSlot00
static int tolua_AllToLua_cItemGrid_SetSlot00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cItemGrid",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,4,&tolua_err) || !tolua_isusertype(tolua_S,4,"const cItem",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cItemGrid* self = (cItemGrid*)  tolua_tousertype(tolua_S,1,0);
  int a_X = ((int)  tolua_tonumber(tolua_S,2,0));
  int a_Y = ((int)  tolua_tonumber(tolua_S,3,0));
  const cItem* a_Item = ((const cItem*)  tolua_tousertype(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetSlot'", NULL);
#endif
  {
   self->SetSlot(a_X,a_Y,*a_Item);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetSlot'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetSlot of class  cItemGrid */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cItemGrid_SetSlot01
static int tolua_AllToLua_cItemGrid_SetSlot01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cItemGrid",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,6,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,7,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  cItemGrid* self = (cItemGrid*)  tolua_tousertype(tolua_S,1,0);
  int a_X = ((int)  tolua_tonumber(tolua_S,2,0));
  int a_Y = ((int)  tolua_tonumber(tolua_S,3,0));
  short a_ItemType = ((short)  tolua_tonumber(tolua_S,4,0));
  char a_ItemCount = ((char)  tolua_tonumber(tolua_S,5,0));
  short a_ItemDamage = ((short)  tolua_tonumber(tolua_S,6,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetSlot'", NULL);
#endif
  {
   self->SetSlot(a_X,a_Y,a_ItemType,a_ItemCount,a_ItemDamage);
  }
 }
 return 0;
tolua_lerror:
 return tolua_AllToLua_cItemGrid_SetSlot00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetSlot of class  cItemGrid */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cItemGrid_SetSlot02
static int tolua_AllToLua_cItemGrid_SetSlot02(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cItemGrid",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,3,&tolua_err) || !tolua_isusertype(tolua_S,3,"const cItem",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  cItemGrid* self = (cItemGrid*)  tolua_tousertype(tolua_S,1,0);
  int a_SlotNum = ((int)  tolua_tonumber(tolua_S,2,0));
  const cItem* a_Item = ((const cItem*)  tolua_tousertype(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetSlot'", NULL);
#endif
  {
   self->SetSlot(a_SlotNum,*a_Item);
  }
 }
 return 0;
tolua_lerror:
 return tolua_AllToLua_cItemGrid_SetSlot01(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetSlot of class  cItemGrid */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cItemGrid_SetSlot03
static int tolua_AllToLua_cItemGrid_SetSlot03(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cItemGrid",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  cItemGrid* self = (cItemGrid*)  tolua_tousertype(tolua_S,1,0);
  int a_SlotNum = ((int)  tolua_tonumber(tolua_S,2,0));
  short a_ItemType = ((short)  tolua_tonumber(tolua_S,3,0));
  char a_ItemCount = ((char)  tolua_tonumber(tolua_S,4,0));
  short a_ItemDamage = ((short)  tolua_tonumber(tolua_S,5,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetSlot'", NULL);
#endif
  {
   self->SetSlot(a_SlotNum,a_ItemType,a_ItemCount,a_ItemDamage);
  }
 }
 return 0;
tolua_lerror:
 return tolua_AllToLua_cItemGrid_SetSlot02(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: EmptySlot of class  cItemGrid */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cItemGrid_EmptySlot00
static int tolua_AllToLua_cItemGrid_EmptySlot00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cItemGrid",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cItemGrid* self = (cItemGrid*)  tolua_tousertype(tolua_S,1,0);
  int a_X = ((int)  tolua_tonumber(tolua_S,2,0));
  int a_Y = ((int)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'EmptySlot'", NULL);
#endif
  {
   self->EmptySlot(a_X,a_Y);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'EmptySlot'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: EmptySlot of class  cItemGrid */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cItemGrid_EmptySlot01
static int tolua_AllToLua_cItemGrid_EmptySlot01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cItemGrid",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  cItemGrid* self = (cItemGrid*)  tolua_tousertype(tolua_S,1,0);
  int a_SlotNum = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'EmptySlot'", NULL);
#endif
  {
   self->EmptySlot(a_SlotNum);
  }
 }
 return 0;
tolua_lerror:
 return tolua_AllToLua_cItemGrid_EmptySlot00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: IsSlotEmpty of class  cItemGrid */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cItemGrid_IsSlotEmpty00
static int tolua_AllToLua_cItemGrid_IsSlotEmpty00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cItemGrid",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cItemGrid* self = (const cItemGrid*)  tolua_tousertype(tolua_S,1,0);
  int a_SlotNum = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'IsSlotEmpty'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->IsSlotEmpty(a_SlotNum);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'IsSlotEmpty'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: IsSlotEmpty of class  cItemGrid */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cItemGrid_IsSlotEmpty01
static int tolua_AllToLua_cItemGrid_IsSlotEmpty01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cItemGrid",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  const cItemGrid* self = (const cItemGrid*)  tolua_tousertype(tolua_S,1,0);
  int a_X = ((int)  tolua_tonumber(tolua_S,2,0));
  int a_Y = ((int)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'IsSlotEmpty'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->IsSlotEmpty(a_X,a_Y);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
tolua_lerror:
 return tolua_AllToLua_cItemGrid_IsSlotEmpty00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: Clear of class  cItemGrid */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cItemGrid_Clear00
static int tolua_AllToLua_cItemGrid_Clear00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cItemGrid",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cItemGrid* self = (cItemGrid*)  tolua_tousertype(tolua_S,1,0);
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

/* method: HowManyCanFit of class  cItemGrid */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cItemGrid_HowManyCanFit00
static int tolua_AllToLua_cItemGrid_HowManyCanFit00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cItemGrid",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const cItem",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cItemGrid* self = (cItemGrid*)  tolua_tousertype(tolua_S,1,0);
  const cItem* a_ItemStack = ((const cItem*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'HowManyCanFit'", NULL);
#endif
  {
   int tolua_ret = (int)  self->HowManyCanFit(*a_ItemStack);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'HowManyCanFit'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: AddItem of class  cItemGrid */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cItemGrid_AddItem00
static int tolua_AllToLua_cItemGrid_AddItem00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cItemGrid",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"cItem",0,&tolua_err)) ||
     !tolua_isboolean(tolua_S,3,1,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cItemGrid* self = (cItemGrid*)  tolua_tousertype(tolua_S,1,0);
  cItem* a_ItemStack = ((cItem*)  tolua_tousertype(tolua_S,2,0));
  bool a_AllowNewStacks = ((bool)  tolua_toboolean(tolua_S,3,true));
  int a_PrioritarySlot = ((int)  tolua_tonumber(tolua_S,4,-1));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'AddItem'", NULL);
#endif
  {
   int tolua_ret = (int)  self->AddItem(*a_ItemStack,a_AllowNewStacks,a_PrioritarySlot);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
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

/* method: AddItems of class  cItemGrid */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cItemGrid_AddItems00
static int tolua_AllToLua_cItemGrid_AddItems00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cItemGrid",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"cItems",0,&tolua_err)) ||
     !tolua_isboolean(tolua_S,3,1,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cItemGrid* self = (cItemGrid*)  tolua_tousertype(tolua_S,1,0);
  cItems* a_ItemStackList = ((cItems*)  tolua_tousertype(tolua_S,2,0));
  bool a_AllowNewStacks = ((bool)  tolua_toboolean(tolua_S,3,true));
  int a_PrioritarySlot = ((int)  tolua_tonumber(tolua_S,4,-1));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'AddItems'", NULL);
#endif
  {
   int tolua_ret = (int)  self->AddItems(*a_ItemStackList,a_AllowNewStacks,a_PrioritarySlot);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'AddItems'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: ChangeSlotCount of class  cItemGrid */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cItemGrid_ChangeSlotCount00
static int tolua_AllToLua_cItemGrid_ChangeSlotCount00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cItemGrid",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cItemGrid* self = (cItemGrid*)  tolua_tousertype(tolua_S,1,0);
  int a_SlotNum = ((int)  tolua_tonumber(tolua_S,2,0));
  int a_AddToCount = ((int)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'ChangeSlotCount'", NULL);
#endif
  {
   int tolua_ret = (int)  self->ChangeSlotCount(a_SlotNum,a_AddToCount);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'ChangeSlotCount'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: ChangeSlotCount of class  cItemGrid */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cItemGrid_ChangeSlotCount01
static int tolua_AllToLua_cItemGrid_ChangeSlotCount01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cItemGrid",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  cItemGrid* self = (cItemGrid*)  tolua_tousertype(tolua_S,1,0);
  int a_X = ((int)  tolua_tonumber(tolua_S,2,0));
  int a_Y = ((int)  tolua_tonumber(tolua_S,3,0));
  int a_AddToCount = ((int)  tolua_tonumber(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'ChangeSlotCount'", NULL);
#endif
  {
   int tolua_ret = (int)  self->ChangeSlotCount(a_X,a_Y,a_AddToCount);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
tolua_lerror:
 return tolua_AllToLua_cItemGrid_ChangeSlotCount00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: RemoveOneItem of class  cItemGrid */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cItemGrid_RemoveOneItem00
static int tolua_AllToLua_cItemGrid_RemoveOneItem00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cItemGrid",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cItemGrid* self = (cItemGrid*)  tolua_tousertype(tolua_S,1,0);
  int a_SlotNum = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'RemoveOneItem'", NULL);
#endif
  {
   cItem tolua_ret = (cItem)  self->RemoveOneItem(a_SlotNum);
   {
#ifdef __cplusplus
    void* tolua_obj = Mtolua_new((cItem)(tolua_ret));
     tolua_pushusertype(tolua_S,tolua_obj,"cItem");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(cItem));
     tolua_pushusertype(tolua_S,tolua_obj,"cItem");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'RemoveOneItem'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: RemoveOneItem of class  cItemGrid */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cItemGrid_RemoveOneItem01
static int tolua_AllToLua_cItemGrid_RemoveOneItem01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cItemGrid",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  cItemGrid* self = (cItemGrid*)  tolua_tousertype(tolua_S,1,0);
  int a_X = ((int)  tolua_tonumber(tolua_S,2,0));
  int a_Y = ((int)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'RemoveOneItem'", NULL);
#endif
  {
   cItem tolua_ret = (cItem)  self->RemoveOneItem(a_X,a_Y);
   {
#ifdef __cplusplus
    void* tolua_obj = Mtolua_new((cItem)(tolua_ret));
     tolua_pushusertype(tolua_S,tolua_obj,"cItem");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(cItem));
     tolua_pushusertype(tolua_S,tolua_obj,"cItem");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#endif
   }
  }
 }
 return 1;
tolua_lerror:
 return tolua_AllToLua_cItemGrid_RemoveOneItem00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: HowManyItems of class  cItemGrid */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cItemGrid_HowManyItems00
static int tolua_AllToLua_cItemGrid_HowManyItems00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cItemGrid",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const cItem",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cItemGrid* self = (cItemGrid*)  tolua_tousertype(tolua_S,1,0);
  const cItem* a_Item = ((const cItem*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'HowManyItems'", NULL);
#endif
  {
   int tolua_ret = (int)  self->HowManyItems(*a_Item);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'HowManyItems'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: HasItems of class  cItemGrid */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cItemGrid_HasItems00
static int tolua_AllToLua_cItemGrid_HasItems00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cItemGrid",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const cItem",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cItemGrid* self = (cItemGrid*)  tolua_tousertype(tolua_S,1,0);
  const cItem* a_ItemStack = ((const cItem*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'HasItems'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->HasItems(*a_ItemStack);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'HasItems'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetFirstEmptySlot of class  cItemGrid */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cItemGrid_GetFirstEmptySlot00
static int tolua_AllToLua_cItemGrid_GetFirstEmptySlot00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cItemGrid",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cItemGrid* self = (const cItemGrid*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetFirstEmptySlot'", NULL);
#endif
  {
   int tolua_ret = (int)  self->GetFirstEmptySlot();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetFirstEmptySlot'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetFirstUsedSlot of class  cItemGrid */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cItemGrid_GetFirstUsedSlot00
static int tolua_AllToLua_cItemGrid_GetFirstUsedSlot00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cItemGrid",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cItemGrid* self = (const cItemGrid*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetFirstUsedSlot'", NULL);
#endif
  {
   int tolua_ret = (int)  self->GetFirstUsedSlot();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetFirstUsedSlot'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetLastEmptySlot of class  cItemGrid */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cItemGrid_GetLastEmptySlot00
static int tolua_AllToLua_cItemGrid_GetLastEmptySlot00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cItemGrid",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cItemGrid* self = (const cItemGrid*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetLastEmptySlot'", NULL);
#endif
  {
   int tolua_ret = (int)  self->GetLastEmptySlot();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetLastEmptySlot'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetLastUsedSlot of class  cItemGrid */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cItemGrid_GetLastUsedSlot00
static int tolua_AllToLua_cItemGrid_GetLastUsedSlot00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cItemGrid",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cItemGrid* self = (const cItemGrid*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetLastUsedSlot'", NULL);
#endif
  {
   int tolua_ret = (int)  self->GetLastUsedSlot();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetLastUsedSlot'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetNextEmptySlot of class  cItemGrid */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cItemGrid_GetNextEmptySlot00
static int tolua_AllToLua_cItemGrid_GetNextEmptySlot00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cItemGrid",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cItemGrid* self = (const cItemGrid*)  tolua_tousertype(tolua_S,1,0);
  int a_StartFrom = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetNextEmptySlot'", NULL);
#endif
  {
   int tolua_ret = (int)  self->GetNextEmptySlot(a_StartFrom);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetNextEmptySlot'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetNextUsedSlot of class  cItemGrid */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cItemGrid_GetNextUsedSlot00
static int tolua_AllToLua_cItemGrid_GetNextUsedSlot00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cItemGrid",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cItemGrid* self = (const cItemGrid*)  tolua_tousertype(tolua_S,1,0);
  int a_StartFrom = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetNextUsedSlot'", NULL);
#endif
  {
   int tolua_ret = (int)  self->GetNextUsedSlot(a_StartFrom);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetNextUsedSlot'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: CopyToItems of class  cItemGrid */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cItemGrid_CopyToItems00
static int tolua_AllToLua_cItemGrid_CopyToItems00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cItemGrid",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"cItems",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cItemGrid* self = (const cItemGrid*)  tolua_tousertype(tolua_S,1,0);
  cItems* a_Items = ((cItems*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'CopyToItems'", NULL);
#endif
  {
   self->CopyToItems(*a_Items);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'CopyToItems'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: DamageItem of class  cItemGrid */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cItemGrid_DamageItem00
static int tolua_AllToLua_cItemGrid_DamageItem00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cItemGrid",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cItemGrid* self = (cItemGrid*)  tolua_tousertype(tolua_S,1,0);
  int a_SlotNum = ((int)  tolua_tonumber(tolua_S,2,0));
  short a_Amount = ((short)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'DamageItem'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->DamageItem(a_SlotNum,a_Amount);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'DamageItem'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: DamageItem of class  cItemGrid */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cItemGrid_DamageItem01
static int tolua_AllToLua_cItemGrid_DamageItem01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cItemGrid",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  cItemGrid* self = (cItemGrid*)  tolua_tousertype(tolua_S,1,0);
  int a_X = ((int)  tolua_tonumber(tolua_S,2,0));
  int a_Y = ((int)  tolua_tonumber(tolua_S,3,0));
  short a_Amount = ((short)  tolua_tonumber(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'DamageItem'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->DamageItem(a_X,a_Y,a_Amount);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
tolua_lerror:
 return tolua_AllToLua_cItemGrid_DamageItem00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetPosX of class  cBlockEntity */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cBlockEntity_GetPosX00
static int tolua_AllToLua_cBlockEntity_GetPosX00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cBlockEntity",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cBlockEntity* self = (const cBlockEntity*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetPosX'", NULL);
#endif
  {
   int tolua_ret = (int)  self->GetPosX();
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

/* method: GetPosY of class  cBlockEntity */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cBlockEntity_GetPosY00
static int tolua_AllToLua_cBlockEntity_GetPosY00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cBlockEntity",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cBlockEntity* self = (const cBlockEntity*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetPosY'", NULL);
#endif
  {
   int tolua_ret = (int)  self->GetPosY();
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

/* method: GetPosZ of class  cBlockEntity */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cBlockEntity_GetPosZ00
static int tolua_AllToLua_cBlockEntity_GetPosZ00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cBlockEntity",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cBlockEntity* self = (const cBlockEntity*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetPosZ'", NULL);
#endif
  {
   int tolua_ret = (int)  self->GetPosZ();
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

/* method: GetBlockType of class  cBlockEntity */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cBlockEntity_GetBlockType00
static int tolua_AllToLua_cBlockEntity_GetBlockType00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cBlockEntity",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cBlockEntity* self = (const cBlockEntity*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetBlockType'", NULL);
#endif
  {
   unsigned char tolua_ret = ( unsigned char)  self->GetBlockType();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetBlockType'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetWorld of class  cBlockEntity */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cBlockEntity_GetWorld00
static int tolua_AllToLua_cBlockEntity_GetWorld00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cBlockEntity",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cBlockEntity* self = (const cBlockEntity*)  tolua_tousertype(tolua_S,1,0);
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

/* method: GetChunkX of class  cBlockEntity */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cBlockEntity_GetChunkX00
static int tolua_AllToLua_cBlockEntity_GetChunkX00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cBlockEntity",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cBlockEntity* self = (const cBlockEntity*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetChunkX'", NULL);
#endif
  {
   int tolua_ret = (int)  self->GetChunkX();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetChunkX'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetChunkZ of class  cBlockEntity */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cBlockEntity_GetChunkZ00
static int tolua_AllToLua_cBlockEntity_GetChunkZ00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cBlockEntity",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cBlockEntity* self = (const cBlockEntity*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetChunkZ'", NULL);
#endif
  {
   int tolua_ret = (int)  self->GetChunkZ();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetChunkZ'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetRelX of class  cBlockEntity */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cBlockEntity_GetRelX00
static int tolua_AllToLua_cBlockEntity_GetRelX00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cBlockEntity",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cBlockEntity* self = (const cBlockEntity*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetRelX'", NULL);
#endif
  {
   int tolua_ret = (int)  self->GetRelX();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetRelX'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetRelZ of class  cBlockEntity */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cBlockEntity_GetRelZ00
static int tolua_AllToLua_cBlockEntity_GetRelZ00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cBlockEntity",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cBlockEntity* self = (const cBlockEntity*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetRelZ'", NULL);
#endif
  {
   int tolua_ret = (int)  self->GetRelZ();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetRelZ'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetSlot of class  cBlockEntityWithItems */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cBlockEntityWithItems_GetSlot00
static int tolua_AllToLua_cBlockEntityWithItems_GetSlot00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cBlockEntityWithItems",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cBlockEntityWithItems* self = (const cBlockEntityWithItems*)  tolua_tousertype(tolua_S,1,0);
  int a_SlotNum = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetSlot'", NULL);
#endif
  {
   const cItem& tolua_ret = (const cItem&)  self->GetSlot(a_SlotNum);
    tolua_pushusertype(tolua_S,(void*)&tolua_ret,"const cItem");
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

/* method: GetSlot of class  cBlockEntityWithItems */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cBlockEntityWithItems_GetSlot01
static int tolua_AllToLua_cBlockEntityWithItems_GetSlot01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cBlockEntityWithItems",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  const cBlockEntityWithItems* self = (const cBlockEntityWithItems*)  tolua_tousertype(tolua_S,1,0);
  int a_X = ((int)  tolua_tonumber(tolua_S,2,0));
  int a_Y = ((int)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetSlot'", NULL);
#endif
  {
   const cItem& tolua_ret = (const cItem&)  self->GetSlot(a_X,a_Y);
    tolua_pushusertype(tolua_S,(void*)&tolua_ret,"const cItem");
  }
 }
 return 1;
tolua_lerror:
 return tolua_AllToLua_cBlockEntityWithItems_GetSlot00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetSlot of class  cBlockEntityWithItems */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cBlockEntityWithItems_SetSlot00
static int tolua_AllToLua_cBlockEntityWithItems_SetSlot00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cBlockEntityWithItems",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,3,&tolua_err) || !tolua_isusertype(tolua_S,3,"const cItem",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cBlockEntityWithItems* self = (cBlockEntityWithItems*)  tolua_tousertype(tolua_S,1,0);
  int a_SlotNum = ((int)  tolua_tonumber(tolua_S,2,0));
  const cItem* a_Item = ((const cItem*)  tolua_tousertype(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetSlot'", NULL);
#endif
  {
   self->SetSlot(a_SlotNum,*a_Item);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetSlot'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetSlot of class  cBlockEntityWithItems */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cBlockEntityWithItems_SetSlot01
static int tolua_AllToLua_cBlockEntityWithItems_SetSlot01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cBlockEntityWithItems",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,4,&tolua_err) || !tolua_isusertype(tolua_S,4,"const cItem",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  cBlockEntityWithItems* self = (cBlockEntityWithItems*)  tolua_tousertype(tolua_S,1,0);
  int a_X = ((int)  tolua_tonumber(tolua_S,2,0));
  int a_Y = ((int)  tolua_tonumber(tolua_S,3,0));
  const cItem* a_Item = ((const cItem*)  tolua_tousertype(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetSlot'", NULL);
#endif
  {
   self->SetSlot(a_X,a_Y,*a_Item);
  }
 }
 return 0;
tolua_lerror:
 return tolua_AllToLua_cBlockEntityWithItems_SetSlot00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetContents of class  cBlockEntityWithItems */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cBlockEntityWithItems_GetContents00
static int tolua_AllToLua_cBlockEntityWithItems_GetContents00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cBlockEntityWithItems",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cBlockEntityWithItems* self = (cBlockEntityWithItems*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetContents'", NULL);
#endif
  {
   cItemGrid& tolua_ret = (cItemGrid&)  self->GetContents();
    tolua_pushusertype(tolua_S,(void*)&tolua_ret,"cItemGrid");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetContents'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  cChestEntity */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cChestEntity_new00
static int tolua_AllToLua_cChestEntity_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"cChestEntity",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  int a_BlockX = ((int)  tolua_tonumber(tolua_S,2,0));
  int a_BlockY = ((int)  tolua_tonumber(tolua_S,3,0));
  int a_BlockZ = ((int)  tolua_tonumber(tolua_S,4,0));
  {
   cChestEntity* tolua_ret = (cChestEntity*)  Mtolua_new((cChestEntity)(a_BlockX,a_BlockY,a_BlockZ));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"cChestEntity");
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

/* method: new_local of class  cChestEntity */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cChestEntity_new00_local
static int tolua_AllToLua_cChestEntity_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"cChestEntity",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  int a_BlockX = ((int)  tolua_tonumber(tolua_S,2,0));
  int a_BlockY = ((int)  tolua_tonumber(tolua_S,3,0));
  int a_BlockZ = ((int)  tolua_tonumber(tolua_S,4,0));
  {
   cChestEntity* tolua_ret = (cChestEntity*)  Mtolua_new((cChestEntity)(a_BlockX,a_BlockY,a_BlockZ));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"cChestEntity");
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

/* method: AddDropSpenserDir of class  cDropSpenserEntity */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cDropSpenserEntity_AddDropSpenserDir00
static int tolua_AllToLua_cDropSpenserEntity_AddDropSpenserDir00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cDropSpenserEntity",0,&tolua_err) ||
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
  cDropSpenserEntity* self = (cDropSpenserEntity*)  tolua_tousertype(tolua_S,1,0);
  int a_BlockX = ((int)  tolua_tonumber(tolua_S,2,0));
  int a_BlockY = ((int)  tolua_tonumber(tolua_S,3,0));
  int a_BlockZ = ((int)  tolua_tonumber(tolua_S,4,0));
  unsigned char a_Direction = (( unsigned char)  tolua_tonumber(tolua_S,5,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'AddDropSpenserDir'", NULL);
#endif
  {
   self->AddDropSpenserDir(a_BlockX,a_BlockY,a_BlockZ,a_Direction);
   tolua_pushnumber(tolua_S,(lua_Number)a_BlockX);
   tolua_pushnumber(tolua_S,(lua_Number)a_BlockY);
   tolua_pushnumber(tolua_S,(lua_Number)a_BlockZ);
  }
 }
 return 3;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'AddDropSpenserDir'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: Activate of class  cDropSpenserEntity */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cDropSpenserEntity_Activate00
static int tolua_AllToLua_cDropSpenserEntity_Activate00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cDropSpenserEntity",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cDropSpenserEntity* self = (cDropSpenserEntity*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'Activate'", NULL);
#endif
  {
   self->Activate();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Activate'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetRedstonePower of class  cDropSpenserEntity */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cDropSpenserEntity_SetRedstonePower00
static int tolua_AllToLua_cDropSpenserEntity_SetRedstonePower00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cDropSpenserEntity",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cDropSpenserEntity* self = (cDropSpenserEntity*)  tolua_tousertype(tolua_S,1,0);
  bool a_IsPowered = ((bool)  tolua_toboolean(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetRedstonePower'", NULL);
#endif
  {
   self->SetRedstonePower(a_IsPowered);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetRedstonePower'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* get function: __cBlockEntityWindowOwner__ of class  cDropSpenserEntity */
#ifndef TOLUA_DISABLE_tolua_get_cDropSpenserEntity___cBlockEntityWindowOwner__
static int tolua_get_cDropSpenserEntity___cBlockEntityWindowOwner__(lua_State* tolua_S)
{
  cDropSpenserEntity* self = (cDropSpenserEntity*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable '__cBlockEntityWindowOwner__'",NULL);
#endif
#ifdef __cplusplus
   tolua_pushusertype(tolua_S,(void*)static_cast<cBlockEntityWindowOwner*>(self), "cBlockEntityWindowOwner");
#else
   tolua_pushusertype(tolua_S,(void*)((cBlockEntityWindowOwner*)self), "cBlockEntityWindowOwner");
#endif
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  cDispenserEntity */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cDispenserEntity_new00
static int tolua_AllToLua_cDispenserEntity_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"cDispenserEntity",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  int a_BlockX = ((int)  tolua_tonumber(tolua_S,2,0));
  int a_BlockY = ((int)  tolua_tonumber(tolua_S,3,0));
  int a_BlockZ = ((int)  tolua_tonumber(tolua_S,4,0));
  {
   cDispenserEntity* tolua_ret = (cDispenserEntity*)  Mtolua_new((cDispenserEntity)(a_BlockX,a_BlockY,a_BlockZ));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"cDispenserEntity");
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

/* method: new_local of class  cDispenserEntity */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cDispenserEntity_new00_local
static int tolua_AllToLua_cDispenserEntity_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"cDispenserEntity",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  int a_BlockX = ((int)  tolua_tonumber(tolua_S,2,0));
  int a_BlockY = ((int)  tolua_tonumber(tolua_S,3,0));
  int a_BlockZ = ((int)  tolua_tonumber(tolua_S,4,0));
  {
   cDispenserEntity* tolua_ret = (cDispenserEntity*)  Mtolua_new((cDispenserEntity)(a_BlockX,a_BlockY,a_BlockZ));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"cDispenserEntity");
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

/* method: new of class  cDropperEntity */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cDropperEntity_new00
static int tolua_AllToLua_cDropperEntity_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"cDropperEntity",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  int a_BlockX = ((int)  tolua_tonumber(tolua_S,2,0));
  int a_BlockY = ((int)  tolua_tonumber(tolua_S,3,0));
  int a_BlockZ = ((int)  tolua_tonumber(tolua_S,4,0));
  {
   cDropperEntity* tolua_ret = (cDropperEntity*)  Mtolua_new((cDropperEntity)(a_BlockX,a_BlockY,a_BlockZ));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"cDropperEntity");
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

/* method: new_local of class  cDropperEntity */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cDropperEntity_new00_local
static int tolua_AllToLua_cDropperEntity_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"cDropperEntity",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  int a_BlockX = ((int)  tolua_tonumber(tolua_S,2,0));
  int a_BlockY = ((int)  tolua_tonumber(tolua_S,3,0));
  int a_BlockZ = ((int)  tolua_tonumber(tolua_S,4,0));
  {
   cDropperEntity* tolua_ret = (cDropperEntity*)  Mtolua_new((cDropperEntity)(a_BlockX,a_BlockY,a_BlockZ));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"cDropperEntity");
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

/* method: new of class  cFurnaceEntity */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cFurnaceEntity_new00
static int tolua_AllToLua_cFurnaceEntity_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"cFurnaceEntity",0,&tolua_err) ||
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
  int a_BlockX = ((int)  tolua_tonumber(tolua_S,2,0));
  int a_BlockY = ((int)  tolua_tonumber(tolua_S,3,0));
  int a_BlockZ = ((int)  tolua_tonumber(tolua_S,4,0));
  unsigned char a_BlockType = (( unsigned char)  tolua_tonumber(tolua_S,5,0));
  unsigned char a_BlockMeta = (( unsigned char)  tolua_tonumber(tolua_S,6,0));
  {
   cFurnaceEntity* tolua_ret = (cFurnaceEntity*)  Mtolua_new((cFurnaceEntity)(a_BlockX,a_BlockY,a_BlockZ,a_BlockType,a_BlockMeta));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"cFurnaceEntity");
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

/* method: new_local of class  cFurnaceEntity */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cFurnaceEntity_new00_local
static int tolua_AllToLua_cFurnaceEntity_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"cFurnaceEntity",0,&tolua_err) ||
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
  int a_BlockX = ((int)  tolua_tonumber(tolua_S,2,0));
  int a_BlockY = ((int)  tolua_tonumber(tolua_S,3,0));
  int a_BlockZ = ((int)  tolua_tonumber(tolua_S,4,0));
  unsigned char a_BlockType = (( unsigned char)  tolua_tonumber(tolua_S,5,0));
  unsigned char a_BlockMeta = (( unsigned char)  tolua_tonumber(tolua_S,6,0));
  {
   cFurnaceEntity* tolua_ret = (cFurnaceEntity*)  Mtolua_new((cFurnaceEntity)(a_BlockX,a_BlockY,a_BlockZ,a_BlockType,a_BlockMeta));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"cFurnaceEntity");
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

/* method: GetInputSlot of class  cFurnaceEntity */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cFurnaceEntity_GetInputSlot00
static int tolua_AllToLua_cFurnaceEntity_GetInputSlot00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cFurnaceEntity",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cFurnaceEntity* self = (const cFurnaceEntity*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetInputSlot'", NULL);
#endif
  {
   const cItem& tolua_ret = (const cItem&)  self->GetInputSlot();
    tolua_pushusertype(tolua_S,(void*)&tolua_ret,"const cItem");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetInputSlot'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetFuelSlot of class  cFurnaceEntity */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cFurnaceEntity_GetFuelSlot00
static int tolua_AllToLua_cFurnaceEntity_GetFuelSlot00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cFurnaceEntity",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cFurnaceEntity* self = (const cFurnaceEntity*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetFuelSlot'", NULL);
#endif
  {
   const cItem& tolua_ret = (const cItem&)  self->GetFuelSlot();
    tolua_pushusertype(tolua_S,(void*)&tolua_ret,"const cItem");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetFuelSlot'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetOutputSlot of class  cFurnaceEntity */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cFurnaceEntity_GetOutputSlot00
static int tolua_AllToLua_cFurnaceEntity_GetOutputSlot00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cFurnaceEntity",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cFurnaceEntity* self = (const cFurnaceEntity*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetOutputSlot'", NULL);
#endif
  {
   const cItem& tolua_ret = (const cItem&)  self->GetOutputSlot();
    tolua_pushusertype(tolua_S,(void*)&tolua_ret,"const cItem");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetOutputSlot'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetInputSlot of class  cFurnaceEntity */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cFurnaceEntity_SetInputSlot00
static int tolua_AllToLua_cFurnaceEntity_SetInputSlot00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cFurnaceEntity",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const cItem",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cFurnaceEntity* self = (cFurnaceEntity*)  tolua_tousertype(tolua_S,1,0);
  const cItem* a_Item = ((const cItem*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetInputSlot'", NULL);
#endif
  {
   self->SetInputSlot(*a_Item);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetInputSlot'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetFuelSlot of class  cFurnaceEntity */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cFurnaceEntity_SetFuelSlot00
static int tolua_AllToLua_cFurnaceEntity_SetFuelSlot00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cFurnaceEntity",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const cItem",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cFurnaceEntity* self = (cFurnaceEntity*)  tolua_tousertype(tolua_S,1,0);
  const cItem* a_Item = ((const cItem*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetFuelSlot'", NULL);
#endif
  {
   self->SetFuelSlot(*a_Item);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetFuelSlot'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetOutputSlot of class  cFurnaceEntity */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cFurnaceEntity_SetOutputSlot00
static int tolua_AllToLua_cFurnaceEntity_SetOutputSlot00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cFurnaceEntity",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const cItem",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cFurnaceEntity* self = (cFurnaceEntity*)  tolua_tousertype(tolua_S,1,0);
  const cItem* a_Item = ((const cItem*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetOutputSlot'", NULL);
#endif
  {
   self->SetOutputSlot(*a_Item);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetOutputSlot'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetTimeCooked of class  cFurnaceEntity */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cFurnaceEntity_GetTimeCooked00
static int tolua_AllToLua_cFurnaceEntity_GetTimeCooked00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cFurnaceEntity",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cFurnaceEntity* self = (const cFurnaceEntity*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetTimeCooked'", NULL);
#endif
  {
   int tolua_ret = (int)  self->GetTimeCooked();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetTimeCooked'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetCookTimeLeft of class  cFurnaceEntity */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cFurnaceEntity_GetCookTimeLeft00
static int tolua_AllToLua_cFurnaceEntity_GetCookTimeLeft00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cFurnaceEntity",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cFurnaceEntity* self = (const cFurnaceEntity*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetCookTimeLeft'", NULL);
#endif
  {
   int tolua_ret = (int)  self->GetCookTimeLeft();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetCookTimeLeft'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetFuelBurnTimeLeft of class  cFurnaceEntity */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cFurnaceEntity_GetFuelBurnTimeLeft00
static int tolua_AllToLua_cFurnaceEntity_GetFuelBurnTimeLeft00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cFurnaceEntity",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cFurnaceEntity* self = (const cFurnaceEntity*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetFuelBurnTimeLeft'", NULL);
#endif
  {
   int tolua_ret = (int)  self->GetFuelBurnTimeLeft();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetFuelBurnTimeLeft'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: HasFuelTimeLeft of class  cFurnaceEntity */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cFurnaceEntity_HasFuelTimeLeft00
static int tolua_AllToLua_cFurnaceEntity_HasFuelTimeLeft00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cFurnaceEntity",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cFurnaceEntity* self = (const cFurnaceEntity*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'HasFuelTimeLeft'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->HasFuelTimeLeft();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'HasFuelTimeLeft'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* get function: Name of class  HTTPFormData */
#ifndef TOLUA_DISABLE_tolua_get_HTTPFormData_Name
static int tolua_get_HTTPFormData_Name(lua_State* tolua_S)
{
  HTTPFormData* self = (HTTPFormData*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'Name'",NULL);
#endif
  tolua_pushcppstring(tolua_S,(const char*)self->Name);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: Name of class  HTTPFormData */
#ifndef TOLUA_DISABLE_tolua_set_HTTPFormData_Name
static int tolua_set_HTTPFormData_Name(lua_State* tolua_S)
{
  HTTPFormData* self = (HTTPFormData*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'Name'",NULL);
  if (!tolua_iscppstring(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->Name = ((std::string)  tolua_tocppstring(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: Value of class  HTTPFormData */
#ifndef TOLUA_DISABLE_tolua_get_HTTPFormData_Value
static int tolua_get_HTTPFormData_Value(lua_State* tolua_S)
{
  HTTPFormData* self = (HTTPFormData*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'Value'",NULL);
#endif
  tolua_pushcppstring(tolua_S,(const char*)self->Value);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: Value of class  HTTPFormData */
#ifndef TOLUA_DISABLE_tolua_set_HTTPFormData_Value
static int tolua_set_HTTPFormData_Value(lua_State* tolua_S)
{
  HTTPFormData* self = (HTTPFormData*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'Value'",NULL);
  if (!tolua_iscppstring(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->Value = ((std::string)  tolua_tocppstring(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: Type of class  HTTPFormData */
#ifndef TOLUA_DISABLE_tolua_get_HTTPFormData_Type
static int tolua_get_HTTPFormData_Type(lua_State* tolua_S)
{
  HTTPFormData* self = (HTTPFormData*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'Type'",NULL);
#endif
  tolua_pushcppstring(tolua_S,(const char*)self->Type);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: Type of class  HTTPFormData */
#ifndef TOLUA_DISABLE_tolua_set_HTTPFormData_Type
static int tolua_set_HTTPFormData_Type(lua_State* tolua_S)
{
  HTTPFormData* self = (HTTPFormData*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'Type'",NULL);
  if (!tolua_iscppstring(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->Type = ((std::string)  tolua_tocppstring(tolua_S,2,0))
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
  self->Method = ((AString)  tolua_tocppstring(tolua_S,2,0))
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
  self->Path = ((AString)  tolua_tocppstring(tolua_S,2,0))
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
  self->Username = ((AString)  tolua_tocppstring(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: Request of class  HTTPTemplateRequest */
#ifndef TOLUA_DISABLE_tolua_get_HTTPTemplateRequest_Request
static int tolua_get_HTTPTemplateRequest_Request(lua_State* tolua_S)
{
  HTTPTemplateRequest* self = (HTTPTemplateRequest*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'Request'",NULL);
#endif
   tolua_pushusertype(tolua_S,(void*)&self->Request,"HTTPRequest");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: Request of class  HTTPTemplateRequest */
#ifndef TOLUA_DISABLE_tolua_set_HTTPTemplateRequest_Request
static int tolua_set_HTTPTemplateRequest_Request(lua_State* tolua_S)
{
  HTTPTemplateRequest* self = (HTTPTemplateRequest*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'Request'",NULL);
  if ((tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"HTTPRequest",0,&tolua_err)))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->Request = *((HTTPRequest*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: Content of class  sWebAdminPage */
#ifndef TOLUA_DISABLE_tolua_get_sWebAdminPage_Content
static int tolua_get_sWebAdminPage_Content(lua_State* tolua_S)
{
  sWebAdminPage* self = (sWebAdminPage*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'Content'",NULL);
#endif
  tolua_pushcppstring(tolua_S,(const char*)self->Content);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: Content of class  sWebAdminPage */
#ifndef TOLUA_DISABLE_tolua_set_sWebAdminPage_Content
static int tolua_set_sWebAdminPage_Content(lua_State* tolua_S)
{
  sWebAdminPage* self = (sWebAdminPage*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'Content'",NULL);
  if (!tolua_iscppstring(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->Content = ((AString)  tolua_tocppstring(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: PluginName of class  sWebAdminPage */
#ifndef TOLUA_DISABLE_tolua_get_sWebAdminPage_PluginName
static int tolua_get_sWebAdminPage_PluginName(lua_State* tolua_S)
{
  sWebAdminPage* self = (sWebAdminPage*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'PluginName'",NULL);
#endif
  tolua_pushcppstring(tolua_S,(const char*)self->PluginName);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: PluginName of class  sWebAdminPage */
#ifndef TOLUA_DISABLE_tolua_set_sWebAdminPage_PluginName
static int tolua_set_sWebAdminPage_PluginName(lua_State* tolua_S)
{
  sWebAdminPage* self = (sWebAdminPage*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'PluginName'",NULL);
  if (!tolua_iscppstring(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->PluginName = ((AString)  tolua_tocppstring(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: TabName of class  sWebAdminPage */
#ifndef TOLUA_DISABLE_tolua_get_sWebAdminPage_TabName
static int tolua_get_sWebAdminPage_TabName(lua_State* tolua_S)
{
  sWebAdminPage* self = (sWebAdminPage*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'TabName'",NULL);
#endif
  tolua_pushcppstring(tolua_S,(const char*)self->TabName);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: TabName of class  sWebAdminPage */
#ifndef TOLUA_DISABLE_tolua_set_sWebAdminPage_TabName
static int tolua_set_sWebAdminPage_TabName(lua_State* tolua_S)
{
  sWebAdminPage* self = (sWebAdminPage*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'TabName'",NULL);
  if (!tolua_iscppstring(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->TabName = ((AString)  tolua_tocppstring(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetPort of class  cWebAdmin */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cWebAdmin_GetPort00
static int tolua_AllToLua_cWebAdmin_GetPort00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cWebAdmin",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cWebAdmin* self = (cWebAdmin*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetPort'", NULL);
#endif
  {
   int tolua_ret = (int)  self->GetPort();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetPort'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetPage of class  cWebAdmin */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cWebAdmin_GetPage00
static int tolua_AllToLua_cWebAdmin_GetPage00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cWebAdmin",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const HTTPRequest",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cWebAdmin* self = (cWebAdmin*)  tolua_tousertype(tolua_S,1,0);
  const HTTPRequest* a_Request = ((const HTTPRequest*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetPage'", NULL);
#endif
  {
   sWebAdminPage tolua_ret = (sWebAdminPage)  self->GetPage(*a_Request);
   {
#ifdef __cplusplus
    void* tolua_obj = Mtolua_new((sWebAdminPage)(tolua_ret));
     tolua_pushusertype(tolua_S,tolua_obj,"sWebAdminPage");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(sWebAdminPage));
     tolua_pushusertype(tolua_S,tolua_obj,"sWebAdminPage");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetPage'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetBaseURL of class  cWebAdmin */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cWebAdmin_GetBaseURL00
static int tolua_AllToLua_cWebAdmin_GetBaseURL00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cWebAdmin",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cWebAdmin* self = (cWebAdmin*)  tolua_tousertype(tolua_S,1,0);
  const AString a_URL = ((const AString)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetBaseURL'", NULL);
#endif
  {
   AString tolua_ret = (AString)  self->GetBaseURL(a_URL);
   tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
   tolua_pushcppstring(tolua_S,(const char*)a_URL);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetBaseURL'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetMemoryUsage of class  cWebAdmin */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cWebAdmin_GetMemoryUsage00
static int tolua_AllToLua_cWebAdmin_GetMemoryUsage00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"cWebAdmin",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   AString tolua_ret = (AString)  cWebAdmin::GetMemoryUsage();
   tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetMemoryUsage'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetWebTitle of class  cWebPlugin */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cWebPlugin_GetWebTitle00
static int tolua_AllToLua_cWebPlugin_GetWebTitle00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cWebPlugin",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cWebPlugin* self = (const cWebPlugin*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetWebTitle'", NULL);
#endif
  {
   const AString tolua_ret = (const AString)  self->GetWebTitle();
   tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetWebTitle'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: HandleWebRequest of class  cWebPlugin */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cWebPlugin_HandleWebRequest00
static int tolua_AllToLua_cWebPlugin_HandleWebRequest00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cWebPlugin",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const HTTPRequest",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cWebPlugin* self = (cWebPlugin*)  tolua_tousertype(tolua_S,1,0);
  const HTTPRequest* a_Request = ((const HTTPRequest*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'HandleWebRequest'", NULL);
#endif
  {
   AString tolua_ret = (AString)  self->HandleWebRequest(a_Request);
   tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'HandleWebRequest'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SafeString of class  cWebPlugin */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cWebPlugin_SafeString00
static int tolua_AllToLua_cWebPlugin_SafeString00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"cWebPlugin",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const AString a_String = ((const AString)  tolua_tocppstring(tolua_S,2,0));
  {
   AString tolua_ret = (AString)  cWebPlugin::SafeString(a_String);
   tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
   tolua_pushcppstring(tolua_S,(const char*)a_String);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SafeString'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

 class Lua__cWebPlugin : public cWebPlugin, public ToluaBase {
public:
	 const AString  GetWebTitle( void )const {
		if (push_method("GetWebTitle",  tolua_AllToLua_cWebPlugin_GetWebTitle00)) {
			ToluaBase::dbcall(lua_state, 1, 1);
			 const AString  tolua_ret = ( const AString )tolua_tocppstring(lua_state, -1, 0);
			lua_pop(lua_state, 1);
			return tolua_ret;
		} else {
			if (lua_state)
				LOG("pure-virtual method cWebPlugin::GetWebTitle not implemented.");
			else {
				LOG("pure-virtual method cWebPlugin::GetWebTitle called with no lua_state. Aborting");
				::abort();
			};
			return ( const AString )0;
		};
	};
	 AString  HandleWebRequest( const HTTPRequest* a_Request) {
		if (push_method("HandleWebRequest",  tolua_AllToLua_cWebPlugin_HandleWebRequest00)) {
			tolua_pushusertype(lua_state, (void*)a_Request, "const HTTPRequest");
			ToluaBase::dbcall(lua_state, 2, 1);
			 AString  tolua_ret = ( AString )tolua_tocppstring(lua_state, -1, 0);
			lua_pop(lua_state, 1);
			return tolua_ret;
		} else {
			if (lua_state)
				LOG("pure-virtual method cWebPlugin::HandleWebRequest not implemented.");
			else {
				LOG("pure-virtual method cWebPlugin::HandleWebRequest called with no lua_state. Aborting");
				::abort();
			};
			return ( AString )0;
		};
	};

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
  int a_MicroPosX = ((int)  tolua_tonumber(tolua_S,2,0));
  int a_MicroPosY = ((int)  tolua_tonumber(tolua_S,3,0));
  int a_MicroPosZ = ((int)  tolua_tonumber(tolua_S,4,0));
  const cItem* a_Item = ((const cItem*)  tolua_tousertype(tolua_S,5,0));
  float a_SpeedX = ((float)  tolua_tonumber(tolua_S,6,0.f));
  float a_SpeedY = ((float)  tolua_tonumber(tolua_S,7,0.f));
  float a_SpeedZ = ((float)  tolua_tonumber(tolua_S,8,0.f));
  {
   cPickup* tolua_ret = (cPickup*)  Mtolua_new((cPickup)(a_MicroPosX,a_MicroPosY,a_MicroPosZ,*a_Item,a_SpeedX,a_SpeedY,a_SpeedZ));
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
  int a_MicroPosX = ((int)  tolua_tonumber(tolua_S,2,0));
  int a_MicroPosY = ((int)  tolua_tonumber(tolua_S,3,0));
  int a_MicroPosZ = ((int)  tolua_tonumber(tolua_S,4,0));
  const cItem* a_Item = ((const cItem*)  tolua_tousertype(tolua_S,5,0));
  float a_SpeedX = ((float)  tolua_tonumber(tolua_S,6,0.f));
  float a_SpeedY = ((float)  tolua_tonumber(tolua_S,7,0.f));
  float a_SpeedZ = ((float)  tolua_tonumber(tolua_S,8,0.f));
  {
   cPickup* tolua_ret = (cPickup*)  Mtolua_new((cPickup)(a_MicroPosX,a_MicroPosY,a_MicroPosZ,*a_Item,a_SpeedX,a_SpeedY,a_SpeedZ));
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
   cItem& tolua_ret = (cItem&)  self->GetItem();
    tolua_pushusertype(tolua_S,(void*)&tolua_ret,"cItem");
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
	 bool  IsA( const char* a_ClassName)const {
		if (push_method("IsA",  tolua_AllToLua_cEntity_IsA00)) {
			tolua_pushstring(lua_state, (const char*)a_ClassName);
			ToluaBase::dbcall(lua_state, 2, 1);
			 bool  tolua_ret = ( bool )tolua_toboolean(lua_state, -1, 0);
			lua_pop(lua_state, 1);
			return tolua_ret;
		} else {
			return ( bool ) cPickup:: IsA(a_ClassName);
		};
	};
	 const char*  GetClass( void )const {
		if (push_method("GetClass",  tolua_AllToLua_cEntity_GetClass00)) {
			ToluaBase::dbcall(lua_state, 1, 1);
			 const char*  tolua_ret = ( const char* )tolua_tostring(lua_state, -1, 0);
			lua_pop(lua_state, 1);
			return tolua_ret;
		} else {
			return ( const char* ) cPickup:: GetClass();
		};
	};
	 const char*  GetParentClass( void )const {
		if (push_method("GetParentClass",  tolua_AllToLua_cEntity_GetParentClass00)) {
			ToluaBase::dbcall(lua_state, 1, 1);
			 const char*  tolua_ret = ( const char* )tolua_tostring(lua_state, -1, 0);
			lua_pop(lua_state, 1);
			return tolua_ret;
		} else {
			return ( const char* ) cPickup:: GetParentClass();
		};
	};
	 int  GetRawDamageAgainst( const cEntity& a_Receiver) {
		if (push_method("GetRawDamageAgainst",  tolua_AllToLua_cEntity_GetRawDamageAgainst00)) {
			tolua_pushusertype(lua_state, (void*)&a_Receiver, "const cEntity");
			ToluaBase::dbcall(lua_state, 2, 1);
			 int  tolua_ret = ( int )tolua_tonumber(lua_state, -1, 0);
			lua_pop(lua_state, 1);
			return tolua_ret;
		} else {
			return ( int ) cPickup:: GetRawDamageAgainst(a_Receiver);
		};
	};
	 int  GetArmorCoverAgainst( const cEntity* a_Attacker, eDamageType a_DamageType, int a_RawDamage) {
		if (push_method("GetArmorCoverAgainst",  tolua_AllToLua_cEntity_GetArmorCoverAgainst00)) {
			tolua_pushusertype(lua_state, (void*)a_Attacker, "const cEntity");
			tolua_pushnumber(lua_state, (lua_Number)a_DamageType);
			tolua_pushnumber(lua_state, (lua_Number)a_RawDamage);
			ToluaBase::dbcall(lua_state, 4, 1);
			 int  tolua_ret = ( int )tolua_tonumber(lua_state, -1, 0);
			lua_pop(lua_state, 1);
			return tolua_ret;
		} else {
			return ( int ) cPickup:: GetArmorCoverAgainst(a_Attacker,a_DamageType,a_RawDamage);
		};
	};
	 double  GetKnockbackAmountAgainst( const cEntity& a_Receiver) {
		if (push_method("GetKnockbackAmountAgainst",  tolua_AllToLua_cEntity_GetKnockbackAmountAgainst00)) {
			tolua_pushusertype(lua_state, (void*)&a_Receiver, "const cEntity");
			ToluaBase::dbcall(lua_state, 2, 1);
			 double  tolua_ret = ( double )tolua_tonumber(lua_state, -1, 0);
			lua_pop(lua_state, 1);
			return tolua_ret;
		} else {
			return ( double ) cPickup:: GetKnockbackAmountAgainst(a_Receiver);
		};
	};
	 cItem  GetEquippedWeapon( void )const {
		if (push_method("GetEquippedWeapon",  tolua_AllToLua_cEntity_GetEquippedWeapon00)) {
			ToluaBase::dbcall(lua_state, 1, 1);
			 cItem  tolua_ret = ( cItem )*(cItem*)tolua_tousertype(lua_state, -1, 0);
			lua_pop(lua_state, 1);
			return tolua_ret;
		} else {
			return ( cItem ) cPickup:: GetEquippedWeapon();
		};
	};
	 cItem  GetEquippedHelmet( void )const {
		if (push_method("GetEquippedHelmet",  tolua_AllToLua_cEntity_GetEquippedHelmet00)) {
			ToluaBase::dbcall(lua_state, 1, 1);
			 cItem  tolua_ret = ( cItem )*(cItem*)tolua_tousertype(lua_state, -1, 0);
			lua_pop(lua_state, 1);
			return tolua_ret;
		} else {
			return ( cItem ) cPickup:: GetEquippedHelmet();
		};
	};
	 cItem  GetEquippedChestplate( void )const {
		if (push_method("GetEquippedChestplate",  tolua_AllToLua_cEntity_GetEquippedChestplate00)) {
			ToluaBase::dbcall(lua_state, 1, 1);
			 cItem  tolua_ret = ( cItem )*(cItem*)tolua_tousertype(lua_state, -1, 0);
			lua_pop(lua_state, 1);
			return tolua_ret;
		} else {
			return ( cItem ) cPickup:: GetEquippedChestplate();
		};
	};
	 cItem  GetEquippedLeggings( void )const {
		if (push_method("GetEquippedLeggings",  tolua_AllToLua_cEntity_GetEquippedLeggings00)) {
			ToluaBase::dbcall(lua_state, 1, 1);
			 cItem  tolua_ret = ( cItem )*(cItem*)tolua_tousertype(lua_state, -1, 0);
			lua_pop(lua_state, 1);
			return tolua_ret;
		} else {
			return ( cItem ) cPickup:: GetEquippedLeggings();
		};
	};
	 cItem  GetEquippedBoots( void )const {
		if (push_method("GetEquippedBoots",  tolua_AllToLua_cEntity_GetEquippedBoots00)) {
			ToluaBase::dbcall(lua_state, 1, 1);
			 cItem  tolua_ret = ( cItem )*(cItem*)tolua_tousertype(lua_state, -1, 0);
			lua_pop(lua_state, 1);
			return tolua_ret;
		} else {
			return ( cItem ) cPickup:: GetEquippedBoots();
		};
	};
	 void  KilledBy( cEntity* a_Killer) {
		if (push_method("KilledBy",  tolua_AllToLua_cEntity_KilledBy00)) {
			tolua_pushusertype(lua_state, (void*)a_Killer, "cEntity");
			ToluaBase::dbcall(lua_state, 2, 0);
		} else {
			return ( void ) cPickup:: KilledBy(a_Killer);
		};
	};
	 void  TeleportToEntity( cEntity& a_Entity) {
		if (push_method("TeleportToEntity",  tolua_AllToLua_cEntity_TeleportToEntity00)) {
			tolua_pushusertype(lua_state, (void*)&a_Entity, "cEntity");
			ToluaBase::dbcall(lua_state, 2, 0);
		} else {
			return ( void ) cPickup:: TeleportToEntity(a_Entity);
		};
	};
	 void  TeleportToCoords( double a_PosX, double a_PosY, double a_PosZ) {
		if (push_method("TeleportToCoords",  tolua_AllToLua_cEntity_TeleportToCoords00)) {
			tolua_pushnumber(lua_state, (lua_Number)a_PosX);
			tolua_pushnumber(lua_state, (lua_Number)a_PosY);
			tolua_pushnumber(lua_state, (lua_Number)a_PosZ);
			ToluaBase::dbcall(lua_state, 4, 0);
		} else {
			return ( void ) cPickup:: TeleportToCoords(a_PosX,a_PosY,a_PosZ);
		};
	};
	 bool  IsOnFire( void )const {
		if (push_method("IsOnFire",  tolua_AllToLua_cEntity_IsOnFire00)) {
			ToluaBase::dbcall(lua_state, 1, 1);
			 bool  tolua_ret = ( bool )tolua_toboolean(lua_state, -1, 0);
			lua_pop(lua_state, 1);
			return tolua_ret;
		} else {
			return ( bool ) cPickup:: IsOnFire();
		};
	};
	 bool  IsCrouched( void )const {
		if (push_method("IsCrouched",  tolua_AllToLua_cEntity_IsCrouched00)) {
			ToluaBase::dbcall(lua_state, 1, 1);
			 bool  tolua_ret = ( bool )tolua_toboolean(lua_state, -1, 0);
			lua_pop(lua_state, 1);
			return tolua_ret;
		} else {
			return ( bool ) cPickup:: IsCrouched();
		};
	};
	 bool  IsRiding( void )const {
		if (push_method("IsRiding",  tolua_AllToLua_cEntity_IsRiding00)) {
			ToluaBase::dbcall(lua_state, 1, 1);
			 bool  tolua_ret = ( bool )tolua_toboolean(lua_state, -1, 0);
			lua_pop(lua_state, 1);
			return tolua_ret;
		} else {
			return ( bool ) cPickup:: IsRiding();
		};
	};
	 bool  IsSprinting( void )const {
		if (push_method("IsSprinting",  tolua_AllToLua_cEntity_IsSprinting00)) {
			ToluaBase::dbcall(lua_state, 1, 1);
			 bool  tolua_ret = ( bool )tolua_toboolean(lua_state, -1, 0);
			lua_pop(lua_state, 1);
			return tolua_ret;
		} else {
			return ( bool ) cPickup:: IsSprinting();
		};
	};
	 bool  IsRclking( void )const {
		if (push_method("IsRclking",  tolua_AllToLua_cEntity_IsRclking00)) {
			ToluaBase::dbcall(lua_state, 1, 1);
			 bool  tolua_ret = ( bool )tolua_toboolean(lua_state, -1, 0);
			lua_pop(lua_state, 1);
			return tolua_ret;
		} else {
			return ( bool ) cPickup:: IsRclking();
		};
	};

	 bool cPickup__CollectedBy( cPlayer* a_Dest) {
		return ( bool )cPickup::CollectedBy(a_Dest);
	};
	 bool cPickup__IsA( const char* a_ClassName) {
		return ( bool )cPickup::IsA(a_ClassName);
	};
	 const char* cPickup__GetClass( void ) {
		return ( const char* )cPickup::GetClass();
	};
	 const char* cPickup__GetParentClass( void ) {
		return ( const char* )cPickup::GetParentClass();
	};
	 int cPickup__GetRawDamageAgainst( const cEntity& a_Receiver) {
		return ( int )cPickup::GetRawDamageAgainst(a_Receiver);
	};
	 int cPickup__GetArmorCoverAgainst( const cEntity* a_Attacker, eDamageType a_DamageType, int a_RawDamage) {
		return ( int )cPickup::GetArmorCoverAgainst(a_Attacker,a_DamageType,a_RawDamage);
	};
	 double cPickup__GetKnockbackAmountAgainst( const cEntity& a_Receiver) {
		return ( double )cPickup::GetKnockbackAmountAgainst(a_Receiver);
	};
	 cItem cPickup__GetEquippedWeapon( void ) {
		return ( cItem )cPickup::GetEquippedWeapon();
	};
	 cItem cPickup__GetEquippedHelmet( void ) {
		return ( cItem )cPickup::GetEquippedHelmet();
	};
	 cItem cPickup__GetEquippedChestplate( void ) {
		return ( cItem )cPickup::GetEquippedChestplate();
	};
	 cItem cPickup__GetEquippedLeggings( void ) {
		return ( cItem )cPickup::GetEquippedLeggings();
	};
	 cItem cPickup__GetEquippedBoots( void ) {
		return ( cItem )cPickup::GetEquippedBoots();
	};
	 void cPickup__KilledBy( cEntity* a_Killer) {
		return ( void )cPickup::KilledBy(a_Killer);
	};
	 void cPickup__TeleportToEntity( cEntity& a_Entity) {
		return ( void )cPickup::TeleportToEntity(a_Entity);
	};
	 void cPickup__TeleportToCoords( double a_PosX, double a_PosY, double a_PosZ) {
		return ( void )cPickup::TeleportToCoords(a_PosX,a_PosY,a_PosZ);
	};
	 bool cPickup__IsOnFire( void ) {
		return ( bool )cPickup::IsOnFire();
	};
	 bool cPickup__IsCrouched( void ) {
		return ( bool )cPickup::IsCrouched();
	};
	 bool cPickup__IsRiding( void ) {
		return ( bool )cPickup::IsRiding();
	};
	 bool cPickup__IsSprinting( void ) {
		return ( bool )cPickup::IsSprinting();
	};
	 bool cPickup__IsRclking( void ) {
		return ( bool )cPickup::IsRclking();
	};
	 Lua__cPickup( int a_MicroPosX, int a_MicroPosY, int a_MicroPosZ, const cItem& a_Item, float a_SpeedX = 0.f, float a_SpeedY = 0.f, float a_SpeedZ = 0.f): cPickup(a_MicroPosX,a_MicroPosY,a_MicroPosZ,a_Item,a_SpeedX,a_SpeedY,a_SpeedZ){};
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
  int a_MicroPosX = ((int)  tolua_tonumber(tolua_S,2,0));
  int a_MicroPosY = ((int)  tolua_tonumber(tolua_S,3,0));
  int a_MicroPosZ = ((int)  tolua_tonumber(tolua_S,4,0));
  const cItem* a_Item = ((const cItem*)  tolua_tousertype(tolua_S,5,0));
  float a_SpeedX = ((float)  tolua_tonumber(tolua_S,6,0.f));
  float a_SpeedY = ((float)  tolua_tonumber(tolua_S,7,0.f));
  float a_SpeedZ = ((float)  tolua_tonumber(tolua_S,8,0.f));
  {
   Lua__cPickup* tolua_ret = (Lua__cPickup*)  Mtolua_new((Lua__cPickup)(a_MicroPosX,a_MicroPosY,a_MicroPosZ,*a_Item,a_SpeedX,a_SpeedY,a_SpeedZ));
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
  int a_MicroPosX = ((int)  tolua_tonumber(tolua_S,2,0));
  int a_MicroPosY = ((int)  tolua_tonumber(tolua_S,3,0));
  int a_MicroPosZ = ((int)  tolua_tonumber(tolua_S,4,0));
  const cItem* a_Item = ((const cItem*)  tolua_tousertype(tolua_S,5,0));
  float a_SpeedX = ((float)  tolua_tonumber(tolua_S,6,0.f));
  float a_SpeedY = ((float)  tolua_tonumber(tolua_S,7,0.f));
  float a_SpeedZ = ((float)  tolua_tonumber(tolua_S,8,0.f));
  {
   Lua__cPickup* tolua_ret = (Lua__cPickup*)  Mtolua_new((Lua__cPickup)(a_MicroPosX,a_MicroPosY,a_MicroPosZ,*a_Item,a_SpeedX,a_SpeedY,a_SpeedZ));
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


/* function to release collected object via destructor */
#ifdef __cplusplus

static int tolua_collect_Lua__cPickup (lua_State* tolua_S)
{
 Lua__cPickup* self = (Lua__cPickup*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}
#endif

/* get function: m_PrimaryServerVersion of class  cRoot */
#ifndef TOLUA_DISABLE_tolua_get_cRoot_m_PrimaryServerVersion
static int tolua_get_cRoot_m_PrimaryServerVersion(lua_State* tolua_S)
{
  cRoot* self = (cRoot*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_PrimaryServerVersion'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->m_PrimaryServerVersion);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: m_PrimaryServerVersion of class  cRoot */
#ifndef TOLUA_DISABLE_tolua_set_cRoot_m_PrimaryServerVersion
static int tolua_set_cRoot_m_PrimaryServerVersion(lua_State* tolua_S)
{
  cRoot* self = (cRoot*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_PrimaryServerVersion'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_PrimaryServerVersion = ((int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

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

/* method: GetDefaultWorld of class  cRoot */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cRoot_GetDefaultWorld00
static int tolua_AllToLua_cRoot_GetDefaultWorld00(lua_State* tolua_S)
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetDefaultWorld'", NULL);
#endif
  {
   cWorld* tolua_ret = (cWorld*)  self->GetDefaultWorld();
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"cWorld");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetDefaultWorld'.",&tolua_err);
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
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cRoot* self = (cRoot*)  tolua_tousertype(tolua_S,1,0);
  const AString a_WorldName = ((const AString)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetWorld'", NULL);
#endif
  {
   cWorld* tolua_ret = (cWorld*)  self->GetWorld(a_WorldName);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"cWorld");
   tolua_pushcppstring(tolua_S,(const char*)a_WorldName);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetWorld'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetPrimaryServerVersion of class  cRoot */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cRoot_GetPrimaryServerVersion00
static int tolua_AllToLua_cRoot_GetPrimaryServerVersion00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cRoot",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cRoot* self = (const cRoot*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetPrimaryServerVersion'", NULL);
#endif
  {
   int tolua_ret = (int)  self->GetPrimaryServerVersion();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetPrimaryServerVersion'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetPrimaryServerVersion of class  cRoot */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cRoot_SetPrimaryServerVersion00
static int tolua_AllToLua_cRoot_SetPrimaryServerVersion00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cRoot",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cRoot* self = (cRoot*)  tolua_tousertype(tolua_S,1,0);
  int a_Version = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetPrimaryServerVersion'", NULL);
#endif
  {
   self->SetPrimaryServerVersion(a_Version);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetPrimaryServerVersion'.",&tolua_err);
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

/* method: GetCraftingRecipes of class  cRoot */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cRoot_GetCraftingRecipes00
static int tolua_AllToLua_cRoot_GetCraftingRecipes00(lua_State* tolua_S)
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetCraftingRecipes'", NULL);
#endif
  {
   cCraftingRecipes* tolua_ret = (cCraftingRecipes*)  self->GetCraftingRecipes();
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"cCraftingRecipes");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetCraftingRecipes'.",&tolua_err);
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

/* method: QueueExecuteConsoleCommand of class  cRoot */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cRoot_QueueExecuteConsoleCommand00
static int tolua_AllToLua_cRoot_QueueExecuteConsoleCommand00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cRoot",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cRoot* self = (cRoot*)  tolua_tousertype(tolua_S,1,0);
  const AString a_Cmd = ((const AString)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'QueueExecuteConsoleCommand'", NULL);
#endif
  {
   self->QueueExecuteConsoleCommand(a_Cmd);
   tolua_pushcppstring(tolua_S,(const char*)a_Cmd);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'QueueExecuteConsoleCommand'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetTotalChunkCount of class  cRoot */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cRoot_GetTotalChunkCount00
static int tolua_AllToLua_cRoot_GetTotalChunkCount00(lua_State* tolua_S)
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetTotalChunkCount'", NULL);
#endif
  {
   int tolua_ret = (int)  self->GetTotalChunkCount();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetTotalChunkCount'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SaveAllChunks of class  cRoot */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cRoot_SaveAllChunks00
static int tolua_AllToLua_cRoot_SaveAllChunks00(lua_State* tolua_S)
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SaveAllChunks'", NULL);
#endif
  {
   self->SaveAllChunks();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SaveAllChunks'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetProtocolVersionTextFromInt of class  cRoot */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cRoot_GetProtocolVersionTextFromInt00
static int tolua_AllToLua_cRoot_GetProtocolVersionTextFromInt00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"cRoot",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  int a_ProtocolVersionNum = ((int)  tolua_tonumber(tolua_S,2,0));
  {
   AString tolua_ret = (AString)  cRoot::GetProtocolVersionTextFromInt(a_ProtocolVersionNum);
   tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetProtocolVersionTextFromInt'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

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

/* method: Equals of class  Vector3d */
#ifndef TOLUA_DISABLE_tolua_AllToLua_Vector3d_Equals00
static int tolua_AllToLua_Vector3d_Equals00(lua_State* tolua_S)
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

/* method: new of class  cCuboid */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cCuboid_new03
static int tolua_AllToLua_cCuboid_new03(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"cCuboid",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  int a_X1 = ((int)  tolua_tonumber(tolua_S,2,0));
  int a_Y1 = ((int)  tolua_tonumber(tolua_S,3,0));
  int a_Z1 = ((int)  tolua_tonumber(tolua_S,4,0));
  {
   cCuboid* tolua_ret = (cCuboid*)  Mtolua_new((cCuboid)(a_X1,a_Y1,a_Z1));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"cCuboid");
  }
 }
 return 1;
tolua_lerror:
 return tolua_AllToLua_cCuboid_new02(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  cCuboid */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cCuboid_new03_local
static int tolua_AllToLua_cCuboid_new03_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"cCuboid",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  int a_X1 = ((int)  tolua_tonumber(tolua_S,2,0));
  int a_Y1 = ((int)  tolua_tonumber(tolua_S,3,0));
  int a_Z1 = ((int)  tolua_tonumber(tolua_S,4,0));
  {
   cCuboid* tolua_ret = (cCuboid*)  Mtolua_new((cCuboid)(a_X1,a_Y1,a_Z1));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"cCuboid");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
  }
 }
 return 1;
tolua_lerror:
 return tolua_AllToLua_cCuboid_new02_local(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  cCuboid */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cCuboid_new04
static int tolua_AllToLua_cCuboid_new04(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"cCuboid",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,6,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,7,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,8,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  int a_X1 = ((int)  tolua_tonumber(tolua_S,2,0));
  int a_Y1 = ((int)  tolua_tonumber(tolua_S,3,0));
  int a_Z1 = ((int)  tolua_tonumber(tolua_S,4,0));
  int a_X2 = ((int)  tolua_tonumber(tolua_S,5,0));
  int a_Y2 = ((int)  tolua_tonumber(tolua_S,6,0));
  int a_Z2 = ((int)  tolua_tonumber(tolua_S,7,0));
  {
   cCuboid* tolua_ret = (cCuboid*)  Mtolua_new((cCuboid)(a_X1,a_Y1,a_Z1,a_X2,a_Y2,a_Z2));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"cCuboid");
  }
 }
 return 1;
tolua_lerror:
 return tolua_AllToLua_cCuboid_new03(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  cCuboid */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cCuboid_new04_local
static int tolua_AllToLua_cCuboid_new04_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"cCuboid",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,6,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,7,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,8,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  int a_X1 = ((int)  tolua_tonumber(tolua_S,2,0));
  int a_Y1 = ((int)  tolua_tonumber(tolua_S,3,0));
  int a_Z1 = ((int)  tolua_tonumber(tolua_S,4,0));
  int a_X2 = ((int)  tolua_tonumber(tolua_S,5,0));
  int a_Y2 = ((int)  tolua_tonumber(tolua_S,6,0));
  int a_Z2 = ((int)  tolua_tonumber(tolua_S,7,0));
  {
   cCuboid* tolua_ret = (cCuboid*)  Mtolua_new((cCuboid)(a_X1,a_Y1,a_Z1,a_X2,a_Y2,a_Z2));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"cCuboid");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
  }
 }
 return 1;
tolua_lerror:
 return tolua_AllToLua_cCuboid_new03_local(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: Assign of class  cCuboid */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cCuboid_Assign00
static int tolua_AllToLua_cCuboid_Assign00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cCuboid",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,6,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,7,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,8,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cCuboid* self = (cCuboid*)  tolua_tousertype(tolua_S,1,0);
  int a_X1 = ((int)  tolua_tonumber(tolua_S,2,0));
  int a_Y1 = ((int)  tolua_tonumber(tolua_S,3,0));
  int a_Z1 = ((int)  tolua_tonumber(tolua_S,4,0));
  int a_X2 = ((int)  tolua_tonumber(tolua_S,5,0));
  int a_Y2 = ((int)  tolua_tonumber(tolua_S,6,0));
  int a_Z2 = ((int)  tolua_tonumber(tolua_S,7,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'Assign'", NULL);
#endif
  {
   self->Assign(a_X1,a_Y1,a_Z1,a_X2,a_Y2,a_Z2);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Assign'.",&tolua_err);
 return 0;
#endif
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

/* method: DifX of class  cCuboid */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cCuboid_DifX00
static int tolua_AllToLua_cCuboid_DifX00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cCuboid",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cCuboid* self = (const cCuboid*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'DifX'", NULL);
#endif
  {
   int tolua_ret = (int)  self->DifX();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'DifX'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: DifY of class  cCuboid */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cCuboid_DifY00
static int tolua_AllToLua_cCuboid_DifY00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cCuboid",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cCuboid* self = (const cCuboid*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'DifY'", NULL);
#endif
  {
   int tolua_ret = (int)  self->DifY();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'DifY'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: DifZ of class  cCuboid */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cCuboid_DifZ00
static int tolua_AllToLua_cCuboid_DifZ00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cCuboid",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cCuboid* self = (const cCuboid*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'DifZ'", NULL);
#endif
  {
   int tolua_ret = (int)  self->DifZ();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'DifZ'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: DoesIntersect of class  cCuboid */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cCuboid_DoesIntersect00
static int tolua_AllToLua_cCuboid_DoesIntersect00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cCuboid",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const cCuboid",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cCuboid* self = (const cCuboid*)  tolua_tousertype(tolua_S,1,0);
  const cCuboid* a_Other = ((const cCuboid*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'DoesIntersect'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->DoesIntersect(*a_Other);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'DoesIntersect'.",&tolua_err);
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
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  const cCuboid* self = (const cCuboid*)  tolua_tousertype(tolua_S,1,0);
  int a_X = ((int)  tolua_tonumber(tolua_S,2,0));
  int a_Y = ((int)  tolua_tonumber(tolua_S,3,0));
  int a_Z = ((int)  tolua_tonumber(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'IsInside'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->IsInside(a_X,a_Y,a_Z);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
tolua_lerror:
 return tolua_AllToLua_cCuboid_IsInside00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: IsInside of class  cCuboid */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cCuboid_IsInside02
static int tolua_AllToLua_cCuboid_IsInside02(lua_State* tolua_S)
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
 return tolua_AllToLua_cCuboid_IsInside01(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: IsCompletelyInside of class  cCuboid */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cCuboid_IsCompletelyInside00
static int tolua_AllToLua_cCuboid_IsCompletelyInside00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cCuboid",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const cCuboid",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cCuboid* self = (const cCuboid*)  tolua_tousertype(tolua_S,1,0);
  const cCuboid* a_Outer = ((const cCuboid*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'IsCompletelyInside'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->IsCompletelyInside(*a_Outer);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'IsCompletelyInside'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: Move of class  cCuboid */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cCuboid_Move00
static int tolua_AllToLua_cCuboid_Move00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cCuboid",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cCuboid* self = (cCuboid*)  tolua_tousertype(tolua_S,1,0);
  int a_OfsX = ((int)  tolua_tonumber(tolua_S,2,0));
  int a_OfsY = ((int)  tolua_tonumber(tolua_S,3,0));
  int a_OfsZ = ((int)  tolua_tonumber(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'Move'", NULL);
#endif
  {
   self->Move(a_OfsX,a_OfsY,a_OfsZ);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Move'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: IsSorted of class  cCuboid */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cCuboid_IsSorted00
static int tolua_AllToLua_cCuboid_IsSorted00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cCuboid",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cCuboid* self = (const cCuboid*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'IsSorted'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->IsSorted();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'IsSorted'.",&tolua_err);
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
#ifndef TOLUA_DISABLE_tolua_get_cTracer_BlockHitPosition
static int tolua_get_cTracer_BlockHitPosition(lua_State* tolua_S)
{
  cTracer* self = (cTracer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'BlockHitPosition'",NULL);
#endif
   tolua_pushusertype(tolua_S,(void*)&self->BlockHitPosition,"Vector3f");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: BlockHitPosition of class  cTracer */
#ifndef TOLUA_DISABLE_tolua_set_cTracer_BlockHitPosition
static int tolua_set_cTracer_BlockHitPosition(lua_State* tolua_S)
{
  cTracer* self = (cTracer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'BlockHitPosition'",NULL);
  if ((tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"Vector3f",0,&tolua_err)))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->BlockHitPosition = *((Vector3f*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: HitNormal of class  cTracer */
#ifndef TOLUA_DISABLE_tolua_get_cTracer_HitNormal
static int tolua_get_cTracer_HitNormal(lua_State* tolua_S)
{
  cTracer* self = (cTracer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'HitNormal'",NULL);
#endif
   tolua_pushusertype(tolua_S,(void*)&self->HitNormal,"Vector3f");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: HitNormal of class  cTracer */
#ifndef TOLUA_DISABLE_tolua_set_cTracer_HitNormal
static int tolua_set_cTracer_HitNormal(lua_State* tolua_S)
{
  cTracer* self = (cTracer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'HitNormal'",NULL);
  if ((tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"Vector3f",0,&tolua_err)))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->HitNormal = *((Vector3f*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: RealHit of class  cTracer */
#ifndef TOLUA_DISABLE_tolua_get_cTracer_RealHit
static int tolua_get_cTracer_RealHit(lua_State* tolua_S)
{
  cTracer* self = (cTracer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'RealHit'",NULL);
#endif
   tolua_pushusertype(tolua_S,(void*)&self->RealHit,"Vector3f");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: RealHit of class  cTracer */
#ifndef TOLUA_DISABLE_tolua_set_cTracer_RealHit
static int tolua_set_cTracer_RealHit(lua_State* tolua_S)
{
  cTracer* self = (cTracer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'RealHit'",NULL);
  if ((tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"Vector3f",0,&tolua_err)))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->RealHit = *((Vector3f*)  tolua_tousertype(tolua_S,2,0))
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
   const AString tolua_ret = (const AString)  self->GetColor();
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

/* method: new of class  cBlockArea */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cBlockArea_new00
static int tolua_AllToLua_cBlockArea_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"cBlockArea",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   cBlockArea* tolua_ret = (cBlockArea*)  Mtolua_new((cBlockArea)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"cBlockArea");
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

/* method: new_local of class  cBlockArea */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cBlockArea_new00_local
static int tolua_AllToLua_cBlockArea_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"cBlockArea",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   cBlockArea* tolua_ret = (cBlockArea*)  Mtolua_new((cBlockArea)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"cBlockArea");
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

/* method: delete of class  cBlockArea */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cBlockArea_delete00
static int tolua_AllToLua_cBlockArea_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cBlockArea",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cBlockArea* self = (cBlockArea*)  tolua_tousertype(tolua_S,1,0);
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

/* method: Clear of class  cBlockArea */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cBlockArea_Clear00
static int tolua_AllToLua_cBlockArea_Clear00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cBlockArea",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cBlockArea* self = (cBlockArea*)  tolua_tousertype(tolua_S,1,0);
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

/* method: Create of class  cBlockArea */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cBlockArea_Create00
static int tolua_AllToLua_cBlockArea_Create00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cBlockArea",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cBlockArea* self = (cBlockArea*)  tolua_tousertype(tolua_S,1,0);
  int a_SizeX = ((int)  tolua_tonumber(tolua_S,2,0));
  int a_SizeY = ((int)  tolua_tonumber(tolua_S,3,0));
  int a_SizeZ = ((int)  tolua_tonumber(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'Create'", NULL);
#endif
  {
   self->Create(a_SizeX,a_SizeY,a_SizeZ);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Create'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: Create of class  cBlockArea */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cBlockArea_Create01
static int tolua_AllToLua_cBlockArea_Create01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cBlockArea",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  cBlockArea* self = (cBlockArea*)  tolua_tousertype(tolua_S,1,0);
  int a_SizeX = ((int)  tolua_tonumber(tolua_S,2,0));
  int a_SizeY = ((int)  tolua_tonumber(tolua_S,3,0));
  int a_SizeZ = ((int)  tolua_tonumber(tolua_S,4,0));
  int a_DataTypes = ((int)  tolua_tonumber(tolua_S,5,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'Create'", NULL);
#endif
  {
   self->Create(a_SizeX,a_SizeY,a_SizeZ,a_DataTypes);
  }
 }
 return 0;
tolua_lerror:
 return tolua_AllToLua_cBlockArea_Create00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetOrigin of class  cBlockArea */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cBlockArea_SetOrigin00
static int tolua_AllToLua_cBlockArea_SetOrigin00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cBlockArea",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cBlockArea* self = (cBlockArea*)  tolua_tousertype(tolua_S,1,0);
  int a_OriginX = ((int)  tolua_tonumber(tolua_S,2,0));
  int a_OriginY = ((int)  tolua_tonumber(tolua_S,3,0));
  int a_OriginZ = ((int)  tolua_tonumber(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetOrigin'", NULL);
#endif
  {
   self->SetOrigin(a_OriginX,a_OriginY,a_OriginZ);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetOrigin'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: Read of class  cBlockArea */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cBlockArea_Read00
static int tolua_AllToLua_cBlockArea_Read00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cBlockArea",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"cWorld",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,6,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,7,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,8,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,9,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cBlockArea* self = (cBlockArea*)  tolua_tousertype(tolua_S,1,0);
  cWorld* a_World = ((cWorld*)  tolua_tousertype(tolua_S,2,0));
  int a_MinBlockX = ((int)  tolua_tonumber(tolua_S,3,0));
  int a_MaxBlockX = ((int)  tolua_tonumber(tolua_S,4,0));
  int a_MinBlockY = ((int)  tolua_tonumber(tolua_S,5,0));
  int a_MaxBlockY = ((int)  tolua_tonumber(tolua_S,6,0));
  int a_MinBlockZ = ((int)  tolua_tonumber(tolua_S,7,0));
  int a_MaxBlockZ = ((int)  tolua_tonumber(tolua_S,8,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'Read'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->Read(a_World,a_MinBlockX,a_MaxBlockX,a_MinBlockY,a_MaxBlockY,a_MinBlockZ,a_MaxBlockZ);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Read'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: Read of class  cBlockArea */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cBlockArea_Read01
static int tolua_AllToLua_cBlockArea_Read01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cBlockArea",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"cWorld",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,6,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,7,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,8,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,9,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,10,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  cBlockArea* self = (cBlockArea*)  tolua_tousertype(tolua_S,1,0);
  cWorld* a_World = ((cWorld*)  tolua_tousertype(tolua_S,2,0));
  int a_MinBlockX = ((int)  tolua_tonumber(tolua_S,3,0));
  int a_MaxBlockX = ((int)  tolua_tonumber(tolua_S,4,0));
  int a_MinBlockY = ((int)  tolua_tonumber(tolua_S,5,0));
  int a_MaxBlockY = ((int)  tolua_tonumber(tolua_S,6,0));
  int a_MinBlockZ = ((int)  tolua_tonumber(tolua_S,7,0));
  int a_MaxBlockZ = ((int)  tolua_tonumber(tolua_S,8,0));
  int a_DataTypes = ((int)  tolua_tonumber(tolua_S,9,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'Read'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->Read(a_World,a_MinBlockX,a_MaxBlockX,a_MinBlockY,a_MaxBlockY,a_MinBlockZ,a_MaxBlockZ,a_DataTypes);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
tolua_lerror:
 return tolua_AllToLua_cBlockArea_Read00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: Write of class  cBlockArea */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cBlockArea_Write00
static int tolua_AllToLua_cBlockArea_Write00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cBlockArea",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"cWorld",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cBlockArea* self = (cBlockArea*)  tolua_tousertype(tolua_S,1,0);
  cWorld* a_World = ((cWorld*)  tolua_tousertype(tolua_S,2,0));
  int a_MinBlockX = ((int)  tolua_tonumber(tolua_S,3,0));
  int a_MinBlockY = ((int)  tolua_tonumber(tolua_S,4,0));
  int a_MinBlockZ = ((int)  tolua_tonumber(tolua_S,5,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'Write'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->Write(a_World,a_MinBlockX,a_MinBlockY,a_MinBlockZ);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Write'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: Write of class  cBlockArea */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cBlockArea_Write01
static int tolua_AllToLua_cBlockArea_Write01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cBlockArea",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"cWorld",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,6,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,7,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  cBlockArea* self = (cBlockArea*)  tolua_tousertype(tolua_S,1,0);
  cWorld* a_World = ((cWorld*)  tolua_tousertype(tolua_S,2,0));
  int a_MinBlockX = ((int)  tolua_tonumber(tolua_S,3,0));
  int a_MinBlockY = ((int)  tolua_tonumber(tolua_S,4,0));
  int a_MinBlockZ = ((int)  tolua_tonumber(tolua_S,5,0));
  int a_DataTypes = ((int)  tolua_tonumber(tolua_S,6,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'Write'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->Write(a_World,a_MinBlockX,a_MinBlockY,a_MinBlockZ,a_DataTypes);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
tolua_lerror:
 return tolua_AllToLua_cBlockArea_Write00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: CopyTo of class  cBlockArea */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cBlockArea_CopyTo00
static int tolua_AllToLua_cBlockArea_CopyTo00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cBlockArea",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"cBlockArea",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cBlockArea* self = (const cBlockArea*)  tolua_tousertype(tolua_S,1,0);
  cBlockArea* a_Into = ((cBlockArea*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'CopyTo'", NULL);
#endif
  {
   self->CopyTo(*a_Into);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'CopyTo'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: CopyFrom of class  cBlockArea */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cBlockArea_CopyFrom00
static int tolua_AllToLua_cBlockArea_CopyFrom00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cBlockArea",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const cBlockArea",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cBlockArea* self = (cBlockArea*)  tolua_tousertype(tolua_S,1,0);
  const cBlockArea* a_From = ((const cBlockArea*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'CopyFrom'", NULL);
#endif
  {
   self->CopyFrom(*a_From);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'CopyFrom'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: DumpToRawFile of class  cBlockArea */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cBlockArea_DumpToRawFile00
static int tolua_AllToLua_cBlockArea_DumpToRawFile00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cBlockArea",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cBlockArea* self = (cBlockArea*)  tolua_tousertype(tolua_S,1,0);
  const AString a_FileName = ((const AString)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'DumpToRawFile'", NULL);
#endif
  {
   self->DumpToRawFile(a_FileName);
   tolua_pushcppstring(tolua_S,(const char*)a_FileName);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'DumpToRawFile'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: LoadFromSchematicFile of class  cBlockArea */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cBlockArea_LoadFromSchematicFile00
static int tolua_AllToLua_cBlockArea_LoadFromSchematicFile00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cBlockArea",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cBlockArea* self = (cBlockArea*)  tolua_tousertype(tolua_S,1,0);
  const AString a_FileName = ((const AString)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'LoadFromSchematicFile'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->LoadFromSchematicFile(a_FileName);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
   tolua_pushcppstring(tolua_S,(const char*)a_FileName);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'LoadFromSchematicFile'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SaveToSchematicFile of class  cBlockArea */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cBlockArea_SaveToSchematicFile00
static int tolua_AllToLua_cBlockArea_SaveToSchematicFile00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cBlockArea",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cBlockArea* self = (cBlockArea*)  tolua_tousertype(tolua_S,1,0);
  const AString a_FileName = ((const AString)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SaveToSchematicFile'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->SaveToSchematicFile(a_FileName);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
   tolua_pushcppstring(tolua_S,(const char*)a_FileName);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SaveToSchematicFile'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: Crop of class  cBlockArea */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cBlockArea_Crop00
static int tolua_AllToLua_cBlockArea_Crop00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cBlockArea",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,6,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,7,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,8,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cBlockArea* self = (cBlockArea*)  tolua_tousertype(tolua_S,1,0);
  int a_AddMinX = ((int)  tolua_tonumber(tolua_S,2,0));
  int a_SubMaxX = ((int)  tolua_tonumber(tolua_S,3,0));
  int a_AddMinY = ((int)  tolua_tonumber(tolua_S,4,0));
  int a_SubMaxY = ((int)  tolua_tonumber(tolua_S,5,0));
  int a_AddMinZ = ((int)  tolua_tonumber(tolua_S,6,0));
  int a_SubMaxZ = ((int)  tolua_tonumber(tolua_S,7,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'Crop'", NULL);
#endif
  {
   self->Crop(a_AddMinX,a_SubMaxX,a_AddMinY,a_SubMaxY,a_AddMinZ,a_SubMaxZ);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Crop'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: Expand of class  cBlockArea */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cBlockArea_Expand00
static int tolua_AllToLua_cBlockArea_Expand00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cBlockArea",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,6,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,7,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,8,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cBlockArea* self = (cBlockArea*)  tolua_tousertype(tolua_S,1,0);
  int a_SubMinX = ((int)  tolua_tonumber(tolua_S,2,0));
  int a_AddMaxX = ((int)  tolua_tonumber(tolua_S,3,0));
  int a_SubMinY = ((int)  tolua_tonumber(tolua_S,4,0));
  int a_AddMaxY = ((int)  tolua_tonumber(tolua_S,5,0));
  int a_SubMinZ = ((int)  tolua_tonumber(tolua_S,6,0));
  int a_AddMaxZ = ((int)  tolua_tonumber(tolua_S,7,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'Expand'", NULL);
#endif
  {
   self->Expand(a_SubMinX,a_AddMaxX,a_SubMinY,a_AddMaxY,a_SubMinZ,a_AddMaxZ);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Expand'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: Merge of class  cBlockArea */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cBlockArea_Merge00
static int tolua_AllToLua_cBlockArea_Merge00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cBlockArea",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const cBlockArea",0,&tolua_err)) ||
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
  cBlockArea* self = (cBlockArea*)  tolua_tousertype(tolua_S,1,0);
  const cBlockArea* a_Src = ((const cBlockArea*)  tolua_tousertype(tolua_S,2,0));
  int a_RelX = ((int)  tolua_tonumber(tolua_S,3,0));
  int a_RelY = ((int)  tolua_tonumber(tolua_S,4,0));
  int a_RelZ = ((int)  tolua_tonumber(tolua_S,5,0));
  cBlockArea::eMergeStrategy a_Strategy = ((cBlockArea::eMergeStrategy) (int)  tolua_tonumber(tolua_S,6,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'Merge'", NULL);
#endif
  {
   self->Merge(*a_Src,a_RelX,a_RelY,a_RelZ,a_Strategy);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Merge'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: Fill of class  cBlockArea */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cBlockArea_Fill00
static int tolua_AllToLua_cBlockArea_Fill00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cBlockArea",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,1,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,1,&tolua_err) ||
     !tolua_isnumber(tolua_S,6,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,7,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cBlockArea* self = (cBlockArea*)  tolua_tousertype(tolua_S,1,0);
  int a_DataTypes = ((int)  tolua_tonumber(tolua_S,2,0));
  unsigned char a_BlockType = (( unsigned char)  tolua_tonumber(tolua_S,3,0));
  unsigned char a_BlockMeta = (( unsigned char)  tolua_tonumber(tolua_S,4,0));
  unsigned char a_BlockLight = (( unsigned char)  tolua_tonumber(tolua_S,5,0));
  unsigned char a_BlockSkyLight = (( unsigned char)  tolua_tonumber(tolua_S,6,0x0f));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'Fill'", NULL);
#endif
  {
   self->Fill(a_DataTypes,a_BlockType,a_BlockMeta,a_BlockLight,a_BlockSkyLight);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Fill'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: FillRelCuboid of class  cBlockArea */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cBlockArea_FillRelCuboid00
static int tolua_AllToLua_cBlockArea_FillRelCuboid00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cBlockArea",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,6,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,7,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,8,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,9,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,10,1,&tolua_err) ||
     !tolua_isnumber(tolua_S,11,1,&tolua_err) ||
     !tolua_isnumber(tolua_S,12,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,13,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cBlockArea* self = (cBlockArea*)  tolua_tousertype(tolua_S,1,0);
  int a_MinRelX = ((int)  tolua_tonumber(tolua_S,2,0));
  int a_MaxRelX = ((int)  tolua_tonumber(tolua_S,3,0));
  int a_MinRelY = ((int)  tolua_tonumber(tolua_S,4,0));
  int a_MaxRelY = ((int)  tolua_tonumber(tolua_S,5,0));
  int a_MinRelZ = ((int)  tolua_tonumber(tolua_S,6,0));
  int a_MaxRelZ = ((int)  tolua_tonumber(tolua_S,7,0));
  int a_DataTypes = ((int)  tolua_tonumber(tolua_S,8,0));
  unsigned char a_BlockType = (( unsigned char)  tolua_tonumber(tolua_S,9,0));
  unsigned char a_BlockMeta = (( unsigned char)  tolua_tonumber(tolua_S,10,0));
  unsigned char a_BlockLight = (( unsigned char)  tolua_tonumber(tolua_S,11,0));
  unsigned char a_BlockSkyLight = (( unsigned char)  tolua_tonumber(tolua_S,12,0x0f));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'FillRelCuboid'", NULL);
#endif
  {
   self->FillRelCuboid(a_MinRelX,a_MaxRelX,a_MinRelY,a_MaxRelY,a_MinRelZ,a_MaxRelZ,a_DataTypes,a_BlockType,a_BlockMeta,a_BlockLight,a_BlockSkyLight);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'FillRelCuboid'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: RelLine of class  cBlockArea */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cBlockArea_RelLine00
static int tolua_AllToLua_cBlockArea_RelLine00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cBlockArea",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,6,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,7,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,8,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,9,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,10,1,&tolua_err) ||
     !tolua_isnumber(tolua_S,11,1,&tolua_err) ||
     !tolua_isnumber(tolua_S,12,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,13,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cBlockArea* self = (cBlockArea*)  tolua_tousertype(tolua_S,1,0);
  int a_RelX1 = ((int)  tolua_tonumber(tolua_S,2,0));
  int a_RelY1 = ((int)  tolua_tonumber(tolua_S,3,0));
  int a_RelZ1 = ((int)  tolua_tonumber(tolua_S,4,0));
  int a_RelX2 = ((int)  tolua_tonumber(tolua_S,5,0));
  int a_RelY2 = ((int)  tolua_tonumber(tolua_S,6,0));
  int a_RelZ2 = ((int)  tolua_tonumber(tolua_S,7,0));
  int a_DataTypes = ((int)  tolua_tonumber(tolua_S,8,0));
  unsigned char a_BlockType = (( unsigned char)  tolua_tonumber(tolua_S,9,0));
  unsigned char a_BlockMeta = (( unsigned char)  tolua_tonumber(tolua_S,10,0));
  unsigned char a_BlockLight = (( unsigned char)  tolua_tonumber(tolua_S,11,0));
  unsigned char a_BlockSkyLight = (( unsigned char)  tolua_tonumber(tolua_S,12,0x0f));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'RelLine'", NULL);
#endif
  {
   self->RelLine(a_RelX1,a_RelY1,a_RelZ1,a_RelX2,a_RelY2,a_RelZ2,a_DataTypes,a_BlockType,a_BlockMeta,a_BlockLight,a_BlockSkyLight);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'RelLine'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: RotateCCW of class  cBlockArea */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cBlockArea_RotateCCW00
static int tolua_AllToLua_cBlockArea_RotateCCW00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cBlockArea",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cBlockArea* self = (cBlockArea*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'RotateCCW'", NULL);
#endif
  {
   self->RotateCCW();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'RotateCCW'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: RotateCW of class  cBlockArea */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cBlockArea_RotateCW00
static int tolua_AllToLua_cBlockArea_RotateCW00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cBlockArea",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cBlockArea* self = (cBlockArea*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'RotateCW'", NULL);
#endif
  {
   self->RotateCW();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'RotateCW'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: MirrorXY of class  cBlockArea */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cBlockArea_MirrorXY00
static int tolua_AllToLua_cBlockArea_MirrorXY00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cBlockArea",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cBlockArea* self = (cBlockArea*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'MirrorXY'", NULL);
#endif
  {
   self->MirrorXY();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'MirrorXY'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: MirrorXZ of class  cBlockArea */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cBlockArea_MirrorXZ00
static int tolua_AllToLua_cBlockArea_MirrorXZ00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cBlockArea",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cBlockArea* self = (cBlockArea*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'MirrorXZ'", NULL);
#endif
  {
   self->MirrorXZ();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'MirrorXZ'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: MirrorYZ of class  cBlockArea */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cBlockArea_MirrorYZ00
static int tolua_AllToLua_cBlockArea_MirrorYZ00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cBlockArea",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cBlockArea* self = (cBlockArea*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'MirrorYZ'", NULL);
#endif
  {
   self->MirrorYZ();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'MirrorYZ'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: RotateCCWNoMeta of class  cBlockArea */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cBlockArea_RotateCCWNoMeta00
static int tolua_AllToLua_cBlockArea_RotateCCWNoMeta00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cBlockArea",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cBlockArea* self = (cBlockArea*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'RotateCCWNoMeta'", NULL);
#endif
  {
   self->RotateCCWNoMeta();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'RotateCCWNoMeta'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: RotateCWNoMeta of class  cBlockArea */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cBlockArea_RotateCWNoMeta00
static int tolua_AllToLua_cBlockArea_RotateCWNoMeta00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cBlockArea",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cBlockArea* self = (cBlockArea*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'RotateCWNoMeta'", NULL);
#endif
  {
   self->RotateCWNoMeta();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'RotateCWNoMeta'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: MirrorXYNoMeta of class  cBlockArea */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cBlockArea_MirrorXYNoMeta00
static int tolua_AllToLua_cBlockArea_MirrorXYNoMeta00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cBlockArea",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cBlockArea* self = (cBlockArea*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'MirrorXYNoMeta'", NULL);
#endif
  {
   self->MirrorXYNoMeta();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'MirrorXYNoMeta'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: MirrorXZNoMeta of class  cBlockArea */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cBlockArea_MirrorXZNoMeta00
static int tolua_AllToLua_cBlockArea_MirrorXZNoMeta00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cBlockArea",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cBlockArea* self = (cBlockArea*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'MirrorXZNoMeta'", NULL);
#endif
  {
   self->MirrorXZNoMeta();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'MirrorXZNoMeta'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: MirrorYZNoMeta of class  cBlockArea */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cBlockArea_MirrorYZNoMeta00
static int tolua_AllToLua_cBlockArea_MirrorYZNoMeta00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cBlockArea",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cBlockArea* self = (cBlockArea*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'MirrorYZNoMeta'", NULL);
#endif
  {
   self->MirrorYZNoMeta();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'MirrorYZNoMeta'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetRelBlockType of class  cBlockArea */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cBlockArea_SetRelBlockType00
static int tolua_AllToLua_cBlockArea_SetRelBlockType00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cBlockArea",0,&tolua_err) ||
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
  cBlockArea* self = (cBlockArea*)  tolua_tousertype(tolua_S,1,0);
  int a_RelX = ((int)  tolua_tonumber(tolua_S,2,0));
  int a_RelY = ((int)  tolua_tonumber(tolua_S,3,0));
  int a_RelZ = ((int)  tolua_tonumber(tolua_S,4,0));
  unsigned char a_BlockType = (( unsigned char)  tolua_tonumber(tolua_S,5,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetRelBlockType'", NULL);
#endif
  {
   self->SetRelBlockType(a_RelX,a_RelY,a_RelZ,a_BlockType);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetRelBlockType'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetBlockType of class  cBlockArea */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cBlockArea_SetBlockType00
static int tolua_AllToLua_cBlockArea_SetBlockType00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cBlockArea",0,&tolua_err) ||
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
  cBlockArea* self = (cBlockArea*)  tolua_tousertype(tolua_S,1,0);
  int a_BlockX = ((int)  tolua_tonumber(tolua_S,2,0));
  int a_BlockY = ((int)  tolua_tonumber(tolua_S,3,0));
  int a_BlockZ = ((int)  tolua_tonumber(tolua_S,4,0));
  unsigned char a_BlockType = (( unsigned char)  tolua_tonumber(tolua_S,5,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetBlockType'", NULL);
#endif
  {
   self->SetBlockType(a_BlockX,a_BlockY,a_BlockZ,a_BlockType);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetBlockType'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetRelBlockMeta of class  cBlockArea */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cBlockArea_SetRelBlockMeta00
static int tolua_AllToLua_cBlockArea_SetRelBlockMeta00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cBlockArea",0,&tolua_err) ||
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
  cBlockArea* self = (cBlockArea*)  tolua_tousertype(tolua_S,1,0);
  int a_RelX = ((int)  tolua_tonumber(tolua_S,2,0));
  int a_RelY = ((int)  tolua_tonumber(tolua_S,3,0));
  int a_RelZ = ((int)  tolua_tonumber(tolua_S,4,0));
  unsigned char a_BlockMeta = (( unsigned char)  tolua_tonumber(tolua_S,5,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetRelBlockMeta'", NULL);
#endif
  {
   self->SetRelBlockMeta(a_RelX,a_RelY,a_RelZ,a_BlockMeta);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetRelBlockMeta'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetBlockMeta of class  cBlockArea */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cBlockArea_SetBlockMeta00
static int tolua_AllToLua_cBlockArea_SetBlockMeta00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cBlockArea",0,&tolua_err) ||
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
  cBlockArea* self = (cBlockArea*)  tolua_tousertype(tolua_S,1,0);
  int a_BlockX = ((int)  tolua_tonumber(tolua_S,2,0));
  int a_BlockY = ((int)  tolua_tonumber(tolua_S,3,0));
  int a_BlockZ = ((int)  tolua_tonumber(tolua_S,4,0));
  unsigned char a_BlockMeta = (( unsigned char)  tolua_tonumber(tolua_S,5,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetBlockMeta'", NULL);
#endif
  {
   self->SetBlockMeta(a_BlockX,a_BlockY,a_BlockZ,a_BlockMeta);
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

/* method: SetRelBlockLight of class  cBlockArea */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cBlockArea_SetRelBlockLight00
static int tolua_AllToLua_cBlockArea_SetRelBlockLight00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cBlockArea",0,&tolua_err) ||
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
  cBlockArea* self = (cBlockArea*)  tolua_tousertype(tolua_S,1,0);
  int a_RelX = ((int)  tolua_tonumber(tolua_S,2,0));
  int a_RelY = ((int)  tolua_tonumber(tolua_S,3,0));
  int a_RelZ = ((int)  tolua_tonumber(tolua_S,4,0));
  unsigned char a_BlockLight = (( unsigned char)  tolua_tonumber(tolua_S,5,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetRelBlockLight'", NULL);
#endif
  {
   self->SetRelBlockLight(a_RelX,a_RelY,a_RelZ,a_BlockLight);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetRelBlockLight'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetBlockLight of class  cBlockArea */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cBlockArea_SetBlockLight00
static int tolua_AllToLua_cBlockArea_SetBlockLight00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cBlockArea",0,&tolua_err) ||
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
  cBlockArea* self = (cBlockArea*)  tolua_tousertype(tolua_S,1,0);
  int a_BlockX = ((int)  tolua_tonumber(tolua_S,2,0));
  int a_BlockY = ((int)  tolua_tonumber(tolua_S,3,0));
  int a_BlockZ = ((int)  tolua_tonumber(tolua_S,4,0));
  unsigned char a_BlockLight = (( unsigned char)  tolua_tonumber(tolua_S,5,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetBlockLight'", NULL);
#endif
  {
   self->SetBlockLight(a_BlockX,a_BlockY,a_BlockZ,a_BlockLight);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetBlockLight'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetRelBlockSkyLight of class  cBlockArea */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cBlockArea_SetRelBlockSkyLight00
static int tolua_AllToLua_cBlockArea_SetRelBlockSkyLight00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cBlockArea",0,&tolua_err) ||
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
  cBlockArea* self = (cBlockArea*)  tolua_tousertype(tolua_S,1,0);
  int a_RelX = ((int)  tolua_tonumber(tolua_S,2,0));
  int a_RelY = ((int)  tolua_tonumber(tolua_S,3,0));
  int a_RelZ = ((int)  tolua_tonumber(tolua_S,4,0));
  unsigned char a_BlockSkyLight = (( unsigned char)  tolua_tonumber(tolua_S,5,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetRelBlockSkyLight'", NULL);
#endif
  {
   self->SetRelBlockSkyLight(a_RelX,a_RelY,a_RelZ,a_BlockSkyLight);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetRelBlockSkyLight'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetBlockSkyLight of class  cBlockArea */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cBlockArea_SetBlockSkyLight00
static int tolua_AllToLua_cBlockArea_SetBlockSkyLight00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cBlockArea",0,&tolua_err) ||
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
  cBlockArea* self = (cBlockArea*)  tolua_tousertype(tolua_S,1,0);
  int a_BlockX = ((int)  tolua_tonumber(tolua_S,2,0));
  int a_BlockY = ((int)  tolua_tonumber(tolua_S,3,0));
  int a_BlockZ = ((int)  tolua_tonumber(tolua_S,4,0));
  unsigned char a_BlockSkyLight = (( unsigned char)  tolua_tonumber(tolua_S,5,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetBlockSkyLight'", NULL);
#endif
  {
   self->SetBlockSkyLight(a_BlockX,a_BlockY,a_BlockZ,a_BlockSkyLight);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetBlockSkyLight'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetRelBlockType of class  cBlockArea */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cBlockArea_GetRelBlockType00
static int tolua_AllToLua_cBlockArea_GetRelBlockType00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cBlockArea",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cBlockArea* self = (const cBlockArea*)  tolua_tousertype(tolua_S,1,0);
  int a_RelX = ((int)  tolua_tonumber(tolua_S,2,0));
  int a_RelY = ((int)  tolua_tonumber(tolua_S,3,0));
  int a_RelZ = ((int)  tolua_tonumber(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetRelBlockType'", NULL);
#endif
  {
   unsigned char tolua_ret = ( unsigned char)  self->GetRelBlockType(a_RelX,a_RelY,a_RelZ);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetRelBlockType'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetBlockType of class  cBlockArea */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cBlockArea_GetBlockType00
static int tolua_AllToLua_cBlockArea_GetBlockType00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cBlockArea",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cBlockArea* self = (const cBlockArea*)  tolua_tousertype(tolua_S,1,0);
  int a_BlockX = ((int)  tolua_tonumber(tolua_S,2,0));
  int a_BlockY = ((int)  tolua_tonumber(tolua_S,3,0));
  int a_BlockZ = ((int)  tolua_tonumber(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetBlockType'", NULL);
#endif
  {
   unsigned char tolua_ret = ( unsigned char)  self->GetBlockType(a_BlockX,a_BlockY,a_BlockZ);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetBlockType'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetRelBlockMeta of class  cBlockArea */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cBlockArea_GetRelBlockMeta00
static int tolua_AllToLua_cBlockArea_GetRelBlockMeta00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cBlockArea",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cBlockArea* self = (const cBlockArea*)  tolua_tousertype(tolua_S,1,0);
  int a_RelX = ((int)  tolua_tonumber(tolua_S,2,0));
  int a_RelY = ((int)  tolua_tonumber(tolua_S,3,0));
  int a_RelZ = ((int)  tolua_tonumber(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetRelBlockMeta'", NULL);
#endif
  {
   unsigned char tolua_ret = ( unsigned char)  self->GetRelBlockMeta(a_RelX,a_RelY,a_RelZ);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetRelBlockMeta'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetBlockMeta of class  cBlockArea */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cBlockArea_GetBlockMeta00
static int tolua_AllToLua_cBlockArea_GetBlockMeta00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cBlockArea",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cBlockArea* self = (const cBlockArea*)  tolua_tousertype(tolua_S,1,0);
  int a_BlockX = ((int)  tolua_tonumber(tolua_S,2,0));
  int a_BlockY = ((int)  tolua_tonumber(tolua_S,3,0));
  int a_BlockZ = ((int)  tolua_tonumber(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetBlockMeta'", NULL);
#endif
  {
   unsigned char tolua_ret = ( unsigned char)  self->GetBlockMeta(a_BlockX,a_BlockY,a_BlockZ);
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

/* method: GetRelBlockLight of class  cBlockArea */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cBlockArea_GetRelBlockLight00
static int tolua_AllToLua_cBlockArea_GetRelBlockLight00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cBlockArea",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cBlockArea* self = (const cBlockArea*)  tolua_tousertype(tolua_S,1,0);
  int a_RelX = ((int)  tolua_tonumber(tolua_S,2,0));
  int a_RelY = ((int)  tolua_tonumber(tolua_S,3,0));
  int a_RelZ = ((int)  tolua_tonumber(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetRelBlockLight'", NULL);
#endif
  {
   unsigned char tolua_ret = ( unsigned char)  self->GetRelBlockLight(a_RelX,a_RelY,a_RelZ);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetRelBlockLight'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetBlockLight of class  cBlockArea */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cBlockArea_GetBlockLight00
static int tolua_AllToLua_cBlockArea_GetBlockLight00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cBlockArea",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cBlockArea* self = (const cBlockArea*)  tolua_tousertype(tolua_S,1,0);
  int a_BlockX = ((int)  tolua_tonumber(tolua_S,2,0));
  int a_BlockY = ((int)  tolua_tonumber(tolua_S,3,0));
  int a_BlockZ = ((int)  tolua_tonumber(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetBlockLight'", NULL);
#endif
  {
   unsigned char tolua_ret = ( unsigned char)  self->GetBlockLight(a_BlockX,a_BlockY,a_BlockZ);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetBlockLight'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetRelBlockSkyLight of class  cBlockArea */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cBlockArea_GetRelBlockSkyLight00
static int tolua_AllToLua_cBlockArea_GetRelBlockSkyLight00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cBlockArea",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cBlockArea* self = (const cBlockArea*)  tolua_tousertype(tolua_S,1,0);
  int a_RelX = ((int)  tolua_tonumber(tolua_S,2,0));
  int a_RelY = ((int)  tolua_tonumber(tolua_S,3,0));
  int a_RelZ = ((int)  tolua_tonumber(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetRelBlockSkyLight'", NULL);
#endif
  {
   unsigned char tolua_ret = ( unsigned char)  self->GetRelBlockSkyLight(a_RelX,a_RelY,a_RelZ);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetRelBlockSkyLight'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetBlockSkyLight of class  cBlockArea */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cBlockArea_GetBlockSkyLight00
static int tolua_AllToLua_cBlockArea_GetBlockSkyLight00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cBlockArea",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cBlockArea* self = (const cBlockArea*)  tolua_tousertype(tolua_S,1,0);
  int a_BlockX = ((int)  tolua_tonumber(tolua_S,2,0));
  int a_BlockY = ((int)  tolua_tonumber(tolua_S,3,0));
  int a_BlockZ = ((int)  tolua_tonumber(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetBlockSkyLight'", NULL);
#endif
  {
   unsigned char tolua_ret = ( unsigned char)  self->GetBlockSkyLight(a_BlockX,a_BlockY,a_BlockZ);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetBlockSkyLight'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetBlockTypeMeta of class  cBlockArea */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cBlockArea_SetBlockTypeMeta00
static int tolua_AllToLua_cBlockArea_SetBlockTypeMeta00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cBlockArea",0,&tolua_err) ||
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
  cBlockArea* self = (cBlockArea*)  tolua_tousertype(tolua_S,1,0);
  int a_BlockX = ((int)  tolua_tonumber(tolua_S,2,0));
  int a_BlockY = ((int)  tolua_tonumber(tolua_S,3,0));
  int a_BlockZ = ((int)  tolua_tonumber(tolua_S,4,0));
  unsigned char a_BlockType = (( unsigned char)  tolua_tonumber(tolua_S,5,0));
  unsigned char a_BlockMeta = (( unsigned char)  tolua_tonumber(tolua_S,6,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetBlockTypeMeta'", NULL);
#endif
  {
   self->SetBlockTypeMeta(a_BlockX,a_BlockY,a_BlockZ,a_BlockType,a_BlockMeta);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetBlockTypeMeta'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetRelBlockTypeMeta of class  cBlockArea */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cBlockArea_SetRelBlockTypeMeta00
static int tolua_AllToLua_cBlockArea_SetRelBlockTypeMeta00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cBlockArea",0,&tolua_err) ||
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
  cBlockArea* self = (cBlockArea*)  tolua_tousertype(tolua_S,1,0);
  int a_RelX = ((int)  tolua_tonumber(tolua_S,2,0));
  int a_RelY = ((int)  tolua_tonumber(tolua_S,3,0));
  int a_RelZ = ((int)  tolua_tonumber(tolua_S,4,0));
  unsigned char a_BlockType = (( unsigned char)  tolua_tonumber(tolua_S,5,0));
  unsigned char a_BlockMeta = (( unsigned char)  tolua_tonumber(tolua_S,6,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetRelBlockTypeMeta'", NULL);
#endif
  {
   self->SetRelBlockTypeMeta(a_RelX,a_RelY,a_RelZ,a_BlockType,a_BlockMeta);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetRelBlockTypeMeta'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetBlockTypeMeta of class  cBlockArea */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cBlockArea_GetBlockTypeMeta00
static int tolua_AllToLua_cBlockArea_GetBlockTypeMeta00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cBlockArea",0,&tolua_err) ||
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
  const cBlockArea* self = (const cBlockArea*)  tolua_tousertype(tolua_S,1,0);
  int a_BlockX = ((int)  tolua_tonumber(tolua_S,2,0));
  int a_BlockY = ((int)  tolua_tonumber(tolua_S,3,0));
  int a_BlockZ = ((int)  tolua_tonumber(tolua_S,4,0));
  unsigned char a_BlockType = (( unsigned char)  tolua_tonumber(tolua_S,5,0));
  unsigned char a_BlockMeta = (( unsigned char)  tolua_tonumber(tolua_S,6,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetBlockTypeMeta'", NULL);
#endif
  {
   self->GetBlockTypeMeta(a_BlockX,a_BlockY,a_BlockZ,a_BlockType,a_BlockMeta);
   tolua_pushnumber(tolua_S,(lua_Number)a_BlockType);
   tolua_pushnumber(tolua_S,(lua_Number)a_BlockMeta);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetBlockTypeMeta'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetRelBlockTypeMeta of class  cBlockArea */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cBlockArea_GetRelBlockTypeMeta00
static int tolua_AllToLua_cBlockArea_GetRelBlockTypeMeta00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cBlockArea",0,&tolua_err) ||
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
  const cBlockArea* self = (const cBlockArea*)  tolua_tousertype(tolua_S,1,0);
  int a_RelX = ((int)  tolua_tonumber(tolua_S,2,0));
  int a_RelY = ((int)  tolua_tonumber(tolua_S,3,0));
  int a_RelZ = ((int)  tolua_tonumber(tolua_S,4,0));
  unsigned char a_BlockType = (( unsigned char)  tolua_tonumber(tolua_S,5,0));
  unsigned char a_BlockMeta = (( unsigned char)  tolua_tonumber(tolua_S,6,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetRelBlockTypeMeta'", NULL);
#endif
  {
   self->GetRelBlockTypeMeta(a_RelX,a_RelY,a_RelZ,a_BlockType,a_BlockMeta);
   tolua_pushnumber(tolua_S,(lua_Number)a_BlockType);
   tolua_pushnumber(tolua_S,(lua_Number)a_BlockMeta);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetRelBlockTypeMeta'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetSizeX of class  cBlockArea */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cBlockArea_GetSizeX00
static int tolua_AllToLua_cBlockArea_GetSizeX00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cBlockArea",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cBlockArea* self = (const cBlockArea*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetSizeX'", NULL);
#endif
  {
   int tolua_ret = (int)  self->GetSizeX();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetSizeX'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetSizeY of class  cBlockArea */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cBlockArea_GetSizeY00
static int tolua_AllToLua_cBlockArea_GetSizeY00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cBlockArea",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cBlockArea* self = (const cBlockArea*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetSizeY'", NULL);
#endif
  {
   int tolua_ret = (int)  self->GetSizeY();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetSizeY'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetSizeZ of class  cBlockArea */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cBlockArea_GetSizeZ00
static int tolua_AllToLua_cBlockArea_GetSizeZ00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cBlockArea",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cBlockArea* self = (const cBlockArea*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetSizeZ'", NULL);
#endif
  {
   int tolua_ret = (int)  self->GetSizeZ();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetSizeZ'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetOriginX of class  cBlockArea */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cBlockArea_GetOriginX00
static int tolua_AllToLua_cBlockArea_GetOriginX00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cBlockArea",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cBlockArea* self = (const cBlockArea*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetOriginX'", NULL);
#endif
  {
   int tolua_ret = (int)  self->GetOriginX();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetOriginX'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetOriginY of class  cBlockArea */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cBlockArea_GetOriginY00
static int tolua_AllToLua_cBlockArea_GetOriginY00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cBlockArea",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cBlockArea* self = (const cBlockArea*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetOriginY'", NULL);
#endif
  {
   int tolua_ret = (int)  self->GetOriginY();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetOriginY'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetOriginZ of class  cBlockArea */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cBlockArea_GetOriginZ00
static int tolua_AllToLua_cBlockArea_GetOriginZ00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cBlockArea",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cBlockArea* self = (const cBlockArea*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetOriginZ'", NULL);
#endif
  {
   int tolua_ret = (int)  self->GetOriginZ();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetOriginZ'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetDataTypes of class  cBlockArea */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cBlockArea_GetDataTypes00
static int tolua_AllToLua_cBlockArea_GetDataTypes00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cBlockArea",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cBlockArea* self = (const cBlockArea*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetDataTypes'", NULL);
#endif
  {
   int tolua_ret = (int)  self->GetDataTypes();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetDataTypes'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: HasBlockTypes of class  cBlockArea */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cBlockArea_HasBlockTypes00
static int tolua_AllToLua_cBlockArea_HasBlockTypes00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cBlockArea",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cBlockArea* self = (const cBlockArea*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'HasBlockTypes'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->HasBlockTypes();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'HasBlockTypes'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: HasBlockMetas of class  cBlockArea */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cBlockArea_HasBlockMetas00
static int tolua_AllToLua_cBlockArea_HasBlockMetas00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cBlockArea",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cBlockArea* self = (const cBlockArea*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'HasBlockMetas'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->HasBlockMetas();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'HasBlockMetas'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: HasBlockLights of class  cBlockArea */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cBlockArea_HasBlockLights00
static int tolua_AllToLua_cBlockArea_HasBlockLights00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cBlockArea",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cBlockArea* self = (const cBlockArea*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'HasBlockLights'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->HasBlockLights();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'HasBlockLights'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: HasBlockSkyLights of class  cBlockArea */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cBlockArea_HasBlockSkyLights00
static int tolua_AllToLua_cBlockArea_HasBlockSkyLights00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cBlockArea",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cBlockArea* self = (const cBlockArea*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'HasBlockSkyLights'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->HasBlockSkyLights();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'HasBlockSkyLights'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetChunkX of class  cChunkDesc */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cChunkDesc_GetChunkX00
static int tolua_AllToLua_cChunkDesc_GetChunkX00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cChunkDesc",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cChunkDesc* self = (const cChunkDesc*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetChunkX'", NULL);
#endif
  {
   int tolua_ret = (int)  self->GetChunkX();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetChunkX'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetChunkZ of class  cChunkDesc */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cChunkDesc_GetChunkZ00
static int tolua_AllToLua_cChunkDesc_GetChunkZ00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cChunkDesc",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cChunkDesc* self = (const cChunkDesc*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetChunkZ'", NULL);
#endif
  {
   int tolua_ret = (int)  self->GetChunkZ();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetChunkZ'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetChunkCoords of class  cChunkDesc */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cChunkDesc_SetChunkCoords00
static int tolua_AllToLua_cChunkDesc_SetChunkCoords00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cChunkDesc",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cChunkDesc* self = (cChunkDesc*)  tolua_tousertype(tolua_S,1,0);
  int a_ChunkX = ((int)  tolua_tonumber(tolua_S,2,0));
  int a_ChunkZ = ((int)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetChunkCoords'", NULL);
#endif
  {
   self->SetChunkCoords(a_ChunkX,a_ChunkZ);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetChunkCoords'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: FillBlocks of class  cChunkDesc */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cChunkDesc_FillBlocks00
static int tolua_AllToLua_cChunkDesc_FillBlocks00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cChunkDesc",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cChunkDesc* self = (cChunkDesc*)  tolua_tousertype(tolua_S,1,0);
  unsigned char a_BlockType = (( unsigned char)  tolua_tonumber(tolua_S,2,0));
  unsigned char a_BlockMeta = (( unsigned char)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'FillBlocks'", NULL);
#endif
  {
   self->FillBlocks(a_BlockType,a_BlockMeta);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'FillBlocks'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetBlockTypeMeta of class  cChunkDesc */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cChunkDesc_SetBlockTypeMeta00
static int tolua_AllToLua_cChunkDesc_SetBlockTypeMeta00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cChunkDesc",0,&tolua_err) ||
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
  cChunkDesc* self = (cChunkDesc*)  tolua_tousertype(tolua_S,1,0);
  int a_RelX = ((int)  tolua_tonumber(tolua_S,2,0));
  int a_RelY = ((int)  tolua_tonumber(tolua_S,3,0));
  int a_RelZ = ((int)  tolua_tonumber(tolua_S,4,0));
  unsigned char a_BlockType = (( unsigned char)  tolua_tonumber(tolua_S,5,0));
  unsigned char a_BlockMeta = (( unsigned char)  tolua_tonumber(tolua_S,6,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetBlockTypeMeta'", NULL);
#endif
  {
   self->SetBlockTypeMeta(a_RelX,a_RelY,a_RelZ,a_BlockType,a_BlockMeta);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetBlockTypeMeta'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetBlockTypeMeta of class  cChunkDesc */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cChunkDesc_GetBlockTypeMeta00
static int tolua_AllToLua_cChunkDesc_GetBlockTypeMeta00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cChunkDesc",0,&tolua_err) ||
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
  cChunkDesc* self = (cChunkDesc*)  tolua_tousertype(tolua_S,1,0);
  int a_RelX = ((int)  tolua_tonumber(tolua_S,2,0));
  int a_RelY = ((int)  tolua_tonumber(tolua_S,3,0));
  int a_RelZ = ((int)  tolua_tonumber(tolua_S,4,0));
  unsigned char a_BlockType = (( unsigned char)  tolua_tonumber(tolua_S,5,0));
  unsigned char a_BlockMeta = (( unsigned char)  tolua_tonumber(tolua_S,6,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetBlockTypeMeta'", NULL);
#endif
  {
   self->GetBlockTypeMeta(a_RelX,a_RelY,a_RelZ,a_BlockType,a_BlockMeta);
   tolua_pushnumber(tolua_S,(lua_Number)a_BlockType);
   tolua_pushnumber(tolua_S,(lua_Number)a_BlockMeta);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetBlockTypeMeta'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetBlockType of class  cChunkDesc */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cChunkDesc_SetBlockType00
static int tolua_AllToLua_cChunkDesc_SetBlockType00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cChunkDesc",0,&tolua_err) ||
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
  cChunkDesc* self = (cChunkDesc*)  tolua_tousertype(tolua_S,1,0);
  int a_RelX = ((int)  tolua_tonumber(tolua_S,2,0));
  int a_RelY = ((int)  tolua_tonumber(tolua_S,3,0));
  int a_RelZ = ((int)  tolua_tonumber(tolua_S,4,0));
  unsigned char a_BlockType = (( unsigned char)  tolua_tonumber(tolua_S,5,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetBlockType'", NULL);
#endif
  {
   self->SetBlockType(a_RelX,a_RelY,a_RelZ,a_BlockType);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetBlockType'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetBlockType of class  cChunkDesc */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cChunkDesc_GetBlockType00
static int tolua_AllToLua_cChunkDesc_GetBlockType00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cChunkDesc",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cChunkDesc* self = (cChunkDesc*)  tolua_tousertype(tolua_S,1,0);
  int a_RelX = ((int)  tolua_tonumber(tolua_S,2,0));
  int a_RelY = ((int)  tolua_tonumber(tolua_S,3,0));
  int a_RelZ = ((int)  tolua_tonumber(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetBlockType'", NULL);
#endif
  {
   unsigned char tolua_ret = ( unsigned char)  self->GetBlockType(a_RelX,a_RelY,a_RelZ);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetBlockType'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetBlockMeta of class  cChunkDesc */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cChunkDesc_SetBlockMeta00
static int tolua_AllToLua_cChunkDesc_SetBlockMeta00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cChunkDesc",0,&tolua_err) ||
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
  cChunkDesc* self = (cChunkDesc*)  tolua_tousertype(tolua_S,1,0);
  int a_RelX = ((int)  tolua_tonumber(tolua_S,2,0));
  int a_RelY = ((int)  tolua_tonumber(tolua_S,3,0));
  int a_RelZ = ((int)  tolua_tonumber(tolua_S,4,0));
  unsigned char a_BlockMeta = (( unsigned char)  tolua_tonumber(tolua_S,5,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetBlockMeta'", NULL);
#endif
  {
   self->SetBlockMeta(a_RelX,a_RelY,a_RelZ,a_BlockMeta);
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

/* method: GetBlockMeta of class  cChunkDesc */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cChunkDesc_GetBlockMeta00
static int tolua_AllToLua_cChunkDesc_GetBlockMeta00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cChunkDesc",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cChunkDesc* self = (cChunkDesc*)  tolua_tousertype(tolua_S,1,0);
  int a_RelX = ((int)  tolua_tonumber(tolua_S,2,0));
  int a_RelY = ((int)  tolua_tonumber(tolua_S,3,0));
  int a_RelZ = ((int)  tolua_tonumber(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetBlockMeta'", NULL);
#endif
  {
   unsigned char tolua_ret = ( unsigned char)  self->GetBlockMeta(a_RelX,a_RelY,a_RelZ);
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

/* method: SetBiome of class  cChunkDesc */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cChunkDesc_SetBiome00
static int tolua_AllToLua_cChunkDesc_SetBiome00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cChunkDesc",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cChunkDesc* self = (cChunkDesc*)  tolua_tousertype(tolua_S,1,0);
  int a_RelX = ((int)  tolua_tonumber(tolua_S,2,0));
  int a_RelZ = ((int)  tolua_tonumber(tolua_S,3,0));
  int a_BiomeID = ((int)  tolua_tonumber(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetBiome'", NULL);
#endif
  {
   self->SetBiome(a_RelX,a_RelZ,a_BiomeID);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetBiome'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetBiome of class  cChunkDesc */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cChunkDesc_GetBiome00
static int tolua_AllToLua_cChunkDesc_GetBiome00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cChunkDesc",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cChunkDesc* self = (cChunkDesc*)  tolua_tousertype(tolua_S,1,0);
  int a_RelX = ((int)  tolua_tonumber(tolua_S,2,0));
  int a_RelZ = ((int)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetBiome'", NULL);
#endif
  {
   EMCSBiome tolua_ret = (EMCSBiome)  self->GetBiome(a_RelX,a_RelZ);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetBiome'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetHeight of class  cChunkDesc */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cChunkDesc_SetHeight00
static int tolua_AllToLua_cChunkDesc_SetHeight00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cChunkDesc",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cChunkDesc* self = (cChunkDesc*)  tolua_tousertype(tolua_S,1,0);
  int a_RelX = ((int)  tolua_tonumber(tolua_S,2,0));
  int a_RelZ = ((int)  tolua_tonumber(tolua_S,3,0));
  int a_Height = ((int)  tolua_tonumber(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetHeight'", NULL);
#endif
  {
   self->SetHeight(a_RelX,a_RelZ,a_Height);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetHeight'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetHeight of class  cChunkDesc */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cChunkDesc_GetHeight00
static int tolua_AllToLua_cChunkDesc_GetHeight00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cChunkDesc",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cChunkDesc* self = (cChunkDesc*)  tolua_tousertype(tolua_S,1,0);
  int a_RelX = ((int)  tolua_tonumber(tolua_S,2,0));
  int a_RelZ = ((int)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetHeight'", NULL);
#endif
  {
   int tolua_ret = (int)  self->GetHeight(a_RelX,a_RelZ);
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

/* method: SetUseDefaultBiomes of class  cChunkDesc */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cChunkDesc_SetUseDefaultBiomes00
static int tolua_AllToLua_cChunkDesc_SetUseDefaultBiomes00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cChunkDesc",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cChunkDesc* self = (cChunkDesc*)  tolua_tousertype(tolua_S,1,0);
  bool a_bUseDefaultBiomes = ((bool)  tolua_toboolean(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetUseDefaultBiomes'", NULL);
#endif
  {
   self->SetUseDefaultBiomes(a_bUseDefaultBiomes);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetUseDefaultBiomes'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: IsUsingDefaultBiomes of class  cChunkDesc */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cChunkDesc_IsUsingDefaultBiomes00
static int tolua_AllToLua_cChunkDesc_IsUsingDefaultBiomes00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cChunkDesc",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cChunkDesc* self = (const cChunkDesc*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'IsUsingDefaultBiomes'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->IsUsingDefaultBiomes();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'IsUsingDefaultBiomes'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetUseDefaultHeight of class  cChunkDesc */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cChunkDesc_SetUseDefaultHeight00
static int tolua_AllToLua_cChunkDesc_SetUseDefaultHeight00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cChunkDesc",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cChunkDesc* self = (cChunkDesc*)  tolua_tousertype(tolua_S,1,0);
  bool a_bUseDefaultHeight = ((bool)  tolua_toboolean(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetUseDefaultHeight'", NULL);
#endif
  {
   self->SetUseDefaultHeight(a_bUseDefaultHeight);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetUseDefaultHeight'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: IsUsingDefaultHeight of class  cChunkDesc */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cChunkDesc_IsUsingDefaultHeight00
static int tolua_AllToLua_cChunkDesc_IsUsingDefaultHeight00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cChunkDesc",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cChunkDesc* self = (const cChunkDesc*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'IsUsingDefaultHeight'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->IsUsingDefaultHeight();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'IsUsingDefaultHeight'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetUseDefaultComposition of class  cChunkDesc */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cChunkDesc_SetUseDefaultComposition00
static int tolua_AllToLua_cChunkDesc_SetUseDefaultComposition00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cChunkDesc",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cChunkDesc* self = (cChunkDesc*)  tolua_tousertype(tolua_S,1,0);
  bool a_bUseDefaultComposition = ((bool)  tolua_toboolean(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetUseDefaultComposition'", NULL);
#endif
  {
   self->SetUseDefaultComposition(a_bUseDefaultComposition);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetUseDefaultComposition'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: IsUsingDefaultComposition of class  cChunkDesc */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cChunkDesc_IsUsingDefaultComposition00
static int tolua_AllToLua_cChunkDesc_IsUsingDefaultComposition00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cChunkDesc",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cChunkDesc* self = (const cChunkDesc*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'IsUsingDefaultComposition'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->IsUsingDefaultComposition();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'IsUsingDefaultComposition'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetUseDefaultStructures of class  cChunkDesc */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cChunkDesc_SetUseDefaultStructures00
static int tolua_AllToLua_cChunkDesc_SetUseDefaultStructures00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cChunkDesc",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cChunkDesc* self = (cChunkDesc*)  tolua_tousertype(tolua_S,1,0);
  bool a_bUseDefaultStructures = ((bool)  tolua_toboolean(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetUseDefaultStructures'", NULL);
#endif
  {
   self->SetUseDefaultStructures(a_bUseDefaultStructures);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetUseDefaultStructures'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: IsUsingDefaultStructures of class  cChunkDesc */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cChunkDesc_IsUsingDefaultStructures00
static int tolua_AllToLua_cChunkDesc_IsUsingDefaultStructures00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cChunkDesc",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cChunkDesc* self = (const cChunkDesc*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'IsUsingDefaultStructures'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->IsUsingDefaultStructures();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'IsUsingDefaultStructures'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetUseDefaultFinish of class  cChunkDesc */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cChunkDesc_SetUseDefaultFinish00
static int tolua_AllToLua_cChunkDesc_SetUseDefaultFinish00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cChunkDesc",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cChunkDesc* self = (cChunkDesc*)  tolua_tousertype(tolua_S,1,0);
  bool a_bUseDefaultFinish = ((bool)  tolua_toboolean(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetUseDefaultFinish'", NULL);
#endif
  {
   self->SetUseDefaultFinish(a_bUseDefaultFinish);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetUseDefaultFinish'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: IsUsingDefaultFinish of class  cChunkDesc */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cChunkDesc_IsUsingDefaultFinish00
static int tolua_AllToLua_cChunkDesc_IsUsingDefaultFinish00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cChunkDesc",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cChunkDesc* self = (const cChunkDesc*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'IsUsingDefaultFinish'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->IsUsingDefaultFinish();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'IsUsingDefaultFinish'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: WriteBlockArea of class  cChunkDesc */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cChunkDesc_WriteBlockArea00
static int tolua_AllToLua_cChunkDesc_WriteBlockArea00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cChunkDesc",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const cBlockArea",0,&tolua_err)) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,6,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,7,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cChunkDesc* self = (cChunkDesc*)  tolua_tousertype(tolua_S,1,0);
  const cBlockArea* a_BlockArea = ((const cBlockArea*)  tolua_tousertype(tolua_S,2,0));
  int a_RelX = ((int)  tolua_tonumber(tolua_S,3,0));
  int a_RelY = ((int)  tolua_tonumber(tolua_S,4,0));
  int a_RelZ = ((int)  tolua_tonumber(tolua_S,5,0));
  cBlockArea::eMergeStrategy a_MergeStrategy = ((cBlockArea::eMergeStrategy) (int)  tolua_tonumber(tolua_S,6,cBlockArea::msOverwrite));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'WriteBlockArea'", NULL);
#endif
  {
   self->WriteBlockArea(*a_BlockArea,a_RelX,a_RelY,a_RelZ,a_MergeStrategy);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'WriteBlockArea'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: ReadBlockArea of class  cChunkDesc */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cChunkDesc_ReadBlockArea00
static int tolua_AllToLua_cChunkDesc_ReadBlockArea00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cChunkDesc",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"cBlockArea",0,&tolua_err)) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,6,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,7,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,8,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,9,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cChunkDesc* self = (cChunkDesc*)  tolua_tousertype(tolua_S,1,0);
  cBlockArea* a_Dest = ((cBlockArea*)  tolua_tousertype(tolua_S,2,0));
  int a_MinRelX = ((int)  tolua_tonumber(tolua_S,3,0));
  int a_MaxRelX = ((int)  tolua_tonumber(tolua_S,4,0));
  int a_MinRelY = ((int)  tolua_tonumber(tolua_S,5,0));
  int a_MaxRelY = ((int)  tolua_tonumber(tolua_S,6,0));
  int a_MinRelZ = ((int)  tolua_tonumber(tolua_S,7,0));
  int a_MaxRelZ = ((int)  tolua_tonumber(tolua_S,8,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'ReadBlockArea'", NULL);
#endif
  {
   self->ReadBlockArea(*a_Dest,a_MinRelX,a_MaxRelX,a_MinRelY,a_MaxRelY,a_MinRelZ,a_MaxRelZ);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'ReadBlockArea'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetMaxHeight of class  cChunkDesc */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cChunkDesc_GetMaxHeight00
static int tolua_AllToLua_cChunkDesc_GetMaxHeight00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cChunkDesc",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cChunkDesc* self = (const cChunkDesc*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetMaxHeight'", NULL);
#endif
  {
   unsigned char tolua_ret = ( unsigned char)  self->GetMaxHeight();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetMaxHeight'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: FillRelCuboid of class  cChunkDesc */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cChunkDesc_FillRelCuboid00
static int tolua_AllToLua_cChunkDesc_FillRelCuboid00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cChunkDesc",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,6,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,7,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,8,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,9,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,10,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cChunkDesc* self = (cChunkDesc*)  tolua_tousertype(tolua_S,1,0);
  int a_MinX = ((int)  tolua_tonumber(tolua_S,2,0));
  int a_MaxX = ((int)  tolua_tonumber(tolua_S,3,0));
  int a_MinY = ((int)  tolua_tonumber(tolua_S,4,0));
  int a_MaxY = ((int)  tolua_tonumber(tolua_S,5,0));
  int a_MinZ = ((int)  tolua_tonumber(tolua_S,6,0));
  int a_MaxZ = ((int)  tolua_tonumber(tolua_S,7,0));
  unsigned char a_BlockType = (( unsigned char)  tolua_tonumber(tolua_S,8,0));
  unsigned char a_BlockMeta = (( unsigned char)  tolua_tonumber(tolua_S,9,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'FillRelCuboid'", NULL);
#endif
  {
   self->FillRelCuboid(a_MinX,a_MaxX,a_MinY,a_MaxY,a_MinZ,a_MaxZ,a_BlockType,a_BlockMeta);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'FillRelCuboid'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: FillRelCuboid of class  cChunkDesc */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cChunkDesc_FillRelCuboid01
static int tolua_AllToLua_cChunkDesc_FillRelCuboid01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cChunkDesc",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const cCuboid",0,&tolua_err)) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  cChunkDesc* self = (cChunkDesc*)  tolua_tousertype(tolua_S,1,0);
  const cCuboid* a_RelCuboid = ((const cCuboid*)  tolua_tousertype(tolua_S,2,0));
  unsigned char a_BlockType = (( unsigned char)  tolua_tonumber(tolua_S,3,0));
  unsigned char a_BlockMeta = (( unsigned char)  tolua_tonumber(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'FillRelCuboid'", NULL);
#endif
  {
   self->FillRelCuboid(*a_RelCuboid,a_BlockType,a_BlockMeta);
  }
 }
 return 0;
tolua_lerror:
 return tolua_AllToLua_cChunkDesc_FillRelCuboid00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: ReplaceRelCuboid of class  cChunkDesc */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cChunkDesc_ReplaceRelCuboid00
static int tolua_AllToLua_cChunkDesc_ReplaceRelCuboid00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cChunkDesc",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,6,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,7,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,8,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,9,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,10,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,11,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,12,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cChunkDesc* self = (cChunkDesc*)  tolua_tousertype(tolua_S,1,0);
  int a_MinX = ((int)  tolua_tonumber(tolua_S,2,0));
  int a_MaxX = ((int)  tolua_tonumber(tolua_S,3,0));
  int a_MinY = ((int)  tolua_tonumber(tolua_S,4,0));
  int a_MaxY = ((int)  tolua_tonumber(tolua_S,5,0));
  int a_MinZ = ((int)  tolua_tonumber(tolua_S,6,0));
  int a_MaxZ = ((int)  tolua_tonumber(tolua_S,7,0));
  unsigned char a_SrcType = (( unsigned char)  tolua_tonumber(tolua_S,8,0));
  unsigned char a_SrcMeta = (( unsigned char)  tolua_tonumber(tolua_S,9,0));
  unsigned char a_DstType = (( unsigned char)  tolua_tonumber(tolua_S,10,0));
  unsigned char a_DstMeta = (( unsigned char)  tolua_tonumber(tolua_S,11,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'ReplaceRelCuboid'", NULL);
#endif
  {
   self->ReplaceRelCuboid(a_MinX,a_MaxX,a_MinY,a_MaxY,a_MinZ,a_MaxZ,a_SrcType,a_SrcMeta,a_DstType,a_DstMeta);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'ReplaceRelCuboid'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: ReplaceRelCuboid of class  cChunkDesc */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cChunkDesc_ReplaceRelCuboid01
static int tolua_AllToLua_cChunkDesc_ReplaceRelCuboid01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cChunkDesc",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const cCuboid",0,&tolua_err)) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,6,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,7,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  cChunkDesc* self = (cChunkDesc*)  tolua_tousertype(tolua_S,1,0);
  const cCuboid* a_RelCuboid = ((const cCuboid*)  tolua_tousertype(tolua_S,2,0));
  unsigned char a_SrcType = (( unsigned char)  tolua_tonumber(tolua_S,3,0));
  unsigned char a_SrcMeta = (( unsigned char)  tolua_tonumber(tolua_S,4,0));
  unsigned char a_DstType = (( unsigned char)  tolua_tonumber(tolua_S,5,0));
  unsigned char a_DstMeta = (( unsigned char)  tolua_tonumber(tolua_S,6,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'ReplaceRelCuboid'", NULL);
#endif
  {
   self->ReplaceRelCuboid(*a_RelCuboid,a_SrcType,a_SrcMeta,a_DstType,a_DstMeta);
  }
 }
 return 0;
tolua_lerror:
 return tolua_AllToLua_cChunkDesc_ReplaceRelCuboid00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: FloorRelCuboid of class  cChunkDesc */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cChunkDesc_FloorRelCuboid00
static int tolua_AllToLua_cChunkDesc_FloorRelCuboid00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cChunkDesc",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,6,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,7,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,8,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,9,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,10,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cChunkDesc* self = (cChunkDesc*)  tolua_tousertype(tolua_S,1,0);
  int a_MinX = ((int)  tolua_tonumber(tolua_S,2,0));
  int a_MaxX = ((int)  tolua_tonumber(tolua_S,3,0));
  int a_MinY = ((int)  tolua_tonumber(tolua_S,4,0));
  int a_MaxY = ((int)  tolua_tonumber(tolua_S,5,0));
  int a_MinZ = ((int)  tolua_tonumber(tolua_S,6,0));
  int a_MaxZ = ((int)  tolua_tonumber(tolua_S,7,0));
  unsigned char a_DstType = (( unsigned char)  tolua_tonumber(tolua_S,8,0));
  unsigned char a_DstMeta = (( unsigned char)  tolua_tonumber(tolua_S,9,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'FloorRelCuboid'", NULL);
#endif
  {
   self->FloorRelCuboid(a_MinX,a_MaxX,a_MinY,a_MaxY,a_MinZ,a_MaxZ,a_DstType,a_DstMeta);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'FloorRelCuboid'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: FloorRelCuboid of class  cChunkDesc */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cChunkDesc_FloorRelCuboid01
static int tolua_AllToLua_cChunkDesc_FloorRelCuboid01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cChunkDesc",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const cCuboid",0,&tolua_err)) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  cChunkDesc* self = (cChunkDesc*)  tolua_tousertype(tolua_S,1,0);
  const cCuboid* a_RelCuboid = ((const cCuboid*)  tolua_tousertype(tolua_S,2,0));
  unsigned char a_DstType = (( unsigned char)  tolua_tonumber(tolua_S,3,0));
  unsigned char a_DstMeta = (( unsigned char)  tolua_tonumber(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'FloorRelCuboid'", NULL);
#endif
  {
   self->FloorRelCuboid(*a_RelCuboid,a_DstType,a_DstMeta);
  }
 }
 return 0;
tolua_lerror:
 return tolua_AllToLua_cChunkDesc_FloorRelCuboid00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: RandomFillRelCuboid of class  cChunkDesc */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cChunkDesc_RandomFillRelCuboid00
static int tolua_AllToLua_cChunkDesc_RandomFillRelCuboid00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cChunkDesc",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,6,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,7,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,8,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,9,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,10,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,11,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,12,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cChunkDesc* self = (cChunkDesc*)  tolua_tousertype(tolua_S,1,0);
  int a_MinX = ((int)  tolua_tonumber(tolua_S,2,0));
  int a_MaxX = ((int)  tolua_tonumber(tolua_S,3,0));
  int a_MinY = ((int)  tolua_tonumber(tolua_S,4,0));
  int a_MaxY = ((int)  tolua_tonumber(tolua_S,5,0));
  int a_MinZ = ((int)  tolua_tonumber(tolua_S,6,0));
  int a_MaxZ = ((int)  tolua_tonumber(tolua_S,7,0));
  unsigned char a_BlockType = (( unsigned char)  tolua_tonumber(tolua_S,8,0));
  unsigned char a_BlockMeta = (( unsigned char)  tolua_tonumber(tolua_S,9,0));
  int a_RandomSeed = ((int)  tolua_tonumber(tolua_S,10,0));
  int a_ChanceOutOf10k = ((int)  tolua_tonumber(tolua_S,11,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'RandomFillRelCuboid'", NULL);
#endif
  {
   self->RandomFillRelCuboid(a_MinX,a_MaxX,a_MinY,a_MaxY,a_MinZ,a_MaxZ,a_BlockType,a_BlockMeta,a_RandomSeed,a_ChanceOutOf10k);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'RandomFillRelCuboid'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: RandomFillRelCuboid of class  cChunkDesc */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cChunkDesc_RandomFillRelCuboid01
static int tolua_AllToLua_cChunkDesc_RandomFillRelCuboid01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cChunkDesc",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const cCuboid",0,&tolua_err)) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,6,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,7,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  cChunkDesc* self = (cChunkDesc*)  tolua_tousertype(tolua_S,1,0);
  const cCuboid* a_RelCuboid = ((const cCuboid*)  tolua_tousertype(tolua_S,2,0));
  unsigned char a_BlockType = (( unsigned char)  tolua_tonumber(tolua_S,3,0));
  unsigned char a_BlockMeta = (( unsigned char)  tolua_tonumber(tolua_S,4,0));
  int a_RandomSeed = ((int)  tolua_tonumber(tolua_S,5,0));
  int a_ChanceOutOf10k = ((int)  tolua_tonumber(tolua_S,6,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'RandomFillRelCuboid'", NULL);
#endif
  {
   self->RandomFillRelCuboid(*a_RelCuboid,a_BlockType,a_BlockMeta,a_RandomSeed,a_ChanceOutOf10k);
  }
 }
 return 0;
tolua_lerror:
 return tolua_AllToLua_cChunkDesc_RandomFillRelCuboid00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  cCraftingGrid */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cCraftingGrid_new00
static int tolua_AllToLua_cCraftingGrid_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"cCraftingGrid",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  int a_Width = ((int)  tolua_tonumber(tolua_S,2,0));
  int a_Height = ((int)  tolua_tonumber(tolua_S,3,0));
  {
   cCraftingGrid* tolua_ret = (cCraftingGrid*)  Mtolua_new((cCraftingGrid)(a_Width,a_Height));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"cCraftingGrid");
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

/* method: new_local of class  cCraftingGrid */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cCraftingGrid_new00_local
static int tolua_AllToLua_cCraftingGrid_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"cCraftingGrid",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  int a_Width = ((int)  tolua_tonumber(tolua_S,2,0));
  int a_Height = ((int)  tolua_tonumber(tolua_S,3,0));
  {
   cCraftingGrid* tolua_ret = (cCraftingGrid*)  Mtolua_new((cCraftingGrid)(a_Width,a_Height));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"cCraftingGrid");
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

/* method: GetWidth of class  cCraftingGrid */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cCraftingGrid_GetWidth00
static int tolua_AllToLua_cCraftingGrid_GetWidth00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cCraftingGrid",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cCraftingGrid* self = (const cCraftingGrid*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetWidth'", NULL);
#endif
  {
   int tolua_ret = (int)  self->GetWidth();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetWidth'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetHeight of class  cCraftingGrid */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cCraftingGrid_GetHeight00
static int tolua_AllToLua_cCraftingGrid_GetHeight00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cCraftingGrid",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cCraftingGrid* self = (const cCraftingGrid*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetHeight'", NULL);
#endif
  {
   int tolua_ret = (int)  self->GetHeight();
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

/* method: GetItem of class  cCraftingGrid */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cCraftingGrid_GetItem00
static int tolua_AllToLua_cCraftingGrid_GetItem00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cCraftingGrid",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cCraftingGrid* self = (const cCraftingGrid*)  tolua_tousertype(tolua_S,1,0);
  int x = ((int)  tolua_tonumber(tolua_S,2,0));
  int y = ((int)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetItem'", NULL);
#endif
  {
   cItem& tolua_ret = (cItem&)  self->GetItem(x,y);
    tolua_pushusertype(tolua_S,(void*)&tolua_ret,"cItem");
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

/* method: SetItem of class  cCraftingGrid */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cCraftingGrid_SetItem00
static int tolua_AllToLua_cCraftingGrid_SetItem00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cCraftingGrid",0,&tolua_err) ||
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
  cCraftingGrid* self = (cCraftingGrid*)  tolua_tousertype(tolua_S,1,0);
  int x = ((int)  tolua_tonumber(tolua_S,2,0));
  int y = ((int)  tolua_tonumber(tolua_S,3,0));
  ENUM_ITEM_ID a_ItemType = ((ENUM_ITEM_ID) (int)  tolua_tonumber(tolua_S,4,0));
  int a_ItemCount = ((int)  tolua_tonumber(tolua_S,5,0));
  short a_ItemHealth = ((short)  tolua_tonumber(tolua_S,6,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetItem'", NULL);
#endif
  {
   self->SetItem(x,y,a_ItemType,a_ItemCount,a_ItemHealth);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetItem'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetItem of class  cCraftingGrid */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cCraftingGrid_SetItem01
static int tolua_AllToLua_cCraftingGrid_SetItem01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cCraftingGrid",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,4,&tolua_err) || !tolua_isusertype(tolua_S,4,"const cItem",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  cCraftingGrid* self = (cCraftingGrid*)  tolua_tousertype(tolua_S,1,0);
  int x = ((int)  tolua_tonumber(tolua_S,2,0));
  int y = ((int)  tolua_tonumber(tolua_S,3,0));
  const cItem* a_Item = ((const cItem*)  tolua_tousertype(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetItem'", NULL);
#endif
  {
   self->SetItem(x,y,*a_Item);
  }
 }
 return 0;
tolua_lerror:
 return tolua_AllToLua_cCraftingGrid_SetItem00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: Clear of class  cCraftingGrid */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cCraftingGrid_Clear00
static int tolua_AllToLua_cCraftingGrid_Clear00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cCraftingGrid",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cCraftingGrid* self = (cCraftingGrid*)  tolua_tousertype(tolua_S,1,0);
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

/* method: ConsumeGrid of class  cCraftingGrid */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cCraftingGrid_ConsumeGrid00
static int tolua_AllToLua_cCraftingGrid_ConsumeGrid00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cCraftingGrid",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const cCraftingGrid",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cCraftingGrid* self = (cCraftingGrid*)  tolua_tousertype(tolua_S,1,0);
  const cCraftingGrid* a_Grid = ((const cCraftingGrid*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'ConsumeGrid'", NULL);
#endif
  {
   self->ConsumeGrid(*a_Grid);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'ConsumeGrid'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: Dump of class  cCraftingGrid */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cCraftingGrid_Dump00
static int tolua_AllToLua_cCraftingGrid_Dump00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cCraftingGrid",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cCraftingGrid* self = (cCraftingGrid*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'Dump'", NULL);
#endif
  {
   self->Dump();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Dump'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: Clear of class  cCraftingRecipe */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cCraftingRecipe_Clear00
static int tolua_AllToLua_cCraftingRecipe_Clear00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cCraftingRecipe",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cCraftingRecipe* self = (cCraftingRecipe*)  tolua_tousertype(tolua_S,1,0);
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

/* method: GetIngredientsWidth of class  cCraftingRecipe */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cCraftingRecipe_GetIngredientsWidth00
static int tolua_AllToLua_cCraftingRecipe_GetIngredientsWidth00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cCraftingRecipe",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cCraftingRecipe* self = (const cCraftingRecipe*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetIngredientsWidth'", NULL);
#endif
  {
   int tolua_ret = (int)  self->GetIngredientsWidth();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetIngredientsWidth'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetIngredientsHeight of class  cCraftingRecipe */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cCraftingRecipe_GetIngredientsHeight00
static int tolua_AllToLua_cCraftingRecipe_GetIngredientsHeight00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cCraftingRecipe",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cCraftingRecipe* self = (const cCraftingRecipe*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetIngredientsHeight'", NULL);
#endif
  {
   int tolua_ret = (int)  self->GetIngredientsHeight();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetIngredientsHeight'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetIngredient of class  cCraftingRecipe */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cCraftingRecipe_GetIngredient00
static int tolua_AllToLua_cCraftingRecipe_GetIngredient00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cCraftingRecipe",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cCraftingRecipe* self = (const cCraftingRecipe*)  tolua_tousertype(tolua_S,1,0);
  int x = ((int)  tolua_tonumber(tolua_S,2,0));
  int y = ((int)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetIngredient'", NULL);
#endif
  {
   cItem& tolua_ret = (cItem&)  self->GetIngredient(x,y);
    tolua_pushusertype(tolua_S,(void*)&tolua_ret,"cItem");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetIngredient'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetResult of class  cCraftingRecipe */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cCraftingRecipe_GetResult00
static int tolua_AllToLua_cCraftingRecipe_GetResult00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cCraftingRecipe",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cCraftingRecipe* self = (const cCraftingRecipe*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetResult'", NULL);
#endif
  {
   const cItem& tolua_ret = (const cItem&)  self->GetResult();
    tolua_pushusertype(tolua_S,(void*)&tolua_ret,"const cItem");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetResult'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetResult of class  cCraftingRecipe */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cCraftingRecipe_SetResult00
static int tolua_AllToLua_cCraftingRecipe_SetResult00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cCraftingRecipe",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cCraftingRecipe* self = (cCraftingRecipe*)  tolua_tousertype(tolua_S,1,0);
  ENUM_ITEM_ID a_ItemType = ((ENUM_ITEM_ID) (int)  tolua_tonumber(tolua_S,2,0));
  int a_ItemCount = ((int)  tolua_tonumber(tolua_S,3,0));
  short a_ItemHealth = ((short)  tolua_tonumber(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetResult'", NULL);
#endif
  {
   self->SetResult(a_ItemType,a_ItemCount,a_ItemHealth);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetResult'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetResult of class  cCraftingRecipe */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cCraftingRecipe_SetResult01
static int tolua_AllToLua_cCraftingRecipe_SetResult01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cCraftingRecipe",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const cItem",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  cCraftingRecipe* self = (cCraftingRecipe*)  tolua_tousertype(tolua_S,1,0);
  const cItem* a_Item = ((const cItem*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetResult'", NULL);
#endif
  {
   self->SetResult(*a_Item);
  }
 }
 return 0;
tolua_lerror:
 return tolua_AllToLua_cCraftingRecipe_SetResult00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetIngredient of class  cCraftingRecipe */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cCraftingRecipe_SetIngredient00
static int tolua_AllToLua_cCraftingRecipe_SetIngredient00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cCraftingRecipe",0,&tolua_err) ||
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
  cCraftingRecipe* self = (cCraftingRecipe*)  tolua_tousertype(tolua_S,1,0);
  int x = ((int)  tolua_tonumber(tolua_S,2,0));
  int y = ((int)  tolua_tonumber(tolua_S,3,0));
  ENUM_ITEM_ID a_ItemType = ((ENUM_ITEM_ID) (int)  tolua_tonumber(tolua_S,4,0));
  int a_ItemCount = ((int)  tolua_tonumber(tolua_S,5,0));
  short a_ItemHealth = ((short)  tolua_tonumber(tolua_S,6,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetIngredient'", NULL);
#endif
  {
   self->SetIngredient(x,y,a_ItemType,a_ItemCount,a_ItemHealth);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetIngredient'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetIngredient of class  cCraftingRecipe */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cCraftingRecipe_SetIngredient01
static int tolua_AllToLua_cCraftingRecipe_SetIngredient01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cCraftingRecipe",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,4,&tolua_err) || !tolua_isusertype(tolua_S,4,"const cItem",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  cCraftingRecipe* self = (cCraftingRecipe*)  tolua_tousertype(tolua_S,1,0);
  int x = ((int)  tolua_tonumber(tolua_S,2,0));
  int y = ((int)  tolua_tonumber(tolua_S,3,0));
  const cItem* a_Item = ((const cItem*)  tolua_tousertype(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetIngredient'", NULL);
#endif
  {
   self->SetIngredient(x,y,*a_Item);
  }
 }
 return 0;
tolua_lerror:
 return tolua_AllToLua_cCraftingRecipe_SetIngredient00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: ConsumeIngredients of class  cCraftingRecipe */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cCraftingRecipe_ConsumeIngredients00
static int tolua_AllToLua_cCraftingRecipe_ConsumeIngredients00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cCraftingRecipe",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"cCraftingGrid",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cCraftingRecipe* self = (cCraftingRecipe*)  tolua_tousertype(tolua_S,1,0);
  cCraftingGrid* a_CraftingGrid = ((cCraftingGrid*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'ConsumeIngredients'", NULL);
#endif
  {
   self->ConsumeIngredients(*a_CraftingGrid);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'ConsumeIngredients'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: Dump of class  cCraftingRecipe */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cCraftingRecipe_Dump00
static int tolua_AllToLua_cCraftingRecipe_Dump00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cCraftingRecipe",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cCraftingRecipe* self = (cCraftingRecipe*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'Dump'", NULL);
#endif
  {
   self->Dump();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Dump'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetWindowID of class  cWindow */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cWindow_GetWindowID00
static int tolua_AllToLua_cWindow_GetWindowID00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cWindow",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cWindow* self = (const cWindow*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetWindowID'", NULL);
#endif
  {
   char tolua_ret = (char)  self->GetWindowID();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetWindowID'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetWindowType of class  cWindow */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cWindow_GetWindowType00
static int tolua_AllToLua_cWindow_GetWindowType00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cWindow",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cWindow* self = (const cWindow*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetWindowType'", NULL);
#endif
  {
   int tolua_ret = (int)  self->GetWindowType();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetWindowType'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetSlot of class  cWindow */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cWindow_GetSlot00
static int tolua_AllToLua_cWindow_GetSlot00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cWindow",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"cPlayer",0,&tolua_err)) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cWindow* self = (const cWindow*)  tolua_tousertype(tolua_S,1,0);
  cPlayer* a_Player = ((cPlayer*)  tolua_tousertype(tolua_S,2,0));
  int a_SlotNum = ((int)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetSlot'", NULL);
#endif
  {
   const cItem* tolua_ret = (const cItem*)  self->GetSlot(*a_Player,a_SlotNum);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"const cItem");
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

/* method: SetSlot of class  cWindow */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cWindow_SetSlot00
static int tolua_AllToLua_cWindow_SetSlot00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cWindow",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"cPlayer",0,&tolua_err)) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,4,&tolua_err) || !tolua_isusertype(tolua_S,4,"const cItem",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cWindow* self = (cWindow*)  tolua_tousertype(tolua_S,1,0);
  cPlayer* a_Player = ((cPlayer*)  tolua_tousertype(tolua_S,2,0));
  int a_SlotNum = ((int)  tolua_tonumber(tolua_S,3,0));
  const cItem* a_Item = ((const cItem*)  tolua_tousertype(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetSlot'", NULL);
#endif
  {
   self->SetSlot(*a_Player,a_SlotNum,*a_Item);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetSlot'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: IsSlotInPlayerMainInventory of class  cWindow */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cWindow_IsSlotInPlayerMainInventory00
static int tolua_AllToLua_cWindow_IsSlotInPlayerMainInventory00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cWindow",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cWindow* self = (const cWindow*)  tolua_tousertype(tolua_S,1,0);
  int a_SlotNum = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'IsSlotInPlayerMainInventory'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->IsSlotInPlayerMainInventory(a_SlotNum);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'IsSlotInPlayerMainInventory'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: IsSlotInPlayerHotbar of class  cWindow */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cWindow_IsSlotInPlayerHotbar00
static int tolua_AllToLua_cWindow_IsSlotInPlayerHotbar00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cWindow",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cWindow* self = (const cWindow*)  tolua_tousertype(tolua_S,1,0);
  int a_SlotNum = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'IsSlotInPlayerHotbar'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->IsSlotInPlayerHotbar(a_SlotNum);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'IsSlotInPlayerHotbar'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: IsSlotInPlayerInventory of class  cWindow */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cWindow_IsSlotInPlayerInventory00
static int tolua_AllToLua_cWindow_IsSlotInPlayerInventory00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cWindow",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cWindow* self = (const cWindow*)  tolua_tousertype(tolua_S,1,0);
  int a_SlotNum = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'IsSlotInPlayerInventory'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->IsSlotInPlayerInventory(a_SlotNum);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'IsSlotInPlayerInventory'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetWindowTitle of class  cWindow */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cWindow_GetWindowTitle00
static int tolua_AllToLua_cWindow_GetWindowTitle00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const cWindow",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const cWindow* self = (const cWindow*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetWindowTitle'", NULL);
#endif
  {
   const AString tolua_ret = (const AString)  self->GetWindowTitle();
   tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetWindowTitle'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetWindowTitle of class  cWindow */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cWindow_SetWindowTitle00
static int tolua_AllToLua_cWindow_SetWindowTitle00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cWindow",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cWindow* self = (cWindow*)  tolua_tousertype(tolua_S,1,0);
  const AString a_WindowTitle = ((const AString)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetWindowTitle'", NULL);
#endif
  {
   self->SetWindowTitle(a_WindowTitle);
   tolua_pushcppstring(tolua_S,(const char*)a_WindowTitle);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetWindowTitle'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  cLuaWindow */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cLuaWindow_new00
static int tolua_AllToLua_cLuaWindow_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"cLuaWindow",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,5,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cWindow::WindowType a_WindowType = ((cWindow::WindowType) (int)  tolua_tonumber(tolua_S,2,0));
  int a_SlotsX = ((int)  tolua_tonumber(tolua_S,3,0));
  int a_SlotsY = ((int)  tolua_tonumber(tolua_S,4,0));
  const AString a_Title = ((const AString)  tolua_tocppstring(tolua_S,5,0));
  {
   cLuaWindow* tolua_ret = (cLuaWindow*)  Mtolua_new((cLuaWindow)(a_WindowType,a_SlotsX,a_SlotsY,a_Title));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"cLuaWindow");
   tolua_pushcppstring(tolua_S,(const char*)a_Title);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  cLuaWindow */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cLuaWindow_new00_local
static int tolua_AllToLua_cLuaWindow_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"cLuaWindow",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,5,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cWindow::WindowType a_WindowType = ((cWindow::WindowType) (int)  tolua_tonumber(tolua_S,2,0));
  int a_SlotsX = ((int)  tolua_tonumber(tolua_S,3,0));
  int a_SlotsY = ((int)  tolua_tonumber(tolua_S,4,0));
  const AString a_Title = ((const AString)  tolua_tocppstring(tolua_S,5,0));
  {
   cLuaWindow* tolua_ret = (cLuaWindow*)  Mtolua_new((cLuaWindow)(a_WindowType,a_SlotsX,a_SlotsY,a_Title));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"cLuaWindow");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
   tolua_pushcppstring(tolua_S,(const char*)a_Title);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  cLuaWindow */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cLuaWindow_delete00
static int tolua_AllToLua_cLuaWindow_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cLuaWindow",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cLuaWindow* self = (cLuaWindow*)  tolua_tousertype(tolua_S,1,0);
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

/* method: GetContents of class  cLuaWindow */
#ifndef TOLUA_DISABLE_tolua_AllToLua_cLuaWindow_GetContents00
static int tolua_AllToLua_cLuaWindow_GetContents00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"cLuaWindow",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  cLuaWindow* self = (cLuaWindow*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetContents'", NULL);
#endif
  {
   cItemGrid& tolua_ret = (cItemGrid&)  self->GetContents();
    tolua_pushusertype(tolua_S,(void*)&tolua_ret,"cItemGrid");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetContents'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* get function: __cItemGrid of class  cLuaWindow */
#ifndef TOLUA_DISABLE_tolua_get_cLuaWindow___cItemGrid__cListener__
static int tolua_get_cLuaWindow___cItemGrid__cListener__(lua_State* tolua_S)
{
  cLuaWindow* self = (cLuaWindow*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable '__cItemGrid'",NULL);
#endif
#ifdef __cplusplus
   tolua_pushusertype(tolua_S,(void*)static_cast<cItemGrid::cListener*>(self), "cItemGrid::cListener");
#else
   tolua_pushusertype(tolua_S,(void*)((cItemGrid::cListener*)self), "cItemGrid::cListener");
#endif
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* Open function */
TOLUA_API int tolua_AllToLua_open (lua_State* tolua_S)
{
 tolua_open(tolua_S);
 tolua_reg_types(tolua_S);
 tolua_module(tolua_S,NULL,1);
 tolua_beginmodule(tolua_S,NULL);
  tolua_constant(tolua_S,"biOcean",biOcean);
  tolua_constant(tolua_S,"biPlains",biPlains);
  tolua_constant(tolua_S,"biDesert",biDesert);
  tolua_constant(tolua_S,"biExtremeHills",biExtremeHills);
  tolua_constant(tolua_S,"biForest",biForest);
  tolua_constant(tolua_S,"biTaiga",biTaiga);
  tolua_constant(tolua_S,"biSwampland",biSwampland);
  tolua_constant(tolua_S,"biRiver",biRiver);
  tolua_constant(tolua_S,"biHell",biHell);
  tolua_constant(tolua_S,"biNether",biNether);
  tolua_constant(tolua_S,"biSky",biSky);
  tolua_constant(tolua_S,"biFrozenOcean",biFrozenOcean);
  tolua_constant(tolua_S,"biFrozenRiver",biFrozenRiver);
  tolua_constant(tolua_S,"biIcePlains",biIcePlains);
  tolua_constant(tolua_S,"biTundra",biTundra);
  tolua_constant(tolua_S,"biIceMountains",biIceMountains);
  tolua_constant(tolua_S,"biMushroomIsland",biMushroomIsland);
  tolua_constant(tolua_S,"biMushroomShore",biMushroomShore);
  tolua_constant(tolua_S,"biBeach",biBeach);
  tolua_constant(tolua_S,"biDesertHills",biDesertHills);
  tolua_constant(tolua_S,"biForestHills",biForestHills);
  tolua_constant(tolua_S,"biTaigaHills",biTaigaHills);
  tolua_constant(tolua_S,"biExtremeHillsEdge",biExtremeHillsEdge);
  tolua_constant(tolua_S,"biJungle",biJungle);
  tolua_constant(tolua_S,"biJungleHills",biJungleHills);
  tolua_constant(tolua_S,"biNumBiomes",biNumBiomes);
  tolua_constant(tolua_S,"biMaxBiome",biMaxBiome);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"cIniFile","cIniFile","",tolua_collect_cIniFile);
  #else
  tolua_cclass(tolua_S,"cIniFile","cIniFile","",NULL);
  #endif
  tolua_beginmodule(tolua_S,"cIniFile");
   tolua_constant(tolua_S,"noID",cIniFile::noID);
   tolua_function(tolua_S,"new",tolua_AllToLua_cIniFile_new00);
   tolua_function(tolua_S,"new_local",tolua_AllToLua_cIniFile_new00_local);
   tolua_function(tolua_S,".call",tolua_AllToLua_cIniFile_new00_local);
   tolua_function(tolua_S,"new",tolua_AllToLua_cIniFile_new01);
   tolua_function(tolua_S,"new_local",tolua_AllToLua_cIniFile_new01_local);
   tolua_function(tolua_S,".call",tolua_AllToLua_cIniFile_new01_local);
   tolua_function(tolua_S,"CaseSensitive",tolua_AllToLua_cIniFile_CaseSensitive00);
   tolua_function(tolua_S,"CaseInsensitive",tolua_AllToLua_cIniFile_CaseInsensitive00);
   tolua_function(tolua_S,"Path",tolua_AllToLua_cIniFile_Path00);
   tolua_function(tolua_S,"Path",tolua_AllToLua_cIniFile_Path01);
   tolua_function(tolua_S,"SetPath",tolua_AllToLua_cIniFile_SetPath00);
   tolua_function(tolua_S,"ReadFile",tolua_AllToLua_cIniFile_ReadFile00);
   tolua_function(tolua_S,"WriteFile",tolua_AllToLua_cIniFile_WriteFile00);
   tolua_function(tolua_S,"Clear",tolua_AllToLua_cIniFile_Clear00);
   tolua_function(tolua_S,"Reset",tolua_AllToLua_cIniFile_Reset00);
   tolua_function(tolua_S,"Erase",tolua_AllToLua_cIniFile_Erase00);
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
   tolua_function(tolua_S,"GetValueF",tolua_AllToLua_cIniFile_GetValueF00);
   tolua_function(tolua_S,"GetValueI",tolua_AllToLua_cIniFile_GetValueI00);
   tolua_function(tolua_S,"GetValueB",tolua_AllToLua_cIniFile_GetValueB00);
   tolua_function(tolua_S,"GetValueSet",tolua_AllToLua_cIniFile_GetValueSet00);
   tolua_function(tolua_S,"GetValueSet",tolua_AllToLua_cIniFile_GetValueSet01);
   tolua_function(tolua_S,"GetValueSetF",tolua_AllToLua_cIniFile_GetValueSetF00);
   tolua_function(tolua_S,"GetValueSetI",tolua_AllToLua_cIniFile_GetValueSetI00);
   tolua_function(tolua_S,"GetValueSetB",tolua_AllToLua_cIniFile_GetValueSetB00);
   tolua_function(tolua_S,"SetValue",tolua_AllToLua_cIniFile_SetValue00);
   tolua_function(tolua_S,"SetValue",tolua_AllToLua_cIniFile_SetValue01);
   tolua_function(tolua_S,"SetValueI",tolua_AllToLua_cIniFile_SetValueI00);
   tolua_function(tolua_S,"SetValueB",tolua_AllToLua_cIniFile_SetValueB00);
   tolua_function(tolua_S,"SetValueF",tolua_AllToLua_cIniFile_SetValueF00);
   tolua_function(tolua_S,"DeleteValueByID",tolua_AllToLua_cIniFile_DeleteValueByID00);
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
  tolua_constant(tolua_S,"E_BLOCK_PLANKS",E_BLOCK_PLANKS);
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
  tolua_constant(tolua_S,"E_BLOCK_LAPIS_ORE",E_BLOCK_LAPIS_ORE);
  tolua_constant(tolua_S,"E_BLOCK_LAPIS_BLOCK",E_BLOCK_LAPIS_BLOCK);
  tolua_constant(tolua_S,"E_BLOCK_DISPENSER",E_BLOCK_DISPENSER);
  tolua_constant(tolua_S,"E_BLOCK_SANDSTONE",E_BLOCK_SANDSTONE);
  tolua_constant(tolua_S,"E_BLOCK_NOTE_BLOCK",E_BLOCK_NOTE_BLOCK);
  tolua_constant(tolua_S,"E_BLOCK_BED",E_BLOCK_BED);
  tolua_constant(tolua_S,"E_BLOCK_POWERED_RAIL",E_BLOCK_POWERED_RAIL);
  tolua_constant(tolua_S,"E_BLOCK_DETECTOR_RAIL",E_BLOCK_DETECTOR_RAIL);
  tolua_constant(tolua_S,"E_BLOCK_STICKY_PISTON",E_BLOCK_STICKY_PISTON);
  tolua_constant(tolua_S,"E_BLOCK_COBWEB",E_BLOCK_COBWEB);
  tolua_constant(tolua_S,"E_BLOCK_TALL_GRASS",E_BLOCK_TALL_GRASS);
  tolua_constant(tolua_S,"E_BLOCK_DEAD_BUSH",E_BLOCK_DEAD_BUSH);
  tolua_constant(tolua_S,"E_BLOCK_PISTON",E_BLOCK_PISTON);
  tolua_constant(tolua_S,"E_BLOCK_PISTON_EXTENSION",E_BLOCK_PISTON_EXTENSION);
  tolua_constant(tolua_S,"E_BLOCK_WOOL",E_BLOCK_WOOL);
  tolua_constant(tolua_S,"E_BLOCK_PISTON_MOVED_BLOCK",E_BLOCK_PISTON_MOVED_BLOCK);
  tolua_constant(tolua_S,"E_BLOCK_YELLOW_FLOWER",E_BLOCK_YELLOW_FLOWER);
  tolua_constant(tolua_S,"E_BLOCK_RED_ROSE",E_BLOCK_RED_ROSE);
  tolua_constant(tolua_S,"E_BLOCK_BROWN_MUSHROOM",E_BLOCK_BROWN_MUSHROOM);
  tolua_constant(tolua_S,"E_BLOCK_RED_MUSHROOM",E_BLOCK_RED_MUSHROOM);
  tolua_constant(tolua_S,"E_BLOCK_GOLD_BLOCK",E_BLOCK_GOLD_BLOCK);
  tolua_constant(tolua_S,"E_BLOCK_IRON_BLOCK",E_BLOCK_IRON_BLOCK);
  tolua_constant(tolua_S,"E_BLOCK_DOUBLE_STONE_SLAB",E_BLOCK_DOUBLE_STONE_SLAB);
  tolua_constant(tolua_S,"E_BLOCK_STONE_SLAB",E_BLOCK_STONE_SLAB);
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
  tolua_constant(tolua_S,"E_BLOCK_CRAFTING_TABLE",E_BLOCK_CRAFTING_TABLE);
  tolua_constant(tolua_S,"E_BLOCK_WORKBENCH",E_BLOCK_WORKBENCH);
  tolua_constant(tolua_S,"E_BLOCK_CROPS",E_BLOCK_CROPS);
  tolua_constant(tolua_S,"E_BLOCK_FARMLAND",E_BLOCK_FARMLAND);
  tolua_constant(tolua_S,"E_BLOCK_FURNACE",E_BLOCK_FURNACE);
  tolua_constant(tolua_S,"E_BLOCK_LIT_FURNACE",E_BLOCK_LIT_FURNACE);
  tolua_constant(tolua_S,"E_BLOCK_BURNING_FURNACE",E_BLOCK_BURNING_FURNACE);
  tolua_constant(tolua_S,"E_BLOCK_SIGN_POST",E_BLOCK_SIGN_POST);
  tolua_constant(tolua_S,"E_BLOCK_WOODEN_DOOR",E_BLOCK_WOODEN_DOOR);
  tolua_constant(tolua_S,"E_BLOCK_LADDER",E_BLOCK_LADDER);
  tolua_constant(tolua_S,"E_BLOCK_RAIL",E_BLOCK_RAIL);
  tolua_constant(tolua_S,"E_BLOCK_MINECART_TRACKS",E_BLOCK_MINECART_TRACKS);
  tolua_constant(tolua_S,"E_BLOCK_COBBLESTONE_STAIRS",E_BLOCK_COBBLESTONE_STAIRS);
  tolua_constant(tolua_S,"E_BLOCK_WALLSIGN",E_BLOCK_WALLSIGN);
  tolua_constant(tolua_S,"E_BLOCK_LEVER",E_BLOCK_LEVER);
  tolua_constant(tolua_S,"E_BLOCK_STONE_PRESSURE_PLATE",E_BLOCK_STONE_PRESSURE_PLATE);
  tolua_constant(tolua_S,"E_BLOCK_IRON_DOOR",E_BLOCK_IRON_DOOR);
  tolua_constant(tolua_S,"E_BLOCK_WOODEN_PRESSURE_PLATE",E_BLOCK_WOODEN_PRESSURE_PLATE);
  tolua_constant(tolua_S,"E_BLOCK_REDSTONE_ORE",E_BLOCK_REDSTONE_ORE);
  tolua_constant(tolua_S,"E_BLOCK_REDSTONE_ORE_GLOWING",E_BLOCK_REDSTONE_ORE_GLOWING);
  tolua_constant(tolua_S,"E_BLOCK_REDSTONE_TORCH_OFF",E_BLOCK_REDSTONE_TORCH_OFF);
  tolua_constant(tolua_S,"E_BLOCK_REDSTONE_TORCH_ON",E_BLOCK_REDSTONE_TORCH_ON);
  tolua_constant(tolua_S,"E_BLOCK_STONE_BUTTON",E_BLOCK_STONE_BUTTON);
  tolua_constant(tolua_S,"E_BLOCK_SNOW",E_BLOCK_SNOW);
  tolua_constant(tolua_S,"E_BLOCK_ICE",E_BLOCK_ICE);
  tolua_constant(tolua_S,"E_BLOCK_SNOW_BLOCK",E_BLOCK_SNOW_BLOCK);
  tolua_constant(tolua_S,"E_BLOCK_CACTUS",E_BLOCK_CACTUS);
  tolua_constant(tolua_S,"E_BLOCK_CLAY",E_BLOCK_CLAY);
  tolua_constant(tolua_S,"E_BLOCK_SUGARCANE",E_BLOCK_SUGARCANE);
  tolua_constant(tolua_S,"E_BLOCK_REEDS",E_BLOCK_REEDS);
  tolua_constant(tolua_S,"E_BLOCK_JUKEBOX",E_BLOCK_JUKEBOX);
  tolua_constant(tolua_S,"E_BLOCK_FENCE",E_BLOCK_FENCE);
  tolua_constant(tolua_S,"E_BLOCK_PUMPKIN",E_BLOCK_PUMPKIN);
  tolua_constant(tolua_S,"E_BLOCK_NETHERRACK",E_BLOCK_NETHERRACK);
  tolua_constant(tolua_S,"E_BLOCK_SOULSAND",E_BLOCK_SOULSAND);
  tolua_constant(tolua_S,"E_BLOCK_GLOWSTONE",E_BLOCK_GLOWSTONE);
  tolua_constant(tolua_S,"E_BLOCK_NETHER_PORTAL",E_BLOCK_NETHER_PORTAL);
  tolua_constant(tolua_S,"E_BLOCK_JACK_O_LANTERN",E_BLOCK_JACK_O_LANTERN);
  tolua_constant(tolua_S,"E_BLOCK_CAKE",E_BLOCK_CAKE);
  tolua_constant(tolua_S,"E_BLOCK_REDSTONE_REPEATER_OFF",E_BLOCK_REDSTONE_REPEATER_OFF);
  tolua_constant(tolua_S,"E_BLOCK_REDSTONE_REPEATER_ON",E_BLOCK_REDSTONE_REPEATER_ON);
  tolua_constant(tolua_S,"E_BLOCK_LOCKED_CHEST",E_BLOCK_LOCKED_CHEST);
  tolua_constant(tolua_S,"E_BLOCK_TRAPDOOR",E_BLOCK_TRAPDOOR);
  tolua_constant(tolua_S,"E_BLOCK_SILVERFISH_EGG",E_BLOCK_SILVERFISH_EGG);
  tolua_constant(tolua_S,"E_BLOCK_STONE_BRICKS",E_BLOCK_STONE_BRICKS);
  tolua_constant(tolua_S,"E_BLOCK_HUGE_BROWN_MUSHROOM",E_BLOCK_HUGE_BROWN_MUSHROOM);
  tolua_constant(tolua_S,"E_BLOCK_HUGE_RED_MUSHROOM",E_BLOCK_HUGE_RED_MUSHROOM);
  tolua_constant(tolua_S,"E_BLOCK_IRON_BARS",E_BLOCK_IRON_BARS);
  tolua_constant(tolua_S,"E_BLOCK_GLASS_PANE",E_BLOCK_GLASS_PANE);
  tolua_constant(tolua_S,"E_BLOCK_MELON",E_BLOCK_MELON);
  tolua_constant(tolua_S,"E_BLOCK_PUMPKIN_STEM",E_BLOCK_PUMPKIN_STEM);
  tolua_constant(tolua_S,"E_BLOCK_MELON_STEM",E_BLOCK_MELON_STEM);
  tolua_constant(tolua_S,"E_BLOCK_VINES",E_BLOCK_VINES);
  tolua_constant(tolua_S,"E_BLOCK_FENCE_GATE",E_BLOCK_FENCE_GATE);
  tolua_constant(tolua_S,"E_BLOCK_BRICK_STAIRS",E_BLOCK_BRICK_STAIRS);
  tolua_constant(tolua_S,"E_BLOCK_STONE_BRICK_STAIRS",E_BLOCK_STONE_BRICK_STAIRS);
  tolua_constant(tolua_S,"E_BLOCK_MYCELIUM",E_BLOCK_MYCELIUM);
  tolua_constant(tolua_S,"E_BLOCK_LILY_PAD",E_BLOCK_LILY_PAD);
  tolua_constant(tolua_S,"E_BLOCK_NETHER_BRICK",E_BLOCK_NETHER_BRICK);
  tolua_constant(tolua_S,"E_BLOCK_NETHER_BRICK_FENCE",E_BLOCK_NETHER_BRICK_FENCE);
  tolua_constant(tolua_S,"E_BLOCK_NETHER_BRICK_STAIRS",E_BLOCK_NETHER_BRICK_STAIRS);
  tolua_constant(tolua_S,"E_BLOCK_NETHER_WART",E_BLOCK_NETHER_WART);
  tolua_constant(tolua_S,"E_BLOCK_ENCHANTMENT_TABLE",E_BLOCK_ENCHANTMENT_TABLE);
  tolua_constant(tolua_S,"E_BLOCK_BREWING_STAND",E_BLOCK_BREWING_STAND);
  tolua_constant(tolua_S,"E_BLOCK_CAULDRON",E_BLOCK_CAULDRON);
  tolua_constant(tolua_S,"E_BLOCK_END_PORTAL",E_BLOCK_END_PORTAL);
  tolua_constant(tolua_S,"E_BLOCK_END_PORTAL_FRAME",E_BLOCK_END_PORTAL_FRAME);
  tolua_constant(tolua_S,"E_BLOCK_END_STONE",E_BLOCK_END_STONE);
  tolua_constant(tolua_S,"E_BLOCK_DRAGON_EGG",E_BLOCK_DRAGON_EGG);
  tolua_constant(tolua_S,"E_BLOCK_REDSTONE_LAMP_OFF",E_BLOCK_REDSTONE_LAMP_OFF);
  tolua_constant(tolua_S,"E_BLOCK_REDSTONE_LAMP_ON",E_BLOCK_REDSTONE_LAMP_ON);
  tolua_constant(tolua_S,"E_BLOCK_DOUBLE_WOODEN_SLAB",E_BLOCK_DOUBLE_WOODEN_SLAB);
  tolua_constant(tolua_S,"E_BLOCK_WOODEN_SLAB",E_BLOCK_WOODEN_SLAB);
  tolua_constant(tolua_S,"E_BLOCK_COCOA_POD",E_BLOCK_COCOA_POD);
  tolua_constant(tolua_S,"E_BLOCK_SANDSTONE_STAIRS",E_BLOCK_SANDSTONE_STAIRS);
  tolua_constant(tolua_S,"E_BLOCK_EMERALD_ORE",E_BLOCK_EMERALD_ORE);
  tolua_constant(tolua_S,"E_BLOCK_ENDER_CHEST",E_BLOCK_ENDER_CHEST);
  tolua_constant(tolua_S,"E_BLOCK_TRIPWIRE_HOOK",E_BLOCK_TRIPWIRE_HOOK);
  tolua_constant(tolua_S,"E_BLOCK_TRIPWIRE",E_BLOCK_TRIPWIRE);
  tolua_constant(tolua_S,"E_BLOCK_EMERALD_BLOCK",E_BLOCK_EMERALD_BLOCK);
  tolua_constant(tolua_S,"E_BLOCK_SPRUCE_WOOD_STAIRS",E_BLOCK_SPRUCE_WOOD_STAIRS);
  tolua_constant(tolua_S,"E_BLOCK_BIRCH_WOOD_STAIRS",E_BLOCK_BIRCH_WOOD_STAIRS);
  tolua_constant(tolua_S,"E_BLOCK_JUNGLE_WOOD_STAIRS",E_BLOCK_JUNGLE_WOOD_STAIRS);
  tolua_constant(tolua_S,"E_BLOCK_COMMAND_BLOCK",E_BLOCK_COMMAND_BLOCK);
  tolua_constant(tolua_S,"E_BLOCK_BEACON",E_BLOCK_BEACON);
  tolua_constant(tolua_S,"E_BLOCK_COBBLESTONE_WALL",E_BLOCK_COBBLESTONE_WALL);
  tolua_constant(tolua_S,"E_BLOCK_FLOWER_POT",E_BLOCK_FLOWER_POT);
  tolua_constant(tolua_S,"E_BLOCK_CARROTS",E_BLOCK_CARROTS);
  tolua_constant(tolua_S,"E_BLOCK_POTATOES",E_BLOCK_POTATOES);
  tolua_constant(tolua_S,"E_BLOCK_WOODEN_BUTTON",E_BLOCK_WOODEN_BUTTON);
  tolua_constant(tolua_S,"E_BLOCK_HEAD",E_BLOCK_HEAD);
  tolua_constant(tolua_S,"E_BLOCK_ANVIL",E_BLOCK_ANVIL);
  tolua_constant(tolua_S,"E_BLOCK_TRAPPED_CHEST",E_BLOCK_TRAPPED_CHEST);
  tolua_constant(tolua_S,"E_BLOCK_LIGHT_WEIGHTED_PRESSURE_PLATE",E_BLOCK_LIGHT_WEIGHTED_PRESSURE_PLATE);
  tolua_constant(tolua_S,"E_BLOCK_HEAVY_WEIGHTED_PRESSURE_PLATE",E_BLOCK_HEAVY_WEIGHTED_PRESSURE_PLATE);
  tolua_constant(tolua_S,"E_BLOCK_INACTIVE_COMPARATOR",E_BLOCK_INACTIVE_COMPARATOR);
  tolua_constant(tolua_S,"E_BLOCK_ACTIVE_COMPARATOR",E_BLOCK_ACTIVE_COMPARATOR);
  tolua_constant(tolua_S,"E_BLOCK_DAYLIGHT_SENSOR",E_BLOCK_DAYLIGHT_SENSOR);
  tolua_constant(tolua_S,"E_BLOCK_BLOCK_OF_REDSTONE",E_BLOCK_BLOCK_OF_REDSTONE);
  tolua_constant(tolua_S,"E_BLOCK_NETHER_QUARTZ_ORE",E_BLOCK_NETHER_QUARTZ_ORE);
  tolua_constant(tolua_S,"E_BLOCK_HOPPER",E_BLOCK_HOPPER);
  tolua_constant(tolua_S,"E_BLOCK_QUARTZ_BLOCK",E_BLOCK_QUARTZ_BLOCK);
  tolua_constant(tolua_S,"E_BLOCK_ACTIVATOR_RAIL",E_BLOCK_ACTIVATOR_RAIL);
  tolua_constant(tolua_S,"E_BLOCK_DROPPER",E_BLOCK_DROPPER);
  tolua_constant(tolua_S,"E_BLOCK_CARPET",E_BLOCK_CARPET);
  tolua_constant(tolua_S,"E_BLOCK_NUMBER_OF_TYPES",E_BLOCK_NUMBER_OF_TYPES);
  tolua_constant(tolua_S,"E_BLOCK_MAX_TYPE_ID",E_BLOCK_MAX_TYPE_ID);
  tolua_constant(tolua_S,"E_ITEM_EMPTY",E_ITEM_EMPTY);
  tolua_constant(tolua_S,"E_ITEM_FIRST",E_ITEM_FIRST);
  tolua_constant(tolua_S,"E_ITEM_IRON_SHOVEL",E_ITEM_IRON_SHOVEL);
  tolua_constant(tolua_S,"E_ITEM_IRON_PICKAXE",E_ITEM_IRON_PICKAXE);
  tolua_constant(tolua_S,"E_ITEM_IRON_AXE",E_ITEM_IRON_AXE);
  tolua_constant(tolua_S,"E_ITEM_FLINT_AND_STEEL",E_ITEM_FLINT_AND_STEEL);
  tolua_constant(tolua_S,"E_ITEM_RED_APPLE",E_ITEM_RED_APPLE);
  tolua_constant(tolua_S,"E_ITEM_BOW",E_ITEM_BOW);
  tolua_constant(tolua_S,"E_ITEM_ARROW",E_ITEM_ARROW);
  tolua_constant(tolua_S,"E_ITEM_COAL",E_ITEM_COAL);
  tolua_constant(tolua_S,"E_ITEM_DIAMOND",E_ITEM_DIAMOND);
  tolua_constant(tolua_S,"E_ITEM_IRON",E_ITEM_IRON);
  tolua_constant(tolua_S,"E_ITEM_GOLD",E_ITEM_GOLD);
  tolua_constant(tolua_S,"E_ITEM_IRON_SWORD",E_ITEM_IRON_SWORD);
  tolua_constant(tolua_S,"E_ITEM_WOODEN_SWORD",E_ITEM_WOODEN_SWORD);
  tolua_constant(tolua_S,"E_ITEM_WOODEN_SHOVEL",E_ITEM_WOODEN_SHOVEL);
  tolua_constant(tolua_S,"E_ITEM_WOODEN_PICKAXE",E_ITEM_WOODEN_PICKAXE);
  tolua_constant(tolua_S,"E_ITEM_WOODEN_AXE",E_ITEM_WOODEN_AXE);
  tolua_constant(tolua_S,"E_ITEM_STONE_SWORD",E_ITEM_STONE_SWORD);
  tolua_constant(tolua_S,"E_ITEM_STONE_SHOVEL",E_ITEM_STONE_SHOVEL);
  tolua_constant(tolua_S,"E_ITEM_STONE_PICKAXE",E_ITEM_STONE_PICKAXE);
  tolua_constant(tolua_S,"E_ITEM_STONE_AXE",E_ITEM_STONE_AXE);
  tolua_constant(tolua_S,"E_ITEM_DIAMOND_SWORD",E_ITEM_DIAMOND_SWORD);
  tolua_constant(tolua_S,"E_ITEM_DIAMOND_SHOVEL",E_ITEM_DIAMOND_SHOVEL);
  tolua_constant(tolua_S,"E_ITEM_DIAMOND_PICKAXE",E_ITEM_DIAMOND_PICKAXE);
  tolua_constant(tolua_S,"E_ITEM_DIAMOND_AXE",E_ITEM_DIAMOND_AXE);
  tolua_constant(tolua_S,"E_ITEM_STICK",E_ITEM_STICK);
  tolua_constant(tolua_S,"E_ITEM_BOWL",E_ITEM_BOWL);
  tolua_constant(tolua_S,"E_ITEM_MUSHROOM_SOUP",E_ITEM_MUSHROOM_SOUP);
  tolua_constant(tolua_S,"E_ITEM_GOLD_SWORD",E_ITEM_GOLD_SWORD);
  tolua_constant(tolua_S,"E_ITEM_GOLD_SHOVEL",E_ITEM_GOLD_SHOVEL);
  tolua_constant(tolua_S,"E_ITEM_GOLD_PICKAXE",E_ITEM_GOLD_PICKAXE);
  tolua_constant(tolua_S,"E_ITEM_GOLD_AXE",E_ITEM_GOLD_AXE);
  tolua_constant(tolua_S,"E_ITEM_STRING",E_ITEM_STRING);
  tolua_constant(tolua_S,"E_ITEM_FEATHER",E_ITEM_FEATHER);
  tolua_constant(tolua_S,"E_ITEM_GUNPOWDER",E_ITEM_GUNPOWDER);
  tolua_constant(tolua_S,"E_ITEM_WOODEN_HOE",E_ITEM_WOODEN_HOE);
  tolua_constant(tolua_S,"E_ITEM_STONE_HOE",E_ITEM_STONE_HOE);
  tolua_constant(tolua_S,"E_ITEM_IRON_HOE",E_ITEM_IRON_HOE);
  tolua_constant(tolua_S,"E_ITEM_DIAMOND_HOE",E_ITEM_DIAMOND_HOE);
  tolua_constant(tolua_S,"E_ITEM_GOLD_HOE",E_ITEM_GOLD_HOE);
  tolua_constant(tolua_S,"E_ITEM_SEEDS",E_ITEM_SEEDS);
  tolua_constant(tolua_S,"E_ITEM_WHEAT",E_ITEM_WHEAT);
  tolua_constant(tolua_S,"E_ITEM_BREAD",E_ITEM_BREAD);
  tolua_constant(tolua_S,"E_ITEM_LEATHER_CAP",E_ITEM_LEATHER_CAP);
  tolua_constant(tolua_S,"E_ITEM_LEATHER_TUNIC",E_ITEM_LEATHER_TUNIC);
  tolua_constant(tolua_S,"E_ITEM_LEATHER_PANTS",E_ITEM_LEATHER_PANTS);
  tolua_constant(tolua_S,"E_ITEM_LEATHER_BOOTS",E_ITEM_LEATHER_BOOTS);
  tolua_constant(tolua_S,"E_ITEM_CHAIN_HELMET",E_ITEM_CHAIN_HELMET);
  tolua_constant(tolua_S,"E_ITEM_CHAIN_CHESTPLATE",E_ITEM_CHAIN_CHESTPLATE);
  tolua_constant(tolua_S,"E_ITEM_CHAIN_LEGGINGS",E_ITEM_CHAIN_LEGGINGS);
  tolua_constant(tolua_S,"E_ITEM_CHAIN_BOOTS",E_ITEM_CHAIN_BOOTS);
  tolua_constant(tolua_S,"E_ITEM_IRON_HELMET",E_ITEM_IRON_HELMET);
  tolua_constant(tolua_S,"E_ITEM_IRON_CHESTPLATE",E_ITEM_IRON_CHESTPLATE);
  tolua_constant(tolua_S,"E_ITEM_IRON_LEGGINGS",E_ITEM_IRON_LEGGINGS);
  tolua_constant(tolua_S,"E_ITEM_IRON_BOOTS",E_ITEM_IRON_BOOTS);
  tolua_constant(tolua_S,"E_ITEM_DIAMOND_HELMET",E_ITEM_DIAMOND_HELMET);
  tolua_constant(tolua_S,"E_ITEM_DIAMOND_CHESTPLATE",E_ITEM_DIAMOND_CHESTPLATE);
  tolua_constant(tolua_S,"E_ITEM_DIAMOND_LEGGINGS",E_ITEM_DIAMOND_LEGGINGS);
  tolua_constant(tolua_S,"E_ITEM_DIAMOND_BOOTS",E_ITEM_DIAMOND_BOOTS);
  tolua_constant(tolua_S,"E_ITEM_GOLD_HELMET",E_ITEM_GOLD_HELMET);
  tolua_constant(tolua_S,"E_ITEM_GOLD_CHESTPLATE",E_ITEM_GOLD_CHESTPLATE);
  tolua_constant(tolua_S,"E_ITEM_GOLD_LEGGINGS",E_ITEM_GOLD_LEGGINGS);
  tolua_constant(tolua_S,"E_ITEM_GOLD_BOOTS",E_ITEM_GOLD_BOOTS);
  tolua_constant(tolua_S,"E_ITEM_FLINT",E_ITEM_FLINT);
  tolua_constant(tolua_S,"E_ITEM_RAW_PORKCHOP",E_ITEM_RAW_PORKCHOP);
  tolua_constant(tolua_S,"E_ITEM_COOKED_PORKCHOP",E_ITEM_COOKED_PORKCHOP);
  tolua_constant(tolua_S,"E_ITEM_PAINTINGS",E_ITEM_PAINTINGS);
  tolua_constant(tolua_S,"E_ITEM_GOLDEN_APPLE",E_ITEM_GOLDEN_APPLE);
  tolua_constant(tolua_S,"E_ITEM_SIGN",E_ITEM_SIGN);
  tolua_constant(tolua_S,"E_ITEM_WOODEN_DOOR",E_ITEM_WOODEN_DOOR);
  tolua_constant(tolua_S,"E_ITEM_BUCKET",E_ITEM_BUCKET);
  tolua_constant(tolua_S,"E_ITEM_WATER_BUCKET",E_ITEM_WATER_BUCKET);
  tolua_constant(tolua_S,"E_ITEM_LAVA_BUCKET",E_ITEM_LAVA_BUCKET);
  tolua_constant(tolua_S,"E_ITEM_MINECART",E_ITEM_MINECART);
  tolua_constant(tolua_S,"E_ITEM_SADDLE",E_ITEM_SADDLE);
  tolua_constant(tolua_S,"E_ITEM_IRON_DOOR",E_ITEM_IRON_DOOR);
  tolua_constant(tolua_S,"E_ITEM_REDSTONE_DUST",E_ITEM_REDSTONE_DUST);
  tolua_constant(tolua_S,"E_ITEM_SNOWBALL",E_ITEM_SNOWBALL);
  tolua_constant(tolua_S,"E_ITEM_BOAT",E_ITEM_BOAT);
  tolua_constant(tolua_S,"E_ITEM_LEATHER",E_ITEM_LEATHER);
  tolua_constant(tolua_S,"E_ITEM_MILK",E_ITEM_MILK);
  tolua_constant(tolua_S,"E_ITEM_CLAY_BRICK",E_ITEM_CLAY_BRICK);
  tolua_constant(tolua_S,"E_ITEM_CLAY",E_ITEM_CLAY);
  tolua_constant(tolua_S,"E_ITEM_SUGARCANE",E_ITEM_SUGARCANE);
  tolua_constant(tolua_S,"E_ITEM_SUGAR_CANE",E_ITEM_SUGAR_CANE);
  tolua_constant(tolua_S,"E_ITEM_PAPER",E_ITEM_PAPER);
  tolua_constant(tolua_S,"E_ITEM_BOOK",E_ITEM_BOOK);
  tolua_constant(tolua_S,"E_ITEM_SLIMEBALL",E_ITEM_SLIMEBALL);
  tolua_constant(tolua_S,"E_ITEM_CHEST_MINECART",E_ITEM_CHEST_MINECART);
  tolua_constant(tolua_S,"E_ITEM_FURNACE_MINECART",E_ITEM_FURNACE_MINECART);
  tolua_constant(tolua_S,"E_ITEM_EGG",E_ITEM_EGG);
  tolua_constant(tolua_S,"E_ITEM_COMPASS",E_ITEM_COMPASS);
  tolua_constant(tolua_S,"E_ITEM_FISHING_ROD",E_ITEM_FISHING_ROD);
  tolua_constant(tolua_S,"E_ITEM_CLOCK",E_ITEM_CLOCK);
  tolua_constant(tolua_S,"E_ITEM_GLOWSTONE_DUST",E_ITEM_GLOWSTONE_DUST);
  tolua_constant(tolua_S,"E_ITEM_RAW_FISH",E_ITEM_RAW_FISH);
  tolua_constant(tolua_S,"E_ITEM_COOKED_FISH",E_ITEM_COOKED_FISH);
  tolua_constant(tolua_S,"E_ITEM_DYE",E_ITEM_DYE);
  tolua_constant(tolua_S,"E_ITEM_BONE",E_ITEM_BONE);
  tolua_constant(tolua_S,"E_ITEM_SUGAR",E_ITEM_SUGAR);
  tolua_constant(tolua_S,"E_ITEM_CAKE",E_ITEM_CAKE);
  tolua_constant(tolua_S,"E_ITEM_BED",E_ITEM_BED);
  tolua_constant(tolua_S,"E_ITEM_REDSTONE_REPEATER",E_ITEM_REDSTONE_REPEATER);
  tolua_constant(tolua_S,"E_ITEM_COOKIE",E_ITEM_COOKIE);
  tolua_constant(tolua_S,"E_ITEM_MAP",E_ITEM_MAP);
  tolua_constant(tolua_S,"E_ITEM_SHEARS",E_ITEM_SHEARS);
  tolua_constant(tolua_S,"E_ITEM_MELON_SLICE",E_ITEM_MELON_SLICE);
  tolua_constant(tolua_S,"E_ITEM_PUMPKIN_SEEDS",E_ITEM_PUMPKIN_SEEDS);
  tolua_constant(tolua_S,"E_ITEM_MELON_SEEDS",E_ITEM_MELON_SEEDS);
  tolua_constant(tolua_S,"E_ITEM_RAW_BEEF",E_ITEM_RAW_BEEF);
  tolua_constant(tolua_S,"E_ITEM_STEAK",E_ITEM_STEAK);
  tolua_constant(tolua_S,"E_ITEM_RAW_CHICKEN",E_ITEM_RAW_CHICKEN);
  tolua_constant(tolua_S,"E_ITEM_COOKED_CHICKEN",E_ITEM_COOKED_CHICKEN);
  tolua_constant(tolua_S,"E_ITEM_ROTTEN_FLESH",E_ITEM_ROTTEN_FLESH);
  tolua_constant(tolua_S,"E_ITEM_ENDER_PEARL",E_ITEM_ENDER_PEARL);
  tolua_constant(tolua_S,"E_ITEM_BLAZE_ROD",E_ITEM_BLAZE_ROD);
  tolua_constant(tolua_S,"E_ITEM_GHAST_TEAR",E_ITEM_GHAST_TEAR);
  tolua_constant(tolua_S,"E_ITEM_GOLD_NUGGET",E_ITEM_GOLD_NUGGET);
  tolua_constant(tolua_S,"E_ITEM_NETHER_WART",E_ITEM_NETHER_WART);
  tolua_constant(tolua_S,"E_ITEM_POTIONS",E_ITEM_POTIONS);
  tolua_constant(tolua_S,"E_ITEM_GLASS_BOTTLE",E_ITEM_GLASS_BOTTLE);
  tolua_constant(tolua_S,"E_ITEM_SPIDER_EYE",E_ITEM_SPIDER_EYE);
  tolua_constant(tolua_S,"E_ITEM_FERMENTED_SPIDER_EYE",E_ITEM_FERMENTED_SPIDER_EYE);
  tolua_constant(tolua_S,"E_ITEM_BLAZE_POWDER",E_ITEM_BLAZE_POWDER);
  tolua_constant(tolua_S,"E_ITEM_MAGMA_CREAM",E_ITEM_MAGMA_CREAM);
  tolua_constant(tolua_S,"E_ITEM_BREWING_STAND",E_ITEM_BREWING_STAND);
  tolua_constant(tolua_S,"E_ITEM_CAULDRON",E_ITEM_CAULDRON);
  tolua_constant(tolua_S,"E_ITEM_EYE_OF_ENDER",E_ITEM_EYE_OF_ENDER);
  tolua_constant(tolua_S,"E_ITEM_GLISTERING_MELON",E_ITEM_GLISTERING_MELON);
  tolua_constant(tolua_S,"E_ITEM_SPAWN_EGG",E_ITEM_SPAWN_EGG);
  tolua_constant(tolua_S,"E_ITEM_BOTTLE_O_ENCHANTING",E_ITEM_BOTTLE_O_ENCHANTING);
  tolua_constant(tolua_S,"E_ITEM_FIRE_CHARGE",E_ITEM_FIRE_CHARGE);
  tolua_constant(tolua_S,"E_ITEM_BOOK_AND_QUILL",E_ITEM_BOOK_AND_QUILL);
  tolua_constant(tolua_S,"E_ITEM_WRITTEN_BOOK",E_ITEM_WRITTEN_BOOK);
  tolua_constant(tolua_S,"E_ITEM_EMERALD",E_ITEM_EMERALD);
  tolua_constant(tolua_S,"E_ITEM_FLOWER_POT",E_ITEM_FLOWER_POT);
  tolua_constant(tolua_S,"E_ITEM_CARROT",E_ITEM_CARROT);
  tolua_constant(tolua_S,"E_ITEM_POTATO",E_ITEM_POTATO);
  tolua_constant(tolua_S,"E_ITEM_BAKED_POTATO",E_ITEM_BAKED_POTATO);
  tolua_constant(tolua_S,"E_ITEM_POISONOUS_POTATO",E_ITEM_POISONOUS_POTATO);
  tolua_constant(tolua_S,"E_ITEM_GOLDEN_CARROT",E_ITEM_GOLDEN_CARROT);
  tolua_constant(tolua_S,"E_ITEM_HEAD",E_ITEM_HEAD);
  tolua_constant(tolua_S,"E_ITEM_CARROT_ON_STICK",E_ITEM_CARROT_ON_STICK);
  tolua_constant(tolua_S,"E_ITEM_PUMPKIN_PIE",E_ITEM_PUMPKIN_PIE);
  tolua_constant(tolua_S,"E_ITEM_FIREWORK_ROCKET",E_ITEM_FIREWORK_ROCKET);
  tolua_constant(tolua_S,"E_ITEM_FIREWORK_STAR",E_ITEM_FIREWORK_STAR);
  tolua_constant(tolua_S,"E_ITEM_ENCHANTED_BOOK",E_ITEM_ENCHANTED_BOOK);
  tolua_constant(tolua_S,"E_ITEM_COMPARATOR",E_ITEM_COMPARATOR);
  tolua_constant(tolua_S,"E_ITEM_NETHER_BRICK",E_ITEM_NETHER_BRICK);
  tolua_constant(tolua_S,"E_ITEM_NETHER_QUARTZ",E_ITEM_NETHER_QUARTZ);
  tolua_constant(tolua_S,"E_ITEM_MINECART_WITH_TNT",E_ITEM_MINECART_WITH_TNT);
  tolua_constant(tolua_S,"E_ITEM_MINECART_WITH_HOPPER",E_ITEM_MINECART_WITH_HOPPER);
  tolua_constant(tolua_S,"E_ITEM_NUMBER_OF_CONSECUTIVE_TYPES",E_ITEM_NUMBER_OF_CONSECUTIVE_TYPES);
  tolua_constant(tolua_S,"E_ITEM_MAX_CONSECUTIVE_TYPE_ID",E_ITEM_MAX_CONSECUTIVE_TYPE_ID);
  tolua_constant(tolua_S,"E_ITEM_FIRST_DISC",E_ITEM_FIRST_DISC);
  tolua_constant(tolua_S,"E_ITEM_13_DISC",E_ITEM_13_DISC);
  tolua_constant(tolua_S,"E_ITEM_CAT_DISC",E_ITEM_CAT_DISC);
  tolua_constant(tolua_S,"E_ITEM_BLOCKS_DISC",E_ITEM_BLOCKS_DISC);
  tolua_constant(tolua_S,"E_ITEM_CHIRP_DISC",E_ITEM_CHIRP_DISC);
  tolua_constant(tolua_S,"E_ITEM_FAR_DISC",E_ITEM_FAR_DISC);
  tolua_constant(tolua_S,"E_ITEM_MALL_DISC",E_ITEM_MALL_DISC);
  tolua_constant(tolua_S,"E_ITEM_MELLOHI_DISC",E_ITEM_MELLOHI_DISC);
  tolua_constant(tolua_S,"E_ITEM_STAL_DISC",E_ITEM_STAL_DISC);
  tolua_constant(tolua_S,"E_ITEM_STRAD_DISC",E_ITEM_STRAD_DISC);
  tolua_constant(tolua_S,"E_ITEM_WARD_DISC",E_ITEM_WARD_DISC);
  tolua_constant(tolua_S,"E_ITEM_11_DISC",E_ITEM_11_DISC);
  tolua_constant(tolua_S,"E_ITEM_WAIT_DISC",E_ITEM_WAIT_DISC);
  tolua_constant(tolua_S,"E_ITEM_LAST_DISC_PLUS_ONE",E_ITEM_LAST_DISC_PLUS_ONE);
  tolua_constant(tolua_S,"E_ITEM_LAST_DISC",E_ITEM_LAST_DISC);
  tolua_constant(tolua_S,"E_META_CHEST_FACING_ZM",E_META_CHEST_FACING_ZM);
  tolua_constant(tolua_S,"E_META_CHEST_FACING_ZP",E_META_CHEST_FACING_ZP);
  tolua_constant(tolua_S,"E_META_CHEST_FACING_XM",E_META_CHEST_FACING_XM);
  tolua_constant(tolua_S,"E_META_CHEST_FACING_XP",E_META_CHEST_FACING_XP);
  tolua_constant(tolua_S,"E_META_DROPSPENSER_FACING_YM",E_META_DROPSPENSER_FACING_YM);
  tolua_constant(tolua_S,"E_META_DROPSPENSER_FACING_YP",E_META_DROPSPENSER_FACING_YP);
  tolua_constant(tolua_S,"E_META_DROPSPENSER_FACING_ZM",E_META_DROPSPENSER_FACING_ZM);
  tolua_constant(tolua_S,"E_META_DROPSPENSER_FACING_ZP",E_META_DROPSPENSER_FACING_ZP);
  tolua_constant(tolua_S,"E_META_DROPSPENSER_FACING_XM",E_META_DROPSPENSER_FACING_XM);
  tolua_constant(tolua_S,"E_META_DROPSPENSER_FACING_XP",E_META_DROPSPENSER_FACING_XP);
  tolua_constant(tolua_S,"E_META_DOUBLE_STEP_STONE",E_META_DOUBLE_STEP_STONE);
  tolua_constant(tolua_S,"E_META_DOUBLE_STEP_SANDSTONE",E_META_DOUBLE_STEP_SANDSTONE);
  tolua_constant(tolua_S,"E_META_DOUBLE_STEP_WOODEN",E_META_DOUBLE_STEP_WOODEN);
  tolua_constant(tolua_S,"E_META_DOUBLE_STEP_COBBLESTONE",E_META_DOUBLE_STEP_COBBLESTONE);
  tolua_constant(tolua_S,"E_META_DOUBLE_STEP_BRICK",E_META_DOUBLE_STEP_BRICK);
  tolua_constant(tolua_S,"E_META_DOUBLE_STEP_STONE_BRICK",E_META_DOUBLE_STEP_STONE_BRICK);
  tolua_constant(tolua_S,"E_META_DOUBLE_STEP_NETHER_BRICK",E_META_DOUBLE_STEP_NETHER_BRICK);
  tolua_constant(tolua_S,"E_META_DOUBLE_STEP_STONE_SECRET",E_META_DOUBLE_STEP_STONE_SECRET);
  tolua_constant(tolua_S,"E_META_HOPPER_FACING_YM",E_META_HOPPER_FACING_YM);
  tolua_constant(tolua_S,"E_META_HOPPER_UNATTACHED",E_META_HOPPER_UNATTACHED);
  tolua_constant(tolua_S,"E_META_HOPPER_FACING_ZM",E_META_HOPPER_FACING_ZM);
  tolua_constant(tolua_S,"E_META_HOPPER_FACING_ZP",E_META_HOPPER_FACING_ZP);
  tolua_constant(tolua_S,"E_META_HOPPER_FACING_XM",E_META_HOPPER_FACING_XM);
  tolua_constant(tolua_S,"E_META_HOPPER_FACING_XP",E_META_HOPPER_FACING_XP);
  tolua_constant(tolua_S,"E_META_LEAVES_APPLE",E_META_LEAVES_APPLE);
  tolua_constant(tolua_S,"E_META_LEAVES_CONIFER",E_META_LEAVES_CONIFER);
  tolua_constant(tolua_S,"E_META_LEAVES_BIRCH",E_META_LEAVES_BIRCH);
  tolua_constant(tolua_S,"E_META_LEAVES_JUNGLE",E_META_LEAVES_JUNGLE);
  tolua_constant(tolua_S,"E_META_LOG_APPLE",E_META_LOG_APPLE);
  tolua_constant(tolua_S,"E_META_LOG_CONIFER",E_META_LOG_CONIFER);
  tolua_constant(tolua_S,"E_META_LOG_BIRCH",E_META_LOG_BIRCH);
  tolua_constant(tolua_S,"E_META_LOG_JUNGLE",E_META_LOG_JUNGLE);
  tolua_constant(tolua_S,"E_META_PLANKS_APPLE",E_META_PLANKS_APPLE);
  tolua_constant(tolua_S,"E_META_PLANKS_CONIFER",E_META_PLANKS_CONIFER);
  tolua_constant(tolua_S,"E_META_PLANKS_BIRCH",E_META_PLANKS_BIRCH);
  tolua_constant(tolua_S,"E_META_PLANKS_JUNGLE",E_META_PLANKS_JUNGLE);
  tolua_constant(tolua_S,"E_META_SANDSTONE_NORMAL",E_META_SANDSTONE_NORMAL);
  tolua_constant(tolua_S,"E_META_SANDSTONE_ORNAMENT",E_META_SANDSTONE_ORNAMENT);
  tolua_constant(tolua_S,"E_META_SANDSTONE_SMOOTH",E_META_SANDSTONE_SMOOTH);
  tolua_constant(tolua_S,"E_META_SAPLING_APPLE",E_META_SAPLING_APPLE);
  tolua_constant(tolua_S,"E_META_SAPLING_CONIFER",E_META_SAPLING_CONIFER);
  tolua_constant(tolua_S,"E_META_SAPLING_BIRCH",E_META_SAPLING_BIRCH);
  tolua_constant(tolua_S,"E_META_SAPLING_JUNGLE",E_META_SAPLING_JUNGLE);
  tolua_constant(tolua_S,"E_META_SILVERFISH_EGG_STONE",E_META_SILVERFISH_EGG_STONE);
  tolua_constant(tolua_S,"E_META_SILVERFISH_EGG_COBBLESTONE",E_META_SILVERFISH_EGG_COBBLESTONE);
  tolua_constant(tolua_S,"E_META_SILVERFISH_EGG_STONE_BRICK",E_META_SILVERFISH_EGG_STONE_BRICK);
  tolua_constant(tolua_S,"E_META_STEP_STONE",E_META_STEP_STONE);
  tolua_constant(tolua_S,"E_META_STEP_SANDSTONE",E_META_STEP_SANDSTONE);
  tolua_constant(tolua_S,"E_META_STEP_PLANKS",E_META_STEP_PLANKS);
  tolua_constant(tolua_S,"E_META_STEP_COBBLESTONE",E_META_STEP_COBBLESTONE);
  tolua_constant(tolua_S,"E_META_STEP_BRICK",E_META_STEP_BRICK);
  tolua_constant(tolua_S,"E_META_STEP_STONE_BRICK",E_META_STEP_STONE_BRICK);
  tolua_constant(tolua_S,"E_META_STEP_NETHER_BRICK",E_META_STEP_NETHER_BRICK);
  tolua_constant(tolua_S,"E_META_STEP_STONE_SECRET",E_META_STEP_STONE_SECRET);
  tolua_constant(tolua_S,"E_META_STONE_BRICK_NORMAL",E_META_STONE_BRICK_NORMAL);
  tolua_constant(tolua_S,"E_META_STONE_BRICK_MOSSY",E_META_STONE_BRICK_MOSSY);
  tolua_constant(tolua_S,"E_META_STONE_BRICK_CRACKED",E_META_STONE_BRICK_CRACKED);
  tolua_constant(tolua_S,"E_META_STONE_BRICK_ORNAMENT",E_META_STONE_BRICK_ORNAMENT);
  tolua_constant(tolua_S,"E_META_TALL_GRASS_DEAD_SHRUB",E_META_TALL_GRASS_DEAD_SHRUB);
  tolua_constant(tolua_S,"E_META_TALL_GRASS_GRASS",E_META_TALL_GRASS_GRASS);
  tolua_constant(tolua_S,"E_META_TALL_GRASS_FERN",E_META_TALL_GRASS_FERN);
  tolua_constant(tolua_S,"E_META_TORCH_EAST",E_META_TORCH_EAST);
  tolua_constant(tolua_S,"E_META_TORCH_WEST",E_META_TORCH_WEST);
  tolua_constant(tolua_S,"E_META_TORCH_SOUTH",E_META_TORCH_SOUTH);
  tolua_constant(tolua_S,"E_META_TORCH_NORTH",E_META_TORCH_NORTH);
  tolua_constant(tolua_S,"E_META_TORCH_FLOOR",E_META_TORCH_FLOOR);
  tolua_constant(tolua_S,"E_META_TORCH_XM",E_META_TORCH_XM);
  tolua_constant(tolua_S,"E_META_TORCH_XP",E_META_TORCH_XP);
  tolua_constant(tolua_S,"E_META_TORCH_ZM",E_META_TORCH_ZM);
  tolua_constant(tolua_S,"E_META_TORCH_ZP",E_META_TORCH_ZP);
  tolua_constant(tolua_S,"E_BLOCK_WOODEN_DOUBLE_STEP_APPLE",E_BLOCK_WOODEN_DOUBLE_STEP_APPLE);
  tolua_constant(tolua_S,"E_BLOCK_WOODEN_DOUBLE_STEP_CONIFER",E_BLOCK_WOODEN_DOUBLE_STEP_CONIFER);
  tolua_constant(tolua_S,"E_BLOCK_WOODEN_DOUBLE_STEP_BIRCH",E_BLOCK_WOODEN_DOUBLE_STEP_BIRCH);
  tolua_constant(tolua_S,"E_BLOCK_WOODEN_DOUBLE_STEP_JUNGLE",E_BLOCK_WOODEN_DOUBLE_STEP_JUNGLE);
  tolua_constant(tolua_S,"E_BLOCK_WOODEN_STEP_APPLE",E_BLOCK_WOODEN_STEP_APPLE);
  tolua_constant(tolua_S,"E_BLOCK_WOODEN_STEP_CONIFER",E_BLOCK_WOODEN_STEP_CONIFER);
  tolua_constant(tolua_S,"E_BLOCK_WOODEN_STEP_BIRCH",E_BLOCK_WOODEN_STEP_BIRCH);
  tolua_constant(tolua_S,"E_BLOCK_WOODEN_STEP_JUNGLE",E_BLOCK_WOODEN_STEP_JUNGLE);
  tolua_constant(tolua_S,"E_META_WOOL_WHITE",E_META_WOOL_WHITE);
  tolua_constant(tolua_S,"E_META_WOOL_ORANGE",E_META_WOOL_ORANGE);
  tolua_constant(tolua_S,"E_META_WOOL_MAGENTA",E_META_WOOL_MAGENTA);
  tolua_constant(tolua_S,"E_META_WOOL_LIGHTBLUE",E_META_WOOL_LIGHTBLUE);
  tolua_constant(tolua_S,"E_META_WOOL_YELLOW",E_META_WOOL_YELLOW);
  tolua_constant(tolua_S,"E_META_WOOL_LIGHTGREEN",E_META_WOOL_LIGHTGREEN);
  tolua_constant(tolua_S,"E_META_WOOL_PINK",E_META_WOOL_PINK);
  tolua_constant(tolua_S,"E_META_WOOL_GRAY",E_META_WOOL_GRAY);
  tolua_constant(tolua_S,"E_META_WOOL_LIGHTGRAY",E_META_WOOL_LIGHTGRAY);
  tolua_constant(tolua_S,"E_META_WOOL_CYAN",E_META_WOOL_CYAN);
  tolua_constant(tolua_S,"E_META_WOOL_PURPLE",E_META_WOOL_PURPLE);
  tolua_constant(tolua_S,"E_META_WOOL_BLUE",E_META_WOOL_BLUE);
  tolua_constant(tolua_S,"E_META_WOOL_BROWN",E_META_WOOL_BROWN);
  tolua_constant(tolua_S,"E_META_WOOL_GREEN",E_META_WOOL_GREEN);
  tolua_constant(tolua_S,"E_META_WOOL_RED",E_META_WOOL_RED);
  tolua_constant(tolua_S,"E_META_WOOL_BLACK",E_META_WOOL_BLACK);
  tolua_constant(tolua_S,"E_META_COAL_NORMAL",E_META_COAL_NORMAL);
  tolua_constant(tolua_S,"E_META_COAL_CHARCOAL",E_META_COAL_CHARCOAL);
  tolua_constant(tolua_S,"E_META_DYE_BLACK",E_META_DYE_BLACK);
  tolua_constant(tolua_S,"E_META_DYE_RED",E_META_DYE_RED);
  tolua_constant(tolua_S,"E_META_DYE_GREEN",E_META_DYE_GREEN);
  tolua_constant(tolua_S,"E_META_DYE_BROWN",E_META_DYE_BROWN);
  tolua_constant(tolua_S,"E_META_DYE_BLUE",E_META_DYE_BLUE);
  tolua_constant(tolua_S,"E_META_DYE_PURPLE",E_META_DYE_PURPLE);
  tolua_constant(tolua_S,"E_META_DYE_CYAN",E_META_DYE_CYAN);
  tolua_constant(tolua_S,"E_META_DYE_LIGHTGRAY",E_META_DYE_LIGHTGRAY);
  tolua_constant(tolua_S,"E_META_DYE_GRAY",E_META_DYE_GRAY);
  tolua_constant(tolua_S,"E_META_DYE_PINK",E_META_DYE_PINK);
  tolua_constant(tolua_S,"E_META_DYE_LIGHTGREEN",E_META_DYE_LIGHTGREEN);
  tolua_constant(tolua_S,"E_META_DYE_YELLOW",E_META_DYE_YELLOW);
  tolua_constant(tolua_S,"E_META_DYE_LIGHTBLUE",E_META_DYE_LIGHTBLUE);
  tolua_constant(tolua_S,"E_META_DYE_MAGENTA",E_META_DYE_MAGENTA);
  tolua_constant(tolua_S,"E_META_DYE_ORANGE",E_META_DYE_ORANGE);
  tolua_constant(tolua_S,"E_META_DYE_WHITE",E_META_DYE_WHITE);
  tolua_constant(tolua_S,"E_META_GOLDEN_APPLE_NORMAL",E_META_GOLDEN_APPLE_NORMAL);
  tolua_constant(tolua_S,"E_META_GOLDEN_APPLE_ENCHANTED",E_META_GOLDEN_APPLE_ENCHANTED);
  tolua_constant(tolua_S,"E_META_TRACKS_X",E_META_TRACKS_X);
  tolua_constant(tolua_S,"E_META_TRACKS_Z",E_META_TRACKS_Z);
  tolua_constant(tolua_S,"E_META_SPAWN_EGG_CREEPER",E_META_SPAWN_EGG_CREEPER);
  tolua_constant(tolua_S,"E_META_SPAWN_EGG_SKELETON",E_META_SPAWN_EGG_SKELETON);
  tolua_constant(tolua_S,"E_META_SPAWN_EGG_SPIDER",E_META_SPAWN_EGG_SPIDER);
  tolua_constant(tolua_S,"E_META_SPAWN_EGG_ZOMBIE",E_META_SPAWN_EGG_ZOMBIE);
  tolua_constant(tolua_S,"E_META_SPAWN_EGG_GIANT",E_META_SPAWN_EGG_GIANT);
  tolua_constant(tolua_S,"E_META_SPAWN_EGG_SLIME",E_META_SPAWN_EGG_SLIME);
  tolua_constant(tolua_S,"E_META_SPAWN_EGG_GHAST",E_META_SPAWN_EGG_GHAST);
  tolua_constant(tolua_S,"E_META_SPAWN_EGG_ZOMBIE_PIGMAN",E_META_SPAWN_EGG_ZOMBIE_PIGMAN);
  tolua_constant(tolua_S,"E_META_SPAWN_EGG_ENDERMAN",E_META_SPAWN_EGG_ENDERMAN);
  tolua_constant(tolua_S,"E_META_SPAWN_EGG_CAVE_SPIDER",E_META_SPAWN_EGG_CAVE_SPIDER);
  tolua_constant(tolua_S,"E_META_SPAWN_EGG_SILVERFISH",E_META_SPAWN_EGG_SILVERFISH);
  tolua_constant(tolua_S,"E_META_SPAWN_EGG_BLAZE",E_META_SPAWN_EGG_BLAZE);
  tolua_constant(tolua_S,"E_META_SPAWN_EGG_MAGMA_CUBE",E_META_SPAWN_EGG_MAGMA_CUBE);
  tolua_constant(tolua_S,"E_META_SPAWN_EGG_ENDER_DRAGON",E_META_SPAWN_EGG_ENDER_DRAGON);
  tolua_constant(tolua_S,"E_META_SPAWN_EGG_PIG",E_META_SPAWN_EGG_PIG);
  tolua_constant(tolua_S,"E_META_SPAWN_EGG_SHEEP",E_META_SPAWN_EGG_SHEEP);
  tolua_constant(tolua_S,"E_META_SPAWN_EGG_COW",E_META_SPAWN_EGG_COW);
  tolua_constant(tolua_S,"E_META_SPAWN_EGG_CHICKEN",E_META_SPAWN_EGG_CHICKEN);
  tolua_constant(tolua_S,"E_META_SPAWN_EGG_SQUID",E_META_SPAWN_EGG_SQUID);
  tolua_constant(tolua_S,"E_META_SPAWN_EGG_WOLF",E_META_SPAWN_EGG_WOLF);
  tolua_constant(tolua_S,"E_META_SPAWN_EGG_MOOSHROOM",E_META_SPAWN_EGG_MOOSHROOM);
  tolua_constant(tolua_S,"E_META_SPAWN_EGG_SNOW_GOLEM",E_META_SPAWN_EGG_SNOW_GOLEM);
  tolua_constant(tolua_S,"E_META_SPAWN_EGG_OCELOT",E_META_SPAWN_EGG_OCELOT);
  tolua_constant(tolua_S,"E_META_SPAWN_EGG_IRON_GOLEM",E_META_SPAWN_EGG_IRON_GOLEM);
  tolua_constant(tolua_S,"E_META_SPAWN_EGG_VILLAGER",E_META_SPAWN_EGG_VILLAGER);
  tolua_constant(tolua_S,"E_ENTITY_TYPE_CREEPER",E_ENTITY_TYPE_CREEPER);
  tolua_constant(tolua_S,"E_ENTITY_TYPE_SKELETON",E_ENTITY_TYPE_SKELETON);
  tolua_constant(tolua_S,"E_ENTITY_TYPE_SPIDER",E_ENTITY_TYPE_SPIDER);
  tolua_constant(tolua_S,"E_ENTITY_TYPE_GIANT",E_ENTITY_TYPE_GIANT);
  tolua_constant(tolua_S,"E_ENTITY_TYPE_ZOMBIE",E_ENTITY_TYPE_ZOMBIE);
  tolua_constant(tolua_S,"E_ENTITY_TYPE_SLIME",E_ENTITY_TYPE_SLIME);
  tolua_constant(tolua_S,"E_ENTITY_TYPE_GHAST",E_ENTITY_TYPE_GHAST);
  tolua_constant(tolua_S,"E_ENTITY_TYPE_ZOMBIE_PIGMAN",E_ENTITY_TYPE_ZOMBIE_PIGMAN);
  tolua_constant(tolua_S,"E_ENTITY_TYPE_ENDERMAN",E_ENTITY_TYPE_ENDERMAN);
  tolua_constant(tolua_S,"E_ENTITY_TYPE_CAVE_SPIDER",E_ENTITY_TYPE_CAVE_SPIDER);
  tolua_constant(tolua_S,"E_ENTITY_TYPE_SILVERFISH",E_ENTITY_TYPE_SILVERFISH);
  tolua_constant(tolua_S,"E_ENTITY_TYPE_BLAZE",E_ENTITY_TYPE_BLAZE);
  tolua_constant(tolua_S,"E_ENTITY_TYPE_MAGMA_CUBE",E_ENTITY_TYPE_MAGMA_CUBE);
  tolua_constant(tolua_S,"E_ENTITY_TYPE_ENDER_DRAGON",E_ENTITY_TYPE_ENDER_DRAGON);
  tolua_constant(tolua_S,"E_ENTITY_TYPE_WITHER",E_ENTITY_TYPE_WITHER);
  tolua_constant(tolua_S,"E_ENTITY_TYPE_BAT",E_ENTITY_TYPE_BAT);
  tolua_constant(tolua_S,"E_ENTITY_TYPE_WITCH",E_ENTITY_TYPE_WITCH);
  tolua_constant(tolua_S,"E_ENTITY_TYPE_PIG",E_ENTITY_TYPE_PIG);
  tolua_constant(tolua_S,"E_ENTITY_TYPE_SHEEP",E_ENTITY_TYPE_SHEEP);
  tolua_constant(tolua_S,"E_ENTITY_TYPE_COW",E_ENTITY_TYPE_COW);
  tolua_constant(tolua_S,"E_ENTITY_TYPE_CHICKEN",E_ENTITY_TYPE_CHICKEN);
  tolua_constant(tolua_S,"E_ENTITY_TYPE_SQUID",E_ENTITY_TYPE_SQUID);
  tolua_constant(tolua_S,"E_ENTITY_TYPE_WOLF",E_ENTITY_TYPE_WOLF);
  tolua_constant(tolua_S,"E_ENTITY_TYPE_MOOSHROOM",E_ENTITY_TYPE_MOOSHROOM);
  tolua_constant(tolua_S,"E_ENTITY_TYPE_SNOW_GOLEM",E_ENTITY_TYPE_SNOW_GOLEM);
  tolua_constant(tolua_S,"E_ENTITY_TYPE_OCELOT",E_ENTITY_TYPE_OCELOT);
  tolua_constant(tolua_S,"E_ENTITY_TYPE_IRON_GOLEM",E_ENTITY_TYPE_IRON_GOLEM);
  tolua_constant(tolua_S,"E_ENTITY_TYPE_VILLAGER",E_ENTITY_TYPE_VILLAGER);
  tolua_constant(tolua_S,"dimNether",dimNether);
  tolua_constant(tolua_S,"dimOverworld",dimOverworld);
  tolua_constant(tolua_S,"dimEnd",dimEnd);
  tolua_constant(tolua_S,"dtAttack",dtAttack);
  tolua_constant(tolua_S,"dtLightning",dtLightning);
  tolua_constant(tolua_S,"dtFalling",dtFalling);
  tolua_constant(tolua_S,"dtDrowning",dtDrowning);
  tolua_constant(tolua_S,"dtSuffocating",dtSuffocating);
  tolua_constant(tolua_S,"dtStarving",dtStarving);
  tolua_constant(tolua_S,"dtCactusContact",dtCactusContact);
  tolua_constant(tolua_S,"dtLavaContact",dtLavaContact);
  tolua_constant(tolua_S,"dtPoisoning",dtPoisoning);
  tolua_constant(tolua_S,"dtOnFire",dtOnFire);
  tolua_constant(tolua_S,"dtFireContact",dtFireContact);
  tolua_constant(tolua_S,"dtInVoid",dtInVoid);
  tolua_constant(tolua_S,"dtPotionOfHarming",dtPotionOfHarming);
  tolua_constant(tolua_S,"dtAdmin",dtAdmin);
  tolua_constant(tolua_S,"dtPawnAttack",dtPawnAttack);
  tolua_constant(tolua_S,"dtEntityAttack",dtEntityAttack);
  tolua_constant(tolua_S,"dtMob",dtMob);
  tolua_constant(tolua_S,"dtMobAttack",dtMobAttack);
  tolua_constant(tolua_S,"dtFall",dtFall);
  tolua_constant(tolua_S,"dtDrown",dtDrown);
  tolua_constant(tolua_S,"dtSuffocation",dtSuffocation);
  tolua_constant(tolua_S,"dtStarvation",dtStarvation);
  tolua_constant(tolua_S,"dtHunger",dtHunger);
  tolua_constant(tolua_S,"dtCactus",dtCactus);
  tolua_constant(tolua_S,"dtCactuses",dtCactuses);
  tolua_constant(tolua_S,"dtCacti",dtCacti);
  tolua_constant(tolua_S,"dtLava",dtLava);
  tolua_constant(tolua_S,"dtPoison",dtPoison);
  tolua_constant(tolua_S,"dtBurning",dtBurning);
  tolua_constant(tolua_S,"dtInFire",dtInFire);
  tolua_constant(tolua_S,"dtPlugin",dtPlugin);
  tolua_function(tolua_S,"BlockStringToType",tolua_AllToLua_BlockStringToType00);
  tolua_function(tolua_S,"StringToItem",tolua_AllToLua_StringToItem00);
  tolua_function(tolua_S,"ItemToString",tolua_AllToLua_ItemToString00);
  tolua_function(tolua_S,"ItemTypeToString",tolua_AllToLua_ItemTypeToString00);
  tolua_function(tolua_S,"ItemToFullString",tolua_AllToLua_ItemToFullString00);
  tolua_function(tolua_S,"StringToBiome",tolua_AllToLua_StringToBiome00);
  tolua_function(tolua_S,"StringToMobType",tolua_AllToLua_StringToMobType00);
  tolua_function(tolua_S,"StringToDimension",tolua_AllToLua_StringToDimension00);
  tolua_function(tolua_S,"DamageTypeToString",tolua_AllToLua_DamageTypeToString00);
  tolua_function(tolua_S,"StringToDamageType",tolua_AllToLua_StringToDamageType00);
  tolua_function(tolua_S,"TrimString",tolua_AllToLua_TrimString00);
  tolua_function(tolua_S,"NoCaseCompare",tolua_AllToLua_NoCaseCompare00);
  tolua_function(tolua_S,"ReplaceString",tolua_AllToLua_ReplaceString00);
  tolua_function(tolua_S,"EscapeString",tolua_AllToLua_EscapeString00);
  tolua_function(tolua_S,"StripColorCodes",tolua_AllToLua_StripColorCodes00);
  tolua_array(tolua_S,"g_BlockLightValue",tolua_get_AllToLua_g_BlockLightValue,tolua_set_AllToLua_g_BlockLightValue);
  tolua_array(tolua_S,"g_BlockSpreadLightFalloff",tolua_get_AllToLua_g_BlockSpreadLightFalloff,tolua_set_AllToLua_g_BlockSpreadLightFalloff);
  tolua_array(tolua_S,"g_BlockTransparent",tolua_get_AllToLua_g_BlockTransparent,tolua_set_AllToLua_g_BlockTransparent);
  tolua_array(tolua_S,"g_BlockOneHitDig",tolua_get_AllToLua_g_BlockOneHitDig,tolua_set_AllToLua_g_BlockOneHitDig);
  tolua_constant(tolua_S,"BLOCK_FACE_NONE",BLOCK_FACE_NONE);
  tolua_constant(tolua_S,"BLOCK_FACE_BOTTOM",BLOCK_FACE_BOTTOM);
  tolua_constant(tolua_S,"BLOCK_FACE_TOP",BLOCK_FACE_TOP);
  tolua_constant(tolua_S,"BLOCK_FACE_NORTH",BLOCK_FACE_NORTH);
  tolua_constant(tolua_S,"BLOCK_FACE_SOUTH",BLOCK_FACE_SOUTH);
  tolua_constant(tolua_S,"BLOCK_FACE_WEST",BLOCK_FACE_WEST);
  tolua_constant(tolua_S,"BLOCK_FACE_EAST",BLOCK_FACE_EAST);
  tolua_constant(tolua_S,"DIG_STATUS_STARTED",DIG_STATUS_STARTED);
  tolua_constant(tolua_S,"DIG_STATUS_CANCELLED",DIG_STATUS_CANCELLED);
  tolua_constant(tolua_S,"DIG_STATUS_FINISHED",DIG_STATUS_FINISHED);
  tolua_constant(tolua_S,"DIG_STATUS_DROP_HELD",DIG_STATUS_DROP_HELD);
  tolua_constant(tolua_S,"DIG_STATUS_SHOOT_EAT",DIG_STATUS_SHOOT_EAT);
  tolua_constant(tolua_S,"caLeftClick",caLeftClick);
  tolua_constant(tolua_S,"caRightClick",caRightClick);
  tolua_constant(tolua_S,"caShiftLeftClick",caShiftLeftClick);
  tolua_constant(tolua_S,"caShiftRightClick",caShiftRightClick);
  tolua_constant(tolua_S,"caNumber1",caNumber1);
  tolua_constant(tolua_S,"caNumber2",caNumber2);
  tolua_constant(tolua_S,"caNumber3",caNumber3);
  tolua_constant(tolua_S,"caNumber4",caNumber4);
  tolua_constant(tolua_S,"caNumber5",caNumber5);
  tolua_constant(tolua_S,"caNumber6",caNumber6);
  tolua_constant(tolua_S,"caNumber7",caNumber7);
  tolua_constant(tolua_S,"caNumber8",caNumber8);
  tolua_constant(tolua_S,"caNumber9",caNumber9);
  tolua_constant(tolua_S,"caMiddleClick",caMiddleClick);
  tolua_constant(tolua_S,"caDropKey",caDropKey);
  tolua_constant(tolua_S,"caCtrlDropKey",caCtrlDropKey);
  tolua_constant(tolua_S,"caLeftClickOutside",caLeftClickOutside);
  tolua_constant(tolua_S,"caRightClickOutside",caRightClickOutside);
  tolua_constant(tolua_S,"caLeftClickOutsideHoldNothing",caLeftClickOutsideHoldNothing);
  tolua_constant(tolua_S,"caRightClickOutsideHoldNothing",caRightClickOutsideHoldNothing);
  tolua_constant(tolua_S,"caLeftPaintBegin",caLeftPaintBegin);
  tolua_constant(tolua_S,"caRightPaintBegin",caRightPaintBegin);
  tolua_constant(tolua_S,"caLeftPaintProgress",caLeftPaintProgress);
  tolua_constant(tolua_S,"caRightPaintProgress",caRightPaintProgress);
  tolua_constant(tolua_S,"caLeftPaintEnd",caLeftPaintEnd);
  tolua_constant(tolua_S,"caRightPaintEnd",caRightPaintEnd);
  tolua_constant(tolua_S,"caDblClick",caDblClick);
  tolua_constant(tolua_S,"caUnknown",caUnknown);
  tolua_constant(tolua_S,"eGameMode_NotSet",eGameMode_NotSet);
  tolua_constant(tolua_S,"eGameMode_Survival",eGameMode_Survival);
  tolua_constant(tolua_S,"eGameMode_Creative",eGameMode_Creative);
  tolua_constant(tolua_S,"eGameMode_Adventure",eGameMode_Adventure);
  tolua_constant(tolua_S,"gmNotSet",gmNotSet);
  tolua_constant(tolua_S,"gmSurvival",gmSurvival);
  tolua_constant(tolua_S,"gmCreative",gmCreative);
  tolua_constant(tolua_S,"gmAdventure",gmAdventure);
  tolua_constant(tolua_S,"gmMax",gmMax);
  tolua_constant(tolua_S,"gmMin",gmMin);
  tolua_constant(tolua_S,"eWeather_Sunny",eWeather_Sunny);
  tolua_constant(tolua_S,"eWeather_Rain",eWeather_Rain);
  tolua_constant(tolua_S,"eWeather_ThunderStorm",eWeather_ThunderStorm);
  tolua_constant(tolua_S,"wSunny",wSunny);
  tolua_constant(tolua_S,"wRain",wRain);
  tolua_constant(tolua_S,"wThunderstorm",wThunderstorm);
  tolua_constant(tolua_S,"wStorm",wStorm);
  tolua_function(tolua_S,"ClickActionToString",tolua_AllToLua_ClickActionToString00);
  tolua_function(tolua_S,"IsValidBlock",tolua_AllToLua_IsValidBlock00);
  tolua_function(tolua_S,"IsValidItem",tolua_AllToLua_IsValidItem00);
  tolua_function(tolua_S,"AddFaceDirection",tolua_AllToLua_AddFaceDirection00);
  tolua_module(tolua_S,"ItemCategory",0);
  tolua_beginmodule(tolua_S,"ItemCategory");
   tolua_function(tolua_S,"IsPickaxe",tolua_AllToLua_ItemCategory_IsPickaxe00);
   tolua_function(tolua_S,"IsAxe",tolua_AllToLua_ItemCategory_IsAxe00);
   tolua_function(tolua_S,"IsSword",tolua_AllToLua_ItemCategory_IsSword00);
   tolua_function(tolua_S,"IsHoe",tolua_AllToLua_ItemCategory_IsHoe00);
   tolua_function(tolua_S,"IsShovel",tolua_AllToLua_ItemCategory_IsShovel00);
   tolua_function(tolua_S,"IsTool",tolua_AllToLua_ItemCategory_IsTool00);
   tolua_function(tolua_S,"IsHelmet",tolua_AllToLua_ItemCategory_IsHelmet00);
   tolua_function(tolua_S,"IsChestPlate",tolua_AllToLua_ItemCategory_IsChestPlate00);
   tolua_function(tolua_S,"IsLeggings",tolua_AllToLua_ItemCategory_IsLeggings00);
   tolua_function(tolua_S,"IsBoots",tolua_AllToLua_ItemCategory_IsBoots00);
   tolua_function(tolua_S,"IsArmor",tolua_AllToLua_ItemCategory_IsArmor00);
  tolua_endmodule(tolua_S);
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
   tolua_variable(tolua_S,"Delimiter",tolua_get_cChatColor_Delimiter,NULL);
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
   tolua_variable(tolua_S,"Random",tolua_get_cChatColor_Random,NULL);
   tolua_variable(tolua_S,"Bold",tolua_get_cChatColor_Bold,NULL);
   tolua_variable(tolua_S,"Strikethrough",tolua_get_cChatColor_Strikethrough,NULL);
   tolua_variable(tolua_S,"Underlined",tolua_get_cChatColor_Underlined,NULL);
   tolua_variable(tolua_S,"Italic",tolua_get_cChatColor_Italic,NULL);
   tolua_variable(tolua_S,"Plain",tolua_get_cChatColor_Plain,NULL);
   tolua_function(tolua_S,"MakeColor",tolua_AllToLua_cChatColor_MakeColor00);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"cClientHandle","cClientHandle","",NULL);
  tolua_beginmodule(tolua_S,"cClientHandle");
   tolua_function(tolua_S,"GetPlayer",tolua_AllToLua_cClientHandle_GetPlayer00);
   tolua_function(tolua_S,"Kick",tolua_AllToLua_cClientHandle_Kick00);
   tolua_function(tolua_S,"SendBlockChange",tolua_AllToLua_cClientHandle_SendBlockChange00);
   tolua_function(tolua_S,"GetUsername",tolua_AllToLua_cClientHandle_GetUsername00);
   tolua_function(tolua_S,"SetUsername",tolua_AllToLua_cClientHandle_SetUsername00);
   tolua_function(tolua_S,"GetPing",tolua_AllToLua_cClientHandle_GetPing00);
   tolua_function(tolua_S,"SetViewDistance",tolua_AllToLua_cClientHandle_SetViewDistance00);
   tolua_function(tolua_S,"GetViewDistance",tolua_AllToLua_cClientHandle_GetViewDistance00);
   tolua_function(tolua_S,"GetUniqueID",tolua_AllToLua_cClientHandle_GetUniqueID00);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"TakeDamageInfo","TakeDamageInfo","",NULL);
  tolua_beginmodule(tolua_S,"TakeDamageInfo");
   tolua_variable(tolua_S,"DamageType",tolua_get_TakeDamageInfo_DamageType,tolua_set_TakeDamageInfo_DamageType);
   tolua_variable(tolua_S,"Attacker",tolua_get_TakeDamageInfo_Attacker_ptr,tolua_set_TakeDamageInfo_Attacker_ptr);
   tolua_variable(tolua_S,"RawDamage",tolua_get_TakeDamageInfo_RawDamage,tolua_set_TakeDamageInfo_RawDamage);
   tolua_variable(tolua_S,"FinalDamage",tolua_get_TakeDamageInfo_FinalDamage,tolua_set_TakeDamageInfo_FinalDamage);
   tolua_variable(tolua_S,"Knockback",tolua_get_TakeDamageInfo_Knockback,tolua_set_TakeDamageInfo_Knockback);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"cEntity","cEntity","",NULL);
  tolua_beginmodule(tolua_S,"cEntity");
   tolua_constant(tolua_S,"ENTITY_STATUS_HURT",cEntity::ENTITY_STATUS_HURT);
   tolua_constant(tolua_S,"ENTITY_STATUS_DEAD",cEntity::ENTITY_STATUS_DEAD);
   tolua_constant(tolua_S,"ENTITY_STATUS_WOLF_TAMING",cEntity::ENTITY_STATUS_WOLF_TAMING);
   tolua_constant(tolua_S,"ENTITY_STATUS_WOLF_TAMED",cEntity::ENTITY_STATUS_WOLF_TAMED);
   tolua_constant(tolua_S,"ENTITY_STATUS_WOLF_SHAKING",cEntity::ENTITY_STATUS_WOLF_SHAKING);
   tolua_constant(tolua_S,"ENTITY_STATUS_EATING_ACCEPTED",cEntity::ENTITY_STATUS_EATING_ACCEPTED);
   tolua_constant(tolua_S,"ENTITY_STATUS_SHEEP_EATING",cEntity::ENTITY_STATUS_SHEEP_EATING);
   tolua_constant(tolua_S,"FIRE_TICKS_PER_DAMAGE",cEntity::FIRE_TICKS_PER_DAMAGE);
   tolua_constant(tolua_S,"FIRE_DAMAGE",cEntity::FIRE_DAMAGE);
   tolua_constant(tolua_S,"LAVA_TICKS_PER_DAMAGE",cEntity::LAVA_TICKS_PER_DAMAGE);
   tolua_constant(tolua_S,"LAVA_DAMAGE",cEntity::LAVA_DAMAGE);
   tolua_constant(tolua_S,"BURN_TICKS_PER_DAMAGE",cEntity::BURN_TICKS_PER_DAMAGE);
   tolua_constant(tolua_S,"BURN_DAMAGE",cEntity::BURN_DAMAGE);
   tolua_constant(tolua_S,"BURN_TICKS",cEntity::BURN_TICKS);
   tolua_constant(tolua_S,"etEntity",cEntity::etEntity);
   tolua_constant(tolua_S,"etPlayer",cEntity::etPlayer);
   tolua_constant(tolua_S,"etPickup",cEntity::etPickup);
   tolua_constant(tolua_S,"etMonster",cEntity::etMonster);
   tolua_constant(tolua_S,"etMob",cEntity::etMob);
   tolua_constant(tolua_S,"etFallingBlock",cEntity::etFallingBlock);
   tolua_constant(tolua_S,"etMinecart",cEntity::etMinecart);
   tolua_constant(tolua_S,"etTNT",cEntity::etTNT);
   tolua_constant(tolua_S,"eEntityType_Entity",cEntity::eEntityType_Entity);
   tolua_constant(tolua_S,"eEntityType_Player",cEntity::eEntityType_Player);
   tolua_constant(tolua_S,"eEntityType_Pickup",cEntity::eEntityType_Pickup);
   tolua_constant(tolua_S,"eEntityType_Mob",cEntity::eEntityType_Mob);
   tolua_function(tolua_S,"GetEntityType",tolua_AllToLua_cEntity_GetEntityType00);
   tolua_function(tolua_S,"IsPlayer",tolua_AllToLua_cEntity_IsPlayer00);
   tolua_function(tolua_S,"IsPickup",tolua_AllToLua_cEntity_IsPickup00);
   tolua_function(tolua_S,"IsMob",tolua_AllToLua_cEntity_IsMob00);
   tolua_function(tolua_S,"IsMinecart",tolua_AllToLua_cEntity_IsMinecart00);
   tolua_function(tolua_S,"IsTNT",tolua_AllToLua_cEntity_IsTNT00);
   tolua_function(tolua_S,"IsA",tolua_AllToLua_cEntity_IsA00);
   tolua_function(tolua_S,"GetClass",tolua_AllToLua_cEntity_GetClass00);
   tolua_function(tolua_S,"GetClassStatic",tolua_AllToLua_cEntity_GetClassStatic00);
   tolua_function(tolua_S,"GetParentClass",tolua_AllToLua_cEntity_GetParentClass00);
   tolua_function(tolua_S,"GetWorld",tolua_AllToLua_cEntity_GetWorld00);
   tolua_function(tolua_S,"GetHeadYaw",tolua_AllToLua_cEntity_GetHeadYaw00);
   tolua_function(tolua_S,"GetHeight",tolua_AllToLua_cEntity_GetHeight00);
   tolua_function(tolua_S,"GetMass",tolua_AllToLua_cEntity_GetMass00);
   tolua_function(tolua_S,"GetPosition",tolua_AllToLua_cEntity_GetPosition00);
   tolua_function(tolua_S,"GetPosX",tolua_AllToLua_cEntity_GetPosX00);
   tolua_function(tolua_S,"GetPosY",tolua_AllToLua_cEntity_GetPosY00);
   tolua_function(tolua_S,"GetPosZ",tolua_AllToLua_cEntity_GetPosZ00);
   tolua_function(tolua_S,"GetRot",tolua_AllToLua_cEntity_GetRot00);
   tolua_function(tolua_S,"GetRotation",tolua_AllToLua_cEntity_GetRotation00);
   tolua_function(tolua_S,"GetPitch",tolua_AllToLua_cEntity_GetPitch00);
   tolua_function(tolua_S,"GetRoll",tolua_AllToLua_cEntity_GetRoll00);
   tolua_function(tolua_S,"GetLookVector",tolua_AllToLua_cEntity_GetLookVector00);
   tolua_function(tolua_S,"GetSpeed",tolua_AllToLua_cEntity_GetSpeed00);
   tolua_function(tolua_S,"GetSpeedX",tolua_AllToLua_cEntity_GetSpeedX00);
   tolua_function(tolua_S,"GetSpeedY",tolua_AllToLua_cEntity_GetSpeedY00);
   tolua_function(tolua_S,"GetSpeedZ",tolua_AllToLua_cEntity_GetSpeedZ00);
   tolua_function(tolua_S,"GetWidth",tolua_AllToLua_cEntity_GetWidth00);
   tolua_function(tolua_S,"GetChunkX",tolua_AllToLua_cEntity_GetChunkX00);
   tolua_function(tolua_S,"GetChunkZ",tolua_AllToLua_cEntity_GetChunkZ00);
   tolua_function(tolua_S,"SetHeadYaw",tolua_AllToLua_cEntity_SetHeadYaw00);
   tolua_function(tolua_S,"SetHeight",tolua_AllToLua_cEntity_SetHeight00);
   tolua_function(tolua_S,"SetMass",tolua_AllToLua_cEntity_SetMass00);
   tolua_function(tolua_S,"SetPosX",tolua_AllToLua_cEntity_SetPosX00);
   tolua_function(tolua_S,"SetPosY",tolua_AllToLua_cEntity_SetPosY00);
   tolua_function(tolua_S,"SetPosZ",tolua_AllToLua_cEntity_SetPosZ00);
   tolua_function(tolua_S,"SetPosition",tolua_AllToLua_cEntity_SetPosition00);
   tolua_function(tolua_S,"SetPosition",tolua_AllToLua_cEntity_SetPosition01);
   tolua_function(tolua_S,"SetRot",tolua_AllToLua_cEntity_SetRot00);
   tolua_function(tolua_S,"SetRotation",tolua_AllToLua_cEntity_SetRotation00);
   tolua_function(tolua_S,"SetPitch",tolua_AllToLua_cEntity_SetPitch00);
   tolua_function(tolua_S,"SetRoll",tolua_AllToLua_cEntity_SetRoll00);
   tolua_function(tolua_S,"SetSpeed",tolua_AllToLua_cEntity_SetSpeed00);
   tolua_function(tolua_S,"SetSpeed",tolua_AllToLua_cEntity_SetSpeed01);
   tolua_function(tolua_S,"SetSpeedX",tolua_AllToLua_cEntity_SetSpeedX00);
   tolua_function(tolua_S,"SetSpeedY",tolua_AllToLua_cEntity_SetSpeedY00);
   tolua_function(tolua_S,"SetSpeedZ",tolua_AllToLua_cEntity_SetSpeedZ00);
   tolua_function(tolua_S,"SetWidth",tolua_AllToLua_cEntity_SetWidth00);
   tolua_function(tolua_S,"AddPosX",tolua_AllToLua_cEntity_AddPosX00);
   tolua_function(tolua_S,"AddPosY",tolua_AllToLua_cEntity_AddPosY00);
   tolua_function(tolua_S,"AddPosZ",tolua_AllToLua_cEntity_AddPosZ00);
   tolua_function(tolua_S,"AddPosition",tolua_AllToLua_cEntity_AddPosition00);
   tolua_function(tolua_S,"AddPosition",tolua_AllToLua_cEntity_AddPosition01);
   tolua_function(tolua_S,"AddSpeed",tolua_AllToLua_cEntity_AddSpeed00);
   tolua_function(tolua_S,"AddSpeed",tolua_AllToLua_cEntity_AddSpeed01);
   tolua_function(tolua_S,"AddSpeedX",tolua_AllToLua_cEntity_AddSpeedX00);
   tolua_function(tolua_S,"AddSpeedY",tolua_AllToLua_cEntity_AddSpeedY00);
   tolua_function(tolua_S,"AddSpeedZ",tolua_AllToLua_cEntity_AddSpeedZ00);
   tolua_function(tolua_S,"GetUniqueID",tolua_AllToLua_cEntity_GetUniqueID00);
   tolua_function(tolua_S,"IsDestroyed",tolua_AllToLua_cEntity_IsDestroyed00);
   tolua_function(tolua_S,"Destroy",tolua_AllToLua_cEntity_Destroy00);
   tolua_function(tolua_S,"TakeDamage",tolua_AllToLua_cEntity_TakeDamage00);
   tolua_function(tolua_S,"TakeDamage",tolua_AllToLua_cEntity_TakeDamage01);
   tolua_function(tolua_S,"TakeDamage",tolua_AllToLua_cEntity_TakeDamage02);
   tolua_function(tolua_S,"GetRawDamageAgainst",tolua_AllToLua_cEntity_GetRawDamageAgainst00);
   tolua_function(tolua_S,"GetArmorCoverAgainst",tolua_AllToLua_cEntity_GetArmorCoverAgainst00);
   tolua_function(tolua_S,"GetKnockbackAmountAgainst",tolua_AllToLua_cEntity_GetKnockbackAmountAgainst00);
   tolua_function(tolua_S,"GetEquippedWeapon",tolua_AllToLua_cEntity_GetEquippedWeapon00);
   tolua_function(tolua_S,"GetEquippedHelmet",tolua_AllToLua_cEntity_GetEquippedHelmet00);
   tolua_function(tolua_S,"GetEquippedChestplate",tolua_AllToLua_cEntity_GetEquippedChestplate00);
   tolua_function(tolua_S,"GetEquippedLeggings",tolua_AllToLua_cEntity_GetEquippedLeggings00);
   tolua_function(tolua_S,"GetEquippedBoots",tolua_AllToLua_cEntity_GetEquippedBoots00);
   tolua_function(tolua_S,"KilledBy",tolua_AllToLua_cEntity_KilledBy00);
   tolua_function(tolua_S,"Heal",tolua_AllToLua_cEntity_Heal00);
   tolua_function(tolua_S,"GetHealth",tolua_AllToLua_cEntity_GetHealth00);
   tolua_function(tolua_S,"SetHealth",tolua_AllToLua_cEntity_SetHealth00);
   tolua_function(tolua_S,"SetMaxHealth",tolua_AllToLua_cEntity_SetMaxHealth00);
   tolua_function(tolua_S,"GetMaxHealth",tolua_AllToLua_cEntity_GetMaxHealth00);
   tolua_function(tolua_S,"StartBurning",tolua_AllToLua_cEntity_StartBurning00);
   tolua_function(tolua_S,"StopBurning",tolua_AllToLua_cEntity_StopBurning00);
   tolua_function(tolua_S,"TeleportToEntity",tolua_AllToLua_cEntity_TeleportToEntity00);
   tolua_function(tolua_S,"TeleportToCoords",tolua_AllToLua_cEntity_TeleportToCoords00);
   tolua_function(tolua_S,"IsOnFire",tolua_AllToLua_cEntity_IsOnFire00);
   tolua_function(tolua_S,"IsCrouched",tolua_AllToLua_cEntity_IsCrouched00);
   tolua_function(tolua_S,"IsRiding",tolua_AllToLua_cEntity_IsRiding00);
   tolua_function(tolua_S,"IsSprinting",tolua_AllToLua_cEntity_IsSprinting00);
   tolua_function(tolua_S,"IsRclking",tolua_AllToLua_cEntity_IsRclking00);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"Lua__cEntity","Lua__cEntity","cEntity",NULL);
  tolua_beginmodule(tolua_S,"Lua__cEntity");
   tolua_function(tolua_S,"tolua__set_instance",tolua_AllToLua_Lua__cEntity_tolua__set_instance00);
   tolua_function(tolua_S,"cEntity__IsA",tolua_AllToLua_Lua__cEntity_cEntity__IsA00);
   tolua_function(tolua_S,"cEntity__GetClass",tolua_AllToLua_Lua__cEntity_cEntity__GetClass00);
   tolua_function(tolua_S,"cEntity__GetParentClass",tolua_AllToLua_Lua__cEntity_cEntity__GetParentClass00);
   tolua_function(tolua_S,"cEntity__GetRawDamageAgainst",tolua_AllToLua_Lua__cEntity_cEntity__GetRawDamageAgainst00);
   tolua_function(tolua_S,"cEntity__GetArmorCoverAgainst",tolua_AllToLua_Lua__cEntity_cEntity__GetArmorCoverAgainst00);
   tolua_function(tolua_S,"cEntity__GetKnockbackAmountAgainst",tolua_AllToLua_Lua__cEntity_cEntity__GetKnockbackAmountAgainst00);
   tolua_function(tolua_S,"cEntity__GetEquippedWeapon",tolua_AllToLua_Lua__cEntity_cEntity__GetEquippedWeapon00);
   tolua_function(tolua_S,"cEntity__GetEquippedHelmet",tolua_AllToLua_Lua__cEntity_cEntity__GetEquippedHelmet00);
   tolua_function(tolua_S,"cEntity__GetEquippedChestplate",tolua_AllToLua_Lua__cEntity_cEntity__GetEquippedChestplate00);
   tolua_function(tolua_S,"cEntity__GetEquippedLeggings",tolua_AllToLua_Lua__cEntity_cEntity__GetEquippedLeggings00);
   tolua_function(tolua_S,"cEntity__GetEquippedBoots",tolua_AllToLua_Lua__cEntity_cEntity__GetEquippedBoots00);
   tolua_function(tolua_S,"cEntity__KilledBy",tolua_AllToLua_Lua__cEntity_cEntity__KilledBy00);
   tolua_function(tolua_S,"cEntity__TeleportToEntity",tolua_AllToLua_Lua__cEntity_cEntity__TeleportToEntity00);
   tolua_function(tolua_S,"cEntity__TeleportToCoords",tolua_AllToLua_Lua__cEntity_cEntity__TeleportToCoords00);
   tolua_function(tolua_S,"cEntity__IsOnFire",tolua_AllToLua_Lua__cEntity_cEntity__IsOnFire00);
   tolua_function(tolua_S,"cEntity__IsCrouched",tolua_AllToLua_Lua__cEntity_cEntity__IsCrouched00);
   tolua_function(tolua_S,"cEntity__IsRiding",tolua_AllToLua_Lua__cEntity_cEntity__IsRiding00);
   tolua_function(tolua_S,"cEntity__IsSprinting",tolua_AllToLua_Lua__cEntity_cEntity__IsSprinting00);
   tolua_function(tolua_S,"cEntity__IsRclking",tolua_AllToLua_Lua__cEntity_cEntity__IsRclking00);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"cPawn","cPawn","cEntity",NULL);
  tolua_beginmodule(tolua_S,"cPawn");
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"cPlayer","cPlayer","cPawn",NULL);
  tolua_beginmodule(tolua_S,"cPlayer");
   tolua_constant(tolua_S,"MAX_HEALTH",cPlayer::MAX_HEALTH);
   tolua_constant(tolua_S,"MAX_FOOD_LEVEL",cPlayer::MAX_FOOD_LEVEL);
   tolua_constant(tolua_S,"EATING_TICKS",cPlayer::EATING_TICKS);
   tolua_function(tolua_S,"Initialize",tolua_AllToLua_cPlayer_Initialize00);
   tolua_function(tolua_S,"GetEyeHeight",tolua_AllToLua_cPlayer_GetEyeHeight00);
   tolua_function(tolua_S,"GetEyePosition",tolua_AllToLua_cPlayer_GetEyePosition00);
   tolua_function(tolua_S,"IsOnGround",tolua_AllToLua_cPlayer_IsOnGround00);
   tolua_function(tolua_S,"GetStance",tolua_AllToLua_cPlayer_GetStance00);
   tolua_function(tolua_S,"GetInventory",tolua_AllToLua_cPlayer_GetInventory00);
   tolua_function(tolua_S,"GetEquippedItem",tolua_AllToLua_cPlayer_GetEquippedItem00);
   tolua_function(tolua_S,"GetGameMode",tolua_AllToLua_cPlayer_GetGameMode00);
   tolua_function(tolua_S,"SetGameMode",tolua_AllToLua_cPlayer_SetGameMode00);
   tolua_function(tolua_S,"IsGameModeCreative",tolua_AllToLua_cPlayer_IsGameModeCreative00);
   tolua_function(tolua_S,"IsGameModeSurvival",tolua_AllToLua_cPlayer_IsGameModeSurvival00);
   tolua_function(tolua_S,"IsGameModeAdventure",tolua_AllToLua_cPlayer_IsGameModeAdventure00);
   tolua_function(tolua_S,"GetIP",tolua_AllToLua_cPlayer_GetIP00);
   tolua_function(tolua_S,"MoveTo",tolua_AllToLua_cPlayer_MoveTo00);
   tolua_function(tolua_S,"GetWindow",tolua_AllToLua_cPlayer_GetWindow00);
   tolua_function(tolua_S,"CloseWindow",tolua_AllToLua_cPlayer_CloseWindow00);
   tolua_function(tolua_S,"CloseWindowIfID",tolua_AllToLua_cPlayer_CloseWindowIfID00);
   tolua_function(tolua_S,"GetClientHandle",tolua_AllToLua_cPlayer_GetClientHandle00);
   tolua_function(tolua_S,"SendMessage",tolua_AllToLua_cPlayer_SendMessage00);
   tolua_function(tolua_S,"GetName",tolua_AllToLua_cPlayer_GetName00);
   tolua_function(tolua_S,"SetName",tolua_AllToLua_cPlayer_SetName00);
   tolua_function(tolua_S,"AddToGroup",tolua_AllToLua_cPlayer_AddToGroup00);
   tolua_function(tolua_S,"RemoveFromGroup",tolua_AllToLua_cPlayer_RemoveFromGroup00);
   tolua_function(tolua_S,"CanUseCommand",tolua_AllToLua_cPlayer_CanUseCommand00);
   tolua_function(tolua_S,"HasPermission",tolua_AllToLua_cPlayer_HasPermission00);
   tolua_function(tolua_S,"IsInGroup",tolua_AllToLua_cPlayer_IsInGroup00);
   tolua_function(tolua_S,"GetColor",tolua_AllToLua_cPlayer_GetColor00);
   tolua_function(tolua_S,"TossItem",tolua_AllToLua_cPlayer_TossItem00);
   tolua_function(tolua_S,"Heal",tolua_AllToLua_cPlayer_Heal00);
   tolua_function(tolua_S,"GetFoodLevel",tolua_AllToLua_cPlayer_GetFoodLevel00);
   tolua_function(tolua_S,"GetFoodSaturationLevel",tolua_AllToLua_cPlayer_GetFoodSaturationLevel00);
   tolua_function(tolua_S,"GetFoodTickTimer",tolua_AllToLua_cPlayer_GetFoodTickTimer00);
   tolua_function(tolua_S,"GetFoodExhaustionLevel",tolua_AllToLua_cPlayer_GetFoodExhaustionLevel00);
   tolua_function(tolua_S,"GetFoodPoisonedTicksRemaining",tolua_AllToLua_cPlayer_GetFoodPoisonedTicksRemaining00);
   tolua_function(tolua_S,"IsSatiated",tolua_AllToLua_cPlayer_IsSatiated00);
   tolua_function(tolua_S,"SetFoodLevel",tolua_AllToLua_cPlayer_SetFoodLevel00);
   tolua_function(tolua_S,"SetFoodSaturationLevel",tolua_AllToLua_cPlayer_SetFoodSaturationLevel00);
   tolua_function(tolua_S,"SetFoodTickTimer",tolua_AllToLua_cPlayer_SetFoodTickTimer00);
   tolua_function(tolua_S,"SetFoodExhaustionLevel",tolua_AllToLua_cPlayer_SetFoodExhaustionLevel00);
   tolua_function(tolua_S,"SetFoodPoisonedTicksRemaining",tolua_AllToLua_cPlayer_SetFoodPoisonedTicksRemaining00);
   tolua_function(tolua_S,"Feed",tolua_AllToLua_cPlayer_Feed00);
   tolua_function(tolua_S,"AddFoodExhaustion",tolua_AllToLua_cPlayer_AddFoodExhaustion00);
   tolua_function(tolua_S,"FoodPoison",tolua_AllToLua_cPlayer_FoodPoison00);
   tolua_function(tolua_S,"IsEating",tolua_AllToLua_cPlayer_IsEating00);
   tolua_function(tolua_S,"Respawn",tolua_AllToLua_cPlayer_Respawn00);
   tolua_function(tolua_S,"SetVisible",tolua_AllToLua_cPlayer_SetVisible00);
   tolua_function(tolua_S,"IsVisible",tolua_AllToLua_cPlayer_IsVisible00);
   tolua_function(tolua_S,"MoveToWorld",tolua_AllToLua_cPlayer_MoveToWorld00);
   tolua_function(tolua_S,"LoadPermissionsFromDisk",tolua_AllToLua_cPlayer_LoadPermissionsFromDisk00);
   tolua_function(tolua_S,"GetMaxSpeed",tolua_AllToLua_cPlayer_GetMaxSpeed00);
   tolua_function(tolua_S,"GetNormalMaxSpeed",tolua_AllToLua_cPlayer_GetNormalMaxSpeed00);
   tolua_function(tolua_S,"GetSprintingMaxSpeed",tolua_AllToLua_cPlayer_GetSprintingMaxSpeed00);
   tolua_function(tolua_S,"SetNormalMaxSpeed",tolua_AllToLua_cPlayer_SetNormalMaxSpeed00);
   tolua_function(tolua_S,"SetSprintingMaxSpeed",tolua_AllToLua_cPlayer_SetSprintingMaxSpeed00);
   tolua_function(tolua_S,"SetCrouch",tolua_AllToLua_cPlayer_SetCrouch00);
   tolua_function(tolua_S,"SetSprint",tolua_AllToLua_cPlayer_SetSprint00);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"Lua__cPlayer","Lua__cPlayer","cPlayer",NULL);
  tolua_beginmodule(tolua_S,"Lua__cPlayer");
   tolua_function(tolua_S,"tolua__set_instance",tolua_AllToLua_Lua__cPlayer_tolua__set_instance00);
   tolua_function(tolua_S,"cPlayer__Initialize",tolua_AllToLua_Lua__cPlayer_cPlayer__Initialize00);
   tolua_function(tolua_S,"cPlayer__MoveTo",tolua_AllToLua_Lua__cPlayer_cPlayer__MoveTo00);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"cPluginManager","cPluginManager","",NULL);
  tolua_beginmodule(tolua_S,"cPluginManager");
   tolua_constant(tolua_S,"HOOK_BLOCK_TO_PICKUPS",cPluginManager::HOOK_BLOCK_TO_PICKUPS);
   tolua_constant(tolua_S,"HOOK_CHAT",cPluginManager::HOOK_CHAT);
   tolua_constant(tolua_S,"HOOK_CHUNK_AVAILABLE",cPluginManager::HOOK_CHUNK_AVAILABLE);
   tolua_constant(tolua_S,"HOOK_CHUNK_GENERATED",cPluginManager::HOOK_CHUNK_GENERATED);
   tolua_constant(tolua_S,"HOOK_CHUNK_GENERATING",cPluginManager::HOOK_CHUNK_GENERATING);
   tolua_constant(tolua_S,"HOOK_CHUNK_UNLOADED",cPluginManager::HOOK_CHUNK_UNLOADED);
   tolua_constant(tolua_S,"HOOK_CHUNK_UNLOADING",cPluginManager::HOOK_CHUNK_UNLOADING);
   tolua_constant(tolua_S,"HOOK_COLLECTING_PICKUP",cPluginManager::HOOK_COLLECTING_PICKUP);
   tolua_constant(tolua_S,"HOOK_CRAFTING_NO_RECIPE",cPluginManager::HOOK_CRAFTING_NO_RECIPE);
   tolua_constant(tolua_S,"HOOK_DISCONNECT",cPluginManager::HOOK_DISCONNECT);
   tolua_constant(tolua_S,"HOOK_EXECUTE_COMMAND",cPluginManager::HOOK_EXECUTE_COMMAND);
   tolua_constant(tolua_S,"HOOK_HANDSHAKE",cPluginManager::HOOK_HANDSHAKE);
   tolua_constant(tolua_S,"HOOK_KILLING",cPluginManager::HOOK_KILLING);
   tolua_constant(tolua_S,"HOOK_LOGIN",cPluginManager::HOOK_LOGIN);
   tolua_constant(tolua_S,"HOOK_PLAYER_BREAKING_BLOCK",cPluginManager::HOOK_PLAYER_BREAKING_BLOCK);
   tolua_constant(tolua_S,"HOOK_PLAYER_BROKEN_BLOCK",cPluginManager::HOOK_PLAYER_BROKEN_BLOCK);
   tolua_constant(tolua_S,"HOOK_PLAYER_EATING",cPluginManager::HOOK_PLAYER_EATING);
   tolua_constant(tolua_S,"HOOK_PLAYER_JOINED",cPluginManager::HOOK_PLAYER_JOINED);
   tolua_constant(tolua_S,"HOOK_PLAYER_LEFT_CLICK",cPluginManager::HOOK_PLAYER_LEFT_CLICK);
   tolua_constant(tolua_S,"HOOK_PLAYER_MOVING",cPluginManager::HOOK_PLAYER_MOVING);
   tolua_constant(tolua_S,"HOOK_PLAYER_PLACED_BLOCK",cPluginManager::HOOK_PLAYER_PLACED_BLOCK);
   tolua_constant(tolua_S,"HOOK_PLAYER_PLACING_BLOCK",cPluginManager::HOOK_PLAYER_PLACING_BLOCK);
   tolua_constant(tolua_S,"HOOK_PLAYER_RIGHT_CLICK",cPluginManager::HOOK_PLAYER_RIGHT_CLICK);
   tolua_constant(tolua_S,"HOOK_PLAYER_RIGHT_CLICKING_ENTITY",cPluginManager::HOOK_PLAYER_RIGHT_CLICKING_ENTITY);
   tolua_constant(tolua_S,"HOOK_PLAYER_SHOOTING",cPluginManager::HOOK_PLAYER_SHOOTING);
   tolua_constant(tolua_S,"HOOK_PLAYER_SPAWNED",cPluginManager::HOOK_PLAYER_SPAWNED);
   tolua_constant(tolua_S,"HOOK_PLAYER_TOSSING_ITEM",cPluginManager::HOOK_PLAYER_TOSSING_ITEM);
   tolua_constant(tolua_S,"HOOK_PLAYER_USED_BLOCK",cPluginManager::HOOK_PLAYER_USED_BLOCK);
   tolua_constant(tolua_S,"HOOK_PLAYER_USED_ITEM",cPluginManager::HOOK_PLAYER_USED_ITEM);
   tolua_constant(tolua_S,"HOOK_PLAYER_USING_BLOCK",cPluginManager::HOOK_PLAYER_USING_BLOCK);
   tolua_constant(tolua_S,"HOOK_PLAYER_USING_ITEM",cPluginManager::HOOK_PLAYER_USING_ITEM);
   tolua_constant(tolua_S,"HOOK_POST_CRAFTING",cPluginManager::HOOK_POST_CRAFTING);
   tolua_constant(tolua_S,"HOOK_PRE_CRAFTING",cPluginManager::HOOK_PRE_CRAFTING);
   tolua_constant(tolua_S,"HOOK_TAKE_DAMAGE",cPluginManager::HOOK_TAKE_DAMAGE);
   tolua_constant(tolua_S,"HOOK_TICK",cPluginManager::HOOK_TICK);
   tolua_constant(tolua_S,"HOOK_UPDATED_SIGN",cPluginManager::HOOK_UPDATED_SIGN);
   tolua_constant(tolua_S,"HOOK_UPDATING_SIGN",cPluginManager::HOOK_UPDATING_SIGN);
   tolua_constant(tolua_S,"HOOK_WEATHER_CHANGED",cPluginManager::HOOK_WEATHER_CHANGED);
   tolua_constant(tolua_S,"HOOK_WEATHER_CHANGING",cPluginManager::HOOK_WEATHER_CHANGING);
   tolua_function(tolua_S,"Get",tolua_AllToLua_cPluginManager_Get00);
   tolua_function(tolua_S,"GetPlugin",tolua_AllToLua_cPluginManager_GetPlugin00);
   tolua_function(tolua_S,"FindPlugins",tolua_AllToLua_cPluginManager_FindPlugins00);
   tolua_function(tolua_S,"ReloadPlugins",tolua_AllToLua_cPluginManager_ReloadPlugins00);
   tolua_function(tolua_S,"AddHook",tolua_AllToLua_cPluginManager_AddHook00);
   tolua_function(tolua_S,"GetNumPlugins",tolua_AllToLua_cPluginManager_GetNumPlugins00);
   tolua_function(tolua_S,"DisablePlugin",tolua_AllToLua_cPluginManager_DisablePlugin00);
   tolua_function(tolua_S,"LoadPlugin",tolua_AllToLua_cPluginManager_LoadPlugin00);
   tolua_function(tolua_S,"IsCommandBound",tolua_AllToLua_cPluginManager_IsCommandBound00);
   tolua_function(tolua_S,"GetCommandPermission",tolua_AllToLua_cPluginManager_GetCommandPermission00);
   tolua_function(tolua_S,"ExecuteCommand",tolua_AllToLua_cPluginManager_ExecuteCommand00);
   tolua_function(tolua_S,"ForceExecuteCommand",tolua_AllToLua_cPluginManager_ForceExecuteCommand00);
   tolua_function(tolua_S,"IsConsoleCommandBound",tolua_AllToLua_cPluginManager_IsConsoleCommandBound00);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"cPlugin","cPlugin","",NULL);
  tolua_beginmodule(tolua_S,"cPlugin");
   tolua_function(tolua_S,"GetName",tolua_AllToLua_cPlugin_GetName00);
   tolua_function(tolua_S,"SetName",tolua_AllToLua_cPlugin_SetName00);
   tolua_function(tolua_S,"GetVersion",tolua_AllToLua_cPlugin_GetVersion00);
   tolua_function(tolua_S,"SetVersion",tolua_AllToLua_cPlugin_SetVersion00);
   tolua_function(tolua_S,"GetDirectory",tolua_AllToLua_cPlugin_GetDirectory00);
   tolua_function(tolua_S,"GetLocalDirectory",tolua_AllToLua_cPlugin_GetLocalDirectory00);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"cPlugin_NewLua","cPlugin_NewLua","cPlugin",NULL);
  tolua_beginmodule(tolua_S,"cPlugin_NewLua");
   tolua_variable(tolua_S,"__cWebPlugin__",tolua_get_cPlugin_NewLua___cWebPlugin__,NULL);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"cServer","cServer","",NULL);
  tolua_beginmodule(tolua_S,"cServer");
   tolua_function(tolua_S,"BroadcastChat",tolua_AllToLua_cServer_BroadcastChat00);
   tolua_function(tolua_S,"SendMessage",tolua_AllToLua_cServer_SendMessage00);
   tolua_function(tolua_S,"GetServerID",tolua_AllToLua_cServer_GetServerID00);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"cWorld","cWorld","",NULL);
  tolua_beginmodule(tolua_S,"cWorld");
   tolua_function(tolua_S,"GetClassStatic",tolua_AllToLua_cWorld_GetClassStatic00);
   tolua_function(tolua_S,"GetTime",tolua_AllToLua_cWorld_GetTime00);
   tolua_function(tolua_S,"GetTicksUntilWeatherChange",tolua_AllToLua_cWorld_GetTicksUntilWeatherChange00);
   tolua_function(tolua_S,"GetWorldAge",tolua_AllToLua_cWorld_GetWorldAge00);
   tolua_function(tolua_S,"GetTimeOfDay",tolua_AllToLua_cWorld_GetTimeOfDay00);
   tolua_function(tolua_S,"SetTicksUntilWeatherChange",tolua_AllToLua_cWorld_SetTicksUntilWeatherChange00);
   tolua_function(tolua_S,"SetTimeOfDay",tolua_AllToLua_cWorld_SetTimeOfDay00);
   tolua_function(tolua_S,"SetWorldTime",tolua_AllToLua_cWorld_SetWorldTime00);
   tolua_function(tolua_S,"GetGameMode",tolua_AllToLua_cWorld_GetGameMode00);
   tolua_function(tolua_S,"IsGameModeCreative",tolua_AllToLua_cWorld_IsGameModeCreative00);
   tolua_function(tolua_S,"IsGameModeSurvival",tolua_AllToLua_cWorld_IsGameModeSurvival00);
   tolua_function(tolua_S,"IsGameModeAdventure",tolua_AllToLua_cWorld_IsGameModeAdventure00);
   tolua_function(tolua_S,"IsPVPEnabled",tolua_AllToLua_cWorld_IsPVPEnabled00);
   tolua_function(tolua_S,"IsDeepSnowEnabled",tolua_AllToLua_cWorld_IsDeepSnowEnabled00);
   tolua_function(tolua_S,"GetDimension",tolua_AllToLua_cWorld_GetDimension00);
   tolua_function(tolua_S,"GetHeight",tolua_AllToLua_cWorld_GetHeight00);
   tolua_function(tolua_S,"UnloadUnusedChunks",tolua_AllToLua_cWorld_UnloadUnusedChunks00);
   tolua_function(tolua_S,"GetMaxPlayers",tolua_AllToLua_cWorld_GetMaxPlayers00);
   tolua_function(tolua_S,"SetMaxPlayers",tolua_AllToLua_cWorld_SetMaxPlayers00);
   tolua_function(tolua_S,"GetNumPlayers",tolua_AllToLua_cWorld_GetNumPlayers00);
   tolua_function(tolua_S,"RegenerateChunk",tolua_AllToLua_cWorld_RegenerateChunk00);
   tolua_function(tolua_S,"GenerateChunk",tolua_AllToLua_cWorld_GenerateChunk00);
   tolua_function(tolua_S,"SetBlock",tolua_AllToLua_cWorld_SetBlock00);
   tolua_function(tolua_S,"FastSetBlock",tolua_AllToLua_cWorld_FastSetBlock00);
   tolua_function(tolua_S,"GetBlock",tolua_AllToLua_cWorld_GetBlock00);
   tolua_function(tolua_S,"GetBlockMeta",tolua_AllToLua_cWorld_GetBlockMeta00);
   tolua_function(tolua_S,"SetBlockMeta",tolua_AllToLua_cWorld_SetBlockMeta00);
   tolua_function(tolua_S,"GetBlockSkyLight",tolua_AllToLua_cWorld_GetBlockSkyLight00);
   tolua_function(tolua_S,"GetBlockBlockLight",tolua_AllToLua_cWorld_GetBlockBlockLight00);
   tolua_function(tolua_S,"GetBlockTypeMeta",tolua_AllToLua_cWorld_GetBlockTypeMeta00);
   tolua_function(tolua_S,"GetBlockInfo",tolua_AllToLua_cWorld_GetBlockInfo00);
   tolua_function(tolua_S,"FastSetBlock",tolua_AllToLua_cWorld_FastSetBlock01);
   tolua_function(tolua_S,"GetBlock",tolua_AllToLua_cWorld_GetBlock01);
   tolua_function(tolua_S,"GetBlockMeta",tolua_AllToLua_cWorld_GetBlockMeta01);
   tolua_function(tolua_S,"SetBlockMeta",tolua_AllToLua_cWorld_SetBlockMeta01);
   tolua_function(tolua_S,"SpawnItemPickups",tolua_AllToLua_cWorld_SpawnItemPickups00);
   tolua_function(tolua_S,"SpawnItemPickups",tolua_AllToLua_cWorld_SpawnItemPickups01);
   tolua_function(tolua_S,"SpawnPrimedTNT",tolua_AllToLua_cWorld_SpawnPrimedTNT00);
   tolua_function(tolua_S,"DigBlock",tolua_AllToLua_cWorld_DigBlock00);
   tolua_function(tolua_S,"SendBlockTo",tolua_AllToLua_cWorld_SendBlockTo00);
   tolua_function(tolua_S,"GetSpawnX",tolua_AllToLua_cWorld_GetSpawnX00);
   tolua_function(tolua_S,"GetSpawnY",tolua_AllToLua_cWorld_GetSpawnY00);
   tolua_function(tolua_S,"GetSpawnZ",tolua_AllToLua_cWorld_GetSpawnZ00);
   tolua_function(tolua_S,"WakeUpSimulators",tolua_AllToLua_cWorld_WakeUpSimulators00);
   tolua_function(tolua_S,"WakeUpSimulatorsInArea",tolua_AllToLua_cWorld_WakeUpSimulatorsInArea00);
   tolua_function(tolua_S,"GetSignLines",tolua_AllToLua_cWorld_GetSignLines00);
   tolua_function(tolua_S,"GrowTree",tolua_AllToLua_cWorld_GrowTree00);
   tolua_function(tolua_S,"GrowTreeFromSapling",tolua_AllToLua_cWorld_GrowTreeFromSapling00);
   tolua_function(tolua_S,"GrowTreeByBiome",tolua_AllToLua_cWorld_GrowTreeByBiome00);
   tolua_function(tolua_S,"GrowRipePlant",tolua_AllToLua_cWorld_GrowRipePlant00);
   tolua_function(tolua_S,"GrowCactus",tolua_AllToLua_cWorld_GrowCactus00);
   tolua_function(tolua_S,"GrowMelonPumpkin",tolua_AllToLua_cWorld_GrowMelonPumpkin00);
   tolua_function(tolua_S,"GrowSugarcane",tolua_AllToLua_cWorld_GrowSugarcane00);
   tolua_function(tolua_S,"GetBiomeAt",tolua_AllToLua_cWorld_GetBiomeAt00);
   tolua_function(tolua_S,"GetName",tolua_AllToLua_cWorld_GetName00);
   tolua_function(tolua_S,"SaveAllChunks",tolua_AllToLua_cWorld_SaveAllChunks00);
   tolua_function(tolua_S,"GetNumChunks",tolua_AllToLua_cWorld_GetNumChunks00);
   tolua_function(tolua_S,"GetGeneratorQueueLength",tolua_AllToLua_cWorld_GetGeneratorQueueLength00);
   tolua_function(tolua_S,"GetLightingQueueLength",tolua_AllToLua_cWorld_GetLightingQueueLength00);
   tolua_function(tolua_S,"GetStorageLoadQueueLength",tolua_AllToLua_cWorld_GetStorageLoadQueueLength00);
   tolua_function(tolua_S,"GetStorageSaveQueueLength",tolua_AllToLua_cWorld_GetStorageSaveQueueLength00);
   tolua_function(tolua_S,"QueueBlockForTick",tolua_AllToLua_cWorld_QueueBlockForTick00);
   tolua_function(tolua_S,"CastThunderbolt",tolua_AllToLua_cWorld_CastThunderbolt00);
   tolua_function(tolua_S,"SetWeather",tolua_AllToLua_cWorld_SetWeather00);
   tolua_function(tolua_S,"ChangeWeather",tolua_AllToLua_cWorld_ChangeWeather00);
   tolua_function(tolua_S,"GetWeather",tolua_AllToLua_cWorld_GetWeather00);
   tolua_function(tolua_S,"SetNextBlockTick",tolua_AllToLua_cWorld_SetNextBlockTick00);
   tolua_function(tolua_S,"GetMaxSugarcaneHeight",tolua_AllToLua_cWorld_GetMaxSugarcaneHeight00);
   tolua_function(tolua_S,"GetMaxCactusHeight",tolua_AllToLua_cWorld_GetMaxCactusHeight00);
   tolua_function(tolua_S,"IsBlockDirectlyWatered",tolua_AllToLua_cWorld_IsBlockDirectlyWatered00);
   tolua_function(tolua_S,"SpawnMob",tolua_AllToLua_cWorld_SpawnMob00);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"cInventory","cInventory","cItemGrid::cListener",NULL);
  tolua_beginmodule(tolua_S,"cInventory");
   tolua_constant(tolua_S,"invArmorCount",cInventory::invArmorCount);
   tolua_constant(tolua_S,"invInventoryCount",cInventory::invInventoryCount);
   tolua_constant(tolua_S,"invHotbarCount",cInventory::invHotbarCount);
   tolua_constant(tolua_S,"invArmorOffset",cInventory::invArmorOffset);
   tolua_constant(tolua_S,"invInventoryOffset",cInventory::invInventoryOffset);
   tolua_constant(tolua_S,"invHotbarOffset",cInventory::invHotbarOffset);
   tolua_constant(tolua_S,"invNumSlots",cInventory::invNumSlots);
   tolua_function(tolua_S,"Clear",tolua_AllToLua_cInventory_Clear00);
   tolua_function(tolua_S,"HowManyCanFit",tolua_AllToLua_cInventory_HowManyCanFit00);
   tolua_function(tolua_S,"HowManyCanFit",tolua_AllToLua_cInventory_HowManyCanFit01);
   tolua_function(tolua_S,"AddItem",tolua_AllToLua_cInventory_AddItem00);
   tolua_function(tolua_S,"AddItems",tolua_AllToLua_cInventory_AddItems00);
   tolua_function(tolua_S,"RemoveOneEquippedItem",tolua_AllToLua_cInventory_RemoveOneEquippedItem00);
   tolua_function(tolua_S,"HowManyItems",tolua_AllToLua_cInventory_HowManyItems00);
   tolua_function(tolua_S,"HasItems",tolua_AllToLua_cInventory_HasItems00);
   tolua_function(tolua_S,"GetArmorGrid",tolua_AllToLua_cInventory_GetArmorGrid00);
   tolua_function(tolua_S,"GetInventoryGrid",tolua_AllToLua_cInventory_GetInventoryGrid00);
   tolua_function(tolua_S,"GetHotbarGrid",tolua_AllToLua_cInventory_GetHotbarGrid00);
   tolua_function(tolua_S,"GetOwner",tolua_AllToLua_cInventory_GetOwner00);
   tolua_function(tolua_S,"CopyToItems",tolua_AllToLua_cInventory_CopyToItems00);
   tolua_function(tolua_S,"GetSlot",tolua_AllToLua_cInventory_GetSlot00);
   tolua_function(tolua_S,"GetArmorSlot",tolua_AllToLua_cInventory_GetArmorSlot00);
   tolua_function(tolua_S,"GetInventorySlot",tolua_AllToLua_cInventory_GetInventorySlot00);
   tolua_function(tolua_S,"GetHotbarSlot",tolua_AllToLua_cInventory_GetHotbarSlot00);
   tolua_function(tolua_S,"GetEquippedItem",tolua_AllToLua_cInventory_GetEquippedItem00);
   tolua_function(tolua_S,"SetSlot",tolua_AllToLua_cInventory_SetSlot00);
   tolua_function(tolua_S,"SetArmorSlot",tolua_AllToLua_cInventory_SetArmorSlot00);
   tolua_function(tolua_S,"SetInventorySlot",tolua_AllToLua_cInventory_SetInventorySlot00);
   tolua_function(tolua_S,"SetHotbarSlot",tolua_AllToLua_cInventory_SetHotbarSlot00);
   tolua_function(tolua_S,"SetEquippedSlotNum",tolua_AllToLua_cInventory_SetEquippedSlotNum00);
   tolua_function(tolua_S,"GetEquippedSlotNum",tolua_AllToLua_cInventory_GetEquippedSlotNum00);
   tolua_function(tolua_S,"ChangeSlotCount",tolua_AllToLua_cInventory_ChangeSlotCount00);
   tolua_function(tolua_S,"DamageItem",tolua_AllToLua_cInventory_DamageItem00);
   tolua_function(tolua_S,"DamageEquippedItem",tolua_AllToLua_cInventory_DamageEquippedItem00);
   tolua_function(tolua_S,"GetEquippedHelmet",tolua_AllToLua_cInventory_GetEquippedHelmet00);
   tolua_function(tolua_S,"GetEquippedChestplate",tolua_AllToLua_cInventory_GetEquippedChestplate00);
   tolua_function(tolua_S,"GetEquippedLeggings",tolua_AllToLua_cInventory_GetEquippedLeggings00);
   tolua_function(tolua_S,"GetEquippedBoots",tolua_AllToLua_cInventory_GetEquippedBoots00);
  tolua_endmodule(tolua_S);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"cEnchantments","cEnchantments","",tolua_collect_cEnchantments);
  #else
  tolua_cclass(tolua_S,"cEnchantments","cEnchantments","",NULL);
  #endif
  tolua_beginmodule(tolua_S,"cEnchantments");
   tolua_constant(tolua_S,"enchProtection",cEnchantments::enchProtection);
   tolua_constant(tolua_S,"enchFireProtection",cEnchantments::enchFireProtection);
   tolua_constant(tolua_S,"enchFeatherFalling",cEnchantments::enchFeatherFalling);
   tolua_constant(tolua_S,"enchBlastProtection",cEnchantments::enchBlastProtection);
   tolua_constant(tolua_S,"enchProjectileProtection",cEnchantments::enchProjectileProtection);
   tolua_constant(tolua_S,"enchRespiration",cEnchantments::enchRespiration);
   tolua_constant(tolua_S,"enchAquaAffinity",cEnchantments::enchAquaAffinity);
   tolua_constant(tolua_S,"enchThorns",cEnchantments::enchThorns);
   tolua_constant(tolua_S,"enchSharpness",cEnchantments::enchSharpness);
   tolua_constant(tolua_S,"enchSmite",cEnchantments::enchSmite);
   tolua_constant(tolua_S,"enchBaneOfArthropods",cEnchantments::enchBaneOfArthropods);
   tolua_constant(tolua_S,"enchKnockback",cEnchantments::enchKnockback);
   tolua_constant(tolua_S,"enchFireAspect",cEnchantments::enchFireAspect);
   tolua_constant(tolua_S,"enchLooting",cEnchantments::enchLooting);
   tolua_constant(tolua_S,"enchEfficiency",cEnchantments::enchEfficiency);
   tolua_constant(tolua_S,"enchSilkTouch",cEnchantments::enchSilkTouch);
   tolua_constant(tolua_S,"enchUnbreaking",cEnchantments::enchUnbreaking);
   tolua_constant(tolua_S,"enchFortune",cEnchantments::enchFortune);
   tolua_constant(tolua_S,"enchPower",cEnchantments::enchPower);
   tolua_constant(tolua_S,"enchPunch",cEnchantments::enchPunch);
   tolua_constant(tolua_S,"enchFlame",cEnchantments::enchFlame);
   tolua_constant(tolua_S,"enchInfinity",cEnchantments::enchInfinity);
   tolua_function(tolua_S,"new",tolua_AllToLua_cEnchantments_new00);
   tolua_function(tolua_S,"new_local",tolua_AllToLua_cEnchantments_new00_local);
   tolua_function(tolua_S,".call",tolua_AllToLua_cEnchantments_new00_local);
   tolua_function(tolua_S,"new",tolua_AllToLua_cEnchantments_new01);
   tolua_function(tolua_S,"new_local",tolua_AllToLua_cEnchantments_new01_local);
   tolua_function(tolua_S,".call",tolua_AllToLua_cEnchantments_new01_local);
   tolua_function(tolua_S,"AddFromString",tolua_AllToLua_cEnchantments_AddFromString00);
   tolua_function(tolua_S,"ToString",tolua_AllToLua_cEnchantments_ToString00);
   tolua_function(tolua_S,"GetLevel",tolua_AllToLua_cEnchantments_GetLevel00);
   tolua_function(tolua_S,"SetLevel",tolua_AllToLua_cEnchantments_SetLevel00);
   tolua_function(tolua_S,"Clear",tolua_AllToLua_cEnchantments_Clear00);
   tolua_function(tolua_S,"IsEmpty",tolua_AllToLua_cEnchantments_IsEmpty00);
   tolua_function(tolua_S,"StringToEnchantmentID",tolua_AllToLua_cEnchantments_StringToEnchantmentID00);
   tolua_function(tolua_S,".eq",tolua_AllToLua_cEnchantments__eq00);
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
   tolua_function(tolua_S,"new",tolua_AllToLua_cItem_new01);
   tolua_function(tolua_S,"new_local",tolua_AllToLua_cItem_new01_local);
   tolua_function(tolua_S,".call",tolua_AllToLua_cItem_new01_local);
   tolua_function(tolua_S,"new",tolua_AllToLua_cItem_new02);
   tolua_function(tolua_S,"new_local",tolua_AllToLua_cItem_new02_local);
   tolua_function(tolua_S,".call",tolua_AllToLua_cItem_new02_local);
   tolua_function(tolua_S,"new",tolua_AllToLua_cItem_new03);
   tolua_function(tolua_S,"new_local",tolua_AllToLua_cItem_new03_local);
   tolua_function(tolua_S,".call",tolua_AllToLua_cItem_new03_local);
   tolua_function(tolua_S,"Empty",tolua_AllToLua_cItem_Empty00);
   tolua_function(tolua_S,"Clear",tolua_AllToLua_cItem_Clear00);
   tolua_function(tolua_S,"IsEmpty",tolua_AllToLua_cItem_IsEmpty00);
   tolua_function(tolua_S,"IsEqual",tolua_AllToLua_cItem_IsEqual00);
   tolua_function(tolua_S,"IsSameType",tolua_AllToLua_cItem_IsSameType00);
   tolua_function(tolua_S,"CopyOne",tolua_AllToLua_cItem_CopyOne00);
   tolua_function(tolua_S,"AddCount",tolua_AllToLua_cItem_AddCount00);
   tolua_function(tolua_S,"GetMaxDamage",tolua_AllToLua_cItem_GetMaxDamage00);
   tolua_function(tolua_S,"DamageItem",tolua_AllToLua_cItem_DamageItem00);
   tolua_function(tolua_S,"IsDamageable",tolua_AllToLua_cItem_IsDamageable00);
   tolua_function(tolua_S,"IsStackableWith",tolua_AllToLua_cItem_IsStackableWith00);
   tolua_function(tolua_S,"IsFullStack",tolua_AllToLua_cItem_IsFullStack00);
   tolua_variable(tolua_S,"m_ItemType",tolua_get_cItem_m_ItemType,tolua_set_cItem_m_ItemType);
   tolua_variable(tolua_S,"m_ItemCount",tolua_get_cItem_m_ItemCount,tolua_set_cItem_m_ItemCount);
   tolua_variable(tolua_S,"m_ItemDamage",tolua_get_cItem_m_ItemDamage,tolua_set_cItem_m_ItemDamage);
   tolua_variable(tolua_S,"m_Enchantments",tolua_get_cItem_m_Enchantments,tolua_set_cItem_m_Enchantments);
  tolua_endmodule(tolua_S);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"cItems","cItems","",tolua_collect_cItems);
  #else
  tolua_cclass(tolua_S,"cItems","cItems","",NULL);
  #endif
  tolua_beginmodule(tolua_S,"cItems");
   tolua_function(tolua_S,"new",tolua_AllToLua_cItems_new00);
   tolua_function(tolua_S,"new_local",tolua_AllToLua_cItems_new00_local);
   tolua_function(tolua_S,".call",tolua_AllToLua_cItems_new00_local);
   tolua_function(tolua_S,"Get",tolua_AllToLua_cItems_Get00);
   tolua_function(tolua_S,"Set",tolua_AllToLua_cItems_Set00);
   tolua_function(tolua_S,"Add",tolua_AllToLua_cItems_Add00);
   tolua_function(tolua_S,"Delete",tolua_AllToLua_cItems_Delete00);
   tolua_function(tolua_S,"Clear",tolua_AllToLua_cItems_Clear00);
   tolua_function(tolua_S,"Size",tolua_AllToLua_cItems_Size00);
   tolua_function(tolua_S,"Set",tolua_AllToLua_cItems_Set01);
   tolua_function(tolua_S,"Add",tolua_AllToLua_cItems_Add01);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"cItemGrid","cItemGrid","",NULL);
  tolua_beginmodule(tolua_S,"cItemGrid");
   tolua_function(tolua_S,"GetWidth",tolua_AllToLua_cItemGrid_GetWidth00);
   tolua_function(tolua_S,"GetHeight",tolua_AllToLua_cItemGrid_GetHeight00);
   tolua_function(tolua_S,"GetNumSlots",tolua_AllToLua_cItemGrid_GetNumSlots00);
   tolua_function(tolua_S,"GetSlotNum",tolua_AllToLua_cItemGrid_GetSlotNum00);
   tolua_function(tolua_S,"GetSlot",tolua_AllToLua_cItemGrid_GetSlot00);
   tolua_function(tolua_S,"GetSlot",tolua_AllToLua_cItemGrid_GetSlot01);
   tolua_function(tolua_S,"SetSlot",tolua_AllToLua_cItemGrid_SetSlot00);
   tolua_function(tolua_S,"SetSlot",tolua_AllToLua_cItemGrid_SetSlot01);
   tolua_function(tolua_S,"SetSlot",tolua_AllToLua_cItemGrid_SetSlot02);
   tolua_function(tolua_S,"SetSlot",tolua_AllToLua_cItemGrid_SetSlot03);
   tolua_function(tolua_S,"EmptySlot",tolua_AllToLua_cItemGrid_EmptySlot00);
   tolua_function(tolua_S,"EmptySlot",tolua_AllToLua_cItemGrid_EmptySlot01);
   tolua_function(tolua_S,"IsSlotEmpty",tolua_AllToLua_cItemGrid_IsSlotEmpty00);
   tolua_function(tolua_S,"IsSlotEmpty",tolua_AllToLua_cItemGrid_IsSlotEmpty01);
   tolua_function(tolua_S,"Clear",tolua_AllToLua_cItemGrid_Clear00);
   tolua_function(tolua_S,"HowManyCanFit",tolua_AllToLua_cItemGrid_HowManyCanFit00);
   tolua_function(tolua_S,"AddItem",tolua_AllToLua_cItemGrid_AddItem00);
   tolua_function(tolua_S,"AddItems",tolua_AllToLua_cItemGrid_AddItems00);
   tolua_function(tolua_S,"ChangeSlotCount",tolua_AllToLua_cItemGrid_ChangeSlotCount00);
   tolua_function(tolua_S,"ChangeSlotCount",tolua_AllToLua_cItemGrid_ChangeSlotCount01);
   tolua_function(tolua_S,"RemoveOneItem",tolua_AllToLua_cItemGrid_RemoveOneItem00);
   tolua_function(tolua_S,"RemoveOneItem",tolua_AllToLua_cItemGrid_RemoveOneItem01);
   tolua_function(tolua_S,"HowManyItems",tolua_AllToLua_cItemGrid_HowManyItems00);
   tolua_function(tolua_S,"HasItems",tolua_AllToLua_cItemGrid_HasItems00);
   tolua_function(tolua_S,"GetFirstEmptySlot",tolua_AllToLua_cItemGrid_GetFirstEmptySlot00);
   tolua_function(tolua_S,"GetFirstUsedSlot",tolua_AllToLua_cItemGrid_GetFirstUsedSlot00);
   tolua_function(tolua_S,"GetLastEmptySlot",tolua_AllToLua_cItemGrid_GetLastEmptySlot00);
   tolua_function(tolua_S,"GetLastUsedSlot",tolua_AllToLua_cItemGrid_GetLastUsedSlot00);
   tolua_function(tolua_S,"GetNextEmptySlot",tolua_AllToLua_cItemGrid_GetNextEmptySlot00);
   tolua_function(tolua_S,"GetNextUsedSlot",tolua_AllToLua_cItemGrid_GetNextUsedSlot00);
   tolua_function(tolua_S,"CopyToItems",tolua_AllToLua_cItemGrid_CopyToItems00);
   tolua_function(tolua_S,"DamageItem",tolua_AllToLua_cItemGrid_DamageItem00);
   tolua_function(tolua_S,"DamageItem",tolua_AllToLua_cItemGrid_DamageItem01);
  tolua_endmodule(tolua_S);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"cBlockEntity","cBlockEntity","",tolua_collect_cBlockEntity);
  #else
  tolua_cclass(tolua_S,"cBlockEntity","cBlockEntity","",NULL);
  #endif
  tolua_beginmodule(tolua_S,"cBlockEntity");
   tolua_function(tolua_S,"GetPosX",tolua_AllToLua_cBlockEntity_GetPosX00);
   tolua_function(tolua_S,"GetPosY",tolua_AllToLua_cBlockEntity_GetPosY00);
   tolua_function(tolua_S,"GetPosZ",tolua_AllToLua_cBlockEntity_GetPosZ00);
   tolua_function(tolua_S,"GetBlockType",tolua_AllToLua_cBlockEntity_GetBlockType00);
   tolua_function(tolua_S,"GetWorld",tolua_AllToLua_cBlockEntity_GetWorld00);
   tolua_function(tolua_S,"GetChunkX",tolua_AllToLua_cBlockEntity_GetChunkX00);
   tolua_function(tolua_S,"GetChunkZ",tolua_AllToLua_cBlockEntity_GetChunkZ00);
   tolua_function(tolua_S,"GetRelX",tolua_AllToLua_cBlockEntity_GetRelX00);
   tolua_function(tolua_S,"GetRelZ",tolua_AllToLua_cBlockEntity_GetRelZ00);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"cBlockEntityWithItems","cBlockEntityWithItems","cBlockEntity",NULL);
  tolua_beginmodule(tolua_S,"cBlockEntityWithItems");
   tolua_function(tolua_S,"GetSlot",tolua_AllToLua_cBlockEntityWithItems_GetSlot00);
   tolua_function(tolua_S,"GetSlot",tolua_AllToLua_cBlockEntityWithItems_GetSlot01);
   tolua_function(tolua_S,"SetSlot",tolua_AllToLua_cBlockEntityWithItems_SetSlot00);
   tolua_function(tolua_S,"SetSlot",tolua_AllToLua_cBlockEntityWithItems_SetSlot01);
   tolua_function(tolua_S,"GetContents",tolua_AllToLua_cBlockEntityWithItems_GetContents00);
  tolua_endmodule(tolua_S);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"cChestEntity","cChestEntity","cBlockEntityWithItems",tolua_collect_cChestEntity);
  #else
  tolua_cclass(tolua_S,"cChestEntity","cChestEntity","cBlockEntityWithItems",NULL);
  #endif
  tolua_beginmodule(tolua_S,"cChestEntity");
   tolua_constant(tolua_S,"ContentsHeight",cChestEntity::ContentsHeight);
   tolua_constant(tolua_S,"ContentsWidth",cChestEntity::ContentsWidth);
   tolua_function(tolua_S,"new",tolua_AllToLua_cChestEntity_new00);
   tolua_function(tolua_S,"new_local",tolua_AllToLua_cChestEntity_new00_local);
   tolua_function(tolua_S,".call",tolua_AllToLua_cChestEntity_new00_local);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"cDropSpenserEntity","cDropSpenserEntity","cBlockEntityWithItems",NULL);
  tolua_beginmodule(tolua_S,"cDropSpenserEntity");
   tolua_constant(tolua_S,"ContentsHeight",cDropSpenserEntity::ContentsHeight);
   tolua_constant(tolua_S,"ContentsWidth",cDropSpenserEntity::ContentsWidth);
   tolua_function(tolua_S,"AddDropSpenserDir",tolua_AllToLua_cDropSpenserEntity_AddDropSpenserDir00);
   tolua_function(tolua_S,"Activate",tolua_AllToLua_cDropSpenserEntity_Activate00);
   tolua_function(tolua_S,"SetRedstonePower",tolua_AllToLua_cDropSpenserEntity_SetRedstonePower00);
   tolua_variable(tolua_S,"__cBlockEntityWindowOwner__",tolua_get_cDropSpenserEntity___cBlockEntityWindowOwner__,NULL);
  tolua_endmodule(tolua_S);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"cDispenserEntity","cDispenserEntity","cDropSpenserEntity",tolua_collect_cDispenserEntity);
  #else
  tolua_cclass(tolua_S,"cDispenserEntity","cDispenserEntity","cDropSpenserEntity",NULL);
  #endif
  tolua_beginmodule(tolua_S,"cDispenserEntity");
   tolua_function(tolua_S,"new",tolua_AllToLua_cDispenserEntity_new00);
   tolua_function(tolua_S,"new_local",tolua_AllToLua_cDispenserEntity_new00_local);
   tolua_function(tolua_S,".call",tolua_AllToLua_cDispenserEntity_new00_local);
  tolua_endmodule(tolua_S);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"cDropperEntity","cDropperEntity","cDropSpenserEntity",tolua_collect_cDropperEntity);
  #else
  tolua_cclass(tolua_S,"cDropperEntity","cDropperEntity","cDropSpenserEntity",NULL);
  #endif
  tolua_beginmodule(tolua_S,"cDropperEntity");
   tolua_function(tolua_S,"new",tolua_AllToLua_cDropperEntity_new00);
   tolua_function(tolua_S,"new_local",tolua_AllToLua_cDropperEntity_new00_local);
   tolua_function(tolua_S,".call",tolua_AllToLua_cDropperEntity_new00_local);
  tolua_endmodule(tolua_S);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"cFurnaceEntity","cFurnaceEntity","cBlockEntityWithItems",tolua_collect_cFurnaceEntity);
  #else
  tolua_cclass(tolua_S,"cFurnaceEntity","cFurnaceEntity","cBlockEntityWithItems",NULL);
  #endif
  tolua_beginmodule(tolua_S,"cFurnaceEntity");
   tolua_constant(tolua_S,"fsInput",cFurnaceEntity::fsInput);
   tolua_constant(tolua_S,"fsFuel",cFurnaceEntity::fsFuel);
   tolua_constant(tolua_S,"fsOutput",cFurnaceEntity::fsOutput);
   tolua_constant(tolua_S,"ContentsWidth",cFurnaceEntity::ContentsWidth);
   tolua_constant(tolua_S,"ContentsHeight",cFurnaceEntity::ContentsHeight);
   tolua_function(tolua_S,"new",tolua_AllToLua_cFurnaceEntity_new00);
   tolua_function(tolua_S,"new_local",tolua_AllToLua_cFurnaceEntity_new00_local);
   tolua_function(tolua_S,".call",tolua_AllToLua_cFurnaceEntity_new00_local);
   tolua_function(tolua_S,"GetInputSlot",tolua_AllToLua_cFurnaceEntity_GetInputSlot00);
   tolua_function(tolua_S,"GetFuelSlot",tolua_AllToLua_cFurnaceEntity_GetFuelSlot00);
   tolua_function(tolua_S,"GetOutputSlot",tolua_AllToLua_cFurnaceEntity_GetOutputSlot00);
   tolua_function(tolua_S,"SetInputSlot",tolua_AllToLua_cFurnaceEntity_SetInputSlot00);
   tolua_function(tolua_S,"SetFuelSlot",tolua_AllToLua_cFurnaceEntity_SetFuelSlot00);
   tolua_function(tolua_S,"SetOutputSlot",tolua_AllToLua_cFurnaceEntity_SetOutputSlot00);
   tolua_function(tolua_S,"GetTimeCooked",tolua_AllToLua_cFurnaceEntity_GetTimeCooked00);
   tolua_function(tolua_S,"GetCookTimeLeft",tolua_AllToLua_cFurnaceEntity_GetCookTimeLeft00);
   tolua_function(tolua_S,"GetFuelBurnTimeLeft",tolua_AllToLua_cFurnaceEntity_GetFuelBurnTimeLeft00);
   tolua_function(tolua_S,"HasFuelTimeLeft",tolua_AllToLua_cFurnaceEntity_HasFuelTimeLeft00);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"HTTPFormData","HTTPFormData","",NULL);
  tolua_beginmodule(tolua_S,"HTTPFormData");
   tolua_variable(tolua_S,"Name",tolua_get_HTTPFormData_Name,tolua_set_HTTPFormData_Name);
   tolua_variable(tolua_S,"Value",tolua_get_HTTPFormData_Value,tolua_set_HTTPFormData_Value);
   tolua_variable(tolua_S,"Type",tolua_get_HTTPFormData_Type,tolua_set_HTTPFormData_Type);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"HTTPRequest","HTTPRequest","",NULL);
  tolua_beginmodule(tolua_S,"HTTPRequest");
   tolua_variable(tolua_S,"Method",tolua_get_HTTPRequest_Method,tolua_set_HTTPRequest_Method);
   tolua_variable(tolua_S,"Path",tolua_get_HTTPRequest_Path,tolua_set_HTTPRequest_Path);
   tolua_variable(tolua_S,"Username",tolua_get_HTTPRequest_Username,tolua_set_HTTPRequest_Username);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"HTTPTemplateRequest","HTTPTemplateRequest","",NULL);
  tolua_beginmodule(tolua_S,"HTTPTemplateRequest");
   tolua_variable(tolua_S,"Request",tolua_get_HTTPTemplateRequest_Request,tolua_set_HTTPTemplateRequest_Request);
  tolua_endmodule(tolua_S);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"sWebAdminPage","sWebAdminPage","",tolua_collect_sWebAdminPage);
  #else
  tolua_cclass(tolua_S,"sWebAdminPage","sWebAdminPage","",NULL);
  #endif
  tolua_beginmodule(tolua_S,"sWebAdminPage");
   tolua_variable(tolua_S,"Content",tolua_get_sWebAdminPage_Content,tolua_set_sWebAdminPage_Content);
   tolua_variable(tolua_S,"PluginName",tolua_get_sWebAdminPage_PluginName,tolua_set_sWebAdminPage_PluginName);
   tolua_variable(tolua_S,"TabName",tolua_get_sWebAdminPage_TabName,tolua_set_sWebAdminPage_TabName);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"cWebAdmin","cWebAdmin","",NULL);
  tolua_beginmodule(tolua_S,"cWebAdmin");
   tolua_function(tolua_S,"GetPort",tolua_AllToLua_cWebAdmin_GetPort00);
   tolua_function(tolua_S,"GetPage",tolua_AllToLua_cWebAdmin_GetPage00);
   tolua_function(tolua_S,"GetBaseURL",tolua_AllToLua_cWebAdmin_GetBaseURL00);
   tolua_function(tolua_S,"GetMemoryUsage",tolua_AllToLua_cWebAdmin_GetMemoryUsage00);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"cWebPlugin","cWebPlugin","",NULL);
  tolua_beginmodule(tolua_S,"cWebPlugin");
   tolua_function(tolua_S,"GetWebTitle",tolua_AllToLua_cWebPlugin_GetWebTitle00);
   tolua_function(tolua_S,"HandleWebRequest",tolua_AllToLua_cWebPlugin_HandleWebRequest00);
   tolua_function(tolua_S,"SafeString",tolua_AllToLua_cWebPlugin_SafeString00);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"Lua__cWebPlugin","Lua__cWebPlugin","cWebPlugin",NULL);
  tolua_beginmodule(tolua_S,"Lua__cWebPlugin");
   tolua_function(tolua_S,"tolua__set_instance",tolua_AllToLua_Lua__cWebPlugin_tolua__set_instance00);
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
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"cRoot","cRoot","",NULL);
  tolua_beginmodule(tolua_S,"cRoot");
   tolua_variable(tolua_S,"m_PrimaryServerVersion",tolua_get_cRoot_m_PrimaryServerVersion,tolua_set_cRoot_m_PrimaryServerVersion);
   tolua_function(tolua_S,"Get",tolua_AllToLua_cRoot_Get00);
   tolua_function(tolua_S,"GetServer",tolua_AllToLua_cRoot_GetServer00);
   tolua_function(tolua_S,"GetDefaultWorld",tolua_AllToLua_cRoot_GetDefaultWorld00);
   tolua_function(tolua_S,"GetWorld",tolua_AllToLua_cRoot_GetWorld00);
   tolua_function(tolua_S,"GetPrimaryServerVersion",tolua_AllToLua_cRoot_GetPrimaryServerVersion00);
   tolua_function(tolua_S,"SetPrimaryServerVersion",tolua_AllToLua_cRoot_SetPrimaryServerVersion00);
   tolua_function(tolua_S,"GetGroupManager",tolua_AllToLua_cRoot_GetGroupManager00);
   tolua_function(tolua_S,"GetCraftingRecipes",tolua_AllToLua_cRoot_GetCraftingRecipes00);
   tolua_function(tolua_S,"GetFurnaceRecipe",tolua_AllToLua_cRoot_GetFurnaceRecipe00);
   tolua_function(tolua_S,"GetWebAdmin",tolua_AllToLua_cRoot_GetWebAdmin00);
   tolua_function(tolua_S,"GetPluginManager",tolua_AllToLua_cRoot_GetPluginManager00);
   tolua_function(tolua_S,"QueueExecuteConsoleCommand",tolua_AllToLua_cRoot_QueueExecuteConsoleCommand00);
   tolua_function(tolua_S,"GetTotalChunkCount",tolua_AllToLua_cRoot_GetTotalChunkCount00);
   tolua_function(tolua_S,"SaveAllChunks",tolua_AllToLua_cRoot_SaveAllChunks00);
   tolua_function(tolua_S,"GetProtocolVersionTextFromInt",tolua_AllToLua_cRoot_GetProtocolVersionTextFromInt00);
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
   tolua_function(tolua_S,"Equals",tolua_AllToLua_Vector3d_Equals00);
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
   tolua_variable(tolua_S,"p1",tolua_get_cCuboid_p1,tolua_set_cCuboid_p1);
   tolua_variable(tolua_S,"p2",tolua_get_cCuboid_p2,tolua_set_cCuboid_p2);
   tolua_function(tolua_S,"new",tolua_AllToLua_cCuboid_new00);
   tolua_function(tolua_S,"new_local",tolua_AllToLua_cCuboid_new00_local);
   tolua_function(tolua_S,".call",tolua_AllToLua_cCuboid_new00_local);
   tolua_function(tolua_S,"new",tolua_AllToLua_cCuboid_new01);
   tolua_function(tolua_S,"new_local",tolua_AllToLua_cCuboid_new01_local);
   tolua_function(tolua_S,".call",tolua_AllToLua_cCuboid_new01_local);
   tolua_function(tolua_S,"new",tolua_AllToLua_cCuboid_new02);
   tolua_function(tolua_S,"new_local",tolua_AllToLua_cCuboid_new02_local);
   tolua_function(tolua_S,".call",tolua_AllToLua_cCuboid_new02_local);
   tolua_function(tolua_S,"new",tolua_AllToLua_cCuboid_new03);
   tolua_function(tolua_S,"new_local",tolua_AllToLua_cCuboid_new03_local);
   tolua_function(tolua_S,".call",tolua_AllToLua_cCuboid_new03_local);
   tolua_function(tolua_S,"new",tolua_AllToLua_cCuboid_new04);
   tolua_function(tolua_S,"new_local",tolua_AllToLua_cCuboid_new04_local);
   tolua_function(tolua_S,".call",tolua_AllToLua_cCuboid_new04_local);
   tolua_function(tolua_S,"Assign",tolua_AllToLua_cCuboid_Assign00);
   tolua_function(tolua_S,"Sort",tolua_AllToLua_cCuboid_Sort00);
   tolua_function(tolua_S,"DifX",tolua_AllToLua_cCuboid_DifX00);
   tolua_function(tolua_S,"DifY",tolua_AllToLua_cCuboid_DifY00);
   tolua_function(tolua_S,"DifZ",tolua_AllToLua_cCuboid_DifZ00);
   tolua_function(tolua_S,"DoesIntersect",tolua_AllToLua_cCuboid_DoesIntersect00);
   tolua_function(tolua_S,"IsInside",tolua_AllToLua_cCuboid_IsInside00);
   tolua_function(tolua_S,"IsInside",tolua_AllToLua_cCuboid_IsInside01);
   tolua_function(tolua_S,"IsInside",tolua_AllToLua_cCuboid_IsInside02);
   tolua_function(tolua_S,"IsCompletelyInside",tolua_AllToLua_cCuboid_IsCompletelyInside00);
   tolua_function(tolua_S,"Move",tolua_AllToLua_cCuboid_Move00);
   tolua_function(tolua_S,"IsSorted",tolua_AllToLua_cCuboid_IsSorted00);
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
   tolua_variable(tolua_S,"BlockHitPosition",tolua_get_cTracer_BlockHitPosition,tolua_set_cTracer_BlockHitPosition);
   tolua_variable(tolua_S,"HitNormal",tolua_get_cTracer_HitNormal,tolua_set_cTracer_HitNormal);
   tolua_variable(tolua_S,"RealHit",tolua_get_cTracer_RealHit,tolua_set_cTracer_RealHit);
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
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"cBlockArea","cBlockArea","",tolua_collect_cBlockArea);
  #else
  tolua_cclass(tolua_S,"cBlockArea","cBlockArea","",NULL);
  #endif
  tolua_beginmodule(tolua_S,"cBlockArea");
   tolua_constant(tolua_S,"baTypes",cBlockArea::baTypes);
   tolua_constant(tolua_S,"baMetas",cBlockArea::baMetas);
   tolua_constant(tolua_S,"baLight",cBlockArea::baLight);
   tolua_constant(tolua_S,"baSkyLight",cBlockArea::baSkyLight);
   tolua_constant(tolua_S,"msOverwrite",cBlockArea::msOverwrite);
   tolua_constant(tolua_S,"msFillAir",cBlockArea::msFillAir);
   tolua_constant(tolua_S,"msImprint",cBlockArea::msImprint);
   tolua_constant(tolua_S,"msLake",cBlockArea::msLake);
   tolua_function(tolua_S,"new",tolua_AllToLua_cBlockArea_new00);
   tolua_function(tolua_S,"new_local",tolua_AllToLua_cBlockArea_new00_local);
   tolua_function(tolua_S,".call",tolua_AllToLua_cBlockArea_new00_local);
   tolua_function(tolua_S,"delete",tolua_AllToLua_cBlockArea_delete00);
   tolua_function(tolua_S,"Clear",tolua_AllToLua_cBlockArea_Clear00);
   tolua_function(tolua_S,"Create",tolua_AllToLua_cBlockArea_Create00);
   tolua_function(tolua_S,"Create",tolua_AllToLua_cBlockArea_Create01);
   tolua_function(tolua_S,"SetOrigin",tolua_AllToLua_cBlockArea_SetOrigin00);
   tolua_function(tolua_S,"Read",tolua_AllToLua_cBlockArea_Read00);
   tolua_function(tolua_S,"Read",tolua_AllToLua_cBlockArea_Read01);
   tolua_function(tolua_S,"Write",tolua_AllToLua_cBlockArea_Write00);
   tolua_function(tolua_S,"Write",tolua_AllToLua_cBlockArea_Write01);
   tolua_function(tolua_S,"CopyTo",tolua_AllToLua_cBlockArea_CopyTo00);
   tolua_function(tolua_S,"CopyFrom",tolua_AllToLua_cBlockArea_CopyFrom00);
   tolua_function(tolua_S,"DumpToRawFile",tolua_AllToLua_cBlockArea_DumpToRawFile00);
   tolua_function(tolua_S,"LoadFromSchematicFile",tolua_AllToLua_cBlockArea_LoadFromSchematicFile00);
   tolua_function(tolua_S,"SaveToSchematicFile",tolua_AllToLua_cBlockArea_SaveToSchematicFile00);
   tolua_function(tolua_S,"Crop",tolua_AllToLua_cBlockArea_Crop00);
   tolua_function(tolua_S,"Expand",tolua_AllToLua_cBlockArea_Expand00);
   tolua_function(tolua_S,"Merge",tolua_AllToLua_cBlockArea_Merge00);
   tolua_function(tolua_S,"Fill",tolua_AllToLua_cBlockArea_Fill00);
   tolua_function(tolua_S,"FillRelCuboid",tolua_AllToLua_cBlockArea_FillRelCuboid00);
   tolua_function(tolua_S,"RelLine",tolua_AllToLua_cBlockArea_RelLine00);
   tolua_function(tolua_S,"RotateCCW",tolua_AllToLua_cBlockArea_RotateCCW00);
   tolua_function(tolua_S,"RotateCW",tolua_AllToLua_cBlockArea_RotateCW00);
   tolua_function(tolua_S,"MirrorXY",tolua_AllToLua_cBlockArea_MirrorXY00);
   tolua_function(tolua_S,"MirrorXZ",tolua_AllToLua_cBlockArea_MirrorXZ00);
   tolua_function(tolua_S,"MirrorYZ",tolua_AllToLua_cBlockArea_MirrorYZ00);
   tolua_function(tolua_S,"RotateCCWNoMeta",tolua_AllToLua_cBlockArea_RotateCCWNoMeta00);
   tolua_function(tolua_S,"RotateCWNoMeta",tolua_AllToLua_cBlockArea_RotateCWNoMeta00);
   tolua_function(tolua_S,"MirrorXYNoMeta",tolua_AllToLua_cBlockArea_MirrorXYNoMeta00);
   tolua_function(tolua_S,"MirrorXZNoMeta",tolua_AllToLua_cBlockArea_MirrorXZNoMeta00);
   tolua_function(tolua_S,"MirrorYZNoMeta",tolua_AllToLua_cBlockArea_MirrorYZNoMeta00);
   tolua_function(tolua_S,"SetRelBlockType",tolua_AllToLua_cBlockArea_SetRelBlockType00);
   tolua_function(tolua_S,"SetBlockType",tolua_AllToLua_cBlockArea_SetBlockType00);
   tolua_function(tolua_S,"SetRelBlockMeta",tolua_AllToLua_cBlockArea_SetRelBlockMeta00);
   tolua_function(tolua_S,"SetBlockMeta",tolua_AllToLua_cBlockArea_SetBlockMeta00);
   tolua_function(tolua_S,"SetRelBlockLight",tolua_AllToLua_cBlockArea_SetRelBlockLight00);
   tolua_function(tolua_S,"SetBlockLight",tolua_AllToLua_cBlockArea_SetBlockLight00);
   tolua_function(tolua_S,"SetRelBlockSkyLight",tolua_AllToLua_cBlockArea_SetRelBlockSkyLight00);
   tolua_function(tolua_S,"SetBlockSkyLight",tolua_AllToLua_cBlockArea_SetBlockSkyLight00);
   tolua_function(tolua_S,"GetRelBlockType",tolua_AllToLua_cBlockArea_GetRelBlockType00);
   tolua_function(tolua_S,"GetBlockType",tolua_AllToLua_cBlockArea_GetBlockType00);
   tolua_function(tolua_S,"GetRelBlockMeta",tolua_AllToLua_cBlockArea_GetRelBlockMeta00);
   tolua_function(tolua_S,"GetBlockMeta",tolua_AllToLua_cBlockArea_GetBlockMeta00);
   tolua_function(tolua_S,"GetRelBlockLight",tolua_AllToLua_cBlockArea_GetRelBlockLight00);
   tolua_function(tolua_S,"GetBlockLight",tolua_AllToLua_cBlockArea_GetBlockLight00);
   tolua_function(tolua_S,"GetRelBlockSkyLight",tolua_AllToLua_cBlockArea_GetRelBlockSkyLight00);
   tolua_function(tolua_S,"GetBlockSkyLight",tolua_AllToLua_cBlockArea_GetBlockSkyLight00);
   tolua_function(tolua_S,"SetBlockTypeMeta",tolua_AllToLua_cBlockArea_SetBlockTypeMeta00);
   tolua_function(tolua_S,"SetRelBlockTypeMeta",tolua_AllToLua_cBlockArea_SetRelBlockTypeMeta00);
   tolua_function(tolua_S,"GetBlockTypeMeta",tolua_AllToLua_cBlockArea_GetBlockTypeMeta00);
   tolua_function(tolua_S,"GetRelBlockTypeMeta",tolua_AllToLua_cBlockArea_GetRelBlockTypeMeta00);
   tolua_function(tolua_S,"GetSizeX",tolua_AllToLua_cBlockArea_GetSizeX00);
   tolua_function(tolua_S,"GetSizeY",tolua_AllToLua_cBlockArea_GetSizeY00);
   tolua_function(tolua_S,"GetSizeZ",tolua_AllToLua_cBlockArea_GetSizeZ00);
   tolua_function(tolua_S,"GetOriginX",tolua_AllToLua_cBlockArea_GetOriginX00);
   tolua_function(tolua_S,"GetOriginY",tolua_AllToLua_cBlockArea_GetOriginY00);
   tolua_function(tolua_S,"GetOriginZ",tolua_AllToLua_cBlockArea_GetOriginZ00);
   tolua_function(tolua_S,"GetDataTypes",tolua_AllToLua_cBlockArea_GetDataTypes00);
   tolua_function(tolua_S,"HasBlockTypes",tolua_AllToLua_cBlockArea_HasBlockTypes00);
   tolua_function(tolua_S,"HasBlockMetas",tolua_AllToLua_cBlockArea_HasBlockMetas00);
   tolua_function(tolua_S,"HasBlockLights",tolua_AllToLua_cBlockArea_HasBlockLights00);
   tolua_function(tolua_S,"HasBlockSkyLights",tolua_AllToLua_cBlockArea_HasBlockSkyLights00);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"cChunkDesc","cChunkDesc","",NULL);
  tolua_beginmodule(tolua_S,"cChunkDesc");
   tolua_function(tolua_S,"GetChunkX",tolua_AllToLua_cChunkDesc_GetChunkX00);
   tolua_function(tolua_S,"GetChunkZ",tolua_AllToLua_cChunkDesc_GetChunkZ00);
   tolua_function(tolua_S,"SetChunkCoords",tolua_AllToLua_cChunkDesc_SetChunkCoords00);
   tolua_function(tolua_S,"FillBlocks",tolua_AllToLua_cChunkDesc_FillBlocks00);
   tolua_function(tolua_S,"SetBlockTypeMeta",tolua_AllToLua_cChunkDesc_SetBlockTypeMeta00);
   tolua_function(tolua_S,"GetBlockTypeMeta",tolua_AllToLua_cChunkDesc_GetBlockTypeMeta00);
   tolua_function(tolua_S,"SetBlockType",tolua_AllToLua_cChunkDesc_SetBlockType00);
   tolua_function(tolua_S,"GetBlockType",tolua_AllToLua_cChunkDesc_GetBlockType00);
   tolua_function(tolua_S,"SetBlockMeta",tolua_AllToLua_cChunkDesc_SetBlockMeta00);
   tolua_function(tolua_S,"GetBlockMeta",tolua_AllToLua_cChunkDesc_GetBlockMeta00);
   tolua_function(tolua_S,"SetBiome",tolua_AllToLua_cChunkDesc_SetBiome00);
   tolua_function(tolua_S,"GetBiome",tolua_AllToLua_cChunkDesc_GetBiome00);
   tolua_function(tolua_S,"SetHeight",tolua_AllToLua_cChunkDesc_SetHeight00);
   tolua_function(tolua_S,"GetHeight",tolua_AllToLua_cChunkDesc_GetHeight00);
   tolua_function(tolua_S,"SetUseDefaultBiomes",tolua_AllToLua_cChunkDesc_SetUseDefaultBiomes00);
   tolua_function(tolua_S,"IsUsingDefaultBiomes",tolua_AllToLua_cChunkDesc_IsUsingDefaultBiomes00);
   tolua_function(tolua_S,"SetUseDefaultHeight",tolua_AllToLua_cChunkDesc_SetUseDefaultHeight00);
   tolua_function(tolua_S,"IsUsingDefaultHeight",tolua_AllToLua_cChunkDesc_IsUsingDefaultHeight00);
   tolua_function(tolua_S,"SetUseDefaultComposition",tolua_AllToLua_cChunkDesc_SetUseDefaultComposition00);
   tolua_function(tolua_S,"IsUsingDefaultComposition",tolua_AllToLua_cChunkDesc_IsUsingDefaultComposition00);
   tolua_function(tolua_S,"SetUseDefaultStructures",tolua_AllToLua_cChunkDesc_SetUseDefaultStructures00);
   tolua_function(tolua_S,"IsUsingDefaultStructures",tolua_AllToLua_cChunkDesc_IsUsingDefaultStructures00);
   tolua_function(tolua_S,"SetUseDefaultFinish",tolua_AllToLua_cChunkDesc_SetUseDefaultFinish00);
   tolua_function(tolua_S,"IsUsingDefaultFinish",tolua_AllToLua_cChunkDesc_IsUsingDefaultFinish00);
   tolua_function(tolua_S,"WriteBlockArea",tolua_AllToLua_cChunkDesc_WriteBlockArea00);
   tolua_function(tolua_S,"ReadBlockArea",tolua_AllToLua_cChunkDesc_ReadBlockArea00);
   tolua_function(tolua_S,"GetMaxHeight",tolua_AllToLua_cChunkDesc_GetMaxHeight00);
   tolua_function(tolua_S,"FillRelCuboid",tolua_AllToLua_cChunkDesc_FillRelCuboid00);
   tolua_function(tolua_S,"FillRelCuboid",tolua_AllToLua_cChunkDesc_FillRelCuboid01);
   tolua_function(tolua_S,"ReplaceRelCuboid",tolua_AllToLua_cChunkDesc_ReplaceRelCuboid00);
   tolua_function(tolua_S,"ReplaceRelCuboid",tolua_AllToLua_cChunkDesc_ReplaceRelCuboid01);
   tolua_function(tolua_S,"FloorRelCuboid",tolua_AllToLua_cChunkDesc_FloorRelCuboid00);
   tolua_function(tolua_S,"FloorRelCuboid",tolua_AllToLua_cChunkDesc_FloorRelCuboid01);
   tolua_function(tolua_S,"RandomFillRelCuboid",tolua_AllToLua_cChunkDesc_RandomFillRelCuboid00);
   tolua_function(tolua_S,"RandomFillRelCuboid",tolua_AllToLua_cChunkDesc_RandomFillRelCuboid01);
  tolua_endmodule(tolua_S);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"cCraftingGrid","cCraftingGrid","",tolua_collect_cCraftingGrid);
  #else
  tolua_cclass(tolua_S,"cCraftingGrid","cCraftingGrid","",NULL);
  #endif
  tolua_beginmodule(tolua_S,"cCraftingGrid");
   tolua_function(tolua_S,"new",tolua_AllToLua_cCraftingGrid_new00);
   tolua_function(tolua_S,"new_local",tolua_AllToLua_cCraftingGrid_new00_local);
   tolua_function(tolua_S,".call",tolua_AllToLua_cCraftingGrid_new00_local);
   tolua_function(tolua_S,"GetWidth",tolua_AllToLua_cCraftingGrid_GetWidth00);
   tolua_function(tolua_S,"GetHeight",tolua_AllToLua_cCraftingGrid_GetHeight00);
   tolua_function(tolua_S,"GetItem",tolua_AllToLua_cCraftingGrid_GetItem00);
   tolua_function(tolua_S,"SetItem",tolua_AllToLua_cCraftingGrid_SetItem00);
   tolua_function(tolua_S,"SetItem",tolua_AllToLua_cCraftingGrid_SetItem01);
   tolua_function(tolua_S,"Clear",tolua_AllToLua_cCraftingGrid_Clear00);
   tolua_function(tolua_S,"ConsumeGrid",tolua_AllToLua_cCraftingGrid_ConsumeGrid00);
   tolua_function(tolua_S,"Dump",tolua_AllToLua_cCraftingGrid_Dump00);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"cCraftingRecipe","cCraftingRecipe","",NULL);
  tolua_beginmodule(tolua_S,"cCraftingRecipe");
   tolua_function(tolua_S,"Clear",tolua_AllToLua_cCraftingRecipe_Clear00);
   tolua_function(tolua_S,"GetIngredientsWidth",tolua_AllToLua_cCraftingRecipe_GetIngredientsWidth00);
   tolua_function(tolua_S,"GetIngredientsHeight",tolua_AllToLua_cCraftingRecipe_GetIngredientsHeight00);
   tolua_function(tolua_S,"GetIngredient",tolua_AllToLua_cCraftingRecipe_GetIngredient00);
   tolua_function(tolua_S,"GetResult",tolua_AllToLua_cCraftingRecipe_GetResult00);
   tolua_function(tolua_S,"SetResult",tolua_AllToLua_cCraftingRecipe_SetResult00);
   tolua_function(tolua_S,"SetResult",tolua_AllToLua_cCraftingRecipe_SetResult01);
   tolua_function(tolua_S,"SetIngredient",tolua_AllToLua_cCraftingRecipe_SetIngredient00);
   tolua_function(tolua_S,"SetIngredient",tolua_AllToLua_cCraftingRecipe_SetIngredient01);
   tolua_function(tolua_S,"ConsumeIngredients",tolua_AllToLua_cCraftingRecipe_ConsumeIngredients00);
   tolua_function(tolua_S,"Dump",tolua_AllToLua_cCraftingRecipe_Dump00);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"cWindow","cWindow","",NULL);
  tolua_beginmodule(tolua_S,"cWindow");
   tolua_constant(tolua_S,"Inventory",cWindow::Inventory);
   tolua_constant(tolua_S,"Chest",cWindow::Chest);
   tolua_constant(tolua_S,"Workbench",cWindow::Workbench);
   tolua_constant(tolua_S,"Furnace",cWindow::Furnace);
   tolua_constant(tolua_S,"DropSpenser",cWindow::DropSpenser);
   tolua_constant(tolua_S,"Enchantment",cWindow::Enchantment);
   tolua_constant(tolua_S,"Brewery",cWindow::Brewery);
   tolua_constant(tolua_S,"NPCTrade",cWindow::NPCTrade);
   tolua_constant(tolua_S,"Beacon",cWindow::Beacon);
   tolua_constant(tolua_S,"Anvil",cWindow::Anvil);
   tolua_constant(tolua_S,"Hopper",cWindow::Hopper);
   tolua_function(tolua_S,"GetWindowID",tolua_AllToLua_cWindow_GetWindowID00);
   tolua_function(tolua_S,"GetWindowType",tolua_AllToLua_cWindow_GetWindowType00);
   tolua_function(tolua_S,"GetSlot",tolua_AllToLua_cWindow_GetSlot00);
   tolua_function(tolua_S,"SetSlot",tolua_AllToLua_cWindow_SetSlot00);
   tolua_function(tolua_S,"IsSlotInPlayerMainInventory",tolua_AllToLua_cWindow_IsSlotInPlayerMainInventory00);
   tolua_function(tolua_S,"IsSlotInPlayerHotbar",tolua_AllToLua_cWindow_IsSlotInPlayerHotbar00);
   tolua_function(tolua_S,"IsSlotInPlayerInventory",tolua_AllToLua_cWindow_IsSlotInPlayerInventory00);
   tolua_function(tolua_S,"GetWindowTitle",tolua_AllToLua_cWindow_GetWindowTitle00);
   tolua_function(tolua_S,"SetWindowTitle",tolua_AllToLua_cWindow_SetWindowTitle00);
  tolua_endmodule(tolua_S);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"cLuaWindow","cLuaWindow","cWindow",tolua_collect_cLuaWindow);
  #else
  tolua_cclass(tolua_S,"cLuaWindow","cLuaWindow","cWindow",NULL);
  #endif
  tolua_beginmodule(tolua_S,"cLuaWindow");
   tolua_function(tolua_S,"new",tolua_AllToLua_cLuaWindow_new00);
   tolua_function(tolua_S,"new_local",tolua_AllToLua_cLuaWindow_new00_local);
   tolua_function(tolua_S,".call",tolua_AllToLua_cLuaWindow_new00_local);
   tolua_function(tolua_S,"delete",tolua_AllToLua_cLuaWindow_delete00);
   tolua_function(tolua_S,"GetContents",tolua_AllToLua_cLuaWindow_GetContents00);
   tolua_variable(tolua_S,"__cItemGrid__cListener__",tolua_get_cLuaWindow___cItemGrid__cListener__,NULL);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"cLineBlockTracer","cLineBlockTracer","",NULL);
  tolua_beginmodule(tolua_S,"cLineBlockTracer");
  tolua_endmodule(tolua_S);
 tolua_endmodule(tolua_S);
 return 1;
}


#if defined(LUA_VERSION_NUM) && LUA_VERSION_NUM >= 501
 TOLUA_API int luaopen_AllToLua (lua_State* tolua_S) {
 return tolua_AllToLua_open(tolua_S);
};
#endif

