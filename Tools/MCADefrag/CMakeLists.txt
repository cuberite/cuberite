cmake_minimum_required(VERSION 3.5)
project (MCADefrag)

# Without this, the MSVC variable isn't defined for MSVC builds ( https://www.cmake.org/pipermail/cmake/2011-November/047130.html )
enable_language(CXX C)

include(FlattenFiles)
include(SetFlags)
set_flags()
set_lib_flags()
enable_profile()


set_exe_flags()

# Include the shared files:
set(SHARED_SRC
	../../src/StringCompression.cpp
	../../src/StringUtils.cpp
	../../src/LoggerListeners.cpp
	../../src/Logger.cpp
)
set(SHARED_HDR
	../../src/ByteBuffer.h
	../../src/StringUtils.h
)

flatten_files(SHARED_SRC)
flatten_files(SHARED_HDR)
source_group("Shared" FILES ${SHARED_SRC} ${SHARED_HDR})



# Include the main source files:
set(SOURCES
	MCADefrag.cpp
	Globals.cpp
)
set(HEADERS
	MCADefrag.h
	Globals.h
)

source_group("" FILES ${SOURCES} ${HEADERS})

add_executable(MCADefrag
	${SOURCES}
	${HEADERS}
	${SHARED_SRC}
	${SHARED_HDR}
)
target_include_directories(MCADefrag PRIVATE ../../src/) # For the SHARED_HDRs
target_link_libraries(MCADefrag PRIVATE zlib fmt::fmt OSSupport)
