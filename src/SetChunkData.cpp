
// SetChunkData.cpp

// Implements the cSetChunkData class used for sending loaded / generated chunk

#include "Globals.h"
#include "SetChunkData.h"
#include "BlockEntities/BlockEntity.h"





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
	cEntityList & a_Entities,
	cBlockEntityList & a_BlockEntities,
	bool a_ShouldMarkDirty
) :
	m_ChunkX(a_ChunkX),
	m_ChunkZ(a_ChunkZ),
	m_ShouldMarkDirty(a_ShouldMarkDirty)
{
	// Check the params' validity:
	ASSERT(a_BlockTypes != NULL);
	ASSERT(a_BlockMetas != NULL);

	// Copy block types and metas:
	memcpy(m_BlockTypes, a_BlockTypes, sizeof(cChunkDef::BlockTypes));
	memcpy(m_BlockMetas, a_BlockMetas, sizeof(cChunkDef::BlockNibbles));
	
	// Copy lights, if both given:
	if ((a_BlockLight != NULL) && (a_SkyLight != NULL))
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
	if (a_HeightMap != NULL)
	{
		memcpy(m_HeightMap, a_HeightMap, sizeof(m_HeightMap));
		m_IsHeightMapValid = true;
	}
	else
	{
		m_IsHeightMapValid = false;
	}
	
	// Copy biomes, if available:
	if (a_Biomes != NULL)
	{
		memcpy(m_Biomes, a_Biomes, sizeof(m_Biomes));
		m_AreBiomesValid = true;
	}
	else
	{
		m_AreBiomesValid = false;
	}
	
	// Move entities and blockentities:
	std::swap(m_Entities,      a_Entities);
	std::swap(m_BlockEntities, a_BlockEntities);
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
					m_HeightMap[x + z * cChunkDef::Width] = (HEIGHTTYPE)y;
					break;
				}
			}  // for y
		}  // for z
	}  // for x
	m_IsHeightMapValid = true;
}






void cSetChunkData::RemoveInvalidBlockEntities(void)
{
	for (cBlockEntityList::iterator itr = m_BlockEntities.begin(); itr != m_BlockEntities.end();)
	{
		BLOCKTYPE EntityBlockType = (*itr)->GetBlockType();
		BLOCKTYPE WorldBlockType = cChunkDef::GetBlock(m_BlockTypes, (*itr)->GetRelX(), (*itr)->GetPosY(), (*itr)->GetRelZ());
		if (EntityBlockType != WorldBlockType)
		{
			// Bad blocktype, remove the block entity:
			LOGD("Block entity blocktype mismatch at {%d, %d, %d}: entity for blocktype %s(%d) in block %s(%d). Deleting the block entity.",
				(*itr)->GetPosX(), (*itr)->GetPosY(), (*itr)->GetPosZ(),
				ItemTypeToString(EntityBlockType).c_str(), EntityBlockType,
				ItemTypeToString(WorldBlockType).c_str(),  WorldBlockType
			);
			cBlockEntityList::iterator itr2 = itr;
			itr2++;
			delete *itr;
			m_BlockEntities.erase(itr);
			itr = itr2;
		}
		else
		{
			// Good blocktype, keep the block entity:
			++itr;
		}
	}  // for itr - m_BlockEntities[]
}




