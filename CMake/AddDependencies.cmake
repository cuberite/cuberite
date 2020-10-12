function(build_dependencies)
	# Set options for SQLiteCpp, disable all their tests and lints:
	set(SQLITECPP_BUILD_EXAMPLES       OFF CACHE BOOL "Build examples.")
	set(SQLITECPP_BUILD_TESTS          OFF CACHE BOOL "Build and run tests.")
	set(SQLITECPP_INTERNAL_SQLITE      ON  CACHE BOOL "Add the internal SQLite3 source to the project.")
	set(SQLITECPP_RUN_CPPCHECK         OFF CACHE BOOL "Run cppcheck C++ static analysis tool.")
	set(SQLITECPP_RUN_CPPLINT          OFF CACHE BOOL "Run cpplint.py tool for Google C++ StyleGuide.")
	set(SQLITECPP_RUN_DOXYGEN          OFF CACHE BOOL "Run Doxygen C++ documentation tool.")
	set(SQLITECPP_USE_STACK_PROTECTION OFF CACHE BOOL "USE Stack Protection hardening.")
	set(SQLITE_ENABLE_COLUMN_METADATA  OFF CACHE BOOL "Enable Column::getColumnOriginName(). Require support from sqlite3 library.")

	# Set options for LibEvent, disable all their tests and benchmarks:
	set(EVENT__DISABLE_OPENSSL   YES CACHE BOOL   "Disable OpenSSL in LibEvent")
	set(EVENT__DISABLE_BENCHMARK YES CACHE BOOL   "Disable LibEvent benchmarks")
	set(EVENT__DISABLE_TESTS     YES CACHE BOOL   "Disable LibEvent tests")
	set(EVENT__DISABLE_REGRESS   YES CACHE BOOL   "Disable LibEvent regression tests")
	set(EVENT__DISABLE_SAMPLES   YES CACHE BOOL   "Disable LibEvent samples")
	set(EVENT__LIBRARY_TYPE "STATIC" CACHE STRING "Use static LibEvent libraries")

	# Set options for JsonCPP, disabling all of their tests:
	set(JSONCPP_WITH_TESTS OFF CACHE BOOL "Compile and (for jsoncpp_check) run JsonCpp test executables")
	set(JSONCPP_WITH_POST_BUILD_UNITTEST OFF CACHE BOOL "Automatically run unit-tests as a post build step")
	set(JSONCPP_WITH_PKGCONFIG_SUPPORT OFF CACHE BOOL "Generate and install .pc files")

	# Set options for mbedtls:
	set(ENABLE_PROGRAMS OFF CACHE BOOL "Build mbed TLS programs.")
	set(ENABLE_TESTING OFF CACHE BOOL "Build mbed TLS tests.")

	# Enumerate all submodule libraries
	# SQLiteCpp needs to be included before sqlite so the lsqlite target is available:
	set(DEPENDENCIES expat fmt jsoncpp libevent lua luaexpat mbedtls SQLiteCpp sqlite tolua++ zlib)
	foreach(DEPENDENCY ${DEPENDENCIES})
		# Check that the libraries are present:
		if (NOT EXISTS "${PROJECT_SOURCE_DIR}/lib/${DEPENDENCY}/CMakeLists.txt")
			message(FATAL_ERROR "${DEPENDENCY} is missing in folder lib/${DEPENDENCY}. Have you initialized and updated the submodules / downloaded the extra libraries?")
		endif()

		# Include all the libraries
		# We use EXCLUDE_FROM_ALL so that only the explicit dependencies are compiled
		# (mbedTLS also has test and example programs in their CMakeLists.txt, we don't want those):
		add_subdirectory("lib/${DEPENDENCY}" EXCLUDE_FROM_ALL)
	endforeach()

	if (WIN32)
		add_subdirectory(lib/luaproxy)
	endif()
endfunction()

function(link_dependencies TARGET)
	# Add required includes:
	target_include_directories(
		${TARGET} SYSTEM PRIVATE
		lib/mbedtls/include
		lib/TCLAP/include
		lib # TODO fix files including zlib/x instead of x
	)

	# Link dependencies as private:
	target_link_libraries(
		${TARGET} PRIVATE
		event_core
		event_extra
		fmt::fmt
		jsoncpp_lib
		lsqlite
		lualib
		luaexpat
		mbedtls
		SQLiteCpp
		tolualib
		zlib
	)

	# Link process information library:
	if (WIN32)
		target_link_libraries(${TARGET} PRIVATE Psapi.lib)
	endif()

	# Special case handling for libevent pthreads:
	if(NOT WIN32)
		target_link_libraries(${TARGET} PRIVATE event_pthreads)
	endif()

	# Prettify jsoncpp_lib name in VS solution explorer:
	set_property(TARGET jsoncpp_lib PROPERTY PROJECT_LABEL "jsoncpp")
endfunction()
