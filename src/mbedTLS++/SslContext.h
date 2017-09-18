
// SslContext.h

// Declares the cSslContext class that holds everything a single SSL context needs to function





#pragma once

#include "mbedtls/ssl.h"
#include "../ByteBuffer.h"





// fwd:
class cCtrDrbgContext;
class cSslConfig;





/**
Acts as a generic SSL encryptor / decryptor between the two endpoints. The "owner" of this class is expected
to create it, initialize it and then provide the means of reading and writing data through the SSL link.
This is an abstract base class, there are descendants that handle the specific aspects of how the SSL peer
data comes into the system:
	- cBufferedSslContext uses a cByteBuffer to read and write the data
	- cCallbackSslContext uses callbacks to provide the data
*/
class cSslContext abstract
{
public:
	/** Creates a new uninitialized context */
	cSslContext(void);

	virtual ~cSslContext();

	/** Initializes the context for use as a server or client.
	a_Config must not be nullptr and the config must not be changed after this call.
	Returns 0 on success, mbedTLS error on failure. */
	int Initialize(std::shared_ptr<const cSslConfig> a_Config);

	/** Initializes the context using the default config. */
	int Initialize(bool a_IsClient);

	/** Returns true if the object has been initialized properly. */
	bool IsValid(void) const { return m_IsValid; }

	/** Sets the SSL peer name expected for this context. Must be called after Initialize().
	\param a_ExpectedPeerName CommonName that we expect the SSL peer to have in its cert,
	if it is different, the verification will fail. An empty string will disable the CN check. */
	void SetExpectedPeerName(const AString & a_ExpectedPeerName);

	/** Writes data to be encrypted and sent to the SSL peer. Will perform SSL handshake, if needed.
	Returns the number of bytes actually written, or mbedTLS error code.
	If the return value is MBEDTLS_ERR_SSL_WANT_READ or MBEDTLS_ERR_SSL_WANT_WRITE, the owner should send any
	cached outgoing data to the SSL peer and write any incoming data received from the SSL peer and then call
	this function again with the same parameters. Note that this may repeat a few times before the data is
	actually written, mainly due to initial handshake. */
	int WritePlain(const void * a_Data, size_t a_NumBytes);

	/** Reads data decrypted from the SSL stream. Will perform SSL handshake, if needed.
	Returns the number of bytes actually read, or mbedTLS error code.
	If the return value is MBEDTLS_ERR_SSL_WANT_READ or MBEDTLS_ERR_SSL_WANT_WRITE, the owner should send any
	cached outgoing data to the SSL peer and write any incoming data received from the SSL peer and then call
	this function again with the same parameters. Note that this may repeat a few times before the data is
	actually read, mainly due to initial handshake. */
	int ReadPlain(void * a_Data, size_t a_MaxBytes);

	/** Performs the SSL handshake.
	Returns zero on success, mbedTLS error code on failure.
	If the return value is MBEDTLS_ERR_SSL_WANT_READ or MBEDTLS_ERR_SSL_WANT_WRITE, the owner should send any
	cached outgoing data to the SSL peer and write any incoming data received from the SSL peer and then call
	this function again. Note that this may repeat a few times before the handshake is completed. */
	int Handshake(void);

	/** Returns true if the SSL handshake has been completed. */
	bool HasHandshaken(void) const { return m_HasHandshaken; }

	/** Notifies the SSL peer that the connection is being closed.
	Returns 0 on success, mbedTLS error code on failure. */
	int NotifyClose(void);

protected:

	/** Configuration of the SSL context. */
	std::shared_ptr<const cSslConfig> m_Config;

	/** The SSL context that mbedTLS uses. */
	mbedtls_ssl_context m_Ssl;

	/** True if the object has been initialized properly. */
	bool m_IsValid;

	/** True if the SSL handshake has been completed. */
	bool m_HasHandshaken;

	/** The callback used by mbedTLS when it wants to read encrypted data. */
	static int ReceiveEncrypted(void * a_This, unsigned char * a_Buffer, size_t a_NumBytes)
	{
		return (reinterpret_cast<cSslContext *>(a_This))->ReceiveEncrypted(a_Buffer, a_NumBytes);
	}

	/** The callback used by mbedTLS when it wants to write encrypted data. */
	static int SendEncrypted(void * a_This, const unsigned char * a_Buffer, size_t a_NumBytes)
	{
		return (reinterpret_cast<cSslContext *>(a_This))->SendEncrypted(a_Buffer, a_NumBytes);
	}

	/** Called when mbedTLS wants to read encrypted data. */
	virtual int ReceiveEncrypted(unsigned char * a_Buffer, size_t a_NumBytes) = 0;

	/** Called when mbedTLS wants to write encrypted data. */
	virtual int SendEncrypted(const unsigned char * a_Buffer, size_t a_NumBytes) = 0;
} ;




