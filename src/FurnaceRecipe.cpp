
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "FurnaceRecipe.h"
#include "Item.h"

#include <fstream>

#define FURNACE_RECIPE_FILE "furnace.txt"





typedef std::list<cFurnaceRecipe::cRecipe> RecipeList;
typedef std::list<cFurnaceRecipe::cFuel> FuelList;





struct cFurnaceRecipe::sFurnaceRecipeState
{
	RecipeList Recipes;
	FuelList Fuel;
};





cFurnaceRecipe::cFurnaceRecipe()
	: m_pState(new sFurnaceRecipeState)
{
	ReloadRecipes();
}





cFurnaceRecipe::~cFurnaceRecipe()
{
	ClearRecipes();
	delete m_pState;
	m_pState = nullptr;
}





void cFurnaceRecipe::ReloadRecipes(void)
{
	ClearRecipes();
	LOGD("Loading furnace recipes...");

	std::ifstream f(FURNACE_RECIPE_FILE, std::ios::in);
	if (!f.good())
	{
		LOG("Could not open the furnace recipes file \"%s\". No furnace recipes are available.", FURNACE_RECIPE_FILE);
		return;
	}

	unsigned int LineNum = 0;
	AString ParsingLine;

	while (std::getline(f, ParsingLine))
	{
		LineNum++;

		// Remove comments from the line:
		size_t FirstCommentSymbol = ParsingLine.find('#');
		if ((FirstCommentSymbol != AString::npos) && (FirstCommentSymbol != 0))
		{
			ParsingLine.erase(ParsingLine.begin() + static_cast<const long>(FirstCommentSymbol), ParsingLine.end());
		}

		if (IsOnlyWhitespace(ParsingLine))
		{
			// Ignore empty and whitespace only lines
			continue;
		}

		switch (ParsingLine[0])
		{
			case '#':
			{
				// Comment
				break;
			}

			case '!':
			{
				AddFuelFromLine(ParsingLine, LineNum);
				break;
			}

			default:
			{
				AddRecipeFromLine(ParsingLine, LineNum);
				break;
			}
		}  // switch (ParsingLine[0])
	}  // while (getline(ParsingLine))

	LOG("Loaded %zu furnace recipes and %zu fuels", m_pState->Recipes.size(), m_pState->Fuel.size());
}





void cFurnaceRecipe::AddFuelFromLine(const AString & a_Line, unsigned int a_LineNum)
{
	AString Line(a_Line);
	Line.erase(Line.begin());  // Remove the beginning "!"
	Line.erase(std::remove_if(Line.begin(), Line.end(), isspace), Line.end());

	std::unique_ptr<cItem> Item = std::make_unique<cItem>();
	int BurnTime;

	const AStringVector & Sides = StringSplit(Line, "=");
	if (Sides.size() != 2)
	{
		LOGWARNING("furnace.txt: line %d: A single '=' was expected, got %zu", a_LineNum, Sides.size() - 1);
		LOGINFO("Offending line: \"%s\"", a_Line.c_str());
		return;
	}

	if (!ParseItem(Sides[0], *Item))
	{
		LOGWARNING("furnace.txt: line %d: Cannot parse item \"%s\".", a_LineNum, Sides[0].c_str());
		LOGINFO("Offending line: \"%s\"", a_Line.c_str());
		return;
	}

	if (!StringToInteger<int>(Sides[1], BurnTime))
	{
		LOGWARNING("furnace.txt: line %d: Cannot parse burn time.", a_LineNum);
		LOGINFO("Offending line: \"%s\"", a_Line.c_str());
		return;
	}

	// Add to fuel list:
	cFuel Fuel;
	Fuel.In = Item.release();
	Fuel.BurnTime = BurnTime;
	m_pState->Fuel.push_back(Fuel);
}





