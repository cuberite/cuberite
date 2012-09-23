#pragma once
#include "FluidSimulator.h"

class cWaterSimulator : public cFluidSimulator
{
public:
	cWaterSimulator( cWorld* a_World );

	virtual bool IsAllowedBlock( char a_BlockID );

};