macro (add_flags_lnk FLAGS)
	set(CMAKE_EXE_LINKER_FLAGS            "${CMAKE_EXE_LINKER_FLAGS}            ${FLAGS}")
	set(CMAKE_EXE_LINKER_FLAGS_DEBUG      "${CMAKE_EXE_LINKER_FLAGS_DEBUG}      ${FLAGS}")
	set(CMAKE_EXE_LINKER_FLAGS_RELEASE    "${CMAKE_EXE_LINKER_FLAGS_RELEASE}    ${FLAGS}")
	set(CMAKE_SHARED_LINKER_FLAGS         "${CMAKE_SHARED_LINKER_FLAGS}         ${FLAGS}")
	set(CMAKE_SHARED_LINKER_FLAGS_DEBUG   "${CMAKE_SHARED_LINKER_FLAGS_DEBUG}   ${FLAGS}")
	set(CMAKE_SHARED_LINKER_FLAGS_RELEASE "${CMAKE_SHARED_LINKER_FLAGS_RELEASE} ${FLAGS}")
	set(CMAKE_MODULE_LINKER_FLAGS         "${CMAKE_MODULE_LINKER_FLAGS}         ${FLAGS}")
	set(CMAKE_MODULE_LINKER_FLAGS_DEBUG   "${CMAKE_MODULE_LINKER_FLAGS_DEBUG}   ${FLAGS}")
	set(CMAKE_MODULE_LINKER_FLAGS_RELEASE "${CMAKE_MODULE_LINKER_FLAGS_RELEASE} ${FLAGS}")
endmacro()

macro(add_flags_cxx FLAGS)
	set(CMAKE_CXX_FLAGS         "${CMAKE_CXX_FLAGS}         ${FLAGS}")
	set(CMAKE_C_FLAGS           "${CMAKE_C_FLAGS}           ${FLAGS}")
	set(CMAKE_CXX_FLAGS_DEBUG   "${CMAKE_CXX_FLAGS_DEBUG}   ${FLAGS}")
	set(CMAKE_C_FLAGS_DEBUG     "${CMAKE_C_FLAGS_DEBUG}     ${FLAGS}")
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${FLAGS}")
	set(CMAKE_C_FLAGS_RELEASE   "${CMAKE_C_FLAGS_RELEASE}   ${FLAGS}")
endmacro()


macro(set_flags)
	# Add the preprocessor macros used for distinguishing between debug and release builds (CMake does this automatically for MSVC):
	if (NOT MSVC)
		set(CMAKE_CXX_FLAGS_DEBUG   "${CMAKE_CXX_FLAGS_DEBUG}   -D_DEBUG")
		set(CMAKE_C_FLAGS_DEBUG     "${CMAKE_C_FLAGS_DEBUG}     -D_DEBUG")
		set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DNDEBUG")
		set(CMAKE_C_FLAGS_RELEASE   "${CMAKE_C_FLAGS_RELEASE}   -DNDEBUG")
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
	elseif(APPLE)
		#on os x clang adds pthread for us but we need to add it for gcc
		if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
			set(CMAKE_CXX_FLAGS         "${CMAKE_CXX_FLAGS}         -std=c++11")
			set(CMAKE_CXX_FLAGS_DEBUG   "${CMAKE_CXX_FLAGS_DEBUG}   -std=c++11")
			set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -std=c++11")
			add_flags_cxx("-stdlib=libc++")
			add_flags_lnk("-stdlib=libc++")
		else()
			add_flags_cxx("-pthread")
		endif()

	else()
		# Let gcc / clang know that we're compiling a multi-threaded app:
		add_flags_cxx("-pthread")
		if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
			set(CMAKE_CXX_FLAGS         "${CMAKE_CXX_FLAGS}         -std=c++11")
			set(CMAKE_CXX_FLAGS_DEBUG   "${CMAKE_CXX_FLAGS_DEBUG}   -std=c++11")
			set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -std=c++11")
		endif()

		# We use a signed char (fixes #640 on RasPi)
		add_flags_cxx("-fsigned-char")
		
	endif()


	# Allow for a forced 32-bit build under 64-bit OS:
	if (FORCE_32)
		add_flags_cxx("-m32")
		add_flags_lnk("-m32")
	endif()


	# Have the compiler generate code specifically targeted at the current machine on Linux
	if(LINUX AND NOT CROSSCOMPILE)
		add_flags_cxx("-march=native")
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
		set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -w")
		set(CMAKE_C_FLAGS_RELEASE   "${CMAKE_C_FLAGS_RELEASE}   -w")
		set(CMAKE_CXX_FLAGS_DEBUG   "${CMAKE_CXX_FLAGS_DEBUG}   -w")
		set(CMAKE_C_FLAGS_DEBUG     "${CMAKE_C_FLAGS_DEBUG}     -w")
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
		set (CXX_PROFILING "")
		set (LNK_PROFILING "/PROFILE")
	else()
		set (CXX_PROFILING "-pg")
		set (LNK_PROFILING "-pg")
	endif()


	# Declare the profiling configurations:
	SET(CMAKE_CXX_FLAGS_DEBUGPROFILE
		"${CMAKE_CXX_FLAGS_DEBUG} ${PCXX_ROFILING}"
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
	set(CMAKE_CONFIGURATION_TYPES "Debug;Release;DebugProfile;ReleaseProfile" CACHE STRING "" FORCE)
endmacro()
    
macro(set_exe_flags)
	# Remove disabling the maximum warning level:
	# clang does not like a command line that reads -Wall -Wextra -w -Wall -Wextra and does not output any warnings
	# We do not do that for MSVC since MSVC produces an awful lot of warnings for its own STL headers;
	# the important warnings are turned on using #pragma in Globals.h
	if (NOT MSVC)
		string(REPLACE "-w" "" CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}")
		string(REPLACE "-w" "" CMAKE_C_FLAGS_RELEASE   "${CMAKE_C_FLAGS_RELEASE}")
		string(REPLACE "-w" "" CMAKE_CXX_FLAGS_DEBUG   "${CMAKE_CXX_FLAGS_DEBUG}")
		string(REPLACE "-w" "" CMAKE_C_FLAGS_DEBUG     "${CMAKE_C_FLAGS_DEBUG}")
		add_flags_cxx("-Wall -Wextra -Wno-unused-parameter -Wno-error=switch")
		
		# we support non-IEEE 754 fpus so can make no guarentees about error
		add_flags_cxx("-ffast-math")
		
		if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
			# clang does not provide the __extern_always_inline macro and a part of libm depends on this when using fast-math
			add_flags_cxx("-D__extern_always_inline=inline")
			add_flags_cxx("-Werror -Weverything -Wno-c++98-compat-pedantic -Wno-string-conversion")
			add_flags_cxx("-Wno-extra-semi -Wno-error=switch-enum -Wno-documentation")
			add_flags_cxx("-Wno-error=sign-conversion -Wno-error=conversion -Wno-padded")
			add_flags_cxx("-Wno-error=deprecated -Wno-error=weak-vtables -Wno-error=float-equal")
			add_flags_cxx("-Wno-error=missing-prototypes -Wno-error=non-virtual-dtor")
			add_flags_cxx("-Wno-error=covered-switch-default -Wno-error=shadow")
			add_flags_cxx("-Wno-error=exit-time-destructors -Wno-error=missing-variable-declarations")
			add_flags_cxx("-Wno-error=global-constructors -Wno-implicit-fallthrough")
			add_flags_cxx("-Wno-weak-vtables -Wno-switch-enum -Wno-exit-time-destructors")
		endif()
	endif()

endmacro()
