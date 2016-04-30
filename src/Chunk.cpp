
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#ifndef _WIN32
	#include <cstdlib>
#endif


#include "Chunk.h"
#include "World.h"
#include "ClientHandle.h"
#include "Server.h"
#include "zlib/zlib.h"
#include "Defines.h"
#include "BlockEntities/BeaconEntity.h"
#include "BlockEntities/BrewingstandEntity.h"
#include "BlockEntities/ChestEntity.h"
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
// sSetBlock:

sSetBlock::sSetBlock(int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta):
	m_RelX(a_BlockX),
	m_RelY(a_BlockY),
	m_RelZ(a_BlockZ),
	m_BlockType(a_BlockType),
	m_BlockMeta(a_BlockMeta)
{
	cChunkDef::AbsoluteToRelative(m_RelX, m_RelY, m_RelZ, m_ChunkX, m_ChunkZ);
}





////////////////////////////////////////////////////////////////////////////////
// cChunk:

cChunk::cChunk(
	int a_ChunkX, int a_ChunkZ,
	cChunkMap * a_ChunkMap, cWorld * a_World,
	cChunk * a_NeighborXM, cChunk * a_NeighborXP, cChunk * a_NeighborZM, cChunk * a_NeighborZP,
	cAllocationPool<cChunkData::sChunkSection> & a_Pool
) :
	m_Presence(cpInvalid),
	m_ShouldGenerateIfLoadFailed(false),
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
	m_BlockTickX(0),
	m_BlockTickY(0),
	m_BlockTickZ(0),
	m_NeighborXM(a_NeighborXM),
	m_NeighborXP(a_NeighborXP),
	m_NeighborZM(a_NeighborZM),
	m_NeighborZP(a_NeighborZP),
	m_WaterSimulatorData(a_World->GetWaterSimulator()->CreateChunkData()),
	m_LavaSimulatorData (a_World->GetLavaSimulator ()->CreateChunkData()),
	m_RedstoneSimulatorData(a_World->GetRedstoneSimulator()->CreateChunkData()),
	m_AlwaysTicked(0)
{
	if (a_NeighborXM != nullptr)
	{
		a_NeighborXM->m_NeighborXP = this;
	}
	if (a_NeighborXP != nullptr)
	{
		a_NeighborXP->m_NeighborXM = this;
	}
	if (a_NeighborZM != nullptr)
	{
		a_NeighborZM->m_NeighborZP = this;
	}
	if (a_NeighborZP != nullptr)
	{
		a_NeighborZP->m_NeighborZM = this;
	}
}





cChunk::~cChunk()
{
	cPluginManager::Get().CallHookChunkUnloaded(*m_World, m_PosX, m_PosZ);

	// LOGINFO("### delete cChunk() (%i, %i) from %p, thread 0x%x ###", m_PosX, m_PosZ, this, GetCurrentThreadId());

	for (cBlockEntityList::iterator itr = m_BlockEntities.begin(); itr != m_BlockEntities.end(); ++itr)
	{
		delete *itr;
	}
	m_BlockEntities.clear();

	// Remove and destroy all entities:
	for (auto & Entity : m_Entities)
	{
		if (Entity->IsDestroyed())
		{
			// Workaround to mitigate crashing in cPlayer::SaveToDisk which may try to access destroyed member variables on server stop
			// All entities will have been destroyed in cWorld::Stop in this circumstance
			continue;
		}

		// Scheduling a normal destruction is neither possible (Since this chunk will be gone till the schedule occurs) nor necessary.
		Entity->DestroyNoScheduling(false);  // No point in broadcasting in an unloading chunk. Chunks unload when no one is nearby.
	}

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





void cChunk::SetShouldGenerateIfLoadFailed(bool a_ShouldGenerateIfLoadFailed)
{
	m_ShouldGenerateIfLoadFailed = a_ShouldGenerateIfLoadFailed;
}





void cChunk::MarkRegenerating(void)
{
	// Set as queued again:
	SetPresence(cpQueued);

	// Tell all clients attached to this chunk that they want this chunk:
	GetWorld()->GetChunkSender().QueueSendChunkTo(GetPosX(), GetPosZ(), cChunkSender::E_CHUNK_PRIORITY_MEDIUM, GetAllWeakClientPtrs());
}





bool cChunk::CanUnload(void)
{
	return
		m_LoadedByClient.empty() &&  // The chunk is not loaded by any client
		!m_IsDirty &&                // The chunk has been saved properly or hasn't been touched since the load / gen
		(m_StayCount == 0) &&        // The chunk is not in a ChunkStay
		(m_Presence != cpQueued) &&  // The chunk is not queued for loading / generating (otherwise multi-load / multi-gen could occur)
		(std::find_if(m_Entities.cbegin(), m_Entities.cend(), [](const decltype(m_Entities)::value_type & a_Value) { return a_Value->IsPlayer(); }) == m_Entities.cend())  // The chunk does not contain a player
	;
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

	// If the chunk is marked as needed, generate it:
	if (m_ShouldGenerateIfLoadFailed)
	{
		m_World->GetGenerator().QueueGenerateChunk(m_PosX, m_PosZ, false);
	}
	else
	{
		m_Presence = cpInvalid;
	}
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

	for (cBlockEntityList::iterator itr = m_BlockEntities.begin(); itr != m_BlockEntities.end(); ++itr)
	{
		a_Callback.BlockEntity(*itr);
	}
}





void cChunk::SetAllData(cSetChunkData & a_SetChunkData)
{
	ASSERT(a_SetChunkData.IsHeightMapValid());
	ASSERT(a_SetChunkData.AreBiomesValid());
	ASSERT(IsQueued());

	memcpy(m_BiomeMap, a_SetChunkData.GetBiomes(), sizeof(m_BiomeMap));
	memcpy(m_HeightMap, a_SetChunkData.GetHeightMap(), sizeof(m_HeightMap));

	m_ChunkData.SetBlockTypes(a_SetChunkData.GetBlockTypes());
	m_ChunkData.SetMetas(a_SetChunkData.GetBlockMetas());
	if (a_SetChunkData.IsLightValid())
	{
		m_ChunkData.SetBlockLight(a_SetChunkData.GetBlockLight());
		m_ChunkData.SetSkyLight(a_SetChunkData.GetSkyLight());
		m_IsLightValid = true;
	}
	else
	{
		m_IsLightValid = false;
	}

	// Clear the block entities present - either the loader / saver has better, or we'll create empty ones:
	for (cBlockEntityList::iterator itr = m_BlockEntities.begin(); itr != m_BlockEntities.end(); ++itr)
	{
		delete *itr;
	}
	m_BlockEntities.clear();
	std::swap(a_SetChunkData.GetBlockEntities(), m_BlockEntities);

	// Check that all block entities have a valid blocktype at their respective coords (DEBUG-mode only):
	#ifdef _DEBUG
		for (cBlockEntityList::iterator itr = m_BlockEntities.begin(); itr != m_BlockEntities.end(); ++itr)
		{
			BLOCKTYPE EntityBlockType = (*itr)->GetBlockType();
			BLOCKTYPE WorldBlockType = GetBlock((*itr)->GetRelX(), (*itr)->GetPosY(), (*itr)->GetRelZ());
			ASSERT(WorldBlockType == EntityBlockType);
		}  // for itr - m_BlockEntities
	#endif  // _DEBUG

	// Set all block entities' World variable:
	for (cBlockEntityList::iterator itr = m_BlockEntities.begin(); itr != m_BlockEntities.end(); ++itr)
	{
		(*itr)->SetWorld(m_World);
	}

	// Create block entities that the loader didn't load; fill them with defaults
	CreateBlockEntities();

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
		LOGWARNING("cChunk::WriteBlockArea(): unsupported datatype request, can write only types + metas (0x%x), requested 0x%x. Ignoring.",
			(cBlockArea::baTypes | cBlockArea::baMetas), a_DataTypes & (cBlockArea::baTypes | cBlockArea::baMetas)
		);
		return;
	}

	// SizeX, SizeZ are the dimensions of the block data to copy to the chunk (size of the geometric union)

	int BlockStartX = std::max(a_MinBlockX, m_PosX * cChunkDef::Width);
	int BlockEndX   = std::min(a_MinBlockX + a_Area.GetSizeX(), (m_PosX + 1) * cChunkDef::Width);
	int BlockStartZ = std::max(a_MinBlockZ, m_PosZ * cChunkDef::Width);
	int BlockEndZ   = std::min(a_MinBlockZ + a_Area.GetSizeZ(), (m_PosZ + 1) * cChunkDef::Width);
	int SizeX = BlockEndX - BlockStartX;
	int SizeZ = BlockEndZ - BlockStartZ;
	int OffX = BlockStartX - m_PosX * cChunkDef::Width;
	int OffZ = BlockStartZ - m_PosZ * cChunkDef::Width;
	int BaseX = BlockStartX - a_MinBlockX;
	int BaseZ = BlockStartZ - a_MinBlockZ;
	int SizeY = std::min(a_Area.GetSizeY(), cChunkDef::Height - a_MinBlockY);

	// TODO: Improve this by not calling FastSetBlock() and doing the processing here
	// so that the heightmap is touched only once for each column.
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
				int idx = a_Area.MakeIndex(AreaX, AreaY, AreaZ);
				BLOCKTYPE BlockType = AreaBlockTypes[idx];
				NIBBLETYPE BlockMeta = AreaBlockMetas[idx];
				FastSetBlock(ChunkX, ChunkY, ChunkZ, BlockType, BlockMeta);
			}  // for x
		}  // for z
	}  // for y
}





