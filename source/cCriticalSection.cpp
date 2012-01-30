
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cCriticalSection:

cCriticalSection::cCriticalSection()
{
#ifdef _WIN32
	m_CriticalSectionPtr = new CRITICAL_SECTION;
	InitializeCriticalSection( (CRITICAL_SECTION*)m_CriticalSectionPtr );
#else
	m_Attributes = new pthread_mutexattr_t;
	pthread_mutexattr_init((pthread_mutexattr_t*)m_Attributes);
	pthread_mutexattr_settype((pthread_mutexattr_t*)m_Attributes, PTHREAD_MUTEX_RECURSIVE);

    m_CriticalSectionPtr = new pthread_mutex_t;
    if( pthread_mutex_init( (pthread_mutex_t*)m_CriticalSectionPtr, (pthread_mutexattr_t*)m_Attributes ) != 0 )
    {
        LOG("ERROR: Could not initialize Critical Section!");
    }
#endif
}





cCriticalSection::~cCriticalSection()
{
#ifdef _WIN32
	DeleteCriticalSection( (CRITICAL_SECTION*)m_CriticalSectionPtr );
	delete (CRITICAL_SECTION*)m_CriticalSectionPtr;
#else
    if( pthread_mutex_destroy( (pthread_mutex_t*)m_CriticalSectionPtr ) != 0 )
    {
        LOG("ERROR: Could not destroy Critical Section!");
    }
    delete (pthread_mutex_t*)m_CriticalSectionPtr;
	pthread_mutexattr_destroy( (pthread_mutexattr_t*)m_Attributes );
	delete (pthread_mutexattr_t*)m_Attributes;
#endif
}





void cCriticalSection::Lock()
{
#ifdef _WIN32
	EnterCriticalSection( (CRITICAL_SECTION*)m_CriticalSectionPtr );
#else
    pthread_mutex_lock( (pthread_mutex_t*)m_CriticalSectionPtr );
#endif
}





void cCriticalSection::Unlock()
{
#ifdef _WIN32
	LeaveCriticalSection( (CRITICAL_SECTION*)m_CriticalSectionPtr );
#else
    pthread_mutex_unlock( (pthread_mutex_t*)m_CriticalSectionPtr );
#endif
}





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cCSLock

cCSLock::cCSLock(cCriticalSection * a_CS) 
	: m_CS(a_CS)
	#ifdef _DEBUG
	, m_IsLocked(false)
	#endif
{
	Lock();
}





cCSLock::cCSLock(cCriticalSection & a_CS) 
	: m_CS(&a_CS)
	#ifdef _DEBUG
	, m_IsLocked(false)
	#endif
{
	Lock();
}





cCSLock::~cCSLock()
{
	#ifdef _DEBUG
	if (!m_IsLocked)
	{
		return;
	}
	#endif  // _DEBUG
	Unlock();
}





void cCSLock::Lock(void)
{
	#ifdef _DEBUG
	assert(!m_IsLocked);
	m_IsLocked = true;
	#endif  // _DEBUG
	
	m_CS->Lock();
}





void cCSLock::Unlock(void)
{
	#ifdef _DEBUG
	assert(m_IsLocked);
	m_IsLocked = false;
	#endif  // _DEBUG
	
	m_CS->Unlock();
}





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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




