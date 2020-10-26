
// LightingThread.cpp

// Implements the cLightingThread class representing the thread that processes requests for lighting

#include "Globals.h"
#include "LightingThread.h"
#include "ChunkMap.h"
#include "World.h"
#include "BlockInfo.h"





/** Chunk data callback that takes the chunk data and puts them into cLightingThread's m_BlockTypes[] / m_HeightMap[]: */
class cReader :
	public cChunkDataCallback
{
	virtual void ChunkData(const cChunkData & a_ChunkBuffer) override
	{
		BLOCKTYPE * OutputRows = m_BlockTypes;
		int OutputIdx = m_ReadingChunkX + m_ReadingChunkZ * cChunkDef::Width * 3;
		for (size_t i = 0; i != cChunkData::NumSections; ++i)
		{
			auto * Section = a_ChunkBuffer.GetSection(i);
			if (Section == nullptr)
			{
				// Skip to the next section
				OutputIdx += 9 * cChunkData::SectionHeight * cChunkDef::Width;
				continue;
			}

			for (size_t OffsetY = 0; OffsetY != cChunkData::SectionHeight; ++OffsetY)
			{
				for (size_t Z = 0; Z != cChunkDef::Width; ++Z)
				{
					auto InPtr = Section->m_BlockTypes + Z * cChunkDef::Width + OffsetY * cChunkDef::Width * cChunkDef::Width;
					std::copy_n(InPtr, cChunkDef::Width, OutputRows + OutputIdx * cChunkDef::Width);

					OutputIdx += 3;
				}
				// Skip into the next y-level in the 3x3 chunk blob; each level has cChunkDef::Width * 9 rows
				// We've already walked cChunkDef::Width * 3 in the "for z" cycle, that makes cChunkDef::Width * 6 rows left to skip
				OutputIdx += cChunkDef::Width * 6;
			}
		}
	}  // BlockTypes()


	virtual void HeightMap(const cChunkDef::HeightMap * a_Heightmap) override
	{
		// Copy the entire heightmap, distribute it into the 3x3 chunk blob:
		typedef struct {HEIGHTTYPE m_Row[16]; } ROW;
		const ROW * InputRows  = reinterpret_cast<const ROW *>(a_Heightmap);
		ROW * OutputRows = reinterpret_cast<ROW *>(m_HeightMap);
		int InputIdx = 0;
		int OutputIdx = m_ReadingChunkX + m_ReadingChunkZ * cChunkDef::Width * 3;
		for (int z = 0; z < cChunkDef::Width; z++)
		{
			OutputRows[OutputIdx] = InputRows[InputIdx++];
			OutputIdx += 3;
		}  // for z

		// Find the highest block in the entire chunk, use it as a base for m_MaxHeight:
		HEIGHTTYPE MaxHeight = m_MaxHeight;
		for (size_t i = 0; i < ARRAYCOUNT(*a_Heightmap); i++)
		{
			if ((*a_Heightmap)[i] > MaxHeight)
			{
				MaxHeight = (*a_Heightmap)[i];
			}
		}
		m_MaxHeight = MaxHeight;
	}

public:
	int m_ReadingChunkX;  // 0, 1 or 2; x-offset of the chunk we're reading from the BlockTypes start
	int m_ReadingChunkZ;  // 0, 1 or 2; z-offset of the chunk we're reading from the BlockTypes start
	HEIGHTTYPE m_MaxHeight;  // Maximum value in this chunk's heightmap
	BLOCKTYPE * m_BlockTypes;  // 3x3 chunks of block types, organized as a single XZY blob of data (instead of 3x3 XZY blobs)
	HEIGHTTYPE * m_HeightMap;  // 3x3 chunks of height map,  organized as a single XZY blob of data (instead of 3x3 XZY blobs)

	cReader(BLOCKTYPE * a_BlockTypes, HEIGHTTYPE * a_HeightMap) :
		m_ReadingChunkX(0),
		m_ReadingChunkZ(0),
		m_MaxHeight(0),
		m_BlockTypes(a_BlockTypes),
		m_HeightMap(a_HeightMap)
	{
		std::fill_n(m_BlockTypes, cChunkDef::NumBlocks * 9, E_BLOCK_AIR);
	}
} ;





////////////////////////////////////////////////////////////////////////////////
// cLightingThread:

