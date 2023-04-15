
// WSSAnvil.cpp

// Implements the cWSSAnvil class representing the Anvil world storage scheme

#include "Globals.h"
#include "WSSAnvil.h"
#include "NBTChunkSerializer.h"
#include "EnchantmentSerializer.h"
#include "NamespaceSerializer.h"
#include "json/json.h"
#include "OSSupport/GZipFile.h"
#include "../World.h"
#include "../Item.h"
#include "../ItemGrid.h"
#include "../StringCompression.h"
#include "../SetChunkData.h"
#include "../Root.h"
#include "../BlockType.h"
#include "../JsonUtils.h"

#include "../BlockEntities/BannerEntity.h"
#include "../BlockEntities/BeaconEntity.h"
#include "../BlockEntities/BedEntity.h"
#include "../BlockEntities/BrewingstandEntity.h"
#include "../BlockEntities/ChestEntity.h"
#include "../BlockEntities/CommandBlockEntity.h"
#include "../BlockEntities/DispenserEntity.h"
#include "../BlockEntities/DropperEntity.h"
#include "../BlockEntities/EnchantingTableEntity.h"
#include "../BlockEntities/EnderChestEntity.h"
#include "../BlockEntities/EndPortalEntity.h"
#include "../BlockEntities/FurnaceEntity.h"
#include "../BlockEntities/HopperEntity.h"
#include "../BlockEntities/JukeboxEntity.h"
#include "../BlockEntities/NoteEntity.h"
#include "../BlockEntities/SignEntity.h"
#include "../BlockEntities/MobHeadEntity.h"
#include "../BlockEntities/MobSpawnerEntity.h"
#include "../BlockEntities/FlowerPotEntity.h"

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
#include "../Entities/LeashKnot.h"
#include "../Entities/Painting.h"

#include "../Protocol/MojangAPI.h"
#include "../Server.h"
#include "../BoundingBox.h"





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
	Super(a_World),
	m_Compressor(a_CompressionFactor)
{
	// Create a level.dat file for mapping tools, if it doesn't already exist:
	AString fnam;
	Printf(fnam, "%s%clevel.dat", a_World->GetDataPath().c_str(), cFile::PathSeparator());
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
		Writer.AddInt("SpawnX", a_World->GetSpawnX());
		Writer.AddInt("SpawnY", a_World->GetSpawnY());
		Writer.AddInt("SpawnZ", a_World->GetSpawnZ());
		Writer.AddInt("version", 19133);
		Writer.AddLong("DayTime", a_World->GetWorldDate().count());
		Writer.AddLong("Time", a_World->GetWorldAge().count());
		Writer.AddLong("SizeOnDisk", 0);
		Writer.AddString("generatorName", "default");
		Writer.AddString("generatorOptions", "");
		Writer.AddString("LevelName", a_World->GetName());
		Writer.EndCompound();
		Writer.Finish();

		GZipFile::Write(fnam, Writer.GetResult());
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
	ContiguousByteBuffer ChunkData;
	if (!GetChunkData(a_Chunk, ChunkData))
	{
		// The reason for failure is already printed in GetChunkData()
		return false;
	}

	return LoadChunkFromData(a_Chunk, ChunkData);
}





bool cWSSAnvil::SaveChunk(const cChunkCoords & a_Chunk)
{
	try
	{
		if (!SetChunkData(a_Chunk, SaveChunkToData(a_Chunk).GetView()))
		{
			LOGWARNING("Cannot store chunk [%d, %d] data", a_Chunk.m_ChunkX, a_Chunk.m_ChunkZ);
			return false;
		}
	}
	catch (const std::exception & Oops)
	{
		LOGWARNING("Cannot serialize chunk [%d, %d] into data: %s", a_Chunk.m_ChunkX, a_Chunk.m_ChunkZ, Oops.what());
		return false;
	}

	// Everything successful
	return true;
}





