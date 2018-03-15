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





cmake_minimum_required (VERSION 3.5)

# Without this, the MSVC variable isn't defined for MSVC builds ( https://cmake.org/pipermail/cmake/2011-November/047130.html )
enable_language(CXX C)

# Enable the support for solution folders in MSVC
if (MSVC)
	set_property(GLOBAL PROPERTY USE_FOLDERS ON)
endif()

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

# We need C++11 features, Visual Studio has those from VS2012, but it needs a new platform toolset for those; VS2013 supports them natively:
# Adapted from https://web.archive.org/web/https://binglongx.wordpress.com/2013/06/28/set-non-default-platform-toolset-in-cmake/
if(MSVC OR MSVC_IDE)
	if( MSVC_VERSION LESS 1700 )       # VC10- / VS2010-
		message(FATAL_ERROR "The project requires C++11 features. "
			"You need at least Visual Studio 11 (Microsoft Visual Studio 2012), "
			"with Microsoft Visual C++ Compiler Nov 2012 CTP (v120_CTP_Nov2012).")
	elseif( MSVC_VERSION EQUAL 1700 )  # VC11 / VS2012
		message( "VC11: using Microsoft Visual Studio 2012 "
			"with Microsoft Visual C++ Compiler Nov 2012 CTP (v120_CTP_Nov2012)" )
		set(CMAKE_GENERATOR_TOOLSET "v120_CTP_Nov2012" CACHE STRING "Platform Toolset" FORCE)
	else() # VC12+, assuming C++11 supported.
	endif()
else()  # GCC or Clang, so get compiler version directly since CMAKE_CXX_COMPILER_VERSION is only available in CMake 2.8.8 and later
	execute_process(COMMAND ${CMAKE_CXX_COMPILER} -dumpversion OUTPUT_VARIABLE DUMPED_COMPILER_VERSION OUTPUT_STRIP_TRAILING_WHITESPACE)

	# Check for gcc version 4.8 or greater
	if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" AND DUMPED_COMPILER_VERSION VERSION_LESS "4.8")
		message(FATAL_ERROR "You have ${CMAKE_CXX_COMPILER_ID} version ${DUMPED_COMPILER_VERSION}, but at least 4.8 is needed")
	endif()

	# Check for clang version 3.4 or greater
	if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" AND DUMPED_COMPILER_VERSION VERSION_LESS "3.4")
		message(FATAL_ERROR "You have ${CMAKE_CXX_COMPILER_ID} version ${DUMPED_COMPILER_VERSION}, but at least 3.4 is needed")
	endif()
endif()

set(BUILD_TOOLS OFF CACHE BOOL "")
set(SELF_TEST OFF CACHE BOOL "")

# Check whether Lua can be used:
if (NOT(DISABLE_SYSTEM_LUA))
	include(CheckLua.cmake)
	if(HAS_LUA_INTERPRETER)
		message(STATUS "Lua has been found in your system and will be used for the build.")
		set(USE_SYSTEM_LUA 1)
	else()
		message(STATUS "Lua has NOT been found in your system, the build will use its own Lua implementation.")
		unset(USE_SYSTEM_LUA)
	endif()
else()
	message(STATUS "System Lua is disabled via CMake command-line parameters. The build will use its own Lua implementation.")
endif()

include(SetFlags.cmake)
set_flags()
set_lib_flags()
enable_profile()

# Under Windows, we need Lua as DLL; on *nix we need it linked statically:
if (WIN32)
	add_definitions(-DLUA_BUILD_AS_DLL)
endif()

# The Expat library is linked in statically, make the source files aware of that:
add_definitions(-DXML_STATIC)

# Let Lua use additional checks on its C API. This is only compiled into Debug builds:
add_definitions(-DLUA_USE_APICHECK)

# Self Test Mode enables extra checks at startup
if(${SELF_TEST})
	add_definitions(-DSELF_TEST)
endif()

# Build all dependent libraries as static:
SET(CMAKE_BUILD_STATIC_LIBRARIES ON)

####



project (Cuberite)

add_subdirectory(lib)
link_directories(lib/jsoncpp/src/lib_json)

# Add proper includes for LibEvent's event-config.h header:
include_directories(SYSTEM ${LIBEVENT_INCLUDE_DIRS})

if (WIN32)
	add_subdirectory(lib/luaproxy/)
endif()

# We use EXCLUDE_FROM_ALL so that only the explicit dependencies are used
# (mbedTLS also has test and example programs in their CMakeLists.txt, we don't want those)
include(lib/mbedtls.cmake EXCLUDE_FROM_ALL)

if(NOT MSVC AND "${CMAKE_SYSTEM_PROCESSOR}" MATCHES "arm")
	# mbed TLS uses the frame pointer's register in inline assembly for its bignum implementation:
	# https://tls.mbed.org/kb/development/arm-thumb-error-r7-cannot-be-used-in-asm-here
	target_compile_options(mbedcrypto PRIVATE -fomit-frame-pointer)
endif()

set_exe_flags()

add_subdirectory(src)

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

if(${SELF_TEST})
	message("Tests enabled")
	enable_testing()
	add_subdirectory(tests)
endif()

# Put projects into solution folders in MSVC:
if (MSVC)
	set_target_properties(
		event_core
		event_extra
		expat
		fmt
		jsoncpp_lib_static
		lua
		luaexpat
		mbedcrypto
		mbedtls
		mbedx509
		sqlite
		SQLiteCpp
		tolualib
		zlib
		PROPERTIES FOLDER Lib
	)
	set_target_properties(
		luaproxy
		PROPERTIES FOLDER Support
	)

	if(${BUILD_TOOLS})
		set_target_properties(
			MCADefrag
			ProtoProxy
			PROPERTIES FOLDER Tools
		)
	endif()
endif()
