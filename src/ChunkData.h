
// ChunkData.h

// Declares the cChunkData class that represents the block's type, meta, blocklight and skylight storage for a chunk





#pragma once


#include <cstring>


#include "ChunkDef.h"

#include "AllocationPool.h"



#if __cplusplus < 201103L
// auto_ptr style interface for memory management
#else
// unique_ptr style interface for memory management
#endif

class cChunkData
{
private:

	static const int SectionHeight = 16;
	static const size_t NumSections = (cChunkDef::Height / SectionHeight);
	static const size_t SectionBlockCount = SectionHeight * cChunkDef::Width * cChunkDef::Width;

public:

	struct sChunkSection;

	cChunkData(cAllocationPool<cChunkData::sChunkSection> & a_Pool);
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
	
	/** Creates a (deep) copy of self. */
	cChunkData Copy(void) const;

	/** Copies the blocktype data into the specified flat array.
	Optionally, only a part of the data is copied, as specified by the a_Idx and a_Length parameters. */
	void CopyBlockTypes(BLOCKTYPE * a_Dest, size_t a_Idx = 0, size_t a_Length = cChunkDef::NumBlocks) const;

	/** Copies the metadata into the specified flat array. */
	void CopyMetas(NIBBLETYPE * a_Dest) const;

	/** Copies the block light data into the specified flat array. */
	void CopyBlockLight(NIBBLETYPE * a_Dest) const;

	/** Copies the skylight data into the specified flat array. */
	void CopySkyLight  (NIBBLETYPE * a_Dest) const;
	
	/** Copies the blocktype data from the specified flat array into the internal representation.
	Allocates sections that are needed for the operation.
	Requires that a_Src is a valid pointer. */
	void SetBlockTypes(const BLOCKTYPE * a_Src);

	/** Copies the metadata from the specified flat array into the internal representation.
	Allocates sectios that are needed for the operation.
	Requires that a_Src is a valid pointer. */
	void SetMetas(const NIBBLETYPE * a_Src);

	/** Copies the blocklight data from the specified flat array into the internal representation.
	Allocates sectios that are needed for the operation.
	Allows a_Src to be nullptr, in which case it doesn't do anything. */
	void SetBlockLight(const NIBBLETYPE * a_Src);

	/** Copies the skylight data from the specified flat array into the internal representation.
	Allocates sectios that are needed for the operation.
	Allows a_Src to be nullptr, in which case it doesn't do anything. */
	void SetSkyLight(const NIBBLETYPE * a_Src);

	struct sChunkSection
	{
		BLOCKTYPE  m_BlockTypes   [SectionHeight * 16 * 16]    ;
		NIBBLETYPE m_BlockMetas   [SectionHeight * 16 * 16 / 2];
		NIBBLETYPE m_BlockLight   [SectionHeight * 16 * 16 / 2];
		NIBBLETYPE m_BlockSkyLight[SectionHeight * 16 * 16 / 2];
	};
	
private:
	#if __cplusplus < 201103L
	// auto_ptr style interface for memory management
	mutable bool m_IsOwner;
	#endif

	sChunkSection * m_Sections[NumSections];

	cAllocationPool<cChunkData::sChunkSection> & m_Pool;
	
	/** Allocates a new section. Entry-point to custom allocators. */
	sChunkSection * Allocate(void);

	/** Frees the specified section, previously allocated using Allocate().
	Note that a_Section may be nullptr. */
	void Free(sChunkSection * a_Section);
	
	/** Sets the data in the specified section to their default values. */
	void ZeroSection(sChunkSection * a_Section) const;

};






