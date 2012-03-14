
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
#include "cWorldGenerator.h"
#include "cBlockToPickup.h"
#include "MersenneTwister.h"
#include "cPlayer.h"

#include "packets/cPacket_DestroyEntity.h"
#include "packets/cPacket_PreChunk.h"
#include "packets/cPacket_BlockChange.h"
#include "packets/cPacket_MapChunk.h"
#include "packets/cPacket_MultiBlock.h"

#include <json/json.h>





extern bool g_bWaterPhysics;





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// sSetBlock:

sSetBlock::sSetBlock( int a_X, int a_Y, int a_Z, char a_BlockType, char a_BlockMeta )  // absolute block position
	: x( a_X )
	, y( a_Y )
	, z( a_Z )
	, BlockType( a_BlockType )
	, BlockMeta( a_BlockMeta )
{
	cChunkDef::AbsoluteToRelative(x, y, z, ChunkX, ChunkZ);
}





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cChunk:

cChunk::cChunk(int a_X, int a_Y, int a_Z, cChunkMap * a_ChunkMap, cWorld * a_World)
	: m_bCalculateLighting( false )
	, m_PosX( a_X )
	, m_PosY( a_Y )
	, m_PosZ( a_Z )
	, m_BlockTickNum( 0 )
	, m_BlockTickX( 0 )
	, m_BlockTickY( 0 )
	, m_BlockTickZ( 0 )
	, m_World( a_World )
	, m_ChunkMap(a_ChunkMap)
	, m_IsValid(false)
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





