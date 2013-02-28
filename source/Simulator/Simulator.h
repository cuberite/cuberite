
#pragma once

#include "../Vector3i.h"





class cWorld;
class cChunk;





class cSimulator
{
public:
	cSimulator(cWorld * a_World);
	virtual ~cSimulator();

	/// Called in each tick, a_Dt is the time passed since the last tick, in msec
	virtual void Simulate(float a_Dt) = 0;
	
	/// Called when a block changes
	virtual void WakeUp(int a_BlockX, int a_BlockY, int a_BlockZ, cChunk * a_Chunk);

	virtual bool IsAllowedBlock(BLOCKTYPE a_BlockType) = 0;

protected:
	/// Called to simulate a new block
	virtual void AddBlock(int a_BlockX, int a_BlockY, int a_BlockZ, cChunk * a_Chunk) = 0;

	cWorld * m_World;
} ;




