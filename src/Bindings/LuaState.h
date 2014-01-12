
// LuaState.h

// Declares the cLuaState class representing the wrapper over lua_State *, provides associated helper functions

/*
The contained lua_State can be either owned or attached.
Owned lua_State is created by calling Create() and the cLuaState automatically closes the state
Or, lua_State can be attached by calling Attach(), the cLuaState doesn't close such a state
Attaching a state will automatically close an owned state.

Calling a Lua function is done by pushing the function, either by PushFunction() or PushFunctionFromRegistry(),
then pushing the arguments (PushString(), PushNumber(), PushUserData() etc.) and finally
executing CallFunction(). cLuaState automatically keeps track of the number of arguments and the name of the
function (for logging purposes), which makes the call less error-prone.

Reference management is provided by the cLuaState::cRef class. This is used when you need to hold a reference to
any Lua object across several function calls; usually this is used for callbacks. The class is RAII-like, with
automatic resource management.
*/




#pragma once

extern "C"
{
	#include "lua/src/lauxlib.h"
}





class cWorld;
class cPlayer;
class cEntity;
class cMonster;
class cItem;
class cItems;
class cClientHandle;
class cPickup;
class cChunkDesc;
class cCraftingGrid;
class cCraftingRecipe;
struct TakeDamageInfo;
class cWindow;
class cPluginLua;
struct HTTPRequest;
class cWebAdmin;
struct HTTPTemplateRequest;
class cTNTEntity;
class cCreeper;
class Vector3i;
class cHopperEntity;
class cBlockEntity;





/// Encapsulates a Lua state and provides some syntactic sugar for common operations
class cLuaState
{
public:

	/// Used for storing references to object in the global registry
	class cRef
	{
	public:
		/// Creates a reference in the specified LuaState for object at the specified StackPos
		cRef(cLuaState & a_LuaState, int a_StackPos);
		~cRef();
		
		/// Returns true if the reference is valid
		bool IsValid(void) const {return (m_Ref != LUA_REFNIL); }
		
		/// Allows to use this class wherever an int (i. e. ref) is to be used
		operator int(void) const { return m_Ref; }
		
	protected:
		cLuaState & m_LuaState;
		int m_Ref;
	} ;
	
	
	/** Used for calling functions stored in a reference-stored table */
	class cTableRef
	{
		int m_TableRef;
		const char * m_FnName;
	public:
		cTableRef(int a_TableRef, const char * a_FnName) :
			m_TableRef(a_TableRef),
			m_FnName(a_FnName)
		{
		}
		
		int GetTableRef(void) const { return m_TableRef; }
		const char * GetFnName(void) const { return m_FnName; }
	} ;
	
	
	/// A dummy class that's used only to delimit function args from return values for cLuaState::Call()
	class cRet
	{
	} ;
	
	static const cRet Return;  // Use this constant to delimit function args from return values for cLuaState::Call()


	/** Creates a new instance. The LuaState is not initialized.
	a_SubsystemName is used for reporting problems in the console, it is "plugin %s" for plugins, 
	or "LuaScript" for the cLuaScript template
	*/
	cLuaState(const AString & a_SubsystemName);
	
	/** Creates a new instance. The a_AttachState is attached.
	Subsystem name is set to "<attached>".
	*/
	explicit cLuaState(lua_State * a_AttachState);
	
	~cLuaState();
	
	/// Allows this object to be used in the same way as a lua_State *, for example in the LuaLib functions
	operator lua_State * (void) { return m_LuaState; }
	
	/// Creates the m_LuaState, if not closed already. This state will be automatically closed in the destructor
	void Create(void);
	
	/// Closes the m_LuaState, if not closed already
	void Close(void);
	
	/// Attaches the specified state. Operations will be carried out on this state, but it will not be closed in the destructor
	void Attach(lua_State * a_State);
	
	/// Detaches a previously attached state.
	void Detach(void);
	
	/// Returns true if the m_LuaState is valid
	bool IsValid(void) const { return (m_LuaState != NULL); }
	
