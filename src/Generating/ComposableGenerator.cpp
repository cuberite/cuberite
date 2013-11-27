
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
#include "Noise3DGenerator.h"
#include "Ravines.h"










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
	for (cStructureGenList::const_iterator itr = m_StructureGens.begin(); itr != m_StructureGens.end(); ++itr)
	{
		delete *itr;
	}
	m_StructureGens.clear();

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





void cComposableGenerator::Initialize(cWorld * a_World, cIniFile & a_IniFile)
{
	super::Initialize(a_World, a_IniFile);
	
	InitBiomeGen(a_IniFile);
	InitHeightGen(a_IniFile);
	InitCompositionGen(a_IniFile);
	InitStructureGens(a_IniFile);
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
	
	if (a_ChunkDesc.IsUsingDefaultComposition())
	{
		m_CompositionGen->ComposeTerrain(a_ChunkDesc);
	}

	if (a_ChunkDesc.IsUsingDefaultStructures())
	{	
		for (cStructureGenList::iterator itr = m_StructureGens.begin(); itr != m_StructureGens.end(); ++itr)
		{
			(*itr)->GenStructures(a_ChunkDesc);
		}   // for itr - m_StructureGens[]
	}
	
	if (a_ChunkDesc.IsUsingDefaultFinish())
	{
		for (cFinishGenList::iterator itr = m_FinishGens.begin(); itr != m_FinishGens.end(); ++itr)
		{
			(*itr)->GenFinish(a_ChunkDesc);
		}  // for itr - m_FinishGens[]
	}
}





void cComposableGenerator::InitBiomeGen(cIniFile & a_IniFile)
{
	AString BiomeGenName = a_IniFile.GetValueSet("Generator", "BiomeGen", "");
	if (BiomeGenName.empty())
	{
		LOGWARN("[Generator] BiomeGen value not set in world.ini, using \"MultiStepMap\".");
		BiomeGenName = "MultiStepMap";
	}
	
	int Seed = m_ChunkGenerator.GetSeed();
	bool CacheOffByDefault = false;
	if (NoCaseCompare(BiomeGenName, "constant") == 0)
	{
		m_BiomeGen = new cBioGenConstant;
		CacheOffByDefault = true;  // we're generating faster than a cache would retrieve data :)
	}
	else if (NoCaseCompare(BiomeGenName, "checkerboard") == 0)
	{
		m_BiomeGen = new cBioGenCheckerboard;
		CacheOffByDefault = true;  // we're (probably) generating faster than a cache would retrieve data
	}
	else if (NoCaseCompare(BiomeGenName, "voronoi") == 0)
	{
		m_BiomeGen = new cBioGenVoronoi(Seed);
	}
	else if (NoCaseCompare(BiomeGenName, "distortedvoronoi") == 0)
	{
		m_BiomeGen = new cBioGenDistortedVoronoi(Seed);
	}
	else
	{
		if (NoCaseCompare(BiomeGenName, "multistepmap") != 0)
		{
			LOGWARNING("Unknown BiomeGen \"%s\", using \"MultiStepMap\" instead.", BiomeGenName.c_str());
		}
		m_BiomeGen = new cBioGenMultiStepMap(Seed);

		/*
		// Performance-testing:
		LOGINFO("Measuring performance of cBioGenMultiStepMap...");
		clock_t BeginTick = clock();
		for (int x = 0; x < 5000; x++)
		{
			cChunkDef::BiomeMap Biomes;
			m_BiomeGen->GenBiomes(x * 5, x * 5, Biomes);
		}
		clock_t Duration = clock() - BeginTick;
		LOGINFO("cBioGenMultiStepMap for 5000 chunks took %d ticks (%.02f sec)", Duration, (double)Duration / CLOCKS_PER_SEC);
		//*/
	}
	
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
	m_BiomeGen->InitializeBiomeGen(a_IniFile);
}





