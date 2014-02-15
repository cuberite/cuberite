cmake_minimum_required (VERSION 2.6)

# Without this, the MSVC variable isn't defined for MSVC builds ( http://www.cmake.org/pipermail/cmake/2011-November/047130.html )
enable_language(CXX C)

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

# Self Test Mode enables extra checks at startup
if(${SELF_TEST})
	add_definitions(-DSELF_TEST)
endif()





project (MCServer)

# Include all the libraries:
add_subdirectory(lib/inifile/)
add_subdirectory(lib/jsoncpp/)
add_subdirectory(lib/zlib/)
add_subdirectory(lib/lua/)
add_subdirectory(lib/tolua++/)
add_subdirectory(lib/sqlite/)
add_subdirectory(lib/expat/)
add_subdirectory(lib/luaexpat/)
add_subdirectory(lib/md5/)


# We use EXCLUDE_FROM_ALL so that only the explicit dependencies are used
# (PolarSSL also has test and example programs in their CMakeLists.txt, we don't want those)
add_subdirectory(lib/polarssl/ EXCLUDE_FROM_ALL)

set_exe_flags()

if(${BUILD_TOOLS})
add_subdirectory(Tools/GeneratorPerformanceTest/)
endif()

add_subdirectory (src)

