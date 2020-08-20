#include "Globals.h"
#include "ChunkDataSerializer.h"
#include "zlib/zlib.h"
#include "Protocol_1_8.h"
#include "Protocol_1_9.h"
#include "../ByteBuffer.h"
#include "../ClientHandle.h"
#include "../WorldStorage/FastNBT.h"

#include "Palettes/Upgrade.h"
#include "Palettes/Palette_1_13.h"
#include "Palettes/Palette_1_13_1.h"
#include "Palettes/Palette_1_14.h"





/** Calls the given function with every present chunk section. */
template <class Func>
void ForEachSection(const cChunkData & a_Data, Func a_Func)
{
	for (size_t SectionIdx = 0; SectionIdx < cChunkData::NumSections; ++SectionIdx)
	{
		auto Section = a_Data.GetSection(SectionIdx);
		if (Section != nullptr)
		{
			a_Func(*Section);
		}
	}
}





namespace
{
	auto PaletteLegacy(const BLOCKTYPE a_BlockType, const NIBBLETYPE a_Meta)
	{
		return (a_BlockType << 4) | a_Meta;
	}

	auto Palette393(const BLOCKTYPE a_BlockType, const NIBBLETYPE a_Meta)
	{
		return Palette_1_13::FromBlock(PaletteUpgrade::FromBlock(a_BlockType, a_Meta));
	}

	auto Palette401(const BLOCKTYPE a_BlockType, const NIBBLETYPE a_Meta)
	{
		return Palette_1_13_1::FromBlock(PaletteUpgrade::FromBlock(a_BlockType, a_Meta));
	}

	auto Palette477(const BLOCKTYPE a_BlockType, const NIBBLETYPE a_Meta)
	{
		return Palette_1_14::FromBlock(PaletteUpgrade::FromBlock(a_BlockType, a_Meta));
	}
}





////////////////////////////////////////////////////////////////////////////////
// cChunkDataSerializer:

cChunkDataSerializer::cChunkDataSerializer(
	int                   a_ChunkX,
	int                   a_ChunkZ,
	const cChunkData &    a_Data,
	const unsigned char * a_BiomeData,
	const eDimension      a_Dimension
) :
	m_ChunkX(a_ChunkX),
	m_ChunkZ(a_ChunkZ),
	m_Data(a_Data),
	m_BiomeData(a_BiomeData),
	m_Dimension(a_Dimension)
{
}





void cChunkDataSerializer::SendToClients(const std::unordered_set<cClientHandle *> & a_SendTo)
{
	std::unordered_map<cProtocol::Version, std::vector<cClientHandle *>> ClientProtocolVersions;

	for (const auto Client : a_SendTo)
	{
		const auto ClientProtocol = static_cast<cProtocol::Version>(Client->GetProtocolVersion());
		ClientProtocolVersions[ClientProtocol].emplace_back(Client);
	}

	for (const auto & Entry : ClientProtocolVersions)
	{
		switch (Entry.first)
		{
			case cProtocol::Version::v1_8_0:
			{
				Serialize47(Entry.second);
				continue;
			}
			case cProtocol::Version::v1_9_0:
			case cProtocol::Version::v1_9_1:
			case cProtocol::Version::v1_9_2:
			{
				Serialize107(Entry.second);
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
				Serialize110(Entry.second);
				continue;
			}
			case cProtocol::Version::v1_13:
			{
				Serialize393<&Palette393>(Entry.second);  // This version didn't last very long xD
				continue;
			}
			case cProtocol::Version::v1_13_1:
			case cProtocol::Version::v1_13_2:
			{
				Serialize393<&Palette401>(Entry.second);
				continue;
			}
			case cProtocol::Version::v1_14:
			{
				Serialize477(Entry.second);
				continue;
			}
		}

		UNREACHABLE("Unknown chunk data serialization version");
	}
}





