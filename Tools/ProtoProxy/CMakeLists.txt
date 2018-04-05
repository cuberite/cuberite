cmake_minimum_required(VERSION 3.5)
project (ProtoProxy)

include(FlattenFiles)
include(SetFlags)

set_flags()
set_lib_flags()



set_exe_flags()

# Include the shared files:
set(SHARED_SRC
	../../src/ByteBuffer.cpp
	../../src/StringUtils.cpp
	../../src/UUID.cpp
	../../src/LoggerListeners.cpp
	../../src/Logger.cpp
)
set(SHARED_HDR
	../../src/ByteBuffer.h
	../../src/StringUtils.h
	../../src/UUID.h
)

flatten_files(SHARED_SRC)
flatten_files(SHARED_HDR)
source_group("Shared" FILES ${SHARED_SRC} ${SHARED_HDR})



# Include the main source files:
set(SOURCES
	Connection.cpp
	Globals.cpp
	ProtoProxy.cpp
	Server.cpp
)
set(HEADERS
	Connection.h
	Globals.h
	Server.h
)
source_group("" FILES ${SOURCES} ${HEADERS})

add_executable(ProtoProxy
	${SOURCES}
	${HEADERS}
	${SHARED_SRC}
	${SHARED_HDR}
)
target_include_directories(ProtoProxy PRIVATE ../../src/) # For the SHARED_HDRs
target_link_libraries(ProtoProxy PRIVATE zlib mbedtls fmt::fmt OSSupport)
