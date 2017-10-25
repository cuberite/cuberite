

macro (add_flags_lnk FLAGS)
	set(CMAKE_EXE_LINKER_FLAGS             "${CMAKE_EXE_LINKER_FLAGS}             ${FLAGS}")
	set(CMAKE_EXE_LINKER_FLAGS_DEBUG       "${CMAKE_EXE_LINKER_FLAGS_DEBUG}       ${FLAGS}")
	set(CMAKE_EXE_LINKER_FLAGS_COVERAGE    "${CMAKE_EXE_LINKER_FLAGS_COVERAGE}    ${FLAGS}")
	set(CMAKE_EXE_LINKER_FLAGS_RELEASE     "${CMAKE_EXE_LINKER_FLAGS_RELEASE}     ${FLAGS}")
	set(CMAKE_SHARED_LINKER_FLAGS          "${CMAKE_SHARED_LINKER_FLAGS}          ${FLAGS}")
	set(CMAKE_SHARED_LINKER_FLAGS_DEBUG    "${CMAKE_SHARED_LINKER_FLAGS_DEBUG}    ${FLAGS}")
	set(CMAKE_SHARED_LINKER_FLAGS_COVERAGE "${CMAKE_SHARED_LINKER_FLAGS_COVERAGE} ${FLAGS}")
	set(CMAKE_SHARED_LINKER_FLAGS_RELEASE  "${CMAKE_SHARED_LINKER_FLAGS_RELEASE}  ${FLAGS}")
	set(CMAKE_MODULE_LINKER_FLAGS          "${CMAKE_MODULE_LINKER_FLAGS}          ${FLAGS}")
	set(CMAKE_MODULE_LINKER_FLAGS_DEBUG    "${CMAKE_MODULE_LINKER_FLAGS_DEBUG}    ${FLAGS}")
	set(CMAKE_MODULE_LINKER_FLAGS_COVERAGE "${CMAKE_MODULE_LINKER_FLAGS_COVERAGE} ${FLAGS}")
	set(CMAKE_MODULE_LINKER_FLAGS_RELEASE  "${CMAKE_MODULE_LINKER_FLAGS_RELEASE}  ${FLAGS}")
endmacro()

macro(add_flags_cxx FLAGS)
	set(CMAKE_CXX_FLAGS          "${CMAKE_CXX_FLAGS} ${FLAGS}")
	set(CMAKE_C_FLAGS            "${CMAKE_C_FLAGS}   ${FLAGS}")
endmacro()


#this is a hack because we can't use cmake 2.8.10 because of travis
macro(get_clang_version)
	execute_process(
		COMMAND "${CMAKE_CXX_COMPILER}" "--version"
		OUTPUT_VARIABLE CLANG_VERSION_OUTPUT)
	string(REGEX MATCH "version ([0-9]\\.[0-9])" x ${CLANG_VERSION_OUTPUT})
	set(CLANG_VERSION ${CMAKE_MATCH_1})
endmacro()


