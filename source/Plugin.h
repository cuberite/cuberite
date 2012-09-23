
#pragma once

#include "Item.h"

class cClientHandle;
class cPlayer;
class cPickup;
class cItem;
class cEntity;
class cPawn;
class cWorld;
class cLuaChunk;
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
	cPlugin();
	virtual ~cPlugin();
	
	virtual void OnDisable() {}
	virtual bool Initialize() = 0;

	// Called each tick
	virtual void Tick(float a_Dt);

	/**
	 * On all these functions, return true if you want to override default behavior
	 * You can also return false, so default behavior is used.
	 **/
	virtual bool OnBlockDig        (cPlayer * a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace, char a_Status, BLOCKTYPE a_OldBlock, NIBBLETYPE a_OldMeta);
	virtual bool OnBlockPlace      (cPlayer * a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace, const cItem & a_HeldItem);
	virtual bool OnBlockToPickup   (BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, const cPlayer * a_Player, const cItem & a_EquippedItem, cItems & a_Pickups);
	virtual bool OnChat            (cPlayer * a_Player, const AString & a_Message);
	virtual void OnChunkGenerated  (cWorld * a_World, int a_ChunkX, int a_ChunkZ);
	virtual bool OnChunkGenerating (cWorld * a_World, int a_ChunkX, int a_ChunkZ, cLuaChunk * a_pLuaChunk);
	virtual bool OnCollectPickup   (cPlayer * a_Player, cPickup * a_Pickup);
	virtual bool OnCraftingNoRecipe(const cPlayer * a_Player, const cCraftingGrid * a_Grid, cCraftingRecipe * a_Recipe);
	virtual bool OnDisconnect      (cPlayer * a_Player, const AString & a_Reason);
	virtual bool OnKilled          (cPawn * a_Killed, cEntity* a_Killer );
	virtual bool OnLogin           (cClientHandle * a_Client, int a_ProtocolVersion, const AString & a_Username);
	virtual bool OnPlayerJoin      (cPlayer* a_Player );
	virtual void OnPlayerMove      (cPlayer* a_Player );
	virtual void OnPlayerSpawn     (cPlayer* a_Player );
	virtual bool OnPostCrafting    (const cPlayer * a_Player, const cCraftingGrid * a_Grid, cCraftingRecipe * a_Recipe);
	virtual bool OnPreCrafting     (const cPlayer * a_Player, const cCraftingGrid * a_Grid, cCraftingRecipe * a_Recipe);
	virtual void OnTakeDamage      (cPawn * a_Pawn, TakeDamageInfo * a_TakeDamageInfo );
	virtual bool OnUpdatedSign     (cWorld * a_World, int a_BlockX, int a_BlockY, int a_BlockZ, const AString & a_Line1, const AString & a_Line2, const AString & a_Line3, const AString & a_Line4, cPlayer * a_Player);
	virtual bool OnUpdatingSign    (cWorld * a_World, int a_BlockX, int a_BlockY, int a_BlockZ,       AString & a_Line1,       AString & a_Line2,       AString & a_Line3,       AString & a_Line4, cPlayer * a_Player);
	virtual bool OnWeatherChanged  (cWorld * a_World);
	virtual bool OnHandshake       (cClientHandle * a_Client, const AString & a_Username);
	
	// Accessors
	const AString & GetName() const { return m_Name; }
	virtual void SetName( const AString & a_Name ) { m_Name = a_Name; }

	int GetVersion() const { return m_Version; }
	void SetVersion( int a_Version ) { m_Version = a_Version; }

	struct CommandStruct
	{
		std::string Command;
		std::string Description;
		std::string Permission;
	};

	void AddCommand(const AString & a_Command, const AString & a_Description, const AString & a_Permission);
	// tolua_end
	
	typedef bool (FuncCommandHandler)( std::string & a_Command, std::vector< std::string > & a_Split );
	void BindCommand( FuncCommandHandler* a_Function, std::string & a_Command );		// >> EXPORTED IN MANUALBINDINGS <<
	const std::vector< CommandStruct > & GetCommands() const { return m_Commands; }		// >> EXPORTED IN MANUALBINDINGS <<


	/* This should not be exposed to scripting languages */
	enum PluginLanguage
	{
		E_CPP,
		E_LUA,
		E_SQUIRREL,
	};
	PluginLanguage GetLanguage() { return m_Language; }
	void SetLanguage( PluginLanguage a_Language ) { m_Language = a_Language; }

	bool CanBindCommands() { return m_bCanBindCommands; }
private:
	friend class cPluginManager;
	bool m_bCanBindCommands;	// Only changed by cPluginManager

	PluginLanguage m_Language;
	std::vector< CommandStruct > m_Commands;
	std::string m_Name;
	int m_Version;
};	//tolua_export




