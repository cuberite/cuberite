
#include "Globals.h"

#include "cChunkGenerator.h"
#include "cWorld.h"
#include "../iniFile/iniFile.h"
#include "BioGen.h"
#include "HeiGen.h"
#include "CompoGen.h"
#include "StructGen.h"
#include "FinishGen.h"
#include "cRoot.h"
#include "cPluginManager.h"
#include "cLuaChunk.h"





/// If the generation queue size exceeds this number, a warning will be output
const int QUEUE_WARNING_LIMIT = 1000;

/// If the generation queue size exceeds this number, chunks with no clients will be skipped
const int QUEUE_SKIP_LIMIT = 500;





static BLOCKTYPE GetIniBlock(cIniFile & a_IniFile, const AString & a_SectionName, const AString & a_ValueName, const AString & a_Default)
{
	AString BlockType = a_IniFile.GetValueSet(a_SectionName, a_ValueName, a_Default);
	BLOCKTYPE Block = BlockStringToType(BlockType);
	if (Block < 0)
	{
		LOGWARN("[&s].%s Could not parse block value \"%s\". Using default: \"%s\".", a_SectionName.c_str(), a_ValueName.c_str(), BlockType.c_str(),a_Default.c_str());
		return BlockStringToType(a_Default);
	}
	return Block;
}





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cChunkGenerator:

cChunkGenerator::cChunkGenerator(void)
	: super("cChunkGenerator")
	, m_World(NULL)
	, m_BiomeGen(NULL)
	, m_HeightGen(NULL)
	, m_CompositionGen(NULL)
{
}





cChunkGenerator::~cChunkGenerator()
{
	Stop();
}





bool cChunkGenerator::Start(cWorld * a_World, cIniFile & a_IniFile)
{
	MTRand rnd;
	m_World = a_World;
	m_Seed = a_IniFile.GetValueSetI("Seed", "Seed", rnd.randInt());

	InitBiomeGen(a_IniFile);
	InitHeightGen(a_IniFile);
	InitCompositionGen(a_IniFile);
	InitStructureGens(a_IniFile);
	InitFinishGens(a_IniFile);
	
	a_IniFile.WriteFile();
	
	return super::Start();
}





void cChunkGenerator::Stop(void)
{
	m_ShouldTerminate = true;
	m_Event.Set();
	m_evtRemoved.Set();  // Wake up anybody waiting for empty queue
	Wait();

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
}





void cChunkGenerator::InitBiomeGen(cIniFile & a_IniFile)
{
	AString BiomeGenName = a_IniFile.GetValueSet("Generator", "BiomeGen", "");
	if (BiomeGenName.empty())
	{
		LOGWARN("[Generator]::BiomeGen value not found in world.ini, using \"DistortedVoronoi\".");
		BiomeGenName = "DistortedVoronoi";
	}
	
	bool CacheOffByDefault = false;
	if (NoCaseCompare(BiomeGenName, "constant") == 0)
	{
		AString Biome = a_IniFile.GetValueSet("Generator", "ConstantBiome", "Plains");
		EMCSBiome b = StringToBiome(Biome);
		if (b == -1)
		{
			LOGWARN("[Generator]::ConstantBiome value \"%s\" not recognized, using \"Plains\".", Biome.c_str());
			b = biPlains;
		}
		m_BiomeGen = new cBioGenConstant(b);
		CacheOffByDefault = true;  // we're generating faster than a cache would retrieve data :)
	}
	else if (NoCaseCompare(BiomeGenName, "checkerboard") == 0)
	{
		int BiomeSize  = a_IniFile.GetValueSetI("Generator", "CheckerboardBiomeSize", 64);
		AString Biomes = a_IniFile.GetValueSet ("Generator", "CheckerBoardBiomes",    "");
		m_BiomeGen = new cBioGenCheckerboard(BiomeSize, Biomes);
		CacheOffByDefault = true;  // we're (probably) generating faster than a cache would retrieve data
	}
	else if (NoCaseCompare(BiomeGenName, "voronoi") == 0)
	{
		int CellSize   = a_IniFile.GetValueSetI("Generator", "VoronoiCellSize", 64);
		AString Biomes = a_IniFile.GetValueSet ("Generator", "VoronoiBiomes",   "");
		m_BiomeGen = new cBioGenVoronoi(m_Seed, CellSize, Biomes);
	}
	else
	{
		if (NoCaseCompare(BiomeGenName, "distortedvoronoi") != 0)
		{
			LOGWARNING("Unknown BiomeGen \"%s\", using \"DistortedVoronoi\" instead.", BiomeGenName.c_str());
		}
		int CellSize   = a_IniFile.GetValueSetI("Generator", "DistortedVoronoiCellSize", 96);
		AString Biomes = a_IniFile.GetValueSet ("Generator", "DistortedVoronoiBiomes",   "");
		m_BiomeGen = new cBioGenDistortedVoronoi(m_Seed, CellSize, Biomes);
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
		LOGINFO("Using a cache for biomegen of size %d.", CacheSize);
		m_BiomeGen = new cBioGenCache(m_BiomeGen, CacheSize);
	}
}





