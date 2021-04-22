
#include "Globals.h"

#include "ChunkInterface.h"
#include "BlockHandler.h"
#include "WorldInterface.h"
#include "../ChunkMap.h"
#include "../World.h"





bool cChunkInterface::DoWithChunkAt(Vector3i a_BlockPos, cChunkCallback a_Callback)
{
	return m_ChunkMap->DoWithChunkAt(a_BlockPos, a_Callback);
}





BLOCKTYPE cChunkInterface::GetBlock(Vector3i a_Pos)
{
	return m_ChunkMap->GetBlock(a_Pos);
}





NIBBLETYPE cChunkInterface::GetBlockMeta(Vector3i a_Pos)
{
	return m_ChunkMap->GetBlockMeta(a_Pos);
}





bool cChunkInterface::GetBlockTypeMeta(Vector3i a_Pos, BLOCKTYPE & a_BlockType, NIBBLETYPE & a_BlockMeta)
{
	return m_ChunkMap->GetBlockTypeMeta(a_Pos, a_BlockType, a_BlockMeta);
}





void cChunkInterface::SetBlock(Vector3i a_BlockPos, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta)
{
	m_ChunkMap->SetBlock(a_BlockPos, a_BlockType, a_BlockMeta);
}





void cChunkInterface::SetBlockMeta(Vector3i a_BlockPos, NIBBLETYPE a_MetaData)
{
	m_ChunkMap->SetBlockMeta(a_BlockPos, a_MetaData);
}





void cChunkInterface::FastSetBlock(Vector3i a_BlockPos, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta)
{
	m_ChunkMap->FastSetBlock(a_BlockPos, a_BlockType, a_BlockMeta);
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





bool cChunkInterface::DigBlock(cWorldInterface & a_WorldInterface, Vector3i a_BlockPos, cEntity * a_Digger)
{
	BLOCKTYPE BlockType;
	NIBBLETYPE BlockMeta;
	GetBlockTypeMeta(a_BlockPos, BlockType, BlockMeta);

	if (!m_ChunkMap->DigBlock(a_BlockPos))
	{
		return false;
	}

	cBlockHandler::For(BlockType).OnBroken(*this, a_WorldInterface, a_BlockPos, BlockType, BlockMeta, a_Digger);
	return true;
}





void cChunkInterface::DropBlockAsPickups(Vector3i a_BlockPos, const cEntity * a_Digger, const cItem * a_Tool)
{
	m_ChunkMap->GetWorld()->DropBlockAsPickups(a_BlockPos, a_Digger, a_Tool);
}
