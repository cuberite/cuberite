// LuaStateParams.h

// Declares the cLuaStateParams class that provides high-level functions for reading API parameters from Lua state

#pragma once





#include "LuaState.h"
#include "tolua++/include/tolua++.h"





namespace Detail
{
	/** Utility struct that provides the functionality to cLuaStateParams::GetTypeDescription.
	Supports decorators such as cLuaStateParams::cSelf<T>.
	Must be a struct in order to support decorators, and must not be inside a class (gcc / clang complain about that).
	The general declaration must not be used by the code, all types need a specialization returning the correct type. */
	template <typename T>
	struct TypeDescription
	{
		static const char * desc()
		{
			static_assert(false, "Type is missing a TypeDescription<> template");
			/* If compiler complains on the previous line, you need to make sure that the type
			passed as the template parameter to this structure has an appropriate TypeDescription specialization.
			Usually these are created automatically for all API classes by ToLua++ in the LuaStateParams_TypeDescs.inc file.
			For basic types, the specializations are below.
			*/
		}
	};

	// Specializations are defined after the cLuaStateParams class declaration
}





/** A namespace-like class for reading parameters to API functions from Lua, using templates.
The Read() function is the main entrypoint for this, and does all the work.
Example usage:
int Param1, Param2, Param3;
AString ParamA, ParamB;
cItem * Item;
switch (cLuaStateParams::Read(LuaState,
	std::tie(cLuaStateParams::self(Item), Param1, Param2, Param3),
	std::tie(cLuaStateParams::self(Item), ParamA, ParamB)
))
{
	case 0:
	{
		// The first overload was used; Item, Param1, Param2 and Param3 are valid
		Item->DoSomething(Param1, Param2, Param3);
		break;
	}
	case 1:
	{
		// The second overload was used; Item, ParamA and ParamB are valid
		Item->DoSomething(ParamA, ParamB);
		break;
	}
	// No need for default: if no overloads match, the function raises a Lua error, returning to the Lua's caller
}
*/
class cLuaStateParams
{
public:
	/** A wrapper used in cLuaStateParams::Read() to signalize that the value must be a non-nil pointer when read from Lua. */
	template <typename T>
	class cNonNil
	{
		friend class cLuaStateParams;
		typedef T * Tptr;
	public:
		typedef T ParentType;

		cNonNil(Tptr & a_Dest):
			m_Dest(a_Dest)
		{
		}

	protected:
		/** The destination pointer where the param will be written. */
		Tptr & m_Dest;
	};
	template <typename T> static cNonNil<T> nonNil(T * & a_Dest) { return cNonNil<T>(a_Dest); }


	/** A wrapper used in cLuaStateParams::Read() to signalize that the value is a "self" - it must be a non-nil pointer when read from Lua,
	and has a special error message when mismatched. */
	template <typename T>
	class cSelf
	{
		friend class cLuaStateParams;
		typedef T * Tptr;
	public:
		typedef T ParentType;

		cSelf(Tptr & a_Self):
			m_Self(a_Self)
		{
		}

	protected:
		Tptr & m_Self;
	};
	template <typename T> static cSelf<T> self(T * & a_Self) { return cSelf<T>(a_Self); }


	/** A wrapper used in cLuaState::ReadParams() to signalize that the value is a "static self",
	it must be the table representing the class T when read from Lua. */
	template <class T>
	class cStaticSelf
	{
	public:
		typedef T ParentType;
	};
	template <class T> static cStaticSelf<T> staticSelf() { return cStaticSelf<T>(); }



protected:

	/** Attempts to match the params on the Lua stack to the API function overloads given (recursively).
	a_CurOverload is the index of the currently processed overload (recursion level),
	Returns the index of the overload that matches the parameters.
	If no overloads match, returns -1. */
	template <typename... T1, typename... T2>
	static int ReadInternal(cLuaState & a_LuaState, int a_CurOverload, const std::tuple<T1...> & a_Ovl, T2 &&... a_OtherParams)
	{
		// Try to read this overload
		if (ReadSingleOverload(a_LuaState, a_Ovl))
		{
			return a_CurOverload;
		}

		// Try the next overload:
		return ReadInternal(a_LuaState, a_CurOverload + 1, std::forward<T2>(a_OtherParams)...);
	}


