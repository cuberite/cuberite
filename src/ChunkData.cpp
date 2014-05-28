
#include "Globals.h"
#include "ChunkData.h"

cChunkData::cChunkData()
#if __cplusplus < 201103L
	// auto_ptr style interface for memory management
	: m_IsOwner(true)
#endif
{
	for (size_t i = 0; i < CHUNK_SECTION_COUNT; i++)
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
	for (size_t i = 0; i < CHUNK_SECTION_COUNT; i++)
	{
		Free(m_Sections[i]);
	}
}





#if __cplusplus < 201103L
	// auto_ptr style interface for memory management
	cChunkData::cChunkData(const cChunkData & a_Other) :
		m_IsOwner(true)
	{
		// Move contents and ownership from a_Other to this, pointer-wise:
		for (size_t i = 0; i < CHUNK_SECTION_COUNT; i++)
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
			for (size_t i = 0; i < CHUNK_SECTION_COUNT; i++)
			{
				Free(m_Sections[i]);
			}
		}
		
		// Move contents and ownership from a_Other to this, pointer-wise:
		m_IsOwner = true;
		for (size_t i = 0; i < CHUNK_SECTION_COUNT; i++)
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
		for (size_t i = 0; i < CHUNK_SECTION_COUNT; i++)
		{
			m_Sections[i] = other.m_Sections[i];
			other.m_Sections[i] = NULL;
		}
	}
	
	
	
	

	cChunkData & cChunkData::operator =(cChunkData && other)
	{
		if (&other != this)
		{
			for (size_t i = 0; i < CHUNK_SECTION_COUNT; i++)
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
	int Section = a_Y / CHUNK_SECTION_HEIGHT;
	if (m_Sections[Section] != NULL)
	{
		int Index = cChunkDef::MakeIndexNoCheck(a_X, a_Y - (Section * CHUNK_SECTION_HEIGHT), a_Z);
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

	int Section = a_RelY / CHUNK_SECTION_HEIGHT;
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
	int Index = cChunkDef::MakeIndexNoCheck(a_RelX, a_RelY - (Section * CHUNK_SECTION_HEIGHT), a_RelZ);
	m_Sections[Section]->m_BlockTypes[Index] = a_Block;
}





NIBBLETYPE cChunkData::GetMeta(int a_RelX, int a_RelY, int a_RelZ) const
{
	if (
		(a_RelX < cChunkDef::Width) && (a_RelX > -1) &&
		(a_RelY < cChunkDef::Height) && (a_RelY > -1) &&
		(a_RelZ < cChunkDef::Width) && (a_RelZ > -1))
	{
		int Section = a_RelY / CHUNK_SECTION_HEIGHT;
		if (m_Sections[Section] != NULL)
		{
			int Index = cChunkDef::MakeIndexNoCheck(a_RelX, a_RelY - (Section * CHUNK_SECTION_HEIGHT), a_RelZ);
			return (m_Sections[Section]->m_BlockMeta[Index / 2] >> ((Index & 1) * 4)) & 0x0f;
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

	int Section = a_RelY / CHUNK_SECTION_HEIGHT;
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
	int Index = cChunkDef::MakeIndexNoCheck(a_RelX, a_RelY - (Section * CHUNK_SECTION_HEIGHT), a_RelZ);
	NIBBLETYPE oldval = m_Sections[Section]->m_BlockMeta[Index / 2] >> ((Index & 1) * 4) & 0xf;
	m_Sections[Section]->m_BlockMeta[Index / 2] = static_cast<NIBBLETYPE>(
		(m_Sections[Section]->m_BlockMeta[Index / 2] & (0xf0 >> ((Index & 1) * 4))) |  // The untouched nibble
		((a_Nibble & 0x0f) << ((Index & 1) * 4))  // The nibble being set
	);
	return oldval == a_Nibble;
}





NIBBLETYPE cChunkData::GetBlockLight(int a_RelX, int a_RelY, int a_RelZ) const
{
	if (
		(a_RelX < cChunkDef::Width) && (a_RelX > -1) &&
		(a_RelY < cChunkDef::Height) && (a_RelY > -1) &&
		(a_RelZ < cChunkDef::Width) && (a_RelZ > -1)
	)
	{
		int Section = a_RelY / CHUNK_SECTION_HEIGHT;
		if (m_Sections[Section] != NULL)
		{
			int Index = cChunkDef::MakeIndexNoCheck(a_RelX, a_RelY - (Section * CHUNK_SECTION_HEIGHT), a_RelZ);
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
		int Section = a_RelY / CHUNK_SECTION_HEIGHT;
		if (m_Sections[Section] != NULL)
		{
			int Index = cChunkDef::MakeIndexNoCheck(a_RelX, a_RelY - (Section * CHUNK_SECTION_HEIGHT), a_RelZ);
			return (m_Sections[Section]->m_BlockLight[Index / 2] >> ((Index & 1) * 4)) & 0x0f;
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
	for (size_t i = 0; i < CHUNK_SECTION_COUNT; i++)
	{
		if (m_Sections[i] != NULL)
		{
			copy.m_Sections[i] = Allocate();
			*copy.m_Sections[i] = *m_Sections[i];
		}
	}
	return copy;
}





void cChunkData::CopyBlocks(BLOCKTYPE * a_Dest, size_t a_Idx, size_t a_Length)  const
{
	for (size_t i = 0; i < CHUNK_SECTION_COUNT; i++)
	{
		const size_t SegmentLength = CHUNK_SECTION_HEIGHT * 16 * 16;
		if (a_Idx > 0)
		{
			a_Idx = std::max(a_Idx - a_Length, (size_t) 0);
		}
		if (a_Idx == 0) 
		{
			size_t ToCopy = std::min(SegmentLength, a_Length);
			length -= ToCopy;
			if (m_Sections[i] != NULL)
			{
				memcpy(
					&a_Dest[i * SegmentLength],
					&m_Sections[i]->m_BlockTypes, 
					sizeof(BLOCKTYPE) * ToCopy
				);
			}
			else
			{
				memset(
					&a_Dest[i * SegmentLength],
					0,
					sizeof(BLOCKTYPE) * ToCopy
				);
			}
		}
	}
}





void cChunkData::CopyMeta(NIBBLETYPE * a_Dest) const
{
	for (size_t i = 0; i < CHUNK_SECTION_COUNT; i++)
	{
		const size_t SegmentLength = CHUNK_SECTION_HEIGHT * 16 * 16 / 2;
		if (m_Sections[i] != NULL)
		{
			memcpy(
				&a_Dest[i * SegmentLength], 
				&m_Sections[i]->m_BlockMeta, 
				sizeof(NIBBLETYPE) * SegmentLength
			);
		}
		else
		{
			memset(
				&a_Dest[i * SegmentLength],
				0,
				sizeof(BLOCKTYPE) * SegmentLength
			);
		}
	}
}





void cChunkData::CopyBlockLight(NIBBLETYPE * a_Dest) const
{
	for (size_t i = 0; i < CHUNK_SECTION_COUNT; i++)
	{
		const size_t SegmentLength = CHUNK_SECTION_HEIGHT * 16 * 16 / 2;
		if (m_Sections[i] != NULL)
		{
			memcpy(
				&a_Dest[i * SegmentLength],
				&m_Sections[i]->m_BlockLight,
				sizeof(NIBBLETYPE) * SegmentLength
			);
		}
		else
		{
			memset(
				&a_Dest[i * SegmentLength],
				0,
				sizeof(BLOCKTYPE) * SegmentLength
			);
		}
	}
}





void cChunkData::CopySkyLight(NIBBLETYPE * a_Dest) const
{
	for (size_t i = 0; i < CHUNK_SECTION_COUNT; i++)
	{
		const size_t SegmentLength = CHUNK_SECTION_HEIGHT * 16 * 16 / 2;
		if (m_Sections[i] != NULL)
		{
			memcpy(
				&a_Dest[i * SegmentLength],
				&m_Sections[i]->m_BlockSkyLight,
				sizeof(NIBBLETYPE) * SegmentLength
			);
		}
		else
		{
			memset(
				&a_Dest[i * SegmentLength],
				0xFF,
				sizeof(BLOCKTYPE) * SegmentLength
			);
		}
	}
}





void cChunkData::SetBlocks(const BLOCKTYPE * a_Src)
{
	ASSERT(a_Src != NULL);
	
	for (size_t i = 0; i < CHUNK_SECTION_COUNT; i++)
	{
		const size_t SegmentLength = CHUNK_SECTION_HEIGHT * 16 * 16;
		if (m_Sections[i] != NULL)
		{
			memcpy(
				&m_Sections[i]->m_BlockTypes,
				&a_Src[i * SegmentLength],
				sizeof(BLOCKTYPE) * SegmentLength
			);
		}
		else
		{
			// j counts how many of leading zeros the buffer has
			// if j == SegmentLength then the buffer is all zeros so there is no point
			// creating the buffer.
			size_t j = 0;
			// do nothing whilst 0
			for (; (j < SegmentLength) && (a_Src[i * SegmentLength + j] == 0); j++);
			if (j != SegmentLength)
			{
				m_Sections[i] = Allocate();
				memcpy(
					&m_Sections[i]->m_BlockTypes, 
					&a_Src[i * SegmentLength],
					sizeof(BLOCKTYPE) * SegmentLength
					);
				memset(
					m_Sections[i]->m_BlockMeta,
					0x00,
					sizeof(m_Sections[i]->m_BlockMeta)
				);
				memset(
					m_Sections[i]->m_BlockLight,
					0x00,
					sizeof(m_Sections[i]->m_BlockLight)
				);
				memset(
					m_Sections[i]->m_BlockSkyLight,
					0xFF,
					sizeof(m_Sections[i]->m_BlockSkyLight)
				);
			}
		}
	} 
}




void cChunkData::SetMeta(const NIBBLETYPE * a_Src)
{
	ASSERT(a_Src != NULL);
	
	for (size_t i = 0; i < CHUNK_SECTION_COUNT; i++)
	{
		const size_t SegmentLength = CHUNK_SECTION_HEIGHT * 16 * 16 / 2;
		if (m_Sections[i] != NULL)
		{
			memcpy(
				&m_Sections[i]->m_BlockMeta, 
				&a_Src[i * SegmentLength],
				sizeof(NIBBLETYPE) * SegmentLength
			);
		}
		else
		{
			// j counts how many of leading zeros the buffer has
			// if j == SegmentLength then the buffer is all zeros so there is no point
			// creating the buffer.
			size_t j = 0;
			// do nothing whilst 0
			for (; (j < SegmentLength) && (a_Src[i * SegmentLength + j] == 0); j++);
			if (j != SegmentLength)
			{
				m_Sections[i] = Allocate();
				memcpy(
					&m_Sections[i]->m_BlockMeta, 
					&a_Src[i * SegmentLength],
					sizeof(BLOCKTYPE) * SegmentLength
					);
				memset(
					m_Sections[i]->m_BlockTypes,
					0x00,
					sizeof(m_Sections[i]->m_BlockTypes)
				);
				memset(
					m_Sections[i]->m_BlockLight,
					0x00,
					sizeof(m_Sections[i]->m_BlockLight)
				);
				memset(
					m_Sections[i]->m_BlockSkyLight,
					0xFF,
					sizeof(m_Sections[i]->m_BlockSkyLight)
				);
			}
		}
	} 
}




void cChunkData::SetBlockLight(const NIBBLETYPE * a_Src)
{
	if (a_Src == NULL)
	{
		return;
	}
	for (size_t i = 0; i < CHUNK_SECTION_COUNT; i++)
	{
		const size_t SegmentLength = CHUNK_SECTION_HEIGHT * 16 * 16 / 2;
		if (m_Sections[i] != NULL)
		{
			memcpy(
				&m_Sections[i]->m_BlockLight,
				&a_Src[i * SegmentLength],
				sizeof(NIBBLETYPE) * SegmentLength
			);
		}
		else
		{
			// j counts how many of leading zeros the buffer has
			// if j == SegmentLength then the buffer is all zeros so there is no point
			// creating the buffer.
			size_t j = 0;
			// do nothing whilst 0
			for (; (j < SegmentLength) && (a_Src[i * SegmentLength + j] == 0); j++);
			if (j != SegmentLength)
			{
				m_Sections[i] = Allocate();
				memcpy(
					&m_Sections[i]->m_BlockLight, 
					&a_Src[i * SegmentLength],
					sizeof(BLOCKTYPE) * SegmentLength
					);
				memset(
					m_Sections[i]->m_BlockTypes,
					0x00,
					sizeof(m_Sections[i]->m_BlockTypes)
				);
				memset(
					m_Sections[i]->m_BlockMeta,
					0x00,
					sizeof(m_Sections[i]->m_BlockMeta)
				);
				memset(
					m_Sections[i]->m_BlockSkyLight,
					0xFF,
					sizeof(m_Sections[i]->m_BlockSkyLight)
				);
			}
		}
	}
}




void cChunkData::SetSkyLight(const NIBBLETYPE * a_Src)
{
	if (a_Src == NULL)
	{
		return;
	}
	
	for (size_t i = 0; i < CHUNK_SECTION_COUNT; i++)
	{
		const size_t SegmentLength = CHUNK_SECTION_HEIGHT * 16 * 16 / 2;
		if (m_Sections[i] != NULL)
		{
			memcpy(
				&m_Sections[i]->m_BlockSkyLight,
				&a_Src[i * SegmentLength],
				sizeof(NIBBLETYPE) * SegmentLength
			);
		}
		else
		{
			// j counts how many of leading zeros the buffer has
			// if j == SegmentLength then the buffer is all zeros so there is no point
			// creating the buffer.
			size_t j = 0;
			// do nothing whilst 0
			for (; (j < SegmentLength) && (a_Src[i * SegmentLength + j]) == 0xFF; j++);
			if (j != SegmentLength)
			{
				m_Sections[i] = Allocate();
				memcpy(
					&m_Sections[i]->m_BlockSkyLight, 
					&a_Src[i * SegmentLength],
					sizeof(BLOCKTYPE) * SegmentLength
					);
				memset(
					m_Sections[i]->m_BlockTypes,
					0x00,
					sizeof(m_Sections[i]->m_BlockTypes)
				);
				memset(
					m_Sections[i]->m_BlockMeta,
					0x00,
					sizeof(m_Sections[i]->m_BlockMeta)
				);
				memset(
					m_Sections[i]->m_BlockLight,
					0x00,
					sizeof(m_Sections[i]->m_BlockLight)
				);
			}
		}
	}
}





cChunkData::sChunkSection * cChunkData::Allocate(void) const
{
	// TODO: Use an allocation pool
	return new cChunkData::sChunkSection;
}





void cChunkData::Free(cChunkData::sChunkSection * a_Section) const
{
	// TODO: Use an allocation pool
	delete a_Section;
}





void cChunkData::ZeroSection(cChunkData::sChunkSection * a_Section) const
{
	memset(
		a_Section->m_BlockTypes,
		0x00,
		sizeof(a_Section->m_BlockTypes)
	);
	memset(
		a_Section->m_BlockMeta,
		0x00,
		sizeof(a_Section->m_BlockMeta)
	);
	memset(
		a_Section->m_BlockLight,
		0x00,
		sizeof(a_Section->m_BlockLight)
	);
	memset(
		a_Section->m_BlockSkyLight,
		0xFF,
		sizeof(a_Section->m_BlockSkyLight)
	);
}




