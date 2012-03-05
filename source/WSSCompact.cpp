
// WSSCompact.cpp

// Interfaces to the cWSSCompact class representing the "compact" storage schema (PAK-files)

#include "Globals.h"
#include "WSSCompact.h"
#include "cWorld.h"
#include "zlib.h"
#include <json/json.h>
#include "StringCompression.h"
#include "cChestEntity.h"
#include "cSignEntity.h"
#include "cFurnaceEntity.h"
#include "BlockID.h"





#pragma pack(push, 1)
/// The chunk header, as stored in the file:
struct cWSSCompact::sChunkHeader
{
	int m_ChunkX;
	int m_ChunkZ;
	int m_CompressedSize;
	int m_UncompressedSize;
} ;
#pragma pack(pop)





/// The maximum number of PAK files that are cached
const int MAX_PAK_FILES = 16;

/// The maximum number of unsaved chunks before the cPAKFile saves them to disk
const int MAX_DIRTY_CHUNKS = 16;





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cWSSCompact:

cWSSCompact::~cWSSCompact()
{
	for (cPAKFiles::iterator itr = m_PAKFiles.begin(); itr != m_PAKFiles.end(); ++itr)
	{
		delete *itr;
	}
}





bool cWSSCompact::LoadChunk(const cChunkCoords & a_Chunk)
{
	AString ChunkData;
	int UncompressedSize = 0;
	if (!GetChunkData(a_Chunk, UncompressedSize, ChunkData))
	{
		// The reason for failure is already printed in GetChunkData()
		return false;
	}
	
	return LoadChunkFromData(a_Chunk, UncompressedSize, ChunkData, m_World);
}





bool cWSSCompact::SaveChunk(const cChunkCoords & a_Chunk)
{
	cCSLock Lock(m_CS);
	
	cPAKFile * f = LoadPAKFile(a_Chunk);
	if (f == NULL)
	{
		// For some reason we couldn't locate the file
		LOG("Cannot locate a proper PAK file for chunk [%d, %d]", a_Chunk.m_ChunkX, a_Chunk.m_ChunkZ);
		return false;
	}
	return f->SaveChunk(a_Chunk, m_World);
}





cWSSCompact::cPAKFile * cWSSCompact::LoadPAKFile(const cChunkCoords & a_Chunk)
{
	// ASSUMES that m_CS has been locked
	
	// We need to retain this weird conversion code, because some edge chunks are in the wrong PAK file
	const int LayerX = (int)(floorf((float)a_Chunk.m_ChunkX / 32.0f));
	const int LayerZ = (int)(floorf((float)a_Chunk.m_ChunkZ / 32.0f));
	
	// Is it already cached?
	for (cPAKFiles::iterator itr = m_PAKFiles.begin(); itr != m_PAKFiles.end(); ++itr)
	{
		if (((*itr) != NULL) && ((*itr)->GetLayerX() == LayerX) && ((*itr)->GetLayerZ() == LayerZ))
		{
			// Move the file to front and return it:
			cPAKFile * f = *itr;
			if (itr != m_PAKFiles.begin())
			{
				m_PAKFiles.erase(itr);
				m_PAKFiles.push_front(f);
			}
			return f;
		}
	}
	
	// Load it anew:
	AString FileName;
	Printf(FileName, "%s/X%i_Z%i.pak", m_World->GetName().c_str(), LayerX, LayerZ );
	cPAKFile * f = new cPAKFile(FileName, LayerX, LayerZ);
	if (f == NULL)
	{
		return NULL;
	}
	m_PAKFiles.push_front(f);
	
	// If there are too many PAK files cached, delete the last one used:
	if (m_PAKFiles.size() > MAX_PAK_FILES)
	{
		delete m_PAKFiles.back();
		m_PAKFiles.pop_back();
	}
	return f;
}





bool cWSSCompact::GetChunkData(const cChunkCoords & a_Chunk, int & a_UncompressedSize, AString & a_Data)
{
	cCSLock Lock(m_CS);
	cPAKFile * f = LoadPAKFile(a_Chunk);
	if (f == NULL)
	{
		return false;
	}
	return f->GetChunkData(a_Chunk, a_UncompressedSize, a_Data);
}





/*
// TODO: Rewrite saving to use the same principles as loading
bool cWSSCompact::SetChunkData(const cChunkCoords & a_Chunk, int a_UncompressedSize, const AString & a_Data)
{
	cCSLock Lock(m_CS);
	cPAKFile * f = LoadPAKFile(a_Chunk);
	if (f == NULL)
	{
		return false;
	}
	return f->SetChunkData(a_Chunk, a_UncompressedSize, a_Data);
}
*/





