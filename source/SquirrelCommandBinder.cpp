
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "SquirrelCommandBinder.h"
#include "Player.h"
#include "Plugin.h"
#include "Plugin_Squirrel.h"
#include "squirrelbindings/SquirrelArray.h"


cSquirrelCommandBinder::cSquirrelCommandBinder()
{
}

cSquirrelCommandBinder::~cSquirrelCommandBinder()
{
}

void cSquirrelCommandBinder::ClearBindings()
{
	m_BoundCommands.clear();
}

void cSquirrelCommandBinder::RemoveBindingsForPlugin( cPlugin* a_Plugin )
{
	for( CommandMap::iterator itr = m_BoundCommands.begin(); itr != m_BoundCommands.end(); )
	{
		if( itr->second.Plugin == a_Plugin )
		{
			LOGINFO("Unbinding %s ", itr->first.c_str( ) );
			CommandMap::iterator eraseme = itr;
			++itr;
			m_BoundCommands.erase( eraseme );
			continue;
		}
		++itr;
	}
}

bool cSquirrelCommandBinder::BindCommand( const std::string & a_Command, const std::string & a_Permission, cPlugin* a_Plugin, Sqrat::Function a_Callback )
{
	if( !a_Plugin->CanBindCommands() )
	{
		LOGERROR("ERROR: Trying to bind command \"%s\" to a plugin that is not initialized.", a_Command.c_str() );
		return false;
	}
	if( m_BoundCommands.find( a_Command ) != m_BoundCommands.end() )
	{
		LOGERROR("ERROR: Trying to bind command \"%s\" that has already been bound.", a_Command.c_str() );
		return false;
	}
	LOGINFO("Binding %s (%s)", a_Command.c_str(), a_Permission.c_str() );

	BoundFunction Callback;
	Callback.Callback = a_Callback;
	Callback.Plugin = a_Plugin;
	Callback.Permission = a_Permission;

	m_BoundCommands[ a_Command ] = Callback;
	return true;
}

bool cSquirrelCommandBinder::HandleCommand( const std::string & a_Command, cPlayer* a_Player )
{
	AStringVector Split = StringSplit(a_Command, " ");
	if (Split.size() == 0)
	{
		return false;
	}
	
	CommandMap::iterator FoundCommand = m_BoundCommands.find( Split[0] );
	if( FoundCommand != m_BoundCommands.end() )
	{
		const BoundFunction & func = FoundCommand->second;
		if( func.Permission.size() > 0 )
		{
			if( !a_Player->HasPermission( func.Permission.c_str() ) )
			{
				return false;
			}
		}

		
		// Push the split
		SquirrelStringArray SplitData;

		std::vector<std::string>::const_iterator iter = Split.begin();
		while(iter != Split.end()) {
			SplitData.Add(*iter);
			++iter;
		}

		// Push player
		Sqrat::Function callback = func.Callback;
		return callback.Evaluate<bool>(&SplitData, a_Player);
	}
	return false;
}
