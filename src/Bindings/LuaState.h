
// LuaState.h

// Declares the cLuaState class representing the wrapper over lua_State *, provides associated helper functions

/*
The contained lua_State can be either owned or attached.
Owned lua_State is created by calling Create() and the cLuaState automatically closes the state
Or, lua_State can be attached by calling Attach(), the cLuaState doesn't close such a state
If owning a state, trying to attach a state will automatically close the previously owned state.

Calling a Lua function is done internally by pushing the function using PushFunction(), then pushing the
arguments and finally executing CallFunction(). cLuaState automatically keeps track of the number of
arguments and the name of the function (for logging purposes). After the call the return values are read from
the stack using GetStackValue(). All of this is wrapped in a templated function overloads cLuaState::Call().

Reference management is provided by the cLuaState::cRef class. This is used when you need to hold a reference to
any Lua object across several function calls. The class is RAII-like, with automatic resource management. Note
that the cRef object is not inherently thread-safe and is not notified when its cLuaState is closed. For those
purposes, cTrackedRef can be used.

Callbacks management is provided by the cLuaState::cCallback class. Use a GetStackValue() with cCallbackPtr
parameter to store the callback, and then at any time you can use the cCallback's Call() templated function
to call the callback. The callback management takes care of cLuaState being destroyed - the cCallback object
stays valid but doesn't call into Lua code anymore, returning false for "failure" instead.
*/




#pragma once

extern "C"
{
	#include "lua/src/lauxlib.h"
}


#include <functional>

#include "../Defines.h"
#include "../FunctionRef.h"
#include "PluginManager.h"
#include "LuaState_Typedefs.inc"

// fwd:
class cLuaServerHandle;
class cLuaTCPLink;
class cLuaUDPEndpoint;
class cDeadlockDetect;





/** Encapsulates a Lua state and provides some syntactic sugar for common operations */
class cLuaState
{
public:

	#ifdef _DEBUG
		/** Asserts that the Lua stack has the same amount of entries when this object is destructed, as when it was constructed.
		Used for checking functions that should preserve Lua stack balance. */
		class cStackBalanceCheck
		{
		public:
			cStackBalanceCheck(const char * a_FileName, int a_LineNum, lua_State * a_LuaState, bool a_ShouldLogStack = true):
				m_FileName(a_FileName),
				m_LineNum(a_LineNum),
				m_LuaState(a_LuaState),
				m_StackPos(lua_gettop(a_LuaState))
			{
				if (a_ShouldLogStack)
				{
					// DEBUG: If an unbalanced stack is reported, uncommenting the next line can help debug the imbalance
					// cLuaState::LogStackValues(a_LuaState, Printf("Started checking Lua stack balance, currently %d items:", m_StackPos).c_str());
					// Since LogStackValues() itself uses the balance check, we must not call it recursively
				}
			}

			~cStackBalanceCheck() noexcept(false)
			{
				auto currStackPos = lua_gettop(m_LuaState);
				if (currStackPos != m_StackPos)
				{
					LOGD("Lua stack not balanced. Expected %d items, found %d items. Stack watching started in %s:%d",
						m_StackPos, currStackPos,
						m_FileName.c_str(), m_LineNum
					);
					cLuaState::LogStackValues(m_LuaState);
					ASSERT(!"Lua stack unbalanced");  // If this assert fires, the Lua stack is inbalanced, check the callstack / m_FileName / m_LineNum
				}
			}

		protected:
			const AString m_FileName;
			int m_LineNum;
			lua_State * m_LuaState;
			int m_StackPos;
		};

		#define STRINGIFY2(X, Y) X##Y
		#define STRINGIFY(X, Y) STRINGIFY2(X, Y)
		#define ASSERT_LUA_STACK_BALANCE(...) cStackBalanceCheck STRINGIFY(Check, __COUNTER__)(__FILE__, __LINE__, __VA_ARGS__)
	#else
		#define ASSERT_LUA_STACK_BALANCE(...)
	#endif


	/** Makes sure that the Lua state's stack has the same number of elements on destruction as it had on construction of this object (RAII).
	Can only remove elements, if there are less than expected, throws. */
	class cStackBalancePopper
	{
	public:
		cStackBalancePopper(cLuaState & a_LuaState):
			m_LuaState(a_LuaState),
			m_Count(lua_gettop(a_LuaState))
		{
		}

		~cStackBalancePopper() noexcept(false)
		{
			auto curTop = lua_gettop(m_LuaState);
			if (curTop > m_Count)
			{
				// There are some leftover elements, adjust the stack:
				m_LuaState.LogStackValues(Printf("Re-balancing Lua stack, expected %d values, got %d:", m_Count, curTop).c_str());
				lua_pop(m_LuaState, curTop - m_Count);
			}
			else if (curTop < m_Count)
			{
				// This is an irrecoverable error, rather than letting the Lua engine crash undefinedly later on, abort now:
				LOGERROR("Unable to re-balance Lua stack, there are elements missing. Expected at least %d elements, got %d.", m_Count, curTop);
				throw std::runtime_error(Printf("Unable to re-balance Lua stack, there are elements missing. Expected at least %d elements, got %d.", m_Count, curTop));
			}
		}

