
#pragma once

#include "Simulator.h"
#include "../BlockEntity.h"
#include "../Vector3i.h"





class cSandSimulator : public cSimulator
{
public:
	cSandSimulator(cWorld & a_World);
	~cSandSimulator();

	virtual void Simulate( float a_Dt ) override;

	virtual bool IsAllowedBlock( BLOCKTYPE a_BlockType ) override;
	virtual bool IsPassable( BLOCKTYPE a_BlockType );

protected:
	virtual void AddBlock(int a_BlockX, int a_BlockY, int a_BlockZ, cChunk * a_Chunk) override;

	typedef std::list <Vector3i> BlockList;
	BlockList * m_Blocks;
	BlockList * m_Buffer;
};




