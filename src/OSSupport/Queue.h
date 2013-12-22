#pragma once

template<class T>
class cDeleter 
{
	public:
		static void Delete(T) {};
};

template<class T, class D = cDeleter<T>>
class cQueue
{
public:
	cQueue(int warnsize);
	cQueue(cQueue<T>& queue);
	~cQueue();

	void EnqueueItem(T item);
	bool TryDequeueItem(T& item);
	T    DequeueItem();
	void BlockTillEmpty(cEvent CancelationEvent);
	void Clear();
	int  Size();
	
private:
	int warnsize;
	std::list<T> contents;
};

//template classes must be implemented in the header
#include "Queue.inc"
