
#pragma once

// Guard against macro new
#pragma push_macro("new")
#undef new

// Declared only so it can be partially specialized
template <class Signature>
class cFunctionRef;

/** A light-weight, type-erased reference to a function object.
Non-owning, be mindful of lifetimes. */
template <class Ret, class... Args>
class cFunctionRef<Ret(Args...)>
{
public:
	/** Construct from a function object. */
	template <class FunctionObject>
	cFunctionRef(FunctionObject && a_FunctionObject)
	{
		static_assert(  // Ensure function object is callable with signature Ret(Args...)
			std::is_convertible<
				decltype(a_FunctionObject(std::forward<Args>(std::declval<Args>())...)),
				Ret
			>::value,
			"Function object does not have a compatible call operator"
		);

		m_CallableData = &a_FunctionObject;
		m_CallFunction = &ObjectFunctionCaller<typename std::remove_reference<FunctionObject>::type>;
	}

	/** Call the referenced function object */
	Ret operator () (Args... a_Args)
	{
		return m_CallFunction(m_CallableData, std::forward<Args>(a_Args)...);
	}

private:

	/** Function that performs the call. */
	template <class T>
	static Ret ObjectFunctionCaller(void * a_Callable, Args...  a_Args)
	{
		auto & Object = *static_cast<T *>(a_Callable);
		return Object(std::forward<Args>(a_Args)...);
	}

	using cCallFunction = Ret(*)(void *, Args...);

	/** Type erased reference to a callable. */
	void * m_CallableData;

	/** Function pointer to call the erased function. */
	cCallFunction m_CallFunction;
};


#pragma pop_macro("new")