void cChunkGenerator::InitHeightGen(cIniFile & a_IniFile)
{
	AString HeightGenName = a_IniFile.GetValueSet("Generator", "HeightGen", "");
	if (HeightGenName.empty())
	{
		LOGWARN("[Generator]::HeightGen value not found in world.ini, using \"Biomal\".");
		HeightGenName = "Biomal";
	}
	
	bool CacheOffByDefault = false;
	if (NoCaseCompare(HeightGenName, "flat") == 0)
	{
		int Height = a_IniFile.GetValueSetI("Generator", "FlatHeight", 5);
		m_HeightGen = new cHeiGenFlat(Height);
		CacheOffByDefault = true;  // We're generating faster than a cache would retrieve data
	}
	else if (NoCaseCompare(HeightGenName, "classic") == 0)
	{
		// These used to be in terrain.ini, but now they are in world.ini (so that multiple worlds can have different values):
		float HeightFreq1 = (float)a_IniFile.GetValueSetF("Generator", "ClassicHeightFreq1", 0.1);
		float HeightFreq2 = (float)a_IniFile.GetValueSetF("Generator", "ClassicHeightFreq2", 1.0);
		float HeightFreq3 = (float)a_IniFile.GetValueSetF("Generator", "ClassicHeightFreq3", 2.0);
		float HeightAmp1  = (float)a_IniFile.GetValueSetF("Generator", "ClassicHeightAmp1",  1.0);
		float HeightAmp2  = (float)a_IniFile.GetValueSetF("Generator", "ClassicHeightAmp2",  0.5);
		float HeightAmp3  = (float)a_IniFile.GetValueSetF("Generator", "ClassicHeightAmp3",  0.5);
		m_HeightGen = new cHeiGenClassic(m_Seed, HeightFreq1, HeightAmp1, HeightFreq2, HeightAmp2, HeightFreq3, HeightAmp3);
	}
	else  // "biomal" or <not found>
	{
		if (NoCaseCompare(HeightGenName, "biomal") != 0)
		{
			LOGWARN("Unknown HeightGen \"%s\", using \"Biomal\" instead.", HeightGenName.c_str());
		}
		m_HeightGen = new cHeiGenBiomal(m_Seed, *m_BiomeGen);
	}
	
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
		LOGINFO("Using a cache for Heightgen of size %d.", CacheSize);
		m_HeightGen = new cHeiGenCache(m_HeightGen, CacheSize);
	}
}





