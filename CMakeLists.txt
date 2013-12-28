
cmake_minimum_required (VERSION 2.6)

# Without this, the MSVC variable isn't defined for MSVC builds ( http://www.cmake.org/pipermail/cmake/2011-November/047130.html )
enable_language(CXX C)

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
else()
	# Let gcc / clang know that we're compiling a multi-threaded app:
	add_flags_cxx("-pthread")
endif()

# Allow for a forced 32-bit build under 32-bit OS:
if (FORCE_32)
	add_flags_cxx("-m32")
	add_flags_lnk("-m32")
endif()

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

# Under clang, we need to disable ASM support in CryptoPP:
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
	add_definitions(-DCRYPTOPP_DISABLE_ASM)
endif()

# Under Windows, we need Lua as DLL; on *nix we need it linked statically:
if (WIN32)
	add_definitions(-DLUA_BUILD_AS_DLL)
endif()

# The Expat library is linked in statically, make the source files aware of that:
add_definitions(-DXML_STATIC)


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
    "${CMAKE_CXX_FLAGS_DEBUG} ${CXX_PROFILING}"
    CACHE STRING "Flags used by the C++ compiler during profile builds."
    FORCE )
SET(CMAKE_C_FLAGS_RELEASEPROFILE
    "${CMAKE_C_FLAGS_DEBUG} ${CXX_PROFILING}"
    CACHE STRING "Flags used by the C compiler during profile builds."
    FORCE )
SET(CMAKE_EXE_LINKER_FLAGS_RELEASEPROFILE
    "${CMAKE_EXE_LINKER_FLAGS_DEBUG} ${LNK_PROFILING}"
    CACHE STRING "Flags used for linking binaries during profile builds."
    FORCE )
SET(CMAKE_SHARED_LINKER_FLAGS_RELEASEPROFILE
    "${CMAKE_SHARED_LINKER_FLAGS_DEBUG} ${LNK_PROFILING}"
    CACHE STRING "Flags used by the shared libraries linker during profile builds."
    FORCE )
MARK_AS_ADVANCED(
    CMAKE_CXX_FLAGS_RELEASEPROFILE
    CMAKE_C_FLAGS_RELEASEPROFILE
    CMAKE_EXE_LINKER_FLAGS_RELEASEPROFILE
    CMAKE_SHARED_LINKER_FLAGS_RELEASEPROFILE )


# The configuration types need to be set after their respective c/cxx/linker flags and before the project directive
set(CMAKE_CONFIGURATION_TYPES "Debug;Release;DebugProfile;ReleaseProfile" CACHE STRING "" FORCE)
project (MCServer)


# Include all the libraries:
add_subdirectory(lib/inifile/)
add_subdirectory(lib/jsoncpp/)
add_subdirectory(lib/cryptopp/)
add_subdirectory(lib/zlib/)
add_subdirectory(lib/lua/)
add_subdirectory(lib/tolua++/)
add_subdirectory(lib/sqlite/)
add_subdirectory(lib/expat/)
add_subdirectory(lib/luaexpat/)
add_subdirectory(lib/md5/)

# Re-add the maximum warning level:
# We do not do that for MSVC since MSVC produces an awful lot of warnings for its own STL headers;
# the important warnings will be turned on using #pragma in Globals.h
if (NOT MSVC)
	#TODO: set -Wall -Werror -Wextra
	add_flags("-Wall -Wextra")
endif()

if (NOT WIN32)
	set(CMAKE_EXE_LINKER_FLAGS         "${CMAKE_EXE_LINKER_FLAGS}         -rdynamic")
	set(CMAKE_EXE_LINKER_FLAGS_DEBUG   "${CMAKE_EXE_LINKER_FLAGS_DEBUG}   -rdynamic")
	set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -rdynamic")
	set(CMAKE_EXE_LINKER_FLAGS_PROFILE "${CMAKE_EXE_LINKER_FLAGS_PROFILE} -rdynamic")
endif()


add_subdirectory (src) 

