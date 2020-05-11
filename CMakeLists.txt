# This is the top-level CMakeLists.txt file for the Cuberite project
#
# Use CMake to generate the build files for your platform
#
# This script supports some configuration through CMake arguments (-Dparam=val syntax):
#   BUILD_TOOLS=1            sets up additional executables to be built along with the server (ProtoProxy, GrownBiomeGenVisualiser, MCADefrag)
#   BUILD_UNSTABLE_TOOLS=1   sets up yet more executables to be built, these can be broken and generally are obsolete (GeneratorPerformanceTest)
#   NO_NATIVE_OPTIMIZATION=1 disables CPU-specific optimisations for the current machine, allows use on other CPUs of the same platform
#   DISABLE_SYSTEM_LUA=1     disables the use of system Lua interpreter; the tolua executable will be built and used instead. Incompatible with cross-compiling
#   SELF_TEST=1              enables testing code to be built





cmake_minimum_required (VERSION 3.13)
project(
	Cuberite
	DESCRIPTION "A lightweight, fast and extensible game server for Minecraft"
	HOMEPAGE_URL "https://cuberite.org"
	LANGUAGES C CXX
)

option(BUILD_TOOLS "Sets up additional executables to be built along with the server" OFF)
option(PRECOMPILE_HEADERS "Enable precompiled headers for faster builds" ON)
option(SELF_TEST "Enables testing code to be built" OFF)
option(UNITY_BUILDS "Enables source aggregation for faster builds" ON)

# These env variables are used for configuring Travis CI builds.
if(DEFINED ENV{TRAVIS_CUBERITE_BUILD_TYPE})
	message("Setting build type to $ENV{TRAVIS_CUBERITE_BUILD_TYPE}")
	set(CMAKE_BUILD_TYPE $ENV{TRAVIS_CUBERITE_BUILD_TYPE})
endif()

if(DEFINED ENV{TRAVIS_CUBERITE_FORCE32})
	set(FORCE32 $ENV{TRAVIS_CUBERITE_FORCE32})
endif()

if(DEFINED ENV{TRAVIS_BUILD_WITH_COVERAGE})
	set(BUILD_WITH_COVERAGE $ENV{TRAVIS_BUILD_WITH_COVERAGE})
endif()

if(DEFINED ENV{CUBERITE_BUILD_ID})
	# The build info is defined by the build system (Travis / Jenkins)
	set(BUILD_ID $ENV{CUBERITE_BUILD_ID})
	set(BUILD_SERIES_NAME $ENV{CUBERITE_BUILD_SERIES_NAME})
	set(BUILD_DATETIME $ENV{CUBERITE_BUILD_DATETIME})
	if(DEFINED ENV{CUBERITE_BUILD_COMMIT_ID})
		set(BUILD_COMMIT_ID $ENV{CUBERITE_BUILD_COMMIT_ID})
	else()
		message("Commit id not set, attempting to determine id from git")
		execute_process(
			COMMAND git rev-parse HEAD
			WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
			RESULT_VARIABLE GIT_EXECUTED
			OUTPUT_VARIABLE BUILD_COMMIT_ID
		)
		string(STRIP ${BUILD_COMMIT_ID} BUILD_COMMIT_ID)
		if (NOT (GIT_EXECUTED EQUAL 0))
			message(FATAL_ERROR "Could not identifiy git commit id")
		endif()
	endif()
else()
	# This is a local build, stuff in some basic info:
	set(BUILD_ID "Unknown")
	set(BUILD_SERIES_NAME "local build")
	execute_process(
		COMMAND git rev-parse HEAD
		WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
		RESULT_VARIABLE GIT_EXECUTED
		OUTPUT_VARIABLE BUILD_COMMIT_ID
	)
	if (NOT(GIT_EXECUTED EQUAL 0))
		set(BUILD_COMMIT_ID "Unknown")
	endif()
	string(STRIP ${BUILD_COMMIT_ID} BUILD_COMMIT_ID)
	execute_process(
		COMMAND git log -1 --date=iso --pretty=format:%ai
		WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
		RESULT_VARIABLE GIT_EXECUTED
		OUTPUT_VARIABLE BUILD_DATETIME
	)
	if (NOT(GIT_EXECUTED EQUAL 0))
		set(BUILD_DATETIME "Unknown")
	endif()
	string(STRIP ${BUILD_DATETIME} BUILD_DATETIME)

	# The BUILD_COMMIT_ID and BUILD_DATETIME aren't updated on each repo pull
	# They are only updated when cmake re-configures the project
	# Therefore mark them as "approx: "
	set(BUILD_COMMIT_ID "approx: ${BUILD_COMMIT_ID}")
	set(BUILD_DATETIME "approx: ${BUILD_DATETIME}")
endif()

