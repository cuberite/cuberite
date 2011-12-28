#pragma once
#include "cSimulator.h"
#include "cBlockEntity.h"
#include "vector"

class Vector3i;
class cWorld;
class cSandSimulator : public cSimulator
{
public:
	cSandSimulator( cWorld* a_World );
	~cSandSimulator();

	virtual void Simulate( float a_Dt );
	virtual inline void WakeUp( int a_X, int a_Y, int a_Z );

	virtual inline bool IsAllowedBlock( char a_BlockID );
	virtual inline bool IsPassable( char a_BlockID );

protected:
	virtual void AddBlock(int a_X, int a_Y, int a_Z);

	std::vector <Vector3i *> *m_Blocks;
	std::vector <Vector3i *> *m_Buffer;
};