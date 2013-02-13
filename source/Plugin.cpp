
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Plugin.h"
#include "Pawn.h"
#include "Player.h"
#include "World.h"





cPlugin::cPlugin( const AString & a_PluginDirectory )
	: m_Version(0)
	, m_Language(E_CPP)
	, m_Directory(a_PluginDirectory)
{
}





cPlugin::~cPlugin()
{
	LOGD("Destroying plugin \"%s\".", m_Name.c_str());
}





void cPlugin::Tick(float a_Dt)
{
	UNUSED(a_Dt);
}





bool cPlugin::OnBlockToPickups(cWorld * a_World, cEntity * a_Digger, int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, cItems & a_Pickups)
{
	UNUSED(a_World);
	UNUSED(a_Digger);
	UNUSED(a_BlockX);
	UNUSED(a_BlockY);
	UNUSED(a_BlockZ);
	UNUSED(a_BlockType);
	UNUSED(a_BlockMeta);
	UNUSED(a_Pickups);
	return false;
}






bool cPlugin::OnChat(cPlayer * a_Player, const AString & a_Message)
{
	UNUSED(a_Player);
	UNUSED(a_Message);
	return false;
}





bool cPlugin::OnChunkAvailable(cWorld * a_World, int a_ChunkX, int a_ChunkZ)
{
	UNUSED(a_World);
	UNUSED(a_ChunkX);
	UNUSED(a_ChunkZ);
	return false;
}





bool cPlugin::OnChunkGenerated(cWorld * a_World, int a_ChunkX, int a_ChunkZ, cChunkDesc * a_ChunkDesc)
{
	UNUSED(a_World);
	UNUSED(a_ChunkX);
	UNUSED(a_ChunkZ);
	UNUSED(a_ChunkDesc);
	return false;
}





bool cPlugin::OnChunkGenerating(cWorld * a_World, int a_ChunkX, int a_ChunkZ, cChunkDesc * a_ChunkDesc)
{
	UNUSED(a_World);
	UNUSED(a_ChunkX);
	UNUSED(a_ChunkZ);
	UNUSED(a_ChunkDesc);
	return false;
}





bool cPlugin::OnChunkUnloaded(cWorld * a_World, int a_ChunkX, int a_ChunkZ)
{
	UNUSED(a_World);
	UNUSED(a_ChunkX);
	UNUSED(a_ChunkZ);
	return false;
}





bool cPlugin::OnChunkUnloading(cWorld * a_World, int a_ChunkX, int a_ChunkZ)
{
	UNUSED(a_World);
	UNUSED(a_ChunkX);
	UNUSED(a_ChunkZ);
	return false;
}





bool cPlugin::OnCollectingPickup(cPlayer * a_Player, cPickup * a_Pickup)
{
	UNUSED(a_Player);
	UNUSED(a_Pickup);
	return false;
}





bool cPlugin::OnCraftingNoRecipe(const cPlayer * a_Player, const cCraftingGrid * a_Grid, cCraftingRecipe * a_Recipe)
{
	UNUSED(a_Player);
	UNUSED(a_Grid);
	UNUSED(a_Recipe);
	return false;
}





bool cPlugin::OnDisconnect(cPlayer * a_Player, const AString & a_Reason)
{
	UNUSED(a_Reason);
	UNUSED(a_Player);
	return false;
}





bool cPlugin::OnHandshake(cClientHandle * a_Client, const AString & a_Username)
{
	UNUSED(a_Client);
	UNUSED(a_Username);
	return false;
}





bool cPlugin::OnKilling(cPawn & a_Victim, cEntity * a_Killer)
{
	UNUSED(a_Victim);
	UNUSED(a_Killer);
	return false;
}





bool cPlugin::OnLogin(cClientHandle * a_Client, int a_ProtocolVersion, const AString & a_Username)
{
	UNUSED(a_Client);
	UNUSED(a_ProtocolVersion);
	UNUSED(a_Username);
	return false;
}





