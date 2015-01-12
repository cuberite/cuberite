
// Network.cpp

// Implements the classes used for the Network API

#include "Globals.h"
#include "Network.h"
#include <event2/event.h>
#include <event2/thread.h>
#include <event2/bufferevent.h>
#include <event2/dns.h>
#include <event2/listener.h>
#include <thread>
#include "Event.h"





////////////////////////////////////////////////////////////////////////////////
// Class definitions:

/** Holds information about an in-progress Hostname-to-IP lookup. */
class cHostnameLookup
{
	/** The callbacks to call for resolved names / errors. */
	cNetwork::cResolveNameCallbacksPtr m_Callbacks;

	/** The hostname that was queried (needed for the callbacks). */
	AString m_Hostname;

	static void Callback(int a_ErrCode, struct evutil_addrinfo * a_Addr, void * a_Self);

public:
	cHostnameLookup(const AString & a_Hostname, cNetwork::cResolveNameCallbacksPtr a_Callbacks);
};
typedef SharedPtr<cHostnameLookup> cHostnameLookupPtr;
typedef std::vector<cHostnameLookupPtr> cHostnameLookupPtrs;





/** Holds information about an in-progress IP-to-Hostname lookup. */
class cIPLookup
{
	/** The callbacks to call for resolved names / errors. */
	cNetwork::cResolveNameCallbacksPtr m_Callbacks;

	/** The IP that was queried (needed for the callbacks). */
	AString m_IP;

	static void Callback(int a_Result, char a_Type, int a_Count, int a_Ttl, void * a_Addresses, void * a_Self);

public:
	cIPLookup(const AString & a_IP, cNetwork::cResolveNameCallbacksPtr a_Callbacks);
};
typedef SharedPtr<cIPLookup> cIPLookupPtr;
typedef std::vector<cIPLookupPtr> cIPLookupPtrs;





// fwd:
class cServerHandleImpl;





/** Implements the cTCPLink details so that it can represent the single connection between two endpoints. */
class cTCPLinkImpl:
	public cTCPLink
{
	typedef cTCPLink super;
	friend class cServerHandleImpl;

public:
	/** Creates a new link to be queued to connect to a specified host:port.
	Used for outgoing connections created using cNetwork::Connect().
	Call Connect() first, before using the link. */
	cTCPLinkImpl(const cCallbacksPtr a_LinkCallbacks);

	/** Creates a new link based on the given socket.
	Used for connections accepted in a server using cNetwork::Listen().
	a_Address and a_AddrLen describe the remote peer that has connected. */
	cTCPLinkImpl(evutil_socket_t a_Socket, cCallbacksPtr a_LinkCallbacks, cServerHandleImpl * a_Server, const sockaddr * a_Address, int a_AddrLen);

	/** Queues a connection request to the specified host.
	a_ConnectCallbacks must be valid.
	The object must have been constructed by the right constructor (without the Socket param). */
	bool Connect(const AString & a_Host, UInt16 a_Port, cNetwork::cConnectCallbacksPtr a_ConnectCallbacks);

	// cTCPLink overrides:
	virtual bool Send(const void * a_Data, size_t a_Length) override;
	virtual AString GetLocalIP(void) const override { return m_LocalIP; }
	virtual UInt16 GetLocalPort(void) const override { return m_LocalPort; }
	virtual AString GetRemoteIP(void) const override { return m_RemoteIP; }
	virtual UInt16 GetRemotePort(void) const override { return m_RemotePort; }
	virtual void Close(void) override;
	virtual void Drop(void) override;

protected:

	/** Callbacks to call when the connection is established.
	May be NULL if not used. Only used for outgoing connections (cNetwork::Connect()). */
	cNetwork::cConnectCallbacksPtr m_ConnectCallbacks;

	/** The LibEvent handle representing this connection. */
	bufferevent * m_BufferEvent;

	/** The server handle that has created this link.
	Only valid for incoming connections, NULL for outgoing connections. */
	cServerHandleImpl * m_Server;

	/** The IP address of the local endpoint. Valid only after the socket has been connected. */
	AString m_LocalIP;

	/** The port of the local endpoint. Valid only after the socket has been connected. */
	UInt16 m_LocalPort;

	/** The IP address of the remote endpoint. Valid only after the socket has been connected. */
	AString m_RemoteIP;

	/** The port of the remote endpoint. Valid only after the socket has been connected. */
	UInt16 m_RemotePort;


	/** Callback that LibEvent calls when there's data available from the remote peer. */
	static void ReadCallback(bufferevent * a_BufferEvent, void * a_Self);

	/** Callback that LibEvent calls when there's a non-data-related event on the socket. */
	static void EventCallback(bufferevent * a_BufferEvent, short a_What, void * a_Self);

	/** Sets a_IP and a_Port to values read from a_Address, based on the correct address family. */
	static void UpdateAddress(const sockaddr * a_Address, int a_AddrLen, AString & a_IP, UInt16 & a_Port);

	/** Updates m_LocalIP and m_LocalPort based on the metadata read from the socket. */
	void UpdateLocalAddress(void);

	/** Updates m_RemoteIP and m_RemotePort based on the metadata read from the socket. */
	void UpdateRemoteAddress(void);
};
typedef SharedPtr<cTCPLinkImpl> cTCPLinkImplPtr;
typedef std::vector<cTCPLinkImplPtr> cTCPLinkImplPtrs;





