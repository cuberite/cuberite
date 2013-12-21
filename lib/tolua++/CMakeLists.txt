
cmake_minimum_required (VERSION 2.6)
project (tolua++)

include_directories ("${PROJECT_SOURCE_DIR}/../../src/")
include_directories ("${PROJECT_SOURCE_DIR}/include/")
include_directories ("${PROJECT_SOURCE_DIR}/../")

file(GLOB LIB_SOURCE
    "src/lib/*.c"
)

file(GLOB BIN_SOURCE
    "src/bin/*.c"
)

add_executable(tolua ${BIN_SOURCE})
add_library(tolualib ${LIB_SOURCE})

#m is the standard math librarys
if(UNIX)
target_link_libraries(tolua m)
endif()

target_link_libraries(tolua lua tolualib)
