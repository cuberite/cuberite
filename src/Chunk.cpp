
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#ifndef _WIN32
	#include <cstdlib>
#endif


#include "Chunk.h"
#include "BlockInfo.h"
#include "World.h"
#include "ClientHandle.h"
#include "Server.h"
#include "Defines.h"
#include "Entities/Pickup.h"
#include "Item.h"
#include "Noise/Noise.h"
#include "Root.h"
#include "Entities/Player.h"
#include "BlockArea.h"
#include "Bindings/PluginManager.h"
#include "Blocks/BlockHandler.h"
#include "Simulator/FluidSimulator.h"
#include "Simulator/RedstoneSimulator.h"
#include "MobCensus.h"
#include "MobSpawner.h"
#include "BlockInServerPluginInterface.h"
#include "SetChunkData.h"
#include "BoundingBox.h"
#include "Blocks/ChunkInterface.h"
#include "Blocks/BlockSnow.h"
#include "Blocks/BlockLeaves.h"
#include "Blocks/BlockAir.h"
#include "Blocks/BlockSignPost.h"
#include "Blocks/BlockWallSign.h"

#include "json/json.h"





////////////////////////////////////////////////////////////////////////////////
// cChunk:

cChunk::cChunk(
	int a_ChunkX, int a_ChunkZ,
	cChunkMap * a_ChunkMap, cWorld * a_World
):
	m_Presence(cpInvalid),
	m_IsLightValid(false),
	m_IsDirty(false),
	m_IsSaving(false),
	m_StayCount(0),
	m_PosX(a_ChunkX),
	m_PosZ(a_ChunkZ),
	m_World(a_World),
	m_ChunkMap(a_ChunkMap),
	m_WaterSimulatorData(a_World->GetWaterSimulator()->CreateChunkData()),
	m_LavaSimulatorData (a_World->GetLavaSimulator ()->CreateChunkData()),
	m_RedstoneSimulatorData(a_World->GetRedstoneSimulator()->CreateChunkData()),
	m_AlwaysTicked(0)
{
	m_NeighborXM = a_ChunkMap->FindChunk(a_ChunkX - 1, a_ChunkZ);
	m_NeighborXP = a_ChunkMap->FindChunk(a_ChunkX + 1, a_ChunkZ);
	m_NeighborZM = a_ChunkMap->FindChunk(a_ChunkX, a_ChunkZ - 1);
	m_NeighborZP = a_ChunkMap->FindChunk(a_ChunkX, a_ChunkZ + 1);

	if (m_NeighborXM != nullptr)
	{
		m_NeighborXM->m_NeighborXP = this;
	}
	if (m_NeighborXP != nullptr)
	{
		m_NeighborXP->m_NeighborXM = this;
	}
	if (m_NeighborZM != nullptr)
	{
		m_NeighborZM->m_NeighborZP = this;
	}
	if (m_NeighborZP != nullptr)
	{
		m_NeighborZP->m_NeighborZM = this;
	}
}





cChunk::~cChunk()
{
	// LOGINFO("### delete cChunk() (%i, %i) from %p, thread 0x%x ###", m_PosX, m_PosZ, this, GetCurrentThreadId());

	// Inform our neighbours that we're no longer valid:
	if (m_NeighborXM != nullptr)
	{
		m_NeighborXM->m_NeighborXP = nullptr;
	}
	if (m_NeighborXP != nullptr)
	{
		m_NeighborXP->m_NeighborXM = nullptr;
	}
	if (m_NeighborZM != nullptr)
	{
		m_NeighborZM->m_NeighborZP = nullptr;
	}
	if (m_NeighborZP != nullptr)
	{
		m_NeighborZP->m_NeighborZM = nullptr;
	}

	delete m_WaterSimulatorData;
	m_WaterSimulatorData = nullptr;
	delete m_LavaSimulatorData;
	m_LavaSimulatorData = nullptr;
	delete m_RedstoneSimulatorData;
	m_RedstoneSimulatorData = nullptr;
}





void cChunk::BroadcastPendingChanges(void)
{
	if (const auto PendingBlocksCount = m_PendingSendBlocks.size(); PendingBlocksCount >= 10240)
	{
		// Resend the full chunk:
		for (const auto ClientHandle : m_LoadedByClient)
		{
			m_World->ForceSendChunkTo(m_PosX, m_PosZ, cChunkSender::Priority::Medium, ClientHandle);
		}
	}
	else if (PendingBlocksCount == 0)
	{
		// Only send block entity changes:
		for (const auto ClientHandle : m_LoadedByClient)
		{
			for (const auto BlockEntity : m_PendingSendBlockEntities)
			{
				BlockEntity->SendTo(*ClientHandle);
			}
		}
	}
	else
	{
		// Send block and block entity changes:
		for (const auto ClientHandle : m_LoadedByClient)
		{
			ClientHandle->SendBlockChanges(m_PosX, m_PosZ, m_PendingSendBlocks);

			for (const auto BlockEntity : m_PendingSendBlockEntities)
			{
				BlockEntity->SendTo(*ClientHandle);
			}
		}
	}

	m_PendingSendBlocks.clear();
	m_PendingSendBlockEntities.clear();
}





void cChunk::SetPresence(cChunk::ePresence a_Presence)
{
	m_Presence = a_Presence;
	if (a_Presence == cpPresent)
	{
		m_World->GetChunkMap()->ChunkValidated();
	}
}





void cChunk::MarkRegenerating(void)
{
	// Set as queued again:
	SetPresence(cpQueued);

	// Tell all clients attached to this Chunk that they want this Chunk:
	for (auto ClientHandle : m_LoadedByClient)
	{
		ClientHandle->AddWantedChunk(m_PosX, m_PosZ);
	}  // for itr - m_LoadedByClient[]
}





bool cChunk::HasPlayerEntities() const
{
	return std::any_of(
		m_Entities.begin(), m_Entities.end(),
		[](const auto & Entity)
		{
			return Entity->IsPlayer();
		}
	);
}





bool cChunk::CanUnload(void) const
{
	return
		m_LoadedByClient.empty() &&  // The Chunk is not used by any client
		!HasPlayerEntities() &&      // Ensure not only the absence of ClientHandlers, but also of cPlayer objects
		!m_IsDirty &&                // The Chunk has been saved properly or hasn't been touched since the load / gen
		(m_StayCount == 0) &&        // The Chunk is not in a ChunkStay
		(m_Presence != cpQueued) ;   // The Chunk is not queued for loading / generating (otherwise multi-load / multi-gen could occur)
}





bool cChunk::CanUnloadAfterSaving(void) const
{
	return
		m_LoadedByClient.empty() &&  // The Chunk is not used by any client
		!HasPlayerEntities() &&      // Ensure not only the absence of ClientHandlers, but also of cPlayer objects
		m_IsDirty &&                 // The Chunk is dirty
		(m_StayCount == 0) &&        // The Chunk is not in a ChunkStay
		(m_Presence != cpQueued) ;   // The Chunk is not queued for loading / generating (otherwise multi-load / multi-gen could occur)
}





void cChunk::OnUnload()
{
	// Note: this is only called during normal operation, not during shutdown

	// Notify all entities of imminent unload:
	for (auto & Entity : m_Entities)
	{
		// Chunks cannot be unloaded when they still contain players:
		ASSERT(!Entity->IsPlayer());

		// Notify the entity:
		Entity->OnRemoveFromWorld(*Entity->GetWorld());
	}

	// Notify all block entities of imminent unload:
	for (auto & KeyPair : m_BlockEntities)
	{
		KeyPair.second->OnRemoveFromWorld();
	}
}





void cChunk::MarkSaving(void)
{
	m_IsSaving = true;
}





