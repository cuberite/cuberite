#pragma once

#include <string>

struct lua_State;
struct HTTPRequest;
//tolua_begin
class cWebPlugin
{
public:
	cWebPlugin( lua_State* L );
	virtual ~cWebPlugin();
	
	void SetName( std::string a_Name ) { m_Name = a_Name; }
	std::string GetName() { return m_Name; }

	virtual std::string HandleRequest( HTTPRequest* a_Request ) = 0;
	virtual void Initialize() = 0;
	//tolua_end

	lua_State* GetLuaState() { return m_LuaState; }
private:
	lua_State* m_LuaState;
	std::string m_Name;
}; //tolua_export