/** Implements the cServerHandle details so that it can represent a real server socket, with a list of clients. */
class cServerHandleImpl:
	public cServerHandle
{
	typedef cServerHandle super;
	friend class cTCPLinkImpl;

public:
	/** Creates a new instance with the specified callbacks.
	Initializes the internals, but doesn't start listening yet. */
	cServerHandleImpl(
		cNetwork::cListenCallbacksPtr a_ListenCallbacks,
		cTCPLink::cCallbacksPtr a_LinkCallbacks
	);

	/** Starts listening on the specified port.
	Both IPv4 and IPv6 interfaces are used, if possible. */
	bool Listen(UInt16 a_Port);

	// cServerHandle overrides:
	virtual void Close(void) override;
	virtual bool IsListening(void) const override { return m_IsListening; }

protected:
	/** The callbacks used to notify about incoming connections. */
	cNetwork::cListenCallbacksPtr m_ListenCallbacks;

	/** The callbacks used to create new cTCPLink instances for incoming connections. */
	cTCPLink::cCallbacksPtr m_LinkCallbacks;

	/** The LibEvent handle representing the main listening socket. */
	evconnlistener * m_ConnListener;

	/** The LibEvent handle representing the secondary listening socket (only when side-by-side listening is needed, such as WinXP). */
	evconnlistener * m_SecondaryConnListener;

	/** Set to true when the server is initialized successfully and is listening for incoming connections. */
	bool m_IsListening;

	/** Container for all currently active connections on this server. */
	cTCPLinkImplPtrs m_Connections;


	/** The callback called by LibEvent upon incoming connection. */
	static void Callback(evconnlistener * a_Listener, evutil_socket_t a_Socket, sockaddr * a_Addr, int a_Len, void * a_Self);

	/** Removes the specified link from m_Connections.
	Called by cTCPLinkImpl when the link is terminated. */
	void RemoveLink(const cTCPLinkImpl * a_Link);
};
typedef SharedPtr<cServerHandleImpl> cServerHandleImplPtr;
typedef std::vector<cServerHandleImplPtr> cServerHandleImplPtrs;





class cNetworkSingleton
{
	friend class cHostnameLookup;    // Needs access to m_DNSBase
	friend class cIPLookup;          // Needs access to m_DNSBase
	friend class cTCPLinkImpl;       // Needs access to m_EventBase and m_DNSBase
	friend class cServerHandleImpl;  // Needs access to m_EventBase

public:
	/** Returns the singleton instance of this class */
	static cNetworkSingleton & Get(void);


	// The following functions are implementations for the cNetwork class

	/** Queues a TCP connection to be made to the specified host.
	Calls one the connection callbacks (success, error) when the connection is successfully established, or upon failure.
	The a_LinkCallbacks is passed to the newly created cTCPLink.
	Returns true if queueing was successful, false on failure to queue.
	Note that the return value doesn't report the success of the actual connection; the connection is established asynchronously in the background. */
	bool Connect(
		const AString & a_Host,
		const UInt16 a_Port,
		cNetwork::cConnectCallbacksPtr a_ConnectCallbacks,
		cTCPLink::cCallbacksPtr a_LinkCallbacks
	);


	/** Opens up the specified port for incoming connections.
	Calls an OnAccepted callback for each incoming connection.
	A cTCPLink with the specified link callbacks is created for each connection.
	Returns a cServerHandle that can be used to query the operation status and close the server. */
	cServerHandlePtr Listen(
		const UInt16 a_Port,
		cNetwork::cListenCallbacksPtr a_ListenCallbacks,
		cTCPLink::cCallbacksPtr a_LinkCallbacks
	);


	/** Queues a DNS query to resolve the specified hostname to IP address.
	Calls one of the callbacks when the resolving succeeds, or when it fails.
	Returns true if queueing was successful, false if not.
	Note that the return value doesn't report the success of the actual lookup; the lookup happens asynchronously on the background. */
	bool HostnameToIP(
		const AString & a_Hostname,
		cNetwork::cResolveNameCallbacksPtr a_Callbacks
	);


