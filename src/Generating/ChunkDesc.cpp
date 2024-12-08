
// ChunkDesc.cpp

// Implements the cChunkDesc class representing the chunk description used while generating a chunk. This class is also exported to Lua for HOOK_CHUNK_GENERATING.

#include "Globals.h"
#include "ChunkDesc.h"
#include "../Blocks/BlockAir.h"
#include "../Noise/Noise.h"
#include "../BlockEntities/BlockEntity.h"
#include "../Entities/Entity.h"
#include "BlockInfo.h"





cChunkDesc::cChunkDesc(cChunkCoords a_Coords) :
	m_Coords(a_Coords),
	m_bUseDefaultBiomes(true),
	m_bUseDefaultHeight(true),
	m_bUseDefaultComposition(true),
	m_bUseDefaultFinish(true)
{
	m_BlockArea.Create(cChunkDef::Width, cChunkDef::Height, cChunkDef::Width);
	/*
	memset(m_BlockTypes, 0, sizeof(cChunkDef::BlockTypes));
	memset(m_BlockMeta,  0, sizeof(cChunkDef::BlockNibbles));
	*/
	memset(m_BiomeMap.data(),   0, sizeof(cChunkDef::BiomeMap));
	memset(m_HeightMap.data(),  0, sizeof(cChunkDef::HeightMap));
}





cChunkDesc::~cChunkDesc()
{
	// Nothing needed yet
}





void cChunkDesc::SetChunkCoords(cChunkCoords a_Coords)
{
	m_Coords = a_Coords;
}





void cChunkDesc::FillBlocks(BlockState a_Block)
{
	m_BlockArea.Fill(cBlockArea::baBlocks, a_Block);
}





void cChunkDesc::SetBlock(Vector3i a_RelPos, BlockState a_Block)
{
	m_BlockArea.SetRelBlock(a_RelPos, a_Block);
}





BlockState cChunkDesc::GetBlock(Vector3i a_RelPos) const
{
	return m_BlockArea.GetRelBlock(a_RelPos);
}





void cChunkDesc::SetBiome(int a_RelX, int a_RelZ, EMCSBiome a_BiomeID)
{
	cChunkDef::SetBiome(m_BiomeMap, a_RelX, a_RelZ, a_BiomeID);
}





EMCSBiome cChunkDesc::GetBiome(int a_RelX, int a_RelZ) const
{
	return cChunkDef::GetBiome(m_BiomeMap, a_RelX, a_RelZ);
}





void cChunkDesc::SetHeight(int a_RelX, int a_RelZ, HEIGHTTYPE a_Height)
{
	cChunkDef::SetHeight(m_HeightMap, a_RelX, a_RelZ, a_Height);
}





HEIGHTTYPE cChunkDesc::GetHeight(int a_RelX, int a_RelZ) const
{
	return cChunkDef::GetHeight(m_HeightMap, a_RelX, a_RelZ);
}





void cChunkDesc::SetHeightFromShape(const Shape & a_Shape)
{
	for (int z = 0; z < cChunkDef::Width; z++)
	{
		for (int x = 0; x < cChunkDef::Width; x++)
		{
			for (HEIGHTTYPE y = cChunkDef::Height - 1; y > 0; y--)
			{
				if (a_Shape[y + x * 256 + z * 16 * 256] != 0)
				{
					cChunkDef::SetHeight(m_HeightMap, x, z, y);
					break;
				}
			}  // for y
		}  // for x
	}  // for z
}





void cChunkDesc::GetShapeFromHeight(Shape & a_Shape) const
{
	for (int z = 0; z < cChunkDef::Width; z++)
	{
		for (int x = 0; x < cChunkDef::Width; x++)
		{
			int height = cChunkDef::GetHeight(m_HeightMap, x, z);
			for (int y = 0; y <= height; y++)
			{
				a_Shape[y + x * 256 + z * 16 * 256] = 1;
			}

			for (int y = height + 1; y < cChunkDef::Height; y++)
			{
				a_Shape[y + x * 256 + z * 16 * 256] = 0;
			}  // for y
		}  // for x
	}  // for z
}





void cChunkDesc::SetUseDefaultBiomes(bool a_bUseDefaultBiomes)
{
	m_bUseDefaultBiomes = a_bUseDefaultBiomes;
}