macro(set_flags)
	# Add coverage processing, if requested:
	if (NOT MSVC)

		if (CMAKE_BUILD_TYPE STREQUAL "COVERAGE")
			message("Including CodeCoverage")
			set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/lib/cmake-coverage/")
			include(CodeCoverage)
		endif()
	endif()

	# Add the preprocessor macros used for distinguishing between debug and release builds (CMake does this automatically for MSVC):
	if (NOT MSVC)
		set(CMAKE_CXX_FLAGS_DEBUG    "${CMAKE_CXX_FLAGS_DEBUG}    -D_DEBUG")
		set(CMAKE_C_FLAGS_DEBUG      "${CMAKE_C_FLAGS_DEBUG}      -D_DEBUG")
		set(CMAKE_CXX_FLAGS_COVERAGE "${CMAKE_CXX_FLAGS_COVERAGE} -D_DEBUG")
		set(CMAKE_C_FLAGS_COVERAGE   "${CMAKE_C_FLAGS_COVERAGE}   -D_DEBUG")
		set(CMAKE_CXX_FLAGS_RELEASE  "${CMAKE_CXX_FLAGS_RELEASE}  -DNDEBUG")
		set(CMAKE_C_FLAGS_RELEASE    "${CMAKE_C_FLAGS_RELEASE}    -DNDEBUG")
	endif()

	if(MSVC)
		# Make build use multiple threads under MSVC:
		add_flags_cxx("/MP")

		# Make release builds use link-time code generation:
		set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /GL")
		set(CMAKE_C_FLAGS_RELEASE   "${CMAKE_C_FLAGS_RELEASE}   /GL")
		set(CMAKE_EXE_LINKER_FLAGS_RELEASE    "${CMAKE_EXE_LINKER_FLAGS_RELEASE}    /LTCG")
		set(CMAKE_SHARED_LINKER_FLAGS_RELEASE "${CMAKE_SHARED_LINKER_FLAGS_RELEASE} /LTCG")
		set(CMAKE_MODULE_LINKER_FLAGS_RELEASE "${CMAKE_MODULE_LINKER_FLAGS_RELEASE} /LTCG")

		# Make build use Unicode:
		add_definitions(-DUNICODE -D_UNICODE)
	elseif(APPLE)

		if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
			execute_process(
				COMMAND ${CMAKE_C_COMPILER} -dumpversion
				OUTPUT_VARIABLE GCC_VERSION
			)
		endif()

		set(CMAKE_CXX_FLAGS          "${CMAKE_CXX_FLAGS}          -std=c++11")
		set(CMAKE_CXX_FLAGS_DEBUG    "${CMAKE_CXX_FLAGS_DEBUG}    -std=c++11")
		set(CMAKE_CXX_FLAGS_COVERAGE "${CMAKE_CXX_FLAGS_COVERAGE} -std=c++11")
		set(CMAKE_CXX_FLAGS_RELEASE  "${CMAKE_CXX_FLAGS_RELEASE}  -std=c++11")

		#on os x clang adds pthread for us but we need to add it for gcc
		if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
			add_flags_cxx("-stdlib=libc++")
			add_flags_lnk("-stdlib=libc++")
		else()
			add_flags_cxx("-pthread")
		endif()
	elseif (ANDROID)
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
		set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -std=c++11")
		set(CMAKE_CXX_FLAGS_COVERAGE "${CMAKE_CXX_FLAGS_COVERAGE} -std=c++11")
		set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -std=c++11")
		
		add_flags_cxx("-fsigned-char")
	else()
		# Let gcc / clang know that we're compiling a multi-threaded app:
		if (${UNIX})
			add_flags_cxx("-pthread")
			add_flags_lnk("-pthread")
		endif()

		if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
			execute_process(
				COMMAND ${CMAKE_C_COMPILER} -dumpversion
				OUTPUT_VARIABLE GCC_VERSION
			)
		endif()

		set(CMAKE_CXX_FLAGS          "${CMAKE_CXX_FLAGS}          -std=c++11")
		set(CMAKE_CXX_FLAGS_DEBUG    "${CMAKE_CXX_FLAGS_DEBUG}    -std=c++11")
		set(CMAKE_CXX_FLAGS_COVERAGE "${CMAKE_CXX_FLAGS_COVERAGE} -std=c++11")
		set(CMAKE_CXX_FLAGS_RELEASE  "${CMAKE_CXX_FLAGS_RELEASE}  -std=c++11")

		# We use a signed char (fixes #640 on RasPi)
		add_flags_cxx("-fsigned-char")

	endif()


	# Allow for a forced 32-bit build under 64-bit OS:
	if (FORCE_32)
		add_flags_cxx("-m32")
		add_flags_lnk("-m32")
	endif()


	# Have the compiler generate code specifically targeted at the current machine on Linux
	if(LINUX AND NOT NO_NATIVE_OPTIMIZATION)
		add_flags_cxx("-march=native")
	endif()

	if(CROSSCOMPILE)
		message(FATAL_ERROR "The CROSSCOMPILE flag has been renamed to NO_NATIVE_OPTIMIZATION. Please update your build scripts to compile Cuberite.")
	endif()

	if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
		get_clang_version()
	endif()


	# Use static CRT in MSVC builds:
	if (MSVC)
		string(REPLACE "/MD" "/MT" CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}")
		string(REPLACE "/MD" "/MT" CMAKE_C_FLAGS_RELEASE   "${CMAKE_C_FLAGS_RELEASE}")
		string(REPLACE "/MDd" "/MTd" CMAKE_CXX_FLAGS_DEBUG   "${CMAKE_CXX_FLAGS_DEBUG}")
		string(REPLACE "/MDd" "/MTd" CMAKE_C_FLAGS_DEBUG     "${CMAKE_C_FLAGS_DEBUG}")
	endif()
