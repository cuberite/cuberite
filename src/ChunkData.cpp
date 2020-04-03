
// ChunkData.cpp

// Implements the cChunkData class that represents the block's type, meta, blocklight and skylight storage for a chunk

#include "Globals.h"
#include "ChunkData.h"
#include "BlockType.h"





namespace
{
	/** Returns true if all a_Array's elements between [0] and [a_NumElements - 1] are equal to a_Value. */
	template <typename T>
	bool IsAllValue(const T * a_Array, size_t a_NumElements, T a_Value)
	{
		for (size_t i = 0; i < a_NumElements; i++)
		{
			if (a_Array[i] != a_Value)
			{
				return false;
			}
		}
		return true;
	}





	struct sSectionIndices
	{
		int Section = 0;  // Index into m_Sections
		int Index = 0;    // Index into a single sChunkSection
	};

	sSectionIndices IndicesFromRelPos(Vector3i a_RelPos)
	{
		ASSERT(cChunkDef::IsValidRelPos(a_RelPos));
		sSectionIndices Ret;
		Ret.Section = a_RelPos.y / cChunkData::SectionHeight;
		Ret.Index = cChunkDef::MakeIndexNoCheck(a_RelPos.x, a_RelPos.y % cChunkData::SectionHeight, a_RelPos.z);
		return Ret;
	}
}  // namespace (anonymous)





cChunkData::cChunkData(cAllocationPool<cChunkData::sChunkSection> & a_Pool):
	m_Sections(),
	m_Pool(a_Pool)
{
}





cChunkData::cChunkData(cChunkData && a_Other):
	m_Pool(a_Other.m_Pool)
{
	for (size_t i = 0; i < NumSections; i++)
	{
		m_Sections[i] = a_Other.m_Sections[i];
		a_Other.m_Sections[i] = nullptr;
	}
}





cChunkData::~cChunkData()
{
	Clear();
}





void cChunkData::Assign(const cChunkData & a_Other)
{
	// If assigning to self, no-op
	if (&a_Other == this)
	{
		return;
	}

	Clear();
	for (size_t i = 0; i < NumSections; ++i)
	{
		if (a_Other.m_Sections[i] != nullptr)
		{
			m_Sections[i] = Allocate();
			*m_Sections[i] = *a_Other.m_Sections[i];
		}
	}
}





void cChunkData::Assign(cChunkData && a_Other)
{
	if (&a_Other == this)
	{
		return;
	}

	if (m_Pool != a_Other.m_Pool)
	{
		// Cannot transfer the memory, do a copy instead
		const cChunkData & CopyOther = a_Other;
		Assign(CopyOther);
		return;
	}

	Clear();
	for (size_t i = 0; i < NumSections; i++)
	{
		m_Sections[i] = a_Other.m_Sections[i];
		a_Other.m_Sections[i] = nullptr;
	}
}





BLOCKTYPE cChunkData::GetBlock(Vector3i a_RelPos) const
{
	if (!cChunkDef::IsValidRelPos(a_RelPos))
	{
		return E_BLOCK_AIR;  // Coordinates are outside outside the world, so this must be an air block
	}
	auto Idxs = IndicesFromRelPos(a_RelPos);
	if (m_Sections[Idxs.Section] != nullptr)
	{
		return m_Sections[Idxs.Section]->m_BlockTypes[Idxs.Index];
	}
	else
	{
		return 0;
	}
}





void cChunkData::SetBlock(Vector3i a_RelPos, BLOCKTYPE a_Block)
{
	if (!cChunkDef::IsValidRelPos(a_RelPos))
	{
		ASSERT(!"cChunkData::SetMeta(): index out of range!");
		return;
	}

	auto Idxs = IndicesFromRelPos(a_RelPos);
	if (m_Sections[Idxs.Section] == nullptr)
	{
		if (a_Block == 0x00)
		{
			return;
		}
		m_Sections[Idxs.Section] = Allocate();
		if (m_Sections[Idxs.Section] == nullptr)
		{
			ASSERT(!"Failed to allocate a new section in Chunkbuffer");
			return;
		}
		ZeroSection(m_Sections[Idxs.Section]);
	}
	m_Sections[Idxs.Section]->m_BlockTypes[Idxs.Index] = a_Block;
}





