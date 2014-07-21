
// ComposableGenerator.cpp

// Implements the cComposableGenerator class representing the chunk generator that takes the composition approach to generating chunks

#include "Globals.h"

#include "ComposableGenerator.h"
#include "../World.h"
#include "inifile/iniFile.h"
#include "../Root.h"

// Individual composed algorithms:
#include "BioGen.h"
#include "HeiGen.h"
#include "CompoGen.h"
#include "StructGen.h"
#include "FinishGen.h"

#include "Caves.h"
#include "DistortedHeightmap.h"
#include "EndGen.h"
#include "MineShafts.h"
#include "NetherFortGen.h"
#include "Noise3DGenerator.h"
#include "POCPieceGenerator.h"
#include "RainbowRoadsGen.h"
#include "Ravines.h"
#include "TestRailsGen.h"
#include "UnderwaterBaseGen.h"
#include "VillageGen.h"





////////////////////////////////////////////////////////////////////////////////
// cTerrainCompositionGen:

cTerrainCompositionGen * cTerrainCompositionGen::CreateCompositionGen(cIniFile & a_IniFile, cBiomeGen & a_BiomeGen, cTerrainHeightGen & a_HeightGen, int a_Seed)
{
	AString CompoGenName = a_IniFile.GetValueSet("Generator", "CompositionGen", "");
	if (CompoGenName.empty())
	{
		LOGWARN("[Generator] CompositionGen value not set in world.ini, using \"Biomal\".");
		CompoGenName = "Biomal";
		a_IniFile.SetValue("Generator", "CompositionGen", CompoGenName);
	}
	
	cTerrainCompositionGen * res = NULL;
	if (NoCaseCompare(CompoGenName, "sameblock") == 0)
	{
		res = new cCompoGenSameBlock;
	}
	else if (NoCaseCompare(CompoGenName, "debugbiomes") == 0)
	{
		res = new cCompoGenDebugBiomes;
	}
	else if (NoCaseCompare(CompoGenName, "classic") == 0)
	{
		res = new cCompoGenClassic;
	}
	else if (NoCaseCompare(CompoGenName, "DistortedHeightmap") == 0)
	{
		res = new cDistortedHeightmap(a_Seed, a_BiomeGen);
	}
	else if (NoCaseCompare(CompoGenName, "end") == 0)
	{
		res = new cEndGen(a_Seed);
	}
	else if (NoCaseCompare(CompoGenName, "nether") == 0)
	{
		res = new cCompoGenNether(a_Seed);
	}
	else if (NoCaseCompare(CompoGenName, "Noise3D") == 0)
	{
		res = new cNoise3DComposable(a_Seed);
	}
	else if (NoCaseCompare(CompoGenName, "biomal") == 0)
	{
		res = new cCompoGenBiomal(a_Seed);

		/*
		// Performance-testing:
		LOGINFO("Measuring performance of cCompoGenBiomal...");
		clock_t BeginTick = clock();
		for (int x = 0; x < 500; x++)
		{
			cChunkDesc Desc(200 + x * 8, 200 + x * 8);
			a_BiomeGen->GenBiomes(Desc.GetChunkX(), Desc.GetChunkZ(), Desc.GetBiomeMap());
			a_HeightGen->GenHeightMap(Desc.GetChunkX(), Desc.GetChunkZ(), Desc.GetHeightMap());
			res->ComposeTerrain(Desc);
		}
		clock_t Duration = clock() - BeginTick;
		LOGINFO("CompositionGen for 500 chunks took %d ticks (%.02f sec)", Duration, (double)Duration / CLOCKS_PER_SEC);
		//*/
	}
	else
	{
		LOGWARN("Unknown CompositionGen \"%s\", using \"Biomal\" instead.", CompoGenName.c_str());
		a_IniFile.DeleteValue("Generator", "CompositionGen");
		a_IniFile.SetValue("Generator", "CompositionGen", "Biomal");
		return CreateCompositionGen(a_IniFile, a_BiomeGen, a_HeightGen, a_Seed);
	}
	ASSERT(res != NULL);
	
	// Read the settings from the ini file:
	res->InitializeCompoGen(a_IniFile);
	
	return res;
}





////////////////////////////////////////////////////////////////////////////////
// cComposableGenerator:

