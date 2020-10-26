
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





#pragma once

#include "ComposableGenerator.h"
#include "../Noise/Noise.h"
#include "../ProbabDistrib.h"
#include "../Mobs/Monster.h"





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





class cFinishGenClumpTopBlock :
	public cFinishGen
{
public:
	// Contains the meta, type and weight for a clump block
	struct FoliageInfo
	{
		BLOCKTYPE m_BlockType;
		NIBBLETYPE m_BlockMeta;
		int m_Weight;

		FoliageInfo(BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, int a_Weight) :
			m_BlockType(a_BlockType),
			m_BlockMeta(a_BlockMeta),
			m_Weight(a_Weight)
		{}
	};

	// Contains the minimum and maximum amount of clumps for a biome and it's blocks.
	struct BiomeInfo
	{
		int m_MinNumClumpsPerChunk;
		int m_MaxNumClumpsPerChunk;
		std::vector<FoliageInfo> m_Blocks;

		BiomeInfo() :
			m_MinNumClumpsPerChunk(0),
			m_MaxNumClumpsPerChunk(2),
			m_Blocks()
		{}

		BiomeInfo(int a_MinNumClumpsPerChunk, int a_MaxNumClumpsPerChunk, std::vector<FoliageInfo> a_Blocks) :
			m_MinNumClumpsPerChunk(a_MinNumClumpsPerChunk),
			m_MaxNumClumpsPerChunk(a_MaxNumClumpsPerChunk),
			m_Blocks(std::move(a_Blocks))
		{}
	};


	cFinishGenClumpTopBlock(int a_Seed, std::vector<BiomeInfo> a_BlockList) :
		m_Noise(a_Seed),
		m_FlowersPerBiome()
	{
		std::swap(a_BlockList, m_FlowersPerBiome);
	}

	/** Parses a string and puts a vector with a length of biMaxVariantBiome in a_Output.
	The format of the string is "<Biomes separated with a comma>;<Blocks separated with a comma>". This can also be repeated with a | */
	static void ParseConfigurationString(const AString & a_String, std::vector<BiomeInfo> & a_Output);

	/** Parses an inifile in search for all clumps */
	static std::vector<BiomeInfo> ParseIniFile(cIniFile & a_IniFile, AString a_ClumpPrefix);
protected:

	cNoise m_Noise;
	std::vector<BiomeInfo> m_FlowersPerBiome;

	/** The maximum number of foliage per clump */
	const int MAX_NUM_FOLIAGE = 8;

	/** The mininum number of foliage per clump */
	const int MIN_NUM_FOLIAGE = 4;

	/** The maximum range a foliage can be placed from the center of the clump */
	const int RANGE_FROM_CENTER = 5;

	void TryPlaceFoliageClump(cChunkDesc & a_ChunkDesc, int a_RelX, int a_RelZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, bool a_IsDoubleTall);
	virtual void GenFinish(cChunkDesc & a_ChunkDesc) override;
};





