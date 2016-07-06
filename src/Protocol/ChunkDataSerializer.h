
// ChunkDataSerializer.h

// Interfaces to the cChunkDataSerializer class representing the object that can:
//  - serialize chunk data to different protocol versions
//  - cache such serialized data for multiple clients





class cChunkDataSerializer
{
protected:
	const cChunkDef::BlockTypes   & m_BlockTypes;
	const cChunkDef::BlockNibbles & m_BlockMetas;
	const cChunkDef::BlockNibbles & m_BlockLight;
	const cChunkDef::BlockNibbles & m_BlockSkyLight;
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
		const cChunkDef::BlockTypes   & a_BlockTypes,
		const cChunkDef::BlockNibbles & a_BlockMetas,
		const cChunkDef::BlockNibbles & a_BlockLight,
		const cChunkDef::BlockNibbles & a_BlockSkyLight,
		const unsigned char *           a_BiomeData,
		const eDimension                a_Dimension
	);

	const AString & Serialize(int a_Version, int a_ChunkX, int a_ChunkZ);  // Returns one of the internal m_Serializations[]
} ;




