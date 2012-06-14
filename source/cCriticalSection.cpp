
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cCriticalSection:

cCriticalSection::cCriticalSection()
{
#ifdef _WIN32
	InitializeCriticalSection( &m_CriticalSection );
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
	DeleteCriticalSection( &m_CriticalSection );
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
	EnterCriticalSection( &m_CriticalSection );
#else
    pthread_mutex_lock( (pthread_mutex_t*)m_CriticalSectionPtr );
#endif
}





void cCriticalSection::Unlock()
{
#ifdef _WIN32
	LeaveCriticalSection( &m_CriticalSection );
#else
    pthread_mutex_unlock( (pthread_mutex_t*)m_CriticalSectionPtr );
#endif
}





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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