void cChunk::MarkSaved(void)
{
	if (!m_IsSaving)
	{
		return;
	}
	m_IsDirty = false;
}





void cChunk::MarkLoaded(void)
{
	m_IsDirty = false;
	SetPresence(cpPresent);
}





void cChunk::MarkLoadFailed(void)
{
	ASSERT(m_Presence == cpQueued);

	// Mark dirty before generating, so that we get saved and don't have to later generate again:
	MarkDirty();

	// The Chunk is always needed, generate it:
	m_World->GetGenerator().QueueGenerateChunk({ m_PosX, m_PosZ }, false);
}





void cChunk::GetAllData(cChunkDataCallback & a_Callback) const
{
	ASSERT(m_Presence == cpPresent);

	a_Callback.LightIsValid(m_IsLightValid);
	a_Callback.ChunkData(m_BlockData, m_LightData);
	a_Callback.HeightMap(m_HeightMap);
	a_Callback.BiomeMap(m_BiomeMap);

	for (const auto & Entity : m_Entities)
	{
		a_Callback.Entity(Entity.get());
	}

	for (auto & KeyPair : m_BlockEntities)
	{
		a_Callback.BlockEntity(KeyPair.second.get());
	}
}





void cChunk::SetAllData(SetChunkData && a_SetChunkData)
{
	std::copy_n(a_SetChunkData.HeightMap.data(), a_SetChunkData.HeightMap.size(), m_HeightMap.data());
	std::copy_n(a_SetChunkData.BiomeMap.data(), a_SetChunkData.BiomeMap.size(), m_BiomeMap.data());

	m_BlockData = std::move(a_SetChunkData.BlockData);
	m_LightData = std::move(a_SetChunkData.LightData);
	m_IsLightValid = a_SetChunkData.IsLightValid;

	m_PendingSendBlocks.clear();
	m_PendingSendBlockEntities.clear();

	// Entities need some extra steps to destroy, so here we're keeping the old ones.
	// Move the entities already in the Chunk, including player entities, so that we don't lose any:
	a_SetChunkData.Entities.insert(
		a_SetChunkData.Entities.end(),
		std::make_move_iterator(m_Entities.begin()),
		std::make_move_iterator(m_Entities.end())
	);

	// Store the augmented result:
	m_Entities = std::move(a_SetChunkData.Entities);

	// Set all the entity variables again:
	for (const auto & Entity : m_Entities)
	{
		Entity->SetWorld(m_World);
		Entity->SetParentChunk(this);
		Entity->SetIsTicking(true);
	}

	// Remove the block entities present - either the loader / saver has better, or we'll create empty ones:
	for (auto & KeyPair : m_BlockEntities)
	{
		KeyPair.second->Destroy();
		KeyPair.second->OnRemoveFromWorld();
	}

	// Clear the old ones:
	m_BlockEntities = std::move(a_SetChunkData.BlockEntities);

	// Check that all block entities have a valid blocktype at their respective coords (DEBUG-mode only):

	for (auto & KeyPair : m_BlockEntities)
	{
#ifndef NDEBUG
		cBlockEntity * BlockEntity = KeyPair.second.get();
		auto EntityBlockType = BlockEntity->GetBlockType();
		auto WorldBlockType = GetBlock(BlockEntity->GetRelPos()).Type();
		ASSERT(WorldBlockType == EntityBlockType);
#endif
		// Reset Pointer
		KeyPair.second->SetWorld(nullptr);

		auto Pos = cChunkDef::RelativeToAbsolute({KeyPair.second->GetRelX(), 0, KeyPair.second->GetRelZ()}, {m_PosX, m_PosZ});
		if ((Pos.x != KeyPair.second->GetPosX()) || (Pos.z != KeyPair.second->GetPosZ()))
		{
			KeyPair.second->SetPos(Pos.addedY(KeyPair.second->GetPosY()));
		}
		KeyPair.second->SetWorld(m_World);
	}

	// Set the chunk data as valid.
	// This may be needed for some simulators that perform actions upon block adding (Vaporize),
	// as well as some block entities upon being added to the chunk (Chests).
	SetPresence(cpPresent);

	// Initialise all block entities:
	for (auto & KeyPair : m_BlockEntities)
	{
		KeyPair.second->OnAddToWorld(*m_World, *this);
	}

	// Wake up all simulators for their respective blocks:
	WakeUpSimulators();
}





void cChunk::SetLight(
	const cChunkDef::LightNibbles & a_BlockLight,
	const cChunkDef::LightNibbles & a_SkyLight
)
{
	// TODO: We might get cases of wrong lighting when a Chunk changes in the middle of a lighting calculation.
	// Postponing until we see how bad it is :)

	m_LightData.SetAll(a_BlockLight, a_SkyLight);

	MarkDirty();
	m_IsLightValid = true;
}





void cChunk::WriteBlockArea(cBlockArea & a_Area, int a_MinBlockX, int a_MinBlockY, int a_MinBlockZ, int a_DataTypes)
{
	if (a_DataTypes != cBlockArea::baBlocks)
	{
		LOGWARNING("cChunk::WriteBlockArea(): unsupported datatype request, can write only types + metas together (0x%x), requested 0x%x. Ignoring.",
			cBlockArea::baBlocks, a_DataTypes & cBlockArea::baBlocks
		);
		return;
	}

	// SizeX, SizeZ are the dimensions of the block data to copy to the Chunk (size of the geometric union)

	int BlockStartX = std::max(a_MinBlockX, m_PosX * cChunkDef::Width);
	int BlockEndX   = std::min(a_MinBlockX + a_Area.GetSizeX(), (m_PosX + 1) * cChunkDef::Width);
	int BlockStartZ = std::max(a_MinBlockZ, m_PosZ * cChunkDef::Width);
	int BlockEndZ   = std::min(a_MinBlockZ + a_Area.GetSizeZ(), (m_PosZ + 1) * cChunkDef::Width);
	int SizeX = BlockEndX - BlockStartX;  // Size of the union
	int SizeZ = BlockEndZ - BlockStartZ;
	int SizeY = std::min(a_Area.GetSizeY(), cChunkDef::Height - a_MinBlockY);
	int OffX = BlockStartX - m_PosX * cChunkDef::Width;  // Offset within the Chunk where the union starts
	int OffZ = BlockStartZ - m_PosZ * cChunkDef::Width;
	int BaseX = BlockStartX - a_MinBlockX;  // Offset within the area where the union starts
	int BaseZ = BlockStartZ - a_MinBlockZ;

	// Copy blocktype and blockmeta:
	auto Blocks = a_Area.GetBlocks();
	for (int y = 0; y < SizeY; y++)
	{
		int ChunkY = a_MinBlockY + y;
		int AreaY = y;
		for (int z = 0; z < SizeZ; z++)
		{
			int ChunkZ = OffZ + z;
			int AreaZ = BaseZ + z;
			for (int x = 0; x < SizeX; x++)
			{
				int ChunkX = OffX + x;
				int AreaX = BaseX + x;
				auto idx = a_Area.MakeIndex(AreaX, AreaY, AreaZ);
				FastSetBlock(ChunkX, ChunkY, ChunkZ, Blocks[idx]);
			}  // for x
		}  // for z
	}  // for y

	// Erase all affected block entities:
	{
		// The affected area, in world coordinates.
		cCuboid affectedArea(
			{ BlockStartX, a_MinBlockY, BlockStartZ },
			{ BlockEndX, a_MinBlockY + SizeY - 1, BlockEndZ }
		);

		// Where in the pending block entity send list to start removing the invalidated elements from.
		auto PendingRemove = m_PendingSendBlockEntities.end();

		for (auto itr = m_BlockEntities.begin(); itr != m_BlockEntities.end();)
		{
			if (affectedArea.IsInside(itr->second->GetPos()))
			{
				itr->second->Destroy();
				itr->second->OnRemoveFromWorld();

				PendingRemove = std::remove(m_PendingSendBlockEntities.begin(), PendingRemove, itr->second.get());  // Search the remaining valid pending sends.
				itr = m_BlockEntities.erase(itr);
			}
			else
			{
				++itr;
			}
		}

		// Remove all the deleted block entities from the pending send list:
		m_PendingSendBlockEntities.erase(PendingRemove, m_PendingSendBlockEntities.end());
	}

	// Clone block entities from a_Area into this chunk:
	if ((a_DataTypes & cBlockArea::baBlockEntities) != 0)
	{
		for (const auto & keyPair: a_Area.GetBlockEntities())
		{
			auto & be = keyPair.second;
			auto posX = be->GetPosX() + a_MinBlockX;
			auto posY = be->GetPosY() + a_MinBlockY;
			auto posZ = be->GetPosZ() + a_MinBlockZ;
			if (
				(posX < m_PosX * cChunkDef::Width) || (posX >= m_PosX * cChunkDef::Width + cChunkDef::Width) ||
				(posZ < m_PosZ * cChunkDef::Width) || (posZ >= m_PosZ * cChunkDef::Width + cChunkDef::Width)
			)
			{
				continue;
			}

			// This block entity is inside the chunk.
			// The code above should have removed any that were here before:
			ASSERT(GetBlockEntityRel(cChunkDef::AbsoluteToRelative({ posX, posY, posZ })) == nullptr);

			// Clone, and add the new one:
			AddBlockEntity(be->Clone({posX, posY, posZ}));
		}
	}
}





