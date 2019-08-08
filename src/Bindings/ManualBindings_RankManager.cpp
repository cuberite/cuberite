
// ManualBindings_RankManager.cpp

// Implements the cRankManager Lua bindings

#include "Globals.h"
#include "ManualBindings.h"
#include "tolua++/include/tolua++.h"
#include "LuaState.h"
#include "../Root.h"
#include "../UUID.h"





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
	cRoot::Get()->GetRankManager()->AddGroup(GroupName);
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
	S.Push(cRoot::Get()->GetRankManager()->AddGroupToRank(GroupName, RankName));
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
	S.Push(cRoot::Get()->GetRankManager()->AddPermissionToGroup(Permission, GroupName));
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
	cRoot::Get()->GetRankManager()->AddRank(RankName, MsgPrefix, MsgSuffix, MsgNameColorCode);
	return 0;
}





/** Binds cRankManager::AddRestrictionToGroup */
static int tolua_cRankManager_AddRestrictionToGroup(lua_State * L)
{
	// Function signature:
	// cRankManager:AddRestrictionToGroup(Restriction, GroupName) -> bool

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
	AString GroupName, Restriction;
	S.GetStackValues(2, Restriction, GroupName);

	// Add the group to the rank:
	S.Push(cRoot::Get()->GetRankManager()->AddRestrictionToGroup(Restriction, GroupName));
	return 1;
}





/** Binds cRankManager::ClearPlayerRanks */
static int tolua_cRankManager_ClearPlayerRanks(lua_State * L)
{
	cLuaState S(L);
	if (
		!S.CheckParamUserTable(1, "cRankManager") ||
		!S.CheckParamEnd(2)
	)
	{
		return 0;
	}

	// Remove all players:
	cRoot::Get()->GetRankManager()->ClearPlayerRanks();
	return 1;
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
	AStringVector Groups = cRoot::Get()->GetRankManager()->GetAllGroups();

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
	AStringVector Permissions = cRoot::Get()->GetRankManager()->GetAllPermissions();

	// Push the results:
	S.Push(Permissions);
	return 1;
}





/** Binds cRankManager::GetAllPermissionsRestrictions */
static int tolua_cRankManager_GetAllPermissionsRestrictions(lua_State * L)
{
	// Function signature:
	// cRankManager:GetAllPermissionsRestrictions() -> arraytable of Permissions and Restrictions together

	cLuaState S(L);
	if (
		!S.CheckParamUserTable(1, "cRankManager") ||
		!S.CheckParamEnd(2)
	)
	{
		return 0;
	}

	// Push the results:
	S.Push(cRoot::Get()->GetRankManager()->GetAllPermissionsRestrictions());

	return 1;
}





/** Binds cRankManager::GetAllPlayerUUIDs */
static int tolua_cRankManager_GetAllPlayerUUIDs(lua_State * L)
{
	// Function signature:
	// cRankManager:GetAllPlayerUUIDs() -> arraytable of Player UUID's

	cLuaState S(L);
	if (
		!S.CheckParamStaticSelf("cRankManager") ||
		!S.CheckParamEnd(2)
	)
	{
		return 0;
	}

	// Get the player uuid's:
	std::vector<cUUID> Players = cRoot::Get()->GetRankManager()->GetAllPlayerUUIDs();

	// Convert to string UUIDs
	std::vector<AString> StrUUIDs;
	StrUUIDs.reserve(Players.size());
	for (const auto & UUID : Players)
	{
		StrUUIDs.push_back(UUID.ToShortString());
	}

	// Push the results:
	S.Push(StrUUIDs);
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

	// Push the results:
	S.Push(cRoot::Get()->GetRankManager()->GetAllRanks());
	return 1;
}





