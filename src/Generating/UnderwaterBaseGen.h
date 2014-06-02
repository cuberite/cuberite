
// UnderwaterBaseGen.h

// Declares the cUnderwaterBaseGen class representing the underwater base generator





#pragma once

#include "GridStructGen.h"
#include "PrefabPiecePool.h"





class cUnderwaterBaseGen :
	public cGridStructGen
{
	typedef cGridStructGen super;
	
public:
	cUnderwaterBaseGen(int a_Seed, int a_GridSize, int a_MaxDepth, int a_MaxSize, cBiomeGen & a_BiomeGen);

protected:
	class cUnderwaterBase;  // fwd: UnderwaterBaseGen.cpp


	/** The noise used for generating random numbers */
	cNoise m_Noise;
	
	/** Maximum depth of the generator tree*/
	int m_MaxDepth;
	
	/** Maximum size, in X/Z blocks, of the base (radius from the origin) */
	int m_MaxSize;
	
	/** The underlying biome generator that defines whether the base is created or not */
	cBiomeGen & m_BiomeGen;


	// cGridStructGen overrides:
	virtual cStructurePtr CreateStructure(int a_OriginX, int a_OriginZ) override;
} ;




