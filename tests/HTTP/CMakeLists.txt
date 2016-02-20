cmake_minimum_required (VERSION 2.6) 

enable_testing()

include_directories(${CMAKE_SOURCE_DIR}/src/)
include_directories(${CMAKE_SOURCE_DIR}/lib/libevent/include)

add_definitions(-DTEST_GLOBALS=1)

# Create a single HTTP library that contains all the HTTP code:
set (HTTP_SRCS
	${CMAKE_SOURCE_DIR}/src/HTTP/EnvelopeParser.cpp
	${CMAKE_SOURCE_DIR}/src/HTTP/HTTPMessage.cpp
	${CMAKE_SOURCE_DIR}/src/HTTP/HTTPMessageParser.cpp
	${CMAKE_SOURCE_DIR}/src/HTTP/TransferEncodingParser.cpp
	${CMAKE_SOURCE_DIR}/src/StringUtils.cpp
)

set (HTTP_HDRS
	${CMAKE_SOURCE_DIR}/src/HTTP/EnvelopeParser.h
	${CMAKE_SOURCE_DIR}/src/HTTP/HTTPMessage.h
	${CMAKE_SOURCE_DIR}/src/HTTP/HTTPMessageParser.h
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

# HTTPMessageParser_file: Feed file contents into a cHTTPResponseParser and print the callbacks as they're called:
add_executable(HTTPMessageParser_file-exe HTTPMessageParser_file.cpp)
target_link_libraries(HTTPMessageParser_file-exe HTTP)

# Test parsing the response file in 2-byte chunks (should go from response line parsing through headers parsing to body parsing, each within a different step):
add_test(NAME HTTPMessageParser_file-test1-2 COMMAND HTTPMessageParser_file-exe HTTPResponse1.data 2)

# Test parsing the response file in 128-byte chunks (should parse response line and part of headers in one step, the rest in another step):
add_test(NAME HTTPMessageParser_file-test1-128 COMMAND HTTPMessageParser_file-exe HTTPResponse1.data 128)

# Test parsing a chunked-encoding response:
add_test(NAME HTTPMessageParser_file-test2 COMMAND HTTPMessageParser_file-exe HTTPResponse2.data)

# Test parsing the request file in 2-byte chunks (should go from request line parsing through headers parsing to body parsing, each within a different step):
add_test(NAME HTTPMessageParser_file-test3-2 COMMAND HTTPMessageParser_file-exe HTTPRequest1.data 2)

# Test parsing the request file in 512-byte chunks (should process everything in a single call):
add_test(NAME HTTPMessageParser_file-test4-512 COMMAND HTTPMessageParser_file-exe HTTPRequest1.data 512)

