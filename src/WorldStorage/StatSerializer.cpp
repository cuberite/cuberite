
// StatSerializer.cpp


#include "Globals.h"
#include "StatSerializer.h"

#include "../Statistics.h"

#include <fstream>





cStatSerializer::cStatSerializer(const AString& a_WorldName, const AString& a_PlayerName, cStatManager* a_Manager)
	: m_Manager(a_Manager)
{
	AString StatsPath;
	Printf(StatsPath, "%s/stats", a_WorldName.c_str());

	m_Path = StatsPath + "/" + a_PlayerName + ".dat";

	/* Ensure that the directory exists. */
	cFile::CreateFolder(FILE_IO_PREFIX + StatsPath);
}





bool cStatSerializer::Load(void)
{
	AString Data = cFile::ReadWholeFile(FILE_IO_PREFIX + m_Path);
	if (Data.empty())
	{
		return false;
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
	for (unsigned int i = 0; i < (unsigned int)statCount; ++i)
	{
		StatValue Value = m_Manager->GetValue((eStatistic) i);

		if (Value != 0)
		{
			const AString & StatName = cStatInfo::GetName((eStatistic) i);

			a_Out[StatName] = Value;
		}
	}
}





bool cStatSerializer::LoadStatFromJSON(const Json::Value & a_In)
{
	m_Manager->Reset();

	for (Json::ValueIterator it = a_In.begin() ; it != a_In.end() ; ++it)
	{
		AString StatName = it.key().asString();

		eStatistic StatType = cStatInfo::GetType(StatName);

		if (StatType == statInvalid)
		{
			LOGWARNING("Invalid statistic type %s", StatName.c_str());
			continue;
		}

		m_Manager->SetValue(StatType, (*it).asInt());
	}

	return true;
}








