
// LuaState.h

// Declares the cLuaState class representing the wrapper over lua_State *, provides associated helper functions

/*
The contained lua_State can be either owned or attached.
Owned lua_State is created by calling Create() and the cLuaState automatically closes the state
Or, lua_State can be attached by calling Attach(), the cLuaState doesn't close such a state
Attaching a state will automatically close an owned state.

Calling a Lua function is done internally by pushing the function using PushFunction(), then pushing the
arguments and finally executing CallFunction(). cLuaState automatically keeps track of the number of
arguments and the name of the function (for logging purposes). After the call the return values are read from
the stack using GetStackValue(). All of this is wrapped in a templated function overloads cLuaState::Call(),
which is generated automatically by gen_LuaState_Call.lua script file into the LuaState_Call.inc file.

Reference management is provided by the cLuaState::cRef class. This is used when you need to hold a reference to
any Lua object across several function calls; usually this is used for callbacks. The class is RAII-like, with
automatic resource management.
*/




#pragma once

extern "C"
{
	#include "lua/src/lauxlib.h"
}

#include "../Vector3.h"
#include "../Defines.h"





class cWorld;
class cPlayer;
class cEntity;
class cProjectileEntity;
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
class cHopperEntity;
class cBlockEntity;
class cBoundingBox;
class cLuaTCPLink;
class cLuaServerHandle;

typedef cBoundingBox * pBoundingBox;
typedef cWorld *       pWorld;





/** Encapsulates a Lua state and provides some syntactic sugar for common operations */
class cLuaState
{
public:

	/** Used for storing references to object in the global registry.
	Can be bound (contains a reference) or unbound (doesn't contain reference).
	The reference can also be reset by calling RefStack(). */
	class cRef
	{
	public:
		/** Creates an unbound reference object. */
		cRef(void);
		
		/** Creates a reference in the specified LuaState for object at the specified StackPos */
		cRef(cLuaState & a_LuaState, int a_StackPos);

		/** Moves the reference from the specified instance into a newly created instance.
		The old instance is then "!IsValid()". */
		cRef(cRef && a_FromRef);
		
		~cRef();
		
		/** Creates a reference to Lua object at the specified stack pos, binds this object to it.
		Calls UnRef() first if previously bound to another reference. */
		void RefStack(cLuaState & a_LuaState, int a_StackPos);
		
		/** Removes the bound reference, resets the object to Unbound state. */
		void UnRef(void);
		
		/** Returns true if the reference is valid */
		bool IsValid(void) const {return (m_Ref != LUA_REFNIL); }
		
		/** Allows to use this class wherever an int (i. e. ref) is to be used */
		operator int(void) const { return m_Ref; }
		
	protected:
		cLuaState * m_LuaState;
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
	
	
	/** A dummy class that's used only to delimit function args from return values for cLuaState::Call() */
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
	
	/** Allows this object to be used in the same way as a lua_State *, for example in the LuaLib functions */
	operator lua_State * (void) { return m_LuaState; }
	
	/** Creates the m_LuaState, if not closed already. This state will be automatically closed in the destructor.
	The regular Lua libs are registered, but the MCS API is not registered (so that Lua can be used as
	lite-config as well), use RegisterAPILibs() to do that. */
	void Create(void);
	
	/** Registers all the API libraries that MCS provides into m_LuaState. */
	void RegisterAPILibs(void);
	
	/** Closes the m_LuaState, if not closed already */
	void Close(void);
	
	/** Attaches the specified state. Operations will be carried out on this state, but it will not be closed in the destructor */
	void Attach(lua_State * a_State);
	
	/** Detaches a previously attached state. */
	void Detach(void);
	
	/** Returns true if the m_LuaState is valid */
	bool IsValid(void) const { return (m_LuaState != nullptr); }
	
	/** Adds the specified path to package.<a_PathVariable> */
	void AddPackagePath(const AString & a_PathVariable, const AString & a_Path);
	
	/** Loads the specified file
	Returns false and logs a warning to the console if not successful (but the LuaState is kept open).
	m_SubsystemName is displayed in the warning log message.
	*/
	bool LoadFile(const AString & a_FileName);
	
	/** Returns true if a_FunctionName is a valid Lua function that can be called */
	bool HasFunction(const char * a_FunctionName);
	
	// Push a const value onto the stack (keep alpha-sorted):
	void Push(const AString & a_String);
	void Push(const AStringVector & a_Vector);
	void Push(const cCraftingGrid * a_Grid);
	void Push(const cCraftingRecipe * a_Recipe);
	void Push(const char * a_Value);
	void Push(const cItems & a_Items);
	void Push(const cPlayer * a_Player);
	void Push(const HTTPRequest * a_Request);
	void Push(const HTTPTemplateRequest * a_Request);
	void Push(const Vector3d & a_Vector);
	void Push(const Vector3d * a_Vector);
	void Push(const Vector3i & a_Vector);
	void Push(const Vector3i * a_Vector);

