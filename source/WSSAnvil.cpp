
// WSSAnvil.cpp

// Implements the cWSSAnvil class representing the Anvil world storage scheme

#include "Globals.h"
#include "WSSAnvil.h"
#include "cWorld.h"
#include "zlib.h"
#include "NBT.h"
#include "BlockID.h"
#include "cChestEntity.h"
#include "cItem.h"





/** Maximum number of MCA files that are cached in memory. 
Since only the header is actually in the memory, this number can be high, but still, each file means an OS FS handle.
*/
#define MAX_MCA_FILES 32

/// The maximum size of an inflated chunk
#define CHUNK_INFLATE_MAX 128 KiB






///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cWSSAnvil:

cWSSAnvil::~cWSSAnvil()
{
	cCSLock Lock(m_CS);
	for (cMCAFiles::iterator itr = m_Files.begin(); itr != m_Files.end(); ++itr)
	{
		delete *itr;
	}  // for itr - m_Files[]
}





bool cWSSAnvil::LoadChunk(const cChunkCoords & a_Chunk)
{
	AString ChunkData;
	if (!GetChunkData(a_Chunk, ChunkData))
	{
		// The reason for failure is already printed in GetChunkData()
		return false;
	}
	
	return LoadChunkFromData(a_Chunk, ChunkData);
}





bool cWSSAnvil::SaveChunk(const cChunkCoords & a_Chunk)
{
	// TODO: We're read-only for now
	return false;
}





bool cWSSAnvil::GetChunkData(const cChunkCoords & a_Chunk, AString & a_Data)
{
	cCSLock Lock(m_CS);
	cMCAFile * File = LoadMCAFile(a_Chunk);
	if (File == NULL)
	{
		return false;
	}
	return File->GetChunkData(a_Chunk, a_Data);
}





cWSSAnvil::cMCAFile * cWSSAnvil::LoadMCAFile(const cChunkCoords & a_Chunk)
{
	// ASSUME m_CS is locked

	const int RegionX = (int)(floorf((float)a_Chunk.m_ChunkX / 32.0f));
	const int RegionZ = (int)(floorf((float)a_Chunk.m_ChunkZ / 32.0f));
	
	// Is it already cached?
	for (cMCAFiles::iterator itr = m_Files.begin(); itr != m_Files.end(); ++itr)
	{
		if (((*itr) != NULL) && ((*itr)->GetRegionX() == RegionX) && ((*itr)->GetRegionZ() == RegionZ))
		{
			// Move the file to front and return it:
			cMCAFile * f = *itr;
			if (itr != m_Files.begin())
			{
				m_Files.erase(itr);
				m_Files.push_front(f);
			}
			return f;
		}
	}
	
	// Load it anew:
	AString FileName;
	Printf(FileName, "%s/r.%d.%d.mca", m_World->GetName().c_str(), RegionX, RegionZ);
	cMCAFile * f = new cMCAFile(FileName, RegionX, RegionZ);
	if (f == NULL)
	{
		return NULL;
	}
	m_Files.push_front(f);
	
	// If there are too many MCA files cached, delete the last one used:
	if (m_Files.size() > MAX_MCA_FILES)
	{
		delete m_Files.back();
		m_Files.pop_back();
	}
	return f;
}





bool cWSSAnvil::LoadChunkFromData(const cChunkCoords & a_Chunk, const AString & a_Data)
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
	strm.next_in   = (Bytef *)a_Data.data();
	strm.avail_in  = a_Data.size();
	int res = inflate(&strm, Z_FINISH);
	inflateEnd(&strm);
	if (res != Z_STREAM_END)
	{
		return false;
	}
	
	// Parse the NBT data:
	std::auto_ptr<cNBTTree> Tree(cNBTParser::Parse(Uncompressed, strm.total_out));
	if (Tree.get() == NULL)
	{
		return false;
	}

	// Load the data from NBT:
	return LoadChunkFromNBT(a_Chunk, *Tree.get());
}





