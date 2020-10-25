
// Network.h

// Declares the classes used for the Network API





#pragma once





#ifdef __FreeBSD__
	#include <netinet/in.h>
#endif





// fwd:
class cTCPLink;
typedef std::shared_ptr<cTCPLink> cTCPLinkPtr;
typedef std::vector<cTCPLinkPtr> cTCPLinkPtrs;
class cServerHandle;
typedef std::shared_ptr<cServerHandle> cServerHandlePtr;
typedef std::vector<cServerHandlePtr> cServerHandlePtrs;
class cCryptoKey;
typedef std::shared_ptr<cCryptoKey> cCryptoKeyPtr;
class cX509Cert;
typedef std::shared_ptr<cX509Cert> cX509CertPtr;





/** Interface that provides the methods available on a single TCP connection. */
class cTCPLink
{
	friend class cNetwork;

public:
	class cCallbacks
	{
	public:
		// Force a virtual destructor for all descendants:
		virtual ~cCallbacks() {}

		/** Called when the cTCPLink for the connection is created.
		The callback may store the cTCPLink instance for later use, but it should remove it in OnError(), OnRemoteClosed() or right after Close(). */
		virtual void OnLinkCreated(cTCPLinkPtr a_Link) = 0;

		/** Called when there's data incoming from the remote peer. */
		virtual void OnReceivedData(const char * a_Data, size_t a_Length) = 0;

		/** Called when the remote end closes the connection.
		The link is still available for connection information query (IP / port).
		Sending data on the link is not an error, but the data won't be delivered. */
		virtual void OnRemoteClosed(void) = 0;

		/** Called when the TLS handshake has been completed and communication can continue regularly.
		Has an empty default implementation, so that link callback descendants don't need to specify TLS handlers when they don't use TLS at all. */
		virtual void OnTlsHandshakeCompleted(void) {}

		/** Called when an error is detected on the connection. */
		virtual void OnError(int a_ErrorCode, const AString & a_ErrorMsg) = 0;
	};
	typedef std::shared_ptr<cCallbacks> cCallbacksPtr;


	// Force a virtual destructor for all descendants:
	virtual ~cTCPLink() {}

	/** Queues the specified data for sending to the remote peer.
	Returns true on success, false on failure. Note that this success or failure only reports the queue status, not the actual data delivery. */
	virtual bool Send(const void * a_Data, size_t a_Length) = 0;

	/** Queues the specified data for sending to the remote peer.
	Returns true on success, false on failure. Note that this success or failure only reports the queue status, not the actual data delivery. */
	bool Send(const AString & a_Data)
	{
		return Send(a_Data.data(), a_Data.size());
	}

	/** Returns the IP address of the local endpoint of the connection. */
	virtual AString GetLocalIP(void) const = 0;

	/** Returns the port used by the local endpoint of the connection. */
	virtual UInt16 GetLocalPort(void) const = 0;

	/** Returns the IP address of the remote endpoint of the connection. */
	virtual AString GetRemoteIP(void) const = 0;

	/** Returns the port used by the remote endpoint of the connection. */
	virtual UInt16 GetRemotePort(void) const = 0;

	/** Closes the link gracefully.
	The link will send any queued outgoing data, then it will send the FIN packet.
	The link will still receive incoming data from remote until the remote closes the connection. */
	virtual void Shutdown(void) = 0;

	/** Drops the connection without any more processing.
	Sends the RST packet, queued outgoing and incoming data is lost. */
	virtual void Close(void) = 0;

	/** Starts a TLS handshake as a client connection.
	If a client certificate should be used for the connection, set the certificate into a_OwnCertData and
	its corresponding private key to a_OwnPrivKeyData. If both are empty, no client cert is presented.
	a_OwnPrivKeyPassword is the password to be used for decoding PrivKey, empty if not passworded.
	Returns empty string on success, non-empty error description on failure. */
	virtual AString StartTLSClient(
		cX509CertPtr a_OwnCert,
		cCryptoKeyPtr a_OwnPrivKey
	) = 0;

	/** Starts a TLS handshake as a server connection.
	Set the server certificate into a_CertData and its corresponding private key to a_OwnPrivKeyData.
	a_OwnPrivKeyPassword is the password to be used for decoding PrivKey, empty if not passworded.
	a_StartTLSData is any data that should be pushed into the TLS before reading more data from the remote.
	This is used mainly for protocols starting TLS in the middle of communication, when the TLS start command
	can be received together with the TLS Client Hello message in one OnReceivedData() call, to re-queue the
	Client Hello message into the TLS handshake buffer.
	Returns empty string on success, non-empty error description on failure. */
	virtual AString StartTLSServer(
		cX509CertPtr a_OwnCert,
		cCryptoKeyPtr a_OwnPrivKey,
		const AString & a_StartTLSData
	) = 0;

