target_sources(
	${CMAKE_PROJECT_NAME} PRIVATE

	Authenticator.cpp
	ChunkDataSerializer.cpp
	ForgeHandshake.cpp
	MojangAPI.cpp
	Packetizer.cpp
	Protocol_1_8.cpp
	Protocol_1_9.cpp
	Protocol_1_10.cpp
	Protocol_1_11.cpp
	Protocol_1_12.cpp
	Protocol_1_13.cpp
	Protocol_1_14.cpp
	ProtocolRecognizer.cpp
	RecipeMapper.cpp

	Authenticator.h
	ChunkDataSerializer.h
	ForgeHandshake.h
	MojangAPI.h
	Packetizer.h
	Protocol.h
	Protocol_1_8.h
	Protocol_1_9.h
	Protocol_1_10.h
	Protocol_1_11.h
	Protocol_1_12.h
	Protocol_1_13.h
	Protocol_1_14.h
	ProtocolRecognizer.h
	RecipeMapper.h
)

add_subdirectory(Palettes)