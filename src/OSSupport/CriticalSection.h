
#pragma once





class cCriticalSection
{
	friend class cDeadlockDetect;  // Allow the DeadlockDetect to read the internals, so that it may output some statistics

public:
	void Lock(void);
	void Unlock(void);

	cCriticalSection(void);

	/** Returns true if the CS is currently locked.
	Note that since it relies on the m_RecursionCount value, it is inherently thread-unsafe, prone to false positives.
	Also, due to multithreading, the state can change between this when function is evaluated and the returned value is used.
	To be used in ASSERT(IsLocked()) only. */
	bool IsLocked(void);

	/** Returns true if the CS is currently locked by the thread calling this function.
	Note that since it relies on the m_RecursionCount value, it is inherently thread-unsafe, prone to false positives.
	Also, due to multithreading, the state can change between this when function is evaluated and the returned value is used.
	To be used in ASSERT(IsLockedByCurrentThread()) only. */
	bool IsLockedByCurrentThread(void);

private:

	/** Number of times that this CS is currently locked (levels of recursion). Zero if not locked.
	Note that this value should be considered true only when the CS is locked; without the lock, it is UndefinedBehavior to even read it,
	but making it std::atomic would impose too much of a runtime penalty.
	It is only ever read without the lock in the DeadlockDetect, where the server is terminating anyway. */
	int m_RecursionCount;

	/** ID of the thread that is currently holding the CS.
	Note that this value should be considered true only when the CS is locked; without the lock, it is UndefinedBehavior to even read it,
	but making it std::atomic would impose too much of a runtime penalty.
	When unlocked, the value stored here has no meaning, it may be an ID of a previous holder, or it could be any garbage.
	It is only ever read without the lock in the DeadlockDetect, where the server is terminating anyway. */
	std::thread::id m_OwningThreadID;

	std::recursive_mutex m_Mutex;
};




/** RAII for cCriticalSection - locks the CS on creation, unlocks on destruction */
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





/** Temporary RAII unlock for a cCSLock. Useful for unlock-wait-relock scenarios */
class cCSUnlock
{
	cCSLock & m_Lock;
public:
	cCSUnlock(cCSLock & a_Lock);
	~cCSUnlock();

private:
	DISALLOW_COPY_AND_ASSIGN(cCSUnlock);
} ;




