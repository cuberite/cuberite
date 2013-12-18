
cmake_minimum_required (VERSION 2.6)
project (MCServer)

set(CMAKE_CXX_FLAGS_BAK ${CMAKE_CXX_FLAGS})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -w")
set(CMAKE_BUILD_TYPE_BAK ${CMAKE_BUILD_TYPE})
set(CMAKE_BUILD_TYPE "Release")

add_subdirectory(lib/inifile/)
add_subdirectory(lib/jsoncpp/)
add_subdirectory(lib/cryptopp/)
add_subdirectory(lib/zlib/)
add_subdirectory(lib/lua/)
add_subdirectory(lib/tolua++/)

#TODo: set -Wall -Werror -Wextra
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS_BAK}")
set(CMAKE_BUILD_TYPE "${CMAKE_BUILD_TYPE_BAK}")

add_subdirectory (src) 