void cChunk::SetValid(bool a_SendToClients)
{
	m_IsValid = true;
	
	m_World->GetChunkMap()->ChunkValidated();
	
	if (!a_SendToClients)
	{
		return;
	}
	
	if (m_LoadedByClient.empty())
	{
		return;
	}
	
	// Sending the chunk here interferes with the lighting done in the tick thread and results in the "invalid compressed data" on the client
	/*
	cPacket_PreChunk PreChunk;
	PreChunk.m_PosX = m_PosX;
	PreChunk.m_PosZ = m_PosZ;
	PreChunk.m_bLoad = true;
	cPacket_MapChunk MapChunk(this);
	Broadcast(&PreChunk);
	Broadcast(&MapChunk);
	
	// Let all clients of this chunk know that it has been already sent to the client
	for (cClientHandleList::iterator itr = m_LoadedByClient.begin(); itr != m_LoadedByClient.end(); ++itr)
	{
		(*itr)->ChunkJustSent(this);
	}  // for itr - m_LoadedByClient[]
	*/
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
	a_Callback.BlockTypes   (m_BlockTypes);
	a_Callback.BlockMeta    (m_BlockMeta);
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
	const BLOCKTYPE * a_BlockTypes, 
	const BLOCKTYPE * a_BlockMeta,
	const BLOCKTYPE * a_BlockLight,
	const BLOCKTYPE * a_BlockSkyLight,
	const HeightMap * a_HeightMap,
	cEntityList & a_Entities, 
	cBlockEntityList & a_BlockEntities
)
{
	if (a_HeightMap != NULL)
	{
		memcpy(m_HeightMap, a_HeightMap, sizeof(m_HeightMap));
	}
	
	memcpy(m_BlockTypes,    a_BlockTypes,    sizeof(m_BlockTypes));
	memcpy(m_BlockMeta,     a_BlockMeta,     sizeof(m_BlockMeta));
	memcpy(m_BlockLight,    a_BlockLight,    sizeof(m_BlockLight));
	memcpy(m_BlockSkyLight, a_BlockSkyLight, sizeof(m_BlockSkyLight));
	
	if (a_HeightMap == NULL)
	{
		CalculateHeightmap();
	}

	// Clear the internal entities:
	for (cEntityList::iterator itr = m_Entities.begin(); itr != m_Entities.end(); ++itr)
	{
		if ((*itr)->GetEntityType() == cEntity::eEntityType_Player)
		{
			// Move players into the new entity list
			a_Entities.push_back(*itr);
		}
		else
		{
			// Delete other entities (there should not be any, since we're now loading / generating the chunk)
			LOGWARNING("cChunk: There is an unexpected entity #%d of type %s in chunk [%d, %d]; it will be deleted",
				(*itr)->GetUniqueID(), (*itr)->GetClass(),
				m_PosX, m_PosZ
			);

			// MD 2012_03_10: This may happen if a mob is generated near the edge of loaded chunks and walks off of the edge.
			// Older: Assert because this is a very curious case. These lines were executed once before, when a player died, re spawned, and walked around a bit. It's uncertain why an entity would be in the chunk in this case.
			// ASSERT(!"Unexpected entity in chunk!");

			(*itr)->Destroy();
		}
	}
	for (cBlockEntityList::iterator itr = m_BlockEntities.begin(); itr != m_BlockEntities.end(); ++itr)
	{
		delete *itr;
	}
	
	// Swap the entity lists:
	std::swap(a_Entities, m_Entities);
	std::swap(a_BlockEntities, m_BlockEntities);
	
	// Create block entities that the loader didn't load; fill them with defaults
	CreateBlockEntities();

	m_HasLoadFailed = false;
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
	if (m_bCalculateLighting)
	{
		CalculateLighting();
	}

	cCSLock Lock(m_CSBlockLists);
	unsigned int PendingSendBlocks = m_PendingSendBlocks.size();
	if( PendingSendBlocks > 1 )
	{
		cPacket_MultiBlock MultiBlock;
		MultiBlock.m_ChunkX = m_PosX;
		MultiBlock.m_ChunkZ = m_PosZ;
		MultiBlock.m_NumBlocks = (short)PendingSendBlocks;
#if (MINECRAFT_1_2_2 == 1)
		MultiBlock.m_Data = new cPacket_MultiBlock::sBlockChange[ PendingSendBlocks ];
		MultiBlock.m_DataSize = PendingSendBlocks * sizeof( cPacket_MultiBlock::sBlockChange );
#else
		MultiBlock.m_BlockCoordinates = new unsigned short[PendingSendBlocks];
		MultiBlock.m_BlockTypes = new char[PendingSendBlocks];
		MultiBlock.m_BlockMetas = new char[PendingSendBlocks];
#endif
		//LOG("Sending multiblock packet for %i blocks", PendingSendBlocks );
		for( unsigned int i = 0; i < PendingSendBlocks; i++)
		{
			unsigned int index = m_PendingSendBlocks[i];
			Vector3i BlockPos = IndexToCoordinate( index );

#if (MINECRAFT_1_2_2 == 1)
			unsigned int Coords = BlockPos.y | (BlockPos.z << 8) | (BlockPos.x << 12);
			unsigned int Blocks = GetNibble( m_BlockMeta, index ) | (m_BlockTypes[index] << 4);
			MultiBlock.m_Data[i].Data = Coords << 16 | Blocks;
#else
			MultiBlock.m_BlockCoordinates[i] = (BlockPos.z&0xf) | (BlockPos.x&0xf)<<4 | (BlockPos.y&0xff)<<8;
			//LOG("X: %i Y: %i Z: %i Combo: 0x%04x", X, Y, Z, MultiBlock.m_BlockCoordinates[i] );
			MultiBlock.m_BlockTypes[i] = m_BlockTypes[index];
			MultiBlock.m_BlockMetas[i] = GetNibble( m_BlockMeta, index );
#endif
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

	cCSLock Lock2(m_CSBlockLists);
	unsigned int NumTickBlocks = m_ToTickBlocks.size();
	Lock2.Unlock();

	if( NumTickBlocks > 0 )
	{
		Lock2.Lock();
		std::deque< unsigned int > ToTickBlocks = m_ToTickBlocks;
		m_ToTickBlocks.clear();
		Lock2.Unlock();
	
		bool isRedstone = false;
		for( std::deque< unsigned int >::iterator itr = ToTickBlocks.begin(); itr != ToTickBlocks.end(); ++itr )
		{
			unsigned int index = (*itr);
			Vector3i BlockPos = IndexToCoordinate( index );

			char BlockID = GetBlock( index );
			switch( BlockID )
			{
			case E_BLOCK_REDSTONE_REPEATER_OFF:
			case E_BLOCK_REDSTONE_REPEATER_ON:
			case E_BLOCK_REDSTONE_WIRE:
				{
					isRedstone = true;
				}
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
					if( GetBlock( BlockPos.x, BlockPos.y-1, BlockPos.z ) == E_BLOCK_AIR )
					{
						SetBlock( BlockPos, E_BLOCK_AIR, 0 );

						Vector3i WorldPos = PositionToWorldPosition( BlockPos );

						m_World->GetSimulatorManager()->WakeUp(WorldPos.x, WorldPos.y, WorldPos.z);

						cPickup* Pickup = new cPickup( WorldPos.x * 32 + 16, WorldPos.y * 32 + 16, WorldPos.z * 32 + 16, cItem( cBlockToPickup::ToPickup( (ENUM_ITEM_ID)BlockID, E_ITEM_EMPTY) , 1 ) );
						Pickup->Initialize( m_World );
					}
				}
				break;
			case E_BLOCK_REDSTONE_TORCH_OFF:
			case E_BLOCK_REDSTONE_TORCH_ON:
					isRedstone = true;
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

						cPickup* Pickup = new cPickup( WorldPos.x * 32 + 16, WorldPos.y * 32 + 16, WorldPos.z * 32 + 16, cItem( cBlockToPickup::ToPickup( (ENUM_ITEM_ID)BlockID, E_ITEM_EMPTY) , 1 ) );
						Pickup->Initialize( m_World );
					}
				}
				break;
			case E_BLOCK_LADDER:
				{
					char Dir = cLadder::MetaDataToDirection( GetNibble( m_BlockMeta, BlockPos ) );
					Vector3i WorldPos = PositionToWorldPosition( BlockPos );
					Vector3i AttachedTo = WorldPos;
					AddDirection( AttachedTo.x, AttachedTo.y, AttachedTo.z, Dir, true );
					if( m_World->GetBlock( AttachedTo ) == E_BLOCK_AIR )
					{
						SetBlock( BlockPos, E_BLOCK_AIR, 0 );
						cPickup* Pickup = new cPickup( WorldPos.x * 32 + 16, WorldPos.y * 32 + 16, WorldPos.z * 32 + 16,  cItem( (ENUM_ITEM_ID)BlockID, 1 ) );
						Pickup->Initialize( m_World );
					}
				}
				break;
			default:
				break;
			};
		}
	}

	// Tick dem blocks
	int RandomX = a_TickRandom.randInt();
	int RandomY = a_TickRandom.randInt();
	int RandomZ = a_TickRandom.randInt();

	for(int i = 0; i < 50; i++)
	{
		m_BlockTickX = (m_BlockTickX + RandomX) % Width;
		m_BlockTickY = (m_BlockTickY + RandomY) % Height;
		m_BlockTickZ = (m_BlockTickZ + RandomZ) % Width;

		if( m_BlockTickY > m_HeightMap[ m_BlockTickX + m_BlockTickZ * Width ] ) continue; // It's all air up here

		unsigned int Index = MakeIndexNoCheck( m_BlockTickX, m_BlockTickY, m_BlockTickZ );
		char ID = m_BlockTypes[Index];
		switch( ID )
		{
			/*
			// TODO: re-enable
			case E_BLOCK_DIRT:
			{
				char AboveBlock = GetBlock( Index+1 );
				if ( (AboveBlock == 0) && GetNibble( m_BlockSkyLight, Index ) > 0xf/2 ) // Half lit //changed to not allow grass if any one hit object is on top
				{
					FastSetBlock( m_BlockTickX, m_BlockTickY, m_BlockTickZ, E_BLOCK_GRASS, GetNibble( m_BlockMeta, Index ) );
				}
				if ( (g_BlockOneHitDig[AboveBlock]) && GetNibble( m_BlockSkyLight, Index+1 ) > 0xf/2 ) // Half lit //ch$
				{
					FastSetBlock( m_BlockTickX, m_BlockTickY, m_BlockTickZ, E_BLOCK_GRASS, GetNibble( m_BlockMeta, Index ) );
				}

				break;
			}
			*/
			
			case E_BLOCK_GRASS:
			{
#if AXIS_ORDER == AXIS_ORDER_YZX
				char AboveBlock = GetBlock( Index+1 );
#elif AXIS_ORDER == AXIS_ORDER_XZY
				char AboveBlock = GetBlock( Index + (Width*Width) );
#endif
				if (!( (AboveBlock == 0) || (g_BlockOneHitDig[AboveBlock]) || (g_BlockTransparent[AboveBlock]) ) ) //changed to not allow grass if any one hit object is on top
				{
					FastSetBlock( m_BlockTickX, m_BlockTickY, m_BlockTickZ, E_BLOCK_DIRT, GetNibble( m_BlockMeta, Index ) );
				}
			}
			break;
		case E_BLOCK_SAPLING: //todo: check meta of sapling. change m_World->GrowTree to look change trunk and leaves based on meta of sapling
			{
				FastSetBlock( m_BlockTickX, m_BlockTickY, m_BlockTickZ, E_BLOCK_AIR, GetNibble( m_BlockMeta, Index ) );
				m_World->GrowTree( m_BlockTickX + m_PosX*Width, m_BlockTickY, m_BlockTickZ + m_PosZ*Width );
			}
			break;
		case E_BLOCK_LEAVES: //todo, http://www.minecraftwiki.net/wiki/Data_values#Leaves
			{
			}
			break;
		default:
			break;
		}
	}

	// Tick block entities (furnaces)
	for (cBlockEntityList::iterator itr = m_BlockEntities.begin(); itr != m_BlockEntities.end(); ++itr)
	{
		if ((*itr)->GetBlockType() == E_BLOCK_FURNACE)
		{
			((cFurnaceEntity *)(*itr))->Tick( a_Dt );
		}
	}
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





