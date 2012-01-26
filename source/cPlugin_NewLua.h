#pragma once

#include "cPlugin.h"
#include <string>

typedef struct lua_State lua_State;

class cPlugin_NewLua : public cPlugin			//tolua_export
{												//tolua_export
public:											//tolua_export
	cPlugin_NewLua( const char* a_PluginName );
	~cPlugin_NewLua();

	virtual bool Initialize();					//tolua_export
	virtual void Tick(float a_Dt);				//tolua_export

private:
	std::string m_Directory;
	lua_State* m_LuaState;
};//tolua_export