NIBBLETYPE cChunkData::GetMeta(Vector3i a_RelPos) const
{
	if (cChunkDef::IsValidRelPos(a_RelPos))
	{
		auto Idxs = IndicesFromRelPos(a_RelPos);
		if (m_Sections[Idxs.Section] != nullptr)
		{
			return (m_Sections[Idxs.Section]->m_BlockMetas[Idxs.Index / 2] >> ((Idxs.Index & 1) * 4)) & 0x0f;
		}
		else
		{
			return 0;
		}
	}
	// Coordinates are outside outside the world, so it must be an air block with a blank meta
	return 0;
}





bool cChunkData::SetMeta(Vector3i a_RelPos, NIBBLETYPE a_Nibble)
{
	if (!cChunkDef::IsValidRelPos(a_RelPos))
	{
		ASSERT(!"cChunkData::SetMeta(): index out of range!");
		return false;
	}

	auto Idxs = IndicesFromRelPos(a_RelPos);
	if (m_Sections[Idxs.Section] == nullptr)
	{
		if ((a_Nibble & 0xf) == 0x00)
		{
			return false;
		}
		m_Sections[Idxs.Section] = Allocate();
		if (m_Sections[Idxs.Section] == nullptr)
		{
			ASSERT(!"Failed to allocate a new section in Chunkbuffer");
			return false;
		}
		ZeroSection(m_Sections[Idxs.Section]);
	}
	NIBBLETYPE oldval = m_Sections[Idxs.Section]->m_BlockMetas[Idxs.Index / 2] >> ((Idxs.Index & 1) * 4) & 0xf;
	m_Sections[Idxs.Section]->m_BlockMetas[Idxs.Index / 2] = static_cast<NIBBLETYPE>(
		(m_Sections[Idxs.Section]->m_BlockMetas[Idxs.Index / 2] & (0xf0 >> ((Idxs.Index & 1) * 4))) |  // The untouched nibble
		((a_Nibble & 0x0f) << ((Idxs.Index & 1) * 4))  // The nibble being set
	);
	return oldval != a_Nibble;
}





NIBBLETYPE cChunkData::GetBlockLight(Vector3i a_RelPos) const
{
	if (cChunkDef::IsValidRelPos(a_RelPos))
	{
		auto Idxs = IndicesFromRelPos(a_RelPos);
		if (m_Sections[Idxs.Section] != nullptr)
		{
			return (m_Sections[Idxs.Section]->m_BlockLight[Idxs.Index / 2] >> ((Idxs.Index & 1) * 4)) & 0x0f;
		}
		else
		{
			return 0;
		}
	}
	ASSERT(!"cChunkData::GetMeta(): coords out of chunk range!");
	return 0;
}





NIBBLETYPE cChunkData::GetSkyLight(Vector3i a_RelPos) const
{
	if (cChunkDef::IsValidRelPos(a_RelPos))
	{
		auto Idxs = IndicesFromRelPos(a_RelPos);
		if (m_Sections[Idxs.Section] != nullptr)
		{
			return (m_Sections[Idxs.Section]->m_BlockSkyLight[Idxs.Index / 2] >> ((Idxs.Index & 1) * 4)) & 0x0f;
		}
		else
		{
			return 0xF;
		}
	}
	ASSERT(!"cChunkData::GetMeta(): coords out of chunk range!");
	return 0;
}





const cChunkData::sChunkSection * cChunkData::GetSection(size_t a_SectionNum) const
{
	if (a_SectionNum < NumSections)
	{
		return m_Sections[a_SectionNum];
	}
	ASSERT(!"cChunkData::GetSection: section index out of range");
	return nullptr;
}





UInt16 cChunkData::GetSectionBitmask() const
{
	static_assert(NumSections <= 16U, "cChunkData::GetSectionBitmask needs a bigger data type");
	UInt16 Res = 0U;
	for (size_t i = 0U; i < NumSections; ++i)
	{
		Res |= ((m_Sections[i] != nullptr) << i);
	}
	return Res;
}





void cChunkData::Clear()
{
	for (size_t i = 0; i < NumSections; ++i)
	{
		if (m_Sections[i] != nullptr)
		{
			Free(m_Sections[i]);
			m_Sections[i] = nullptr;
		}
	}
}





