enable_testing()

if (CMAKE_BUILD_TYPE STREQUAL "COVERAGE")
	setup_target_for_coverage("${PROJECT_NAME}_coverage" "ctest" coverage)
endif()

# include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(BoundingBox)
add_subdirectory(ByteBuffer)
add_subdirectory(ChunkData)
add_subdirectory(CompositeChat)
add_subdirectory(FastRandom)
add_subdirectory(HTTP)
add_subdirectory(LoadablePieces)
add_subdirectory(Network)
add_subdirectory(OSSupport)
add_subdirectory(SchematicFileSerializer)
