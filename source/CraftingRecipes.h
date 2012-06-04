
// CraftingRecipes.h

// Interfaces to the cCraftingRecipes class representing the storage of crafting recipes




#pragma once

#include "cItem.h"





class cCraftingRecipes
{
public:
	static const int MAX_GRID_WIDTH  = 3;
	static const int MAX_GRID_HEIGHT = 3;
	
	cCraftingRecipes(void);
	~cCraftingRecipes();
	
	/// Offers an item resulting from the crafting grid setup. Doesn't modify the grid
	cItem Offer(const cItem * a_CraftingGrid, int a_GridWidth, int a_GridHeight);
	
	/// Crafts the item resulting from the crafting grid setup. Modifies the grid, returns the crafted item
	cItem Craft(cItem * a_CraftingGrid, int a_GridWidth, int a_GridHeight);
	
protected:

	struct cRecipeSlot
	{
		cItem m_Item;
		int x, y;  // 1..3, or -1 for "any"
	} ;
	typedef std::vector<cRecipeSlot> cRecipeSlots;
	
	// A single recipe, stored. Each recipe is normalized right after parsing (NormalizeIngredients())
	struct cRecipe
	{
		cRecipeSlots m_Ingredients;
		cItem        m_Result;
		
		// Size of the regular items in the recipe; "anywhere" items are excluded:
		int m_Width;
		int m_Height;
	} ;
	typedef std::vector<cRecipe *> cRecipes;
	
	cRecipes m_Recipes;
	
	void LoadRecipes(void);
	void ClearRecipes(void);
	
	/// Parses the recipe line and adds it into m_Recipes. a_LineNum is used for diagnostic warnings only
	void AddRecipeLine(int a_LineNum, const AString & a_RecipeLine);
	
	/// Parses an item string in the format "<ItemType>[^<Damage>]", returns true if successful.
	bool ParseItem(const AString & a_String, cItem & a_Item);
	
	/// Parses one ingredient and adds it to the specified recipe. Returns true if successful.
	bool ParseIngredient(const AString & a_String, cRecipe * a_Recipe);
	
	/// Moves the recipe to top-left corner, sets its MinWidth / MinHeight
	void NormalizeIngredients(cRecipe * a_Recipe);
	
	/// Finds a recipe matching the crafting grid. Returns a newly allocated recipe (with all its coords set) or NULL if not found. Caller must delete return value!
	cRecipe * FindRecipe(const cItem * a_CraftingGrid, int a_GridWidth, int a_GridHeight);
	
	/// Same as FindRecipe, but the grid is guaranteed to be of minimal dimensions needed
	cRecipe * FindRecipeCropped(const cItem * a_CraftingGrid, int a_GridWidth, int a_GridHeight, int a_GridStride);
	
	/// Checks if the grid matches the specified recipe, offset by the specified offsets. Returns a matched cRecipe * if so, or NULL if not matching. Caller must delete the return value!
	cRecipe * MatchRecipe(const cItem * a_CraftingGrid, int a_GridWidth, int a_GridHeight, int a_GridStride, const cRecipe * a_Recipe, int a_OffsetX, int a_OffsetY);
} ;




