
// cIsThread.cpp

// Implements the cIsThread class representing an OS-independent wrapper for a class that implements a thread.
// This class will eventually suupersede the old cThread class

#include "Globals.h"

#include "cIsThread.h"





// When in MSVC, the debugger provides "thread naming" by catching special exceptions. Interface here:
#if defined(_MSC_VER) && defined(_DEBUG)
//
// Usage: SetThreadName (-1, "MainThread");
//

static void SetThreadName( DWORD dwThreadID, LPCSTR szThreadName)
{
	struct
	{
		DWORD dwType; // must be 0x1000
		LPCSTR szName; // pointer to name (in user addr space)
		DWORD dwThreadID; // thread ID (-1=caller thread)
		DWORD dwFlags; // reserved for future use, must be zero
	} info;
	
	info.dwType = 0x1000;
	info.szName = szThreadName;
	info.dwThreadID = dwThreadID;
	info.dwFlags = 0;

	__try
	{
		RaiseException(0x406D1388, 0, sizeof(info) / sizeof(DWORD), (DWORD *)&info);
	}
	__except(EXCEPTION_CONTINUE_EXECUTION)
	{
	}
}
#endif  // _MSC_VER && _DEBUG





////////////////////////////////////////////////////////////////////////////////
// cIsThread:

cIsThread::cIsThread(const AString & iThreadName) :
	mThreadName(iThreadName),
	mShouldTerminate(false),
	#ifdef _WIN32
	mHandle(NULL)
	#else  // _WIN32
	mHasStarted(false)
	#endif  // else _WIN32
{
}





cIsThread::~cIsThread()
{
	mShouldTerminate = true;
	Wait();
}





bool cIsThread::Start(void)
{
	#ifdef _WIN32
		assert(mHandle == NULL);  // Has already started one thread?
		
		// Create the thread suspended, so that the mHandle variable is valid in the thread procedure
		DWORD ThreadID = 0;
		mHandle = CreateThread(NULL, 0, thrExecute, this, CREATE_SUSPENDED, &ThreadID);
		if (mHandle == NULL)
		{
			LOGERROR("ERROR: Could not create thread \"%s\", GLE = %d!", mThreadName.c_str(), GetLastError());
			return false;
		}
		ResumeThread(mHandle);

		#if defined(_DEBUG) && defined(_MSC_VER)
			// Thread naming is available only in MSVC
			if (!mThreadName.empty())
			{
				SetThreadName(ThreadID, mThreadName.c_str());
			}
		#endif  // _DEBUG and _MSC_VER
		
	#else  // _WIN32
		assert(!mHasStarted);
		
		if (pthread_create(&mHandle, NULL, thrExecute, this))
		{
			LOGERROR("ERROR: Could not create thread \"%s\", !", mThreadName.c_str());
			return false;
		}
	#endif  // else _WIN32

	return true;
}





bool cIsThread::Wait(void)
{
	#ifdef _WIN32
	
		if (mHandle == NULL)
		{
			return true;
		}
		// Cannot log, logger may already be stopped:
		// LOG("Waiting for thread \"%s\" to terminate.", mThreadName.c_str());
		int res = WaitForSingleObject(mHandle, INFINITE);
		mHandle = NULL;
		// Cannot log, logger may already be stopped:
		// LOG("Thread \"%s\" %s terminated, GLE = %d", mThreadName.c_str(), (res == WAIT_OBJECT_0) ? "" : "not", GetLastError());
		return (res == WAIT_OBJECT_0);
		
	#else  // _WIN32
	
		if (!mHasStarted)
		{
			return true;
		}
		LOG("Waiting for thread \"%s\" to terminate.", mThreadName.c_str());
		int res = pthread_join(mHandle, NULL);
		mHasStarted = false;
		LOG("Thread \"%s\" %s terminated, errno = %d", mThreadName.c_str(), (res == 0) ? "" : "not", errno);
		return (res == 0);
		
	#endif  // else _WIN32
}





unsigned long cIsThread::GetCurrentID(void)
{
	#ifdef _WIN32
		return (unsigned long) GetCurrentThreadId();
	#else
		return (unsigned long) pthread_self();
	#endif
}




