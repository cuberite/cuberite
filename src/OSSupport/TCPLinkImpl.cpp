
// TCPLinkImpl.cpp

// Implements the cTCPLinkImpl class implementing the TCP link functionality

#include "Globals.h"
#include "TCPLinkImpl.h"
#include "NetworkSingleton.h"
#include "ServerHandleImpl.h"





////////////////////////////////////////////////////////////////////////////////
// cTCPLinkImpl:

cTCPLinkImpl::cTCPLinkImpl(cTCPLink::cCallbacksPtr a_LinkCallbacks):
	super(a_LinkCallbacks),
	m_BufferEvent(bufferevent_socket_new(cNetworkSingleton::Get().GetEventBase(), -1, BEV_OPT_CLOSE_ON_FREE | BEV_OPT_THREADSAFE))
{
	LOGD("Created new cTCPLinkImpl at %p with BufferEvent at %p", this, m_BufferEvent);
}





cTCPLinkImpl::cTCPLinkImpl(evutil_socket_t a_Socket, cTCPLink::cCallbacksPtr a_LinkCallbacks, cServerHandleImplPtr a_Server, const sockaddr * a_Address, socklen_t a_AddrLen):
	super(a_LinkCallbacks),
	m_BufferEvent(bufferevent_socket_new(cNetworkSingleton::Get().GetEventBase(), a_Socket, BEV_OPT_CLOSE_ON_FREE | BEV_OPT_THREADSAFE)),
	m_Server(a_Server)
{
	LOGD("Created new cTCPLinkImpl at %p with BufferEvent at %p", this, m_BufferEvent);

	// Update the endpoint addresses:
	UpdateLocalAddress();
	UpdateAddress(a_Address, a_AddrLen, m_RemoteIP, m_RemotePort);
}





cTCPLinkImpl::~cTCPLinkImpl()
{
	LOGD("Deleting cTCPLinkImpl at %p with BufferEvent at %p", this, m_BufferEvent);
	bufferevent_free(m_BufferEvent);
}





cTCPLinkImplPtr cTCPLinkImpl::Connect(const AString & a_Host, UInt16 a_Port, cTCPLink::cCallbacksPtr a_LinkCallbacks, cNetwork::cConnectCallbacksPtr a_ConnectCallbacks)
{
	ASSERT(a_LinkCallbacks != nullptr);
	ASSERT(a_ConnectCallbacks != nullptr);

	// Create a new link:
	cTCPLinkImplPtr res{new cTCPLinkImpl(a_LinkCallbacks)};  // Cannot use std::make_shared here, constructor is not accessible
	res->m_ConnectCallbacks = a_ConnectCallbacks;
	cNetworkSingleton::Get().AddLink(res);
	res->m_Callbacks->OnLinkCreated(res);
	res->Enable(res);

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

		// Queue the connect request:
		if (bufferevent_socket_connect(res->m_BufferEvent, reinterpret_cast<sockaddr *>(&sa), salen) == 0)
		{
			// Success
			return res;
		}
		// Failure
		cNetworkSingleton::Get().RemoveLink(res.get());
		return nullptr;
	}

	// a_Host is a hostname, connect after a lookup:
	if (bufferevent_socket_connect_hostname(res->m_BufferEvent, cNetworkSingleton::Get().GetDNSBase(), AF_UNSPEC, a_Host.c_str(), a_Port) == 0)
	{
		// Success
		return res;
	}
	// Failure
	cNetworkSingleton::Get().RemoveLink(res.get());
	return nullptr;
}





void cTCPLinkImpl::Enable(cTCPLinkImplPtr a_Self)
{
	// Take hold of a shared copy of self, to keep as long as the callbacks are coming:
	m_Self = a_Self;

	// Set the LibEvent callbacks and enable processing:
	bufferevent_setcb(m_BufferEvent, ReadCallback, nullptr, EventCallback, this);
	bufferevent_enable(m_BufferEvent, EV_READ | EV_WRITE);
}





bool cTCPLinkImpl::Send(const void * a_Data, size_t a_Length)
{
	return (bufferevent_write(m_BufferEvent, a_Data, a_Length) == 0);
}





