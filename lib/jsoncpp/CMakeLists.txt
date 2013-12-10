
cmake_minimum_required (VERSION 2.6)
project (jsoncpp)

include_directories ("${PROJECT_SOURCE_DIR}/../../src/")

add_library(jsoncpp src/lib_json/json_value.cpp)