	/** Loads the specified file
	Returns false and logs a warning to the console if not successful (but the LuaState is kept open).
	m_SubsystemName is displayed in the warning log message.
	*/
	bool LoadFile(const AString & a_FileName);
	
	/// Returns true if a_FunctionName is a valid Lua function that can be called
	bool HasFunction(const char * a_FunctionName);
	
	// Push a value onto the stack
	void Push(const AString & a_String);
	void Push(const AStringVector & a_Vector);
	void Push(int a_Value);
	void Push(double a_Value);
	void Push(const char * a_Value);
	void Push(bool a_Value);
	void Push(cWorld * a_World);
	void Push(cPlayer * a_Player);
	void Push(const cPlayer * a_Player);
	void Push(cEntity * a_Entity);
	void Push(cMonster * a_Monster);
	void Push(cItem * a_Item);
	void Push(cItems * a_Items);
	void Push(const cItems & a_Items);
	void Push(cClientHandle * a_ClientHandle);
	void Push(cPickup * a_Pickup);
	void Push(cChunkDesc * a_ChunkDesc);
	void Push(const cCraftingGrid * a_Grid);
	void Push(const cCraftingRecipe * a_Recipe);
	void Push(TakeDamageInfo * a_TDI);
	void Push(cWindow * a_Window);
	void Push(cPluginLua * a_Plugin);
	void Push(const HTTPRequest * a_Request);
	void Push(cWebAdmin * a_WebAdmin);
	void Push(const HTTPTemplateRequest * a_Request);
	void Push(cTNTEntity * a_TNTEntity);
	void Push(cCreeper * a_Creeper);
	void Push(Vector3i * a_Vector);
	void Push(void * a_Ptr);
	void Push(cHopperEntity * a_Hopper);
	void Push(cBlockEntity * a_BlockEntity);

	/// Call any 0-param 0-return Lua function in a single line:
	template <typename FnT>
	bool Call(FnT a_FnName)
	{
		if (!PushFunction(a_FnName))
		{
			return false;
		}
		return CallFunction(0);
	}

	/// Call any 1-param 0-return Lua function in a single line:
	template<
		typename FnT,
		typename ArgT1
	>
	bool Call(FnT a_FnName, ArgT1 a_Arg1)
	{
		if (!PushFunction(a_FnName))
		{
			return false;
		}
		Push(a_Arg1);
		return CallFunction(0);
	}

	/// Call any 2-param 0-return Lua function in a single line:
	template<
		typename FnT, typename ArgT1, typename ArgT2
	>
	bool Call(FnT a_FnName, ArgT1 a_Arg1, ArgT2 a_Arg2)
	{
		if (!PushFunction(a_FnName))
		{
			return false;
		}
		Push(a_Arg1);
		Push(a_Arg2);
		return CallFunction(0);
	}

	/// Call any 3-param 0-return Lua function in a single line:
	template<
		typename FnT, typename ArgT1, typename ArgT2, typename ArgT3
	>
	bool Call(FnT a_FnName, ArgT1 a_Arg1, ArgT2 a_Arg2, ArgT3 a_Arg3)
	{
		if (!PushFunction(a_FnName))
		{
			return false;
		}
		Push(a_Arg1);
		Push(a_Arg2);
		Push(a_Arg3);
		return CallFunction(0);
	}

	/// Call any 0-param 1-return Lua function in a single line:
	template<
		typename FnT, typename RetT1
	>
	bool Call(FnT a_FnName, const cRet & a_Mark, RetT1 & a_Ret1)
	{
		UNUSED(a_Mark);
		if (!PushFunction(a_FnName))
		{
			return false;
		}
		if (!CallFunction(1))
		{
			return false;
		}
		GetReturn(-1, a_Ret1);
		lua_pop(m_LuaState, 1);
		return true;
	}

