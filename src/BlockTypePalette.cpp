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
		return loadFromTsv(aString);
	}
	else if (aString.substr(0, hdrTsvUpgrade.length()) == hdrTsvUpgrade)
	{
		return loadFromTsv(aString);
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





void BlockTypePalette::loadFromTsv(const AString & aTsvPalette)
{
	// Parses the TSV data using a state machine
	// The data is "split" at tabs, CRs and LFs, and at each split,
	// the machine state dictates what to do with the value

	static const AString hdrTsvRegular = "BlockTypePalette";
	static const AString hdrTsvUpgrade = "UpgradeBlockTypePalette";
	enum
	{
		Signature,
		HeaderKey,
		HeaderValue,
		Index,
		Meta,
		BlockTypeName,
		BlockStateKey,
		BlockStateValue
	} state = Signature;
	int line = 1;
	auto len = aTsvPalette.length();
	size_t lastIdx = 0;
	AString currHeaderKey;
	bool isFileVersionPresent = false;
	bool isUpgrade = false;
	AString commonPrefix;
	AString blockTypeName, blockStateKey;
	UInt32 id = 0;
	std::map<AString, AString> blockStatePairs;
	for (size_t i = 0; i < len; ++i)
	{
		auto ch = aTsvPalette[i];
		if (ch == '\t')
		{
			switch (state)
			{
				case Signature:
				{
					throw LoadFailedException("Signature is invalid (contains a TAB)");
				}

				case HeaderKey:
				{
					currHeaderKey = aTsvPalette.substr(lastIdx, i - lastIdx);
					state = HeaderValue;
					break;
				}

				case HeaderValue:
				{
					throw LoadFailedException(Printf("Header value contains TAB (line %d)", line));
				}

				case Index:
				{
					try
					{
						id = static_cast<UInt32>(std::stoul(aTsvPalette.substr(lastIdx, i - lastIdx)));
					}
					catch (const std::exception & exc)
					{
						throw LoadFailedException(Printf("Invalid ID (line %d; %s)", line, exc.what()));
					}
					state = isUpgrade ? Meta : BlockTypeName;
					break;
				}

				case Meta:
				{
					id = id * 16;
					try
					{
						id = id + static_cast<UInt32>(Clamp(std::stoi(aTsvPalette.substr(lastIdx, i - lastIdx)), 0, 15));
					}
					catch (const std::exception & exc)
					{
						throw LoadFailedException(Printf("Invalid Meta (line %d; %s)", line, exc.what()));
					}
					state = BlockTypeName;
					break;
				}

				case BlockTypeName:
				{
					blockTypeName = aTsvPalette.substr(lastIdx, i - lastIdx);
					state = BlockStateKey;
					break;
				}

				case BlockStateKey:
				{
					blockStateKey = aTsvPalette.substr(lastIdx, i - lastIdx);
					state = BlockStateValue;
					break;
				}

				case BlockStateValue:
				{
					blockStatePairs[blockStateKey] = aTsvPalette.substr(lastIdx, i - lastIdx);
					blockStateKey.clear();
					state = BlockStateKey;
					break;
				}
			}
			lastIdx = i + 1;
		}
		else if ((ch == '\n') || (ch == '\r'))
		{
			if ((ch == '\n') && (i > 0) && (aTsvPalette[i - 1] == '\r'))
			{
				// This is the LF of a CRLF, skip it:
				lastIdx = i + 1;
				continue;
			}
			switch (state)
			{
				case Signature:
				{
					auto signature = aTsvPalette.substr(lastIdx, i - lastIdx);
					if (signature == hdrTsvRegular)
					{
						isUpgrade = false;
					}
					else if (signature == hdrTsvUpgrade)
					{
						isUpgrade = true;
					}
					else
					{
						throw LoadFailedException("Unknown signature");
					}
					state = HeaderKey;
					break;
				}  // case Signature

				case HeaderKey:
				{
					if (lastIdx == i)
					{
						// This is the end of the headers
						if (!isFileVersionPresent)
						{
							throw LoadFailedException("FileVersion is missing");
						}
						state = Index;
					}
					else
					{
						throw LoadFailedException(Printf("Header without a value (line %d)", line));
					}
					break;
				}  // case HeaderKey

				case HeaderValue:
				{
					auto headerValue = aTsvPalette.substr(lastIdx, i - lastIdx);
					if (currHeaderKey == "FileVersion")
					{
						auto fileVersion = std::stoi(headerValue);
						if (fileVersion != 1)
						{
							throw LoadFailedException(Printf("Unsupported FileVersion: %d", fileVersion));
						}
						isFileVersionPresent = true;
					}
					else if (currHeaderKey == "CommonPrefix")
					{
						commonPrefix = headerValue;
					}
					state = HeaderKey;
					break;
				}  // case HeaderValue

				case Index:
				{
					throw LoadFailedException(Printf("Incomplete line, ended after Index (line %d)", line));
				}

				case Meta:
				{
					throw LoadFailedException(Printf("Incomplete line, ended after Meta (line %d)", line));
				}

				case BlockTypeName:
				{
					addMapping(id, commonPrefix + aTsvPalette.substr(lastIdx, i - lastIdx), {});
					state = Index;
					break;
				}

				case BlockStateKey:
				{
					throw LoadFailedException(Printf("Incomplete line, ended after BlockStateKey (line %d)", line));
				}

				case BlockStateValue:
				{
					blockStatePairs[blockStateKey] = aTsvPalette.substr(lastIdx, i - lastIdx);
					addMapping(id, commonPrefix + blockTypeName, blockStatePairs);
					blockStatePairs.clear();
					state = Index;
					break;
				}
			}  // switch (state)
			lastIdx = i + 1;
			line = line + 1;
		}  // if \r or \n
	}  // for i - aTsvPalette[]
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
