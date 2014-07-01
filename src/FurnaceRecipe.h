
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

	/** Parses the fuel contained in the line, adds it to m_pState's fuels.
	Logs a warning to the console on input error. */
	void AddFuelFromLine(const AString & a_Line, int a_LineNum);

	/** Parses the recipe contained in the line, adds it to m_pState's recipes.
	Logs a warning to the console on input error. */
	void AddRecipeFromLine(const AString & a_Line, int a_LineNum);

	/** Calls LOGWARN with the line, position, and error */
	static void PrintParseError(unsigned int a_Line, size_t a_Position, const AString & a_CharactersMissing);

	/** Reads a number from a string given, starting at a given position and ending at a delimiter given
	Updates beginning position to the delimiter found + 1, and updates the value to the one read
	If it encounters a substring that is not fully numeric, it will call SetParseError() and return false; the caller should abort processing
	Otherwise, the function will return true
	*/
	static bool ReadMandatoryNumber(AString::size_type & a_Begin, const AString & a_Delimiter, const AString & a_Text, unsigned int a_Line, int & a_Value, bool a_IsLastValue = false);

	/** Reads two numbers from a string given, starting at a given position and ending at the first delimiter given, then again (with an updated position) until the second delimiter given
	Updates beginning position to the second delimiter found + 1, and updates the values to the ones read
	If it encounters a substring that is not fully numeric whilst reading the second value, it will call SetParseError() and return false; the caller should abort processing
	If this happens whilst reading the first value, it will call ReadMandatoryNumber() with the appropriate position, as this may legitimately occur with the optional value and AString::find_first_of finding the incorrect delimiter. It will return the result of ReadMandatoryNumber()
	True will be returned definitively for an optional value that is valid
	*/
	static bool ReadOptionalNumbers(AString::size_type & a_Begin, const AString & a_DelimiterOne, const AString & a_DelimiterTwo, const AString & a_Text, unsigned int a_Line, int & a_ValueOne, int & a_ValueTwo, bool a_IsLastValue = false);

	/** Uses std::all_of to determine if a string contains only digits */
	static bool DoesStringContainOnlyNumbers(const AString & a_String);


	struct sFurnaceRecipeState;
	sFurnaceRecipeState * m_pState;
};




