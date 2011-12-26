#pragma once

#include "cSimulator.h"


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
class cFluidSimulator : public cSimulator
{
public:
	cFluidSimulator( cWorld* a_World );
	~cFluidSimulator();

	virtual void Simulate( float a_Dt );

	//Gets the flowing direction. if a_Over is true also the block over the current block affects the direction (standard)
	Direction GetFlowingDirection(int a_X, int a_Y, int a_Z, bool a_Over = true);

	virtual inline bool IsAllowedBlock( char a_BlockID ) = 0;
	virtual inline bool IsPassableForFluid( char a_BlockID ) = 0;

protected:
	virtual void AddBlock( int a_X, int a_Y, int a_Z);
	char GetHighestLevelAround( int a_X, int a_Y, int a_Z );


	float m_Timer;

	class FluidData;
	FluidData* m_Data;

	//Customize
	char m_FluidBlock;
	char m_MaxHeight;
	char m_FlowReduction;

};