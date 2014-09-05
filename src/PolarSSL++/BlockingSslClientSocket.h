
// BlockingSslClientSocket.h

// Declares the cBlockingSslClientSocket class representing a blocking TCP socket with client SSL encryption over it





#pragma once

#include "CallbackSslContext.h"
#include "../OSSupport/Socket.h"





class cBlockingSslClientSocket :
	protected cCallbackSslContext::cDataCallbacks
{
public:
	cBlockingSslClientSocket(void);
	
	/** Connects to the specified server and performs SSL handshake.
	Returns true if successful, false on failure. Sets internal error text on failure. */
	bool Connect(const AString & a_ServerName, UInt16 a_Port);
	
	/** Sends the specified data over the connection.
	Returns true if successful, false on failure. Sets the internal error text on failure. */
	bool Send(const void * a_Data, size_t a_NumBytes);
	
	/** Receives data from the connection.
	Blocks until there is any data available, then returns as much as possible.
	Returns the number of bytes actually received, negative number on failure.
	Sets the internal error text on failure. */
	int Receive(void * a_Data, size_t a_MaxBytes);
	
	/** Disconnects the connection gracefully, if possible.
	Note that this also frees the internal SSL context, so all the certificates etc. are lost. */
	void Disconnect(void);
	
	/** Sets the root certificates that are to be trusted. Forces the connection to use strict cert
	verification. Needs to be used before calling Connect().
	a_ExpectedPeerName is the name that we expect to receive in the SSL peer's cert; verification will fail if
	the presented name is different (possible MITM).
	Returns true on success, false on failure. Sets internal error text on failure. */
	bool SetTrustedRootCertsFromString(const AString & a_CACerts, const AString & a_ExpectedPeerName);
	
	/** Returns the text of the last error that has occurred in this instance. */
	const AString & GetLastErrorText(void) const { return m_LastErrorText; }
	
protected:
	/** The SSL context used for the socket */
	cCallbackSslContext m_Ssl;
	
	/** The underlying socket to the SSL server */
	cSocket m_Socket;
	
	/** The trusted CA root cert store, if we are to verify the cert strictly. Set by SetTrustedRootCertsFromString(). */
	cX509CertPtr m_CACerts;
	
	/** The expected SSL peer's name, if we are to verify the cert strictly. Set by SetTrustedRootCertsFromString(). */
	AString m_ExpectedPeerName;
	
	/** Text of the last error that has occurred. */
	AString m_LastErrorText;
	
	/** Set to true if the connection established successfully. */
	bool m_IsConnected;
	
	
	// cCallbackSslContext::cDataCallbacks overrides:
	virtual int ReceiveEncrypted(unsigned char * a_Buffer, size_t a_NumBytes) override;
	virtual int SendEncrypted(const unsigned char * a_Buffer, size_t a_NumBytes) override;
} ;




