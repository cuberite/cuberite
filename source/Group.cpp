
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Group.h"

void cGroup::AddCommand( std::string a_Command )
{
	m_Commands[ a_Command ] = true;
}

void cGroup::AddPermission( std::string a_Permission )
{
	m_Permissions[ a_Permission ] = true;
}

bool cGroup::HasCommand( std::string a_Command )
{
	if( m_Commands.find("*") != m_Commands.end() ) return true;

	CommandMap::iterator itr = m_Commands.find( a_Command );
	if( itr != m_Commands.end() )
	{
		if( itr->second ) return true;
	}

	for( GroupList::iterator itr = m_Inherits.begin(); itr != m_Inherits.end(); ++itr )
	{
		if( (*itr)->HasCommand( a_Command ) ) return true;
	}
	return false;
}

void cGroup::InheritFrom( cGroup* a_Group )
{
	m_Inherits.remove( a_Group );
	m_Inherits.push_back( a_Group );
}