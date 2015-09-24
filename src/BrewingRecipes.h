
#pragma once





class cItem;





class cBrewingRecipes
{
public:
	cBrewingRecipes(void);
	~cBrewingRecipes();

	void ReloadRecipes(void);

	struct cRecipe
	{
		cRecipe() {}
		cRecipe(cRecipe &&) {}

		cRecipe(const cRecipe&) = delete;
		cRecipe & operator=(const cRecipe&) = delete;

		std::unique_ptr<cItem> Input;
		std::unique_ptr<cItem> Output;
		std::unique_ptr<cItem> Ingredient;
	};

	/** Returns a recipe for the specified input, nullptr if no recipe found */
	const cRecipe * GetRecipeFrom(const cItem & a_Input, const cItem & a_Ingredient) const;

	/** Returns true if the item is a ingredient, false if not. */
	bool IsIngredient(const cItem & a_Ingredient) const;

	/** Returns true if the item is a bottle / potion, false if not. */
	bool IsBottle(const cItem & a_Bottle) const;
private:
	void ClearRecipes(void);

	/** Parses the recipe contained in the line, adds it to m_pState's recipes.
	Logs a warning to the console on input error. */
	void AddRecipeFromLine(const AString & a_Line, unsigned int a_LineNum);

	/** Parses an item string, returns true if successful. */
	bool ParseItem(const AString & a_String, cItem & a_Item);

	struct sBrewingRecipeState;
	std::unique_ptr<sBrewingRecipeState> m_pState;
};
