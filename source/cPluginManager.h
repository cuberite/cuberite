
#pragma once

struct lua_State;
class cLuaCommandBinder;
class cPlugin;
class cPlugin_Lua;

// fwd: cPlayer.h
class cPlayer;

// fwd: CraftingRecipes.h
class cCraftingGrid;
class cCraftingRecipe;





class cPluginManager													//tolua_export
{																		//tolua_export
public:																	//tolua_export

	// Called each tick
	virtual void Tick(float a_Dt);

	// tolua_begin
	enum PluginHook
	{
		HOOK_TICK,
		HOOK_CHAT,
		HOOK_COLLECT_ITEM,
		HOOK_BLOCK_DIG,
		HOOK_BLOCK_PLACE,
		HOOK_DISCONNECT,
		HOOK_HANDSHAKE,
		HOOK_LOGIN,
		HOOK_PLAYER_SPAWN,
		HOOK_PLAYER_JOIN,
		HOOK_PLAYER_MOVE,
		HOOK_TAKE_DAMAGE,
		HOOK_KILLED,
		HOOK_CHUNK_GENERATED,
		HOOK_CHUNK_GENERATING,
		HOOK_BLOCK_TO_DROPS,
		HOOK_PRE_CRAFTING,        /// cPlayer, cCraftingGrid, cCraftingRecipe
		HOOK_CRAFTING_NO_RECIPE,  /// cPlayer, cCraftingGrid, cCraftingRecipe
		HOOK_POST_CRAFTING,       /// cPlayer, cCraftingGrid, cCraftingRecipe
		
		// E_PLUGIN_ names are obsolete, but are kept for compatibility reasons
		E_PLUGIN_TICK             = HOOK_TICK,
		E_PLUGIN_CHAT             = HOOK_CHAT,
		E_PLUGIN_COLLECT_ITEM     = HOOK_COLLECT_ITEM,
		E_PLUGIN_BLOCK_DIG        = HOOK_BLOCK_DIG,
		E_PLUGIN_BLOCK_PLACE      = HOOK_BLOCK_PLACE,
		E_PLUGIN_DISCONNECT       = HOOK_DISCONNECT,
		E_PLUGIN_HANDSHAKE        = HOOK_HANDSHAKE,
		E_PLUGIN_LOGIN            = HOOK_LOGIN,
		E_PLUGIN_PLAYER_SPAWN     = HOOK_PLAYER_SPAWN,
		E_PLUGIN_PLAYER_JOIN      = HOOK_PLAYER_JOIN,
		E_PLUGIN_PLAYER_MOVE      = HOOK_PLAYER_MOVE,
		E_PLUGIN_TAKE_DAMAGE      = HOOK_TAKE_DAMAGE,
		E_PLUGIN_KILLED           = HOOK_KILLED,
		E_PLUGIN_CHUNK_GENERATED  = HOOK_CHUNK_GENERATED,
		E_PLUGIN_CHUNK_GENERATING = HOOK_CHUNK_GENERATING,
		E_PLUGIN_BLOCK_TO_DROPS   = HOOK_BLOCK_TO_DROPS,
	};
	// tolua_end

	static cPluginManager * GetPluginManager();							//tolua_export

	typedef std::list< cPlugin * > PluginList;
	cPlugin* GetPlugin( const char* a_Plugin ) const;					//tolua_export
	const PluginList & GetAllPlugins() const;							// >> EXPORTED IN MANUALBINDINGS <<

	void ReloadPlugins();												//tolua_export
	bool AddPlugin( cPlugin* a_Plugin );
	bool AddPlugin( lua_State* a_LuaState, cPlugin* a_Plugin );			//tolua_export
	bool AddLuaPlugin( cPlugin_Lua* a_Plugin );
	void AddHook( cPlugin* a_Plugin, PluginHook a_Hook );				//tolua_export

	unsigned int GetNumPlugins() const;									//tolua_export

	bool CallHook( PluginHook a_Hook, unsigned int a_NumArgs, ... );
	
	bool CallHookPreCrafting     (const cPlayer * a_Player, const cCraftingGrid * a_Grid, cCraftingRecipe * a_Recipe);
	bool CallHookCraftingNoRecipe(const cPlayer * a_Player, const cCraftingGrid * a_Grid, cCraftingRecipe * a_Recipe);
	bool CallHookPostCrafting    (const cPlayer * a_Player, const cCraftingGrid * a_Grid, cCraftingRecipe * a_Recipe);

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
