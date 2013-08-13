
// IsThread.cpp

// Implements the cIsThread class representing an OS-independent wrapper for a class that implements a thread.
// This class will eventually suupersede the old cThread class

#include "Globals.h"

#include "IsThread.h"





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
	m_ThreadName(iThreadName),
	m_ShouldTerminate(false),
	#ifdef _WIN32
		m_Handle(NULL)
	#else  // _WIN32
		m_HasStarted(false)
	#endif  // else _WIN32
{
}





cIsThread::~cIsThread()
{
	m_ShouldTerminate = true;
	Wait();
}





bool cIsThread::Start(void)
{
	#ifdef _WIN32
		ASSERT(m_Handle == NULL);  // Has already started one thread?
		
		// Create the thread suspended, so that the mHandle variable is valid in the thread procedure
		DWORD ThreadID = 0;
		m_Handle = CreateThread(NULL, 0, thrExecute, this, CREATE_SUSPENDED, &ThreadID);
		if (m_Handle == NULL)
		{
			LOGERROR("ERROR: Could not create thread \"%s\", GLE = %d!", m_ThreadName.c_str(), GetLastError());
			return false;
		}
		ResumeThread(m_Handle);

		#if defined(_DEBUG) && defined(_MSC_VER)
			// Thread naming is available only in MSVC
			if (!m_ThreadName.empty())
			{
				SetThreadName(ThreadID, m_ThreadName.c_str());
			}
		#endif  // _DEBUG and _MSC_VER
		
	#else  // _WIN32
		ASSERT(!m_HasStarted);
		
		if (pthread_create(&m_Handle, NULL, thrExecute, this))
		{
			LOGERROR("ERROR: Could not create thread \"%s\", !", m_ThreadName.c_str());
			return false;
		}
		m_HasStarted = true;
	#endif  // else _WIN32

	return true;
}





bool cIsThread::Wait(void)
{
	#ifdef _WIN32
	
		if (m_Handle == NULL)
		{
			return true;
		}
		// Cannot log, logger may already be stopped:
		// LOG("Waiting for thread \"%s\" to terminate.", m_ThreadName.c_str());
		int res = WaitForSingleObject(m_Handle, INFINITE);
		m_Handle = NULL;
		// Cannot log, logger may already be stopped:
		// LOG("Thread \"%s\" %s terminated, GLE = %d", m_ThreadName.c_str(), (res == WAIT_OBJECT_0) ? "" : "not", GetLastError());
		return (res == WAIT_OBJECT_0);
		
	#else  // _WIN32
	
		if (!m_HasStarted)
		{
			return true;
		}
		// Cannot log, logger may already be stopped:
		// LOG("Waiting for thread \"%s\" to terminate.", m_ThreadName.c_str());
		int res = pthread_join(m_Handle, NULL);
		m_HasStarted = false;
		// Cannot log, logger may already be stopped:
		// LOG("Thread \"%s\" %s terminated, errno = %d", m_ThreadName.c_str(), (res == 0) ? "" : "not", errno);
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




