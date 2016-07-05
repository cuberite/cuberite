
// Queue.h

// Implements the cQueue class representing a thread safe queue

#pragma once

/*
Items can be added multiple times to a queue, there are two functions for
adding, EnqueueItem() and EnqueueItemIfNotPresent(). The first one always
enqueues the specified item, the second one checks if the item is already
present and only queues it if it isn't.

Usage:
To create a queue of type T, instantiate a cQueue<T> object. You can also
modify the behavior of the queue when deleting items and when adding items
that are already in the queue by providing a second parameter, a class that
implements the functions Delete() and Combine(). An example is given in
cQueueFuncs and is used as the default behavior. */

/** This empty struct allows for the callback functions to be inlined */
template <class T>
struct cQueueFuncs
{
public:

	/** Called when an Item is deleted from the queue without being returned */
	static void Delete(T) {}

	/** Called when an Item is inserted with EnqueueItemIfNotPresent and there is another equal value already inserted */
	static void Combine(T & a_existing, const T & a_new)
	{
		UNUSED(a_existing);
		UNUSED(a_new);
	}
};





template <class ItemType, class Funcs = cQueueFuncs<ItemType> >
class cQueue
{
	// The actual storage type for the queue
	typedef typename std::list<ItemType> QueueType;

	// Make iterator an alias for the QueueType's iterator
	typedef typename QueueType::iterator iterator;

public:
	cQueue() {}
	~cQueue() {}


	/** Enqueues an item to the queue, may block if other threads are accessing the queue. */
	void EnqueueItem(ItemType a_Item)
	{
		cCSLock Lock(m_CS);
		m_Contents.push_back(a_Item);
		m_evtAdded.Set();
	}


	/** Enqueues an item in the queue if not already present (as determined by operator ==). Blocks other threads from accessing the queue. */
	void EnqueueItemIfNotPresent(ItemType a_Item)
	{
		cCSLock Lock(m_CS);

		for (iterator itr = m_Contents.begin(); itr != m_Contents.end(); ++itr)
		{
			if ((*itr) == a_Item)
			{
				Funcs::Combine(*itr, a_Item);
				return;
			}
		}
		m_Contents.push_back(a_Item);
		m_evtAdded.Set();
	}


	/** Dequeues an item from the queue if any are present.
	Returns true if successful. Value of item is undefined if dequeuing was unsuccessful. */
	bool TryDequeueItem(ItemType & item)
	{
		cCSLock Lock(m_CS);
		if (m_Contents.empty())
		{
			return false;
		}
		item = m_Contents.front();
		m_Contents.pop_front();
		m_evtRemoved.Set();
		return true;
	}


	/** Dequeues an item from the queue, blocking until an item is available. */
	ItemType DequeueItem(void)
	{
		cCSLock Lock(m_CS);
		while (m_Contents.empty())
		{
			cCSUnlock Unlock(Lock);
			m_evtAdded.Wait();
		}
		ItemType item = m_Contents.front();
		m_Contents.pop_front();
		m_evtRemoved.Set();
		return item;
	}


	/** Blocks until the queue is empty. */
	void BlockTillEmpty(void)
	{
		cCSLock Lock(m_CS);
		while (!m_Contents.empty())
		{
			cCSUnlock Unlock(Lock);
			m_evtRemoved.Wait();
		}
	}


	/** Removes all Items from the Queue, calling Delete on each of them. */
	void Clear(void)
	{
		cCSLock Lock(m_CS);
		while (!m_Contents.empty())
		{
			Funcs::Delete(m_Contents.front());
			m_Contents.pop_front();
		}
	}


	/** Returns the size at time of being called.
	Do not use to determine whether to call DequeueItem(), use TryDequeueItem() instead */
	size_t Size(void)
	{
		cCSLock Lock(m_CS);
		return m_Contents.size();
	}


	/** Removes the item from the queue. If there are multiple such items, only the first one is removed.
	Returns true if the item has been removed, false if no such item found. */
	bool Remove(ItemType a_Item)
	{
		cCSLock Lock(m_CS);
		for (iterator itr = m_Contents.begin(); itr != m_Contents.end(); ++itr)
		{
			if ((*itr) == a_Item)
			{
				m_Contents.erase(itr);
				m_evtRemoved.Set();
				return true;
			}
		}
		return false;
	}


	/** Removes all items for which the predicate returns true. */
	template <class Predicate>
	void RemoveIf(Predicate a_Predicate)
	{
		cCSLock Lock(m_CS);
		for (auto itr = m_Contents.begin(); itr != m_Contents.end();)
		{
			if (a_Predicate(*itr))
			{
				auto itr2 = itr;
				++itr2;
				m_Contents.erase(itr);
				m_evtRemoved.Set();
				itr = itr2;
			}
			else
			{
				++itr;
			}
		}  // for itr - m_Contents[]
	}

private:
	/** The contents of the queue */
	QueueType m_Contents;

	/** Mutex that protects access to the queue contents */
	cCriticalSection m_CS;

	/** Event that is signalled when an item is added */
	cEvent m_evtAdded;

	/** Event that is signalled when an item is removed (both dequeued or erased) */
	cEvent m_evtRemoved;
};