/** Binds cRankManager::GetAllRestrictions */
static int tolua_cRankManager_GetAllRestrictions(lua_State * L)
{
	// Function signature:
	// cRankManager:GetAllRestrictions() -> arraytable of Restrictions

	cLuaState S(L);
	if (
		!S.CheckParamUserTable(1, "cRankManager") ||
		!S.CheckParamEnd(2)
	)
	{
		return 0;
	}

	// Push the results:
	S.Push(cRoot::Get()->GetRankManager()->GetAllRestrictions());

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
	S.Push(cRoot::Get()->GetRankManager()->GetDefaultRank());
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
	AStringVector Permissions = cRoot::Get()->GetRankManager()->GetGroupPermissions(GroupName);

	// Push the results:
	S.Push(Permissions);
	return 1;
}





/** Binds cRankManager::GetGroupRestrictions */
static int tolua_cRankManager_GetGroupRestrictions(lua_State * L)
{
	// Function signature:
	// cRankManager:GetGroupRestrictions(GroupName) -> arraytable of restrictions

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

	// Get the restrictions:
	AStringVector Restrictions = cRoot::Get()->GetRankManager()->GetGroupRestrictions(GroupName);

	// Push the results:
	S.Push(Restrictions);
	return 1;
}





/** Binds cRankManager::GetPlayerGroups */
static int tolua_cRankManager_GetPlayerGroups(lua_State * L)
{
	// Function signature:
	// cRankManager:GetPlayerGroups(PlayerUUID) -> arraytable of GroupNames

	cLuaState S(L);
	if (
		!S.CheckParamStaticSelf("cRankManager") ||
		!S.CheckParamUUID(2) ||
		!S.CheckParamEnd(3)
	)
	{
		return 0;
	}

	// Get the params:
	cUUID PlayerUUID;
	S.GetStackValue(2, PlayerUUID);

	// Get the groups:
	AStringVector Groups = cRoot::Get()->GetRankManager()->GetPlayerGroups(PlayerUUID);

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
		!S.CheckParamStaticSelf("cRankManager") ||
		!S.CheckParamUUID(2) ||
		!S.CheckParamEnd(3)
	)
	{
		return 0;
	}

	// Get the params:
	cUUID PlayerUUID;
	S.GetStackValue(2, PlayerUUID);

	// Get the permissions:
	AString MsgPrefix, MsgSuffix, MsgNameColorCode;
	if (!cRoot::Get()->GetRankManager()->GetPlayerMsgVisuals(PlayerUUID, MsgPrefix, MsgSuffix, MsgNameColorCode))
	{
		return 0;
	}

	// Push the results:
	S.Push(MsgPrefix, MsgSuffix, MsgNameColorCode);
	return 3;
}





/** Binds cRankManager::GetPlayerPermissions */
static int tolua_cRankManager_GetPlayerPermissions(lua_State * L)
{
	// Function signature:
	// cRankManager:GetPlayerPermissions(PlayerUUID) -> arraytable of permissions

	cLuaState S(L);
	if (
		!S.CheckParamStaticSelf("cRankManager") ||
		!S.CheckParamUUID(2) ||
		!S.CheckParamEnd(3)
	)
	{
		return 0;
	}

	// Get the params:
	cUUID PlayerUUID;
	S.GetStackValue(2, PlayerUUID);

	// Get the permissions:
	AStringVector Permissions = cRoot::Get()->GetRankManager()->GetPlayerPermissions(PlayerUUID);

	// Push the results:
	S.Push(Permissions);
	return 1;
}





/** Binds cRankManager::GetPlayerRestrictions */
static int tolua_cRankManager_GetPlayerRestrictions(lua_State * L)
{
	// Function signature:
	// cRankManager:GetPlayerRestrictions(PlayerUUID) -> arraytable of restrictions

	cLuaState S(L);
	if (
		!S.CheckParamStaticSelf("cRankManager") ||
		!S.CheckParamUUID(2) ||
		!S.CheckParamEnd(3)
	)
	{
		return 0;
	}

	// Get the params:
	cUUID PlayerUUID;
	S.GetStackValue(2, PlayerUUID);

	// Get the permissions:
	AStringVector Restrictions = cRoot::Get()->GetRankManager()->GetPlayerRestrictions(PlayerUUID);

	// Push the results:
	S.Push(Restrictions);
	return 1;
}





