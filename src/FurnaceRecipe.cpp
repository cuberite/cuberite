
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "FurnaceRecipe.h"
#include "Item.h"

#include <fstream>

#define FURNACE_RECIPE_FILE "furnace.txt"





typedef std::list< cFurnaceRecipe::Recipe > RecipeList;
typedef std::list< cFurnaceRecipe::Fuel > FuelList;





struct cFurnaceRecipe::sFurnaceRecipeState
{
	RecipeList Recipes;
	FuelList Fuel;
};





cFurnaceRecipe::cFurnaceRecipe()
	: m_pState( new sFurnaceRecipeState)
{
	ReloadRecipes();
}





cFurnaceRecipe::~cFurnaceRecipe()
{
	ClearRecipes();
	delete m_pState;
	m_pState = NULL;
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
		ParsingLine.erase(std::remove_if(ParsingLine.begin(), ParsingLine.end(), isspace), ParsingLine.end());  // Remove ALL whitespace from the line
		if (ParsingLine.empty())
		{
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

	LOG("Loaded " SIZE_T_FMT " furnace recipes and " SIZE_T_FMT " fuels", m_pState->Recipes.size(), m_pState->Fuel.size());
}





void cFurnaceRecipe::AddFuelFromLine(const AString & a_Line, int a_LineNum)
{
	// Fuel
	int IItemID = 0, IItemCount = 0, IItemHealth = 0, IBurnTime = 0;
	AString::size_type BeginPos = 1;  // Begin at one after exclamation mark (bang)

	if (
		!ReadMandatoryNumber(BeginPos, ":", a_Line, a_LineNum, IItemID) ||                       // Read item ID
		!ReadOptionalNumbers(BeginPos, ":", "=", a_Line, a_LineNum, IItemCount, IItemHealth) ||  // Read item count (and optionally health)
		!ReadMandatoryNumber(BeginPos, "0123456789", a_Line, a_LineNum, IBurnTime, true)         // Read item burn time - last value
	)
	{
		return;
	}

	// Add to fuel list:
	Fuel F;
	F.In = new cItem((ENUM_ITEM_ID)IItemID, (char)IItemCount, (short)IItemHealth);
	F.BurnTime = IBurnTime;
	m_pState->Fuel.push_back(F);
}





void cFurnaceRecipe::AddRecipeFromLine(const AString & a_Line, int a_LineNum)
{
	int IItemID = 0, IItemCount = 0, IItemHealth = 0, IBurnTime = 0;
	int OItemID = 0, OItemCount = 0, OItemHealth = 0;
	AString::size_type BeginPos = 0;  // Begin at start of line

	if (
		!ReadMandatoryNumber(BeginPos, ":", a_Line, a_LineNum, IItemID) ||                                  // Read item ID
		!ReadOptionalNumbers(BeginPos, ":", "@", a_Line, a_LineNum, IItemCount, IItemHealth) ||             // Read item count (and optionally health)
		!ReadMandatoryNumber(BeginPos, "=", a_Line, a_LineNum, IBurnTime) ||                                // Read item burn time
		!ReadMandatoryNumber(BeginPos, ":", a_Line, a_LineNum, OItemID) ||                                  // Read result ID
		!ReadOptionalNumbers(BeginPos, ":", "012456789", a_Line, a_LineNum, OItemCount, OItemHealth, true)  // Read result count (and optionally health) - last value
	)
	{
		return;
	}

	// Add to recipe list
	Recipe R;
	R.In = new cItem((ENUM_ITEM_ID)IItemID, (char)IItemCount, (short)IItemHealth);
	R.Out = new cItem((ENUM_ITEM_ID)OItemID, (char)OItemCount, (short)OItemHealth);
	R.CookTime = IBurnTime;
	m_pState->Recipes.push_back(R);
}





void cFurnaceRecipe::PrintParseError(unsigned int a_Line, size_t a_Position, const AString & a_CharactersMissing)
{
	LOGWARN("Error parsing furnace recipes at line %i pos " SIZE_T_FMT ": missing '%s'", a_Line, a_Position, a_CharactersMissing.c_str());
}





bool cFurnaceRecipe::ReadMandatoryNumber(AString::size_type & a_Begin, const AString & a_Delimiter, const AString & a_Text, unsigned int a_Line, int & a_Value, bool a_IsLastValue)
{
	// TODO: replace atoi with std::stoi
	AString::size_type End;
	if (a_IsLastValue)
	{
		End = a_Text.find_first_not_of(a_Delimiter, a_Begin);
	}
	else
	{
		End = a_Text.find_first_of(a_Delimiter, a_Begin);
		if (End == AString::npos)
		{
			PrintParseError(a_Line, a_Begin, a_Delimiter);
			return false;
		}
	}
	
	// stoi won't throw an exception if the string is alphanumeric, we should check for this
	if (!DoesStringContainOnlyNumbers(a_Text.substr(a_Begin, End - a_Begin)))
	{
		PrintParseError(a_Line, a_Begin, "number");
		return false;
	}
	a_Value = atoi(a_Text.substr(a_Begin, End - a_Begin).c_str());

	a_Begin = End + 1;  // Jump over delimiter
	return true;
}





bool cFurnaceRecipe::ReadOptionalNumbers(AString::size_type & a_Begin, const AString & a_DelimiterOne, const AString & a_DelimiterTwo, const AString & a_Text, unsigned int a_Line, int & a_ValueOne, int & a_ValueTwo, bool a_IsLastValue)
{
	// TODO: replace atoi with std::stoi
	AString::size_type End, Begin = a_Begin;

	End = a_Text.find_first_of(a_DelimiterOne, Begin);
	if (End != AString::npos)
	{
		if (DoesStringContainOnlyNumbers(a_Text.substr(Begin, End - Begin)))
		{
			a_ValueOne = std::atoi(a_Text.substr(Begin, End - Begin).c_str());
			Begin = End + 1;

			if (a_IsLastValue)
			{
				End = a_Text.find_first_not_of(a_DelimiterTwo, Begin);
			}
			else
			{
				End = a_Text.find_first_of(a_DelimiterTwo, Begin);
				if (End == AString::npos)
				{
					PrintParseError(a_Line, Begin, a_DelimiterTwo);
					return false;
				}
			}

			// stoi won't throw an exception if the string is alphanumeric, we should check for this
			if (!DoesStringContainOnlyNumbers(a_Text.substr(Begin, End - Begin)))
			{
				PrintParseError(a_Line, Begin, "number");
				return false;
			}
			a_ValueTwo = atoi(a_Text.substr(Begin, End - Begin).c_str());

			a_Begin = End + 1;  // Jump over delimiter
			return true;
		}
		else
		{
			return ReadMandatoryNumber(a_Begin, a_DelimiterTwo, a_Text, a_Line, a_ValueOne, a_IsLastValue);
		}
	}
	
	return ReadMandatoryNumber(a_Begin, a_DelimiterTwo, a_Text, a_Line, a_ValueOne, a_IsLastValue);
}





bool cFurnaceRecipe::DoesStringContainOnlyNumbers(const AString & a_String)
{
	// TODO: replace this with std::all_of(a_String.begin(), a_String.end(), isdigit)
	return (a_String.find_first_not_of("0123456789") == AString::npos);
}





void cFurnaceRecipe::ClearRecipes(void)
{
	for (RecipeList::iterator itr = m_pState->Recipes.begin(); itr != m_pState->Recipes.end(); ++itr)
	{
		Recipe R = *itr;
		delete R.In;
		R.In = NULL;
		delete R.Out;
		R.Out = NULL;
	}
	m_pState->Recipes.clear();

	for (FuelList::iterator itr = m_pState->Fuel.begin(); itr != m_pState->Fuel.end(); ++itr)
	{
		Fuel F = *itr;
		delete F.In;
		F.In = NULL;
	}
	m_pState->Fuel.clear();
}





const cFurnaceRecipe::Recipe * cFurnaceRecipe::GetRecipeFrom(const cItem & a_Ingredient) const
{
	const Recipe * BestRecipe = 0;
	for (RecipeList::const_iterator itr = m_pState->Recipes.begin(); itr != m_pState->Recipes.end(); ++itr)
	{
		const Recipe & R = *itr;
		if ((R.In->m_ItemType == a_Ingredient.m_ItemType) && (R.In->m_ItemCount <= a_Ingredient.m_ItemCount))
		{
			if (BestRecipe && (BestRecipe->In->m_ItemCount > R.In->m_ItemCount))
			{
				continue;
			}
			else
			{
				BestRecipe = &R;
			}
		}
	}
	return BestRecipe;
}





int cFurnaceRecipe::GetBurnTime(const cItem & a_Fuel) const
{
	int BestFuel = 0;
	for (FuelList::const_iterator itr = m_pState->Fuel.begin(); itr != m_pState->Fuel.end(); ++itr)
	{
		const Fuel & F = *itr;
		if ((F.In->m_ItemType == a_Fuel.m_ItemType) && (F.In->m_ItemCount <= a_Fuel.m_ItemCount))
		{
			if (BestFuel > 0 && (BestFuel > F.BurnTime))
			{
				continue;
			}
			else
			{
				BestFuel = F.BurnTime;
			}
		}
	}
	return BestFuel;
}