void cComposableGenerator::InitHeightGen(cIniFile & a_IniFile)
{
	AString HeightGenName = a_IniFile.GetValueSet("Generator", "HeightGen", "");
	if (HeightGenName.empty())
	{
		LOGWARN("[Generator] HeightGen value not set in world.ini, using \"Biomal\".");
		HeightGenName = "Biomal";
	}
	
	int Seed = m_ChunkGenerator.GetSeed();
	bool CacheOffByDefault = false;
	if (NoCaseCompare(HeightGenName, "flat") == 0)
	{
		m_HeightGen = new cHeiGenFlat;
		CacheOffByDefault = true;  // We're generating faster than a cache would retrieve data
	}
	else if (NoCaseCompare(HeightGenName, "classic") == 0)
	{
		m_HeightGen = new cHeiGenClassic(Seed);
	}
	else if (NoCaseCompare(HeightGenName, "DistortedHeightmap") == 0)
	{
		m_HeightGen = new cDistortedHeightmap(Seed, *m_BiomeGen);
	}
	else if (NoCaseCompare(HeightGenName, "End") == 0)
	{
		m_HeightGen = new cEndGen(Seed);
	}
	else if (NoCaseCompare(HeightGenName, "Noise3D") == 0)
	{
		m_HeightGen = new cNoise3DComposable(Seed);
	}
	else  // "biomal" or <not found>
	{
		if (NoCaseCompare(HeightGenName, "biomal") != 0)
		{
			LOGWARN("Unknown HeightGen \"%s\", using \"Biomal\" instead.", HeightGenName.c_str());
		}
		m_HeightGen = new cHeiGenBiomal(Seed, *m_BiomeGen);

		/*
		// Performance-testing:
		LOGINFO("Measuring performance of cHeiGenBiomal...");
		clock_t BeginTick = clock();
		for (int x = 0; x < 500; x++)
		{
			cChunkDef::HeightMap Heights;
			m_HeightGen->GenHeightMap(x * 5, x * 5, Heights);
		}
		clock_t Duration = clock() - BeginTick;
		LOGINFO("HeightGen for 500 chunks took %d ticks (%.02f sec)", Duration, (double)Duration / CLOCKS_PER_SEC);
		//*/
	}
	
	// Read the settings:
	m_HeightGen->InitializeHeightGen(a_IniFile);
	
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
	int Seed = m_ChunkGenerator.GetSeed();
	AString CompoGenName = a_IniFile.GetValueSet("Generator", "CompositionGen", "");
	if (CompoGenName.empty())
	{
		LOGWARN("[Generator] CompositionGen value not set in world.ini, using \"Biomal\".");
		CompoGenName = "Biomal";
	}
	if (NoCaseCompare(CompoGenName, "sameblock") == 0)
	{
		m_CompositionGen = new cCompoGenSameBlock;
	}
	else if (NoCaseCompare(CompoGenName, "debugbiomes") == 0)
	{
		m_CompositionGen = new cCompoGenDebugBiomes;
	}
	else if (NoCaseCompare(CompoGenName, "classic") == 0)
	{
		m_CompositionGen = new cCompoGenClassic;
	}
	else if (NoCaseCompare(CompoGenName, "DistortedHeightmap") == 0)
	{
		m_CompositionGen = new cDistortedHeightmap(Seed, *m_BiomeGen);
	}
	else if (NoCaseCompare(CompoGenName, "end") == 0)
	{
		m_CompositionGen = new cEndGen(Seed);
	}
	else if (NoCaseCompare(CompoGenName, "nether") == 0)
	{
		m_CompositionGen = new cCompoGenNether(Seed);
	}
	else if (NoCaseCompare(CompoGenName, "Noise3D") == 0)
	{
		m_CompositionGen = new cNoise3DComposable(m_ChunkGenerator.GetSeed());
	}
	else
	{
		if (NoCaseCompare(CompoGenName, "biomal") != 0)
		{
			LOGWARN("Unknown CompositionGen \"%s\", using \"biomal\" instead.", CompoGenName.c_str());
		}
		m_CompositionGen = new cCompoGenBiomal(Seed);

		/*
		// Performance-testing:
		LOGINFO("Measuring performance of cCompoGenBiomal...");
		clock_t BeginTick = clock();
		for (int x = 0; x < 500; x++)
		{
			cChunkDesc Desc(200 + x * 8, 200 + x * 8);
			m_BiomeGen->GenBiomes(Desc.GetChunkX(), Desc.GetChunkZ(), Desc.GetBiomeMap());
			m_HeightGen->GenHeightMap(Desc.GetChunkX(), Desc.GetChunkZ(), Desc.GetHeightMap());
			m_CompositionGen->ComposeTerrain(Desc);
		}
		clock_t Duration = clock() - BeginTick;
		LOGINFO("CompositionGen for 500 chunks took %d ticks (%.02f sec)", Duration, (double)Duration / CLOCKS_PER_SEC);
		//*/
	}
	
	// Read the settings from the ini file:
	m_CompositionGen->InitializeCompoGen(a_IniFile);
	
	int CompoGenCacheSize = a_IniFile.GetValueSetI("Generator", "CompositionGenCacheSize", 64);
	if (CompoGenCacheSize > 1)
	{
		m_UnderlyingCompositionGen = m_CompositionGen;
		m_CompositionGen = new cCompoGenCache(*m_UnderlyingCompositionGen, 32);
	}
}





