
// CraftingRecipes.cpp

// Interfaces to the cCraftingRecipes class representing the storage of crafting recipes

#include "Globals.h"
#include "CraftingRecipes.h"
#include "Root.h"
#include "Bindings/PluginManager.h"





////////////////////////////////////////////////////////////////////////////////
// cCraftingGrid:

cCraftingGrid::cCraftingGrid(int a_Width, int a_Height) :
	m_Width(a_Width),
	m_Height(a_Height),
	m_Items(new cItem[ToUnsigned(a_Width * a_Height)])
{
}





cCraftingGrid::cCraftingGrid(const cItem * a_Items, int a_Width, int a_Height) :
	cCraftingGrid(a_Width, a_Height)
{
	for (int i = a_Width * a_Height - 1; i >= 0; i--)
	{
		m_Items[i] = a_Items[i];
	}
}





cCraftingGrid::cCraftingGrid(const cCraftingGrid & a_Original) :
	cCraftingGrid(a_Original.m_Width, a_Original.m_Height)
{
	for (int i = m_Width * m_Height - 1; i >= 0; i--)
	{
		m_Items[i] = a_Original.m_Items[i];
	}
}





cCraftingGrid::~cCraftingGrid()
{
	delete[] m_Items;
	m_Items = nullptr;
}





cItem & cCraftingGrid::GetItem(int x, int y) const
{
	// Accessible through scripting, must verify parameters:
	if ((x < 0) || (x >= m_Width) || (y < 0) || (y >= m_Height))
	{
		LOGERROR("Attempted to get an invalid item from a crafting grid: (%d, %d), grid dimensions: (%d, %d).",
			x, y, m_Width, m_Height
		);
		return m_Items[0];
	}
	return m_Items[x + m_Width * y];
}





void cCraftingGrid::SetItem(int x, int y, ENUM_ITEM_TYPE a_ItemType, char a_ItemCount, short a_ItemHealth)
{
	// Accessible through scripting, must verify parameters:
	if ((x < 0) || (x >= m_Width) || (y < 0) || (y >= m_Height))
	{
		LOGERROR("Attempted to set an invalid item in a crafting grid: (%d, %d), grid dimensions: (%d, %d).",
			x, y, m_Width, m_Height
		);
		return;
	}

	m_Items[x + m_Width * y] = cItem(a_ItemType, a_ItemCount, a_ItemHealth);
}





void cCraftingGrid::SetItem(int x, int y, const cItem & a_Item)
{
	// Accessible through scripting, must verify parameters:
	if ((x < 0) || (x >= m_Width) || (y < 0) || (y >= m_Height))
	{
		LOGERROR("Attempted to set an invalid item in a crafting grid: (%d, %d), grid dimensions: (%d, %d).",
			x, y, m_Width, m_Height
		);
		return;
	}

	m_Items[x + m_Width * y] = a_Item;
}





void cCraftingGrid::Clear(void)
{
	for (int y = 0; y < m_Height; y++) for (int x = 0; x < m_Width; x++)
	{
		m_Items[x + m_Width * y].Empty();
	}
}





void cCraftingGrid::ConsumeGrid(const cCraftingGrid & a_Grid)
{
	if ((a_Grid.m_Width != m_Width) || (a_Grid.m_Height != m_Height))
	{
		LOGWARNING("Consuming a grid of different dimensions: (%d, %d) vs (%d, %d)",
			a_Grid.m_Width, a_Grid.m_Height, m_Width, m_Height
		);
	}
	int MinX = std::min(a_Grid.m_Width,  m_Width);
	int MinY = std::min(a_Grid.m_Height, m_Height);
	for (int y = 0; y <  MinY; y++) for (int x = 0; x < MinX; x++)
	{
		int ThatIdx = x + a_Grid.m_Width * y;
		if (a_Grid.m_Items[ThatIdx].IsEmpty())
		{
			continue;
		}
		int ThisIdx = x + m_Width * y;
		if (a_Grid.m_Items[ThatIdx].m_ItemType != m_Items[ThisIdx].m_ItemType)
		{
			LOGWARNING("Consuming incompatible grids: item at (%d, %d) is %d in grid and %d in ingredients. Item not consumed.",
				x, y, m_Items[ThisIdx].m_ItemType, a_Grid.m_Items[ThatIdx].m_ItemType
			);
			continue;
		}
		char NumWantedItems = a_Grid.m_Items[ThatIdx].m_ItemCount;
		if (NumWantedItems > m_Items[ThisIdx].m_ItemCount)
		{
			LOGWARNING("Consuming more items than there actually are in slot (%d, %d), item %d (want %d, have %d). Item zeroed out.",
				x, y, m_Items[ThisIdx].m_ItemType,
				NumWantedItems, m_Items[ThisIdx].m_ItemCount
			);
			NumWantedItems = m_Items[ThisIdx].m_ItemCount;
		}
		m_Items[ThisIdx].m_ItemCount -= NumWantedItems;
		if (m_Items[ThisIdx].m_ItemCount == 0)
		{
			if ((m_Items[ThisIdx].m_ItemType == E_ITEM_MILK) || (m_Items[ThisIdx].m_ItemType == E_ITEM_WATER_BUCKET) || (m_Items[ThisIdx].m_ItemType == E_ITEM_LAVA_BUCKET))
			{
				m_Items[ThisIdx] = cItem(E_ITEM_BUCKET);
			}
			else
			{
				m_Items[ThisIdx].Clear();
			}
		}
	}  // for x, for y
}





