
// ChunkData.cpp

// Implements the cChunkData class that represents the block's type, meta, blocklight and skylight storage for a chunk

#include "Globals.h"
#include "ChunkData.h"





/** Returns true if all a_Array's elements between [0] and [a_NumElements - 1] are equal to a_Value. */
template <typename T> inline bool IsAllValue(const T * a_Array, size_t a_NumElements, T a_Value)
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





cChunkData::cChunkData(void)
#if __cplusplus < 201103L
	// auto_ptr style interface for memory management
	: m_IsOwner(true)
#endif
{
	for (size_t i = 0; i < NumSections; i++)
	{
		m_Sections[i] = NULL;
	}
}





cChunkData::~cChunkData()
{
	#if __cplusplus < 201103L
		// auto_ptr style interface for memory management
		if (!m_IsOwner)
		{
			return;
		}
	#endif
	for (size_t i = 0; i < NumSections; i++)
	{
		Free(m_Sections[i]);
		m_Sections[i] = NULL;
	}
}





#if __cplusplus < 201103L
	// auto_ptr style interface for memory management
	cChunkData::cChunkData(const cChunkData & a_Other) :
		m_IsOwner(true)
	{
		// Move contents and ownership from a_Other to this, pointer-wise:
		for (size_t i = 0; i < NumSections; i++)
		{
			m_Sections[i] = a_Other.m_Sections[i];
		}
		a_Other.m_IsOwner = false;
	}





	cChunkData & cChunkData::operator =(const cChunkData & a_Other)
	{
		// If assigning to self, no-op
		if (&a_Other == this)
		{
			return *this;
		}
		
		// Free any currently held contents:
		if (m_IsOwner)
		{
			for (size_t i = 0; i < NumSections; i++)
			{
				Free(m_Sections[i]);
				m_Sections[i] = NULL;
			}
		}
		
		// Move contents and ownership from a_Other to this, pointer-wise:
		m_IsOwner = true;
		for (size_t i = 0; i < NumSections; i++)
		{
			m_Sections[i] = a_Other.m_Sections[i];
		}
		a_Other.m_IsOwner = false;
		return *this;
	}
	
#else

	// unique_ptr style interface for memory management
	cChunkData::cChunkData(cChunkData && other)
	{
		for (size_t i = 0; i < NumSections; i++)
		{
			m_Sections[i] = other.m_Sections[i];
			other.m_Sections[i] = NULL;
		}
	}
	
	
	
	

	cChunkData & cChunkData::operator =(cChunkData && other)
	{
		if (&other != this)
		{
			for (size_t i = 0; i < NumSections; i++)
			{
				Free(m_Sections[i]);
				m_Sections[i] = other.m_Sections[i];
				other.m_Sections[i] = NULL;
			}
		}
		return *this;
	}
#endif





BLOCKTYPE cChunkData::GetBlock(int a_X, int a_Y, int a_Z) const
{
	ASSERT((a_X >= 0) && (a_X < cChunkDef::Width));
	ASSERT((a_Y >= 0) && (a_Y < cChunkDef::Height));
	ASSERT((a_Z >= 0) && (a_Z < cChunkDef::Width));
	int Section = a_Y / SectionHeight;
	if (m_Sections[Section] != NULL)
	{
		int Index = cChunkDef::MakeIndexNoCheck(a_X, a_Y - (Section * SectionHeight), a_Z);
		return m_Sections[Section]->m_BlockTypes[Index];
	}
	else
	{
		return 0;
	}
}





void cChunkData::SetBlock(int a_RelX, int a_RelY, int a_RelZ, BLOCKTYPE a_Block)
{
	if (
		(a_RelX >= cChunkDef::Width)  || (a_RelX < 0) ||
		(a_RelY >= cChunkDef::Height) || (a_RelY < 0) ||
		(a_RelZ >= cChunkDef::Width)  || (a_RelZ < 0)
	)
	{
		ASSERT(!"cChunkData::SetMeta(): index out of range!");
		return;
	}

	int Section = a_RelY / SectionHeight;
	if (m_Sections[Section] == NULL)
	{
		if (a_Block == 0x00)
		{
			return;
		}
		m_Sections[Section] = Allocate();
		if (m_Sections[Section] == NULL)
		{
			ASSERT(!"Failed to allocate a new section in Chunkbuffer");
			return;
		}
		ZeroSection(m_Sections[Section]);
	}
	int Index = cChunkDef::MakeIndexNoCheck(a_RelX, a_RelY - (Section * SectionHeight), a_RelZ);
	m_Sections[Section]->m_BlockTypes[Index] = a_Block;
}





