#include "Globals.h"
#include "BlockTypePalette.h"
#include "json/value.h"
#include "JsonUtils.h"





/** Returns the index into aString >= aStartIdx at which the next separator occurs.
Separator is one of \t, \n or \r.
Returns AString::npos if no such separator. */
static size_t findNextSeparator(const AString & aString, size_t aStartIdx = 0)
{
	for (size_t i = aStartIdx, len = aString.length(); i < len; ++i)
	{
		switch (aString[i])
		{
			case '\t':
			case '\n':
			case '\r':
			{
				return i;
			}
		}
	}
	return AString::npos;
}





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
	std::string errs;
	if (!JsonUtils::ParseString(aJsonPalette, root, &errs))
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
	static const AString hdrTsvRegular = "BlockTypePalette";
	static const AString hdrTsvUpgrade = "UpgradeBlockTypePalette";

	// Check the file signature:
	auto idx = findNextSeparator(aTsvPalette);
	if ((idx == AString::npos) || (aTsvPalette[idx] == '\t'))
	{
		throw LoadFailedException("Invalid signature");
	}
	auto signature = aTsvPalette.substr(0, idx);
	bool isUpgrade = (signature == hdrTsvUpgrade);
	if (!isUpgrade && (signature != hdrTsvRegular))
	{
		throw LoadFailedException("Unknown signature");
	}
	if (aTsvPalette[idx] == '\r')   // CR of the CRLF pair, skip the LF:
	{
		idx += 1;
	}

	// Parse the header:
	bool hasHadVersion = false;
	AString commonPrefix;
	int line = 2;
	auto len = aTsvPalette.length();
	while (true)
	{
		auto keyStart = idx + 1;
		auto keyEnd = findNextSeparator(aTsvPalette, idx + 1);
		if (keyEnd == AString::npos)
		{
			throw LoadFailedException(Printf("Invalid header key format on line %u", line));
		}
		if (keyEnd == idx + 1)  // Empty line, end of headers
		{
			if (aTsvPalette[keyEnd] == '\r')  // CR of the CRLF pair, skip the LF:
			{
				++keyEnd;
			}
			idx = keyEnd;
			++line;
			break;
		}
		auto valueEnd = findNextSeparator(aTsvPalette, keyEnd + 1);
		if ((valueEnd == AString::npos) || (aTsvPalette[valueEnd] == '\t'))
		{
			throw LoadFailedException(Printf("Invalid header value format on line %u", line));
		}
		auto key = aTsvPalette.substr(keyStart, keyEnd - keyStart);
		if (key == "FileVersion")
		{
			unsigned version = 0;
			auto value = aTsvPalette.substr(keyEnd + 1, valueEnd - keyEnd - 1);
			if (!StringToInteger(value, version))
			{
				throw LoadFailedException("Invalid FileVersion value");
			}
			else if (version != 1)
			{
				throw LoadFailedException(Printf("Unknown FileVersion: %u. Only version 1 is supported.", version));
			}
			hasHadVersion = true;
		}
		else if (key == "CommonPrefix")
		{
			commonPrefix = aTsvPalette.substr(keyEnd + 1, valueEnd - keyEnd - 1);
		}
		idx = valueEnd;
		if (aTsvPalette[idx] == '\r')  // CR of the CRLF pair, skip the LF:
		{
			++idx;
		}
		++line;
	}
	if (!hasHadVersion)
	{
		throw LoadFailedException("No FileVersion value");
	}

	// Parse the data:
	while (idx + 1 < len)
	{
		auto lineStart = idx + 1;
		auto idEnd = findNextSeparator(aTsvPalette, lineStart);
		if ((idEnd == AString::npos) || (aTsvPalette[idEnd] != '\t'))
		{
			throw LoadFailedException(Printf("Incomplete data on line %u (id)", line));
		}
		UInt32 id;
		if (!StringToInteger(aTsvPalette.substr(lineStart, idEnd - lineStart), id))
		{
			throw LoadFailedException(Printf("Failed to parse id on line %u", line));
		}
		size_t metaEnd = idEnd;
		if (isUpgrade)
		{
			metaEnd = findNextSeparator(aTsvPalette, idEnd + 1);
			if ((metaEnd == AString::npos) || (aTsvPalette[metaEnd] != '\t'))
			{
				throw LoadFailedException(Printf("Incomplete data on line %u (meta)", line));
			}
			UInt32 meta = 0;
			if (!StringToInteger(aTsvPalette.substr(idEnd + 1, metaEnd - idEnd - 1), meta))
			{
				throw LoadFailedException(Printf("Failed to parse meta on line %u", line));
			}
			if (meta > 15)
			{
				throw LoadFailedException(Printf("Invalid meta value on line %u: %u", line, meta));
			}
			id = (id * 16) | meta;
		}
		auto blockTypeEnd = findNextSeparator(aTsvPalette, metaEnd + 1);
		if (blockTypeEnd == AString::npos)
		{
			throw LoadFailedException(Printf("Incomplete data on line %u (blockTypeName)", line));
		}
		auto blockTypeName = aTsvPalette.substr(metaEnd + 1, blockTypeEnd - metaEnd - 1);
		auto blockStateEnd = blockTypeEnd;
		AStringMap blockState;
		while (aTsvPalette[blockStateEnd] == '\t')
		{
			auto keyEnd = findNextSeparator(aTsvPalette, blockStateEnd + 1);
			if ((keyEnd == AString::npos) || (aTsvPalette[keyEnd] != '\t'))
			{
				throw LoadFailedException(Printf("Incomplete data on line %u (blockState key)", line));
			}
			auto valueEnd = findNextSeparator(aTsvPalette, keyEnd + 1);
			if (valueEnd == AString::npos)
			{
				throw LoadFailedException(Printf("Incomplete data on line %u (blockState value)", line));
			}
			auto key = aTsvPalette.substr(blockStateEnd + 1, keyEnd - blockStateEnd - 1);
			auto value = aTsvPalette.substr(keyEnd + 1, valueEnd - keyEnd - 1);
			blockState[key] = value;
			blockStateEnd = valueEnd;
		}
		addMapping(id, commonPrefix + blockTypeName, std::move(blockState));
		++line;
		if (aTsvPalette[blockStateEnd] == '\r')  // CR of the CRLF pair, skip the LF:
		{
			++blockStateEnd;
		}
		idx = blockStateEnd;
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
