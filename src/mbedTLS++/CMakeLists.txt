target_sources(
	${CMAKE_PROJECT_NAME} PRIVATE

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
