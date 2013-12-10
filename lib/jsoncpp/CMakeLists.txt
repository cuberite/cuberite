
cmake_minimum_required (VERSION 2.6)
project (jsoncpp)

include_directories ("${PROJECT_SOURCE_DIR}/../../src/")

file(GLOB SOURCE
    "src/lib_json/*.h"
    "src/lib_json/*.cpp"
)


add_library(jsoncpp ${SOURCE})
