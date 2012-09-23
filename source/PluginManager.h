
#pragma once

#include "Item.h"

struct lua_State;
class cLuaCommandBinder;
class cSquirrelCommandBinder;
class cPlugin;
class cPlugin_Lua;

// fwd: cWorld.h
class cWorld;

// fwd: cLuaChunk.h
class cLuaChunk;

// fwd: cPlayer.h
class cPlayer;

// fwd: CraftingRecipes.h
class cCraftingGrid;
class cCraftingRecipe;

// fwd: cPickup.h
class cPickup;





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
		HOOK_COLLECT_PICKUP,
		HOOK_COLLECT_ITEM = HOOK_COLLECT_PICKUP,  // OBSOLETE, use HOOK_COLLECT_PICKUP instead
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
		HOOK_BLOCK_TO_PICKUP,     /// BlockType, BlockMeta, cPlayer, cItem, cItems
		HOOK_WEATHER_CHANGED,     /// cWorld
		HOOK_UPDATING_SIGN,       /// cWorld, int, int, int, string, string, string, string
		HOOK_UPDATED_SIGN,        /// cWorld, int, int, int, string, string, string, string
		
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
	cPlugin * GetPlugin( const AString & a_Plugin ) const;					//tolua_export
	const PluginList & GetAllPlugins() const;							// >> EXPORTED IN MANUALBINDINGS <<

	void ReloadPlugins();												//tolua_export
	bool AddPlugin( cPlugin* a_Plugin );
	bool AddPlugin( lua_State* a_LuaState, cPlugin* a_Plugin );			//tolua_export
	bool AddLuaPlugin( cPlugin_Lua* a_Plugin );
	void AddHook( cPlugin* a_Plugin, PluginHook a_Hook );				//tolua_export

	unsigned int GetNumPlugins() const;									//tolua_export

	// If the hook returns true, no further hook is called and the functions return false
	bool CallHook( PluginHook a_Hook, unsigned int a_NumArgs, ... );
	
	bool CallHookBlockDig        (cPlayer * a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace, char a_Status, BLOCKTYPE OldBlock, NIBBLETYPE OldMeta);
	bool CallHookBlockPlace      (cPlayer * a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace, const cItem & a_HeldItem);
	bool CallHookBlockToPickup   (BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, const cPlayer * a_Player, const cItem & a_EquippedItem, cItems & a_Pickups);
	bool CallHookChat            (cPlayer * a_Player, const AString & a_Message);
	bool CallHookChunkGenerating (cWorld * a_World, int a_ChunkX, int a_ChunkZ, cLuaChunk * a_Chunk);
	bool CallHookCollectPickup   (cPlayer * a_Player, cPickup & a_Pickup);
	bool CallHookCraftingNoRecipe(const cPlayer * a_Player, const cCraftingGrid * a_Grid, cCraftingRecipe * a_Recipe);
	bool CallHookDisconnect      (cPlayer * a_Player, const AString & a_Reason);
	bool CallHookLogin           (cClientHandle * a_Client, int a_ProtocolVersion, const AString & a_Username);
	bool CallHookPostCrafting    (const cPlayer * a_Player, const cCraftingGrid * a_Grid, cCraftingRecipe * a_Recipe);
	bool CallHookPreCrafting     (const cPlayer * a_Player, const cCraftingGrid * a_Grid, cCraftingRecipe * a_Recipe);
	bool CallHookUpdatedSign     (cWorld * a_World, int a_BlockX, int a_BlockY, int a_BlockZ, const AString & a_Line1, const AString & a_Line2, const AString & a_Line3, const AString & a_Line4, cPlayer * a_Player);
	bool CallHookUpdatingSign    (cWorld * a_World, int a_BlockX, int a_BlockY, int a_BlockZ,       AString & a_Line1,       AString & a_Line2,       AString & a_Line3,       AString & a_Line4, cPlayer * a_Player);
	bool CallHookWeatherChanged  (cWorld * a_World);
	bool CallHookHandshake       (cClientHandle * a_ClientHandle, const AString & a_Username);

	void RemoveHooks( cPlugin* a_Plugin );
	void RemovePlugin( cPlugin* a_Plugin, bool a_bDelete = false );		//tolua_export
	void RemoveLuaPlugin( std::string a_FileName );						//tolua_export
	cPlugin_Lua* GetLuaPlugin( lua_State* a_State );					//tolua_export
	
	cLuaCommandBinder* GetLuaCommandBinder() const { return m_LuaCommandBinder; }
	
	cSquirrelCommandBinder* GetSquirrelCommandBinder() { return m_SquirrelCommandBinder; }

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
	cSquirrelCommandBinder* m_SquirrelCommandBinder;

	bool m_bReloadPlugins;
}; //tolua_export
