
// SleepResolutionBooster.h

// Increases the accuracy of Sleep on Windows (GH #5140).

// This file MUST NOT be included from anywhere other than main.cpp.





#ifdef _WIN32

#include <timeapi.h>





static TIMECAPS g_Resolution;





namespace SleepResolutionBooster
{
	static void Register()
	{
		// Default sleep resolution on Windows isn't accurate enough (GH #5140) so try to boost it:
		if (
			(timeGetDevCaps(&g_Resolution, sizeof(g_Resolution)) == MMSYSERR_NOERROR) &&
			(timeBeginPeriod(g_Resolution.wPeriodMin) == MMSYSERR_NOERROR)
		)
		{
			return;
		}

		// Max < Min sentinel for failure, to prevent bogus timeEndPeriod calls:
		g_Resolution.wPeriodMax = 0;
		g_Resolution.wPeriodMin = 1;
	}

	static void Unregister()
	{
		if (g_Resolution.wPeriodMax >= g_Resolution.wPeriodMin)
		{
			timeEndPeriod(g_Resolution.wPeriodMin);
		}
	}
};

#else

namespace SleepResolutionBooster
{
	static void Register()
	{
	}

	static void Unregister()
	{
	}
};

#endif
