#pragma once

class cSemaphore
{
public:
	cSemaphore( unsigned int a_MaxCount, unsigned int a_InitialCount = 0);
	~cSemaphore();

	void Wait();
	void Signal();
private:
	void* m_Handle;  // HANDLE pointer

#ifndef _WIN32
	bool m_bNamed;
#endif
};
