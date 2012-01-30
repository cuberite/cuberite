
// Globals.h

// This file gets included from every module in the project, so that global symbols may be introduced easily
// Also used for precompiled header generation in MSVC environments





// OS-dependent stuff:
#ifdef _WIN32
	#define WIN32_LEAN_AND_MEAN
	#include <Windows.h>
	#include <winsock.h>
#else
	#include <sys/types.h>
	#include <sys/stat.h>   // for mkdir
	#include <sys/time.h>
	#include <time.h>
	#include <dirent.h>
	#include <errno.h>
	#include <iostream>

	#include <cstdio>
	#include <cstring>
	#include <pthread.h>
	#include <semaphore.h>
	#include <errno.h>
	#include <fcntl.h>
#endif





// CRT stuff:
#include <assert.h>
#include <stdio.h>





// STL stuff:
#include <vector>
#include <list>
#include <string>
#include <map>
#include <algorithm>





// Common headers:
#include "cSleep.h"
#include "cCriticalSection.h"
#include "cSemaphore.h"
#include "cEvent.h"
#include "cThread.h"
#include "cFile.h"
#include "cMCLogger.h"





// Common definitions:

/// Evaluates to the number of elements in an array (compile-time!)
#define ARRAYCOUNT(X) (sizeof(X) / sizeof(*(X)))

// sprintf_s is the preferred call in MSVC ("secure"); make it *nix-compatible:
#ifndef _WIN32
	#define sprintf_s(dst, size, format, ...) sprintf(dst, format, __VA_ARGS__ )
	#define	vsnprintf_s(buffer, buffer_size, maxcount, stringbuffer, ...) (vsnprintf(buffer, maxcount, stringbuffer, __VA_ARGS__))
#endif  // _WIN32





