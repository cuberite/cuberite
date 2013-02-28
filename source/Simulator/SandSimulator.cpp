
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "SandSimulator.h"
#include "../World.h"
#include "../BlockID.h"
#include "../Defines.h"
#include "../FallingBlock.h"
#include "../Chunk.h"





cSandSimulator::cSandSimulator(cWorld & a_World)
	: cSimulator(a_World)
	, m_Blocks(new BlockList)
	, m_Buffer(new BlockList)
{

}





cSandSimulator::~cSandSimulator()
{
	delete m_Buffer;
	delete m_Blocks;
}





void cSandSimulator::Simulate(float a_Dt)
{
	m_Buffer->clear();
	std::swap( m_Blocks, m_Buffer );

	for( BlockList::iterator itr = m_Buffer->begin(); itr != m_Buffer->end(); ++itr )
	{
		Vector3i Pos = *itr;
		BLOCKTYPE BlockID = m_World.GetBlock(Pos.x, Pos.y, Pos.z);
		if(!IsAllowedBlock(BlockID))
			continue;

		BLOCKTYPE BottomBlock = m_World.GetBlock( Pos.x, Pos.y - 1, Pos.z );
		
		if( IsPassable(BottomBlock) )
		{
			cFallingBlock * FallingBlock = new cFallingBlock(Pos, BlockID);
			FallingBlock->Initialize(&m_World);
 			m_World.SetBlock( Pos.x, Pos.y, Pos.z, E_BLOCK_AIR, 0 );
		}
	}
		
}





bool cSandSimulator::IsAllowedBlock( BLOCKTYPE a_BlockType )
{
	return a_BlockType == E_BLOCK_SAND
		|| a_BlockType == E_BLOCK_GRAVEL;
}





void cSandSimulator::AddBlock(int a_BlockX, int a_BlockY, int a_BlockZ, cChunk * a_Chunk)
{
	// TODO: Optimize this by passing the block type along
	int RelX = a_BlockX;
	int RelY = a_BlockY;
	int RelZ = a_BlockZ;
	int ChunkX, ChunkZ;
	cChunkDef::AbsoluteToRelative(RelX, RelY, RelZ, ChunkX, ChunkZ);
	if (!IsAllowedBlock(a_Chunk->GetBlock(RelX, RelY, RelZ)))
	{
		return;
	}

	Vector3i Block(a_BlockX, a_BlockY, a_BlockZ);
	
	//check for duplicates
	for (BlockList::iterator itr = m_Blocks->begin(); itr != m_Blocks->end(); ++itr)
	{
		Vector3i Pos = *itr;
		if ((Pos.x == a_BlockX) && (Pos.y == a_BlockY) && (Pos.z == a_BlockZ))
		{
			return;
		}
	}

	m_Blocks->push_back(Block);
}





bool cSandSimulator::IsPassable(BLOCKTYPE a_BlockType)
{
	return (a_BlockType == E_BLOCK_AIR)
		|| IsBlockWater(a_BlockType)
		|| IsBlockLava(a_BlockType)
		|| (a_BlockType == E_BLOCK_FIRE);
}
