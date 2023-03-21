
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
	cCraftingGrid(std::size_t a_Width, std::size_t a_Height);  // tolua_export
	cCraftingGrid(const cItem * a_Items, std::size_t a_Width, std::size_t a_Height);
	~cCraftingGrid();

	// tolua_begin
	std::size_t     GetWidth (void) const { return m_Width; }
	std::size_t     GetHeight(void) const { return m_Height; }
	cItem & GetItem  (std::size_t x, std::size_t y) const;
	void    SetItem  (std::size_t x, std::size_t y, ENUM_ITEM_TYPE a_ItemType, char a_ItemCount, short a_ItemHealth);
	void    SetItem  (std::size_t x, std::size_t y, const cItem & a_Item);
	void    Clear    (void);

	/** Removes items in a_Grid from m_Items[] (used by cCraftingRecipe::ConsumeIngredients()) */
	void ConsumeGrid(const cCraftingGrid & a_Grid);

	/** Dumps the entire crafting grid using LOGD() */
	void Dump(void) const;

	// tolua_end

	cItem * GetItems(void) const {return m_Items; }

	/** Copies internal contents into the item array specified. Assumes that the array has the same dimensions as self */
	void  CopyToItems(cItem * a_Items) const;

protected:

	std::size_t     m_Width;
	std::size_t     m_Height;
	cItem * m_Items;
} ;  // tolua_export





class cCraftingRecipe  // tolua_export
{  // tolua_export
public:
	cCraftingRecipe(const cCraftingGrid & a_CraftingGrid);

	// tolua_begin
	void          Clear               (void);
	std::size_t   GetIngredientsWidth (void) const {return m_Ingredients.GetWidth(); }
	std::size_t   GetIngredientsHeight(void) const {return m_Ingredients.GetHeight(); }
	cItem &       GetIngredient       (std::size_t x, std::size_t y) const {return m_Ingredients.GetItem(x, y); }
	const cItem & GetResult           (void) const {return m_Result; }
	void          SetResult           (ENUM_ITEM_TYPE a_ItemType, char a_ItemCount, short a_ItemHealth);
	void          SetResult           (const cItem & a_Item)
	{
		m_Result = a_Item;
	}

	void          SetIngredient       (std::size_t x, std::size_t y, ENUM_ITEM_TYPE a_ItemType, char a_ItemCount, short a_ItemHealth)
	{
		m_Ingredients.SetItem(x, y, a_ItemType, a_ItemCount, a_ItemHealth);
	}

	void          SetIngredient       (std::size_t x, std::size_t y, const cItem & a_Item)
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




/**
The crafting recipes are the configurations to build a result item out of a set
of ingredient items.

The recipes are configured in the `crafting.txt`. When populating the crafting
grid in game (inventory or crafting table), the items are compared to the
ingredients to find a matching recipe and show and craft the result.

Each recipe is defined via the result, the ingredients and the minecraft recipe
name.

To handle the crafting recipes internally efficient the vector index of the
`cRecipes` is used as `RecipeId`.
*/
class cCraftingRecipes
{
public:
	static const int MAX_GRID_WIDTH  = 3;
	static const int MAX_GRID_HEIGHT = 3;

	cCraftingRecipes(void);
	~cCraftingRecipes();

	/** Returns the recipe for current crafting grid. Doesn't modify the grid. Clears a_Recipe if no recipe found. */
	void GetRecipe(cPlayer & a_Player, cCraftingGrid & a_CraftingGrid, cCraftingRecipe & a_Recipe);

	/** Find recipes and returns the RecipeIds which contain the new item and all ingredients are in the known items */
	std::vector<UInt32> FindNewRecipesForItem(const cItem & a_Item, const std::set<cItem, cItem::sItemCompare> & a_KnownItems);

	struct cRecipeSlot
	{
		cItem m_Item;
		std::size_t x, y;  // 1..3, or ILLEGAL_SLOT_NUMBER for "any"
	} ;
	using cRecipeSlots = std::vector<cRecipeSlot>;

	/** A single recipe, stored. Each recipe is normalized right after parsing (NormalizeIngredients())
	A normalized recipe starts at (0, 0) */
	struct cRecipe
	{
		cRecipeSlots m_Ingredients;
		cItem        m_Result;
		AString      m_RecipeName;

		// Size of the regular items in the recipe; "anywhere" items are excluded:
		std::size_t m_Width;
		std::size_t m_Height;
	} ;

	/** Returns the recipe by id */
	cRecipe * GetRecipeById(UInt32 a_RecipeId);

	/** Gets a map of all recipes with name and recipe id */
	const std::map<AString, UInt32> & GetRecipeNameMap();

protected:

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
	cRecipe * FindRecipe(const cItem * a_CraftingGrid, std::size_t a_GridWidth, std::size_t a_GridHeight);

	/** Same as FindRecipe, but the grid is guaranteed to be of minimal dimensions needed */
	cRecipe * FindRecipeCropped(const cItem * a_CraftingGrid, std::size_t a_GridWidth, std::size_t a_GridHeight, std::size_t a_GridStride);

	/** Checks if the grid matches the specified recipe, offset by the specified offsets. Returns a matched cRecipe * if so, or nullptr if not matching. Caller must delete the return value! */
	cRecipe * MatchRecipe(const cItem * a_CraftingGrid, std::size_t a_GridWidth, std::size_t a_GridHeight, std::size_t a_GridStride, const cRecipe * a_Recipe, std::size_t a_OffsetX, std::size_t a_OffsetY);

	/** Searches for anything firework related, and does the data setting if appropriate */
	void HandleFireworks(const cItem * a_CraftingGrid, cCraftingRecipes::cRecipe * a_Recipe, std::size_t a_GridStride, std::size_t a_OffsetX, std::size_t a_OffsetY);

	/** Searches for anything dye related for leather, calculates the appropriate color value, and sets the resulting value. */
	void HandleDyedLeather(const cItem * a_CraftingGrid, cCraftingRecipes::cRecipe * a_Recipe, std::size_t a_GridStride, std::size_t a_GridWidth, std::size_t a_GridHeight);

private:
	/** Mapping the minecraft recipe names to the internal cuberite recipe Ids */
	std::map<AString, UInt32> m_RecipeNameMap;

	/**
	Checks if all ingredients of the a_Recipe are within the a_KnownItems list and
	if the a_NewItem is part of the ingredients.
	This makes sure to only find 'newly discovered' recipes.
	*/
	bool IsNewCraftableRecipe(
		const cRecipe * a_Recipe,
		const cItem & a_NewItem,
		const std::set<cItem, cItem::sItemCompare> & a_KnownItems
	);

	/** Populates the RecipeNameMap */
	void PopulateRecipeNameMap(void);
} ;
