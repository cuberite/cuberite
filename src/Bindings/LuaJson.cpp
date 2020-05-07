
// LuaJson.cpp

// Implements the Json exposure bindings to Lua

#include "Globals.h"
#include <sstream>
#include "LuaJson.h"
#include "LuaState.h"
#include "tolua++/include/tolua++.h"
#include "json/json.h"
#include "../JsonUtils.h"





// fwd:

static void PushJsonValue(const Json::Value & a_Value, cLuaState & a_LuaState);
static Json::Value JsonSerializeValue(cLuaState & a_LuaState);




/** Exception thrown when the input cannot be serialized.
Keeps track of the error message and the problematic value's path in the table.
*/
class CannotSerializeException:
	public std::runtime_error
{
	using Super = std::runtime_error;

public:
	/** Constructs a new instance of the exception based on the provided values directly. */
	explicit CannotSerializeException(const AString & a_ValueName, const char * a_ErrorMsg):
		Super(a_ErrorMsg),
		m_ValueName(a_ValueName)
	{
	}

	/** Constructs a new instance of the exception based on the provided values directly. */
	explicit CannotSerializeException(int a_ValueIndex, const char * a_ErrorMsg):
		Super(a_ErrorMsg),
		m_ValueName(Printf("%d", a_ValueIndex))
	{
	}

	/** Constructs a new instance of the exception that takes the error message and value name from the parent, and prefix the value name with the specified prefix.
	Used for prefixing the value name's path along the call stack that lead to the main exception. */
	explicit CannotSerializeException(const CannotSerializeException & a_Parent, const AString & a_ValueNamePrefix):
		Super(a_Parent.what()),
		m_ValueName(a_ValueNamePrefix + "." + a_Parent.m_ValueName)
	{
	}

	/** Constructs a new instance of the exception that takes the error message and value name from the parent, and prefix the value name with the specified prefix.
	Used for prefixing the value name's path along the call stack that lead to the main exception. */
	explicit CannotSerializeException(const CannotSerializeException & a_Parent, int a_ValueNamePrefixIndex):
		Super(a_Parent.what()),
		m_ValueName(Printf("%d", a_ValueNamePrefixIndex) + "." + a_Parent.m_ValueName)
	{
	}

	const AString & GetValueName() const { return m_ValueName; }

protected:
	AString m_ValueName;
};





/** Pushes the specified Json array as a table on top of the specified Lua state.
Assumes that a_Value is an array. */
static void PushJsonArray(const Json::Value & a_Value, cLuaState & a_LuaState)
{
	// Create the appropriately-sized Lua table:
	lua_createtable(a_LuaState, static_cast<int>(a_Value.size()), 0);

	// Insert each value to the appropriate index (1-based):
	int idx = 1;
	for (const auto & v: a_Value)
	{
		// Include Json null values in the array - it will have holes, but indices will stay the same
		PushJsonValue(v, a_LuaState);
		lua_rawseti(a_LuaState, -2, idx);
		idx += 1;
	}  // for v: a_Value[]
}





/** Pushes the specified Json object as a table on top of the specified Lua state.
Assumes that a_Value is an object. */
static void PushJsonObject(const Json::Value & a_Value, cLuaState & a_LuaState)
{
	// Create the appropriately-sized Lua table:
	lua_createtable(a_LuaState, 0, static_cast<int>(a_Value.size()));

	// Json::Value has no means of iterating over children with their names included.
	// We need to iterate over names and "find" them in the object again:
	auto names = a_Value.getMemberNames();
	for (const auto & n: names)
	{
		auto v = a_Value[n];
		if (v.isNull())
		{
			// Skip null values
			continue;
		}

		// Set the value in Lua's table:
		a_LuaState.Push(n);
		PushJsonValue(v, a_LuaState);
		lua_rawset(a_LuaState, -3);
	}  // for itr - a_Value[]
}





/** Pushes the specified Json value as an appropriate type on top of the specified Lua state. */
void PushJsonValue(const Json::Value & a_Value, cLuaState & a_LuaState)
{
	switch (a_Value.type())
	{
		case Json::nullValue:
		{
			a_LuaState.Push(cLuaState::Nil);
			break;
		}

		case Json::intValue:
		case Json::uintValue:
		case Json::realValue:
		{
			a_LuaState.Push(static_cast<lua_Number>(a_Value.asDouble()));
			break;
		}

		case Json::booleanValue:
		{
			a_LuaState.Push(a_Value.asBool());
			break;
		}

		case Json::stringValue:
		{
			a_LuaState.Push(a_Value.asString());
			break;
		}

		case Json::arrayValue:
		{
			PushJsonArray(a_Value, a_LuaState);
			break;
		}

		case Json::objectValue:
		{
			PushJsonObject(a_Value, a_LuaState);
			break;
		}
	}  // switch (v.type())
}





