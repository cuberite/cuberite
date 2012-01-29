
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "cChunkMap.h"
#include "cChunk.h"
#include "cWorld.h"
#include "cRoot.h"
#include "cMakeDir.h"
#include <math.h>  // floorf

#ifndef _WIN32
	#include <cstring> // memcpy
	#include <cstdlib> // abs
#endif

#include "zlib.h"
#include <json/json.h>

#define USE_MEMCPY

#define LAYER_SIZE (32)





cChunkMap::cChunkMap(cWorld* a_World )
	: m_Layers( 0 )
	, m_NumLayers( 0 )
	, m_World( a_World )
{
}





cChunkMap::~cChunkMap()
{
}





cChunkMap::cChunkData* cChunkMap::cChunkLayer::GetChunk( int a_X, int a_Z )
{
	const int LocalX = a_X - m_X * LAYER_SIZE;
	const int LocalZ = a_Z - m_Z * LAYER_SIZE;
	//LOG("LocalX:%i LocalZ:%i", LocalX, LocalZ );
	if( LocalX < LAYER_SIZE && LocalZ < LAYER_SIZE && LocalX > -1 && LocalZ > -1 )
		return &m_Chunks[ LocalX + LocalZ * LAYER_SIZE ];
	return 0;
}





bool cChunkMap::RemoveLayer( cChunkLayer* a_Layer )
{
	cChunkLayer* NewLayers = 0;
	if( m_NumLayers > 1 )
		NewLayers = new cChunkLayer[m_NumLayers-1];

	int idx = 0;
	bool bExcludedLayer = false;
	for( int i = 0; i < m_NumLayers; ++i )
	{
		if( &m_Layers[i] != a_Layer )
		{
			if( idx < m_NumLayers-1 )
			{
				NewLayers[ idx ] = m_Layers[i];
				idx++;
			}
		}
		else
			bExcludedLayer = true;
	}

	if( !bExcludedLayer )
	{
		LOGWARN("Could not remove layer, because layer was not found %i %i", a_Layer->m_X, a_Layer->m_Z);
		delete [] NewLayers;
		return false;
	}

	if( m_Layers ) delete [] m_Layers;
	m_Layers = NewLayers;
	m_NumLayers--;
	return true;
}





cChunkMap::cChunkLayer* cChunkMap::AddLayer( const cChunkLayer & a_Layer )
{
	cChunkLayer* TempLayers = new cChunkLayer[m_NumLayers+1];
	if( m_NumLayers > 0 )
	{
		memcpy( TempLayers, m_Layers, sizeof( cChunkLayer ) * m_NumLayers );
		delete [] m_Layers;
	}
	m_Layers = TempLayers;
	
	m_Layers[m_NumLayers] = a_Layer;
	cChunkLayer* NewLayer = &m_Layers[m_NumLayers];
	m_NumLayers++;

	return NewLayer;
}





void cChunkMap::AddChunk( cChunk* a_Chunk )
{
	/* // OLD
	m_Nodes[ MakeHash( a_Chunk->GetPosX(), a_Chunk->GetPosZ() ) ].push_back( a_Chunk );
	*/

	// NEW
	const int LayerX = (int)(floorf((float)a_Chunk->GetPosX() / (float)(LAYER_SIZE)));
	const int LayerZ = (int)(floorf((float)a_Chunk->GetPosZ() / (float)(LAYER_SIZE)));
	cChunkLayer* FoundLayer = GetLayer( LayerX, LayerZ );
	if( !FoundLayer )
	{
		cChunkLayer NewLayer( LAYER_SIZE*LAYER_SIZE );
		NewLayer.m_X = LayerX;
		NewLayer.m_Z = LayerZ;
		FoundLayer = AddLayer( NewLayer );
		LOGWARN("Created new layer %i %i (total layers %i)", LayerX, LayerZ, m_NumLayers );
	}

	//Get local coordinates in layer
	const int LocalX = a_Chunk->GetPosX() - LayerX * LAYER_SIZE;
	const int LocalZ = a_Chunk->GetPosZ() - LayerZ * LAYER_SIZE;
	if( FoundLayer->m_Chunks[ LocalX + LocalZ * LAYER_SIZE ].m_LiveChunk )
		LOGWARN("WARNING: Added chunk to layer while it was already loaded!");
	if( FoundLayer->m_Chunks[ LocalX + LocalZ * LAYER_SIZE ].m_Compressed )
		LOGWARN("WARNING: Added chunk to layer while a compressed version exists!");
	FoundLayer->m_Chunks[ LocalX + LocalZ * LAYER_SIZE ].m_LiveChunk = a_Chunk;
	FoundLayer->m_NumChunksLoaded++;
}





