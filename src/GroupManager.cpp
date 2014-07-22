#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "GroupManager.h"
#include "Group.h"
#include "inifile/iniFile.h"
#include "ChatColor.h"
#include "Root.h"





typedef std::map< AString, cGroup* > GroupMap;





struct cGroupManager::sGroupManagerState
{
	GroupMap Groups;
};





cGroupManager::~cGroupManager()
{
	for (GroupMap::iterator itr = m_pState->Groups.begin(); itr != m_pState->Groups.end(); ++itr)
	{
		delete itr->second;
		itr->second = NULL;
	}
	m_pState->Groups.clear();

	delete m_pState;
	m_pState = NULL;
}





cGroupManager::cGroupManager()
	: m_pState( new sGroupManagerState)
{
	LOGD("-- Loading Groups --");
	
	if (!LoadGroups())
	{
		LOGWARNING("ERROR: Groups could not load!");
	}
	if (!CheckUsers())
	{
		LOGWARNING("ERROR: User file could not be found!");
	}
	
	LOGD("-- Groups Successfully Loaded --");
}





void cGroupManager::GenerateDefaultUsersIni(cIniFile & a_IniFile)
{
	LOGWARN("Regenerating users.ini, all users will be reset");
	a_IniFile.AddHeaderComment(" This file stores the players' groups.");
	a_IniFile.AddHeaderComment(" The format is:");
	a_IniFile.AddHeaderComment(" [PlayerName]");
	a_IniFile.AddHeaderComment(" Groups = GroupName1, GroupName2, ...");

	a_IniFile.WriteFile("users.ini");
}





bool cGroupManager::CheckUsers()
{
	cIniFile IniFile;
	if (!IniFile.ReadFile("users.ini"))
	{
		GenerateDefaultUsersIni(IniFile);
		return true;
	}
	
	int NumKeys = IniFile.GetNumKeys();
	for (int i = 0; i < NumKeys; i++)
	{
		AString Player = IniFile.GetKeyName(i);
		AString Groups = IniFile.GetValue(Player, "Groups", "");
		if (Groups.empty())
		{
			continue;
		}
		AStringVector Split = StringSplitAndTrim(Groups, ",");
		for (AStringVector::const_iterator itr = Split.begin(), end = Split.end(); itr != end; ++itr)
		{
			if (!ExistsGroup(*itr))
			{
				LOGWARNING("The group %s for player %s was not found!", Split[i].c_str(), Player.c_str());
			}
		}  // for itr - Split[]
	}  // for i - ini file keys
	// Always return true for now, just but we can handle writefile fails later.
	return true;
}





bool cGroupManager::LoadGroups()
{
	cIniFile IniFile;
	if (!IniFile.ReadFile("groups.ini"))
	{
		LOGWARNING("Regenerating groups.ini, all groups will be reset");
		IniFile.AddHeaderComment(" This is the MCServer permissions manager groups file");
		IniFile.AddHeaderComment(" It stores all defined groups such as Administrators, Players, or Moderators");

		IniFile.SetValue("Owner", "Permissions", "*", true);
		IniFile.SetValue("Owner", "Color", "2", true);

		IniFile.SetValue("Moderator", "Permissions", "core.time, core.item, core.tpa, core.tpaccept, core.ban, core.unban, core.save-all, core.toggledownfall");
		IniFile.SetValue("Moderator", "Color", "2", true);
		IniFile.SetValue("Moderator", "Inherits", "Player", true);

		IniFile.SetValue("Player", "Permissions", "core.portal", true);
		IniFile.SetValue("Player", "Color", "f", true);
		IniFile.SetValue("Player", "Inherits", "Default", true);

		IniFile.SetValue("Default", "Permissions", "core.help, core.plugins, core.spawn, core.worlds, core.back, core.motd, core.build, core.locate, core.viewdistance", true);
		IniFile.SetValue("Default", "Color", "f", true);

		IniFile.WriteFile("groups.ini");
	}

	int NumKeys = IniFile.GetNumKeys();
	for (int i = 0; i < NumKeys; i++)
	{
		AString KeyName = IniFile.GetKeyName(i);
		cGroup * Group = GetGroup(KeyName.c_str());
		
		Group->ClearPermission();  // Needed in case the groups are reloaded.

		LOGD("Loading group %s", KeyName.c_str());

		Group->SetName(KeyName);
		AString Color = IniFile.GetValue(KeyName, "Color", "-");
		if ((Color != "-") && (Color.length() >= 1))
		{
			Group->SetColor(cChatColor::Delimiter + Color[0]);
		}
		else
		{
			Group->SetColor(cChatColor::White);
		}

		AString Commands = IniFile.GetValue(KeyName, "Commands", "");
		if (!Commands.empty())
		{
			AStringVector Split = StringSplitAndTrim(Commands, ",");
			for (size_t i = 0; i < Split.size(); i++)
			{
				Group->AddCommand(Split[i]);
			}
		}

		AString Permissions = IniFile.GetValue(KeyName, "Permissions", "");
		if (!Permissions.empty())
		{
			AStringVector Split = StringSplitAndTrim(Permissions, ",");
			for (size_t i = 0; i < Split.size(); i++)
			{
				Group->AddPermission(Split[i]);
			}
		}

		AString Groups = IniFile.GetValue(KeyName, "Inherits", "");
		if (!Groups.empty())
		{
			AStringVector Split = StringSplitAndTrim(Groups, ",");
			for (size_t i = 0; i < Split.size(); i++)
			{
				Group->InheritFrom(GetGroup(Split[i].c_str()));
			}
		}
	}
	// Always return true, we can handle writefile fails later.
	return true;
}





bool cGroupManager::ExistsGroup( const AString & a_Name)
{
	GroupMap::iterator itr = m_pState->Groups.find( a_Name);
	return ( itr != m_pState->Groups.end());
}





cGroup* cGroupManager::GetGroup( const AString & a_Name)
{
	GroupMap::iterator itr = m_pState->Groups.find( a_Name);
	if (itr != m_pState->Groups.end())
	{
		return itr->second;
	}

	cGroup* Group = new cGroup();
	m_pState->Groups[a_Name] = Group;

	return Group;
}




