
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
	NetworkInterfaceEnum.cpp
	NetworkSingleton.cpp
	ServerHandleImpl.cpp
	StackTrace.cpp
	TCPLinkImpl.cpp
	UDPEndpointImpl.cpp
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
	Network.h
	NetworkSingleton.h
	Queue.h
	ServerHandleImpl.h
	StackTrace.h
	TCPLinkImpl.h
	UDPEndpointImpl.h
)

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
	add_flags_cxx("-Wno-error=global-constructors ")
endif()

if(NOT MSVC)
	add_library(OSSupport ${SRCS} ${HDRS})

	if(UNIX)
		if(NOT APPLE)
			target_link_libraries(OSSupport rt)
		endif()

		target_link_libraries(OSSupport pthread event_core event_extra)
	endif()
endif()
