
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
	cPAKFile * f = LoadPAKFile(a_Chunk);
	if (f == NULL)
	{
		// For some reason we couldn't locate the file
		return false;
	}
	
	return f->LoadChunk(a_Chunk, m_World);
}





bool cWSSCompact::SaveChunk(const cChunkCoords & a_Chunk)
{
	cPAKFile * f = LoadPAKFile(a_Chunk);
	if (f == NULL)
	{
		// For some reason we couldn't locate the file
		return false;
	}
	return f->SaveChunk(a_Chunk, m_World);
}





cWSSCompact::cPAKFile * cWSSCompact::LoadPAKFile(const cChunkCoords & a_Chunk)
{
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
	m_NumDirty(0)
{
	cFile f;
	if (!f.Open(m_FileName, cFile::fmRead))
	{
		return;
	}
	
	// Read headers:
	char PakVersion = 0;
	READ(PakVersion);
	if (PakVersion != 1)
	{
		LOGERROR("File \"%s\" is in an unknown pak format (%d)", m_FileName.c_str(), PakVersion);
		return;
	}
	
	char ChunkVersion = 0;
	READ(ChunkVersion);
	if (ChunkVersion != 1)
	{
		LOGERROR("File \"%s\" is in an unknown chunk format (%d)", m_FileName.c_str(), ChunkVersion);
		return;
	}
	
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





bool cWSSCompact::cPAKFile::LoadChunk(const cChunkCoords & a_Chunk, cWorld * a_World)
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

	return LoadChunk(a_Chunk, Offset, Header, a_World);
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





bool cWSSCompact::cPAKFile::LoadChunk(const cChunkCoords & a_Chunk, int a_Offset, sChunkHeader * a_Header, cWorld * a_World)
{
	// Crude data integrity check:
	if (a_Header->m_UncompressedSize < cChunk::c_BlockDataSize)
	{
		LOGWARNING("Chunk [%d, %d] has too short decompressed data (%d out of %d needed), erasing",
			a_Chunk.m_ChunkX, a_Chunk.m_ChunkZ,
			a_Header->m_UncompressedSize, cChunk::c_BlockDataSize
		);
		EraseChunk(a_Chunk);
		m_NumDirty++;
		return false;
	}
	
	// Decompress the data:
	AString UncompressedData;
	int errorcode = UncompressString(m_DataContents.data() + a_Offset, a_Header->m_CompressedSize, UncompressedData, a_Header->m_UncompressedSize);
	if (errorcode != Z_OK)
	{
		LOGERROR("Error %d decompressing data for chunk [%d, %d] from file \"%s\"", 
			errorcode,
			a_Chunk.m_ChunkX, a_Chunk.m_ChunkZ,
			m_FileName.c_str()
		);
		return false;
	}
	
	if (a_Header->m_UncompressedSize != (int)UncompressedData.size())
	{
		LOGWARNING("Uncompressed data size differs (exp %d, got %d) for chunk [%d, %d] from file \"%s\"",
			a_Header->m_UncompressedSize, UncompressedData.size(),
			a_Chunk.m_ChunkX, a_Chunk.m_ChunkZ,
			m_FileName.c_str()
		);
		return false;
	}

	cEntityList      Entities;
	cBlockEntityList BlockEntities;
	
	if (a_Header->m_UncompressedSize > cChunk::c_BlockDataSize )	// We gots some extra data :D
	{
		LOGINFO("Parsing trailing JSON");
		Json::Value root;   // will contain the root value after parsing.
		Json::Reader reader;
		if ( !reader.parse( UncompressedData.data() + cChunk::c_BlockDataSize, root, false ) )
		{
			LOGERROR("Failed to parse trailing JSON!");
		}
		else
		{
			LoadEntitiesFromJson(root, Entities, BlockEntities, a_World);
		}
	}

	a_World->ChunkDataLoaded(a_Chunk.m_ChunkX, 0, a_Chunk.m_ChunkZ, UncompressedData.data(), Entities, BlockEntities);

	return true;
}





void cWSSCompact::cPAKFile::EraseChunk(const cChunkCoords & a_Chunk)
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
			return;
		}
		Offset += (*itr)->m_CompressedSize;
	}
}





bool cWSSCompact::cPAKFile::SaveChunkToData(const cChunkCoords & a_Chunk, cWorld * a_World)
{
	// Serialize the chunk:
	cJsonChunkSerializer Serializer;
	a_World->GetChunkData(a_Chunk.m_ChunkX, 0, a_Chunk.m_ChunkZ, &Serializer);
	if (Serializer.GetBlockData().empty())
	{
		// Chunk not valid
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
		LOGERROR("Error %i compressing data for chunk [%d, %d]", errorcode, a_Chunk.m_ChunkX, a_Chunk.m_ChunkZ);
		return false;
	}
	
	// Erase any existing data for the chunk:
	EraseChunk(a_Chunk);
	
	// Save the header:
	sChunkHeader * Header = new sChunkHeader;
	if (Header == NULL)
	{
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
	
	char PakVersion = 1;
	WRITE(PakVersion);
	char ChunkVersion = 1;
	WRITE(ChunkVersion);
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





void cWSSCompact::cPAKFile::LoadEntitiesFromJson(Json::Value & a_Value, cEntityList & a_Entities, cBlockEntityList & a_BlockEntities, cWorld * a_World)
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




