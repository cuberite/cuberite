
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

	typedef std::map<int, AString> Serializations;

	Serializations m_Serializations;

	void Serialize39(AString & a_Data);  // Release 1.3.1 to 1.7.10
	void Serialize47(AString & a_Data, int a_ChunkX, int a_ChunkZ);  // Release 1.8
	void Serialize107(AString & a_Data, int a_ChunkX, int a_ChunkZ);  // Release 1.9

public:
	enum
	{
		RELEASE_1_3_2 = 39,
		RELEASE_1_8_0 = 47,
		RELEASE_1_9_0 = 107,
	} ;

	cChunkDataSerializer(
		const cChunkDef::BlockTypes   & a_BlockTypes,
		const cChunkDef::BlockNibbles & a_BlockMetas,
		const cChunkDef::BlockNibbles & a_BlockLight,
		const cChunkDef::BlockNibbles & a_BlockSkyLight,
		const unsigned char *           a_BiomeData
	);

	const AString & Serialize(int a_Version, int a_ChunkX, int a_ChunkZ);  // Returns one of the internal m_Serializations[]
} ;




