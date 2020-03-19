#pragma once

#include "../ChunkData.h"





/** Serializes one chunk's data to (possibly multiple) protocol versions.
Caches the serialized data for as long as this object lives, so that the same data can be sent to
other clients using the same protocol. */
class cChunkDataSerializer
{
public:
	enum
	{
		RELEASE_1_8_0 =  47,
		RELEASE_1_9_0 = 107,
		RELEASE_1_9_4 = 110,
		RELEASE_1_13  = 393,
	} ;

	cChunkDataSerializer(
		const cChunkData &    a_Data,
		const unsigned char * a_BiomeData,
		const eDimension      a_Dimension
	);

	/** Serializes the contained chunk data into the specified protocol version.
	TEMPORARY: a_BlockTypeMap is used for the 1.13+ protocols to map from BLOCKTYPE#META to NetBlockID.
	a_BlockTypeMap is ignored for pre-1.13 protocols. */
	const AString & Serialize(int a_Version, int a_ChunkX, int a_ChunkZ, const std::map<UInt32, UInt32> & a_BlockTypeMap);


protected:

	using Serializations = std::map<int, AString>;


	/** The data read from the chunk, to be serialized. */
	const cChunkData & m_Data;

	/** The biomes in the chunk, to be serialized. */
	const unsigned char * m_BiomeData;

	/** The dimension where the chunk resides. */
	const eDimension m_Dimension;

	/** The per-protocol serialized data, cached for reuse for other clients. */
	Serializations m_Serializations;


	void Serialize47 (AString & a_Data, int a_ChunkX, int a_ChunkZ);  // Release 1.8
	void Serialize107(AString & a_Data, int a_ChunkX, int a_ChunkZ);  // Release 1.9
	void Serialize110(AString & a_Data, int a_ChunkX, int a_ChunkZ);  // Release 1.9.4
	void Serialize393(AString & a_Data, int a_ChunkX, int a_ChunkZ, const std::map<UInt32, UInt32> & a_BlockTypeMap);  // Release 1.13
} ;




