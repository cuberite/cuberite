
// UDPEndpointImpl.cpp

// Implements the cUDPEndpointImpl class representing an implementation of an endpoint in UDP communication

#include "Globals.h"
#include "UDPEndpointImpl.h"
#include "NetworkSingleton.h"





////////////////////////////////////////////////////////////////////////////////
// Globals:

namespace UDPEndpointImplHelper
{
	static bool IsValidSocket(evutil_socket_t a_Socket)
	{
#ifdef _WIN32
		return (a_Socket != INVALID_SOCKET);
#else  // _WIN32
		return (a_Socket >= 0);
#endif  // else _WIN32
	}
}





/** Converts a_SrcAddr in IPv4 format to a_DstAddr in IPv6 format (using IPv4-mapped IPv6). */
static void ConvertIPv4ToMappedIPv6(sockaddr_in & a_SrcAddr, sockaddr_in6 & a_DstAddr)
{
	memset(&a_DstAddr, 0, sizeof(a_DstAddr));
	a_DstAddr.sin6_family = AF_INET6;
	a_DstAddr.sin6_addr.s6_addr[10] = 0xff;
	a_DstAddr.sin6_addr.s6_addr[11] = 0xff;
	a_DstAddr.sin6_addr.s6_addr[12] = static_cast<Byte>((a_SrcAddr.sin_addr.s_addr >>  0) & 0xff);
	a_DstAddr.sin6_addr.s6_addr[13] = static_cast<Byte>((a_SrcAddr.sin_addr.s_addr >>  8) & 0xff);
	a_DstAddr.sin6_addr.s6_addr[14] = static_cast<Byte>((a_SrcAddr.sin_addr.s_addr >> 16) & 0xff);
	a_DstAddr.sin6_addr.s6_addr[15] = static_cast<Byte>((a_SrcAddr.sin_addr.s_addr >> 24) & 0xff);
	a_DstAddr.sin6_port = a_SrcAddr.sin_port;
}





////////////////////////////////////////////////////////////////////////////////
// cUDPSendAfterLookup:

/** A hostname-to-IP resolver callback that sends the data stored within to the resolved IP address.
This is used for sending UDP datagrams to hostnames, so that the cUDPEndpoint::Send() doesn't block.
Instead an instance of this callback is queued for resolving and the data is sent once the IP is resolved. */
class cUDPSendAfterLookup:
	public cNetwork::cResolveNameCallbacks
{
public:
	cUDPSendAfterLookup(const AString & a_Data, UInt16 a_Port, evutil_socket_t a_MainSock, evutil_socket_t a_SecondSock, bool a_IsMainSockIPv6):
		m_Data(a_Data),
		m_Port(a_Port),
		m_MainSock(a_MainSock),
		m_SecondSock(a_SecondSock),
		m_IsMainSockIPv6(a_IsMainSockIPv6),
		m_HasIPv4(false),
		m_HasIPv6(false)
	{
	}

protected:
	/** The data to send after the hostname is resolved. */
	AString m_Data;

	/** The port to which to send the data. */
	UInt16 m_Port;

	/** The primary socket to use for sending. */
	evutil_socket_t m_MainSock;

	/** The secondary socket to use for sending, if needed by the OS. */
	evutil_socket_t m_SecondSock;

	/** True if m_MainSock is an IPv6 socket. */
	bool m_IsMainSockIPv6;

	/** The IPv4 address resolved, if any. */
	sockaddr_in m_AddrIPv4;

	/** Set to true if the name resolved to an IPv4 address. */
	bool m_HasIPv4;

	/** The IPv6 address resolved, if any. */
	sockaddr_in6 m_AddrIPv6;

	/** Set to true if the name resolved to an IPv6 address. */
	bool m_HasIPv6;


	// cNetwork::cResolveNameCallbacks overrides:
	virtual void OnNameResolved(const AString & a_Name, const AString & a_PI) override
	{
		// Not needed
	}

	virtual bool OnNameResolvedV4(const AString & a_Name, const sockaddr_in * a_IP) override
	{
		if (!m_HasIPv4)
		{
			m_AddrIPv4 = *a_IP;
			m_AddrIPv4.sin_port = htons(m_Port);
			m_HasIPv4 = true;
		}

		// Don't want OnNameResolved() callback
		return false;
	}

	virtual bool OnNameResolvedV6(const AString & a_Name, const sockaddr_in6 * a_IP) override
	{
		if (!m_HasIPv6)
		{
			m_AddrIPv6 = *a_IP;
			m_AddrIPv6.sin6_port = htons(m_Port);
			m_HasIPv6 = true;
		}

		// Don't want OnNameResolved() callback
		return false;
	}

	virtual void OnFinished(void) override
	{
		// Send the actual data, through the correct socket and using the correct resolved address:
		if (m_IsMainSockIPv6)
		{
			if (m_HasIPv6)
			{
				sendto(m_MainSock, m_Data.data(), m_Data.size(), 0, reinterpret_cast<const sockaddr *>(&m_AddrIPv6), static_cast<socklen_t>(sizeof(m_AddrIPv6)));
			}
			else if (m_HasIPv4)
			{
				// If the secondary socket is valid, it is an IPv4 socket, so use that:
				if (m_SecondSock != -1)
				{
					sendto(m_SecondSock, m_Data.data(), m_Data.size(), 0, reinterpret_cast<const sockaddr *>(&m_AddrIPv4), static_cast<socklen_t>(sizeof(m_AddrIPv4)));
				}
				else
				{
					// Need an address conversion from IPv4 to IPv6-mapped-IPv4:
					ConvertIPv4ToMappedIPv6(m_AddrIPv4, m_AddrIPv6);
					sendto(m_MainSock, m_Data.data(), m_Data.size(), 0, reinterpret_cast<const sockaddr *>(&m_AddrIPv6), static_cast<socklen_t>(sizeof(m_AddrIPv6)));
				}
			}
			else
			{
				LOGD("UDP endpoint queued sendto: Name not resolved");
				return;
			}
		}
		else  // m_IsMainSockIPv6
		{
			// Main socket is IPv4 only, only allow IPv4 dst address:
			if (!m_HasIPv4)
			{
				LOGD("UDP endpoint queued sendto: Name not resolved to IPv4 for an IPv4-only socket");
				return;
			}
			sendto(m_MainSock, m_Data.data(), m_Data.size(), 0, reinterpret_cast<const sockaddr *>(&m_AddrIPv4), static_cast<socklen_t>(sizeof(m_AddrIPv4)));
		}
	}

	virtual void OnError(int a_ErrorCode, const AString & a_ErrorMsg) override
	{
		// Nothing needed
	}
};





