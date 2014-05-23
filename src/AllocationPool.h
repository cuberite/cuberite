
#pragma once

#include <memory>

template<class T, size_t BufferSize>
class cAllocationPool {
	public:

		class cStarvationCallbacks
		{
			public:
				virtual ~cStarvationCallbacks() {}
				virtual void OnStartingUsingBuffer() = 0;
				virtual void OnStopUsingBuffer() = 0;
				virtual void OnBufferEmpty() = 0;
		};
		
		cAllocationPool(std::auto_ptr<cStarvationCallbacks> a_Callbacks) :
		m_Callbacks(a_Callbacks)
		{
		}
		
		~cAllocationPool()
		{
			while (!m_FreeList.empty())
			{
				free (m_FreeList.front());
				m_FreeList.pop_front();
			}
		}	
		
		T* Allocate()
		{
			if (m_FreeList.size() <= BufferSize)
			{
				try
				{
					return new(malloc(sizeof(T))) T;
				}
				catch (std::bad_alloc&)
				{
					if (m_FreeList.size() == BufferSize)
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
			}
			// placement new, used to initalize the object
			T* ret = new (m_FreeList.front()) T;
			m_FreeList.pop_front();
			return ret;
		}
		void Free(T* ptr)
		{
			// placement destruct.
			ptr->~T();
			m_FreeList.push_front(ptr);
			if (m_FreeList.size() == BufferSize)
			{
				m_Callbacks->OnStopUsingBuffer();
			}
		}
		
	private:
		std::list<void *> m_FreeList;
		std::auto_ptr<cStarvationCallbacks> m_Callbacks;
};
