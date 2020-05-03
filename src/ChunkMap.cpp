
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "ChunkMap.h"
#include "BlockInfo.h"
#include "World.h"
#include "Root.h"
#include "Entities/Player.h"
#include "Item.h"
#include "Chunk.h"
#include "Generating/Trees.h"  // used in cChunkMap::ReplaceTreeBlocks() for tree block discrimination
#include "BlockArea.h"
#include "Bindings/PluginManager.h"
#include "Entities/TNTEntity.h"
#include "Blocks/BlockHandler.h"
#include "MobCensus.h"
#include "MobSpawner.h"
#include "BoundingBox.h"
#include "SetChunkData.h"
#include "Blocks/ChunkInterface.h"
#include "Entities/Pickup.h"
#include "DeadlockDetect.h"
#include "BlockEntities/BlockEntity.h"

#ifndef _WIN32
	#include <cstdlib>  // abs
#endif

#include "zlib/zlib.h"
#include "json/json.h"





////////////////////////////////////////////////////////////////////////////////
// cChunkMap:

cChunkMap::cChunkMap(cWorld * a_World) :
	m_World(a_World),
	m_Pool(
		cpp14::make_unique<cListAllocationPool<cChunkData::sChunkSection>>(
			cpp14::make_unique<cStarvationCallbacks>(), 1600u, 5000u
		)
	)
{
}





cChunkMap::~cChunkMap()
{
	// Explicitly destroy all chunks, so that they're guaranteed to be
	// destroyed before other internals. This fixes crashes on stopping the server.
	// because the chunk destructor deletes entities and those may access the chunkmap.
	// Also, the cChunkData destructor accesses the chunkMap's allocator.
	m_Chunks.clear();
}





cChunkPtr cChunkMap::ConstructChunk(int a_ChunkX, int a_ChunkZ)
{
	auto Chunk = FindChunk(a_ChunkX, a_ChunkZ);
	if (Chunk == nullptr)
	{
		return (
			*m_Chunks.emplace(
				ChunkCoordinate{ a_ChunkX, a_ChunkZ },
				cpp14::make_unique<cChunk>(
					a_ChunkX,
					a_ChunkZ,
					this,
					GetWorld(),
					FindChunk(a_ChunkX - 1, a_ChunkZ),
					FindChunk(a_ChunkX + 1, a_ChunkZ),
					FindChunk(a_ChunkX, a_ChunkZ - 1),
					FindChunk(a_ChunkX, a_ChunkZ + 1),
					*m_Pool
				)
			).first
		).second.get();
	}
	return Chunk;
}





cChunkPtr cChunkMap::GetChunk(int a_ChunkX, int a_ChunkZ)
{
	ASSERT(m_CSChunks.IsLockedByCurrentThread());  // m_CSChunks should already be locked by the operation that called us

	auto Chunk = ConstructChunk(a_ChunkX, a_ChunkZ);
	if (Chunk == nullptr)
	{
		return nullptr;
	}
	if (!Chunk->IsValid() && !Chunk->IsQueued())
	{
		Chunk->SetPresence(cChunk::cpQueued);
		Chunk->SetShouldGenerateIfLoadFailed(true);
		m_World->GetStorage().QueueLoadChunk(a_ChunkX, a_ChunkZ);
	}
	return Chunk;
}





cChunkPtr cChunkMap::GetChunkNoGen(cChunkCoords a_Chunk)
{
	ASSERT(m_CSChunks.IsLockedByCurrentThread());  // m_CSChunks should already be locked by the operation that called us

	auto Chunk = ConstructChunk(a_Chunk.m_ChunkX, a_Chunk.m_ChunkZ);
	if (Chunk == nullptr)
	{
		return nullptr;
	}
	if (!Chunk->IsValid() && !Chunk->IsQueued())
	{
		Chunk->SetPresence(cChunk::cpQueued);
		m_World->GetStorage().QueueLoadChunk(a_Chunk.m_ChunkX, a_Chunk.m_ChunkZ);
	}

	return Chunk;
}





cChunkPtr cChunkMap::GetChunkNoLoad(cChunkCoords a_Coords)
{
	ASSERT(m_CSChunks.IsLockedByCurrentThread());  // m_CSChunks should already be locked by the operation that called us
	return ConstructChunk(a_Coords.m_ChunkX, a_Coords.m_ChunkZ);
}





cChunk * cChunkMap::FindChunk(int a_ChunkX, int a_ChunkZ)
{
	ASSERT(m_CSChunks.IsLockedByCurrentThread());

	auto Chunk = m_Chunks.find({ a_ChunkX, a_ChunkZ });
	return (Chunk == m_Chunks.end()) ? nullptr : Chunk->second.get();
}





void cChunkMap::SendBlockEntity(int a_BlockX, int a_BlockY, int a_BlockZ, cClientHandle & a_Client)
{
	cCSLock Lock(m_CSChunks);
	int ChunkX, ChunkZ;
	cChunkDef::BlockToChunk(a_BlockX, a_BlockZ, ChunkX, ChunkZ);
	cChunkPtr Chunk = GetChunkNoGen(ChunkX, ChunkZ);
	if ((Chunk == nullptr) || !Chunk->IsValid())
	{
		return;
	}
	Chunk->SendBlockEntity(a_BlockX, a_BlockY, a_BlockZ, a_Client);
}





bool cChunkMap::UseBlockEntity(cPlayer * a_Player, int a_BlockX, int a_BlockY, int a_BlockZ)
{
	// a_Player rclked block entity at the coords specified, handle it
	cCSLock Lock(m_CSChunks);
	int ChunkX, ChunkZ;
	cChunkDef::BlockToChunk(a_BlockX, a_BlockZ, ChunkX, ChunkZ);
	cChunkPtr Chunk = GetChunkNoGen(ChunkX, ChunkZ);
	if ((Chunk == nullptr) || !Chunk->IsValid())
	{
		return false;
	}
	return Chunk->UseBlockEntity(a_Player, a_BlockX, a_BlockY, a_BlockZ);
}





bool cChunkMap::DoWithChunk(int a_ChunkX, int a_ChunkZ, cChunkCallback a_Callback)
{
	cCSLock Lock(m_CSChunks);
	cChunkPtr Chunk = GetChunkNoLoad(a_ChunkX, a_ChunkZ);
	if (Chunk == nullptr)
	{
		return false;
	}
	return a_Callback(*Chunk);
}





bool cChunkMap::DoWithChunkAt(Vector3i a_BlockPos, cChunkCallback a_Callback)
{
	int ChunkX, ChunkZ;
	cChunkDef::BlockToChunk(a_BlockPos.x, a_BlockPos.z, ChunkX, ChunkZ);
	return DoWithChunk(ChunkX, ChunkZ, a_Callback);
}





void cChunkMap::WakeUpSimulators(Vector3i a_Block)
{
	cCSLock Lock(m_CSChunks);
	cChunkPtr Chunk = GetChunkNoGen(cChunkDef::BlockToChunk(a_Block));
	if ((Chunk == nullptr) || !Chunk->IsValid())
	{
		return;
	}
	m_World->GetSimulatorManager()->WakeUp(a_Block, Chunk);
}





void cChunkMap::MarkChunkDirty(int a_ChunkX, int a_ChunkZ)
{
	cCSLock Lock(m_CSChunks);
	cChunkPtr Chunk = GetChunkNoGen(a_ChunkX, a_ChunkZ);
	if ((Chunk == nullptr) || !Chunk->IsValid())
	{
		return;
	}
	Chunk->MarkDirty();
}





void cChunkMap::MarkChunkSaving(int a_ChunkX, int a_ChunkZ)
{
	cCSLock Lock(m_CSChunks);
	cChunkPtr Chunk = GetChunkNoGen(a_ChunkX, a_ChunkZ);
	if ((Chunk == nullptr) || !Chunk->IsValid())
	{
		return;
	}
	Chunk->MarkSaving();
}





void cChunkMap::MarkChunkSaved (int a_ChunkX, int a_ChunkZ)
{
	cCSLock Lock(m_CSChunks);
	cChunkPtr Chunk = GetChunkNoGen(a_ChunkX, a_ChunkZ);
	if ((Chunk == nullptr) || !Chunk->IsValid())
	{
		return;
	}
	Chunk->MarkSaved();
}





void cChunkMap::SetChunkData(cSetChunkData & a_SetChunkData)
{
	int ChunkX = a_SetChunkData.GetChunkX();
	int ChunkZ = a_SetChunkData.GetChunkZ();
	{
		cCSLock Lock(m_CSChunks);
		cChunkPtr Chunk = GetChunkNoLoad(ChunkX, ChunkZ);
		if (Chunk == nullptr)
		{
			return;
		}
		Chunk->SetAllData(a_SetChunkData);

		if (a_SetChunkData.ShouldMarkDirty())
		{
			Chunk->MarkDirty();
		}

		// Notify relevant ChunkStays:
		cChunkStays ToBeDisabled;
		for (cChunkStays::iterator itr = m_ChunkStays.begin(), end = m_ChunkStays.end(); itr != end; ++itr)
		{
			if ((*itr)->ChunkAvailable(ChunkX, ChunkZ))
			{
				// The chunkstay wants to be disabled, add it to a list of to-be-disabled chunkstays for later processing:
				ToBeDisabled.push_back(*itr);
			}
		}  // for itr - m_ChunkStays[]

		// Disable (and possibly remove) the chunkstays that chose to get disabled:
		for (cChunkStays::iterator itr = ToBeDisabled.begin(), end = ToBeDisabled.end(); itr != end; ++itr)
		{
			(*itr)->Disable();
		}
	}

	// Notify plugins of the chunk becoming available
	cPluginManager::Get()->CallHookChunkAvailable(*m_World, ChunkX, ChunkZ);
}





