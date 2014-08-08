
// RankManager.cpp

// Implements the cRankManager class that represents the rank manager responsible for assigning permissions and message visuals to players

#include "Globals.h"
#include "RankManager.h"
#include "Protocol/MojangAPI.h"
#include "inifile/iniFile.h"
#include <iostream>





//*
// This code is for internal testing while developing the cRankManager class
static class cRankMgrTest
{
public:
	cRankMgrTest(void) :
		m_Mgr()
	{
		// Initialize the cMojangAPI so that it can convert playernames to UUIDs:
		cIniFile Ini;
		Ini.ReadFile("settings.ini");
		m_API.Start(Ini);
		
		// Test the cRankManager class:
		ReportPlayer("xoft");
	}
	
	void ReportPlayer(const AString & a_PlayerName)
	{
		// Get the player's UUID and rank:
		AString UUID = m_API.GetUUIDFromPlayerName(a_PlayerName);
		std::cout << "Player " << a_PlayerName << " has UUID '" << UUID <<"'." << std::endl;
		std::cout << "  Rank: '" << m_Mgr.GetPlayerRankName(UUID) << "'." << std::endl;

		// List all the permission groups for the player:
		AStringVector Groups = m_Mgr.GetPlayerGroups(UUID);
		std::cout << "  Groups(" << Groups.size() << "):" << std::endl;
		for (AStringVector::const_iterator itr = Groups.begin(), end = Groups.end(); itr != end; ++itr)
		{
			std::cout << "    '" << *itr << "'." << std::endl;
		}  // for itr - Groups[]
		
		// List all the permissions for the player:
		AStringVector Permissions = m_Mgr.GetPlayerPermissions(UUID);
		std::cout << "  Permissions(" << Permissions.size() << "):" << std::endl;
		for (AStringVector::const_iterator itr = Permissions.begin(), end = Permissions.end(); itr != end; ++itr)
		{
			std::cout << "    '" << *itr << "'." << std::endl;
		}  // for itr - Groups[]
		
		std::cout << "Done." << std::endl;
	}
	
protected:
	cRankManager m_Mgr;
	cMojangAPI   m_API;
} g_RankMgrTest;
//*/





cRankManager::cRankManager(void) :
	m_DB("Ranks.sqlite", SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE)
{
	// Create the DB tables, if they don't exist:
	m_DB.exec("CREATE TABLE IF NOT EXISTS Rank (RankID INTEGER PRIMARY KEY, Name, MsgPrefix, MsgPostfix, MsgNameColorCode)");
	m_DB.exec("CREATE TABLE IF NOT EXISTS PlayerRank (PlayerUUID, PlayerName, RankID INTEGER)");
	m_DB.exec("CREATE TABLE IF NOT EXISTS PermGroup (PermGroupID INTEGER PRIMARY KEY, Name)");
	m_DB.exec("CREATE TABLE IF NOT EXISTS RankPermGroup (RankID INTEGER, PermGroupID INTEGER)");
	m_DB.exec("CREATE TABLE IF NOT EXISTS PermissionItem (PermGroupID INTEGER, Permission)");
	
	// TODO: Check if tables empty, add some defaults then
}





AString cRankManager::GetPlayerRankName(const AString & a_PlayerUUID)
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





AStringVector cRankManager::GetPlayerGroups(const AString & a_PlayerUUID)
{
	// Prepare the DB statement:
	SQLite::Statement stmt(m_DB,
		"SELECT Group.Name FROM Group "
			"LEFT JOIN RankGroups "
				"ON Group.GroupID = RankGroups.GroupID "
			"LEFT JOIN PlayerRank "
				"ON PlayerRank.RankID = RankGroups.RankID "
		"WHERE PlayerRank.PlayerUUID = ?"
	);
	stmt.bind(1, a_PlayerUUID);
	
	// Execute and get results:
	AStringVector res;
	while (stmt.executeStep())
	{
		res.push_back(stmt.getColumn(0).getText());
	}
	return res;
}





AStringVector cRankManager::GetPlayerPermissions(const AString & a_PlayerUUID)
{
	// Prepare the DB statement:
	SQLite::Statement stmt(m_DB,
		"SELECT PermissionItem.Permission FROM PermissionItem "
			"LEFT JOIN RankGroups "
				"ON PermissionItem.GroupID = RankGroups.GroupID "
			"LEFT JOIN PlayerRank "
				"ON PlayerRank.RankID = RankGroups.RankID "
		"WHERE PlayerRank.PlayerUUID = ?"
	);
	stmt.bind(1, a_PlayerUUID);
	
	// Execute and get results:
	AStringVector res;
	while (stmt.executeStep())
	{
		res.push_back(stmt.getColumn(0).getText());
	}
	return res;
}




