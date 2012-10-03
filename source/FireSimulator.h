
#pragma once

#include "Simulator.h"
#include "BlockEntity.h"





class Vector3i;
class cWorld;





class cFireSimulator : public cSimulator
{
public:
	cFireSimulator( cWorld* a_World );
	~cFireSimulator();

	virtual void Simulate( float a_Dt ) override;

	virtual bool IsAllowedBlock( BLOCKTYPE a_BlockType ) override;

	virtual bool IsBurnable( BLOCKTYPE a_BlockType );
	virtual bool IsForeverBurnable( BLOCKTYPE a_BlockType );
	virtual bool FiresForever( BLOCKTYPE a_BlockType );

protected:
	virtual void AddBlock(int a_X, int a_Y, int a_Z) override;
	virtual void _AddBlock(int a_X, int a_Y, int a_Z);
	virtual bool BurnBlockAround(int a_X, int a_Y, int a_Z);
	virtual bool BurnBlock(int a_X, int a_Y, int a_Z);

	typedef std::list <Vector3i> BlockList;
	BlockList *m_Blocks;
	BlockList *m_Buffer;

	BlockList *m_BurningBlocks;
};




