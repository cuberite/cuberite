
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#ifndef _WIN32
	#include <cstdlib>
#endif


#include "Chunk.h"
#include "World.h"
#include "ClientHandle.h"
#include "Server.h"
#include "zlib.h"
#include "Defines.h"
#include "BlockEntities/ChestEntity.h"
#include "BlockEntities/DispenserEntity.h"
#include "BlockEntities/DropperEntity.h"
#include "BlockEntities/FurnaceEntity.h"
#include "BlockEntities/HopperEntity.h"
#include "BlockEntities/JukeboxEntity.h"
#include "BlockEntities/NoteEntity.h"
#include "BlockEntities/SignEntity.h"
#include "Entities/Pickup.h"
#include "Item.h"
#include "Noise.h"
#include "Root.h"
#include "MersenneTwister.h"
#include "Entities/Player.h"
#include "BlockArea.h"
#include "PluginManager.h"
#include "Blocks/BlockHandler.h"
#include "Simulator/FluidSimulator.h"
#include "MobCensus.h"
#include "MobSpawner.h"


#include <json/json.h>





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// sSetBlock:

sSetBlock::sSetBlock( int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta )  // absolute block position
	: x( a_BlockX )
	, y( a_BlockY )
	, z( a_BlockZ )
	, BlockType( a_BlockType )
	, BlockMeta( a_BlockMeta )
{
	cChunkDef::AbsoluteToRelative(x, y, z, ChunkX, ChunkZ);
}





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cChunk:

cChunk::cChunk(
	int a_ChunkX, int a_ChunkY, int a_ChunkZ, 
	cChunkMap * a_ChunkMap, cWorld * a_World,
	cChunk * a_NeighborXM, cChunk * a_NeighborXP, cChunk * a_NeighborZM, cChunk * a_NeighborZP
)
	: m_PosX( a_ChunkX )
	, m_PosY( a_ChunkY )
	, m_PosZ( a_ChunkZ )
	, m_BlockTickX( 0 )
	, m_BlockTickY( 0 )
	, m_BlockTickZ( 0 )
	, m_World( a_World )
	, m_ChunkMap(a_ChunkMap)
	, m_IsValid(false)
	, m_IsLightValid(false)
	, m_IsDirty(false)
	, m_IsSaving(false)
	, m_StayCount(0)
	, m_NeighborXM(a_NeighborXM)
	, m_NeighborXP(a_NeighborXP)
	, m_NeighborZM(a_NeighborZM)
	, m_NeighborZP(a_NeighborZP)
	, m_WaterSimulatorData(a_World->GetWaterSimulator()->CreateChunkData())
	, m_LavaSimulatorData (a_World->GetLavaSimulator ()->CreateChunkData())
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
	
	// LOGINFO("### delete cChunk() (%i, %i) from %p, thread 0x%x ###", m_PosX, m_PosZ, this, GetCurrentThreadId() );
	
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
	delete m_LavaSimulatorData;
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
	a_Callback.HeightMap    (&m_HeightMap);
	a_Callback.BiomeData    (&m_BiomeMap);
	a_Callback.BlockTypes   (m_BlockTypes);
	a_Callback.BlockMeta    (m_BlockMeta);
	a_Callback.LightIsValid (m_IsLightValid);
	a_Callback.BlockLight   (m_BlockLight);
	a_Callback.BlockSkyLight(m_BlockSkyLight);
	
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
	
	memcpy(m_BlockTypes, a_BlockTypes, sizeof(m_BlockTypes));
	memcpy(m_BlockMeta,  a_BlockMeta,  sizeof(m_BlockMeta));
	if (a_BlockLight != NULL)
	{
		memcpy(m_BlockLight, a_BlockLight, sizeof(m_BlockLight));
	}
	if (a_BlockSkyLight != NULL)
	{
		memcpy(m_BlockSkyLight, a_BlockSkyLight, sizeof(m_BlockSkyLight));
	}
	
	m_IsLightValid = (a_BlockLight != NULL) && (a_BlockSkyLight != NULL);
	
	if (a_HeightMap == NULL)
	{
		CalculateHeightmap();
	}

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
	memcpy(m_BlockLight,    a_BlockLight, sizeof(m_BlockLight));
	memcpy(m_BlockSkyLight, a_SkyLight,   sizeof(m_BlockSkyLight));
	m_IsLightValid = true;
}





