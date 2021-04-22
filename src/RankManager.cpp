
// RankManager.cpp

// Implements the cRankManager class that represents the rank manager responsible for assigning permissions and message visuals to players

#include "Globals.h"
#include "RankManager.h"
#include "Protocol/MojangAPI.h"
#include "ClientHandle.h"





////////////////////////////////////////////////////////////////////////////////
// cRankManager:

cRankManager::cRankManager(void) :
	m_DB("Ranks.sqlite", SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE),
	m_IsInitialized(false),
	m_MojangAPI(nullptr)
{
}





cRankManager::~cRankManager()
{
	if (m_MojangAPI != nullptr)
	{
		m_MojangAPI->SetRankManager(nullptr);
	}
}





void cRankManager::Initialize(cMojangAPI & a_MojangAPI)
{
	ASSERT(!m_IsInitialized);  // Calling Initialize for the second time?

	// Create the DB tables, if they don't exist:
	m_DB.exec("CREATE TABLE IF NOT EXISTS Rank (RankID INTEGER PRIMARY KEY, Name, MsgPrefix, MsgSuffix, MsgNameColorCode)");
	m_DB.exec("CREATE TABLE IF NOT EXISTS PlayerRank (PlayerUUID, PlayerName, RankID INTEGER)");
	m_DB.exec("CREATE TABLE IF NOT EXISTS PermGroup (PermGroupID INTEGER PRIMARY KEY, Name)");
	m_DB.exec("CREATE TABLE IF NOT EXISTS RankPermGroup (RankID INTEGER, PermGroupID INTEGER)");
	m_DB.exec("CREATE TABLE IF NOT EXISTS PermissionItem (PermGroupID INTEGER, Permission)");
	m_DB.exec("CREATE TABLE IF NOT EXISTS RestrictionItem (PermGroupID INTEGER, Permission)");
	m_DB.exec("CREATE TABLE IF NOT EXISTS DefaultRank (RankID INTEGER)");

	m_IsInitialized = true;

	a_MojangAPI.SetRankManager(this);

	// If tables are empty, create default ranks
	if (AreDBTablesEmpty())
	{
		LOGINFO("Creating default ranks...");
		CreateDefaults();
		LOGINFO("Default ranks created.");
	}

	// Load the default rank:
	try
	{
		SQLite::Statement stmt(m_DB,
			"SELECT Rank.Name FROM Rank "
			"LEFT JOIN DefaultRank ON Rank.RankID = DefaultRank.RankID"
		);
		if (stmt.executeStep())
		{
			m_DefaultRank = stmt.getColumn(0).getText();
		}
	}
	catch (const SQLite::Exception & ex)
	{
		LOGWARNING("%s: Cannot load default rank: %s", __FUNCTION__, ex.what());
		return;
	}

	// If the default rank cannot be loaded, use the first rank:
	if (m_DefaultRank.empty())
	{
		SetDefaultRank(GetAllRanks()[0]);
	}
}





AString cRankManager::GetPlayerRankName(const cUUID & a_PlayerUUID)
{
	ASSERT(m_IsInitialized);
	cCSLock Lock(m_CS);

	try
	{
		SQLite::Statement stmt(m_DB, "SELECT Rank.Name FROM Rank LEFT JOIN PlayerRank ON Rank.RankID = PlayerRank.RankID WHERE PlayerRank.PlayerUUID = ?");
		stmt.bind(1, a_PlayerUUID.ToShortString());
		// executeStep returns false on no data
		if (!stmt.executeStep())
		{
			// No data returned from the DB
			return AString();
		}
		return stmt.getColumn(0).getText();
	}
	catch (const SQLite::Exception & ex)
	{
		LOGWARNING("%s: Cannot get player rank name: %s", __FUNCTION__, ex.what());
	}
	return AString();
}





AString cRankManager::GetPlayerName(const cUUID & a_PlayerUUID)
{
	ASSERT(m_IsInitialized);
	cCSLock Lock(m_CS);

	try
	{
		// Prepare the DB statement:
		SQLite::Statement stmt(m_DB, "SELECT PlayerName FROM PlayerRank WHERE PlayerUUID = ?");
		stmt.bind(1, a_PlayerUUID.ToShortString());

		if (stmt.executeStep())
		{
			return stmt.getColumn(0).getText();
		}
	}
	catch (SQLite::Exception & ex)
	{
		LOGWARNING("%s: Cannot get player name: %s", __FUNCTION__, ex.what());
	}
	return AString();
}





AStringVector cRankManager::GetPlayerGroups(const cUUID & a_PlayerUUID)
{
	ASSERT(m_IsInitialized);
	cCSLock Lock(m_CS);

	AStringVector res;
	try
	{
		// Prepare the DB statement:
		SQLite::Statement stmt(m_DB,
			"SELECT PermGroup.Name FROM PermGroup "
				"LEFT JOIN RankPermGroup ON PermGroup.PermGroupID = RankPermGroup.PermGroupID "
				"LEFT JOIN PlayerRank ON PlayerRank.RankID = RankPermGroup.RankID "
			"WHERE PlayerRank.PlayerUUID = ?"
		);
		stmt.bind(1, a_PlayerUUID.ToShortString());

		// Execute and get results:
		while (stmt.executeStep())
		{
			res.push_back(stmt.getColumn(0).getText());
		}
	}
	catch (const SQLite::Exception & ex)
	{
		LOGWARNING("%s: Cannot get player groups: %s", __FUNCTION__, ex.what());
	}
	return res;
}





AStringVector cRankManager::GetPlayerPermissions(const cUUID & a_PlayerUUID)
{
	AString Rank = GetPlayerRankName(a_PlayerUUID);
	if (Rank.empty())
	{
		Rank = m_DefaultRank;
	}
	return GetRankPermissions(Rank);
}





AStringVector cRankManager::GetPlayerRestrictions(const cUUID & a_PlayerUUID)
{
	AString Rank = GetPlayerRankName(a_PlayerUUID);
	if (Rank.empty())
	{
		Rank = m_DefaultRank;
	}
	return GetRankRestrictions(Rank);
}





AStringVector cRankManager::GetRankGroups(const AString & a_RankName)
{
	ASSERT(m_IsInitialized);
	cCSLock Lock(m_CS);

	AStringVector res;
	try
	{
		SQLite::Statement stmt(m_DB,
			"SELECT PermGroup.Name FROM PermGroup "
				"LEFT JOIN RankPermGroup ON RankPermGroup.PermGroupID = PermGroup.PermGroupID "
				"LEFT JOIN Rank ON Rank.RankID = RankPermGroup.RankID "
			"WHERE Rank.Name = ?"
		);
		stmt.bind(1, a_RankName);
		while (stmt.executeStep())
		{
			res.push_back(stmt.getColumn(0).getText());
		}
	}
	catch (const SQLite::Exception & ex)
	{
		LOGWARNING("%s: Failed to get rank groups from DB: %s", __FUNCTION__, ex.what());
	}
	return res;
}





