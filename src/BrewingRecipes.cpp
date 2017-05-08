
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "BrewingRecipes.h"
#include "Item.h"

#include <fstream>

#define BREWING_RECIPE_FILE "brewing.txt"





typedef std::vector<std::unique_ptr<cBrewingRecipes::cRecipe>> RecipeList;





struct cBrewingRecipes::sBrewingRecipeState
{
	RecipeList Recipes;
};





cBrewingRecipes::cBrewingRecipes()
	: m_pState(new sBrewingRecipeState)
{
	ReloadRecipes();
}





cBrewingRecipes::~cBrewingRecipes()
{
	ClearRecipes();
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
			ParsingLine.erase(ParsingLine.begin() += static_cast<long>(FirstCommentSymbol), ParsingLine.end());
		}

		if (ParsingLine.empty())
		{
			continue;
		}
		AddRecipeFromLine(ParsingLine, LineNum);
	}  // while (getline(ParsingLine))

	LOG("Loaded " SIZE_T_FMT " brewing recipes", m_pState->Recipes.size());
}





void cBrewingRecipes::AddRecipeFromLine(const AString & a_Line, unsigned int a_LineNum)
{
	AString Line(a_Line);
	Line.erase(std::remove_if(Line.begin(), Line.end(), isspace), Line.end());

	short InputDamage;
	short OutputDamage;

	std::unique_ptr<cItem> InputItem = cpp14::make_unique<cItem>();
	std::unique_ptr<cItem> IngredientItem = cpp14::make_unique<cItem>();
	std::unique_ptr<cItem> OutputItem = cpp14::make_unique<cItem>();

	const AStringVector & InputAndIngredient = StringSplit(Line, "+");

	if (InputAndIngredient.size() != 2)
	{
		LOGWARNING("brewing.txt: line %d: A line with '+' was expected", a_LineNum);
		LOGINFO("Offending line: \"%s\"", a_Line.c_str());
		return;
	}

	const AStringVector & IngredientAndOutput = StringSplit(InputAndIngredient[1].c_str(), "=");
	if (IngredientAndOutput.size() != 2)
	{
		LOGWARNING("brewing.txt: line %d: A line with '=' was expected", a_LineNum);
		LOGINFO("Offending line: \"%s\"", a_Line.c_str());
		return;
	}

	if (!ParseItem(IngredientAndOutput[0], *IngredientItem))
	{
		LOGWARNING("brewing.txt: Parsing of the item didn't worked.");
		LOGINFO("Offending line: \"%s\"", a_Line.c_str());
		return;
	}

	if (!StringToInteger<short>(InputAndIngredient[0], InputDamage))
	{
		LOGWARNING("brewing.txt: line %d: Cannot parse the damage value for the input item\"%s\".", a_LineNum, InputAndIngredient[0].c_str());
		LOGINFO("Offending line: \"%s\"", a_Line.c_str());
		return;
	}

	if (!StringToInteger<short>(IngredientAndOutput[1], OutputDamage))
	{
		LOGWARNING("brewing.txt: line %d: Cannot parse the damage value for the output item\"%s\".", a_LineNum, IngredientAndOutput[1].c_str());
		LOGINFO("Offending line: \"%s\"", a_Line.c_str());
		return;
	}

	// The items has always the same type
	InputItem->m_ItemType = E_ITEM_POTION;
	InputItem->m_ItemDamage = InputDamage;

	OutputItem->m_ItemType = E_ITEM_POTION;
	OutputItem->m_ItemDamage = OutputDamage;

	std::unique_ptr<cRecipe> Recipe = cpp14::make_unique<cRecipe>();
	Recipe->Input = std::move(InputItem);
	Recipe->Output = std::move(OutputItem);
	Recipe->Ingredient = std::move(IngredientItem);
	m_pState->Recipes.push_back(std::move(Recipe));
}





bool cBrewingRecipes::ParseItem(const AString & a_String, cItem & a_Item)
{
	return StringToItem(a_String, a_Item);
}





void cBrewingRecipes::ClearRecipes(void)
{
	m_pState->Recipes.clear();
}





const cBrewingRecipes::cRecipe * cBrewingRecipes::GetRecipeFrom(const cItem & a_Input, const cItem & a_Ingredient) const
{
	for (auto & Recipe : m_pState->Recipes)
	{
		if ((Recipe->Input->IsEqual(a_Input)) && (Recipe->Ingredient->IsEqual(a_Ingredient)))
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
			std::unique_ptr<cItem> InputItem = cpp14::make_unique<cItem>();
			std::unique_ptr<cItem> IngredientItem = cpp14::make_unique<cItem>();
			std::unique_ptr<cItem> OutputItem = cpp14::make_unique<cItem>();

			InputItem->m_ItemType = E_ITEM_POTION;
			InputItem->m_ItemDamage = a_Input.m_ItemDamage;
			OutputItem->m_ItemType = E_ITEM_POTION;
			OutputItem->m_ItemDamage = a_Input.m_ItemDamage + 8192;
			IngredientItem->m_ItemType = E_ITEM_GUNPOWDER;

			std::unique_ptr<cRecipe> Recipe = cpp14::make_unique<cRecipe>();
			Recipe->Input = std::move(InputItem);
			Recipe->Output = std::move(OutputItem);
			Recipe->Ingredient = std::move(IngredientItem);
			m_pState->Recipes.push_back(std::move(Recipe));
			return Recipe.get();
		}
		return nullptr;
	}

	// Check for splash potion
	if (a_Input.m_ItemDamage & 0x4000)
	{
		const std::unique_ptr<cRecipe> * FoundRecipe = nullptr;
		// Search for the drinkable potion, the ingredients are the same
		short SplashItemDamage = a_Input.m_ItemDamage - 8192;

		for (auto & Recipe : m_pState->Recipes)
		{
			if ((Recipe->Input->m_ItemDamage == SplashItemDamage) && (Recipe->Ingredient->IsEqual(a_Ingredient)))
			{
				FoundRecipe = &Recipe;
				break;
			}
		}

		if (FoundRecipe == nullptr)
		{
			return nullptr;
		}

		// Create new recipe and add it to list
		std::unique_ptr<cItem> InputItem = cpp14::make_unique<cItem>();
		std::unique_ptr<cItem> IngredientItem = cpp14::make_unique<cItem>();
		std::unique_ptr<cItem> OutputItem = cpp14::make_unique<cItem>();

		InputItem->m_ItemType = E_ITEM_POTION;
		InputItem->m_ItemDamage = a_Input.m_ItemDamage;
		OutputItem->m_ItemType = E_ITEM_POTION;
		OutputItem->m_ItemDamage = (*FoundRecipe)->Output->m_ItemDamage + 8192;
		IngredientItem->m_ItemType = (*FoundRecipe)->Ingredient->m_ItemType;

		std::unique_ptr<cRecipe> Recipe = cpp14::make_unique<cRecipe>();
		Recipe->Input = std::move(InputItem);
		Recipe->Output = std::move(OutputItem);
		Recipe->Ingredient = std::move(IngredientItem);
		m_pState->Recipes.push_back(std::move(Recipe));
		return Recipe.get();
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

	for (auto & Recipe : m_pState->Recipes)
	{
		if (Recipe->Ingredient->IsEqual(a_Ingredient))
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




