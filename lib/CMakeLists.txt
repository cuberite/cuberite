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

# Set options for mbedtls, disable their programs and tests
set(ENABLE_TESTING OFF CACHE BOOL "Disable tests")
set(ENABLE_PROGRAMS OFF CACHE BOOL "Disable programs")

if (NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/cmake-coverage/CodeCoverage.cmake")
	message(FATAL_ERROR "cmake-coverage is missing in folder lib/cmake-coverage. Have you initialized and updated the submodules / downloaded the extra libraries?")
endif()

# Check and include all the libraries
function(friendly_add_subdirectory dir)
	if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${dir}/CMakeLists.txt")
		message(FATAL_ERROR "${dir} is missing in folder lib/${dir}. Have you initialized the submodules / downloaded the extra libraries?")
	endif()
	add_subdirectory(${ARGV})
endfunction()

friendly_add_subdirectory(jsoncpp)
friendly_add_subdirectory(mbedtls EXCLUDE_FROM_ALL)
friendly_add_subdirectory(zlib)
friendly_add_subdirectory(lua)
friendly_add_subdirectory(tolua++ EXCLUDE_FROM_ALL)
friendly_add_subdirectory(sqlite)
friendly_add_subdirectory(SQLiteCpp)
friendly_add_subdirectory(expat)
friendly_add_subdirectory(luaexpat)
friendly_add_subdirectory(libevent EXCLUDE_FROM_ALL)
friendly_add_subdirectory(fmt)

# Under Windows, we need Lua as DLL; on *nix we need it linked statically:
if (WIN32)
	target_compile_definitions(lua PUBLIC LUA_BUILD_AS_DLL)
endif()

# The Expat library is linked in statically, make the source files aware of that:
target_compile_definitions(expat PUBLIC XML_STATIC)

# Let Lua use additional checks on its C API. This is only compiled into Debug builds:
target_compile_definitions(lua PUBLIC LUA_USE_APICHECK)

# We set the current directory to the tolua++ include directory to allow overwriting
# the header file with Bindings/tolua++.h
target_include_directories(tolualib INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}")

# Set the proper include directories on the targets
target_include_directories(mbedtls SYSTEM PUBLIC mbedtls/include)
target_include_directories(SQLiteCpp PUBLIC SQLiteCpp/include)
target_include_directories(zlib SYSTEM PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
foreach(INCLUDE_DIR ${LIBEVENT_INCLUDE_DIRS})
	target_include_directories(event_core PUBLIC $<BUILD_INTERFACE:${INCLUDE_DIR}>)
endforeach()

# Because we don't use SQLiteCpp's internal sqlite library, we need to add the includes manually
target_include_directories(SQLiteCpp PUBLIC sqlite)

# Prettify jsoncpp_lib_static name in VS solution explorer
set_property(TARGET jsoncpp_lib_static PROPERTY PROJECT_LABEL "jsoncpp")

# Create a TCLAP CMake target
add_library(TCLAP INTERFACE)
target_include_directories(TCLAP SYSTEM INTERFACE TCLAP/include)
