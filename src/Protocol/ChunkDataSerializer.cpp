#include "Globals.h"
#include "ChunkDataSerializer.h"
#include "Protocol_1_8.h"
#include "Protocol_1_9.h"
#include "../ClientHandle.h"
#include "../WorldStorage/FastNBT.h"

#include "Palettes/Upgrade.h"
#include "Palettes/Palette_1_13.h"
#include "Palettes/Palette_1_13_1.h"
#include "Palettes/Palette_1_14.h"





namespace
{
	std::pair<UInt16, size_t> GetSectionBitmask(const ChunkBlockData & a_BlockData, const ChunkLightData & a_LightData)
	{
		size_t Present = 0;
		UInt16 Mask = 0;

		ChunkDef_ForEachSection(a_BlockData, a_LightData,
		{
			Present++;
			Mask |= (1 << Y);
		});

		return { Mask, Present };
	}

	auto PaletteLegacy(const BLOCKTYPE a_BlockType, const NIBBLETYPE a_Meta)
	{
		return (a_BlockType << 4) | a_Meta;
	}

	auto Palette393(const BLOCKTYPE a_BlockType, const NIBBLETYPE a_Meta)
	{
		return Palette_1_13::From(PaletteUpgrade::FromBlock(a_BlockType, a_Meta));
	}

	auto Palette401(const BLOCKTYPE a_BlockType, const NIBBLETYPE a_Meta)
	{
		return Palette_1_13_1::From(PaletteUpgrade::FromBlock(a_BlockType, a_Meta));
	}

	auto Palette477(const BLOCKTYPE a_BlockType, const NIBBLETYPE a_Meta)
	{
		return Palette_1_14::From(PaletteUpgrade::FromBlock(a_BlockType, a_Meta));
	}
}





////////////////////////////////////////////////////////////////////////////////
// cChunkDataSerializer:

cChunkDataSerializer::cChunkDataSerializer(const eDimension a_Dimension) :
	m_Packet(512 KiB),
	m_Dimension(a_Dimension)
{
}





void cChunkDataSerializer::SendToClients(const int a_ChunkX, const int a_ChunkZ, const ChunkBlockData & a_BlockData, const ChunkLightData & a_LightData, const unsigned char * a_BiomeMap, const ClientHandles & a_SendTo)
{
	for (const auto & Client : a_SendTo)
	{
		switch (static_cast<cProtocol::Version>(Client->GetProtocolVersion()))
		{
			case cProtocol::Version::v1_8_0:
			{
				Serialize(Client, a_ChunkX, a_ChunkZ, a_BlockData, a_LightData, a_BiomeMap, CacheVersion::v47);
				continue;
			}
			case cProtocol::Version::v1_9_0:
			case cProtocol::Version::v1_9_1:
			case cProtocol::Version::v1_9_2:
			{
				Serialize(Client, a_ChunkX, a_ChunkZ, a_BlockData, a_LightData, a_BiomeMap, CacheVersion::v107);
				continue;
			}
			case cProtocol::Version::v1_9_4:
			case cProtocol::Version::v1_10_0:
			case cProtocol::Version::v1_11_0:
			case cProtocol::Version::v1_11_1:
			case cProtocol::Version::v1_12:
			case cProtocol::Version::v1_12_1:
			case cProtocol::Version::v1_12_2:
			{
				Serialize(Client, a_ChunkX, a_ChunkZ, a_BlockData, a_LightData, a_BiomeMap, CacheVersion::v110);
				continue;
			}
			case cProtocol::Version::v1_13:
			{
				Serialize(Client, a_ChunkX, a_ChunkZ, a_BlockData, a_LightData, a_BiomeMap, CacheVersion::v393);  // This version didn't last very long xD
				continue;
			}
			case cProtocol::Version::v1_13_1:
			case cProtocol::Version::v1_13_2:
			{
				Serialize(Client, a_ChunkX, a_ChunkZ, a_BlockData, a_LightData, a_BiomeMap, CacheVersion::v401);
				continue;
			}
			case cProtocol::Version::v1_14:
			case cProtocol::Version::v1_14_1:
			case cProtocol::Version::v1_14_2:
			case cProtocol::Version::v1_14_3:
			case cProtocol::Version::v1_14_4:
			{
				Serialize(Client, a_ChunkX, a_ChunkZ, a_BlockData, a_LightData, a_BiomeMap, CacheVersion::v477);
				continue;
			}
		}
		UNREACHABLE("Unknown chunk data serialization version");
	}

	// Our cache is only persistent during the function call:
	for (auto & Cache : m_Cache)
	{
		Cache.Engaged = false;
	}
}