void cChunkDataSerializer::Serialize47(const std::vector<cClientHandle *> & a_SendTo)
{
	// This function returns the fully compressed packet (including packet size), not the raw packet!

	// Create the packet:
	cByteBuffer Packet(512 KiB);
	Packet.WriteVarInt32(0x21);  // Packet id (Chunk Data packet)
	Packet.WriteBEInt32(m_ChunkX);
	Packet.WriteBEInt32(m_ChunkZ);
	Packet.WriteBool(true);      // "Ground-up continuous", or rather, "biome data present" flag
	Packet.WriteBEUInt16(m_Data.GetSectionBitmask());

	// Write the chunk size:
	const int BiomeDataSize = cChunkDef::Width * cChunkDef::Width;
	UInt32 ChunkSize = (
		m_Data.NumPresentSections() * cChunkData::SectionBlockCount * 3 +  // Blocks and lighting
		BiomeDataSize    // Biome data
	);
	Packet.WriteVarInt32(ChunkSize);

	// Chunk written as seperate arrays of (blocktype + meta), blocklight and skylight
	// each array stores all present sections of the same kind packed together

	// Write the block types to the packet:
	ForEachSection(m_Data, [&](const cChunkData::sChunkSection & a_Section)
		{
			for (size_t BlockIdx = 0; BlockIdx != cChunkData::SectionBlockCount; ++BlockIdx)
			{
				BLOCKTYPE BlockType = a_Section.m_BlockTypes[BlockIdx] & 0xFF;
				NIBBLETYPE BlockMeta = a_Section.m_BlockMetas[BlockIdx / 2] >> ((BlockIdx & 1) * 4) & 0x0f;
				Packet.WriteBEUInt8(static_cast<unsigned char>(BlockType << 4) | BlockMeta);
				Packet.WriteBEUInt8(static_cast<unsigned char>(BlockType >> 4));
			}
		}
	);

	// Write the block lights:
	ForEachSection(m_Data, [&](const cChunkData::sChunkSection & a_Section)
		{
			Packet.WriteBuf(a_Section.m_BlockLight, sizeof(a_Section.m_BlockLight));
		}
	);

	// Write the sky lights:
	ForEachSection(m_Data, [&](const cChunkData::sChunkSection & a_Section)
		{
			Packet.WriteBuf(a_Section.m_BlockSkyLight, sizeof(a_Section.m_BlockSkyLight));
		}
	);

	// Write the biome data:
	Packet.WriteBuf(m_BiomeData, BiomeDataSize);

	CompressAndSend(Packet, a_SendTo);
}





void cChunkDataSerializer::Serialize107(const std::vector<cClientHandle *> & a_SendTo)
{
	// This function returns the fully compressed packet (including packet size), not the raw packet!

	// Create the packet:
	cByteBuffer Packet(512 KiB);
	Packet.WriteVarInt32(0x20);  // Packet id (Chunk Data packet)
	Packet.WriteBEInt32(m_ChunkX);
	Packet.WriteBEInt32(m_ChunkZ);
	Packet.WriteBool(true);        // "Ground-up continuous", or rather, "biome data present" flag
	Packet.WriteVarInt32(m_Data.GetSectionBitmask());
	// Write the chunk size:
	const UInt8 BitsPerEntry = 13;
	const size_t ChunkSectionDataArraySize = (cChunkData::SectionBlockCount * BitsPerEntry) / 8 / 8;  // Convert from bit count to long count
	size_t ChunkSectionSize = (
		1 +                                // Bits per block - set to 13, so the global palette is used and the palette has a length of 0
		1 +                                // Palette length
		2 +                                // Data array length VarInt - 2 bytes for the current value
		ChunkSectionDataArraySize * 8 +    // Actual block data - multiplied by 8 because first number is longs
		cChunkData::SectionBlockCount / 2  // Block light
	);

	if (m_Dimension == dimOverworld)
	{
		// Sky light is only sent in the overworld.
		ChunkSectionSize += cChunkData::SectionBlockCount / 2;
	}

	const size_t BiomeDataSize = cChunkDef::Width * cChunkDef::Width;
	size_t ChunkSize = (
		ChunkSectionSize * m_Data.NumPresentSections() +
		BiomeDataSize
	);
	Packet.WriteVarInt32(static_cast<UInt32>(ChunkSize));

	// Write each chunk section...
	ForEachSection(m_Data, [&](const cChunkData::sChunkSection & a_Section)
		{
			Packet.WriteBEUInt8(BitsPerEntry);
			Packet.WriteVarInt32(0);  // Palette length is 0
			Packet.WriteVarInt32(static_cast<UInt32>(ChunkSectionDataArraySize));
			WriteSectionDataSeamless<&PaletteLegacy>(Packet, a_Section, BitsPerEntry);

			// Write lighting:
			Packet.WriteBuf(a_Section.m_BlockLight, sizeof(a_Section.m_BlockLight));
			if (m_Dimension == dimOverworld)
			{
				// Skylight is only sent in the overworld; the nether and end do not use it
				Packet.WriteBuf(a_Section.m_BlockSkyLight, sizeof(a_Section.m_BlockSkyLight));
			}
		}
	);

	// Write the biome data
	Packet.WriteBuf(m_BiomeData, BiomeDataSize);

	CompressAndSend(Packet, a_SendTo);
}





