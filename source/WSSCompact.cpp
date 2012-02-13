
// WSSCompact.cpp

// Interfaces to the cWSSCompact class representing the "compact" storage schema (PAK-files)

#include "Globals.h"
#include "WSSCompact.h"
#include "cWorld.h"
#include "zlib.h"
#include <json/json.h>





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





bool cWSSCompact::LoadChunk(const cChunkPtr & a_Chunk)
{
	cPAKFile * f = LoadPAKFile(a_Chunk);
	if (f == NULL)
	{
		// For some reason we couldn't locate the file
		return false;
	}
	
	return f->LoadChunk(a_Chunk);
}





bool cWSSCompact::SaveChunk(const cChunkPtr & a_Chunk)
{
	cPAKFile * f = LoadPAKFile(a_Chunk);
	if (f == NULL)
	{
		// For some reason we couldn't locate the file
		return false;
	}
	return f->SaveChunk(a_Chunk);
}





cWSSCompact::cPAKFile * cWSSCompact::LoadPAKFile(const cChunkPtr & a_Chunk)
{
	// We need to retain this weird conversion code, because some edge chunks are in the wrong PAK file
	const int LayerX = (int)(floorf((float)a_Chunk->GetPosX() / 32.0f));
	const int LayerZ = (int)(floorf((float)a_Chunk->GetPosZ() / 32.0f));
	
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





bool cWSSCompact::cPAKFile::LoadChunk(const cChunkPtr & a_Chunk)
{
	int ChunkX = a_Chunk->GetPosX();
	int ChunkZ = a_Chunk->GetPosZ();
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

	return LoadChunk(a_Chunk, Offset, Header);
}





bool cWSSCompact::cPAKFile::SaveChunk(const cChunkPtr & a_Chunk)
{
	if (!SaveChunkToData(a_Chunk))
	{
		return false;
	}
	if (m_NumDirty > MAX_DIRTY_CHUNKS)
	{
		SynchronizeFile();
	}
	return true;
}





bool cWSSCompact::cPAKFile::LoadChunk(const cChunkPtr & a_Chunk, int a_Offset, sChunkHeader * a_Header)
{
	// Decompress the data:
	uLongf DestSize = a_Header->m_UncompressedSize;
	std::auto_ptr<char> BlockData(new char[ DestSize ]);
	int errorcode = uncompress( (Bytef*)BlockData.get(), &DestSize, (Bytef*)m_DataContents.data() + a_Offset, a_Header->m_CompressedSize );
	if (errorcode != Z_OK)
	{
		LOGERROR("Error %d decompressing data for chunk [%d, %d] from file \"%s\"", 
			errorcode,
			a_Chunk->GetPosX(), a_Chunk->GetPosZ(),
			m_FileName.c_str()
		);
		return false;
	}
	
	if (a_Header->m_UncompressedSize != DestSize)
	{
		LOGWARNING("Uncompressed data size differs (exp %d, got %d) for chunk [%d, %d] from file \"%s\"",
			a_Header->m_UncompressedSize, DestSize,
			a_Chunk->GetPosX(), a_Chunk->GetPosZ(),
			m_FileName.c_str()
		);
		return false;
	}

	a_Chunk->CopyBlockDataFrom(BlockData.get());
	a_Chunk->SetValid();

	if (DestSize > cChunk::c_BlockDataSize )	// We gots some extra data :D
	{
		LOGINFO("Parsing trailing JSON");
		Json::Value root;   // will contain the root value after parsing.
		Json::Reader reader;
		if ( !reader.parse( BlockData.get() + cChunk::c_BlockDataSize, root, false ) )
		{
			LOGERROR("Failed to parse trailing JSON!");
		}
		else
		{
			a_Chunk->LoadFromJson( root );
		}
	}

	return true;
}





void cWSSCompact::cPAKFile::EraseChunk(const cChunkPtr & a_Chunk)
{
	int ChunkX = a_Chunk->GetPosX();
	int ChunkZ = a_Chunk->GetPosZ();
	sChunkHeader * Header = NULL;
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





bool cWSSCompact::cPAKFile::SaveChunkToData(const cChunkPtr & a_Chunk)
{
	// Erase any existing data for the chunk:
	EraseChunk(a_Chunk);
	
	// Serialize the chunk:
	AString Data;
	Data.assign(a_Chunk->pGetBlockData(), cChunk::c_BlockDataSize);
	Json::Value root;
	a_Chunk->SaveToJson( root );
	if (!root.empty())
	{
		AString JsonData;
		Json::StyledWriter writer;
		JsonData = writer.write( root );
		Data.append(JsonData);
	}
	
	// Compress the data:
	uLongf CompressedSize = compressBound(Data.size());
	std::auto_ptr<char> Compressed(new char[CompressedSize]);
	int errorcode = compress2( (Bytef*)Compressed.get(), &CompressedSize, (const Bytef*)Data.data(), Data.size(), Z_DEFAULT_COMPRESSION);
	if ( errorcode != Z_OK )
	{
		LOGERROR("Error %i compressing data for chunk [%d, %d]", errorcode, a_Chunk->GetPosX(), a_Chunk->GetPosZ() );
		return false;
	}
	
	// Save the header:
	sChunkHeader * Header = new sChunkHeader;
	if (Header == NULL)
	{
		return false;
	}
	Header->m_CompressedSize = CompressedSize;
	Header->m_ChunkX = a_Chunk->GetPosX();
	Header->m_ChunkZ = a_Chunk->GetPosZ();
	Header->m_UncompressedSize = Data.size();
	m_ChunkHeaders.push_back(Header);
	
	m_DataContents.append(Compressed.get(), CompressedSize);
	
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
	if (f.Write(m_DataContents.data(), m_DataContents.size()) != m_DataContents.size())
	{
		LOGERROR("cWSSCompact: ERROR writing chunk contents to file \"%s\" (line %d); file offset %d", m_FileName.c_str(), __LINE__, f.Tell());
		return;
	}
	m_NumDirty = 0;
}