bool cPlugin::OnPlayerBreakingBlock(cPlayer & a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta)
{
	UNUSED(a_Player);
	UNUSED(a_BlockX);
	UNUSED(a_BlockY);
	UNUSED(a_BlockZ);
	UNUSED(a_BlockFace);
	UNUSED(a_BlockType);
	UNUSED(a_BlockMeta);
	return false;
}





bool cPlugin::OnPlayerBrokenBlock(cPlayer & a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta)
{
	UNUSED(a_Player);
	UNUSED(a_BlockX);
	UNUSED(a_BlockY);
	UNUSED(a_BlockZ);
	UNUSED(a_BlockFace);
	UNUSED(a_BlockType);
	UNUSED(a_BlockMeta);
	return false;
}





bool cPlugin::OnPlayerEating(cPlayer & a_Player)
{
	UNUSED(a_Player);
	return false;
}





bool cPlugin::OnPlayerJoined(cPlayer & a_Player)
{
	UNUSED(a_Player);
	return false;
}





bool cPlugin::OnPlayerLeftClick(cPlayer & a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace, char a_Status)
{
	UNUSED(a_Player);
	UNUSED(a_BlockX);
	UNUSED(a_BlockY);
	UNUSED(a_BlockZ);
	UNUSED(a_BlockFace);
	UNUSED(a_Status);
	return false;
}





bool cPlugin::OnPlayerMoved(cPlayer & a_Player)
{
	UNUSED(a_Player);
	return false;
}





bool cPlugin::OnPlayerPlacedBlock(cPlayer & a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace, int a_CursorX, int a_CursorY, int a_CursorZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta)
{
	UNUSED(a_Player);
	UNUSED(a_BlockX);
	UNUSED(a_BlockY);
	UNUSED(a_BlockZ);
	UNUSED(a_BlockFace);
	UNUSED(a_CursorX);
	UNUSED(a_CursorY);
	UNUSED(a_CursorZ);
	UNUSED(a_BlockType);
	UNUSED(a_BlockMeta);
	return false;
}





bool cPlugin::OnPlayerPlacingBlock(cPlayer & a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace, int a_CursorX, int a_CursorY, int a_CursorZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta)
{
	UNUSED(a_Player);
	UNUSED(a_BlockX);
	UNUSED(a_BlockY);
	UNUSED(a_BlockZ);
	UNUSED(a_BlockFace);
	UNUSED(a_CursorX);
	UNUSED(a_CursorY);
	UNUSED(a_CursorZ);
	UNUSED(a_BlockType);
	UNUSED(a_BlockMeta);
	return false;
}





bool cPlugin::OnPlayerRightClick(cPlayer & a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace, int a_CursorX, int a_CursorY, int a_CursorZ)
{
	UNUSED(a_Player);
	UNUSED(a_BlockX);
	UNUSED(a_BlockY);
	UNUSED(a_BlockZ);
	UNUSED(a_BlockFace);
	UNUSED(a_CursorX);
	UNUSED(a_CursorY);
	UNUSED(a_CursorZ);
	return false;
}





bool cPlugin::OnPlayerShooting(cPlayer & a_Player)
{
	UNUSED(a_Player);
	return false;
}





bool cPlugin::OnPlayerSpawned(cPlayer & a_Player)
{
	UNUSED(a_Player);
	return false;
}





bool cPlugin::OnPlayerTossingItem(cPlayer & a_Player)
{
	UNUSED(a_Player);
	return false;
}





bool cPlugin::OnPlayerUsedBlock(cPlayer & a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace, int a_CursorX, int a_CursorY, int a_CursorZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta)
{
	UNUSED(a_Player);
	UNUSED(a_BlockX);
	UNUSED(a_BlockY);
	UNUSED(a_BlockZ);
	UNUSED(a_BlockFace);
	UNUSED(a_CursorX);
	UNUSED(a_CursorY);
	UNUSED(a_CursorZ);
	UNUSED(a_BlockType);
	UNUSED(a_BlockMeta);
	return false;
}





