
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#ifndef _WIN32
	#include <cstdlib>
#endif


#include "cChunk.h"
#include "cWorld.h"
#include "cWaterSimulator.h"
#include "cLavaSimulator.h"
#include "cClientHandle.h"
#include "cServer.h"
#include "zlib.h"
#include "Defines.h"
#include "cChestEntity.h"
#include "cFurnaceEntity.h"
#include "cSignEntity.h"
#include "cTorch.h"
#include "cLadder.h"
#include "cPickup.h"
#include "cRedstone.h"
#include "cItem.h"
#include "cNoise.h"
#include "cRoot.h"
#include "cBlockToPickup.h"
#include "MersenneTwister.h"
#include "cPlayer.h"

#include "packets/cPacket_DestroyEntity.h"
#include "packets/cPacket_PreChunk.h"
#include "packets/cPacket_BlockChange.h"
#include "packets/cPacket_MultiBlock.h"

#include <json/json.h>





extern bool g_bWaterPhysics;





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





void cChunk::GetBlockData(BLOCKTYPE * a_BlockData)
{
	memcpy(a_BlockData, m_BlockTypes, NumBlocks);
	memcpy(a_BlockData + MetaOffset,     m_BlockMeta,     NumBlocks / 2);
	memcpy(a_BlockData + LightOffset,    m_BlockLight,    NumBlocks / 2);
	memcpy(a_BlockData + SkyLightOffset, m_BlockSkyLight, NumBlocks / 2);
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
	cCSLock Lock(m_CSBlockLists);
	unsigned int PendingSendBlocks = m_PendingSendBlocks.size();
	if( PendingSendBlocks > 1 )
	{
		cPacket_MultiBlock MultiBlock;
		MultiBlock.m_ChunkX = m_PosX;
		MultiBlock.m_ChunkZ = m_PosZ;
		MultiBlock.m_NumBlocks = (short)PendingSendBlocks;
		MultiBlock.m_Data = new cPacket_MultiBlock::sBlockChange[ PendingSendBlocks ];
		MultiBlock.m_DataSize = PendingSendBlocks * sizeof( cPacket_MultiBlock::sBlockChange );
		//LOG("Sending multiblock packet for %i blocks", PendingSendBlocks );
		for( unsigned int i = 0; i < PendingSendBlocks; i++)
		{
			unsigned int index = m_PendingSendBlocks[i];
			Vector3i BlockPos = IndexToCoordinate( index );

			unsigned int Coords = BlockPos.y | (BlockPos.z << 8) | (BlockPos.x << 12);
			unsigned int Blocks = GetNibble( m_BlockMeta, index ) | (m_BlockTypes[index] << 4);
			MultiBlock.m_Data[i].Data = Coords << 16 | Blocks;
		}
		m_PendingSendBlocks.clear();
		PendingSendBlocks = m_PendingSendBlocks.size();
		Broadcast( MultiBlock );
	}
	if( PendingSendBlocks > 0 )
	{
		for( unsigned int i = 0; i < PendingSendBlocks; i++)
		{
			unsigned int index = m_PendingSendBlocks[i];
			Vector3i WorldPos = PositionToWorldPosition( IndexToCoordinate( index ) );

			cPacket_BlockChange BlockChange;
			BlockChange.m_PosX = WorldPos.x;
			BlockChange.m_PosY = (unsigned char)WorldPos.y;
			BlockChange.m_PosZ = WorldPos.z;
			BlockChange.m_BlockType = m_BlockTypes[index];
			BlockChange.m_BlockMeta = GetNibble( m_BlockMeta, index );
			Broadcast( BlockChange );
		}
		m_PendingSendBlocks.clear();
	}
	Lock.Unlock();

	while ( !m_UnloadQuery.empty() )
	{
		cPacket_PreChunk UnloadPacket;
		UnloadPacket.m_PosX = GetPosX();
		UnloadPacket.m_PosZ = GetPosZ();
		UnloadPacket.m_bLoad = false; // Unload
		(*m_UnloadQuery.begin())->Send( UnloadPacket );
		m_UnloadQuery.remove( *m_UnloadQuery.begin() );
	}

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

		BLOCKTYPE  BlockType = GetBlock(index);
		NIBBLETYPE BlockMeta = GetMeta (index);
		switch (BlockType)
		{
			case E_BLOCK_REDSTONE_REPEATER_OFF:
			case E_BLOCK_REDSTONE_REPEATER_ON:
			case E_BLOCK_REDSTONE_WIRE:
			case E_BLOCK_CACTUS:
			case E_BLOCK_REEDS:
			case E_BLOCK_WOODEN_PRESSURE_PLATE:
			case E_BLOCK_STONE_PRESSURE_PLATE:
			case E_BLOCK_MINECART_TRACKS:
			case E_BLOCK_SIGN_POST:
			case E_BLOCK_CROPS:
			case E_BLOCK_SAPLING:
			case E_BLOCK_YELLOW_FLOWER:
			case E_BLOCK_RED_ROSE:
			case E_BLOCK_RED_MUSHROOM:
			case E_BLOCK_BROWN_MUSHROOM:		// Stuff that drops when block below is destroyed
			{
				if (GetBlock(BlockPos.x, BlockPos.y - 1, BlockPos.z) == E_BLOCK_AIR)
				{
					SetBlock( BlockPos, E_BLOCK_AIR, 0 );

					Vector3i WorldPos = PositionToWorldPosition( BlockPos );

					m_World->GetSimulatorManager()->WakeUp(WorldPos.x, WorldPos.y, WorldPos.z);
					
					cItems Pickups;
					cBlockToPickup::ToPickup(BlockType, BlockMeta, E_ITEM_EMPTY, Pickups);
					m_World->SpawnItemPickups(Pickups, WorldPos.x, WorldPos.y, WorldPos.z);
				}
				break;
			}

			case E_BLOCK_REDSTONE_TORCH_OFF:
			case E_BLOCK_REDSTONE_TORCH_ON:
			case E_BLOCK_TORCH:
			{
				char Dir = cTorch::MetaDataToDirection( GetNibble( m_BlockMeta, BlockPos ) );
				Vector3i WorldPos = PositionToWorldPosition( BlockPos );

				Vector3i AttachedTo = WorldPos;
				AddDirection( AttachedTo.x, AttachedTo.y, AttachedTo.z, Dir, true );
				if( m_World->GetBlock( AttachedTo ) == E_BLOCK_AIR )
				{
					SetBlock( BlockPos, E_BLOCK_AIR, 0 );

					m_World->GetSimulatorManager()->WakeUp(WorldPos.x, WorldPos.y, WorldPos.z);

					cItems Pickups;
					cBlockToPickup::ToPickup(BlockType, BlockMeta, E_ITEM_EMPTY, Pickups);
					m_World->SpawnItemPickups(Pickups, WorldPos.x, WorldPos.y, WorldPos.z);
				}
				break;
			}

			case E_BLOCK_LADDER:
			{
				char Dir = cLadder::MetaDataToDirection( GetNibble( m_BlockMeta, BlockPos ) );
				Vector3i WorldPos = PositionToWorldPosition( BlockPos );
				Vector3i AttachedTo = WorldPos;
				AddDirection( AttachedTo.x, AttachedTo.y, AttachedTo.z, Dir, true );
				if( m_World->GetBlock( AttachedTo ) == E_BLOCK_AIR )
				{
					SetBlock( BlockPos, E_BLOCK_AIR, 0 );
					cItems Pickups;
					cBlockToPickup::ToPickup(BlockType, BlockMeta, E_ITEM_EMPTY, Pickups);
					m_World->SpawnItemPickups(Pickups, WorldPos.x, WorldPos.y, WorldPos.z);
				}
				break;
			}
		}  // switch (BlockType)
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

		unsigned int Index = MakeIndexNoCheck( m_BlockTickX, m_BlockTickY, m_BlockTickZ );
		BLOCKTYPE ID = m_BlockTypes[Index];
		switch( ID )
		{
			case E_BLOCK_CROPS:
			{
				NIBBLETYPE Meta = GetMeta(Index);
				if (Meta < 7)
				{
					FastSetBlock(m_BlockTickX, m_BlockTickY, m_BlockTickZ, E_BLOCK_CROPS, ++Meta);
				}
				break;
			}
			
			case E_BLOCK_GRASS:        TickGrass       (m_BlockTickX, m_BlockTickY, m_BlockTickZ, a_TickRandom); break;
			case E_BLOCK_PUMPKIN_STEM:
			case E_BLOCK_MELON_STEM:   TickMelonPumpkin(m_BlockTickX, m_BlockTickY, m_BlockTickZ, Index, ID, a_TickRandom); break;
			case E_BLOCK_FARMLAND:     TickFarmland    (m_BlockTickX, m_BlockTickY, m_BlockTickZ); break;
			case E_BLOCK_SUGARCANE:    GrowSugarcane   (m_BlockTickX, m_BlockTickY, m_BlockTickZ, 1); break;
			case E_BLOCK_CACTUS:       GrowCactus      (m_BlockTickX, m_BlockTickY, m_BlockTickZ, 1); break;
			
			case E_BLOCK_SAPLING:
			{
				// Check the highest bit, if set, grow the tree, if not, set it (1-bit delay):
				NIBBLETYPE Meta = GetMeta(m_BlockTickX, m_BlockTickY, m_BlockTickZ);
				if ((Meta & 0x08) != 0)
				{
					m_World->GrowTree( m_BlockTickX + m_PosX*Width, m_BlockTickY, m_BlockTickZ + m_PosZ*Width );
				}
				else
				{
					SetMeta(m_BlockTickX, m_BlockTickY, m_BlockTickZ, Meta | 0x08);
				}
				break;
			}
			
			case E_BLOCK_LEAVES: //todo, http://www.minecraftwiki.net/wiki/Data_values#Leaves
			{
				break;
			}
			
			default:
			{
				break;
			}
		}
	}
}