void cChunkDataSerializer::Serialize110(const std::vector<cClientHandle *> & a_SendTo)
{
	// This function returns the fully compressed packet (including packet size), not the raw packet!

	// Create the packet:
	cByteBuffer Packet(512 KiB);
	Packet.WriteVarInt32(0x20);  // Packet id (Chunk Data packet)
	Packet.WriteBEInt32(m_ChunkX);
	Packet.WriteBEInt32(m_ChunkZ);
	Packet.WriteBool(true);        // "Ground-up continuous", or rather, "biome data present" flag
	Packet.WriteVarInt32(m_Data.GetSectionBitmask());
	// Write the chunk size:
	const UInt8 BitsPerEntry = 13;
	const size_t ChunkSectionDataArraySize = (cChunkData::SectionBlockCount * BitsPerEntry) / 8 / 8;  // Convert from bit count to long count
	size_t ChunkSectionSize = (
		1 +                                // Bits per block - set to 13, so the global palette is used and the palette has a length of 0
		1 +                                // Palette length
		2 +                                // Data array length VarInt - 2 bytes for the current value
		ChunkSectionDataArraySize * 8 +    // Actual block data - multiplied by 8 because first number is longs
		cChunkData::SectionBlockCount / 2  // Block light
	);

	if (m_Dimension == dimOverworld)
	{
		// Sky light is only sent in the overworld.
		ChunkSectionSize += cChunkData::SectionBlockCount / 2;
	}

	const size_t BiomeDataSize = cChunkDef::Width * cChunkDef::Width;
	size_t ChunkSize = (
		ChunkSectionSize * m_Data.NumPresentSections() +
		BiomeDataSize
	);
	Packet.WriteVarInt32(static_cast<UInt32>(ChunkSize));

	// Write each chunk section...
	ForEachSection(m_Data, [&](const cChunkData::sChunkSection & a_Section)
		{
			Packet.WriteBEUInt8(BitsPerEntry);
			Packet.WriteVarInt32(0);  // Palette length is 0
			Packet.WriteVarInt32(static_cast<UInt32>(ChunkSectionDataArraySize));
			WriteSectionDataSeamless<&PaletteLegacy>(Packet, a_Section, BitsPerEntry);

			// Write lighting:
			Packet.WriteBuf(a_Section.m_BlockLight, sizeof(a_Section.m_BlockLight));
			if (m_Dimension == dimOverworld)
			{
				// Skylight is only sent in the overworld; the nether and end do not use it
				Packet.WriteBuf(a_Section.m_BlockSkyLight, sizeof(a_Section.m_BlockSkyLight));
			}
		}
	);

	// Write the biome data
	Packet.WriteBuf(m_BiomeData, BiomeDataSize);

	// Identify 1.9.4's tile entity list as empty
	Packet.WriteBEUInt8(0);

	CompressAndSend(Packet, a_SendTo);
}





