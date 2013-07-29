
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
	
	void Serialize29(AString & a_Data);  // Release 1.2.4 and 1.2.5
	void Serialize39(AString & a_Data);  // Release 1.3.1 and 1.3.2
	
public:
	enum
	{
		RELEASE_1_2_5 = 29,
		RELEASE_1_3_2 = 39,
	} ;
	
	cChunkDataSerializer(
		const cChunkDef::BlockTypes   & a_BlockTypes,
		const cChunkDef::BlockNibbles & a_BlockMetas,
		const cChunkDef::BlockNibbles & a_BlockLight,
		const cChunkDef::BlockNibbles & a_BlockSkyLight,
		const unsigned char *           a_BiomeData
	);

	const AString & Serialize(int a_Version);  // Returns one of the internal m_Serializations[]
} ;




