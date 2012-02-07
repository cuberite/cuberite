
// Globals.h

// This file gets included from every module in the project, so that global symbols may be introduced easily
// Also used for precompiled header generation in MSVC environments





// Compiler-dependent stuff:
#ifndef _MSC_VER
	// Non-MS compilers don't know the override keyword
	#define override
#endif  // _MSC_VER





// OS-dependent stuff:
#ifdef _WIN32
	#define WIN32_LEAN_AND_MEAN
	#include <Windows.h>
	#include <winsock.h>
#else
	#include <sys/types.h>
	#include <sys/stat.h>   // for mkdir
	#include <sys/time.h>
	#include <sys/socket.h>
	#include <netinet/in.h>
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





// Compatibility:
#define ASSERT assert





// STL stuff:
#include <vector>
#include <list>
#include <deque>
#include <string>
#include <map>
#include <algorithm>
#include <memory>





// Common headers:
#include "StringUtils.h"
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

#ifdef _MSC_VER
	#define OBSOLETE __declspec(deprecated)
#else
	// TODO: how do other compilers mark functions as obsolete, so that their usage results in a compile-time warning?
	#define OBSOLETE
#endif





