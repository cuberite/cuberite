/**
 * Copyright 2014 MCServer Contributors
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

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




