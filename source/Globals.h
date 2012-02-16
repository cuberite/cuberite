
// Globals.h

// This file gets included from every module in the project, so that global symbols may be introduced easily
// Also used for precompiled header generation in MSVC environments





// Compiler-dependent stuff:
#ifndef _MSC_VER
	// Non-MS compilers don't know the override keyword
	#define override
#else
	// MSVC produces warning C4481 on the override keyword usage, so disable the warning altogether
	#pragma warning(disable:4481)
	
	// Disable some warnings that we don't care about:
	#pragma warning(disable:4100)
#endif  // _MSC_VER





// A macro to disallow the copy constructor and operator= functions
// This should be used in the private: declarations for any class that shouldn't allow copying itself
#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
	TypeName(const TypeName &); \
	void operator=(const TypeName &)





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
	#include <arpa/inet.h>
	#include <netdb.h>
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
	#include <tr1/memory>
#endif





// CRT stuff:
#include <assert.h>
#include <stdio.h>
#include <math.h>





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
	#define ABSTRACT abstract
#else
	// TODO: how do other compilers mark functions as obsolete, so that their usage results in a compile-time warning?
	#define OBSOLETE
	// TODO: Can other compilers explicitly mark classes as abstract (no instances can be created)?
	#define ABSTRACT
#endif

/// Faster than (int)floorf((float)x / (float)div)
#define FAST_FLOOR_DIV( x, div ) ( (x) < 0 ? (((int)x / div) - 1) : ((int)x / div) )





/// A generic interface used mainly in ForEach() functions
template <typename Type> class cItemCallback
{
public:
	/// Called for each item in the internal list; return true to stop the loop, or false to continue enumerating
	virtual bool Item(Type * a_Type) = 0;
} ;




