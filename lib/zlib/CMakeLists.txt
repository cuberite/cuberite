
cmake_minimum_required (VERSION 2.6)
project (zlib)

include_directories ("${PROJECT_SOURCE_DIR}/../../src/")

file(GLOB SOURCE
    "*.c"
    "*.h"
)

if(NOT TARGET zlib)
	add_library(zlib ${SOURCE})

	if (MSVC)
		# Remove SCL warnings, we expect this library to have been tested safe
		SET_TARGET_PROPERTIES(
			zlib PROPERTIES COMPILE_FLAGS "-D_CRT_SECURE_NO_WARNINGS"
		)
	endif()
endif()

