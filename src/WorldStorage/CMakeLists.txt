
cmake_minimum_required (VERSION 2.6)
project (MCServer)

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
	set_source_files_properties(FastNBT.cpp PROPERTIES COMPILE_FLAGS "-Wno-error=sign-conversion ")
	set_source_files_properties(FireworksSerializer.cpp PROPERTIES COMPILE_FLAGS "-Wno-error=switch-enum ")
	set_source_files_properties(NBTChunkSerializer.cpp PROPERTIES COMPILE_FLAGS "-Wno-error=switch-enum -Wno-error=sign-conversion -Wno-error=conversion ")
	set_source_files_properties(SchematicFileSerializer.cpp PROPERTIES COMPILE_FLAGS "-Wno-error=shadow -Wno-error=conversion  -Wno-error=global-constructors")
	set_source_files_properties(WSSAnvil.cpp PROPERTIES COMPILE_FLAGS "-Wno-error=sign-conversion -Wno-error=shorten-64-to-32 -Wno-error=switch -Wno-error=switch-enum -Wno-error=conversion ")
endif()

if(NOT MSVC)
	add_library(WorldStorage ${SRCS} ${HDRS})

	target_link_libraries(WorldStorage OSSupport)
endif()
