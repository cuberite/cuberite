
// Timer.h

// Declares the cTimer class representing a RAII class that measures time from its creation till its destruction





#pragma once

#include "time.h"





class cTimer
{
public:
	cTimer(const AString & a_Title) :
		m_Title(a_Title),
		m_StartTime(clock())
	{
	}
	
	~cTimer()
	{
		clock_t NumTicks = clock() - m_StartTime;
		LOG("%s took %d ticks (%.02f sec)", m_Title.c_str(), NumTicks, (double)NumTicks / CLOCKS_PER_SEC);
	}
	
protected:
	AString m_Title;
	clock_t m_StartTime;
} ;




