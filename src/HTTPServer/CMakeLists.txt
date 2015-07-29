
cmake_minimum_required (VERSION 2.6)
project (MCServer)

include_directories ("${PROJECT_SOURCE_DIR}/../")

SET (SRCS
	EnvelopeParser.cpp
	HTTPConnection.cpp
	HTTPFormParser.cpp
	HTTPMessage.cpp
	HTTPServer.cpp
	MultipartParser.cpp
	NameValueParser.cpp
	SslHTTPConnection.cpp)

SET (HDRS
	EnvelopeParser.h
	HTTPConnection.h
	HTTPFormParser.h
	HTTPMessage.h
	HTTPServer.h
	MultipartParser.h
	NameValueParser.h
	SslHTTPConnection.h)

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
	set_source_files_properties(HTTPServer.cpp PROPERTIES COMPILE_FLAGS "-Wno-error=global-constructors ")
	set_source_files_properties(HTTPConnection.cpp PROPERTIES COMPILE_FLAGS "-Wno-error=switch-enum")
	set_source_files_properties(HTTPMessage.cpp PROPERTIES COMPILE_FLAGS "-Wno-error=tautological-compare")
endif()

if(NOT MSVC)
	add_library(HTTPServer ${SRCS} ${HDRS})
endif()
