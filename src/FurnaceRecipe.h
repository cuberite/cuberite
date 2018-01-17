
#pragma once





class cItem;





class cFurnaceRecipe
{
public:
	cFurnaceRecipe(void);
	~cFurnaceRecipe();

	void ReloadRecipes(void);

	struct cFuel
	{
		cItem * In;
		int BurnTime;  ///< How long this fuel burns, in ticks
	};

	struct cRecipe
	{
		cItem * In;
		cItem * Out;
		int CookTime;  ///< How long this recipe takes to smelt, in ticks
	};

	/** Returns a recipe for the specified input, nullptr if no recipe found */
	const cRecipe * GetRecipeFrom(const cItem & a_Ingredient) const;

	/** Returns true if the item is a fuel, false if not. */
	bool IsFuel(const cItem & a_Item) const;

	/** Returns the amount of time that the specified fuel burns, in ticks */
	int GetBurnTime(const cItem & a_Fuel) const;

private:
	void ClearRecipes(void);

	/** Parses the fuel contained in the line, adds it to m_pState's fuels.
	Logs a warning to the console on input error. */
	void AddFuelFromLine(const AString & a_Line, unsigned int a_LineNum);

	/** Parses the recipe contained in the line, adds it to m_pState's recipes.
	Logs a warning to the console on input error. */
	void AddRecipeFromLine(const AString & a_Line, unsigned int a_LineNum);

	/** Parses an item string in the format "<ItemType>[: <Damage>][, <Amount>]", returns true if successful. */
	bool ParseItem(const AString & a_String, cItem & a_Item);

	struct sFurnaceRecipeState;
	sFurnaceRecipeState * m_pState;
};




