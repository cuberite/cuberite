
// ChunkDataSerializer.cpp

// Implements the cChunkDataSerializer class representing the object that can:
//  - serialize chunk data to different protocol versions
//  - cache such serialized data for multiple clients

#include "Globals.h"
#include "ChunkDataSerializer.h"
#include "zlib/zlib.h"
#include "ByteBuffer.h"
#include "Protocol_1_8.h"
#include "Protocol_1_9.h"





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
	const cChunkData &    a_Data,
	const unsigned char * a_BiomeData,
	const eDimension      a_Dimension
):
	m_Data(a_Data),
	m_BiomeData(a_BiomeData),
	m_Dimension(a_Dimension)
{
}





const AString & cChunkDataSerializer::Serialize(int a_Version, int a_ChunkX, int a_ChunkZ)
{
	Serializations::const_iterator itr = m_Serializations.find(a_Version);
	if (itr != m_Serializations.end())
	{
		return itr->second;
	}

	AString data;
	switch (a_Version)
	{
		case RELEASE_1_8_0: Serialize47(data, a_ChunkX, a_ChunkZ); break;
		case RELEASE_1_9_0: Serialize107(data, a_ChunkX, a_ChunkZ); break;
		case RELEASE_1_9_4: Serialize110(data, a_ChunkX, a_ChunkZ); break;
		// TODO: Other protocol versions may serialize the data differently; implement here

		default:
		{
			LOGERROR("cChunkDataSerializer::Serialize(): Unknown version: %d", a_Version);
			ASSERT(!"Unknown chunk data serialization version");
			break;
		}
	}
	if (!data.empty())
	{
		m_Serializations[a_Version] = data;
	}
	return m_Serializations[a_Version];
}





void cChunkDataSerializer::Serialize47(AString & a_Data, int a_ChunkX, int a_ChunkZ)
{
	// This function returns the fully compressed packet (including packet size), not the raw packet!

	// Create the packet:
	cByteBuffer Packet(512 KiB);
	Packet.WriteVarInt32(0x21);  // Packet id (Chunk Data packet)
	Packet.WriteBEInt32(a_ChunkX);
	Packet.WriteBEInt32(a_ChunkZ);
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

	AString PacketData;
	Packet.ReadAll(PacketData);
	Packet.CommitRead();

	cByteBuffer Buffer(20);
	if (PacketData.size() >= 256)
	{
		if (!cProtocol_1_8_0::CompressPacket(PacketData, a_Data))
		{
			ASSERT(!"Packet compression failed.");
			a_Data.clear();
			return;
		}
	}
	else
	{
		AString PostData;
		Buffer.WriteVarInt32(static_cast<UInt32>(Packet.GetUsedSpace() + 1));
		Buffer.WriteVarInt32(0);
		Buffer.ReadAll(PostData);
		Buffer.CommitRead();

		a_Data.clear();
		a_Data.reserve(PostData.size() + PacketData.size());
		a_Data.append(PostData.data(), PostData.size());
		a_Data.append(PacketData.data(), PacketData.size());
	}
}





void cChunkDataSerializer::Serialize107(AString & a_Data, int a_ChunkX, int a_ChunkZ)
{
	// This function returns the fully compressed packet (including packet size), not the raw packet!

	// Create the packet:
	cByteBuffer Packet(512 KiB);
	Packet.WriteVarInt32(0x20);  // Packet id (Chunk Data packet)
	Packet.WriteBEInt32(a_ChunkX);
	Packet.WriteBEInt32(a_ChunkZ);
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
			Packet.WriteBEUInt8(BitsPerEntry);
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

	AString PacketData;
	Packet.ReadAll(PacketData);
	Packet.CommitRead();

	cByteBuffer Buffer(20);
	if (PacketData.size() >= 256)
	{
		if (!cProtocol_1_9_0::CompressPacket(PacketData, a_Data))
		{
			ASSERT(!"Packet compression failed.");
			a_Data.clear();
			return;
		}
	}
	else
	{
		AString PostData;
		Buffer.WriteVarInt32(static_cast<UInt32>(Packet.GetUsedSpace() + 1));
		Buffer.WriteVarInt32(0);
		Buffer.ReadAll(PostData);
		Buffer.CommitRead();

		a_Data.clear();
		a_Data.reserve(PostData.size() + PacketData.size());
		a_Data.append(PostData.data(), PostData.size());
		a_Data.append(PacketData.data(), PacketData.size());
	}
}





void cChunkDataSerializer::Serialize110(AString & a_Data, int a_ChunkX, int a_ChunkZ)
{
	// This function returns the fully compressed packet (including packet size), not the raw packet!

	// Create the packet:
	cByteBuffer Packet(512 KiB);
	Packet.WriteVarInt32(0x20);  // Packet id (Chunk Data packet)
	Packet.WriteBEInt32(a_ChunkX);
	Packet.WriteBEInt32(a_ChunkZ);
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
			Packet.WriteBEUInt8(BitsPerEntry);
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

	AString PacketData;
	Packet.ReadAll(PacketData);
	Packet.CommitRead();

	cByteBuffer Buffer(20);
	if (PacketData.size() >= 256)
	{
		if (!cProtocol_1_9_0::CompressPacket(PacketData, a_Data))
		{
			ASSERT(!"Packet compression failed.");
			a_Data.clear();
			return;
		}
	}
	else
	{
		AString PostData;
		Buffer.WriteVarInt32(static_cast<UInt32>(Packet.GetUsedSpace() + 1));
		Buffer.WriteVarInt32(0);
		Buffer.ReadAll(PostData);
		Buffer.CommitRead();

		a_Data.clear();
		a_Data.reserve(PostData.size() + PacketData.size());
		a_Data.append(PostData.data(), PostData.size());
		a_Data.append(PacketData.data(), PacketData.size());
	}
}




