
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
	#include <Winsock2.h>
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





// STL stuff:
#include <vector>
#include <list>
#include <string>
#include <map>
#include <algorithm>
#include <memory>





// Common headers:
#include "../source/StringUtils.h"
#include "../source/cCriticalSection.h"
#include "../source/cMCLogger.h"





// Common definitions:

/// Evaluates to the number of elements in an array (compile-time!)
#define ARRAYCOUNT(X) (sizeof(X) / sizeof(*(X)))

// sprintf_s is the preferred call in MSVC ("secure"); make it *nix-compatible:
#ifndef _WIN32
	#define sprintf_s(dst, size, format, ...) sprintf(dst, format, __VA_ARGS__ )
	#define	vsnprintf_s(buffer, buffer_size, maxcount, stringbuffer, ...) (vsnprintf(buffer, maxcount, stringbuffer, __VA_ARGS__))
#endif  // _WIN32





