
// ManualBindings_RankManager.cpp

// Implements the cRankManager Lua bindings

#include "Globals.h"
#include "ManualBindings.h"
#include "../Root.h"
#include "tolua++/include/tolua++.h"
#include "LuaState.h"





/** Binds cRankManager::AddGroup */
static int tolua_cRankManager_AddGroup(lua_State * L)
{
	// Function signature:
	// cRankManager:AddGroup(GroupName)

	cLuaState S(L);
	if (
		!S.CheckParamUserTable(1, "cRankManager") ||
		!S.CheckParamString(2) ||
		!S.CheckParamEnd(3)
	)
	{
		return 0;
	}
	
	// Read the params:
	AString GroupName;
	S.GetStackValue(2, GroupName);
	
	// Add the group:
	cRoot::Get()->GetRankManager().AddGroup(GroupName);
	return 0;
}





/** Binds cRankManager::AddGroupToRank */
static int tolua_cRankManager_AddGroupToRank(lua_State * L)
{
	// Function signature:
	// cRankManager:AddGroupToRank(GroupName, RankName) -> bool

	cLuaState S(L);
	if (
		!S.CheckParamUserTable(1, "cRankManager") ||
		!S.CheckParamString(2, 3) ||
		!S.CheckParamEnd(4)
	)
	{
		return 0;
	}
	
	// Read the params:
	AString GroupName, RankName;
	S.GetStackValues(2, GroupName, RankName);
	
	// Add the group to the rank:
	S.Push(cRoot::Get()->GetRankManager().AddGroupToRank(GroupName, RankName));
	return 1;
}





/** Binds cRankManager::AddPermissionToGroup */
static int tolua_cRankManager_AddPermissionToGroup(lua_State * L)
{
	// Function signature:
	// cRankManager:AddPermissionToGroup(Permission, GroupName) -> bool

	cLuaState S(L);
	if (
		!S.CheckParamUserTable(1, "cRankManager") ||
		!S.CheckParamString(2, 3) ||
		!S.CheckParamEnd(4)
	)
	{
		return 0;
	}
	
	// Read the params:
	AString GroupName, Permission;
	S.GetStackValues(2, Permission, GroupName);
	
	// Add the group to the rank:
	S.Push(cRoot::Get()->GetRankManager().AddPermissionToGroup(Permission, GroupName));
	return 1;
}





/** Binds cRankManager::AddRank */
static int tolua_cRankManager_AddRank(lua_State * L)
{
	// Function signature:
	// cRankManager:AddRank(RankName)
	
	cLuaState S(L);
	if (
		!S.CheckParamUserTable(1, "cRankManager") ||
		!S.CheckParamString(2, 5) ||
		!S.CheckParamEnd(6)
	)
	{
		return 0;
	}
	
	// Read the params:
	AString RankName, MsgPrefix, MsgSuffix, MsgNameColorCode;
	S.GetStackValues(2, RankName, MsgPrefix, MsgSuffix, MsgNameColorCode);
	
	// Add the rank:
	cRoot::Get()->GetRankManager().AddRank(RankName, MsgPrefix, MsgSuffix, MsgNameColorCode);
	return 0;
}





/** Binds cRankManager::GetAllGroups */
static int tolua_cRankManager_GetAllGroups(lua_State * L)
{
	// Function signature:
	// cRankManager:GetAllGroups() -> arraytable of GroupNames
	
	cLuaState S(L);
	if (
		!S.CheckParamUserTable(1, "cRankManager") ||
		!S.CheckParamEnd(2)
	)
	{
		return 0;
	}
	
	// Get the groups:
	AStringVector Groups = cRoot::Get()->GetRankManager().GetAllGroups();
	
	// Push the results:
	S.Push(Groups);
	return 1;
}





