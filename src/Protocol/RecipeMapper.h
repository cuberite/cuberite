#pragma once

#include "../CraftingRecipes.h"

class cRecipeMapper
{
public:
	cRecipeMapper(void);
	~cRecipeMapper();
	UInt32 GetProtocolRecipeId(UInt32 a_RecipeId, UInt32 a_ProtocolVersion);
	UInt32 GetCuberiteRecipeId(UInt32 a_ProtocolRecipeId, UInt32 a_ProtocolVersion);

private:
	std::map<AString, std::map<UInt32, UInt32>> m_ProtocolVersionMap;
	void AddRecipeLine(const AString & a_ProtocolVersion, int a_LineNum, const AString & a_RecipeLine, std::map<AString, UInt32> RecipeNameMap);

};
