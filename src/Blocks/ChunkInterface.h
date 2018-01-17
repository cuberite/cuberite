
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
	Full processing, incl. updating neighbors, is performed.
	*/
	void SetBlock(int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta);

	void SetBlockMeta(int a_BlockX, int a_BlockY, int a_BlockZ, NIBBLETYPE a_MetaData, bool a_ShouldMarkDirty = true, bool a_ShouldInformClient = true);

	/** Sets the block at the specified coords to the specified value.
	The replacement doesn't trigger block updates.
	The replaced blocks aren't checked for block entities (block entity is leaked if it exists at this block)
	*/
	void FastSetBlock(int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta);

	void FastSetBlock(const Vector3i & a_Pos, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta);

	/** Use block entity on coordinate.
	returns true if the use was successful, return false to use the block as a "normal" block */
	bool UseBlockEntity(cPlayer * a_Player, int a_BlockX, int a_BlockY, int a_BlockZ);

	virtual bool ForEachChunkInRect(int a_MinChunkX, int a_MaxChunkX, int a_MinChunkZ, int a_MaxChunkZ, cChunkDataCallback & a_Callback) override;

	virtual bool WriteBlockArea(cBlockArea & a_Area, int a_MinBlockX, int a_MinBlockY, int a_MinBlockZ, int a_DataTypes) override;

	bool DigBlock(cWorldInterface & a_WorldInterface, int a_X, int a_Y, int a_Z);

private:
	cChunkMap * m_ChunkMap;
};