bool cWSSAnvil::LoadChunkFromNBT(const cChunkCoords & a_Chunk, cNBTTag & a_NBT)
{
	// The data arrays, in MCA-native y/z/x ordering (will be reordered for the final chunk data)
	char BlockData[cChunk::c_NumBlocks];
	char MetaData[cChunk::c_NumBlocks / 2];
	char BlockLight[cChunk::c_NumBlocks / 2];
	char SkyLight[cChunk::c_NumBlocks / 2];
	
	memset(BlockData,  E_BLOCK_AIR, sizeof(BlockData));
	memset(MetaData,   0,           sizeof(MetaData));
	memset(BlockLight, 0,           sizeof(BlockLight));
	memset(SkyLight,   0xff,        sizeof(SkyLight));  // By default, data not present in the NBT means air, which means full skylight
	
	// Load the blockdata, blocklight and skylight:
	cNBTList * Sections = (cNBTList *)a_NBT.FindChildByPath("Level\\Sections");
	if ((Sections == NULL) || (Sections->GetType() != cNBTTag::TAG_List) || (Sections->GetChildrenType() != cNBTTag::TAG_Compound))
	{
		return false;
	}
	const cNBTTags & LevelSections = Sections->GetChildren();
	for (cNBTTags::const_iterator itr = LevelSections.begin(); itr != LevelSections.end(); ++itr)
	{
		int y = 0;
		cNBTByte * SectionY = (cNBTByte *)((*itr)->FindChildByName("Y"));
		if ((SectionY == NULL) || (SectionY->GetType() != cNBTTag::TAG_Byte) || (SectionY->m_Value < 0) || (SectionY->m_Value > 15))
		{
			continue;
		}
		y = SectionY->m_Value;
		cNBTByteArray * baBlocks = (cNBTByteArray *)((*itr)->FindChildByName("Blocks"));
		if ((baBlocks != NULL) && (baBlocks->GetType() == cNBTTag::TAG_ByteArray) && (baBlocks->m_Value.size() == 4096))
		{
			memcpy(&(BlockData[y * 4096]), baBlocks->m_Value.data(), 4096);
		}
		cNBTByteArray * baMetaData = (cNBTByteArray *)((*itr)->FindChildByName("Data"));
		if ((baMetaData != NULL) && (baMetaData->GetType() == cNBTTag::TAG_ByteArray) && (baMetaData->m_Value.size() == 2048))
		{
			memcpy(&(MetaData[y * 2048]), baMetaData->m_Value.data(), 2048);
		}
		cNBTByteArray * baSkyLight = (cNBTByteArray *)((*itr)->FindChildByName("SkyLight"));
		if ((baSkyLight != NULL) && (baSkyLight->GetType() == cNBTTag::TAG_ByteArray) && (baSkyLight->m_Value.size() == 2048))
		{
			memcpy(&(SkyLight[y * 2048]), baSkyLight->m_Value.data(), 2048);
		}
		cNBTByteArray * baBlockLight = (cNBTByteArray *)((*itr)->FindChildByName("BlockLight"));
		if ((baBlockLight != NULL) && (baBlockLight->GetType() == cNBTTag::TAG_ByteArray) && (baBlockLight->m_Value.size() == 2048))
		{
			memcpy(&(BlockLight[y * 2048]), baBlockLight->m_Value.data(), 2048);
		}
	}  // for itr - LevelSections[]
	
	cEntityList Entities;
	cBlockEntityList BlockEntities;
	
	// Load the entities from NBT:
	LoadEntitiesFromNBT     (Entities,      (cNBTList *)(a_NBT.FindChildByPath("Level\\Entities")));
	LoadBlockEntitiesFromNBT(BlockEntities, (cNBTList *)(a_NBT.FindChildByPath("Level\\TileEntities")));
	
	// Reorder the chunk data - walk the MCA-formatted data sequentially and copy it into the right place in the ChunkData:
	char ChunkData[cChunk::c_BlockDataSize];
	memset(ChunkData, 0, sizeof(ChunkData));
	int Index = 0;  // Index into the MCA-formatted data, incremented sequentially
	for (int y = 0; y < cChunk::c_ChunkHeight; y++) for (int z = 0; z < cChunk::c_ChunkWidth; z++) for (int x = 0; x < cChunk::c_ChunkWidth; x++)
	{
		ChunkData[cChunk::MakeIndex(x, y, z)] = BlockData[Index];
		Index++;
	}  // for y/z/x
	char * ChunkMeta = ChunkData + cChunk::c_NumBlocks;
	Index = 0;
	for (int y = 0; y < cChunk::c_ChunkHeight; y++) for (int z = 0; z < cChunk::c_ChunkWidth; z++) for (int x = 0; x < cChunk::c_ChunkWidth; x++)
	{
		cChunk::SetNibble(ChunkMeta, x, y, z, MetaData[Index / 2] >> ((Index % 2) * 4));
		Index++;
	}  // for y/z/x
	char * ChunkBlockLight = ChunkMeta + cChunk::c_NumBlocks / 2;
	Index = 0;
	for (int y = 0; y < cChunk::c_ChunkHeight; y++) for (int z = 0; z < cChunk::c_ChunkWidth; z++) for (int x = 0; x < cChunk::c_ChunkWidth; x++)
	{
		cChunk::SetNibble(ChunkBlockLight, x, y, z, BlockLight[Index / 2] >> ((Index % 2) * 4));
		Index++;
	}  // for y/z/x
	char * ChunkSkyLight = ChunkBlockLight + cChunk::c_NumBlocks / 2;
	Index = 0;
	for (int y = 0; y < cChunk::c_ChunkHeight; y++) for (int z = 0; z < cChunk::c_ChunkWidth; z++) for (int x = 0; x < cChunk::c_ChunkWidth; x++)
	{
		cChunk::SetNibble(ChunkSkyLight, x, y, z, SkyLight[Index / 2] >> ((Index % 2) * 4));
		Index++;
	}  // for y/z/x
	
	m_World->ChunkDataLoaded(a_Chunk.m_ChunkX, a_Chunk.m_ChunkY, a_Chunk.m_ChunkZ, ChunkData, Entities, BlockEntities);
	return true;
}





