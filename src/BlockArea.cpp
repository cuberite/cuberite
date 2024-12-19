
// BlockArea.cpp

// NOTE: compile.sh checks for this file in order to determine if this is the Cuberite folder.
// Please modify compile.sh if you want to rename or remove this file.
// This file was chosen arbitrarily and it's a good enough indicator we're in the Cuberite folder.

// Implements the cBlockArea object representing an area of block data that can be queried from cWorld and then accessed again without further queries
// The object also supports writing the blockdata back into cWorld, even into other coords

#include "Globals.h"
#include "BlockArea.h"
#include "OSSupport/GZipFile.h"
#include "Blocks/BlockHandler.h"
#include "ChunkData.h"
#include "BlockEntities/BlockEntity.h"
#include "Item.h"
#include "BlockInfo.h"
#include "Blocks/BlockAir.h"





// Disable MSVC warnings: "conditional expression is constant"
#ifdef _MSC_VER
	#pragma warning(push)
	#pragma warning(disable:4127)
#endif





typedef void (CombinatorFunc)(BlockState & a_DstBlock, BlockState a_SrcBlock);

/** Merges two blocktypes and blockmetas of the specified sizes and offsets using the specified combinator function
This wild construct allows us to pass a function argument and still have it inlined by the compiler. */
template <CombinatorFunc Combinator>
void InternalMergeBlocks(
	BlockState * a_DstBlocks, const BlockState * a_SrcBlocks,
	int a_SizeX, int a_SizeY, int a_SizeZ,
	int a_SrcOffX, int a_SrcOffY, int a_SrcOffZ,
	int a_DstOffX, int a_DstOffY, int a_DstOffZ,
	int a_SrcSizeX, int a_SrcSizeY, int a_SrcSizeZ,
	int a_DstSizeX, int a_DstSizeY, int a_DstSizeZ
)
{
	UNUSED(a_SrcSizeY);
	UNUSED(a_DstSizeY);
	for (int y = 0; y < a_SizeY; y++)
	{
		int SrcBaseY = (y + a_SrcOffY) * a_SrcSizeX * a_SrcSizeZ;
		int DstBaseY = (y + a_DstOffY) * a_DstSizeX * a_DstSizeZ;
		for (int z = 0; z < a_SizeZ; z++)
		{
			int SrcBaseZ = SrcBaseY + (z + a_SrcOffZ) * a_SrcSizeX;
			int DstBaseZ = DstBaseY + (z + a_DstOffZ) * a_DstSizeX;
			int SrcIdx = SrcBaseZ + a_SrcOffX;
			int DstIdx = DstBaseZ + a_DstOffX;
			for (int x = 0; x < a_SizeX; x++)
			{
				Combinator(a_DstBlocks[DstIdx], a_SrcBlocks[SrcIdx]);
				++DstIdx;
				++SrcIdx;
			}  // for x
		}  // for z
	}  // for y
}





/** Combinator used for cBlockArea::msOverwrite merging */
static inline void MergeCombinatorOverwrite(BlockState & a_DstBlock, BlockState a_SrcBlock)
{
	a_DstBlock = a_SrcBlock;
}





/** Combinator used for cBlockArea::msFillAir merging */
static inline void MergeCombinatorFillAir(BlockState & a_DstBlock, BlockState a_SrcBlock)
{
	if (cBlockAirHandler::IsBlockAir(a_DstBlock))
	{
		a_DstBlock = a_SrcBlock;
	}
	// "else" is the default, already in place
}





/** Combinator used for cBlockArea::msImprint merging */
static inline void MergeCombinatorImprint(BlockState & a_DstBlock, BlockState a_SrcBlock)
{
	if (!cBlockAirHandler::IsBlockAir(a_SrcBlock))
	{
		a_DstBlock = a_SrcBlock;
	}
	// "else" is the default, already in place
}





/** Combinator used for cBlockArea::msLake merging */
static inline void MergeCombinatorLake(BlockState & a_DstBlock, BlockState a_SrcBlock)
{
	// Sponge is the NOP block
	if (a_SrcBlock.Type() == BlockType::Sponge)
	{
		return;
	}

	// Air is always hollowed out
	if (cBlockAirHandler::IsBlockAir(a_SrcBlock))
	{
		a_DstBlock = Block::Air::Air();
		return;
	}

	// Water and lava are never overwritten
	switch (a_DstBlock.Type())
	{
		case BlockType::Water:
		case BlockType::Lava:
		{
			return;
		}
		default: break;
	}

	// Water and lava always overwrite
	switch (a_SrcBlock.Type())
	{
		case BlockType::Water:
		case BlockType::Lava:
		{
			a_DstBlock = a_SrcBlock;
			return;
		}
		default: break;
	}

	if (a_SrcBlock.Type() == BlockType::Stone)
	{
		switch (a_DstBlock.Type())
		{
			case BlockType::Dirt:
			case BlockType::ShortGrass:
			case BlockType::Mycelium:
			{
				a_DstBlock = BlockType::Stone;
				return;
			}
			default: break;
		}
	}
	// Everything else is left as it is
}





/** Combinator used for cBlockArea::msSpongePrint merging */
static inline void MergeCombinatorSpongePrint(BlockState & a_DstBlock, BlockState a_SrcBlock)
{
	// Sponge overwrites nothing, everything else overwrites anything
	if (a_SrcBlock != BlockType::Sponge)
	{
		a_DstBlock = a_SrcBlock;
	}
}





/** Combinator used for cBlockArea::msDifference merging */
static inline void MergeCombinatorDifference(BlockState & a_DstBlock, BlockState a_SrcBlock)
{
	if (a_DstBlock.Type() == a_SrcBlock.Type())
	{
		a_DstBlock = BlockType::Air;
	}
	else
	{
		a_DstBlock = a_SrcBlock;
	}
}





/** Combinator used for cBlockArea::msSimpleCompare merging */
static inline void MergeCombinatorSimpleCompare(BlockState & a_DstBlock, BlockState a_SrcBlock)
{
	if (a_DstBlock == a_SrcBlock)
	{
		// The blocktypes are the same, and the blockmetas are not present or are the same
		a_DstBlock = Block::Air::Air();
	}
	else
	{
		// The blocktypes or blockmetas differ
		a_DstBlock = Block::Stone::Stone();
	}
}





/** Combinator used for cBlockArea::msMask merging */
static inline void MergeCombinatorMask(BlockState & a_DstBlock, BlockState a_SrcBlock)
{
	// If the blocks are the same, keep the dest; otherwise replace with air
	if (a_SrcBlock.Type() != a_DstBlock.Type())
	{
		a_DstBlock = BlockType::Air;
	}
}

// Re-enable previously disabled MSVC warnings
#ifdef _MSC_VER
	#pragma warning(pop)
#endif





////////////////////////////////////////////////////////////////////////////////
// cBlockArea:

cBlockArea::cBlockArea(void) = default;

bool cBlockArea::IsValidDataTypeCombination(int a_DataTypes)
{
	// BlockEntities require that BlockTypes be present, too
	if ((a_DataTypes & baBlockEntities) != 0)
	{
		if ((a_DataTypes & baBlocks) == 0)
		{
			return false;
		}
	}

	// All other combinations are considered valid
	return true;
}





void cBlockArea::Clear(void)
{
	m_Blocks.reset();
	m_BlockLight.reset();
	m_BlockSkyLight.reset();
	m_BlockEntities.reset();
	m_Origin.Set(0, 0, 0);
	m_Size.Set(0, 0, 0);
}





void cBlockArea::Create(int a_SizeX, int a_SizeY, int a_SizeZ, int a_DataTypes)
{
	ASSERT(a_SizeX > 0);
	ASSERT(a_SizeY > 0);
	ASSERT(a_SizeZ > 0);
	ASSERT(IsValidDataTypeCombination(a_DataTypes));

	// Warn if the height is too much, but proceed with the creation:
	if (a_SizeY > cChunkDef::Height)
	{
		LOGWARNING("Creating a cBlockArea with height larger than world height (%d). Continuing, but the area may misbehave.", a_SizeY);
	}

	Clear();
	SetSize(a_SizeX, a_SizeY, a_SizeZ, a_DataTypes);
	Fill(a_DataTypes, Block::Air::Air());
}





void cBlockArea::Create(const Vector3i & a_Size, int a_DataTypes)
{
	Create(a_Size.x, a_Size.y, a_Size.z, a_DataTypes);
}





void cBlockArea::SetWEOffset(int a_OffsetX, int a_OffsetY, int a_OffsetZ)
{
	m_WEOffset.Set(a_OffsetX, a_OffsetY, a_OffsetZ);
}





void cBlockArea::SetWEOffset(const Vector3i & a_Offset)
{
	m_WEOffset.Set(a_Offset.x, a_Offset.y, a_Offset.z);
}





