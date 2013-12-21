
cmake_minimum_required (VERSION 2.6)
project (zlib)

include_directories ("${PROJECT_SOURCE_DIR}/../../src/")

file(GLOB SOURCE
    "*.c"
)

add_library(zlib ${SOURCE})