	/** Queues a DNS query to resolve the specified IP address to a hostname.
	Calls one of the callbacks when the resolving succeeds, or when it fails.
	Returns true if queueing was successful, false if not.
	Note that the return value doesn't report the success of the actual lookup; the lookup happens asynchronously on the background. */
	bool IPToHostName(
		const AString & a_IP,
		cNetwork::cResolveNameCallbacksPtr a_Callbacks
	);

protected:

	/** The main LibEvent container for driving the event loop. */
	event_base * m_EventBase;

	/** The LibEvent handle for doing DNS lookups. */
	evdns_base * m_DNSBase;

	/** Container for all client connections, including ones with pending-connect. */
	cTCPLinkImplPtrs m_Connections;

	/** Container for all servers that are currently active. */
	cServerHandleImplPtrs m_Servers;

	/** Container for all pending hostname lookups. */
	cHostnameLookupPtrs m_HostnameLookups;

	/** Container for all pending IP lookups. */
	cIPLookupPtrs m_IPLookups;


	/** Initializes the LibEvent internals. */
	cNetworkSingleton(void);

	/** Converts LibEvent-generated log events into log messages in MCS log. */
	static void LogCallback(int a_Severity, const char * a_Msg);

	/** Implements the thread that runs LibEvent's event dispatcher loop. */
	static void RunEventLoop(cNetworkSingleton * a_Self);

	/** Removes the specified hostname lookup from m_HostnameLookups.
	Used by the underlying lookup implementation when the lookup is finished. */
	void RemoveHostnameLookup(const cHostnameLookup * a_HostnameLookup);

	/** Removes the specified IP lookup from m_IPLookups.
	Used by the underlying lookup implementation when the lookup is finished. */
	void RemoveIPLookup(const cIPLookup * a_IPLookup);

	/** Removes the specified link from m_Connections.
	Used by the underlying link implementation when the link is closed / errored. */
	void RemoveLink(const cTCPLinkImpl * a_Link);
};





////////////////////////////////////////////////////////////////////////////////
// Globals:

bool IsValidSocket(evutil_socket_t a_Socket)
{
	#ifdef _WIN32
		return (a_Socket != INVALID_SOCKET);
	#else  // _WIN32
		return (a_Socket >= 0);
	#endif  // else _WIN32
}





////////////////////////////////////////////////////////////////////////////////
// cHostnameLookup:

cHostnameLookup::cHostnameLookup(const AString & a_Hostname, cNetwork::cResolveNameCallbacksPtr a_Callbacks):
	m_Callbacks(a_Callbacks),
	m_Hostname(a_Hostname)
{
	evutil_addrinfo hints;
	memset(&hints, 0, sizeof(hints));
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_family = AF_UNSPEC;
	hints.ai_flags = EVUTIL_AI_CANONNAME;
	evdns_getaddrinfo(cNetworkSingleton::Get().m_DNSBase, a_Hostname.c_str(), nullptr, &hints, Callback, this);
}





void cHostnameLookup::Callback(int a_ErrCode, evutil_addrinfo * a_Addr, void * a_Self)
{
	// Get the Self class:
	cHostnameLookup * Self = reinterpret_cast<cHostnameLookup *>(a_Self);
	ASSERT(Self != nullptr);

	// If an error has occurred, notify the error callback:
	if (a_ErrCode != 0)
	{
		Self->m_Callbacks->OnError(a_ErrCode);
		cNetworkSingleton::Get().RemoveHostnameLookup(Self);
		return;
	}

	// Call the success handler for each entry received:
	bool HasResolved = false;
	evutil_addrinfo * OrigAddr = a_Addr;
	for (;a_Addr != nullptr; a_Addr = a_Addr->ai_next)
	{
		char IP[128];
		switch (a_Addr->ai_family)
		{
			case AF_INET:  // IPv4
			{
				sockaddr_in * sin = reinterpret_cast<sockaddr_in *>(a_Addr->ai_addr);
				evutil_inet_ntop(AF_INET, &(sin->sin_addr), IP, sizeof(IP));
				break;
			}
			case AF_INET6:  // IPv6
			{
				sockaddr_in6 * sin = reinterpret_cast<sockaddr_in6 *>(a_Addr->ai_addr);
				evutil_inet_ntop(AF_INET6, &(sin->sin6_addr), IP, sizeof(IP));
				break;
			}
			default:
			{
				// Unknown address family, handle as if this entry wasn't received
				continue;  // for (a_Addr)
			}
		}
		Self->m_Callbacks->OnNameResolved(Self->m_Hostname, IP);
		HasResolved = true;
	}  // for (a_Addr)

	// If only unsupported families were reported, call the Error handler:
	if (!HasResolved)
	{
		Self->m_Callbacks->OnError(1);
	}
	else
	{
		Self->m_Callbacks->OnFinished();
	}
	evutil_freeaddrinfo(OrigAddr);
	cNetworkSingleton::Get().RemoveHostnameLookup(Self);
}