AStringVector cRankManager::GetGroupPermissions(const AString & a_GroupName)
{
	ASSERT(m_IsInitialized);
	cCSLock Lock(m_CS);

	AStringVector res;
	try
	{
		SQLite::Statement stmt(m_DB,
			"SELECT PermissionItem.Permission FROM PermissionItem "
				"LEFT JOIN PermGroup ON PermGroup.PermGroupID = PermissionItem.PermGroupID "
			"WHERE PermGroup.Name = ?"
		);
		stmt.bind(1, a_GroupName);
		while (stmt.executeStep())
		{
			res.push_back(stmt.getColumn(0).getText());
		}
	}
	catch (const SQLite::Exception & ex)
	{
		LOGWARNING("%s: Failed to get group permissions from DB: %s", __FUNCTION__, ex.what());
	}
	return res;
}





AStringVector cRankManager::GetGroupRestrictions(const AString & a_GroupName)
{
	ASSERT(m_IsInitialized);
	cCSLock Lock(m_CS);

	AStringVector res;
	try
	{
		SQLite::Statement stmt(m_DB,
			"SELECT RestrictionItem.Permission FROM RestrictionItem "
				"LEFT JOIN PermGroup ON PermGroup.PermGroupID = RestrictionItem.PermGroupID "
			"WHERE PermGroup.Name = ?"
		);
		stmt.bind(1, a_GroupName);
		while (stmt.executeStep())
		{
			res.push_back(stmt.getColumn(0).getText());
		}
	}
	catch (const SQLite::Exception & ex)
	{
		LOGWARNING("%s: Failed to get group restrictions from DB: %s", __FUNCTION__, ex.what());
	}
	return res;
}





AStringVector cRankManager::GetRankPermissions(const AString & a_RankName)
{
	ASSERT(m_IsInitialized);
	cCSLock Lock(m_CS);

	AStringVector res;
	try
	{
		SQLite::Statement stmt(m_DB,
			"SELECT PermissionItem.Permission FROM PermissionItem "
				"LEFT JOIN RankPermGroup ON RankPermGroup.PermGroupID = PermissionItem.PermGroupID "
				"LEFT JOIN Rank ON Rank.RankID = RankPermGroup.RankID "
			"WHERE Rank.Name = ?"
		);
		stmt.bind(1, a_RankName);
		while (stmt.executeStep())
		{
			res.push_back(stmt.getColumn(0).getText());
		}
	}
	catch (const SQLite::Exception & ex)
	{
		LOGWARNING("%s: Failed to get rank permissions from DB: %s", __FUNCTION__, ex.what());
	}
	return res;
}





AStringVector cRankManager::GetRankRestrictions(const AString & a_RankName)
{
	ASSERT(m_IsInitialized);
	cCSLock Lock(m_CS);

	AStringVector res;
	try
	{
		SQLite::Statement stmt(m_DB,
			"SELECT RestrictionItem.Permission FROM RestrictionItem "
				"LEFT JOIN RankPermGroup ON RankPermGroup.PermGroupID = RestrictionItem.PermGroupID "
				"LEFT JOIN Rank ON Rank.RankID = RankPermGroup.RankID "
			"WHERE Rank.Name = ?"
		);
		stmt.bind(1, a_RankName);
		while (stmt.executeStep())
		{
			res.push_back(stmt.getColumn(0).getText());
		}
	}
	catch (const SQLite::Exception & ex)
	{
		LOGWARNING("%s: Failed to get rank restrictions from DB: %s", __FUNCTION__, ex.what());
	}
	return res;
}





std::vector<cUUID> cRankManager::GetAllPlayerUUIDs(void)
{
	ASSERT(m_IsInitialized);
	cCSLock Lock(m_CS);

	cUUID tempUUID;
	std::vector<cUUID> res;
	try
	{
		SQLite::Statement stmt(m_DB, "SELECT PlayerUUID FROM PlayerRank ORDER BY PlayerName COLLATE NOCASE");
		while (stmt.executeStep())
		{
			if (!tempUUID.FromString(stmt.getColumn(0).getText()))
			{
				// Invalid UUID, ignore
				continue;
			}
			res.push_back(tempUUID);
		}
	}
	catch (const SQLite::Exception & ex)
	{
		LOGWARNING("%s: Failed to get players from DB: %s", __FUNCTION__, ex.what());
	}
	return res;
}





AStringVector cRankManager::GetAllRanks(void)
{
	ASSERT(m_IsInitialized);
	cCSLock Lock(m_CS);

	AStringVector res;
	try
	{
		SQLite::Statement stmt(m_DB, "SELECT Name FROM Rank");
		while (stmt.executeStep())
		{
			res.push_back(stmt.getColumn(0).getText());
		}
	}
	catch (const SQLite::Exception & ex)
	{
		LOGWARNING("%s: Failed to get ranks from DB: %s", __FUNCTION__, ex.what());
	}
	return res;
}





AStringVector cRankManager::GetAllGroups(void)
{
	ASSERT(m_IsInitialized);
	cCSLock Lock(m_CS);

	AStringVector res;
	try
	{
		SQLite::Statement stmt(m_DB, "SELECT Name FROM PermGroup");
		while (stmt.executeStep())
		{
			res.push_back(stmt.getColumn(0).getText());
		}
	}
	catch (const SQLite::Exception & ex)
	{
		LOGWARNING("%s: Failed to get groups from DB: %s", __FUNCTION__, ex.what());
	}
	return res;
}





AStringVector cRankManager::GetAllPermissions(void)
{
	ASSERT(m_IsInitialized);
	cCSLock Lock(m_CS);

	AStringVector res;
	try
	{
		SQLite::Statement stmt(m_DB, "SELECT DISTINCT(Permission) FROM PermissionItem");
		while (stmt.executeStep())
		{
			res.push_back(stmt.getColumn(0).getText());
		}
	}
	catch (const SQLite::Exception & ex)
	{
		LOGWARNING("%s: Failed to get permissions from DB: %s", __FUNCTION__, ex.what());
	}
	return res;
}





AStringVector cRankManager::GetAllRestrictions(void)
{
	ASSERT(m_IsInitialized);
	cCSLock Lock(m_CS);

	AStringVector res;
	try
	{
		SQLite::Statement stmt(m_DB, "SELECT DISTINCT(Permission) FROM RestrictionItem");
		while (stmt.executeStep())
		{
			res.push_back(stmt.getColumn(0).getText());
		}
	}
	catch (const SQLite::Exception & ex)
	{
		LOGWARNING("%s: Failed to get restrictions from DB: %s", __FUNCTION__, ex.what());
	}
	return res;
}





AStringVector cRankManager::GetAllPermissionsRestrictions(void)
{
	AStringVector Permissions = GetAllPermissions();
	AStringVector Restrictions = GetAllRestrictions();
	for (auto & restriction: Restrictions)
	{
		Permissions.push_back(restriction);
	}
	return Permissions;
}





bool cRankManager::GetPlayerMsgVisuals(
	const cUUID & a_PlayerUUID,
	AString & a_MsgPrefix,
	AString & a_MsgSuffix,
	AString & a_MsgNameColorCode
)
{
	AString Rank = GetPlayerRankName(a_PlayerUUID);
	if (Rank.empty())
	{
		// Rank not found, return failure:
		a_MsgPrefix.clear();
		a_MsgSuffix.clear();
		a_MsgNameColorCode.clear();
		return false;
	}
	return GetRankVisuals(Rank, a_MsgPrefix, a_MsgSuffix, a_MsgNameColorCode);
}





