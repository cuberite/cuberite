#include "cCriticalSection.h"
#include "cMCLogger.h"

#ifdef _WIN32
#include <Windows.h>
#else
#include <pthread.h>
#endif

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