	protected:
		cLuaState & m_LuaState;
		int m_Count;
	};


	/** Provides a RAII-style locking for the LuaState.
	Used mainly by the cPluginLua internals to provide the actual locking for interface operations, such as callbacks. */
	class cLock
	{
	public:
		cLock(cLuaState & a_LuaState):
			m_Lock(a_LuaState.m_CS)
		{
		}
	protected:
		cCSLock m_Lock;
	};


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
		explicit operator int(void) const { return m_Ref; }

		/** Returns the Lua state associated with the value. */
		lua_State * GetLuaState(void) { return m_LuaState; }

		/** Creates a Lua reference to the specified object instance in the specified Lua state.
		This is useful to make anti-GC references for objects that were created by Lua and need to stay alive longer than Lua GC would normally guarantee. */
		template <typename T> void CreateFromObject(cLuaState & a_LuaState, T && a_Object)
		{
			a_LuaState.Push(std::forward<T>(a_Object));
			RefStack(a_LuaState, -1);
			a_LuaState.Pop();
		}

	protected:
		lua_State * m_LuaState;
		int m_Ref;

		// Remove the copy-constructor:
		cRef(const cRef &) = delete;
	} ;


	/** Represents a reference to a Lua object that has a tracked lifetime -
	- when the Lua state to which it belongs is closed, the object is kept alive, but invalidated.
	Is thread-safe and unload-safe.
	To receive the cTrackedRef instance from the Lua side, use RefStack() or (better) cLuaState::GetStackValue().
	Note that instances of this class are tracked in the canon LuaState instance, so that
	they can be invalidated when the LuaState is unloaded; due to multithreading issues they can only be tracked
	by-ptr, which has an unfortunate effect of disabling the copy and move constructors. */
	class cTrackedRef
	{
		friend class ::cLuaState;
	public:
		/** Creates an unbound ref instance. */
		cTrackedRef(void);

		~cTrackedRef()
		{
			Clear();
		}

		/** Set the contained reference to the object at the specified Lua state's stack position.
		If another reference has been previously contained, it is Clear()-ed first. */
		bool RefStack(cLuaState & a_LuaState, int a_StackPos);

		/** Frees the contained reference, if any.
		Untracks the reference from its canon Lua state. */
		void Clear(void);

		/** Returns true if the contained reference is valid.
		(Note that depending on this value is not thread-safe, another thread may invalidate the ref in the meantime. It is meant for quick ASSERTs only). */
		bool IsValid(void);

		/** Returns true if the reference resides in the specified Lua state.
		Internally, compares the reference's canon Lua state.
		(Note that depending on this value is not thread-safe, another thread may modify the ref in the meantime. It is meant for quick ASSERTs only). */
		bool IsSameLuaState(cLuaState & a_LuaState);

	protected:
		friend class cLuaState;

		/** The mutex protecting m_Ref against multithreaded access.
		Actually points to the canon Lua state's m_CriticalSection.
		Is nullptr when ref is empty (not bound). */
		std::atomic<cCriticalSection *> m_CS;

		/** Reference to the Lua callback */
		cRef m_Ref;


		/** Invalidates the callback, without untracking it from the cLuaState.
		Called only from cLuaState when closing the Lua state. */
		void Invalidate(void);

		/** Returns the internal reference.
		Only to be used when the cLuaState's CS is held and the cLuaState is known to be valid. */
		cRef & GetRef() { return m_Ref; }

		/** This class cannot be copied, because it is tracked in the LuaState by-ptr.
		Use a smart pointer for a copyable object. */
		cTrackedRef(const cTrackedRef &) = delete;

		/** This class cannot be moved, because it is tracked in the LuaState by-ptr.
		Use a smart pointer for a copyable object. */
		cTrackedRef(cTrackedRef &&) = delete;
	};
	typedef std::unique_ptr<cTrackedRef> cTrackedRefPtr;
	typedef std::shared_ptr<cTrackedRef> cTrackedRefSharedPtr;


	/** Represents a stored callback to Lua that C++ code can call.
	Is thread-safe and unload-safe.
	When the Lua state is unloaded, the callback returns an error instead of calling into non-existent code.
	To receive the callback instance from the Lua side, use RefStack() or (better) cLuaState::GetStackValue()
	with a cCallbackPtr. Note that instances of this class are tracked in the canon LuaState instance, so that
	they can be invalidated when the LuaState is unloaded; due to multithreading issues they can only be tracked
	by-ptr, which has an unfortunate effect of disabling the copy and move constructors. */
	class cCallback:
		public cTrackedRef
	{
		using Super = cTrackedRef;

	public:

		cCallback(void) {}

		/** Calls the Lua callback, if still available.
		Returns true if callback has been called.
		Returns false if the Lua state isn't valid anymore. */
		template <typename... Args>
		bool Call(Args &&... args)
		{
			auto cs = m_CS.load();
			if (cs == nullptr)
			{
				return false;
			}
			cCSLock Lock(*cs);
			if (!m_Ref.IsValid())
			{
				return false;
			}
			return cLuaState(m_Ref.GetLuaState()).Call(m_Ref, std::forward<Args>(args)...);
		}

		/** Set the contained callback to the function in the specified Lua state's stack position.
		If a callback has been previously contained, it is unreferenced first.
		Returns true on success, false on failure (not a function at the specified stack pos). */
		bool RefStack(cLuaState & a_LuaState, int a_StackPos);

	protected:

		/** This class cannot be copied, because it is tracked in the LuaState by-ptr.
		Use cCallbackPtr for a copyable object. */
		cCallback(const cCallback &) = delete;

		/** This class cannot be moved, because it is tracked in the LuaState by-ptr.
		Use cCallbackPtr for a copyable object. */
		cCallback(cCallback &&) = delete;
	};
	typedef std::unique_ptr<cCallback> cCallbackPtr;
	typedef std::shared_ptr<cCallback> cCallbackSharedPtr;


	/** Same thing as cCallback, but GetStackValue() won't fail if the callback value is nil.
	Used for callbacks that are optional - they needn't be present and in such a case they won't get called. */
	class cOptionalCallback:
		public cCallback
	{
		using Super = cCallback;

	public:

		cOptionalCallback(void) {}

		/** Set the contained callback to the function in the specified Lua state's stack position.
		If a callback has been previously contained, it is unreferenced first.
		Returns true on success, false on failure (not a function at the specified stack pos). */
		bool RefStack(cLuaState & a_LuaState, int a_StackPos);

	protected:

		/** This class cannot be copied, because it is tracked in the LuaState by-ptr.
		Use cCallbackPtr for a copyable object. */
		cOptionalCallback(const cOptionalCallback &) = delete;

		/** This class cannot be moved, because it is tracked in the LuaState by-ptr.
		Use cCallbackPtr for a copyable object. */
		cOptionalCallback(cOptionalCallback &&) = delete;
	};
	typedef std::unique_ptr<cOptionalCallback> cOptionalCallbackPtr;


	/** Represents a stored Lua table with callback functions that C++ code can call.
	Is thread-safe and unload-safe.
	When Lua state is unloaded, the CallFn() will return failure instead of calling into non-existent code.
	To receive the cTableRef instance from the Lua side, use RefStack() or (better) cLuaState::GetStackValue()
	with a cTableRefPtr.
	Note that instances of this class are tracked in the canon LuaState instance, so that they can be
	invalidated when the LuaState is unloaded; due to multithreading issues they can only be tracked by-ptr,
	which has an unfortunate effect of disabling the copy and move constructors. */
	class cTableRef:
		public cTrackedRef
	{
		using Super = cTrackedRef;

	public:
		cTableRef(void) {}

		/** Calls the Lua function stored under the specified name in the referenced table, if still available.
		Returns true if callback has been called.
		Returns false if the Lua state isn't valid anymore, or the function doesn't exist. */
		template <typename... Args>
		bool CallTableFn(const char * a_FnName, Args &&... args)
		{
			auto cs = m_CS.load();
			if (cs == nullptr)
			{
				return false;
			}
			cCSLock Lock(*cs);
			if (!m_Ref.IsValid())
			{
				return false;
			}
			return cLuaState(m_Ref.GetLuaState()).CallTableFn(m_Ref, a_FnName, std::forward<Args>(args)...);
		}

		/** Calls the Lua function stored under the specified name in the referenced table, if still available.
		A "self" parameter is injected in front of all the given parameters and is set to the callback table.
		Returns true if callback has been called.
		Returns false if the Lua state isn't valid anymore, or the function doesn't exist. */
		template <typename... Args>
		bool CallTableFnWithSelf(const char * a_FnName, Args &&... args)
		{
			auto cs = m_CS.load();
			if (cs == nullptr)
			{
				return false;
			}
			cCSLock Lock(*cs);
			if (!m_Ref.IsValid())
			{
				return false;
			}
			return cLuaState(m_Ref.GetLuaState()).CallTableFn(m_Ref, a_FnName, m_Ref, std::forward<Args>(args)...);
		}

		/** Set the contained reference to the table in the specified Lua state's stack position.
		If another table has been previously contained, it is unreferenced first.
		Returns true on success, false on failure (not a table at the specified stack pos). */
		bool RefStack(cLuaState & a_LuaState, int a_StackPos);
	};
	typedef std::unique_ptr<cTableRef> cTableRefPtr;


	/** Represents a parameter that is optional - calling a GetStackValue() with this object will not fail if the value on the Lua stack is nil.
	Note that the GetStackValue() will still fail if the param is present but of a different type.
	The class itself is just a marker so that the template magic will select the correct GetStackValue() overload. */
	template <typename T>
	class cOptionalParam
	{
	public:
		explicit cOptionalParam(T & a_Dest):
			m_Dest(a_Dest)
		{
		}

		T & GetDest(void) { return m_Dest; }

	protected:
		T & m_Dest;
	};


	/** A dummy class that's used only to delimit function args from return values for cLuaState::Call() */
	class cRet
	{
	} ;
	static const cRet Return;  // Use this constant to delimit function args from return values for cLuaState::Call()


	/** A dummy class that's used only to push a constant nil as a function parameter in Call(). */
	class cNil
	{
	};
	static const cNil Nil;  // Use this constant to give a function a nil parameter in Call()


	/** A RAII class for values pushed onto the Lua stack.
	Will pop the value off the stack in the destructor. */
	class cStackValue
	{
	public:
		cStackValue(void):
			m_LuaState(nullptr)
		{
		}

		cStackValue(cLuaState & a_LuaState):
			m_LuaState(a_LuaState)
		{
			m_StackLen = lua_gettop(a_LuaState);
		}

		cStackValue(cStackValue && a_Src):
			m_LuaState(nullptr),
			m_StackLen(-1)
		{
			std::swap(m_LuaState, a_Src.m_LuaState);
			std::swap(m_StackLen, a_Src.m_StackLen);
		}

		~cStackValue() noexcept(false)
		{
			if (m_LuaState != nullptr)
			{
				ASSERT(m_StackLen == lua_gettop(m_LuaState));
				lua_pop(m_LuaState, 1);
			}
		}

		void Set(cLuaState & a_LuaState)
		{
			m_LuaState = a_LuaState;
			m_StackLen = lua_gettop(a_LuaState);
		}

		bool IsValid(void) const
		{
			return (m_LuaState != nullptr);
		}

	protected:
		lua_State * m_LuaState;

		/** Used for debugging, Lua state's stack size is checked against this number to make sure
		it is the same as when the value was pushed. */
		int m_StackLen;

		// Remove the copy-constructor:
		cStackValue(const cStackValue &) = delete;
	};


	/** Represents a table on the Lua stack.
	Provides easy access to the elements in the table.
	Note that this class doesn't have cTrackedRef's thread-safeness and unload-safeness, it is expected to be
	used for immediate queries in API bindings. */
	class cStackTable
	{
	public:
		cStackTable(cLuaState & a_LuaState, int a_StackPos);

		/** Iterates over all array elements in the table consecutively and calls the a_ElementCallback for each.
		The callback receives the LuaState in which the table resides, and the element's index. The LuaState has
		the element on top of its stack. If the callback returns true, the iteration is aborted, if it returns
		false, the iteration continues with the next element. */
		void ForEachArrayElement(cFunctionRef<bool(cLuaState & a_LuaState, int a_Index)> a_ElementCallback) const;

		/** Iterates over all dictionary elements in the table in random order, and calls the a_ElementCallback for
		each of them.
		The callback receives the LuaState in which the table reside. The LuaState has the element on top of its
		stack, and the element's key just below it. If the callback returns true, the iteration is aborted, if it
		returns false, the iteration continues with the next element. */
		void ForEachElement(cFunctionRef<bool(cLuaState & a_LuaState)> a_ElementCallback) const;

		cLuaState & GetLuaState(void) const { return m_LuaState; }

	protected:
		/** The Lua state in which the table resides. */
		cLuaState & m_LuaState;

		/** The stack index where the table resides in the Lua state. */
		int m_StackPos;
	};
	typedef std::unique_ptr<cStackTable> cStackTablePtr;


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

	/** Creates the m_LuaState, if not created already.
	This state will be automatically closed in the destructor.
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

	/** Returns the name of the subsystem, as specified when the instance was created. */
	AString GetSubsystemName(void) const { return m_SubsystemName; }

	/** Adds the specified path to package.<a_PathVariable> */
	void AddPackagePath(const AString & a_PathVariable, const AString & a_Path);

	/** Loads the specified file
	Returns false and optionally logs a warning to the console if not successful (but the LuaState is kept open).
	m_SubsystemName is displayed in the warning log message. */
	bool LoadFile(const AString & a_FileName, bool a_LogWarnings = true);

	/** Loads the specified string.
	Returns false and optionally logs a warning to the console if not successful (but the LuaState is kept open).
	a_FileName is the original filename from where the string was read, and is used only for logging. It may be empty.
	m_SubsystemName is displayed in the warning log message. */
	bool LoadString(const AString & a_StringToLoad, const AString & a_FileName, bool a_LogWarnings = true);

	/** Returns true if a_FunctionName is a valid Lua function that can be called */
	bool HasFunction(const char * a_FunctionName);

	/** Pushes multiple arguments onto the Lua stack. */
	template <typename Arg1, typename Arg2, typename... Args>
	void Push(Arg1 && a_Arg1, Arg2 && a_Arg2, Args &&... a_Args)
	{
		Push(std::forward<Arg1>(a_Arg1));
		Push(std::forward<Arg2>(a_Arg2), std::forward<Args>(a_Args)...);
	}

	// Push a const value onto the stack (keep alpha-sorted):
	// Note that these functions will make a copy of the actual value, because Lua doesn't have the concept
	// of "const", and there would be lifetime management problems if they didn't.
	void Push(const AString & a_String);
	void Push(const AStringMap & a_Dictionary);
	void Push(const AStringVector & a_Vector);
	void Push(const char * a_Value);
	void Push(const cItem & a_Item);
	void Push(const cNil & a_Nil);
	void Push(const cRef & a_Ref);
	void Push(const Vector3d & a_Vector);
	void Push(const Vector3i & a_Vector);

	// Push a simple value onto the stack (keep alpha-sorted):
	void Push(bool a_Value);
	void Push(cEntity * a_Entity);
	void Push(const cEntity * a_Entity);
	void Push(cLuaServerHandle * a_ServerHandle);
	void Push(cLuaTCPLink * a_TCPLink);
	void Push(cLuaUDPEndpoint * a_UDPEndpoint);
	void Push(double a_Value);
	void Push(int a_Value);
	void Push(long a_Value);
	void Push(const UInt32 a_Value);
	void Push(std::chrono::milliseconds a_time);

	/** Pops the specified number of values off the top of the Lua stack. */
	void Pop(int a_NumValuesToPop = 1);

	// GetStackValue() retrieves the value at a_StackPos, if it is a valid type. If not, a_Value is unchanged.
	// Returns whether value was changed
	// Enum values are checked for their allowed values and fail if the value is not assigned.
	bool GetStackValue(int a_StackPos, AString & a_Value);
	bool GetStackValue(int a_StackPos, AStringMap & a_Value);
	bool GetStackValue(int a_StackPos, AStringVector & a_Value);
	bool GetStackValue(int a_StackPos, bool & a_Value);
	bool GetStackValue(int a_StackPos, cCallback & a_Callback);
	bool GetStackValue(int a_StackPos, cCallbackPtr & a_Callback);
	bool GetStackValue(int a_StackPos, cCallbackSharedPtr & a_Callback);
	bool GetStackValue(int a_StackPos, cOptionalCallback & a_Callback);
	bool GetStackValue(int a_StackPos, cOptionalCallbackPtr & a_Callback);
	bool GetStackValue(int a_StackPos, cPluginManager::CommandResult & a_Result);
	bool GetStackValue(int a_StackPos, cRef & a_Ref);
	bool GetStackValue(int a_StackPos, cStackTablePtr & a_StackTable);
	bool GetStackValue(int a_StackPos, cTableRef & a_TableRef);
	bool GetStackValue(int a_StackPos, cTableRefPtr & a_TableRef);
	bool GetStackValue(int a_StackPos, cTrackedRef & a_Ref);
	bool GetStackValue(int a_StackPos, cTrackedRefPtr & a_Ref);
	bool GetStackValue(int a_StackPos, cTrackedRefSharedPtr & a_Ref);
	bool GetStackValue(int a_StackPos, double & a_Value);
	bool GetStackValue(int a_StackPos, eBlockFace & a_Value);
	bool GetStackValue(int a_StackPos, eWeather & a_Value);
	bool GetStackValue(int a_StackPos, float & a_ReturnedVal);
	bool GetStackValue(int a_StackPos, cUUID & a_Value);

	// template to catch all of the various c++ integral types without overload conflicts
	template <class T>
	bool GetStackValue(int a_StackPos, T & a_ReturnedVal, typename std::enable_if<std::is_integral<T>::value>::type * unused = nullptr)
	{
		UNUSED(unused);
		if (!lua_isnumber(m_LuaState, a_StackPos))  // Also accepts strings representing a number: https://pgl.yoyo.org/luai/i/lua_isnumber
		{
			return false;
		}
		lua_Number Val = lua_tonumber(m_LuaState, a_StackPos);
		if (Val > std::numeric_limits<T>::max())
		{
			return false;
		}
		if (Val < std::numeric_limits<T>::min())
		{
			return false;
		}
		a_ReturnedVal = static_cast<T>(Val);
		return true;
	}

	/** Retrieves an optional value on the stack - doesn't fail if the stack contains nil instead of the value. */
	template <typename T>
	bool GetStackValue(int a_StackPos, cOptionalParam<T> && a_ReturnedVal)
	{
		if (lua_isnoneornil(m_LuaState, a_StackPos))
		{
			return true;
		}
		return GetStackValue(a_StackPos, a_ReturnedVal.GetDest());
	}

	/** Retrieves any Vector3 value and coerces it into a Vector3<T>. */
	template <typename T>
	bool GetStackValue(int a_StackPos, Vector3<T> & a_ReturnedVal);

	/** Pushes the named value in the table at the top of the stack.
	a_Name may be a path containing multiple table levels, such as "cChatColor.Blue".
	If the value is found, it is pushed on top of the stack and the returned cStackValue is valid.
	If the value is not found, the stack is unchanged and the returned cStackValue is invalid. */
	cStackValue WalkToValue(const AString & a_Name);

	/** Pushes the named value in the global table to the top of the stack.
	a_Name may be a path containing multiple table levels, such as "cChatColor.Blue".
	If the value is found in the global table, it is pushed to the top of the stack and the returned cStackValue is valid.
	If the value is not found, the stack is unchanged and the returned cStackValue is invalid. */
	cStackValue WalkToNamedGlobal(const AString & a_Name);

	/** Retrieves the named value in the table at the top of the Lua stack.
	a_Name may be a path containing multiple table levels, such as "_G.cChatColor.Blue".
	Returns true if the value was successfully retrieved, false on error. */
	template <typename T> bool GetNamedValue(const AString & a_Name, T & a_Value)
	{
		auto stk = WalkToValue(a_Name);
		if (!stk.IsValid())
		{
			// Name not found
			return false;
		}
		return GetStackValue(-1, a_Value);
	}

	/** Retrieves the named global value. a_Name may be a path containing multiple table levels, such as "_G.cChatColor.Blue".
	Returns true if the value was successfully retrieved, false on error. */
	template <typename T> bool GetNamedGlobal(const AString & a_Name, T & a_Value)
	{
		// Push the globals table onto the stack and make it RAII-removed:
		lua_getglobal(m_LuaState, "_G");
		cStackValue stk(*this);

		// Get the named global:
		return GetNamedValue(a_Name, a_Value);
	}

	// Include the auto-generated Push and GetStackValue() functions:
	#include "LuaState_Declaration.inc"

	/** Call the specified Lua function.
	Returns true if call succeeded, false if there was an error.
	A special param of cRet & signifies the end of param list and the start of return values.
	Example call: Call(Fn, Param1, Param2, Param3, cLuaState::Return, Ret1, Ret2) */
	template <typename FnT, typename... Args>
	bool Call(const FnT & a_Function, Args &&... args)
	{
		cStackBalancePopper balancer(*this);
		m_NumCurrentFunctionArgs = -1;
		if (!PushFunction(std::forward<const FnT &>(a_Function)))
		{
			// Pushing the function failed
			return false;
		}
		auto res = PushCallPop(std::forward<Args>(args)...);
		return res;
	}

	/** Retrieves a list of values from the Lua stack, starting at the specified index. */
	template <typename Arg1, typename... Args>
	inline bool GetStackValues(int a_StartStackPos, Arg1 && a_Arg1, Args &&... args)
	{
		if (!GetStackValue(a_StartStackPos, std::forward<Arg1>(a_Arg1)))
		{
			return false;
		}
		return GetStackValues(a_StartStackPos + 1, std::forward<Args>(args)...);
	}

	/** Returns true if the specified parameters on the stack are of the specified usertable type; also logs warning if not. Used for static functions */
	bool CheckParamUserTable(int a_StartParam, const char * a_UserTable, int a_EndParam = -1);

	/** Returns true if the specified parameters on the stack are of the specified usertype; also logs warning if not. Used for regular functions */
	bool CheckParamUserType(int a_StartParam, const char * a_UserType, int a_EndParam = -1);

	/** Returns true if the specified parameters on the stack are tables; also logs warning if not */
	bool CheckParamTable(int a_StartParam, int a_EndParam = -1);

	/** Returns true if the specified parameters on the stack are numbers; also logs warning if not */
	bool CheckParamNumber(int a_StartParam, int a_EndParam = -1);

	/** Returns true if the specified parameters on the stack are bools; also logs warning if not */
	bool CheckParamBool(int a_StartParam, int a_EndParam = -1);

	/** Returns true if the specified parameters on the stack are strings; also logs warning if not */
	bool CheckParamString(int a_StartParam, int a_EndParam = -1);

	/** Returns true if the specified parameters on the stack are functions; also logs warning if not */
	bool CheckParamFunction(int a_StartParam, int a_EndParam = -1);

	/** Returns true if the specified parameters on the stack are functions or nils; also logs warning if not */
	bool CheckParamFunctionOrNil(int a_StartParam, int a_EndParam = -1);

	/** Returns true if the specified parameters on the stack are UUIDs; also logs warning if not
	Accepts either cUUID instances or strings that contain UUIDs */
	bool CheckParamUUID(int a_StartParam, int a_EndParam = -1);

	/** Returns true if the specified parameters on the stack are Vector3s; also logs warning if not.
	Accepts any Vector3 type instances or tables. */
	bool CheckParamVector3(int a_StartParam, int a_EndParam = -1);

	/** Returns true if the specified parameter on the stack is nil (indicating an end-of-parameters) */
	bool CheckParamEnd(int a_Param);

	/** Returns true if the first parameter is an instance of the expected class name.
	Returns false and logs a special warning ("wrong calling convention") if not. */
	bool CheckParamSelf(const char * a_SelfClassName);

	/** Returns true if the first parameter is the expected class (static).
	Returns false and logs a special warning ("wrong calling convention") if not. */
	bool CheckParamStaticSelf(const char * a_SelfClassName);

	/** Returns true if the specified parameter is of the specified class. */
	bool IsParamUserType(int a_ParamIdx, const AString & a_UserType);

	/** Returns true if the specified parameter is a number. */
	bool IsParamNumber(int a_ParamIdx);

	/** Returns true if the specified parameter is any of the Vector3 types. */
	bool IsParamVector3(int a_ParamIdx);

	/** If the status is nonzero, prints the text on the top of Lua stack and returns true */
	bool ReportErrors(int status);

	/** If the status is nonzero, prints the text on the top of Lua stack and returns true */
	static bool ReportErrors(lua_State * a_LuaState, int status);

	/** Logs all items in the current stack trace to the server console */
	void LogStackTrace(int a_StartingDepth = 0);

	/** Logs all items in the current stack trace to the server console */
	static void LogStackTrace(lua_State * a_LuaState, int a_StartingDepth = 0);

	/** Prints the message, prefixed with the current function name, then logs the stack contents and raises a Lua error.
	To be used for bindings when they detect bad parameters.
	Doesn't return, but a dummy return type is provided so that Lua API functions may do "return ApiParamError(...)". */
	int ApiParamError(std::string_view a_Msg);

	/** Formats and prints the message using printf-style format specifiers, but prefixed with the current function name, then logs the stack contents and raises a Lua error.
	To be used for bindings when they detect bad parameters.
	Doesn't return, but a dummy return type is provided so that Lua API functions may do "return ApiParamError(...)". */
	template <typename... Args>
	int ApiParamError(const char * a_MsgFormat, const Args & ... a_Args)
	{
		return ApiParamError(Printf(a_MsgFormat, a_Args...));
	}

	/** Formats and prints the message using python-style format specifiers, but prefixed with the current function name, then logs the stack contents and raises a Lua error.
	To be used for bindings when they detect bad parameters.
	Doesn't return, but a dummy return type is provided so that Lua API functions may do "return ApiParamError(...)". */
	template <typename... Args>
	int FApiParamError(const char * a_MsgFormat, const Args & ... a_Args)
	{
		return ApiParamError(fmt::format(a_MsgFormat, a_Args...));
	}


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
	Only numbers, bools, strings, API classes and simple tables containing these (recursively) are copied.
	a_NumAllowedNestingLevels specifies how many table nesting levels are allowed, copying fails if there's a deeper table.
	If successful, returns the number of objects copied.
	If failed, returns a negative number and rewinds the stack position. */
	int CopyStackFrom(cLuaState & a_SrcLuaState, int a_SrcStart, int a_SrcEnd, int a_NumAllowedNestingLevels = 16);

	/** Copies a table at the specified stack index of the source Lua state to the top of this Lua state's stack.
	a_NumAllowedNestingLevels specifies how many table nesting levels are allowed, copying fails if there's a deeper table.
	Returns true if successful, false on failure.
	Can copy only simple values - numbers, bools, strings and recursively simple tables. */
	bool CopyTableFrom(cLuaState & a_SrcLuaState, int a_TableIdx, int a_NumAllowedNestingLevels);

	/** Copies a single value from the specified stack index of the source Lua state to the top of this Lua state's stack.
	a_NumAllowedNestingLevels specifies how many table nesting levels are allowed, copying fails if there's a deeper table.
	Returns true if the value was copied, false on failure. */
	bool CopySingleValueFrom(cLuaState & a_SrcLuaState, int a_StackIdx, int a_NumAllowedNestingLevels);

	/** Reads the value at the specified stack position as a string and sets it to a_String. */
	void ToString(int a_StackPos, AString & a_String);

	/** Logs all the elements' types on the API stack, with an optional header for the listing. */
	void LogStackValues(const char * a_Header = nullptr);

	/** Logs all the elements' types on the API stack, with an optional header for the listing. */
	static void LogStackValues(lua_State * a_LuaState, const char * a_Header = nullptr);

	/** Returns the canon Lua state (the primary cLuaState instance that was used to create, rather than attach, the lua_State structure).
	Returns nullptr if the canon Lua state cannot be queried. */
	cLuaState * QueryCanonLuaState(void) const;

	/** Outputs to log a warning about API call being unable to read its parameters from the stack,
	logs the stack trace and stack values. */
	void LogApiCallParamFailure(const char * a_FnName, const char * a_ParamNames);

	/** Adds this object's CS to the DeadlockDetect's tracked CSs. */
	void TrackInDeadlockDetect(cDeadlockDetect & a_DeadlockDetect);

	/** Removes this object's CS from the DeadlockDetect's tracked CSs. */
	void UntrackInDeadlockDetect(cDeadlockDetect & a_DeadlockDetect);

