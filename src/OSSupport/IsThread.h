
// IsThread.h

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
	/// This is the main thread entrypoint
	virtual void Execute(void) = 0;

	/// The overriden Execute() method should check this value periodically and terminate if this is true
	volatile bool m_ShouldTerminate;
	
public:
	cIsThread(const AString & iThreadName);
	virtual ~cIsThread();
	
	/// Starts the thread; returns without waiting for the actual start
	bool Start(void);
	
	/// Signals the thread to terminate and waits until it's finished
	void Stop(void);
	
	/// Waits for the thread to finish. Doesn't signalize the ShouldTerminate flag
	bool Wait(void);
	
	/// Returns the OS-dependent thread ID for the caller's thread
	static unsigned long GetCurrentID(void);

	/** Returns true if the thread calling this function is the thread contained within this object. */
	bool IsCurrentThread(void) const;

protected:
	AString m_ThreadName;
	
	// Value used for "no handle":
	#ifdef _WIN32
		#define NULL_HANDLE NULL
	#else
		#define NULL_HANDLE 0
	#endif

	#ifdef _WIN32
	
		DWORD m_ThreadID;
		HANDLE m_Handle;
		
		static DWORD __stdcall thrExecute(LPVOID a_Param)
		{
			// Create a window so that the thread can be identified by 3rd party tools:
			HWND IdentificationWnd = CreateWindowA("STATIC", ((cIsThread *)a_Param)->m_ThreadName.c_str(), 0, 0, 0, 0, WS_OVERLAPPED, NULL, NULL, NULL, NULL);
			
			// Run the thread:
			((cIsThread *)a_Param)->Execute();
			
			// Destroy the identification window:
			DestroyWindow(IdentificationWnd);
			
			return 0;
		}
		
	#else  // _WIN32
	
		pthread_t m_Handle;
		
		static void * thrExecute(void * a_Param)
		{
			((cIsThread *)a_Param)->Execute();
			return NULL;
		}
		
	#endif  // else _WIN32
} ;





#endif  // CISTHREAD_H_INCLUDED




