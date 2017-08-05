
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
};





/** Allocates memory storing unused elements in a linked list. Keeps at least NumElementsInReserve
elements in the list unless malloc fails so that the program has a reserve to handle OOM. */
template <class T, size_t NumElementsInReserve>
class cListAllocationPool:
	public cAllocationPool<T>
{
public:

	cListAllocationPool(std::unique_ptr<typename cAllocationPool<T>::cStarvationCallbacks> a_Callbacks):
		m_Callbacks(std::move(a_Callbacks))
	{
		for (size_t i = 0; i < NumElementsInReserve; i++)
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
			free (m_FreeList.front());
			m_FreeList.pop_front();
		}
	}


	virtual T * Allocate() override
	{
		if (m_FreeList.size() <= NumElementsInReserve)
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
			else if (m_FreeList.size() == NumElementsInReserve)
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
		// placement destruct.
		a_ptr->~T();
		m_FreeList.push_front(a_ptr);
		if (m_FreeList.size() == NumElementsInReserve)
		{
			m_Callbacks->OnEndUsingReserve();
		}
	}

private:
	std::list<void *> m_FreeList;
	std::unique_ptr<typename cAllocationPool<T>::cStarvationCallbacks> m_Callbacks;
};




