
cmake_minimum_required (VERSION 2.6)
project (MCServer)

include_directories ("${PROJECT_SOURCE_DIR}/../")

SET (SRCS
	Authenticator.cpp
	ChunkDataSerializer.cpp
	MojangAPI.cpp
	Protocol17x.cpp
	Protocol18x.cpp
	ProtocolUnknown.cpp
	ProtocolRecognizer.cpp)

SET (HDRS
	Authenticator.h
	ChunkDataSerializer.h
	MojangAPI.h
	Protocol.h
	Protocol17x.h
	Protocol18x.h
	ProtocolUnknown.h
	ProtocolRecognizer.h)

if(NOT MSVC)
	add_library(Protocol ${SRCS} ${HDRS})
endif()
