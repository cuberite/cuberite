
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#ifndef _WIN32
	#include <cstdlib>
#endif


#include "Chunk.h"
#include "BlockInfo.h"
#include "World.h"
#include "ClientHandle.h"
#include "Server.h"
#include "zlib/zlib.h"
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

#include "json/json.h"





////////////////////////////////////////////////////////////////////////////////
// cChunk:

cChunk::cChunk(
	int a_ChunkX, int a_ChunkZ,
	cChunkMap * a_ChunkMap, cWorld * a_World,
	cAllocationPool<cChunkData::sChunkSection> & a_Pool
):
	m_Presence(cpInvalid),
	m_IsLightValid(false),
	m_IsDirty(false),
	m_IsSaving(false),
	m_HasLoadFailed(false),
	m_StayCount(0),
	m_PosX(a_ChunkX),
	m_PosZ(a_ChunkZ),
	m_World(a_World),
	m_ChunkMap(a_ChunkMap),
	m_ChunkData(a_Pool),
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

	// Tell all clients attached to this chunk that they want this chunk:
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
		m_LoadedByClient.empty() &&  // The chunk is not used by any client
		!HasPlayerEntities() &&      // Ensure not only the absence of ClientHandlers, but also of cPlayer objects
		!m_IsDirty &&                // The chunk has been saved properly or hasn't been touched since the load / gen
		(m_StayCount == 0) &&        // The chunk is not in a ChunkStay
		(m_Presence != cpQueued) ;   // The chunk is not queued for loading / generating (otherwise multi-load / multi-gen could occur)
}





bool cChunk::CanUnloadAfterSaving(void) const
{
	return
		m_LoadedByClient.empty() &&  // The chunk is not used by any client
		!HasPlayerEntities() &&      // Ensure not only the absence of ClientHandlers, but also of cPlayer objects
		m_IsDirty &&                 // The chunk is dirty
		(m_StayCount == 0) &&        // The chunk is not in a ChunkStay
		(m_Presence != cpQueued) ;   // The chunk is not queued for loading / generating (otherwise multi-load / multi-gen could occur)
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

	// The chunk is always needed, generate it:
	m_World->GetGenerator().QueueGenerateChunk({m_PosX, m_PosZ}, false);
}





void cChunk::GetAllData(cChunkDataCallback & a_Callback)
{
	ASSERT(m_Presence == cpPresent);

	a_Callback.HeightMap(&m_HeightMap);
	a_Callback.BiomeData(&m_BiomeMap);

	a_Callback.LightIsValid(m_IsLightValid);

	a_Callback.ChunkData(m_ChunkData);

	for (const auto & Entity : m_Entities)
	{
		a_Callback.Entity(Entity.get());
	}

	for (auto & KeyPair : m_BlockEntities)
	{
		a_Callback.BlockEntity(KeyPair.second.get());
	}
}





void cChunk::SetAllData(cSetChunkData & a_SetChunkData)
{
	ASSERT(a_SetChunkData.IsHeightMapValid());
	ASSERT(a_SetChunkData.AreBiomesValid());

	memcpy(m_BiomeMap, a_SetChunkData.GetBiomes(), sizeof(m_BiomeMap));
	memcpy(m_HeightMap, a_SetChunkData.GetHeightMap(), sizeof(m_HeightMap));

	m_ChunkData.Assign(std::move(a_SetChunkData.GetChunkData()));
	m_IsLightValid = a_SetChunkData.IsLightValid();

	// Clear the block entities present - either the loader / saver has better, or we'll create empty ones:
	m_BlockEntities = std::move(a_SetChunkData.GetBlockEntities());

	// Check that all block entities have a valid blocktype at their respective coords (DEBUG-mode only):
	#ifdef _DEBUG
		for (auto & KeyPair : m_BlockEntities)
		{
			cBlockEntity * Block = KeyPair.second.get();
			BLOCKTYPE EntityBlockType = Block->GetBlockType();
			BLOCKTYPE WorldBlockType = GetBlock(Block->GetRelX(), Block->GetPosY(), Block->GetRelZ());
			ASSERT(WorldBlockType == EntityBlockType);
		}  // for KeyPair - m_BlockEntities
	#endif  // _DEBUG

	// Set all block entities' World variable:
	for (auto & KeyPair : m_BlockEntities)
	{
		KeyPair.second->SetWorld(m_World);
	}

	// Set the chunk data as valid. This may be needed for some simulators that perform actions upon block adding (Vaporize)
	SetPresence(cpPresent);

	// Wake up all simulators for their respective blocks:
	WakeUpSimulators();

	m_HasLoadFailed = false;
}





void cChunk::SetLight(
	const cChunkDef::BlockNibbles & a_BlockLight,
	const cChunkDef::BlockNibbles & a_SkyLight
)
{
	// TODO: We might get cases of wrong lighting when a chunk changes in the middle of a lighting calculation.
	// Postponing until we see how bad it is :)

	m_ChunkData.SetBlockLight(a_BlockLight);

	m_ChunkData.SetSkyLight(a_SkyLight);

	m_IsLightValid = true;
}





