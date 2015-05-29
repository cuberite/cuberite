
cmake_minimum_required (VERSION 2.6)
project (MCServer)

include_directories ("${PROJECT_SOURCE_DIR}/../")

SET (SRCS
	Noise.cpp
)

SET (HDRS
	Noise.h
	OctavedNoise.h
	RidgedNoise.h
)

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
	set_source_files_properties(Noise.cpp PROPERTIES COMPILE_FLAGS "-Wno-error=old-style-cast")
endif()

if(NOT MSVC)
	add_library(Noise ${SRCS} ${HDRS})

	target_link_libraries(Noise OSSupport)
endif()
