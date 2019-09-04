#pragma once

#include <utility>
#include "BlockState.h"





/** Holds a palette that maps block type + state into numbers.
Used primarily by PalettedBlockArea to translate between numeric and stringular block representation.
The object itself provides no thread safety, users of this class need to handle locking, if required. */
class BlockTypePalette
{
public:

	/** Create a new empty instance. */
	BlockTypePalette();

	/** Returns the index of the specified block type name and state.
	If the combination is not found, it is added to the palette and the new index is returned. */
	UInt32 index(const AString & aBlockTypeName, const BlockState & aBlockState);

	/** Returns the <index, true> of the specified block type name and state, if it exists.
	If the combination is not found, returns <undefined, false>. */
	std::pair<UInt32, bool> maybeIndex(const AString & aBlockTypeName, const BlockState & aBlockState) const;

	/** Returns the total number of entries in the palette. */
	UInt32 count() const;

	/** Returns the blockspec represented by the specified palette index.
	The index must be valid (ASSERTed). */
	const std::pair<AString, BlockState> & entry(UInt32 aIndex) const;

	/** Adds missing entries from aOther to this, and returns an index-transform map from aOther to this.
	Used when pasting two areas, to transform the src palette to dst palette. */
	std::map<UInt32, UInt32> createTransformMap(const BlockTypePalette & aOther);


protected:

	/** The palette. Each item in the vector represents a single entry in the palette, the vector index is the palette index. */
	std::vector<std::pair<AString, BlockState>> mPalette;
};