bool cChunk::HasBlockEntityAt(int a_BlockX, int a_BlockY, int a_BlockZ)
{
	for (cBlockEntityList::iterator itr = m_BlockEntities.begin(); itr != m_BlockEntities.end(); ++itr)
	{
		if (
			((*itr)->GetPosX() == a_BlockX) &&
			((*itr)->GetPosY() == a_BlockY) &&
			((*itr)->GetPosZ() == a_BlockZ)
		)
		{
			return true;
		}
	}  // for itr - m_BlockEntities[]
	return false;
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
	for (const auto & ClientHandle : GetAllStrongClientPtrs())
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
			auto & Monster = reinterpret_cast<cMonster &>(*entity);
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
	ASSERT(a_MaxX * a_MaxY * a_MaxZ * 8 < 0x00ffffff);
	int Random = m_World->GetTickRandomNumber(0x00ffffff);
	a_X =   Random % (a_MaxX * 2);
	a_Y =  (Random / (a_MaxX * 2)) % (a_MaxY * 2);
	a_Z = ((Random / (a_MaxX * 2)) / (a_MaxY * 2)) % (a_MaxZ * 2);
	a_X /= 2;
	a_Y /= 2;
	a_Z /= 2;
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

		EMCSBiome Biome = m_ChunkMap->GetBiomeAt(TryX, TryZ);
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

		auto newMob = a_MobSpawner.TryToSpawnHere(this, TryX, TryY, TryZ, Biome, MaxNbOfSuccess);
		if (newMob == nullptr)
		{
			continue;
		}
		int WorldX, WorldY, WorldZ;
		PositionToWorldPosition(TryX, TryY, TryZ, WorldX, WorldY, WorldZ);
		double ActualX = WorldX + 0.5;
		double ActualZ = WorldZ + 0.5;
		newMob->SetPosition(ActualX, WorldY, ActualZ);
		LOGD("Spawning %s #%i at {%d, %d, %d}", newMob->GetClass(), newMob->GetUniqueID(), WorldX, WorldY, WorldZ);
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

	BroadcastPendingBlockChanges();

	CheckBlocks();

	// Tick simulators:
	m_World->GetSimulatorManager()->SimulateChunk(a_Dt, m_PosX, m_PosZ, this);

	TickBlocks();

	// Tick all block entities in this chunk:
	for (cBlockEntityList::iterator itr = m_BlockEntities.begin(); itr != m_BlockEntities.end(); ++itr)
	{
		m_IsDirty = (*itr)->Tick(a_Dt, *this) | m_IsDirty;
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
			// The entity moved out of the chunk, move it to the neighbor

			(*itr)->SetParentChunk(nullptr);
			MoveEntityToNewChunk(*itr);

			// Mark as dirty if it was a server-generated entity:
			if (!(*itr)->IsPlayer())
			{
				MarkDirty();
			}
			itr = m_Entities.erase(itr);
		}
		else
		{
			++itr;
		}
	}  // for itr - m_Entitites[]

	ApplyWeatherToTop();
}





void cChunk::TickBlock(int a_RelX, int a_RelY, int a_RelZ)
{
	cBlockHandler * Handler = BlockHandler(GetBlock(a_RelX, a_RelY, a_RelZ));
	ASSERT(Handler != nullptr);  // Happenned on server restart, FS #243
	cChunkInterface ChunkInterface(this->GetWorld()->GetChunkMap());
	cBlockInServerPluginInterface PluginInterface(*this->GetWorld());
	Handler->OnUpdate(ChunkInterface, *this->GetWorld(), PluginInterface, *this, a_RelX, a_RelY, a_RelZ);
}





void cChunk::MoveEntityToNewChunk(std::shared_ptr<cEntity> & a_Entity)
{
	cChunk * Neighbor = GetNeighborChunk(a_Entity->GetChunkX() * cChunkDef::Width, a_Entity->GetChunkZ() * cChunkDef::Width);
	if (Neighbor == nullptr)
	{
		Neighbor = m_ChunkMap->GetChunk(a_Entity->GetChunkX(), a_Entity->GetChunkZ());
		if (Neighbor == nullptr)  // This will assert inside GetChunk in debug builds
		{
			LOGWARNING("%s: Failed to move entity, destination chunk unreachable. Entity lost", __FUNCTION__);
			return;
		}
	}

	ASSERT(Neighbor != this);  // Moving into the same chunk? wtf?
	Neighbor->AddEntity(a_Entity);

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
	} Mover(*a_Entity);

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
		m_World->GetChunkSender().QueueSendChunkTo(m_PosX, m_PosZ, cChunkSender::E_CHUNK_PRIORITY_MEDIUM, m_LoadedByClient);
	}
	else
	{
		// Only send block changes
		for (const auto & ClientHandle : GetAllStrongClientPtrs())
		{
			ClientHandle->SendBlockChanges(m_PosX, m_PosZ, m_PendingSendBlocks);
		}
	}
	m_PendingSendBlocks.clear();
}





void cChunk::CheckBlocks()
{
	if (m_ToTickBlocks.empty())
	{
		return;
	}
	std::vector<Vector3i> ToTickBlocks;
	std::swap(m_ToTickBlocks, ToTickBlocks);

	cChunkInterface ChunkInterface(m_World->GetChunkMap());
	cBlockInServerPluginInterface PluginInterface(*m_World);

	for (std::vector<Vector3i>::const_iterator itr = ToTickBlocks.begin(), end = ToTickBlocks.end(); itr != end; ++itr)
	{
		Vector3i Pos = (*itr);

		cBlockHandler * Handler = BlockHandler(GetBlock(Pos));
		Handler->Check(ChunkInterface, PluginInterface, Pos.x, Pos.y, Pos.z, *this);
	}  // for itr - ToTickBlocks[]
}





void cChunk::TickBlocks(void)
{
	// Tick dem blocks
	// _X: We must limit the random number or else we get a nasty int overflow bug - https://forum.cuberite.org/thread-457.html
	int RandomX = m_World->GetTickRandomNumber(0x00ffffff);
	int RandomY = m_World->GetTickRandomNumber(0x00ffffff);
	int RandomZ = m_World->GetTickRandomNumber(0x00ffffff);
	int TickX = m_BlockTickX;
	int TickY = m_BlockTickY;
	int TickZ = m_BlockTickZ;

	cChunkInterface ChunkInterface(this->GetWorld()->GetChunkMap());
	cBlockInServerPluginInterface PluginInterface(*this->GetWorld());

	// This for loop looks disgusting, but it actually does a simple thing - first processes m_BlockTick, then adds random to it
	// This is so that SetNextBlockTick() works
	for (int i = 0; i < 50; i++,

		// This weird construct (*2, then /2) is needed,
		// otherwise the blocktick distribution is too biased towards even coords!

		TickX = (TickX + RandomX) % (Width * 2),
		TickY = (TickY + RandomY) % (Height * 2),
		TickZ = (TickZ + RandomZ) % (Width * 2),
		m_BlockTickX = TickX / 2,
		m_BlockTickY = TickY / 2,
		m_BlockTickZ = TickZ / 2
	)
	{

		if (m_BlockTickY > cChunkDef::GetHeight(m_HeightMap, m_BlockTickX, m_BlockTickZ))
		{
			continue;  // It's all air up here
		}

		cBlockHandler * Handler = BlockHandler(GetBlock(m_BlockTickX, m_BlockTickY, m_BlockTickZ));
		ASSERT(Handler != nullptr);  // Happenned on server restart, FS #243
		Handler->OnUpdate(ChunkInterface, *this->GetWorld(), PluginInterface, *this, m_BlockTickX, m_BlockTickY, m_BlockTickZ);
	}  // for i - tickblocks
}





