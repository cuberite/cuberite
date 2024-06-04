#include "Globals.h"

#include "WorldStorage/WSSAnvil.h"

#include "WorldStorage/NBTChunkSerializer.h"
#include "WorldStorage/EnchantmentSerializer.h"
#include "WorldStorage/NamespaceSerializer.h"

#include "WorldStorage/WSSAnvilVersions/WSSAnvilHandler.h"

#include "json/json.h"
#include "OSSupport/GZipFile.h"
#include "World.h"
#include "Item.h"
#include "ItemGrid.h"
#include "StringCompression.h"
#include "SetChunkData.h"
#include "Root.h"
#include "BlockType.h"
#include "JsonUtils.h"

#include "BlockEntities/BannerEntity.h"
#include "BlockEntities/BeaconEntity.h"
#include "BlockEntities/BedEntity.h"
#include "BlockEntities/BrewingstandEntity.h"
#include "BlockEntities/ChestEntity.h"
#include "BlockEntities/CommandBlockEntity.h"
#include "BlockEntities/DispenserEntity.h"
#include "BlockEntities/DropperEntity.h"
#include "BlockEntities/EnchantingTableEntity.h"
#include "BlockEntities/EnderChestEntity.h"
#include "BlockEntities/EndPortalEntity.h"
#include "BlockEntities/FurnaceEntity.h"
#include "BlockEntities/HopperEntity.h"
#include "BlockEntities/JukeboxEntity.h"
#include "BlockEntities/NoteEntity.h"
#include "BlockEntities/SignEntity.h"
#include "BlockEntities/MobHeadEntity.h"
#include "BlockEntities/MobSpawnerEntity.h"
#include "BlockEntities/FlowerPotEntity.h"

#include "Mobs/IncludeAllMonsters.h"

#include "Entities/Boat.h"
#include "Entities/EnderCrystal.h"
#include "Entities/FallingBlock.h"
#include "Entities/Minecart.h"
#include "Entities/Pickup.h"
#include "Entities/ArrowEntity.h"
#include "Entities/SplashPotionEntity.h"
#include "Entities/ThrownEggEntity.h"
#include "Entities/ThrownEnderPearlEntity.h"
#include "Entities/ThrownSnowballEntity.h"
#include "Entities/FireChargeEntity.h"
#include "Entities/GhastFireballEntity.h"
#include "Entities/TNTEntity.h"
#include "Entities/ExpOrb.h"
#include "Entities/HangingEntity.h"
#include "Entities/ItemFrame.h"
#include "Entities/LeashKnot.h"
#include "Entities/Painting.h"

#include "Protocol/MojangAPI.h"
#include "Server.h"
#include "BoundingBox.h"





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

