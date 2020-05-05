
// RankManager.h

// Declares the cRankManager class that represents the rank manager responsible for assigning permissions and message visuals to players




#pragma once

#include "SQLiteCpp/Database.h"
#include "SQLiteCpp/Transaction.h"




class cUUID;
class cMojangAPI;





class cRankManager
{
public:
	/** Acquire this lock to perform mass changes.
	Improves performance by wrapping everything into a transaction.
	Makes sure that no other thread is accessing the DB. */
	class cMassChangeLock
	{
	public:
		cMassChangeLock(cRankManager & a_RankManager) :
			m_Lock(a_RankManager.m_CS),
			m_Transaction(a_RankManager.m_DB)
		{
		}

		~cMassChangeLock()
		{
			m_Transaction.commit();
		}

	protected:
		cCSLock m_Lock;
		SQLite::Transaction m_Transaction;
	};


	/** Creates the rank manager. Needs to be initialized before other use. */
	cRankManager(void);

	~cRankManager();

	/** Initializes the rank manager. Performs migration and default-setting if no data is found in the DB.
	The a_MojangAPI param is used to keep player names in sync with UUIDs, since Mojang allows username changes. */
	void Initialize(cMojangAPI & a_MojangAPI);

	/** Returns the name of the rank that the specified player has assigned to them.
	If the player has no rank assigned, returns an empty string (NOT the default rank). */
	AString GetPlayerRankName(const cUUID & a_PlayerUUID);

	/** Returns the last name that the specified player has.
	An empty string is returned if the player isn't in the database. */
	AString GetPlayerName(const cUUID & a_PlayerUUID);

	/** Returns the names of Groups that the specified player has assigned to them. */
	AStringVector GetPlayerGroups(const cUUID & a_PlayerUUID);

	/** Returns the permissions that the specified player has assigned to them.
	If the player has no rank assigned to them, returns the default rank's permissions. */
	AStringVector GetPlayerPermissions(const cUUID & a_PlayerUUID);

	/** Returns the restrictions that the specified player has assigned to them.
	If the player has no rank assigned to them, returns the default rank's restrictions. */
	AStringVector GetPlayerRestrictions(const cUUID & a_PlayerUUID);

	/** Returns the names of groups that the specified rank has assigned to it.
	Returns an empty vector if the rank doesn't exist. */
	AStringVector GetRankGroups(const AString & a_RankName);

	/** Returns the permissions that the specified group has assigned to it.
	Returns an empty vector if the group doesn't exist. */
	AStringVector GetGroupPermissions(const AString & a_GroupName);

	/** Returns the restrictions that the specified group has assigned to it.
	Returns an empty vector if the group doesn't exist. */
	AStringVector GetGroupRestrictions(const AString & a_GroupName);

	/** Returns all permissions that the specified rank has assigned to it, through all its groups.
	Returns an empty vector if the rank doesn't exist. Any non-existent groups are ignored. */
	AStringVector GetRankPermissions(const AString & a_RankName);

	/** Returns all restrictions that the specified rank has assigned to it, through all its groups.
	Returns an empty vector if the rank doesn't exist. Any non-existent groups are ignored. */
	AStringVector GetRankRestrictions(const AString & a_RankName);

	/** Returns the uuids of all defined players. The returned players are ordered by their name (NOT their UUIDs). */
	std::vector<cUUID> GetAllPlayerUUIDs(void);

	/** Returns the names of all defined ranks. */
	AStringVector GetAllRanks(void);

	/** Returns the names of all permission groups. */
	AStringVector GetAllGroups(void);

	/** Returns all the distinct permissions that are stored in the DB. */
	AStringVector GetAllPermissions(void);

	/** Returns all the distinct restrictions that are stored in the DB. */
	AStringVector GetAllRestrictions(void);

	/** Returns all the distinct permissions and restrictions that are stored in the DB. */
	AStringVector GetAllPermissionsRestrictions(void);

