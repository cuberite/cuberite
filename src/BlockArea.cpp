
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





// Disable MSVC warnings: "conditional expression is constant"
#ifdef _MSC_VER
	#pragma warning(push)
	#pragma warning(disable:4127)
#endif





typedef void (CombinatorFunc)(BLOCKTYPE & a_DstType, BLOCKTYPE a_SrcType, NIBBLETYPE & a_DstMeta, NIBBLETYPE a_SrcMeta);

/** Merges two blocktypes and blockmetas of the specified sizes and offsets using the specified combinator function
This wild construct allows us to pass a function argument and still have it inlined by the compiler. */
template <bool MetasValid, CombinatorFunc Combinator>
void InternalMergeBlocks(
	BLOCKTYPE * a_DstTypes, const BLOCKTYPE * a_SrcTypes,
	NIBBLETYPE * a_DstMetas, const NIBBLETYPE * a_SrcMetas,
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
				if (MetasValid)
				{
					Combinator(a_DstTypes[DstIdx], a_SrcTypes[SrcIdx], a_DstMetas[DstIdx], a_SrcMetas[SrcIdx]);
				}
				else
				{
					NIBBLETYPE FakeDestMeta = 0;
					Combinator(a_DstTypes[DstIdx], a_SrcTypes[SrcIdx], FakeDestMeta, static_cast<NIBBLETYPE>(0));
				}
				++DstIdx;
				++SrcIdx;
			}  // for x
		}  // for z
	}  // for y
}





/** Combinator used for cBlockArea::msOverwrite merging */
template <bool MetaValid>
void MergeCombinatorOverwrite(BLOCKTYPE & a_DstType, BLOCKTYPE a_SrcType, NIBBLETYPE & a_DstMeta, NIBBLETYPE a_SrcMeta)
{
	a_DstType = a_SrcType;
	if (MetaValid)
	{
		a_DstMeta = a_SrcMeta;
	}
}





/** Combinator used for cBlockArea::msFillAir merging */
template <bool MetaValid>
void MergeCombinatorFillAir(BLOCKTYPE & a_DstType, BLOCKTYPE a_SrcType, NIBBLETYPE & a_DstMeta, NIBBLETYPE a_SrcMeta)
{
	if (a_DstType == E_BLOCK_AIR)
	{
		a_DstType = a_SrcType;
		if (MetaValid)
		{
			a_DstMeta = a_SrcMeta;
		}
	}
	// "else" is the default, already in place
}





/** Combinator used for cBlockArea::msImprint merging */
template <bool MetaValid>
void MergeCombinatorImprint(BLOCKTYPE & a_DstType, BLOCKTYPE a_SrcType, NIBBLETYPE & a_DstMeta, NIBBLETYPE a_SrcMeta)
{
	if (a_SrcType != E_BLOCK_AIR)
	{
		a_DstType = a_SrcType;
		if (MetaValid)
		{
			a_DstMeta = a_SrcMeta;
		}
	}
	// "else" is the default, already in place
}





/** Combinator used for cBlockArea::msLake merging */
template <bool MetaValid>
void MergeCombinatorLake(BLOCKTYPE & a_DstType, BLOCKTYPE a_SrcType, NIBBLETYPE & a_DstMeta, NIBBLETYPE a_SrcMeta)
{
	// Sponge is the NOP block
	if (a_SrcType == E_BLOCK_SPONGE)
	{
		return;
	}

	// Air is always hollowed out
	if (a_SrcType == E_BLOCK_AIR)
	{
		a_DstType = E_BLOCK_AIR;
		if (MetaValid)
		{
			a_DstMeta = 0;
		}
		return;
	}

	// Water and lava are never overwritten
	switch (a_DstType)
	{
		case E_BLOCK_WATER:
		case E_BLOCK_STATIONARY_WATER:
		case E_BLOCK_LAVA:
		case E_BLOCK_STATIONARY_LAVA:
		{
			return;
		}
	}

	// Water and lava always overwrite
	switch (a_SrcType)
	{
		case E_BLOCK_WATER:
		case E_BLOCK_STATIONARY_WATER:
		case E_BLOCK_LAVA:
		case E_BLOCK_STATIONARY_LAVA:
		{
			a_DstType = a_SrcType;
			if (MetaValid)
			{
				a_DstMeta = a_SrcMeta;
			}
			return;
		}
	}

	if (a_SrcType == E_BLOCK_STONE)
	{
		switch (a_DstType)
		{
			case E_BLOCK_DIRT:
			case E_BLOCK_GRASS:
			case E_BLOCK_MYCELIUM:
			{
				a_DstType = E_BLOCK_STONE;
				if (MetaValid)
				{
					a_DstMeta = 0;
				}
				return;
			}
		}
	}
	// Everything else is left as it is
}





/** Combinator used for cBlockArea::msSpongePrint merging */
template <bool MetaValid>
void MergeCombinatorSpongePrint(BLOCKTYPE & a_DstType, BLOCKTYPE a_SrcType, NIBBLETYPE & a_DstMeta, NIBBLETYPE a_SrcMeta)
{
	// Sponge overwrites nothing, everything else overwrites anything
	if (a_SrcType != E_BLOCK_SPONGE)
	{
		a_DstType = a_SrcType;
		if (MetaValid)
		{
			a_DstMeta = a_SrcMeta;
		}
	}
}





/** Combinator used for cBlockArea::msDifference merging */
template <bool MetaValid>
void MergeCombinatorDifference(BLOCKTYPE & a_DstType, BLOCKTYPE a_SrcType, NIBBLETYPE & a_DstMeta, NIBBLETYPE a_SrcMeta)
{
	if ((a_DstType == a_SrcType) && (!MetaValid || (a_DstMeta == a_SrcMeta)))
	{
		a_DstType = E_BLOCK_AIR;
		if (MetaValid)
		{
			a_DstMeta = 0;
		}
	}
	else
	{
		a_DstType = a_SrcType;
		if (MetaValid)
		{
			a_DstMeta = a_SrcMeta;
		}
	}
}





/** Combinator used for cBlockArea::msSimpleCompare merging */
template <bool MetaValid>
void MergeCombinatorSimpleCompare(BLOCKTYPE & a_DstType, BLOCKTYPE a_SrcType, NIBBLETYPE & a_DstMeta, NIBBLETYPE a_SrcMeta)
{
	if ((a_DstType == a_SrcType) && (!MetaValid || (a_DstMeta == a_SrcMeta)))
	{
		// The blocktypes are the same, and the blockmetas are not present or are the same
		a_DstType = E_BLOCK_AIR;
	}
	else
	{
		// The blocktypes or blockmetas differ
		a_DstType = E_BLOCK_STONE;
	}
}





