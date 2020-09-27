
// cLootTableProvider.cpp

#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "LootTableProvider.h"
#include "JsonUtils.h"
#include "World.h"




////////////////////////////////////////////////////////////////////////////////
// cLootTableProvider

cLootTable cLootTableProvider::m_EmptyLootTable = cLootTable();





cLootTableProvider::cLootTableProvider(cWorld & a_World):
	m_World(a_World)
{
	ReloadLootTables();
}





void cLootTableProvider::ReloadLootTables()
{
	LOG("Loading loot tables...");
	// Load default loot tables
	LoadLootTablesFromFolder(LootTable::LootTablePath);

	// Check for custom tables
	LoadLootTablesFromFolder(m_World.GetDataPath() + cFile::PathSeparator() + LootTable::LootTablePath);
}





void cLootTableProvider::LoadLootTable(const AString & a_Path, const AString & a_Name)
{
	auto Data = cFile::ReadWholeFile(a_Path);
	if (Data.empty())
	{
		LOGWARNING("There was an error on loading loot table %s", a_Path);
	}
	AString ErrorMessage;
	Json::Value JsonObject;
	JsonUtils::ParseString(Data, JsonObject, & ErrorMessage);
	if (!ErrorMessage.empty() || !JsonObject.isObject())
	{
		LOGWARNING(ErrorMessage);
	}
	else
	{
		auto Name = a_Name;
		ReplaceString(Name, ".json", "");
		m_LootTables[Name] = cLootTable(JsonObject);
	}
}





const cLootTable * cLootTableProvider::GetLootTable(const AString & a_Name)
{
	try
	{
		return & (m_LootTables.at(a_Name));
	}
	catch (const std::out_of_range)
	{
		if (
			FindLootTable(LootTable::LootTablePath, a_Name) ||
			FindLootTable(LootTable::LootTablePath + cFile::GetPathSeparator() + m_World.GetDataPath(), a_Name)
			)
		{
			try
			{
				return & (m_LootTables.at(a_Name));
			}
			catch (const std::out_of_range)
			{
				LOGWARNING("Got unknown string for loot table: %s. Returning empty", a_Name);
				return & m_EmptyLootTable;
			}
		}
		LOGWARNING("Got unknown string for loot table: %s. Returning empty", a_Name);
		return & m_EmptyLootTable;
	}
}





void cLootTableProvider::LoadLootTablesFromFolder(const AString & a_Path)
{
	if (!cFile::IsFolder(a_Path))
	{
		return;
	}

	for (const auto & FileName : cFile::GetFolderContents(a_Path))
	{
		if (cFile::IsFolder(a_Path + cFile::GetPathSeparator() + FileName))
		{
			LoadLootTablesFromFolder(a_Path + cFile::GetPathSeparator() + FileName);
		}
		else
		{
			LoadLootTable(a_Path + cFile::GetPathSeparator() + FileName, FileName);
		}
	}
}





bool cLootTableProvider::FindLootTable(const AString & a_Path, const AString & a_FileName)
{
	if (!cFile::IsFolder(a_Path))
	{
		return false;
	}

	for (const auto & FileName : cFile::GetFolderContents(a_Path))
	{
		LOG(a_FileName + ".json");
		if (cFile::IsFolder(a_Path + cFile::GetPathSeparator() + FileName))
		{
			LoadLootTablesFromFolder(a_Path + cFile::GetPathSeparator() + FileName);
		}
		else
		{
			if ((a_FileName + ".json") == FileName)
			{
				LoadLootTable(a_Path + cFile::GetPathSeparator() + FileName, FileName);
				return true;
			}
		}
	}
	return false;
}




