
// WSSAnvil.cpp

// Implements the cWSSAnvil class representing the Anvil world storage scheme

#include "Globals.h"
#include "WSSAnvil.h"
#include "NBTChunkSerializer.h"
#include "FastNBT.h"
#include "EnchantmentSerializer.h"
#include "zlib/zlib.h"
#include "../World.h"
#include "../BlockID.h"
#include "../Item.h"
#include "../ItemGrid.h"
#include "../StringCompression.h"
#include "../SetChunkData.h"
#include "../Root.h"

#include "../BlockEntities/BeaconEntity.h"
#include "../BlockEntities/ChestEntity.h"
#include "../BlockEntities/CommandBlockEntity.h"
#include "../BlockEntities/DispenserEntity.h"
#include "../BlockEntities/DropperEntity.h"
#include "../BlockEntities/FurnaceEntity.h"
#include "../BlockEntities/HopperEntity.h"
#include "../BlockEntities/JukeboxEntity.h"
#include "../BlockEntities/NoteEntity.h"
#include "../BlockEntities/SignEntity.h"
#include "../BlockEntities/MobHeadEntity.h"
#include "../BlockEntities/MobSpawnerEntity.h"
#include "../BlockEntities/FlowerPotEntity.h"

#include "../Mobs/Monster.h"
#include "../Mobs/IncludeAllMonsters.h"

#include "../Entities/Boat.h"
#include "../Entities/EnderCrystal.h"
#include "../Entities/FallingBlock.h"
#include "../Entities/Minecart.h"
#include "../Entities/Pickup.h"
#include "../Entities/ArrowEntity.h"
#include "../Entities/SplashPotionEntity.h"
#include "../Entities/ThrownEggEntity.h"
#include "../Entities/ThrownEnderPearlEntity.h"
#include "../Entities/ThrownSnowballEntity.h"
#include "../Entities/FireChargeEntity.h"
#include "../Entities/GhastFireballEntity.h"
#include "../Entities/TNTEntity.h"
#include "../Entities/ExpOrb.h"
#include "../Entities/HangingEntity.h"
#include "../Entities/ItemFrame.h"

#include "../Protocol/MojangAPI.h"
#include "Server.h"





/** If defined, the BlockSkyLight values will be copied over to BlockLight upon chunk saving,
thus making skylight visible in Minutor's Lighting mode
*/
// #define DEBUG_SKYLIGHT

/** Maximum number of MCA files that are cached in memory.
Since only the header is actually in the memory, this number can be high, but still, each file means an OS FS handle.
*/
#define MAX_MCA_FILES 32

#define LOAD_FAILED(CHX, CHZ) \
	{ \
		const int RegionX = FAST_FLOOR_DIV(CHX, 32); \
		const int RegionZ = FAST_FLOOR_DIV(CHZ, 32); \
		LOGERROR("%s (%d): Loading chunk [%d, %d] from file r.%d.%d.mca failed. " \
			"The server will now abort in order to avoid further data loss. " \
			"Please add the reported file and this message to the issue report.", \
			__FUNCTION__, __LINE__, CHX, CHZ, RegionX, RegionZ \
		); \
		*((volatile int *)0) = 0;  /* Crash intentionally */ \
	}






////////////////////////////////////////////////////////////////////////////////
// cWSSAnvil:

cWSSAnvil::cWSSAnvil(cWorld * a_World, int a_CompressionFactor) :
	super(a_World),
	m_CompressionFactor(a_CompressionFactor)
{
	// Create a level.dat file for mapping tools, if it doesn't already exist:
	AString fnam;
	Printf(fnam, "%s/level.dat", a_World->GetName().c_str());
	if (!cFile::Exists(fnam))
	{
		cFastNBTWriter Writer;
		Writer.BeginCompound("Data");
		Writer.AddByte("allowCommands", 1);
		Writer.AddByte("Difficulty", 2);
		Writer.AddByte("hardcore", cRoot::Get()->GetServer()->IsHardcore() ? 1 : 0);
		Writer.AddByte("initialized", 1);
		Writer.AddByte("MapFeatures", 1);
		Writer.AddByte("raining", a_World->IsWeatherRain() ? 1 : 0);
		Writer.AddByte("thundering", a_World->IsWeatherStorm() ? 1 : 0);
		Writer.AddInt("GameType", (int)a_World->GetGameMode());
		Writer.AddInt("generatorVersion", 1);
		Writer.AddInt("SpawnX", (int)a_World->GetSpawnX());
		Writer.AddInt("SpawnY", (int)a_World->GetSpawnY());
		Writer.AddInt("SpawnZ", (int)a_World->GetSpawnZ());
		Writer.AddInt("version", 19133);
		Writer.AddLong("DayTime", (Int64)a_World->GetTimeOfDay());
		Writer.AddLong("Time", a_World->GetWorldAge());
		Writer.AddLong("SizeOnDisk", 0);
		Writer.AddString("generatorName", "default");
		Writer.AddString("generatorOptions", "");
		Writer.AddString("LevelName", a_World->GetName());
		Writer.EndCompound();
		Writer.Finish();
		
		#ifdef _DEBUG
		cParsedNBT TestParse(Writer.GetResult().data(), Writer.GetResult().size());
		ASSERT(TestParse.IsValid());
		#endif  // _DEBUG
		
		gzFile gz = gzopen((FILE_IO_PREFIX + fnam).c_str(), "wb");
		if (gz != nullptr)
		{
			gzwrite(gz, Writer.GetResult().data(), (unsigned)Writer.GetResult().size());
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
		LOGWARNING("Cannot serialize chunk [%d, %d] into data", a_Chunk.m_ChunkX, a_Chunk.m_ChunkZ);
		return false;
	}
	if (!SetChunkData(a_Chunk, ChunkData))
	{
		LOGWARNING("Cannot store chunk [%d, %d] data", a_Chunk.m_ChunkX, a_Chunk.m_ChunkZ);
		return false;
	}
	
	// Everything successful
	return true;
}





bool cWSSAnvil::GetChunkData(const cChunkCoords & a_Chunk, AString & a_Data)
{
	cCSLock Lock(m_CS);
	cMCAFile * File = LoadMCAFile(a_Chunk);
	if (File == nullptr)
	{
		return false;
	}
	return File->GetChunkData(a_Chunk, a_Data);
}





bool cWSSAnvil::SetChunkData(const cChunkCoords & a_Chunk, const AString & a_Data)
{
	cCSLock Lock(m_CS);
	cMCAFile * File = LoadMCAFile(a_Chunk);
	if (File == nullptr)
	{
		return false;
	}
	return File->SetChunkData(a_Chunk, a_Data);
}





cWSSAnvil::cMCAFile * cWSSAnvil::LoadMCAFile(const cChunkCoords & a_Chunk)
{
	// ASSUME m_CS is locked
	ASSERT(m_CS.IsLocked());

	const int RegionX = FAST_FLOOR_DIV(a_Chunk.m_ChunkX, 32);
	const int RegionZ = FAST_FLOOR_DIV(a_Chunk.m_ChunkZ, 32);
	ASSERT(a_Chunk.m_ChunkX - RegionX * 32 >= 0);
	ASSERT(a_Chunk.m_ChunkZ - RegionZ * 32 >= 0);
	ASSERT(a_Chunk.m_ChunkX - RegionX * 32 < 32);
	ASSERT(a_Chunk.m_ChunkZ - RegionZ * 32 < 32);
	
	// Is it already cached?
	for (cMCAFiles::iterator itr = m_Files.begin(); itr != m_Files.end(); ++itr)
	{
		if (((*itr) != nullptr) && ((*itr)->GetRegionX() == RegionX) && ((*itr)->GetRegionZ() == RegionZ))
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
	cFile::CreateFolder(FILE_IO_PREFIX + FileName);
	AppendPrintf(FileName, "/r.%d.%d.mca", RegionX, RegionZ);
	cMCAFile * f = new cMCAFile(FileName, RegionX, RegionZ);
	if (f == nullptr)
	{
		return nullptr;
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
	// Uncompress the data:
	AString Uncompressed;
	int res = InflateString(a_Data.data(), a_Data.size(), Uncompressed);
	if (res != Z_OK)
	{
		LOGWARNING("Uncompressing chunk [%d, %d] failed: %d", a_Chunk.m_ChunkX, a_Chunk.m_ChunkZ, res);
		LOAD_FAILED(a_Chunk.m_ChunkX, a_Chunk.m_ChunkZ);
		return false;
	}
	
	// Parse the NBT data:
	cParsedNBT NBT(Uncompressed.data(), Uncompressed.size());
	if (!NBT.IsValid())
	{
		// NBT Parsing failed
		LOAD_FAILED(a_Chunk.m_ChunkX, a_Chunk.m_ChunkZ);
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
		LOGWARNING("Cannot save chunk [%d, %d] to NBT", a_Chunk.m_ChunkX, a_Chunk.m_ChunkZ);
		return false;
	}
	Writer.Finish();
	
	CompressString(Writer.GetResult().data(), Writer.GetResult().size(), a_Data, m_CompressionFactor);
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
		LOAD_FAILED(a_Chunk.m_ChunkX, a_Chunk.m_ChunkZ);
		return false;
	}
	int Sections = a_NBT.FindChildByName(Level, "Sections");
	if ((Sections < 0) || (a_NBT.GetType(Sections) != TAG_List))
	{
		LOAD_FAILED(a_Chunk.m_ChunkX, a_Chunk.m_ChunkZ);
		return false;
	}
	eTagType SectionsType = a_NBT.GetChildrenType(Sections);
	if ((SectionsType != TAG_Compound) && (SectionsType != TAG_End))
	{
		LOAD_FAILED(a_Chunk.m_ChunkX, a_Chunk.m_ChunkZ);
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
	
	// Load the biomes from NBT, if present and valid. First try MCS-style, then Vanilla-style:
	cChunkDef::BiomeMap BiomeMap;
	cChunkDef::BiomeMap * Biomes = LoadBiomeMapFromNBT(&BiomeMap, a_NBT, a_NBT.FindChildByName(Level, "MCSBiomes"));
	if (Biomes == nullptr)
	{
		// MCS-style biomes not available, load vanilla-style:
		Biomes = LoadVanillaBiomeMapFromNBT(&BiomeMap, a_NBT, a_NBT.FindChildByName(Level, "Biomes"));
	}
	
	// Load the entities from NBT:
	cEntityList      Entities;
	cBlockEntityList BlockEntities;
	LoadEntitiesFromNBT     (Entities,      a_NBT, a_NBT.FindChildByName(Level, "Entities"));
	LoadBlockEntitiesFromNBT(BlockEntities, a_NBT, a_NBT.FindChildByName(Level, "TileEntities"), BlockTypes, MetaData);
	
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
					case E_BLOCK_NEW_LEAVES:
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
	
	cSetChunkDataPtr SetChunkData(new cSetChunkData(
		a_Chunk.m_ChunkX, a_Chunk.m_ChunkZ,
		BlockTypes, MetaData,
		IsLightValid ? BlockLight : nullptr,
		IsLightValid ? SkyLight : nullptr,
		nullptr, Biomes,
		Entities, BlockEntities,
		false
	));
	m_World->QueueSetChunkData(SetChunkData);
	return true;
}




void cWSSAnvil::CopyNBTData(const cParsedNBT & a_NBT, int a_Tag, const AString & a_ChildName, char * a_Destination, size_t a_Length)
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
	if (!m_World->GetChunkData(a_Chunk.m_ChunkX, a_Chunk.m_ChunkZ, Serializer))
	{
		LOGWARNING("Cannot get chunk [%d, %d] data for NBT saving", a_Chunk.m_ChunkX, a_Chunk.m_ChunkZ);
		return false;
	}
	Serializer.Finish();  // Close NBT tags
	
	// Save biomes, both MCS (IntArray) and MC-vanilla (ByteArray):
	if (Serializer.m_BiomesAreValid)
	{
		a_Writer.AddByteArray("Biomes",    (const char *)(Serializer.m_VanillaBiomes), ARRAYCOUNT(Serializer.m_VanillaBiomes));
		a_Writer.AddIntArray ("MCSBiomes", (const int *)(Serializer.m_Biomes),         ARRAYCOUNT(Serializer.m_Biomes));
	}

	// Save heightmap (Vanilla require this):
	a_Writer.AddIntArray("HeightMap", (const int *)Serializer.m_VanillaHeightMap, ARRAYCOUNT(Serializer.m_VanillaHeightMap));

	// Save blockdata:
	a_Writer.BeginList("Sections", TAG_Compound);
	size_t SliceSizeBlock  = cChunkDef::Width * cChunkDef::Width * 16;
	size_t SliceSizeNibble = SliceSizeBlock / 2;
	const char * BlockTypes    = (const char *)(Serializer.m_BlockTypes);
	const char * BlockMetas    = (const char *)(Serializer.m_BlockMetas);
	#ifdef DEBUG_SKYLIGHT
		const char * BlockLight  = (const char *)(Serializer.m_BlockSkyLight);
	#else
		const char * BlockLight  = (const char *)(Serializer.m_BlockLight);
	#endif
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

	// Save the world age to the chunk data. Required by vanilla and mcedit.
	a_Writer.AddLong("LastUpdate", m_World->GetWorldAge());
	
	// Store the flag that the chunk has all the ores, trees, dungeons etc. MCS chunks are always complete.
	a_Writer.AddByte("TerrainPopulated", 1);
	
	a_Writer.EndCompound();  // "Level"
	return true;
}





cChunkDef::BiomeMap * cWSSAnvil::LoadVanillaBiomeMapFromNBT(cChunkDef::BiomeMap * a_BiomeMap, const cParsedNBT & a_NBT, int a_TagIdx)
{
	if ((a_TagIdx < 0) || (a_NBT.GetType(a_TagIdx) != TAG_ByteArray))
	{
		return nullptr;
	}
	if (a_NBT.GetDataLength(a_TagIdx) != 16 * 16)
	{
		// The biomes stored don't match in size
		return nullptr;
	}
	const unsigned char * VanillaBiomeData = (const unsigned char *)(a_NBT.GetData(a_TagIdx));
	for (size_t i = 0; i < ARRAYCOUNT(*a_BiomeMap); i++)
	{
		if ((VanillaBiomeData)[i] == 0xff)
		{
			// Unassigned biomes
			return nullptr;
		}
		(*a_BiomeMap)[i] = (EMCSBiome)(VanillaBiomeData[i]);
	}
	return a_BiomeMap;
}





cChunkDef::BiomeMap * cWSSAnvil::LoadBiomeMapFromNBT(cChunkDef::BiomeMap * a_BiomeMap, const cParsedNBT & a_NBT, int a_TagIdx)
{
	if ((a_TagIdx < 0) || (a_NBT.GetType(a_TagIdx) != TAG_IntArray))
	{
		return nullptr;
	}
	if (a_NBT.GetDataLength(a_TagIdx) != sizeof(*a_BiomeMap))
	{
		// The biomes stored don't match in size
		return nullptr;
	}
	const char * BiomeData = (a_NBT.GetData(a_TagIdx));
	for (size_t i = 0; i < ARRAYCOUNT(*a_BiomeMap); i++)
	{
		(*a_BiomeMap)[i] = (EMCSBiome)(GetBEInt(&BiomeData[i * 4]));
		if ((*a_BiomeMap)[i] == 0xff)
		{
			// Unassigned biomes
			return nullptr;
		}
	}
	return a_BiomeMap;
}





void cWSSAnvil::LoadEntitiesFromNBT(cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx)
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
		LoadEntityFromNBT(a_Entities, a_NBT, Child, a_NBT.GetData(sID), a_NBT.GetDataLength(sID));
	}  // for Child - a_NBT[]
}