inline void cChunkDataSerializer::Serialize(const ClientHandles::value_type & a_Client, const int a_ChunkX, const int a_ChunkZ, const ChunkBlockData & a_BlockData, const ChunkLightData & a_LightData, const unsigned char * a_BiomeMap, const CacheVersion a_CacheVersion)
{
	auto & Cache = m_Cache[static_cast<size_t>(a_CacheVersion)];
	if (Cache.Engaged)
	{
		// Success! We've done it already, just re-use:
		a_Client->SendChunkData(a_ChunkX, a_ChunkZ, Cache.ToSend);
		return;
	}

	switch (a_CacheVersion)
	{
		case CacheVersion::v47:
		{
			Serialize47(a_ChunkX, a_ChunkZ, a_BlockData, a_LightData, a_BiomeMap);
			break;
		}
		case CacheVersion::v107:
		{
			Serialize107(a_ChunkX, a_ChunkZ, a_BlockData, a_LightData, a_BiomeMap);
			break;
		}
		case CacheVersion::v110:
		{
			Serialize110(a_ChunkX, a_ChunkZ, a_BlockData, a_LightData, a_BiomeMap);
			break;
		}
		case CacheVersion::v393:
		{
			Serialize393<&Palette393>(a_ChunkX, a_ChunkZ, a_BlockData, a_LightData, a_BiomeMap);
			break;
		}
		case CacheVersion::v401:
		{
			Serialize393<&Palette401>(a_ChunkX, a_ChunkZ, a_BlockData, a_LightData, a_BiomeMap);
			break;
		}
		case CacheVersion::v477:
		{
			Serialize477(a_ChunkX, a_ChunkZ, a_BlockData, a_LightData, a_BiomeMap);
			break;
		}
	}

	CompressPacketInto(Cache);
	ASSERT(Cache.Engaged);  // Cache must be populated now
	a_Client->SendChunkData(a_ChunkX, a_ChunkZ, Cache.ToSend);
}





