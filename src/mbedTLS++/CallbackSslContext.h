
// CallbackSslContext.h

// Declares the cCallbackSslContext class representing a SSL context wrapper that uses callbacks to read and write SSL peer data





#pragma once

#include "SslContext.h"
#include "ErrorCodes.h"





class cCallbackSslContext :
	public cSslContext
{
public:
	/** Interface used as a data sink for the SSL peer data. */
	class cDataCallbacks
	{
	public:
		// Force a virtual destructor in descendants:
		virtual ~cDataCallbacks() {}

		/** Called when mbedTLS wants to read encrypted data from the SSL peer.
		The returned value is the number of bytes received, or a mbedTLS error on failure.
		The implementation can return MBEDTLS_ERR_SSL_WANT_READ or MBEDTLS_ERR_SSL_WANT_WRITE to indicate
		that there's currently no more data and that there might be more data in the future. In such cases the
		SSL operation that invoked this call will terminate with the same return value, so that the owner is
		notified of this condition and can potentially restart the operation later on. */
		virtual int ReceiveEncrypted(unsigned char * a_Buffer, size_t a_NumBytes) = 0;

		/** Called when mbedTLS wants to write encrypted data to the SSL peer.
		The returned value is the number of bytes sent, or a mbedTLS error on failure.
		The implementation can return MBEDTLS_ERR_SSL_WANT_READ or MBEDTLS_ERR_SSL_WANT_WRITE to indicate
		that there's currently no more data and that there might be more data in the future. In such cases the
		SSL operation that invoked this call will terminate with the same return value, so that the owner is
		notified of this condition and can potentially restart the operation later on. */
		virtual int SendEncrypted(const unsigned char * a_Buffer, size_t a_NumBytes) = 0;
	} ;


	/** Creates a new SSL context with no callbacks assigned */
	cCallbackSslContext(void);

	/** Creates a new SSL context with the specified callbacks */
	cCallbackSslContext(cDataCallbacks & a_Callbacks);

protected:
	/** The callbacks to use to send and receive SSL peer data */
	cDataCallbacks * m_Callbacks;

	// cSslContext overrides:
	virtual int ReceiveEncrypted(unsigned char * a_Buffer, size_t a_NumBytes) override;
	virtual int SendEncrypted(const unsigned char * a_Buffer, size_t a_NumBytes) override;
};