	/** Terminator for the template-based recursion of the function above - for a single overload. */
	template <typename... T1>
	static int ReadInternal(cLuaState & a_LuaState, int a_CurOverload, const std::tuple<T1...> & a_Ovl)
	{
		// Try to read this overload
		if (ReadSingleOverload(a_LuaState, a_Ovl))
		{
			return a_CurOverload;
		}

		// No more overloads, report failure
		return -1;
	}



	/** Helper struct to implement iterating over std::tuple elements */
	template <size_t N> struct SizeT {};



	/** Attempts to match the params on the Lua stack to the given API function overload.
	Returns true if successful, false on failure. */
	template <typename... T>
	static bool ReadSingleOverload(cLuaState & a_LuaState, const std::tuple<T...> & a_Overload)
	{
		// Check that there exactly as many params as the tuple items:
		if (!lua_isnone(a_LuaState, sizeof...(T) + 1))
		{
			// Too many params
			return false;
		}
		if (lua_isnone(a_LuaState, sizeof...(T)))
		{
			// Too few params
			return false;
		}

		// Read the tuple, compile-time-recursively:
		return ReadSingleOverloadRecurse(a_LuaState, a_Overload, SizeT<sizeof...(T)>());
	}



	/** Attempts to match the params on the Lua stack to the given API function overload.
	The compile-time recursive worker implementation of ReadSingleOverload, recurses by the number of elements in the overload tuple.
	Returns true on success, false on failure. */
	template <typename... T, size_t N>
	static bool ReadSingleOverloadRecurse(cLuaState & a_LuaState, const std::tuple<T...> & a_Overload, SizeT<N>)
	{
		// First read the params from the lower tuple indices:
		if (!ReadSingleOverloadRecurse(a_LuaState, a_Overload, SizeT<N - 1>()))
		{
			return false;
		}

		// Then read the param pointed to by our index:
		return GetStackValue(a_LuaState, N, std::get<N - 1>(a_Overload));
	}


	/** Terminator for the above compile-time-recursive function. */
	template <typename... T>
	static bool ReadSingleOverloadRecurse(cLuaState & a_LuaState, const std::tuple<T...> & a_Overload, SizeT<1>)
	{
		return GetStackValue(a_LuaState, 1, std::get<0>(a_Overload));
	}



	/** Reads one value from the Lua stack.
	Returns true on success, false on failure.
	The hard work is delegated into cLuaState that already has this function, but we need to specialize it for decorators (cNonNil, cSelf etc.). */
	template <typename T>
	static bool GetStackValue(cLuaState & a_LuaState, int a_StackPos, T && a_ReturnedVal)
	{
		return a_LuaState.GetStackValue(a_StackPos, std::forward<T>(a_ReturnedVal));
	}


	/** Reads one value from the Lua stack.
	Returns true on success, false on failure.
	Specialization: Retrieves a value that should represent a non-nil pointer. Used primarily for ReadParams(... nonNil(...)) */
	template <typename T>
	static bool GetStackValue(cLuaState & a_LuaState, int a_StackPos, cNonNil<T> & a_ReturnedVal)
	{
		auto res = GetStackValue(a_LuaState, a_StackPos, a_ReturnedVal.m_Dest);
		if (res)
		{
			if (a_ReturnedVal.m_Dest == nullptr)
			{
				return false;
			}
		}
		return res;
	}


