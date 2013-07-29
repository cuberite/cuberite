
// LuaScript.h

// Declares the cLuaScript class that loads a Lua script file to produce a web template out of it





#pragma once

struct lua_State;





struct sLuaUsertype
{
	sLuaUsertype(void* a_pObject, const char* a_pClassName) : Object(a_pObject), ClassName(a_pClassName) {}
	//
	void*       Object;
	const char* ClassName;
} ;





class cLuaScript
{
public:
	cLuaScript();
	~cLuaScript();

	/// Prepares a Lua state
	void        Initialize();

	/// Load a Lua script on the given path
	bool        LoadFile(const char* a_FilePath);

	/// Execute the loaded Lua script
	bool        Execute();

	/// Call a function on the Lua script. Put all overloads here
	bool        CallFunction(const char* a_Function, AString& ReturnedString);
	bool        CallFunction(const char* a_Function, const sLuaUsertype& a_UserType, AString& ReturnedString);
	bool        CallFunction(const char* a_Function, const sLuaUsertype& a_UserType1, const sLuaUsertype& a_UserType2, AString& ReturnedString);

protected:
	/// Reports an error in the log if a_Status is flagged as an error. Returns true when a_Status is flagged as error, returns false when no error occured.
	bool        ReportErrors(int a_Status);

	/// Helper functions for calling functions in Lua 
	bool        LuaPushFunction(const char * a_FunctionName, bool a_bLogError = true);
	bool        LuaCallFunction(int a_NumArgs, int a_NumResults, const char * a_FunctionName );  // a_FunctionName is only used for error messages, nothing else
private:
	lua_State*  m_LuaState;
} ;




