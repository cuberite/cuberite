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
#include "Palettes/Palette_1_15.h"
#include "Palettes/Palette_1_16.h"
#include "Palettes/Palette_1_18.h"
#include "Palettes/Palette_1_19.h"
#include "Palettes/Palette_1_20.h"
#include "Palettes/Palette_1_21.h"
#include "Palettes/Palette_1_21_2.h"
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

	std::pair<UInt16, size_t> GetSectionBitmask2(const ChunkBlockData & a_BlockData, const ChunkLightData & a_LightData)
	{
		size_t Present = 0;
		UInt16 Mask = 0;

		do
		{
			for (size_t Y = 0; Y < cChunkDef::NumSections; ++Y)
			{
				const auto Blocks = a_BlockData.GetSection(Y);
				const auto BlockLights = a_LightData.GetBlockLightSection(Y);
				const auto SkyLights = a_LightData.GetSkyLightSection(Y);
				if ((Blocks != nullptr) || (BlockLights != nullptr) || (SkyLights != nullptr))
				{
					{
						Present++;
						Mask |= (1 << Y);
					}
				}
			}
		} while (false);

		return { Mask, Present };
	}

	auto PaletteLegacy(const BlockState a_Block)
	{
		auto NumericBlock = PaletteUpgrade::ToBlock(a_Block);
		return (NumericBlock.first << 4) | NumericBlock.second;
	}

	auto Palette393(const BlockState a_Block)
	{
		return Palette_1_13::From(a_Block);
	}

	auto Palette401(const BlockState a_Block)
	{
		return Palette_1_13_1::From(a_Block);
	}

	auto Palette477(const BlockState a_Block)
	{
		return Palette_1_14::From(a_Block);
	}

	auto Palette573(const BlockState a_Block)
	{
		return Palette_1_15::From(a_Block);
	}

	auto Palette754(const BlockState a_Block)
	{
		return Palette_1_16::From(a_Block);
	}

	auto Palette757(const BlockState a_Block)
	{
		// return Palette_1_18::From(a_Block);
		return 0;
	}

	auto Palette759(const BlockState a_Block)
	{
		// return Palette_1_19::From(a_Block);
		return 0;
	}

	// TODO: Implement palettes
	auto Palette760(const BlockState a_Block)
	{
		// return Palette_1_19_1::From(a_Block);
		return 0;
	}

	auto Palette761(const BlockState a_Block)
	{
		// return Palette_1_19_3::From(a_Block);
		return 0;
	}

	auto Palette762(const BlockState a_Block)
	{
		// return Palette_1_19_4::From(a_Block);
		return 0;
	}

	auto Palette766(const BlockState a_Block)
	{
		// return Palette_1_20_5::From(a_Block);
		return 0;
	}

	auto Palette763(const BlockState a_Block)
	{
		return Palette_1_20::From(a_Block);
	}

	auto Palette764(const BlockState a_Block)
	{
		// return Palette_1_20_2::From(a_Block);
		return 0;
	}

	auto Palette767(const BlockState a_Block)
	{
		return Palette_1_21::From(a_Block);
	}

	auto Palette768(const BlockState a_Block)
	{
		return Palette_1_21_2::From(a_Block);
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
			case cProtocol::Version::v1_15:
			case cProtocol::Version::v1_15_1:
			case cProtocol::Version::v1_15_2:
			{
				Serialize(Client, a_ChunkX, a_ChunkZ, a_BlockData, a_LightData, a_BiomeMap, CacheVersion::v573);
				continue;
			}
			case cProtocol::Version::v1_16:
			case cProtocol::Version::v1_16_1:
			{
				Serialize(Client, a_ChunkX, a_ChunkZ, a_BlockData, a_LightData, a_BiomeMap, CacheVersion::v735);
				continue;
			}
			case cProtocol::Version::v1_16_2:
			case cProtocol::Version::v1_16_3:
			case cProtocol::Version::v1_16_4:
			{
				Serialize(Client, a_ChunkX, a_ChunkZ, a_BlockData, a_LightData, a_BiomeMap, CacheVersion::v751);
				continue;
			}
			case cProtocol::Version::v1_17:
			case cProtocol::Version::v1_17_1:
			{
				Serialize(Client, a_ChunkX, a_ChunkZ, a_BlockData, a_LightData, a_BiomeMap, CacheVersion::v755);
				continue;
			}
			case cProtocol::Version::v1_18:
			case cProtocol::Version::v1_18_2:
			{
				Serialize(Client, a_ChunkX, a_ChunkZ, a_BlockData, a_LightData, a_BiomeMap, CacheVersion::v757);
				continue;
			}
			case cProtocol::Version::v1_19:
			{
				Serialize(Client, a_ChunkX, a_ChunkZ, a_BlockData, a_LightData, a_BiomeMap, CacheVersion::v759);
				continue;
			}
			case cProtocol::Version::v1_19_1:
			{
				Serialize(Client, a_ChunkX, a_ChunkZ, a_BlockData, a_LightData, a_BiomeMap, CacheVersion::v760);
				continue;
			}
			case cProtocol::Version::v1_19_3:
			{
				Serialize(Client, a_ChunkX, a_ChunkZ, a_BlockData, a_LightData, a_BiomeMap, CacheVersion::v761);
				continue;
			}
			case cProtocol::Version::v1_19_4:
			{
				Serialize(Client, a_ChunkX, a_ChunkZ, a_BlockData, a_LightData, a_BiomeMap, CacheVersion::v762);
				continue;
			}
			case cProtocol::Version::v1_20:
			{
				Serialize(Client, a_ChunkX, a_ChunkZ, a_BlockData, a_LightData, a_BiomeMap, CacheVersion::v763);
				continue;
			}
			case cProtocol::Version::v1_20_2:
			{
				Serialize(Client, a_ChunkX, a_ChunkZ, a_BlockData, a_LightData, a_BiomeMap, CacheVersion::v764);
				continue;
			}
			case cProtocol::Version::v1_20_3:
			{
				Serialize(Client, a_ChunkX, a_ChunkZ, a_BlockData, a_LightData, a_BiomeMap, CacheVersion::v765);
				continue;
			}
			case cProtocol::Version::v1_20_5:
			{
				Serialize(Client, a_ChunkX, a_ChunkZ, a_BlockData, a_LightData, a_BiomeMap, CacheVersion::v766);
				continue;
			}
			case cProtocol::Version::v1_21:
			{
				Serialize(Client, a_ChunkX, a_ChunkZ, a_BlockData, a_LightData, a_BiomeMap, CacheVersion::v767);
				continue;
			}
			case cProtocol::Version::v1_21_2:
			{
				Serialize(Client, a_ChunkX, a_ChunkZ, a_BlockData, a_LightData, a_BiomeMap, CacheVersion::v768);
				continue;
			}
			case cProtocol::Version::v1_21_4:
			{
				Serialize(Client, a_ChunkX, a_ChunkZ, a_BlockData, a_LightData, a_BiomeMap, CacheVersion::v769);
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
		case CacheVersion::v573:
		{
			Serialize573(a_ChunkX, a_ChunkZ, a_BlockData, a_BlockData, a_LightData, a_BiomeMap);
			break;
		}
		case CacheVersion::v735:
		{
			Serialize735(a_ChunkX, a_ChunkZ, a_BlockData, a_LightData, a_BiomeMap);
			break;
		}
		case CacheVersion::v751:
		{
			Serialize751(a_ChunkX, a_ChunkZ, a_BlockData, a_LightData, a_BiomeMap);
			break;
		}
		case CacheVersion::v755:
		{
			Serialize755(a_ChunkX, a_ChunkZ, a_BlockData, a_LightData, a_BiomeMap);
			break;
		}
		case CacheVersion::v757:
		{
			Serialize757<&Palette757>(a_ChunkX, a_ChunkZ, a_BlockData, a_LightData, a_BiomeMap, 0x22);
			break;
		}
		case CacheVersion::v759:
		{
			Serialize757<&Palette759>(a_ChunkX, a_ChunkZ, a_BlockData, a_LightData, a_BiomeMap, 0x1F);
			break;
		}
		case CacheVersion::v760:
		{
			Serialize757<&Palette760>(a_ChunkX, a_ChunkZ, a_BlockData, a_LightData, a_BiomeMap, 0x21);
			break;
		}
		case CacheVersion::v761:
		{
			Serialize757<&Palette761>(a_ChunkX, a_ChunkZ, a_BlockData, a_LightData, a_BiomeMap, 0x20);
			break;
		}
		case CacheVersion::v762:
		{
			Serialize757<&Palette762>(a_ChunkX, a_ChunkZ, a_BlockData, a_LightData, a_BiomeMap, 0x24);
			break;
		}
		case CacheVersion::v763:
		{
			Serialize763<&Palette763>(a_ChunkX, a_ChunkZ, a_BlockData, a_LightData, a_BiomeMap, 0x24);
			break;
		}
		case CacheVersion::v764:
		case CacheVersion::v765:
		{
			Serialize764<&Palette764>(a_ChunkX, a_ChunkZ, a_BlockData, a_LightData, a_BiomeMap, 0x25);
			break;
		}
		case CacheVersion::v766:
		{
			Serialize764<&Palette766>(a_ChunkX, a_ChunkZ, a_BlockData, a_LightData, a_BiomeMap, 0x27);
			break;
		}
		case CacheVersion::v767:
		{
			Serialize764<&Palette767>(a_ChunkX, a_ChunkZ, a_BlockData, a_LightData, a_BiomeMap, 0x27);
			break;
		}
		case CacheVersion::v768:
		{
			Serialize764<&Palette768>(a_ChunkX, a_ChunkZ, a_BlockData, a_LightData, a_BiomeMap, 0x28);
			break;
		}
		case CacheVersion::v769:  // TODO: fix palette
		{
			Serialize764<&Palette768>(a_ChunkX, a_ChunkZ, a_BlockData, a_LightData, a_BiomeMap, 0x28);
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
	{
		for (size_t Y = 0; Y < cChunkDef::NumSections; ++Y)
		{
			const auto Blocks = a_BlockData.GetSection(Y);
			const auto BlockLights = a_LightData.GetBlockLightSection(Y);
			const auto SkyLights = a_LightData.GetSkyLightSection(Y);
			if ((Blocks != nullptr) || (BlockLights != nullptr) || (SkyLights != nullptr))
			{
				const bool BlocksExist = Blocks != nullptr;
				for (size_t BlockIdx = 0; BlockIdx != ChunkBlockData::SectionBlockCount; ++BlockIdx)
				{
					auto NumericBlock = BlocksExist ? PaletteUpgrade::ToBlock((*Blocks)[BlockIdx]) : std::make_pair<unsigned char, unsigned char>(0, 0);
					m_Packet.WriteBEUInt8(static_cast<unsigned char>(BlocksExist ? ((NumericBlock.first << 4) | NumericBlock.second) : 0));
					m_Packet.WriteBEUInt8(static_cast<unsigned char>(BlocksExist ? (NumericBlock.first >> 4) : 0));
				}
			}
		}
	}

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
		WriteBlockSectionSeamless<&PaletteLegacy>(Blocks, BitsPerEntry);
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
		WriteBlockSectionSeamless<&PaletteLegacy>(Blocks, BitsPerEntry);
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

	const auto Bitmask = GetSectionBitmask2(a_BlockData, a_LightData);

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
		WriteBlockSectionSeamless<Palette>(Blocks, BitsPerEntry);
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

	const auto Bitmask = GetSectionBitmask2(a_BlockData, a_LightData);

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
		WriteBlockSectionSeamless<&Palette477>(Blocks, BitsPerEntry);
	});

	// Write the biome data
	for (size_t i = 0; i != BiomeDataSize; i++)
	{
		m_Packet.WriteBEUInt32(a_BiomeMap[i]);
	}

	// Identify 1.9.4's tile entity list as empty
	m_Packet.WriteVarInt32(0);
}