void cChunk::GetBlockTypes(BLOCKTYPE * a_BlockTypes)
{
	memcpy(a_BlockTypes, m_BlockTypes, NumBlocks);
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





/// Sets or resets the internal flag that prevents chunk from being unloaded
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
		//LOGD("Counting entity #%i (%s)", (*itr)->GetUniqueID(), (*itr)->GetClass());
		if ((*itr)->IsMob())
		{
			cMonster& Monster = (cMonster&)(**itr);
			currentPosition = Monster.GetPosition();
			for (std::list<const Vector3d*>::const_iterator itr2 = playerPositions.begin(); itr2 != playerPositions.end(); itr2 ++)
			{
				toFill.CollectMob(Monster,*this,(currentPosition-**itr2).SqrLength());
			}
		}
	}  // for itr - m_Entitites[]
}




void cChunk::getThreeRandomNumber(int& a_X, int& a_Y, int& a_Z,int a_MaxX, int a_MaxY, int a_MaxZ)
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





void cChunk::getRandomBlockCoords(int& a_X, int& a_Y, int& a_Z)
{
	// MG TODO : check if this kind of optimization (only one random call) is still needed
	// MG TODO : if so propagate it

	getThreeRandomNumber(a_X, a_Y, a_Z, Width, Height-2, Width);
	a_Y++;
}





void cChunk::SpawnMobs(cMobSpawner& a_MobSpawner)
{
	int Center_X,Center_Y,Center_Z;
	getRandomBlockCoords(Center_X,Center_Y,Center_Z);

	BLOCKTYPE PackCenterBlock = GetBlock(Center_X, Center_Y, Center_Z);
	if (a_MobSpawner.CheckPackCenter(PackCenterBlock))
	{
		a_MobSpawner.NewPack();
		int NumberOfTries = 0;
		int NumberOfSuccess = 0;
		int MaxNbOfSuccess = 4; // this can be changed during the process for Wolves and Ghass
		while (NumberOfTries < 12 && NumberOfSuccess < MaxNbOfSuccess)
		{
			const int HorizontalRange = 20; // MG TODO : relocate
			const int VerticalRange = 0; // MG TODO : relocate
			int Try_X, Try_Y, Try_Z;
			getThreeRandomNumber(Try_X, Try_Y, Try_Z, 2*HorizontalRange+1 , 2*VerticalRange+1 , 2*HorizontalRange+1);
			Try_X -= HorizontalRange;
			Try_Y -= VerticalRange;
			Try_Z -= HorizontalRange;
			Try_X += Center_X;
			Try_Y += Center_Y;
			Try_Z += Center_Z;

			ASSERT(Try_Y > 0);
			ASSERT(Try_Y < cChunkDef::Height-1);
			
			BLOCKTYPE BlockType;
			NIBBLETYPE BlockMeta;
			BLOCKTYPE BlockType_below;
			NIBBLETYPE BlockMeta_below;
			BLOCKTYPE BlockType_above;
			NIBBLETYPE BlockMeta_above;
			if (UnboundedRelGetBlock(Try_X, Try_Y  , Try_Z, BlockType, BlockMeta) &&
				UnboundedRelGetBlock(Try_X, Try_Y-1, Try_Z, BlockType_below, BlockMeta_below)&&
				UnboundedRelGetBlock(Try_X, Try_Y+1, Try_Z, BlockType_above, BlockMeta_above)
				)
			{
				EMCSBiome Biome = m_ChunkMap->GetBiomeAt (Try_X, Try_Z);
				// MG TODO :
				// Moon cycle (for slime)
				// check player and playerspawn presence < 24 blocks
				// check mobs presence on the block

				// MG TODO : check that "Level" really means Y
				
				NIBBLETYPE SkyLight = 0;

				NIBBLETYPE BlockLight = 0;

				if (IsLightValid() && (UnboundedRelGetBlockBlockLight(Try_X, Try_Y, Try_Z, BlockLight)) && (UnboundedRelGetBlockSkyLight(Try_X, Try_Y, Try_Z, SkyLight)))
				{
					cEntity* newMob = a_MobSpawner.TryToSpawnHere(BlockType, BlockMeta, BlockType_below, BlockMeta_below, BlockType_above, BlockMeta_above, SkyLight, BlockLight, Biome, Try_Y, MaxNbOfSuccess);
					if (newMob)
					{
						int WorldX, WorldY, WorldZ;
						PositionToWorldPosition(Try_X, Try_Y, Try_Z, WorldX, WorldY, WorldZ);
						newMob->SetPosition(WorldX, WorldY, WorldZ);
						LOGD("Spawning %s #%i at %d,%d,%d",newMob->GetClass(),newMob->GetUniqueID(),WorldX, WorldY, WorldZ);
						NumberOfSuccess++;
					}
				}
			}
			
			NumberOfTries++;
		}
	}

}





