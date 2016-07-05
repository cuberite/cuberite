
cmake_minimum_required (VERSION 2.6)
project (Cuberite)

include_directories ("${PROJECT_SOURCE_DIR}/../")

SET (SRCS
	DelayedFluidSimulator.cpp
	FireSimulator.cpp
	FloodyFluidSimulator.cpp
	FluidSimulator.cpp
	SandSimulator.cpp
	Simulator.cpp
	SimulatorManager.cpp
	VanillaFluidSimulator.cpp
	VaporizeFluidSimulator.cpp)

SET (HDRS
	DelayedFluidSimulator.h
	FireSimulator.h
	FloodyFluidSimulator.h
	FluidSimulator.h
	NoopFluidSimulator.h
	NoopRedstoneSimulator.h
	RedstoneSimulator.h
	SandSimulator.h
	Simulator.h
	SimulatorManager.h
	VanillaFluidSimulator.h
	VaporizeFluidSimulator.h)


if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
	set_source_files_properties(FireSimulator.cpp PROPERTIES COMPILE_FLAGS "-Wno-error=sign-conversion")
	set_source_files_properties(FluidSimulator.cpp PROPERTIES COMPILE_FLAGS "-Wno-error=sign-conversion -Wno-error=shadow")
endif()

if(NOT MSVC)
	add_library(Simulator ${SRCS} ${HDRS})
endif()