/** Binds cRankManager::GetAllPermissions */
static int tolua_cRankManager_GetAllPermissions(lua_State * L)
{
	// Function signature:
	// cRankManager:GetAllPermissions() -> arraytable of Permissions
	
	cLuaState S(L);
	if (
		!S.CheckParamUserTable(1, "cRankManager") ||
		!S.CheckParamEnd(2)
	)
	{
		return 0;
	}
	
	// Get the permissions:
	AStringVector Permissions = cRoot::Get()->GetRankManager().GetAllPermissions();
	
	// Push the results:
	S.Push(Permissions);
	return 1;
}





/** Binds cRankManager::GetAllRanks */
static int tolua_cRankManager_GetAllRanks(lua_State * L)
{
	// Function signature:
	// cRankManager:GetAllRanks() -> arraytable of RankNames
	
	cLuaState S(L);
	if (
		!S.CheckParamUserTable(1, "cRankManager") ||
		!S.CheckParamEnd(2)
	)
	{
		return 0;
	}
	
	// Get the ranks:
	AStringVector Ranks = cRoot::Get()->GetRankManager().GetAllRanks();
	
	// Push the results:
	S.Push(Ranks);
	return 1;
}





/** Binds cRankManager::GetDefaultRank */
static int tolua_cRankManager_GetDefaultRank(lua_State * L)
{
	// Function signature:
	// cRankManager:GetDefaultRank() -> string
	
	cLuaState S(L);
	if (
		!S.CheckParamUserTable(1, "cRankManager") ||
		!S.CheckParamEnd(2)
	)
	{
		return 0;
	}
	
	// Return the rank name:
	S.Push(cRoot::Get()->GetRankManager().GetDefaultRank());
	return 1;
}





/** Binds cRankManager::GetGroupPermissions */
static int tolua_cRankManager_GetGroupPermissions(lua_State * L)
{
	// Function signature:
	// cRankManager:GetGroupPermissions(GroupName) -> arraytable of permissions
	
	cLuaState S(L);
	if (
		!S.CheckParamUserTable(1, "cRankManager") ||
		!S.CheckParamString(2) ||
		!S.CheckParamEnd(3)
	)
	{
		return 0;
	}
	
	// Get the params:
	AString GroupName;
	S.GetStackValue(2, GroupName);
	
	// Get the permissions:
	AStringVector Permissions = cRoot::Get()->GetRankManager().GetGroupPermissions(GroupName);
	
	// Push the results:
	S.Push(Permissions);
	return 1;
}





/** Binds cRankManager::GetPlayerGroups */
static int tolua_cRankManager_GetPlayerGroups(lua_State * L)
{
	// Function signature:
	// cRankManager:GetPlayerGroups(PlayerUUID) -> arraytable of GroupNames
	
	cLuaState S(L);
	if (
		!S.CheckParamUserTable(1, "cRankManager") ||
		!S.CheckParamString(2) ||
		!S.CheckParamEnd(3)
	)
	{
		return 0;
	}
	
	// Get the params:
	AString PlayerUUID;
	S.GetStackValue(2, PlayerUUID);
	
	// Get the groups:
	AStringVector Groups = cRoot::Get()->GetRankManager().GetPlayerGroups(PlayerUUID);
	
	// Push the results:
	S.Push(Groups);
	return 1;
}





/** Binds cRankManager::GetPlayerMsgVisuals */
static int tolua_cRankManager_GetPlayerMsgVisuals(lua_State * L)
{
	// Function signature:
	// cRankManager:GetPlayerMsgVisuals(PlayerUUID) -> string, string, string
	
	cLuaState S(L);
	if (
		!S.CheckParamUserTable(1, "cRankManager") ||
		!S.CheckParamString(2) ||
		!S.CheckParamEnd(3)
	)
	{
		return 0;
	}
	
	// Get the params:
	AString PlayerUUID;
	S.GetStackValue(2, PlayerUUID);
	
	// Get the permissions:
	AString MsgPrefix, MsgSuffix, MsgNameColorCode;
	if (!cRoot::Get()->GetRankManager().GetPlayerMsgVisuals(PlayerUUID, MsgPrefix, MsgSuffix, MsgNameColorCode))
	{
		return 0;
	}
	
	// Push the results:
	S.Push(MsgPrefix);
	S.Push(MsgSuffix);
	S.Push(MsgNameColorCode);
	return 3;
}





