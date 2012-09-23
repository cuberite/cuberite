
#pragma once

#include "Simulator.h"
#include "BlockEntity.h"
#include "Vector3i.h"





class cWorld;





class cSandSimulator : public cSimulator
{
public:
	cSandSimulator( cWorld* a_World );
	~cSandSimulator();

	virtual void Simulate( float a_Dt );
	virtual void WakeUp( int a_X, int a_Y, int a_Z );

	virtual bool IsAllowedBlock( char a_BlockID );
	virtual bool IsPassable( char a_BlockID );

protected:
	virtual void AddBlock(int a_X, int a_Y, int a_Z);

	typedef std::list <Vector3i> BlockList;
	BlockList * m_Blocks;
	BlockList * m_Buffer;
};




