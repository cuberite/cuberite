
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Timer.h"






cTimer::cTimer(void)
{
	#ifdef _WIN32
		QueryPerformanceFrequency(&m_TicksPerSecond);
	#endif
}





long long cTimer::GetNowTime(void)
{
	#ifdef _WIN32
		LARGE_INTEGER now;
		QueryPerformanceCounter(&now);
		return ((now.QuadPart * 1000) / m_TicksPerSecond.QuadPart);
	#else
		struct timeval  now;
		gettimeofday(&now, NULL);
		return (long long)now.tv_sec * 1000 + (long long)now.tv_usec / 1000;
	#endif
}




