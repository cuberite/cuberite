
// FinishGen.h

/* Interfaces to the various finishing generators:
	- cFinishGenSnow
	- cFinishGenIce
	- cFinishGenSprinkleFoliage
	- cFinishGenLilypads
	- cFinishGenBottomLava
	- cFinishGenPreSimulator
	- cFinishGenDeadBushes
*/





#include "ComposableGenerator.h"
#include "../Noise.h"
#include "../ProbabDistrib.h"





class cFinishGenSnow :
	public cFinishGen
{
protected:
	// cFinishGen override:
	virtual void GenFinish(cChunkDesc & a_ChunkDesc) override;
} ;





class cFinishGenIce :
	public cFinishGen
{
protected:
	// cFinishGen override:
	virtual void GenFinish(cChunkDesc & a_ChunkDesc) override;
} ;





class cFinishGenNetherClumpFoliage :
	public cFinishGen
{
public:
	cFinishGenNetherClumpFoliage(int a_Seed) :
		m_Noise(a_Seed),
		m_Seed(a_Seed)
	{
	}

protected:
	cNoise m_Noise;
	int    m_Seed;

	void TryPlaceClump(cChunkDesc & a_ChunkDesc, int a_RelX, int a_RelY, int a_RelZ, BLOCKTYPE a_Block);
	virtual void GenFinish(cChunkDesc & a_ChunkDesc) override;
} ;





class cFinishGenTallGrass :
	public cFinishGen
{
public:
	cFinishGenTallGrass(int a_Seed) : m_Noise(a_Seed), m_Seed(a_Seed) {}

protected:
	cNoise m_Noise;
	int m_Seed;

	// cFinishGen override:
	virtual void GenFinish(cChunkDesc & a_ChunkDesc) override;

	int GetBiomeDensity(EMCSBiome a_Biome)
	{
		switch (a_Biome)
		{
			case biSavanna:
			case biSavannaM:
			case biSavannaPlateau:
			case biSavannaPlateauM:
			case biPlains:
			{
				return 70;
			}

			case biExtremeHillsEdge:
			case biExtremeHillsPlus:
			case biExtremeHills:
			case biExtremeHillsPlusM:
			case biExtremeHillsM:
			case biIceMountains:
			{
				return 3;
			}

			default:
			{
				return 20;
			}
		}
	}
};





class cFinishGenSprinkleFoliage :
	public cFinishGen
{
public:
	cFinishGenSprinkleFoliage(int a_Seed) : m_Noise(a_Seed), m_Seed(a_Seed) {}
	
protected:
	cNoise m_Noise;
	int    m_Seed;
	
	/// Tries to place sugarcane at the coords specified, returns true if successful
	bool TryAddSugarcane(cChunkDesc & a_ChunkDesc, int a_RelX, int a_RelY, int a_RelZ);
	
	// cFinishGen override:
	virtual void GenFinish(cChunkDesc & a_ChunkDesc) override;
} ;