/** Binds cRankManager::GetPlayerRankName */
static int tolua_cRankManager_GetPlayerRankName(lua_State * L)
{
	// Function signature:
	// cRankManager:GetPlayerRankName(PlayerUUID) -> string

	cLuaState S(L);
	if (
		!S.CheckParamStaticSelf("cRankManager") ||
		!S.CheckParamUUID(2) ||
		!S.CheckParamEnd(3)
	)
	{
		return 0;
	}

	// Get the params:
	cUUID PlayerUUID;
	S.GetStackValue(2, PlayerUUID);

	// Get the rank name:
	AString RankName = cRoot::Get()->GetRankManager()->GetPlayerRankName(PlayerUUID);

	// Push the result:
	S.Push(RankName);
	return 1;
}





/** Binds cRankManager::GetPlayerName */
static int tolua_cRankManager_GetPlayerName(lua_State * L)
{
	// Function signature:
	// cRankManager:GetPlayerName(PlayerUUID) -> string

	cLuaState S(L);
	if (
		!S.CheckParamStaticSelf("cRankManager") ||
		!S.CheckParamUUID(2) ||
		!S.CheckParamEnd(3)
	)
	{
		return 0;
	}

	// Get the params:
	cUUID PlayerUUID;
	S.GetStackValue(2, PlayerUUID);

	// Get the player name:
	AString PlayerName = cRoot::Get()->GetRankManager()->GetPlayerName(PlayerUUID);

	// Push the result:
	S.Push(PlayerName);
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
	AStringVector Groups = cRoot::Get()->GetRankManager()->GetRankGroups(RankName);

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
	AStringVector Permissions = cRoot::Get()->GetRankManager()->GetRankPermissions(RankName);

	// Push the results:
	S.Push(Permissions);
	return 1;
}





/** Binds cRankManager::GetRankRestrictions */
static int tolua_cRankManager_GetRankRestrictions(lua_State * L)
{
	// Function signature:
	// cRankManager:GetRankRestrictions(RankName) -> arraytable of restrictions

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
	AStringVector Restrictions = cRoot::Get()->GetRankManager()->GetRankRestrictions(RankName);

	// Push the results:
	S.Push(Restrictions);
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
	if (!cRoot::Get()->GetRankManager()->GetRankVisuals(RankName, MsgPrefix, MsgSuffix, MsgNameColorCode))
	{
		// No such rank, return nothing:
		return 0;
	}

	// Push the results:
	S.Push(MsgPrefix, MsgSuffix, MsgNameColorCode);
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
	bool res = cRoot::Get()->GetRankManager()->GroupExists(GroupName);

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
	bool res = cRoot::Get()->GetRankManager()->IsGroupInRank(GroupName, RankName);

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
	bool res = cRoot::Get()->GetRankManager()->IsPermissionInGroup(Permission, GroupName);

	// Push the result:
	S.Push(res);
	return 1;
}





/** Binds cRankManager::IsRestrictionInGroup */
static int tolua_cRankManager_IsRestrictionInGroup(lua_State * L)
{
	// Function signature:
	// cRankManager:IsRestrictionInGroup(Restriction, GroupName) -> bool

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
	AString GroupName, Restriction;
	S.GetStackValues(2, Restriction, GroupName);

	// Get the response:
	bool res = cRoot::Get()->GetRankManager()->IsRestrictionInGroup(Restriction, GroupName);

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
		!S.CheckParamStaticSelf("cRankManager") ||
		!S.CheckParamUUID(2) ||
		!S.CheckParamEnd(3)
	)
	{
		return 0;
	}

	// Get the params:
	cUUID PlayerUUID;
	S.GetStackValue(2, PlayerUUID);

	// Get the response:
	bool res = cRoot::Get()->GetRankManager()->IsPlayerRankSet(PlayerUUID);

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
	bool res = cRoot::Get()->GetRankManager()->RankExists(RankName);

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
	cRoot::Get()->GetRankManager()->RemoveGroup(GroupName);
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
	cRoot::Get()->GetRankManager()->RemoveGroupFromRank(GroupName, RankName);
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

	// Remove the permission:
	cRoot::Get()->GetRankManager()->RemovePermissionFromGroup(Permission, GroupName);
	return 0;
}





