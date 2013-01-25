
#pragma once

#include "Item.h"

struct lua_State;
class cLuaCommandBinder;

#ifdef USE_SQUIRREL
	class cSquirrelCommandBinder;
#endif  // USE_SQUIRREL

class cPlugin;

// fwd: World.h
class cWorld;

// fwd: ChunkDesc.h
class cChunkDesc;

// fwd: Player.h
class cPlayer;

// fwd: CraftingRecipes.h
class cCraftingGrid;
class cCraftingRecipe;

// fwd: Pickup.h
class cPickup;

// fwd: Pawn.h
struct TakeDamageInfo;
class cPawn;





class cPluginManager													// tolua_export
{																		// tolua_export
public:																	// tolua_export

	// Called each tick
	virtual void Tick(float a_Dt);

	// tolua_begin
	enum PluginHook
	{
		HOOK_CHAT,
		HOOK_CHUNK_GENERATED,
		HOOK_CHUNK_GENERATING,
		HOOK_COLLECT_PICKUP,
		HOOK_CRAFTING_NO_RECIPE,
		HOOK_DISCONNECT,
		HOOK_HANDSHAKE,
		HOOK_KILLED,
		HOOK_LOGIN,
		HOOK_PLAYER_BREAKING_BLOCK,
		HOOK_PLAYER_BROKEN_BLOCK,
		HOOK_PLAYER_EATING,
		HOOK_PLAYER_JOINED,
		HOOK_PLAYER_LEFT_CLICK,
		HOOK_PLAYER_MOVED,
		HOOK_PLAYER_PLACED_BLOCK,
		HOOK_PLAYER_PLACING_BLOCK,
		HOOK_PLAYER_RIGHT_CLICK,
		HOOK_PLAYER_SHOOTING,
		HOOK_PLAYER_SPAWNED,
		HOOK_PLAYER_TOSSING_ITEM,
		HOOK_PLAYER_USED_BLOCK,
		HOOK_PLAYER_USED_ITEM,
		HOOK_PLAYER_USING_BLOCK,
		HOOK_PLAYER_USING_ITEM,
		HOOK_POST_CRAFTING,
		HOOK_PRE_CRAFTING,
		HOOK_TAKE_DAMAGE,
		HOOK_TICK,
		HOOK_UPDATED_SIGN,
		HOOK_UPDATING_SIGN,
		HOOK_WEATHER_CHANGED,
		
		// Note that if a hook type is added, it may need processing in cPlugin::CanAddHook() descendants!
	} ;
	// tolua_end

	static cPluginManager * GetPluginManager();							// tolua_export

	typedef std::map< AString, cPlugin * > PluginMap;
	typedef std::list< cPlugin * > PluginList;
	cPlugin * GetPlugin( const AString & a_Plugin ) const;				// tolua_export
	const PluginMap & GetAllPlugins() const;							// >> EXPORTED IN MANUALBINDINGS <<

	void FindPlugins();													// tolua_export
	void ReloadPlugins();												// tolua_export
	bool AddPlugin( cPlugin* a_Plugin );
	void AddHook( cPlugin* a_Plugin, PluginHook a_Hook );				// tolua_export

	unsigned int GetNumPlugins() const;									// tolua_export

