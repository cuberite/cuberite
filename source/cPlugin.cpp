
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "cPlugin.h"





cPlugin::cPlugin()
	: m_Version( 0 )
	, m_Language( E_CPP )
	, m_bCanBindCommands( false )
{
}

cPlugin::~cPlugin()
{
}

// bool cPlugin::Initialize()
// {
// 	LOG("cPlugin::Initialize()");
// 	return false;
// }





void cPlugin::Tick(float a_Dt)
{
	UNUSED(a_Dt);
}





bool cPlugin::OnBlockPlace(cPacket_BlockPlace * a_PacketData, cPlayer * a_Player)
{
	UNUSED(a_PacketData);
	UNUSED(a_Player);
	return false;
}





bool cPlugin::OnBlockDig(cPacket_BlockDig * a_PacketData, cPlayer * a_Player, cItem * a_PickupItem)
{
	UNUSED(a_PacketData);
	UNUSED(a_Player);
	UNUSED(a_PickupItem);
	return false;
}





bool cPlugin::OnCollectItem(cPickup * a_Pickup, cPlayer * a_Player)
{
	UNUSED(a_Pickup);
	UNUSED(a_Player);
	return false;
}





bool cPlugin::OnDisconnect(const AString & a_Reason, cPlayer * a_Player)
{
	UNUSED(a_Reason);
	UNUSED(a_Player);
	return false;
}





bool cPlugin::OnChat(const char * a_Chat, cPlayer * a_Player)
{
	UNUSED(a_Chat);
	UNUSED(a_Player);
	return false;
}





bool cPlugin::OnLogin(cPacket_Login * a_PacketData)
{
	UNUSED(a_PacketData);
	return false;
}





void cPlugin::OnPlayerSpawn(cPlayer * a_Player)
{
	UNUSED(a_Player);
}





bool cPlugin::OnPlayerJoin(cPlayer * a_Player)
{
	UNUSED(a_Player);
	return false;
}





void cPlugin::OnPlayerMove(cPlayer * a_Player)
{
	UNUSED(a_Player);
}





void cPlugin::OnTakeDamage(cPawn * a_Pawn, TakeDamageInfo * a_TakeDamageInfo)
{
	UNUSED(a_Pawn);
	UNUSED(a_TakeDamageInfo);
}





bool cPlugin::OnKilled(cPawn * a_Killed, cEntity * a_Killer)
{
	UNUSED(a_Killed);
	UNUSED(a_Killer);
	return false;
}





void cPlugin::OnChunkGenerated(cWorld * a_World, int a_ChunkX, int a_ChunkZ)
{
	UNUSED(a_World);
	UNUSED(a_ChunkX);
	UNUSED(a_ChunkZ);
}





bool cPlugin::OnChunkGenerating(cWorld * a_World, int a_ChunkX, int a_ChunkZ, cLuaChunk * a_pLuaChunk)
{
	UNUSED(a_World);
	UNUSED(a_ChunkX);
	UNUSED(a_ChunkZ);
	UNUSED(a_pLuaChunk);
	return false;
}





bool cPlugin::OnPreCrafting(const cPlayer * a_Player, const cCraftingGrid * a_Grid, cCraftingRecipe * a_Recipe)
{
	UNUSED(a_Player);
	UNUSED(a_Grid);
	UNUSED(a_Recipe);
	return false;
}





bool cPlugin::OnCraftingNoRecipe(const cPlayer * a_Player, const cCraftingGrid * a_Grid, cCraftingRecipe * a_Recipe)
{
	UNUSED(a_Player);
	UNUSED(a_Grid);
	UNUSED(a_Recipe);
	return false;
}





bool cPlugin::OnPostCrafting(const cPlayer * a_Player, const cCraftingGrid * a_Grid, cCraftingRecipe * a_Recipe)
{
	UNUSED(a_Player);
	UNUSED(a_Grid);
	UNUSED(a_Recipe);
	return false;
}





bool cPlugin::OnBlockToPickup(BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, const cPlayer * a_Player, const cItem & a_EquippedItem, cItems & a_Pickups)
{
	UNUSED(a_BlockType);
	UNUSED(a_BlockMeta);
	UNUSED(a_Player);
	UNUSED(a_EquippedItem);
	UNUSED(a_Pickups);
	return false;
}





bool cPlugin::OnWeatherChanged(cWorld * a_World)
{
	UNUSED(a_World);
	return false;
}





bool cPlugin::OnUpdatingSign(cWorld * a_World, int a_BlockX, int a_BlockY, int a_BlockZ, AString & a_Line1, AString & a_Line2, AString & a_Line3, AString & a_Line4)
{
	UNUSED(a_World);
	UNUSED(a_BlockX);
	UNUSED(a_BlockY);
	UNUSED(a_BlockZ);
	UNUSED(a_Line1);
	UNUSED(a_Line2);
	UNUSED(a_Line3);
	UNUSED(a_Line4);
	return false;
}





bool cPlugin::OnUpdatedSign(cWorld * a_World, int a_BlockX, int a_BlockY, int a_BlockZ, const AString & a_Line1, const AString & a_Line2, const AString & a_Line3, const AString & a_Line4)
{
	UNUSED(a_World);
	UNUSED(a_BlockX);
	UNUSED(a_BlockY);
	UNUSED(a_BlockZ);
	UNUSED(a_Line1);
	UNUSED(a_Line2);
	UNUSED(a_Line3);
	UNUSED(a_Line4);
	return false;
}





void cPlugin::AddCommand(const AString & a_Command, const AString & a_Description, const AString & a_Permission)
{
	CommandStruct Command;
	Command.Command = a_Command;
	Command.Description = a_Description;
	Command.Permission = a_Permission;
	m_Commands.push_back( Command );
}




