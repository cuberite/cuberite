/**
 * Copyright 2014 MCServer Contributors
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#pragma once





class cCriticalSection
{
public:
	cCriticalSection(void);
	~cCriticalSection();

	void Lock(void);
	void Unlock(void);
	
	// IsLocked/IsLockedByCurrentThread are only used in ASSERT statements, but because of the changes with ASSERT they must always be defined
	// The fake versions (in Release) will not effect the program in any way
	#ifdef _DEBUG
	bool IsLocked(void);
	bool IsLockedByCurrentThread(void);
	#else
	bool IsLocked(void) { return false; }
	bool IsLockedByCurrentThread(void) { return false; }
	#endif  // _DEBUG
	
private:
	#ifdef _DEBUG
	int           m_IsLocked;  // Number of times this CS is locked
	unsigned long m_OwningThreadID;
	#endif  // _DEBUG
	
	#ifdef _WIN32
		CRITICAL_SECTION m_CriticalSection;
	#else  // _WIN32
		pthread_mutex_t     m_CriticalSection;
		pthread_mutexattr_t m_Attributes;
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