# We need C++17 features
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# The need for speed (in Release)
include(CheckIPOSupported)
check_ipo_supported(RESULT IPO_SUPPORTED)
if(IPO_SUPPORTED)
	set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_RELEASE ON)
endif()

# Static CRT
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

# This has to be done before any flags have been set up.
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

include(SetFlags.cmake)
set_flags()
set_lib_flags()
enable_profile()

# Set options for SQLiteCpp, disable all their tests and lints:
set(SQLITECPP_RUN_CPPLINT     OFF CACHE BOOL "Run cpplint.py tool for Google C++ StyleGuide."  FORCE)
set(SQLITECPP_RUN_CPPCHECK    OFF CACHE BOOL "Run cppcheck C++ static analysis tool."          FORCE)
set(SQLITECPP_RUN_DOXYGEN     OFF CACHE BOOL "Run Doxygen C++ documentation tool."             FORCE)
set(SQLITECPP_BUILD_EXAMPLES  OFF CACHE BOOL "Build examples."                                 FORCE)
set(SQLITECPP_BUILD_TESTS     OFF CACHE BOOL "Build and run tests."                            FORCE)
set(SQLITECPP_INTERNAL_SQLITE ON  CACHE BOOL "Add the internal SQLite3 source to the project." FORCE)
set(SQLITE_ENABLE_COLUMN_METADATA OFF CACHE BOOL "" FORCE)

# Set options for LibEvent, disable all their tests and benchmarks:
set(EVENT__DISABLE_OPENSSL   YES CACHE BOOL   "Disable OpenSSL in LibEvent"       FORCE)
set(EVENT__DISABLE_BENCHMARK YES CACHE BOOL   "Disable LibEvent benchmarks"       FORCE)
set(EVENT__DISABLE_TESTS     YES CACHE BOOL   "Disable LibEvent tests"            FORCE)
set(EVENT__DISABLE_REGRESS   YES CACHE BOOL   "Disable LibEvent regression tests" FORCE)
set(EVENT__DISABLE_SAMPLES   YES CACHE BOOL   "Disable LibEvent samples"          FORCE)
set(EVENT__LIBRARY_TYPE "STATIC" CACHE STRING "Use static LibEvent libraries"     FORCE)

# Set options for JsonCPP, disabling all of their tests:
set(JSONCPP_WITH_TESTS OFF CACHE BOOL "Compile and (for jsoncpp_check) run JsonCpp test executables")
set(JSONCPP_WITH_POST_BUILD_UNITTEST OFF CACHE BOOL "Automatically run unit-tests as a post build step")
set(JSONCPP_WITH_PKGCONFIG_SUPPORT OFF CACHE BOOL "Generate and install .pc files")

# Set options for mbedtls:
set(ENABLE_PROGRAMS OFF CACHE BOOL "Build mbed TLS programs.")
set(ENABLE_TESTING OFF CACHE BOOL "Build mbed TLS tests.")

# Check that the libraries are present:
if (NOT EXISTS ${PROJECT_SOURCE_DIR}/lib/SQLiteCpp/CMakeLists.txt)
	message(FATAL_ERROR "SQLiteCpp is missing in folder lib/SQLiteCpp. Have you initialized the submodules / downloaded the extra libraries?")
endif()
if (NOT EXISTS ${PROJECT_SOURCE_DIR}/lib/mbedtls/CMakeLists.txt)
	message(FATAL_ERROR "mbedTLS is missing in folder lib/mbedtls. Have you initialized the submodules / downloaded the extra libraries?")
endif()
if (NOT EXISTS ${PROJECT_SOURCE_DIR}/lib/libevent/CMakeLists.txt)
	message(FATAL_ERROR "LibEvent is missing in folder lib/libevent. Have you initialized and updated the submodules / downloaded the extra libraries?")
endif()
if (NOT EXISTS ${PROJECT_SOURCE_DIR}/lib/jsoncpp/CMakeLists.txt)
	message(FATAL_ERROR "JsonCPP is missing in folder lib/jsoncpp. Have you initialized and updated the submodules / downloaded the extra libraries?")
endif()
if (NOT EXISTS ${PROJECT_SOURCE_DIR}/lib/cmake-coverage/CodeCoverage.cmake)
	message(FATAL_ERROR "cmake-coverage is missing in folder lib/cmake-coverage. Have you initialized and updated the submodules / downloaded the extra libraries?")
endif()
if (NOT EXISTS ${PROJECT_SOURCE_DIR}/lib/expat/CMakeLists.txt)
	message(FATAL_ERROR "expat is missing in folder lib/expat. Have you initialized and updated the submodules / downloaded the extra libraries?")