template <auto Palette>
void cChunkDataSerializer::Serialize393(const std::vector<cClientHandle *> & a_SendTo)
{
	// This function returns the fully compressed packet (including packet size), not the raw packet!

	// Create the packet:
	cByteBuffer Packet(512 KiB);
	Packet.WriteVarInt32(0x22);  // Packet id (Chunk Data packet)
	Packet.WriteBEInt32(m_ChunkX);
	Packet.WriteBEInt32(m_ChunkZ);
	Packet.WriteBool(true);  // "Ground-up continuous", or rather, "biome data present" flag
	Packet.WriteVarInt32(m_Data.GetSectionBitmask());

	// Write the chunk size in bytes:
	const UInt8 BitsPerEntry = 14;
	const size_t ChunkSectionDataArraySize = (cChunkData::SectionBlockCount * BitsPerEntry) / 8 / 8;
	size_t ChunkSectionSize = (
		1 +  // Bits per entry, BEUInt8, 1 byte
		Packet.GetVarIntSize(static_cast<UInt32>(ChunkSectionDataArraySize)) +  // Field containing "size of whole section", VarInt32, variable size
		ChunkSectionDataArraySize * 8 +  // Actual section data, lots of bytes (multiplier 1 long = 8 bytes)
		cChunkData::SectionBlockCount / 2  // Size of blocklight which is always sent
	);

	if (m_Dimension == dimOverworld)
	{
		// Sky light is only sent in the overworld.
		ChunkSectionSize += cChunkData::SectionBlockCount / 2;
	}

	const size_t BiomeDataSize = cChunkDef::Width * cChunkDef::Width;
	size_t ChunkSize = (
		ChunkSectionSize * m_Data.NumPresentSections() +
		BiomeDataSize * 4  // Biome data now BE ints
	);
	Packet.WriteVarInt32(static_cast<UInt32>(ChunkSize));

	// Write each chunk section...
	ForEachSection(m_Data, [&](const cChunkData::sChunkSection & a_Section)
		{
			Packet.WriteBEUInt8(BitsPerEntry);
			Packet.WriteVarInt32(static_cast<UInt32>(ChunkSectionDataArraySize));
			WriteSectionDataSeamless<Palette>(Packet, a_Section, BitsPerEntry);

			// Write lighting:
			Packet.WriteBuf(a_Section.m_BlockLight, sizeof(a_Section.m_BlockLight));
			if (m_Dimension == dimOverworld)
			{
				// Skylight is only sent in the overworld; the nether and end do not use it
				Packet.WriteBuf(a_Section.m_BlockSkyLight, sizeof(a_Section.m_BlockSkyLight));
			}
		}
	);

	// Write the biome data
	for (size_t i = 0; i != BiomeDataSize; i++)
	{
		Packet.WriteBEUInt32(static_cast<UInt32>(m_BiomeData[i]) & 0xff);
	}

	// Identify 1.9.4's tile entity list as empty
	Packet.WriteVarInt32(0);

	CompressAndSend(Packet, a_SendTo);
}





