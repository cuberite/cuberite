
#pragma once

#include <list>

//this empty struct allows function inlining
template<class T>
struct cQueueFuncs 
{
	public:
		static void Delete(T) {};
		static void Combine(T&, const T) {};
};

template<class ItemType, class Funcs = cQueueFuncs<ItemType> >
class cQueue
{

typedef typename std::list<ItemType> ListType;
//magic typedef to persuade clang that the iterator is a type
typedef typename ListType::iterator iterator;
public:
	cQueue() {}
	~cQueue() {}

	void     EnqueueItem(ItemType a_item) 
	{
		cCSLock Lock(m_CS);
		m_contents.push_back(a_item);
		m_evtAdded.Set();
	}
	void     EnqueueItemIfNotPresent(ItemType a_item)
	{
		cCSLock Lock(m_CS);

		for (iterator itr = m_contents.begin(); itr != m_contents.end(); ++itr)
		{
			if((*itr) == a_item) {
				Funcs funcTable;
				funcTable.Combine(*itr,a_item);
				return;
			}
		}
		m_contents.push_back(a_item);
		m_evtAdded.Set();
	}
	bool     TryDequeueItem(ItemType& item)
	{
		cCSLock Lock(m_CS);
		if (m_contents.size() == 0) return false;
		item = m_contents.front();
		m_contents.pop_front();
		m_evtRemoved.Set();
		return true;
	}
	ItemType DequeueItem()
	{
		cCSLock Lock(m_CS);
		while (m_contents.size() == 0) 
		{
			cCSUnlock Unlock(m_CS);
			m_evtAdded.Wait();
		}
		ItemType item = m_contents.front();
		m_contents.pop_front();
		m_evtRemoved.Set();
		return item;
	}
	void BlockTillEmpty() {
		//There is a very slight race condition here if the load completes between the check
		//and the wait.
		while(!(Size() == 0)){m_evtRemoved.Wait();}
	}
	//can all be inlined when delete is a noop
	void     Clear()
	{
		cCSLock Lock(m_CS);
		Funcs funcTable;
		while (!m_contents.empty())
		{
			funcTable.Delete(m_contents.front());
			m_contents.pop_front();
		}
	}
	size_t   Size()
	{
		cCSLock Lock(m_CS);
		return m_contents.size();
	}
	bool     Remove(ItemType item)
	{
		cCSLock Lock(m_CS);
		m_contents.remove(item);
		m_evtRemoved.Set();
	}

private:
	ListType m_contents;
	cCriticalSection m_CS;
	cEvent m_evtAdded;
	cEvent m_evtRemoved;
};