bool cWSSCompact::EraseChunkData(const cChunkCoords & a_Chunk)
{
	cCSLock Lock(m_CS);
	cPAKFile * f = LoadPAKFile(a_Chunk);
	if (f == NULL)
	{
		return false;
	}
	return f->EraseChunkData(a_Chunk);
}





void cWSSCompact::LoadEntitiesFromJson(Json::Value & a_Value, cEntityList & a_Entities, cBlockEntityList & a_BlockEntities, cWorld * a_World)
{
	// Load chests
	Json::Value AllChests = a_Value.get("Chests", Json::nullValue);
	if (!AllChests.empty())
	{
		for (Json::Value::iterator itr = AllChests.begin(); itr != AllChests.end(); ++itr )
		{
			Json::Value & Chest = *itr;
			cChestEntity * ChestEntity = new cChestEntity(0,0,0, a_World);
			if (!ChestEntity->LoadFromJson( Chest ) )
			{
				LOGERROR("ERROR READING CHEST FROM JSON!" );
				delete ChestEntity;
			}
			else
			{
				a_BlockEntities.push_back( ChestEntity );
			}
		}  // for itr - AllChests[]
	}

	// Load furnaces
	Json::Value AllFurnaces = a_Value.get("Furnaces", Json::nullValue);
	if( !AllFurnaces.empty() )
	{
		for( Json::Value::iterator itr = AllFurnaces.begin(); itr != AllFurnaces.end(); ++itr )
		{
			Json::Value & Furnace = *itr;
			cFurnaceEntity * FurnaceEntity = new cFurnaceEntity(0,0,0, a_World);
			if( !FurnaceEntity->LoadFromJson( Furnace ) )
			{
				LOGERROR("ERROR READING FURNACE FROM JSON!" );
				delete FurnaceEntity;
			}
			else
			{
				a_BlockEntities.push_back( FurnaceEntity );
			}
		}  // for itr - AllFurnaces[]
	}

	// Load signs
	Json::Value AllSigns = a_Value.get("Signs", Json::nullValue);
	if( !AllSigns.empty() )
	{
		for( Json::Value::iterator itr = AllSigns.begin(); itr != AllSigns.end(); ++itr )
		{
			Json::Value & Sign = *itr;
			cSignEntity * SignEntity = new cSignEntity( E_BLOCK_SIGN_POST, 0,0,0, a_World);
			if ( !SignEntity->LoadFromJson( Sign ) )
			{
				LOGERROR("ERROR READING SIGN FROM JSON!" );
				delete SignEntity;
			}
			else
			{
				a_BlockEntities.push_back( SignEntity );
			}
		}  // for itr - AllSigns[]
	}
}





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cWSSCompact::cPAKFile

