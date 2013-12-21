
cmake_minimum_required (VERSION 2.6)
project (MCServer)

macro(add_flags FLAGS)
	set(CMAKE_CXX_FLAGS         "${CMAKE_CXX_FLAGS}         ${FLAGS}")
	set(CMAKE_C_FLAGS           "${CMAKE_C_FLAGS}           ${FLAGS}")
	set(CMAKE_CXX_FLAGS_DEBUG   "${CMAKE_CXX_FLAGS_DEBUG}   ${FLAGS}")
	set(CMAKE_C_FLAGS_DEBUG     "${CMAKE_C_FLAGS_DEBUG}     ${FLAGS}")
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${FLAGS}")
	set(CMAKE_C_FLAGS_RELEASE   "${CMAKE_C_FLAGS_RELEASE}   ${FLAGS}")
	set(CMAKE_CXX_FLAGS_PROFILE "${CMAKE_CXX_FLAGS_PROFILE} ${FLAGS}")
	set(CMAKE_C_FLAGS_PROFILE   "${CMAKE_C_FLAGS_PROFILE}   ${FLAGS}")
endmacro()

SET(CMAKE_CXX_FLAGS_PROFILE
    "${CMAKE_CXX_FLAGS_DEBUG} -pg -DNDEBUG"
    CACHE STRING "Flags used by the C++ compiler during profile builds."
    FORCE )
SET(CMAKE_C_FLAGS_PROFILE
    "${CMAKE_C_FLAGS_DEBUG} -pg -DNDEBUG"
    CACHE STRING "Flags used by the C compiler during profile builds."
    FORCE )
SET(CMAKE_EXE_LINKER_FLAGS_PROFILE
    "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -pg"
    CACHE STRING "Flags used for linking binaries during profile builds."
    FORCE )
SET(CMAKE_SHARED_LINKER_FLAGS_PROFILE
    "${CMAKE_SHARED_LINKER_FLAGS_DEBUG} -pg"
    CACHE STRING "Flags used by the shared libraries linker during profile builds."
    FORCE )
MARK_AS_ADVANCED(
    CMAKE_CXX_FLAGS_PROFILE
    CMAKE_C_FLAGS_PROFILE
    CMAKE_EXE_LINKER_FLAGS_PROFILE
    CMAKE_SHARED_LINKER_FLAGS_PROFILE )

if(UNIX)
set(CMAKE_CXX_FLAGS_DEBUG   "${CMAKE_CXX_FLAGS_DEBUG}   -DNDEBUG")
set(CMAKE_C_FLAGS_DEBUG     "${CMAKE_C_FLAGS_DEBUG}     -DNDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -D_DEBUG")
set(CMAKE_C_FLAGS_RELEASE   "${CMAKE_C_FLAGS_RELEASE}   -D_DEBUG")
endif()

if(WIN32)
	add_flags("/MP")
else()
	add_flags("-pthread")
endif()



set(CMAKE_CXX_FLAGS_RELEASE_BAK "${CMAKE_CXX_FLAGS_RELEASE}")
set(CMAKE_C_FLAGS_RELEASE_BAK   "${CMAKE_C_FLAGS_RELEASE}")
if (UNIX)
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -w")
	set(CMAKE_C_FLAGS_RELEASE   "${CMAKE_C_FLAGS_RELEASE}   -w")
else()
	#remove /W3 from command line -- cannot just cancel it later with /w like in unix because of D9025
	#only remove frome relase as we force release
	string(REPLACE "/W3" "" CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}")
	string(REPLACE "/W3" "" CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE}")
endif()

set(CMAKE_BUILD_TYPE_BAK ${CMAKE_BUILD_TYPE})
set(CMAKE_BUILD_TYPE "Release")

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
	add_definitions(-DCRYPTOPP_DISABLE_ASM)
endif()

add_definitions(-DLUA_USE_DLOPEN)

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

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE_BAK}")
set(CMAKE_C_FLAGS_RELEASE   "${CMAKE_C_FLAGS_RELEASE_BAK}")

#TODo: set -Wall -Werror -Wextra
if(UNIX)
	add_flags("-Wall -Wextra")
else()
	add_flags("/Wall")
endif()
set(CMAKE_BUILD_TYPE "${CMAKE_BUILD_TYPE_BAK}")
set(CMAKE_EXE_LINKER_FLAGS         "${CMAKE_EXE_LINKER_FLAGS}         -rdynamic")
set(CMAKE_EXE_LINKER_FLAGS_DEBUG   "${CMAKE_EXE_LINKER_FLAGS_DEBUG}   -rdynamic")
set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -rdynamic")
set(CMAKE_EXE_LINKER_FLAGS_PROFILE "${CMAKE_EXE_LINKER_FLAGS_PROFILE} -rdynamic")

add_subdirectory (src) 

