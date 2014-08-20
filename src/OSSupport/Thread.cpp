
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules





// When in MSVC, the debugger provides "thread naming" by catching special exceptions. Interface here:
#ifdef _MSC_VER
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
#endif  // _MSC_VER





cThread::cThread( ThreadFunc a_ThreadFunction, void* a_Param, const char* a_ThreadName /* = 0 */)
	: m_ThreadFunction( a_ThreadFunction)
	, m_Param( a_Param)
	, m_Event( new cEvent())
	, m_StopEvent( 0)
{
	if (a_ThreadName)
	{
		m_ThreadName.assign(a_ThreadName);
	}
}





cThread::~cThread()
{
	delete m_Event;
	m_Event = NULL;
	
	if (m_StopEvent)
	{
		m_StopEvent->Wait();
		delete m_StopEvent;
		m_StopEvent = NULL;
	}
}





void cThread::Start( bool a_bWaitOnDelete /* = true */)
{
	if (a_bWaitOnDelete)
		m_StopEvent = new cEvent();

#ifndef _WIN32
	pthread_t SndThread;
	if (pthread_create( &SndThread, NULL, MyThread, this))
		LOGERROR("ERROR: Could not create thread!");
#else
	DWORD ThreadID = 0;
	HANDLE hThread = CreateThread(NULL  // security
		, 0  // stack size
		, (LPTHREAD_START_ROUTINE) MyThread  // function name
		, this  // parameters
		, 0  // flags
		, &ThreadID);  // thread id
	CloseHandle( hThread);

	#ifdef _MSC_VER
	if (!m_ThreadName.empty())
	{
		SetThreadName(ThreadID, m_ThreadName.c_str());
	}
	#endif  // _MSC_VER
#endif

	// Wait until thread has actually been created
	m_Event->Wait();
}





#ifdef _WIN32
unsigned long cThread::MyThread(void* a_Param)
#else
void *cThread::MyThread( void *a_Param)
#endif
{
	cThread* self = (cThread*)a_Param;
	cEvent* StopEvent = self->m_StopEvent;

	ThreadFunc* ThreadFunction = self->m_ThreadFunction;
	void* ThreadParam = self->m_Param;

	// Set event to let other thread know this thread has been created and it's safe to delete the cThread object
	self->m_Event->Set();

	ThreadFunction( ThreadParam);

	if (StopEvent) StopEvent->Set();
	return 0;
}
