
// CraftingRecipes.h

// Interfaces to the cCraftingRecipes class representing the storage of crafting recipes




#pragma once

#include "Item.h"





// fwd: cPlayer.h
class cPlayer;





class cCraftingGrid  // tolua_export
{  // tolua_export
public:
	cCraftingGrid(const cCraftingGrid & a_Original);
	cCraftingGrid(int a_Width, int a_Height);  // tolua_export
	cCraftingGrid(const cItem * a_Items, int a_Width, int a_Height);
	~cCraftingGrid();

	// tolua_begin
	int     GetWidth (void) const {return m_Width; }
	int     GetHeight(void) const {return m_Height; }
	cItem & GetItem  (int x, int y) const;
	void    SetItem  (int x, int y, ENUM_ITEM_ID a_ItemType, char a_ItemCount, short a_ItemHealth);
	void    SetItem  (int x, int y, const cItem & a_Item);
	void    Clear    (void);

	/** Removes items in a_Grid from m_Items[] (used by cCraftingRecipe::ConsumeIngredients()) */
	void ConsumeGrid(const cCraftingGrid & a_Grid);

	/** Dumps the entire crafting grid using LOGD() */
	void Dump(void);

	// tolua_end

	cItem * GetItems(void) const {return m_Items; }

	/** Copies internal contents into the item array specified. Assumes that the array has the same dimensions as self */
	void  CopyToItems(cItem * a_Items) const;

protected:

	int     m_Width;
	int     m_Height;
	cItem * m_Items;
} ;  // tolua_export





class cCraftingRecipe  // tolua_export
{  // tolua_export
public:
	cCraftingRecipe(const cCraftingGrid & a_CraftingGrid);

	// tolua_begin
	void          Clear               (void);
	int           GetIngredientsWidth (void) const {return m_Ingredients.GetWidth(); }
	int           GetIngredientsHeight(void) const {return m_Ingredients.GetHeight(); }
	cItem &       GetIngredient       (int x, int y) const {return m_Ingredients.GetItem(x, y); }
	const cItem & GetResult           (void) const {return m_Result; }
	void          SetResult           (ENUM_ITEM_ID a_ItemType, char a_ItemCount, short a_ItemHealth);
	void          SetResult           (const cItem & a_Item)
	{
		m_Result = a_Item;
	}

	void          SetIngredient       (int x, int y, ENUM_ITEM_ID a_ItemType, char a_ItemCount, short a_ItemHealth)
	{
		m_Ingredients.SetItem(x, y, a_ItemType, a_ItemCount, a_ItemHealth);
	}

	void          SetIngredient       (int x, int y, const cItem & a_Item)
	{
		m_Ingredients.SetItem(x, y, a_Item);
	}

	/** Consumes ingredients from the crafting grid specified */
	void ConsumeIngredients(cCraftingGrid & a_CraftingGrid);

	/** Dumps the entire recipe using LOGD() */
	void Dump(void);
	// tolua_end

protected:

	cCraftingGrid m_Ingredients;  // Adjusted to correspond to the input crafting grid!
	cItem         m_Result;
} ;  // tolua_export





class cCraftingRecipes
{
public:
	static const int MAX_GRID_WIDTH  = 3;
	static const int MAX_GRID_HEIGHT = 3;

	cCraftingRecipes(void);
	~cCraftingRecipes();

	/** Returns the recipe for current crafting grid. Doesn't modify the grid. Clears a_Recipe if no recipe found. */
	void GetRecipe(cPlayer & a_Player, cCraftingGrid & a_CraftingGrid, cCraftingRecipe & a_Recipe);

protected:

	struct cRecipeSlot
	{
		cItem m_Item;
		int x, y;  // 1..3, or -1 for "any"
	} ;
	typedef std::vector<cRecipeSlot> cRecipeSlots;

	/** A single recipe, stored. Each recipe is normalized right after parsing (NormalizeIngredients())
	A normalized recipe starts at (0, 0) */
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

	/** Parses the recipe line and adds it into m_Recipes. a_LineNum is used for diagnostic warnings only */
	void AddRecipeLine(int a_LineNum, const AString & a_RecipeLine);

	/** Parses an item string in the format "<ItemType>[^<Damage>]", returns true if successful. */
	bool ParseItem(const AString & a_String, cItem & a_Item);

	/** Parses one ingredient and adds it to the specified recipe. Returns true if successful. */
	bool ParseIngredient(const AString & a_String, cRecipe * a_Recipe);

	/** Moves the recipe to top-left corner, sets its MinWidth / MinHeight */
	void NormalizeIngredients(cRecipe * a_Recipe);

	/** Finds a recipe matching the crafting grid. Returns a newly allocated recipe (with all its coords set) or nullptr if not found. Caller must delete return value! */
	cRecipe * FindRecipe(const cItem * a_CraftingGrid, int a_GridWidth, int a_GridHeight);

	/** Same as FindRecipe, but the grid is guaranteed to be of minimal dimensions needed */
	cRecipe * FindRecipeCropped(const cItem * a_CraftingGrid, int a_GridWidth, int a_GridHeight, int a_GridStride);

	/** Checks if the grid matches the specified recipe, offset by the specified offsets. Returns a matched cRecipe * if so, or nullptr if not matching. Caller must delete the return value! */
	cRecipe * MatchRecipe(const cItem * a_CraftingGrid, int a_GridWidth, int a_GridHeight, int a_GridStride, const cRecipe * a_Recipe, int a_OffsetX, int a_OffsetY);

	/** Searches for anything firework related, and does the data setting if appropriate */
	void HandleFireworks(const cItem * a_CraftingGrid, cCraftingRecipes::cRecipe * a_Recipe, int a_GridStride, int a_OffsetX, int a_OffsetY);

	/** Searches for anything dye related for leather, calculates the appropriate color value, and sets the resulting value. */
	void HandleDyedLeather(const cItem * a_CraftingGrid, cCraftingRecipes::cRecipe * a_Recipe, int a_GridStride, int a_GridWidth, int a_GridHeight);
} ;




