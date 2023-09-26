
// StatisticsSerializer.cpp


#include "Globals.h"
#include "StatisticsManager.h"
#include "StatisticsSerializer.h"
#include "NamespaceSerializer.h"

#include <json/json.h>




// Helper function to construct the statistics directory path
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




// Helper function to save custom statistics to JSON
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




// Helper function to load custom statistics from JSON
static void LoadCustomStatFromJSON(StatisticsManager & Manager, const Json::Value & a_In)
{
	for (auto it = a_In.begin(); it != a_In.end(); ++it)
	{
		const auto & Key = it.key().asString();
		const auto & [Namespace, Name] = NamespaceSerializer::SplitNamespacedID(Key);

		if (Namespace == NamespaceSerializer::Namespace::Unknown)
		{
			// Ignore non-Vanilla, non-Cuberite namespaces for now:
			continue;
		}

		Manager.Custom[NamespaceSerializer::ToCustomStatistic(Name)] = it->asUInt();
	}
}




// Load statistics from a file
void StatisticsSerializer::Load(StatisticsManager & Manager, const std::string & WorldPath, std::string && FileName)
{
	Json::Value Root;
	InputFileStream(MakeStatisticsDirectory(WorldPath, std::move(FileName))) >> Root;

	LoadCustomStatFromJSON(Manager, Root["stats"]["custom"]);
}




// Save statistics to a file
void StatisticsSerializer::Save(const StatisticsManager & Manager, const std::string & WorldPath, std::string && FileName)
{
	Json::Value Root;

	SaveStatToJSON(Manager, Root["stats"]);
	Root["DataVersion"] = NamespaceSerializer::DataVersion();

	OutputFileStream(MakeStatisticsDirectory(WorldPath, std::move(FileName))) << Root;
}
