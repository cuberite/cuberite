
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





void cIsThread::Start(void)
{
	// Initialize the thread:
	m_Thread = std::thread(&cIsThread::Entrypoint, this);

	// Notify the thread that initialization is complete and it can run its code safely:
	m_Initialisation.Set();
}





void cIsThread::Stop(void)
{
	m_ShouldTerminate = true;
	{
		LOGD("Waiting for the %s thread to finish", m_ThreadName.c_str());
		if (m_Thread.joinable())
		{
			m_Thread.join();
		}
		LOGD("The %s thread finished", m_ThreadName.c_str());
	}
	m_ShouldTerminate = false;
}





void cIsThread::Entrypoint(void)
{
	// Apply thread naming:
	SetThreadName();

	// Wait for initialisation:
	m_Initialisation.Wait();

	try
	{
		Execute();
	}
	catch (const std::exception & Oops)
	{
		LOGERROR("Thread %s faulted with standard exception: %s", m_ThreadName.c_str(), Oops.what());
		std::abort();
	}
	catch (...)
	{
		LOGERROR("Thread %s faulted with unknown exception!", m_ThreadName.c_str());
		std::abort();
	}
}





void cIsThread::SetThreadName() const
{
#if defined(_MSC_VER) && !defined(NDEBUG)
	/* Sets the name of this thread.
	(When in MSVC, the debugger provides "thread naming" by catching special exceptions)
	Code adapted from MSDN: https://msdn.microsoft.com/en-us/library/xcb2z8hs.aspx */

	if (m_ThreadName.empty())
	{
		return;
	}

#pragma pack(push, 8)
	struct THREADNAME_INFO
	{
		DWORD  dwType;      // Must be 0x1000.
		LPCSTR szName;      // Pointer to name (in user addr space).
		DWORD  dwThreadID;  // Thread ID (-1 = caller thread).
		DWORD  dwFlags;     // Reserved for future use, must be zero.
	};
#pragma pack(pop)

	const DWORD NAME_EXCEPTION = 0x406D1388;
	const THREADNAME_INFO Name = { 0x1000, m_ThreadName.c_str(), -1, 0 };

	__try
	{
		RaiseException(NAME_EXCEPTION, 0, sizeof(Name) / sizeof(ULONG_PTR), reinterpret_cast<const ULONG_PTR *>(&Name));
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
	}
#endif
}
