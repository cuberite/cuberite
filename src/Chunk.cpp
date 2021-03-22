
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
#include "BlockEntities/BeaconEntity.h"
#include "BlockEntities/BedEntity.h"
#include "BlockEntities/BrewingstandEntity.h"
#include "BlockEntities/ChestEntity.h"
#include "BlockEntities/CommandBlockEntity.h"
#include "BlockEntities/DispenserEntity.h"
#include "BlockEntities/DropperEntity.h"
#include "BlockEntities/FlowerPotEntity.h"
#include "BlockEntities/FurnaceEntity.h"
#include "BlockEntities/HopperEntity.h"
#include "BlockEntities/JukeboxEntity.h"
#include "BlockEntities/MobHeadEntity.h"
#include "BlockEntities/MobSpawnerEntity.h"
#include "BlockEntities/NoteEntity.h"
#include "BlockEntities/SignEntity.h"
#include "Entities/Pickup.h"
#include "Item.h"
#include "Noise/Noise.h"
#include "Root.h"
#include "Entities/Player.h"
#include "BlockArea.h"
#include "Bindings/PluginManager.h"
#include "Blocks/BlockHandler.h"
#include "Simulator/FluidSimulator.h"
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
	for (auto & BlockEntity : m_BlockEntities)
	{
		BlockEntity.second->OnRemoveFromWorld();
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
	std::copy(a_SetChunkData.HeightMap.begin(), a_SetChunkData.HeightMap.end(), m_HeightMap.data());
	std::copy(a_SetChunkData.BiomeMap.begin(), a_SetChunkData.BiomeMap.end(), m_BiomeMap.data());

	m_BlockData = std::move(a_SetChunkData.BlockData);
	m_LightData = std::move(a_SetChunkData.LightData);
	m_IsLightValid = a_SetChunkData.IsLightValid;

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

	// Clear the block entities present - either the loader / saver has better, or we'll create empty ones:
	m_BlockEntities = std::move(a_SetChunkData.BlockEntities);

	// Check that all block entities have a valid blocktype at their respective coords (DEBUG-mode only):
	#ifndef NDEBUG
		for (auto & KeyPair : m_BlockEntities)
		{
			cBlockEntity * Block = KeyPair.second.get();
			auto EntityBlock = Block->GetBlockType();
			auto WorldBlock = GetBlock(Block->GetRelX(), Block->GetPosY(), Block->GetRelZ()).Type();

			// ASSERT(WorldBlock == EntityBlock);  // TODO (12xx12) readd check
		}  // for KeyPair - m_BlockEntities
	#endif  // !NDEBUG

	// Set all block entities' World variable:
	for (auto & KeyPair : m_BlockEntities)
	{
		KeyPair.second->SetWorld(m_World);
	}

	// Set the Chunk data as valid. This may be needed for some simulators that perform actions upon block adding (Vaporize)
	SetPresence(cpPresent);

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
	auto & Blocks = a_Area.GetBlocks();
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
				FastSetBlock(ChunkX, ChunkY, ChunkZ, Blocks->at(idx));
			}  // for x
		}  // for z
	}  // for y

	// Erase all affected block entities:
	cCuboid affectedArea(  // In world coordinates
		{BlockStartX, a_MinBlockY, BlockStartZ},
		{BlockEndX, a_MinBlockY + SizeY - 1, BlockEndZ}
	);
	for (auto itr = m_BlockEntities.begin(); itr != m_BlockEntities.end();)
	{
		if (affectedArea.IsInside(itr->second->GetPos()))
		{
			itr->second->Destroy();
			itr->second->OnRemoveFromWorld();
			itr = m_BlockEntities.erase(itr);
		}
		else
		{
			++itr;
		}
	}

	// Clone block entities from a_Area into this Chunk:
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
			// This block entity is inside the Chunk, clone it (and remove any that is there currently):
			auto idx = static_cast<size_t>(cChunkDef::MakeIndex(posX - m_PosX * cChunkDef::Width, posY, posZ - m_PosZ * cChunkDef::Width));
			auto itr = m_BlockEntities.find(idx);
			if (itr != m_BlockEntities.end())
			{
				m_BlockEntities.erase(itr);
			}
			auto clone = be->Clone({posX, posY, posZ});
			clone->SetWorld(m_World);
			AddBlockEntity(std::move(clone));
			m_World->BroadcastBlockEntity({posX, posY, posZ});
		}
	}
}





