
// WSSAnvil.cpp

// Implements the cWSSAnvil class representing the Anvil world storage scheme

#include "Globals.h"
#include "WSSAnvil.h"
#include "../cWorld.h"
#include "zlib.h"
#include "../BlockID.h"
#include "../cChestEntity.h"
#include "../cFurnaceEntity.h"
#include "../cSignEntity.h"
#include "../cNoteEntity.h"
#include "../cItem.h"
#include "../StringCompression.h"
#include "../cEntity.h"
#include "../cMakeDir.h"
#include "FastNBT.h"





/** Maximum number of MCA files that are cached in memory. 
Since only the header is actually in the memory, this number can be high, but still, each file means an OS FS handle.
*/
#define MAX_MCA_FILES 32

/// The maximum size of an inflated chunk; raw chunk data is 192 KiB, allow 64 KiB more of entities
#define CHUNK_INFLATE_MAX 256 KiB






///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cNBTChunkSerializer

class cNBTChunkSerializer :
	public cChunkDataSeparateCollector
{
public:
	cNBTChunkSerializer(cFastNBTWriter & a_Writer) :
		m_Writer(a_Writer),
		m_IsTagOpen(false),
		m_HasHadEntity(false),
		m_HasHadBlockEntity(false),
		m_IsLightValid(false)
	{
	}


	/// Close NBT tags that we've opened
	void Finish(void)
	{
		if (m_IsTagOpen)
		{
			m_Writer.EndList();
		}
	}
	
	
	bool IsLightValid(void) const {return m_IsLightValid; }
	
protected:
	
	/* From cChunkDataSeparateCollector we inherit:
	- m_BlockTypes[]
	- m_BlockMetas[]
	- m_BlockLight[]
	- m_BlockSkyLight[]
	*/
	
	// TODO: Biomes
	
	cFastNBTWriter & m_Writer;
	
	bool m_IsTagOpen;  // True if a tag has been opened in the callbacks and not yet closed.
	bool m_HasHadEntity;  // True if any Entity has already been received and processed
	bool m_HasHadBlockEntity;  // True if any BlockEntity has already been received and processed
	bool m_IsLightValid;  // True if the chunk lighting is valid
	
	
	void AddBasicTileEntity(cBlockEntity * a_Entity, const char * a_EntityTypeID)
	{
		m_Writer.AddInt   ("x",  a_Entity->GetPosX());
		m_Writer.AddInt   ("y",  a_Entity->GetPosY());
		m_Writer.AddInt   ("z",  a_Entity->GetPosZ());
		m_Writer.AddString("id", a_EntityTypeID);
	}
	
	
	void AddItem(const cItem * a_Item, int a_Slot)
	{
		m_Writer.BeginCompound("");
		m_Writer.AddShort("id",     (short)(a_Item->m_ItemID));
		m_Writer.AddShort("Damage", a_Item->m_ItemHealth);
		m_Writer.AddByte ("Count",  a_Item->m_ItemCount);
		m_Writer.AddByte ("Slot",   (unsigned char)a_Slot);
		m_Writer.EndCompound();
	}
	
	
	void AddChestEntity(cChestEntity * a_Entity)
	{
		m_Writer.BeginCompound("");
		AddBasicTileEntity(a_Entity, "Chest");
		m_Writer.BeginList("Items", TAG_Compound);
		for (int i = 0; i < cChestEntity::c_ChestHeight * cChestEntity::c_ChestWidth; i++)
		{
			const cItem * Item = a_Entity->GetSlot(i);
			if ((Item == NULL) || Item->IsEmpty())
			{
				continue;
			}
			AddItem(Item, i);
		}
		m_Writer.EndList();
		m_Writer.EndCompound();
	}
	
	
	void AddFurnaceEntity(cFurnaceEntity * a_Furnace)
	{
		m_Writer.BeginCompound("");
		AddBasicTileEntity(a_Furnace, "Furnace");
		m_Writer.BeginList("Items", TAG_Compound);
		AddItem(a_Furnace->GetSlot(0), 0);
		AddItem(a_Furnace->GetSlot(1), 1);
		AddItem(a_Furnace->GetSlot(2), 2);
		m_Writer.EndList();
		m_Writer.AddShort("BurnTime", (Int16)(a_Furnace->GetTimeToBurn() / 50.0));
		m_Writer.AddShort("CookTime", (Int16)(a_Furnace->GetTimeCooked() / 50.0));
		m_Writer.EndCompound();
	}


	void AddSignEntity(cSignEntity * a_Sign)
	{
		m_Writer.BeginCompound("");
		AddBasicTileEntity(a_Sign, "Sign");
		m_Writer.AddString("Text1",   a_Sign->GetLine(0));
		m_Writer.AddString("Text2",   a_Sign->GetLine(1));
		m_Writer.AddString("Text3",   a_Sign->GetLine(2));
		m_Writer.AddString("Text4",   a_Sign->GetLine(3));
		m_Writer.EndCompound();
	}

	void AddNoteEntity(cNoteEntity * a_Note)
	{
		m_Writer.BeginCompound("");
		AddBasicTileEntity(a_Note, "Music");
		m_Writer.AddByte("note", a_Note->GetPitch());
		m_Writer.EndCompound();
	}


	virtual bool LightIsValid(bool a_IsLightValid) override
	{
		m_IsLightValid = a_IsLightValid;
		return a_IsLightValid;  // We want lighting only if it's valid, otherwise don't bother
	}
	
	
	virtual void Entity(cEntity * a_Entity) override
	{
		// TODO: Add entity into NBT:
	}
	
	
	virtual void BlockEntity(cBlockEntity * a_Entity)
	{
		if (m_IsTagOpen)
		{
			if (!m_HasHadBlockEntity)
			{
				m_Writer.EndList();
				m_Writer.BeginList("TileEntities", TAG_Compound);
			}
		}
		else
		{
			m_Writer.BeginList("TileEntities", TAG_Compound);
		}
		m_IsTagOpen = true;
		
		// Add tile-entity into NBT:
		switch (a_Entity->GetBlockType())
		{
			case E_BLOCK_CHEST:     AddChestEntity  ((cChestEntity *)  a_Entity); break;
			case E_BLOCK_FURNACE:   AddFurnaceEntity((cFurnaceEntity *)a_Entity); break;
			case E_BLOCK_SIGN_POST:
			case E_BLOCK_WALLSIGN:  AddSignEntity   ((cSignEntity *)   a_Entity); break;
			case E_BLOCK_NOTE_BLOCK:  AddNoteEntity   ((cNoteEntity *)   a_Entity); break;
			default:
			{
				ASSERT(!"Unhandled block entity saved into Anvil");
			}
		}
		m_HasHadBlockEntity = true;
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
		cFastNBTWriter Writer;
		Writer.BeginCompound("");
		Writer.AddInt("SpawnX", (int)(a_World->GetSpawnX()));
		Writer.AddInt("SpawnY", (int)(a_World->GetSpawnY()));
		Writer.AddInt("SpawnZ", (int)(a_World->GetSpawnZ()));
		Writer.EndCompound();
		Writer.Finish();
		
		#ifdef _DEBUG
		cParsedNBT TestParse(Writer.GetResult().data(), Writer.GetResult().size());
		ASSERT(TestParse.IsValid());
		#endif  // _DEBUG
		
		gzFile gz = gzopen(fnam.c_str(), "wb");
		if (gz != NULL)
		{
			gzwrite(gz, Writer.GetResult().data(), Writer.GetResult().size());
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
	cParsedNBT NBT(Uncompressed, strm.total_out);
	if (!NBT.IsValid())
	{
		// NBT Parsing failed
		return false;
	}

	// Load the data from NBT:
	return LoadChunkFromNBT(a_Chunk, NBT);
}





bool cWSSAnvil::SaveChunkToData(const cChunkCoords & a_Chunk, AString & a_Data)
{
	cFastNBTWriter Writer;
	if (!SaveChunkToNBT(a_Chunk, Writer))
	{
		return false;
	}
	Writer.Finish();
	CompressString(Writer.GetResult().data(), Writer.GetResult().size(), a_Data);
	return true;
}





bool cWSSAnvil::LoadChunkFromNBT(const cChunkCoords & a_Chunk, const cParsedNBT & a_NBT)
{
	// The data arrays, in MCA-native y/z/x ordering (will be reordered for the final chunk data)
	cChunkDef::BlockTypes   BlockTypes;
	cChunkDef::BlockNibbles MetaData;
	cChunkDef::BlockNibbles BlockLight;
	cChunkDef::BlockNibbles SkyLight;
	
	memset(BlockTypes, E_BLOCK_AIR, sizeof(BlockTypes));
	memset(MetaData,   0,           sizeof(MetaData));
	memset(SkyLight,   0xff,        sizeof(SkyLight));  // By default, data not present in the NBT means air, which means full skylight
	memset(BlockLight, 0x00,        sizeof(BlockLight));
	
	// Load the blockdata, blocklight and skylight:
	int Level = a_NBT.FindChildByName(0, "Level");
	if (Level < 0)
	{
		return false;
	}
	int Sections = a_NBT.FindChildByName(Level, "Sections");
	if ((Sections < 0) || (a_NBT.GetType(Sections) != TAG_List) || (a_NBT.GetChildrenType(Sections) != TAG_Compound))
	{
		return false;
	}
	for (int Child = a_NBT.GetFirstChild(Sections); Child >= 0; Child = a_NBT.GetNextSibling(Child))
	{
		int y = 0;
		int SectionY = a_NBT.FindChildByName(Child, "Y");
		if ((SectionY < 0) || (a_NBT.GetType(SectionY) != TAG_Byte))
		{
			continue;
		}
		y = a_NBT.GetByte(SectionY);
		if ((y < 0) || (y > 15))
		{
			continue;
		}
		CopyNBTData(a_NBT, Child, "Blocks",     (char *)&(BlockTypes[y * 4096]), 4096);
		CopyNBTData(a_NBT, Child, "Data",       (char *)&(MetaData[y   * 2048]), 2048);
		CopyNBTData(a_NBT, Child, "SkyLight",   (char *)&(SkyLight[y   * 2048]), 2048);
		CopyNBTData(a_NBT, Child, "BlockLight", (char *)&(BlockLight[y * 2048]), 2048);
	}  // for itr - LevelSections[]
	
	// Load the biomes from NBT, if present and valid:
	cChunkDef::BiomeMap BiomeMap;
	cChunkDef::BiomeMap * Biomes = LoadBiomeMapFromNBT(&BiomeMap, a_NBT, a_NBT.FindChildByName(Level, "Biomes"));
	
	// Load the entities from NBT:
	cEntityList      Entities;
	cBlockEntityList BlockEntities;
	LoadEntitiesFromNBT     (Entities,      a_NBT, a_NBT.FindChildByName(Level, "Entities"));
	LoadBlockEntitiesFromNBT(BlockEntities, a_NBT, a_NBT.FindChildByName(Level, "TileEntities"));
	
	bool IsLightValid = (a_NBT.FindChildByName(Level, "MCSIsLightValid") > 0);
	
	/*
	// Uncomment this block for really cool stuff :)
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
				BlockTypes[Index] = (BlockTypes[Index] == E_BLOCK_AIR) ? E_BLOCK_STONE : E_BLOCK_AIR;
			}
			else
			{
				switch (BlockTypes[Index])
				{
					case E_BLOCK_AIR:
					case E_BLOCK_LEAVES:
					{
						// nothing needed
						break;
					}
					default:
					{
						ShouldInvert[x + cChunkDef::Width * z] = true;
					}
				}
				BlockTypes[Index] = E_BLOCK_AIR;
			}
		}
	}  // for y
	//*/
	
	m_World->SetChunkData(
		a_Chunk.m_ChunkX, a_Chunk.m_ChunkY, a_Chunk.m_ChunkZ,
		BlockTypes, MetaData, 
		IsLightValid ? BlockLight : NULL,
		IsLightValid ? SkyLight : NULL,
		NULL, Biomes,
		Entities, BlockEntities,
		false
	);
	return true;
}




void cWSSAnvil::CopyNBTData(const cParsedNBT & a_NBT, int a_Tag, const AString & a_ChildName, char * a_Destination, int a_Length)
{
	int Child = a_NBT.FindChildByName(a_Tag, a_ChildName);
	if ((Child >= 0) && (a_NBT.GetType(Child) == TAG_ByteArray) && (a_NBT.GetDataLength(Child) == a_Length))
	{
		memcpy(a_Destination, a_NBT.GetData(Child), a_Length);
	}
}





bool cWSSAnvil::SaveChunkToNBT(const cChunkCoords & a_Chunk, cFastNBTWriter & a_Writer)
{
	a_Writer.BeginCompound("Level");
	a_Writer.AddInt("xPos", a_Chunk.m_ChunkX);
	a_Writer.AddInt("zPos", a_Chunk.m_ChunkZ);
	cNBTChunkSerializer Serializer(a_Writer);
	if (!m_World->GetChunkData(a_Chunk.m_ChunkX, a_Chunk.m_ChunkY, a_Chunk.m_ChunkZ, Serializer))
	{
		return false;
	}
	Serializer.Finish();  // Close NBT tags
	
	// TODO: Save biomes, both MCS (IntArray) and MC-vanilla (ByteArray):
	// Level->Add(new cNBTByteArray(Level, "Biomes", AString(Serializer.m_Biomes, sizeof(Serializer.m_Biomes));
	// Level->Add(new cNBTByteArray(Level, "MCSBiomes", AString(Serializer.m_Biomes, sizeof(Serializer.m_Biomes));
	
	// Save blockdata:
	a_Writer.BeginList("Sections", TAG_Compound);
	int SliceSizeBlock  = cChunkDef::Width * cChunkDef::Width * 16;
	int SliceSizeNibble = SliceSizeBlock / 2;
	const char * BlockTypes    = (const char *)(Serializer.m_BlockTypes);
	const char * BlockMetas    = (const char *)(Serializer.m_BlockMetas);
	const char * BlockLight    = (const char *)(Serializer.m_BlockLight);
	const char * BlockSkyLight = (const char *)(Serializer.m_BlockSkyLight);
	for (int Y = 0; Y < 16; Y++)
	{
		a_Writer.BeginCompound("");
		a_Writer.AddByteArray("Blocks",     BlockTypes    + Y * SliceSizeBlock,  SliceSizeBlock);
		a_Writer.AddByteArray("Data",       BlockMetas    + Y * SliceSizeNibble, SliceSizeNibble);
		a_Writer.AddByteArray("SkyLight",   BlockSkyLight + Y * SliceSizeNibble, SliceSizeNibble);
		a_Writer.AddByteArray("BlockLight", BlockLight    + Y * SliceSizeNibble, SliceSizeNibble);
		a_Writer.AddByte("Y", (unsigned char)Y);
		a_Writer.EndCompound();
	}
	a_Writer.EndList();  // "Sections"
	
	// Store the information that the lighting is valid. 
	// For compatibility reason, the default is "invalid" (missing) - this means older data is re-lighted upon loading.
	if (Serializer.IsLightValid())
	{
		a_Writer.AddByte("MCSIsLightValid", 1);
	}
	
	a_Writer.EndCompound();  // "Level"
	return true;
}





cChunkDef::BiomeMap * cWSSAnvil::LoadBiomeMapFromNBT(cChunkDef::BiomeMap * a_BiomeMap, const cParsedNBT & a_NBT, int a_TagIdx)
{
	if ((a_TagIdx < 0) || (a_NBT.GetType(a_TagIdx) != TAG_ByteArray))
	{
		return NULL;
	}
	if (a_NBT.GetDataLength(a_TagIdx) != sizeof(*a_BiomeMap))
	{
		// The biomes stored don't match in size
		return NULL;
	}
	memcpy(a_BiomeMap, a_NBT.GetData(a_TagIdx), sizeof(*a_BiomeMap));
	for (int i = 0; i < ARRAYCOUNT(*a_BiomeMap); i++)
	{
		if ((*a_BiomeMap)[i] == 0xff)
		{
			// Unassigned biomes
			return NULL;
		}
	}
	return a_BiomeMap;
}





void cWSSAnvil::LoadEntitiesFromNBT(cEntityList & a_Entitites, const cParsedNBT & a_NBT, int a_TagIdx)
{
	// TODO: Load the entities
}





void cWSSAnvil::LoadBlockEntitiesFromNBT(cBlockEntityList & a_BlockEntities, const cParsedNBT & a_NBT, int a_TagIdx)
{
	if ((a_TagIdx < 0) || (a_NBT.GetType(a_TagIdx) != TAG_List))
	{
		return;
	}
	
	for (int Child = a_NBT.GetFirstChild(a_TagIdx); Child != -1; Child = a_NBT.GetNextSibling(Child))
	{
		if (a_NBT.GetType(Child) != TAG_Compound)
		{
			continue;
		}
		int sID = a_NBT.FindChildByName(Child, "id");
		if (sID < 0)
		{
			continue;
		}
		if (strncmp(a_NBT.GetData(sID), "Chest", a_NBT.GetDataLength(sID)) == 0)
		{
			LoadChestFromNBT(a_BlockEntities, a_NBT, Child);
		}
		else if (strncmp(a_NBT.GetData(sID), "Furnace", a_NBT.GetDataLength(sID)) == 0)
		{
			LoadFurnaceFromNBT(a_BlockEntities, a_NBT, Child);
		}
		else if (strncmp(a_NBT.GetData(sID), "Sign", a_NBT.GetDataLength(sID)) == 0)
		{
			LoadSignFromNBT(a_BlockEntities, a_NBT, Child);
		}
		else if (strncmp(a_NBT.GetData(sID), "Music", a_NBT.GetDataLength(sID)) == 0)
		{
			LoadNoteFromNBT(a_BlockEntities, a_NBT, Child);
		}
		// TODO: Other block entities
	}  // for Child - tag children
}





void cWSSAnvil::LoadChestFromNBT(cBlockEntityList & a_BlockEntities, const cParsedNBT & a_NBT, int a_TagIdx)
{
	ASSERT(a_NBT.GetType(a_TagIdx) == TAG_Compound);
	int x, y, z;
	if (!GetBlockEntityNBTPos(a_NBT, a_TagIdx, x, y, z))
	{
		return;
	}
	int Items = a_NBT.FindChildByName(a_TagIdx, "Items");
	if ((Items < 0) || (a_NBT.GetType(Items) != TAG_List))
	{
		return;  // Make it an empty chest - the chunk loader will provide an empty cChestEntity for this
	}
	std::auto_ptr<cChestEntity> Chest(new cChestEntity(x, y, z, m_World));
	for (int Child = a_NBT.GetFirstChild(Items); Child != -1; Child = a_NBT.GetNextSibling(Child))
	{
		int Slot = a_NBT.FindChildByName(Child, "Slot");
		if ((Slot < 0) || (a_NBT.GetType(Slot) != TAG_Byte))
		{
			continue;
		}
		cItem Item;
		int ID = a_NBT.FindChildByName(Child, "id");
		if ((ID < 0) || (a_NBT.GetType(ID) != TAG_Short))
		{
			continue;
		}
		Item.m_ItemID = (ENUM_ITEM_ID)(a_NBT.GetShort(ID));
		int Damage = a_NBT.FindChildByName(Child, "Damage");
		if ((Damage < 0) || (a_NBT.GetType(Damage) != TAG_Short))
		{
			continue;
		}
		Item.m_ItemHealth = a_NBT.GetShort(Damage);
		int Count = a_NBT.FindChildByName(Child, "Count");
		if ((Count < 0) || (a_NBT.GetType(Count) != TAG_Byte))
		{
			continue;
		}
		Item.m_ItemCount = a_NBT.GetByte(Count);
		Chest->SetSlot(a_NBT.GetByte(Slot), Item);
	}  // for itr - ItemDefs[]
	a_BlockEntities.push_back(Chest.release());
}





void cWSSAnvil::LoadFurnaceFromNBT(cBlockEntityList & a_BlockEntities, const cParsedNBT & a_NBT, int a_TagIdx)
{
	ASSERT(a_NBT.GetType(a_TagIdx) == TAG_Compound);
	int x, y, z;
	if (!GetBlockEntityNBTPos(a_NBT, a_TagIdx, x, y, z))
	{
		return;
	}
	int Items = a_NBT.FindChildByName(a_TagIdx, "Items");
	if ((Items < 0) || (a_NBT.GetType(Items) != TAG_List))
	{
		return;  // Make it an empty furnace - the chunk loader will provide an empty cFurnaceEntity for this
	}
	std::auto_ptr<cFurnaceEntity> Furnace(new cFurnaceEntity(x, y, z, m_World));
	for (int Child = a_NBT.GetFirstChild(Items); Child != -1; Child = a_NBT.GetNextSibling(Child))
	{
		int Slot = a_NBT.FindChildByName(Child, "Slot");
		if ((Slot < 0) || (a_NBT.GetType(Slot) != TAG_Byte))
		{
			continue;
		}
		cItem Item;
		int ID = a_NBT.FindChildByName(Child, "id");
		if ((ID < 0) || (a_NBT.GetType(ID) != TAG_Short))
		{
			continue;
		}
		Item.m_ItemID = (ENUM_ITEM_ID)(a_NBT.GetShort(ID));
		int Damage = a_NBT.FindChildByName(Child, "Damage");
		if ((Damage < 0) || (a_NBT.GetType(Damage) != TAG_Short))
		{
			continue;
		}
		Item.m_ItemHealth = a_NBT.GetShort(Damage);
		int Count = a_NBT.FindChildByName(Child, "Count");
		if ((Count < 0) || (a_NBT.GetType(Count) != TAG_Byte))
		{
			continue;
		}
		Item.m_ItemCount = a_NBT.GetByte(Count);
		Furnace->SetSlot(a_NBT.GetByte(Slot), Item);
	}  // for itr - ItemDefs[]
	int BurnTime = a_NBT.FindChildByName(a_TagIdx, "BurnTime");
	if (BurnTime >= 0)
	{
		Int16 bt = a_NBT.GetShort(BurnTime);
		// Anvil doesn't store the time that the fuel can burn. We simply "reset" the current value to be the 100%
		Furnace->SetBurnTimes((float)(bt * 50.0), (float)(bt * 50.0));
	}
	int CookTime = a_NBT.FindChildByName(a_TagIdx, "CookTime");
	if (CookTime >= 0)
	{
		Int16 ct = a_NBT.GetShort(CookTime);
		// Anvil doesn't store the time that an item takes to cook. We simply use the default - 10 seconds
		Furnace->SetCookTimes(10000.0, (float)(ct * 50.0));
	}
	Furnace->ContinueCooking();
	a_BlockEntities.push_back(Furnace.release());
}





void cWSSAnvil::LoadSignFromNBT(cBlockEntityList & a_BlockEntities, const cParsedNBT & a_NBT, int a_TagIdx)
{
	ASSERT(a_NBT.GetType(a_TagIdx) == TAG_Compound);
	int x, y, z;
	if (!GetBlockEntityNBTPos(a_NBT, a_TagIdx, x, y, z))
	{
		return;
	}
	std::auto_ptr<cSignEntity> Sign(new cSignEntity(E_BLOCK_SIGN_POST, x, y, z, m_World));

	int currentLine = a_NBT.FindChildByName(a_TagIdx, "Text1");
	if (currentLine >= 0)
	{
		Sign->SetLine(0, a_NBT.GetString(currentLine));
	}

	currentLine = a_NBT.FindChildByName(a_TagIdx, "Text2");
	if (currentLine >= 0)
	{
		Sign->SetLine(1, a_NBT.GetString(currentLine));
	}

	currentLine = a_NBT.FindChildByName(a_TagIdx, "Text3");
	if (currentLine >= 0)
	{
		Sign->SetLine(2, a_NBT.GetString(currentLine));
	}

	currentLine = a_NBT.FindChildByName(a_TagIdx, "Text4");
	if (currentLine >= 0)
	{
		Sign->SetLine(3, a_NBT.GetString(currentLine));
	}

	a_BlockEntities.push_back(Sign.release());
}




void cWSSAnvil::LoadNoteFromNBT(cBlockEntityList & a_BlockEntities, const cParsedNBT & a_NBT, int a_TagIdx)
{
	ASSERT(a_NBT.GetType(a_TagIdx) == TAG_Compound);
	int x, y, z;
	if (!GetBlockEntityNBTPos(a_NBT, a_TagIdx, x, y, z))
	{
		return;
	}
	std::auto_ptr<cNoteEntity> Note(new cNoteEntity(x, y, z, m_World));
	int note = a_NBT.FindChildByName(a_TagIdx, "note");
	if (note >= 0)
	{
		Note->SetPitch(a_NBT.GetByte(note));
	}
	a_BlockEntities.push_back(Note.release());
}





bool cWSSAnvil::GetBlockEntityNBTPos(const cParsedNBT & a_NBT, int a_TagIdx, int & a_X, int & a_Y, int & a_Z)
{
	int x = a_NBT.FindChildByName(a_TagIdx, "x");
	if ((x < 0) || (a_NBT.GetType(x) != TAG_Int))
	{
		return false;
	}
	int y = a_NBT.FindChildByName(a_TagIdx, "y");
	if ((y < 0) || (a_NBT.GetType(y) != TAG_Int))
	{
		return false;
	}
	int z = a_NBT.FindChildByName(a_TagIdx, "z");
	if ((z < 0) || (a_NBT.GetType(z) != TAG_Int))
	{
		return false;
	}
	a_X = a_NBT.GetInt(x);
	a_Y = a_NBT.GetInt(y);
	a_Z = a_NBT.GetInt(z);
	return true;
}





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cWSSAnvil::cMCAFile:

cWSSAnvil::cMCAFile::cMCAFile(const AString & a_FileName, int a_RegionX, int a_RegionZ) :
	m_RegionX(a_RegionX),
	m_RegionZ(a_RegionZ),
	m_FileName(a_FileName)
{
}





bool cWSSAnvil::cMCAFile::OpenFile(bool a_IsForReading)
{
	if (m_File.IsOpen())
	{
		// Already open
		return true;
	}
	
	if (a_IsForReading)
	{
		if (!cFile::Exists(m_FileName))
		{
			// We want to read and the file doesn't exist. Fail.
			return false;
		}
	}
	
	if (!m_File.Open(m_FileName, cFile::fmReadWrite))
	{
		// The file failed to open
		return false;
	}
	
	// Load the header:
	if (m_File.Read(m_Header, sizeof(m_Header)) != sizeof(m_Header))
	{
		// Cannot read the header - perhaps the file has just been created?
		// Try writing a NULL header (both chunk offsets and timestamps):
		memset(m_Header, 0, sizeof(m_Header));
		if (
			(m_File.Write(m_Header, sizeof(m_Header)) != sizeof(m_Header)) ||  // Real header - chunk offsets
			(m_File.Write(m_Header, sizeof(m_Header)) != sizeof(m_Header))     // Bogus data for the chunk timestamps
		)
		{
			LOGWARNING("Cannot process MCA header in file \"%s\", chunks in that file will be lost", m_FileName.c_str());
			m_File.Close();
			return false;
		}
	}
	return true;
}





bool cWSSAnvil::cMCAFile::GetChunkData(const cChunkCoords & a_Chunk, AString & a_Data)
{
	if (!OpenFile(true))
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
	if (!OpenFile(false))
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
	if (m_File.Write(a_Data.data(), a_Data.size()) != (int)(a_Data.size()))
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




