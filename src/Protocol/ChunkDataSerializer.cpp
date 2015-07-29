
// ChunkDataSerializer.cpp

// Implements the cChunkDataSerializer class representing the object that can:
//  - serialize chunk data to different protocol versions
//  - cache such serialized data for multiple clients

#include "Globals.h"
#include "ChunkDataSerializer.h"
#include "zlib/zlib.h"
#include "ByteBuffer.h"
#include "Protocol18x.h"




cChunkDataSerializer::cChunkDataSerializer(
	const cChunkDef::BlockTypes   & a_BlockTypes,
	const cChunkDef::BlockNibbles & a_BlockMetas,
	const cChunkDef::BlockNibbles & a_BlockLight,
	const cChunkDef::BlockNibbles & a_BlockSkyLight,
	const unsigned char *           a_BiomeData
) :
	m_BlockTypes(a_BlockTypes),
	m_BlockMetas(a_BlockMetas),
	m_BlockLight(a_BlockLight),
	m_BlockSkyLight(a_BlockSkyLight),
	m_BiomeData(a_BiomeData)
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
		case RELEASE_1_3_2: Serialize39(data); break;
		case RELEASE_1_8_0: Serialize47(data, a_ChunkX, a_ChunkZ); break;
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




void cChunkDataSerializer::Serialize39(AString & a_Data)
{
	// TODO: Do not copy data and then compress it; rather, compress partial blocks of data (zlib can stream)

	const int BiomeDataSize    = cChunkDef::Width * cChunkDef::Width;
	const int MetadataOffset   = sizeof(m_BlockTypes);
	const int BlockLightOffset = MetadataOffset   + sizeof(m_BlockMetas);
	const int SkyLightOffset   = BlockLightOffset + sizeof(m_BlockLight);
	const int BiomeOffset      = SkyLightOffset   + sizeof(m_BlockSkyLight);
	const int DataSize         = BiomeOffset      + BiomeDataSize;
	
	// Temporary buffer for the composed data:
	char AllData [DataSize];

	memcpy(AllData, m_BlockTypes, sizeof(m_BlockTypes));
	memcpy(AllData + MetadataOffset,   m_BlockMetas,    sizeof(m_BlockMetas));
	memcpy(AllData + BlockLightOffset, m_BlockLight,    sizeof(m_BlockLight));
	memcpy(AllData + SkyLightOffset,   m_BlockSkyLight, sizeof(m_BlockSkyLight));
	memcpy(AllData + BiomeOffset,      m_BiomeData,     BiomeDataSize);

	// Compress the data:
	// In order not to use allocation, use a fixed-size buffer, with the size
	// that uses the same calculation as compressBound():
	const uLongf CompressedMaxSize = DataSize + (DataSize >> 12) + (DataSize >> 14) + (DataSize >> 25) + 16;
	char CompressedBlockData[CompressedMaxSize];

	uLongf CompressedSize = compressBound(DataSize);
	
	// Run-time check that our compile-time guess about CompressedMaxSize was enough:
	ASSERT(CompressedSize <= CompressedMaxSize);
	
	compress2(reinterpret_cast<Bytef*>(CompressedBlockData), &CompressedSize, reinterpret_cast<const Bytef*>(AllData), sizeof(AllData), Z_DEFAULT_COMPRESSION);

	// Now put all those data into a_Data:
	
	// "Ground-up continuous", or rather, "biome data present" flag:
	a_Data.push_back('\x01');
	
	// Two bitmaps; we're aways sending the full chunk with no additional data, so the bitmaps are 0xffff and 0, respectively
	// Also, no endian flipping is needed because of the const values
	unsigned short BitMap1 = 0xffff;
	unsigned short BitMap2 = 0;
	a_Data.append(reinterpret_cast<const char *>(&BitMap1), sizeof(short));
	a_Data.append(reinterpret_cast<const char *>(&BitMap2), sizeof(short));
	
	UInt32 CompressedSizeBE = htonl(static_cast<UInt32>(CompressedSize));
	a_Data.append(reinterpret_cast<const char *>(&CompressedSizeBE), sizeof(CompressedSizeBE));
	
	// Unlike 29, 39 doesn't have the "unused" int
	
	a_Data.append(CompressedBlockData, CompressedSize);
}





void cChunkDataSerializer::Serialize47(AString & a_Data, int a_ChunkX, int a_ChunkZ)
{
	// This function returns the fully compressed packet (including packet size), not the raw packet!

	// Create the packet:
	cByteBuffer Packet(512 KiB);
	Packet.WriteVarInt32(0x21);  // Packet id (Chunk Data packet)
	Packet.WriteBEInt32(a_ChunkX);
	Packet.WriteBEInt32(a_ChunkZ);
	Packet.WriteBool(true);        // "Ground-up continuous", or rather, "biome data present" flag
	Packet.WriteBEUInt16(0xffff);  // We're aways sending the full chunk with no additional data, so the bitmap is 0xffff

	// Write the chunk size:
	const int BiomeDataSize = cChunkDef::Width * cChunkDef::Width;
	UInt32 ChunkSize = (
		(cChunkDef::NumBlocks * 2) +  // Block meta + type
		sizeof(m_BlockLight) +        // Block light
		sizeof(m_BlockSkyLight) +     // Block sky light
		BiomeDataSize                 // Biome data
	);
	Packet.WriteVarInt32(ChunkSize);

	// Write the block types to the packet:
	for (size_t Index = 0; Index < cChunkDef::NumBlocks; Index++)
	{
		BLOCKTYPE BlockType = m_BlockTypes[Index] & 0xFF;
		NIBBLETYPE BlockMeta = m_BlockMetas[Index / 2] >> ((Index & 1) * 4) & 0x0f;
		Packet.WriteBEUInt8(static_cast<unsigned char>(BlockType << 4) | BlockMeta);
		Packet.WriteBEUInt8(static_cast<unsigned char>(BlockType >> 4));
	}

	// Write the rest:
	Packet.WriteBuf(m_BlockLight,    sizeof(m_BlockLight));
	Packet.WriteBuf(m_BlockSkyLight, sizeof(m_BlockSkyLight));
	Packet.WriteBuf(m_BiomeData,     BiomeDataSize);

	AString PacketData;
	Packet.ReadAll(PacketData);
	Packet.CommitRead();

	cByteBuffer Buffer(20);
	if (PacketData.size() >= 256)
	{
		if (!cProtocol180::CompressPacket(PacketData, a_Data))
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




