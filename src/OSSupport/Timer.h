
// Timer.h

// Declares the cTimer class representing an OS-independent of retrieving current time with msec accuracy





#pragma once





class cTimer
{
public:
	cTimer(void);

	// Returns the current time expressed in milliseconds
	long long GetNowTime(void);
private:

	#ifdef _WIN32
		LARGE_INTEGER m_TicksPerSecond;
	#endif
} ;