endmacro()

macro(set_lib_flags)
	# Set lower warnings-level for the libraries:
	if (MSVC)
		# Remove /W3 from command line -- cannot just cancel it later with /w like in unix, MSVC produces a D9025 warning (option1 overriden by option2)
		string(REPLACE "/W3" "" CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}")
		string(REPLACE "/W3" "" CMAKE_C_FLAGS_RELEASE   "${CMAKE_C_FLAGS_RELEASE}")
		string(REPLACE "/W3" "" CMAKE_CXX_FLAGS_DEBUG   "${CMAKE_CXX_FLAGS_DEBUG}")
		string(REPLACE "/W3" "" CMAKE_C_FLAGS_DEBUG     "${CMAKE_C_FLAGS_DEBUG}")
	else()
		set(CMAKE_CXX_FLAGS_RELEASE  "${CMAKE_CXX_FLAGS_RELEASE}  -w")
		set(CMAKE_C_FLAGS_RELEASE    "${CMAKE_C_FLAGS_RELEASE}    -w")
		set(CMAKE_CXX_FLAGS_DEBUG    "${CMAKE_CXX_FLAGS_DEBUG}    -w")
		set(CMAKE_C_FLAGS_DEBUG      "${CMAKE_C_FLAGS_DEBUG}      -w")
		set(CMAKE_CXX_FLAGS_COVERAGE "${CMAKE_CXX_FLAGS_COVERAGE} -w")
		set(CMAKE_C_FLAGS_COVERAGE   "${CMAKE_C_FLAGS_COVERAGE}   -w")
	endif()

	# On Unix we use two dynamic loading libraries dl and ltdl.
	# Preference is for dl on unknown systems as it is specified in POSIX
	# the dynamic loader is used by lua and sqllite.
	if (UNIX)
		if(${CMAKE_SYSTEM_NAME} STREQUAL "FreeBSD")
			set(DYNAMIC_LOADER ltdl)
		else()
			set(DYNAMIC_LOADER dl)
		endif()
	endif()
endmacro()

