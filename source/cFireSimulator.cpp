#include "cFireSimulator.h"
#include "cWorld.h"
#include "Vector3i.h"
#include "BlockID.h"
#include "Defines.h"
#include <vector>

cFireSimulator::cFireSimulator( cWorld* a_World )
	: cSimulator(a_World)
	, m_Blocks(new std::vector <Vector3i *>)
	, m_Buffer(new std::vector <Vector3i *>)
	, m_BurningBlocks(new std::vector <Vector3i *>)
{

}

cFireSimulator::~cFireSimulator()
{
	delete m_Buffer;
	delete m_Blocks;
	delete m_BurningBlocks;
}

void cFireSimulator::Simulate( float a_Dt )
{
	m_Buffer->clear();
	std::swap( m_Blocks, m_Buffer );

	for( std::vector<Vector3i *>::iterator itr = m_Buffer->begin(); itr != m_Buffer->end(); ++itr )
	{
		Vector3i *Pos = *itr;

		if(!IsAllowedBlock(m_World->GetBlock(Pos->x, Pos->y, Pos->z)))	//Check wheather the block is still burning
			continue;

		if(BurnBlockAround(Pos->x, Pos->y, Pos->z))	//Burn single block and if there was one -> next time again
			_AddBlock(Pos->x, Pos->y, Pos->z);
		else
			if(!IsForeverBurnable(m_World->GetBlock(Pos->x, Pos->y - 1, Pos->z)))
				m_World->SetBlock(Pos->x, Pos->y, Pos->z, E_BLOCK_AIR, 0);

	}
		
}


bool cFireSimulator::IsAllowedBlock( char a_BlockID )
{
	return a_BlockID == E_BLOCK_FIRE
		|| IsBlockLava(a_BlockID);
}

void cFireSimulator::AddBlock(int a_X, int a_Y, int a_Z)
{
	char BlockID = m_World->GetBlock(a_X, a_Y, a_Z);
	if(!IsAllowedBlock(BlockID))		//This should save very much time because it doesn´t have to iterate through all blocks
		return;
	
	//check for duplicates
	for( std::vector<Vector3i *>::iterator itr = m_Blocks->begin(); itr != m_Blocks->end(); ++itr )
	{
		Vector3i *Pos = *itr;
		if( Pos->x == a_X && Pos->y == a_Y && Pos->z == a_Z )
			return;
	}

	_AddBlock(a_X, a_Y, a_Z);

}

void cFireSimulator::_AddBlock(int a_X, int a_Y, int a_Z)
{
	Vector3i *Block = new Vector3i(a_X, a_Y, a_Z);
	m_Blocks->push_back(Block);

}

bool cFireSimulator::IsForeverBurnable( char a_BlockID )
{
	return a_BlockID == E_BLOCK_BLOODSTONE;
}

bool cFireSimulator::IsBurnable( char a_BlockID )
{
	return a_BlockID == E_BLOCK_WOOD
		|| a_BlockID == E_BLOCK_LEAVES
		|| a_BlockID == E_BLOCK_LOG
		|| a_BlockID == E_BLOCK_WHITE_CLOTH
		|| a_BlockID == E_BLOCK_BOOKCASE
		|| a_BlockID == E_BLOCK_FENCE
		|| a_BlockID == E_BLOCK_TNT;
}

bool cFireSimulator::BurnBlockAround(int a_X, int a_Y, int a_Z)
{
	return BurnBlock(a_X + 1, a_Y, a_Z)
		|| BurnBlock(a_X - 1, a_Y, a_Z)
		|| BurnBlock(a_X, a_Y + 1, a_Z)
		|| BurnBlock(a_X, a_Y - 1, a_Z)
		|| BurnBlock(a_X, a_Y, a_Z + 1)
		|| BurnBlock(a_X, a_Y, a_Z - 1);
}

bool cFireSimulator::BurnBlock(int a_X, int a_Y, int a_Z)
{
	char BlockID = m_World->GetBlock(a_X, a_Y, a_Z);
	if(IsBurnable(BlockID))
	{
		m_World->SetBlock(a_X, a_Y, a_Z, E_BLOCK_FIRE, 0);
		return true;
	}
	if(IsForeverBurnable(BlockID))
	{
		char BlockAbove = m_World->GetBlock(a_X, a_Y + 1, a_Z);
		if(BlockAbove == E_BLOCK_AIR)
		{
			m_World->SetBlock(a_X, a_Y + 1, a_Z, E_BLOCK_FIRE, 0);	//Doesn´t notify the simulator so it won´t go off
			return true;
		}
		return false;
	}

	return false;
}