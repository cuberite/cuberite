
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules
#include "CriticalSection.h"





////////////////////////////////////////////////////////////////////////////////
// cCriticalSection:

cCriticalSection::cCriticalSection():
	m_RecursionCount(0)
{
}





void cCriticalSection::Lock()
{
	m_Mutex.lock();

	m_RecursionCount += 1;
	m_OwningThreadID = std::this_thread::get_id();
}





void cCriticalSection::Unlock()
{
	ASSERT(IsLockedByCurrentThread());
	m_RecursionCount -= 1;

	m_Mutex.unlock();
}





bool cCriticalSection::IsLocked(void)
{
	return (m_RecursionCount > 0);
}





bool cCriticalSection::IsLockedByCurrentThread(void)
{
	return ((m_RecursionCount > 0) && (m_OwningThreadID == std::this_thread::get_id()));
}





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




