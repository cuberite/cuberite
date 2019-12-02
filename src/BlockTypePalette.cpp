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
	// TODO: Detect format (Json vs Lua)
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

	// Check the JSON's metadata + version:
	if (!root.isObject() ||
		!root.isMember("Metadata") ||
		!root["Metadata"].isMember("ProtocolBlockTypePaletteVersion") ||
		!root.isMember("Palette") ||
		!root["Palette"].isArray())
	{
		throw LoadFailedException("Incorrect palette format, wrong or missing metadata.");
	}
	auto version = root["Metadata"]["ProtocolBlockTypePaletteVersion"].asUInt();
	if (version != 1)
	{
		throw(Printf("Palette format version %d not supported.", version));
	}

	// Load the palette:
	auto len = root["Palette"].size();
	for (decltype(len) i = 0; i < len; ++i)
	{
		const auto & record = root["Palette"][i];
		if (!record.isObject())
		{
			throw LoadFailedException(Printf("Palette record #%u is not a JSON object.", i));
		}

		auto blockTypeName = record["name"].asString();
		auto id = static_cast<UInt32>(std::stoul(record["id"].asString()));
		std::map<AString, AString> state;

		if (record.isMember("props"))
		{
			const auto & props = record["props"];
			if (!props.isObject())
			{
				throw LoadFailedException(Printf("Palette record #%u: \"props\" value is not a JSON object.", i));
			}
			for (const auto & key: props.getMemberNames())
			{
				state[key] = props[key].asString();
			}
		}
		BlockState blockState(state);

		// Insert / update in the maps:
		mNumberToBlock[id] = {blockTypeName, blockState};
		mBlockToNumber[blockTypeName][blockState] = id;
		if (id > mMaxIndex)
		{
			mMaxIndex = id;
		}
	}  // for i - Palette[]
}
