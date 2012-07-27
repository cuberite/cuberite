
// Caves.h

// Interfaces to the various cave structure generators:
//   - cStructGenWormNestCaves
//   - cStructGenMarbleCaves
//   - cStructGenNetherCaves





#pragma once

#include "cChunkGenerator.h"
#include "cNoise.h"





class cStructGenMarbleCaves :
	public cStructureGen
{
public:
	cStructGenMarbleCaves(int a_Seed) : m_Seed(a_Seed) {}
	
protected:

	int m_Seed;
	
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





class cStructGenDualRidgeCaves :
	public cStructureGen
{
public:
	cStructGenDualRidgeCaves(int a_Seed, float a_Threshold) :
		m_Seed(a_Seed),
		m_Threshold(a_Threshold)
	{
	}
	
protected:

	int m_Seed;
	float m_Threshold;
	
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





class cStructGenWormNestCaves :
	public cStructureGen
{
public:
	cStructGenWormNestCaves(int a_Seed, int a_Size = 64, int a_Grid = 96, int a_MaxOffset = 128) :
		m_Noise(a_Seed),
		m_Size(a_Size),
		m_Grid(a_Grid),
		m_MaxOffset(a_MaxOffset)
	{
	}
	
	~cStructGenWormNestCaves();
	
protected:
	class cCaveSystem;  // fwd: Caves.cpp
	typedef std::list<cCaveSystem *> cCaveSystems;

	cNoise       m_Noise;
	int          m_Size;  // relative size of the cave systems' caves. Average number of blocks of each initial tunnel
	int          m_MaxOffset;  // maximum offset of the cave nest origin from the grid cell the nest belongs to
	int          m_Grid;  // average spacing of the nests
	cCaveSystems m_Cache;
	
	/// Clears everything from the cache
	void ClearCache(void);
	
	/// Returns all caves that *may* intersect the given chunk. All the caves are valid until the next call to this function.
	void GetCavesForChunk(int a_ChunkX, int a_ChunkZ, cCaveSystems & a_Caves);
	
	// cStructGen override:
	virtual void GenStructures(
		int a_ChunkX, int a_ChunkZ,
		cChunkDef::BlockTypes & a_BlockTypes,   // Block types to read and change
		cChunkDef::BlockNibbles & a_BlockMeta,  // Block meta to read and change
		cChunkDef::HeightMap & a_HeightMap,     // Height map to read and change by the current data
		cEntityList & a_Entities,               // Entities may be added or deleted
		cBlockEntityList & a_BlockEntities      // Block entities may be added or deleted
	) override;
} ;