	/// Call any 1-param 1-return Lua function in a single line:
	template<
		typename FnT, typename ArgT1, typename RetT1
	>
	bool Call(FnT a_FnName, ArgT1 a_Arg1, const cRet & a_Mark, RetT1 & a_Ret1)
	{
		UNUSED(a_Mark);
		if (!PushFunction(a_FnName))
		{
			return false;
		}
		Push(a_Arg1);
		if (!CallFunction(1))
		{
			return false;
		}
		GetReturn(-1, a_Ret1);
		lua_pop(m_LuaState, 1);
		return true;
	}

	/// Call any 2-param 1-return Lua function in a single line:
	template<
		typename FnT, typename ArgT1, typename ArgT2, typename RetT1
	>
	bool Call(FnT a_FnName, ArgT1 a_Arg1, ArgT2 a_Arg2, const cRet & a_Mark, RetT1 & a_Ret1)
	{
		UNUSED(a_Mark);
		if (!PushFunction(a_FnName))
		{
			return false;
		}
		Push(a_Arg1);
		Push(a_Arg2);
		if (!CallFunction(1))
		{
			return false;
		}
		GetReturn(-1, a_Ret1);
		lua_pop(m_LuaState, 1);
		return true;
	}

	/// Call any 3-param 1-return Lua function in a single line:
	template<
		typename FnT, typename ArgT1, typename ArgT2, typename ArgT3, typename RetT1
	>
	bool Call(FnT a_FnName, ArgT1 a_Arg1, ArgT2 a_Arg2, ArgT3 a_Arg3, const cRet & a_Mark, RetT1 & a_Ret1)
	{
		UNUSED(a_Mark);
		if (!PushFunction(a_FnName))
		{
			return false;
		}
		Push(a_Arg1);
		Push(a_Arg2);
		Push(a_Arg3);
		if (!CallFunction(1))
		{
			return false;
		}
		GetReturn(-1, a_Ret1);
		lua_pop(m_LuaState, 1);
		return true;
	}
	
	/// Call any 4-param 1-return Lua function in a single line:
	template<
		typename FnT, typename ArgT1, typename ArgT2, typename ArgT3, typename ArgT4, typename RetT1
	>
	bool Call(FnT a_FnName, ArgT1 a_Arg1, ArgT2 a_Arg2, ArgT3 a_Arg3, ArgT4 a_Arg4, const cRet & a_Mark, RetT1 & a_Ret1)
	{
		UNUSED(a_Mark);
		if (!PushFunction(a_FnName))
		{
			return false;
		}
		Push(a_Arg1);
		Push(a_Arg2);
		Push(a_Arg3);
		Push(a_Arg4);
		if (!CallFunction(1))
		{
			return false;
		}
		GetReturn(-1, a_Ret1);
		lua_pop(m_LuaState, 1);
		return true;
	}
	
	/// Call any 5-param 1-return Lua function in a single line:
	template<
		typename FnT, typename ArgT1, typename ArgT2, typename ArgT3, typename ArgT4, typename ArgT5, typename RetT1
	>
	bool Call(FnT a_FnName, ArgT1 a_Arg1, ArgT2 a_Arg2, ArgT3 a_Arg3, ArgT4 a_Arg4, ArgT5 a_Arg5, const cRet & a_Mark, RetT1 & a_Ret1)
	{
		UNUSED(a_Mark);
		if (!PushFunction(a_FnName))
		{
			return false;
		}
		Push(a_Arg1);
		Push(a_Arg2);
		Push(a_Arg3);
		Push(a_Arg4);
		Push(a_Arg5);
		if (!CallFunction(1))
		{
			return false;
		}
		GetReturn(-1, a_Ret1);
		lua_pop(m_LuaState, 1);
		return true;
	}
	
	/// Call any 6-param 1-return Lua function in a single line:
	template<
		typename FnT, typename ArgT1, typename ArgT2, typename ArgT3, typename ArgT4, typename ArgT5, typename ArgT6,
		typename RetT1
	>
	bool Call(FnT a_FnName, ArgT1 a_Arg1, ArgT2 a_Arg2, ArgT3 a_Arg3, ArgT4 a_Arg4, ArgT5 a_Arg5, ArgT6 a_Arg6, const cRet & a_Mark, RetT1 & a_Ret1)
	{
		UNUSED(a_Mark);
		if (!PushFunction(a_FnName))
		{
			return false;
		}
		Push(a_Arg1);
		Push(a_Arg2);
		Push(a_Arg3);
		Push(a_Arg4);
		Push(a_Arg5);
		Push(a_Arg6);
		if (!CallFunction(1))
		{
			return false;
		}
		GetReturn(-1, a_Ret1);
		lua_pop(m_LuaState, 1);
		return true;
	}
	
