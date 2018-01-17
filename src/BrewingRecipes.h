
#pragma once





#include "Item.h"





class cBrewingRecipes
{
public:
	cBrewingRecipes(void);

	void ReloadRecipes(void);

	struct cRecipe
	{
		cRecipe()
		{
			// These items always have the same type
			Input.m_ItemType = E_ITEM_POTION;
			Output.m_ItemType = E_ITEM_POTION;
		}

		cItem Input;
		cItem Output;
		cItem Ingredient;
	};

	/** Returns a recipe for the specified input, nullptr if no recipe found */
	const cRecipe * GetRecipeFrom(const cItem & a_Input, const cItem & a_Ingredient) const;

	/** Returns true if the item is a ingredient, false if not. */
	bool IsIngredient(const cItem & a_Ingredient) const;

	/** Returns true if the item is a bottle / potion, false if not. */
	bool IsBottle(const cItem & a_Item) const;

	/** Returns true if the item is the fuel, false if not. */
	bool IsFuel(const cItem & a_Item) const;
private:
	using cRecipes = std::vector<std::unique_ptr<cBrewingRecipes::cRecipe>>;

	void ClearRecipes(void);

	/** Parses the recipe contained in the line, adds it to m_pState's recipes.
	Logs a warning to the console on input error. */
	void AddRecipeFromLine(AString a_Line, unsigned int a_LineNum);

	/** Parses an item string, returns true if successful. */
	bool ParseItem(const AString & a_String, cItem & a_Item);

	/** The collection of parsed recipes.
	GetRecipeFrom may cache splash variants of recipes here but the observable behaviour is constant, so this should be mutable. */
	mutable cRecipes m_Recipes;
};