void cChunkMap::ChunkLighted(
	int a_ChunkX, int a_ChunkZ,
	const cChunkDef::BlockNibbles & a_BlockLight,
	const cChunkDef::BlockNibbles & a_SkyLight
)
{
	cCSLock Lock(m_CSChunks);
	cChunkPtr Chunk = GetChunkNoLoad(a_ChunkX, a_ChunkZ);
	if (Chunk == nullptr)
	{
		return;
	}
	Chunk->SetLight(a_BlockLight, a_SkyLight);
	Chunk->MarkDirty();
}





bool cChunkMap::GetChunkData(cChunkCoords a_Coords, cChunkDataCallback & a_Callback)
{
	if (!a_Callback.Coords(a_Coords.m_ChunkX, a_Coords.m_ChunkZ))
	{
		// The callback doesn't want the data
		return false;
	}
	cCSLock Lock(m_CSChunks);
	cChunkPtr Chunk = GetChunkNoGen(a_Coords);
	if ((Chunk == nullptr) || !Chunk->IsValid())
	{
		// The chunk is not present
		return false;
	}
	Chunk->GetAllData(a_Callback);
	return true;
}





bool cChunkMap::GetChunkBlockTypes(int a_ChunkX, int a_ChunkZ, BLOCKTYPE * a_BlockTypes)
{
	cCSLock Lock(m_CSChunks);
	cChunkPtr Chunk = GetChunkNoGen(a_ChunkX, a_ChunkZ);
	if ((Chunk == nullptr) || !Chunk->IsValid())
	{
		return false;
	}
	Chunk->GetBlockTypes(a_BlockTypes);
	return true;
}





bool cChunkMap::IsChunkQueued(int a_ChunkX, int a_ChunkZ)
{
	cCSLock Lock(m_CSChunks);
	cChunkPtr Chunk = GetChunkNoLoad(a_ChunkX, a_ChunkZ);
	return (Chunk != nullptr) && Chunk->IsQueued();
}





bool cChunkMap::IsChunkValid(int a_ChunkX, int a_ChunkZ)
{
	cCSLock Lock(m_CSChunks);
	cChunkPtr Chunk = GetChunkNoLoad(a_ChunkX, a_ChunkZ);
	return (Chunk != nullptr) && Chunk->IsValid();
}





bool cChunkMap::HasChunkAnyClients(int a_ChunkX, int a_ChunkZ)
{
	cCSLock Lock(m_CSChunks);
	cChunkPtr Chunk = GetChunkNoGen(a_ChunkX, a_ChunkZ);
	return (Chunk != nullptr) && Chunk->HasAnyClients();
}





int  cChunkMap::GetHeight(int a_BlockX, int a_BlockZ)
{
	for (;;)
	{
		cCSLock Lock(m_CSChunks);
		int ChunkX, ChunkZ, BlockY = 0;
		cChunkDef::AbsoluteToRelative(a_BlockX, BlockY, a_BlockZ, ChunkX, ChunkZ);
		cChunkPtr Chunk = GetChunk(ChunkX, ChunkZ);
		if (Chunk == nullptr)
		{
			return 0;
		}

		if (Chunk->IsValid())
		{
			return Chunk->GetHeight(a_BlockX, a_BlockZ);
		}

		// The chunk is not valid, wait for it to become valid:
		cCSUnlock Unlock(Lock);
		m_evtChunkValid.Wait();
	}  // while (true)
}





bool cChunkMap::TryGetHeight(int a_BlockX, int a_BlockZ, int & a_Height)
{
	// Returns false if chunk not loaded / generated
	cCSLock Lock(m_CSChunks);
	int ChunkX, ChunkZ, BlockY = 0;
	cChunkDef::AbsoluteToRelative(a_BlockX, BlockY, a_BlockZ, ChunkX, ChunkZ);
	cChunkPtr Chunk = GetChunkNoLoad(ChunkX, ChunkZ);
	if ((Chunk == nullptr) || !Chunk->IsValid())
	{
		return false;
	}
	a_Height = Chunk->GetHeight(a_BlockX, a_BlockZ);
	return true;
}





void cChunkMap::FastSetBlock(Vector3i a_BlockPos, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta)
{
	auto chunkPos = cChunkDef::BlockToChunk(a_BlockPos);
	auto relPos = cChunkDef::AbsoluteToRelative(a_BlockPos, chunkPos);

	cCSLock Lock(m_CSChunks);
	auto chunk = GetChunk(chunkPos.m_ChunkX, chunkPos.m_ChunkZ);
	if ((chunk != nullptr) && chunk->IsValid())
	{
		chunk->FastSetBlock(relPos, a_BlockType, a_BlockMeta);
	}
}





void cChunkMap::SetBlocks(const sSetBlockVector & a_Blocks)
{
	cCSLock lock(m_CSChunks);
	cChunkPtr chunk = nullptr;
	int lastChunkX = 0x7fffffff;  // Bogus coords so that chunk is updated on first pass
	int lastChunkZ = 0x7fffffff;
	for (auto block: a_Blocks)
	{
		// Update the chunk, if different from last time:
		if ((block.m_ChunkX != lastChunkX) || (block.m_ChunkZ != lastChunkZ))
		{
			lastChunkX = block.m_ChunkX;
			lastChunkZ = block.m_ChunkZ;
			chunk = GetChunk(lastChunkX, lastChunkZ);
		}

		// If the chunk is valid, set the block:
		if (chunk != nullptr)
		{
			chunk->SetBlock({block.m_RelX, block.m_RelY, block.m_RelZ}, block.m_BlockType, block.m_BlockMeta);
		}
	}  // for block - a_Blocks[]
}





void cChunkMap::CollectPickupsByPlayer(cPlayer & a_Player)
{
	int BlockX = static_cast<int>(a_Player.GetPosX());  // Truncating doesn't matter much; we're scanning entire chunks anyway
	int BlockY = static_cast<int>(a_Player.GetPosY());
	int BlockZ = static_cast<int>(a_Player.GetPosZ());
	int ChunkX = 0, ChunkZ = 0;
	cChunkDef::AbsoluteToRelative(BlockX, BlockY, BlockZ, ChunkX, ChunkZ);
	int OtherChunkX = ChunkX + ((BlockX > 8) ? 1 : -1);
	int OtherChunkZ = ChunkZ + ((BlockZ > 8) ? 1 : -1);

	// We suppose that each player keeps their chunks in memory, therefore it makes little sense to try to re-load or even generate them.
	// The only time the chunks are not valid is when the player is downloading the initial world and they should not call this at that moment

	cCSLock Lock(m_CSChunks);
	GetChunkNoLoad(ChunkX, ChunkZ)->CollectPickupsByPlayer(a_Player);

	// Check the neighboring chunks as well:
	GetChunkNoLoad(OtherChunkX, ChunkZ)->CollectPickupsByPlayer     (a_Player);
	GetChunkNoLoad(OtherChunkX, OtherChunkZ)->CollectPickupsByPlayer(a_Player);
	GetChunkNoLoad(ChunkX,      ChunkZ)->CollectPickupsByPlayer     (a_Player);
	GetChunkNoLoad(ChunkX,      OtherChunkZ)->CollectPickupsByPlayer(a_Player);
}





BLOCKTYPE cChunkMap::GetBlock(Vector3i a_BlockPos)
{
	auto chunkPos = cChunkDef::BlockToChunk(a_BlockPos);
	auto relPos = cChunkDef::AbsoluteToRelative(a_BlockPos, chunkPos);

	// Query the chunk, if loaded:
	cCSLock Lock(m_CSChunks);
	auto chunk = GetChunk(chunkPos.m_ChunkX, chunkPos.m_ChunkZ);
	if ((chunk != nullptr) && chunk->IsValid())
	{
		return chunk->GetBlock(relPos);
	}
	return 0;
}





NIBBLETYPE cChunkMap::GetBlockMeta(Vector3i a_BlockPos)
{
	auto chunkPos = cChunkDef::BlockToChunk(a_BlockPos);
	auto relPos = cChunkDef::AbsoluteToRelative(a_BlockPos, chunkPos);

	// Query the chunk, if loaded:
	cCSLock Lock(m_CSChunks);
	auto chunk = GetChunk(chunkPos.m_ChunkX, chunkPos.m_ChunkZ);
	if ((chunk != nullptr) && chunk->IsValid())
	{
		return chunk->GetMeta(relPos);
	}
	return 0;
}





NIBBLETYPE cChunkMap::GetBlockSkyLight(Vector3i a_BlockPos)
{
	auto chunkPos = cChunkDef::BlockToChunk(a_BlockPos);
	auto relPos = cChunkDef::AbsoluteToRelative(a_BlockPos, chunkPos);

	// Query the chunk, if loaded:
	cCSLock Lock(m_CSChunks);
	auto chunk = GetChunk(chunkPos.m_ChunkX, chunkPos.m_ChunkZ);
	if ((chunk != nullptr) && chunk->IsValid())
	{
		return chunk->GetSkyLight(relPos);
	}
	return 0;
}





NIBBLETYPE cChunkMap::GetBlockBlockLight(Vector3i a_BlockPos)
{
	auto chunkPos = cChunkDef::BlockToChunk(a_BlockPos);
	auto relPos = cChunkDef::AbsoluteToRelative(a_BlockPos, chunkPos);

	// Query the chunk, if loaded:
	cCSLock Lock(m_CSChunks);
	auto chunk = GetChunk(chunkPos.m_ChunkX, chunkPos.m_ChunkZ);
	if ((chunk != nullptr) && chunk->IsValid())
	{
		return chunk->GetBlockLight(relPos);
	}
	return 0;
}