void cChunkMap::RemoveChunk( cChunk* a_Chunk )
{
	/* // OLD
	m_Nodes[ MakeHash( a_Chunk->GetPosX(), a_Chunk->GetPosZ() ) ].erase( a_Chunk );
	*/

	// NEW
	cChunkLayer* Layer = GetLayerForChunk( a_Chunk->GetPosX(), a_Chunk->GetPosZ() );
	if( Layer )
	{
		cChunkData* Data = Layer->GetChunk( a_Chunk->GetPosX(), a_Chunk->GetPosZ() );
		if( Data->m_LiveChunk )
		{
			CompressChunk( Data );
			Data->m_LiveChunk = 0; // Set live chunk to 0
		}
		Layer->m_NumChunksLoaded--;
	}
}





void cChunkMap::CompressChunk( cChunkData* a_ChunkData )
{
	if( a_ChunkData->m_LiveChunk )
	{
		// Delete already present compressed data
		if( a_ChunkData->m_Compressed ) delete [] a_ChunkData->m_Compressed;

		// Get Json data
		Json::Value root;
		std::string JsonData = "";
		a_ChunkData->m_LiveChunk->SaveToJson( root );
		if( !root.empty() )
		{
			Json::StyledWriter writer; // TODO FIXME: change to FastWriter ? :D
			JsonData = writer.write( root );
		}

		unsigned int TotalSize = cChunk::c_BlockDataSize + JsonData.size();
		uLongf CompressedSize = compressBound( TotalSize );
		a_ChunkData->m_Compressed = new char[CompressedSize];
		char* DataSource = a_ChunkData->m_LiveChunk->pGetBlockData();
		if( JsonData.size() > 0 )
		{
			// Move stuff around, so data is aligned in memory
			DataSource = new char[TotalSize];
			memcpy( DataSource, a_ChunkData->m_LiveChunk->pGetBlockData(), cChunk::c_BlockDataSize );
			memcpy( DataSource + cChunk::c_BlockDataSize, JsonData.c_str(), JsonData.size() );
		}

		int errorcode = compress2( (Bytef*)a_ChunkData->m_Compressed, &CompressedSize, (const Bytef*)DataSource, TotalSize, Z_DEFAULT_COMPRESSION);
		if( errorcode != Z_OK )
		{
			LOGERROR("Error compressing data (%i)", errorcode );
		}

		a_ChunkData->m_CompressedSize = CompressedSize;
		a_ChunkData->m_UncompressedSize = TotalSize;

		if( DataSource != a_ChunkData->m_LiveChunk->pGetBlockData() )
			delete [] DataSource;
	}
}





cChunkMap::cChunkLayer* cChunkMap::GetLayerForChunk( int a_ChunkX, int a_ChunkZ )
{
	const int LayerX = (int)(floorf((float)a_ChunkX / (float)(LAYER_SIZE)));
	const int LayerZ = (int)(floorf((float)a_ChunkZ / (float)(LAYER_SIZE)));
	return GetLayer( LayerX, LayerZ );
}





cChunkMap::cChunkLayer* cChunkMap::GetLayer( int a_LayerX, int a_LayerZ )
{
	// Find layer in memory
	for( int i = 0; i < m_NumLayers; ++i )
	{
		if( m_Layers[i].m_X == a_LayerX && m_Layers[i].m_Z == a_LayerZ )
		{
			return &m_Layers[i];
		}
	}

	// Find layer on disk
	cChunkLayer* Layer = LoadLayer( a_LayerX, a_LayerZ );
	if( !Layer ) return 0;
	
	cChunkLayer* NewLayer = AddLayer( *Layer );
	delete Layer;
	return NewLayer;
}





