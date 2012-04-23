
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
#include "StringCompression.h"
#include "cEntity.h"
#include "cBlockEntity.h"
#include "cMakeDir.h"





/** Maximum number of MCA files that are cached in memory. 
Since only the header is actually in the memory, this number can be high, but still, each file means an OS FS handle.
*/
#define MAX_MCA_FILES 32

/// The maximum size of an inflated chunk
#define CHUNK_INFLATE_MAX 256 KiB






///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cNBTChunkSerializer

class cNBTChunkSerializer :
	public cChunkDataSeparateCollector
{
public:
	cNBTChunkSerializer(cNBTList * a_Entities, cNBTList * a_TileEntities) :
		m_Entities(a_Entities),
		m_TileEntities(a_TileEntities)
	{
	}
	
protected:
	
	/* From cChunkDataSeparateCollector we inherit:
	- m_BlockTypes[]
	- m_BlockMetas[]
	- m_BlockLight[]
	- m_BlockSkyLight[]
	*/
	
	// TODO: Biomes
	
	// We need to save entities and blockentities into NBT
	cNBTList * m_Entities;  // Tag where entities will be saved
	cNBTList * m_TileEntities;  // Tag where block-entities will be saved
	
	
	cNBTCompound * AddBasicTileEntity(cBlockEntity * a_Entity, const char * a_EntityTypeID)
	{
		cNBTCompound * res = new cNBTCompound(m_TileEntities);
		res->Add(new cNBTInt   (res, "x",  a_Entity->GetPosX()));
		res->Add(new cNBTInt   (res, "y",  a_Entity->GetPosY()));
		res->Add(new cNBTInt   (res, "z",  a_Entity->GetPosZ()));
		res->Add(new cNBTString(res, "id", a_EntityTypeID));
		return res;
	}
	
	
	void AddItem(cNBTList * a_Items, cItem * a_Item, int a_Slot)
	{
		cNBTCompound * Tag = new cNBTCompound(a_Items);
		Tag->Add(new cNBTShort(Tag, "id",     a_Item->m_ItemID));
		Tag->Add(new cNBTShort(Tag, "Damage", a_Item->m_ItemHealth));
		Tag->Add(new cNBTByte (Tag, "Count",  a_Item->m_ItemCount));
		Tag->Add(new cNBTByte (Tag, "Slot",   a_Slot));
	}
	
	
	void AddChestEntity(cChestEntity * a_Entity)
	{
		cNBTCompound * Base = AddBasicTileEntity(a_Entity, "chest");
		cNBTList * Items = new cNBTList(Base, "Items", cNBTTag::TAG_Compound);
		Base->Add(Items);
		for (int i = 0; i < cChestEntity::c_ChestHeight * cChestEntity::c_ChestWidth; i++)
		{
			cItem * Item = a_Entity->GetSlot(i);
			if ((Item == NULL) || Item->IsEmpty())
			{
				continue;
			}
			AddItem(Items, Item, i);
		}
	}


	virtual void Entity(cEntity * a_Entity) override
	{
		// TODO: Add entity into NBT:
	}
	
	
	virtual void BlockEntity(cBlockEntity * a_Entity)
	{
		// Add tile-entity into NBT:
		switch (a_Entity->GetBlockType())
		{
			case E_BLOCK_CHEST: AddChestEntity((cChestEntity *)a_Entity); break;
			default:
			{
				ASSERT(!"Unhandled block entity saved into Anvil");
			}
		}
	}
} ;  // class cNBTChunkSerializer





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cWSSAnvil:

