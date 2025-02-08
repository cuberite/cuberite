#pragma once
#include "Globals.h"
#include <filesystem>
#include "WorldStorage/NamespaceSerializer.h"
#include "JsonUtils.h"
#include "json/value.h"
#include "Protocol/Packetizer.h"

namespace TagRegistry
{
	constexpr size_t PrefixLength = std::char_traits<char>::length("minecraft:");
	constexpr size_t RefPrefixLength = std::char_traits<char>::length("#minecraft:");
	constexpr size_t FileSuffixLength = std::char_traits<char>::length(".json");

	using namespace std::filesystem;
	template <class TagType, auto ConversionFunc>
	class cTagRegistryEntry
	{
	public:
		// So we can access private fields
		template <class TagType2, auto ConversionFunc2, class TagEnum, auto EnumToString>
		friend class cTagRegistry;

		cTagRegistryEntry() = default;
		cTagRegistryEntry(const directory_entry & a_File, const AString & a_RegistryName)
		{
			const AString parent_name = a_File.path().parent_path().filename().string();
			AString file_name = a_File.path().filename().string();
			if (parent_name != a_RegistryName)
			{
				file_name = parent_name + "/" + file_name;
			}
			// LOGD(fmt::format(FMT_STRING("fn: {} -- n2: {}"), file_name, n2));
			m_Name = file_name.substr(0, file_name.size() - FileSuffixLength);
			std::fstream file_data(a_File.path().c_str());
			if (!file_data.is_open())
			{
				LOGERROR(fmt::format(FMT_STRING("Failed to open tags file {}. Check if it exists at all. This tag will not be loaded"), file_name));
				return;
			}
			if (file_data.fail())
			{
				LOGERROR(fmt::format(FMT_STRING("Failed to read tags file {}. This tag will not be loaded"), file_name));
				return;
			}
			const auto sz = file_size(a_File);
			std::string result(sz, '\0');
			file_data.read(result.data(), static_cast<std::streamsize>(sz));
			Json::Value RootNode;
			AString json_error;
			JsonUtils::ParseString(result, RootNode, &json_error);
			if (!json_error.empty())
			{
				LOGERROR(fmt::format(FMT_STRING("Failed to parse tags file {}. Error: {}"), file_name, json_error));
				return;
			}
			auto values = RootNode.get("values", Json::Value());
			if (values.type() == Json::nullValue)
			{
				LOGERROR(fmt::format(FMT_STRING("Failed to get values object in json file tags file {}."), file_name));
				return;
			}
			if (values.empty())
			{
				// LOGWARN(fmt::format(FMT_STRING("Empty values array in json file tags {}."), file_name));
				return;
			}
			for (const auto & value  : values)
			{
				if (value.asString()[0] == '#')
				{
					m_UnresolvedRef.push_back(value.asString().substr(RefPrefixLength));
					continue;
				}
				auto current_v = AString(value.asString().substr(PrefixLength));
				m_Values.push_back(ConversionFunc(current_v));  // Removes the prefix
			}
		}

		[[nodiscard]] const std::vector<TagType> & GetAllValues() const { return m_Values; }
		[[nodiscard]] const AString & GetName() { return m_Name; }
	protected:
		void AddValue(TagType a_Value) { m_Values.push_back(a_Value); }
		void AddValues(const std::vector<TagType> & a_Values) { m_Values.insert(m_Values.begin(), a_Values.begin(), a_Values.end()); }
		[[nodiscard]] const std::vector<AString> & GetUnresolvedRef() const { return m_UnresolvedRef; }
	private:
		AString m_Name;
		std::vector<TagType> m_Values;
		std::vector<AString> m_UnresolvedRef;
	};

	template <class TagType, auto ConversionFunc, class TagEnum, auto EnumToString>
	class cTagRegistry
	{

	public:
		[[nodiscard]] const AString & GetName() const
		{
			return m_RegistryName;
		}

		[[nodiscard]] const std::vector<cTagRegistryEntry<TagType, ConversionFunc>> & GetAllTags() const
		{
			return m_Tags;
		}

		//  TODO: use binary search instead of linear
		//  also vector should be initialized with proper capacities where possible
		[[nodiscard]] const cTagRegistryEntry<TagType, ConversionFunc> * GetTag(const AString & m_TagName) const
		{
			for (const auto & tag : m_Tags)
			{
				if (tag.m_Name == m_TagName)
				{
					return &tag;
				}
			}
			LOGERROR(fmt::format(FMT_STRING("Tag {} not found in registry {}."), m_TagName, m_RegistryName));
			return nullptr;
		}

		// TODO: optimize
		[[nodiscard]] bool HasTag(TagEnum a_TagName, TagType a_ToQuery) const
		{
			std::string_view TagName = EnumToString(a_TagName);
			for (auto & tag : m_Tags)
			{
				if (tag.m_Name == TagName)
				{
					for (auto & value : tag.m_Values)
					{
						if (value == a_ToQuery)
						{
							return true;
						}
					}
					return false;
				}
			}
			return false;
		}

		void LoadFromFiles(const AString & tags_path, const AString & a_RegistryName)
		{
			m_RegistryName = a_RegistryName;
			if (!exists(path(tags_path)))
			{
				return;
			}
			recursive_directory_iterator files(tags_path);
			for (const auto & dir_entry : files)
			{
				if (dir_entry.is_directory() || dir_entry.path().filename().extension() != ".json")
				{
					continue;
				}
				m_Tags.push_back(cTagRegistryEntry<TagType, ConversionFunc>(dir_entry, m_RegistryName));
			}
			bool has_resolved = false;
			do
			{
				has_resolved = false;
				for (auto & tag : m_Tags)
				{
					std::vector<AString> new_ur = tag.GetUnresolvedRef();
					for (auto & ref : tag.GetUnresolvedRef())
					{
						if (ref == tag.GetName())
						{
							LOGERROR(fmt::format(FMT_STRING("Self referencing json file tags {}."), ref));
							return;
						}
						auto values_to_ad = GetTag(ref);
						if (values_to_ad == nullptr)
						{
							LOGERROR(fmt::format(FMT_STRING("Failed to resolve reference {} in json file tags {}."), ref, tag.GetName()));
							return;
						}
						tag.AddValues(values_to_ad->GetAllValues());
						new_ur.erase(std::remove(new_ur.begin(), new_ur.end(), ref), new_ur.end());
						auto ref_to_add = values_to_ad->GetUnresolvedRef();
						new_ur.insert(new_ur.end(), ref_to_add.begin(), ref_to_add.end());
						has_resolved = true;
					}
					tag.m_UnresolvedRef = new_ur;
				}
			} while (has_resolved);
		}

		template <UInt32 ToNetworkValue(TagType)>
		void WriteTags(cPacketizer & a_Packet) const
		{
			a_Packet.WriteString("minecraft:" + m_RegistryName);
			a_Packet.WriteVarInt32(static_cast<UInt32>(m_Tags.size()));
			for (const auto & tag : m_Tags)
			{
				a_Packet.WriteString(tag.m_Name);
				// TODO: don't write values that don't exist for this version
				a_Packet.WriteVarInt32(static_cast<UInt32>(tag.m_Values.size()));
				for (const auto & value : tag.m_Values)
				{
					a_Packet.WriteVarInt32(ToNetworkValue(value));
				}
			}
		}
	private:
		AString m_RegistryName;
		std::vector<cTagRegistryEntry<TagType, ConversionFunc>> m_Tags;
	};
}