void cChunk::Stay(bool a_Stay)
{
	if (a_Stay)
	{
		m_StayCount++;
	}
	else
	{
		ASSERT(m_StayCount != 0);
		m_StayCount--;
	}
}





void cChunk::CollectMobCensus(cMobCensus & toFill)
{
	toFill.CollectSpawnableChunk(*this);
	std::vector<Vector3d> PlayerPositions;
	PlayerPositions.reserve(m_LoadedByClient.size());
	for (auto ClientHandle : m_LoadedByClient)
	{
		const cPlayer * currentPlayer = ClientHandle->GetPlayer();
		PlayerPositions.push_back(currentPlayer->GetPosition());
	}

	Vector3d currentPosition;
	for (auto & entity : m_Entities)
	{
		// LOGD("Counting entity #%i (%s)", (*itr)->GetUniqueID(), (*itr)->GetClass());
		if (entity->IsMob())
		{
			auto & Monster = static_cast<cMonster &>(*entity);
			currentPosition = Monster.GetPosition();
			for (const auto & PlayerPos : PlayerPositions)
			{
				toFill.CollectMob(Monster, *this, (currentPosition - PlayerPos).SqrLength());
			}
		}
	}  // for itr - m_Entitites[]
}





void cChunk::GetThreeRandomNumbers(int & a_X, int & a_Y, int & a_Z, int a_MaxX, int a_MaxY, int a_MaxZ)
{
	ASSERT(
		(a_MaxX > 0) && (a_MaxY > 0) && (a_MaxZ > 0) &&
		(a_MaxX <= std::numeric_limits<int>::max() / a_MaxY) &&  // a_MaxX * a_MaxY doesn't overflow
		(a_MaxX * a_MaxY <= std::numeric_limits<int>::max() / a_MaxZ)  // a_MaxX * a_MaxY * a_MaxZ doesn't overflow
	);

	// MTRand gives an inclusive range [0, Max] but this gives the exclusive range [0, Max)
	int OverallMax = (a_MaxX * a_MaxY * a_MaxZ) - 1;
	int Random = m_World->GetTickRandomNumber(OverallMax);

	a_X =   Random % a_MaxX;
	a_Y =  (Random / a_MaxX) % a_MaxY;
	a_Z = ((Random / a_MaxX) / a_MaxY) % a_MaxZ;
}





void cChunk::GetRandomBlockCoords(int & a_X, int & a_Y, int & a_Z)
{
	// MG TODO : check if this kind of optimization (only one random call) is still needed
	// MG TODO : if so propagate it

	GetThreeRandomNumbers(a_X, a_Y, a_Z, cChunkDef::Width, cChunkDef::Height - 2, cChunkDef::Width);
	a_Y++;
}





void cChunk::SpawnMobs(cMobSpawner & a_MobSpawner)
{
	int CenterX, CenterY, CenterZ;
	GetRandomBlockCoords(CenterX, CenterY, CenterZ);

	auto PackCenterBlock = GetBlock(CenterX, CenterY, CenterZ);
	if (!a_MobSpawner.CheckPackCenter(PackCenterBlock))
	{
		return;
	}

	a_MobSpawner.NewPack();
	int NumberOfTries = 0;
	int NumberOfSuccess = 0;
	int MaxNbOfSuccess = 4;  // This can be changed during the process for Wolves and Ghasts
	while ((NumberOfTries < 12) && (NumberOfSuccess < MaxNbOfSuccess))
	{
		const int HorizontalRange = 20;  // MG TODO : relocate
		const int VerticalRange = 0;     // MG TODO : relocate
		int TryX, TryY, TryZ;
		GetThreeRandomNumbers(TryX, TryY, TryZ, 2 * HorizontalRange + 1, 2 * VerticalRange + 1, 2 * HorizontalRange + 1);
		TryX -= HorizontalRange;
		TryY -= VerticalRange;
		TryZ -= HorizontalRange;
		TryX += CenterX;
		TryY += CenterY;
		TryZ += CenterZ;

		ASSERT(TryY > 0);
		ASSERT(TryY < cChunkDef::Height - 1);

		int WorldX, WorldY, WorldZ;
		PositionToWorldPosition(TryX, TryY, TryZ, WorldX, WorldY, WorldZ);

		// MG TODO :
		// Moon cycle (for slime)
		// check player and playerspawn presence < 24 blocks
		// check mobs presence on the block

		// MG TODO : check that "Level" really means Y

		/*
		NIBBLETYPE SkyLight = 0;

		NIBBLETYPE BlockLight = 0;
		*/

		NumberOfTries++;

		Vector3i Try(TryX, TryY, TryZ);
		const auto Chunk = GetRelNeighborChunkAdjustCoords(Try);

		if ((Chunk == nullptr) || !Chunk->IsValid() || !Chunk->IsLightValid())
		{
			continue;
		}

		auto newMob = a_MobSpawner.TryToSpawnHere(this, Try, GetBiomeAt(Try.x, Try.z), MaxNbOfSuccess);
		if (newMob == nullptr)
		{
			continue;
		}
		double ActualX = WorldX + 0.5;
		double ActualZ = WorldZ + 0.5;
		newMob->SetPosition(ActualX, WorldY, ActualZ);
		FLOGD("Spawning {0} #{1} at {2}", newMob->GetClass(), newMob->GetUniqueID(), Vector3i{WorldX, WorldY, WorldZ});
		NumberOfSuccess++;
	}  // while (retry)
}