void cTCPLinkImpl::Shutdown(void)
{
	#ifdef _WIN32
		shutdown(bufferevent_getfd(m_BufferEvent), SD_SEND);
	#else
		shutdown(bufferevent_getfd(m_BufferEvent), SHUT_WR);
	#endif
	bufferevent_disable(m_BufferEvent, EV_WRITE);
}





void cTCPLinkImpl::Close(void)
{
	// Disable all events on the socket, but keep it alive:
	bufferevent_disable(m_BufferEvent, EV_READ | EV_WRITE);
	if (m_Server == nullptr)
	{
		cNetworkSingleton::Get().RemoveLink(this);
	}
	else
	{
		m_Server->RemoveLink(this);
	}
	m_Self.reset();
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
		Self->m_Callbacks->OnReceivedData(data, length);
	}
}





void cTCPLinkImpl::EventCallback(bufferevent * a_BufferEvent, short a_What, void * a_Self)
{
	LOGD("cTCPLink event callback for link %p, BEV %p; what = 0x%02x", a_Self, a_BufferEvent, a_What);

	ASSERT(a_Self != nullptr);
	cTCPLinkImplPtr Self = static_cast<cTCPLinkImpl *>(a_Self)->m_Self;

	// If an error is reported, call the error callback:
	if (a_What & BEV_EVENT_ERROR)
	{
		// Choose the proper callback to call based on whether we were waiting for connection or not:
		int err = EVUTIL_SOCKET_ERROR();
		if (Self->m_ConnectCallbacks != nullptr)
		{
			if (err == 0)
			{
				// This could be a DNS failure
				err = bufferevent_socket_get_dns_error(a_BufferEvent);
			}
			Self->m_ConnectCallbacks->OnError(err, evutil_socket_error_to_string(err));
		}
		else
		{
			Self->m_Callbacks->OnError(err, evutil_socket_error_to_string(err));
			if (Self->m_Server == nullptr)
			{
				cNetworkSingleton::Get().RemoveLink(Self.get());
			}
			else
			{
				Self->m_Server->RemoveLink(Self.get());
			}
		}
		Self->m_Self.reset();
		return;
	}

	// Pending connection succeeded, call the connection callback:
	if (a_What & BEV_EVENT_CONNECTED)
	{
		Self->UpdateLocalAddress();
		Self->UpdateRemoteAddress();
		if (Self->m_ConnectCallbacks != nullptr)
		{
			Self->m_ConnectCallbacks->OnConnected(*Self);
			// Reset the connect callbacks so that later errors get reported through the link callbacks:
			Self->m_ConnectCallbacks.reset();
			return;
		}
	}

	// If the connection has been closed, call the link callback and remove the connection:
	if (a_What & BEV_EVENT_EOF)
	{
		Self->m_Callbacks->OnRemoteClosed();
		if (Self->m_Server != nullptr)
		{
			Self->m_Server->RemoveLink(Self.get());
		}
		else
		{
			cNetworkSingleton::Get().RemoveLink(Self.get());
		}
		Self->m_Self.reset();
		return;
	}
	
	// Unknown event, report it:
	LOGWARNING("cTCPLinkImpl: Unhandled LibEvent event %d (0x%x)", a_What, a_What);
	ASSERT(!"cTCPLinkImpl: Unhandled LibEvent event");
}





void cTCPLinkImpl::UpdateAddress(const sockaddr * a_Address, socklen_t a_AddrLen, AString & a_IP, UInt16 & a_Port)
{
	// Based on the family specified in the address, use the correct datastructure to convert to IP string:
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
	UpdateAddress(reinterpret_cast<const sockaddr *>(&sa), salen, m_RemoteIP, m_RemotePort);
}





////////////////////////////////////////////////////////////////////////////////
// cNetwork API:

bool cNetwork::Connect(
	const AString & a_Host,
	UInt16 a_Port,
	cNetwork::cConnectCallbacksPtr a_ConnectCallbacks,
	cTCPLink::cCallbacksPtr a_LinkCallbacks
)
{
	// Add a connection request to the queue:
	cTCPLinkImplPtr Conn = cTCPLinkImpl::Connect(a_Host, a_Port, a_LinkCallbacks, a_ConnectCallbacks);
	return (Conn != nullptr);
}





