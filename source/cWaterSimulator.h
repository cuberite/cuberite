#pragma once

class Vector3i;
class cWorld;
class cWaterSimulator
{
public:
	cWaterSimulator( cWorld* a_World );
	~cWaterSimulator();

	void Simulate( float a_Dt );
	void WakeUp( int a_X, int a_Y, int a_Z );

private:
	void AddBlock( int a_X, int a_Y, int a_Z);
	char GetHighestLevelAround( int a_X, int a_Y, int a_Z );

	bool IsWaterBlock( char a_BlockID );

	float m_Timer;
	cWorld* m_World;

	class WaterData;
	WaterData* m_Data;
};