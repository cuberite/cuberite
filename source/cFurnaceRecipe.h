#pragma once

#include <list>

class cItem;
class cFurnaceRecipe
{
public:
	cFurnaceRecipe();
	~cFurnaceRecipe();

	void ReloadRecipes();

	struct Fuel
	{
		cItem* In;
		float BurnTime;
	};

	struct Recipe
	{
		cItem* In;
		cItem* Out;
		float CookTime;
	};
	const Recipe* GetRecipeFrom( const cItem & a_Ingredient ) const;
	float GetBurnTime( const cItem & a_Fuel ) const;

private:
	void ClearRecipes();

	struct sFurnaceRecipeState;
	sFurnaceRecipeState* m_pState;
};