	// Push a value onto the stack (keep alpha-sorted):
	void Push(bool a_Value);
	void Push(cBlockEntity * a_BlockEntity);
	void Push(cChunkDesc * a_ChunkDesc);
	void Push(cClientHandle * a_ClientHandle);
	void Push(cEntity * a_Entity);
	void Push(cHopperEntity * a_Hopper);
	void Push(cItem * a_Item);
	void Push(cItems * a_Items);
	void Push(cLuaServerHandle * a_ServerHandle);
	void Push(cLuaTCPLink * a_TCPLink);
	void Push(cMonster * a_Monster);
	void Push(cPickup * a_Pickup);
	void Push(cPlayer * a_Player);
	void Push(cPluginLua * a_Plugin);
	void Push(cProjectileEntity * a_ProjectileEntity);
	void Push(cTNTEntity * a_TNTEntity);
	void Push(cWebAdmin * a_WebAdmin);
	void Push(cWindow * a_Window);
	void Push(cWorld * a_World);
	void Push(double a_Value);
	void Push(int a_Value);
	void Push(TakeDamageInfo * a_TDI);
	void Push(Vector3d * a_Vector);
	void Push(Vector3i * a_Vector);
	void Push(void * a_Ptr);
	void Push(std::chrono::milliseconds a_time);
	
	/** Retrieve value at a_StackPos, if it is a valid bool. If not, a_Value is unchanged */
	void GetStackValue(int a_StackPos, bool & a_Value);
	
	/** Retrieve value at a_StackPos, if it is a valid string. If not, a_Value is unchanged */
	void GetStackValue(int a_StackPos, AString & a_Value);
	
	/** Retrieve value at a_StackPos, if it is a valid number. If not, a_Value is unchanged */
	void GetStackValue(int a_StackPos, int & a_Value);
	
	/** Retrieve value at a_StackPos, if it is a valid number. If not, a_Value is unchanged */
	void GetStackValue(int a_StackPos, double & a_Value);
	
	/** Retrieve value at a_StackPos, if it is a valid number, converting and clamping it to eWeather.
	If not, a_Value is unchanged. */
	void GetStackValue(int a_StackPos, eWeather & a_Value);

	/** Retrieve value at a_StackPos, if it is a valid cBoundingBox class. If not, a_Value is unchanged */
	void GetStackValue(int a_StackPos, pBoundingBox & a_Value);
	
	/** Retrieve value at a_StackPos, if it is a valid cWorld class. If not, a_Value is unchanged */
	void GetStackValue(int a_StackPos, pWorld & a_Value);

	/** Store the value at a_StackPos as a reference. */
	void GetStackValue(int a_StackPos, cRef & a_Ref);
	
	/** Call the specified Lua function.
	Returns true if call succeeded, false if there was an error.
	A special param of cRet & signifies the end of param list and the start of return values.
	Example call: Call(Fn, Param1, Param2, Param3, cLuaState::Return, Ret1, Ret2) */
	template <typename FnT, typename... Args>
	bool Call(const FnT & a_Function, Args &&... args)
	{
		if (!PushFunction(a_Function))
		{
			// Pushing the function failed
			return false;
		}
		return PushCallPop(args...);
	}

	/** Retrieves a list of values from the Lua stack, starting at the specified index. */
	template <typename T, typename... Args>
	inline void GetStackValues(int a_StartStackPos, T & a_Ret, Args &&... args)
	{
		GetStackValue(a_StartStackPos, a_Ret);
		GetStackValues(a_StartStackPos + 1, args...);
	}

	/** Returns true if the specified parameters on the stack are of the specified usertable type; also logs warning if not. Used for static functions */
	bool CheckParamUserTable(int a_StartParam, const char * a_UserTable, int a_EndParam = -1);
	
	/** Returns true if the specified parameters on the stack are of the specified usertype; also logs warning if not. Used for regular functions */
	bool CheckParamUserType(int a_StartParam, const char * a_UserType, int a_EndParam = -1);
	
	/** Returns true if the specified parameters on the stack are tables; also logs warning if not */
	bool CheckParamTable(int a_StartParam, int a_EndParam = -1);
	
	/** Returns true if the specified parameters on the stack are numbers; also logs warning if not */
	bool CheckParamNumber(int a_StartParam, int a_EndParam = -1);
	
	/** Returns true if the specified parameters on the stack are strings; also logs warning if not */
	bool CheckParamString(int a_StartParam, int a_EndParam = -1);
	
	/** Returns true if the specified parameters on the stack are functions; also logs warning if not */
	bool CheckParamFunction(int a_StartParam, int a_EndParam = -1);
	
	/** Returns true if the specified parameters on the stack are functions or nils; also logs warning if not */
	bool CheckParamFunctionOrNil(int a_StartParam, int a_EndParam = -1);
	
	/** Returns true if the specified parameter on the stack is nil (indicating an end-of-parameters) */
	bool CheckParamEnd(int a_Param);
	