/** This class adds a single top block in random positions in the specified biome on top of specified allowed blocks.
Used for:
- Lilypads finisher
- DeadBushes finisher
*/
class cFinishGenSingleTopBlock :
	public cFinishGen
{
public:
	typedef std::vector<BLOCKTYPE> BlockList;
	bool m_IsAllowedBelow[256];

	typedef std::vector<EMCSBiome> BiomeList;
	bool m_IsBiomeAllowed[256];


	cFinishGenSingleTopBlock(
		int a_Seed, BLOCKTYPE a_BlockType, BiomeList a_Biomes, int a_Amount,
		BlockList a_AllowedBelow
	) :
		m_Noise(a_Seed),
		m_BlockType(a_BlockType),
		m_Amount(a_Amount)
	{
		// Initialize all the block types.
		for (size_t idx = 0; idx < ARRAYCOUNT(m_IsAllowedBelow); ++idx)
		{
			m_IsAllowedBelow[idx] = false;
		}
		
		// Load the allowed blocks into m_IsAllowedBelow
		for (BlockList::iterator itr = a_AllowedBelow.begin(); itr != a_AllowedBelow.end(); ++itr)
		{
			m_IsAllowedBelow[*itr] = true;
		}
		
		// Initialize all the biome types.
		for (size_t idx = 0; idx < ARRAYCOUNT(m_IsBiomeAllowed); ++idx)
		{
			m_IsBiomeAllowed[idx] = false;
		}
		
		// Load the allowed biomes into m_IsBiomeAllowed
		for (BiomeList::iterator itr = a_Biomes.begin(); itr != a_Biomes.end(); ++itr)
		{
			m_IsBiomeAllowed[*itr] = true;
		}
	}
	
protected:
	cNoise m_Noise;
	BLOCKTYPE m_BlockType;
	int       m_Amount;         ///< Relative amount of blocks to try adding. 1 = one block per 256 biome columns.
	
	int GetNumToGen(const cChunkDef::BiomeMap & a_BiomeMap);

	// Returns true if the given biome is a biome that is allowed.
	inline bool IsAllowedBiome(EMCSBiome a_Biome)
	{
		return m_IsBiomeAllowed[a_Biome];
	}

	// Returns true if the given blocktype may be below m_BlockType
	inline bool IsAllowedBlockBelow(BLOCKTYPE a_BlockBelow)
	{
		return m_IsAllowedBelow[a_BlockBelow];
	}

	
	// cFinishGen override:
	virtual void GenFinish(cChunkDesc & a_ChunkDesc) override;
} ;





class cFinishGenBottomLava :
	public cFinishGen
{
public:
	cFinishGenBottomLava(int a_Level) :
		m_Level(a_Level)
	{
	}
	
	int GetLevel(void) const { return m_Level; }
protected:
	int m_Level;
	
	// cFinishGen override:
	virtual void GenFinish(cChunkDesc & a_ChunkDesc) override;
} ;





class cFinishGenPreSimulator :
	public cFinishGen
{
public:
	cFinishGenPreSimulator(bool a_PreSimulateFallingBlocks, bool a_PreSimulateWater, bool a_PreSimulateLava);
	
protected:

	bool m_PreSimulateFallingBlocks;
	bool m_PreSimulateWater;
	bool m_PreSimulateLava;

	// Drops hanging sand and gravel down to the ground, recalculates heightmap
	void CollapseSandGravel(
		cChunkDef::BlockTypes & a_BlockTypes,    // Block types to read and change
		cChunkDef::HeightMap & a_HeightMap       // Height map to update by the current data
	);
	
	/** For each fluid block:
	- if all surroundings are of the same fluid, makes it stationary; otherwise makes it flowing (excl. top)
	- all fluid on the chunk's edge is made flowing
	*/
	void StationarizeFluid(
		cChunkDef::BlockTypes & a_BlockTypes,    // Block types to read and change
		cChunkDef::HeightMap & a_HeightMap,      // Height map to read
		BLOCKTYPE a_Fluid,
		BLOCKTYPE a_StationaryFluid
	);

	// cFinishGen override:
	virtual void GenFinish(cChunkDesc & a_ChunkDesc) override;
} ;





class cFinishGenFluidSprings :
	public cFinishGen
{
public:
	cFinishGenFluidSprings(int a_Seed, BLOCKTYPE a_Fluid, cIniFile & a_IniFile, eDimension a_Dimension);
	
protected:

	cNoise         m_Noise;
	cProbabDistrib m_HeightDistribution;
	BLOCKTYPE      m_Fluid;
	int            m_Chance;  ///< Chance, [0..100], that a spring will be generated in a chunk

	// cFinishGen override:
	virtual void GenFinish(cChunkDesc & a_ChunkDesc) override;

	/// Tries to place a spring at the specified coords, checks neighbors. Returns true if successful
	bool TryPlaceSpring(cChunkDesc & a_ChunkDesc, int x, int y, int z);
} ;