void cCraftingGrid::CopyToItems(cItem * a_Items) const
{
	for (int i = m_Height * m_Width - 1; i >= 0; i--)
	{
		a_Items[i] = m_Items[i];
	}  // for x, for y
}





void cCraftingGrid::Dump(void)
{
	for (int y = 0; y < m_Height; y++) for (int x = 0; x < m_Width; x++)
	{
		#ifdef _DEBUG
		int idx = x + m_Width * y;
		#endif
		LOGD("Slot (%d, %d): Type %d, health %d, count %d",
			x, y, m_Items[idx].m_ItemType, m_Items[idx].m_ItemDamage, m_Items[idx].m_ItemCount
		);
	}
}





////////////////////////////////////////////////////////////////////////////////
// cCraftingRecipe:

cCraftingRecipe::cCraftingRecipe(const cCraftingGrid & a_CraftingGrid) :
	m_Ingredients(a_CraftingGrid)
{
}





void cCraftingRecipe::Clear(void)
{
	m_Ingredients.Clear();
	m_Result.Clear();
}





void cCraftingRecipe::SetResult(ENUM_ITEM_TYPE a_ItemType, char a_ItemCount, short a_ItemHealth)
{
	m_Result = cItem(a_ItemType, a_ItemCount, a_ItemHealth);
}





void cCraftingRecipe::ConsumeIngredients(cCraftingGrid & a_CraftingGrid)
{
	a_CraftingGrid.ConsumeGrid(m_Ingredients);
}





void cCraftingRecipe::Dump(void)
{
	LOGD("Recipe ingredients:");
	m_Ingredients.Dump();
	LOGD("Result: Type %d, health %d, count %d",
		m_Result.m_ItemType, m_Result.m_ItemDamage, m_Result.m_ItemCount
	);
}





////////////////////////////////////////////////////////////////////////////////
// cCraftingRecipes:

cCraftingRecipes::cCraftingRecipes(void)
{
	LoadRecipes();
	PopulateRecipeNameMap();
}





cCraftingRecipes::~cCraftingRecipes()
{
	ClearRecipes();
}





bool cCraftingRecipes::IsNewCraftableRecipe(const cRecipe * a_Recipe, const cItem & a_Item, const std::set<cItem, cItem::sItemCompare> & a_KnownItems)
{
	bool ContainsNewItem = false;
	for (const auto & Ingredient : a_Recipe->m_Ingredients)
	{
		if (
			(Ingredient.m_Item.m_ItemType == a_Item.m_ItemType) &&
			(
				(Ingredient.m_Item.m_ItemDamage == a_Item.m_ItemDamage) ||
				(Ingredient.m_Item.m_ItemDamage == -1)
			)
		)
		{
			ContainsNewItem = true;
		}
		if (a_KnownItems.find(Ingredient.m_Item) == a_KnownItems.end())
		{
			return false;
		}
	}
	return ContainsNewItem;
}





std::vector<UInt32> cCraftingRecipes::FindNewRecipesForItem(const cItem & a_Item, const std::set<cItem, cItem::sItemCompare> & a_KnownItems)
{
	std::vector<UInt32> Recipes;
	for (UInt32 i = 0; i < m_Recipes.size(); i++)
	{
		if (m_Recipes[i]->m_RecipeName.empty())
		{
			continue;
		}
		if (IsNewCraftableRecipe(m_Recipes[i], a_Item, a_KnownItems))
		{
			Recipes.push_back(i);
		}
	}
	return Recipes;
}





const std::map<AString, UInt32> & cCraftingRecipes::GetRecipeNameMap()
{
	return m_RecipeNameMap;
}





cCraftingRecipes::cRecipe * cCraftingRecipes::GetRecipeById(UInt32 a_RecipeId)
{
	return m_Recipes[a_RecipeId];
}





