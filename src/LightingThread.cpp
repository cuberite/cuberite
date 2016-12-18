
// LightingThread.cpp

// Implements the cLightingThread class representing the thread that processes requests for lighting

#include "Globals.h"
#include "LightingThread.h"
#include "ChunkMap.h"
#include "ChunkStay.h"
#include "World.h"






/** Chunk data callback that takes the chunk data and puts them into cLightingThread's m_BlockTypes[] / m_HeightMap[]: */
class cReader :
	public cChunkDataCallback
{
	virtual void ChunkData(const cChunkData & a_ChunkBuffer) override
	{
		a_ChunkBuffer.CopyBlockTypes(m_BlockTypes.data());
	}

	virtual void HeightMap(const cChunkDef::HeightMap * a_Heightmap) override
	{
		std::copy(std::begin(*a_Heightmap), std::end(*a_Heightmap), std::begin(m_HeightMap));
	}

public:

	cLightingThread::BlockDataArray & m_BlockTypes;  // 3x3 chunks of block types, organized as a single XZY blob of data (instead of 3x3 XZY blobs)
	cLightingThread::HeightDataArray & m_HeightMap;  // 3x3 chunks of height map,  organized as a single XZY blob of data (instead of 3x3 XZY blobs)

	cReader(cLightingThread::BlockDataArray & a_BlockTypes, cLightingThread::HeightDataArray & a_HeightMap) :
		m_BlockTypes(a_BlockTypes),
		m_HeightMap(a_HeightMap)
	{
	}
} ;





////////////////////////////////////////////////////////////////////////////////
// cLightingThread:

cLightingThread::cLightingThread(void) :
	super("cLightingThread"),
	m_World(nullptr)
{
}





cLightingThread::~cLightingThread()
{
	Stop();
}





bool cLightingThread::Start(cWorld * a_World)
{
	ASSERT(m_World == nullptr);  // Not started yet
	m_World = a_World;

	return super::Start();
}





void cLightingThread::Stop(void)
{
	m_ShouldTerminate = true;
	m_evtItemDataLoaded.Set();
	Wait();
}





void cLightingThread::QueueChunk(int a_ChunkX, int a_ChunkZ, std::unique_ptr<cChunkCoordCallback> a_CallbackAfter)
{
	ASSERT(m_World != nullptr);  // Did you call Start() properly?

	// If the chunk is already lit, skip it:
	if (m_World->IsChunkValid(a_ChunkX, a_ChunkZ) && m_World->IsChunkLighted(a_ChunkX, a_ChunkZ))
	{
		if (a_CallbackAfter != nullptr)
		{
			a_CallbackAfter->Call(a_ChunkX, a_ChunkZ, true);
		}
		return;
	}


	auto ChunkStay = new cLightingChunkStay(*this, a_ChunkX, a_ChunkZ, std::move(a_CallbackAfter));
	{
		// The ChunkStay will enqueue itself using the QueueChunkStay() once it is fully loaded
		// In the meantime, put it into the PendingQueue so that it can be removed when stopping the thread
		cCSLock Lock(m_CS);
		m_PendingQueue.emplace_back(ChunkStay);
	}
	ChunkStay->Enable(*m_World->GetChunkMap());
}





size_t cLightingThread::GetQueueLength(void)
{
	cCSLock Lock(m_CS);
	return m_PendingQueue.size();
}





void cLightingThread::Execute(void)
{
	while (!m_ShouldTerminate)
	{
		m_evtItemDataLoaded.Wait();

		decltype(m_Queue) QueuedChunks;
		{
			cCSLock Lock(m_CS);
			std::swap(m_Queue, QueuedChunks);
		}

		for (const auto & Entry : QueuedChunks)
		{
			Entry->m_BlockTypes = cpp14::make_unique<BlockDataArray>();
			Entry->m_HeightMap = cpp14::make_unique<HeightDataArray>();
		}

		m_World->QueueTask(
			[&QueuedChunks](cWorld & a_World)
			{
				for (const auto & Entry : QueuedChunks)
				{
					cReader Reader(*Entry->m_BlockTypes, *Entry->m_HeightMap);
					VERIFY(a_World.GetChunkData(Entry->m_ChunkX, Entry->m_ChunkZ, Reader));
				}
			}
		).wait();

		for (const auto & Entry : QueuedChunks)
		{
			LightChunk(*Entry);
		}
	}  // while (!m_ShouldTerminate)
}





