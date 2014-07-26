
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules





cSemaphore::cSemaphore( unsigned int a_MaxCount, unsigned int a_InitialCount /* = 0 */)
#ifndef _WIN32
	: m_bNamed( false)
#endif
{
#ifndef _WIN32
	(void)a_MaxCount;
	m_Handle = new sem_t;
	if (sem_init( (sem_t*)m_Handle, 0, 0))
	{
		LOG("WARNING cSemaphore: Could not create unnamed semaphore, fallback to named.");
		delete (sem_t*)m_Handle;    // named semaphores return their own address
		m_bNamed = true;

		AString Name;
		Printf(Name, "cSemaphore%p", this);
		m_Handle = sem_open(Name.c_str(), O_CREAT, 777, a_InitialCount);
		if (m_Handle == SEM_FAILED)
		{
			LOG("ERROR: Could not create Semaphore. (%i)", errno);
		}
		else
		{
			if (sem_unlink(Name.c_str()) != 0)
			{
				LOG("ERROR: Could not unlink cSemaphore. (%i)", errno);
			}
		}
	}
#else
	m_Handle = CreateSemaphore(
		NULL,  // security attribute
		a_InitialCount,  // initial count
		a_MaxCount,  // maximum count
		0  // name (optional)
		);
#endif
}





cSemaphore::~cSemaphore()
{
#ifdef _WIN32
	CloseHandle( m_Handle);
#else
	if (m_bNamed)
	{
		if (sem_close( (sem_t*)m_Handle) != 0)
		{
			LOG("ERROR: Could not close cSemaphore. (%i)", errno);
		}
	}
	else
	{
		sem_destroy( (sem_t*)m_Handle);
		delete (sem_t*)m_Handle;
	}
	m_Handle = 0;

#endif
}





void cSemaphore::Wait()
{
#ifndef _WIN32
	if (sem_wait( (sem_t*)m_Handle) != 0)
	{
		LOG("ERROR: Could not wait for cSemaphore. (%i)", errno);
	}
#else
	WaitForSingleObject( m_Handle, INFINITE);
#endif
}





void cSemaphore::Signal()
{
#ifndef _WIN32
	if (sem_post( (sem_t*)m_Handle) != 0)
	{
		LOG("ERROR: Could not signal cSemaphore. (%i)", errno);
	}
#else
	ReleaseSemaphore( m_Handle, 1, NULL);
#endif
}




