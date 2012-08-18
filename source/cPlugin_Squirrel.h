#pragma once
#include "cPlugin.h"
#include <sqrat.h>
#include "squirrelbindings/SquirrelObject.h"

class cPlugin_Squirrel : public cPlugin
{
public:
	cPlugin_Squirrel(const char* a_PluginName);
	~cPlugin_Squirrel();

	void OnDisable();
	bool Initialize();

	void Tick(float a_Dt);

	bool OnCollectItem     (cPickup* a_Pickup, cPlayer* a_Player ) override;
	bool OnDisconnect      (const AString & a_Reason, cPlayer * a_Player ) override;
	bool OnBlockPlace      (cPlayer * a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace, const cItem & a_HeldItem) override;
	bool OnBlockDig        (cPlayer * a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace, char a_Status, BLOCKTYPE a_OldBlock, NIBBLETYPE a_OldMeta) override;
	bool OnChat            (const char * a_Chat, cPlayer * a_Player ) override;
	bool OnLogin           (cClientHandle * a_Client, int a_ProtocolVersion, const AString & a_Username) override;
	void OnPlayerSpawn     (cPlayer* a_Player ) override;
	bool OnPlayerJoin      (cPlayer* a_Player ) override;
	void OnPlayerMove      (cPlayer* a_Player ) override;
	void OnTakeDamage      (cPawn* a_Pawn, TakeDamageInfo* a_TakeDamageInfo ) override;
	bool OnKilled          (cPawn* a_Killed, cEntity* a_Killer ) override;
	void OnChunkGenerated  (cWorld * a_World, int a_ChunkX, int a_ChunkZ) override;
	bool OnChunkGenerating (cWorld * a_World, int a_ChunkX, int a_ChunkZ, cLuaChunk * a_pLuaChunk ) override;
	bool OnPreCrafting     (const cPlayer * a_Player, const cCraftingGrid * a_Grid, cCraftingRecipe * a_Recipe) override;
	bool OnCraftingNoRecipe(const cPlayer * a_Player, const cCraftingGrid * a_Grid, cCraftingRecipe * a_Recipe) override;
	bool OnPostCrafting    (const cPlayer * a_Player, const cCraftingGrid * a_Grid, cCraftingRecipe * a_Recipe) override;
	bool OnBlockToPickup   (BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, const cPlayer * a_Player, const cItem & a_EquippedItem, cItems & a_Pickups);
	bool OnWeatherChanged  (cWorld * a_World) override;
	bool OnUpdatingSign    (cWorld * a_World, int a_BlockX, int a_BlockY, int a_BlockZ,       AString & a_Line1,       AString & a_Line2,       AString & a_Line3,       AString & a_Line4) override;
	bool OnUpdatedSign     (cWorld * a_World, int a_BlockX, int a_BlockY, int a_BlockZ, const AString & a_Line1, const AString & a_Line2, const AString & a_Line3, const AString & a_Line4) override;

protected:
	const char * m_PluginName;
	cCriticalSection m_CriticalSection;
	SquirrelObject *m_Plugin;
};