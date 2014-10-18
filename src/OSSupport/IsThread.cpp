
// IsThread.cpp

// Implements the cIsThread class representing an OS-independent wrapper for a class that implements a thread.
// This class will eventually suupersede the old cThread class

#include "Globals.h"
#include "IsThread.h"





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
		return true;
	}
	catch (std::system_error & a_Exception)
	{
		LOGERROR("ERROR: Could not create thread \"%s\", error = %s!", m_ThreadName.c_str(), a_Exception.code(), a_Exception.what());
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
			LOGERROR("ERROR: Could wait for thread \"%s\" to finish, error = %s!", m_ThreadName.c_str(), a_Exception.code(), a_Exception.what());
			return false;
		}
	}

	#ifdef LOGD  // ProtoProxy doesn't have LOGD
		LOGD("Thread %s finished", m_ThreadName.c_str());
	#endif

	return true;
}




