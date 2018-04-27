project (Cuberite)

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

add_library(WorldStorage ${SRCS} ${HDRS})
target_include_directories(WorldStorage PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}" PRIVATE ..)
target_link_libraries(WorldStorage PRIVATE fmt::fmt OSSupport jsoncpp_lib_static mbedTLS++ SQLiteCpp zlib)
