
// Zapper.cpp

// Implements the cZapper class representing the processor that actually zaps blocks and entities

#include "Globals.h"

#include "WorldStorage/FastNBT.h"
#include "StringCompression.h"
#include "zlib/zlib.h"

#include "Zapper.h"





/** The maximum size of an inflated chunk; raw chunk data is 192 KiB, allow 64 KiB more of entities */
#define CHUNK_INFLATE_MAX 256 KiB





cZapper::cZapper(const AString & a_MCAFolder) :
	m_MCAFolder(a_MCAFolder)
{
}





void cZapper::ZapRegions(const cRegionVector & a_Regions)
{
	for (cRegionVector::const_iterator itr = a_Regions.begin(), end = a_Regions.end(); itr != end; ++itr)
	{
		int MinAnvX, MinAnvZ;
		int MaxAnvX, MaxAnvZ;
		BlockToMCA(itr->m_MinX, itr->m_MinZ, MinAnvX, MinAnvZ);
		BlockToMCA(itr->m_MaxX, itr->m_MaxZ, MaxAnvX, MaxAnvZ);
		for (int x = MinAnvX; x <= MaxAnvX; x++)
		{
			for (int z = MinAnvZ; z <= MaxAnvZ; z++)
			{
				ZapRegionInMCAFile(*itr, x, z);
			}
		}
	}  // for itr - a_Regions
}





void cZapper::BlockToMCA(int a_BlockX, int a_BlockZ, int & a_MCAX, int & a_MCAZ)
{
	// These need to be arithmetic shifts, consult your compiler documentation to see if it's so
	// MSVC and GCC both use arithmetic shifts
	a_MCAX = a_BlockX >> 10;
	a_MCAZ = a_BlockZ >> 10;
}





void cZapper::BlockToChunk(int a_BlockX, int a_BlockZ, int & a_ChunkX, int & a_ChunkZ)
{
	// These need to be arithmetic shifts, consult your compiler documentation to see if it's so
	// MSVC and GCC both use arithmetic shifts
	a_ChunkX = a_BlockX >> 4;
	a_ChunkZ = a_BlockZ >> 4;
}





void cZapper::ZapRegionInMCAFile(const cRegion & a_Region, int a_MCAX, int a_MCAZ)
{
	cFile fIn;
	AString FileNameIn = Printf("%s/r.%d.%d.mca", m_MCAFolder.c_str(), a_MCAX, a_MCAZ);
	if (!fIn.Open(FileNameIn, cFile::fmRead))
	{
		return;
	}
	cFile fOut;
	AString FileNameOut = Printf("%s/r.%d.%d.zap", m_MCAFolder.c_str(), a_MCAX, a_MCAZ);
	if (!fOut.Open(FileNameOut, cFile::fmWrite))
	{
		fprintf(stderr, "Cannot open temporary file \"%s\" for writing, skipping file \"%s\".", FileNameOut.c_str(), FileNameIn.c_str());
		return;
	}

	AString DataOut;
	DataOut.reserve(fIn.GetSize());

	int HeaderIn[2048];
	if (fIn.Read(HeaderIn, sizeof(HeaderIn)) != sizeof(HeaderIn))
	{
		fprintf(stderr, "Cannot read header from file \"%s\", skipping file.", FileNameIn.c_str());
	}
	int HeaderOut[2048];
	for (int i = 0; i < 1024; i++)
	{
		if (HeaderIn[i] == 0)
		{
			// Chunk not present
			HeaderOut[i] = 0;
			continue;
		}
		AString ChunkData;
		int ChunkX = a_MCAX * ChunksPerMCAX + (i % ChunksPerMCAX);
		int ChunkZ = a_MCAZ * ChunksPerMCAZ + (i / ChunksPerMCAX);

		LoadChunkData(fIn, HeaderIn[i], ChunkData, ChunkX, ChunkZ);

		if (a_Region.TouchesChunk(ChunkX, ChunkZ))
		{
			ZapRegionInRawChunkData(a_Region, ChunkData, ChunkX, ChunkZ);
		}
		unsigned char ChunkHeader[5];
		size_t DataSize = ChunkData.size() + 1;
		ChunkHeader[0] = (DataSize >> 24) & 0xff;
		ChunkHeader[1] = (DataSize >> 16) & 0xff;
		ChunkHeader[2] = (DataSize >> 8)  & 0xff;
		ChunkHeader[3] = DataSize & 0xff;
		ChunkHeader[4] = 2;  // zlib compression
		size_t Alignment = 4096 - (ChunkData.size() + 5) % 4096;  // 5 bytes of the header are appended outside of ChunkData
		if (Alignment > 0)
		{
			ChunkData.append(Alignment, (char)0);
		}
		HeaderOut[i] = htonl(((DataOut.size() / 4096 + 2) << 8) | ((ChunkData.size() + 5) / 4096));
		DataOut.append((const char *)ChunkHeader, sizeof(ChunkHeader));
		DataOut.append(ChunkData);
	}  // for i - chunks in fIn
	for (int i = 1024; i < 2048; i++)
	{
		HeaderOut[i] = HeaderIn[i];
	}
	fIn.Close();
	fOut.Write(HeaderOut, sizeof(HeaderOut));
	fOut.Write(DataOut.data(), DataOut.size());
	fOut.Close();
	cFile::Delete(FileNameIn);
	cFile::Rename(FileNameOut, FileNameIn);
}





