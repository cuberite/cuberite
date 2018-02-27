
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

	// Mark functions as obsolete, so that their usage results in a compile-time warning
	#define OBSOLETE

	// Mark types / variables for alignment. Do the platforms need it?
	#define ALIGN_8
	#define ALIGN_16
	*/


#endif





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
	#include <Windows.h>
	#include <winsock2.h>
	#include <ws2tcpip.h>

	// Windows SDK defines min and max macros, messing up with our std::min and std::max usage
	#undef min
	#undef max

	// Windows SDK defines GetFreeSpace as a constant, probably a Win16 API remnant
	#ifdef GetFreeSpace
		#undef GetFreeSpace
	#endif  // GetFreeSpace

	#define SocketError WSAGetLastError()
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
	#include <unistd.h>

	#include <cstdio>
	#include <cstring>
	#include <pthread.h>
	#include <semaphore.h>
	#include <errno.h>
	#include <fcntl.h>

	typedef int SOCKET;
	enum
	{
		INVALID_SOCKET = -1,
	};
	#define closesocket close
	#define SocketError errno
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
#include <assert.h>
#include <stdio.h>
#include <math.h>
#include <stdarg.h>
#include <time.h>





// STL stuff:
#include <chrono>
#include <vector>
#include <list>
#include <deque>
#include <string>
#include <map>
#include <algorithm>
#include <memory>
#include <atomic>
#include <mutex>
#include <thread>
#include <condition_variable>





// Common headers (part 1, without macros):
#include "fmt/format.h"
#include "StringUtils.h"
#include "OSSupport/CriticalSection.h"

#include "LoggerSimple.h"



// Common definitions:

/** Evaluates to the number of elements in an array (compile-time!) */
#define ARRAYCOUNT(X) (sizeof(X) / sizeof(*(X)))

/* Allows arithmetic expressions like "32 KiB" (but consider using parenthesis around it, "(32 KiB)") */
#define KiB * 1024

/* Faster than (int)floorf((float)x / (float)div) */
#define FAST_FLOOR_DIV(x, div) ((x) < 0 ? (((int)x / div) - 1) : ((int)x / div))

// Own version of assert() that writes failed assertions to the log for review
#ifdef  NDEBUG
	#define ASSERT(x) ((void)0)
#else
	#define ASSERT assert
#endif

// Pretty much the same as ASSERT() but stays in Release builds
#define VERIFY(x) (!!(x) || (LOGERROR("Verification failed: %s, file %s, line %i", #x, __FILE__, __LINE__), exit(1), 0))

// C++11 has std::shared_ptr in <memory>, included earlier
#define SharedPtr std::shared_ptr






/* A generic interface used mainly in ForEach() functions */
template <typename Type> class cItemCallback
{
public:
	/* Called for each item in the internal list; return true to stop the loop, or false to continue enumerating */
	virtual bool Item(Type * a_Type) = 0;
} ;