bool cChunkDesc::IsUsingDefaultBiomes(void) const
{
	return m_bUseDefaultBiomes;
}





void cChunkDesc::SetUseDefaultHeight(bool a_bUseDefaultHeight)
{
	m_bUseDefaultHeight = a_bUseDefaultHeight;
}





bool cChunkDesc::IsUsingDefaultHeight(void) const
{
	return m_bUseDefaultHeight;
}





void cChunkDesc::SetUseDefaultComposition(bool a_bUseDefaultComposition)
{
	m_bUseDefaultComposition = a_bUseDefaultComposition;
}





bool cChunkDesc::IsUsingDefaultComposition(void) const
{
	return m_bUseDefaultComposition;
}





void cChunkDesc::SetUseDefaultFinish(bool a_bUseDefaultFinish)
{
	m_bUseDefaultFinish = a_bUseDefaultFinish;
}





bool cChunkDesc::IsUsingDefaultFinish(void) const
{
	return m_bUseDefaultFinish;
}





void cChunkDesc::WriteBlockArea(const cBlockArea & a_BlockArea, Vector3i a_RelPos, cBlockArea::eMergeStrategy a_MergeStrategy)
{
	m_BlockArea.Merge(a_BlockArea, a_RelPos, a_MergeStrategy);
}





void cChunkDesc::ReadBlockArea(cBlockArea & a_Dest, int a_MinRelX, int a_MaxRelX, int a_MinRelY, int a_MaxRelY, int a_MinRelZ, int a_MaxRelZ)
{
	// Normalize the coords:
	if (a_MinRelX > a_MaxRelX)
	{
		std::swap(a_MinRelX, a_MaxRelX);
	}
	if (a_MinRelY > a_MaxRelY)
	{
		std::swap(a_MinRelY, a_MaxRelY);
	}
	if (a_MinRelZ > a_MaxRelZ)
	{
		std::swap(a_MinRelZ, a_MaxRelZ);
	}

	// Include the Max coords:
	a_MaxRelX += 1;
	a_MaxRelY += 1;
	a_MaxRelZ += 1;

	// Check coords validity:
	if (a_MinRelX < 0)
	{
		LOGWARNING("%s: MinRelX less than zero, adjusting to zero", __FUNCTION__);
		a_MinRelX = 0;
	}
	else if (a_MinRelX >= cChunkDef::Width)
	{
		LOGWARNING("%s: MinRelX more than chunk width, adjusting to chunk width", __FUNCTION__);
		a_MinRelX = cChunkDef::Width - 1;
	}
	if (a_MaxRelX < 0)
	{
		LOGWARNING("%s: MaxRelX less than zero, adjusting to zero", __FUNCTION__);
		a_MaxRelX = 0;
	}
	else if (a_MaxRelX > cChunkDef::Width)
	{
		LOGWARNING("%s: MaxRelX more than chunk width, adjusting to chunk width", __FUNCTION__);
		a_MaxRelX = cChunkDef::Width;
	}

	if (a_MinRelY < 0)
	{
		LOGWARNING("%s: MinRelY less than zero, adjusting to zero", __FUNCTION__);
		a_MinRelY = 0;
	}
	else if (a_MinRelY >= cChunkDef::Height)
	{
		LOGWARNING("%s: MinRelY more than chunk height, adjusting to chunk height", __FUNCTION__);
		a_MinRelY = cChunkDef::Height - 1;
	}
	if (a_MaxRelY < 0)
	{
		LOGWARNING("%s: MaxRelY less than zero, adjusting to zero", __FUNCTION__);
		a_MaxRelY = 0;
	}
	else if (a_MaxRelY > cChunkDef::Height)
	{
		LOGWARNING("%s: MaxRelY more than chunk height, adjusting to chunk height", __FUNCTION__);
		a_MaxRelY = cChunkDef::Height;
	}

	if (a_MinRelZ < 0)
	{
		LOGWARNING("%s: MinRelZ less than zero, adjusting to zero", __FUNCTION__);
		a_MinRelZ = 0;
	}
	else if (a_MinRelZ >= cChunkDef::Width)
	{
		LOGWARNING("%s: MinRelZ more than chunk width, adjusting to chunk width", __FUNCTION__);
		a_MinRelZ = cChunkDef::Width - 1;
	}
	if (a_MaxRelZ < 0)
	{
		LOGWARNING("%s: MaxRelZ less than zero, adjusting to zero", __FUNCTION__);
		a_MaxRelZ = 0;
	}
	else if (a_MaxRelZ > cChunkDef::Width)
	{
		LOGWARNING("%s: MaxRelZ more than chunk width, adjusting to chunk width", __FUNCTION__);
		a_MaxRelZ = cChunkDef::Width;
	}

	// Prepare the block area:
	int SizeX = a_MaxRelX - a_MinRelX;
	int SizeY = a_MaxRelY - a_MinRelY;
	int SizeZ = a_MaxRelZ - a_MinRelZ;
	a_Dest.Clear();
	a_Dest.m_Origin.x = m_Coords.m_ChunkX * cChunkDef::Width + a_MinRelX;
	a_Dest.m_Origin.y = a_MinRelY;
	a_Dest.m_Origin.z = m_Coords.m_ChunkZ * cChunkDef::Width + a_MinRelZ;
	a_Dest.SetSize(SizeX, SizeY, SizeZ, cBlockArea::baBlocks);

	for (int y = 0; y < SizeY; y++)
	{
		int CDY = a_MinRelY + y;
		for (int z = 0; z < SizeZ; z++)
		{
			int CDZ = a_MinRelZ + z;
			for (int x = 0; x < SizeX; x++)
			{
				int CDX = a_MinRelX + x;
				a_Dest.SetRelBlock({x, y, z}, GetBlock({CDX, CDY, CDZ}));
			}  // for x
		}  // for z
	}  // for y
}