////////////////////////////////////////////////////////////////////////////////
// cUDPEndpointImpl:

cUDPEndpointImpl::cUDPEndpointImpl(UInt16 a_Port, cUDPEndpoint::cCallbacks & a_Callbacks):
	Super(a_Callbacks),
	m_Port(0),
	m_MainSock(-1),
	m_IsMainSockIPv6(true),
	m_SecondarySock(-1),
	m_MainEvent(nullptr),
	m_SecondaryEvent(nullptr)
{
	Open(a_Port);
}





cUDPEndpointImpl::~cUDPEndpointImpl()
{
	Close();
}





void cUDPEndpointImpl::Close(void)
{
	if (m_Port == 0)
	{
		// Already closed
		return;
	}

	// Close the LibEvent handles:
	if (m_MainEvent != nullptr)
	{
		event_free(m_MainEvent);
		m_MainEvent = nullptr;
	}
	if (m_SecondaryEvent != nullptr)
	{
		event_free(m_SecondaryEvent);
		m_SecondaryEvent = nullptr;
	}

	// Close the OS sockets:
	evutil_closesocket(m_MainSock);
	m_MainSock = -1;
	evutil_closesocket(m_SecondarySock);
	m_SecondarySock = -1;

	// Mark as closed:
	m_Port = 0;
}





bool cUDPEndpointImpl::IsOpen(void) const
{
	return (m_Port != 0);
}





UInt16 cUDPEndpointImpl::GetPort(void) const
{
	return m_Port;
}