void cChunk::CalculateLighting()
{
	// Calculate sunlight
	memset(m_BlockSkyLight, 0xff, NumBlocks / 2 ); // Set all to fully lit, so everything above HeightMap is lit
	for (int x = 0; x < Width; x++)
	{
		for (int z = 0; z < Width; z++)
		{
			char sunlight = 0xf;
			for (int y = m_HeightMap[x + z*Width]; y > -1; y--)
			{
				int index = MakeIndexNoCheck(x, y, z);

				if( g_BlockTransparent[ (int)m_BlockTypes[index] ] == false )
				{
					sunlight = 0x0;
				}
				SetNibble( m_BlockSkyLight, index, sunlight );
			}
		}
	}

	// Calculate blocklights
	for (int x = 0; x < Width; x++)
	{
		for (int z = 0; z < Width; z++)
		{
			int MaxHeight = m_HeightMap[x + z * Width];
			for (int y = 0; y < MaxHeight; y++)
			{
				char BlockID = GetBlock(x, y, z);
				SetNibble( m_BlockLight, x, y, z, g_BlockLightValue[(int)BlockID] );
			}
		}
	}

	SpreadLight(m_BlockSkyLight);
	SpreadLight(m_BlockLight);

	MarkDirty();

	// Stop it from calculating again :P
	m_bCalculateLighting = false;
}





