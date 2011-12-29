#pragma once
#include "cFluidSimulator.h"

class cLavaSimulator : public cFluidSimulator
{
public:
	cLavaSimulator( cWorld* a_World );

	virtual inline bool IsAllowedBlock( char a_BlockID );

};