cLightingThread::cLightingThread(cWorld & a_World):
	Super("cLightingThread"),
	m_World(a_World),
	m_MaxHeight(0),
	m_NumSeeds(0)
{
}





cLightingThread::~cLightingThread()
{
	Stop();
}





void cLightingThread::Stop(void)
{
	{
		cCSLock Lock(m_CS);
		for (cChunkStays::iterator itr = m_PendingQueue.begin(), end = m_PendingQueue.end(); itr != end; ++itr)
		{
			(*itr)->Disable();
			delete *itr;
		}
		m_PendingQueue.clear();
		for (cChunkStays::iterator itr = m_Queue.begin(), end = m_Queue.end(); itr != end; ++itr)
		{
			(*itr)->Disable();
			delete *itr;
		}
		m_Queue.clear();
	}
	m_ShouldTerminate = true;
	m_evtItemAdded.Set();

	Super::Stop();
}





void cLightingThread::QueueChunk(int a_ChunkX, int a_ChunkZ, std::unique_ptr<cChunkCoordCallback> a_CallbackAfter)
{
	cChunkStay * ChunkStay = new cLightingChunkStay(*this, a_ChunkX, a_ChunkZ, std::move(a_CallbackAfter));
	{
		// The ChunkStay will enqueue itself using the QueueChunkStay() once it is fully loaded
		// In the meantime, put it into the PendingQueue so that it can be removed when stopping the thread
		cCSLock Lock(m_CS);
		m_PendingQueue.push_back(ChunkStay);
	}
	ChunkStay->Enable(*m_World.GetChunkMap());
}





void cLightingThread::WaitForQueueEmpty(void)
{
	cCSLock Lock(m_CS);
	while (!m_ShouldTerminate && (!m_Queue.empty() || !m_PendingQueue.empty()))
	{
		cCSUnlock Unlock(Lock);
		m_evtQueueEmpty.Wait();
	}
}





size_t cLightingThread::GetQueueLength(void)
{
	cCSLock Lock(m_CS);
	return m_Queue.size() + m_PendingQueue.size();
}





void cLightingThread::Execute(void)
{
	for (;;)
	{
		{
			cCSLock Lock(m_CS);
			if (m_Queue.empty())
			{
				cCSUnlock Unlock(Lock);
				m_evtItemAdded.Wait();
			}
		}

		if (m_ShouldTerminate)
		{
			return;
		}

		// Process one items from the queue:
		cLightingChunkStay * Item;
		{
			cCSLock Lock(m_CS);
			if (m_Queue.empty())
			{
				continue;
			}
			Item = static_cast<cLightingChunkStay *>(m_Queue.front());
			m_Queue.pop_front();
			if (m_Queue.empty())
			{
				m_evtQueueEmpty.Set();
			}
		}  // CSLock(m_CS)

		LightChunk(*Item);
		Item->Disable();
		delete Item;
	}
}





