#pragma once

#include "ChunkDef.h"

class cLuaChunk			//tolua_export
{						//tolua_export
public:
	cLuaChunk( cChunkDef::BlockTypes   & a_BlockTypes
	         , cChunkDef::BlockNibbles & a_BlockNibbles 
			 , cChunkDef::HeightMap    & a_HeightMap
			 , cChunkDef::BiomeMap     & a_BiomeMap
			 )
		: m_BiomeMap( a_BiomeMap )
		, m_BlockTypes( a_BlockTypes )
		, m_BlockMeta( a_BlockNibbles )
		, m_HeightMap( a_HeightMap )
	{}
	~cLuaChunk()
	{}

	void SetBlock( const Vector3i & a_Pos, char a_BlockID, unsigned char a_BlockMeta )	//tolua_export
	{	//tolua_export
	}	//tolua_export

	void FillBlocks( char a_BlockID, unsigned char a_BlockMeta )	//tolua_export
	{	//tolua_export
		const NIBBLETYPE CompressedMeta = a_BlockMeta | a_BlockMeta << 4;
		memset( m_BlockTypes, a_BlockID, sizeof( cChunkDef::BlockTypes ) );
		memset( m_BlockMeta, CompressedMeta, sizeof( cChunkDef::BlockNibbles ) );

		memset( m_BiomeMap, 0, sizeof( cChunkDef::BiomeMap ) );
		memset( m_HeightMap, 0, sizeof( cChunkDef::HeightMap ) );
	}	//tolua_export
private:
	EMCSBiome  * m_BiomeMap;
	BLOCKTYPE  * m_BlockTypes;
	NIBBLETYPE * m_BlockMeta;
	HEIGHTTYPE * m_HeightMap;
};	//tolua_export