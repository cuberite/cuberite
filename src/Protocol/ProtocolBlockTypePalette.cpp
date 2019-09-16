#include "Globals.h"
#include "ProtocolBlockTypePalette.h"
#include <cstdint>
#include <sstream>
#include "json/value.h"
#include "json/reader.h"





ProtocolBlockTypePalette::ProtocolBlockTypePalette()
{
	// empty
}





ProtocolBlockTypePalette::ProtocolBlockTypePalette(const AString & aMapping)
{
	loadFromString(aMapping);
}





bool ProtocolBlockTypePalette::loadFromString(const AString & aMapping)
{
	Json::Value root;
	std::stringstream strm;
	strm << aMapping;
	strm >> root;

	if (root.empty())
	{
		return true;  // accept empty string.
	}
	if (!root.isMember("Metadata") ||
		!root["Metadata"].isMember("ProtocolBlockType") ||
		(root["Metadata"]["ProtocolBlockType"].asUInt() != 1) ||
		!root.isMember("Palette") ||
		!root["Palette"].isArray())
	{
		LOGD("Wrong palette format.");
		return false;
	}

	auto len = root["Palette"].size();
	for (decltype(len) i = 0; i < len; ++i)
	{
		const auto record = root["Palette"][i];
		ASSERT(record.isObject());
		const auto blocktype = record["name"].asString();
		const auto id = std::stoul(record["id"].asString());
		std::map<AString, AString> state;

		ASSERT(id < NOT_FOUND);  // this is a fatal error.

		if (record.isMember("props"))
		{
			const auto props = record["props"];
			for (const auto key: props.getMemberNames())
			{
				state[key] = props[key].asString();
			}
		}

		// Block type map entry already exists?
		if (mIndex.count(blocktype) == 0)
		{
			mIndex.insert({blocktype, std::map<BlockState, UInt32>()});
		}

		// ASSERT(std::stoul(record["id"].asString()) == i);
		mIndex[blocktype].insert({BlockState(state), id});
	}
	return true;
}





UInt32 ProtocolBlockTypePalette::index(const AString & aBlockTypeName, const BlockState & aBlockState) const
{
	auto a = mIndex.find(aBlockTypeName);
	if (a != mIndex.end())
	{
		auto b = a->second.find(aBlockState);
		if (b != a->second.end())
		{
			return b->second;
		}
	}
	return NOT_FOUND;
}





void ProtocolBlockTypePalette::clear()
{
	return mIndex.clear();
}