	/** Reads one value from the Lua stack.
	Returns true on success, false on failure.
	Specialization: Retrieves a value that should represent a pointer to self - must not be nil. Used primarily for ReadParams(self(...)) */
	template <typename T>
	static bool GetStackValue(cLuaState & a_LuaState, int a_StackPos, cSelf<T> & a_ReturnedVal)
	{
		auto res = GetStackValue(a_LuaState, a_StackPos, a_ReturnedVal.m_Self);
		if (res)
		{
			if (a_ReturnedVal.m_Self == nullptr)
			{
				return false;
			}
		}
		return res;
	}


	/** Reads one value from the Lua stack.
	Returns true on success, false on failure.
	Specialization: The value should represent a class and has no useful meaning, it is not read, just type-checked.
	Used primarily for ReadParams(staticSelf(...)) */
	template <typename T>
	static bool GetStackValue(cLuaState & a_LuaState, int a_StackPos, cStaticSelf<T> & a_ReturnedVal)
	{
		return (CheckValueType(a_LuaState, a_StackPos, a_ReturnedVal).empty());
	}



	/** Raises a Lua error that the parameters don't match the overloads.
	Builds and logs the whole error message, including the reason why each overload wasn't matched. */
	template <typename... T>
	static void RaiseError(cLuaState & a_LuaState, T &&... a_Overloads)
	{
		auto matcherMsgs = BuildMatcherErrorMessages(a_LuaState, std::forward<T>(a_Overloads)...);
		a_LuaState.ApiParamError("Parameters don't match function signatures:\n%s",
			StringJoin(matcherMsgs, "\n\t").c_str()
		);
	}



	/** Returns a vector of string, each item representing a single overload's signature
	and the error message from the matcher why the signature cannot be used. */
	template <typename... T1, typename... T2>
	static AStringVector BuildMatcherErrorMessages(
		cLuaState & a_LuaState,
		const std::tuple<T1...> & a_Overload1,
		T2 &&... a_OtherOverloads
	)
	{
		auto res = BuildMatcherErrorMessages(a_LuaState, std::forward<T2>(a_OtherOverloads)...);
		auto signature = BuildSingleOverloadDescription<T1...>();
		auto msg = GetMatcherErrorMessage(a_LuaState, a_Overload1);
		res.emplace_back(std::move(Printf("(%s): %s", signature.c_str(), msg.c_str())));
		return res;
	}


	/** Terminator for the function above. */
	static AStringVector BuildMatcherErrorMessages(cLuaState & a_LuaState)
	{
		return AStringVector();
	}



	/** Returns the error message why the specified overload signature cannot be used for current params. */
	template <typename... T>
	static AString GetMatcherErrorMessage(cLuaState & a_LuaState, const std::tuple<T...> & a_Overload)
	{
		if (!lua_isnone(a_LuaState, static_cast<int>(sizeof...(T) + 1)))
		{
			return Printf("There are more parameters present (%d) than the signature allows (%u)",
				lua_gettop(a_LuaState),
				static_cast<unsigned>(sizeof...(T))
			);
		}
		if (lua_isnone(a_LuaState, static_cast<int>(sizeof...(T))))
		{
			return Printf("There are not enough parameters present (%d) to match the signature (%u).",
				lua_gettop(a_LuaState),
				static_cast<unsigned>(sizeof...(T))
			);
		}
		return GetParamMatchError(a_LuaState, a_Overload, SizeT<sizeof...(T)>());
	}



	/** Returns the string describing why the specified overload signature doesn't match current params.
	Checks each individual param using compile-time recursion.
	Doesn't check param end (checked by GetMatcherErrorMessage(), which is the only one calling this function). */
	template <typename... T, size_t N>
	static AString GetParamMatchError(cLuaState & a_LuaState, const std::tuple<T...> & a_Overload, SizeT<N>)
	{
		// Try to read the param into a dummy variable of the proper type:
		auto res = CheckValueType(a_LuaState, static_cast<int>(N), std::get<N - 1>(a_Overload));
		if (!res.empty())
		{
			return Printf("Parameter %u: %s", static_cast<unsigned>(N), res.c_str());
		}

		// Reading succeeded, try the next param:
		return GetParamMatchError(a_LuaState, a_Overload, SizeT<N - 1>());
	}