void cChunkData::CopyBlockTypes(BLOCKTYPE * a_Dest, size_t a_Idx, size_t a_Length) const
{
	size_t ToSkip = a_Idx;

	for (size_t i = 0; i < NumSections; i++)
	{
		size_t StartPos = 0;
		if (ToSkip > 0)
		{
			StartPos = std::min(ToSkip, +SectionBlockCount);
			ToSkip -= StartPos;
		}
		if (StartPos < SectionBlockCount)
		{
			size_t ToCopy = std::min(+SectionBlockCount - StartPos, a_Length);
			a_Length -= ToCopy;
			if (m_Sections[i] != nullptr)
			{
				BLOCKTYPE * blockbuffer = m_Sections[i]->m_BlockTypes;
				memcpy(&a_Dest[(i * SectionBlockCount) + StartPos - a_Idx], blockbuffer + StartPos, sizeof(BLOCKTYPE) * ToCopy);
			}
			else
			{
				memset(&a_Dest[(i * SectionBlockCount) + StartPos - a_Idx], 0, sizeof(BLOCKTYPE) * ToCopy);
			}
		}
	}
}





void cChunkData::CopyMetas(NIBBLETYPE * a_Dest) const
{
	for (size_t i = 0; i < NumSections; i++)
	{
		if (m_Sections[i] != nullptr)
		{
			memcpy(&a_Dest[i * SectionBlockCount / 2], &m_Sections[i]->m_BlockMetas, sizeof(m_Sections[i]->m_BlockMetas));
		}
		else
		{
			memset(&a_Dest[i * SectionBlockCount / 2], 0, sizeof(m_Sections[i]->m_BlockMetas));
		}
	}
}





void cChunkData::CopyBlockLight(NIBBLETYPE * a_Dest) const
{
	for (size_t i = 0; i < NumSections; i++)
	{
		if (m_Sections[i] != nullptr)
		{
			memcpy(&a_Dest[i * SectionBlockCount / 2], &m_Sections[i]->m_BlockLight, sizeof(m_Sections[i]->m_BlockLight));
		}
		else
		{
			memset(&a_Dest[i * SectionBlockCount / 2], 0, sizeof(m_Sections[i]->m_BlockLight));
		}
	}
}





void cChunkData::CopySkyLight(NIBBLETYPE * a_Dest) const
{
	for (size_t i = 0; i < NumSections; i++)
	{
		if (m_Sections[i] != nullptr)
		{
			memcpy(&a_Dest[i * SectionBlockCount / 2], &m_Sections[i]->m_BlockSkyLight, sizeof(m_Sections[i]->m_BlockSkyLight));
		}
		else
		{
			memset(&a_Dest[i * SectionBlockCount / 2], 0xff, sizeof(m_Sections[i]->m_BlockSkyLight));
		}
	}
}





void cChunkData::FillBlockTypes(BLOCKTYPE a_Value)
{
	// If needed, allocate any missing sections
	if (a_Value != 0x00)
	{
		for (auto & Section : m_Sections)
		{
			if (Section == nullptr)
			{
				Section = Allocate();
				std::fill(std::begin(Section->m_BlockMetas),    std::end(Section->m_BlockMetas),    0x00);
				std::fill(std::begin(Section->m_BlockLight),    std::end(Section->m_BlockLight),    0x00);
				std::fill(std::begin(Section->m_BlockSkyLight), std::end(Section->m_BlockSkyLight), 0xff);
			}
		}
	}

	for (auto Section : m_Sections)
	{
		if (Section != nullptr)
		{
			std::fill(std::begin(Section->m_BlockTypes), std::end(Section->m_BlockTypes), a_Value);
		}
	}
}





void cChunkData::FillMetas(NIBBLETYPE a_Value)
{
	// If needed, allocate any missing sections
	if (a_Value != 0x00)
	{
		for (auto & Section : m_Sections)
		{
			if (Section == nullptr)
			{
				Section = Allocate();
				std::fill(std::begin(Section->m_BlockTypes),    std::end(Section->m_BlockTypes),    0x00);
				std::fill(std::begin(Section->m_BlockLight),    std::end(Section->m_BlockLight),    0x00);
				std::fill(std::begin(Section->m_BlockSkyLight), std::end(Section->m_BlockSkyLight), 0xff);
			}
		}
	}

	NIBBLETYPE NewMeta = static_cast<NIBBLETYPE>((a_Value << 4) | a_Value);
	for (auto Section : m_Sections)
	{
		if (Section != nullptr)
		{
			std::fill(std::begin(Section->m_BlockMetas), std::end(Section->m_BlockMetas), NewMeta);
		}
	}
}