void cChunkMap::SetBlockMeta(Vector3i a_BlockPos, NIBBLETYPE a_BlockMeta, bool a_ShouldMarkDirty, bool a_ShouldInformClients)
{
	auto chunkPos = cChunkDef::BlockToChunk(a_BlockPos);
	auto relPos = cChunkDef::AbsoluteToRelative(a_BlockPos, chunkPos);

	// Query the chunk, if loaded:
	cCSLock Lock(m_CSChunks);
	auto chunk = GetChunk(chunkPos.m_ChunkX, chunkPos.m_ChunkZ);
	if ((chunk != nullptr) && chunk->IsValid())
	{
		chunk->SetMeta(relPos, a_BlockMeta, a_ShouldMarkDirty, a_ShouldInformClients);
	}
}





void cChunkMap::SetBlock(Vector3i a_BlockPos, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta)
{
	auto chunkPos = cChunkDef::BlockToChunk(a_BlockPos);
	auto relPos = cChunkDef::AbsoluteToRelative(a_BlockPos, chunkPos);

	cCSLock Lock(m_CSChunks);
	auto chunk = GetChunk(chunkPos.m_ChunkX, chunkPos.m_ChunkZ);
	if ((chunk != nullptr) && chunk->IsValid())
	{
		BLOCKTYPE blockType;
		NIBBLETYPE blockMeta;
		GetBlockTypeMeta(a_BlockPos, blockType, blockMeta);
		cChunkInterface ChunkInterface(this);

		BlockHandler(blockType)->OnBroken(ChunkInterface, *m_World, a_BlockPos, blockType, blockMeta);

		chunk->SetBlock(relPos, a_BlockType, a_BlockMeta);
		m_World->GetSimulatorManager()->WakeUp(a_BlockPos, chunk);
		BlockHandler(a_BlockType)->OnPlaced(ChunkInterface, *m_World, a_BlockPos, a_BlockType, a_BlockMeta);
	}
}





bool cChunkMap::GetBlockTypeMeta(Vector3i a_BlockPos, BLOCKTYPE & a_BlockType, NIBBLETYPE & a_BlockMeta)
{
	auto chunkCoord = cChunkDef::BlockToChunk(a_BlockPos);
	auto relPos = cChunkDef::AbsoluteToRelative(a_BlockPos, chunkCoord);

	cCSLock Lock(m_CSChunks);
	auto chunk = GetChunk(chunkCoord.m_ChunkX, chunkCoord.m_ChunkZ);
	if ((chunk != nullptr) && chunk->IsValid())
	{
		chunk->GetBlockTypeMeta(relPos, a_BlockType, a_BlockMeta);
		return true;
	}
	return false;
}





bool cChunkMap::GetBlockInfo(Vector3i a_BlockPos, BLOCKTYPE & a_BlockType, NIBBLETYPE & a_Meta, NIBBLETYPE & a_SkyLight, NIBBLETYPE & a_BlockLight)
{
	auto chunkPos = cChunkDef::BlockToChunk(a_BlockPos);
	auto relPos = cChunkDef::AbsoluteToRelative(a_BlockPos, chunkPos);

	// Query the chunk, if loaded:
	cCSLock Lock(m_CSChunks);
	auto chunk = GetChunk(chunkPos.m_ChunkX, chunkPos.m_ChunkZ);
	if ((chunk != nullptr) && chunk->IsValid())
	{
		chunk->GetBlockInfo(relPos, a_BlockType, a_Meta, a_SkyLight, a_BlockLight);
		return true;
	}
	return false;
}





void cChunkMap::ReplaceBlocks(const sSetBlockVector & a_Blocks, BLOCKTYPE a_FilterBlockType)
{
	cCSLock Lock(m_CSChunks);
	for (sSetBlockVector::const_iterator itr = a_Blocks.begin(); itr != a_Blocks.end(); ++itr)
	{
		auto chunk = GetChunk(itr->m_ChunkX, itr->m_ChunkZ);
		if ((chunk == nullptr) || !chunk->IsValid())
		{
			continue;
		}
		Vector3i relPos(itr->m_RelX, itr->m_RelY, itr->m_RelZ);
		if (chunk->GetBlock(relPos) == a_FilterBlockType)
		{
			chunk->SetBlock(relPos, itr->m_BlockType, itr->m_BlockMeta);
		}
	}
}





void cChunkMap::ReplaceTreeBlocks(const sSetBlockVector & a_Blocks)
{
	cCSLock Lock(m_CSChunks);
	for (sSetBlockVector::const_iterator itr = a_Blocks.begin(); itr != a_Blocks.end(); ++itr)
	{
		auto chunk = GetChunk(itr->m_ChunkX, itr->m_ChunkZ);
		if ((chunk == nullptr) || !chunk->IsValid())
		{
			continue;
		}
		Vector3i relPos(itr->m_RelX, itr->m_RelY, itr->m_RelZ);
		switch (chunk->GetBlock(relPos))
		{
			CASE_TREE_OVERWRITTEN_BLOCKS:
			{
				chunk->SetBlock(relPos, itr->m_BlockType, itr->m_BlockMeta);
				break;
			}
			case E_BLOCK_LEAVES:
			case E_BLOCK_NEW_LEAVES:
			{
				if ((itr->m_BlockType == E_BLOCK_LOG) || (itr->m_BlockType == E_BLOCK_NEW_LOG))
				{
					chunk->SetBlock(relPos, itr->m_BlockType, itr->m_BlockMeta);
				}
				break;
			}
		}
	}  // for itr - a_Blocks[]
}





EMCSBiome cChunkMap::GetBiomeAt (int a_BlockX, int a_BlockZ)
{
	int ChunkX, ChunkZ, X = a_BlockX, Y = 0, Z = a_BlockZ;
	cChunkDef::AbsoluteToRelative(X, Y, Z, ChunkX, ChunkZ);

	cCSLock Lock(m_CSChunks);
	cChunkPtr Chunk = GetChunk(ChunkX, ChunkZ);
	if ((Chunk != nullptr) && Chunk->IsValid())
	{
		return Chunk->GetBiomeAt(X, Z);
	}
	else
	{
		return m_World->GetGenerator().GetBiomeAt(a_BlockX, a_BlockZ);
	}
}





bool cChunkMap::SetBiomeAt(int a_BlockX, int a_BlockZ, EMCSBiome a_Biome)
{
	int ChunkX, ChunkZ, X = a_BlockX, Y = 0, Z = a_BlockZ;
	cChunkDef::AbsoluteToRelative(X, Y, Z, ChunkX, ChunkZ);

	cCSLock Lock(m_CSChunks);
	cChunkPtr Chunk = GetChunk(ChunkX, ChunkZ);
	if ((Chunk != nullptr) && Chunk->IsValid())
	{
		Chunk->SetBiomeAt(X, Z, a_Biome);
		return true;
	}
	return false;
}





bool cChunkMap::SetAreaBiome(int a_MinX, int a_MaxX, int a_MinZ, int a_MaxZ, EMCSBiome a_Biome)
{
	// Translate coords to relative:
	int Y = 0;
	int MinChunkX, MinChunkZ, MinX = a_MinX, MinZ = a_MinZ;
	int MaxChunkX, MaxChunkZ, MaxX = a_MaxX, MaxZ = a_MaxZ;
	cChunkDef::AbsoluteToRelative(MinX, Y, MinZ, MinChunkX, MinChunkZ);
	cChunkDef::AbsoluteToRelative(MaxX, Y, MaxZ, MaxChunkX, MaxChunkZ);

	// Go through all chunks, set:
	bool res = true;
	cCSLock Lock(m_CSChunks);
	for (int x = MinChunkX; x <= MaxChunkX; x++)
	{
		int MinRelX = (x == MinChunkX) ? MinX : 0;
		int MaxRelX = (x == MaxChunkX) ? MaxX : cChunkDef::Width - 1;
		for (int z = MinChunkZ; z <= MaxChunkZ; z++)
		{
			int MinRelZ = (z == MinChunkZ) ? MinZ : 0;
			int MaxRelZ = (z == MaxChunkZ) ? MaxZ : cChunkDef::Width - 1;
			cChunkPtr Chunk = GetChunkNoLoad(x, z);
			if ((Chunk != nullptr) && Chunk->IsValid())
			{
				Chunk->SetAreaBiome(MinRelX, MaxRelX, MinRelZ, MaxRelZ, a_Biome);
			}
			else
			{
				res = false;
			}
		}  // for z
	}  // for x
	return res;
}





bool cChunkMap::GetBlocks(sSetBlockVector & a_Blocks, bool a_ContinueOnFailure)
{
	bool res = true;
	cCSLock Lock(m_CSChunks);
	for (sSetBlockVector::iterator itr = a_Blocks.begin(); itr != a_Blocks.end(); ++itr)
	{
		cChunkPtr Chunk = GetChunk(itr->m_ChunkX, itr->m_ChunkZ);
		if ((Chunk == nullptr) || !Chunk->IsValid())
		{
			if (!a_ContinueOnFailure)
			{
				return false;
			}
			res = false;
			continue;
		}
		itr->m_BlockType = Chunk->GetBlock(itr->m_RelX, itr->m_RelY, itr->m_RelZ);
		itr->m_BlockMeta = Chunk->GetMeta(itr->m_RelX, itr->m_RelY, itr->m_RelZ);
	}
	return res;
}





bool cChunkMap::DigBlock(Vector3i a_BlockPos)
{
	auto chunkCoords = cChunkDef::BlockToChunk(a_BlockPos);
	auto relPos = cChunkDef::AbsoluteToRelative(a_BlockPos, chunkCoords);

	{
		cCSLock Lock(m_CSChunks);
		auto destChunk = GetChunk(chunkCoords.m_ChunkX, chunkCoords.m_ChunkZ);
		if ((destChunk == nullptr) || !destChunk->IsValid())
		{
			return false;
		}

		destChunk->SetBlock(relPos, E_BLOCK_AIR, 0);
		m_World->GetSimulatorManager()->WakeUp(a_BlockPos, destChunk);
	}
	return true;
}





