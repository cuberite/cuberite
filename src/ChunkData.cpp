
#include "Globals.h"
#include "ChunkData.h"

cChunkData::cChunkData()
#if __cplusplus < 201103L
	// auto_ptr style interface for memory management
	: IsOwner(true)
#endif
{
	memset(m_Sections, 0, sizeof(m_Sections));
}


cChunkData::~cChunkData()
{
	#if __cplusplus < 201103L
		// auto_ptr style interface for memory management
		if (!IsOwner)
		{
			return;
		}
	#endif
	for (size_t i = 0; i < CHUNK_SECTION_COUNT; i++)
	{
		if (m_Sections[i] == NULL) Free(m_Sections[i]);;
	}
}

#if __cplusplus < 201103L
	// auto_ptr style interface for memory management
	cChunkData::cChunkData(const cChunkData& other) :
	IsOwner(true)
	{
		for (size_t i = 0; i < CHUNK_SECTION_COUNT; i++)
		{
			m_Sections[i] = other.m_Sections[i];
		}
		other.IsOwner = false;
	}

	cChunkData& cChunkData::operator=(const cChunkData& other)
	{
		if (&other != this)
		{
			if (IsOwner)
			{
				for (size_t i = 0; i < CHUNK_SECTION_COUNT; i++)
				{
					if (m_Sections[i]) Free(m_Sections[i]);;
				}
			}
			IsOwner = true;
			for (size_t i = 0; i < CHUNK_SECTION_COUNT; i++)
			{
				m_Sections[i] = other.m_Sections[i];
			}
			other.IsOwner = false;
		}
		return *this;
	
	}