/** Binds cRankManager::RemoveRestrictionFromGroup */
static int tolua_cRankManager_RemoveRestrictionFromGroup(lua_State * L)
{
	// Function signature:
	// cRankManager:RemoveRestrictionFromGroup(Restriction, GroupName)

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
	AString GroupName, Restriction;
	S.GetStackValues(2, Restriction, GroupName);

	// Remove the restriction:
	cRoot::Get()->GetRankManager()->RemoveRestrictionFromGroup(Restriction, GroupName);
	return 0;
}





/** Binds cRankManager::RemovePlayerRank */
static int tolua_cRankManager_RemovePlayerRank(lua_State * L)
{
	// Function signature:
	// cRankManager:RemovePlayerRank(PlayerUUID)

	cLuaState S(L);
	if (
		!S.CheckParamStaticSelf("cRankManager") ||
		!S.CheckParamUUID(2) ||
		!S.CheckParamEnd(3)
	)
	{
		return 0;
	}

	// Get the params:
	cUUID PlayerUUID;
	S.GetStackValue(2, PlayerUUID);

	// Remove the player's rank:
	cRoot::Get()->GetRankManager()->RemovePlayerRank(PlayerUUID);
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
	cRoot::Get()->GetRankManager()->RemoveRank(RankName, ReplacementRankName);
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
	bool res = cRoot::Get()->GetRankManager()->RenameGroup(OldName, NewName);

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
	bool res = cRoot::Get()->GetRankManager()->RenameRank(OldName, NewName);

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
	S.Push(cRoot::Get()->GetRankManager()->SetDefaultRank(RankName));
	return 1;
}





/** Binds cRankManager::SetPlayerRank */
static int tolua_cRankManager_SetPlayerRank(lua_State * L)
{
	// Function signature:
	// cRankManager:SetPlayerRank(PlayerUUID, PlayerName, RankName)

	cLuaState S(L);
	if (
		!S.CheckParamStaticSelf("cRankManager") ||
		!S.CheckParamUUID(2) ||
		!S.CheckParamString(3, 4) ||
		!S.CheckParamEnd(5)
	)
	{
		return 0;
	}

	// Get the params:
	AString PlayerName, RankName;
	cUUID PlayerUUID;
	S.GetStackValues(2, PlayerUUID, PlayerName, RankName);

	// Set the rank:
	cRoot::Get()->GetRankManager()->SetPlayerRank(PlayerUUID, PlayerName, RankName);
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
	cRoot::Get()->GetRankManager()->SetRankVisuals(RankName, MsgPrefix, MsgSuffix, MsgNameColorCode);
	return 0;
}