NIBBLETYPE cChunkData::GetMeta(int a_RelX, int a_RelY, int a_RelZ) const
{
	if (
		(a_RelX < cChunkDef::Width) && (a_RelX > -1) &&
		(a_RelY < cChunkDef::Height) && (a_RelY > -1) &&
		(a_RelZ < cChunkDef::Width) && (a_RelZ > -1))
	{
		int Section = a_RelY / SectionHeight;
		if (m_Sections[Section] != NULL)
		{
			int Index = cChunkDef::MakeIndexNoCheck(a_RelX, a_RelY - (Section * SectionHeight), a_RelZ);
			return (m_Sections[Section]->m_BlockMetas[Index / 2] >> ((Index & 1) * 4)) & 0x0f;
		}
		else
		{
			return 0;
		}
	}
	ASSERT(!"cChunkData::GetMeta(): coords out of chunk range!");
	return 0;
}





bool cChunkData::SetMeta(int a_RelX, int a_RelY, int a_RelZ, NIBBLETYPE a_Nibble)
{
	if (
		(a_RelX >= cChunkDef::Width)  || (a_RelX < 0) ||
		(a_RelY >= cChunkDef::Height) || (a_RelY < 0) ||
		(a_RelZ >= cChunkDef::Width)  || (a_RelZ < 0)
	)
	{
		ASSERT(!"cChunkData::SetMeta(): index out of range!");
		return false;
	}

	int Section = a_RelY / SectionHeight;
	if (m_Sections[Section] == NULL)
	{
		if ((a_Nibble & 0xf) == 0x00)
		{
			return false;
		}
		m_Sections[Section] = Allocate();
		if (m_Sections[Section] == NULL)
		{
			ASSERT(!"Failed to allocate a new section in Chunkbuffer");
			return false;
		}
		ZeroSection(m_Sections[Section]);
	}
	int Index = cChunkDef::MakeIndexNoCheck(a_RelX, a_RelY - (Section * SectionHeight), a_RelZ);
	NIBBLETYPE oldval = m_Sections[Section]->m_BlockMetas[Index / 2] >> ((Index & 1) * 4) & 0xf;
	m_Sections[Section]->m_BlockMetas[Index / 2] = static_cast<NIBBLETYPE>(
		(m_Sections[Section]->m_BlockMetas[Index / 2] & (0xf0 >> ((Index & 1) * 4))) |  // The untouched nibble
		((a_Nibble & 0x0f) << ((Index & 1) * 4))  // The nibble being set
	);
	return oldval != a_Nibble;
}





NIBBLETYPE cChunkData::GetBlockLight(int a_RelX, int a_RelY, int a_RelZ) const
{
	if (
		(a_RelX < cChunkDef::Width) && (a_RelX > -1) &&
		(a_RelY < cChunkDef::Height) && (a_RelY > -1) &&
		(a_RelZ < cChunkDef::Width) && (a_RelZ > -1)
	)
	{
		int Section = a_RelY / SectionHeight;
		if (m_Sections[Section] != NULL)
		{
			int Index = cChunkDef::MakeIndexNoCheck(a_RelX, a_RelY - (Section * SectionHeight), a_RelZ);
			return (m_Sections[Section]->m_BlockLight[Index / 2] >> ((Index & 1) * 4)) & 0x0f;
		}
		else
		{
			return 0;
		}
	}
	ASSERT(!"cChunkData::GetMeta(): coords out of chunk range!");
	return 0;
}





NIBBLETYPE cChunkData::GetSkyLight(int a_RelX, int a_RelY, int a_RelZ) const
{
	if ((a_RelX < cChunkDef::Width) && (a_RelX > -1) && (a_RelY < cChunkDef::Height) && (a_RelY > -1) && (a_RelZ < cChunkDef::Width) && (a_RelZ > -1))
	{
		int Section = a_RelY / SectionHeight;
		if (m_Sections[Section] != NULL)
		{
			int Index = cChunkDef::MakeIndexNoCheck(a_RelX, a_RelY - (Section * SectionHeight), a_RelZ);
			return (m_Sections[Section]->m_BlockSkyLight[Index / 2] >> ((Index & 1) * 4)) & 0x0f;
		}
		else
		{
			return 0xF;
		}
	}
	ASSERT(!"cChunkData::GetMeta(): coords out of chunk range!");
	return 0;
}