void cLightingThread::LightChunk(cLightingChunkStay & a_Item)
{
	std::unique_ptr<BlockDataArray>
		BlockLight{ cpp14::make_unique<decltype(BlockLight)::element_type>() },
		SkyLight{ cpp14::make_unique<decltype(BlockLight)::element_type>() }
	;

	CalcLight(a_Item, PrepareBlockLight(a_Item, *BlockLight), *BlockLight);

	/*
	// DEBUG: Save chunk data with highlighted seeds for visual inspection:
	cFile f4;
	if (
		f4.Open(Printf("Chunk_%d_%d_seeds.grab", a_Item.m_ChunkX, a_Item.m_ChunkZ), cFile::fmWrite)
	)
	{
		for (int z = 0; z < cChunkDef::Width * 3; z++)
		{
			for (int y = cChunkDef::Height / 2; y >= 0; y--)
			{
				unsigned char Seeds     [cChunkDef::Width * 3];
				memcpy(Seeds, m_BlockTypes + y * cLightingChunkStay::BlocksPerYLayer + z * cChunkDef::Width * 3, cChunkDef::Width * 3);
				for (int x = 0; x < cChunkDef::Width * 3; x++)
				{
					if (m_IsSeed1[y * cLightingChunkStay::BlocksPerYLayer + z * cChunkDef::Width * 3 + x])
					{
						Seeds[x] = E_BLOCK_DIAMOND_BLOCK;
					}
				}
				f4.Write(Seeds, cChunkDef::Width * 3);
			}
		}
		f4.Close();
	}
	//*/

	CalcLight(a_Item, PrepareSkyLight(a_Item, *SkyLight), *SkyLight);

	/*
	// DEBUG: Save XY slices of the chunk data and lighting for visual inspection:
	cFile f1, f2, f3;
	if (
		f1.Open(Printf("Chunk_%d_%d_data.grab",  a_Item.m_ChunkX, a_Item.m_ChunkZ), cFile::fmWrite) &&
		f2.Open(Printf("Chunk_%d_%d_sky.grab",   a_Item.m_ChunkX, a_Item.m_ChunkZ), cFile::fmWrite) &&
		f3.Open(Printf("Chunk_%d_%d_glow.grab",  a_Item.m_ChunkX, a_Item.m_ChunkZ), cFile::fmWrite)
	)
	{
		for (int z = 0; z < cChunkDef::Width * 3; z++)
		{
			for (int y = cChunkDef::Height / 2; y >= 0; y--)
			{
				f1.Write(m_BlockTypes + y * cLightingChunkStay::BlocksPerYLayer + z * cChunkDef::Width * 3, cChunkDef::Width * 3);
				unsigned char SkyLight  [cChunkDef::Width * 3];
				unsigned char BlockLight[cChunkDef::Width * 3];
				for (int x = 0; x < cChunkDef::Width * 3; x++)
				{
					SkyLight[x]   = m_SkyLight  [y * cLightingChunkStay::BlocksPerYLayer + z * cChunkDef::Width * 3 + x] << 4;
					BlockLight[x] = m_BlockLight[y * cLightingChunkStay::BlocksPerYLayer + z * cChunkDef::Width * 3 + x] << 4;
				}
				f2.Write(SkyLight,   cChunkDef::Width * 3);
				f3.Write(BlockLight, cChunkDef::Width * 3);
			}
		}
		f1.Close();
		f2.Close();
		f3.Close();
	}
	//*/

	std::shared_ptr<BlockNibbles>
		CompressedBlockLight{ std::make_shared<decltype(CompressedBlockLight)::element_type>() },
		CompressedSkyLight{ std::make_shared<decltype(CompressedSkyLight)::element_type>() }
	;
	CompressLight(*BlockLight, *CompressedBlockLight);
	CompressLight(*SkyLight, *CompressedSkyLight);

	m_World->QueueTask(
		[&a_Item, CompressedBlockLight, CompressedSkyLight](cWorld & a_World)
		{
			a_World.ChunkLighted(
				a_Item.m_ChunkX,
				a_Item.m_ChunkZ,
				reinterpret_cast<const cChunkDef::BlockNibbles &>(*CompressedBlockLight->data()),
				reinterpret_cast<const cChunkDef::BlockNibbles &>(*CompressedSkyLight->data())
			);

			if (a_Item.m_CallbackAfter != nullptr)
			{
				a_Item.m_CallbackAfter->Call(a_Item.m_ChunkX, a_Item.m_ChunkZ, true);
			}

			a_Item.Disable();
		}
	);
}