void cChunkGenerator::InitCompositionGen(cIniFile & a_IniFile)
{
	AString CompoGenName = a_IniFile.GetValueSet("Generator", "CompositionGen", "");
	if (CompoGenName.empty())
	{
		LOGWARN("[Generator]::CompositionGen value not found in world.ini, using \"Biomal\".");
		CompoGenName = "Biomal";
	}
	if (NoCaseCompare(CompoGenName, "sameblock") == 0)
	{
		AString BlockType = a_IniFile.GetValueSet("Generator", "SameBlockType", "");
		if (BlockType.empty())
		{
			LOGWARN("[Generator]::SameBlockType value not found in world.ini, using \"stone\".");
			BlockType = "stone";
		}
		int Block = GetIniBlock(a_IniFile, "[Generator]", "SameBlockType", "stone");
		bool Bedrocked = (a_IniFile.GetValueSetI("Generator", "SameBlockBedrocked", 1) != 0);
		m_CompositionGen = new cCompoGenSameBlock((BLOCKTYPE)Block, Bedrocked);
	}
	else if (NoCaseCompare(CompoGenName, "debugbiomes") == 0)
	{
		m_CompositionGen = new cCompoGenDebugBiomes;
	}
	else if (NoCaseCompare(CompoGenName, "classic") == 0)
	{
		int SeaLevel    = a_IniFile.GetValueSetI("Generator", "ClassicSeaLevel", 60);
		int BeachHeight = a_IniFile.GetValueSetI("Generator", "ClassicBeachHeight", 2);
		int BeachDepth  = a_IniFile.GetValueSetI("Generator", "ClassicBeachDepth", 4);
		BLOCKTYPE BlockTop         = GetIniBlock(a_IniFile, "Generator", "ClassicBlockTop",         "grass");
		BLOCKTYPE BlockMiddle      = GetIniBlock(a_IniFile, "Generator", "ClassicBlockMiddle",      "dirt");
		BLOCKTYPE BlockBottom      = GetIniBlock(a_IniFile, "Generator", "ClassicBlockBottom",      "stone");
		BLOCKTYPE BlockBeach       = GetIniBlock(a_IniFile, "Generator", "ClassicBlockBeach",       "sand");
		BLOCKTYPE BlockBeachBottom = GetIniBlock(a_IniFile, "Generator", "ClassicBlockBeachBottom", "sandstone");
		BLOCKTYPE BlockSea         = GetIniBlock(a_IniFile, "Generator", "ClassicBlockSea",         "9");
		m_CompositionGen = new cCompoGenClassic(
			SeaLevel, BeachHeight, BeachDepth, BlockTop, BlockMiddle, BlockBottom, BlockBeach,
			BlockBeachBottom, BlockSea
		);
	}
	else
	{
		if (NoCaseCompare(CompoGenName, "biomal") != 0)
		{
			LOGWARN("Unknown CompositionGen \"%s\", using \"biomal\" instead.", CompoGenName.c_str());
		}
		int SeaLevel = a_IniFile.GetValueSetI("Generator", "BiomalSeaLevel", 62);
		m_CompositionGen = new cCompoGenBiomal(m_Seed, SeaLevel);
	}
}





void cChunkGenerator::InitStructureGens(cIniFile & a_IniFile)
{
	AString Structures = a_IniFile.GetValueSet("Generator", "Structures", "Trees,MarbleCaves,OreNests");

	AStringVector Str = StringSplit(Structures, ",");
	for (AStringVector::const_iterator itr = Str.begin(); itr != Str.end(); ++itr)
	{
		if (NoCaseCompare(*itr, "trees") == 0)
		{
			m_StructureGens.push_back(new cStructGenTrees(m_Seed, m_BiomeGen, m_HeightGen, m_CompositionGen));
		}
		else if (NoCaseCompare(*itr, "marblecaves") == 0)
		{
			m_StructureGens.push_back(new cStructGenMarbleCaves(m_Seed));
		}
		else if (NoCaseCompare(*itr, "orenests") == 0)
		{
			m_StructureGens.push_back(new cStructGenOreNests(m_Seed));
		}
		else
		{
			LOGWARNING("Unknown structure generator: \"%s\". Ignoring.", itr->c_str());
		}
	}  // for itr - Str[]
}





void cChunkGenerator::InitFinishGens(cIniFile & a_IniFile)
{
	AString Structures = a_IniFile.GetValueSet("Generator", "Finishers", "SprinkleFoliage,Ice,Snow");

	AStringVector Str = StringSplit(Structures, ",");
	for (AStringVector::const_iterator itr = Str.begin(); itr != Str.end(); ++itr)
	{
		if (NoCaseCompare(*itr, "SprinkleFoliage") == 0)
		{
			m_FinishGens.push_back(new cFinishGenSprinkleFoliage(m_Seed));
		}
		else if (NoCaseCompare(*itr, "Snow") == 0)
		{
			m_FinishGens.push_back(new cFinishGenSnow);
		}
		else if (NoCaseCompare(*itr, "Ice") == 0)
		{
			m_FinishGens.push_back(new cFinishGenIce);
		}
	}  // for itr - Str[]
}