void cChunk::ApplyWeatherToTop()
{
	if (
		(m_World->GetTickRandomNumber(100) != 0) ||
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

	// TODO: Check light levels, don't snow over when the BlockLight is higher than (7?)
	int Height = GetHeight(X, Z);

	if (GetSnowStartHeight(GetBiomeAt(X, Z)) > Height)
	{
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
		SetBlock(X, Height + 1, Z, E_BLOCK_SNOW, 0);
	}
	else if (IsBlockWater(TopBlock) && (TopMeta == 0))
	{
		SetBlock(X, Height, Z, E_BLOCK_ICE, 0);
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
		SetBlock(X, Height, Z, E_BLOCK_SNOW, 0);
	}
}





void cChunk::GrowMelonPumpkin(int a_RelX, int a_RelY, int a_RelZ, BLOCKTYPE a_BlockType, MTRand & a_TickRandom)
{
	// Convert the stem BlockType into produce BlockType
	BLOCKTYPE ProduceType;
	switch (a_BlockType)
	{
		case E_BLOCK_MELON_STEM:   ProduceType = E_BLOCK_MELON;   break;
		case E_BLOCK_PUMPKIN_STEM: ProduceType = E_BLOCK_PUMPKIN; break;
		default:
		{
			ASSERT(!"Unhandled blocktype in TickMelonPumpkin()");
			return;
		}
	}

	// Check if there's another melon / pumpkin around that stem, if so, abort:
	bool IsValid;
	BLOCKTYPE BlockType[4];
	NIBBLETYPE BlockMeta;  // unused
	IsValid =            UnboundedRelGetBlock(a_RelX + 1, a_RelY, a_RelZ,     BlockType[0], BlockMeta);
	IsValid = IsValid && UnboundedRelGetBlock(a_RelX - 1, a_RelY, a_RelZ,     BlockType[1], BlockMeta);
	IsValid = IsValid && UnboundedRelGetBlock(a_RelX,     a_RelY, a_RelZ + 1, BlockType[2], BlockMeta);
	IsValid = IsValid && UnboundedRelGetBlock(a_RelX,     a_RelY, a_RelZ - 1, BlockType[3], BlockMeta);
	if (
		!IsValid ||
		(BlockType[0] == ProduceType) ||
		(BlockType[1] == ProduceType) ||
		(BlockType[2] == ProduceType) ||
		(BlockType[3] == ProduceType)
	)
	{
		// Neighbors not valid or already taken by the same produce
		return;
	}

	// Pick a direction in which to place the produce:
	int x = 0, z = 0;
	int CheckType = a_TickRandom.randInt(3);  // The index to the neighbors array which should be checked for emptiness
	switch (CheckType)
	{
		case 0: x =  1; break;
		case 1: x = -1; break;
		case 2: z =  1; break;
		case 3: z = -1; break;
	}

	// Check that the block in that direction is empty:
	switch (BlockType[CheckType])
	{
		case E_BLOCK_AIR:
		case E_BLOCK_SNOW:
		case E_BLOCK_TALL_GRASS:
		case E_BLOCK_DEAD_BUSH:
		{
			break;
		}
		default: return;
	}

	// Check if there's soil under the neighbor. We already know the neighbors are valid. Place produce if ok
	BLOCKTYPE Soil;
	VERIFY(UnboundedRelGetBlock(a_RelX + x, a_RelY - 1, a_RelZ + z, Soil, BlockMeta));
	switch (Soil)
	{
		case E_BLOCK_DIRT:
		case E_BLOCK_GRASS:
		case E_BLOCK_FARMLAND:
		{
			// Place a randomly-facing produce:
			NIBBLETYPE Meta = (ProduceType == E_BLOCK_MELON) ? 0 : static_cast<NIBBLETYPE>(a_TickRandom.randInt(4) % 4);
			LOGD("Growing melon / pumpkin at {%d, %d, %d} (<%d, %d> from stem), overwriting %s, growing on top of %s, meta %d",
				a_RelX + x + m_PosX * cChunkDef::Width, a_RelY, a_RelZ + z + m_PosZ * cChunkDef::Width,
				x, z,
				ItemTypeToString(BlockType[CheckType]).c_str(),
				ItemTypeToString(Soil).c_str(),
				Meta
			);
			VERIFY(UnboundedRelFastSetBlock(a_RelX + x, a_RelY, a_RelZ + z, ProduceType, Meta));
			auto Absolute = RelativeToAbsolute(Vector3i{a_RelX + x, a_RelY, a_RelZ + z}, m_PosX, m_PosZ);
			cChunkInterface ChunkInterface(this->GetWorld()->GetChunkMap());
			cBlockHandler::NeighborChanged(ChunkInterface, Absolute.x, Absolute.y - 1, Absolute.z, BLOCK_FACE_YP);
			break;
		}
	}
}





void cChunk::GrowSugarcane(int a_RelX, int a_RelY, int a_RelZ, int a_NumBlocks)
{
	// Check the total height of the sugarcane blocks here:
	int Top = a_RelY + 1;
	while (
		(Top < cChunkDef::Height) &&
		(GetBlock(a_RelX, Top, a_RelZ) == E_BLOCK_SUGARCANE)
	)
	{
		++Top;
	}
	int Bottom = a_RelY - 1;
	while (
		(Bottom > 0) &&
		(GetBlock(a_RelX, Bottom, a_RelZ) == E_BLOCK_SUGARCANE)
	)
	{
		--Bottom;
	}

	// Grow by at most a_NumBlocks, but no more than max height:
	int ToGrow = std::min(a_NumBlocks, m_World->GetMaxSugarcaneHeight() + 1 - (Top - Bottom));
	for (int i = 0; i < ToGrow; i++)
	{
		BLOCKTYPE  BlockType;
		NIBBLETYPE BlockMeta;
		if (UnboundedRelGetBlock(a_RelX, Top + i, a_RelZ, BlockType, BlockMeta) && (BlockType == E_BLOCK_AIR))
		{
			UnboundedRelFastSetBlock(a_RelX, Top + i, a_RelZ, E_BLOCK_SUGARCANE, 0);
		}
		else
		{
			break;
		}
	}  // for i
}





void cChunk::GrowCactus(int a_RelX, int a_RelY, int a_RelZ, int a_NumBlocks)
{
	// Check the total height of the sugarcane blocks here:
	int Top = a_RelY + 1;
	while (
		(Top < cChunkDef::Height) &&
		(GetBlock(a_RelX, Top, a_RelZ) == E_BLOCK_CACTUS)
	)
	{
		++Top;
	}
	int Bottom = a_RelY - 1;
	while (
		(Bottom > 0) &&
		(GetBlock(a_RelX, Bottom, a_RelZ) == E_BLOCK_CACTUS)
	)
	{
		--Bottom;
	}

	// Grow by at most a_NumBlocks, but no more than max height:
	int ToGrow = std::min(a_NumBlocks, m_World->GetMaxCactusHeight() + 1 - (Top - Bottom));
	for (int i = 0; i < ToGrow; i++)
	{
		BLOCKTYPE  BlockType;
		NIBBLETYPE BlockMeta;
		if (UnboundedRelGetBlock(a_RelX, Top + i, a_RelZ, BlockType, BlockMeta) && (BlockType == E_BLOCK_AIR))
		{
			// TODO: Check the surrounding blocks, if they aren't air, break the cactus block into pickups (and continue breaking blocks above in the next loop iterations)
			UnboundedRelFastSetBlock(a_RelX, Top + i, a_RelZ, E_BLOCK_CACTUS, 0);
		}
		else
		{
			break;
		}
	}  // for i
}





bool cChunk::UnboundedRelGetBlock(int a_RelX, int a_RelY, int a_RelZ, BLOCKTYPE & a_BlockType, NIBBLETYPE & a_BlockMeta) const
{
	if (!cChunkDef::IsValidHeight(a_RelY))
	{
		LOGWARNING("%s: requesting a block with a_RelY out of range: %d", __FUNCTION__, a_RelY);
		return false;
	}
	cChunk * Chunk = GetRelNeighborChunkAdjustCoords(a_RelX, a_RelZ);
	if ((Chunk == nullptr) || !Chunk->IsValid())
	{
		// The chunk is not available, bail out
		return false;
	}
	Chunk->GetBlockTypeMeta(a_RelX, a_RelY, a_RelZ, a_BlockType, a_BlockMeta);
	return true;
}





bool cChunk::UnboundedRelGetBlockType(int a_RelX, int a_RelY, int a_RelZ, BLOCKTYPE & a_BlockType) const
{
	if (!cChunkDef::IsValidHeight(a_RelY))
	{
		LOGWARNING("%s: requesting a block with a_RelY out of range: %d", __FUNCTION__, a_RelY);
		return false;
	}
	cChunk * Chunk = GetRelNeighborChunkAdjustCoords(a_RelX, a_RelZ);
	if ((Chunk == nullptr) || !Chunk->IsValid())
	{
		// The chunk is not available, bail out
		return false;
	}
	a_BlockType = Chunk->GetBlock(a_RelX, a_RelY, a_RelZ);
	return true;
}





bool cChunk::UnboundedRelGetBlockMeta(int a_RelX, int a_RelY, int a_RelZ, NIBBLETYPE & a_BlockMeta) const
{
	if (!cChunkDef::IsValidHeight(a_RelY))
	{
		LOGWARNING("%s: requesting a block with a_RelY out of range: %d", __FUNCTION__, a_RelY);
		return false;
	}
	cChunk * Chunk = GetRelNeighborChunkAdjustCoords(a_RelX, a_RelZ);
	if ((Chunk == nullptr) || !Chunk->IsValid())
	{
		// The chunk is not available, bail out
		return false;
	}
	a_BlockMeta = Chunk->GetMeta(a_RelX, a_RelY, a_RelZ);
	return true;
}





bool cChunk::UnboundedRelGetBlockBlockLight(int a_RelX, int a_RelY, int a_RelZ, NIBBLETYPE & a_BlockBlockLight) const
{
	if (!cChunkDef::IsValidHeight(a_RelY))
	{
		LOGWARNING("%s: requesting a block with a_RelY out of range: %d", __FUNCTION__, a_RelY);
		return false;
	}
	cChunk * Chunk = GetRelNeighborChunkAdjustCoords(a_RelX, a_RelZ);
	if ((Chunk == nullptr) || !Chunk->IsValid())
	{
		// The chunk is not available, bail out
		return false;
	}
	a_BlockBlockLight = Chunk->GetBlockLight(a_RelX, a_RelY, a_RelZ);
	return true;
}





bool cChunk::UnboundedRelGetBlockSkyLight(int a_RelX, int a_RelY, int a_RelZ, NIBBLETYPE & a_BlockSkyLight) const
{
	if (!cChunkDef::IsValidHeight(a_RelY))
	{
		LOGWARNING("%s: requesting a block with a_RelY out of range: %d", __FUNCTION__, a_RelY);
		return false;
	}
	cChunk * Chunk = GetRelNeighborChunkAdjustCoords(a_RelX, a_RelZ);
	if ((Chunk == nullptr) || !Chunk->IsValid())
	{
		// The chunk is not available, bail out
		return false;
	}
	a_BlockSkyLight = Chunk->GetSkyLight(a_RelX, a_RelY, a_RelZ);
	return true;
}





bool cChunk::UnboundedRelGetBlockLights(int a_RelX, int a_RelY, int a_RelZ, NIBBLETYPE & a_BlockLight, NIBBLETYPE & a_SkyLight) const
{
	if (!cChunkDef::IsValidHeight(a_RelY))
	{
		LOGWARNING("%s: requesting a block with a_RelY out of range: %d", __FUNCTION__, a_RelY);
		return false;
	}
	cChunk * Chunk = GetRelNeighborChunkAdjustCoords(a_RelX, a_RelZ);
	if ((Chunk == nullptr) || !Chunk->IsValid())
	{
		// The chunk is not available, bail out
		return false;
	}
	a_BlockLight = Chunk->GetBlockLight(a_RelX, a_RelY, a_RelZ);
	a_SkyLight = Chunk->GetSkyLight(a_RelX, a_RelY, a_RelZ);
	return true;
}





bool cChunk::UnboundedRelSetBlock(int a_RelX, int a_RelY, int a_RelZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta)
{
	if (!cChunkDef::IsValidHeight(a_RelY))
	{
		LOGWARNING("UnboundedRelSetBlock(): requesting a block with a_RelY out of range: %d", a_RelY);
		return false;
	}
	cChunk * Chunk = GetRelNeighborChunkAdjustCoords(a_RelX, a_RelZ);
	if ((Chunk == nullptr) || !Chunk->IsValid())
	{
		// The chunk is not available, bail out
		return false;
	}
	Chunk->SetBlock(a_RelX, a_RelY, a_RelZ, a_BlockType, a_BlockMeta);
	return true;
}





bool cChunk::UnboundedRelFastSetBlock(int a_RelX, int a_RelY, int a_RelZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta)
{
	if (!cChunkDef::IsValidHeight(a_RelY))
	{
		LOGWARNING("UnboundedRelFastSetBlock(): requesting a block with a_RelY out of range: %d", a_RelY);
		return false;
	}
	cChunk * Chunk = GetRelNeighborChunkAdjustCoords(a_RelX, a_RelZ);
	if ((Chunk == nullptr) || !Chunk->IsValid())
	{
		// The chunk is not available, bail out
		return false;
	}
	Chunk->FastSetBlock(a_RelX, a_RelY, a_RelZ, a_BlockType, a_BlockMeta);
	return true;
}





void cChunk::UnboundedQueueTickBlock(int a_RelX, int a_RelY, int a_RelZ)
{
	if (!cChunkDef::IsValidHeight(a_RelY))
	{
		// Outside of chunkmap
		return;
	}
	cChunk * Chunk = GetRelNeighborChunkAdjustCoords(a_RelX, a_RelZ);
	if ((Chunk != nullptr) && Chunk->IsValid())
	{
		Chunk->QueueTickBlock(a_RelX, a_RelY, a_RelZ);
	}
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





void cChunk::CreateBlockEntities(void)
{
	for (int x = 0; x < Width; x++)
	{
		for (int z = 0; z < Width; z++)
		{
			for (int y = 0; y < Height; y++)
			{
				BLOCKTYPE BlockType = GetBlock(x, y, z);
				switch (BlockType)
				{
					case E_BLOCK_BEACON:
					case E_BLOCK_TRAPPED_CHEST:
					case E_BLOCK_CHEST:
					case E_BLOCK_COMMAND_BLOCK:
					case E_BLOCK_DISPENSER:
					case E_BLOCK_DROPPER:
					case E_BLOCK_ENDER_CHEST:
					case E_BLOCK_LIT_FURNACE:
					case E_BLOCK_FURNACE:
					case E_BLOCK_HOPPER:
					case E_BLOCK_SIGN_POST:
					case E_BLOCK_WALLSIGN:
					case E_BLOCK_HEAD:
					case E_BLOCK_NOTE_BLOCK:
					case E_BLOCK_JUKEBOX:
					case E_BLOCK_FLOWER_POT:
					case E_BLOCK_MOB_SPAWNER:
					case E_BLOCK_BREWING_STAND:
					{
						if (!HasBlockEntityAt(x + m_PosX * Width, y, z + m_PosZ * Width))
						{
							m_BlockEntities.push_back(cBlockEntity::CreateByBlockType(
								BlockType, GetMeta(x, y, z),
								x + m_PosX * Width, y, z + m_PosZ * Width, m_World
							));
						}
						break;
					}
				}  // switch (BlockType)
			}  // for y
		}  // for z
	}  // for x
}





void cChunk::WakeUpSimulators(void)
{
	cSimulator * WaterSimulator = m_World->GetWaterSimulator();
	cSimulator * LavaSimulator  = m_World->GetLavaSimulator();
	cSimulator * RedstoneSimulator = m_World->GetRedstoneSimulator();
	int BaseX = m_PosX * cChunkDef::Width;
	int BaseZ = m_PosZ * cChunkDef::Width;
	for (int x = 0; x < Width; x++)
	{
		int BlockX = x + BaseX;
		for (int z = 0; z < Width; z++)
		{
			int BlockZ = z + BaseZ;
			for (int y = GetHeight(x, z); y >= 0; y--)
			{
				BLOCKTYPE Block = GetBlock(x, y, z);

				// The redstone sim takes multiple blocks, use the inbuilt checker
				if (RedstoneSimulator->IsAllowedBlock(Block))
				{
					RedstoneSimulator->AddBlock(BlockX, y, BlockZ, this);
					continue;
				}

				switch (Block)
				{
					case E_BLOCK_WATER:
					{
						WaterSimulator->AddBlock(BlockX, y, BlockZ, this);
						break;
					}
					case E_BLOCK_LAVA:
					{
						LavaSimulator->AddBlock(BlockX, y, BlockZ, this);
						break;
					}
					default:
					{
						break;
					}
				}  // switch (BlockType)
			}  // for y
		}  // for z
	}  // for x
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





void cChunk::SetBlock(int a_RelX, int a_RelY, int a_RelZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, bool a_SendToClients)
{
	FastSetBlock(a_RelX, a_RelY, a_RelZ, a_BlockType, a_BlockMeta, a_SendToClients);

	// Tick this block and its neighbors:
	m_ToTickBlocks.push_back(Vector3i(a_RelX, a_RelY, a_RelZ));
	QueueTickBlockNeighbors(a_RelX, a_RelY, a_RelZ);

	// If there was a block entity, remove it:
	Vector3i WorldPos = PositionToWorldPosition(a_RelX, a_RelY, a_RelZ);
	cBlockEntity * BlockEntity = GetBlockEntity(WorldPos);
	if (BlockEntity != nullptr)
	{
		BlockEntity->Destroy();
		RemoveBlockEntity(BlockEntity);
		delete BlockEntity;
		BlockEntity = nullptr;
	}

	// If the new block is a block entity, create the entity object:
	switch (a_BlockType)
	{
		case E_BLOCK_BEACON:
		case E_BLOCK_TRAPPED_CHEST:
		case E_BLOCK_CHEST:
		case E_BLOCK_COMMAND_BLOCK:
		case E_BLOCK_DISPENSER:
		case E_BLOCK_DROPPER:
		case E_BLOCK_ENDER_CHEST:
		case E_BLOCK_LIT_FURNACE:
		case E_BLOCK_FURNACE:
		case E_BLOCK_HOPPER:
		case E_BLOCK_SIGN_POST:
		case E_BLOCK_WALLSIGN:
		case E_BLOCK_HEAD:
		case E_BLOCK_NOTE_BLOCK:
		case E_BLOCK_JUKEBOX:
		case E_BLOCK_FLOWER_POT:
		case E_BLOCK_MOB_SPAWNER:
		case E_BLOCK_BREWING_STAND:
		{
			AddBlockEntity(cBlockEntity::CreateByBlockType(a_BlockType, a_BlockMeta, WorldPos.x, WorldPos.y, WorldPos.z, m_World));
			break;
		}
	}  // switch (a_BlockType)
}





void cChunk::QueueTickBlock(int a_RelX, int a_RelY, int a_RelZ)
{
	ASSERT (
		(a_RelX >= 0) && (a_RelX < Width) &&
		(a_RelY >= 0) && (a_RelY < Height) &&
		(a_RelZ >= 0) && (a_RelZ < Width)
	);  // Coords need to be valid

	if (!IsValid())
	{
		return;
	}

	m_ToTickBlocks.push_back(Vector3i(a_RelX, a_RelY, a_RelZ));
}





void cChunk::QueueTickBlockNeighbors(int a_RelX, int a_RelY, int a_RelZ)
{
	struct
	{
		int x, y, z;
	}
	Coords[] =
	{
		{ 1,  0,  0},
		{-1,  0,  0},
		{ 0,  1,  0},
		{ 0, -1,  0},
		{ 0,  0,  1},
		{ 0,  0, -1},
	} ;
	for (size_t i = 0; i < ARRAYCOUNT(Coords); i++)
	{
		UnboundedQueueTickBlock(a_RelX + Coords[i].x, a_RelY + Coords[i].y, a_RelZ + Coords[i].z);
	}  // for i - Coords[]
}





void cChunk::FastSetBlock(int a_RelX, int a_RelY, int a_RelZ, BLOCKTYPE a_BlockType, BLOCKTYPE a_BlockMeta, bool a_SendToClients)
{
	ASSERT(!((a_RelX < 0) || (a_RelX >= Width) || (a_RelY < 0) || (a_RelY >= Height) || (a_RelZ < 0) || (a_RelZ >= Width)));

	ASSERT(IsValid());

	const BLOCKTYPE OldBlockType = GetBlock(a_RelX, a_RelY, a_RelZ);
	const BLOCKTYPE OldBlockMeta = m_ChunkData.GetMeta(a_RelX, a_RelY, a_RelZ);
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

	m_ChunkData.SetBlock(a_RelX, a_RelY, a_RelZ, a_BlockType);

	// Queue block to be sent only if ...
	if (
		a_SendToClients &&                  // ... we are told to do so AND ...
		(
			!(                                // ... the old and new blocktypes AREN'T leaves (because the client doesn't need meta updates)
				((OldBlockType == E_BLOCK_LEAVES) && (a_BlockType == E_BLOCK_LEAVES)) ||
				((OldBlockType == E_BLOCK_NEW_LEAVES) && (a_BlockType == E_BLOCK_NEW_LEAVES))
			) &&                              // ... AND ...
			(
				(OldBlockMeta != a_BlockMeta) || (!ReplacingLiquids)
			)
		)
	)
	{
		m_PendingSendBlocks.push_back(sSetBlock(m_PosX, m_PosZ, a_RelX, a_RelY, a_RelZ, a_BlockType, a_BlockMeta));
	}

	m_ChunkData.SetMeta(a_RelX, a_RelY, a_RelZ, a_BlockMeta);

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
	for (cBlockEntityList::iterator itr = m_BlockEntities.begin(), end = m_BlockEntities.end(); itr != end; ++itr)
	{
		if (((*itr)->GetPosX() == wp.x) && ((*itr)->GetPosY() == wp.y) && ((*itr)->GetPosZ() == wp.z))
		{
			(*itr)->SendTo(*a_Client);
		}
	}  // for itr - m_BlockEntities
}





void cChunk::AddBlockEntity(cBlockEntity * a_BlockEntity)
{
	MarkDirty();
	m_BlockEntities.push_back(a_BlockEntity);
}





cBlockEntity * cChunk::GetBlockEntity(int a_BlockX, int a_BlockY, int a_BlockZ)
{
	// Check that the query coords are within chunk bounds:
	ASSERT(a_BlockX >= m_PosX * cChunkDef::Width);
	ASSERT(a_BlockX < m_PosX * cChunkDef::Width + cChunkDef::Width);
	ASSERT(a_BlockZ >= m_PosZ * cChunkDef::Width);
	ASSERT(a_BlockZ < m_PosZ * cChunkDef::Width + cChunkDef::Width);

	for (cBlockEntityList::iterator itr = m_BlockEntities.begin(); itr != m_BlockEntities.end(); ++itr)
	{
		if (
			((*itr)->GetPosX() == a_BlockX) &&
			((*itr)->GetPosY() == a_BlockY) &&
			((*itr)->GetPosZ() == a_BlockZ)
		)
		{
			return *itr;
		}
	}  // for itr - m_BlockEntities[]

	return nullptr;
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
	}
	else
	{
		m_AlwaysTicked -= 1;
	}
}





std::vector<std::shared_ptr<cClientHandle>> cChunk::GetAllStrongClientPtrs(void)
{
	std::vector<std::shared_ptr<cClientHandle>> Clients;
	m_LoadedByClient.erase(
		std::remove_if(
			m_LoadedByClient.begin(),
			m_LoadedByClient.end(),
			[&Clients](const decltype(m_LoadedByClient)::value_type & a_Value)
			{
				auto ClientHandle = a_Value.lock();
				if (ClientHandle == nullptr)
				{
					return true;
				}

				Clients.emplace_back(ClientHandle);
				return false;
			}
		),
		m_LoadedByClient.end()
	);

	return Clients;
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
	m_World->GetChunkSender().QueueSendChunkTo(m_PosX, m_PosZ, cChunkSender::E_CHUNK_PRIORITY_MEDIUM, m_LoadedByClient);
}





void cChunk::CollectPickupsByPlayer(cPlayer & a_Player)
{
	double PosX = a_Player.GetPosX();
	double PosY = a_Player.GetPosY();
	double PosZ = a_Player.GetPosZ();

	for (auto & Entity : m_Entities)
	{
		if ((!Entity->IsPickup()) && (!Entity->IsProjectile()))
		{
			continue;  // Only pickups and projectiles can be picked up
		}
		float DiffX = static_cast<float>(Entity->GetPosX() - PosX);
		float DiffY = static_cast<float>(Entity->GetPosY() - PosY);
		float DiffZ = static_cast<float>(Entity->GetPosZ() - PosZ);
		float SqrDist = DiffX * DiffX + DiffY * DiffY + DiffZ * DiffZ;
		if (SqrDist < 1.5f * 1.5f)  // 1.5 block
		{
			/*
			LOG("Pickup %d being collected by player \"%s\", distance %f",
				(*itr)->GetUniqueID(), a_Player->GetName().c_str(), SqrDist
			);
			*/
			MarkDirty();
			if (Entity->IsPickup())
			{
				reinterpret_cast<cPickup *>(Entity.get())->CollectedBy(a_Player);
			}
			else
			{
				reinterpret_cast<cProjectileEntity *>(Entity.get())->CollectedBy(a_Player);
			}
		}
		else if (SqrDist < 5 * 5)
		{
			/*
			LOG("Pickup %d close to player \"%s\", but still too far to collect: %f",
				(*itr)->GetUniqueID(), a_Player->GetName().c_str(), SqrDist
			);
			*/
		}
	}
}





bool cChunk::SetSignLines(int a_PosX, int a_PosY, int a_PosZ, const AString & a_Line1, const AString & a_Line2, const AString & a_Line3, const AString & a_Line4)
{
	// Also sends update packets to all clients in the chunk
	for (cBlockEntityList::iterator itr = m_BlockEntities.begin(); itr != m_BlockEntities.end(); ++itr)
	{
		if (
			((*itr)->GetPosX() == a_PosX) &&
			((*itr)->GetPosY() == a_PosY) &&
			((*itr)->GetPosZ() == a_PosZ) &&
			(
				((*itr)->GetBlockType() == E_BLOCK_WALLSIGN) ||
				((*itr)->GetBlockType() == E_BLOCK_SIGN_POST)
			)
		)
		{
			MarkDirty();
			reinterpret_cast<cSignEntity *>(*itr)->SetLines(a_Line1, a_Line2, a_Line3, a_Line4);
			m_World->BroadcastBlockEntity(a_PosX, a_PosY, a_PosZ);
			return true;
		}
	}  // for itr - m_BlockEntities[]
	return false;
}





void cChunk::RemoveBlockEntity(cBlockEntity * a_BlockEntity)
{
	MarkDirty();
	m_BlockEntities.remove(a_BlockEntity);
}





bool cChunk::AddClient(const std::shared_ptr<cClientHandle> & a_Client)
{
	if (
		std::find_if(
			m_LoadedByClient.cbegin(),
			m_LoadedByClient.cend(),
			std::bind(std::equal_to<decltype(m_LoadedByClient)::value_type>(), a_Client, std::placeholders::_1)  // Alternatively, std::bind1st, but you know, it's deprecated
		) != m_LoadedByClient.end()
	)
	{
		// Already there, nothing needed
		return false;
	}

	m_LoadedByClient.emplace_back(a_Client);

	for (const auto & Entity : m_Entities)
	{
		/*
		// DEBUG:
		LOGD("cChunk: Entity #%d (%s) at [%i, %i, %i] spawning for player \"%s\"",
			(*itr)->GetUniqueID(), (*itr)->GetClass(),
			m_PosX, m_PosY, m_PosZ,
			a_Client->GetUsername().c_str()
		);
		*/
		Entity->SpawnOn(*a_Client);
	}

	for (const auto & BlockEntity : m_BlockEntities)
	{
		BlockEntity->SendTo(*a_Client);
	}
	return true;
}





void cChunk::RemoveClient(const std::shared_ptr<cClientHandle> & a_Client)
{
	auto itr = std::remove_if(
		m_LoadedByClient.begin(),
		m_LoadedByClient.end(),
		std::bind(std::equal_to<decltype(m_LoadedByClient)::value_type>(), a_Client, std::placeholders::_1)
	);

	// We should always remove at most one client.
	ASSERT(std::distance(itr, m_LoadedByClient.end()) <= 1);

	// Note: itr can equal m_LoadedByClient.end()
	m_LoadedByClient.erase(itr, m_LoadedByClient.end());

	for (const auto & Entity : m_Entities)
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





bool cChunk::HasAnyClients(void) const
{
	return !m_LoadedByClient.empty();
}





void cChunk::AddEntity(const std::shared_ptr<cEntity> & a_Entity)
{
	if (!a_Entity->IsPlayer())
	{
		MarkDirty();
	}

	ASSERT(std::find(m_Entities.begin(), m_Entities.end(), a_Entity) == m_Entities.end());  // Not there already
	m_Entities.emplace_back(a_Entity);

	ASSERT(a_Entity->GetParentChunk() == nullptr);
	a_Entity->SetParentChunk(this);
	a_Entity->SetIsTicking(true);
}





void cChunk::RemoveEntity(const std::shared_ptr<cEntity> & a_Entity)
{
	ASSERT(a_Entity->GetParentChunk() == this);
	ASSERT(!a_Entity->IsTicking());
	a_Entity->SetParentChunk(nullptr);

	m_Entities.erase(
		std::remove(
			m_Entities.begin(),
			m_Entities.end(),
			a_Entity
		),
		m_Entities.end()
	);

	MarkDirty();
}





bool cChunk::HasEntity(UInt32 a_EntityID)
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





bool cChunk::ForEachEntity(cEntityCallback & a_Callback)
{
	// The entity list is locked by the parent chunkmap's CS
	for (auto itr = m_Entities.begin(), itr2 = itr; itr != m_Entities.end(); itr = itr2)
	{
		++itr2;
		if (!(*itr)->IsTicking())
		{
			continue;
		}
		if (a_Callback.Item((*itr).get()))
		{
			return false;
		}
	}  // for itr - m_Entitites[]
	return true;
}





bool cChunk::ForEachEntityInBox(const cBoundingBox & a_Box, cEntityCallback & a_Callback)
{
	// The entity list is locked by the parent chunkmap's CS
	for (auto itr = m_Entities.begin(), itr2 = itr; itr != m_Entities.end(); itr = itr2)
	{
		++itr2;
		if (!(*itr)->IsTicking())
		{
			continue;
		}
		cBoundingBox EntBox((*itr)->GetPosition(), (*itr)->GetWidth() / 2, (*itr)->GetHeight());
		if (!EntBox.DoesIntersect(a_Box))
		{
			// The entity is not in the specified box
			continue;
		}
		if (a_Callback.Item((*itr).get()))
		{
			return false;
		}
	}  // for itr - m_Entitites[]
	return true;
}





bool cChunk::DoWithEntityByID(UInt32 a_EntityID, cEntityCallback & a_Callback, bool & a_CallbackResult)
{
	for (auto & Entity : m_Entities)
	{
		if ((Entity->GetUniqueID() == a_EntityID) && Entity->IsTicking())
		{
			a_CallbackResult = a_Callback.Item(Entity.get());
			return true;
		}
	}
	return false;
}





bool cChunk::ForEachBlockEntity(cBlockEntityCallback & a_Callback)
{
	// The blockentity list is locked by the parent chunkmap's CS
	for (cBlockEntityList::iterator itr = m_BlockEntities.begin(), itr2 = itr; itr != m_BlockEntities.end(); itr = itr2)
	{
		++itr2;
		if (a_Callback.Item(*itr))
		{
			return false;
		}
	}  // for itr - m_BlockEntitites[]
	return true;
}





bool cChunk::ForEachBrewingstand(cBrewingstandCallback & a_Callback)
{
	// The blockentity list is locked by the parent chunkmap's CS
	for (cBlockEntityList::iterator itr = m_BlockEntities.begin(), itr2 = itr; itr != m_BlockEntities.end(); itr = itr2)
	{
		++itr2;
		if (a_Callback.Item(reinterpret_cast<cBrewingstandEntity *>(*itr)))
		{
			return false;
		}
	}  // for itr - m_BlockEntitites[]
	return true;
}





bool cChunk::ForEachChest(cChestCallback & a_Callback)
{
	// The blockentity list is locked by the parent chunkmap's CS
	for (cBlockEntityList::iterator itr = m_BlockEntities.begin(), itr2 = itr; itr != m_BlockEntities.end(); itr = itr2)
	{
		++itr2;
		if ((*itr)->GetBlockType() != E_BLOCK_CHEST)
		{
			continue;
		}
		if (a_Callback.Item(reinterpret_cast<cChestEntity *>(*itr)))
		{
			return false;
		}
	}  // for itr - m_BlockEntitites[]
	return true;
}





bool cChunk::ForEachDispenser(cDispenserCallback & a_Callback)
{
	// The blockentity list is locked by the parent chunkmap's CS
	for (cBlockEntityList::iterator itr = m_BlockEntities.begin(), itr2 = itr; itr != m_BlockEntities.end(); itr = itr2)
	{
		++itr2;
		if ((*itr)->GetBlockType() != E_BLOCK_DISPENSER)
		{
			continue;
		}
		if (a_Callback.Item(reinterpret_cast<cDispenserEntity *>(*itr)))
		{
			return false;
		}
	}  // for itr - m_BlockEntitites[]
	return true;
}





bool cChunk::ForEachDropper(cDropperCallback & a_Callback)
{
	// The blockentity list is locked by the parent chunkmap's CS
	for (cBlockEntityList::iterator itr = m_BlockEntities.begin(), itr2 = itr; itr != m_BlockEntities.end(); itr = itr2)
	{
		++itr2;
		if ((*itr)->GetBlockType() != E_BLOCK_DROPPER)
		{
			continue;
		}
		if (a_Callback.Item(reinterpret_cast<cDropperEntity *>(*itr)))
		{
			return false;
		}
	}  // for itr - m_BlockEntitites[]
	return true;
}





bool cChunk::ForEachDropSpenser(cDropSpenserCallback & a_Callback)
{
	// The blockentity list is locked by the parent chunkmap's CS
	for (cBlockEntityList::iterator itr = m_BlockEntities.begin(), itr2 = itr; itr != m_BlockEntities.end(); itr = itr2)
	{
		++itr2;
		if (((*itr)->GetBlockType() != E_BLOCK_DISPENSER) && ((*itr)->GetBlockType() != E_BLOCK_DROPPER))
		{
			continue;
		}
		if (a_Callback.Item(reinterpret_cast<cDropSpenserEntity *>(*itr)))
		{
			return false;
		}
	}  // for itr - m_BlockEntitites[]
	return true;
}





bool cChunk::ForEachFurnace(cFurnaceCallback & a_Callback)
{
	// The blockentity list is locked by the parent chunkmap's CS
	for (cBlockEntityList::iterator itr = m_BlockEntities.begin(), itr2 = itr; itr != m_BlockEntities.end(); itr = itr2)
	{
		++itr2;
		switch ((*itr)->GetBlockType())
		{
			case E_BLOCK_FURNACE:
			case E_BLOCK_LIT_FURNACE:
			{
				break;
			}
			default:
			{
				continue;
			}
		}
		if (a_Callback.Item(reinterpret_cast<cFurnaceEntity *>(*itr)))
		{
			return false;
		}
	}  // for itr - m_BlockEntitites[]
	return true;
}





bool cChunk::DoWithBlockEntityAt(int a_BlockX, int a_BlockY, int a_BlockZ, cBlockEntityCallback & a_Callback)
{
	// The blockentity list is locked by the parent chunkmap's CS
	for (cBlockEntityList::iterator itr = m_BlockEntities.begin(), itr2 = itr; itr != m_BlockEntities.end(); itr = itr2)
	{
		++itr2;
		if (((*itr)->GetPosX() != a_BlockX) || ((*itr)->GetPosY() != a_BlockY) || ((*itr)->GetPosZ() != a_BlockZ))
		{
			continue;
		}

		if (a_Callback.Item(*itr))
		{
			return false;
		}
		return true;
	}  // for itr - m_BlockEntitites[]

	// Not found:
	return false;
}




bool cChunk::DoWithBeaconAt(int a_BlockX, int a_BlockY, int a_BlockZ, cBeaconCallback & a_Callback)
{
	// The blockentity list is locked by the parent chunkmap's CS
	for (cBlockEntityList::iterator itr = m_BlockEntities.begin(), itr2 = itr; itr != m_BlockEntities.end(); itr = itr2)
	{
		++itr2;
		if (((*itr)->GetPosX() != a_BlockX) || ((*itr)->GetPosY() != a_BlockY) || ((*itr)->GetPosZ() != a_BlockZ))
		{
			continue;
		}
		if ((*itr)->GetBlockType() != E_BLOCK_BEACON)
		{
			// There is a block entity here, but of different type. No other block entity can be here, so we can safely bail out
			return false;
		}

		// The correct block entity is here
		if (a_Callback.Item(reinterpret_cast<cBeaconEntity *>(*itr)))
		{
			return false;
		}
		return true;
	}  // for itr - m_BlockEntitites[]

	// Not found:
	return false;
}





bool cChunk::DoWithBrewingstandAt(int a_BlockX, int a_BlockY, int a_BlockZ, cBrewingstandCallback & a_Callback)
{
	// The blockentity list is locked by the parent chunkmap's CS
	for (cBlockEntityList::iterator itr = m_BlockEntities.begin(), itr2 = itr; itr != m_BlockEntities.end(); itr = itr2)
	{
		++itr2;
		if (((*itr)->GetPosX() != a_BlockX) || ((*itr)->GetPosY() != a_BlockY) || ((*itr)->GetPosZ() != a_BlockZ))
		{
			continue;
		}
		if ((*itr)->GetBlockType() != E_BLOCK_BREWING_STAND)
		{
			// There is a block entity here, but of different type. No other block entity can be here, so we can safely bail out
			return false;
		}

		// The correct block entity is here
		if (a_Callback.Item(reinterpret_cast<cBrewingstandEntity *>(*itr)))
		{
			return false;
		}
		return true;
	}  // for itr - m_BlockEntitites[]

	// Not found:
	return false;
}





bool cChunk::DoWithChestAt(int a_BlockX, int a_BlockY, int a_BlockZ, cChestCallback & a_Callback)
{
	// The blockentity list is locked by the parent chunkmap's CS
	for (cBlockEntityList::iterator itr = m_BlockEntities.begin(), itr2 = itr; itr != m_BlockEntities.end(); itr = itr2)
	{
		++itr2;
		if (((*itr)->GetPosX() != a_BlockX) || ((*itr)->GetPosY() != a_BlockY) || ((*itr)->GetPosZ() != a_BlockZ))
		{
			continue;
		}
		if (((*itr)->GetBlockType() != E_BLOCK_CHEST) && ((*itr)->GetBlockType() != E_BLOCK_TRAPPED_CHEST))  // Trapped chests use normal chests' handlers
		{
			// There is a block entity here, but of different type. No other block entity can be here, so we can safely bail out
			return false;
		}

		// The correct block entity is here
		if (a_Callback.Item(reinterpret_cast<cChestEntity *>(*itr)))
		{
			return false;
		}
		return true;
	}  // for itr - m_BlockEntitites[]

	// Not found:
	return false;
}





bool cChunk::DoWithDispenserAt(int a_BlockX, int a_BlockY, int a_BlockZ, cDispenserCallback & a_Callback)
{
	// The blockentity list is locked by the parent chunkmap's CS
	for (cBlockEntityList::iterator itr = m_BlockEntities.begin(), itr2 = itr; itr != m_BlockEntities.end(); itr = itr2)
	{
		++itr2;
		if (((*itr)->GetPosX() != a_BlockX) || ((*itr)->GetPosY() != a_BlockY) || ((*itr)->GetPosZ() != a_BlockZ))
		{
			continue;
		}
		if ((*itr)->GetBlockType() != E_BLOCK_DISPENSER)
		{
			// There is a block entity here, but of different type. No other block entity can be here, so we can safely bail out
			return false;
		}

		// The correct block entity is here
		if (a_Callback.Item(reinterpret_cast<cDispenserEntity *>(*itr)))
		{
			return false;
		}
		return true;
	}  // for itr - m_BlockEntitites[]

	// Not found:
	return false;
}





bool cChunk::DoWithDropperAt(int a_BlockX, int a_BlockY, int a_BlockZ, cDropperCallback & a_Callback)
{
	// The blockentity list is locked by the parent chunkmap's CS
	for (cBlockEntityList::iterator itr = m_BlockEntities.begin(), itr2 = itr; itr != m_BlockEntities.end(); itr = itr2)
	{
		++itr2;
		if (((*itr)->GetPosX() != a_BlockX) || ((*itr)->GetPosY() != a_BlockY) || ((*itr)->GetPosZ() != a_BlockZ))
		{
			continue;
		}
		if ((*itr)->GetBlockType() != E_BLOCK_DROPPER)
		{
			// There is a block entity here, but of different type. No other block entity can be here, so we can safely bail out
			return false;
		}

		// The correct block entity is here
		if (a_Callback.Item(reinterpret_cast<cDropperEntity *>(*itr)))
		{
			return false;
		}
		return true;
	}  // for itr - m_BlockEntitites[]

	// Not found:
	return false;
}





bool cChunk::DoWithDropSpenserAt(int a_BlockX, int a_BlockY, int a_BlockZ, cDropSpenserCallback & a_Callback)
{
	// The blockentity list is locked by the parent chunkmap's CS
	for (cBlockEntityList::iterator itr = m_BlockEntities.begin(), itr2 = itr; itr != m_BlockEntities.end(); itr = itr2)
	{
		++itr2;
		if (((*itr)->GetPosX() != a_BlockX) || ((*itr)->GetPosY() != a_BlockY) || ((*itr)->GetPosZ() != a_BlockZ))
		{
			continue;
		}
		if (((*itr)->GetBlockType() != E_BLOCK_DISPENSER) && ((*itr)->GetBlockType() != E_BLOCK_DROPPER))
		{
			// There is a block entity here, but of different type. No other block entity can be here, so we can safely bail out
			return false;
		}

		// The correct block entity is here
		if (a_Callback.Item(reinterpret_cast<cDropSpenserEntity *>(*itr)))
		{
			return false;
		}
		return true;
	}  // for itr - m_BlockEntitites[]

	// Not found:
	return false;
}





bool cChunk::DoWithFurnaceAt(int a_BlockX, int a_BlockY, int a_BlockZ, cFurnaceCallback & a_Callback)
{
	// The blockentity list is locked by the parent chunkmap's CS
	for (cBlockEntityList::iterator itr = m_BlockEntities.begin(), itr2 = itr; itr != m_BlockEntities.end(); itr = itr2)
	{
		++itr2;
		if (((*itr)->GetPosX() != a_BlockX) || ((*itr)->GetPosY() != a_BlockY) || ((*itr)->GetPosZ() != a_BlockZ))
		{
			continue;
		}
		switch ((*itr)->GetBlockType())
		{
			case E_BLOCK_FURNACE:
			case E_BLOCK_LIT_FURNACE:
			{
				break;
			}
			default:
			{
				// There is a block entity here, but of different type. No other block entity can be here, so we can safely bail out
				return false;
			}
		}  // switch (BlockType)

		// The correct block entity is here,
		if (a_Callback.Item(reinterpret_cast<cFurnaceEntity *>(*itr)))
		{
			return false;
		}
		return true;
	}  // for itr - m_BlockEntitites[]

	// Not found:
	return false;
}





bool cChunk::DoWithNoteBlockAt(int a_BlockX, int a_BlockY, int a_BlockZ, cNoteBlockCallback & a_Callback)
{
	// The blockentity list is locked by the parent chunkmap's CS
	for (cBlockEntityList::iterator itr = m_BlockEntities.begin(), itr2 = itr; itr != m_BlockEntities.end(); itr = itr2)
	{
		++itr2;
		if (((*itr)->GetPosX() != a_BlockX) || ((*itr)->GetPosY() != a_BlockY) || ((*itr)->GetPosZ() != a_BlockZ))
		{
			continue;
		}
		if ((*itr)->GetBlockType() != E_BLOCK_NOTE_BLOCK)
		{
			// There is a block entity here, but of different type. No other block entity can be here, so we can safely bail out
			return false;
		}

		// The correct block entity is here
		if (a_Callback.Item(reinterpret_cast<cNoteEntity *>(*itr)))
		{
			return false;
		}
		return true;
	}  // for itr - m_BlockEntitites[]

	// Not found:
	return false;
}





bool cChunk::DoWithCommandBlockAt(int a_BlockX, int a_BlockY, int a_BlockZ, cCommandBlockCallback & a_Callback)
{
	// The blockentity list is locked by the parent chunkmap's CS
	for (cBlockEntityList::iterator itr = m_BlockEntities.begin(), itr2 = itr; itr != m_BlockEntities.end(); itr = itr2)
	{
		++itr2;
		if (((*itr)->GetPosX() != a_BlockX) || ((*itr)->GetPosY() != a_BlockY) || ((*itr)->GetPosZ() != a_BlockZ))
		{
			continue;
		}
		if ((*itr)->GetBlockType() != E_BLOCK_COMMAND_BLOCK)
		{
			// There is a block entity here, but of different type. No other block entity can be here, so we can safely bail out
			return false;
		}

		// The correct block entity is here,
		if (a_Callback.Item(reinterpret_cast<cCommandBlockEntity *>(*itr)))
		{
			return false;
		}
		return true;
	}  // for itr - m_BlockEntitites[]

	// Not found:
	return false;
}





bool cChunk::DoWithMobHeadAt(int a_BlockX, int a_BlockY, int a_BlockZ, cMobHeadCallback & a_Callback)
{
	// The blockentity list is locked by the parent chunkmap's CS
	for (cBlockEntityList::iterator itr = m_BlockEntities.begin(), itr2 = itr; itr != m_BlockEntities.end(); itr = itr2)
	{
		++itr2;
		if (((*itr)->GetPosX() != a_BlockX) || ((*itr)->GetPosY() != a_BlockY) || ((*itr)->GetPosZ() != a_BlockZ))
		{
			continue;
		}
		if ((*itr)->GetBlockType() != E_BLOCK_HEAD)
		{
			// There is a block entity here, but of different type. No other block entity can be here, so we can safely bail out
			return false;
		}

		// The correct block entity is here,
		if (a_Callback.Item(reinterpret_cast<cMobHeadEntity *>(*itr)))
		{
			return false;
		}
		return true;
	}  // for itr - m_BlockEntitites[]

	// Not found:
	return false;
}





bool cChunk::DoWithFlowerPotAt(int a_BlockX, int a_BlockY, int a_BlockZ, cFlowerPotCallback & a_Callback)
{
	// The blockentity list is locked by the parent chunkmap's CS
	for (cBlockEntityList::iterator itr = m_BlockEntities.begin(), itr2 = itr; itr != m_BlockEntities.end(); itr = itr2)
	{
		++itr2;
		if (((*itr)->GetPosX() != a_BlockX) || ((*itr)->GetPosY() != a_BlockY) || ((*itr)->GetPosZ() != a_BlockZ))
		{
			continue;
		}
		if ((*itr)->GetBlockType() != E_BLOCK_FLOWER_POT)
		{
			// There is a block entity here, but of different type. No other block entity can be here, so we can safely bail out
			return false;
		}

		// The correct block entity is here
		if (a_Callback.Item(reinterpret_cast<cFlowerPotEntity *>(*itr)))
		{
			return false;
		}
		return true;
	}  // for itr - m_BlockEntitites[]

	// Not found:
	return false;
}





bool cChunk::GetSignLines(int a_BlockX, int a_BlockY, int a_BlockZ, AString & a_Line1, AString & a_Line2, AString & a_Line3, AString & a_Line4)
{
	// The blockentity list is locked by the parent chunkmap's CS
	for (cBlockEntityList::iterator itr = m_BlockEntities.begin(); itr != m_BlockEntities.end(); ++itr)
	{
		if (((*itr)->GetPosX() != a_BlockX) || ((*itr)->GetPosY() != a_BlockY) || ((*itr)->GetPosZ() != a_BlockZ))
		{
			continue;
		}
		switch ((*itr)->GetBlockType())
		{
			case E_BLOCK_WALLSIGN:
			case E_BLOCK_SIGN_POST:
			{
				a_Line1 = reinterpret_cast<cSignEntity *>(*itr)->GetLine(0);
				a_Line2 = reinterpret_cast<cSignEntity *>(*itr)->GetLine(1);
				a_Line3 = reinterpret_cast<cSignEntity *>(*itr)->GetLine(2);
				a_Line4 = reinterpret_cast<cSignEntity *>(*itr)->GetLine(3);
				return true;
			}
		}  // switch (BlockType)

		// There is a block entity here, but of different type. No other block entity can be here, so we can safely bail out
		return false;
	}  // for itr - m_BlockEntitites[]

	// Not found:
	return false;
}





BLOCKTYPE cChunk::GetBlock(int a_RelX, int a_RelY, int a_RelZ) const
{
	return m_ChunkData.GetBlock(a_RelX, a_RelY, a_RelZ);
}





void cChunk::GetBlockTypeMeta(int a_RelX, int a_RelY, int a_RelZ, BLOCKTYPE & a_BlockType, NIBBLETYPE & a_BlockMeta) const
{
	a_BlockType = GetBlock(a_RelX, a_RelY, a_RelZ);
	a_BlockMeta = m_ChunkData.GetMeta(a_RelX, a_RelY, a_RelZ);
}





void cChunk::GetBlockInfo(int a_RelX, int a_RelY, int a_RelZ, BLOCKTYPE & a_BlockType, NIBBLETYPE & a_Meta, NIBBLETYPE & a_SkyLight, NIBBLETYPE & a_BlockLight)
{
	a_BlockType  = GetBlock(a_RelX, a_RelY, a_RelZ);
	a_Meta       = m_ChunkData.GetMeta(a_RelX, a_RelY, a_RelZ);
	a_SkyLight   = m_ChunkData.GetSkyLight(a_RelX, a_RelY, a_RelZ);
	a_BlockLight = m_ChunkData.GetBlockLight(a_RelX, a_RelY, a_RelZ);
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
		return m_ChunkMap->GetChunkNoLoad(ChunkX, ChunkZ);
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





cChunk * cChunk::GetRelNeighborChunkAdjustCoords(int & a_RelX, int & a_RelZ) const
{
	cChunk * ToReturn = const_cast<cChunk *>(this);

	// The most common case: inside this chunk:
	if (
		(a_RelX >= 0) && (a_RelX < Width) &&
		(a_RelZ >= 0) && (a_RelZ < Width)
	)
	{
		return ToReturn;
	}

	// Request for a different chunk, calculate chunk offset:
	int RelX = a_RelX;  // Make a local copy of the coords (faster access)
	int RelZ = a_RelZ;
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
		a_RelX = RelX;
		a_RelZ = RelZ;
		return ToReturn;
	}

	// The chunk cannot be walked through neighbors, find it through the chunkmap:
	int AbsX = a_RelX + m_PosX * Width;
	int AbsZ = a_RelZ + m_PosZ * Width;
	int DstChunkX, DstChunkZ;
	BlockToChunk(AbsX, AbsZ, DstChunkX, DstChunkZ);
	ToReturn = m_ChunkMap->FindChunk(DstChunkX, DstChunkZ);
	a_RelX = AbsX - DstChunkX * Width;
	a_RelZ = AbsZ - DstChunkZ * Width;
	return ToReturn;
}





void cChunk::BroadcastAttachEntity(const cEntity & a_Entity, const cEntity * a_Vehicle)
{
	for (const auto & ClientHandle : GetAllStrongClientPtrs())
	{
		ClientHandle->SendAttachEntity(a_Entity, a_Vehicle);
	}
}





void cChunk::BroadcastBlockAction(int a_BlockX, int a_BlockY, int a_BlockZ, char a_Byte1, char a_Byte2, BLOCKTYPE a_BlockType, const cClientHandle * a_Exclude)
{
	for (const auto & ClientHandle : GetAllStrongClientPtrs())
	{
		if (ClientHandle.get() == a_Exclude)
		{
			continue;
		}

		ClientHandle->SendBlockAction(a_BlockX, a_BlockY, a_BlockZ, a_Byte1, a_Byte2, a_BlockType);
	}
}





void cChunk::BroadcastBlockBreakAnimation(UInt32 a_EntityID, int a_BlockX, int a_BlockY, int a_BlockZ, char a_Stage, const cClientHandle * a_Exclude)
{
	for (const auto & ClientHandle : GetAllStrongClientPtrs())
	{
		if (ClientHandle.get() == a_Exclude)
		{
			continue;
		}

		ClientHandle->SendBlockBreakAnim(a_EntityID, a_BlockX, a_BlockY, a_BlockZ, a_Stage);
	}
}





void cChunk::BroadcastBlockEntity(int a_BlockX, int a_BlockY, int a_BlockZ, const cClientHandle * a_Exclude)
{
	// We can operate on entity pointers, we're inside the ChunkMap's CS lock which guards the list
	cBlockEntity * Entity = GetBlockEntity(a_BlockX, a_BlockY, a_BlockZ);
	if (Entity == nullptr)
	{
		return;
	}

	for (const auto & ClientHandle : GetAllStrongClientPtrs())
	{
		if (ClientHandle.get() == a_Exclude)
		{
			continue;
		}

		Entity->SendTo(*ClientHandle);
	}
}





void cChunk::BroadcastCollectEntity(const cEntity & a_Entity, const cPlayer & a_Player, const cClientHandle * a_Exclude)
{
	for (const auto & ClientHandle : GetAllStrongClientPtrs())
	{
		if (ClientHandle.get() == a_Exclude)
		{
			continue;
		}

		ClientHandle->SendCollectEntity(a_Entity, a_Player);
	}
}





void cChunk::BroadcastDestroyEntity(const cEntity & a_Entity, const cClientHandle * a_Exclude)
{
	for (const auto & ClientHandle : GetAllStrongClientPtrs())
	{
		if (ClientHandle.get() == a_Exclude)
		{
			continue;
		}

		ClientHandle->SendDestroyEntity(a_Entity);
	}
}





void cChunk::BroadcastEntityEffect(const cEntity & a_Entity, int a_EffectID, int a_Amplifier, short a_Duration, const cClientHandle * a_Exclude)
{
	for (const auto & ClientHandle : GetAllStrongClientPtrs())
	{
		if (ClientHandle.get() == a_Exclude)
		{
			continue;
		}

		ClientHandle->SendEntityEffect(a_Entity, a_EffectID, a_Amplifier, a_Duration);
	}
}





void cChunk::BroadcastEntityEquipment(const cEntity & a_Entity, short a_SlotNum, const cItem & a_Item, const cClientHandle * a_Exclude)
{
	for (const auto & ClientHandle : GetAllStrongClientPtrs())
	{
		if (ClientHandle.get() == a_Exclude)
		{
			continue;
		}

		ClientHandle->SendEntityEquipment(a_Entity, a_SlotNum, a_Item);
	}
}





void cChunk::BroadcastEntityHeadLook(const cEntity & a_Entity, const cClientHandle * a_Exclude)
{
	for (const auto & ClientHandle : GetAllStrongClientPtrs())
	{
		if (ClientHandle.get() == a_Exclude)
		{
			continue;
		}

		ClientHandle->SendEntityHeadLook(a_Entity);
	}
}





void cChunk::BroadcastEntityLook(const cEntity & a_Entity, const cClientHandle * a_Exclude)
{
	for (const auto & ClientHandle : GetAllStrongClientPtrs())
	{
		if (ClientHandle.get() == a_Exclude)
		{
			continue;
		}

		ClientHandle->SendEntityLook(a_Entity);
	}
}





void cChunk::BroadcastEntityMetadata(const cEntity & a_Entity, const cClientHandle * a_Exclude)
{
	for (const auto & ClientHandle : GetAllStrongClientPtrs())
	{
		if (ClientHandle.get() == a_Exclude)
		{
			continue;
		}

		ClientHandle->SendEntityMetadata(a_Entity);
	}
}





void cChunk::BroadcastEntityRelMove(const cEntity & a_Entity, char a_RelX, char a_RelY, char a_RelZ, const cClientHandle * a_Exclude)
{
	for (const auto & ClientHandle : GetAllStrongClientPtrs())
	{
		if (ClientHandle.get() == a_Exclude)
		{
			continue;
		}

		ClientHandle->SendEntityRelMove(a_Entity, a_RelX, a_RelY, a_RelZ);
	}
}





void cChunk::BroadcastEntityRelMoveLook(const cEntity & a_Entity, char a_RelX, char a_RelY, char a_RelZ, const cClientHandle * a_Exclude)
{
	for (const auto & ClientHandle : GetAllStrongClientPtrs())
	{
		if (ClientHandle.get() == a_Exclude)
		{
			continue;
		}

		ClientHandle->SendEntityRelMoveLook(a_Entity, a_RelX, a_RelY, a_RelZ);
	}
}





void cChunk::BroadcastEntityStatus(const cEntity & a_Entity, char a_Status, const cClientHandle * a_Exclude)
{
	for (const auto & ClientHandle : GetAllStrongClientPtrs())
	{
		if (ClientHandle.get() == a_Exclude)
		{
			continue;
		}

		ClientHandle->SendEntityStatus(a_Entity, a_Status);
	}
}





void cChunk::BroadcastEntityVelocity(const cEntity & a_Entity, const cClientHandle * a_Exclude)
{
	for (const auto & ClientHandle : GetAllStrongClientPtrs())
	{
		if (ClientHandle.get() == a_Exclude)
		{
			continue;
		}

		ClientHandle->SendEntityVelocity(a_Entity);
	}
}





void cChunk::BroadcastEntityAnimation(const cEntity & a_Entity, char a_Animation, const cClientHandle * a_Exclude)
{
	for (const auto & ClientHandle : GetAllStrongClientPtrs())
	{
		if (ClientHandle.get() == a_Exclude)
		{
			continue;
		}

		ClientHandle->SendEntityAnimation(a_Entity, a_Animation);
	}
}





void cChunk::BroadcastParticleEffect(const AString & a_ParticleName, float a_SrcX, float a_SrcY, float a_SrcZ, float a_OffsetX, float a_OffsetY, float a_OffsetZ, float a_ParticleData, int a_ParticleAmount, cClientHandle * a_Exclude)
{
	for (const auto & ClientHandle : GetAllStrongClientPtrs())
	{
		if (ClientHandle.get() == a_Exclude)
		{
			continue;
		}

		ClientHandle->SendParticleEffect(a_ParticleName, a_SrcX, a_SrcY, a_SrcZ, a_OffsetX, a_OffsetY, a_OffsetZ, a_ParticleData, a_ParticleAmount);
	}
}





void cChunk::BroadcastRemoveEntityEffect(const cEntity & a_Entity, int a_EffectID, const cClientHandle * a_Exclude)
{
	for (const auto & ClientHandle : GetAllStrongClientPtrs())
	{
		if (ClientHandle.get() == a_Exclude)
		{
			continue;
		}

		ClientHandle->SendRemoveEntityEffect(a_Entity, a_EffectID);
	}
}





void cChunk::BroadcastSoundEffect(const AString & a_SoundName, double a_X, double a_Y, double a_Z, float a_Volume, float a_Pitch, const cClientHandle * a_Exclude)
{
	for (const auto & ClientHandle : GetAllStrongClientPtrs())
	{
		if (ClientHandle.get() == a_Exclude)
		{
			continue;
		}

		ClientHandle->SendSoundEffect(a_SoundName, a_X, a_Y, a_Z, a_Volume, a_Pitch);
	}
}





void cChunk::BroadcastSoundParticleEffect(const EffectID a_EffectID, int a_SrcX, int a_SrcY, int a_SrcZ, int a_Data, const cClientHandle * a_Exclude)
{
	for (const auto & ClientHandle : GetAllStrongClientPtrs())
	{
		if (ClientHandle.get() == a_Exclude)
		{
			continue;
		}

		ClientHandle->SendSoundParticleEffect(a_EffectID, a_SrcX, a_SrcY, a_SrcZ, a_Data);
	}
}





void cChunk::BroadcastSpawnEntity(cEntity & a_Entity, const cClientHandle * a_Exclude)
{
	for (const auto & ClientHandle : GetAllStrongClientPtrs())
	{
		if (ClientHandle.get() == a_Exclude)
		{
			continue;
		}

		a_Entity.SpawnOn(*ClientHandle);
	}
}





void cChunk::BroadcastThunderbolt(int a_BlockX, int a_BlockY, int a_BlockZ, const cClientHandle * a_Exclude)
{
	for (const auto & ClientHandle : GetAllStrongClientPtrs())
	{
		if (ClientHandle.get() == a_Exclude)
		{
			continue;
		}

		ClientHandle->SendThunderbolt(a_BlockX, a_BlockY, a_BlockZ);
	}
}





void cChunk::BroadcastUseBed(const cEntity & a_Entity, int a_BlockX, int a_BlockY, int a_BlockZ)
{
	for (const auto & ClientHandle : GetAllStrongClientPtrs())
	{
		ClientHandle->SendUseBed(a_Entity, a_BlockX, a_BlockY, a_BlockZ);
	}
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
