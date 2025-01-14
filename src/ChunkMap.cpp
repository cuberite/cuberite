
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
#include "Blocks/BlockHandler.h"
#include "MobCensus.h"
#include "MobSpawner.h"
#include "BoundingBox.h"
#include "SetChunkData.h"
#include "Blocks/ChunkInterface.h"
#include "Entities/Pickup.h"
#include "DeadlockDetect.h"
#include "BlockEntities/BlockEntity.h"
#include "Blocks/BlockLog.h"





////////////////////////////////////////////////////////////////////////////////
// cChunkMap:

cChunkMap::cChunkMap(cWorld * a_World) :
	m_World(a_World)
{
}





cChunk & cChunkMap::ConstructChunk(int a_ChunkX, int a_ChunkZ)
{
	// If not exists insert. Then, return the chunk at these coordinates:
	return m_Chunks.try_emplace(
		{ a_ChunkX, a_ChunkZ },
		a_ChunkX, a_ChunkZ, this, m_World
	).first->second;
}





cChunk & cChunkMap::GetChunk(int a_ChunkX, int a_ChunkZ)
{
	ASSERT(m_CSChunks.IsLockedByCurrentThread());  // m_CSChunks should already be locked by the operation that called us

	auto & Chunk = ConstructChunk(a_ChunkX, a_ChunkZ);
	if (!Chunk.IsValid() && !Chunk.IsQueued())
	{
		Chunk.SetPresence(cChunk::cpQueued);
		m_World->GetStorage().QueueLoadChunk(a_ChunkX, a_ChunkZ);
	}
	return Chunk;
}





cChunk * cChunkMap::FindChunk(int a_ChunkX, int a_ChunkZ)
{
	ASSERT(m_CSChunks.IsLockedByCurrentThread());

	const auto Chunk = m_Chunks.find({ a_ChunkX, a_ChunkZ });
	return (Chunk == m_Chunks.end()) ? nullptr : &Chunk->second;
}





const cChunk * cChunkMap::FindChunk(int a_ChunkX, int a_ChunkZ) const
{
	ASSERT(m_CSChunks.IsLockedByCurrentThread());

	const auto Chunk = m_Chunks.find({ a_ChunkX, a_ChunkZ });
	return (Chunk == m_Chunks.end()) ? nullptr : &Chunk->second;
}





void cChunkMap::SendBlockEntity(int a_BlockX, int a_BlockY, int a_BlockZ, cClientHandle & a_Client)
{
	cCSLock Lock(m_CSChunks);
	int ChunkX, ChunkZ;
	cChunkDef::BlockToChunk(a_BlockX, a_BlockZ, ChunkX, ChunkZ);
	const auto Chunk = FindChunk(ChunkX, ChunkZ);
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
	const auto Chunk = FindChunk(ChunkX, ChunkZ);
	if ((Chunk == nullptr) || !Chunk->IsValid())
	{
		return false;
	}
	return Chunk->UseBlockEntity(a_Player, a_BlockX, a_BlockY, a_BlockZ);
}





bool cChunkMap::DoWithChunk(int a_ChunkX, int a_ChunkZ, cChunkCallback a_Callback)
{
	cCSLock Lock(m_CSChunks);
	const auto Chunk = FindChunk(a_ChunkX, a_ChunkZ);
	if ((Chunk == nullptr) || !Chunk->IsValid())
	{
		return false;
	}
	return a_Callback(*Chunk);
}





bool cChunkMap::DoWithChunkAt(Vector3i a_BlockPos, cChunkCallback a_Callback)
{
	const auto Position = cChunkDef::BlockToChunk(a_BlockPos);
	return DoWithChunk(Position.m_ChunkX, Position.m_ChunkZ, a_Callback);
}





void cChunkMap::WakeUpSimulators(Vector3i a_Block)
{
	cCSLock Lock(m_CSChunks);
	const auto Position = cChunkDef::BlockToChunk(a_Block);
	const auto Chunk = FindChunk(Position.m_ChunkX, Position.m_ChunkZ);
	if ((Chunk == nullptr) || !Chunk->IsValid())
	{
		return;
	}

	m_World->GetSimulatorManager()->WakeUp(*Chunk, cChunkDef::AbsoluteToRelative(a_Block, Position));
}





void cChunkMap::MarkChunkDirty(int a_ChunkX, int a_ChunkZ)
{
	cCSLock Lock(m_CSChunks);
	const auto Chunk = FindChunk(a_ChunkX, a_ChunkZ);
	if ((Chunk == nullptr) || !Chunk->IsValid())
	{
		return;
	}
	Chunk->MarkDirty();
}





