
// SslContext.h

// Declares the cSslContext class that holds everything a single SSL context needs to function





#pragma once

#include "polarssl/ssl.h"
#include "../ByteBuffer.h"
#include "CryptoKey.h"
#include "RsaPrivateKey.h"
#include "X509Cert.h"





// fwd:
class cCtrDrbgContext;





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
	Returns 0 on success, PolarSSL error on failure. */
	int Initialize(bool a_IsClient, const SharedPtr<cCtrDrbgContext> & a_CtrDrbg = SharedPtr<cCtrDrbgContext>());
	
	/** Returns true if the object has been initialized properly. */
	bool IsValid(void) const { return m_IsValid; }
	
	/** Sets the certificate to use as our own. Must be used when representing a server, optional when client.
	Must be called after Initialize(). */
	void SetOwnCert(const cX509CertPtr & a_OwnCert, const cRsaPrivateKeyPtr & a_OwnCertPrivKey);
	
	/** Sets the certificate to use as our own. Must be used when representing a server, optional when client.
	Must be called after Initialize(). */
	void SetOwnCert(const cX509CertPtr & a_OwnCert, const cCryptoKeyPtr & a_OwnCertPrivKey);
	
	/** Sets a cert chain as the trusted cert store for this context. Must be called after Initialize().
	Calling this will switch the context into strict cert verification mode.
	a_ExpectedPeerName is the CommonName that we expect the SSL peer to have in its cert,
	if it is different, the verification will fail. An empty string will disable the CN check. */
	void SetCACerts(const cX509CertPtr & a_CACert, const AString & a_ExpectedPeerName);
	
	/** Writes data to be encrypted and sent to the SSL peer. Will perform SSL handshake, if needed.
	Returns the number of bytes actually written, or PolarSSL error code.
	If the return value is POLARSSL_ERR_NET_WANT_READ or POLARSSL_ERR_NET_WANT_WRITE, the owner should send any
	cached outgoing data to the SSL peer and write any incoming data received from the SSL peer and then call
	this function again with the same parameters. Note that this may repeat a few times before the data is
	actually written, mainly due to initial handshake. */
	int WritePlain(const void * a_Data, size_t a_NumBytes);
	
	/** Reads data decrypted from the SSL stream. Will perform SSL handshake, if needed.
	Returns the number of bytes actually read, or PolarSSL error code.
	If the return value is POLARSSL_ERR_NET_WANT_READ or POLARSSL_ERR_NET_WANT_WRITE, the owner should send any
	cached outgoing data to the SSL peer and write any incoming data received from the SSL peer and then call
	this function again with the same parameters. Note that this may repeat a few times before the data is
	actually read, mainly due to initial handshake. */
	int ReadPlain(void * a_Data, size_t a_MaxBytes);
	
	/** Performs the SSL handshake.
	Returns zero on success, PoladSSL error code on failure.
	If the return value is POLARSSL_ERR_NET_WANT_READ or POLARSSL_ERR_NET_WANT_WRITE, the owner should send any
	cached outgoing data to the SSL peer and write any incoming data received from the SSL peer and then call
	this function again. Note that this may repeat a few times before the handshake is completed. */
	int Handshake(void);
	
	/** Returns true if the SSL handshake has been completed. */
	bool HasHandshaken(void) const { return m_HasHandshaken; }
	
	/** Notifies the SSL peer that the connection is being closed.
	Returns 0 on success, PolarSSL error code on failure. */
	int NotifyClose(void);
	
protected:
	/** True if the object has been initialized properly. */
	bool m_IsValid;
	
	/** The random generator to use */
	SharedPtr<cCtrDrbgContext> m_CtrDrbg;
	
	/** The SSL context that PolarSSL uses. */
	ssl_context m_Ssl;
	
	/** The certificate that we present to the peer. */
	cX509CertPtr m_OwnCert;
	
	/** Private key for m_OwnCert, if initialized from a cRsaPrivateKey. */
	cRsaPrivateKeyPtr m_OwnCertPrivKey;
	
	/** Private key for m_OwnCert, if initialized from a cCryptoKey. */
	cCryptoKeyPtr m_OwnCertPrivKey2;
	
	/** True if the SSL handshake has been completed. */
	bool m_HasHandshaken;
	
	/** A copy of the trusted CA root cert store that is passed to us in SetCACerts(), so that the pointer
	stays valid even after the call, when PolarSSL finally uses it. */
	cX509CertPtr m_CACerts;
	
	/** Buffer for the expected peer name. We need to buffer it because the caller may free the string they
	give us before PolarSSL consumes the raw pointer it gets to the CN. */
	AString m_ExpectedPeerName;
	
	
	/** The callback used by PolarSSL when it wants to read encrypted data. */
	static int ReceiveEncrypted(void * a_This, unsigned char * a_Buffer, size_t a_NumBytes)
	{
		return ((cSslContext *)a_This)->ReceiveEncrypted(a_Buffer, a_NumBytes);
	}
	
	/** The callback used by PolarSSL when it wants to write encrypted data. */
	static int SendEncrypted(void * a_This, const unsigned char * a_Buffer, size_t a_NumBytes)
	{
		return ((cSslContext *)a_This)->SendEncrypted(a_Buffer, a_NumBytes);
	}
	
	#ifdef _DEBUG
		/** The callback used by PolarSSL to output debug messages */
		static void SSLDebugMessage(void * a_UserParam, int a_Level, const char * a_Text);
		
		/** The callback used by PolarSSL to log information on the cert chain */
		static int SSLVerifyCert(void * a_This, x509_crt * a_Crt, int a_Depth, int * a_Flags);
	#endif  // _DEBUG

	/** Called when PolarSSL wants to read encrypted data. */
	virtual int ReceiveEncrypted(unsigned char * a_Buffer, size_t a_NumBytes) = 0;
	
	/** Called when PolarSSL wants to write encrypted data. */
	virtual int SendEncrypted(const unsigned char * a_Buffer, size_t a_NumBytes) = 0;
} ;




