#pragma once

namespace cpp17
{

struct nullopt_t {};
extern nullopt_t nullopt;


/** Cheap imitation of std::optional<T>

	Provides the interface of std::optional but doesn't end the lifetime of the
	held object when !has_value(). Instead, it just sets it to the default
	value.
*/
template <typename T>
class optional
{
public:
	optional():
		m_IsEngaged(false)
	{
	}

	optional(nullopt_t):
		optional()
	{
	}

	optional(T value):
		m_Value(std::move(value)),
		m_IsEngaged(true)
	{
	}

	void operator = (T value)
	{
		emplace(std::move(value));
	}

	void operator = (nullopt_t)
	{
		reset();
	}

	bool has_value() const
	{
		return m_IsEngaged;
	}

	explicit operator bool() const
	{
		return has_value();
	}

	T & value()
	{
		ASSERT(m_IsEngaged);
		return m_Value;
	}

	const T & value() const
	{
		ASSERT(m_IsEngaged);
		return m_Value;
	}

	T * operator ->() { return &value(); }
	const T * operator ->() const { return &value(); }

	T & operator *() { return value(); }
	const T & operator *() const { return value(); }

	template <typename U>
	T value_or(U && default_value) const&
	{
		if (m_IsEngaged)
		{
			return m_Value;
		}

		return T(std::forward<U>(default_value));
	}

	template <typename U>
	T value_or(U && default_value) &&
	{
		if (m_IsEngaged)
		{
			return std::move(m_Value);
		}

		return T(std::forward<U>(default_value));
	}

	void swap(optional & a_Other) noexcept
	{
		std::swap(m_Value, a_Other.m_Value);
		std::swap(m_IsEngaged, a_Other.m_IsEngaged);
	}

	friend void swap(optional & a_Lhs, optional & a_Rhs) noexcept
	{
		a_Lhs.swap(a_Rhs);
	}

	void reset()
	{
		m_Value = T();
		m_IsEngaged = false;
	}

	template <typename... Args>
	T & emplace(Args && ... args)
	{
		m_Value = T(std::forward<Args>(args)...);
		m_IsEngaged = true;
	}


private:
	T m_Value;
	bool m_IsEngaged;

};

}