void cChunk::TickGrass(int a_RelX, int a_RelY, int a_RelZ, MTRand & a_TickRandom)
{
	// Grass turns into dirt if there's another block on top of it:
	{
		BLOCKTYPE AboveBlock = cChunkDef::GetBlock(m_BlockTypes, a_RelX, a_RelY + 1, a_RelZ);
		if (!((g_BlockOneHitDig[AboveBlock]) || (g_BlockTransparent[AboveBlock])))
		{
			FastSetBlock(a_RelX, a_RelY, a_RelZ, E_BLOCK_DIRT, 0);
			return;
		}
	}
	
	// Grass spreads to nearby blocks if there's enough light (TODO) and free space above that block
	// Ref.: http://www.minecraftwiki.net/wiki/Grass_Block#Growth
	for (int i = 0; i < 2; i++)  // Pick two blocks to grow to
	{
		int OfsX = a_TickRandom.randInt(2) - 1;  // [-1 .. 1]
		int OfsY = a_TickRandom.randInt(4) - 3;  // [-3 .. 1]
		int OfsZ = a_TickRandom.randInt(2) - 1;  // [-1 .. 1]
		
		BLOCKTYPE  DestBlock;
		NIBBLETYPE DestMeta;
		if (
			!UnboundedRelGetBlock(a_RelX + OfsX, a_RelY + OfsY,     a_RelZ + OfsZ, DestBlock, DestMeta) ||
			(DestBlock != E_BLOCK_DIRT)
		)
		{
			continue;
		}

		BLOCKTYPE AboveDest;
		NIBBLETYPE AboveMeta;
		if (
			UnboundedRelGetBlock(a_RelX + OfsX, a_RelY + OfsY + 1, a_RelZ + OfsZ, AboveDest, AboveMeta) &&
			((g_BlockOneHitDig[AboveDest]) || (g_BlockTransparent[AboveDest]))
			// TODO: Query dest light, if it's enough
		)
		{
			UnboundedRelFastSetBlock(a_RelX + OfsX, a_RelY + OfsY, a_RelZ + OfsZ, E_BLOCK_GRASS, 0);
		}
	}  // for i - repeat twice
}





