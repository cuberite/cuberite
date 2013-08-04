
// LuaState.h

// Declares the cLuaState class representing the wrapper over lua_State *, provides associated helper functions





#pragma once





// fwd: lua.h
struct lua_State;





class cLuaState
{
public:
	/** Creates a new instance. The LuaState is not initialized.
	a_SubsystemName is used for reporting problems in the console, it is "plugin %s" for plugins, 
	or "LuaScript" for the cLuaScript template
	*/
	cLuaState(const AString & a_SubsystemName);
	
	~cLuaState();
	
	operator lua_State * (void) { return m_LuaState; }
	
	/// Creates the m_LuaState, if not closed already
	void Create(void);
	
	/// Closes the m_LuaState, if not closed already
	void Close(void);
	
	/// Returns true if the m_LuaState is valid
	bool IsValid(void) const { return (m_LuaState != NULL); }
	
	/** Loads the specified file
	Returns false and logs a warning to the console if not successful (but the LuaState is kept open).
	m_SubsystemName is displayed in the warning log message.
	*/
	bool LoadFile(const AString & a_FileName);
	
	/** Pushes the function of the specified name onto the stack.
	Returns true if successful.
	If a_ShouldLogFail is true, logs a warning on failure (incl. m_SubsystemName)
	*/
	bool PushFunction(const char * a_FunctionName, bool a_ShouldLogFailure = true);
	
	/** Pushes a function that has been saved into the global registry, identified by a_FnRef.
	Returns true if successful. Logs a warning on failure
	*/
	bool PushFunctionFromRegistry(int a_FnRef);
	
	/// Pushes a string vector, as a table, onto the stack
	void PushStringVector(const AStringVector & a_Vector);
	
	/**
	Calls the function that has been pushed onto the stack by PushFunction.
	Returns true if successful, logs a warning on failure.
	a_FunctionName is used only for the warning log message, the function
	to be called must be pushed by PushFunction() beforehand.
	*/
	bool CallFunction(int a_NumArgs, int a_NumResults, const char * a_FunctionName);

	/// If the status is nonzero, prints the text on the top of Lua stack and returns true
	bool ReportErrors(int status);
	
	/// If the status is nonzero, prints the text on the top of Lua stack and returns true
	static bool ReportErrors(lua_State * a_LuaState, int status);
	
protected:
	lua_State * m_LuaState;
	
	/** The subsystem name is used for reporting errors to the console, it is either "plugin %s" or "LuaScript"
	whatever is given to the constructor
	*/
	AString m_SubsystemName;
} ;