void cWSSAnvil::LoadBlockEntitiesFromNBT(cBlockEntityList & a_BlockEntities, const cParsedNBT & a_NBT, int a_TagIdx, BLOCKTYPE * a_BlockTypes, NIBBLETYPE * a_BlockMetas)
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

		// Get the BlockEntity's position
		int x, y, z;
		if (!GetBlockEntityNBTPos(a_NBT, Child, x, y, z) || (y < 0) || (y >= cChunkDef::Height))
		{
			LOGWARNING("Bad block entity, missing the coords. Will be ignored.");
			continue;
		}
		int RelX = x, RelY = y, RelZ = z, ChunkX, ChunkZ;
		cChunkDef::AbsoluteToRelative(RelX, RelY, RelZ, ChunkX, ChunkZ);

		// Load the proper BlockEntity type based on the block type:
		BLOCKTYPE BlockType = cChunkDef::GetBlock(a_BlockTypes, RelX, RelY, RelZ);
		NIBBLETYPE BlockMeta = cChunkDef::GetNibble(a_BlockMetas, RelX, RelY, RelZ);
		std::unique_ptr<cBlockEntity> be(LoadBlockEntityFromNBT(a_NBT, Child, x, y, z, BlockType, BlockMeta));
		if (be.get() == nullptr)
		{
			continue;
		}

		// Add the BlockEntity to the loaded data:
		a_BlockEntities.push_back(be.release());
	}  // for Child - tag children
}





cBlockEntity * cWSSAnvil::LoadBlockEntityFromNBT(const cParsedNBT & a_NBT, int a_Tag, int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta)
{
	ASSERT((a_BlockY >= 0) && (a_BlockY < cChunkDef::Height));

	// Load the specific BlockEntity type:
	switch (a_BlockType)
	{
		// Specific entity loaders:
		case E_BLOCK_BEACON:        return LoadBeaconFromNBT      (a_NBT, a_Tag, a_BlockX, a_BlockY, a_BlockZ);
		case E_BLOCK_CHEST:         return LoadChestFromNBT       (a_NBT, a_Tag, a_BlockX, a_BlockY, a_BlockZ, E_BLOCK_CHEST);
		case E_BLOCK_COMMAND_BLOCK: return LoadCommandBlockFromNBT(a_NBT, a_Tag, a_BlockX, a_BlockY, a_BlockZ);
		case E_BLOCK_DISPENSER:     return LoadDispenserFromNBT   (a_NBT, a_Tag, a_BlockX, a_BlockY, a_BlockZ);
		case E_BLOCK_DROPPER:       return LoadDropperFromNBT     (a_NBT, a_Tag, a_BlockX, a_BlockY, a_BlockZ);
		case E_BLOCK_FLOWER_POT:    return LoadFlowerPotFromNBT   (a_NBT, a_Tag, a_BlockX, a_BlockY, a_BlockZ);
		case E_BLOCK_FURNACE:       return LoadFurnaceFromNBT     (a_NBT, a_Tag, a_BlockX, a_BlockY, a_BlockZ, E_BLOCK_FURNACE, a_BlockMeta);
		case E_BLOCK_HEAD:          return LoadMobHeadFromNBT     (a_NBT, a_Tag, a_BlockX, a_BlockY, a_BlockZ);
		case E_BLOCK_HOPPER:        return LoadHopperFromNBT      (a_NBT, a_Tag, a_BlockX, a_BlockY, a_BlockZ);
		case E_BLOCK_JUKEBOX:       return LoadJukeboxFromNBT     (a_NBT, a_Tag, a_BlockX, a_BlockY, a_BlockZ);
		case E_BLOCK_LIT_FURNACE:   return LoadFurnaceFromNBT     (a_NBT, a_Tag, a_BlockX, a_BlockY, a_BlockZ, E_BLOCK_LIT_FURNACE, a_BlockMeta);
		case E_BLOCK_MOB_SPAWNER:   return LoadMobSpawnerFromNBT  (a_NBT, a_Tag, a_BlockX, a_BlockY, a_BlockZ);
		case E_BLOCK_NOTE_BLOCK:    return LoadNoteBlockFromNBT   (a_NBT, a_Tag, a_BlockX, a_BlockY, a_BlockZ);
		case E_BLOCK_SIGN_POST:     return LoadSignFromNBT        (a_NBT, a_Tag, a_BlockX, a_BlockY, a_BlockZ, E_BLOCK_SIGN_POST);
		case E_BLOCK_TRAPPED_CHEST: return LoadChestFromNBT       (a_NBT, a_Tag, a_BlockX, a_BlockY, a_BlockZ, E_BLOCK_TRAPPED_CHEST);
		case E_BLOCK_WALLSIGN:      return LoadSignFromNBT        (a_NBT, a_Tag, a_BlockX, a_BlockY, a_BlockZ, E_BLOCK_WALLSIGN);

		// Blocktypes that have block entities but don't load their contents from disk:
		case E_BLOCK_ENDER_CHEST:   return nullptr;
	}

	// All the other blocktypes should have no entities assigned to them. Report an error:
	// Get the "id" tag:
	int TagID = a_NBT.FindChildByName(a_Tag, "id");
	AString TypeName("<unknown>");
	if (TagID >= 0)
	{
		TypeName.assign(a_NBT.GetData(TagID), (size_t)a_NBT.GetDataLength(TagID));
	}
	LOGINFO("WorldLoader(%s): Block entity mismatch: block type %s (%d), type \"%s\", at {%d, %d, %d}; the entity will be lost.",
		m_World->GetName().c_str(),
		ItemTypeToString(a_BlockType).c_str(), a_BlockType, TypeName.c_str(),
		a_BlockX, a_BlockY, a_BlockZ
	);
	return nullptr;
}





bool cWSSAnvil::LoadItemFromNBT(cItem & a_Item, const cParsedNBT & a_NBT, int a_TagIdx)
{
	int Type = a_NBT.FindChildByName(a_TagIdx, "id");
	if (Type <= 0)
	{
		return false;
	}

	if (a_NBT.GetType(Type) == TAG_String)
	{
		if (!StringToItem(a_NBT.GetString(Type), a_Item))
		{
			// Can't resolve item type
			return false;
		}
	}
	else if (a_NBT.GetType(Type) == TAG_Short)
	{
		a_Item.m_ItemType = a_NBT.GetShort(Type);
	}
	else
	{
		return false;
	}

	if (a_Item.m_ItemType < 0)
	{
		a_Item.Empty();
		return true;
	}
	
	int Damage = a_NBT.FindChildByName(a_TagIdx, "Damage");
	if ((Damage > 0) && (a_NBT.GetType(Damage) == TAG_Short))
	{
		a_Item.m_ItemDamage = a_NBT.GetShort(Damage);
	}
	
	int Count = a_NBT.FindChildByName(a_TagIdx, "Count");
	if ((Count > 0) && (a_NBT.GetType(Count) == TAG_Byte))
	{
		a_Item.m_ItemCount = a_NBT.GetByte(Count);
	}
	
	// Find the "tag" tag, used for enchantments and other extra data
	int TagTag = a_NBT.FindChildByName(a_TagIdx, "tag");
	if (TagTag <= 0)
	{
		// No extra data
		return true;
	}

	// Load repair cost:
	int RepairCost = a_NBT.FindChildByName(TagTag, "RepairCost");
	if ((RepairCost > 0) && (a_NBT.GetType(RepairCost) == TAG_Int))
	{
		a_Item.m_RepairCost = a_NBT.GetInt(RepairCost);
	}

	// Load display name:
	int DisplayTag = a_NBT.FindChildByName(TagTag, "display");
	if (DisplayTag > 0)
	{
		int DisplayName = a_NBT.FindChildByName(DisplayTag, "Name");
		if ((DisplayName > 0) && (a_NBT.GetType(DisplayName) == TAG_String))
		{
			a_Item.m_CustomName = a_NBT.GetString(DisplayName);
		}
		int Lore = a_NBT.FindChildByName(DisplayTag, "Lore");
		if ((Lore > 0) && (a_NBT.GetType(Lore) == TAG_String))
		{
			a_Item.m_Lore = a_NBT.GetString(Lore);
		}
	}

	// Load enchantments:
	const char * EnchName = (a_Item.m_ItemType == E_ITEM_BOOK) ? "StoredEnchantments" : "ench";
	int EnchTag = a_NBT.FindChildByName(TagTag, EnchName);
	if (EnchTag > 0)
	{
		EnchantmentSerializer::ParseFromNBT(a_Item.m_Enchantments, a_NBT, EnchTag);
	}

	// Load firework data:
	int FireworksTag = a_NBT.FindChildByName(TagTag, ((a_Item.m_ItemType == E_ITEM_FIREWORK_STAR) ? "Fireworks" : "Explosion"));
	if (EnchTag > 0)
	{
		cFireworkItem::ParseFromNBT(a_Item.m_FireworkItem, a_NBT, FireworksTag, (ENUM_ITEM_ID)a_Item.m_ItemType);
	}
	
	return true;
}