cItems cChunkMap::PickupsFromBlock(Vector3i a_BlockPos, const cEntity * a_Digger, const cItem * a_Tool)
{
	auto chunkCoords = cChunkDef::BlockToChunk(a_BlockPos);
	auto relPos = cChunkDef::AbsoluteToRelative(a_BlockPos, chunkCoords);

	cCSLock Lock(m_CSChunks);
	auto destChunk = GetChunk(chunkCoords.m_ChunkX, chunkCoords.m_ChunkZ);
	if ((destChunk == nullptr) || !destChunk->IsValid())
	{
		return {};
	}
	return destChunk->PickupsFromBlock(relPos, a_Digger, a_Tool);
}





void cChunkMap::SendBlockTo(int a_X, int a_Y, int a_Z, cPlayer & a_Player)
{
	int ChunkX, ChunkZ;
	cChunkDef::AbsoluteToRelative(a_X, a_Y, a_Z, ChunkX, ChunkZ);

	cCSLock Lock(m_CSChunks);
	cChunkPtr Chunk = GetChunk(ChunkX, ChunkZ);
	if ((Chunk != nullptr) && (Chunk->IsValid()))
	{
		Chunk->SendBlockTo(a_X, a_Y, a_Z, a_Player.GetClientHandle());
	}
}





void cChunkMap::CompareChunkClients(int a_ChunkX1, int a_ChunkZ1, int a_ChunkX2, int a_ChunkZ2, cClientDiffCallback & a_Callback)
{
	cCSLock Lock(m_CSChunks);
	cChunkPtr Chunk1 = GetChunkNoGen(a_ChunkX1, a_ChunkZ1);
	if (Chunk1 == nullptr)
	{
		return;
	}
	cChunkPtr Chunk2 = GetChunkNoGen(a_ChunkX2, a_ChunkZ2);
	if (Chunk2 == nullptr)
	{
		return;
	}

	CompareChunkClients(Chunk1, Chunk2, a_Callback);
}





void cChunkMap::CompareChunkClients(cChunk * a_Chunk1, cChunk * a_Chunk2, cClientDiffCallback & a_Callback)
{
	auto Clients1 = a_Chunk1->GetAllClients();
	auto Clients2 = a_Chunk2->GetAllClients();

	// Find "removed" clients:
	for (auto * Client : Clients1)
	{
		bool Found = (std::find(Clients2.begin(), Clients2.end(), Client) != Clients2.end());
		if (!Found)
		{
			a_Callback.Removed(Client);
		}
	}  // for Client - Clients1[]

	// Find "added" clients:
	for (auto * Client : Clients2)
	{
		bool Found = (std::find(Clients1.begin(), Clients1.end(), Client) != Clients1.end());
		if (!Found)
		{
			a_Callback.Added(Client);
		}
	}  // for Client - Clients2[]
}





bool cChunkMap::AddChunkClient(int a_ChunkX, int a_ChunkZ, cClientHandle * a_Client)
{
	cCSLock Lock(m_CSChunks);
	cChunkPtr Chunk = GetChunk(a_ChunkX, a_ChunkZ);
	if (Chunk == nullptr)
	{
		return false;
	}
	return Chunk->AddClient(a_Client);
}





void cChunkMap::RemoveChunkClient(int a_ChunkX, int a_ChunkZ, cClientHandle * a_Client)
{
	cCSLock Lock(m_CSChunks);
	cChunkPtr Chunk = GetChunkNoGen(a_ChunkX, a_ChunkZ);
	if (Chunk == nullptr)
	{
		return;
	}
	Chunk->RemoveClient(a_Client);
}





void cChunkMap::RemoveClientFromChunks(cClientHandle * a_Client)
{
	cCSLock Lock(m_CSChunks);
	for (const auto & Chunk : m_Chunks)
	{
		Chunk.second->RemoveClient(a_Client);
	}
}





void cChunkMap::AddEntity(OwnedEntity a_Entity)
{
	cCSLock Lock(m_CSChunks);
	cChunkPtr Chunk = GetChunk(a_Entity->GetChunkX(), a_Entity->GetChunkZ());
	if (Chunk == nullptr)  // This will assert inside GetChunk in Debug builds
	{
		LOGWARNING("Entity at %p (%s, ID %d) spawning in a non-existent chunk, the entity is lost.",
			static_cast<void *>(a_Entity.get()), a_Entity->GetClass(), a_Entity->GetUniqueID()
		);
		return;
	}
	Chunk->AddEntity(std::move(a_Entity));
}





void cChunkMap::AddEntityIfNotPresent(OwnedEntity a_Entity)
{
	cCSLock Lock(m_CSChunks);
	cChunkPtr Chunk = GetChunk(a_Entity->GetChunkX(), a_Entity->GetChunkZ());
	if (Chunk == nullptr)  // This will assert inside GetChunk in Debug builds
	{
		LOGWARNING("Entity at %p (%s, ID %d) spawning in a non-existent chunk, the entity is lost.",
			static_cast<void *>(a_Entity.get()), a_Entity->GetClass(), a_Entity->GetUniqueID()
		);
		return;
	}
	if (!Chunk->HasEntity(a_Entity->GetUniqueID()))
	{
		Chunk->AddEntity(std::move(a_Entity));
	}
}





bool cChunkMap::HasEntity(UInt32 a_UniqueID)
{
	cCSLock Lock(m_CSChunks);
	for (const auto & Chunk : m_Chunks)
	{
		if (Chunk.second->IsValid() && Chunk.second->HasEntity(a_UniqueID))
		{
			return true;
		}
	}
	return false;
}





OwnedEntity cChunkMap::RemoveEntity(cEntity & a_Entity)
{
	cCSLock Lock(m_CSChunks);
	cChunkPtr Chunk = a_Entity.GetParentChunk();

	if (Chunk == nullptr)
	{
		return nullptr;
	}

	// Remove the entity no matter whether the chunk itself is valid or not (#1190)
	return Chunk->RemoveEntity(a_Entity);
}





bool cChunkMap::ForEachEntity(cEntityCallback a_Callback)
{
	cCSLock Lock(m_CSChunks);
	for (const auto & Chunk : m_Chunks)
	{
		if (Chunk.second->IsValid() && !Chunk.second->ForEachEntity(a_Callback))
		{
			return false;
		}
	}
	return true;
}





bool cChunkMap::ForEachEntityInChunk(int a_ChunkX, int a_ChunkZ, cEntityCallback a_Callback)
{
	cCSLock Lock(m_CSChunks);
	cChunkPtr Chunk = GetChunkNoGen(a_ChunkX, a_ChunkZ);
	if ((Chunk == nullptr) || !Chunk->IsValid())
	{
		return false;
	}
	return Chunk->ForEachEntity(a_Callback);
}





bool cChunkMap::ForEachEntityInBox(const cBoundingBox & a_Box, cEntityCallback a_Callback)
{
	// Calculate the chunk range for the box:
	int MinChunkX = FloorC(a_Box.GetMinX() / cChunkDef::Width);
	int MinChunkZ = FloorC(a_Box.GetMinZ() / cChunkDef::Width);
	int MaxChunkX = FloorC((a_Box.GetMaxX() + cChunkDef::Width) / cChunkDef::Width);
	int MaxChunkZ = FloorC((a_Box.GetMaxZ() + cChunkDef::Width) / cChunkDef::Width);

	// Iterate over each chunk in the range:
	cCSLock Lock(m_CSChunks);
	for (int z = MinChunkZ; z <= MaxChunkZ; z++)
	{
		for (int x = MinChunkX; x <= MaxChunkX; x++)
		{
			cChunkPtr Chunk = GetChunkNoGen(x, z);
			if ((Chunk == nullptr) || !Chunk->IsValid())
			{
				continue;
			}
			if (!Chunk->ForEachEntityInBox(a_Box, a_Callback))
			{
				return false;
			}
		}  // for x
	}  // for z
	return true;
}