void cChunk::GetBlockTypes(BLOCKTYPE * a_BlockTypes)
{
	m_ChunkData.CopyBlockTypes(a_BlockTypes);
}





void cChunk::WriteBlockArea(cBlockArea & a_Area, int a_MinBlockX, int a_MinBlockY, int a_MinBlockZ, int a_DataTypes)
{
	if ((a_DataTypes & (cBlockArea::baTypes | cBlockArea::baMetas)) != (cBlockArea::baTypes | cBlockArea::baMetas))
	{
		LOGWARNING("cChunk::WriteBlockArea(): unsupported datatype request, can write only types + metas together (0x%x), requested 0x%x. Ignoring.",
			(cBlockArea::baTypes | cBlockArea::baMetas), a_DataTypes & (cBlockArea::baTypes | cBlockArea::baMetas)
		);
		return;
	}

	// SizeX, SizeZ are the dimensions of the block data to copy to the chunk (size of the geometric union)

	int BlockStartX = std::max(a_MinBlockX, m_PosX * cChunkDef::Width);
	int BlockEndX   = std::min(a_MinBlockX + a_Area.GetSizeX(), (m_PosX + 1) * cChunkDef::Width);
	int BlockStartZ = std::max(a_MinBlockZ, m_PosZ * cChunkDef::Width);
	int BlockEndZ   = std::min(a_MinBlockZ + a_Area.GetSizeZ(), (m_PosZ + 1) * cChunkDef::Width);
	int SizeX = BlockEndX - BlockStartX;  // Size of the union
	int SizeZ = BlockEndZ - BlockStartZ;
	int SizeY = std::min(a_Area.GetSizeY(), cChunkDef::Height - a_MinBlockY);
	int OffX = BlockStartX - m_PosX * cChunkDef::Width;  // Offset within the chunk where the union starts
	int OffZ = BlockStartZ - m_PosZ * cChunkDef::Width;
	int BaseX = BlockStartX - a_MinBlockX;  // Offset within the area where the union starts
	int BaseZ = BlockStartZ - a_MinBlockZ;

	// Copy blocktype and blockmeta:
	BLOCKTYPE *  AreaBlockTypes = a_Area.GetBlockTypes();
	NIBBLETYPE * AreaBlockMetas = a_Area.GetBlockMetas();
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
				BLOCKTYPE BlockType = AreaBlockTypes[idx];
				NIBBLETYPE BlockMeta = AreaBlockMetas[idx];
				FastSetBlock(ChunkX, ChunkY, ChunkZ, BlockType, BlockMeta);
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
			itr = m_BlockEntities.erase(itr);
		}
		else
		{
			++itr;
		}
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
			// This block entity is inside the chunk, clone it (and remove any that is there currently):
			auto idx = static_cast<size_t>(MakeIndex(posX - m_PosX * cChunkDef::Width, posY, posZ - m_PosZ * cChunkDef::Width));
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
	m_StayCount += (a_Stay ? 1 : -1);
	ASSERT(m_StayCount >= 0);
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

	GetThreeRandomNumbers(a_X, a_Y, a_Z, Width, Height - 2, Width);
	a_Y++;
}





void cChunk::SpawnMobs(cMobSpawner & a_MobSpawner)
{
	int CenterX, CenterY, CenterZ;
	GetRandomBlockCoords(CenterX, CenterY, CenterZ);

	BLOCKTYPE PackCenterBlock = GetBlock(CenterX, CenterY, CenterZ);
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

		EMCSBiome Biome = m_ChunkMap->GetBiomeAt(WorldX, WorldZ);
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
		if (!IsLightValid())
		{
			continue;
		}

		auto newMob = a_MobSpawner.TryToSpawnHere(this, {TryX, TryY, TryZ}, Biome, MaxNbOfSuccess);
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
	// If we are not valid, tick players and bailout
	if (!IsValid())
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

	CheckBlocks();

	// Tick simulators:
	m_World->GetSimulatorManager()->SimulateChunk(a_Dt, m_PosX, m_PosZ, this);

	TickBlocks();

	// Tick all block entities in this chunk:
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
			// Tick all entities in this chunk (except mobs):
			ASSERT((*itr)->GetParentChunk() == this);
			(*itr)->Tick(a_Dt, *this);
			ASSERT((*itr)->GetParentChunk() == this);
		}

		// Do not move mobs that are detached from the world to neighbors. They're either scheduled for teleportation or for removal.
		// Because the schedulded destruction is going to look for them in this chunk. See cEntity::destroy.
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
			// The entity moved out of the chunk, move it to the neighbor
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

	BroadcastPendingBlockChanges();
}





void cChunk::TickBlock(const Vector3i a_RelPos)
{
	cChunkInterface ChunkInterface(this->GetWorld()->GetChunkMap());
	cBlockInServerPluginInterface PluginInterface(*this->GetWorld());
	cBlockHandler::For(GetBlock(a_RelPos)).OnUpdate(ChunkInterface, *this->GetWorld(), PluginInterface, *this, a_RelPos);
}





