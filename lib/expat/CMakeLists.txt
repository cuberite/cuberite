
cmake_minimum_required (VERSION 2.6)
project (expat)

file(GLOB SOURCE
    "*.c"
)

# add headers to MSVC project files:
if (WIN32)
	file(GLOB HEADERS "*.h")
	set(SOURCE ${SOURCE} ${HEADERS})
	source_group("Sources" FILES ${SOURCE})
endif()

add_library(expat ${SOURCE})