void cChunkMap::DoExplosionAt(double a_ExplosionSize, double a_BlockX, double a_BlockY, double a_BlockZ, cVector3iArray & a_BlocksAffected)
{
	// Don't explode if outside of Y range (prevents the following test running into unallocated memory):
	if (!cChunkDef::IsValidHeight(FloorC(a_BlockY)))
	{
		return;
	}

	bool ShouldDestroyBlocks = true;

	// Don't explode if the explosion center is inside a liquid block:
	if (IsBlockLiquid(m_World->GetBlock(FloorC(a_BlockX), FloorC(a_BlockY), FloorC(a_BlockZ))))
	{
		ShouldDestroyBlocks = false;
	}

	int ExplosionSizeInt = CeilC(a_ExplosionSize);
	int ExplosionSizeSq = ExplosionSizeInt * ExplosionSizeInt;

	int bx = FloorC(a_BlockX);
	int by = FloorC(a_BlockY);
	int bz = FloorC(a_BlockZ);

	int MinY = std::max(FloorC(a_BlockY - ExplosionSizeInt), 0);
	int MaxY = std::min(CeilC(a_BlockY + ExplosionSizeInt), cChunkDef::Height - 1);

	if (ShouldDestroyBlocks)
	{
		cBlockArea area;
		a_BlocksAffected.reserve(8 * static_cast<size_t>(ExplosionSizeInt * ExplosionSizeInt * ExplosionSizeInt));
		if (!area.Read(*m_World, bx - ExplosionSizeInt, static_cast<int>(ceil(a_BlockX + ExplosionSizeInt)), MinY, MaxY, bz - ExplosionSizeInt, static_cast<int>(ceil(a_BlockZ + ExplosionSizeInt))))
		{
			return;
		}

		for (int x = -ExplosionSizeInt; x < ExplosionSizeInt; x++)
		{
			for (int y = -ExplosionSizeInt; y < ExplosionSizeInt; y++)
			{
				if ((by + y >= cChunkDef::Height) || (by + y < 0))
				{
					// Outside of the world
					continue;
				}
				for (int z = -ExplosionSizeInt; z < ExplosionSizeInt; z++)
				{
					if ((x * x + y * y + z * z) > ExplosionSizeSq)
					{
						// Too far away
						continue;
					}

					BLOCKTYPE Block = area.GetBlockType(bx + x, by + y, bz + z);
					switch (Block)
					{
						case E_BLOCK_TNT:
						{
							// Activate the TNT, with a random fuse between 10 to 30 game ticks
							int FuseTime = GetRandomProvider().RandInt(10, 30);
							m_World->SpawnPrimedTNT({a_BlockX + x + 0.5, a_BlockY + y + 0.5, a_BlockZ + z + 0.5}, FuseTime, 1, false);  // Initial velocity, no fuse sound
							area.SetBlockTypeMeta(bx + x, by + y, bz + z, E_BLOCK_AIR, 0);
							a_BlocksAffected.push_back(Vector3i(bx + x, by + y, bz + z));
							break;
						}

						case E_BLOCK_OBSIDIAN:
						case E_BLOCK_BEACON:
						case E_BLOCK_BEDROCK:
						case E_BLOCK_BARRIER:
						case E_BLOCK_WATER:
						case E_BLOCK_LAVA:
						{
							// These blocks are not affected by explosions
							break;
						}

						case E_BLOCK_STATIONARY_WATER:
						{
							// Turn into simulated water:
							area.SetBlockType(bx + x, by + y, bz + z, E_BLOCK_WATER);
							break;
						}

						case E_BLOCK_STATIONARY_LAVA:
						{
							// Turn into simulated lava:
							area.SetBlockType(bx + x, by + y, bz + z, E_BLOCK_LAVA);
							break;
						}

						case E_BLOCK_AIR:
						{
							// No pickups for air
							break;
						}

						default:
						{
							auto & Random = GetRandomProvider();
							if (Random.RandBool(0.25))  // 25% chance of pickups
							{
								auto pickups = area.PickupsFromBlock({bx + x, by + y, bz + z});
								m_World->SpawnItemPickups(pickups, bx + x, by + y, bz + z);
							}
							else if ((m_World->GetTNTShrapnelLevel() > slNone) && Random.RandBool(0.20))  // 20% chance of flinging stuff around
							{
								// If the block is shrapnel-able, make a falling block entity out of it:
								if (
									((m_World->GetTNTShrapnelLevel() == slAll) && cBlockInfo::FullyOccupiesVoxel(Block)) ||
									((m_World->GetTNTShrapnelLevel() == slGravityAffectedOnly) && ((Block == E_BLOCK_SAND) || (Block == E_BLOCK_GRAVEL)))
								)
								{
									m_World->SpawnFallingBlock(bx + x, by + y + 5, bz + z, Block, area.GetBlockMeta(bx + x, by + y, bz + z));
								}
							}

							// Destroy any block entities
							if (cBlockEntity::IsBlockEntityBlockType(Block))
							{
								Vector3i BlockPos(bx + x, by + y, bz + z);
								DoWithBlockEntityAt(BlockPos.x, BlockPos.y, BlockPos.z, [](cBlockEntity & a_BE)
									{
										a_BE.Destroy();
										return true;
									}
								);
							}

							area.SetBlockTypeMeta(bx + x, by + y, bz + z, E_BLOCK_AIR, 0);
							a_BlocksAffected.push_back(Vector3i(bx + x, by + y, bz + z));
							break;
						}
					}  // switch (BlockType)
				}  // for z
			}  // for y
		}  // for x
		area.Write(*m_World, bx - ExplosionSizeInt, MinY, bz - ExplosionSizeInt);
	}

	Vector3d ExplosionPos{ a_BlockX, a_BlockY, a_BlockZ };
	cBoundingBox bbTNT(ExplosionPos, 0.5, 1);
	bbTNT.Expand(ExplosionSizeInt * 2, ExplosionSizeInt * 2, ExplosionSizeInt * 2);

	ForEachEntity([&](cEntity & a_Entity)
		{
			if (a_Entity.IsPickup() && (a_Entity.GetTicksAlive() < 20))
			{
				// If pickup age is smaller than one second, it is invincible (so we don't kill pickups that were just spawned)
				return false;
			}

			Vector3d DistanceFromExplosion = a_Entity.GetPosition() - ExplosionPos;

			if (!a_Entity.IsTNT() && !a_Entity.IsFallingBlock())  // Don't apply damage to other TNT entities and falling blocks, they should be invincible
			{
				auto EntityBox = a_Entity.GetBoundingBox();
				if (!bbTNT.IsInside(EntityBox))  // If entity box is inside tnt box, not vice versa!
				{
					return false;
				}

				// Ensure that the damage dealt is inversely proportional to the distance to the TNT centre - the closer a player is, the harder they are hit
				a_Entity.TakeDamage(dtExplosion, nullptr, static_cast<int>((1 / std::max(1.0, DistanceFromExplosion.Length())) * 8 * ExplosionSizeInt), 0);
			}

			float EntityExposure = a_Entity.GetExplosionExposureRate(ExplosionPos, static_cast<float>(a_ExplosionSize));

			// Exposure reduced by armor
			EntityExposure = EntityExposure * (1.0f - a_Entity.GetEnchantmentBlastKnockbackReduction());

			auto Impact = std::pow(std::max(0.2, DistanceFromExplosion.Length()), -1);
			Impact *= EntityExposure * ExplosionSizeInt * 6.0;

			if (Impact > 0.0)
			{
				DistanceFromExplosion.Normalize();
				DistanceFromExplosion *= Vector3d{Impact, 0.0, Impact};
				DistanceFromExplosion.y += 0.3 * Impact;

				a_Entity.SetSpeed(DistanceFromExplosion);
			}

			return false;
		}
	);

	// Wake up all simulators for the area, so that water and lava flows and sand falls into the blasted holes (FS #391):
	m_World->GetSimulatorManager()->WakeUpArea(cCuboid(
		{bx - ExplosionSizeInt - 1, MinY, bz - ExplosionSizeInt - 1},
		{bx + ExplosionSizeInt + 1, MaxY, bz + ExplosionSizeInt + 1}
	));
}





bool cChunkMap::DoWithEntityByID(UInt32 a_UniqueID, cEntityCallback a_Callback)
{
	cCSLock Lock(m_CSChunks);
	bool res = false;
	for (const auto & Chunk : m_Chunks)
	{
		if (Chunk.second->IsValid() && Chunk.second->DoWithEntityByID(a_UniqueID, a_Callback, res))
		{
			return res;
		}
	}
	return false;
}





bool cChunkMap::ForEachBlockEntityInChunk(int a_ChunkX, int a_ChunkZ, cBlockEntityCallback a_Callback)
{
	cCSLock Lock(m_CSChunks);
	cChunkPtr Chunk = GetChunkNoGen(a_ChunkX, a_ChunkZ);
	if ((Chunk == nullptr) || !Chunk->IsValid())
	{
		return false;
	}
	return Chunk->ForEachBlockEntity(a_Callback);
}





bool cChunkMap::ForEachBrewingstandInChunk(int a_ChunkX, int a_ChunkZ, cBrewingstandCallback a_Callback)
{
	cCSLock Lock(m_CSChunks);
	cChunkPtr Chunk = GetChunkNoGen(a_ChunkX, a_ChunkZ);
	if ((Chunk == nullptr) || !Chunk->IsValid())
	{
		return false;
	}
	return Chunk->ForEachBrewingstand(a_Callback);
}





bool cChunkMap::ForEachChestInChunk(int a_ChunkX, int a_ChunkZ, cChestCallback a_Callback)
{
	cCSLock Lock(m_CSChunks);
	cChunkPtr Chunk = GetChunkNoGen(a_ChunkX, a_ChunkZ);
	if ((Chunk == nullptr) || !Chunk->IsValid())
	{
		return false;
	}
	return Chunk->ForEachChest(a_Callback);
}





bool cChunkMap::ForEachDispenserInChunk(int a_ChunkX, int a_ChunkZ, cDispenserCallback a_Callback)
{
	cCSLock Lock(m_CSChunks);
	cChunkPtr Chunk = GetChunkNoGen(a_ChunkX, a_ChunkZ);
	if ((Chunk == nullptr) || !Chunk->IsValid())
	{
		return false;
	}
	return Chunk->ForEachDispenser(a_Callback);
}





bool cChunkMap::ForEachDropperInChunk(int a_ChunkX, int a_ChunkZ, cDropperCallback a_Callback)
{
	cCSLock Lock(m_CSChunks);
	cChunkPtr Chunk = GetChunkNoGen(a_ChunkX, a_ChunkZ);
	if ((Chunk == nullptr) || !Chunk->IsValid())
	{
		return false;
	}
	return Chunk->ForEachDropper(a_Callback);
}





bool cChunkMap::ForEachDropSpenserInChunk(int a_ChunkX, int a_ChunkZ, cDropSpenserCallback a_Callback)
{
	cCSLock Lock(m_CSChunks);
	cChunkPtr Chunk = GetChunkNoGen(a_ChunkX, a_ChunkZ);
	if ((Chunk == nullptr) || !Chunk->IsValid())
	{
		return false;
	}
	return Chunk->ForEachDropSpenser(a_Callback);
}





