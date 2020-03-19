#include "Globals.h"
#include "ProtocolPalettes.h"
#include "../BlockTypePalette.h"





void ProtocolPalettes::load(const AString & aProtocolFolder)
{
	auto contents = cFile::GetFolderContents(aProtocolFolder);
	for (const auto & c: contents)
	{
		auto fullName = aProtocolFolder + cFile::PathSeparator() + c;
		if (cFile::IsFolder(fullName))
		{
			loadSingleVersion(c, fullName);
		}
	}
}





std::shared_ptr<const BlockTypePalette> ProtocolPalettes::blockTypePalette(const AString & aProtocolVersion) const
{
	cCSLock lock(mCS);
	auto itr = mPalettes.find(aProtocolVersion);
	if (itr == mPalettes.end())
	{
		return nullptr;
	}
	return itr->second.mBlockTypePalette;
}





std::vector<AString> ProtocolPalettes::protocolVersions() const
{
	cCSLock lock(mCS);

	std::vector<AString> res;
	for (const auto & p: mPalettes)
	{
		res.push_back(p.first);
	}
	return res;
}





void ProtocolPalettes::loadSingleVersion(const AString & aProtocolVersion, const AString & aFolder)
{
	// Get the file list, sort by name
	auto contents = cFile::GetFolderContents(aFolder);
	std::sort(contents.begin(), contents.end());

	// Load files into the palettes:
	cCSLock lock(mCS);
	auto & pal = mPalettes[aProtocolVersion];
	for (const auto & c: contents)
	{
		if (c.length() < 8)
		{
			// Name too short, can't have the ".btp.txt" etc. suffix
			continue;
		}
		auto fnam = aFolder + cFile::PathSeparator() + c;
		if (!cFile::IsFile(fnam))
		{
			continue;
		}
		auto fileType = c.substr(c.length() - 8);
		if ((fileType == ".btp.txt") || (c == "blocks.json"))
		{
			try
			{
				pal.mBlockTypePalette->loadFromString(cFile::ReadWholeFile(fnam));
			}
			catch (...)
			{
				// Ignore silently
			}
		}
		else if ((fileType == ".itp.txt") || (c == "items.json"))
		{
			// TODO: Load item type palette
		}
	}
}





////////////////////////////////////////////////////////////////////////////////
// ProtocolPalettes::Palettes:

ProtocolPalettes::Palettes::Palettes():
	mBlockTypePalette(new BlockTypePalette)
{
}