HEIGHTTYPE cChunkDesc::GetMaxHeight(void) const
{
	return *std::max_element(m_HeightMap.begin(), m_HeightMap.end());
}





HEIGHTTYPE cChunkDesc::GetMinHeight(void) const
{
	return *std::min_element(m_HeightMap.begin(), m_HeightMap.end());
}





void cChunkDesc::FillRelCuboid(
	int a_MinX, int a_MaxX,
	int a_MinY, int a_MaxY,
	int a_MinZ, int a_MaxZ,
	BlockState a_Block
)
{
	int MinX = std::max(a_MinX, 0);
	int MinY = std::max(a_MinY, 0);
	int MinZ = std::max(a_MinZ, 0);
	int MaxX = std::min(a_MaxX, cChunkDef::Width - 1);
	int MaxY = std::min(a_MaxY, cChunkDef::Height - 1);
	int MaxZ = std::min(a_MaxZ, cChunkDef::Width - 1);

	for (int y = MinY; y <= MaxY; y++)
	{
		for (int z = MinZ; z <= MaxZ; z++)
		{
			for (int x = MinX; x <= MaxX; x++)
			{
				SetBlock({x, y, z}, a_Block);
			}
		}  // for z
	}  // for y
}





void cChunkDesc::ReplaceRelCuboid(
	int a_MinX, int a_MaxX,
	int a_MinY, int a_MaxY,
	int a_MinZ, int a_MaxZ,
	BlockState a_SrcBlock,
	BlockState a_DstBlock
)
{
	int MinX = std::max(a_MinX, 0);
	int MinY = std::max(a_MinY, 0);
	int MinZ = std::max(a_MinZ, 0);
	int MaxX = std::min(a_MaxX, cChunkDef::Width - 1);
	int MaxY = std::min(a_MaxY, cChunkDef::Height - 1);
	int MaxZ = std::min(a_MaxZ, cChunkDef::Width - 1);

	for (int y = MinY; y <= MaxY; y++)
	{
		for (int z = MinZ; z <= MaxZ; z++)
		{
			for (int x = MinX; x <= MaxX; x++)
			{
				auto OldBlock = GetBlock({x, y, z});
				if (OldBlock == a_SrcBlock)
				{
					SetBlock({x, y, z}, a_DstBlock);
				}
			}
		}  // for z
	}  // for y
}





