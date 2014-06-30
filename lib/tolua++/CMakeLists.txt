
cmake_minimum_required (VERSION 2.6)
project (tolua++)

include_directories ("${PROJECT_SOURCE_DIR}/../../src/")
include_directories ("${PROJECT_SOURCE_DIR}/include/")
include_directories ("${PROJECT_SOURCE_DIR}/../")
include_directories ("${PROJECT_SOURCE_DIR}")

find_program(XXD_EXECUTABLE xxd)

if(NOT XXD_EXECUTABLE STREQUAL "XXD_EXECUTABLE-NOTFOUND")
	add_custom_command(OUTPUT ${PROJECT_SOURCE_DIR}/src/bin/basic_lua.h
					COMMAND ${XXD_EXECUTABLE} -i lua/basic.lua | sed 's/unsigned char/static const unsigned char/g' >basic_lua.h
					WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/src/bin/
					DEPENDS ${PROJECT_SOURCE_DIR}/src/bin/lua/basic.lua)
	add_custom_command(OUTPUT ${PROJECT_SOURCE_DIR}/src/bin/enumerate_lua.h
					COMMAND ${XXD_EXECUTABLE} -i lua/enumerate.lua | sed 's/unsigned char/static const unsigned char/g' >enumerate_lua.h
					WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/src/bin/
					DEPENDS ${PROJECT_SOURCE_DIR}/src/bin/lua/enumerate.lua)
	add_custom_command(OUTPUT ${PROJECT_SOURCE_DIR}/src/bin/function_lua.h
					COMMAND ${XXD_EXECUTABLE} -i lua/function.lua | sed 's/unsigned char/static const unsigned char/g' >function_lua.h
					WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/src/bin/
					DEPENDS ${PROJECT_SOURCE_DIR}/src/bin/lua/function.lua)
	add_custom_command(OUTPUT ${PROJECT_SOURCE_DIR}/src/bin/declaration_lua.h
					COMMAND ${XXD_EXECUTABLE} -i lua/declaration.lua | sed 's/unsigned char/static const unsigned char/g' >declaration_lua.h
					WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/src/bin/
					DEPENDS ${PROJECT_SOURCE_DIR}/src/bin/lua/declaration.lua)
	add_custom_command(OUTPUT ${PROJECT_SOURCE_DIR}/src/bin/container_lua.h
					COMMAND ${XXD_EXECUTABLE} -i lua/container.lua | sed 's/unsigned char/static const unsigned char/g' >container_lua.h
					WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/src/bin/
					DEPENDS ${PROJECT_SOURCE_DIR}/src/bin/lua/container.lua)
	set_property(SOURCE src/bin/toluabind.c APPEND PROPERTY OBJECT_DEPENDS ${PROJECT_SOURCE_DIR}/src/bin/enumerate_lua.h ${PROJECT_SOURCE_DIR}/src/bin/basic_lua.h ${PROJECT_SOURCE_DIR}/src/bin/function_lua.h ${PROJECT_SOURCE_DIR}/src/bin/declaration_lua.h ${PROJECT_SOURCE_DIR}/src/bin/container_lua.h)
else()
	message("xxd not found, changes to tolua scripts will be ignored")
endif()


file(GLOB LIB_SOURCE
    "src/lib/*.c"
)

file(GLOB BIN_SOURCE
    "src/bin/*.c"
)

add_executable(tolua ${BIN_SOURCE})
add_library(tolualib ${LIB_SOURCE})
target_link_libraries(tolualib lua)

#m is the standard math librarys
if(UNIX)
target_link_libraries(tolua m ${DYNAMIC_LOADER})
endif()

target_link_libraries(tolua tolualib lua)
