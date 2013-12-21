
cmake_minimum_required (VERSION 2.6)
project (sqlite)

include_directories ("${PROJECT_SOURCE_DIR}/../../src/")
include_directories ("${PROJECT_SOURCE_DIR}/../")

file(GLOB SOURCE
    "*.c"
)

add_library(sqlite ${SOURCE})

target_link_libraries(sqlite dl)