void cComposableGenerator::InitStructureGens(cIniFile & a_IniFile)
{
	AString Structures = a_IniFile.GetValueSet("Generator", "Structures", "Ravines, WormNestCaves, WaterLakes, LavaLakes, OreNests, Trees");

	int Seed = m_ChunkGenerator.GetSeed();
	AStringVector Str = StringSplitAndTrim(Structures, ",");
	for (AStringVector::const_iterator itr = Str.begin(); itr != Str.end(); ++itr)
	{
		if (NoCaseCompare(*itr, "DualRidgeCaves") == 0)
		{
			float Threshold = (float)a_IniFile.GetValueSetF("Generator", "DualRidgeCavesThreshold", 0.3);
			m_StructureGens.push_back(new cStructGenDualRidgeCaves(Seed, Threshold));
		}
		else if (NoCaseCompare(*itr, "DirectOverhangs") == 0)
		{
			m_StructureGens.push_back(new cStructGenDirectOverhangs(Seed));
		}
		else if (NoCaseCompare(*itr, "DistortedMembraneOverhangs") == 0)
		{
			m_StructureGens.push_back(new cStructGenDistortedMembraneOverhangs(Seed));
		}
		else if (NoCaseCompare(*itr, "LavaLakes") == 0)
		{
			int Probability = a_IniFile.GetValueSetI("Generator", "LavaLakesProbability", 10);
			m_StructureGens.push_back(new cStructGenLakes(Seed * 5 + 16873, E_BLOCK_STATIONARY_LAVA, *m_HeightGen, Probability));
		}
		else if (NoCaseCompare(*itr, "MarbleCaves") == 0)
		{
			m_StructureGens.push_back(new cStructGenMarbleCaves(Seed));
		}
		else if (NoCaseCompare(*itr, "MineShafts") == 0)
		{
			int GridSize        = a_IniFile.GetValueSetI("Generator", "MineShaftsGridSize",        512);
			int MaxSystemSize   = a_IniFile.GetValueSetI("Generator", "MineShaftsMaxSystemSize",   160);
			int ChanceCorridor  = a_IniFile.GetValueSetI("Generator", "MineShaftsChanceCorridor",  600);
			int ChanceCrossing  = a_IniFile.GetValueSetI("Generator", "MineShaftsChanceCrossing",  200);
			int ChanceStaircase = a_IniFile.GetValueSetI("Generator", "MineShaftsChanceStaircase", 200);
			m_StructureGens.push_back(new cStructGenMineShafts(
				Seed, GridSize, MaxSystemSize, 
				ChanceCorridor, ChanceCrossing, ChanceStaircase
			));
		}
		else if (NoCaseCompare(*itr, "OreNests") == 0)
		{
			m_StructureGens.push_back(new cStructGenOreNests(Seed));
		}
		else if (NoCaseCompare(*itr, "Ravines") == 0)
		{
			m_StructureGens.push_back(new cStructGenRavines(Seed, 128));
		}
		else if (NoCaseCompare(*itr, "Trees") == 0)
		{
			m_StructureGens.push_back(new cStructGenTrees(Seed, m_BiomeGen, m_HeightGen, m_CompositionGen));
		}
		else if (NoCaseCompare(*itr, "WaterLakes") == 0)
		{
			int Probability = a_IniFile.GetValueSetI("Generator", "WaterLakesProbability", 25);
			m_StructureGens.push_back(new cStructGenLakes(Seed * 3 + 652, E_BLOCK_STATIONARY_WATER, *m_HeightGen, Probability));
		}
		else if (NoCaseCompare(*itr, "WormNestCaves") == 0)
		{
			m_StructureGens.push_back(new cStructGenWormNestCaves(Seed));
		}
		else
		{
			LOGWARNING("Unknown structure generator: \"%s\". Ignoring.", itr->c_str());
		}
	}  // for itr - Str[]
}