	/** Returns the callbacks that are used. */
	cCallbacksPtr GetCallbacks(void) const { return m_Callbacks; }

protected:
	/** Callbacks to be used for the various situations. */
	cCallbacksPtr m_Callbacks;


	/** Creates a new link, with the specified callbacks. */
	cTCPLink(cCallbacksPtr a_Callbacks):
		m_Callbacks(std::move(a_Callbacks))
	{
	}
};





/** Interface that provides the methods available on a listening server socket. */
class cServerHandle
{
	friend class cNetwork;
public:

	// Force a virtual destructor for all descendants:
	virtual ~cServerHandle() {}

	/** Stops the server, no more incoming connections will be accepted.
	All current connections will be shut down (cTCPLink::Shutdown()). */
	virtual void Close(void) = 0;

	/** Returns true if the server has been started correctly and is currently listening for incoming connections. */
	virtual bool IsListening(void) const = 0;
};





/** Interface that provides methods available on UDP communication endpoints. */
class cUDPEndpoint
{
public:
	/** Interface for the callbacks for events that can happen on the endpoint. */
	class cCallbacks
	{
	public:
		// Force a virtual destructor in all descendants:
		virtual ~cCallbacks() {}

		/** Called when an error occurs on the endpoint. */
		virtual void OnError(int a_ErrorCode, const AString & a_ErrorMsg) = 0;

		/** Called when there is an incoming datagram from a remote host. */
		virtual void OnReceivedData(const char * a_Data, size_t a_Size, const AString & a_RemoteHost, UInt16 a_RemotePort) = 0;
	};


	// Force a virtual destructor for all descendants:
	virtual ~cUDPEndpoint() {}

	/** Closes the underlying socket.
	Note that there still might be callbacks in-flight after this method returns. */
	virtual void Close(void) = 0;

	/** Returns true if the endpoint is open. */
	virtual bool IsOpen(void) const = 0;

	/** Returns the local port to which the underlying socket is bound. */
	virtual UInt16 GetPort(void) const = 0;

	/** Sends the specified payload in a single UDP datagram to the specified host + port combination.
	Note that in order to send to a broadcast address, you need to call EnableBroadcasts() first. */
	virtual bool Send(const AString & a_Payload, const AString & a_Host, UInt16 a_Port) = 0;

	/** Marks the socket as capable of sending broadcast, using whatever OS API is needed.
	Without this call, sending to a broadcast address using Send() may fail. */
	virtual void EnableBroadcasts(void) = 0;

protected:
	/** The callbacks used for various events on the endpoint. */
	cCallbacks & m_Callbacks;


	/** Creates a new instance of an endpoint, with the specified callbacks. */
	cUDPEndpoint(cCallbacks & a_Callbacks):
		m_Callbacks(a_Callbacks)
	{
	}
};

typedef std::shared_ptr<cUDPEndpoint> cUDPEndpointPtr;





class cNetwork
{
public:
	/** Callbacks used for connecting to other servers as a client. */
	class cConnectCallbacks
	{
	public:
		// Force a virtual destructor for all descendants:
		virtual ~cConnectCallbacks() {}

		/** Called when the Connect call succeeds.
		Provides the newly created link that can be used for communication. */
		virtual void OnConnected(cTCPLink & a_Link) = 0;

		/** Called when the Connect call fails. */
		virtual void OnError(int a_ErrorCode, const AString & a_ErrorMsg) = 0;
	};
	typedef std::shared_ptr<cConnectCallbacks> cConnectCallbacksPtr;


	/** Callbacks used when listening for incoming connections as a server. */
	class cListenCallbacks
	{
	public:
		// Force a virtual destructor for all descendants:
		virtual ~cListenCallbacks() {}

		/** Called when the TCP server created with Listen() receives a new incoming connection.
		Returns the link callbacks that the server should use for the newly created link.
		If a nullptr is returned, the connection is dropped immediately;
		otherwise a new cTCPLink instance is created and OnAccepted() is called. */
		virtual cTCPLink::cCallbacksPtr OnIncomingConnection(const AString & a_RemoteIPAddress, UInt16 a_RemotePort) = 0;

		/** Called when the TCP server created with Listen() creates a new link for an incoming connection.
		Provides the newly created Link that can be used for communication.
		Called right after a successful OnIncomingConnection(). */
		virtual void OnAccepted(cTCPLink & a_Link) = 0;