/** Binds cRankManager::GetPlayerPermissions */
static int tolua_cRankManager_GetPlayerPermissions(lua_State * L)
{
	// Function signature:
	// cRankManager:GetPlayerPermissions(PlayerUUID) -> arraytable of permissions
	
	cLuaState S(L);
	if (
		!S.CheckParamUserTable(1, "cRankManager") ||
		!S.CheckParamString(2) ||
		!S.CheckParamEnd(3)
	)
	{
		return 0;
	}
	
	// Get the params:
	AString PlayerUUID;
	S.GetStackValue(2, PlayerUUID);
	
	// Get the permissions:
	AStringVector Permissions = cRoot::Get()->GetRankManager().GetPlayerPermissions(PlayerUUID);
	
	// Push the results:
	S.Push(Permissions);
	return 1;
}





/** Binds cRankManager::GetPlayerRankName */
static int tolua_cRankManager_GetPlayerRankName(lua_State * L)
{
	// Function signature:
	// cRankManager:GetPlayerRankName(PlayerUUID) -> string
	
	cLuaState S(L);
	if (
		!S.CheckParamUserTable(1, "cRankManager") ||
		!S.CheckParamString(2) ||
		!S.CheckParamEnd(3)
	)
	{
		return 0;
	}
	
	// Get the params:
	AString PlayerUUID;
	S.GetStackValue(2, PlayerUUID);
	
	// Get the rank name:
	AString RankName = cRoot::Get()->GetRankManager().GetPlayerRankName(PlayerUUID);
	
	// Push the result:
	S.Push(RankName);
	return 1;
}





/** Binds cRankManager::GetRankGroups */
static int tolua_cRankManager_GetRankGroups(lua_State * L)
{
	// Function signature:
	// cRankManager:GetRankGroups(RankName) -> arraytable of groupnames
	
	cLuaState S(L);
	if (
		!S.CheckParamUserTable(1, "cRankManager") ||
		!S.CheckParamString(2) ||
		!S.CheckParamEnd(3)
	)
	{
		return 0;
	}
	
	// Get the params:
	AString RankName;
	S.GetStackValue(2, RankName);
	
	// Get the groups:
	AStringVector Groups = cRoot::Get()->GetRankManager().GetRankGroups(RankName);
	
	// Push the results:
	S.Push(Groups);
	return 1;
}





/** Binds cRankManager::GetRankPermissions */
static int tolua_cRankManager_GetRankPermissions(lua_State * L)
{
	// Function signature:
	// cRankManager:GetRankPermissions(RankName) -> arraytable of permissions
	
	cLuaState S(L);
	if (
		!S.CheckParamUserTable(1, "cRankManager") ||
		!S.CheckParamString(2) ||
		!S.CheckParamEnd(3)
	)
	{
		return 0;
	}
	
	// Get the params:
	AString RankName;
	S.GetStackValue(2, RankName);
	
	// Get the permissions:
	AStringVector Permissions = cRoot::Get()->GetRankManager().GetRankPermissions(RankName);
	
	// Push the results:
	S.Push(Permissions);
	return 1;
}





