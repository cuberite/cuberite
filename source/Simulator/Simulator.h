
#pragma once

#include "../Vector3i.h"





class cWorld;





class cSimulator
{
public:
	cSimulator(cWorld * a_World);
	virtual ~cSimulator();

	/// Called in each tick, a_Dt is the time passed since the last tick
	virtual void Simulate(float a_Dt) = 0;
	
	/// Called when a block changes via cWorld::SetBlock()
	virtual void WakeUp(int a_BlockX, int a_BlockY, int a_BlockZ);

	virtual bool IsAllowedBlock(BLOCKTYPE a_BlockType) = 0;

protected:
	virtual void AddBlock(int a_BlockX, int a_BlockY, int a_BlockZ) = 0;

	cWorld * m_World;
} ;




