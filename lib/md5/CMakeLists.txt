
cmake_minimum_required (VERSION 2.6)
project (md5)

include_directories ("${PROJECT_SOURCE_DIR}/../../src/")

file(GLOB SOURCE
    "*.cpp"
    "*.h"
)

add_library(md5 ${SOURCE})
