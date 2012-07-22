
// Caves.cpp

// Implements the various cave structure generators:
//   - cStructGenWormNestCaves
//   - cStructGenMarbleCaves
//   - cStructGenNetherCaves

#include "Globals.h"
#include "Caves.h"





struct cDefPoint
{
	int m_BlockX;
	int m_BlockZ;
	int m_Radius;
	
	cDefPoint(int a_BlockX, int a_BlockZ, int a_Radius) :
		m_BlockX(a_BlockX),
		m_BlockZ(a_BlockZ),
		m_Radius(a_Radius)
	{
	}
} ;

typedef std::vector<cDefPoint> cDefPoints;





/// A single non-branching tunnel
class cCaveTunnel
{
	cDefPoints m_Points;
	
	/// Generates the shaping defpoints for the ravine, based on the ravine block coords and noise
	void GenerateBaseDefPoints(int a_BlockX, int a_BlockZ, int a_Size, cNoise & a_Noise);
	
	/// Refines (adds and smooths) defpoints from a_Src into a_Dst
	void RefineDefPoints(const cDefPoints & a_Src, cDefPoints & a_Dst);
	
	/// Does one round of smoothing, two passes of RefineDefPoints()
	void Smooth(void);
	
	/// Linearly interpolates the points so that the maximum distance between two neighbors is max 1 block
	void FinishLinear(void);
	
public:
	// Coords for which the ravine was generated (not necessarily the center)
	int m_BlockX;
	int m_BlockZ;
	
	cCaveTunnel(int a_BlockStartX, int a_BlockStartZ, int a_Size, cNoise & a_Noise);
	
	/// Carves the tunnel into the chunk specified
	void ProcessChunk(
		int a_ChunkX, int a_ChunkZ, 
		cChunkDef::BlockTypes & a_BlockTypes, 
		cChunkDef::HeightMap & a_HeightMap
	);
} ;

typedef std::vector<cCaveTunnel *> cCaveTunnels;





/// A collection of connected tunnels, possibly branching.
class cStructGenWormNestCaves::cCaveSystem
{
public:
	cCaveSystem(int a_BlockX, int a_BlockZ, int a_Size, cNoise & a_Noise);
	~cCaveSystem();
	
	/// Carves the cave system into the chunk specified
	void ProcessChunk(
		int a_ChunkX, int a_ChunkZ, 
		cChunkDef::BlockTypes & a_BlockTypes, 
		cChunkDef::HeightMap & a_HeightMap
	);
	
protected:
	int m_BlockX;
	int m_BlockZ;
	cCaveTunnels m_Tunnels;
	
	void Clear(void);
} ;





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cCaveTunnel:

void cCaveTunnel::ProcessChunk(
	int a_ChunkX, int a_ChunkZ, 
	cChunkDef::BlockTypes & a_BlockTypes, 
	cChunkDef::HeightMap & a_HeightMap
)
{
	// TODO
}





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cStructGenWormNestCaves::cCaveSystem:

cStructGenWormNestCaves::cCaveSystem::cCaveSystem(int a_BlockX, int a_BlockZ, int a_Size, cNoise & a_Noise) :
	m_BlockX(a_BlockX),
	m_BlockZ(a_BlockZ)
{
	// TODO: Generate a cave system
}





cStructGenWormNestCaves::cCaveSystem::~cCaveSystem()
{
	Clear();
}






void cStructGenWormNestCaves::cCaveSystem::Clear(void)
{
	// TODO
}





void cStructGenWormNestCaves::cCaveSystem::ProcessChunk(
	int a_ChunkX, int a_ChunkZ, 
	cChunkDef::BlockTypes & a_BlockTypes, 
	cChunkDef::HeightMap & a_HeightMap
)
{
	for (cCaveTunnels::const_iterator itr = m_Tunnels.begin(), end = m_Tunnels.end(); itr != end; ++itr)
	{
		(*itr)->ProcessChunk(a_ChunkX, a_ChunkZ, a_BlockTypes, a_HeightMap);
	}  // for itr - m_Tunnels[]
}





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cStructGenWormNestCaves:

cStructGenWormNestCaves::~cStructGenWormNestCaves()
{
	ClearCache();
}





void cStructGenWormNestCaves::ClearCache(void)
{
	for (cCaves::const_iterator itr = m_Cache.begin(), end = m_Cache.end(); itr != end; ++itr)
	{
		delete *itr;
	}  // for itr - m_Cache[]
	m_Cache.clear();
}





void cStructGenWormNestCaves::GenStructures(
	int a_ChunkX, int a_ChunkZ,
	cChunkDef::BlockTypes & a_BlockTypes,   // Block types to read and change
	cChunkDef::BlockNibbles & a_BlockMeta,  // Block meta to read and change
	cChunkDef::HeightMap & a_HeightMap,     // Height map to read and change by the current data
	cEntityList & a_Entities,               // Entities may be added or deleted
	cBlockEntityList & a_BlockEntities      // Block entities may be added or deleted
)
{
	cCaves Caves;
	GetCavesForChunk(a_ChunkX, a_ChunkZ, Caves);
	for (cCaves::const_iterator itr = Caves.begin(); itr != Caves.end(); ++itr)
	{
		(*itr)->ProcessChunk(a_ChunkX, a_ChunkZ, a_BlockTypes, a_HeightMap);
	}  // for itr - Caves[]
}





