#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "GroupManager.h"
#include "Group.h"
#include "../lib/inifile/iniFile.h"
#include "ChatColor.h"
#include "Root.h"





typedef std::map< AString, cGroup* > GroupMap;





struct cGroupManager::sGroupManagerState
{
	GroupMap Groups;
};





cGroupManager::~cGroupManager()
{
	for( GroupMap::iterator itr = m_pState->Groups.begin(); itr != m_pState->Groups.end(); ++itr )
	{
		delete itr->second;
	}
	m_pState->Groups.clear();

	delete m_pState;
}





cGroupManager::cGroupManager()
	: m_pState( new sGroupManagerState )
{
	LOGD("-- Loading Groups --");
	cIniFile IniFile;
	if (!IniFile.ReadFile("groups.ini"))
	{
		LOGWARNING("groups.ini inaccessible, no groups are defined");
		return;
	}

	unsigned int NumKeys = IniFile.GetNumKeys();
	for( unsigned int i = 0; i < NumKeys; i++ )
	{
		std::string KeyName = IniFile.GetKeyName( i );
		cGroup* Group = GetGroup( KeyName.c_str() );

		LOGD("Loading group: %s", KeyName.c_str() );

		Group->SetName( KeyName );
		char Color = IniFile.GetValue( KeyName, "Color", "-" )[0];
		if( Color != '-' )
			Group->SetColor( cChatColor::MakeColor(Color) );
		else
			Group->SetColor( cChatColor::White );

		std::string Commands = IniFile.GetValue( KeyName, "Commands", "" );
		if( Commands.size() > 0 )
		{
			AStringVector Split = StringSplit( Commands, "," );
			for( unsigned int i = 0; i < Split.size(); i++)
			{
				Group->AddCommand( Split[i] );
			}
		}

		std::string Permissions = IniFile.GetValue( KeyName, "Permissions", "" );
		if( Permissions.size() > 0 )
		{
			AStringVector Split = StringSplit( Permissions, "," );
			for( unsigned int i = 0; i < Split.size(); i++)
			{
				Group->AddPermission( Split[i] );
			}
		}

		std::string Groups = IniFile.GetValue( KeyName, "Inherits", "" );
		if( Groups.size() > 0 )
		{
			AStringVector Split = StringSplit( Groups, "," );
			for( unsigned int i = 0; i < Split.size(); i++)
			{
				Group->InheritFrom( GetGroup( Split[i].c_str() ) );
			}
		}
	}
	LOGD("-- Groups Successfully Loaded --");
}





cGroup* cGroupManager::GetGroup( const AString & a_Name )
{
	GroupMap::iterator itr = m_pState->Groups.find( a_Name );
	if( itr != m_pState->Groups.end() )
	{
		return itr->second;
	}

	cGroup* Group = new cGroup();
	m_pState->Groups[a_Name] = Group;

	return Group;
}




