
#pragma once


#include <cstring>


#include "ChunkDef.h"




#if __cplusplus < 201103L
// auto_ptr style interface for memory management
#else
// unique_ptr style interface for memory management
#endif

class cChunkData
{
public:

	cChunkData();
	~cChunkData();
	
	#if __cplusplus < 201103L
		// auto_ptr style interface for memory management
		cChunkData(const cChunkData & a_Other);
		cChunkData & operator =(const cChunkData & a_Other);
	#else
		// unique_ptr style interface for memory management
		cChunkData(cChunkData && a_Other);
		cChunkData & operator =(cChunkData && a_ther);
	#endif

	BLOCKTYPE GetBlock(int a_X, int a_Y, int a_Z) const;
	void SetBlock(int a_RelX, int a_RelY, int a_RelZ, BLOCKTYPE a_Block);

	NIBBLETYPE GetMeta(int a_RelX, int a_RelY, int a_RelZ) const;
	bool SetMeta(int a_RelX, int a_RelY, int a_RelZ, NIBBLETYPE a_Nibble);
	
	NIBBLETYPE GetBlockLight(int a_RelX, int a_RelY, int a_RelZ) const;
	
	NIBBLETYPE GetSkyLight(int a_RelX, int a_RelY, int a_RelZ) const;
	
	cChunkData Copy(void) const;
	void CopyBlocks    (BLOCKTYPE * a_Dest, size_t a_Idx = 0, size_t a_Length = cChunkDef::NumBlocks) const;
	void CopyMeta      (NIBBLETYPE * a_Dest) const;
	void CopyBlockLight(NIBBLETYPE * a_Dest) const;
	void CopySkyLight  (NIBBLETYPE * a_Dest) const;
	
	void SetBlocks    (const BLOCKTYPE * a_src);
	void SetMeta      (const NIBBLETYPE * a_src);
	void SetBlockLight(const NIBBLETYPE * a_src);
	void SetSkyLight  (const NIBBLETYPE * a_src);
	
private:

	static const size_t CHUNK_SECTION_HEIGHT = 16;
	static const size_t CHUNK_SECTION_COUNT = (256 / CHUNK_SECTION_HEIGHT);

	#if __cplusplus < 201103L
	// auto_ptr style interface for memory management
	mutable bool m_IsOwner;
	#endif
	
	struct sChunkSection {
		BLOCKTYPE  m_BlockTypes   [CHUNK_SECTION_HEIGHT * 16 * 16];
		NIBBLETYPE m_BlockMeta    [CHUNK_SECTION_HEIGHT * 16 * 16 / 2];
		NIBBLETYPE m_BlockLight   [CHUNK_SECTION_HEIGHT * 16 * 16 / 2];
		NIBBLETYPE m_BlockSkyLight[CHUNK_SECTION_HEIGHT * 16 * 16 / 2];
	};
	
	sChunkSection * m_Sections[CHUNK_SECTION_COUNT];
	
	/** Allocates a new section. Entry-point to custom allocators. */
	static sChunkSection * Allocate(void);
	
	/** Frees the specified section, previously allocated using Allocate().
	Note that a_Section may be NULL. */
	static void Free(sChunkSection * a_Section);
	
	/** Sets the data in the specified section to their default values. */
	void ZeroSection(sChunkSection * a_Section) const;
};






