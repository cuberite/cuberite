
// HostnameLookup.cpp

// Implements the cHostnameLookup class representing an in-progress hostname-to-IP lookup

#include "Globals.h"
#include "HostnameLookup.h"
#include <event2/dns.h>
#include "NetworkSingleton.h"





////////////////////////////////////////////////////////////////////////////////
// cHostnameLookup:

cHostnameLookup::cHostnameLookup(cNetwork::cResolveNameCallbacksPtr a_Callbacks):
	m_Callbacks(a_Callbacks)
{
}





void cHostnameLookup::Lookup(const AString & a_Hostname)
{
	// Store the hostname for the callback:
	m_Hostname = a_Hostname;

	// Start the lookup:
	// Note that we don't have to store the LibEvent lookup handle, LibEvent will free it on its own.
	evutil_addrinfo hints;
	memset(&hints, 0, sizeof(hints));
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_family = AF_UNSPEC;
	hints.ai_flags = EVUTIL_AI_CANONNAME;
	evdns_getaddrinfo(cNetworkSingleton::Get().GetDNSBase(), a_Hostname.c_str(), nullptr, &hints, Callback, this);
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
		Self->m_Callbacks->OnError(DNS_ERR_NODATA);
	}
	else
	{
		Self->m_Callbacks->OnFinished();
	}
	evutil_freeaddrinfo(OrigAddr);
	cNetworkSingleton::Get().RemoveHostnameLookup(Self);
}





////////////////////////////////////////////////////////////////////////////////
// cNetwork API:

bool cNetwork::HostnameToIP(
	const AString & a_Hostname,
	cNetwork::cResolveNameCallbacksPtr a_Callbacks
)
{
	auto Lookup = std::make_shared<cHostnameLookup>(a_Callbacks);
	cNetworkSingleton::Get().AddHostnameLookup(Lookup);
	Lookup->Lookup(a_Hostname);
	return true;
}




