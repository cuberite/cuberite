project (Cuberite)

include_directories ("${PROJECT_SOURCE_DIR}/../")

SET (SRCS
	EnchantmentSerializer.cpp
	FastNBT.cpp
	FireworksSerializer.cpp
	MapSerializer.cpp
	NBTChunkSerializer.cpp
	SchematicFileSerializer.cpp
	ScoreboardSerializer.cpp
	StatSerializer.cpp
	WSSAnvil.cpp
	WorldStorage.cpp
)

SET (HDRS
	EnchantmentSerializer.h
	FastNBT.h
	FireworksSerializer.h
	MapSerializer.h
	NBTChunkSerializer.h
	SchematicFileSerializer.h
	ScoreboardSerializer.h
	StatSerializer.h
	WSSAnvil.h
	WorldStorage.h
)

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
	set_source_files_properties(FireworksSerializer.cpp PROPERTIES COMPILE_FLAGS "-Wno-error=switch-enum ")
	set_source_files_properties(NBTChunkSerializer.cpp PROPERTIES COMPILE_FLAGS "-Wno-error=switch-enum ")
	set_source_files_properties(SchematicFileSerializer.cpp PROPERTIES COMPILE_FLAGS "-Wno-error=global-constructors")
	set_source_files_properties(WSSAnvil.cpp PROPERTIES COMPILE_FLAGS "-Wno-error=switch -Wno-error=switch-enum ")
endif()

if(NOT MSVC)
	add_library(WorldStorage ${SRCS} ${HDRS})

	target_link_libraries(WorldStorage OSSupport)
endif()
