
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#if 0 // ignore all contents of this file
#include "cChunkLoader.h"
#include "cChunk.h"
#include "cMCLogger.h"
#include "BlockID.h"
#include "cCriticalSection.h"
#include "cEvent.h"
#include "cThread.h"
#include "cSleep.h"

#include "cChestEntity.h"
#include "cFurnaceEntity.h"
#include "cSignEntity.h"

#include <iostream>
#ifndef _WIN32
#include <cstring>
#include <cstdlib>
#include <stdio.h>
#include <sys/stat.h>   // for mkdir
#include <sys/types.h>
#else
#include <Windows.h>
#endif

#include <list>

struct ChunkData
{
	ChunkData()
		: x( 0 )
		, z( 0 )
		, Data( 0 )
		, LiveChunk( 0 )
	{}
	int x, z;
	unsigned int DataSize;
	unsigned int ChunkStart;
	char* Data;
	cChunk* LiveChunk;
};

typedef std::list< ChunkData > ChunkDataList;
struct cChunkLoader::ChunkPack
{
	ChunkDataList AllChunks;
	int x, y, z;
};

typedef std::list< cChunkLoader::ChunkPack > ChunkPackList;
struct cChunkLoader::ChunkPacks
{
	ChunkPackList AllPacks;
};

cChunkLoader::cChunkLoader()
	: m_bStop( false )
	, m_CriticalSection( new cCriticalSection() )
	, m_Event( new cEvent() )
	, m_ChunkPacks( new ChunkPacks )
{
	cThread( SaveThread, this );
}

cChunkLoader::~cChunkLoader()
{
	m_bStop = true;
	m_Event->Wait();
	delete m_CriticalSection;
}

void cChunkLoader::SaveThread( void* a_Param )
{
	cChunkLoader* self = (cChunkLoader*)a_Param;
	while( !self->m_bStop )
	{
		cSleep::MilliSleep( 1000 ); // Only check for saving once a second
	}
	self->m_Event->Set();
}

cChunk* cChunkLoader::LoadChunk( int a_X, int a_Y, int a_Z )
{
	m_CriticalSection->Lock();
	cChunk* Chunk = 0;
	
	Chunk = LoadOldFormat( a_X, a_Y, a_Z );
	if( Chunk ) { Chunk->CalculateHeightmap(); }
	else
	{
		1; // load new format()
	}

	m_CriticalSection->Unlock();
	return Chunk;
}

bool cChunkLoader::SaveChunk( const cChunk & a_Chunk )
{
	m_CriticalSection->Lock();
	bool Success = SaveOldFormat( a_Chunk );
	m_CriticalSection->Unlock();

	return Success;
}

/**************************************************
 * Old format stuffs
 **/

cChunk* cChunkLoader::LoadOldFormat( int a_X, int a_Y, int a_Z )
{
	char SourceFile[128];
	sprintf_s(SourceFile, 128, "world/X%i_Y%i_Z%i.bin", a_X, a_Y, a_Z );

	FILE* f = 0;
	#ifdef _WIN32
	if( fopen_s(&f, SourceFile, "rb" ) == 0 )	// no error
	#else
	if( (f = fopen(SourceFile, "rb" )) != 0 )	// no error
	#endif
	{
		cChunk* Chunk = new cChunk( a_X, a_Y, a_Z );
		if( fread( Chunk->m_BlockData, sizeof(char)*cChunk::c_BlockDataSize, 1, f) != 1 ) { LOGERROR("ERROR READING FROM FILE %s", SourceFile); fclose(f); return false; }

		// Now load Block Entities
		ENUM_BLOCK_ID BlockType;
		while( fread( &BlockType, sizeof(ENUM_BLOCK_ID), 1, f) == 1 )
		{
			switch( BlockType )
			{
			case E_BLOCK_CHEST:
				{
					cChestEntity* ChestEntity = new cChestEntity( 0, 0, 0 );
					if( !ChestEntity->LoadFromFile( f ) )
					{
						LOGERROR("ERROR READING CHEST FROM FILE %s", SourceFile );
						delete ChestEntity;
						fclose(f);
						return false;
					}
					Chunk->m_BlockEntities.push_back( ChestEntity );
				}
				break;
			case E_BLOCK_FURNACE:
				{
					cFurnaceEntity* FurnaceEntity = new cFurnaceEntity( 0, 0, 0 );
					if( !FurnaceEntity->LoadFromFile( f ) )
					{
						LOGERROR("ERROR READING FURNACE FROM FILE %s", SourceFile );
						delete FurnaceEntity;
						fclose(f);
						return false;
					}
					Chunk->m_BlockEntities.push_back( FurnaceEntity );
					Chunk->m_TickBlockEntities.push_back( FurnaceEntity ); // They need tickin'
				}
				break;
			case E_BLOCK_SIGN_POST:
			case E_BLOCK_WALLSIGN:
				{
					cSignEntity* SignEntity = new cSignEntity(BlockType, 0, 0, 0 );
					if( !SignEntity->LoadFromFile( f ) )
					{
						LOGERROR("ERROR READING SIGN FROM FILE %s", SourceFile );
						delete SignEntity;
						fclose(f);
						return false;
					}
					Chunk->m_BlockEntities.push_back( SignEntity );
				}
				break;
			default:
				break;
			}
		}

		fclose(f);
		return Chunk;
	}
	else
	{
		return 0;
	}
}