inline void cChunkDataSerializer::Serialize573(const int a_ChunkX, const int a_ChunkZ, const ChunkBlockData & a_BlockData, const ChunkBlockData & a_BlockData2, const ChunkLightData & a_LightData, const unsigned char * a_BiomeMap)
{
	// This function returns the fully compressed packet (including packet
	// size), not the raw packet! Below variables tagged static because of
	// https://developercommunity.visualstudio.com/content/problem/367326

	static constexpr UInt8 BitsPerEntry = 14;
	static constexpr size_t ChunkSectionDataArraySize = (ChunkBlockData::SectionBlockCount * BitsPerEntry) / 8 / 8;

	const auto Bitmask = GetSectionBitmask2(a_BlockData2, a_LightData);

	// Create the packet:
	m_Packet.WriteVarInt32(0x22);  // Packet id (Chunk Data packet)
	m_Packet.WriteBEInt32(a_ChunkX);
	m_Packet.WriteBEInt32(a_ChunkZ);
	m_Packet.WriteBool(true);   // "Ground-up continuous", or rather, "biome data present" flag
	m_Packet.WriteVarInt32(Bitmask.first);

	{
		cFastNBTWriter Writer;
		// TODO: client works fine without?
		// std::array<Int64, 36> Longz = {};
		// Writer.AddLongArray("MOTION_BLOCKING", Longz.data(), Longz.size());
		Writer.Finish();
		m_Packet.Write(Writer.GetResult().data(), Writer.GetResult().size());
	}

	// BiomeArray
	int HORIZONTAL_SECTION_COUNT = static_cast<int>(round(log(16.0) / log(2.0))) - 2;  // 2
	int VERTICAL_SECTION_COUNT = static_cast<int>(round(log(256.0) / log(2.0))) - 2;   // 6
	int DEFAULT_LENGTH = 1 << (HORIZONTAL_SECTION_COUNT +  HORIZONTAL_SECTION_COUNT + VERTICAL_SECTION_COUNT);  // should be 1024

	for (int i = 0; i < DEFAULT_LENGTH; i++)
	{
		int realx = i % 16;
		int realz = (i / 16) % 16;
		m_Packet.WriteBEInt32(a_BiomeMap[realx + realz * 16]);  //  Biome ???
	}

	const size_t ChunkSectionSize =
		(2 +  // Block count, BEInt16, 2 bytes
		1 +  // Bits per entry, BEUInt8, 1 byte
		m_Packet.GetVarIntSize(static_cast<UInt32>(
			ChunkSectionDataArraySize)) +   // Field containing "size of whole section", VarInt32, variable size
			ChunkSectionDataArraySize * 8   // Actual section data, lots of bytes (multiplier 1 long = 8 bytes)
		);

	const size_t ChunkSize = ChunkSectionSize * Bitmask.second;

	// Write the chunk size in bytes:
	m_Packet.WriteVarInt32(static_cast<UInt32>(ChunkSize));

	// Write each chunk section...
	for (size_t Y = 0; Y < cChunkDef::NumSections; ++Y)
	{
		const auto Blocks = a_BlockData2.GetSection(Y);
		const auto BlockLights = a_LightData.GetBlockLightSection(Y);
		const auto SkyLights = a_LightData.GetSkyLightSection(Y);
		if ((Blocks != nullptr) ||  (BlockLights != nullptr) || (SkyLights != nullptr))
		{
			m_Packet.WriteBEInt16(4096);
			m_Packet.WriteBEUInt8(BitsPerEntry);
			m_Packet.WriteVarInt32(static_cast<UInt32>(ChunkSectionDataArraySize));
			WriteBlockSectionSeamless2<&Palette573>(Blocks, BitsPerEntry, false);
		}
	}

	// Identify 1.9.4's tile entity list as empty
	m_Packet.WriteVarInt32(0);
}





