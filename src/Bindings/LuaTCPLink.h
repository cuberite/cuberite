
// LuaTCPLink.h

// Declares the cLuaTCPLink class representing a Lua wrapper for the cTCPLink class and the callbacks it needs





#pragma once

#include "../OSSupport/Network.h"
#include "../PolarSSL++/SslContext.h"
#include "LuaState.h"





// fwd:
class cLuaServerHandle;
typedef WeakPtr<cLuaServerHandle> cLuaServerHandleWPtr;





class cLuaTCPLink:
	public cNetwork::cConnectCallbacks,
	public cTCPLink::cCallbacks
{
public:
	/** Creates a new instance of the link, wrapping the callbacks that are in the specified table. */
	cLuaTCPLink(cLuaState::cTableRefPtr && a_Callbacks);

	/** Creates a new instance of the link, attached to the specified plugin and wrapping the callbacks that are in the specified referenced table. */
	cLuaTCPLink(cLuaState::cTableRefPtr && a_Callbacks, cLuaServerHandleWPtr a_Server);

	~cLuaTCPLink();

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
	// fwd:
	class cLinkSslContext;
	typedef SharedPtr<cLinkSslContext> cLinkSslContextPtr;
	typedef WeakPtr<cLinkSslContext> cLinkSslContextWPtr;

	/** Wrapper around cSslContext that is used when this link is being encrypted by SSL. */
	class cLinkSslContext :
		public cSslContext
	{
		cLuaTCPLink & m_Link;

		/** Buffer for storing the incoming encrypted data until it is requested by the SSL decryptor. */
		AString m_EncryptedData;

		/** Buffer for storing the outgoing cleartext data until the link has finished handshaking. */
		AString m_CleartextData;

		/** Shared ownership of self, so that this object can keep itself alive for as long as it needs. */
		cLinkSslContextWPtr m_Self;

	public:
		cLinkSslContext(cLuaTCPLink & a_Link);

		/** Shares ownership of self, so that this object can keep itself alive for as long as it needs. */
		void SetSelf(cLinkSslContextWPtr a_Self);

		/** Removes the self ownership so that we can detect the SSL closure. */
		void ResetSelf(void);

		/** Stores the specified block of data into the buffer of the data to be decrypted (incoming from remote).
		Also flushes the SSL buffers by attempting to read any data through the SSL context. */
		void StoreReceivedData(const char * a_Data, size_t a_NumBytes);

		/** Tries to read any cleartext data available through the SSL, reports it in the link. */
		void FlushBuffers(void);

		/** Tries to finish handshaking the SSL. */
		void TryFinishHandshaking(void);

		/** Sends the specified cleartext data over the SSL to the remote peer.
		If the handshake hasn't been completed yet, queues the data for sending when it completes. */
		void Send(const AString & a_Data);

		// cSslContext overrides:
		virtual int ReceiveEncrypted(unsigned char * a_Buffer, size_t a_NumBytes) override;
		virtual int SendEncrypted(const unsigned char * a_Buffer, size_t a_NumBytes) override;
	};


	/** The Lua table that holds the callbacks to be invoked. */
	cLuaState::cTableRefPtr m_Callbacks;

	/** The underlying link representing the connection.
	May be nullptr. */
	cTCPLinkPtr m_Link;

	/** The server that is responsible for this link, if any. */
	cLuaServerHandleWPtr m_Server;

	/** The SSL context used for encryption, if this link uses SSL.
	If valid, the link uses encryption through this context. */
	cLinkSslContextPtr m_SslContext;


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