void cWSSAnvil::LoadItemGridFromNBT(cItemGrid & a_ItemGrid, const cParsedNBT & a_NBT, int a_ItemsTagIdx, int a_SlotOffset)
{
	int NumSlots = a_ItemGrid.GetNumSlots();
	for (int Child = a_NBT.GetFirstChild(a_ItemsTagIdx); Child != -1; Child = a_NBT.GetNextSibling(Child))
	{
		int SlotTag = a_NBT.FindChildByName(Child, "Slot");
		if ((SlotTag < 0) || (a_NBT.GetType(SlotTag) != TAG_Byte))
		{
			continue;
		}
		int SlotNum = (int)(a_NBT.GetByte(SlotTag)) - a_SlotOffset;
		if ((SlotNum < 0) || (SlotNum >= NumSlots))
		{
			// SlotNum outside of the range
			continue;
		}
		cItem Item;
		if (LoadItemFromNBT(Item, a_NBT, Child))
		{
			a_ItemGrid.SetSlot(SlotNum, Item);
		}
	}  // for itr - ItemDefs[]
}





bool cWSSAnvil::CheckBlockEntityType(const cParsedNBT & a_NBT, int a_TagIdx, const char * a_ExpectedType)
{
	// Check if the given tag is a compound:
	if (a_NBT.GetType(a_TagIdx) != TAG_Compound)
	{
		return false;
	}

	// Get the "id" tag:
	int TagID = a_NBT.FindChildByName(a_TagIdx, "id");
	if (TagID < 0)
	{
		return false;
	}

	// Compare the value:
	if (strncmp(a_NBT.GetData(TagID), a_ExpectedType, (size_t)a_NBT.GetDataLength(TagID)) == 0)
	{
		return true;
	}
	LOGWARNING("Block entity type mismatch: exp \"%s\", got \"%s\".",
		a_ExpectedType,
		AString(a_NBT.GetData(TagID), (size_t)a_NBT.GetDataLength(TagID)).c_str()
	);
	return false;
}





cBlockEntity * cWSSAnvil::LoadBeaconFromNBT(const cParsedNBT & a_NBT, int a_TagIdx, int a_BlockX, int a_BlockY, int a_BlockZ)
{
	// Check if the data has a proper type:
	if (!CheckBlockEntityType(a_NBT, a_TagIdx, "Beacon"))
	{
		return nullptr;
	}

	std::unique_ptr<cBeaconEntity> Beacon(new cBeaconEntity(a_BlockX, a_BlockY, a_BlockZ, m_World));

	int CurrentLine = a_NBT.FindChildByName(a_TagIdx, "Levels");
	if (CurrentLine >= 0)
	{
		Beacon->SetBeaconLevel((char)a_NBT.GetInt(CurrentLine));
	}

	CurrentLine = a_NBT.FindChildByName(a_TagIdx, "Primary");
	if (CurrentLine >= 0)
	{
		Beacon->SetPrimaryEffect((cEntityEffect::eType)a_NBT.GetInt(CurrentLine));
	}

	CurrentLine = a_NBT.FindChildByName(a_TagIdx, "Secondary");
	if (CurrentLine >= 0)
	{
		Beacon->SetSecondaryEffect((cEntityEffect::eType)a_NBT.GetInt(CurrentLine));
	}

	// We are better than mojang, we load/save the beacon inventory!
	int Items = a_NBT.FindChildByName(a_TagIdx, "Items");
	if ((Items >= 0) && (a_NBT.GetType(Items) == TAG_List))
	{
		LoadItemGridFromNBT(Beacon->GetContents(), a_NBT, Items);
	}

	return Beacon.release();
}





cBlockEntity * cWSSAnvil::LoadChestFromNBT(const cParsedNBT & a_NBT, int a_TagIdx, int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE a_ChestBlockType)
{
	// Check if the data has a proper type:
	// TODO: Does vanilla use "TrappedChest" or not? MCWiki says no, but previous code says yes
	// Ref.: http://minecraft.gamepedia.com/Trapped_Chest
	//       https://github.com/mc-server/MCServer/blob/d0551e2e0a98a28f31a88d489d17b408e4a7d38d/src/WorldStorage/WSSAnvil.cpp#L637
	if (!CheckBlockEntityType(a_NBT, a_TagIdx, "Chest") && !CheckBlockEntityType(a_NBT, a_TagIdx, "TrappedChest"))
	{
		return nullptr;
	}

	int Items = a_NBT.FindChildByName(a_TagIdx, "Items");
	if ((Items < 0) || (a_NBT.GetType(Items) != TAG_List))
	{
		return nullptr;  // Make it an empty chest - the chunk loader will provide an empty cChestEntity for this
	}
	std::unique_ptr<cChestEntity> Chest(new cChestEntity(a_BlockX, a_BlockY, a_BlockZ, m_World, a_ChestBlockType));
	LoadItemGridFromNBT(Chest->GetContents(), a_NBT, Items);
	return Chest.release();
}





cBlockEntity * cWSSAnvil::LoadCommandBlockFromNBT(const cParsedNBT & a_NBT, int a_TagIdx, int a_BlockX, int a_BlockY, int a_BlockZ)
{
	// Check if the data has a proper type:
	if (!CheckBlockEntityType(a_NBT, a_TagIdx, "Control"))
	{
		return nullptr;
	}

	std::unique_ptr<cCommandBlockEntity> CmdBlock(new cCommandBlockEntity(a_BlockX, a_BlockY, a_BlockZ, m_World));

	int currentLine = a_NBT.FindChildByName(a_TagIdx, "Command");
	if (currentLine >= 0)
	{
		CmdBlock->SetCommand(a_NBT.GetString(currentLine));
	}

	currentLine = a_NBT.FindChildByName(a_TagIdx, "SuccessCount");
	if (currentLine >= 0)
	{
		CmdBlock->SetResult(a_NBT.GetInt(currentLine));
	}

	currentLine = a_NBT.FindChildByName(a_TagIdx, "LastOutput");
	if (currentLine >= 0)
	{
		CmdBlock->SetLastOutput(a_NBT.GetString(currentLine));
	}

	// TODO 2014-01-18 xdot: Figure out what TrackOutput is and parse it.

	return CmdBlock.release();
}





cBlockEntity * cWSSAnvil::LoadDispenserFromNBT(const cParsedNBT & a_NBT, int a_TagIdx, int a_BlockX, int a_BlockY, int a_BlockZ)
{
	// Check if the data has a proper type:
	if (!CheckBlockEntityType(a_NBT, a_TagIdx, "Trap"))
	{
		return nullptr;
	}

	int Items = a_NBT.FindChildByName(a_TagIdx, "Items");
	if ((Items < 0) || (a_NBT.GetType(Items) != TAG_List))
	{
		return nullptr;  // Make it an empty dispenser - the chunk loader will provide an empty cDispenserEntity for this
	}
	std::unique_ptr<cDispenserEntity> Dispenser(new cDispenserEntity(a_BlockX, a_BlockY, a_BlockZ, m_World));
	LoadItemGridFromNBT(Dispenser->GetContents(), a_NBT, Items);
	return Dispenser.release();
}





cBlockEntity * cWSSAnvil::LoadDropperFromNBT(const cParsedNBT & a_NBT, int a_TagIdx, int a_BlockX, int a_BlockY, int a_BlockZ)
{
	// Check if the data has a proper type:
	if (!CheckBlockEntityType(a_NBT, a_TagIdx, "Dropper"))
	{
		return nullptr;
	}

	int Items = a_NBT.FindChildByName(a_TagIdx, "Items");
	if ((Items < 0) || (a_NBT.GetType(Items) != TAG_List))
	{
		return nullptr;  // Make it an empty dropper - the chunk loader will provide an empty cDropperEntity for this
	}
	std::unique_ptr<cDropperEntity> Dropper(new cDropperEntity(a_BlockX, a_BlockY, a_BlockZ, m_World));
	LoadItemGridFromNBT(Dropper->GetContents(), a_NBT, Items);
	return Dropper.release();
}





cBlockEntity * cWSSAnvil::LoadFlowerPotFromNBT(const cParsedNBT & a_NBT, int a_TagIdx, int a_BlockX, int a_BlockY, int a_BlockZ)
{
	// Check if the data has a proper type:
	if (!CheckBlockEntityType(a_NBT, a_TagIdx, "FlowerPot"))
	{
		return nullptr;
	}

	std::unique_ptr<cFlowerPotEntity> FlowerPot(new cFlowerPotEntity(a_BlockX, a_BlockY, a_BlockZ, m_World));
	short ItemType = 0, ItemData = 0;

	int currentLine = a_NBT.FindChildByName(a_TagIdx, "Item");
	if (currentLine >= 0)
	{
		ItemType = (short) a_NBT.GetInt(currentLine);
	}

	currentLine = a_NBT.FindChildByName(a_TagIdx, "Data");
	if (currentLine >= 0)
	{
		ItemData = (short) a_NBT.GetInt(currentLine);
	}

	FlowerPot->SetItem(cItem(ItemType, 1, ItemData));
	return FlowerPot.release();
}





cBlockEntity * cWSSAnvil::LoadFurnaceFromNBT(const cParsedNBT & a_NBT, int a_TagIdx, int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta)
{
	// Check if the data has a proper type:
	if (!CheckBlockEntityType(a_NBT, a_TagIdx, "Furnace"))
	{
		return nullptr;
	}

	int Items = a_NBT.FindChildByName(a_TagIdx, "Items");
	if ((Items < 0) || (a_NBT.GetType(Items) != TAG_List))
	{
		return nullptr;  // Make it an empty furnace - the chunk loader will provide an empty cFurnaceEntity for this
	}
	
	std::unique_ptr<cFurnaceEntity> Furnace(new cFurnaceEntity(a_BlockX, a_BlockY, a_BlockZ, a_BlockType, a_BlockMeta, m_World));
	
	// Load slots:
	for (int Child = a_NBT.GetFirstChild(Items); Child != -1; Child = a_NBT.GetNextSibling(Child))
	{
		int Slot = a_NBT.FindChildByName(Child, "Slot");
		if ((Slot < 0) || (a_NBT.GetType(Slot) != TAG_Byte))
		{
			continue;
		}
		cItem Item;
		if (LoadItemFromNBT(Item, a_NBT, Child))
		{
			Furnace->SetSlot(a_NBT.GetByte(Slot), Item);
		}
	}  // for itr - ItemDefs[]
	
	// Load burn time:
	int BurnTime = a_NBT.FindChildByName(a_TagIdx, "BurnTime");
	if (BurnTime >= 0)
	{
		Int16 bt = a_NBT.GetShort(BurnTime);
		// Anvil doesn't store the time that the fuel can burn. We simply "reset" the current value to be the 100%
		Furnace->SetBurnTimes(bt, 0);
	}
	
	// Load cook time:
	int CookTime = a_NBT.FindChildByName(a_TagIdx, "CookTime");
	if (CookTime >= 0)
	{
		Int16 ct = a_NBT.GetShort(CookTime);
		// Anvil doesn't store the time that an item takes to cook. We simply use the default - 10 seconds (200 ticks)
		Furnace->SetCookTimes(200, ct);
	}
	
	// Restart cooking:
	Furnace->ContinueCooking();
	return Furnace.release();
}





