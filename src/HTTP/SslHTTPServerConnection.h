
// SslHTTPServerConnection.h

// Declares the cSslHTTPServerConnection class representing a HTTP connection made over an SSL link





#pragma once

#include "HTTPServerConnection.h"
#include "../mbedTLS++/BufferedSslContext.h"





class cSslHTTPServerConnection:
	public cHTTPServerConnection
{
	using Super = cHTTPServerConnection;

public:

	/** Creates a new connection on the specified server.
	Sends the specified cert as the server certificate, uses the private key for decryption. */
	cSslHTTPServerConnection(cHTTPServer & a_HTTPServer, const std::shared_ptr<const cSslConfig> & a_Config);

	virtual ~cSslHTTPServerConnection() override;

protected:
	cBufferedSslContext m_Ssl;

	// cHTTPConnection overrides:
	virtual void OnReceivedData(const char * a_Data, size_t a_Size) override;  // Data is received from the client
	virtual void SendData(const void * a_Data, size_t a_Size) override;  // Data is to be sent to client
} ;




