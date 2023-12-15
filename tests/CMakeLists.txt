if (CMAKE_BUILD_TYPE STREQUAL "COVERAGE")
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
	FetchContent_Populate(googletest)
	add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR})
endif()

find_package(GoogleTest)
include(GoogleTest)

add_compile_definitions(TEST_GLOBALS)

add_subdirectory(BlockTypeRegistry)
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
