
// IsThread.cpp

// Implements the cIsThread class representing an OS-independent wrapper for a class that implements a thread.
// This class will eventually suupersede the old cThread class

#include "Globals.h"
#include "IsThread.h"





#if defined(_MSC_VER) && defined(_DEBUG)
	// Code adapted from MSDN: https://msdn.microsoft.com/en-us/library/xcb2z8hs.aspx

	const DWORD MS_VC_EXCEPTION = 0x406D1388;
	#pragma pack(push, 8)
		struct THREADNAME_INFO
		{
			DWORD  dwType;      // Must be 0x1000.
			LPCSTR szName;      // Pointer to name (in user addr space).
			DWORD  dwThreadID;  // Thread ID (-1 = caller thread).
			DWORD  dwFlags;     // Reserved for future use, must be zero.
		};
	#pragma pack(pop)

	/** Sets the name of a thread with the specified ID
	(When in MSVC, the debugger provides "thread naming" by catching special exceptions)
	*/
	static void SetThreadName(std::thread * a_Thread, const char * a_ThreadName)
	{
		THREADNAME_INFO info { 0x1000, a_ThreadName, GetThreadId(a_Thread->native_handle()), 0 };
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

cIsThread::cIsThread(const AString & a_ThreadName) :
	m_ShouldTerminate(false),
	m_ThreadName(a_ThreadName)
{
}





cIsThread::~cIsThread()
{
	m_ShouldTerminate = true;
	Wait();
}





void cIsThread::DoExecute(void)
{
	m_evtStart.Wait();
	Execute();
}





bool cIsThread::Start(void)
{
	try
	{
		// Initialize the thread:
		m_Thread = std::thread(&cIsThread::DoExecute, this);

		#if defined (_MSC_VER) && defined(_DEBUG)
		if (!m_ThreadName.empty())
		{
			SetThreadName(&m_Thread, m_ThreadName.c_str());
		}
		#endif

		// Notify the thread that initialization is complete and it can run its code safely:
		m_evtStart.Set();

		return true;
	}
	catch (const std::system_error & a_Exception)
	{
		LOGERROR("cIsThread::Start error %i: could not construct thread %s; %s", a_Exception.code().value(), m_ThreadName.c_str(), a_Exception.code().message().c_str());
		return false;
	}
}





void cIsThread::Stop(void)
{
	if (!m_Thread.joinable())
	{
		// The thread hasn't been started or has already been joined
		return;
	}

	m_ShouldTerminate = true;
	Wait();
}





bool cIsThread::Wait(void)
{
	LOGD("Waiting for thread %s to finish", m_ThreadName.c_str());
	if (m_Thread.joinable())
	{
		try
		{
			m_Thread.join();
			return true;
		}
		catch (const std::system_error & a_Exception)
		{
			LOGERROR("%s error %i: could not join thread %s; %s", __FUNCTION__, a_Exception.code().value(), m_ThreadName.c_str(), a_Exception.code().message().c_str());
			return false;
		}
	}

	LOGD("Thread %s finished", m_ThreadName.c_str());
	return true;
}
