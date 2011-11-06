#pragma once

class Vector3i;
class cWorld;
class cLavaSimulator
{
public:
	cLavaSimulator( cWorld* a_World );
	~cLavaSimulator();

	void Simulate( float a_Dt );
	void WakeUp( int a_X, int a_Y, int a_Z );

private:
	void AddBlock( int a_X, int a_Y, int a_Z);
	char GetHighestLevelAround( int a_X, int a_Y, int a_Z );

	bool IsLavaBlock( char a_BlockID );

	float m_Timer;
	cWorld* m_World;

	class LavaData;
	LavaData* m_Data;
};
