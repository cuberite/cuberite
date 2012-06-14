
// cIsThread.h

// Interfaces to the cIsThread class representing an OS-independent wrapper for a class that implements a thread.
// This class will eventually suupersede the old cThread class

/*
Usage:
To have a new thread, declare a class descending from cIsClass.
Then override its Execute() method to provide your thread processing.
In the descending class' constructor call the Start() method to start the thread once you're finished with initialization.
*/





#pragma once
#ifndef CISTHREAD_H_INCLUDED
#define CISTHREAD_H_INCLUDED





class cIsThread
{
protected:
	virtual void Execute(void) = 0;  // This function is called in the new thread's context

	volatile bool m_ShouldTerminate;  // The overriden Execute() method should check this periodically and terminate if this is true
	
public:
	cIsThread(const AString & iThreadName);
	~cIsThread();
	
	bool Start(void);  // Starts the thread
	bool Wait(void);   // Waits for the thread to finish
	
	static unsigned long GetCurrentID(void);  // Returns the OS-dependent thread ID for the caller's thread

private:
	AString m_ThreadName;
	
	#ifdef _WIN32
	
		HANDLE m_Handle;
		
		static DWORD_PTR __stdcall thrExecute(LPVOID a_Param)
		{
			((cIsThread *)a_Param)->Execute();
			return 0;
		}
		
	#else  // _WIN32
	
		pthread_t m_Handle;
		bool      m_HasStarted;
		
		static void * thrExecute(void * a_Param)
		{
			((cIsThread *)a_Param)->Execute();
			return NULL;
		}
		
	#endif  // else _WIN32
	
} ;





#endif  // CISTHREAD_H_INCLUDED




