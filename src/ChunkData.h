
// ChunkData.h

// Declares the cChunkData class that represents the block's type, meta, blocklight and skylight storage for a chunk





#pragma once


#include <cstring>

#include "AllocationPool.h"
#include "ChunkDef.h"


class cChunkData
{
public:

	static const int SectionHeight = 16;
	static const size_t NumSections = (cChunkDef::Height / SectionHeight);
	static const size_t SectionBlockCount = SectionHeight * cChunkDef::Width * cChunkDef::Width;

	struct sChunkSection
	{
		BLOCKTYPE  m_BlockTypes[SectionBlockCount];
		NIBBLETYPE m_BlockMetas[SectionBlockCount / 2];
		NIBBLETYPE m_BlockLight[SectionBlockCount / 2];
		NIBBLETYPE m_BlockSkyLight[SectionBlockCount / 2];
	};

	cChunkData(cAllocationPool<sChunkSection> & a_Pool);
	cChunkData(cChunkData && a_Other);
	~cChunkData();

	cChunkData & operator = (cChunkData && a_Other)
	{
		Assign(std::move(a_Other));
		return *this;
	}

	/** Copy assign from another cChunkData */
	void Assign(const cChunkData & a_Other);

	/** Move assign from another cChunkData */
	void Assign(cChunkData && a_Other);

	BLOCKTYPE GetBlock(Vector3i a_RelPos) const;
	void SetBlock(Vector3i a_RelPos, BLOCKTYPE a_Block);

	NIBBLETYPE GetMeta(Vector3i a_RelPos) const;
	bool SetMeta(Vector3i a_RelPos, NIBBLETYPE a_Nibble);

	NIBBLETYPE GetBlockLight(Vector3i a_RelPos) const;

	NIBBLETYPE GetSkyLight(Vector3i a_RelPos) const;

	/** Return a pointer to the chunk section or nullptr if all air */
	const sChunkSection * GetSection(size_t a_SectionNum) const;

	/** Returns a bitmask of chunk sections which are currently stored. */
	UInt16 GetSectionBitmask() const;

	/** Clears all data */
	void Clear();

	/** Copies the blocktype data into the specified flat array.
	Optionally, only a part of the data is copied, as specified by the a_Idx and a_Length parameters. */
	void CopyBlockTypes(BLOCKTYPE * a_Dest, size_t a_Idx = 0, size_t a_Length = cChunkDef::NumBlocks) const;

	/** Copies the metadata into the specified flat array. */
	void CopyMetas(NIBBLETYPE * a_Dest) const;

	/** Copies the block light data into the specified flat array. */
	void CopyBlockLight(NIBBLETYPE * a_Dest) const;

	/** Copies the skylight data into the specified flat array. */
	void CopySkyLight  (NIBBLETYPE * a_Dest) const;

	/** Fills the chunk with the specified block. */
	void FillBlockTypes(BLOCKTYPE a_Value);

	/** Fills the chunk with the specified meta value. */
	void FillMetas     (NIBBLETYPE a_Value);

	/** Fills the chunk with the specified block light. */
	void FillBlockLight(NIBBLETYPE a_Value);

	/** Fills the chunk with the specified sky light. */
	void FillSkyLight  (NIBBLETYPE a_Value);

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

	/** Returns the number of sections present (i.e. non-air). */
	UInt32 NumPresentSections() const;

private:

	sChunkSection * m_Sections[NumSections];

	cAllocationPool<sChunkSection> & m_Pool;

	/** Allocates a new section. Entry-point to custom allocators. */
	sChunkSection * Allocate(void);

	/** Frees the specified section, previously allocated using Allocate().
	Note that a_Section may be nullptr. */
	void Free(sChunkSection * a_Section);

	/** Sets the data in the specified section to their default values. */
	void ZeroSection(sChunkSection * a_Section) const;

};






