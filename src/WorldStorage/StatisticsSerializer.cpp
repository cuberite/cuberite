
// StatisticsSerializer.cpp


#include "Globals.h"
#include "StatisticsManager.h"
#include "StatisticsSerializer.h"
#include "NamespaceSerializer.h"

#include <json/json.h>





static auto MakeStatisticsDirectory(const std::string & WorldPath, std::string && FileName)
{
	// Even though stats are shared between worlds, they are (usually) saved
	// inside the folder of the default world.

	// Path to the world's statistics folder.
	const auto Path = WorldPath + cFile::GetPathSeparator() + "stats";

	// Ensure that the directory exists.
	cFile::CreateFolder(Path);

	return Path + cFile::GetPathSeparator() + std::move(FileName) + ".json";
}





static void SaveStatToJSON(const StatisticsManager & Manager, Json::Value & a_Out)
{
	if (Manager.Custom.empty())
	{
		// Avoid saving "custom": null to disk:
		return;
	}

	auto & Custom = a_Out["custom"];
	for (const auto & [Statistic, Value] : Manager.Custom)
	{
		Custom[NamespaceSerializer::From(Statistic).data()] = Value;
	}
}





static void LoadCustomStatFromJSON(StatisticsManager & Manager, const Json::Value & a_In)
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
			Manager.Custom[NamespaceSerializer::ToCustomStatistic(StatName)] = it->asUInt();
		}
		catch (const std::out_of_range &)
		{
			FLOGWARNING("Invalid statistic type \"{}\"", StatName);
		}
		catch (const Json::LogicError &)
		{
			FLOGWARNING("Invalid statistic value for type \"{}\"", StatName);
		}
	}
}





void StatisticsSerializer::Load(StatisticsManager & Manager, const std::string & WorldPath, std::string && FileName)
{
	Json::Value Root;
	InputFileStream(MakeStatisticsDirectory(WorldPath, std::move(FileName))) >> Root;

	LoadCustomStatFromJSON(Manager, Root["stats"]["custom"]);
}





void StatisticsSerializer::Save(const StatisticsManager & Manager, const std::string & WorldPath, std::string && FileName)
{
	Json::Value Root;

	SaveStatToJSON(Manager, Root["stats"]);
	Root["DataVersion"] = NamespaceSerializer::DataVersion();

	OutputFileStream(MakeStatisticsDirectory(WorldPath, std::move(FileName))) << Root;
}
