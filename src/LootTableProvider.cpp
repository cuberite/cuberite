
// cLootTableProvider.cpp

#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "LootTableProvider.h"
#include "JsonUtils.h"




////////////////////////////////////////////////////////////////////////////////
// cLootTableProvider

cLootTableProvider::cLootTableProvider(AString & a_Path, cWorld * a_World):
	m_World(a_World)
{
	LOG("Loading loot tables...");
	// Load default loot tables
	for (const auto & FileName: LootTable::FileNames)
	{
		auto Data = cFile::ReadWholeFile(FileName);
		if (!Data.empty())
		{
			LoadLootTable(Data, FileName);
		}
		else
		{
			// Todo: write better error message
			LOGERROR("Could not find default loot table: %s! "
			"Please make sure the file is readable or download them from cuberite.org", FileName);
		}
	}
	// Check for global user tables
	auto UserTables = cFile::GetFolderContents(LootTable::PathToUserLootTables);
	for (const auto & UserTable : UserTables)
	{
		auto Data = cFile::ReadWholeFile(LootTable::PathToUserLootTables + cFile::PathSeparator() + UserTable);
		if (!Data.empty())
		{
			LOGINFO("Found global user loot table: %s", LootTable::PathToUserLootTables + cFile::PathSeparator() + UserTable);
			LoadLootTable(Data, UserTable);
		}
	}


	// Check for custom tables
	for (auto & FileName: LootTable::FileNames)
	{
		auto FilePathWithWorld = a_Path + cFile::PathSeparator() + FileName;
		auto Data = cFile::ReadWholeFile(FilePathWithWorld);
		if (!Data.empty())
		{
			LOGINFO("Found custom loot table: %s", FilePathWithWorld);
			LoadLootTable(Data, FileName);
		}
	}
}





cLootTable cLootTableProvider::m_EmptyLootTable = cLootTable();





void cLootTableProvider::LoadLootTable(const AString & a_String, const AString & a_Type)
{
	AString ErrorMessage;
	Json::Value JsonObject;
	JsonUtils::ParseString(a_String, JsonObject, & ErrorMessage);
	if (!ErrorMessage.empty() || !JsonObject.isObject())
	{
		LOGERROR(ErrorMessage);
	}
	else
	{
		switch (LootTable::eType(LootTable::NamespaceConverter(JsonObject["type"].asString())))
		{
			case LootTable::eType::Chest:
			{
				auto Name = a_Type;
				ReplaceString(Name, AString("LootTables") + cFile::PathSeparator() + "Chests" + cFile::PathSeparator(), "");
				ReplaceString(Name, ".json", "");
				const auto ChestType = LootTable::eChestType(Name);
				m_ChestLootTables[ChestType] = cLootTable(JsonObject, m_World);
				break;
			}
			case LootTable::eType::User:
			{
				auto Name = a_Type;
				ReplaceString(Name, AString("LootTables") + cFile::PathSeparator() + "User", "");
				ReplaceString(Name, ".json", "");
				m_UserLootTables[Name] = cLootTable(JsonObject, m_World);
				break;
			}
			default:
			{
				LOGWARNING("This loot table type is not supported: %s", LootTable::NamespaceConverter(JsonObject["type"].asString()));
				break;
			}
		}
	}
}





const cLootTable * cLootTableProvider::GetLootTable(const AString & a_Name)
{
	auto Data = StringSplitAndTrim(a_Name,"|");

	if (Data.size() != 2)
	{
		LOGWARNING("Got ill formatted string: \"%s\" to look up a loot table.\n"
			"Please use Type|Subtype. Returning empty loot table!", a_Name);
		return & m_EmptyLootTable;
	}

	auto Type = LootTable::eType(Data[0]);

	switch (Type)
	{
		case LootTable::eType::Chest: return GetLootTable(LootTable::eChestType(Data[1]));
		case LootTable::eType::User:  return GetUserLootTable(Data[1]);
		default:
		{
			LOGWARNING("Trying to use unsupported or unknown loot table type: %s", Data[1]);
			return & m_EmptyLootTable;
		}
	}
}





const cLootTable * cLootTableProvider::GetLootTable(enum LootTable::eChestType a_Type)
{
	if (a_Type != LootTable::eChestType::None)
	{
		return & (m_ChestLootTables.at(a_Type));
	}
	return & m_EmptyLootTable;
}





const cLootTable * cLootTableProvider::GetUserLootTable(const AString & a_String)
{
	try
	{
		return & (m_UserLootTables.at(a_String));
	}
	catch (const std::out_of_range)
	{
		LOGWARNING("Got unknown string for user type loot table: %s. Returning empty", a_String);
		return & m_EmptyLootTable;
	}
}




