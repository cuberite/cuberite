
#pragma once





class cCriticalSection
{
public:
	cCriticalSection(void);
	~cCriticalSection();

	void Lock(void);
	void Unlock(void);
	
private:

	#ifdef _WIN32
		CRITICAL_SECTION m_CriticalSection;
	#else  // _WIN32
		void* m_CriticalSectionPtr ALIGN_8; // Pointer to a CRITICAL_SECTION object
		void* m_Attributes ALIGN_8;
	#endif  // else _WIN32
} ALIGN_8;




/// RAII for cCriticalSection - locks the CS on creation, unlocks on destruction
class cCSLock
{
	cCriticalSection * m_CS;

	// Unlike a cCriticalSection, this object should be used from a single thread, therefore access to m_IsLocked is not threadsafe
	// In Windows, it is an error to call cCriticalSection::Unlock() multiple times if the lock is not held, 
	// therefore we need to check this value whether we are locked or not.
	bool m_IsLocked;
	
public:
	cCSLock(cCriticalSection * a_CS);
	cCSLock(cCriticalSection & a_CS);
	~cCSLock();
	
	// Temporarily unlock or re-lock:
	void Lock(void);
	void Unlock(void);

private:
	DISALLOW_COPY_AND_ASSIGN(cCSLock);
} ;





/// Temporary RAII unlock for a cCSLock. Useful for unlock-wait-relock scenarios
class cCSUnlock
{
	cCSLock & m_Lock;
public:
	cCSUnlock(cCSLock & a_Lock);
	~cCSUnlock();
	
private:
	DISALLOW_COPY_AND_ASSIGN(cCSUnlock);
} ;