void cChunk::Tick(float a_Dt)
{
	BroadcastPendingBlockChanges();

	// Unload the chunk from all clients that have queued unloading:
	for (cClientHandleList::iterator itr = m_UnloadQuery.begin(), end = m_UnloadQuery.end(); itr != end; ++itr)
	{
		(*itr)->SendUnloadChunk(m_PosX, m_PosZ);
	}
	m_UnloadQuery.clear();
	
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
				continue;
			}
			itr++;
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
		if (itr->m_Tick < CurrTick)
		{
			// Not yet
			++itr;
			continue;
		}
		else
		{
			// Now is the time to set the block
			SetBlock(itr->m_RelX, itr->m_RelY, itr->m_RelZ, itr->m_BlockType, itr->m_BlockMeta);
			itr = m_SetBlockQueue.erase(itr);
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





void cChunk::CheckBlocks(void)
{
	if (m_ToTickBlocks.size() == 0)
	{
		return;
	}
	std::vector<unsigned int> ToTickBlocks;
	std::swap(m_ToTickBlocks, ToTickBlocks);
	
	for (std::vector<unsigned int>::const_iterator itr = ToTickBlocks.begin(), end = ToTickBlocks.end(); itr != end; ++itr)
	{
		unsigned int index = (*itr);
		Vector3i BlockPos = IndexToCoordinate(index);

		cBlockHandler * Handler = BlockHandler(GetBlock(index));
		Handler->Check(BlockPos.x, BlockPos.y, BlockPos.z, *this);
	}  // for itr - ToTickBlocks[]
}





void cChunk::TickBlocks(void)
{
	// Tick dem blocks
	// _X: We must limit the random number or else we get a nasty int overflow bug ( http://forum.mc-server.org/showthread.php?tid=457 )
	int RandomX = m_World->GetTickRandomNumber(0x00ffffff);
	int RandomY = m_World->GetTickRandomNumber(0x00ffffff);
	int RandomZ = m_World->GetTickRandomNumber(0x00ffffff);
	int TickX = m_BlockTickX;
	int TickY = m_BlockTickY;
	int TickZ = m_BlockTickZ;

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
			continue; // It's all air up here
		}

		unsigned int Index = MakeIndexNoCheck(m_BlockTickX, m_BlockTickY, m_BlockTickZ);
		cBlockHandler * Handler = BlockHandler(m_BlockTypes[Index]);
		ASSERT(Handler != NULL);  // Happenned on server restart, FS #243
		Handler->OnUpdate(m_World, m_BlockTickX + m_PosX * Width, m_BlockTickY, m_BlockTickZ + m_PosZ * Width);
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
			else if (g_BlockIsSnowable[TopBlock])
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
				BLOCKTYPE BlockType = cChunkDef::GetBlock(m_BlockTypes, x, y, z);
				switch (BlockType)
				{
					case E_BLOCK_CHEST:
					{
						if (!HasBlockEntityAt(x + m_PosX * Width, y + m_PosY * Height, z + m_PosZ * Width))
						{
							m_BlockEntities.push_back(new cChestEntity(x + m_PosX * Width, y + m_PosY * Height, z + m_PosZ * Width, m_World));
						}
						break;
					}

					case E_BLOCK_DISPENSER:
					{
						if (!HasBlockEntityAt(x + m_PosX * Width, y + m_PosY * Height, z + m_PosZ * Width))
						{
							m_BlockEntities.push_back(new cDispenserEntity(x + m_PosX * Width, y + m_PosY * Height, z + m_PosZ * Width, m_World));
						}
						break;
					}
					
					case E_BLOCK_DROPPER:
					{
						if (!HasBlockEntityAt(x + m_PosX * Width, y + m_PosY * Height, z + m_PosZ * Width))
						{
							m_BlockEntities.push_back(new cDropperEntity(x + m_PosX * Width, y + m_PosY * Height, z + m_PosZ * Width, m_World));
						}
						break;
					}
					
					case E_BLOCK_LIT_FURNACE:
					case E_BLOCK_FURNACE:
					{
						if (!HasBlockEntityAt(x + m_PosX * Width, y + m_PosY * Height, z + m_PosZ * Width))
						{
							NIBBLETYPE BlockMeta = cChunkDef::GetNibble(m_BlockMeta, x, y, z);
							m_BlockEntities.push_back(new cFurnaceEntity(x + m_PosX * Width, y + m_PosY * Height, z + m_PosZ * Width, BlockType, BlockMeta, m_World));
						}
						break;
					}
					
					case E_BLOCK_HOPPER:
					{
						if (!HasBlockEntityAt(x + m_PosX * Width, y + m_PosY * Height, z + m_PosZ * Width))
						{
							m_BlockEntities.push_back(new cHopperEntity(x + m_PosX * Width, y + m_PosY * Height, z + m_PosZ * Width, m_World));
						}
					}
					
					case E_BLOCK_SIGN_POST:
					case E_BLOCK_WALLSIGN:
					{
						if (!HasBlockEntityAt(x + m_PosX * Width, y + m_PosY * Height, z + m_PosZ * Width))
						{
							m_BlockEntities.push_back( new cSignEntity(BlockType, x + m_PosX * Width, y + m_PosY * Height, z + m_PosZ * Width, m_World));
						}
						break;
					}

					case E_BLOCK_NOTE_BLOCK:
					{
						if (!HasBlockEntityAt(x + m_PosX * Width, y + m_PosY * Height, z + m_PosZ * Width))
						{
							m_BlockEntities.push_back(new cNoteEntity(x + m_PosX * Width, y + m_PosY * Height, z + m_PosZ * Width, m_World));
						}
						break;
					}

					case E_BLOCK_JUKEBOX:
					{
						if (!HasBlockEntityAt(x + m_PosX * Width, y + m_PosY * Height, z + m_PosZ * Width))
						{
							m_BlockEntities.push_back(new cJukeboxEntity(x + m_PosX * Width, y + m_PosY * Height, z + m_PosZ * Width, m_World));
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
				switch (cChunkDef::GetBlock(m_BlockTypes, x, y, z))
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
				}  // switch (BlockType)
			}  // for y
		}  // for z
	}  // for x
}





