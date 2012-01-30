
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "cRecipeChecker.h"

#include <fstream>
#include <sstream>

#ifndef _WIN32
#include <cstring>
#include <cstdlib>
#endif

#include "Defines.h"
#include "cRoot.h"





typedef std::list< cRecipeChecker::Recipe* > RecipeList;

struct cRecipeChecker::sRecipeCheckerState
{
	RecipeList Recipes;
};





cRecipeChecker* cRecipeChecker::GetRecipeChecker()
{
	LOGWARN("WARNING: Using deprecated function cRecipeChecker::GetRecipeChecker() use cRoot::Get()->GetRecipeChecker() instead!");
	return cRoot::Get()->GetRecipeChecker();
}





cRecipeChecker::Recipe::~Recipe()
{
	delete [] Slots;
	Slots = 0;
}





cRecipeChecker::~cRecipeChecker()
{
	ClearRecipes();
	delete m_pState;
}





cRecipeChecker::cRecipeChecker()
	: m_pState( new sRecipeCheckerState )
{
	ReloadRecipes();
}





void cRecipeChecker::ClearRecipes()
{
	while( m_pState->Recipes.size() > 0 )
	{
		delete *m_pState->Recipes.begin();
		m_pState->Recipes.remove( *m_pState->Recipes.begin() );
	}
}





void PrintRecipe( std::vector< cRecipeChecker::RecipeSlot > & RecipeSlots )
{
	LOG("Recipe:");
	for(unsigned int i = 0; i < RecipeSlots.size(); i++)
	{
		cRecipeChecker::RecipeSlot Slot = RecipeSlots[i];
		LOG("x:%i y:%i id:%i #%i", Slot.x, Slot.y, Slot.Item.m_ItemID, Slot.Item.m_ItemCount );
	}
}





void PrintNear( std::ifstream & f, int a_History = 64 )
{
	f.clear();

	// Calculate how far we can move pointer back
	std::streamoff Position = f.tellg();
	f.seekg( 0, std::ios_base::beg );
	std::streamoff Difference = Position - f.tellg();
	if( Difference > a_History ) Difference = a_History;
	f.seekg( Position - Difference, std::ios_base::beg );

	std::string Near;
	if( f.good() )
	{
		while( f.good() && Near.size() < (unsigned int)Difference )
		{
			char c;
			f.get(c);
			Near.push_back( c );
		}
	}
	LOGERROR("Error near: \"%s\"", Near.c_str() );
}