/** Binds cRankManager::GetRankVisuals */
static int tolua_cRankManager_GetRankVisuals(lua_State * L)
{
	// Function signature:
	// cRankManager:GetRankVisuals(RankName) -> MsgPrefix, MsgSuffix, MsgNameColorCode
	
	cLuaState S(L);
	if (
		!S.CheckParamUserTable(1, "cRankManager") ||
		!S.CheckParamString(2) ||
		!S.CheckParamEnd(3)
	)
	{
		return 0;
	}
	
	// Get the params:
	AString RankName;
	S.GetStackValue(2, RankName);
	
	// Get the visuals:
	AString MsgPrefix, MsgSuffix, MsgNameColorCode;
	if (!cRoot::Get()->GetRankManager().GetRankVisuals(RankName, MsgPrefix, MsgSuffix, MsgNameColorCode))
	{
		// No such rank, return nothing:
		return 0;
	}
	
	// Push the results:
	S.Push(MsgPrefix);
	S.Push(MsgSuffix);
	S.Push(MsgNameColorCode);
	return 3;
}





/** Binds cRankManager::GroupExists */
static int tolua_cRankManager_GroupExists(lua_State * L)
{
	// Function signature:
	// cRankManager:GroupExists(GroupName) -> bool
	
	cLuaState S(L);
	if (
		!S.CheckParamUserTable(1, "cRankManager") ||
		!S.CheckParamString(2) ||
		!S.CheckParamEnd(3)
	)
	{
		return 0;
	}
	
	// Get the params:
	AString GroupName;
	S.GetStackValue(2, GroupName);
	
	// Get the response:
	bool res = cRoot::Get()->GetRankManager().GroupExists(GroupName);
	
	// Push the result:
	S.Push(res);
	return 1;
}





/** Binds cRankManager::IsGroupInRank */
static int tolua_cRankManager_IsGroupInRank(lua_State * L)
{
	// Function signature:
	// cRankManager:IsGroupInRank(GroupName, RankName) -> bool
	
	cLuaState S(L);
	if (
		!S.CheckParamUserTable(1, "cRankManager") ||
		!S.CheckParamString(2, 3) ||
		!S.CheckParamEnd(4)
	)
	{
		return 0;
	}
	
	// Get the params:
	AString GroupName, RankName;
	S.GetStackValues(2, GroupName, RankName);
	
	// Get the response:
	bool res = cRoot::Get()->GetRankManager().IsGroupInRank(GroupName, RankName);
	
	// Push the result:
	S.Push(res);
	return 1;
}





/** Binds cRankManager::IsPermissionInGroup */
static int tolua_cRankManager_IsPermissionInGroup(lua_State * L)
{
	// Function signature:
	// cRankManager:IsPermissionInGroup(Permission, GroupName) -> bool
	
	cLuaState S(L);
	if (
		!S.CheckParamUserTable(1, "cRankManager") ||
		!S.CheckParamString(2, 3) ||
		!S.CheckParamEnd(4)
	)
	{
		return 0;
	}
	
	// Get the params:
	AString GroupName, Permission;
	S.GetStackValues(2, Permission, GroupName);
	
	// Get the response:
	bool res = cRoot::Get()->GetRankManager().IsPermissionInGroup(Permission, GroupName);
	
	// Push the result:
	S.Push(res);
	return 1;
}





/** Binds cRankManager::IsPlayerRankSet */
static int tolua_cRankManager_IsPlayerRankSet(lua_State * L)
{
	// Function signature:
	// cRankManager:IsPlayerRankSet(PlayerUUID) -> bool
	
	cLuaState S(L);
	if (
		!S.CheckParamUserTable(1, "cRankManager") ||
		!S.CheckParamString(2) ||
		!S.CheckParamEnd(3)
	)
	{
		return 0;
	}
	
	// Get the params:
	AString PlayerUUID;
	S.GetStackValue(2, PlayerUUID);
	
	// Get the response:
	bool res = cRoot::Get()->GetRankManager().IsPlayerRankSet(PlayerUUID);
	
	// Push the result:
	S.Push(res);
	return 1;
}





