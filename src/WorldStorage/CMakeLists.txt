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

if(NOT MSVC)
	add_library(WorldStorage ${SRCS} ${HDRS})
	target_link_libraries(WorldStorage fmt::fmt OSSupport)
endif()