inline void cChunkDataSerializer::Serialize735(const int a_ChunkX, const int a_ChunkZ, const ChunkBlockData & a_BlockData2, const ChunkLightData & a_LightData, const unsigned char * a_BiomeMap)
{
	// This function returns the fully compressed packet (including packet
	// size), not the raw packet! Below variables tagged static because of
	// https://developercommunity.visualstudio.com/content/problem/367326

	static constexpr UInt8 BitsPerEntry = 15;
	static constexpr UInt8 EntriesPerLong = 64 / BitsPerEntry;
	// static int Longs = CeilC<int, float>(ChunkBlockData::SectionBlockCount / EntriesPerLong);
	static size_t ChunkSectionDataArraySize = CeilC<size_t, float>(ChunkBlockData::SectionBlockCount / EntriesPerLong);  //  (ChunkBlockData::SectionBlockCount * BitsPerEntry) / 8 / 8;

	const auto Bitmask = GetSectionBitmask2(a_BlockData2, a_LightData);

	// Create the packet:
	m_Packet.WriteVarInt32(0x21);  // Packet id (Chunk Data packet)
	m_Packet.WriteBEInt32(a_ChunkX);
	m_Packet.WriteBEInt32(a_ChunkZ);
	m_Packet.WriteBool(true);   // "Ground-up continuous", or rather, "biome data present" flag
	m_Packet.WriteBool(false);  // has light???
	m_Packet.WriteVarInt32(Bitmask.first);

	// BiomeArray
	int HORIZONTAL_SECTION_COUNT = static_cast<int>(round(log(16.0) / log(2.0))) - 2;  // 2
	int VERTICAL_SECTION_COUNT = static_cast<int>(round(log(256.0) / log(2.0))) - 2;   // 6
	int DEFAULT_LENGTH = 1 << (HORIZONTAL_SECTION_COUNT +  HORIZONTAL_SECTION_COUNT + VERTICAL_SECTION_COUNT);  // should be 1024

	{
		cFastNBTWriter Writer;
		// TODO: client works fine without?
		// std::array<Int64, 36> Longz = {};
		// Writer.AddLongArray("MOTION_BLOCKING", Longz.data(), Longz.size());
		Writer.Finish();
		m_Packet.Write(Writer.GetResult().data(), Writer.GetResult().size());
	}

	for (int i = 0; i < DEFAULT_LENGTH; i++)
	{
		int realx = i % 16;
		int realz = (i / 16) % 16;
		m_Packet.WriteBEInt32(a_BiomeMap[realx + realz * 16]);  //  Biome ???
	}

	const size_t ChunkSectionSize =
		(2 +  // Block count, BEInt16, 2 bytes
		1 +  // Bits per entry, BEUInt8, 1 byte
		m_Packet.GetVarIntSize(static_cast<UInt32>(ChunkSectionDataArraySize)) +   // Field containing "size of whole section", VarInt32, variable size
			ChunkSectionDataArraySize * 8   // Actual section data, lots of bytes (multiplier 1 long = 8 bytes)
		);

	const size_t ChunkSize = ChunkSectionSize * Bitmask.second;

	// Write the chunk size in bytes:
	m_Packet.WriteVarInt32(static_cast<UInt32>(ChunkSize));

	// Write each chunk section...
	for (size_t Y = 0; Y < cChunkDef::NumSections; ++Y)
	{
		const auto Blocks = a_BlockData2.GetSection(Y);
		const auto BlockLights = a_LightData.GetBlockLightSection(Y);
		const auto SkyLights = a_LightData.GetSkyLightSection(Y);
		if ((Blocks != nullptr) ||  (BlockLights != nullptr) || (SkyLights != nullptr))
		{
			m_Packet.WriteBEInt16(4096);
			m_Packet.WriteBEUInt8(BitsPerEntry);
			m_Packet.WriteVarInt32(static_cast<UInt32>(ChunkSectionDataArraySize));
			WriteBlockSectionSeamless2<&Palette754>(Blocks, BitsPerEntry, true);
		}
	}

	// Identify 1.9.4's tile entity list as empty
	m_Packet.WriteVarInt32(0);
}





