
#include "Globals.h"

#include "ChunkGenerator.h"
#include "inifile/iniFile.h"
#include "ChunkDesc.h"
#include "ComposableGenerator.h"
#include "Noise3DGenerator.h"
#include "../MersenneTwister.h"





/// If the generation queue size exceeds this number, a warning will be output
const unsigned int QUEUE_WARNING_LIMIT = 1000;

/// If the generation queue size exceeds this number, chunks with no clients will be skipped
const unsigned int QUEUE_SKIP_LIMIT = 500;





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cChunkGenerator:

cChunkGenerator::cChunkGenerator(void) :
	super("cChunkGenerator"),
	m_Generator(NULL),
	m_PluginInterface(NULL),
	m_ChunkSink(NULL)
{
}





cChunkGenerator::~cChunkGenerator()
{
	Stop();
}





bool cChunkGenerator::Start(cPluginInterface & a_PluginInterface, cChunkSink & a_ChunkSink, cIniFile & a_IniFile)
{
	m_PluginInterface = &a_PluginInterface;
	m_ChunkSink = &a_ChunkSink;

	MTRand rnd;
	m_Seed = a_IniFile.GetValueSetI("Seed", "Seed", rnd.randInt());
	AString GeneratorName = a_IniFile.GetValueSet("Generator", "Generator", "Composable");

	if (NoCaseCompare(GeneratorName, "Noise3D") == 0)
	{
		m_Generator = new cNoise3DGenerator(*this);
	}
	else
	{
		if (NoCaseCompare(GeneratorName, "composable") != 0)
		{
			LOGWARN("[Generator]::Generator value \"%s\" not recognized, using \"Composable\".", GeneratorName.c_str());
		}
		m_Generator = new cComposableGenerator(*this);
	}

	if (m_Generator == NULL)
	{
		LOGERROR("Generator could not start, aborting the server");
		return false;
	}

	m_Generator->Initialize(a_IniFile);

	return super::Start();
}





void cChunkGenerator::Stop(void)
{
	m_ShouldTerminate = true;
	m_Event.Set();
	m_evtRemoved.Set();  // Wake up anybody waiting for empty queue
	Wait();

	delete m_Generator;
	m_Generator = NULL;
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
	if (m_Generator != NULL)
	{
		m_Generator->GenerateBiomes(a_ChunkX, a_ChunkZ, a_BiomeMap);
	}
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
	ASSERT(m_Generator != NULL);
	return m_Generator->GetBiomeAt(a_BlockX, a_BlockZ);
}





BLOCKTYPE cChunkGenerator::GetIniBlock(cIniFile & a_IniFile, const AString & a_SectionName, const AString & a_ValueName, const AString & a_Default)
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





void cChunkGenerator::Execute(void)
{
	// To be able to display performance information, the generator counts the chunks generated.
	// When the queue gets empty, the count is reset, so that waiting for the queue is not counted into the total time.
	int NumChunksGenerated = 0;  // Number of chunks generated since the queue was last empty
	clock_t GenerationStart = clock();  // Clock tick when the queue started to fill
	clock_t LastReportTick = clock();  // Clock tick of the last report made (so that performance isn't reported too often)

	while (!m_ShouldTerminate)
	{
		cCSLock Lock(m_CS);
		while (m_Queue.size() == 0)
		{
			if ((NumChunksGenerated > 16) && (clock() - LastReportTick > CLOCKS_PER_SEC))
			{
				LOG("Chunk generator performance: %.2f ch/s (%d ch total)",
					(double)NumChunksGenerated * CLOCKS_PER_SEC/ (clock() - GenerationStart),
					NumChunksGenerated
				);
			}
			cCSUnlock Unlock(Lock);
			m_Event.Wait();
			if (m_ShouldTerminate)
			{
				return;
			}
			NumChunksGenerated = 0;
			GenerationStart = clock();
			LastReportTick = clock();
		}

		cChunkCoords coords = m_Queue.front();		// Get next coord from queue
		m_Queue.erase( m_Queue.begin() );	// Remove coordinate from queue
		bool SkipEnabled = (m_Queue.size() > QUEUE_SKIP_LIMIT);
		Lock.Unlock();			// Unlock ASAP
		m_evtRemoved.Set();

		// Display perf info once in a while:
		if ((NumChunksGenerated > 16) && (clock() - LastReportTick > 2 * CLOCKS_PER_SEC))
		{
			LOG("Chunk generator performance: %.2f ch/s (%d ch total)",
				(double)NumChunksGenerated * CLOCKS_PER_SEC / (clock() - GenerationStart),
				NumChunksGenerated
			);
			LastReportTick = clock();
		}

		// Hack for regenerating chunks: if Y != 0, the chunk is considered invalid, even if it has its data set
		if ((coords.m_ChunkY == 0) && m_ChunkSink->IsChunkValid(coords.m_ChunkX, coords.m_ChunkZ))
		{
			LOGD("Chunk [%d, %d] already generated, skipping generation", coords.m_ChunkX, coords.m_ChunkZ);
			// Already generated, ignore request
			continue;
		}

		if (SkipEnabled && !m_ChunkSink->HasChunkAnyClients(coords.m_ChunkX, coords.m_ChunkZ))
		{
			LOGWARNING("Chunk generator overloaded, skipping chunk [%d, %d]", coords.m_ChunkX, coords.m_ChunkZ);
			continue;
		}

		LOGD("Generating chunk [%d, %d, %d]", coords.m_ChunkX, coords.m_ChunkY, coords.m_ChunkZ);
		DoGenerate(coords.m_ChunkX, coords.m_ChunkY, coords.m_ChunkZ);

		NumChunksGenerated++;
	}  // while (!bStop)
}




void cChunkGenerator::DoGenerate(int a_ChunkX, int a_ChunkY, int a_ChunkZ)
{
	ASSERT(m_PluginInterface != NULL);
	ASSERT(m_ChunkSink != NULL);
	
	cChunkDesc ChunkDesc(a_ChunkX, a_ChunkZ);
	m_PluginInterface->CallHookChunkGenerating(ChunkDesc);
	m_Generator->DoGenerate(a_ChunkX, a_ChunkZ, ChunkDesc);
	m_PluginInterface->CallHookChunkGenerated(ChunkDesc);

	#ifdef _DEBUG
	// Verify that the generator has produced valid data:
	ChunkDesc.VerifyHeightmap();
	#endif

	m_ChunkSink->OnChunkGenerated(ChunkDesc);
}





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cChunkGenerator::cGenerator:

cChunkGenerator::cGenerator::cGenerator(cChunkGenerator & a_ChunkGenerator) :
	m_ChunkGenerator(a_ChunkGenerator)
{
}





void cChunkGenerator::cGenerator::Initialize(cIniFile & a_IniFile)
{
	UNUSED(a_IniFile);
}





EMCSBiome cChunkGenerator::cGenerator::GetBiomeAt(int a_BlockX, int a_BlockZ)
{
	cChunkDef::BiomeMap Biomes;
	int Y = 0;
	int ChunkX, ChunkZ;
	cChunkDef::AbsoluteToRelative(a_BlockX, Y, a_BlockZ, ChunkX, ChunkZ);
	GenerateBiomes(ChunkX, ChunkZ, Biomes);
	return cChunkDef::GetBiome(Biomes, a_BlockX, a_BlockZ);
}