////////////////////////////////////////////////////////////////////////////////
// cIPLookup:

cIPLookup::cIPLookup(const AString & a_IP, cNetwork::cResolveNameCallbacksPtr a_Callbacks):
	m_Callbacks(a_Callbacks),
	m_IP(a_IP)
{
	sockaddr_storage sa;
	int salen = static_cast<int>(sizeof(sa));
	evutil_parse_sockaddr_port(a_IP.c_str(), reinterpret_cast<sockaddr *>(&sa), &salen);
	switch (sa.ss_family)
	{
		case AF_INET:
		{
			sockaddr_in * sa4 = reinterpret_cast<sockaddr_in *>(&sa);
			evdns_base_resolve_reverse(cNetworkSingleton::Get().m_DNSBase, &(sa4->sin_addr), 0, Callback, this);
			break;
		}
		case AF_INET6:
		{
			sockaddr_in6 * sa6 = reinterpret_cast<sockaddr_in6 *>(&sa);
			evdns_base_resolve_reverse_ipv6(cNetworkSingleton::Get().m_DNSBase, &(sa6->sin6_addr), 0, Callback, this);
			break;
		}
		default:
		{
			LOGWARNING("%s: Unknown address family: %d", __FUNCTION__, sa.ss_family);
			ASSERT(!"Unknown address family");
			break;
		}
	}  // switch (address family)
}





void cIPLookup::Callback(int a_Result, char a_Type, int a_Count, int a_Ttl, void * a_Addresses, void * a_Self)
{
	// Get the Self class:
	cIPLookup * Self = reinterpret_cast<cIPLookup *>(a_Self);
	ASSERT(Self != nullptr);

	if ((a_Result != 0) || (a_Addresses == nullptr))
	{
		// An error has occurred, notify the error callback:
		Self->m_Callbacks->OnError(a_Result);
	}
	else
	{
		// Call the success handler::
		Self->m_Callbacks->OnNameResolved(*(reinterpret_cast<char **>(a_Addresses)), Self->m_IP);
		Self->m_Callbacks->OnFinished();
	}
	cNetworkSingleton::Get().RemoveIPLookup(Self);
}





////////////////////////////////////////////////////////////////////////////////
// cTCPLinkImpl:

cTCPLinkImpl::cTCPLinkImpl(cTCPLink::cCallbacksPtr a_LinkCallbacks):
	super(a_LinkCallbacks),
	m_BufferEvent(bufferevent_socket_new(cNetworkSingleton::Get().m_EventBase, -1, BEV_OPT_CLOSE_ON_FREE)),
	m_Server(nullptr)
{
	bufferevent_setcb(m_BufferEvent, ReadCallback, nullptr, EventCallback, this);
	bufferevent_enable(m_BufferEvent, EV_READ | EV_WRITE);
}





cTCPLinkImpl::cTCPLinkImpl(evutil_socket_t a_Socket, cTCPLink::cCallbacksPtr a_LinkCallbacks, cServerHandleImpl * a_Server, const sockaddr * a_Address, int a_AddrLen):
	super(a_LinkCallbacks),
	m_BufferEvent(bufferevent_socket_new(cNetworkSingleton::Get().m_EventBase, a_Socket, BEV_OPT_CLOSE_ON_FREE)),
	m_Server(a_Server)
{
	UpdateLocalAddress();
	UpdateAddress(a_Address, a_AddrLen, m_RemoteIP, m_RemotePort);
	bufferevent_setcb(m_BufferEvent, ReadCallback, nullptr, EventCallback, this);
	bufferevent_enable(m_BufferEvent, EV_READ | EV_WRITE);
}





/** Schedules the actual connection request.
Returns true on success, false on failure. */
bool cTCPLinkImpl::Connect(const AString & a_Host, UInt16 a_Port, cNetwork::cConnectCallbacksPtr a_ConnectCallbacks)
{
	ASSERT(bufferevent_getfd(m_BufferEvent) == -1);  // Did you create this object using the right constructor (the one without the Socket param)?
	ASSERT(m_Server == nullptr);
	ASSERT(a_ConnectCallbacks != nullptr);

	m_ConnectCallbacks = a_ConnectCallbacks;

	// If a_Host is an IP address, schedule a connection immediately:
	sockaddr_storage sa;
	int salen = static_cast<int>(sizeof(sa));
	if (evutil_parse_sockaddr_port(a_Host.c_str(), reinterpret_cast<sockaddr *>(&sa), &salen) == 0)
	{
		// Insert the correct port:
		if (sa.ss_family == AF_INET6)
		{
			reinterpret_cast<sockaddr_in6 *>(&sa)->sin6_port = htons(a_Port);
		}
		else
		{
			reinterpret_cast<sockaddr_in *>(&sa)->sin_port = htons(a_Port);
		}

		if (bufferevent_socket_connect(m_BufferEvent, reinterpret_cast<sockaddr *>(&sa), salen) == 0)
		{
			// Success
			return true;
		}
		// Failure
		return false;
	}

	// a_Host is a hostname, connect after a lookup:
	if (bufferevent_socket_connect_hostname(m_BufferEvent, cNetworkSingleton::Get().m_DNSBase, AF_UNSPEC, a_Host.c_str(), a_Port) == 0)
	{
		// Success
		return true;
	}
	// Failure
	return false;
}