void cLightingThread::LightChunk(cLightingChunkStay & a_Item)
{
	// If the chunk is already lit, skip it (report as success):
	if (m_World.IsChunkLighted(a_Item.m_ChunkX, a_Item.m_ChunkZ))
	{
		if (a_Item.m_CallbackAfter != nullptr)
		{
			a_Item.m_CallbackAfter->Call({a_Item.m_ChunkX, a_Item.m_ChunkZ}, true);
		}
		return;
	}

	cChunkDef::BlockNibbles BlockLight, SkyLight;

	ReadChunks(a_Item.m_ChunkX, a_Item.m_ChunkZ);

	PrepareBlockLight();
	CalcLight(m_BlockLight);

	PrepareSkyLight();

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
				memcpy(Seeds, m_BlockTypes + y * BlocksPerYLayer + z * cChunkDef::Width * 3, cChunkDef::Width * 3);
				for (int x = 0; x < cChunkDef::Width * 3; x++)
				{
					if (m_IsSeed1[y * BlocksPerYLayer + z * cChunkDef::Width * 3 + x])
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

	CalcLight(m_SkyLight);

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
				f1.Write(m_BlockTypes + y * BlocksPerYLayer + z * cChunkDef::Width * 3, cChunkDef::Width * 3);
				unsigned char SkyLight  [cChunkDef::Width * 3];
				unsigned char BlockLight[cChunkDef::Width * 3];
				for (int x = 0; x < cChunkDef::Width * 3; x++)
				{
					SkyLight[x]   = m_SkyLight  [y * BlocksPerYLayer + z * cChunkDef::Width * 3 + x] << 4;
					BlockLight[x] = m_BlockLight[y * BlocksPerYLayer + z * cChunkDef::Width * 3 + x] << 4;
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

	CompressLight(m_BlockLight, BlockLight);
	CompressLight(m_SkyLight, SkyLight);

	m_World.ChunkLighted(a_Item.m_ChunkX, a_Item.m_ChunkZ, BlockLight, SkyLight);

	if (a_Item.m_CallbackAfter != nullptr)
	{
		a_Item.m_CallbackAfter->Call({a_Item.m_ChunkX, a_Item.m_ChunkZ}, true);
	}
}





void cLightingThread::ReadChunks(int a_ChunkX, int a_ChunkZ)
{
	cReader Reader(m_BlockTypes, m_HeightMap);

	for (int z = 0; z < 3; z++)
	{
		Reader.m_ReadingChunkZ = z;
		for (int x = 0; x < 3; x++)
		{
			Reader.m_ReadingChunkX = x;
			VERIFY(m_World.GetChunkData({a_ChunkX + x - 1, a_ChunkZ + z - 1}, Reader));
		}  // for z
	}  // for x

	memset(m_BlockLight, 0, sizeof(m_BlockLight));
	memset(m_SkyLight,   0, sizeof(m_SkyLight));
	m_MaxHeight = Reader.m_MaxHeight;
}





void cLightingThread::PrepareSkyLight(void)
{
	// Clear seeds:
	memset(m_IsSeed1, 0, sizeof(m_IsSeed1));
	m_NumSeeds = 0;

	// Fill the top of the chunk with all-light:
	if (m_MaxHeight < cChunkDef::Height - 1)
	{
		std::fill(m_SkyLight + (m_MaxHeight + 1) * BlocksPerYLayer, m_SkyLight + ARRAYCOUNT(m_SkyLight), 15);
	}

	// Walk every column that has all XZ neighbors
	for (int z = 1; z < cChunkDef::Width * 3 - 1; z++)
	{
		int BaseZ = z * cChunkDef::Width * 3;
		for (int x = 1; x < cChunkDef::Width * 3 - 1; x++)
		{
			int idx = BaseZ + x;
			// Find the lowest block in this column that receives full sunlight (go through transparent blocks):
			int Current = m_HeightMap[idx];
			ASSERT(Current < cChunkDef::Height);
			while (
				(Current >= 0) &&
				cBlockInfo::IsTransparent(m_BlockTypes[idx + Current * BlocksPerYLayer]) &&
				!cBlockInfo::IsSkylightDispersant(m_BlockTypes[idx + Current * BlocksPerYLayer])
			)
			{
				Current -= 1;  // Sunlight goes down unchanged through this block
			}
			Current += 1;  // Point to the last sunlit block, rather than the first non-transparent one
			// The other neighbors don't need transparent-block-checking. At worst we'll have a few dud seeds above the ground.
			int Neighbor1 = m_HeightMap[idx + 1] + 1;  // X + 1
			int Neighbor2 = m_HeightMap[idx - 1] + 1;  // X - 1
			int Neighbor3 = m_HeightMap[idx + cChunkDef::Width * 3] + 1;  // Z + 1
			int Neighbor4 = m_HeightMap[idx - cChunkDef::Width * 3] + 1;  // Z - 1
			int MaxNeighbor = std::max(std::max(Neighbor1, Neighbor2), std::max(Neighbor3, Neighbor4));  // Maximum of the four neighbors

			// Fill the column from m_MaxHeight to Current with all-light:
			for (int y = m_MaxHeight, Index = idx + y * BlocksPerYLayer; y >= Current; y--, Index -= BlocksPerYLayer)
			{
				m_SkyLight[Index] = 15;
			}

			// Add Current as a seed:
			if (Current < cChunkDef::Height)
			{
				int CurrentIdx = idx + Current * BlocksPerYLayer;
				m_IsSeed1[CurrentIdx] = true;
				m_SeedIdx1[m_NumSeeds++] = static_cast<UInt32>(CurrentIdx);
			}

			// Add seed from Current up to the highest neighbor:
			for (int y = Current + 1, Index = idx + y * BlocksPerYLayer; y < MaxNeighbor; y++, Index += BlocksPerYLayer)
			{
				m_IsSeed1[Index] = true;
				m_SeedIdx1[m_NumSeeds++] = static_cast<UInt32>(Index);
			}
		}
	}
}





void cLightingThread::PrepareBlockLight()
{
	// Clear seeds:
	memset(m_IsSeed1, 0, sizeof(m_IsSeed1));
	memset(m_IsSeed2, 0, sizeof(m_IsSeed2));
	m_NumSeeds = 0;

	// Add each emissive block into the seeds:
	for (int Idx = 0; Idx < (m_MaxHeight * BlocksPerYLayer); ++Idx)
	{
		if (cBlockInfo::GetLightValue(m_BlockTypes[Idx]) == 0)
		{
			// Not a light-emissive block
			continue;
		}

		// Add current block as a seed:
		m_IsSeed1[Idx] = true;
		m_SeedIdx1[m_NumSeeds++] = static_cast<UInt32>(Idx);

		// Light it up:
		m_BlockLight[Idx] = cBlockInfo::GetLightValue(m_BlockTypes[Idx]);
	}
}





void cLightingThread::CalcLight(NIBBLETYPE * a_Light)
{
	size_t NumSeeds2 = 0;
	while (m_NumSeeds > 0)
	{
		// Buffer 1 -> buffer 2
		memset(m_IsSeed2, 0, sizeof(m_IsSeed2));
		NumSeeds2 = 0;
		CalcLightStep(a_Light, m_NumSeeds, m_IsSeed1, m_SeedIdx1, NumSeeds2, m_IsSeed2, m_SeedIdx2);
		if (NumSeeds2 == 0)
		{
			return;
		}

		// Buffer 2 -> buffer 1
		memset(m_IsSeed1, 0, sizeof(m_IsSeed1));
		m_NumSeeds = 0;
		CalcLightStep(a_Light, NumSeeds2, m_IsSeed2, m_SeedIdx2, m_NumSeeds, m_IsSeed1, m_SeedIdx1);
	}
}





void cLightingThread::CalcLightStep(
	NIBBLETYPE * a_Light,
	size_t a_NumSeedsIn,    unsigned char * a_IsSeedIn,  unsigned int * a_SeedIdxIn,
	size_t & a_NumSeedsOut, unsigned char * a_IsSeedOut, unsigned int * a_SeedIdxOut
)
{
	UNUSED(a_IsSeedIn);
	size_t NumSeedsOut = 0;
	for (size_t i = 0; i < a_NumSeedsIn; i++)
	{
		UInt32 SeedIdx = static_cast<UInt32>(a_SeedIdxIn[i]);
		int SeedX = SeedIdx % (cChunkDef::Width * 3);
		int SeedZ = (SeedIdx / (cChunkDef::Width * 3)) % (cChunkDef::Width * 3);

		// Propagate seed:
		if (SeedX < cChunkDef::Width * 3 - 1)
		{
			PropagateLight(a_Light, SeedIdx, SeedIdx + 1, NumSeedsOut, a_IsSeedOut, a_SeedIdxOut);
		}
		if (SeedX > 0)
		{
			PropagateLight(a_Light, SeedIdx, SeedIdx - 1, NumSeedsOut, a_IsSeedOut, a_SeedIdxOut);
		}
		if (SeedZ < cChunkDef::Width * 3 - 1)
		{
			PropagateLight(a_Light, SeedIdx, SeedIdx + cChunkDef::Width * 3, NumSeedsOut, a_IsSeedOut, a_SeedIdxOut);
		}
		if (SeedZ > 0)
		{
			PropagateLight(a_Light, SeedIdx, SeedIdx - cChunkDef::Width * 3, NumSeedsOut, a_IsSeedOut, a_SeedIdxOut);
		}
		if (SeedIdx < (cChunkDef::Height - 1) * BlocksPerYLayer)
		{
			PropagateLight(a_Light, SeedIdx, SeedIdx + BlocksPerYLayer, NumSeedsOut, a_IsSeedOut, a_SeedIdxOut);
		}
		if (SeedIdx >= BlocksPerYLayer)
		{
			PropagateLight(a_Light, SeedIdx, SeedIdx - BlocksPerYLayer, NumSeedsOut, a_IsSeedOut, a_SeedIdxOut);
		}
	}  // for i - a_SeedIdxIn[]
	a_NumSeedsOut = NumSeedsOut;
}





void cLightingThread::CompressLight(NIBBLETYPE * a_LightArray, NIBBLETYPE * a_ChunkLight)
{
	int InIdx = cChunkDef::Width * 49;  // Index to the first nibble of the middle chunk in the a_LightArray
	int OutIdx = 0;
	for (int y = 0; y < cChunkDef::Height; y++)
	{
		for (int z = 0; z < cChunkDef::Width; z++)
		{
			for (int x = 0; x < cChunkDef::Width; x += 2)
			{
				a_ChunkLight[OutIdx++] = static_cast<NIBBLETYPE>(a_LightArray[InIdx + 1] << 4) | a_LightArray[InIdx];
				InIdx += 2;
			}
			InIdx += cChunkDef::Width * 2;
		}
		// Skip into the next y-level in the 3x3 chunk blob; each level has cChunkDef::Width * 9 rows
		// We've already walked cChunkDef::Width * 3 in the "for z" cycle, that makes cChunkDef::Width * 6 rows left to skip
		InIdx += cChunkDef::Width * cChunkDef::Width * 6;
	}
}





void cLightingThread::PropagateLight(
	NIBBLETYPE * a_Light,
	unsigned int a_SrcIdx, unsigned int a_DstIdx,
	size_t & a_NumSeedsOut, unsigned char * a_IsSeedOut, unsigned int * a_SeedIdxOut
)
{
	ASSERT(a_SrcIdx < ARRAYCOUNT(m_SkyLight));
	ASSERT(a_DstIdx < ARRAYCOUNT(m_BlockTypes));

	if (a_Light[a_SrcIdx] <= a_Light[a_DstIdx] + cBlockInfo::GetSpreadLightFalloff(m_BlockTypes[a_DstIdx]))
	{
		// We're not offering more light than the dest block already has
		return;
	}

	a_Light[a_DstIdx] = a_Light[a_SrcIdx] - cBlockInfo::GetSpreadLightFalloff(m_BlockTypes[a_DstIdx]);
	if (!a_IsSeedOut[a_DstIdx])
	{
		a_IsSeedOut[a_DstIdx] = true;
		a_SeedIdxOut[a_NumSeedsOut++] = a_DstIdx;
	}
}





void cLightingThread::QueueChunkStay(cLightingChunkStay & a_ChunkStay)
{
	// Move the ChunkStay from the Pending queue to the lighting queue.
	{
		cCSLock Lock(m_CS);
		m_PendingQueue.remove(&a_ChunkStay);
		m_Queue.push_back(&a_ChunkStay);
	}
	m_evtItemAdded.Set();
}





////////////////////////////////////////////////////////////////////////////////
// cLightingThread::cLightingChunkStay:

cLightingThread::cLightingChunkStay::cLightingChunkStay(cLightingThread & a_LightingThread, int a_ChunkX, int a_ChunkZ, std::unique_ptr<cChunkCoordCallback> a_CallbackAfter) :
	m_LightingThread(a_LightingThread),
	m_ChunkX(a_ChunkX),
	m_ChunkZ(a_ChunkZ),
	m_CallbackAfter(std::move(a_CallbackAfter))
{
	Add(a_ChunkX + 1, a_ChunkZ + 1);
	Add(a_ChunkX + 1, a_ChunkZ);
	Add(a_ChunkX + 1, a_ChunkZ - 1);
	Add(a_ChunkX,     a_ChunkZ + 1);
	Add(a_ChunkX,     a_ChunkZ);
	Add(a_ChunkX,     a_ChunkZ - 1);
	Add(a_ChunkX - 1, a_ChunkZ + 1);
	Add(a_ChunkX - 1, a_ChunkZ);
	Add(a_ChunkX - 1, a_ChunkZ - 1);
}





bool cLightingThread::cLightingChunkStay::OnAllChunksAvailable(void)
{
	m_LightingThread.QueueChunkStay(*this);

	// Keep the ChunkStay alive:
	return false;
}





void cLightingThread::cLightingChunkStay::OnDisabled(void)
{
	// Nothing needed in this callback
}




