# This is the top-level CMakeLists.txt file for the Cuberite project
#
# Use CMake to generate the build files for your platform
#
# This script supports some configuration through CMake arguments (-Dparam=val syntax):
#   BUILD_TOOLS=1                    sets up additional executables to be built along with the server (ProtoProxy, GrownBiomeGenVisualiser, MCADefrag)
#   BUILD_UNSTABLE_TOOLS=1           sets up yet more executables to be built, these can be broken and generally are obsolete (GeneratorPerformanceTest)
#   NO_NATIVE_OPTIMIZATION=1         disables CPU-specific optimisations for the current machine, allows use on other CPUs of the same platform
#   DISABLE_SYSTEM_LUA=1             disables the use of system Lua interpreter; the tolua executable will be built and used instead. Incompatible with cross-compiling
#   SELF_TEST=1                      enables testing code to be built
#   UNITY_BUILDS=OFF                 disables unity builds
#   PRECOMPILE_HEADERS=OFF           disables precompiled headers
#   WHOLE_PROGRAM_OPTIMISATION=OFF   disables link time optimisation





cmake_minimum_required (VERSION 3.13)
cmake_policy(VERSION 3.13...3.17.2)
project(
	Cuberite
	DESCRIPTION "A lightweight, fast and extensible game server for Minecraft"
	HOMEPAGE_URL "https://cuberite.org"
	LANGUAGES C CXX
)

option(BUILD_TOOLS "Sets up additional executables to be built along with the server" OFF)
option(WHOLE_PROGRAM_OPTIMISATION "Enables link time optimisation for Release" ON)
option(PRECOMPILE_HEADERS "Enable precompiled headers for faster builds" ON)
option(SELF_TEST "Enables testing code to be built" OFF)
option(UNITY_BUILDS "Enables source aggregation for faster builds" ON)

include("CMake/AddDependencies.cmake")
include("CMake/Fixups.cmake")
include("CMake/GenerateBindings.cmake")
include("CMake/GroupSources.cmake")
include("SetFlags.cmake")

# Add build timestamp and details:
include("CMake/StampBuild.cmake")

# We need C++17 features:
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# TODO: use standard NDEBUG in place of _DEBUG
if("${CMAKE_BUILD_TYPE}" MATCHES "DEBUG")
	add_definitions(-D_DEBUG)
endif()

# The need for speed (in Release):
if(WHOLE_PROGRAM_OPTIMISATION)
	include(CheckIPOSupported)
	check_ipo_supported(RESULT IPO_SUPPORTED)
	set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_RELEASE ${IPO_SUPPORTED})
endif()

# Static CRT:
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

# TODO: set_build_stamp()
set_global_flags()
build_dependencies()

add_executable(${CMAKE_PROJECT_NAME})
add_subdirectory(src)

set_exe_flags(${CMAKE_PROJECT_NAME})
link_dependencies(${CMAKE_PROJECT_NAME})

# Set the startup project to Cuberite, and the debugger dir:
set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT ${CMAKE_PROJECT_NAME})
set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/Server")

# Enable PCH and jumbo builds on supporting CMake:
if (CMAKE_VERSION VERSION_GREATER_EQUAL "3.16")
	if (PRECOMPILE_HEADERS)
		target_precompile_headers(${CMAKE_PROJECT_NAME} PRIVATE src/Globals.h)
	endif()

	set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES UNITY_BUILD ${UNITY_BUILDS})
else()
	message(WARNING "Precompiled headers for FASTER BUILDS not enabled, upgrade to CMake 1.16 or newer!")
endif()

# Selectively disable warnings in the level where the target is created:
if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
	# Generated file has old-style casts, missing prototypes, and deprecated declarations
	set_source_files_properties("${PROJECT_SOURCE_DIR}/src/Bindings/Bindings.cpp" PROPERTIES COMPILE_OPTIONS -Wno-everything)

	# File failed to follow NHS guidelines on handwashing and has not maintained good hygiene
	set_source_files_properties("${PROJECT_SOURCE_DIR}/src/IniFile.cpp" PROPERTIES COMPILE_OPTIONS -Wno-header-hygiene)
endif()

if(${BUILD_TOOLS})
	message("Building tools")
	add_subdirectory(Tools/GrownBiomeGenVisualiser/)
	add_subdirectory(Tools/MCADefrag/)
	add_subdirectory(Tools/NoiseSpeedTest/)
	add_subdirectory(Tools/ProtoProxy/)
endif()

if(${BUILD_UNSTABLE_TOOLS})
	message("Building unstable tools")
	add_subdirectory(Tools/GeneratorPerformanceTest/)
endif()

# Self Test Mode enables extra checks at startup
if(${SELF_TEST})
	message("Tests enabled")
	enable_testing()
	add_subdirectory(tests)
endif()

emit_fixups()
group_sources()
enable_bindings_generation()
