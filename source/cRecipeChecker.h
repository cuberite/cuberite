#pragma once

#include "cItem.h"

class cRecipeChecker
{
public:
	static cRecipeChecker * GetRecipeChecker();

	// Grid of cItems of a_Width width and a_Height Height
	cItem CookIngredients( cItem* a_Items, int a_Width, int a_Height, bool a_bConsumeIngredients = false );

	struct RecipeSlot
	{
		cItem Item;
		int x, y;
	};

	struct Recipe
	{
		Recipe() : Slots( 0 ), NumItems( 0 ) {}
		~Recipe();
		RecipeSlot* Slots; // Array of RecipeSlots
		unsigned int NumItems;
		cItem Result;
	};

	void ReloadRecipes();
	static void DeleteMe();
private:
	friend class cRoot;
	cRecipeChecker();
	~cRecipeChecker();

	struct sRecipeCheckerState;
	sRecipeCheckerState* m_pState;

	void ClearRecipes();
};