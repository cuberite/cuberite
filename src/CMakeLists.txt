
cmake_minimum_required (VERSION 2.6)
project (MCServer)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++03")
endif()

include_directories (SYSTEM "${PROJECT_SOURCE_DIR}/../lib/")
include_directories (SYSTEM "${PROJECT_SOURCE_DIR}/../lib/jsoncpp/include")

add_subdirectory(OSSupport)
add_subdirectory(HTTPServer)
add_subdirectory(Bindings)
add_subdirectory(Items)
add_subdirectory(Blocks)
add_subdirectory(Protocol)
add_subdirectory(Generating)
add_subdirectory(WorldStorage)
add_subdirectory(Mobs)
add_subdirectory(Entities)
add_subdirectory(Simulator)
add_subdirectory(UI)


file(GLOB SOURCE
    "*.cpp"
)

list(REMOVE_ITEM SOURCE "${PROJECT_SOURCE_DIR}/StackWalker.cpp" "${PROJECT_SOURCE_DIR}/LeakFinder.cpp")

add_executable(../MCServer/MCServer ${SOURCE})

target_link_libraries(../MCServer/MCServer OSSupport HTTPServer iniFile Bindings Items Blocks)
target_link_libraries(../MCServer/MCServer Protocol Generating WorldStorage jsoncpp cryptopp)
target_link_libraries(../MCServer/MCServer Mobs Entities Simulator UI zlib)