void cManualBindings::BindRankManager(lua_State * tolua_S)
{
	// Create the cRankManager class in the API:
	tolua_usertype(tolua_S, "cRankManager");
	tolua_cclass(tolua_S, "cRankManager", "cRankManager", "", nullptr);

	// Fill in the functions (alpha-sorted):
	tolua_beginmodule(tolua_S, "cRankManager");
		tolua_function(tolua_S, "AddGroup",                      tolua_cRankManager_AddGroup);
		tolua_function(tolua_S, "AddGroupToRank",                tolua_cRankManager_AddGroupToRank);
		tolua_function(tolua_S, "AddPermissionToGroup",          tolua_cRankManager_AddPermissionToGroup);
		tolua_function(tolua_S, "AddRestrictionToGroup",         tolua_cRankManager_AddRestrictionToGroup);
		tolua_function(tolua_S, "AddRank",                       tolua_cRankManager_AddRank);
		tolua_function(tolua_S, "ClearPlayerRanks",              tolua_cRankManager_ClearPlayerRanks);
		tolua_function(tolua_S, "GetAllGroups",                  tolua_cRankManager_GetAllGroups);
		tolua_function(tolua_S, "GetAllPermissions",             tolua_cRankManager_GetAllPermissions);
		tolua_function(tolua_S, "GetAllRestrictions",            tolua_cRankManager_GetAllRestrictions);
		tolua_function(tolua_S, "GetAllPermissionsRestrictions", tolua_cRankManager_GetAllPermissionsRestrictions);
		tolua_function(tolua_S, "GetAllPlayerUUIDs",             tolua_cRankManager_GetAllPlayerUUIDs);
		tolua_function(tolua_S, "GetAllRanks",                   tolua_cRankManager_GetAllRanks);
		tolua_function(tolua_S, "GetDefaultRank",                tolua_cRankManager_GetDefaultRank);
		tolua_function(tolua_S, "GetGroupPermissions",           tolua_cRankManager_GetGroupPermissions);
		tolua_function(tolua_S, "GetGroupRestrictions",          tolua_cRankManager_GetGroupRestrictions);
		tolua_function(tolua_S, "GetPlayerGroups",               tolua_cRankManager_GetPlayerGroups);
		tolua_function(tolua_S, "GetPlayerMsgVisuals",           tolua_cRankManager_GetPlayerMsgVisuals);
		tolua_function(tolua_S, "GetPlayerPermissions",          tolua_cRankManager_GetPlayerPermissions);
		tolua_function(tolua_S, "GetPlayerPermissions",          tolua_cRankManager_GetPlayerRestrictions);
		tolua_function(tolua_S, "GetPlayerRankName",             tolua_cRankManager_GetPlayerRankName);
		tolua_function(tolua_S, "GetPlayerName",                 tolua_cRankManager_GetPlayerName);
		tolua_function(tolua_S, "GetRankGroups",                 tolua_cRankManager_GetRankGroups);
		tolua_function(tolua_S, "GetRankPermissions",            tolua_cRankManager_GetRankPermissions);
		tolua_function(tolua_S, "GetRankRestrictions",           tolua_cRankManager_GetRankRestrictions);
		tolua_function(tolua_S, "GetRankVisuals",                tolua_cRankManager_GetRankVisuals);
		tolua_function(tolua_S, "GroupExists",                   tolua_cRankManager_GroupExists);
		tolua_function(tolua_S, "IsGroupInRank",                 tolua_cRankManager_IsGroupInRank);
		tolua_function(tolua_S, "IsPermissionInGroup",           tolua_cRankManager_IsPermissionInGroup);
		tolua_function(tolua_S, "IsRestrictionInGroup",          tolua_cRankManager_IsRestrictionInGroup);
		tolua_function(tolua_S, "IsPlayerRankSet",               tolua_cRankManager_IsPlayerRankSet);
		tolua_function(tolua_S, "RankExists",                    tolua_cRankManager_RankExists);
		tolua_function(tolua_S, "RemoveGroup",                   tolua_cRankManager_RemoveGroup);
		tolua_function(tolua_S, "RemoveGroupFromRank",           tolua_cRankManager_RemoveGroupFromRank);
		tolua_function(tolua_S, "RemovePermissionFromGroup",     tolua_cRankManager_RemovePermissionFromGroup);
		tolua_function(tolua_S, "RemoveRestrictionFromGroup",    tolua_cRankManager_RemoveRestrictionFromGroup);
		tolua_function(tolua_S, "RemovePlayerRank",              tolua_cRankManager_RemovePlayerRank);
		tolua_function(tolua_S, "RemoveRank",                    tolua_cRankManager_RemoveRank);
		tolua_function(tolua_S, "RenameGroup",                   tolua_cRankManager_RenameGroup);
		tolua_function(tolua_S, "RenameRank",                    tolua_cRankManager_RenameRank);
		tolua_function(tolua_S, "SetDefaultRank",                tolua_cRankManager_SetDefaultRank);
		tolua_function(tolua_S, "SetPlayerRank",                 tolua_cRankManager_SetPlayerRank);
		tolua_function(tolua_S, "SetRankVisuals",                tolua_cRankManager_SetRankVisuals);
	tolua_endmodule(tolua_S);
}