void cCraftingRecipes::GetRecipe(cPlayer & a_Player, cCraftingGrid & a_CraftingGrid, cCraftingRecipe & a_Recipe)
{
	// Allow plugins to intercept recipes using a pre-craft hook:
	if (cRoot::Get()->GetPluginManager()->CallHookPreCrafting(a_Player, a_CraftingGrid, a_Recipe))
	{
		return;
	}

	// Built-in recipes:
	std::unique_ptr<cRecipe> Recipe(FindRecipe(a_CraftingGrid.GetItems(), a_CraftingGrid.GetWidth(), a_CraftingGrid.GetHeight()));
	a_Recipe.Clear();
	if (Recipe.get() == nullptr)
	{
		// Allow plugins to intercept a no-recipe-found situation:
		cRoot::Get()->GetPluginManager()->CallHookCraftingNoRecipe(a_Player, a_CraftingGrid, a_Recipe);
		return;
	}
	for (cRecipeSlots::const_iterator itr = Recipe->m_Ingredients.begin(); itr != Recipe->m_Ingredients.end(); ++itr)
	{
		a_Recipe.SetIngredient(itr->x, itr->y, itr->m_Item);
	}  // for itr
	a_Recipe.SetResult(Recipe->m_Result);

	// Allow plugins to intercept recipes after they are processed:
	cRoot::Get()->GetPluginManager()->CallHookPostCrafting(a_Player, a_CraftingGrid, a_Recipe);
}





void cCraftingRecipes::LoadRecipes(void)
{
	LOGD("Loading crafting recipes from crafting.txt...");
	ClearRecipes();

	// Load the crafting.txt file:
	cFile f;
	if (!f.Open("crafting.txt", cFile::fmRead))
	{
		LOGWARNING("Cannot open file \"crafting.txt\", no crafting recipes will be available!");
		return;
	}
	AString Everything;
	if (!f.ReadRestOfFile(Everything))
	{
		LOGWARNING("Cannot read file \"crafting.txt\", no crafting recipes will be available!");
		return;
	}
	f.Close();

	// Split it into lines, then process each line as a single recipe:
	AStringVector Split = StringSplit(Everything, "\n");
	int LineNum = 1;
	for (AStringVector::const_iterator itr = Split.begin(); itr != Split.end(); ++itr, ++LineNum)
	{
		// Remove anything after a '#' sign and trim away the whitespace:
		AString Recipe = TrimString(itr->substr(0, itr->find('#')));
		if (Recipe.empty())
		{
			// Empty recipe
			continue;
		}
		AddRecipeLine(LineNum, Recipe);
	}  // for itr - Split[]
	LOG("Loaded %zu crafting recipes", m_Recipes.size());
}





void cCraftingRecipes::PopulateRecipeNameMap(void)
{
	for (UInt32 i = 0; i < m_Recipes.size(); i++)
	{
		if (!m_Recipes[i]->m_RecipeName.empty())
		{
			m_RecipeNameMap.emplace(m_Recipes[i]->m_RecipeName, i);
		}
	}
}





void cCraftingRecipes::ClearRecipes(void)
{
	for (cRecipes::iterator itr = m_Recipes.begin(); itr != m_Recipes.end(); ++itr)
	{
		delete *itr;
	}
	m_Recipes.clear();
}





void cCraftingRecipes::AddRecipeLine(int a_LineNum, const AString & a_RecipeLine)
{
	// Remove any spaces within the line:
	AString RecipeLine(a_RecipeLine);
	RecipeLine.erase(std::remove_if(RecipeLine.begin(), RecipeLine.end(), isspace), RecipeLine.end());

	AStringVector Sides = StringSplit(RecipeLine, "=");
	if (Sides.size() != 2)
	{
		LOGWARNING("crafting.txt: line %d: A single '=' was expected, got %zu", a_LineNum, Sides.size() - 1);
		LOGINFO("Offending line: \"%s\"", a_RecipeLine.c_str());
		return;
	}

	std::unique_ptr<cCraftingRecipes::cRecipe> Recipe = std::make_unique<cCraftingRecipes::cRecipe>();

	AStringVector RecipeSplit = StringSplit(Sides[0], ":");
	const auto * resultPart = &RecipeSplit[0];
	if (RecipeSplit.size() > 1)
	{
		resultPart = &RecipeSplit[1];
		Recipe->m_RecipeName = RecipeSplit[0];
	}
	// Parse the result:
	AStringVector ResultSplit = StringSplit(*resultPart, ",");
	if (ResultSplit.empty())
	{
		LOGWARNING("crafting.txt: line %d: Result is empty, ignoring the recipe.", a_LineNum);
		LOGINFO("Offending line: \"%s\"", a_RecipeLine.c_str());
		return;
	}
	if (!ParseItem(ResultSplit[0], Recipe->m_Result))
	{
		LOGWARNING("crafting.txt: line %d: Cannot parse result item, ignoring the recipe.", a_LineNum);
		LOGINFO("Offending line: \"%s\"", a_RecipeLine.c_str());
		return;
	}
	if (ResultSplit.size() > 1)
	{
		if (!StringToInteger<char>(ResultSplit[1], Recipe->m_Result.m_ItemCount))
		{
			LOGWARNING("crafting.txt: line %d: Cannot parse result count, ignoring the recipe.", a_LineNum);
			LOGINFO("Offending line: \"%s\"", a_RecipeLine.c_str());
			return;
		}
	}
	else
	{
		Recipe->m_Result.m_ItemCount = 1;
	}

	// Parse each ingredient:
	AStringVector Ingredients = StringSplit(Sides[1], "|");
	int Num = 1;
	for (AStringVector::const_iterator itr = Ingredients.begin(); itr != Ingredients.end(); ++itr, ++Num)
	{
		if (!ParseIngredient(*itr, Recipe.get()))
		{
			LOGWARNING("crafting.txt: line %d: Cannot parse ingredient #%d, ignoring the recipe.", a_LineNum, Num);
			LOGINFO("Offending line: \"%s\"", a_RecipeLine.c_str());
			return;
		}
	}  // for itr - Ingredients[]

	NormalizeIngredients(Recipe.get());

	m_Recipes.push_back(Recipe.release());
}