	/// Call any 7-param 1-return Lua function in a single line:
	template<
		typename FnT, typename ArgT1, typename ArgT2, typename ArgT3, typename ArgT4, typename ArgT5, typename ArgT6,
		typename ArgT7, typename RetT1
	>
	bool Call(FnT a_FnName, ArgT1 a_Arg1, ArgT2 a_Arg2, ArgT3 a_Arg3, ArgT4 a_Arg4, ArgT5 a_Arg5, ArgT6 a_Arg6, ArgT7 a_Arg7, const cRet & a_Mark, RetT1 & a_Ret1)
	{
		UNUSED(a_Mark);
		if (!PushFunction(a_FnName))
		{
			return false;
		}
		Push(a_Arg1);
		Push(a_Arg2);
		Push(a_Arg3);
		Push(a_Arg4);
		Push(a_Arg5);
		Push(a_Arg6);
		Push(a_Arg7);
		if (!CallFunction(1))
		{
			return false;
		}
		GetReturn(-1, a_Ret1);
		lua_pop(m_LuaState, 1);
		return true;
	}
	
	/// Call any 8-param 1-return Lua function in a single line:
	template<
		typename FnT, typename ArgT1, typename ArgT2, typename ArgT3, typename ArgT4, typename ArgT5, typename ArgT6,
		typename ArgT7, typename ArgT8, typename RetT1
	>
	bool Call(FnT a_FnName, ArgT1 a_Arg1, ArgT2 a_Arg2, ArgT3 a_Arg3, ArgT4 a_Arg4, ArgT5 a_Arg5, ArgT6 a_Arg6, ArgT7 a_Arg7, ArgT8 a_Arg8, const cRet & a_Mark, RetT1 & a_Ret1)
	{
		UNUSED(a_Mark);
		if (!PushFunction(a_FnName))
		{
			return false;
		}
		Push(a_Arg1);
		Push(a_Arg2);
		Push(a_Arg3);
		Push(a_Arg4);
		Push(a_Arg5);
		Push(a_Arg6);
		Push(a_Arg7);
		Push(a_Arg8);
		if (!CallFunction(1))
		{
			return false;
		}
		GetReturn(-1, a_Ret1);
		lua_pop(m_LuaState, 1);
		return true;
	}
	
	/// Call any 9-param 1-return Lua function in a single line:
	template<
		typename FnT, typename ArgT1, typename ArgT2, typename ArgT3, typename ArgT4, typename ArgT5, typename ArgT6,
		typename ArgT7, typename ArgT8, typename ArgT9, typename RetT1
	>
	bool Call(FnT a_FnName, ArgT1 a_Arg1, ArgT2 a_Arg2, ArgT3 a_Arg3, ArgT4 a_Arg4, ArgT5 a_Arg5, ArgT6 a_Arg6, ArgT7 a_Arg7, ArgT8 a_Arg8, ArgT9 a_Arg9, const cRet & a_Mark, RetT1 & a_Ret1)
	{
		UNUSED(a_Mark);
		if (!PushFunction(a_FnName))
		{
			return false;
		}
		Push(a_Arg1);
		Push(a_Arg2);
		Push(a_Arg3);
		Push(a_Arg4);
		Push(a_Arg5);
		Push(a_Arg6);
		Push(a_Arg7);
		Push(a_Arg8);
		Push(a_Arg9);
		if (!CallFunction(1))
		{
			return false;
		}
		GetReturn(-1, a_Ret1);
		lua_pop(m_LuaState, 1);
		return true;
	}
	
