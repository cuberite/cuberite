
cmake_minimum_required (VERSION 2.6)
project (Cuberite)

include_directories ("${PROJECT_SOURCE_DIR}/../")

SET (SRCS
	Authenticator.cpp
	ChunkDataSerializer.cpp
	MojangAPI.cpp
	Packetizer.cpp
	Protocol17x.cpp
	Protocol18x.cpp
	ProtocolRecognizer.cpp
)

SET (HDRS
	Authenticator.h
	ChunkDataSerializer.h
	MojangAPI.h
	Packetizer.h
	Protocol.h
	Protocol17x.h
	Protocol18x.h
	ProtocolRecognizer.h
)

get_directory_property(BINDING_OUTPUTS DIRECTORY ../ DEFINITION BINDING_OUTPUTS)
set_source_files_properties(${BINDING_OUTPUTS} PROPERTIES GENERATED TRUE)

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
	set_source_files_properties(Protocol18x.cpp PROPERTIES COMPILE_FLAGS "-Wno-error=old-style-cast -Wno-error=sign-conversion -Wno-error=conversion -Wno-error=switch-enum -Wno-error=switch")
	set_source_files_properties(Protocol17x.cpp PROPERTIES COMPILE_FLAGS "-Wno-error=switch-enum ")
endif()

if (NOT MSVC)
	add_library(Protocol ${SRCS} ${HDRS})
	add_dependencies(Protocol BindingsGeneration)
endif()
