
cmake_minimum_required (VERSION 2.6)
project (sqlite)

include_directories ("${PROJECT_SOURCE_DIR}/../")

file(GLOB SOURCE
    "*.c"
)


# Lua is required as a DLL for LuaSQLite:
if (WIN32)
	add_definitions(-DLUA_BUILD_AS_DLL)
endif()


# add headers to MSVC project files:
if (MSVC)
	file(GLOB HEADERS "src/*.h")
	list(REMOVE_ITEM SOURCE "${PROJECT_SOURCE_DIR}/src/lua.h" "${PROJECT_SOURCE_DIR}/src/luac.h")
	set(SOURCE ${SOURCE} ${HEADERS})
	source_group("Sources" FILES ${SOURCE})
endif()

# FreeBSD requires us to define this to get POSIX 2001 standard
if (${CMAKE_SYSTEM_NAME} STREQUAL "FreeBSD")
	add_flags_cxx("-D_XOPEN_SOURCE=600")
endif()

add_library(sqlite ${SOURCE})
target_link_libraries(sqlite lua)

if (UNIX)
	target_link_libraries(sqlite ${DYNAMIC_LOADER})
endif()
