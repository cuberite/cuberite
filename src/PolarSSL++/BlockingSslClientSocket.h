
// BlockingSslClientSocket.h

// Declares the cBlockingSslClientSocket class representing a blocking TCP socket with client SSL encryption over it





#pragma once

#include "OSSupport/Network.h"
#include "CallbackSslContext.h"





class cBlockingSslClientSocket :
	protected cCallbackSslContext::cDataCallbacks
{
public:
	cBlockingSslClientSocket(void);

	~cBlockingSslClientSocket(void)
	{
		Disconnect();
	}
	
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
	friend class cBlockingSslClientSocketConnectCallbacks;
	friend class cBlockingSslClientSocketLinkCallbacks;

	/** The SSL context used for the socket */
	cCallbackSslContext m_Ssl;
	
	/** The underlying socket to the SSL server */
	cTCPLinkPtr m_Socket;

	/** The object used to signal state changes in the socket (the cause of the blocking). */
	cEvent m_Event;
	
	/** The trusted CA root cert store, if we are to verify the cert strictly. Set by SetTrustedRootCertsFromString(). */
	cX509CertPtr m_CACerts;
	
	/** The expected SSL peer's name, if we are to verify the cert strictly. Set by SetTrustedRootCertsFromString(). */
	AString m_ExpectedPeerName;

	/** The hostname to which the socket is connecting (stored for error reporting). */
	AString m_ServerName;
	
	/** Text of the last error that has occurred. */
	AString m_LastErrorText;
	
	/** Set to true if the connection established successfully. */
	bool m_IsConnected;

	/** Protects m_IncomingData against multithreaded access. */
	cCriticalSection m_CSIncomingData;

	/** Buffer for the data incoming on the network socket.
	Protected by m_CSIncomingData. */
	AString m_IncomingData;
	
	
	/** Called when the connection is established successfully. */
	void OnConnected(void);

	/** Called when an error occurs while connecting the socket. */
	void OnConnectError(const AString & a_ErrorMsg);

	/** Called when there's incoming data from the socket. */
	void OnReceivedData(const char * a_Data, size_t a_Size);

	/** Called when the link for the connection is created. */
	void SetLink(cTCPLinkPtr a_Link);

	/** Called when the link is disconnected, either gracefully or by an error. */
	void OnDisconnected(void);

	// cCallbackSslContext::cDataCallbacks overrides:
	virtual int ReceiveEncrypted(unsigned char * a_Buffer, size_t a_NumBytes) override;
	virtual int SendEncrypted(const unsigned char * a_Buffer, size_t a_NumBytes) override;
} ;




