
// RankManager.cpp

// Implements the cRankManager class that represents the rank manager responsible for assigning permissions and message visuals to players

#include "Globals.h"
#include "RankManager.h"
#include "SQLiteCpp/Transaction.h"





/*
// This code is for internal testing while developing the cRankManager class
static class cRankMgrTest
{
public:
	cRankMgrTest(void) :
		m_Mgr()
	{
		// Initialize logging:
		new cMCLogger();
		AString UUID = "b1caf24202a841a78055a079c460eee7";  // UUID for "xoft"
		LOG("Testing UUID %s", UUID.c_str());
		
		// Test the initial state of the ranks:
		LOG("Initial test:");
		ReportPlayer(UUID);
		
		// Add a rank, a few groups and permissions and set the player to use them:
		LOG("Adding data...");
		m_Mgr.AddRank("TestRank1", "[test]", "[/test]", "7");
		m_Mgr.AddRank("TestRank2", "[t2]", "[/t2]", "8");
		m_Mgr.AddGroup("TestGroup1");
		m_Mgr.AddGroup("TestGroup2");
		m_Mgr.AddGroupToRank("TestGroup1", "TestRank1");
		m_Mgr.AddGroupToRank("TestGroup2", "TestRank1");
		m_Mgr.AddGroupToRank("TestGroup2", "TestRank2");
		m_Mgr.AddPermissionToGroup("testpermission1.1", "TestGroup1");
		m_Mgr.AddPermissionToGroup("testpermission1.2", "TestGroup1");
		m_Mgr.AddPermissionToGroup("testpermission1.3", "TestGroup1");
		m_Mgr.AddPermissionToGroup("common", "TestGroup1");
		m_Mgr.AddPermissionToGroup("testpermission2.1", "TestGroup2");
		m_Mgr.AddPermissionToGroup("common", "TestGroup2");
		m_Mgr.SetPlayerRank(UUID, "xoft", "TestRank1");
		
		// Test the added data:
		LOG("Testing the added data:");
		LOG("IsGroupInRank(TestGroup1, TestRank1) = %s", m_Mgr.IsGroupInRank("TestGroup1", "TestRank1") ? "true" : "false");
		LOG("IsGroupInRank(TestGroup3, TestRank1) = %s", m_Mgr.IsGroupInRank("TestGroup3", "TestRank1") ? "true" : "false");
		LOG("IsPermissionInGroup(testpermission1.2, TestGroup1) = %s", m_Mgr.IsPermissionInGroup("testpermission1.2", "TestGroup1") ? "true" : "false");  // Existing permission, in group
		LOG("IsPermissionInGroup(testpermission1.2, TestGroup2) = %s", m_Mgr.IsPermissionInGroup("testpermission1.2", "TestGroup2") ? "true" : "false");  // Existing permission, not in group
		LOG("IsPermissionInGroup(testpermission1.9, TestGroup2) = %s", m_Mgr.IsPermissionInGroup("testpermission1.9", "TestGroup2") ? "true" : "false");  // Non-existing permission
		LOG("IsPlayerRankSet(%s) = %s",  UUID.c_str(), m_Mgr.IsPlayerRankSet(UUID) ? "true" : "false");
		LOG("IsPlayerRankSet(%s1) = %s", UUID.c_str(), m_Mgr.IsPlayerRankSet(UUID + "1") ? "true" : "false");
		LOG("GroupExists(TestGroup1) = %s", m_Mgr.GroupExists("TestGroup1") ? "true" : "false");
		LOG("GroupExists(TestGroup3) = %s", m_Mgr.GroupExists("TestGroup3") ? "true" : "false");
		LOG("RankExists(TestRank1) = %s",        m_Mgr.RankExists("TestRank1") ? "true" : "false");
		LOG("RankExists(NonexistentRank) = %s", m_Mgr.RankExists("NonexistentRank") ? "true" : "false");
		ReportRankGroups("TestRank1");
		ReportRankGroups("NonexistentRank");
		ReportGroupPermissions("TestGroup1");
		ReportGroupPermissions("NonexistentGroup");
		
		// Report the contents of the DB:
		ReportAll();
		
		// Test the assignments above:
		LOG("After-assignment test:");
		ReportPlayer(UUID);
		
		// Test removing a permission from a group:
		LOG("Removing permission testpermission1.3 from group TestGroup1.");
		m_Mgr.RemovePermissionFromGroup("testpermission1.3", "TestGroup1");
		ReportGroupPermissions("TestGroup1");
		LOG("Removing permission common from group TestGroup1.");
		m_Mgr.RemovePermissionFromGroup("common", "TestGroup1");
		ReportGroupPermissions("TestGroup1");  // Check that it's not present
		ReportGroupPermissions("TestGroup2");  // Check that it's still present here
		
		// Test removing a group from rank:
		LOG("Removing group TestGroup2 from rank TestRank1.");
		m_Mgr.RemoveGroupFromRank("TestGroup2", "TestRank1");
		ReportRankGroups("TestRank1");
		LOG("Removing group TestGroup3 from rank TestRank1.");
		m_Mgr.RemoveGroupFromRank("TestGroup3", "TestRank1");
		ReportRankGroups("TestRank1");
		
		// Test re-adding the groups:
		LOG("Re-adding groups to TestRank1.");
		m_Mgr.AddGroupToRank("TestGroup1", "TestRank1");
		m_Mgr.AddGroupToRank("TestGroup2", "TestRank1");
		ReportRankGroups("TestRank1");
		
		// Test removing a group altogether:
		LOG("Removing group TestGroup2");
		m_Mgr.RemoveGroup("TestGroup2");
		ReportAll();
		
		// Test removing a rank:
		LOG("Removing rank TestRank2, replacing with rank TestRank1.");
		m_Mgr.RemoveRank("TestRank2", "TestRank1");
		ReportAll();
		LOG("Removing rank Test altogether.");
		m_Mgr.RemoveRank("Test", "");
		ReportAll();

		// Test renaming a rank:
		LOG("Renaming rank TestRank1 to Test");
		m_Mgr.RenameRank("TestRank1", "Test");
		ReportRankGroups("TestRank1");
		ReportRankGroups("Test");
		LOG("Player after renaming:");
		ReportPlayer(UUID);
		
		// Test renaming a group:
		LOG("Renaming group TestGroup1 to Test");
		m_Mgr.RenameGroup("TestGroup1", "Test");
		ReportGroupPermissions("TestGroup1");
		ReportGroupPermissions("Test");
		LOG("Player after renaming:");
		ReportPlayer(UUID);
		m_Mgr.RenameGroup("Test", "TestGroup1");

		// Test removing the rank in favor of another one:
		m_Mgr.RemoveRank("Test", "TestRank2");
		LOG("After-removal test:");
		ReportPlayer(UUID);
		
		LOG("Done.");
	}
	
	
	void ReportAll(void)
	{
		// Report all ranks:
		AStringVector Ranks = m_Mgr.GetAllRanks();
		LOG("All ranks (%u):", (unsigned)Ranks.size());
		for (AStringVector::const_iterator itr = Ranks.begin(), end = Ranks.end(); itr != end; ++itr)
		{
			LOG("  '%s'", itr->c_str());
		}
		
		// Report all groups:
		AStringVector Groups = m_Mgr.GetAllGroups();
		LOG("All groups (%u):", (unsigned)Groups.size());
		for (AStringVector::const_iterator itr = Groups.begin(), end = Groups.end(); itr != end; ++itr)
		{
			LOG("  '%s'", itr->c_str());
		}
		
		// Report all permissions:
		AStringVector Permissions = m_Mgr.GetAllPermissions();
		LOG("All permissions (%u):", (unsigned)Permissions.size());
		for (AStringVector::const_iterator itr = Permissions.begin(), end = Permissions.end(); itr != end; ++itr)
		{
			LOG("  '%s'", itr->c_str());
		}
	}
	
	
	void ReportPlayer(const AString & a_PlayerUUID)
	{
		// Get the player's UUID and rank:
		LOG("  Rank: '%s'", m_Mgr.GetPlayerRankName(a_PlayerUUID).c_str());

		// List all the permission groups for the player:
		AStringVector Groups = m_Mgr.GetPlayerGroups(a_PlayerUUID);
		LOG("  Groups (%u):", (unsigned)Groups.size());
		for (AStringVector::const_iterator itr = Groups.begin(), end = Groups.end(); itr != end; ++itr)
		{
			LOG("    '%s'" , itr->c_str());
		}  // for itr - Groups[]
		
		// List all the permissions for the player:
		AStringVector Permissions = m_Mgr.GetPlayerPermissions(a_PlayerUUID);
		LOG("  Permissions (%u):", (unsigned)Permissions.size());
		for (AStringVector::const_iterator itr = Permissions.begin(), end = Permissions.end(); itr != end; ++itr)
		{
			LOG("    '%s'", itr->c_str());
		}  // for itr - Groups[]
	}
	
	
	void ReportRankGroups(const AString & a_RankName)
	{
		AStringVector Groups = m_Mgr.GetRankGroups(a_RankName);
		LOG("Groups in rank %s: %u", a_RankName.c_str(), (unsigned)Groups.size());
		for (AStringVector::const_iterator itr = Groups.begin(), end = Groups.end(); itr != end; ++itr)
		{
			LOG("  '%s'", itr->c_str());
		}
		AStringVector Permissions = m_Mgr.GetRankPermissions(a_RankName);
		LOG("Permissions in rank %s: %u", a_RankName.c_str(), (unsigned)Permissions.size());
		for (AStringVector::const_iterator itr = Permissions.begin(), end = Permissions.end(); itr != end; ++itr)
		{
			LOG("  '%s'", itr->c_str());
		}
	}
	

	void ReportGroupPermissions(const AString & a_GroupName)
	{
		AStringVector Permissions = m_Mgr.GetGroupPermissions(a_GroupName);
		LOG("Permissions in group %s: %u", a_GroupName.c_str(), (unsigned)Permissions.size());
		for (AStringVector::const_iterator itr = Permissions.begin(), end = Permissions.end(); itr != end; ++itr)
		{
			LOG("  '%s'", itr->c_str());
		}
	}
	
protected:
	cRankManager m_Mgr;
} g_RankMgrTest;
//*/





