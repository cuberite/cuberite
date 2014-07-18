
cmake_minimum_required (VERSION 2.6)
project (MCServer)

include_directories ("${PROJECT_SOURCE_DIR}/../")

SET (SRCS
	DelayedFluidSimulator.cpp
	FireSimulator.cpp
	FloodyFluidSimulator.cpp
	FluidSimulator.cpp
	IncrementalRedstoneSimulator.cpp
	RedstoneSimulator.cpp
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
	IncrementalRedstoneSimulator.h
	NoopFluidSimulator.h
	NoopRedstoneSimulator.h
	RedstoneSimulator.h
	SandSimulator.h
	Simulator.h
	SimulatorManager.h
	VanillaFluidSimulator.h
	VaporizeFluidSimulator.h)

if(NOT MSVC)
	add_library(Simulator ${SRCS} ${HDRS})
endif()
