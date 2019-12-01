#pragma once

#include <unordered_map>
#include <utility>
#include "BlockState.h"





/** Holds a palette that maps between block type + state and numbers.
Used primarily by PalettedBlockArea to map from stringular block representation to numeric,
and by protocols to map from stringular block representation to protocol-numeric.
The object itself provides no thread safety, users of this class need to handle locking, if required.
Note that the palette itself doesn't support erasing;
to erase, create a new instance and re-add only the wanted items.

Internally, the object uses two synced maps, one for each translation direction. */
class BlockTypePalette
{
public:

	/** Exception that is thrown if requiesting an index not present in the palette. */
	class NoSuchIndexException:
		public std::runtime_error
	{
		using Super = std::runtime_error;

	public:
		NoSuchIndexException(UInt32 aIndex):
			Super(Printf("No such palette index: %u", aIndex))
		{
		}
	};



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
	If the index is not valid, throws a NoSuchIndexException. */
	const std::pair<AString, BlockState> & entry(UInt32 aIndex) const;

	/** Returns an index-transform map from aFrom to this (this.entry(idx) == aFrom.entry(res[idx])).
	Entries from aFrom that are not present in this are added.
	Used when pasting two areas, to transform the src palette to dst palette. */
	std::map<UInt32, UInt32> createTransformMapAddMissing(const BlockTypePalette & aFrom);

	/** Returns an index-transform map from aFrom to this (this.entry(idx) == aFrom.entry(res[idx])).
	Entries from aFrom that are not present in this are assigned the fallback index.
	Used for protocol block type mapping. */
	std::map<UInt32, UInt32> createTransformMapWithFallback(const BlockTypePalette & aFrom, UInt32 aFallbackIndex) const;


protected:

	/** The mapping from numeric to stringular representation.
	mNumberToBlock[index] = {"blockTypeName", blockState}. */
	std::map<UInt32, std::pair<AString, BlockState>> mNumberToBlock;

	/** The mapping from stringular to numeric representation.
	mStringToNumber["blockTypeName"][blockState] = index. */
	std::unordered_map<AString, std::map<BlockState, UInt32>> mBlockToNumber;

	/** The maximum index ever used in the maps.
	Used when adding new entries through the index() call. */
	UInt32 mMaxIndex;
};