void cRankManager::AddRank(
	const AString & a_RankName,
	const AString & a_MsgPrefix,
	const AString & a_MsgSuffix,
	const AString & a_MsgNameColorCode
)
{
	ASSERT(m_IsInitialized);
	cCSLock Lock(m_CS);

	try
	{
		// Check if such a rank name is already used:
		{
			SQLite::Statement stmt(m_DB, "SELECT COUNT(*) FROM Rank WHERE Name = ?");
			stmt.bind(1, a_RankName);
			if (stmt.executeStep())
			{
				if (stmt.getColumn(0).getInt() > 0)
				{
					// Rank already exists, do nothing:
					return;
				}
			}
		}

		// Insert a new rank:
		SQLite::Statement stmt(m_DB, "INSERT INTO Rank (Name, MsgPrefix, MsgSuffix, MsgNameColorCode) VALUES (?, ?, ?, ?)");
		stmt.bind(1, a_RankName);
		stmt.bind(2, a_MsgPrefix);
		stmt.bind(3, a_MsgSuffix);
		stmt.bind(4, a_MsgNameColorCode);
		if (stmt.exec() <= 0)
		{
			LOGWARNING("%s: Failed to add a new rank \"%s\".", __FUNCTION__, a_RankName.c_str());
			return;
		}
	}
	catch (const SQLite::Exception & ex)
	{
		LOGWARNING("%s: Failed to add a new rank \"%s\": %s", __FUNCTION__, a_RankName.c_str(), ex.what());
	}
}





void cRankManager::AddGroup(const AString & a_GroupName)
{
	ASSERT(m_IsInitialized);
	cCSLock Lock(m_CS);

	try
	{
		// Check if such a group name is already used:
		{
			SQLite::Statement stmt(m_DB, "SELECT COUNT(*) FROM PermGroup WHERE Name = ?");
			stmt.bind(1, a_GroupName);
			if (stmt.executeStep())
			{
				if (stmt.getColumn(0).getInt() > 0)
				{
					// Group already exists, do nothing:
					return;
				}
			}
		}

		// Insert a new group:
		SQLite::Statement stmt(m_DB, "INSERT INTO PermGroup (Name) VALUES (?)");
		stmt.bind(1, a_GroupName);
		if (stmt.exec() <= 0)
		{
			LOGWARNING("%s: Failed to add a new group \"%s\".", __FUNCTION__, a_GroupName.c_str());
			return;
		}
	}
	catch (const SQLite::Exception & ex)
	{
		LOGWARNING("%s: Failed to add a new group \"%s\": %s", __FUNCTION__, a_GroupName.c_str(), ex.what());
	}
}





void cRankManager::AddGroups(const AStringVector & a_GroupNames)
{
	ASSERT(m_IsInitialized);
	cCSLock Lock(m_CS);

	try
	{
		for (AStringVector::const_iterator itr = a_GroupNames.begin(), end = a_GroupNames.end(); itr != end; ++itr)
		{
			// Check if such the group name is already used:
			{
				SQLite::Statement stmt(m_DB, "SELECT COUNT(*) FROM PermGroup WHERE Name = ?");
				stmt.bind(1, *itr);
				if (stmt.executeStep())
				{
					if (stmt.getColumn(0).getInt() > 0)
					{
						// Group already exists, do nothing:
						return;
					}
				}
			}

			// Insert a new group:
			SQLite::Statement stmt(m_DB, "INSERT INTO PermGroup (Name) VALUES (?)");
			stmt.bind(1, *itr);
			if (stmt.exec() <= 0)
			{
				LOGWARNING("%s: Failed to add a new group \"%s\".", __FUNCTION__, itr->c_str());
				return;
			}
		}  // for itr - a_GroupNames[]
	}
	catch (const SQLite::Exception & ex)
	{
		LOGWARNING("%s: Failed to add new groups: %s", __FUNCTION__, ex.what());
	}
}





bool cRankManager::AddGroupToRank(const AString & a_GroupName, const AString & a_RankName)
{
	ASSERT(m_IsInitialized);
	cCSLock Lock(m_CS);

	try
	{
		// Get the group's ID:
		int GroupID;
		{
			SQLite::Statement stmt(m_DB, "SELECT PermGroupID FROM PermGroup WHERE Name = ?");
			stmt.bind(1, a_GroupName);
			if (!stmt.executeStep())
			{
				LOGWARNING("%s: No such group (%s), aborting.", __FUNCTION__, a_GroupName.c_str());
				return false;
			}
			GroupID = stmt.getColumn(0);
		}

		// Get the rank's ID:
		int RankID;
		{
			SQLite::Statement stmt(m_DB, "SELECT RankID FROM Rank WHERE Name = ?");
			stmt.bind(1, a_RankName);
			if (!stmt.executeStep())
			{
				LOGWARNING("%s: No such rank (%s), aborting.", __FUNCTION__, a_RankName.c_str());
				return false;
			}
			RankID = stmt.getColumn(0);
		}

		// Check if the group is already there:
		{
			SQLite::Statement stmt(m_DB, "SELECT COUNT(*) FROM RankPermGroup WHERE RankID = ? AND PermGroupID = ?");
			stmt.bind(1, RankID);
			stmt.bind(2, GroupID);
			if (!stmt.executeStep())
			{
				LOGWARNING("%s: Failed to check binding between rank %s and group %s, aborting.", __FUNCTION__, a_RankName.c_str(), a_GroupName.c_str());
				return false;
			}
			if (stmt.getColumn(0).getInt() > 0)
			{
				LOGD("%s: Group %s already present in rank %s, skipping and returning success.",
					__FUNCTION__, a_GroupName.c_str(), a_RankName.c_str()
				);
				return true;
			}
		}

		// Add the group:
		{
			SQLite::Statement stmt(m_DB, "INSERT INTO RankPermGroup (RankID, PermGroupID) VALUES (?, ?)");
			stmt.bind(1, RankID);
			stmt.bind(2, GroupID);
			if (stmt.exec() <= 0)
			{
				LOGWARNING("%s: Failed to add group %s to rank %s, aborting.", __FUNCTION__, a_GroupName.c_str(), a_RankName.c_str());
				return false;
			}
		}

		// Adding succeeded:
		return true;
	}
	catch (const SQLite::Exception & ex)
	{
		LOGWARNING("%s: Failed to add group %s to rank %s: %s", __FUNCTION__, a_GroupName.c_str(), a_RankName.c_str(), ex.what());
	}
	return false;
}