inline void cChunkDataSerializer::Serialize751(const int a_ChunkX, const int a_ChunkZ, const ChunkBlockData & a_BlockData2, const ChunkLightData & a_LightData, const unsigned char * a_BiomeMap)
{
	// This function returns the fully compressed packet (including packet
	// size), not the raw packet! Below variables tagged static because of
	// https://developercommunity.visualstudio.com/content/problem/367326

	static constexpr UInt8 BitsPerEntry = 15;
	static constexpr UInt8 EntriesPerLong = 64 / BitsPerEntry;
	// static int Longs = CeilC<int, float>(ChunkBlockData::SectionBlockCount / EntriesPerLong);
	static size_t ChunkSectionDataArraySize = CeilC<size_t, float>(ChunkBlockData::SectionBlockCount / EntriesPerLong);  //  (ChunkBlockData::SectionBlockCount * BitsPerEntry) / 8 / 8;

	const auto Bitmask = GetSectionBitmask2(a_BlockData2, a_LightData);

	// Create the packet:
	m_Packet.WriteVarInt32(0x20);  // Packet id (Chunk Data packet)
	m_Packet.WriteBEInt32(a_ChunkX);
	m_Packet.WriteBEInt32(a_ChunkZ);
	m_Packet.WriteBool(true);   // "Ground-up continuous", or rather, "biome data present" flag
	m_Packet.WriteVarInt32(Bitmask.first);

	// BiomeArray
	int HORIZONTAL_SECTION_COUNT = static_cast<int>(round(log(16.0) / log(2.0))) - 2;  // 2
	int VERTICAL_SECTION_COUNT = static_cast<int>(round(log(256.0) / log(2.0))) - 2;   // 6
	int DEFAULT_LENGTH = 1 << (HORIZONTAL_SECTION_COUNT +  HORIZONTAL_SECTION_COUNT + VERTICAL_SECTION_COUNT);  // should be 1024

	{
		cFastNBTWriter Writer;
		// TODO: client works fine without?
		// std::array<Int64, 36> Longz = {};
		// Writer.AddLongArray("MOTION_BLOCKING", Longz.data(), Longz.size());
		Writer.Finish();
		m_Packet.Write(Writer.GetResult().data(), Writer.GetResult().size());
	}


	m_Packet.WriteVarInt32(static_cast<UInt32>(DEFAULT_LENGTH));
	for (int i = 0; i < DEFAULT_LENGTH; i++)
	{
		int realx = i % 16;
		int realz = (i / 16) % 16;
		m_Packet.WriteVarInt32(a_BiomeMap[realx + realz * 16]);  //  Biome ???
	}

	const size_t ChunkSectionSize =
		(2 +  // Block count, BEInt16, 2 bytes
		1 +  // Bits per entry, BEUInt8, 1 byte
		m_Packet.GetVarIntSize(static_cast<UInt32>(ChunkSectionDataArraySize)) +   // Field containing "size of whole section", VarInt32, variable size
			ChunkSectionDataArraySize * 8   // Actual section data, lots of bytes (multiplier 1 long = 8 bytes)
		);

	const size_t ChunkSize = ChunkSectionSize * Bitmask.second;

	// Write the chunk size in bytes:
	m_Packet.WriteVarInt32(static_cast<UInt32>(ChunkSize));

	// Write each chunk section...
	for (size_t Y = 0; Y < cChunkDef::NumSections; ++Y)
	{
		const auto Blocks = a_BlockData2.GetSection(Y);
		const auto BlockLights = a_LightData.GetBlockLightSection(Y);
		const auto SkyLights = a_LightData.GetSkyLightSection(Y);
		if ((Blocks != nullptr) ||  (BlockLights != nullptr) || (SkyLights != nullptr))
		{
			m_Packet.WriteBEInt16(4096);
			m_Packet.WriteBEUInt8(BitsPerEntry);
			m_Packet.WriteVarInt32(static_cast<UInt32>(ChunkSectionDataArraySize));
			WriteBlockSectionSeamless2<&Palette754>(Blocks, BitsPerEntry, true);
		}
	}

	// Identify 1.9.4's tile entity list as empty
	m_Packet.WriteVarInt32(0);
}





