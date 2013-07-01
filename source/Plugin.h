
#pragma once

#include "Item.h"
#include "PluginManager.h"





class cClientHandle;
class cPlayer;
class cPickup;
class cItem;
class cEntity;
class cWorld;
class cChunkDesc;
struct TakeDamageInfo;

// fwd: cPlayer.h
class cPlayer;

// fwd: CraftingRecipes.h
class cCraftingGrid;
class cCraftingRecipe;





// tolua_begin
class cPlugin
{
public:
	// tolua_end
	
	cPlugin( const AString & a_PluginDirectory );
	virtual ~cPlugin();
	
	virtual void OnDisable(void) {}
	virtual bool Initialize(void) = 0;

	// Called each tick
	virtual void Tick(float a_Dt);

	/**
	 * On all these functions, return true if you want to override default behavior and not call other plugins on that callback.
	 * You can also return false, so default behavior is used.
	 **/
	virtual bool OnBlockToPickups     (cWorld * a_World, cEntity * a_Digger, int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, cItems & a_Pickups);
	virtual bool OnChat               (cPlayer * a_Player, AString & a_Message);
	virtual bool OnChunkAvailable     (cWorld * a_World, int a_ChunkX, int a_ChunkZ);
	virtual bool OnChunkGenerated     (cWorld * a_World, int a_ChunkX, int a_ChunkZ, cChunkDesc * a_ChunkDesc);
	virtual bool OnChunkGenerating    (cWorld * a_World, int a_ChunkX, int a_ChunkZ, cChunkDesc * a_ChunkDesc);
	virtual bool OnChunkUnloaded      (cWorld * a_World, int a_ChunkX, int a_ChunkZ);
	virtual bool OnChunkUnloading     (cWorld * a_World, int a_ChunkX, int a_ChunkZ);
	virtual bool OnCollectingPickup   (cPlayer * a_Player, cPickup * a_Pickup);
	virtual bool OnCraftingNoRecipe   (const cPlayer * a_Player, const cCraftingGrid * a_Grid, cCraftingRecipe * a_Recipe);
	virtual bool OnDisconnect         (cPlayer * a_Player, const AString & a_Reason);
	virtual bool OnExecuteCommand     (cPlayer * a_Player, const AStringVector & a_Split);
	virtual bool OnHandshake          (cClientHandle * a_Client, const AString & a_Username);
	virtual bool OnKilling            (cEntity & a_Victim, cEntity * a_Killer);
	virtual bool OnLogin              (cClientHandle * a_Client, int a_ProtocolVersion, const AString & a_Username);
	virtual bool OnPlayerBreakingBlock(cPlayer & a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta);
	virtual bool OnPlayerBrokenBlock  (cPlayer & a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta);
	virtual bool OnPlayerEating       (cPlayer & a_Player);
	virtual bool OnPlayerJoined       (cPlayer & a_Player);
	virtual bool OnPlayerLeftClick    (cPlayer & a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace, char a_Status);
	virtual bool OnPlayerMoved        (cPlayer & a_Player);
	virtual bool OnPlayerPlacedBlock  (cPlayer & a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace, int a_CursorX, int a_CursorY, int a_CursorZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta);
	virtual bool OnPlayerPlacingBlock (cPlayer & a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace, int a_CursorX, int a_CursorY, int a_CursorZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta);
	virtual bool OnPlayerRightClick   (cPlayer & a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace, int a_CursorX, int a_CursorY, int a_CursorZ);
	virtual bool OnPlayerShooting     (cPlayer & a_Player);
	virtual bool OnPlayerSpawned      (cPlayer & a_Player);
	virtual bool OnPlayerTossingItem  (cPlayer & a_Player);
	virtual bool OnPlayerUsedBlock    (cPlayer & a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace, int a_CursorX, int a_CursorY, int a_CursorZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta);
	virtual bool OnPlayerUsedItem     (cPlayer & a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace, int a_CursorX, int a_CursorY, int a_CursorZ);
	virtual bool OnPlayerUsingBlock   (cPlayer & a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace, int a_CursorX, int a_CursorY, int a_CursorZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta);
	virtual bool OnPlayerUsingItem    (cPlayer & a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace, int a_CursorX, int a_CursorY, int a_CursorZ);
	virtual bool OnPostCrafting       (const cPlayer * a_Player, const cCraftingGrid * a_Grid, cCraftingRecipe * a_Recipe);
	virtual bool OnPreCrafting        (const cPlayer * a_Player, const cCraftingGrid * a_Grid, cCraftingRecipe * a_Recipe);
	virtual bool OnTakeDamage         (cEntity & a_Receiver, TakeDamageInfo & a_TakeDamageInfo);
	virtual bool OnUpdatedSign        (cWorld * a_World, int a_BlockX, int a_BlockY, int a_BlockZ, const AString & a_Line1, const AString & a_Line2, const AString & a_Line3, const AString & a_Line4, cPlayer * a_Player);
	virtual bool OnUpdatingSign       (cWorld * a_World, int a_BlockX, int a_BlockY, int a_BlockZ,       AString & a_Line1,       AString & a_Line2,       AString & a_Line3,       AString & a_Line4, cPlayer * a_Player);
	virtual bool OnWeatherChanged     (cWorld & a_World);
	virtual bool OnWeatherChanging    (cWorld & a_World, eWeather & a_NewWeather);
	
	/** Handles the command split into a_Split, issued by player a_Player.
	Command permissions have already been checked.
	Returns true if command handled successfully
	*/
	virtual bool HandleCommand(const AStringVector & a_Split, cPlayer * a_Player);
	
	/** Handles the console command split into a_Split.
	Returns true if command handled successfully. Output is to be sent to the a_Output callback.
	*/
	virtual bool HandleConsoleCommand(const AStringVector & a_Split, cCommandOutputCallback & a_Output);
	
	/// All bound commands are to be removed, do any language-dependent cleanup here
	virtual void ClearCommands(void) {} ;
	
	/// All bound console commands are to be removed, do any language-dependent cleanup here
	virtual void ClearConsoleCommands(void) {} ;
	
	/** Called from cPluginManager::AddHook() to check if the hook can be added.
	Plugin API providers may check if the plugin is written correctly (has the hook handler function)
	Returns true if the hook can be added (handler exists)
	Descendants should also log the specific error message as a warning if they return false.
	*/
	virtual bool CanAddHook(cPluginManager::PluginHook a_Hook) { return false; }
	
	// tolua_begin
	const AString & GetName(void) const  { return m_Name; }
	void SetName(const AString & a_Name) { m_Name = a_Name; }

	int GetVersion(void) const     { return m_Version; }
	void SetVersion(int a_Version) { m_Version = a_Version; }

	const AString & GetDirectory(void) const {return m_Directory; }
	AString GetLocalDirectory(void) const;
	// tolua_end
	

	/* This should not be exposed to scripting languages */
	enum PluginLanguage
	{
		E_CPP,
		E_LUA,
		E_SQUIRREL,
	};
	PluginLanguage GetLanguage() { return m_Language; }
	void SetLanguage( PluginLanguage a_Language ) { m_Language = a_Language; }

private:
	PluginLanguage m_Language;
	AString m_Name;
	int m_Version;

	AString m_Directory;
};	// tolua_export




