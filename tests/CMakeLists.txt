if (CMAKE_BUILD_TYPE STREQUAL "Coverage")
	message(STATUS "Building with coverage")
	SET(CMAKE_CXX_FLAGS "-g -O0 -fprofile-arcs -ftest-coverage")
	SET(CMAKE_C_FLAGS "-g -O0 -fprofile-arcs -ftest-coverage")
	include(../lib/cmake-coverage/CodeCoverage.cmake)
	setup_target_for_coverage("${PROJECT_NAME}_coverage" "ctest" coverage)
endif()

cmake_minimum_required(VERSION 3.11)

include(FetchContent)
FetchContent_Declare(
		googletest
		GIT_REPOSITORY https://github.com/google/googletest.git
		GIT_TAG        main
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_GetProperties(googletest)

if(NOT googletest_POPULATED)
	message(STATUS "Downloading googletest")
	FetchContent_Populate(googletest)
endif()

add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR})
find_package(googletest)
include(googletest)

add_compile_definitions(TEST_GLOBALS)

set_property(GLOBAL PROPERTY TEST_TARGETS "")

function(add_test_executable name)
	set_property(GLOBAL APPEND PROPERTY TEST_TARGETS ${name})
	add_test(NAME ${name} COMMAND ${name})
endfunction()

message(STATUS "Test Setup finished, collecting Tests")

add_subdirectory(BlockTypeRegistry)
message(STATUS "Test targets: ${TEST_TARGETS}")
add_subdirectory(BoundingBox)
add_subdirectory(ByteBuffer)
add_subdirectory(ChunkData)
add_subdirectory(CompositeChat)
add_subdirectory(FastRandom)
add_subdirectory(Generating)
add_subdirectory(HTTP)
add_subdirectory(LuaThreadStress)
add_subdirectory(Network)
add_subdirectory(OSSupport)
add_subdirectory(SchematicFileSerializer)
add_subdirectory(UUID)

get_property(TEST_TARGETS GLOBAL PROPERTY TEST_TARGETS)

add_custom_target(
	ALL_TESTS
	DEPENDS
	${TEST_TARGETS}
)