cWSSAnvil::cWSSAnvil(cWorld * a_World) :
	super(a_World)
{
	// Create a level.dat file for mapping tools, if it doesn't already exist:
	AString fnam;
	Printf(fnam, "%s/level.dat", a_World->GetName().c_str());
	if (!cFile::Exists(fnam))
	{
		std::auto_ptr<cNBTCompound> Root(new cNBTCompound(NULL));
		cNBTCompound * Data = new cNBTCompound(Root.get());
		Root->Add(Data);
		Data->Add(new cNBTInt(Data, "SpawnX", (int)(a_World->GetSpawnX())));
		Data->Add(new cNBTInt(Data, "SpawnY", (int)(a_World->GetSpawnY())));
		Data->Add(new cNBTInt(Data, "SpawnZ", (int)(a_World->GetSpawnZ())));
		AString Uncompressed;
		cNBTSerializer::Serialize(Root.get(), Uncompressed);
		gzFile gz = gzopen(fnam.c_str(), "wb");
		if (gz != NULL)
		{
			gzwrite(gz, Uncompressed.data(), Uncompressed.size());
		}
		gzclose(gz);
	}
}





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
	AString ChunkData;
	if (!SaveChunkToData(a_Chunk, ChunkData))
	{
		return false;
	}
	if (!SetChunkData(a_Chunk, ChunkData))
	{
		return false;
	}
	
	// Everything successful
	return true;
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





