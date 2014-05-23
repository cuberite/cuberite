
#pragma once

template<class T, size_t BufferSize, class StarvationCallbacks>
class AllocationPool {
	public:
		
		~AllocationPool()
		{
			while (!m_FreeList.empty())
			{
				delete m_FreeList.front();
				m_FreeList.pop_front();
			}
		}	
		
		T* Allocate()
		{
			if (m_FreeList.Size() <= BufferSize)
			{
				try
				{
					return new T;
				}
				catch (std::bad_alloc& ex)
				{
					if (m_FreeList.size() == BufferSize)
					{
						StarvationCallbacks.OnStartingUsingBuffer();
					}
					else if (m_FreeList.empty())
					{
						StarvationCallbacks.OnBufferEmpty();
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
				StarvationCallbacks.OnStopUsingBuffer();
			}
		}
		
	private:
		std::list<void *> m_FreeList;
}