protected:

	cCriticalSection m_CS;

	lua_State * m_LuaState;

	/** If true, the state is owned by this object and will be auto-Closed. False => attached state */
	bool m_IsOwned;

	/** The subsystem name is used for reporting errors to the console, it is either "plugin %s" or "LuaScript"
	whatever is given to the constructor. */
	AString m_SubsystemName;

	/** Name of the currently pushed function (for the Push / Call chain) */
	AString m_CurrentFunctionName;

	/** Number of arguments currently pushed (for the Push / Call chain) */
	int m_NumCurrentFunctionArgs;

	/** The tracked references.
	The cLuaState will invalidate all of these when it is about to be closed.
	Protected against multithreaded access by m_CSTrackedRefs. */
	std::vector<cTrackedRef *> m_TrackedRefs;

	/** Protects m_TrackedRefs against multithreaded access. */
	cCriticalSection m_CSTrackedRefs;


	/** Call the Lua function specified by name in the table stored as a reference.
	Returns true if call succeeded, false if there was an error (not a table ref, function name not found).
	A special param of cRet & signifies the end of param list and the start of return values.
	Example call: CallTableFn(TableRef, "FnName", Param1, Param2, Param3, cLuaState::Return, Ret1, Ret2) */
	template <typename... Args>
	bool CallTableFn(const cRef & a_TableRef, const char * a_FnName, Args &&... args)
	{
		if (!PushFunction(a_TableRef, a_FnName))
		{
			// Pushing the function failed
			return false;
		}
		return PushCallPop(std::forward<Args>(args)...);
	}

	/** Variadic template terminator: If there's nothing more to push / pop, just call the function.
	Note that there are no return values either, because those are prefixed by a cRet value, so the arg list is never empty. */
	bool PushCallPop(void)
	{
		return CallFunction(0);
	}

	/** Variadic template recursor: More params to push. Push them and recurse. */
	template <typename T, typename... Args>
	inline bool PushCallPop(T && a_Param, Args &&... args)
	{
		Push(std::forward<T>(a_Param));
		m_NumCurrentFunctionArgs += 1;
		return PushCallPop(std::forward<Args>(args)...);
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
		GetStackValues(-NumReturns, std::forward<Args>(args)...);
		lua_pop(m_LuaState, NumReturns);

		// All successful:
		return true;
	}

	/** Variadic template terminator: If there are no more values to get, bail out.
	This function is not available in the public API, because it's an error to request no values directly; only internal functions can do that.
	If you get a compile error saying this function is not accessible, check your calling code, you aren't reading any stack values. */
	bool GetStackValues(int a_StartingStackPos)
	{
		return true;
	}

	/** Pushes the function of the specified name onto the stack.
	Returns true if successful. Logs a warning on failure (incl. m_SubsystemName)
	*/
	bool PushFunction(const char * a_FunctionName);

	/** Pushes a function that has been saved as a reference.
	Returns true if successful. Logs a warning on failure
	*/
	bool PushFunction(const cRef & a_FnRef);

	/** Pushes a function that is stored under the specified name in a table that has been saved as a reference.
	Returns true if successful. */
	bool PushFunction(const cRef & a_TableRef, const char * a_FnName);

	/** Pushes a usertype of the specified class type onto the stack */
	// void PushUserType(void * a_Object, const char * a_Type);

	/**
	Calls the function that has been pushed onto the stack by PushFunction(),
	with arguments pushed by PushXXX().
	Returns true if successful, returns false and logs a warning on failure.
	Pops the function params, the function itself and the error handler off the stack.
	If successful, leaves a_NumReturnValues new values on Lua stack, corresponding to the return values.
	On failure, leaves no new values on the Lua stack.
	*/
	bool CallFunction(int a_NumReturnValues);

	/** Used as the error reporting function for function calls */
	static int ReportFnCallErrors(lua_State * a_LuaState);

	/** Tries to break into the MobDebug debugger, if it is installed. */
	static int BreakIntoDebugger(lua_State * a_LuaState);

	/** Adds the specified reference to tracking.
	The reference will be invalidated when this Lua state is about to be closed. */
	void TrackRef(cTrackedRef & a_Ref);

	/** Removes the specified reference from tracking.
	The reference will no longer be invalidated when this Lua state is about to be closed. */
	void UntrackRef(cTrackedRef & a_Ref);
};  // cLuaState

