
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

	volatile bool mShouldTerminate;  // The overriden Execute() method should check this periodically and terminate if this is true
	
public:
	cIsThread(const AString & iThreadName);
	~cIsThread();
	
	bool Start(void);  // Starts the thread
	bool Wait(void);   // Waits for the thread to finish
	
private:
	AString mThreadName;
	
	#ifdef _WIN32
	
		HANDLE mHandle;
		
		static DWORD_PTR __stdcall thrExecute(LPVOID iParam)
		{
			((cIsThread *)iParam)->Execute();
			return 0;
		}
		
	#else  // _WIN32
	
		pthread_t mHandle;
		bool      mHasStarted;
		
		static void * thrExecute(void * iParam)
		{
			((cIsThread *)iParam)->Execute();
			return NULL;
		}
		
	#endif  // else _WIN32
	
} ;





#endif  // CISTHREAD_H_INCLUDED