void cChunkData::FillBlockLight(NIBBLETYPE a_Value)
{
	// If needed, allocate any missing sections
	if (a_Value != 0x00)
	{
		for (auto & Section : m_Sections)
		{
			if (Section == nullptr)
			{
				Section = Allocate();
				std::fill(std::begin(Section->m_BlockTypes), std::end(Section->m_BlockTypes), 0x00);
				std::fill(std::begin(Section->m_BlockMetas), std::end(Section->m_BlockMetas), 0x00);
				std::fill(std::begin(Section->m_BlockSkyLight), std::end(Section->m_BlockSkyLight), 0xff);
			}
		}
	}

	NIBBLETYPE NewLight = static_cast<NIBBLETYPE>((a_Value << 4) | a_Value);
	for (auto Section : m_Sections)
	{
		if (Section != nullptr)
		{
			std::fill(std::begin(Section->m_BlockLight), std::end(Section->m_BlockLight), NewLight);
		}
	}
}





void cChunkData::FillSkyLight(NIBBLETYPE a_Value)
{
	// If needed, allocate any missing sections
	if (a_Value != 0x0f)
	{
		for (auto & Section : m_Sections)
		{
			if (Section == nullptr)
			{
				Section = Allocate();
				std::fill(std::begin(Section->m_BlockTypes), std::end(Section->m_BlockTypes), 0x00);
				std::fill(std::begin(Section->m_BlockMetas), std::end(Section->m_BlockMetas), 0x00);
				std::fill(std::begin(Section->m_BlockLight), std::end(Section->m_BlockLight), 0x00);
			}
		}
	}

	NIBBLETYPE NewSkyLight = static_cast<NIBBLETYPE>((a_Value << 4) | a_Value);
	for (auto Section : m_Sections)
	{
		if (Section != nullptr)
		{
			std::fill(std::begin(Section->m_BlockSkyLight), std::end(Section->m_BlockSkyLight), NewSkyLight);
		}
	}
}





void cChunkData::SetBlockTypes(const BLOCKTYPE * a_Src)
{
	ASSERT(a_Src != nullptr);

	for (size_t i = 0; i < NumSections; i++)
	{
		// If the section is already allocated, copy the data into it:
		if (m_Sections[i] != nullptr)
		{
			memcpy(m_Sections[i]->m_BlockTypes, &a_Src[i * SectionBlockCount], sizeof(m_Sections[i]->m_BlockTypes));
			continue;
		}

		// The section doesn't exist, find out if it is needed:
		if (IsAllValue(a_Src + i * SectionBlockCount, SectionBlockCount, static_cast<BLOCKTYPE>(0)))
		{
			// No need for the section, the data is all-air
			continue;
		}

		// Allocate the section and copy the data into it:
		m_Sections[i] = Allocate();
		memcpy(m_Sections[i]->m_BlockTypes, &a_Src[i * SectionBlockCount], sizeof(m_Sections[i]->m_BlockTypes));
		memset(m_Sections[i]->m_BlockMetas,    0x00, sizeof(m_Sections[i]->m_BlockMetas));
		memset(m_Sections[i]->m_BlockLight,    0x00, sizeof(m_Sections[i]->m_BlockLight));
		memset(m_Sections[i]->m_BlockSkyLight, 0xff, sizeof(m_Sections[i]->m_BlockSkyLight));
	}  // for i - m_Sections[]
}





void cChunkData::SetMetas(const NIBBLETYPE * a_Src)
{
	ASSERT(a_Src != nullptr);

	for (size_t i = 0; i < NumSections; i++)
	{
		// If the section is already allocated, copy the data into it:
		if (m_Sections[i] != nullptr)
		{
			memcpy(m_Sections[i]->m_BlockMetas, &a_Src[i * SectionBlockCount / 2], sizeof(m_Sections[i]->m_BlockMetas));
			continue;
		}

		// The section doesn't exist, find out if it is needed:
		if (IsAllValue(a_Src + i * SectionBlockCount / 2, SectionBlockCount / 2, static_cast<NIBBLETYPE>(0)))
		{
			// No need for the section, the data is all zeroes
			continue;
		}

		// Allocate the section and copy the data into it:
		m_Sections[i] = Allocate();
		memcpy(m_Sections[i]->m_BlockMetas, &a_Src[i * SectionBlockCount / 2], sizeof(m_Sections[i]->m_BlockMetas));
		memset(m_Sections[i]->m_BlockTypes,    0x00, sizeof(m_Sections[i]->m_BlockTypes));
		memset(m_Sections[i]->m_BlockLight,    0x00, sizeof(m_Sections[i]->m_BlockLight));
		memset(m_Sections[i]->m_BlockSkyLight, 0xff, sizeof(m_Sections[i]->m_BlockSkyLight));
	}  // for i - m_Sections[]
}