bool cChunkMap::ForEachFurnaceInChunk(int a_ChunkX, int a_ChunkZ, cFurnaceCallback a_Callback)
{
	cCSLock Lock(m_CSChunks);
	cChunkPtr Chunk = GetChunkNoGen(a_ChunkX, a_ChunkZ);
	if ((Chunk == nullptr) || !Chunk->IsValid())
	{
		return false;
	}
	return Chunk->ForEachFurnace(a_Callback);
}





bool cChunkMap::DoWithBlockEntityAt(int a_BlockX, int a_BlockY, int a_BlockZ, cBlockEntityCallback a_Callback)
{
	int ChunkX, ChunkZ;
	int BlockX = a_BlockX, BlockY = a_BlockY, BlockZ = a_BlockZ;
	cChunkDef::AbsoluteToRelative(BlockX, BlockY, BlockZ, ChunkX, ChunkZ);
	cCSLock Lock(m_CSChunks);
	cChunkPtr Chunk = GetChunkNoGen(ChunkX, ChunkZ);
	if ((Chunk == nullptr) || !Chunk->IsValid())
	{
		return false;
	}
	return Chunk->DoWithBlockEntityAt(a_BlockX, a_BlockY, a_BlockZ, a_Callback);
}





bool cChunkMap::DoWithBeaconAt(int a_BlockX, int a_BlockY, int a_BlockZ, cBeaconCallback a_Callback)
{
	int ChunkX, ChunkZ;
	int BlockX = a_BlockX, BlockY = a_BlockY, BlockZ = a_BlockZ;
	cChunkDef::AbsoluteToRelative(BlockX, BlockY, BlockZ, ChunkX, ChunkZ);
	cCSLock Lock(m_CSChunks);
	cChunkPtr Chunk = GetChunkNoGen(ChunkX, ChunkZ);
	if ((Chunk == nullptr) || !Chunk->IsValid())
	{
		return false;
	}
	return Chunk->DoWithBeaconAt(a_BlockX, a_BlockY, a_BlockZ, a_Callback);
}





bool cChunkMap::DoWithBedAt(int a_BlockX, int a_BlockY, int a_BlockZ, cBedCallback a_Callback)
{
	int ChunkX, ChunkZ;
	int BlockX = a_BlockX, BlockY = a_BlockY, BlockZ = a_BlockZ;
	cChunkDef::AbsoluteToRelative(BlockX, BlockY, BlockZ, ChunkX, ChunkZ);
	cCSLock Lock(m_CSChunks);
	cChunkPtr Chunk = GetChunkNoGen(ChunkX, ChunkZ);
	if ((Chunk == nullptr) || !Chunk->IsValid())
	{
		return false;
	}
	return Chunk->DoWithBedAt(a_BlockX, a_BlockY, a_BlockZ, a_Callback);
}





bool cChunkMap::DoWithBrewingstandAt(int a_BlockX, int a_BlockY, int a_BlockZ, cBrewingstandCallback a_Callback)
{
	int ChunkX, ChunkZ;
	int BlockX = a_BlockX, BlockY = a_BlockY, BlockZ = a_BlockZ;
	cChunkDef::AbsoluteToRelative(BlockX, BlockY, BlockZ, ChunkX, ChunkZ);
	cCSLock Lock(m_CSChunks);
	cChunkPtr Chunk = GetChunkNoGen(ChunkX, ChunkZ);
	if ((Chunk == nullptr) || !Chunk->IsValid())
	{
		return false;
	}
	return Chunk->DoWithBrewingstandAt(a_BlockX, a_BlockY, a_BlockZ, a_Callback);
}





bool cChunkMap::DoWithChestAt(int a_BlockX, int a_BlockY, int a_BlockZ, cChestCallback a_Callback)
{
	int ChunkX, ChunkZ;
	int BlockX = a_BlockX, BlockY = a_BlockY, BlockZ = a_BlockZ;
	cChunkDef::AbsoluteToRelative(BlockX, BlockY, BlockZ, ChunkX, ChunkZ);
	cCSLock Lock(m_CSChunks);
	cChunkPtr Chunk = GetChunkNoGen(ChunkX, ChunkZ);
	if ((Chunk == nullptr) || !Chunk->IsValid())
	{
		return false;
	}
	return Chunk->DoWithChestAt(a_BlockX, a_BlockY, a_BlockZ, a_Callback);
}





bool cChunkMap::DoWithDispenserAt(int a_BlockX, int a_BlockY, int a_BlockZ, cDispenserCallback a_Callback)
{
	int ChunkX, ChunkZ;
	int BlockX = a_BlockX, BlockY = a_BlockY, BlockZ = a_BlockZ;
	cChunkDef::AbsoluteToRelative(BlockX, BlockY, BlockZ, ChunkX, ChunkZ);
	cCSLock Lock(m_CSChunks);
	cChunkPtr Chunk = GetChunkNoGen(ChunkX, ChunkZ);
	if ((Chunk == nullptr) || !Chunk->IsValid())
	{
		return false;
	}
	return Chunk->DoWithDispenserAt(a_BlockX, a_BlockY, a_BlockZ, a_Callback);
}





bool cChunkMap::DoWithDropperAt(int a_BlockX, int a_BlockY, int a_BlockZ, cDropperCallback a_Callback)
{
	int ChunkX, ChunkZ;
	int BlockX = a_BlockX, BlockY = a_BlockY, BlockZ = a_BlockZ;
	cChunkDef::AbsoluteToRelative(BlockX, BlockY, BlockZ, ChunkX, ChunkZ);
	cCSLock Lock(m_CSChunks);
	cChunkPtr Chunk = GetChunkNoGen(ChunkX, ChunkZ);
	if ((Chunk == nullptr) || !Chunk->IsValid())
	{
		return false;
	}
	return Chunk->DoWithDropperAt(a_BlockX, a_BlockY, a_BlockZ, a_Callback);
}





bool cChunkMap::DoWithDropSpenserAt(int a_BlockX, int a_BlockY, int a_BlockZ, cDropSpenserCallback a_Callback)
{
	int ChunkX, ChunkZ;
	int BlockX = a_BlockX, BlockY = a_BlockY, BlockZ = a_BlockZ;
	cChunkDef::AbsoluteToRelative(BlockX, BlockY, BlockZ, ChunkX, ChunkZ);
	cCSLock Lock(m_CSChunks);
	cChunkPtr Chunk = GetChunkNoGen(ChunkX, ChunkZ);
	if ((Chunk == nullptr) || !Chunk->IsValid())
	{
		return false;
	}
	return Chunk->DoWithDropSpenserAt(a_BlockX, a_BlockY, a_BlockZ, a_Callback);
}





bool cChunkMap::DoWithFurnaceAt(int a_BlockX, int a_BlockY, int a_BlockZ, cFurnaceCallback a_Callback)
{
	int ChunkX, ChunkZ;
	int BlockX = a_BlockX, BlockY = a_BlockY, BlockZ = a_BlockZ;
	cChunkDef::AbsoluteToRelative(BlockX, BlockY, BlockZ, ChunkX, ChunkZ);
	cCSLock Lock(m_CSChunks);
	cChunkPtr Chunk = GetChunkNoGen(ChunkX, ChunkZ);
	if ((Chunk == nullptr) || !Chunk->IsValid())
	{
		return false;
	}
	return Chunk->DoWithFurnaceAt(a_BlockX, a_BlockY, a_BlockZ, a_Callback);
}





bool cChunkMap::DoWithHopperAt(int a_BlockX, int a_BlockY, int a_BlockZ, cHopperCallback a_Callback)
{
	int ChunkX, ChunkZ;
	int BlockX = a_BlockX, BlockY = a_BlockY, BlockZ = a_BlockZ;
	cChunkDef::AbsoluteToRelative(BlockX, BlockY, BlockZ, ChunkX, ChunkZ);
	cCSLock Lock(m_CSChunks);
	cChunkPtr Chunk = GetChunkNoGen(ChunkX, ChunkZ);
	if ((Chunk == nullptr) || !Chunk->IsValid())
	{
		return false;
	}
	return Chunk->DoWithHopperAt(a_BlockX, a_BlockY, a_BlockZ, a_Callback);
}





bool cChunkMap::DoWithNoteBlockAt(int a_BlockX, int a_BlockY, int a_BlockZ, cNoteBlockCallback a_Callback)
{
	int ChunkX, ChunkZ;
	int BlockX = a_BlockX, BlockY = a_BlockY, BlockZ = a_BlockZ;
	cChunkDef::AbsoluteToRelative(BlockX, BlockY, BlockZ, ChunkX, ChunkZ);
	cCSLock Lock(m_CSChunks);
	cChunkPtr Chunk = GetChunkNoGen(ChunkX, ChunkZ);
	if ((Chunk == nullptr) || !Chunk->IsValid())
	{
		return false;
	}
	return Chunk->DoWithNoteBlockAt(a_BlockX, a_BlockY, a_BlockZ, a_Callback);
}





bool cChunkMap::DoWithCommandBlockAt(int a_BlockX, int a_BlockY, int a_BlockZ, cCommandBlockCallback a_Callback)
{
	int ChunkX, ChunkZ;
	int BlockX = a_BlockX, BlockY = a_BlockY, BlockZ = a_BlockZ;
	cChunkDef::AbsoluteToRelative(BlockX, BlockY, BlockZ, ChunkX, ChunkZ);
	cCSLock Lock(m_CSChunks);
	cChunkPtr Chunk = GetChunkNoGen(ChunkX, ChunkZ);
	if ((Chunk == nullptr) || !Chunk->IsValid())
	{
		return false;
	}
	return Chunk->DoWithCommandBlockAt(a_BlockX, a_BlockY, a_BlockZ, a_Callback);
}





