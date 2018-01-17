
// Event.cpp

// Interfaces to the cEvent object representing a synchronization primitive that can be waited-for
// Implemented using C++11 condition variable and mutex

#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Event.h"
#include "Errors.h"




cEvent::cEvent(void) :
	m_ShouldContinue(false)
{
}





void cEvent::Wait(void)
{
	{
		std::unique_lock<std::mutex> Lock(m_Mutex);
		m_CondVar.wait(Lock, [this](){ return m_ShouldContinue; });
		m_ShouldContinue = false;
	}
}





bool cEvent::Wait(unsigned a_TimeoutMSec)
{
	auto dst = std::chrono::system_clock::now() + std::chrono::milliseconds(a_TimeoutMSec);
	bool Result;
	{
		std::unique_lock<std::mutex> Lock(m_Mutex);  // We assume that this lock is acquired without much delay - we are the only user of the mutex
		Result = m_CondVar.wait_until(Lock, dst, [this](){ return m_ShouldContinue; });
		m_ShouldContinue = false;
	}
	return Result;
}





void cEvent::Set(void)
{
	{
		std::unique_lock<std::mutex> Lock(m_Mutex);
		m_ShouldContinue = true;
	}
	m_CondVar.notify_one();
}




void cEvent::SetAll(void)
{
	{
		std::unique_lock<std::mutex> Lock(m_Mutex);
		m_ShouldContinue = true;
	}
	m_CondVar.notify_all();
}