bool cRankManager::AddPermissionToGroup(const AString & a_Permission, const AString & a_GroupName)
{
	ASSERT(m_IsInitialized);
	cCSLock Lock(m_CS);

	try
	{
		// Get the group's ID:
		int GroupID;
		{
			SQLite::Statement stmt(m_DB, "SELECT PermGroupID FROM PermGroup WHERE Name = ?");
			stmt.bind(1, a_GroupName);
			if (!stmt.executeStep())
			{
				LOGWARNING("%s: No such group (%s), aborting.", __FUNCTION__, a_GroupName.c_str());
				return false;
			}
			GroupID = stmt.getColumn(0).getInt();
		}

		// Check if the permission is already present:
		{
			SQLite::Statement stmt(m_DB, "SELECT COUNT(*) FROM PermissionItem WHERE PermGroupID = ? AND Permission = ?");
			stmt.bind(1, GroupID);
			stmt.bind(2, a_Permission);
			if (!stmt.executeStep())
			{
				LOGWARNING("%s: Failed to check binding between permission %s and group %s, aborting.", __FUNCTION__, a_Permission.c_str(), a_GroupName.c_str());
				return false;
			}
			if (stmt.getColumn(0).getInt() > 0)
			{
				LOGD("%s: Permission %s is already present in group %s, skipping and returning success.",
					__FUNCTION__, a_Permission.c_str(), a_GroupName.c_str()
				);
				return true;
			}
		}

		// Add the permission:
		{
			SQLite::Statement stmt(m_DB, "INSERT INTO PermissionItem (Permission, PermGroupID) VALUES (?, ?)");
			stmt.bind(1, a_Permission);
			stmt.bind(2, GroupID);
			if (stmt.exec() <= 0)
			{
				LOGWARNING("%s: Failed to add permission %s to group %s, aborting.", __FUNCTION__, a_Permission.c_str(), a_GroupName.c_str());
				return false;
			}
		}

		// Adding succeeded:
		return true;
	}
	catch (const SQLite::Exception & ex)
	{
		LOGWARNING("%s: Failed to add permission %s to group %s: %s",
			__FUNCTION__, a_Permission.c_str(), a_GroupName.c_str(), ex.what()
		);
	}
	return false;
}





bool cRankManager::AddRestrictionToGroup(const AString & a_Restriction, const AString & a_GroupName)
{
	ASSERT(m_IsInitialized);
	cCSLock Lock(m_CS);

	try
	{
		// Get the group's ID:
		int GroupID;
		{
			SQLite::Statement stmt(m_DB, "SELECT PermGroupID FROM PermGroup WHERE Name = ?");
			stmt.bind(1, a_GroupName);
			if (!stmt.executeStep())
			{
				LOGWARNING("%s: No such group (%s), aborting.", __FUNCTION__, a_GroupName.c_str());
				return false;
			}
			GroupID = stmt.getColumn(0).getInt();
		}

		// Check if the restriction is already present:
		{
			SQLite::Statement stmt(m_DB, "SELECT COUNT(*) FROM RestrictionItem WHERE PermGroupID = ? AND Permission = ?");
			stmt.bind(1, GroupID);
			stmt.bind(2, a_Restriction);
			if (!stmt.executeStep())
			{
				LOGWARNING("%s: Failed to check binding between restriction %s and group %s, aborting.", __FUNCTION__, a_Restriction.c_str(), a_GroupName.c_str());
				return false;
			}
			if (stmt.getColumn(0).getInt() > 0)
			{
				LOGD("%s: Restriction %s is already present in group %s, skipping and returning success.",
					__FUNCTION__, a_Restriction.c_str(), a_GroupName.c_str()
				);
				return true;
			}
		}

		// Add the restriction:
		{
			SQLite::Statement stmt(m_DB, "INSERT INTO RestrictionItem (Permission, PermGroupID) VALUES (?, ?)");
			stmt.bind(1, a_Restriction);
			stmt.bind(2, GroupID);
			if (stmt.exec() <= 0)
			{
				LOGWARNING("%s: Failed to add restriction %s to group %s, aborting.", __FUNCTION__, a_Restriction.c_str(), a_GroupName.c_str());
				return false;
			}
		}

		// Adding succeeded:
		return true;
	}
	catch (const SQLite::Exception & ex)
	{
		LOGWARNING("%s: Failed to add restriction %s to group %s: %s",
			__FUNCTION__, a_Restriction.c_str(), a_GroupName.c_str(), ex.what()
		);
	}
	return false;
}





bool cRankManager::AddPermissionsToGroup(const AStringVector & a_Permissions, const AString & a_GroupName)
{
	ASSERT(m_IsInitialized);
	cCSLock Lock(m_CS);

	try
	{
		// Get the group's ID:
		int GroupID;
		{
			SQLite::Statement stmt(m_DB, "SELECT PermGroupID FROM PermGroup WHERE Name = ?");
			stmt.bind(1, a_GroupName);
			if (!stmt.executeStep())
			{
				LOGWARNING("%s: No such group (%s), aborting.", __FUNCTION__, a_GroupName.c_str());
				return false;
			}
			GroupID = stmt.getColumn(0).getInt();
		}

		for (AStringVector::const_iterator itr = a_Permissions.begin(), end = a_Permissions.end(); itr != end; ++itr)
		{
			// Check if the permission is already present:
			{
				SQLite::Statement stmt(m_DB, "SELECT COUNT(*) FROM PermissionItem WHERE PermGroupID = ? AND Permission = ?");
				stmt.bind(1, GroupID);
				stmt.bind(2, *itr);
				if (!stmt.executeStep())
				{
					LOGWARNING("%s: Failed to check binding between permission %s and group %s, aborting.", __FUNCTION__, itr->c_str(), a_GroupName.c_str());
					return false;
				}
				if (stmt.getColumn(0).getInt() > 0)
				{
					LOGD("%s: Permission %s is already present in group %s, skipping and returning success.",
						__FUNCTION__, itr->c_str(), a_GroupName.c_str()
					);
					continue;
				}
			}

			// Add the permission:
			{
				SQLite::Statement stmt(m_DB, "INSERT INTO PermissionItem (Permission, PermGroupID) VALUES (?, ?)");
				stmt.bind(1, *itr);
				stmt.bind(2, GroupID);
				if (stmt.exec() <= 0)
				{
					LOGWARNING("%s: Failed to add permission %s to group %s, skipping.", __FUNCTION__, itr->c_str(), a_GroupName.c_str());
					continue;
				}
			}
		}  // for itr - a_Permissions[]

		// Adding succeeded:
		return true;
	}
	catch (const SQLite::Exception & ex)
	{
		LOGWARNING("%s: Failed to add permissions to group %s: %s",
			__FUNCTION__, a_GroupName.c_str(), ex.what()
		);
	}
	return false;
}





