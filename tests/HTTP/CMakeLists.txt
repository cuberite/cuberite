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
add_test(NAME HTTPResponseParser_file-test1 COMMAND HTTPResponseParser_file-exe HTTPResponse1.data)
add_test(NAME HTTPResponseParser_file-test2 COMMAND HTTPResponseParser_file-exe HTTPResponse2.data)
