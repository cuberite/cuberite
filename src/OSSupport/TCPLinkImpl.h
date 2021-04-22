
// TCPLinkImpl.h

// Declares the cTCPLinkImpl class implementing the TCP link functionality

// This is an internal header, no-one outside OSSupport should need to include it; use Network.h instead





#pragma once

#include "Network.h"
#include <event2/event.h>
#include <event2/bufferevent.h>
#include "../mbedTLS++/SslContext.h"





// fwd:
class cServerHandleImpl;
typedef std::shared_ptr<cServerHandleImpl> cServerHandleImplPtr;
class cTCPLinkImpl;
typedef std::shared_ptr<cTCPLinkImpl> cTCPLinkImplPtr;
typedef std::vector<cTCPLinkImplPtr> cTCPLinkImplPtrs;





class cTCPLinkImpl:
	public cTCPLink
{
	using Super = cTCPLink;

public:

	/** Creates a new link based on the given socket.
	Used for connections accepted in a server using cNetwork::Listen().
	a_Address and a_AddrLen describe the remote peer that has connected.
	The link is created disabled, you need to call Enable() to start the regular communication. */
	cTCPLinkImpl(evutil_socket_t a_Socket, cCallbacksPtr a_LinkCallbacks, cServerHandleImplPtr a_Server, const sockaddr * a_Address, socklen_t a_AddrLen);

	/** Destroys the LibEvent handle representing the link. */
	virtual ~cTCPLinkImpl() override;

	/** Queues a connection request to the specified host.
	a_ConnectCallbacks must be valid.
	Returns a link that has the connection request queued, or NULL for failure. */
	static cTCPLinkImplPtr Connect(const AString & a_Host, UInt16 a_Port, cTCPLink::cCallbacksPtr a_LinkCallbacks, cNetwork::cConnectCallbacksPtr a_ConnectCallbacks);

	/** Enables communication over the link.
	Links are created with communication disabled, so that creation callbacks can be called first.
	This function then enables the regular communication to be reported.
	The a_Self parameter is used so that the socket can keep itself alive as long as the callbacks are coming. */
	void Enable(cTCPLinkImplPtr a_Self);

	// cTCPLink overrides:
	virtual bool Send(const void * a_Data, size_t a_Length) override;
	virtual AString GetLocalIP(void) const override { return m_LocalIP; }
	virtual UInt16 GetLocalPort(void) const override { return m_LocalPort; }
	virtual AString GetRemoteIP(void) const override { return m_RemoteIP; }
	virtual UInt16 GetRemotePort(void) const override { return m_RemotePort; }
	virtual void Shutdown(void) override;
	virtual void Close(void) override;
	virtual AString StartTLSClient(
		cX509CertPtr a_OwnCert,
		cCryptoKeyPtr a_OwnPrivKey
	) override;
	virtual AString StartTLSServer(
		cX509CertPtr a_OwnCert,
		cCryptoKeyPtr a_OwnPrivKey,
		const AString & a_StartTLSData
	) override;

protected:

	// fwd:
	class cLinkTlsContext;
	typedef std::shared_ptr<cLinkTlsContext> cLinkTlsContextPtr;
	typedef std::weak_ptr<cLinkTlsContext> cLinkTlsContextWPtr;

	/** Wrapper around cSslContext that is used when this link is being encrypted by SSL. */
	class cLinkTlsContext :
		public cSslContext
	{
		cTCPLinkImpl & m_Link;

		/** Buffer for storing the incoming encrypted data until it is requested by the SSL decryptor. */
		AString m_EncryptedData;

		/** Buffer for storing the outgoing cleartext data until the link has finished handshaking. */
		AString m_CleartextData;

		/** Shared ownership of self, so that this object can keep itself alive for as long as it needs. */
		cLinkTlsContextWPtr m_Self;

	public:
		cLinkTlsContext(cTCPLinkImpl & a_Link);

		/** Shares ownership of self, so that this object can keep itself alive for as long as it needs. */
		void SetSelf(cLinkTlsContextWPtr a_Self);

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
		void Send(const void * a_Data, size_t a_Length);

		// cSslContext overrides:
		virtual int ReceiveEncrypted(unsigned char * a_Buffer, size_t a_NumBytes) override;
		virtual int SendEncrypted(const unsigned char * a_Buffer, size_t a_NumBytes) override;

		/** Returns true if the context's associated TCP link is the same link as a_Link. */
		bool IsLink(cTCPLinkImpl * a_Link)
		{
			return (a_Link == &m_Link);
		}
	};


	/** Callbacks to call when the connection is established.
	May be NULL if not used. Only used for outgoing connections (cNetwork::Connect()). */
	cNetwork::cConnectCallbacksPtr m_ConnectCallbacks;

	/** The LibEvent handle representing this connection. */
	bufferevent * m_BufferEvent;

	/** The server handle that has created this link.
	Only valid for incoming connections, nullptr for outgoing connections. */
	cServerHandleImplPtr m_Server;

	/** The IP address of the local endpoint. Valid only after the socket has been connected. */
	AString m_LocalIP;

	/** The port of the local endpoint. Valid only after the socket has been connected. */
	UInt16 m_LocalPort;

	/** The IP address of the remote endpoint. Valid only after the socket has been connected. */
	AString m_RemoteIP;

	/** The port of the remote endpoint. Valid only after the socket has been connected. */
	UInt16 m_RemotePort;

	/** SharedPtr to self, used to keep this object alive as long as the callbacks are coming.
	Initialized in Enable(), cleared in Close() and EventCallback(RemoteClosed). */
	cTCPLinkImplPtr m_Self;

	/** If true, Shutdown() has been called and is in queue.
	No more data is allowed to be sent via Send() and after all the currently buffered
	data is sent to the OS TCP stack, the socket gets shut down. */
	bool m_ShouldShutdown;

	/** The SSL context used for encryption, if this link uses SSL.
	If valid, the link uses encryption through this context. */
	cLinkTlsContextPtr m_TlsContext;


	/** Creates a new link to be queued to connect to a specified host:port.
	Used for outgoing connections created using cNetwork::Connect().
	To be used only by the Connect() factory function.
	The link is created disabled, you need to call Enable() to start the regular communication. */
	cTCPLinkImpl(const cCallbacksPtr a_LinkCallbacks);

	/** Callback that LibEvent calls when there's data available from the remote peer. */
	static void ReadCallback(bufferevent * a_BufferEvent, void * a_Self);

	/** Callback that LibEvent calls when the remote peer can receive more data. */
	static void WriteCallback(bufferevent * a_BufferEvent, void * a_Self);

	/** Callback that LibEvent calls when there's a non-data-related event on the socket. */
	static void EventCallback(bufferevent * a_BufferEvent, short a_What, void * a_Self);

	/** Sets a_IP and a_Port to values read from a_Address, based on the correct address family. */
	static void UpdateAddress(const sockaddr * a_Address, socklen_t a_AddrLen, AString & a_IP, UInt16 & a_Port);

	/** Updates m_LocalIP and m_LocalPort based on the metadata read from the socket. */
	void UpdateLocalAddress(void);

	/** Updates m_RemoteIP and m_RemotePort based on the metadata read from the socket. */
	void UpdateRemoteAddress(void);

	/** Calls shutdown on the link and disables LibEvent writing.
	Called after all data from LibEvent buffers is sent to the OS TCP stack and shutdown() has been called before. */
	void DoActualShutdown(void);

	/** Sends the data directly to the socket (without the optional TLS). */
	bool SendRaw(const void * a_Data, size_t a_Length);

	/** Called by the TLS when it has decoded a piece of incoming cleartext data from the socket. */
	void ReceivedCleartextData(const char * a_Data, size_t a_Length);
};




