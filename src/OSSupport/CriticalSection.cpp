
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules
#include "CriticalSection.h"





////////////////////////////////////////////////////////////////////////////////
// cCriticalSection:

#ifdef _DEBUG
cCriticalSection::cCriticalSection()
{
	m_IsLocked = 0;
}
#endif  // _DEBUG





void cCriticalSection::Lock()
{
	m_Mutex.lock();
	
	#ifdef _DEBUG
		m_IsLocked += 1;
		m_OwningThreadID = std::this_thread::get_id();
	#endif  // _DEBUG
}





void cCriticalSection::Unlock()
{
	#ifdef _DEBUG
		ASSERT(m_IsLocked > 0);
		m_IsLocked -= 1;
	#endif  // _DEBUG
	
	m_Mutex.unlock();
}





#ifdef _DEBUG
bool cCriticalSection::IsLocked(void)
{
	return (m_IsLocked > 0);
}





bool cCriticalSection::IsLockedByCurrentThread(void)
{
	return ((m_IsLocked > 0) && (m_OwningThreadID == std::this_thread::get_id()));
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