void cChunkDataSerializer::Serialize477(const std::vector<cClientHandle *> & a_SendTo)
{
	// This function returns the fully compressed packet (including packet size), not the raw packet!

	// Create the packet:
	cByteBuffer Packet(512 KiB);
	Packet.WriteVarInt32(0x21);  // Packet id (Chunk Data packet)
	Packet.WriteBEInt32(m_ChunkX);
	Packet.WriteBEInt32(m_ChunkZ);
	Packet.WriteBool(true);  // "Ground-up continuous", or rather, "biome data present" flag
	Packet.WriteVarInt32(m_Data.GetSectionBitmask());

	{
		cFastNBTWriter Writer;
		// TODO: client works fine without?
		// std::array<Int64, 36> Longz = {};
		// Writer.AddLongArray("MOTION_BLOCKING", Longz.data(), Longz.size());
		Writer.Finish();
		Packet.Write(Writer.GetResult().data(), Writer.GetResult().size());
	}

	// Write the chunk size in bytes:
	const UInt8 BitsPerEntry = 14;
	const size_t ChunkSectionDataArraySize = (cChunkData::SectionBlockCount * BitsPerEntry) / 8 / 8;
	const size_t ChunkSectionSize = (
		2 +  // Block count, BEInt16, 2 bytes
		1 +  // Bits per entry, BEUInt8, 1 byte
		Packet.GetVarIntSize(static_cast<UInt32>(ChunkSectionDataArraySize)) +  // Field containing "size of whole section", VarInt32, variable size
		ChunkSectionDataArraySize * 8  // Actual section data, lots of bytes (multiplier 1 long = 8 bytes)
	);

	const size_t BiomeDataSize = cChunkDef::Width * cChunkDef::Width;
	const size_t ChunkSize = (
		ChunkSectionSize * m_Data.NumPresentSections() +
		BiomeDataSize * 4  // Biome data now BE ints
	);
	Packet.WriteVarInt32(static_cast<UInt32>(ChunkSize));

	// Write each chunk section...
	ForEachSection(m_Data, [&](const cChunkData::sChunkSection & a_Section)
		{
			Packet.WriteBEInt16(-1);
			Packet.WriteBEUInt8(BitsPerEntry);
			Packet.WriteVarInt32(static_cast<UInt32>(ChunkSectionDataArraySize));
			WriteSectionDataSeamless<&Palette477>(Packet, a_Section, BitsPerEntry);
		}
	);

	// Write the biome data
	for (size_t i = 0; i != BiomeDataSize; i++)
	{
		Packet.WriteBEUInt32(static_cast<UInt32>(m_BiomeData[i]) & 0xff);
	}

	// Identify 1.9.4's tile entity list as empty
	Packet.WriteVarInt32(0);

	CompressAndSend(Packet, a_SendTo);
}





template <auto Palette>
void cChunkDataSerializer::WriteSectionDataSeamless(cByteBuffer & a_Packet, const cChunkData::sChunkSection & a_Section, const UInt8 a_BitsPerEntry)
{
	// https://wiki.vg/Chunk_Format#Data_structure

	// We shift a UInt64 by a_BitsPerEntry, the latter cannot be too big:
	ASSERT(a_BitsPerEntry < 64);

	UInt64 Buffer = 0;  // A buffer to compose multiple smaller bitsizes into one 64-bit number
	unsigned char BitIndex = 0;  // The bit-position in Buffer that represents where to write next

	for (size_t Index = 0; Index != cChunkData::SectionBlockCount; Index++)
	{
		const UInt32 BlockType = a_Section.m_BlockTypes[Index];
		const UInt32 BlockMeta = (a_Section.m_BlockMetas[Index / 2] >> ((Index % 2) * 4)) & 0x0f;
		const UInt32 Value = Palette(BlockType, BlockMeta);

		// Write as much as possible of Value, starting from BitIndex, into Buffer:
		Buffer |= static_cast<UInt64>(Value) << BitIndex;

		// The _signed_ count of bits in Value left to write
		const char Remaining = a_BitsPerEntry - (64 - BitIndex);
		if (Remaining >= 0)
		{
			// There were some bits remaining: we've filled the buffer. Flush it:
			a_Packet.WriteBEUInt64(Buffer);

			// And write the remaining bits, setting the new BitIndex:
			Buffer = Value >> (a_BitsPerEntry - Remaining);
			BitIndex = Remaining;
		}
		else
		{
			// It fit, sexcellent.
			BitIndex += a_BitsPerEntry;
		}
	}

	static_assert((cChunkData::SectionBlockCount % 64) == 0, "Section must fit wholly into a 64-bit long array");
	ASSERT(BitIndex == 0);
	ASSERT(Buffer == 0);
}





void cChunkDataSerializer::CompressAndSend(cByteBuffer & a_Packet, const std::vector<cClientHandle *> & a_SendTo)
{
	AString PacketData;
	a_Packet.ReadAll(PacketData);

	AString ToSend;
	if (!cProtocol_1_8_0::CompressPacket(PacketData, ToSend))
	{
		ASSERT(!"Packet compression failed.");
		return;
	}

	for (const auto Client : a_SendTo)
	{
		Client->SendChunkData(m_ChunkX, m_ChunkZ, ToSend);
	}
}
