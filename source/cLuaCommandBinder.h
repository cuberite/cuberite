#pragma once

#include <vector>
#include <string>
#include <map>

struct lua_State;
class cPlugin;
class cPlayer;
class cLuaCommandBinder
{
public:
	cLuaCommandBinder();
	~cLuaCommandBinder();

	bool HandleCommand( const std::string & a_Command, cPlayer* a_Player );

	bool BindCommand( const std::string & a_Command, const std::string & a_Permission, cPlugin* a_Plugin, lua_State * a_LuaState, int a_FunctionReference );

	void ClearBindings();
	void RemoveBindingsForPlugin( cPlugin* a_Plugin );
private:
	struct BoundFunction
	{
		BoundFunction() : Plugin( 0 ), LuaState( 0 ), Reference( 0 ) {}
		BoundFunction( cPlugin* a_Plugin, lua_State * a_LuaState, int a_Reference, const std::string & a_Permission ) : Plugin( a_Plugin ), LuaState( a_LuaState ), Reference( a_Reference ), Permission( a_Permission ) {}
		cPlugin* Plugin;
		lua_State* LuaState;
		int Reference;
		std::string Permission;
	};

	typedef std::map< std::string, BoundFunction > CommandMap;
	CommandMap m_BoundCommands;

};