void cZapper::LoadChunkData(cFile & a_InFile, int a_ChunkHeaderValue, AString & a_ChunkData, int a_ChunkX, int a_ChunkZ)
{
	a_ChunkHeaderValue = ntohl(a_ChunkHeaderValue);  // Convert from big-endian to system-endian
	int ChunkOffset = (a_ChunkHeaderValue >> 8) * 4096;
	int ChunkSize = (a_ChunkHeaderValue & 0xff) * 4096;
	a_InFile.Seek(ChunkOffset);
	unsigned char ChunkHeader[5];
	a_InFile.Read(ChunkHeader, sizeof(ChunkHeader));
	if (ChunkHeader[4] != 2)
	{
		fprintf(stderr, "Chunk [%d, %d] is compressed in an unknown scheme (%d), skipping", a_ChunkX, a_ChunkZ, ChunkHeader[4]);
		return;
	}
	int ActualSize = (ChunkHeader[0] << 24) |  (ChunkHeader[1] << 16) | (ChunkHeader[2] << 8) | ChunkHeader[3];
	ActualSize -= 1;  // Compression took 1 byte
	a_ChunkData.resize(ActualSize);
	int BytesRead = a_InFile.Read((void *)(a_ChunkData.data()), ActualSize);
	if (BytesRead != ActualSize)
	{
		fprintf(stderr, "Chunk is truncated in file (%d bytes out of %d), skipping.", BytesRead, ActualSize);
		a_ChunkData.clear();
		return;
	}
}





void cZapper::ZapRegionInRawChunkData(const cRegion & a_Region, AString & a_ChunkData, int a_ChunkX, int a_ChunkZ)
{
	// Decompress the data:
	char Uncompressed[CHUNK_INFLATE_MAX];
	z_stream strm;
	strm.zalloc = (alloc_func)NULL;
	strm.zfree = (free_func)NULL;
	strm.opaque = NULL;
	inflateInit(&strm);
	strm.next_out  = (Bytef *)Uncompressed;
	strm.avail_out = sizeof(Uncompressed);
	strm.next_in   = (Bytef *)a_ChunkData.data();
	strm.avail_in  = a_ChunkData.size();
	int res = inflate(&strm, Z_FINISH);
	inflateEnd(&strm);
	if (res != Z_STREAM_END)
	{
		fprintf(stderr, "Chunk [%d, %d] failed to decompress: error %d. Skipping chunk.", a_ChunkX, a_ChunkZ, res);
		return;
	}

	/*
	// DEBUG: Output src to a file:
	cFile f1;
	if (f1.Open(Printf("chunk_%d_%d_in.nbt", a_ChunkX, a_ChunkZ), cFile::fmWrite))
	{
		f1.Write(Uncompressed, strm.total_out);
	}
	//*/

	cParsedNBT NBT(Uncompressed, strm.total_out);
	if (!NBT.IsValid())
	{
		fprintf(stderr, "Chunk [%d, %d] failed to parse. Skipping chunk.", a_ChunkX, a_ChunkZ);
		return;
	}
	ZapRegionInNBTChunk(a_Region, NBT, a_ChunkX, a_ChunkZ);

	cFastNBTWriter Writer;
	for (int ch = NBT.GetFirstChild(0); ch >= 0; ch = NBT.GetNextSibling(ch))
	{
		SerializeNBTTag(NBT, ch, Writer);
	}
	Writer.Finish();

	/*
	// DEBUG: Output dst to a file:
	cFile f2;
	if (f2.Open(Printf("chunk_%d_%d_out.nbt", a_ChunkX, a_ChunkZ), cFile::fmWrite))
	{
		f2.Write(Writer.GetResult().data(), Writer.GetResult().size());
	}
	//*/

	// Compress the serialized data into "Uncompressed" (reuse buffer)
	CompressString(Writer.GetResult().data(), Writer.GetResult().size(), a_ChunkData);
}