bool cChunkMap::DoWithMobHeadAt(int a_BlockX, int a_BlockY, int a_BlockZ, cMobHeadCallback a_Callback)
{
	int ChunkX, ChunkZ;
	int BlockX = a_BlockX, BlockY = a_BlockY, BlockZ = a_BlockZ;
	cChunkDef::AbsoluteToRelative(BlockX, BlockY, BlockZ, ChunkX, ChunkZ);
	cCSLock Lock(m_CSChunks);
	cChunkPtr Chunk = GetChunkNoGen(ChunkX, ChunkZ);
	if ((Chunk == nullptr) || !Chunk->IsValid())
	{
		return false;
	}
	return Chunk->DoWithMobHeadAt(a_BlockX, a_BlockY, a_BlockZ, a_Callback);
}





bool cChunkMap::DoWithFlowerPotAt(int a_BlockX, int a_BlockY, int a_BlockZ, cFlowerPotCallback a_Callback)
{
	int ChunkX, ChunkZ;
	int BlockX = a_BlockX, BlockY = a_BlockY, BlockZ = a_BlockZ;
	cChunkDef::AbsoluteToRelative(BlockX, BlockY, BlockZ, ChunkX, ChunkZ);
	cCSLock Lock(m_CSChunks);
	cChunkPtr Chunk = GetChunkNoGen(ChunkX, ChunkZ);
	if ((Chunk == nullptr) || !Chunk->IsValid())
	{
		return false;
	}
	return Chunk->DoWithFlowerPotAt(a_BlockX, a_BlockY, a_BlockZ, a_Callback);
}





bool cChunkMap::GetSignLines(int a_BlockX, int a_BlockY, int a_BlockZ, AString & a_Line1, AString & a_Line2, AString & a_Line3, AString & a_Line4)
{
	int ChunkX, ChunkZ;
	int BlockX = a_BlockX, BlockY = a_BlockY, BlockZ = a_BlockZ;
	cChunkDef::AbsoluteToRelative(BlockX, BlockY, BlockZ, ChunkX, ChunkZ);
	cCSLock Lock(m_CSChunks);
	cChunkPtr Chunk = GetChunkNoGen(ChunkX, ChunkZ);
	if ((Chunk == nullptr) || !Chunk->IsValid())
	{
		return false;
	}
	return Chunk->GetSignLines(a_BlockX, a_BlockY, a_BlockZ, a_Line1, a_Line2, a_Line3, a_Line4);
}





void cChunkMap::TouchChunk(int a_ChunkX, int a_ChunkZ)
{
	cCSLock Lock(m_CSChunks);
	GetChunk(a_ChunkX, a_ChunkZ);
}





void cChunkMap::PrepareChunk(int a_ChunkX, int a_ChunkZ, std::unique_ptr<cChunkCoordCallback> a_Callback)
{
	cCSLock Lock(m_CSChunks);
	cChunkPtr Chunk = GetChunkNoLoad(a_ChunkX, a_ChunkZ);

	// If the chunk is not prepared, queue it in the lighting thread, that will do all the needed processing:
	if ((Chunk == nullptr) || !Chunk->IsValid() || !Chunk->IsLightValid())
	{
		m_World->GetLightingThread().QueueChunk(a_ChunkX, a_ChunkZ, std::move(a_Callback));
		return;
	}

	// The chunk is present and lit, just call the callback, report as success:
	if (a_Callback != nullptr)
	{
		a_Callback->Call({a_ChunkX, a_ChunkZ}, true);
	}
}





bool cChunkMap::GenerateChunk(int a_ChunkX, int a_ChunkZ, cChunkCoordCallback * a_Callback)
{
	cCSLock Lock(m_CSChunks);
	cChunkPtr Chunk = GetChunkNoLoad(a_ChunkX, a_ChunkZ);
	if (Chunk == nullptr)
	{
		// Generic error while getting the chunk - out of memory?
		return false;
	}

	// Try loading the chunk:
	if ((Chunk == nullptr) || (!Chunk->IsValid()))
	{
		Chunk->SetPresence(cChunk::cpQueued);
		class cPrepareLoadCallback: public cChunkCoordCallback
		{
		public:
			cPrepareLoadCallback(cWorld & a_CBWorld, cChunkMap & a_CBChunkMap, cChunkCoordCallback * a_CBCallback):
				m_World(a_CBWorld),
				m_ChunkMap(a_CBChunkMap),
				m_Callback(a_CBCallback)
			{
			}

			// cChunkCoordCallback override:
			virtual void Call(cChunkCoords a_Coords, bool a_CBIsSuccess) override
			{
				// If success is reported, the chunk is already valid, no need to do anything else:
				if (a_CBIsSuccess)
				{
					if (m_Callback != nullptr)
					{
						m_Callback->Call(a_Coords, true);
					}
					return;
				}

				// The chunk failed to load, generate it:
				cCSLock CBLock(m_ChunkMap.m_CSChunks);
				cChunkPtr CBChunk = m_ChunkMap.GetChunkNoLoad(a_Coords.m_ChunkX, a_Coords.m_ChunkZ);

				if (CBChunk == nullptr)
				{
					// An error occurred, but we promised to call the callback, so call it even when there's no real chunk data:
					if (m_Callback != nullptr)
					{
						m_Callback->Call(a_Coords, false);
					}
					return;
				}

				CBChunk->SetPresence(cChunk::cpQueued);
				m_World.GetGenerator().QueueGenerateChunk(a_Coords, false, m_Callback);
			}

		protected:
			cWorld & m_World;
			cChunkMap & m_ChunkMap;
			cChunkCoordCallback * m_Callback;
		};
		m_World->GetStorage().QueueLoadChunk(a_ChunkX, a_ChunkZ, new cPrepareLoadCallback(*m_World, *this, a_Callback));
		return true;
	}

	// The chunk is valid, just call the callback:
	if (a_Callback != nullptr)
	{
		a_Callback->Call({a_ChunkX, a_ChunkZ}, true);
	}
	return true;
}





void cChunkMap::ChunkLoadFailed(int a_ChunkX, int a_ChunkZ)
{
	cCSLock Lock(m_CSChunks);
	cChunkPtr Chunk = GetChunkNoLoad(a_ChunkX, a_ChunkZ);
	if (Chunk == nullptr)
	{
		return;
	}
	Chunk->MarkLoadFailed();
}





bool cChunkMap::SetSignLines(int a_BlockX, int a_BlockY, int a_BlockZ, const AString & a_Line1, const AString & a_Line2, const AString & a_Line3, const AString & a_Line4)
{
	cCSLock Lock(m_CSChunks);
	int ChunkX, ChunkZ;
	cChunkDef::BlockToChunk(a_BlockX, a_BlockZ, ChunkX, ChunkZ);
	cChunkPtr Chunk = GetChunkNoGen(ChunkX, ChunkZ);
	if ((Chunk == nullptr) || !Chunk->IsValid())
	{
		return false;
	}
	return Chunk->SetSignLines(a_BlockX, a_BlockY, a_BlockZ, a_Line1, a_Line2, a_Line3, a_Line4);
}





void cChunkMap::MarkChunkRegenerating(int a_ChunkX, int a_ChunkZ)
{
	cCSLock Lock(m_CSChunks);
	cChunkPtr Chunk = GetChunkNoLoad(a_ChunkX, a_ChunkZ);
	if (Chunk == nullptr)
	{
		// Not present
		return;
	}
	Chunk->MarkRegenerating();
}





bool cChunkMap::IsChunkLighted(int a_ChunkX, int a_ChunkZ)
{
	cCSLock Lock(m_CSChunks);
	cChunkPtr Chunk = GetChunkNoLoad(a_ChunkX, a_ChunkZ);
	if (Chunk == nullptr)
	{
		// Not present
		return false;
	}
	return Chunk->IsLightValid();
}





bool cChunkMap::ForEachChunkInRect(int a_MinChunkX, int a_MaxChunkX, int a_MinChunkZ, int a_MaxChunkZ, cChunkDataCallback & a_Callback)
{
	bool Result = true;
	cCSLock Lock(m_CSChunks);
	for (int z = a_MinChunkZ; z <= a_MaxChunkZ; z++)
	{
		for (int x = a_MinChunkX; x <= a_MaxChunkX; x++)
		{
			cChunkPtr Chunk = GetChunkNoLoad(x, z);
			if ((Chunk == nullptr) || (!Chunk->IsValid()))
			{
				// Not present / not valid
				Result = false;
				continue;
			}
			if (!a_Callback.Coords(x, z))
			{
				continue;
			}
			Chunk->GetAllData(a_Callback);
		}
	}
	return Result;
}





bool cChunkMap::ForEachLoadedChunk(cFunctionRef<bool(int, int)> a_Callback)
{
	cCSLock Lock(m_CSChunks);
	for (const auto & Chunk : m_Chunks)
	{
		if (Chunk.second->IsValid())
		{
			if (a_Callback(Chunk.first.ChunkX, Chunk.first.ChunkZ))
			{
				return false;
			}
		}
	}
	return true;
}