void cBlockArea::SetOrigin(int a_OriginX, int a_OriginY, int a_OriginZ)
{
	m_Origin.Set(a_OriginX, a_OriginY, a_OriginZ);
}





void cBlockArea::SetOrigin(const Vector3i & a_Origin)
{
	m_Origin.Set(a_Origin.x, a_Origin.y, a_Origin.z);
}





bool cBlockArea::IsValidRelCoords(Vector3i a_RelPos) const
{
	return (
		(a_RelPos.x >= 0) && (a_RelPos.x < m_Size.x) &&
		(a_RelPos.y >= 0) && (a_RelPos.y < m_Size.y) &&
		(a_RelPos.z >= 0) && (a_RelPos.z < m_Size.z)
	);
}





bool cBlockArea::IsValidCoords(const Vector3i a_Coords) const
{
	return IsValidRelCoords(a_Coords - m_Origin);
}





bool cBlockArea::Read(cForEachChunkProvider & a_ForEachChunkProvider, int a_MinBlockX, int a_MaxBlockX, int a_MinBlockY, int a_MaxBlockY, int a_MinBlockZ, int a_MaxBlockZ, int a_DataTypes)
{
	ASSERT(IsValidDataTypeCombination(a_DataTypes));
	ASSERT(cChunkDef::IsValidHeight({a_MinBlockX, a_MinBlockY, a_MinBlockZ}));
	ASSERT(cChunkDef::IsValidHeight({a_MaxBlockX, a_MaxBlockY, a_MaxBlockZ}));
	ASSERT(a_MinBlockX <= a_MaxBlockX);
	ASSERT(a_MinBlockY <= a_MaxBlockY);
	ASSERT(a_MinBlockZ <= a_MaxBlockZ);

	// Include the Max coords:
	a_MaxBlockX += 1;
	a_MaxBlockY += 1;
	a_MaxBlockZ += 1;

	// Allocate the needed memory:
	Clear();
	if (!SetSize(a_MaxBlockX - a_MinBlockX, a_MaxBlockY - a_MinBlockY, a_MaxBlockZ - a_MinBlockZ, a_DataTypes))
	{
		return false;
	}
	m_Origin.Set(a_MinBlockX, a_MinBlockY, a_MinBlockZ);
	cChunkReader Reader(*this);

	// Convert block coords to chunks coords:
	int MinChunkX, MaxChunkX;
	int MinChunkZ, MaxChunkZ;
	cChunkDef::AbsoluteToRelative(a_MinBlockX, a_MinBlockY, a_MinBlockZ, MinChunkX, MinChunkZ);
	cChunkDef::AbsoluteToRelative(a_MaxBlockX, a_MaxBlockY, a_MaxBlockZ, MaxChunkX, MaxChunkZ);

	// Query block data:
	if (!a_ForEachChunkProvider.ForEachChunkInRect(MinChunkX, MaxChunkX, MinChunkZ, MaxChunkZ, Reader))
	{
		Clear();
		return false;
	}

	return true;
}





bool cBlockArea::Read(cForEachChunkProvider & a_ForEachChunkProvider, const cCuboid & a_Bounds, int a_DataTypes)
{
	return Read(
		a_ForEachChunkProvider,
		a_Bounds.p1.x, a_Bounds.p2.x,
		a_Bounds.p1.y, a_Bounds.p2.y,
		a_Bounds.p1.z, a_Bounds.p2.z,
		a_DataTypes
	);
}





bool cBlockArea::Read(cForEachChunkProvider & a_ForEachChunkProvider, const Vector3i & a_Point1, const Vector3i & a_Point2, int a_DataTypes)
{
	return Read(
		a_ForEachChunkProvider,
		a_Point1.x, a_Point2.x,
		a_Point1.y, a_Point2.y,
		a_Point1.z, a_Point2.z,
		a_DataTypes
	);
}





bool cBlockArea::Write(cForEachChunkProvider & a_ForEachChunkProvider, int a_MinBlockX, int a_MinBlockY, int a_MinBlockZ, int a_DataTypes)
{
	ASSERT((a_DataTypes & GetDataTypes()) == a_DataTypes);  // Are you requesting only the data that I have?
	ASSERT(cChunkDef::IsValidHeight({a_MinBlockX, a_MinBlockY, a_MinBlockZ}));
	ASSERT(cChunkDef::IsValidHeight({a_MinBlockX, a_MinBlockY + m_Size.y - 1, a_MinBlockZ}));

	return a_ForEachChunkProvider.WriteBlockArea(*this, a_MinBlockX, a_MinBlockY, a_MinBlockZ, a_DataTypes);
}





bool cBlockArea::Write(cForEachChunkProvider & a_ForEachChunkProvider, const Vector3i & a_MinCoords, int a_DataTypes)
{
	return Write(
		a_ForEachChunkProvider,
		a_MinCoords.x, a_MinCoords.y, a_MinCoords.z,
		a_DataTypes
	);
}





void cBlockArea::CopyTo(cBlockArea & a_Into) const
{
	if (&a_Into == this)
	{
		LOGWARNING("Trying to copy a cBlockArea into self, ignoring.");
		return;
	}

	a_Into.Clear();
	a_Into.SetSize(m_Size.x, m_Size.y, m_Size.z, GetDataTypes());
	a_Into.m_Origin = m_Origin;
	size_t BlockCount = GetBlockCount();
	if (HasBlocks())
	{
		memcpy(a_Into.GetBlocks(), GetBlocks(), BlockCount * sizeof(BlockState));
	}
	if (HasBlockLights())
	{
		memcpy(a_Into.GetBlockLight(), GetBlockLight(), BlockCount * sizeof(LIGHTTYPE));
	}
	if (HasBlockSkyLights())
	{
		memcpy(a_Into.GetBlockSkyLight(), GetBlockSkyLight(), BlockCount * sizeof(LIGHTTYPE));
	}
	if (HasBlockEntities())
	{
		a_Into.m_BlockEntities->clear();
		for (const auto & keyPair: *m_BlockEntities)
		{
			const auto & pos = keyPair.second->GetPos();
			a_Into.m_BlockEntities->emplace(keyPair.first, keyPair.second->Clone(pos));
		}
	}
}





void cBlockArea::CopyFrom(const cBlockArea & a_From)
{
	a_From.CopyTo(*this);
}





void cBlockArea::DumpToRawFile(const AString & a_FileName)
{
	cFile f;
	if (!f.Open(a_FileName, cFile::fmWrite))
	{
		LOGWARNING("cBlockArea: Cannot open file \"%s\" for raw dump", a_FileName.c_str());
		return;
	}
	UInt32 SizeX = ntohl(static_cast<UInt32>(m_Size.x));
	UInt32 SizeY = ntohl(static_cast<UInt32>(m_Size.y));
	UInt32 SizeZ = ntohl(static_cast<UInt32>(m_Size.z));
	f.Write(&SizeX, 4);
	f.Write(&SizeY, 4);
	f.Write(&SizeZ, 4);
	unsigned char DataTypes = static_cast<unsigned char>(GetDataTypes());
	f.Write(&DataTypes, 1);
	size_t NumBlocks = GetBlockCount();
	if (HasBlocks())
	{
		f.Write(GetBlocks(), NumBlocks * sizeof(BlockState));
	}
	if (HasBlockLights())
	{
		f.Write(GetBlockLight(), NumBlocks);
	}
	if (HasBlockSkyLights())
	{
		f.Write(GetBlockSkyLight(), NumBlocks);
	}
}





void cBlockArea::Crop(int a_AddMinX, int a_SubMaxX, int a_AddMinY, int a_SubMaxY, int a_AddMinZ, int a_SubMaxZ)
{
	if (
		(a_AddMinX + a_SubMaxX >= m_Size.x) ||
		(a_AddMinY + a_SubMaxY >= m_Size.y) ||
		(a_AddMinZ + a_SubMaxZ >= m_Size.z)
	)
	{
		LOGWARNING("cBlockArea:Crop called with more croping than the dimensions: %d x %d x %d with cropping %d, %d and %d",
			m_Size.x, m_Size.y, m_Size.z,
			a_AddMinX + a_SubMaxX, a_AddMinY + a_SubMaxY, a_AddMinZ + a_SubMaxZ
		);
		return;
	}

	if (HasBlocks())
	{
		CropBlocks(a_AddMinX, a_SubMaxX, a_AddMinY, a_SubMaxY, a_AddMinZ, a_SubMaxZ);
	}
	if (HasBlockLights())
	{
		CropLightValues(m_BlockLight, a_AddMinX, a_SubMaxX, a_AddMinY, a_SubMaxY, a_AddMinZ, a_SubMaxZ);
	}
	if (HasBlockSkyLights())
	{
		CropLightValues(m_BlockSkyLight, a_AddMinX, a_SubMaxX, a_AddMinY, a_SubMaxY, a_AddMinZ, a_SubMaxZ);
	}
	if (HasBlockEntities())
	{
		const Vector3i AddMin{ a_AddMinX, a_AddMinY, a_AddMinZ };
		const cCuboid CropBox{ AddMin, m_Size - Vector3i{a_SubMaxX, a_SubMaxY, a_SubMaxZ} };

		// Move and crop block Entities:
		cBlockEntities oldBE;
		std::swap(oldBE, *m_BlockEntities);
		for (auto & keyPair: oldBE)
		{
			auto & be = keyPair.second;
			auto Pos = be->GetPos();
			if (CropBox.IsInside(Pos))
			{
				// The block entity is within the new coords, recalculate its coords to match the new area:
				Pos -= AddMin;
				be->SetPos(Pos);
				m_BlockEntities->emplace(MakeIndex(Pos.x, Pos.y, Pos.z), std::move(be));
			}
		}
	}

	m_Origin.Move(a_AddMinX, a_AddMinY, a_AddMinZ);
	m_Size.x -= a_AddMinX + a_SubMaxX;
	m_Size.y -= a_AddMinY + a_SubMaxY;
	m_Size.z -= a_AddMinZ + a_SubMaxZ;
}





