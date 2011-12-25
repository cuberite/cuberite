#pragma once
#include "cFluidSimulator.h"

class cWaterSimulator : public cFluidSimulator
{
public:
	cWaterSimulator( cWorld* a_World );

	virtual inline bool IsAllowedBlock( char a_BlockID );
	virtual inline bool IsPassableForFluid( char a_BlockID );

};