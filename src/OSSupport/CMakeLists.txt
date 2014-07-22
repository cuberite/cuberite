
cmake_minimum_required (VERSION 2.6)
project (MCServer)

include_directories ("${PROJECT_SOURCE_DIR}/../")

SET (SRCS
	CriticalSection.cpp
	Errors.cpp
	Event.cpp
	File.cpp
	GZipFile.cpp
	IsThread.cpp
	ListenThread.cpp
	Semaphore.cpp
	Sleep.cpp
	Socket.cpp
	SocketThreads.cpp
	Thread.cpp
	Timer.cpp)

SET (HDRS
	CriticalSection.h
	Errors.h
	Event.h
	File.h
	GZipFile.h
	IsThread.h
	ListenThread.h
	Queue.h
	Semaphore.h
	Sleep.h
	Socket.h
	SocketThreads.h
	Thread.h
	Timer.h)

if(NOT MSVC)
	add_library(OSSupport ${SRCS} ${HDRS})

	if(UNIX)
		target_link_libraries(OSSupport pthread)
	endif()
endif()
