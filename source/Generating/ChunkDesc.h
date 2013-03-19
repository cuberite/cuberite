
// ChunkDesc.h

// Declares the cChunkDesc class representing the chunk description used while generating a chunk. This class is also exported to Lua for HOOK_CHUNK_GENERATING.





#pragma once

#include "../ChunkDef.h"
#include "../BlockArea.h"





// fwd: ../BlockArea.h
class cBlockArea;





// tolua_begin
class cChunkDesc
{
public:
	// tolua_end
	
	cChunkDesc(int a_ChunkX, int a_ChunkZ);
	~cChunkDesc();

	// tolua_begin

	int GetChunkX(void) const { return m_ChunkX; }
	int GetChunkZ(void) const { return m_ChunkZ; }
	
	void SetChunkCoords(int a_ChunkX, int a_ChunkZ);
	
	void       FillBlocks(BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta);
	void       SetBlockTypeMeta(int a_RelX, int a_RelY, int a_RelZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta);
	void       GetBlockTypeMeta(int a_RelX, int a_RelY, int a_RelZ, BLOCKTYPE & a_BlockType, NIBBLETYPE & a_BlockMeta);

	void       SetBlockType(int a_RelX, int a_RelY, int a_RelZ, BLOCKTYPE a_BlockType);
	BLOCKTYPE  GetBlockType(int a_RelX, int a_RelY, int a_RelZ);
	
	void       SetBlockMeta(int a_RelX, int a_RelY, int a_RelZ, NIBBLETYPE a_BlockMeta);
	NIBBLETYPE GetBlockMeta(int a_RelX, int a_RelY, int a_RelZ);

	void       SetBiome(int a_RelX, int a_RelZ, int a_BiomeID);
	EMCSBiome  GetBiome(int a_RelX, int a_RelZ);

	void       SetHeight(int a_RelX, int a_RelZ, int a_Height);
	int        GetHeight(int a_RelX, int a_RelZ);

	// Default generation:
	void SetUseDefaultBiomes(bool a_bUseDefaultBiomes);
	bool IsUsingDefaultBiomes(void) const;
	void SetUseDefaultHeight(bool a_bUseDefaultHeight);
	bool IsUsingDefaultHeight(void) const;
	void SetUseDefaultComposition(bool a_bUseDefaultComposition);
	bool IsUsingDefaultComposition(void) const;
	void SetUseDefaultStructures(bool a_bUseDefaultStructures);
	bool IsUsingDefaultStructures(void) const;
	void SetUseDefaultFinish(bool a_bUseDefaultFinish);
	bool IsUsingDefaultFinish(void) const;

	/// Writes the block area into the chunk, with its origin set at the specified relative coords. Area's data overwrite everything in the chunk.
	void WriteBlockArea(const cBlockArea & a_BlockArea, int a_RelX, int a_RelY, int a_RelZ, cBlockArea::eMergeStrategy a_MergeStrategy = cBlockArea::msOverwrite);

	/// Reads an area from the chunk into a cBlockArea
	void ReadBlockArea(cBlockArea & a_Dest, int a_MinRelX, int a_MaxRelX, int a_MinRelY, int a_MaxRelY, int a_MinRelZ, int a_MaxRelZ);

	/// Returns the maximum height value in the heightmap
	HEIGHTTYPE GetMaxHeight(void) const;
	
	// tolua_end
	
	
	// Accessors used by cChunkGenerator::Generator descendants:
	inline cChunkDef::BiomeMap &     GetBiomeMap     (void) { return m_BiomeMap; }
	inline cChunkDef::BlockTypes &   GetBlockTypes   (void) { return *((cChunkDef::BlockTypes *)m_BlockArea.GetBlockTypes()); }
	// CANNOT, different compression! 
	// inline cChunkDef::BlockNibbles & GetBlockMetas   (void) { return *((cChunkDef::BlockNibbles *)m_BlockArea.GetBlockMetas()); }
	inline cChunkDef::HeightMap &    GetHeightMap    (void) { return m_HeightMap; }
	inline cEntityList &             GetEntities     (void) { return m_Entities; }
	inline cBlockEntityList &        GetBlockEntities(void) { return m_BlockEntities; }
	
	/// Compresses the metas from the BlockArea format (1 meta per byte) into regular format (2 metas per byte)
	void CompressBlockMetas(cChunkDef::BlockNibbles & a_DestMetas);
	
private:
	int m_ChunkX;
	int m_ChunkZ;
	
	cChunkDef::BiomeMap     m_BiomeMap;
	cBlockArea              m_BlockArea;
	cChunkDef::HeightMap    m_HeightMap;
	cEntityList             m_Entities;       // Individual entities are NOT owned by this object!
	cBlockEntityList        m_BlockEntities;  // Individual block entities are NOT owned by this object!

	bool m_bUseDefaultBiomes;
	bool m_bUseDefaultHeight;
	bool m_bUseDefaultComposition;
	bool m_bUseDefaultStructures;
	bool m_bUseDefaultFinish;
} ;  // tolua_export