bool cRankManager::AddRestrictionsToGroup(const AStringVector & a_Restrictions, const AString & a_GroupName)
{
	ASSERT(m_IsInitialized);
	cCSLock Lock(m_CS);

	try
	{
		// Get the group's ID:
		int GroupID;
		{
			SQLite::Statement stmt(m_DB, "SELECT PermGroupID FROM PermGroup WHERE Name = ?");
			stmt.bind(1, a_GroupName);
			if (!stmt.executeStep())
			{
				LOGWARNING("%s: No such group (%s), aborting.", __FUNCTION__, a_GroupName.c_str());
				return false;
			}
			GroupID = stmt.getColumn(0).getInt();
		}

		for (auto itr = a_Restrictions.cbegin(), end = a_Restrictions.cend(); itr != end; ++itr)
		{
			// Check if the restriction is already present:
			{
				SQLite::Statement stmt(m_DB, "SELECT COUNT(*) FROM RestrictionItem WHERE PermGroupID = ? AND Permission = ?");
				stmt.bind(1, GroupID);
				stmt.bind(2, *itr);
				if (!stmt.executeStep())
				{
					LOGWARNING("%s: Failed to check binding between restriction %s and group %s, aborting.", __FUNCTION__, itr->c_str(), a_GroupName.c_str());
					return false;
				}
				if (stmt.getColumn(0).getInt() > 0)
				{
					LOGD("%s: Restriction %s is already present in group %s, skipping and returning success.",
						__FUNCTION__, itr->c_str(), a_GroupName.c_str()
					);
					continue;
				}
			}

			// Add the permission:
			{
				SQLite::Statement stmt(m_DB, "INSERT INTO RestrictionItem (Permission, PermGroupID) VALUES (?, ?)");
				stmt.bind(1, *itr);
				stmt.bind(2, GroupID);
				if (stmt.exec() <= 0)
				{
					LOGWARNING("%s: Failed to add restriction %s to group %s, skipping.", __FUNCTION__, itr->c_str(), a_GroupName.c_str());
					continue;
				}
			}
		}  // for itr - a_Restrictions[]

		// Adding succeeded:
		return true;
	}
	catch (const SQLite::Exception & ex)
	{
		LOGWARNING("%s: Failed to add restrictions to group %s: %s",
			__FUNCTION__, a_GroupName.c_str(), ex.what()
		);
	}
	return false;
}





void cRankManager::RemoveRank(const AString & a_RankName, const AString & a_ReplacementRankName)
{
	ASSERT(m_IsInitialized);
	cCSLock Lock(m_CS);

	// Check if the default rank is being removed with a proper replacement:
	if ((a_RankName == m_DefaultRank) && !RankExists(a_ReplacementRankName))
	{
		LOGWARNING("%s: Cannot remove rank %s, it is the default rank and the replacement rank doesn't exist.", __FUNCTION__, a_RankName.c_str());
		return;
	}

	AStringVector res;
	try
	{
		// Get the RankID for the rank being removed:
		int RemoveRankID;
		{
			SQLite::Statement stmt(m_DB, "SELECT RankID FROM Rank WHERE Name = ?");
			stmt.bind(1, a_RankName);
			if (!stmt.executeStep())
			{
				LOGINFO("%s: Rank %s was not found. Skipping.", __FUNCTION__, a_RankName.c_str());
				return;
			}
			RemoveRankID = stmt.getColumn(0).getInt();
		}

		// Get the RankID for the replacement rank:
		int ReplacementRankID = -1;
		{
			SQLite::Statement stmt(m_DB, "SELECT RankID FROM Rank WHERE Name = ?");
			stmt.bind(1, a_ReplacementRankName);
			if (stmt.executeStep())
			{
				ReplacementRankID = stmt.getColumn(0).getInt();
			}
		}

		// Remove the rank's bindings to groups:
		{
			SQLite::Statement stmt(m_DB, "DELETE FROM RankPermGroup WHERE RankID = ?");
			stmt.bind(1, RemoveRankID);
			stmt.exec();
		}

		// Adjust players:
		if (ReplacementRankID == -1)
		{
			// No replacement, just delete all the players that have the rank:
			SQLite::Statement stmt(m_DB, "DELETE FROM PlayerRank WHERE RankID = ?");
			stmt.bind(1, RemoveRankID);
			stmt.exec();
		}
		else
		{
			// Replacement available, change all the player records:
			SQLite::Statement stmt(m_DB, "UPDATE PlayerRank SET RankID = ? WHERE RankID = ?");
			stmt.bind(1, ReplacementRankID);
			stmt.bind(2, RemoveRankID);
			stmt.exec();
		}

		// Remove the rank from the DB:
		{
			SQLite::Statement stmt(m_DB, "DELETE FROM Rank WHERE RankID = ?");
			stmt.bind(1, RemoveRankID);
			stmt.exec();
		}

		// Update the default rank, if it was the one being removed:
		if (a_RankName == m_DefaultRank)
		{
			m_DefaultRank = a_RankName;
		}
	}
	catch (const SQLite::Exception & ex)
	{
		LOGWARNING("%s: Failed to remove rank from DB: %s", __FUNCTION__, ex.what());
	}
}





void cRankManager::RemoveGroup(const AString & a_GroupName)
{
	ASSERT(m_IsInitialized);
	cCSLock Lock(m_CS);

	try
	{
		// Get the ID of the group:
		int GroupID;
		{
			SQLite::Statement stmt(m_DB, "SELECT PermGroupID FROM PermGroup WHERE Name = ?");
			stmt.bind(1, a_GroupName);
			if (!stmt.executeStep())
			{
				LOGINFO("%s: Group %s was not found, skipping.", __FUNCTION__, a_GroupName.c_str());
				return;
			}
			GroupID = stmt.getColumn(0).getInt();
		}

		// Remove all permissions from the group:
		{
			SQLite::Statement stmt(m_DB, "DELETE FROM PermissionItem WHERE PermGroupID = ?");
			stmt.bind(1, GroupID);
			stmt.exec();
		}

		// Remove the group from all ranks that contain it:
		{
			SQLite::Statement stmt(m_DB, "DELETE FROM RankPermGroup WHERE PermGroupID = ?");
			stmt.bind(1, GroupID);
			stmt.exec();
		}

		// Remove the group itself:
		{
			SQLite::Statement stmt(m_DB, "DELETE FROM PermGroup WHERE PermGroupID = ?");
			stmt.bind(1, GroupID);
			stmt.exec();
		}
	}
	catch (const SQLite::Exception & ex)
	{
		LOGWARNING("%s: Failed to remove group %s from DB: %s", __FUNCTION__, a_GroupName.c_str(), ex.what());
	}
}





void cRankManager::RemoveGroupFromRank(const AString & a_GroupName, const AString & a_RankName)
{
	ASSERT(m_IsInitialized);
	cCSLock Lock(m_CS);

	try
	{
		// Get the IDs of the group and the rank:
		int GroupID, RankID;
		{
			SQLite::Statement stmt(m_DB,
				"SELECT PermGroup.PermGroupID, Rank.RankID FROM PermGroup "
					"LEFT JOIN RankPermGroup ON RankPermGroup.PermGroupID = PermGroup.PermGroupID "
					"LEFT JOIN Rank ON Rank.RankID = RankPermGroup.RankID "
				"WHERE PermGroup.Name = ? AND Rank.Name = ?"
			);
			stmt.bind(1, a_GroupName);
			stmt.bind(2, a_RankName);
			if (!stmt.executeStep())
			{
				LOGINFO("%s: Group %s was not found in rank %s, skipping.", __FUNCTION__, a_GroupName.c_str(), a_RankName.c_str());
				return;
			}
			GroupID = stmt.getColumn(0).getInt();
			RankID = stmt.getColumn(1).getInt();
		}

		// Remove the group from all ranks that contain it:
		{
			SQLite::Statement stmt(m_DB, "DELETE FROM RankPermGroup WHERE PermGroupID = ?");
			stmt.bind(1, GroupID);
			stmt.exec();
		}

		// Remove the group-to-rank binding:
		{
			SQLite::Statement stmt(m_DB, "DELETE FROM RankPermGroup WHERE PermGroupID = ? AND RankID = ?");
			stmt.bind(1, GroupID);
			stmt.bind(1, RankID);
			stmt.exec();
		}
	}
	catch (const SQLite::Exception & ex)
	{
		LOGWARNING("%s: Failed to remove group %s from rank %s in the DB: %s", __FUNCTION__, a_GroupName.c_str(), a_RankName.c_str(), ex.what());
	}
}





