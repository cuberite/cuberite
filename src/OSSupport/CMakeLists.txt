
cmake_minimum_required (VERSION 2.6)
project (MCServer)

include_directories ("${PROJECT_SOURCE_DIR}/../")
file(GLOB SOURCE
    "*.cpp"
    "*.h"
)

add_library(OSSupport ${SOURCE})

if(UNIX)
	target_link_libraries(OSSupport pthread)
endif()
