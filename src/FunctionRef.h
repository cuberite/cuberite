
#pragma once

// Declared only so it can be partially specialized
template <class Signature>
class cFunctionRef;

/** A light-weight, type-erased reference to a function object.
This is similar to a std::function but doesn't copy the function object
which means that mutable function objects will be modified for the caller
but would not be if using a std::function (See #3990 for implications of this).
A cFunctionRef has no empty state but is non-owning and so is safe to call
as long as the referred object is still alive. */
template <class Ret, class... Args>
class cFunctionRef<Ret(Args...)>
{
public:
	/** Construct from a function object. */
	template <class FunctionObject>
	cFunctionRef(FunctionObject && a_FunctionObject)
	{
		// Store an opaque reference to the object.
		m_CallableData = &a_FunctionObject;

		// Along with a function that knows how to call the object.
		m_CallFunction = &ObjectFunctionCaller<FunctionObject>;
	}

	/** Call the referenced function object */
	Ret operator () (Args... a_Args)
	{
		return m_CallFunction(m_CallableData, std::forward<Args>(a_Args)...);
	}

private:

	/** Function that performs the call. */
	template <class ObjectType>
	static Ret ObjectFunctionCaller(void * a_Callable, Args...  a_Args)
	{
		// Convert opaque reference to the concrete type.
		using ObjectPtr = typename std::add_pointer<ObjectType>::type;
		auto & Object = *static_cast<ObjectPtr>(a_Callable);

		// Forward the call down to the object.
		return Object(std::forward<Args>(a_Args)...);
	}

	using cCallFunction = Ret(*)(void *, Args...);

	/** Type erased reference to a callable. */
	void * m_CallableData;

	/** Function that knows how to call the type erased reference. */
	cCallFunction m_CallFunction;
};


