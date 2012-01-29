
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "cSandSimulator.h"
#include "cWorld.h"
#include "Vector3i.h"
#include "BlockID.h"
#include "Defines.h"





cSandSimulator::cSandSimulator( cWorld* a_World )
	: cSimulator(a_World)
	, m_Blocks(new std::list <Vector3i *>)
	, m_Buffer(new std::list <Vector3i *>)
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

	for( std::list<Vector3i *>::iterator itr = m_Buffer->begin(); itr != m_Buffer->end(); ++itr )
	{
		Vector3i *Pos = *itr;
		char BlockID = m_World->GetBlock(Pos->x, Pos->y, Pos->z);
		if(!IsAllowedBlock(BlockID))
			continue;

		char BottomBlock = m_World->GetBlock( Pos->x, Pos->y - 1, Pos->z );
		
		if( IsPassable(BottomBlock) )
		{
			m_World->SetBlock( Pos->x, Pos->y, Pos->z, E_BLOCK_AIR, 0 );
			m_World->SetBlock( Pos->x, Pos->y - 1, Pos->z, BlockID, 0 );
		}
	}
		
}


bool cSandSimulator::IsAllowedBlock( char a_BlockID )
{
	return a_BlockID == E_BLOCK_SAND
		|| a_BlockID == E_BLOCK_GRAVEL;
}

void cSandSimulator::AddBlock(int a_X, int a_Y, int a_Z)
{
	if(!IsAllowedBlock(m_World->GetBlock(a_X, a_Y, a_Z)))		//This should save very much time because it doesn´t have to iterate through all blocks
		return;

	Vector3i *Block = new Vector3i(a_X, a_Y, a_Z);
	
	//check for duplicates
	for( std::list<Vector3i *>::iterator itr = m_Blocks->begin(); itr != m_Blocks->end(); ++itr )
	{
		Vector3i *Pos = *itr;
		if( Pos->x == a_X && Pos->y == a_Y && Pos->z == a_Z )
			return;
	}

	m_Blocks->push_back(Block);

}

bool cSandSimulator::IsPassable( char a_BlockID )
{
	return a_BlockID == E_BLOCK_AIR
		|| IsBlockWater(a_BlockID)
		|| IsBlockLava(a_BlockID)
		|| a_BlockID == E_BLOCK_FIRE;

}

void cSandSimulator::WakeUp( int a_X, int a_Y, int a_Z )
{
	//Nothing else needs to be simulated :D (Bugs not included :s)
	AddBlock( a_X, a_Y+1, a_Z );
	AddBlock( a_X, a_Y, a_Z );
}
