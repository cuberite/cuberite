
cmake_minimum_required (VERSION 2.6)
project (MCServer)

include_directories ("${PROJECT_SOURCE_DIR}/../")

SET (SRCS
	CriticalSection.cpp
	Errors.cpp
	Event.cpp
	File.cpp
	GZipFile.cpp
	HostnameLookup.cpp
	IPLookup.cpp
	IsThread.cpp
	ListenThread.cpp
	NetworkSingleton.cpp
	Semaphore.cpp
	ServerHandleImpl.cpp
	Socket.cpp
	SocketThreads.cpp
	StackTrace.cpp
	TCPLinkImpl.cpp
)

SET (HDRS
	CriticalSection.h
	Errors.h
	Event.h
	File.h
	GZipFile.h
	HostnameLookup.h
	IPLookup.h
	IsThread.h
	ListenThread.h
	Network.h
	NetworkSingleton.h
	Queue.h
	Semaphore.h
	ServerHandleImpl.h
	Socket.h
	SocketThreads.h
	StackTrace.h
	TCPLinkImpl.h
)

if(NOT MSVC)
	add_library(OSSupport ${SRCS} ${HDRS})

	if(UNIX)
		if(NOT APPLE)
			target_link_libraries(OSSupport rt)
		endif()

		target_link_libraries(OSSupport pthread event_core event_extra)
	endif()
endif()