void cRecipeChecker::ReloadRecipes()
{
	LOG("--Loading recipes--");
	ClearRecipes();

	std::ifstream f;

	char a_File[] = "recipes.txt";
	f.open(a_File, std::ios::in);
	std::string input;

	if( !f.good() )
	{
		f.close();
		LOG("Could not open file for recipes: %s", a_File);
		return;
	}

	std::vector< RecipeSlot > RecipeSlots;

	bool bError = false;
	while( f.good() )
	{
		bool bLoadSlot = false;
		bool bLoadResult = false;

		char c;
		f >> c;
		f.unget();
		if( c == '#' )
		{
			//LOG("Ignoring comment");
			while( f.good() && c != '\n' )
			{
				f.get( c );
			}
			continue;
		}

		f.get( c );
		while( f.good() && ( c == '\n' || c == '\r' ) ) { f.get( c ); }
		if( f.eof() ) break;
		f.unget();

		int width, height;
		f >> width;
		f >> c;	if( c != 'x' ) { bError=true; break; }
		f >> height;
		f >> c;
		if( c == ',' ) bLoadSlot = true;

		while( f.good() && bLoadSlot )
		{
			bool bDontAddRecipe = false;
			int x, y, ItemID, Amount;
			if( f.peek() == '*' )
			{
				f >> c;
				x = -1;
			}
			else
			{
				f >> x;
			}
			f >> c;	if( c != ':' ) { bError=true; break; }
			if( f.peek() == '*' )
			{
				f >> c;
				y = -1;
			}
			else
			{
				f >> y;
			}
			f >> c;	if( c != ':' ) { bError=true; break; }
			f >> ItemID;
			f >> c;	if( c != ':' ) { bError=true; break; }
			f >> Amount;

			f >> c;
			if( c == '@' ) bLoadResult = true;
			if( c != ',' ) bLoadSlot = false;

			if( !IsValidItem( ItemID ) )
			{
				LOGERROR("Error in recipes file (%s): Invalid Item ID %i", a_File, ItemID );
				bDontAddRecipe = true;
			}
			if( x < 0 && y < 0 )
			{
				if( Amount < 0 )
				{
					LOGERROR("Error in recipes file (%s): Invalid use of negative amount on wildcard coordinates", a_File );
					bDontAddRecipe = true;
				}
				for(int x = 0; x < width; ++x) for(int y = 0; y < height; ++y )
				{
					cItem Item( (ENUM_ITEM_ID)ItemID, (char)Amount );
					RecipeSlot Slot;
					Slot.Item = Item;
					Slot.x = x;
					Slot.y = y;
					RecipeSlots.push_back( Slot );
				}
			}
			else if( x < 0 )
			{
				if( Amount < 0 )
				{
					for(int x = 0; x < width; ++x) for(int yy = 0; yy < height; ++yy )
					{
						if( yy == y-1 ) continue;
						cItem Item( (ENUM_ITEM_ID)ItemID, (char)abs(Amount) );
						RecipeSlot Slot;
						Slot.Item = Item;
						Slot.x = x;
						Slot.y = yy;
						RecipeSlots.push_back( Slot );
					}
				}
				else
				{
					for(int x = 0; x < width; ++x)
					{
						cItem Item( (ENUM_ITEM_ID)ItemID, (char)Amount );
						RecipeSlot Slot;
						Slot.Item = Item;
						Slot.x = x;
						Slot.y = y-1;
						RecipeSlots.push_back( Slot );
					}
				}
			}
			else if( y < 0 )
			{
				if( Amount < 0 )
				{
					for(int xx = 0; xx < width; ++xx) for(int y = 0; y < height; ++y )
					{
						if( xx == x-1 ) continue;
						cItem Item( (ENUM_ITEM_ID)ItemID, (char)abs(Amount) );
						RecipeSlot Slot;
						Slot.Item = Item;
						Slot.x = xx;
						Slot.y = y;
						RecipeSlots.push_back( Slot );
					}
				}
				else
				{
					for(int y = 0; y < height; ++y)
					{
						cItem Item( (ENUM_ITEM_ID)ItemID, (char)Amount );
						RecipeSlot Slot;
						Slot.Item = Item;
						Slot.x = x-1;
						Slot.y = y;
						RecipeSlots.push_back( Slot );
					}
				}
			}
			else
			{
				if( Amount < 0 )
				{
					for(int xx = 0; xx < width; ++xx) for(int yy = 0; yy < height; ++yy )
					{
						if( xx == x-1 && yy == y-1 ) continue;
						cItem Item( (ENUM_ITEM_ID)ItemID, (char)abs(Amount) );
						RecipeSlot Slot;
						Slot.Item = Item;
						Slot.x = xx;
						Slot.y = yy;
						RecipeSlots.push_back( Slot );
					}
				}
				else
				{
					cItem Item( (ENUM_ITEM_ID)ItemID, (char)Amount );
					RecipeSlot Slot;
					Slot.Item = Item;
					Slot.x = x-1;
					Slot.y = y-1;
					RecipeSlots.push_back( Slot );
				}
			}
			//LOG("%i %i %i %i", x, y, ItemID, Amount );

			if( bLoadResult )
			{
				bLoadResult = false;
				f >> ItemID;
				f >> c;	if( c != ':' ) { bError=true; break; }
				f >> Amount;
				//LOG("%i %i", ItemID, Amount );
				if( !IsValidItem( ItemID ) )
				{
					LOGERROR("Error in recipes file (%s): Invalid Item ID %i", a_File, ItemID );
					bDontAddRecipe = true;
				}

				// Do a sanity check - Handshake algorithm :)
				bool bDuplicateEntries = false;
				for(unsigned int i = 0; i < RecipeSlots.size(); i++)
				{
					for(unsigned int j = i+1; j < RecipeSlots.size(); j++)
					{
						if( RecipeSlots[i].x == RecipeSlots[j].x && RecipeSlots[i].y == RecipeSlots[j].y )
						{
							LOGERROR("Error in recipes file (%s): Duplicate item on coordinates %i:%i", a_File, RecipeSlots[i].x+1, RecipeSlots[i].y+1 );
							bDontAddRecipe = true;
							bDuplicateEntries = true;
							break;
						}
					}
				}
				if( bDuplicateEntries )
				{
					PrintNear( f, 64 );
					PrintRecipe( RecipeSlots );
				}

				if( bDontAddRecipe == false )
				{
					cItem Item( (ENUM_ITEM_ID)ItemID, (char)Amount );
					Recipe* recipe = new Recipe;
					recipe->Result = Item;
					recipe->NumItems = RecipeSlots.size();
					recipe->Slots = new RecipeSlot[ recipe->NumItems ];
					memcpy( recipe->Slots, &RecipeSlots[0], sizeof(RecipeSlot)*recipe->NumItems );
					m_pState->Recipes.push_back( recipe );
					//LOG("Loaded recipe for %i times %i", Amount, ItemID );
				}

				RecipeSlots.clear();
			}
		}
		if( bError ) break;
	}
	if( bError || ( !f.eof() && !f.good() ) )
	{
		LOGERROR("Error: Wrong format");
		PrintNear( f, 128 );
	}
	f.close();

 	LOG("--Done loading recipes, found %i recipes", m_pState->Recipes.size() );
}





