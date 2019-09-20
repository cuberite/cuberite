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





bool ProtocolBlockTypePalette::loadFromString(const AString & aMapping)
{
	std::stringstream stream;
	stream << aMapping;

	return loadFromStream(stream);
}





bool ProtocolBlockTypePalette::loadFromStream(std::istream & aInputStream)
{
	Json::Value root;

	try
	{
		aInputStream >> root;
	}
	#if defined _DEBUG
	catch (const std::exception & e)
	{
		LOGD(e.what());
		return false;
	}
	#else
	catch (const std::exception &)
	{
		return false;
	}
	#endif

	if (!root.isObject() ||
		!root.isMember("Metadata") ||
		!root["Metadata"].isMember("ProtocolBlockTypePaletteVersion") ||
		!root.isMember("Palette") ||
		!root["Palette"].isArray())
	{
		LOGD("Incorrect palette format.");
		return false;
	}

	if (root["Metadata"]["ProtocolBlockTypePaletteVersion"].asUInt() != 1)
	{
		LOGD("Palette format version not supported.");
		return false;
	}

	auto len = root["Palette"].size();
	for (decltype(len) i = 0; i < len; ++i)
	{
		const auto & record = root["Palette"][i];
		if (!record.isObject())
		{
			LOGD("Record #%u must be a JSON object.", i);
			return false;
		}

		auto blocktype = record["name"].asString();
		auto id = std::stoul(record["id"].asString());
		std::map<AString, AString> state;

		if (id >= NOT_FOUND)
		{
			LOGD("`id` must be less than ProtocolBlockTypePalette::NOT_FOUND, but is %lu", id);
			return false;
		}

		if (record.isMember("props"))
		{
			const auto & props = record["props"];
			if (!props.isObject())
			{
				LOGD("`props` key must be a JSON object.");
				return false;
			}
			for (const auto & key: props.getMemberNames())
			{
				state[key] = props[key].asString();
			}
		}

		// Block type map entry already exists?
		if (mIndex.count(blocktype) == 0)
		{
			mIndex.insert({blocktype, std::map<BlockState, UInt32>()});
		}

		const auto & result = mIndex[blocktype].insert({BlockState(state), id});
		if (result.second == false)
		{
			LOGINFO("Duplicate block state encountered (Current ID: %lu, other: %lu)", result.first->second, id);
		}
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
