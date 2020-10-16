
// ItemTags.cpp

#include "Globals.h"

#include "ItemTags.h"
#include "../IniFile.h"

cItemTags::cItemTags()
{
	cIniFile Ini;
	if (!Ini.ReadFile("items.ini"))
	{
		UNREACHABLE("Missing Items.ini file, please download with the newest version of cuberite!");
	}
	int KeyID = Ini.FindKey("ItemTags");
	if (KeyID == cIniFile::noID)
	{
		UNREACHABLE("Missing Item tags in item.ini, please download with the newest version of cuberite!");
	}
	int NumValues = Ini.GetNumValues(KeyID);
	for (int i = 0; i < NumValues; i++)
	{
		AString Name = Ini.GetValueName(KeyID, i);
		if (Name.empty())
		{
			continue;
		}
		AString Value = Ini.GetValue(KeyID, i);
		AddToMap(Name, Value);
	}  // for i - Ini.Values[]
}





const cItems cItemTags::Resolve(const AString & a_Name) const
{
	auto It = m_Map.find(a_Name);
	if (It != m_Map.end())
	{
		cItems Result;
		const auto & Items = It->second;
		for (const auto & Item : Items)
		{
			std::visit(
				overloaded
				{
					[&] (const AString & a_ItemTagName)
					{
						// Just to prevent infinite recursion
						if (NoCaseCompare(a_Name, a_ItemTagName) == 0)
						{
							return;
						}
						auto SubResult = Resolve(a_ItemTagName);
						Result.insert(Result.end(), SubResult.begin(), SubResult.end());
					},
					[&] (const cItem & a_Item) { Result.Add(a_Item); }
				}
			, Item);
		}
		return Result;
	}
	LOGWARNING("Item tags: Got unknown item tag: %s!", a_Name);
	return cItems();
}





void cItemTags::AddToMap(const AString & a_Name, const AString & a_Value)
{
	const auto Items = StringSplitAndTrim(a_Value, "|");
	cItemTag Value;
	for (const auto & ItemString : Items)
	{
		cItem Item;
		// If there is no item present save it as a string to map
		if (!StringToItem(ItemString, Item))
		{
			Value.emplace_back(ItemString);
		}
		else
		{
			Value.emplace_back(Item);
		}
	}
	m_Map[a_Name] = Value;
}