/** Binds cRankManager::RankExists */
static int tolua_cRankManager_RankExists(lua_State * L)
{
	// Function signature:
	// cRankManager:RankExists(RankName) -> bool
	
	cLuaState S(L);
	if (
		!S.CheckParamUserTable(1, "cRankManager") ||
		!S.CheckParamString(2) ||
		!S.CheckParamEnd(3)
	)
	{
		return 0;
	}
	
	// Get the params:
	AString RankName;
	S.GetStackValue(2, RankName);
	
	// Get the response:
	bool res = cRoot::Get()->GetRankManager().RankExists(RankName);
	
	// Push the result:
	S.Push(res);
	return 1;
}





/** Binds cRankManager::RemoveGroup */
static int tolua_cRankManager_RemoveGroup(lua_State * L)
{
	// Function signature:
	// cRankManager:RemoveGroup(GroupName)
	
	cLuaState S(L);
	if (
		!S.CheckParamUserTable(1, "cRankManager") ||
		!S.CheckParamString(2) ||
		!S.CheckParamEnd(3)
	)
	{
		return 0;
	}
	
	// Get the params:
	AString GroupName;
	S.GetStackValue(2, GroupName);
	
	// Remove the group:
	cRoot::Get()->GetRankManager().RemoveGroup(GroupName);
	return 0;
}





/** Binds cRankManager::RemoveGroupFromRank */
static int tolua_cRankManager_RemoveGroupFromRank(lua_State * L)
{
	// Function signature:
	// cRankManager:RemoveGroupFromRank(GroupName, RankName)
	
	cLuaState S(L);
	if (
		!S.CheckParamUserTable(1, "cRankManager") ||
		!S.CheckParamString(2, 3) ||
		!S.CheckParamEnd(4)
	)
	{
		return 0;
	}
	
	// Get the params:
	AString GroupName, RankName;
	S.GetStackValues(2, GroupName, RankName);
	
	// Remove the group:
	cRoot::Get()->GetRankManager().RemoveGroupFromRank(GroupName, RankName);
	return 0;
}





/** Binds cRankManager::RemovePermissionFromGroup */
static int tolua_cRankManager_RemovePermissionFromGroup(lua_State * L)
{
	// Function signature:
	// cRankManager:RemovePermissionFromGroup(Permission, GroupName)
	
	cLuaState S(L);
	if (
		!S.CheckParamUserTable(1, "cRankManager") ||
		!S.CheckParamString(2, 3) ||
		!S.CheckParamEnd(4)
	)
	{
		return 0;
	}
	
	// Get the params:
	AString GroupName, Permission;
	S.GetStackValues(2, Permission, GroupName);
	
	// Remove the group:
	cRoot::Get()->GetRankManager().RemovePermissionFromGroup(Permission, GroupName);
	return 0;
}





/** Binds cRankManager::RemovePlayerRank */
static int tolua_cRankManager_RemovePlayerRank(lua_State * L)
{
	// Function signature:
	// cRankManager:RemovePlayerRank(PlayerUUID)
	
	cLuaState S(L);
	if (
		!S.CheckParamUserTable(1, "cRankManager") ||
		!S.CheckParamString(2) ||
		!S.CheckParamEnd(3)
	)
	{
		return 0;
	}
	
	// Get the params:
	AString PlayerUUID;
	S.GetStackValue(2, PlayerUUID);
	
	// Remove the player's rank:
	cRoot::Get()->GetRankManager().RemovePlayerRank(PlayerUUID);
	return 0;
}





/** Binds cRankManager::RemoveRank */
static int tolua_cRankManager_RemoveRank(lua_State * L)
{
	// Function signature:
	// cRankManager:RemoveRank(RankName, [ReplacementRankName])
	
	cLuaState S(L);
	if (
		!S.CheckParamUserTable(1, "cRankManager") ||
		!S.CheckParamString(2) ||
		// Param 3 is otpional, defaults to nil -> empty string
		!S.CheckParamEnd(4)
	)
	{
		return 0;
	}
	
	// Get the params:
	AString RankName, ReplacementRankName;
	S.GetStackValues(2, RankName, ReplacementRankName);
	
	// Remove the rank:
	cRoot::Get()->GetRankManager().RemoveRank(RankName, ReplacementRankName);
	return 0;
}





