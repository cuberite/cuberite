
cmake_minimum_required (VERSION 2.6)
project (lua)

include_directories ("${PROJECT_SOURCE_DIR}/../../src/")

file(GLOB SOURCE
	"*.c"
)

list(REMOVE_ITEM SOURCE "${PROJECT_SOURCE_DIR}/lua.c")

if ((${CMAKE_GENERATOR} MATCHES "Unix Makefiles") AND (NOT LUA_CMAKE_BUILD))
	if(NOT ${PROJECT_SOURCE_DIR} MATCHES ${PROJECT_BINARY_DIR})
		message(WARNING "lua does not support prefix when using the makefile")
	endif()
	ADD_CUSTOM_COMMAND(
   OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/lua
   COMMAND "make"
   DEPENDS ${SOURCE}
   )


else()
	message("generator [${CMAKE_GENERATOR}] does not match Unix Makefiles so manually compiling lua") 

	add_library(lua ${SOURCE})

endif()