class cFinishGenGlowStone :
	public cFinishGen
{
public:
	cFinishGenGlowStone(int a_Seed) :
		m_Noise(a_Seed),
		m_Seed(a_Seed)
	{
	}

protected:
	cNoise m_Noise;
	int    m_Seed;

	void TryPlaceGlowstone(cChunkDesc & a_ChunkDesc, int a_RelX, int a_RelY, int a_RelZ, int a_Size, int a_NumStrings);
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





class cFinishGenVines :
	public cFinishGen
{
public:
	cFinishGenVines(int a_Seed, int a_Level) :
		m_Noise(a_Seed),
		m_Level(a_Level)
	{
	}

	bool IsJungleVariant(EMCSBiome a_Biome);

protected:
	cNoise m_Noise;
	int    m_Level;

	virtual void GenFinish(cChunkDesc & a_ChunkDesc) override;
};





class cFinishGenSoulsandRims :
	public cFinishGen
{
public:
	cFinishGenSoulsandRims(int a_Seed) :
		m_Noise(a_Seed)
	{
	}

protected:
	cNoise m_Noise;

	virtual void GenFinish(cChunkDesc & a_ChunkDesc) override;
} ;





class cFinishGenSprinkleFoliage :
	public cFinishGen
{
public:
	cFinishGenSprinkleFoliage(int a_Seed, int a_MaxCactusHeight, int a_MaxSugarcaneHeight):
		m_Noise(a_Seed),
		m_Seed(a_Seed),
		m_MaxCactusHeight(a_MaxCactusHeight),
		m_MaxSugarcaneHeight(a_MaxSugarcaneHeight)
	{
	}
protected:
	cNoise m_Noise;
	int    m_Seed;
	int    m_MaxCactusHeight;
	int    m_MaxSugarcaneHeight;

	/** Tries to place sugarcane at the coords specified, returns true if successful, updates the top variable (hence the & a_RefY) */
	bool TryAddSugarcane(cChunkDesc & a_ChunkDesc, int a_RelX, HEIGHTTYPE & a_RelY, int a_RelZ);

	/** Tries to place cactus at the coords specified, returns true if successful, updates the top variable (hence the & a_RefY) */
	bool TryAddCactus(cChunkDesc & a_ChunkDesc, int a_RelX, HEIGHTTYPE & a_RelY, int a_RelZ);

	// Returns true is the specified biome is a desert or its variant
	static bool IsDesertVariant(EMCSBiome a_biome);

	// cFinishGen override:
	virtual void GenFinish(cChunkDesc & a_ChunkDesc) override;
} ;





/** This class adds a single top block in random positions in the specified biome on top of specified allowed blocks.
Used for:
- Lilypads finisher
- DeadBushes finisher */
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

	/** Relative amount of blocks to try adding. 1 = one block per 256 biome columns. */
	int m_Amount;


	int GetNumToGen(const cChunkDef::BiomeMap & a_BiomeMap);

	/** Returns true if the given biome is a biome that is allowed. */
	inline bool IsAllowedBiome(EMCSBiome a_Biome)
	{
		return m_IsBiomeAllowed[a_Biome];
	}

	/** Returns true if the given blocktype may be below m_BlockType */
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

	/** Drops hanging sand and gravel down to the ground, recalculates heightmap */
	void CollapseSandGravel(cChunkDesc & a_ChunkDesc);

	/** For each fluid block:
	- if all surroundings are of the same fluid, makes it stationary; otherwise makes it flowing (excl. top)
	- all fluid on the chunk's edge is made flowing */
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

	/** Tries to place a spring at the specified coords, checks neighbors. Returns true if successful. */
	bool TryPlaceSpring(cChunkDesc & a_ChunkDesc, int x, int y, int z);
} ;





/** This class populates generated chunks with packs of biome-dependant animals
Animals: cows, sheep, pigs, mooshrooms, squid, horses, wolves, ocelots */
class cFinishGenPassiveMobs :
	public cFinishGen
{
public:

	cFinishGenPassiveMobs(int a_Seed, cIniFile & a_IniFile, eDimension a_Dimension);

protected:

	/** The noise used as the source of randomness */
	cNoise m_Noise;

	/** Chance, [0..100], that an animal pack will be generated in a chunk */
	int m_AnimalProbability;


	// cFinishGen override:
	virtual void GenFinish(cChunkDesc & a_ChunkDesc) override;

	/** Returns false if an animal cannot spawn at given coords, else adds it to the chunk's entity list and returns true */
	bool TrySpawnAnimals(cChunkDesc & a_ChunkDesc, int x, int y, int z, eMonsterType AnimalToSpawn);

	/** Picks a random animal from biome-dependant list for a random position in the chunk.
	Returns the chosen mob type, or mtInvalid if no mob chosen. */
	eMonsterType GetRandomMob(cChunkDesc & a_ChunkDesc);
} ;