	/** Returns the message visuals (prefix, postfix, color) for the specified player.
	Returns true if the visuals were read from the DB, false if not (player not found etc). */
	bool GetPlayerMsgVisuals(
		const cUUID & a_PlayerUUID,
		AString & a_MsgPrefix,
		AString & a_MsgSuffix,
		AString & a_MsgNameColorCode
	);

	/** Adds a new rank. No action if the rank already exists. */
	void AddRank(
		const AString & a_RankName,
		const AString & a_MsgPrefix,
		const AString & a_MsgSuffix,
		const AString & a_MsgNameColorCode
	);

	/** Adds a new permission group. No action if such a group already exists. */
	void AddGroup(const AString & a_GroupName);

	/** Bulk-adds groups. Group names that already exist are silently skipped. */
	void AddGroups(const AStringVector & a_GroupNames);

	/** Adds the specified permission group to the specified rank.
	Fails if the rank or group names are not found.
	Returns true if successful, false on error. */
	bool AddGroupToRank(const AString & a_GroupName, const AString & a_RankName);

	/** Adds the specified permission to the specified permission group.
	Fails if the permission group name is not found.
	Returns true if successful, false on error. */
	bool AddPermissionToGroup(const AString & a_Permission, const AString & a_GroupName);

	/** Adds the specified restriction to the specified group.
	Fails if the group name is not found.
	Returns true if successful, false on error. */
	bool AddRestrictionToGroup(const AString & a_Restriction, const AString & a_GroupName);

	/** Adds the specified permissions to the specified permission group.
	Fails if the permission group name is not found.
	Returns true if successful, false on error. */
	bool AddPermissionsToGroup(const AStringVector & a_Permissions, const AString & a_GroupName);

	/** Adds the specified restrictions to the specified group.
	Fails if the group name is not found.
	Returns true if successful, false on error. */
	bool AddRestrictionsToGroup(const AStringVector & a_Restrictions, const AString & a_GroupName);

	/** Removes the specified rank.
	All players assigned to that rank will be re-assigned to a_ReplacementRankName.
	If a_ReplacementRankName is empty or not a valid rank, the player will be removed from the DB,
	which means they will receive the default rank the next time they are queried.
	If the rank being removed is the default rank, the default will be changed to the replacement
	rank; the operation fails silently if there's no replacement. */
	void RemoveRank(const AString & a_RankName, const AString & a_ReplacementRankName);

	/** Removes the specified group completely.
	The group will first be removed from all ranks using it, and then removed itself. */
	void RemoveGroup(const AString & a_GroupName);

	/** Removes the specified group from the specified rank.
	The group will stay defined, even if no rank is using it. */
	void RemoveGroupFromRank(const AString & a_GroupName, const AString & a_RankName);

	/** Removes the specified permission from the specified group. */
	void RemovePermissionFromGroup(const AString & a_Permission, const AString & a_GroupName);

	/** Removes the specified restriction from the specified group. */
	void RemoveRestrictionFromGroup(const AString & a_Restriction, const AString & a_GroupName);

	/** Renames the specified rank. No action if the rank name is not found.
	Fails if the new name is already used.
	Updates the cached m_DefaultRank if the default rank is being renamed.
	Returns true on success, false on failure. */
	bool RenameRank(const AString & a_OldName, const AString & a_NewName);

	/** Renames the specified group. No action if the rank name is not found.
	Fails if the new name is already used.
	Returns true on success, false on failure. */
	bool RenameGroup(const AString & a_OldName, const AString & a_NewName);

	/** Sets the specified player's rank.
	If the player already had rank assigned to them, it is overwritten with the new rank and name.
	Note that this doesn't change the cPlayer if the player is already connected, you need to update all the
	cPlayer instances manually.
	The PlayerName is provided for reference, so that GetRankPlayerNames() can work. */
	void SetPlayerRank(const cUUID & a_PlayerUUID, const AString & a_PlayerName, const AString & a_RankName);

