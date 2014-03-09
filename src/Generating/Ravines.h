
// Ravines.h

// Interfaces to the cStructGenRavines class representing the ravine structure generator





#pragma once

#include "ComposableGenerator.h"
#include "../Noise.h"





class cStructGenRavines :
	public cFinishGen
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
	
	// cFinishGen override:
	virtual void GenFinish(cChunkDesc & a_ChunkDesc) override;
} ;




