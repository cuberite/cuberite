#pragma once





#include <utility>

#include "BlockTypePalette.h"
#include "Cuboid.h"





/** Represents an area of blocks that are represented using a palette.
The object itself provides no thread safety, users of this class need to handle locking, if required.
The PalettedBlockArea always contains Blocks and their associated BlockEntities, it may optionally contain Entities.
There's no way to instantiate this class directly, you need to use either createFilled(), or read from cWorld. */
class PalettedBlockArea
{
public:

	/** Creates a new PBA of the specified size filled with the specified block.
	Throws if there is an error (memory allocation etc.) */
	static PalettedBlockArea createFilled(Vector3i aSize, const AString & aBlockTypeName, const BlockState & aBlockState);

	/** Returns the actual size of the area in all 3 axes. */
	const Vector3i & size() const { return mSize; }

	/** Returns a cCuboid that encompasses the entire PBA.
	Technically, {0, 0, 0} to mSize. */
	cCuboid whole() const;

	/** Sets a single block using its full blockspec.
	The position must be valid (ASSERTed).
	If the block is not already in palette, it is added. */
	void setBlock(Vector3i aPos, const AString & aBlockTypeName, const BlockState & aBlockState);

	/** Sets a single block using an index to the palette (retrieved earlier by paletteIndex()).
	The position must be valid (ASSERTed).
	The palette index must be valid (ASSERTed). */
	void setBlock(Vector3i aPos, UInt32 aPaletteIndex);

	/** Returns the index into the palette that is used by the specified full blockspec.
	Adds the blockspec to palette if not already there. */
	UInt32 paletteIndex(const AString & aBlockTypeName, const BlockState & aBlockState);

	/** Returns the <index, true> into the palette that is used by the specified full blockspec.
	Returns <undefined, false> if blockspec not in palette. */
	std::pair<UInt32, bool> maybePaletteIndex(const AString & aBlockTypeName, const BlockState & aBlockState) const;

	/** Returns the index into the palette for the block at the specified pos.
	The position must be valid (ASSERTed). */
	UInt32 blockPaletteIndex(Vector3i aPos) const;

	/** Returns the full blockspec of the block at the specified position.
	The position must be valid (ASSERTed). */
	const std::pair<AString, BlockState> & block(Vector3i aPos) const;

	/** Returns the blockspec represented by the specified palette index.
	The index must be valid (ASSERTed). */
	const std::pair<AString, BlockState> & paletteEntry(UInt32 aPaletteIndex) const;

	/** Returns true if the specified position is within the size bounds of the area. */
	bool isPositionValid(Vector3i aPos) const;

	/** Fills the entire PBA with a single block of the specified type.
	The palette is reset to one containing only the single block. */
	void fill(const AString & aBlockTypeName, const BlockState & aBlockState);

	/** Pastes (copies verbatim) a cCuboid out of the src PBA into this PBA.
	aSrcCuboid is the coord range in aSrc that will be copied (min-coord is inclusive, max-coord is exclusive).
	aDstOrigin is the coord relative to this PBA where the lowest coords of the copied area will be put.
	aDstOrigin can be outside of this PBA's coord range (only part of the src is copied).
	Automatically crops aSrcCuboid so that the copied part is entirely inside this PBA's coord range. */
	void paste(const PalettedBlockArea & aSrc, const cCuboid & aSrcCuboid, Vector3i aDstOrigin = Vector3i());

	/** Pastes (copies verbatim) the entire src PBA into this PBA.
	aDstOrigin is the coord relative to this PBA where the lowest coords of the copied area will be put.
	aDstOrigin can be outside of this PBA's coord range (only part of the src is copied).
	Gracefully handles situations where the copied src PBA goes outside of this PBA's coord range. */
	inline void paste(const PalettedBlockArea & aSrc, Vector3i aDstOrigin = Vector3i())
	{
		paste(aSrc, aSrc.whole(), aDstOrigin);
	}

	/** Crops this PBA by the specified coords.
	aArea is first cropped to the size of this PBA (so it's only possible to shrink a PBA, not enlarge). */
	void crop(const cCuboid & aArea);

	/** Returns the (reqd-only) palette used internally by this object. */
	const BlockTypePalette & palette() { return mPalette; }


protected:

	/** The palette used in the area. */
	BlockTypePalette mPalette;

	/** The blocks contained in the area, stored as indices into mPalette. */
	std::vector<UInt32> mBlocks;

	/** The size (dimensions) of the area. */
	Vector3i mSize;


	/** Creates a new uninitialized instance (all sizes zero). */
	PalettedBlockArea();

	/** Converts the position to index in mBlocks.
	This may be removed later on when optimizing the RAM usage of this class by compressing low-palette-count PBAs. */
	UInt32 positionToIndex(Vector3i aPos) const;
};
