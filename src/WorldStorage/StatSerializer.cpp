
// StatSerializer.cpp


#include "Globals.h"
#include "StatSerializer.h"
#include "../Statistics.h"
#include "NamespaceSerializer.h"

#include <fstream>
#include <json/json.h>





cStatSerializer::cStatSerializer(cStatManager & a_Manager, const AString & a_WorldName, const AString & a_FileName)
	: m_Manager(a_Manager)
{
	// Even though stats are shared between worlds, they are (usually) saved
	// inside the folder of the default world.

	AString StatsPath;
	Printf(StatsPath, "%s%cstats", a_WorldName.c_str(), cFile::PathSeparator());

	m_Path = StatsPath + cFile::PathSeparator() + a_FileName + ".json";

	// Ensure that the directory exists.
	cFile::CreateFolder(StatsPath);
}





void cStatSerializer::Load(void)
{
	Json::Value Root;
	std::ifstream(m_Path) >> Root;

	LoadCustomStatFromJSON(Root["stats"]["custom"]);
}





void cStatSerializer::Save(void)
{
	Json::Value Root;

	SaveStatToJSON(Root["stats"]);
	Root["DataVersion"] = NamespaceSerializer::DataVersion();

	std::ofstream(m_Path) << Root;
}





void cStatSerializer::SaveStatToJSON(Json::Value & a_Out)
{
	m_Manager.ForEachStatisticType([&a_Out](const cStatManager::CustomStore & Store)
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





void cStatSerializer::LoadCustomStatFromJSON(const Json::Value & a_In)
{
	for (auto it = a_In.begin() ; it != a_In.end() ; ++it)
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
			m_Manager.SetValue(NamespaceSerializer::ToCustomStatistic(StatName), it->asInt());
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
