cmake_minimum_required(VERSION 3.0.2)
enable_testing()

include_directories(${CMAKE_SOURCE_DIR}/src/)
include_directories(SYSTEM ${CMAKE_SOURCE_DIR}/lib/libevent/include)
include_directories(SYSTEM ${CMAKE_SOURCE_DIR}/lib/mbedtls/include)

add_definitions(-DTEST_GLOBALS=1)

# Create a single Network library that contains all the networking code:
set (Network_SRCS
	${CMAKE_SOURCE_DIR}/src/OSSupport/CriticalSection.cpp
	${CMAKE_SOURCE_DIR}/src/OSSupport/Event.cpp
	${CMAKE_SOURCE_DIR}/src/OSSupport/HostnameLookup.cpp
	${CMAKE_SOURCE_DIR}/src/OSSupport/IPLookup.cpp
	${CMAKE_SOURCE_DIR}/src/OSSupport/IsThread.cpp
	${CMAKE_SOURCE_DIR}/src/OSSupport/NetworkInterfaceEnum.cpp
	${CMAKE_SOURCE_DIR}/src/OSSupport/NetworkLookup.cpp
	${CMAKE_SOURCE_DIR}/src/OSSupport/NetworkSingleton.cpp
	${CMAKE_SOURCE_DIR}/src/OSSupport/ServerHandleImpl.cpp
	${CMAKE_SOURCE_DIR}/src/OSSupport/StackTrace.cpp
	${CMAKE_SOURCE_DIR}/src/OSSupport/TCPLinkImpl.cpp
	${CMAKE_SOURCE_DIR}/src/OSSupport/WinStackWalker.cpp
	${CMAKE_SOURCE_DIR}/src/mbedTLS++/CtrDrbgContext.cpp
	${CMAKE_SOURCE_DIR}/src/mbedTLS++/CryptoKey.cpp
	${CMAKE_SOURCE_DIR}/src/mbedTLS++/EntropyContext.cpp
	${CMAKE_SOURCE_DIR}/src/mbedTLS++/SslConfig.cpp
	${CMAKE_SOURCE_DIR}/src/mbedTLS++/SslContext.cpp
	${CMAKE_SOURCE_DIR}/src/mbedTLS++/X509Cert.cpp
	${CMAKE_SOURCE_DIR}/src/StringUtils.cpp
)

set (Network_HDRS
	${CMAKE_SOURCE_DIR}/src/OSSupport/CriticalSection.h
	${CMAKE_SOURCE_DIR}/src/OSSupport/Event.h
	${CMAKE_SOURCE_DIR}/src/OSSupport/GetAddressInfoError.h
	${CMAKE_SOURCE_DIR}/src/OSSupport/HostnameLookup.h
	${CMAKE_SOURCE_DIR}/src/OSSupport/IPLookup.h
	${CMAKE_SOURCE_DIR}/src/OSSupport/IsThread.h
	${CMAKE_SOURCE_DIR}/src/OSSupport/Network.h
	${CMAKE_SOURCE_DIR}/src/OSSupport/NetworkLookup.h
	${CMAKE_SOURCE_DIR}/src/OSSupport/NetworkSingleton.h
	${CMAKE_SOURCE_DIR}/src/OSSupport/ServerHandleImpl.h
	${CMAKE_SOURCE_DIR}/src/OSSupport/StackTrace.h
	${CMAKE_SOURCE_DIR}/src/OSSupport/TCPLinkImpl.h
	${CMAKE_SOURCE_DIR}/src/OSSupport/Queue.h
	${CMAKE_SOURCE_DIR}/src/OSSupport/WinStackWalker.h
	${CMAKE_SOURCE_DIR}/src/mbedTLS++/CtrDrbgContext.h
	${CMAKE_SOURCE_DIR}/src/mbedTLS++/CryptoKey.h
	${CMAKE_SOURCE_DIR}/src/mbedTLS++/EntropyContext.h
	${CMAKE_SOURCE_DIR}/src/mbedTLS++/SslConfig.h
	${CMAKE_SOURCE_DIR}/src/mbedTLS++/SslContext.h
	${CMAKE_SOURCE_DIR}/src/mbedTLS++/X509Cert.h
	${CMAKE_SOURCE_DIR}/src/StringUtils.h
)

add_library(Network
	${Network_SRCS}
	${Network_HDRS}
)

target_link_libraries(Network event_core event_extra fmt::fmt mbedtls)
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

# EnumInterfaces: List all network interfaces:
add_executable(EnumInterfaces-exe EnumInterfaces.cpp)
target_link_libraries(EnumInterfaces-exe Network)
add_test(NAME EnumInterfaces-test COMMAND EnumInterfaces-exe)




# Put all the tests into a solution folder (MSVC):
set_target_properties(
	EchoServer
	Google-exe
	NameLookup
	EnumInterfaces-exe
	PROPERTIES FOLDER Tests/Network
)
set_target_properties(
	Network
	PROPERTIES FOLDER Lib
)




