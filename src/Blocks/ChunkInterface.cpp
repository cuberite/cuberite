
#include "Globals.h"

#include "ChunkInterface.h"
#include "ChunkMap.h"
#include "BlockHandler.h"
#include "WorldInterface.h"





BLOCKTYPE cChunkInterface::GetBlock(Vector3i a_Pos)
{
	return m_ChunkMap->GetBlock(a_Pos.x, a_Pos.y, a_Pos.z);
}





NIBBLETYPE cChunkInterface::GetBlockMeta(Vector3i a_Pos)
{
	return m_ChunkMap->GetBlockMeta(a_Pos.x, a_Pos.y, a_Pos.z);
}






bool cChunkInterface::GetBlockTypeMeta(Vector3i a_Pos, BLOCKTYPE & a_BlockType, NIBBLETYPE & a_BlockMeta)
{
	return m_ChunkMap->GetBlockTypeMeta(a_Pos.x, a_Pos.y, a_Pos.z, a_BlockType, a_BlockMeta);
}






/** Sets the block at the specified coords to the specified value.
Full processing, incl. updating neighbors, is performed.
*/
void cChunkInterface::SetBlock(int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta)
{
	m_ChunkMap->SetBlock(a_BlockX, a_BlockY, a_BlockZ, a_BlockType, a_BlockMeta);
}





void cChunkInterface::SetBlockMeta(int a_BlockX, int a_BlockY, int a_BlockZ, NIBBLETYPE a_MetaData, bool a_ShouldMarkDirty, bool a_ShouldInformClient)
{
	m_ChunkMap->SetBlockMeta(a_BlockX, a_BlockY, a_BlockZ, a_MetaData, a_ShouldMarkDirty, a_ShouldInformClient);
}




/** Sets the block at the specified coords to the specified value.
The replacement doesn't trigger block updates.
The replaced blocks aren't checked for block entities (block entity is leaked if it exists at this block)
*/
void cChunkInterface::FastSetBlock(int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta)
{
	m_ChunkMap->FastSetBlock(a_BlockX, a_BlockY, a_BlockZ, a_BlockType, a_BlockMeta);
}






void cChunkInterface::FastSetBlock(const Vector3i & a_Pos, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta)
{
	FastSetBlock( a_Pos.x, a_Pos.y, a_Pos.z, a_BlockType, a_BlockMeta);
}






bool cChunkInterface::UseBlockEntity(cPlayer * a_Player, int a_BlockX, int a_BlockY, int a_BlockZ)
{
	return m_ChunkMap->UseBlockEntity(a_Player, a_BlockX, a_BlockY, a_BlockZ);
}






bool cChunkInterface::ForEachChunkInRect(int a_MinChunkX, int a_MaxChunkX, int a_MinChunkZ, int a_MaxChunkZ, cChunkDataCallback & a_Callback)
{
	return m_ChunkMap->ForEachChunkInRect(a_MinChunkX, a_MaxChunkX, a_MinChunkZ, a_MaxChunkZ, a_Callback);
}






bool cChunkInterface::WriteBlockArea(cBlockArea & a_Area, int a_MinBlockX, int a_MinBlockY, int a_MinBlockZ, int a_DataTypes)
{
	return m_ChunkMap->WriteBlockArea(a_Area, a_MinBlockX, a_MinBlockY, a_MinBlockZ, a_DataTypes);
}






bool cChunkInterface::DigBlock(cWorldInterface & a_WorldInterface, int a_X, int a_Y, int a_Z)
{
	cBlockHandler * Handler = cBlockInfo::GetHandler(GetBlock({a_X, a_Y, a_Z}));
	Handler->OnDestroyed(*this, a_WorldInterface, a_X, a_Y, a_Z);
	return m_ChunkMap->DigBlock(a_X, a_Y, a_Z);
}

