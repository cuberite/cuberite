cmake_minimum_required (VERSION 2.8.7)

# Without this, the MSVC variable isn't defined for MSVC builds ( http://www.cmake.org/pipermail/cmake/2011-November/047130.html )
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
# Adapted from http://binglongx.wordpress.com/2013/06/28/set-non-default-platform-toolset-in-cmake/
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
endif()

set(BUILD_TOOLS OFF CACHE BOOL "")
set(SELF_TEST OFF CACHE BOOL "")

# This has to be done before any flags have been set up.
if(${BUILD_TOOLS})
	message("Building tools")
	add_subdirectory(Tools/MCADefrag/)
	add_subdirectory(Tools/ProtoProxy/)
endif()

if(${BUILD_UNSTABLE_TOOLS})
	message("Building unstable tools")
	add_subdirectory(Tools/GeneratorPerformanceTest/)
endif()

include(CheckCXXCompilerFlag)
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





project (Cuberite)

# Set options for SQLiteCpp, disable all their tests and lints:
set(SQLITECPP_RUN_CPPLINT     OFF CACHE BOOL "Run cpplint.py tool for Google C++ StyleGuide."  FORCE)
set(SQLITECPP_RUN_CPPCHECK    OFF CACHE BOOL "Run cppcheck C++ static analysis tool."          FORCE)
set(SQLITECPP_RUN_DOXYGEN     OFF CACHE BOOL "Run Doxygen C++ documentation tool."             FORCE)
set(SQLITECPP_BUILD_EXAMPLES  OFF CACHE BOOL "Build examples."                                 FORCE)
set(SQLITECPP_BUILD_TESTS     OFF CACHE BOOL "Build and run tests."                            FORCE)
set(SQLITECPP_INTERNAL_SQLITE OFF CACHE BOOL "Add the internal SQLite3 source to the project." FORCE)
set(SQLITE_ENABLE_COLUMN_METADATA OFF CACHE BOOL "" FORCE)

# Set options for LibEvent, disable all their tests and benchmarks:
set(EVENT__DISABLE_OPENSSL   YES CACHE BOOL "Disable OpenSSL in LibEvent"       FORCE)
set(EVENT__DISABLE_BENCHMARK YES CACHE BOOL "Disable LibEvent benchmarks"       FORCE)
set(EVENT__DISABLE_TESTS     YES CACHE BOOL "Disable LibEvent tests"            FORCE)
set(EVENT__DISABLE_REGRESS   YES CACHE BOOL "Disable LibEvent regression tests" FORCE)
set(EVENT__DISABLE_SAMPLES   YES CACHE BOOL "Disable LibEvent samples"          FORCE)

# Set options for JsonCPP, disabling all of their tests
# Additionally, their library is output to a strange location; make sure the linker knows where to find it
set(JSONCPP_WITH_TESTS OFF CACHE BOOL "Compile and (for jsoncpp_check) run JsonCpp test executables")
set(JSONCPP_WITH_POST_BUILD_UNITTEST OFF CACHE BOOL "Automatically run unit-tests as a post build step")
set(JSONCPP_WITH_PKGCONFIG_SUPPORT OFF CACHE BOOL "Generate and install .pc files")
link_directories(lib/jsoncpp/src/lib_json)

# Check that the libraries are present:
if (NOT EXISTS ${CMAKE_SOURCE_DIR}/lib/SQLiteCpp/CMakeLists.txt)
	message(FATAL_ERROR "SQLiteCpp is missing in folder lib/SQLiteCpp. Have you initialized the submodules / downloaded the extra libraries?")
endif()
if (NOT EXISTS ${CMAKE_SOURCE_DIR}/lib/polarssl/CMakeLists.txt)
	message(FATAL_ERROR "PolarSSL is missing in folder lib/polarssl. Have you initialized the submodules / downloaded the extra libraries?")
endif()
if (NOT EXISTS ${CMAKE_SOURCE_DIR}/lib/libevent/CMakeLists.txt)
	message(FATAL_ERROR "LibEvent is missing in folder lib/libevent. Have you initialized and updated the submodules / downloaded the extra libraries?")
endif()
if (NOT EXISTS ${CMAKE_SOURCE_DIR}/lib/jsoncpp/CMakeLists.txt)
	message(FATAL_ERROR "JsonCPP is missing in folder lib/jsoncpp. Have you initialized and updated the submodules / downloaded the extra libraries?")
endif()
if (NOT EXISTS ${CMAKE_SOURCE_DIR}/lib/cmake-coverage/CodeCoverage.cmake)
	message(FATAL_ERROR "cmake-coverage is missing in folder lib/cmake-coverage. Have you initialized and updated the submodules / downloaded the extra libraries?")
endif()
if (NOT EXISTS ${CMAKE_SOURCE_DIR}/lib/expat/CMakeLists.txt)
	message(FATAL_ERROR "expat is missing in folder lib/expat. Have you initialized and updated the submodules / downloaded the extra libraries?")