inline void cChunkDataSerializer::Serialize755(const int a_ChunkX, const int a_ChunkZ, const ChunkBlockData & a_BlockData2, const ChunkLightData & a_LightData, const unsigned char * a_BiomeMap)
{
		// This function returns the fully compressed packet (including packet
	// size), not the raw packet! Below variables tagged static because of
	// https://developercommunity.visualstudio.com/content/problem/367326

	static constexpr UInt8 BitsPerEntry = 15;
	static constexpr UInt8 EntriesPerLong = 64 / BitsPerEntry;
	// static int Longs = CeilC<int, float>(ChunkBlockData::SectionBlockCount / EntriesPerLong);
	static size_t ChunkSectionDataArraySize = CeilC<size_t, float>(ChunkBlockData::SectionBlockCount / EntriesPerLong);  //  (ChunkBlockData::SectionBlockCount * BitsPerEntry) / 8 / 8;

	const auto Bitmask = GetSectionBitmask2(a_BlockData2, a_LightData);

	// Create the packet:
	m_Packet.WriteVarInt32(0x22);  // Packet id (Chunk Data packet)
	m_Packet.WriteBEInt32(a_ChunkX);
	m_Packet.WriteBEInt32(a_ChunkZ);
	m_Packet.WriteVarInt32(1);
	m_Packet.WriteBEInt64(Bitmask.first);

	// BiomeArray
	int HORIZONTAL_SECTION_COUNT = static_cast<int>(round(log(16.0) / log(2.0))) - 2;  // 2
	int VERTICAL_SECTION_COUNT = static_cast<int>(round(log(256.0) / log(2.0))) - 2;   // 6
	int DEFAULT_LENGTH = 1 << (HORIZONTAL_SECTION_COUNT +  HORIZONTAL_SECTION_COUNT + VERTICAL_SECTION_COUNT);  // should be 1024

	{
		cFastNBTWriter Writer;
		// TODO: client works fine without?
		// std::array<Int64, 36> Longz = {};
		// Writer.AddLongArray("MOTION_BLOCKING", Longz.data(), Longz.size());
		Writer.Finish();
		m_Packet.Write(Writer.GetResult().data(), Writer.GetResult().size());
	}


	m_Packet.WriteVarInt32(static_cast<UInt32>(DEFAULT_LENGTH));
	for (int i = 0; i < DEFAULT_LENGTH; i++)
	{
		int realx = i % 16;
		int realz = (i / 16) % 16;
		m_Packet.WriteVarInt32(a_BiomeMap[realx + realz * 16]);  //  Biome ???
	}

	const size_t ChunkSectionSize =
		(2 +  // Block count, BEInt16, 2 bytes
		1 +  // Bits per entry, BEUInt8, 1 byte
		m_Packet.GetVarIntSize(static_cast<UInt32>(ChunkSectionDataArraySize)) +   // Field containing "size of whole section", VarInt32, variable size
			ChunkSectionDataArraySize * 8   // Actual section data, lots of bytes (multiplier 1 long = 8 bytes)
		);

	const size_t ChunkSize = ChunkSectionSize * Bitmask.second;

	// Write the chunk size in bytes:
	m_Packet.WriteVarInt32(static_cast<UInt32>(ChunkSize));

	// Write each chunk section...
	for (size_t Y = 0; Y < cChunkDef::NumSections; ++Y)
	{
		const auto Blocks = a_BlockData2.GetSection(Y);
		const auto BlockLights = a_LightData.GetBlockLightSection(Y);
		const auto SkyLights = a_LightData.GetSkyLightSection(Y);
		if ((Blocks != nullptr) ||  (BlockLights != nullptr) || (SkyLights != nullptr))
		{
			m_Packet.WriteBEInt16(4096);
			m_Packet.WriteBEUInt8(BitsPerEntry);
			m_Packet.WriteVarInt32(static_cast<UInt32>(ChunkSectionDataArraySize));
			WriteBlockSectionSeamless2<&Palette754>(Blocks, BitsPerEntry, true);
		}
	}

	// Identify 1.9.4's tile entity list as empty
	m_Packet.WriteVarInt32(0);
}





