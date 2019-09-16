#pragma once
#include <unordered_map>
#include "../BlockState.h"


/** Holds a palette that maps block type + state into IDs.

Init / parsing is comparatively expensive, lookup (index) is O(1) when
considering palette size as free variable.
*/
class ProtocolBlockTypePalette
{
public:
	static const UInt32 NOT_FOUND = UINT32_MAX;

	/** Create a new empty instance. */
	ProtocolBlockTypePalette();

	/** Create a new instance and initialise with aMapping. */
	ProtocolBlockTypePalette(const AString & aMapping);

	/** Returns the block state index for {aBlockTypeName, aBlockState}.
	Returns `true` on success, `false` otherwise. */
	bool loadFromString(const AString & aMapping);

	/** Returns the index of the specified {block type name, state}.
	Returns ProtocolBlockTypePalette::NOT_FOUND if the tuple is not found. */
	UInt32 index(const AString & aBlockTypeName, const BlockState & aBlockState) const;

	/** Clears the palette. */
	void clear();


protected:

	/** The palette index. Each item in the map represents a single block state
	palette entry.
	The key is the combination of Name and serialised BlockState, the value is
	the palette index. */
	std::unordered_map<AString, std::map<BlockState, UInt32>> mIndex;
};
