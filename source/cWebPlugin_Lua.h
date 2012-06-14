#pragma once

#include "cWebPlugin.h"

class cPlugin_NewLua;
typedef struct lua_State lua_State;

// a WebPlugin class more specialized for Lua
class cWebPlugin_Lua : public cWebPlugin	//tolua_export
{											//tolua_export
public:										//tolua_export
	cWebPlugin_Lua( cPlugin_NewLua* a_Plugin );
	virtual ~cWebPlugin_Lua();

	bool AddTab( const char* a_Title, lua_State * a_LuaState, int a_FunctionReference );	// >> EXPORTED IN MANUALBINDINGS <<

	virtual std::string HandleRequest( HTTPRequest* a_Request );
	virtual void Initialize();

	std::pair< std::string, std::string > GetTabNameForRequest( HTTPRequest* a_Request );

	std::list< std::pair<std::string, std::string> > GetTabNames();
private:
	cPlugin_NewLua* m_Plugin;
	
	struct sWebPluginTab;
	typedef std::list< sWebPluginTab* > TabList;
	TabList m_Tabs;
};	//tolua_export