cComposableGenerator::cComposableGenerator(cChunkGenerator & a_ChunkGenerator) :
	super(a_ChunkGenerator),
	m_BiomeGen(NULL),
	m_HeightGen(NULL),
	m_CompositionGen(NULL),
	m_UnderlyingBiomeGen(NULL),
	m_UnderlyingHeightGen(NULL),
	m_UnderlyingCompositionGen(NULL)
{
}





cComposableGenerator::~cComposableGenerator()
{
	// Delete the generating composition:
	for (cFinishGenList::const_iterator itr = m_FinishGens.begin(); itr != m_FinishGens.end(); ++itr)
	{
		delete *itr;
	}
	m_FinishGens.clear();

	delete m_CompositionGen;
	m_CompositionGen = NULL;
	delete m_HeightGen;
	m_HeightGen = NULL;
	delete m_BiomeGen;
	m_BiomeGen = NULL;
	delete m_UnderlyingCompositionGen;
	m_UnderlyingCompositionGen = NULL;
	delete m_UnderlyingHeightGen;
	m_UnderlyingHeightGen = NULL;
	delete m_UnderlyingBiomeGen;
	m_UnderlyingBiomeGen = NULL;
}





void cComposableGenerator::Initialize(cIniFile & a_IniFile)
{
	super::Initialize(a_IniFile);
	
	InitBiomeGen(a_IniFile);
	InitHeightGen(a_IniFile);
	InitCompositionGen(a_IniFile);
	InitFinishGens(a_IniFile);
}





void cComposableGenerator::GenerateBiomes(int a_ChunkX, int a_ChunkZ, cChunkDef::BiomeMap & a_BiomeMap)
{
	if (m_BiomeGen != NULL)  // Quick fix for generator deinitializing before the world storage finishes loading
	{
		m_BiomeGen->GenBiomes(a_ChunkX, a_ChunkZ, a_BiomeMap);
	}
}





void cComposableGenerator::DoGenerate(int a_ChunkX, int a_ChunkZ, cChunkDesc & a_ChunkDesc)
{
	if (a_ChunkDesc.IsUsingDefaultBiomes())
	{
		m_BiomeGen->GenBiomes(a_ChunkX, a_ChunkZ, a_ChunkDesc.GetBiomeMap());
	}
	
	if (a_ChunkDesc.IsUsingDefaultHeight())
	{
		m_HeightGen->GenHeightMap(a_ChunkX, a_ChunkZ, a_ChunkDesc.GetHeightMap());
	}
	
	bool ShouldUpdateHeightmap = false;
	if (a_ChunkDesc.IsUsingDefaultComposition())
	{
		m_CompositionGen->ComposeTerrain(a_ChunkDesc);
		ShouldUpdateHeightmap = true;
	}

	if (a_ChunkDesc.IsUsingDefaultFinish())
	{
		for (cFinishGenList::iterator itr = m_FinishGens.begin(); itr != m_FinishGens.end(); ++itr)
		{
			(*itr)->GenFinish(a_ChunkDesc);
		}  // for itr - m_FinishGens[]
		ShouldUpdateHeightmap = true;
	}
	
	if (ShouldUpdateHeightmap)
	{
		a_ChunkDesc.UpdateHeightmap();
	}
}





void cComposableGenerator::InitBiomeGen(cIniFile & a_IniFile)
{
	bool CacheOffByDefault = false;
	m_BiomeGen = cBiomeGen::CreateBiomeGen(a_IniFile, m_ChunkGenerator.GetSeed(), CacheOffByDefault);
	
	// Add a cache, if requested:
	int CacheSize = a_IniFile.GetValueSetI("Generator", "BiomeGenCacheSize", CacheOffByDefault ? 0 : 64);
	if (CacheSize > 0)
	{
		if (CacheSize < 4)
		{
			LOGWARNING("Biomegen cache size set too low, would hurt performance instead of helping. Increasing from %d to %d",
				CacheSize, 4
			);
			CacheSize = 4;
		}
		LOGD("Using a cache for biomegen of size %d.", CacheSize);
		m_UnderlyingBiomeGen = m_BiomeGen;
		m_BiomeGen = new cBioGenCache(m_UnderlyingBiomeGen, CacheSize);
	}
}