void cChunk::TickMelonPumpkin(int a_RelX, int a_RelY, int a_RelZ, int a_BlockIdx, BLOCKTYPE a_BlockType, MTRand & a_TickRandom)
{
	NIBBLETYPE Meta = GetMeta(a_BlockIdx);
	if (Meta < 7)
	{
		FastSetBlock(m_BlockTickX, m_BlockTickY, m_BlockTickZ, a_BlockType, ++Meta);
		return;
	}
	GrowMelonPumpkin(a_RelX, a_RelY, a_RelZ, a_BlockType, a_TickRandom);
}





void cChunk::TickFarmland(int a_RelX, int a_RelY, int a_RelZ)
{
	// TODO: Rain hydrates blocks, too. Check world weather, don't search for water if raining.
	
	// Search for water in a close proximity:
	// Ref.: http://www.minecraftwiki.net/wiki/Farmland#Hydrated_Farmland_Tiles
	bool Found = false;
	for (int y = a_RelY; y <= a_RelY + 1; y++)
	{
		for (int z = a_RelZ - 4; z <= a_RelZ + 4; z++)
		{
			for (int x = a_RelX - 4; x <= a_RelX + 4; x++)
			{
				BLOCKTYPE BlockType;
				NIBBLETYPE Meta;  // unused
				
				if (!UnboundedRelGetBlock(x, y, z, BlockType, Meta))
				{
					// Too close to an unloaded chunk, we might miss a water block there, so don't tick at all
					return;
				}
				if (
					(BlockType == E_BLOCK_WATER) ||
					(BlockType == E_BLOCK_STATIONARY_WATER)
				)
				{
					Found = true;
					break;
				}
			}  // for x
			if (Found)
			{
				break;
			}
		}  // for z
		if (Found)
		{
			break;
		}
	}  // for y
	
	NIBBLETYPE BlockMeta = GetMeta(a_RelX, a_RelY, a_RelZ);
	
	if (Found)
	{
		// Water was found, hydrate the block until hydration reaches 7:
		if (BlockMeta < 7)
		{
			FastSetBlock(a_RelX, a_RelY, a_RelZ, E_BLOCK_FARMLAND, ++BlockMeta);
		}
		return;
	}

	// Water wasn't found, de-hydrate block:
	if (BlockMeta > 0)
	{
		FastSetBlock(a_RelX, a_RelY, a_RelZ, E_BLOCK_FARMLAND, --BlockMeta);
		return;
	}
	
	// Farmland too dry. Turn back to dirt:
	FastSetBlock(a_RelX, a_RelY, a_RelZ, E_BLOCK_DIRT, 0);
	
	// TODO: Uproot whatever was growing on top:
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
	
	// Grow by at most a_NumBlocks, but no more than height 3:
	int ToGrow = std::min(a_NumBlocks, 4 - (Top - Bottom));
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
	
	// Grow by at most a_NumBlocks, but no more than height 3:
	int ToGrow = std::min(a_NumBlocks, 4 - (Top - Bottom));
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
				ENUM_BLOCK_ID BlockType = (ENUM_BLOCK_ID)m_BlockTypes[ MakeIndex( x, y, z ) ];
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

	m_ToTickBlocks.push_back( MakeIndex( a_RelX,     a_RelY,     a_RelZ ) );
	m_ToTickBlocks.push_back( MakeIndex( a_RelX + 1, a_RelY,     a_RelZ ) );
	m_ToTickBlocks.push_back( MakeIndex( a_RelX - 1, a_RelY,     a_RelZ ) );
	m_ToTickBlocks.push_back( MakeIndex( a_RelX,     a_RelY + 1, a_RelZ ) );
	m_ToTickBlocks.push_back( MakeIndex( a_RelX,     a_RelY - 1, a_RelZ ) );
	m_ToTickBlocks.push_back( MakeIndex( a_RelX,     a_RelY,     a_RelZ + 1 ) );
	m_ToTickBlocks.push_back( MakeIndex( a_RelX,     a_RelY,     a_RelZ - 1 ) );

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
	}  // switch (a_BlockType)
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





