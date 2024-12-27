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

		# Turn off CRT warnings:
		add_compile_definitions(_CRT_SECURE_NO_WARNINGS)

		return ()
	endif()

	# Allow for a forced 32-bit build under 64-bit OS:
	if (FORCE_32)
		add_compile_options(-m32)
		add_link_options(-m32)
	endif()

	# https://en.wikipedia.org/wiki/Uname
	# https://gcc.gnu.org/onlinedocs/gcc/index.html
	# Have the compiler generate code specifically targeted at the current machine on Linux:
	if(NOT NO_NATIVE_OPTIMIZATION AND NOT CMAKE_CROSSCOMPILING)
		string(TOLOWER ${CMAKE_SYSTEM_PROCESSOR} SYSTEM_PROCESSOR)
		if (SYSTEM_PROCESSOR MATCHES "^(i386|i686|x86|amd64|mips)")
			message(STATUS "Optimising for this machine (march=native)")
			add_compile_options(-march=native)
		elseif (SYSTEM_PROCESSOR MATCHES "^(arm|aarch|powerpc|ppc|sparc|alpha)")
			message(STATUS "Optimising for this machine (mcpu=native)")
			add_compile_options(-mcpu=native)
		endif()
	endif()
endfunction()

function(set_exe_flags TARGET)
	if (MSVC)
		# TODO: Warnings as errors
		target_compile_options(
			${TARGET} PRIVATE

			# Warnings level 4:
			/W4

			# Excessive amount of logspam (Unreferenced formal parameter), disable for now:
			/wd4100
		)
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

			# TODO: actually fix the warnings instead of disabling them
			# or at least disable on a file-level basis:
			-Wno-padded
			-Wno-double-promotion

			# This is a pretty useless warning, we've already got -Wswitch which is what we need:
			-Wno-switch-enum

			# Weverything with Clang exceptions:
			-Weverything -Wno-exit-time-destructors -Wno-error=disabled-macro-expansion -Wno-weak-vtables
			-Wno-string-conversion -Wno-c++98-compat-pedantic -Wno-c++2a-compat-pedantic -Wno-documentation
			-Wno-documentation-unknown-command -Wno-reserved-id-macro -Wno-error=unused-command-line-argument
		)

		# Werror only for debug builds
		if(CMAKE_BUILD_TYPE STREQUAL "Debug")
			target_compile_options(
				${TARGET} PRIVATE

				# Warnings-as-errors only on Clang for now:
				-Werror
			)
		endif()

		if(CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL 7 AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS 13)
			target_compile_options(
				${TARGET} PRIVATE

				# We aren't using C++11:
				-Wno-return-std-move-in-c++11
			)
		endif()

		if(CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL 10)
			target_compile_options(
				${TARGET} PRIVATE

				# int to float conversions happen a lot, not worth fixing all warnings:
				-Wno-implicit-int-float-conversion
			)
		endif()

		if(CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL 13)
			target_compile_options(
				${TARGET} PRIVATE

				# TODO: fix
				-Wno-reserved-identifier
			)
		endif()
		if(CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL 16)
			target_compile_options(
				${TARGET} PRIVATE
				-Wno-unsafe-buffer-usage
			)
		endif()
	endif()
endfunction()