void cBlockArea::Expand(int a_SubMinX, int a_AddMaxX, int a_SubMinY, int a_AddMaxY, int a_SubMinZ, int a_AddMaxZ)
{
	if (HasBlocks())
	{
		ExpandBlocks(a_SubMinX, a_AddMaxX, a_SubMinY, a_AddMaxY, a_SubMinZ, a_AddMaxZ);
	}
	if (HasBlockLights())
	{
		ExpandLightValues(m_BlockLight, a_SubMinX, a_AddMaxX, a_SubMinY, a_AddMaxY, a_SubMinZ, a_AddMaxZ);
	}
	if (HasBlockSkyLights())
	{
		ExpandLightValues(m_BlockSkyLight, a_SubMinX, a_AddMaxX, a_SubMinY, a_AddMaxY, a_SubMinZ, a_AddMaxZ);
	}
	if (HasBlockEntities())
	{
		// Move block entities:
		cBlockEntities oldBE;
		std::swap(oldBE, *m_BlockEntities);
		for (auto & keyPair: oldBE)
		{
			auto & be = keyPair.second;
			auto posX = be->GetPosX() + a_SubMinX;
			auto posY = be->GetPosY() + a_SubMinY;
			auto posZ = be->GetPosZ() + a_SubMinZ;
			be->SetPos({posX, posY, posZ});
			m_BlockEntities->emplace(MakeIndex(posX, posY, posZ), std::move(be));
		}
	}

	m_Origin.Move(-a_SubMinX, -a_SubMinY, -a_SubMinZ);
	m_Size.x += a_SubMinX + a_AddMaxX;
	m_Size.y += a_SubMinY + a_AddMaxY;
	m_Size.z += a_SubMinZ + a_AddMaxZ;
}





void cBlockArea::Merge(const cBlockArea & a_Src, const Vector3i & a_RelMinCoords, eMergeStrategy a_Strategy)
{
	MergeByStrategy(a_Src, a_RelMinCoords, a_Strategy);
}





void cBlockArea::Fill(int a_DataTypes, BlockState a_Block, LIGHTTYPE a_BlockLight, LIGHTTYPE a_BlockSkyLight)
{
	if ((a_DataTypes & GetDataTypes()) != a_DataTypes)
	{
		LOGWARNING("%s: requested datatypes that are not present in the BlockArea object, trimming those away (req 0x%x, stor 0x%x)",
			__FUNCTION__, a_DataTypes, GetDataTypes()
		);
		a_DataTypes = a_DataTypes & GetDataTypes();
	}

	size_t BlockCount = GetBlockCount();
	if ((a_DataTypes & baBlocks) != 0)
	{
		for (size_t i = 0; i < BlockCount; i++)
		{
			m_Blocks[i] = a_Block;
		}
	}
	if ((a_DataTypes & baLight) != 0)
	{
		for (size_t i = 0; i < BlockCount; i++)
		{
			m_BlockLight[i] = a_BlockLight;
		}
	}
	if ((a_DataTypes & baSkyLight) != 0)
	{
		for (size_t i = 0; i < BlockCount; i++)
		{
			m_BlockSkyLight[i] = a_BlockSkyLight;
		}
	}

	// If the area contains block entities, remove those not matching and replace with whatever block entity block was filled
	if (HasBlockEntities() && ((a_DataTypes & baBlocks) != 0))
	{
		if (cBlockEntity::IsBlockEntityBlockType(a_Block))
		{
			RescanBlockEntities();
		}
		else
		{
			m_BlockEntities->clear();
		}
	}
}





void cBlockArea::FillRelCuboid(int a_MinRelX, int a_MaxRelX, int a_MinRelY, int a_MaxRelY, int a_MinRelZ, int a_MaxRelZ,
	int a_DataTypes, BlockState a_Block,
	LIGHTTYPE a_BlockLight, LIGHTTYPE a_BlockSkyLight
)
{
	if ((a_DataTypes & GetDataTypes()) != a_DataTypes)
	{
		LOGWARNING("%s: requested datatypes that are not present in the BlockArea object, trimming those away (req 0x%x, stor 0x%x)",
			__FUNCTION__, a_DataTypes, GetDataTypes()
		);
		a_DataTypes = a_DataTypes & GetDataTypes();
	}

	if ((a_DataTypes & baBlocks) != 0)
	{
		for (int y = a_MinRelY; y <= a_MaxRelY; y++) for (int z = a_MinRelZ; z <= a_MaxRelZ; z++) for (int x = a_MinRelX; x <= a_MaxRelX; x++)
		{
			m_Blocks[MakeIndex(x, y, z)] = a_Block;
		}  // for x, z, y
	}
	if ((a_DataTypes & baLight) != 0)
	{
		for (int y = a_MinRelY; y <= a_MaxRelY; y++) for (int z = a_MinRelZ; z <= a_MaxRelZ; z++) for (int x = a_MinRelX; x <= a_MaxRelX; x++)
		{
			m_BlockLight[MakeIndex(x, y, z)] = a_BlockLight;
		}  // for x, z, y
	}
	if ((a_DataTypes & baSkyLight) != 0)
	{
		for (int y = a_MinRelY; y <= a_MaxRelY; y++) for (int z = a_MinRelZ; z <= a_MaxRelZ; z++) for (int x = a_MinRelX; x <= a_MaxRelX; x++)
		{
			m_BlockSkyLight[MakeIndex(x, y, z)] = a_BlockSkyLight;
		}  // for x, z, y
	}

	// If the area contains block entities, remove those in the affected cuboid and replace with whatever block entity block was filled:
	if (HasBlockEntities() && ((a_DataTypes & baBlocks) != 0))
	{
		if (cBlockEntity::IsBlockEntityBlockType(a_Block))
		{
			RescanBlockEntities();
		}
		else
		{
			m_BlockEntities->clear();
		}
	}
}





void cBlockArea::FillRelCuboid(const cCuboid & a_RelCuboid,
	int a_DataTypes, BlockState a_Block,
	LIGHTTYPE a_BlockLight, LIGHTTYPE a_BlockSkyLight
)
{
	FillRelCuboid(
		a_RelCuboid.p1.x, a_RelCuboid.p2.x,
		a_RelCuboid.p1.y, a_RelCuboid.p2.y,
		a_RelCuboid.p1.z, a_RelCuboid.p2.z,
		a_DataTypes, a_Block, a_BlockLight, a_BlockSkyLight
	);
}





