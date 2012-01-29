#pragma once

class cCriticalSection
{
public:
	cCriticalSection();
	~cCriticalSection();

	void Lock();
	void Unlock();
private:
	void* m_CriticalSectionPtr; // Pointer to a CRITICAL_SECTION object
#ifndef _WIN32
	void* m_Attributes;
#endif
};




/// RAII for cCriticalSection - locks the CS on creation, unlocks on destruction
class cCSLock
{
	cCriticalSection * m_CS;

	#ifdef _DEBUG
	// Unlike a cCriticalSection, this object should be used from a single thread, therefore access to m_IsLocked is not threadsafe
	bool m_IsLocked;
	#endif  // _DEBUG
	
public:
	cCSLock(cCriticalSection * a_CS);
	~cCSLock();
	
	// Temporarily unlock or re-lock:
	void Lock(void);
	void Unlock(void);
} ;





/// Temporary RAII unlock for a cCSLock. Useful for unlock-wait-relock scenarios
class cCSUnlock
{
	cCSLock & m_Lock;
public:
	cCSUnlock(cCSLock & a_Lock);
	~cCSUnlock();
} ;




