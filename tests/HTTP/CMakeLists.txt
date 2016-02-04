cmake_minimum_required (VERSION 2.6) 

enable_testing()

include_directories(${CMAKE_SOURCE_DIR}/src/)
include_directories(${CMAKE_SOURCE_DIR}/lib/libevent/include)

add_definitions(-DTEST_GLOBALS=1)

# Create a single HTTP library that contains all the HTTP code:
set (HTTP_SRCS
	${CMAKE_SOURCE_DIR}/src/HTTP/EnvelopeParser.cpp
	${CMAKE_SOURCE_DIR}/src/HTTP/HTTPMessage.cpp
	${CMAKE_SOURCE_DIR}/src/HTTP/HTTPResponseParser.cpp
	${CMAKE_SOURCE_DIR}/src/HTTP/TransferEncodingParser.cpp
	${CMAKE_SOURCE_DIR}/src/StringUtils.cpp
)

set (HTTP_HDRS
	${CMAKE_SOURCE_DIR}/src/HTTP/EnvelopeParser.h
	${CMAKE_SOURCE_DIR}/src/HTTP/HTTPMessage.h
	${CMAKE_SOURCE_DIR}/src/HTTP/HTTPResponseParser.h
	${CMAKE_SOURCE_DIR}/src/HTTP/TransferEncodingParser.h
	${CMAKE_SOURCE_DIR}/src/StringUtils.h
)

add_library(HTTP
	${HTTP_SRCS}
	${HTTP_HDRS}
)

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
	add_flags_cxx("-Wno-error=conversion -Wno-error=old-style-cast")
endif()





# Define individual tests:

# HTTPResponseParser_file: Feed file contents into a cHTTPResponseParser and print the callbacks as they're called:
add_executable(HTTPResponseParser_file-exe HTTPResponseParser_file.cpp)
target_link_libraries(HTTPResponseParser_file-exe HTTP)

# Test parsing the file in 2-byte chunks (should go from response line parsing through headers parsing to body parsing, each within a different step):
add_test(NAME HTTPResponseParser_file-test1-2 COMMAND HTTPResponseParser_file-exe HTTPResponse1.data 2)

# Test parsing the file in 128-byte chunks (should parse response line and part of headers in one step, the rest in another step):
add_test(NAME HTTPResponseParser_file-test1-128 COMMAND HTTPResponseParser_file-exe HTTPResponse1.data 128)

# Test parsing a chunked-encoding content:
add_test(NAME HTTPResponseParser_file-test2 COMMAND HTTPResponseParser_file-exe HTTPResponse2.data)
