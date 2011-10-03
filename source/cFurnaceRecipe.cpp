#include "cFurnaceRecipe.h"
#include "cItem.h"
#include "cMCLogger.h"

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

void cFurnaceRecipe::ReloadRecipes()
{
	ClearRecipes();
	LOG("--Loading furnace recipes--");

	std::ifstream f;
	char a_File[] = "furnace.txt";
	f.open(a_File, std::ios::in);
	std::string input;

	if( !f.good() )
	{
		f.close();
		LOG("Could not open file for recipes: %s", a_File);
		return;
	}

	bool bSyntaxError = false;
	while( f.good() )
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
		if( f.eof() ) break;
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
			float BurnTime;
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

		float CookTime;
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

		// Add to recipe list
		Recipe R;
		R.In = new cItem( (ENUM_ITEM_ID)IItemID, (char)IItemCount, (short)IItemHealth );
		R.Out = new cItem( (ENUM_ITEM_ID)OItemID, (char)OItemCount, (short)OItemHealth );
		R.CookTime = CookTime;
		m_pState->Recipes.push_back( R );
	}
	if( bSyntaxError )
	{
		LOGERROR("ERROR: FurnaceRecipe, syntax error" );
	}
	LOG("Got %i furnace recipes, and %i fuels.", m_pState->Recipes.size(), m_pState->Fuel.size() );

	LOG("--Done loading furnace recipes--");
}

void cFurnaceRecipe::ClearRecipes()
{
	for( RecipeList::iterator itr = m_pState->Recipes.begin(); itr != m_pState->Recipes.end(); ++itr )
	{
		Recipe R = *itr;
		delete R.In;
		delete R.Out;
	}
	m_pState->Recipes.clear();

	for( FuelList::iterator itr = m_pState->Fuel.begin(); itr != m_pState->Fuel.end(); ++itr )
	{
		Fuel F = *itr;
		delete F.In;
	}
	m_pState->Fuel.clear();
}

const cFurnaceRecipe::Recipe* cFurnaceRecipe::GetRecipeFrom( const cItem & a_Ingredient ) const
{
	const Recipe* BestRecipe = 0;
	for( RecipeList::const_iterator itr = m_pState->Recipes.begin(); itr != m_pState->Recipes.end(); ++itr )
	{
		const Recipe & R = *itr;
		if( (R.In->m_ItemID == a_Ingredient.m_ItemID) && (R.In->m_ItemCount <= a_Ingredient.m_ItemCount ) )
		{
			if( BestRecipe && (BestRecipe->In->m_ItemCount > R.In->m_ItemCount) )
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

float cFurnaceRecipe::GetBurnTime( const cItem & a_Fuel ) const
{
	float BestFuel = 0.f;
	for( FuelList::const_iterator itr = m_pState->Fuel.begin(); itr != m_pState->Fuel.end(); ++itr )
	{
		const Fuel & F = *itr;
		if( (F.In->m_ItemID == a_Fuel.m_ItemID) && (F.In->m_ItemCount <= a_Fuel.m_ItemCount ) )
		{
			if( BestFuel > 0.f && (BestFuel > F.BurnTime ) )
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