void cBlockArea::RelLine(int a_RelX1, int a_RelY1, int a_RelZ1, int a_RelX2, int a_RelY2, int a_RelZ2,
	int a_DataTypes, BlockState a_Block,
	LIGHTTYPE a_BlockLight, LIGHTTYPE a_BlockSkyLight
)
{
	// Bresenham-3D algorithm for drawing lines:
	int dx = abs(a_RelX2 - a_RelX1);
	int dy = abs(a_RelY2 - a_RelY1);
	int dz = abs(a_RelZ2 - a_RelZ1);
	int sx = (a_RelX1 < a_RelX2) ? 1 : -1;
	int sy = (a_RelY1 < a_RelY2) ? 1 : -1;
	int sz = (a_RelZ1 < a_RelZ2) ? 1 : -1;

	if (dx >= std::max(dy, dz))  // x dominant
	{
		int yd = dy - dx / 2;
		int zd = dz - dx / 2;

		for (;;)
		{
			RelSetData({a_RelX1, a_RelY1, a_RelZ1}, a_DataTypes, a_Block, a_BlockLight, a_BlockSkyLight);

			if (a_RelX1 == a_RelX2)
			{
				break;
			}

			if (yd >= 0)  // move along y
			{
				a_RelY1 += sy;
				yd -= dx;
			}

			if (zd >= 0)  // move along z
			{
				a_RelZ1 += sz;
				zd -= dx;
			}

			// move along x
			a_RelX1  += sx;
			yd += dy;
			zd += dz;
		}
	}
	else if (dy >= std::max(dx, dz))  // y dominant
	{
		int xd = dx - dy / 2;
		int zd = dz - dy / 2;

		for (;;)
		{
			RelSetData({a_RelX1, a_RelY1, a_RelZ1}, a_DataTypes, a_Block, a_BlockLight, a_BlockSkyLight);

			if (a_RelY1 == a_RelY2)
			{
				break;
			}

			if (xd >= 0)  // move along x
			{
				a_RelX1 += sx;
				xd -= dy;
			}

			if (zd >= 0)  // move along z
			{
				a_RelZ1 += sz;
				zd -= dy;
			}

			// move along y
			a_RelY1 += sy;
			xd += dx;
			zd += dz;
		}
	}
	else
	{
		// z dominant
		ASSERT(dz >= std::max(dx, dy));
		int xd = dx - dz / 2;
		int yd = dy - dz / 2;

		for (;;)
		{
			RelSetData({a_RelX1, a_RelY1, a_RelZ1}, a_DataTypes, a_Block, a_BlockLight, a_BlockSkyLight);

			if (a_RelZ1 == a_RelZ2)
			{
				break;
			}

			if (xd >= 0)  // move along x
			{
				a_RelX1 += sx;
				xd -= dz;
			}

			if (yd >= 0)  // move along y
			{
				a_RelY1 += sy;
				yd -= dz;
			}

			// move along z
			a_RelZ1 += sz;
			xd += dx;
			yd += dy;
		}
	}  // if (which dimension is dominant)
}





void cBlockArea::RelLine(const Vector3i & a_Point1, const Vector3i & a_Point2,
	int a_DataTypes, BlockState a_Block,
	LIGHTTYPE a_BlockLight, LIGHTTYPE a_BlockSkyLight
)
{
	RelLine(
		a_Point1.x, a_Point1.y, a_Point1.z,
		a_Point2.x, a_Point2.y, a_Point2.z,
		a_DataTypes, a_Block, a_BlockLight, a_BlockSkyLight
	);
}





void cBlockArea::RotateCCW(void)
{
	if (!HasBlocks())
	{
		LOGWARNING("cBlockArea: Cannot rotate blockmeta without blocktypes!");
		return;
	}

	// We are guaranteed that both blocktypes and blockmetas exist; rotate both at the same time:
	BLOCKARRAY NewBlocks{ new BlockState[GetBlockCount()] };
	for (int x = 0; x < m_Size.x; x++)
	{
		int NewZ = m_Size.x - x - 1;
		for (int z = 0; z < m_Size.z; z++)
		{
			int NewX = z;
			for (int y = 0; y < m_Size.y; y++)
			{
				auto NewIdx = MakeIndexForSize({ NewX, y, NewZ }, { m_Size.z, m_Size.y, m_Size.x });
				auto OldIdx = MakeIndex(x, y, z);
				NewBlocks[NewIdx] = m_Blocks[OldIdx];
			}  // for y
		}  // for z
	}  // for x
	m_Blocks = std::move(NewBlocks);

	// Rotate the BlockEntities:
	if (HasBlockEntities())
	{
		cBlockEntities oldBE;
		std::swap(oldBE, *m_BlockEntities);
		for (auto & keyPair: oldBE)
		{
			auto & be = keyPair.second;
			auto newX = be->GetPosZ();
			auto newY = be->GetPosY();
			auto newZ = m_Size.x - be->GetPosX() - 1;
			auto newIdx = newX + newZ * m_Size.z + newY * m_Size.x * m_Size.z;
			be->SetPos({newX, newY, newZ});
			m_BlockEntities->emplace(newIdx, std::move(be));
		}
	}

	std::swap(m_Size.x, m_Size.z);
}





void cBlockArea::RotateCW(void)
{
	if (!HasBlocks())
	{
		LOGWARNING("cBlockArea: Cannot rotate blockmeta without blocktypes!");
		return;
	}

	// We are guaranteed that both blocktypes and blockmetas exist; rotate both at the same time:
	BLOCKARRAY NewBlocks{ new BlockState[GetBlockCount()] };
	for (int x = 0; x < m_Size.x; x++)
	{
		int NewZ = x;
		for (int z = 0; z < m_Size.z; z++)
		{
			int NewX = m_Size.z - z - 1;
			for (int y = 0; y < m_Size.y; y++)
			{
				auto NewIdx = MakeIndexForSize({ NewX, y, NewZ }, { m_Size.z, m_Size.y, m_Size.x });
				auto OldIdx = MakeIndex(x, y, z);
				NewBlocks[NewIdx] = m_Blocks[OldIdx];
			}  // for y
		}  // for z
	}  // for x
	m_Blocks = std::move(NewBlocks);

	// Rotate the BlockEntities:
	if (HasBlockEntities())
	{
		cBlockEntities oldBE;
		std::swap(oldBE, *m_BlockEntities);
		for (auto & keyPair: oldBE)
		{
			auto & be = keyPair.second;
			auto newX = m_Size.z - be->GetPosZ() - 1;
			auto newY = be->GetPosY();
			auto newZ = be->GetPosX();
			auto newIdx = newX + newZ * m_Size.z + newY * m_Size.x * m_Size.z;
			be->SetPos({newX, newY, newZ});
			m_BlockEntities->emplace(newIdx, std::move(be));
		}
	}

	std::swap(m_Size.x, m_Size.z);
}





void cBlockArea::MirrorXY(void)
{
	if (!HasBlocks())
	{
		LOGWARNING("cBlockArea: Cannot mirror meta without blocktypes!");
		return;
	}

	// We are guaranteed that both blocktypes and blockmetas exist; mirror both at the same time:
	int HalfZ = m_Size.z / 2;
	int MaxZ = m_Size.z - 1;
	for (int y = 0; y < m_Size.y; y++)
	{
		for (int z = 0; z < HalfZ; z++)
		{
			for (int x = 0; x < m_Size.x; x++)
			{
				auto Idx1 = MakeIndex(x, y, z);
				auto Idx2 = MakeIndex(x, y, MaxZ - z);
				auto Block2 = cBlockHandler::For(m_Blocks[Idx2].Type()).MirrorXY(m_Blocks[Idx2]);
				auto Block1 = cBlockHandler::For(m_Blocks[Idx1].Type()).MirrorXY(m_Blocks[Idx1]);
				m_Blocks[Idx1] = Block2;
				m_Blocks[Idx2] = Block1;
			}  // for x
		}  // for z
	}  // for y

	// Mirror the BlockEntities:
	if (HasBlockEntities())
	{
		cBlockEntities oldBE;
		std::swap(oldBE, *m_BlockEntities);
		for (auto & keyPair: oldBE)
		{
			auto & be = keyPair.second;
			auto newX = be->GetPosX();
			auto newY = be->GetPosY();
			auto newZ = MaxZ - be->GetPosZ();
			auto newIdx = MakeIndex(newX, newY, newZ);
			be->SetPos({newX, newY, newZ});
			m_BlockEntities->emplace(newIdx, std::move(be));
		}
	}
}





void cBlockArea::MirrorXZ(void)
{
	if (!HasBlocks())
	{
		LOGWARNING("cBlockArea: Cannot mirror meta without blocktypes!");
		return;
	}

	// We are guaranteed that both blocktypes and blockmetas exist; mirror both at the same time:
	int HalfY = m_Size.y / 2;
	int MaxY = m_Size.y - 1;
	for (int y = 0; y < HalfY; y++)
	{
		for (int z = 0; z < m_Size.z; z++)
		{
			for (int x = 0; x < m_Size.x; x++)
			{
				auto Idx1 = MakeIndex(x, y, z);
				auto Idx2 = MakeIndex(x, MaxY - y, z);
				auto Block2 = cBlockHandler::For(m_Blocks[Idx2].Type()).MirrorXZ(m_Blocks[Idx2]);
				auto Block1 = cBlockHandler::For(m_Blocks[Idx1].Type()).MirrorXZ(m_Blocks[Idx1]);
				m_Blocks[Idx1] = Block2;
				m_Blocks[Idx2] = Block1;
			}  // for x
		}  // for z
	}  // for y

	// Mirror the BlockEntities:
	if (HasBlockEntities())
	{
		cBlockEntities oldBE;
		std::swap(oldBE, *m_BlockEntities);
		for (auto & keyPair: oldBE)
		{
			auto & be = keyPair.second;
			auto newX = be->GetPosX();
			auto newY = MaxY - be->GetPosY();
			auto newZ = be->GetPosZ();
			auto newIdx = MakeIndex(newX, newY, newZ);
			be->SetPos({newX, newY, newZ});
			m_BlockEntities->emplace(newIdx, std::move(be));
		}
	}
}