/** Serializes the Lua table at the top of the specified Lua state's stack into a Json value.
Lets jsoncpp decides whether to serialize into an object or an array. */
static Json::Value JsonSerializeTable(cLuaState & a_LuaState)
{
	Json::Value res;
	lua_pushnil(a_LuaState);
	while (lua_next(a_LuaState, -2) != 0)
	{
		if (lua_type(a_LuaState, -2) == LUA_TNUMBER)
		{
			int idx;
			a_LuaState.GetStackValue(-2, idx);
			try
			{
				res[idx - 1] = JsonSerializeValue(a_LuaState);
			}
			catch (const CannotSerializeException & exc)
			{
				throw CannotSerializeException(exc, idx);
			}
			catch (const std::exception & exc)  // Cannot catch Json::Exception, because it's not properly defined
			{
				throw CannotSerializeException(idx, exc.what());
			}
		}
		else
		{
			AString name;
			if (a_LuaState.GetStackValue(-2, name))
			{
				try
				{
					res[name] = JsonSerializeValue(a_LuaState);
				}
				catch (const CannotSerializeException & exc)
				{
					throw CannotSerializeException(exc, name);
				}
				catch (const std::exception & exc)  // Cannot catch Json::Exception, because it's not properly defined
				{
					throw CannotSerializeException(name, exc.what());
				}
			}
		}
		lua_pop(a_LuaState, 1);
	}
	return res;
}





/** Serializes the Lua value at the top of the specified Lua state into a Json value. */
static Json::Value JsonSerializeValue(cLuaState & a_LuaState)
{
	switch (lua_type(a_LuaState, -1))
	{
		case LUA_TBOOLEAN:
		{
			bool v;
			a_LuaState.GetStackValue(-1, v);
			return Json::Value(v);
		}
		case LUA_TNIL:
		{
			return Json::Value(Json::nullValue);
		}
		case LUA_TNUMBER:
		{
			lua_Number v;
			a_LuaState.GetStackValue(-1, v);
			return Json::Value(v);
		}
		case LUA_TSTRING:
		{
			AString v;
			a_LuaState.GetStackValue(-1, v);
			return Json::Value(v);
		}
		case LUA_TTABLE:
		{
			return JsonSerializeTable(a_LuaState);
		}
		default:
		{
			LOGD("Attempting to serialize an unhandled Lua value type: %d", lua_type(a_LuaState, -1));
			return Json::Value(Json::nullValue);
		}
	}
}





static int tolua_cJson_Parse(lua_State * a_LuaState)
{
	// Function signature:
	// cJson:Parse("string") -> table

	// Check the param types:
	cLuaState L(a_LuaState);
	if (
		!L.CheckParamUserTable(1, "cJson") ||
		!L.CheckParamString(2) ||
		!L.CheckParamEnd(3)
	)
	{
		return 0;
	}

	// Get the input string:
	AString input;
	if (!L.GetStackValue(2, input))
	{
		LOGWARNING("cJson:Parse(): Cannot read input string");
		L.LogStackTrace();
		return 0;
	}

	// Parse the string:
	Json::Value root;
	AString ParseError;
	if (!JsonUtils::ParseString(input, root, &ParseError))
	{
		L.Push(cLuaState::Nil, Printf("Parsing Json failed: %s", ParseError));
		return 2;
	}

	// Push the Json value onto Lua stack:
	PushJsonValue(root, L);
	return 1;
}





static int tolua_cJson_Serialize(lua_State * a_LuaState)
{
	// Function signature:
	// cJson:Serialize({table}, [{option1 = value1, option2 = value2}]) -> string

	// Check the param types:
	cLuaState L(a_LuaState);
	if (
		!L.CheckParamUserTable(1, "cJson") ||
		!L.CheckParamTable(2) ||
		!L.CheckParamEnd(4)
	)
	{
		return 0;
	}

	// Push the table to the top of the Lua stack, and call the serializing function:
	lua_pushvalue(L, 2);
	Json::Value root;
	try
	{
		root = JsonSerializeValue(L);
	}
	catch (const CannotSerializeException & exc)
	{
		lua_pushnil(L);
		L.Push(Printf("Cannot serialize into Json, value \"%s\" caused an error \"%s\"", exc.GetValueName().c_str(), exc.what()));
		return 2;
	}
	lua_pop(L, 1);

	// Create the writer, with all properties (optional param 3) applied to it:
	Json::StreamWriterBuilder builder;
	if (lua_istable(L, 3))
	{
		lua_pushnil(L);
		while (lua_next(L, -2) != 0)
		{
			if (lua_type(L, -2) == LUA_TSTRING)
			{
				AString propName, propValue;
				if (L.GetStackValues(-2, propName, propValue))
				{
					builder[propName] = propValue;
				}
			}
			lua_pop(L, 1);
		}
		// Check for invalid settings:
		Json::Value invalid;
		if (!builder.validate(&invalid))
		{
			LOGINFO("cJson:Serialize(): detected invalid settings:");
			for (const auto & n: invalid.getMemberNames())
			{
				LOGINFO("  \"%s\" (\"%s\")", n.c_str(), invalid[n].asCString());
			}
		}
	}
	auto writer(builder.newStreamWriter());

	// Serialize the string and push it as the return value:
	std::stringstream ss;
	writer->write(root, &ss);
	L.Push(ss.str());
	return 1;
}





void cLuaJson::Bind(cLuaState & a_LuaState)
{
	tolua_beginmodule(a_LuaState, nullptr);

		// Create the cJson API class:
		tolua_usertype(a_LuaState, "cJson");
		tolua_cclass(a_LuaState, "cJson", "cJson", "", nullptr);

		// Fill in the functions (alpha-sorted):
		tolua_beginmodule(a_LuaState, "cJson");
			tolua_function(a_LuaState, "Parse",     tolua_cJson_Parse);
			tolua_function(a_LuaState, "Serialize", tolua_cJson_Serialize);
		tolua_endmodule(a_LuaState);
	tolua_endmodule(a_LuaState);
}




