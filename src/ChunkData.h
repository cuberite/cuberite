
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

	cChunkData()
	#if __cplusplus < 201103L
		// auto_ptr style interface for memory management
		: IsOwner(true)
	#endif
	{
		memset(m_Sections, 0, sizeof(m_Sections));
	}
	~cChunkData()
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
		cChunkData(const cChunkData& other) :
		IsOwner(true)
		{
			for (size_t i = 0; i < CHUNK_SECTION_COUNT; i++)
			{
				m_Sections[i] = other.m_Sections[i];
			}
			other.IsOwner = false;
		}

		cChunkData& operator=(const cChunkData& other)
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
		cChunkData(cChunkData&& other)
		{
			for (size_t i = 0; i < CHUNK_SECTION_COUNT; i++)
			{
				m_Sections[i] = other.m_Sections[i];
				other.m_Sections[i] = NULL;
			}
		}
	
		cChunkData& operator=(cChunkData&& other)
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

	BLOCKTYPE GetBlock(int a_X, int a_Y, int a_Z) const
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
	
	void SetBlock(int a_RelX, int a_RelY, int a_RelZ, BLOCKTYPE a_Block)
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

	NIBBLETYPE GetMeta(int a_RelX, int a_RelY, int a_RelZ) const
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
	
	bool SetMeta(int a_RelX, int a_RelY, int a_RelZ, NIBBLETYPE a_Nibble)
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
	
	NIBBLETYPE GetBlockLight(int a_RelX, int a_RelY, int a_RelZ) const
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
	
	NIBBLETYPE GetSkyLight(int a_RelX, int a_RelY, int a_RelZ) const
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
	
	cChunkData Copy() const;
	void CopyBlocks   (BLOCKTYPE * a_dest, size_t a_Idx = 0, size_t length = cChunkDef::NumBlocks)  const;
	void CopyMeta     (NIBBLETYPE * a_dest) const;
	void CopyBlockLight    (NIBBLETYPE * a_dest) const;
	void CopySkyLight (NIBBLETYPE * a_dest) const;
	
	void SetBlocks    (const BLOCKTYPE * a_src);
	void SetMeta      (const NIBBLETYPE * a_src);
	void SetBlockLight     (const NIBBLETYPE * a_src);
	void SetSkyLight  (const NIBBLETYPE * a_src);
	
private:

	static const size_t CHUNK_SECTION_HEIGHT = 16;
	static const size_t CHUNK_SECTION_COUNT = (256 / CHUNK_SECTION_HEIGHT);

	#if __cplusplus < 201103L
	// auto_ptr style interface for memory management
	mutable bool IsOwner;
	#endif
	
	struct sChunkSection {
		BLOCKTYPE  m_BlockTypes   [CHUNK_SECTION_HEIGHT * 16 * 16]    ;
		NIBBLETYPE m_BlockMeta    [CHUNK_SECTION_HEIGHT * 16 * 16 / 2];
		NIBBLETYPE m_BlockLight   [CHUNK_SECTION_HEIGHT * 16 * 16 / 2];
		NIBBLETYPE m_BlockSkyLight[CHUNK_SECTION_HEIGHT * 16 * 16 / 2];
	};
	
	sChunkSection *m_Sections[CHUNK_SECTION_COUNT];
	
	sChunkSection * Allocate() const;
	void Free(sChunkSection * ptr) const;
	
	void ZeroSection(sChunkSection * ptr) const;
};






