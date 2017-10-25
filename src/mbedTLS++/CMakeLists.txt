project (Cuberite)

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
	SslConfig.cpp
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
	ErrorCodes.h
	RsaPrivateKey.h
	SslConfig.h
	SslContext.h
	Sha1Checksum.h
	X509Cert.h
)

if(NOT MSVC)
	add_library(mbedTLS++ ${SRCS} ${HDRS})
	target_link_libraries(mbedTLS++ fmt::fmt mbedtls)
endif()