void cChunk::MoveEntityToNewChunk(OwnedEntity a_Entity)
{
	cChunk * Neighbor = GetNeighborChunk(a_Entity->GetChunkX() * cChunkDef::Width, a_Entity->GetChunkZ() * cChunkDef::Width);
	if (Neighbor == nullptr)
	{
		LOGWARNING("%s: Failed to move entity, destination chunk unreachable. Entity lost", __FUNCTION__);
		return;
	}

	ASSERT(Neighbor != this);  // Moving into the same chunk? wtf?
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
		// Resend the full chunk
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

		cBlockHandler::For(GetBlock(Pos)).Check(ChunkInterface, PluginInterface, Pos, *this);
	}
}





void cChunk::TickBlocks(void)
{
	cChunkInterface ChunkInterface(this->GetWorld()->GetChunkMap());
	cBlockInServerPluginInterface PluginInterface(*this->GetWorld());

	// Tick random blocks, but the first one should be m_BlockToTick (so that SetNextBlockToTick() works)
	auto Idx = cChunkDef::MakeIndexNoCheck(m_BlockToTick);
	for (int i = 0; i < 50; ++i)
	{
		auto Pos = cChunkDef::IndexToCoordinate(static_cast<size_t>(Idx));
		Idx = m_World->GetTickRandomNumber(cChunkDef::NumBlocks - 1);
		if (Pos.y > cChunkDef::GetHeight(m_HeightMap, Pos.x, Pos.z))
		{
			continue;  // It's all air up here
		}

		cBlockHandler::For(GetBlock(Pos)).OnUpdate(ChunkInterface, *this->GetWorld(), PluginInterface, *this, Pos);
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

	BLOCKTYPE TopBlock = GetBlock(X, Height, Z);
	NIBBLETYPE TopMeta = GetMeta (X, Height, Z);
	if (m_World->IsDeepSnowEnabled() && (TopBlock == E_BLOCK_SNOW))
	{
		int MaxSize = 7;
		BLOCKTYPE  BlockType[4];
		NIBBLETYPE BlockMeta[4];
		UnboundedRelGetBlock(X - 1, Height, Z,     BlockType[0], BlockMeta[0]);
		UnboundedRelGetBlock(X + 1, Height, Z,     BlockType[1], BlockMeta[1]);
		UnboundedRelGetBlock(X,     Height, Z - 1, BlockType[2], BlockMeta[2]);
		UnboundedRelGetBlock(X,     Height, Z + 1, BlockType[3], BlockMeta[3]);
		for (int i = 0; i < 4; i++)
		{
			switch (BlockType[i])
			{
				case E_BLOCK_AIR:
				{
					MaxSize = 0;
					break;
				}
				case E_BLOCK_SNOW:
				{
					MaxSize = std::min(BlockMeta[i] + 1, MaxSize);
					break;
				}
			}
		}
		if (TopMeta < MaxSize)
		{
			FastSetBlock(X, Height, Z, E_BLOCK_SNOW, TopMeta + 1);
		}
		else if (TopMeta > MaxSize)
		{
			FastSetBlock(X, Height, Z, E_BLOCK_SNOW, TopMeta - 1);
		}
	}
	else if (cBlockInfo::IsSnowable(TopBlock) && (Height < cChunkDef::Height - 1))
	{
		SetBlock({X, Height + 1, Z}, E_BLOCK_SNOW, 0);
	}
	else if (IsBlockWater(TopBlock) && (TopMeta == 0))
	{
		SetBlock({X, Height, Z}, E_BLOCK_ICE, 0);
	}
	else if (
		(m_World->IsDeepSnowEnabled()) &&
		(
			(TopBlock == E_BLOCK_RED_ROSE) ||
			(TopBlock == E_BLOCK_YELLOW_FLOWER) ||
			(TopBlock == E_BLOCK_RED_MUSHROOM) ||
			(TopBlock == E_BLOCK_BROWN_MUSHROOM)
		)
	)
	{
		SetBlock({X, Height, Z}, E_BLOCK_SNOW, 0);
	}
}





cItems cChunk::PickupsFromBlock(Vector3i a_RelPos, const cEntity * a_Digger, const cItem * a_Tool)
{
	BLOCKTYPE BlockType;
	NIBBLETYPE BlockMeta;
	GetBlockTypeMeta(a_RelPos, BlockType, BlockMeta);

	cItems Pickups;
	const auto BlockEntity = GetBlockEntityRel(a_RelPos);

	const auto ToolHandler = (a_Tool != nullptr) ? a_Tool->GetHandler() : cItemHandler::GetItemHandler(E_ITEM_EMPTY);
	if (ToolHandler->CanHarvestBlock(BlockType))
	{
		Pickups = cBlockHandler::For(BlockType).ConvertToPickups(BlockMeta, a_Digger, a_Tool);

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
		BlockType, BlockMeta, BlockEntity,
		a_Digger, a_Tool, Pickups
	);

	return Pickups;
}





int cChunk::GrowPlantAt(Vector3i a_RelPos, int a_NumStages)
{
	return cBlockHandler::For(GetBlock(a_RelPos)).Grow(*this, a_RelPos, a_NumStages);
}





bool cChunk::UnboundedRelGetBlock(Vector3i a_RelPos, BLOCKTYPE & a_BlockType, NIBBLETYPE & a_BlockMeta) const
{
	if (!cChunkDef::IsValidHeight(a_RelPos.y))
	{
		LOGWARNING("%s: requesting a block with a_RelY out of range: %d", __FUNCTION__, a_RelPos.y);
		return false;
	}
	auto chunk = GetRelNeighborChunkAdjustCoords(a_RelPos);
	if ((chunk == nullptr) || !chunk->IsValid())
	{
		// The chunk is not available, bail out
		return false;
	}
	chunk->GetBlockTypeMeta(a_RelPos, a_BlockType, a_BlockMeta);
	return true;
}





bool cChunk::UnboundedRelGetBlockType(Vector3i a_RelPos, BLOCKTYPE & a_BlockType) const
{
	if (!cChunkDef::IsValidHeight(a_RelPos.y))
	{
		LOGWARNING("%s: requesting a block with a_RelY out of range: %d", __FUNCTION__, a_RelPos.y);
		return false;
	}
	auto chunk = GetRelNeighborChunkAdjustCoords(a_RelPos);
	if ((chunk == nullptr) || !chunk->IsValid())
	{
		// The chunk is not available, bail out
		return false;
	}
	a_BlockType = chunk->GetBlock(a_RelPos);
	return true;
}





bool cChunk::UnboundedRelGetBlockMeta(Vector3i a_RelPos, NIBBLETYPE & a_BlockMeta) const
{
	if (!cChunkDef::IsValidHeight(a_RelPos.y))
	{
		LOGWARNING("%s: requesting a block with a_RelY out of range: %d", __FUNCTION__, a_RelPos.y);
		return false;
	}
	auto chunk = GetRelNeighborChunkAdjustCoords(a_RelPos);
	if ((chunk == nullptr) || !chunk->IsValid())
	{
		// The chunk is not available, bail out
		return false;
	}
	a_BlockMeta = chunk->GetMeta(a_RelPos);
	return true;
}





bool cChunk::UnboundedRelGetBlockBlockLight(Vector3i a_RelPos, NIBBLETYPE & a_BlockBlockLight) const
{
	if (!cChunkDef::IsValidHeight(a_RelPos.y))
	{
		LOGWARNING("%s: requesting a block with a_RelY out of range: %d", __FUNCTION__, a_RelPos.y);
		return false;
	}
	auto chunk = GetRelNeighborChunkAdjustCoords(a_RelPos);
	if ((chunk == nullptr) || !chunk->IsValid())
	{
		// The chunk is not available, bail out
		return false;
	}
	a_BlockBlockLight = chunk->GetBlockLight(a_RelPos);
	return true;
}





bool cChunk::UnboundedRelGetBlockSkyLight(Vector3i a_RelPos, NIBBLETYPE & a_BlockSkyLight) const
{
	if (!cChunkDef::IsValidHeight(a_RelPos.y))
	{
		LOGWARNING("%s: requesting a block with a_RelY out of range: %d", __FUNCTION__, a_RelPos.y);
		return false;
	}
	auto chunk = GetRelNeighborChunkAdjustCoords(a_RelPos);
	if ((chunk == nullptr) || !chunk->IsValid())
	{
		// The chunk is not available, bail out
		return false;
	}
	a_BlockSkyLight = chunk->GetSkyLight(a_RelPos);
	return true;
}





bool cChunk::UnboundedRelGetBlockLights(Vector3i a_RelPos, NIBBLETYPE & a_BlockLight, NIBBLETYPE & a_SkyLight) const
{
	if (!cChunkDef::IsValidHeight(a_RelPos.y))
	{
		LOGWARNING("%s: requesting a block with a_RelY out of range: %d", __FUNCTION__, a_RelPos.y);
		return false;
	}
	auto chunk = GetRelNeighborChunkAdjustCoords(a_RelPos);
	if ((chunk == nullptr) || !chunk->IsValid())
	{
		// The chunk is not available, bail out
		return false;
	}
	a_BlockLight = chunk->GetBlockLight(a_RelPos);
	a_SkyLight   = chunk->GetSkyLight  (a_RelPos);
	return true;
}





bool cChunk::UnboundedRelSetBlock(Vector3i a_RelPos, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta)
{
	if (!cChunkDef::IsValidHeight(a_RelPos.y))
	{
		LOGWARNING("%s: requesting a block with a_RelY out of range: %d", __FUNCTION__, a_RelPos.y);
		return false;
	}
	auto chunk = GetRelNeighborChunkAdjustCoords(a_RelPos);
	if ((chunk == nullptr) || !chunk->IsValid())
	{
		// The chunk is not available, bail out
		return false;
	}
	chunk->SetBlock(a_RelPos, a_BlockType, a_BlockMeta);
	return true;
}





bool cChunk::UnboundedRelFastSetBlock(Vector3i a_RelPos, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta)
{
	if (!cChunkDef::IsValidHeight(a_RelPos.y))
	{
		LOGWARNING("%s: requesting a block with a_RelY out of range: %d", __FUNCTION__, a_RelPos.y);
		return false;
	}
	auto chunk = GetRelNeighborChunkAdjustCoords(a_RelPos);
	if ((chunk == nullptr) || !chunk->IsValid())
	{
		// The chunk is not available, bail out
		return false;
	}
	chunk->FastSetBlock(a_RelPos, a_BlockType, a_BlockMeta);
	return true;
}





int cChunk::GetHeight(int a_X, int a_Z)
{
	ASSERT((a_X >= 0) && (a_X < Width) && (a_Z >= 0) && (a_Z < Width));

	if ((a_X >= 0) && (a_X < Width) && (a_Z >= 0) && (a_Z < Width))
	{
		return m_HeightMap[a_X + a_Z * Width];
	}
	return 0;
}





void cChunk::WakeUpSimulators(void)
{
	auto * WaterSimulator = m_World->GetWaterSimulator();
	auto * LavaSimulator  = m_World->GetLavaSimulator();
	auto * RedstoneSimulator = m_World->GetRedstoneSimulator();

	for (size_t SectionIdx = 0; SectionIdx != cChunkData::NumSections; ++SectionIdx)
	{
		const auto * Section = m_ChunkData.GetSection(SectionIdx);
		if (Section == nullptr)
		{
			continue;
		}

		for (size_t BlockIdx = 0; BlockIdx != cChunkData::SectionBlockCount; ++BlockIdx)
		{
			const auto BlockType = Section->m_BlockTypes[BlockIdx];
			auto Position = IndexToCoordinate(BlockIdx);
			Position.y += static_cast<int>(SectionIdx * cChunkData::SectionHeight);

			RedstoneSimulator->AddBlock(*this, Position, BlockType);
			WaterSimulator->AddBlock(*this, Position, BlockType);
			LavaSimulator->AddBlock(*this, Position, BlockType);
		}
	}
}





void cChunk::CalculateHeightmap(const BLOCKTYPE * a_BlockTypes)
{
	for (int x = 0; x < Width; x++)
	{
		for (int z = 0; z < Width; z++)
		{
			for (int y = Height - 1; y > -1; y--)
			{
				int index = MakeIndex( x, y, z);
				if (a_BlockTypes[index] != E_BLOCK_AIR)
				{
					m_HeightMap[x + z * Width] = static_cast<HEIGHTTYPE>(y);
					break;
				}
			}  // for y
		}  // for z
	}  // for x
}





void cChunk::SetBlock(Vector3i a_RelPos, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta)
{
	FastSetBlock(a_RelPos, a_BlockType, a_BlockMeta);

	// Tick this block's neighbors via cBlockHandler::Check:
	m_ToTickBlocks.push(a_RelPos);

	// Wake up the simulators for this block:
	GetWorld()->GetSimulatorManager()->WakeUp(*this, a_RelPos);

	// If there was a block entity, remove it:
	cBlockEntity * BlockEntity = GetBlockEntityRel(a_RelPos);
	if (BlockEntity != nullptr)
	{
		BlockEntity->Destroy();
		RemoveBlockEntity(BlockEntity);
	}

	// If the new block is a block entity, create the entity object:
	if (cBlockEntity::IsBlockEntityBlockType(a_BlockType))
	{
		AddBlockEntity(cBlockEntity::CreateByBlockType(a_BlockType, a_BlockMeta, RelativeToAbsolute(a_RelPos), m_World));
	}
}





void cChunk::FastSetBlock(int a_RelX, int a_RelY, int a_RelZ, BLOCKTYPE a_BlockType, BLOCKTYPE a_BlockMeta)
{
	ASSERT(!((a_RelX < 0) || (a_RelX >= Width) || (a_RelY < 0) || (a_RelY >= Height) || (a_RelZ < 0) || (a_RelZ >= Width)));

	ASSERT(IsValid());

	const BLOCKTYPE OldBlockType = GetBlock(a_RelX, a_RelY, a_RelZ);
	const BLOCKTYPE OldBlockMeta = m_ChunkData.GetMeta({ a_RelX, a_RelY, a_RelZ });
	if ((OldBlockType == a_BlockType) && (OldBlockMeta == a_BlockMeta))
	{
		return;
	}

	bool ReplacingLiquids = (
		((OldBlockType == E_BLOCK_STATIONARY_WATER) && (a_BlockType == E_BLOCK_WATER)) ||             // Replacing stationary water with water
		((OldBlockType == E_BLOCK_WATER)            && (a_BlockType == E_BLOCK_STATIONARY_WATER)) ||  // Replacing water with stationary water
		((OldBlockType == E_BLOCK_STATIONARY_LAVA)  && (a_BlockType == E_BLOCK_LAVA)) ||              // Replacing stationary lava with lava
		((OldBlockType == E_BLOCK_LAVA)             && (a_BlockType == E_BLOCK_STATIONARY_LAVA))      // Replacing lava with stationary lava
	);

	if (!ReplacingLiquids)
	{
		MarkDirty();
	}

	m_ChunkData.SetBlock({ a_RelX, a_RelY, a_RelZ }, a_BlockType);

	// Queue block to be sent only if ...
	if (
		!(                                // ... the old and new blocktypes AREN'T leaves (because the client doesn't need meta updates)
			((OldBlockType == E_BLOCK_LEAVES) && (a_BlockType == E_BLOCK_LEAVES)) ||
			((OldBlockType == E_BLOCK_NEW_LEAVES) && (a_BlockType == E_BLOCK_NEW_LEAVES))
		) &&                              // ... AND ...
		(
			(OldBlockMeta != a_BlockMeta) || (!ReplacingLiquids)
		)
	)
	{
		m_PendingSendBlocks.push_back(sSetBlock(m_PosX, m_PosZ, a_RelX, a_RelY, a_RelZ, a_BlockType, a_BlockMeta));
	}

	m_ChunkData.SetMeta({ a_RelX, a_RelY, a_RelZ }, a_BlockMeta);

	// ONLY recalculate lighting if it's necessary!
	if (
		(cBlockInfo::GetLightValue        (OldBlockType) != cBlockInfo::GetLightValue        (a_BlockType)) ||
		(cBlockInfo::GetSpreadLightFalloff(OldBlockType) != cBlockInfo::GetSpreadLightFalloff(a_BlockType)) ||
		(cBlockInfo::IsTransparent        (OldBlockType) != cBlockInfo::IsTransparent        (a_BlockType))
	)
	{
		m_IsLightValid = false;
	}

	// Update heightmap, if needed:
	if (a_RelY >= m_HeightMap[a_RelX + a_RelZ * Width])
	{
		if (a_BlockType != E_BLOCK_AIR)
		{
			m_HeightMap[a_RelX + a_RelZ * Width] = static_cast<HEIGHTTYPE>(a_RelY);
		}
		else
		{
			for (int y = a_RelY - 1; y > 0; --y)
			{
				if (GetBlock(a_RelX, y, a_RelZ) != E_BLOCK_AIR)
				{
					m_HeightMap[a_RelX + a_RelZ * Width] = static_cast<HEIGHTTYPE>(y);
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
		// Queue the block for all clients in the chunk (will be sent in Tick())
		m_PendingSendBlocks.push_back(sSetBlock(m_PosX, m_PosZ, a_RelX, a_RelY, a_RelZ, GetBlock(a_RelX, a_RelY, a_RelZ), GetMeta(a_RelX, a_RelY, a_RelZ)));
		return;
	}

	Vector3i wp = PositionToWorldPosition(a_RelX, a_RelY, a_RelZ);
	a_Client->SendBlockChange(wp.x, wp.y, wp.z, GetBlock(a_RelX, a_RelY, a_RelZ), GetMeta(a_RelX, a_RelY, a_RelZ));

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
		MakeIndex(a_BlockEntity->GetRelX(), a_BlockEntity->GetPosY(), a_BlockEntity->GetRelZ()),
		std::move(a_BlockEntity)
	);
	ASSERT(Result.second);  // No block entity already at this position
}





cBlockEntity * cChunk::GetBlockEntity(Vector3i a_AbsPos)
{
	auto relPos = AbsoluteToRelative(a_AbsPos);

	if (!IsValidRelPos(relPos))
	{
		// Coordinates are outside outside this chunk, no block entities here
		return nullptr;
	}

	auto itr = m_BlockEntities.find(static_cast<size_t>(MakeIndexNoCheck(relPos)));
	return (itr == m_BlockEntities.end()) ? nullptr : itr->second.get();
}





cBlockEntity * cChunk::GetBlockEntityRel(Vector3i a_RelPos)
{
	ASSERT(IsValidRelPos(a_RelPos));
	auto itr = m_BlockEntities.find(static_cast<size_t>(MakeIndexNoCheck(a_RelPos)));
	return (itr == m_BlockEntities.end()) ? nullptr : itr->second.get();
}





bool cChunk::ShouldBeTicked(void) const
{
	return (HasAnyClients() || (m_AlwaysTicked > 0));
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

	// Re-send the chunk to all clients:
	for (auto ClientHandle : m_LoadedByClient)
	{
		m_World->ForceSendChunkTo(m_PosX, m_PosZ, cChunkSender::Priority::Medium, ClientHandle);
	}  // for itr - m_LoadedByClient[]
}





bool cChunk::SetSignLines(int a_PosX, int a_PosY, int a_PosZ, const AString & a_Line1, const AString & a_Line2, const AString & a_Line3, const AString & a_Line4)
{
	// Also sends update packets to all clients in the chunk
	auto Entity = GetBlockEntity(a_PosX, a_PosY, a_PosZ);
	if (Entity == nullptr)
	{
		return false;  // Not a block entity
	}
	if (
		(Entity->GetBlockType() != E_BLOCK_WALLSIGN) &&
		(Entity->GetBlockType() != E_BLOCK_SIGN_POST)
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
	auto idx = static_cast<size_t>(MakeIndex(a_BlockEntity->GetRelX(), a_BlockEntity->GetPosY(), a_BlockEntity->GetRelZ()));
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
			LOGD("chunk [%i, %i] destroying entity #%i for player \"%s\"",
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
	// The entity list is locked by the parent chunkmap's CS
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
	// The entity list is locked by the parent chunkmap's CS
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
	// The entity list is locked by the parent chunkmap's CS
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





template <class tyEntity, BLOCKTYPE... tBlocktype>
bool cChunk::GenericForEachBlockEntity(cFunctionRef<bool(tyEntity &)> a_Callback)
{
	// The blockentity list is locked by the parent chunkmap's CS
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
	}  // for KeyPair - m_BlockEntitites[]
	return true;
}





bool cChunk::ForEachBlockEntity(cBlockEntityCallback a_Callback)
{
	return GenericForEachBlockEntity<cBlockEntity>(a_Callback);
}





bool cChunk::ForEachBrewingstand(cBrewingstandCallback a_Callback)
{
	return GenericForEachBlockEntity<cBrewingstandEntity,
		E_BLOCK_BREWING_STAND
	>(a_Callback);
}





bool cChunk::ForEachChest(cChestCallback a_Callback)
{
	return GenericForEachBlockEntity<cChestEntity,
		E_BLOCK_CHEST
	>(a_Callback);
}





bool cChunk::ForEachDispenser(cDispenserCallback a_Callback)
{
	return GenericForEachBlockEntity<cDispenserEntity,
		E_BLOCK_DISPENSER
	>(a_Callback);
}





bool cChunk::ForEachDropper(cDropperCallback a_Callback)
{
	return GenericForEachBlockEntity<cDropperEntity,
		E_BLOCK_DROPPER
	>(a_Callback);
}





bool cChunk::ForEachDropSpenser(cDropSpenserCallback a_Callback)
{
	return GenericForEachBlockEntity<cDropSpenserEntity,
		E_BLOCK_DISPENSER,
		E_BLOCK_DROPPER
	>(a_Callback);
}





bool cChunk::ForEachFurnace(cFurnaceCallback a_Callback)
{
	return GenericForEachBlockEntity<cFurnaceEntity,
		E_BLOCK_FURNACE,
		E_BLOCK_LIT_FURNACE
	>(a_Callback);
}





template <class tyEntity, BLOCKTYPE... tBlocktype>
bool cChunk::GenericDoWithBlockEntityAt(Vector3i a_Position, cFunctionRef<bool(tyEntity &)> a_Callback)
{
	// The blockentity list is locked by the parent chunkmap's CS
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
		E_BLOCK_BEACON
	>(a_Position, a_Callback);
}





bool cChunk::DoWithBedAt(Vector3i a_Position, cBedCallback a_Callback)
{
	return GenericDoWithBlockEntityAt<cBedEntity,
		E_BLOCK_BED
	>(a_Position, a_Callback);
}





bool cChunk::DoWithBrewingstandAt(Vector3i a_Position, cBrewingstandCallback a_Callback)
{
	return GenericDoWithBlockEntityAt<cBrewingstandEntity,
		E_BLOCK_BREWING_STAND
	>(a_Position, a_Callback);
}





bool cChunk::DoWithChestAt(Vector3i a_Position, cChestCallback a_Callback)
{
	return GenericDoWithBlockEntityAt<cChestEntity,
		E_BLOCK_CHEST,
		E_BLOCK_TRAPPED_CHEST
	>(a_Position, a_Callback);
}





bool cChunk::DoWithDispenserAt(Vector3i a_Position, cDispenserCallback a_Callback)
{
	return GenericDoWithBlockEntityAt<cDispenserEntity,
		E_BLOCK_DISPENSER
	>(a_Position, a_Callback);
}





bool cChunk::DoWithDropperAt(Vector3i a_Position, cDropperCallback a_Callback)
{
	return GenericDoWithBlockEntityAt<cDropperEntity,
		E_BLOCK_DROPPER
	>(a_Position, a_Callback);
}





bool cChunk::DoWithDropSpenserAt(Vector3i a_Position, cDropSpenserCallback a_Callback)
{
	return GenericDoWithBlockEntityAt<cDropSpenserEntity,
		E_BLOCK_DISPENSER,
		E_BLOCK_DROPPER
	>(a_Position, a_Callback);
}





bool cChunk::DoWithFurnaceAt(Vector3i a_Position, cFurnaceCallback a_Callback)
{
	return GenericDoWithBlockEntityAt<cFurnaceEntity,
		E_BLOCK_FURNACE,
		E_BLOCK_LIT_FURNACE
	>(a_Position, a_Callback);
}





bool cChunk::DoWithHopperAt(Vector3i a_Position, cHopperCallback a_Callback)
{
	return GenericDoWithBlockEntityAt<cHopperEntity,
		E_BLOCK_HOPPER
	>(a_Position, a_Callback);
}





bool cChunk::DoWithNoteBlockAt(Vector3i a_Position, cNoteBlockCallback a_Callback)
{
	return GenericDoWithBlockEntityAt<cNoteEntity,
		E_BLOCK_NOTE_BLOCK
	>(a_Position, a_Callback);
}





bool cChunk::DoWithCommandBlockAt(Vector3i a_Position, cCommandBlockCallback a_Callback)
{
	return GenericDoWithBlockEntityAt<cCommandBlockEntity,
		E_BLOCK_COMMAND_BLOCK
	>(a_Position, a_Callback);
}





bool cChunk::DoWithMobHeadAt(Vector3i a_Position, cMobHeadCallback a_Callback)
{
	return GenericDoWithBlockEntityAt<cMobHeadEntity,
		E_BLOCK_HEAD
	>(a_Position, a_Callback);
}





bool cChunk::DoWithFlowerPotAt(Vector3i a_Position, cFlowerPotCallback a_Callback)
{
	return GenericDoWithBlockEntityAt<cFlowerPotEntity,
		E_BLOCK_FLOWER_POT
	>(a_Position, a_Callback);
}





bool cChunk::GetSignLines(Vector3i a_Position, AString & a_Line1, AString & a_Line2, AString & a_Line3, AString & a_Line4)
{
	// The blockentity list is locked by the parent chunkmap's CS
	auto Entity = GetBlockEntity(a_Position);
	if (Entity == nullptr)
	{
		return false;  // Not a block entity
	}
	if (
		(Entity->GetBlockType() != E_BLOCK_WALLSIGN) &&
		(Entity->GetBlockType() != E_BLOCK_SIGN_POST)
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





void cChunk::GetBlockTypeMeta(Vector3i a_RelPos, BLOCKTYPE & a_BlockType, NIBBLETYPE & a_BlockMeta) const
{
	a_BlockType = GetBlock(a_RelPos);
	a_BlockMeta = GetMeta(a_RelPos);
}





void cChunk::GetBlockInfo(Vector3i a_RelPos, BLOCKTYPE & a_BlockType, NIBBLETYPE & a_Meta, NIBBLETYPE & a_SkyLight, NIBBLETYPE & a_BlockLight)
{
	a_BlockType  = GetBlock(a_RelPos);
	a_Meta       = m_ChunkData.GetMeta(a_RelPos);
	a_SkyLight   = m_ChunkData.GetSkyLight(a_RelPos);
	a_BlockLight = m_ChunkData.GetBlockLight(a_RelPos);
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
	// If the relative coords are too far away, use the parent's chunk lookup instead:
	if ((a_RelX < -128) || (a_RelX > 128) || (a_RelZ < -128) || (a_RelZ > 128))
	{
		int BlockX = m_PosX * cChunkDef::Width + a_RelX;
		int BlockZ = m_PosZ * cChunkDef::Width + a_RelZ;
		int ChunkX, ChunkZ;
		BlockToChunk(BlockX, BlockZ, ChunkX, ChunkZ);
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

	// The most common case: inside this chunk:
	if (
		(a_RelPos.x >= 0) && (a_RelPos.x < Width) &&
		(a_RelPos.z >= 0) && (a_RelPos.z < Width)
	)
	{
		return ToReturn;
	}

	// Request for a different chunk, calculate chunk offset:
	int RelX = a_RelPos.x;  // Make a local copy of the coords (faster access)
	int RelZ = a_RelPos.z;
	while ((RelX >= Width) && (ToReturn != nullptr))
	{
		RelX -= Width;
		ToReturn = ToReturn->m_NeighborXP;
	}
	while ((RelX < 0) && (ToReturn != nullptr))
	{
		RelX += Width;
		ToReturn = ToReturn->m_NeighborXM;
	}
	while ((RelZ >= Width) && (ToReturn != nullptr))
	{
		RelZ -= Width;
		ToReturn = ToReturn->m_NeighborZP;
	}
	while ((RelZ < 0) && (ToReturn != nullptr))
	{
		RelZ += Width;
		ToReturn = ToReturn->m_NeighborZM;
	}
	if (ToReturn != nullptr)
	{
		a_RelPos.x = RelX;
		a_RelPos.z = RelZ;
		return ToReturn;
	}

	// The chunk cannot be walked through neighbors, find it through the chunkmap:
	int AbsX = a_RelPos.x + m_PosX * Width;
	int AbsZ = a_RelPos.z + m_PosZ * Width;
	int DstChunkX, DstChunkZ;
	BlockToChunk(AbsX, AbsZ, DstChunkX, DstChunkZ);
	ToReturn = m_ChunkMap->FindChunk(DstChunkX, DstChunkZ);
	a_RelPos.x = AbsX - DstChunkX * Width;
	a_RelPos.z = AbsZ - DstChunkZ * Width;
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
	a_BlockX = m_PosX * Width + a_RelX;
	a_BlockZ = m_PosZ * Width + a_RelZ;
}





Vector3i cChunk::PositionToWorldPosition(int a_RelX, int a_RelY, int a_RelZ)
{
	return Vector3i(m_PosX * Width + a_RelX, a_RelY, m_PosZ * Width + a_RelZ);
}





NIBBLETYPE cChunk::GetTimeAlteredLight(NIBBLETYPE a_Skylight) const
{
	a_Skylight -= m_World->GetSkyDarkness();
	// Because NIBBLETYPE is unsigned, we clamp it to 0 .. 15 by checking for values above 15
	return (a_Skylight < 16)? a_Skylight : 0;
}