bool cCraftingRecipes::ParseItem(const AString & a_String, cItem & a_Item)
{
	// The caller provides error logging

	AStringVector Split = StringSplit(a_String, "^");
	if (Split.empty())
	{
		return false;
	}

	if (!StringToItem(Split[0], a_Item))
	{
		return false;
	}

	if (Split.size() > 1)
	{
		AString Damage = TrimString(Split[1]);
		if (!StringToInteger<short>(Damage, a_Item.m_ItemDamage))
		{
			// Parsing the number failed
			return false;
		}
	}

	// Success
	return true;
}





bool cCraftingRecipes::ParseIngredient(const AString & a_String, cRecipe * a_Recipe)
{
	// a_String is in this format: "ItemType^damage, X:Y, X:Y, X:Y..."
	AStringVector Split = StringSplit(a_String, ",");
	if (Split.size() < 2)
	{
		// Not enough split items
		return false;
	}
	cItem Item;
	if (!ParseItem(Split[0], Item))
	{
		return false;
	}
	Item.m_ItemCount = 1;

	cCraftingRecipes::cRecipeSlots TempSlots;
	for (AStringVector::const_iterator itr = Split.begin() + 1; itr != Split.end(); ++itr)
	{
		// Parse the coords in the split item:
		AStringVector Coords = StringSplit(*itr, ":");
		if ((Coords.size() == 1) && (TrimString(Coords[0]) == "*"))
		{
			cCraftingRecipes::cRecipeSlot Slot;
			Slot.m_Item = Item;
			Slot.x = -1;
			Slot.y = -1;
			TempSlots.push_back(Slot);
			continue;
		}
		if (Coords.size() != 2)
		{
			return false;
		}
		Coords[0] = TrimString(Coords[0]);
		Coords[1] = TrimString(Coords[1]);
		if (Coords[0].empty() || Coords[1].empty())
		{
			return false;
		}
		cCraftingRecipes::cRecipeSlot Slot;
		Slot.m_Item = Item;
		switch (Coords[0][0])
		{
			case '1': Slot.x = 0;  break;
			case '2': Slot.x = 1;  break;
			case '3': Slot.x = 2;  break;
			case '*': Slot.x = -1; break;
			default:
			{
				return false;
			}
		}
		switch (Coords[1][0])
		{
			case '1': Slot.y = 0;  break;
			case '2': Slot.y = 1;  break;
			case '3': Slot.y = 2;  break;
			case '*': Slot.y = -1; break;
			default:
			{
				return false;
			}
		}
		TempSlots.push_back(Slot);
	}  // for itr - Split[]

	// Append the ingredients:
	a_Recipe->m_Ingredients.insert(a_Recipe->m_Ingredients.end(), TempSlots.begin(), TempSlots.end());
	return true;
}





