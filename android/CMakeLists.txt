cmake_minimum_required (VERSION 3.12.4)
project(Cuberite)

# Set up Android parameters
set(ANDROID TRUE)
set(CMAKE_POSITION_INDEPENDENT_CODE TRUE)

# We're crosscompiling for Android
set(NO_NATIVE_OPTIMIZATION TRUE)

# Build the rest of the server
add_subdirectory(../ Cuberite)

# Strip debug symbols to reduce binary size
set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES LINK_FLAGS_RELEASE -s)
