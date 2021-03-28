
// IsThread.h

// Interfaces to the cIsThread class representing an OS-independent wrapper for a class that implements a thread.

/*
Usage:
To have a new thread, declare a class descending from cIsThread.
Then override its Execute() method to provide your thread processing.
In the descending class' constructor call the Start() method to start the thread once you're finished with initialization.
*/





#pragma once





class cIsThread
{
public:

	cIsThread(AString && a_ThreadName);
	virtual ~cIsThread();

	/** Starts the thread; returns without waiting for the actual start. */
	void Start(void);

	/** Signals the thread to terminate and waits until it's finished. */
	void Stop(void);

	/** Returns true if the thread calling this function is the thread contained within this object. */
	bool IsCurrentThread(void) const { return std::this_thread::get_id() == m_Thread.get_id(); }

protected:

	/** This function, overloaded by the descendants, is called in the new thread. */
	virtual void Execute(void) = 0;

	/** The overriden Execute() method should check this value periodically and terminate if this is true. */
	std::atomic<bool> m_ShouldTerminate;

private:

	/** The thread object which holds the created thread for later manipulation */
	std::thread m_Thread;

	/** The name of the thread, used to aid debugging in IDEs which support named threads */
	AString m_ThreadName;

	/** The event that is used to wait with the thread's execution until the thread object is fully initialized.
	This prevents the IsCurrentThread() call to fail because of a race-condition where the thread starts before m_Thread has been fully assigned. */
	cEvent m_Initialisation;

	/** This is the main thread entrypoint.
	Wrapper for Execute() that waits for the initialization event, to prevent race conditions in thread initialization. */
	void Entrypoint(void);

	/** Sets the name of the current thread to be the name provided in m_ThreadName. */
	void SetThreadName() const;
} ;
