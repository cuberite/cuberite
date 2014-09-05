
// SslHTTPConnection.h

// Declared the cSslHTTPConnection class representing a HTTP connection made over a SSL link





#pragma once

#include "HTTPConnection.h"
#include "PolarSSL++/BufferedSslContext.h"





class cSslHTTPConnection :
	public cHTTPConnection
{
	typedef cHTTPConnection super;
	
public:
	/** Creates a new connection on the specified server.
	Sends the specified cert as the server certificate, uses the private key for decryption. */
	cSslHTTPConnection(cHTTPServer & a_HTTPServer, const cX509CertPtr & a_Cert, const cCryptoKeyPtr & a_PrivateKey);
	
protected:
	cBufferedSslContext m_Ssl;
	
	/** The certificate to send to the client */
	cX509CertPtr m_Cert;
	
	/** The private key used for the certificate */
	cCryptoKeyPtr m_PrivateKey;
	
	// cHTTPConnection overrides:
	virtual bool DataReceived   (const char * a_Data, size_t a_Size) override;  // Data is received from the client
	virtual void GetOutgoingData(AString & a_Data) override;  // Data can be sent to client
} ;




