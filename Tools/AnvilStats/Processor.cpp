
// Processor.cpp

// Implements the cProcessor class representing the overall processor engine that manages threads, calls callbacks etc.

#include "Globals.h"
#include "Processor.h"
#include "Callback.h"
#include "../../src/WorldStorage/FastNBT.h"
#include "zlib/zlib.h"
#include "Utils.h"





const int CHUNK_INFLATE_MAX = 1 MiB;





////////////////////////////////////////////////////////////////////////////////
// cProcessor::cThread:

cProcessor::cThread::cThread(cCallback & a_Callback, cProcessor & a_ParentProcessor) :
	super("cProcessor::cThread"),
	m_Callback(a_Callback),
	m_ParentProcessor(a_ParentProcessor)
{
	LOG("Created a new thread: %p", this);
	super::Start();
}





void cProcessor::cThread::WaitForStart(void)
{
	m_HasStarted.Wait();
}





void cProcessor::cThread::Execute(void)
{
	LOG("Started a new thread: %p, ID %d", this, cIsThread::GetCurrentID());

	m_HasStarted.Set();

	for (;;)
	{
		AString FileName = m_ParentProcessor.GetOneFileName();
		if (FileName.empty())
		{
			// All done, terminate the thread
			break;
		}
		ProcessFile(FileName);
	}  // for-ever

	LOG("Thread %p (ID %d) terminated", this, cIsThread::GetCurrentID());
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

	if (m_Callback.OnNewRegion(RegionX, RegionZ))
	{
		// Callback doesn't want the region file processed
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

	m_Callback.OnRegionFinished(RegionX, RegionZ);
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
			((Location == 0) && (Timestamp == 0)) ||  // Official docs' "not present"
			(Location >> 8 < 2)                   ||  // Logical - no chunk can start inside the header
			((Location & 0xff) == 0)              ||  // Logical - no chunk can be zero bytes
			((Location >> 8) * 4096 > a_Size)         // Logical - no chunk can start at beyond the file end
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

	if (ProcessChunkEntities(a_ChunkX, a_ChunkZ, a_NBT, LevelTag))
	{
		return;
	}

	if (ProcessChunkTileEntities(a_ChunkX, a_ChunkZ, a_NBT, LevelTag))
	{
		return;
	}

	if (ProcessChunkTileTicks(a_ChunkX, a_ChunkZ, a_NBT, LevelTag))
	{
		return;
	}
}





bool cProcessor::cThread::ProcessChunkSections(int a_ChunkX, int a_ChunkZ, cParsedNBT & a_NBT, int a_LevelTag)
{
	int Sections = a_NBT.FindChildByName(a_LevelTag, "Sections");
	if (Sections < 0)
	{
		return false;
	}

	bool SectionProcessed[16];
	memset(SectionProcessed, 0, sizeof(SectionProcessed));
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

		unsigned char SectionY = a_NBT.GetByte(YTag);
		if (SectionY >= 16)
		{
			LOG("WARNING: Section Y >= 16 (%d), high world, wtf? Skipping section!", SectionY);
			continue;
		}
		if (m_Callback.OnSection(
			SectionY,
			(const BLOCKTYPE *) (a_NBT.GetData(BlocksTag)),
			(AddTag > 0) ? (const NIBBLETYPE *)(a_NBT.GetData(AddTag)) : NULL,
			(const NIBBLETYPE *)(a_NBT.GetData(DataTag)),
			(BlockLightTag > 0) ? (const NIBBLETYPE *)(a_NBT.GetData(BlockLightTag)) : NULL,
			(BlockLightTag > 0) ? (const NIBBLETYPE *)(a_NBT.GetData(BlockLightTag)) : NULL
		))
		{
			return true;
		}
		SectionProcessed[SectionY] = true;
	}  // for Tag - Sections[]

	// Call the callback for empty sections:
	for (unsigned char y = 0; y < 16; y++)
	{
		if (!SectionProcessed[y])
		{
			if (m_Callback.OnEmptySection(y))
			{
				return true;
			}
		}
	}

	if (m_Callback.OnSectionsFinished())
	{
		return true;
	}

	return false;
}





bool cProcessor::cThread::ProcessChunkEntities(int a_ChunkX, int a_ChunkZ, cParsedNBT & a_NBT, int a_LevelTag)
{
	int EntitiesTag = a_NBT.FindChildByName(a_LevelTag, "Entities");
	if (EntitiesTag < 0)
	{
		return false;
	}

	for (int EntityTag = a_NBT.GetFirstChild(EntitiesTag); EntityTag > 0; EntityTag = a_NBT.GetNextSibling(EntityTag))
	{
		int PosTag = a_NBT.FindChildByName(EntityTag, "Pos");
		if (PosTag < 0)
		{
			continue;
		}
		int SpeedTag = a_NBT.FindChildByName(EntityTag, "Motion");
		if (SpeedTag < 0)
		{
			continue;
		}
		int RotTag = a_NBT.FindChildByName(EntityTag, "Rotation");
		if (RotTag < 0)
		{
			continue;
		}
		double Pos[3];
		for (int i = 0, tag = a_NBT.GetFirstChild(PosTag); (i < 3) && (tag > 0); i++)
		{
			Pos[i] = a_NBT.GetDouble(tag);
		}
		double Speed[3];
		for (int i = 0, tag = a_NBT.GetFirstChild(SpeedTag); (i < 3) && (tag > 0); i++)
		{
			Speed[i] = a_NBT.GetDouble(tag);
		}
		float Rot[2];
		for (int i = 0, tag = a_NBT.GetFirstChild(RotTag); (i < 2) && (tag > 0); i++)
		{
			Rot[i] = a_NBT.GetFloat(tag);
		}

		if (m_Callback.OnEntity(
			a_NBT.GetString(a_NBT.FindChildByName(EntityTag, "id")),
			Pos[0], Pos[1], Pos[2],
			Speed[0], Speed[1], Speed[2],
			Rot[0], Rot[1],
			a_NBT.GetFloat(a_NBT.FindChildByName(EntityTag, "FallDistance")),
			a_NBT.GetShort(a_NBT.FindChildByName(EntityTag, "Fire")),
			a_NBT.GetShort(a_NBT.FindChildByName(EntityTag, "Air")),
			a_NBT.GetByte(a_NBT.FindChildByName(EntityTag, "OnGround")),
			a_NBT, EntityTag
		))
		{
			return true;
		}
	}  // for EntityTag - Entities[]
	return false;
}