bool cPlugin::OnPlayerUsedItem(cPlayer & a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace, int a_CursorX, int a_CursorY, int a_CursorZ)
{
	UNUSED(a_Player);
	UNUSED(a_BlockX);
	UNUSED(a_BlockY);
	UNUSED(a_BlockZ);
	UNUSED(a_BlockFace);
	UNUSED(a_CursorX);
	UNUSED(a_CursorY);
	UNUSED(a_CursorZ);
	return false;
}





bool cPlugin::OnPlayerUsingBlock(cPlayer & a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace, int a_CursorX, int a_CursorY, int a_CursorZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta)
{
	UNUSED(a_Player);
	UNUSED(a_BlockX);
	UNUSED(a_BlockY);
	UNUSED(a_BlockZ);
	UNUSED(a_BlockFace);
	UNUSED(a_CursorX);
	UNUSED(a_CursorY);
	UNUSED(a_CursorZ);
	UNUSED(a_BlockType);
	UNUSED(a_BlockMeta);
	return false;
}





bool cPlugin::OnPlayerUsingItem(cPlayer & a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace, int a_CursorX, int a_CursorY, int a_CursorZ)
{
	UNUSED(a_Player);
	UNUSED(a_BlockX);
	UNUSED(a_BlockY);
	UNUSED(a_BlockZ);
	UNUSED(a_BlockFace);
	UNUSED(a_CursorX);
	UNUSED(a_CursorY);
	UNUSED(a_CursorZ);
	return false;
}





bool cPlugin::OnPostCrafting(const cPlayer * a_Player, const cCraftingGrid * a_Grid, cCraftingRecipe * a_Recipe)
{
	UNUSED(a_Player);
	UNUSED(a_Grid);
	UNUSED(a_Recipe);
	return false;
}





bool cPlugin::OnPreCrafting(const cPlayer * a_Player, const cCraftingGrid * a_Grid, cCraftingRecipe * a_Recipe)
{
	UNUSED(a_Player);
	UNUSED(a_Grid);
	UNUSED(a_Recipe);
	return false;
}





bool cPlugin::OnTakeDamage(cPawn & a_Pawn, TakeDamageInfo & a_TakeDamageInfo)
{
	UNUSED(a_Pawn);
	UNUSED(a_TakeDamageInfo);
	return false;
}





bool cPlugin::OnUpdatedSign(cWorld * a_World, int a_BlockX, int a_BlockY, int a_BlockZ, const AString & a_Line1, const AString & a_Line2, const AString & a_Line3, const AString & a_Line4, cPlayer * a_Player)
{
	UNUSED(a_World);
	UNUSED(a_BlockX);
	UNUSED(a_BlockY);
	UNUSED(a_BlockZ);
	UNUSED(a_Line1);
	UNUSED(a_Line2);
	UNUSED(a_Line3);
	UNUSED(a_Line4);
	UNUSED(a_Player);
	return false;
}





bool cPlugin::OnUpdatingSign(cWorld * a_World, int a_BlockX, int a_BlockY, int a_BlockZ, AString & a_Line1, AString & a_Line2, AString & a_Line3, AString & a_Line4, cPlayer * a_Player)
{
	UNUSED(a_World);
	UNUSED(a_BlockX);
	UNUSED(a_BlockY);
	UNUSED(a_BlockZ);
	UNUSED(a_Line1);
	UNUSED(a_Line2);
	UNUSED(a_Line3);
	UNUSED(a_Line4);
	UNUSED(a_Player);
	return false;
}





bool cPlugin::OnWeatherChanged(cWorld & a_World)
{
	UNUSED(a_World);
	return false;
}





bool cPlugin::OnWeatherChanging(cWorld & a_World, eWeather & a_Weather)
{
	UNUSED(a_World);
	UNUSED(a_Weather);
	return false;
}





bool cPlugin::HandleCommand(const AStringVector & a_Split, cPlayer * a_Player)
{
	UNUSED(a_Split);
	UNUSED(a_Player);
	return false;
}





AString cPlugin::GetLocalDirectory(void) const
{
	return std::string("Plugins/") + m_Directory;
}