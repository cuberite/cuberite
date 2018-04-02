
// StatSerializer.cpp


#include "Globals.h"
#include "StatSerializer.h"

#include "../Statistics.h"





cStatSerializer::cStatSerializer(const AString & a_WorldName, const AString & a_PlayerName, const AString & a_FileName, cStatManager * a_Manager)
	: m_Manager(a_Manager)
{
	// Even though stats are shared between worlds, they are (usually) saved
	// inside the folder of the default world.

	AString StatsPath;
	Printf(StatsPath, "%s%cstats", a_WorldName.c_str(), cFile::PathSeparator());

	m_LegacyPath = StatsPath + "/" + a_PlayerName + ".json";
	m_Path = StatsPath + "/" + a_FileName + ".json";

	// Ensure that the directory exists.
	cFile::CreateFolder(FILE_IO_PREFIX + StatsPath);
}





bool cStatSerializer::Load(void)
{
	AString Data = cFile::ReadWholeFile(FILE_IO_PREFIX + m_Path);
	if (Data.empty())
	{
		Data = cFile::ReadWholeFile(FILE_IO_PREFIX + m_LegacyPath);
		if (Data.empty())
		{
			return false;
		}
	}

	Json::Value Root;
	Json::Reader Reader;

	if (Reader.parse(Data, Root, false))
	{
		return LoadStatFromJSON(Root);
	}

	return false;
}





bool cStatSerializer::Save(void)
{
	Json::Value Root;
	SaveStatToJSON(Root);

	cFile File;
	if (!File.Open(FILE_IO_PREFIX + m_Path, cFile::fmWrite))
	{
		return false;
	}

	Json::StyledWriter Writer;
	AString JsonData = Writer.write(Root);

	File.Write(JsonData.data(), JsonData.size());
	File.Close();

	return true;
}





void cStatSerializer::SaveStatToJSON(Json::Value & a_Out)
{
	for (unsigned int i = 0; i < static_cast<unsigned int>(statCount); ++i)
	{
		StatValue Value = m_Manager->GetValue(static_cast<eStatistic>(i));

		if (Value != 0)
		{
			const AString & StatName = cStatInfo::GetName(static_cast<eStatistic>(i));

			a_Out[StatName] = Value;
		}

		// TODO 2014-05-11 xdot: Save "progress"
	}
}





bool cStatSerializer::LoadStatFromJSON(const Json::Value & a_In)
{
	m_Manager->Reset();

	for (Json::Value::const_iterator it = a_In.begin() ; it != a_In.end() ; ++it)
	{
		AString StatName = it.key().asString();

		eStatistic StatType = cStatInfo::GetType(StatName);

		if (StatType == statInvalid)
		{
			LOGWARNING("Invalid statistic type \"%s\"", StatName.c_str());
			continue;
		}

		const Json::Value & Node = *it;

		if (Node.isInt())
		{
			m_Manager->SetValue(StatType, Node.asInt());
		}
		else if (Node.isObject())
		{
			StatValue Value = Node.get("value", 0).asInt();

			// TODO 2014-05-11 xdot: Load "progress"

			m_Manager->SetValue(StatType, Value);
		}
		else
		{
			LOGWARNING("Invalid statistic value for type \"%s\"", StatName.c_str());
		}
	}

	return true;
}








