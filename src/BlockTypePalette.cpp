#include "Globals.h"
#include "BlockTypePalette.h"
#include "json/value.h"
#include "json/reader.h"





BlockTypePalette::BlockTypePalette():
	mMaxIndex(0)
{
}





UInt32 BlockTypePalette::index(const AString & aBlockTypeName, const BlockState & aBlockState)
{
	auto idx = maybeIndex(aBlockTypeName, aBlockState);
	if (idx.second)
	{
		return idx.first;
	}

	// Not found, append:
	auto index = mMaxIndex++;
	mBlockToNumber[aBlockTypeName][aBlockState] = index;
	mNumberToBlock[index] = {aBlockTypeName, aBlockState};
	return index;
}





std::pair<UInt32, bool> BlockTypePalette::maybeIndex(const AString & aBlockTypeName, const BlockState & aBlockState) const
{
	auto itr1 = mBlockToNumber.find(aBlockTypeName);
	if (itr1 == mBlockToNumber.end())
	{
		return {0, false};
	}
	auto itr2 = itr1->second.find(aBlockState);
	if (itr2 == itr1->second.end())
	{
		return {0, false};
	}
	return {itr2->second, true};
}





UInt32 BlockTypePalette::count() const
{
	return static_cast<UInt32>(mNumberToBlock.size());
}





const std::pair<AString, BlockState> & BlockTypePalette::entry(UInt32 aIndex) const
{
	auto itr = mNumberToBlock.find(aIndex);
	if (itr == mNumberToBlock.end())
	{
		throw NoSuchIndexException(aIndex);
	}
	return itr->second;
}





std::map<UInt32, UInt32> BlockTypePalette::createTransformMapAddMissing(const BlockTypePalette & aFrom)
{
	std::map<UInt32, UInt32> res;
	for (const auto & fromEntry: aFrom.mNumberToBlock)
	{
		auto fromIndex = fromEntry.first;
		const auto & blockTypeName = fromEntry.second.first;
		const auto & blockState = fromEntry.second.second;
		res[fromIndex] = index(blockTypeName, blockState);
	}
	return res;
}





std::map<UInt32, UInt32> BlockTypePalette::createTransformMapWithFallback(const BlockTypePalette & aFrom, UInt32 aFallbackIndex) const
{
	std::map<UInt32, UInt32> res;
	for (const auto & fromEntry: aFrom.mNumberToBlock)
	{
		auto fromIndex = fromEntry.first;
		const auto & blockTypeName = fromEntry.second.first;
		const auto & blockState = fromEntry.second.second;
		auto thisIndex = maybeIndex(blockTypeName, blockState);
		if (thisIndex.second)
		{
			// The entry was found in this
			res[fromIndex] = thisIndex.first;
		}
		else
		{
			// The entry was NOT found in this, replace with fallback:
			res[fromIndex] = aFallbackIndex;
		}
	}
	return res;
}





void BlockTypePalette::loadFromString(const AString & aString)
{
	static const AString hdrTsvRegular = "BlockTypePalette";
	static const AString hdrTsvUpgrade = "UpgradeBlockTypePalette";

	// Detect format by checking the header line (none -> JSON):
	if (aString.substr(0, hdrTsvRegular.length()) == hdrTsvRegular)
	{
		return loadFromTsv(aString, false);
	}
	else if (aString.substr(0, hdrTsvUpgrade.length()) == hdrTsvUpgrade)
	{
		return loadFromTsv(aString, true);
	}
	return loadFromJsonString(aString);
}