#else
	// unique_ptr style interface for memory management
	cChunkData::cChunkData(cChunkData&& other)
	{
		for (size_t i = 0; i < CHUNK_SECTION_COUNT; i++)
		{
			m_Sections[i] = other.m_Sections[i];
			other.m_Sections[i] = NULL;
		}
	}

	cChunkData& cChunkData::operator=(cChunkData&& other)
	{
		if (&other != this)
		{
			for (size_t i = 0; i < CHUNK_SECTION_COUNT; i++)
			{
				Free(m_Sections[i]);;
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

cChunkData cChunkData::Copy() const
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





void cChunkData::CopyBlocks   (BLOCKTYPE * a_dest, size_t a_Idx, size_t length)  const
{
	for (size_t i = 0; i < CHUNK_SECTION_COUNT; i++)
	{
		const size_t segment_length =  CHUNK_SECTION_HEIGHT * 16 * 16;
		if (a_Idx > 0)
		{
			a_Idx = std::max(a_Idx - length, (size_t) 0);
		}
		if (a_Idx == 0) 
		{
			size_t tocopy = std::min(segment_length, length);
			length -= tocopy;
			if (m_Sections[i] != NULL)
			{
				memcpy(
					&a_dest[i * segment_length],
					&m_Sections[i]->m_BlockTypes, 
					sizeof(BLOCKTYPE) * tocopy
				);
			}
			else
			{
				memset(
					&a_dest[i * segment_length],
					0,
					sizeof(BLOCKTYPE) * tocopy
				);
			}
		}
	}
}





void cChunkData::CopyMeta(NIBBLETYPE * a_dest) const
{
	for (size_t i = 0; i < CHUNK_SECTION_COUNT; i++)
	{
		const size_t segment_length = CHUNK_SECTION_HEIGHT * 16 * 16 / 2;
		if (m_Sections[i] != NULL)
		{
			memcpy(
				&a_dest[i * segment_length], 
				&m_Sections[i]->m_BlockMeta, 
				sizeof(NIBBLETYPE) * segment_length
			);
		}
		else
		{
			memset(
				&a_dest[i * segment_length],
				0,
				sizeof(BLOCKTYPE) * segment_length
			);
		}
	}
}





void cChunkData::CopyBlockLight(NIBBLETYPE * a_dest) const
{
	for (size_t i = 0; i < CHUNK_SECTION_COUNT; i++)
	{
		const size_t segment_length = CHUNK_SECTION_HEIGHT * 16 * 16 / 2;
		if (m_Sections[i] != NULL)
		{
			memcpy(
				&a_dest[i * segment_length],
				&m_Sections[i]->m_BlockLight,
				sizeof(NIBBLETYPE) * segment_length
			);
		}
		else
		{
			memset(
				&a_dest[i * segment_length],
				0,
				sizeof(BLOCKTYPE) * segment_length
			);
		}
	}
}





void cChunkData::CopySkyLight(NIBBLETYPE * a_dest) const
{
	for (size_t i = 0; i < CHUNK_SECTION_COUNT; i++)
	{
		const size_t segment_length = CHUNK_SECTION_HEIGHT * 16 * 16 / 2;
		if (m_Sections[i] != NULL)
		{
			memcpy(
				&a_dest[i * segment_length],
				&m_Sections[i]->m_BlockSkyLight,
				sizeof(NIBBLETYPE) * segment_length
			);
		}
		else
		{
			memset(
				&a_dest[i * segment_length],
				0xFF,
				sizeof(BLOCKTYPE) * segment_length
			);
		}
	}
}





void cChunkData::SetBlocks(const BLOCKTYPE * a_src)
{
	for (size_t i = 0; i < CHUNK_SECTION_COUNT; i++)
	{
		const size_t segment_length = CHUNK_SECTION_HEIGHT * 16 * 16;
		if (m_Sections[i] != NULL)
		{
			memcpy(
				&m_Sections[i]->m_BlockTypes,
				&a_src[i * segment_length],
				sizeof(BLOCKTYPE) * segment_length
			);
		}
		else
		{
			// j counts how many of leading zeros the buffer has
			// if j == segment_length then the buffer is all zeros so there is no point
			// creating the buffer.
			size_t j = 0;
			// do nothing whilst 0
			for (; j < segment_length && a_src[i * segment_length + j] == 0; j++);
			if (j != segment_length)
			{
				m_Sections[i] = Allocate();
				memcpy(
					&m_Sections[i]->m_BlockTypes, 
					&a_src[i * segment_length],
					sizeof(BLOCKTYPE) * segment_length
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




void cChunkData::SetMeta(const NIBBLETYPE * a_src)
{
	for (size_t i = 0; i < CHUNK_SECTION_COUNT; i++)
	{
		const size_t segment_length = CHUNK_SECTION_HEIGHT * 16 * 16 / 2;
		if (m_Sections[i] != NULL)
		{
			memcpy(
				&m_Sections[i]->m_BlockMeta, 
				&a_src[i * segment_length],
				sizeof(NIBBLETYPE) * segment_length
			);
		}
		else
		{
			// j counts how many of leading zeros the buffer has
			// if j == segment_length then the buffer is all zeros so there is no point
			// creating the buffer.
			size_t j = 0;
			// do nothing whilst 0
			for (; j < segment_length && a_src[i * segment_length + j] == 0; j++);
			if (j != segment_length)
			{
				m_Sections[i] = Allocate();
				memcpy(
					&m_Sections[i]->m_BlockMeta, 
					&a_src[i * segment_length],
					sizeof(BLOCKTYPE) * segment_length
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




void cChunkData::SetBlockLight(const NIBBLETYPE * a_src)
{
	if (!a_src) return;
	for (size_t i = 0; i < CHUNK_SECTION_COUNT; i++)
	{
		const size_t segment_length = CHUNK_SECTION_HEIGHT * 16 * 16 / 2;
		if (m_Sections[i] != NULL)
		{
			memcpy(
				&m_Sections[i]->m_BlockLight,
				&a_src[i * segment_length],
				sizeof(NIBBLETYPE) * segment_length
			);
		}
		else
		{
			// j counts how many of leading zeros the buffer has
			// if j == segment_length then the buffer is all zeros so there is no point
			// creating the buffer.
			size_t j = 0;
			// do nothing whilst 0
			for (; j < segment_length && a_src[i * segment_length + j] == 0; j++);
			if (j != segment_length)
			{
				m_Sections[i] = Allocate();
				memcpy(
					&m_Sections[i]->m_BlockLight, 
					&a_src[i * segment_length],
					sizeof(BLOCKTYPE) * segment_length
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




void cChunkData::SetSkyLight  (const NIBBLETYPE * a_src)
{
	if (!a_src) return;
	for (size_t i = 0; i < CHUNK_SECTION_COUNT; i++)
	{
		const size_t segment_length = CHUNK_SECTION_HEIGHT * 16 * 16 / 2;
		if (m_Sections[i] != NULL)
		{
			memcpy(
				&m_Sections[i]->m_BlockSkyLight,
				&a_src[i * segment_length],
				sizeof(NIBBLETYPE) * segment_length
			);
		}
		else
		{
			// j counts how many of leading zeros the buffer has
			// if j == segment_length then the buffer is all zeros so there is no point
			// creating the buffer.
			size_t j = 0;
			// do nothing whilst 0
			for (; j < segment_length && a_src[i * segment_length + j] == 0xFF; j++);
			if (j != segment_length)
			{
				m_Sections[i] = Allocate();
				memcpy(
					&m_Sections[i]->m_BlockSkyLight, 
					&a_src[i * segment_length],
					sizeof(BLOCKTYPE) * segment_length
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





cChunkData::sChunkSection * cChunkData::Allocate() const
{
	// TODO: use a allocation pool
	return new cChunkData::sChunkSection;
}



void cChunkData::Free(cChunkData::sChunkSection * ptr) const
{
	delete ptr;
}



void cChunkData::ZeroSection(cChunkData::sChunkSection * ptr) const
{
	memset(
		ptr->m_BlockTypes,
		0x00,
		sizeof(ptr->m_BlockTypes)
	);
	memset(
		ptr->m_BlockMeta,
		0x00,
		sizeof(ptr->m_BlockMeta)
	);
	memset(
		ptr->m_BlockLight,
		0x00,
		sizeof(ptr->m_BlockLight)
	);
	memset(
		ptr->m_BlockSkyLight,
		0xFF,
		sizeof(ptr->m_BlockSkyLight)
	);
}