void cChunk::Tick(std::chrono::milliseconds a_Dt)
{
	TickBlocks();

	// Tick all block entities in this Chunk:
	for (auto & KeyPair : m_BlockEntities)
	{
		m_IsDirty = KeyPair.second->Tick(a_Dt, *this) | m_IsDirty;
	}

	for (auto itr = m_Entities.begin(); itr != m_Entities.end();)
	{
		// Do not tick mobs that are detached from the world. They're either scheduled for teleportation or for removal.
		if (!(*itr)->IsTicking())
		{
			++itr;
			continue;
		}

		if (!((*itr)->IsMob()))  // Mobs are ticked inside cWorld::TickMobs() (as we don't have to tick them if they are far away from players)
		{
			// Tick all entities in this Chunk (except mobs):
			ASSERT((*itr)->GetParentChunk() == this);
			(*itr)->Tick(a_Dt, *this);
			ASSERT((*itr)->GetParentChunk() == this);
		}

		// Do not move mobs that are detached from the world to neighbors. They're either scheduled for teleportation or for removal.
		// Because the schedulded destruction is going to look for them in this Chunk. See cEntity::destroy.
		if (!(*itr)->IsTicking())
		{
			++itr;
			continue;
		}

		if (
			((*itr)->GetChunkX() != m_PosX) ||
			((*itr)->GetChunkZ() != m_PosZ)
		)
		{
			// Mark as dirty if it was a server-generated entity:
			if (!(*itr)->IsPlayer())
			{
				MarkDirty();
			}

			// This block is very similar to RemoveEntity, except it uses an iterator to avoid scanning the whole m_Entities
			// The entity moved out of the Chunk, move it to the neighbor
			(*itr)->SetParentChunk(nullptr);
			MoveEntityToNewChunk(std::move(*itr));

			itr = m_Entities.erase(itr);
		}
		else
		{
			++itr;
		}
	}  // for itr - m_Entitites[]

	ApplyWeatherToTop();

	// Tick simulators:
	m_World->GetSimulatorManager()->SimulateChunk(a_Dt, m_PosX, m_PosZ, this);

	// Check blocks after everything else to apply at least one round of queued ticks (i.e. cBlockHandler::Check) this tick:
	CheckBlocks();
}





void cChunk::TickBlock(const Vector3i a_RelPos)
{
	cChunkInterface ChunkInterface(this->GetWorld()->GetChunkMap());
	cBlockInServerPluginInterface PluginInterface(*this->GetWorld());
	cBlockHandler::For(GetBlock(a_RelPos).Type()).OnUpdate(ChunkInterface, *this->GetWorld(), PluginInterface, *this, a_RelPos);
}





void cChunk::MoveEntityToNewChunk(OwnedEntity a_Entity)
{
	cChunk * Neighbor = GetNeighborChunk(a_Entity->GetChunkX() * cChunkDef::Width, a_Entity->GetChunkZ() * cChunkDef::Width);
	if (Neighbor == nullptr)
	{
		LOGWARNING("%s: Entity at %p (%s, ID %d) moving to a non-existent Chunk.",
			__FUNCTION__, static_cast<void *>(a_Entity.get()), a_Entity->GetClass(), a_Entity->GetUniqueID()
		);

		Neighbor = &m_ChunkMap->ConstructChunk(a_Entity->GetChunkX(), a_Entity->GetChunkZ());
	}

	ASSERT(Neighbor != this);  // Moving into the same Chunk? wtf?
	auto & Entity = *a_Entity;
	Neighbor->AddEntity(std::move(a_Entity));

	class cMover :
		public cClientDiffCallback
	{
		virtual void Removed(cClientHandle * a_Client) override
		{
			a_Client->SendDestroyEntity(m_Entity);
		}

		virtual void Added(cClientHandle * a_Client) override
		{
			m_Entity.SpawnOn(*a_Client);
		}

		cEntity & m_Entity;

	public:
		cMover(cEntity & a_CallbackEntity) :
			m_Entity(a_CallbackEntity)
		{}
	} Mover(Entity);

	m_ChunkMap->CompareChunkClients(this, Neighbor, Mover);
}





void cChunk::CheckBlocks()
{
	cChunkInterface ChunkInterface(m_World->GetChunkMap());
	cBlockInServerPluginInterface PluginInterface(*m_World);

	// Process a limited number of blocks since cBlockHandler::Check may queue more to tick
	auto Count = m_BlocksToCheck.size();

	while (Count != 0)
	{
		const auto Pos = m_BlocksToCheck.front();
		m_BlocksToCheck.pop();
		Count--;

		cBlockHandler::For(GetBlock(Pos).Type()).Check(ChunkInterface, PluginInterface, Pos, *this);
	}
}





void cChunk::TickBlocks(void)
{
	cChunkInterface ChunkInterface(m_World->GetChunkMap());
	cBlockInServerPluginInterface PluginInterface(*m_World);

	// Tick random blocks, but the first one should be m_BlockToTick (so that SetNextBlockToTick() works):
	cBlockHandler::For(GetBlock(m_BlockToTick).Type()).OnUpdate(ChunkInterface, *m_World, PluginInterface, *this, m_BlockToTick);

	auto & Random = GetRandomProvider();

	// Set a new random coord for the next tick:
	m_BlockToTick = cChunkDef::IndexToCoordinate(Random.RandInt<size_t>(cChunkDef::NumBlocks - 1));

	// Choose a number of blocks for each section to randomly tick.
	// http://minecraft.wiki/w/Tick#Random_tick
	for (size_t Y = 0; Y < cChunkDef::NumSections; ++Y)
	{
		const auto Section = m_BlockData.GetSection(Y);
		if (Section == nullptr)
		{
			continue;
		}

		for (int Tick = 0; Tick != 3; Tick++)  // TODO: configurability via gamerule randomTickSpeed
		{
			const auto Index = Random.RandInt<size_t>(ChunkBlockData::SectionBlockCount - 1);
			const auto Position = cChunkDef::IndexToCoordinate(Y * ChunkBlockData::SectionBlockCount + Index);

			cBlockHandler::For((*Section)[Index].Type()).OnUpdate(ChunkInterface, *m_World, PluginInterface, *this, Position);
		}
	}
}





void cChunk::ApplyWeatherToTop()
{
	if (
		(GetRandomProvider().RandBool(0.99)) ||
		(
			(m_World->GetWeather() != eWeather_Rain) &&
			(m_World->GetWeather() != eWeather_ThunderStorm)
		)
	)
	{
		// Not the right weather, or not at this tick; bail out
		return;
	}

	int X = m_World->GetTickRandomNumber(15);
	int Z = m_World->GetTickRandomNumber(15);

	int Height = GetHeight(X, Z);

	if (GetSnowStartHeight(GetBiomeAt(X, Z)) > Height)
	{
		return;
	}

	if (GetBlockLight(X, Height, Z) > 10)
	{
		// Snow only generates on blocks with a block light level of 10 or less.
		// Ref: https://minecraft.wiki/w/Snow_(layer)#Snowfall
		return;
	}

	auto TopBlock = GetBlock(X, Height, Z);
	if (m_World->IsDeepSnowEnabled() && (TopBlock == BlockType::Snow))
	{
		int MaxSize = 7;
		BlockState Blocks[4];
		auto OldLayerCount = Block::Snow::Layers(TopBlock);
		UnboundedRelGetBlock(X - 1, Height, Z,     Blocks[0]);
		UnboundedRelGetBlock(X + 1, Height, Z,     Blocks[1]);
		UnboundedRelGetBlock(X,     Height, Z - 1, Blocks[2]);
		UnboundedRelGetBlock(X,     Height, Z + 1, Blocks[3]);
		for (int i = 0; i < 4; i++)
		{
			switch (Blocks[i].Type())
			{
				case BlockType::Air:
				case BlockType::CaveAir:
				case BlockType::VoidAir:
				{
					MaxSize = 0;
					break;
				}
				case BlockType::Snow:
				{
					MaxSize = std::min(Block::Snow::Layers(Blocks[i]) + 1, MaxSize);
					break;
				}
				default: break;
			}
		}

		if (OldLayerCount < MaxSize)
		{
			FastSetBlock(X, Height, Z, Block::Snow::Snow(OldLayerCount + 1));
		}
		else if (OldLayerCount > MaxSize)
		{
			FastSetBlock(X, Height, Z, Block::Snow::Snow(OldLayerCount - 1));
		}
	}
	else if (cBlockInfo::IsSnowable(TopBlock) && (Height < cChunkDef::Height - 1))
	{
		SetBlock({X, Height + 1, Z}, Block::Snow::Snow());
	}
	else if ((TopBlock.Type() == BlockType::Water) && (Block::Water::Level(TopBlock)) == 0)
	{
		SetBlock({X, Height, Z}, Block::Ice::Ice());
	}
	else if (
		(m_World->IsDeepSnowEnabled()) &&
		(
			(TopBlock == BlockType::Poppy) ||
			(TopBlock == BlockType::Dandelion) ||
			(TopBlock == BlockType::RedMushroom) ||
			(TopBlock == BlockType::BrownMushroom)
		)
	)
	{
		SetBlock({X, Height, Z}, Block::Snow::Snow());
	}
}





