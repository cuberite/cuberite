
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
	source_group("Source Files\\${PATH}" FILES ${FOLDER_FILES}) 
endfunction(includefolder) 

foreach(folder ${FOLDERS})
	includefolder(${folder})
endforeach(folder)

file(GLOB_RECURSE SOURCE
    "*.cpp"
)


endif()

list(REMOVE_ITEM SOURCE "${PROJECT_SOURCE_DIR}/StackWalker.cpp" "${PROJECT_SOURCE_DIR}/LeakFinder.cpp")

add_executable(../MCServer/MCServer ${SOURCE})

target_link_libraries(../MCServer/MCServer OSSupport HTTPServer iniFile Bindings Items Blocks)
target_link_libraries(../MCServer/MCServer Protocol Generating WorldStorage jsoncpp cryptopp)
target_link_libraries(../MCServer/MCServer Mobs Entities Simulator UI zlib lua BlockEntities)
target_link_libraries(../MCServer/MCServer md5 luaexpat)