cBlockEntity * cWSSAnvil::LoadMobSpawnerFromNBT(const cParsedNBT & a_NBT, int a_TagIdx, int a_BlockX, int a_BlockY, int a_BlockZ)
{
	// Check if the data has a proper type:
	if (!CheckBlockEntityType(a_NBT, a_TagIdx, "MobSpawner"))
	{
		return nullptr;
	}

	std::unique_ptr<cMobSpawnerEntity> MobSpawner(new cMobSpawnerEntity(a_BlockX, a_BlockY, a_BlockZ, m_World));

	// Load entity (MCServer worlds):
	int Type = a_NBT.FindChildByName(a_TagIdx, "Entity");
	if ((Type >= 0) && (a_NBT.GetType(Type) == TAG_Short))
	{
		short MonsterType = a_NBT.GetShort(Type);
		if ((MonsterType >= 50) && (MonsterType <= 120))
		{
			MobSpawner->SetEntity(static_cast<eMonsterType>(MonsterType));
		}
	}
	else
	{
		// Load entity (vanilla worlds):
		Type = a_NBT.FindChildByName(a_TagIdx, "EntityId");
		if ((Type >= 0) && (a_NBT.GetType(Type) == TAG_String))
		{
			eMonsterType MonsterType = cMonster::StringToMobType(a_NBT.GetString(Type));
			if (MonsterType != eMonsterType::mtInvalidType)
			{
				MobSpawner->SetEntity(MonsterType);
			}
		}
	}

	// Load delay:
	int Delay = a_NBT.FindChildByName(a_TagIdx, "Delay");
	if ((Delay >= 0) && (a_NBT.GetType(Delay) == TAG_Short))
	{
		MobSpawner->SetSpawnDelay(a_NBT.GetShort(Delay));
	}

	return MobSpawner.release();
}





cBlockEntity * cWSSAnvil::LoadHopperFromNBT(const cParsedNBT & a_NBT, int a_TagIdx, int a_BlockX, int a_BlockY, int a_BlockZ)
{
	// Check if the data has a proper type:
	if (!CheckBlockEntityType(a_NBT, a_TagIdx, "Hopper"))
	{
		return nullptr;
	}

	int Items = a_NBT.FindChildByName(a_TagIdx, "Items");
	if ((Items < 0) || (a_NBT.GetType(Items) != TAG_List))
	{
		return nullptr;  // Make it an empty hopper - the chunk loader will provide an empty cHopperEntity for this
	}
	std::unique_ptr<cHopperEntity> Hopper(new cHopperEntity(a_BlockX, a_BlockY, a_BlockZ, m_World));
	LoadItemGridFromNBT(Hopper->GetContents(), a_NBT, Items);
	return Hopper.release();
}





cBlockEntity * cWSSAnvil::LoadJukeboxFromNBT(const cParsedNBT & a_NBT, int a_TagIdx, int a_BlockX, int a_BlockY, int a_BlockZ)
{
	// Check if the data has a proper type:
	if (!CheckBlockEntityType(a_NBT, a_TagIdx, "RecordPlayer"))
	{
		return nullptr;
	}

	std::unique_ptr<cJukeboxEntity> Jukebox(new cJukeboxEntity(a_BlockX, a_BlockY, a_BlockZ, m_World));
	int Record = a_NBT.FindChildByName(a_TagIdx, "Record");
	if (Record >= 0)
	{
		Jukebox->SetRecord(a_NBT.GetInt(Record));
	}
	return Jukebox.release();
}





cBlockEntity * cWSSAnvil::LoadMobHeadFromNBT(const cParsedNBT & a_NBT, int a_TagIdx, int a_BlockX, int a_BlockY, int a_BlockZ)
{
	// Check if the data has a proper type:
	if (!CheckBlockEntityType(a_NBT, a_TagIdx, "Skull"))
	{
		return nullptr;
	}

	std::unique_ptr<cMobHeadEntity> MobHead(new cMobHeadEntity(a_BlockX, a_BlockY, a_BlockZ, m_World));

	int currentLine = a_NBT.FindChildByName(a_TagIdx, "SkullType");
	if (currentLine >= 0)
	{
		MobHead->SetType(static_cast<eMobHeadType>(a_NBT.GetByte(currentLine)));
	}

	currentLine = a_NBT.FindChildByName(a_TagIdx, "Rot");
	if (currentLine >= 0)
	{
		MobHead->SetRotation(static_cast<eMobHeadRotation>(a_NBT.GetByte(currentLine)));
	}

	currentLine = a_NBT.FindChildByName(a_TagIdx, "ExtraType");
	if (currentLine >= 0)
	{
		MobHead->SetOwner(a_NBT.GetString(currentLine));
	}

	return MobHead.release();
}





cBlockEntity * cWSSAnvil::LoadNoteBlockFromNBT(const cParsedNBT & a_NBT, int a_TagIdx, int a_BlockX, int a_BlockY, int a_BlockZ)
{
	// Check if the data has a proper type:
	if (!CheckBlockEntityType(a_NBT, a_TagIdx, "Music"))
	{
		return nullptr;
	}

	std::unique_ptr<cNoteEntity> NoteBlock(new cNoteEntity(a_BlockX, a_BlockY, a_BlockZ, m_World));
	int note = a_NBT.FindChildByName(a_TagIdx, "note");
	if (note >= 0)
	{
		NoteBlock->SetPitch(a_NBT.GetByte(note));
	}
	return NoteBlock.release();
}





cBlockEntity * cWSSAnvil::LoadSignFromNBT(const cParsedNBT & a_NBT, int a_TagIdx, int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE a_BlockType)
{
	// Check if the data has a proper type:
	if (!CheckBlockEntityType(a_NBT, a_TagIdx, "Sign"))
	{
		return nullptr;
	}

	std::unique_ptr<cSignEntity> Sign(new cSignEntity(a_BlockType, a_BlockX, a_BlockY, a_BlockZ, m_World));

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

	return Sign.release();
}





void cWSSAnvil::LoadEntityFromNBT(cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_EntityTagIdx, const char * a_IDTag, size_t a_IDTagLength)
{
	if (strncmp(a_IDTag, "Boat", a_IDTagLength) == 0)
	{
		LoadBoatFromNBT(a_Entities, a_NBT, a_EntityTagIdx);
	}
	else if (strncmp(a_IDTag, "EnderCrystal", a_IDTagLength) == 0)
	{
		LoadEnderCrystalFromNBT(a_Entities, a_NBT, a_EntityTagIdx);
	}
	else if (strncmp(a_IDTag, "FallingBlock", a_IDTagLength) == 0)
	{
		LoadFallingBlockFromNBT(a_Entities, a_NBT, a_EntityTagIdx);
	}
	else if (strncmp(a_IDTag, "Minecart", a_IDTagLength) == 0)
	{
		// It is a minecart, old style, find out the type:
		int TypeTag = a_NBT.FindChildByName(a_EntityTagIdx, "Type");
		if ((TypeTag < 0) || (a_NBT.GetType(TypeTag) != TAG_Int))
		{
			return;
		}
		switch (a_NBT.GetInt(TypeTag))
		{
			case 0: LoadMinecartRFromNBT(a_Entities, a_NBT, a_EntityTagIdx); break;  // Rideable minecart
			case 1: LoadMinecartCFromNBT(a_Entities, a_NBT, a_EntityTagIdx); break;  // Minecart with chest
			case 2: LoadMinecartFFromNBT(a_Entities, a_NBT, a_EntityTagIdx); break;  // Minecart with furnace
			case 3: LoadMinecartTFromNBT(a_Entities, a_NBT, a_EntityTagIdx); break;  // Minecart with TNT
			case 4: LoadMinecartHFromNBT(a_Entities, a_NBT, a_EntityTagIdx); break;  // Minecart with Hopper
		}
	}
	else if (strncmp(a_IDTag, "MinecartRideable", a_IDTagLength) == 0)
	{
		LoadMinecartRFromNBT(a_Entities, a_NBT, a_EntityTagIdx);
	}
	else if (strncmp(a_IDTag, "MinecartChest", a_IDTagLength) == 0)
	{
		LoadMinecartCFromNBT(a_Entities, a_NBT, a_EntityTagIdx);
	}
	else if (strncmp(a_IDTag, "MinecartFurnace", a_IDTagLength) == 0)
	{
		LoadMinecartFFromNBT(a_Entities, a_NBT, a_EntityTagIdx);
	}
	else if (strncmp(a_IDTag, "MinecartTNT", a_IDTagLength) == 0)
	{
		LoadMinecartTFromNBT(a_Entities, a_NBT, a_EntityTagIdx);
	}
	else if (strncmp(a_IDTag, "MinecartHopper", a_IDTagLength) == 0)
	{
		LoadMinecartHFromNBT(a_Entities, a_NBT, a_EntityTagIdx);
	}
	else if (strncmp(a_IDTag, "Item", a_IDTagLength) == 0)
	{
		LoadPickupFromNBT(a_Entities, a_NBT, a_EntityTagIdx);
	}
	else if (strncmp(a_IDTag, "PrimedTnt", a_IDTagLength) == 0)
	{
		LoadTNTFromNBT(a_Entities, a_NBT, a_EntityTagIdx);
	}
	else if (strncmp(a_IDTag, "XPOrb", a_IDTagLength) == 0)
	{
		LoadExpOrbFromNBT(a_Entities, a_NBT, a_EntityTagIdx);
	}
	else if (strncmp(a_IDTag, "ItemFrame", a_IDTagLength) == 0)
	{
		LoadItemFrameFromNBT(a_Entities, a_NBT, a_EntityTagIdx);
	}
	else if (strncmp(a_IDTag, "Arrow", a_IDTagLength) == 0)
	{
		LoadArrowFromNBT(a_Entities, a_NBT, a_EntityTagIdx);
	}
	else if (strncmp(a_IDTag, "SplashPotion", a_IDTagLength) == 0)
	{
		LoadSplashPotionFromNBT(a_Entities, a_NBT, a_EntityTagIdx);
	}
	else if (strncmp(a_IDTag, "Snowball", a_IDTagLength) == 0)
	{
		LoadSnowballFromNBT(a_Entities, a_NBT, a_EntityTagIdx);
	}
	else if (strncmp(a_IDTag, "Egg", a_IDTagLength) == 0)
	{
		LoadEggFromNBT(a_Entities, a_NBT, a_EntityTagIdx);
	}
	else if (strncmp(a_IDTag, "Fireball", a_IDTagLength) == 0)
	{
		LoadFireballFromNBT(a_Entities, a_NBT, a_EntityTagIdx);
	}
	else if (strncmp(a_IDTag, "SmallFireball", a_IDTagLength) == 0)
	{
		LoadFireChargeFromNBT(a_Entities, a_NBT, a_EntityTagIdx);
	}
	else if (strncmp(a_IDTag, "ThrownEnderpearl", a_IDTagLength) == 0)
	{
		LoadThrownEnderpearlFromNBT(a_Entities, a_NBT, a_EntityTagIdx);
	}
	else if (strncmp(a_IDTag, "Bat", a_IDTagLength) == 0)
	{
		LoadBatFromNBT(a_Entities, a_NBT, a_EntityTagIdx);
	}
	else if (strncmp(a_IDTag, "Blaze", a_IDTagLength) == 0)
	{
		LoadBlazeFromNBT(a_Entities, a_NBT, a_EntityTagIdx);
	}
	else if (strncmp(a_IDTag, "CaveSpider", a_IDTagLength) == 0)
	{
		LoadCaveSpiderFromNBT(a_Entities, a_NBT, a_EntityTagIdx);
	}
	else if (strncmp(a_IDTag, "Chicken", a_IDTagLength) == 0)
	{
		LoadChickenFromNBT(a_Entities, a_NBT, a_EntityTagIdx);
	}
	else if (strncmp(a_IDTag, "Cow", a_IDTagLength) == 0)
	{
		LoadCowFromNBT(a_Entities, a_NBT, a_EntityTagIdx);
	}
	else if (strncmp(a_IDTag, "Creeper", a_IDTagLength) == 0)
	{
		LoadCreeperFromNBT(a_Entities, a_NBT, a_EntityTagIdx);
	}
	else if (strncmp(a_IDTag, "EnderDragon", a_IDTagLength) == 0)
	{
		LoadEnderDragonFromNBT(a_Entities, a_NBT, a_EntityTagIdx);
	}
	else if (strncmp(a_IDTag, "Enderman", a_IDTagLength) == 0)
	{
		LoadEndermanFromNBT(a_Entities, a_NBT, a_EntityTagIdx);
	}
	else if (strncmp(a_IDTag, "Ghast", a_IDTagLength) == 0)
	{
		LoadGhastFromNBT(a_Entities, a_NBT, a_EntityTagIdx);
	}
	else if (strncmp(a_IDTag, "Giant", a_IDTagLength) == 0)
	{
		LoadGiantFromNBT(a_Entities, a_NBT, a_EntityTagIdx);
	}
	else if (strncmp(a_IDTag, "Guardian", a_IDTagLength) == 0)
	{
		LoadGuardianFromNBT(a_Entities, a_NBT, a_EntityTagIdx);
	}
	else if (strncmp(a_IDTag, "Horse", a_IDTagLength) == 0)
	{
		LoadHorseFromNBT(a_Entities, a_NBT, a_EntityTagIdx);
	}
	else if (strncmp(a_IDTag, "Villager", a_IDTagLength) == 0)
	{
		LoadVillagerFromNBT(a_Entities, a_NBT, a_EntityTagIdx);
	}
	else if (strncmp(a_IDTag, "VillagerGolem", a_IDTagLength) == 0)
	{
		LoadIronGolemFromNBT(a_Entities, a_NBT, a_EntityTagIdx);
	}
	else if (strncmp(a_IDTag, "LavaSlime", a_IDTagLength) == 0)
	{
		LoadMagmaCubeFromNBT(a_Entities, a_NBT, a_EntityTagIdx);
	}
	else if (strncmp(a_IDTag, "MushroomCow", a_IDTagLength) == 0)
	{
		LoadMooshroomFromNBT(a_Entities, a_NBT, a_EntityTagIdx);
	}
	else if (strncmp(a_IDTag, "Ozelot", a_IDTagLength) == 0)
	{
		LoadOcelotFromNBT(a_Entities, a_NBT, a_EntityTagIdx);
	}
	else if (strncmp(a_IDTag, "Pig", a_IDTagLength) == 0)
	{
		LoadPigFromNBT(a_Entities, a_NBT, a_EntityTagIdx);
	}
	else if (strncmp(a_IDTag, "Rabbit", a_IDTagLength) == 0)
	{
		LoadRabbitFromNBT(a_Entities, a_NBT, a_EntityTagIdx);
	}
	else if (strncmp(a_IDTag, "Sheep", a_IDTagLength) == 0)
	{
		LoadSheepFromNBT(a_Entities, a_NBT, a_EntityTagIdx);
	}
	else if (strncmp(a_IDTag, "Silverfish", a_IDTagLength) == 0)
	{
		LoadSilverfishFromNBT(a_Entities, a_NBT, a_EntityTagIdx);
	}
	else if (strncmp(a_IDTag, "Skeleton", a_IDTagLength) == 0)
	{
		LoadSkeletonFromNBT(a_Entities, a_NBT, a_EntityTagIdx);
	}
	else if (strncmp(a_IDTag, "Slime", a_IDTagLength) == 0)
	{
		LoadSlimeFromNBT(a_Entities, a_NBT, a_EntityTagIdx);
	}
	else if (strncmp(a_IDTag, "SnowMan", a_IDTagLength) == 0)
	{
		LoadSnowGolemFromNBT(a_Entities, a_NBT, a_EntityTagIdx);
	}
	else if (strncmp(a_IDTag, "Spider", a_IDTagLength) == 0)
	{
		LoadSpiderFromNBT(a_Entities, a_NBT, a_EntityTagIdx);
	}
	else if (strncmp(a_IDTag, "Squid", a_IDTagLength) == 0)
	{
		LoadSquidFromNBT(a_Entities, a_NBT, a_EntityTagIdx);
	}
	else if (strncmp(a_IDTag, "Witch", a_IDTagLength) == 0)
	{
		LoadWitchFromNBT(a_Entities, a_NBT, a_EntityTagIdx);
	}
	else if (strncmp(a_IDTag, "WitherBoss", a_IDTagLength) == 0)
	{
		LoadWitherFromNBT(a_Entities, a_NBT, a_EntityTagIdx);
	}
	else if (strncmp(a_IDTag, "Wolf", a_IDTagLength) == 0)
	{
		LoadWolfFromNBT(a_Entities, a_NBT, a_EntityTagIdx);
	}
	else if (strncmp(a_IDTag, "Zombie", a_IDTagLength) == 0)
	{
		LoadZombieFromNBT(a_Entities, a_NBT, a_EntityTagIdx);
	}
	else if (strncmp(a_IDTag, "PigZombie", a_IDTagLength) == 0)
	{
		LoadPigZombieFromNBT(a_Entities, a_NBT, a_EntityTagIdx);
	}
	// TODO: other entities
}





