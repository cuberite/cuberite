
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
#include "../BlockEntities/BrewingstandEntity.h"
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
#include "../Entities/Painting.h"

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






////////////////////////////////////////////////////////////////////////////////
// cWSSAnvil:

cWSSAnvil::cWSSAnvil(cWorld * a_World, int a_CompressionFactor) :
	super(a_World),
	m_CompressionFactor(a_CompressionFactor)
{
	// Create a level.dat file for mapping tools, if it doesn't already exist:
	AString fnam;
	Printf(fnam, "%s%clevel.dat", a_World->GetName().c_str(), cFile::PathSeparator);
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
		Writer.AddInt("GameType", static_cast<int>(a_World->GetGameMode()));
		Writer.AddInt("generatorVersion", 1);
		Writer.AddInt("SpawnX", FloorC(a_World->GetSpawnX()));
		Writer.AddInt("SpawnY", FloorC(a_World->GetSpawnY()));
		Writer.AddInt("SpawnZ", FloorC(a_World->GetSpawnZ()));
		Writer.AddInt("version", 19133);
		Writer.AddLong("DayTime", a_World->GetTimeOfDay());
		Writer.AddLong("Time", a_World->GetWorldAge());
		Writer.AddLong("SizeOnDisk", 0);
		Writer.AddString("generatorName", "default");
		Writer.AddString("generatorOptions", "");
		Writer.AddString("LevelName", a_World->GetName());
		Writer.EndCompound();
		Writer.Finish();

		gzFile gz = gzopen((FILE_IO_PREFIX + fnam).c_str(), "wb");
		if (gz != nullptr)
		{
			gzwrite(gz, Writer.GetResult().data(), static_cast<unsigned>(Writer.GetResult().size()));
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





void cWSSAnvil::ChunkLoadFailed(int a_ChunkX, int a_ChunkZ, const AString & a_Reason, const AString & a_ChunkDataToSave)
{
	// Construct the filename for offloading:
	AString OffloadFileName;
	Printf(OffloadFileName, "%s%cregion%cbadchunks", m_World->GetName().c_str(), cFile::PathSeparator, cFile::PathSeparator);
	cFile::CreateFolder(FILE_IO_PREFIX + OffloadFileName);
	auto t = time(nullptr);
	struct tm stm;
	#ifdef _MSC_VER
		localtime_s(&stm, &t);
	#else
		localtime_r(&t, &stm);
	#endif
	AppendPrintf(OffloadFileName, "%cch.%d.%d.%d-%02d-%02d-%02d-%02d-%02d.dat",
		cFile::PathSeparator, a_ChunkX, a_ChunkZ,
		stm.tm_year + 1900, stm.tm_mon + 1, stm.tm_mday, stm.tm_hour, stm.tm_min, stm.tm_sec
	);

	// Log the warning to console:
	const int RegionX = FAST_FLOOR_DIV(a_ChunkX, 32);
	const int RegionZ = FAST_FLOOR_DIV(a_ChunkZ, 32);
	AString Info = Printf("Loading chunk [%d, %d] for world %s from file r.%d.%d.mca failed: %s. Offloading old chunk data to file %s and regenerating chunk.",
		a_ChunkX, a_ChunkZ, m_World->GetName().c_str(), RegionX, RegionZ, a_Reason.c_str(), OffloadFileName.c_str()
	);
	LOGWARNING("%s", Info.c_str());

	// Write the data:
	cFile f;
	if (!f.Open(OffloadFileName, cFile::fmWrite))
	{
		LOGWARNING("Cannot open file %s for writing! Old chunk data is lost.", OffloadFileName.c_str());
		return;
	}
	f.Write(a_ChunkDataToSave.data(), a_ChunkDataToSave.size());
	f.Close();

	// Write a description file:
	if (!f.Open(OffloadFileName + ".info", cFile::fmWrite))
	{
		LOGWARNING("Cannot open file %s.info for writing! The information about the failed chunk will not be written.", OffloadFileName.c_str());
		return;
	}
	f.Write(Info.c_str(), Info.size());
	f.Close();
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
	Printf(FileName, "%s%cregion", m_World->GetName().c_str(), cFile::PathSeparator);
	cFile::CreateFolder(FILE_IO_PREFIX + FileName);
	AppendPrintf(FileName, "/r.%d.%d.mca", RegionX, RegionZ);
	cMCAFile * f = new cMCAFile(*this, FileName, RegionX, RegionZ);
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
		ChunkLoadFailed(a_Chunk.m_ChunkX, a_Chunk.m_ChunkZ, "InflateString() failed", a_Data);
		return false;
	}

	// Parse the NBT data:
	cParsedNBT NBT(Uncompressed.data(), Uncompressed.size());
	if (!NBT.IsValid())
	{
		// NBT Parsing failed
		ChunkLoadFailed(a_Chunk.m_ChunkX, a_Chunk.m_ChunkZ, "NBT parsing failed", a_Data);
		return false;
	}

	// Load the data from NBT:
	return LoadChunkFromNBT(a_Chunk, NBT, a_Data);
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





bool cWSSAnvil::LoadChunkFromNBT(const cChunkCoords & a_Chunk, const cParsedNBT & a_NBT, const AString & a_RawChunkData)
{
	// The data arrays, in MCA-native y / z / x ordering (will be reordered for the final chunk data)
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
		ChunkLoadFailed(a_Chunk.m_ChunkX, a_Chunk.m_ChunkZ, "Missing NBT tag: Level", a_RawChunkData);
		return false;
	}
	int Sections = a_NBT.FindChildByName(Level, "Sections");
	if ((Sections < 0) || (a_NBT.GetType(Sections) != TAG_List))
	{
		ChunkLoadFailed(a_Chunk.m_ChunkX, a_Chunk.m_ChunkZ, "Missing NBT tag: Sections", a_RawChunkData);
		return false;
	}
	eTagType SectionsType = a_NBT.GetChildrenType(Sections);
	if ((SectionsType != TAG_Compound) && (SectionsType != TAG_End))
	{
		ChunkLoadFailed(a_Chunk.m_ChunkX, a_Chunk.m_ChunkZ, "NBT tag has wrong type: Sections", a_RawChunkData);
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
		CopyNBTData(a_NBT, Child, "Blocks",     reinterpret_cast<char *>(&(BlockTypes[y * 4096])), 4096);
		CopyNBTData(a_NBT, Child, "Data",       reinterpret_cast<char *>(&(MetaData[y   * 2048])), 2048);
		CopyNBTData(a_NBT, Child, "SkyLight",   reinterpret_cast<char *>(&(SkyLight[y   * 2048])), 2048);
		CopyNBTData(a_NBT, Child, "BlockLight", reinterpret_cast<char *>(&(BlockLight[y * 2048])), 2048);
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
	cBlockEntities   BlockEntities;
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
		std::move(Entities), std::move(BlockEntities),
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
		a_Writer.AddByteArray("Biomes",    reinterpret_cast<const char *>(Serializer.m_VanillaBiomes), ARRAYCOUNT(Serializer.m_VanillaBiomes));
		a_Writer.AddIntArray ("MCSBiomes", reinterpret_cast<const int *>(Serializer.m_Biomes),         ARRAYCOUNT(Serializer.m_Biomes));
	}

	// Save heightmap (Vanilla require this):
	a_Writer.AddIntArray("HeightMap", reinterpret_cast<const int *>(Serializer.m_VanillaHeightMap), ARRAYCOUNT(Serializer.m_VanillaHeightMap));

	// Save blockdata:
	a_Writer.BeginList("Sections", TAG_Compound);
	size_t SliceSizeBlock  = cChunkDef::Width * cChunkDef::Width * 16;
	size_t SliceSizeNibble = SliceSizeBlock / 2;
	const char * BlockTypes    = reinterpret_cast<const char *>(Serializer.m_BlockTypes);
	const char * BlockMetas    = reinterpret_cast<const char *>(Serializer.m_BlockMetas);
	#ifdef DEBUG_SKYLIGHT
		const char * BlockLight  = reinterpret_cast<const char *>(Serializer.m_BlockSkyLight);
	#else
		const char * BlockLight  = reinterpret_cast<const char *>(Serializer.m_BlockLight);
	#endif
	const char * BlockSkyLight = reinterpret_cast<const char *>(Serializer.m_BlockSkyLight);
	for (int Y = 0; Y < 16; Y++)
	{
		a_Writer.BeginCompound("");
		a_Writer.AddByteArray("Blocks",     BlockTypes    + static_cast<unsigned int>(Y) * SliceSizeBlock,  SliceSizeBlock);
		a_Writer.AddByteArray("Data",       BlockMetas    + static_cast<unsigned int>(Y) * SliceSizeNibble, SliceSizeNibble);
		a_Writer.AddByteArray("SkyLight",   BlockSkyLight + static_cast<unsigned int>(Y) * SliceSizeNibble, SliceSizeNibble);
		a_Writer.AddByteArray("BlockLight", BlockLight    + static_cast<unsigned int>(Y) * SliceSizeNibble, SliceSizeNibble);
		a_Writer.AddByte("Y", static_cast<unsigned char>(Y));
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
	const unsigned char * VanillaBiomeData = reinterpret_cast<const unsigned char *>(a_NBT.GetData(a_TagIdx));
	for (size_t i = 0; i < ARRAYCOUNT(*a_BiomeMap); i++)
	{
		if ((VanillaBiomeData)[i] == 0xff)
		{
			// Unassigned biomes
			return nullptr;
		}
		(*a_BiomeMap)[i] = static_cast<EMCSBiome>(VanillaBiomeData[i]);
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
		(*a_BiomeMap)[i] = static_cast<EMCSBiome>(GetBEInt(&BiomeData[i * 4]));
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





void cWSSAnvil::LoadBlockEntitiesFromNBT(cBlockEntities & a_BlockEntities, const cParsedNBT & a_NBT, int a_TagIdx, BLOCKTYPE * a_BlockTypes, NIBBLETYPE * a_BlockMetas)
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
		auto Idx = cChunkDef::MakeIndex(be->GetRelX(), be->GetPosY(), be->GetRelZ());
		a_BlockEntities.insert({ Idx, be.get() });
		// Release after inserting in case it throws.
		be.release();
	}  // for Child - tag children
}





cBlockEntity * cWSSAnvil::LoadBlockEntityFromNBT(const cParsedNBT & a_NBT, int a_Tag, int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta)
{
	ASSERT((a_BlockY >= 0) && (a_BlockY < cChunkDef::Height));

	// Load the specific BlockEntity type:
	switch (a_BlockType)
	{
		// Specific entity loaders:
		case E_BLOCK_BEACON:        return LoadBeaconFromNBT      (a_NBT, a_Tag, a_BlockType, a_BlockMeta, a_BlockX, a_BlockY, a_BlockZ);
		case E_BLOCK_BREWING_STAND: return LoadBrewingstandFromNBT(a_NBT, a_Tag, a_BlockType, a_BlockMeta, a_BlockX, a_BlockY, a_BlockZ);
		case E_BLOCK_CHEST:         return LoadChestFromNBT       (a_NBT, a_Tag, a_BlockType, a_BlockMeta, a_BlockX, a_BlockY, a_BlockZ);
		case E_BLOCK_COMMAND_BLOCK: return LoadCommandBlockFromNBT(a_NBT, a_Tag, a_BlockType, a_BlockMeta, a_BlockX, a_BlockY, a_BlockZ);
		case E_BLOCK_DISPENSER:     return LoadDispenserFromNBT   (a_NBT, a_Tag, a_BlockType, a_BlockMeta, a_BlockX, a_BlockY, a_BlockZ);
		case E_BLOCK_DROPPER:       return LoadDropperFromNBT     (a_NBT, a_Tag, a_BlockType, a_BlockMeta, a_BlockX, a_BlockY, a_BlockZ);
		case E_BLOCK_FLOWER_POT:    return LoadFlowerPotFromNBT   (a_NBT, a_Tag, a_BlockType, a_BlockMeta, a_BlockX, a_BlockY, a_BlockZ);
		case E_BLOCK_FURNACE:       return LoadFurnaceFromNBT     (a_NBT, a_Tag, a_BlockType, a_BlockMeta, a_BlockX, a_BlockY, a_BlockZ);
		case E_BLOCK_HEAD:          return LoadMobHeadFromNBT     (a_NBT, a_Tag, a_BlockType, a_BlockMeta, a_BlockX, a_BlockY, a_BlockZ);
		case E_BLOCK_HOPPER:        return LoadHopperFromNBT      (a_NBT, a_Tag, a_BlockType, a_BlockMeta, a_BlockX, a_BlockY, a_BlockZ);
		case E_BLOCK_JUKEBOX:       return LoadJukeboxFromNBT     (a_NBT, a_Tag, a_BlockType, a_BlockMeta, a_BlockX, a_BlockY, a_BlockZ);
		case E_BLOCK_LIT_FURNACE:   return LoadFurnaceFromNBT     (a_NBT, a_Tag, a_BlockType, a_BlockMeta, a_BlockX, a_BlockY, a_BlockZ);
		case E_BLOCK_MOB_SPAWNER:   return LoadMobSpawnerFromNBT  (a_NBT, a_Tag, a_BlockType, a_BlockMeta, a_BlockX, a_BlockY, a_BlockZ);
		case E_BLOCK_NOTE_BLOCK:    return LoadNoteBlockFromNBT   (a_NBT, a_Tag, a_BlockType, a_BlockMeta, a_BlockX, a_BlockY, a_BlockZ);
		case E_BLOCK_SIGN_POST:     return LoadSignFromNBT        (a_NBT, a_Tag, a_BlockType, a_BlockMeta, a_BlockX, a_BlockY, a_BlockZ);
		case E_BLOCK_TRAPPED_CHEST: return LoadChestFromNBT       (a_NBT, a_Tag, a_BlockType, a_BlockMeta, a_BlockX, a_BlockY, a_BlockZ);
		case E_BLOCK_WALLSIGN:      return LoadSignFromNBT        (a_NBT, a_Tag, a_BlockType, a_BlockMeta, a_BlockX, a_BlockY, a_BlockZ);

		// Blocktypes that have block entities but don't load their contents from disk:
		case E_BLOCK_ENDER_CHEST:   return nullptr;
	}

	// All the other blocktypes should have no entities assigned to them. Report an error:
	// Get the "id" tag:
	int TagID = a_NBT.FindChildByName(a_Tag, "id");
	AString TypeName("<unknown>");
	if (TagID >= 0)
	{
		TypeName.assign(a_NBT.GetData(TagID), static_cast<size_t>(a_NBT.GetDataLength(TagID)));
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
		a_Item.m_ItemCount = static_cast<char>(a_NBT.GetByte(Count));
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
	int FireworksTag = a_NBT.FindChildByName(TagTag, ((a_Item.m_ItemType == E_ITEM_FIREWORK_STAR) ? "Explosion" : "Fireworks"));
	if (FireworksTag > 0)
	{
		cFireworkItem::ParseFromNBT(a_Item.m_FireworkItem, a_NBT, FireworksTag, static_cast<ENUM_ITEM_ID>(a_Item.m_ItemType));
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
		int SlotNum = static_cast<int>(a_NBT.GetByte(SlotTag)) - a_SlotOffset;
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





bool cWSSAnvil::CheckBlockEntityType(const cParsedNBT & a_NBT, int a_TagIdx, const AStringVector & a_ExpectedTypes, int a_BlockX, int a_BlockY, int a_BlockZ)
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
	for (const auto & et: a_ExpectedTypes)
	{
		if (strncmp(a_NBT.GetData(TagID), et.c_str(), static_cast<size_t>(a_NBT.GetDataLength(TagID))) == 0)
		{
			return true;
		}
	}

	// Expectation not met, output an error into the log:
	AString expectedTypes;
	for (const auto & et : a_ExpectedTypes)
	{
		expectedTypes.append(", \"");
		expectedTypes.append(et);
		expectedTypes.push_back('\"');
	}
	LOGWARNING("Block entity type mismatch: exp %s, got \"%s\". The block entity at {%d, %d, %d} will lose all its properties.",
		expectedTypes.c_str() + 2,  // Skip the first ", " that is extra in the string
		AString(a_NBT.GetData(TagID), static_cast<size_t>(a_NBT.GetDataLength(TagID))).c_str(),
		a_BlockX, a_BlockY, a_BlockZ
	);
	return false;
}





cBlockEntity * cWSSAnvil::LoadBeaconFromNBT(const cParsedNBT & a_NBT, int a_TagIdx, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, int a_BlockX, int a_BlockY, int a_BlockZ)
{
	// Check if the data has a proper type:
	static const AStringVector expectedTypes({"Beacon", "minecraft:beacon"});
	if (!CheckBlockEntityType(a_NBT, a_TagIdx, expectedTypes, a_BlockX, a_BlockY, a_BlockZ))
	{
		return nullptr;
	}

	auto Beacon = cpp14::make_unique<cBeaconEntity>(a_BlockType, a_BlockMeta, a_BlockX, a_BlockY, a_BlockZ, m_World);

	int CurrentLine = a_NBT.FindChildByName(a_TagIdx, "Levels");
	if (CurrentLine >= 0)
	{
		Beacon->SetBeaconLevel(static_cast<char>(a_NBT.GetInt(CurrentLine)));
	}

	CurrentLine = a_NBT.FindChildByName(a_TagIdx, "Primary");
	if (CurrentLine >= 0)
	{
		Beacon->SetPrimaryEffect(static_cast<cEntityEffect::eType>(a_NBT.GetInt(CurrentLine)));
	}

	CurrentLine = a_NBT.FindChildByName(a_TagIdx, "Secondary");
	if (CurrentLine >= 0)
	{
		Beacon->SetSecondaryEffect(static_cast<cEntityEffect::eType>(a_NBT.GetInt(CurrentLine)));
	}

	// We are better than mojang, we load / save the beacon inventory!
	int Items = a_NBT.FindChildByName(a_TagIdx, "Items");
	if ((Items >= 0) && (a_NBT.GetType(Items) == TAG_List))
	{
		LoadItemGridFromNBT(Beacon->GetContents(), a_NBT, Items);
	}

	return Beacon.release();
}





cBlockEntity * cWSSAnvil::LoadBrewingstandFromNBT(const cParsedNBT & a_NBT, int a_TagIdx, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, int a_BlockX, int a_BlockY, int a_BlockZ)
{
	// Check if the data has a proper type:
	static const AStringVector expectedTypes({ "Brewingstand", "minecraft:brewing_stand" });
	if (!CheckBlockEntityType(a_NBT, a_TagIdx, expectedTypes, a_BlockX, a_BlockY, a_BlockZ))
	{
		return nullptr;
	}

	int Items = a_NBT.FindChildByName(a_TagIdx, "Items");
	if ((Items < 0) || (a_NBT.GetType(Items) != TAG_List))
	{
		return nullptr;  // Make it an empty brewingstand - the chunk loader will provide an empty cBrewingstandEntity for this
	}

	auto Brewingstand = cpp14::make_unique<cBrewingstandEntity>(a_BlockType, a_BlockMeta, a_BlockX, a_BlockY, a_BlockZ, m_World);

	// Fuel has to be loaded at first, because of slot events:
	int Fuel = a_NBT.FindChildByName(a_TagIdx, "Fuel");
	if (Fuel >= 0)
	{
		Int16 tb = a_NBT.GetShort(Fuel);
		Brewingstand->SetRemainingFuel(tb);
	}

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
			Brewingstand->SetSlot(a_NBT.GetByte(Slot), Item);
		}
	}  // for itr - ItemDefs[]

	// Load brewing time:
	int BrewTime = a_NBT.FindChildByName(a_TagIdx, "BrewTime");
	if (BrewTime >= 0)
	{
		Int16 tb = a_NBT.GetShort(BrewTime);
		Brewingstand->SetTimeBrewed(tb);
	}

	// Restart brewing:
	Brewingstand->LoadRecipes();
	Brewingstand->ContinueBrewing();
	return Brewingstand.release();
}





cBlockEntity * cWSSAnvil::LoadChestFromNBT(const cParsedNBT & a_NBT, int a_TagIdx, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, int a_BlockX, int a_BlockY, int a_BlockZ)
{
	// Check if the data has a proper type:
	// Note that older Cuberite code used "TrappedChest" for trapped chests; new code mimics vanilla and uses "Chest" throughout, but we allow migration here:
	static const AStringVector expectedTypes({ "Chest", "TrappedChest", "minecraft:chest" });
	if (!CheckBlockEntityType(a_NBT, a_TagIdx, expectedTypes, a_BlockX, a_BlockY, a_BlockZ))
	{
		return nullptr;
	}

	int Items = a_NBT.FindChildByName(a_TagIdx, "Items");
	if ((Items < 0) || (a_NBT.GetType(Items) != TAG_List))
	{
		return nullptr;  // Make it an empty chest - the chunk loader will provide an empty cChestEntity for this
	}
	auto Chest = cpp14::make_unique<cChestEntity>(a_BlockType, a_BlockMeta, a_BlockX, a_BlockY, a_BlockZ, m_World);
	LoadItemGridFromNBT(Chest->GetContents(), a_NBT, Items);
	return Chest.release();
}





cBlockEntity * cWSSAnvil::LoadCommandBlockFromNBT(const cParsedNBT & a_NBT, int a_TagIdx, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, int a_BlockX, int a_BlockY, int a_BlockZ)
{
	// Check if the data has a proper type:
	static const AStringVector expectedTypes({ "Control", "minecraft:command_block" });
	if (!CheckBlockEntityType(a_NBT, a_TagIdx, expectedTypes, a_BlockX, a_BlockY, a_BlockZ))
	{
		return nullptr;
	}

	auto CmdBlock = cpp14::make_unique<cCommandBlockEntity>(a_BlockType, a_BlockMeta, a_BlockX, a_BlockY, a_BlockZ, m_World);

	int currentLine = a_NBT.FindChildByName(a_TagIdx, "Command");
	if (currentLine >= 0)
	{
		CmdBlock->SetCommand(a_NBT.GetString(currentLine));
	}

	currentLine = a_NBT.FindChildByName(a_TagIdx, "SuccessCount");
	if (currentLine >= 0)
	{
		CmdBlock->SetResult(static_cast<NIBBLETYPE>(a_NBT.GetInt(currentLine)));
	}

	currentLine = a_NBT.FindChildByName(a_TagIdx, "LastOutput");
	if (currentLine >= 0)
	{
		CmdBlock->SetLastOutput(a_NBT.GetString(currentLine));
	}

	// TODO 2014-01-18 xdot: Figure out what TrackOutput is and parse it.

	return CmdBlock.release();
}





cBlockEntity * cWSSAnvil::LoadDispenserFromNBT(const cParsedNBT & a_NBT, int a_TagIdx, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, int a_BlockX, int a_BlockY, int a_BlockZ)
{
	// Check if the data has a proper type:
	static const AStringVector expectedTypes({ "Trap", "minecraft:dispenser" });
	if (!CheckBlockEntityType(a_NBT, a_TagIdx, expectedTypes, a_BlockX, a_BlockY, a_BlockZ))
	{
		return nullptr;
	}

	int Items = a_NBT.FindChildByName(a_TagIdx, "Items");
	if ((Items < 0) || (a_NBT.GetType(Items) != TAG_List))
	{
		return nullptr;  // Make it an empty dispenser - the chunk loader will provide an empty cDispenserEntity for this
	}
	auto Dispenser = cpp14::make_unique<cDispenserEntity>(a_BlockType, a_BlockMeta, a_BlockX, a_BlockY, a_BlockZ, m_World);
	LoadItemGridFromNBT(Dispenser->GetContents(), a_NBT, Items);
	return Dispenser.release();
}





cBlockEntity * cWSSAnvil::LoadDropperFromNBT(const cParsedNBT & a_NBT, int a_TagIdx, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, int a_BlockX, int a_BlockY, int a_BlockZ)
{
	// Check if the data has a proper type:
	static const AStringVector expectedTypes({ "Dropper", "minecraft:dropper" });
	if (!CheckBlockEntityType(a_NBT, a_TagIdx, expectedTypes, a_BlockX, a_BlockY, a_BlockZ))
	{
		return nullptr;
	}

	int Items = a_NBT.FindChildByName(a_TagIdx, "Items");
	if ((Items < 0) || (a_NBT.GetType(Items) != TAG_List))
	{
		return nullptr;  // Make it an empty dropper - the chunk loader will provide an empty cDropperEntity for this
	}
	auto Dropper = cpp14::make_unique<cDropperEntity>(a_BlockType, a_BlockMeta, a_BlockX, a_BlockY, a_BlockZ, m_World);
	LoadItemGridFromNBT(Dropper->GetContents(), a_NBT, Items);
	return Dropper.release();
}





cBlockEntity * cWSSAnvil::LoadFlowerPotFromNBT(const cParsedNBT & a_NBT, int a_TagIdx, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, int a_BlockX, int a_BlockY, int a_BlockZ)
{
	// Check if the data has a proper type:
	static const AStringVector expectedTypes({ "FlowerPot", "minecraft:flower_pot" });
	if (!CheckBlockEntityType(a_NBT, a_TagIdx, expectedTypes, a_BlockX, a_BlockY, a_BlockZ))
	{
		return nullptr;
	}

	auto FlowerPot = cpp14::make_unique<cFlowerPotEntity>(a_BlockType, a_BlockMeta, a_BlockX, a_BlockY, a_BlockZ, m_World);
	cItem Item;

	int currentLine = a_NBT.FindChildByName(a_TagIdx, "Item");
	if (currentLine >= 0)
	{
		if (a_NBT.GetType(currentLine) == TAG_String)
		{
			StringToItem(a_NBT.GetString(currentLine), Item);
		}
		else if (a_NBT.GetType(currentLine) == TAG_Int)
		{
			Item.m_ItemType = static_cast<short>(a_NBT.GetInt(currentLine));
		}
	}

	currentLine = a_NBT.FindChildByName(a_TagIdx, "Data");
	if ((currentLine >= 0) && (a_NBT.GetType(currentLine) == TAG_Int))
	{
		Item.m_ItemDamage = static_cast<short>(a_NBT.GetInt(currentLine));
	}

	FlowerPot->SetItem(Item);
	return FlowerPot.release();
}





cBlockEntity * cWSSAnvil::LoadFurnaceFromNBT(const cParsedNBT & a_NBT, int a_TagIdx, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, int a_BlockX, int a_BlockY, int a_BlockZ)
{
	// Check if the data has a proper type:
	static const AStringVector expectedTypes({ "Furnace", "minecraft:furnace" });
	if (!CheckBlockEntityType(a_NBT, a_TagIdx, expectedTypes, a_BlockX, a_BlockY, a_BlockZ))
	{
		return nullptr;
	}

	int Items = a_NBT.FindChildByName(a_TagIdx, "Items");
	if ((Items < 0) || (a_NBT.GetType(Items) != TAG_List))
	{
		return nullptr;  // Make it an empty furnace - the chunk loader will provide an empty cFurnaceEntity for this
	}

	auto Furnace = cpp14::make_unique<cFurnaceEntity>(a_BlockType, a_BlockMeta, a_BlockX, a_BlockY, a_BlockZ, m_World);
	Furnace->SetLoading(true);

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
	Furnace->SetLoading(false);
	return Furnace.release();
}





cBlockEntity * cWSSAnvil::LoadHopperFromNBT(const cParsedNBT & a_NBT, int a_TagIdx, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, int a_BlockX, int a_BlockY, int a_BlockZ)
{
	// Check if the data has a proper type:
	static const AStringVector expectedTypes({ "Hopper", "minecraft:hopper" });
	if (!CheckBlockEntityType(a_NBT, a_TagIdx, expectedTypes, a_BlockX, a_BlockY, a_BlockZ))
	{
		return nullptr;
	}

	int Items = a_NBT.FindChildByName(a_TagIdx, "Items");
	if ((Items < 0) || (a_NBT.GetType(Items) != TAG_List))
	{
		return nullptr;  // Make it an empty hopper - the chunk loader will provide an empty cHopperEntity for this
	}
	auto Hopper = cpp14::make_unique<cHopperEntity>(a_BlockType, a_BlockMeta, a_BlockX, a_BlockY, a_BlockZ, m_World);
	LoadItemGridFromNBT(Hopper->GetContents(), a_NBT, Items);
	return Hopper.release();
}





cBlockEntity * cWSSAnvil::LoadJukeboxFromNBT(const cParsedNBT & a_NBT, int a_TagIdx, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, int a_BlockX, int a_BlockY, int a_BlockZ)
{
	// Check if the data has a proper type:
	static const AStringVector expectedTypes({ "RecordPlayer", "minecraft:jukebox" });
	if (!CheckBlockEntityType(a_NBT, a_TagIdx, expectedTypes, a_BlockX, a_BlockY, a_BlockZ))
	{
		return nullptr;
	}

	auto Jukebox = cpp14::make_unique<cJukeboxEntity>(a_BlockType, a_BlockMeta, a_BlockX, a_BlockY, a_BlockZ, m_World);
	int Record = a_NBT.FindChildByName(a_TagIdx, "Record");
	if (Record >= 0)
	{
		Jukebox->SetRecord(a_NBT.GetInt(Record));
	}
	return Jukebox.release();
}





cBlockEntity * cWSSAnvil::LoadMobSpawnerFromNBT(const cParsedNBT & a_NBT, int a_TagIdx, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, int a_BlockX, int a_BlockY, int a_BlockZ)
{
	// Check if the data has a proper type:
	static const AStringVector expectedTypes({ "MobSpawner", "minecraft:mob_spawner" });
	if (!CheckBlockEntityType(a_NBT, a_TagIdx, expectedTypes, a_BlockX, a_BlockY, a_BlockZ))
	{
		return nullptr;
	}

	auto MobSpawner = cpp14::make_unique<cMobSpawnerEntity>(a_BlockType, a_BlockMeta, a_BlockX, a_BlockY, a_BlockZ, m_World);

	// Load entity (Cuberite worlds):
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





cBlockEntity * cWSSAnvil::LoadMobHeadFromNBT(const cParsedNBT & a_NBT, int a_TagIdx, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, int a_BlockX, int a_BlockY, int a_BlockZ)
{
	// Check if the data has a proper type:
	static const AStringVector expectedTypes({ "Skull", "minecraft:skull" });
	if (!CheckBlockEntityType(a_NBT, a_TagIdx, expectedTypes, a_BlockX, a_BlockY, a_BlockZ))
	{
		return nullptr;
	}

	auto MobHead = cpp14::make_unique<cMobHeadEntity>(a_BlockType, a_BlockMeta, a_BlockX, a_BlockY, a_BlockZ, m_World);

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

	int ownerLine = a_NBT.FindChildByName(a_TagIdx, "Owner");
	if (ownerLine >= 0)
	{
		AString OwnerName, OwnerUUID, OwnerTexture, OwnerTextureSignature;

		currentLine = a_NBT.FindChildByName(ownerLine, "Id");
		if (currentLine >= 0)
		{
			OwnerUUID = a_NBT.GetString(currentLine);
		}

		currentLine = a_NBT.FindChildByName(ownerLine, "Name");
		if (currentLine >= 0)
		{
			OwnerName = a_NBT.GetString(currentLine);
		}

		int textureLine = a_NBT.GetFirstChild(  // The first texture of
			a_NBT.FindChildByName(              // The texture list of
				a_NBT.FindChildByName(          // The Properties compound of
					ownerLine,                  // The Owner compound
					"Properties"
				),
				"textures"
			)
		);
		if (textureLine >= 0)
		{
			currentLine = a_NBT.FindChildByName(textureLine, "Signature");
			if (currentLine >= 0)
			{
				OwnerTextureSignature = a_NBT.GetString(currentLine);
			}

			currentLine = a_NBT.FindChildByName(textureLine, "Value");
			if (currentLine >= 0)
			{
				OwnerTexture = a_NBT.GetString(currentLine);
			}
		}
		MobHead->SetOwner(OwnerUUID, OwnerName, OwnerTexture, OwnerTextureSignature);
	}

	return MobHead.release();
}





cBlockEntity * cWSSAnvil::LoadNoteBlockFromNBT(const cParsedNBT & a_NBT, int a_TagIdx, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, int a_BlockX, int a_BlockY, int a_BlockZ)
{
	// Check if the data has a proper type:
	static const AStringVector expectedTypes({ "Music", "minecraft:noteblock" });
	if (!CheckBlockEntityType(a_NBT, a_TagIdx, expectedTypes, a_BlockX, a_BlockY, a_BlockZ))
	{
		return nullptr;
	}

	auto NoteBlock = cpp14::make_unique<cNoteEntity>(a_BlockType, a_BlockMeta, a_BlockX, a_BlockY, a_BlockZ, m_World);
	int note = a_NBT.FindChildByName(a_TagIdx, "note");
	if (note >= 0)
	{
		NoteBlock->SetPitch(static_cast<char>(a_NBT.GetByte(note)));
	}
	return NoteBlock.release();
}





cBlockEntity * cWSSAnvil::LoadSignFromNBT(const cParsedNBT & a_NBT, int a_TagIdx, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, int a_BlockX, int a_BlockY, int a_BlockZ)
{
	// Check if the data has a proper type:
	static const AStringVector expectedTypes({ "Sign", "minecraft:sign" });
	if (!CheckBlockEntityType(a_NBT, a_TagIdx, expectedTypes, a_BlockX, a_BlockY, a_BlockZ))
	{
		return nullptr;
	}

	auto Sign = cpp14::make_unique<cSignEntity>(a_BlockType, a_BlockMeta, a_BlockX, a_BlockY, a_BlockZ, m_World);

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
	else if (strncmp(a_IDTag, "Painting", a_IDTagLength) == 0)
	{
		LoadPaintingFromNBT(a_Entities, a_NBT, a_EntityTagIdx);
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
	std::unique_ptr<cBoat> Boat = cpp14::make_unique<cBoat>(0, 0, 0, cBoat::bmOak);
	if (!LoadEntityBaseFromNBT(*Boat.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	int TypeIdx = a_NBT.FindChildByName(a_TagIdx, "Type");
	if (TypeIdx > 0)
	{
		Boat->SetMaterial(cBoat::StringToMaterial(a_NBT.GetString(TypeIdx)));
	}
	a_Entities.push_back(Boat.release());
}





void cWSSAnvil::LoadEnderCrystalFromNBT(cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx)
{
	std::unique_ptr<cEnderCrystal> EnderCrystal = cpp14::make_unique<cEnderCrystal>(0, 0, 0);
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

	BLOCKTYPE Type = static_cast<BLOCKTYPE>(a_NBT.GetInt(TypeIdx));
	NIBBLETYPE Meta = static_cast<NIBBLETYPE>(a_NBT.GetByte(MetaIdx));

	std::unique_ptr<cFallingBlock> FallingBlock = cpp14::make_unique<cFallingBlock>(Vector3i(0, 0, 0), Type, Meta);
	if (!LoadEntityBaseFromNBT(*FallingBlock.get(), a_NBT, a_TagIdx))
	{
		return;
	}
	a_Entities.push_back(FallingBlock.release());
}





void cWSSAnvil::LoadMinecartRFromNBT(cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx)
{
	std::unique_ptr<cRideableMinecart> Minecart = cpp14::make_unique<cRideableMinecart>(0, 0, 0, cItem(), 1);  // TODO: Load the block and the height
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
	std::unique_ptr<cMinecartWithChest> Minecart = cpp14::make_unique<cMinecartWithChest>(0, 0, 0);
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
	std::unique_ptr<cMinecartWithFurnace> Minecart = cpp14::make_unique<cMinecartWithFurnace>(0, 0, 0);
	if (!LoadEntityBaseFromNBT(*Minecart.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	// TODO: Load the Push and Fuel tags

	a_Entities.push_back(Minecart.release());
}





void cWSSAnvil::LoadMinecartTFromNBT(cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx)
{
	std::unique_ptr<cMinecartWithTNT> Minecart = cpp14::make_unique<cMinecartWithTNT>(0, 0, 0);
	if (!LoadEntityBaseFromNBT(*Minecart.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	// TODO: Everything to do with TNT carts

	a_Entities.push_back(Minecart.release());
}





void cWSSAnvil::LoadMinecartHFromNBT(cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx)
{
	std::unique_ptr<cMinecartWithHopper> Minecart = cpp14::make_unique<cMinecartWithHopper>(0, 0, 0);
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

	std::unique_ptr<cPickup> Pickup = cpp14::make_unique<cPickup>(0, 0, 0, Item, false);  // Pickup delay doesn't matter, just say false
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
	std::unique_ptr<cTNTEntity> TNT = cpp14::make_unique<cTNTEntity>(0.0, 0.0, 0.0, 0);
	if (!LoadEntityBaseFromNBT(*TNT.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	// Load Fuse Ticks:
	int FuseTicks = a_NBT.FindChildByName(a_TagIdx, "Fuse");
	if (FuseTicks > 0)
	{
		TNT->SetFuseTicks(static_cast<int>(a_NBT.GetByte(FuseTicks)));
	}

	a_Entities.push_back(TNT.release());
}





void cWSSAnvil::LoadExpOrbFromNBT(cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx)
{
	std::unique_ptr<cExpOrb> ExpOrb = cpp14::make_unique<cExpOrb>(0.0, 0.0, 0.0, 0);
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
	if (Facing < 0)
	{
		return;
	}

	a_Hanging.SetProtocolFacing(a_NBT.GetByte(Facing));

	int TileX = a_NBT.FindChildByName(a_TagIdx, "TileX");
	int TileY = a_NBT.FindChildByName(a_TagIdx, "TileY");
	int TileZ = a_NBT.FindChildByName(a_TagIdx, "TileZ");
	if ((TileX > 0) && (TileY > 0) && (TileZ > 0))
	{
		a_Hanging.SetPosition(
			static_cast<double>(a_NBT.GetInt(TileX)),
			static_cast<double>(a_NBT.GetInt(TileY)),
			static_cast<double>(a_NBT.GetInt(TileZ))
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

	std::unique_ptr<cItemFrame> ItemFrame = cpp14::make_unique<cItemFrame>(BLOCK_FACE_NONE, 0.0, 0.0, 0.0);
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
		ItemFrame->SetItemRotation(static_cast<Byte>(a_NBT.GetByte(Rotation)));
	}

	a_Entities.push_back(ItemFrame.release());
}





void cWSSAnvil::LoadPaintingFromNBT(cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx)
{
	// Load painting name:
	int MotiveTag = a_NBT.FindChildByName(a_TagIdx, "Motive");
	if ((MotiveTag < 0) || (a_NBT.GetType(MotiveTag) != TAG_String))
	{
		return;
	}

	std::unique_ptr<cPainting> Painting = cpp14::make_unique<cPainting>(a_NBT.GetString(MotiveTag), BLOCK_FACE_NONE, 0.0, 0.0, 0.0);
	if (!LoadEntityBaseFromNBT(*Painting.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	LoadHangingFromNBT(*Painting.get(), a_NBT, a_TagIdx);
	a_Entities.push_back(Painting.release());
}





void cWSSAnvil::LoadArrowFromNBT(cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx)
{
	std::unique_ptr<cArrowEntity> Arrow = cpp14::make_unique<cArrowEntity>(nullptr, 0, 0, 0, Vector3d(0, 0, 0));
	if (!LoadProjectileBaseFromNBT(*Arrow.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	// Load pickup state:
	int PickupIdx = a_NBT.FindChildByName(a_TagIdx, "pickup");
	if ((PickupIdx > 0) && (a_NBT.GetType(PickupIdx) == TAG_Byte))
	{
		Arrow->SetPickupState(static_cast<cArrowEntity::ePickupState>(a_NBT.GetByte(PickupIdx)));
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
					Arrow->SetBlockHit(Vector3i(a_NBT.GetShort(InBlockXIdx), a_NBT.GetShort(InBlockYIdx), a_NBT.GetShort(InBlockZIdx)));
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
	std::unique_ptr<cSplashPotionEntity> SplashPotion = cpp14::make_unique<cSplashPotionEntity>(nullptr, 0, 0, 0, Vector3d(0, 0, 0), cItem());
	if (!LoadProjectileBaseFromNBT(*SplashPotion.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	int EffectDuration         = a_NBT.FindChildByName(a_TagIdx, "EffectDuration");
	int EffectIntensity        = a_NBT.FindChildByName(a_TagIdx, "EffectIntensity");
	int EffectDistanceModifier = a_NBT.FindChildByName(a_TagIdx, "EffectDistanceModifier");

	SplashPotion->SetEntityEffectType(static_cast<cEntityEffect::eType>(a_NBT.FindChildByName(a_TagIdx, "EffectType")));
	SplashPotion->SetEntityEffect(cEntityEffect(EffectDuration, static_cast<Int16>(EffectIntensity), EffectDistanceModifier));
	SplashPotion->SetPotionColor(a_NBT.FindChildByName(a_TagIdx, "PotionName"));

	// Store the new splash potion in the entities list:
	a_Entities.push_back(SplashPotion.release());
}





void cWSSAnvil::LoadSnowballFromNBT(cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx)
{
	std::unique_ptr<cThrownSnowballEntity> Snowball = cpp14::make_unique<cThrownSnowballEntity>(nullptr, 0, 0, 0, Vector3d(0, 0, 0));
	if (!LoadProjectileBaseFromNBT(*Snowball.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	// Store the new snowball in the entities list:
	a_Entities.push_back(Snowball.release());
}





void cWSSAnvil::LoadEggFromNBT(cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx)
{
	std::unique_ptr<cThrownEggEntity> Egg = cpp14::make_unique<cThrownEggEntity>(nullptr, 0, 0, 0, Vector3d(0, 0, 0));
	if (!LoadProjectileBaseFromNBT(*Egg.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	// Store the new egg in the entities list:
	a_Entities.push_back(Egg.release());
}





void cWSSAnvil::LoadFireballFromNBT(cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx)
{
	std::unique_ptr<cGhastFireballEntity> Fireball = cpp14::make_unique<cGhastFireballEntity>(nullptr, 0, 0, 0, Vector3d(0, 0, 0));
	if (!LoadProjectileBaseFromNBT(*Fireball.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	// Store the new fireball in the entities list:
	a_Entities.push_back(Fireball.release());
}





void cWSSAnvil::LoadFireChargeFromNBT(cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx)
{
	std::unique_ptr<cFireChargeEntity> FireCharge = cpp14::make_unique<cFireChargeEntity>(nullptr, 0, 0, 0, Vector3d(0, 0, 0));
	if (!LoadProjectileBaseFromNBT(*FireCharge.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	// Store the new FireCharge in the entities list:
	a_Entities.push_back(FireCharge.release());
}





void cWSSAnvil::LoadThrownEnderpearlFromNBT(cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx)
{
	std::unique_ptr<cThrownEnderPearlEntity> Enderpearl = cpp14::make_unique<cThrownEnderPearlEntity>(nullptr, 0, 0, 0, Vector3d(0, 0, 0));
	if (!LoadProjectileBaseFromNBT(*Enderpearl.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	// Store the new enderpearl in the entities list:
	a_Entities.push_back(Enderpearl.release());
}





void cWSSAnvil::LoadBatFromNBT(cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx)
{
	std::unique_ptr<cBat> Monster = cpp14::make_unique<cBat>();
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
	std::unique_ptr<cBlaze> Monster = cpp14::make_unique<cBlaze>();
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
	std::unique_ptr<cCaveSpider> Monster = cpp14::make_unique<cCaveSpider>();
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
	std::unique_ptr<cChicken> Monster = cpp14::make_unique<cChicken>();
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
	std::unique_ptr<cCow> Monster = cpp14::make_unique<cCow>();
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
	std::unique_ptr<cCreeper> Monster = cpp14::make_unique<cCreeper>();
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
	std::unique_ptr<cEnderDragon> Monster = cpp14::make_unique<cEnderDragon>();
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
	std::unique_ptr<cEnderman> Monster = cpp14::make_unique<cEnderman>();
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
	std::unique_ptr<cGhast> Monster = cpp14::make_unique<cGhast>();
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
	std::unique_ptr<cGiant> Monster = cpp14::make_unique<cGiant>();
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
	std::unique_ptr<cGuardian> Monster = cpp14::make_unique<cGuardian>();
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

	std::unique_ptr<cHorse> Monster = cpp14::make_unique<cHorse>(Type, Color, Style, 1);

	if (!LoadEntityBaseFromNBT(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	if (!LoadMonsterBaseFromNBT(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	int AgeableIdx  = a_NBT.FindChildByName(a_TagIdx, "Age");
	if (AgeableIdx > 0)
	{
		int Age;
		switch (a_NBT.GetType(AgeableIdx))
		{
			case TAG_Byte: Age = static_cast<int>(a_NBT.GetByte(AgeableIdx)); break;
			case TAG_Int:  Age = a_NBT.GetInt (AgeableIdx); break;
			default: Age = 0; break;
		}
		Monster->SetAge(Age);
	}

	a_Entities.push_back(Monster.release());
}





void cWSSAnvil::LoadIronGolemFromNBT(cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx)
{
	std::unique_ptr<cIronGolem> Monster = cpp14::make_unique<cIronGolem>();
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

	std::unique_ptr<cMagmaCube> Monster = cpp14::make_unique<cMagmaCube>(Size);
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
	std::unique_ptr<cMooshroom> Monster = cpp14::make_unique<cMooshroom>();
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
	std::unique_ptr<cOcelot> Monster = cpp14::make_unique<cOcelot>();
	if (!LoadEntityBaseFromNBT(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	if (!LoadMonsterBaseFromNBT(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	int AgeableIdx  = a_NBT.FindChildByName(a_TagIdx, "Age");
	if (AgeableIdx > 0)
	{
		int Age;
		switch (a_NBT.GetType(AgeableIdx))
		{
			case TAG_Byte: Age = static_cast<int>(a_NBT.GetByte(AgeableIdx)); break;
			case TAG_Int:  Age = a_NBT.GetInt (AgeableIdx); break;
			default: Age = 0; break;
		}
		Monster->SetAge(Age);
	}

	a_Entities.push_back(Monster.release());
}





void cWSSAnvil::LoadPigFromNBT(cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx)
{
	std::unique_ptr<cPig> Monster = cpp14::make_unique<cPig>();
	if (!LoadEntityBaseFromNBT(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	if (!LoadMonsterBaseFromNBT(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	int AgeableIdx  = a_NBT.FindChildByName(a_TagIdx, "Age");
	if (AgeableIdx > 0)
	{
		int Age;
		switch (a_NBT.GetType(AgeableIdx))
		{
			case TAG_Byte: Age = static_cast<int>(a_NBT.GetByte(AgeableIdx)); break;
			case TAG_Int:  Age = a_NBT.GetInt (AgeableIdx); break;
			default: Age = 0; break;
		}
		Monster->SetAge(Age);
	}

	a_Entities.push_back(Monster.release());
}





void cWSSAnvil::LoadRabbitFromNBT(cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx)
{
	int TypeIdx  = a_NBT.FindChildByName(a_TagIdx, "RabbitType");
	int MoreCarrotTicksIdx = a_NBT.FindChildByName(a_TagIdx, "MoreCarrotTicks");

	if ((TypeIdx < 0) || (MoreCarrotTicksIdx < 0))
	{
		return;
	}

	int Type = a_NBT.GetInt(TypeIdx);
	int MoreCarrotTicks = a_NBT.GetInt(MoreCarrotTicksIdx);

	std::unique_ptr<cRabbit> Monster = cpp14::make_unique<cRabbit>(static_cast<eRabbitType>(Type), MoreCarrotTicks);
	if (!LoadEntityBaseFromNBT(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	if (!LoadMonsterBaseFromNBT(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	int AgeableIdx  = a_NBT.FindChildByName(a_TagIdx, "Age");
	if (AgeableIdx > 0)
	{
		int Age;
		switch (a_NBT.GetType(AgeableIdx))
		{
			case TAG_Byte: Age = static_cast<int>(a_NBT.GetByte(AgeableIdx)); break;
			case TAG_Int:  Age = a_NBT.GetInt (AgeableIdx); break;
			default: Age = 0; break;
		}
		Monster->SetAge(Age);
	}

	a_Entities.push_back(Monster.release());
}





void cWSSAnvil::LoadSheepFromNBT(cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx)
{
	int ColorIdx = a_NBT.FindChildByName(a_TagIdx, "Color");
	int Color = -1;
	if (ColorIdx > 0)
	{
		Color = static_cast<int>(a_NBT.GetByte(ColorIdx));
	}

	std::unique_ptr<cSheep> Monster = cpp14::make_unique<cSheep>(Color);
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

	int AgeableIdx  = a_NBT.FindChildByName(a_TagIdx, "Age");
	if (AgeableIdx > 0)
	{
		int Age;
		switch (a_NBT.GetType(AgeableIdx))
		{
			case TAG_Byte: Age = static_cast<int>(a_NBT.GetByte(AgeableIdx)); break;
			case TAG_Int:  Age = a_NBT.GetInt (AgeableIdx); break;
			default: Age = 0; break;
		}
		Monster->SetAge(Age);
	}

	a_Entities.push_back(Monster.release());
}





void cWSSAnvil::LoadSilverfishFromNBT(cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx)
{
	std::unique_ptr<cSilverfish> Monster = cpp14::make_unique<cSilverfish>();
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

	std::unique_ptr<cSkeleton> Monster = cpp14::make_unique<cSkeleton>(Type);
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

	std::unique_ptr<cSlime> Monster = cpp14::make_unique<cSlime>(Size);
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
	std::unique_ptr<cSnowGolem> Monster = cpp14::make_unique<cSnowGolem>();
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
	std::unique_ptr<cSpider> Monster = cpp14::make_unique<cSpider>();
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
	std::unique_ptr<cSquid> Monster = cpp14::make_unique<cSquid>();
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

	std::unique_ptr<cVillager> Monster = cpp14::make_unique<cVillager>(cVillager::eVillagerType(Type));
	if (!LoadEntityBaseFromNBT(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	if (!LoadMonsterBaseFromNBT(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	int AgeableIdx  = a_NBT.FindChildByName(a_TagIdx, "Age");
	if (AgeableIdx > 0)
	{
		int Age;
		switch (a_NBT.GetType(AgeableIdx))
		{
			case TAG_Byte: Age = static_cast<int>(a_NBT.GetByte(AgeableIdx)); break;
			case TAG_Int:  Age = a_NBT.GetInt (AgeableIdx); break;
			default: Age = 0; break;
		}
		Monster->SetAge(Age);
	}


	a_Entities.push_back(Monster.release());
}





void cWSSAnvil::LoadWitchFromNBT(cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx)
{
	std::unique_ptr<cWitch> Monster = cpp14::make_unique<cWitch>();
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
	std::unique_ptr<cWither> Monster = cpp14::make_unique<cWither>();
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
		Monster->SetWitherInvulnerableTicks(static_cast<unsigned int>(a_NBT.GetInt(CurrLine)));
	}

	a_Entities.push_back(Monster.release());
}





void cWSSAnvil::LoadWolfFromNBT(cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx)
{
	std::unique_ptr<cWolf> Monster = cpp14::make_unique<cWolf>();
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
				Monster->SetCollarColor(a_NBT.GetByte(CollarColorIdx));
				break;
			}
			case TAG_Int:
			{
				// Old MCS code used this, keep reading it for compatibility reasons:
				Monster->SetCollarColor(a_NBT.GetInt(CollarColorIdx));
				break;
			}
			default:
			{
				// No other values are interpreted
				break;
			}
		}
	}

	int AgeableIdx  = a_NBT.FindChildByName(a_TagIdx, "Age");
	if (AgeableIdx > 0)
	{
		int Age;
		switch (a_NBT.GetType(AgeableIdx))
		{
			case TAG_Byte: Age = static_cast<int>(a_NBT.GetByte(AgeableIdx)); break;
			case TAG_Int:  Age = a_NBT.GetInt (AgeableIdx); break;
			default: Age = 0; break;
		}
		Monster->SetAge(Age);
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

	std::unique_ptr<cZombie> Monster = cpp14::make_unique<cZombie>(IsVillagerZombie);
	if (!LoadEntityBaseFromNBT(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	if (!LoadMonsterBaseFromNBT(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	int AgeableIdx  = a_NBT.FindChildByName(a_TagIdx, "Age");
	if (AgeableIdx > 0)
	{
		int Age;
		switch (a_NBT.GetType(AgeableIdx))
		{
			case TAG_Byte: Age = static_cast<int>(a_NBT.GetByte(AgeableIdx)); break;
			case TAG_Int:  Age = a_NBT.GetInt (AgeableIdx); break;
			default: Age = 0; break;
		}
		Monster->SetAge(Age);
	}

	a_Entities.push_back(Monster.release());
}





void cWSSAnvil::LoadPigZombieFromNBT(cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx)
{
	std::unique_ptr<cZombiePigman> Monster = cpp14::make_unique<cZombiePigman>();
	if (!LoadEntityBaseFromNBT(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	if (!LoadMonsterBaseFromNBT(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	int AgeableIdx  = a_NBT.FindChildByName(a_TagIdx, "Age");
	if (AgeableIdx > 0)
	{
		int Age;
		switch (a_NBT.GetType(AgeableIdx))
		{
			case TAG_Byte: Age = static_cast<int>(a_NBT.GetByte(AgeableIdx)); break;
			case TAG_Int:  Age = a_NBT.GetInt (AgeableIdx); break;
			default: Age = 0; break;
		}
		Monster->SetAge(Age);
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

cWSSAnvil::cMCAFile::cMCAFile(cWSSAnvil & a_ParentSchema, const AString & a_FileName, int a_RegionX, int a_RegionZ) :
	m_ParentSchema(a_ParentSchema),
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

	m_File.Seek(static_cast<int>(ChunkOffset * 4096));

	UInt32 ChunkSize = 0;
	if (m_File.Read(&ChunkSize, 4) != 4)
	{
		m_ParentSchema.ChunkLoadFailed(a_Chunk.m_ChunkX, a_Chunk.m_ChunkZ, "Cannot read chunk size", "");
		return false;
	}
	ChunkSize = ntohl(ChunkSize);
	if (ChunkSize < 1)
	{
		// Chunk size too small
		m_ParentSchema.ChunkLoadFailed(a_Chunk.m_ChunkX, a_Chunk.m_ChunkZ, "Chunk size too small", "");
		return false;
	}

	char CompressionType = 0;
	if (m_File.Read(&CompressionType, 1) != 1)
	{
		m_ParentSchema.ChunkLoadFailed(a_Chunk.m_ChunkX, a_Chunk.m_ChunkZ, "Cannot read chunk compression", "");
		return false;
	}
	ChunkSize--;

	a_Data = m_File.Read(ChunkSize);
	if (a_Data.size() != ChunkSize)
	{
		m_ParentSchema.ChunkLoadFailed(a_Chunk.m_ChunkX, a_Chunk.m_ChunkZ, "Cannot read entire chunk data", a_Data);
		return false;
	}

	if (CompressionType != 2)
	{
		// Chunk is in an unknown compression
		m_ParentSchema.ChunkLoadFailed(a_Chunk.m_ChunkX, a_Chunk.m_ChunkZ, Printf("Unknown chunk compression: %d", CompressionType).c_str(), a_Data);
		return false;
	}
		return true;
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
	m_File.Seek(static_cast<int>(ChunkSector * 4096));
	UInt32 ChunkSize = htonl(static_cast<UInt32>(a_Data.size() + 1));
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
	if (m_File.Write(a_Data.data(), a_Data.size()) != static_cast<int>(a_Data.size()))
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
	ChunkSize = (static_cast<UInt32>(a_Data.size()) + MCA_CHUNK_HEADER_LENGTH + 4095) / 4096;  // Round data size up to nearest 4KB sector, make it a sector number
	if (ChunkSize > 255)
	{
		LOGWARNING("Cannot save chunk [%d, %d], the data is too large (%u KiB, maximum is 1024 KiB). Remove some entities and retry.",
			a_Chunk.m_ChunkX, a_Chunk.m_ChunkZ, static_cast<unsigned>(ChunkSize * 4)
		);
		return false;
	}

	// Store the header info in the table
	m_Header[LocalX + 32 * LocalZ] = htonl(static_cast<UInt32>((ChunkSector << 8) | ChunkSize));

	// Set the modification time
	m_TimeStamps[LocalX + 32 * LocalZ] =  htonl(static_cast<UInt32>(time(nullptr)));

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