void cBlockArea::MirrorYZ(void)
{
	if (!HasBlocks())
	{
		LOGWARNING("cBlockArea: Cannot mirror meta without blocktypes!");
		return;
	}

	// We are guaranteed that both blocktypes and blockmetas exist; mirror both at the same time:
	int HalfX = m_Size.x / 2;
	int MaxX = m_Size.x - 1;
	for (int y = 0; y < m_Size.y; y++)
	{
		for (int z = 0; z < m_Size.z; z++)
		{
			for (int x = 0; x < HalfX; x++)
			{
				auto Idx1 = MakeIndex(x, y, z);
				auto Idx2 = MakeIndex(MaxX - x, y, z);
				auto Block2 = cBlockHandler::For(m_Blocks[Idx2].Type()).MirrorXZ(m_Blocks[Idx2]);
				auto Block1 = cBlockHandler::For(m_Blocks[Idx1].Type()).MirrorXZ(m_Blocks[Idx1]);
				m_Blocks[Idx1] = Block2;
				m_Blocks[Idx2] = Block1;
			}  // for x
		}  // for z
	}  // for y

	// Mirror the BlockEntities:
	if (HasBlockEntities())
	{
		cBlockEntities oldBE;
		std::swap(oldBE, *m_BlockEntities);
		for (auto & keyPair: oldBE)
		{
			auto & be = keyPair.second;
			auto newX = MaxX - be->GetPosX();
			auto newY = be->GetPosY();
			auto newZ = be->GetPosZ();
			auto newIdx = MakeIndex(newX, newY, newZ);
			be->SetPos({newX, newY, newZ});
			m_BlockEntities->emplace(newIdx, std::move(be));
		}
	}
}





void cBlockArea::SetRelBlock(Vector3i a_RelPos, BlockState a_Block)
{
	ASSERT(m_Blocks != nullptr);
	auto idx = MakeIndex(a_RelPos);
	m_Blocks[idx] = a_Block;

	// Update the block entities, if appropriate:
	if (HasBlockEntities())
	{
		auto itr = m_BlockEntities->find(idx);
		if (itr != m_BlockEntities->end())
		{
			if (itr->second->GetBlockType() == a_Block.Type())
			{
				// The block entity is for the same block type, keep the current one
				return;
			}
			m_BlockEntities->erase(itr);
		}
		if (cBlockEntity::IsBlockEntityBlockType(a_Block))
		{
			m_BlockEntities->emplace(idx, cBlockEntity::CreateByBlockType(a_Block, a_RelPos));
		}
	}
}





void cBlockArea::SetBlock(Vector3i a_Pos, BlockState a_BlockType)
{
	SetRelBlock(a_Pos - m_Origin, a_BlockType);
}





void cBlockArea::SetRelBlockLight(Vector3i a_RelPos, LIGHTTYPE a_BlockLight)
{
	SetRelLightValue(a_RelPos, a_BlockLight, GetBlockLight());
}





void cBlockArea::SetBlockLight(Vector3i a_Pos, LIGHTTYPE a_BlockLight)
{
	SetLightValue(a_Pos, a_BlockLight, GetBlockLight());
}





void cBlockArea::SetRelBlockSkyLight(Vector3i a_RelPos, LIGHTTYPE a_BlockSkyLight)
{
	SetRelLightValue(a_RelPos, a_BlockSkyLight, GetBlockSkyLight());
}





void cBlockArea::SetBlockSkyLight(Vector3i a_Pos, LIGHTTYPE a_BlockSkyLight)
{
	SetLightValue(a_Pos, a_BlockSkyLight, GetBlockSkyLight());
}





BlockState cBlockArea::GetRelBlock(Vector3i a_RelPos) const
{
	if (m_Blocks == nullptr)
	{
		LOGWARNING("cBlockArea: BlockTypes have not been read!");
		return Block::Air::Air();
	}
	return m_Blocks[MakeIndex(a_RelPos)];
}





BlockState cBlockArea::GetBlock(Vector3i a_Pos) const
{
	return GetRelBlock(a_Pos - m_Origin);
}





LIGHTTYPE cBlockArea::GetRelBlockLight(Vector3i a_RelPos) const
{
	return GetRelLightValue(a_RelPos, GetBlockLight());
}





LIGHTTYPE cBlockArea::GetBlockLight(Vector3i a_Pos) const
{
	return GetLightValue(a_Pos, GetBlockLight());
}





LIGHTTYPE cBlockArea::GetRelBlockSkyLight(Vector3i a_RelPos) const
{
	return GetRelLightValue(a_RelPos, GetBlockSkyLight());
}





LIGHTTYPE cBlockArea::GetBlockSkyLight(Vector3i a_Pos) const
{
	return GetLightValue(a_Pos, GetBlockSkyLight());
}





cCuboid cBlockArea::GetBounds(void) const
{
	return cCuboid(
		m_Origin,
		m_Origin + m_Size - Vector3i(1, 1, 1)
	);
}





size_t cBlockArea::CountNonAirBlocks(void) const
{
	// Check if blocktypes are valid:
	if (m_Blocks == nullptr)
	{
		LOGWARNING("%s: BlockTypes have not been read!", __FUNCTION__);
		return 0;
	}

	// Count the blocks:
	size_t res = 0;
	for (int y = 0; y < m_Size.y; y++)
	{
		for (int z = 0; z < m_Size.z; z++)
		{
			for (int x = 0; x < m_Size.x; x++)
			{
				if (!cBlockAirHandler::IsBlockAir(m_Blocks[MakeIndex(x, y, z)]))
				{
					++res;
				}
			}  // for x
		}  // for z
	}  // for y
	return res;
}





size_t cBlockArea::CountSpecificBlocks(BlockType a_Block) const
{
	// If blocktypes are not valid, log a warning and return zero occurences:
	if (m_Blocks == nullptr)
	{
		LOGWARNING("%s: BlockTypes not available!", __FUNCTION__);
		return 0;
	}

	// Count the blocks:
	size_t num = GetBlockCount();
	size_t res = 0;
	for (size_t i = 0; i < num; i++)
	{
		if (m_Blocks[i].Type() == a_Block)
		{
			res++;
		}
	}
	return res;
}





void cBlockArea::GetNonAirCropRelCoords(int & a_MinRelX, int & a_MinRelY, int & a_MinRelZ, int & a_MaxRelX, int & a_MaxRelY, int & a_MaxRelZ, BlockType a_IgnoreBlockType)
{
	// Check if blocktypes are valid:
	if (m_Blocks == nullptr)
	{
		LOGWARNING("%s: BlockTypes have not been read!", __FUNCTION__);
		a_MinRelX = 1;
		a_MaxRelX = 0;
		return;
	}

	// Walk all the blocks and find the min and max coords for the non-ignored ones:
	int MaxX = 0, MinX = m_Size.x - 1;
	int MaxY = 0, MinY = m_Size.y - 1;
	int MaxZ = 0, MinZ = m_Size.z - 1;
	for (int y = 0; y < m_Size.y; y++)
	{
		for (int z = 0; z < m_Size.z; z++)
		{
			for (int x = 0; x < m_Size.x; x++)
			{
				if (m_Blocks[MakeIndex(x, y, z)] == a_IgnoreBlockType)
				{
					continue;
				}
				// The block is not ignored, update any coords that need updating:
				if (x < MinX)
				{
					MinX = x;
				}
				if (x > MaxX)
				{
					MaxX = x;
				}
				if (y < MinY)
				{
					MinY = y;
				}
				if (y > MaxY)
				{
					MaxY = y;
				}
				if (z < MinZ)
				{
					MinZ = z;
				}
				if (z > MaxZ)
				{
					MaxZ = z;
				}
			}  // for x
		}  // for z
	}  // for y

	// Assign to the output:
	a_MinRelX = MinX;
	a_MinRelY = MinY;
	a_MinRelZ = MinZ;
	a_MaxRelX = MaxX;
	a_MaxRelY = MaxY;
	a_MaxRelZ = MaxZ;
}





int cBlockArea::GetDataTypes(void) const
{
	int res = 0;
	if (m_Blocks != nullptr)
	{
		res |= baBlocks;
	}
	if (m_BlockLight != nullptr)
	{
		res |= baLight;
	}
	if (m_BlockSkyLight != nullptr)
	{
		res |= baSkyLight;
	}
	if (m_BlockEntities != nullptr)
	{
		res |= baBlockEntities;
	}
	return res;
}