	/** Terminator for the above function
	We've checked all the params, so this should never be reached. */
	template <typename... T>
	static AString GetParamMatchError(cLuaState & a_LuaState, const std::tuple<T...> & a_Overload, SizeT<0>)
	{
		return "[internal matcher error - no reason for mismatch can be found]";
	}



	/** Returns an error message if the value on the specified index on the Lua stack is of the wrong (template) type.
	If the type matches, returns an empty string.
	This is the generic version for regular values. */
	template <typename T>
	static AString CheckValueType(cLuaState & a_LuaState, int a_StackPos, const T & a_Dest)
	{
		typename std::remove_reference<T>::type dummy;
		if (!a_LuaState.GetStackValue(a_StackPos, dummy))
		{
			return Printf("Mismatch, expected %s, got %s",
				GetTypeDescription<T>().c_str(),
				a_LuaState.GetTypeText(a_StackPos).c_str()
			);
		}
		return AString();
	}


	/** Returns an error message if the value on the specified index on the Lua stack is of the wrong (template) type.
	This is the specialization for "cNonNil"-decorated values. */
	template <typename T>
	static AString CheckValueType(cLuaState & a_LuaState, int a_StackPos, const cNonNil<T> & a_Dest)
	{
		if (lua_isnil(a_LuaState, a_StackPos))
		{
			return Printf("Expected a non-nil instance of %s, got a nil",
				GetTypeDescription<std::remove_reference<T>::type>().c_str(),
				a_LuaState.GetTypeText(a_StackPos).c_str()
			);
		}
		return CheckValueType(a_LuaState, a_StackPos, a_Dest.m_Dest);
	}


	/** Returns an error message if the value on the specified index on the Lua stack is of the wrong (template) type.
	This is the specialization for "cSelf"-decorated values. */
	template <typename T>
	static AString CheckValueType(cLuaState & a_LuaState, int a_StackPos, const cSelf<T> & a_Dest)
	{
		if (lua_isnil(a_LuaState, a_StackPos))
		{
			return Printf("Expected an instance of %s, got a %s. Did you use the right calling convention?",
				GetTypeDescription<T>().c_str(),
				a_LuaState.GetTypeText(a_StackPos).c_str()
			);
		}
		return CheckValueType(a_LuaState, a_StackPos, a_Dest.m_Self);
	}


	/** Returns an error message if the value on the specified index on the Lua stack is of the wrong (template) type.
	This is the specialization for "cStaticSelf"-decorated values. */
	template <typename T>
	static AString CheckValueType(cLuaState & a_LuaState, int a_StackPos, cStaticSelf<T> & a_Dest)
	{
		tolua_Error err;
		auto type = GetTypeDescription<T>();
		if (lua_isnil(a_LuaState, a_StackPos))
		{
			return Printf("Expected the class %s, got a nil", type.c_str());
		}
		if (tolua_isusertype(a_LuaState, a_StackPos, type.c_str(), 0, &err))
		{
			return Printf("Expected the class %s, got a %s. This function is static, remember to use the right calling convention.",
				type.c_str(),
				a_LuaState.GetTypeText(a_StackPos).c_str()
			);
		}
		return AString();
	}



	/** Returns a string describing the function overload composed of the specified (template) types. */
	template <typename T1, typename T2, typename... T3>
	static AString BuildSingleOverloadDescription()
	{
		return GetTypeDescription<T1>() + ", " + BuildSingleOverloadDescription<T2, T3...>();
	}


	/** Terminator for the above function. */
	template <typename T>
	static AString BuildSingleOverloadDescription()
	{
		return GetTypeDescription<T>();
	}



	/** Returns the Lua type representing the specified C++ type.
	Supports decorators such as cSelf, references and pointers. */
	template <typename T>
	static AString GetTypeDescription(void)
	{
		return Detail::TypeDescription<typename std::remove_pointer<typename std::remove_reference<T>::type>::type>::desc();
	}



public:

