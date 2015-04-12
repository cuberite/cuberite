cmake_minimum_required (VERSION 2.8.7)
project(Cuberite)
include(ExternalProject)

externalproject_add(lua_native
	SOURCE_DIR ../lib/lua
	CMAKE_GENERATOR "Visual Studio 14 2015"
	CMAKE_ARGS -DANDROID_CROSSCOMPILE=TRUE
	BUILD_COMMAND
		${CMAKE_COMMAND} --build <BINARY_DIR> --config Release
	INSTALL_COMMAND ""
)
externalproject_add(tolua_native
	SOURCE_DIR ../lib/tolua++
	CMAKE_GENERATOR "Visual Studio 14 2015"
	INSTALL_DIR ${CMAKE_SOURCE_DIR}/../src/Bindings
	BUILD_COMMAND
		${CMAKE_COMMAND} --build <BINARY_DIR> --config Release
	INSTALL_COMMAND
		${CMAKE_COMMAND} -E copy "${CMAKE_SOURCE_DIR}/tolua_native-prefix/src/tolua_native-build/Release/tolua.exe" <INSTALL_DIR>
)

# Tell libevent that Android has pthread (not auto detected due to NDK implementation problems)
# Tell Lua to not use unimplemented Android locale features
add_definitions(-Dgetlocaledecpoint\(\)="'.'")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
set(CROSSCOMPILE 1)

add_library(AndroidNative jni/app-android.cpp)
add_dependencies(tolua_native lua_native)
add_dependencies(AndroidNative tolua_native)
target_link_libraries(AndroidNative log dl)

include_directories(
	../lib/
	../src/
	../lib/jsoncpp/include/
	../lib/polarssl/include/
	../lib/sqlitecpp/include/
	../lib/sqlitecpp/sqlite3/
	../lib/libevent/include/
)

# Probably not the recommended method, but other ways don't seem to generate libraries in libs/armeabi
set(CMAKE_SOURCE_DIR "${CMAKE_SOURCE_DIR}/../")
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../ ${CMAKE_CURRENT_BINARY_DIR}/gen)