bool cTCPLinkImpl::Send(const void * a_Data, size_t a_Length)
{
	return (bufferevent_write(m_BufferEvent, a_Data, a_Length) == 0);
}





void cTCPLinkImpl::Close(void)
{
	// TODO
	ASSERT(!"cTCPLinkImpl::Close(): Not implemented yet");
}





void cTCPLinkImpl::Drop(void)
{
	// TODO
	ASSERT(!"cTCPLinkImpl::Drop(): Not implemented yet");
}






void cTCPLinkImpl::ReadCallback(bufferevent * a_BufferEvent, void * a_Self)
{
	ASSERT(a_Self != nullptr);
	cTCPLinkImpl * Self = static_cast<cTCPLinkImpl *>(a_Self);
	ASSERT(Self->m_Callbacks != nullptr);

	// Read all the incoming data, in 1024-byte chunks:
	char data[1024];
	size_t length;
	while ((length = bufferevent_read(a_BufferEvent, data, sizeof(data))) > 0)
	{
		Self->m_Callbacks->OnReceivedData(*Self, data, length);
	}
}





void cTCPLinkImpl::EventCallback(bufferevent * a_BufferEvent, short a_What, void * a_Self)
{
	ASSERT(a_Self != nullptr);
	cTCPLinkImpl * Self = static_cast<cTCPLinkImpl *>(a_Self);

	// If an error is reported, call the error callback:
	if (a_What & BEV_EVENT_ERROR)
	{
		// Choose the proper callback to call based on whether we were waiting for connection or not:
		if (Self->m_ConnectCallbacks != nullptr)
		{
			Self->m_ConnectCallbacks->OnError(EVUTIL_SOCKET_ERROR());
		}
		else
		{
			Self->m_Callbacks->OnError(*Self, EVUTIL_SOCKET_ERROR());
			if (Self->m_Server == nullptr)
			{
				cNetworkSingleton::Get().RemoveLink(Self);
			}
			else
			{
				Self->m_Server->RemoveLink(Self);
			}
		}
		return;
	}

	// Pending connection succeeded, call the connection callback:
	if (a_What & BEV_EVENT_CONNECTED)
	{
		if (Self->m_ConnectCallbacks != nullptr)
		{
			Self->m_ConnectCallbacks->OnSuccess(*Self);
			// Reset the connect callbacks so that later errors get reported through the link callbacks:
			Self->m_ConnectCallbacks.reset();
			return;
		}
		Self->UpdateLocalAddress();
		Self->UpdateRemoteAddress();
	}

	// If the connection has been closed, call the link callback and remove the connection:
	if (a_What & BEV_EVENT_EOF)
	{
		Self->m_Callbacks->OnRemoteClosed(*Self);
		if (Self->m_Server != nullptr)
		{
			Self->m_Server->RemoveLink(Self);
		}
		else
		{
			cNetworkSingleton::Get().RemoveLink(Self);
		}
		return;
	}
	
	// Unknown event, report it:
	LOGWARNING("cTCPLinkImpl: Unhandled LibEvent event %d (0x%x)", a_What, a_What);
	ASSERT(!"cTCPLinkImpl: Unhandled LibEvent event");
}





void cTCPLinkImpl::UpdateAddress(const sockaddr * a_Address, int a_AddrLen, AString & a_IP, UInt16 & a_Port)
{
	char IP[128];
	switch (a_Address->sa_family)
	{
		case AF_INET:  // IPv4:
		{
			const sockaddr_in * sin = reinterpret_cast<const sockaddr_in *>(a_Address);
			evutil_inet_ntop(AF_INET, &(sin->sin_addr), IP, sizeof(IP));
			a_Port = ntohs(sin->sin_port);
			break;
		}
		case AF_INET6:  // IPv6
		{
			const sockaddr_in6 * sin = reinterpret_cast<const sockaddr_in6 *>(a_Address);
			evutil_inet_ntop(AF_INET6, &(sin->sin6_addr), IP, sizeof(IP));
			a_Port = ntohs(sin->sin6_port);
			break;
		}

		default:
		{
			LOGWARNING("%s: Unknown socket address family: %d", __FUNCTION__, a_Address->sa_family);
			ASSERT(!"Unknown socket address family");
			break;
		}
	}
	a_IP.assign(IP);
}





