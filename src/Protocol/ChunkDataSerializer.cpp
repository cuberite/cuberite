#include "Globals.h"
#include "ChunkDataSerializer.h"
#include "zlib/zlib.h"
#include "Protocol_1_8.h"
#include "Protocol_1_9.h"
#include "../ByteBuffer.h"
#include "../ClientHandle.h"

#include "Palettes/Upgrade.h"
#include "Palettes/Palette_1_13.h"
#include "Palettes/Palette_1_13_1.h"





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
			case cProtocol::Version::Version_1_8_0:
			{
				Serialize47(Entry.second);
				continue;
			}
			case cProtocol::Version::Version_1_9_0:
			case cProtocol::Version::Version_1_9_1:
			case cProtocol::Version::Version_1_9_2:
			{
				Serialize107(Entry.second);
				continue;
			}
			case cProtocol::Version::Version_1_9_4:
			case cProtocol::Version::Version_1_10_0:
			case cProtocol::Version::Version_1_11_0:
			case cProtocol::Version::Version_1_11_1:
			case cProtocol::Version::Version_1_12:
			case cProtocol::Version::Version_1_12_1:
			case cProtocol::Version::Version_1_12_2:
			{
				Serialize110(Entry.second);
				continue;
			}
			case cProtocol::Version::Version_1_13:
			{
				Serialize393And401<&Palette_1_13::FromBlock>(Entry.second);  // This version didn't last very long xD
				continue;
			}
			case cProtocol::Version::Version_1_13_1:
			case cProtocol::Version::Version_1_13_2:
			{
				Serialize393And401<&Palette_1_13_1::FromBlock>(Entry.second);
				continue;
			}
		}

		LOGERROR("cChunkDataSerializer::Serialize(): Unknown version: %d", Entry.first);
		ASSERT(!"Unknown chunk data serialization version");
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
	const size_t BitsPerEntry = 13;
	const size_t Mask = (1 << BitsPerEntry) - 1;  // Creates a mask that is 13 bits long, ie 0b1111111111111
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
			Packet.WriteBEUInt8(static_cast<UInt8>(BitsPerEntry));
			Packet.WriteVarInt32(0);  // Palette length is 0
			Packet.WriteVarInt32(static_cast<UInt32>(ChunkSectionDataArraySize));

			UInt64 TempLong = 0;  // Temporary value that will be stored into
			UInt64 CurrentlyWrittenIndex = 0;  // "Index" of the long that would be written to

			for (size_t Index = 0; Index < cChunkData::SectionBlockCount; Index++)
			{
				UInt64 Value = static_cast<UInt64>(a_Section.m_BlockTypes[Index] << 4);
				if (Index % 2 == 0)
				{
					Value |= a_Section.m_BlockMetas[Index / 2] & 0x0f;
				}
				else
				{
					Value |= a_Section.m_BlockMetas[Index / 2] >> 4;
				}
				Value &= Mask;  // It shouldn't go out of bounds, but it's still worth being careful

				// Painful part where we write data into the long array.  Based off of the normal code.
				size_t BitPosition = Index * BitsPerEntry;
				size_t FirstIndex = BitPosition / 64;
				size_t SecondIndex = ((Index + 1) * BitsPerEntry - 1) / 64;
				size_t BitOffset = BitPosition % 64;

				if (FirstIndex != CurrentlyWrittenIndex)
				{
					// Write the current data before modifiying it.
					Packet.WriteBEUInt64(TempLong);
					TempLong = 0;
					CurrentlyWrittenIndex = FirstIndex;
				}

				TempLong |= (Value << BitOffset);

				if (FirstIndex != SecondIndex)
				{
					// Part of the data is now in the second long; write the first one first
					Packet.WriteBEUInt64(TempLong);
					CurrentlyWrittenIndex = SecondIndex;

					TempLong = (Value >> (64 - BitOffset));
				}
			}
			// The last long will generally not be written
			Packet.WriteBEUInt64(TempLong);

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
	const size_t BitsPerEntry = 13;
	const size_t Mask = (1 << BitsPerEntry) - 1;  // Creates a mask that is 13 bits long, ie 0b1111111111111
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
			Packet.WriteBEUInt8(static_cast<UInt8>(BitsPerEntry));
			Packet.WriteVarInt32(0);  // Palette length is 0
			Packet.WriteVarInt32(static_cast<UInt32>(ChunkSectionDataArraySize));

			UInt64 TempLong = 0;  // Temporary value that will be stored into
			UInt64 CurrentlyWrittenIndex = 0;  // "Index" of the long that would be written to

			for (size_t Index = 0; Index < cChunkData::SectionBlockCount; Index++)
			{
				UInt64 Value = static_cast<UInt64>(a_Section.m_BlockTypes[Index] << 4);
				if (Index % 2 == 0)
				{
					Value |= a_Section.m_BlockMetas[Index / 2] & 0x0f;
				}
				else
				{
					Value |= a_Section.m_BlockMetas[Index / 2] >> 4;
				}
				Value &= Mask;  // It shouldn't go out of bounds, but it's still worth being careful

				// Painful part where we write data into the long array.  Based off of the normal code.
				size_t BitPosition = Index * BitsPerEntry;
				size_t FirstIndex = BitPosition / 64;
				size_t SecondIndex = ((Index + 1) * BitsPerEntry - 1) / 64;
				size_t BitOffset = BitPosition % 64;

				if (FirstIndex != CurrentlyWrittenIndex)
				{
					// Write the current data before modifiying it.
					Packet.WriteBEUInt64(TempLong);
					TempLong = 0;
					CurrentlyWrittenIndex = FirstIndex;
				}

				TempLong |= (Value << BitOffset);

				if (FirstIndex != SecondIndex)
				{
					// Part of the data is now in the second long; write the first one first
					Packet.WriteBEUInt64(TempLong);
					CurrentlyWrittenIndex = SecondIndex;

					TempLong = (Value >> (64 - BitOffset));
				}
			}
			// The last long will generally not be written
			Packet.WriteBEUInt64(TempLong);

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
void cChunkDataSerializer::Serialize393And401(const std::vector<cClientHandle *> & a_SendTo)
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
	const size_t BitsPerEntry = 14;
	const size_t Mask = (1 << BitsPerEntry) - 1;
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
			Packet.WriteBEUInt8(static_cast<UInt8>(BitsPerEntry));
			Packet.WriteVarInt32(static_cast<UInt32>(ChunkSectionDataArraySize));

			UInt64 TempLong = 0;  // Temporary value that will be stored into
			UInt64 CurrentlyWrittenIndex = 0;  // "Index" of the long that would be written to

			for (size_t Index = 0; Index < cChunkData::SectionBlockCount; Index++)
			{
				UInt32 blockType = a_Section.m_BlockTypes[Index];
				UInt32 blockMeta = (a_Section.m_BlockMetas[Index / 2] >> ((Index % 2) * 4)) & 0x0f;
				UInt64 Value = Palette(PaletteUpgrade::FromBlock(blockType, blockMeta));
				Value &= Mask;  // It shouldn't go out of bounds, but it's still worth being careful

				// Painful part where we write data into the long array.  Based off of the normal code.
				size_t BitPosition = Index * BitsPerEntry;
				size_t FirstIndex = BitPosition / 64;
				size_t SecondIndex = ((Index + 1) * BitsPerEntry - 1) / 64;
				size_t BitOffset = BitPosition % 64;

				if (FirstIndex != CurrentlyWrittenIndex)
				{
					// Write the current data before modifiying it.
					Packet.WriteBEUInt64(TempLong);
					TempLong = 0;
					CurrentlyWrittenIndex = FirstIndex;
				}

				TempLong |= (Value << BitOffset);

				if (FirstIndex != SecondIndex)
				{
					// Part of the data is now in the second long; write the first one first
					Packet.WriteBEUInt64(TempLong);
					CurrentlyWrittenIndex = SecondIndex;

					TempLong = (Value >> (64 - BitOffset));
				}
			}
			// The last long will generally not be written
			Packet.WriteBEUInt64(TempLong);

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
