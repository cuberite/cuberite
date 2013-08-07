
// LuaScript.h

// Declares the cLuaScript class that loads a Lua script file to produce a web template out of it





#pragma once

#include "LuaState.h"





// fwd:
class cWebAdmin;
struct HTTPTemplateRequest;





class cLuaScript
{
public:
	cLuaScript(void);

	/// Prepares a Lua state
	void Initialize();

	/// Load a Lua script on the given path
	bool LoadFile(const char * a_FilePath);

	bool CallShowPage(cWebAdmin & a_WebAdmin, HTTPTemplateRequest & a_Request, AString & a_ReturnedString);

protected:
	cLuaState m_LuaState;
} ;