void cTCPLinkImpl::UpdateLocalAddress(void)
{
	sockaddr_storage sa;
	socklen_t salen = static_cast<socklen_t>(sizeof(sa));
	getsockname(bufferevent_getfd(m_BufferEvent), reinterpret_cast<sockaddr *>(&sa), &salen);
	UpdateAddress(reinterpret_cast<const sockaddr *>(&sa), salen, m_LocalIP, m_LocalPort);
}





void cTCPLinkImpl::UpdateRemoteAddress(void)
{
	sockaddr_storage sa;
	socklen_t salen = static_cast<socklen_t>(sizeof(sa));
	getpeername(bufferevent_getfd(m_BufferEvent), reinterpret_cast<sockaddr *>(&sa), &salen);
	UpdateAddress(reinterpret_cast<const sockaddr *>(&sa), salen, m_LocalIP, m_LocalPort);
}





////////////////////////////////////////////////////////////////////////////////
// cServerHandleImpl:

cServerHandleImpl::cServerHandleImpl(cNetwork::cListenCallbacksPtr a_ListenCallbacks, cTCPLink::cCallbacksPtr a_LinkCallbacks):
	m_ListenCallbacks(a_ListenCallbacks),
	m_LinkCallbacks(a_LinkCallbacks),
	m_ConnListener(nullptr),
	m_SecondaryConnListener(nullptr),
	m_IsListening(false)
{
}





void cServerHandleImpl::Close(void)
{
	// Stop the listener sockets:
	evconnlistener_free(m_ConnListener);
	m_ConnListener = nullptr;
	if (m_SecondaryConnListener != nullptr)
	{
		evconnlistener_free(m_SecondaryConnListener);
		m_SecondaryConnListener = nullptr;
	}
	m_IsListening = false;

	// Close all connections:
	cTCPLinkImplPtrs Conns;
	std::swap(Conns, m_Connections);
	for (auto conn: Conns)
	{
		conn->Close();
	}
}





bool cServerHandleImpl::Listen(UInt16 a_Port)
{
	ASSERT(!m_IsListening);

	// Set up the main socket:
	// It should listen on IPv6 with IPv4 fallback, when available; IPv4 when IPv6 is not available.
	bool NeedsTwoSockets = false;
	evutil_socket_t MainSock = socket(AF_INET6, SOCK_STREAM, IPPROTO_TCP);
	if (!IsValidSocket(MainSock))
	{
		// Failed to create IPv6 socket, create an IPv4 one instead:
		MainSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (!IsValidSocket(MainSock))
		{
			int err = EVUTIL_SOCKET_ERROR();
			LOGWARNING("%s: Cannot create a socket for neither IPv6 nor IPv4: %d (%s)", __FUNCTION__, err, evutil_socket_error_to_string(err));
			return false;
		}

		// Bind to all interfaces:
		sockaddr_in name;
		memset(&name, 0, sizeof(name));
		name.sin_family = AF_INET;
		name.sin_port = ntohs(a_Port);
		if (bind(MainSock, reinterpret_cast<const sockaddr *>(&name), sizeof(name)) != 0)
		{
			int err = EVUTIL_SOCKET_ERROR();
			LOGWARNING("Cannot bind to IPv4 port %d: %d (%s)", a_Port, err, evutil_socket_error_to_string(err));
			evutil_closesocket(MainSock);
			return false;
		}
	}
	else
	{
		// IPv6 socket created, switch it into "dualstack" mode:
		UInt32 Zero = 0;
		#ifdef _WIN32
			// WinXP doesn't support this feature, so if the setting fails, create another socket later on:
			NeedsTwoSockets = (
				(setsockopt(MainSock, IPPROTO_IPV6, IPV6_V6ONLY, reinterpret_cast<const char *>(&Zero), sizeof(Zero)) == SOCKET_ERROR) &&
				(EVUTIL_SOCKET_ERROR() == WSAENOPROTOOPT)
			);
		#else
			setsockopt(MainSock, IPPROTO_IPV6, IPV6_V6ONLY, reinterpret_cast<const char *>(&Zero), sizeof(Zero));
		#endif

		// Bind to all interfaces:
		sockaddr_in6 name;
		memset(&name, 0, sizeof(name));
		name.sin6_family = AF_INET6;
		name.sin6_port = ntohs(a_Port);
		if (bind(MainSock, reinterpret_cast<const sockaddr *>(&name), sizeof(name)) != 0)
		{
			int err = EVUTIL_SOCKET_ERROR();
			LOGWARNING("Cannot bind to IPv6 port %d: %d (%s)", a_Port, err, evutil_socket_error_to_string(err));
			evutil_closesocket(MainSock);
			return false;
		}
	}
	if (evutil_make_socket_nonblocking(MainSock) != 0)
	{
		int err = EVUTIL_SOCKET_ERROR();
		LOGWARNING("Cannot make socket for port %d non-blocking: %d (%s)", a_Port, err, evutil_socket_error_to_string(err));
		evutil_closesocket(MainSock);
		return false;
	}
	if (listen(MainSock, 0) != 0)
	{
		int err = EVUTIL_SOCKET_ERROR();
		LOGWARNING("Cannot listen on port %d: %d (%s)", a_Port, err, evutil_socket_error_to_string(err));
		evutil_closesocket(MainSock);
		return false;
	}
	m_ConnListener = evconnlistener_new(cNetworkSingleton::Get().m_EventBase, Callback, this, LEV_OPT_CLOSE_ON_FREE | LEV_OPT_REUSEABLE, 0, MainSock);

	// If a secondary socket is required (WinXP dual-stack), create it here:
	if (NeedsTwoSockets)
	{
		evutil_socket_t SecondSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (!IsValidSocket(SecondSock))
		{
			evutil_make_socket_nonblocking(SecondSock);
			m_SecondaryConnListener = evconnlistener_new(cNetworkSingleton::Get().m_EventBase, Callback, this, LEV_OPT_CLOSE_ON_FREE | LEV_OPT_REUSEABLE, 0, SecondSock);
		}
	}
	m_IsListening = true;
	return true;
}





