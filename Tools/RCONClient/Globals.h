
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

	#define FORMATSTRING(formatIndex, va_argsIndex)

	// MSVC has its own custom version of zu format
	#define SIZE_T_FMT "%Iu"
	#define SIZE_T_FMT_PRECISION(x) "%" #x "Iu"
	#define SIZE_T_FMT_HEX "%Ix"

	#define NORETURN      __declspec(noreturn)

	// Use non-standard defines in <cmath>
	#define _USE_MATH_DEFINES

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

	#define FORMATSTRING(formatIndex, va_argsIndex) __attribute__((format (printf, formatIndex, va_argsIndex)))

	#if defined(_WIN32)
		// We're compiling on MinGW, which uses an old MSVCRT library that has no support for size_t printfing.
		// We need direct size formats:
		#if defined(_WIN64)
			#define SIZE_T_FMT "%I64u"
			#define SIZE_T_FMT_PRECISION(x) "%" #x "I64u"
			#define SIZE_T_FMT_HEX "%I64x"
		#else
			#define SIZE_T_FMT "%u"
			#define SIZE_T_FMT_PRECISION(x) "%" #x "u"
			#define SIZE_T_FMT_HEX "%x"
		#endif
	#else
		// We're compiling on Linux, so we can use libc's size_t printf format:
		#define SIZE_T_FMT "%zu"
		#define SIZE_T_FMT_PRECISION(x) "%" #x "zu"
		#define SIZE_T_FMT_HEX "%zx"
	#endif

	#define NORETURN      __attribute((__noreturn__))

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





// Integral types with predefined sizes:
typedef long long Int64;
typedef int       Int32;
typedef short     Int16;

typedef unsigned long long UInt64;
typedef unsigned int       UInt32;
typedef unsigned short     UInt16;

typedef unsigned char Byte;





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

	#define _WIN32_WINNT 0x502  // We want to target WinXP SP2 and higher

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
#endif

#if !defined(ANDROID_NDK)
	#define USE_SQUIRREL
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
#include <time.h>





// STL stuff:
#include <vector>
#include <list>
#include <deque>
#include <string>
#include <map>
#include <algorithm>
#include <memory>





// Common headers (part 1, without macros):
#include "StringUtils.h"
#include "OSSupport/CriticalSection.h"
#include "OSSupport/File.h"
#include "OSSupport/Event.h"
#include "Logger.h"





// Common definitions:

/** Evaluates to the number of elements in an array (compile-time!) */
#define ARRAYCOUNT(X) (sizeof(X) / sizeof(*(X)))

/** Allows arithmetic expressions like "32 KiB" (but consider using parenthesis around it, "(32 KiB)") */
#define KiB * 1024

/** Faster than (int)floorf((float)x / (float)div) */
#define FAST_FLOOR_DIV(x, div) ((x) < 0 ? (((int)x / div) - 1) : ((int)x / div))

// Own version of assert() that writes failed assertions to the log for review
#ifdef  NDEBUG
	#define ASSERT(x) ((void)0)
#else
	#define ASSERT assert
#endif

// Pretty much the same as ASSERT() but stays in Release builds
#define VERIFY(x) (!!(x) || (LOGERROR("Verification failed: %s, file %s, line %i", #x, __FILE__, __LINE__), exit(1), 0))
