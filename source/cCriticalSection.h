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