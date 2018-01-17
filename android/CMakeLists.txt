cmake_minimum_required (VERSION 3.7)
project(Cuberite)

# Set up Android parameters
add_definitions(-DANDROID)
set(ANDROID TRUE)
set(CMAKE_POSITION_INDEPENDENT_CODE TRUE)
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fPIE -pie")

# We're crosscompiling for Android
set(NO_NATIVE_OPTIMIZATION TRUE)

# SYSTEM flag to silence warnings for external headers
include_directories(SYSTEM
	../lib/
	../src/
	../lib/jsoncpp/include/
	../lib/polarssl/include/
	../lib/sqlitecpp/include/
	../lib/sqlitecpp/sqlite3/
	../lib/libevent/include/
)

# Disable some compiler warnings (the lazy way out)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-double-promotion")

# Build the rest of the server
add_subdirectory(../ Cuberite)
