#pragma once

#include "../ByteBuffer.h"
#include "../ChunkData.h"
#include "../Defines.h"





class cByteBuffer;





/** Serializes one chunk's data to (possibly multiple) protocol versions.
Caches the serialized data for as long as this object lives, so that the same data can be sent to
other clients using the same protocol. */
class cChunkDataSerializer
{
	using ClientHandles = std::unordered_set<cClientHandle *>;

	/** Enum to collapse protocol versions into a contiguous index. */
	enum class CacheVersion
	{
		v47,
		v107,
		v110,
		v393,
		v401,
		v477,

		Last = CacheVersion::v477
	};

	/** A single cache entry containing the raw data, compressed data, and a validity flag. */
	struct ChunkDataCache
	{
		std::string PacketData;
		std::string ToSend;
		bool Engaged = false;
	};

public:

	cChunkDataSerializer(eDimension a_Dimension);

	/** For each client, serializes the chunk into their protocol version and sends it.
	Parameters are the coordinates of the chunk to serialise, and the data and biome data read from the chunk. */
	void SendToClients(int a_ChunkX, int a_ChunkZ, const cChunkData & a_Data, const unsigned char * a_BiomeData, const ClientHandles & a_SendTo);

protected:

	/** Serialises the given chunk, storing the result into the given cache entry, and sends the data.
	If the cache entry is already present, simply re-uses it. */
	inline void Serialize(cClientHandle * a_Client, int a_ChunkX, int a_ChunkZ, const cChunkData & a_Data, const unsigned char * a_BiomeData, CacheVersion a_CacheVersion);

	inline void Serialize47 (int a_ChunkX, int a_ChunkZ, const cChunkData & a_Data, const unsigned char * a_BiomeData);  // Release 1.8
	inline void Serialize107(int a_ChunkX, int a_ChunkZ, const cChunkData & a_Data, const unsigned char * a_BiomeData);  // Release 1.9
	inline void Serialize110(int a_ChunkX, int a_ChunkZ, const cChunkData & a_Data, const unsigned char * a_BiomeData);  // Release 1.9.4
	template <auto Palette>
	inline void Serialize393(int a_ChunkX, int a_ChunkZ, const cChunkData & a_Data, const unsigned char * a_BiomeData);  // Release 1.13 - 1.13.2
	inline void Serialize477(int a_ChunkX, int a_ChunkZ, const cChunkData & a_Data, const unsigned char * a_BiomeData);  // Release 1.14 - 1.14.4

	/** Writes all blocks in a chunk section into a series of Int64.
	Writes start from the bit directly subsequent to the previous write's end, possibly crossing over to the next Int64. */
	template <auto Palette>
	inline void WriteSectionDataSeamless(const cChunkData::sChunkSection & a_Section, const UInt8 a_BitsPerEntry);

	/** Finalises the data, compresses it if required, and stores it into cache. */
	inline void CompressPacketInto(ChunkDataCache & a_Cache);

	/** A staging area used to construct the chunk packet, persistent to avoid reallocating. */
	cByteBuffer m_Packet;

	/** The dimension for the World this Serializer is tied to. */
	const eDimension m_Dimension;

	/** A cache, mapping protocol version to a fully serialised chunk.
	It is used during a single invocation of SendToClients with more than one client. */
	std::array<ChunkDataCache, static_cast<size_t>(CacheVersion::Last) + 1> m_Cache;
} ;




