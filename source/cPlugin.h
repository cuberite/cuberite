
#pragma once

#include "cItem.h"

class cPacket_BlockPlace;
class cPacket_PickupSpawn;
class cPacket_EntityEquipment;
class cPacket_Disconnect;
class cPacket_Chat;
class cPacket_BlockDig;
class cPacket_Login;
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
	 * You can also return false, so default behavior is used, but with changed PacketData
	 **/
	virtual bool OnCollectItem     (cPickup* a_Pickup, cPlayer* a_Player );
	virtual bool OnDisconnect      (const AString & a_Reason, cPlayer * a_Player );
	virtual bool OnBlockPlace      (cPacket_BlockPlace* a_PacketData, cPlayer* a_Player );
	virtual bool OnBlockDig        (cPacket_BlockDig * a_PacketData, cPlayer* a_Player, cItem* a_PickupItem ) { (void)a_PacketData; (void)a_Player; (void)a_PickupItem; return false; }
	virtual bool OnChat            (const char * a_Chat, cPlayer* a_Player );
	virtual bool OnLogin           (cPacket_Login* a_PacketData );
	virtual void OnPlayerSpawn     (cPlayer* a_Player );
	virtual bool OnPlayerJoin      (cPlayer* a_Player );
	virtual void OnPlayerMove      (cPlayer* a_Player ) { (void)a_Player; }
	virtual void OnTakeDamage      (cPawn* a_Pawn, TakeDamageInfo* a_TakeDamageInfo ) { (void)a_Pawn; (void)a_TakeDamageInfo; }
	virtual bool OnKilled          (cPawn* a_Killed, cEntity* a_Killer ) { (void)a_Killed; (void)a_Killer; return false; }
	virtual void OnChunkGenerated  (cWorld * a_World, int a_ChunkX, int a_ChunkZ) {}
	virtual bool OnChunkGenerating (int a_ChunkX, int a_ChunkZ, cLuaChunk * a_pLuaChunk ) { return false; }
	virtual bool OnPreCrafting     (const cPlayer * a_Player, const cCraftingGrid * a_Grid, cCraftingRecipe * a_Recipe) {return false; }
	virtual bool OnCraftingNoRecipe(const cPlayer * a_Player, const cCraftingGrid * a_Grid, cCraftingRecipe * a_Recipe) {return false; }
	virtual bool OnPostCrafting    (const cPlayer * a_Player, const cCraftingGrid * a_Grid, cCraftingRecipe * a_Recipe) {return false; }
	virtual bool OnBlockToPickup   (BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, const cPlayer * a_Player, const cItem & a_EquippedItem, cItems & a_Pickups) {return false; }
	
	// Accessors
	const char* GetName() const { return m_Name.c_str(); }
	void SetName( const char* a_Name ) { m_Name = a_Name; }

	int GetVersion() const { return m_Version; }
	void SetVersion( int a_Version ) { m_Version = a_Version; }

	struct CommandStruct
	{
		std::string Command;
		std::string Description;
		std::string Permission;
	};

	void AddCommand( std::string & a_Command, std::string & a_Description, std::string & a_Permission );
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




