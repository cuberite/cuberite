
// LuaTCPLink.h

// Declares the cLuaTCPLink class representing a Lua wrapper for the cTCPLink class and the callbacks it needs





#pragma once

#include "../OSSupport/Network.h"
#include "LuaState.h"





// fwd:
class cLuaServerHandle;
typedef std::weak_ptr<cLuaServerHandle> cLuaServerHandleWPtr;





class cLuaTCPLink:
	public cNetwork::cConnectCallbacks,
	public cTCPLink::cCallbacks
{
public:
	/** Creates a new instance of the link, wrapping the callbacks that are in the specified table. */
	cLuaTCPLink(cLuaState::cTableRefPtr && a_Callbacks);

	/** Creates a new instance of the link, attached to the specified plugin and wrapping the callbacks that are in the specified referenced table. */
	cLuaTCPLink(cLuaState::cTableRefPtr && a_Callbacks, cLuaServerHandleWPtr a_Server);

	virtual ~cLuaTCPLink() override;

	/** Sends the data contained in the string to the remote peer.
	Returns true if successful, false on immediate failure (queueing the data failed or link not available). */
	bool Send(const AString & a_Data);

	/** Returns the IP address of the local endpoint of the connection. */
	AString GetLocalIP(void) const;

	/** Returns the port used by the local endpoint of the connection. */
	UInt16 GetLocalPort(void) const;

	/** Returns the IP address of the remote endpoint of the connection. */
	AString GetRemoteIP(void) const;

	/** Returns the port used by the remote endpoint of the connection. */
	UInt16 GetRemotePort(void) const;

	/** Closes the link gracefully.
	The link will send any queued outgoing data, then it will send the FIN packet.
	The link will still receive incoming data from remote until the remote closes the connection. */
	void Shutdown(void);

	/** Drops the connection without any more processing.
	Sends the RST packet, queued outgoing and incoming data is lost. */
	void Close(void);

	/** Starts a TLS handshake as a client connection.
	If a client certificate should be used for the connection, set the certificate into a_OwnCertData and
	its corresponding private key to a_OwnPrivKeyData. If both are empty, no client cert is presented.
	a_OwnPrivKeyPassword is the password to be used for decoding PrivKey, empty if not passworded.
	Returns empty string on success, non-empty error description on failure. */
	AString StartTLSClient(
		const AString & a_OwnCertData,
		const AString & a_OwnPrivKeyData,
		const AString & a_OwnPrivKeyPassword
	);

	/** Starts a TLS handshake as a server connection.
	Set the server certificate into a_CertData and its corresponding private key to a_OwnPrivKeyData.
	a_OwnPrivKeyPassword is the password to be used for decoding PrivKey, empty if not passworded.
	a_StartTLSData is any data that should be pushed into the TLS before reading more data from the remote.
	This is used mainly for protocols starting TLS in the middle of communication, when the TLS start command
	can be received together with the TLS Client Hello message in one OnReceivedData() call, to re-queue the
	Client Hello message into the TLS handshake buffer.
	Returns empty string on success, non-empty error description on failure. */
	AString StartTLSServer(
		const AString & a_OwnCertData,
		const AString & a_OwnPrivKeyData,
		const AString & a_OwnPrivKeyPassword,
		const AString & a_StartTLSData
	);

protected:

	/** The Lua table that holds the callbacks to be invoked. */
	cLuaState::cTableRefPtr m_Callbacks;

	/** The underlying link representing the connection.
	May be nullptr. */
	cTCPLinkPtr m_Link;

	/** The server that is responsible for this link, if any. */
	cLuaServerHandleWPtr m_Server;

	/** Common code called when the link is considered as terminated.
	Releases m_Link, m_Callbacks and this from m_Server, each when applicable. */
	void Terminated(void);

	/** Called by the SSL context when there's incoming data available in the cleartext.
	Reports the data via the Lua callback function. */
	void ReceivedCleartextData(const char * a_Data, size_t a_NumBytes);

	// cNetwork::cConnectCallbacks overrides:
	virtual void OnConnected(cTCPLink & a_Link) override;
	virtual void OnError(int a_ErrorCode, const AString & a_ErrorMsg) override;

	// cTCPLink::cCallbacks overrides:
	virtual void OnLinkCreated(cTCPLinkPtr a_Link) override;
	virtual void OnReceivedData(const char * a_Data, size_t a_Length) override;
	virtual void OnRemoteClosed(void) override;
	// The OnError() callback is shared with cNetwork::cConnectCallbacks
};




