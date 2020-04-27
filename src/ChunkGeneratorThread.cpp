#include "Globals.h"
#include "ChunkGeneratorThread.h"
#include "Generating/ChunkGenerator.h"
#include "Generating/ChunkDesc.h"





/** If the generation queue size exceeds this number, a warning will be output */
const size_t QUEUE_WARNING_LIMIT = 1000;

/** If the generation queue size exceeds this number, chunks with no clients will be skipped */
const size_t QUEUE_SKIP_LIMIT = 500;





cChunkGeneratorThread::cChunkGeneratorThread(void) :
	Super("cChunkGeneratorThread"),
	m_Generator(nullptr),
	m_PluginInterface(nullptr),
	m_ChunkSink(nullptr)
{
}





cChunkGeneratorThread::~cChunkGeneratorThread()
{
	Stop();
}





bool cChunkGeneratorThread::Initialize(cPluginInterface & a_PluginInterface, cChunkSink & a_ChunkSink, cIniFile & a_IniFile)
{
	m_PluginInterface = &a_PluginInterface;
	m_ChunkSink = &a_ChunkSink;

	m_Generator = cChunkGenerator::CreateFromIniFile(a_IniFile);
	if (m_Generator == nullptr)
	{
		LOGERROR("Generator could not start, aborting the server");
		return false;
	}
	return true;
}





void cChunkGeneratorThread::Stop(void)
{
	m_ShouldTerminate = true;
	m_Event.Set();
	m_evtRemoved.Set();  // Wake up anybody waiting for empty queue
	Super::Stop();
	m_Generator.reset();
}





void cChunkGeneratorThread::QueueGenerateChunk(
	cChunkCoords a_Coords,
	bool a_ForceRegeneration,
	cChunkCoordCallback * a_Callback
)
{
	ASSERT(m_ChunkSink->IsChunkQueued(a_Coords));

	{
		cCSLock Lock(m_CS);

		// Add to queue, issue a warning if too many:
		if (m_Queue.size() >= QUEUE_WARNING_LIMIT)
		{
			LOGWARN("WARNING: Adding chunk %s to generation queue; Queue is too big! (%zu)", a_Coords.ToString().c_str(), m_Queue.size());
		}
		m_Queue.push_back(QueueItem{a_Coords, a_ForceRegeneration, a_Callback});
	}

	m_Event.Set();
}





void cChunkGeneratorThread::GenerateBiomes(cChunkCoords a_Coords, cChunkDef::BiomeMap & a_BiomeMap)
{
	if (m_Generator != nullptr)
	{
		m_Generator->GenerateBiomes(a_Coords, a_BiomeMap);
	}
}





void cChunkGeneratorThread::WaitForQueueEmpty(void)
{
	cCSLock Lock(m_CS);
	while (!m_ShouldTerminate && !m_Queue.empty())
	{
		cCSUnlock Unlock(Lock);
		m_evtRemoved.Wait();
	}
}





size_t cChunkGeneratorThread::GetQueueLength(void) const
{
	cCSLock Lock(m_CS);
	return m_Queue.size();
}





int cChunkGeneratorThread::GetSeed() const
{
	return m_Generator->GetSeed();
}





EMCSBiome cChunkGeneratorThread::GetBiomeAt(int a_BlockX, int a_BlockZ)
{
	ASSERT(m_Generator != nullptr);
	return m_Generator->GetBiomeAt(a_BlockX, a_BlockZ);
}





void cChunkGeneratorThread::Execute(void)
{
	// To be able to display performance information, the generator counts the chunks generated.
	// When the queue gets empty, the count is reset, so that waiting for the queue is not counted into the total time.
	int NumChunksGenerated = 0;  // Number of chunks generated since the queue was last empty
	clock_t GenerationStart = clock();  // Clock tick when the queue started to fill
	clock_t LastReportTick = clock();  // Clock tick of the last report made (so that performance isn't reported too often)

	while (!m_ShouldTerminate)
	{
		cCSLock Lock(m_CS);
		while (m_Queue.empty())
		{
			if ((NumChunksGenerated > 16) && (clock() - LastReportTick > CLOCKS_PER_SEC))
			{
				/* LOG("Chunk generator performance: %.2f ch / sec (%d ch total)",
					static_cast<double>(NumChunksGenerated) * CLOCKS_PER_SEC/ (clock() - GenerationStart),
					NumChunksGenerated
				); */
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

		if (m_Queue.empty())
		{
			// Sometimes the queue remains empty
			// If so, we can't do any front() operations on it!
			continue;
		}

		auto item = m_Queue.front();  // Get next chunk from the queue
		bool SkipEnabled = (m_Queue.size() > QUEUE_SKIP_LIMIT);
		m_Queue.erase(m_Queue.begin());  // Remove the item from the queue
		Lock.Unlock();  // Unlock ASAP
		m_evtRemoved.Set();

		// Display perf info once in a while:
		if ((NumChunksGenerated > 512) && (clock() - LastReportTick > 2 * CLOCKS_PER_SEC))
		{
			LOG("Chunk generator performance: %.2f ch / sec (%d ch total)",
				static_cast<double>(NumChunksGenerated) * CLOCKS_PER_SEC / (clock() - GenerationStart),
				NumChunksGenerated
			);
			LastReportTick = clock();
		}

		// Skip the chunk if it's already generated and regeneration is not forced. Report as success:
		if (!item.m_ForceRegeneration && m_ChunkSink->IsChunkValid(item.m_Coords))
		{
			LOGD("Chunk %s already generated, skipping generation", item.m_Coords.ToString().c_str());
			if (item.m_Callback != nullptr)
			{
				item.m_Callback->Call(item.m_Coords, true);
			}
			continue;
		}

		// Skip the chunk if the generator is overloaded:
		if (SkipEnabled && !m_ChunkSink->HasChunkAnyClients(item.m_Coords))
		{
			LOGWARNING("Chunk generator overloaded, skipping chunk %s", item.m_Coords.ToString().c_str());
			if (item.m_Callback != nullptr)
			{
				item.m_Callback->Call(item.m_Coords, false);
			}
			continue;
		}

		// Generate the chunk:
		DoGenerate(item.m_Coords);
		if (item.m_Callback != nullptr)
		{
			item.m_Callback->Call(item.m_Coords, true);
		}
		NumChunksGenerated++;
	}  // while (!bStop)
}





void cChunkGeneratorThread::DoGenerate(cChunkCoords a_Coords)
{
	ASSERT(m_PluginInterface != nullptr);
	ASSERT(m_ChunkSink != nullptr);

	cChunkDesc ChunkDesc(a_Coords);
	m_PluginInterface->CallHookChunkGenerating(ChunkDesc);
	m_Generator->Generate(ChunkDesc);
	m_PluginInterface->CallHookChunkGenerated(ChunkDesc);

	#ifdef _DEBUG
		// Verify that the generator has produced valid data:
		ChunkDesc.VerifyHeightmap();
	#endif

	m_ChunkSink->OnChunkGenerated(ChunkDesc);
}





