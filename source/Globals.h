
// Globals.h

// This file gets included from every module in the project, so that global symbols may be introduced easily
// Also used for precompiled header generation in MSVC environments





// Compiler-dependent stuff:
#if defined(_MSC_VER)
	// MSVC produces warning C4481 on the override keyword usage, so disable the warning altogether
	#pragma warning(disable:4481)
	
	// Disable some warnings that we don't care about:
	#pragma warning(disable:4100)

	#define OBSOLETE __declspec(deprecated)
	
	// No alignment needed in MSVC
	#define ALIGN_8
	#define ALIGN_16
	
#elif defined(__GNUC__)

	// TODO: Can GCC explicitly mark classes as abstract (no instances can be created)?
	#define abstract
	
	// TODO: Can GCC mark virtual methods as overriding (forcing them to have a virtual function of the same signature in the base class)
	#define override
	
	#define OBSOLETE __attribute__((deprecated))

	#define ALIGN_8 __attribute__((aligned(8)))
	#define ALIGN_16 __attribute__((aligned(16)))

	// Some portability macros :)
	#define stricmp strcasecmp

#else

	#error "You are using an unsupported compiler, you might need to #define some stuff here for your compiler"
	
	/*
	// Copy and uncomment this into another #elif section based on your compiler identification
	
	// Explicitly mark classes as abstract (no instances can be created)
	#define abstract
	
	// Mark virtual methods as overriding (forcing them to have a virtual function of the same signature in the base class)
	#define override

	// Mark functions as obsolete, so that their usage results in a compile-time warning
	#define OBSOLETE

	// Mark types / variables for alignment. Do the platforms need it?
	#define ALIGN_8
	#define ALIGN_16
	*/

#endif





// A macro to disallow the copy constructor and operator= functions
// This should be used in the private: declarations for any class that shouldn't allow copying itself
#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
	TypeName(const TypeName &); \
	void operator=(const TypeName &)





// OS-dependent stuff:
#ifdef _WIN32
	#define WIN32_LEAN_AND_MEAN
	#include <Windows.h>
	#include <winsock2.h>
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
#include <stdarg.h>





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

/// Allows arithmetic expressions like "32 KiB" (but consider using parenthesis around it, "(32 KiB)" )
#define KiB * 1024

/// Faster than (int)floorf((float)x / (float)div)
#define FAST_FLOOR_DIV( x, div ) ( (x) < 0 ? (((int)x / div) - 1) : ((int)x / div) )

// Own version of assert() that writes failed assertions to the log for review
#ifdef  NDEBUG
	#define ASSERT(x) ((void)0)
#else
	#define ASSERT( x ) ( !!(x) || ( LOGERROR("Assertion failed: %s, file %s, line %i", #x, __FILE__, __LINE__ ), assert(0), 0 ) )
#endif

// Pretty much the same as ASSERT() but stays in Release builds
#define VERIFY( x ) ( !!(x) || ( LOGERROR("Verification failed: %s, file %s, line %i", #x, __FILE__, __LINE__ ), exit(1), 0 ) )




/// A generic interface used mainly in ForEach() functions
template <typename Type> class cItemCallback
{
public:
	/// Called for each item in the internal list; return true to stop the loop, or false to continue enumerating
	virtual bool Item(Type * a_Type) = 0;
} ;




