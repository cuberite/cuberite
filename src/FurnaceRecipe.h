
#pragma once





class cItem;





class cFurnaceRecipe
{
public:
	cFurnaceRecipe(void);
	~cFurnaceRecipe();

	void ReloadRecipes(void);

	struct Fuel
	{
		cItem * In;
		int BurnTime;  ///< How long this fuel burns, in ticks
	};

	struct Recipe
	{
		cItem * In;
		cItem * Out;
		float OutExp;
		int CookTime;  ///< How long this recipe takes to smelt, in ticks
	};
	
	/// Returns a recipe for the specified input, NULL if no recipe found
	const Recipe * GetRecipeFrom(const cItem & a_Ingredient) const;
	
	/// Returns the amount of time that the specified fuel burns, in ticks
	int GetBurnTime(const cItem & a_Fuel) const;

private:
	void ClearRecipes(void);

	struct sFurnaceRecipeState;
	sFurnaceRecipeState * m_pState;
};




