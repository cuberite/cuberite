
// GridStructGen.cpp

// Implements the cGridStructGen class representing a common base class for structure generators that place structures in a semi-random grid

#include "Globals.h"
#include "GridStructGen.h"





////////////////////////////////////////////////////////////////////////////////
// cEmptyStructure:

/** A cStructure descendant representing an empty structure.
Used when the generator descended from cGridStructGen doesn't return any structure, to keep at least the
Origin coords so that the structure isn't queried over and over again. */
class cEmptyStructure:
	public cGridStructGen::cStructure
{
	using Super = cGridStructGen::cStructure;

public:

	cEmptyStructure(int a_GridX, int a_GridZ, int a_OriginX, int a_OriginZ) :
		Super(a_GridX, a_GridZ, a_OriginX, a_OriginZ)
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
	int a_MaxOffsetX, int a_MaxOffsetZ,
	int a_MaxStructureSizeX, int a_MaxStructureSizeZ,
	size_t a_MaxCacheSize
) :
	m_Seed(a_Seed),
	m_Noise(a_Seed),
	m_GridSizeX(a_GridSizeX),
	m_GridSizeZ(a_GridSizeZ),
	m_MaxOffsetX(a_MaxOffsetX),
	m_MaxOffsetZ(a_MaxOffsetZ),
	m_MaxStructureSizeX(a_MaxStructureSizeX),
	m_MaxStructureSizeZ(a_MaxStructureSizeZ),
	m_MaxCacheSize(a_MaxCacheSize)
{
	if (m_GridSizeX == 0)
	{
		LOG("Grid Size cannot be zero, setting to 1");
		m_GridSizeX = 1;
	}
	if (m_GridSizeZ == 0)
	{
		LOG("Grid Size cannot be zero, setting to 1");
		m_GridSizeZ = 1;
	}
	size_t NumStructuresPerQuery = static_cast<size_t>(((m_MaxStructureSizeX + m_MaxOffsetX) / m_GridSizeX + 1) * ((m_MaxStructureSizeZ + m_MaxOffsetZ) / m_GridSizeZ + 1));
	if (NumStructuresPerQuery > m_MaxCacheSize)
	{
		m_MaxCacheSize = NumStructuresPerQuery * 4;
		LOGINFO(
			"cGridStructGen: The cache size is too small (%u), increasing the cache size to %u to avoid inefficiency.",
			static_cast<unsigned>(a_MaxCacheSize), static_cast<unsigned>(m_MaxCacheSize)
		);
	}
}





cGridStructGen::cGridStructGen(int a_Seed):
	m_BaseSeed(a_Seed),
	m_Seed(a_Seed),
	m_Noise(a_Seed),
	m_GridSizeX(256),
	m_GridSizeZ(256),
	m_MaxOffsetX(128),
	m_MaxOffsetZ(128),
	m_MaxStructureSizeX(128),
	m_MaxStructureSizeZ(128),
	m_MaxCacheSize(256)
{
}





void cGridStructGen::SetGeneratorParams(const AStringMap & a_GeneratorParams)
{
	ASSERT(m_Cache.empty());  // No changing the params after chunks are generated
	m_GridSizeX         = GetStringMapInteger<int>   (a_GeneratorParams, "GridSizeX",         m_GridSizeX);
	m_GridSizeZ         = GetStringMapInteger<int>   (a_GeneratorParams, "GridSizeZ",         m_GridSizeZ);
	m_MaxOffsetX        = GetStringMapInteger<int>   (a_GeneratorParams, "MaxOffsetX",        m_MaxOffsetX);
	m_MaxOffsetZ        = GetStringMapInteger<int>   (a_GeneratorParams, "MaxOffsetZ",        m_MaxOffsetZ);
	m_MaxStructureSizeX = GetStringMapInteger<int>   (a_GeneratorParams, "MaxStructureSizeX", m_MaxStructureSizeX);
	m_MaxStructureSizeZ = GetStringMapInteger<int>   (a_GeneratorParams, "MaxStructureSizeZ", m_MaxStructureSizeZ);
	m_MaxCacheSize      = GetStringMapInteger<size_t>(a_GeneratorParams, "MaxCacheSize",      m_MaxCacheSize);

	// Silently fix out-of-range parameters:
	if (m_MaxOffsetX < 1)
	{
		m_MaxOffsetX = 1;
	}
	if (m_MaxOffsetZ < 1)
	{
		m_MaxOffsetZ = 1;
	}

	// Set the seed based on the seed offset from the parameters:
	auto seedOffset = GetStringMapInteger<int>(a_GeneratorParams, "SeedOffset", 0);
	m_Seed = m_BaseSeed + seedOffset;
	m_Noise.SetSeed(m_Seed);
}





void cGridStructGen::GetStructuresForChunk(int a_ChunkX, int a_ChunkZ, cStructurePtrs & a_Structures)
{
	// Calculate the min and max grid coords of the structures to be returned:
	int MinBlockX = a_ChunkX * cChunkDef::Width - m_MaxStructureSizeX - m_MaxOffsetX;
	int MinBlockZ = a_ChunkZ * cChunkDef::Width - m_MaxStructureSizeZ - m_MaxOffsetZ;
	int MaxBlockX = a_ChunkX * cChunkDef::Width + m_MaxStructureSizeX + m_MaxOffsetX + cChunkDef::Width - 1;
	int MaxBlockZ = a_ChunkZ * cChunkDef::Width + m_MaxStructureSizeZ + m_MaxOffsetZ + cChunkDef::Width - 1;
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
			((*itr)->m_GridX >= MinX) && ((*itr)->m_GridX < MaxX) &&
			((*itr)->m_GridZ >= MinZ) && ((*itr)->m_GridZ < MaxZ)
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
		int GridX = x * m_GridSizeX;
		for (int z = MinGridZ; z < MaxGridZ; z++)
		{
			int GridZ = z * m_GridSizeZ;
			bool Found = false;
			for (cStructurePtrs::const_iterator itr = a_Structures.begin(), end = a_Structures.end(); itr != end; ++itr)
			{
				if (((*itr)->m_GridX == GridX) && ((*itr)->m_GridZ == GridZ))
				{
					Found = true;
					break;
				}
			}  // for itr - a_Structures[]
			if (!Found)
			{
				int OriginX = GridX + ((m_Noise.IntNoise2DInt(GridX + 3, GridZ + 5) / 7) % (m_MaxOffsetX * 2)) - m_MaxOffsetX;
				int OriginZ = GridZ + ((m_Noise.IntNoise2DInt(GridX + 5, GridZ + 3) / 7) % (m_MaxOffsetZ * 2)) - m_MaxOffsetZ;
				cStructurePtr Structure = CreateStructure(GridX, GridZ, OriginX, OriginZ);
				if (Structure.get() == nullptr)
				{
					Structure.reset(new cEmptyStructure(GridX, GridZ, OriginX, OriginZ));
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





