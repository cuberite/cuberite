#ifndef _WIN32
#include <cstring>
#include <cstdlib>
#include <stdio.h>
#include <sys/stat.h>   // for mkdir
#include <sys/types.h>
#endif


#include "cChunk.h"
#include "cWorld.h"
#include "cWaterSimulator.h"
#include "cLavaSimulator.h"
#include "cClientHandle.h"
#include "cServer.h"
#include "zlib.h"
#include "Defines.h"
#include <string> // memset
#include "cChestEntity.h"
#include "cFurnaceEntity.h"
#include "cSignEntity.h"
#include "cMCLogger.h"
#include "cTorch.h"
#include "cLadder.h"
#include "cPickup.h"
#include "cRedstone.h"
#include "cItem.h"
#include "cNoise.h"
#include "cRoot.h"
#include "cCriticalSection.h"
#include "cWorldGenerator.h"
#include "cBlockToPickup.h"
#include "MersenneTwister.h"

#include "packets/cPacket_DestroyEntity.h"
#include "packets/cPacket_PreChunk.h"
#include "packets/cPacket_BlockChange.h"
#include "packets/cPacket_MapChunk.h"
#include "packets/cPacket_MultiBlock.h"

#include <json/json.h>

#include <list>
#include <vector>
#include <map>

#ifndef _WIN32
#define sprintf_s(dst, size, format, ...) sprintf(dst, format, __VA_ARGS__ )
#endif

extern bool g_bWaterPhysics;


typedef std::list< cFurnaceEntity* > FurnaceEntityList;
typedef std::list< cClientHandle* > ClientHandleList;
typedef std::list< cBlockEntity* > BlockEntityList;
typedef std::list< cEntity* > EntityList;
struct cChunk::sChunkState
{
	std::map< unsigned int, int > m_ToTickBlocks;
	FurnaceEntityList m_TickBlockEntities;
	std::vector< unsigned int > m_PendingSendBlocks;
	ClientHandleList m_LoadedByClient;
	ClientHandleList m_UnloadQuery;
	BlockEntityList m_BlockEntities;
	EntityList m_Entities;
};

cChunk::~cChunk()
{
	//LOG("~cChunk() %i %i %i", m_PosX, m_PosY, m_PosZ );
	if( !m_pState->m_LoadedByClient.empty() )
	{
		LOGWARN("WARNING: Deleting cChunk while it contains %i clients!", m_pState->m_LoadedByClient.size() );
	}

	for( std::list<cBlockEntity*>::iterator itr = m_pState->m_BlockEntities.begin(); itr != m_pState->m_BlockEntities.end(); ++itr)
	{
		delete *itr;
	}
	m_pState->m_BlockEntities.clear();

	LockEntities();
	if( m_pState->m_Entities.size() > 0 )
	{
		EntityList Entities = m_pState->m_Entities; // Copy list to a temporary list
		for( EntityList::iterator itr = Entities.begin(); itr != Entities.end(); ++itr)
		{
			if( (*itr)->GetEntityType() != cEntity::E_PLAYER )
			{
				(*itr)->RemoveFromChunk( this );
				(*itr)->Destroy();
			}
		}
		m_pState->m_Entities.clear();
	}
	UnlockEntities();

	if( m_EntitiesCriticalSection )
	{
		delete m_EntitiesCriticalSection;
		m_EntitiesCriticalSection = 0;
	}
	delete m_pState;
}

cChunk::cChunk(int a_X, int a_Y, int a_Z, cWorld* a_World)
	: m_pState( new sChunkState )
	, m_bCalculateLighting( false )
	, m_bCalculateHeightmap( false )
	, m_PosX( a_X )
	, m_PosY( a_Y )
	, m_PosZ( a_Z )
	, m_BlockType( m_BlockData ) // Offset the pointers
	, m_BlockMeta( m_BlockType + c_NumBlocks )
	, m_BlockLight( m_BlockMeta + c_NumBlocks/2 )
	, m_BlockSkyLight( m_BlockLight + c_NumBlocks/2 )
	, m_BlockTickNum( 0 )
	, m_BlockTickX( 0 )
	, m_BlockTickY( 0 )
	, m_BlockTickZ( 0 )
	, m_EntitiesCriticalSection( 0 )
	, m_World( a_World )
{
	//LOG("cChunk::cChunk(%i, %i, %i)", a_X, a_Y, a_Z);
	m_EntitiesCriticalSection = new cCriticalSection();
}

void cChunk::Initialize()
{
	if( !LoadFromDisk() )
	{
		// Clear memory
		memset( m_BlockData, 0x00, c_BlockDataSize );

		cWorldGenerator Generator;
		Generator.GenerateChunk( this );

		CalculateHeightmap();
		CalculateLighting();

		CreateBlockEntities();

		// During generation, some blocks might have been set by using (Fast)SetBlock() causing this list to fill.
		// This chunk has not been sent to anybody yet, so there is no need for separately sending block changes when you can send an entire chunk
		m_pState->m_PendingSendBlocks.clear();
	}
	else
	{
		//LOGINFO("Successfully loaded from disk");
		CalculateHeightmap();
	}
}