	/** Removes the player's rank assignment. The player is left without a rank.
	Note that this doesn't change the cPlayer instances for the already connected players, you need to update
	all the instances manually.
	No action if the player has no rank assigned to them already. */
	void RemovePlayerRank(const cUUID & a_PlayerUUID);

	/** Sets the message visuals of an existing rank. No action if the rank name is not found. */
	void SetRankVisuals(
		const AString & a_RankName,
		const AString & a_MsgPrefix,
		const AString & a_MsgSuffix,
		const AString & a_MsgNameColorCode
	);

	/** Returns the message visuals of an existing rank.
	Returns true if successful, false on error (rank doesn't exist). */
	bool GetRankVisuals(
		const AString & a_RankName,
		AString & a_MsgPrefix,
		AString & a_MsgSuffix,
		AString & a_MsgNameColorCode
	);

	/** Returns true iff the specified rank exists in the DB. */
	bool RankExists(const AString & a_RankName);

	/** Returns true iff the specified group exists in the DB. */
	bool GroupExists(const AString & a_GroupName);

	/** Returns true iff the specified player has a rank assigned to them in the DB. */
	bool IsPlayerRankSet(const cUUID & a_PlayerUUID);

	/** Returns true iff the specified rank contains the specified group. */
	bool IsGroupInRank(const AString & a_GroupName, const AString & a_RankName);

	/** Returns true iff the specified group contains the specified permission. */
	bool IsPermissionInGroup(const AString & a_Permission, const AString & a_GroupName);

	/** Returns true iff the specified group contains the specified restriction. */
	bool IsRestrictionInGroup(const AString & a_Restriction, const AString & a_GroupName);

	/** Called by cMojangAPI whenever the playername-uuid pairing is discovered. Updates the DB. */
	void NotifyNameUUID(const AString & a_PlayerName, const cUUID & a_UUID);

	/** Sets the specified rank as the default rank.
	Returns true on success, false on failure (rank not found). */
	bool SetDefaultRank(const AString & a_RankName);

	/** Returns the name of the default rank. */
	const AString & GetDefaultRank(void) const { return m_DefaultRank; }

	/** Removes all player ranks from the database. Note that this doesn't change the cPlayer instances
	for the already connected players, you need to update all the instances manually. */
	void ClearPlayerRanks(void);

	/** Updates the playername that is saved with this uuid. Returns false if a error occurred */
	bool UpdatePlayerName(const cUUID & a_PlayerUUID, const AString & a_NewPlayerName);

protected:

	/** The database storage for all the data. Protected by m_CS. */
	SQLite::Database m_DB;

	/** The name of the default rank. Kept as a cache so that queries for it don't need to go through the DB. */
	AString m_DefaultRank;

	/** The mutex protecting m_DB and m_DefaultRank against multi-threaded access. */
	cCriticalSection m_CS;

	/** Set to true once the manager is initialized. */
	bool m_IsInitialized;

	/** The MojangAPI instance that is used for keeping player names and UUIDs in sync.
	Set in Initialize(), may be nullptr. */
	cMojangAPI * m_MojangAPI;


	/** Returns true if all the DB tables are empty, indicating a fresh new install. */
	bool AreDBTablesEmpty(void);

	/** Returns true iff the specified DB table is empty.
	If there's an error while querying, returns false. */
	bool IsDBTableEmpty(const AString & a_TableName);

	/** Creates a default set of ranks / groups / permissions. */
	void CreateDefaults(void);

	/** Returns true if the specified column exists in the specified table. */
	bool DoesColumnExist(const char * a_TableName, const char * a_ColumnName);

	/** If the specified table doesn't contain the specified column, it is added to the table.
	The column type is used only when creating the column, it is not used when checking for existence. */
	void CreateColumnIfNotExists(const char * a_TableName, const char * a_ColumnName, const char * a_ColumnType = "");
} ;




