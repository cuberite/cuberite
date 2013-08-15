
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
	~cIsThread();
	
	/// Starts the thread; returns without waiting for the actual start
	bool Start(void);
	
	/// Signals the thread to terminate and waits until it's finished
	void Stop(void);
	
	/// Waits for the thread to finish. Doesn't signalize the ShouldTerminate flag
	bool Wait(void);
	
	/// Returns the OS-dependent thread ID for the caller's thread
	static unsigned long GetCurrentID(void);

private:
	AString m_ThreadName;
	
	#ifdef _WIN32
	
		HANDLE m_Handle;
		
		static DWORD_PTR __stdcall thrExecute(LPVOID a_Param)
		{
			HWND IdentificationWnd = CreateWindow("STATIC", ((cIsThread *)a_Param)->m_ThreadName.c_str(), 0, 0, 0, 0, WS_OVERLAPPED, NULL, NULL, NULL, NULL);
			((cIsThread *)a_Param)->Execute();
			DestroyWindow(IdentificationWnd);
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