void cChunk::SpreadLight(BLOCKTYPE * a_LightBuffer)
{
	// Spread the light
	for(int x = 0; x < Width; x++)	for(int z = 0; z < Width; z++)	for(int y = 0; y < Height; y++)
	{
		int index = MakeIndexNoCheck(x, y, z);
		if( g_BlockSpreadLightFalloff[ m_BlockTypes[index] ] > 0 )
		{
			SpreadLightOfBlock(a_LightBuffer, x, y, z, g_BlockSpreadLightFalloff[ m_BlockTypes[index] ]);
		}
	}

	for(int x = Width-1; x > -1; x--) for(int z = Width-1; z > -1; z--) for(int y = Height-1; y > -1; y--)
	{
		int index = MakeIndexNoCheck(x, y, z);
		if( g_BlockSpreadLightFalloff[ m_BlockTypes[index] ] > 0 )
		{
			SpreadLightOfBlock(a_LightBuffer, x, y, z, g_BlockSpreadLightFalloff[ m_BlockTypes[index] ]);
		}
	}

	bool bCalcLeft, bCalcRight, bCalcFront, bCalcBack;
	bCalcLeft = bCalcRight = bCalcFront = bCalcBack = false;
	
	// Spread to neighbour chunks X-axis
	cChunkPtr LeftChunk  = m_ChunkMap->GetChunkNoGen( m_PosX - 1, m_PosY, m_PosZ );
	cChunkPtr RightChunk = m_ChunkMap->GetChunkNoGen( m_PosX + 1, m_PosY, m_PosZ );
	BLOCKTYPE * LeftSky = NULL, *RightSky = NULL;
	if (LeftChunk->IsValid())
	{
		LeftSky = (a_LightBuffer == m_BlockSkyLight) ? LeftChunk->m_BlockSkyLight : LeftChunk->m_BlockLight;
	}
	if (RightChunk->IsValid())
	{
		RightSky = (a_LightBuffer == m_BlockSkyLight) ? RightChunk->m_BlockSkyLight : RightChunk->m_BlockLight;
	}
	
	for (int z = 0; z < Width; z++) for(int y = 0; y < Height; y++)
	{
		if (LeftSky != NULL)
		{
			int index = MakeIndexNoCheck( 0, y, z );
			if( g_BlockSpreadLightFalloff[ m_BlockTypes[index] ] > 0 )
			{
				BLOCKTYPE CurrentLight = GetNibble( a_LightBuffer, 0, y, z );
				BLOCKTYPE LeftLight = GetNibble( LeftSky, Width-1, y, z );
				if( LeftLight < CurrentLight-g_BlockSpreadLightFalloff[ m_BlockTypes[index] ] )
				{
					SetNibble( LeftSky, Width - 1, y, z, MAX(0, CurrentLight-g_BlockSpreadLightFalloff[ m_BlockTypes[index] ]) );
					bCalcLeft = true;
				}
			}
		}
		if (RightSky != NULL)
		{
			int index = MakeIndexNoCheck( Width - 1, y, z );
			if( g_BlockSpreadLightFalloff[ m_BlockTypes[index] ] > 0 )
			{
				BLOCKTYPE CurrentLight = GetNibble( a_LightBuffer, Width-1, y, z );
				BLOCKTYPE RightLight = GetNibble( RightSky, 0, y, z );
				if( RightLight < CurrentLight-g_BlockSpreadLightFalloff[ m_BlockTypes[index] ] )
				{
					SetNibble( RightSky, 0, y, z,  MAX(0, CurrentLight-g_BlockSpreadLightFalloff[ m_BlockTypes[index] ]) );
					bCalcRight = true;
				}
			}
		}
	}

	// Spread to neighbour chunks Z-axis
	cChunkPtr FrontChunk = m_ChunkMap->GetChunkNoGen( m_PosX, m_PosY, m_PosZ - 1 );
	cChunkPtr BackChunk  = m_ChunkMap->GetChunkNoGen( m_PosX, m_PosY, m_PosZ + 1 );
	BLOCKTYPE * FrontSky = NULL, * BackSky = NULL;
	if (FrontChunk->IsValid())
	{
		FrontSky = (a_LightBuffer == m_BlockSkyLight) ? FrontChunk->m_BlockSkyLight : FrontChunk->m_BlockLight;
	}
	if (BackChunk->IsValid())
	{
		BackSky = (a_LightBuffer == m_BlockSkyLight) ? BackChunk->m_BlockSkyLight : BackChunk->m_BlockLight;
	}
	for(int x = 0; x < Width; x++)	for(int y = 0; y < Height; y++)
	{
		if (FrontSky != NULL)
		{
			int index = MakeIndexNoCheck( x, y, 0 );
			if( g_BlockSpreadLightFalloff[ m_BlockTypes[index] ] > 0 )
			{
				BLOCKTYPE CurrentLight = GetNibble( a_LightBuffer, x, y, 0 );
				BLOCKTYPE FrontLight = GetNibble( FrontSky, x, y, Width-1 );
				if( FrontLight < CurrentLight-g_BlockSpreadLightFalloff[ m_BlockTypes[index] ] )
				{
					SetNibble( FrontSky, x, y, Width-1,  MAX(0, CurrentLight-g_BlockSpreadLightFalloff[ m_BlockTypes[index] ]) );
					bCalcFront = true;
				}
			}
		}
		if (BackSky != NULL)
		{
			int index = MakeIndexNoCheck( x, y, Width-1 );
			if( g_BlockSpreadLightFalloff[ m_BlockTypes[index] ] > 0 )
			{
				BLOCKTYPE CurrentLight = GetNibble( a_LightBuffer, x, y, Width-1 );
				BLOCKTYPE BackLight = GetNibble( BackSky, x, y, 0 );
				if ( BackLight < CurrentLight-g_BlockSpreadLightFalloff[ m_BlockTypes[index] ] )
				{
					SetNibble( BackSky, x, y, 0, MAX(0, CurrentLight-g_BlockSpreadLightFalloff[ m_BlockTypes[index] ]) );
					bCalcBack = true;
				}
			}
		}
	}

	if ( bCalcLeft )  m_World->ReSpreadLighting( m_PosX - 1, m_PosY, m_PosZ );
	if ( bCalcRight ) m_World->ReSpreadLighting( m_PosX + 1, m_PosY, m_PosZ );
	if ( bCalcFront ) m_World->ReSpreadLighting( m_PosX, m_PosY, m_PosZ - 1 );
	if ( bCalcBack )  m_World->ReSpreadLighting( m_PosX, m_PosY, m_PosZ + 1 );
	// No need to set those neighbors dirty, they will recalc their light anyway so they'll get marked dirty there
}





