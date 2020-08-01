
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "BrewingRecipes.h"

#include <fstream>

#define BREWING_RECIPE_FILE "brewing.txt"





cBrewingRecipes::cBrewingRecipes()
{
	ReloadRecipes();
}





void cBrewingRecipes::ReloadRecipes(void)
{
	ClearRecipes();
	LOGD("Loading brewing recipes...");

	std::ifstream f(BREWING_RECIPE_FILE, std::ios::in);
	if (!f.good())
	{
		LOG("Could not open the brewing recipes file \"%s\". No brewing recipes are available.", BREWING_RECIPE_FILE);
		return;
	}

	unsigned int LineNum = 0;
	AString ParsingLine;

	while (std::getline(f, ParsingLine))
	{
		LineNum++;
		// Remove comments from the line:
		size_t FirstCommentSymbol = ParsingLine.find('#');
		if (FirstCommentSymbol != AString::npos)
		{
			ParsingLine.erase(FirstCommentSymbol);
		}

		if (IsOnlyWhitespace(ParsingLine))
		{
			// Ignore empty and whitespace only lines
			continue;
		}
		AddRecipeFromLine(ParsingLine, LineNum);
	}  // while (getline(ParsingLine))

	LOG("Loaded %zu brewing recipes", m_Recipes.size());
}





void cBrewingRecipes::AddRecipeFromLine(AString a_Line, unsigned int a_LineNum)
{
	a_Line.erase(std::remove_if(a_Line.begin(), a_Line.end(), isspace), a_Line.end());

	auto Recipe = std::make_unique<cRecipe>();

	const AStringVector & InputAndIngredient = StringSplit(a_Line, "+");

	if (InputAndIngredient.size() != 2)
	{
		LOGWARNING("brewing.txt: line %d: A line with '+' was expected", a_LineNum);
		LOGINFO("Offending line: \"%s\"", a_Line.c_str());
		return;
	}

	const AStringVector & IngredientAndOutput = StringSplit(InputAndIngredient[1], "=");
	if (IngredientAndOutput.size() != 2)
	{
		LOGWARNING("brewing.txt: line %d: A line with '=' was expected", a_LineNum);
		LOGINFO("Offending line: \"%s\"", a_Line.c_str());
		return;
	}

	if (!ParseItem(IngredientAndOutput[0], Recipe->Ingredient))
	{
		LOGWARNING("brewing.txt: Parsing of the item didn't worked.");
		LOGINFO("Offending line: \"%s\"", a_Line.c_str());
		return;
	}

	if (!StringToInteger<short>(InputAndIngredient[0], Recipe->Input.m_ItemDamage))
	{
		LOGWARNING("brewing.txt: line %d: Cannot parse the damage value for the input item\"%s\".", a_LineNum, InputAndIngredient[0].c_str());
		LOGINFO("Offending line: \"%s\"", a_Line.c_str());
		return;
	}

	if (!StringToInteger<short>(IngredientAndOutput[1], Recipe->Output.m_ItemDamage))
	{
		LOGWARNING("brewing.txt: line %d: Cannot parse the damage value for the output item\"%s\".", a_LineNum, IngredientAndOutput[1].c_str());
		LOGINFO("Offending line: \"%s\"", a_Line.c_str());
		return;
	}

	m_Recipes.push_back(std::move(Recipe));
}





bool cBrewingRecipes::ParseItem(const AString & a_String, cItem & a_Item)
{
	return StringToItem(a_String, a_Item);
}





void cBrewingRecipes::ClearRecipes(void)
{
	m_Recipes.clear();
}





const cBrewingRecipes::cRecipe * cBrewingRecipes::GetRecipeFrom(const cItem & a_Input, const cItem & a_Ingredient) const
{
	for (const auto & Recipe : m_Recipes)
	{
		if ((Recipe->Input.IsEqual(a_Input)) && (Recipe->Ingredient.IsEqual(a_Ingredient)))
		{
			return Recipe.get();
		}
	}

	// Check for gunpowder
	if (a_Ingredient.m_ItemType == E_ITEM_GUNPOWDER)
	{
		if (a_Input.m_ItemDamage & 0x2000)
		{
			// Create new recipe and add it to list
			auto Recipe = std::make_unique<cRecipe>();

			Recipe->Input.m_ItemType = a_Input.m_ItemDamage;
			Recipe->Output.m_ItemDamage = a_Input.m_ItemDamage + 8192;
			Recipe->Ingredient.m_ItemType = E_ITEM_GUNPOWDER;

			auto RecipePtr = Recipe.get();
			m_Recipes.push_back(std::move(Recipe));
			return RecipePtr;
		}
		return nullptr;
	}

	// Check for splash potion
	if (a_Input.m_ItemDamage & 0x4000)
	{
		// Search for the drinkable potion, the ingredients are the same
		short SplashItemDamage = a_Input.m_ItemDamage - 8192;

		auto FoundRecipe = std::find_if(m_Recipes.cbegin(), m_Recipes.cend(), [&](const std::unique_ptr<cRecipe>& a_Recipe)
		{
			return (
				(a_Recipe->Input.m_ItemDamage == SplashItemDamage) &&
				(a_Recipe->Ingredient.IsEqual(a_Ingredient))
			);
		});

		if (FoundRecipe == m_Recipes.cend())
		{
			return nullptr;
		}

		// Create new recipe and add it to list
		auto Recipe = std::make_unique<cRecipe>();

		Recipe->Input.m_ItemDamage = a_Input.m_ItemDamage;
		Recipe->Output.m_ItemDamage = (*FoundRecipe)->Output.m_ItemDamage + 8192;
		Recipe->Ingredient.m_ItemType = (*FoundRecipe)->Ingredient.m_ItemType;

		auto RecipePtr = Recipe.get();
		m_Recipes.push_back(std::move(Recipe));
		return RecipePtr;
	}
	return nullptr;
}





bool cBrewingRecipes::IsIngredient(const cItem & a_Ingredient) const
{
	// Check for gunpowder
	if (a_Ingredient.m_ItemType == E_ITEM_GUNPOWDER)
	{
		return true;
	}

	for (const auto & Recipe : m_Recipes)
	{
		if (Recipe->Ingredient.IsEqual(a_Ingredient))
		{
			return true;
		}
	}
	return false;
}





bool cBrewingRecipes::IsBottle(const cItem & a_Item) const
{
	return (a_Item.m_ItemType == E_ITEM_POTION);
}





bool cBrewingRecipes::IsFuel(const cItem & a_Item) const
{
	return (a_Item.m_ItemType == E_ITEM_BLAZE_POWDER);
}