bool cWSSAnvil::SetChunkData(const cChunkCoords & a_Chunk, const AString & a_Data)
{
	cCSLock Lock(m_CS);
	cMCAFile * File = LoadMCAFile(a_Chunk);
	if (File == NULL)
	{
		return false;
	}
	return File->SetChunkData(a_Chunk, a_Data);
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
	Printf(FileName, "%s/region", m_World->GetName().c_str());
	cMakeDir::MakeDir(FileName);
	AppendPrintf(FileName, "/r.%d.%d.mca", RegionX, RegionZ);
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





bool cWSSAnvil::SaveChunkToData(const cChunkCoords & a_Chunk, AString & a_Data)
{
	std::auto_ptr<cNBTTree> Tree(SaveChunkToNBT(a_Chunk));
	if (Tree.get() == NULL)
	{
		return false;
	}
	AString Uncompressed;
	cNBTSerializer::Serialize(Tree.get(), Uncompressed);
	CompressString(Uncompressed.data(), Uncompressed.size(), a_Data);
	return true;
}





bool cWSSAnvil::LoadChunkFromNBT(const cChunkCoords & a_Chunk, cNBTTag & a_NBT)
{
	// The data arrays, in MCA-native y/z/x ordering (will be reordered for the final chunk data)
	BLOCKTYPE BlockData[cChunkDef::BlockDataSize];
	BLOCKTYPE * MetaData   = BlockData + cChunkDef::MetaOffset;
	BLOCKTYPE * BlockLight = BlockData + cChunkDef::LightOffset;
	BLOCKTYPE * SkyLight   = BlockData + cChunkDef::SkyLightOffset;
	
	memset(BlockData,  E_BLOCK_AIR, sizeof(BlockData) - cChunkDef::NumBlocks / 2);
	memset(SkyLight,   0xff,        cChunkDef::NumBlocks / 2);  // By default, data not present in the NBT means air, which means full skylight
	
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
	
	#if (AXIS_ORDER == AXIS_ORDER_YZX)
	// Reorder the chunk data - walk the MCA-formatted data sequentially and copy it into the right place in the ChunkData:
	BLOCKTYPE ChunkData[cChunkDef::BlockDataSize];
	memset(ChunkData, 0, sizeof(ChunkData));
	int Index = 0;  // Index into the MCA-formatted data, incremented sequentially
	for (int y = 0; y < cChunkDef::Height; y++) for (int z = 0; z < cChunkDef::Width; z++) for (int x = 0; x < cChunkDef::Width; x++)
	{
		ChunkData[cChunk::MakeIndex(x, y, z)] = BlockData[Index];
		Index++;
	}  // for y/z/x
	BLOCKTYPE * ChunkMeta = ChunkData + cChunkDef::NumBlocks;
	Index = 0;
	for (int y = 0; y < cChunkDef::Height; y++) for (int z = 0; z < cChunkDef::Width; z++) for (int x = 0; x < cChunkDef::Width; x++)
	{
		cChunk::SetNibble(ChunkMeta, x, y, z, MetaData[Index / 2] >> ((Index % 2) * 4));
		Index++;
	}  // for y/z/x
	BLOCKTYPE * ChunkBlockLight = ChunkMeta + cChunkDef::NumBlocks / 2;
	Index = 0;
	for (int y = 0; y < cChunkDef::Height; y++) for (int z = 0; z < cChunkDef::Width; z++) for (int x = 0; x < cChunkDef::Width; x++)
	{
		cChunk::SetNibble(ChunkBlockLight, x, y, z, BlockLight[Index / 2] >> ((Index % 2) * 4));
		Index++;
	}  // for y/z/x
	BLOCKTYPE * ChunkSkyLight = ChunkBlockLight + cChunkDef::NumBlocks / 2;
	Index = 0;
	for (int y = 0; y < cChunkDef::Height; y++) for (int z = 0; z < cChunkDef::Width; z++) for (int x = 0; x < cChunkDef::Width; x++)
	{
		cChunk::SetNibble(ChunkSkyLight, x, y, z, SkyLight[Index / 2] >> ((Index % 2) * 4));
		Index++;
	}  // for y/z/x
	#else  // AXIS_ORDER_YZX
	BLOCKTYPE * ChunkData = BlockData;
	#endif  // else AXIS_ORDER_YZX
	
	/*
	// Delete the comment above for really cool stuff :)
	// DEBUG magic: Invert the underground, so that we can see the MC generator in action :)
	bool ShouldInvert[cChunkDef::Width * cChunkDef::Width];
	memset(ShouldInvert, 0, sizeof(ShouldInvert));
	for (int y = cChunkDef::Height - 1; y >= 0; y--)
	{
		for (int x = 0; x < cChunkDef::Width; x++) for (int z = 0; z < cChunkDef::Width; z++)
		{
			int Index = cChunkDef::MakeIndexNoCheck(x, y, z);
			if (ShouldInvert[x + cChunkDef::Width * z])
			{
				ChunkData[Index] = (ChunkData[Index] == E_BLOCK_AIR) ? E_BLOCK_STONE : E_BLOCK_AIR;
			}
			else
			{
				ShouldInvert[x + cChunkDef::Width * z] = (ChunkData[Index] != E_BLOCK_AIR);
			}
		}
	}  // for y
	// Set everything alight, so that we can see:
	memset(ChunkData + cChunkDef::SkyLightOffset, 0xff, cChunkDef::NumBlocks / 2);
	//*/
	
	m_World->ChunkDataLoaded(
		a_Chunk.m_ChunkX, a_Chunk.m_ChunkY, a_Chunk.m_ChunkZ,
		ChunkData,
		ChunkData + cChunkDef::MetaOffset,
		ChunkData + cChunkDef::LightOffset,
		ChunkData + cChunkDef::SkyLightOffset,
		NULL,
		Entities,
		BlockEntities
	);
	return true;
}





cNBTTag * cWSSAnvil::SaveChunkToNBT(const cChunkCoords & a_Chunk)
{
	std::auto_ptr<cNBTCompound> res(new cNBTCompound(NULL));
	cNBTCompound * Level = new cNBTCompound(res.get(), "Level");
	res->Add(Level);
	cNBTList * Entities = new cNBTList(Level, "Entities", cNBTTag::TAG_Compound);
	Level->Add(Entities);
	cNBTList * TileEntities = new cNBTList(Level, "TileEntities", cNBTTag::TAG_Compound);
	Level->Add(TileEntities);
	cNBTChunkSerializer Serializer(Entities, TileEntities);
	if (!m_World->GetChunkData(a_Chunk.m_ChunkX, a_Chunk.m_ChunkY, a_Chunk.m_ChunkZ, Serializer))
	{
		return NULL;
	}
	
	Level->Add(new cNBTInt(Level, "xPos", a_Chunk.m_ChunkX));
	Level->Add(new cNBTInt(Level, "zPos", a_Chunk.m_ChunkZ));
	
	// TODO: Save biomes:
	// Level->Add(new cNBTByteArray(Level, "Biomes", AString(Serializer.m_Biomes, sizeof(Serializer.m_Biomes));
	
	// Save blockdata:
	cNBTList * Sections = new cNBTList(Level, "Sections", cNBTTag::TAG_Compound);
	Level->Add(Sections);
	int SliceSizeBlock  = cChunkDef::Width * cChunkDef::Width * 16;
	int SliceSizeNibble = SliceSizeBlock / 2;
	for (int Y = 0; Y < 16; Y++)
	{
		cNBTCompound * Slice = new cNBTCompound(Sections);
		Sections->Add(Slice);
		Slice->Add(new cNBTByteArray(Slice, "Blocks",     AString(Serializer.m_BlockTypes    + Y * SliceSizeBlock,  SliceSizeBlock)));
		Slice->Add(new cNBTByteArray(Slice, "Data",       AString(Serializer.m_BlockMetas    + Y * SliceSizeNibble, SliceSizeNibble)));
		Slice->Add(new cNBTByteArray(Slice, "SkyLight",   AString(Serializer.m_BlockSkyLight + Y * SliceSizeNibble, SliceSizeNibble)));
		Slice->Add(new cNBTByteArray(Slice, "BlockLight", AString(Serializer.m_BlockLight    + Y * SliceSizeNibble, SliceSizeNibble)));
		Slice->Add(new cNBTByte(Slice, "Y", Y));
	}
	
	return res.release();
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
	m_File(a_FileName, cFile::fmReadWrite),
	m_FileName(a_FileName)
{
	if (!m_File.IsOpen())
	{
		return;
	}
	
	// Load the header:
	if (m_File.Read(m_Header, sizeof(m_Header)) != sizeof(m_Header))
	{
		// Cannot read the header - perhaps the file has just been created?
		// Try writing a NULL header (both chunk offsets and timestamps):
		memset(m_Header, 0, sizeof(m_Header));
		if (
			(m_File.Write(m_Header, sizeof(m_Header)) != sizeof(m_Header)) ||
			(m_File.Write(m_Header, sizeof(m_Header)) != sizeof(m_Header))
		)
		{
			LOGWARNING("Cannot process MCA header in file \"%s\", chunks in that file will be lost", m_FileName.c_str());
			m_File.Close();
			return;
		}
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





bool cWSSAnvil::cMCAFile::SetChunkData(const cChunkCoords & a_Chunk, const AString & a_Data)
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
	
	unsigned ChunkSector = FindFreeLocation(LocalX, LocalZ, a_Data);

	// Store the chunk data:
	m_File.Seek(ChunkSector * 4096);
	unsigned ChunkSize = htonl(a_Data.size() + 1);
	if (m_File.Write(&ChunkSize, 4) != 4)
	{
		return false;
	}
	char CompressionType = 2;
	if (m_File.Write(&CompressionType, 1) != 1)
	{
		return false;
	}
	if (m_File.Write(a_Data.data(), a_Data.size()) != a_Data.size())
	{
		return false;
	}
	
	// Store the header:
	ChunkSize = (a_Data.size() + MCA_CHUNK_HEADER_LENGTH + 4095) / 4096;  // Round data size *up* to nearest 4KB sector, make it a sector number
	ASSERT(ChunkSize < 256);
	m_Header[LocalX + 32 * LocalZ] = htonl((ChunkSector << 8) | ChunkSize);
	m_File.Seek(0);
	if (m_File.Write(m_Header, sizeof(m_Header)) != sizeof(m_Header))
	{
		return false;
	}
	
	return true;
}





unsigned cWSSAnvil::cMCAFile::FindFreeLocation(int a_LocalX, int a_LocalZ, const AString & a_Data)
{
	// See if it fits the current location:
	unsigned ChunkLocation = ntohl(m_Header[a_LocalX + 32 * a_LocalZ]);
	unsigned ChunkLen = ChunkLocation & 0xff;
	if (a_Data.size() + MCA_CHUNK_HEADER_LENGTH <= (ChunkLen * 4096))
	{
		return ChunkLocation >> 8;
	}
	
	// Doesn't fit, append to the end of file (we're wasting a lot of space, TODO: fix this later)
	unsigned MaxLocation = 2 << 8;  // Minimum sector is #2 - after the headers
	for (int i = 0; i < ARRAYCOUNT(m_Header); i++)
	{
		ChunkLocation = ntohl(m_Header[i]);
		ChunkLocation = ChunkLocation + ((ChunkLocation & 0xff) << 8);  // Add the number of sectors used; don't care about the 4th byte
		if (MaxLocation < ChunkLocation)
		{
			MaxLocation = ChunkLocation;
		}
	}  // for i - m_Header[]
	return MaxLocation >> 8;
}