bool cBlockArea::SetSize(int a_SizeX, int a_SizeY, int a_SizeZ, int a_DataTypes)
{
	ASSERT(IsValidDataTypeCombination(a_DataTypes));

	BLOCKARRAY NewBlocks;
	LIGHTARRAY NewLight;
	LIGHTARRAY NewSkyLight;
	cBlockEntitiesPtr NewBlockEntities;

	// Try to allocate the new storage
	if ((a_DataTypes & baBlocks) != 0)
	{
		NewBlocks.reset(new BlockState[ToUnsigned(a_SizeX * a_SizeY * a_SizeZ)]);
		if (NewBlocks == nullptr)
		{
			return false;
		}
	}
	if ((a_DataTypes & baLight) != 0)
	{
		NewLight.reset(new LIGHTTYPE[ToUnsigned(a_SizeX * a_SizeY * a_SizeZ)]);
		if (NewLight == nullptr)
		{
			return false;
		}
	}
	if ((a_DataTypes & baSkyLight) != 0)
	{
		NewSkyLight.reset(new LIGHTTYPE[ToUnsigned(a_SizeX * a_SizeY * a_SizeZ)]);
		if (NewSkyLight == nullptr)
		{
			return false;
		}
	}
	if ((a_DataTypes & baBlockEntities) != 0)
	{
		NewBlockEntities.reset(new cBlockEntities);
		if (NewBlockEntities == nullptr)
		{
			return false;
		}
	}

	// Commit changes
	m_Blocks = std::move(NewBlocks);
	m_BlockLight = std::move(NewLight);
	m_BlockSkyLight = std::move(NewSkyLight);
	m_BlockEntities = std::move(NewBlockEntities);
	m_Size.Set(a_SizeX, a_SizeY, a_SizeZ);
	return true;
}





size_t cBlockArea::MakeIndexForSize(Vector3i a_RelPos, Vector3i a_Size)
{
	ASSERT(a_RelPos.x >= 0);
	ASSERT(a_RelPos.x < a_Size.x);
	ASSERT(a_RelPos.y >= 0);
	ASSERT(a_RelPos.y < a_Size.y);
	ASSERT(a_RelPos.z >= 0);
	ASSERT(a_RelPos.z < a_Size.z);

	return static_cast<size_t>(a_RelPos.x + a_RelPos.z * a_Size.x + a_RelPos.y * a_Size.x * a_Size.z);
}





bool cBlockArea::DoWithBlockEntityRelAt(Vector3i a_RelPos, cBlockEntityCallback a_Callback)
{
	ASSERT(IsValidRelCoords(a_RelPos));
	if (!HasBlockEntities())
	{
		return false;
	}
	auto idx = MakeIndex(a_RelPos);
	auto itr = m_BlockEntities->find(idx);
	if (itr == m_BlockEntities->end())
	{
		return false;
	}
	return a_Callback(*itr->second);
}





bool cBlockArea::DoWithBlockEntityAt(Vector3i a_Pos, cBlockEntityCallback a_Callback)
{
	return DoWithBlockEntityRelAt(a_Pos - m_Origin, a_Callback);
}





bool cBlockArea::ForEachBlockEntity(cBlockEntityCallback a_Callback)
{
	if (!HasBlockEntities())
	{
		return true;
	}
	for (auto & keyPair: *m_BlockEntities)
	{
		if (a_Callback(*keyPair.second))
		{
			return false;
		}
	}
	return true;
}





void cBlockArea::SetRelLightValue(Vector3i a_RelPos, LIGHTTYPE a_Value, LIGHTTYPE * a_Array)
{
	if (a_Array == nullptr)
	{
		LOGWARNING("cBlockArea: datatype has not been read!");
		return;
	}
	a_Array[MakeIndex(a_RelPos)] = a_Value;
}





void cBlockArea::SetLightValue(Vector3i a_Pos, LIGHTTYPE a_Value, LIGHTTYPE * a_Array)
{
	SetRelLightValue(a_Pos - m_Origin, a_Value, a_Array);
}





LIGHTTYPE cBlockArea::GetRelLightValue(Vector3i a_RelPos, LIGHTTYPE * a_Array) const
{
	if (a_Array == nullptr)
	{
		LOGWARNING("cBlockArea: datatype has not been read!");
		return 16;
	}
	return a_Array[MakeIndex(a_RelPos)];
}





LIGHTTYPE cBlockArea::GetLightValue(Vector3i a_Pos, LIGHTTYPE * a_Array) const
{
	return GetRelLightValue(a_Pos - m_Origin, a_Array);
}





void cBlockArea::CropBlocks(int a_AddMinX, int a_SubMaxX, int a_AddMinY, int a_SubMaxY, int a_AddMinZ, int a_SubMaxZ)
{
	int NewSizeX = GetSizeX() - a_AddMinX - a_SubMaxX;
	int NewSizeY = GetSizeY() - a_AddMinY - a_SubMaxY;
	int NewSizeZ = GetSizeZ() - a_AddMinZ - a_SubMaxZ;
	BLOCKARRAY NewBlocks{ new BlockState[ToUnsigned(NewSizeX * NewSizeY * NewSizeZ)] };
	size_t idx = 0;
	for (int y = 0; y < NewSizeY; y++)
	{
		for (int z = 0; z < NewSizeZ; z++)
		{
			for (int x = 0; x < NewSizeX; x++)
			{
				auto OldIndex = MakeIndex(x + a_AddMinX, y + a_AddMinY, z + a_AddMinZ);
				NewBlocks[idx++] = m_Blocks[OldIndex];
			}  // for x
		}  // for z
	}  // for y
	m_Blocks = std::move(NewBlocks);
}





void cBlockArea::CropLightValues(LIGHTARRAY & a_Array, int a_AddMinX, int a_SubMaxX, int a_AddMinY, int a_SubMaxY, int a_AddMinZ, int a_SubMaxZ)
{
	int NewSizeX = GetSizeX() - a_AddMinX - a_SubMaxX;
	int NewSizeY = GetSizeY() - a_AddMinY - a_SubMaxY;
	int NewSizeZ = GetSizeZ() - a_AddMinZ - a_SubMaxZ;
	LIGHTARRAY NewLightValues{ new LIGHTTYPE[ToUnsigned(NewSizeX * NewSizeY * NewSizeZ)] };
	size_t idx = 0;
	for (int y = 0; y < NewSizeY; y++)
	{
		for (int z = 0; z < NewSizeZ; z++)
		{
			for (int x = 0; x < NewSizeX; x++)
			{
				NewLightValues[idx++] = a_Array[MakeIndex(x + a_AddMinX, y + a_AddMinY, z + a_AddMinZ)];
			}  // for x
		}  // for z
	}  // for y
	a_Array = std::move(NewLightValues);
}





void cBlockArea::ExpandBlocks(int a_SubMinX, int a_AddMaxX, int a_SubMinY, int a_AddMaxY, int a_SubMinZ, int a_AddMaxZ)
{
	int NewSizeX = m_Size.x + a_SubMinX + a_AddMaxX;
	int NewSizeY = m_Size.y + a_SubMinY + a_AddMaxY;
	int NewSizeZ = m_Size.z + a_SubMinZ + a_AddMaxZ;
	size_t BlockCount = static_cast<size_t>(NewSizeX * NewSizeY * NewSizeZ);
	BLOCKARRAY NewBlocks{ new BlockState[BlockCount] };
	memset(NewBlocks.get(), 0, BlockCount * sizeof(NewBlocks[0]));
	size_t OldIndex = 0;
	for (int y = 0; y < m_Size.y; y++)
	{
		int IndexBaseY = (y + a_SubMinY) * m_Size.x * m_Size.z;
		for (int z = 0; z < m_Size.z; z++)
		{
			int IndexBaseZ = IndexBaseY + (z + a_SubMinZ) * m_Size.x;
			auto idx = static_cast<size_t>(IndexBaseZ + a_SubMinX);
			for (int x = 0; x < m_Size.x; x++)
			{
				NewBlocks[idx++] = m_Blocks[OldIndex++];
			}  // for x
		}  // for z
	}  // for y
	m_Blocks = std::move(NewBlocks);
}





void cBlockArea::ExpandLightValues(LIGHTARRAY & a_Array, int a_SubMinX, int a_AddMaxX, int a_SubMinY, int a_AddMaxY, int a_SubMinZ, int a_AddMaxZ)
{
	int NewSizeX = m_Size.x + a_SubMinX + a_AddMaxX;
	int NewSizeY = m_Size.y + a_SubMinY + a_AddMaxY;
	int NewSizeZ = m_Size.z + a_SubMinZ + a_AddMaxZ;
	size_t BlockCount = static_cast<size_t>(NewSizeX * NewSizeY * NewSizeZ);
	LIGHTARRAY NewLightValues{ new LIGHTTYPE [BlockCount] };
	memset(NewLightValues.get(), 0, BlockCount * sizeof(LIGHTTYPE));
	size_t OldIndex = 0;
	for (int y = 0; y < m_Size.y; y++)
	{
		int IndexBaseY = (y + a_SubMinY) * m_Size.x * m_Size.z;
		for (int z = 0; z < m_Size.z; z++)
		{
			int IndexBaseZ = IndexBaseY + (z + a_SubMinZ) * m_Size.x;
			auto idx = static_cast<size_t>(IndexBaseZ + a_SubMinX);
			for (int x = 0; x < m_Size.x; x++)
			{
				NewLightValues[idx++] = a_Array[OldIndex++];
			}  // for x
		}  // for z
	}  // for y
	a_Array = std::move(NewLightValues);
}





