#include "Globals.h"
#include "ProtocolBlockTypePalette.h"
#include <cstdint>
#include <sstream>
#include <json/value.h>
#include <json/reader.h>





ProtocolBlockTypePalette::ProtocolBlockTypePalette()
{
	// empty
}





ProtocolBlockTypePalette::ProtocolBlockTypePalette(const AString & aMapping)
{
	load(aMapping);
}





bool ProtocolBlockTypePalette::loadFromString(const AString & aMapping)
{
	try
	{
		return load(aMapping);
	}
	catch (const std::exception& e)
	{
		LOGD("Error reading protocol palette: %s", e.what());
		return false;
	}
}





bool ProtocolBlockTypePalette::load(const std::string & aMapping)
{
	Json::Value root;
	std::stringstream strm;
	strm << aMapping;
	strm >> root;

	if (root.empty())
	{
		return true;  // accept empty string.
	}
	if (!root.isArray())
	{
		return false;
	}

	auto len = root.size();
	for (decltype(len) i=0; i<len; i++)
	{
		auto record = root[i];
		auto blocktype = record["name"].asString();
		std::map<AString, AString> state;

		ASSERT(i != NOT_FOUND);  // this is a fatal error.

		if (record.isMember("props"))
		{
			auto props = record["props"];
			for (auto key: props.getMemberNames())
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
		mIndex[blocktype].insert({BlockState(state), i});
	}
	return true;
}





bool ProtocolBlockTypePalette::is_initialized()
{
	return !mIndex.empty();
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