void BlockTypePalette::loadFromJsonString(const AString & aJsonPalette)
{
	// Parse the string into JSON object:
	Json::Value root;
	Json::CharReaderBuilder builder;
	std::unique_ptr<Json::CharReader> reader(builder.newCharReader());
	std::string errs;
	if (!reader->parse(aJsonPalette.data(), aJsonPalette.data() + aJsonPalette.size(), &root, &errs))
	{
		throw LoadFailedException(errs);
	}

	// Sanity-check the JSON's structure:
	if (!root.isObject())
	{
		throw LoadFailedException("Incorrect palette format, expected an object at root.");
	}

	// Load the palette:
	for (auto itr = root.begin(), end = root.end(); itr != end; ++itr)
	{
		const auto & blockTypeName = itr.name();
		const auto & states = (*itr)["states"];
		if (states == Json::Value())
		{
			throw LoadFailedException(Printf("Missing \"states\" for block type \"%s\"", blockTypeName));
		}
		for (const auto & state: states)
		{
			auto id = static_cast<UInt32>(std::stoul(state["id"].asString()));
			std::map<AString, AString> props;
			if (state.isMember("properties"))
			{
				const auto & properties = state["properties"];
				if (!properties.isObject())
				{
					throw LoadFailedException(Printf("Member \"properties\" is not a JSON object (block type \"%s\", id %u).", blockTypeName, id));
				}
				for (const auto & key: properties.getMemberNames())
				{
					props[key] = properties[key].asString();
				}
			}
			addMapping(id, blockTypeName, props);
		}
	}
}





void BlockTypePalette::loadFromTsv(const AString & aTsvPalette, bool aIsUpgrade)
{
	auto lines = StringSplitAndTrim(aTsvPalette, "\n");

	// Parse the header:
	int fileVersion = 0;
	AString commonPrefix;
	auto numLines = lines.size();
	for (size_t idx = 1; idx < numLines; ++idx)
	{
		const auto & line = lines[idx];
		if (line.empty())
		{
			// End of headers, erase them from lines[] and go parse the data
			lines.erase(lines.begin(), lines.begin() + static_cast<AStringVector::difference_type>(idx) + 1);
			break;
		}
		auto s = StringSplit(line, "\t");
		if (s.size() != 2)
		{
			throw LoadFailedException(Printf("Invalid header format on line %u", idx + 1));
		}
		if (s[0] == "FileVersion")
		{
			try
			{
				fileVersion = std::stoi(s[1]);
			}
			catch (const std::exception & exc)
			{
				throw LoadFailedException(Printf("Invalid file version: \"%d\" (%s)", s[1], exc.what()));
			}
		}
		else if (s[0] == "CommonPrefix")
		{
			commonPrefix = s[1];
		}
	}
	if (fileVersion != 1)
	{
		throw LoadFailedException(Printf("Unknown file version (%d), only version 1 is supported", fileVersion));
	}

	// Parse the data:
	size_t minSplit = aIsUpgrade ? 3 : 2;
	for (const auto & line: lines)
	{
		auto s = StringSplit(line, "\t");
		auto numSplit = s.size();
		if (numSplit < minSplit)
		{
			throw LoadFailedException(Printf("Not enough values on data line: \"%s\"", line));
		}
		UInt32 id;
		try
		{
			id = static_cast<UInt32>(std::stoi(s[0]));
		}
		catch (const std::exception & exc)
		{
			throw LoadFailedException(Printf("Invalid block ID: \"%s\" (%s)", s[0], exc.what()));
		}
		size_t idx = 1;
		if (aIsUpgrade)
		{
			id = id * 16;
			try
			{
				id = id + static_cast<UInt32>(Clamp(std::stoi(s[1]), 0, 15));
			}
			catch (const std::exception & exc)
			{
				throw LoadFailedException(Printf("Invalid block meta: \"%s\" (%s)", s[1], exc.what()));
			}
			idx = 2;
		}
		const auto & blockTypeName = s[idx];
		idx += 1;
		std::map<AString, AString> state;
		while (idx + 1 < numSplit)
		{
			state[s[idx]] = s[idx + 1];
			idx += 2;
		}
		addMapping(id, commonPrefix + blockTypeName, state);
	}
}





void BlockTypePalette::addMapping(UInt32 aID, const AString & aBlockTypeName, const BlockState & aBlockState)
{
	mNumberToBlock[aID] = {aBlockTypeName, aBlockState};
	mBlockToNumber[aBlockTypeName][aBlockState] = aID;
	if (aID > mMaxIndex)
	{
		mMaxIndex = aID;
	}
}
