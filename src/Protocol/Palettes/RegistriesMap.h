#pragma once
#include "../Protocol.h"
#include <filesystem>
#include "JsonUtils.h"
#include "json/value.h"
#include "Protocol/ProtocolRecognizer.h"

namespace RegistriesMap
{
	constexpr size_t PrefixLength = std::char_traits<char>::length("minecraft:");
	class cVersionSpecificMap
	{
public:
		void LoadFromJson(const Json::Value & a_Root, const AString & file_name)
		{
			LoadItems(a_Root, file_name);
		}

		void LoadItems(const Json::Value & a_Root, const AString & file_name)
		{
			auto items = a_Root.get("minecraft:item", Json::Value());
			if (items.type() == Json::nullValue)
			{
				LOGERROR(fmt::format(FMT_STRING("Failed to get minecraft:item object in json file registries file {}."), file_name));
				return;
			}
			items = items.get("entries", Json::Value());
			if (items.type() == Json::nullValue)
			{
				LOGERROR(fmt::format(FMT_STRING("Failed to get entries in items object in json registries tags file {}."), file_name));
				return;
			}
			for (auto itr = items.begin(), end = items.end(); itr != end; ++itr)
			{
				auto Item = NamespaceSerializer::ToItem(itr.name().substr(PrefixLength));
				auto id = itr->get("protocol_id", Json::Value()).asInt();

				m_ToProtocolItemMap[Item] = id;
				m_ProtocolToItemMap[id] = Item;
			}
		}

		UInt32 ItemToProtocol(Item a_Item)
		{
			return m_ToProtocolItemMap[a_Item];
		}

		Item ProtocolToItem(UInt32 a_Item)
		{
			return m_ProtocolToItemMap[a_Item];
		}

	private:
		std::map<Item, UInt32> m_ToProtocolItemMap;
		std::map<UInt32, Item> m_ProtocolToItemMap;
	};

	class cRegistryHandler
	{
public:
		void LoadAll()
		{
			using namespace std::filesystem;
			auto dir = path("Protocol/");
			directory_iterator end_itr(dir);
			for (auto version : end_itr)
			{
				if (version.is_directory())
				{
					LoadFromFile(version.path().string());
				}
			}
		}

		void LoadFromFile(AString a_Path)
		{
			using namespace std::filesystem;
			auto version = cMultiVersionProtocol::GetVersionFromText(path(a_Path).filename().string());
			if (version == static_cast<cProtocol::Version>(-1))
			{
				LOGERROR(fmt::format(FMT_STRING("Failed to get version from path {}. This registry will not be loaded"), a_Path));
				return;
			}
			if (version < cProtocol::Version::v1_13)
			{
				return;
			}
			auto File = path(a_Path + "/registries.json");
			AString file_name = File.filename().string();
			if (!exists(File))
			{
				LOGERROR(fmt::format(FMT_STRING("Failed to find registries file {}. Check if it exists at all. This registry will not be loaded"), File.string()));
				return;
			}
			std::fstream file_data(File.c_str());
			if (!file_data.is_open())
			{
				LOGERROR(fmt::format(FMT_STRING("Failed to open tags file {}. Check if it exists at all. This tag will not be loaded"), File.string()));
				return;
			}
			if (file_data.fail())
			{
				LOGERROR(fmt::format(FMT_STRING("Failed to read tags file {}. This tag will not be loaded"), File.string()));
				return;
			}
			const auto sz = file_size(File);
			std::string result(sz, '\0');
			file_data.read(result.data(), static_cast<std::streamsize>(sz));
			Json::Value RootNode;
			AString json_error;
			JsonUtils::ParseString(result, RootNode, &json_error);
			if (!json_error.empty())
			{
				LOGERROR(fmt::format(FMT_STRING("Failed to parse tags file {}. Error: {}"), File.string(), json_error));
				return;
			}
			LOGD(fmt::format(FMT_STRING("Loading registries for version {}"), path(a_Path).filename().string()));
			m_VersionSpecificMap[version] = cVersionSpecificMap();
			m_VersionSpecificMap[version].LoadFromJson(RootNode, file_name);
		}

		UInt32 ItemToProtocol(Item a_Item, cProtocol::Version a_Version)
		{
			if (m_VersionSpecificMap.count(a_Version) == 0)
			{
				LOGERROR(fmt::format(FMT_STRING("Tried to get Id for item {} for Version: {}. But the version is not loaded returning 0"), NamespaceSerializer::From(a_Item), cMultiVersionProtocol::GetVersionTextFromInt(a_Version)));
				return 0;
			}
			return m_VersionSpecificMap[a_Version].ItemToProtocol(a_Item);
		}

		Item ProtocolToItem(UInt32 a_ID, cProtocol::Version a_Version)
		{
			if (m_VersionSpecificMap.count(a_Version) == 0)
			{
				LOGERROR(fmt::format(FMT_STRING("Tried to get Item from id {} for Version: {}. But the version is not loaded returning Air"), a_ID, cMultiVersionProtocol::GetVersionTextFromInt(a_Version)));
				return Item::Air;
			}
			return m_VersionSpecificMap[a_Version].ProtocolToItem(a_ID);
		}
private:
		std::map<cProtocol::Version, cVersionSpecificMap> m_VersionSpecificMap;
	};
}
