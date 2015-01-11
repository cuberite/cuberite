
// Network.cpp

// Implements the classes used for the Network API

#include "Globals.h"
#include "Network.h"
#include "Event.h"
#include <event2/event.h>
#include <event2/thread.h>
#include <event2/bufferevent.h>
#include <event2/dns.h>

#include <thread>



////////////////////////////////////////////////////////////////////////////////
// Self-test:

class cNetworkTest
{
public:


	/** cTCPLink callbacks that echo everything they receive back to the remote peer. */
	class cEchoCallbacks:
		public cTCPLink::cCallbacks
	{
		cEvent & m_Event;

		virtual void OnReceivedData(cTCPLink & a_Link, const char * a_Data, size_t a_Size) override
		{
			// Echo the incoming data back to outgoing data:
			LOGD("Data received (%u bytes), echoing back.", static_cast<unsigned>(a_Size));
			a_Link.Send(a_Data, a_Size);
			LOGD("Echo queued");
		}

		virtual void OnRemoteClosed(cTCPLink & a_Link) override
		{
			LOGD("Remote has closed the connection.");
			m_Event.Set();
		}

		virtual void OnError(cTCPLink & a_Link, int a_ErrorCode) override
		{
			LOGD("Error in the cEchoCallbacks.");
			m_Event.Set();
		}

	public:
		cEchoCallbacks(cEvent & a_Event):
			m_Event(a_Event)
		{
		}
	};


	/** Connect callbacks that send a simple test message when connected. */
	class cConnectCallbacks:
		public cNetwork::cConnectCallbacks
	{
		cEvent & m_Event;

		virtual void OnSuccess(cTCPLink & a_Link) override
		{
			LOGD("Connected, sending test message");
			a_Link.Send("test message");
			LOGD("Message queued.");
		}

		virtual void OnError(int a_ErrorCode) override
		{
			LOGD("Error while connecting: %d", a_ErrorCode);
			m_Event.Set();
		}

	public:
		cConnectCallbacks(cEvent & a_Event):
			m_Event(a_Event)
		{
		}
	};

	/** Listen callbacks that send a simple welcome message to all connecting peers. */
	class cListenCallbacks:
		public cNetwork::cListenCallbacks
	{
		virtual void OnAccepted(cTCPLink & a_Link) override
		{
			// Send some trivial data:
			a_Link.Send("Welcome to echo server\n");
		}
	};

	cNetworkTest(void)
	{

		/*
		LOGD("Creating a server on port 33033");
		auto Server = cNetwork::Listen(33033, std::make_shared<cListenCallbacks>(), std::make_shared<cEchoCallbacks>());
		LOGD("Test server created.");

		LOGD("Connecting to test server");
		cNetwork::Connect("localhost", 33033, std::make_shared<cConnectCallbacks>(), std::make_shared<cDumpCallbacks>());

		LOGD("Waiting for network operations to finish.");
		evtFinish.Wait();

		LOGD("Terminating test server.");
		Server->Close();
		*/


	}
} g_NetworkTest;





////////////////////////////////////////////////////////////////////////////////
// Class definitions:

/** Holds information about an in-progress hostname lookup. */
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





/** Implements the cTCPLink details so that it can represent the single connection between two endpoints. */
class cTCPLinkImpl:
	public cTCPLink
{
	typedef cTCPLink super;
public:
	/** Creates a new link to be queued to connect to a specified host:port.
	Used for outgoing connections created using cNetwork::Connect().
	Call Connect() first, before using the link. */
	cTCPLinkImpl(const cCallbacksPtr a_LinkCallbacks);

	/** Creates a new link based on the given socket.
	Used for connections accepted in a server using cNetwork::Listen(). */
	cTCPLinkImpl(evutil_socket_t a_Socket, cCallbacksPtr a_LinkCallbacks);

	/** Queues a connection request to the specified host.
	a_ConnectCallbacks must be valid.
	The object must have been constructed by the right constructor (without the Socket param). */
	bool Connect(const AString & a_Host, UInt16 a_Port, cNetwork::cConnectCallbacksPtr a_ConnectCallbacks);

	// cTCPLink overrides:
	virtual bool Send(const void * a_Data, size_t a_Length) override;
	virtual AString GetLocalIP(void) const override;
	virtual UInt16 GetLocalPort(void) const override;
	virtual AString GetRemoteIP(void) const override;
	virtual UInt16 GetRemotePort(void) const override;
	virtual void Close(void) override;
	virtual void Drop(void) override;

protected:

	/** Callbacks to call when the connection is established.
	May be NULL if not used. Only used for outgoing connections (cNetwork::Connect()). */
	cNetwork::cConnectCallbacksPtr m_ConnectCallbacks;

	/** The LibEvent handle representing this connection. */
	bufferevent * m_BufferEvent;


	/** Callback that LibEvent calls when there's data available from the remote peer. */
	static void ReadCallback(bufferevent * a_BufferEvent, void * a_Self);

	/** Callback that LibEvent calls when there's a non-data-related event on the socket. */
	static void EventCallback(bufferevent * a_BufferEvent, short a_What, void * a_Self);
};
typedef SharedPtr<cTCPLinkImpl> cTCPLinkImplPtr;
typedef std::vector<cTCPLinkImplPtr> cTCPLinkImplPtrs;





