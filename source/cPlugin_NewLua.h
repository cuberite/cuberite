
#pragma once

#include "cPlugin.h"
#include "cWebPlugin.h"





typedef struct lua_State lua_State;
class cWebPlugin_Lua;





class cPlugin_NewLua : public cPlugin, public cWebPlugin	//tolua_export
{															//tolua_export
public:														//tolua_export
	cPlugin_NewLua( const char* a_PluginName );
	~cPlugin_NewLua();

	virtual void OnDisable();	//tolua_export
	virtual bool Initialize();	//tolua_export

	virtual void Tick(float a_Dt);	//tolua_export

	virtual bool OnBlockDig        (cPlayer * a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace, char a_Status, BLOCKTYPE a_OldBlock, NIBBLETYPE a_OldMeta) override;
	virtual bool OnBlockPlace      (cPlayer * a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace, const cItem & a_HeldItem) override;
	virtual bool OnBlockToPickup   (BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, const cPlayer * a_Player, const cItem & a_EquippedItem, cItems & a_Pickups);
	virtual bool OnChat            (cPlayer * a_Player, const AString & a_Message) override;
	virtual void OnChunkGenerated  (cWorld * a_World, int a_ChunkX, int a_ChunkZ) override;
	virtual bool OnChunkGenerating (cWorld * a_World, int a_ChunkX, int a_ChunkZ, cLuaChunk * a_pLuaChunk ) override;
	virtual bool OnCollectItem     (cPickup* a_Pickup, cPlayer* a_Player ) override;
	virtual bool OnCraftingNoRecipe(const cPlayer * a_Player, const cCraftingGrid * a_Grid, cCraftingRecipe * a_Recipe) override;
	virtual bool OnDisconnect      (cPlayer * a_Player, const AString & a_Reason) override;
	virtual bool OnKilled          (cPawn* a_Killed, cEntity* a_Killer ) override;
	virtual bool OnLogin           (cClientHandle * a_Client, int a_ProtocolVersion, const AString & a_Username) override;
	virtual bool OnPlayerJoin      (cPlayer* a_Player ) override;
	virtual void OnPlayerMove      (cPlayer* a_Player ) override;
	virtual void OnPlayerSpawn     (cPlayer* a_Player ) override;
	virtual bool OnPostCrafting    (const cPlayer * a_Player, const cCraftingGrid * a_Grid, cCraftingRecipe * a_Recipe) override;
	virtual bool OnPreCrafting     (const cPlayer * a_Player, const cCraftingGrid * a_Grid, cCraftingRecipe * a_Recipe) override;
	virtual void OnTakeDamage      (cPawn* a_Pawn, TakeDamageInfo* a_TakeDamageInfo ) override;
	virtual bool OnUpdatedSign     (cWorld * a_World, int a_BlockX, int a_BlockY, int a_BlockZ, const AString & a_Line1, const AString & a_Line2, const AString & a_Line3, const AString & a_Line4) override;
	virtual bool OnUpdatingSign    (cWorld * a_World, int a_BlockX, int a_BlockY, int a_BlockZ,       AString & a_Line1,       AString & a_Line2,       AString & a_Line3,       AString & a_Line4) override;
	virtual bool OnWeatherChanged  (cWorld * a_World) override;

	virtual void SetName( const AString & a_Name ) override { cPlugin::SetName(a_Name); cWebPlugin::SetName(a_Name); }

	// cWebPlugin and WebAdmin stuff
	virtual AString HandleWebRequest( HTTPRequest * a_Request ) override;
	bool AddWebTab( const AString & a_Title, lua_State * a_LuaState, int a_FunctionReference );	// >> EXPORTED IN MANUALBINDINGS <<
	OBSOLETE bool AddTab( const AString & a_Title, lua_State * a_LuaState, int a_FunctionReference ); // >> EXPORTED IN MANUALBINDINGS <<

	lua_State* GetLuaState() { return m_LuaState; }

	OBSOLETE cPlugin_NewLua * CreateWebPlugin(lua_State* a_LuaState);	//tolua_export

	cCriticalSection & GetCriticalSection() { return m_CriticalSection; }
private:
	bool PushFunction( const char* a_FunctionName, bool a_bLogError = true );
	bool CallFunction( int a_NumArgs, int a_NumResults, const char* a_FunctionName ); // a_FunctionName is only used for error messages, nothing else

	cCriticalSection m_CriticalSection;

	std::string m_Directory;
	lua_State* m_LuaState;
};//tolua_export