void cWSSAnvil::ChunkLoadFailed(int a_ChunkX, int a_ChunkZ, const AString & a_Reason, const ContiguousByteBufferView a_ChunkDataToSave)
{
	// Construct the filename for offloading:
	AString OffloadFileName;
	Printf(OffloadFileName, "%s%cregion%cbadchunks", m_World->GetDataPath().c_str(), cFile::PathSeparator(), cFile::PathSeparator());
	cFile::CreateFolder(OffloadFileName);
	auto t = time(nullptr);
	struct tm stm;
	#ifdef _MSC_VER
		localtime_s(&stm, &t);
	#else
		localtime_r(&t, &stm);
	#endif
	AppendPrintf(OffloadFileName, "%cch.%d.%d.%d-%02d-%02d-%02d-%02d-%02d.dat",
		cFile::PathSeparator(), a_ChunkX, a_ChunkZ,
		stm.tm_year + 1900, stm.tm_mon + 1, stm.tm_mday, stm.tm_hour, stm.tm_min, stm.tm_sec
	);

	// Log the warning to console:
	const int RegionX = FAST_FLOOR_DIV(a_ChunkX, 32);
	const int RegionZ = FAST_FLOOR_DIV(a_ChunkZ, 32);
	AString Info = Printf("Loading chunk [%d, %d] for world %s from file r.%d.%d.mca failed: %s Offloading old chunk data to file %s and regenerating chunk.",
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





bool cWSSAnvil::GetChunkData(const cChunkCoords & a_Chunk, ContiguousByteBuffer & a_Data)
{
	cCSLock Lock(m_CS);
	cMCAFile * File = LoadMCAFile(a_Chunk);
	if (File == nullptr)
	{
		return false;
	}
	return File->GetChunkData(a_Chunk, a_Data);
}





bool cWSSAnvil::SetChunkData(const cChunkCoords & a_Chunk, const ContiguousByteBufferView a_Data)
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
	Printf(FileName, "%s%cregion", m_World->GetDataPath().c_str(), cFile::PathSeparator());
	cFile::CreateFolder(FileName);
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





bool cWSSAnvil::LoadChunkFromData(const cChunkCoords & a_Chunk, const ContiguousByteBufferView a_Data)
{
	try
	{
		const auto Extracted = m_Extractor.ExtractZLib(a_Data);
		cParsedNBT NBT(Extracted.GetView());

		if (!NBT.IsValid())
		{
			// NBT Parsing failed:
			throw std::runtime_error(fmt::format("NBT parsing failed. {} at position {}.", NBT.GetErrorCode().message(), NBT.GetErrorPos()));
		}

		// Load the data from NBT:
		return LoadChunkFromNBT(a_Chunk, NBT, a_Data);
	}
	catch (const std::exception & Oops)
	{
		ChunkLoadFailed(a_Chunk.m_ChunkX, a_Chunk.m_ChunkZ, Oops.what(), a_Data);
		return false;
	}
}





Compression::Result cWSSAnvil::SaveChunkToData(const cChunkCoords & a_Chunk)
{
	cFastNBTWriter Writer;
	NBTChunkSerializer::Serialize(*m_World, a_Chunk, Writer);
	Writer.Finish();

	return m_Compressor.CompressZLib(Writer.GetResult());
}





bool cWSSAnvil::LoadChunkFromNBT(const cChunkCoords & a_Chunk, const cParsedNBT & a_NBT, const ContiguousByteBufferView a_RawChunkData)
{
	struct SetChunkData Data(a_Chunk);

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
		const int SectionYTag = a_NBT.FindChildByName(Child, "Y");
		if ((SectionYTag < 0) || (a_NBT.GetType(SectionYTag) != TAG_Byte))
		{
			ChunkLoadFailed(a_Chunk.m_ChunkX, a_Chunk.m_ChunkZ, "NBT tag missing or has wrong: Y", a_RawChunkData);
			return false;
		}

		const int Y = a_NBT.GetByte(SectionYTag);
		if ((Y < 0) || (Y > static_cast<int>(cChunkDef::NumSections - 1)))
		{
			ChunkLoadFailed(a_Chunk.m_ChunkX, a_Chunk.m_ChunkZ, "NBT tag exceeds chunk bounds: Y", a_RawChunkData);
			return false;
		}

		const auto
			BlockData = GetSectionData(a_NBT, Child, "Blocks", ChunkBlockData::SectionBlockCount),
			MetaData = GetSectionData(a_NBT, Child, "Data", ChunkBlockData::SectionMetaCount),
			BlockLightData = GetSectionData(a_NBT, Child, "BlockLight", ChunkLightData::SectionLightCount),
			SkyLightData = GetSectionData(a_NBT, Child, "SkyLight", ChunkLightData::SectionLightCount);
		if ((BlockData != nullptr) && (MetaData != nullptr) && (SkyLightData != nullptr) && (BlockLightData != nullptr))
		{
			Data.BlockData.SetSection(*reinterpret_cast<const ChunkBlockData::SectionType *>(BlockData), *reinterpret_cast<const ChunkBlockData::SectionMetaType *>(MetaData), static_cast<size_t>(Y));
			Data.LightData.SetSection(*reinterpret_cast<const ChunkLightData::SectionType *>(BlockLightData), *reinterpret_cast<const ChunkLightData::SectionType *>(SkyLightData), static_cast<size_t>(Y));
		}
		else
		{
			ChunkLoadFailed(a_Chunk.m_ChunkX, a_Chunk.m_ChunkZ, "Missing chunk block/light data", a_RawChunkData);
			return false;
		}
	}  // for itr - LevelSections[]

	// Load the biomes from NBT, if present and valid:
	if (!LoadBiomeMapFromNBT(Data.BiomeMap, a_NBT, a_NBT.FindChildByName(Level, "Biomes")))
	{
		ChunkLoadFailed(a_Chunk.m_ChunkX, a_Chunk.m_ChunkZ, "Missing chunk biome data", a_RawChunkData);
		return false;
	}

	// Height map too:
	if (!LoadHeightMapFromNBT(Data.HeightMap, a_NBT, a_NBT.FindChildByName(Level, "HeightMap")))
	{
		ChunkLoadFailed(a_Chunk.m_ChunkX, a_Chunk.m_ChunkZ, "Missing chunk height data", a_RawChunkData);
		return false;
	}

	// Load the entities from NBT:
	LoadEntitiesFromNBT     (Data.Entities,      a_NBT, a_NBT.FindChildByName(Level, "Entities"));
	LoadBlockEntitiesFromNBT(Data.BlockEntities, a_NBT, a_NBT.FindChildByName(Level, "TileEntities"), Data.BlockData);

	Data.IsLightValid = (a_NBT.FindChildByName(Level, "MCSIsLightValid") > 0);

	/*
	// Uncomment this block for really cool stuff :)
	// DEBUG magic: Invert the underground, so that we can see the MC generator in action :)
	bool ShouldInvert[cChunkDef::Width * cChunkDef::Width];
	memset(ShouldInvert, 0, sizeof(ShouldInvert));
	for (int y = cChunkDef::UpperLimit - 1; y >= cChunkDef::LowerLimit; y--)
	{
		for (int x = 0; x < cChunkDef::Width; x++) for (int z = 0; z < cChunkDef::Width; z++)
		{
			const auto Index = cChunkDef::MakeIndex(x, y, z);
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

	m_World->QueueSetChunkData(std::move(Data));
	return true;
}





bool cWSSAnvil::LoadBiomeMapFromNBT(cChunkDef::BiomeMap & a_BiomeMap, const cParsedNBT & a_NBT, const int a_TagIdx)
{
	if (
		(a_TagIdx < 0) ||
		(a_NBT.GetType(a_TagIdx) != TAG_ByteArray) ||
		(a_NBT.GetDataLength(a_TagIdx) != std::size(a_BiomeMap))
	)
	{
		return false;
	}

	const auto * const BiomeData = a_NBT.GetData(a_TagIdx);
	for (size_t i = 0; i < ARRAYCOUNT(a_BiomeMap); i++)
	{
		if (BiomeData[i] > std::byte(EMCSBiome::biMaxVariantBiome))
		{
			// Unassigned biomes:
			return false;
		}

		a_BiomeMap[i] = static_cast<EMCSBiome>(BiomeData[i]);
	}

	return true;
}





bool cWSSAnvil::LoadHeightMapFromNBT(cChunkDef::HeightMap & a_HeightMap, const cParsedNBT & a_NBT, const int a_TagIdx)
{
	if (
		(a_TagIdx < 0) ||
		(a_NBT.GetType(a_TagIdx) != TAG_IntArray) ||
		(a_NBT.GetDataLength(a_TagIdx) != (4 * std::size(a_HeightMap)))
	)
	{
		return false;
	}

	const auto * const HeightData = a_NBT.GetData(a_TagIdx);
	for (int RelZ = 0; RelZ < cChunkDef::Width; RelZ++)
	{
		for (int RelX = 0; RelX < cChunkDef::Width; RelX++)
		{
			const int Index = 4 * (RelX + RelZ * cChunkDef::Width);
			const int Height = GetBEInt(HeightData + Index);

			if (Height > std::numeric_limits<HEIGHTTYPE>::max())
			{
				// Invalid data:
				return false;
			}

			cChunkDef::SetHeight(a_HeightMap, RelX, RelZ, static_cast<HEIGHTTYPE>(Height));
		}
	}

	return true;
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

		try
		{
			LoadEntityFromNBT(a_Entities, a_NBT, Child, a_NBT.GetStringView(sID));
		}
		catch (...)
		{
			continue;
		}
	}  // for Child - a_NBT[]
}





void cWSSAnvil::LoadBlockEntitiesFromNBT(cBlockEntities & a_BlockEntities, const cParsedNBT & a_NBT, int a_TagIdx, const ChunkBlockData & a_BlockData)
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
		Vector3i absPos;
		if (!GetBlockEntityNBTPos(a_NBT, Child, absPos) || (absPos.y < cChunkDef::LowerLimit) || (absPos.y >= cChunkDef::UpperLimit))
		{
			LOGWARNING("Bad block entity, missing the coords. Will be ignored.");
			continue;
		}
		const auto relPos = cChunkDef::AbsoluteToRelative(absPos);

		// Load the proper BlockEntity type based on the block type:
		const auto BlockType = a_BlockData.GetBlock(relPos);
		const auto BlockMeta = a_BlockData.GetMeta(relPos);
		OwnedBlockEntity Entity;

		try
		{
			Entity = LoadBlockEntityFromNBT(a_NBT, Child, absPos, BlockType, BlockMeta);
		}
		catch (...)
		{
			continue;
		}

		// TODO: exception-ify the failure case
		if (Entity == nullptr)
		{
			continue;
		}

		// Index computed before Entity moved.
		const auto Index = cChunkDef::MakeIndex(Entity->GetRelPos());

		// Add the BlockEntity to the loaded data:
		a_BlockEntities.emplace(Index, std::move(Entity));
	}  // for Child - tag children
}





OwnedBlockEntity cWSSAnvil::LoadBlockEntityFromNBT(const cParsedNBT & a_NBT, int a_Tag, Vector3i a_Pos, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta)
{
	ASSERT(cChunkDef::IsValidHeight(a_Pos));

	// Load the specific BlockEntity type:
	switch (a_BlockType)
	{
		// Banners:
		case E_BLOCK_STANDING_BANNER:
		case E_BLOCK_WALL_BANNER:        return LoadBannerFromNBT          (a_NBT, a_Tag, a_BlockType, a_BlockMeta, a_Pos);

		// Others:
		case E_BLOCK_BEACON:             return LoadBeaconFromNBT          (a_NBT, a_Tag, a_BlockType, a_BlockMeta, a_Pos);
		case E_BLOCK_BED:                return LoadBedFromNBT             (a_NBT, a_Tag, a_BlockType, a_BlockMeta, a_Pos);
		case E_BLOCK_BREWING_STAND:      return LoadBrewingstandFromNBT    (a_NBT, a_Tag, a_BlockType, a_BlockMeta, a_Pos);
		case E_BLOCK_CHEST:              return LoadChestFromNBT           (a_NBT, a_Tag, a_BlockType, a_BlockMeta, a_Pos);
		case E_BLOCK_COMMAND_BLOCK:      return LoadCommandBlockFromNBT    (a_NBT, a_Tag, a_BlockType, a_BlockMeta, a_Pos);
		case E_BLOCK_DISPENSER:          return LoadDispenserFromNBT       (a_NBT, a_Tag, a_BlockType, a_BlockMeta, a_Pos);
		case E_BLOCK_DROPPER:            return LoadDropperFromNBT         (a_NBT, a_Tag, a_BlockType, a_BlockMeta, a_Pos);
		case E_BLOCK_ENCHANTMENT_TABLE:  return LoadEnchantingTableFromNBT (a_NBT, a_Tag, a_BlockType, a_BlockMeta, a_Pos);
		case E_BLOCK_ENDER_CHEST:        return LoadEnderChestFromNBT      (a_NBT, a_Tag, a_BlockType, a_BlockMeta, a_Pos);
		case E_BLOCK_END_PORTAL:         return LoadEndPortalFromNBT       (a_NBT, a_Tag, a_BlockType, a_BlockMeta, a_Pos);
		case E_BLOCK_FLOWER_POT:         return LoadFlowerPotFromNBT       (a_NBT, a_Tag, a_BlockType, a_BlockMeta, a_Pos);
		case E_BLOCK_FURNACE:            return LoadFurnaceFromNBT         (a_NBT, a_Tag, a_BlockType, a_BlockMeta, a_Pos);
		case E_BLOCK_HEAD:               return LoadMobHeadFromNBT         (a_NBT, a_Tag, a_BlockType, a_BlockMeta, a_Pos);
		case E_BLOCK_HOPPER:             return LoadHopperFromNBT          (a_NBT, a_Tag, a_BlockType, a_BlockMeta, a_Pos);
		case E_BLOCK_JUKEBOX:            return LoadJukeboxFromNBT         (a_NBT, a_Tag, a_BlockType, a_BlockMeta, a_Pos);
		case E_BLOCK_LIT_FURNACE:        return LoadFurnaceFromNBT         (a_NBT, a_Tag, a_BlockType, a_BlockMeta, a_Pos);
		case E_BLOCK_MOB_SPAWNER:        return LoadMobSpawnerFromNBT      (a_NBT, a_Tag, a_BlockType, a_BlockMeta, a_Pos);
		case E_BLOCK_NOTE_BLOCK:         return LoadNoteBlockFromNBT       (a_NBT, a_Tag, a_BlockType, a_BlockMeta, a_Pos);
		case E_BLOCK_SIGN_POST:          return LoadSignFromNBT            (a_NBT, a_Tag, a_BlockType, a_BlockMeta, a_Pos);
		case E_BLOCK_TRAPPED_CHEST:      return LoadChestFromNBT           (a_NBT, a_Tag, a_BlockType, a_BlockMeta, a_Pos);
		case E_BLOCK_WALLSIGN:           return LoadSignFromNBT            (a_NBT, a_Tag, a_BlockType, a_BlockMeta, a_Pos);
		default:
		{
			// All the other blocktypes should have no entities assigned to them. Report an error:
			// Get the "id" tag:
			int TagID = a_NBT.FindChildByName(a_Tag, "id");
			FLOGINFO("WorldLoader({0}): Block entity mismatch: block type {1} ({2}), type \"{3}\", at {4}; the entity will be lost.",
				m_World->GetName(),
				ItemTypeToString(a_BlockType), a_BlockType, (TagID >= 0) ? a_NBT.GetStringView(TagID) : "unknown",
				a_Pos
			);
			return nullptr;
		}
	}
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
			// Legacy string lore
			a_Item.m_LoreTable = StringSplit(a_NBT.GetString(Lore), "`");
		}
		else if ((Lore > 0) && (a_NBT.GetType(Lore) == TAG_List))
		{
			// Lore table
			a_Item.m_LoreTable.clear();
			for (int loretag = a_NBT.GetFirstChild(Lore); loretag >= 0; loretag = a_NBT.GetNextSibling(loretag))  // Loop through array of strings
			{
				a_Item.m_LoreTable.push_back(a_NBT.GetString(loretag));
			}
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
		cFireworkItem::ParseFromNBT(a_Item.m_FireworkItem, a_NBT, FireworksTag, static_cast<ENUM_ITEM_TYPE>(a_Item.m_ItemType));
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





AString cWSSAnvil::DecodeSignLine(const AString & a_Line)
{
	if (a_Line.empty())
	{
		return AString();
	}
	if (a_Line[0] != '{')
	{
		return a_Line;
	}

	// Try to parse the JSON:
	Json::Value root;
	if (!JsonUtils::ParseString(a_Line, root) || !root.isObject())
	{
		return a_Line;
	}
	const auto & txt = root["text"];
	if (txt.isString())
	{
		return txt.asString();
	}
	return a_Line;
}





bool cWSSAnvil::CheckBlockEntityType(const cParsedNBT & a_NBT, int a_TagIdx, const AStringVector & a_ExpectedTypes, Vector3i a_Pos)
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

	// Check if the "id" tag is a string:
	if (a_NBT.GetType(TagID) != eTagType::TAG_String)
	{
		return false;
	}

	// Compare the value:
	for (const auto & et: a_ExpectedTypes)
	{
		if (a_NBT.GetStringView(TagID) == et)
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
	FLOGWARNING("Block entity type mismatch: exp {0}, got \"{1}\". The block entity at {2} will lose all its properties.",
		expectedTypes.c_str() + 2,  // Skip the first ", " that is extra in the string
		a_NBT.GetStringView(TagID), a_Pos
	);
	return false;
}





OwnedBlockEntity cWSSAnvil::LoadBannerFromNBT(const cParsedNBT & a_NBT, int a_TagIdx, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, Vector3i a_Pos)
{
	static const AStringVector expectedTypes({"Banner", "minecraft:standingbanner","minecraft:wallbanner"});
	if (!CheckBlockEntityType(a_NBT, a_TagIdx, expectedTypes, a_Pos))
	{
		return nullptr;
	}

	// Reads base color from NBT
	int CurrentLine = a_NBT.FindChildByName(a_TagIdx, "Base");
	if (CurrentLine >= 0)
	{
		const auto Color = static_cast<unsigned char>(a_NBT.GetInt(CurrentLine));
		return std::make_unique<cBannerEntity>(a_BlockType, a_BlockMeta, a_Pos, m_World, Color);
	}

	return nullptr;
}





OwnedBlockEntity cWSSAnvil::LoadBeaconFromNBT(const cParsedNBT & a_NBT, int a_TagIdx, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, Vector3i a_Pos)
{
	// Check if the data has a proper type:
	static const AStringVector expectedTypes({"Beacon", "minecraft:beacon"});
	if (!CheckBlockEntityType(a_NBT, a_TagIdx, expectedTypes, a_Pos))
	{
		return nullptr;
	}

	auto Beacon = std::make_unique<cBeaconEntity>(a_BlockType, a_BlockMeta, a_Pos, m_World);

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

	return Beacon;
}





OwnedBlockEntity cWSSAnvil::LoadBedFromNBT(const cParsedNBT & a_NBT, int a_TagIdx, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, Vector3i a_Pos)
{
	// Check if the data has a proper type:
	static const AStringVector expectedTypes({ "Bed", "minecraft:bed" });
	if (!CheckBlockEntityType(a_NBT, a_TagIdx, expectedTypes, a_Pos))
	{
		return nullptr;
	}

	// Use color red as default
	short Color = E_META_WOOL_RED;

	int ColorIDx = a_NBT.FindChildByName(a_TagIdx, "color");
	if (ColorIDx >= 0)
	{
		Color = static_cast<short>(a_NBT.GetInt(ColorIDx));
	}

	return std::make_unique<cBedEntity>(a_BlockType, a_BlockMeta, a_Pos, m_World, Color);
}





OwnedBlockEntity cWSSAnvil::LoadBrewingstandFromNBT(const cParsedNBT & a_NBT, int a_TagIdx, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, Vector3i a_Pos)
{
	// Check if the data has a proper type:
	static const AStringVector expectedTypes({ "Brewingstand", "minecraft:brewing_stand" });
	if (!CheckBlockEntityType(a_NBT, a_TagIdx, expectedTypes, a_Pos))
	{
		return nullptr;
	}

	int Items = a_NBT.FindChildByName(a_TagIdx, "Items");
	if ((Items < 0) || (a_NBT.GetType(Items) != TAG_List))
	{
		return nullptr;  // Make it an empty brewingstand - the chunk loader will provide an empty cBrewingstandEntity for this
	}

	auto Brewingstand = std::make_unique<cBrewingstandEntity>(a_BlockType, a_BlockMeta, a_Pos, m_World);

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
	return Brewingstand;
}





OwnedBlockEntity cWSSAnvil::LoadChestFromNBT(const cParsedNBT & a_NBT, int a_TagIdx, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, Vector3i a_Pos)
{
	// Check if the data has a proper type:
	// Note that older Cuberite code used "TrappedChest" for trapped chests; new code mimics vanilla and uses "Chest" throughout, but we allow migration here:
	static const AStringVector expectedTypes({ "Chest", "TrappedChest", "minecraft:chest" });
	if (!CheckBlockEntityType(a_NBT, a_TagIdx, expectedTypes, a_Pos))
	{
		return nullptr;
	}

	int Items = a_NBT.FindChildByName(a_TagIdx, "Items");
	if ((Items < 0) || (a_NBT.GetType(Items) != TAG_List))
	{
		return nullptr;  // Make it an empty chest - the chunk loader will provide an empty cChestEntity for this
	}
	auto Chest = std::make_unique<cChestEntity>(a_BlockType, a_BlockMeta, a_Pos, m_World);
	LoadItemGridFromNBT(Chest->GetContents(), a_NBT, Items);
	return Chest;
}





OwnedBlockEntity cWSSAnvil::LoadCommandBlockFromNBT(const cParsedNBT & a_NBT, int a_TagIdx, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, Vector3i a_Pos)
{
	// Check if the data has a proper type:
	static const AStringVector expectedTypes({ "Control", "minecraft:command_block" });
	if (!CheckBlockEntityType(a_NBT, a_TagIdx, expectedTypes, a_Pos))
	{
		return nullptr;
	}

	auto CmdBlock = std::make_unique<cCommandBlockEntity>(a_BlockType, a_BlockMeta, a_Pos, m_World);

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

	return CmdBlock;
}





OwnedBlockEntity cWSSAnvil::LoadDispenserFromNBT(const cParsedNBT & a_NBT, int a_TagIdx, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, Vector3i a_Pos)
{
	// Check if the data has a proper type:
	static const AStringVector expectedTypes({ "Trap", "minecraft:dispenser" });
	if (!CheckBlockEntityType(a_NBT, a_TagIdx, expectedTypes, a_Pos))
	{
		return nullptr;
	}

	int Items = a_NBT.FindChildByName(a_TagIdx, "Items");
	if ((Items < 0) || (a_NBT.GetType(Items) != TAG_List))
	{
		return nullptr;  // Make it an empty dispenser - the chunk loader will provide an empty cDispenserEntity for this
	}
	auto Dispenser = std::make_unique<cDispenserEntity>(a_BlockType, a_BlockMeta, a_Pos, m_World);
	LoadItemGridFromNBT(Dispenser->GetContents(), a_NBT, Items);
	return Dispenser;
}





OwnedBlockEntity cWSSAnvil::LoadDropperFromNBT(const cParsedNBT & a_NBT, int a_TagIdx, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, Vector3i a_Pos)
{
	// Check if the data has a proper type:
	static const AStringVector expectedTypes({ "Dropper", "minecraft:dropper" });
	if (!CheckBlockEntityType(a_NBT, a_TagIdx, expectedTypes, a_Pos))
	{
		return nullptr;
	}

	int Items = a_NBT.FindChildByName(a_TagIdx, "Items");
	if ((Items < 0) || (a_NBT.GetType(Items) != TAG_List))
	{
		return nullptr;  // Make it an empty dropper - the chunk loader will provide an empty cDropperEntity for this
	}
	auto Dropper = std::make_unique<cDropperEntity>(a_BlockType, a_BlockMeta, a_Pos, m_World);
	LoadItemGridFromNBT(Dropper->GetContents(), a_NBT, Items);
	return Dropper;
}





OwnedBlockEntity cWSSAnvil::LoadEnchantingTableFromNBT(const cParsedNBT & a_NBT, int a_TagIdx, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, Vector3i a_Pos)
{
	// Check if the data has a proper type:
	static const AStringVector expectedTypes({ "EnchantingTable", "minecraft:enchanting_table" });
	if (!CheckBlockEntityType(a_NBT, a_TagIdx, expectedTypes, a_Pos))
	{
		return nullptr;
	}

	AString CustomName;
	int currentLine = a_NBT.FindChildByName(a_TagIdx, "CustomName");
	if (currentLine >= 0)
	{
		if (a_NBT.GetType(currentLine) == TAG_String)
		{
			CustomName = a_NBT.GetString(currentLine);
		}
	}
	return std::make_unique<cEnchantingTableEntity>(a_BlockType, a_BlockMeta, a_Pos, m_World, CustomName);
}





OwnedBlockEntity cWSSAnvil::LoadEnderChestFromNBT(const cParsedNBT & a_NBT, int a_TagIdx, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, Vector3i a_Pos)
{
	// Check if the data has a proper type:
	static const AStringVector expectedTypes({ "EnderChest", "minecraft:ender_chest" });
	if (!CheckBlockEntityType(a_NBT, a_TagIdx, expectedTypes, a_Pos))
	{
		return nullptr;
	}
	return std::make_unique<cEnderChestEntity>(a_BlockType, a_BlockMeta, a_Pos, m_World);
}





OwnedBlockEntity cWSSAnvil::LoadEndPortalFromNBT(const cParsedNBT & a_NBT, int a_TagIdx, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, Vector3i a_Pos)
{
	// Check if the data has a proper type:
	static const AStringVector expectedTypes({ "EndPortal", "minecraft:end_portal" });
	if (!CheckBlockEntityType(a_NBT, a_TagIdx, expectedTypes, a_Pos))
	{
		return nullptr;
	}
	return std::make_unique<cEndPortalEntity>(a_BlockType, a_BlockMeta, a_Pos, m_World);
}





OwnedBlockEntity cWSSAnvil::LoadFlowerPotFromNBT(const cParsedNBT & a_NBT, int a_TagIdx, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, Vector3i a_Pos)
{
	// Check if the data has a proper type:
	static const AStringVector expectedTypes({ "FlowerPot", "minecraft:flower_pot" });
	if (!CheckBlockEntityType(a_NBT, a_TagIdx, expectedTypes, a_Pos))
	{
		return nullptr;
	}

	auto FlowerPot = std::make_unique<cFlowerPotEntity>(a_BlockType, a_BlockMeta, a_Pos, m_World);
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
	return FlowerPot;
}





OwnedBlockEntity cWSSAnvil::LoadFurnaceFromNBT(const cParsedNBT & a_NBT, int a_TagIdx, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, Vector3i a_Pos)
{
	// Check if the data has a proper type:
	static const AStringVector expectedTypes({ "Furnace", "minecraft:furnace" });
	if (!CheckBlockEntityType(a_NBT, a_TagIdx, expectedTypes, a_Pos))
	{
		return nullptr;
	}

	int Items = a_NBT.FindChildByName(a_TagIdx, "Items");
	if ((Items < 0) || (a_NBT.GetType(Items) != TAG_List))
	{
		return nullptr;  // Make it an empty furnace - the chunk loader will provide an empty cFurnaceEntity for this
	}

	auto Furnace = std::make_unique<cFurnaceEntity>(a_BlockType, a_BlockMeta, a_Pos, m_World);
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
	return Furnace;
}





OwnedBlockEntity cWSSAnvil::LoadHopperFromNBT(const cParsedNBT & a_NBT, int a_TagIdx, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, Vector3i a_Pos)
{
	// Check if the data has a proper type:
	static const AStringVector expectedTypes({ "Hopper", "minecraft:hopper" });
	if (!CheckBlockEntityType(a_NBT, a_TagIdx, expectedTypes, a_Pos))
	{
		return nullptr;
	}

	int Items = a_NBT.FindChildByName(a_TagIdx, "Items");
	if ((Items < 0) || (a_NBT.GetType(Items) != TAG_List))
	{
		return nullptr;  // Make it an empty hopper - the chunk loader will provide an empty cHopperEntity for this
	}
	auto Hopper = std::make_unique<cHopperEntity>(a_BlockType, a_BlockMeta, a_Pos, m_World);
	LoadItemGridFromNBT(Hopper->GetContents(), a_NBT, Items);
	return Hopper;
}





OwnedBlockEntity cWSSAnvil::LoadJukeboxFromNBT(const cParsedNBT & a_NBT, int a_TagIdx, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, Vector3i a_Pos)
{
	// Check if the data has a proper type:
	static const AStringVector expectedTypes({ "RecordPlayer", "minecraft:jukebox" });
	if (!CheckBlockEntityType(a_NBT, a_TagIdx, expectedTypes, a_Pos))
	{
		return nullptr;
	}

	auto Jukebox = std::make_unique<cJukeboxEntity>(a_BlockType, a_BlockMeta, a_Pos, m_World);
	int Record = a_NBT.FindChildByName(a_TagIdx, "Record");
	if (Record >= 0)
	{
		Jukebox->SetRecord(a_NBT.GetInt(Record));
	}
	return Jukebox;
}





OwnedBlockEntity cWSSAnvil::LoadMobSpawnerFromNBT(const cParsedNBT & a_NBT, int a_TagIdx, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, Vector3i a_Pos)
{
	// Check if the data has a proper type:
	static const AStringVector expectedTypes({ "MobSpawner", "minecraft:mob_spawner" });
	if (!CheckBlockEntityType(a_NBT, a_TagIdx, expectedTypes, a_Pos))
	{
		return nullptr;
	}

	auto MobSpawner = std::make_unique<cMobSpawnerEntity>(a_BlockType, a_BlockMeta, a_Pos, m_World);

	// Load entity type
	int Type = a_NBT.FindChildByName(a_TagIdx, "EntityId");
	if ((Type >= 0) && (a_NBT.GetType(Type) == TAG_String))
	{
		const auto StatInfo = NamespaceSerializer::SplitNamespacedID(a_NBT.GetStringView(Type));
		if (StatInfo.first == NamespaceSerializer::Namespace::Unknown)
		{
			return nullptr;
		}

		eMonsterType MonsterType = NamespaceSerializer::ToMonsterType(StatInfo.second);
		if (MonsterType != eMonsterType::mtInvalidType)
		{
			MobSpawner->SetEntity(MonsterType);
		}
	}

	// Load spawn count:
	int CurrentLine = a_NBT.FindChildByName(a_TagIdx, "SpawnCount");
	if ((CurrentLine >= 0) && (a_NBT.GetType(CurrentLine) == TAG_Short))
	{
		MobSpawner->SetSpawnCount(a_NBT.GetShort(CurrentLine));
	}

	// Load spawn range:
	CurrentLine = a_NBT.FindChildByName(a_TagIdx, "SpawnRange");
	if ((CurrentLine >= 0) && (a_NBT.GetType(CurrentLine) == TAG_Short))
	{
		MobSpawner->SetSpawnRange(a_NBT.GetShort(CurrentLine));
	}

	// Load delay:
	CurrentLine = a_NBT.FindChildByName(a_TagIdx, "Delay");
	if ((CurrentLine >= 0) && (a_NBT.GetType(CurrentLine) == TAG_Short))
	{
		MobSpawner->SetSpawnDelay(a_NBT.GetShort(CurrentLine));
	}

	// Load delay range:
	CurrentLine = a_NBT.FindChildByName(a_TagIdx, "MinSpawnDelay");
	if ((CurrentLine >= 0) && (a_NBT.GetType(CurrentLine) == TAG_Short))
	{
		MobSpawner->SetMinSpawnDelay(a_NBT.GetShort(CurrentLine));
	}

	CurrentLine = a_NBT.FindChildByName(a_TagIdx, "MaxSpawnDelay");
	if ((CurrentLine >= 0) && (a_NBT.GetType(CurrentLine) == TAG_Short))
	{
		MobSpawner->SetMaxSpawnDelay(a_NBT.GetShort(CurrentLine));
	}

	// Load MaxNearbyEntities:
	CurrentLine = a_NBT.FindChildByName(a_TagIdx, "MaxNearbyEntities");
	if ((CurrentLine >= 0) && (a_NBT.GetType(CurrentLine) == TAG_Short))
	{
		MobSpawner->SetMaxNearbyEntities(a_NBT.GetShort(CurrentLine));
	}

	// Load RequiredPlayerRange:
	CurrentLine = a_NBT.FindChildByName(a_TagIdx, "RequiredPlayerRange");
	if ((CurrentLine >= 0) && (a_NBT.GetType(CurrentLine) == TAG_Short))
	{
		MobSpawner->SetRequiredPlayerRange(a_NBT.GetShort(CurrentLine));
	}


	return MobSpawner;
}





OwnedBlockEntity cWSSAnvil::LoadMobHeadFromNBT(const cParsedNBT & a_NBT, int a_TagIdx, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, Vector3i a_Pos)
{
	// Check if the data has a proper type:
	static const AStringVector expectedTypes({ "Skull", "minecraft:skull" });
	if (!CheckBlockEntityType(a_NBT, a_TagIdx, expectedTypes, a_Pos))
	{
		return nullptr;
	}

	auto MobHead = std::make_unique<cMobHeadEntity>(a_BlockType, a_BlockMeta, a_Pos, m_World);

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
		AString OwnerName, OwnerTexture, OwnerTextureSignature;
		cUUID OwnerUUID;

		currentLine = a_NBT.FindChildByName(ownerLine, "Id");
		if (currentLine >= 0)
		{
			OwnerUUID.FromString(a_NBT.GetString(currentLine));
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

	return MobHead;
}





OwnedBlockEntity cWSSAnvil::LoadNoteBlockFromNBT(const cParsedNBT & a_NBT, int a_TagIdx, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, Vector3i a_Pos)
{
	// Check if the data has a proper type:
	static const AStringVector expectedTypes({ "Music", "minecraft:noteblock" });
	if (!CheckBlockEntityType(a_NBT, a_TagIdx, expectedTypes, a_Pos))
	{
		return nullptr;
	}

	auto NoteBlock = std::make_unique<cNoteEntity>(a_BlockType, a_BlockMeta, a_Pos, m_World);
	int note = a_NBT.FindChildByName(a_TagIdx, "note");
	if (note >= 0)
	{
		NoteBlock->SetNote(a_NBT.GetByte(note));
	}
	return NoteBlock;
}





OwnedBlockEntity cWSSAnvil::LoadSignFromNBT(const cParsedNBT & a_NBT, int a_TagIdx, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, Vector3i a_Pos)
{
	// Check if the data has a proper type:
	static const AStringVector expectedTypes({ "Sign", "minecraft:sign" });
	if (!CheckBlockEntityType(a_NBT, a_TagIdx, expectedTypes, a_Pos))
	{
		return nullptr;
	}

	auto Sign = std::make_unique<cSignEntity>(a_BlockType, a_BlockMeta, a_Pos, m_World);

	int currentLine = a_NBT.FindChildByName(a_TagIdx, "Text1");
	if (currentLine >= 0)
	{
		Sign->SetLine(0, DecodeSignLine(a_NBT.GetString(currentLine)));
	}

	currentLine = a_NBT.FindChildByName(a_TagIdx, "Text2");
	if (currentLine >= 0)
	{
		Sign->SetLine(1, DecodeSignLine(a_NBT.GetString(currentLine)));
	}

	currentLine = a_NBT.FindChildByName(a_TagIdx, "Text3");
	if (currentLine >= 0)
	{
		Sign->SetLine(2, DecodeSignLine(a_NBT.GetString(currentLine)));
	}

	currentLine = a_NBT.FindChildByName(a_TagIdx, "Text4");
	if (currentLine >= 0)
	{
		Sign->SetLine(3, DecodeSignLine(a_NBT.GetString(currentLine)));
	}

	return Sign;
}





void cWSSAnvil::LoadEntityFromNBT(cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_EntityTagIdx, const std::string_view a_EntityName)
{
	typedef void (cWSSAnvil::*EntityLoaderFunc)(cEntityList &, const cParsedNBT &, int a_EntityTagIdx);
	typedef std::map<std::string_view, EntityLoaderFunc> EntityLoaderMap;
	static const EntityLoaderMap EntityTypeToFunction
	{
		{ "Boat",                          &cWSSAnvil::LoadBoatFromNBT },
		{ "minecraft:boat",                &cWSSAnvil::LoadBoatFromNBT },
		{ "EnderCrystal",                  &cWSSAnvil::LoadEnderCrystalFromNBT },
		{ "minecraft:ender_crystal",       &cWSSAnvil::LoadEnderCrystalFromNBT },
		{ "FallingBlock",                  &cWSSAnvil::LoadFallingBlockFromNBT },
		{ "minecraft:falling_block",       &cWSSAnvil::LoadFallingBlockFromNBT },
		{ "Minecart",                      &cWSSAnvil::LoadOldMinecartFromNBT },
		{ "MinecartChest",                 &cWSSAnvil::LoadMinecartCFromNBT },
		{ "minecraft:chest_minecart",      &cWSSAnvil::LoadMinecartCFromNBT },
		{ "MinecartFurnace",               &cWSSAnvil::LoadMinecartFFromNBT },
		{ "minecraft:furnace_minecart",    &cWSSAnvil::LoadMinecartFFromNBT },
		{ "MinecartTNT",                   &cWSSAnvil::LoadMinecartTFromNBT },
		{ "minecraft:tnt_minecart",        &cWSSAnvil::LoadMinecartTFromNBT },
		{ "MinecartHopper",                &cWSSAnvil::LoadMinecartHFromNBT },
		{ "minecraft:hopper_minecart",     &cWSSAnvil::LoadMinecartHFromNBT },
		{ "MinecartRideable",              &cWSSAnvil::LoadMinecartRFromNBT },
		{ "minecraft:minecart",            &cWSSAnvil::LoadMinecartRFromNBT },
		{ "Item",                          &cWSSAnvil::LoadPickupFromNBT },
		{ "minecraft:item",                &cWSSAnvil::LoadPickupFromNBT },
		{ "Painting",                      &cWSSAnvil::LoadPaintingFromNBT },
		{ "minecraft:painting",            &cWSSAnvil::LoadPaintingFromNBT },
		{ "PrimedTnt",                     &cWSSAnvil::LoadTNTFromNBT },
		{ "minecraft:tnt",                 &cWSSAnvil::LoadTNTFromNBT },
		{ "XPOrb",                         &cWSSAnvil::LoadExpOrbFromNBT },
		{ "minecraft:xp_orb",              &cWSSAnvil::LoadExpOrbFromNBT },
		{ "ItemFrame",                     &cWSSAnvil::LoadItemFrameFromNBT },
		{ "minecraft:item_frame",          &cWSSAnvil::LoadItemFrameFromNBT },
		{ "LeashKnot",                     &cWSSAnvil::LoadLeashKnotFromNBT },
		{ "minecraft:leash_knot",          &cWSSAnvil::LoadLeashKnotFromNBT },
		{ "Arrow",                         &cWSSAnvil::LoadArrowFromNBT },
		{ "minecraft:arrow",               &cWSSAnvil::LoadArrowFromNBT },
		{ "SplashPotion",                  &cWSSAnvil::LoadSplashPotionFromNBT },
		{ "minecraft:potion",              &cWSSAnvil::LoadSplashPotionFromNBT },
		{ "Snowball",                      &cWSSAnvil::LoadSnowballFromNBT },
		{ "minecraft:snowball",            &cWSSAnvil::LoadSnowballFromNBT },
		{ "Egg",                           &cWSSAnvil::LoadEggFromNBT },
		{ "minecraft:egg",                 &cWSSAnvil::LoadEggFromNBT },
		{ "Fireball",                      &cWSSAnvil::LoadFireballFromNBT },
		{ "minecraft:fireball",            &cWSSAnvil::LoadFireballFromNBT },
		{ "SmallFireball",                 &cWSSAnvil::LoadFireChargeFromNBT },
		{ "minecraft:small_fireball",      &cWSSAnvil::LoadFireChargeFromNBT },
		{ "ThrownEnderpearl",              &cWSSAnvil::LoadThrownEnderpearlFromNBT },
		{ "minecraft:ender_pearl",         &cWSSAnvil::LoadThrownEnderpearlFromNBT }
	};

	// TODO: flatten monster\projectile into one entity type enum

	const auto it = EntityTypeToFunction.find(a_EntityName);
	if (it != EntityTypeToFunction.end())
	{
		(this->*it->second)(a_Entities, a_NBT, a_EntityTagIdx);
		return;
	}

	const auto StatInfo = NamespaceSerializer::SplitNamespacedID(a_EntityName);
	if (StatInfo.first == NamespaceSerializer::Namespace::Unknown)
	{
		return;
	}

	switch (NamespaceSerializer::ToMonsterType(StatInfo.second))
	{
		case mtBat:             return LoadBatFromNBT(a_Entities, a_NBT, a_EntityTagIdx);
		case mtBlaze:           return LoadBlazeFromNBT(a_Entities, a_NBT, a_EntityTagIdx);
		case mtCat:             return LoadCatFromNBT(a_Entities, a_NBT, a_EntityTagIdx);
		case mtCaveSpider:      return LoadCaveSpiderFromNBT(a_Entities, a_NBT, a_EntityTagIdx);
		case mtChicken:         return LoadChickenFromNBT(a_Entities, a_NBT, a_EntityTagIdx);
		case mtCod:             return LoadCodFromNBT(a_Entities, a_NBT, a_EntityTagIdx);
		case mtCow:             return LoadCowFromNBT(a_Entities, a_NBT, a_EntityTagIdx);
		case mtCreeper:         return LoadCreeperFromNBT(a_Entities, a_NBT, a_EntityTagIdx);
		case mtDolphin:         return LoadDolphinFromNBT(a_Entities, a_NBT, a_EntityTagIdx);
		case mtDonkey:          return LoadDonkeyFromNBT(a_Entities, a_NBT, a_EntityTagIdx);
		case mtDrowned:         return LoadDrownedFromNBT(a_Entities, a_NBT, a_EntityTagIdx);
		case mtElderGuardian:   return LoadElderGuardianFromNBT(a_Entities, a_NBT, a_EntityTagIdx);
		case mtEnderDragon:     return LoadEnderDragonFromNBT(a_Entities, a_NBT, a_EntityTagIdx);
		case mtEnderman:        return LoadEndermanFromNBT(a_Entities, a_NBT, a_EntityTagIdx);
		case mtEndermite:       return LoadEndermiteFromNBT(a_Entities, a_NBT, a_EntityTagIdx);
		case mtEvoker:          return LoadEvokerFromNBT(a_Entities, a_NBT, a_EntityTagIdx);
		case mtFox:             return LoadFoxFromNBT(a_Entities, a_NBT, a_EntityTagIdx);
		case mtGhast:           return LoadGhastFromNBT(a_Entities, a_NBT, a_EntityTagIdx);
		case mtGiant:           return LoadGiantFromNBT(a_Entities, a_NBT, a_EntityTagIdx);
		case mtGuardian:        return LoadGuardianFromNBT(a_Entities, a_NBT, a_EntityTagIdx);
		case mtHorse:           return LoadHorseFromNBT(a_Entities, a_NBT, a_EntityTagIdx);
		case mtHoglin:          return LoadHoglinFromNBT(a_Entities, a_NBT, a_EntityTagIdx);
		case mtHusk:            return LoadHuskFromNBT(a_Entities, a_NBT, a_EntityTagIdx);
		case mtIllusioner:      return LoadIllusionerFromNBT(a_Entities, a_NBT, a_EntityTagIdx);
		case mtIronGolem:       return LoadIronGolemFromNBT(a_Entities, a_NBT, a_EntityTagIdx);
		case mtLlama:           return LoadLlamaFromNBT(a_Entities, a_NBT, a_EntityTagIdx);
		case mtMagmaCube:       return LoadMagmaCubeFromNBT(a_Entities, a_NBT, a_EntityTagIdx);
		case mtMooshroom:       return LoadMooshroomFromNBT(a_Entities, a_NBT, a_EntityTagIdx);
		case mtMule:            return LoadMuleFromNBT(a_Entities, a_NBT, a_EntityTagIdx);
		case mtOcelot:          return LoadOcelotFromNBT(a_Entities, a_NBT, a_EntityTagIdx);
		case mtPanda:           return LoadPandaFromNBT(a_Entities, a_NBT, a_EntityTagIdx);
		case mtParrot:          return LoadParrotFromNBT(a_Entities, a_NBT, a_EntityTagIdx);
		case mtPhantom:         return LoadPhantomFromNBT(a_Entities, a_NBT, a_EntityTagIdx);
		case mtPig:             return LoadPigFromNBT(a_Entities, a_NBT, a_EntityTagIdx);
		case mtPiglin:          return LoadPiglinFromNBT(a_Entities, a_NBT, a_EntityTagIdx);
		case mtPiglinBrute:     return LoadPiglinBruteFromNBT(a_Entities, a_NBT, a_EntityTagIdx);
		case mtPillager:        return LoadPillagerFromNBT(a_Entities, a_NBT, a_EntityTagIdx);
		case mtPolarBear:       return LoadPolarBearFromNBT(a_Entities, a_NBT, a_EntityTagIdx);
		case mtPufferfish:      return LoadPufferfishFromNBT(a_Entities, a_NBT, a_EntityTagIdx);
		case mtRabbit:          return LoadRabbitFromNBT(a_Entities, a_NBT, a_EntityTagIdx);
		case mtRavager:         return LoadRavagerFromNBT(a_Entities, a_NBT, a_EntityTagIdx);
		case mtSalmon:          return LoadSalmonFromNBT(a_Entities, a_NBT, a_EntityTagIdx);
		case mtSheep:           return LoadSheepFromNBT(a_Entities, a_NBT, a_EntityTagIdx);
		case mtShulker:         return LoadShulkerFromNBT(a_Entities, a_NBT, a_EntityTagIdx);
		case mtSilverfish:      return LoadSilverfishFromNBT(a_Entities, a_NBT, a_EntityTagIdx);
		case mtSkeleton:        return LoadSkeletonFromNBT(a_Entities, a_NBT, a_EntityTagIdx);
		case mtSkeletonHorse:   return LoadSkeletonHorseFromNBT(a_Entities, a_NBT, a_EntityTagIdx);
		case mtSlime:           return LoadSlimeFromNBT(a_Entities, a_NBT, a_EntityTagIdx);
		case mtSnowGolem:       return LoadSnowGolemFromNBT(a_Entities, a_NBT, a_EntityTagIdx);
		case mtSpider:          return LoadSpiderFromNBT(a_Entities, a_NBT, a_EntityTagIdx);
		case mtSquid:           return LoadSquidFromNBT(a_Entities, a_NBT, a_EntityTagIdx);
		case mtStray:           return LoadStrayFromNBT(a_Entities, a_NBT, a_EntityTagIdx);
		case mtStrider:         return LoadStriderFromNBT(a_Entities, a_NBT, a_EntityTagIdx);
		case mtTraderLlama:     return LoadTraderLlamaFromNBT(a_Entities, a_NBT, a_EntityTagIdx);
		case mtTropicalFish:    return LoadTropicalFishFromNBT(a_Entities, a_NBT, a_EntityTagIdx);
		case mtTurtle:          return LoadTurtleFromNBT(a_Entities, a_NBT, a_EntityTagIdx);
		case mtVex:             return LoadVexFromNBT(a_Entities, a_NBT, a_EntityTagIdx);
		case mtVillager:        return LoadVillagerFromNBT(a_Entities, a_NBT, a_EntityTagIdx);
		case mtVindicator:      return LoadVindicatorFromNBT(a_Entities, a_NBT, a_EntityTagIdx);
		case mtWanderingTrader: return LoadWanderingTraderFromNBT(a_Entities, a_NBT, a_EntityTagIdx);
		case mtWitch:           return LoadWitchFromNBT(a_Entities, a_NBT, a_EntityTagIdx);
		case mtWither:          return LoadWitherFromNBT(a_Entities, a_NBT, a_EntityTagIdx);
		case mtWitherSkeleton:  return LoadWitherSkeletonFromNBT(a_Entities, a_NBT, a_EntityTagIdx);
		case mtWolf:            return LoadWolfFromNBT(a_Entities, a_NBT, a_EntityTagIdx);
		case mtZoglin:          return LoadZoglinFromNBT(a_Entities, a_NBT, a_EntityTagIdx);
		case mtZombie:          return LoadZombieFromNBT(a_Entities, a_NBT, a_EntityTagIdx);
		case mtZombieHorse:     return LoadZombieHorseFromNBT(a_Entities, a_NBT, a_EntityTagIdx);
		case mtZombifiedPiglin: return LoadZombifiedPiglinFromNBT(a_Entities, a_NBT, a_EntityTagIdx);
		case mtZombieVillager:  return LoadZombieVillagerFromNBT(a_Entities, a_NBT, a_EntityTagIdx);
		case mtInvalidType:     break;
	}
}





void cWSSAnvil::LoadOldMinecartFromNBT(cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx)
{
	// It is a minecart, old style, find out the type:
	int TypeTag = a_NBT.FindChildByName(a_TagIdx, "Type");
	if ((TypeTag < 0) || (a_NBT.GetType(TypeTag) != TAG_Int))
	{
		return;
	}
	int MinecartType = a_NBT.GetInt(TypeTag);
	switch (MinecartType)
	{
		case 0: LoadMinecartRFromNBT(a_Entities, a_NBT, a_TagIdx); break;  // Rideable minecart
		case 1: LoadMinecartCFromNBT(a_Entities, a_NBT, a_TagIdx); break;  // Minecart with chest
		case 2: LoadMinecartFFromNBT(a_Entities, a_NBT, a_TagIdx); break;  // Minecart with furnace
		case 3: LoadMinecartTFromNBT(a_Entities, a_NBT, a_TagIdx); break;  // Minecart with TNT
		case 4: LoadMinecartHFromNBT(a_Entities, a_NBT, a_TagIdx); break;  // Minecart with Hopper
		default: LOGWARNING("cWSSAnvil::LoadOldMinecartFromNBT: Unhandled minecart type (%d)", MinecartType); break;
	}
}





void cWSSAnvil::LoadBoatFromNBT(cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx)
{
	auto Boat = std::make_unique<cBoat>(Vector3d(), cBoat::bmOak);
	if (!LoadEntityBaseFromNBT(*Boat.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	int TypeIdx = a_NBT.FindChildByName(a_TagIdx, "Type");
	if (TypeIdx > 0)
	{
		Boat->SetMaterial(cBoat::StringToMaterial(a_NBT.GetString(TypeIdx)));
	}
	a_Entities.emplace_back(std::move(Boat));
}





void cWSSAnvil::LoadEnderCrystalFromNBT(cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx)
{
	bool DisplayBeam = false, ShowBottom = false;
	Vector3i BeamTarget;
	int CurrentLine = a_NBT.FindChildByName(a_TagIdx, "BeamTarget");
	if (CurrentLine > 0)
	{
		DisplayBeam = true;
		if (a_NBT.GetType(CurrentLine) == TAG_Compound)
		{
			int CoordinateLine = a_NBT.FindChildByName(CurrentLine, "X");
			if (CoordinateLine > 0)
			{
				BeamTarget.x = a_NBT.GetInt(CoordinateLine);
			}
			CoordinateLine = a_NBT.FindChildByName(CurrentLine, "Y");
			if (CoordinateLine > 0)
			{
				BeamTarget.y = a_NBT.GetInt(CoordinateLine);
			}
			CoordinateLine = a_NBT.FindChildByName(CurrentLine, "Z");
			if (CoordinateLine > 0)
			{
				BeamTarget.z = a_NBT.GetInt(CoordinateLine);
			}
		}
	}
	CurrentLine = a_NBT.FindChildByName(a_TagIdx, "ShowBottom");
	if (CurrentLine > 0)
	{
		ShowBottom = a_NBT.GetByte(CurrentLine) == 1;
	}

	auto EnderCrystal = std::make_unique<cEnderCrystal>(Vector3d(), BeamTarget, DisplayBeam, ShowBottom);
	if (!LoadEntityBaseFromNBT(*EnderCrystal.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	a_Entities.emplace_back(std::move(EnderCrystal));
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

	auto FallingBlock = std::make_unique<cFallingBlock>(Vector3i(0, 0, 0), Type, Meta);
	if (!LoadEntityBaseFromNBT(*FallingBlock.get(), a_NBT, a_TagIdx))
	{
		return;
	}
	a_Entities.emplace_back(std::move(FallingBlock));
}





void cWSSAnvil::LoadMinecartRFromNBT(cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx)
{
	auto Minecart = std::make_unique<cRideableMinecart>(Vector3d(), cItem(), 1);  // TODO: Load the block and the height
	if (!LoadEntityBaseFromNBT(*Minecart.get(), a_NBT, a_TagIdx))
	{
		return;
	}
	a_Entities.emplace_back(std::move(Minecart));
}





void cWSSAnvil::LoadMinecartCFromNBT(cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx)
{
	int Items = a_NBT.FindChildByName(a_TagIdx, "Items");
	if ((Items < 0) || (a_NBT.GetType(Items) != TAG_List))
	{
		return;  // Make it an empty chest - the chunk loader will provide an empty cChestEntity for this
	}
	auto Minecart = std::make_unique<cMinecartWithChest>(Vector3d());
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
	a_Entities.emplace_back(std::move(Minecart));
}





void cWSSAnvil::LoadMinecartFFromNBT(cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx)
{
	auto Minecart = std::make_unique<cMinecartWithFurnace>(Vector3d());
	if (!LoadEntityBaseFromNBT(*Minecart.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	// TODO: Load the Push and Fuel tags

	a_Entities.emplace_back(std::move(Minecart));
}





void cWSSAnvil::LoadMinecartTFromNBT(cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx)
{
	auto Minecart = std::make_unique<cMinecartWithTNT>(Vector3d());
	if (!LoadEntityBaseFromNBT(*Minecart.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	// TODO: Everything to do with TNT carts

	a_Entities.emplace_back(std::move(Minecart));
}





void cWSSAnvil::LoadMinecartHFromNBT(cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx)
{
	auto Minecart = std::make_unique<cMinecartWithHopper>(Vector3d());
	if (!LoadEntityBaseFromNBT(*Minecart.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	// TODO: Everything to do with hopper carts

	a_Entities.emplace_back(std::move(Minecart));
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

	auto Pickup = std::make_unique<cPickup>(Vector3d(), Item, false);  // Pickup delay doesn't matter, just say false
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

	a_Entities.emplace_back(std::move(Pickup));
}





void cWSSAnvil::LoadTNTFromNBT(cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx)
{
	auto TNT = std::make_unique<cTNTEntity>(Vector3d(), 0);
	if (!LoadEntityBaseFromNBT(*TNT.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	// Load Fuse Ticks:
	int FuseTicks = a_NBT.FindChildByName(a_TagIdx, "Fuse");
	if (FuseTicks > 0)
	{
		TNT->SetFuseTicks(a_NBT.GetByte(FuseTicks));
	}

	a_Entities.emplace_back(std::move(TNT));
}





void cWSSAnvil::LoadExpOrbFromNBT(cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx)
{
	auto ExpOrb = std::make_unique<cExpOrb>(Vector3d(), 0);
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

	a_Entities.emplace_back(std::move(ExpOrb));
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

	auto ItemFrame = std::make_unique<cItemFrame>(BLOCK_FACE_NONE, Vector3d());
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

	a_Entities.emplace_back(std::move(ItemFrame));
}





void cWSSAnvil::LoadLeashKnotFromNBT(cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx)
{
	auto LeashKnot = std::make_unique<cLeashKnot>(BLOCK_FACE_NONE, Vector3d());

	if (!LoadEntityBaseFromNBT(*LeashKnot.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	LoadHangingFromNBT(*LeashKnot.get(), a_NBT, a_TagIdx);

	a_Entities.emplace_back(std::move(LeashKnot));
}





void cWSSAnvil::LoadPaintingFromNBT(cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx)
{
	// Load painting name:
	int MotiveTag = a_NBT.FindChildByName(a_TagIdx, "Motive");
	if ((MotiveTag < 0) || (a_NBT.GetType(MotiveTag) != TAG_String))
	{
		return;
	}

	auto Painting = std::make_unique<cPainting>(a_NBT.GetString(MotiveTag), BLOCK_FACE_NONE, Vector3d());
	if (!LoadEntityBaseFromNBT(*Painting.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	LoadHangingFromNBT(*Painting.get(), a_NBT, a_TagIdx);
	a_Entities.emplace_back(std::move(Painting));
}





void cWSSAnvil::LoadArrowFromNBT(cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx)
{
	auto Arrow = std::make_unique<cArrowEntity>(nullptr, Vector3d(), Vector3d());
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
	a_Entities.emplace_back(std::move(Arrow));
}





void cWSSAnvil::LoadSplashPotionFromNBT(cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx)
{
	auto SplashPotion = std::make_unique<cSplashPotionEntity>(nullptr, Vector3d(), Vector3d(), cItem());
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
	a_Entities.emplace_back(std::move(SplashPotion));
}





void cWSSAnvil::LoadSnowballFromNBT(cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx)
{
	auto Snowball = std::make_unique<cThrownSnowballEntity>(nullptr, Vector3d(), Vector3d());
	if (!LoadProjectileBaseFromNBT(*Snowball.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	// Store the new snowball in the entities list:
	a_Entities.emplace_back(std::move(Snowball));
}





void cWSSAnvil::LoadEggFromNBT(cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx)
{
	auto Egg = std::make_unique<cThrownEggEntity>(nullptr, Vector3d(), Vector3d());
	if (!LoadProjectileBaseFromNBT(*Egg.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	// Store the new egg in the entities list:
	a_Entities.emplace_back(std::move(Egg));
}





void cWSSAnvil::LoadFireballFromNBT(cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx)
{
	auto Fireball = std::make_unique<cGhastFireballEntity>(nullptr, Vector3d(), Vector3d());
	if (!LoadProjectileBaseFromNBT(*Fireball.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	// Store the new fireball in the entities list:
	a_Entities.emplace_back(std::move(Fireball));
}





void cWSSAnvil::LoadFireChargeFromNBT(cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx)
{
	auto FireCharge = std::make_unique<cFireChargeEntity>(nullptr, Vector3d(), Vector3d());
	if (!LoadProjectileBaseFromNBT(*FireCharge.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	// Store the new FireCharge in the entities list:
	a_Entities.emplace_back(std::move(FireCharge));
}





void cWSSAnvil::LoadThrownEnderpearlFromNBT(cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx)
{
	auto Enderpearl = std::make_unique<cThrownEnderPearlEntity>(nullptr, Vector3d(), Vector3d());
	if (!LoadProjectileBaseFromNBT(*Enderpearl.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	// Store the new enderpearl in the entities list:
	a_Entities.emplace_back(std::move(Enderpearl));
}





void cWSSAnvil::LoadBatFromNBT(cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx)
{
	auto Monster = std::make_unique<cBat>();
	if (!LoadEntityBaseFromNBT(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	if (!LoadMonsterBaseFromNBT(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	a_Entities.emplace_back(std::move(Monster));
}





void cWSSAnvil::LoadBlazeFromNBT(cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx)
{
	std::unique_ptr<cBlaze> Monster = std::make_unique<cBlaze>();
	if (!LoadEntityBaseFromNBT(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	if (!LoadMonsterBaseFromNBT(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	a_Entities.emplace_back(std::move(Monster));
}





void cWSSAnvil::LoadCatFromNBT(cEntityList &a_Entities, const cParsedNBT &a_NBT, int a_TagIdx)
{
	// TODO
}





void cWSSAnvil::LoadCaveSpiderFromNBT(cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx)
{
	std::unique_ptr<cCaveSpider> Monster = std::make_unique<cCaveSpider>();
	if (!LoadEntityBaseFromNBT(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	if (!LoadMonsterBaseFromNBT(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	a_Entities.emplace_back(std::move(Monster));
}





void cWSSAnvil::LoadChickenFromNBT(cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx)
{
	std::unique_ptr<cChicken> Monster = std::make_unique<cChicken>();
	if (!LoadEntityBaseFromNBT(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	if (!LoadMonsterBaseFromNBT(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	a_Entities.emplace_back(std::move(Monster));
}





void cWSSAnvil::LoadCodFromNBT(cEntityList &a_Entities, const cParsedNBT &a_NBT, int a_TagIdx)
{
	// TODO
}





void cWSSAnvil::LoadCowFromNBT(cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx)
{
	std::unique_ptr<cCow> Monster = std::make_unique<cCow>();
	if (!LoadEntityBaseFromNBT(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	if (!LoadMonsterBaseFromNBT(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	a_Entities.emplace_back(std::move(Monster));
}





void cWSSAnvil::LoadCreeperFromNBT(cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx)
{
	std::unique_ptr<cCreeper> Monster = std::make_unique<cCreeper>();
	if (!LoadEntityBaseFromNBT(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	if (!LoadMonsterBaseFromNBT(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	a_Entities.emplace_back(std::move(Monster));
}





void cWSSAnvil::LoadDolphinFromNBT(cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx)
{
	// TODO
}





void cWSSAnvil::LoadDonkeyFromNBT(cEntityList &a_Entities, const cParsedNBT &a_NBT, int a_TagIdx)
{
	// TODO
}





void cWSSAnvil::LoadDrownedFromNBT(cEntityList &a_Entities, const cParsedNBT &a_NBT, int a_TagIdx)
{
	// TODO
}





void cWSSAnvil::LoadElderGuardianFromNBT(cEntityList &a_Entities, const cParsedNBT &a_NBT, int a_TagIdx)
{
	// TODO
}





void cWSSAnvil::LoadEnderDragonFromNBT(cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx)
{
	std::unique_ptr<cEnderDragon> Monster = std::make_unique<cEnderDragon>();
	if (!LoadEntityBaseFromNBT(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	if (!LoadMonsterBaseFromNBT(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	a_Entities.emplace_back(std::move(Monster));
}





void cWSSAnvil::LoadEndermanFromNBT(cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx)
{
	std::unique_ptr<cEnderman> Monster = std::make_unique<cEnderman>();
	if (!LoadEntityBaseFromNBT(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	if (!LoadMonsterBaseFromNBT(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	a_Entities.emplace_back(std::move(Monster));
}





void cWSSAnvil::LoadEndermiteFromNBT(cEntityList &a_Entities, const cParsedNBT &a_NBT, int a_TagIdx)
{
	// TODO
}





void cWSSAnvil::LoadEvokerFromNBT(cEntityList &a_Entities, const cParsedNBT &a_NBT, int a_TagIdx)
{
	// TODO
}





void cWSSAnvil::LoadFoxFromNBT(cEntityList &a_Entities, const cParsedNBT &a_NBT, int a_TagIdx)
{
	// TODO
}





void cWSSAnvil::LoadGhastFromNBT(cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx)
{
	std::unique_ptr<cGhast> Monster = std::make_unique<cGhast>();
	if (!LoadEntityBaseFromNBT(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	if (!LoadMonsterBaseFromNBT(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	a_Entities.emplace_back(std::move(Monster));
}





void cWSSAnvil::LoadGiantFromNBT(cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx)
{
	std::unique_ptr<cGiant> Monster = std::make_unique<cGiant>();
	if (!LoadEntityBaseFromNBT(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	if (!LoadMonsterBaseFromNBT(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	a_Entities.emplace_back(std::move(Monster));
}





void cWSSAnvil::LoadGuardianFromNBT(cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx)
{
	std::unique_ptr<cGuardian> Monster = std::make_unique<cGuardian>();
	if (!LoadEntityBaseFromNBT(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	if (!LoadMonsterBaseFromNBT(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	a_Entities.emplace_back(std::move(Monster));
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

	std::unique_ptr<cHorse> Monster = std::make_unique<cHorse>(Type, Color, Style, 1);

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

	a_Entities.emplace_back(std::move(Monster));
}





void cWSSAnvil::LoadHoglinFromNBT(cEntityList &a_Entities, const cParsedNBT &a_NBT, int a_TagIdx)
{
	// TODO
}





void cWSSAnvil::LoadHuskFromNBT(cEntityList &a_Entities, const cParsedNBT &a_NBT, int a_TagIdx)
{
	// TODO
}





void cWSSAnvil::LoadIllusionerFromNBT(cEntityList &a_Entities, const cParsedNBT &a_NBT, int a_TagIdx)
{
	// TODO
}





void cWSSAnvil::LoadIronGolemFromNBT(cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx)
{
	std::unique_ptr<cIronGolem> Monster = std::make_unique<cIronGolem>();
	if (!LoadEntityBaseFromNBT(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	if (!LoadMonsterBaseFromNBT(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	a_Entities.emplace_back(std::move(Monster));
}





void cWSSAnvil::LoadLlamaFromNBT(cEntityList &a_Entities, const cParsedNBT &a_NBT, int a_TagIdx)
{
	// TODO
}





void cWSSAnvil::LoadMagmaCubeFromNBT(cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx)
{
	int SizeIdx = a_NBT.FindChildByName(a_TagIdx, "Size");

	if (SizeIdx < 0)
	{
		return;
	}

	int Size = a_NBT.GetInt(SizeIdx);

	std::unique_ptr<cMagmaCube> Monster = std::make_unique<cMagmaCube>(Size);
	if (!LoadEntityBaseFromNBT(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	if (!LoadMonsterBaseFromNBT(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	a_Entities.emplace_back(std::move(Monster));
}





void cWSSAnvil::LoadMooshroomFromNBT(cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx)
{
	std::unique_ptr<cMooshroom> Monster = std::make_unique<cMooshroom>();
	if (!LoadEntityBaseFromNBT(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	if (!LoadMonsterBaseFromNBT(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	a_Entities.emplace_back(std::move(Monster));
}





void cWSSAnvil::LoadMuleFromNBT(cEntityList &a_Entities, const cParsedNBT &a_NBT, int a_TagIdx)
{
	// TODO
}





void cWSSAnvil::LoadOcelotFromNBT(cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx)
{
	std::unique_ptr<cOcelot> Monster = std::make_unique<cOcelot>();
	if (!LoadEntityBaseFromNBT(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	if (!LoadMonsterBaseFromNBT(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	auto OwnerInfo = LoadEntityOwner(a_NBT, a_TagIdx);
	if (!OwnerInfo.first.empty() && !OwnerInfo.second.IsNil())
	{
		Monster->SetOwner(OwnerInfo.first, OwnerInfo.second);
		Monster->SetIsTame(true);
	}

	int TypeIdx  = a_NBT.FindChildByName(a_TagIdx, "CatType");
	if (TypeIdx > 0)
	{
		int Type = a_NBT.GetInt(TypeIdx);
		Monster->SetCatType(static_cast<cOcelot::eCatType>(Type));
	}

	int SittingIdx = a_NBT.FindChildByName(a_TagIdx, "Sitting");
	if ((SittingIdx > 0) && (a_NBT.GetType(SittingIdx) == TAG_Byte))
	{
		bool Sitting = (a_NBT.GetByte(SittingIdx) == 1);
		Monster->SetIsSitting(Sitting);
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

	a_Entities.emplace_back(std::move(Monster));
}





void cWSSAnvil::LoadPandaFromNBT(cEntityList &a_Entities, const cParsedNBT &a_NBT, int a_TagIdx)
{
	// TODO
}





void cWSSAnvil::LoadParrotFromNBT(cEntityList &a_Entities, const cParsedNBT &a_NBT, int a_TagIdx)
{
	// TODO
}





void cWSSAnvil::LoadPhantomFromNBT(cEntityList &a_Entities, const cParsedNBT &a_NBT, int a_TagIdx)
{
	// TODO
}





void cWSSAnvil::LoadPigFromNBT(cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx)
{
	std::unique_ptr<cPig> Monster = std::make_unique<cPig>();
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

	a_Entities.emplace_back(std::move(Monster));
}





void cWSSAnvil::LoadPiglinFromNBT(cEntityList &a_Entities, const cParsedNBT &a_NBT, int a_TagIdx)
{
	// TODO
}





void cWSSAnvil::LoadPiglinBruteFromNBT(cEntityList &a_Entities, const cParsedNBT &a_NBT, int a_TagIdx)
{
	// TODO
}





void cWSSAnvil::LoadPillagerFromNBT(cEntityList &a_Entities, const cParsedNBT &a_NBT, int a_TagIdx)
{
	// TODO
}





void cWSSAnvil::LoadPolarBearFromNBT(cEntityList &a_Entities, const cParsedNBT &a_NBT, int a_TagIdx)
{
	// TODO
}





void cWSSAnvil::LoadPufferfishFromNBT(cEntityList &a_Entities, const cParsedNBT &a_NBT, int a_TagIdx)
{
	// TODO
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

	std::unique_ptr<cRabbit> Monster = std::make_unique<cRabbit>(static_cast<eRabbitType>(Type), MoreCarrotTicks);
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

	a_Entities.emplace_back(std::move(Monster));
}





void cWSSAnvil::LoadRavagerFromNBT(cEntityList &a_Entities, const cParsedNBT &a_NBT, int a_TagIdx)
{
	// TODO
}





void cWSSAnvil::LoadSalmonFromNBT(cEntityList &a_Entities, const cParsedNBT &a_NBT, int a_TagIdx)
{
	// TODO
}





void cWSSAnvil::LoadSheepFromNBT(cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx)
{
	int ColorIdx = a_NBT.FindChildByName(a_TagIdx, "Color");
	int Color = -1;
	if (ColorIdx > 0)
	{
		Color = static_cast<int>(a_NBT.GetByte(ColorIdx));
	}

	std::unique_ptr<cSheep> Monster = std::make_unique<cSheep>(Color);
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

	a_Entities.emplace_back(std::move(Monster));
}





void cWSSAnvil::LoadShulkerFromNBT(cEntityList &a_Entities, const cParsedNBT &a_NBT, int a_TagIdx)
{
	// TODO
}





void cWSSAnvil::LoadSilverfishFromNBT(cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx)
{
	std::unique_ptr<cSilverfish> Monster = std::make_unique<cSilverfish>();
	if (!LoadEntityBaseFromNBT(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	if (!LoadMonsterBaseFromNBT(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	a_Entities.emplace_back(std::move(Monster));
}





void cWSSAnvil::LoadSkeletonFromNBT(cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx)
{
	// Wither skeleton is a separate mob in Minecraft 1.11+, but we need this to
	// load them from older worlds where wither skeletons were only a skeleton with a flag
	int TypeIdx = a_NBT.FindChildByName(a_TagIdx, "SkeletonType");

	std::unique_ptr<cMonster> Monster;
	if ((TypeIdx > 0) && (a_NBT.GetByte(TypeIdx) == 1))
	{
		Monster = std::make_unique<cWitherSkeleton>();
	}
	else
	{
		Monster = std::make_unique<cSkeleton>();
	}

	if (!LoadEntityBaseFromNBT(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	if (!LoadMonsterBaseFromNBT(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	a_Entities.emplace_back(std::move(Monster));
}





void cWSSAnvil::LoadSkeletonHorseFromNBT(cEntityList &a_Entities, const cParsedNBT &a_NBT, int a_TagIdx)
{
	// TODO
}





void cWSSAnvil::LoadSlimeFromNBT(cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx)
{
	int SizeIdx = a_NBT.FindChildByName(a_TagIdx, "Size");

	if (SizeIdx < 0)
	{
		return;
	}

	int Size = a_NBT.GetInt(SizeIdx);

	std::unique_ptr<cSlime> Monster = std::make_unique<cSlime>(Size);
	if (!LoadEntityBaseFromNBT(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	if (!LoadMonsterBaseFromNBT(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	a_Entities.emplace_back(std::move(Monster));
}





void cWSSAnvil::LoadSnowGolemFromNBT(cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx)
{
	std::unique_ptr<cSnowGolem> Monster = std::make_unique<cSnowGolem>();
	if (!LoadEntityBaseFromNBT(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	if (!LoadMonsterBaseFromNBT(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	a_Entities.emplace_back(std::move(Monster));
}





void cWSSAnvil::LoadSpiderFromNBT(cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx)
{
	std::unique_ptr<cSpider> Monster = std::make_unique<cSpider>();
	if (!LoadEntityBaseFromNBT(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	if (!LoadMonsterBaseFromNBT(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	a_Entities.emplace_back(std::move(Monster));
}





void cWSSAnvil::LoadSquidFromNBT(cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx)
{
	std::unique_ptr<cSquid> Monster = std::make_unique<cSquid>();
	if (!LoadEntityBaseFromNBT(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	if (!LoadMonsterBaseFromNBT(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	a_Entities.emplace_back(std::move(Monster));
}





void cWSSAnvil::LoadStrayFromNBT(cEntityList &a_Entities, const cParsedNBT &a_NBT, int a_TagIdx)
{
	// TODO
}





void cWSSAnvil::LoadStriderFromNBT(cEntityList &a_Entities, const cParsedNBT &a_NBT, int a_TagIdx)
{
	// TODO
}





void cWSSAnvil::LoadTraderLlamaFromNBT(cEntityList &a_Entities, const cParsedNBT &a_NBT, int a_TagIdx)
{
	// TODO
}





void cWSSAnvil::LoadTropicalFishFromNBT(cEntityList &a_Entities, const cParsedNBT &a_NBT, int a_TagIdx)
{
	// TODO
}





void cWSSAnvil::LoadTurtleFromNBT(cEntityList &a_Entities, const cParsedNBT &a_NBT, int a_TagIdx)
{
	// TODO
}





void cWSSAnvil::LoadVexFromNBT(cEntityList &a_Entities, const cParsedNBT &a_NBT, int a_TagIdx)
{
	// TODO
}





void cWSSAnvil::LoadVillagerFromNBT(cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx)
{
	int TypeIdx = a_NBT.FindChildByName(a_TagIdx, "Profession");
	if (TypeIdx < 0)
	{
		return;
	}

	int Type = a_NBT.GetInt(TypeIdx);

	std::unique_ptr<cVillager> Monster = std::make_unique<cVillager>(cVillager::eVillagerType(Type));
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

	int InventoryIdx = a_NBT.FindChildByName(a_TagIdx, "Inventory");
	if (InventoryIdx > 0)
	{
		LoadItemGridFromNBT(Monster->GetInventory(), a_NBT, InventoryIdx);
	}


	a_Entities.emplace_back(std::move(Monster));
}





void cWSSAnvil::LoadVindicatorFromNBT(cEntityList &a_Entities, const cParsedNBT &a_NBT, int a_TagIdx)
{
	// TODO
}





void cWSSAnvil::LoadWanderingTraderFromNBT(cEntityList &a_Entities, const cParsedNBT &a_NBT, int a_TagIdx)
{
	// TODO
}





void cWSSAnvil::LoadWitchFromNBT(cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx)
{
	std::unique_ptr<cWitch> Monster = std::make_unique<cWitch>();
	if (!LoadEntityBaseFromNBT(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	if (!LoadMonsterBaseFromNBT(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	a_Entities.emplace_back(std::move(Monster));
}





void cWSSAnvil::LoadWitherFromNBT(cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx)
{
	std::unique_ptr<cWither> Monster = std::make_unique<cWither>();
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

	a_Entities.emplace_back(std::move(Monster));
}





void cWSSAnvil::LoadWitherSkeletonFromNBT(cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx)
{
	auto Monster = std::make_unique<cWitherSkeleton>();
	if (!LoadEntityBaseFromNBT(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	if (!LoadMonsterBaseFromNBT(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	a_Entities.emplace_back(std::move(Monster));
}





void cWSSAnvil::LoadWolfFromNBT(cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx)
{
	std::unique_ptr<cWolf> Monster = std::make_unique<cWolf>();
	if (!LoadEntityBaseFromNBT(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}
	if (!LoadMonsterBaseFromNBT(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	auto OwnerInfo = LoadEntityOwner(a_NBT, a_TagIdx);
	if (!OwnerInfo.first.empty() && !OwnerInfo.second.IsNil())
	{
		Monster->SetOwner(OwnerInfo.first, OwnerInfo.second);
		Monster->SetIsTame(true);
	}

	int SittingIdx = a_NBT.FindChildByName(a_TagIdx, "Sitting");
	if ((SittingIdx > 0) && (a_NBT.GetType(SittingIdx) == TAG_Byte))
	{
		bool Sitting = (a_NBT.GetByte(SittingIdx) == 1);
		Monster->SetIsSitting(Sitting);
	}
	int AngryIdx = a_NBT.FindChildByName(a_TagIdx, "Angry");
	if ((AngryIdx > 0) && (a_NBT.GetType(AngryIdx) == TAG_Byte))
	{
		bool Angry = (a_NBT.GetByte(AngryIdx) == 1);
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

	a_Entities.emplace_back(std::move(Monster));
}





void cWSSAnvil::LoadZoglinFromNBT(cEntityList &a_Entities, const cParsedNBT &a_NBT, int a_TagIdx)
{
	// TODO
}





void cWSSAnvil::LoadZombieFromNBT(cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx)
{
	std::unique_ptr<cZombie> Monster = std::make_unique<cZombie>();
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
			case TAG_Int:  Age = a_NBT.GetInt(AgeableIdx); break;
			default: Age = 0; break;
		}
		Monster->SetAge(Age);
	}

	a_Entities.emplace_back(std::move(Monster));
}





void cWSSAnvil::LoadZombieHorseFromNBT(cEntityList &a_Entities, const cParsedNBT &a_NBT, int a_TagIdx)
{
	// TODO
}





void cWSSAnvil::LoadZombifiedPiglinFromNBT(cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx)
{
	std::unique_ptr<cZombiePigman> Monster = std::make_unique<cZombiePigman>();
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

	a_Entities.emplace_back(std::move(Monster));
}





void cWSSAnvil::LoadZombieVillagerFromNBT(cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx)
{
	int ProfessionIdx = a_NBT.FindChildByName(a_TagIdx, "Profession");
	if (ProfessionIdx < 0)
	{
		return;
	}

	cVillager::eVillagerType Profession = static_cast<cVillager::eVillagerType>(a_NBT.GetInt(ProfessionIdx));

	std::unique_ptr<cZombieVillager> Monster = std::make_unique<cZombieVillager>(Profession);
	if (!LoadEntityBaseFromNBT(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	if (!LoadMonsterBaseFromNBT(*Monster.get(), a_NBT, a_TagIdx))
	{
		return;
	}

	// TODO: Conversion time

	int AgeableIdx  = a_NBT.FindChildByName(a_TagIdx, "Age");
	if (AgeableIdx > 0)
	{
		int Age;
		switch (a_NBT.GetType(AgeableIdx))
		{
			case TAG_Byte: Age = static_cast<int>(a_NBT.GetByte(AgeableIdx)); break;
			case TAG_Int:  Age = a_NBT.GetInt (AgeableIdx); break;
			default:       Age = 0; break;
		}
		Monster->SetAge(Age);
	}

	a_Entities.emplace_back(std::move(Monster));
}





std::pair<AString, cUUID> cWSSAnvil::LoadEntityOwner(const cParsedNBT & a_NBT, int a_TagIdx)
{
	// Load the owner information. OwnerUUID or Owner may be specified, possibly both:
	AString OwnerName;
	cUUID OwnerUUID;
	int OwnerUUIDIdx = a_NBT.FindChildByName(a_TagIdx, "OwnerUUID");
	if (OwnerUUIDIdx > 0)
	{
		OwnerUUID.FromString(a_NBT.GetString(OwnerUUIDIdx));
	}
	int OwnerIdx = a_NBT.FindChildByName(a_TagIdx, "Owner");
	if (OwnerIdx > 0)
	{
		OwnerName = a_NBT.GetString(OwnerIdx);
	}
	if (OwnerName.empty() && OwnerUUID.IsNil())
	{
		// There is no owner, bail out:
		return {};
	}

	// Convert name to UUID, if needed:
	if (OwnerUUID.IsNil())
	{
		// This entity has only playername stored (pre-1.7.6), look up the UUID
		// The lookup is blocking, but we're running in a separate thread, so it's ok
		OwnerUUID = cRoot::Get()->GetMojangAPI().GetUUIDFromPlayerName(OwnerName);
		if (OwnerUUID.IsNil())
		{
			// Not a known player, un-tame the entity by bailing out
			return {};
		}
	}

	// Convert UUID to name, if needed:
	if (OwnerName.empty())
	{
		// The lookup is blocking, but we're running in a separate thread, so it's ok
		OwnerName = cRoot::Get()->GetMojangAPI().GetPlayerNameFromUUID(OwnerUUID);
		if (OwnerName.empty())
		{
			// Not a known player, un-tame the entity by bailing out
			return {};
		}
	}

	return { OwnerName, OwnerUUID };
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

	// Depending on the Minecraft version, the entity's health is
	// stored either as a float Health tag (HealF prior to 1.9) or
	// as a short Health tag. The float tags should be preferred.
	int Health = a_NBT.FindChildByName(a_TagIdx, "Health");
	int HealF  = a_NBT.FindChildByName(a_TagIdx, "HealF");

	if (Health > 0 && a_NBT.GetType(Health) == TAG_Float)
	{
		a_Entity.SetHealth(a_NBT.GetFloat(Health));
	}
	else if (HealF > 0)
	{
		a_Entity.SetHealth(a_NBT.GetFloat(HealF));
	}
	else if (Health > 0)
	{
		a_Entity.SetHealth(static_cast<float>(a_NBT.GetShort(Health)));
	}
	else
	{
		a_Entity.SetHealth(a_Entity.GetMaxHealth());
	}

	return true;
}





bool cWSSAnvil::LoadMonsterBaseFromNBT(cMonster & a_Monster, const cParsedNBT & a_NBT, int a_TagIdx)
{
	float DropChance[5];
	if (LoadFloatsListFromNBT(DropChance, 5, a_NBT, a_NBT.FindChildByName(a_TagIdx, "DropChances")))
	{
		a_Monster.SetDropChanceWeapon(DropChance[0]);
		a_Monster.SetDropChanceHelmet(DropChance[1]);
		a_Monster.SetDropChanceChestplate(DropChance[2]);
		a_Monster.SetDropChanceLeggings(DropChance[3]);
		a_Monster.SetDropChanceBoots(DropChance[4]);
	}
	if (LoadFloatsListFromNBT(DropChance, 2, a_NBT, a_NBT.FindChildByName(a_TagIdx, "HandDropChances")))
	{
		a_Monster.SetDropChanceWeapon(DropChance[0]);
	}
	if (LoadFloatsListFromNBT(DropChance, 4, a_NBT, a_NBT.FindChildByName(a_TagIdx, "ArmorDropChances")))
	{
		a_Monster.SetDropChanceHelmet(DropChance[0]);
		a_Monster.SetDropChanceChestplate(DropChance[1]);
		a_Monster.SetDropChanceLeggings(DropChance[2]);
		a_Monster.SetDropChanceBoots(DropChance[3]);
	}

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

	// Leashed to a knot
	int LeashedIdx = a_NBT.FindChildByName(a_TagIdx, "Leashed");
	if ((LeashedIdx >= 0) && a_NBT.GetByte(LeashedIdx))
	{
		LoadLeashToPosition(a_Monster, a_NBT, a_TagIdx);
	}

	return true;
}





void cWSSAnvil::LoadLeashToPosition(cMonster & a_Monster, const cParsedNBT & a_NBT, int a_TagIdx)
{
	int LeashIdx = a_NBT.FindChildByName(a_TagIdx, "Leash");
	if (LeashIdx < 0)
	{
		return;
	}

	double PosX = 0.0, PosY = 0.0, PosZ = 0.0;
	bool KnotPosPresent = true;
	int LeashDataLine = a_NBT.FindChildByName(LeashIdx, "X");
	if (LeashDataLine >= 0)
	{
		PosX = a_NBT.GetDouble(LeashDataLine);
	}
	else
	{
		KnotPosPresent = false;
	}
	LeashDataLine = a_NBT.FindChildByName(LeashIdx, "Y");
	if (LeashDataLine >= 0)
	{
		PosY = a_NBT.GetDouble(LeashDataLine);
	}
	else
	{
		KnotPosPresent = false;
	}
	LeashDataLine = a_NBT.FindChildByName(LeashIdx, "Z");
	if (LeashDataLine >= 0)
	{
		PosZ = a_NBT.GetDouble(LeashDataLine);
	}
	else
	{
		KnotPosPresent = false;
	}
	if (KnotPosPresent)
	{
		// Set leash pos for the mob
		a_Monster.SetLeashToPos(new Vector3d(PosX, PosY, PosZ));
	}
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





bool cWSSAnvil::GetBlockEntityNBTPos(const cParsedNBT & a_NBT, int a_TagIdx, Vector3i & a_AbsPos)
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
	a_AbsPos.Set(
		Clamp(a_NBT.GetInt(x), -40000000, 40000000),  // World is limited to 30M blocks in XZ, we clamp to 40M
		Clamp(a_NBT.GetInt(y), -10000,    10000),     // Y is limited to 0 .. 255, we clamp to 10K
		Clamp(a_NBT.GetInt(z), -40000000, 40000000)
	);
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





bool cWSSAnvil::cMCAFile::GetChunkData(const cChunkCoords & a_Chunk, ContiguousByteBuffer & a_Data)
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
		m_ParentSchema.ChunkLoadFailed(a_Chunk.m_ChunkX, a_Chunk.m_ChunkZ, "Cannot read chunk size", {});
		return false;
	}
	ChunkSize = ntohl(ChunkSize);
	if (ChunkSize < 1)
	{
		// Chunk size too small
		m_ParentSchema.ChunkLoadFailed(a_Chunk.m_ChunkX, a_Chunk.m_ChunkZ, "Chunk size too small", {});
		return false;
	}

	char CompressionType = 0;
	if (m_File.Read(&CompressionType, 1) != 1)
	{
		m_ParentSchema.ChunkLoadFailed(a_Chunk.m_ChunkX, a_Chunk.m_ChunkZ, "Cannot read chunk compression", {});
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
		m_ParentSchema.ChunkLoadFailed(a_Chunk.m_ChunkX, a_Chunk.m_ChunkZ, Printf("Unknown chunk compression: %d", CompressionType), a_Data);
		return false;
	}
	return true;
}





const std::byte * cWSSAnvil::GetSectionData(const cParsedNBT & a_NBT, int a_Tag, const AString & a_ChildName, size_t a_Length)
{
	int Child = a_NBT.FindChildByName(a_Tag, a_ChildName);
	if ((Child >= 0) && (a_NBT.GetType(Child) == TAG_ByteArray) && (a_NBT.GetDataLength(Child) == a_Length))
	{
		return a_NBT.GetData(Child);
	}
	return nullptr;
}





bool cWSSAnvil::cMCAFile::SetChunkData(const cChunkCoords & a_Chunk, const ContiguousByteBufferView a_Data)
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

	unsigned ChunkSector = FindFreeLocation(LocalX, LocalZ, a_Data.size());

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





unsigned cWSSAnvil::cMCAFile::FindFreeLocation(int a_LocalX, int a_LocalZ, const size_t a_DataSize)
{
	// See if it fits the current location:
	unsigned ChunkLocation = ntohl(m_Header[a_LocalX + 32 * a_LocalZ]);
	unsigned ChunkLen = ChunkLocation & 0xff;
	if (a_DataSize + MCA_CHUNK_HEADER_LENGTH <= (ChunkLen * 4096))
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