cRankManager::cRankManager(void) :
	m_DB("Ranks.sqlite", SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE)
{
	// Create the DB tables, if they don't exist:
	m_DB.exec("CREATE TABLE IF NOT EXISTS Rank (RankID INTEGER PRIMARY KEY, Name, MsgPrefix, MsgSuffix, MsgNameColorCode)");
	m_DB.exec("CREATE TABLE IF NOT EXISTS PlayerRank (PlayerUUID, PlayerName, RankID INTEGER)");
	m_DB.exec("CREATE TABLE IF NOT EXISTS PermGroup (PermGroupID INTEGER PRIMARY KEY, Name)");
	m_DB.exec("CREATE TABLE IF NOT EXISTS RankPermGroup (RankID INTEGER, PermGroupID INTEGER)");
	m_DB.exec("CREATE TABLE IF NOT EXISTS PermissionItem (PermGroupID INTEGER, Permission)");
	
	// TODO: Check if tables empty, add some defaults then
}





AString cRankManager::GetPlayerRankName(const AString & a_PlayerUUID)
{
	try
	{
		SQLite::Statement stmt(m_DB, "SELECT Rank.Name FROM Rank LEFT JOIN PlayerRank ON Rank.RankID = PlayerRank.RankID WHERE PlayerRank.PlayerUUID = ?");
		stmt.bind(1, a_PlayerUUID);
		stmt.executeStep();
		if (stmt.isDone())
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





AStringVector cRankManager::GetPlayerGroups(const AString & a_PlayerUUID)
{
	AStringVector res;
	try
	{
		// Prepare the DB statement:
		SQLite::Statement stmt(m_DB,
			"SELECT PermGroup.Name FROM PermGroup "
				"LEFT JOIN RankPermGroup "
					"ON PermGroup.PermGroupID = RankPermGroup.PermGroupID "
				"LEFT JOIN PlayerRank "
					"ON PlayerRank.RankID = RankPermGroup.RankID "
			"WHERE PlayerRank.PlayerUUID = ?"
		);
		stmt.bind(1, a_PlayerUUID);
		
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





AStringVector cRankManager::GetPlayerPermissions(const AString & a_PlayerUUID)
{
	AStringVector res;
	try
	{
		// Prepare the DB statement:
		SQLite::Statement stmt(m_DB,
			"SELECT DISTINCT(PermissionItem.Permission) FROM PermissionItem "
				"LEFT JOIN RankPermGroup "
					"ON PermissionItem.PermGroupID = RankPermGroup.PermGroupID "
				"LEFT JOIN PlayerRank "
					"ON PlayerRank.RankID = RankPermGroup.RankID "
			"WHERE PlayerRank.PlayerUUID = ?"
		);
		stmt.bind(1, a_PlayerUUID);
		
		// Execute and get results:
		while (stmt.executeStep())
		{
			res.push_back(stmt.getColumn(0).getText());
		}
	}
	catch (const SQLite::Exception & ex)
	{
		LOGWARNING("%s: Cannot get player permissions: %s", __FUNCTION__, ex.what());
	}
	return res;
}





AStringVector cRankManager::GetRankGroups(const AString & a_RankName)
{
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





AStringVector cRankManager::GetRankPermissions(const AString & a_RankName)
{
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





AStringVector cRankManager::GetAllRanks(void)
{
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





bool cRankManager::GetPlayerMsgVisuals(
	const AString & a_PlayerUUID,
	AString & a_MsgPrefix,
	AString & a_MsgSuffix,
	AString & a_MsgNameColorCode
)
{
	AStringVector res;
	try
	{
		SQLite::Statement stmt(m_DB,
			"SELECT Rank.MsgPrefix, Rank.MsgSuffix, Rank.MsgNameColorCode FROM Rank "
				"LEFT JOIN PlayerRank ON Rank.RankID = PlayerRank.RankID "
			"WHERE PlayerRank.PlayerUUID = ?"
		);
		stmt.bind(1, a_PlayerUUID);
		if (!stmt.executeStep())
		{
			LOGD("%s: Player UUID %s not found in the DB, returning empty values.", __FUNCTION__, a_PlayerUUID.c_str());
			a_MsgPrefix.clear();
			a_MsgSuffix.clear();
			a_MsgNameColorCode.clear();
			return false;
		}
		a_MsgPrefix = stmt.getColumn(0).getText();
		a_MsgSuffix = stmt.getColumn(1).getText();
		a_MsgNameColorCode = stmt.getColumn(2).getText();
		return true;
	}
	catch (const SQLite::Exception & ex)
	{
		LOGWARNING("%s: Failed to get ranks from DB: %s. Returning empty values.", __FUNCTION__, ex.what());
	}
	a_MsgPrefix.clear();
	a_MsgSuffix.clear();
	a_MsgNameColorCode.clear();
	return false;
}





void cRankManager::AddRank(
	const AString & a_RankName,
	const AString & a_MsgPrefix,
	const AString & a_MsgSuffix,
	const AString & a_MsgNameColorCode
)
{
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
	try
	{
		// Check if such a rank name is already used:
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
		
		// Insert a new rank:
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





bool cRankManager::AddGroupToRank(const AString & a_GroupName, const AString & a_RankName)
{
	try
	{
		SQLite::Transaction trans(m_DB);
		int GroupID, RankID;
		
		// Get the group's ID:
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
		trans.commit();
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
	try
	{
		// Wrapp the entire operation into a transaction:
		SQLite::Transaction trans(m_DB);
		
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
		trans.commit();
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





void cRankManager::RemoveRank(const AString & a_RankName, const AString & a_ReplacementRankName)
{
	AStringVector res;
	try
	{
		// Wrap everything into a transaction:
		SQLite::Transaction trans(m_DB);
		
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
		
		trans.commit();
	}
	catch (const SQLite::Exception & ex)
	{
		LOGWARNING("%s: Failed to remove rank from DB: %s", __FUNCTION__, ex.what());
	}
}





void cRankManager::RemoveGroup(const AString & a_GroupName)
{
	try
	{
		// Wrap everything into a transaction:
		SQLite::Transaction trans(m_DB);
		
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
		
		trans.commit();
	}
	catch (const SQLite::Exception & ex)
	{
		LOGWARNING("%s: Failed to remove group %s from DB: %s", __FUNCTION__, a_GroupName.c_str(), ex.what());
	}
}





void cRankManager::RemoveGroupFromRank(const AString & a_GroupName, const AString & a_RankName)
{
	try
	{
		// Wrap everything into a transaction:
		SQLite::Transaction trans(m_DB);
		
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
		
		trans.commit();
	}
	catch (const SQLite::Exception & ex)
	{
		LOGWARNING("%s: Failed to remove group %s from rank %s in the DB: %s", __FUNCTION__, a_GroupName.c_str(), a_RankName.c_str(), ex.what());
	}
}





void cRankManager::RemovePermissionFromGroup(const AString & a_Permission, const AString & a_GroupName)
{
	try
	{
		// Wrap everything into a transaction:
		SQLite::Transaction trans(m_DB);
		
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
		
		trans.commit();
	}
	catch (const SQLite::Exception & ex)
	{
		LOGWARNING("%s: Failed to remove permission %s from group %s in DB: %s",
			__FUNCTION__, a_Permission.c_str(), a_GroupName.c_str(), ex.what()
		);
	}
}





bool cRankManager::RenameRank(const AString & a_OldName, const AString & a_NewName)
{
	try
	{
		// Wrap everything into a transaction:
		SQLite::Transaction trans(m_DB);
		
		// Check that NewName doesn't exist:
		{
			SQLite::Statement stmt(m_DB, "SELECT RankID FROM Rank WHERE Name = ?");
			stmt.bind(1, a_NewName);
			if (stmt.executeStep())
			{
				LOGD("%s: Rank %s is already present, cannot rename %s", __FUNCTION__, a_NewName.c_str(), a_OldName.c_str());
				return false;
			}
		}
		
		// Rename:
		bool res;
		{
			SQLite::Statement stmt(m_DB, "UPDATE Rank SET Name = ? WHERE Name = ?");
			stmt.bind(1, a_NewName);
			stmt.bind(2, a_OldName);
			res = (stmt.exec() > 0);
		}
		
		trans.commit();
		return res;
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
	try
	{
		// Wrap everything into a transaction:
		SQLite::Transaction trans(m_DB);
		
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
		
		trans.commit();
		return res;
	}
	catch (const SQLite::Exception & ex)
	{
		LOGWARNING("%s: Failed to rename group %s to %s in DB: %s",
			__FUNCTION__, a_OldName.c_str(), a_NewName.c_str(), ex.what());
	}
	return false;
}





void cRankManager::SetPlayerRank(const AString & a_PlayerUUID, const AString & a_PlayerName, const AString & a_RankName)
{
	try
	{
		// Wrap the entire operation into a transaction:
		SQLite::Transaction trans(m_DB);
		
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
			stmt.bind(3, a_PlayerUUID);
			if (stmt.exec() > 0)
			{
				// Successfully updated the player's rank
				trans.commit();
				return;
			}
		}
		
		// The player is not yet in the DB, add them:
		SQLite::Statement stmt(m_DB, "INSERT INTO PlayerRank (RankID, PlayerUUID, PlayerName) VALUES (?, ?, ?)");
		stmt.bind(1, RankID);
		stmt.bind(2, a_PlayerUUID);
		stmt.bind(3, a_PlayerName);
		if (stmt.exec() > 0)
		{
			// Successfully added the player
			trans.commit();
			return;
		}
		
		LOGWARNING("%s: Failed to set player UUID %s to rank %s.",
			__FUNCTION__, a_PlayerUUID.c_str(), a_RankName.c_str()
		);
	}
	catch (const SQLite::Exception & ex)
	{
		LOGWARNING("%s: Failed to set player UUID %s to rank %s: %s",
			__FUNCTION__, a_PlayerUUID.c_str(), a_RankName.c_str(), ex.what()
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
	AStringVector res;
	try
	{
		SQLite::Statement stmt(m_DB, "UPDATE Rank SET MsgPrefix = ?, MsgSuffix = ?, MsgNameColorCode = ? WHERE Name = ?");
		stmt.bind(1, a_MsgPrefix);
		stmt.bind(2, a_MsgSuffix);
		stmt.bind(1, a_MsgNameColorCode);
		stmt.bind(2, a_RankName);
		if (!stmt.executeStep())
		{
			LOGINFO("%s: Rank %s not found, visuals not set.", __FUNCTION__, a_RankName.c_str());
		}
	}
	catch (const SQLite::Exception & ex)
	{
		LOGWARNING("%s: Failed to get ranks from DB: %s", __FUNCTION__, ex.what());
	}
}





bool cRankManager::RankExists(const AString & a_RankName)
{
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





bool cRankManager::IsPlayerRankSet(const AString & a_PlayerUUID)
{
	try
	{
		SQLite::Statement stmt(m_DB, "SELECT * FROM PlayerRank WHERE PlayerUUID = ?");
		stmt.bind(1, a_PlayerUUID);
		if (stmt.executeStep())
		{
			// The player UUID was found, they have a rank
			return true;
		}
	}
	catch (const SQLite::Exception & ex)
	{
		LOGWARNING("%s: Failed to query DB for player UUID %s: %s", __FUNCTION__, a_PlayerUUID.c_str(), ex.what());
	}
	return false;
}





bool cRankManager::IsGroupInRank(const AString & a_GroupName, const AString & a_RankName)
{
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