		/** Called when the socket fails to listen on the specified port. */
		virtual void OnError(int a_ErrorCode, const AString & a_ErrorMsg) = 0;
	};
	typedef std::shared_ptr<cListenCallbacks> cListenCallbacksPtr;


	/** Callbacks used when resolving names to IPs. */
	class cResolveNameCallbacks
	{
	public:
		// Force a virtual destructor for all descendants:
		virtual ~cResolveNameCallbacks() {}

		/** Called when the hostname is successfully resolved into an IP address.
		May be called multiple times if a name resolves to multiple addresses.
		a_IP may be either an IPv4 or an IPv6 address with their proper formatting.
		Each call to OnNameResolved() is preceded by a call to either OnNameResolvedV4() or OnNameResolvedV6(). */
		virtual void OnNameResolved(const AString & a_Name, const AString & a_IP) = 0;

		/** Called when the hostname is successfully resolved into an IPv4 address.
		May be called multiple times if a name resolves to multiple addresses.
		Each call to OnNameResolvedV4 is followed by OnNameResolved with the IP address serialized to a string.
		If this callback returns false, the OnNameResolved() call is skipped for this address. */
		virtual bool OnNameResolvedV4(const AString & a_Name, const sockaddr_in * a_IP) { return true; }

		/** Called when the hostname is successfully resolved into an IPv6 address.
		May be called multiple times if a name resolves to multiple addresses.
		Each call to OnNameResolvedV4 is followed by OnNameResolved with the IP address serialized to a string.
		If this callback returns false, the OnNameResolved() call is skipped for this address. */
		virtual bool OnNameResolvedV6(const AString & a_Name, const sockaddr_in6 * a_IP) { return true; }

		/** Called when an error is encountered while resolving.
		If an error is reported, the OnFinished() callback is not called. */
		virtual void OnError(int a_ErrorCode, const AString & a_ErrorMsg) = 0;

		/** Called when all the addresses resolved have been reported via the OnNameResolved() callback.
		Only called if there was no error reported. */
		virtual void OnFinished(void) = 0;
	};
	typedef std::shared_ptr<cResolveNameCallbacks> cResolveNameCallbacksPtr;


	/** Queues a TCP connection to be made to the specified host.
	Calls one the connection callbacks (success, error) when the connection is successfully established, or upon failure.
	The a_LinkCallbacks is passed to the newly created cTCPLink.
	Returns true if queueing was successful, false on failure to queue.
	Note that the return value doesn't report the success of the actual connection; the connection is established asynchronously in the background.
	Implemented in TCPLinkImpl.cpp. */
	static bool Connect(
		const AString & a_Host,
		UInt16 a_Port,
		cConnectCallbacksPtr a_ConnectCallbacks,
		cTCPLink::cCallbacksPtr a_LinkCallbacks
	);


	/** Opens up the specified port for incoming connections.
	Calls an OnAccepted callback for each incoming connection.
	A cTCPLink with the specified link callbacks is created for each connection.
	Returns a cServerHandle that can be used to query the operation status and close the server.
	Implemented in ServerHandleImpl.cpp. */
	static cServerHandlePtr Listen(
		UInt16 a_Port,
		cListenCallbacksPtr a_ListenCallbacks
	);


	/** Queues a DNS query to resolve the specified hostname to IP address.
	Calls one of the callbacks when the resolving succeeds, or when it fails.
	Returns true if queueing was successful, false if not.
	Note that the return value doesn't report the success of the actual lookup; the lookup happens asynchronously on the background.
	Implemented in HostnameLookup.cpp. */
	static bool HostnameToIP(
		const AString & a_Hostname,
		cResolveNameCallbacksPtr a_Callbacks
	);


	/** Queues a DNS query to resolve the specified IP address to a hostname.
	Calls one of the callbacks when the resolving succeeds, or when it fails.
	Returns true if queueing was successful, false if not.
	Note that the return value doesn't report the success of the actual lookup; the lookup happens asynchronously on the background.
	Implemented in IPLookup.cpp. */
	static bool IPToHostName(
		const AString & a_IP,
		cResolveNameCallbacksPtr a_Callbacks
	);

	/** Opens up an UDP endpoint for sending and receiving UDP datagrams on the specified port.
	If a_Port is 0, the OS is free to assign any port number it likes to the endpoint.
	Returns the endpoint object that can be interacted with. */
	static cUDPEndpointPtr CreateUDPEndpoint(UInt16 a_Port, cUDPEndpoint::cCallbacks & a_Callbacks);

	/** Returns all local IP addresses for network interfaces currently available. */
	static AStringVector EnumLocalIPAddresses(void);
};