void cChunk::SetBlock( int a_X, int a_Y, int a_Z, BLOCKTYPE a_BlockType, BLOCKTYPE a_BlockMeta )
{
	if (a_X < 0 || a_X >= Width || a_Y < 0 || a_Y >= Height || a_Z < 0 || a_Z >= Width)
	{
		return;  // Clip
	}

	ASSERT(IsValid());  // Is this chunk loaded / generated?
	
	int index = MakeIndexNoCheck( a_X, a_Y, a_Z );
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
		RecalculateLighting();
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
				if (m_BlockTypes[MakeIndex(a_X, y, a_Z)] != E_BLOCK_AIR)
				{
					m_HeightMap[a_X + a_Z * Width] = (unsigned char)y;
					break;
				}
			}  // for y - column in m_BlockData
		}
	}

	m_ToTickBlocks.push_back( MakeIndex( a_X, a_Y, a_Z ) );
	m_ToTickBlocks.push_back( MakeIndex( a_X+1, a_Y, a_Z ) );
	m_ToTickBlocks.push_back( MakeIndex( a_X-1, a_Y, a_Z ) );
	m_ToTickBlocks.push_back( MakeIndex( a_X, a_Y+1, a_Z ) );
	m_ToTickBlocks.push_back( MakeIndex( a_X, a_Y-1, a_Z ) );
	m_ToTickBlocks.push_back( MakeIndex( a_X, a_Y, a_Z+1 ) );
	m_ToTickBlocks.push_back( MakeIndex( a_X, a_Y, a_Z-1 ) );

	Vector3i WorldPos = PositionToWorldPosition( a_X, a_Y, a_Z );
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
		RecalculateLighting();
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
	m_LoadedByClient.remove( a_Client );

	if ( !a_Client->IsDestroyed() )
	{
		for (cEntityList::iterator itr = m_Entities.begin(); itr != m_Entities.end(); ++itr )
		{
			LOG("chunk [%i, %i] destroying entity #%i for player \"%s\"", m_PosX, m_PosZ, (*itr)->GetUniqueID(), a_Client->GetUsername().c_str() );
			cPacket_DestroyEntity DestroyEntity( *itr );
			a_Client->Send( DestroyEntity );
		}
	}
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