void cChunk::CalculateHeightmap()
{
	for (int x = 0; x < Width; x++)
	{
		for (int z = 0; z < Width; z++)
		{
			for (int y = Height - 1; y > -1; y--)
			{
				int index = MakeIndex( x, y, z );
				if (m_BlockTypes[index] != E_BLOCK_AIR)
				{
					m_HeightMap[x + z * Width] = (unsigned char)y;
					break;
				}
			}  // for y
		}  // for z
	}  // for x
}





void cChunk::SetBlock(int a_RelX, int a_RelY, int a_RelZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta)
{
	FastSetBlock(a_RelX, a_RelY, a_RelZ, a_BlockType, a_BlockMeta);

	const int index = MakeIndexNoCheck(a_RelX, a_RelY, a_RelZ);
	
	// Tick this block and its neighbors:
	m_ToTickBlocks.push_back(index);
	QueueTickBlockNeighbors(a_RelX, a_RelY, a_RelZ);

	// If there was a block entity, remove it:
	Vector3i WorldPos = PositionToWorldPosition(a_RelX, a_RelY, a_RelZ);
	cBlockEntity * BlockEntity = GetBlockEntity(WorldPos);
	if (BlockEntity != NULL)
	{
		BlockEntity->Destroy();
		RemoveBlockEntity(BlockEntity);
		delete BlockEntity;
	}
	
	// If the new block is a block entity, create the entity object:
	switch (a_BlockType)
	{
		case E_BLOCK_CHEST:
		{
			AddBlockEntity(new cChestEntity(WorldPos.x, WorldPos.y, WorldPos.z, m_World));
			break;
		}
		case E_BLOCK_DISPENSER:
		{
			AddBlockEntity(new cDispenserEntity(WorldPos.x, WorldPos.y, WorldPos.z, m_World));
			break;
		}
		case E_BLOCK_DROPPER:
		{
			AddBlockEntity(new cDropperEntity(WorldPos.x, WorldPos.y, WorldPos.z, m_World));
			break;
		}
		case E_BLOCK_LIT_FURNACE:
		case E_BLOCK_FURNACE:
		{
			AddBlockEntity(new cFurnaceEntity(WorldPos.x, WorldPos.y, WorldPos.z, a_BlockType, a_BlockMeta, m_World));
			break;
		}
		case E_BLOCK_HOPPER:
		{
			AddBlockEntity(new cHopperEntity(WorldPos.x, WorldPos.y, WorldPos.z, m_World));
			break;
		}
		case E_BLOCK_SIGN_POST:
		case E_BLOCK_WALLSIGN:
		{
			AddBlockEntity(new cSignEntity(a_BlockType, WorldPos.x, WorldPos.y, WorldPos.z, m_World));
			break;
		}
		case E_BLOCK_NOTE_BLOCK:
		{
			AddBlockEntity(new cNoteEntity(WorldPos.x, WorldPos.y, WorldPos.z, m_World));
			break;
		}
		case E_BLOCK_JUKEBOX:
		{
			AddBlockEntity(new cJukeboxEntity(WorldPos.x, WorldPos.y, WorldPos.z, m_World));
			break;
		}
	}  // switch (a_BlockType)
}