void cBlockArea::RelSetData(
	Vector3i a_RelPos,
	int a_DataTypes, BlockState a_Block,
	LIGHTTYPE a_BlockLight, LIGHTTYPE a_BlockSkyLight
)
{
	if (!IsValidCoords(a_RelPos))
	{
		return;
	}

	auto Index = MakeIndex(a_RelPos);
	if ((a_DataTypes & baBlocks) != 0)
	{
		m_Blocks[Index] = a_Block;
	}
	if ((a_DataTypes & baLight) != 0)
	{
		m_BlockLight[Index] = a_BlockLight;
	}
	if ((a_DataTypes & baSkyLight) != 0)
	{
		m_BlockSkyLight[Index] = a_BlockSkyLight;
	}

	// Update the block entities, if appropriate:
	if (HasBlockEntities())
	{
		auto itr = m_BlockEntities->find(Index);
		if (itr != m_BlockEntities->end())
		{
			if (itr->second->GetBlockType() == a_Block.Type())
			{
				// The block entity is for the same block type, keep the current one
				return;
			}
			// The block entity is for a different block type, remove it:
			m_BlockEntities->erase(itr);
		}
		if (cBlockEntity::IsBlockEntityBlockType(a_Block))
		{
			// The block type should have a block entity attached to it, create an empty one:
			m_BlockEntities->emplace(Index, cBlockEntity::CreateByBlockType(a_Block, {a_RelPos}));
		}
	}
}





void cBlockArea::MergeByStrategy(const cBlockArea & a_Src, Vector3i a_RelPos, eMergeStrategy a_Strategy)
{
	// Block types are compulsory, block metas are optional
	if (!HasBlocks() || !a_Src.HasBlocks())
	{
		LOGWARNING("%s: cannot merge because one of the areas doesn't have blocktypes.", __FUNCTION__);
		return;
	}

	// Dst is *this, Src is a_Src
	int SrcOffX = std::max(0, -a_RelPos.x);  // Offset in Src where to start reading
	int DstOffX = std::max(0,  a_RelPos.x);  // Offset in Dst where to start writing
	int SizeX   = std::min(a_Src.GetSizeX() - SrcOffX, GetSizeX() - DstOffX);  // How many blocks to copy

	int SrcOffY = std::max(0, -a_RelPos.y);  // Offset in Src where to start reading
	int DstOffY = std::max(0,  a_RelPos.y);  // Offset in Dst where to start writing
	int SizeY   = std::min(a_Src.GetSizeY() - SrcOffY, GetSizeY() - DstOffY);  // How many blocks to copy

	int SrcOffZ = std::max(0, -a_RelPos.z);  // Offset in Src where to start reading
	int DstOffZ = std::max(0,  a_RelPos.z);  // Offset in Dst where to start writing
	int SizeZ   = std::min(a_Src.GetSizeZ() - SrcOffZ, GetSizeZ() - DstOffZ);  // How many blocks to copy

	[&]
	{
		switch (a_Strategy)
		{
			case cBlockArea::msOverwrite:
			{
				InternalMergeBlocks<MergeCombinatorOverwrite>(
					GetBlocks(), a_Src.GetBlocks(),
					SizeX, SizeY, SizeZ,
					SrcOffX, SrcOffY, SrcOffZ,
					DstOffX, DstOffY, DstOffZ,
					a_Src.GetSizeX(), a_Src.GetSizeY(), a_Src.GetSizeZ(),
					m_Size.x, m_Size.y, m_Size.z
				);
				return;
			}  // case msOverwrite

			case cBlockArea::msFillAir:
			{
				InternalMergeBlocks<MergeCombinatorFillAir>(
					GetBlocks(), a_Src.GetBlocks(),
					SizeX, SizeY, SizeZ,
					SrcOffX, SrcOffY, SrcOffZ,
					DstOffX, DstOffY, DstOffZ,
					a_Src.GetSizeX(), a_Src.GetSizeY(), a_Src.GetSizeZ(),
					m_Size.x, m_Size.y, m_Size.z
				);
				return;
			}  // case msFillAir

			case cBlockArea::msImprint:
			{
				InternalMergeBlocks<MergeCombinatorImprint>(
					GetBlocks(), a_Src.GetBlocks(),
					SizeX, SizeY, SizeZ,
					SrcOffX, SrcOffY, SrcOffZ,
					DstOffX, DstOffY, DstOffZ,
					a_Src.GetSizeX(), a_Src.GetSizeY(), a_Src.GetSizeZ(),
					m_Size.x, m_Size.y, m_Size.z
				);
				return;
			}  // case msImprint

			case cBlockArea::msLake:
			{
				InternalMergeBlocks<MergeCombinatorLake>(
					GetBlocks(), a_Src.GetBlocks(),
					SizeX, SizeY, SizeZ,
					SrcOffX, SrcOffY, SrcOffZ,
					DstOffX, DstOffY, DstOffZ,
					a_Src.GetSizeX(), a_Src.GetSizeY(), a_Src.GetSizeZ(),
					m_Size.x, m_Size.y, m_Size.z
				);
				return;
			}  // case msLake

			case cBlockArea::msSpongePrint:
			{
				InternalMergeBlocks<MergeCombinatorSpongePrint>(
					GetBlocks(), a_Src.GetBlocks(),
					SizeX, SizeY, SizeZ,
					SrcOffX, SrcOffY, SrcOffZ,
					DstOffX, DstOffY, DstOffZ,
					a_Src.GetSizeX(), a_Src.GetSizeY(), a_Src.GetSizeZ(),
					m_Size.x, m_Size.y, m_Size.z
				);
				return;
			}  // case msSpongePrint

			case cBlockArea::msDifference:
			{
				InternalMergeBlocks<MergeCombinatorDifference>(
					GetBlocks(), a_Src.GetBlocks(),
					SizeX, SizeY, SizeZ,
					SrcOffX, SrcOffY, SrcOffZ,
					DstOffX, DstOffY, DstOffZ,
					a_Src.GetSizeX(), a_Src.GetSizeY(), a_Src.GetSizeZ(),
					m_Size.x, m_Size.y, m_Size.z
				);
				return;
			}  // case msDifference

			case cBlockArea::msSimpleCompare:
			{
				InternalMergeBlocks<MergeCombinatorSimpleCompare>(
					GetBlocks(), a_Src.GetBlocks(),
					SizeX, SizeY, SizeZ,
					SrcOffX, SrcOffY, SrcOffZ,
					DstOffX, DstOffY, DstOffZ,
					a_Src.GetSizeX(), a_Src.GetSizeY(), a_Src.GetSizeZ(),
					m_Size.x, m_Size.y, m_Size.z
				);
				return;
			}  // case msSimpleCompare

			case cBlockArea::msMask:
			{
				InternalMergeBlocks<MergeCombinatorMask>(
					GetBlocks(), a_Src.GetBlocks(),
					SizeX, SizeY, SizeZ,
					SrcOffX, SrcOffY, SrcOffZ,
					DstOffX, DstOffY, DstOffZ,
					a_Src.GetSizeX(), a_Src.GetSizeY(), a_Src.GetSizeZ(),
					m_Size.x, m_Size.y, m_Size.z
				);
				return;
			}  // case msMask
		}  // switch (a_Strategy)
		UNREACHABLE("Unsupported block area merge strategy");
	}();

	if (HasBlockEntities())
	{
		if (a_Src.HasBlockEntities())
		{
			MergeBlockEntities(a_RelPos, a_Src);
		}
		else
		{
			RescanBlockEntities();
		}
	}
}





