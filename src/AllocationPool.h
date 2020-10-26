
#pragma once





template <class T>
class cAllocationPool
{
public:
	class cStarvationCallbacks
	{
	public:
		virtual ~cStarvationCallbacks() {}

		/** Is called when the reserve buffer starts to be used */
		virtual void OnStartUsingReserve() = 0;

		/** Is called once the reserve buffer has returned to normal size */
		virtual void OnEndUsingReserve() = 0;

		/** Is called when the allocation pool is unable to allocate memory. Will be repeatedly
			called if it does not free sufficient memory */
		virtual void OnOutOfReserve() = 0;
	};

	virtual ~cAllocationPool() {}

	/** Allocates a pointer to T */
	virtual T * Allocate() = 0;

	/** Frees the pointer passed in a_ptr, invalidating it */
	virtual void Free(T * a_ptr) = 0;

	/** Two pools compare equal if memory allocated by one can be freed by the other */
	bool IsEqual(const cAllocationPool & a_Other) const noexcept
	{
		return ((this == &a_Other) || DoIsEqual(a_Other) || a_Other.DoIsEqual(*this));
	}

	friend bool operator == (const cAllocationPool & a_Lhs, const cAllocationPool & a_Rhs)
	{
		return a_Lhs.IsEqual(a_Rhs);
	}

	friend bool operator != (const cAllocationPool & a_Lhs, const cAllocationPool & a_Rhs)
	{
		return !a_Lhs.IsEqual(a_Rhs);
	}

private:
	virtual bool DoIsEqual(const cAllocationPool & a_Other) const noexcept = 0;
};





/** Allocates memory storing unused elements in a linked list. Keeps at least NumElementsInReserve
elements in the list unless malloc fails so that the program has a reserve to handle OOM. */
template <class T>
class cListAllocationPool:
	public cAllocationPool<T>
{
public:

	cListAllocationPool(std::unique_ptr<typename cAllocationPool<T>::cStarvationCallbacks> a_Callbacks, size_t a_MinElementsInReserve, size_t a_MaxElementsInReserve) :
		m_MinElementsInReserve(a_MinElementsInReserve),
		m_MaxElementsInReserve(a_MaxElementsInReserve),
		m_Callbacks(std::move(a_Callbacks))
	{
		for (size_t i = 0; i < m_MinElementsInReserve; i++)
		{
			void * space = malloc(sizeof(T));
			if (space == nullptr)
			{
				m_Callbacks->OnStartUsingReserve();
				break;
			}
			m_FreeList.push_front(space);
		}
	}


	virtual ~cListAllocationPool() override
	{
		while (!m_FreeList.empty())
		{
			free(m_FreeList.front());
			m_FreeList.pop_front();
		}
	}


	virtual T * Allocate() override
	{
		if (m_FreeList.size() <= m_MinElementsInReserve)
		{
			void * space = malloc(sizeof(T));
			if (space != nullptr)
			{
				#if defined(_MSC_VER) && defined(_DEBUG)
					// The debugging-new that is set up using macros in Globals.h doesn't support the placement-new syntax used here.
					// Temporarily disable the macro
					#pragma push_macro("new")
					#undef new
				#endif

				return new(space) T;

				#if defined(_MSC_VER) && defined(_DEBUG)
					// Re-enable the debugging-new macro
					#pragma pop_macro("new")
				#endif
			}
			else if (m_FreeList.size() == m_MinElementsInReserve)
			{
				m_Callbacks->OnStartUsingReserve();
			}
			else if (m_FreeList.empty())
			{
				m_Callbacks->OnOutOfReserve();
				// Try again until the memory is avalable
				return Allocate();
			}
		}
		// placement new, used to initalize the object

		#if defined(_MSC_VER) && defined(_DEBUG)
			// The debugging-new that is set up using macros in Globals.h doesn't support the placement-new syntax used here.
			// Temporarily disable the macro
			#pragma push_macro("new")
			#undef new
		#endif

		T * ret = new (m_FreeList.front()) T;

		#if defined(_MSC_VER) && defined(_DEBUG)
			// Re-enable the debugging-new macro
			#pragma pop_macro("new")
		#endif

		m_FreeList.pop_front();
		return ret;
	}


	virtual void Free(T * a_ptr) override
	{
		if (a_ptr == nullptr)
		{
			return;
		}

		a_ptr->~T();  // placement destruct.

		if (m_FreeList.size() >= m_MaxElementsInReserve)
		{
			free(a_ptr);
			return;
		}

		m_FreeList.push_front(a_ptr);
		if (m_FreeList.size() == m_MinElementsInReserve)
		{
			m_Callbacks->OnEndUsingReserve();
		}
	}

private:
	/** The minimum number of elements to keep in the free list before malloc fails */
	size_t m_MinElementsInReserve;
	/** Maximum free list size before returning memory to the OS */
	size_t m_MaxElementsInReserve;
	std::list<void *> m_FreeList;
	std::unique_ptr<typename cAllocationPool<T>::cStarvationCallbacks> m_Callbacks;

	virtual bool DoIsEqual(const cAllocationPool<T> & a_Other) const noexcept override
	{
		return (dynamic_cast<const cListAllocationPool<T>*>(&a_Other) != nullptr);
	}
};