/** Combinator used for cBlockArea::msMask merging */
template <bool MetaValid>
void MergeCombinatorMask(BLOCKTYPE & a_DstType, BLOCKTYPE a_SrcType, NIBBLETYPE & a_DstMeta, NIBBLETYPE a_SrcMeta)
{
	// If the blocks are the same, keep the dest; otherwise replace with air
	if ((a_SrcType != a_DstType) || !MetaValid || (a_SrcMeta != a_DstMeta))
	{
		a_DstType = E_BLOCK_AIR;
		if (MetaValid)
		{
			a_DstMeta = 0;
		}
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
		if ((a_DataTypes & baTypes) == 0)
		{
			return false;
		}
	}

	// All other combinations are considered valid
	return true;
}





void cBlockArea::Clear(void)
{
	m_BlockTypes.reset();
	m_BlockMetas.reset();
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
	Fill(a_DataTypes, E_BLOCK_AIR);
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





bool cBlockArea::IsValidRelCoords(int a_RelX, int a_RelY, int a_RelZ) const
{
	return (
		(a_RelX >= 0) && (a_RelX < m_Size.x) &&
		(a_RelY >= 0) && (a_RelY < m_Size.y) &&
		(a_RelZ >= 0) && (a_RelZ < m_Size.z)
	);
}





bool cBlockArea::IsValidRelCoords(const Vector3i & a_RelCoords) const
{
	return IsValidRelCoords(a_RelCoords.x, a_RelCoords.y, a_RelCoords.z);
}





bool cBlockArea::IsValidCoords(int a_BlockX, int a_BlockY, int a_BlockZ) const
{
	return IsValidRelCoords(a_BlockX - m_Origin.x, a_BlockY - m_Origin.y, a_BlockZ - m_Origin.z);
}





bool cBlockArea::IsValidCoords(const Vector3i & a_Coords) const
{
	return IsValidRelCoords(a_Coords - m_Origin);
}





bool cBlockArea::Read(cForEachChunkProvider & a_ForEachChunkProvider, int a_MinBlockX, int a_MaxBlockX, int a_MinBlockY, int a_MaxBlockY, int a_MinBlockZ, int a_MaxBlockZ, int a_DataTypes)
{
	ASSERT(IsValidDataTypeCombination(a_DataTypes));
	ASSERT(cChunkDef::IsValidHeight(a_MinBlockY));
	ASSERT(cChunkDef::IsValidHeight(a_MaxBlockY));
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
	ASSERT(cChunkDef::IsValidHeight(a_MinBlockY));
	ASSERT(cChunkDef::IsValidHeight(a_MinBlockY + m_Size.y - 1));

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
	if (HasBlockTypes())
	{
		memcpy(a_Into.GetBlockTypes(), GetBlockTypes(), BlockCount * sizeof(BLOCKTYPE));
	}
	if (HasBlockMetas())
	{
		memcpy(a_Into.GetBlockMetas(), GetBlockMetas(), BlockCount * sizeof(NIBBLETYPE));
	}
	if (HasBlockLights())
	{
		memcpy(a_Into.GetBlockLight(), GetBlockLight(), BlockCount * sizeof(NIBBLETYPE));
	}
	if (HasBlockSkyLights())
	{
		memcpy(a_Into.GetBlockSkyLight(), GetBlockSkyLight(), BlockCount * sizeof(NIBBLETYPE));
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
	if (HasBlockTypes())
	{
		f.Write(GetBlockTypes(), NumBlocks * sizeof(BLOCKTYPE));
	}
	if (HasBlockMetas())
	{
		f.Write(GetBlockMetas(), NumBlocks);
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

	if (HasBlockTypes())
	{
		CropBlockTypes(a_AddMinX, a_SubMaxX, a_AddMinY, a_SubMaxY, a_AddMinZ, a_SubMaxZ);
	}
	if (HasBlockMetas())
	{
		CropNibbles(m_BlockMetas, a_AddMinX, a_SubMaxX, a_AddMinY, a_SubMaxY, a_AddMinZ, a_SubMaxZ);
	}
	if (HasBlockLights())
	{
		CropNibbles(m_BlockLight, a_AddMinX, a_SubMaxX, a_AddMinY, a_SubMaxY, a_AddMinZ, a_SubMaxZ);
	}
	if (HasBlockSkyLights())
	{
		CropNibbles(m_BlockSkyLight, a_AddMinX, a_SubMaxX, a_AddMinY, a_SubMaxY, a_AddMinZ, a_SubMaxZ);
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
	if (HasBlockTypes())
	{
		ExpandBlockTypes(a_SubMinX, a_AddMaxX, a_SubMinY, a_AddMaxY, a_SubMinZ, a_AddMaxZ);
	}
	if (HasBlockMetas())
	{
		ExpandNibbles(m_BlockMetas, a_SubMinX, a_AddMaxX, a_SubMinY, a_AddMaxY, a_SubMinZ, a_AddMaxZ);
	}
	if (HasBlockLights())
	{
		ExpandNibbles(m_BlockLight, a_SubMinX, a_AddMaxX, a_SubMinY, a_AddMaxY, a_SubMinZ, a_AddMaxZ);
	}
	if (HasBlockSkyLights())
	{
		ExpandNibbles(m_BlockSkyLight, a_SubMinX, a_AddMaxX, a_SubMinY, a_AddMaxY, a_SubMinZ, a_AddMaxZ);
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





void cBlockArea::Merge(const cBlockArea & a_Src, int a_RelX, int a_RelY, int a_RelZ, eMergeStrategy a_Strategy)
{

	const NIBBLETYPE * SrcMetas = a_Src.GetBlockMetas();
	NIBBLETYPE * DstMetas = GetBlockMetas();

	bool IsDummyMetas = ((SrcMetas == nullptr) || (DstMetas == nullptr));

	if (IsDummyMetas)
	{
		MergeByStrategy<false>(a_Src, a_RelX, a_RelY, a_RelZ, a_Strategy, SrcMetas, DstMetas);
	}
	else
	{
		MergeByStrategy<true>(a_Src, a_RelX, a_RelY, a_RelZ, a_Strategy, SrcMetas, DstMetas);
	}
}





void cBlockArea::Merge(const cBlockArea & a_Src, const Vector3i & a_RelMinCoords, eMergeStrategy a_Strategy)
{
	Merge(a_Src, a_RelMinCoords.x, a_RelMinCoords.y, a_RelMinCoords.z, a_Strategy);
}





void cBlockArea::Fill(int a_DataTypes, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, NIBBLETYPE a_BlockLight, NIBBLETYPE a_BlockSkyLight)
{
	if ((a_DataTypes & GetDataTypes()) != a_DataTypes)
	{
		LOGWARNING("%s: requested datatypes that are not present in the BlockArea object, trimming those away (req 0x%x, stor 0x%x)",
			__FUNCTION__, a_DataTypes, GetDataTypes()
		);
		a_DataTypes = a_DataTypes & GetDataTypes();
	}

	size_t BlockCount = GetBlockCount();
	if ((a_DataTypes & baTypes) != 0)
	{
		for (size_t i = 0; i < BlockCount; i++)
		{
			m_BlockTypes[i] = a_BlockType;
		}
	}
	if ((a_DataTypes & baMetas) != 0)
	{
		for (size_t i = 0; i < BlockCount; i++)
		{
			m_BlockMetas[i] = a_BlockMeta;
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
	if (HasBlockEntities() && ((a_DataTypes & baTypes) != 0))
	{
		if (cBlockEntity::IsBlockEntityBlockType(a_BlockType))
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
	int a_DataTypes, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta,
	NIBBLETYPE a_BlockLight, NIBBLETYPE a_BlockSkyLight
)
{
	if ((a_DataTypes & GetDataTypes()) != a_DataTypes)
	{
		LOGWARNING("%s: requested datatypes that are not present in the BlockArea object, trimming those away (req 0x%x, stor 0x%x)",
			__FUNCTION__, a_DataTypes, GetDataTypes()
		);
		a_DataTypes = a_DataTypes & GetDataTypes();
	}

	if ((a_DataTypes & baTypes) != 0)
	{
		for (int y = a_MinRelY; y <= a_MaxRelY; y++) for (int z = a_MinRelZ; z <= a_MaxRelZ; z++) for (int x = a_MinRelX; x <= a_MaxRelX; x++)
		{
			m_BlockTypes[MakeIndex(x, y, z)] = a_BlockType;
		}  // for x, z, y
	}
	if ((a_DataTypes & baMetas) != 0)
	{
		for (int y = a_MinRelY; y <= a_MaxRelY; y++) for (int z = a_MinRelZ; z <= a_MaxRelZ; z++) for (int x = a_MinRelX; x <= a_MaxRelX; x++)
		{
			m_BlockMetas[MakeIndex(x, y, z)] = a_BlockMeta;
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
	if (HasBlockEntities() && ((a_DataTypes & baTypes) != 0))
	{
		if (cBlockEntity::IsBlockEntityBlockType(a_BlockType))
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
	int a_DataTypes, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta,
	NIBBLETYPE a_BlockLight, NIBBLETYPE a_BlockSkyLight
)
{
	FillRelCuboid(
		a_RelCuboid.p1.x, a_RelCuboid.p2.x,
		a_RelCuboid.p1.y, a_RelCuboid.p2.y,
		a_RelCuboid.p1.z, a_RelCuboid.p2.z,
		a_DataTypes, a_BlockType, a_BlockMeta, a_BlockLight, a_BlockSkyLight
	);
}





void cBlockArea::RelLine(int a_RelX1, int a_RelY1, int a_RelZ1, int a_RelX2, int a_RelY2, int a_RelZ2,
	int a_DataTypes, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta,
	NIBBLETYPE a_BlockLight, NIBBLETYPE a_BlockSkyLight
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
			RelSetData(a_RelX1, a_RelY1, a_RelZ1, a_DataTypes, a_BlockType, a_BlockMeta, a_BlockLight, a_BlockSkyLight);

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
			RelSetData(a_RelX1, a_RelY1, a_RelZ1, a_DataTypes, a_BlockType, a_BlockMeta, a_BlockLight, a_BlockSkyLight);

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
			RelSetData(a_RelX1, a_RelY1, a_RelZ1, a_DataTypes, a_BlockType, a_BlockMeta, a_BlockLight, a_BlockSkyLight);

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
	int a_DataTypes, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta,
	NIBBLETYPE a_BlockLight, NIBBLETYPE a_BlockSkyLight
)
{
	RelLine(
		a_Point1.x, a_Point1.y, a_Point1.z,
		a_Point2.x, a_Point2.y, a_Point2.z,
		a_DataTypes, a_BlockType, a_BlockMeta, a_BlockLight, a_BlockSkyLight
	);
}





void cBlockArea::RotateCCW(void)
{
	if (!HasBlockTypes())
	{
		LOGWARNING("cBlockArea: Cannot rotate blockmeta without blocktypes!");
		return;
	}

	if (!HasBlockMetas())
	{
		// There are no blockmetas to rotate, just use the NoMeta function
		RotateCCWNoMeta();
		return;
	}

	// We are guaranteed that both blocktypes and blockmetas exist; rotate both at the same time:
	BLOCKARRAY NewTypes{ new BLOCKTYPE[GetBlockCount()] };
	NIBBLEARRAY NewMetas{ new NIBBLETYPE[GetBlockCount()] };
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
				NewTypes[NewIdx] = m_BlockTypes[OldIdx];
				NewMetas[NewIdx] = cBlockHandler::For(m_BlockTypes[OldIdx]).MetaRotateCCW(m_BlockMetas[OldIdx]);
			}  // for y
		}  // for z
	}  // for x
	m_BlockTypes = std::move(NewTypes);
	m_BlockMetas = std::move(NewMetas);

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
	if (!HasBlockTypes())
	{
		LOGWARNING("cBlockArea: Cannot rotate blockmeta without blocktypes!");
		return;
	}

	if (!HasBlockMetas())
	{
		// There are no blockmetas to rotate, just use the NoMeta function
		RotateCWNoMeta();
		return;
	}

	// We are guaranteed that both blocktypes and blockmetas exist; rotate both at the same time:
	BLOCKARRAY NewTypes{ new BLOCKTYPE[GetBlockCount()] };
	NIBBLEARRAY NewMetas{ new NIBBLETYPE[GetBlockCount()] };
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
				NewTypes[NewIdx] = m_BlockTypes[OldIdx];
				NewMetas[NewIdx] = cBlockHandler::For(m_BlockTypes[OldIdx]).MetaRotateCW(m_BlockMetas[OldIdx]);
			}  // for y
		}  // for z
	}  // for x
	m_BlockTypes = std::move(NewTypes);
	m_BlockMetas = std::move(NewMetas);

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
	if (!HasBlockTypes())
	{
		LOGWARNING("cBlockArea: Cannot mirror meta without blocktypes!");
		return;
	}

	if (!HasBlockMetas())
	{
		// There are no blockmetas to mirror, just use the NoMeta function
		MirrorXYNoMeta();
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
				std::swap(m_BlockTypes[Idx1], m_BlockTypes[Idx2]);
				NIBBLETYPE Meta1 = cBlockHandler::For(m_BlockTypes[Idx2]).MetaMirrorXY(m_BlockMetas[Idx1]);
				NIBBLETYPE Meta2 = cBlockHandler::For(m_BlockTypes[Idx1]).MetaMirrorXY(m_BlockMetas[Idx2]);
				m_BlockMetas[Idx1] = Meta2;
				m_BlockMetas[Idx2] = Meta1;
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
	if (!HasBlockTypes())
	{
		LOGWARNING("cBlockArea: Cannot mirror meta without blocktypes!");
		return;
	}

	if (!HasBlockMetas())
	{
		// There are no blockmetas to mirror, just use the NoMeta function
		MirrorXZNoMeta();
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
				std::swap(m_BlockTypes[Idx1], m_BlockTypes[Idx2]);
				NIBBLETYPE Meta1 = cBlockHandler::For(m_BlockTypes[Idx2]).MetaMirrorXZ(m_BlockMetas[Idx1]);
				NIBBLETYPE Meta2 = cBlockHandler::For(m_BlockTypes[Idx1]).MetaMirrorXZ(m_BlockMetas[Idx2]);
				m_BlockMetas[Idx1] = Meta2;
				m_BlockMetas[Idx2] = Meta1;
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
	if (!HasBlockTypes())
	{
		LOGWARNING("cBlockArea: Cannot mirror meta without blocktypes!");
		return;
	}

	if (!HasBlockMetas())
	{
		// There are no blockmetas to mirror, just use the NoMeta function
		MirrorYZNoMeta();
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
				std::swap(m_BlockTypes[Idx1], m_BlockTypes[Idx2]);
				NIBBLETYPE Meta1 = cBlockHandler::For(m_BlockTypes[Idx2]).MetaMirrorYZ(m_BlockMetas[Idx1]);
				NIBBLETYPE Meta2 = cBlockHandler::For(m_BlockTypes[Idx1]).MetaMirrorYZ(m_BlockMetas[Idx2]);
				m_BlockMetas[Idx1] = Meta2;
				m_BlockMetas[Idx2] = Meta1;
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





void cBlockArea::RotateCCWNoMeta(void)
{
	if (HasBlockTypes())
	{
		BLOCKARRAY NewTypes{ new BLOCKTYPE[GetBlockCount()] };
		for (int x = 0; x < m_Size.x; x++)
		{
			int NewZ = m_Size.x - x - 1;
			for (int z = 0; z < m_Size.z; z++)
			{
				int NewX = z;
				for (int y = 0; y < m_Size.y; y++)
				{
					NewTypes[MakeIndexForSize({ NewX, y, NewZ }, { m_Size.z, m_Size.y, m_Size.x })] = m_BlockTypes[MakeIndex(x, y, z)];
				}  // for y
			}  // for z
		}  // for x
		m_BlockTypes = std::move(NewTypes);
	}
	if (HasBlockMetas())
	{
		NIBBLEARRAY NewMetas{ new NIBBLETYPE[GetBlockCount()] };
		for (int x = 0; x < m_Size.x; x++)
		{
			int NewZ = m_Size.x - x - 1;
			for (int z = 0; z < m_Size.z; z++)
			{
				int NewX = z;
				for (int y = 0; y < m_Size.y; y++)
				{
					NewMetas[MakeIndexForSize({ NewX, y, NewZ }, { m_Size.z, m_Size.y, m_Size.x })] = m_BlockMetas[MakeIndex(x, y, z)];
				}  // for y
			}  // for z
		}  // for x
		m_BlockMetas = std::move(NewMetas);
	}

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





void cBlockArea::RotateCWNoMeta(void)
{
	if (HasBlockTypes())
	{
		BLOCKARRAY NewTypes{ new BLOCKTYPE[GetBlockCount()] };
		for (int z = 0; z < m_Size.z; z++)
		{
			int NewX = m_Size.z - z - 1;
			for (int x = 0; x < m_Size.x; x++)
			{
				int NewZ = x;
				for (int y = 0; y < m_Size.y; y++)
				{
					NewTypes[MakeIndexForSize({ NewX, y, NewZ }, { m_Size.z, m_Size.y, m_Size.x })] = m_BlockTypes[MakeIndex(x, y, z)];
				}  // for y
			}  // for x
		}  // for z
		m_BlockTypes = std::move(NewTypes);
	}
	if (HasBlockMetas())
	{
		NIBBLEARRAY NewMetas{ new NIBBLETYPE[GetBlockCount()] };
		for (int z = 0; z < m_Size.z; z++)
		{
			int NewX = m_Size.z - z - 1;
			for (int x = 0; x < m_Size.x; x++)
			{
				int NewZ = x;
				for (int y = 0; y < m_Size.y; y++)
				{
					NewMetas[MakeIndexForSize({ NewX, y, NewZ }, { m_Size.z, m_Size.y, m_Size.x })] = m_BlockMetas[MakeIndex(x, y, z)];
				}  // for y
			}  // for x
		}  // for z
		m_BlockMetas = std::move(NewMetas);
	}

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





void cBlockArea::MirrorXYNoMeta(void)
{
	int HalfZ = m_Size.z / 2;
	int MaxZ = m_Size.z - 1;
	if (HasBlockTypes())
	{
		for (int y = 0; y < m_Size.y; y++)
		{
			for (int z = 0; z < HalfZ; z++)
			{
				for (int x = 0; x < m_Size.x; x++)
				{
					std::swap(m_BlockTypes[MakeIndex(x, y, z)], m_BlockTypes[MakeIndex(x, y, MaxZ - z)]);
				}  // for x
			}  // for z
		}  // for y
	}  // if (HasBlockTypes)

	if (HasBlockMetas())
	{
		for (int y = 0; y < m_Size.y; y++)
		{
			for (int z = 0; z < HalfZ; z++)
			{
				for (int x = 0; x < m_Size.x; x++)
				{
					std::swap(m_BlockMetas[MakeIndex(x, y, z)], m_BlockMetas[MakeIndex(x, y, MaxZ - z)]);
				}  // for x
			}  // for z
		}  // for y
	}  // if (HasBlockMetas)

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





void cBlockArea::MirrorXZNoMeta(void)
{
	int HalfY = m_Size.y / 2;
	int MaxY = m_Size.y - 1;
	if (HasBlockTypes())
	{
		for (int y = 0; y < HalfY; y++)
		{
			for (int z = 0; z < m_Size.z; z++)
			{
				for (int x = 0; x < m_Size.x; x++)
				{
					std::swap(m_BlockTypes[MakeIndex(x, y, z)], m_BlockTypes[MakeIndex(x, MaxY - y, z)]);
				}  // for x
			}  // for z
		}  // for y
	}  // if (HasBlockTypes)

	if (HasBlockMetas())
	{
		for (int y = 0; y < HalfY; y++)
		{
			for (int z = 0; z < m_Size.z; z++)
			{
				for (int x = 0; x < m_Size.x; x++)
				{
					std::swap(m_BlockMetas[MakeIndex(x, y, z)], m_BlockMetas[MakeIndex(x, MaxY - y, z)]);
				}  // for x
			}  // for z
		}  // for y
	}  // if (HasBlockMetas)

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





void cBlockArea::MirrorYZNoMeta(void)
{
	int HalfX = m_Size.x / 2;
	int MaxX = m_Size.x - 1;
	if (HasBlockTypes())
	{
		for (int y = 0; y < m_Size.y; y++)
		{
			for (int z = 0; z < m_Size.z; z++)
			{
				for (int x = 0; x < HalfX; x++)
				{
					std::swap(m_BlockTypes[MakeIndex(x, y, z)], m_BlockTypes[MakeIndex(MaxX - x, y, z)]);
				}  // for x
			}  // for z
		}  // for y
	}  // if (HasBlockTypes)

	if (HasBlockMetas())
	{
		for (int y = 0; y < m_Size.y; y++)
		{
			for (int z = 0; z < m_Size.z; z++)
			{
				for (int x = 0; x < HalfX; x++)
				{
					std::swap(m_BlockMetas[MakeIndex(x, y, z)], m_BlockMetas[MakeIndex(MaxX - x, y, z)]);
				}  // for x
			}  // for z
		}  // for y
	}  // if (HasBlockMetas)

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





void cBlockArea::SetRelBlockType(int a_RelX, int a_RelY, int a_RelZ, BLOCKTYPE a_BlockType)
{
	ASSERT(m_BlockTypes != nullptr);
	auto idx = MakeIndex(a_RelX, a_RelY, a_RelZ);
	m_BlockTypes[idx] = a_BlockType;

	// Update the block entities, if appropriate:
	if (HasBlockEntities())
	{
		auto itr = m_BlockEntities->find(idx);
		if (itr != m_BlockEntities->end())
		{
			if (itr->second->GetBlockType() == a_BlockType)
			{
				// The block entity is for the same block type, keep the current one
				return;
			}
			m_BlockEntities->erase(itr);
		}
		if (cBlockEntity::IsBlockEntityBlockType(a_BlockType))
		{
			NIBBLETYPE meta = HasBlockMetas() ? m_BlockMetas[idx] : 0;
			m_BlockEntities->emplace(idx, cBlockEntity::CreateByBlockType(a_BlockType, meta, {a_RelX, a_RelY, a_RelZ}));
		}
	}
}





void cBlockArea::SetBlockType(int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE a_BlockType)
{
	SetRelBlockType(a_BlockX - m_Origin.x, a_BlockY - m_Origin.y, a_BlockZ - m_Origin.z, a_BlockType);
}





void cBlockArea::SetRelBlockMeta(int a_RelX, int a_RelY, int a_RelZ, NIBBLETYPE a_BlockMeta)
{
	SetRelNibble(a_RelX, a_RelY, a_RelZ, a_BlockMeta, GetBlockMetas());
}





void cBlockArea::SetBlockMeta(int a_BlockX, int a_BlockY, int a_BlockZ, NIBBLETYPE a_BlockMeta)
{
	SetNibble(a_BlockX, a_BlockY, a_BlockZ, a_BlockMeta, GetBlockMetas());
}





void cBlockArea::SetRelBlockLight(int a_RelX, int a_RelY, int a_RelZ, NIBBLETYPE a_BlockLight)
{
	SetRelNibble(a_RelX, a_RelY, a_RelZ, a_BlockLight, GetBlockLight());
}





void cBlockArea::SetBlockLight(int a_BlockX, int a_BlockY, int a_BlockZ, NIBBLETYPE a_BlockLight)
{
	SetNibble(a_BlockX, a_BlockY, a_BlockZ, a_BlockLight, GetBlockLight());
}





void cBlockArea::SetRelBlockSkyLight(int a_RelX, int a_RelY, int a_RelZ, NIBBLETYPE a_BlockSkyLight)
{
	SetRelNibble(a_RelX, a_RelY, a_RelZ, a_BlockSkyLight, GetBlockSkyLight());
}





void cBlockArea::SetBlockSkyLight(int a_BlockX, int a_BlockY, int a_BlockZ, NIBBLETYPE a_BlockSkyLight)
{
	SetNibble(a_BlockX, a_BlockY, a_BlockZ, a_BlockSkyLight, GetBlockSkyLight());
}





BLOCKTYPE cBlockArea::GetRelBlockType(int a_RelX, int a_RelY, int a_RelZ) const
{
	if (m_BlockTypes == nullptr)
	{
		LOGWARNING("cBlockArea: BlockTypes have not been read!");
		return E_BLOCK_AIR;
	}
	return m_BlockTypes[MakeIndex(a_RelX, a_RelY, a_RelZ)];
}





BLOCKTYPE cBlockArea::GetBlockType(int a_BlockX, int a_BlockY, int a_BlockZ) const
{
	return GetRelBlockType(a_BlockX - m_Origin.x, a_BlockY - m_Origin.y, a_BlockZ - m_Origin.z);
}





NIBBLETYPE cBlockArea::GetRelBlockMeta(int a_RelX, int a_RelY, int a_RelZ) const
{
	return GetRelNibble(a_RelX, a_RelY, a_RelZ, GetBlockMetas());
}





NIBBLETYPE cBlockArea::GetBlockMeta(int a_BlockX, int a_BlockY, int a_BlockZ) const
{
	return GetNibble(a_BlockX, a_BlockY, a_BlockZ, GetBlockMetas());
}





NIBBLETYPE cBlockArea::GetRelBlockLight(int a_RelX, int a_RelY, int a_RelZ) const
{
	return GetRelNibble(a_RelX, a_RelY, a_RelZ, GetBlockLight());
}





NIBBLETYPE cBlockArea::GetBlockLight(int a_BlockX, int a_BlockY, int a_BlockZ) const
{
	return GetNibble(a_BlockX, a_BlockY, a_BlockZ, GetBlockLight());
}





NIBBLETYPE cBlockArea::GetRelBlockSkyLight(int a_RelX, int a_RelY, int a_RelZ) const
{
	return GetRelNibble(a_RelX, a_RelY, a_RelZ, GetBlockSkyLight());
}





NIBBLETYPE cBlockArea::GetBlockSkyLight(int a_BlockX, int a_BlockY, int a_BlockZ) const
{
	return GetNibble(a_BlockX, a_BlockY, a_BlockZ, GetBlockSkyLight());
}





void cBlockArea::SetBlockTypeMeta(int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta)
{
	SetRelBlockTypeMeta(a_BlockX - m_Origin.x, a_BlockY - m_Origin.y, a_BlockZ - m_Origin.z, a_BlockType, a_BlockMeta);
}





void cBlockArea::SetRelBlockTypeMeta(int a_RelX,   int a_RelY,   int a_RelZ,   BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta)
{
	auto idx = MakeIndex(a_RelX, a_RelY, a_RelZ);
	if (m_BlockTypes == nullptr)
	{
		LOGWARNING("%s: BlockTypes not available but requested to be written to.", __FUNCTION__);
	}
	else
	{
		m_BlockTypes[idx] = a_BlockType;
	}
	if (m_BlockMetas == nullptr)
	{
		LOGWARNING("%s: BlockMetas not available but requested to be written to.", __FUNCTION__);
	}
	else
	{
		m_BlockMetas[idx] = a_BlockMeta;
	}

	// Update the block entities, if appropriate:
	if (HasBlockEntities())
	{
		auto itr = m_BlockEntities->find(idx);
		if (itr != m_BlockEntities->end())
		{
			if (itr->second->GetBlockType() == a_BlockType)
			{
				// The block entity is for the same block type, keep the current one
				return;
			}
			m_BlockEntities->erase(itr);
		}
		if (cBlockEntity::IsBlockEntityBlockType(a_BlockType))
		{
			m_BlockEntities->emplace(idx, cBlockEntity::CreateByBlockType(a_BlockType, a_BlockMeta, {a_RelX, a_RelY, a_RelZ}));
		}
	}
}





void cBlockArea::GetBlockTypeMeta(int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE & a_BlockType, NIBBLETYPE & a_BlockMeta) const
{
	return GetRelBlockTypeMeta(a_BlockX - m_Origin.x, a_BlockY - m_Origin.y, a_BlockZ - m_Origin.z, a_BlockType, a_BlockMeta);
}





void cBlockArea::GetRelBlockTypeMeta(int a_RelX, int a_RelY, int a_RelZ, BLOCKTYPE & a_BlockType, NIBBLETYPE & a_BlockMeta) const
{
	auto idx = MakeIndex(a_RelX, a_RelY, a_RelZ);
	if (m_BlockTypes == nullptr)
	{
		LOGWARNING("cBlockArea: BlockTypes have not been read!");
		a_BlockType = E_BLOCK_AIR;
	}
	else
	{
		a_BlockType = m_BlockTypes[idx];
	}

	if (m_BlockMetas == nullptr)
	{
		LOGWARNING("cBlockArea: BlockMetas have not been read!");
		a_BlockMeta = 0;
	}
	else
	{
		a_BlockMeta = m_BlockMetas[idx];
	}
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
	if (m_BlockTypes == nullptr)
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
				if (m_BlockTypes[MakeIndex(x, y, z)] != E_BLOCK_AIR)
				{
					++res;
				}
			}  // for x
		}  // for z
	}  // for y
	return res;
}





size_t cBlockArea::CountSpecificBlocks(BLOCKTYPE a_BlockType) const
{
	// If blocktypes are not valid, log a warning and return zero occurences:
	if (m_BlockTypes == nullptr)
	{
		LOGWARNING("%s: BlockTypes not available!", __FUNCTION__);
		return 0;
	}

	// Count the blocks:
	size_t num = GetBlockCount();
	size_t res = 0;
	for (size_t i = 0; i < num; i++)
	{
		if (m_BlockTypes[i] == a_BlockType)
		{
			res++;
		}
	}
	return res;
}





size_t cBlockArea::CountSpecificBlocks(BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta) const
{
	// If blocktypes are not valid, log a warning and return zero occurences:
	if (m_BlockTypes == nullptr)
	{
		LOGWARNING("%s: BlockTypes not available!", __FUNCTION__);
		return 0;
	}

	// If blockmetas are not valid, log a warning and count only blocktypes:
	if (m_BlockMetas == nullptr)
	{
		LOGWARNING("%s: BlockMetas not available, comparing blocktypes only!", __FUNCTION__);
		return CountSpecificBlocks(a_BlockType);
	}

	// Count the blocks:
	size_t num = GetBlockCount();
	size_t res = 0;
	for (size_t i = 0; i < num; i++)
	{
		if ((m_BlockTypes[i] == a_BlockType) && (m_BlockMetas[i] == a_BlockMeta))
		{
			res++;
		}
	}
	return res;
}





void cBlockArea::GetNonAirCropRelCoords(int & a_MinRelX, int & a_MinRelY, int & a_MinRelZ, int & a_MaxRelX, int & a_MaxRelY, int & a_MaxRelZ, BLOCKTYPE a_IgnoreBlockType)
{
	// Check if blocktypes are valid:
	if (m_BlockTypes == nullptr)
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
				if (m_BlockTypes[MakeIndex(x, y, z)] == a_IgnoreBlockType)
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
	if (m_BlockTypes != nullptr)
	{
		res |= baTypes;
	}
	if (m_BlockMetas != nullptr)
	{
		res |= baMetas;
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
	NIBBLEARRAY NewMetas;
	NIBBLEARRAY NewLight;
	NIBBLEARRAY NewSkyLight;
	cBlockEntitiesPtr NewBlockEntities;

	// Try to allocate the new storage
	if ((a_DataTypes & baTypes) != 0)
	{
		NewBlocks.reset(new BLOCKTYPE[ToUnsigned(a_SizeX * a_SizeY * a_SizeZ)]);
		if (NewBlocks == nullptr)
		{
			return false;
		}
	}
	if ((a_DataTypes & baMetas) != 0)
	{
		NewMetas.reset(new NIBBLETYPE[ToUnsigned(a_SizeX * a_SizeY * a_SizeZ)]);
		if (NewMetas == nullptr)
		{
			return false;
		}
	}
	if ((a_DataTypes & baLight) != 0)
	{
		NewLight.reset(new NIBBLETYPE[ToUnsigned(a_SizeX * a_SizeY * a_SizeZ)]);
		if (NewLight == nullptr)
		{
			return false;
		}
	}
	if ((a_DataTypes & baSkyLight) != 0)
	{
		NewSkyLight.reset(new NIBBLETYPE[ToUnsigned(a_SizeX * a_SizeY * a_SizeZ)]);
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
	m_BlockTypes = std::move(NewBlocks);
	m_BlockMetas = std::move(NewMetas);
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





bool cBlockArea::DoWithBlockEntityRelAt(int a_RelX, int a_RelY, int a_RelZ, cBlockEntityCallback a_Callback)
{
	ASSERT(IsValidRelCoords(a_RelX, a_RelY, a_RelZ));
	if (!HasBlockEntities())
	{
		return false;
	}
	auto idx = MakeIndex(a_RelX, a_RelY, a_RelZ);
	auto itr = m_BlockEntities->find(idx);
	if (itr == m_BlockEntities->end())
	{
		return false;
	}
	return a_Callback(*itr->second);
}





bool cBlockArea::DoWithBlockEntityAt(int a_BlockX, int a_BlockY, int a_BlockZ, cBlockEntityCallback a_Callback)
{
	return DoWithBlockEntityRelAt(a_BlockX - m_Origin.x, a_BlockY - m_Origin.y, a_BlockZ - m_Origin.z, a_Callback);
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





void cBlockArea::SetRelNibble(int a_RelX, int a_RelY, int a_RelZ, NIBBLETYPE a_Value, NIBBLETYPE * a_Array)
{
	if (a_Array == nullptr)
	{
		LOGWARNING("cBlockArea: datatype has not been read!");
		return;
	}
	a_Array[MakeIndex(a_RelX, a_RelY, a_RelZ)] = a_Value;
}





void cBlockArea::SetNibble(int a_BlockX, int a_BlockY, int a_BlockZ, NIBBLETYPE a_Value, NIBBLETYPE * a_Array)
{
	SetRelNibble(a_BlockX - m_Origin.x, a_BlockY - m_Origin.y, a_BlockZ - m_Origin.z, a_Value, a_Array);
}





NIBBLETYPE cBlockArea::GetRelNibble(int a_RelX, int a_RelY, int a_RelZ, NIBBLETYPE * a_Array) const
{
	if (a_Array == nullptr)
	{
		LOGWARNING("cBlockArea: datatype has not been read!");
		return 16;
	}
	return a_Array[MakeIndex(a_RelX, a_RelY, a_RelZ)];
}





NIBBLETYPE cBlockArea::GetNibble(int a_BlockX, int a_BlockY, int a_BlockZ, NIBBLETYPE * a_Array) const
{
	return GetRelNibble(a_BlockX - m_Origin.x, a_BlockY - m_Origin.y, a_BlockZ - m_Origin.z, a_Array);
}





void cBlockArea::CropBlockTypes(int a_AddMinX, int a_SubMaxX, int a_AddMinY, int a_SubMaxY, int a_AddMinZ, int a_SubMaxZ)
{
	int NewSizeX = GetSizeX() - a_AddMinX - a_SubMaxX;
	int NewSizeY = GetSizeY() - a_AddMinY - a_SubMaxY;
	int NewSizeZ = GetSizeZ() - a_AddMinZ - a_SubMaxZ;
	BLOCKARRAY NewBlockTypes{ new BLOCKTYPE[ToUnsigned(NewSizeX * NewSizeY * NewSizeZ)] };
	size_t idx = 0;
	for (int y = 0; y < NewSizeY; y++)
	{
		for (int z = 0; z < NewSizeZ; z++)
		{
			for (int x = 0; x < NewSizeX; x++)
			{
				auto OldIndex = MakeIndex(x + a_AddMinX, y + a_AddMinY, z + a_AddMinZ);
				NewBlockTypes[idx++] = m_BlockTypes[OldIndex];
			}  // for x
		}  // for z
	}  // for y
	m_BlockTypes = std::move(NewBlockTypes);
}





void cBlockArea::CropNibbles(NIBBLEARRAY & a_Array, int a_AddMinX, int a_SubMaxX, int a_AddMinY, int a_SubMaxY, int a_AddMinZ, int a_SubMaxZ)
{
	int NewSizeX = GetSizeX() - a_AddMinX - a_SubMaxX;
	int NewSizeY = GetSizeY() - a_AddMinY - a_SubMaxY;
	int NewSizeZ = GetSizeZ() - a_AddMinZ - a_SubMaxZ;
	NIBBLEARRAY NewNibbles{ new NIBBLETYPE[ToUnsigned(NewSizeX * NewSizeY * NewSizeZ)] };
	size_t idx = 0;
	for (int y = 0; y < NewSizeY; y++)
	{
		for (int z = 0; z < NewSizeZ; z++)
		{
			for (int x = 0; x < NewSizeX; x++)
			{
				NewNibbles[idx++] = a_Array[MakeIndex(x + a_AddMinX, y + a_AddMinY, z + a_AddMinZ)];
			}  // for x
		}  // for z
	}  // for y
	a_Array = std::move(NewNibbles);
}





void cBlockArea::ExpandBlockTypes(int a_SubMinX, int a_AddMaxX, int a_SubMinY, int a_AddMaxY, int a_SubMinZ, int a_AddMaxZ)
{
	int NewSizeX = m_Size.x + a_SubMinX + a_AddMaxX;
	int NewSizeY = m_Size.y + a_SubMinY + a_AddMaxY;
	int NewSizeZ = m_Size.z + a_SubMinZ + a_AddMaxZ;
	size_t BlockCount = static_cast<size_t>(NewSizeX * NewSizeY * NewSizeZ);
	BLOCKARRAY NewBlockTypes{ new BLOCKTYPE[BlockCount] };
	memset(NewBlockTypes.get(), 0, BlockCount * sizeof(BLOCKTYPE));
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
				NewBlockTypes[idx++] = m_BlockTypes[OldIndex++];
			}  // for x
		}  // for z
	}  // for y
	m_BlockTypes = std::move(NewBlockTypes);
}





void cBlockArea::ExpandNibbles(NIBBLEARRAY & a_Array, int a_SubMinX, int a_AddMaxX, int a_SubMinY, int a_AddMaxY, int a_SubMinZ, int a_AddMaxZ)
{
	int NewSizeX = m_Size.x + a_SubMinX + a_AddMaxX;
	int NewSizeY = m_Size.y + a_SubMinY + a_AddMaxY;
	int NewSizeZ = m_Size.z + a_SubMinZ + a_AddMaxZ;
	size_t BlockCount = static_cast<size_t>(NewSizeX * NewSizeY * NewSizeZ);
	NIBBLEARRAY NewNibbles{ new NIBBLETYPE[BlockCount] };
	memset(NewNibbles.get(), 0, BlockCount * sizeof(NIBBLETYPE));
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
				NewNibbles[idx++] = a_Array[OldIndex++];
			}  // for x
		}  // for z
	}  // for y
	a_Array = std::move(NewNibbles);
}





void cBlockArea::RelSetData(
	int a_RelX, int a_RelY, int a_RelZ,
	int a_DataTypes, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta,
	NIBBLETYPE a_BlockLight, NIBBLETYPE a_BlockSkyLight
)
{
	if (!IsValidCoords(a_RelX, a_RelY, a_RelZ))
	{
		return;
	}

	auto Index = MakeIndex(a_RelX, a_RelY, a_RelZ);
	if ((a_DataTypes & baTypes) != 0)
	{
		m_BlockTypes[Index] = a_BlockType;
	}
	if ((a_DataTypes & baMetas) != 0)
	{
		m_BlockMetas[Index] = a_BlockMeta;
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
			if (itr->second->GetBlockType() == a_BlockType)
			{
				// The block entity is for the same block type, keep the current one
				return;
			}
			// The block entity is for a different block type, remove it:
			m_BlockEntities->erase(itr);
		}
		if (cBlockEntity::IsBlockEntityBlockType(a_BlockType))
		{
			// The block type should have a block entity attached to it, create an empty one:
			m_BlockEntities->emplace(Index, cBlockEntity::CreateByBlockType(a_BlockType, a_BlockMeta, {a_RelX, a_RelY, a_RelZ}));
		}
	}
}





template <bool MetasValid>
void cBlockArea::MergeByStrategy(const cBlockArea & a_Src, int a_RelX, int a_RelY, int a_RelZ, eMergeStrategy a_Strategy, const NIBBLETYPE * SrcMetas, NIBBLETYPE * DstMetas)
{
	// Block types are compulsory, block metas are optional
	if (!HasBlockTypes() || !a_Src.HasBlockTypes())
	{
		LOGWARNING("%s: cannot merge because one of the areas doesn't have blocktypes.", __FUNCTION__);
		return;
	}

	// Dst is *this, Src is a_Src
	int SrcOffX = std::max(0, -a_RelX);  // Offset in Src where to start reading
	int DstOffX = std::max(0,  a_RelX);  // Offset in Dst where to start writing
	int SizeX   = std::min(a_Src.GetSizeX() - SrcOffX, GetSizeX() - DstOffX);  // How many blocks to copy

	int SrcOffY = std::max(0, -a_RelY);  // Offset in Src where to start reading
	int DstOffY = std::max(0,  a_RelY);  // Offset in Dst where to start writing
	int SizeY   = std::min(a_Src.GetSizeY() - SrcOffY, GetSizeY() - DstOffY);  // How many blocks to copy

	int SrcOffZ = std::max(0, -a_RelZ);  // Offset in Src where to start reading
	int DstOffZ = std::max(0,  a_RelZ);  // Offset in Dst where to start writing
	int SizeZ   = std::min(a_Src.GetSizeZ() - SrcOffZ, GetSizeZ() - DstOffZ);  // How many blocks to copy

	[&]
	{
		switch (a_Strategy)
		{
			case cBlockArea::msOverwrite:
			{
				InternalMergeBlocks<MetasValid, MergeCombinatorOverwrite<MetasValid> >(
					GetBlockTypes(), a_Src.GetBlockTypes(),
					DstMetas, SrcMetas,
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
				InternalMergeBlocks<MetasValid, MergeCombinatorFillAir<MetasValid> >(
					GetBlockTypes(), a_Src.GetBlockTypes(),
					DstMetas, SrcMetas,
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
				InternalMergeBlocks<MetasValid, MergeCombinatorImprint<MetasValid> >(
					GetBlockTypes(), a_Src.GetBlockTypes(),
					DstMetas, SrcMetas,
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
				InternalMergeBlocks<MetasValid, MergeCombinatorLake<MetasValid> >(
					GetBlockTypes(), a_Src.GetBlockTypes(),
					DstMetas, SrcMetas,
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
				InternalMergeBlocks<MetasValid, MergeCombinatorSpongePrint<MetasValid> >(
					GetBlockTypes(), a_Src.GetBlockTypes(),
					DstMetas, SrcMetas,
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
				InternalMergeBlocks<MetasValid, MergeCombinatorDifference<MetasValid> >(
					GetBlockTypes(), a_Src.GetBlockTypes(),
					DstMetas, SrcMetas,
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
				InternalMergeBlocks<MetasValid, MergeCombinatorSimpleCompare<MetasValid> >(
					GetBlockTypes(), a_Src.GetBlockTypes(),
					DstMetas, SrcMetas,
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
				InternalMergeBlocks<MetasValid, MergeCombinatorMask<MetasValid> >(
					GetBlockTypes(), a_Src.GetBlockTypes(),
					DstMetas, SrcMetas,
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
			MergeBlockEntities(a_RelX, a_RelY, a_RelZ, a_Src);
		}
		else
		{
			RescanBlockEntities();
		}
	}
}





void cBlockArea::MergeBlockEntities(int a_RelX, int a_RelY, int a_RelZ, const cBlockArea & a_Src)
{
	// Only supported with both BlockEntities and BlockTypes (caller should check):
	ASSERT(HasBlockTypes());
	ASSERT(HasBlockEntities());
	ASSERT(a_Src.HasBlockTypes());
	ASSERT(a_Src.HasBlockEntities());

	// Remove block entities that no longer match the block at their coords:
	RemoveNonMatchingBlockEntities();

	// Clone BEs from a_Src wherever a BE is missing:
	for (int y = 0; y < m_Size.y; ++y) for (int z = 0; z < m_Size.z; ++z) for (int x = 0; x < m_Size.x; ++x)
	{
		auto idx = MakeIndex(x, y, z);
		auto type = m_BlockTypes[idx];
		if (!cBlockEntity::IsBlockEntityBlockType(type))
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
		auto srcX = x + a_RelX;
		auto srcY = y + a_RelY;
		auto srcZ = z + a_RelZ;
		if (a_Src.IsValidRelCoords(srcX, srcY, srcZ))
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
		NIBBLETYPE meta = HasBlockMetas() ? m_BlockMetas[idx] : 0;
		m_BlockEntities->emplace(idx, cBlockEntity::CreateByBlockType(type, meta, {x, y, z}));
	}  // for x, z, y
}





void cBlockArea::RescanBlockEntities(void)
{
	// Only supported with both BlockEntities and BlockTypes
	if (!HasBlockEntities() || !HasBlockTypes())
	{
		return;
	}

	// Remove block entities that no longer match the block at their coords:
	RemoveNonMatchingBlockEntities();

	// Add block entities for all block types that should have a BE assigned to them:
	for (int y = 0; y < m_Size.y; ++y) for (int z = 0; z < m_Size.z; ++z) for (int x = 0; x < m_Size.x; ++x)
	{
		auto idx = MakeIndex(x, y, z);
		auto type = m_BlockTypes[idx];
		if (!cBlockEntity::IsBlockEntityBlockType(type))
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
		NIBBLETYPE meta = HasBlockMetas() ? m_BlockMetas[idx] : 0;
		m_BlockEntities->emplace(idx, cBlockEntity::CreateByBlockType(type, meta, {x, y, z}));
	}  // for x, z, y
}





void cBlockArea::RemoveNonMatchingBlockEntities(void)
{
	// Only supported with both BlockEntities and BlockTypes:
	ASSERT(HasBlockTypes());
	ASSERT(HasBlockEntities());

	cBlockEntities oldBE;
	std::swap(oldBE, *m_BlockEntities);
	for (auto & keyPair: oldBE)
	{
		auto type = m_BlockTypes[static_cast<size_t>(keyPair.first)];
		if (type == keyPair.second->GetBlockType())
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





void cBlockArea::cChunkReader::CopyNibbles(NIBBLETYPE * a_AreaDst, const NIBBLETYPE * a_ChunkSrc)
{
	int SizeY = m_Area.m_Size.y;
	int MinY = m_Origin.y;

	// SizeX, SizeZ are the dmensions of the block data to copy from the current chunk (size of the geometric union)
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

	for (int y = 0; y < SizeY; y++)
	{
		int ChunkY = MinY + y;
		int AreaY = y;
		for (int z = 0; z < SizeZ; z++)
		{
			int ChunkZ = BaseZ + z;
			int AreaZ = OffZ + z;
			for (int x = 0; x < SizeX; x++)
			{
				int ChunkX = BaseX + x;
				int AreaX = OffX + x;
				a_AreaDst[m_Area.MakeIndex(AreaX, AreaY, AreaZ)] = cChunkDef::GetNibble(a_ChunkSrc, ChunkX, ChunkY, ChunkZ);
			}  // for x
		}  // for z
	}  // for y
}





bool cBlockArea::cChunkReader::Coords(int a_ChunkX, int a_ChunkZ)
{
	m_CurrentChunkX = a_ChunkX;
	m_CurrentChunkZ = a_ChunkZ;
	return true;
}





void cBlockArea::cChunkReader::ChunkData(const cChunkData & a_BlockBuffer)
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
	if (m_Area.m_BlockTypes != nullptr)
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
					m_Area.m_BlockTypes[m_Area.MakeIndex(AreaX, AreaY, AreaZ)] = a_BlockBuffer.GetBlock({ InChunkX, InChunkY, InChunkZ });
				}  // for x
			}  // for z
		}  // for y
	}

	// Copy the block metas:
	if (m_Area.m_BlockMetas != nullptr)
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
					m_Area.m_BlockMetas[m_Area.MakeIndex(AreaX, AreaY, AreaZ)] = a_BlockBuffer.GetMeta({ InChunkX, InChunkY, InChunkZ });
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
					m_Area.m_BlockLight[m_Area.MakeIndex(AreaX, AreaY, AreaZ)] = a_BlockBuffer.GetBlockLight({ InChunkX, InChunkY, InChunkZ });
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
					m_Area.m_BlockSkyLight[m_Area.MakeIndex(AreaX, AreaY, AreaZ)] = a_BlockBuffer.GetSkyLight({ InChunkX, InChunkY, InChunkZ });
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





