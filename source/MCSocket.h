#pragma once

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <WinSock.h>
// #define socklen_t int
// #ifdef SendMessage
// #undef SendMessage
// #endif
#else

// Linux threads http://www.yolinux.com/TUTORIALS/LinuxTutorialPosixThreads.html

#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <unistd.h>
#define SOCKET int
typedef void *HANDLE;
#define CRITICAL_SECTION pthread_mutex_t
#define SD_BOTH (2)
#define closesocket(x) (shutdown(x, SD_BOTH), close(x))
#define SOCKET_ERROR SO_ERROR
#define EnterCriticalSection(x) pthread_mutex_lock(x)
#define LeaveCriticalSection(x) pthread_mutex_unlock(x)
#define InitializeCriticalSection(x) pthread_mutex_init(x, NULL)
#define DeleteCriticalSection(x) (x)
#define sprintf_s(x, y, ...) sprintf(x, __VA_ARGS__)

#endif

inline bool IsSocketError( int a_ReturnedValue )
{
    #ifdef _WIN32
    return (a_ReturnedValue == SOCKET_ERROR || a_ReturnedValue == 0);
    #else
    return (a_ReturnedValue <= 0);
    #endif
}
