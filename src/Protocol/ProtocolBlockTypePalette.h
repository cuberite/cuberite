#pragma once
#include <unordered_map>
#include "../BlockState.h"


/** Parses and holds a collection of block types and their possible states
together with their corresponding Id within the Minecraft network protocol. */
class ProtocolBlockTypePalette
{
public:
	static const UInt32 NOT_FOUND = UINT32_MAX;

	/** Create a new empty instance. */
	ProtocolBlockTypePalette();

	/** Loads the palette from a string.
	See loadFromStream() for further details. */
	bool loadFromString(const AString & aMapping);

	/** Loads the palette from an input stream.
	Returns `true` on success, `false` otherwise. Sucessive calls to this method
	will _add_ data to the palette. If duplicate keys are encountered, they will
	be ignored and an info message logged. */
	bool loadFromStream(std::istream & aInputStream);

	/** Returns the defined index corresponding of the given aBlockTypeName and
	aBlockState.
	Returns ProtocolBlockTypePalette::NOT_FOUND if the tuple is not found. */
	UInt32 index(const AString & aBlockTypeName, const BlockState & aBlockState) const;

	/** Clears the palette. */
	void clear();


protected:

	/** The palette index. Each item in the map represents a single block state
	palette entry. The value is the block state ID. */
	std::unordered_map<AString, std::map<BlockState, UInt32>> mIndex;
};