// Instantiate the GetStackValue(Vector3<>) function for all Vector3 types:
extern template bool cLuaState::GetStackValue(int a_StackPos, Vector3d & a_ReturnedVal);
extern template bool cLuaState::GetStackValue(int a_StackPos, Vector3f & a_ReturnedVal);
extern template bool cLuaState::GetStackValue(int a_StackPos, Vector3i & a_ReturnedVal);





/** Keeps track of all create cLuaState instances.
Can query each for the amount of currently used memory. */
class cLuaStateTracker
{
public:
	/** Adds the specified Lua state to the internal list of LuaStates. */
	static void Add(cLuaState & a_LuaState);

	/** Deletes the specified Lua state from the internal list of LuaStates. */
	static void Del(cLuaState & a_LuaState);

	/** Returns the statistics for all the registered LuaStates. */
	static AString GetStats(void);

protected:
	typedef cLuaState * cLuaStatePtr;
	typedef std::vector<cLuaStatePtr> cLuaStatePtrs;

	/** The internal list of LuaStates.
	Protected against multithreaded access by m_CSLuaStates. */
	cLuaStatePtrs m_LuaStates;

	/** Protects m_LuaStates against multithreaded access. */
	cCriticalSection m_CSLuaStates;


	/** Returns the single instance of this class. */
	static cLuaStateTracker & Get(void);
};




