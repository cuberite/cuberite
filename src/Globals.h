
// Globals.h

// This file gets included from every module in the project, so that global symbols may be introduced easily
// Also used for precompiled header generation in MSVC environments





// Compiler-dependent stuff:
#if defined(_MSC_VER)
	// Disable some warnings that we don't care about:
	#pragma warning(disable:4100)  // Unreferenced formal parameter

	// Useful warnings from warning level 4:
	#pragma warning(3 : 4189)  // Local variable is initialized but not referenced
	#pragma warning(3 : 4245)  // Conversion from 'type1' to 'type2', signed / unsigned mismatch
	#pragma warning(3 : 4310)  // Cast truncates constant value
	#pragma warning(3 : 4389)  // Signed / unsigned mismatch
	#pragma warning(3 : 4505)  // Unreferenced local function has been removed
	#pragma warning(3 : 4701)  // Potentially unitialized local variable used
	#pragma warning(3 : 4702)  // Unreachable code
	#pragma warning(3 : 4706)  // Assignment within conditional expression

	// 2014-10-23 xoft: Disabled this because the new C++11 headers in MSVC produce tons of these warnings uselessly
	// #pragma warning(3 : 4127)  // Conditional expression is constant

	// Disabling this warning, because we know what we're doing when we're doing this:
	#pragma warning(disable: 4355)  // 'this' used in initializer list

	// Disabled because it's useless:
	#pragma warning(disable: 4512)  // 'class': assignment operator could not be generated - reported for each class that has a reference-type member
	#pragma warning(disable: 4351)  // new behavior: elements of array 'member' will be default initialized

	// 2014_01_06 xoft: Disabled this warning because MSVC is stupid and reports it in obviously wrong places
	// #pragma warning(3 : 4244)  // Conversion from 'type1' to 'type2', possible loss of data

	#define OBSOLETE __declspec(deprecated)

	#define NORETURN __declspec(noreturn)
	#if (_MSC_VER < 1900)  // noexcept support was added in VS 2015
		#define NOEXCEPT  throw()
		#define CAN_THROW throw(...)
	#else
		#define NOEXCEPT  noexcept
		#define CAN_THROW noexcept(false)
	#endif

	// Use non-standard defines in <cmath>
	#define _USE_MATH_DEFINES

	#ifdef _DEBUG
		// Override the "new" operator to include file and line specification for debugging memory leaks
		// Ref.: https://social.msdn.microsoft.com/Forums/en-US/ebc7dd7a-f3c6-49f1-8a60-e381052f21b6/debugging-memory-leaks?forum=vcgeneral#53f0cc89-62fe-45e8-bbf0-56b89f2a1901
		// This causes MSVC Debug runs to produce a report upon program exit, that contains memory-leaks
		// together with the file:line information about where the memory was allocated.
		// Note that this doesn't work with placement-new, which needs to temporarily #undef the macro
		// (See AllocationPool.h for an example).
		#ifdef _DEBUG
			#define _CRTDBG_MAP_ALLOC
			#include <stdlib.h>
			#include <crtdbg.h>
			#define DEBUG_CLIENTBLOCK   new(_CLIENT_BLOCK, __FILE__, __LINE__)
			#define new DEBUG_CLIENTBLOCK
			// For some reason this works magically - each "new X" gets replaced as "new(_CLIENT_BLOCK, "file", line) X"
			// The CRT has a definition for this operator new that stores the debugging info for leak-finding later.
		#endif
	#endif

#elif defined(__GNUC__)

	// TODO: Can GCC explicitly mark classes as abstract (no instances can be created)?
	#define abstract

	// override is part of c++11
	#if __cplusplus < 201103L
		#define override
	#endif

	#define OBSOLETE __attribute__((deprecated))

	#define NORETURN __attribute((__noreturn__))
	#define NOEXCEPT  noexcept
	#define CAN_THROW noexcept(false)

#else

	#error "You are using an unsupported compiler, you might need to #define some stuff here for your compiler"

#endif




#ifdef  _DEBUG
	#define NORETURNDEBUG NORETURN
#else
	#define NORETURNDEBUG
#endif


#include <stddef.h>


// Integral types with predefined sizes:
typedef signed long long Int64;
typedef signed int       Int32;
typedef signed short     Int16;
typedef signed char      Int8;

typedef unsigned long long UInt64;
typedef unsigned int       UInt32;
typedef unsigned short     UInt16;
typedef unsigned char      UInt8;

typedef unsigned char Byte;
typedef Byte ColourID;


template <typename T, size_t Size>
class SizeChecker
{
	static_assert(sizeof(T) == Size, "Check the size of integral types");
};

template class SizeChecker<Int64, 8>;
template class SizeChecker<Int32, 4>;
template class SizeChecker<Int16, 2>;
template class SizeChecker<Int8,  1>;

template class SizeChecker<UInt64, 8>;
template class SizeChecker<UInt32, 4>;
template class SizeChecker<UInt16, 2>;
template class SizeChecker<UInt8,  1>;

// A macro to disallow the copy constructor and operator = functions
// This should be used in the declarations for any class that shouldn't allow copying itself
#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
	TypeName(const TypeName &) = delete; \
	TypeName & operator =(const TypeName &) = delete

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

	// Windows SDK defines min and max macros, messing up with our std::min and std::max usage
	#define NOMINMAX

	#include <Windows.h>
	#include <winsock2.h>
	#include <Ws2tcpip.h>  // IPv6 stuff

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
	#include <cstring>
	#include <pthread.h>
	#include <semaphore.h>
	#include <fcntl.h>
	#include <unistd.h>
#endif

