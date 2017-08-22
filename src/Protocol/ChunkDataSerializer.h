
// ChunkDataSerializer.h

// Interfaces to the cChunkDataSerializer class representing the object that can:
//  - serialize chunk data to different protocol versions
//  - cache such serialized data for multiple clients

#include "ChunkData.h"




class cChunkDataSerializer
{
protected:
	const cChunkData & m_Data;
	const unsigned char * m_BiomeData;
	const eDimension m_Dimension;

	typedef std::map<int, AString> Serializations;

	Serializations m_Serializations;

	void Serialize47(AString & a_Data, int a_ChunkX, int a_ChunkZ);  // Release 1.8
	void Serialize107(AString & a_Data, int a_ChunkX, int a_ChunkZ);  // Release 1.9
	void Serialize110(AString & a_Data, int a_ChunkX, int a_ChunkZ);  // Release 1.9.4

public:
	enum
	{
		RELEASE_1_8_0 = 47,
		RELEASE_1_9_0 = 107,
		RELEASE_1_9_4 = 110,
	} ;

	cChunkDataSerializer(
		const cChunkData &    a_Data,
		const unsigned char * a_BiomeData,
		const eDimension      a_Dimension
	);

	const AString & Serialize(int a_Version, int a_ChunkX, int a_ChunkZ);  // Returns one of the internal m_Serializations[]
} ;