endif()
if (NOT EXISTS ${CMAKE_SOURCE_DIR}/lib/lua/CMakeLists.txt)
	message(FATAL_ERROR "lua is missing in folder lib/lua. Have you initialized and updated the submodules / downloaded the extra libraries?")
endif()
if (NOT EXISTS ${CMAKE_SOURCE_DIR}/lib/luaexpat/CMakeLists.txt)
	message(FATAL_ERROR "luaexpat is missing in folder lib/luaexpat. Have you initialized and updated the submodules / downloaded the extra libraries?")
endif()
if (NOT EXISTS ${CMAKE_SOURCE_DIR}/lib/luaproxy/CMakeLists.txt)
	message(FATAL_ERROR "luaproxy is missing in folder lib/luaproxy. Have you initialized and updated the submodules / downloaded the extra libraries?")
endif()
if (NOT EXISTS ${CMAKE_SOURCE_DIR}/lib/sqlite/CMakeLists.txt)
	message(FATAL_ERROR "sqlite is missing in folder lib/sqlite. Have you initialized and updated the submodules / downloaded the extra libraries?")
endif()
if (NOT EXISTS ${CMAKE_SOURCE_DIR}/lib/tolua++/CMakeLists.txt)
	message(FATAL_ERROR "tolua++ is missing in folder lib/tolua++. Have you initialized and updated the submodules / downloaded the extra libraries?")
endif()
if (NOT EXISTS ${CMAKE_SOURCE_DIR}/lib/zlib/CMakeLists.txt)
	message(FATAL_ERROR "zlib is missing in folder lib/zlib. Have you initialized and updated the submodules / downloaded the extra libraries?")
endif()




# Include all the libraries:
add_subdirectory(lib/jsoncpp/)
add_subdirectory(lib/zlib/)
add_subdirectory(lib/lua/)
add_subdirectory(lib/tolua++/)
add_subdirectory(lib/sqlite/)
add_subdirectory(lib/SQLiteCpp/)
add_subdirectory(lib/expat/)
add_subdirectory(lib/luaexpat/)
add_subdirectory(lib/libevent/ EXCLUDE_FROM_ALL)

# Add proper include directories so that SQLiteCpp can find SQLite3:
get_property(SQLITECPP_INCLUDES DIRECTORY "lib/SQLiteCpp/" PROPERTY INCLUDE_DIRECTORIES)
set(SQLITECPP_INCLUDES "${SQLITECPP_INCLUDES}" "${CMAKE_CURRENT_SOURCE_DIR}/lib/sqlite/")
set_property(DIRECTORY lib/SQLiteCpp/ PROPERTY INCLUDE_DIRECTORIES "${SQLITECPP_INCLUDES}")
set_property(TARGET SQLiteCpp PROPERTY INCLUDE_DIRECTORIES "${SQLITECPP_INCLUDES}")

# Add proper includes for LibEvent's event-config.h header:
include_directories(SYSTEM ${LIBEVENT_INCLUDE_DIRS})

# Prettify jsoncpp_lib_static name in VS solution explorer
set_property(TARGET jsoncpp_lib_static PROPERTY PROJECT_LABEL "jsoncpp")

if (WIN32)
	add_subdirectory(lib/luaproxy/)
endif()


# We use EXCLUDE_FROM_ALL so that only the explicit dependencies are used
# (PolarSSL also has test and example programs in their CMakeLists.txt, we don't want those)
include(lib/polarssl.cmake EXCLUDE_FROM_ALL)

set_exe_flags()

add_subdirectory (src)

if(${SELF_TEST})
	message("Tests enabled")
	enable_testing()
	add_subdirectory (tests)
endif()

# Put projects into solution folders in MSVC:
if (MSVC)
	set_target_properties(
		event_core
		event_extra
		expat
		jsoncpp_lib_static
		lua
		luaexpat
		mbedtls
		sqlite
		SQLiteCpp
		tolualib
		zlib
		PROPERTIES FOLDER Lib
	)
	set_target_properties(
		luaproxy
		tolua
		PROPERTIES FOLDER Support
	)
	if (${SELF_TEST})
		set_target_properties(
			Network
			HTTP
			PROPERTIES FOLDER Lib
		)
		set_target_properties(
			arraystocoords-exe
			ChunkBuffer
			coordinates-exe
			copies-exe
			copyblocks-exe
			creatable-exe
			EchoServer
			Google-exe
			HTTPResponseParser_file-exe
			LoadablePieces
			NameLookup
			PROPERTIES FOLDER Tests
		)
	endif()

	if(${BUILD_TOOLS})
		set_target_properties(
			MCADefrag
			ProtoProxy
			PROPERTIES FOLDER Tools
		)
	endif()
endif()
