
#include "Globals.h"

#include "Errors.h"

AString GetOSErrorString( int a_ErrNo)
{
	char buffer[ 1024 ];
	AString Out;

	#ifdef _WIN32

	FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, a_ErrNo, 0, buffer, ARRAYCOUNT(buffer), NULL);
	Printf(Out, "%d: %s", a_ErrNo, buffer);
	if (!Out.empty() && (Out[Out.length() - 1] == '\n'))
	{
		Out.erase(Out.length() - 2);
	}
	return Out;
	
	#else  // _WIN32
	
	// According to http://linux.die.net/man/3/strerror_r there are two versions of strerror_r():
	
	#if !defined(__APPLE__) && ( _GNU_SOURCE) && !defined(ANDROID_NDK)  // GNU version of strerror_r()
	
	char * res = strerror_r( errno, buffer, ARRAYCOUNT(buffer));
	if (res != NULL)
	{
		Printf(Out, "%d: %s", a_ErrNo, res);
		return Out;
	}
	
	#else  // XSI version of strerror_r():
	
	int res = strerror_r( errno, buffer, ARRAYCOUNT(buffer));
	if (res == 0)
	{
		Printf(Out, "%d: %s", a_ErrNo, buffer);
		return Out;
	}
	
	#endif  // strerror_r() version
	
	else
	{
		Printf(Out, "Error %d while getting error string for error #%d!", errno, a_ErrNo);
		return Out;
	}
	
	#endif  // else _WIN32
}

