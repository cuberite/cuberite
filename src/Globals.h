
// Globals.h

// This file gets included from every module in the project, so that global symbols may be introduced easily
// Also used for precompiled header generation in MSVC environments





// Compiler-dependent stuff:
#if defined(_MSC_VER)
	// MSVC produces warning C4481 on the override keyword usage, so disable the warning altogether
	#pragma warning(disable:4481)

	// Disable some warnings that we don't care about:
	#pragma warning(disable:4100) // Unreferenced formal parameter
    
    // Useful warnings from warning level 4:
	#pragma warning(3 : 4127) // Conditional expression is constant
	#pragma warning(3 : 4189) // Local variable is initialized but not referenced
	#pragma warning(3 : 4245) // Conversion from 'type1' to 'type2', signed/unsigned mismatch
	#pragma warning(3 : 4310) // Cast truncates constant value
	#pragma warning(3 : 4389) // Signed/unsigned mismatch
	#pragma warning(3 : 4505) // Unreferenced local function has been removed
	#pragma warning(3 : 4701) // Potentially unitialized local variable used
	#pragma warning(3 : 4702) // Unreachable code
	#pragma warning(3 : 4706) // Assignment within conditional expression
	
	// Disabling this warning, because we know what we're doing when we're doing this:
	#pragma warning(disable: 4355)  // 'this' used in initializer list
	
	// 2014_01_06 xoft: Disabled this warning because MSVC is stupid and reports it in obviously wrong places
	// #pragma warning(3 : 4244) // Conversion from 'type1' to 'type2', possible loss of data

	#define OBSOLETE __declspec(deprecated)

	// No alignment needed in MSVC
	#define ALIGN_8
	#define ALIGN_16
	
	// MSVC has its own custom version of zu format
	#define SIZE_T_FMT "%Iu"

#elif defined(__GNUC__)

	// TODO: Can GCC explicitly mark classes as abstract (no instances can be created)?
	#define abstract

	// override is part of c++11
	#if __cplusplus < 201103L
  		#define override
	#endif	

	#define OBSOLETE __attribute__((deprecated))

	#define ALIGN_8 __attribute__((aligned(8)))
	#define ALIGN_16 __attribute__((aligned(16)))

	// Some portability macros :)
	#define stricmp strcasecmp
	
	#define SIZE_T_FMT "%zu"

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


#include <stddef.h>


// Integral types with predefined sizes:
typedef long long Int64;
typedef int       Int32;
typedef short     Int16;

typedef unsigned long long UInt64;
typedef unsigned int       UInt32;
typedef unsigned short     UInt16;

typedef unsigned char Byte;


// If you get an error about specialization check the size of integral types
template <typename T, size_t Size, bool x = sizeof(T) == Size>
class SizeChecker;

template <typename T, size_t Size>
class SizeChecker<T, Size, true> {
  T v;
};

template class SizeChecker<Int64, 8>;
template class SizeChecker<Int32, 4>;
template class SizeChecker<Int16, 2>;

template class SizeChecker<UInt64, 8>;
template class SizeChecker<UInt32, 4>;
template class SizeChecker<UInt16, 2>;

// A macro to disallow the copy constructor and operator= functions
// This should be used in the private: declarations for any class that shouldn't allow copying itself
#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
	TypeName(const TypeName &); \
	void operator=(const TypeName &)

// A macro that is used to mark unused function parameters, to avoid pedantic warnings in gcc
#define UNUSED(X) (void)(X)




// OS-dependent stuff:
#ifdef _WIN32
	#define WIN32_LEAN_AND_MEAN

	#define _WIN32_WINNT 0x501  // We want to target WinXP and higher

	#include <Windows.h>
	#include <winsock2.h>
	#include <Ws2tcpip.h>  // IPv6 stuff

	// Windows SDK defines min and max macros, messing up with our std::min and std::max usage
	#undef min
	#undef max

	// Windows SDK defines GetFreeSpace as a constant, probably a Win16 API remnant
	#ifdef GetFreeSpace
		#undef GetFreeSpace
	#endif  // GetFreeSpace
#else
	#include <sys/types.h>
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
#endif

#if defined(ANDROID_NDK)
	#define FILE_IO_PREFIX "/sdcard/mcserver/"
#else
	#define FILE_IO_PREFIX ""
#endif





// CRT stuff:
#include <sys/stat.h>
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
#include <set>
#include <queue>
#include <limits>




// Common headers (part 1, without macros):
#include "StringUtils.h"
#include "OSSupport/Sleep.h"
#include "OSSupport/CriticalSection.h"
#include "OSSupport/Semaphore.h"
#include "OSSupport/Event.h"
#include "OSSupport/Thread.h"
#include "OSSupport/File.h"
#include "MCLogger.h"





// Common definitions:

/// Evaluates to the number of elements in an array (compile-time!)
#define ARRAYCOUNT(X) (sizeof(X) / sizeof(*(X)))

/// Allows arithmetic expressions like "32 KiB" (but consider using parenthesis around it, "(32 KiB)" )
#define KiB * 1024
#define MiB * 1024 * 1024

/// Faster than (int)floorf((float)x / (float)div)
#define FAST_FLOOR_DIV( x, div ) (((x) - (((x) < 0) ? ((div) - 1) : 0)) / (div))

// Own version of assert() that writes failed assertions to the log for review
#ifdef  _DEBUG
	#define ASSERT( x ) ( !!(x) || ( LOGERROR("Assertion failed: %s, file %s, line %i", #x, __FILE__, __LINE__ ), assert(0), 0 ) )
#else
	#define ASSERT(x) ((void)(x))
#endif

// Pretty much the same as ASSERT() but stays in Release builds
#define VERIFY( x ) ( !!(x) || ( LOGERROR("Verification failed: %s, file %s, line %i", #x, __FILE__, __LINE__ ), exit(1), 0 ) )

// Same as assert but in all Self test builds
#ifdef SELF_TEST
#define assert_test(x) ( !!(x) || (assert(!#x), exit(1), 0))
#endif

/// A generic interface used mainly in ForEach() functions
template <typename Type> class cItemCallback
{
public:
	/// Called for each item in the internal list; return true to stop the loop, or false to continue enumerating
	virtual bool Item(Type * a_Type) = 0;
} ;




/** Clamp X to the specified range. */
template <typename T>
T Clamp(T a_Value, T a_Min, T a_Max)
{
	return (a_Value < a_Min) ? a_Min : ((a_Value > a_Max) ? a_Max : a_Value);
}





#ifndef TOLUA_TEMPLATE_BIND
#define TOLUA_TEMPLATE_BIND(x)
#endif





// Common headers (part 2, with macros):
#include "ChunkDef.h"
#include "BiomeDef.h"
#include "BlockID.h"
#include "BlockInfo.h"
#include "Entities/Effects.h"