macro(enable_profile)
	# Declare the flags used for profiling builds:
	if (MSVC)
		set (CXX_PROFILING "/Zi")
		set (LNK_PROFILING "/PROFILE /DEBUG")
	else()
		set (CXX_PROFILING "-pg")
		set (LNK_PROFILING "-pg")
	endif()


	# Declare the profiling configurations:
	SET(CMAKE_CXX_FLAGS_DEBUGPROFILE
		"${CMAKE_CXX_FLAGS_DEBUG} ${CXX_PROFILING}"
		CACHE STRING "Flags used by the C++ compiler during profile builds."
		FORCE )
	SET(CMAKE_C_FLAGS_DEBUGPROFILE
		"${CMAKE_C_FLAGS_DEBUG} ${CXX_PROFILING}"
		CACHE STRING "Flags used by the C compiler during profile builds."
		FORCE )
	SET(CMAKE_EXE_LINKER_FLAGS_DEBUGPROFILE
		"${CMAKE_EXE_LINKER_FLAGS_DEBUG} ${LNK_PROFILING}"
		CACHE STRING "Flags used for linking binaries during profile builds."
		FORCE )
	SET(CMAKE_SHARED_LINKER_FLAGS_DEBUGPROFILE
		"${CMAKE_SHARED_LINKER_FLAGS_DEBUG} ${LNK_PROFILING}"
		CACHE STRING "Flags used by the shared libraries linker during profile builds."
		FORCE )
	MARK_AS_ADVANCED(
		CMAKE_CXX_FLAGS_DEBUGPROFILE
		CMAKE_C_FLAGS_DEBUGPROFILE
		CMAKE_EXE_LINKER_FLAGS_DEBUGPROFILE
		CMAKE_SHARED_LINKER_FLAGS_DEBUGPROFILE )

	SET(CMAKE_CXX_FLAGS_RELEASEPROFILE
		"${CMAKE_CXX_FLAGS_RELEASE} ${CXX_PROFILING}"
		CACHE STRING "Flags used by the C++ compiler during profile builds."
		FORCE )
	SET(CMAKE_C_FLAGS_RELEASEPROFILE
		"${CMAKE_C_FLAGS_RELEASE} ${CXX_PROFILING}"
		CACHE STRING "Flags used by the C compiler during profile builds."
		FORCE )
	SET(CMAKE_EXE_LINKER_FLAGS_RELEASEPROFILE
		"${CMAKE_EXE_LINKER_FLAGS_RELEASE} ${LNK_PROFILING}"
		CACHE STRING "Flags used for linking binaries during profile builds."
		FORCE )
	SET(CMAKE_SHARED_LINKER_FLAGS_RELEASEPROFILE
		"${CMAKE_SHARED_LINKER_FLAGS_RELEASE} ${LNK_PROFILING}"
		CACHE STRING "Flags used by the shared libraries linker during profile builds."
		FORCE )
	MARK_AS_ADVANCED(
		CMAKE_CXX_FLAGS_RELEASEPROFILE
		CMAKE_C_FLAGS_RELEASEPROFILE
		CMAKE_EXE_LINKER_FLAGS_RELEASEPROFILE
		CMAKE_SHARED_LINKER_FLAGS_RELEASEPROFILE )
	# The configuration types need to be set after their respective c/cxx/linker flags and before the project directive
	if(MSVC)
		set(CMAKE_CONFIGURATION_TYPES "Debug;Release;DebugProfile;ReleaseProfile" CACHE STRING "" FORCE)
	else()
		set(CMAKE_CONFIGURATION_TYPES "Debug;Release;DebugProfile;ReleaseProfile;Coverage" CACHE STRING "" FORCE)
	endif()
endmacro()