/** Base class for generators that have an ore list attached to them.
Provides the storage and parsing for the ore list, as well as the generic plumbing for generating individual ores.
Descendants should override GenerateOre() to provide the specific ore generation technique.
Note that this class uses the "Nest" terminology for individual packs of ore, it doesn't imply any shape or algorithm. */
class cFinishGenOres:
	public cFinishGen
{
	using Super = cFinishGen;

public:

	struct OreInfo
	{
		BLOCKTYPE  m_BlockType;  // The type of the nest.
		NIBBLETYPE m_BlockMeta;  // The block meta
		int        m_MaxHeight;  // The highest possible a nest can occur
		int        m_NumNests;   // How many nests per chunk
		int        m_NestSize;   // The amount of blocks a nest can have.

		OreInfo() :
			m_BlockType(0),
			m_BlockMeta(0),
			m_MaxHeight(0),
			m_NumNests(0),
			m_NestSize(0)
		{
		}

		OreInfo(BLOCKTYPE a_OreType, NIBBLETYPE a_OreMeta, int a_MaxHeight, int a_NumNests, int a_NestSize) :
			m_BlockType(a_OreType),
			m_BlockMeta(a_OreMeta),
			m_MaxHeight(a_MaxHeight),
			m_NumNests(a_NumNests),
			m_NestSize(a_NestSize)
		{
		}
	};

	typedef std::vector<OreInfo> OreInfos;

	cFinishGenOres(int a_Seed, const OreInfos & a_OreInfos):
		m_Noise(a_Seed),
		m_OreInfos(a_OreInfos)
	{
	}

	// cFinishGen overrides:
	virtual void GenFinish(cChunkDesc & a_ChunkDesc) override;

	/** Returns a vector of OreInfo structures describing the default Overworld ores, usable in the constructor. */
	static const OreInfos & DefaultOverworldOres(void);

	/** Returns a vector of OreInfo structures describing the default Nether ores, usable in the constructor. */
	static const OreInfos & DefaultNetherOres(void);

	/** Returns a vector of OreInfo structures describing the default Overworld non-ore pockets (dirt, diorite etc), usable in the constructor. */
	static const OreInfos & DefaultNaturalPatches(void);

	/** Parses the parameter string into OreInfos array.
	See OreInfosToString() for the complementary function.
	Used for loading configuration from INI files. */
	static OreInfos OreInfosFromString(const AString & a_OreInfosString);

	/** Returns a string that represents the OreInfos given as the parameter.
	See OreInfosFromString() for the complementary function.
	Used for storing defaults in the INI file. */
	static AString OreInfosToString(const OreInfos & a_OreInfos);

	/** (Re-)sets the seed used by the internal generating mechanisms. */
	void SetSeed(int a_Seed);

protected:
	/** The noise used for generating. */
	cNoise m_Noise;

	/** All the ores enabled in this generator. */
	OreInfos m_OreInfos;


	/** Generates a single ore in the specified chunk image.
	a_Seq is the sequencing number (used as a complement to seed to make each ore in the same chunk have different nests) */
	virtual void GenerateOre(
		cChunkDesc & a_ChunkDesc,
		BLOCKTYPE a_OreType, NIBBLETYPE a_OreMeta,
		int a_MaxHeight, int a_NumNests, int a_NestSize,
		int a_Seq
	) = 0;

	// TODO: Helper function to parse a config string into m_OreInfos
};





class cFinishGenOreNests:
	public cFinishGenOres
{
	using Super = cFinishGenOres;

public:

	cFinishGenOreNests(int a_Seed, const OreInfos & a_OreInfos):
		Super(a_Seed, a_OreInfos)
	{}

protected:

	// cFinishGenOreClumps overrides:
	virtual void GenerateOre(
		cChunkDesc & a_ChunkDesc,
		BLOCKTYPE a_OreType, NIBBLETYPE a_OreMeta,
		int a_MaxHeight, int a_NumNests, int a_NestSize,
		int a_Seq
	) override;
} ;





class cFinishGenOrePockets:
	public cFinishGenOres
{
	using Super = cFinishGenOres;

public:

	cFinishGenOrePockets(int a_Seed, const OreInfos & a_OreInfos):
		Super(a_Seed, a_OreInfos)
	{}

	/** Reads the configuration from the specified INI file.
	a_GenName is the name of the generator (this class may be used for OrePockets and DirtPockets, each has a different default).
	Returns true on success, false and logs errors to console on failure. */
	bool Initialize(cIniFile & a_IniFile, const AString & a_GenName);

protected:

	// cFinishGenOreClumps overrides:
	virtual void GenerateOre(
		cChunkDesc & a_ChunkDesc,
		BLOCKTYPE a_OreType, NIBBLETYPE a_OreMeta,
		int a_MaxNestHeight, int a_NumNests, int a_NestSize,
		int a_Seq
	) override;

	/** Calculates the pockets for the specified chunk and imprints them into the specified ChunkDesc (not necessarily the same chunk).
	a_Seq is the sequence number of the ore, to provide another source of randomness. */
	void imprintChunkOrePockets(
		int a_ChunkX, int a_ChunkZ,
		cChunkDesc & a_ChunkDesc,
		BLOCKTYPE a_OreType, NIBBLETYPE a_OreMeta,
		int a_MaxHeight, int a_NumNests, int a_NestSize,
		int a_Seq
	);

	/** Imprints a single pocket of the specified ore at the specified coords into the chunk.
	The pocket shape has its minimum X and Z coords specified, Y can be anywhere around the specified Y coord.
	a_Seq is the sequence number of the ore, to provide another source of randomness. */
	void imprintPocket(
		cChunkDesc & a_ChunkDesc,
		int a_MinPocketX, int a_PocketY, int a_MinPocketZ,
		int a_NestSize, int a_Seq,
		BLOCKTYPE a_OreType, NIBBLETYPE a_OreMeta
	);

	/** Imprints a single sphere of the specified ore at the specified coords. */
	void imprintSphere(
		cChunkDesc & a_ChunkDesc,
		double a_SphereX, double a_SphereY, double a_SphereZ, double a_Radius,
		BLOCKTYPE a_OreType, NIBBLETYPE a_OreMeta
	);
};