cWSSAnvil::cWSSAnvil(cWorld * a_World, int a_CompressionFactor):
	Super(a_World),
	m_Compressor(a_CompressionFactor)
{
	// Create a level.dat file for mapping tools, if it doesn't already exist:
	auto fnam = fmt::format(FMT_STRING("{}{}level.dat"), a_World->GetDataPath(), cFile::PathSeparator());
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





void cWSSAnvil::ChunkLoadFailed(const cChunkCoords a_ChunkCoords, const AString & a_Reason, const ContiguousByteBufferView a_ChunkDataToSave) const
{
	// Construct the filename for offloading:
	auto OffloadFileName = fmt::format(FMT_STRING("{0}{1}region{1}badchunks"), m_World->GetDataPath(), cFile::PathSeparator());
	cFile::CreateFolder(OffloadFileName);
	auto t = time(nullptr);
	struct tm stm;
	#ifdef _MSC_VER
		localtime_s(&stm, &t);
	#else
		localtime_r(&t, &stm);
	#endif
	OffloadFileName.append(fmt::format(
		FMT_STRING("{}ch.{}.{}.{}-{:02d}-{:02d}-{:02d}-{:02d}-{:02d}.dat"),
		cFile::PathSeparator(), a_ChunkCoords.m_ChunkX, a_ChunkCoords.m_ChunkZ,
		stm.tm_year + 1900, stm.tm_mon + 1, stm.tm_mday, stm.tm_hour, stm.tm_min, stm.tm_sec
	));

	// Log the warning to console:
	const int RegionX = FAST_FLOOR_DIV(a_ChunkCoords.m_ChunkX, 32);
	const int RegionZ = FAST_FLOOR_DIV(a_ChunkCoords.m_ChunkZ, 32);
	auto Info = fmt::format(
		FMT_STRING("Loading chunk {} for world {} from file r.{}.{}.mca failed: {} Offloading old chunk data to file {} and regenerating chunk."),
		a_ChunkCoords, m_World->GetName(), RegionX, RegionZ, a_Reason, OffloadFileName
	);
	LOGWARNING("%s", Info);

	// Write the data:
	cFile f;
	if (!f.Open(OffloadFileName, cFile::fmWrite))
	{
		LOGWARNING("Cannot open file %s for writing! Old chunk data is lost.", OffloadFileName);
		return;
	}
	f.Write(a_ChunkDataToSave.data(), a_ChunkDataToSave.size());
	f.Close();

	// Write a description file:
	if (!f.Open(OffloadFileName + ".info", cFile::fmWrite))
	{
		LOGWARNING("Cannot open file %s.info for writing! The information about the failed chunk will not be written.", OffloadFileName);
		return;
	}
	f.Write(Info.c_str(), Info.size());
	f.Close();
}





bool cWSSAnvil::GetChunkData(const cChunkCoords & a_Chunk, ContiguousByteBuffer & a_Data)
{
	cCSLock Lock(m_CS);
	auto File = LoadMCAFile(a_Chunk);
	if (File == nullptr)
	{
		return false;
	}
	return File->GetChunkData(a_Chunk, a_Data);
}





bool cWSSAnvil::SetChunkData(const cChunkCoords & a_Chunk, const ContiguousByteBufferView a_Data)
{
	cCSLock Lock(m_CS);
	auto File = LoadMCAFile(a_Chunk);
	if (File == nullptr)
	{
		return false;
	}
	return File->SetChunkData(a_Chunk, a_Data);
}





std::shared_ptr<cWSSAnvil::cMCAFile> cWSSAnvil::LoadMCAFile(const cChunkCoords & a_Chunk)
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
	for (auto itr = m_Files.begin(); itr != m_Files.end(); ++itr)
	{
		if (((*itr) != nullptr) && ((*itr)->GetRegionX() == RegionX) && ((*itr)->GetRegionZ() == RegionZ))
		{
			// Move the file to front and return it:
			auto File = *itr;
			if (itr != m_Files.begin())
			{
				m_Files.erase(itr);
				m_Files.push_front(File);
			}
			return File;
		}
	}

	// Load it anew:
	auto FileName = fmt::format(FMT_STRING("{}{}region"), m_World->GetDataPath(), cFile::PathSeparator());
	cFile::CreateFolder(FileName);
	FileName.append(fmt::format(FMT_STRING("/r.{}.{}.mca"), RegionX, RegionZ));
	auto File = std::make_shared<cMCAFile>(*this, FileName, RegionX, RegionZ);
	if (File == nullptr)
	{
		return nullptr;
	}
	m_Files.push_front(File);

	// If there are too many MCA files cached, delete the last one used:
	if (m_Files.size() > MAX_MCA_FILES)
	{
		m_Files.pop_back();
	}
	return File;
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
		ChunkLoadFailed(a_Chunk, Oops.what(), a_Data);
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





bool cWSSAnvil::LoadChunkFromNBT(const cChunkCoords & a_Chunk, const cParsedNBT & a_NBT, const ContiguousByteBufferView a_RawChunkData) const
{
	struct SetChunkData Data(a_Chunk);

	// Load the blockdata, blocklight and skylight:

	// Find the "Level", the root, tag
	int Level = a_NBT.FindChildByName(0, "Level");
	if (Level < 0)
	{
		ChunkLoadFailed(a_Chunk, "Missing NBT tag: Level", a_RawChunkData);
		return false;
	}

	int DataVersionPos = a_NBT.FindChildByName(0, "DataVersion");
	eDataVersion DataVersion = eDataVersion::v1_8;
	if (DataVersionPos >= 0)
	{
		DataVersion = NamespaceSerializer::ToDataVersion(a_NBT.GetInt(DataVersionPos));
	}

	int Sections = a_NBT.FindChildByName(Level, "Sections");
	if ((Sections < 0) || (a_NBT.GetType(Sections) != TAG_List))
	{
		ChunkLoadFailed(a_Chunk, "Missing NBT tag: Sections", a_RawChunkData);
		return false;
	}

	eTagType SectionsType = a_NBT.GetChildrenType(Sections);
	if ((SectionsType != TAG_Compound) && (SectionsType != TAG_End))
	{
		ChunkLoadFailed(a_Chunk, "NBT tag has wrong type: Sections", a_RawChunkData);
		return false;
	}
	for (int Child = a_NBT.GetFirstChild(Sections); Child >= 0; Child = a_NBT.GetNextSibling(Child))
	{
		const int SectionYTag = a_NBT.FindChildByName(Child, "Y");
		if ((SectionYTag < 0) || (a_NBT.GetType(SectionYTag) != TAG_Byte))
		{
			ChunkLoadFailed(a_Chunk, "NBT tag missing or has wrong: Y", a_RawChunkData);
			return false;
		}

		const int Y = a_NBT.GetByte(SectionYTag);
		if ((Y < 0) || (Y > static_cast<int>(cChunkDef::NumSections - 1)))
		{
			ChunkLoadFailed(a_Chunk, "NBT tag exceeds chunk bounds: Y", a_RawChunkData);
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
			ChunkLoadFailed(a_Chunk, "Missing chunk block/light data", a_RawChunkData);
			return false;
		}
	}  // for itr - LevelSections[]

	// Load the biomes from NBT, if present and valid:
	if (!LoadBiomeMapFromNBT(Data.BiomeMap, a_NBT, a_NBT.FindChildByName(Level, "Biomes"), DataVersion))
	{
		ChunkLoadFailed(a_Chunk, "Missing chunk biome data", a_RawChunkData);
		return false;
	}

	// Load the Height map, if it fails, recalculate it:
	if (!LoadHeightMapFromNBT(Data.HeightMap, a_NBT, a_NBT.FindChildByName(Level, "HeightMap"), DataVersion))
	{
		Data.UpdateHeightMap();
	}

	// Load the entities from NBT:
	LoadEntitiesFromNBT     (Data.Entities,      a_NBT, a_NBT.FindChildByName(Level, "Entities"), DataVersion);
	LoadBlockEntitiesFromNBT(Data.BlockEntities, a_NBT, a_NBT.FindChildByName(Level, "TileEntities"), Data.BlockData, DataVersion);

	Data.IsLightValid = (a_NBT.FindChildByName(Level, "MCSIsLightValid") > 0);

#ifdef READDEBUG
	// Uncomment this block for really cool stuff :)
	// DEBUG magic: Invert the underground, so that we can see the MC generator in action :)
	bool ShouldInvert[cChunkDef::Width * cChunkDef::Width];
	memset(ShouldInvert, 0, sizeof(ShouldInvert));
	for (int y = cChunkDef::Height - 1; y >= 0; y--)
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
#endif  // READDEBUG

	m_World->QueueSetChunkData(std::move(Data));
	return true;
}





bool cWSSAnvil::LoadBiomeMapFromNBT(cChunkDef::BiomeMap & a_BiomeMap, const cParsedNBT & a_NBT, const int a_TagIdx, eDataVersion a_DataVersion) const
{
	return cWSSAnvilHandler::From(a_DataVersion).LoadBiomeMap(a_BiomeMap, a_NBT, a_TagIdx);
}





bool cWSSAnvil::LoadHeightMapFromNBT(cChunkDef::HeightMap & a_HeightMap, const cParsedNBT & a_NBT, const int a_TagIdx, eDataVersion a_DataVersion) const
{
	return cWSSAnvilHandler::From(a_DataVersion).LoadHeightMap(a_HeightMap, a_NBT, a_TagIdx);
}





void cWSSAnvil::LoadEntitiesFromNBT(cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx, eDataVersion a_DataVersion) const
{
	cWSSAnvilHandler::From(a_DataVersion).LoadEntities(a_Entities, a_NBT, a_TagIdx);
}





void cWSSAnvil::LoadBlockEntitiesFromNBT(cBlockEntities & a_BlockEntities, const cParsedNBT & a_NBT, int a_TagIdx, const ChunkBlockData & a_BlockData, eDataVersion a_DataVersion) const
{
	return cWSSAnvilHandler::From(a_DataVersion).LoadBlockEntities(a_BlockEntities, a_NBT, a_TagIdx, a_BlockData, m_World);
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
	m_Header.fill(0);
	if (m_File.Read(m_Header.data(), m_Header.size()) != static_cast<int>(m_Header.size()))
	{
		// Cannot read the header - perhaps the file has just been created?
		// Try writing a nullptr header for chunk offsets:
		m_Header.fill(0);
		writeOutNeeded = true;
	}

	// Load the TimeStamps:
	m_TimeStamps.fill(0);
	if (m_File.Read(m_TimeStamps.data(), m_TimeStamps.size()) != static_cast<int>(m_TimeStamps.size()))
	{
		// Cannot read the time stamps - perhaps the file has just been created?
		// Try writing a nullptr header for timestamps:
		m_TimeStamps.fill(0);
		writeOutNeeded = true;
	}

	if (writeOutNeeded)
	{
		m_File.Seek(0);
		if (
			(m_File.Write(m_Header.data(), m_Header.size()) != static_cast<int>(m_Header.size())) ||           // Write chunk offsets
			(m_File.Write(m_TimeStamps.data(), m_TimeStamps.size()) != static_cast<int>(m_TimeStamps.size()))  // Write chunk timestamps
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

	size_t HeaderIndex = ((a_Chunk.m_ChunkX & 31) + (a_Chunk.m_ChunkZ & 31) * 32);
	unsigned ChunkLocation = ntohl(m_Header[HeaderIndex]);
	unsigned ChunkOffset = ChunkLocation >> 8;

	ASSERT(static_cast<long>(ChunkOffset * 4096) < m_File.GetSize());

	// First chunk is at offset 2, because the first two sectors are the header and timestamp
	if (ChunkOffset < 2)
	{
		return false;
	}

	// Chunk doesn't exist, if the offset is 0 and the timestamp is 0
	if (
		(m_Header[HeaderIndex] == 0) &&
		(m_TimeStamps[HeaderIndex] == 0)
		)
	{
		return false;
	}

	m_File.Seek(static_cast<int>(ChunkOffset * 4096));

	UInt32 ChunkSize = 0;

	if (m_File.Read(&ChunkSize, 4) != 4)
	{
		m_ParentSchema.ChunkLoadFailed(a_Chunk, "Cannot read chunk size", {});
		return false;
	}
	ChunkSize = ntohl(ChunkSize);
	if (ChunkSize < 1)
	{
		// Chunk size too small
		m_ParentSchema.ChunkLoadFailed(a_Chunk, "Chunk size too small", {});
		return false;
	}

	char CompressionType = 0;
	if (m_File.Read(&CompressionType, 1) != 1)
	{
		m_ParentSchema.ChunkLoadFailed(a_Chunk, "Cannot read chunk compression", {});
		return false;
	}
	ChunkSize--;

	a_Data = m_File.Read(ChunkSize);
	if (a_Data.size() != ChunkSize)
	{
		m_ParentSchema.ChunkLoadFailed(a_Chunk, "Cannot read entire chunk data", a_Data);
		return false;
	}

	if (CompressionType != 2)
	{
		// Chunk is in an unknown compression
		m_ParentSchema.ChunkLoadFailed(a_Chunk, fmt::format(FMT_STRING("Unknown chunk compression: {}"), CompressionType), a_Data);
		return false;
	}
	return true;
}





const std::byte * cWSSAnvil::GetSectionData(const cParsedNBT & a_NBT, int a_Tag, const AString & a_ChildName, size_t a_Length) const
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
	size_t HeaderIndex = ((a_Chunk.m_ChunkX & 31) + (a_Chunk.m_ChunkZ & 31) * 32);
	auto ChunkSector = FindFreeLocation(LocalX, LocalZ, a_Data.size());

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
	m_Header[HeaderIndex] = htonl(static_cast<UInt32>((ChunkSector << 8) | ChunkSize));

	// Set the modification time
	m_TimeStamps[HeaderIndex] =  htonl(static_cast<UInt32>(time(nullptr)));

	if (m_File.Seek(0) < 0)
	{
		LOGWARNING("Cannot save chunk [%d, %d], seeking in file \"%s\" failed", a_Chunk.m_ChunkX, a_Chunk.m_ChunkZ, GetFileName().c_str());
		return false;
	}
	if (m_File.Write(m_Header.data(), m_Header.size()) != static_cast<int>(m_Header.size()))
	{
		LOGWARNING("Cannot save chunk [%d, %d], writing header to file \"%s\" failed", a_Chunk.m_ChunkX, a_Chunk.m_ChunkZ, GetFileName().c_str());
		return false;
	}
	if (m_File.Write(m_TimeStamps.data(), m_TimeStamps.size()) != static_cast<int>(m_TimeStamps.size()))
	{
		LOGWARNING("Cannot save chunk [%d, %d], writing timestamps to file \"%s\" failed", a_Chunk.m_ChunkX, a_Chunk.m_ChunkZ, GetFileName().c_str());
		return false;
	}

	return true;
}





unsigned cWSSAnvil::cMCAFile::FindFreeLocation(int a_LocalX, int a_LocalZ, const size_t a_DataSize)
{
	// See if it fits the current location:
	unsigned ChunkLocation = ntohl(m_Header[static_cast<size_t>(a_LocalX + 32 * a_LocalZ)]);
	unsigned ChunkLen = ChunkLocation & 0xff;
	if (a_DataSize + MCA_CHUNK_HEADER_LENGTH <= (ChunkLen * 4096))
	{
		return ChunkLocation >> 8;
	}

	// Doesn't fit, append to the end of file (we're wasting a lot of space, TODO: fix this later)
	unsigned MaxLocation = 2 << 8;  // Minimum sector is #2 - after the headers
	for (const auto & Header : m_Header)
	{
		ChunkLocation = ntohl(Header);
		ChunkLocation = ChunkLocation + ((ChunkLocation & 0xff) << 8);  // Add the number of sectors used; don't care about the 4th byte
		if (MaxLocation < ChunkLocation)
		{
			MaxLocation = ChunkLocation;
		}
	}  // for i - m_Header[]
	return MaxLocation >> 8;
}
