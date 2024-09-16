
// Globals.h

// This file gets included from every module in the project, so that global symbols may be introduced easily
// Also used for precompiled header generation in MSVC environments





#pragma once





// Compiler-dependent stuff:
#if defined(_MSC_VER)
	// Use non-standard defines in <cmath>
	#define _USE_MATH_DEFINES

	#ifndef NDEBUG
		// Override the "new" operator to include file and line specification for debugging memory leaks
		// Ref.: https://social.msdn.microsoft.com/Forums/en-US/ebc7dd7a-f3c6-49f1-8a60-e381052f21b6/debugging-memory-leaks?forum=vcgeneral#53f0cc89-62fe-45e8-bbf0-56b89f2a1901
		// This causes MSVC Debug runs to produce a report upon program exit, that contains memory-leaks
		// together with the file:line information about where the memory was allocated.
		// Note that this doesn't work with placement-new, which needs to temporarily #undef the macro
		// (See AllocationPool.h for an example).
		#define _CRTDBG_MAP_ALLOC
		#include <stdlib.h>
		#include <crtdbg.h>
		#define DEBUG_CLIENTBLOCK   new(_CLIENT_BLOCK, __FILE__, __LINE__)
		#define new DEBUG_CLIENTBLOCK
		// For some reason this works magically - each "new X" gets replaced as "new(_CLIENT_BLOCK, "file", line) X"
		// The CRT has a definition for this operator new that stores the debugging info for leak-finding later.
	#endif

	#define UNREACHABLE_INTRINSIC __assume(false)

#elif defined(__GNUC__)

	// TODO: Can GCC explicitly mark classes as abstract (no instances can be created)?
	#define abstract

	#define UNREACHABLE_INTRINSIC __builtin_unreachable()

#else

	#error "You are using an unsupported compiler, you might need to #define some stuff here for your compiler"

#endif





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
	#define NOMINMAX  // Windows SDK defines min and max macros, messing up with our std::min and std::max usage.
	#define WIN32_LEAN_AND_MEAN
	#define _WIN32_WINNT 0x0501  // We want to target Windows XP with Service Pack 2 & Windows Server 2003 with Service Pack 1 and higher.

	// Use CryptoAPI primitives when targeting a version that supports encrypting with AES-CFB8 smaller than a full block at a time.
	#define PLATFORM_CRYPTOGRAPHY (_WIN32_WINNT >= 0x0602)

	#include <Windows.h>
	#include <winsock2.h>
	#include <Ws2tcpip.h>  // IPv6 stuff

	// Windows SDK defines GetFreeSpace as a constant, probably a Win16 API remnant:
	#ifdef GetFreeSpace
		#undef GetFreeSpace
	#endif  // GetFreeSpace
#else
	#define PLATFORM_CRYPTOGRAPHY 0

	#include <arpa/inet.h>
	#include <netinet/in.h>
	#include <netinet/tcp.h>
	#include <sys/socket.h>
	#include <unistd.h>
#endif





// CRT stuff:
#include <cassert>
#include <cstdio>
#include <cmath>
#include <cstdarg>
#include <cstddef>





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
#include <utility>
#include <vector>
#include <variant>





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
template class SizeChecker<Int8, 1>;

template class SizeChecker<UInt64, 8>;
template class SizeChecker<UInt32, 4>;
template class SizeChecker<UInt16, 2>;
template class SizeChecker<UInt8, 1>;

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

	#ifdef NDEBUG
		#define ASSERT(x)
	#else
		#define ASSERT(x) do { if (!(x)) { throw cAssertFailure(#x, __FILE__, __LINE__);} } while (0)
	#endif

	// Pretty much the same as ASSERT() but stays in Release builds
	#define VERIFY(x) (!!(x) || ( LOGERROR("Verification failed: %s, file %s, line %i", #x, __FILE__, __LINE__), std::abort(), 0))

#else  // TEST_GLOBALS

	#ifdef NDEBUG
		#define ASSERT(x)
	#else
		#define ASSERT(x) ( !!(x) || ( LOGERROR("Assertion failed: %s, file %s, line %i", #x, __FILE__, __LINE__), std::abort(), 0))
	#endif

	// Pretty much the same as ASSERT() but stays in Release builds
	#define VERIFY(x) (!!(x) || ( LOGERROR("Verification failed: %s, file %s, line %i", #x, __FILE__, __LINE__), std::abort(), 0))