template <auto Palette>
inline void cChunkDataSerializer::Serialize757(const int a_ChunkX, const int a_ChunkZ, const ChunkBlockData & a_BlockData2, const ChunkLightData & a_LightData, const unsigned char * a_BiomeMap, UInt32 a_packet_id)
{
	// This function returns the fully compressed packet (including packet
	// size), not the raw packet! Below variables tagged static because of
	// https://developercommunity.visualstudio.com/content/problem/367326

	static constexpr UInt8 BitsPerEntry = 15;
	static constexpr UInt8 EntriesPerLong = 64 / BitsPerEntry;
	// static int Longs = CeilC<int, float>(ChunkBlockData::SectionBlockCount / EntriesPerLong);
	static size_t ChunkSectionDataArraySize = CeilC<size_t, float>(ChunkBlockData::SectionBlockCount / EntriesPerLong);  //  (ChunkBlockData::SectionBlockCount * BitsPerEntry) / 8 / 8;

	// const auto Bitmask = GetSectionBitmask2(a_BlockData2, a_LightData);

	// Create the packet:
	m_Packet.WriteVarInt32(a_packet_id);
	m_Packet.WriteBEInt32(a_ChunkX);
	m_Packet.WriteBEInt32(a_ChunkZ);


	{
		cFastNBTWriter Writer;
		// TODO: client works fine without?
		// std::array<Int64, 36> Longz = {};
		// Writer.AddLongArray("MOTION_BLOCKING", Longz.data(), Longz.size());
		Writer.Finish();
		m_Packet.Write(Writer.GetResult().data(), Writer.GetResult().size());
	}


	const size_t ChunkSectionSize =
		(2 +  // Block count, BEInt16, 2 bytes
		1 +  // Bits per entry, BEUInt8, 1 byte
		m_Packet.GetVarIntSize(static_cast<UInt32>(ChunkSectionDataArraySize)) +   // Field containing "size of whole section", VarInt32, variable size
			ChunkSectionDataArraySize * 8   // Actual section data, lots of bytes (multiplier 1 long = 8 bytes)
		) + 3;	 // for biomes

	const size_t ChunkSize = ChunkSectionSize * cChunkDef::NumSections;

	// Write the chunk size in bytes:
	m_Packet.WriteVarInt32(static_cast<UInt32>(ChunkSize));
	// Write each chunk section...
	for (size_t Y = 0; Y < cChunkDef::NumSections; ++Y)
	{
		const auto Blocks = a_BlockData2.GetSection(Y);
		m_Packet.WriteBEInt16(4096);
		m_Packet.WriteBEUInt8(BitsPerEntry);
		m_Packet.WriteVarInt32(static_cast<UInt32>(ChunkSectionDataArraySize));
		WriteBlockSectionSeamless2<Palette>(Blocks, BitsPerEntry, true);

		// Biomes
		m_Packet.WriteBEUInt8(0);
		m_Packet.WriteVarInt32(0);
		m_Packet.WriteVarInt32(0);
	}


	// Identify 1.9.4's tile entity list as empty
	m_Packet.WriteVarInt32(0);

	// Light Data
	m_Packet.WriteBool(false);

	m_Packet.WriteVarInt32(0);
	m_Packet.WriteVarInt32(0);
	m_Packet.WriteVarInt32(0);
	m_Packet.WriteVarInt32(0);

	m_Packet.WriteVarInt32(0);
	m_Packet.WriteVarInt32(0);
}





