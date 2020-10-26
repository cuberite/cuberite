project (ProtoProxy)
find_package(Threads REQUIRED)

# Set include paths to the used libraries:
include_directories(SYSTEM "../../lib")
include_directories(SYSTEM "../../lib/mbedtls/include")
include_directories("../../src")

function(flatten_files arg1)
	set(res "")
	foreach(f ${${arg1}})
		get_filename_component(f ${f} ABSOLUTE)
		list(APPEND res ${f})
	endforeach()
	set(${arg1} "${res}" PARENT_SCOPE)
endfunction()

add_subdirectory(../../lib/zlib ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_FILES_DIRECTORY}/lib/zlib)

# Include the shared files:
set(SHARED_SRC
	../../src/ByteBuffer.cpp
	../../src/StringUtils.cpp
	../../src/UUID.cpp
	../../src/mbedTLS++/AesCfb128Decryptor.cpp
	../../src/mbedTLS++/AesCfb128Encryptor.cpp
	../../src/mbedTLS++/CryptoKey.cpp
	../../src/mbedTLS++/CtrDrbgContext.cpp
	../../src/mbedTLS++/EntropyContext.cpp
	../../src/mbedTLS++/RsaPrivateKey.cpp
	../../src/LoggerListeners.cpp
	../../src/Logger.cpp
)
set(SHARED_HDR
	../../src/ByteBuffer.h
	../../src/StringUtils.h
	../../src/UUID.h
	../../src/mbedTLS++/AesCfb128Decryptor.h
	../../src/mbedTLS++/AesCfb128Encryptor.h
	../../src/mbedTLS++/CryptoKey.h
	../../src/mbedTLS++/CtrDrbgContext.h
	../../src/mbedTLS++/EntropyContext.h
	../../src/mbedTLS++/RsaPrivateKey.h
)
set(SHARED_OSS_SRC
	../../src/OSSupport/CriticalSection.cpp
	../../src/OSSupport/Event.cpp
	../../src/OSSupport/File.cpp
	../../src/OSSupport/IsThread.cpp
	../../src/OSSupport/StackTrace.cpp
	../../src/OSSupport/WinStackWalker.cpp
)

set(SHARED_OSS_HDR
	../../src/OSSupport/CriticalSection.h
	../../src/OSSupport/Event.h
	../../src/OSSupport/File.h
	../../src/OSSupport/IsThread.h
	../../src/OSSupport/StackTrace.h
	../../src/OSSupport/WinStackWalker.h
)

flatten_files(SHARED_SRC)
flatten_files(SHARED_HDR)
flatten_files(SHARED_OSS_SRC)
flatten_files(SHARED_OSS_HDR)
source_group("Shared" FILES ${SHARED_SRC} ${SHARED_HDR})
source_group("Shared\\OSSupport" FILES ${SHARED_OSS_SRC} ${SHARED_OSS_HDR})



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
	${SHARED_OSS_SRC}
	${SHARED_OSS_HDR}
)

target_link_libraries(ProtoProxy zlib mbedtls fmt::fmt Threads::Threads)

include(../../SetFlags.cmake)
set_exe_flags(ProtoProxy)
