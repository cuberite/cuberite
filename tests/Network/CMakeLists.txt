cmake_minimum_required (VERSION 2.6) 

enable_testing()

include_directories(${CMAKE_SOURCE_DIR}/src/)
include_directories(${CMAKE_SOURCE_DIR}/lib/libevent/include)

add_definitions(-DTEST_GLOBALS=1)

# Create a single Network library that contains all the networking code:
set (Network_SRCS
	${CMAKE_SOURCE_DIR}/src/OSSupport/CriticalSection.cpp
	${CMAKE_SOURCE_DIR}/src/OSSupport/Event.cpp
	${CMAKE_SOURCE_DIR}/src/OSSupport/HostnameLookup.cpp
	${CMAKE_SOURCE_DIR}/src/OSSupport/IPLookup.cpp
	${CMAKE_SOURCE_DIR}/src/OSSupport/NetworkSingleton.cpp
	${CMAKE_SOURCE_DIR}/src/OSSupport/ServerHandleImpl.cpp
	${CMAKE_SOURCE_DIR}/src/OSSupport/TCPLinkImpl.cpp
	${CMAKE_SOURCE_DIR}/src/StringUtils.cpp
)

set (Network_HDRS
	${CMAKE_SOURCE_DIR}/src/OSSupport/CriticalSection.h
	${CMAKE_SOURCE_DIR}/src/OSSupport/Event.h
	${CMAKE_SOURCE_DIR}/src/OSSupport/HostnameLookup.h
	${CMAKE_SOURCE_DIR}/src/OSSupport/IPLookup.h
	${CMAKE_SOURCE_DIR}/src/OSSupport/Network.h
	${CMAKE_SOURCE_DIR}/src/OSSupport/NetworkSingleton.h
	${CMAKE_SOURCE_DIR}/src/OSSupport/ServerHandleImpl.h
	${CMAKE_SOURCE_DIR}/src/OSSupport/TCPLinkImpl.h
	${CMAKE_SOURCE_DIR}/src/StringUtils.h
)

add_library(Network
	${Network_SRCS}
	${Network_HDRS}
)

target_link_libraries(Network event_core event_extra)
if (MSVC)
	target_link_libraries(Network ws2_32.lib)
endif()




# Define individual tests:

# Google: download the google.com frontpage using http client socket:
add_executable(Google-exe Google.cpp)
target_link_libraries(Google-exe Network)
add_test(NAME Google-test COMMAND Google-exe)

# EchoServer: Listen on port 9876, echo everything back:
add_executable(EchoServer EchoServer.cpp)
target_link_libraries(EchoServer Network)

# NameLookup: Lookup hostname-to-IP and IP-to-hostname:
add_executable(NameLookup NameLookup.cpp)
target_link_libraries(NameLookup Network)
