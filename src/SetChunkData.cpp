
// SetChunkData.cpp

// Implements the cSetChunkData class used for sending loaded / generated chunk

#include "Globals.h"
#include "SetChunkData.h"
#include "BlockEntities/BlockEntity.h"
#include "Entities/Entity.h"

namespace
{
struct sMemCallbacks:
	cAllocationPool<cChunkData::sChunkSection>::cStarvationCallbacks
{
	virtual void OnStartUsingReserve() override {}
	virtual void OnEndUsingReserve() override {}
	virtual void OnOutOfReserve() override {}
};
}  // namespace (anonymous)





cSetChunkData::cSetChunkData(int a_ChunkX, int a_ChunkZ, bool a_ShouldMarkDirty) :
	m_ChunkX(a_ChunkX),
	m_ChunkZ(a_ChunkZ),
	m_Pool(std::make_unique<sMemCallbacks>(), 0u, cChunkData::NumSections),
	m_ChunkData(m_Pool),
	m_IsLightValid(false),
	m_IsHeightMapValid(false),
	m_AreBiomesValid(false),
	m_ShouldMarkDirty(a_ShouldMarkDirty)
{
}





cSetChunkData::cSetChunkData(
	int a_ChunkX, int a_ChunkZ,
	const BLOCKTYPE * a_BlockTypes,
	const NIBBLETYPE * a_BlockMetas,
	const NIBBLETYPE * a_BlockLight,
	const NIBBLETYPE * a_SkyLight,
	const cChunkDef::HeightMap * a_HeightMap,
	const cChunkDef::BiomeMap * a_Biomes,
	cEntityList && a_Entities,
	cBlockEntities && a_BlockEntities,
	bool a_ShouldMarkDirty
) :
	cSetChunkData(a_ChunkX, a_ChunkZ, a_ShouldMarkDirty)
{
	// Check the params' validity:
	ASSERT(a_BlockTypes != nullptr);
	ASSERT(a_BlockMetas != nullptr);

	// Copy block types and metas:
	m_ChunkData.SetBlockTypes(a_BlockTypes);
	m_ChunkData.SetMetas(a_BlockMetas);

	// Copy lights, if both given:
	if ((a_BlockLight != nullptr) && (a_SkyLight != nullptr))
	{
		m_ChunkData.SetBlockLight(a_BlockLight);
		m_ChunkData.SetSkyLight(a_SkyLight);
		m_IsLightValid = true;
	}

	// Copy the heightmap, if available:
	if (a_HeightMap != nullptr)
	{
		memcpy(m_HeightMap, a_HeightMap, sizeof(m_HeightMap));
		m_IsHeightMapValid = true;
	}

	// Copy biomes, if available:
	if (a_Biomes != nullptr)
	{
		memcpy(m_Biomes, a_Biomes, sizeof(m_Biomes));
		m_AreBiomesValid = true;
	}

	// Move entities and blockentities:
	m_Entities = std::move(a_Entities);
	m_BlockEntities = std::move(a_BlockEntities);
}





void cSetChunkData::CalculateHeightMap(void)
{
	// Find the heighest present section in the chunk
	size_t MaxSection = 0;
	for (size_t i = cChunkData::NumSections - 1; i != 0; --i)
	{
		if (m_ChunkData.GetSection(i) != nullptr)
		{
			MaxSection = i;
			break;
		}
	}
	const int MaxHeight = static_cast<int>(MaxSection + 1) * cChunkData::SectionHeight - 1;

	for (int x = 0; x < cChunkDef::Width; x++)
	{
		for (int z = 0; z < cChunkDef::Width; z++)
		{
			for (int y = MaxHeight; y > -1; y--)
			{
				if (m_ChunkData.GetBlock({x, y, z}) != E_BLOCK_AIR)
				{
					m_HeightMap[x + z * cChunkDef::Width] = static_cast<HEIGHTTYPE>(y);
					break;
				}
			}  // for y
		}  // for z
	}  // for x
	m_IsHeightMapValid = true;
}





void cSetChunkData::RemoveInvalidBlockEntities(void)
{
	for (cBlockEntities::iterator itr = m_BlockEntities.begin(); itr != m_BlockEntities.end();)
	{
		auto & BlockEntity = itr->second;
		BLOCKTYPE EntityBlockType = BlockEntity->GetBlockType();
		BLOCKTYPE WorldBlockType = m_ChunkData.GetBlock({BlockEntity->GetRelX(), BlockEntity->GetPosY(), BlockEntity->GetRelZ()});
		if (EntityBlockType != WorldBlockType)
		{
			// Bad blocktype, remove the block entity:
			FLOGD("Block entity blocktype mismatch at {0}: entity for blocktype {1}({2}) in block {3}({4}). Deleting the block entity.",
				BlockEntity->GetPos(),
				ItemTypeToString(EntityBlockType), EntityBlockType,
				ItemTypeToString(WorldBlockType),  WorldBlockType
			);
			itr = m_BlockEntities.erase(itr);
		}
		else
		{
			// Good blocktype, keep the block entity:
			++itr;
		}
	}  // for itr - m_BlockEntities[]
}