void cChunkData::SetBlockLight(const NIBBLETYPE * a_Src)
{
	if (a_Src == nullptr)
	{
		return;
	}

	for (size_t i = 0; i < NumSections; i++)
	{
		// If the section is already allocated, copy the data into it:
		if (m_Sections[i] != nullptr)
		{
			memcpy(m_Sections[i]->m_BlockLight, &a_Src[i * SectionBlockCount / 2], sizeof(m_Sections[i]->m_BlockLight));
			continue;
		}

		// The section doesn't exist, find out if it is needed:
		if (IsAllValue(a_Src + i * SectionBlockCount / 2, SectionBlockCount / 2, static_cast<NIBBLETYPE>(0)))
		{
			// No need for the section, the data is all zeroes
			continue;
		}

		// Allocate the section and copy the data into it:
		m_Sections[i] = Allocate();
		memcpy(m_Sections[i]->m_BlockLight, &a_Src[i * SectionBlockCount / 2], sizeof(m_Sections[i]->m_BlockLight));
		memset(m_Sections[i]->m_BlockTypes,    0x00, sizeof(m_Sections[i]->m_BlockTypes));
		memset(m_Sections[i]->m_BlockMetas,    0x00, sizeof(m_Sections[i]->m_BlockMetas));
		memset(m_Sections[i]->m_BlockSkyLight, 0xff, sizeof(m_Sections[i]->m_BlockSkyLight));
	}  // for i - m_Sections[]
}





void cChunkData::SetSkyLight(const NIBBLETYPE * a_Src)
{
	if (a_Src == nullptr)
	{
		return;
	}

	for (size_t i = 0; i < NumSections; i++)
	{
		// If the section is already allocated, copy the data into it:
		if (m_Sections[i] != nullptr)
		{
			memcpy(m_Sections[i]->m_BlockSkyLight, &a_Src[i * SectionBlockCount / 2], sizeof(m_Sections[i]->m_BlockSkyLight));
			continue;
		}

		// The section doesn't exist, find out if it is needed:
		if (IsAllValue(a_Src + i * SectionBlockCount / 2, SectionBlockCount / 2, static_cast<NIBBLETYPE>(0xff)))
		{
			// No need for the section, the data is all zeroes
			continue;
		}

		// Allocate the section and copy the data into it:
		m_Sections[i] = Allocate();
		memcpy(m_Sections[i]->m_BlockSkyLight, &a_Src[i * SectionBlockCount / 2], sizeof(m_Sections[i]->m_BlockSkyLight));
		memset(m_Sections[i]->m_BlockTypes, 0x00, sizeof(m_Sections[i]->m_BlockTypes));
		memset(m_Sections[i]->m_BlockMetas, 0x00, sizeof(m_Sections[i]->m_BlockMetas));
		memset(m_Sections[i]->m_BlockLight, 0x00, sizeof(m_Sections[i]->m_BlockLight));
	}  // for i - m_Sections[]
}





UInt32 cChunkData::NumPresentSections() const
{
	UInt32 Ret = 0U;
	for (size_t i = 0; i < NumSections; i++)
	{
		if (m_Sections[i] != nullptr)
		{
			++Ret;
		}
	}
	return Ret;
}





cChunkData::sChunkSection * cChunkData::Allocate(void)
{
	return m_Pool.Allocate();
}





void cChunkData::Free(cChunkData::sChunkSection * a_Section)
{
	m_Pool.Free(a_Section);
}





void cChunkData::ZeroSection(cChunkData::sChunkSection * a_Section) const
{
	memset(a_Section->m_BlockTypes,    0x00, sizeof(a_Section->m_BlockTypes));
	memset(a_Section->m_BlockMetas,    0x00, sizeof(a_Section->m_BlockMetas));
	memset(a_Section->m_BlockLight,    0x00, sizeof(a_Section->m_BlockLight));
	memset(a_Section->m_BlockSkyLight, 0xff, sizeof(a_Section->m_BlockSkyLight));
}