void cFurnaceRecipe::AddRecipeFromLine(const AString & a_Line, unsigned int a_LineNum)
{
	AString Line(a_Line);
	Line.erase(std::remove_if(Line.begin(), Line.end(), isspace), Line.end());

	int CookTime = 200;
	float Reward = 0;
	std::unique_ptr<cItem> InputItem = std::make_unique<cItem>();
	std::unique_ptr<cItem> OutputItem = std::make_unique<cItem>();

	const AStringVector & Sides = StringSplit(Line, "=");
	if (Sides.size() != 2)
	{
		LOGWARNING("furnace.txt: line %d: A single '=' was expected, got %zu", a_LineNum, Sides.size() - 1);
		LOGINFO("Offending line: \"%s\"", a_Line.c_str());
		return;
	}

	const AStringVector & InputSplit = StringSplit(Sides[0], "@");
	if (!ParseItem(InputSplit[0], *InputItem))
	{
		LOGWARNING("furnace.txt: line %d: Cannot parse input item \"%s\".", a_LineNum, InputSplit[0].c_str());
		LOGINFO("Offending line: \"%s\"", a_Line.c_str());
		return;
	}

	if (InputSplit.size() > 1)
	{
		if (!StringToInteger<int>(InputSplit[1], CookTime))
		{
			LOGWARNING("furnace.txt: line %d: Cannot parse cook time \"%s\".", a_LineNum, InputSplit[1].c_str());
			LOGINFO("Offending line: \"%s\"", a_Line.c_str());
			return;
		}
	}
	const AStringVector & OutputSplit = StringSplit(Sides[1], "$");
	if (!ParseItem(OutputSplit[0], *OutputItem))
	{
		LOGWARNING("furnace.txt: line %d: Cannot parse output item \"%s\".", a_LineNum, OutputSplit[0].c_str());
		LOGINFO("Offending line: \"%s\"", a_Line.c_str());
		return;
	}
	if (OutputSplit.size() > 1)
	{
		if (!StringToFloat(OutputSplit[1], Reward))
		{
			LOGWARNING("furnace.txt: line %d: Cannot parse reward \"%s\".", a_LineNum, OutputSplit[1].c_str());
			LOGINFO("Offending line: \"%s\"", a_Line.c_str());
			return;
		}
	}
	cRecipe Recipe;
	Recipe.In = InputItem.release();
	Recipe.Out = OutputItem.release();
	Recipe.CookTime = CookTime;
	Recipe.Reward = Reward;
	m_pState->Recipes.push_back(Recipe);
}





bool cFurnaceRecipe::ParseItem(const AString & a_String, cItem & a_Item)
{
	AString ItemString = a_String;

	const AStringVector & SplitAmount = StringSplit(ItemString, ",");
	ItemString = SplitAmount[0];

	const AStringVector & SplitMeta = StringSplit(ItemString, ":");
	ItemString = SplitMeta[0];

	if (!StringToItem(ItemString, a_Item))
	{
		return false;
	}

	if (SplitAmount.size() > 1)
	{
		if (!StringToInteger<char>(SplitAmount[1], a_Item.m_ItemCount))
		{
			return false;
		}
	}

	if (SplitMeta.size() > 1)
	{
		if (!StringToInteger<short>(SplitMeta[1], a_Item.m_ItemDamage))
		{
			return false;
		}
	}
	return true;
}





void cFurnaceRecipe::ClearRecipes(void)
{
	for (RecipeList::iterator itr = m_pState->Recipes.begin(); itr != m_pState->Recipes.end(); ++itr)
	{
		cRecipe Recipe = *itr;
		delete Recipe.In;
		Recipe.In = nullptr;
		delete Recipe.Out;
		Recipe.Out = nullptr;
	}
	m_pState->Recipes.clear();

	for (FuelList::iterator itr = m_pState->Fuel.begin(); itr != m_pState->Fuel.end(); ++itr)
	{
		cFuel Fuel = *itr;
		delete Fuel.In;
		Fuel.In = nullptr;
	}
	m_pState->Fuel.clear();
}





const cFurnaceRecipe::cRecipe * cFurnaceRecipe::GetRecipeFrom(const cItem & a_Ingredient) const
{
	const cRecipe * BestRecipe = nullptr;
	for (RecipeList::const_iterator itr = m_pState->Recipes.begin(); itr != m_pState->Recipes.end(); ++itr)
	{
		const cRecipe & Recipe = *itr;
		if ((Recipe.In->m_ItemType == a_Ingredient.m_ItemType) && (Recipe.In->m_ItemCount <= a_Ingredient.m_ItemCount))
		{
			if (BestRecipe && (BestRecipe->In->m_ItemCount > Recipe.In->m_ItemCount))
			{
				continue;
			}
			else if ((Recipe.In->m_ItemDamage == -1) || (Recipe.In->m_ItemDamage == a_Ingredient.m_ItemDamage))
			{
				BestRecipe = &Recipe;
			}
		}
	}
	return BestRecipe;
}





bool cFurnaceRecipe::IsFuel(const cItem & a_Item) const
{
	for (auto & Fuel : m_pState->Fuel)
	{
		if ((Fuel.In->m_ItemType == a_Item.m_ItemType) && (Fuel.In->m_ItemCount <= a_Item.m_ItemCount))
		{
			return true;
		}
	}
	return false;
}





int cFurnaceRecipe::GetBurnTime(const cItem & a_Fuel) const
{
	int BestFuel = 0;
	for (FuelList::const_iterator itr = m_pState->Fuel.begin(); itr != m_pState->Fuel.end(); ++itr)
	{
		const cFuel & Fuel = *itr;
		if ((Fuel.In->m_ItemType == a_Fuel.m_ItemType) && (Fuel.In->m_ItemCount <= a_Fuel.m_ItemCount))
		{
			if (BestFuel > 0 && (BestFuel > Fuel.BurnTime))
			{
				continue;
			}
			else
			{
				BestFuel = Fuel.BurnTime;
			}
		}
	}
	return BestFuel;
}



