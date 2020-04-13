
// BufferedSslContext.h

// Declares the cBufferedSslContext class representing a SSL context with the SSL peer data backed by a cByteBuffer





#pragma once

#include "SslContext.h"
#include "ErrorCodes.h"





class cBufferedSslContext:
	public cSslContext
{
	using Super = cSslContext;

public:

	/** Creates a new context with the buffers of specified size for the encrypted / decrypted data. */
	cBufferedSslContext(size_t a_BufferSize = 64000);

	/** Stores the specified data in the "incoming" buffer, to be process by the SSL decryptor.
	This is the data received from the SSL peer.
	Returns the number of bytes actually stored. If 0 is returned, owner should check the error state. */
	size_t WriteIncoming(const void * a_Data, size_t a_NumBytes);

	/** Retrieves data from the "outgoing" buffer, after being processed by the SSL encryptor.
	This is the data to be sent to the SSL peer.
	Returns the number of bytes actually retrieved. */
	size_t ReadOutgoing(void * a_Data, size_t a_DataMaxSize);

protected:
	/** Buffer for the data that has been encrypted into the SSL stream and should be sent out. */
	cByteBuffer m_OutgoingData;

	/** Buffer for the data that has come in and needs to be decrypted from the SSL stream. */
	cByteBuffer m_IncomingData;


	// cSslContext overrides:
	virtual int ReceiveEncrypted(unsigned char * a_Buffer, size_t a_NumBytes) override;
	virtual int SendEncrypted(const unsigned char * a_Buffer, size_t a_NumBytes) override;
} ;