void cChunk::SendBlockTo( int a_X, int a_Y, int a_Z, cClientHandle* a_Client )
{
	if( a_Client == 0 )
	{
		cCSLock Lock(m_CSBlockLists);
		unsigned int index = MakeIndex( a_X, a_Y, a_Z );
		if( index != INDEX_OUT_OF_RANGE )
		{
			m_PendingSendBlocks.push_back( index );
		}
		else
		{
			LOGWARN("cChunk::SendBlockTo Index out of range!");
		}
		return;
	}

	for (cClientHandleList::iterator itr = m_LoadedByClient.begin(); itr != m_LoadedByClient.end(); ++itr )
	{
		if ( *itr == a_Client )
		{
			unsigned int index = MakeIndex( a_X, a_Y, a_Z );
			Vector3i WorldPos = PositionToWorldPosition( a_X, a_Y, a_Z );
			cPacket_BlockChange BlockChange;
			BlockChange.m_PosX = WorldPos.x;
			BlockChange.m_PosY = (unsigned char)WorldPos.y;
			BlockChange.m_PosZ = WorldPos.z;
			if( index != INDEX_OUT_OF_RANGE )
			{
				BlockChange.m_BlockType = m_BlockTypes[ index ];
				BlockChange.m_BlockMeta = GetNibble( m_BlockMeta, index );
			} // else it's both 0
			a_Client->Send( BlockChange );
			break;
		}
	}
}





void cChunk::AddBlockEntity( cBlockEntity* a_BlockEntity )
{
	cCSLock Lock(m_CSBlockLists);
	m_BlockEntities.push_back( a_BlockEntity );
}