cItem cRecipeChecker::CookIngredients( cItem* a_Items, int a_Width, int a_Height, bool a_bConsumeIngredients /* = false */ )
{
	int iLeft = 999, iTop = 999;
	int iRight = 0, iBottom = 0;
	for(int y = 0; y < a_Height; y++ ) for(int x = 0; x < a_Width; x++)
	{
		cItem Item = a_Items[x + y*a_Width];
		if( Item.m_ItemID != E_ITEM_EMPTY && Item.m_ItemCount > 0 )
		{
			iRight  = MAX(x, iRight);
			iBottom = MAX(y, iBottom);
			iLeft   = MIN(x, iLeft);
			iTop    = MIN(y, iTop);
		}
	}

	for(RecipeList::iterator itr = m_pState->Recipes.begin(); itr != m_pState->Recipes.end(); ++itr )
	{
		Recipe* recipe = (*itr);
		int Left = 999, Top = 999;
		int Right = 0, Bottom = 0;
		for(unsigned int i = 0; i < recipe->NumItems; i++)
		{
			Right  = MAX(recipe->Slots[i].x, Right);
			Bottom = MAX(recipe->Slots[i].y, Bottom);
			Left   = MIN(recipe->Slots[i].x, Left);
			Top    = MIN(recipe->Slots[i].y, Top);
		}
		if( Right-Left != iRight-iLeft || Bottom-Top != iBottom-iTop ) continue;
		// it has the right dimensions

		// Check for empty spaces
		unsigned int Hash = 0;
		for(unsigned int i = 0; i < recipe->NumItems; i++)
		{
			int x = recipe->Slots[i].x - Left + iLeft +1;
			int y = recipe->Slots[i].y - Top + iTop +1;
			Hash += x + y * a_Width;
		}
		for(int y = 0; y < a_Height; y++ ) for(int x = 0; x < a_Width; x++)
		{
			cItem & Item = a_Items[x + y*a_Width];
			if( Item.m_ItemID != E_ITEM_EMPTY && Item.m_ItemCount > 0 )
			{
				Hash -= (x+1) + (y+1)*a_Width;
			}
		}
		if( Hash != 0 ) continue; // Empty spaces not in right place

		bool bWrong = false;
		for(unsigned int i = 0; i < recipe->NumItems; i++)
		{
			int x = recipe->Slots[i].x - Left + iLeft;
			int y = recipe->Slots[i].y - Top + iTop;
			cItem Item = a_Items[x + y*a_Width];
			if( Item.m_ItemID != recipe->Slots[i].Item.m_ItemID
				|| Item.m_ItemCount < recipe->Slots[i].Item.m_ItemCount )
			{
				bWrong = true;
				break;
			}
		}
		if( bWrong ) continue;

		cItem Dish = recipe->Result;

		// else
		if( a_bConsumeIngredients )
		{
			// Consume! nomnom~
			for(unsigned int i = 0; i < recipe->NumItems; i++)
			{
				int x = recipe->Slots[i].x - Left + iLeft;
				int y = recipe->Slots[i].y - Top + iTop;
				a_Items[x + y*a_Width].m_ItemCount -= recipe->Slots[i].Item.m_ItemCount;
				if( a_Items[x + y*a_Width].m_ItemCount <= 0 ) a_Items[x + y*a_Width].Empty();
			}
			Dish = CookIngredients( a_Items, a_Width, a_Height, false );
		}

		// Return the resulting dish!
		return Dish;
	}
	return cItem();
}