/** Binds cRankManager::RenameGroup */
static int tolua_cRankManager_RenameGroup(lua_State * L)
{
	// Function signature:
	// cRankManager:RenameGroup(OldName, NewName)
	
	cLuaState S(L);
	if (
		!S.CheckParamUserTable(1, "cRankManager") ||
		!S.CheckParamString(2, 3) ||
		!S.CheckParamEnd(4)
	)
	{
		return 0;
	}
	
	// Get the params:
	AString OldName, NewName;
	S.GetStackValues(2, OldName, NewName);
	
	// Remove the group:
	bool res = cRoot::Get()->GetRankManager().RenameGroup(OldName, NewName);
	
	// Push the result:
	S.Push(res);
	return 1;
}





/** Binds cRankManager::RenameRank */
static int tolua_cRankManager_RenameRank(lua_State * L)
{
	// Function signature:
	// cRankManager:RenameRank(OldName, NewName)
	
	cLuaState S(L);
	if (
		!S.CheckParamUserTable(1, "cRankManager") ||
		!S.CheckParamString(2, 3) ||
		!S.CheckParamEnd(4)
	)
	{
		return 0;
	}
	
	// Get the params:
	AString OldName, NewName;
	S.GetStackValues(2, OldName, NewName);
	
	// Remove the rank:
	bool res = cRoot::Get()->GetRankManager().RenameRank(OldName, NewName);
	
	// Push the result:
	S.Push(res);
	return 1;
}





/** Binds cRankManager::SetDefaultRank */
static int tolua_cRankManager_SetDefaultRank(lua_State * L)
{
	// Function signature:
	// cRankManager:SetDefaultRank(RankName) -> bool
	
	cLuaState S(L);
	if (
		!S.CheckParamUserTable(1, "cRankManager") ||
		!S.CheckParamString(2) ||
		!S.CheckParamEnd(3)
	)
	{
		return 0;
	}
	
	// Get the params:
	AString RankName;
	S.GetStackValue(2, RankName);
	
	// Set the rank, return the result:
	S.Push(cRoot::Get()->GetRankManager().SetDefaultRank(RankName));
	return 0;
}





/** Binds cRankManager::SetPlayerRank */
static int tolua_cRankManager_SetPlayerRank(lua_State * L)
{
	// Function signature:
	// cRankManager:SetPlayerRank(PlayerUUID, PlayerName, RankName)
	
	cLuaState S(L);
	if (
		!S.CheckParamUserTable(1, "cRankManager") ||
		!S.CheckParamString(2, 4) ||
		!S.CheckParamEnd(5)
	)
	{
		return 0;
	}
	
	// Get the params:
	AString PlayerUUID, PlayerName, RankName;
	S.GetStackValues(2, PlayerUUID, PlayerName, RankName);
	
	// Set the rank:
	cRoot::Get()->GetRankManager().SetPlayerRank(PlayerUUID, PlayerName, RankName);
	return 0;
}





/** Binds cRankManager::SetRankVisuals */
static int tolua_cRankManager_SetRankVisuals(lua_State * L)
{
	// Function signature:
	// cRankManager:SetRankVisuals(RankName, MsgPrefix, MsgSuffix, MsgNameColorCode)
	
	cLuaState S(L);
	if (
		!S.CheckParamUserTable(1, "cRankManager") ||
		!S.CheckParamString(2, 5) ||
		!S.CheckParamEnd(6)
	)
	{
		return 0;
	}
	
	// Get the params:
	AString RankName, MsgPrefix, MsgSuffix, MsgNameColorCode;
	S.GetStackValues(2, RankName, MsgPrefix, MsgSuffix, MsgNameColorCode);
	
	// Set the visuals:
	cRoot::Get()->GetRankManager().SetRankVisuals(RankName, MsgPrefix, MsgSuffix, MsgNameColorCode);
	return 0;
}