void cChunk::QueueSetBlock(int a_RelX, int a_RelY, int a_RelZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, Int64 a_Tick)
{
	m_SetBlockQueue.push_back(sSetBlockQueueItem(a_RelX, a_RelY, a_RelZ, a_BlockType, a_BlockMeta, a_Tick));
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
	
	m_ToTickBlocks.push_back(MakeIndexNoCheck(a_RelX, a_RelY, a_RelZ));
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
	for (int i = 0; i < ARRAYCOUNT(Coords); i++)
	{
		UnboundedQueueTickBlock(a_RelX + Coords[i].x, a_RelY + Coords[i].y, a_RelZ + Coords[i].z);
	}  // for i - Coords[]
}





void cChunk::FastSetBlock(int a_RelX, int a_RelY, int a_RelZ, BLOCKTYPE a_BlockType, BLOCKTYPE a_BlockMeta)
{
	ASSERT(!((a_RelX < 0) || (a_RelX >= Width) || (a_RelY < 0) || (a_RelY >= Height) || (a_RelZ < 0) || (a_RelZ >= Width)));

	ASSERT(IsValid());
	
	const int index = MakeIndexNoCheck(a_RelX, a_RelY, a_RelZ);
	const BLOCKTYPE OldBlockType = cChunkDef::GetBlock(m_BlockTypes, index);
	const BLOCKTYPE OldBlockMeta = GetNibble(m_BlockMeta, index);
	if ((OldBlockType == a_BlockType) && (OldBlockMeta == a_BlockMeta))
	{
		return;
	}

	MarkDirty();
	
	m_BlockTypes[index] = a_BlockType;

	// The client doesn't need to distinguish between stationary and nonstationary fluids:
	if (
		(OldBlockMeta != a_BlockMeta) ||  // Different meta always gets sent to the client
		!(
			((OldBlockType == E_BLOCK_STATIONARY_WATER) && (a_BlockType == E_BLOCK_WATER)) ||             // Replacing stationary water with water
			((OldBlockType == E_BLOCK_WATER)            && (a_BlockType == E_BLOCK_STATIONARY_WATER)) ||  // Replacing water with stationary water
			((OldBlockType == E_BLOCK_STATIONARY_LAVA)  && (a_BlockType == E_BLOCK_LAVA)) ||              // Replacing stationary water with water
			((OldBlockType == E_BLOCK_LAVA)             && (a_BlockType == E_BLOCK_STATIONARY_LAVA))      // Replacing water with stationary water
		)
	)
	{
		m_PendingSendBlocks.push_back(sSetBlock(m_PosX, m_PosZ, a_RelX, a_RelY, a_RelZ, a_BlockType, a_BlockMeta));
	}
	
	SetNibble(m_BlockMeta, index, a_BlockMeta);

	// ONLY recalculate lighting if it's necessary!
	if(
		(g_BlockLightValue[OldBlockType ]        != g_BlockLightValue[a_BlockType]) ||
		(g_BlockSpreadLightFalloff[OldBlockType] != g_BlockSpreadLightFalloff[a_BlockType]) ||
		(g_BlockTransparent[OldBlockType]        != g_BlockTransparent[a_BlockType])
	)
	{
		m_IsLightValid = false;
	}

	// Update heightmap, if needed:
	if (a_RelY >= m_HeightMap[a_RelX + a_RelZ * Width])
	{
		if (a_BlockType != E_BLOCK_AIR)
		{
			m_HeightMap[a_RelX + a_RelZ * Width] = (unsigned char)a_RelY;
		}
		else
		{
			for (int y = a_RelY - 1; y > 0; --y)
			{
				if (m_BlockTypes[MakeIndexNoCheck(a_RelX, y, a_RelZ)] != E_BLOCK_AIR)
				{
					m_HeightMap[a_RelX + a_RelZ * Width] = (unsigned char)y;
					break;
				}
			}  // for y - column in m_BlockData
		}
	}
}