void cCraftingRecipes::NormalizeIngredients(cCraftingRecipes::cRecipe * a_Recipe)
{
	// Calculate the minimum coords for ingredients, excluding the "anywhere" items:
	int MinX = MAX_GRID_WIDTH, MaxX = 0;
	int MinY = MAX_GRID_HEIGHT, MaxY = 0;
	for (cRecipeSlots::const_iterator itr = a_Recipe->m_Ingredients.begin(); itr != a_Recipe->m_Ingredients.end(); ++itr)
	{
		if (itr->x >= 0)
		{
			MinX = std::min(itr->x, MinX);
			MaxX = std::max(itr->x, MaxX);
		}
		if (itr->y >= 0)
		{
			MinY = std::min(itr->y, MinY);
			MaxY = std::max(itr->y, MaxY);
		}
	}  // for itr - a_Recipe->m_Ingredients[]

	// Move ingredients so that the minimum coords are 0:0
	for (cRecipeSlots::iterator itr = a_Recipe->m_Ingredients.begin(); itr != a_Recipe->m_Ingredients.end(); ++itr)
	{
		if (itr->x >= 0)
		{
			itr->x -= MinX;
		}
		if (itr->y >= 0)
		{
			itr->y -= MinY;
		}
	}  // for itr - a_Recipe->m_Ingredients[]
	a_Recipe->m_Width  = std::max(MaxX - MinX + 1, 1);
	a_Recipe->m_Height = std::max(MaxY - MinY + 1, 1);

	// TODO: Compress two same ingredients with the same coords into a single ingredient with increased item count
}





cCraftingRecipes::cRecipe * cCraftingRecipes::FindRecipe(const cItem * a_CraftingGrid, int a_GridWidth, int a_GridHeight)
{
	ASSERT(a_GridWidth <= MAX_GRID_WIDTH);
	ASSERT(a_GridHeight <= MAX_GRID_HEIGHT);

	// Get the real bounds of the crafting grid:
	int GridLeft = MAX_GRID_WIDTH, GridTop = MAX_GRID_HEIGHT;
	int GridRight = 0,  GridBottom = 0;
	for (int y = 0; y < a_GridHeight; y++) for (int x = 0; x < a_GridWidth; x++)
	{
		if (!a_CraftingGrid[x + y * a_GridWidth].IsEmpty())
		{
			GridRight  = std::max(x, GridRight);
			GridBottom = std::max(y, GridBottom);
			GridLeft   = std::min(x, GridLeft);
			GridTop    = std::min(y, GridTop);
		}
	}
	int GridWidth = GridRight - GridLeft + 1;
	int GridHeight = GridBottom - GridTop + 1;

	// Search in the possibly minimized grid, but keep the stride:
	const cItem * Grid = a_CraftingGrid + GridLeft + (a_GridWidth * GridTop);
	cRecipe * Recipe = FindRecipeCropped(Grid, GridWidth, GridHeight, a_GridWidth);
	if (Recipe == nullptr)
	{
		return nullptr;
	}

	// A recipe has been found, move it to correspond to the original crafting grid:
	for (cRecipeSlots::iterator itrS = Recipe->m_Ingredients.begin(); itrS != Recipe->m_Ingredients.end(); ++itrS)
	{
		itrS->x += GridLeft;
		itrS->y += GridTop;
	}  // for itrS - Recipe->m_Ingredients[]

	return Recipe;
}





cCraftingRecipes::cRecipe * cCraftingRecipes::FindRecipeCropped(const cItem * a_CraftingGrid, int a_GridWidth, int a_GridHeight, int a_GridStride)
{
	for (cRecipes::const_iterator itr = m_Recipes.begin(); itr != m_Recipes.end(); ++itr)
	{
		// Both the crafting grid and the recipes are normalized. The only variable possible is the "anywhere" items.
		// This still means that the "anywhere" item may be the one that is offsetting the grid contents to the right or downwards, so we need to check all possible positions.
		// E. g. recipe "A, * | B, 1:1 | ..." still needs to check grid for B at 2:2 (in case A was in grid's 1:1)
		// Calculate the maximum offsets for this recipe relative to the grid size, and iterate through all combinations of offsets.
		// Also, this calculation automatically filters out recipes that are too large for the current grid - the loop won't be entered at all.

		int MaxOfsX = a_GridWidth  - (*itr)->m_Width;
		int MaxOfsY = a_GridHeight - (*itr)->m_Height;
		for (int x = 0; x <= MaxOfsX; x++) for (int y = 0; y <= MaxOfsY; y++)
		{
			cRecipe * Recipe = MatchRecipe(a_CraftingGrid, a_GridWidth, a_GridHeight, a_GridStride, *itr, x, y);
			if (Recipe != nullptr)
			{
				return Recipe;
			}
		}  // for y, for x
	}  // for itr - m_Recipes[]

	// No matching recipe found
	return nullptr;
}





