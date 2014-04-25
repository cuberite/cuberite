
cmake_minimum_required (VERSION 2.6)
project (MCServer)

include_directories ("${PROJECT_SOURCE_DIR}/../")

file(GLOB SOURCE
    "*.cpp"
    "*.h"
)

add_library(Generating ${SOURCE})

target_link_libraries(Generating OSSupport iniFile Blocks)