void cChunk::SendBlockTo(int a_RelX, int a_RelY, int a_RelZ, cClientHandle * a_Client)
{
	// The coords must be valid, because the upper level already does chunk lookup. No need to check them again.
	// There's an debug-time assert in MakeIndexNoCheck anyway
	unsigned int index = MakeIndexNoCheck(a_RelX, a_RelY, a_RelZ);

	if (a_Client == NULL)
	{
		// Queue the block for all clients in the chunk (will be sent in Tick())
		m_PendingSendBlocks.push_back(sSetBlock(m_PosX, m_PosZ, a_RelX, a_RelY, a_RelZ, GetBlock(index), GetMeta(index)));
		return;
	}

	Vector3i wp = PositionToWorldPosition(a_RelX, a_RelY, a_RelZ);
	a_Client->SendBlockChange(wp.x, wp.y, wp.z, GetBlock(index), GetMeta(index));
	
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





void cChunk::UseBlockEntity(cPlayer * a_Player, int a_X, int a_Y, int a_Z)
{
	cBlockEntity * be = GetBlockEntity(a_X, a_Y, a_Z);
	if (be != NULL)
	{
		be->UsedBy(a_Player);
	}
}





void cChunk::CollectPickupsByPlayer(cPlayer * a_Player)
{
	double PosX = a_Player->GetPosX();
	double PosY = a_Player->GetPosY();
	double PosZ = a_Player->GetPosZ();
	
	for (cEntityList::iterator itr = m_Entities.begin(); itr != m_Entities.end(); ++itr)
	{
		if (!(*itr)->IsPickup())
		{
			continue; // Only pickups
		}
		float DiffX = (float)((*itr)->GetPosX() - PosX );
		float DiffY = (float)((*itr)->GetPosY() - PosY );
		float DiffZ = (float)((*itr)->GetPosZ() - PosZ );
		float SqrDist = DiffX * DiffX + DiffY * DiffY + DiffZ * DiffZ;
		if (SqrDist < 1.5f * 1.5f)  // 1.5 block
		{
			/*
			LOG("Pickup %d being collected by player \"%s\", distance %f",
				(*itr)->GetUniqueID(), a_Player->GetName().c_str(), SqrDist
			);
			*/
			MarkDirty();
			(reinterpret_cast<cPickup *>(*itr))->CollectedBy( a_Player );
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





void cChunk::RemoveBlockEntity( cBlockEntity* a_BlockEntity )
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
	m_LoadedByClient.push_back( a_Client );

	for (cEntityList::iterator itr = m_Entities.begin(); itr != m_Entities.end(); ++itr )
	{
		LOGD("cChunk: Entity #%d (%s) at [%i, %i, %i] spawning for player \"%s\"", (*itr)->GetUniqueID(), (*itr)->GetClass(), m_PosX, m_PosY, m_PosZ, a_Client->GetUsername().c_str());
		(*itr)->SpawnOn(*a_Client);
	}
	return true;
}





void cChunk::RemoveClient( cClientHandle* a_Client )
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
			for (cEntityList::iterator itr = m_Entities.begin(); itr != m_Entities.end(); ++itr )
			{
				LOGD("chunk [%i, %i] destroying entity #%i for player \"%s\"", m_PosX, m_PosZ, (*itr)->GetUniqueID(), a_Client->GetUsername().c_str() );
				a_Client->SendDestroyEntity(*(*itr));
			}
		}
		return;
	}  // for itr - m_LoadedByClient[]
}





