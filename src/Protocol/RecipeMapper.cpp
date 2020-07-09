#include "Globals.h"
#include "RecipeMapper.h"
#include "../Root.h"

cRecipeMapper::cRecipeMapper(void)
{
	AString path = "Protocol";
	auto contents = cFile::GetFolderContents(path);
	for (const auto & content: contents)
	{
		auto fullName = path + cFile::PathSeparator() + content;
		if (cFile::IsFolder(fullName))
		{
			loadRecipes(content);
		}
	}
}





void cRecipeMapper::loadRecipes(const AString & a_ProtocolVersion)
{
	cFile f;
	if (!f.Open("Protocol/" + a_ProtocolVersion + "/base.recipes.txt", cFile::fmRead))
	{
		LOGWARNING("Cannot open file \"Protocol/%s/base.recipes.txt\", no recipe book recipes will be available!", a_ProtocolVersion);
		return;
	}
	AString Everything;
	if (!f.ReadRestOfFile(Everything))
	{
		LOGWARNING("Cannot read file \"Protocol/%s/base.recipes.txt\", no recipe book recipes will be available!", a_ProtocolVersion);
		return;
	}
	f.Close();

	// Split it into lines, then process each line as a single recipe:
	AStringVector Split = StringSplit(Everything, "\n");
	m_ProtocolVersionMap[a_ProtocolVersion] = {};
	const auto & RecipeNameMap = cRoot::Get()->GetCraftingRecipes()->GetRecipeNameMap();

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
		AddRecipeLine(a_ProtocolVersion, LineNum, Recipe, RecipeNameMap);
	}
	LOG("Loaded %s %zu recipe book", a_ProtocolVersion, m_ProtocolVersionMap[a_ProtocolVersion].size());
}





cRecipeMapper::~cRecipeMapper()
{
}





void cRecipeMapper::AddRecipeLine(const AString & a_ProtocolVersion, int a_LineNum, const AString & a_RecipeLine, const std::map<AString, UInt32> & a_RecipeNameMap)
{
	AStringVector Sides = StringSplit(a_RecipeLine, " ");
	UInt32 Id;
	if (Sides.size() != 2)
	{
		LOGINFO("Recipe incompletely configured %s", a_RecipeLine);
		return;
	}
	StringToInteger<UInt32>(Sides[0], Id);

	auto RecipeIndex = a_RecipeNameMap.find(Sides[1]);
	if (RecipeIndex == a_RecipeNameMap.end())
	{
		return;
	}
	m_ProtocolVersionMap[a_ProtocolVersion].emplace(Id, RecipeIndex->second);
}





std::optional<UInt32> cRecipeMapper::GetProtocolRecipeId(UInt32 a_RecipeId, UInt32 a_ProtocolVersion)
{
	auto ProtocolMap = m_ProtocolVersionMap.find(cRoot::Get()->GetProtocolVersionTextFromInt(static_cast<int>(a_ProtocolVersion)));
	if (ProtocolMap == m_ProtocolVersionMap.end())
	{
		return {};
	}
	for (const auto & item: ProtocolMap->second)
	{
		if (item.second == a_RecipeId)
		{
			return item.first;
		}
	}
	return {};
}





std::optional<UInt32> cRecipeMapper::GetCuberiteRecipeId(UInt32 a_ProtocolRecipeId, UInt32 a_ProtocolVersion)
{
	auto ProtocolMap = m_ProtocolVersionMap.find(cRoot::Get()->GetProtocolVersionTextFromInt(static_cast<int>(a_ProtocolVersion)));
	if (ProtocolMap == m_ProtocolVersionMap.end())
	{
		return {};
	}
	auto Element = ProtocolMap->second.find(a_ProtocolRecipeId);
	if (Element != ProtocolMap->second.end())
	{
		return Element->second;
	}
	return {};
}