/** Implements the cServerHandle details so that it can represent a real server socket, with a list of clients. */
class cServerHandleImpl:
	public cServerHandle
{
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

	/** Set to true when the server is initialized successfully and is listening for incoming connections. */
	bool m_IsListening;
};
typedef SharedPtr<cServerHandleImpl> cServerHandleImplPtr;
typedef std::vector<cServerHandleImplPtr> cServerHandleImplPtrs;





class cNetworkSingleton
{
	friend class cHostnameLookup;  // Needs access to m_DNSBase
	friend class cTCPLinkImpl;     // Needs access to m_EventBase and m_DNSBase

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


	/** Initializes the LibEvent internals. */
	cNetworkSingleton(void);

	/** Converts LibEvent-generated log events into log messages in MCS log. */
	static void LogCallback(int a_Severity, const char * a_Msg);

	/** Implements the thread that runs LibEvent's event dispatcher loop. */
	static void RunEventLoop(cNetworkSingleton * a_Self);

	/** Removes the specified hostname lookup from m_HostnameLookups. */
	void RemoveHostnameLookup(cHostnameLookup * a_HostnameLookup);
};





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





void cHostnameLookup::Callback(int a_ErrCode, struct evutil_addrinfo * a_Addr, void * a_Self)
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
				// Unknown address family, handle as error
				continue;
			}
		}
		Self->m_Callbacks->OnNameResolved(Self->m_Hostname, IP);
		HasResolved = true;
	}

	// If only unsupported families were reported, call the Error handler:
	if (!HasResolved)
	{
		Self->m_Callbacks->OnError(1);
	}
	else
	{
		Self->m_Callbacks->OnFinished();
	}
	cNetworkSingleton::Get().RemoveHostnameLookup(Self);
}





////////////////////////////////////////////////////////////////////////////////
// cTCPLinkImpl:

cTCPLinkImpl::cTCPLinkImpl(cTCPLink::cCallbacksPtr a_LinkCallbacks):
	super(a_LinkCallbacks),
	m_BufferEvent(bufferevent_socket_new(cNetworkSingleton::Get().m_EventBase, -1, BEV_OPT_CLOSE_ON_FREE))
{
	bufferevent_setcb(m_BufferEvent, ReadCallback, nullptr, EventCallback, this);
	bufferevent_enable(m_BufferEvent, EV_READ | EV_WRITE);
}





cTCPLinkImpl::cTCPLinkImpl(evutil_socket_t a_Socket, cTCPLink::cCallbacksPtr a_LinkCallbacks):
	super(a_LinkCallbacks),
	m_BufferEvent(bufferevent_socket_new(cNetworkSingleton::Get().m_EventBase, a_Socket, BEV_OPT_CLOSE_ON_FREE))
{
	bufferevent_setcb(m_BufferEvent, ReadCallback, nullptr, EventCallback, this);
	bufferevent_enable(m_BufferEvent, EV_READ | EV_WRITE);
}





/** Schedules the actual connection request.
Returns true on success, false on failure. */
bool cTCPLinkImpl::Connect(const AString & a_Host, UInt16 a_Port, cNetwork::cConnectCallbacksPtr a_ConnectCallbacks)
{
	ASSERT(bufferevent_getfd(m_BufferEvent) == -1);  // Did you create this object using the right constructor (the one without the Socket param)?
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





AString cTCPLinkImpl::GetLocalIP(void) const
{
	// TODO
	ASSERT(!"cTCPLinkImpl::GetLocalIP: Not implemented yet");
	return "";
}





UInt16 cTCPLinkImpl::GetLocalPort(void) const
{
	// TODO
	ASSERT(!"cTCPLinkImpl::GetLocalPort(): Not implemented yet");
	return 0;
}





AString cTCPLinkImpl::GetRemoteIP(void) const
{
	// TODO
	ASSERT(!"cTCPLinkImpl::GetRemoteIP(): Not implemented yet");
	return "";
}





UInt16 cTCPLinkImpl::GetRemotePort(void) const
{
	// TODO
	ASSERT(!"cTCPLinkImpl::GetRemotePort(): Not implemented yet");
	return 0;
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
	}

	// If the connectino has been closed, call the link callback:
	if (a_What & BEV_EVENT_EOF)
	{
		Self->m_Callbacks->OnRemoteClosed(*Self);
		return;
	}
	
	// Unknown event, report it:
	LOGWARNING("cTCPLinkImpl: Unhandled LibEvent event %d (0x%x)", a_What, a_What);
	ASSERT(!"cTCPLinkImpl: Unhandled LibEvent event");
}





////////////////////////////////////////////////////////////////////////////////
// cServerHandleImpl:

cServerHandleImpl::cServerHandleImpl(cNetwork::cListenCallbacksPtr a_ListenCallbacks, cTCPLink::cCallbacksPtr a_LinkCallbacks):
	m_ListenCallbacks(a_ListenCallbacks),
	m_LinkCallbacks(a_LinkCallbacks)
{
	// TODO
	ASSERT(!"Not implemented yet!");
}





void cServerHandleImpl::Close(void)
{
	// TODO
	ASSERT(!"Not implemented yet!");
}





bool cServerHandleImpl::Listen(UInt16 a_Port)
{
	// TODO
	ASSERT(!"Not implemented yet!");
	return false;
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
		// TODO: m_Connections.remove(ConnRequest);
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
	// TODO
	ASSERT(!"Not implemented yet!");
	return false;
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




void cNetworkSingleton::RemoveHostnameLookup(cHostnameLookup * a_HostnameLookup)
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




