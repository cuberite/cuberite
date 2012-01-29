#pragma once

#ifndef _WIN32
	// Linux threads http://www.yolinux.com/TUTORIALS/LinuxTutorialPosixThreads.html
	#include <netdb.h>
	#include <sys/socket.h>
	#include <netinet/in.h>
	#include <arpa/inet.h>
	#include <unistd.h>

	// TODO: We shouldn't need these! Use the OS support objects instead
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
#endif





inline bool IsSocketError( int a_ReturnedValue )
{
    #ifdef _WIN32
    return (a_ReturnedValue == SOCKET_ERROR || a_ReturnedValue == 0);
    #else
    return (a_ReturnedValue <= 0);
    #endif
}
