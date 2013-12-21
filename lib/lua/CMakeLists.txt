
cmake_minimum_required (VERSION 2.6)
project (lua)

include_directories ("${PROJECT_SOURCE_DIR}/../../src/")

file(GLOB SOURCE
	"src/*.c"
)

if(${STATIC_LUA})
add_library(lua ${SOURCE})
else()
add_library(lua SHARED ${SOURCE})
endif()

if(UNIX)
target_link_libraries(lua m dl)
endif()