void cRankManager::RemovePermissionFromGroup(const AString & a_Permission, const AString & a_GroupName)
{
	ASSERT(m_IsInitialized);
	cCSLock Lock(m_CS);

	try
	{
		// Get the ID of the group:
		int GroupID;
		{
			SQLite::Statement stmt(m_DB, "SELECT PermGroupID FROM PermGroup WHERE Name = ?");
			stmt.bind(1, a_GroupName);
			if (!stmt.executeStep())
			{
				LOGINFO("%s: Group %s was not found, skipping.", __FUNCTION__, a_GroupName.c_str());
				return;
			}
			GroupID = stmt.getColumn(0).getInt();
		}

		// Remove the permission from the group:
		{
			SQLite::Statement stmt(m_DB, "DELETE FROM PermissionItem WHERE PermGroupID = ? AND Permission = ?");
			stmt.bind(1, GroupID);
			stmt.bind(2, a_Permission);
			stmt.exec();
		}
	}
	catch (const SQLite::Exception & ex)
	{
		LOGWARNING("%s: Failed to remove permission %s from group %s in DB: %s",
			__FUNCTION__, a_Permission.c_str(), a_GroupName.c_str(), ex.what()
		);
	}
}





void cRankManager::RemoveRestrictionFromGroup(const AString & a_Restriction, const AString & a_GroupName)
{
	ASSERT(m_IsInitialized);
	cCSLock Lock(m_CS);

	try
	{
		// Get the ID of the group:
		int GroupID;
		{
			SQLite::Statement stmt(m_DB, "SELECT PermGroupID FROM PermGroup WHERE Name = ?");
			stmt.bind(1, a_GroupName);
			if (!stmt.executeStep())
			{
				LOGINFO("%s: Group %s was not found, skipping.", __FUNCTION__, a_GroupName.c_str());
				return;
			}
			GroupID = stmt.getColumn(0).getInt();
		}

		// Remove the permission from the group:
		{
			SQLite::Statement stmt(m_DB, "DELETE FROM RestrictionItem WHERE PermGroupID = ? AND Permission = ?");
			stmt.bind(1, GroupID);
			stmt.bind(2, a_Restriction);
			stmt.exec();
		}
	}
	catch (const SQLite::Exception & ex)
	{
		LOGWARNING("%s: Failed to remove restriction %s from group %s in DB: %s",
			__FUNCTION__, a_Restriction.c_str(), a_GroupName.c_str(), ex.what()
		);
	}
}





bool cRankManager::RenameRank(const AString & a_OldName, const AString & a_NewName)
{
	ASSERT(m_IsInitialized);
	cCSLock Lock(m_CS);

	try
	{
		// Check that NewName doesn't exist:
		{
			SQLite::Statement stmt(m_DB, "SELECT RankID FROM Rank WHERE Name = ?");
			stmt.bind(1, a_NewName);
			if (stmt.executeStep())
			{
				LOGINFO("%s: Rank %s is already present, cannot rename %s", __FUNCTION__, a_NewName.c_str(), a_OldName.c_str());
				return false;
			}
		}

		// Rename:
		{
			SQLite::Statement stmt(m_DB, "UPDATE Rank SET Name = ? WHERE Name = ?");
			stmt.bind(1, a_NewName);
			stmt.bind(2, a_OldName);
			if (stmt.exec() <= 0)
			{
				LOGINFO("%s: There is no rank %s, cannot rename to %s.", __FUNCTION__, a_OldName.c_str(), a_NewName.c_str());
				return false;
			}
		}

		// Update the default rank, if it was the one being renamed:
		if (a_OldName == m_DefaultRank)
		{
			m_DefaultRank = a_NewName;
		}

		return true;
	}
	catch (const SQLite::Exception & ex)
	{
		LOGWARNING("%s: Failed to rename rank %s to %s in DB: %s",
			__FUNCTION__, a_OldName.c_str(), a_NewName.c_str(), ex.what());
	}
	return false;
}





bool cRankManager::RenameGroup(const AString & a_OldName, const AString & a_NewName)
{
	ASSERT(m_IsInitialized);
	cCSLock Lock(m_CS);

	try
	{
		// Check that NewName doesn't exist:
		{
			SQLite::Statement stmt(m_DB, "SELECT PermGroupID FROM PermGroup WHERE Name = ?");
			stmt.bind(1, a_NewName);
			if (stmt.executeStep())
			{
				LOGD("%s: Group %s is already present, cannot rename %s", __FUNCTION__, a_NewName.c_str(), a_OldName.c_str());
				return false;
			}
		}

		// Rename:
		bool res;
		{
			SQLite::Statement stmt(m_DB, "UPDATE PermGroup SET Name = ? WHERE Name = ?");
			stmt.bind(1, a_NewName);
			stmt.bind(2, a_OldName);
			res = (stmt.exec() > 0);
		}

		return res;
	}
	catch (const SQLite::Exception & ex)
	{
		LOGWARNING("%s: Failed to rename group %s to %s in DB: %s",
			__FUNCTION__, a_OldName.c_str(), a_NewName.c_str(), ex.what());
	}
	return false;
}





void cRankManager::SetPlayerRank(const cUUID & a_PlayerUUID, const AString & a_PlayerName, const AString & a_RankName)
{
	ASSERT(m_IsInitialized);
	cCSLock Lock(m_CS);

	AString StrUUID = a_PlayerUUID.ToShortString();

	try
	{
		// Get the rank ID:
		int RankID;
		{
			SQLite::Statement stmt(m_DB, "SELECT RankID FROM Rank WHERE Name = ?");
			stmt.bind(1, a_RankName);
			if (!stmt.executeStep())
			{
				LOGWARNING("%s: There is no rank %s, aborting.", __FUNCTION__, a_RankName.c_str());
				return;
			}
			RankID = stmt.getColumn(0).getInt();
		}

		// Update the player's rank, if already in DB:
		{
			SQLite::Statement stmt(m_DB, "UPDATE PlayerRank SET RankID = ?, PlayerName = ? WHERE PlayerUUID = ?");
			stmt.bind(1, RankID);
			stmt.bind(2, a_PlayerName);
			stmt.bind(3, StrUUID);
			if (stmt.exec() > 0)
			{
				// Successfully updated the player's rank
				return;
			}
		}

		// The player is not yet in the DB, add them:
		SQLite::Statement stmt(m_DB, "INSERT INTO PlayerRank (RankID, PlayerUUID, PlayerName) VALUES (?, ?, ?)");
		stmt.bind(1, RankID);
		stmt.bind(2, StrUUID);
		stmt.bind(3, a_PlayerName);
		if (stmt.exec() > 0)
		{
			// Successfully added the player
			return;
		}

		LOGWARNING("%s: Failed to set player UUID %s to rank %s.",
			__FUNCTION__, StrUUID.c_str(), a_RankName.c_str()
		);
	}
	catch (const SQLite::Exception & ex)
	{
		LOGWARNING("%s: Failed to set player UUID %s to rank %s: %s",
			__FUNCTION__, StrUUID.c_str(), a_RankName.c_str(), ex.what()
		);
	}
}