void ManualBindings::BindRankManager(lua_State * tolua_S)
{
	// Create the cRankManager class in the API:
	tolua_usertype(tolua_S, "cRankManager");
	tolua_cclass(tolua_S, "cRankManager", "cRankManager", "", NULL);
	
	// Fill in the functions (alpha-sorted):
	tolua_beginmodule(tolua_S, "cRankManager");
		tolua_function(tolua_S, "AddGroup",                  tolua_cRankManager_AddGroup);
		tolua_function(tolua_S, "AddGroupToRank",            tolua_cRankManager_AddGroupToRank);
		tolua_function(tolua_S, "AddPermissionToGroup",      tolua_cRankManager_AddPermissionToGroup);
		tolua_function(tolua_S, "AddRank",                   tolua_cRankManager_AddRank);
		tolua_function(tolua_S, "GetAllGroups",              tolua_cRankManager_GetAllGroups);
		tolua_function(tolua_S, "GetAllPermissions",         tolua_cRankManager_GetAllPermissions);
		tolua_function(tolua_S, "GetAllRanks",               tolua_cRankManager_GetAllRanks);
		tolua_function(tolua_S, "GetDefaultRank",            tolua_cRankManager_GetDefaultRank);
		tolua_function(tolua_S, "GetGroupPermissions",       tolua_cRankManager_GetGroupPermissions);
		tolua_function(tolua_S, "GetPlayerGroups",           tolua_cRankManager_GetPlayerGroups);
		tolua_function(tolua_S, "GetPlayerMsgVisuals",       tolua_cRankManager_GetPlayerMsgVisuals);
		tolua_function(tolua_S, "GetPlayerPermissions",      tolua_cRankManager_GetPlayerPermissions);
		tolua_function(tolua_S, "GetPlayerRankName",         tolua_cRankManager_GetPlayerRankName);
		tolua_function(tolua_S, "GetRankGroups",             tolua_cRankManager_GetRankGroups);
		tolua_function(tolua_S, "GetRankPermissions",        tolua_cRankManager_GetRankPermissions);
		tolua_function(tolua_S, "GetRankVisuals",            tolua_cRankManager_GetRankVisuals);
		tolua_function(tolua_S, "GroupExists",               tolua_cRankManager_GroupExists);
		tolua_function(tolua_S, "IsGroupInRank",             tolua_cRankManager_IsGroupInRank);
		tolua_function(tolua_S, "IsPermissionInGroup",       tolua_cRankManager_IsPermissionInGroup);
		tolua_function(tolua_S, "IsPlayerRankSet",           tolua_cRankManager_IsPlayerRankSet);
		tolua_function(tolua_S, "RankExists",                tolua_cRankManager_RankExists);
		tolua_function(tolua_S, "RemoveGroup",               tolua_cRankManager_RemoveGroup);
		tolua_function(tolua_S, "RemoveGroupFromRank",       tolua_cRankManager_RemoveGroupFromRank);
		tolua_function(tolua_S, "RemovePermissionFromGroup", tolua_cRankManager_RemovePermissionFromGroup);
		tolua_function(tolua_S, "RemovePlayerRank",          tolua_cRankManager_RemovePlayerRank);
		tolua_function(tolua_S, "RemoveRank",                tolua_cRankManager_RemoveRank);
		tolua_function(tolua_S, "RenameGroup",               tolua_cRankManager_RenameGroup);
		tolua_function(tolua_S, "RenameRank",                tolua_cRankManager_RenameRank);
		tolua_function(tolua_S, "SetDefaultRank",            tolua_cRankManager_SetDefaultRank);
		tolua_function(tolua_S, "SetPlayerRank",             tolua_cRankManager_SetPlayerRank);
		tolua_function(tolua_S, "SetRankVisuals",            tolua_cRankManager_SetRankVisuals);
	tolua_endmodule(tolua_S);
}