void cWSSAnvil::LoadEntitiesFromNBT(cEntityList & a_Entitites, const cNBTList * a_NBT)
{
	// TODO: Load the entities
}





void cWSSAnvil::LoadBlockEntitiesFromNBT(cBlockEntityList & a_BlockEntities, const cNBTList * a_NBT)
{
	if ((a_NBT == NULL) || (a_NBT->GetType() != cNBTTag::TAG_List))
	{
		return;
	}
	const cNBTTags & Tags = a_NBT->GetChildren();
	
	for (cNBTTags::const_iterator itr = Tags.begin(); itr != Tags.end(); ++itr)
	{
		if ((*itr)->GetType() != cNBTTag::TAG_Compound)
		{
			continue;
		}
		cNBTString * sID = (cNBTString *)((*itr)->FindChildByName("id"));
		if (sID == NULL)
		{
			continue;
		}
		if (sID->m_Value == "Chest")
		{
			LoadChestFromNBT(a_BlockEntities, (cNBTCompound *)(*itr));
		}
		// TODO: Other block entities
	}  // for itr - Tags[]
}





void cWSSAnvil::LoadChestFromNBT(cBlockEntityList & a_BlockEntities, const cNBTCompound * a_NBT)
{
	ASSERT(a_NBT != NULL);
	ASSERT(a_NBT->GetType() == cNBTTag::TAG_Compound);
	int x, y, z;
	if (!GetBlockEntityNBTPos(a_NBT, x, y, z))
	{
		return;
	}
	cNBTList * Items = (cNBTList *)(a_NBT->FindChildByName("Items"));
	if ((Items == NULL) || (Items->GetType() != cNBTTag::TAG_List))
	{
		return;  // Make it an empty chest
	}
	std::auto_ptr<cChestEntity> Chest(new cChestEntity(x, y, z, m_World));
	const cNBTTags & ItemDefs = Items->GetChildren();
	for (cNBTTags::const_iterator itr = ItemDefs.begin(); itr != ItemDefs.end(); ++itr)
	{
		cNBTByte * Slot = (cNBTByte *)((*itr)->FindChildByName("Slot"));
		if ((Slot == NULL) || (Slot->GetType() != cNBTTag::TAG_Byte))
		{
			continue;
		}
		cItem Item;
		cNBTShort * ID = (cNBTShort *)((*itr)->FindChildByName("id"));
		if ((ID == NULL) || (ID->GetType() != cNBTTag::TAG_Short))
		{
			continue;
		}
		Item.m_ItemID = (ENUM_ITEM_ID)(ID->m_Value);
		cNBTShort * Damage = (cNBTShort *)((*itr)->FindChildByName("Damage"));
		if ((Damage == NULL) || (Damage->GetType() != cNBTTag::TAG_Short))
		{
			continue;
		}
		Item.m_ItemHealth = Damage->m_Value;
		cNBTByte * Count = (cNBTByte *)((*itr)->FindChildByName("Count"));
		if ((Count == NULL) || (Count->GetType() != cNBTTag::TAG_Byte))
		{
			continue;
		}
		Item.m_ItemCount = Count->m_Value;
		Chest->SetSlot(Slot->m_Value, Item);
	}  // for itr - ItemDefs[]
	a_BlockEntities.push_back(Chest.release());
}