template <auto Palette>
inline void cChunkDataSerializer::Serialize763(const int a_ChunkX, const int a_ChunkZ, const ChunkBlockData & a_BlockData2, const ChunkLightData & a_LightData, const unsigned char * a_BiomeMap, UInt32 a_packet_id)
{
	// This function returns the fully compressed packet (including packet
	// size), not the raw packet! Below variables tagged static because of
	// https://developercommunity.visualstudio.com/content/problem/367326

	static constexpr UInt8 BitsPerEntry = 15;
	static constexpr UInt8 EntriesPerLong = 64 / BitsPerEntry;
	// static int Longs = CeilC<int, float>(ChunkBlockData::SectionBlockCount / EntriesPerLong);
	static size_t ChunkSectionDataArraySize = CeilC<size_t, float>(ChunkBlockData::SectionBlockCount / EntriesPerLong);  //  (ChunkBlockData::SectionBlockCount * BitsPerEntry) / 8 / 8;

	// const auto Bitmask = GetSectionBitmask2(a_BlockData2, a_LightData);

	// Create the packet:
	m_Packet.WriteVarInt32(a_packet_id);
	m_Packet.WriteBEInt32(a_ChunkX);
	m_Packet.WriteBEInt32(a_ChunkZ);


	{
		cFastNBTWriter Writer;
		// TODO: client works fine without?
		// std::array<Int64, 36> Longz = {};
		// Writer.AddLongArray("MOTION_BLOCKING", Longz.data(), Longz.size());
		Writer.Finish();
		m_Packet.Write(Writer.GetResult().data(), Writer.GetResult().size());
	}


	const size_t ChunkSectionSize =
		(2 +  // Block count, BEInt16, 2 bytes
		1 +  // Bits per entry, BEUInt8, 1 byte
		m_Packet.GetVarIntSize(static_cast<UInt32>(ChunkSectionDataArraySize)) +   // Field containing "size of whole section", VarInt32, variable size
			ChunkSectionDataArraySize * 8   // Actual section data, lots of bytes (multiplier 1 long = 8 bytes)
		) + 3;	 // for biomes

	const size_t ChunkSize = ChunkSectionSize * cChunkDef::NumSections;

	// Write the chunk size in bytes:
	m_Packet.WriteVarInt32(static_cast<UInt32>(ChunkSize));
	// Write each chunk section...
	for (size_t Y = 0; Y < cChunkDef::NumSections; ++Y)
	{
		const auto Blocks = a_BlockData2.GetSection(Y);
		m_Packet.WriteBEInt16(4096);
		m_Packet.WriteBEUInt8(BitsPerEntry);
		m_Packet.WriteVarInt32(static_cast<UInt32>(ChunkSectionDataArraySize));
		WriteBlockSectionSeamless2<Palette>(Blocks, BitsPerEntry, true);

		// Biomes
		m_Packet.WriteBEUInt8(0);
		m_Packet.WriteVarInt32(0);
		m_Packet.WriteVarInt32(0);
	}

	// Identify 1.9.4's tile entity list as empty
	m_Packet.WriteVarInt32(0);

	// Light Data
	m_Packet.WriteVarInt32(0);
	m_Packet.WriteVarInt32(0);
	m_Packet.WriteVarInt32(0);
	m_Packet.WriteVarInt32(0);

	m_Packet.WriteVarInt32(0);
	m_Packet.WriteVarInt32(0);
}





template <auto Palette>
inline void cChunkDataSerializer::Serialize764(const int a_ChunkX, const int a_ChunkZ, const ChunkBlockData & a_BlockData2, const ChunkLightData & a_LightData, const unsigned char * a_BiomeMap, UInt32 a_packet_id)
{
	// This function returns the fully compressed packet (including packet
	// size), not the raw packet! Below variables tagged static because of
	// https://developercommunity.visualstudio.com/content/problem/367326

	static constexpr UInt8 BitsPerEntry = 15;
	static constexpr UInt8 EntriesPerLong = 64 / BitsPerEntry;
	// static int Longs = CeilC<int, float>(ChunkBlockData::SectionBlockCount / EntriesPerLong);
	static size_t ChunkSectionDataArraySize = CeilC<size_t, float>(ChunkBlockData::SectionBlockCount / EntriesPerLong);  //  (ChunkBlockData::SectionBlockCount * BitsPerEntry) / 8 / 8;

	// const auto Bitmask = GetSectionBitmask2(a_BlockData2, a_LightData);

	// Create the packet:
	m_Packet.WriteVarInt32(a_packet_id);
	m_Packet.WriteBEInt32(a_ChunkX);
	m_Packet.WriteBEInt32(a_ChunkZ);


	{
		cFastNBTWriter Writer = cFastNBTWriter(true);
		// TODO: client works fine without?
		// std::array<Int64, 36> Longz = {};
		// Writer.AddLongArray("MOTION_BLOCKING", Longz.data(), Longz.size());
		Writer.Finish();
		m_Packet.Write(Writer.GetResult().data(), Writer.GetResult().size());
	}


	const size_t ChunkSectionSize =
		(2 +  // Block count, BEInt16, 2 bytes
		1 +  // Bits per entry, BEUInt8, 1 byte
		m_Packet.GetVarIntSize(static_cast<UInt32>(ChunkSectionDataArraySize)) +   // Field containing "size of whole section", VarInt32, variable size
			ChunkSectionDataArraySize * 8   // Actual section data, lots of bytes (multiplier 1 long = 8 bytes)
		) + 3;	 // for biomes

	const size_t ChunkSize = ChunkSectionSize * cChunkDef::NumSections;

	// Write the chunk size in bytes:
	m_Packet.WriteVarInt32(static_cast<UInt32>(ChunkSize));
	// Write each chunk section...
	for (size_t Y = 0; Y < cChunkDef::NumSections; ++Y)
	{
		const auto Blocks = a_BlockData2.GetSection(Y);
		// const auto BlockLights = a_LightData.GetBlockLightSection(Y);
		// const auto SkyLights = a_LightData.GetSkyLightSection(Y);
		m_Packet.WriteBEInt16(4096);
		m_Packet.WriteBEUInt8(BitsPerEntry);
		m_Packet.WriteVarInt32(static_cast<UInt32>(ChunkSectionDataArraySize));
		WriteBlockSectionSeamless2<Palette>(Blocks, BitsPerEntry, true);

		// Biomes
		m_Packet.WriteBEUInt8(0);
		m_Packet.WriteVarInt32(0);
		m_Packet.WriteVarInt32(0);
	}

	// Identify 1.9.4's tile entity list as empty
	m_Packet.WriteVarInt32(0);

	// Light Data
	// Each bit corresponds to one chunk section that is lit
	// Skylight
	m_Packet.WriteVarInt32(1);
	Int64 SkyLight = 0;
	Int64 BlockLight = 0;
	int light_count = 0, block_count = 0;
	for (size_t Y = 0; Y < cChunkDef::NumSections; ++Y)
	{
		SkyLight |= (a_LightData.GetSkyLightSection(Y) != nullptr) << Y;
		if (a_LightData.GetSkyLightSection(Y) != nullptr)
		{
			light_count++;
		}
		BlockLight |= (a_LightData.GetBlockLightSection(Y) != nullptr) << Y;
		if (a_LightData.GetBlockLightSection(Y) != nullptr)
		{
			block_count++;
		}
	}
	m_Packet.WriteBEInt64(SkyLight);
	// BlockLight
	m_Packet.WriteVarInt32(1);
	m_Packet.WriteBEInt64(BlockLight);

	m_Packet.WriteVarInt32(0);
	m_Packet.WriteVarInt32(0);

	m_Packet.WriteVarInt32(static_cast<UInt32>(light_count));
	for (size_t Y = 0; Y < cChunkDef::NumSections; ++Y)
	{
		auto light = a_LightData.GetSkyLightSection(Y);
		if (light != nullptr)
		{
			m_Packet.WriteVarInt32(2048);
			m_Packet.Write(light, 2048);
		}
	}
	m_Packet.WriteVarInt32(static_cast<UInt32>(block_count));
	for (size_t Y = 0; Y < cChunkDef::NumSections; ++Y)
	{
		auto light = a_LightData.GetBlockLightSection(Y);
		if (light != nullptr)
		{
			m_Packet.WriteVarInt32(2048);
			m_Packet.Write(light, 2048);
		}
	}
}