#endif  // else TEST_GLOBALS

// Use to mark code that should be impossible to reach.
#ifdef NDEBUG
	#define UNREACHABLE(x) UNREACHABLE_INTRINSIC
#else
	#define UNREACHABLE(x) ( FLOGERROR("Hit unreachable code: {0}, file {1}, line {2}", #x, __FILE__, __LINE__), std::abort(), 0)
#endif





namespace cpp20
{
	template <class T>
	std::enable_if_t<std::is_array_v<T> && (std::extent_v<T> == 0), std::unique_ptr<T>> make_unique_for_overwrite(std::size_t a_Size)
	{
		return std::unique_ptr<T>(new std::remove_extent_t<T>[a_Size]);
	}

	template <class T>
	std::enable_if_t<!std::is_array_v<T>, std::unique_ptr<T>> make_unique_for_overwrite()
	{
		return std::unique_ptr<T>(new T);
	}
}





/**
You can use this struct to use in std::visit
example:
std::visit(
	OverloadedVariantAccess
	{
		[&] (cFirstType  & a_FirstTypeObject)  {  // Your code to handle cFirstType },
		[&] (cSecondType & a_SecondTypeObject) {  // YourCode to handle cSecondType },
		...
	}
, YourVariant);
You can use constant references if you want to.
*/
template<class... Ts> struct OverloadedVariantAccess : Ts... { using Ts::operator()...; };
template<class... Ts> OverloadedVariantAccess(Ts...)->OverloadedVariantAccess<Ts...>;





/** Clamp X to the specified range. */
template <typename T>
T Clamp(T a_Value, T a_Min, T a_Max)
{
	return (a_Value < a_Min) ? a_Min : ((a_Value > a_Max) ? a_Max : a_Value);
}





/** Floors a value, then casts it to C (an int by default). */
template <typename C = int, typename T>
typename std::enable_if<std::is_arithmetic<T>::value, C>::type FloorC(T a_Value)
{
	return static_cast<C>(std::floor(a_Value));
}

/** Ceils a value, then casts it to C (an int by default). */
template <typename C = int, typename T>
typename std::enable_if<std::is_arithmetic<T>::value, C>::type CeilC(T a_Value)
{
	return static_cast<C>(std::ceil(a_Value));
}





// A time duration representing a Minecraft tick (50 ms), capable of storing at least 32'767 ticks.
using cTickTime = std::chrono::duration<signed int, std::ratio_multiply<std::chrono::milliseconds::period, std::ratio<50>>>;

// A time duration representing a Minecraft tick (50 ms), capable of storing at least a 64 bit signed duration.
using cTickTimeLong = std::chrono::duration<signed long long int, cTickTime::period>;

/** Converts a literal to a tick time. */
constexpr cTickTimeLong operator ""_tick(const unsigned long long a_Ticks)
{
	return cTickTimeLong(a_Ticks);
}

using ContiguousByteBuffer = std::basic_string<std::byte>;
using ContiguousByteBufferView = std::basic_string_view<std::byte>;

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

// https://stackoverflow.com/questions/1666802/is-there-a-class-macro-in-c
constexpr std::string_view methodName(std::string_view a_PrettyFunction)
{
	size_t Bracket = a_PrettyFunction.rfind("(");
	size_t Space = a_PrettyFunction.rfind(" ", Bracket) + 1;

	return a_PrettyFunction.substr(Space, Bracket - Space);
}

// https://stackoverflow.com/questions/48857887/pretty-function-in-visual-c
#if !defined(__PRETTY_FUNCTION__) && !defined(__GNUC__)
#define __PRETTY_FUNCTION__ __FUNCSIG__
#endif

#define __METHOD_NAME__ methodName(__PRETTY_FUNCTION__)

// Common headers (part 2, with macros):
#include "Vector3.h"
#include "BlockState.h"
#include "Registries/BlockStates.h"
#include "Registries/Items.h"
