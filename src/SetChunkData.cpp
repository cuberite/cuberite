
// SetChunkData.cpp

// Implements the cSetChunkData class used for sending loaded / generated chunk

#include "Globals.h"
#include "SetChunkData.h"
#include "BlockEntities/BlockEntity.h"
#include "Entities/Entity.h"





cSetChunkData::cSetChunkData(int a_ChunkX, int a_ChunkZ, bool a_ShouldMarkDirty) :
	m_ChunkX(a_ChunkX),
	m_ChunkZ(a_ChunkZ),
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
	m_ChunkX(a_ChunkX),
	m_ChunkZ(a_ChunkZ),
	m_ShouldMarkDirty(a_ShouldMarkDirty)
{
	// Check the params' validity:
	ASSERT(a_BlockTypes != nullptr);
	ASSERT(a_BlockMetas != nullptr);

	// Copy block types and metas:
	memcpy(m_BlockTypes, a_BlockTypes, sizeof(cChunkDef::BlockTypes));
	memcpy(m_BlockMetas, a_BlockMetas, sizeof(cChunkDef::BlockNibbles));

	// Copy lights, if both given:
	if ((a_BlockLight != nullptr) && (a_SkyLight != nullptr))
	{
		memcpy(m_BlockLight, a_BlockLight, sizeof(m_BlockLight));
		memcpy(m_SkyLight,   a_SkyLight,   sizeof(m_SkyLight));
		m_IsLightValid = true;
	}
	else
	{
		m_IsLightValid = false;
	}

	// Copy the heightmap, if available:
	if (a_HeightMap != nullptr)
	{
		memcpy(m_HeightMap, a_HeightMap, sizeof(m_HeightMap));
		m_IsHeightMapValid = true;
	}
	else
	{
		m_IsHeightMapValid = false;
	}

	// Copy biomes, if available:
	if (a_Biomes != nullptr)
	{
		memcpy(m_Biomes, a_Biomes, sizeof(m_Biomes));
		m_AreBiomesValid = true;
	}
	else
	{
		m_AreBiomesValid = false;
	}

	// Move entities and blockentities:
	m_Entities = std::move(a_Entities);
	m_BlockEntities = std::move(a_BlockEntities);
}





void cSetChunkData::CalculateHeightMap(void)
{
	for (int x = 0; x < cChunkDef::Width; x++)
	{
		for (int z = 0; z < cChunkDef::Width; z++)
		{
			for (int y = cChunkDef::Height - 1; y > -1; y--)
			{
				int index = cChunkDef::MakeIndexNoCheck(x, y, z);
				if (m_BlockTypes[index] != E_BLOCK_AIR)
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
		cBlockEntity * BlockEntity = itr->second;
		BLOCKTYPE EntityBlockType = BlockEntity->GetBlockType();
		BLOCKTYPE WorldBlockType = cChunkDef::GetBlock(m_BlockTypes, BlockEntity->GetRelX(), BlockEntity->GetPosY(), BlockEntity->GetRelZ());
		if (EntityBlockType != WorldBlockType)
		{
			// Bad blocktype, remove the block entity:
			LOGD("Block entity blocktype mismatch at {%d, %d, %d}: entity for blocktype %s(%d) in block %s(%d). Deleting the block entity.",
				BlockEntity->GetPosX(), BlockEntity->GetPosY(), BlockEntity->GetPosZ(),
				ItemTypeToString(EntityBlockType).c_str(), EntityBlockType,
				ItemTypeToString(WorldBlockType).c_str(),  WorldBlockType
			);
			delete BlockEntity;
			itr = m_BlockEntities.erase(itr);
		}
		else
		{
			// Good blocktype, keep the block entity:
			++itr;
		}
	}  // for itr - m_BlockEntities[]
}




