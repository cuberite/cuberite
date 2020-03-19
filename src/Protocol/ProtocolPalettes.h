#pragma once

#include "../OSSupport/CriticalSection.h"





// fwd:
class BlockTypePalette;





/** Loads the protocol-specific palettes on startup and provides them to the individual protocol
instances when they are created.
Uses the data in the $/Server/Protocol folder. Each protocol version has a subfolder there,
containing possibly multiple palette files. All the files are loaded in sequence (alpha-sorted),
into the palette corresponding to the file's extension (*.btp.txt -> BlockTypePalette).
Provides thread safety for the data properly. */
class ProtocolPalettes
{
public:

	/** Loads all the per-protocol palettes.
	aProtocolFolder is the folder that contains a subfolder for each protocol version;
	each subfolder contains the protocol-specific palettes (as in $/Server/Protocol)
	If a protocol version is already loaded, yet present in the folder, the newly loaded data is merged
	into the current data.
	Always succeeds (even when there are no palettes). */
	void load(const AString & aProtocolFolder);

	/** Returns the BlockTypePalette for the specified protocol.
	Returns nullptr if no such palette has been loaded. */
	std::shared_ptr<const BlockTypePalette> blockTypePalette(const AString & aProtocolVersion) const;

	/** Returns the version names of all protocols that have been loaded. */
	std::vector<AString> protocolVersions() const;


protected:

	/** Container for all palettes for a single protocol. */
	struct Palettes
	{
		std::shared_ptr<BlockTypePalette> mBlockTypePalette;
		// TODO: ItemTypePalette

		Palettes();
	};


	/** The CS protecting all members against multithreaded access. */
	mutable cCriticalSection mCS;

	/** The map of protocol version -> all its palettes. */
	std::map<AString, Palettes> mPalettes;


	/** Loads all the palettes from the specified folder into mPalettes under the aProtocolVersion key. */
	void loadSingleVersion(const AString & aProtocolVersion, const AString & aFolder);
};