inline void cChunkDataSerializer::Serialize47(const int a_ChunkX, const int a_ChunkZ, const ChunkBlockData & a_BlockData, const ChunkLightData & a_LightData, const unsigned char * a_BiomeMap)
{
	// This function returns the fully compressed packet (including packet size), not the raw packet!

	const auto Bitmask = GetSectionBitmask(a_BlockData, a_LightData);

	// Create the packet:
	m_Packet.WriteVarInt32(0x21);  // Packet id (Chunk Data packet)
	m_Packet.WriteBEInt32(a_ChunkX);
	m_Packet.WriteBEInt32(a_ChunkZ);
	m_Packet.WriteBool(true);      // "Ground-up continuous", or rather, "biome data present" flag

	// Minecraft 1.8 does not like completely empty packets
	// Send one completely empty chunk section if this is the case
	m_Packet.WriteBEUInt16(Bitmask.first ? Bitmask.first : 1);

	// Write the chunk size:
	// Account for the single empty section if sending an empty chunk
	const int BiomeDataSize = cChunkDef::Width * cChunkDef::Width;
	const size_t ChunkSize = (
		(Bitmask.second ? Bitmask.second : 1) * (ChunkBlockData::SectionBlockCount * 2 + ChunkLightData::SectionLightCount * 2) +  // Blocks and lighting
		BiomeDataSize    // Biome data
	);
	m_Packet.WriteVarInt32(static_cast<UInt32>(ChunkSize));

	// Chunk written as seperate arrays of (blocktype + meta), blocklight and skylight
	// each array stores all present sections of the same kind packed together

	// Write the block types to the packet:
	ChunkDef_ForEachSection(a_BlockData, a_LightData,
	{
		const bool BlocksExist = Blocks != nullptr;
		const bool MetasExist = Metas != nullptr;

		for (size_t BlockIdx = 0; BlockIdx != ChunkBlockData::SectionBlockCount; ++BlockIdx)
		{
			BLOCKTYPE BlockType = BlocksExist ? (*Blocks)[BlockIdx] : 0;
			NIBBLETYPE BlockMeta = MetasExist ? cChunkDef::ExpandNibble(Metas->data(), BlockIdx) : 0;
			m_Packet.WriteBEUInt8(static_cast<unsigned char>(BlockType << 4) | BlockMeta);
			m_Packet.WriteBEUInt8(static_cast<unsigned char>(BlockType >> 4));
		}
	});

	// Write the block lights:
	ChunkDef_ForEachSection(a_BlockData, a_LightData,
	{
		if (BlockLights == nullptr)
		{
			m_Packet.WriteBuf(ChunkLightData::SectionLightCount, ChunkLightData::DefaultBlockLightValue);
		}
		else
		{
			m_Packet.WriteBuf(BlockLights->data(), BlockLights->size());
		}
	});

	// Write the sky lights:
	ChunkDef_ForEachSection(a_BlockData, a_LightData,
	{
		if (SkyLights == nullptr)
		{
			m_Packet.WriteBuf(ChunkLightData::SectionLightCount, ChunkLightData::DefaultSkyLightValue);
		}
		else
		{
			m_Packet.WriteBuf(SkyLights->data(), SkyLights->size());
		}
	});

	// Serialize a single empty section if sending an empty chunk
	if (!Bitmask.first)
	{
		// Block data (all air)
		for (size_t i = 0; i < ChunkBlockData::SectionBlockCount * 2; i++)
		{
			m_Packet.WriteBEUInt8(0);
		}
		// Light data (XXX: sky light is not sent if in the nether)
		m_Packet.WriteBuf(ChunkLightData::SectionLightCount, ChunkLightData::DefaultSkyLightValue);
		m_Packet.WriteBuf(ChunkLightData::SectionLightCount, ChunkLightData::DefaultSkyLightValue);
	}

	// Write the biome data:
	m_Packet.WriteBuf(a_BiomeMap, BiomeDataSize);
}





inline void cChunkDataSerializer::Serialize107(const int a_ChunkX, const int a_ChunkZ, const ChunkBlockData & a_BlockData, const ChunkLightData & a_LightData, const unsigned char * a_BiomeMap)
{
	// This function returns the fully compressed packet (including packet size), not the raw packet!
	// Below variables tagged static because of https://developercommunity.visualstudio.com/content/problem/367326

	static constexpr UInt8 BitsPerEntry = 13;
	static constexpr size_t ChunkSectionDataArraySize = (ChunkBlockData::SectionBlockCount * BitsPerEntry) / 8 / 8;  // Convert from bit count to long count

	const auto Bitmask = GetSectionBitmask(a_BlockData, a_LightData);

	// Create the packet:
	m_Packet.WriteVarInt32(0x20);  // Packet id (Chunk Data packet)
	m_Packet.WriteBEInt32(a_ChunkX);
	m_Packet.WriteBEInt32(a_ChunkZ);
	m_Packet.WriteBool(true);        // "Ground-up continuous", or rather, "biome data present" flag
	m_Packet.WriteVarInt32(Bitmask.first);

	size_t ChunkSectionSize = (
		1 +                                // Bits per block - set to 13, so the global palette is used and the palette has a length of 0
		1 +                                // Palette length
		2 +                                // Data array length VarInt - 2 bytes for the current value
		ChunkSectionDataArraySize * 8 +    // Actual block data - multiplied by 8 because first number is longs
		ChunkLightData::SectionLightCount  // Block light
	);

	if (m_Dimension == dimOverworld)
	{
		// Sky light is only sent in the overworld.
		ChunkSectionSize += ChunkLightData::SectionLightCount;
	}

	const size_t BiomeDataSize = cChunkDef::Width * cChunkDef::Width;
	const size_t ChunkSize = (
		ChunkSectionSize * Bitmask.second +
		BiomeDataSize
	);

	// Write the chunk size:
	m_Packet.WriteVarInt32(static_cast<UInt32>(ChunkSize));

	// Write each chunk section...
	ChunkDef_ForEachSection(a_BlockData, a_LightData,
	{
		m_Packet.WriteBEUInt8(BitsPerEntry);
		m_Packet.WriteVarInt32(0);  // Palette length is 0
		m_Packet.WriteVarInt32(static_cast<UInt32>(ChunkSectionDataArraySize));
		WriteBlockSectionSeamless<&PaletteLegacy>(Blocks, Metas, BitsPerEntry);
		WriteLightSectionGrouped(BlockLights, SkyLights);
	});

	// Write the biome data
	m_Packet.WriteBuf(a_BiomeMap, BiomeDataSize);
}





