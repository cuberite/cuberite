
// Processor.cpp

// Implements the cProcessor class representing the overall processor engine that manages threads, calls callbacks etc.

#include "Globals.h"
#include "Processor.h"
#include "Callback.h"
#include "../source/WorldStorage/FastNBT.h"
#include "zlib.h"





const int CHUNK_INFLATE_MAX = 1 MiB;





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cProcessor::cThread:

cProcessor::cThread::cThread(cCallback & a_Callback, cProcessor & a_ParentProcessor) :
	super("cProcessor::cThread"),
	m_Callback(a_Callback),
	m_ParentProcessor(a_ParentProcessor)
{
	super::Start();
}





void cProcessor::cThread::Execute(void)
{
	for (;;)
	{
		AString FileName = m_ParentProcessor.GetOneFileName();
		if (FileName.empty())
		{
			// All done, terminate the thread
			return;
		}
		ProcessFile(FileName);
	}  // for-ever
}





void cProcessor::cThread::ProcessFile(const AString & a_FileName)
{
	LOG("Processing file \"%s\"", a_FileName.c_str());
	
	size_t idx = a_FileName.rfind("r.");
	if (idx == AString::npos)
	{
		LOG("Cannot parse filename \"%s\", skipping file.", a_FileName.c_str());
		return;
	}
	int RegionX = 0, RegionZ = 0;
	if (sscanf_s(a_FileName.c_str() + idx, "r.%d.%d.mca", &RegionX, &RegionZ) != 2)
	{
		LOG("Cannot parse filename \"%s\" into coords, skipping file.", a_FileName.c_str());
		return;
	}

	cFile f;
	if (!f.Open(a_FileName, cFile::fmRead))
	{
		LOG("Cannot open file \"%s\", skipping file.", a_FileName.c_str());
		return;
	}
	
	AString FileContents;
	f.ReadRestOfFile(FileContents);
	if (FileContents.size() < sizeof(8 KiB))
	{
		LOG("Cannot read header in file \"%s\", skipping file.", a_FileName.c_str());
		return;
	}
	
	ProcessFileData(FileContents.data(), FileContents.size(), RegionX * 32, RegionZ * 32);
}





void cProcessor::cThread::ProcessFileData(const char * a_FileData, size_t a_Size, int a_ChunkBaseX, int a_ChunkBaseZ)
{
	int Header[2048];
	int * HeaderPtr = (int *)a_FileData;
	for (int i = 0; i < ARRAYCOUNT(Header); i++)
	{
		Header[i] = ntohl(HeaderPtr[i]);
	}
	
	for (int i = 0; i < 1024; i++)
	{
		unsigned Location = Header[i];
		unsigned Timestamp = Header[i + 1024];
		if (
			((Location == 0) && (Timestamp == 0)) || // Official docs' "not present"
			(Location >> 8 < 2)                   || // Logical - no chunk can start inside the header
			((Location & 0xff) == 0)              || // Logical - no chunk can be zero bytes
			((Location >> 8) * 4096 > a_Size)        // Logical - no chunk can start at beyond the file end
		)
		{
			// Chunk not present in the file
			continue;
		}
		int ChunkX = a_ChunkBaseX + (i % 32);
		int ChunkZ = a_ChunkBaseZ + (i / 32);
		if (m_Callback.OnNewChunk(ChunkX, ChunkZ))
		{
			continue;
		}
		ProcessChunk(a_FileData, ChunkX, ChunkZ, Location >> 8, Location & 0xff, Timestamp);
	}  // for i - chunk index
}





void cProcessor::cThread::ProcessChunk(const char * a_FileData, int a_ChunkX, int a_ChunkZ, unsigned a_SectorStart, unsigned a_SectorSize, unsigned a_TimeStamp)
{
	if (m_Callback.OnHeader(a_SectorStart * 4096, a_SectorSize, a_TimeStamp))
	{
		return;
	}
	
	const char * ChunkStart = a_FileData + a_SectorStart * 4096;
	int ByteSize = ntohl(*(int *)ChunkStart);
	char CompressionMethod = ChunkStart[4];
	
	if (m_Callback.OnCompressedDataSizePos(ByteSize, a_SectorStart * 4096 + 5, CompressionMethod))
	{
		return;
	}

	ProcessCompressedChunkData(a_ChunkX, a_ChunkZ, ChunkStart + 5, ByteSize);
}