void cComposableGenerator::InitFinishGens(cIniFile & a_IniFile)
{
	int Seed = m_ChunkGenerator.GetSeed();
	AString Structures = a_IniFile.GetValueSet("Generator", "Finishers", "SprinkleFoliage,Ice,Snow,Lilypads,BottomLava,DeadBushes,PreSimulator");

	AStringVector Str = StringSplitAndTrim(Structures, ",");
	for (AStringVector::const_iterator itr = Str.begin(); itr != Str.end(); ++itr)
	{
		// Finishers, alpha-sorted:
		if (NoCaseCompare(*itr, "BottomLava") == 0)
		{
			int DefaultBottomLavaLevel = (m_World->GetDimension() == dimNether) ? 30 : 10;
			int BottomLavaLevel = a_IniFile.GetValueSetI("Generator", "BottomLavaLevel", DefaultBottomLavaLevel);
			m_FinishGens.push_back(new cFinishGenBottomLava(BottomLavaLevel));
		}
		else if (NoCaseCompare(*itr, "DeadBushes") == 0)
		{
			m_FinishGens.push_back(new cFinishGenSingleBiomeSingleTopBlock(Seed, E_BLOCK_DEAD_BUSH, biDesert, 2, E_BLOCK_SAND, E_BLOCK_SAND));
		}
		else if (NoCaseCompare(*itr, "Ice") == 0)
		{
			m_FinishGens.push_back(new cFinishGenIce);
		}
		else if (NoCaseCompare(*itr, "LavaSprings") == 0)
		{
			m_FinishGens.push_back(new cFinishGenFluidSprings(Seed, E_BLOCK_LAVA, a_IniFile, *m_World));
		}
		else if (NoCaseCompare(*itr, "Lilypads") == 0)
		{
			m_FinishGens.push_back(new cFinishGenSingleBiomeSingleTopBlock(Seed, E_BLOCK_LILY_PAD, biSwampland, 4, E_BLOCK_WATER, E_BLOCK_STATIONARY_WATER));
		}
		else if (NoCaseCompare(*itr, "PreSimulator") == 0)
		{
			m_FinishGens.push_back(new cFinishGenPreSimulator);
		}
		else if (NoCaseCompare(*itr, "Snow") == 0)
		{
			m_FinishGens.push_back(new cFinishGenSnow);
		}
		else if (NoCaseCompare(*itr, "SprinkleFoliage") == 0)
		{
			m_FinishGens.push_back(new cFinishGenSprinkleFoliage(Seed));
		}
		else if (NoCaseCompare(*itr, "WaterSprings") == 0)
		{
			m_FinishGens.push_back(new cFinishGenFluidSprings(Seed, E_BLOCK_WATER, a_IniFile, *m_World));
		}
	}  // for itr - Str[]
}