void cRankManager::RemovePlayerRank(const cUUID & a_PlayerUUID)
{
	ASSERT(m_IsInitialized);
	cCSLock Lock(m_CS);

	AString StrUUID = a_PlayerUUID.ToShortString();

	try
	{
		SQLite::Statement stmt(m_DB, "DELETE FROM PlayerRank WHERE PlayerUUID = ?");
		stmt.bind(1, StrUUID);
		stmt.exec();
	}
	catch (const SQLite::Exception & ex)
	{
		LOGWARNING("%s: Failed to remove rank from player UUID %s: %s",
			__FUNCTION__, StrUUID.c_str(), ex.what()
		);
	}
}





void cRankManager::SetRankVisuals(
	const AString & a_RankName,
	const AString & a_MsgPrefix,
	const AString & a_MsgSuffix,
	const AString & a_MsgNameColorCode
)
{
	ASSERT(m_IsInitialized);
	cCSLock Lock(m_CS);

	try
	{
		SQLite::Statement stmt(m_DB, "UPDATE Rank SET MsgPrefix = ?, MsgSuffix = ?, MsgNameColorCode = ? WHERE Name = ?");
		stmt.bind(1, a_MsgPrefix);
		stmt.bind(2, a_MsgSuffix);
		stmt.bind(3, a_MsgNameColorCode);
		stmt.bind(4, a_RankName);
		if (stmt.exec() < 1)
		{
			LOGINFO("%s: Rank %s not found, visuals not set.", __FUNCTION__, a_RankName.c_str());
		}
	}
	catch (const SQLite::Exception & ex)
	{
		LOGWARNING("%s: Failed to get ranks from DB: %s", __FUNCTION__, ex.what());
	}
}





bool cRankManager::GetRankVisuals(
	const AString & a_RankName,
	AString & a_MsgPrefix,
	AString & a_MsgSuffix,
	AString & a_MsgNameColorCode
)
{
	ASSERT(m_IsInitialized);
	cCSLock Lock(m_CS);

	try
	{
		SQLite::Statement stmt(m_DB, "SELECT MsgPrefix, MsgSuffix, MsgNameColorCode FROM Rank WHERE Name = ?");
		stmt.bind(1, a_RankName);
		if (!stmt.executeStep())
		{
			// Rank not found
			return false;
		}
		a_MsgPrefix = stmt.getColumn(0).getText();
		a_MsgSuffix = stmt.getColumn(1).getText();
		a_MsgNameColorCode = stmt.getColumn(2).getText();
		return true;
	}
	catch (const SQLite::Exception & ex)
	{
		LOGWARNING("%s: Failed to get ranks from DB: %s", __FUNCTION__, ex.what());
	}
	return false;
}





bool cRankManager::RankExists(const AString & a_RankName)
{
	ASSERT(m_IsInitialized);
	cCSLock Lock(m_CS);

	try
	{
		SQLite::Statement stmt(m_DB, "SELECT * FROM Rank WHERE Name = ?");
		stmt.bind(1, a_RankName);
		if (stmt.executeStep())
		{
			// The rank was found
			return true;
		}
	}
	catch (const SQLite::Exception & ex)
	{
		LOGWARNING("%s: Failed to query DB for rank %s: %s", __FUNCTION__, a_RankName.c_str(), ex.what());
	}
	return false;
}





bool cRankManager::GroupExists(const AString & a_GroupName)
{
	ASSERT(m_IsInitialized);
	cCSLock Lock(m_CS);

	try
	{
		SQLite::Statement stmt(m_DB, "SELECT * FROM PermGroup WHERE Name = ?");
		stmt.bind(1, a_GroupName);
		if (stmt.executeStep())
		{
			// The group was found
			return true;
		}
	}
	catch (const SQLite::Exception & ex)
	{
		LOGWARNING("%s: Failed to query DB for group %s: %s", __FUNCTION__, a_GroupName.c_str(), ex.what());
	}
	return false;
}





bool cRankManager::IsPlayerRankSet(const cUUID & a_PlayerUUID)
{
	ASSERT(m_IsInitialized);
	cCSLock Lock(m_CS);

	AString StrUUID = a_PlayerUUID.ToShortString();

	try
	{
		SQLite::Statement stmt(m_DB, "SELECT * FROM PlayerRank WHERE PlayerUUID = ?");
		stmt.bind(1, StrUUID);
		if (stmt.executeStep())
		{
			// The player UUID was found, they have a rank
			return true;
		}
	}
	catch (const SQLite::Exception & ex)
	{
		LOGWARNING("%s: Failed to query DB for player UUID %s: %s", __FUNCTION__, StrUUID.c_str(), ex.what());
	}
	return false;
}





bool cRankManager::IsGroupInRank(const AString & a_GroupName, const AString & a_RankName)
{
	ASSERT(m_IsInitialized);
	cCSLock Lock(m_CS);

	try
	{
		SQLite::Statement stmt(m_DB,
			"SELECT * FROM Rank "
				"LEFT JOIN RankPermGroup ON Rank.RankID = RankPermGroup.RankID "
				"LEFT JOIN PermGroup ON PermGroup.PermGroupID = RankPermGroup.PermGroupID "
			"WHERE Rank.Name = ? AND PermGroup.Name = ?"
		);
		stmt.bind(1, a_RankName);
		stmt.bind(2, a_GroupName);
		if (stmt.executeStep())
		{
			// The group is in the rank
			return true;
		}
	}
	catch (const SQLite::Exception & ex)
	{
		LOGWARNING("%s: Failed to query DB: %s", __FUNCTION__, ex.what());
	}
	return false;
}





bool cRankManager::IsPermissionInGroup(const AString & a_Permission, const AString & a_GroupName)
{
	ASSERT(m_IsInitialized);
	cCSLock Lock(m_CS);

	try
	{
		SQLite::Statement stmt(m_DB,
			"SELECT * FROM PermissionItem "
				"LEFT JOIN PermGroup ON PermGroup.PermGroupID = PermissionItem.PermGroupID "
			"WHERE PermissionItem.Permission = ? AND PermGroup.Name = ?"
		);
		stmt.bind(1, a_Permission);
		stmt.bind(2, a_GroupName);
		if (stmt.executeStep())
		{
			// The permission is in the group
			return true;
		}
	}
	catch (const SQLite::Exception & ex)
	{
		LOGWARNING("%s: Failed to query DB: %s", __FUNCTION__, ex.what());
	}
	return false;
}





bool cRankManager::IsRestrictionInGroup(const AString & a_Restriction, const AString & a_GroupName)
{
	ASSERT(m_IsInitialized);
	cCSLock Lock(m_CS);

	try
	{
		SQLite::Statement stmt(m_DB,
			"SELECT * FROM RestrictionItem "
				"LEFT JOIN PermGroup ON PermGroup.PermGroupID = RestrictionItem.PermGroupID "
			"WHERE RestrictionItem.Permission = ? AND PermGroup.Name = ?"
		);
		stmt.bind(1, a_Restriction);
		stmt.bind(2, a_GroupName);
		if (stmt.executeStep())
		{
			// The restriction is in the group
			return true;
		}
	}
	catch (const SQLite::Exception & ex)
	{
		LOGWARNING("%s: Failed to query DB: %s", __FUNCTION__, ex.what());
	}
	return false;
}





