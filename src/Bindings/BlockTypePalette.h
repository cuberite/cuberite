#pragma once

#include <utility>
#include "BlockState.h"





/** Holds a palette that maps between block type + state and numbers.
Used primarily by PalettedBlockArea to map from stringular block representation to numeric,
and by protocols to map from stringular block representation to protocol-numeric.
The object itself provides no thread safety, users of this class need to handle locking, if required.
Note that the palette itself doesn't support erasing;
to erase, create a new instance and re-add only the wanted items.

Internally, the object uses two synced maps, one for each translation direction.

The palette can be loaded from a string (file). The loader supports either the blocks.json file exported by
the vanilla server itself (https://minecraft.wiki/w/Minecraft_Wiki:Projects/wiki.vg_merge/Data_Generators), or a processed text file generated by
our tool $/Tools/BlockTypePaletteGenerator/, or a hand-written text file describing the upgrade from
1.12 BlockType + BlockMeta to 1.13 string representations.
The text file is a TSV (tab-separated values), which basically means the data is generally structured as
<value1><tab><value2><tab><value3><tab>...<valueN><eol>, where eol is the platform's CR / CRLF / LF lineend.
The file starts with a single value on the first line, "BlockTypePalette" or "UpgradeBlockTypePalette", which
is used to detect the file format. The following lines are "headers", simple <key><tab><value><eol> entries
that contain the metadata about the file. "FileVersion" is a compulsory key, "CommonPrefix" is supported, others
are ignored.
The headers are followed by an empty line (that signalizes the end of headers) and then the actual data.
For regular BlockTypePalette TSV file of version 1, the data is in the format:
<index><tab><blockTypeName><tab><state1Name><tab><state1Value><tab><state2Name> ... <eol>
For the UpgradeBlockTypePalette TSV file of version 1, the data is in the format:
<blockType><tab><blockMeta><tab><blockTypeName><tab><state1Name><tab><state1Value><tab><state2Name> ... <eol>
If a CommonPrefix header is present, its value is pre-pended to each blockTypeName loaded (thus allowing
the file to be overall smaller). */
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
			Super(fmt::format(FMT_STRING("No such palette index: {}"), aIndex))
		{
		}
	};


	/** Exception that is thrown when loading the palette fails hard (bad format). */
	class LoadFailedException:
		public std::runtime_error
	{
		using Super = std::runtime_error;

	public:
		LoadFailedException(const AString & aReason):
			Super(aReason)
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

	/** Loads the palette from the string representation.
	Throws a LoadFailedException if the loading fails hard (bad string format);
	but still a part of the data may already be loaded at that point.
	If the string specifies duplicate entries (either to already existing entries, or to itself),
	the duplicates replace the current values silently (this allows us to chain multiple files as "overrides".
	Auto-detects the string format (json / tsv, normal / upgrade palette) and calls the appropriate load function. */
	void loadFromString(const AString & aString);


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


	/** Loads the palette from the JSON representation, https://minecraft.wiki/w/Minecraft_Wiki:Projects/wiki.vg_merge/Data_Generators
	Throws a LoadFailedException if the loading fails hard (bad string format);
	but still a part of the data may already be loaded at that point.
	See also: loadFromString(). */
	void loadFromJsonString(const AString & aJsonPalette);

	/** Loads the palette from the regular or upgrade TSV representation.
	aIsUpgrade specifies whether the format is an upgrade TSV (true) or a regular one (false)
	Throws a LoadFailedException if the loading fails hard (bad string format);
	but still a part of the data may already be loaded at that point.
	See also: loadFromString(). */
	void loadFromTsv(const AString & aTsvPalette, bool aIsUpgrade);

	/** Adds a mapping between the numeric and stringular representation into both maps,
	updates the mMaxIndex, if appropriate.
	Silently overwrites any previous mapping for the ID, if present, but keeps the old string->id mapping. */
	void addMapping(UInt32 aID, const AString & aBlockTypeName, const BlockState & aBlockState);
};
