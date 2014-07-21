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
#include "BlockEntities/ChestEntity.h"
#include "BlockEntities/DispenserEntity.h"
#include "BlockEntities/DropperEntity.h"
#include "BlockEntities/FurnaceEntity.h"
#include "BlockEntities/HopperEntity.h"
#include "BlockEntities/JukeboxEntity.h"
#include "BlockEntities/NoteEntity.h"
#include "BlockEntities/SignEntity.h"
#include "BlockEntities/MobHeadEntity.h"
#include "BlockEntities/FlowerPotEntity.h"
#include "Entities/Pickup.h"
#include "Item.h"
#include "Noise.h"
#include "Root.h"
#include "MersenneTwister.h"
#include "Entities/Player.h"
#include "BlockArea.h"
#include "Bindings/PluginManager.h"
#include "Blocks/BlockHandler.h"
#include "Simulator/FluidSimulator.h"
#include "MobCensus.h"
#include "MobSpawner.h"
#include "BlockInServerPluginInterface.h"

#include "json/json.h"





////////////////////////////////////////////////////////////////////////////////
// sSetBlock:

sSetBlock::sSetBlock( int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta)  // absolute block position
	: x( a_BlockX)
	, y( a_BlockY)
	, z( a_BlockZ)
	, BlockType( a_BlockType)
	, BlockMeta( a_BlockMeta)
{
	cChunkDef::AbsoluteToRelative(x, y, z, ChunkX, ChunkZ);
}





////////////////////////////////////////////////////////////////////////////////
// cChunk:

cChunk::cChunk(
	int a_ChunkX, int a_ChunkY, int a_ChunkZ,
	cChunkMap * a_ChunkMap, cWorld * a_World,
	cChunk * a_NeighborXM, cChunk * a_NeighborXP, cChunk * a_NeighborZM, cChunk * a_NeighborZP,
	cAllocationPool<cChunkData::sChunkSection> & a_Pool
) :
	m_IsValid(false),
	m_IsLightValid(false),
	m_IsDirty(false),
	m_IsSaving(false),
	m_HasLoadFailed(false),
	m_StayCount(0),
	m_PosX(a_ChunkX),
	m_PosY(a_ChunkY),
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
	m_AlwaysTicked(0)
{
	if (a_NeighborXM != NULL)
	{
		a_NeighborXM->m_NeighborXP = this;
	}
	if (a_NeighborXP != NULL)
	{
		a_NeighborXP->m_NeighborXM = this;
	}
	if (a_NeighborZM != NULL)
	{
		a_NeighborZM->m_NeighborZP = this;
	}
	if (a_NeighborZP != NULL)
	{
		a_NeighborZP->m_NeighborZM = this;
	}
}





cChunk::~cChunk()
{
	cPluginManager::Get()->CallHookChunkUnloaded(m_World, m_PosX, m_PosZ);
	
	// LOGINFO("### delete cChunk() (%i, %i) from %p, thread 0x%x ###", m_PosX, m_PosZ, this, GetCurrentThreadId());
	
	for (cBlockEntityList::iterator itr = m_BlockEntities.begin(); itr != m_BlockEntities.end(); ++itr)
	{
		delete *itr;
	}
	m_BlockEntities.clear();

	// Remove and destroy all entities that are not players:
	cEntityList Entities;
	std::swap(Entities, m_Entities);  // Need another list because cEntity destructors check if they've been removed from chunk
	for (cEntityList::const_iterator itr = Entities.begin(); itr != Entities.end(); ++itr)
	{
		if (!(*itr)->IsPlayer())
		{
			(*itr)->Destroy(false);
			delete *itr;
		}
	}
	
	if (m_NeighborXM != NULL)
	{
		m_NeighborXM->m_NeighborXP = NULL;
	}
	if (m_NeighborXP != NULL)
	{
		m_NeighborXP->m_NeighborXM = NULL;
	}
	if (m_NeighborZM != NULL)
	{
		m_NeighborZM->m_NeighborZP = NULL;
	}
	if (m_NeighborZP != NULL)
	{
		m_NeighborZP->m_NeighborZM = NULL;
	}
	delete m_WaterSimulatorData;
	m_WaterSimulatorData = NULL;
	delete m_LavaSimulatorData;
	m_LavaSimulatorData = NULL;
}





void cChunk::SetValid(void)
{
	m_IsValid = true;
	
	m_World->GetChunkMap()->ChunkValidated();
}





void cChunk::MarkRegenerating(void)
{
	// Tell all clients attached to this chunk that they want this chunk:
	for (cClientHandleList::iterator itr = m_LoadedByClient.begin(); itr != m_LoadedByClient.end(); ++itr)
	{
		(*itr)->AddWantedChunk(m_PosX, m_PosZ);
	}  // for itr - m_LoadedByClient[]
}





bool cChunk::CanUnload(void)
{
	return m_LoadedByClient.empty() && !m_IsDirty && (m_StayCount == 0);
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
	SetValid();
}





void cChunk::MarkLoadFailed(void)
{
	if (m_IsValid)
	{
		return;
	}
	
	m_HasLoadFailed = true;
}





void cChunk::GetAllData(cChunkDataCallback & a_Callback)
{
	a_Callback.HeightMap(&m_HeightMap);
	a_Callback.BiomeData(&m_BiomeMap);

	a_Callback.LightIsValid(m_IsLightValid);

	a_Callback.ChunkData(m_ChunkData);
	
	for (cEntityList::iterator itr = m_Entities.begin(); itr != m_Entities.end(); ++itr)
	{
		a_Callback.Entity(*itr);
	}
	
	for (cBlockEntityList::iterator itr = m_BlockEntities.begin(); itr != m_BlockEntities.end(); ++itr)
	{
		a_Callback.BlockEntity(*itr);
	}
}





