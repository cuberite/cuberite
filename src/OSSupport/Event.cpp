
// Event.cpp

// Implements the cEvent object representing an OS-specific synchronization primitive that can be waited-for
// Implemented as an Event on Win and as a 1-semaphore on *nix

#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Event.h"





cEvent::cEvent(void)
{
#ifdef _WIN32
	m_Event = CreateEvent(NULL, FALSE, FALSE, NULL);
	if (m_Event == NULL)
	{
		LOGERROR("cEvent: cannot create event, GLE = %d. Aborting server.", GetLastError());
		abort();
	}
#else  // *nix
	m_bIsNamed = false;
	m_Event = new sem_t;
	if (sem_init(m_Event, 0, 0))
	{
		// This path is used by MacOS, because it doesn't support unnamed semaphores.
		delete m_Event;
		m_bIsNamed = true;

		AString EventName;
		Printf(EventName, "cEvent%p", this);
		m_Event = sem_open(EventName.c_str(), O_CREAT, 777, 0 );
		if (m_Event == SEM_FAILED)
		{
			LOGERROR("cEvent: Cannot create event, errno = %i. Aborting server.", errno);
			abort();
		}
		// Unlink the semaphore immediately - it will continue to function but will not pollute the namespace
		// We don't store the name, so can't call this in the destructor
		if (sem_unlink(EventName.c_str()) != 0)
		{
			LOGWARN("ERROR: Could not unlink cEvent. (%i)", errno);
		}
	}
#endif  // *nix
}





cEvent::~cEvent()
{
#ifdef _WIN32
	CloseHandle(m_Event);
#else
	if (m_bIsNamed)
	{
		if (sem_close(m_Event) != 0)
		{
			LOGERROR("ERROR: Could not close cEvent. (%i)", errno);
		}
	}
	else
	{
		sem_destroy(m_Event);
		delete m_Event;
	}
#endif
}





void cEvent::Wait(void)
{
	#ifdef _WIN32
		DWORD res = WaitForSingleObject(m_Event, INFINITE);
		if (res != WAIT_OBJECT_0)
		{
			LOGWARN("cEvent: waiting for the event failed: %d, GLE = %d. Continuing, but server may be unstable.", res, GetLastError());
		}
	#else
		int res = sem_wait(m_Event);
		if (res != 0 )
		{
			LOGWARN("cEvent: waiting for the event failed: %i, errno = %i. Continuing, but server may be unstable.", res, errno);
		}
	#endif
}





void cEvent::Set(void)
{
	#ifdef _WIN32
		if (!SetEvent(m_Event))
		{
			LOGWARN("cEvent: Could not set cEvent: GLE = %d", GetLastError());
		}
	#else
		int res = sem_post(m_Event);
		if (res != 0)
		{
			LOGWARN("cEvent: Could not set cEvent: %i, errno = %d", res, errno);
		}
	#endif
}




