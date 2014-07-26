
// RoughRavines.h

// Declares the cRoughRavines class representing the rough ravine generator




#pragma once

#include "GridStructGen.h"





class cRoughRavines :
	public cGridStructGen
{
	typedef cGridStructGen super;
	
public:
	cRoughRavines(int a_Seed, int a_MaxSize, int a_GridSize, int a_MaxOffset);
	
protected:
	int m_Seed;
	
	/** Maximum size of the ravine, in each of the X / Z axis */
	int m_MaxSize;

	// cGridStructGen overrides:
	virtual cStructurePtr CreateStructure(int a_GridX, int a_GridZ, int a_OriginX, int a_OriginZ) override;
};




