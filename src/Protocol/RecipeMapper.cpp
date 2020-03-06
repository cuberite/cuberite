#include "Globals.h"
#include "RecipeMapper.h"
#include "../Root.h"

cRecipeMapper::cRecipeMapper(void)
{
	cFile f;
	if (!f.Open("Protocol/1.12.2/base.recipes.txt", cFile::fmRead))
	{
		LOGWARNING("Cannot open file \"Protocol/1.12.2/base.recipes.txt\", no recipe book recipes will be available!");
		return;
	}
	AString Everything;
	if (!f.ReadRestOfFile(Everything))
	{
		LOGWARNING("Cannot read file \"Protocol/1.12.2/base.recipes.txt\", no recipe book recipes will be available!");
		return;
	}
	f.Close();

	// Split it into lines, then process each line as a single recipe:
	AStringVector Split = StringSplit(Everything, "\n");
	m_ProtocolVersionMap["1.12.2"] = {};
	auto RecipeNameMap = cRoot::Get()->GetCraftingRecipes()->getRecipeNameMap();

	int LineNum = 1;
	for (AStringVector::const_iterator itr = Split.begin(); itr != Split.end(); ++itr, ++LineNum)
	{
		// Remove anything after a '#' sign and trim away the whitespace:
		AString Recipe = TrimString(itr->substr(0, itr->find('#')));
		if (Recipe.empty())
		{
			// Empty recipe
			continue;
		}
		AddRecipeLine("1.12.2", LineNum, Recipe, RecipeNameMap);
	}
	LOG("Loaded 1.12.2 %zu recipe book", m_ProtocolVersionMap["1.12.2"].size(), LineNum);
}





cRecipeMapper::~cRecipeMapper()
{
}





void cRecipeMapper::AddRecipeLine(const AString & a_ProtocolVersion, int a_LineNum, const AString & a_RecipeLine, std::map<AString, UInt32> RecipeNameMap)
{
	AStringVector Sides = StringSplit(a_RecipeLine, " ");
	UInt32 Id;
	if (Sides.size() != 2)
	{
		LOGINFO("Recipe incompletely configured %s", a_RecipeLine);
	}
	StringToInteger<UInt32>(Sides[0].c_str(), Id);
	const UInt32 RecipeIndex = RecipeNameMap[Sides[1]];
	m_ProtocolVersionMap[a_ProtocolVersion].emplace(Id, RecipeIndex);
}





UInt32 cRecipeMapper::GetProtocolRecipeId(UInt32 a_RecipeId, UInt32 a_ProtocolVersion)
{
	auto ProtocolMap = m_ProtocolVersionMap.find(cRoot::Get()->GetProtocolVersionTextFromInt(static_cast<int>(a_ProtocolVersion)));
	if (ProtocolMap == m_ProtocolVersionMap.end())
	{
		return 0;
	}
	for (const auto & item: ProtocolMap->second)
	{
		if (item.second == a_RecipeId)
		{
			return item.first;
		}
	}
	return 0;
}





UInt32 cRecipeMapper::GetCuberiteRecipeId(UInt32 a_ProtocolRecipeId, UInt32 a_ProtocolVersion)
{
	auto ProtocolMap = m_ProtocolVersionMap.find(cRoot::Get()->GetProtocolVersionTextFromInt(static_cast<int>(a_ProtocolVersion)));
	if (ProtocolMap == m_ProtocolVersionMap.end())
	{
		return 0;
	}
	auto Element = ProtocolMap->second.find(a_ProtocolRecipeId);
	if (Element != ProtocolMap->second.end())
	{
		return Element->second;
	}
	return 0;
}
