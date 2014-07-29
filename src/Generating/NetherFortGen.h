
// NetherFortGen.h

// Declares the cNetherFortGen class representing the nether fortress generator





#pragma once

#include "ComposableGenerator.h"
#include "PrefabPiecePool.h"
#include "GridStructGen.h"





class cNetherFortGen :
	public cGridStructGen
{
	typedef cGridStructGen super;
	
public:
	cNetherFortGen(int a_Seed, int a_GridSize, int a_MaxOffset, int a_MaxDepth);
	
protected:
	friend class cNetherFortPerfTest;  // fwd: NetherFortGen.cpp
	class cNetherFort;  // fwd: NetherFortGen.cpp
	
	/** Maximum depth of the piece-generator tree */
	int m_MaxDepth;
	
	/** The pool of pieces to use for generating. Static, so that it's shared by multiple generators. */
	static cPrefabPiecePool m_PiecePool;
	

	// cGridStructGen overrides:
	virtual cStructurePtr CreateStructure(int a_GridX, int a_GridZ, int a_OriginX, int a_OriginZ) override;
} ;