void cProcessor::cThread::ProcessCompressedChunkData(int a_ChunkX, int a_ChunkZ, const char * a_CompressedData, int a_CompressedSize)
{
	char Decompressed[CHUNK_INFLATE_MAX];
	z_stream strm;
	strm.zalloc = (alloc_func)NULL;
	strm.zfree = (free_func)NULL;
	strm.opaque = NULL;
	inflateInit(&strm);
	strm.next_out  = (Bytef *)Decompressed;
	strm.avail_out = sizeof(Decompressed);
	strm.next_in   = (Bytef *)a_CompressedData;
	strm.avail_in  = a_CompressedSize;
	int res = inflate(&strm, Z_FINISH);
	inflateEnd(&strm);
	if (res != Z_STREAM_END)
	{
		LOG("Decompression failed, skipping chunk [%d, %d]", a_ChunkX, a_ChunkZ);
		return;
	}
	
	if (m_Callback.OnDecompressedData(Decompressed, strm.total_out))
	{
		return;
	}

	// Parse the NBT data:
	cParsedNBT NBT(Decompressed, strm.total_out);
	if (!NBT.IsValid())
	{
		LOG("NBT Parsing failed, skipping chunk [%d, %d]", a_ChunkX, a_ChunkZ);
		return;
	}
	
	ProcessParsedChunkData(a_ChunkX, a_ChunkZ, NBT);
}





void cProcessor::cThread::ProcessParsedChunkData(int a_ChunkX, int a_ChunkZ, cParsedNBT & a_NBT)
{
	int LevelTag = a_NBT.FindChildByName(0, "Level");
	if (LevelTag < 0)
	{
		LOG("Bad logical structure of the NBT, skipping chunk [%d, %d].", a_ChunkX, a_ChunkZ);
		return;
	}
	int XPosTag = a_NBT.FindChildByName(LevelTag, "xPos");
	int ZPosTag = a_NBT.FindChildByName(LevelTag, "zPos");
	if ((XPosTag < 0) || (ZPosTag < 0))
	{
		LOG("Pos tags missing in NTB, skipping chunk [%d, %d].", a_ChunkX, a_ChunkZ);
		return;
	}
	if (m_Callback.OnRealCoords(a_NBT.GetInt(XPosTag), a_NBT.GetInt(ZPosTag)))
	{
		return;
	}
	
	int LastUpdateTag = a_NBT.FindChildByName(LevelTag, "LastUpdate");
	if (LastUpdateTag > 0)
	{
		if (m_Callback.OnLastUpdate(a_NBT.GetLong(LastUpdateTag)))
		{
			return;
		}
	}
	
	int TerrainPopulatedTag = a_NBT.FindChildByName(LevelTag, "TerrainPopulated");
	bool TerrainPopulated = (TerrainPopulatedTag < 0) ? false : (a_NBT.GetByte(TerrainPopulatedTag) != 0);
	if (m_Callback.OnTerrainPopulated(TerrainPopulated))
	{
		return;
	}
	
	int BiomesTag = a_NBT.FindChildByName(LevelTag, "Biomes");
	if (BiomesTag > 0)
	{
		if (m_Callback.OnBiomes((const unsigned char *)(a_NBT.GetData(BiomesTag))))
		{
			return;
		}
	}
	
	int HeightMapTag = a_NBT.FindChildByName(LevelTag, "HeightMap");
	if (HeightMapTag > 0)
	{
		if (m_Callback.OnHeightMap((const int *)(a_NBT.GetData(HeightMapTag))))
		{
			return;
		}
	}
	
	if (ProcessChunkSections(a_ChunkX, a_ChunkZ, a_NBT, LevelTag))
	{
		return;
	}
	// TODO: entities, tile-entities etc.
}





