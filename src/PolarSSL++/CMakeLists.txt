cmake_minimum_required (VERSION 2.6)
project (MCServer)

include_directories ("${PROJECT_SOURCE_DIR}/../")

set(SRCS
	AesCfb128Decryptor.cpp
	AesCfb128Encryptor.cpp
	BlockingSslClientSocket.cpp
	BufferedSslContext.cpp
	CallbackSslContext.cpp
	CtrDrbgContext.cpp
	CryptoKey.cpp
	EntropyContext.cpp
	RsaPrivateKey.cpp
	Sha1Checksum.cpp
	SslContext.cpp
	X509Cert.cpp
)

set(HDRS
	AesCfb128Decryptor.h
	AesCfb128Encryptor.h
	BlockingSslClientSocket.h
	BufferedSslContext.h
	CallbackSslContext.h
	CtrDrbgContext.h
	CryptoKey.h
	EntropyContext.h
	RsaPrivateKey.h
	SslContext.h
	Sha1Checksum.h
	X509Cert.h
)

if(NOT MSVC)
	add_library(PolarSSL++ ${SRCS} ${HDRS})

	if (UNIX)
		target_link_libraries(PolarSSL++ polarssl)
	endif()
endif()
