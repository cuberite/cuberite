#pragma once

class cThread
{
public:
	typedef void (ThreadFunc)(void*);
	cThread( ThreadFunc a_ThreadFunction, void* a_Param, const char* a_ThreadName = 0);
	~cThread();

	void Start( bool a_bWaitOnDelete = true);
	void WaitForThread();
private:
	ThreadFunc* m_ThreadFunction;

#ifdef _WIN32
	static unsigned long MyThread(void* a_Param);
#else
	static void *MyThread( void *lpParam);
#endif

	void* m_Param;
	cEvent* m_Event;
	cEvent* m_StopEvent;

	AString m_ThreadName;
};
