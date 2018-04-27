project(GeneratorPerformanceTest)

set(SOURCES
	GeneratorPerformanceTest.cpp
)

add_executable(GeneratorPerformanceTest ${SOURCES})
target_include_directories(GeneratorPerformanceTest PRIVATE ../../src/) # TODO: Just for Globals.h! Remove this dependency
target_link_libraries(GeneratorPerformanceTest PRIVATE Generating fmt::fmt OSSupport) # TODO fmt also just for Globals.h