bool cChunk::HasClient( cClientHandle* a_Client )
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





bool cChunk::HasAnyClients(void)
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
		if ((*itr)->GetBlockType() != E_BLOCK_CHEST)
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
		return 0; // Clip
	}

	return m_BlockTypes[MakeIndexNoCheck(a_RelX, a_RelY, a_RelZ)];
}





BLOCKTYPE cChunk::GetBlock(int a_BlockIdx) const
{
	if ((a_BlockIdx < 0) || (a_BlockIdx >= NumBlocks))
	{
		ASSERT(!"GetBlock(idx) out of bounds!");
		return 0;
	}
	
	return m_BlockTypes[ a_BlockIdx ];
}





void cChunk::GetBlockTypeMeta(int a_RelX, int a_RelY, int a_RelZ, BLOCKTYPE & a_BlockType, NIBBLETYPE & a_BlockMeta)
{
	int Idx = cChunkDef::MakeIndexNoCheck(a_RelX, a_RelY, a_RelZ);
	a_BlockType = cChunkDef::GetBlock (m_BlockTypes, a_RelX, a_RelY, a_RelZ);
	a_BlockMeta = cChunkDef::GetNibble(m_BlockMeta, a_RelX, a_RelY, a_RelZ);
}





void cChunk::GetBlockInfo(int a_RelX, int a_RelY, int a_RelZ, BLOCKTYPE & a_BlockType, NIBBLETYPE & a_Meta, NIBBLETYPE & a_SkyLight, NIBBLETYPE & a_BlockLight)
{
	int Idx = cChunkDef::MakeIndexNoCheck(a_RelX, a_RelY, a_RelZ);
	a_BlockType  = cChunkDef::GetBlock (m_BlockTypes,    Idx);
	a_Meta       = cChunkDef::GetNibble(m_BlockMeta,     Idx);
	a_SkyLight   = cChunkDef::GetNibble(m_BlockSkyLight, Idx);
	a_BlockLight = cChunkDef::GetNibble(m_BlockLight,    Idx);
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
	for (cClientHandleList::const_iterator itr = m_LoadedByClient.begin(); itr != m_LoadedByClient.end(); ++itr )
	{
		(*itr)->SendAttachEntity(a_Entity, a_Vehicle);
	}  // for itr - LoadedByClient[]
}





void cChunk::BroadcastBlockAction(int a_BlockX, int a_BlockY, int a_BlockZ, char a_Byte1, char a_Byte2, BLOCKTYPE a_BlockType, const cClientHandle * a_Exclude)
{
	for (cClientHandleList::const_iterator itr = m_LoadedByClient.begin(); itr != m_LoadedByClient.end(); ++itr )
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
	for (cClientHandleList::iterator itr = m_LoadedByClient.begin(); itr != m_LoadedByClient.end(); ++itr )
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
	for (cClientHandleList::iterator itr = m_LoadedByClient.begin(); itr != m_LoadedByClient.end(); ++itr )
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
	for (cClientHandleList::iterator itr = m_LoadedByClient.begin(); itr != m_LoadedByClient.end(); ++itr )
	{
		if (*itr == a_Exclude)
		{
			continue;
		}
		(*itr)->SendChunkData(m_PosX, m_PosZ, a_Serializer);
	}  // for itr - LoadedByClient[]
}





void cChunk::BroadcastCollectPickup(const cPickup & a_Pickup, const cPlayer & a_Player, const cClientHandle * a_Exclude)
{
	for (cClientHandleList::iterator itr = m_LoadedByClient.begin(); itr != m_LoadedByClient.end(); ++itr )
	{
		if (*itr == a_Exclude)
		{
			continue;
		}
		(*itr)->SendCollectPickup(a_Pickup, a_Player);
	}  // for itr - LoadedByClient[]
}