cCraftingRecipes::cRecipe * cCraftingRecipes::MatchRecipe(const cItem * a_CraftingGrid, int a_GridWidth, int a_GridHeight, int a_GridStride, const cRecipe * a_Recipe, int a_OffsetX, int a_OffsetY)
{
	// Check the regular items first:
	bool HasMatched[MAX_GRID_WIDTH][MAX_GRID_HEIGHT];
	memset(HasMatched, 0, sizeof(HasMatched));
	for (cRecipeSlots::const_iterator itrS = a_Recipe->m_Ingredients.begin(); itrS != a_Recipe->m_Ingredients.end(); ++itrS)
	{
		if ((itrS->x < 0) || (itrS->y < 0))
		{
			// "Anywhere" item, process later
			continue;
		}
		ASSERT(itrS->x + a_OffsetX < a_GridWidth);
		ASSERT(itrS->y + a_OffsetY < a_GridHeight);
		int GridID = (itrS->x + a_OffsetX) + a_GridStride * (itrS->y + a_OffsetY);

		const cItem & Item = itrS->m_Item;
		if (
			(itrS->x >= a_GridWidth) ||
			(itrS->y >= a_GridHeight) ||
			(Item.m_ItemType != a_CraftingGrid[GridID].m_ItemType) ||   // same item type?
			(Item.m_ItemCount > a_CraftingGrid[GridID].m_ItemCount) ||  // not enough items
			(
				(Item.m_ItemDamage >= 0) &&  // should compare damage values?
				(Item.m_ItemDamage != a_CraftingGrid[GridID].m_ItemDamage)
			)
		)
		{
			// Doesn't match
			return nullptr;
		}
		HasMatched[itrS->x + a_OffsetX][itrS->y + a_OffsetY] = true;
	}  // for itrS - Recipe->m_Ingredients[]

	// Process the "Anywhere" items now, and only in the cells that haven't matched yet
	// The "anywhere" items are processed on a first-come-first-served basis.
	// Do not use a recipe with one horizontal and one vertical "anywhere" ("*:1, 1:*") as it may not match properly!
	cRecipeSlots MatchedSlots;  // Stores the slots of "anywhere" items that have matched, with the match coords
	for (cRecipeSlots::const_iterator itrS = a_Recipe->m_Ingredients.begin(); itrS != a_Recipe->m_Ingredients.end(); ++itrS)
	{
		if ((itrS->x >= 0) && (itrS->y >= 0))
		{
			// Regular item, already processed
			continue;
		}
		int StartX = 0, EndX = a_GridWidth  - 1;
		int StartY = 0, EndY = a_GridHeight - 1;
		if (itrS->x >= 0)
		{
			StartX = itrS->x;
			EndX = itrS->x;
		}
		else if (itrS->y >= 0)
		{
			StartY = itrS->y;
			EndY = itrS->y;
		}
		bool Found = false;
		for (int x = StartX; x <= EndX; x++)
		{
			for (int y = StartY; y <= EndY; y++)
			{
				if (HasMatched[x][y])
				{
					// Already matched some other item
					continue;
				}
				int GridIdx = x + a_GridStride * y;
				if (
					(a_CraftingGrid[GridIdx].m_ItemType == itrS->m_Item.m_ItemType) &&
					(
						(itrS->m_Item.m_ItemDamage < 0) ||  // doesn't want damage comparison
						(itrS->m_Item.m_ItemDamage == a_CraftingGrid[GridIdx].m_ItemDamage)  // the damage matches
					)
				)
				{
					HasMatched[x][y] = true;
					Found = true;
					MatchedSlots.push_back(*itrS);
					MatchedSlots.back().x = x;
					MatchedSlots.back().y = y;
					break;
				}
			}  // for y
			if (Found)
			{
				break;
			}
		}  // for x
		if (!Found)
		{
			return nullptr;
		}
	}  // for itrS - a_Recipe->m_Ingredients[]

	// Check if the whole grid has matched:
	for (int x = 0; x < a_GridWidth; x++) for (int y = 0; y < a_GridHeight; y++)
	{
		if (!HasMatched[x][y] && !a_CraftingGrid[x + a_GridStride * y].IsEmpty())
		{
			// There's an unmatched item in the grid
			return nullptr;
		}
	}  // for y, for x

	// The recipe has matched. Create a copy of the recipe and set its coords to match the crafting grid:
	std::unique_ptr<cRecipe> Recipe = std::make_unique<cRecipe>();
	Recipe->m_Result = a_Recipe->m_Result;
	Recipe->m_Width  = a_Recipe->m_Width;
	Recipe->m_Height = a_Recipe->m_Height;
	for (cRecipeSlots::const_iterator itrS = a_Recipe->m_Ingredients.begin(); itrS != a_Recipe->m_Ingredients.end(); ++itrS)
	{
		if ((itrS->x < 0) || (itrS->y < 0))
		{
			// "Anywhere" item, process later
			continue;
		}
		Recipe->m_Ingredients.push_back(*itrS);
		Recipe->m_Ingredients.back().x += a_OffsetX;
		Recipe->m_Ingredients.back().y += a_OffsetY;
	}
	Recipe->m_Ingredients.insert(Recipe->m_Ingredients.end(), MatchedSlots.begin(), MatchedSlots.end());

	// Handle the fireworks-related effects:
	// We use Recipe instead of a_Recipe because we want the wildcard ingredients' slot numbers as well, which was just added previously
	HandleFireworks(a_CraftingGrid, Recipe.get(), a_GridStride, a_OffsetX, a_OffsetY);

	// Handle Dyed Leather
	HandleDyedLeather(a_CraftingGrid, Recipe.get(), a_GridStride, a_GridWidth, a_GridHeight);

	return Recipe.release();
}





