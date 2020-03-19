#include "Globals.h"
#include "PalettedBlockArea.h"





PalettedBlockArea::PalettedBlockArea()
{
	// Nothing needed yet
}





PalettedBlockArea PalettedBlockArea::createFilled(Vector3i aSize, const AString & aBlockTypeName, const BlockState & aBlockState)
{
	ASSERT(aSize.x > 0);
	ASSERT(aSize.y > 0);
	ASSERT(aSize.z > 0);

	PalettedBlockArea res;
	auto numBlocks = static_cast<UInt64>(aSize.x) * static_cast<UInt64>(aSize.y) * static_cast<UInt64>(aSize.z);
	if (numBlocks >= std::numeric_limits<UInt32>::max())
	{
		// We use 32-bit indices in some functions (for ARM speed), so we need the entire area to fit into UInt32
		throw std::runtime_error("Size is too large");
	}
	res.mSize = aSize;
	res.mBlocks.resize(static_cast<size_t>(numBlocks));
	res.fill(aBlockTypeName, aBlockState);
	return res;
}





cCuboid PalettedBlockArea::whole() const
{
	return cCuboid(Vector3i(), mSize);
}





void PalettedBlockArea::setBlock(Vector3i aPos, const AString & aBlockTypeName, const BlockState & aBlockState)
{
	setBlock(aPos, paletteIndex(aBlockTypeName, aBlockState));
}





void PalettedBlockArea::setBlock(Vector3i aPos, UInt32 aPaletteIndex)
{
	ASSERT(isPositionValid(aPos));
	ASSERT(aPaletteIndex < mPalette.count());

	auto idx = positionToIndex(aPos);
	mBlocks[idx] = aPaletteIndex;
}





UInt32 PalettedBlockArea::paletteIndex(const AString & aBlockTypeName, const BlockState & aBlockState)
{
	return mPalette.index(aBlockTypeName, aBlockState);
}





std::pair<UInt32, bool> PalettedBlockArea::maybePaletteIndex(const AString & aBlockTypeName, const BlockState & aBlockState) const
{
	return mPalette.maybeIndex(aBlockTypeName, aBlockState);
}





UInt32 PalettedBlockArea::blockPaletteIndex(Vector3i aPos) const
{
	auto idx = positionToIndex(aPos);
	return mBlocks[idx];
}





const std::pair<AString, BlockState> & PalettedBlockArea::block(Vector3i aPos) const
{
	return paletteEntry(blockPaletteIndex(aPos));
}





const std::pair<AString, BlockState> & PalettedBlockArea::paletteEntry(UInt32 aPaletteIndex) const
{
	return mPalette.entry(aPaletteIndex);
}





bool PalettedBlockArea::isPositionValid(Vector3i aPos) const
{
	return (
		(aPos.x >= 0) && (aPos.y >= 0) && (aPos.z >= 0) &&              // Non-negative coords
		(aPos.x < mSize.x) && (aPos.y < mSize.y) && (aPos.z < mSize.z)  // Fit into size
	);
}





void PalettedBlockArea::fill(const AString & aBlockTypeName, const BlockState & aBlockState)
{
	BlockTypePalette btp;
	auto idx = btp.index(aBlockTypeName, aBlockState);
	std::swap(mPalette, btp);
	std::fill(mBlocks.begin(), mBlocks.end(), idx);
}





