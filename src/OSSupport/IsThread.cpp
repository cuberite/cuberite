
// IsThread.cpp

// Implements the cIsThread class representing an OS-independent wrapper for a class that implements a thread.

#include "Globals.h"
#include "IsThread.h"





////////////////////////////////////////////////////////////////////////////////
// cIsThread:

cIsThread::cIsThread(AString && a_ThreadName) :
	m_ShouldTerminate(false),
	m_ThreadName(std::move(a_ThreadName))
{
}





cIsThread::~cIsThread()
{
	Stop();
}





void cIsThread::DoExecute(void)
{
#if defined(_MSC_VER) && !defined(NDEBUG)
	/* Sets the name of this thread.
	(When in MSVC, the debugger provides "thread naming" by catching special exceptions)
	Code adapted from MSDN: https://msdn.microsoft.com/en-us/library/xcb2z8hs.aspx */

#pragma pack(push, 8)
	struct THREADNAME_INFO
	{
		DWORD  dwType;      // Must be 0x1000.
		LPCSTR szName;      // Pointer to name (in user addr space).
		DWORD  dwThreadID;  // Thread ID (-1 = caller thread).
		DWORD  dwFlags;     // Reserved for future use, must be zero.
	};
#pragma pack(pop)

	if (!m_ThreadName.empty())
	{
		const DWORD NAME_EXCEPTION = 0x406D1388;
		const THREADNAME_INFO Name = { 0x1000, m_ThreadName.c_str(), -1, 0 };

		__try
		{
			RaiseException(NAME_EXCEPTION, 0, sizeof(Name) / sizeof(ULONG_PTR), reinterpret_cast<const ULONG_PTR *>(&Name));
		}
		__except (EXCEPTION_EXECUTE_HANDLER)
		{
		}
	}
#endif

	m_evtStart.Wait();
	Execute();
}





bool cIsThread::Start(void)
{
	try
	{
		// Initialize the thread:
		m_Thread = std::thread(&cIsThread::DoExecute, this);

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
	m_ShouldTerminate = true;
	Wait();
	m_ShouldTerminate = false;
}





bool cIsThread::Wait(void)
{
	LOGD("Waiting for the %s thread to finish", m_ThreadName.c_str());
	if (m_Thread.joinable())
	{
		try
		{
			m_Thread.join();
			return true;
		}
		catch (const std::system_error & a_Exception)
		{
			LOGERROR("%s error %i: could not join the %s thread; %s", __FUNCTION__, a_Exception.code().value(), m_ThreadName.c_str(), a_Exception.code().message().c_str());
			return false;
		}
	}

	LOGD("The %s thread finished", m_ThreadName.c_str());
	return true;
}
