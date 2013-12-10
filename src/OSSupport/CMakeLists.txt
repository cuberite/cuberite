
cmake_minimum_required (VERSION 2.6)
project (MCServer)

include_directories ("${PROJECT_SOURCE_DIR}/../")
file(GLOB SOURCE
    "*.h"
    "*.cpp"
)

add_library(OSSupport ${SOURCE})
