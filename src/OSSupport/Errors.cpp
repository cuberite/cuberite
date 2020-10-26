
#include "Globals.h"

#include "Errors.h"

AString GetOSErrorString( int a_ErrNo)
{
	char buffer[ 1024 ];
	AString Out;

	#ifdef _WIN32

	FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM, nullptr, a_ErrNo, 0, buffer, ARRAYCOUNT(buffer), nullptr);
	Printf(Out, "%d: %s", a_ErrNo, buffer);
	if (!Out.empty() && (Out[Out.length() - 1] == '\n'))
	{
		Out.erase(Out.length() - 2);
	}
	return Out;

	#else  // _WIN32

	// According to https://linux.die.net/man/3/strerror_r there are two versions of strerror_r():

	#if defined(__GLIBC__) && defined( _GNU_SOURCE)  // GNU version of strerror_r()

	char * res = strerror_r( errno, buffer, ARRAYCOUNT(buffer));
	if (res != nullptr)
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

