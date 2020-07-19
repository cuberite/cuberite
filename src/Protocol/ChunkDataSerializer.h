#pragma once

#include "../ChunkData.h"
#include "../Defines.h"





class cByteBuffer;





/** Serializes one chunk's data to (possibly multiple) protocol versions.
Caches the serialized data for as long as this object lives, so that the same data can be sent to
other clients using the same protocol. */
class cChunkDataSerializer
{
public:

	cChunkDataSerializer(
		int                                         a_ChunkX,
		int                                         a_ChunkZ,
		const cChunkData &                          a_Data,
		const unsigned char *                       a_BiomeData,
		const eDimension                            a_Dimension
	);

	/** For each client, serializes the chunk into their protocol version and sends it. */
	void SendToClients(const std::unordered_set<cClientHandle *> & a_SendTo);

protected:

	void Serialize47 (const std::vector<cClientHandle *> & a_SendTo);  // Release 1.8
	void Serialize107(const std::vector<cClientHandle *> & a_SendTo);  // Release 1.9
	void Serialize110(const std::vector<cClientHandle *> & a_SendTo);  // Release 1.9.4

	template <auto Palette>
	void Serialize393And401(const std::vector<cClientHandle *> & a_SendTo);  // Release 1.13 - 1.13.1

	/** Finalises the data, compresses it if required, and delivers it to all clients. */
	void CompressAndSend(cByteBuffer & a_Packet, const std::vector<cClientHandle *> & a_SendTo);

	/** The coordinates of the chunk to serialise. */
	int m_ChunkX, m_ChunkZ;

	/** The data read from the chunk, to be serialized. */
	const cChunkData & m_Data;

	/** The biomes in the chunk, to be serialized. */
	const unsigned char * m_BiomeData;

	/** The dimension where the chunk resides. */
	const eDimension m_Dimension;
} ;




