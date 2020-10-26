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

function(set_global_flags)
	if(MSVC)
		# Make build use multiple threads under MSVC:
		add_compile_options(/MP)

		# Make build use Unicode:
		add_compile_definitions(UNICODE _UNICODE)
	else()
		# TODO: is this needed? NDEBUG is standard. Also, why are we using _DEBUG?
		# Add the preprocessor macros used for distinguishing between debug and release builds (CMake does this automatically for MSVC):
		set(CMAKE_CXX_FLAGS_DEBUG    "${CMAKE_CXX_FLAGS_DEBUG}    -D_DEBUG")
		set(CMAKE_C_FLAGS_DEBUG      "${CMAKE_C_FLAGS_DEBUG}      -D_DEBUG")
		set(CMAKE_CXX_FLAGS_COVERAGE "${CMAKE_CXX_FLAGS_COVERAGE} -D_DEBUG")
		set(CMAKE_C_FLAGS_COVERAGE   "${CMAKE_C_FLAGS_COVERAGE}   -D_DEBUG")
		set(CMAKE_CXX_FLAGS_RELEASE  "${CMAKE_CXX_FLAGS_RELEASE}  -DNDEBUG")
		set(CMAKE_C_FLAGS_RELEASE    "${CMAKE_C_FLAGS_RELEASE}    -DNDEBUG")
	endif()

	# Allow for a forced 32-bit build under 64-bit OS:
	if (FORCE_32)
		add_compile_options(-m32)
		add_link_options(-m32)
	endif()

	# Have the compiler generate code specifically targeted at the current machine on Linux:
	if(LINUX AND NOT NO_NATIVE_OPTIMIZATION)
		add_compile_options(-march=native)
	endif()
endfunction()

function (try_add_flag TARGET FLAG)
	include(CheckCXXCompilerFlag)
	check_cxx_compiler_flag("${FLAG}" "HAS_FLAG_${FLAG}")
	if ("${HAS_FLAG_${FLAG}}")
		target_compile_options(${TARGET} PRIVATE "${FLAG}")
	endif()
endfunction()

function(set_exe_flags TARGET)
	if (MSVC)
		# TODO: MSVC level 4, warnings as errors
		return ()
	endif()

	target_compile_options(
		${TARGET} PRIVATE

		# We use a signed char (fixes #640 on RasPi)
		# TODO: specify this in code, not a compile flag:
		-fsigned-char

		# We support non-IEEE 754 FPUs so can make no guarantees about error:
		-ffast-math

		# All warnings:
		-Wall -Wextra

		# Excessive amount of logspam, disable for now:
		-Wno-unused-parameter
	)

	if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
		target_compile_options(
			${TARGET} PRIVATE

			# Warnings-as-errors only on Clang for now:
			-Werror

			# TODO: actually fix the warnings instead of disabling them
			# or at least disable on a file-level basis:
			-Wno-missing-noreturn -Wno-padded -Wno-implicit-fallthrough
			-Wno-double-promotion

			# This is a pretty useless warning, we've already got -Wswitch which is what we need:
			-Wno-switch-enum

			# Weverything with Clang exceptions:
			-Weverything -Wno-error=disabled-macro-expansion -Wno-weak-vtables
			-Wno-exit-time-destructors -Wno-string-conversion -Wno-c++98-compat-pedantic
			-Wno-documentation -Wno-documentation-unknown-command -Wno-reserved-id-macro
			-Wno-error=unused-command-line-argument
		)

		# We aren't using C++11
		try_add_flag(${TARGET} -Wno-return-std-move-in-c++11)
		# int to float conversions happen a lot, not worth fixing all warnings
		try_add_flag(${TARGET} -Wno-implicit-int-float-conversion)
	endif()
endfunction()