template <auto Palette>
inline void cChunkDataSerializer::WriteBlockSectionSeamless2(const ChunkBlockData::BlockArray * a_Blocks, const UInt8 a_BitsPerEntry, bool padding)
{
	// https://wiki.vg/Chunk_Format#Data_structure

	// We shift a UInt64 by a_BitsPerEntry, the latter cannot be too big:
	ASSERT(a_BitsPerEntry < 64);

	UInt64 Buffer = 0;  // A buffer to compose multiple smaller bitsizes into one 64-bit number
	unsigned char BitIndex = 0;  // The bit-position in Buffer that represents where to write next

	// const bool BlocksExist = a_Blocks != nullptr;
	// const bool MetasExist = a_Metas != nullptr;

	for (size_t Index = 0; Index != ChunkBlockData::SectionBlockCount; Index++)
	{
		const auto v2 = a_Blocks == nullptr ? 0 : Palette((*a_Blocks)[Index]);

		const auto Value = v2;  // Palette(BlockType, BlockMeta);

		// The _signed_ count of bits in Value left to write
		const auto Remaining = static_cast<char>(a_BitsPerEntry - (64 - BitIndex));

		// Write as much as possible of Value, starting from BitIndex, into Buffer:
		// Buffer |= static_cast<UInt64>(Value) << BitIndex;

		if (Remaining >= 0)
		{
			if (padding)
			{
				// There were some bits remaining: we've filled the buffer. Flush it:
				m_Packet.WriteBEUInt64(Buffer);
				Buffer = 0;
				BitIndex = 0;

				// Write as much as possible of Value, starting from BitIndex, into Buffer:
				Buffer |= static_cast<UInt64>(Value) << BitIndex;

				// It fit, excellent.
				BitIndex += a_BitsPerEntry;
			}
			else
			{
				// Write as much as possible of Value, starting from BitIndex, into Buffer:
				Buffer |= static_cast<UInt64>(Value) << BitIndex;

				// There were some bits remaining: we've filled the buffer. Flush it:
				m_Packet.WriteBEUInt64(Buffer);

				// And write the remaining bits, setting the new BitIndex:
				Buffer = static_cast<UInt64>(Value >> (a_BitsPerEntry - Remaining));
				BitIndex = static_cast<unsigned char>(Remaining);
			}
		}
		else
		{
			// Write as much as possible of Value, starting from BitIndex, into Buffer:
			Buffer |= static_cast<UInt64>(Value) << BitIndex;

			// It fit, excellent.
			BitIndex += a_BitsPerEntry;
		}
		if (((Index + 1) == ChunkBlockData::SectionBlockCount) && padding)
		{
			m_Packet.WriteBEUInt64(Buffer);
			BitIndex = 0;
			Buffer = 0;
		}
	}

	static_assert((ChunkBlockData::SectionBlockCount % 64) == 0, "Section must fit wholly into a 64-bit long array");
	ASSERT(BitIndex == 0);
	ASSERT(Buffer == 0);
}





template <auto Palette>
inline void cChunkDataSerializer::WriteBlockSectionSeamless(const ChunkBlockData::BlockArray * a_Blocks, const UInt8 a_BitsPerEntry)
{
	// https://wiki.vg/Chunk_Format#Data_structure

	// We shift a UInt64 by a_BitsPerEntry, the latter cannot be too big:
	ASSERT(a_BitsPerEntry < 64);

	UInt64 Buffer = 0;  // A buffer to compose multiple smaller bitsizes into one 64-bit number
	unsigned char BitIndex = 0;  // The bit-position in Buffer that represents where to write next

	const bool BlocksExist = a_Blocks != nullptr;

	for (size_t Index = 0; Index != ChunkBlockData::SectionBlockCount; Index++)
	{
		auto Block = BlocksExist ? (*a_Blocks)[Index] : 0;

		const auto Value = Palette(Block);

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
