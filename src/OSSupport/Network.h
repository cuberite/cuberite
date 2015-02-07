
// Network.h

// Declares the classes used for the Network API





#pragma once





// fwd:
class cTCPLink;
typedef SharedPtr<cTCPLink> cTCPLinkPtr;
typedef std::vector<cTCPLinkPtr> cTCPLinkPtrs;
class cServerHandle;
typedef SharedPtr<cServerHandle> cServerHandlePtr;
typedef std::vector<cServerHandlePtr> cServerHandlePtrs;





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

		/** Called when an error is detected on the connection. */
		virtual void OnError(int a_ErrorCode, const AString & a_ErrorMsg) = 0;
	};
	typedef SharedPtr<cCallbacks> cCallbacksPtr;


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

	/** Returns the callbacks that are used. */
	cCallbacksPtr GetCallbacks(void) const { return m_Callbacks; }

protected:
	/** Callbacks to be used for the various situations. */
	cCallbacksPtr m_Callbacks;


	/** Creates a new link, with the specified callbacks. */
	cTCPLink(cCallbacksPtr a_Callbacks):
		m_Callbacks(a_Callbacks)
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
	typedef SharedPtr<cConnectCallbacks> cConnectCallbacksPtr;


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
	typedef SharedPtr<cListenCallbacks> cListenCallbacksPtr;


	/** Callbacks used when resolving names to IPs. */
	class cResolveNameCallbacks
	{
	public:
		// Force a virtual destructor for all descendants:
		virtual ~cResolveNameCallbacks() {}

		/** Called when the hostname is successfully resolved into an IP address.
		May be called multiple times if a name resolves to multiple addresses.
		a_IP may be either an IPv4 or an IPv6 address with their proper formatting. */
		virtual void OnNameResolved(const AString & a_Name, const AString & a_IP) = 0;

		/** Called when an error is encountered while resolving.
		If an error is reported, the OnFinished() callback is not called. */
		virtual void OnError(int a_ErrorCode, const AString & a_ErrorMsg) = 0;

		/** Called when all the addresses resolved have been reported via the OnNameResolved() callback.
		Only called if there was no error reported. */
		virtual void OnFinished(void) = 0;
	};
	typedef SharedPtr<cResolveNameCallbacks> cResolveNameCallbacksPtr;


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
};




