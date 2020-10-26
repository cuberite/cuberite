#pragma once





// Compiler-dependent stuff:
#if defined(_MSC_VER)
	// MSVC produces warning C4481 on the override keyword usage, so disable the warning altogether
	#pragma warning(disable:4481)

	// Disable some warnings that we don't care about:
	#pragma warning(disable:4100)  // Unreferenced formal parameter

	// Useful warnings from warning level 4:
	#pragma warning(3 : 4127)  // Conditional expression is constant
	#pragma warning(3 : 4189)  // Local variable is initialized but not referenced
	#pragma warning(3 : 4245)  // Conversion from 'type1' to 'type2', signed / unsigned mismatch
	#pragma warning(3 : 4310)  // Cast truncates constant value
	#pragma warning(3 : 4389)  // Signed / unsigned mismatch
	#pragma warning(3 : 4505)  // Unreferenced local function has been removed
	#pragma warning(3 : 4701)  // Potentially unitialized local variable used
	#pragma warning(3 : 4702)  // Unreachable code
	#pragma warning(3 : 4706)  // Assignment within conditional expression

	// Disabling this warning, because we know what we're doing when we're doing this:
	#pragma warning(disable: 4355)  // 'this' used in initializer list

	// Disabled because it's useless:
	#pragma warning(disable: 4512)  // 'class': assignment operator could not be generated - reported for each class that has a reference-type member

	// 2014_01_06 xoft: Disabled this warning because MSVC is stupid and reports it in obviously wrong places
	// #pragma warning(3 : 4244)  // Conversion from 'type1' to 'type2', possible loss of data


	#define FORMATSTRING(formatIndex, va_argsIndex)
#elif defined(__GNUC__)

	// TODO: Can GCC explicitly mark classes as abstract (no instances can be created)?
	#define abstract


	#define FORMATSTRING(formatIndex, va_argsIndex) __attribute__((format (printf, formatIndex, va_argsIndex)))
#else

	#error "You are using an unsupported compiler, you might need to #define some stuff here for your compiler"

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
class SizeChecker<T, Size, true>
{
	T v;
};

template class SizeChecker<Int64, 8>;
template class SizeChecker<Int32, 4>;
template class SizeChecker<Int16, 2>;

template class SizeChecker<UInt64, 8>;
template class SizeChecker<UInt32, 4>;
template class SizeChecker<UInt16, 2>;

// A macro to disallow the copy constructor and operator = functions
// This should be used in the private: declarations for any class that shouldn't allow copying itself
#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
	TypeName(const TypeName &); \
	void operator =(const TypeName &)

// A macro that is used to mark unused local variables, to avoid pedantic warnings in gcc / clang / MSVC
// Note that in MSVC it requires the full type of X to be known
#define UNUSED_VAR(X) (void)(X)

// A macro that is used to mark unused function parameters, to avoid pedantic warnings in gcc
// Written so that the full type of param needn't be known
#ifdef _MSC_VER
	#define UNUSED(X)
#else
	#define UNUSED UNUSED_VAR
#endif




// OS-dependent stuff:
#ifdef _WIN32
	#define WIN32_LEAN_AND_MEAN
	#define _WIN32_WINNT _WIN32_WINNT_WS03  // We want to target Windows XP with Service Pack 2 & Windows Server 2003 with Service Pack 1 and higher

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



#ifndef TEST_GLOBALS
	// Common headers (part 1, without macros):
	#include "src/StringUtils.h"
	#include "src/OSSupport/CriticalSection.h"
	#include "src/OSSupport/Semaphore.h"
	#include "src/OSSupport/Event.h"
	#include "src/OSSupport/File.h"
	#include "src/Logger.h"
#else
	// Logging functions
void inline LOGERROR(const char * a_Format, ...) FORMATSTRING(1, 2);

void inline LOGERROR(const char * a_Format, ...)
{
	va_list argList;
	va_start(argList, a_Format);
	vprintf(a_Format, argList);
	va_end(argList);
}
#endif





// Common definitions:

/** Evaluates to the number of elements in an array (compile-time!) */
#define ARRAYCOUNT(X) (sizeof(X) / sizeof(*(X)))

/** Allows arithmetic expressions like "32 KiB" (but consider using parenthesis
around it, "(32 KiB)") */
#define KiB * 1024
#define MiB * 1024 * 1024

/** Faster than (int)floorf((float)x / (float)div) */
#define FAST_FLOOR_DIV( x, div) (((x) - (((x) < 0) ? ((div) - 1) : 0)) / (div))

// Own version of assert() that writes failed assertions to the log for review
#ifdef TEST_GLOBALS

	class cAssertFailure
	{
	};

	#ifdef _WIN32
		#if (defined(_MSC_VER) && defined(_DEBUG))
			#define DBG_BREAK _CrtDbgBreak()
		#else
			#define DBG_BREAK
		#endif
		#define REPORT_ERROR(FMT, ...) \
		{ \
			AString msg = Printf(FMT, __VA_ARGS__); \
			puts(msg.c_str()); \
			fflush(stdout); \
			OutputDebugStringA(msg.c_str()); \
			DBG_BREAK; \
		}
	#else
		#define REPORT_ERROR(FMT, ...) \
		{ \
			AString msg = Printf(FMT, __VA_ARGS__); \
			puts(msg.c_str()); \
			fflush(stdout); \
		}
	#endif
	#define ASSERT(x) do { if (!(x)) { throw cAssertFailure();} } while (0)
	#define testassert(x) do { if (!(x)) { REPORT_ERROR("Test failure: %s, file %s, line %d\n", #x, __FILE__, __LINE__); exit(1); } } while (0)
	#define CheckAsserts(x) do { try {x} catch (cAssertFailure) { break; } REPORT_ERROR("Test failure: assert didn't fire for %s, file %s, line %d\n", #x, __FILE__, __LINE__); exit(1); } while (0)

#else
	#ifdef  _DEBUG
		#define ASSERT( x) ( !!(x) || ( LOGERROR("Assertion failed: %s, file %s, line %i", #x, __FILE__, __LINE__), assert(0), 0))
	#else
		#define ASSERT(x) ((void)(x))
	#endif
#endif

// Pretty much the same as ASSERT() but stays in Release builds
#define VERIFY( x) ( !!(x) || ( LOGERROR("Verification failed: %s, file %s, line %i", #x, __FILE__, __LINE__), exit(1), 0))

// C++11 has std::shared_ptr in <memory>, included earlier
#define SharedPtr std::shared_ptr




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
#include "src/BlockID.h"
#include "src/BlockInfo.h"
