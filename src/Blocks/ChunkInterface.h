
#pragma once

class cChunkInterface
{
public:

	BLOCKTYPE  GetBlock          (int a_BlockX, int a_BlockY, int a_BlockZ);
	BLOCKTYPE  GetBlock    (const Vector3i & a_Pos ) { return GetBlock( a_Pos.x, a_Pos.y, a_Pos.z ); }
	NIBBLETYPE GetBlockMeta      (int a_BlockX, int a_BlockY, int a_BlockZ);

	/** Sets the block at the specified coords to the specified value.
	Full processing, incl. updating neighbors, is performed.
	*/
	void SetBlock(int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta);
	
	
	/** Sets the block at the specified coords to the specified value.
	The replacement doesn't trigger block updates.
	The replaced blocks aren't checked for block entities (block entity is leaked if it exists at this block)
	*/
	void FastSetBlock(int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta);	
	void       FastSetBlock(const Vector3i & a_Pos, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta ) { FastSetBlock( a_Pos.x, a_Pos.y, a_Pos.z, a_BlockType, a_BlockMeta ); }
	
	void DoExplosionAt(double a_ExplosionSize, double a_BlockX, double a_BlockY, double a_BlockZ, bool a_CanCauseFire, eExplosionSource a_Source, void * a_SourceData);  
};
