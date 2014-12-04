
// IsThread.cpp

// Implements the cIsThread class representing an OS-independent wrapper for a class that implements a thread.
// This class will eventually suupersede the old cThread class

#include "Globals.h"
#include "IsThread.h"





#if defined(_MSC_VER) && defined(_DEBUG)
	// Code adapted from MSDN: http://msdn.microsoft.com/en-us/library/xcb2z8hs.aspx

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





bool cIsThread::Start(void)
{
	try
	{
		m_Thread = std::thread(&cIsThread::Execute, this);

		#if defined (_MSC_VER) && defined(_DEBUG)
		if (!m_ThreadName.empty())
		{
			SetThreadName(&m_Thread, m_ThreadName.c_str());
		}
		#endif

		return true;
	}
	catch (std::system_error & a_Exception)
	{
		LOGERROR("cIsThread::Wait (std::thread) error %i: could not construct thread %s; %s", a_Exception.code().value(), m_ThreadName.c_str(), a_Exception.what());
		return false;
	}
}





void cIsThread::Stop(void)
{
	m_ShouldTerminate = true;
	Wait();
}





bool cIsThread::Wait(void)
{
	#ifdef LOGD  // ProtoProxy doesn't have LOGD
		LOGD("Waiting for thread %s to finish", m_ThreadName.c_str());
	#endif  // LOGD

	if (m_Thread.joinable())
	{
		try
		{
			m_Thread.join();
			return true;
		}
		catch (std::system_error & a_Exception)
		{
			LOGERROR("cIsThread::Wait (std::thread) error %i: could not join thread %s; %s", a_Exception.code().value(), m_ThreadName.c_str(), a_Exception.what());
			return false;
		}
	}

	#ifdef LOGD  // ProtoProxy doesn't have LOGD
		LOGD("Thread %s finished", m_ThreadName.c_str());
	#endif

	return true;
}