#if defined(ANDROID_NDK)
	#define FILE_IO_PREFIX "/sdcard/Cuberite/"
#else
	#define FILE_IO_PREFIX ""
#endif





// CRT stuff:
#include <sys/stat.h>
#include <cassert>
#include <cstdio>
#include <cmath>
#include <cstdarg>





// STL stuff:
#include <array>
#include <chrono>
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
#include <random>
#include <type_traits>
#include <atomic>
#include <mutex>
#include <thread>
#include <condition_variable>




// Common headers (part 1, without macros):
#include "fmt/format.h"
#include "StringUtils.h"
#include "OSSupport/CriticalSection.h"
#include "OSSupport/Event.h"
#include "OSSupport/File.h"
#include "OSSupport/StackTrace.h"

#ifndef TEST_GLOBALS

	#include "LoggerSimple.h"

#else
	#include "fmt/printf.h"

	// Logging functions
	template <typename ... Args>
	void LOG(const char * a_Format, const Args & ... a_Args)
	{
		fmt::printf(a_Format, a_Args...);
		putchar('\n');
		fflush(stdout);
	}

	#define LOGERROR   LOG
	#define LOGWARNING LOG
	#define LOGD       LOG
	#define LOGINFO    LOG
	#define LOGWARN    LOG

	template <typename ... Args>
	void FLOG(const char * a_Format, const Args & ... a_Args)
	{
		fmt::print(a_Format, a_Args...);
		putchar('\n');
		fflush(stdout);
	}

	#define FLOGERROR   FLOG
	#define FLOGWARNING FLOG
	#define FLOGD       FLOG
	#define FLOGINFO    FLOG
	#define FLOGWARN    FLOG

#endif





// Common definitions:

/** Evaluates to the number of elements in an array (compile-time!) */
#define ARRAYCOUNT(X) (sizeof(X) / sizeof(*(X)))

/** Allows arithmetic expressions like "32 KiB" (but consider using parenthesis around it, "(32 KiB)") */
#define KiB * 1024
#define MiB * 1024 * 1024

/** Faster than (int)floorf((float)x / (float)div) */
#define FAST_FLOOR_DIV(x, div) (((x) - (((x) < 0) ? ((div) - 1) : 0)) / (div))

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

	#ifdef _DEBUG
		#define ASSERT(x) do { if (!(x)) { throw cAssertFailure();} } while (0)
		#define testassert(x) do { if (!(x)) { REPORT_ERROR("Test failure: %s, file %s, line %d\n", #x, __FILE__, __LINE__); exit(1); } } while (0)
		#define CheckAsserts(x) do { try {x} catch (cAssertFailure) { break; } REPORT_ERROR("Test failure: assert didn't fire for %s, file %s, line %d\n", #x, __FILE__, __LINE__); exit(1); } while (0)
	#else
		#define ASSERT(...)
		#define testassert(...)
		#define CheckAsserts(...) LOG("Assert checking is disabled in Release-mode executables (file %s, line %d)", __FILE__, __LINE__)
	#endif

#else
	#ifdef _DEBUG
		#define ASSERT(x) ( !!(x) || ( LOGERROR("Assertion failed: %s, file %s, line %i", #x, __FILE__, __LINE__), PrintStackTrace(), assert(0), 0))
	#else
		#define ASSERT(x)
	#endif
#endif

// Pretty much the same as ASSERT() but stays in Release builds
#define VERIFY( x) ( !!(x) || ( LOGERROR("Verification failed: %s, file %s, line %i", #x, __FILE__, __LINE__), PrintStackTrace(), exit(1), 0))

// Same as assert but in all Self test builds
#ifdef SELF_TEST
	#define assert_test(x) ( !!(x) || (assert(!#x), exit(1), 0))
#endif

/** Use to mark code that should be impossible to reach. */
#define UNREACHABLE(x) do { FLOGERROR("Hit unreachable code: {0}, file {1}, line {2}", #x, __FILE__, __LINE__); PrintStackTrace(); std::terminate(); } while (false)




/** Clamp X to the specified range. */
template <typename T>
T Clamp(T a_Value, T a_Min, T a_Max)
{
	return (a_Value < a_Min) ? a_Min : ((a_Value > a_Max) ? a_Max : a_Value);
}





/** Floors a value, then casts it to C (an int by default) */
template <typename C = int, typename T>
typename std::enable_if<std::is_arithmetic<T>::value, C>::type FloorC(T a_Value)
{
	return static_cast<C>(std::floor(a_Value));
}

/** Ceils a value, then casts it to C (an int by default) */
template <typename C = int, typename T>
typename std::enable_if<std::is_arithmetic<T>::value, C>::type CeilC(T a_Value)
{
	return static_cast<C>(std::ceil(a_Value));
}



//temporary replacement for std::make_unique until we get c++14

namespace cpp14
{
	template <class T, class... Args>
	std::unique_ptr<T> make_unique(Args&&... args)
	{
		return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
	}
}

// a tick is 50 ms
using cTickTime = std::chrono::duration<int,  std::ratio_multiply<std::chrono::milliseconds::period, std::ratio<50>>>;
using cTickTimeLong = std::chrono::duration<Int64,  cTickTime::period>;

#ifndef TOLUA_TEMPLATE_BIND
	#define TOLUA_TEMPLATE_BIND(x)
#endif

#ifdef TOLUA_EXPOSITION
	#error TOLUA_EXPOSITION should never actually be defined
#endif





// Common headers (part 2, with macros):
#include "Vector3.h"
#include "BiomeDef.h"
#include "ChunkDef.h"
#include "BlockID.h"
#include "BlockInfo.h"




