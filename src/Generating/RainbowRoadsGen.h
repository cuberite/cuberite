
// RainbowRoadsGen.h

// Declares the cRainbowRoadsGen class representing the underwater base generator





#pragma once

#include "GridStructGen.h"
#include "PrefabPiecePool.h"





class cRainbowRoadsGen :
	public cGridStructGen
{
	typedef cGridStructGen super;
	
public:
	cRainbowRoadsGen(int a_Seed, int a_GridSize, int a_MaxOffset, int a_MaxDepth, int a_MaxSize);

protected:
	class cRainbowRoads;  // fwd: RainbowRoadsGen.cpp


	/** The noise used for generating random numbers */
	cNoise m_Noise;
	
	/** Maximum depth of the generator tree */
	int m_MaxDepth;
	
	/** Maximum size, in X / Z blocks, of the structure (radius from the origin) */
	int m_MaxSize;


	// cGridStructGen overrides:
	virtual cStructurePtr CreateStructure(int a_GridX, int a_GridZ, int a_OriginX, int a_OriginZ) override;
} ;




