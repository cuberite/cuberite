project (Cuberite)

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
	NetworkLookup.cpp
	NetworkSingleton.cpp
	ServerHandleImpl.cpp
	StackTrace.cpp
	TCPLinkImpl.cpp
	UDPEndpointImpl.cpp
	WinStackWalker.cpp
)

SET (HDRS
	CriticalSection.h
	Errors.h
	Event.h
	File.h
	GetAddressInfoError.h
	GZipFile.h
	HostnameLookup.h
	IPLookup.h
	IsThread.h
	Network.h
	NetworkLookup.h
	NetworkSingleton.h
	Queue.h
	ServerHandleImpl.h
	StackTrace.h
	TCPLinkImpl.h
	UDPEndpointImpl.h
	WinStackWalker.h
)

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
	add_flags_cxx("-Wno-error=global-constructors ")
endif()

if(NOT MSVC)
	add_library(OSSupport ${SRCS} ${HDRS})
	target_link_libraries(OSSupport fmt::fmt)
endif()
