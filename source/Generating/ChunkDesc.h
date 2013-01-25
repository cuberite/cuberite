
// ChunkDesc.h

// Declares the cChunkDesc class representing the chunk description used while generating a chunk. This class is also exported to Lua for HOOK_CHUNK_GENERATING.





#pragma once

#include "../ChunkDef.h"





class cChunkDesc  // tolua_export
{  // tolua_export
public:
	cChunkDesc(
		cChunkDef::BlockTypes &   a_BlockTypes,
		cChunkDef::BlockNibbles & a_BlockNibbles ,
		cChunkDef::HeightMap &    a_HeightMap,
		cChunkDef::BiomeMap &     a_BiomeMap
	)
		: m_BiomeMap(a_BiomeMap)
		, m_BlockTypes(a_BlockTypes)
		, m_BlockMeta(a_BlockNibbles)
		, m_HeightMap(a_HeightMap)
		, m_bUseDefaultBiomes(true)
		, m_bUseDefaultHeight(true)
		, m_bUseDefaultComposition(true)
		, m_bUseDefaultStructures(true)
		, m_bUseDefaultFinish(true)
	{
		memset(m_BlockTypes, 0, sizeof(cChunkDef::BlockTypes));
		memset(m_BlockMeta,  0, sizeof(cChunkDef::BlockNibbles));
		memset(m_BiomeMap,   0, sizeof(cChunkDef::BiomeMap));
		memset(m_HeightMap,  0, sizeof(cChunkDef::HeightMap));
	}
	
	~cChunkDesc()
	{}

	// tolua_begin



	// Block functions:
	void FillBlocks(BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta)
	{
		const NIBBLETYPE CompressedMeta = a_BlockMeta | a_BlockMeta << 4;
		memset(m_BlockTypes, a_BlockType,    sizeof(cChunkDef::BlockTypes));
		memset(m_BlockMeta,  CompressedMeta, sizeof(cChunkDef::BlockNibbles));
	}


	void SetBlock(int a_X, int a_Y, int a_Z, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta)
	{
		cChunkDef::SetBlock( m_BlockTypes, a_X, a_Y, a_Z, a_BlockType );
		cChunkDef::SetNibble( m_BlockMeta, a_X, a_Y, a_Z, a_BlockMeta );
	}


	BLOCKTYPE GetBlock( int a_X, int a_Y, int a_Z )
	{
		return cChunkDef::GetBlock( m_BlockTypes, a_X, a_Y, a_Z );
	}


	NIBBLETYPE GetBlockMeta( int a_X, int a_Y, int a_Z )
	{
		return cChunkDef::GetNibble( m_BlockMeta, a_X, a_Y, a_Z );
	}



	// Biome functinos
	void SetBiome( int a_X, int a_Z, int a_BiomeID )
	{
		cChunkDef::SetBiome( m_BiomeMap, a_X, a_Z, (EMCSBiome)a_BiomeID );
	}


	int GetBiome( int a_X, int a_Z )
	{
		return cChunkDef::GetBiome( m_BiomeMap, a_X, a_Z );
	}



	// Height functions
	void SetHeight( int a_X, int a_Z, int a_Height )
	{
		cChunkDef::SetHeight( m_HeightMap, a_X, a_Z, a_Height );
	}


	int GetHeight( int a_X, int a_Z )
	{
		return cChunkDef::GetHeight( m_HeightMap, a_X, a_Z );
	}



	// Functions to explicitly tell the server to use default behavior for certain parts of generating terrain
	void SetUseDefaultBiomes(bool a_bUseDefaultBiomes)
	{
		m_bUseDefaultBiomes = a_bUseDefaultBiomes;
	}
	
	
	bool IsUsingDefaultBiomes(void) const
	{
		return m_bUseDefaultBiomes;
	}


	void SetUseDefaultHeight(bool a_bUseDefaultHeight)
	{
		m_bUseDefaultHeight = a_bUseDefaultHeight;
	}
	
	
	bool IsUsingDefaultHeight(void) const
	{
		return m_bUseDefaultHeight;
	}
	
	
	void SetUseDefaultComposition(bool a_bUseDefaultComposition)
	{
		m_bUseDefaultComposition = a_bUseDefaultComposition;
	}
	
	
	bool IsUsingDefaultComposition(void) const
	{
		return m_bUseDefaultComposition;
	}


	void SetUseDefaultStructures(bool a_bUseDefaultStructures)
	{
		m_bUseDefaultStructures = a_bUseDefaultStructures;
	}
	
	
	bool IsUsingDefaultStructures(void) const
	{
		return m_bUseDefaultStructures;
	}


	void SetUseDefaultFinish(bool a_bUseDefaultFinish)
	{
		m_bUseDefaultFinish = a_bUseDefaultFinish;
	}
	
	
	bool IsUsingDefaultFinish(void) const
	{
		return m_bUseDefaultFinish;
	}

	// tolua_end
	
	
	// Accessors used by cChunkGenerator::Generator descendants:
	cChunkDef::BiomeMap &     GetBiomeMap  (void) { return m_BiomeMap; }
	cChunkDef::BlockTypes &   GetBlockTypes(void) { return m_BlockTypes; }
	cChunkDef::BlockNibbles & GetBlockMetas(void) { return m_BlockMeta; }
	cChunkDef::HeightMap &    GetHeightMap (void) { return m_HeightMap; }

private:
	bool m_bUseDefaultBiomes;
	bool m_bUseDefaultHeight;
	bool m_bUseDefaultComposition;
	bool m_bUseDefaultStructures;
	bool m_bUseDefaultFinish;

	cChunkDef::BiomeMap     & m_BiomeMap;
	cChunkDef::BlockTypes   & m_BlockTypes;
	cChunkDef::BlockNibbles & m_BlockMeta;
	cChunkDef::HeightMap    & m_HeightMap;
} ;  // tolua_export




