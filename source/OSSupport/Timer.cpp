
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Timer.h"






cTimer::cTimer()
#ifdef _WIN32
	: m_TicksPerSecond( new LARGE_INTEGER )
#endif
{
#ifdef _WIN32
	QueryPerformanceFrequency( (LARGE_INTEGER*)m_TicksPerSecond );
#endif
}

cTimer::~cTimer()
{
#ifdef _WIN32
	delete (LARGE_INTEGER*)m_TicksPerSecond;
#endif
}

long long cTimer::GetNowTime()
{
#ifdef _WIN32
	LARGE_INTEGER now;
	QueryPerformanceCounter( &now );
	LARGE_INTEGER & tps = *((LARGE_INTEGER*)m_TicksPerSecond);
	return ((now.QuadPart*1000) / tps.QuadPart );
#else
	struct timeval  now;
	gettimeofday(&now, NULL);
	return (long long)(now.tv_sec*1000 + now.tv_usec/1000);
#endif
}