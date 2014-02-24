
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "FurnaceRecipe.h"
#include "Item.h"

#include <fstream>
#include <sstream>





typedef std::list< cFurnaceRecipe::Recipe > RecipeList;
typedef std::list< cFurnaceRecipe::Fuel > FuelList;





struct cFurnaceRecipe::sFurnaceRecipeState
{
	RecipeList Recipes;
	FuelList Fuel;
};





cFurnaceRecipe::cFurnaceRecipe()
	: m_pState( new sFurnaceRecipeState )
{
	ReloadRecipes();
}





cFurnaceRecipe::~cFurnaceRecipe()
{
	ClearRecipes();
	delete m_pState;
}





void cFurnaceRecipe::ReloadRecipes(void)
{
	ClearRecipes();
	LOGD("Loading furnace recipes...");

	std::ifstream f;
	char a_File[] = "furnace.txt";
	f.open(a_File, std::ios::in);
	std::string input;

	if (!f.good())
	{
		f.close();
		LOG("Could not open the furnace recipes file \"%s\"", a_File);
		return;
	}

	// TODO: Replace this messy parse with a high-level-structured one (ReadLine / ProcessLine)
	bool bSyntaxError = false;
	while (f.good())
	{
		char c;

		//////////////////////////////////////////////////////////////////////////
		// comments
		f >> c;
		f.unget();
		if( c == '#' )
		{
			while( f.good() && c != '\n' )
			{
				f.get( c );
			}
			continue;
		}


		//////////////////////////////////////////////////////////////////////////
		// Line breaks
		f.get( c );
		while( f.good() && ( c == '\n' || c == '\r' ) ) { f.get( c ); }
		if (f.eof())
		{
			break;
		}
		f.unget();

		//////////////////////////////////////////////////////////////////////////
		// Check for fuel
		f >> c;
		if( c == '!' ) // It's fuel :)
		{
			// Read item
			int IItemID = 0, IItemCount = 0, IItemHealth = 0;
			f >> IItemID;
			f >> c; if( c != ':' ) { bSyntaxError = true; break; }
			f >> IItemCount;

			// Optional health
			f >> c; 
			if( c != ':' ) 
				f.unget();
			else
			{
				f >> IItemHealth;
			}

			// Burn time
			int BurnTime;
			f >> c; if( c != '=' ) { bSyntaxError = true; break; }
			f >> BurnTime;

			// Add to fuel list
			Fuel F;
			F.In = new cItem( (ENUM_ITEM_ID) IItemID, (char)IItemCount, (short)IItemHealth );
			F.BurnTime = BurnTime;
			m_pState->Fuel.push_back( F );
			continue;
		}
		f.unget();

		//////////////////////////////////////////////////////////////////////////
		// Read items
		int IItemID = 0, IItemCount = 0, IItemHealth = 0;
		f >> IItemID;
		if (IItemID == 0) continue;
		f >> c; if( c != ':' ) { bSyntaxError = true; break; }
		f >> IItemCount;

		// Optional health
		f >> c; 
		if( c != ':' )
			f.unget();
		else
		{
			f >> IItemHealth;
		}

		int CookTime;
		f >> c; if( c != '@' ) { bSyntaxError = true; break; }
		f >> CookTime;

		int OItemID = 0, OItemCount = 0, OItemHealth = 0;
		f >> c; if( c != '=' ) { bSyntaxError = true; break; }
		f >> OItemID;
		f >> c; if( c != ':' ) { bSyntaxError = true; break; }
		f >> OItemCount;

		// Optional health
		f >> c; 
		if( c != ':' )
			f.unget();
		else
		{
			f >> OItemHealth;
		}
		
		// Optional Experience
		float OExperience = 0.0F;
		f >> c;
		if( c != '%' )
			f.unget();
		else
		{
			f >> OExperience;
		}

		// Add to recipe list
		Recipe R;
		R.In = new cItem( (ENUM_ITEM_ID)IItemID, (char)IItemCount, (short)IItemHealth );
		R.Out = new cItem( (ENUM_ITEM_ID)OItemID, (char)OItemCount, (short)OItemHealth );
		R.OutExp = OExperience;
		R.CookTime = CookTime;
		m_pState->Recipes.push_back( R );
	}
	if (bSyntaxError)
	{
		LOGERROR("ERROR: FurnaceRecipe, syntax error" );
	}
	LOG("Loaded %u furnace recipes and %u fuels", m_pState->Recipes.size(), m_pState->Fuel.size());
}





void cFurnaceRecipe::ClearRecipes(void)
{
	for (RecipeList::iterator itr = m_pState->Recipes.begin(); itr != m_pState->Recipes.end(); ++itr)
	{
		Recipe R = *itr;
		delete R.In;
		delete R.Out;
	}
	m_pState->Recipes.clear();

	for (FuelList::iterator itr = m_pState->Fuel.begin(); itr != m_pState->Fuel.end(); ++itr)
	{
		Fuel F = *itr;
		delete F.In;
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




