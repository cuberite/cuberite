

#pragma once


/** An RAII wrapper for std::atomic<T*>. */
template <typename T>
class cAtomicUniquePtr
{
public:
	static_assert(!std::is_array<T>::value, "cAtomicUniquePtr does not support arrays");
	DISALLOW_COPY_AND_ASSIGN(cAtomicUniquePtr);

	cAtomicUniquePtr() noexcept:
		m_Ptr(nullptr)
	{
	}


	cAtomicUniquePtr(std::unique_ptr<T> a_Ptr) noexcept:
		m_Ptr(a_Ptr.release())
	{
	}

	cAtomicUniquePtr & operator = (std::unique_ptr<T> a_Ptr) noexcept
	{
		store(std::move(a_Ptr));
		return *this;
	}

	~cAtomicUniquePtr() noexcept
	{
		delete load();
	}

	operator T * () const noexcept
	{
		return load();
	}

	bool compare_exchange_weak(T *& a_Expected, std::unique_ptr<T> && a_Desired, std::memory_order a_Order = std::memory_order_seq_cst) noexcept
	{
		bool DidExchange = m_Ptr.compare_exchange_weak(a_Expected, a_Desired.get(), a_Order);
		if (DidExchange)
		{
			// Only release ownership from the caller if the exchange occurred
			a_Desired.release();
		}
		return DidExchange;
	}

	bool compare_exchange_strong(T *& a_Expected, std::unique_ptr<T> && a_Desired, std::memory_order a_Order = std::memory_order_seq_cst) noexcept
	{
		bool DidExchange = m_Ptr.compare_exchange_strong(a_Expected, a_Desired.get(), a_Order);
		if (DidExchange)
		{
			// Only release ownership from the caller if the exchange occurred
			a_Desired.release();
		}
		return DidExchange;
	}

	std::unique_ptr<T> exchange(std::unique_ptr<T> a_Ptr, std::memory_order a_Order = std::memory_order_seq_cst) noexcept
	{
		return std::unique_ptr<T>{ m_Ptr.exchange(a_Ptr.release(), a_Order) };
	}

	T * load(std::memory_order a_Order = std::memory_order_seq_cst) const noexcept
	{
		return m_Ptr.load(a_Order);
	}

	void store(std::unique_ptr<T> a_Ptr, std::memory_order a_Order = std::memory_order_seq_cst) noexcept
	{
		// Store new value and delete old value
		delete m_Ptr.exchange(a_Ptr.release(), a_Order);
	}

private:
	std::atomic<T*> m_Ptr;
};