cBlockEntity * cChunk::GetBlockEntity(int a_X, int a_Y, int a_Z)
{
	// Assumes that the m_CSBlockList is already locked, we're being called from SetBlock()
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
			(*itr)->SendTo(NULL);
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
		LOGD("cChunk: Entity #%d (%s) at [%i, %i, %i] spawning for player \"%s\"", (*itr)->GetUniqueID(), (*itr)->GetClass(), m_PosX, m_PosY, m_PosZ, a_Client->GetUsername().c_str() );
		(*itr)->SpawnOn( a_Client );
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

		if ( !a_Client->IsDestroyed() )
		{
			for (cEntityList::iterator itr = m_Entities.begin(); itr != m_Entities.end(); ++itr )
			{
				LOGD("chunk [%i, %i] destroying entity #%i for player \"%s\"", m_PosX, m_PosZ, (*itr)->GetUniqueID(), a_Client->GetUsername().c_str() );
				cPacket_DestroyEntity DestroyEntity( *itr );
				a_Client->Send( DestroyEntity );
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





/*
// _X 2012_02_23: Loading in old format not supported anymore
/// Loads the chunk from the old-format disk file, erases the file afterwards. Returns true if successful
bool cChunk::LoadFromDisk()
{
	AString SourceFile;
	Printf(SourceFile, "world/X%i_Y%i_Z%i.bin", m_PosX, m_PosY, m_PosZ );

	cFile f;
	if (!f.Open(SourceFile, cFile::fmRead))
	{
		return false;
	}

	if (f.Read(m_BlockData, sizeof(m_BlockData)) != sizeof(m_BlockData))
	{
		LOGERROR("ERROR READING FROM FILE %s", SourceFile.c_str()); 
		return false;
	}

	// Now load Block Entities:
	ENUM_BLOCK_ID BlockType;
	while (f.Read(&BlockType, sizeof(ENUM_BLOCK_ID)) == sizeof(ENUM_BLOCK_ID))
	{
		switch (BlockType)
		{
			case E_BLOCK_CHEST:
			{
				cChestEntity * ChestEntity = new cChestEntity( 0, 0, 0, m_World );
				if (!ChestEntity->LoadFromFile(f))
				{
					LOGERROR("ERROR READING CHEST FROM FILE %s", SourceFile.c_str());
					delete ChestEntity;
					return false;
				}
				m_BlockEntities.push_back( ChestEntity );
				break;
			}
			
			case E_BLOCK_FURNACE:
			{
				cFurnaceEntity* FurnaceEntity = new cFurnaceEntity( 0, 0, 0, m_World );
				if (!FurnaceEntity->LoadFromFile(f))
				{
					LOGERROR("ERROR READING FURNACE FROM FILE %s", SourceFile.c_str());
					delete FurnaceEntity;
					return false;
				}
				m_BlockEntities.push_back( FurnaceEntity );
				break;
			}
			
			case E_BLOCK_SIGN_POST:
			case E_BLOCK_WALLSIGN:
			{
				cSignEntity * SignEntity = new cSignEntity(BlockType, 0, 0, 0, m_World );
				if (!SignEntity->LoadFromFile( f ) )
				{
					LOGERROR("ERROR READING SIGN FROM FILE %s", SourceFile.c_str());
					delete SignEntity;
					return false;
				}
				m_BlockEntities.push_back( SignEntity );
				break;
			}
			
			default:
			{
				ASSERT(!"Unhandled block entity in file");
				break;
			}
		}
	}
	f.Close();

	// Delete old format file
	if (std::remove(SourceFile.c_str()) != 0)
	{
		LOGERROR("Could not delete file %s", SourceFile.c_str());
	}
	else
	{
		LOGINFO("Successfully deleted old format file \"%s\"", SourceFile.c_str());
	}
	m_IsDirty = false;
	return true;
}
*/





void cChunk::Broadcast( const cPacket * a_Packet, cClientHandle* a_Exclude)
{
	for (cClientHandleList::const_iterator itr = m_LoadedByClient.begin(); itr != m_LoadedByClient.end(); ++itr )
	{
		if (*itr == a_Exclude)
		{
			continue;
		}
		(*itr)->Send(*a_Packet);
	}  // for itr - LoadedByClient[]
}





void cChunk::PositionToWorldPosition(int a_ChunkX, int a_ChunkY, int a_ChunkZ, int & a_X, int & a_Y, int & a_Z)
{
	a_Y = a_ChunkY;
	a_X = m_PosX * Width + a_ChunkX;
	a_Z = m_PosZ * Width + a_ChunkZ;
}





Vector3i cChunk::PositionToWorldPosition( int a_ChunkX, int a_ChunkY, int a_ChunkZ )
{
	return Vector3i( m_PosX * Width + a_ChunkX, m_PosY * Height + a_ChunkY, m_PosZ * Width + a_ChunkZ );
}





#if !C_CHUNK_USE_INLINE
# include "cChunk.inl.h"
#endif




