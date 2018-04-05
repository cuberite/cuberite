cmake_minimum_required(VERSION 3.5)
project (GrownBiomeGenVisualiser)

# Without this, the MSVC variable isn't defined for MSVC builds ( https://www.cmake.org/pipermail/cmake/2011-November/047130.html )
enable_language(CXX C)

include(../../SetFlags.cmake)
set_flags()
set_lib_flags()
enable_profile()


function(flatten_files arg1)
	set(res "")
	foreach(f ${${arg1}})
		get_filename_component(f ${f} ABSOLUTE)
		list(APPEND res ${f})
	endforeach()
	set(${arg1} "${res}" PARENT_SCOPE)
endfunction()


set_exe_flags()

# Include the shared files:
set(SHARED_SRC
	../../src/StringUtils.cpp
	../../src/Logger.cpp
	../../src/BiomeDef.cpp
)
set(SHARED_HDR
	../../src/StringUtils.h
)

flatten_files(SHARED_SRC)
flatten_files(SHARED_HDR)
source_group("Shared" FILES ${SHARED_SRC} ${SHARED_HDR})



# Include the main source files:
set(SOURCES
	GrownBiomeGenVisualiser.cpp
	Globals.cpp
)
set(HEADERS
	Globals.h
)

source_group("" FILES ${SOURCES} ${HEADERS})

add_executable(GrownBiomeGenVisualiser
	${SOURCES}
	${HEADERS}
	${SHARED_SRC}
	${SHARED_HDR}
)

target_link_libraries(GrownBiomeGenVisualiser PRIVATE fmt::fmt OSSupport Noise)
target_include_directories(GrownBiomeGenVisualiser PRIVATE ../../src) # For SHARED_HDR
set_target_properties(GrownBiomeGenVisualiser PROPERTIES FOLDER Tools)
