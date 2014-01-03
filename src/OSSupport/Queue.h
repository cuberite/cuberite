
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
cQueueFuncs and is used as the default behavior.
*/

// this empty struct allows for the callback functions to be inlined
template<class T>
struct cQueueFuncs 
{
	public:
		// Called when an Item is deleted form the queue without being returned
		static void Delete(T) {};
		// Called when an Item is inserted with EnqueueItemIfNotPresent and
		// there is another equal value already inserted
		static void Combine(T& a_existing, const T& a_new) {};
};

template<class ItemType, class Funcs = cQueueFuncs<ItemType> >
class cQueue
{
// internal typedef for a List of Items
typedef typename std::list<ItemType> ListType;
// magic typedef to persuade clang that the iterator is a type
typedef typename ListType::iterator iterator;
public:
	cQueue() {}
	~cQueue() {}

	// Enqueues an item to the queue, may block if other threads are accessing
	// the queue.
	void     EnqueueItem(ItemType a_item) 
	{
		cCSLock Lock(m_CS);
		m_contents.push_back(a_item);
		m_evtAdded.Set();
	}

	// Enqueues an item to the queue if not already present as determined with
	// operator ==. Will block other threads from accessing the queue.
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

	// Dequeues an Item from the queue if any are present, provides no
	// guarantees about success if the list is empty but an item is enqueued at
	// the same time. Returns true if successful. Value of item is undefined if
	// Dequeuing was unsuccessful.
	bool     TryDequeueItem(ItemType& item)
	{
		cCSLock Lock(m_CS);
		if (m_contents.size() == 0) 
		{
			return false; 
		}
		item = m_contents.front();
		m_contents.pop_front();
		m_evtRemoved.Set();
		return true;
	}

	// Dequeues an Item from the Queue, blocking until an Item is Available.
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

	// Blocks Until the queue is Empty, Has a slight race condition which may
	// cause it to miss the queue being empty.
	void BlockTillEmpty() {
		// There is a very slight race condition here if the load completes between the check
		// and the wait.
		while(!(Size() == 0)){m_evtRemoved.Wait();}
	}

	// Removes all Items from the Queue, calling Delete on each of them.
	// can all be inlined when delete is a noop
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

	// Returns the Size at time of being called
	// Do not use to detirmine weather to call DequeueItem, use TryDequeue instead
	size_t   Size()
	{
		cCSLock Lock(m_CS);
		return m_contents.size();
	}

	// Removes an Item from the queue
	bool     Remove(ItemType item)
	{
		cCSLock Lock(m_CS);
		m_evtRemoved.Set();
		return m_contents.remove(item);
	}

private:
	ListType m_contents;
	cCriticalSection m_CS;
	cEvent m_evtAdded;
	cEvent m_evtRemoved;
};