bool cProcessor::cThread::ProcessChunkSections(int a_ChunkX, int a_ChunkZ, cParsedNBT & a_NBT, int a_LevelTag)
{
	int Sections = a_NBT.FindChildByName(a_LevelTag, "Sections");
	if (Sections < 0)
	{
		return false;
	}
	
	for (int Tag = a_NBT.GetFirstChild(Sections); Tag > 0; Tag = a_NBT.GetNextSibling(Tag))
	{
		int YTag          = a_NBT.FindChildByName(Tag, "Y");
		int BlocksTag     = a_NBT.FindChildByName(Tag, "Blocks");
		int AddTag        = a_NBT.FindChildByName(Tag, "Add");
		int DataTag       = a_NBT.FindChildByName(Tag, "Data");
		int BlockLightTag = a_NBT.FindChildByName(Tag, "BlockLightTag");
		int SkyLightTag   = a_NBT.FindChildByName(Tag, "SkyLight");
		
		if ((YTag < 0) || (BlocksTag < 0) || (DataTag < 0))
		{
			continue;
		}
		
		if (m_Callback.OnSection(
			a_NBT.GetByte(YTag),
			(const BLOCKTYPE *) (a_NBT.GetData(BlocksTag)),
			(AddTag > 0) ? (const NIBBLETYPE *)(a_NBT.GetData(AddTag)) : NULL,
			(const NIBBLETYPE *)(a_NBT.GetData(DataTag)),
			(BlockLightTag > 0) ? (const NIBBLETYPE *)(a_NBT.GetData(BlockLightTag)) : NULL,
			(BlockLightTag > 0) ? (const NIBBLETYPE *)(a_NBT.GetData(BlockLightTag)) : NULL
		))
		{
			return true;
		}
	}  // for Tag - Sections[]
	
	return false;
}





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cProcessor:

cProcessor::cProcessor(void) :
	m_IsShuttingDown(false)
{
}





cProcessor::~cProcessor()
{
}





void cProcessor::ProcessWorld(const AString & a_WorldFolder, cCallbackFactory & a_CallbackFactory)
{
	PopulateFileQueue(a_WorldFolder);
	
	// Start as many threads as there are cores:
	// Get number of cores by querying the system process affinity mask
	DWORD Affinity, ProcAffinity;
	GetProcessAffinityMask(GetCurrentProcess(), &ProcAffinity, &Affinity);
	while (Affinity > 0)
	{
		if ((Affinity & 1) == 1)
		{
			cCallback * Callback = a_CallbackFactory.GetNewCallback();
			m_Threads.push_back(new cThread(*Callback, *this));
		}
		Affinity >>= 1;
	}  // while (Affinity > 0)
	if (m_Threads.size() == 0)
	{
		LOG("Zero cores detected - how am I running? Running in a single thread.");
		cCallback * Callback = a_CallbackFactory.GetNewCallback();
		m_Threads.push_back(new cThread(*Callback, *this));
	}

	// Wait for all threads to finish
	// simply by calling each thread's destructor sequentially
	for (cThreads::iterator itr = m_Threads.begin(), end = m_Threads.end(); itr != end; ++itr)
	{
		delete *itr;
	}  // for itr - m_Threads[]
}





void cProcessor::PopulateFileQueue(const AString & a_WorldFolder)
{
	LOG("Processing world in \"%s\"...", a_WorldFolder.c_str());

	AString Path = a_WorldFolder;
	Path.push_back(cFile::PathSeparator);
	AStringList AllFiles = GetDirectoryContents(Path.c_str());
	for (AStringList::iterator itr = AllFiles.begin(), end = AllFiles.end(); itr != end; ++itr)
	{
		if (itr->rfind(".mca") != itr->length() - 4)
		{
			// Not a .mca file
			continue;
		}
		m_FileQueue.push_back(Path + *itr);
	}  // for itr - AllFiles[]
}





AString cProcessor::GetOneFileName(void)
{
	cCSLock Lock(m_CS);
	if (m_FileQueue.empty())
	{
		return "";
	}
	AString res = m_FileQueue.back();
	m_FileQueue.pop_back();
	return res;
}