bool cUDPEndpointImpl::Send(const AString & a_Payload, const AString & a_Host, UInt16 a_Port)
{
	// If a_Host is an IP address, send the data directly:
	sockaddr_storage sa;
	int salen = static_cast<int>(sizeof(sa));
	memset(&sa, 0, sizeof(sa));
	if (evutil_parse_sockaddr_port(a_Host.c_str(), reinterpret_cast<sockaddr *>(&sa), &salen) != 0)
	{
		// a_Host is a hostname, we need to do a lookup first:
		auto queue = std::make_shared<cUDPSendAfterLookup>(a_Payload, a_Port, m_MainSock, m_SecondarySock, m_IsMainSockIPv6);
		return cNetwork::HostnameToIP(a_Host, queue);
	}

	// a_Host is an IP address and has been parsed into "sa"
	// Insert the correct port and send data:
	int NumSent;
	switch (sa.ss_family)
	{
		case AF_INET:
		{
			reinterpret_cast<sockaddr_in *>(&sa)->sin_port = htons(a_Port);
			if (m_IsMainSockIPv6)
			{
				if (UDPEndpointImplHelper::IsValidSocket(m_SecondarySock))
				{
					// The secondary socket, which is always IPv4, is present:
					NumSent = static_cast<int>(sendto(m_SecondarySock, a_Payload.data(), a_Payload.size(), 0, reinterpret_cast<const sockaddr *>(&sa), static_cast<socklen_t>(salen)));
				}
				else
				{
					// Need to convert IPv4 to IPv6 address before sending:
					sockaddr_in6 IPv6;
					ConvertIPv4ToMappedIPv6(*reinterpret_cast<sockaddr_in *>(&sa), IPv6);
					NumSent = static_cast<int>(sendto(m_MainSock, a_Payload.data(), a_Payload.size(), 0, reinterpret_cast<const sockaddr *>(&IPv6), static_cast<socklen_t>(sizeof(IPv6))));
				}
			}
			else
			{
				NumSent = static_cast<int>(sendto(m_MainSock, a_Payload.data(), a_Payload.size(), 0, reinterpret_cast<const sockaddr *>(&sa), static_cast<socklen_t>(salen)));
			}
			break;
		}

		case AF_INET6:
		{
			reinterpret_cast<sockaddr_in6 *>(&sa)->sin6_port = htons(a_Port);
			NumSent = static_cast<int>(sendto(m_MainSock, a_Payload.data(), a_Payload.size(), 0, reinterpret_cast<const sockaddr *>(&sa), static_cast<socklen_t>(salen)));
			break;
		}
		default:
		{
			LOGD("UDP sendto: Invalid address family for address \"%s\".", a_Host.c_str());
			return false;
		}
	}
	return (NumSent > 0);
}





void cUDPEndpointImpl::EnableBroadcasts(void)
{
	ASSERT(IsOpen());

	// Enable broadcasts on the main socket:
	// Some OSes use ints, others use chars, so we try both
	int broadcastInt = 1;
	char broadcastChar = 1;
	// (Note that Windows uses const char * for option values, while Linux uses const void *)
	if (setsockopt(m_MainSock, SOL_SOCKET, SO_BROADCAST, reinterpret_cast<const char *>(&broadcastInt), sizeof(broadcastInt)) == -1)
	{
		if (setsockopt(m_MainSock, SOL_SOCKET, SO_BROADCAST, &broadcastChar, sizeof(broadcastChar)) == -1)
		{
			int err = EVUTIL_SOCKET_ERROR();
			LOGWARNING("Cannot enable broadcasts on port %d: %d (%s)", m_Port, err, evutil_socket_error_to_string(err));
			return;
		}

		// Enable broadcasts on the secondary socket, if opened (use char, it worked for primary):
		if (UDPEndpointImplHelper::IsValidSocket(m_SecondarySock))
		{
			if (setsockopt(m_SecondarySock, SOL_SOCKET, SO_BROADCAST, &broadcastChar, sizeof(broadcastChar)) == -1)
			{
				int err = EVUTIL_SOCKET_ERROR();
				LOGWARNING("Cannot enable broadcasts on port %d (secondary): %d (%s)", m_Port, err, evutil_socket_error_to_string(err));
			}
		}
		return;
	}

	// Enable broadcasts on the secondary socket, if opened (use int, it worked for primary):
	if (UDPEndpointImplHelper::IsValidSocket(m_SecondarySock))
	{
		if (setsockopt(m_SecondarySock, SOL_SOCKET, SO_BROADCAST, reinterpret_cast<const char *>(&broadcastInt), sizeof(broadcastInt)) == -1)
		{
			int err = EVUTIL_SOCKET_ERROR();
			LOGWARNING("Cannot enable broadcasts on port %d (secondary): %d (%s)", m_Port, err, evutil_socket_error_to_string(err));
		}
	}
}





