
// GridStructGen.cpp

// Implements the cGridStructGen class representing a common base class for structure generators that place structures in a semi-random grid

#include "Globals.h"
#include "GridStructGen.h"




///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cEmptyStructure:

/** A cStructure descendant representing an empty structure.
Used when the generator descended from cGridStructGen doesn't return any structure, to keep at least the
Origin coords so that the structure isn't queried over and over again. */
class cEmptyStructure :
	public cGridStructGen::cStructure
{
	typedef cGridStructGen::cStructure super;
	
public:
	cEmptyStructure(int a_OriginX, int a_OriginZ) :
		super(a_OriginX, a_OriginZ)
	{
	}
	
protected:
	virtual void DrawIntoChunk(cChunkDesc & a_ChunkDesc) override
	{
		// Do nothing
	}
} ;





cGridStructGen::cGridStructGen(
	int a_Seed,
	int a_GridSizeX, int a_GridSizeZ,
	int a_MaxStructureSizeX, int a_MaxStructureSizeZ,
	size_t a_MaxCacheSize
) :
	m_Seed(a_Seed),
	m_GridSizeX(a_GridSizeX),
	m_GridSizeZ(a_GridSizeZ),
	m_MaxStructureSizeX(a_MaxStructureSizeX),
	m_MaxStructureSizeZ(a_MaxStructureSizeZ),
	m_MaxCacheSize(a_MaxCacheSize)
{
	size_t NumStructuresPerQuery = (size_t)((m_MaxStructureSizeX / m_GridSizeX + 1) * (m_MaxStructureSizeZ / m_GridSizeZ + 1));
	if (NumStructuresPerQuery > m_MaxCacheSize)
	{
		m_MaxCacheSize = NumStructuresPerQuery * 4;
		LOGINFO(
			"cGridStructGen: The cache size is too small (%u), increasing the cache size to %u to avoid inefficiency.",
			(unsigned)a_MaxCacheSize, (unsigned)m_MaxCacheSize
		);
	}
}





void cGridStructGen::GetStructuresForChunk(int a_ChunkX, int a_ChunkZ, cStructurePtrs & a_Structures)
{
	// Calculate the min and max grid coords of the structures to be returned:
	int MinBlockX = a_ChunkX * cChunkDef::Width - m_MaxStructureSizeX;
	int MinBlockZ = a_ChunkZ * cChunkDef::Width - m_MaxStructureSizeZ;
	int MaxBlockX = a_ChunkX * cChunkDef::Width + m_MaxStructureSizeX + cChunkDef::Width - 1;
	int MaxBlockZ = a_ChunkZ * cChunkDef::Width + m_MaxStructureSizeZ + cChunkDef::Width - 1;
	int MinGridX = MinBlockX / m_GridSizeX;
	int MinGridZ = MinBlockZ / m_GridSizeZ;
	int MaxGridX = (MaxBlockX + m_GridSizeX - 1) / m_GridSizeX;
	int MaxGridZ = (MaxBlockZ + m_GridSizeZ - 1) / m_GridSizeZ;
	int MinX = MinGridX * m_GridSizeX;
	int MaxX = MaxGridX * m_GridSizeX;
	int MinZ = MinGridZ * m_GridSizeZ;
	int MaxZ = MaxGridZ * m_GridSizeZ;

	// Walk the cache, move each structure that we want into a_Structures:
	for (cStructurePtrs::iterator itr = m_Cache.begin(), end = m_Cache.end(); itr != end;)
	{
		if (
			((*itr)->m_OriginX >= MinX) && ((*itr)->m_OriginX < MaxX) &&
			((*itr)->m_OriginZ >= MinZ) && ((*itr)->m_OriginZ < MaxZ)
		)
		{
			// want
			a_Structures.push_back(*itr);
			itr = m_Cache.erase(itr);
		}
		else
		{
			// don't want
			++itr;
		}
	}  // for itr - m_Cache[]

	// Create those structures that haven't been in the cache:
	for (int x = MinGridX; x < MaxGridX; x++)
	{
		int OriginX = x * m_GridSizeX;
		for (int z = MinGridZ; z < MaxGridZ; z++)
		{
			int OriginZ = z * m_GridSizeZ;
			bool Found = false;
			for (cStructurePtrs::const_iterator itr = a_Structures.begin(), end = a_Structures.end(); itr != end; ++itr)
			{
				if (((*itr)->m_OriginX == OriginX) && ((*itr)->m_OriginZ == OriginZ))
				{
					Found = true;
					break;
				}
			}  // for itr - a_Structures[]
			if (!Found)
			{
				cStructurePtr Structure = CreateStructure(OriginX, OriginZ);
				if (Structure.get() == NULL)
				{
					Structure.reset(new cEmptyStructure(OriginX, OriginZ));
				}
				a_Structures.push_back(Structure);
			}
		}  // for z
	}  // for x

	// Copy a_Forts into m_Cache to the beginning:
	cStructurePtrs StructuresCopy (a_Structures);
	m_Cache.splice(m_Cache.begin(), StructuresCopy, StructuresCopy.begin(), StructuresCopy.end());

	// Trim the cache if it's too long:
	size_t CacheSize = 0;
	for (cStructurePtrs::iterator itr = m_Cache.begin(), end = m_Cache.end(); itr != end; ++itr)
	{
		CacheSize += (*itr)->GetCacheCost();
		if (CacheSize > m_MaxCacheSize)
		{
			// Erase all items from this one till the cache end
			m_Cache.erase(itr, m_Cache.end());
			break;
		}
	}
}





void cGridStructGen::GenFinish(cChunkDesc & a_ChunkDesc)
{
	int ChunkX = a_ChunkDesc.GetChunkX();
	int ChunkZ = a_ChunkDesc.GetChunkZ();
	cStructurePtrs Structures;
	GetStructuresForChunk(ChunkX, ChunkZ, Structures);
	for (cStructurePtrs::const_iterator itr = Structures.begin(); itr != Structures.end(); ++itr)
	{
		(*itr)->DrawIntoChunk(a_ChunkDesc);
	}  // for itr - Structures[]
}





