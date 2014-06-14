
#pragma once

#include <memory>

template<class T>
class cAllocationPool
{
public:
	class cStarvationCallbacks
	{
		public:
			virtual ~cStarvationCallbacks() {}
			virtual void OnStartingUsingBuffer() = 0;
			virtual void OnStopUsingBuffer() = 0;
			virtual void OnBufferEmpty() = 0;
	};
	
	virtual ~cAllocationPool() {}
	
	/** Allocates a pointer to T **/
	virtual T * Allocate() = 0;
	
	/** Frees the pointer passed in a_ptr, invalidating it **/
	virtual void Free(T * a_ptr) = 0;
};

/** Allocates memory storing unused elements in a linked list. Keeps at least NumElementsInReserve
    elements in the list unless malloc fails so that the program has a reserve to handle OOM.**/
template<class T, size_t NumElementsInReserve>
class cListAllocationPool : public cAllocationPool<T>
{
	public:
		
		cListAllocationPool(std::auto_ptr<typename cAllocationPool<T>::cStarvationCallbacks> a_Callbacks) :
			m_Callbacks(a_Callbacks)
		{
			for (size_t i = 0; i < NumElementsInReserve; i++)
			{
				void * space = malloc(sizeof(T));
				if (space == NULL)
				{
					m_Callbacks->OnStartingUsingBuffer();
					break;
				}
				m_FreeList.push_front(space);
			}
		}
		
		virtual ~cListAllocationPool()
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
				if (space != NULL)
				{
					return new(space) T;
				}
				else if (m_FreeList.size() == NumElementsInReserve)
				{
					m_Callbacks->OnStartingUsingBuffer();
				}
				else if (m_FreeList.empty())
				{
					m_Callbacks->OnBufferEmpty();
					// Try again until the memory is avalable
					return Allocate();
				}
			}
			// placement new, used to initalize the object
			T * ret = new (m_FreeList.front()) T;
			m_FreeList.pop_front();
			return ret;
		}
		virtual void Free(T * a_ptr) override
		{
			if (a_ptr == NULL) 
			{
				return;
			}
			// placement destruct.
			a_ptr->~T();
			m_FreeList.push_front(a_ptr);
			if (m_FreeList.size() == NumElementsInReserve)
			{
				m_Callbacks->OnStopUsingBuffer();
			}
		}
		
	private:
		std::list<void *> m_FreeList;
		std::auto_ptr<typename cAllocationPool<T>::cStarvationCallbacks> m_Callbacks;
};