cItems cChunk::PickupsFromBlock(Vector3i a_RelPos, const cEntity * a_Digger, const cItem * a_Tool)
{
	auto BlockToDestroy = GetBlock(a_RelPos);

	cItems Pickups;
	const auto BlockEntity = GetBlockEntityRel(a_RelPos);

	if ((a_Tool == nullptr) || a_Tool->GetHandler().CanHarvestBlock(BlockToDestroy.Type()))
	{
		Pickups = cBlockHandler::For(BlockToDestroy.Type()).ConvertToPickups(BlockToDestroy, a_Tool);

		if (BlockEntity != nullptr)
		{
			auto BlockEntityPickups = BlockEntity->ConvertToPickups();
			Pickups.insert(Pickups.end(), std::make_move_iterator(BlockEntityPickups.begin()), std::make_move_iterator(BlockEntityPickups.end()));
		}
	}

	// TODO: this should be in cWorld::DropBlockAsPickups. When it's here we can't check the return value and cancel spawning:
	cRoot::Get()->GetPluginManager()->CallHookBlockToPickups(
		*m_World,
		cChunkDef::RelativeToAbsolute(a_RelPos, GetPos()),
		BlockToDestroy, BlockEntity,
		a_Digger, a_Tool, Pickups
	);

	return Pickups;
}





int cChunk::GrowPlantAt(Vector3i a_RelPos, char a_NumStages)
{
	return cBlockHandler::For(GetBlock(a_RelPos).Type()).Grow(*this, a_RelPos, a_NumStages);
}





bool cChunk::UnboundedRelGetBlock(Vector3i a_RelPos, BlockState & a_Block) const
{
	if (!cChunkDef::IsValidHeight(a_RelPos))
	{
		LOGWARNING("%s: requesting a block with a_RelY out of range: %d", __FUNCTION__, a_RelPos.y);
		return false;
	}
	auto Chunk = GetRelNeighborChunkAdjustCoords(a_RelPos);
	if ((Chunk == nullptr) || !Chunk->IsValid())
	{
		// The Chunk is not available, bail out
		return false;
	}
	a_Block = Chunk->GetBlock(a_RelPos);
	return true;
}





bool cChunk::UnboundedRelGetBlockBlockLight(Vector3i a_RelPos, LIGHTTYPE & a_BlockBlockLight) const
{
	if (!cChunkDef::IsValidHeight(a_RelPos))
	{
		LOGWARNING("%s: requesting a block with a_RelY out of range: %d", __FUNCTION__, a_RelPos.y);
		return false;
	}
	auto Chunk = GetRelNeighborChunkAdjustCoords(a_RelPos);
	if ((Chunk == nullptr) || !Chunk->IsValid())
	{
		// The Chunk is not available, bail out
		return false;
	}
	a_BlockBlockLight = Chunk->GetBlockLight(a_RelPos);
	return true;
}





bool cChunk::UnboundedRelGetBlockSkyLight(Vector3i a_RelPos, LIGHTTYPE & a_BlockSkyLight) const
{
	if (!cChunkDef::IsValidHeight(a_RelPos))
	{
		LOGWARNING("%s: requesting a block with a_RelY out of range: %d", __FUNCTION__, a_RelPos.y);
		return false;
	}
	auto Chunk = GetRelNeighborChunkAdjustCoords(a_RelPos);
	if ((Chunk == nullptr) || !Chunk->IsValid())
	{
		// The Chunk is not available, bail out
		return false;
	}
	a_BlockSkyLight = Chunk->GetSkyLight(a_RelPos);
	return true;
}





bool cChunk::UnboundedRelGetBlockLights(Vector3i a_RelPos, LIGHTTYPE & a_BlockLight, LIGHTTYPE & a_SkyLight) const
{
	if (!cChunkDef::IsValidHeight(a_RelPos))
	{
		LOGWARNING("%s: requesting a block with a_RelY out of range: %d", __FUNCTION__, a_RelPos.y);
		return false;
	}
	auto Chunk = GetRelNeighborChunkAdjustCoords(a_RelPos);
	if ((Chunk == nullptr) || !Chunk->IsValid())
	{
		// The Chunk is not available, bail out
		return false;
	}
	a_BlockLight = Chunk->GetBlockLight(a_RelPos);
	a_SkyLight   = Chunk->GetSkyLight  (a_RelPos);
	return true;
}





bool cChunk::UnboundedRelSetBlock(Vector3i a_RelPos, BlockState a_Block)
{
	if (!cChunkDef::IsValidHeight(a_RelPos))
	{
		LOGWARNING("%s: requesting a block with a_RelY out of range: %d", __FUNCTION__, a_RelPos.y);
		return false;
	}
	auto Chunk = GetRelNeighborChunkAdjustCoords(a_RelPos);
	if ((Chunk == nullptr) || !Chunk->IsValid())
	{
		// The Chunk is not available, bail out
		return false;
	}
	Chunk->SetBlock(a_RelPos, a_Block);
	return true;
}





bool cChunk::UnboundedRelFastSetBlock(Vector3i a_RelPos, BlockState a_Block)
{
	if (!cChunkDef::IsValidHeight(a_RelPos))
	{
		LOGWARNING("%s: requesting a block with a_RelY out of range: %d", __FUNCTION__, a_RelPos.y);
		return false;
	}
	auto Chunk = GetRelNeighborChunkAdjustCoords(a_RelPos);
	if ((Chunk == nullptr) || !Chunk->IsValid())
	{
		// The Chunk is not available, bail out
		return false;
	}
	Chunk->FastSetBlock(a_RelPos, a_Block);
	return true;
}





int cChunk::GetHeight(int a_X, int a_Z) const
{
	ASSERT((a_X >= 0) && (a_X < cChunkDef::Width) && (a_Z >= 0) && (a_Z < cChunkDef::Width));
	return m_HeightMap[static_cast<size_t>(a_X + a_Z * cChunkDef::Width)];
}





bool cChunk::IsWeatherSunnyAt(int a_RelX, int a_RelZ) const
{
	return m_World->IsWeatherSunny() || IsBiomeNoDownfall(GetBiomeAt(a_RelX, a_RelZ));
}





bool cChunk::IsWeatherWetAt(const int a_RelX, const int a_RelZ) const
{
	const auto Biome = GetBiomeAt(a_RelX, a_RelZ);
	return m_World->IsWeatherWet() && !IsBiomeNoDownfall(Biome) && !IsBiomeCold(Biome);
}





