
// LuaScript.h

// Declares the cLuaScript class that loads a Lua script file to produce a web template out of it





#pragma once

#include "LuaState.h"





/*
struct sLuaUsertype
{
	sLuaUsertype(void* a_pObject, const char* a_pClassName) : Object(a_pObject), ClassName(a_pClassName) {}
	//
	void*       Object;
	const char* ClassName;
} ;
*/





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




