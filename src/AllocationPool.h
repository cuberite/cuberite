
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
			T* ret = m_FreeList.front();
			m_FreeList.pop_front();
			return ret;
		}
		void Free(T* ptr)
		{
			m_FreeList.push_front(ptr);
		}
		
	private:
		std::list<T*> m_FreeList;
}