void cChunkMap::MarkChunkSaving(int a_ChunkX, int a_ChunkZ)
{
	cCSLock Lock(m_CSChunks);
	const auto Chunk = FindChunk(a_ChunkX, a_ChunkZ);
	if ((Chunk == nullptr) || !Chunk->IsValid())
	{
		return;
	}
	Chunk->MarkSaving();
}





void cChunkMap::MarkChunkSaved (int a_ChunkX, int a_ChunkZ)
{
	cCSLock Lock(m_CSChunks);
	const auto Chunk = FindChunk(a_ChunkX, a_ChunkZ);
	if ((Chunk == nullptr) || !Chunk->IsValid())
	{
		return;
	}
	Chunk->MarkSaved();
}





void cChunkMap::SetChunkData(struct SetChunkData && a_SetChunkData)
{
	const int ChunkX = a_SetChunkData.Chunk.m_ChunkX;
	const int ChunkZ = a_SetChunkData.Chunk.m_ChunkZ;
	{
		cCSLock Lock(m_CSChunks);
		const auto Chunk = FindChunk(ChunkX, ChunkZ);
		ASSERT(Chunk != nullptr);  // Chunk cannot have unloaded since it is marked as queued
		Chunk->SetAllData(std::move(a_SetChunkData));

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
	const cChunkDef::LightNibbles & a_BlockLight,
	const cChunkDef::LightNibbles & a_SkyLight
)
{
	cCSLock Lock(m_CSChunks);
	const auto Chunk = FindChunk(a_ChunkX, a_ChunkZ);
	if (Chunk == nullptr)
	{
		// Chunk probably unloaded in the meantime
		return;
	}
	Chunk->SetLight(a_BlockLight, a_SkyLight);
}





bool cChunkMap::GetChunkData(cChunkCoords a_Coords, cChunkDataCallback & a_Callback) const
{
	if (!a_Callback.Coords(a_Coords.m_ChunkX, a_Coords.m_ChunkZ))
	{
		// The callback doesn't want the data
		return false;
	}
	cCSLock Lock(m_CSChunks);
	const auto Chunk = FindChunk(a_Coords.m_ChunkX, a_Coords.m_ChunkZ);
	if ((Chunk == nullptr) || !Chunk->IsValid())
	{
		// The chunk is not present
		return false;
	}
	Chunk->GetAllData(a_Callback);
	return true;
}





bool cChunkMap::IsChunkQueued(int a_ChunkX, int a_ChunkZ) const
{
	cCSLock Lock(m_CSChunks);
	const auto Chunk = FindChunk(a_ChunkX, a_ChunkZ);
	return (Chunk != nullptr) && Chunk->IsQueued();
}





bool cChunkMap::IsWeatherSunnyAt(int a_BlockX, int a_BlockZ) const
{
	int ChunkX, ChunkZ, BlockY = 0;
	cChunkDef::AbsoluteToRelative(a_BlockX, BlockY, a_BlockZ, ChunkX, ChunkZ);

	cCSLock Lock(m_CSChunks);
	const auto Chunk = FindChunk(ChunkX, ChunkZ);
	if ((Chunk == nullptr) || !Chunk->IsValid())
	{
		return m_World->IsWeatherSunny();
	}

	return Chunk->IsWeatherSunnyAt(a_BlockX, a_BlockZ);
}





bool cChunkMap::IsWeatherWetAt(int a_BlockX, int a_BlockZ) const
{
	int ChunkX, ChunkZ, BlockY = 0;
	cChunkDef::AbsoluteToRelative(a_BlockX, BlockY, a_BlockZ, ChunkX, ChunkZ);

	cCSLock Lock(m_CSChunks);
	const auto Chunk = FindChunk(ChunkX, ChunkZ);
	if ((Chunk == nullptr) || !Chunk->IsValid())
	{
		return m_World->IsWeatherWet();
	}

	return Chunk->IsWeatherWetAt(a_BlockX, a_BlockZ);
}





bool cChunkMap::IsWeatherWetAt(const Vector3i a_Position) const
{
	const auto ChunkPosition = cChunkDef::BlockToChunk(a_Position);
	const auto Position = cChunkDef::AbsoluteToRelative(a_Position, ChunkPosition);

	cCSLock Lock(m_CSChunks);
	const auto Chunk = FindChunk(ChunkPosition.m_ChunkX, ChunkPosition.m_ChunkZ);
	if ((Chunk == nullptr) || !Chunk->IsValid())
	{
		return m_World->IsWeatherWet();
	}

	return Chunk->IsWeatherWetAt(Position);
}





bool cChunkMap::IsChunkValid(int a_ChunkX, int a_ChunkZ) const
{
	cCSLock Lock(m_CSChunks);
	const auto Chunk = FindChunk(a_ChunkX, a_ChunkZ);
	return (Chunk != nullptr) && Chunk->IsValid();
}





bool cChunkMap::HasChunkAnyClients(int a_ChunkX, int a_ChunkZ) const
{
	cCSLock Lock(m_CSChunks);
	const auto Chunk = FindChunk(a_ChunkX, a_ChunkZ);
	return (Chunk != nullptr) && Chunk->HasAnyClients();
}





std::optional<int> cChunkMap::GetHeight(int a_BlockX, int a_BlockZ)
{
	// Returns std::nullopt if chunk not loaded / generated.
	cCSLock Lock(m_CSChunks);
	int ChunkX, ChunkZ, BlockY = 0;
	cChunkDef::AbsoluteToRelative(a_BlockX, BlockY, a_BlockZ, ChunkX, ChunkZ);
	const auto Chunk = FindChunk(ChunkX, ChunkZ);
	if ((Chunk == nullptr) || !Chunk->IsValid())
	{
		return std::nullopt;
	}

	return Chunk->GetHeight(a_BlockX, a_BlockZ);
}





void cChunkMap::FastSetBlock(Vector3i a_BlockPos, BlockState a_Block)
{
	auto chunkPos = cChunkDef::BlockToChunk(a_BlockPos);
	auto RelPos = cChunkDef::AbsoluteToRelative(a_BlockPos, chunkPos);

	cCSLock Lock(m_CSChunks);
	const auto Chunk = FindChunk(chunkPos.m_ChunkX, chunkPos.m_ChunkZ);
	if ((Chunk != nullptr) && Chunk->IsValid())
	{
		Chunk->FastSetBlock(RelPos, a_Block);
	}
}





void cChunkMap::CollectPickupsByEntity(cEntity & a_Entity)
{
	cCSLock Lock(m_CSChunks);

	auto BoundingBox = a_Entity.GetBoundingBox();
	BoundingBox.Expand(1, 0.5, 1);

	ForEachEntityInBox(BoundingBox, [&a_Entity](cEntity & Entity)
	{
		// Only pickups and projectiles can be picked up:
		if (Entity.IsPickup())
		{
			/*
			LOG("Pickup %d being collected by player \"%s\", distance %f",
				(*itr)->GetUniqueID(), a_Player->GetName().c_str(), SqrDist
			);
			*/
			static_cast<cPickup &>(Entity).CollectedBy(a_Entity);
		}
		else if (Entity.IsProjectile() && a_Entity.IsPlayer())
		{
			static_cast<cProjectileEntity &>(Entity).CollectedBy(static_cast<cPlayer&>(a_Entity));
		}

		// The entities will MarkDirty when they Destroy themselves
		return false;
	});
}





BlockState cChunkMap::GetBlock(Vector3i a_BlockPos) const
{
	if (!cChunkDef::IsValidHeight(a_BlockPos))
	{
		return 0;
	}

	auto chunkPos = cChunkDef::BlockToChunk(a_BlockPos);
	auto RelPos = cChunkDef::AbsoluteToRelative(a_BlockPos, chunkPos);

	// Query the chunk, if loaded:
	cCSLock Lock(m_CSChunks);
	const auto Chunk = FindChunk(chunkPos.m_ChunkX, chunkPos.m_ChunkZ);
	if ((Chunk != nullptr) && Chunk->IsValid())
	{
		return Chunk->GetBlock(RelPos);
	}
	return 0;
}





LIGHTTYPE cChunkMap::GetBlockSkyLight(Vector3i a_BlockPos) const
{
	if (!cChunkDef::IsValidHeight(a_BlockPos))
	{
		return 0;
	}

	auto chunkPos = cChunkDef::BlockToChunk(a_BlockPos);
	auto RelPos = cChunkDef::AbsoluteToRelative(a_BlockPos, chunkPos);

	// Query the chunk, if loaded:
	cCSLock Lock(m_CSChunks);
	const auto Chunk = FindChunk(chunkPos.m_ChunkX, chunkPos.m_ChunkZ);
	if ((Chunk != nullptr) && Chunk->IsValid())
	{
		return Chunk->GetSkyLight(RelPos);
	}
	return 0;
}





LIGHTTYPE cChunkMap::GetBlockBlockLight(Vector3i a_BlockPos) const
{
	auto chunkPos = cChunkDef::BlockToChunk(a_BlockPos);
	auto RelPos = cChunkDef::AbsoluteToRelative(a_BlockPos, chunkPos);

	// Query the chunk, if loaded:
	cCSLock Lock(m_CSChunks);
	const auto Chunk = FindChunk(chunkPos.m_ChunkX, chunkPos.m_ChunkZ);
	if ((Chunk != nullptr) && Chunk->IsValid())
	{
		return Chunk->GetBlockLight(RelPos);
	}
	return 0;
}





void cChunkMap::SetBlock(Vector3i a_BlockPos, BlockState a_Block)
{
	auto chunkPos = cChunkDef::BlockToChunk(a_BlockPos);
	auto RelPos = cChunkDef::AbsoluteToRelative(a_BlockPos, chunkPos);

	cCSLock Lock(m_CSChunks);
	const auto Chunk = FindChunk(chunkPos.m_ChunkX, chunkPos.m_ChunkZ);
	if ((Chunk != nullptr) && Chunk->IsValid())
	{
		Chunk->SetBlock(RelPos, a_Block);
	}
}





bool cChunkMap::GetBlock(Vector3i a_BlockPos, BlockState & a_Block) const
{
	if (!cChunkDef::IsValidHeight(a_BlockPos))
	{
		// Initialise the params to fulfil our contract.
		a_Block= 0;
		return false;
	}

	auto chunkCoord = cChunkDef::BlockToChunk(a_BlockPos);
	auto RelPos = cChunkDef::AbsoluteToRelative(a_BlockPos, chunkCoord);

	cCSLock Lock(m_CSChunks);
	const auto Chunk = FindChunk(chunkCoord.m_ChunkX, chunkCoord.m_ChunkZ);
	if ((Chunk != nullptr) && Chunk->IsValid())
	{
		a_Block = Chunk->GetBlock(RelPos);
		return true;
	}

	// Initialise the params to fulfil our contract.
	a_Block= 0;
	return false;
}





bool cChunkMap::GetBlockInfo(Vector3i a_BlockPos, BlockState & a_Block, LIGHTTYPE & a_SkyLight, LIGHTTYPE & a_BlockLight) const
{
	auto chunkPos = cChunkDef::BlockToChunk(a_BlockPos);
	auto RelPos = cChunkDef::AbsoluteToRelative(a_BlockPos, chunkPos);

	// Query the chunk, if loaded:
	cCSLock Lock(m_CSChunks);
	const auto Chunk = FindChunk(chunkPos.m_ChunkX, chunkPos.m_ChunkZ);
	if ((Chunk != nullptr) && Chunk->IsValid())
	{
		Chunk->GetBlockInfo(RelPos, a_Block, a_SkyLight, a_BlockLight);
		return true;
	}
	return false;
}





void cChunkMap::ReplaceTreeBlocks(const sSetBlockVector & a_Blocks)
{
	cCSLock Lock(m_CSChunks);
	for (const auto & TreeBlock : a_Blocks)
	{
		const auto Chunk = FindChunk(TreeBlock.m_ChunkX, TreeBlock.m_ChunkZ);
		if ((Chunk == nullptr) || !Chunk->IsValid())
		{
			continue;
		}
		Vector3i RelPos(TreeBlock.m_RelX, TreeBlock.m_RelY, TreeBlock.m_RelZ);
		switch (Chunk->GetBlock(RelPos).Type())
		{
			CASE_TREE_OVERWRITTEN_BLOCKS:
			{
				Chunk->SetBlock(RelPos, TreeBlock.m_Block);
				break;
			}
			case BlockType::AcaciaLeaves:
			case BlockType::BirchLeaves:
			case BlockType::DarkOakLeaves:
			case BlockType::JungleLeaves:
			case BlockType::OakLeaves:
			case BlockType::SpruceLeaves:
			{

				if (cBlockLogHandler::IsBlockLog(TreeBlock.m_Block))
				{
					Chunk->SetBlock(RelPos, TreeBlock.m_Block);
				}
				break;
			}
			default: break;
		}
	}  // for itr - a_Blocks[]
}





EMCSBiome cChunkMap::GetBiomeAt(int a_BlockX, int a_BlockZ) const
{
	int ChunkX, ChunkZ, X = a_BlockX, Y = 0, Z = a_BlockZ;
	cChunkDef::AbsoluteToRelative(X, Y, Z, ChunkX, ChunkZ);

	cCSLock Lock(m_CSChunks);
	const auto Chunk = FindChunk(ChunkX, ChunkZ);
	if ((Chunk != nullptr) && Chunk->IsValid())
	{
		return Chunk->GetBiomeAt(X, Z);
	}
	return EMCSBiome::biInvalidBiome;
}





bool cChunkMap::SetBiomeAt(int a_BlockX, int a_BlockZ, EMCSBiome a_Biome)
{
	int ChunkX, ChunkZ, X = a_BlockX, Y = 0, Z = a_BlockZ;
	cChunkDef::AbsoluteToRelative(X, Y, Z, ChunkX, ChunkZ);

	cCSLock Lock(m_CSChunks);
	const auto Chunk = FindChunk(ChunkX, ChunkZ);
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
			const auto Chunk = FindChunk(x, z);
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
	for (auto & Block : a_Blocks)
	{
		const auto Chunk = FindChunk(Block.m_ChunkX, Block.m_ChunkZ);
		if ((Chunk == nullptr) || !Chunk->IsValid())
		{
			if (!a_ContinueOnFailure)
			{
				return false;
			}
			res = false;
			continue;
		}
		if (!cChunkDef::IsValidHeight(Block.GetRelativePos()))
		{
			continue;
		}
		Block.m_Block = Chunk->GetBlock(Block.m_RelX, Block.m_RelY, Block.m_RelZ);
	}
	return res;
}





bool cChunkMap::DigBlock(Vector3i a_BlockPos)
{
	auto ChunkCoords = cChunkDef::BlockToChunk(a_BlockPos);
	auto RelPos = cChunkDef::AbsoluteToRelative(a_BlockPos, ChunkCoords);

	{
		cCSLock Lock(m_CSChunks);
		const auto Chunk = FindChunk(ChunkCoords.m_ChunkX, ChunkCoords.m_ChunkZ);
		if ((Chunk == nullptr) || !Chunk->IsValid())
		{
			return false;
		}

		Chunk->SetBlock(RelPos, Block::Air::Air());
	}
	return true;
}





cItems cChunkMap::PickupsFromBlock(Vector3i a_BlockPos, const cEntity * a_Digger, const cItem * a_Tool)
{
	auto ChunkCoords = cChunkDef::BlockToChunk(a_BlockPos);
	auto RelPos = cChunkDef::AbsoluteToRelative(a_BlockPos, ChunkCoords);

	cCSLock Lock(m_CSChunks);
	const auto Chunk = FindChunk(ChunkCoords.m_ChunkX, ChunkCoords.m_ChunkZ);
	if ((Chunk == nullptr) || !Chunk->IsValid())
	{
		return {};
	}
	return Chunk->PickupsFromBlock(RelPos, a_Digger, a_Tool);
}





void cChunkMap::SendBlockTo(int a_X, int a_Y, int a_Z, const cPlayer & a_Player)
{
	int ChunkX, ChunkZ;
	cChunkDef::AbsoluteToRelative(a_X, a_Y, a_Z, ChunkX, ChunkZ);

	cCSLock Lock(m_CSChunks);
	const auto Chunk = FindChunk(ChunkX, ChunkZ);
	if ((Chunk != nullptr) && (Chunk->IsValid()))
	{
		Chunk->SendBlockTo(a_X, a_Y, a_Z, a_Player.GetClientHandle());
	}
}





void cChunkMap::CompareChunkClients(int a_ChunkX1, int a_ChunkZ1, int a_ChunkX2, int a_ChunkZ2, cClientDiffCallback & a_Callback)
{
	cCSLock Lock(m_CSChunks);
	const auto Chunk1 = FindChunk(a_ChunkX1, a_ChunkZ1);
	if (Chunk1 == nullptr)
	{
		return;
	}
	const auto Chunk2 = FindChunk(a_ChunkX2, a_ChunkZ2);
	if (Chunk2 == nullptr)
	{
		return;
	}

	CompareChunkClients(Chunk1, Chunk2, a_Callback);
}





void cChunkMap::CompareChunkClients(cChunk * a_Chunk1, cChunk * a_Chunk2, cClientDiffCallback & a_Callback)
{
	const auto & Clients1 = a_Chunk1->GetAllClients();
	const auto & Clients2 = a_Chunk2->GetAllClients();

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
	return GetChunk(a_ChunkX, a_ChunkZ).AddClient(a_Client);
}





void cChunkMap::RemoveChunkClient(int a_ChunkX, int a_ChunkZ, cClientHandle * a_Client)
{
	cCSLock Lock(m_CSChunks);
	const auto Chunk = FindChunk(a_ChunkX, a_ChunkZ);
	ASSERT(Chunk != nullptr);
	Chunk->RemoveClient(a_Client);
}





void cChunkMap::RemoveClientFromChunks(cClientHandle * a_Client)
{
	cCSLock Lock(m_CSChunks);
	for (auto & Chunk : m_Chunks)
	{
		Chunk.second.RemoveClient(a_Client);
	}
}





void cChunkMap::AddEntity(OwnedEntity a_Entity)
{
	cCSLock Lock(m_CSChunks);
	if (FindChunk(a_Entity->GetChunkX(), a_Entity->GetChunkZ()) == nullptr)
	{
		LOGWARNING("%s: Entity at %p (%s, ID %d) spawning in a non-existent chunk.",
			__FUNCTION__, static_cast<void *>(a_Entity.get()), a_Entity->GetClass(), a_Entity->GetUniqueID()
		);
	}

	const auto EntityPtr = a_Entity.get();
	ASSERT(EntityPtr->GetWorld() == m_World);

	auto & Chunk = ConstructChunk(a_Entity->GetChunkX(), a_Entity->GetChunkZ());
	Chunk.AddEntity(std::move(a_Entity));

	EntityPtr->OnAddToWorld(*m_World);
	ASSERT(!EntityPtr->IsTicking());
	EntityPtr->SetIsTicking(true);

	cPluginManager::Get()->CallHookSpawnedEntity(*m_World, *EntityPtr);
}





void cChunkMap::AddPlayer(std::unique_ptr<cPlayer> a_Player)
{
	cCSLock Lock(m_CSChunks);
	auto & Chunk = ConstructChunk(a_Player->GetChunkX(), a_Player->GetChunkZ());  // Always construct the chunk for players
	ASSERT(!Chunk.HasEntity(a_Player->GetUniqueID()));
	Chunk.AddEntity(std::move(a_Player));
}





bool cChunkMap::HasEntity(UInt32 a_UniqueID) const
{
	cCSLock Lock(m_CSChunks);
	for (const auto & Chunk : m_Chunks)
	{
		if (Chunk.second.IsValid() && Chunk.second.HasEntity(a_UniqueID))
		{
			return true;
		}
	}
	return false;
}





OwnedEntity cChunkMap::RemoveEntity(cEntity & a_Entity)
{
	cCSLock Lock(m_CSChunks);
	const auto Chunk = a_Entity.GetParentChunk();

	if (Chunk == nullptr)
	{
		return nullptr;
	}

	// Remove the entity no matter whether the chunk itself is valid or not (#1190)
	return Chunk->RemoveEntity(a_Entity);
}





bool cChunkMap::ForEachEntity(cEntityCallback a_Callback) const
{
	cCSLock Lock(m_CSChunks);
	for (const auto & Chunk : m_Chunks)
	{
		if (Chunk.second.IsValid() && !Chunk.second.ForEachEntity(a_Callback))
		{
			return false;
		}
	}
	return true;
}





bool cChunkMap::ForEachEntityInChunk(int a_ChunkX, int a_ChunkZ, cEntityCallback a_Callback)
{
	cCSLock Lock(m_CSChunks);
	const auto Chunk = FindChunk(a_ChunkX, a_ChunkZ);
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
			const auto Chunk = FindChunk(x, z);
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





bool cChunkMap::DoWithEntityByID(UInt32 a_UniqueID, cEntityCallback a_Callback) const
{
	cCSLock Lock(m_CSChunks);
	bool res = false;
	for (const auto & Chunk : m_Chunks)
	{
		if (Chunk.second.IsValid() && Chunk.second.DoWithEntityByID(a_UniqueID, a_Callback, res))
		{
			return res;
		}
	}
	return false;
}





bool cChunkMap::ForEachBlockEntityInChunk(int a_ChunkX, int a_ChunkZ, cBlockEntityCallback a_Callback)
{
	cCSLock Lock(m_CSChunks);
	const auto Chunk = FindChunk(a_ChunkX, a_ChunkZ);
	if ((Chunk == nullptr) || !Chunk->IsValid())
	{
		return false;
	}
	return Chunk->ForEachBlockEntity(a_Callback);
}





bool cChunkMap::DoWithBlockEntityAt(const Vector3i a_Position, cBlockEntityCallback a_Callback)
{
	const auto ChunkPosition = cChunkDef::BlockToChunk(a_Position);
	const auto Relative = cChunkDef::AbsoluteToRelative(a_Position, ChunkPosition);
	cCSLock Lock(m_CSChunks);
	const auto Chunk = FindChunk(ChunkPosition.m_ChunkX, ChunkPosition.m_ChunkZ);
	if ((Chunk == nullptr) || !Chunk->IsValid())
	{
		return false;
	}
	return Chunk->DoWithBlockEntityAt(Relative, a_Callback);
}





void cChunkMap::PrepareChunk(int a_ChunkX, int a_ChunkZ, std::unique_ptr<cChunkCoordCallback> a_Callback)
{
	cCSLock Lock(m_CSChunks);
	const auto Chunk = FindChunk(a_ChunkX, a_ChunkZ);

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





void cChunkMap::GenerateChunk(int a_ChunkX, int a_ChunkZ)
{
	cCSLock Lock(m_CSChunks);
	GetChunk(a_ChunkX, a_ChunkZ);  // Touches the chunk, loading or generating it
}





void cChunkMap::ChunkLoadFailed(int a_ChunkX, int a_ChunkZ)
{
	cCSLock Lock(m_CSChunks);
	const auto Chunk = FindChunk(a_ChunkX, a_ChunkZ);
	ASSERT(Chunk != nullptr);  // Chunk cannot have unloaded since it is marked as queued
	Chunk->MarkLoadFailed();
}





void cChunkMap::MarkChunkRegenerating(int a_ChunkX, int a_ChunkZ)
{
	cCSLock Lock(m_CSChunks);
	ConstructChunk(a_ChunkX, a_ChunkZ).MarkRegenerating();
}





bool cChunkMap::IsChunkLighted(int a_ChunkX, int a_ChunkZ)
{
	cCSLock Lock(m_CSChunks);
	const auto Chunk = FindChunk(a_ChunkX, a_ChunkZ);
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
			const auto Chunk = FindChunk(x, z);
			if ((Chunk == nullptr) || !Chunk->IsValid())
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





bool cChunkMap::ForEachLoadedChunk(cFunctionRef<bool(int, int)> a_Callback) const
{
	cCSLock Lock(m_CSChunks);
	for (const auto & Chunk : m_Chunks)
	{
		if (Chunk.second.IsValid())
		{
			if (a_Callback(Chunk.first.m_ChunkX, Chunk.first.m_ChunkZ))
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
			const auto Chunk = FindChunk(x, z);
			if ((Chunk == nullptr) || !Chunk->IsValid())
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





void cChunkMap::GetChunkStats(int & a_NumChunksValid, int & a_NumChunksDirty) const
{
	a_NumChunksValid = 0;
	a_NumChunksDirty = 0;
	cCSLock Lock(m_CSChunks);
	for (const auto & Chunk : m_Chunks)
	{
		a_NumChunksValid++;
		if (Chunk.second.IsDirty())
		{
			a_NumChunksDirty++;
		}
	}
}





int cChunkMap::GrowPlantAt(Vector3i a_BlockPos, char a_NumStages)
{
	auto chunkPos = cChunkDef::BlockToChunk(a_BlockPos);
	auto RelPos = cChunkDef::AbsoluteToRelative(a_BlockPos, chunkPos);
	cCSLock lock(m_CSChunks);
	const auto Chunk = FindChunk(chunkPos.m_ChunkX, chunkPos.m_ChunkZ);
	if ((Chunk == nullptr) || !Chunk->IsValid())
	{
		return 0;
	}
	return Chunk->GrowPlantAt(RelPos, a_NumStages);
}





void cChunkMap::SetNextBlockToTick(const Vector3i a_BlockPos)
{
	auto ChunkPos = cChunkDef::BlockToChunk(a_BlockPos);
	auto RelPos = cChunkDef::AbsoluteToRelative(a_BlockPos, ChunkPos);

	cCSLock Lock(m_CSChunks);
	const auto Chunk = FindChunk(ChunkPos.m_ChunkX, ChunkPos.m_ChunkZ);
	if (Chunk != nullptr)
	{
		Chunk->SetNextBlockToTick(RelPos);
	}
}





void cChunkMap::CollectMobCensus(cMobCensus & a_ToFill)
{
	cCSLock Lock(m_CSChunks);
	for (auto & Chunk : m_Chunks)
	{
		// We do count every Mobs in the world. But we are assuming that every chunk not loaded by any client
		// doesn't affect us. Normally they should not have mobs because every "too far" mobs despawn
		// If they have (f.i. when player disconnect) we assume we don't have to make them live or despawn
		if (Chunk.second.IsValid() && Chunk.second.HasAnyClients())
		{
			Chunk.second.CollectMobCensus(a_ToFill);
		}
	}
}





void cChunkMap::SpawnMobs(cMobSpawner & a_MobSpawner)
{
	cCSLock Lock(m_CSChunks);
	for (auto & Chunk : m_Chunks)
	{
		// We only spawn close to players
		if (Chunk.second.IsValid() && Chunk.second.HasAnyClients())
		{
			Chunk.second.SpawnMobs(a_MobSpawner);
		}
	}
}





void cChunkMap::Tick(std::chrono::milliseconds a_Dt)
{
	cCSLock Lock(m_CSChunks);

	// Do the magic of updating the world:
	for (auto & Chunk : m_Chunks)
	{
		if (Chunk.second.ShouldBeTicked())
		{
			Chunk.second.Tick(a_Dt);
		}
	}

	// Finally, only after all chunks are ticked, tell the client about all aggregated changes:
	for (auto & Chunk : m_Chunks)
	{
		Chunk.second.BroadcastPendingChanges();
	}
}





void cChunkMap::FlushPendingBlockChanges()
{
	for (auto & Chunk : m_Chunks)
	{
		Chunk.second.BroadcastPendingChanges();
	}
}





void cChunkMap::TickBlock(const Vector3i a_BlockPos)
{
	auto ChunkPos = cChunkDef::BlockToChunk(a_BlockPos);
	auto RelPos = cChunkDef::AbsoluteToRelative(a_BlockPos, ChunkPos);
	cCSLock Lock(m_CSChunks);
	const auto Chunk = FindChunk(ChunkPos.m_ChunkX, ChunkPos.m_ChunkZ);
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
			itr->second.CanUnload() &&  // Can unload
			!cPluginManager::Get()->CallHookChunkUnloading(*GetWorld(), itr->first.m_ChunkX, itr->first.m_ChunkZ)  // Plugins agree
		)
		{
			// First notify plugins:
			cPluginManager::Get()->CallHookChunkUnloaded(*m_World, itr->first.m_ChunkX, itr->first.m_ChunkZ);

			// Notify entities within the chunk, while everything's still valid:
			itr->second.OnUnload();

			// Kill the chunk:
			itr = m_Chunks.erase(itr);
		}
		else
		{
			++itr;
		}
	}
}





void cChunkMap::SaveAllChunks(void) const
{
	cCSLock Lock(m_CSChunks);
	for (const auto & Chunk : m_Chunks)
	{
		if (Chunk.second.IsValid() && Chunk.second.IsDirty())
		{
			GetWorld()->GetStorage().QueueSaveChunk(Chunk.first.m_ChunkX, Chunk.first.m_ChunkZ);
		}
	}
}





size_t cChunkMap::GetNumChunks(void) const
{
	cCSLock Lock(m_CSChunks);
	return m_Chunks.size();
}





size_t cChunkMap::GetNumUnusedDirtyChunks(void) const
{
	cCSLock Lock(m_CSChunks);
	size_t res = 0;
	for (const auto & Chunk : m_Chunks)
	{
		if (Chunk.second.IsValid() && Chunk.second.CanUnloadAfterSaving())
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





void cChunkMap::SetChunkAlwaysTicked(int a_ChunkX, int a_ChunkZ, bool a_AlwaysTicked)
{
	cCSLock Lock(m_CSChunks);
	GetChunk(a_ChunkX, a_ChunkZ).SetAlwaysTicked(a_AlwaysTicked);
}





void cChunkMap::TrackInDeadlockDetect(cDeadlockDetect & a_DeadlockDetect, const AString & a_WorldName)
{
	a_DeadlockDetect.TrackCriticalSection(m_CSChunks, fmt::format(FMT_STRING("World {} chunkmap"), a_WorldName));
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
		auto & Chunk = GetChunk(itr->m_ChunkX, itr->m_ChunkZ);
		Chunk.Stay(true);
		if (Chunk.IsValid())
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
		const auto Chunk = FindChunk(itr->m_ChunkX, itr->m_ChunkZ);
		ASSERT(Chunk != nullptr);  // Stayed chunks cannot unload
		Chunk->Stay(false);
	}  // for itr - Chunks[]
	a_ChunkStay.OnDisabled();
}