cChunkData cChunkData::Copy(void) const
{
	cChunkData copy;
	for (size_t i = 0; i < NumSections; i++)
	{
		if (m_Sections[i] != NULL)
		{
			copy.m_Sections[i] = Allocate();
			*copy.m_Sections[i] = *m_Sections[i];
		}
	}
	return copy;
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
			if (m_Sections[i] != NULL)
			{
				BLOCKTYPE * blockbuffer = m_Sections[i]->m_BlockTypes;
				memcpy(&a_Dest[(i * SectionBlockCount) + StartPos - a_Idx], blockbuffer + StartPos, sizeof(BLOCKTYPE) * ToCopy);
			}
			else
			{
				memset(&a_Dest[(i * SectionBlockCount) - a_Idx], 0, sizeof(BLOCKTYPE) * ToCopy);
			}
		}
	}
}





void cChunkData::CopyMetas(NIBBLETYPE * a_Dest) const
{
	for (size_t i = 0; i < NumSections; i++)
	{
		if (m_Sections[i] != NULL)
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
		if (m_Sections[i] != NULL)
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
		if (m_Sections[i] != NULL)
		{
			memcpy(&a_Dest[i * SectionBlockCount / 2], &m_Sections[i]->m_BlockSkyLight, sizeof(m_Sections[i]->m_BlockSkyLight));
		}
		else
		{
			memset(&a_Dest[i * SectionBlockCount / 2], 0xff, sizeof(m_Sections[i]->m_BlockSkyLight));
		}
	}
}





void cChunkData::SetBlockTypes(const BLOCKTYPE * a_Src)
{
	ASSERT(a_Src != NULL);
	
	for (size_t i = 0; i < NumSections; i++)
	{
		// If the section is already allocated, copy the data into it:
		if (m_Sections[i] != NULL)
		{
			memcpy(m_Sections[i]->m_BlockTypes, &a_Src[i * SectionBlockCount], sizeof(m_Sections[i]->m_BlockTypes));
			continue;
		}

		// The section doesn't exist, find out if it is needed:
		if (IsAllValue(a_Src + i * SectionBlockCount, SectionBlockCount, (const BLOCKTYPE)0))
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
	ASSERT(a_Src != NULL);
	
	for (size_t i = 0; i < NumSections; i++)
	{
		// If the section is already allocated, copy the data into it:
		if (m_Sections[i] != NULL)
		{
			memcpy(m_Sections[i]->m_BlockMetas, &a_Src[i * SectionBlockCount / 2], sizeof(m_Sections[i]->m_BlockMetas));
			continue;
		}
		
		// The section doesn't exist, find out if it is needed:
		if (IsAllValue(a_Src + i * SectionBlockCount / 2, SectionBlockCount / 2, (NIBBLETYPE)0))
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
	if (a_Src == NULL)
	{
		return;
	}
	
	for (size_t i = 0; i < NumSections; i++)
	{
		// If the section is already allocated, copy the data into it:
		if (m_Sections[i] != NULL)
		{
			memcpy(m_Sections[i]->m_BlockLight, &a_Src[i * SectionBlockCount / 2], sizeof(m_Sections[i]->m_BlockLight));
			continue;
		}
		
		// The section doesn't exist, find out if it is needed:
		if (IsAllValue(a_Src + i * SectionBlockCount / 2, SectionBlockCount / 2, (NIBBLETYPE)0))
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
	if (a_Src == NULL)
	{
		return;
	}
	
	for (size_t i = 0; i < NumSections; i++)
	{
		// If the section is already allocated, copy the data into it:
		if (m_Sections[i] != NULL)
		{
			memcpy(m_Sections[i]->m_BlockSkyLight, &a_Src[i * SectionBlockCount / 2], sizeof(m_Sections[i]->m_BlockSkyLight));
			continue;
		}

		// The section doesn't exist, find out if it is needed:
		if (IsAllValue(a_Src + i * SectionBlockCount / 2, SectionBlockCount / 2, (NIBBLETYPE)0xff))
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





cChunkData::sChunkSection * cChunkData::Allocate(void)
{
	// TODO: Use an allocation pool
	return new cChunkData::sChunkSection;
}





void cChunkData::Free(cChunkData::sChunkSection * a_Section)
{
	// TODO: Use an allocation pool
	delete a_Section;
}





void cChunkData::ZeroSection(cChunkData::sChunkSection * a_Section) const
{
	memset(a_Section->m_BlockTypes,    0x00, sizeof(a_Section->m_BlockTypes));
	memset(a_Section->m_BlockMetas,    0x00, sizeof(a_Section->m_BlockMetas));
	memset(a_Section->m_BlockLight,    0x00, sizeof(a_Section->m_BlockLight));
	memset(a_Section->m_BlockSkyLight, 0xff, sizeof(a_Section->m_BlockSkyLight));
}




