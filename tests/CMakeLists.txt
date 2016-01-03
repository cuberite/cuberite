cmake_minimum_required (VERSION 2.6)

enable_testing()

if (CMAKE_BUILD_TYPE STREQUAL "COVERAGE")
	setup_target_for_coverage("${PROJECT_NAME}_coverage" "ctest" coverage)
endif()

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(ChunkData)
add_subdirectory(HTTP)
add_subdirectory(Network)
add_subdirectory(LoadablePieces)