void cComposableGenerator::InitHeightGen(cIniFile & a_IniFile)
{
	bool CacheOffByDefault = false;
	m_HeightGen = cTerrainHeightGen::CreateHeightGen(a_IniFile, *m_BiomeGen, m_ChunkGenerator.GetSeed(), CacheOffByDefault);
	
	// Add a cache, if requested:
	int CacheSize = a_IniFile.GetValueSetI("Generator", "HeightGenCacheSize", CacheOffByDefault ? 0 : 64);
	if (CacheSize > 0)
	{
		if (CacheSize < 4)
		{
			LOGWARNING("Heightgen cache size set too low, would hurt performance instead of helping. Increasing from %d to %d",
				CacheSize, 4
			);
			CacheSize = 4;
		}
		LOGD("Using a cache for Heightgen of size %d.", CacheSize);
		m_UnderlyingHeightGen = m_HeightGen;
		m_HeightGen = new cHeiGenCache(*m_UnderlyingHeightGen, CacheSize);
	}
}





void cComposableGenerator::InitCompositionGen(cIniFile & a_IniFile)
{
	m_CompositionGen = cTerrainCompositionGen::CreateCompositionGen(a_IniFile, *m_BiomeGen, *m_HeightGen, m_ChunkGenerator.GetSeed());
	
	int CompoGenCacheSize = a_IniFile.GetValueSetI("Generator", "CompositionGenCacheSize", 64);
	if (CompoGenCacheSize > 1)
	{
		m_UnderlyingCompositionGen = m_CompositionGen;
		m_CompositionGen = new cCompoGenCache(*m_UnderlyingCompositionGen, 32);
	}
}