bool cChunk::HasBlockEntityAt(Vector3i a_BlockPos)
{
	return (GetBlockEntity(a_BlockPos) != nullptr);
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
	const auto ShouldTick = ShouldBeTicked();

	// If we are not valid, tick players and bailout
	if (!ShouldTick)
	{
		for (const auto & Entity : m_Entities)
		{
			if (Entity->IsPlayer())
			{
				Entity->Tick(a_Dt, *this);
			}
		}
		return;
	}

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

	// Finally, tell the client about all block changes:
	BroadcastPendingBlockChanges();
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





void cChunk::BroadcastPendingBlockChanges(void)
{
	if (m_PendingSendBlocks.empty())
	{
		return;
	}

	if (m_PendingSendBlocks.size() >= 10240)
	{
		// Resend the full Chunk
		for (auto ClientHandle : m_LoadedByClient)
		{
			m_World->ForceSendChunkTo(m_PosX, m_PosZ, cChunkSender::Priority::Medium, ClientHandle);
		}
	}
	else
	{
		// Only send block changes
		for (auto ClientHandle : m_LoadedByClient)
		{
			ClientHandle->SendBlockChanges(m_PosX, m_PosZ, m_PendingSendBlocks);
		}
	}
	m_PendingSendBlocks.clear();
}





void cChunk::CheckBlocks()
{
	cChunkInterface ChunkInterface(m_World->GetChunkMap());
	cBlockInServerPluginInterface PluginInterface(*m_World);

	// Process a limited number of blocks since cBlockHandler::Check may queue more to tick
	auto Count = m_ToTickBlocks.size();

	while (Count != 0)
	{
		const auto Pos = m_ToTickBlocks.front();
		m_ToTickBlocks.pop();
		Count--;

		cBlockHandler::For(GetBlock(Pos).Type()).Check(ChunkInterface, PluginInterface, Pos, *this);
	}
}





void cChunk::TickBlocks(void)
{
	cChunkInterface ChunkInterface(this->GetWorld()->GetChunkMap());
	cBlockInServerPluginInterface PluginInterface(*this->GetWorld());

	// Tick random blocks, but the first one should be m_BlockToTick (so that SetNextBlockToTick() works)
	auto Idx = cChunkDef::MakeIndexNoCheck(m_BlockToTick);
	auto & Random = GetRandomProvider();

	for (int i = 0; i < 50; ++i)
	{
		auto Pos = cChunkDef::IndexToCoordinate(static_cast<size_t>(Idx));
		Idx = Random.RandInt(cChunkDef::NumBlocks - 1);
		if (Pos.y > cChunkDef::GetHeight(m_HeightMap, Pos.x, Pos.z))
		{
			continue;  // It's all air up here
		}

		cBlockHandler::For(GetBlock(Pos).Type()).OnUpdate(ChunkInterface, *this->GetWorld(), PluginInterface, *this, Pos);
	}  // for i

	// Set a new random coord for the next tick:
	m_BlockToTick = cChunkDef::IndexToCoordinate(static_cast<size_t>(Idx));
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
		// Ref: https://minecraft.gamepedia.com/Snow_(layer)#Snowfall
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

	const auto ToolHandler = (a_Tool != nullptr) ? a_Tool->GetHandler() : cItemHandler::GetItemHandler(E_ITEM_EMPTY);
	if (ToolHandler->CanHarvestBlock(BlockToDestroy))
	{
		Pickups = cBlockHandler::For(BlockToDestroy.Type()).ConvertToPickups(BlockToDestroy, a_Digger, a_Tool);

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





int cChunk::GrowPlantAt(Vector3i a_RelPos, unsigned char a_NumStages)
{
	return cBlockHandler::For(GetBlock(a_RelPos).Type()).Grow(*this, a_RelPos, a_NumStages);
}





bool cChunk::UnboundedRelGetBlock(Vector3i a_RelPos, BlockState & a_Block) const
{
	if (!cChunkDef::IsValidHeight(a_RelPos.y))
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
	if (!cChunkDef::IsValidHeight(a_RelPos.y))
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
	if (!cChunkDef::IsValidHeight(a_RelPos.y))
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
	if (!cChunkDef::IsValidHeight(a_RelPos.y))
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
	if (!cChunkDef::IsValidHeight(a_RelPos.y))
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
	if (!cChunkDef::IsValidHeight(a_RelPos.y))
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
	return m_HeightMap[a_X + a_Z * cChunkDef::Width];
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
			const auto BlockType = (*Section)[BlockIdx];
			const auto Position = cChunkDef::IndexToCoordinate(BlockIdx + SectionIdx * ChunkBlockData::SectionBlockCount);

			RedstoneSimulator->AddBlock(*this, Position, BlockType);
			WaterSimulator->AddBlock(*this, Position, BlockType);
			LavaSimulator->AddBlock(*this, Position, BlockType);
		}
	}
}





void cChunk::SetBlock(Vector3i a_RelPos, BlockState a_Block)
{
	FastSetBlock(a_RelPos, a_Block);

	// Tick this block's neighbors via cBlockHandler::Check:
	m_ToTickBlocks.push(a_RelPos);

	// Wake up the simulators for this block:
	GetWorld()->GetSimulatorManager()->WakeUp(*this, a_RelPos);

	// If there was a block entity, remove it:
	cBlockEntity * BlockEntity = GetBlockEntityRel(a_RelPos);
	if (BlockEntity != nullptr)
	{
		BlockEntity->Destroy();
		BlockEntity->OnRemoveFromWorld();
		RemoveBlockEntity(BlockEntity);
	}

	// If the new block is a block entity, create the entity object:
	if (cBlockEntity::IsBlockEntityBlockType(a_Block.Type()))
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
		((OldBlock.Type() == BlockType::Water)            || (a_Block.Type() == BlockType::Water)) ||  // Replacing water with stationary water
		((OldBlock.Type() == BlockType::Lava)             || (a_Block.Type() == BlockType::Lava))      // Replacing lava with stationary lava
	);

	if (!ReplacingLiquids)
	{
		MarkDirty();
	}

	m_BlockData.SetBlock({ a_RelX, a_RelY, a_RelZ }, a_Block);

	// Queue block to be sent only if ...
	if (
		!(cBlockLeavesHandler::IsBlockLeaves(OldBlock) || cBlockLeavesHandler::IsBlockLeaves(a_Block)) &&  // ... the old and new blocktypes AREN'T leaves (because the client doesn't need meta updates)
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
	if (a_RelY >= m_HeightMap[a_RelX + a_RelZ * cChunkDef::Width])
	{
		if (cBlockAirHandler::IsBlockAir(a_Block.Type()))
		{
			m_HeightMap[a_RelX + a_RelZ * cChunkDef::Width] = static_cast<HEIGHTTYPE>(a_RelY);
		}
		else
		{
			for (int y = a_RelY - 1; y > 0; --y)
			{
				if (!cBlockAirHandler::IsBlockAir(GetBlock(a_RelX, y, a_RelZ)))
				{
					m_HeightMap[a_RelX + a_RelZ * cChunkDef::Width] = static_cast<HEIGHTTYPE>(y);
					break;
				}
			}  // for y - column in m_BlockData
		}
	}
}





void cChunk::SendBlockTo(int a_RelX, int a_RelY, int a_RelZ, cClientHandle * a_Client)
{

	if (a_Client == nullptr)
	{
		// Queue the block for all clients in the Chunk (will be sent in Tick())
		m_PendingSendBlocks.emplace_back(m_PosX, m_PosZ, a_RelX, a_RelY, a_RelZ, GetBlock(a_RelX, a_RelY, a_RelZ));
		return;
	}

	Vector3i wp = PositionToWorldPosition(a_RelX, a_RelY, a_RelZ);
	a_Client->SendBlockChange(wp.x, wp.y, wp.z, GetBlock(a_RelX, a_RelY, a_RelZ));

	// FS #268 - if a BlockEntity digging is cancelled by a plugin, the entire block entity must be re-sent to the client:
	cBlockEntity * Block = GetBlockEntity(wp.x, wp.y, wp.z);
	if (Block != nullptr)
	{
		Block->SendTo(*a_Client);
	}
}





void cChunk::AddBlockEntity(OwnedBlockEntity a_BlockEntity)
{
	[[maybe_unused]] const auto Result = m_BlockEntities.emplace(
		cChunkDef::MakeIndex(a_BlockEntity->GetRelX(), a_BlockEntity->GetPosY(), a_BlockEntity->GetRelZ()),
		std::move(a_BlockEntity)
	);
	ASSERT(Result.second);  // No block entity already at this position
}





cBlockEntity * cChunk::GetBlockEntity(Vector3i a_AbsPos)
{
	const auto relPos = cChunkDef::AbsoluteToRelative(a_AbsPos);

	if (!cChunkDef::IsValidRelPos(relPos))
	{
		// Coordinates are outside outside this Chunk, no block entities here
		return nullptr;
	}

	auto itr = m_BlockEntities.find(static_cast<size_t>(cChunkDef::MakeIndexNoCheck(relPos)));
	return (itr == m_BlockEntities.end()) ? nullptr : itr->second.get();
}





cBlockEntity * cChunk::GetBlockEntityRel(Vector3i a_RelPos)
{
	ASSERT(cChunkDef::IsValidRelPos(a_RelPos));
	auto itr = m_BlockEntities.find(static_cast<size_t>(cChunkDef::MakeIndexNoCheck(a_RelPos)));
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





bool cChunk::SetSignLines(int a_PosX, int a_PosY, int a_PosZ, const AString & a_Line1, const AString & a_Line2, const AString & a_Line3, const AString & a_Line4)
{
	// Also sends update packets to all clients in the Chunk
	auto Entity = GetBlockEntity(a_PosX, a_PosY, a_PosZ);
	if (Entity == nullptr)
	{
		return false;  // Not a block entity
	}
	if (
		(!cBlockWallSignHandler::IsBlockWallSign(Entity->GetBlockType())) &&
		(!cBlockSignPostHandler::IsBlockSignPost(Entity->GetBlockType()))
	)
	{
		return false;  // Not a sign
	}

	MarkDirty();
	auto Sign = static_cast<cSignEntity *>(Entity);
	Sign->SetLines(a_Line1, a_Line2, a_Line3, a_Line4);
	m_World->BroadcastBlockEntity({a_PosX, a_PosY, a_PosZ});
	return true;
}





void cChunk::RemoveBlockEntity(cBlockEntity * a_BlockEntity)
{
	MarkDirty();
	ASSERT(a_BlockEntity != nullptr);
	auto idx = static_cast<size_t>(cChunkDef::MakeIndex(a_BlockEntity->GetRelX(), a_BlockEntity->GetPosY(), a_BlockEntity->GetRelZ()));
	m_BlockEntities.erase(idx);
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





template <class tyEntity, BlockType... tBlocktype>
bool cChunk::GenericForEachBlockEntity(cFunctionRef<bool(tyEntity &)> a_Callback)
{
	// The blockentity list is locked by the parent Chunkmap's CS
	for (auto & KeyPair : m_BlockEntities)
	{
		cBlockEntity * Block = KeyPair.second.get();
		if (
			(sizeof...(tBlocktype) == 0) ||  // Let empty list mean all block entities
			(IsOneOf<tBlocktype...>(Block->GetBlockType()))
		)
		{
			if (a_Callback(*static_cast<tyEntity *>(Block)))
			{
				return false;
			}
		}
	}
	return true;
}





bool cChunk::ForEachBlockEntity(cBlockEntityCallback a_Callback)
{
	return GenericForEachBlockEntity<cBlockEntity>(a_Callback);
}





bool cChunk::ForEachBrewingstand(cBrewingstandCallback a_Callback)
{
	return GenericForEachBlockEntity<cBrewingstandEntity,
		BlockType::BrewingStand
	>(a_Callback);
}





bool cChunk::ForEachChest(cChestCallback a_Callback)
{
	return GenericForEachBlockEntity<cChestEntity,
		BlockType::Chest
	>(a_Callback);
}





bool cChunk::ForEachDispenser(cDispenserCallback a_Callback)
{
	return GenericForEachBlockEntity<cDispenserEntity,
		BlockType::Dispenser
	>(a_Callback);
}





bool cChunk::ForEachDropper(cDropperCallback a_Callback)
{
	return GenericForEachBlockEntity<cDropperEntity,
		BlockType::Dropper
	>(a_Callback);
}





bool cChunk::ForEachDropSpenser(cDropSpenserCallback a_Callback)
{
	return GenericForEachBlockEntity<cDropSpenserEntity,
		BlockType::Dispenser,
		BlockType::Dropper
	>(a_Callback);
}





bool cChunk::ForEachFurnace(cFurnaceCallback a_Callback)
{
	return GenericForEachBlockEntity<cFurnaceEntity,
		BlockType::Furnace
	>(a_Callback);
}





template <class tyEntity, BlockType... tBlocktype>
bool cChunk::GenericDoWithBlockEntityAt(Vector3i a_Position, cFunctionRef<bool(tyEntity &)> a_Callback)
{
	// The blockentity list is locked by the parent Chunkmap's CS
	cBlockEntity * Block = GetBlockEntityRel(a_Position);
	if (Block == nullptr)
	{
		return false;  // No block entity here
	}
	if (
		(sizeof...(tBlocktype) != 0) &&  // Let empty list mean all block entities
		(!IsOneOf<tBlocktype...>(Block->GetBlockType()))
	)
	{
		return false;  // Not any of the given tBlocktypes
	}
	return !a_Callback(*static_cast<tyEntity *>(Block));
}





bool cChunk::DoWithBlockEntityAt(Vector3i a_Position, cBlockEntityCallback a_Callback)
{
	return GenericDoWithBlockEntityAt<cBlockEntity>(a_Position, a_Callback);
}





bool cChunk::DoWithBeaconAt(Vector3i a_Position, cBeaconCallback a_Callback)
{
	return GenericDoWithBlockEntityAt<cBeaconEntity,
		BlockType::Beacon
	>(a_Position, a_Callback);
}





bool cChunk::DoWithBedAt(Vector3i a_Position, cBedCallback a_Callback)
{
	return GenericDoWithBlockEntityAt<cBedEntity,
		BlockType::BlackBed,
		BlockType::BlueBed,
		BlockType::BrownBed,
		BlockType::CyanBed,
		BlockType::GrayBed,
		BlockType::GreenBed,
		BlockType::LightBlueBed,
		BlockType::LightGrayBed,
		BlockType::LimeBed,
		BlockType::MagentaBed,
		BlockType::OrangeBed,
		BlockType::PinkBed,
		BlockType::PurpleBed,
		BlockType::RedBed,
		BlockType::WhiteBed,
		BlockType::YellowBed
	>(a_Position, a_Callback);
}





bool cChunk::DoWithBrewingstandAt(Vector3i a_Position, cBrewingstandCallback a_Callback)
{
	return GenericDoWithBlockEntityAt<cBrewingstandEntity,
		BlockType::BrewingStand
	>(a_Position, a_Callback);
}





bool cChunk::DoWithChestAt(Vector3i a_Position, cChestCallback a_Callback)
{
	return GenericDoWithBlockEntityAt<cChestEntity,
		BlockType::Chest,
		BlockType::TrappedChest
	>(a_Position, a_Callback);
}





bool cChunk::DoWithDispenserAt(Vector3i a_Position, cDispenserCallback a_Callback)
{
	return GenericDoWithBlockEntityAt<cDispenserEntity,
		BlockType::Dispenser
	>(a_Position, a_Callback);
}





bool cChunk::DoWithDropperAt(Vector3i a_Position, cDropperCallback a_Callback)
{
	return GenericDoWithBlockEntityAt<cDropperEntity,
		BlockType::Dropper
	>(a_Position, a_Callback);
}





bool cChunk::DoWithDropSpenserAt(Vector3i a_Position, cDropSpenserCallback a_Callback)
{
	return GenericDoWithBlockEntityAt<cDropSpenserEntity,
		BlockType::Dispenser,
		BlockType::Dropper
	>(a_Position, a_Callback);
}





bool cChunk::DoWithFurnaceAt(Vector3i a_Position, cFurnaceCallback a_Callback)
{
	return GenericDoWithBlockEntityAt<cFurnaceEntity,
		BlockType::Furnace
	>(a_Position, a_Callback);
}





bool cChunk::DoWithHopperAt(Vector3i a_Position, cHopperCallback a_Callback)
{
	return GenericDoWithBlockEntityAt<cHopperEntity,
		BlockType::Hopper
	>(a_Position, a_Callback);
}





bool cChunk::DoWithNoteBlockAt(Vector3i a_Position, cNoteBlockCallback a_Callback)
{
	return GenericDoWithBlockEntityAt<cNoteEntity,
		BlockType::NoteBlock
	>(a_Position, a_Callback);
}





bool cChunk::DoWithCommandBlockAt(Vector3i a_Position, cCommandBlockCallback a_Callback)
{
	return GenericDoWithBlockEntityAt<cCommandBlockEntity,
		BlockType::CommandBlock,
		BlockType::ChainCommandBlock,
		BlockType::RepeatingCommandBlock
	>(a_Position, a_Callback);
}





bool cChunk::DoWithMobHeadAt(Vector3i a_Position, cMobHeadCallback a_Callback)
{
	return GenericDoWithBlockEntityAt<cMobHeadEntity,
	BlockType::CreeperHead,
	BlockType::CreeperWallHead,
	BlockType::DragonHead,
	BlockType::DragonWallHead,
	BlockType::PlayerHead,
	BlockType::PlayerWallHead,
	BlockType::SkeletonSkull,
	BlockType::SkeletonWallSkull,
	BlockType::WitherSkeletonSkull,
	BlockType::WitherSkeletonWallSkull,
	BlockType::ZombieHead,
	BlockType::ZombieWallHead
	>(a_Position, a_Callback);
}





bool cChunk::DoWithFlowerPotAt(Vector3i a_Position, cFlowerPotCallback a_Callback)
{
	return GenericDoWithBlockEntityAt<cFlowerPotEntity,
	BlockType::PottedAcaciaSapling,
	BlockType::PottedAzureBluet,
	BlockType::PottedBamboo,
	BlockType::PottedBirchSapling,
	BlockType::PottedBlueOrchid,
	BlockType::PottedBrownMushroom,
	BlockType::PottedCactus,
	BlockType::PottedCornflower,
	BlockType::PottedCrimsonRoots,
	BlockType::PottedCrimsonFungus,
	BlockType::PottedDandelion,
	BlockType::PottedDarkOakSapling,
	BlockType::PottedDeadBush,
	BlockType::PottedFern,
	BlockType::PottedJungleSapling,
	BlockType::PottedLilyOfTheValley,
	BlockType::PottedOakSapling,
	BlockType::PottedOrangeTulip,
	BlockType::PottedOxeyeDaisy,
	BlockType::PottedPinkTulip,
	BlockType::PottedPoppy,
	BlockType::PottedRedMushroom,
	BlockType::PottedRedTulip,
	BlockType::PottedSpruceSapling,
	BlockType::PottedWarpedFungus,
	BlockType::PottedWarpedRoots,
	BlockType::PottedWhiteTulip,
	BlockType::PottedWitherRose,
	BlockType::PottedAllium
	>(a_Position, a_Callback);
}





bool cChunk::GetSignLines(Vector3i a_Position, AString & a_Line1, AString & a_Line2, AString & a_Line3, AString & a_Line4)
{
	// The blockentity list is locked by the parent Chunkmap's CS
	auto Entity = GetBlockEntity(a_Position);
	if (Entity == nullptr)
	{
		return false;  // Not a block entity
	}
	if (
		cBlockWallSignHandler::IsBlockWallSign(Entity->GetBlockType()) &&
		cBlockSignPostHandler::IsBlockSignPost(Entity->GetBlockType())
	)
	{
		return false;  // Not a sign
	}

	auto Sign = static_cast<cSignEntity *>(Entity);
	a_Line1 = Sign->GetLine(0);
	a_Line2 = Sign->GetLine(1);
	a_Line3 = Sign->GetLine(2);
	a_Line4 = Sign->GetLine(3);
	return true;
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
