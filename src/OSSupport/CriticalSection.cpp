
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules
#include "IsThread.h"





////////////////////////////////////////////////////////////////////////////////
// cCriticalSection:

cCriticalSection::cCriticalSection()
{
	#ifdef _WIN32
		InitializeCriticalSection(&m_CriticalSection);
	#else
		pthread_mutexattr_init(&m_Attributes);
		pthread_mutexattr_settype(&m_Attributes, PTHREAD_MUTEX_RECURSIVE);

		if (pthread_mutex_init(&m_CriticalSection, &m_Attributes) != 0)
		{
			LOGERROR("Could not initialize Critical Section!");
		}
	#endif
	
	#ifdef _DEBUG
		m_IsLocked = 0;
	#endif  // _DEBUG
}





cCriticalSection::~cCriticalSection()
{
	#ifdef _WIN32
		DeleteCriticalSection(&m_CriticalSection);
	#else
		if (pthread_mutex_destroy(&m_CriticalSection) != 0)
		{
			LOGWARNING("Could not destroy Critical Section!");
		}
		pthread_mutexattr_destroy(&m_Attributes);
	#endif
}





void cCriticalSection::Lock()
{
	#ifdef _WIN32
		EnterCriticalSection(&m_CriticalSection);
	#else
		pthread_mutex_lock(&m_CriticalSection);
	#endif
	
	#ifdef _DEBUG
		m_IsLocked += 1;
		m_OwningThreadID = cIsThread::GetCurrentID();
	#endif  // _DEBUG
}





void cCriticalSection::Unlock()
{
	#ifdef _DEBUG
		ASSERT(m_IsLocked > 0);
		m_IsLocked -= 1;
	#endif  // _DEBUG
	
	#ifdef _WIN32
		LeaveCriticalSection(&m_CriticalSection);
	#else
		pthread_mutex_unlock(&m_CriticalSection);
	#endif
}





#ifdef _DEBUG
bool cCriticalSection::IsLocked(void)
{
	return (m_IsLocked > 0);
}





bool cCriticalSection::IsLockedByCurrentThread(void)
{
	return ((m_IsLocked > 0) && (m_OwningThreadID == cIsThread::GetCurrentID()));
}
#endif  // _DEBUG





////////////////////////////////////////////////////////////////////////////////
// cCSLock

cCSLock::cCSLock(cCriticalSection * a_CS)
	: m_CS(a_CS)
	, m_IsLocked(false)
{
	Lock();
}





cCSLock::cCSLock(cCriticalSection & a_CS)
	: m_CS(&a_CS)
	, m_IsLocked(false)
{
	Lock();
}





cCSLock::~cCSLock()
{
	if (!m_IsLocked)
	{
		return;
	}
	Unlock();
}





void cCSLock::Lock(void)
{
	ASSERT(!m_IsLocked);
	m_IsLocked = true;
	m_CS->Lock();
}





void cCSLock::Unlock(void)
{
	ASSERT(m_IsLocked);
	m_IsLocked = false;
	m_CS->Unlock();
}





////////////////////////////////////////////////////////////////////////////////
// cCSUnlock:

cCSUnlock::cCSUnlock(cCSLock & a_Lock) :
	m_Lock(a_Lock)
{
	m_Lock.Unlock();
}





cCSUnlock::~cCSUnlock()
{
	m_Lock.Lock();
}