void cWSSAnvil::LoadBoatFromNBT(cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx)
{
	std::unique_ptr<cBoat> Boat(new cBoat(0, 0, 0));
	if (!LoadEntityBaseFromNBT(*Boat.get(), a_NBT, a_TagIdx))
	{
		return;
	}
	a_Entities.push_back(Boat.release());
}





void cWSSAnvil::LoadEnderCrystalFromNBT(cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx)
{
	std::unique_ptr<cEnderCrystal> EnderCrystal(new cEnderCrystal(0, 0, 0));
	if (!LoadEntityBaseFromNBT(*EnderCrystal.get(), a_NBT, a_TagIdx))
	{
		return;
	}
	a_Entities.push_back(EnderCrystal.release());
}





void cWSSAnvil::LoadFallingBlockFromNBT(cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx)
{
	int TypeIdx = a_NBT.FindChildByName(a_TagIdx, "TileID");
	int MetaIdx = a_NBT.FindChildByName(a_TagIdx, "Data");

	if ((TypeIdx < 0) || (MetaIdx < 0))
	{
		return;
	}

	int Type = a_NBT.GetInt(TypeIdx);
	NIBBLETYPE Meta = (NIBBLETYPE)a_NBT.GetByte(MetaIdx);

	std::unique_ptr<cFallingBlock> FallingBlock(new cFallingBlock(Vector3i(0, 0, 0), Type, Meta));
	if (!LoadEntityBaseFromNBT(*FallingBlock.get(), a_NBT, a_TagIdx))
	{
		return;
	}
	a_Entities.push_back(FallingBlock.release());
}





void cWSSAnvil::LoadMinecartRFromNBT(cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx)
{
	std::unique_ptr<cRideableMinecart> Minecart(new cRideableMinecart(0, 0, 0, cItem(), 1));  // TODO: Load the block and the height
	if (!LoadEntityBaseFromNBT(*Minecart.get(), a_NBT, a_TagIdx))
	{
		return;
	}
	a_Entities.push_back(Minecart.release());
}





void cWSSAnvil::LoadMinecartCFromNBT(cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx)
{
	int Items = a_NBT.FindChildByName(a_TagIdx, "Items");
	if ((Items < 0) || (a_NBT.GetType(Items) != TAG_List))
	{
		return;  // Make it an empty chest - the chunk loader will provide an empty cChestEntity for this
	}
	std::unique_ptr<cMinecartWithChest> Minecart(new cMinecartWithChest(0, 0, 0));
	if (!LoadEntityBaseFromNBT(*Minecart.get(), a_NBT, a_TagIdx))
	{
		return;
	}
	for (int Child = a_NBT.GetFirstChild(Items); Child != -1; Child = a_NBT.GetNextSibling(Child))
	{
		int Slot = a_NBT.FindChildByName(Child, "Slot");
		if ((Slot < 0) || (a_NBT.GetType(Slot) != TAG_Byte))
		{
			continue;
		}
		cItem Item;
		if (LoadItemFromNBT(Item, a_NBT, Child))
		{
			Minecart->SetSlot(a_NBT.GetByte(Slot), Item);
		}
	}  // for itr - ItemDefs[]
	a_Entities.push_back(Minecart.release());
}





void cWSSAnvil::LoadMinecartFFromNBT(cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx)
{
	std::unique_ptr<cMinecartWithFurnace> Minecart(new cMinecartWithFurnace(0, 0, 0));
	if (!LoadEntityBaseFromNBT(*Minecart.get(), a_NBT, a_TagIdx))
	{
		return;
	}
	
	// TODO: Load the Push and Fuel tags
	
	a_Entities.push_back(Minecart.release());
}





void cWSSAnvil::LoadMinecartTFromNBT(cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx)
{
	std::unique_ptr<cMinecartWithTNT> Minecart(new cMinecartWithTNT(0, 0, 0));
	if (!LoadEntityBaseFromNBT(*Minecart.get(), a_NBT, a_TagIdx))
	{
		return;
	}
	
	// TODO: Everything to do with TNT carts

	a_Entities.push_back(Minecart.release());
}





void cWSSAnvil::LoadMinecartHFromNBT(cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx)
{
	std::unique_ptr<cMinecartWithHopper> Minecart(new cMinecartWithHopper(0, 0, 0));
	if (!LoadEntityBaseFromNBT(*Minecart.get(), a_NBT, a_TagIdx))
	{
		return;
	}
	
	// TODO: Everything to do with hopper carts

	a_Entities.push_back(Minecart.release());
}





void cWSSAnvil::LoadPickupFromNBT(cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx)
{
	// Load item:
	int ItemTag = a_NBT.FindChildByName(a_TagIdx, "Item");
	if ((ItemTag < 0) || (a_NBT.GetType(ItemTag) != TAG_Compound))
	{
		return;
	}
	cItem Item;
	if (!LoadItemFromNBT(Item, a_NBT, ItemTag))
	{
		return;
	}
	
	std::unique_ptr<cPickup> Pickup(new cPickup(0, 0, 0, Item, false));  // Pickup delay doesn't matter, just say false
	if (!LoadEntityBaseFromNBT(*Pickup.get(), a_NBT, a_TagIdx))
	{
		return;
	}
	
	// Load age:
	int Age = a_NBT.FindChildByName(a_TagIdx, "Age");
	if (Age > 0)
	{
		Pickup->SetAge(a_NBT.GetShort(Age));
	}

	a_Entities.push_back(Pickup.release());
}





void cWSSAnvil::LoadTNTFromNBT(cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx)
{
	std::unique_ptr<cTNTEntity> TNT(new cTNTEntity(0.0, 0.0, 0.0, 0));
	if (!LoadEntityBaseFromNBT(*TNT.get(), a_NBT, a_TagIdx))
	{
		return;
	}
	
	// Load Fuse Ticks:
	int FuseTicks = a_NBT.FindChildByName(a_TagIdx, "Fuse");
	if (FuseTicks > 0)
	{
		TNT->SetFuseTicks((int) a_NBT.GetByte(FuseTicks));
	}
	
	a_Entities.push_back(TNT.release());
}





