#pragma once

class Vector3i;
class cWorld;
class cSimulator
{
public:
	cSimulator( cWorld* a_World );
	~cSimulator();

	virtual void Simulate( float a_Dt ) = 0;
	virtual void WakeUp( int a_X, int a_Y, int a_Z );

	virtual bool IsAllowedBlock( char a_BlockID ) = 0;

protected:
	virtual void AddBlock(int a_X, int a_Y, int a_Z) = 0;

	cWorld * m_World;
};