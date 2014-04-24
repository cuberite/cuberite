
cmake_minimum_required (VERSION 2.6)
project (MCServer)

include_directories ("${PROJECT_SOURCE_DIR}/../")

set(SOURCES
	"EntropyContext.cpp"
	"CtrDrbgContext.cpp"
	"x509Cert.cpp"
)

add_library(PolarSSL++ ${SOURCES})

if (UNIX)
	target_link_libraries(PolarSSL++ polarssl)
endif()
