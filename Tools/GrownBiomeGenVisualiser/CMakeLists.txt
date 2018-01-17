cmake_minimum_required(VERSION 3.0.2)
project (GrownBiomeGenVisualiser)

# Without this, the MSVC variable isn't defined for MSVC builds ( https://www.cmake.org/pipermail/cmake/2011-November/047130.html )
enable_language(CXX C)

include(../../SetFlags.cmake)
set_flags()
set_lib_flags()
enable_profile()


# Set include paths to the used libraries:
include_directories(SYSTEM "../../lib")
include_directories("../../src")


function(flatten_files arg1)
	set(res "")
	foreach(f ${${arg1}})
		get_filename_component(f ${f} ABSOLUTE)
		list(APPEND res ${f})
	endforeach()
	set(${arg1} "${res}" PARENT_SCOPE)
endfunction()


# Include the libraries:

set_exe_flags()

# Include the shared files:
set(SHARED_SRC
	../../src/StringUtils.cpp
	../../src/Logger.cpp
	../../src/Noise/Noise.cpp
	../../src/BiomeDef.cpp
)
set(SHARED_HDR
	../../src/StringUtils.h
)

flatten_files(SHARED_SRC)
flatten_files(SHARED_HDR)
source_group("Shared" FILES ${SHARED_SRC} ${SHARED_HDR})

set(SHARED_OSS_SRC
	../../src/OSSupport/CriticalSection.cpp
	../../src/OSSupport/Event.cpp
	../../src/OSSupport/File.cpp
	../../src/OSSupport/IsThread.cpp
	../../src/OSSupport/StackTrace.cpp
	../../src/OSSupport/WinStackWalker.cpp
)

set(SHARED_OSS_HDR
	../../src/OSSupport/CriticalSection.h
	../../src/OSSupport/Event.h
	../../src/OSSupport/File.h
	../../src/OSSupport/IsThread.h
	../../src/OSSupport/StackTrace.h
	../../src/OSSupport/WinStackWalker.h
)


flatten_files(SHARED_OSS_SRC)
flatten_files(SHARED_OSS_HDR)

source_group("Shared\\OSSupport" FILES ${SHARED_OSS_SRC} ${SHARED_OSS_HDR})



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
	${SHARED_OSS_SRC}
	${SHARED_OSS_HDR}
)

target_link_libraries(GrownBiomeGenVisualiser fmt::fmt)

set_target_properties(GrownBiomeGenVisualiser PROPERTIES FOLDER Tools)
