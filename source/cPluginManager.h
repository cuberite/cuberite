
#pragma once

struct lua_State;
class cLuaCommandBinder;
class cPlugin;
class cPlugin_Lua;
class cPluginManager													//tolua_export
{																		//tolua_export
public:																	//tolua_export

	// Called each tick
	virtual void Tick(float a_Dt);

	enum PluginHook														//tolua_export
	{																	//tolua_export
		E_PLUGIN_TICK,													//tolua_export
		E_PLUGIN_CHAT,													//tolua_export
		E_PLUGIN_COLLECT_ITEM,											//tolua_export
		E_PLUGIN_BLOCK_DIG,												//tolua_export
		E_PLUGIN_BLOCK_PLACE,											//tolua_export
		E_PLUGIN_DISCONNECT,											//tolua_export
		E_PLUGIN_HANDSHAKE,												//tolua_export
		E_PLUGIN_LOGIN,													//tolua_export
		E_PLUGIN_PLAYER_SPAWN,											//tolua_export
		E_PLUGIN_PLAYER_JOIN,											//tolua_export
		E_PLUGIN_PLAYER_MOVE,											//tolua_export
		E_PLUGIN_TAKE_DAMAGE,											//tolua_export
		E_PLUGIN_KILLED,												//tolua_export
	};																	//tolua_export

	static cPluginManager * GetPluginManager();							//tolua_export

	typedef std::list< cPlugin* > PluginList;
	cPlugin* GetPlugin( const char* a_Plugin ) const;					//tolua_export
	const PluginList & GetAllPlugins() const;							// >> EXPORTED IN MANUALBINDINGS <<

	void ReloadPlugins();												//tolua_export
	bool AddPlugin( cPlugin* a_Plugin );
	bool AddPlugin( lua_State* a_LuaState, cPlugin* a_Plugin );			//tolua_export
	bool AddLuaPlugin( cPlugin_Lua* a_Plugin );
	void AddHook( cPlugin* a_Plugin, PluginHook a_Hook );				//tolua_export

	unsigned int GetNumPlugins() const;									//tolua_export

	bool CallHook( PluginHook a_Hook, unsigned int a_NumArgs, ... );

	void RemoveHooks( cPlugin* a_Plugin );
	void RemovePlugin( cPlugin* a_Plugin, bool a_bDelete = false );		//tolua_export
	void RemoveLuaPlugin( std::string a_FileName );						//tolua_export
	cPlugin_Lua* GetLuaPlugin( lua_State* a_State );					//tolua_export

	cLuaCommandBinder* GetLuaCommandBinder() const { return m_LuaCommandBinder; }

	bool HasPlugin( cPlugin* a_Plugin ) const;
private:
	friend class cRoot;
	cPluginManager();
	~cPluginManager();

	typedef std::list< cPlugin_Lua* > LuaPluginList;
	typedef std::map< cPluginManager::PluginHook, cPluginManager::PluginList > HookMap;

	LuaPluginList m_LuaPlugins;
	PluginList m_Plugins;
	HookMap m_Hooks;

	void ReloadPluginsNow();
	void UnloadPluginsNow();

	cLuaCommandBinder* m_LuaCommandBinder;

	bool m_bReloadPlugins;
}; //tolua_export