cChunk* cChunkMap::GetChunk( int a_X, int a_Y, int a_Z )
{
	cChunkLayer* Layer = GetLayerForChunk( a_X, a_Z );
	if (Layer == NULL)
	{
		return NULL;
	}
	
	cChunkData* Data = Layer->GetChunk( a_X, a_Z );
	if (Data->m_LiveChunk != NULL)
	{
		// Already loaded and alive
		return Data->m_LiveChunk;
	}

	// Do we at least have the compressed chunk?
	if (Data->m_Compressed == NULL)
	{
		return NULL;
	}

	// The chunk has been cached (loaded from file, but not decompressed):
	uLongf DestSize = Data->m_UncompressedSize;
	char* BlockData = new char[ DestSize ];
	int errorcode = uncompress( (Bytef*)BlockData, &DestSize, (Bytef*)Data->m_Compressed, Data->m_CompressedSize );
	if( Data->m_UncompressedSize != DestSize )
	{
		LOGWARN("Lulwtf, expected uncompressed size differs!");
		delete [] BlockData;
	}
	else if( errorcode != Z_OK )
	{
		LOGERROR("ERROR: Decompressing chunk data! %i", errorcode );
		switch( errorcode )
		{
		case Z_MEM_ERROR:
			LOGERROR("Not enough memory");
			break;
		case Z_BUF_ERROR:
			LOGERROR("Not enough room in output buffer");
			break;
		case Z_DATA_ERROR:
			LOGERROR("Input data corrupted or incomplete");
			break;
		default:
			break;
		};

		delete [] BlockData;
	}
	else
	{
		cChunk* Chunk = new cChunk(a_X, a_Y, a_Z, m_World);
		memcpy( Chunk->m_BlockData, BlockData, cChunk::c_BlockDataSize ); 
		Chunk->CalculateHeightmap();
		Data->m_LiveChunk = Chunk;
		Layer->m_NumChunksLoaded++;

		if( DestSize > cChunk::c_BlockDataSize )	// We gots some extra data :D
		{
			LOGINFO("Parsing trailing JSON");
			Json::Value root;   // will contains the root value after parsing.
			Json::Reader reader;
			if( !reader.parse( BlockData + cChunk::c_BlockDataSize, root, false ) )
			{
				LOGERROR("Failed to parse trailing JSON!");
			}
			else
			{
				Chunk->LoadFromJson( root );
			}
		}

		delete [] BlockData;
		delete [] Data->m_Compressed; Data->m_Compressed = 0; Data->m_CompressedSize = 0;
		return Chunk;
	}
	return NULL;
}





void cChunkMap::Tick( float a_Dt )
{
	for( int lay = 0; lay < m_NumLayers; ++lay )
	{
		for( int i = 0; i < LAYER_SIZE*LAYER_SIZE; ++i )
		{
			cChunk* Chunk = m_Layers[lay].m_Chunks[i].m_LiveChunk;
			if ( Chunk != NULL)
			{
				Chunk->Tick( a_Dt );
			}
		}
	}  // for lay - m_Layers[]
}





void cChunkMap::UnloadUnusedChunks()
{
	cWorld* World = m_World;
	for( int l = 0; l < m_NumLayers; ++l )
	{
		cChunkLayer & Layer = m_Layers[l];
		for( int i = 0; i < LAYER_SIZE*LAYER_SIZE; ++i )
		{
			cChunk* Chunk = Layer.m_Chunks[i].m_LiveChunk;
			if( Chunk && Chunk->GetClients().size() == 0 && Chunk->GetReferenceCount() <= 0 )
			{
				Chunk->SaveToDisk();
				World->RemoveSpread( ptr_cChunk( Chunk ) );
				RemoveChunk( Chunk );
				delete Chunk;
			}
		}

		// Unload layers
		if( Layer.m_NumChunksLoaded == 0 )
		{
			SaveLayer( &Layer );
			for( int i = 0; i < LAYER_SIZE*LAYER_SIZE; ++i ) // Free all chunk data for layer
			{
				if( Layer.m_Chunks[i].m_Compressed )
					delete [] Layer.m_Chunks[i].m_Compressed;
				if( Layer.m_Chunks[i].m_LiveChunk )
					delete Layer.m_Chunks[i].m_LiveChunk;
			}
			if( RemoveLayer( &Layer ) ) l--;
		}
		else if( Layer.m_NumChunksLoaded < 0 )
		{
			LOGERROR("WTF! Chunks loaded in layer is %i !!", Layer.m_NumChunksLoaded );
		}
	}
}