void cStructGenWormNestCaves::GetCavesForChunk(int a_ChunkX, int a_ChunkZ, cStructGenWormNestCaves::cCaves & a_Caves)
{
	// TODO: Implement proper caching
	//  - don't destroy caves that are reusable
	//  - don't create caves that are already in the cache
	
	ClearCache();
	
	int BaseX = a_ChunkX * cChunkDef::Width / m_Size;
	int BaseZ = a_ChunkZ * cChunkDef::Width / m_Size;
	if (BaseX < 0)
	{
		--BaseX;
	}
	if (BaseZ < 0)
	{
		--BaseZ;
	}
	BaseX -= 4;
	BaseZ -= 4;
	for (int x = 0; x < 8; x++)
	{
		int RealX = (BaseX + x) * m_Size;
		for (int z = 0; z < 8; z++)
		{
			int RealZ = (BaseZ + z) * m_Size;
			m_Cache.push_back(new cCaveSystem(RealX, RealZ, m_Size, m_Noise));
		}
	}
	a_Caves = m_Cache;
}






///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cStructGenMarbleCaves:

static float GetMarbleNoise( float x, float y, float z, cNoise & a_Noise )
{
	static const float PI_2 = 1.57079633f;
	float oct1 = (a_Noise.CubicNoise3D(x * 0.1f, y * 0.1f, z * 0.1f )) * 4;

	oct1 = oct1 * oct1 * oct1;
	if (oct1 < 0.f)  oct1 = PI_2;
	if (oct1 > PI_2) oct1 = PI_2;

	return oct1;
}





void cStructGenMarbleCaves::GenStructures(
	int a_ChunkX, int a_ChunkZ, 
	cChunkDef::BlockTypes & a_BlockTypes,   // Block types to read and change
	cChunkDef::BlockNibbles & a_BlockMeta,  // Block meta to read and change
	cChunkDef::HeightMap & a_HeightMap,     // Height map to read and change by the current data
	cEntityList & a_Entities,               // Entities may be added or deleted
	cBlockEntityList & a_BlockEntities      // Block entities may be added or deleted
)
{
	cNoise Noise(m_Seed);
	for (int z = 0; z < cChunkDef::Width; z++) 
	{
		const float zz = (float)(a_ChunkZ * cChunkDef::Width + z);
		for (int x = 0; x < cChunkDef::Width; x++)
		{
			const float xx = (float)(a_ChunkX * cChunkDef::Width + x);
			
			int Top = cChunkDef::GetHeight(a_HeightMap, x, z);
			for (int y = 1; y < Top; ++y )
			{
				if (cChunkDef::GetBlock(a_BlockTypes, x, y, z) != E_BLOCK_STONE)
				{
					continue;
				}

				const float yy = (float)y;
				const float WaveNoise = 1;
				if (cosf(GetMarbleNoise(xx, yy * 0.5f, zz, Noise)) * fabs(cosf(yy * 0.2f + WaveNoise * 2) * 0.75f + WaveNoise) > 0.0005f)
				{
					if (y > 4)
					{
						cChunkDef::SetBlock(a_BlockTypes, x, y, z, E_BLOCK_AIR);
					}
					else
					{
						cChunkDef::SetBlock(a_BlockTypes, x, y, z, E_BLOCK_STATIONARY_LAVA);
					}
				}
			}  // for y
		}  // for x
	}  // for z
}





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cStructGenDualRidgeCaves:

void cStructGenDualRidgeCaves::GenStructures(
	int a_ChunkX, int a_ChunkZ, 
	cChunkDef::BlockTypes & a_BlockTypes,   // Block types to read and change
	cChunkDef::BlockNibbles & a_BlockMeta,  // Block meta to read and change
	cChunkDef::HeightMap & a_HeightMap,     // Height map to read and change by the current data
	cEntityList & a_Entities,               // Entities may be added or deleted
	cBlockEntityList & a_BlockEntities      // Block entities may be added or deleted
)
{
	cNoise Noise1(m_Seed);
	cNoise Noise2(2 * m_Seed + 19999);
	for (int z = 0; z < cChunkDef::Width; z++) 
	{
		const float zz = (float)(a_ChunkZ * cChunkDef::Width + z) / 10;
		for (int x = 0; x < cChunkDef::Width; x++)
		{
			const float xx = (float)(a_ChunkX * cChunkDef::Width + x) / 10;
			
			int Top = cChunkDef::GetHeight(a_HeightMap, x, z);
			for (int y = 1; y <= Top; ++y)
			{
				/*
				if (cChunkDef::GetBlock(a_BlockTypes, x, y, z) != E_BLOCK_STONE)
				{
					continue;
				}
				*/

				const float yy = (float)y / 10;
				const float WaveNoise = 1;
				float n1 = Noise1.CubicNoise3D(xx, yy, zz);
				float n2 = Noise2.CubicNoise3D(xx, yy, zz);
				float n3 = Noise1.CubicNoise3D(xx * 4, yy * 4, zz * 4) / 4;
				float n4 = Noise2.CubicNoise3D(xx * 4, yy * 4, zz * 4) / 4;
				if ((abs(n1 + n3) * abs(n2 + n4)) > m_Threshold)
				{
					if (y > 10)
					{
						cChunkDef::SetBlock(a_BlockTypes, x, y, z, E_BLOCK_AIR);
					}
					else
					{
						cChunkDef::SetBlock(a_BlockTypes, x, y, z, E_BLOCK_STATIONARY_LAVA);
					}
				}
			}  // for y
		}  // for x
	}  // for z
}