inline void cChunkDataSerializer::Serialize110(const int a_ChunkX, const int a_ChunkZ, const ChunkBlockData & a_BlockData, const ChunkLightData & a_LightData, const unsigned char * a_BiomeMap)
{
	// This function returns the fully compressed packet (including packet size), not the raw packet!
	// Below variables tagged static because of https://developercommunity.visualstudio.com/content/problem/367326

	static constexpr UInt8 BitsPerEntry = 13;
	static constexpr size_t ChunkSectionDataArraySize = (ChunkBlockData::SectionBlockCount * BitsPerEntry) / 8 / 8;  // Convert from bit count to long count

	const auto Bitmask = GetSectionBitmask(a_BlockData, a_LightData);

	// Create the packet:
	m_Packet.WriteVarInt32(0x20);  // Packet id (Chunk Data packet)
	m_Packet.WriteBEInt32(a_ChunkX);
	m_Packet.WriteBEInt32(a_ChunkZ);
	m_Packet.WriteBool(true);        // "Ground-up continuous", or rather, "biome data present" flag
	m_Packet.WriteVarInt32(Bitmask.first);

	size_t ChunkSectionSize = (
		1 +                                // Bits per block - set to 13, so the global palette is used and the palette has a length of 0
		1 +                                // Palette length
		2 +                                // Data array length VarInt - 2 bytes for the current value
		ChunkSectionDataArraySize * 8 +    // Actual block data - multiplied by 8 because first number is longs
		ChunkLightData::SectionLightCount  // Block light
	);

	if (m_Dimension == dimOverworld)
	{
		// Sky light is only sent in the overworld.
		ChunkSectionSize += ChunkLightData::SectionLightCount;
	}

	const size_t BiomeDataSize = cChunkDef::Width * cChunkDef::Width;
	const size_t ChunkSize = (
		ChunkSectionSize * Bitmask.second +
		BiomeDataSize
	);

	// Write the chunk size:
	m_Packet.WriteVarInt32(static_cast<UInt32>(ChunkSize));

	// Write each chunk section...
	ChunkDef_ForEachSection(a_BlockData, a_LightData,
	{
		m_Packet.WriteBEUInt8(BitsPerEntry);
		m_Packet.WriteVarInt32(0);  // Palette length is 0
		m_Packet.WriteVarInt32(static_cast<UInt32>(ChunkSectionDataArraySize));
		WriteBlockSectionSeamless<&PaletteLegacy>(Blocks, Metas, BitsPerEntry);
		WriteLightSectionGrouped(BlockLights, SkyLights);
	});

	// Write the biome data
	m_Packet.WriteBuf(a_BiomeMap, BiomeDataSize);

	// Identify 1.9.4's tile entity list as empty
	m_Packet.WriteBEUInt8(0);
}





