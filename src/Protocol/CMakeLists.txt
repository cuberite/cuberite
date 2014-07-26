
cmake_minimum_required (VERSION 2.6)
project (MCServer)

include_directories ("${PROJECT_SOURCE_DIR}/../")

SET (SRCS
	Authenticator.cpp
	ChunkDataSerializer.cpp
	Protocol125.cpp
	Protocol132.cpp
	Protocol14x.cpp
	Protocol15x.cpp
	Protocol16x.cpp
	Protocol17x.cpp
	ProtocolRecognizer.cpp)

SET (HDRS
	Authenticator.h
	ChunkDataSerializer.h
	Protocol.h
	Protocol125.h
	Protocol132.h
	Protocol14x.h
	Protocol15x.h
	Protocol16x.h
	Protocol17x.h
	ProtocolRecognizer.h)

if(NOT MSVC)
	add_library(Protocol ${SRCS} ${HDRS})
endif()