void cRankManager::NotifyNameUUID(const AString & a_PlayerName, const cUUID & a_UUID)
{
	ASSERT(m_IsInitialized);
	cCSLock Lock(m_CS);

	try
	{
		SQLite::Statement stmt(m_DB, "UPDATE PlayerRank SET PlayerName = ? WHERE PlayerUUID = ?");
		stmt.bind(1, a_PlayerName);
		stmt.bind(2, a_UUID.ToShortString());
		stmt.exec();
	}
	catch (const SQLite::Exception & ex)
	{
		LOGWARNING("%s: Failed to update DB: %s", __FUNCTION__, ex.what());
	}
}





bool cRankManager::SetDefaultRank(const AString & a_RankName)
{
	ASSERT(m_IsInitialized);
	cCSLock Lock(m_CS);

	try
	{
		// Find the rank's ID:
		int RankID = 0;
		{
			SQLite::Statement stmt(m_DB, "SELECT RankID FROM Rank WHERE Name = ?");
			stmt.bind(1, a_RankName);
			if (!stmt.executeStep())
			{
				LOGINFO("%s: Cannot set rank %s as the default, it does not exist.", __FUNCTION__, a_RankName.c_str());
				return false;
			}
		}

		// Set the rank as the default:
		{
			SQLite::Statement stmt(m_DB, "UPDATE DefaultRank SET RankID = ?");
			stmt.bind(1, RankID);
			if (stmt.exec() < 1)
			{
				// Failed to update, there might be none in the DB, try inserting:
				SQLite::Statement stmt2(m_DB, "INSERT INTO DefaultRank (RankID) VALUES (?)");
				stmt2.bind(1, RankID);
				if (stmt2.exec() < 1)
				{
					LOGINFO("%s: Cannot update the default rank in the DB to %s.", __FUNCTION__, a_RankName.c_str());
					return false;
				}
			}
		}

		// Set the internal cache:
		m_DefaultRank = a_RankName;
		return true;
	}
	catch (const SQLite::Exception & ex)
	{
		LOGWARNING("%s: Failed to update DB: %s", __FUNCTION__, ex.what());
		return false;
	}
}





void cRankManager::ClearPlayerRanks(void)
{
	ASSERT(m_IsInitialized);
	cCSLock Lock(m_CS);

	try
	{
		SQLite::Statement stmt(m_DB, "DELETE FROM PlayerRank");
		stmt.exec();
	}
	catch (SQLite::Exception & ex)
	{
		LOGWARNING("%s: Failed to remove / clear all players: %s", __FUNCTION__, ex.what());
	}
}





bool cRankManager::UpdatePlayerName(const cUUID & a_PlayerUUID, const AString & a_NewPlayerName)
{
	ASSERT(m_IsInitialized);
	cCSLock Lock(m_CS);

	AString StrUUID = a_PlayerUUID.ToShortString();

	try
	{
		SQLite::Statement stmt(m_DB, "UPDATE PlayerRank SET PlayerName = ? WHERE PlayerUUID = ?");
		stmt.bind(1, a_NewPlayerName);
		stmt.bind(2, StrUUID);
		if (stmt.exec() > 0)
		{
			// The player name was changed, returns true
			return true;
		}
	}
	catch (const SQLite::Exception & ex)
	{
		LOGWARNING("%s: Failed to update player name from UUID %s: %s", __FUNCTION__, StrUUID.c_str(), ex.what());
	}
	return false;
}





bool cRankManager::AreDBTablesEmpty(void)
{
	return (
		IsDBTableEmpty("Rank") &&
		IsDBTableEmpty("PlayerRank") &&
		IsDBTableEmpty("PermGroup") &&
		IsDBTableEmpty("RankPermGroup") &&
		IsDBTableEmpty("PermissionItem") &&
		IsDBTableEmpty("DefaultRank")
	);
}





bool cRankManager::IsDBTableEmpty(const AString & a_TableName)
{
	try
	{
		SQLite::Statement stmt(m_DB, "SELECT COUNT(*) FROM " + a_TableName);
		return (stmt.executeStep() && (stmt.getColumn(0).getInt() == 0));
	}
	catch (const SQLite::Exception & ex)
	{
		LOGWARNING("%s: Failed to query DB: %s", __FUNCTION__, ex.what());
	}
	return false;
}





void cRankManager::CreateDefaults(void)
{
	// Wrap everything in a big transaction to speed things up:
	cMassChangeLock Lock(*this);

	// Create ranks:
	AddRank("Default",  "", "", "");
	AddRank("VIP",      "", "", "");
	AddRank("Operator", "", "", "");
	AddRank("Admin",    "", "", "");

	// Create groups:
	AddGroup("Default");
	AddGroup("Kick");
	AddGroup("Teleport");
	AddGroup("Everything");

	// Add groups to ranks:
	AddGroupToRank("Default",    "Default");
	AddGroupToRank("Teleport",   "VIP");
	AddGroupToRank("Teleport",   "Operator");
	AddGroupToRank("Kick",       "Operator");
	AddGroupToRank("Everything", "Admin");

	// Add permissions to groups:
	AddPermissionToGroup("core.help",     "Default");
	AddPermissionToGroup("core.build",    "Default");
	AddPermissionToGroup("core.teleport", "Teleport");
	AddPermissionToGroup("core.kick",     "Kick");
	AddPermissionToGroup("*",             "Everything");

	// Set the default rank:
	SetDefaultRank("Default");
}





bool cRankManager::DoesColumnExist(const char * a_TableName, const char * a_ColumnName)
{
	try
	{
		SQLite::Statement stmt(m_DB, Printf("PRAGMA table_info(%s)", a_TableName));
		while (stmt.executeStep())  // Iterate over all table's columns
		{
			int NumColumns = stmt.getColumnCount();
			for (int i = 0; i < NumColumns; i++)  // Iterate over all reply's columns (table column's metadata)
			{
				auto column = stmt.getColumn(i);
				if (strcmp(column.getName(), "name") == 0)
				{
					if (NoCaseCompare(column.getText(), a_ColumnName) == 0)
					{
						// Colun found
						return true;
					}
				}
			}  // for i - stmt.getColumns()
		}  // while (stmt.executeStep())
	}
	catch (const SQLite::Exception & ex)
	{
		LOGWARNING("%s: Failed to query DB: %s", __FUNCTION__, ex.what());
	}
	return false;
}





void cRankManager::CreateColumnIfNotExists(const char * a_TableName, const char * a_ColumnName, const char * a_ColumnType)
{
	// If the column already exists, bail out:
	if (DoesColumnExist(a_TableName, a_ColumnName))
	{
		return;
	}

	// Add the column:
	try
	{
		m_DB.exec(Printf("ALTER TABLE %s ADD COLUMN %s %s", a_TableName, a_ColumnName, a_ColumnType));
	}
	catch (const SQLite::Exception & exc)
	{
		LOGWARNING("%s: Failed to query DB: %s", __FUNCTION__, exc.what());
	}
}