void cChunk::Tick(float a_Dt)
{
	if( m_bCalculateLighting )
		CalculateLighting();
	if( m_bCalculateHeightmap )
		CalculateHeightmap();

	unsigned int PendingSendBlocks = m_pState->m_PendingSendBlocks.size();
 	if( PendingSendBlocks > 1 )
 	{
 		cPacket_MultiBlock MultiBlock;
 		MultiBlock.m_ChunkX = m_PosX;
 		MultiBlock.m_ChunkZ = m_PosZ;
 		MultiBlock.m_NumBlocks = (short)PendingSendBlocks;
 		MultiBlock.m_BlockCoordinates = new unsigned short[PendingSendBlocks];
 		MultiBlock.m_BlockTypes = new char[PendingSendBlocks];
 		MultiBlock.m_BlockMetas = new char[PendingSendBlocks];
 		//LOG("Sending multiblock packet for %i blocks", PendingSendBlocks );
 		for( unsigned int i = 0; i < PendingSendBlocks; i++)
 		{
 			unsigned int index = m_pState->m_PendingSendBlocks[i];
 			unsigned int Y = index % 128;
 			unsigned int Z = (index / 128) % 16;
 			unsigned int X = (index / (128*16));

 			MultiBlock.m_BlockCoordinates[i] = (Z&0xf) | (X&0xf)<<4 | (Y&0xff)<<8;
 			//LOG("X: %i Y: %i Z: %i Combo: 0x%04x", X, Y, Z, MultiBlock.m_BlockCoordinates[i] );
 			MultiBlock.m_BlockTypes[i] = m_BlockType[index];
 			MultiBlock.m_BlockMetas[i] = GetLight( m_BlockMeta, index );
 		}
 		m_pState->m_PendingSendBlocks.clear();
 		PendingSendBlocks = m_pState->m_PendingSendBlocks.size();
 		Broadcast( MultiBlock );
 	}
	if( PendingSendBlocks > 0 )
	{
		for( unsigned int i = 0; i < PendingSendBlocks; i++)
		{
			unsigned int index = m_pState->m_PendingSendBlocks[i];
			int Y = index % 128;
			int Z = (index / 128) % 16;
			int X = (index / (128*16));

			cPacket_BlockChange BlockChange;
			BlockChange.m_PosX = X + m_PosX*16;
			BlockChange.m_PosY = (char)(Y + m_PosY*128);
			BlockChange.m_PosZ = Z + m_PosZ*16;
			BlockChange.m_BlockType = m_BlockType[index];
			BlockChange.m_BlockMeta = GetLight( m_BlockMeta, index );
			Broadcast( BlockChange );
		}
		m_pState->m_PendingSendBlocks.clear();
	}

	while( !m_pState->m_UnloadQuery.empty() )
	{
		cPacket_PreChunk UnloadPacket;
		UnloadPacket.m_PosX = GetPosX();
		UnloadPacket.m_PosZ = GetPosZ();
		UnloadPacket.m_bLoad = false; // Unload
		(*m_pState->m_UnloadQuery.begin())->Send( UnloadPacket );
		m_pState->m_UnloadQuery.remove( *m_pState->m_UnloadQuery.begin() );
	}

	std::map< unsigned int, int > ToTickBlocks = m_pState->m_ToTickBlocks;
	//unsigned int NumTickBlocks = ToTickBlocks.size();
	//if( NumTickBlocks > 0 ) LOG("To tick: %i", NumTickBlocks );
	m_pState->m_ToTickBlocks.clear();
	bool isRedstone = false;
	for( std::map< unsigned int, int>::iterator itr = ToTickBlocks.begin(); itr != ToTickBlocks.end(); ++itr )
	{
		if( (*itr).second < 0 ) continue;
		unsigned int index = (*itr).first;
		int Y = index % 128;
		int Z = (index / 128) % 16;
		int X = (index / (128*16));

		char BlockID = GetBlock( index );
		switch( BlockID )
		{
		case E_BLOCK_REDSTONE_REPEATER_OFF:
		case E_BLOCK_REDSTONE_REPEATER_ON:
		case E_BLOCK_REDSTONE_WIRE:
			{
				isRedstone = true;
			}
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

					m_World->GetWaterSimulator()->WakeUp( wX, wY, wZ );
					m_World->GetLavaSimulator()->WakeUp( wX, wY, wZ );
					if (isRedstone) {
						cRedstone Redstone(m_World);
						Redstone.ChangeRedstone( (X+m_PosX*16), (Y+m_PosY*16), (Z+m_PosZ*16), false );
					}
					cPickup* Pickup = new cPickup( (X+m_PosX*16) * 32 + 16, (Y+m_PosY*128) * 32 + 16, (Z+m_PosZ*16) * 32 + 16, cItem( cBlockToPickup::ToPickup( (ENUM_ITEM_ID)BlockID, E_ITEM_EMPTY) , 1 ) );
					Pickup->Initialize( m_World );
				}
			}
			break;
		case E_BLOCK_REDSTONE_TORCH_OFF:
		case E_BLOCK_REDSTONE_TORCH_ON:
				isRedstone = true;
		case E_BLOCK_TORCH:
			{
				char Dir = cTorch::MetaDataToDirection( GetLight( m_BlockMeta, X, Y, Z ) );
				LOG("MetaData: %i", Dir );
				int XX = X + m_PosX*16;
				char YY = (char)Y;
				int ZZ = Z + m_PosZ*16;
				AddDirection( XX, YY, ZZ, Dir, true );
				if( m_World->GetBlock( XX, YY, ZZ ) == E_BLOCK_AIR )
				{
					SetBlock( X, Y, Z, 0, 0 );
					if (isRedstone) {
						cRedstone Redstone(m_World);
						Redstone.ChangeRedstone( (X+m_PosX*16), (Y+m_PosY*16), (Z+m_PosZ*16), false );
					}
					cPickup* Pickup = new cPickup( (X+m_PosX*16) * 32 + 16, (Y+m_PosY*128) * 32 + 16, (Z+m_PosZ*16) * 32 + 16, cItem( cBlockToPickup::ToPickup( (ENUM_ITEM_ID)BlockID, E_ITEM_EMPTY) , 1 ) );
					Pickup->Initialize( m_World );
				}
			}
			break;
		case E_BLOCK_LADDER:
			{
				char Dir = cLadder::MetaDataToDirection( GetLight( m_BlockMeta, X, Y, Z ) );
				int XX = X + m_PosX*16;
				char YY = (char)Y;
				int ZZ = Z + m_PosZ*16;
				AddDirection( XX, YY, ZZ, Dir, true );
				if( m_World->GetBlock( XX, YY, ZZ ) == E_BLOCK_AIR )
				{
					SetBlock( X, Y, Z, E_BLOCK_AIR, 0 );
					cPickup* Pickup = new cPickup( (X+m_PosX*16) * 32 + 16, (Y+m_PosY*128) * 32 + 16, (Z+m_PosZ*16) * 32 + 16,  cItem( (ENUM_ITEM_ID)BlockID, 1 ) );
					Pickup->Initialize( m_World );
				}
			}
			break;
		case E_BLOCK_STATIONARY_WATER:
		case E_BLOCK_WATER:
			
			break;
		case E_BLOCK_GRAVEL:
		case E_BLOCK_SAND:
			{
				char BottomBlock = GetBlock( X, Y-1, Z );
				if( BottomBlock == E_BLOCK_AIR || IsBlockWater(BottomBlock) || IsBlockLava(BottomBlock) )
				{
					SetBlock( X, Y, Z, E_BLOCK_AIR, 0 );
					SetBlock( X, Y-1, Z, BlockID, 0 );
					
					int wX, wY, wZ;

					PositionToWorldPosition(X, Y, Z, wX, wY, wZ);

					m_World->GetWaterSimulator()->WakeUp( wX, wY, wZ );
					m_World->GetLavaSimulator()->WakeUp( wX, wY, wZ );
					m_World->GetWaterSimulator()->WakeUp( wX, wY - 1, wZ );
					m_World->GetLavaSimulator()->WakeUp( wX, wY - 1, wZ );
				}
			}
			break;
		default:
			break;
		};
	}
	MTRand r1;
	// Tick dem blocks
	int RandomX = r1.randInt();
	int RandomY = r1.randInt();
	int RandomZ = r1.randInt();

	for(int i = 0; i < 50; i++)
	{
		m_BlockTickX = (m_BlockTickX + RandomX) % 16;
		m_BlockTickY = (m_BlockTickY + RandomY) % 128;
		m_BlockTickZ = (m_BlockTickZ + RandomZ) % 16;

		//LOG("%03i %03i %03i", m_BlockTickX, m_BlockTickY, m_BlockTickZ);

		if( m_BlockTickY > m_HeightMap[ m_BlockTickX + m_BlockTickZ*16 ] ) continue; // It's all air up here

		//m_BlockTickNum = (m_BlockTickNum + 1 ) % c_NumBlocks;
		unsigned int Index = MakeIndex( m_BlockTickX, m_BlockTickY, m_BlockTickZ );
		char ID = m_BlockType[Index];
		switch( ID )
		{
		case E_BLOCK_DIRT:
			{
				char AboveBlock = GetBlock( Index+1 );
				if ( (AboveBlock == 0) && GetLight( m_BlockSkyLight, Index ) > 0xf/2 ) // Half lit //changed to not allow grass if any one hit object is on top
				{
					FastSetBlock( m_BlockTickX, m_BlockTickY, m_BlockTickZ, E_BLOCK_GRASS, GetLight( m_BlockMeta, Index ) );
				}
				if ( (g_BlockOneHitDig[AboveBlock]) && GetLight( m_BlockSkyLight, Index+1 ) > 0xf/2 ) // Half lit //ch$
				{
					FastSetBlock( m_BlockTickX, m_BlockTickY, m_BlockTickZ, E_BLOCK_GRASS, GetLight( m_BlockMeta, Index ) );
				}

			}
			break;
		case E_BLOCK_GRASS:
			{
				char AboveBlock = GetBlock( Index+1 );
				if (!( (AboveBlock == 0) || (g_BlockOneHitDig[AboveBlock]) || (g_BlockTransparent[AboveBlock]) ) ) //changed to not allow grass if any one hit object is on top
				{
					FastSetBlock( m_BlockTickX, m_BlockTickY, m_BlockTickZ, E_BLOCK_DIRT, GetLight( m_BlockMeta, Index ) );
				}
			}
			break;
		case E_BLOCK_SAPLING: //todo: check meta of sapling. change m_World->GrowTree to look change trunk and leaves based on meta of sapling
			{
				FastSetBlock( m_BlockTickX, m_BlockTickY, m_BlockTickZ, E_BLOCK_AIR, GetLight( m_BlockMeta, Index ) );
				m_World->GrowTree( m_BlockTickX + m_PosX*16, m_BlockTickY, m_BlockTickZ + m_PosZ*16 );
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

	// Tick block entities (furnace)
	std::list< cFurnaceEntity* > TickBlockEntites = m_pState->m_TickBlockEntities; // Dangerous stuff, better make a copy.
	for( std::list< cFurnaceEntity* >::iterator itr = TickBlockEntites.begin(); itr != TickBlockEntites.end(); ++itr )
	{
		if( !(*itr)->Tick( a_Dt ) ) // Remove from list
		{
			m_pState->m_TickBlockEntities.remove( *itr );
		}
	}
}

char cChunk::GetHeight( int a_X, int a_Z )
{
	if( a_X >= 0 && a_X < 16 && a_Z >= 0 && a_Z < 16 )
		return m_HeightMap[a_X + a_Z*16];
	return 0;
}

void cChunk::CreateBlockEntities()
{
	for(int x = 0; x < 16; x++)
	{
		for(int z = 0; z < 16; z++)
		{
			for(int y = 0; y < 128; y++)
			{
				ENUM_BLOCK_ID BlockType = (ENUM_BLOCK_ID)m_BlockData[ MakeIndex( x, y, z ) ];
				switch( BlockType )
				{
				case E_BLOCK_CHEST:
					{
						m_pState->m_BlockEntities.push_back( new cChestEntity( x + m_PosX*16, y + m_PosY*128, z + m_PosZ*16, this ) );
					}
					break;
				case E_BLOCK_FURNACE:
					{
						m_pState->m_BlockEntities.push_back( new cFurnaceEntity( x + m_PosX*16, y + m_PosY*128, z + m_PosZ*16, this ) );
					}
					break;
				case E_BLOCK_SIGN_POST:
				case E_BLOCK_WALLSIGN:
					{
						m_pState->m_BlockEntities.push_back( new cSignEntity( BlockType, x + m_PosX*16, y + m_PosY*128, z + m_PosZ*16, this ) );
					}
					break;
				default:
					{
					}
					break;
				}
			}
		}
	}
}

char cChunk::GetLight(char* a_Buffer, int a_BlockIdx)
{
	if( a_BlockIdx > -1 && a_BlockIdx < c_NumBlocks )
	{
		const int cindex = (a_BlockIdx/2);
		if( (a_BlockIdx & 1) == 0 )
		{	// First half byte
			return (a_Buffer[cindex] & 0x0f);
		}
		else
		{
			return ((a_Buffer[cindex] & 0xf0) >> 4);
		}
	}
	return 0;
}

char cChunk::GetLight(char* a_Buffer, int x, int y, int z)
{
	if( x < 16 && x > -1 && y < 128 && y > -1 && z < 16 && z > -1 )
	{
		const int cindex = (y/2) + (z * 64) + (x * 64 * 16);
		if( (y & 1) == 0 )
		{	// First half byte
			return (a_Buffer[cindex] & 0x0f);
		}
		else
		{
			return ((a_Buffer[cindex] & 0xf0) >> 4);
		}
	}
	return 0;
}

void cChunk::SetLight(char* a_Buffer, int a_BlockIdx, char a_Light)
{
	if( a_BlockIdx > -1 && a_BlockIdx < c_NumBlocks )
	{
		const int cindex = (a_BlockIdx/2);
		if( (a_BlockIdx & 1) == 0 )
		{	// First half byte
			a_Buffer[cindex] &= 0xf0; // Set first half to 0
			a_Buffer[cindex] |= (a_Light) & 0x0f;
		}
		else
		{
			a_Buffer[cindex] &= 0x0f; // Set second half to 0
			a_Buffer[cindex] |= (a_Light << 4) & 0xf0;
		}
	}
}

void cChunk::SetLight(char* a_Buffer, int x, int y, int z, char light)
{
	if( x < 16 && x > -1 && y < 128 && y > -1 && z < 16 && z > -1 )
	{
		int cindex = (y/2) + (z * 64) + (x * 64 * 16);
		if( (y & 1) == 0 )
		{	// First half byte
			a_Buffer[cindex] &= 0xf0; // Set first half to 0
			a_Buffer[cindex] |= (light) & 0x0f;
		}
		else
		{
			a_Buffer[cindex] &= 0x0f; // Set second half to 0
			a_Buffer[cindex] |= (light << 4) & 0xf0;
		}
	}
}

inline void cChunk::SpreadLightOfBlock(char* a_LightBuffer, int a_X, int a_Y, int a_Z, char a_Falloff)
{
	unsigned char CurrentLight = GetLight( a_LightBuffer, a_X, a_Y, a_Z );
	SetLight( a_LightBuffer, a_X-1, a_Y, a_Z, MAX(GetLight( a_LightBuffer, a_X-1, a_Y, a_Z ), MAX(0,CurrentLight-a_Falloff) ) );
	SetLight( a_LightBuffer, a_X+1, a_Y, a_Z, MAX(GetLight( a_LightBuffer, a_X+1, a_Y, a_Z ), MAX(0,CurrentLight-a_Falloff) ) );
	SetLight( a_LightBuffer, a_X, a_Y-1, a_Z, MAX(GetLight( a_LightBuffer, a_X, a_Y-1, a_Z ), MAX(0,CurrentLight-a_Falloff) ) );
	SetLight( a_LightBuffer, a_X, a_Y+1, a_Z, MAX(GetLight( a_LightBuffer, a_X, a_Y+1, a_Z ), MAX(0,CurrentLight-a_Falloff) ) );
	SetLight( a_LightBuffer, a_X, a_Y, a_Z-1, MAX(GetLight( a_LightBuffer, a_X, a_Y, a_Z-1 ), MAX(0,CurrentLight-a_Falloff) ) );
	SetLight( a_LightBuffer, a_X, a_Y, a_Z+1, MAX(GetLight( a_LightBuffer, a_X, a_Y, a_Z+1 ), MAX(0,CurrentLight-a_Falloff) ) );
}

inline void cChunk::SpreadLightOfBlockX(char* a_LightBuffer, int a_X, int a_Y, int a_Z)
{
	unsigned char CurrentLight = GetLight( a_LightBuffer, a_X, a_Y, a_Z );
	SetLight( a_LightBuffer, a_X-1, a_Y, a_Z, MAX(GetLight( a_LightBuffer, a_X-1, a_Y, a_Z ), CurrentLight-1) );
	SetLight( a_LightBuffer, a_X+1, a_Y, a_Z, MAX(GetLight( a_LightBuffer, a_X+1, a_Y, a_Z ), CurrentLight-1) );
}

inline void cChunk::SpreadLightOfBlockY(char* a_LightBuffer, int a_X, int a_Y, int a_Z)
{
	unsigned char CurrentLight = GetLight( a_LightBuffer, a_X, a_Y, a_Z );
	SetLight( a_LightBuffer, a_X, a_Y-1, a_Z, MAX(GetLight( a_LightBuffer, a_X, a_Y-1, a_Z ), CurrentLight-1) );
	SetLight( a_LightBuffer, a_X, a_Y+1, a_Z, MAX(GetLight( a_LightBuffer, a_X, a_Y+1, a_Z ), CurrentLight-1) );
}

inline void cChunk::SpreadLightOfBlockZ(char* a_LightBuffer, int a_X, int a_Y, int a_Z)
{
	unsigned char CurrentLight = GetLight( a_LightBuffer, a_X, a_Y, a_Z );
	SetLight( a_LightBuffer, a_X, a_Y, a_Z-1, MAX(GetLight( a_LightBuffer, a_X, a_Y, a_Z-1 ), CurrentLight-1) );
	SetLight( a_LightBuffer, a_X, a_Y, a_Z+1, MAX(GetLight( a_LightBuffer, a_X, a_Y, a_Z+1 ), CurrentLight-1) );
}

void cChunk::CalculateHeightmap()
{
	m_bCalculateHeightmap = false;
	for(int x = 0; x < 16; x++)
	{
		for(int z = 0; z < 16; z++)
		{
			for(int y = 127; y > -1; y--)
			{
				int index = MakeIndex( x, y, z );
				if(m_BlockData[index] != E_BLOCK_AIR)
				{
					m_HeightMap[x + z*16] = (char)y;
					break;
				}
			}
		}
	}
}

void cChunk::CalculateLighting()
{
	// Calculate sunlight
	memset(m_BlockSkyLight, 0xff, c_NumBlocks/2 ); // Set all to fully lit, so everything above HeightMap is lit
	for(int x = 0; x < 16; x++)
	{
		for(int z = 0; z < 16; z++)
		{
			char sunlight = 0xf;
			for(int y = m_HeightMap[x + z*16]; y > -1; y--)
			{
				int index = y + (z * 128) + (x * 128 * 16);

				if( g_BlockTransparent[ (int)m_BlockData[index] ] == false )
				{
					sunlight = 0x0;
				}
				SetLight( m_BlockSkyLight, x, y, z, sunlight );
			}
		}
	}

	// Calculate blocklights
	for(int x = 0; x < 16; x++)
	{
		for(int z = 0; z < 16; z++)
		{
			int MaxHeight = m_HeightMap[x + z*16];
			for(int y = 0; y < MaxHeight; y++)
			{
				char BlockID = GetBlock(x, y, z);
				SetLight( m_BlockLight, x, y, z, g_BlockLightValue[(int)BlockID] );
			}
		}
	}

	SpreadLight(m_BlockSkyLight);
	SpreadLight(m_BlockLight);

	// Stop it from calculating again :P
	m_bCalculateLighting = false;
}

void cChunk::SpreadLight(char* a_LightBuffer)
{
	// Spread the sunlight
	for(int x = 0; x < 16; x++)	for(int z = 0; z < 16; z++)	for(int y = 0; y < 128; y++)
	{
		int index = y + (z * 128) + (x * 128 * 16);
		if( g_BlockSpreadLightFalloff[ m_BlockData[index] ] > 0 )
		{
			SpreadLightOfBlock(a_LightBuffer, x, y, z, g_BlockSpreadLightFalloff[ m_BlockData[index] ]);
		}
	}

	for(int x = 15; x > -1; x--) for(int z = 15; z > -1; z--) for(int y = 127; y > -1; y--)
	{
		int index = y + (z * 128) + (x * 128 * 16);
		if( g_BlockSpreadLightFalloff[ m_BlockData[index] ] > 0 )
		{
			SpreadLightOfBlock(a_LightBuffer, x, y, z, g_BlockSpreadLightFalloff[ m_BlockData[index] ]);
		}
	}

	bool bCalcLeft, bCalcRight, bCalcFront, bCalcBack;
	bCalcLeft = bCalcRight = bCalcFront = bCalcBack = false;
	// Spread to neighbour chunks X-axis
	cChunk* LeftChunk = m_World->GetChunkUnreliable( m_PosX-1, m_PosY, m_PosZ );
	cChunk* RightChunk = m_World->GetChunkUnreliable( m_PosX+1, m_PosY, m_PosZ );
	char* LeftSky = 0, *RightSky = 0;
	if(LeftChunk) LeftSky = (a_LightBuffer==m_BlockSkyLight)?LeftChunk->pGetSkyLight():LeftChunk->pGetLight();
	if(RightChunk) RightSky = (a_LightBuffer==m_BlockSkyLight)?RightChunk->pGetSkyLight():RightChunk->pGetLight();
	for(int z = 0; z < 16; z++)	for(int y = 0; y < 128; y++)
	{
		if( LeftChunk )
		{
			int index = y + (z * 128) + (0  * 128 * 16);
			if( g_BlockSpreadLightFalloff[ m_BlockData[index] ] > 0 )
			{
				char CurrentLight = GetLight( a_LightBuffer, 0, y, z );
				char LeftLight = GetLight( LeftSky, 15, y, z );
				if( LeftLight < CurrentLight-g_BlockSpreadLightFalloff[ m_BlockData[index] ] )
				{
					SetLight( LeftSky, 15, y, z, MAX(0, CurrentLight-g_BlockSpreadLightFalloff[ m_BlockData[index] ]) );
					bCalcLeft = true;
				}
			}
		}
		if( RightChunk )
		{
			int index = y + (z * 128) + (15  * 128 * 16);
			if( g_BlockSpreadLightFalloff[ m_BlockData[index] ] > 0 )
			{
				char CurrentLight = GetLight( a_LightBuffer, 15, y, z );
				char RightLight = GetLight( RightSky, 0, y, z );
				if( RightLight < CurrentLight-g_BlockSpreadLightFalloff[ m_BlockData[index] ] )
				{
					SetLight( RightSky, 0, y, z,  MAX(0, CurrentLight-g_BlockSpreadLightFalloff[ m_BlockData[index] ]) );
					bCalcRight = true;
				}
			}
		}
	}

	// Spread to neighbour chunks Z-axis
	cChunk* FrontChunk = m_World->GetChunkUnreliable( m_PosX, m_PosY, m_PosZ-1 );
	cChunk* BackChunk = m_World->GetChunkUnreliable( m_PosX, m_PosY, m_PosZ+1 );
	char* FrontSky = 0, *BackSky = 0;
	if(FrontChunk) FrontSky = (a_LightBuffer==m_BlockSkyLight)?FrontChunk->pGetSkyLight():FrontChunk->pGetLight();
	if(BackChunk) BackSky = (a_LightBuffer==m_BlockSkyLight)?BackChunk->pGetSkyLight():BackChunk->pGetLight();
	for(int x = 0; x < 16; x++)	for(int y = 0; y < 128; y++)
	{
		if( FrontChunk )
		{
			int index = y + (0 * 128) + (x  * 128 * 16);
			if( g_BlockSpreadLightFalloff[ m_BlockData[index] ] > 0 )
			{
				char CurrentLight = GetLight( a_LightBuffer, x, y, 0 );
				char FrontLight = GetLight( FrontSky, x, y, 15 );
				if( FrontLight < CurrentLight-g_BlockSpreadLightFalloff[ m_BlockData[index] ] )
				{
					SetLight( FrontSky, x, y, 15,  MAX(0, CurrentLight-g_BlockSpreadLightFalloff[ m_BlockData[index] ]) );
					bCalcFront = true;
				}
			}
		}
		if( BackChunk )
		{
			int index = y + (15 * 128) + (x  * 128 * 16);
			if( g_BlockSpreadLightFalloff[ m_BlockData[index] ] > 0 )
			{
				char CurrentLight = GetLight( a_LightBuffer, x, y, 15 );
				char BackLight = GetLight( BackSky, x, y, 0 );
				if( BackLight < CurrentLight-g_BlockSpreadLightFalloff[ m_BlockData[index] ] )
				{
					SetLight( BackSky, x, y, 0, MAX(0, CurrentLight-g_BlockSpreadLightFalloff[ m_BlockData[index] ]) );
					bCalcBack = true;
				}
			}
		}
	}

	if( bCalcLeft )		m_World->ReSpreadLighting( LeftChunk );
	if( bCalcRight )	m_World->ReSpreadLighting( RightChunk );
	if( bCalcFront )	m_World->ReSpreadLighting( FrontChunk );
	if( bCalcBack )		m_World->ReSpreadLighting( BackChunk );
}

void cChunk::AsyncUnload( cClientHandle* a_Client )
{
	m_pState->m_UnloadQuery.remove( a_Client );	// Make sure this client is only in the list once
	m_pState->m_UnloadQuery.push_back( a_Client );
}

void cChunk::Send( cClientHandle* a_Client )
{
	cPacket_PreChunk PreChunk;
	PreChunk.m_PosX = m_PosX;
	PreChunk.m_PosZ = m_PosZ;
	PreChunk.m_bLoad = true;
	a_Client->Send( PreChunk );
	a_Client->Send( cPacket_MapChunk( this ) );

	for( BlockEntityList::iterator itr = m_pState->m_BlockEntities.begin(); itr != m_pState->m_BlockEntities.end(); ++itr )
	{
		(*itr)->SendTo( a_Client );
	}
}

void cChunk::SetBlock( int a_X, int a_Y, int a_Z, char a_BlockType, char a_BlockMeta )
{
	if(a_X < 0 || a_X >= 16 || a_Y < 0 || a_Y >= 128 || a_Z < 0 || a_Z >= 16)
	{
		//printf(">>>>>>>>>>>>>>>> CLIPPED SETBLOCK %i %i %i\n", a_X, a_Y, a_Z );
		return; // Clip
	}

	int index = a_Y + (a_Z * 128) + (a_X * 128 * 16);
	char OldBlockMeta = GetLight( m_BlockMeta, index );
	char OldBlockType = m_BlockType[index];
	m_BlockType[index] = a_BlockType;

	SetLight( m_BlockMeta, index, a_BlockMeta );

	if( OldBlockType != a_BlockType || OldBlockMeta != a_BlockMeta )
	{
		//LOG("Old: %i %i New: %i %i", OldBlockType, OldBlockMeta, a_BlockType, a_BlockMeta );
		m_pState->m_PendingSendBlocks.push_back( index );

		m_pState->m_ToTickBlocks[ MakeIndex( a_X, a_Y, a_Z ) ]++;
		m_pState->m_ToTickBlocks[ MakeIndex( a_X+1, a_Y, a_Z ) ]++;
		m_pState->m_ToTickBlocks[ MakeIndex( a_X-1, a_Y, a_Z ) ]++;
		m_pState->m_ToTickBlocks[ MakeIndex( a_X, a_Y+1, a_Z ) ]++;
		m_pState->m_ToTickBlocks[ MakeIndex( a_X, a_Y-1, a_Z ) ]++;
		m_pState->m_ToTickBlocks[ MakeIndex( a_X, a_Y, a_Z+1 ) ]++;
		m_pState->m_ToTickBlocks[ MakeIndex( a_X, a_Y, a_Z-1 ) ]++;

		cBlockEntity* BlockEntity = GetBlockEntity( a_X + m_PosX*16, a_Y+m_PosY*128, a_Z+m_PosZ*16 );
		if( BlockEntity )
		{
			BlockEntity->Destroy();
			RemoveBlockEntity( BlockEntity );
			delete BlockEntity;
		}
		switch( a_BlockType )
		{
		case E_BLOCK_CHEST:
			AddBlockEntity( new cChestEntity( a_X + m_PosX*16, a_Y + m_PosY*128, a_Z + m_PosZ*16, this ) );
			break;
		case E_BLOCK_FURNACE:
			AddBlockEntity( new cFurnaceEntity( a_X + m_PosX*16, a_Y + m_PosY*128, a_Z + m_PosZ*16, this ) );
			break;
		case E_BLOCK_SIGN_POST:
		case E_BLOCK_WALLSIGN:
			AddBlockEntity( new cSignEntity( (ENUM_BLOCK_ID)a_BlockType, a_X + m_PosX*16, a_Y + m_PosY*128, a_Z + m_PosZ*16, this ) );
			break;
		default:
			break;
		};
	}

	CalculateHeightmap();
	RecalculateLighting();
}

void cChunk::FastSetBlock( int a_X, int a_Y, int a_Z, char a_BlockType, char a_BlockMeta )
{
	if(a_X < 0 || a_X >= 16 || a_Y < 0 || a_Y >= 128 || a_Z < 0 || a_Z >= 16)
	{
		//printf(">>>>>>>>>>>>>>>> CLIPPED SETBLOCK %i %i %i\n", a_X, a_Y, a_Z );
		return; // Clip
	}

	const int index = a_Y + (a_Z * 128) + (a_X * 128 * 16);
	const char OldBlock = m_BlockType[index];
	m_BlockType[index] = a_BlockType;
	m_pState->m_PendingSendBlocks.push_back( index );
	SetLight( m_BlockMeta, index, a_BlockMeta );

	// ONLY recalculate lighting if it's necessary!
	if(		g_BlockLightValue[ OldBlock ] != g_BlockLightValue[ a_BlockType ]
		||	g_BlockSpreadLightFalloff[ OldBlock ] != g_BlockSpreadLightFalloff[ a_BlockType ]
		||	g_BlockTransparent[ OldBlock ] != g_BlockTransparent[ a_BlockType ] )
	{
		RecalculateLighting();
	}

	// Recalculate next tick
	RecalculateHeightmap();
}

void cChunk::SendBlockTo( int a_X, int a_Y, int a_Z, cClientHandle* a_Client )
{
	if( a_Client == 0 )
	{
		m_pState->m_PendingSendBlocks.push_back( MakeIndex( a_X, a_Y, a_Z ) );
		return;
	}

	for( std::list< cClientHandle* >::iterator itr = m_pState->m_LoadedByClient.begin(); itr != m_pState->m_LoadedByClient.end(); ++itr )
	{
		if( *itr == a_Client )
		{
			unsigned int index = MakeIndex( a_X, a_Y, a_Z );
			cPacket_BlockChange BlockChange;
			BlockChange.m_PosX = a_X + m_PosX*16;
			BlockChange.m_PosY = (char)(a_Y + m_PosY*128);
			BlockChange.m_PosZ = a_Z + m_PosZ*16;
			BlockChange.m_BlockType = m_BlockType[ index ];
			BlockChange.m_BlockMeta = GetLight( m_BlockMeta, index );
			a_Client->Send( BlockChange );
			break;
		}
	}
}

void cChunk::AddBlockEntity( cBlockEntity* a_BlockEntity )
{
	m_pState->m_BlockEntities.push_back( a_BlockEntity );
}

void cChunk::RemoveBlockEntity( cBlockEntity* a_BlockEntity )
{
	m_pState->m_BlockEntities.remove( a_BlockEntity );
}

void cChunk::AddClient( cClientHandle* a_Client )
{
	m_pState->m_LoadedByClient.remove( a_Client );
	m_pState->m_LoadedByClient.push_back( a_Client );

	LockEntities();
	for( EntityList::iterator itr = m_pState->m_Entities.begin(); itr != m_pState->m_Entities.end(); ++itr )
	{
		LOG("%i %i %i Spawning on %s", m_PosX, m_PosY, m_PosZ, a_Client->GetUsername() );
		(*itr)->SpawnOn( a_Client );
	}
	UnlockEntities();
}

void cChunk::RemoveClient( cClientHandle* a_Client )
{
	m_pState->m_LoadedByClient.remove( a_Client );

	if( !a_Client->IsDestroyed() )
	{
		LockEntities();
		for( EntityList::iterator itr = m_pState->m_Entities.begin(); itr != m_pState->m_Entities.end(); ++itr )
		{
			LOG("%i %i %i Destroying on %s", m_PosX, m_PosY, m_PosZ, a_Client->GetUsername() );
			cPacket_DestroyEntity DestroyEntity( *itr );
			a_Client->Send( DestroyEntity );
		}
		UnlockEntities();
	}
}

void cChunk::AddEntity( cEntity & a_Entity )
{
	LockEntities();
	m_pState->m_Entities.push_back( &a_Entity );
	UnlockEntities();
}

bool cChunk::RemoveEntity( cEntity & a_Entity, cChunk* a_CalledFrom /* = 0 */ )
{
	LockEntities();
	unsigned int SizeBefore = m_pState->m_Entities.size();
	m_pState->m_Entities.remove( &a_Entity );
	if( SizeBefore == m_pState->m_Entities.size() )
	{
		LOG("WARNING: Entity was not in chunk %i %i %i", m_PosX, m_PosY, m_PosZ );
		if( !a_CalledFrom )
		{
			UnlockEntities();
			return m_World->RemoveEntityFromChunk( a_Entity, this );
		}
		UnlockEntities();
		return false;
	}
	UnlockEntities();
	return true;
}

void cChunk::LockEntities()
{
	m_EntitiesCriticalSection->Lock();
}

void cChunk::UnlockEntities()
{
	m_EntitiesCriticalSection->Unlock();
}

char cChunk::GetBlock( int a_X, int a_Y, int a_Z )
{
	if(a_X < 0 || a_X >= 16 || a_Y < 0 || a_Y >= 128 || a_Z < 0 || a_Z >= 16) return 0; // Clip

	int index = a_Y + (a_Z * 128) + (a_X * 128 * 16);
	return m_BlockType[index];
}

char cChunk::GetBlock( int a_BlockIdx )
{
	if( a_BlockIdx < 0 || a_BlockIdx >= c_NumBlocks ) return 0;
	return m_BlockType[ a_BlockIdx ];
}

cBlockEntity* cChunk::GetBlockEntity( int a_X, int a_Y, int a_Z )
{
	for( std::list<cBlockEntity*>::iterator itr = m_pState->m_BlockEntities.begin(); itr != m_pState->m_BlockEntities.end(); ++itr)
	{
		if( (*itr)->GetPosX() == a_X &&
			(*itr)->GetPosY() == a_Y &&
			(*itr)->GetPosZ() == a_Z )
		{
			return *itr;
		}
	}
	return 0;
}

bool cChunk::LoadFromDisk()
{
	char SourceFile[128];
	sprintf_s(SourceFile, 128, "world/X%i_Y%i_Z%i.bin", m_PosX, m_PosY, m_PosZ );

	FILE* f = 0;
	#ifdef _WIN32
	if( fopen_s(&f, SourceFile, "rb" ) == 0 )	// no error
	#else
	if( (f = fopen(SourceFile, "rb" )) != 0 )	// no error
	#endif
	{
		if( fread( m_BlockData, sizeof(char)*c_BlockDataSize, 1, f) != 1 ) { LOGERROR("ERROR READING FROM FILE %s", SourceFile); fclose(f); return false; }

		// Now load Block Entities
		ENUM_BLOCK_ID BlockType;
		while( fread( &BlockType, sizeof(ENUM_BLOCK_ID), 1, f) == 1 )
		{
			switch( BlockType )
			{
			case E_BLOCK_CHEST:
				{
					cChestEntity* ChestEntity = new cChestEntity( 0, 0, 0, this );
					if( !ChestEntity->LoadFromFile( f ) )
					{
						LOGERROR("ERROR READING CHEST FROM FILE %s", SourceFile );
						delete ChestEntity;
						fclose(f);
						return false;
					}
					m_pState->m_BlockEntities.push_back( ChestEntity );
				}
				break;
			case E_BLOCK_FURNACE:
				{
					cFurnaceEntity* FurnaceEntity = new cFurnaceEntity( 0, 0, 0, this );
					if( !FurnaceEntity->LoadFromFile( f ) )
					{
						LOGERROR("ERROR READING FURNACE FROM FILE %s", SourceFile );
						delete FurnaceEntity;
						fclose(f);
						return false;
					}
					m_pState->m_BlockEntities.push_back( FurnaceEntity );
					m_pState->m_TickBlockEntities.push_back( FurnaceEntity ); // They need tickin'
				}
				break;
			case E_BLOCK_SIGN_POST:
			case E_BLOCK_WALLSIGN:
				{
					cSignEntity* SignEntity = new cSignEntity(BlockType, 0, 0, 0, this );
					if( !SignEntity->LoadFromFile( f ) )
					{
						LOGERROR("ERROR READING SIGN FROM FILE %s", SourceFile );
						delete SignEntity;
						fclose(f);
						return false;
					}
					m_pState->m_BlockEntities.push_back( SignEntity );
				}
				break;
			default:
				break;
			}
		}

		fclose(f);

		// Delete old format file
		if( std::remove( SourceFile ) != 0 )
			LOGERROR("Could not delete file %s", SourceFile );
		else
			LOGINFO("Successfully deleted olf format file %s", SourceFile );

		return true;
	}
	else
	{
		//LOGWARN("COULD NOT OPEN FILE %s\n", SourceFile);
		return false;
	}
}

bool cChunk::SaveToDisk()
{
	return true; //no more saving old format!

	char SourceFile[128];
	sprintf_s(SourceFile, 128, "world/X%i_Y%i_Z%i.bin", m_PosX, m_PosY, m_PosZ );

    #ifdef _WIN32
	{
		SECURITY_ATTRIBUTES Attrib;
		Attrib.nLength = sizeof(SECURITY_ATTRIBUTES);
		Attrib.lpSecurityDescriptor = NULL;
		Attrib.bInheritHandle = false;
		::CreateDirectory("world", &Attrib);
	}
	#else
	{
        mkdir("world", S_IRWXU | S_IRWXG | S_IRWXO);
	}
	#endif

	FILE* f = 0;
#ifdef _WIN32
	if( fopen_s(&f, SourceFile, "wb" ) == 0 )	// no error
	#else
	if( (f = fopen(SourceFile, "wb" )) != 0 )	// no error
	#endif
	{
		fwrite( m_BlockData, sizeof(char)*c_BlockDataSize, 1, f );

		// Now write Block Entities
		for( std::list<cBlockEntity*>::iterator itr = m_pState->m_BlockEntities.begin(); itr != m_pState->m_BlockEntities.end(); ++itr)
		{
			cBlockEntity* BlockEntity = *itr;
			switch( BlockEntity->GetBlockType() )
			{
			case E_BLOCK_CHEST:
				{
					cChestEntity* ChestEntity = reinterpret_cast< cChestEntity* >( BlockEntity );
					ChestEntity->WriteToFile( f );
				}
				break;
			case E_BLOCK_FURNACE:
				{
					cFurnaceEntity* FurnaceEntity = reinterpret_cast< cFurnaceEntity* >( BlockEntity );
					FurnaceEntity->WriteToFile( f );
				}
				break;
			case E_BLOCK_SIGN_POST:
			case E_BLOCK_WALLSIGN:
				{
					cSignEntity* SignEntity = reinterpret_cast< cSignEntity* >( BlockEntity );
					SignEntity->WriteToFile( f );
				}
				break;
			default:
				break;
			}
		}

		fclose(f);
		return true;
	}
	else
	{
		LOGERROR("ERROR WRITING TO FILE %s", SourceFile);
		return false;
	}
}

void cChunk::Broadcast( const cPacket & a_Packet, cClientHandle* a_Exclude /* = 0 */ ) const
{
	for( std::list< cClientHandle* >::const_iterator itr = m_pState->m_LoadedByClient.begin(); itr != m_pState->m_LoadedByClient.end(); ++itr )
	{
		if( *itr == a_Exclude ) continue;
		(*itr)->Send( a_Packet );
	}
}


void cChunk::LoadFromJson( const Json::Value & a_Value )
{
	// Load chests
	Json::Value AllChests = a_Value.get("Chests", Json::nullValue);
	if( !AllChests.empty() )
	{
		for( Json::Value::iterator itr = AllChests.begin(); itr != AllChests.end(); ++itr )
		{
			Json::Value & Chest = *itr;
			cChestEntity* ChestEntity = new cChestEntity(0,0,0, this);
			if( !ChestEntity->LoadFromJson( Chest ) )
			{
				LOGERROR("ERROR READING CHEST FROM JSON!" );
				delete ChestEntity;
			}
			else m_pState->m_BlockEntities.push_back( ChestEntity );
		}
	}

	// Load furnaces
	Json::Value AllFurnaces = a_Value.get("Furnaces", Json::nullValue);
	if( !AllFurnaces.empty() )
	{
		for( Json::Value::iterator itr = AllFurnaces.begin(); itr != AllFurnaces.end(); ++itr )
		{
			Json::Value & Furnace = *itr;
			cFurnaceEntity* FurnaceEntity = new cFurnaceEntity(0,0,0, this);
			if( !FurnaceEntity->LoadFromJson( Furnace ) )
			{
				LOGERROR("ERROR READING FURNACE FROM JSON!" );
				delete FurnaceEntity;
			}
			else m_pState->m_BlockEntities.push_back( FurnaceEntity );
		}
	}

	// Load signs
	Json::Value AllSigns = a_Value.get("Signs", Json::nullValue);
	if( !AllSigns.empty() )
	{
		for( Json::Value::iterator itr = AllSigns.begin(); itr != AllSigns.end(); ++itr )
		{
			Json::Value & Sign = *itr;
			cSignEntity* SignEntity = new cSignEntity( E_BLOCK_SIGN_POST, 0,0,0, this);
			if( !SignEntity->LoadFromJson( Sign ) )
			{
				LOGERROR("ERROR READING SIGN FROM JSON!" );
				delete SignEntity;
			}
			else m_pState->m_BlockEntities.push_back( SignEntity );
		}
	}
}

void cChunk::SaveToJson( Json::Value & a_Value )
{
	Json::Value AllChests;
	Json::Value AllFurnaces;
	Json::Value AllSigns;
	for( std::list<cBlockEntity*>::iterator itr = m_pState->m_BlockEntities.begin(); itr != m_pState->m_BlockEntities.end(); ++itr)
	{
		cBlockEntity* BlockEntity = *itr;
		switch( BlockEntity->GetBlockType() )
		{
		case E_BLOCK_CHEST:
			{
				cChestEntity* ChestEntity = reinterpret_cast< cChestEntity* >( BlockEntity );
				Json::Value NewChest;
				ChestEntity->SaveToJson( NewChest );
				AllChests.append( NewChest );
			}
			break;
		case E_BLOCK_FURNACE:
			{
				cFurnaceEntity* FurnaceEntity = reinterpret_cast< cFurnaceEntity* >( BlockEntity );
				Json::Value NewFurnace;
				FurnaceEntity->SaveToJson( NewFurnace );
				AllFurnaces.append( NewFurnace );
			}
			break;
		case E_BLOCK_SIGN_POST:
		case E_BLOCK_WALLSIGN:
			{
				cSignEntity* SignEntity = reinterpret_cast< cSignEntity* >( BlockEntity );
				Json::Value NewSign;
				SignEntity->SaveToJson( NewSign );
				AllSigns.append( NewSign );
			}
			break;
		default:
			break;
		}
	}

	if( !AllChests.empty() )
		a_Value["Chests"] = AllChests;
	if( !AllFurnaces.empty() )
		a_Value["Furnaces"] = AllFurnaces;
	if( !AllSigns.empty() )
		a_Value["Signs"] = AllSigns;
}

EntityList & cChunk::GetEntities()
{
	return m_pState->m_Entities;
}

const ClientHandleList & cChunk::GetClients()
{
	return m_pState->m_LoadedByClient;
}


void cChunk::AddTickBlockEntity( cFurnaceEntity* a_Entity )
{
	m_pState->m_TickBlockEntities.remove( a_Entity );
	m_pState->m_TickBlockEntities.push_back( a_Entity );
}

void cChunk::RemoveTickBlockEntity( cFurnaceEntity* a_Entity )
{
	m_pState->m_TickBlockEntities.remove( a_Entity );
}


void cChunk::PositionToWorldPosition(int a_ChunkX, int a_ChunkY, int a_ChunkZ, int & a_X, int & a_Y, int & a_Z)
{
	a_Y = a_ChunkY;
	a_X = m_PosX * 16 + a_ChunkX;
	a_Z = m_PosZ * 16 + a_ChunkZ;
}