void cChunk::SetAllData(
	const BLOCKTYPE *  a_BlockTypes,
	const NIBBLETYPE * a_BlockMeta,
	const NIBBLETYPE * a_BlockLight,
	const NIBBLETYPE * a_BlockSkyLight,
	const HeightMap *  a_HeightMap,
	const BiomeMap &   a_BiomeMap,
	cBlockEntityList & a_BlockEntities
)
{
	memcpy(m_BiomeMap, a_BiomeMap, sizeof(m_BiomeMap));

	if (a_HeightMap != NULL)
	{
		memcpy(m_HeightMap, a_HeightMap, sizeof(m_HeightMap));
	}

	if (a_HeightMap == NULL)
	{
		CalculateHeightmap(a_BlockTypes);
	}

	m_ChunkData.SetBlockTypes(a_BlockTypes);
	m_ChunkData.SetMetas(a_BlockMeta);
	m_ChunkData.SetBlockLight(a_BlockLight);
	m_ChunkData.SetSkyLight(a_BlockSkyLight);
	
	m_IsLightValid = (a_BlockLight != NULL) && (a_BlockSkyLight != NULL);

	// Clear the block entities present - either the loader / saver has better, or we'll create empty ones:
	for (cBlockEntityList::iterator itr = m_BlockEntities.begin(); itr != m_BlockEntities.end(); ++itr)
	{
		delete *itr;
	}
	std::swap(a_BlockEntities, m_BlockEntities);
	
	// Set all block entities' World variable:
	for (cBlockEntityList::iterator itr = m_BlockEntities.begin(); itr != m_BlockEntities.end(); ++itr)
	{
		(*itr)->SetWorld(m_World);
	}
	
	// Create block entities that the loader didn't load; fill them with defaults
	CreateBlockEntities();
	
	// Set the chunk data as valid. This may be needed for some simulators that perform actions upon block adding (Vaporize)
	SetValid();
	
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
	int SizeY = a_Area.GetSizeY();

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





/// Returns true if there is a block entity at the coords specified
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




void cChunk::CollectMobCensus(cMobCensus& toFill)
{
	toFill.CollectSpawnableChunk(*this);
	std::list<const Vector3d*> playerPositions;
	cPlayer* currentPlayer;
	for (cClientHandleList::iterator itr = m_LoadedByClient.begin(), end = m_LoadedByClient.end(); itr != end; ++itr)
	{
		currentPlayer = (*itr)->GetPlayer();
		playerPositions.push_back(&(currentPlayer->GetPosition()));
	}

	Vector3d currentPosition;
	for (cEntityList::iterator itr = m_Entities.begin(); itr != m_Entities.end(); ++itr)
	{
		// LOGD("Counting entity #%i (%s)", (*itr)->GetUniqueID(), (*itr)->GetClass());
		if ((*itr)->IsMob())
		{
			cMonster& Monster = (cMonster&)(**itr);
			currentPosition = Monster.GetPosition();
			for (std::list<const Vector3d*>::const_iterator itr2 = playerPositions.begin(); itr2 != playerPositions.end(); ++itr2)
			{
				toFill.CollectMob(Monster, *this, (currentPosition - **itr2).SqrLength());
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





void cChunk::SpawnMobs(cMobSpawner& a_MobSpawner)
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
		
		cEntity * newMob = a_MobSpawner.TryToSpawnHere(this, TryX, TryY, TryZ, Biome, MaxNbOfSuccess);
		if (newMob == NULL)
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





void cChunk::Tick(float a_Dt)
{
	BroadcastPendingBlockChanges();

	// Set all blocks that have been queued for setting later:
	ProcessQueuedSetBlocks();

	CheckBlocks();
	
	// Tick simulators:
	m_World->GetSimulatorManager()->SimulateChunk(a_Dt, m_PosX, m_PosZ, this);
	
	TickBlocks();

	// Tick all block entities in this chunk:
	for (cBlockEntityList::iterator itr = m_BlockEntities.begin(); itr != m_BlockEntities.end(); ++itr)
	{
		m_IsDirty = (*itr)->Tick(a_Dt, *this) | m_IsDirty;
	}
	
	// Tick all entities in this chunk (except mobs):
	for (cEntityList::iterator itr = m_Entities.begin(); itr != m_Entities.end(); ++itr)
	{
		// Mobs are tickes inside MobTick (as we don't have to tick them if they are far away from players)
		if (!((*itr)->IsMob()))
		{
			(*itr)->Tick(a_Dt, *this);
		}
	}  // for itr - m_Entitites[]
	
	// Remove all entities that were scheduled for removal:
	for (cEntityList::iterator itr = m_Entities.begin(); itr != m_Entities.end();)
	{
			if ((*itr)->IsDestroyed())
			{
				LOGD("Destroying entity #%i (%s)", (*itr)->GetUniqueID(), (*itr)->GetClass());
				cEntity * ToDelete = *itr;
				itr = m_Entities.erase(itr);
				delete ToDelete;
				ToDelete = NULL;
				continue;
			}
			++itr;
	}  // for itr - m_Entitites[]
	
	// If any entity moved out of the chunk, move it to the neighbor:
	for (cEntityList::iterator itr = m_Entities.begin(); itr != m_Entities.end();)
	{
		if (
			((*itr)->GetChunkX() != m_PosX) ||
			((*itr)->GetChunkZ() != m_PosZ)
		)
		{
			MoveEntityToNewChunk(*itr);
			itr = m_Entities.erase(itr);
		}
		else
		{
			++itr;
		}
	}
	
	ApplyWeatherToTop();
}





void cChunk::TickBlock(int a_RelX, int a_RelY, int a_RelZ)
{
	cBlockHandler * Handler = BlockHandler(GetBlock(a_RelX, a_RelY, a_RelZ));
	ASSERT(Handler != NULL);  // Happenned on server restart, FS #243
	cChunkInterface ChunkInterface(this->GetWorld()->GetChunkMap());
	cBlockInServerPluginInterface PluginInterface(*this->GetWorld());
	Handler->OnUpdate(ChunkInterface, *this->GetWorld(), PluginInterface, *this, a_RelX, a_RelY, a_RelZ);
}





void cChunk::MoveEntityToNewChunk(cEntity * a_Entity)
{
	cChunk * Neighbor = GetNeighborChunk(a_Entity->GetChunkX() * cChunkDef::Width, a_Entity->GetChunkZ() * cChunkDef::Width);
	if (Neighbor == NULL)
	{
		Neighbor = m_ChunkMap->GetChunkNoLoad(a_Entity->GetChunkX(), ZERO_CHUNK_Y, a_Entity->GetChunkZ());
		if (Neighbor == NULL)
		{
			// TODO: What to do with this?
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
			a_Client->SendDestroyEntity(*m_Entity);
		}

		virtual void Added(cClientHandle * a_Client) override
		{
			m_Entity->SpawnOn(*a_Client);
		}

		cEntity * m_Entity;

	public:
		cMover(cEntity * a_Entity) :
			m_Entity(a_Entity)
		{}
	} Mover(a_Entity);
	
	m_ChunkMap->CompareChunkClients(this, Neighbor, Mover);
}





void cChunk::ProcessQueuedSetBlocks(void)
{
	Int64 CurrTick = m_World->GetWorldAge();
	for (sSetBlockQueueVector::iterator itr = m_SetBlockQueue.begin(); itr != m_SetBlockQueue.end();)
	{
		if (itr->m_Tick <= CurrTick)
		{
			if (itr->m_PreviousType != E_BLOCK_AIR)  // PreviousType defaults to 0 if not specified
			{
				if (GetBlock(itr->m_RelX, itr->m_RelY, itr->m_RelZ) == itr->m_PreviousType)
				{
					// Current world age is bigger than/equal to target world age - delay time reached AND
					// Previous block type was the same as current block type (to prevent duplication)
					SetBlock(itr->m_RelX, itr->m_RelY, itr->m_RelZ, itr->m_BlockType, itr->m_BlockMeta);  // SetMeta doesn't send to client
					itr = m_SetBlockQueue.erase(itr);
					LOGD("Successfully set queued block - previous and current types matched");
				}
				else
				{
					itr = m_SetBlockQueue.erase(itr);
					LOGD("Failure setting queued block - previous and current blocktypes didn't match");
				}
			}
			else
			{
				// Current world age is bigger than/equal to target world age - delay time reached
				SetBlock(itr->m_RelX, itr->m_RelY, itr->m_RelZ, itr->m_BlockType, itr->m_BlockMeta);
				itr = m_SetBlockQueue.erase(itr);
				LOGD("Successfully set queued block - previous type ignored");
			}
		}
		else
		{
			// Not yet
			++itr;
			continue;
		}
	}  // for itr - m_SetBlockQueue[]
}





void cChunk::BroadcastPendingBlockChanges(void)
{
	if (m_PendingSendBlocks.empty())
	{
		return;
	}
	
	for (cClientHandleList::iterator itr = m_LoadedByClient.begin(), end = m_LoadedByClient.end(); itr != end; ++itr)
	{
		(*itr)->SendBlockChanges(m_PosX, m_PosZ, m_PendingSendBlocks);
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
	// _X: We must limit the random number or else we get a nasty int overflow bug - http://forum.mc-server.org/showthread.php?tid=457
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
		ASSERT(Handler != NULL);  // Happenned on server restart, FS #243
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
	switch (GetBiomeAt(X, Z))
	{
		case biTaiga:
		case biFrozenOcean:
		case biFrozenRiver:
		case biIcePlains:
		case biIceMountains:
		case biTaigaHills:
		{
			// TODO: Check light levels, don't snow over when the BlockLight is higher than (7?)
			int Height = GetHeight(X, Z);
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
			else if (cBlockInfo::IsSnowable(TopBlock) && (Height + 1 < cChunkDef::Height))
			{
				SetBlock(X, Height + 1, Z, E_BLOCK_SNOW, 0);
			}
			else if ((TopBlock == E_BLOCK_WATER) || (TopBlock == E_BLOCK_STATIONARY_WATER))
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
			break;
		}  // case (snowy biomes)
		// TODO: Rainy biomes should check for farmland and cauldrons
		default:
		{
			break;
		}
	}  // switch (biome)
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
	UnboundedRelGetBlock(a_RelX + x, a_RelY - 1, a_RelZ + z, Soil, BlockMeta);
	switch (Soil)
	{
		case E_BLOCK_DIRT:
		case E_BLOCK_GRASS:
		case E_BLOCK_FARMLAND:
		{
			// DEBUG: This is here to catch FS #349 - melons growing over other crops.
			LOG("Growing melon/pumpkin overwriting %s, growing on %s",
				ItemTypeToString(BlockType[CheckType]).c_str(),
				ItemTypeToString(Soil).c_str()
			);
			// Place a randomly-facing produce:
			UnboundedRelFastSetBlock(a_RelX + x, a_RelY, a_RelZ + z, ProduceType, (NIBBLETYPE)(a_TickRandom.randInt(4) % 4));
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
	if ((a_RelY < 0) || (a_RelY >= cChunkDef::Height))
	{
		LOGWARNING("%s: requesting a block with a_RelY out of range: %d", __FUNCTION__, a_RelY);
		return false;
	}
	cChunk * Chunk = GetRelNeighborChunkAdjustCoords(a_RelX, a_RelZ);
	if ((Chunk == NULL) || !Chunk->IsValid())
	{
		// The chunk is not available, bail out
		return false;
	}
	Chunk->GetBlockTypeMeta(a_RelX, a_RelY, a_RelZ, a_BlockType, a_BlockMeta);
	return true;
}





bool cChunk::UnboundedRelGetBlockType(int a_RelX, int a_RelY, int a_RelZ, BLOCKTYPE & a_BlockType) const
{
	if ((a_RelY < 0) || (a_RelY >= cChunkDef::Height))
	{
		LOGWARNING("%s: requesting a block with a_RelY out of range: %d", __FUNCTION__, a_RelY);
		return false;
	}
	cChunk * Chunk = GetRelNeighborChunkAdjustCoords(a_RelX, a_RelZ);
	if ((Chunk == NULL) || !Chunk->IsValid())
	{
		// The chunk is not available, bail out
		return false;
	}
	a_BlockType = Chunk->GetBlock(a_RelX, a_RelY, a_RelZ);
	return true;
}





bool cChunk::UnboundedRelGetBlockMeta(int a_RelX, int a_RelY, int a_RelZ, NIBBLETYPE & a_BlockMeta) const
{
	if ((a_RelY < 0) || (a_RelY >= cChunkDef::Height))
	{
		LOGWARNING("%s: requesting a block with a_RelY out of range: %d", __FUNCTION__, a_RelY);
		return false;
	}
	cChunk * Chunk = GetRelNeighborChunkAdjustCoords(a_RelX, a_RelZ);
	if ((Chunk == NULL) || !Chunk->IsValid())
	{
		// The chunk is not available, bail out
		return false;
	}
	a_BlockMeta = Chunk->GetMeta(a_RelX, a_RelY, a_RelZ);
	return true;
}





bool cChunk::UnboundedRelGetBlockBlockLight(int a_RelX, int a_RelY, int a_RelZ, NIBBLETYPE & a_BlockBlockLight) const
{
	if ((a_RelY < 0) || (a_RelY >= cChunkDef::Height))
	{
		LOGWARNING("%s: requesting a block with a_RelY out of range: %d", __FUNCTION__, a_RelY);
		return false;
	}
	cChunk * Chunk = GetRelNeighborChunkAdjustCoords(a_RelX, a_RelZ);
	if ((Chunk == NULL) || !Chunk->IsValid())
	{
		// The chunk is not available, bail out
		return false;
	}
	a_BlockBlockLight = Chunk->GetBlockLight(a_RelX, a_RelY, a_RelZ);
	return true;
}





bool cChunk::UnboundedRelGetBlockSkyLight(int a_RelX, int a_RelY, int a_RelZ, NIBBLETYPE & a_BlockSkyLight) const
{
	if ((a_RelY < 0) || (a_RelY >= cChunkDef::Height))
	{
		LOGWARNING("%s: requesting a block with a_RelY out of range: %d", __FUNCTION__, a_RelY);
		return false;
	}
	cChunk * Chunk = GetRelNeighborChunkAdjustCoords(a_RelX, a_RelZ);
	if ((Chunk == NULL) || !Chunk->IsValid())
	{
		// The chunk is not available, bail out
		return false;
	}
	a_BlockSkyLight = Chunk->GetSkyLight(a_RelX, a_RelY, a_RelZ);
	return true;
}





bool cChunk::UnboundedRelGetBlockLights(int a_RelX, int a_RelY, int a_RelZ, NIBBLETYPE & a_BlockLight, NIBBLETYPE & a_SkyLight) const
{
	if ((a_RelY < 0) || (a_RelY >= cChunkDef::Height))
	{
		LOGWARNING("%s: requesting a block with a_RelY out of range: %d", __FUNCTION__, a_RelY);
		return false;
	}
	cChunk * Chunk = GetRelNeighborChunkAdjustCoords(a_RelX, a_RelZ);
	if ((Chunk == NULL) || !Chunk->IsValid())
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
	if ((a_RelY < 0) || (a_RelY > cChunkDef::Height))
	{
		LOGWARNING("UnboundedRelSetBlock(): requesting a block with a_RelY out of range: %d", a_RelY);
		return false;
	}
	cChunk * Chunk = GetRelNeighborChunkAdjustCoords(a_RelX, a_RelZ);
	if ((Chunk == NULL) || !Chunk->IsValid())
	{
		// The chunk is not available, bail out
		return false;
	}
	Chunk->SetBlock(a_RelX, a_RelY, a_RelZ, a_BlockType, a_BlockMeta);
	return true;
}





bool cChunk::UnboundedRelFastSetBlock(int a_RelX, int a_RelY, int a_RelZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta)
{
	if ((a_RelY < 0) || (a_RelY > cChunkDef::Height))
	{
		LOGWARNING("UnboundedRelFastSetBlock(): requesting a block with a_RelY out of range: %d", a_RelY);
		return false;
	}
	cChunk * Chunk = GetRelNeighborChunkAdjustCoords(a_RelX, a_RelZ);
	if ((Chunk == NULL) || !Chunk->IsValid())
	{
		// The chunk is not available, bail out
		return false;
	}
	Chunk->FastSetBlock(a_RelX, a_RelY, a_RelZ, a_BlockType, a_BlockMeta);
	return true;
}





void cChunk::UnboundedQueueTickBlock(int a_RelX, int a_RelY, int a_RelZ)
{
	if ((a_RelY < 0) || (a_RelY >= cChunkDef::Height))
	{
		// Outside of chunkmap
		return;
	}
	cChunk * Chunk = GetRelNeighborChunkAdjustCoords(a_RelX, a_RelZ);
	if ((Chunk != NULL) && Chunk->IsValid())
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
					{
						if (!HasBlockEntityAt(x + m_PosX * Width, y + m_PosY * Height, z + m_PosZ * Width))
						{
							m_BlockEntities.push_back(cBlockEntity::CreateByBlockType(
								BlockType, GetMeta(x, y, z),
								x + m_PosX * Width, y + m_PosY * Height, z + m_PosZ * Width, m_World
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
					m_HeightMap[x + z * Width] = (HEIGHTTYPE)y;
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
	if (BlockEntity != NULL)
	{
		BlockEntity->Destroy();
		RemoveBlockEntity(BlockEntity);
		delete BlockEntity;
		BlockEntity = NULL;
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
		{
			AddBlockEntity(cBlockEntity::CreateByBlockType(a_BlockType, a_BlockMeta, WorldPos.x, WorldPos.y, WorldPos.z, m_World));
			break;
		}
	}  // switch (a_BlockType)
}





void cChunk::QueueSetBlock(int a_RelX, int a_RelY, int a_RelZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, Int64 a_Tick, BLOCKTYPE a_PreviousBlockType)
{
	m_SetBlockQueue.push_back(sSetBlockQueueItem(a_RelX, a_RelY, a_RelZ, a_BlockType, a_BlockMeta, a_Tick, a_PreviousBlockType));
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

	MarkDirty();
	m_IsRedstoneDirty = true;

	m_ChunkData.SetBlock(a_RelX, a_RelY, a_RelZ, a_BlockType);

	// Queue block to be sent only if ...
	if (
		a_SendToClients &&                  // ... we are told to do so AND ...
		(
			(OldBlockMeta != a_BlockMeta) ||  // ... the meta value is different OR ...
			!(                                // ... the old and new blocktypes AREN'T liquids (because client doesn't need to distinguish betwixt them):
				((OldBlockType == E_BLOCK_STATIONARY_WATER) && (a_BlockType == E_BLOCK_WATER)) ||             // Replacing stationary water with water
				((OldBlockType == E_BLOCK_WATER)            && (a_BlockType == E_BLOCK_STATIONARY_WATER)) ||  // Replacing water with stationary water
				((OldBlockType == E_BLOCK_STATIONARY_LAVA)  && (a_BlockType == E_BLOCK_LAVA)) ||              // Replacing stationary water with water
				((OldBlockType == E_BLOCK_LAVA)             && (a_BlockType == E_BLOCK_STATIONARY_LAVA))      // Replacing water with stationary water
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
			m_HeightMap[a_RelX + a_RelZ * Width] = (HEIGHTTYPE)a_RelY;
		}
		else
		{
			for (int y = a_RelY - 1; y > 0; --y)
			{
				if (GetBlock(a_RelX, y, a_RelZ) != E_BLOCK_AIR)
				{
					m_HeightMap[a_RelX + a_RelZ * Width] = (HEIGHTTYPE)y;
					break;
				}
			}  // for y - column in m_BlockData
		}
	}
}





void cChunk::SendBlockTo(int a_RelX, int a_RelY, int a_RelZ, cClientHandle * a_Client)
{

	if (a_Client == NULL)
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
	
	return NULL;
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





void cChunk::UseBlockEntity(cPlayer * a_Player, int a_X, int a_Y, int a_Z)
{
	cBlockEntity * be = GetBlockEntity(a_X, a_Y, a_Z);
	if (be != NULL)
	{
		be->UsedBy(a_Player);
	}
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
	for (cClientHandleList::iterator itr = m_LoadedByClient.begin(); itr != m_LoadedByClient.end(); ++itr)
	{
		m_World->ForceSendChunkTo(m_PosX, m_PosZ, (*itr));
	}  // for itr - m_LoadedByClient[]
}





void cChunk::CollectPickupsByPlayer(cPlayer * a_Player)
{
	double PosX = a_Player->GetPosX();
	double PosY = a_Player->GetPosY();
	double PosZ = a_Player->GetPosZ();
	
	for (cEntityList::iterator itr = m_Entities.begin(); itr != m_Entities.end(); ++itr)
	{
		if ((!(*itr)->IsPickup()) && (!(*itr)->IsProjectile()))
		{
			continue;  // Only pickups and projectiles can be picked up
		}
		float DiffX = (float)((*itr)->GetPosX() - PosX);
		float DiffY = (float)((*itr)->GetPosY() - PosY);
		float DiffZ = (float)((*itr)->GetPosZ() - PosZ);
		float SqrDist = DiffX * DiffX + DiffY * DiffY + DiffZ * DiffZ;
		if (SqrDist < 1.5f * 1.5f)  // 1.5 block
		{
			/*
			LOG("Pickup %d being collected by player \"%s\", distance %f",
				(*itr)->GetUniqueID(), a_Player->GetName().c_str(), SqrDist
			);
			*/
			MarkDirty();
			if ((*itr)->IsPickup())
			{
				(reinterpret_cast<cPickup *>(*itr))->CollectedBy(a_Player);
			}
			else
			{
				(reinterpret_cast<cProjectileEntity *>(*itr))->CollectedBy(a_Player);
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
			(reinterpret_cast<cSignEntity *>(*itr))->SetLines(a_Line1, a_Line2, a_Line3, a_Line4);
			m_World->BroadcastBlockEntity(a_PosX, a_PosY, a_PosZ);
			return true;
		}
	}  // for itr - m_BlockEntities[]
	return false;
}





void cChunk::RemoveBlockEntity( cBlockEntity* a_BlockEntity)
{
	MarkDirty();
	m_BlockEntities.remove(a_BlockEntity);
}





bool cChunk::AddClient(cClientHandle* a_Client)
{
	for (cClientHandleList::iterator itr = m_LoadedByClient.begin(); itr != m_LoadedByClient.end(); ++itr)
	{
		if (a_Client == *itr)
		{
			// Already there, nothing needed
			return false;
		}
	}
	m_LoadedByClient.push_back( a_Client);

	for (cEntityList::iterator itr = m_Entities.begin(); itr != m_Entities.end(); ++itr)
	{
		/*
		// DEBUG:
		LOGD("cChunk: Entity #%d (%s) at [%i, %i, %i] spawning for player \"%s\"",
			(*itr)->GetUniqueID(), (*itr)->GetClass(),
			m_PosX, m_PosY, m_PosZ,
			a_Client->GetUsername().c_str()
		);
		*/
		(*itr)->SpawnOn(*a_Client);
	}
	return true;
}





void cChunk::RemoveClient( cClientHandle* a_Client)
{
	for (cClientHandleList::iterator itr = m_LoadedByClient.begin(); itr != m_LoadedByClient.end(); ++itr)
	{
		if (*itr != a_Client)
		{
			continue;
		}
		
		m_LoadedByClient.erase(itr);

		if (!a_Client->IsDestroyed())
		{
			for (cEntityList::iterator itr = m_Entities.begin(); itr != m_Entities.end(); ++itr)
			{
				/*
				// DEBUG:
				LOGD("chunk [%i, %i] destroying entity #%i for player \"%s\"",
					m_PosX, m_PosZ,
					(*itr)->GetUniqueID(), a_Client->GetUsername().c_str()
				);
				*/
				a_Client->SendDestroyEntity(*(*itr));
			}
		}
		return;
	}  // for itr - m_LoadedByClient[]
}





bool cChunk::HasClient( cClientHandle* a_Client)
{
	for (cClientHandleList::const_iterator itr = m_LoadedByClient.begin(); itr != m_LoadedByClient.end(); ++itr)
	{
		if ((*itr) == a_Client)
		{
			return true;
		}
	}
	return false;
}





bool cChunk::HasAnyClients(void) const
{
	return !m_LoadedByClient.empty();
}





void cChunk::AddEntity(cEntity * a_Entity)
{
	if (!a_Entity->IsPlayer())
	{
		MarkDirty();
	}
	
	ASSERT(std::find(m_Entities.begin(), m_Entities.end(), a_Entity) == m_Entities.end());  // Not there already
	
	m_Entities.push_back(a_Entity);
}





void cChunk::RemoveEntity(cEntity * a_Entity)
{
	size_t SizeBefore = m_Entities.size();
	m_Entities.remove(a_Entity);
	size_t SizeAfter = m_Entities.size();
	
	if (SizeBefore != SizeAfter)
	{
		// Mark as dirty if it was a server-generated entity:
		if (!a_Entity->IsPlayer())
		{
			MarkDirty();
		}
	}
}





bool cChunk::HasEntity(int a_EntityID)
{
	for (cEntityList::const_iterator itr = m_Entities.begin(), end = m_Entities.end(); itr != end; ++itr)
	{
		if ((*itr)->GetUniqueID() == a_EntityID)
		{
			return true;
		}
	}  // for itr - m_Entities[]
	return false;
}





bool cChunk::ForEachEntity(cEntityCallback & a_Callback)
{
	// The entity list is locked by the parent chunkmap's CS
	for (cEntityList::iterator itr = m_Entities.begin(), itr2 = itr; itr != m_Entities.end(); itr = itr2)
	{
		++itr2;
		if (a_Callback.Item(*itr))
		{
			return false;
		}
	}  // for itr - m_Entitites[]
	return true;
}





bool cChunk::DoWithEntityByID(int a_EntityID, cEntityCallback & a_Callback, bool & a_CallbackResult)
{
	// The entity list is locked by the parent chunkmap's CS
	for (cEntityList::iterator itr = m_Entities.begin(), end = m_Entities.end(); itr != end; ++itr)
	{
		if ((*itr)->GetUniqueID() == a_EntityID)
		{
			a_CallbackResult = a_Callback.Item(*itr);
			return true;
		}
	}  // for itr - m_Entitites[]
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
		if (a_Callback.Item((cChestEntity *)*itr))
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
		if (a_Callback.Item((cDispenserEntity *)*itr))
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
		if (a_Callback.Item((cDropperEntity *)*itr))
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
		if (a_Callback.Item((cDropSpenserEntity *)*itr))
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
		if (a_Callback.Item((cFurnaceEntity *)*itr))
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
		if (a_Callback.Item((cChestEntity *)*itr))
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
		if (a_Callback.Item((cDispenserEntity *)*itr))
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
		if (a_Callback.Item((cDropperEntity *)*itr))
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
		if (a_Callback.Item((cDropSpenserEntity *)*itr))
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
		if (a_Callback.Item((cFurnaceEntity *)*itr))
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
		if (a_Callback.Item((cNoteEntity *)*itr))
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
		if (a_Callback.Item((cCommandBlockEntity *)*itr))
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
		if (a_Callback.Item((cMobHeadEntity *)*itr))
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
		if (a_Callback.Item((cFlowerPotEntity *)*itr))
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
				a_Line1 = ((cSignEntity *)*itr)->GetLine(0);
				a_Line2 = ((cSignEntity *)*itr)->GetLine(1);
				a_Line3 = ((cSignEntity *)*itr)->GetLine(2);
				a_Line4 = ((cSignEntity *)*itr)->GetLine(3);
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
	if (
		(a_RelX < 0) || (a_RelX >= Width) ||
		(a_RelY < 0) || (a_RelY >= Height) ||
		(a_RelZ < 0) || (a_RelZ >= Width)
	)
	{
		ASSERT(!"GetBlock(x, y, z) out of bounds!");
		return 0;  // Clip
	}

	return m_ChunkData.GetBlock(a_RelX, a_RelY, a_RelZ);
}





void cChunk::GetBlockTypeMeta(int a_RelX, int a_RelY, int a_RelZ, BLOCKTYPE & a_BlockType, NIBBLETYPE & a_BlockMeta)
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
		return m_ChunkMap->GetChunkNoLoad(ChunkX, ZERO_CHUNK_Y, ChunkZ);
	}

	// Walk the neighbors:
	bool ReturnThis = true;
	if (a_RelX < 0)
	{
		if (m_NeighborXM != NULL)
		{
			cChunk * Candidate = m_NeighborXM->GetRelNeighborChunk(a_RelX + cChunkDef::Width, a_RelZ);
			if (Candidate != NULL)
			{
				return Candidate;
			}
		}
		// Going X first failed, but if the request is crossing Z as well, let's try the Z first later on.
		ReturnThis = false;
	}
	else if (a_RelX >= cChunkDef::Width)
	{
		if (m_NeighborXP != NULL)
		{
			cChunk * Candidate = m_NeighborXP->GetRelNeighborChunk(a_RelX - cChunkDef::Width, a_RelZ);
			if (Candidate != NULL)
			{
				return Candidate;
			}
		}
		// Going X first failed, but if the request is crossing Z as well, let's try the Z first later on.
		ReturnThis = false;
	}
	
	if (a_RelZ < 0)
	{
		if (m_NeighborZM != NULL)
		{
			return m_NeighborZM->GetRelNeighborChunk(a_RelX, a_RelZ + cChunkDef::Width);
			// For requests crossing both X and Z, the X-first way has been already tried
		}
		return NULL;
	}
	else if (a_RelZ >= cChunkDef::Width)
	{
		if (m_NeighborZP != NULL)
		{
			return m_NeighborZP->GetRelNeighborChunk(a_RelX, a_RelZ - cChunkDef::Width);
			// For requests crossing both X and Z, the X-first way has been already tried
		}
		return NULL;
	}
	
	return (ReturnThis ? this : NULL);
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
	while ((RelX >= Width) && (ToReturn != NULL))
	{
		RelX -= Width;
		ToReturn = ToReturn->m_NeighborXP;
	}
	while ((RelX < 0) && (ToReturn != NULL))
	{
		RelX += Width;
		ToReturn = ToReturn->m_NeighborXM;
	}
	while ((RelZ >= Width) && (ToReturn != NULL))
	{
		RelZ -= Width;
		ToReturn = ToReturn->m_NeighborZP;
	}
	while ((RelZ < 0) && (ToReturn != NULL))
	{
		RelZ += Width;
		ToReturn = ToReturn->m_NeighborZM;
	}
	if (ToReturn != NULL)
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
	for (cClientHandleList::const_iterator itr = m_LoadedByClient.begin(); itr != m_LoadedByClient.end(); ++itr)
	{
		(*itr)->SendAttachEntity(a_Entity, a_Vehicle);
	}  // for itr - LoadedByClient[]
}





void cChunk::BroadcastBlockAction(int a_BlockX, int a_BlockY, int a_BlockZ, char a_Byte1, char a_Byte2, BLOCKTYPE a_BlockType, const cClientHandle * a_Exclude)
{
	for (cClientHandleList::const_iterator itr = m_LoadedByClient.begin(); itr != m_LoadedByClient.end(); ++itr)
	{
		if (*itr == a_Exclude)
		{
			continue;
		}
		(*itr)->SendBlockAction(a_BlockX, a_BlockY, a_BlockZ, a_Byte1, a_Byte2, a_BlockType);
	}  // for itr - LoadedByClient[]
}





void cChunk::BroadcastBlockBreakAnimation(int a_entityID, int a_blockX, int a_blockY, int a_blockZ, char a_stage, const cClientHandle * a_Exclude)
{
	for (cClientHandleList::iterator itr = m_LoadedByClient.begin(); itr != m_LoadedByClient.end(); ++itr)
	{
		if (*itr == a_Exclude)
		{
			continue;
		}
		(*itr)->SendBlockBreakAnim(a_entityID, a_blockX, a_blockY, a_blockZ, a_stage);
	}  // for itr - LoadedByClient[]
}





void cChunk::BroadcastBlockEntity(int a_BlockX, int a_BlockY, int a_BlockZ, const cClientHandle * a_Exclude)
{
	// We can operate on entity pointers, we're inside the ChunkMap's CS lock which guards the list
	cBlockEntity * Entity = GetBlockEntity(a_BlockX, a_BlockY, a_BlockZ);
	if (Entity == NULL)
	{
		return;
	}
	for (cClientHandleList::iterator itr = m_LoadedByClient.begin(); itr != m_LoadedByClient.end(); ++itr)
	{
		if (*itr == a_Exclude)
		{
			continue;
		}
		Entity->SendTo(*(*itr));
	}  // for itr - LoadedByClient[]
}





void cChunk::BroadcastChunkData(cChunkDataSerializer & a_Serializer, const cClientHandle * a_Exclude)
{
	for (cClientHandleList::iterator itr = m_LoadedByClient.begin(); itr != m_LoadedByClient.end(); ++itr)
	{
		if (*itr == a_Exclude)
		{
			continue;
		}
		(*itr)->SendChunkData(m_PosX, m_PosZ, a_Serializer);
	}  // for itr - LoadedByClient[]
}





void cChunk::BroadcastCollectEntity(const cEntity & a_Entity, const cPlayer & a_Player, const cClientHandle * a_Exclude)
{
	for (cClientHandleList::iterator itr = m_LoadedByClient.begin(); itr != m_LoadedByClient.end(); ++itr)
	{
		if (*itr == a_Exclude)
		{
			continue;
		}
		(*itr)->SendCollectEntity(a_Entity, a_Player);
	}  // for itr - LoadedByClient[]
}





void cChunk::BroadcastDestroyEntity(const cEntity & a_Entity, const cClientHandle * a_Exclude)
{
	for (cClientHandleList::const_iterator itr = m_LoadedByClient.begin(); itr != m_LoadedByClient.end(); ++itr)
	{
		if (*itr == a_Exclude)
		{
			continue;
		}
		(*itr)->SendDestroyEntity(a_Entity);
	}  // for itr - LoadedByClient[]
}





void cChunk::BroadcastEntityEffect(const cEntity & a_Entity, int a_EffectID, int a_Amplifier, short a_Duration, const cClientHandle * a_Exclude)
{
	for (cClientHandleList::const_iterator itr = m_LoadedByClient.begin(); itr != m_LoadedByClient.end(); ++itr)
	{
		if (*itr == a_Exclude)
		{
			continue;
		}
		(*itr)->SendEntityEffect(a_Entity, a_EffectID, a_Amplifier, a_Duration);
	}  // for itr - LoadedByClient[]
}





void cChunk::BroadcastEntityEquipment(const cEntity & a_Entity, short a_SlotNum, const cItem & a_Item, const cClientHandle * a_Exclude)
{
	for (cClientHandleList::const_iterator itr = m_LoadedByClient.begin(); itr != m_LoadedByClient.end(); ++itr)
	{
		if (*itr == a_Exclude)
		{
			continue;
		}
		(*itr)->SendEntityEquipment(a_Entity, a_SlotNum, a_Item);
	}  // for itr - LoadedByClient[]
}





void cChunk::BroadcastEntityHeadLook(const cEntity & a_Entity, const cClientHandle * a_Exclude)
{
	for (cClientHandleList::const_iterator itr = m_LoadedByClient.begin(); itr != m_LoadedByClient.end(); ++itr)
	{
		if (*itr == a_Exclude)
		{
			continue;
		}
		(*itr)->SendEntityHeadLook(a_Entity);
	}  // for itr - LoadedByClient[]
}





void cChunk::BroadcastEntityLook(const cEntity & a_Entity, const cClientHandle * a_Exclude)
{
	for (cClientHandleList::const_iterator itr = m_LoadedByClient.begin(); itr != m_LoadedByClient.end(); ++itr)
	{
		if (*itr == a_Exclude)
		{
			continue;
		}
		(*itr)->SendEntityLook(a_Entity);
	}  // for itr - LoadedByClient[]
}





void cChunk::BroadcastEntityMetadata(const cEntity & a_Entity, const cClientHandle * a_Exclude)
{
	for (cClientHandleList::const_iterator itr = m_LoadedByClient.begin(); itr != m_LoadedByClient.end(); ++itr)
	{
		if (*itr == a_Exclude)
		{
			continue;
		}
		(*itr)->SendEntityMetadata(a_Entity);
	}  // for itr - LoadedByClient[]
}





void cChunk::BroadcastEntityRelMove(const cEntity & a_Entity, char a_RelX, char a_RelY, char a_RelZ, const cClientHandle * a_Exclude)
{
	for (cClientHandleList::const_iterator itr = m_LoadedByClient.begin(); itr != m_LoadedByClient.end(); ++itr)
	{
		if (*itr == a_Exclude)
		{
			continue;
		}
		(*itr)->SendEntityRelMove(a_Entity, a_RelX, a_RelY, a_RelZ);
	}  // for itr - LoadedByClient[]
}





void cChunk::BroadcastEntityRelMoveLook(const cEntity & a_Entity, char a_RelX, char a_RelY, char a_RelZ, const cClientHandle * a_Exclude)
{
	for (cClientHandleList::const_iterator itr = m_LoadedByClient.begin(); itr != m_LoadedByClient.end(); ++itr)
	{
		if (*itr == a_Exclude)
		{
			continue;
		}
		(*itr)->SendEntityRelMoveLook(a_Entity, a_RelX, a_RelY, a_RelZ);
	}  // for itr - LoadedByClient[]
}





void cChunk::BroadcastEntityStatus(const cEntity & a_Entity, char a_Status, const cClientHandle * a_Exclude)
{
	for (cClientHandleList::const_iterator itr = m_LoadedByClient.begin(); itr != m_LoadedByClient.end(); ++itr)
	{
		if (*itr == a_Exclude)
		{
			continue;
		}
		(*itr)->SendEntityStatus(a_Entity, a_Status);
	}  // for itr - LoadedByClient[]
}





void cChunk::BroadcastEntityVelocity(const cEntity & a_Entity, const cClientHandle * a_Exclude)
{
	for (cClientHandleList::const_iterator itr = m_LoadedByClient.begin(); itr != m_LoadedByClient.end(); ++itr)
	{
		if (*itr == a_Exclude)
		{
			continue;
		}
		(*itr)->SendEntityVelocity(a_Entity);
	}  // for itr - LoadedByClient[]
}





void cChunk::BroadcastEntityAnimation(const cEntity & a_Entity, char a_Animation, const cClientHandle * a_Exclude)
{
	for (cClientHandleList::const_iterator itr = m_LoadedByClient.begin(); itr != m_LoadedByClient.end(); ++itr)
	{
		if (*itr == a_Exclude)
		{
			continue;
		}
		(*itr)->SendEntityAnimation(a_Entity, a_Animation);
	}  // for itr - LoadedByClient[]
}





void cChunk::BroadcastParticleEffect(const AString & a_ParticleName, float a_SrcX, float a_SrcY, float a_SrcZ, float a_OffsetX, float a_OffsetY, float a_OffsetZ, float a_ParticleData, int a_ParticleAmmount, cClientHandle * a_Exclude)
{
	for (cClientHandleList::iterator itr = m_LoadedByClient.begin(); itr != m_LoadedByClient.end(); ++itr)
	{
		if (*itr == a_Exclude)
		{
			continue;
		}
		(*itr)->SendParticleEffect(a_ParticleName, a_SrcX, a_SrcY, a_SrcZ, a_OffsetX, a_OffsetY, a_OffsetZ, a_ParticleData, a_ParticleAmmount);
	}  // for itr - LoadedByClient[]
}





void cChunk::BroadcastRemoveEntityEffect(const cEntity & a_Entity, int a_EffectID, const cClientHandle * a_Exclude)
{
	for (cClientHandleList::iterator itr = m_LoadedByClient.begin(); itr != m_LoadedByClient.end(); ++itr)
	{
		if (*itr == a_Exclude)
		{
			continue;
		}
		(*itr)->SendRemoveEntityEffect(a_Entity, a_EffectID);
	}  // for itr - LoadedByClient[]
}





void cChunk::BroadcastSoundEffect(const AString & a_SoundName, double a_X, double a_Y, double a_Z, float a_Volume, float a_Pitch, const cClientHandle * a_Exclude)
{
	for (cClientHandleList::iterator itr = m_LoadedByClient.begin(); itr != m_LoadedByClient.end(); ++itr)
	{
		if (*itr == a_Exclude)
		{
			continue;
		}
		(*itr)->SendSoundEffect(a_SoundName, a_X, a_Y, a_Z, a_Volume, a_Pitch);
	}  // for itr - LoadedByClient[]
}





void cChunk::BroadcastSoundParticleEffect(int a_EffectID, int a_SrcX, int a_SrcY, int a_SrcZ, int a_Data, const cClientHandle * a_Exclude)
{
	for (cClientHandleList::iterator itr = m_LoadedByClient.begin(); itr != m_LoadedByClient.end(); ++itr)
	{
		if (*itr == a_Exclude)
		{
			continue;
		}
		(*itr)->SendSoundParticleEffect(a_EffectID, a_SrcX, a_SrcY, a_SrcZ, a_Data);
	}  // for itr - LoadedByClient[]
}





void cChunk::BroadcastSpawnEntity(cEntity & a_Entity, const cClientHandle * a_Exclude)
{
	for (cClientHandleList::iterator itr = m_LoadedByClient.begin(); itr != m_LoadedByClient.end(); ++itr)
	{
		if (*itr == a_Exclude)
		{
			continue;
		}
		a_Entity.SpawnOn(*(*itr));
	}  // for itr - LoadedByClient[]
}





void cChunk::BroadcastThunderbolt(int a_BlockX, int a_BlockY, int a_BlockZ, const cClientHandle * a_Exclude)
{
	for (cClientHandleList::iterator itr = m_LoadedByClient.begin(); itr != m_LoadedByClient.end(); ++itr)
	{
		if (*itr == a_Exclude)
		{
			continue;
		}
		(*itr)->SendThunderbolt(a_BlockX, a_BlockY, a_BlockZ);
	}  // for itr - LoadedByClient[]
}





void cChunk::BroadcastUseBed(const cEntity & a_Entity, int a_BlockX, int a_BlockY, int a_BlockZ)
{
	for (cClientHandleList::iterator itr = m_LoadedByClient.begin(); itr != m_LoadedByClient.end(); ++itr)
	{
		(*itr)->SendUseBed(a_Entity, a_BlockX, a_BlockY, a_BlockZ);
	}  // for itr - LoadedByClient[]
}





void cChunk::SendBlockEntity(int a_BlockX, int a_BlockY, int a_BlockZ, cClientHandle & a_Client)
{
	cBlockEntity * Entity = GetBlockEntity(a_BlockX, a_BlockY, a_BlockZ);
	if (Entity == NULL)
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
	return Vector3i(m_PosX * Width + a_RelX, m_PosY * Height + a_RelY, m_PosZ * Width + a_RelZ);
}





NIBBLETYPE cChunk::GetTimeAlteredLight(NIBBLETYPE a_Skylight) const
{
	a_Skylight -= m_World->GetSkyDarkness();
	// Because NIBBLETYPE is unsigned, we clamp it to 0 .. 15 by checking for values above 15
	return (a_Skylight < 16)? a_Skylight : 0;
}