void cZapper::ZapRegionInNBTChunk(const cRegion & a_Region, cParsedNBT & a_NBT, int a_ChunkX, int a_ChunkZ)
{
	int LevelTag = a_NBT.FindChildByName(a_NBT.GetRoot(), "Level");
	if (LevelTag < 0)
	{
		fprintf(stderr, "Cannot find Level tag in chunk [%d, %d]'s NBT. Skipping chunk.", a_ChunkX, a_ChunkZ);
		return;
	}

	// Create a copy of the region and limit it to the current chunk:
	int BlockX = a_ChunkX * 16;
	int BlockZ = a_ChunkZ * 16;
	cRegion Local;
	Local.m_MinX = std::max(0, a_Region.m_MinX - BlockX);
	Local.m_MaxX = std::min(15, a_Region.m_MaxX - BlockX);
	Local.m_MinY = a_Region.m_MinY;
	Local.m_MaxY = a_Region.m_MaxY;
	Local.m_MinZ = std::max(0, a_Region.m_MinZ - BlockZ);
	Local.m_MaxZ = std::min(15, a_Region.m_MaxZ - BlockZ);

	if (a_Region.m_ShouldZapBlocks)
	{
		int SectionsTag = a_NBT.FindChildByName(LevelTag, "Sections");
		if (SectionsTag < 0)
		{
			fprintf(stderr, "Cannot find the Sections tag in the Level tag in chunk [%d, %d]'s NBT. Skipping block-zapping in chunk.", a_ChunkX, a_ChunkZ);
			return;
		}
		ZapRegionBlocksInNBT(Local, a_NBT, SectionsTag);
	}

	if (a_Region.m_ShouldZapEntities)
	{
		int EntitiesTag = a_NBT.FindChildByName(LevelTag, "Entities");
		if (EntitiesTag < 0)
		{
			fprintf(stderr, "Cannot find the Entities tag in the Level tag in chunk [%d, %d]'s NBT. Skipping entity-zapping in chunk.", a_ChunkX, a_ChunkZ);
			return;
		}
		ZapRegionEntitiesInNBT(Local, a_NBT, EntitiesTag);
	}
}





void cZapper::ZapRegionBlocksInNBT(const cRegion & a_Region, cParsedNBT & a_NBT, int a_SectionsTag)
{
	for (int Child = a_NBT.GetFirstChild(a_SectionsTag); Child >= 0; Child = a_NBT.GetNextSibling(Child))
	{
		int y = 0;
		int SectionY = a_NBT.FindChildByName(Child, "Y");
		if ((SectionY < 0) || (a_NBT.GetType(SectionY) != TAG_Byte))
		{
			continue;
		}
		y = a_NBT.GetByte(SectionY);
		if ((y * 16 > a_Region.m_MaxY) || (y * 16 + 16 < a_Region.m_MinY))
		{
			continue;
		}
		int BlockDataTag = a_NBT.FindChildByName(Child, "Blocks");
		int BlockMetaTag = a_NBT.FindChildByName(Child, "Data");
		int BlockAddTag  = a_NBT.FindChildByName(Child, "Add");
		if (BlockDataTag > 0)
		{
			ZapRegionInNBTSectionBytes(a_Region, y, (unsigned char *)(a_NBT.GetData(BlockDataTag)));
		}
		if (BlockMetaTag > 0)
		{
			ZapRegionInNBTSectionNibbles(a_Region, y, (unsigned char *)(a_NBT.GetData(BlockMetaTag)));
		}
		if (BlockAddTag > 0)
		{
			ZapRegionInNBTSectionNibbles(a_Region, y, (unsigned char *)(a_NBT.GetData(BlockAddTag)));
		}
	}  // for Child - Level / Sections / []
}





