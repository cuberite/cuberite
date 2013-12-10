
cmake_minimum_required (VERSION 2.6)
project (cryptopp)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DCRYPTOPP_DISABLE_ASM")
endif()
include_directories ("${PROJECT_SOURCE_DIR}/../../src/")

file(GLOB cryptopp_SRC
    "*.cpp"
)

add_library(cryptopp ${cryptopp_SRC})