void cWSSAnvil::LoadExpOrbFromNBT(cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx)
{
	std::unique_ptr<cExpOrb> ExpOrb(new cExpOrb(0.0, 0.0, 0.0, 0));
	if (!LoadEntityBaseFromNBT(*ExpOrb.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	// Load Age:
	int Age = a_NBT.FindChildByName(a_TagIdx, "Age");
	if (Age > 0)
	{
		ExpOrb->SetAge(a_NBT.GetShort(Age));
	}

	// Load Reward (Value):
	int Reward = a_NBT.FindChildByName(a_TagIdx, "Value");
	if (Reward > 0)
	{
		ExpOrb->SetReward(a_NBT.GetShort(Reward));
	}

	a_Entities.push_back(ExpOrb.release());
}





void cWSSAnvil::LoadHangingFromNBT(cHangingEntity & a_Hanging, const cParsedNBT & a_NBT, int a_TagIdx)
{
	// "Facing" tag is the prime source of the Facing; if not available, translate from older "Direction" or "Dir"
	int Facing = a_NBT.FindChildByName(a_TagIdx, "Facing");
	if (Facing > 0)
	{
		Facing = (int)a_NBT.GetByte(Facing);
		if ((Facing >= 2) && (Facing <= 5))
		{
			a_Hanging.SetFacing(static_cast<eBlockFace>(Facing));
		}
	}
	else
	{
		Facing = a_NBT.FindChildByName(a_TagIdx, "Direction");
		if (Facing > 0)
		{
			switch ((int)a_NBT.GetByte(Facing))
			{
				case 0: a_Hanging.SetFacing(BLOCK_FACE_ZM); break;
				case 1: a_Hanging.SetFacing(BLOCK_FACE_XM); break;
				case 2: a_Hanging.SetFacing(BLOCK_FACE_ZP); break;
				case 3: a_Hanging.SetFacing(BLOCK_FACE_XP); break;
			}
		}
		else
		{
			Facing = a_NBT.FindChildByName(a_TagIdx, "Dir");  // Has values 0 and 2 swapped
			if (Facing > 0)
			{
				switch ((int)a_NBT.GetByte(Facing))
				{
					case 0: a_Hanging.SetFacing(BLOCK_FACE_ZP); break;
					case 1: a_Hanging.SetFacing(BLOCK_FACE_XM); break;
					case 2: a_Hanging.SetFacing(BLOCK_FACE_ZM); break;
					case 3: a_Hanging.SetFacing(BLOCK_FACE_XP); break;
				}
			}
		}
	}

	int TileX = a_NBT.FindChildByName(a_TagIdx, "TileX");
	int TileY = a_NBT.FindChildByName(a_TagIdx, "TileY");
	int TileZ = a_NBT.FindChildByName(a_TagIdx, "TileZ");
	if ((TileX > 0) && (TileY > 0) && (TileZ > 0))
	{
		a_Hanging.SetPosition(
			(double)a_NBT.GetInt(TileX),
			(double)a_NBT.GetInt(TileY),
			(double)a_NBT.GetInt(TileZ)
		);
	}
}





void cWSSAnvil::LoadItemFrameFromNBT(cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx)
{
	// Load item:
	int ItemTag = a_NBT.FindChildByName(a_TagIdx, "Item");
	if ((ItemTag < 0) || (a_NBT.GetType(ItemTag) != TAG_Compound))
	{
		return;
	}
	cItem Item;
	if (!LoadItemFromNBT(Item, a_NBT, ItemTag))
	{
		return;
	}
	
	std::unique_ptr<cItemFrame> ItemFrame(new cItemFrame(BLOCK_FACE_NONE, 0.0, 0.0, 0.0));
	if (!LoadEntityBaseFromNBT(*ItemFrame.get(), a_NBT, a_TagIdx))
	{
		return;
	}
	ItemFrame->SetItem(Item);

	LoadHangingFromNBT(*ItemFrame.get(), a_NBT, a_TagIdx);
	
	// Load Rotation:
	int Rotation = a_NBT.FindChildByName(a_TagIdx, "ItemRotation");
	if (Rotation > 0)
	{
		ItemFrame->SetItemRotation((Byte)a_NBT.GetByte(Rotation));
	}
	
	a_Entities.push_back(ItemFrame.release());
}





void cWSSAnvil::LoadArrowFromNBT(cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx)
{
	std::unique_ptr<cArrowEntity> Arrow(new cArrowEntity(nullptr, 0, 0, 0, Vector3d(0, 0, 0)));
	if (!LoadProjectileBaseFromNBT(*Arrow.get(), a_NBT, a_TagIdx))
	{
		return;
	}
	
	// Load pickup state:
	int PickupIdx = a_NBT.FindChildByName(a_TagIdx, "pickup");
	if ((PickupIdx > 0) && (a_NBT.GetType(PickupIdx) == TAG_Byte))
	{
		Arrow->SetPickupState((cArrowEntity::ePickupState)a_NBT.GetByte(PickupIdx));
	}
	else
	{
		// Try the older "player" tag:
		int PlayerIdx = a_NBT.FindChildByName(a_TagIdx, "player");
		if ((PlayerIdx > 0) && (a_NBT.GetType(PlayerIdx) == TAG_Byte))
		{
			Arrow->SetPickupState((a_NBT.GetByte(PlayerIdx) == 0) ? cArrowEntity::psNoPickup : cArrowEntity::psInSurvivalOrCreative);
		}
	}
	
	// Load damage:
	int DamageIdx = a_NBT.FindChildByName(a_TagIdx, "damage");
	if ((DamageIdx > 0) && (a_NBT.GetType(DamageIdx) == TAG_Double))
	{
		Arrow->SetDamageCoeff(a_NBT.GetDouble(DamageIdx));
	}
	
	// Load block hit:
	int InBlockXIdx = a_NBT.FindChildByName(a_TagIdx, "xTile");
	int InBlockYIdx = a_NBT.FindChildByName(a_TagIdx, "yTile");
	int InBlockZIdx = a_NBT.FindChildByName(a_TagIdx, "zTile");
	if ((InBlockXIdx > 0) && (InBlockYIdx > 0) && (InBlockZIdx > 0))
	{
		eTagType typeX = a_NBT.GetType(InBlockXIdx);
		if ((typeX == a_NBT.GetType(InBlockYIdx)) &&  (typeX == a_NBT.GetType(InBlockZIdx)))
		{
			switch (typeX)
			{
				case TAG_Int:
				{
					// Old MCS code used this, keep reading it for compatibility reasons:
					Arrow->SetBlockHit(Vector3i(a_NBT.GetInt(InBlockXIdx), a_NBT.GetInt(InBlockYIdx), a_NBT.GetInt(InBlockZIdx)));
					break;
				}
				case TAG_Short:
				{
					// Vanilla uses this
					Arrow->SetBlockHit(Vector3i((int)a_NBT.GetShort(InBlockXIdx), (int)a_NBT.GetShort(InBlockYIdx), (int)a_NBT.GetShort(InBlockZIdx)));
					break;
				}
				default:
				{
					// No hit block, the arrow is still flying?
					break;
				}
			}
		}
	}
	
	// Store the new arrow in the entities list:
	a_Entities.push_back(Arrow.release());
}




void cWSSAnvil::LoadSplashPotionFromNBT(cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx)
{
	std::unique_ptr<cSplashPotionEntity> SplashPotion(new cSplashPotionEntity(nullptr, 0, 0, 0, Vector3d(0, 0, 0), cItem()));
	if (!LoadProjectileBaseFromNBT(*SplashPotion.get(), a_NBT, a_TagIdx))
	{
		return;
	}
	
	int EffectDuration         = a_NBT.FindChildByName(a_TagIdx, "EffectDuration");
	int EffectIntensity        = a_NBT.FindChildByName(a_TagIdx, "EffectIntensity");
	int EffectDistanceModifier = a_NBT.FindChildByName(a_TagIdx, "EffectDistanceModifier");
	
	SplashPotion->SetEntityEffectType((cEntityEffect::eType) a_NBT.FindChildByName(a_TagIdx, "EffectType"));
	SplashPotion->SetEntityEffect(cEntityEffect(EffectDuration, EffectIntensity, EffectDistanceModifier));
	SplashPotion->SetPotionColor(a_NBT.FindChildByName(a_TagIdx, "PotionName"));
	
	// Store the new splash potion in the entities list:
	a_Entities.push_back(SplashPotion.release());
}





void cWSSAnvil::LoadSnowballFromNBT(cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx)
{
	std::unique_ptr<cThrownSnowballEntity> Snowball(new cThrownSnowballEntity(nullptr, 0, 0, 0, Vector3d(0, 0, 0)));
	if (!LoadProjectileBaseFromNBT(*Snowball.get(), a_NBT, a_TagIdx))
	{
		return;
	}
	
	// Store the new snowball in the entities list:
	a_Entities.push_back(Snowball.release());
}





void cWSSAnvil::LoadEggFromNBT(cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx)
{
	std::unique_ptr<cThrownEggEntity> Egg(new cThrownEggEntity(nullptr, 0, 0, 0, Vector3d(0, 0, 0)));
	if (!LoadProjectileBaseFromNBT(*Egg.get(), a_NBT, a_TagIdx))
	{
		return;
	}
	
	// Store the new egg in the entities list:
	a_Entities.push_back(Egg.release());
}





void cWSSAnvil::LoadFireballFromNBT(cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx)
{
	std::unique_ptr<cGhastFireballEntity> Fireball(new cGhastFireballEntity(nullptr, 0, 0, 0, Vector3d(0, 0, 0)));
	if (!LoadProjectileBaseFromNBT(*Fireball.get(), a_NBT, a_TagIdx))
	{
		return;
	}
	
	// Store the new fireball in the entities list:
	a_Entities.push_back(Fireball.release());
}





void cWSSAnvil::LoadFireChargeFromNBT(cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx)
{
	std::unique_ptr<cFireChargeEntity> FireCharge(new cFireChargeEntity(nullptr, 0, 0, 0, Vector3d(0, 0, 0)));
	if (!LoadProjectileBaseFromNBT(*FireCharge.get(), a_NBT, a_TagIdx))
	{
		return;
	}
	
	// Store the new FireCharge in the entities list:
	a_Entities.push_back(FireCharge.release());
}





void cWSSAnvil::LoadThrownEnderpearlFromNBT(cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx)
{
	std::unique_ptr<cThrownEnderPearlEntity> Enderpearl(new cThrownEnderPearlEntity(nullptr, 0, 0, 0, Vector3d(0, 0, 0)));
	if (!LoadProjectileBaseFromNBT(*Enderpearl.get(), a_NBT, a_TagIdx))
	{
		return;
	}
	
	// Store the new enderpearl in the entities list:
	a_Entities.push_back(Enderpearl.release());
}





void cWSSAnvil::LoadBatFromNBT(cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx)
{
	std::unique_ptr<cBat> Monster(new cBat());
	if (!LoadEntityBaseFromNBT(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}
	
	if (!LoadMonsterBaseFromNBT(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	a_Entities.push_back(Monster.release());
}





void cWSSAnvil::LoadBlazeFromNBT(cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx)
{
	std::unique_ptr<cBlaze> Monster(new cBlaze());
	if (!LoadEntityBaseFromNBT(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}
	
	if (!LoadMonsterBaseFromNBT(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	a_Entities.push_back(Monster.release());
}





void cWSSAnvil::LoadCaveSpiderFromNBT(cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx)
{
	std::unique_ptr<cCaveSpider> Monster(new cCaveSpider());
	if (!LoadEntityBaseFromNBT(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}
	
	if (!LoadMonsterBaseFromNBT(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	a_Entities.push_back(Monster.release());
}





void cWSSAnvil::LoadChickenFromNBT(cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx)
{
	std::unique_ptr<cChicken> Monster(new cChicken());
	if (!LoadEntityBaseFromNBT(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}
	
	if (!LoadMonsterBaseFromNBT(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	a_Entities.push_back(Monster.release());
}





void cWSSAnvil::LoadCowFromNBT(cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx)
{
	std::unique_ptr<cCow> Monster(new cCow());
	if (!LoadEntityBaseFromNBT(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}
	
	if (!LoadMonsterBaseFromNBT(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	a_Entities.push_back(Monster.release());
}





void cWSSAnvil::LoadCreeperFromNBT(cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx)
{
	std::unique_ptr<cCreeper> Monster(new cCreeper());
	if (!LoadEntityBaseFromNBT(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}
	
	if (!LoadMonsterBaseFromNBT(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	a_Entities.push_back(Monster.release());
}





void cWSSAnvil::LoadEnderDragonFromNBT(cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx)
{
	std::unique_ptr<cEnderDragon> Monster(new cEnderDragon());
	if (!LoadEntityBaseFromNBT(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}
	
	if (!LoadMonsterBaseFromNBT(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	a_Entities.push_back(Monster.release());
}





void cWSSAnvil::LoadEndermanFromNBT(cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx)
{
	std::unique_ptr<cEnderman> Monster(new cEnderman());
	if (!LoadEntityBaseFromNBT(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	if (!LoadMonsterBaseFromNBT(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	a_Entities.push_back(Monster.release());
}





void cWSSAnvil::LoadGhastFromNBT(cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx)
{
	std::unique_ptr<cGhast> Monster(new cGhast());
	if (!LoadEntityBaseFromNBT(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	if (!LoadMonsterBaseFromNBT(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	a_Entities.push_back(Monster.release());
}





void cWSSAnvil::LoadGiantFromNBT(cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx)
{
	std::unique_ptr<cGiant> Monster(new cGiant());
	if (!LoadEntityBaseFromNBT(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}
	
	if (!LoadMonsterBaseFromNBT(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	a_Entities.push_back(Monster.release());
}





void cWSSAnvil::LoadGuardianFromNBT(cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx)
{
	std::unique_ptr<cGuardian> Monster(new cGuardian());
	if (!LoadEntityBaseFromNBT(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}
	
	if (!LoadMonsterBaseFromNBT(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	a_Entities.push_back(Monster.release());
}





void cWSSAnvil::LoadHorseFromNBT(cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx)
{
	int TypeIdx  = a_NBT.FindChildByName(a_TagIdx, "Type");
	int ColorIdx = a_NBT.FindChildByName(a_TagIdx, "Color");
	int StyleIdx = a_NBT.FindChildByName(a_TagIdx, "Style");
	if ((TypeIdx < 0) || (ColorIdx < 0) || (StyleIdx < 0))
	{
		return;
	}

	int Type = a_NBT.GetInt(TypeIdx);
	int Color = a_NBT.GetInt(ColorIdx);
	int Style = a_NBT.GetInt(StyleIdx);

	std::unique_ptr<cHorse> Monster(new cHorse(Type, Color, Style, 1));

	if (!LoadEntityBaseFromNBT(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}
	
	if (!LoadMonsterBaseFromNBT(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	a_Entities.push_back(Monster.release());
}





void cWSSAnvil::LoadIronGolemFromNBT(cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx)
{
	std::unique_ptr<cIronGolem> Monster(new cIronGolem());
	if (!LoadEntityBaseFromNBT(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}
	
	if (!LoadMonsterBaseFromNBT(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	a_Entities.push_back(Monster.release());
}





void cWSSAnvil::LoadMagmaCubeFromNBT(cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx)
{
	int SizeIdx = a_NBT.FindChildByName(a_TagIdx, "Size");

	if (SizeIdx < 0)
	{
		return;
	}

	int Size = a_NBT.GetInt(SizeIdx);

	std::unique_ptr<cMagmaCube> Monster(new cMagmaCube(Size));
	if (!LoadEntityBaseFromNBT(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}
	
	if (!LoadMonsterBaseFromNBT(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	a_Entities.push_back(Monster.release());
}





void cWSSAnvil::LoadMooshroomFromNBT(cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx)
{
	std::unique_ptr<cMooshroom> Monster(new cMooshroom());
	if (!LoadEntityBaseFromNBT(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}
	
	if (!LoadMonsterBaseFromNBT(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	a_Entities.push_back(Monster.release());
}





void cWSSAnvil::LoadOcelotFromNBT(cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx)
{
	std::unique_ptr<cOcelot> Monster(new cOcelot());
	if (!LoadEntityBaseFromNBT(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}
	
	if (!LoadMonsterBaseFromNBT(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	a_Entities.push_back(Monster.release());
}





void cWSSAnvil::LoadPigFromNBT(cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx)
{
	std::unique_ptr<cPig> Monster(new cPig());
	if (!LoadEntityBaseFromNBT(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}
	
	if (!LoadMonsterBaseFromNBT(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	a_Entities.push_back(Monster.release());
}





void cWSSAnvil::LoadRabbitFromNBT(cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx)
{
	std::unique_ptr<cRabbit> Monster(new cRabbit());
	if (!LoadEntityBaseFromNBT(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}
	
	if (!LoadMonsterBaseFromNBT(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	a_Entities.push_back(Monster.release());
}





void cWSSAnvil::LoadSheepFromNBT(cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx)
{
	int ColorIdx = a_NBT.FindChildByName(a_TagIdx, "Color");
	int Color = -1;
	if (ColorIdx > 0)
	{
		Color = (int)a_NBT.GetByte(ColorIdx);
	}

	std::unique_ptr<cSheep> Monster(new cSheep(Color));
	if (!LoadEntityBaseFromNBT(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}
	
	if (!LoadMonsterBaseFromNBT(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	int ShearedIdx = a_NBT.FindChildByName(a_TagIdx, "Sheared");
	if (ShearedIdx > 0)
	{
		Monster->SetSheared(a_NBT.GetByte(ShearedIdx) != 0);
	}

	a_Entities.push_back(Monster.release());
}





void cWSSAnvil::LoadSilverfishFromNBT(cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx)
{
	std::unique_ptr<cSilverfish> Monster(new cSilverfish());
	if (!LoadEntityBaseFromNBT(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}
	
	if (!LoadMonsterBaseFromNBT(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	a_Entities.push_back(Monster.release());
}





void cWSSAnvil::LoadSkeletonFromNBT(cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx)
{
	int TypeIdx = a_NBT.FindChildByName(a_TagIdx, "SkeletonType");
	if (TypeIdx < 0)
	{
		return;
	}

	bool Type = ((a_NBT.GetByte(TypeIdx) == 1) ? true : false);

	std::unique_ptr<cSkeleton> Monster(new cSkeleton(Type));
	if (!LoadEntityBaseFromNBT(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}
	
	if (!LoadMonsterBaseFromNBT(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	a_Entities.push_back(Monster.release());
}





void cWSSAnvil::LoadSlimeFromNBT(cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx)
{
	int SizeIdx = a_NBT.FindChildByName(a_TagIdx, "Size");

	if (SizeIdx < 0)
	{
		return;
	}

	int Size = a_NBT.GetInt(SizeIdx);

	std::unique_ptr<cSlime> Monster(new cSlime(Size));
	if (!LoadEntityBaseFromNBT(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}
	
	if (!LoadMonsterBaseFromNBT(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	a_Entities.push_back(Monster.release());
}





void cWSSAnvil::LoadSnowGolemFromNBT(cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx)
{
	std::unique_ptr<cSnowGolem> Monster(new cSnowGolem());
	if (!LoadEntityBaseFromNBT(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}
	
	if (!LoadMonsterBaseFromNBT(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	a_Entities.push_back(Monster.release());
}





void cWSSAnvil::LoadSpiderFromNBT(cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx)
{
	std::unique_ptr<cSpider> Monster(new cSpider());
	if (!LoadEntityBaseFromNBT(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}
	
	if (!LoadMonsterBaseFromNBT(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	a_Entities.push_back(Monster.release());
}





void cWSSAnvil::LoadSquidFromNBT(cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx)
{
	std::unique_ptr<cSquid> Monster(new cSquid());
	if (!LoadEntityBaseFromNBT(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}
	
	if (!LoadMonsterBaseFromNBT(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	a_Entities.push_back(Monster.release());
}





void cWSSAnvil::LoadVillagerFromNBT(cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx)
{
	int TypeIdx = a_NBT.FindChildByName(a_TagIdx, "Profession");
	if (TypeIdx < 0)
	{
		return;
	}

	int Type = a_NBT.GetInt(TypeIdx);

	std::unique_ptr<cVillager> Monster(new cVillager(cVillager::eVillagerType(Type)));
	if (!LoadEntityBaseFromNBT(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}
	
	if (!LoadMonsterBaseFromNBT(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	a_Entities.push_back(Monster.release());
}





void cWSSAnvil::LoadWitchFromNBT(cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx)
{
	std::unique_ptr<cWitch> Monster(new cWitch());
	if (!LoadEntityBaseFromNBT(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}
	
	if (!LoadMonsterBaseFromNBT(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	a_Entities.push_back(Monster.release());
}





void cWSSAnvil::LoadWitherFromNBT(cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx)
{
	std::unique_ptr<cWither> Monster(new cWither());
	if (!LoadEntityBaseFromNBT(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}
	
	if (!LoadMonsterBaseFromNBT(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	int CurrLine = a_NBT.FindChildByName(a_TagIdx, "Invul");
	if (CurrLine > 0)
	{
		Monster->SetWitherInvulnerableTicks(a_NBT.GetInt(CurrLine));
	}

	a_Entities.push_back(Monster.release());
}





void cWSSAnvil::LoadWolfFromNBT(cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx)
{
	std::unique_ptr<cWolf> Monster(new cWolf());
	if (!LoadEntityBaseFromNBT(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}
	if (!LoadMonsterBaseFromNBT(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}
	
	LoadWolfOwner(*Monster.get(), a_NBT, a_TagIdx);

	int SittingIdx = a_NBT.FindChildByName(a_TagIdx, "Sitting");
	if ((SittingIdx > 0) && (a_NBT.GetType(SittingIdx) == TAG_Byte))
	{
		bool Sitting = ((a_NBT.GetByte(SittingIdx) == 1) ? true : false);
		Monster->SetIsSitting(Sitting);
	}
	int AngryIdx = a_NBT.FindChildByName(a_TagIdx, "Angry");
	if ((AngryIdx > 0) && (a_NBT.GetType(AngryIdx) == TAG_Byte))
	{
		bool Angry = ((a_NBT.GetByte(AngryIdx) == 1) ? true : false);
		Monster->SetIsAngry(Angry);
	}
	int CollarColorIdx = a_NBT.FindChildByName(a_TagIdx, "CollarColor");
	if (CollarColorIdx > 0)
	{
		switch (a_NBT.GetType(CollarColorIdx))
		{
			case TAG_Byte:
			{
				// Vanilla uses this
				unsigned char CollarColor = a_NBT.GetByte(CollarColorIdx);
				Monster->SetCollarColor(CollarColor);
				break;
			}
			case TAG_Int:
			{
				// Old MCS code used this, keep reading it for compatibility reasons:
				Monster->SetCollarColor(a_NBT.GetInt(CollarColorIdx));
				break;
			}
		}
	}
	a_Entities.push_back(Monster.release());
}





void cWSSAnvil::LoadZombieFromNBT(cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx)
{
	int IsVillagerIdx = a_NBT.FindChildByName(a_TagIdx, "IsVillager");
	if (IsVillagerIdx < 0)
	{
		return;
	}

	bool IsVillagerZombie = ((a_NBT.GetByte(IsVillagerIdx) == 1) ? true : false);

	std::unique_ptr<cZombie> Monster(new cZombie(IsVillagerZombie));
	if (!LoadEntityBaseFromNBT(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}
	
	if (!LoadMonsterBaseFromNBT(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	a_Entities.push_back(Monster.release());
}





void cWSSAnvil::LoadPigZombieFromNBT(cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx)
{
	std::unique_ptr<cZombiePigman> Monster(new cZombiePigman());
	if (!LoadEntityBaseFromNBT(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	if (!LoadMonsterBaseFromNBT(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	a_Entities.push_back(Monster.release());
}





void cWSSAnvil::LoadWolfOwner(cWolf & a_Wolf, const cParsedNBT & a_NBT, int a_TagIdx)
{
	// Load the owner information. OwnerUUID or Owner may be specified, possibly both:
	AString OwnerUUID, OwnerName;
	int OwnerUUIDIdx = a_NBT.FindChildByName(a_TagIdx, "OwnerUUID");
	if (OwnerUUIDIdx > 0)
	{
		OwnerUUID = a_NBT.GetString(OwnerUUIDIdx);
	}
	int OwnerIdx = a_NBT.FindChildByName(a_TagIdx, "Owner");
	if (OwnerIdx > 0)
	{
		OwnerName = a_NBT.GetString(OwnerIdx);
	}
	if (OwnerName.empty() && OwnerUUID.empty())
	{
		// There is no owner, bail out:
		return;
	}
	
	// Convert name to UUID, if needed:
	if (OwnerUUID.empty())
	{
		// This wolf has only playername stored (pre-1.7.6), look up the UUID
		// The lookup is blocking, but we're running in a separate thread, so it's ok
		OwnerUUID = cRoot::Get()->GetMojangAPI().GetUUIDFromPlayerName(OwnerName);
		if (OwnerUUID.empty())
		{
			// Not a known player, un-tame the wolf by bailing out
			return;
		}
	}
	else
	{
		// Normalize the UUID:
		OwnerUUID = cMojangAPI::MakeUUIDShort(OwnerUUID);
	}
	
	// Convert UUID to name, if needed:
	if (OwnerName.empty())
	{
		// The lookup is blocking, but we're running in a separate thread, so it's ok
		OwnerName = cRoot::Get()->GetMojangAPI().GetPlayerNameFromUUID(OwnerUUID);
		if (OwnerName.empty())
		{
			// Not a known player, un-tame the wolf by bailing out
			return;
		}
	}
	
	a_Wolf.SetOwner(OwnerName, OwnerUUID);
	a_Wolf.SetIsTame(true);
}





bool cWSSAnvil::LoadEntityBaseFromNBT(cEntity & a_Entity, const cParsedNBT & a_NBT, int a_TagIdx)
{
	double Pos[3];
	if (!LoadDoublesListFromNBT(Pos, 3, a_NBT, a_NBT.FindChildByName(a_TagIdx, "Pos")))
	{
		return false;
	}
	a_Entity.SetPosition(Pos[0], Pos[1], Pos[2]);
	
	double Speed[3];
	if (!LoadDoublesListFromNBT(Speed, 3, a_NBT, a_NBT.FindChildByName(a_TagIdx, "Motion")))
	{
		// Provide default speed:
		Speed[0] = 0;
		Speed[1] = 0;
		Speed[2] = 0;
	}
	a_Entity.SetSpeed(Speed[0], Speed[1], Speed[2]);
	
	double Rotation[3];
	if (!LoadDoublesListFromNBT(Rotation, 2, a_NBT, a_NBT.FindChildByName(a_TagIdx, "Rotation")))
	{
		// Provide default rotation:
		Rotation[0] = 0;
		Rotation[1] = 0;
	}
	a_Entity.SetYaw(Rotation[0]);
	a_Entity.SetRoll(Rotation[1]);

	// Load health:
	int Health = a_NBT.FindChildByName(a_TagIdx, "Health");
	a_Entity.SetHealth(Health > 0 ? a_NBT.GetShort(Health) : a_Entity.GetMaxHealth());
	
	return true;
}





bool cWSSAnvil::LoadMonsterBaseFromNBT(cMonster & a_Monster, const cParsedNBT & a_NBT, int a_TagIdx)
{
	float DropChance[5];
	if (!LoadFloatsListFromNBT(DropChance, 5, a_NBT, a_NBT.FindChildByName(a_TagIdx, "DropChances")))
	{
		return false;
	}
	a_Monster.SetDropChanceWeapon(DropChance[0]);
	a_Monster.SetDropChanceHelmet(DropChance[1]);
	a_Monster.SetDropChanceChestplate(DropChance[2]);
	a_Monster.SetDropChanceLeggings(DropChance[3]);
	a_Monster.SetDropChanceBoots(DropChance[4]);

	int LootTag = a_NBT.FindChildByName(a_TagIdx, "CanPickUpLoot");
	if (LootTag > 0)
	{
		bool CanPickUpLoot = (a_NBT.GetByte(LootTag) == 1);
		a_Monster.SetCanPickUpLoot(CanPickUpLoot);
	}

	int CustomNameTag = a_NBT.FindChildByName(a_TagIdx, "CustomName");
	if ((CustomNameTag > 0) && (a_NBT.GetType(CustomNameTag) == TAG_String))
	{
		a_Monster.SetCustomName(a_NBT.GetString(CustomNameTag));
	}

	int CustomNameVisibleTag = a_NBT.FindChildByName(a_TagIdx, "CustomNameVisible");
	if ((CustomNameVisibleTag > 0) && (a_NBT.GetType(CustomNameVisibleTag) == TAG_Byte))
	{
		bool CustomNameVisible = (a_NBT.GetByte(CustomNameVisibleTag) == 1);
		a_Monster.SetCustomNameAlwaysVisible(CustomNameVisible);
	}

	return true;
}





bool cWSSAnvil::LoadProjectileBaseFromNBT(cProjectileEntity & a_Entity, const cParsedNBT & a_NBT, int a_TagIdx)
{
	if (!LoadEntityBaseFromNBT(a_Entity, a_NBT, a_TagIdx))
	{
		return false;
	}
	
	bool IsInGround = false;
	int InGroundIdx = a_NBT.FindChildByName(a_TagIdx, "inGround");
	if (InGroundIdx > 0)
	{
		IsInGround = (a_NBT.GetByte(InGroundIdx) != 0);
	}
	a_Entity.SetIsInGround(IsInGround);
	
	return true;
}





bool cWSSAnvil::LoadDoublesListFromNBT(double * a_Doubles, int a_NumDoubles, const cParsedNBT & a_NBT, int a_TagIdx)
{
	if ((a_TagIdx < 0) || (a_NBT.GetType(a_TagIdx) != TAG_List) || (a_NBT.GetChildrenType(a_TagIdx) != TAG_Double))
	{
		return false;
	}
	int idx = 0;
	for (int Tag = a_NBT.GetFirstChild(a_TagIdx); (Tag > 0) && (idx < a_NumDoubles); Tag = a_NBT.GetNextSibling(Tag), ++idx)
	{
		a_Doubles[idx] = a_NBT.GetDouble(Tag);
	}  // for Tag - PosTag[]
	return (idx == a_NumDoubles);  // Did we read enough doubles?
}





bool cWSSAnvil::LoadFloatsListFromNBT(float * a_Floats, int a_NumFloats, const cParsedNBT & a_NBT, int a_TagIdx)
{
	if ((a_TagIdx < 0) || (a_NBT.GetType(a_TagIdx) != TAG_List) || (a_NBT.GetChildrenType(a_TagIdx) != TAG_Float))
	{
		return false;
	}
	int idx = 0;
	for (int Tag = a_NBT.GetFirstChild(a_TagIdx); (Tag > 0) && (idx < a_NumFloats); Tag = a_NBT.GetNextSibling(Tag), ++idx)
	{
		a_Floats[idx] = a_NBT.GetFloat(Tag);
	}  // for Tag - PosTag[]
	return (idx == a_NumFloats);  // Did we read enough doubles?
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
	a_X = Clamp(a_NBT.GetInt(x), -40000000, 40000000);  // World is limited to 30M blocks in XZ, we clamp to 40M
	a_Y = Clamp(a_NBT.GetInt(y), -10000,    10000);     // Y is limited to 0 .. 255, we clamp to 10K
	a_Z = Clamp(a_NBT.GetInt(z), -40000000, 40000000);
	return true;
}





////////////////////////////////////////////////////////////////////////////////
// cWSSAnvil::cMCAFile:

cWSSAnvil::cMCAFile::cMCAFile(const AString & a_FileName, int a_RegionX, int a_RegionZ) :
	m_RegionX(a_RegionX),
	m_RegionZ(a_RegionZ),
	m_FileName(a_FileName)
{
}





bool cWSSAnvil::cMCAFile::OpenFile(bool a_IsForReading)
{
	bool writeOutNeeded = false;
	
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
		// Try writing a nullptr header for chunk offsets:
		memset(m_Header, 0, sizeof(m_Header));
		writeOutNeeded = true;
	}

	// Load the TimeStamps:
	if (m_File.Read(m_TimeStamps, sizeof(m_TimeStamps)) != sizeof(m_TimeStamps))
	{
		// Cannot read the time stamps - perhaps the file has just been created?
		// Try writing a nullptr header for timestamps:
		memset(m_TimeStamps, 0, sizeof(m_TimeStamps));
		writeOutNeeded = true;
	}
	
	if (writeOutNeeded)
	{
		m_File.Seek(0);
		if (
			(m_File.Write(m_Header, sizeof(m_Header)) != sizeof(m_Header)) ||           // Write chunk offsets
			(m_File.Write(m_TimeStamps, sizeof(m_TimeStamps)) != sizeof(m_TimeStamps))  // Write chunk timestamps
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
	if (ChunkOffset < 2)
	{
		return false;
	}
	
	m_File.Seek((int)ChunkOffset * 4096);
	
	int ChunkSize = 0;
	if (m_File.Read(&ChunkSize, 4) != 4)
	{
		LOAD_FAILED(a_Chunk.m_ChunkX, a_Chunk.m_ChunkZ);
		return false;
	}
	ChunkSize = ntohl((u_long)ChunkSize);
	char CompressionType = 0;
	if (m_File.Read(&CompressionType, 1) != 1)
	{
		LOAD_FAILED(a_Chunk.m_ChunkX, a_Chunk.m_ChunkZ);
		return false;
	}
	if (CompressionType != 2)
	{
		// Chunk is in an unknown compression
		LOAD_FAILED(a_Chunk.m_ChunkX, a_Chunk.m_ChunkZ);
		return false;
	}
	ChunkSize--;
	
	// HACK: This depends on the internal knowledge that AString's data() function returns the internal buffer directly
	a_Data.assign(ChunkSize, '\0');
	if (m_File.Read((void *)a_Data.data(), ChunkSize) == ChunkSize)
	{
		return true;
	}
	LOAD_FAILED(a_Chunk.m_ChunkX, a_Chunk.m_ChunkZ);
	return false;
}





bool cWSSAnvil::cMCAFile::SetChunkData(const cChunkCoords & a_Chunk, const AString & a_Data)
{
	if (!OpenFile(false))
	{
		LOGWARNING("Cannot save chunk [%d, %d], opening file \"%s\" failed", a_Chunk.m_ChunkX, a_Chunk.m_ChunkZ, GetFileName().c_str());
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
	u_long ChunkSize = htonl((u_long)a_Data.size() + 1);
	if (m_File.Write(&ChunkSize, 4) != 4)
	{
		LOGWARNING("Cannot save chunk [%d, %d], writing(1) data to file \"%s\" failed", a_Chunk.m_ChunkX, a_Chunk.m_ChunkZ, GetFileName().c_str());
		return false;
	}
	char CompressionType = 2;
	if (m_File.Write(&CompressionType, 1) != 1)
	{
		LOGWARNING("Cannot save chunk [%d, %d], writing(2) data to file \"%s\" failed", a_Chunk.m_ChunkX, a_Chunk.m_ChunkZ, GetFileName().c_str());
		return false;
	}
	if (m_File.Write(a_Data.data(), a_Data.size()) != (int)(a_Data.size()))
	{
		LOGWARNING("Cannot save chunk [%d, %d], writing(3) data to file \"%s\" failed", a_Chunk.m_ChunkX, a_Chunk.m_ChunkZ, GetFileName().c_str());
		return false;
	}
	
	// Add padding to 4K boundary:
	size_t BytesWritten = a_Data.size() + MCA_CHUNK_HEADER_LENGTH;
	if (BytesWritten % 4096 != 0)
	{
		static const char Padding[4095] = {0};
		m_File.Write(Padding, 4096 - (BytesWritten % 4096));
	}
	
	// Store the header:
	ChunkSize = ((u_long)a_Data.size() + MCA_CHUNK_HEADER_LENGTH + 4095) / 4096;  // Round data size *up* to nearest 4KB sector, make it a sector number
	if (ChunkSize > 255)
	{
		LOGWARNING("Cannot save chunk [%d, %d], the data is too large (%u KiB, maximum is 1024 KiB). Remove some entities and retry.",
			a_Chunk.m_ChunkX, a_Chunk.m_ChunkZ, (unsigned)(ChunkSize * 4)
		);
		return false;
	}
	
	// Store the header info in the table
	m_Header[LocalX + 32 * LocalZ] = htonl((ChunkSector << 8) | ChunkSize);

	// Set the modification time
	m_TimeStamps[LocalX + 32 * LocalZ] =  htonl(static_cast<u_long>(time(nullptr)));

	if (m_File.Seek(0) < 0)
	{
		LOGWARNING("Cannot save chunk [%d, %d], seeking in file \"%s\" failed", a_Chunk.m_ChunkX, a_Chunk.m_ChunkZ, GetFileName().c_str());
		return false;
	}
	if (m_File.Write(m_Header, sizeof(m_Header)) != sizeof(m_Header))
	{
		LOGWARNING("Cannot save chunk [%d, %d], writing header to file \"%s\" failed", a_Chunk.m_ChunkX, a_Chunk.m_ChunkZ, GetFileName().c_str());
		return false;
	}
	if (m_File.Write(m_TimeStamps, sizeof(m_TimeStamps)) != sizeof(m_TimeStamps))
	{
		LOGWARNING("Cannot save chunk [%d, %d], writing timestamps to file \"%s\" failed", a_Chunk.m_ChunkX, a_Chunk.m_ChunkZ, GetFileName().c_str());
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
	for (size_t i = 0; i < ARRAYCOUNT(m_Header); i++)
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




