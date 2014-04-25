
cmake_minimum_required (VERSION 2.6)
project (expat)

file(GLOB SOURCE
    "*.c"
    "*.h"
)

# Set files to go to a "Sources" folder in MSVC project files:
if (MSVC)
	source_group("Sources" FILES ${SOURCE})
endif()

add_library(expat ${SOURCE})
