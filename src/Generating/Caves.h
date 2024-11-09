
// Caves.h

// Interfaces to the various cave structure generators:
//   - cStructGenWormNestCaves
//   - cStructGenMarbleCaves
//   - cStructGenNetherCaves





#pragma once

#include "GridStructGen.h"





class cStructGenMarbleCaves :
	public cFinishGen
{
public:
	cStructGenMarbleCaves(int a_Seed) : m_Seed(a_Seed) {}

protected:

	int m_Seed;

	// cFinishGen override:
	virtual void GenFinish(cChunkDesc & a_ChunkDesc) override;
} ;





class cStructGenDualRidgeCaves :
	public cFinishGen
{
public:
	cStructGenDualRidgeCaves(int a_Seed, float a_Threshold) :
		m_Noise1(a_Seed),
		m_Noise2(2 * a_Seed + 19999),
		m_Seed(a_Seed),
		m_Threshold(a_Threshold)
	{
	}

protected:
	cNoise m_Noise1;
	cNoise m_Noise2;
	int    m_Seed;
	float  m_Threshold;

	// cFinishGen override:
	virtual void GenFinish(cChunkDesc & a_ChunkDesc) override;
} ;





class cStructGenWormNestCaves:
	public cGridStructGen
{
	using Super = cGridStructGen;

public:

	cStructGenWormNestCaves(int a_Seed, int a_Size = 64, int a_Grid = 96, int a_MaxOffset = 128) :
		Super(a_Seed, a_Grid, a_Grid, a_MaxOffset, a_MaxOffset, a_Size, a_Size, 100),
		m_Size(a_Size),
		m_MaxOffset(a_MaxOffset),
		m_Grid(a_Grid)
	{
	}

protected:

	class cCaveSystem;  // fwd: Caves.cpp

	int          m_Size;  // relative size of the cave systems' caves. Average number of blocks of each initial tunnel
	int          m_MaxOffset;  // maximum offset of the cave nest origin from the grid cell the nest belongs to
	int          m_Grid;  // average spacing of the nests

	// cGridStructGen override:
	virtual cStructurePtr CreateStructure(int a_GridX, int a_GridZ, int a_OriginX, int a_OriginZ) override;
} ;