void cCraftingRecipes::HandleFireworks(const cItem * a_CraftingGrid, cCraftingRecipes::cRecipe * a_Recipe, int a_GridStride, int a_OffsetX, int a_OffsetY)
{
	// TODO: add support for more than one dye in the recipe
	// A manual and temporary solution (listing everything) is in crafting.txt for fade colours, but a programmatic solutions needs to be done for everything else

	if (a_Recipe->m_Result.m_ItemType == E_ITEM_FIREWORK_ROCKET)
	{
		for (cRecipeSlots::const_iterator itr = a_Recipe->m_Ingredients.begin(); itr != a_Recipe->m_Ingredients.end(); ++itr)
		{
			switch (itr->m_Item.m_ItemType)
			{
				case E_ITEM_FIREWORK_STAR:
				{
					// Result was a rocket, found a star - copy star data to rocket data
					int GridID = (itr->x + a_OffsetX) + a_GridStride * (itr->y + a_OffsetY);
					a_Recipe->m_Result.m_FireworkItem.CopyFrom(a_CraftingGrid[GridID].m_FireworkItem);
					break;
				}
				case E_ITEM_GUNPOWDER:
				{
					// Gunpowder - increase flight time
					a_Recipe->m_Result.m_FireworkItem.m_FlightTimeInTicks += 20;
					break;
				}
				case E_ITEM_PAPER: break;
				default: LOG("Unexpected item in firework rocket recipe, was the crafting file's fireworks section changed?"); break;
			}
		}
	}
	else if (a_Recipe->m_Result.m_ItemType == E_ITEM_FIREWORK_STAR)
	{
		std::vector<int> DyeColours;
		bool FoundStar = false;

		for (cRecipeSlots::const_iterator itr = a_Recipe->m_Ingredients.begin(); itr != a_Recipe->m_Ingredients.end(); ++itr)
		{
			switch (itr->m_Item.m_ItemType)
			{
				case E_ITEM_FIREWORK_STAR:
				{
					// Result was star, found another star - probably adding fade colours, but copy data over anyhow
					FoundStar = true;
					int GridID = (itr->x + a_OffsetX) + a_GridStride * (itr->y + a_OffsetY);
					a_Recipe->m_Result.m_FireworkItem.CopyFrom(a_CraftingGrid[GridID].m_FireworkItem);
					break;
				}
				case E_ITEM_DYE:
				{
					int GridID = (itr->x + a_OffsetX) + a_GridStride * (itr->y + a_OffsetY);
					DyeColours.push_back(cFireworkItem::GetVanillaColourCodeFromDye(static_cast<NIBBLETYPE>(a_CraftingGrid[GridID].m_ItemDamage & 0x0f)));
					break;
				}
				case E_ITEM_GUNPOWDER: break;
				case E_ITEM_DIAMOND: a_Recipe->m_Result.m_FireworkItem.m_HasTrail = true; break;
				case E_ITEM_GLOWSTONE_DUST: a_Recipe->m_Result.m_FireworkItem.m_HasFlicker = true; break;

				case E_ITEM_FIRE_CHARGE: a_Recipe->m_Result.m_FireworkItem.m_Type = 1; break;
				case E_ITEM_GOLD_NUGGET: a_Recipe->m_Result.m_FireworkItem.m_Type = 2; break;
				case E_ITEM_FEATHER: a_Recipe->m_Result.m_FireworkItem.m_Type = 4; break;
				case E_ITEM_HEAD: a_Recipe->m_Result.m_FireworkItem.m_Type = 3; break;
				default: LOG("Unexpected item in firework star recipe, was the crafting file's fireworks section changed?"); break;  // ermahgerd BARD ardmins
			}
		}

		if (FoundStar && (!DyeColours.empty()))
		{
			// Found a star and a dye? Fade colours.
			a_Recipe->m_Result.m_FireworkItem.m_FadeColours = DyeColours;
		}
		else if (!DyeColours.empty())
		{
			// Only dye? Normal colours.
			a_Recipe->m_Result.m_FireworkItem.m_Colours = DyeColours;
		}
	}
}





