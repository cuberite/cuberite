
// Ravines.h

// Interfaces to the cStructGenRavines class representing the ravine structure generator





#pragma once

#include "GridStructGen.h"





class cStructGenRavines :
	public cGridStructGen
{
	typedef cGridStructGen super;

public:
	cStructGenRavines(int a_Seed, int a_Size);

protected:
	class cRavine;  // fwd: Ravines.cpp

	cNoise m_Noise;
	int    m_Size;  // Max size, in blocks, of the ravines generated


	// cGridStructGen overrides:
	virtual cStructurePtr CreateStructure(int a_GridX, int a_GridZ, int a_OriginX, int a_OriginZ) override;
} ;




