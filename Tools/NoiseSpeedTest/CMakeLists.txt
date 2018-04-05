cmake_minimum_required(VERSION 3.5)
project (NoiseSpeedTest)

include(../../SetFlags.cmake)
set_flags()
set_lib_flags()
enable_profile()


set_exe_flags()

# Include the shared files:
set(SHARED_SRC
	../../src/Logger.cpp
	../../src/LoggerListeners.cpp
	../../src/StringUtils.cpp
)

set(SHARED_HDR
	../../src/StringUtils.h
)


source_group("Shared" FILES ${SHARED_SRC} ${SHARED_HDR})




# Include the main source files:
set(SOURCES
	NoiseSpeedTest.cpp
	Globals.cpp
)
set(HEADERS
	NoiseSpeedTest.h
	Globals.h
	SimplexNoise.h
)

source_group("" FILES ${SOURCES} ${HEADERS})

add_executable(NoiseSpeedTest
	${SOURCES}
	${HEADERS}
	${SHARED_SRC}
	${SHARED_HDR}
)
target_include_directories(NoiseSpeedTest PRIVATE ../../src/) # For the SHARED_HDR
target_link_libraries(NoiseSpeedTest PRIVATE fmt::fmt OSSupport Noise)