	/** The main entrypoint for API param reading.
	Tries to match the parameters on the Lua stack onto the specified overloads.
	Each overload is a std::tuple<...> of references which are to be filled with the values read.
	Returns the 0-based index of the first overload that matched the Lua params.
	If no overloads match, raises a Lua error with detailed information about why there was no match.
	The design goals:
		- In case of success, use as little overhead as possible
		- In case of error, provide as much information as possible, even at the cost of huge overhead
	The caller should order the overloads in the order by expected usage - the most used ones first.
	Because the overloads are checked sequentially, this provides the least overhead.
	Uses compile-time template recursion to iterate over all overloads and all their items.

	Call tree:
	Read
	+- ReadInternal <recursive>
	|  +- ReadSingleOverload
	|     +- ReadSingleOverloadRecurse <recursive>
	|        +- GetStackValue
	|           +- cLuaState::GetStackValue
	|           +- CheckValueType [for cStaticSelf]
	+- RaiseError
	|  +- BuildMatcherErrorMessages <recursive>
	|  |  +- BuildSingleOverloadDescription <recursive>
	|  |  |  +- GetTypeDescription
	|  |  |     +- Detail::TypeDescription<T>::desc
	|  |  +- GetMatcherErrorMessage
	|  |     +- GetParamMatchError <recursive>
	|  |        +- CheckValueType
	|  |           +- cLuaState::GetStackValue
	|  +- cLuaState::ApiParamError
	+- return [never reached]
	*/
	template <typename... OverloadTuples>
	static int Read(cLuaState & a_LuaState, OverloadTuples &&... a_Overloads)
	{
		// If the reading succeeded, return success
		auto res = ReadInternal(a_LuaState, 0, std::forward<OverloadTuples>(a_Overloads)...);
		if (res >= 0)
		{
			return res;
		}

		// The reading failed, raise an error:
		RaiseError(a_LuaState, std::forward<OverloadTuples>(a_Overloads)...);
		return 0;  // Never reached, but undefined behavior if not present
	}
};





namespace Detail
{
	// Specializations for basic types:
	template <> struct TypeDescription<AString>      { static const char * desc() { return "string"; } };
	template <> struct TypeDescription<const char *> { static const char * desc() { return "string"; } };
	template <> struct TypeDescription<int>          { static const char * desc() { return "number"; } };
	template <> struct TypeDescription<unsigned>     { static const char * desc() { return "number"; } };
	template <> struct TypeDescription<float>        { static const char * desc() { return "number"; } };
	template <> struct TypeDescription<double>       { static const char * desc() { return "number"; } };

	// Specializations for decorated types:
	template <typename T>
	struct TypeDescription<cLuaStateParams::cNonNil<T>>
	{
		static const char * desc() { return TypeDescription<T>::desc(); }
	};

	template <typename T>
	struct TypeDescription<cLuaStateParams::cNonNil<T> &>
	{
		static const char * desc() { return TypeDescription<T>::desc(); }
	};

	template <typename T>
	struct TypeDescription<cLuaStateParams::cSelf<T>>
	{
		static const char * desc() { return TypeDescription<T>::desc(); }
	};

	template <typename T>
	struct TypeDescription<cLuaStateParams::cSelf<T> &>
	{
		static const char * desc() { return TypeDescription<T>::desc(); }
	};

	template <typename T>
	struct TypeDescription<cLuaStateParams::cStaticSelf<T>>
	{
		static const char * desc() { return TypeDescription<T>::desc(); }
	};

	template <typename T>
	struct TypeDescription<cLuaStateParams::cStaticSelf<T> &>
	{
		static const char * desc() { return TypeDescription<T>::desc(); }
	};

	// Include the TypeDecription<T> specializations generated for all known API classes:
	#include "LuaStateParams_TypeDescs.inc"
}