bool cChunk::IsWeatherWetAt(const Vector3i a_Position) const
{
	if ((a_Position.y < 0) || !IsWeatherWetAt(a_Position.x, a_Position.z))
	{
		return false;
	}

	if (a_Position.y >= cChunkDef::Height)
	{
		return true;
	}

	for (int y = GetHeight(a_Position.x, a_Position.z); y >= a_Position.y; y--)
	{
		if (cBlockInfo::IsRainBlocker(GetBlock({ a_Position.x, y, a_Position.z })))
		{
			return false;
		}
	}

	return true;
}





void cChunk::WakeUpSimulators(void)
{
	auto * WaterSimulator = m_World->GetWaterSimulator();
	auto * LavaSimulator  = m_World->GetLavaSimulator();
	auto * RedstoneSimulator = m_World->GetRedstoneSimulator();

	for (size_t SectionIdx = 0; SectionIdx != cChunkDef::NumSections; ++SectionIdx)
	{
		const auto & Section = m_BlockData.GetSection(SectionIdx);
		if (Section == nullptr)
		{
			continue;
		}

		for (size_t BlockIdx = 0; BlockIdx != ChunkBlockData::SectionBlockCount; ++BlockIdx)
		{
			const auto Block = (*Section)[BlockIdx];
			const auto Position = cChunkDef::IndexToCoordinate(BlockIdx + SectionIdx * ChunkBlockData::SectionBlockCount);

			RedstoneSimulator->AddBlock(*this, Position, Block);
			WaterSimulator->AddBlock(*this, Position, Block);
			LavaSimulator->AddBlock(*this, Position, Block);
		}
	}
}





void cChunk::SetBlock(Vector3i a_RelPos, BlockState a_Block)
{
	FastSetBlock(a_RelPos, a_Block);

	// Queue a check of this block's neighbors:
	m_BlocksToCheck.push(a_RelPos);

	// Wake up the simulators for this block:
	GetWorld()->GetSimulatorManager()->WakeUp(*this, a_RelPos);

	// If there was a block entity, remove it:
	if (const auto FindResult = m_BlockEntities.find(cChunkDef::MakeIndex(a_RelPos)); FindResult != m_BlockEntities.end())
	{
		auto & BlockEntity = *FindResult->second;

		BlockEntity.Destroy();
		BlockEntity.OnRemoveFromWorld();

		m_BlockEntities.erase(FindResult);
		m_PendingSendBlockEntities.erase(std::remove(m_PendingSendBlockEntities.begin(), m_PendingSendBlockEntities.end(), &BlockEntity), m_PendingSendBlockEntities.end());
	}

	// If the new block is a block entity, create the entity object:
	if (cBlockEntity::IsBlockEntityBlockType(a_Block))
	{
		AddBlockEntity(cBlockEntity::CreateByBlockType(a_Block, RelativeToAbsolute(a_RelPos), m_World));
	}
}





void cChunk::FastSetBlock(int a_RelX, int a_RelY, int a_RelZ, BlockState a_Block)
{
	ASSERT(cChunkDef::IsValidRelPos({ a_RelX, a_RelY, a_RelZ }));
	ASSERT(IsValid());

	const auto OldBlock = GetBlock(a_RelX, a_RelY, a_RelZ);
	if (OldBlock == a_Block)
	{
		return;
	}

	bool ReplacingLiquids = (
		((OldBlock.Type() == BlockType::Water)            && (a_Block.Type() == BlockType::Water)) ||  // Replacing water with stationary water
		((OldBlock.Type() == BlockType::Lava)             && (a_Block.Type() == BlockType::Lava))      // Replacing lava with stationary lava
	);

	if (!ReplacingLiquids)
	{
		MarkDirty();
	}

	m_BlockData.SetBlock({ a_RelX, a_RelY, a_RelZ }, a_Block);

	// Queue block to be sent only if ...
	if (
		!(cBlockLeavesHandler::IsBlockLeaves(OldBlock) && cBlockLeavesHandler::IsBlockLeaves(a_Block)) &&  // ... the old and new blocktypes AREN'T leaves (because the client doesn't need meta updates)
		// ... AND ...
		(!ReplacingLiquids)
	)
	{
		m_PendingSendBlocks.emplace_back(m_PosX, m_PosZ, a_RelX, a_RelY, a_RelZ, a_Block);
	}

	// ONLY recalculate lighting if it's necessary!
	if (
		(cBlockInfo::GetLightValue        (OldBlock) != cBlockInfo::GetLightValue        (a_Block)) ||
		(cBlockInfo::GetSpreadLightFalloff(OldBlock) != cBlockInfo::GetSpreadLightFalloff(a_Block)) ||
		(cBlockInfo::IsTransparent        (OldBlock) != cBlockInfo::IsTransparent        (a_Block))
	)
	{
		m_IsLightValid = false;
	}

	// Update heightmap, if needed:
	if (a_RelY >= m_HeightMap[static_cast<size_t>(a_RelX + a_RelZ * cChunkDef::Width)])
	{
		if (cBlockAirHandler::IsBlockAir(a_Block.Type()))
		{
			m_HeightMap[static_cast<size_t>(a_RelX + a_RelZ * cChunkDef::Width)] = static_cast<HEIGHTTYPE>(a_RelY);
		}
		else
		{
			for (int y = a_RelY - 1; y > 0; --y)
			{
				if (!cBlockAirHandler::IsBlockAir(GetBlock(a_RelX, y, a_RelZ)))
				{
					m_HeightMap[static_cast<size_t>(a_RelX + a_RelZ * cChunkDef::Width)] = static_cast<HEIGHTTYPE>(y);
					break;
				}
			}  // for y - column in m_BlockData
		}
	}
}





void cChunk::SendBlockTo(int a_RelX, int a_RelY, int a_RelZ, cClientHandle * a_Client)
{
	const auto BlockEntity = GetBlockEntityRel({ a_RelX, a_RelY, a_RelZ });

	if (a_Client == nullptr)
	{
		// Queue the block (entity) for all clients in the chunk (will be sent in BroadcastPendingBlockChanges()):
		m_PendingSendBlocks.emplace_back(m_PosX, m_PosZ, a_RelX, a_RelY, a_RelZ, GetBlock(a_RelX, a_RelY, a_RelZ));
		if (BlockEntity != nullptr)
		{
			m_PendingSendBlockEntities.push_back(BlockEntity);
		}
		return;
	}

	const auto Position = PositionToWorldPosition(a_RelX, a_RelY, a_RelZ);
	a_Client->SendBlockChange(Position, GetBlock(a_RelX, a_RelY, a_RelZ));

	// FS #268 - if a BlockEntity digging is cancelled by a plugin, the entire block entity must be re-sent to the client:
	if (BlockEntity != nullptr)
	{
		BlockEntity->SendTo(*a_Client);
	}
}





void cChunk::AddBlockEntity(OwnedBlockEntity a_BlockEntity)
{
	const auto BlockEntityPtr = a_BlockEntity.get();
	[[maybe_unused]] const auto Result = m_BlockEntities.emplace(
		cChunkDef::MakeIndex(a_BlockEntity->GetRelX(), a_BlockEntity->GetPosY(), a_BlockEntity->GetRelZ()),
		std::move(a_BlockEntity)
	);

	ASSERT(Result.second);  // No block entity already at this position.
	BlockEntityPtr->OnAddToWorld(*m_World, *this);
}





cBlockEntity * cChunk::GetBlockEntity(Vector3i a_AbsPos)
{
	const auto relPos = cChunkDef::AbsoluteToRelative(a_AbsPos);

	if (!cChunkDef::IsValidRelPos(relPos))
	{
		// Coordinates are outside outside this Chunk, no block entities here
		return nullptr;
	}

	auto itr = m_BlockEntities.find(cChunkDef::MakeIndex(relPos));
	return (itr == m_BlockEntities.end()) ? nullptr : itr->second.get();
}