void PalettedBlockArea::paste(const PalettedBlockArea & aSrc, const cCuboid & aSrcCuboid, Vector3i aDstOrigin)
{
	// Clamp the src cuboid, first by src itself, then by this PBA's coord range:
	cCuboid srcCuboid(aSrcCuboid);
	srcCuboid.Sort();
	srcCuboid.Clamp(aSrc.whole());
	Vector3i maxSize = mSize - aDstOrigin;
	srcCuboid.ClampSize(maxSize);
	Vector3i dstOrigin(aDstOrigin);

	// If any aDstOrigin coord is lower than 0, adjust the coord and src cuboid size:
	if (dstOrigin.x < 0)
	{
		srcCuboid.p1.x -= dstOrigin.x;
		if (srcCuboid.p1.x >= srcCuboid.p2.x)
		{
			return;
		}
		dstOrigin.x = 0;
	}
	if (dstOrigin.y < 0)
	{
		srcCuboid.p1.y -= dstOrigin.y;
		if (srcCuboid.p1.y >= srcCuboid.p2.y)
		{
			return;
		}
		dstOrigin.y = 0;
	}
	if (dstOrigin.z < 0)
	{
		srcCuboid.p1.z -= dstOrigin.z;
		if (srcCuboid.p1.z >= srcCuboid.p2.z)
		{
			return;
		}
		dstOrigin.z = 0;
	}

	// Create a transform map from aSrc's palette to our palette:
	auto paletteTransform = mPalette.createTransformMapAddMissing(aSrc.mPalette);

	// Copy the data:
	UInt32 srcStrideY = static_cast<UInt32>(aSrc.size().x * aSrc.size().z);
	UInt32 srcStrideZ = static_cast<UInt32>(aSrc.size().x);
	UInt32 dstStrideY = static_cast<UInt32>(mSize.x * mSize.z);
	UInt32 dstStrideZ = static_cast<UInt32>(mSize.x);
	UInt32 minX = static_cast<UInt32>(srcCuboid.p1.x);
	UInt32 maxX = static_cast<UInt32>(srcCuboid.p2.x);
	UInt32 minY = static_cast<UInt32>(srcCuboid.p1.y);
	UInt32 maxY = static_cast<UInt32>(srcCuboid.p2.y);
	UInt32 minZ = static_cast<UInt32>(srcCuboid.p1.z);
	UInt32 maxZ = static_cast<UInt32>(srcCuboid.p2.z);
	UInt32 dstX = static_cast<UInt32>(dstOrigin.x);
	UInt32 dstY = static_cast<UInt32>(dstOrigin.y);
	UInt32 dstZ = static_cast<UInt32>(dstOrigin.z);
	for (UInt32 y = minY; y < maxY; ++y)
	{
		UInt32 srcOfsY = y * srcStrideY;
		UInt32 dstOfsY = (y - minY + dstY) * dstStrideY;
		for (UInt32 z = minZ; z < maxZ; ++z)
		{
			UInt32 srcOfs = srcOfsY + z * srcStrideZ + minX;
			UInt32 dstOfs = dstOfsY + (z - minZ + dstZ) * dstStrideZ + dstX;
			for (UInt32 x = minX; x < maxX; ++x)
			{
				mBlocks[dstOfs] = paletteTransform[aSrc.mBlocks[srcOfs]];
				srcOfs += 1;
				dstOfs += 1;
			}
		}
	}
}





void PalettedBlockArea::crop(const cCuboid & aArea)
{
	cCuboid area(aArea);
	area.Clamp(whole());

	// Copy the data:
	UInt32 srcStrideY = static_cast<UInt32>(size().x * size().z);
	UInt32 srcStrideZ = static_cast<UInt32>(size().x);
	UInt32 dstStrideY = static_cast<UInt32>(area.DifX() * area.DifZ());
	UInt32 dstStrideZ = static_cast<UInt32>(area.DifZ());
	UInt32 minX = static_cast<UInt32>(area.p1.x);
	UInt32 maxX = static_cast<UInt32>(area.p2.x);
	UInt32 minY = static_cast<UInt32>(area.p1.y);
	UInt32 maxY = static_cast<UInt32>(area.p2.y);
	UInt32 minZ = static_cast<UInt32>(area.p1.z);
	UInt32 maxZ = static_cast<UInt32>(area.p2.z);
	for (UInt32 y = minY; y < maxY; ++y)
	{
		UInt32 srcOfsY = (y - minY) * srcStrideY;
		UInt32 dstOfsY = y * dstStrideY;
		for (UInt32 z = minZ; z < maxZ; ++z)
		{
			UInt32 srcOfs = srcOfsY + (z - minZ) * srcStrideZ + minX;
			UInt32 dstOfs = dstOfsY + z * dstStrideZ;
			for (UInt32 x = minX; x < maxX; ++x)
			{
				mBlocks[dstOfs] = mBlocks[srcOfs];
				srcOfs += 1;
				dstOfs += 1;
			}
		}
	}
}





UInt32 PalettedBlockArea::positionToIndex(Vector3i aPos) const
{
	ASSERT(isPositionValid(aPos));
	return static_cast<UInt32>(aPos.x + aPos.z * mSize.x + aPos.y * mSize.x * mSize.z);
}
