
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "cGroupManager.h"
#include "cGroup.h"
#include "../iniFile/iniFile.h"
#include "cChatColor.h"
#include "cRoot.h"

extern std::vector< std::string > StringSplit( std::string str, std::string delim);

typedef std::map< std::string, cGroup* > GroupMap;
struct cGroupManager::sGroupManagerState
{
	GroupMap Groups;
};

cGroupManager* cGroupManager::GetGroupManager()
{
	LOGWARN("WARNING: Using deprecated function cGroupManager::GetGroupManager() use cRoot::Get()->GetGroupManager() instead!");
	return cRoot::Get()->GetGroupManager();
}

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
	LOG("-- Loading Groups --");
	cIniFile IniFile("groups.ini");
	if( IniFile.ReadFile() )
	{
		unsigned int NumKeys = IniFile.GetNumKeys();
		for( unsigned int i = 0; i < NumKeys; i++ )
		{
			std::string KeyName = IniFile.GetKeyName( i );
			cGroup* Group = GetGroup( KeyName.c_str() );

			LOG("Loading group: %s", KeyName.c_str() );

			Group->SetName( KeyName );
			char Color = IniFile.GetValue( KeyName, "Color", "-" )[0];
			if( Color != '-' )
				Group->SetColor( cChatColor::MakeColor(Color) );
			else
				Group->SetColor( cChatColor::White );

			std::string Commands = IniFile.GetValue( KeyName, "Commands", "" );
			if( Commands.size() > 0 )
			{
				std::vector< std::string > Split = StringSplit( Commands, "," );
				for( unsigned int i = 0; i < Split.size(); i++)
				{
					Group->AddCommand( Split[i] );
					//LOG("%s", Split[i].c_str() );
				}
			}

			std::string Permissions = IniFile.GetValue( KeyName, "Permissions", "" );
			if( Permissions.size() > 0 )
			{
				std::vector< std::string > Split = StringSplit( Permissions, "," );
				for( unsigned int i = 0; i < Split.size(); i++)
				{
					Group->AddPermission( Split[i] );
					LOGINFO("Permission: %s", Split[i].c_str() );
				}
			}

			std::string Groups = IniFile.GetValue( KeyName, "Inherits", "" );
			if( Groups.size() > 0 )
			{
				std::vector< std::string > Split = StringSplit( Groups, "," );
				for( unsigned int i = 0; i < Split.size(); i++)
				{
					Group->InheritFrom( GetGroup( Split[i].c_str() ) );
				}
			}
		}
	}
	LOG("-- Done Loading Groups --");
}

cGroup* cGroupManager::GetGroup( const char* a_Name )
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