macro(set_exe_flags)
	# Remove disabling the maximum warning level:
	# clang does not like a command line that reads -Wall -Wextra -w -Wall -Wextra and does not output any warnings
	# We do not do that for MSVC since MSVC produces an awful lot of warnings for its own STL headers;
	# the important warnings are turned on using #pragma in Globals.h
	if (NOT MSVC)
		string(REPLACE "-w" "" CMAKE_CXX_FLAGS_RELEASE  "${CMAKE_CXX_FLAGS_RELEASE}")
		string(REPLACE "-w" "" CMAKE_C_FLAGS_RELEASE    "${CMAKE_C_FLAGS_RELEASE}")
		string(REPLACE "-w" "" CMAKE_CXX_FLAGS_DEBUG    "${CMAKE_CXX_FLAGS_DEBUG}")
		string(REPLACE "-w" "" CMAKE_C_FLAGS_DEBUG      "${CMAKE_C_FLAGS_DEBUG}")
		string(REPLACE "-w" "" CMAKE_CXX_FLAGS_COVERAGE "${CMAKE_CXX_FLAGS_COVERAGE}")
		string(REPLACE "-w" "" CMAKE_C_FLAGS_COVERAGE   "${CMAKE_C_FLAGS_COVERAGE}")
		add_flags_cxx("-Wall -Wextra -Wno-unused-parameter -Wno-missing-noreturn")

		# we support non-IEEE 754 fpus so can make no guarentees about error
		add_flags_cxx("-ffast-math")

		if(${CMAKE_SYSTEM_NAME} STREQUAL "FreeBSD")
			# backtrace() and friends are in libexecinfo
			add_flags_lnk("-lexecinfo")
		endif()

		if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
			if ("${CLANG_VERSION}" VERSION_LESS 3.0)
				message(FATAL_ERROR "Cuberite requires clang version 3.0 or higher, your version is ${CLANG_VERSION}")
			endif()
			# clang does not provide the __extern_always_inline macro and a part of libm depends on this when using fast-math
			add_flags_cxx("-D__extern_always_inline=inline")
			add_flags_cxx("-Weverything -Werror -Wno-c++98-compat-pedantic -Wno-string-conversion")
			add_flags_cxx("-Wno-exit-time-destructors -Wno-padded -Wno-weak-vtables")
			add_flags_cxx("-Wno-switch-enum")  # This is a pretty useless warning, we've already got -Wswitch which is what we need
			if ("${CLANG_VERSION}" VERSION_GREATER 3.0)
				# flags that are not present in 3.0
				add_flags_cxx("-Wno-implicit-fallthrough")
			endif()
			if ("${CLANG_VERSION}" VERSION_GREATER 3.1)
				# flags introduced in 3.2
				add_flags_cxx("-Wno-documentation")
			endif()
			if ("${CLANG_VERSION}" VERSION_GREATER 3.5)
				include(CheckCXXCompilerFlag)
				check_cxx_compiler_flag(-Wno-reserved-id-macro HAS_NO_RESERVED_ID_MACRO)
				check_cxx_compiler_flag(-Wno-documentation-unknown-command HAS_NO_DOCUMENTATION_UNKNOWN)
				if (HAS_NO_RESERVED_ID_MACRO)
					# Use this flag to ignore error for a reserved macro problem in sqlite 3
					add_flags_cxx("-Wno-reserved-id-macro")
				endif()
				if (HAS_NO_DOCUMENTATION_UNKNOWN)
					# Ignore another problem in sqlite
					add_flags_cxx("-Wno-documentation-unknown-command")
				endif()
			endif()
			if ("${CLANG_VERSION}" VERSION_GREATER 3.5)
				add_flags_cxx("-Wno-error=disabled-macro-expansion")
			endif()
			if ("${CLANG_VERSION}" VERSION_GREATER 3.7)
				check_cxx_compiler_flag(-Wno-double-promotion HAS_NO_DOUBLE_PROMOTION)
				if (HAS_NO_DOUBLE_PROMOTION)
					add_flags_cxx("-Wno-double-promotion")
				endif()
			endif()
			add_flags_cxx("-Wno-error=unused-command-line-argument")
			add_flags_cxx("-Wno-documentation-unknown-command")
		endif()
	endif()

endmacro()

#	if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
#		foreach(FILENAME ${ARGN})
#			message("downgrade_warnings for ${FILENAME}")
#			set_source_files_properties(${FILENAME} PROPERTIES COMPILE_FLAGS "-Wno-error=sign-conversion -Wno-error=conversion")
#			set_source_files_properties(${FILENAME} PROPERTIES COMPILE_FLAGS "-Wno-error=missing-prototypes -Wno-error=deprecated")
#			set_source_files_properties(${FILENAME} PROPERTIES COMPILE_FLAGS "-Wno-error=shadow -Wno-error=old-style-cast  -Wno-error=switch-enum -Wno-error=switch")
#			set_source_files_properties(${FILENAME} PROPERTIES COMPILE_FLAGS "-Wno-error=float-equal -Wno-error=global-constructors")

#			if ("${CLANG_VERSION}" VERSION_GREATER 3.0)
#				# flags that are not present in 3.0
#				set_source_files_properties(${FILENAME} PROPERTIES COMPILE_FLAGS "-Wno-error=covered-switch-default ")
#				set_source_files_properties(${FILENAME} PROPERTIES COMPILE_FLAGS "-Wno-implicit-fallthrough -Wno-error=extra-semi")
#				set_source_files_properties(${FILENAME} PROPERTIES COMPILE_FLAGS "-Wno-error=missing-variable-declarations")
#			endif()
#		endforeach()
#	endif()
