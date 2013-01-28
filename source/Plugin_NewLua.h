
#pragma once

#include "Plugin.h"
#include "WebPlugin.h"





typedef struct lua_State lua_State;





// tolua_begin
class cPlugin_NewLua :
	public cPlugin,
	public cWebPlugin
{
public:
	// tolua_end
	
	cPlugin_NewLua( const AString & a_PluginDirectory );
	~cPlugin_NewLua();

	virtual void OnDisable(void) override;
	virtual bool Initialize(void) override;

	virtual void Tick(float a_Dt) override;

	virtual bool OnBlockToPickups     (cWorld * a_World, cEntity * a_Digger, int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, cItems & a_Pickups) override;
	virtual bool OnChat               (cPlayer * a_Player, const AString & a_Message) override;
	virtual bool OnChunkGenerated     (cWorld * a_World, int a_ChunkX, int a_ChunkZ) override;
	virtual bool OnChunkGenerating    (cWorld * a_World, int a_ChunkX, int a_ChunkZ, cChunkDesc * a_pLuaChunk ) override;
	virtual bool OnCollectingPickup   (cPlayer * a_Player, cPickup * a_Pickup) override;
	virtual bool OnCraftingNoRecipe   (const cPlayer * a_Player, const cCraftingGrid * a_Grid, cCraftingRecipe * a_Recipe) override;
	virtual bool OnDisconnect         (cPlayer * a_Player, const AString & a_Reason) override;
	virtual bool OnHandshake          (cClientHandle * a_Client, const AString & a_Username) override;
	virtual bool OnKilling            (cPawn & a_Victim, cEntity * a_Killer) override;
	virtual bool OnLogin              (cClientHandle * a_Client, int a_ProtocolVersion, const AString & a_Username) override;
	virtual bool OnPlayerBreakingBlock(cPlayer & a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta) override;
	virtual bool OnPlayerBrokenBlock  (cPlayer & a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta) override;
	virtual bool OnPlayerEating       (cPlayer & a_Player) override;
	virtual bool OnPlayerJoined       (cPlayer & a_Player) override;
	virtual bool OnPlayerMoved        (cPlayer & a_Player) override;
	virtual bool OnPlayerLeftClick    (cPlayer & a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace, char a_Status) override;
	virtual bool OnPlayerPlacedBlock  (cPlayer & a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace, int a_CursorX, int a_CursorY, int a_CursorZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta) override;
	virtual bool OnPlayerPlacingBlock (cPlayer & a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace, int a_CursorX, int a_CursorY, int a_CursorZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta) override;
	virtual bool OnPlayerRightClick   (cPlayer & a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace, int a_CursorX, int a_CursorY, int a_CursorZ) override;
	virtual bool OnPlayerShooting     (cPlayer & a_Player) override;
	virtual bool OnPlayerSpawned      (cPlayer & a_Player) override;
	virtual bool OnPlayerTossingItem  (cPlayer & a_Player) override;
	virtual bool OnPlayerUsedBlock    (cPlayer & a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace, int a_CursorX, int a_CursorY, int a_CursorZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta) override;
	virtual bool OnPlayerUsedItem     (cPlayer & a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace, int a_CursorX, int a_CursorY, int a_CursorZ) override;
	virtual bool OnPlayerUsingBlock   (cPlayer & a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace, int a_CursorX, int a_CursorY, int a_CursorZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta) override;
	virtual bool OnPlayerUsingItem    (cPlayer & a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace, int a_CursorX, int a_CursorY, int a_CursorZ) override;
	virtual bool OnPostCrafting       (const cPlayer * a_Player, const cCraftingGrid * a_Grid, cCraftingRecipe * a_Recipe) override;
	virtual bool OnPreCrafting        (const cPlayer * a_Player, const cCraftingGrid * a_Grid, cCraftingRecipe * a_Recipe) override;
	virtual bool OnTakeDamage         (cPawn & a_Receiver, TakeDamageInfo & a_TakeDamageInfo) override;
	virtual bool OnUpdatedSign        (cWorld * a_World, int a_BlockX, int a_BlockY, int a_BlockZ, const AString & a_Line1, const AString & a_Line2, const AString & a_Line3, const AString & a_Line4, cPlayer * a_Player) override;
	virtual bool OnUpdatingSign       (cWorld * a_World, int a_BlockX, int a_BlockY, int a_BlockZ,       AString & a_Line1,       AString & a_Line2,       AString & a_Line3,       AString & a_Line4, cPlayer * a_Player) override;
	virtual bool OnWeatherChanged     (cWorld * a_World) override;
	
	virtual bool CanAddHook(cPluginManager::PluginHook a_Hook) override;
	
	// cWebPlugin override
	virtual const AString & GetWebTitle(void) const {return GetName(); }

	// cWebPlugin and WebAdmin stuff
	virtual AString HandleWebRequest( HTTPRequest * a_Request ) override;
	bool AddWebTab(const AString & a_Title, lua_State * a_LuaState, int a_FunctionReference);	// >> EXPORTED IN MANUALBINDINGS <<

	lua_State* GetLuaState() { return m_LuaState; }

	cCriticalSection & GetCriticalSection() { return m_CriticalSection; }
	
protected:
	bool PushFunction(const char * a_FunctionName, bool a_bLogError = true);
	bool CallFunction(int a_NumArgs, int a_NumResults, const char * a_FunctionName );  // a_FunctionName is only used for error messages, nothing else
	
	/// Returns the name of Lua function that should handle the specified hook
	const char * GetHookFnName(cPluginManager::PluginHook a_Hook);

	cCriticalSection m_CriticalSection;

	lua_State * m_LuaState;
} ;  // tolua_export