template <auto Palette>
inline void cChunkDataSerializer::Serialize393(const int a_ChunkX, const int a_ChunkZ, const ChunkBlockData & a_BlockData, const ChunkLightData & a_LightData, const unsigned char * a_BiomeMap)
{
	// This function returns the fully compressed packet (including packet size), not the raw packet!
	// Below variables tagged static because of https://developercommunity.visualstudio.com/content/problem/367326

	static constexpr UInt8 BitsPerEntry = 14;
	static constexpr size_t ChunkSectionDataArraySize = (ChunkBlockData::SectionBlockCount * BitsPerEntry) / 8 / 8;

	const auto Bitmask = GetSectionBitmask(a_BlockData, a_LightData);

	// Create the packet:
	m_Packet.WriteVarInt32(0x22);  // Packet id (Chunk Data packet)
	m_Packet.WriteBEInt32(a_ChunkX);
	m_Packet.WriteBEInt32(a_ChunkZ);
	m_Packet.WriteBool(true);  // "Ground-up continuous", or rather, "biome data present" flag
	m_Packet.WriteVarInt32(Bitmask.first);

	size_t ChunkSectionSize = (
		1 +  // Bits per entry, BEUInt8, 1 byte
		m_Packet.GetVarIntSize(static_cast<UInt32>(ChunkSectionDataArraySize)) +  // Field containing "size of whole section", VarInt32, variable size
		ChunkSectionDataArraySize * 8 +  // Actual section data, lots of bytes (multiplier 1 long = 8 bytes)
		ChunkLightData::SectionLightCount  // Size of blocklight which is always sent
	);

	if (m_Dimension == dimOverworld)
	{
		// Sky light is only sent in the overworld.
		ChunkSectionSize += ChunkLightData::SectionLightCount;
	}

	const size_t BiomeDataSize = cChunkDef::Width * cChunkDef::Width;
	const size_t ChunkSize = (
		ChunkSectionSize * Bitmask.second +
		BiomeDataSize * 4  // Biome data now BE ints
	);

	// Write the chunk size in bytes:
	m_Packet.WriteVarInt32(static_cast<UInt32>(ChunkSize));

	// Write each chunk section...
	ChunkDef_ForEachSection(a_BlockData, a_LightData,
	{
		m_Packet.WriteBEUInt8(BitsPerEntry);
		m_Packet.WriteVarInt32(static_cast<UInt32>(ChunkSectionDataArraySize));
		WriteBlockSectionSeamless<Palette>(Blocks, Metas, BitsPerEntry);
		WriteLightSectionGrouped(BlockLights, SkyLights);
	});

	// Write the biome data
	for (size_t i = 0; i != BiomeDataSize; i++)
	{
		m_Packet.WriteBEUInt32(static_cast<UInt32>(a_BiomeMap[i]));
	}

	// Identify 1.9.4's tile entity list as empty
	m_Packet.WriteVarInt32(0);
}





inline void cChunkDataSerializer::Serialize477(const int a_ChunkX, const int a_ChunkZ, const ChunkBlockData & a_BlockData, const ChunkLightData & a_LightData, const unsigned char * a_BiomeMap)
{
	// This function returns the fully compressed packet (including packet size), not the raw packet!
	// Below variables tagged static because of https://developercommunity.visualstudio.com/content/problem/367326

	static constexpr UInt8 BitsPerEntry = 14;
	static constexpr size_t ChunkSectionDataArraySize = (ChunkBlockData::SectionBlockCount * BitsPerEntry) / 8 / 8;

	const auto Bitmask = GetSectionBitmask(a_BlockData, a_LightData);

	// Create the packet:
	m_Packet.WriteVarInt32(0x21);  // Packet id (Chunk Data packet)
	m_Packet.WriteBEInt32(a_ChunkX);
	m_Packet.WriteBEInt32(a_ChunkZ);
	m_Packet.WriteBool(true);  // "Ground-up continuous", or rather, "biome data present" flag
	m_Packet.WriteVarInt32(Bitmask.first);

	{
		cFastNBTWriter Writer;
		// TODO: client works fine without?
		// std::array<Int64, 36> Longz = {};
		// Writer.AddLongArray("MOTION_BLOCKING", Longz.data(), Longz.size());
		Writer.Finish();
		m_Packet.Write(Writer.GetResult().data(), Writer.GetResult().size());
	}

	const size_t ChunkSectionSize = (
		2 +  // Block count, BEInt16, 2 bytes
		1 +  // Bits per entry, BEUInt8, 1 byte
		m_Packet.GetVarIntSize(static_cast<UInt32>(ChunkSectionDataArraySize)) +  // Field containing "size of whole section", VarInt32, variable size
		ChunkSectionDataArraySize * 8  // Actual section data, lots of bytes (multiplier 1 long = 8 bytes)
	);

	const size_t BiomeDataSize = cChunkDef::Width * cChunkDef::Width;
	const size_t ChunkSize = (
		ChunkSectionSize * Bitmask.second +
		BiomeDataSize * 4  // Biome data now BE ints
	);

	// Write the chunk size in bytes:
	m_Packet.WriteVarInt32(static_cast<UInt32>(ChunkSize));

	// Write each chunk section...
	ChunkDef_ForEachSection(a_BlockData, a_LightData,
	{
		m_Packet.WriteBEInt16(ChunkBlockData::SectionBlockCount);  // a temp fix to make sure sections don't disappear
		m_Packet.WriteBEUInt8(BitsPerEntry);
		m_Packet.WriteVarInt32(static_cast<UInt32>(ChunkSectionDataArraySize));
		WriteBlockSectionSeamless<&Palette477>(Blocks, Metas, BitsPerEntry);
	});

	// Write the biome data
	for (size_t i = 0; i != BiomeDataSize; i++)
	{
		m_Packet.WriteBEUInt32(a_BiomeMap[i]);
	}

	// Identify 1.9.4's tile entity list as empty
	m_Packet.WriteVarInt32(0);
}





