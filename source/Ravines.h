
// Ravines.h

// Interfaces to the cStructGenRavines class representing the ravine structure generator





#pragma once

#include "cChunkGenerator.h"
#include "cNoise.h"





class cStructGenRavines :
	public cStructureGen
{
public:
	cStructGenRavines(int a_Seed, int a_Size);
	~cStructGenRavines();
	
protected:
	class cRavine;  // fwd: Ravines.cpp
	typedef std::list<cRavine *> cRavines;
	
	cNoise   m_Noise;
	int      m_Size;  // Max size, in blocks, of the ravines generated
	cRavines m_Cache;
	
	/// Clears everything from the cache
	void ClearCache(void);
	
	/// Returns all ravines that *may* intersect the given chunk. All the ravines are valid until the next call to this function.
	void GetRavinesForChunk(int a_ChunkX, int a_ChunkZ, cRavines & a_Ravines);
	
	// cStructureGen override:
	virtual void GenStructures(
		int a_ChunkX, int a_ChunkZ,
		cChunkDef::BlockTypes & a_BlockTypes,   // Block types to read and change
		cChunkDef::BlockNibbles & a_BlockMeta,  // Block meta to read and change
		cChunkDef::HeightMap & a_HeightMap,     // Height map to read and change by the current data
		cEntityList & a_Entities,               // Entities may be added or deleted
		cBlockEntityList & a_BlockEntities      // Block entities may be added or deleted
	) override;
} ;




