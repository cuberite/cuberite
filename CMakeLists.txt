
cmake_minimum_required (VERSION 2.6)
project (MCServer)

set(CMAKE_CXX_FLAGS_BAK ${CMAKE_CXX_FLAGS})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -w")

add_subdirectory(lib/inifile/)
add_subdirectory(lib/jsoncpp/)
add_subdirectory(lib/cryptopp/)

#TODo: set -Wall -Werror -Wextra
set( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS_BAK}" )

add_subdirectory (src) 

