
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#ifndef _WIN32
	#include <unistd.h>
#endif





void cSleep::MilliSleep( unsigned int a_MilliSeconds)
{
#ifdef _WIN32
	Sleep(a_MilliSeconds);  // Don't tick too much
#else
	usleep(a_MilliSeconds*1000);
#endif
}
