
// IPLookup.cpp

// Implements the cIPLookup class representing an IP-to-hostname lookup in progress.

#include "Globals.h"
#include "IPLookup.h"
#include <event2/dns.h>
#include "NetworkSingleton.h"





////////////////////////////////////////////////////////////////////////////////
// cIPLookup:

cIPLookup::cIPLookup(const AString & a_IP, cNetwork::cResolveNameCallbacksPtr a_Callbacks):
	m_Callbacks(a_Callbacks),
	m_IP(a_IP)
{
	sockaddr_storage sa;
	int salen = static_cast<int>(sizeof(sa));
	memset(&sa, 0, sizeof(sa));
	evutil_parse_sockaddr_port(a_IP.c_str(), reinterpret_cast<sockaddr *>(&sa), &salen);
	switch (sa.ss_family)
	{
		case AF_INET:
		{
			sockaddr_in * sa4 = reinterpret_cast<sockaddr_in *>(&sa);
			evdns_base_resolve_reverse(cNetworkSingleton::Get().GetDNSBase(), &(sa4->sin_addr), 0, Callback, this);
			break;
		}
		case AF_INET6:
		{
			sockaddr_in6 * sa6 = reinterpret_cast<sockaddr_in6 *>(&sa);
			evdns_base_resolve_reverse_ipv6(cNetworkSingleton::Get().GetDNSBase(), &(sa6->sin6_addr), 0, Callback, this);
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

	// Call the proper callback based on the event received:
	if ((a_Result != 0) || (a_Addresses == nullptr))
	{
		// An error has occurred, notify the error callback:
		Self->m_Callbacks->OnError(a_Result);
	}
	else
	{
		// Call the success handler:
		Self->m_Callbacks->OnNameResolved(*(reinterpret_cast<char **>(a_Addresses)), Self->m_IP);
		Self->m_Callbacks->OnFinished();
	}
	cNetworkSingleton::Get().RemoveIPLookup(Self);
}





////////////////////////////////////////////////////////////////////////////////
// cNetwork API:

bool cNetwork::IPToHostName(
	const AString & a_IP,
	cNetwork::cResolveNameCallbacksPtr a_Callbacks
)
{
	return cNetworkSingleton::Get().IPToHostName(a_IP, a_Callbacks);
}