	/// Call any 10-param 1-return Lua function in a single line:
	template<
		typename FnT, typename ArgT1, typename ArgT2, typename ArgT3, typename ArgT4, typename ArgT5, typename ArgT6,
		typename ArgT7, typename ArgT8, typename ArgT9, typename ArgT10, typename RetT1
	>
	bool Call(FnT a_FnName, ArgT1 a_Arg1, ArgT2 a_Arg2, ArgT3 a_Arg3, ArgT4 a_Arg4, ArgT5 a_Arg5, ArgT6 a_Arg6, ArgT7 a_Arg7, ArgT8 a_Arg8, ArgT9 a_Arg9, ArgT10 a_Arg10, const cRet & a_Mark, RetT1 & a_Ret1)
	{
		UNUSED(a_Mark);
		if (!PushFunction(a_FnName))
		{
			return false;
		}
		Push(a_Arg1);
		Push(a_Arg2);
		Push(a_Arg3);
		Push(a_Arg4);
		Push(a_Arg5);
		Push(a_Arg6);
		Push(a_Arg7);
		Push(a_Arg8);
		Push(a_Arg9);
		Push(a_Arg10);
		if (!CallFunction(1))
		{
			return false;
		}
		GetReturn(-1, a_Ret1);
		lua_pop(m_LuaState, 1);
		return true;
	}
	
	/// Call any 1-param 2-return Lua function in a single line:
	template<
		typename FnT, typename ArgT1, typename RetT1, typename RetT2
	>
	bool Call(FnT a_FnName, ArgT1 a_Arg1, const cRet & a_Mark, RetT1 & a_Ret1, RetT2 & a_Ret2)
	{
		UNUSED(a_Mark);
		if (!PushFunction(a_FnName))
		{
			return false;
		}
		Push(a_Arg1);
		if (!CallFunction(2))
		{
			return false;
		}
		GetReturn(-2, a_Ret1);
		GetReturn(-1, a_Ret2);
		lua_pop(m_LuaState, 2);
		return true;
	}

	/// Call any 2-param 2-return Lua function in a single line:
	template<
		typename FnT, typename ArgT1, typename ArgT2, typename RetT1, typename RetT2
	>
	bool Call(FnT a_FnName, ArgT1 a_Arg1, ArgT2 a_Arg2, const cRet & a_Mark, RetT1 & a_Ret1, RetT2 & a_Ret2)
	{
		UNUSED(a_Mark);
		if (!PushFunction(a_FnName))
		{
			return false;
		}
		Push(a_Arg1);
		Push(a_Arg2);
		if (!CallFunction(2))
		{
			return false;
		}
		GetReturn(-2, a_Ret1);
		GetReturn(-1, a_Ret2);
		lua_pop(m_LuaState, 2);
		return true;
	}

	/// Call any 3-param 2-return Lua function in a single line:
	template<
		typename FnT, typename ArgT1, typename ArgT2, typename ArgT3,
		typename RetT1, typename RetT2
	>
	bool Call(FnT a_FnName, ArgT1 a_Arg1, ArgT2 a_Arg2, ArgT3 a_Arg3, const cRet & a_Mark, RetT1 & a_Ret1, RetT2 & a_Ret2)
	{
		UNUSED(a_Mark);
		if (!PushFunction(a_FnName))
		{
			return false;
		}
		Push(a_Arg1);
		Push(a_Arg2);
		Push(a_Arg3);
		if (!CallFunction(2))
		{
			return false;
		}
		GetReturn(-2, a_Ret1);
		GetReturn(-1, a_Ret2);
		lua_pop(m_LuaState, 2);
		return true;
	}

