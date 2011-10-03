#pragma once

#include <string>
#include <list>

class cPickup;
class cPlayer;
class cPacket_BlockPlace;
class cPacket_BlockDig;
class cPacket_Login;
class cPlugin;
class cPlugin_Lua							//tolua_export
{											//tolua_export
public:
	cPlugin_Lua(const char* a_Plugin);		
	~cPlugin_Lua();

	virtual bool Initialize();

	std::string GetFileName() { return m_FileName; }	//tolua_export
	typedef struct lua_State lua_State;
	lua_State* GetLuaState();

	void AddPlugin( cPlugin* a_Plugin );
	void RemovePlugin( cPlugin* a_Plugin );
private:
	void UnloadPlugins();

	std::string m_FileName;
	lua_State* m_LuaState;

	typedef std::list< cPlugin* > PluginList;
	PluginList m_Plugins;
};											//tolua_export