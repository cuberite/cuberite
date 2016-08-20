
// LightingThread.h

// Interfaces to the cLightingThread class representing the thread that processes requests for lighting

/*
Lighting is done on whole chunks. For each chunk to be lighted, the whole 3x3 chunk area around it is read,
then it is processed, so that the middle chunk area has valid lighting, and the lighting is copied into the ChunkMap.
Lighting is calculated in full char arrays instead of nibbles, so that accessing the arrays is fast.
Lighting is calculated in a flood-fill fashion:
1. Generate seeds from where the light spreads (full skylight / light-emitting blocks)
2. For each seed:
	- Spread the light 1 block in each of the 6 cardinal directions, if the blocktype allows
	- If the recipient block has had lower lighting value than that being spread, make it a new seed
3. Repeat step 2, until there are no more seeds
The seeds need two fast operations:
	- Check if a block at [x, y, z] is already a seed
	- Get the next seed in the row
For that reason it is stored in two arrays, one stores a bool saying a seed is in that position,
the other is an array of seed coords, encoded as a single int.
Step 2 needs two separate storages for old seeds and new seeds, so there are two actual storages for that purpose,
their content is swapped after each full step-2-cycle.

The thread has two queues of chunks that are to be lighted.
The first queue, m_Queue, is the only one that is publicly visible, chunks get queued there by external requests.
The second one, m_PostponedQueue, is for chunks that have been taken out of m_Queue and didn't have neighbors ready.
Chunks from m_PostponedQueue are moved back into m_Queue when their neighbors get valid, using the ChunkReady callback.
*/



#pragma once

#include "OSSupport/IsThread.h"
#include "ChunkDef.h"
#include "ChunkStay.h"





// fwd: "cWorld.h"
class cWorld;





class cLightingThread :
	public cIsThread
{
	typedef cIsThread super;

protected:

	friend class cReader;

	typedef std::array<BLOCKTYPE, cChunkDef::NumBlocks> BlockDataArray;
	typedef std::array<HEIGHTTYPE, cChunkDef::Width * cChunkDef::Width> HeightDataArray;

	class cLightingChunkStay :
		public cChunkStay
	{
	public:
		std::unique_ptr<cChunkCoordCallback> m_CallbackAfter;

		int m_ChunkX;
		int m_ChunkZ;

		/** The highest block in the current chunk data */
		HEIGHTTYPE m_MaxHeight;

		std::unique_ptr<BlockDataArray> m_BlockTypes;
		std::unique_ptr<HeightDataArray> m_HeightMap;

		cLightingChunkStay(cLightingThread & a_LightingThread, int a_ChunkX, int a_ChunkZ, std::unique_ptr<cChunkCoordCallback> a_CallbackAfter);

	protected:
		virtual void OnChunkAvailable(int a_ChunkX, int a_ChunkZ) override
		{
			UNUSED(a_ChunkX);
			UNUSED(a_ChunkZ);
		}
		virtual bool OnAllChunksAvailable(void) override;
		virtual void OnDisabled(void) override;

	private:
		cLightingThread & m_LightingThread;
	};

	typedef std::vector<cLightingChunkStay *> cChunkStays;
	typedef std::array<NIBBLETYPE, cChunkDef::NumBlocks / 2> BlockNibbles;

	cWorld * m_World;

	/** The mutex to protect m_Queue and m_PendingQueue */
	cCriticalSection m_CS;

	/** The ChunkStays that are waiting for load. Used for stopping the thread. */
	cChunkStays m_PendingQueue;

	/** The ChunkStays that are loaded. */
	cChunkStays m_Queue;

	cEvent m_evtItemDataLoaded;

	virtual void Execute(void) override;

	/** Lights the entire chunk. If neighbor chunks don't exist, touches them and re-queues the chunk */
	void LightChunk(cLightingChunkStay & a_Item);

	/** Uses m_HeightMap to initialize the m_SkyLight[] data; fills in seeds for the skylight */
	std::vector<size_t> PrepareSkyLight(const cLightingChunkStay & a_Item, BlockDataArray & a_SkyLight);

	/** Uses m_BlockTypes to initialize the m_BlockLight[] data; fills in seeds for the blocklight */
	std::vector<size_t> PrepareBlockLight(const cLightingChunkStay & a_Item, BlockDataArray & a_BlockLight);

	/** Calculates light in the light array specified, using stored seeds */
	void CalcLight(const cLightingChunkStay & a_Item, std::vector<size_t> a_IndicesToProcess, BlockDataArray & a_Light);

	void DiscoverLightAdjacents(std::vector<size_t> & a_IndicesToProcess, const cLightingChunkStay & a_Item, BlockDataArray & a_Light, size_t a_OriginatorIndex);

	/** Does one step in the light calculation - one seed propagation and seed recalculation */
	void UpdateLightAdjacent(std::vector<size_t> & a_IndicesToProcess, const cLightingChunkStay & a_Item, BlockDataArray & a_Light, size_t a_OriginatorIndex, size_t a_DestinationIndex);

	inline bool PropagateLightToAdjacent(const cLightingChunkStay & a_Item, BlockDataArray & a_Light, size_t a_OriginatorIndex, size_t a_DestinationIndex)
	{
		ASSERT(a_OriginatorIndex < a_Light.size());
		ASSERT(a_DestinationIndex < a_Light.size());

		auto Falloff = cBlockInfo::GetSpreadLightFalloff((*a_Item.m_BlockTypes)[a_DestinationIndex]);
		if (a_Light[a_OriginatorIndex] <= a_Light[a_DestinationIndex] + Falloff)
		{
			// We're not offering more light than the dest block already has
			return false;
		}

		a_Light[a_DestinationIndex] = a_Light[a_OriginatorIndex] - Falloff;
		return true;
	}

	/** Compresses from 1-block-per-byte (faster calc) into 2-blocks-per-byte (MC storage): */
	static void CompressLight(const BlockDataArray & a_LightArray, BlockNibbles & a_ChunkLight);

public:

	cLightingThread(void);
	~cLightingThread();

	bool Start(cWorld * a_World);

	void Stop(void);

	/** Queues the entire chunk for lighting
	Takes a 3-by-3 chunks' worth of buffered data (blocktypes and heightmap) centered around the chunk to light
	The callback, if specified, is called after the lighting has been processed. */
	void QueueChunk(int a_ChunkX, int a_ChunkZ, std::unique_ptr<cChunkCoordCallback> a_CallbackAfter);

	size_t GetQueueLength(void);

} ;