	/** If the status is nonzero, prints the text on the top of Lua stack and returns true */
	bool ReportErrors(int status);
	
	/** If the status is nonzero, prints the text on the top of Lua stack and returns true */
	static bool ReportErrors(lua_State * a_LuaState, int status);
	
	/** Logs all items in the current stack trace to the server console */
	void LogStackTrace(int a_StartingDepth = 0);
	
	/** Logs all items in the current stack trace to the server console */
	static void LogStackTrace(lua_State * a_LuaState, int a_StartingDepth = 0);
	
	/** Returns the type of the item on the specified position in the stack */
	AString GetTypeText(int a_StackPos);
	
	/** Calls the function specified by its name, with arguments copied off the foreign state.
	If successful, keeps the return values on the stack and returns their number.
	If unsuccessful, returns a negative number and keeps the stack position unchanged. */
	int CallFunctionWithForeignParams(
		const AString & a_FunctionName,
		cLuaState & a_SrcLuaState,
		int a_SrcParamStart,
		int a_SrcParamEnd
	);
	
	/** Copies objects on the stack from the specified state.
	Only numbers, bools, strings and userdatas are copied.
	If successful, returns the number of objects copied.
	If failed, returns a negative number and rewinds the stack position. */
	int CopyStackFrom(cLuaState & a_SrcLuaState, int a_SrcStart, int a_SrcEnd);
	
	/** Reads the value at the specified stack position as a string and sets it to a_String. */
	void ToString(int a_StackPos, AString & a_String);

	/** Logs all the elements' types on the API stack, with an optional header for the listing. */
	void LogStack(const char * a_Header = nullptr);
	
	/** Logs all the elements' types on the API stack, with an optional header for the listing. */
	static void LogStack(lua_State * a_LuaState, const char * a_Header = nullptr);
	
protected:

	lua_State * m_LuaState;
	
	/** If true, the state is owned by this object and will be auto-Closed. False => attached state */
	bool m_IsOwned;
	
	/** The subsystem name is used for reporting errors to the console, it is either "plugin %s" or "LuaScript"
	whatever is given to the constructor
	*/
	AString m_SubsystemName;
	
	/** Name of the currently pushed function (for the Push / Call chain) */
	AString m_CurrentFunctionName;
	
	/** Number of arguments currently pushed (for the Push / Call chain) */
	int m_NumCurrentFunctionArgs;

	/** Variadic template terminator: If there's nothing more to push / pop, just call the function.
	Note that there are no return values either, because those are prefixed by a cRet value, so the arg list is never empty. */
	bool PushCallPop(void)
	{
		return CallFunction(0);
	}

	/** Variadic template recursor: More params to push. Push them and recurse. */
	template <typename T, typename... Args>
	inline bool PushCallPop(T a_Param, Args &&... args)
	{
		Push(a_Param);
		return PushCallPop(args...);
	}

	/** Variadic template terminator: If there's nothing more to push, but return values to collect, call the function and collect the returns. */
	template <typename... Args>
	bool PushCallPop(cLuaState::cRet, Args &&... args)
	{
		// Calculate the number of return values (number of args left):
		int NumReturns = sizeof...(args);

		// Call the function:
		if (!CallFunction(NumReturns))
		{
			return false;
		}

		// Collect the return values:
		GetStackValues(-NumReturns, args...);
		lua_pop(m_LuaState, NumReturns);

		// All successful:
		return true;
	}

	/** Variadic template terminator: If there are no more values to get, bail out.
	This function is not available in the public API, because it's an error to request no values directly; only internal functions can do that.
	If you get a compile error saying this function is not accessible, check your calling code, you aren't reading any stack values. */
	void GetStackValues(int a_StartingStackPos)
	{
		// Do nothing
	}

	/** Pushes the function of the specified name onto the stack.
	Returns true if successful. Logs a warning on failure (incl. m_SubsystemName)
	*/
	bool PushFunction(const char * a_FunctionName);
	
	/** Pushes a function that has been saved into the global registry, identified by a_FnRef.
	Returns true if successful. Logs a warning on failure
	*/
	bool PushFunction(int a_FnRef);
	
	/** Pushes a function that has been saved as a reference.
	Returns true if successful. Logs a warning on failure
	*/
	bool PushFunction(const cRef & a_FnRef)
	{
		return PushFunction((int)a_FnRef);
	}
	
	/** Pushes a function that is stored in a referenced table by name
	Returns true if successful. Logs a warning on failure
	*/
	bool PushFunction(const cTableRef & a_TableRef);
	
	/** Pushes a usertype of the specified class type onto the stack */
	void PushUserType(void * a_Object, const char * a_Type);

	/**
	Calls the function that has been pushed onto the stack by PushFunction(),
	with arguments pushed by PushXXX().
	Returns true if successful, logs a warning on failure.
	*/
	bool CallFunction(int a_NumReturnValues);
	
	/** Used as the error reporting function for function calls */
	static int ReportFnCallErrors(lua_State * a_LuaState);
} ;