	// TODO: bool CallHookBlockToPickup   (BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, const cPlayer * a_Player, const cItem & a_EquippedItem, cItems & a_Pickups);
	bool CallHookChat               (cPlayer * a_Player, const AString & a_Message);
	bool CallHookChunkGenerated     (cWorld * a_World, int a_ChunkX, int a_ChunkZ);
	bool CallHookChunkGenerating    (cWorld * a_World, int a_ChunkX, int a_ChunkZ, cChunkDesc * a_Chunk);
	bool CallHookCollectPickup      (cPlayer * a_Player, cPickup & a_Pickup);
	bool CallHookCraftingNoRecipe   (const cPlayer * a_Player, const cCraftingGrid * a_Grid, cCraftingRecipe * a_Recipe);
	bool CallHookDisconnect         (cPlayer * a_Player, const AString & a_Reason);
	bool CallHookHandshake          (cClientHandle * a_ClientHandle, const AString & a_Username);
	bool CallHookKilled             (cPawn & a_Victim, cEntity * a_Killer);
	bool CallHookLogin              (cClientHandle * a_Client, int a_ProtocolVersion, const AString & a_Username);
	bool CallHookPlayerBreakingBlock(cPlayer & a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta);
	bool CallHookPlayerBrokenBlock  (cPlayer & a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta);
	bool CallHookPlayerEating       (cPlayer & a_Player);
	bool CallHookPlayerJoined       (cPlayer & a_Player);
	bool CallHookPlayerMoved        (cPlayer & a_Player);
	bool CallHookPlayerLeftClick    (cPlayer & a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace, char a_Status);
	bool CallHookPlayerPlacedBlock  (cPlayer & a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace, int a_CursorX, int a_CursorY, int a_CursorZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta);
	bool CallHookPlayerPlacingBlock (cPlayer & a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace, int a_CursorX, int a_CursorY, int a_CursorZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta);
	bool CallHookPlayerRightClick   (cPlayer & a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace, int a_CursorX, int a_CursorY, int a_CursorZ);
	bool CallHookPlayerShooting     (cPlayer & a_Player);
	bool CallHookPlayerSpawned      (cPlayer & a_Player);
	bool CallHookPlayerTossingItem  (cPlayer & a_Player);
	bool CallHookPlayerUsedBlock    (cPlayer & a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace, int a_CursorX, int a_CursorY, int a_CursorZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta);
	bool CallHookPlayerUsedItem     (cPlayer & a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace, int a_CursorX, int a_CursorY, int a_CursorZ);
	bool CallHookPlayerUsingBlock   (cPlayer & a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace, int a_CursorX, int a_CursorY, int a_CursorZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta);
	bool CallHookPlayerUsingItem    (cPlayer & a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace, int a_CursorX, int a_CursorY, int a_CursorZ);
	bool CallHookPostCrafting       (const cPlayer * a_Player, const cCraftingGrid * a_Grid, cCraftingRecipe * a_Recipe);
	bool CallHookPreCrafting        (const cPlayer * a_Player, const cCraftingGrid * a_Grid, cCraftingRecipe * a_Recipe);
	bool CallHookTakeDamage         (cPawn & a_Receiver, TakeDamageInfo & a_TDI);
	bool CallHookUpdatedSign        (cWorld * a_World, int a_BlockX, int a_BlockY, int a_BlockZ, const AString & a_Line1, const AString & a_Line2, const AString & a_Line3, const AString & a_Line4, cPlayer * a_Player);
	bool CallHookUpdatingSign       (cWorld * a_World, int a_BlockX, int a_BlockY, int a_BlockZ,       AString & a_Line1,       AString & a_Line2,       AString & a_Line3,       AString & a_Line4, cPlayer * a_Player);
	bool CallHookWeatherChanged     (cWorld * a_World);

	bool DisablePlugin( AString & a_PluginName );						// tolua_export
	bool LoadPlugin( AString & a_PluginName );							// tolua_export

	void RemoveHooks( cPlugin * a_Plugin );
	void RemovePlugin( cPlugin * a_Plugin, bool a_bDelete = false );
	
	cLuaCommandBinder* GetLuaCommandBinder() const { return m_LuaCommandBinder; }
	
	#ifdef USE_SQUIRREL
		cSquirrelCommandBinder * GetSquirrelCommandBinder() { return m_SquirrelCommandBinder; }
	#endif  // USE_SQUIRREL

	bool HasPlugin( cPlugin* a_Plugin ) const;
	
private:

	friend class cRoot;
	
	cPluginManager();
	~cPluginManager();

	typedef std::map< cPluginManager::PluginHook, cPluginManager::PluginList > HookMap;

	PluginList m_DisablePluginList;

	PluginMap m_Plugins;
	HookMap m_Hooks;

	void ReloadPluginsNow();
	void UnloadPluginsNow();
	
	cLuaCommandBinder * m_LuaCommandBinder;
	
	#ifdef USE_SQUIRREL
		cSquirrelCommandBinder * m_SquirrelCommandBinder;
	#endif  // USE_SQUIRREL

	bool m_bReloadPlugins;
}; // tolua_export




