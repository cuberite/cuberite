
cmake_minimum_required (VERSION 2.6)
project (MCServer)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++03")
endif()

include_directories (SYSTEM "${PROJECT_SOURCE_DIR}/../lib/")
include_directories (SYSTEM "${PROJECT_SOURCE_DIR}/../lib/jsoncpp/include")

set(FOLDERS OSSupport HTTPServer Bindings Items Blocks Protocol Generating) 
set(FOLDERS ${FOLDERS} WorldStorage Mobs Entities Simulator UI BlockEntities)

if(NOT WIN32)

foreach(folder ${FOLDERS})
	add_subdirectory(${folder})
endforeach(folder)

file(GLOB SOURCE
    "*.cpp"
)
else()

function(includefolder PATH)
    FILE(GLOB FOLDER_FILES
		"${PATH}/*.cpp"
		"${PATH}/*.h"
	)
	source_group("${PATH}" FILES ${FOLDER_FILES}) 

endfunction(includefolder) 

foreach(folder ${FOLDERS})
	includefolder(${folder})
endforeach(folder)

file(GLOB_RECURSE SOURCE
    "*.cpp"
	"*.h"
)

include_directories("${PROJECT_SOURCE_DIR}")

ADD_CUSTOM_COMMAND(
#add any new generated bindings here
   OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/Bindings/Bindings.cpp ${CMAKE_CURRENT_BINARY_DIR}/Bindings/Bindings.h
#command execuded to regerate bindings
   COMMAND tolua -L virtual_method_hooks.lua -o Bindings/Bindings.cpp -H Bindings/Bindings.h AllToLua.pkg
#add any new generation dependencies here
   DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/Bindings/virtual_method_hooks.lua ${CMAKE_CURRENT_SOURCE_DIR}/Bindings/AllToLua.pkg tolua
   )

source_group("" FILES ${SOURCE}) 

endif()

list(REMOVE_ITEM SOURCE "${PROJECT_SOURCE_DIR}/StackWalker.cpp" "${PROJECT_SOURCE_DIR}/LeakFinder.cpp")

if(UNIX)
	set(EXECUTABLE ../MCServer/MCServer)
else()
	set(EXECUTABLE MCServer)
endif()

add_executable(${EXECUTABLE} ${SOURCE})

target_link_libraries(${EXECUTABLE} OSSupport HTTPServer iniFile Bindings Items Blocks)
target_link_libraries(${EXECUTABLE} Protocol Generating WorldStorage jsoncpp cryptopp)
target_link_libraries(${EXECUTABLE} Mobs Entities Simulator UI zlib lua BlockEntities)
target_link_libraries(${EXECUTABLE} md5 luaexpat)
