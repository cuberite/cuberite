#pragma once

class Vector3i;
class cWorld;
class cSimulator
{
public:
	cSimulator( cWorld* a_World );
	~cSimulator();

	virtual void Simulate( float a_Dt ) = 0;
	virtual inline void WakeUp( int a_X, int a_Y, int a_Z );	//Used often so inline saves some calls

	virtual inline bool IsAllowedBlock( char a_BlockID ) = 0;

protected:
	virtual void AddBlock(int a_X, int a_Y, int a_Z) = 0;

	cWorld *m_World;
};