bool cChunkLoader::SaveOldFormat( const cChunk & a_Chunk )
{
	char SourceFile[128];
	sprintf_s(SourceFile, 128, "world/X%i_Y%i_Z%i.bin", a_Chunk.m_PosX, a_Chunk.m_PosY, a_Chunk.m_PosZ );

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
		fwrite( a_Chunk.m_BlockData, sizeof(char)*cChunk::c_BlockDataSize, 1, f );

		// Now write Block Entities
		for( std::list<cBlockEntity*>::const_iterator itr = a_Chunk.m_BlockEntities.begin(); itr != a_Chunk.m_BlockEntities.end(); ++itr)
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


/******************************************
 * New format
 **/

cChunk* cChunkLoader::LoadFormat1( int a_X, int a_Y, int a_Z )
{
	int PakX = (int)(floorf((float)a_X / 16.f));
	int PakY = (int)(floorf((float)a_Y / 16.f));
	int PakZ = (int)(floorf((float)a_Z / 16.f));

	ChunkPack * Pack = 0;
	ChunkPackList & PackList = m_ChunkPacks->AllPacks;
	for( ChunkPackList::iterator itr = PackList.begin(); itr != PackList.end(); ++itr )
	{
		if( itr->x == PakX && itr->y == PakY && itr->z == PakZ )
		{
			Pack = &(*itr);
			break;
		}
	}

	
	if( !Pack ) // The pack was not in memory, so try to load it from disk
	{
		Pack = LoadPak1( PakX, PakY, PakZ ); // Load .pak file from disk
		if( Pack )
		{
			PackList.push_back( *Pack ); // Add it to the loaded list
		}
	}

	if( Pack ) // Allright, the pack is in memory
	{
		ChunkData * Data = 0;
		ChunkDataList & ChunkList = Pack->AllChunks;
		for( ChunkDataList::iterator itr = ChunkList.begin(); itr != ChunkList.end(); ++itr )
		{
			if( itr->x == a_X && itr->z == a_Z )
			{
				Data = &(*itr);
				break;
			}
		}

		if( !Data ) // Sorry, chunk does not exist (yet)
			return 0;

		if( Data->LiveChunk ) // This chunk is already loaded and decoded (this should actually never happen)
			return Data->LiveChunk;

		// Decompress chunk, and return brand new chunk

		// doing it...

		return 0; // actually return the chunk
	}

	return 0; // .pak file didn't exist
}

cChunkLoader::ChunkPack* cChunkLoader::LoadPak1( int PakX, int PakY, int PakZ )
{
	char SourceFile[128];
	sprintf_s(SourceFile, 128, "world/X%i_Y%i_Z%i.pak", PakX, PakY, PakZ );

	FILE* f = 0;
	#ifdef _WIN32
	if( fopen_s(&f, SourceFile, "rb" ) == 0 )	// no error
	#else
	if( (f = fopen(SourceFile, "rb" )) != 0 )	// no error
	#endif
	{
		cChunkLoader::ChunkPack * Pack = new cChunkLoader::ChunkPack;
		Pack->x = PakX;
		Pack->y = PakY;
		Pack->z = PakZ;

		short Version = 0;
		if( fread( &Version, sizeof( short ), 1, f ) != 1 ) { LOGERROR("Error reading file %s", SourceFile ); return 0; }
		if( Version != 1 ) { LOGERROR("Wrong pak version! %s", SourceFile ); return 0; }
		short NumChunks = 0;
		if( fread( &NumChunks, sizeof( short ), 1, f ) != 1 ) { LOGERROR("Error reading file %s", SourceFile ); return 0; }

		// Load all the headers
		for( short i = 0; i < NumChunks; ++i )
		{
			ChunkData Data;
			if( fread( &Data.x, sizeof( int ), 1, f ) != 1 ) { LOGERROR("Error reading file %s", SourceFile ); return 0; }
			if( fread( &Data.z, sizeof( int ), 1, f ) != 1 ) { LOGERROR("Error reading file %s", SourceFile ); return 0; }
			if( fread( &Data.DataSize, sizeof( unsigned int ), 1, f ) != 1 ) { LOGERROR("Error reading file %s", SourceFile ); return 0; }
			if( fread( &Data.ChunkStart, sizeof( unsigned int ), 1, f ) != 1 ) { LOGERROR("Error reading file %s", SourceFile ); return 0; }
			Pack->AllChunks.push_back( Data );
		}

		// Load all compressed chunk data in the order the headers were loaded
		ChunkDataList::iterator itr = Pack->AllChunks.begin();
		for( short i = 0; i < NumChunks; ++i )
		{
			itr->Data = new char[ itr->DataSize ];
			if( fread( itr->Data, sizeof( char ) * itr->DataSize, 1, f ) != 1 ) { LOGERROR("Error reading file %s", SourceFile ); return 0; }
			++itr;
		}

		// And we're done :)
		return Pack;
	}
	return 0;
}
#endif