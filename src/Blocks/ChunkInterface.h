
#pragma once

#include "../ChunkMap.h"
#include "../ForEachChunkProvider.h"
#include "WorldInterface.h"





class cChunkInterface:
	public cForEachChunkProvider
{
public:

	cChunkInterface(cChunkMap * a_ChunkMap) : m_ChunkMap(a_ChunkMap) {}

	BLOCKTYPE GetBlock(int a_BlockX, int a_BlockY, int a_BlockZ)
	{
		return m_ChunkMap->GetBlock(a_BlockX, a_BlockY, a_BlockZ);
	}
	BLOCKTYPE GetBlock(const Vector3i & a_Pos)
	{
		return GetBlock(a_Pos.x, a_Pos.y, a_Pos.z);
	}
	NIBBLETYPE GetBlockMeta(int a_BlockX, int a_BlockY, int a_BlockZ)
	{
		return m_ChunkMap->GetBlockMeta(a_BlockX, a_BlockY, a_BlockZ);
	}

	bool GetBlockTypeMeta(int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE & a_BlockType, NIBBLETYPE & a_BlockMeta)
	{
		return m_ChunkMap->GetBlockTypeMeta(a_BlockX, a_BlockY, a_BlockZ, a_BlockType, a_BlockMeta);
	}
	
	/** Sets the block at the specified coords to the specified value.
	Full processing, incl. updating neighbors, is performed.
	*/
	void SetBlock(cWorldInterface & a_WorldInterface, int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta)
	{
		m_ChunkMap->SetBlock(a_WorldInterface, a_BlockX, a_BlockY, a_BlockZ, a_BlockType, a_BlockMeta);
	}
	
	void SetBlockMeta(int a_BlockX, int a_BlockY, int a_BlockZ, NIBBLETYPE a_MetaData)
	{
		m_ChunkMap->SetBlockMeta(a_BlockX, a_BlockY, a_BlockZ, a_MetaData);
	}
	
	void QueueSetBlock(int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, int a_TickDelay, BLOCKTYPE a_PreviousBlockType, cWorldInterface &  a_WorldInterface)
	{
		m_ChunkMap->QueueSetBlock(a_BlockX, a_BlockY, a_BlockZ, a_BlockType, a_BlockMeta, a_WorldInterface.GetWorldAge() + a_TickDelay, a_PreviousBlockType);
	}
	
	/** Sets the block at the specified coords to the specified value.
	The replacement doesn't trigger block updates.
	The replaced blocks aren't checked for block entities (block entity is leaked if it exists at this block)
	*/
	void FastSetBlock(int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta)
	{
		m_ChunkMap->FastSetBlock(a_BlockX, a_BlockY, a_BlockZ, a_BlockType, a_BlockMeta);
	}
	
	void FastSetBlock(const Vector3i & a_Pos, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta)
	{
		FastSetBlock( a_Pos.x, a_Pos.y, a_Pos.z, a_BlockType, a_BlockMeta);
	}

	void UseBlockEntity(cPlayer * a_Player, int a_BlockX, int a_BlockY, int a_BlockZ)
	{
		m_ChunkMap->UseBlockEntity(a_Player, a_BlockX, a_BlockY, a_BlockZ);
	}
	
	virtual bool ForEachChunkInRect(int a_MinChunkX, int a_MaxChunkX, int a_MinChunkZ, int a_MaxChunkZ, cChunkDataCallback & a_Callback) override
	{
		return m_ChunkMap->ForEachChunkInRect(a_MinChunkX, a_MaxChunkX, a_MinChunkZ, a_MaxChunkZ, a_Callback);
	}
	
	virtual bool WriteBlockArea(cBlockArea & a_Area, int a_MinBlockX, int a_MinBlockY, int a_MinBlockZ, int a_DataTypes) override
	{
		return m_ChunkMap->WriteBlockArea(a_Area, a_MinBlockX, a_MinBlockY, a_MinBlockZ, a_DataTypes);
	}
	
	bool DigBlock(cWorldInterface & a_WorldInterface, int a_X, int a_Y, int a_Z);
	
private:
	cChunkMap * m_ChunkMap;
};




