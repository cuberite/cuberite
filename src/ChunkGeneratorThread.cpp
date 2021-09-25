#include "Globals.h"
#include "ChunkGeneratorThread.h"
#include "Generating/ChunkGenerator.h"
#include "Generating/ChunkDesc.h"

// The new and free macros break tbb
#pragma push_macro("new")
#undef new
#pragma push_macro("free")
#undef free
#include <oneapi/tbb/task_group.h>
#pragma pop_macro("free")
#pragma pop_macro("new")





/** If the generation queue size exceeds this number, a warning will be output */
const size_t QUEUE_WARNING_LIMIT = 1000;

/** If the generation queue size exceeds this number, chunks with no clients will be skipped */
const size_t QUEUE_SKIP_LIMIT = 500;





cChunkGeneratorThread::cChunkGeneratorThread(void) :
	Super("Chunk Generator"),
	m_Generator(nullptr),
	m_PluginInterface(nullptr),
	m_ChunkSink(nullptr)
{
}





cChunkGeneratorThread::~cChunkGeneratorThread()
{
	Stop();
}





bool cChunkGeneratorThread::Initialize(const cPluginInterface & a_PluginInterface,
                                       cChunkSink & a_ChunkSink, cIniFile & a_IniFile)
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
		std::shared_lock SharedLock{m_SharedMutex};	 // This operation is thread safe unless someone calls GetQueueLength()
		m_Queue.emplace(a_Coords, a_ForceRegeneration, a_Callback);
	}

	m_Event.Set();
}





void cChunkGeneratorThread::GenerateBiomes(cChunkCoords a_Coords, cChunkDef::BiomeMap & a_BiomeMap) const
{
	if (m_Generator != nullptr)
	{
		m_Generator->GenerateBiomes(a_Coords, a_BiomeMap);
	}
}





size_t cChunkGeneratorThread::GetQueueLength(void) const
{
	std::unique_lock UniqueLock{m_SharedMutex};	 // This operation is not thread safe, meaning we must lock the queue
	return m_Queue.unsafe_size();
}





int cChunkGeneratorThread::GetSeed() const
{
	return m_Generator->GetSeed();
}





EMCSBiome cChunkGeneratorThread::GetBiomeAt(int a_BlockX, int a_BlockZ) const
{
	ASSERT(m_Generator != nullptr);
	return m_Generator->GetBiomeAt(a_BlockX, a_BlockZ);
}





void cChunkGeneratorThread::Execute(void)
{
	// To be able to display performance information, the generator counts the chunks generated.
	// When the queue gets empty, the count is reset, so that waiting for the queue is not counted into the total time.
	std::atomic_size_t NumChunksGenerated = 0;	// Number of chunks generated since the queue was last empty
	std::atomic GenerationStart = clock();		// Clock tick when the queue started to fill
	std::atomic LastReportTick = clock();		// Clock tick of the last report made (so that performance isn't reported too often)
	tbb::task_group Pool;						// TBB task group for this generation thread

	while (!m_ShouldTerminate)
	{
		while (GetQueueLength() == 0)
		{
			// Wait for tasks to complete
			if (auto Status = Pool.wait(); Status != tbb::complete)
			{
				LOGD("Generator task group result status: %d", Status);
			}

			m_Event.Wait();

			if (m_ShouldTerminate)
			{
				return;
			}

			// Reset perf counters
			NumChunksGenerated = 0;
			GenerationStart = clock();
			LastReportTick = clock();
		}

		const auto SkipEnabled = GetQueueLength() > QUEUE_SKIP_LIMIT;

		{
			std::shared_lock SharedLock{m_SharedMutex};	 // This operation is concurrent unless someone calls

			QueueItem Item{};
			while (m_Queue.try_pop(Item))
			{
				// Take Item and SkipEnabled by copy
				Pool.run([&, Item, SkipEnabled]() {
					Generate(Item, SkipEnabled);

					++NumChunksGenerated;

					// Display perf info once in a while:
					if ((NumChunksGenerated > 512U) && (clock() - LastReportTick > 2 * CLOCKS_PER_SEC))
					{
						LOG("Chunk generator performance: %.2f ch / sec (%d ch total)",
							static_cast<double>(NumChunksGenerated) * CLOCKS_PER_SEC / (clock() - GenerationStart), NumChunksGenerated);
						LastReportTick = clock();
					}
				});
			}
		}
	}
}





void cChunkGeneratorThread::Generate(const QueueItem & a_Item, const bool a_SkipEnable) const
{
	// Skip the chunk if it's already generated and regeneration is not forced.
	// Report as success:
	if (!a_Item.m_ForceRegeneration && m_ChunkSink->IsChunkValid(a_Item.m_Coords))
	{
		LOGD("Chunk %s already generated, skipping generation", a_Item.m_Coords.ToString().c_str());
		if (a_Item.m_Callback != nullptr)
		{
			a_Item.m_Callback->Call(a_Item.m_Coords, true);
		}
		return;
	}

	// Skip the chunk if the generator is overloaded:
	if (a_SkipEnable && !m_ChunkSink->HasChunkAnyClients(a_Item.m_Coords))
	{
		LOGWARNING("Chunk generator overloaded, skipping chunk %s", a_Item.m_Coords.ToString().c_str());
		if (a_Item.m_Callback != nullptr)
		{
			a_Item.m_Callback->Call(a_Item.m_Coords, false);
		}
		return;
	}

	// Generate the chunk:
	DoGenerate(a_Item.m_Coords);
	if (a_Item.m_Callback != nullptr)
	{
		a_Item.m_Callback->Call(a_Item.m_Coords, true);
	}
}





void cChunkGeneratorThread::DoGenerate(cChunkCoords a_Coords) const
{
	ASSERT(m_PluginInterface != nullptr);
	ASSERT(m_ChunkSink != nullptr);

	cChunkDesc ChunkDesc(a_Coords);
	m_PluginInterface->CallHookChunkGenerating(ChunkDesc);
	m_Generator->Generate(ChunkDesc);
	m_PluginInterface->CallHookChunkGenerated(ChunkDesc);

	#ifndef NDEBUG
		// Verify that the generator has produced valid data:
		ChunkDesc.VerifyHeightmap();
	#endif

	m_ChunkSink->OnChunkGenerated(ChunkDesc);
}