void cServerHandleImpl::Callback(evconnlistener * a_Listener, evutil_socket_t a_Socket, sockaddr * a_Addr, int a_Len, void * a_Self)
{
	// Cast to true self:
	cServerHandleImpl * Self = reinterpret_cast<cServerHandleImpl *>(a_Self);
	ASSERT(Self != nullptr);

	// Create a new cTCPLink for the incoming connection:
	cTCPLinkImplPtr Link = std::make_shared<cTCPLinkImpl>(a_Socket, Self->m_LinkCallbacks, Self, a_Addr, a_Len);
	Self->m_Connections.push_back(Link);

	// Call the OnAccepted callback:
	Self->m_ListenCallbacks->OnAccepted(*Link);
}





void cServerHandleImpl::RemoveLink(const cTCPLinkImpl * a_Link)
{
	for (auto itr = m_Connections.begin(), end = m_Connections.end(); itr != end; ++itr)
	{
		if (itr->get() == a_Link)
		{
			m_Connections.erase(itr);
			return;
		}
	}  // for itr - m_Connections[]
}





////////////////////////////////////////////////////////////////////////////////
// cNetwork:

bool cNetwork::Connect(
	const AString & a_Host,
	const UInt16 a_Port,
	cNetwork::cConnectCallbacksPtr a_ConnectCallbacks,
	cTCPLink::cCallbacksPtr a_LinkCallbacks
)
{
	return cNetworkSingleton::Get().Connect(a_Host, a_Port, a_ConnectCallbacks, a_LinkCallbacks);
}





cServerHandlePtr cNetwork::Listen(
	const UInt16 a_Port,
	cNetwork::cListenCallbacksPtr a_ListenCallbacks,
	cTCPLink::cCallbacksPtr a_LinkCallbacks
)
{
	return cNetworkSingleton::Get().Listen(a_Port, a_ListenCallbacks, a_LinkCallbacks);
}





bool cNetwork::HostnameToIP(
	const AString & a_Hostname,
	cNetwork::cResolveNameCallbacksPtr a_Callbacks
)
{
	return cNetworkSingleton::Get().HostnameToIP(a_Hostname, a_Callbacks);
}





bool cNetwork::IPToHostName(
	const AString & a_IP,
	cNetwork::cResolveNameCallbacksPtr a_Callbacks
)
{
	return cNetworkSingleton::Get().IPToHostName(a_IP, a_Callbacks);
}




////////////////////////////////////////////////////////////////////////////////
// cTCPLink:

cTCPLink::cTCPLink(cCallbacksPtr a_Callbacks):
	m_Callbacks(a_Callbacks)
{
}




////////////////////////////////////////////////////////////////////////////////
// cNetworkSingleton:

