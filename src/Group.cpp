
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Group.h"





void cGroup::AddCommand( const AString & a_Command)
{
	m_Commands[ a_Command ] = true;
}





void cGroup::AddPermission( const AString & a_Permission)
{
	m_Permissions[ a_Permission ] = true;
}





void cGroup::InheritFrom( cGroup* a_Group)
{
	m_Inherits.remove( a_Group);
	m_Inherits.push_back( a_Group);
}





void cGroup::ClearPermission()
{
	m_Permissions.clear();
}
