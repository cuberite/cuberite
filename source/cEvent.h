
// cEvent.h

// Interfaces to the cEvent object representing an OS-specific synchronization primitive that can be waited-for
// Implemented as an Event on Win and as a 1-semaphore on *nix





#pragma once
#ifndef CEVENT_H_INCLUDED
#define CEVENT_H_INCLUDED





class cEvent
{
public:
	cEvent(void);
	~cEvent();

	void Wait(void);
	void Set (void);
	
private:

	#ifdef _WIN32
	HANDLE m_Event;
	#else
	sem_t * m_Event;
	bool    m_bIsNamed;
	#endif
} ;






#endif  // CEVENT_H_INCLUDED




