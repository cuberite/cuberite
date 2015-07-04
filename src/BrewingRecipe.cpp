
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "BrewingRecipe.h"
#include "Item.h"

#include <fstream>

#define BREWING_RECIPE_FILE "brewing.txt"





typedef std::list<cBrewingRecipe::cRecipe> RecipeList;





struct cBrewingRecipe::sBrewingRecipeState
{
	RecipeList Recipes;
};





cBrewingRecipe::cBrewingRecipe()
	: m_pState(new sBrewingRecipeState)
{
	ReloadRecipes();
}





cBrewingRecipe::~cBrewingRecipe()
{
	ClearRecipes();
	delete m_pState;
	m_pState = nullptr;
}





void cBrewingRecipe::ReloadRecipes(void)
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
		if (ParsingLine.empty())
		{
			continue;
		}

		// Remove comments from the line:
		size_t FirstCommentSymbol = ParsingLine.find('#');
		if ((FirstCommentSymbol != AString::npos) && (FirstCommentSymbol != 0))
		{
			ParsingLine.erase(ParsingLine.begin() + static_cast<const long>(FirstCommentSymbol), ParsingLine.end());
		}

		switch (ParsingLine[0])
		{
			case '#':
			{
				// Comment
				break;
			}
			default:
			{
				AddRecipeFromLine(ParsingLine, LineNum);
				break;
			}
		}  // switch (ParsingLine[0])
	}  // while (getline(ParsingLine))

	LOG("Loaded " SIZE_T_FMT " brewing recipes", m_pState->Recipes.size());
}





void cBrewingRecipe::AddRecipeFromLine(const AString & a_Line, unsigned int a_LineNum)
{
	AString Line(a_Line);
	Line.erase(std::remove_if(Line.begin(), Line.end(), isspace), Line.end());

	short InputDamage;
	short OutputDamage;

	std::unique_ptr<cItem> InputItem(new cItem());
	std::unique_ptr<cItem> IngredientItem(new cItem());
	std::unique_ptr<cItem> OutputItem(new cItem());

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
	InputItem->m_ItemType = E_ITEM_POTIONS;
	InputItem->m_ItemDamage = InputDamage;

	OutputItem->m_ItemType = E_ITEM_POTIONS;
	OutputItem->m_ItemDamage = OutputDamage;

	cRecipe Recipe;
	Recipe.Input = InputItem.release();
	Recipe.Output = OutputItem.release();
	Recipe.Ingredient = IngredientItem.release();
	m_pState->Recipes.push_back(Recipe);
}





bool cBrewingRecipe::ParseItem(const AString & a_String, cItem & a_Item)
{
	AString ItemString = a_String;

	if (!StringToItem(ItemString, a_Item))
	{
		return false;
	}

	return true;
}





void cBrewingRecipe::ClearRecipes(void)
{
	for (RecipeList::iterator itr = m_pState->Recipes.begin(); itr != m_pState->Recipes.end(); ++itr)
	{
		cRecipe Recipe = *itr;
		delete Recipe.Input;
		Recipe.Input = nullptr;
		delete Recipe.Output;
		Recipe.Output = nullptr;
		delete Recipe.Ingredient;
		Recipe.Ingredient = nullptr;
	}
	m_pState->Recipes.clear();
}





const cBrewingRecipe::cRecipe * cBrewingRecipe::GetRecipeFrom(const cItem & a_Input, const cItem & a_Ingredient) const
{
	const cRecipe * BestRecipe = nullptr;
	
	for (RecipeList::const_iterator itr = m_pState->Recipes.begin(); itr != m_pState->Recipes.end(); ++itr)
	{
		const cRecipe & Recipe = *itr;
		if ((Recipe.Input->IsEqual(a_Input)) && (Recipe.Ingredient->IsEqual(a_Ingredient)))
		{
			BestRecipe = &Recipe;
			break;
		}
	}
	
	if (BestRecipe != nullptr)
	{
		return BestRecipe;
	}
	
	
	// Check for gunpowder
	if (a_Ingredient.m_ItemType == E_ITEM_GUNPOWDER)
	{
		if (a_Input.m_ItemDamage & 0x2000)
		{
			std::unique_ptr<cItem> InputItem(new cItem());
			std::unique_ptr<cItem> IngredientItem(new cItem());
			std::unique_ptr<cItem> OutputItem(new cItem());
			
			InputItem->m_ItemType = E_ITEM_POTIONS;
			InputItem->m_ItemDamage = a_Input.m_ItemDamage;
			OutputItem->m_ItemType = E_ITEM_POTIONS;
			OutputItem->m_ItemDamage = a_Input.m_ItemDamage + 8192;
			IngredientItem->m_ItemType = E_ITEM_GUNPOWDER;

			cRecipe Recipe;
			Recipe.Input = InputItem.release();
			Recipe.Output = OutputItem.release();
			Recipe.Ingredient = IngredientItem.release();
			m_pState->Recipes.push_back(Recipe);

			BestRecipe = &Recipe;
			return BestRecipe;
		}
		return nullptr;
	}
	
	// Check for splash potion
	if (a_Input.m_ItemDamage & 0x4000)
	{
		// Search for the drinkable potion, the ingredients are the same
		short SplashItemDamage = a_Input.m_ItemDamage - 8192;
		
		for (RecipeList::const_iterator itr = m_pState->Recipes.begin(); itr != m_pState->Recipes.end(); ++itr)
		{
			const cRecipe & Recipe = *itr;
			if ((Recipe.Input->m_ItemDamage == SplashItemDamage) && (Recipe.Ingredient->IsEqual(a_Ingredient)))
			{
				BestRecipe = &Recipe;
				break;
			}
		}

		if (BestRecipe == nullptr)
		{
			return nullptr;
		}

		// Create new item and add it to list
		std::unique_ptr<cItem> InputItem(new cItem());
		std::unique_ptr<cItem> IngredientItem(new cItem());
		std::unique_ptr<cItem> OutputItem(new cItem());

		InputItem->m_ItemType = E_ITEM_POTIONS;
		InputItem->m_ItemDamage = a_Input.m_ItemDamage;
		OutputItem->m_ItemType = E_ITEM_POTIONS;
		OutputItem->m_ItemDamage = BestRecipe->Output->m_ItemDamage + 8192;
		IngredientItem->m_ItemType = BestRecipe->Ingredient->m_ItemType;

		cRecipe Recipe;
		Recipe.Input = InputItem.release();
		Recipe.Output = OutputItem.release();
		Recipe.Ingredient = IngredientItem.release();
		m_pState->Recipes.push_back(Recipe);
		BestRecipe = &Recipe;
	}

	return BestRecipe;
}






bool cBrewingRecipe::IsIngredient(const cItem & a_Ingredient) const
{
	for (RecipeList::const_iterator itr = m_pState->Recipes.begin(); itr != m_pState->Recipes.end(); ++itr)
	{
		const cRecipe & Recipe = *itr;
		if ((Recipe.Ingredient->IsEqual(a_Ingredient)))
		{
			return true;
		}
	}
	
	// Check for gunpowder
	if (a_Ingredient.m_ItemType == E_ITEM_GUNPOWDER)
	{
		return true;
	}
	
	return false;
}





bool cBrewingRecipe::IsBottle(const cItem & a_Bottle) const
{
	if (a_Bottle.m_ItemType == E_ITEM_POTIONS)
	{
		return true;
	}
	return false;
}