void cBlockArea::MergeBlockEntities(Vector3i a_RelPos, const cBlockArea & a_Src)
{
	// Only supported with both BlockEntities and BlockTypes (caller should check):
	ASSERT(HasBlocks());
	ASSERT(HasBlockEntities());
	ASSERT(a_Src.HasBlocks());
	ASSERT(a_Src.HasBlockEntities());

	// Remove block entities that no longer match the block at their coords:
	RemoveNonMatchingBlockEntities();

	// Clone BEs from a_Src wherever a BE is missing:
	for (int y = 0; y < m_Size.y; ++y) for (int z = 0; z < m_Size.z; ++z) for (int x = 0; x < m_Size.x; ++x)
	{
		auto idx = MakeIndex(x, y, z);
		auto Block = m_Blocks[idx];
		if (!cBlockEntity::IsBlockEntityBlockType(Block))
		{
			continue;
		}

		// This block should have a block entity, check that there is one:
		auto itr = m_BlockEntities->find(idx);
		if (itr != m_BlockEntities->end())
		{
			// There is one already
			continue;
		}

		// Copy a BE from a_Src, if it exists there:
		auto srcX = x + a_RelPos.x;
		auto srcY = y + a_RelPos.y;
		auto srcZ = z + a_RelPos.z;
		if (a_Src.IsValidRelCoords({srcX, srcY, srcZ}))
		{
			auto srcIdx = a_Src.MakeIndex(srcX, srcY, srcZ);
			auto itrSrc = a_Src.m_BlockEntities->find(srcIdx);
			if (itrSrc != a_Src.m_BlockEntities->end())
			{
				m_BlockEntities->emplace(idx, itrSrc->second->Clone({x, y, z}));
				continue;
			}
		}
		// No BE found in a_Src, insert a new empty one:
		m_BlockEntities->emplace(idx, cBlockEntity::CreateByBlockType(Block, {x, y, z}));
	}  // for x, z, y
}





void cBlockArea::RescanBlockEntities(void)
{
	// Only supported with both BlockEntities and BlockTypes
	if (!HasBlockEntities() || !HasBlocks())
	{
		return;
	}

	// Remove block entities that no longer match the block at their coords:
	RemoveNonMatchingBlockEntities();

	// Add block entities for all block types that should have a BE assigned to them:
	for (int y = 0; y < m_Size.y; ++y) for (int z = 0; z < m_Size.z; ++z) for (int x = 0; x < m_Size.x; ++x)
	{
		auto idx = MakeIndex(x, y, z);
		auto Block = m_Blocks[idx];
		if (!cBlockEntity::IsBlockEntityBlockType(Block))
		{
			continue;
		}
		// This block should have a block entity, check that there is one:
		auto itr = m_BlockEntities->find(idx);
		if (itr != m_BlockEntities->end())
		{
			continue;
		}
		// Create a new BE for this block:
		m_BlockEntities->emplace(idx, cBlockEntity::CreateByBlockType(Block, {x, y, z}));
	}  // for x, z, y
}





void cBlockArea::RemoveNonMatchingBlockEntities(void)
{
	// Only supported with both BlockEntities and BlockTypes:
	ASSERT(HasBlocks());
	ASSERT(HasBlockEntities());

	cBlockEntities oldBE;
	std::swap(oldBE, *m_BlockEntities);
	for (auto & keyPair: oldBE)
	{
		auto Block = m_Blocks[static_cast<size_t>(keyPair.first)];
		if (Block == keyPair.second->GetBlockType())
		{
			m_BlockEntities->insert(std::move(keyPair));
		}
	}
}





cBlockEntity * cBlockArea::GetBlockEntityRel(Vector3i a_RelPos)
{
	if (!HasBlockEntities())
	{
		return nullptr;
	}
	auto itr = m_BlockEntities->find(MakeIndex(a_RelPos));
	return (itr == m_BlockEntities->end()) ? nullptr : itr->second.get();
}





////////////////////////////////////////////////////////////////////////////////
// cBlockArea::cChunkReader:

cBlockArea::cChunkReader::cChunkReader(cBlockArea & a_Area) :
	m_Area(a_Area),
	m_AreaBounds(cCuboid(a_Area.GetOrigin(), a_Area.GetOrigin() + a_Area.GetSize() - Vector3i(1, 1, 1))),
	m_Origin(a_Area.m_Origin.x, a_Area.m_Origin.y, a_Area.m_Origin.z),
	m_CurrentChunkX(0),
	m_CurrentChunkZ(0)
{
}





bool cBlockArea::cChunkReader::Coords(int a_ChunkX, int a_ChunkZ)
{
	m_CurrentChunkX = a_ChunkX;
	m_CurrentChunkZ = a_ChunkZ;
	return true;
}





void cBlockArea::cChunkReader::ChunkData(const ChunkBlockData & a_BlockData, const ChunkLightData & a_LightData)
{
	int SizeY = m_Area.m_Size.y;
	int MinY = m_Origin.y;

	// SizeX, SizeZ are the dimensions of the block data to copy from the current chunk (size of the geometric union)
	// OffX, OffZ are the offsets of the current chunk data from the area origin
	// BaseX, BaseZ are the offsets of the area data within the current chunk from the chunk borders
	int SizeX = cChunkDef::Width;
	int SizeZ = cChunkDef::Width;
	int OffX, OffZ;
	int BaseX, BaseZ;
	OffX = m_CurrentChunkX * cChunkDef::Width - m_Origin.x;
	if (OffX < 0)
	{
		BaseX = -OffX;
		SizeX += OffX;  // SizeX is decreased, OffX is negative
		OffX = 0;
	}
	else
	{
		BaseX = 0;
	}
	OffZ = m_CurrentChunkZ * cChunkDef::Width - m_Origin.z;
	if (OffZ < 0)
	{
		BaseZ = -OffZ;
		SizeZ += OffZ;  // SizeZ is decreased, OffZ is negative
		OffZ = 0;
	}
	else
	{
		BaseZ = 0;
	}
	// If the chunk extends beyond the area in the X or Z axis, cut off the Size:
	if ((m_CurrentChunkX + 1) * cChunkDef::Width > m_Origin.x + m_Area.m_Size.x)
	{
		SizeX -= (m_CurrentChunkX + 1) * cChunkDef::Width - (m_Origin.x + m_Area.m_Size.x);
	}
	if ((m_CurrentChunkZ + 1) * cChunkDef::Width > m_Origin.z + m_Area.m_Size.z)
	{
		SizeZ -= (m_CurrentChunkZ + 1) * cChunkDef::Width - (m_Origin.z + m_Area.m_Size.z);
	}

	// Copy the blocktypes:
	if (m_Area.m_Blocks != nullptr)
	{
		for (int y = 0; y < SizeY; y++)
		{
			int InChunkY = MinY + y;
			int AreaY = y;
			for (int z = 0; z < SizeZ; z++)
			{
				int InChunkZ = BaseZ + z;
				int AreaZ = OffZ + z;
				for (int x = 0; x < SizeX; x++)
				{
					int InChunkX = BaseX + x;
					int AreaX = OffX + x;
					m_Area.m_Blocks[m_Area.MakeIndex(AreaX, AreaY, AreaZ)] = a_BlockData.GetBlock({ InChunkX, InChunkY, InChunkZ });
				}  // for x
			}  // for z
		}  // for y
	}

	// Copy the blocklight:
	if (m_Area.m_BlockLight != nullptr)
	{
		for (int y = 0; y < SizeY; y++)
		{
			int InChunkY = MinY + y;
			int AreaY = y;
			for (int z = 0; z < SizeZ; z++)
			{
				int InChunkZ = BaseZ + z;
				int AreaZ = OffZ + z;
				for (int x = 0; x < SizeX; x++)
				{
					int InChunkX = BaseX + x;
					int AreaX = OffX + x;
					m_Area.m_BlockLight[m_Area.MakeIndex(AreaX, AreaY, AreaZ)] = a_LightData.GetBlockLight({ InChunkX, InChunkY, InChunkZ });
				}  // for x
			}  // for z
		}  // for y
	}

	// Copy the skylight:
	if (m_Area.m_BlockSkyLight != nullptr)
	{
		for (int y = 0; y < SizeY; y++)
		{
			int InChunkY = MinY + y;
			int AreaY = y;
			for (int z = 0; z < SizeZ; z++)
			{
				int InChunkZ = BaseZ + z;
				int AreaZ = OffZ + z;
				for (int x = 0; x < SizeX; x++)
				{
					int InChunkX = BaseX + x;
					int AreaX = OffX + x;
					m_Area.m_BlockSkyLight[m_Area.MakeIndex(AreaX, AreaY, AreaZ)] = a_LightData.GetSkyLight({ InChunkX, InChunkY, InChunkZ });
				}  // for x
			}  // for z
		}  // for y
	}
}





void cBlockArea::cChunkReader::BlockEntity(cBlockEntity * a_BlockEntity)
{
	if (!m_Area.HasBlockEntities())
	{
		return;
	}
	if (!m_AreaBounds.IsInside(a_BlockEntity->GetPos()))
	{
		return;
	}
	auto areaPos = a_BlockEntity->GetPos() - m_Area.m_Origin;
	auto Idx = m_Area.MakeIndex(areaPos);
	m_Area.m_BlockEntities->emplace(Idx, a_BlockEntity->Clone(areaPos));
}





