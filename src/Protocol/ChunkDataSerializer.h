#pragma once

#include "../ByteBuffer.h"
#include "../ChunkData.h"
#include "../Defines.h"
#include "CircularBufferCompressor.h"
#include "StringCompression.h"





class cByteBuffer;





/** Serializes one chunk's data to (possibly multiple) protocol versions.
Caches the serialized data for as long as this object lives, so that the same data can be sent to
other clients using the same protocol. */
class cChunkDataSerializer
{
	using ClientHandles = std::vector<std::shared_ptr<cClientHandle>>;

	/** Enum to collapse protocol versions into a contiguous index. */
	enum class CacheVersion
	{
		v47,
		v107,
		v110,
		v393,
		v401,
		v477,
		v573,
		v735,
		v751,
		v755,
		v757,
		v759,
		v760,
		v761,
		v762,
		v763,
		v764,
		v765,
		v766,
		v767,
		v768,
		v769,
		Last = CacheVersion::v769
	};

	/** A single cache entry containing the raw data, compressed data, and a validity flag. */
	struct ChunkDataCache
	{
		ContiguousByteBuffer ToSend;
		bool Engaged = false;
	};

public:

	cChunkDataSerializer(eDimension a_Dimension);

	/** For each client, serializes the chunk into their protocol version and sends it.
	Parameters are the coordinates of the chunk to serialise, and the data and biome data read from the chunk. */
	void SendToClients(int a_ChunkX, int a_ChunkZ, const ChunkBlockData & a_BlockData, const ChunkLightData & a_LightData, const unsigned char * a_BiomeMap, const std::vector<cBlockEntity *> & a_BlockEntities, const cChunkDef::HeightMap & a_SurfaceHeightMap, const ClientHandles & a_SendTo);

private:

	/** Serialises the given chunk, storing the result into the given cache entry, and sends the data.
	If the cache entry is already present, simply re-uses it. */
	inline void Serialize(const ClientHandles::value_type & a_Client, int a_ChunkX, int a_ChunkZ, const ChunkBlockData & a_BlockData, const ChunkLightData & a_LightData, const unsigned char * a_BiomeMap, const std::vector<cBlockEntity *> & a_BlockEntities, const cChunkDef::HeightMap & a_SurfaceHeightMap, CacheVersion a_CacheVersion);

	inline void Serialize47 (int a_ChunkX, int a_ChunkZ, const ChunkBlockData & a_BlockData, const ChunkLightData & a_LightData, const unsigned char * a_BiomeMap);  // Release 1.8
	inline void Serialize107(int a_ChunkX, int a_ChunkZ, const ChunkBlockData & a_BlockData, const ChunkLightData & a_LightData, const unsigned char * a_BiomeMap);  // Release 1.9
	inline void Serialize110(int a_ChunkX, int a_ChunkZ, const ChunkBlockData & a_BlockData, const ChunkLightData & a_LightData, const unsigned char * a_BiomeMap);  // Release 1.9.4
	template <auto Palette>
	inline void Serialize393(int a_ChunkX, int a_ChunkZ, const ChunkBlockData & a_BlockData, const ChunkLightData & a_LightData, const unsigned char * a_BiomeMap);  // Release 1.13 - 1.13.2
	inline void Serialize477(int a_ChunkX, int a_ChunkZ, const ChunkBlockData & a_BlockData, const ChunkLightData & a_LightData, const unsigned char * a_BiomeMap);	 // Release 1.14 - 1.14.4

	inline void Serialize573(int a_ChunkX, int a_ChunkZ, const ChunkBlockData & a_BlockData, const ChunkBlockData & a_BlockData2, const ChunkLightData & a_LightData, const unsigned char * a_BiomeMap);

	inline void Serialize735(const int a_ChunkX, const int a_ChunkZ, const ChunkBlockData & a_BlockData, const ChunkLightData & a_LightData, const unsigned char * a_BiomeMap);
	inline void Serialize751(const int a_ChunkX, const int a_ChunkZ, const ChunkBlockData & a_BlockData, const ChunkLightData & a_LightData, const unsigned char * a_BiomeMap);
	inline void Serialize755(const int a_ChunkX, const int a_ChunkZ, const ChunkBlockData & a_BlockData, const ChunkLightData & a_LightData, const unsigned char * a_BiomeMap);
	template <auto Palette>
	inline void Serialize757(const int a_ChunkX, const int a_ChunkZ, const ChunkBlockData & a_BlockData, const ChunkLightData & a_LightData, const unsigned char * a_BiomeMap, UInt32 a_packet_id);
	template <auto Palette>
	inline void Serialize763(const int a_ChunkX, const int a_ChunkZ, const ChunkBlockData & a_BlockData, const ChunkLightData & a_LightData, const unsigned char * a_BiomeMap, UInt32 a_packet_id);
	template <auto Palette>
	inline void Serialize764(const int a_ChunkX, const int a_ChunkZ, const ChunkBlockData & a_BlockData, const ChunkLightData & a_LightData, const unsigned char * a_BiomeMap, const std::vector<cBlockEntity *> & a_BlockEntities, const ClientHandles::value_type & a_Client, const cChunkDef::HeightMap & a_SurfaceHeightMap, UInt32 a_packet_id);

	template <auto Palettee>
	inline void WriteBlockSectionSeamless2(const ChunkBlockData::BlockArray * a_Blocks, const UInt8 a_BitsPerEntry, bool padding);
	/** Writes all blocks in a chunk section into a series of Int64.
	Writes start from the bit directly subsequent to the previous write's end, possibly crossing over to the next Int64. */
	template <auto Palette>
	inline void WriteBlockSectionSeamless(const ChunkBlockData::BlockArray * a_Blocks, UInt8 a_BitsPerEntry);

	inline void WriteHeightMap(UInt64 * a_Array, const cChunkDef::HeightMap & a_HeightMap, const UInt8 a_BitsPerEntry, bool padding);

	/** Copies all lights in a chunk section into the packet, block light followed immediately by sky light. */
	inline void WriteLightSectionGrouped(const ChunkLightData::LightArray * a_BlockLights, const ChunkLightData::LightArray * a_SkyLights);

	/** Finalises the data, compresses it if required, and stores it into cache. */
	inline void CompressPacketInto(ChunkDataCache & a_Cache);

	/** A staging area used to construct the chunk packet, persistent to avoid reallocating. */
	cByteBuffer m_Packet;

	/** A compressor used to compress the chunk data. */
	CircularBufferCompressor m_Compressor;

	/** The dimension for the World this Serializer is tied to. */
	const eDimension m_Dimension;

	/** A cache, mapping protocol version to a fully serialised chunk.
	It is used during a single invocation of SendToClients with more than one client. */
	std::array<ChunkDataCache, static_cast<size_t>(CacheVersion::Last) + 1> m_Cache;
} ;