void cChunk::BroadcastDestroyEntity(const cEntity & a_Entity, const cClientHandle * a_Exclude)
{
	for (cClientHandleList::const_iterator itr = m_LoadedByClient.begin(); itr != m_LoadedByClient.end(); ++itr )
	{
		if (*itr == a_Exclude)
		{
			continue;
		}
		(*itr)->SendDestroyEntity(a_Entity);
	}  // for itr - LoadedByClient[]
}





void cChunk::BroadcastEntityEquipment(const cEntity & a_Entity, short a_SlotNum, const cItem & a_Item, const cClientHandle * a_Exclude)
{
	for (cClientHandleList::const_iterator itr = m_LoadedByClient.begin(); itr != m_LoadedByClient.end(); ++itr )
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
	for (cClientHandleList::const_iterator itr = m_LoadedByClient.begin(); itr != m_LoadedByClient.end(); ++itr )
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
	for (cClientHandleList::const_iterator itr = m_LoadedByClient.begin(); itr != m_LoadedByClient.end(); ++itr )
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
	for (cClientHandleList::const_iterator itr = m_LoadedByClient.begin(); itr != m_LoadedByClient.end(); ++itr )
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
	for (cClientHandleList::const_iterator itr = m_LoadedByClient.begin(); itr != m_LoadedByClient.end(); ++itr )
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
	for (cClientHandleList::const_iterator itr = m_LoadedByClient.begin(); itr != m_LoadedByClient.end(); ++itr )
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
	for (cClientHandleList::const_iterator itr = m_LoadedByClient.begin(); itr != m_LoadedByClient.end(); ++itr )
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
	for (cClientHandleList::const_iterator itr = m_LoadedByClient.begin(); itr != m_LoadedByClient.end(); ++itr )
	{
		if (*itr == a_Exclude)
		{
			continue;
		}
		(*itr)->SendEntityVelocity(a_Entity);
	}  // for itr - LoadedByClient[]
}





void cChunk::BroadcastPlayerAnimation(const cPlayer & a_Player, char a_Animation, const cClientHandle * a_Exclude)
{
	for (cClientHandleList::const_iterator itr = m_LoadedByClient.begin(); itr != m_LoadedByClient.end(); ++itr )
	{
		if (*itr == a_Exclude)
		{
			continue;
		}
		(*itr)->SendPlayerAnimation(a_Player, a_Animation);
	}  // for itr - LoadedByClient[]
}





void cChunk::BroadcastSoundEffect(const AString & a_SoundName, int a_SrcX, int a_SrcY, int a_SrcZ, float a_Volume, float a_Pitch, const cClientHandle * a_Exclude)
{
	for (cClientHandleList::iterator itr = m_LoadedByClient.begin(); itr != m_LoadedByClient.end(); ++itr )
	{
		if (*itr == a_Exclude)
		{
			continue;
		}
		(*itr)->SendSoundEffect(a_SoundName, a_SrcX, a_SrcY, a_SrcZ, a_Volume, a_Pitch);
	}  // for itr - LoadedByClient[]
}





void cChunk::BroadcastSoundParticleEffect(int a_EffectID, int a_SrcX, int a_SrcY, int a_SrcZ, int a_Data, const cClientHandle * a_Exclude)
{
	for (cClientHandleList::iterator itr = m_LoadedByClient.begin(); itr != m_LoadedByClient.end(); ++itr )
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
	for (cClientHandleList::iterator itr = m_LoadedByClient.begin(); itr != m_LoadedByClient.end(); ++itr )
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
	for (cClientHandleList::iterator itr = m_LoadedByClient.begin(); itr != m_LoadedByClient.end(); ++itr )
	{
		if (*itr == a_Exclude)
		{
			continue;
		}
		(*itr)->SendThunderbolt(a_BlockX, a_BlockY, a_BlockZ);
	}  // for itr - LoadedByClient[]
}





void cChunk::BroadcastUseBed(const cEntity & a_Entity, int a_BlockX, int a_BlockY, int a_BlockZ )
{
	for (cClientHandleList::iterator itr = m_LoadedByClient.begin(); itr != m_LoadedByClient.end(); ++itr )
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





#if !C_CHUNK_USE_INLINE
# include "cChunk.inl.h"
#endif




