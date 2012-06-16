
#pragma once

#include "cPlugin.h"





typedef struct lua_State lua_State;
class cWebPlugin_Lua;





class cPlugin_NewLua : public cPlugin						//tolua_export
{															//tolua_export
public:														//tolua_export
	cPlugin_NewLua( const char* a_PluginName );
	~cPlugin_NewLua();

	virtual void OnDisable();	//tolua_export
	virtual bool Initialize();	//tolua_export

	virtual void Tick(float a_Dt);	//tolua_export

	virtual bool OnCollectItem     (cPickup* a_Pickup, cPlayer* a_Player ) override;
	virtual bool OnDisconnect      (const AString & a_Reason, cPlayer * a_Player ) override;
	virtual bool OnBlockPlace      (cPacket_BlockPlace* a_PacketData, cPlayer* a_Player ) override;
	virtual bool OnBlockDig        (cPacket_BlockDig* a_PacketData, cPlayer* a_Player, cItem* a_PickupItem ) override;
	virtual bool OnChat            (const char* a_Chat, cPlayer* a_Player ) override;
	virtual bool OnLogin           (cPacket_Login* a_PacketData ) override;
	virtual void OnPlayerSpawn     (cPlayer* a_Player ) override;
	virtual bool OnPlayerJoin      (cPlayer* a_Player ) override;
	virtual void OnPlayerMove      (cPlayer* a_Player ) override;
	virtual void OnTakeDamage      (cPawn* a_Pawn, TakeDamageInfo* a_TakeDamageInfo ) override;
	virtual bool OnKilled          (cPawn* a_Killed, cEntity* a_Killer ) override;
	virtual void OnChunkGenerated  (cWorld * a_World, int a_ChunkX, int a_ChunkZ) override;
	virtual bool OnChunkGenerating (cWorld * a_World, int a_ChunkX, int a_ChunkZ, cLuaChunk * a_pLuaChunk ) override;
	virtual bool OnPreCrafting     (const cPlayer * a_Player, const cCraftingGrid * a_Grid, cCraftingRecipe * a_Recipe) override;
	virtual bool OnCraftingNoRecipe(const cPlayer * a_Player, const cCraftingGrid * a_Grid, cCraftingRecipe * a_Recipe) override;
	virtual bool OnPostCrafting    (const cPlayer * a_Player, const cCraftingGrid * a_Grid, cCraftingRecipe * a_Recipe) override;
	virtual bool OnBlockToPickup   (BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, const cPlayer * a_Player, const cItem & a_EquippedItem, cItems & a_Pickups);
	virtual bool OnWeatherChanged  (cWorld * a_World) override;
	virtual bool OnUpdatingSign    (cWorld * a_World, int a_BlockX, int a_BlockY, int a_BlockZ,       AString & a_Line1,       AString & a_Line2,       AString & a_Line3,       AString & a_Line4) override;
	virtual bool OnUpdatedSign     (cWorld * a_World, int a_BlockX, int a_BlockY, int a_BlockZ, const AString & a_Line1, const AString & a_Line2, const AString & a_Line3, const AString & a_Line4) override;

	lua_State* GetLuaState() { return m_LuaState; }

	cWebPlugin_Lua* CreateWebPlugin(lua_State* a_LuaState);	//tolua_export
private:
	bool PushFunction( const char* a_FunctionName, bool a_bLogError = true );
	bool CallFunction( int a_NumArgs, int a_NumResults, const char* a_FunctionName ); // a_FunctionName is only used for error messages, nothing else

	typedef std::list< cWebPlugin_Lua* > WebPluginList;
	WebPluginList m_WebPlugins;

	cCriticalSection m_CriticalSection;

	std::string m_Directory;
	lua_State* m_LuaState;
};//tolua_export