void cChunkGenerator::QueueGenerateChunk(int a_ChunkX, int a_ChunkY, int a_ChunkZ)
{
	{
		cCSLock Lock(m_CS);
		
		// Check if it is already in the queue:
		for (cChunkCoordsList::iterator itr = m_Queue.begin(); itr != m_Queue.end(); ++itr)
		{
			if ((itr->m_ChunkX == a_ChunkX) && (itr->m_ChunkY == a_ChunkY) && (itr->m_ChunkZ == a_ChunkZ))
			{
				// Already in the queue, bail out
				return;
			}
		}  // for itr - m_Queue[]
		
		// Add to queue, issue a warning if too many:
		if (m_Queue.size() >= QUEUE_WARNING_LIMIT)
		{
			LOGWARN("WARNING: Adding chunk [%i, %i] to generation queue; Queue is too big! (%i)", a_ChunkX, a_ChunkZ, m_Queue.size());
		}
		m_Queue.push_back(cChunkCoords(a_ChunkX, a_ChunkY, a_ChunkZ));
	}
	
	m_Event.Set();
}





void cChunkGenerator::GenerateBiomes(int a_ChunkX, int a_ChunkZ, cChunkDef::BiomeMap & a_BiomeMap)
{
	m_BiomeGen->GenBiomes(a_ChunkX, a_ChunkZ, a_BiomeMap);
}





void cChunkGenerator::WaitForQueueEmpty(void)
{
	cCSLock Lock(m_CS);
	while (!m_ShouldTerminate && !m_Queue.empty())
	{
		cCSUnlock Unlock(Lock);
		m_evtRemoved.Wait();
	}
}





int cChunkGenerator::GetQueueLength(void)
{
	cCSLock Lock(m_CS);
	return (int)m_Queue.size();
}





EMCSBiome cChunkGenerator::GetBiomeAt(int a_BlockX, int a_BlockZ)
{
	cChunkDef::BiomeMap Biomes;
	int Y = 0;
	int ChunkX, ChunkZ;
	cWorld::AbsoluteToRelative(a_BlockX, Y, a_BlockZ, ChunkX, Y, ChunkZ);
	m_BiomeGen->GenBiomes(ChunkX, ChunkZ, Biomes);
	return cChunkDef::GetBiome(Biomes, a_BlockX, a_BlockZ);
}





void cChunkGenerator::Execute(void)
{
	while (!m_ShouldTerminate)
	{
		cCSLock Lock(m_CS);
		while (m_Queue.size() == 0)
		{
			cCSUnlock Unlock(Lock);
			m_Event.Wait();
			if (m_ShouldTerminate)
			{
				return;
			}
		}
		
		cChunkCoords coords = m_Queue.front();		// Get next coord from queue
		m_Queue.erase( m_Queue.begin() );	// Remove coordinate from queue
		bool SkipEnabled = (m_Queue.size() > QUEUE_SKIP_LIMIT);
		Lock.Unlock();			// Unlock ASAP
		m_evtRemoved.Set();

		// Hack for regenerating chunks: if Y != 0, the chunk is considered invalid, even if it has its data set
		if ((coords.m_ChunkY == 0) && m_World->IsChunkValid(coords.m_ChunkX, coords.m_ChunkY, coords.m_ChunkZ))
		{
			LOGD("Chunk [%d, %d] already generated, skipping generation", coords.m_ChunkX, coords.m_ChunkZ);
			// Already generated, ignore request
			continue;
		}
		
		if (SkipEnabled && !m_World->HasChunkAnyClients(coords.m_ChunkX, coords.m_ChunkY, coords.m_ChunkZ))
		{
			LOGWARNING("Chunk generator overloaded, skipping chunk [%d, %d, %d]", coords.m_ChunkX, coords.m_ChunkY, coords.m_ChunkZ);
			continue;
		}
		
		LOGD("Generating chunk [%d, %d, %d]", coords.m_ChunkX, coords.m_ChunkY, coords.m_ChunkZ);
		DoGenerate(coords.m_ChunkX, coords.m_ChunkY, coords.m_ChunkZ);
		
		// Save the chunk right after generating, so that we don't have to generate it again on next run
		m_World->GetStorage().QueueSaveChunk(coords.m_ChunkX, coords.m_ChunkY, coords.m_ChunkZ);
	}  // while (!bStop)
}




