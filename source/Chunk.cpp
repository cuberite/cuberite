
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
#include "ChestEntity.h"
#include "FurnaceEntity.h"
#include "SignEntity.h"
#include "NoteEntity.h"
#include "JukeboxEntity.h"
#include "Torch.h"
#include "Ladder.h"
#include "Pickup.h"
#include "Item.h"
#include "Noise.h"
#include "Root.h"
#include "MersenneTwister.h"
#include "Player.h"
#include "BlockArea.h"
#include "PluginManager.h"
#include "Blocks/BlockHandler.h"

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

cChunk::cChunk(int a_ChunkX, int a_ChunkY, int a_ChunkZ, cChunkMap * a_ChunkMap, cWorld * a_World)
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
{
	// LOGINFO("### new cChunk (%i, %i) at %p, thread 0x%x ###", a_X, a_Z, this, GetCurrentThreadId());
}





cChunk::~cChunk()
{
	// LOGINFO("### delete cChunk() (%i, %i) from %p, thread 0x%x ###", m_PosX, m_PosZ, this, GetCurrentThreadId() );
	
	for (cBlockEntityList::iterator itr = m_BlockEntities.begin(); itr != m_BlockEntities.end(); ++itr)
	{
		delete *itr;
	}
	m_BlockEntities.clear();

	// Remove and destroy all entities that are not players:
	cEntityList Entities;
	for (cEntityList::const_iterator itr = m_Entities.begin(); itr != m_Entities.end(); ++itr)
	{
		if ((*itr)->GetEntityType() != cEntity::eEntityType_Player)
		{
			Entities.push_back(*itr);
		}
	}
	for (cEntityList::iterator itr = Entities.begin(); itr != Entities.end(); ++itr)
	{
		(*itr)->RemoveFromChunk();
		(*itr)->Destroy();
	}
	m_Entities.clear();
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
	cEntityList & a_Entities, 
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

	// Append entities to current entity list:
	m_Entities.splice(m_Entities.end(), a_Entities);
	
	// Clear the block entities present - either the loader / saver has better, or we'll create empty ones:
	for (cBlockEntityList::iterator itr = m_BlockEntities.begin(); itr != m_BlockEntities.end(); ++itr)
	{
		delete *itr;
	}
	std::swap(a_BlockEntities, m_BlockEntities);
	
	// Create block entities that the loader didn't load; fill them with defaults
	CreateBlockEntities();

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





void cChunk::Tick(float a_Dt, MTRand & a_TickRandom)
{
	BroadcastPendingBlockChanges();

	// Unload the chunk from all clients that have queued unloading:
	for (cClientHandleList::iterator itr = m_UnloadQuery.begin(), end = m_UnloadQuery.end(); itr != end; ++itr)
	{
		(*itr)->SendUnloadChunk(m_PosX, m_PosZ);
	}
	m_UnloadQuery.clear();

	CheckBlocks();
	
	TickBlocks(a_TickRandom);

	// Tick block entities (furnaces)
	for (cBlockEntityList::iterator itr = m_BlockEntities.begin(); itr != m_BlockEntities.end(); ++itr)
	{
		if ((*itr)->GetBlockType() == E_BLOCK_FURNACE)
		{
			m_IsDirty = ((cFurnaceEntity *)(*itr))->Tick( a_Dt ) | m_IsDirty;
		}
	}
	
	ApplyWeatherToTop(a_TickRandom);
}





void cChunk::BroadcastPendingBlockChanges(void)
{
	sSetBlockVector Changes;
	{
		cCSLock Lock(m_CSBlockLists);
		if (m_PendingSendBlocks.empty())
		{
			return;
		}
		Changes.reserve(m_PendingSendBlocks.size());
		for (std::vector<unsigned int>::iterator itr = m_PendingSendBlocks.begin(), end = m_PendingSendBlocks.end(); itr != end; ++itr)
		{
			unsigned int index = *itr;
			Vector3i RelPos = IndexToCoordinate(index);
			Changes.push_back(sSetBlock(m_PosX, m_PosZ, RelPos.x, RelPos.y, RelPos.z, GetBlock(index), GetMeta(index)));
		}  // for itr - m_PendingSendBlocks[]
		m_PendingSendBlocks.clear();
	}
	
	for (cClientHandleList::iterator itr = m_LoadedByClient.begin(), end = m_LoadedByClient.end(); itr != end; ++itr)
	{
		(*itr)->SendBlockChanges(m_PosX, m_PosZ, Changes);
	}
}





void cChunk::CheckBlocks(void)
{
	cCSLock Lock2(m_CSBlockLists);
	unsigned int NumTickBlocks = m_ToTickBlocks.size();
	Lock2.Unlock();

	if (NumTickBlocks == 0)
	{
		return;
	}
	
	Lock2.Lock();
	std::deque< unsigned int > ToTickBlocks = m_ToTickBlocks;
	m_ToTickBlocks.clear();
	Lock2.Unlock();
	
	for (std::deque< unsigned int >::const_iterator itr = ToTickBlocks.begin(); itr != ToTickBlocks.end(); ++itr)
	{
		unsigned int index = (*itr);
		Vector3i BlockPos = IndexToCoordinate(index);
		Vector3i WorldPos = PositionToWorldPosition( BlockPos );

		cBlockHandler * Handler = BlockHandler(GetBlock(index));
		Handler->Check(m_World, WorldPos.x, WorldPos.y, WorldPos.z);
	}  // for itr - ToTickBlocks[]
}





void cChunk::TickBlocks(MTRand & a_TickRandom)
{
	// Tick dem blocks
	// _X: We must limit the random number or else we get a nasty int overflow bug ( http://forum.mc-server.org/showthread.php?tid=457 )
	int RandomX = a_TickRandom.randInt(0x00ffffff);
	int RandomY = a_TickRandom.randInt(0x00ffffff);
	int RandomZ = a_TickRandom.randInt(0x00ffffff);
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





void cChunk::ApplyWeatherToTop(MTRand & a_TickRandom)
{
	if (
		(a_TickRandom.randInt(100) != 0) ||
		(
			(m_World->GetWeather() != eWeather_Rain) &&
			(m_World->GetWeather() != eWeather_ThunderStorm)
		)
	)
	{
		// Not the right weather, or not at this tick; bail out
		return;
	}
	
	int X = a_TickRandom.randInt(15);
	int Z = a_TickRandom.randInt(15);
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





bool cChunk::UnboundedRelGetBlock(int a_RelX, int a_RelY, int a_RelZ, BLOCKTYPE & a_BlockType, NIBBLETYPE & a_BlockMeta)
{
	if ((a_RelY < 0) || (a_RelY > cChunkDef::Height))
	{
		return false;
	}
	
	if ((a_RelX >= 0) && (a_RelX < cChunkDef::Width) && (a_RelZ >= 0) && (a_RelZ < cChunkDef::Width))
	{
		int BlockIdx = cChunkDef::MakeIndexNoCheck(a_RelX, a_RelY, a_RelZ);
		a_BlockType = GetBlock(BlockIdx);
		a_BlockMeta = GetMeta(BlockIdx);
		return true;
	}
	return m_ChunkMap->LockedGetBlock(
		m_PosX * cChunkDef::Width + a_RelX,
		ZERO_CHUNK_Y * cChunkDef::Height + a_RelY,
		m_PosZ * cChunkDef::Width + a_RelZ,
		a_BlockType, a_BlockMeta
	);
}





bool cChunk::UnboundedRelSetBlock(int a_RelX, int a_RelY, int a_RelZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta)
{
	if ((a_RelX >= 0) && (a_RelX < cChunkDef::Width) && (a_RelZ >= 0) && (a_RelZ < cChunkDef::Width))
	{
		SetBlock(a_RelX, a_RelY, a_RelZ, a_BlockType, a_BlockMeta);
		return true;
	}
	return m_ChunkMap->LockedSetBlock(
		m_PosX * cChunkDef::Width + a_RelX,
		ZERO_CHUNK_Y * cChunkDef::Height + a_RelY,
		m_PosZ * cChunkDef::Width + a_RelZ,
		a_BlockType, a_BlockMeta
	);
}





bool cChunk::UnboundedRelFastSetBlock(int a_RelX, int a_RelY, int a_RelZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta)
{
	if ((a_RelX >= 0) && (a_RelX < cChunkDef::Width) && (a_RelZ >= 0) && (a_RelZ < cChunkDef::Width))
	{
		FastSetBlock(a_RelX, a_RelY, a_RelZ, a_BlockType, a_BlockMeta);
		return true;
	}
	return m_ChunkMap->LockedFastSetBlock(
		m_PosX * cChunkDef::Width + a_RelX,
		ZERO_CHUNK_Y * cChunkDef::Height + a_RelY,
		m_PosZ * cChunkDef::Width + a_RelZ,
		a_BlockType, a_BlockMeta
	);
}





int cChunk::GetHeight( int a_X, int a_Z )
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
				ENUM_BLOCK_ID BlockType = (ENUM_BLOCK_ID)cChunkDef::GetBlock(m_BlockTypes, x, y, z);
				switch ( BlockType )
				{
					case E_BLOCK_CHEST:
					{
						if (!HasBlockEntityAt(x + m_PosX * Width, y + m_PosY * Height, z + m_PosZ * Width))
						{
							m_BlockEntities.push_back( new cChestEntity( x + m_PosX * Width, y + m_PosY * Height, z + m_PosZ * Width, m_World) );
						}
						break;
					}
					
					case E_BLOCK_FURNACE:
					{
						if (!HasBlockEntityAt(x + m_PosX * Width, y + m_PosY * Height, z + m_PosZ * Width))
						{
							m_BlockEntities.push_back( new cFurnaceEntity( x + m_PosX * Width, y + m_PosY * Height, z + m_PosZ * Width, m_World) );
						}
						break;
					}
					
					case E_BLOCK_SIGN_POST:
					case E_BLOCK_WALLSIGN:
					{
						if (!HasBlockEntityAt(x + m_PosX * Width, y + m_PosY * Height, z + m_PosZ * Width))
						{
							m_BlockEntities.push_back( new cSignEntity( BlockType, x + m_PosX * Width, y + m_PosY * Height, z + m_PosZ * Width, m_World) );
						}
						break;
					}

					case E_BLOCK_NOTE_BLOCK:
					{
						if (!HasBlockEntityAt(x + m_PosX * Width, y + m_PosY * Height, z + m_PosZ * Width))
						{
							m_BlockEntities.push_back(new cNoteEntity(x + m_PosX * Width, y + m_PosY * Height, z + m_PosZ * Width, m_World) );
						}
						break;
					}

					case E_BLOCK_JUKEBOX:
					{
						if (!HasBlockEntityAt(x + m_PosX * Width, y + m_PosY * Height, z + m_PosZ * Width))
						{
							m_BlockEntities.push_back(new cJukeboxEntity(x + m_PosX * Width, y + m_PosY * Height, z + m_PosZ * Width, m_World) );
						}
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





void cChunk::SetBlock( int a_RelX, int a_RelY, int a_RelZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta )
{
	if (a_RelX < 0 || a_RelX >= Width || a_RelY < 0 || a_RelY >= Height || a_RelZ < 0 || a_RelZ >= Width)
	{
		return;  // Clip
	}

	ASSERT(IsValid());  // Is this chunk loaded / generated?
	
	int index = MakeIndexNoCheck( a_RelX, a_RelY, a_RelZ );
	BLOCKTYPE OldBlockMeta = GetNibble( m_BlockMeta, index );
	BLOCKTYPE OldBlockType = m_BlockTypes[index];
	m_BlockTypes[index] = a_BlockType;

	SetNibble( m_BlockMeta, index, a_BlockMeta );

	if ((OldBlockType == a_BlockType) && (OldBlockMeta == a_BlockMeta))
	{
		return;
	}

	MarkDirty();
	
	{
		cCSLock Lock(m_CSBlockLists);
		m_PendingSendBlocks.push_back( index );
	}

	// ONLY recalculate lighting if it's necessary!
	if(
		(g_BlockLightValue[ OldBlockType ] != g_BlockLightValue[ a_BlockType ]) ||
		(g_BlockSpreadLightFalloff[ OldBlockType ] != g_BlockSpreadLightFalloff[ a_BlockType ]) ||
		(g_BlockTransparent[ OldBlockType ] != g_BlockTransparent[ a_BlockType ] )
	)
	{
		m_IsLightValid = false;
	}

	// Update heightmap, if needed:
	if (a_RelY >= m_HeightMap[a_RelX + a_RelZ * Width])
	{
		if (a_BlockType != E_BLOCK_AIR)
		{
			SetHeight(m_HeightMap, a_RelX, a_RelZ, a_RelY);
		}
		else
		{
			for (int y = a_RelY - 1; y > 0; --y)
			{
				if (cChunkDef::GetBlock(m_BlockTypes, a_RelX, y, a_RelZ) != E_BLOCK_AIR)
				{
					SetHeight(m_HeightMap, a_RelX, a_RelZ, y);
					break;
				}
			}  // for y - column in m_BlockData
		}
	}

	m_ToTickBlocks.push_back(index);
	CheckNeighbors(a_RelX, a_RelY, a_RelZ);

	Vector3i WorldPos = PositionToWorldPosition( a_RelX, a_RelY, a_RelZ );
	cBlockEntity* BlockEntity = GetBlockEntity( WorldPos );
	if( BlockEntity )
	{
		BlockEntity->Destroy();
		RemoveBlockEntity( BlockEntity );
		delete BlockEntity;
	}
	switch( a_BlockType )
	{
		case E_BLOCK_CHEST:
		{
			AddBlockEntity( new cChestEntity( WorldPos.x, WorldPos.y, WorldPos.z, m_World) );
			break;
		}
		case E_BLOCK_FURNACE:
		{
			AddBlockEntity( new cFurnaceEntity( WorldPos.x, WorldPos.y, WorldPos.z, m_World) );
			break;
		}
		case E_BLOCK_SIGN_POST:
		case E_BLOCK_WALLSIGN:
		{
			AddBlockEntity( new cSignEntity( (ENUM_BLOCK_ID)a_BlockType, WorldPos.x, WorldPos.y, WorldPos.z, m_World) );
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





void cChunk::CheckNeighbors(int a_RelX, int a_RelY, int a_RelZ)
{
	int BlockX = m_PosX * cChunkDef::Width + a_RelX;
	int BlockZ = m_PosZ * cChunkDef::Width + a_RelZ;
	if (a_RelX < cChunkDef::Width - 1)
	{
		m_ToTickBlocks.push_back(MakeIndexNoCheck(a_RelX + 1, a_RelY, a_RelZ));
	}
	else
	{
		m_ChunkMap->CheckBlock(BlockX + 1, a_RelY, BlockZ);
	}
	
	if (a_RelX > 0)
	{
		m_ToTickBlocks.push_back( MakeIndexNoCheck(a_RelX - 1, a_RelY,     a_RelZ));
	}
	else
	{
		m_ChunkMap->CheckBlock(BlockX - 1, a_RelY, BlockZ);
	}
	
	if (a_RelY < cChunkDef::Height - 1)
	{
		m_ToTickBlocks.push_back(MakeIndexNoCheck(a_RelX, a_RelY + 1, a_RelZ));
	}
	
	if (a_RelY > 0)
	{
		m_ToTickBlocks.push_back(MakeIndexNoCheck(a_RelX, a_RelY - 1, a_RelZ));
	}
	
	if (a_RelZ < cChunkDef::Width - 1)
	{
		m_ToTickBlocks.push_back(MakeIndexNoCheck(a_RelX, a_RelY, a_RelZ + 1));
	}
	else
	{
		m_ChunkMap->CheckBlock(BlockX, a_RelY, BlockZ + 1);
	}
	
	if (a_RelZ > 0)
	{
		m_ToTickBlocks.push_back(MakeIndexNoCheck(a_RelX, a_RelY, a_RelZ - 1));
	}
	else
	{
		m_ChunkMap->CheckBlock(BlockX, a_RelY, BlockZ - 1);
	}
}





void cChunk::CheckBlock(int a_RelX, int a_RelY, int a_RelZ)
{
	if (!IsValid())
	{
		return;
	}
	
	m_ToTickBlocks.push_back(MakeIndexNoCheck(a_RelX, a_RelY, a_RelZ));
}





void cChunk::FastSetBlock( int a_X, int a_Y, int a_Z, BLOCKTYPE a_BlockType, BLOCKTYPE a_BlockMeta)
{
	ASSERT(!((a_X < 0 || a_X >= Width || a_Y < 0 || a_Y >= Height || a_Z < 0 || a_Z >= Width)));

	ASSERT(IsValid());
	
	const int index = MakeIndexNoCheck( a_X, a_Y, a_Z );
	const BLOCKTYPE OldBlock = m_BlockTypes[index];
	const BLOCKTYPE OldBlockMeta = GetNibble( m_BlockMeta, index );
	if ((OldBlock == a_BlockType) && (OldBlockMeta == a_BlockMeta))
	{
		return;
	}

	MarkDirty();
	
	m_BlockTypes[index] = a_BlockType;

	{
		cCSLock Lock(m_CSBlockLists);
		m_PendingSendBlocks.push_back( index );
	}
	
	SetNibble( m_BlockMeta, index, a_BlockMeta );

	// ONLY recalculate lighting if it's necessary!
	if(
		(g_BlockLightValue[ OldBlock ] != g_BlockLightValue[ a_BlockType ]) ||
		(g_BlockSpreadLightFalloff[ OldBlock ] != g_BlockSpreadLightFalloff[ a_BlockType ]) ||
		(g_BlockTransparent[ OldBlock ] != g_BlockTransparent[ a_BlockType ] )
	)
	{
		m_IsLightValid = false;
	}

	// Update heightmap, if needed:
	if (a_Y >= m_HeightMap[a_X + a_Z * Width])
	{
		if (a_BlockType != E_BLOCK_AIR)
		{
			m_HeightMap[a_X + a_Z * Width] = (unsigned char)a_Y;
		}
		else
		{
			for (int y = a_Y - 1; y > 0; --y)
			{
				if (m_BlockTypes[MakeIndexNoCheck(a_X, y, a_Z)] != E_BLOCK_AIR)
				{
					m_HeightMap[a_X + a_Z * Width] = (unsigned char)y;
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
		m_PendingSendBlocks.push_back(index);
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





void cChunk::AddBlockEntity( cBlockEntity* a_BlockEntity )
{
	cCSLock Lock(m_CSBlockLists);
	m_BlockEntities.push_back( a_BlockEntity );
}





cBlockEntity * cChunk::GetBlockEntity(int a_X, int a_Y, int a_Z)
{
	for (cBlockEntityList::iterator itr = m_BlockEntities.begin(); itr != m_BlockEntities.end(); ++itr)
	{
		if (
			((*itr)->GetPosX() == a_X) &&
			((*itr)->GetPosY() == a_Y) &&
			((*itr)->GetPosZ() == a_Z)
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
		if ( (*itr)->GetEntityType() != cEntity::eEntityType_Pickup )
		{
			continue; // Only pickups
		}
		float DiffX = (float)((*itr)->GetPosX() - PosX );
		float DiffY = (float)((*itr)->GetPosY() - PosY );
		float DiffZ = (float)((*itr)->GetPosZ() - PosZ );
		float SqrDist = DiffX * DiffX + DiffY * DiffY + DiffZ * DiffZ;
		if (SqrDist < 1.5f * 1.5f)  // 1.5 block
		{
			MarkDirty();
			(reinterpret_cast<cPickup *>(*itr))->CollectedBy( a_Player );
		}
	}
}





void cChunk::UpdateSign(int a_PosX, int a_PosY, int a_PosZ, const AString & a_Line1, const AString & a_Line2, const AString & a_Line3, const AString & a_Line4)
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
		}
	}  // for itr - m_BlockEntities[]
}





void cChunk::RemoveBlockEntity( cBlockEntity* a_BlockEntity )
{
	cCSLock Lock(m_CSBlockLists);
	MarkDirty();
	m_BlockEntities.remove( a_BlockEntity );
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





void cChunk::AddEntity( cEntity * a_Entity)
{
	if (a_Entity->GetEntityType() != cEntity::eEntityType_Player)
	{
		MarkDirty();
	}
	m_Entities.push_back( a_Entity );
}





void cChunk::RemoveEntity(cEntity * a_Entity)
{
	size_t SizeBefore = m_Entities.size();
	m_Entities.remove(a_Entity);
	size_t SizeAfter = m_Entities.size();
	
	if (SizeBefore != SizeAfter)
	{
		// Mark as dirty if it was a server-generated entity:
		if (a_Entity->GetEntityType() != cEntity::eEntityType_Player)
		{
			MarkDirty();
		}
	}
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





BLOCKTYPE cChunk::GetBlock( int a_X, int a_Y, int a_Z )
{
	if ((a_X < 0) || (a_X >= Width) || (a_Y < 0) || (a_Y >= Height) || (a_Z < 0) || (a_Z >= Width)) return 0; // Clip

	return m_BlockTypes[ MakeIndexNoCheck( a_X, a_Y, a_Z ) ];
}





BLOCKTYPE cChunk::GetBlock( int a_BlockIdx )
{
	if( a_BlockIdx < 0 || a_BlockIdx >= NumBlocks ) return 0;
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





void cChunk::BroadcastEntRelMoveLook(const cEntity & a_Entity, char a_RelX, char a_RelY, char a_RelZ, const cClientHandle * a_Exclude)
{
	for (cClientHandleList::const_iterator itr = m_LoadedByClient.begin(); itr != m_LoadedByClient.end(); ++itr )
	{
		if (*itr == a_Exclude)
		{
			continue;
		}
		(*itr)->SendEntRelMoveLook(a_Entity, a_RelX, a_RelY, a_RelZ);
	}  // for itr - LoadedByClient[]
}





void cChunk::BroadcastEntRelMove(const cEntity & a_Entity, char a_RelX, char a_RelY, char a_RelZ, const cClientHandle * a_Exclude)
{
	for (cClientHandleList::const_iterator itr = m_LoadedByClient.begin(); itr != m_LoadedByClient.end(); ++itr )
	{
		if (*itr == a_Exclude)
		{
			continue;
		}
		(*itr)->SendEntRelMove(a_Entity, a_RelX, a_RelY, a_RelZ);
	}  // for itr - LoadedByClient[]
}





void cChunk::BroadcastEntLook(const cEntity & a_Entity, const cClientHandle * a_Exclude)
{
	for (cClientHandleList::const_iterator itr = m_LoadedByClient.begin(); itr != m_LoadedByClient.end(); ++itr )
	{
		if (*itr == a_Exclude)
		{
			continue;
		}
		(*itr)->SendEntLook(a_Entity);
	}  // for itr - LoadedByClient[]
}





void cChunk::BroadcastEntHeadLook(const cEntity & a_Entity, const cClientHandle * a_Exclude)
{
	for (cClientHandleList::const_iterator itr = m_LoadedByClient.begin(); itr != m_LoadedByClient.end(); ++itr )
	{
		if (*itr == a_Exclude)
		{
			continue;
		}
		(*itr)->SendEntHeadLook(a_Entity);
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





void cChunk::BroadcastMetadata(const cPawn & a_Pawn, const cClientHandle * a_Exclude)
{
	for (cClientHandleList::const_iterator itr = m_LoadedByClient.begin(); itr != m_LoadedByClient.end(); ++itr )
	{
		if (*itr == a_Exclude)
		{
			continue;
		}
		(*itr)->SendMetadata(a_Pawn);
	}  // for itr - LoadedByClient[]
}





void cChunk::BroadcastSpawn(cEntity & a_Entity, const cClientHandle * a_Exclude)
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





void cChunk::BroadcastUseBed(const cEntity & a_Entity, int a_BlockX, int a_BlockY, int a_BlockZ )
{
	for (cClientHandleList::iterator itr = m_LoadedByClient.begin(); itr != m_LoadedByClient.end(); ++itr )
	{
		(*itr)->SendUseBed(a_Entity, a_BlockX, a_BlockY, a_BlockZ);
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




