#ifndef TOLUA_BASE_H
#define TOLUA_BASE_H

#pragma warning(disable:4800) // This file is ONLY included by Bindings.cpp and it throws lots of C4800 warnings

#include "tolua++.h"





class ToluaBase {

	int lua_instance;

protected:

	lua_State* lua_state;

	void lua_stacktrace(lua_State* L) const
	{
		lua_Debug entry;
		int depth = 0;

		while (lua_getstack(L, depth, &entry))
		{
			lua_getinfo(L, "Sln", &entry);

			LOGERROR("%s(%d): %s", entry.short_src, entry.currentline, entry.name ? entry.name : "?");
			depth++;
		}
	}


	bool report_errors(int status) const
	{
		if ( status!=0 )
		{
			const char* s = lua_tostring(lua_state, -1);
			LOGERROR("-- %s", s );
			//lua_pop(lua_state, 1);
			LOGERROR("Stack:");
			lua_stacktrace( lua_state );
			return true;
		}
		return false;
	}

	bool push_method(const char* name, lua_CFunction f) const {

		if (!lua_state) return false;

		lua_getref(lua_state, lua_instance);
		lua_pushstring(lua_state, name);
		//LOGINFO("1. push_method() Stack size: %i", lua_gettop( lua_state ) );
		lua_gettable(lua_state, -2);
		//LOGINFO("2. push_method() Stack size: %i", lua_gettop( lua_state ) );

		if (lua_isnil(lua_state, -1)) {

			// pop the table
			lua_pop(lua_state, 2);
			return false;

		} else {

			if (f) {
				if (lua_iscfunction(lua_state, -1)) {
					lua_pop(lua_state, 2);
					return false;
				};
				/* // not for now
				lua_pushcfunction(lua_state, f);
				if (lua_rawequal(lua_state, -1, -2)) {

					// avoid recursion, pop both functions and the table
					lua_pop(lua_state, 3);
					return false;
				};

				// pop f
				lua_pop(lua_state, 1);
				*/
			};

			// swap table with function
			lua_insert(lua_state, -2);
		};

		return true;
	};

	void dbcall(lua_State* L, int nargs, int nresults) const {

		// using lua_call for now
		int s = lua_pcall(L, nargs, nresults, 0);
		report_errors( s );
	};
public:
	
	int GetInstance() { return lua_instance; }
	lua_State* GetLuaState() { return lua_state; }

	void tolua__set_instance(lua_State* L, lua_Object lo) {

		lua_state = L;

		lua_pushvalue(L, lo);
		lua_instance = lua_ref(lua_state, 1);
	};

	ToluaBase() {

		lua_state = NULL;
	};

	~ToluaBase() {

		if (lua_state) {

			lua_unref(lua_state, lua_instance);
		};
	};
};

#endif


