
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "cWebPlugin_Lua.h"
#include "cPlugin_NewLua.h"

#include "tolua++.h"
#include "cWebAdmin.h"





extern bool report_errors(lua_State* lua, int status);





static std::string SafeString( const std::string& a_String )
{
	std::string RetVal;
	for( unsigned int i = 0; i < a_String.size(); ++i )
	{
		char c = a_String[i];
		if( c == ' ' ) 
		{
			c = '_';
		}
		RetVal.push_back( c );
	}
	return RetVal;
}





struct cWebPlugin_Lua::sWebPluginTab
{
	std::string Title;
	std::string SafeTitle;

	int Reference;
};

cWebPlugin_Lua::cWebPlugin_Lua( cPlugin_NewLua* a_Plugin )
	: cWebPlugin( a_Plugin->GetLuaState() )
	, m_Plugin( a_Plugin )
{

}

cWebPlugin_Lua::~cWebPlugin_Lua()
{
	for( TabList::iterator itr = m_Tabs.begin(); itr != m_Tabs.end(); ++itr )
	{
		delete *itr;
	}
	m_Tabs.clear();
}

bool cWebPlugin_Lua::AddTab( const char* a_Title, lua_State * a_LuaState, int a_FunctionReference )
{
	if( a_LuaState != m_Plugin->GetLuaState() )
	{
		LOGERROR("Only allowed to add a tab to a WebPlugin of your own Plugin!");
		return false;
	}
	sWebPluginTab* Tab = new sWebPluginTab();
	Tab->Title = a_Title;
	Tab->SafeTitle = SafeString( a_Title );

	Tab->Reference = a_FunctionReference;

	m_Tabs.push_back( Tab );
	return true;
}

std::string cWebPlugin_Lua::HandleRequest( HTTPRequest* a_Request )
{
	cCSLock( m_Plugin->GetCriticalSection() );
	lua_State* LuaState = m_Plugin->GetLuaState();
	std::string RetVal = "";

	std::pair< std::string, std::string > TabName = GetTabNameForRequest(a_Request);
	std::string SafeTabName = TabName.second;
	if( SafeTabName.empty() )
		return "";

	sWebPluginTab* Tab = 0;
	for( TabList::iterator itr = m_Tabs.begin(); itr != m_Tabs.end(); ++itr )
	{
		if( (*itr)->SafeTitle.compare( SafeTabName ) == 0 ) // This is the one! Rawr
		{
			Tab = *itr;
			break;
		}
	}

	if( Tab )
	{
		//LOGINFO("1. Stack size: %i", lua_gettop(LuaState) );
		lua_rawgeti( LuaState, LUA_REGISTRYINDEX, Tab->Reference); // same as lua_getref()

		//LOGINFO("2. Stack size: %i", lua_gettop(LuaState) );
		// Push HTTPRequest
		tolua_pushusertype( LuaState, a_Request, "HTTPRequest" );
		//LOGINFO("Calling bound function! :D");
		int s = lua_pcall( LuaState, 1, 1, 0);

		if ( s != 0 )
		{
			std::string err = lua_tostring(LuaState, -1);
			LOGERROR("-- %s", err.c_str() );
			lua_pop(LuaState, 1);
			LOGINFO("error. Stack size: %i", lua_gettop(LuaState) );
			return err;	// Show the error message in the web page, looks cool
		}

		if( !lua_isstring( LuaState, -1 ) )
		{
			LOGWARN("WARNING: WebPlugin tab '%s' did not return a string!", Tab->Title.c_str() );
			lua_pop(LuaState, 1); // Pop return value
			return std::string("WARNING: WebPlugin tab '") + Tab->Title + std::string("' did not return a string!");
		}

		RetVal += tolua_tostring(LuaState, -1, 0);
		lua_pop(LuaState, 1); // Pop return value
		//LOGINFO("ok. Stack size: %i", lua_gettop(LuaState) );
	}

	return RetVal;
}

void cWebPlugin_Lua::Initialize()
{
}

std::pair< std::string, std::string > cWebPlugin_Lua::GetTabNameForRequest( HTTPRequest* a_Request )
{
	std::pair< std::string, std::string > Names;
	AStringVector Split = StringSplit(a_Request->Path, "/");

	if( Split.size() > 1 )
	{
		sWebPluginTab* Tab = 0;
		if( Split.size() > 2 )	// If we got the tab name, show that page
		{
			for( TabList::iterator itr = m_Tabs.begin(); itr != m_Tabs.end(); ++itr )
			{
				if( (*itr)->SafeTitle.compare( Split[2] ) == 0 ) // This is the one! Rawr
				{
					Tab = *itr;
					break;
				}
			}
		}
		else	// Otherwise show the first tab
		{
			if( m_Tabs.size() > 0 )
				Tab = *m_Tabs.begin();
		}

		if( Tab )
		{
			Names.first = Tab->Title;
			Names.second = Tab->SafeTitle;
		}
	}

	return Names;
}

std::list< std::pair<std::string, std::string> > cWebPlugin_Lua::GetTabNames()
{
	std::list< std::pair< std::string, std::string > > NameList;
	for( TabList::iterator itr = m_Tabs.begin(); itr != m_Tabs.end(); ++itr )
	{
		std::pair< std::string, std::string > StringPair;
		StringPair.first = (*itr)->Title;
		StringPair.second = (*itr)->SafeTitle;
		NameList.push_back( StringPair );
	}
	return NameList;
}