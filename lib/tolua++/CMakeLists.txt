
cmake_minimum_required (VERSION 2.6)
project (tolua++)

include_directories ("${PROJECT_SOURCE_DIR}/../../src/")
include_directories ("${PROJECT_SOURCE_DIR}/include/")
include_directories ("${PROJECT_SOURCE_DIR}/../")

file(GLOB SOURCE
    "src/bin/*.c"
	"src/lib/*.c"
)

add_executable(tolua ${SOURCE})

#m is the standard math librarys
target_link_libraries(tolua lua m)
