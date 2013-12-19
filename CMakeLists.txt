
cmake_minimum_required (VERSION 2.6)
project (MCServer)

if(WIN32)
	set(CMAKE_CXX_FLAGS    "${CMAKE_CXX_FLAGS} /MP")
	set(CMAKE_C_FLAGS      "${CMAKE_C_FLAGS} /MP")
endif()

set(CMAKE_CXX_FLAGS_BAK ${CMAKE_CXX_FLAGS})
set(CMAKE_C_FLAGS_BAK   ${CMAKE_C_FLAGS})
if (UNIX)
	set(CMAKE_CXX_FLAGS    "${CMAKE_CXX_FLAGS} -w")
	set(CMAKE_C_FLAGS      "${CMAKE_C_FLAGS} -w")
else()
	#remove /W3 from command line -- cannot just cancel it later with /w like in unix because of D9025
	string(REPLACE "/W3" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
	string(REPLACE "/W3" "" CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
endif()
set(CMAKE_BUILD_TYPE_BAK ${CMAKE_BUILD_TYPE})
set(CMAKE_BUILD_TYPE "Release")

add_subdirectory(lib/inifile/)
add_subdirectory(lib/jsoncpp/)
add_subdirectory(lib/cryptopp/)
add_subdirectory(lib/zlib/)
add_subdirectory(lib/lua/)
add_subdirectory(lib/tolua++/)
add_subdirectory(lib/sqlite/)
add_subdirectory(lib/expat/)
add_subdirectory(lib/luaexpat/)
add_subdirectory(lib/md5/)

#TODo: set -Wall -Werror -Wextra
if(UNIX)
	set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS_BAK} -Wall -Wextra")
	set(CMAKE_C_FLAGS    "${CMAKE_C_FLAGS_BAK} -Wall -Wextra")
else()
	set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS_BAK} /Wall")
	set(CMAKE_C_FLAGS    "${CMAKE_C_FLAGS_BAK} /Wall")
endif()
set(CMAKE_BUILD_TYPE "${CMAKE_BUILD_TYPE_BAK}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -rdynamic")

add_subdirectory (src) 

