
// MineShafts.h

// Declares the cStructGenMineShafts class representing the structure generator for abandoned mineshafts





#pragma once

#include "ComposableGenerator.h"
#include "../Noise.h"





class cStructGenMineShafts :
	public cStructureGen
{
public:
	cStructGenMineShafts(
		int a_Seed, int a_GridSize, int a_MaxSystemSize,
		int a_ChanceCorridor, int a_ChanceCrossing, int a_ChanceStaircase
	);
	
	virtual ~cStructGenMineShafts();
	
protected:
	friend class cMineShaft;
	friend class cMineShaftDirtRoom;
	friend class cMineShaftCorridor;
	friend class cMineShaftCrossing;
	friend class cMineShaftStaircase;
	class cMineShaftSystem;  // fwd: MineShafts.cpp
	typedef std::list<cMineShaftSystem *> cMineShaftSystems;
	
	cNoise            m_Noise;
	int               m_GridSize;            ///< Average spacing of the systems
	int               m_MaxSystemSize;       ///< Maximum blcok size of a mineshaft system
	int               m_ProbLevelCorridor;   ///< Probability level of a branch object being the corridor
	int               m_ProbLevelCrossing;   ///< Probability level of a branch object being the crossing, minus Corridor
	int               m_ProbLevelStaircase;  ///< Probability level of a branch object being the staircase, minus Crossing
	cMineShaftSystems m_Cache;               ///< Cache of the most recently used systems. MoveToFront used.
	
	/// Clears everything from the cache
	void ClearCache(void);
	
	/** Returns all systems that *may* intersect the given chunk.
	All the systems are valid until the next call to this function (which may delete some of the pointers).
	*/
	void GetMineShaftSystemsForChunk(int a_ChunkX, int a_ChunkZ, cMineShaftSystems & a_MineShaftSystems);

	// cStructureGen overrides:
	virtual void GenStructures(cChunkDesc & a_ChunkDesc) override;
} ;




