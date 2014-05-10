

#include <cstdarg>
#include <cstdio>
#include <cstdlib>


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

	// Disabled because it's useless:
	#pragma warning(disable: 4512)  // 'class': assignment operator could not be generated - reported for each class that has a reference-type member
	
	// 2014_01_06 xoft: Disabled this warning because MSVC is stupid and reports it in obviously wrong places
	// #pragma warning(3 : 4244) // Conversion from 'type1' to 'type2', possible loss of data

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
	
	#define FORMATSTRING(formatIndex, va_argsIndex) __attribute__((format (printf, formatIndex, va_argsIndex)))

	#define SIZE_T_FMT "%zu"
	#define SIZE_T_FMT_PRECISION(x) "%" #x "zu"
	#define SIZE_T_FMT_HEX "%zx"
	
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

class cAssertFailure
{
};

#define ASSERT(x) do { if (!(x)) { throw cAssertFailure();} } while (0)
#define testassert(x) do { if(!(x)) { exit(1); } } while (0)
#define CheckAsserts(x) do { try {x} catch (cAssertFailure) { break; }  exit(1); } while (0)

#ifndef TOLUA_TEMPLATE_BIND
#define TOLUA_TEMPLATE_BIND(x)
#endif

// A macro that is used to mark unused function parameters, to avoid pedantic warnings in gcc
#define UNUSED(X) (void)(X)

// Logging functions
void LOGERROR(const char* a_Format, ...) FORMATSTRING(1,2);

void LOGERROR(const char* a_Format, ...)
{
	va_list argList;
	va_start(argList, a_Format);
	vprintf(a_Format, argList);
	va_end(argList);
}


