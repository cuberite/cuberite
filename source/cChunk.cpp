
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
	cChunkMap::AbsoluteToRelative(x, y, z, ChunkX, ChunkZ);
}





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cChunk:

cChunk::cChunk(int a_X, int a_Y, int a_Z, cChunkMap * a_ChunkMap, cWorld * a_World)
	: m_bCalculateLighting( false )
	, m_PosX( a_X )
	, m_PosY( a_Y )
	, m_PosZ( a_Z )
	, m_BlockType( m_BlockData ) // Offset the pointers
	, m_BlockMeta( m_BlockType + c_NumBlocks )
	, m_BlockLight( m_BlockMeta + c_NumBlocks / 2 )
	, m_BlockSkyLight( m_BlockLight + c_NumBlocks / 2 )
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
		if ((*itr)->GetEntityType() != cEntity::E_PLAYER)
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





void cChunk::GetAllData(cChunkDataCallback * a_Callback)
{
	a_Callback->BlockData(m_BlockData);
	
	for (cEntityList::iterator itr = m_Entities.begin(); itr != m_Entities.end(); ++itr)
	{
		a_Callback->Entity(*itr);
	}
	
	for (cBlockEntityList::iterator itr = m_BlockEntities.begin(); itr != m_BlockEntities.end(); ++itr)
	{
		a_Callback->BlockEntity(*itr);
	}
}