void cChunkDesc::FloorRelCuboid(
	int a_MinX, int a_MaxX,
	int a_MinY, int a_MaxY,
	int a_MinZ, int a_MaxZ,
	BlockState a_DstBlock
)
{
	int MinX = std::max(a_MinX, 0);
	int MinY = std::max(a_MinY, 0);
	int MinZ = std::max(a_MinZ, 0);
	int MaxX = std::min(a_MaxX, cChunkDef::Width - 1);
	int MaxY = std::min(a_MaxY, cChunkDef::Height - 1);
	int MaxZ = std::min(a_MaxZ, cChunkDef::Width - 1);

	for (int y = MinY; y <= MaxY; y++)
	{
		for (int z = MinZ; z <= MaxZ; z++)
		{
			for (int x = MinX; x <= MaxX; x++)
			{
				switch (GetBlock({x, y, z}).Type())
				{
					case BlockType::Air:
					case BlockType::CaveAir:
					case BlockType::VoidAir:
					case BlockType::Water:
					{
						SetBlock({x, y, z}, a_DstBlock);
						break;
					}
					default: break;
				}  // switch (GetBlockType)
			}  // for x
		}  // for z
	}  // for y
}





void cChunkDesc::RandomFillRelCuboid(
	int a_MinX, int a_MaxX,
	int a_MinY, int a_MaxY,
	int a_MinZ, int a_MaxZ,
	BlockState a_DestBlock,
	int a_RandomSeed, int a_ChanceOutOf10k
)
{
	cNoise Noise(a_RandomSeed);
	int MinX = std::max(a_MinX, 0);
	int MinY = std::max(a_MinY, 0);
	int MinZ = std::max(a_MinZ, 0);
	int MaxX = std::min(a_MaxX, cChunkDef::Width - 1);
	int MaxY = std::min(a_MaxY, cChunkDef::Height - 1);
	int MaxZ = std::min(a_MaxZ, cChunkDef::Width - 1);

	for (int y = MinY; y <= MaxY; y++)
	{
		for (int z = MinZ; z <= MaxZ; z++)
		{
			for (int x = MinX; x <= MaxX; x++)
			{
				int rnd = (Noise.IntNoise3DInt(x, y, z) / 7) % 10000;
				if (rnd <= a_ChanceOutOf10k)
				{
					SetBlock({x, y, z}, a_DestBlock);
				}
			}
		}  // for z
	}  // for y
}





cBlockEntity * cChunkDesc::GetBlockEntity(Vector3i a_RelPos)
{
	const auto Idx = cChunkDef::MakeIndex(a_RelPos);
	const auto Iterator = m_BlockArea.GetBlockEntities().find(Idx);

	if (Iterator != m_BlockArea.GetBlockEntities().end())
	{
		// Already in the list:
		cBlockEntity * BlockEntity = Iterator->second.get();
		if (BlockEntity->GetBlockType() == GetBlock(a_RelPos).Type())
		{
			// Correct type, already present. Return it:
			return BlockEntity;
		}
		else
		{
			// Wrong type, the block type has been overwritten. Erase and create new:
			m_BlockArea.GetBlockEntities().erase(Iterator);
		}
	}

	auto AbsPos = cChunkDef::RelativeToAbsolute(a_RelPos, GetChunkCoords());

	// The block entity is not created yet, try to create it and add to list:
	auto be = cBlockEntity::CreateByBlockType(GetBlock(a_RelPos), AbsPos);
	if (be == nullptr)
	{
		// No block entity for this block type
		return nullptr;
	}
	auto res = m_BlockArea.GetBlockEntities().emplace(Idx, std::move(be));
	return res.first->second.get();
}





void cChunkDesc::UpdateHeightmap(void)
{
	for (int x = 0; x < cChunkDef::Width; x++)
	{
		for (int z = 0; z < cChunkDef::Width; z++)
		{
			HEIGHTTYPE Height = 0;
			for (HEIGHTTYPE y = cChunkDef::Height - 1; y > 0; y--)
			{
				if (!IsBlockAir(GetBlock({x, y, z})))
				{
					Height = y;
					break;
				}
			}  // for y
			SetHeight(x, z, Height);
		}  // for z
	}  // for x
}





#ifndef NDEBUG

void cChunkDesc::VerifyHeightmap(void)
{
	for (int x = 0; x < cChunkDef::Width; x++)
	{
		for (int z = 0; z < cChunkDef::Width; z++)
		{
			for (int y = cChunkDef::Height - 1; y > 0; y--)
			{
				if (!IsBlockAir(GetBlock({x, y, z})))
				{
					int Height = GetHeight(x, z);
					ASSERT(Height == y);
					break;
				}
			}  // for y
		}  // for z
	}  // for x
}

#endif  // !NDEBUG





