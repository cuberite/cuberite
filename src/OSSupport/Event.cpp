
// Event.cpp

// Implements the cEvent object representing an OS-specific synchronization primitive that can be waited-for
// Implemented as an Event on Win and as a 1-semaphore on *nix

#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Event.h"
#include "Errors.h"




cEvent::cEvent(void) :
	m_ShouldWait(true)
{
}





void cEvent::Wait(void)
{
	std::unique_lock<std::mutex> Lock(m_Mutex);
	while (m_ShouldWait)
	{
		m_CondVar.wait(Lock);
	}
	m_ShouldWait = true;
}





bool cEvent::Wait(unsigned a_TimeoutMSec)
{
	auto dst = std::chrono::system_clock::now() + std::chrono::milliseconds(a_TimeoutMSec);
	std::unique_lock<std::mutex> Lock(m_Mutex);  // We assume that this lock is acquired without much delay - we are the only user of the mutex
	while (m_ShouldWait && (std::chrono::system_clock::now() <= dst))
	{
		switch (m_CondVar.wait_until(Lock, dst))
		{
			case std::cv_status::no_timeout:
			{
				// The wait was successful, check for spurious wakeup:
				if (!m_ShouldWait)
				{
					m_ShouldWait = true;
					return true;
				}
				// This was a spurious wakeup, wait again:
				continue;
			}
			
			case std::cv_status::timeout:
			{
				// The wait timed out, return failure:
				return false;
			}
		}  // switch (wait_until())
	}  // while (m_ShouldWait && not timeout)

	// The wait timed out in the while condition:
	return false;
}





void cEvent::Set(void)
{
	{
		std::unique_lock<std::mutex> Lock(m_Mutex);
		m_ShouldWait = false;
	}
	m_CondVar.notify_one();
}




