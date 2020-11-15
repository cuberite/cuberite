include_directories(${PROJECT_SOURCE_DIR}/src/)
include_directories(SYSTEM ${PROJECT_SOURCE_DIR}/lib/mbedtls/include)

find_package(Threads REQUIRED)

# Create a single Network library that contains all the networking code:
set (Network_SRCS
	${PROJECT_SOURCE_DIR}/src/OSSupport/CriticalSection.cpp
	${PROJECT_SOURCE_DIR}/src/OSSupport/Event.cpp
	${PROJECT_SOURCE_DIR}/src/OSSupport/HostnameLookup.cpp
	${PROJECT_SOURCE_DIR}/src/OSSupport/IPLookup.cpp
	${PROJECT_SOURCE_DIR}/src/OSSupport/IsThread.cpp
	${PROJECT_SOURCE_DIR}/src/OSSupport/NetworkInterfaceEnum.cpp
	${PROJECT_SOURCE_DIR}/src/OSSupport/NetworkLookup.cpp
	${PROJECT_SOURCE_DIR}/src/OSSupport/NetworkSingleton.cpp
	${PROJECT_SOURCE_DIR}/src/OSSupport/ServerHandleImpl.cpp
	${PROJECT_SOURCE_DIR}/src/OSSupport/StackTrace.cpp
	${PROJECT_SOURCE_DIR}/src/OSSupport/TCPLinkImpl.cpp
	${PROJECT_SOURCE_DIR}/src/OSSupport/WinStackWalker.cpp
	${PROJECT_SOURCE_DIR}/src/mbedTLS++/CtrDrbgContext.cpp
	${PROJECT_SOURCE_DIR}/src/mbedTLS++/CryptoKey.cpp
	${PROJECT_SOURCE_DIR}/src/mbedTLS++/EntropyContext.cpp
	${PROJECT_SOURCE_DIR}/src/mbedTLS++/SslConfig.cpp
	${PROJECT_SOURCE_DIR}/src/mbedTLS++/SslContext.cpp
	${PROJECT_SOURCE_DIR}/src/mbedTLS++/X509Cert.cpp
	${PROJECT_SOURCE_DIR}/src/StringUtils.cpp
)

set (Network_HDRS
	${PROJECT_SOURCE_DIR}/src/OSSupport/CriticalSection.h
	${PROJECT_SOURCE_DIR}/src/OSSupport/Event.h
	${PROJECT_SOURCE_DIR}/src/OSSupport/GetAddressInfoError.h
	${PROJECT_SOURCE_DIR}/src/OSSupport/HostnameLookup.h
	${PROJECT_SOURCE_DIR}/src/OSSupport/IPLookup.h
	${PROJECT_SOURCE_DIR}/src/OSSupport/IsThread.h
	${PROJECT_SOURCE_DIR}/src/OSSupport/Network.h
	${PROJECT_SOURCE_DIR}/src/OSSupport/NetworkLookup.h
	${PROJECT_SOURCE_DIR}/src/OSSupport/NetworkSingleton.h
	${PROJECT_SOURCE_DIR}/src/OSSupport/ServerHandleImpl.h
	${PROJECT_SOURCE_DIR}/src/OSSupport/StackTrace.h
	${PROJECT_SOURCE_DIR}/src/OSSupport/TCPLinkImpl.h
	${PROJECT_SOURCE_DIR}/src/OSSupport/Queue.h
	${PROJECT_SOURCE_DIR}/src/OSSupport/WinStackWalker.h
	${PROJECT_SOURCE_DIR}/src/mbedTLS++/CtrDrbgContext.h
	${PROJECT_SOURCE_DIR}/src/mbedTLS++/CryptoKey.h
	${PROJECT_SOURCE_DIR}/src/mbedTLS++/EntropyContext.h
	${PROJECT_SOURCE_DIR}/src/mbedTLS++/SslConfig.h
	${PROJECT_SOURCE_DIR}/src/mbedTLS++/SslContext.h
	${PROJECT_SOURCE_DIR}/src/mbedTLS++/X509Cert.h
	${PROJECT_SOURCE_DIR}/src/StringUtils.h
)

add_library(Network
	${Network_SRCS}
	${Network_HDRS}
)

target_link_libraries(Network event_core event_extra fmt::fmt mbedtls)
if(NOT WIN32)
	target_link_libraries(Network event_pthreads Threads::Threads)
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
	PROPERTIES FOLDER Tests/Libraries
)




