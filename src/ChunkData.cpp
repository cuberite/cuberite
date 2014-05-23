
#include "Globals.h"
#include "ChunkData.h"

cChunkData cChunkData::Copy() const
{
	cChunkData copy;
	for (int i = 0; i < CHUNK_SECTION_NUM; i++)
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
	for (size_t i = 0; i < CHUNK_SECTION_NUM; i++)
	{
		const size_t segment_length =  CHUNK_SECTION_HEIGHT * 16 * 16;
		if (a_Idx > 0) a_Idx = a_Idx > length ? a_Idx - length : 0;
		if (a_Idx == 0) 
		{
			size_t tocopy = length > segment_length ? segment_length : length;
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
	for (size_t i = 0; i < CHUNK_SECTION_NUM; i++)
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





void cChunkData::CopyLight(NIBBLETYPE * a_dest) const
{
	for (size_t i = 0; i < CHUNK_SECTION_NUM; i++)
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
	for (size_t i = 0; i < CHUNK_SECTION_NUM; i++)
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
	for (size_t i = 0; i < CHUNK_SECTION_NUM; i++)
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
			else
			{
				Free(m_Sections[i]);
				m_Sections[i] = 0;
			}
		}
	} 
}




void cChunkData::SetMeta(const NIBBLETYPE * a_src)
{
	for (size_t i = 0; i < CHUNK_SECTION_NUM; i++)
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
			else
			{
				Free(m_Sections[i]);
				m_Sections[i] = 0;
			}
		}
	} 
}




void cChunkData::SetLight(const NIBBLETYPE * a_src)
{
	if (!a_src) return;
	for (size_t i = 0; i < CHUNK_SECTION_NUM; i++)
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
			else
			{
				Free(m_Sections[i]);
				m_Sections[i] = 0;
			}
		}
	}
}




void cChunkData::SetSkyLight  (const NIBBLETYPE * a_src)
{
	if (!a_src) return;
	for (size_t i = 0; i < CHUNK_SECTION_NUM; i++)
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
			else
			{
				Free(m_Sections[i]);
				m_Sections[i] = 0;
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




