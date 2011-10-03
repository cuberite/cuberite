#pragma once

class cEvent
{
public:
	cEvent( unsigned int a_NumEvents = 1 );
	~cEvent();

	void Wait();
	void Set(unsigned int a_EventNum = 0);
private:
	unsigned int m_NumEvents;
	void* m_Handle; // HANDLE[] pointer

#ifndef _WIN32
    bool m_bNamed;
#endif
};
