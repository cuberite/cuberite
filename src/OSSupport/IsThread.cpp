
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

// Code adapted from MSDN: http://msdn.microsoft.com/en-us/library/xcb2z8hs.aspx

const DWORD MS_VC_EXCEPTION = 0x406D1388;

#pragma pack(push, 8)
typedef struct tagTHREADNAME_INFO
{
	DWORD  dwType;      // Must be 0x1000.
	LPCSTR szName;      // Pointer to name (in user addr space).
	DWORD  dwThreadID;  // Thread ID (-1 = caller thread).
	DWORD  dwFlags;     // Reserved for future use, must be zero.
} THREADNAME_INFO;
#pragma pack(pop)

static void SetThreadName(DWORD dwThreadID, const char * threadName)
{
	THREADNAME_INFO info;
	info.dwType = 0x1000;
	info.szName = threadName;
	info.dwThreadID = dwThreadID;
	info.dwFlags = 0;

	__try
	{
		RaiseException(MS_VC_EXCEPTION, 0, sizeof(info) / sizeof(ULONG_PTR), (ULONG_PTR *)&info);
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
	}
}
#endif  // _MSC_VER && _DEBUG





////////////////////////////////////////////////////////////////////////////////
// cIsThread:

cIsThread::cIsThread(const AString & iThreadName) :
	m_ShouldTerminate(false),
	m_ThreadName(iThreadName),
	#ifdef _WIN32
		m_ThreadID(0),
	#endif
	m_Handle(NULL_HANDLE)
{
}





cIsThread::~cIsThread()
{
	m_ShouldTerminate = true;
	Wait();
}





bool cIsThread::Start(void)
{
	ASSERT(m_Handle == NULL_HANDLE);  // Has already started one thread?
	#ifdef _WIN32
		// Create the thread suspended, so that the mHandle variable is valid in the thread procedure
		m_ThreadID = 0;
		m_Handle = CreateThread(NULL, 0, thrExecute, this, CREATE_SUSPENDED, &m_ThreadID);
		if (m_Handle == NULL)
		{
			LOGERROR("ERROR: Could not create thread \"%s\", GLE = %u!", m_ThreadName.c_str(), (unsigned)GetLastError());
			return false;
		}
		ResumeThread(m_Handle);

		#if defined(_DEBUG) && defined(_MSC_VER)
			// Thread naming is available only in MSVC
			if (!m_ThreadName.empty())
			{
				SetThreadName(m_ThreadID, m_ThreadName.c_str());
			}
		#endif  // _DEBUG and _MSC_VER
		
	#else  // _WIN32
		if (pthread_create(&m_Handle, NULL, thrExecute, this))
		{
			LOGERROR("ERROR: Could not create thread \"%s\", !", m_ThreadName.c_str());
			return false;
		}
	#endif  // else _WIN32

	return true;
}





void cIsThread::Stop(void)
{
	if (m_Handle == NULL_HANDLE)
	{
		return;
	}
	m_ShouldTerminate = true;
	Wait();
}





bool cIsThread::Wait(void)
{
	if (m_Handle == NULL_HANDLE)
	{
		return true;
	}
	
	#ifdef LOGD  // ProtoProxy doesn't have LOGD
		LOGD("Waiting for thread %s to finish", m_ThreadName.c_str());
	#endif  // LOGD
	
	#ifdef _WIN32
		int res = WaitForSingleObject(m_Handle, INFINITE);
		m_Handle = NULL;
		
		#ifdef LOGD  // ProtoProxy doesn't have LOGD
			LOGD("Thread %s finished", m_ThreadName.c_str());
		#endif  // LOGD
		
		return (res == WAIT_OBJECT_0);
	#else  // _WIN32
		int res = pthread_join(m_Handle, NULL);
		m_Handle = NULL_HANDLE;
		
		#ifdef LOGD  // ProtoProxy doesn't have LOGD
			LOGD("Thread %s finished", m_ThreadName.c_str());
		#endif  // LOGD
		
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




bool cIsThread::IsCurrentThread(void) const
{
	#ifdef _WIN32
		return (GetCurrentThreadId() == m_ThreadID);
	#else
		return (m_Handle == pthread_self());
	#endif
}