bool cChunkMap::RemoveEntityFromChunk( cEntity & a_Entity, cChunk* a_CalledFrom /* = 0 */ )
{
	for( int i = 0; i < m_NumLayers; ++i )
	{
		cChunkLayer & Layer = m_Layers[i];
		for( int i = 0; i < LAYER_SIZE*LAYER_SIZE; ++i )
		{
			cChunk* Chunk = Layer.m_Chunks[i].m_LiveChunk;
			if( Chunk != a_CalledFrom )
			{
				if( Chunk && Chunk->RemoveEntity( a_Entity, a_CalledFrom ) )
					return true;
			}
		}
	}

	LOG("WARNING: Entity was not found in any chunk!");
	return false;
}





void cChunkMap::SaveAllChunks()
{
	for( int i = 0; i < m_NumLayers; ++i )
	{
		SaveLayer( &m_Layers[i] );
	}
}





/********************************
 * Saving and loading
 **/

void cChunkMap::SaveLayer( cChunkLayer* a_Layer )
{
	std::string WorldName = m_World->GetName();
	cMakeDir::MakeDir( WorldName.c_str() );

	char SourceFile[128];

	sprintf_s(SourceFile, 128, ( WorldName + "/X%i_Z%i.pak").c_str(), a_Layer->m_X, a_Layer->m_Z );

	FILE* f = 0;
	#ifdef _WIN32
	if (fopen_s(&f, SourceFile, "wb" ) != 0 )	// no error
	#else
	if( (f = fopen(SourceFile, "wb" )) == 0 )	// no error
	#endif
	{
		LOGERROR("ERROR: Could not write to file %s", SourceFile );
		return;
	}

	//---------------
	// Header
	char PakVersion = 1;
	char ChunkVersion = 1;
	fwrite( &PakVersion, sizeof(PakVersion), 1, f );	// pak version
	fwrite( &ChunkVersion, sizeof(ChunkVersion), 1, f );	// chunk version

	// Count number of chunks in layer
	short NumChunks = 0;
	for( int i = 0; i < LAYER_SIZE*LAYER_SIZE; ++i )
	{
		if( a_Layer->m_Chunks[i].m_Compressed ||  a_Layer->m_Chunks[i].m_LiveChunk )
		{
			NumChunks++;
		}
	}

	fwrite( &NumChunks, sizeof( NumChunks ), 1, f );
	LOG("Num Chunks in layer: %i", NumChunks );

	//---------------
	// Chunk headers
	for( int z = 0; z < LAYER_SIZE; ++z )
	{
		for( int x = 0; x < LAYER_SIZE; ++x )
		{
			cChunkData & Data = a_Layer->m_Chunks[x + z*LAYER_SIZE];
			CompressChunk( &Data );
			if( Data.m_Compressed || Data.m_LiveChunk )
			{
				int ChunkX = a_Layer->m_X*LAYER_SIZE + x;
				int ChunkZ = a_Layer->m_Z*LAYER_SIZE + z;
				unsigned int Size = Data.m_CompressedSize; // Needs to be size of compressed data
				unsigned int USize = Data.m_UncompressedSize;	// Uncompressed size
				fwrite( &ChunkX, sizeof( ChunkX ), 1, f );
				fwrite( &ChunkZ, sizeof( ChunkZ ), 1, f );
				fwrite( &Size, sizeof( Size ), 1, f );
				fwrite( &USize, sizeof( USize ), 1, f );
			}
		}
	}

	//----------------
	// Chunk data
	for( int i = 0; i < LAYER_SIZE*LAYER_SIZE; ++i )
	{
		char* Compressed = a_Layer->m_Chunks[i].m_Compressed;
		if( Compressed )
		{
			fwrite( Compressed, a_Layer->m_Chunks[i].m_CompressedSize, 1, f );
			if(a_Layer->m_Chunks[i].m_LiveChunk)	// If there's a live chunk we have no need for compressed data
			{
				delete [] a_Layer->m_Chunks[i].m_Compressed;
				a_Layer->m_Chunks[i].m_Compressed = 0;
				a_Layer->m_Chunks[i].m_CompressedSize = 0;
			}
		}
	}

	fclose(f);
}





