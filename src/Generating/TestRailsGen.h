
// TestRailsGen.h

// Declares the cTestRailsGen class representing the testing rails generator





#pragma once

#include "GridStructGen.h"
#include "PrefabPiecePool.h"





class cTestRailsGen :
	public cGridStructGen
{
	typedef cGridStructGen super;
	
public:
	cTestRailsGen(int a_Seed, int a_GridSize, int a_MaxOffset, int a_MaxDepth, int a_MaxSize);

protected:
	class cTestRails;  // fwd: TestRailsGen.cpp


	/** The noise used for generating random numbers */
	cNoise m_Noise;
	
	/** Maximum depth of the generator tree*/
	int m_MaxDepth;
	
	/** Maximum size, in X/Z blocks, of the base (radius from the origin) */
	int m_MaxSize;


	// cGridStructGen overrides:
	virtual cStructurePtr CreateStructure(int a_GridX, int a_GridZ, int a_OriginX, int a_OriginZ) override;
} ;