endif()
if (NOT EXISTS ${PROJECT_SOURCE_DIR}/lib/fmt/CMakeLists.txt)
	message(FATAL_ERROR  "fmt is missing in folder lib/fmt. Have you initialized and updated the submodules / downloaded the extra libraries?")
endif()
if (NOT EXISTS ${PROJECT_SOURCE_DIR}/lib/lua/CMakeLists.txt)
	message(FATAL_ERROR "lua is missing in folder lib/lua. Have you initialized and updated the submodules / downloaded the extra libraries?")
endif()
if (NOT EXISTS ${PROJECT_SOURCE_DIR}/lib/luaexpat/CMakeLists.txt)
	message(FATAL_ERROR "luaexpat is missing in folder lib/luaexpat. Have you initialized and updated the submodules / downloaded the extra libraries?")
endif()
if (NOT EXISTS ${PROJECT_SOURCE_DIR}/lib/luaproxy/CMakeLists.txt)
	message(FATAL_ERROR "luaproxy is missing in folder lib/luaproxy. Have you initialized and updated the submodules / downloaded the extra libraries?")
endif()
if (NOT EXISTS ${PROJECT_SOURCE_DIR}/lib/sqlite/CMakeLists.txt)
	message(FATAL_ERROR "sqlite is missing in folder lib/sqlite. Have you initialized and updated the submodules / downloaded the extra libraries?")
endif()
if (NOT EXISTS ${PROJECT_SOURCE_DIR}/lib/tolua++/CMakeLists.txt)
	message(FATAL_ERROR "tolua++ is missing in folder lib/tolua++. Have you initialized and updated the submodules / downloaded the extra libraries?")
endif()
if (NOT EXISTS ${PROJECT_SOURCE_DIR}/lib/zlib/CMakeLists.txt)
	message(FATAL_ERROR "zlib is missing in folder lib/zlib. Have you initialized and updated the submodules / downloaded the extra libraries?")
endif()

# Include all the libraries
# We use EXCLUDE_FROM_ALL so that only the explicit dependencies are compiled
# (mbedTLS also has test and example programs in their CMakeLists.txt, we don't want those):
add_subdirectory(lib/expat)
add_subdirectory(lib/fmt)
add_subdirectory(lib/jsoncpp EXCLUDE_FROM_ALL)
add_subdirectory(lib/libevent EXCLUDE_FROM_ALL)
add_subdirectory(lib/sqlite)
add_subdirectory(lib/lua)
add_subdirectory(lib/luaexpat)
add_subdirectory(lib/mbedtls)
add_subdirectory(lib/SQLiteCpp)
add_subdirectory(lib/tolua++ EXCLUDE_FROM_ALL)
add_subdirectory(lib/zlib)

set_exe_flags()
add_executable(${CMAKE_PROJECT_NAME})
add_subdirectory(src)

# Set the startup project to Cuberite, and the debugger dir:
set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT ${CMAKE_PROJECT_NAME})
set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/Server")

# Enable PCH and jumbo builds on supporting CMake:
if (CMAKE_VERSION VERSION_GREATER_EQUAL "3.16")
	if (PRECOMPILE_HEADERS)
		target_precompile_headers(${CMAKE_PROJECT_NAME} PRIVATE src/Globals.h)
	endif()
	
	if (UNITY_BUILDS)
		set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES UNITY_BUILD ON)
	endif()
else()
	message(WARNING "Precompiled headers for FASTER BUILDS not enabled, upgrade to CMake 1.16 or newer!")
endif()

# Add required includes:
target_include_directories(
	${CMAKE_PROJECT_NAME} SYSTEM PRIVATE
	lib/mbedtls/include
	lib/TCLAP/include
	lib # TODO fix files including zlib/x instead of x
)

# Link dependencies as private:
target_link_libraries(
	${CMAKE_PROJECT_NAME} PRIVATE
	event_core
	event_extra
	fmt::fmt
	jsoncpp_lib
	lsqlite
	lua
	luaexpat
	mbedtls
	SQLiteCpp
	tolualib
	zlib
)

# Link process information library:
if (WIN32)
	target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE Psapi.lib)
endif()

# Special case handling for libevent pthreads:
if(NOT WIN32)
	target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE event_pthreads_static)
endif()

# Prettify jsoncpp_lib name in VS solution explorer:
set_property(TARGET jsoncpp_lib PROPERTY PROJECT_LABEL "jsoncpp")

if (WIN32)
	add_subdirectory(lib/luaproxy)
endif()

# Self Test Mode enables extra checks at startup
if(${SELF_TEST})
	message("Tests enabled")
	enable_testing()
	target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE SELF_TEST)
	add_subdirectory(tests)
endif()

include("CMake/Fixups.cmake")
include("CMake/GenerateBindings.cmake")
include("CMake/GroupSources.cmake")
# TODO: include("CMake/SetCompilerFlags.cmake")