void cChunk::SetAllData(const char * a_BlockData, cEntityList & a_Entities, cBlockEntityList & a_BlockEntities)
{
	memcpy(m_BlockData, a_BlockData, sizeof(m_BlockData));

	// Clear the internal entities:
	for (cEntityList::iterator itr = m_Entities.begin(); itr != m_Entities.end(); ++itr)
	{
		if ((*itr)->GetEntityType() == cEntity::E_PLAYER)
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

			// Assert because this is a very curious case. These lines were executed once before, when a player died, re spawned, and walked around a bit. It's uncertain why an entity would be in the chunk in this case.
			ASSERT(!"Unexpected entity in chunk!");

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

	CalculateHeightmap();
	
	m_HasLoadFailed = false;
}





/// Copies m_BlockData into a_Blocks, only the block types
void cChunk::GetBlocks(char * a_Blocks)
{
	memcpy(a_Blocks, m_BlockData, cChunk::c_NumBlocks);
}





/// Copies m_BlockData into a_Blocks, only the block types
void cChunk::GetBlockData(char * a_BlockData)
{
	memcpy(a_BlockData, m_BlockData, cChunk::c_BlockDataSize);
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
			unsigned int Y = index % c_ChunkHeight;
			unsigned int Z = (index / c_ChunkHeight) % c_ChunkWidth;
			unsigned int X = (index / (c_ChunkHeight*c_ChunkWidth));

#if (MINECRAFT_1_2_2 == 1)
			unsigned int Coords = Y | Z << 8 | X << 12;
			unsigned int Blocks = GetNibble( m_BlockMeta, index ) | (m_BlockType[index]<<4);
			MultiBlock.m_Data[i].Data = Coords << 16 | Blocks;
#else
			MultiBlock.m_BlockCoordinates[i] = (Z&0xf) | (X&0xf)<<4 | (Y&0xff)<<8;
			//LOG("X: %i Y: %i Z: %i Combo: 0x%04x", X, Y, Z, MultiBlock.m_BlockCoordinates[i] );
			MultiBlock.m_BlockTypes[i] = m_BlockType[index];
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
			int Y = index % c_ChunkHeight;
			int Z = (index / c_ChunkHeight) % c_ChunkWidth;
			int X = (index / (c_ChunkHeight*c_ChunkWidth));

			cPacket_BlockChange BlockChange;
			BlockChange.m_PosX = X + m_PosX*c_ChunkWidth;
			BlockChange.m_PosY = (char)(Y + m_PosY*c_ChunkHeight);
			BlockChange.m_PosZ = Z + m_PosZ*c_ChunkWidth;
			BlockChange.m_BlockType = m_BlockType[index];
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
			int Y = index % c_ChunkHeight;
			int Z = (index / c_ChunkHeight) % c_ChunkWidth;
			int X = (index / (c_ChunkHeight*c_ChunkWidth));

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
					if( GetBlock( X, Y-1, Z ) == E_BLOCK_AIR )
					{
						SetBlock( X, Y, Z, E_BLOCK_AIR, 0 );

						int wX, wY, wZ;
						PositionToWorldPosition(X, Y, Z, wX, wY, wZ);

						m_World->GetSimulatorManager()->WakeUp(wX, wY, wZ);
						if (isRedstone) {
							cRedstone Redstone(m_World);
							Redstone.ChangeRedstone( (X+m_PosX*c_ChunkWidth), (Y+m_PosY*c_ChunkWidth), (Z+m_PosZ*c_ChunkWidth), false );
						}
						cPickup* Pickup = new cPickup( (X+m_PosX*c_ChunkWidth) * 32 + 16, (Y+m_PosY*c_ChunkHeight) * 32 + 16, (Z+m_PosZ*c_ChunkWidth) * 32 + 16, cItem( cBlockToPickup::ToPickup( (ENUM_ITEM_ID)BlockID, E_ITEM_EMPTY) , 1 ) );
						Pickup->Initialize( m_World );
					}
				}
				break;
			case E_BLOCK_REDSTONE_TORCH_OFF:
			case E_BLOCK_REDSTONE_TORCH_ON:
					isRedstone = true;
			case E_BLOCK_TORCH:
				{
					char Dir = cTorch::MetaDataToDirection( GetNibble( m_BlockMeta, X, Y, Z ) );
					LOG("MetaData: %i", Dir );
					int XX = X + m_PosX*c_ChunkWidth;
					int YY = Y + m_PosY*c_ChunkHeight;
					int ZZ = Z + m_PosZ*c_ChunkWidth;
					AddDirection( XX, YY, ZZ, Dir, true );
					if( m_World->GetBlock( XX, YY, ZZ ) == E_BLOCK_AIR )
					{
						SetBlock( X, Y, Z, 0, 0 );

						Vector3i wPos = PositionToWorldPosition( Vector3i(X, Y, Z) );

						m_World->GetSimulatorManager()->WakeUp(wPos.x, wPos.y, wPos.z);
						if (isRedstone) {
							cRedstone Redstone(m_World);
							Redstone.ChangeRedstone( (X+m_PosX*c_ChunkWidth), (Y+m_PosY*c_ChunkWidth), (Z+m_PosZ*c_ChunkWidth), false );
						}
						cPickup* Pickup = new cPickup( (X+m_PosX*c_ChunkWidth) * 32 + 16, (Y+m_PosY*c_ChunkHeight) * 32 + 16, (Z+m_PosZ*c_ChunkWidth) * 32 + 16, cItem( cBlockToPickup::ToPickup( (ENUM_ITEM_ID)BlockID, E_ITEM_EMPTY) , 1 ) );
						Pickup->Initialize( m_World );
					}
				}
				break;
			case E_BLOCK_LADDER:
				{
					char Dir = cLadder::MetaDataToDirection( GetNibble( m_BlockMeta, X, Y, Z ) );
					int XX = X + m_PosX*c_ChunkWidth;
					int YY = Y + m_PosY*c_ChunkHeight;
					int ZZ = Z + m_PosZ*c_ChunkWidth;
					AddDirection( XX, YY, ZZ, Dir, true );
					if( m_World->GetBlock( XX, YY, ZZ ) == E_BLOCK_AIR )
					{
						SetBlock( X, Y, Z, E_BLOCK_AIR, 0 );
						cPickup* Pickup = new cPickup( (X+m_PosX*c_ChunkWidth) * 32 + 16, (Y+m_PosY*c_ChunkHeight) * 32 + 16, (Z+m_PosZ*c_ChunkWidth) * 32 + 16,  cItem( (ENUM_ITEM_ID)BlockID, 1 ) );
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
		m_BlockTickX = (m_BlockTickX + RandomX) % c_ChunkWidth;
		m_BlockTickY = (m_BlockTickY + RandomY) % c_ChunkHeight;
		m_BlockTickZ = (m_BlockTickZ + RandomZ) % c_ChunkWidth;

		//LOG("%03i %03i %03i", m_BlockTickX, m_BlockTickY, m_BlockTickZ);

		if( m_BlockTickY > m_HeightMap[ m_BlockTickX + m_BlockTickZ*c_ChunkWidth ] ) continue; // It's all air up here

		//m_BlockTickNum = (m_BlockTickNum + 1 ) % c_NumBlocks;
		unsigned int Index = MakeIndex( m_BlockTickX, m_BlockTickY, m_BlockTickZ );
		char ID = m_BlockType[Index];
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
				char AboveBlock = GetBlock( Index+1 );
				if (!( (AboveBlock == 0) || (g_BlockOneHitDig[AboveBlock]) || (g_BlockTransparent[AboveBlock]) ) ) //changed to not allow grass if any one hit object is on top
				{
					FastSetBlock( m_BlockTickX, m_BlockTickY, m_BlockTickZ, E_BLOCK_DIRT, GetNibble( m_BlockMeta, Index ) );
				}
			}
			break;
		case E_BLOCK_SAPLING: //todo: check meta of sapling. change m_World->GrowTree to look change trunk and leaves based on meta of sapling
			{
				FastSetBlock( m_BlockTickX, m_BlockTickY, m_BlockTickZ, E_BLOCK_AIR, GetNibble( m_BlockMeta, Index ) );
				m_World->GrowTree( m_BlockTickX + m_PosX*c_ChunkWidth, m_BlockTickY, m_BlockTickZ + m_PosZ*c_ChunkWidth );
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
	ASSERT((a_X >= 0) && (a_X < c_ChunkWidth) && (a_Z >= 0) && (a_Z < c_ChunkWidth));
	
	if ((a_X >= 0) && (a_X < c_ChunkWidth) && (a_Z >= 0) && (a_Z < c_ChunkWidth))
	{
		return m_HeightMap[a_X + a_Z * c_ChunkWidth];
	}
	return 0;
}





void cChunk::CreateBlockEntities(void)
{
	for (int x = 0; x < c_ChunkWidth; x++)
	{
		for (int z = 0; z < c_ChunkWidth; z++)
		{
			for (int y = 0; y < c_ChunkHeight; y++)
			{
				ENUM_BLOCK_ID BlockType = (ENUM_BLOCK_ID)m_BlockData[ MakeIndex( x, y, z ) ];
				switch ( BlockType )
				{
					case E_BLOCK_CHEST:
					{
						if (!HasBlockEntityAt(x + m_PosX * c_ChunkWidth, y + m_PosY * c_ChunkHeight, z + m_PosZ * c_ChunkWidth))
						{
							m_BlockEntities.push_back( new cChestEntity( x + m_PosX * c_ChunkWidth, y + m_PosY * c_ChunkHeight, z + m_PosZ * c_ChunkWidth, m_World) );
						}
						break;
					}
					
					case E_BLOCK_FURNACE:
					{
						if (!HasBlockEntityAt(x + m_PosX * c_ChunkWidth, y + m_PosY * c_ChunkHeight, z + m_PosZ * c_ChunkWidth))
						{
							m_BlockEntities.push_back( new cFurnaceEntity( x + m_PosX * c_ChunkWidth, y + m_PosY * c_ChunkHeight, z + m_PosZ * c_ChunkWidth, m_World) );
						}
						break;
					}
					
					case E_BLOCK_SIGN_POST:
					case E_BLOCK_WALLSIGN:
					{
						if (!HasBlockEntityAt(x + m_PosX * c_ChunkWidth, y + m_PosY * c_ChunkHeight, z + m_PosZ * c_ChunkWidth))
						{
							m_BlockEntities.push_back( new cSignEntity( BlockType, x + m_PosX * c_ChunkWidth, y + m_PosY * c_ChunkHeight, z + m_PosZ * c_ChunkWidth, m_World) );
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
	for (int x = 0; x < c_ChunkWidth; x++)
	{
		for (int z = 0; z < c_ChunkWidth; z++)
		{
			for (int y = c_ChunkHeight-1; y > -1; y--)
			{
				int index = MakeIndex( x, y, z );
				if (m_BlockData[index] != E_BLOCK_AIR)
				{
					m_HeightMap[x + z * c_ChunkWidth] = (char)y;
					break;
				}
			}  // for y
		}  // for z
	}  // for x
}





void cChunk::CalculateLighting()
{
	// Calculate sunlight
	memset(m_BlockSkyLight, 0xff, c_NumBlocks / 2 ); // Set all to fully lit, so everything above HeightMap is lit
	for (int x = 0; x < c_ChunkWidth; x++)
	{
		for (int z = 0; z < c_ChunkWidth; z++)
		{
			char sunlight = 0xf;
			for (int y = m_HeightMap[x + z*c_ChunkWidth]; y > -1; y--)
			{
				int index = y + (z * c_ChunkHeight) + (x * c_ChunkHeight * c_ChunkWidth);

				if( g_BlockTransparent[ (int)m_BlockData[index] ] == false )
				{
					sunlight = 0x0;
				}
				SetNibble( m_BlockSkyLight, x, y, z, sunlight );
			}
		}
	}

	// Calculate blocklights
	for (int x = 0; x < c_ChunkWidth; x++)
	{
		for (int z = 0; z < c_ChunkWidth; z++)
		{
			int MaxHeight = m_HeightMap[x + z*c_ChunkWidth];
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





void cChunk::SpreadLight(char* a_LightBuffer)
{
	// Spread the sunlight
	for(int x = 0; x < c_ChunkWidth; x++)	for(int z = 0; z < c_ChunkWidth; z++)	for(int y = 0; y < c_ChunkHeight; y++)
	{
		int index = y + (z * c_ChunkHeight) + (x * c_ChunkHeight * c_ChunkWidth);
		if( g_BlockSpreadLightFalloff[ m_BlockData[index] ] > 0 )
		{
			SpreadLightOfBlock(a_LightBuffer, x, y, z, g_BlockSpreadLightFalloff[ m_BlockData[index] ]);
		}
	}

	for(int x = c_ChunkWidth-1; x > -1; x--) for(int z = c_ChunkWidth-1; z > -1; z--) for(int y = c_ChunkHeight-1; y > -1; y--)
	{
		int index = y + (z * c_ChunkHeight) + (x * c_ChunkHeight * c_ChunkWidth);
		if( g_BlockSpreadLightFalloff[ m_BlockData[index] ] > 0 )
		{
			SpreadLightOfBlock(a_LightBuffer, x, y, z, g_BlockSpreadLightFalloff[ m_BlockData[index] ]);
		}
	}

	bool bCalcLeft, bCalcRight, bCalcFront, bCalcBack;
	bCalcLeft = bCalcRight = bCalcFront = bCalcBack = false;
	
	// Spread to neighbour chunks X-axis
	cChunkPtr LeftChunk  = m_ChunkMap->GetChunkNoGen( m_PosX - 1, m_PosY, m_PosZ );
	cChunkPtr RightChunk = m_ChunkMap->GetChunkNoGen( m_PosX + 1, m_PosY, m_PosZ );
	char * LeftSky = NULL, *RightSky = NULL;
	if (LeftChunk->IsValid())
	{
		LeftSky = (a_LightBuffer == m_BlockSkyLight) ? LeftChunk->pGetSkyLight() : LeftChunk->pGetLight();
	}
	if (RightChunk->IsValid())
	{
		RightSky = (a_LightBuffer == m_BlockSkyLight) ? RightChunk->pGetSkyLight() : RightChunk->pGetLight();
	}
	
	for (int z = 0; z < c_ChunkWidth; z++) for(int y = 0; y < c_ChunkHeight; y++)
	{
		if (LeftSky != NULL)
		{
			int index = y + (z * c_ChunkHeight) + (0  * c_ChunkHeight * c_ChunkWidth);
			if( g_BlockSpreadLightFalloff[ m_BlockData[index] ] > 0 )
			{
				char CurrentLight = GetNibble( a_LightBuffer, 0, y, z );
				char LeftLight = GetNibble( LeftSky, c_ChunkWidth-1, y, z );
				if( LeftLight < CurrentLight-g_BlockSpreadLightFalloff[ m_BlockData[index] ] )
				{
					SetNibble( LeftSky, c_ChunkWidth - 1, y, z, MAX(0, CurrentLight-g_BlockSpreadLightFalloff[ m_BlockData[index] ]) );
					bCalcLeft = true;
				}
			}
		}
		if (RightSky != NULL)
		{
			int index = y + (z * c_ChunkHeight) + ((c_ChunkWidth-1)  * c_ChunkHeight * c_ChunkWidth);
			if( g_BlockSpreadLightFalloff[ m_BlockData[index] ] > 0 )
			{
				char CurrentLight = GetNibble( a_LightBuffer, c_ChunkWidth-1, y, z );
				char RightLight = GetNibble( RightSky, 0, y, z );
				if( RightLight < CurrentLight-g_BlockSpreadLightFalloff[ m_BlockData[index] ] )
				{
					SetNibble( RightSky, 0, y, z,  MAX(0, CurrentLight-g_BlockSpreadLightFalloff[ m_BlockData[index] ]) );
					bCalcRight = true;
				}
			}
		}
	}

	// Spread to neighbour chunks Z-axis
	cChunkPtr FrontChunk = m_ChunkMap->GetChunkNoGen( m_PosX, m_PosY, m_PosZ - 1 );
	cChunkPtr BackChunk  = m_ChunkMap->GetChunkNoGen( m_PosX, m_PosY, m_PosZ + 1 );
	char * FrontSky = NULL, * BackSky = NULL;
	if (FrontChunk->IsValid())
	{
		FrontSky = (a_LightBuffer == m_BlockSkyLight) ? FrontChunk->pGetSkyLight() : FrontChunk->pGetLight();
	}
	if (BackChunk->IsValid())
	{
		BackSky = (a_LightBuffer == m_BlockSkyLight) ? BackChunk->pGetSkyLight() : BackChunk->pGetLight();
	}
	for(int x = 0; x < c_ChunkWidth; x++)	for(int y = 0; y < c_ChunkHeight; y++)
	{
		if (FrontSky != NULL)
		{
			int index = y + (0 * c_ChunkHeight) + (x  * c_ChunkHeight * c_ChunkWidth);
			if( g_BlockSpreadLightFalloff[ m_BlockData[index] ] > 0 )
			{
				char CurrentLight = GetNibble( a_LightBuffer, x, y, 0 );
				char FrontLight = GetNibble( FrontSky, x, y, c_ChunkWidth-1 );
				if( FrontLight < CurrentLight-g_BlockSpreadLightFalloff[ m_BlockData[index] ] )
				{
					SetNibble( FrontSky, x, y, c_ChunkWidth-1,  MAX(0, CurrentLight-g_BlockSpreadLightFalloff[ m_BlockData[index] ]) );
					bCalcFront = true;
				}
			}
		}
		if (BackSky != NULL)
		{
			int index = y + ((c_ChunkWidth-1) * c_ChunkHeight) + (x  * c_ChunkHeight * c_ChunkWidth);
			if( g_BlockSpreadLightFalloff[ m_BlockData[index] ] > 0 )
			{
				char CurrentLight = GetNibble( a_LightBuffer, x, y, c_ChunkWidth-1 );
				char BackLight = GetNibble( BackSky, x, y, 0 );
				if ( BackLight < CurrentLight-g_BlockSpreadLightFalloff[ m_BlockData[index] ] )
				{
					SetNibble( BackSky, x, y, 0, MAX(0, CurrentLight-g_BlockSpreadLightFalloff[ m_BlockData[index] ]) );
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





void cChunk::SetBlock( int a_X, int a_Y, int a_Z, char a_BlockType, char a_BlockMeta )
{
	if (a_X < 0 || a_X >= c_ChunkWidth || a_Y < 0 || a_Y >= c_ChunkHeight || a_Z < 0 || a_Z >= c_ChunkWidth)
	{
		return;  // Clip
	}

	ASSERT(IsValid());  // Is this chunk loaded / generated?
	
	int index = a_Y + (a_Z * c_ChunkHeight) + (a_X * c_ChunkHeight * c_ChunkWidth);
	char OldBlockMeta = GetNibble( m_BlockMeta, index );
	char OldBlockType = m_BlockType[index];
	m_BlockType[index] = a_BlockType;

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
	if (a_Y >= m_HeightMap[a_X + a_Z * c_ChunkWidth])
	{
		if (a_BlockType != E_BLOCK_AIR)
		{
			m_HeightMap[a_X + a_Z * c_ChunkWidth] = (unsigned char)a_Y;
		}
		else
		{
			for (int y = a_Y - 1; y > 0; --y)
			{
				if (m_BlockData[MakeIndex(a_X, y, a_Z)] != E_BLOCK_AIR)
				{
					m_HeightMap[a_X + a_Z * c_ChunkWidth] = (unsigned char)y;
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

	cBlockEntity* BlockEntity = GetBlockEntity( a_X + m_PosX * c_ChunkWidth, a_Y + m_PosY * c_ChunkHeight, a_Z + m_PosZ * c_ChunkWidth );
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
			AddBlockEntity( new cChestEntity( a_X + m_PosX * c_ChunkWidth, a_Y + m_PosY * c_ChunkHeight, a_Z + m_PosZ * c_ChunkWidth, m_World) );
			break;
		}
		case E_BLOCK_FURNACE:
		{
			AddBlockEntity( new cFurnaceEntity( a_X + m_PosX * c_ChunkWidth, a_Y + m_PosY * c_ChunkHeight, a_Z + m_PosZ * c_ChunkWidth, m_World) );
			break;
		}
		case E_BLOCK_SIGN_POST:
		case E_BLOCK_WALLSIGN:
		{
			AddBlockEntity( new cSignEntity( (ENUM_BLOCK_ID)a_BlockType, a_X + m_PosX * c_ChunkWidth, a_Y + m_PosY * c_ChunkHeight, a_Z + m_PosZ * c_ChunkWidth, m_World) );
			break;
		}
	}  // switch (a_BlockType)
}





void cChunk::FastSetBlock( int a_X, int a_Y, int a_Z, char a_BlockType, char a_BlockMeta)
{
	ASSERT(!((a_X < 0 || a_X >= c_ChunkWidth || a_Y < 0 || a_Y >= c_ChunkHeight || a_Z < 0 || a_Z >= c_ChunkWidth)));

	ASSERT(IsValid());
	
	const int index = a_Y + (a_Z * c_ChunkHeight) + (a_X * c_ChunkHeight * c_ChunkWidth);
	const char OldBlock = m_BlockType[index];
	const char OldBlockMeta = GetNibble( m_BlockMeta, index );
	if (OldBlock == a_BlockType && OldBlockMeta == a_BlockMeta)
	{
		return;
	}

	MarkDirty();
	
	m_BlockType[index] = a_BlockType;

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
	if (a_Y >= m_HeightMap[a_X + a_Z * c_ChunkWidth])
	{
		if (a_BlockType != E_BLOCK_AIR)
		{
			m_HeightMap[a_X + a_Z * c_ChunkWidth] = (unsigned char)a_Y;
		}
		else
		{
			for (int y = a_Y - 1; y > 0; --y)
			{
				if (m_BlockData[MakeIndex(a_X, y, a_Z)] != E_BLOCK_AIR)
				{
					m_HeightMap[a_X + a_Z * c_ChunkWidth] = (unsigned char)y;
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
			m_PendingSendBlocks.push_back( MakeIndex( a_X, a_Y, a_Z ) );
		}
		return;
	}

	for (cClientHandleList::iterator itr = m_LoadedByClient.begin(); itr != m_LoadedByClient.end(); ++itr )
	{
		if ( *itr == a_Client )
		{
			unsigned int index = MakeIndex( a_X, a_Y, a_Z );
			cPacket_BlockChange BlockChange;
			BlockChange.m_PosX = a_X + m_PosX*c_ChunkWidth;
			BlockChange.m_PosY = (unsigned char)(a_Y + m_PosY*c_ChunkHeight);
			BlockChange.m_PosZ = a_Z + m_PosZ*c_ChunkWidth;
			if( index != INDEX_OUT_OF_RANGE )
			{
				BlockChange.m_BlockType = m_BlockType[ index ];
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
		if ( (*itr)->GetEntityType() != cEntity::E_PICKUP )
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
	if (a_Entity->GetEntityType() != cEntity::E_PLAYER)
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
		if (a_Entity->GetEntityType() != cEntity::E_PLAYER)
		{
			MarkDirty();
		}
	}
}





char cChunk::GetBlock( int a_X, int a_Y, int a_Z )
{
	if ((a_X < 0) || (a_X >= c_ChunkWidth) || (a_Y < 0) || (a_Y >= c_ChunkHeight) || (a_Z < 0) || (a_Z >= c_ChunkWidth)) return 0; // Clip

	int index = a_Y + (a_Z * c_ChunkHeight) + (a_X * c_ChunkHeight * c_ChunkWidth);
	return m_BlockType[index];
}





char cChunk::GetBlock( int a_BlockIdx )
{
	if( a_BlockIdx < 0 || a_BlockIdx >= c_NumBlocks ) return 0;
	return m_BlockType[ a_BlockIdx ];
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
		(*itr)->Send( a_Packet );
	}  // for itr - LoadedByClient[]
}





void cChunk::CopyBlockDataFrom(const char * a_NewBlockData)
{
	// Copies all blockdata, recalculates heightmap (used by chunk loaders)
	memcpy(m_BlockData, a_NewBlockData, sizeof(m_BlockData));
	CalculateHeightmap();
}





void cChunk::PositionToWorldPosition(int a_ChunkX, int a_ChunkY, int a_ChunkZ, int & a_X, int & a_Y, int & a_Z)
{
	a_Y = a_ChunkY;
	a_X = m_PosX * c_ChunkWidth + a_ChunkX;
	a_Z = m_PosZ * c_ChunkWidth + a_ChunkZ;
}





Vector3i cChunk::PositionToWorldPosition( const Vector3i & a_InChunkPos )
{
	return Vector3i( m_PosX * c_ChunkWidth + a_InChunkPos.x, m_PosY * c_ChunkHeight + a_InChunkPos.y, m_PosZ * c_ChunkWidth + a_InChunkPos.z );
}





#if !C_CHUNK_USE_INLINE
# include "cChunk.inc"
#endif




