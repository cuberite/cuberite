
// RankManager.h

// Declares the cRankManager class that represents the rank manager responsible for assigning permissions and message visuals to players




#pragma once

#include "SQLiteCpp/Database.h"





class cRankManager
{
public:
	cRankManager(void);
	
	/** Returns the name of the rank that the specified player has assigned to them. */
	AString GetPlayerRankName(const AString & a_PlayerUUID);
	
	/** Returns the names of PermissionGroups that the specified player has assigned to them. */
	AStringVector GetPlayerPermissionGroups(const AString & a_PlayerUUID);
	
	/** Returns the permissions that the specified player has assigned to them. */
	AStringVector GetPlayerPermissions(const AString & a_PlayerUUID);
	
	/** Returns the names of groups that the specified rank has assigned to it. */
	AStringVector GetRankPermissionGroups(const AString & a_RankName);
	
	/** Returns the permissions that the specified permission group has assigned to it. */
	AStringVector GetPermissionGroupPermissiont(const AString & a_GroupName);
	
	/** Returns the names of all defined ranks. */
	AStringVector GetAllRanks(void);
	
	/** Returns the names of all permission groups. */
	AStringVector GetAllPermissionGroups(void);
	
	/** Returns all the distinct permissions that are stored in the DB. */
	AStringVector GetAllPermissions(void);
	
	/** Returns the message visuals (prefix, postfix, color) for the specified player. */
	void GetPlayerMsgVisuals(
		const AString & a_PlayerUUID,
		AString & a_MsgPrefix,
		AString & a_MsgPostfix,
		AString & a_MsgNameColorCode
	);
	
	/** Adds a new rank. No action if the rank already exists. */
	void AddRank(const AString & a_RankName);
	
	/** Adds a new permission group. No action if such a group already exists. */
	void AddPermissionGroup(const AString & a_GroupName);
	
	/** Adds the specified permission group to the specified rank.
	Fails if the rank or group names are not found.
	Returns true if successful, false on error. */
	bool AddPermissionGroupToRank(const AString & a_RankName, const AString & a_GroupName);
	
	/** Adds the specified permission to the specified permission group.
	Fails if the permission group name is not found.
	Returns true if successful, false on error. */
	bool AddPermissionToPermissionGroup(const AString & a_Permission, const AString & a_GroupName);
	
	/** Removes the specified rank.
	All players assigned to that rank will be re-assigned to a_ReplacementRankName, unless it is empty. */
	void RemoveRank(const AString & a_RankName, const AString & a_ReplacementRankName);
	
	/** Removes the specified permission group. */
	void RemovePermissionGroup(const AString & a_PermissionGroup);
	
	/** Removes the specified permission from the specified permission group. */
	void RemovePermission(const AString & a_Permission, const AString & a_PermissionGroup);
	
	/** Renames the specified rank. No action if the rank name is not found.
	Fails if the new name is already used.
	Returns true on success, false on failure. */
	bool RenameRank(const AString & a_OldName, const AString & a_NewName);
	
	/** Renames the specified permission group. No action if the rank name is not found.
	Fails if the new name is already used.
	Returns true on success, false on failure. */
	bool RenamePermissionGroup(const AString & a_OldName, const AString & a_NewName);
	
	/** Sets the specified player's rank.
	If the player already had rank assigned to them, it is overwritten with the new rank.
	Note that this doesn't change the cPlayer if the player is already connected, you need to update all the
	cPlayer instances manually. */
	void SetPlayerRank(const AString & a_PlayerUUID, const AString & a_RankName);
	
protected:

	SQLite::Database m_DB;
} ;




