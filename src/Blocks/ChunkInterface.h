
#pragma once

#include "../ForEachChunkProvider.h"



class cChunkMap;
class cWorldInterface;
class cPlayer;

class cChunkInterface:
	public cForEachChunkProvider
{
public:

	cChunkInterface(cChunkMap * a_ChunkMap) : m_ChunkMap(a_ChunkMap) {}

	BLOCKTYPE GetBlock(Vector3i a_Pos);
	NIBBLETYPE GetBlockMeta(Vector3i a_Pos);

	bool GetBlockTypeMeta(Vector3i a_Pos, BLOCKTYPE & a_BlockType, NIBBLETYPE & a_BlockMeta);

	/** Sets the block at the specified coords to the specified value.
	Full processing, incl. updating neighbors, is performed. */
	void SetBlock(Vector3i a_BlockPos, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta);

	/** OBSOLETE, use the Vector3i-based overload instead.
	Sets the block at the specified coords to the specified value.
	Full processing, incl. updating neighbors, is performed. */
	void SetBlock(int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta)
	{
		return SetBlock({a_BlockX, a_BlockY, a_BlockZ}, a_BlockType, a_BlockMeta);
	}

	/** Sets the meta for the specified block, while keeping the blocktype.
	If a_ShouldMarkDirty is true, the chunk is marked dirty by this change (false is used eg. by water turning still).
	If a_ShouldInformClients is true, the change is broadcast to all clients of the chunk.
	Ignored if the chunk is invalid. */
	void SetBlockMeta(Vector3i a_BlockPos, NIBBLETYPE a_MetaData, bool a_ShouldMarkDirty = true, bool a_ShouldInformClient = true);

	/** OBSOLETE, Use the Vector3-based overload instead.
	Sets the meta for the specified block, while keeping the blocktype.
	If a_ShouldMarkDirty is true, the chunk is marked dirty by this change (false is used eg. by water turning still).
	If a_ShouldInformClients is true, the change is broadcast to all clients of the chunk.
	Ignored if the chunk is invalid. */
	void SetBlockMeta(int a_BlockX, int a_BlockY, int a_BlockZ, NIBBLETYPE a_MetaData, bool a_ShouldMarkDirty = true, bool a_ShouldInformClient = true)
	{
		return SetBlockMeta({a_BlockX, a_BlockY, a_BlockZ}, a_MetaData, a_ShouldMarkDirty, a_ShouldInformClient);
	}

	/** Sets the block at the specified coords to the specified value.
	The replacement doesn't trigger block updates.
	The replaced blocks aren't checked for block entities (block entity is leaked if it exists at this block). */
	void FastSetBlock(Vector3i a_BlockPos, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta);

	/** Use block entity on coordinate.
	returns true if the use was successful, return false to use the block as a "normal" block */
	bool UseBlockEntity(cPlayer * a_Player, int a_BlockX, int a_BlockY, int a_BlockZ);

	virtual bool ForEachChunkInRect(int a_MinChunkX, int a_MaxChunkX, int a_MinChunkZ, int a_MaxChunkZ, cChunkDataCallback & a_Callback) override;

	virtual bool WriteBlockArea(cBlockArea & a_Area, int a_MinBlockX, int a_MinBlockY, int a_MinBlockZ, int a_DataTypes) override;

	bool DigBlock(cWorldInterface & a_WorldInterface, Vector3i a_BlockPos);

	/** Digs the block and spawns the relevant pickups, as if a_Digger used a_Tool to dig the block. */
	void DropBlockAsPickups(Vector3i a_BlockPos, const cEntity * a_Digger = nullptr, const cItem * a_Tool = nullptr);


private:
	cChunkMap * m_ChunkMap;
};