cNetworkSingleton::cNetworkSingleton(void)
{
	// Windows: initialize networking:
	#ifdef _WIN32
		WSADATA wsaData;
		memset(&wsaData, 0, sizeof(wsaData));
		WSAStartup (MAKEWORD(2, 2), &wsaData);
	#endif  // _WIN32

	// Initialize LibEvent logging:
	event_set_log_callback(LogCallback);

	// Initialize threading:
	#if defined(EVTHREAD_USE_WINDOWS_THREADS_IMPLEMENTED)
		evthread_use_windows_threads();
	#elif defined(EVTHREAD_USE_PTHREADS_IMPLEMENTED)
		evthread_use_pthreads();
	#else
		#error No threading implemented for EVTHREAD
	#endif

	// Create the main event_base:
	m_EventBase = event_base_new();
	if (m_EventBase == nullptr)
	{
		LOGERROR("Failed to initialize LibEvent. The server will now terminate.");
		abort();
	}

	// Create the DNS lookup helper:
	m_DNSBase = evdns_base_new(m_EventBase, 1);
	if (m_DNSBase == nullptr)
	{
		LOGERROR("Failed to initialize LibEvent's DNS subsystem. The server will now terminate.");
		abort();
	}

	// Create the event loop thread:
	std::thread EventLoopThread(RunEventLoop, this);
	EventLoopThread.detach();
}





cNetworkSingleton & cNetworkSingleton::Get(void)
{
	static cNetworkSingleton Instance;
	return Instance;
}





bool cNetworkSingleton::Connect(
	const AString & a_Host,
	const UInt16 a_Port,
	cNetwork::cConnectCallbacksPtr a_ConnectCallbacks,
	cTCPLink::cCallbacksPtr a_LinkCallbacks
)
{
	// Add a connection request to the queue:
	cTCPLinkImplPtr ConnRequest = std::make_shared<cTCPLinkImpl>(a_LinkCallbacks);
	m_Connections.push_back(ConnRequest);

	// Queue the connection:
	if (!ConnRequest->Connect(a_Host, a_Port, a_ConnectCallbacks))
	{
		RemoveLink(ConnRequest.get());
		return false;
	}

	// Everything successful, return success:
	return true;
}





cServerHandlePtr cNetworkSingleton::Listen(
	const UInt16 a_Port,
	cNetwork::cListenCallbacksPtr a_ListenCallbacks,
	cTCPLink::cCallbacksPtr a_LinkCallbacks
)
{
	cServerHandleImplPtr res = std::make_shared<cServerHandleImpl>(a_ListenCallbacks, a_LinkCallbacks);
	if (!res->Listen(a_Port))
	{
		return res;
	}
	m_Servers.push_back(res);
	return res;
}





bool cNetworkSingleton::HostnameToIP(
	const AString & a_Hostname,
	cNetwork::cResolveNameCallbacksPtr a_Callbacks
)
{
	try
	{
		m_HostnameLookups.push_back(std::make_shared<cHostnameLookup>(a_Hostname, a_Callbacks));
	}
	catch (...)
	{
		return false;
	}
	return true;
}




bool cNetworkSingleton::IPToHostName(
	const AString & a_IP,
	cNetwork::cResolveNameCallbacksPtr a_Callbacks
)
{
	try
	{
		m_IPLookups.push_back(std::make_shared<cIPLookup>(a_IP, a_Callbacks));
	}
	catch (...)
	{
		return false;
	}
	return true;
}




void cNetworkSingleton::LogCallback(int a_Severity, const char * a_Msg)
{
	switch (a_Severity)
	{
		case _EVENT_LOG_DEBUG: LOGD      ("LibEvent: %s", a_Msg); break;
		case _EVENT_LOG_MSG:   LOG       ("LibEvent: %s", a_Msg); break;
		case _EVENT_LOG_WARN:  LOGWARNING("LibEvent: %s", a_Msg); break;
		case _EVENT_LOG_ERR:   LOGERROR  ("LibEvent: %s", a_Msg); break;
		default:
		{
			printf("LibEvent: %s", a_Msg);
			break;
		}
	}
}





void cNetworkSingleton::RunEventLoop(cNetworkSingleton * a_Self)
{
	event_base_loop(a_Self->m_EventBase, EVLOOP_NO_EXIT_ON_EMPTY);
}





void cNetworkSingleton::RemoveHostnameLookup(const cHostnameLookup * a_HostnameLookup)
{
	for (auto itr = m_HostnameLookups.begin(), end = m_HostnameLookups.end(); itr != end; ++itr)
	{
		if (itr->get() == a_HostnameLookup)
		{
			m_HostnameLookups.erase(itr);
			return;
		}
	}  // for itr - m_HostnameLookups[]
}





void cNetworkSingleton::RemoveIPLookup(const cIPLookup * a_IPLookup)
{
	for (auto itr = m_IPLookups.begin(), end = m_IPLookups.end(); itr != end; ++itr)
	{
		if (itr->get() == a_IPLookup)
		{
			m_IPLookups.erase(itr);
			return;
		}
	}  // for itr - m_IPLookups[]
}





void cNetworkSingleton::RemoveLink(const cTCPLinkImpl * a_Link)
{
	for (auto itr = m_Connections.begin(), end = m_Connections.end(); itr != end; ++itr)
	{
		if (itr->get() == a_Link)
		{
			m_Connections.erase(itr);
			return;
		}
	}  // for itr - m_Connections[]
}




