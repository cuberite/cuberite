
cmake_minimum_required (VERSION 2.6)
project (lua)

include_directories ("${PROJECT_SOURCE_DIR}/../../src/")

file(GLOB SOURCE
	"src/*.c"
)

list(REMOVE_ITEM SOURCE "${PROJECT_SOURCE_DIR}/src/lua.c" "${PROJECT_SOURCE_DIR}/src/luac.c")

# add headers to MSVC project files:
if (WIN32)
	file(GLOB HEADERS "src/*.h")
	list(REMOVE_ITEM SOURCE "${PROJECT_SOURCE_DIR}/src/lua.h" "${PROJECT_SOURCE_DIR}/src/luac.h")
	set(SOURCE ${SOURCE} ${HEADERS})
	source_group("Sources" FILES ${SOURCE})
endif()

# Lua needs to be linked dynamically on Windows and statically on *nix, so that LuaRocks work
if (WIN32)
	add_library(lua SHARED ${SOURCE})
	set(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/MCServer)

	# Output the executable into the $/MCServer folder, so that MCServer can find it:
	set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/MCServer)
	SET_TARGET_PROPERTIES(${EXECUTABLE} PROPERTIES
		RUNTIME_OUTPUT_DIRECTORY_DEBUG          ${CMAKE_SOURCE_DIR}/MCServer
		RUNTIME_OUTPUT_DIRECTORY_RELEASE        ${CMAKE_SOURCE_DIR}/MCServer
		RUNTIME_OUTPUT_DIRECTORY_DEBUGPROFILE   ${CMAKE_SOURCE_DIR}/MCServer
		RUNTIME_OUTPUT_DIRECTORY_RELEASEPROFILE ${CMAKE_SOURCE_DIR}/MCServer
	)

	if (MSVC)
		# Remove SCL warnings, we expect this library to have been tested safe
		SET_TARGET_PROPERTIES(
			lua PROPERTIES COMPILE_FLAGS "-D_CRT_SECURE_NO_WARNINGS"
		)
	endif()
else()
	add_library(lua ${SOURCE})
endif()

# Tell Lua what dynamic loader to use (for LuaRocks):
if (UNIX)
	add_definitions(-DLUA_USE_DLOPEN)
endif()

if (UNIX)
	target_link_libraries(lua m ${DYNAMIC_LOADER})
endif()

