
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "SandSimulator.h"
#include "World.h"
#include "Vector3i.h"
#include "BlockID.h"
#include "Defines.h"
#include "FallingBlock.h"




cSandSimulator::cSandSimulator( cWorld* a_World )
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





void cSandSimulator::Simulate( float a_Dt )
{
	m_Buffer->clear();
	std::swap( m_Blocks, m_Buffer );

	for( BlockList::iterator itr = m_Buffer->begin(); itr != m_Buffer->end(); ++itr )
	{
		Vector3i Pos = *itr;
		BLOCKTYPE BlockID = m_World->GetBlock(Pos.x, Pos.y, Pos.z);
		if(!IsAllowedBlock(BlockID))
			continue;

		BLOCKTYPE BottomBlock = m_World->GetBlock( Pos.x, Pos.y - 1, Pos.z );
		
		if( IsPassable(BottomBlock) )
		{
			cFallingBlock * FallingBlock = new cFallingBlock( Pos, BlockID );
			FallingBlock->Initialize( m_World );
 			m_World->SetBlock( Pos.x, Pos.y, Pos.z, E_BLOCK_AIR, 0 );
		}
	}
		
}





bool cSandSimulator::IsAllowedBlock( BLOCKTYPE a_BlockID )
{
	return a_BlockID == E_BLOCK_SAND
		|| a_BlockID == E_BLOCK_GRAVEL;
}





void cSandSimulator::AddBlock(int a_X, int a_Y, int a_Z)
{
	if(!IsAllowedBlock(m_World->GetBlock(a_X, a_Y, a_Z)))
		return;

	Vector3i Block(a_X, a_Y, a_Z);
	
	//check for duplicates
	for( BlockList::iterator itr = m_Blocks->begin(); itr != m_Blocks->end(); ++itr )
	{
		Vector3i Pos = *itr;
		if( Pos.x == a_X && Pos.y == a_Y && Pos.z == a_Z )
			return;
	}

	m_Blocks->push_back(Block);
}





bool cSandSimulator::IsPassable( BLOCKTYPE a_BlockID )
{
	return a_BlockID == E_BLOCK_AIR
		|| IsBlockWater(a_BlockID)
		|| IsBlockLava(a_BlockID)
		|| a_BlockID == E_BLOCK_FIRE;
}