bool cChunkMap::WriteBlockArea(cBlockArea & a_Area, int a_MinBlockX, int a_MinBlockY, int a_MinBlockZ, int a_DataTypes)
{
	// Convert block coords to chunks coords:
	int MinChunkX, MaxChunkX;
	int MinChunkZ, MaxChunkZ;
	int MinBlockX = a_MinBlockX;
	int MinBlockY = a_MinBlockY;
	int MinBlockZ = a_MinBlockZ;
	int MaxBlockX = a_MinBlockX + a_Area.GetSizeX();
	int MaxBlockY = a_MinBlockY + a_Area.GetSizeY();
	int MaxBlockZ = a_MinBlockZ + a_Area.GetSizeZ();
	cChunkDef::AbsoluteToRelative(MinBlockX, MinBlockY, MinBlockZ, MinChunkX, MinChunkZ);
	cChunkDef::AbsoluteToRelative(MaxBlockX, MaxBlockY, MaxBlockZ, MaxChunkX, MaxChunkZ);

	// Iterate over chunks, write data into each:
	bool Result = true;
	cCSLock Lock(m_CSChunks);
	for (int z = MinChunkZ; z <= MaxChunkZ; z++)
	{
		for (int x = MinChunkX; x <= MaxChunkX; x++)
		{
			cChunkPtr Chunk = GetChunkNoLoad(x, z);
			if ((Chunk == nullptr) || (!Chunk->IsValid()))
			{
				// Not present / not valid
				Result = false;
				continue;
			}
			Chunk->WriteBlockArea(a_Area, a_MinBlockX, a_MinBlockY, a_MinBlockZ, a_DataTypes);
		}  // for x
	}  // for z
	return Result;
}





void cChunkMap::GetChunkStats(int & a_NumChunksValid, int & a_NumChunksDirty)
{
	a_NumChunksValid = 0;
	a_NumChunksDirty = 0;
	cCSLock Lock(m_CSChunks);
	for (const auto & Chunk : m_Chunks)
	{
		a_NumChunksValid++;
		if (Chunk.second->IsDirty())
		{
			a_NumChunksDirty++;
		}
	}
}





int cChunkMap::GrowPlantAt(Vector3i a_BlockPos, int a_NumStages)
{
	auto chunkPos = cChunkDef::BlockToChunk(a_BlockPos);
	auto relPos = cChunkDef::AbsoluteToRelative(a_BlockPos, chunkPos);
	cCSLock lock(m_CSChunks);
	auto chunk = GetChunkNoLoad(chunkPos);
	if (chunk == nullptr)
	{
		return 0;
	}
	return chunk->GrowPlantAt(relPos, a_NumStages);
}





void cChunkMap::SetNextBlockToTick(const Vector3i a_BlockPos)
{
	auto ChunkPos = cChunkDef::BlockToChunk(a_BlockPos);
	auto RelPos = cChunkDef::AbsoluteToRelative(a_BlockPos, ChunkPos);

	cCSLock Lock(m_CSChunks);
	auto Chunk = GetChunkNoLoad(ChunkPos);
	if (Chunk != nullptr)
	{
		Chunk->SetNextBlockToTick(RelPos);
	}
}





void cChunkMap::CollectMobCensus(cMobCensus & a_ToFill)
{
	cCSLock Lock(m_CSChunks);
	for (const auto & Chunk : m_Chunks)
	{
		// We do count every Mobs in the world. But we are assuming that every chunk not loaded by any client
		// doesn't affect us. Normally they should not have mobs because every "too far" mobs despawn
		// If they have (f.i. when player disconnect) we assume we don't have to make them live or despawn
		if (Chunk.second->IsValid() && Chunk.second->HasAnyClients())
		{
			Chunk.second->CollectMobCensus(a_ToFill);
		}
	}
}





void cChunkMap::SpawnMobs(cMobSpawner & a_MobSpawner)
{
	cCSLock Lock(m_CSChunks);
	for (const auto & Chunk : m_Chunks)
	{
		// We only spawn close to players
		if (Chunk.second->IsValid() && Chunk.second->HasAnyClients())
		{
			Chunk.second->SpawnMobs(a_MobSpawner);
		}
	}
}





void cChunkMap::Tick(std::chrono::milliseconds a_Dt)
{
	cCSLock Lock(m_CSChunks);
	for (const auto & Chunk : m_Chunks)
	{
		// Only tick chunks that are valid and should be ticked:
		if (Chunk.second->IsValid() && Chunk.second->ShouldBeTicked())
		{
			Chunk.second->Tick(a_Dt);
		}
	}
}





void cChunkMap::TickBlock(const Vector3i a_BlockPos)
{
	auto ChunkPos = cChunkDef::BlockToChunk(a_BlockPos);
	auto RelPos = cChunkDef::AbsoluteToRelative(a_BlockPos, ChunkPos);
	cCSLock Lock(m_CSChunks);
	auto Chunk = GetChunkNoLoad(ChunkPos);
	if ((Chunk == nullptr) || !Chunk->IsValid())
	{
		return;
	}
	Chunk->TickBlock(RelPos);
}





void cChunkMap::UnloadUnusedChunks(void)
{
	cCSLock Lock(m_CSChunks);
	for (auto itr = m_Chunks.begin(); itr != m_Chunks.end();)
	{
		if (
			(itr->second->CanUnload()) &&  // Can unload
			!cPluginManager::Get()->CallHookChunkUnloading(*GetWorld(), itr->first.ChunkX, itr->first.ChunkZ)  // Plugins agree
		)
		{
			itr = m_Chunks.erase(itr);
		}
		else
		{
			++itr;
		}
	}
}





void cChunkMap::SaveAllChunks(void)
{
	cCSLock Lock(m_CSChunks);
	for (const auto & Chunk : m_Chunks)
	{
		if (Chunk.second->IsValid() && Chunk.second->IsDirty())
		{
			GetWorld()->GetStorage().QueueSaveChunk(Chunk.first.ChunkX, Chunk.first.ChunkZ);
		}
	}
}





size_t cChunkMap::GetNumChunks(void)
{
	cCSLock Lock(m_CSChunks);
	return m_Chunks.size();
}





size_t cChunkMap::GetNumUnusedDirtyChunks(void)
{
	cCSLock Lock(m_CSChunks);
	size_t res = 0;
	for (const auto & Chunk : m_Chunks)
	{
		if (Chunk.second->IsValid() && Chunk.second->CanUnloadAfterSaving())
		{
			res += 1;
		}
	}
	return res;
}





void cChunkMap::ChunkValidated(void)
{
	m_evtChunkValid.Set();
}





void cChunkMap::QueueTickBlock(Vector3i a_AbsPos)
{
	auto chunkCoords = cChunkDef::BlockToChunk(a_AbsPos);
	auto relPos = cChunkDef::AbsoluteToRelative(a_AbsPos, chunkCoords);

	cCSLock Lock(m_CSChunks);
	cChunkPtr Chunk = GetChunkNoLoad(chunkCoords);
	if (Chunk != nullptr)
	{
		Chunk->QueueTickBlock(relPos);
	}
}





void cChunkMap::SetChunkAlwaysTicked(int a_ChunkX, int a_ChunkZ, bool a_AlwaysTicked)
{
	cCSLock Lock(m_CSChunks);
	cChunkPtr Chunk = GetChunk(a_ChunkX, a_ChunkZ);
	if (Chunk != nullptr)
	{
		Chunk->SetAlwaysTicked(a_AlwaysTicked);
	}
}





void cChunkMap::TrackInDeadlockDetect(cDeadlockDetect & a_DeadlockDetect, const AString & a_WorldName)
{
	a_DeadlockDetect.TrackCriticalSection(m_CSChunks, Printf("World %s chunkmap", a_WorldName.c_str()));
}





void cChunkMap::UntrackInDeadlockDetect(cDeadlockDetect & a_DeadlockDetect)
{
	a_DeadlockDetect.UntrackCriticalSection(m_CSChunks);
}





void cChunkMap::AddChunkStay(cChunkStay & a_ChunkStay)
{
	cCSLock Lock(m_CSChunks);

	// Add it to the list:
	ASSERT(std::find(m_ChunkStays.begin(), m_ChunkStays.end(), &a_ChunkStay) == m_ChunkStays.end());  // Has not yet been added
	m_ChunkStays.push_back(&a_ChunkStay);

	// Schedule all chunks to be loaded / generated, and mark each as locked:
	const cChunkCoordsVector & WantedChunks = a_ChunkStay.GetChunks();
	for (cChunkCoordsVector::const_iterator itr = WantedChunks.begin(); itr != WantedChunks.end(); ++itr)
	{
		cChunkPtr Chunk = GetChunk(itr->m_ChunkX, itr->m_ChunkZ);
		if (Chunk == nullptr)
		{
			continue;
		}
		Chunk->Stay(true);
		if (Chunk->IsValid())
		{
			if (a_ChunkStay.ChunkAvailable(itr->m_ChunkX, itr->m_ChunkZ))
			{
				// The chunkstay wants to be deactivated, disable it and bail out:
				a_ChunkStay.Disable();
				return;
			}
		}
	}  // for itr - WantedChunks[]
}





/** Removes the specified cChunkStay descendant from the internal list of ChunkStays. */
void cChunkMap::DelChunkStay(cChunkStay & a_ChunkStay)
{
	cCSLock Lock(m_CSChunks);

	// Remove from the list of active chunkstays:
	bool HasFound = false;
	for (cChunkStays::iterator itr = m_ChunkStays.begin(), end = m_ChunkStays.end(); itr != end; ++itr)
	{
		if (*itr == &a_ChunkStay)
		{
			m_ChunkStays.erase(itr);
			HasFound = true;
			break;
		}
	}  // for itr - m_ChunkStays[]

	if (!HasFound)
	{
		ASSERT(!"Removing a cChunkStay that hasn't been added!");
		return;
	}

	// Unmark all contained chunks:
	const cChunkCoordsVector & Chunks = a_ChunkStay.GetChunks();
	for (cChunkCoordsVector::const_iterator itr = Chunks.begin(), end = Chunks.end(); itr != end; ++itr)
	{
		cChunkPtr Chunk = GetChunkNoLoad(itr->m_ChunkX, itr->m_ChunkZ);
		if (Chunk == nullptr)
		{
			continue;
		}
		Chunk->Stay(false);
	}  // for itr - Chunks[]
	a_ChunkStay.OnDisabled();
}