	/// Call any 4-param 2-return Lua function in a single line:
	template<
		typename FnT, typename ArgT1, typename ArgT2, typename ArgT3, typename ArgT4,
		typename RetT1, typename RetT2
	>
	bool Call(FnT a_FnName, ArgT1 a_Arg1, ArgT2 a_Arg2, ArgT3 a_Arg3, ArgT4 a_Arg4, const cRet & a_Mark, RetT1 & a_Ret1, RetT2 & a_Ret2)
	{
		UNUSED(a_Mark);
		if (!PushFunction(a_FnName))
		{
			return false;
		}
		Push(a_Arg1);
		Push(a_Arg2);
		Push(a_Arg3);
		Push(a_Arg4);
		if (!CallFunction(2))
		{
			return false;
		}
		GetReturn(-2, a_Ret1);
		GetReturn(-1, a_Ret2);
		lua_pop(m_LuaState, 2);
		return true;
	}

	/// Call any 5-param 2-return Lua function in a single line:
	template<
		typename FnT, typename ArgT1, typename ArgT2, typename ArgT3, typename ArgT4, typename ArgT5,
		typename RetT1, typename RetT2
	>
	bool Call(FnT a_FnName, ArgT1 a_Arg1, ArgT2 a_Arg2, ArgT3 a_Arg3, ArgT4 a_Arg4, ArgT5 a_Arg5, const cRet & a_Mark, RetT1 & a_Ret1, RetT2 & a_Ret2)
	{
		UNUSED(a_Mark);
		if (!PushFunction(a_FnName))
		{
			return false;
		}
		Push(a_Arg1);
		Push(a_Arg2);
		Push(a_Arg3);
		Push(a_Arg4);
		Push(a_Arg5);
		if (!CallFunction(2))
		{
			return false;
		}
		GetReturn(-2, a_Ret1);
		GetReturn(-1, a_Ret2);
		lua_pop(m_LuaState, 2);
		return true;
	}

	/// Call any 6-param 2-return Lua function in a single line:
	template<
		typename FnT, typename ArgT1, typename ArgT2, typename ArgT3, typename ArgT4, typename ArgT5,
		typename ArgT6,
		typename RetT1, typename RetT2
	>
	bool Call(FnT a_FnName, ArgT1 a_Arg1, ArgT2 a_Arg2, ArgT3 a_Arg3, ArgT4 a_Arg4, ArgT5 a_Arg5, ArgT6 a_Arg6, const cRet & a_Mark, RetT1 & a_Ret1, RetT2 & a_Ret2)
	{
		UNUSED(a_Mark);
		if (!PushFunction(a_FnName))
		{
			return false;
		}
		Push(a_Arg1);
		Push(a_Arg2);
		Push(a_Arg3);
		Push(a_Arg4);
		Push(a_Arg5);
		Push(a_Arg6);
		if (!CallFunction(2))
		{
			return false;
		}
		GetReturn(-2, a_Ret1);
		GetReturn(-1, a_Ret2);
		lua_pop(m_LuaState, 2);
		return true;
	}

	/// Call any 7-param 2-return Lua function in a single line:
	template<
		typename FnT, typename ArgT1, typename ArgT2, typename ArgT3, typename ArgT4, typename ArgT5,
		typename ArgT6, typename ArgT7,
		typename RetT1, typename RetT2
	>
	bool Call(FnT a_FnName, ArgT1 a_Arg1, ArgT2 a_Arg2, ArgT3 a_Arg3, ArgT4 a_Arg4, ArgT5 a_Arg5, ArgT6 a_Arg6, ArgT7 a_Arg7, const cRet & a_Mark, RetT1 & a_Ret1, RetT2 & a_Ret2)
	{
		UNUSED(a_Mark);
		if (!PushFunction(a_FnName))
		{
			return false;
		}
		Push(a_Arg1);
		Push(a_Arg2);
		Push(a_Arg3);
		Push(a_Arg4);
		Push(a_Arg5);
		Push(a_Arg6);
		Push(a_Arg7);
		if (!CallFunction(2))
		{
			return false;
		}
		GetReturn(-2, a_Ret1);
		GetReturn(-1, a_Ret2);
		lua_pop(m_LuaState, 2);
		return true;
	}

