
cmake_minimum_required (VERSION 2.6)
project (MCServer)

include_directories ("${PROJECT_SOURCE_DIR}/../")

set(SOURCES
	"BlockingSslClientSocket.cpp"
	"BufferedSslSocket.cpp"
	"CallbackSslContext.cpp"
	"CtrDrbgContext.cpp"
	"EntropyContext.cpp"
	"SslContext.cpp"
	"X509Cert.cpp"
)

set(HEADERS
	"BlockingSslClientSocket.h"
	"BufferedSslSocket.h"
	"CallbackSslContext.h"
	"CtrDrbgContext.h"
	"EntropyContext.h"
	"SslContext.h"
	"X509Cert.h"
)

add_library(PolarSSL++ ${SOURCES} ${HEADERS})

if (UNIX)
	target_link_libraries(PolarSSL++ polarssl)
endif()