cBlockEntity * cChunk::GetBlockEntityRel(Vector3i a_RelPos)
{
	ASSERT(cChunkDef::IsValidRelPos(a_RelPos));
	auto itr = m_BlockEntities.find(cChunkDef::MakeIndex(a_RelPos));
	return (itr == m_BlockEntities.end()) ? nullptr : itr->second.get();
}





bool cChunk::ShouldBeTicked(void) const
{
	return IsValid() && (HasAnyClients() || (m_AlwaysTicked > 0));
}





void cChunk::SetAlwaysTicked(bool a_AlwaysTicked)
{
	if (a_AlwaysTicked)
	{
		m_AlwaysTicked += 1;
		Stay(a_AlwaysTicked);
	}
	else
	{
		m_AlwaysTicked -= 1;
		Stay(a_AlwaysTicked);
	}
}





bool cChunk::UseBlockEntity(cPlayer * a_Player, int a_X, int a_Y, int a_Z)
{
	cBlockEntity * be = GetBlockEntity(a_X, a_Y, a_Z);
	if (be != nullptr)
	{
		return be->UsedBy(a_Player);
	}
	return false;
}





void cChunk::SetBiomeAt(int a_RelX, int a_RelZ, EMCSBiome a_Biome)
{
	cChunkDef::SetBiome(m_BiomeMap, a_RelX, a_RelZ, a_Biome);
	MarkDirty();
}





void cChunk::SetAreaBiome(int a_MinRelX, int a_MaxRelX, int a_MinRelZ, int a_MaxRelZ, EMCSBiome a_Biome)
{
	for (int z = a_MinRelZ; z <= a_MaxRelZ; z++)
	{
		for (int x = a_MinRelX; x <= a_MaxRelX; x++)
		{
			cChunkDef::SetBiome(m_BiomeMap, x, z, a_Biome);
		}
	}
	MarkDirty();

	// Re-send the Chunk to all clients:
	for (auto ClientHandle : m_LoadedByClient)
	{
		m_World->ForceSendChunkTo(m_PosX, m_PosZ, cChunkSender::Priority::Medium, ClientHandle);
	}  // for itr - m_LoadedByClient[]
}





bool cChunk::AddClient(cClientHandle * a_Client)
{
	if (std::find(m_LoadedByClient.begin(), m_LoadedByClient.end(), a_Client) != m_LoadedByClient.end())
	{
		// Already there, nothing needed
		return false;
	}

	m_LoadedByClient.push_back(a_Client);
	return true;
}





void cChunk::RemoveClient(cClientHandle * a_Client)
{
	auto itr = std::remove(m_LoadedByClient.begin(), m_LoadedByClient.end(), a_Client);
	// We should always remove at most one client.
	ASSERT(std::distance(itr, m_LoadedByClient.end()) <= 1);
	// Note: itr can equal m_LoadedByClient.end()
	m_LoadedByClient.erase(itr, m_LoadedByClient.end());

	if (!a_Client->IsDestroyed())
	{
		for (auto & Entity : m_Entities)
		{
			/*
			// DEBUG:
			LOGD("Chunk [%i, %i] destroying entity #%i for player \"%s\"",
				m_PosX, m_PosZ,
				(*itr)->GetUniqueID(), a_Client->GetUsername().c_str()
			);
			*/
			a_Client->SendDestroyEntity(*Entity);
		}
	}
}





bool cChunk::HasClient(cClientHandle * a_Client)
{
	return std::find(m_LoadedByClient.begin(), m_LoadedByClient.end(), a_Client) != m_LoadedByClient.end();
}





bool cChunk::HasAnyClients(void) const
{
	return !m_LoadedByClient.empty();
}





void cChunk::AddEntity(OwnedEntity a_Entity)
{
	if (!a_Entity->IsPlayer())
	{
		MarkDirty();
	}

	auto EntityPtr = a_Entity.get();

	ASSERT(std::find(m_Entities.begin(), m_Entities.end(), a_Entity) == m_Entities.end());  // Not there already
	m_Entities.emplace_back(std::move(a_Entity));

	ASSERT(EntityPtr->GetParentChunk() == nullptr);
	EntityPtr->SetParentChunk(this);
}





OwnedEntity cChunk::RemoveEntity(cEntity & a_Entity)
{
	ASSERT(a_Entity.GetParentChunk() == this);
	ASSERT(!a_Entity.IsTicking());
	a_Entity.SetParentChunk(nullptr);

	// Mark as dirty if it was a server-generated entity:
	if (!a_Entity.IsPlayer())
	{
		MarkDirty();
	}

	OwnedEntity Removed;
	m_Entities.erase(
		std::remove_if(
			m_Entities.begin(),
			m_Entities.end(),
			[&a_Entity, &Removed](decltype(m_Entities)::value_type & a_Value)
			{
				if (a_Value.get() == &a_Entity)
				{
					ASSERT(!Removed);
					Removed = std::move(a_Value);
					return true;
				}

				return false;
			}
		),
		m_Entities.end()
	);

	return Removed;
}





bool cChunk::HasEntity(UInt32 a_EntityID) const
{
	for (const auto & Entity : m_Entities)
	{
		if (Entity->GetUniqueID() == a_EntityID)
		{
			return true;
		}
	}
	return false;
}





bool cChunk::ForEachEntity(cEntityCallback a_Callback) const
{
	// The entity list is locked by the parent Chunkmap's CS
	for (const auto & Entity : m_Entities)
	{
		if (Entity->IsTicking() && a_Callback(*Entity))
		{
			return false;
		}
	}  // for itr - m_Entitites[]
	return true;
}





bool cChunk::ForEachEntityInBox(const cBoundingBox & a_Box, cEntityCallback a_Callback) const
{
	// The entity list is locked by the parent Chunkmap's CS
	for (const auto & Entity : m_Entities)
	{
		if (!Entity->IsTicking())
		{
			continue;
		}
		if (!Entity->GetBoundingBox().DoesIntersect(a_Box))
		{
			// The entity is not in the specified box
			continue;
		}
		if (a_Callback(*Entity))
		{
			return false;
		}
	}  // for itr - m_Entitites[]
	return true;
}





bool cChunk::DoWithEntityByID(UInt32 a_EntityID, cEntityCallback a_Callback, bool & a_CallbackResult) const
{
	// The entity list is locked by the parent Chunkmap's CS
	for (const auto & Entity : m_Entities)
	{
		if ((Entity->GetUniqueID() == a_EntityID) && (Entity->IsTicking()))
		{
			a_CallbackResult = a_Callback(*Entity);
			return true;
		}
	}  // for itr - m_Entitites[]
	return false;
}





bool cChunk::ForEachBlockEntity(cBlockEntityCallback a_Callback)
{
	// The blockentity list is locked by the parent chunkmap's CS

	for (auto & KeyPair : m_BlockEntities)
	{
		if (a_Callback(*KeyPair.second))
		{
			return false;
		}
	}

	return true;
}





bool cChunk::DoWithBlockEntityAt(Vector3i a_Position, cBlockEntityCallback a_Callback)
{
	// The blockentity list is locked by the parent chunkmap's CS

	const auto BlockEntity = GetBlockEntityRel(a_Position);
	if (BlockEntity == nullptr)
	{
		return false;  // No block entity here
	}

	const bool Result = a_Callback(*BlockEntity);
	m_PendingSendBlockEntities.push_back(BlockEntity);
	MarkDirty();
	return Result;
}