	/// Call any 7-param 3-return Lua function in a single line:
	template<
		typename FnT, typename ArgT1, typename ArgT2, typename ArgT3, typename ArgT4, typename ArgT5,
		typename ArgT6, typename ArgT7,
		typename RetT1, typename RetT2, typename RetT3
	>
	bool Call(FnT a_FnName, ArgT1 a_Arg1, ArgT2 a_Arg2, ArgT3 a_Arg3, ArgT4 a_Arg4, ArgT5 a_Arg5, ArgT6 a_Arg6, ArgT7 a_Arg7, const cRet & a_Mark, RetT1 & a_Ret1, RetT2 & a_Ret2, RetT3 & a_Ret3)
	{
		UNUSED(a_Mark);
		if (!PushFunction(a_FnName))
		{
			return false;
		}
		Push(a_Arg1);
		Push(a_Arg2);
		Push(a_Arg3);
		Push(a_Arg4);
		Push(a_Arg5);
		Push(a_Arg6);
		Push(a_Arg7);
		if (!CallFunction(3))
		{
			return false;
		}
		GetReturn(-3, a_Ret1);
		GetReturn(-2, a_Ret2);
		GetReturn(-1, a_Ret3);
		lua_pop(m_LuaState, 3);
		return true;
	}

	/// Call any 8-param 3-return Lua function in a single line:
	template<
		typename FnT, typename ArgT1, typename ArgT2, typename ArgT3, typename ArgT4, typename ArgT5,
		typename ArgT6, typename ArgT7, typename ArgT8,
		typename RetT1, typename RetT2, typename RetT3
	>
	bool Call(FnT a_FnName, ArgT1 a_Arg1, ArgT2 a_Arg2, ArgT3 a_Arg3, ArgT4 a_Arg4, ArgT5 a_Arg5, ArgT6 a_Arg6, ArgT7 a_Arg7, ArgT8 a_Arg8, const cRet & a_Mark, RetT1 & a_Ret1, RetT2 & a_Ret2, RetT3 & a_Ret3)
	{
		UNUSED(a_Mark);
		if (!PushFunction(a_FnName))
		{
			return false;
		}
		Push(a_Arg1);
		Push(a_Arg2);
		Push(a_Arg3);
		Push(a_Arg4);
		Push(a_Arg5);
		Push(a_Arg6);
		Push(a_Arg7);
		Push(a_Arg8);
		if (!CallFunction(3))
		{
			return false;
		}
		GetReturn(-3, a_Ret1);
		GetReturn(-2, a_Ret2);
		GetReturn(-1, a_Ret3);
		lua_pop(m_LuaState, 3);
		return true;
	}

	/// Call any 9-param 5-return Lua function in a single line:
	template<
		typename FnT, typename ArgT1, typename ArgT2, typename ArgT3, typename ArgT4, typename ArgT5,
		typename ArgT6, typename ArgT7, typename ArgT8, typename ArgT9, 
		typename RetT1, typename RetT2, typename RetT3, typename RetT4, typename RetT5
	>
	bool Call(FnT a_FnName, ArgT1 a_Arg1, ArgT2 a_Arg2, ArgT3 a_Arg3, ArgT4 a_Arg4, ArgT5 a_Arg5, ArgT6 a_Arg6, ArgT7 a_Arg7, ArgT8 a_Arg8, ArgT9 a_Arg9, const cRet & a_Mark, RetT1 & a_Ret1, RetT2 & a_Ret2, RetT3 & a_Ret3, RetT4 & a_Ret4, RetT5 & a_Ret5)
	{
		UNUSED(a_Mark);
		if (!PushFunction(a_FnName))
		{
			return false;
		}
		Push(a_Arg1);
		Push(a_Arg2);
		Push(a_Arg3);
		Push(a_Arg4);
		Push(a_Arg5);
		Push(a_Arg6);
		Push(a_Arg7);
		Push(a_Arg8);
		Push(a_Arg9);
		if (!CallFunction(5))
		{
			return false;
		}
		GetReturn(-5, a_Ret1);
		GetReturn(-4, a_Ret2);
		GetReturn(-3, a_Ret3);
		GetReturn(-2, a_Ret4);
		GetReturn(-1, a_Ret5);
		lua_pop(m_LuaState, 5);
		return true;
	}


