include_directories(${PROJECT_SOURCE_DIR}/src/)
include_directories(SYSTEM ${PROJECT_SOURCE_DIR}/lib/mbedtls/include)

# Create a single HTTP library that contains all the HTTP code:
set (HTTP_SRCS
	${PROJECT_SOURCE_DIR}/src/HTTP/EnvelopeParser.cpp
	${PROJECT_SOURCE_DIR}/src/HTTP/HTTPMessage.cpp
	${PROJECT_SOURCE_DIR}/src/HTTP/HTTPMessageParser.cpp
	${PROJECT_SOURCE_DIR}/src/HTTP/TransferEncodingParser.cpp
	${PROJECT_SOURCE_DIR}/src/HTTP/UrlClient.cpp
	${PROJECT_SOURCE_DIR}/src/HTTP/UrlParser.cpp
	${PROJECT_SOURCE_DIR}/src/OSSupport/StackTrace.cpp
	${PROJECT_SOURCE_DIR}/src/OSSupport/WinStackWalker.cpp
	${PROJECT_SOURCE_DIR}/src/StringUtils.cpp
)

set (HTTP_HDRS
	${PROJECT_SOURCE_DIR}/src/HTTP/EnvelopeParser.h
	${PROJECT_SOURCE_DIR}/src/HTTP/HTTPMessage.h
	${PROJECT_SOURCE_DIR}/src/HTTP/HTTPMessageParser.h
	${PROJECT_SOURCE_DIR}/src/HTTP/TransferEncodingParser.h
	${PROJECT_SOURCE_DIR}/src/HTTP/UrlClient.h
	${PROJECT_SOURCE_DIR}/src/HTTP/UrlParser.h
	${PROJECT_SOURCE_DIR}/src/OSSupport/StackTrace.h
	${PROJECT_SOURCE_DIR}/src/OSSupport/WinStackWalker.h
	${PROJECT_SOURCE_DIR}/src/StringUtils.h
)

set (SHARED_SRCS
	${PROJECT_SOURCE_DIR}/src/OSSupport/Event.cpp
)

add_library(HTTP
	${HTTP_SRCS}
	${HTTP_HDRS}
)
target_link_libraries(HTTP Network OSSupport)





set (TEST_DATA_FILES
	HTTPRequest1.data
	HTTPRequest2.data
	HTTPResponse1.data
	HTTPResponse2.data
)

source_group("Data Files" FILES ${TEST_DATA_FILES})




# Define individual test executables:

# HTTPMessageParser_file: Feed file contents into a cHTTPResponseParser and print the callbacks as they're called:
add_executable(HTTPMessageParser_file-exe HTTPMessageParser_file.cpp ${TEST_DATA_FILES})
target_link_libraries(HTTPMessageParser_file-exe HTTP Network OSSupport fmt::fmt)

# UrlClientTest: Tests the UrlClient class by requesting a few things off the internet:
add_executable(UrlClientTest-exe UrlClientTest.cpp)
target_link_libraries(UrlClientTest-exe HTTP fmt::fmt)





# Define individual tests:

# Test parsing the response file in 2-byte chunks (should go from response line parsing through headers parsing to body parsing, each within a different step):
add_test(NAME HTTPMessageParser_file-test1-2 COMMAND HTTPMessageParser_file-exe ${CMAKE_CURRENT_SOURCE_DIR}/HTTPResponse1.data 2)

# Test parsing the response file in 128-byte chunks (should parse response line and part of headers in one step, the rest in another step):
add_test(NAME HTTPMessageParser_file-test1-128 COMMAND HTTPMessageParser_file-exe ${CMAKE_CURRENT_SOURCE_DIR}/HTTPResponse1.data 128)

# Test parsing a chunked-encoding response:
add_test(NAME HTTPMessageParser_file-test2 COMMAND HTTPMessageParser_file-exe ${CMAKE_CURRENT_SOURCE_DIR}/HTTPResponse2.data)

# Test parsing the request file in 2-byte chunks (should go from request line parsing through headers parsing to body parsing, each within a different step):
add_test(NAME HTTPMessageParser_file-test3-2 COMMAND HTTPMessageParser_file-exe ${CMAKE_CURRENT_SOURCE_DIR}/HTTPRequest1.data 2)

# Test parsing the request file in 512-byte chunks (should process everything in a single call):
add_test(NAME HTTPMessageParser_file-test4-512 COMMAND HTTPMessageParser_file-exe ${CMAKE_CURRENT_SOURCE_DIR}/HTTPRequest1.data 512)

# Test the URLClient
add_test(NAME UrlClient-test COMMAND UrlClientTest-exe)





# Put all the tests into a solution folder (MSVC):
set_target_properties(
	HTTPMessageParser_file-exe
	UrlClientTest-exe
	PROPERTIES FOLDER Tests/HTTP
)
set_target_properties(
	HTTP
	PROPERTIES FOLDER Tests/Libraries
)