void cChunkGenerator::DoGenerate(int a_ChunkX, int a_ChunkY, int a_ChunkZ)
{
	cChunkDef::BiomeMap     BiomeMap;
	cChunkDef::BlockTypes   BlockTypes;
	cChunkDef::BlockNibbles BlockMeta;
	cChunkDef::HeightMap    HeightMap;
	cEntityList Entities;
	cBlockEntityList BlockEntities;
	
	cLuaChunk LuaChunk( BlockTypes, BlockMeta, HeightMap, BiomeMap );
	if( cRoot::Get()->GetPluginManager()->CallHook( cPluginManager::E_PLUGIN_CHUNK_GENERATING, 3, a_ChunkX, a_ChunkZ, &LuaChunk ) )
	{
		// A plugin interrupted generation, handle something plugin specific
		if( LuaChunk.IsUsingDefaultBiomes() )
		{
			m_BiomeGen->GenBiomes(a_ChunkX, a_ChunkZ, BiomeMap);
		}
		if( LuaChunk.IsUsingDefaultComposition() )
		{
			m_CompositionGen->ComposeTerrain(a_ChunkX, a_ChunkZ, BlockTypes, BlockMeta, HeightMap, BiomeMap, Entities, BlockEntities);
		}
		if( LuaChunk.IsUsingDefaultStructures() )
		{
			for (cStructureGenList::iterator itr = m_StructureGens.begin(); itr != m_StructureGens.end(); ++itr)
			{
				(*itr)->GenStructures(a_ChunkX, a_ChunkZ, BlockTypes, BlockMeta, HeightMap, Entities, BlockEntities);
			}   // for itr - m_StructureGens[]
		}
		if( LuaChunk.IsUsingDefaultFinish() )
		{
			for (cFinishGenList::iterator itr = m_FinishGens.begin(); itr != m_FinishGens.end(); ++itr)
			{
				(*itr)->GenFinish(a_ChunkX, a_ChunkZ, BlockTypes, BlockMeta, HeightMap, BiomeMap, Entities, BlockEntities);
			}  // for itr - m_FinishGens[]
		}
	}
	else
	{
		// Use the composed generator:
		m_BiomeGen->GenBiomes(a_ChunkX, a_ChunkZ, BiomeMap);
		m_HeightGen->GenHeightMap(a_ChunkX, a_ChunkZ, HeightMap);
		m_CompositionGen->ComposeTerrain(a_ChunkX, a_ChunkZ, BlockTypes, BlockMeta, HeightMap, BiomeMap, Entities, BlockEntities);
		for (cStructureGenList::iterator itr = m_StructureGens.begin(); itr != m_StructureGens.end(); ++itr)
		{
			(*itr)->GenStructures(a_ChunkX, a_ChunkZ, BlockTypes, BlockMeta, HeightMap, Entities, BlockEntities);
		}   // for itr - m_StructureGens[]
		for (cFinishGenList::iterator itr = m_FinishGens.begin(); itr != m_FinishGens.end(); ++itr)
		{
			(*itr)->GenFinish(a_ChunkX, a_ChunkZ, BlockTypes, BlockMeta, HeightMap, BiomeMap, Entities, BlockEntities);
		}  // for itr - m_FinishGens[]
	}
	
	m_World->SetChunkData(
		a_ChunkX, a_ChunkY, a_ChunkZ, 
		BlockTypes, BlockMeta,
		NULL, NULL,  // We don't have lighting, chunk will be lighted when needed
		&HeightMap, &BiomeMap,
		Entities, BlockEntities,
		true
	);
	
	cRoot::Get()->GetPluginManager()->CallHook(cPluginManager::E_PLUGIN_CHUNK_GENERATED, 3, m_World, a_ChunkX, a_ChunkZ);
}




