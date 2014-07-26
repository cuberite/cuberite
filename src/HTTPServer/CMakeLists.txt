
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

if(NOT MSVC)
	add_library(HTTPServer ${SRCS} ${HDRS})
endif()