	/// Returns true if the specified parameters on the stack are of the specified usertable type; also logs warning if not. Used for static functions
	bool CheckParamUserTable(int a_StartParam, const char * a_UserTable, int a_EndParam = -1);
	
	/// Returns true if the specified parameters on the stack are of the specified usertype; also logs warning if not. Used for regular functions
	bool CheckParamUserType(int a_StartParam, const char * a_UserType, int a_EndParam = -1);
	
	/// Returns true if the specified parameters on the stack are tables; also logs warning if not
	bool CheckParamTable(int a_StartParam, int a_EndParam = -1);
	
	/// Returns true if the specified parameters on the stack are numbers; also logs warning if not
	bool CheckParamNumber(int a_StartParam, int a_EndParam = -1);
	
	/// Returns true if the specified parameters on the stack are strings; also logs warning if not
	bool CheckParamString(int a_StartParam, int a_EndParam = -1);
	
	/// Returns true if the specified parameter on the stack is nil (indicating an end-of-parameters)
	bool CheckParamEnd(int a_Param);
	
	/// If the status is nonzero, prints the text on the top of Lua stack and returns true
	bool ReportErrors(int status);
	
	/// If the status is nonzero, prints the text on the top of Lua stack and returns true
	static bool ReportErrors(lua_State * a_LuaState, int status);
	
	/// Logs all items in the current stack trace to the server console
	void LogStackTrace(void);
	
	/// Logs all items in the current stack trace to the server console
	static void LogStackTrace(lua_State * a_LuaState);
	
	/// Returns the type of the item on the specified position in the stack
	AString GetTypeText(int a_StackPos);
	
protected:
	lua_State * m_LuaState;
	
	/// If true, the state is owned by this object and will be auto-Closed. False => attached state
	bool m_IsOwned;
	
	/** The subsystem name is used for reporting errors to the console, it is either "plugin %s" or "LuaScript"
	whatever is given to the constructor
	*/
	AString m_SubsystemName;
	
	/// Name of the currently pushed function (for the Push / Call chain)
	AString m_CurrentFunctionName;
	
	/// Number of arguments currently pushed (for the Push / Call chain)
	int m_NumCurrentFunctionArgs;


	/** Pushes the function of the specified name onto the stack.
	Returns true if successful. Logs a warning on failure (incl. m_SubsystemName)
	*/
	bool PushFunction(const char * a_FunctionName);
	
	/** Pushes a function that has been saved into the global registry, identified by a_FnRef.
	Returns true if successful. Logs a warning on failure
	*/
	bool PushFunction(int a_FnRef);
	
	/** Pushes a function that is stored in a referenced table by name
	Returns true if successful. Logs a warning on failure
	*/
	bool PushFunction(const cTableRef & a_TableRef);
	
	/// Pushes a usertype of the specified class type onto the stack
	void PushUserType(void * a_Object, const char * a_Type);

	/// Retrieve value returned at a_StackPos, if it is a valid bool. If not, a_ReturnedVal is unchanged
	void GetReturn(int a_StackPos, bool & a_ReturnedVal);
	
	/// Retrieve value returned at a_StackPos, if it is a valid string. If not, a_ReturnedVal is unchanged
	void GetReturn(int a_StackPos, AString & a_ReturnedVal);
	
	/// Retrieve value returned at a_StackPos, if it is a valid number. If not, a_ReturnedVal is unchanged
	void GetReturn(int a_StackPos, int & a_ReturnedVal);
	
	/// Retrieve value returned at a_StackPos, if it is a valid number. If not, a_ReturnedVal is unchanged
	void GetReturn(int a_StackPos, double & a_ReturnedVal);
	
	/**
	Calls the function that has been pushed onto the stack by PushFunction(),
	with arguments pushed by PushXXX().
	Returns true if successful, logs a warning on failure.
	*/
	bool CallFunction(int a_NumReturnValues);
	
	/** Used as the error reporting function for function calls */
	static int ReportFnCallErrors(lua_State * a_LuaState);
} ;