void cUDPEndpointImpl::Open(UInt16 a_Port)
{
	ASSERT(m_Port == 0);  // Must not be already open

	// Make sure the cNetwork internals are innitialized:
	cNetworkSingleton::Get();

	// Set up the main socket:
	// It should listen on IPv6 with IPv4 fallback, when available; IPv4 when IPv6 is not available.
	bool NeedsTwoSockets = false;
	m_IsMainSockIPv6 = true;
	m_MainSock = socket(AF_INET6, SOCK_DGRAM, IPPROTO_UDP);

	int err;
	if (!UDPEndpointImplHelper::IsValidSocket(m_MainSock))
	{
		// Failed to create IPv6 socket, create an IPv4 one instead:
		m_IsMainSockIPv6 = false;
		err = EVUTIL_SOCKET_ERROR();
		LOGD("UDP: Failed to create IPv6 MainSock: %d (%s)", err, evutil_socket_error_to_string(err));
		m_MainSock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
		if (!UDPEndpointImplHelper::IsValidSocket(m_MainSock))
		{
			err = EVUTIL_SOCKET_ERROR();
			m_Callbacks.OnError(err, Printf("Cannot create UDP socket for port %d: %s", a_Port, evutil_socket_error_to_string(err)));
			return;
		}

		// Allow the port to be reused right after the socket closes:
		if (evutil_make_listen_socket_reuseable(m_MainSock) != 0)
		{
			err = EVUTIL_SOCKET_ERROR();
			LOG("UDP Port %d cannot be made reusable: %d (%s). Restarting the server might not work.",
				a_Port, err, evutil_socket_error_to_string(err)
			);
		}

		// Bind to all interfaces:
		sockaddr_in name;
		memset(&name, 0, sizeof(name));
		name.sin_family = AF_INET;
		name.sin_port = ntohs(a_Port);
		if (bind(m_MainSock, reinterpret_cast<const sockaddr *>(&name), sizeof(name)) != 0)
		{
			err = EVUTIL_SOCKET_ERROR();
			m_Callbacks.OnError(err, Printf("Cannot bind UDP port %d: %s", a_Port, evutil_socket_error_to_string(err)));
			evutil_closesocket(m_MainSock);
			return;
		}
	}
	else
	{
		// IPv6 socket created, switch it into "dualstack" mode:
		UInt32 Zero = 0;
		#ifdef _WIN32
			// WinXP doesn't support this feature, so if the setting fails, create another socket later on:
			int res = setsockopt(m_MainSock, IPPROTO_IPV6, IPV6_V6ONLY, reinterpret_cast<const char *>(&Zero), sizeof(Zero));
			err = EVUTIL_SOCKET_ERROR();
			NeedsTwoSockets = ((res == SOCKET_ERROR) && (err == WSAENOPROTOOPT));
		#else
			setsockopt(m_MainSock, IPPROTO_IPV6, IPV6_V6ONLY, reinterpret_cast<const char *>(&Zero), sizeof(Zero));
		#endif

		// Allow the port to be reused right after the socket closes:
		if (evutil_make_listen_socket_reuseable(m_MainSock) != 0)
		{
			err  = EVUTIL_SOCKET_ERROR();
			LOG("UDP Port %d cannot be made reusable: %d (%s). Restarting the server might not work.",
				a_Port, err, evutil_socket_error_to_string(err)
			);
		}

		// Bind to all interfaces:
		sockaddr_in6 name;
		memset(&name, 0, sizeof(name));
		name.sin6_family = AF_INET6;
		name.sin6_port = ntohs(a_Port);
		if (bind(m_MainSock, reinterpret_cast<const sockaddr *>(&name), sizeof(name)) != 0)
		{
			err = EVUTIL_SOCKET_ERROR();
			m_Callbacks.OnError(err, Printf("Cannot bind to UDP port %d: %s", a_Port, evutil_socket_error_to_string(err)));
			evutil_closesocket(m_MainSock);
			return;
		}
	}
	if (evutil_make_socket_nonblocking(m_MainSock) != 0)
	{
		err = EVUTIL_SOCKET_ERROR();
		m_Callbacks.OnError(err, Printf("Cannot make socket on UDP port %d nonblocking: %s", a_Port, evutil_socket_error_to_string(err)));
		evutil_closesocket(m_MainSock);
		return;
	}
	m_MainEvent = event_new(cNetworkSingleton::Get().GetEventBase(), m_MainSock, EV_READ | EV_PERSIST, RawCallback, this);
	event_add(m_MainEvent, nullptr);

	// Read the actual port number on which the socket is listening:
	{
		sockaddr_storage name;
		socklen_t namelen = static_cast<socklen_t>(sizeof(name));
		getsockname(m_MainSock, reinterpret_cast<sockaddr *>(&name), &namelen);
		switch (name.ss_family)
		{
			case AF_INET:
			{
				sockaddr_in * sin = reinterpret_cast<sockaddr_in *>(&name);
				m_Port = ntohs(sin->sin_port);
				break;
			}
			case AF_INET6:
			{
				sockaddr_in6 * sin6 = reinterpret_cast<sockaddr_in6 *>(&name);
				m_Port = ntohs(sin6->sin6_port);
				break;
			}
		}
	}

	// If we don't need to create another socket, bail out now:
	if (!NeedsTwoSockets)
	{
		return;
	}

	// If a secondary socket is required (WinXP dual-stack), create it here:
	LOGD("Creating a second UDP socket for IPv4");
	m_SecondarySock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	if (!UDPEndpointImplHelper::IsValidSocket(m_SecondarySock))
	{
		// Don't report as an error, the primary socket is working
		err = EVUTIL_SOCKET_ERROR();
		LOGD("Socket creation failed for secondary UDP socket for port %d: %d, %s", m_Port, err, evutil_socket_error_to_string(err));
		return;
	}

	// Allow the port to be reused right after the socket closes:
	if (evutil_make_listen_socket_reuseable(m_SecondarySock) != 0)
	{
		// Don't report as an error, the primary socket is working
		err = EVUTIL_SOCKET_ERROR();
		LOGD("UDP Port %d cannot be made reusable (second socket): %d (%s). Restarting the server might not work.",
			a_Port, err, evutil_socket_error_to_string(err)
		);
		evutil_closesocket(m_SecondarySock);
		m_SecondarySock = -1;
		return;
	}

	// Make the secondary socket nonblocking:
	if (evutil_make_socket_nonblocking(m_SecondarySock) != 0)
	{
		// Don't report as an error, the primary socket is working
		err = EVUTIL_SOCKET_ERROR();
		LOGD("evutil_make_socket_nonblocking() failed for secondary UDP socket: %d, %s", err, evutil_socket_error_to_string(err));
		evutil_closesocket(m_SecondarySock);
		m_SecondarySock = -1;
		return;
	}

	// Bind to all IPv4 interfaces:
	sockaddr_in name;
	memset(&name, 0, sizeof(name));
	name.sin_family = AF_INET;
	name.sin_port = ntohs(m_Port);
	if (bind(m_SecondarySock, reinterpret_cast<const sockaddr *>(&name), sizeof(name)) != 0)
	{
		// Don't report as an error, the primary socket is working
		err = EVUTIL_SOCKET_ERROR();
		LOGD("Cannot bind secondary socket to UDP port %d: %d (%s)", m_Port, err, evutil_socket_error_to_string(err));
		evutil_closesocket(m_SecondarySock);
		m_SecondarySock = -1;
		return;
	}

	m_SecondaryEvent = event_new(cNetworkSingleton::Get().GetEventBase(), m_SecondarySock, EV_READ | EV_PERSIST, RawCallback, this);
	event_add(m_SecondaryEvent, nullptr);
}





