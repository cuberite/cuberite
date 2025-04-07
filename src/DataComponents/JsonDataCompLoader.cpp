//
// Created by Filip on 01 / 04 / 25.
//
#include "JsonDataCompLoader.h"

#include "../Items/ItemHandler.h"

#include <filesystem>

namespace DataComponents
{
	static DataComponent LoadMaxStackSize(const Json::Value & comp_root)
	{
		MaxStackSizeComponent maxStackSizeComponent;
		maxStackSizeComponent.maxStackSize = comp_root.asUInt();
		return maxStackSizeComponent;
	}

	static DataComponent LoadMaxDamage(const Json::Value & comp_root)
	{
		MaxDamageComponent max_damage_component;
		max_damage_component.MaxDamage = comp_root.asUInt();
		return max_damage_component;
	}

	typedef DataComponent (*LoadDataComponent)(const Json::Value & comp_root);

	void cDataComponents::LoadFromJson(const AString & a_FileName)
	{
		using namespace std::filesystem;
		if (!exists(a_FileName))
		{
			LOGERROR(fmt::format(FMT_STRING("Failed to find Items.json file in {}. Check if it exists at all. Default item components will not be loaded"), a_FileName));
			return;
		}
		const auto sz = file_size(a_FileName);
		std::fstream file_data(a_FileName.c_str());
		if (!file_data.is_open())
		{
			LOGERROR(fmt::format(FMT_STRING("Failed to open Item.json file {}. Check if it exists at all. Default item components will not be loaded"), a_FileName));
			return;
		}
		if (file_data.fail())
		{
			LOGERROR(fmt::format(FMT_STRING("Failed to read tem.json file {}. Default item components will not be loaded"), a_FileName));
			return;
		}
		std::string result(sz, '\0');
		file_data.read(result.data(), static_cast<std::streamsize>(sz));
		Json::Value RootNode;
		AString json_error;
		JsonUtils::ParseString(result, RootNode, &json_error);
		if (!json_error.empty())
		{
			LOGERROR(fmt::format(FMT_STRING("Failed to parse Item.json file {}. Error: {}"), a_FileName, json_error));
			return;
		}
		LoadFromJsonInternal(RootNode);
	}

	void cDataComponents::LoadFromJsonInternal(const Json::Value & a_FileRoot)
	{
		const static std::map<std::string_view, LoadDataComponent> LoaderMap =
		{
			{"minecraft:max_stack_size", &LoadMaxStackSize},
			{"minecraft:max_damage", &LoadMaxDamage}
		};

		if (a_FileRoot.isNull() || !a_FileRoot.isObject())
		{
			return;
		}
		for (auto CurrItem = a_FileRoot.begin(), end = a_FileRoot.end(); CurrItem != end; ++CurrItem)
		{
			auto comps = CurrItem->get("components", Json::Value());
			if (comps.isNull() || !comps.isObject())
			{
				continue;
			}
			AString item_name = CurrItem.name();
			Item curr_item = NamespaceSerializer::ToItem(item_name.substr(10));
			std::map<int, DataComponent> ToAddComps;
			for (auto CurrentComponent = comps.begin(), end2 = comps.end(); CurrentComponent != end2; ++CurrentComponent)
			{
				AString comp_name = CurrentComponent.name();
				auto CompLoader = LoaderMap.find(comp_name);
				if (CompLoader != LoaderMap.end())
				{
					// Parse the component from JSON and add it to the current item as a default component
					auto CurrComp = (*CompLoader->second)(*CurrentComponent);
					auto [_, HasInserted] = ToAddComps.emplace(static_cast<int>(CurrComp.index()), CurrComp);
					if (!HasInserted)
					{
						LOGWARN("Duplicate data component {} on item {}", comp_name, item_name);
					}
				}
				else
				{
					// LOGWARN(fmt::format("{} data component not implemented on item {}, Ignoring", comp_name, item_name));
				}
			}
			cItemHandler::GetDefaultComponentsMap().AddItem(curr_item, ToAddComps);
		}
	}
}