cChunkMap::cChunkLayer* cChunkMap::LoadLayer(int a_LayerX, int a_LayerZ )
{
	std::string WorldName = m_World->GetName();
	char SourceFile[128];
	
	sprintf_s(SourceFile, 128, (WorldName + "/X%i_Z%i.pak").c_str(), a_LayerX, a_LayerZ );
	
	FILE* f = 0;
	#ifdef _WIN32
	if (fopen_s(&f, SourceFile, "rb" ) != 0 )	// no error
	#else
	if ((f = fopen(SourceFile, "rb" )) == 0 )	// no error
	#endif
	{
		return NULL;
	}
	
	char PakVersion = 0;
	char ChunkVersion = 0;
	if( fread( &PakVersion, sizeof(PakVersion), 1, f) != 1 ) { LOGERROR("ERROR 1 READING FROM FILE %s", SourceFile); fclose(f); return false; }
	if( PakVersion != 1 ) { LOGERROR("WRONG PAK VERSION!"); fclose(f); return 0; }
	if( fread( &ChunkVersion, sizeof(ChunkVersion), 1, f) != 1 ) { LOGERROR("ERROR 2 READING FROM FILE %s", SourceFile); fclose(f); return false; }
	if( ChunkVersion != 1 )
	{
		LOGERROR("WRONG CHUNK VERSION!");
		fclose(f);
		return NULL;
	}

	short NumChunks = 0;
	if( fread( &NumChunks, sizeof(NumChunks), 1, f) != 1 ) 
	{
		LOGERROR("ERROR 3 READING FROM FILE %s", SourceFile);
		fclose(f);
		return NULL;
	}
	
	LOG("Num chunks: %i", NumChunks );
	LOG("Source File: %s", SourceFile );

	cChunkLayer* Layer = new cChunkLayer( LAYER_SIZE*LAYER_SIZE );
	Layer->m_X = a_LayerX;
	Layer->m_Z = a_LayerZ;
	cChunkData** OrderedData = new cChunkData*[ NumChunks ]; // So we can loop over the chunks in the order they were loaded
	
	// Loop over all chunk headers
	for( short i = 0; i < NumChunks; ++i )
	{
		int ChunkX = 0;
		int ChunkZ = 0;
		if( fread( &ChunkX, sizeof(ChunkX), 1, f) != 1 ) { LOGERROR("ERROR 4 READING FROM FILE %s", SourceFile); fclose(f); return false; }
		if( fread( &ChunkZ, sizeof(ChunkZ), 1, f) != 1 ) { LOGERROR("ERROR 5 READING FROM FILE %s", SourceFile); fclose(f); return false; }
		cChunkData* Data = Layer->GetChunk( ChunkX, ChunkZ );
		if( Data )
		{
			if( fread( &Data->m_CompressedSize, sizeof(Data->m_CompressedSize), 1, f) != 1 ) { LOGERROR("ERROR 6 READING FROM FILE %s", SourceFile); fclose(f); return false; }
			if( fread( &Data->m_UncompressedSize, sizeof(Data->m_UncompressedSize), 1, f) != 1 ) { LOGERROR("ERROR 7 READING FROM FILE %s", SourceFile); fclose(f); return false; }
		}
		else
		{
			LOGERROR("Chunk with wrong coordinates in pak file! %i %i", ChunkX, ChunkZ );
			fclose(f);
			return NULL;
		}

		OrderedData[i] = Data;
	}

	// Loop over chunks again, in the order they were loaded, and load their compressed data
	for( short i = 0; i < NumChunks; ++i )
	{
		cChunkData* Data = OrderedData[i];
		Data->m_Compressed = new char[ Data->m_CompressedSize ];
		//printf("Compressed chunk size: %i\n",Data->m_CompressedSize);
		if( fread( Data->m_Compressed, Data->m_CompressedSize, 1, f) != 1 )
		{
			LOGERROR("ERROR 8 READING FROM FILE %s", SourceFile); 
			fclose(f); 
			return NULL;
		}
	}
	delete [] OrderedData;

	fclose(f);
	return Layer;
}





int cChunkMap::GetNumChunks()
{
	int NumChunks = 0;
	for( int i = 0; i < m_NumLayers; ++i )
	{
		NumChunks += m_Layers[i].m_NumChunksLoaded;
	}
	return NumChunks;
}