template <auto Palette>
inline void cChunkDataSerializer::WriteBlockSectionSeamless(const ChunkBlockData::BlockArray * a_Blocks, const ChunkBlockData::MetaArray * a_Metas, const UInt8 a_BitsPerEntry)
{
	// https://minecraft.wiki/w/Minecraft_Wiki:Projects/wiki.vg_merge/Chunk_Format#Data_structure

	// We shift a UInt64 by a_BitsPerEntry, the latter cannot be too big:
	ASSERT(a_BitsPerEntry < 64);

	UInt64 Buffer = 0;  // A buffer to compose multiple smaller bitsizes into one 64-bit number
	unsigned char BitIndex = 0;  // The bit-position in Buffer that represents where to write next

	const bool BlocksExist = a_Blocks != nullptr;
	const bool MetasExist = a_Metas != nullptr;

	for (size_t Index = 0; Index != ChunkBlockData::SectionBlockCount; Index++)
	{
		const BLOCKTYPE BlockType = BlocksExist ? (*a_Blocks)[Index] : 0;
		const NIBBLETYPE BlockMeta = MetasExist ? cChunkDef::ExpandNibble(a_Metas->data(), Index) : 0;
		const auto Value = Palette(BlockType, BlockMeta);

		// Write as much as possible of Value, starting from BitIndex, into Buffer:
		Buffer |= static_cast<UInt64>(Value) << BitIndex;

		// The _signed_ count of bits in Value left to write
		const auto Remaining = static_cast<char>(a_BitsPerEntry - (64 - BitIndex));
		if (Remaining >= 0)
		{
			// There were some bits remaining: we've filled the buffer. Flush it:
			m_Packet.WriteBEUInt64(Buffer);

			// And write the remaining bits, setting the new BitIndex:
			Buffer = static_cast<UInt64>(Value >> (a_BitsPerEntry - Remaining));
			BitIndex = static_cast<unsigned char>(Remaining);
		}
		else
		{
			// It fit, excellent.
			BitIndex += a_BitsPerEntry;
		}
	}

	static_assert((ChunkBlockData::SectionBlockCount % 64) == 0, "Section must fit wholly into a 64-bit long array");
	ASSERT(BitIndex == 0);
	ASSERT(Buffer == 0);
}





inline void cChunkDataSerializer::WriteLightSectionGrouped(const ChunkLightData::LightArray * const a_BlockLights, const ChunkLightData::LightArray * const a_SkyLights)
{
	// Write lighting:
	if (a_BlockLights == nullptr)
	{
		m_Packet.WriteBuf(ChunkLightData::SectionLightCount, ChunkLightData::DefaultBlockLightValue);
	}
	else
	{
		m_Packet.WriteBuf(a_BlockLights->data(), a_BlockLights->size());
	}

	// Skylight is only sent in the overworld; the nether and end do not use it:
	if (m_Dimension == dimOverworld)
	{
		if (a_SkyLights == nullptr)
		{
			m_Packet.WriteBuf(ChunkLightData::SectionLightCount, ChunkLightData::DefaultSkyLightValue);
		}
		else
		{
			m_Packet.WriteBuf(a_SkyLights->data(), a_SkyLights->size());
		}
	}
}





inline void cChunkDataSerializer::CompressPacketInto(ChunkDataCache & a_Cache)
{
	m_Compressor.ReadFrom(m_Packet);
	m_Packet.CommitRead();

	cProtocol_1_8_0::CompressPacket(m_Compressor, a_Cache.ToSend);

	a_Cache.Engaged = true;
}
