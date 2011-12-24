#pragma once


//TODO This definitly needs a better naming :D but how?
enum Direction
{
	X_PLUS,
	X_MINUS,
	Y_PLUS,
	Y_MINUS,
	Z_PLUS,
	Z_MINUS,
	NONE
};

class Vector3i;
class cWorld;
class cWaterSimulator
{
public:
	cWaterSimulator( cWorld* a_World );
	~cWaterSimulator();

	void Simulate( float a_Dt );
	void WakeUp( int a_X, int a_Y, int a_Z );

	//Gets the flowing direction of a Waterblock if a_Over is true also the block over the current block affects the direction (standard)
	Direction GetFlowingDirection(int a_X, int a_Y, int a_Z, bool a_Over = true);

private:
	void AddBlock( int a_X, int a_Y, int a_Z);
	char GetHighestLevelAround( int a_X, int a_Y, int a_Z );

	bool IsWaterBlock( char a_BlockID );

	float m_Timer;
	cWorld* m_World;

	class WaterData;
	WaterData* m_Data;

	
};