std::vector<size_t> cLightingThread::PrepareSkyLight(const cLightingChunkStay & a_Item, BlockDataArray & a_SkyLight)
{
	std::vector<size_t> IndicesToProcess;
	IndicesToProcess.reserve(a_Item.m_HeightMap->size() * 40);

	for (size_t Index = 0; Index != a_Item.m_HeightMap->size(); ++Index)
	{
		for (int Y = cChunkDef::Height - 1; Y >= (*a_Item.m_HeightMap)[Index]; --Y)
		{
			auto HeightAdjustedIndex = Index + Y * cChunkDef::Width * cChunkDef::Width;
			a_SkyLight[HeightAdjustedIndex] = 15;

			IndicesToProcess.emplace_back(HeightAdjustedIndex);
		}
	}

	return IndicesToProcess;
}





std::vector<size_t> cLightingThread::PrepareBlockLight(const cLightingChunkStay & a_Item, BlockDataArray & a_BlockLight)
{
	std::vector<size_t> IndicesToProcess;
	IndicesToProcess.reserve(cChunkDef::Width * cChunkDef::Width);

	for (size_t Index = 0; Index != a_Item.m_BlockTypes->size(); ++Index)
	{
		auto Light = cBlockInfo::GetLightValue((*a_Item.m_BlockTypes)[Index]);
		a_BlockLight[Index] = Light;

		if (Light > 1)
		{
			IndicesToProcess.emplace_back(Index);
		}
	}

	return IndicesToProcess;
}





void cLightingThread::CalcLight(const cLightingChunkStay & a_Item, std::vector<size_t> a_IndicesToProcess, BlockDataArray & a_Light)
{
	while (!a_IndicesToProcess.empty())
	{
		auto Back = a_IndicesToProcess.back();
		a_IndicesToProcess.pop_back();

		DiscoverLightAdjacents(a_IndicesToProcess, a_Item, a_Light, Back);
	}
}





void cLightingThread::DiscoverLightAdjacents(std::vector<size_t> & a_IndicesToProcess, const cLightingChunkStay & a_Item, BlockDataArray & a_Light, size_t a_OriginatorIndex)
{
	auto Position = cChunkDef::IndexToCoordinate(a_OriginatorIndex);

	if (Position.x % cChunkDef::Width != 15)
	{
		// TODO: update neighbouring chunk
		UpdateLightAdjacent(a_IndicesToProcess, a_Item, a_Light, a_OriginatorIndex, a_OriginatorIndex + 1);
	}

	if (Position.x % cChunkDef::Width != 0)
	{
		// TODO: update neighbouring chunk
		UpdateLightAdjacent(a_IndicesToProcess, a_Item, a_Light, a_OriginatorIndex, a_OriginatorIndex - 1);
	}

	if (Position.z % cChunkDef::Width != 15)
	{
		// TODO: update neighbouring chunk
		UpdateLightAdjacent(a_IndicesToProcess, a_Item, a_Light, a_OriginatorIndex, a_OriginatorIndex + cChunkDef::Width);
	}

	if (Position.z % cChunkDef::Width != 0)
	{
		// TODO: update neighbouring chunk
		UpdateLightAdjacent(a_IndicesToProcess, a_Item, a_Light, a_OriginatorIndex, a_OriginatorIndex - cChunkDef::Width);
	}

	if (Position.y != cChunkDef::Height - 1)
	{
		UpdateLightAdjacent(a_IndicesToProcess, a_Item, a_Light, a_OriginatorIndex, a_OriginatorIndex + cChunkDef::Width * cChunkDef::Width);
	}

	if (Position.y != 0)
	{
		UpdateLightAdjacent(a_IndicesToProcess, a_Item, a_Light, a_OriginatorIndex, a_OriginatorIndex - cChunkDef::Width * cChunkDef::Width);
	}
}