#define READ(Var) \
	if (f.Read(&Var, sizeof(Var)) != sizeof(Var)) \
	{ \
		LOGERROR("ERROR READING %s FROM FILE %s (line %d); file offset %d", #Var, m_FileName.c_str(), __LINE__, f.Tell()); \
		return; \
	}

cWSSCompact::cPAKFile::cPAKFile(const AString & a_FileName, int a_LayerX, int a_LayerZ) :
	m_FileName(a_FileName),
	m_LayerX(a_LayerX),
	m_LayerZ(a_LayerZ),
	m_NumDirty(0),
	m_ChunkVersion( PAK_VERSION ), // Init with latest version
	m_PakVersion( CHUNK_VERSION )
{
	cFile f;
	if (!f.Open(m_FileName, cFile::fmRead))
	{
		return;
	}
	
	// Read headers:
	READ(m_PakVersion);
	if (m_PakVersion != 1)
	{
		LOGERROR("File \"%s\" is in an unknown pak format (%d)", m_FileName.c_str(), m_PakVersion);
		return;
	}
	
	READ(m_ChunkVersion);
	switch( m_ChunkVersion )
	{
	case 1:
		m_ChunkSize.Set(16, 128, 16);
		break;
	case 2:
		m_ChunkSize.Set(16, 256, 16);
		break;
	default:
		LOGERROR("File \"%s\" is in an unknown chunk format (%d)", m_FileName.c_str(), m_ChunkVersion);
		return;
	};
	
	short NumChunks = 0;
	READ(NumChunks);
	
	// Read chunk headers:
	for (int i = 0; i < NumChunks; i++)
	{
		sChunkHeader * Header = new sChunkHeader;
		READ(*Header);
		m_ChunkHeaders.push_back(Header);
	}  // for i - chunk headers

	// Read chunk data:
	if (f.ReadRestOfFile(m_DataContents) == -1)
	{
		LOGERROR("Cannot read file \"%s\" contents", m_FileName.c_str());
		return;
	}

	if( m_ChunkVersion == 1 ) // Convert chunks to version 2
	{
		UpdateChunk1To2();
	}
}





cWSSCompact::cPAKFile::~cPAKFile()
{
	if (m_NumDirty > 0)
	{
		SynchronizeFile();
	}
	for (sChunkHeaders::iterator itr = m_ChunkHeaders.begin(); itr != m_ChunkHeaders.end(); ++itr)
	{
		delete *itr;
	}
}





bool cWSSCompact::cPAKFile::GetChunkData(const cChunkCoords & a_Chunk, int & a_UncompressedSize, AString & a_Data)
{
	int ChunkX = a_Chunk.m_ChunkX;
	int ChunkZ = a_Chunk.m_ChunkZ;
	sChunkHeader * Header = NULL;
	int Offset = 0;
	for (sChunkHeaders::iterator itr = m_ChunkHeaders.begin(); itr != m_ChunkHeaders.end(); ++itr)
	{
		if (((*itr)->m_ChunkX == ChunkX) && ((*itr)->m_ChunkZ == ChunkZ))
		{
			Header = *itr;
			break;
		}
		Offset += (*itr)->m_CompressedSize;
	}
	if ((Header == NULL) || (Offset + Header->m_CompressedSize > (int)m_DataContents.size()))
	{
		// Chunk not found / data invalid
		return false;
	}

	a_UncompressedSize = Header->m_UncompressedSize;
	a_Data.assign(m_DataContents, Offset, Header->m_CompressedSize);
	return true;
}





bool cWSSCompact::cPAKFile::SaveChunk(const cChunkCoords & a_Chunk, cWorld * a_World)
{
	if (!SaveChunkToData(a_Chunk, a_World))
	{
		return false;
	}
	if (m_NumDirty > MAX_DIRTY_CHUNKS)
	{
		SynchronizeFile();
	}
	return true;
}





void cWSSCompact::cPAKFile::UpdateChunk1To2()
{
	LOGINFO("Updating \"%s\" version 1 to version 2", m_FileName.c_str() );
	int Offset = 0;
	AString NewDataContents;
	int ChunksConverted = 0;
	for (sChunkHeaders::iterator itr = m_ChunkHeaders.begin(); itr != m_ChunkHeaders.end(); ++itr)
	{
		sChunkHeader * Header = *itr;

		
		if( ChunksConverted % 32 == 0 )
		{
			LOGINFO("Updating \"%s\" version 1 to version 2: %d%%", m_FileName.c_str(), (ChunksConverted*100) / m_ChunkHeaders.size() );
		}
		ChunksConverted++;
		

		AString Data;
		int UncompressedSize = Header->m_UncompressedSize;
		Data.assign(m_DataContents, Offset, Header->m_CompressedSize);
		Offset += Header->m_CompressedSize;

		// Crude data integrity check:
		int ExpectedSize = (16*128*16)*2 + (16*128*16)/2; // For version 1
		if (UncompressedSize < ExpectedSize)
		{
			LOGWARNING("Chunk [%d, %d] has too short decompressed data (%d bytes out of %d needed), erasing",
				Header->m_ChunkX, Header->m_ChunkZ,
				UncompressedSize, ExpectedSize
				);
			Offset += Header->m_CompressedSize;
			continue;
		}

		// Decompress the data:
		AString UncompressedData;
		{
			int errorcode = UncompressString(Data.data(), Data.size(), UncompressedData, UncompressedSize);
			if (errorcode != Z_OK)
			{
				LOGERROR("Error %d decompressing data for chunk [%d, %d]", 
					errorcode,
					Header->m_ChunkX, Header->m_ChunkZ
					);
				Offset += Header->m_CompressedSize;
				continue;
			}
		}

		if (UncompressedSize != (int)UncompressedData.size())
		{
			LOGWARNING("Uncompressed data size differs (exp %d bytes, got %d) for chunk [%d, %d]",
				UncompressedSize, UncompressedData.size(),
				Header->m_ChunkX, Header->m_ChunkZ
				);
			Offset += Header->m_CompressedSize;
			continue;
		}


		// Old version is 128 blocks high with YZX axis order
		AString ConvertedData;
		ConvertedData.reserve(cChunk::c_BlockDataSize);  // Pre-alloc, so that push_back() and append() don't need to re-alloc
		unsigned int InChunkOffset = 0;
		for( int x = 0; x < 16; ++x ) for( int z = 0; z < 16; ++z ) 
		{
			for( int y = 0; y < 128; ++y )
			{
				ConvertedData.push_back( UncompressedData[y + z*128 + x*128*16 + InChunkOffset] );
			}
			// Add 128 empty blocks after an old y column
			ConvertedData.append( 128, E_BLOCK_AIR );
		}
		InChunkOffset += (16*128*16);
		for( int x = 0; x < 16; ++x ) for( int z = 0; z < 16; ++z ) // Metadata
		{
			for( int y = 0; y < 64; ++y )
			{
				ConvertedData.push_back( UncompressedData[y + z*64 + x*64*16 + InChunkOffset] );
			}
			ConvertedData.append( 64, 0 );
		}
		InChunkOffset += (16*128*16)/2;
		for( int x = 0; x < 16; ++x ) for( int z = 0; z < 16; ++z ) // Block light
		{
			for( int y = 0; y < 64; ++y )
			{
				ConvertedData.push_back( UncompressedData[y + z*64 + x*64*16 + InChunkOffset] );
			}
			ConvertedData.append( 64, 0 );
		}
		InChunkOffset += (16*128*16)/2;
		for( int x = 0; x < 16; ++x ) for( int z = 0; z < 16; ++z ) // Sky light
		{
			for( int y = 0; y < 64; ++y )
			{
				ConvertedData.push_back( UncompressedData[y + z*64 + x*64*16 + InChunkOffset] );
			}
			ConvertedData.append( 64, 0 );
		}
		InChunkOffset += (16*128*16)/2;
		// Add JSON data afterwards
		if( UncompressedData.size() > InChunkOffset )
		{
			ConvertedData.append( UncompressedData.begin() + InChunkOffset, UncompressedData.end() );
		}

		// Re-compress data
		AString CompressedData;
		{
			int errorcode = CompressString(ConvertedData.data(), ConvertedData.size(), CompressedData);
			if (errorcode != Z_OK)
			{
				LOGERROR("Error %d compressing data for chunk [%d, %d]", 
					errorcode,
					Header->m_ChunkX, Header->m_ChunkZ
					);
				continue;
			}
		}

		Header->m_UncompressedSize = ConvertedData.size();
		Header->m_CompressedSize = CompressedData.size();


		NewDataContents.append( CompressedData );
		m_NumDirty++;
	}

	// Done converting
	m_DataContents = NewDataContents;
	m_ChunkVersion = 2;
	LOGINFO("Updated \"%s\" version 1 to version 2", m_FileName.c_str() );
}





bool cWSSCompact::LoadChunkFromData(const cChunkCoords & a_Chunk, int & a_UncompressedSize, const AString & a_Data, cWorld * a_World)
{
	// Crude data integrity check:
	if (a_UncompressedSize < cChunk::c_BlockDataSize)
	{
		LOGWARNING("Chunk [%d, %d] has too short decompressed data (%d bytes out of %d needed), erasing",
			a_Chunk.m_ChunkX, a_Chunk.m_ChunkZ,
			a_UncompressedSize, cChunk::c_BlockDataSize
		);
		EraseChunkData(a_Chunk);
		return false;
	}
	
	// Decompress the data:
	AString UncompressedData;
	int errorcode = UncompressString(a_Data.data(), a_Data.size(), UncompressedData, a_UncompressedSize);
	if (errorcode != Z_OK)
	{
		LOGERROR("Error %d decompressing data for chunk [%d, %d]", 
			errorcode,
			a_Chunk.m_ChunkX, a_Chunk.m_ChunkZ
		);
		return false;
	}
	
	if (a_UncompressedSize != (int)UncompressedData.size())
	{
		LOGWARNING("Uncompressed data size differs (exp %d bytes, got %d) for chunk [%d, %d]",
			a_UncompressedSize, UncompressedData.size(),
			a_Chunk.m_ChunkX, a_Chunk.m_ChunkZ
		);
		return false;
	}

	cEntityList      Entities;
	cBlockEntityList BlockEntities;
	
	if (a_UncompressedSize > cChunk::c_BlockDataSize)
	{
		Json::Value root;   // will contain the root value after parsing.
		Json::Reader reader;
		if ( !reader.parse( UncompressedData.data() + cChunk::c_BlockDataSize, root, false ) )
		{
			LOGERROR("Failed to parse trailing JSON in chunk [%d, %d]!",
				a_Chunk.m_ChunkX, a_Chunk.m_ChunkZ
			);
		}
		else
		{
			LoadEntitiesFromJson(root, Entities, BlockEntities, a_World);
		}
	}

	a_World->ChunkDataLoaded(a_Chunk.m_ChunkX, a_Chunk.m_ChunkY, a_Chunk.m_ChunkZ, UncompressedData.data(), Entities, BlockEntities);

	return true;
}





bool cWSSCompact::cPAKFile::EraseChunkData(const cChunkCoords & a_Chunk)
{
	int ChunkX = a_Chunk.m_ChunkX;
	int ChunkZ = a_Chunk.m_ChunkZ;
	int Offset = 0;
	for (sChunkHeaders::iterator itr = m_ChunkHeaders.begin(); itr != m_ChunkHeaders.end(); ++itr)
	{
		if (((*itr)->m_ChunkX == ChunkX) && ((*itr)->m_ChunkZ == ChunkZ))
		{
			m_DataContents.erase(Offset, (*itr)->m_CompressedSize);
			delete *itr;
			itr = m_ChunkHeaders.erase(itr);
			return true;
		}
		Offset += (*itr)->m_CompressedSize;
	}
	
	return false;
}





bool cWSSCompact::cPAKFile::SaveChunkToData(const cChunkCoords & a_Chunk, cWorld * a_World)
{
	// Serialize the chunk:
	cJsonChunkSerializer Serializer;
	a_World->GetChunkData(a_Chunk.m_ChunkX, a_Chunk.m_ChunkY, a_Chunk.m_ChunkZ, &Serializer);
	if (Serializer.GetBlockData().empty())
	{
		// Chunk not valid
		LOG("cWSSCompact: Trying to save chunk [%d, %d, %d] that has no data, ignoring request.", a_Chunk.m_ChunkX, a_Chunk.m_ChunkY, a_Chunk.m_ChunkZ);
		return false;
	}

	AString Data;
	std::swap(Serializer.GetBlockData(), Data);
	if (Serializer.HasJsonData())
	{
		AString JsonData;
		Json::StyledWriter writer;
		JsonData = writer.write(Serializer.GetRoot());
		Data.append(JsonData);
	}
	
	// Compress the data:
	AString CompressedData;
	int errorcode = CompressString(Data.data(), Data.size(), CompressedData);
	if ( errorcode != Z_OK )
	{
		LOGERROR("Error %i compressing data for chunk [%d, %d, %d]", errorcode, a_Chunk.m_ChunkX, a_Chunk.m_ChunkY, a_Chunk.m_ChunkZ);
		return false;
	}
	
	// Erase any existing data for the chunk:
	EraseChunkData(a_Chunk);
	
	// Save the header:
	sChunkHeader * Header = new sChunkHeader;
	if (Header == NULL)
	{
		LOGWARNING("Cannot create a new chunk header to save chunk [%d, %d, %d]", a_Chunk.m_ChunkX, a_Chunk.m_ChunkY, a_Chunk.m_ChunkZ);
		return false;
	}
	Header->m_CompressedSize = (int)CompressedData.size();
	Header->m_ChunkX = a_Chunk.m_ChunkX;
	Header->m_ChunkZ = a_Chunk.m_ChunkZ;
	Header->m_UncompressedSize = (int)Data.size();
	m_ChunkHeaders.push_back(Header);
	
	m_DataContents.append(CompressedData.data(), CompressedData.size());
	
	m_NumDirty++;
	return true;
}





#define WRITE(Var) \
	if (f.Write(&Var, sizeof(Var)) != sizeof(Var)) \
	{ \
		LOGERROR("cWSSCompact: ERROR writing %s to file \"%s\" (line %d); file offset %d", #Var, m_FileName.c_str(), __LINE__, f.Tell()); \
		return; \
	}

void cWSSCompact::cPAKFile::SynchronizeFile(void)
{
	cFile f;
	if (!f.Open(m_FileName, cFile::fmWrite))
	{
		LOGERROR("Cannot open PAK file \"%s\" for writing", m_FileName.c_str());
		return;
	}
	
	WRITE(m_PakVersion);
	WRITE(m_ChunkVersion);
	short NumChunks = (short)m_ChunkHeaders.size();
	WRITE(NumChunks);
	for (sChunkHeaders::iterator itr = m_ChunkHeaders.begin(); itr != m_ChunkHeaders.end(); ++itr)
	{
		WRITE(**itr);
	}
	if (f.Write(m_DataContents.data(), m_DataContents.size()) != (int)m_DataContents.size())
	{
		LOGERROR("cWSSCompact: ERROR writing chunk contents to file \"%s\" (line %d); file offset %d", m_FileName.c_str(), __LINE__, f.Tell());
		return;
	}
	m_NumDirty = 0;
}




