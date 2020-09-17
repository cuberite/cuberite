
// Globals.h

// This file gets included from every module in the project, so that global symbols may be introduced easily
// Also used for precompiled header generation in MSVC environments





#pragma once





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

#else

	#error "You are using an unsupported compiler, you might need to #define some stuff here for your compiler"

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
	#include <arpa/inet.h>
	#include <unistd.h>
#endif





// CRT stuff:
#include <cassert>
#include <cstdio>
#include <cmath>
#include <cstdarg>





// STL stuff:
#include <algorithm>
#include <array>
#include <atomic>
#include <chrono>
#include <condition_variable>
#include <deque>
#include <fstream>
#include <limits>
#include <list>
#include <map>
#include <memory>
#include <mutex>
#include <queue>
#include <random>
#include <set>
#include <string>
#include <thread>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#include <vector>




// Common headers (part 1, without macros):
#include "fmt.h"
#include "StringUtils.h"
#include "LoggerSimple.h"
#include "OSSupport/CriticalSection.h"
#include "OSSupport/Event.h"
#include "OSSupport/File.h"
#include "OSSupport/StackTrace.h"

#ifdef TEST_GLOBALS

	// Basic logging function implementations
	namespace Logger
	{

	inline void LogFormat(
		std::string_view a_Format, eLogLevel, fmt::format_args a_ArgList
	)
	{
		fmt::vprint(a_Format, a_ArgList);
		putchar('\n');
		fflush(stdout);
	}

	inline void LogPrintf(
		std::string_view a_Format, eLogLevel, fmt::printf_args a_ArgList
	)
	{
		fmt::vprintf(a_Format, a_ArgList);
		putchar('\n');
		fflush(stdout);
	}

	inline void LogSimple(std::string_view a_Message, eLogLevel)
	{
		fmt::print("{}\n", a_Message);
		fflush(stdout);
	}

	}  // namespace Logger

#endif





// Common definitions:

/** Evaluates to the number of elements in an array (compile-time!) */
#define ARRAYCOUNT(X) (sizeof(X) / sizeof(*(X)))

/** Allows arithmetic expressions like "32 KiB" (but consider using parenthesis around it, "(32 KiB)") */
#define KiB * 1024
#define MiB * 1024 * 1024

/** Faster than (int)floorf((float)x / (float)div) */
#define FAST_FLOOR_DIV(x, div) (((x) - (((x) < 0) ? ((div) - 1) : 0)) / (div))

// Own version of ASSERT() that plays nicely with the testing framework
#ifdef TEST_GLOBALS

	class cAssertFailure
	{
		AString mExpression;
		AString mFileName;
		int mLineNumber;

	public:
		cAssertFailure(const AString & aExpression, const AString & aFileName, int aLineNumber):
			mExpression(aExpression),
			mFileName(aFileName),
			mLineNumber(aLineNumber)
		{
		}

		const AString & expression() const { return mExpression; }
		const AString & fileName() const { return mFileName; }
		int lineNumber() const { return mLineNumber; }
	};

	#ifdef _DEBUG
		#define ASSERT(x) do { if (!(x)) { throw cAssertFailure(#x, __FILE__, __LINE__);} } while (0)
	#else
		#define ASSERT(...)
	#endif

	// Pretty much the same as ASSERT() but stays in Release builds
	#define VERIFY(x) (!!(x) || ( LOGERROR("Verification failed: %s, file %s, line %i", #x, __FILE__, __LINE__), std::abort(), 0))

#else  // TEST_GLOBALS

	#ifdef _DEBUG
		#define ASSERT(x) ( !!(x) || ( LOGERROR("Assertion failed: %s, file %s, line %i", #x, __FILE__, __LINE__), std::abort(), 0))
	#else
		#define ASSERT(x)
	#endif

	// Pretty much the same as ASSERT() but stays in Release builds
	#define VERIFY(x) (!!(x) || ( LOGERROR("Verification failed: %s, file %s, line %i", #x, __FILE__, __LINE__), std::abort(), 0))

#endif  // else TEST_GLOBALS

/** Use to mark code that should be impossible to reach. */
#define UNREACHABLE(x) do { FLOGERROR("Hit unreachable code: {0}, file {1}, line {2}", #x, __FILE__, __LINE__); std::abort(); } while (false)




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





// a tick is 50 ms
using cTickTime = std::chrono::duration<int,  std::ratio_multiply<std::chrono::milliseconds::period, std::ratio<50>>>;
using cTickTimeLong = std::chrono::duration<Int64,  cTickTime::period>;

#ifndef TOLUA_TEMPLATE_BIND
	#define TOLUA_TEMPLATE_BIND(x)
#endif

#ifdef TOLUA_EXPOSITION
	#error TOLUA_EXPOSITION should never actually be defined
#endif

template <typename T>
auto ToUnsigned(T a_Val)
{
	ASSERT(a_Val >= 0);
	return static_cast<std::make_unsigned_t<T>>(a_Val);
}





// Common headers (part 2, with macros):
#include "Vector3.h"

