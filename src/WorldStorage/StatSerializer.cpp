
// StatSerializer.cpp


#include "Globals.h"
#include "../Statistics.h"
#include "NamespaceSerializer.h"

#include <json/json.h>





namespace StatSerializer
{
	auto MakeStatisticsDirectory(const std::string & WorldPath, std::string && FileName)
	{
		// Even though stats are shared between worlds, they are (usually) saved
		// inside the folder of the default world.

		// Path to the world's statistics folder.
		const auto Path = WorldPath + cFile::GetPathSeparator() + "stats";

		// Ensure that the directory exists.
		cFile::CreateFolder(Path);

		return Path + cFile::GetPathSeparator() + std::move(FileName) + ".json";
	}





	void SaveStatToJSON(const cStatManager & Manager, Json::Value & a_Out)
	{
		Manager.ForEachStatisticType([&a_Out](const cStatManager::CustomStore & Store)
		{
			if (Store.empty())
			{
				// Avoid saving "custom": null to disk:
				return;
			}

			auto & Custom = a_Out["custom"];
			for (const auto & Item : Store)
			{
				Custom[NamespaceSerializer::From(Item.first)] = Item.second;
			}
		});
	}





	void LoadCustomStatFromJSON(cStatManager & Manager, const Json::Value & a_In)
	{
		for (auto it = a_In.begin(); it != a_In.end(); ++it)
		{
			const auto & Key = it.key().asString();
			const auto StatInfo = NamespaceSerializer::SplitNamespacedID(Key);
			if (StatInfo.first == NamespaceSerializer::Namespace::Unknown)
			{
				// Ignore non-Vanilla, non-Cuberite namespaces for now:
				continue;
			}

			const auto & StatName = StatInfo.second;
			try
			{
				Manager.SetValue(NamespaceSerializer::ToCustomStatistic(StatName), it->asInt());
			}
			catch (const std::out_of_range & Oops)
			{
				FLOGWARNING("Invalid statistic type \"{}\"", StatName);
			}
			catch (const Json::LogicError & Oops)
			{
				FLOGWARNING("Invalid statistic value for type \"{}\"", StatName);
			}
		}
	}





	void Load(cStatManager & Manager, const std::string & WorldPath, std::string && FileName)
	{
		Json::Value Root;
		InputFileStream(MakeStatisticsDirectory(WorldPath, std::move(FileName))) >> Root;

		LoadCustomStatFromJSON(Manager, Root["stats"]["custom"]);
	}





	void Save(const cStatManager & Manager, const std::string & WorldPath, std::string && FileName)
	{
		Json::Value Root;

		SaveStatToJSON(Manager, Root["stats"]);
		Root["DataVersion"] = NamespaceSerializer::DataVersion();

		OutputFileStream(MakeStatisticsDirectory(WorldPath, std::move(FileName))) << Root;
	}
}