void cCraftingRecipes::HandleDyedLeather(const cItem * a_CraftingGrid, cCraftingRecipes::cRecipe * a_Recipe, int a_GridStride, int a_GridWidth, int a_GridHeight)
{
	short result_type = a_Recipe->m_Result.m_ItemType;
	if ((result_type == E_ITEM_LEATHER_CAP) || (result_type == E_ITEM_LEATHER_TUNIC) || (result_type == E_ITEM_LEATHER_PANTS) || (result_type == E_ITEM_LEATHER_BOOTS))
	{
		bool found = false;
		cItem temp;

		float red = 0;
		float green = 0;
		float blue = 0;
		float dye_count = 0;

		for (int x = 0; x < a_GridWidth; ++x)
		{
			for (int y = 0; y < a_GridHeight; ++y)
			{
				int GridIdx = x + a_GridStride * y;
				if ((a_CraftingGrid[GridIdx].m_ItemType == result_type) && !found)
				{
					found = true;
					temp = a_CraftingGrid[GridIdx].CopyOne();
					// The original color of the item affects the result
					if (temp.m_ItemColor.IsValid())
					{
						red += temp.m_ItemColor.GetRed();
						green += temp.m_ItemColor.GetGreen();
						blue += temp.m_ItemColor.GetBlue();
						++dye_count;
					}
				}
				else if (a_CraftingGrid[GridIdx].m_ItemType == E_ITEM_DYE)
				{
					switch (a_CraftingGrid[GridIdx].m_ItemDamage)
					{
						case E_META_DYE_BLACK:
						{
							red += 23;
							green += 23;
							blue += 23;
							break;
						}
						case E_META_DYE_RED:
						{
							red += 142;
							green += 47;
							blue += 47;
							break;
						}
						case E_META_DYE_GREEN:
						{
							red += 95;
							green += 118;
							blue += 47;
							break;
						}
						case E_META_DYE_BROWN:
						{
							red += 95;
							green += 71;
							blue += 47;
							break;
						}
						case E_META_DYE_BLUE:
						{
							red += 47;
							green += 71;
							blue += 165;
							break;
						}
						case E_META_DYE_PURPLE:
						{
							red += 118;
							green += 59;
							blue += 165;
							break;
						}
						case E_META_DYE_CYAN:
						{
							red += 71;
							green += 118;
							blue += 142;
							break;
						}
						case E_META_DYE_LIGHTGRAY:
						{
							red += 142;
							green += 142;
							blue += 142;
							break;
						}
						case E_META_DYE_GRAY:
						{
							red += 71;
							green += 71;
							blue += 71;
							break;
						}
						case E_META_DYE_PINK:
						{
							red += 225;
							green += 118;
							blue += 153;
							break;
						}
						case E_META_DYE_LIGHTGREEN:
						{
							red += 118;
							green += 190;
							blue += 23;
							break;
						}
						case E_META_DYE_YELLOW:
						{
							red += 213;
							green += 213;
							blue += 47;
							break;
						}
						case E_META_DYE_LIGHTBLUE:
						{
							red += 95;
							green += 142;
							blue += 201;
							break;
						}
						case E_META_DYE_MAGENTA:
						{
							red += 165;
							green += 71;
							blue += 201;
							break;
						}
						case E_META_DYE_ORANGE:
						{
							red += 201;
							green += 118;
							blue += 47;
							break;
						}
						case E_META_DYE_WHITE:
						{
							red += 237;
							green += 237;
							blue += 237;
							break;
						}
					}
					++dye_count;
				}
				else if (a_CraftingGrid[GridIdx].m_ItemType != E_ITEM_EMPTY)
				{
					return;
				}
			}
		}

		if (!found)
		{
			return;
		}

		// Calculate the rgb values
		double maximum = static_cast<double>(std::max({red, green, blue}));

		double average_red = red / dye_count;
		double average_green = green / dye_count;
		double average_blue = blue / dye_count;
		double average_max = maximum / dye_count;

		double max_average = std::max({average_red, average_green, average_blue});

		double gain_factor = average_max / max_average;


		unsigned char result_red = static_cast<unsigned char>(average_red * gain_factor);
		unsigned char result_green = static_cast<unsigned char>(average_green * gain_factor);
		unsigned char result_blue = static_cast<unsigned char>(average_blue * gain_factor);

		// Set the results values
		a_Recipe->m_Result = temp;
		a_Recipe->m_Result.m_ItemColor.SetColor(result_red, result_green, result_blue);
	}
}