void cComposableGenerator::InitFinishGens(cIniFile & a_IniFile)
{
	int Seed = m_ChunkGenerator.GetSeed();
	eDimension Dimension = StringToDimension(a_IniFile.GetValue("General", "Dimension", "Overworld"));

	// Older configuration used "Structures" in addition to "Finishers"; we don't distinguish between the two anymore (#398)
	// Therefore, we load Structures from the ini file for compatibility, but move its contents over to Finishers:
	AString Structures = a_IniFile.GetValue("Generator", "Structures", "");
	AString Finishers = a_IniFile.GetValueSet("Generator", "Finishers", "Ravines, WormNestCaves, WaterLakes, LavaLakes, OreNests, Trees, SprinkleFoliage, Ice, Snow, Lilypads, BottomLava, DeadBushes, PreSimulator");
	if (!Structures.empty())
	{
		LOGINFO("[Generator].Structures is deprecated, moving the contents to [Generator].Finishers.");
		// Structures used to generate before Finishers, so place them first:
		Structures.append(", ");
		Finishers = Structures + Finishers;
		a_IniFile.SetValue("Generator", "Finishers", Finishers);
	}
	a_IniFile.DeleteValue("Generator", "Structures");

	// Create all requested finishers:
	AStringVector Str = StringSplitAndTrim(Finishers, ",");
	for (AStringVector::const_iterator itr = Str.begin(); itr != Str.end(); ++itr)
	{
		// Finishers, alpha-sorted:
		if (NoCaseCompare(*itr, "BottomLava") == 0)
		{
			int DefaultBottomLavaLevel = (Dimension == dimNether) ? 30 : 10;
			int BottomLavaLevel = a_IniFile.GetValueSetI("Generator", "BottomLavaLevel", DefaultBottomLavaLevel);
			m_FinishGens.push_back(new cFinishGenBottomLava(BottomLavaLevel));
		}
		else if (NoCaseCompare(*itr, "DeadBushes") == 0)
		{
			m_FinishGens.push_back(new cFinishGenSingleBiomeSingleTopBlock(Seed, E_BLOCK_DEAD_BUSH, biDesert, 2, E_BLOCK_SAND, E_BLOCK_SAND));
		}
		else if (NoCaseCompare(*itr, "DirectOverhangs") == 0)
		{
			m_FinishGens.push_back(new cStructGenDirectOverhangs(Seed));
		}
		else if (NoCaseCompare(*itr, "DistortedMembraneOverhangs") == 0)
		{
			m_FinishGens.push_back(new cStructGenDistortedMembraneOverhangs(Seed));
		}
		else if (NoCaseCompare(*itr, "DualRidgeCaves") == 0)
		{
			float Threshold = (float)a_IniFile.GetValueSetF("Generator", "DualRidgeCavesThreshold", 0.3);
			m_FinishGens.push_back(new cStructGenDualRidgeCaves(Seed, Threshold));
		}
		else if (NoCaseCompare(*itr, "Ice") == 0)
		{
			m_FinishGens.push_back(new cFinishGenIce);
		}
		else if (NoCaseCompare(*itr, "LavaLakes") == 0)
		{
			int Probability = a_IniFile.GetValueSetI("Generator", "LavaLakesProbability", 10);
			m_FinishGens.push_back(new cStructGenLakes(Seed * 5 + 16873, E_BLOCK_STATIONARY_LAVA, *m_HeightGen, Probability));
		}
		else if (NoCaseCompare(*itr, "LavaSprings") == 0)
		{
			m_FinishGens.push_back(new cFinishGenFluidSprings(Seed, E_BLOCK_LAVA, a_IniFile, Dimension));
		}
		else if (NoCaseCompare(*itr, "MarbleCaves") == 0)
		{
			m_FinishGens.push_back(new cStructGenMarbleCaves(Seed));
		}
		else if (NoCaseCompare(*itr, "MineShafts") == 0)
		{
			int GridSize        = a_IniFile.GetValueSetI("Generator", "MineShaftsGridSize",        512);
			int MaxOffset       = a_IniFile.GetValueSetI("Generator", "MineShaftsMaxOffset",       256);
			int MaxSystemSize   = a_IniFile.GetValueSetI("Generator", "MineShaftsMaxSystemSize",   160);
			int ChanceCorridor  = a_IniFile.GetValueSetI("Generator", "MineShaftsChanceCorridor",  600);
			int ChanceCrossing  = a_IniFile.GetValueSetI("Generator", "MineShaftsChanceCrossing",  200);
			int ChanceStaircase = a_IniFile.GetValueSetI("Generator", "MineShaftsChanceStaircase", 200);
			m_FinishGens.push_back(new cStructGenMineShafts(
				Seed, GridSize, MaxOffset, MaxSystemSize,
				ChanceCorridor, ChanceCrossing, ChanceStaircase
			));
		}
		else if (NoCaseCompare(*itr, "Lilypads") == 0)
		{
			m_FinishGens.push_back(new cFinishGenSingleBiomeSingleTopBlock(Seed, E_BLOCK_LILY_PAD, biSwampland, 4, E_BLOCK_WATER, E_BLOCK_STATIONARY_WATER));
		}
		else if (NoCaseCompare(*itr, "NetherClumpFoliage") == 0)
		{
			m_FinishGens.push_back(new cFinishGenNetherClumpFoliage(Seed));
		}
		else if (NoCaseCompare(*itr, "NetherForts") == 0)
		{
			int GridSize  = a_IniFile.GetValueSetI("Generator", "NetherFortsGridSize", 512);
			int MaxOffset = a_IniFile.GetValueSetI("Generator", "NetherFortMaxOffset", 128);
			int MaxDepth  = a_IniFile.GetValueSetI("Generator", "NetherFortsMaxDepth", 12);
			m_FinishGens.push_back(new cNetherFortGen(Seed, GridSize, MaxOffset, MaxDepth));
		}
		else if (NoCaseCompare(*itr, "OreNests") == 0)
		{
			m_FinishGens.push_back(new cStructGenOreNests(Seed));
		}
		else if (NoCaseCompare(*itr, "POCPieces") == 0)
		{
			m_FinishGens.push_back(new cPOCPieceGenerator(Seed));
		}
		else if (NoCaseCompare(*itr, "PreSimulator") == 0)
		{
			m_FinishGens.push_back(new cFinishGenPreSimulator);
		}
		else if (NoCaseCompare(*itr, "RainbowRoads") == 0)
		{
			int GridSize  = a_IniFile.GetValueSetI("Generator", "RainbowRoadsGridSize", 512);
			int MaxOffset = a_IniFile.GetValueSetI("Generator", "RainbowRoadsMaxOffset", 128);
			int MaxDepth  = a_IniFile.GetValueSetI("Generator", "RainbowRoadsMaxDepth",  30);
			int MaxSize   = a_IniFile.GetValueSetI("Generator", "RainbowRoadsMaxSize",  260);
			m_FinishGens.push_back(new cRainbowRoadsGen(Seed, GridSize, MaxOffset, MaxDepth, MaxSize));
		}
		else if (NoCaseCompare(*itr, "Ravines") == 0)
		{
			m_FinishGens.push_back(new cStructGenRavines(Seed, 128));
		}
		else if (NoCaseCompare(*itr, "Snow") == 0)
		{
			m_FinishGens.push_back(new cFinishGenSnow);
		}
		else if (NoCaseCompare(*itr, "SprinkleFoliage") == 0)
		{
			m_FinishGens.push_back(new cFinishGenSprinkleFoliage(Seed));
		}
		else if (NoCaseCompare(*itr, "TallGrass") == 0)
		{
			m_FinishGens.push_back(new cFinishGenTallGrass(Seed));
		}
		else if (NoCaseCompare(*itr, "TestRails") == 0)
		{
			m_FinishGens.push_back(new cTestRailsGen(Seed, 100, 1, 7, 50));
		}
		else if (NoCaseCompare(*itr, "Trees") == 0)
		{
			m_FinishGens.push_back(new cStructGenTrees(Seed, m_BiomeGen, m_HeightGen, m_CompositionGen));
		}
		else if (NoCaseCompare(*itr, "UnderwaterBases") == 0)
		{
			int GridSize  = a_IniFile.GetValueSetI("Generator", "UnderwaterBaseGridSize", 1024);
			int MaxOffset = a_IniFile.GetValueSetI("Generator", "UnderwaterBaseMaxOffset", 128);
			int MaxDepth  = a_IniFile.GetValueSetI("Generator", "UnderwaterBaseMaxDepth",    7);
			int MaxSize   = a_IniFile.GetValueSetI("Generator", "UnderwaterBaseMaxSize",   128);
			m_FinishGens.push_back(new cUnderwaterBaseGen(Seed, GridSize, MaxOffset, MaxDepth, MaxSize, *m_BiomeGen));
		}
		else if (NoCaseCompare(*itr, "Villages") == 0)
		{
			int GridSize   = a_IniFile.GetValueSetI("Generator", "VillageGridSize",  384);
			int MaxOffset  = a_IniFile.GetValueSetI("Generator", "VillageMaxOffset", 128);
			int MaxDepth   = a_IniFile.GetValueSetI("Generator", "VillageMaxDepth",    2);
			int MaxSize    = a_IniFile.GetValueSetI("Generator", "VillageMaxSize",   128);
			int MinDensity = a_IniFile.GetValueSetI("Generator", "VillageMinDensity", 50);
			int MaxDensity = a_IniFile.GetValueSetI("Generator", "VillageMaxDensity", 80);
			m_FinishGens.push_back(new cVillageGen(Seed, GridSize, MaxOffset, MaxDepth, MaxSize, MinDensity, MaxDensity, *m_BiomeGen, *m_HeightGen));
		}
		else if (NoCaseCompare(*itr, "WaterLakes") == 0)
		{
			int Probability = a_IniFile.GetValueSetI("Generator", "WaterLakesProbability", 25);
			m_FinishGens.push_back(new cStructGenLakes(Seed * 3 + 652, E_BLOCK_STATIONARY_WATER, *m_HeightGen, Probability));
		}
		else if (NoCaseCompare(*itr, "WaterSprings") == 0)
		{
			m_FinishGens.push_back(new cFinishGenFluidSprings(Seed, E_BLOCK_WATER, a_IniFile, Dimension));
		}
		else if (NoCaseCompare(*itr, "WormNestCaves") == 0)
		{
			int Size      = a_IniFile.GetValueSetI("Generator", "WormNestCavesSize", 64);
			int Grid      = a_IniFile.GetValueSetI("Generator", "WormNestCavesGrid", 96);
			int MaxOffset = a_IniFile.GetValueSetI("Generator", "WormNestMaxOffset", 32);
			m_FinishGens.push_back(new cStructGenWormNestCaves(Seed, Size, Grid, MaxOffset));
		}
		else
		{
			LOGWARNING("Unknown Finisher in the [Generator] section: \"%s\". Ignoring.", itr->c_str());
		}
	}  // for itr - Str[]
}