void cChunk::GetBlockInfo(Vector3i a_RelPos, BlockState & a_Block, LIGHTTYPE & a_SkyLight, LIGHTTYPE & a_BlockLight) const
{
	a_Block      = GetBlock(a_RelPos);
	a_SkyLight   = m_LightData.GetSkyLight(a_RelPos);
	a_BlockLight = m_LightData.GetBlockLight(a_RelPos);
}





bool cChunk::GetChunkAndRelByAbsolute(const Vector3d & a_Position, cChunk ** a_Chunk, Vector3i & a_Rel)
{
	return GetChunkAndRelByAbsolute(Vector3i(FloorC(a_Position.x), FloorC(a_Position.y), FloorC(a_Position.z)), a_Chunk, a_Rel);
}





bool cChunk::GetChunkAndRelByAbsolute(const Vector3i & a_Position, cChunk ** a_Chunk, Vector3i & a_Rel)
{
	*a_Chunk = this->GetNeighborChunk(a_Position.x, a_Position.z);
	if ((*a_Chunk == nullptr) || !(*a_Chunk)->IsValid())
	{
		return false;
	}

	a_Rel.x = a_Position.x - (*a_Chunk)->GetPosX() * cChunkDef::Width;
	a_Rel.y = a_Position.y;
	a_Rel.z = a_Position.z - (*a_Chunk)->GetPosZ() * cChunkDef::Width;
	return true;
}





cChunk * cChunk::GetNeighborChunk(int a_BlockX, int a_BlockZ)
{
	// Convert coords to relative, then call the relative version:
	a_BlockX -= m_PosX * cChunkDef::Width;
	a_BlockZ -= m_PosZ * cChunkDef::Width;
	return GetRelNeighborChunk(a_BlockX, a_BlockZ);
}





cChunk * cChunk::GetRelNeighborChunk(int a_RelX, int a_RelZ)
{
	// If the relative coords are too far away, use the parent's Chunk lookup instead:
	if ((a_RelX < -128) || (a_RelX > 128) || (a_RelZ < -128) || (a_RelZ > 128))
	{
		int BlockX = m_PosX * cChunkDef::Width + a_RelX;
		int BlockZ = m_PosZ * cChunkDef::Width + a_RelZ;
		int ChunkX, ChunkZ;
		cChunkDef::BlockToChunk(BlockX, BlockZ, ChunkX, ChunkZ);
		return m_ChunkMap->FindChunk(ChunkX, ChunkZ);
	}

	// Walk the neighbors:
	bool ReturnThis = true;
	if (a_RelX < 0)
	{
		if (m_NeighborXM != nullptr)
		{
			cChunk * Candidate = m_NeighborXM->GetRelNeighborChunk(a_RelX + cChunkDef::Width, a_RelZ);
			if (Candidate != nullptr)
			{
				return Candidate;
			}
		}
		// Going X first failed, but if the request is crossing Z as well, let's try the Z first later on.
		ReturnThis = false;
	}
	else if (a_RelX >= cChunkDef::Width)
	{
		if (m_NeighborXP != nullptr)
		{
			cChunk * Candidate = m_NeighborXP->GetRelNeighborChunk(a_RelX - cChunkDef::Width, a_RelZ);
			if (Candidate != nullptr)
			{
				return Candidate;
			}
		}
		// Going X first failed, but if the request is crossing Z as well, let's try the Z first later on.
		ReturnThis = false;
	}

	if (a_RelZ < 0)
	{
		if (m_NeighborZM != nullptr)
		{
			return m_NeighborZM->GetRelNeighborChunk(a_RelX, a_RelZ + cChunkDef::Width);
			// For requests crossing both X and Z, the X-first way has been already tried
		}
		return nullptr;
	}
	else if (a_RelZ >= cChunkDef::Width)
	{
		if (m_NeighborZP != nullptr)
		{
			return m_NeighborZP->GetRelNeighborChunk(a_RelX, a_RelZ - cChunkDef::Width);
			// For requests crossing both X and Z, the X-first way has been already tried
		}
		return nullptr;
	}

	return (ReturnThis ? this : nullptr);
}





cChunk * cChunk::GetRelNeighborChunkAdjustCoords(Vector3i & a_RelPos) const
{
	cChunk * ToReturn = const_cast<cChunk *>(this);

	// The most common case: inside this Chunk:
	if (
		(a_RelPos.x >= 0) && (a_RelPos.x < cChunkDef::Width) &&
		(a_RelPos.z >= 0) && (a_RelPos.z < cChunkDef::Width)
	)
	{
		return ToReturn;
	}

	// Request for a different Chunk, calculate Chunk offset:
	int RelX = a_RelPos.x;  // Make a local copy of the coords (faster access)
	int RelZ = a_RelPos.z;
	while ((RelX >= cChunkDef::Width) && (ToReturn != nullptr))
	{
		RelX -= cChunkDef::Width;
		ToReturn = ToReturn->m_NeighborXP;
	}
	while ((RelX < 0) && (ToReturn != nullptr))
	{
		RelX += cChunkDef::Width;
		ToReturn = ToReturn->m_NeighborXM;
	}
	while ((RelZ >= cChunkDef::Width) && (ToReturn != nullptr))
	{
		RelZ -= cChunkDef::Width;
		ToReturn = ToReturn->m_NeighborZP;
	}
	while ((RelZ < 0) && (ToReturn != nullptr))
	{
		RelZ += cChunkDef::Width;
		ToReturn = ToReturn->m_NeighborZM;
	}
	if (ToReturn != nullptr)
	{
		a_RelPos.x = RelX;
		a_RelPos.z = RelZ;
		return ToReturn;
	}

	// The Chunk cannot be walked through neighbors, find it through the Chunkmap:
	int AbsX = a_RelPos.x + m_PosX * cChunkDef::Width;
	int AbsZ = a_RelPos.z + m_PosZ * cChunkDef::Width;
	int DstChunkX, DstChunkZ;
	cChunkDef::BlockToChunk(AbsX, AbsZ, DstChunkX, DstChunkZ);
	ToReturn = m_ChunkMap->FindChunk(DstChunkX, DstChunkZ);
	a_RelPos.x = AbsX - DstChunkX * cChunkDef::Width;
	a_RelPos.z = AbsZ - DstChunkZ * cChunkDef::Width;
	return ToReturn;
}





void cChunk::SendBlockEntity(int a_BlockX, int a_BlockY, int a_BlockZ, cClientHandle & a_Client)
{
	cBlockEntity * Entity = GetBlockEntity(a_BlockX, a_BlockY, a_BlockZ);
	if (Entity == nullptr)
	{
		return;
	}
	Entity->SendTo(a_Client);
}





void cChunk::PositionToWorldPosition(int a_RelX, int a_RelY, int a_RelZ, int & a_BlockX, int & a_BlockY, int & a_BlockZ)
{
	a_BlockY = a_RelY;
	a_BlockX = m_PosX * cChunkDef::Width + a_RelX;
	a_BlockZ = m_PosZ * cChunkDef::Width + a_RelZ;
}





Vector3i cChunk::PositionToWorldPosition(int a_RelX, int a_RelY, int a_RelZ)
{
	return Vector3i(m_PosX * cChunkDef::Width + a_RelX, a_RelY, m_PosZ * cChunkDef::Width + a_RelZ);
}





LIGHTTYPE cChunk::GetTimeAlteredLight(LIGHTTYPE a_Skylight) const
{
	a_Skylight -= m_World->GetSkyDarkness();
	// Because NIBBLETYPE is unsigned, we clamp it to 0 .. 15 by checking for values above 15
	return (a_Skylight < 16)? a_Skylight : 0;
}





bool cChunk::IsSlimeChunk() const
{
	return m_World->IsSlimeChunk(m_PosX, m_PosZ);
}
