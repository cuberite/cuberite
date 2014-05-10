
// MineShafts.h

// Declares the cStructGenMineShafts class representing the structure generator for abandoned mineshafts





#pragma once

#include "GridStructGen.h"
#include "../Noise.h"





class cStructGenMineShafts :
	public cGridStructGen
{
	typedef cGridStructGen super;

public:
	cStructGenMineShafts(
		int a_Seed, int a_GridSize, int a_MaxSystemSize,
		int a_ChanceCorridor, int a_ChanceCrossing, int a_ChanceStaircase
	);
	
protected:
	friend class cMineShaft;
	friend class cMineShaftDirtRoom;
	friend class cMineShaftCorridor;
	friend class cMineShaftCrossing;
	friend class cMineShaftStaircase;
	class cMineShaftSystem;  // fwd: MineShafts.cpp
	
	cNoise m_Noise;
	int    m_GridSize;            ///< Average spacing of the systems
	int    m_MaxSystemSize;       ///< Maximum blcok size of a mineshaft system
	int    m_ProbLevelCorridor;   ///< Probability level of a branch object being the corridor
	int    m_ProbLevelCrossing;   ///< Probability level of a branch object being the crossing, minus Corridor
	int    m_ProbLevelStaircase;  ///< Probability level of a branch object being the staircase, minus Crossing
	
	// cGridStructGen overrides:
	virtual cStructurePtr CreateStructure(int a_OriginX, int a_OriginZ) override;
} ;




