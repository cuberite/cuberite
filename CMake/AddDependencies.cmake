function(build_dependencies)
	# Set options for SQLiteCpp, disable all their lints and other features we don't need:
	set(SQLITE_ENABLE_COLUMN_METADATA  OFF CACHE BOOL "Enable Column::getColumnOriginName(). Require support from sqlite3 library.")
	set(SQLITE_ENABLE_JSON1            OFF CACHE BOOL "Enable JSON1 extension when building internal sqlite3 library.")
	set(SQLITECPP_INCLUDE_SCRIPT       OFF CACHE BOOL "Include config & script files.")
	set(SQLITECPP_RUN_CPPCHECK         OFF CACHE BOOL "Run cppcheck C++ static analysis tool.")
	set(SQLITECPP_RUN_CPPLINT          OFF CACHE BOOL "Run cpplint.py tool for Google C++ StyleGuide.")
	set(SQLITECPP_USE_STACK_PROTECTION OFF CACHE BOOL "USE Stack Protection hardening.")
	set(SQLITECPP_USE_STATIC_RUNTIME   OFF CACHE BOOL "Use MSVC static runtime (default for internal googletest).")

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
	set(JSONCPP_WITH_CMAKE_PACKAGE OFF CACHE BOOL "Generate and install cmake package files")
	set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build jsoncpp_lib as a shared library.")
	set(BUILD_OBJECT_LIBS OFF CACHE BOOL "Build jsoncpp_lib as a object library.")

	# Set options for mbedtls:
	set(ENABLE_PROGRAMS OFF CACHE BOOL "Build mbed TLS programs.")
	set(ENABLE_TESTING OFF CACHE BOOL "Build mbed TLS tests.")

	set(MBEDTLS_AS_SUBPROJECT ON CACHE BOOL "Use mbed TLS as subproject")

	# Enumerate all submodule libraries
	# SQLiteCpp needs to be included before sqlite so the lsqlite target is available:
	set(DEPENDENCIES expat fmt jsoncpp libdeflate libevent lua luaexpat mbedtls SQLiteCpp sqlite tolua++)
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
		jsoncpp_static
		libdeflate
		lsqlite
		lualib
		luaexpat
		mbedtls
		SQLiteCpp
		tolualib
	)

	# Link process information, multimedia (for sleep resolution) libraries:
	if (WIN32)
		target_link_libraries(${TARGET} PRIVATE Psapi.lib Winmm.lib)
	endif()

	# Special case handling for libevent pthreads:
	if(NOT WIN32)
		target_link_libraries(${TARGET} PRIVATE event_pthreads)
	endif()

	# Prettify jsoncpp_static name in VS solution explorer:
	set_property(TARGET jsoncpp_static PROPERTY PROJECT_LABEL "jsoncpp")
	if(${CMAKE_SYSTEM_NAME} MATCHES FreeBSD)
		target_link_libraries(${TARGET} PRIVATE kvm)
	endif()
endfunction()