void cUDPEndpointImpl::RawCallback(evutil_socket_t a_Socket, short a_What, void * a_Self)
{
	cUDPEndpointImpl * Self = reinterpret_cast<cUDPEndpointImpl *>(a_Self);
	Self->Callback(a_Socket, a_What);
}





void cUDPEndpointImpl::Callback(evutil_socket_t a_Socket, short a_What)
{
	if ((a_What & EV_READ) != 0)
	{
		// Receive datagram from the socket:
		char buf[64 KiB];
		sockaddr_storage sa;
		socklen_t salen = static_cast<socklen_t>(sizeof(sa));
		auto len = recvfrom(a_Socket, buf, sizeof(buf), 0, reinterpret_cast<sockaddr *>(&sa), &salen);
		if (len >= 0)
		{
			// Convert the remote IP address to a string:
			char RemoteHost[128];
			UInt16 RemotePort;
			switch (sa.ss_family)
			{
				case AF_INET:
				{
					auto sin = reinterpret_cast<sockaddr_in *>(&sa);
					evutil_inet_ntop(sa.ss_family, &sin->sin_addr, RemoteHost, sizeof(RemoteHost));
					RemotePort = ntohs(sin->sin_port);
					break;
				}
				case AF_INET6:
				{
					auto sin = reinterpret_cast<sockaddr_in6 *>(&sa);
					evutil_inet_ntop(sa.ss_family, &sin->sin6_addr, RemoteHost, sizeof(RemoteHost));
					RemotePort = ntohs(sin->sin6_port);
					break;
				}
				default:
				{
					return;
				}
			}

			// Call the callback:
			m_Callbacks.OnReceivedData(buf, static_cast<size_t>(len), RemoteHost, RemotePort);
		}
	}
}





////////////////////////////////////////////////////////////////////////////////
// cNetwork API:

cUDPEndpointPtr cNetwork::CreateUDPEndpoint(UInt16 a_Port, cUDPEndpoint::cCallbacks & a_Callbacks)
{
	return std::make_shared<cUDPEndpointImpl>(a_Port, a_Callbacks);
}




