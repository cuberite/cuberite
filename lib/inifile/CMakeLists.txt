cmake_minimum_required (VERSION 2.6)
project (iniFile)

include_directories ("${PROJECT_SOURCE_DIR}/../../src/")

file(GLOB SOURCE
    "*.h"
    "*.cpp"
)

add_library(iniFile ${SOURCE})