void cLightingThread::UpdateLightAdjacent(std::vector<size_t> & a_IndicesToProcess, const cLightingChunkStay & a_Item, BlockDataArray & a_Light, size_t a_OriginatorIndex, size_t a_DestinationIndex)
{
	// Assertions for index validity occur within PropagateLightToAdjacent
	if (PropagateLightToAdjacent(a_Item, a_Light, a_OriginatorIndex, a_DestinationIndex))
	{
		a_IndicesToProcess.emplace_back(a_DestinationIndex);
	}
}





void cLightingThread::CompressLight(const BlockDataArray & a_LightArray, BlockNibbles & a_ChunkLight)
{
	for (size_t Index = 0; Index != a_ChunkLight.size(); ++Index)
	{
		auto AdjustedIndex = Index * 2;
		a_ChunkLight[Index] = a_LightArray[AdjustedIndex] | static_cast<NIBBLETYPE>(a_LightArray[AdjustedIndex + 1] << 4);
	}
}





////////////////////////////////////////////////////////////////////////////////
// cLightingThread::cLightingChunkStay:

cLightingThread::cLightingChunkStay::cLightingChunkStay(cLightingThread & a_LightingThread, int a_ChunkX, int a_ChunkZ, std::unique_ptr<cChunkCoordCallback> a_CallbackAfter) :
	m_CallbackAfter(std::move(a_CallbackAfter)),
	m_ChunkX(a_ChunkX),
	m_ChunkZ(a_ChunkZ),
	m_MaxHeight(0),  // Required by cReader to be initialised to zero
	m_LightingThread(a_LightingThread)
{
	Add(a_ChunkX, a_ChunkZ);
}





bool cLightingThread::cLightingChunkStay::OnAllChunksAvailable(void)
{
	if (m_LightingThread.m_ShouldTerminate)
	{
		return false;
	}

	ASSERT(m_LightingThread.m_World->IsChunkValid(m_ChunkX, m_ChunkZ));

	// If the chunk is already lit, skip it:
	if (m_LightingThread.m_World->IsChunkLighted(m_ChunkX, m_ChunkZ))
	{
		if (m_CallbackAfter != nullptr)
		{
			m_CallbackAfter->Call(m_ChunkX, m_ChunkZ, true);
		}


		{
			cCSLock Lock(m_LightingThread.m_CS);
			m_LightingThread.m_PendingQueue.erase(
				std::remove(m_LightingThread.m_PendingQueue.begin(), m_LightingThread.m_PendingQueue.end(), this),
				m_LightingThread.m_PendingQueue.end()
			);
		}

		return true;
	}

	{
		cCSLock Lock(m_LightingThread.m_CS);
		m_LightingThread.m_PendingQueue.erase(
			std::remove(m_LightingThread.m_PendingQueue.begin(), m_LightingThread.m_PendingQueue.end(), this),
			m_LightingThread.m_PendingQueue.end()
		);
		m_LightingThread.m_Queue.emplace_back(this);
	}

	m_LightingThread.m_evtItemDataLoaded.Set();

	// Keep the ChunkStay alive:
	return false;
}





void cLightingThread::cLightingChunkStay::OnDisabled(void)
{
	delete this;
}