bool cWSSAnvil::GetBlockEntityNBTPos(const cNBTCompound * a_NBT, int & a_X, int & a_Y, int & a_Z)
{
	cNBTInt * x = (cNBTInt *)(a_NBT->FindChildByName("x"));
	if ((x == NULL) || (x->GetType() != cNBTTag::TAG_Int))
	{
		return false;
	}
	cNBTInt * y = (cNBTInt *)(a_NBT->FindChildByName("y"));
	if ((y == NULL) || (y->GetType() != cNBTTag::TAG_Int))
	{
		return false;
	}
	cNBTInt * z = (cNBTInt *)(a_NBT->FindChildByName("z"));
	if ((z == NULL) || (z->GetType() != cNBTTag::TAG_Int))
	{
		return false;
	}
	a_X = x->m_Value;
	a_Y = y->m_Value;
	a_Z = z->m_Value;
	return true;
}





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cWSSAnvil::cMCAFile:

cWSSAnvil::cMCAFile::cMCAFile(const AString & a_FileName, int a_RegionX, int a_RegionZ) :
	m_RegionX(a_RegionX),
	m_RegionZ(a_RegionZ),
	m_File(a_FileName, cFile::fmRead),
	m_FileName(a_FileName)
{
	if (!m_File.IsOpen())
	{
		return;
	}
	
	// Load the header:
	if (m_File.Read(m_Header, sizeof(m_Header)) != sizeof(m_Header))
	{
		LOGWARNING("Cannot read MCA header from file \"%s\", chunks in that file will be lost", m_FileName.c_str());
		m_File.Close();
		return;
	}
}





bool cWSSAnvil::cMCAFile::GetChunkData(const cChunkCoords & a_Chunk, AString & a_Data)
{
	if (!m_File.IsOpen())
	{
		return false;
	}
	int LocalX = a_Chunk.m_ChunkX % 32;
	if (LocalX < 0)
	{
		LocalX = 32 + LocalX;
	}
	int LocalZ = a_Chunk.m_ChunkZ % 32;
	if (LocalZ < 0)
	{
		LocalZ = 32 + LocalZ;
	}
	unsigned ChunkLocation = ntohl(m_Header[LocalX + 32 * LocalZ]);
	unsigned ChunkOffset = ChunkLocation >> 8;
	unsigned ChunkLen = ChunkLocation & 0xff;
	
	m_File.Seek(ChunkOffset * 4096);
	
	int ChunkSize = 0;
	if (m_File.Read(&ChunkSize, 4) != 4)
	{
		return false;
	}
	ChunkSize = ntohl(ChunkSize);
	char CompressionType = 0;
	if (m_File.Read(&CompressionType, 1) != 1)
	{
		return false;
	}
	if (CompressionType != 2)
	{
		// Chunk is in an unknown compression
		return false;
	}
	ChunkSize--;
	
	// HACK: This depends on the internal knowledge that AString's data() function returns the internal buffer directly
	a_Data.assign(ChunkSize, '\0');
	return (m_File.Read((void *)a_Data.data(), ChunkSize) == ChunkSize);
}




