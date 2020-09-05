
// cLootTableProvider.cpp

#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "LootTableProvider.h"
#include "LootTable.h"
#include "JsonUtils.h"
#include "json/json.h"




////////////////////////////////////////////////////////////////////////////////
// cLootTableProvider

cLootTableProvider::cLootTableProvider(AString & a_Path, cWorld * a_World):
	m_World(a_World)
{
	LOG("Loading loot tables...");
	// Load default loot tables
	for (const auto & FileName: LootTable::FileNames)
	{
		auto FilePathWithPrefix = AString("LootTables") + cFile::PathSeparator() + FileName;
		auto Data = cFile::ReadWholeFile(FilePathWithPrefix);
		if (!Data.empty())
		{
			LoadLootTable(Data, FileName);
		}
		else
		{
			// Todo: write better error message
			LOGERROR("Could not find default loot table: %s! "
			"Please make sure the file is readable or download them from cuberite.org", FilePathWithPrefix);
		}
	}

	// Check for custom tables
	for (auto & FileName: LootTable::FileNames)
	{
		auto FilePathWithWorld = a_Path + cFile::PathSeparator() + AString("LootTables") + cFile::PathSeparator() + FileName;
		auto Data = cFile::ReadWholeFile(FilePathWithWorld);
		if (!Data.empty())
		{
			LOG("Found custom loot table: %s", FilePathWithWorld);
			LoadLootTable(Data, FileName);
		}
	}
	LOG("FINISHED");
}





cLootTableProvider::~cLootTableProvider()
{
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
				LootTable::Replace(Name, AString ("Chests") + cFile::PathSeparator(), "");
				LootTable::Replace(Name, ".json", "");
				const auto ChestType = LootTable::eChestType(Name);
				m_ChestLootTables.insert(std::make_pair(ChestType, cLootTable(JsonObject, m_World)));

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
		default:
		{
			LOGWARNING("Trying to use unsupported or unknown loot table type: %s", Data[1]);
			return & m_EmptyLootTable;
		}
	}
}





cLootTable * cLootTableProvider::GetLootTable(enum LootTable::eChestType a_Type)
{
	if (a_Type != LootTable::eChestType::None)
	{
		return & (m_ChestLootTables.at(a_Type));
	}
	return & m_EmptyLootTable;
}