bool cProcessor::cThread::ProcessChunkTileEntities(int a_ChunkX, int a_ChunkZ, cParsedNBT & a_NBT, int a_LevelTag)
{
	int TileEntitiesTag = a_NBT.FindChildByName(a_LevelTag, "TileEntities");
	if (TileEntitiesTag < 0)
	{
		return false;
	}

	for (int TileEntityTag = a_NBT.GetFirstChild(TileEntitiesTag); TileEntityTag > 0; TileEntityTag = a_NBT.GetNextSibling(TileEntityTag))
	{
		if (m_Callback.OnTileEntity(
			a_NBT.GetString(a_NBT.FindChildByName(TileEntityTag, "id")),
			a_NBT.GetInt(a_NBT.FindChildByName(TileEntityTag, "x")),
			a_NBT.GetInt(a_NBT.FindChildByName(TileEntityTag, "y")),
			a_NBT.GetInt(a_NBT.FindChildByName(TileEntityTag, "z")),
			a_NBT, TileEntityTag
		))
		{
			return true;
		}
	}  // for EntityTag - Entities[]
	return false;
}





bool cProcessor::cThread::ProcessChunkTileTicks(int a_ChunkX, int a_ChunkZ, cParsedNBT & a_NBT, int a_LevelTag)
{
	int TileTicksTag = a_NBT.FindChildByName(a_LevelTag, "TileTicks");
	if (TileTicksTag < 0)
	{
		return false;
	}

	for (int TileTickTag = a_NBT.GetFirstChild(TileTicksTag); TileTickTag > 0; TileTickTag = a_NBT.GetNextSibling(TileTickTag))
	{
		int iTag = a_NBT.FindChildByName(TileTicksTag, "i");
		int tTag = a_NBT.FindChildByName(TileTicksTag, "t");
		int xTag = a_NBT.FindChildByName(TileTicksTag, "x");
		int yTag = a_NBT.FindChildByName(TileTicksTag, "y");
		int zTag = a_NBT.FindChildByName(TileTicksTag, "z");
		if ((iTag < 0) || (tTag < 0) || (xTag < 0) || (yTag < 0) || (zTag < 0))
		{
			continue;
		}
		if (m_Callback.OnTileTick(
			a_NBT.GetInt(iTag),
			a_NBT.GetInt(iTag),
			a_NBT.GetInt(iTag),
			a_NBT.GetInt(iTag),
			a_NBT.GetInt(iTag)
		))
		{
			return true;
		}
	}  // for EntityTag - Entities[]
	return false;
}





////////////////////////////////////////////////////////////////////////////////
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

	if (m_FileQueue.empty())
	{
		LOG("No files to process, exitting.");
		return;
	}

	// Start as many threads as there are cores, plus one:
	// (One more thread can be in the file-read IO block while all other threads crunch the numbers)
	int NumThreads = GetNumCores() + 1;

	/*
	// Limit the number of threads in DEBUG mode to 1 for easier debugging
	#ifdef _DEBUG
	NumThreads = 1;
	#endif  // _DEBUG
	//*/

	// Start all the threads:
	for (int i = 0; i < NumThreads; i++)
	{
		cCallback * Callback = a_CallbackFactory.GetNewCallback();
		m_Threads.push_back(new cThread(*Callback, *this));
	}

	// Wait for all threads to finish:
	LOG("Waiting for threads to finish");
	for (cThreads::iterator itr = m_Threads.begin(), end = m_Threads.end(); itr != end; ++itr)
	{
		(*itr)->WaitForStart();
		delete *itr;
	}  // for itr - m_Threads[]
	LOG("Processor finished");
}





void cProcessor::PopulateFileQueue(const AString & a_WorldFolder)
{
	LOG("Processing world in \"%s\"...", a_WorldFolder.c_str());

	AString Path = a_WorldFolder;
	if (!Path.empty() && (Path[Path.length() - 1] != cFile::PathSeparator))
	{
		Path.push_back(cFile::PathSeparator);
	}
	AStringVector AllFiles = cFile::GetFolderContents(Path.c_str());
	for (AStringVector::iterator itr = AllFiles.begin(), end = AllFiles.end(); itr != end; ++itr)
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
