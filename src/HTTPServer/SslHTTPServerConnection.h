
// SslHTTPServerConnection.h

// Declared the cSslHTTPServerConnection class representing a HTTP connection made over a SSL link





#pragma once

#include "HTTPServerConnection.h"
#include "PolarSSL++/BufferedSslContext.h"





class cSslHTTPServerConnection :
	public cHTTPServerConnection
{
	typedef cHTTPServerConnection super;

public:
	/** Creates a new connection on the specified server.
	Sends the specified cert as the server certificate, uses the private key for decryption. */
	cSslHTTPServerConnection(cHTTPServer & a_HTTPServer, const cX509CertPtr & a_Cert, const cCryptoKeyPtr & a_PrivateKey);

	~cSslHTTPServerConnection();

protected:
	cBufferedSslContext m_Ssl;

	/** The certificate to send to the client */
	cX509CertPtr m_Cert;

	/** The private key used for the certificate */
	cCryptoKeyPtr m_PrivateKey;

	// cHTTPConnection overrides:
	virtual void OnReceivedData(const char * a_Data, size_t a_Size) override;  // Data is received from the client
	virtual void SendData(const void * a_Data, size_t a_Size) override;  // Data is to be sent to client
} ;




