include_directories (SYSTEM "../../lib/jsoncpp/include")

SET (SRCS
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
	ProtocolPalettes.cpp
	ProtocolRecognizer.cpp
)

SET (HDRS
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
	ProtocolPalettes.h
	ProtocolRecognizer.h
)

if (NOT MSVC)
	add_library(Protocol ${SRCS} ${HDRS})
	target_link_libraries(Protocol fmt::fmt)
endif()