void cZapper::ZapRegionInNBTSectionBytes(const cRegion & a_Region, int a_SectionY, unsigned char * a_BlockBytes)
{
	int MinY = std::max(0, a_Region.m_MinY - a_SectionY * 16);
	int MaxY = std::min(15, a_Region.m_MaxY - a_SectionY * 16);
	ASSERT(MinY >= 0);
	ASSERT(MaxY >= 0);
	for (int y = MinY; y <= MaxY; y++)
	{
		for (int z = a_Region.m_MinZ; z <= a_Region.m_MaxZ; z++)
		{
			for (int x = a_Region.m_MinX; x <= a_Region.m_MaxX; x++)
			{
				a_BlockBytes[x + z * 16 + y * 16 * 16] = 0;
			}
		}
	}
}





void cZapper::ZapRegionInNBTSectionNibbles(const cRegion & a_Region, int a_SectionY, unsigned char * a_BlockNibbles)
{
	int MinY = std::max(0, a_Region.m_MinY - a_SectionY * 16);
	int MaxY = std::min(15, a_Region.m_MaxY - a_SectionY * 16);
	ASSERT(MinY >= 0);
	ASSERT(MaxY >= 0);
	for (int y = MinY; y <= MaxY; y++)
	{
		for (int z = a_Region.m_MinZ; z < a_Region.m_MaxZ; z++)
		{
			for (int x = a_Region.m_MinX; x < a_Region.m_MaxX; x++)
			{
				cChunkDef::SetNibble(a_BlockNibbles, x, y, z, 0);
			}
		}
	}
}





void cZapper::ZapRegionEntitiesInNBT(const cRegion & a_Region, cParsedNBT & a_NBT, int a_EntitiesTag)
{
	// TODO
}





void cZapper::SerializeNBTTag(const cParsedNBT & a_NBT, int a_Tag, cFastNBTWriter & a_Writer)
{
	switch (a_NBT.GetType(a_Tag))
	{
		case TAG_Byte:      a_Writer.AddByte     (a_NBT.GetName(a_Tag), a_NBT.GetByte  (a_Tag)); break;
		case TAG_Short:     a_Writer.AddShort    (a_NBT.GetName(a_Tag), a_NBT.GetShort (a_Tag)); break;
		case TAG_Int:       a_Writer.AddInt      (a_NBT.GetName(a_Tag), a_NBT.GetInt   (a_Tag)); break;
		case TAG_Long:      a_Writer.AddLong     (a_NBT.GetName(a_Tag), a_NBT.GetLong  (a_Tag)); break;
		case TAG_Float:     a_Writer.AddFloat    (a_NBT.GetName(a_Tag), a_NBT.GetFloat (a_Tag)); break;
		case TAG_Double:    a_Writer.AddDouble   (a_NBT.GetName(a_Tag), a_NBT.GetDouble(a_Tag)); break;
		case TAG_ByteArray: a_Writer.AddByteArray(a_NBT.GetName(a_Tag), a_NBT.GetData  (a_Tag), a_NBT.GetDataLength(a_Tag)); break;
		case TAG_String:    a_Writer.AddString   (a_NBT.GetName(a_Tag), a_NBT.GetString(a_Tag)); break;
		case TAG_IntArray:
		{
			std::vector<int> Data;
			int NumInts = a_NBT.GetDataLength(a_Tag) / 4;
			Data.reserve(NumInts);
			int * OrigData = (int *)(a_NBT.GetData(a_Tag));
			for (int i = 0; i < NumInts; i++)
			{
				Data.push_back(ntohl(OrigData[i]));
			}
			a_Writer.AddIntArray (a_NBT.GetName(a_Tag), &Data.front(), Data.size()); break;
		}

		case TAG_List:
		{
			a_Writer.BeginList(a_NBT.GetName(a_Tag), a_NBT.GetChildrenType(a_Tag));
			for (int ch = a_NBT.GetFirstChild(a_Tag); ch >= 0; ch = a_NBT.GetNextSibling(ch))
			{
				SerializeNBTTag(a_NBT, ch, a_Writer);
			}  // for ch - children[]
			a_Writer.EndList();
			break;
		}

		case TAG_Compound:
		{
			a_Writer.BeginCompound(a_NBT.GetName(a_Tag));
			for (int ch = a_NBT.GetFirstChild(a_Tag); ch >= 0; ch = a_NBT.GetNextSibling(ch))
			{
				SerializeNBTTag(a_NBT, ch, a_Writer);
			}  // for ch - children[]
			a_Writer.EndCompound();
			break;
		}

		default:
		{
			ASSERT(!"Unknown NBT tag");
			break;
		}
	}
}
