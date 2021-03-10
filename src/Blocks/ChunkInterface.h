
#pragma once

#include "../ForEachChunkProvider.h"
#include "../FunctionRef.h"
#include "ChunkDef.h"



// fwd:
class cItem;
class cChunk;
class cChunkMap;
class cWorldInterface;
class cPlayer;





class cChunkInterface:
	public cForEachChunkProvider
{
public:

	cChunkInterface(cChunkMap * a_ChunkMap) : m_ChunkMap(a_ChunkMap) {}

	bool DoWithChunkAt(Vector3i a_BlockPos, cFunctionRef<bool(cChunk &)> a_Callback);

	BlockState GetBlock(Vector3i a_Pos);
	BlockType GetType(Vector3i a_Pos) { return GetBlock(a_Pos).Type(); }

	/** Sets the block at the specified coords to the specified value.
	Full processing, incl. updating neighbors, is performed. */
	void SetBlock(Vector3i a_BlockPos, BlockState a_Block);

	/** OBSOLETE, use the Vector3i-based overload instead.
	Sets the block at the specified coords to the specified value.
	Full processing, incl. updating neighbors, is performed. */
	void SetBlock(int a_BlockX, int a_BlockY, int a_BlockZ, BlockState a_Block)
	{
		return SetBlock({a_BlockX, a_BlockY, a_BlockZ}, a_Block);
	}

	/** Sets the block at the specified coords to the specified value.
	The replacement doesn't trigger block updates.
	The replaced blocks aren't checked for block entities (block entity is leaked if it exists at this block). */
	void FastSetBlock(Vector3i a_BlockPos, BlockState a_Block);

	/** Use block entity on coordinate.
	returns true if the use was successful, return false to use the block as a "normal" block */
	bool UseBlockEntity(cPlayer * a_Player, int a_BlockX, int a_BlockY, int a_BlockZ);

	virtual bool ForEachChunkInRect(int a_MinChunkX, int a_MaxChunkX, int a_MinChunkZ, int a_MaxChunkZ, cChunkDataCallback & a_Callback) override;

	virtual bool WriteBlockArea(cBlockArea & a_Area, int a_MinBlockX, int a_MinBlockY, int a_MinBlockZ, int a_DataTypes) override;

	bool DigBlock(cWorldInterface & a_WorldInterface, Vector3i a_BlockPos, cEntity * a_Digger);

	/** Digs the block and spawns the relevant pickups, as if a_Digger used a_Tool to dig the block. */
	void DropBlockAsPickups(Vector3i a_BlockPos, const cEntity * a_Digger = nullptr, const cItem * a_Tool = nullptr);


private:
	cChunkMap * m_ChunkMap;
};




