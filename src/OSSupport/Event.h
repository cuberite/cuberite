
// Event.h

// Interfaces to the cEvent object representing a synchronization primitive that can be waited-for
// Implemented using C++11 condition variable and mutex





#pragma once





class cEvent
{
public:
	cEvent(void);

	/** Waits until the event has been set.
	If the event has been set before it has been waited for, Wait() returns immediately. */
	void Wait(void);

	/** Sets the event - releases one thread that has been waiting in Wait().
	If there was no thread waiting, the next call to Wait() will not block. */
	void Set(void);

	/** Sets the event - releases all threads that have been waiting in Wait().
	If there was no thread waiting, the next call to Wait() will not block. */
	void SetAll(void);

	/** Waits for the event until either it is signalled, or the (relative) timeout is passed.
	Returns true if the event was signalled, false if the timeout was hit or there was an error. */
	bool Wait(unsigned a_TimeoutMSec);

private:

	/** Used for checking for spurious wakeups. */
	bool m_ShouldContinue;

	/** Mutex protecting m_ShouldContinue from multithreaded access. */
	std::mutex m_Mutex;

	/** The condition variable used as the Event. */
	std::condition_variable m_CondVar;
} ;





