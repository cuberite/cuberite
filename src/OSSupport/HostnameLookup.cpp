
// HostnameLookup.cpp

// Implements the cHostnameLookup class representing an in-progress hostname-to-IP lookup

#include "Globals.h"
#include "HostnameLookup.h"
#include "NetworkSingleton.h"
#include "GetAddressInfoError.h"





////////////////////////////////////////////////////////////////////////////////
// cHostnameLookup:

cHostnameLookup::cHostnameLookup(const AString & a_Hostname, cNetwork::cResolveNameCallbacksPtr a_Callbacks):
	m_Callbacks(std::move(a_Callbacks)),
	m_Hostname(a_Hostname)
{
}





void cHostnameLookup::Lookup(const AString & a_Hostname, cNetwork::cResolveNameCallbacksPtr a_Callbacks)
{
	// Cannot use std::make_shared here, constructor is not accessible
	cHostnameLookupPtr Lookup{ new cHostnameLookup(a_Hostname, std::move(a_Callbacks)) };

	// Note the Lookup object is owned solely by this lambda which is destroyed after it runs
	cNetworkSingleton::Get().GetLookupThread().ScheduleLookup([=]()
	{
		// Start the lookup:
		addrinfo hints;
		memset(&hints, 0, sizeof(hints));
		hints.ai_protocol = IPPROTO_TCP;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_family = AF_UNSPEC;
		hints.ai_flags = AI_CANONNAME;

		addrinfo * Result;
		int ErrCode = getaddrinfo(Lookup->m_Hostname.c_str(), nullptr, &hints, &Result);

		Lookup->Callback(ErrCode, Result);
	});
}





void cHostnameLookup::Callback(int a_ErrCode, addrinfo * a_Addr)
{
	// If an error has occurred, notify the error callback:
	if (a_ErrCode != 0)
	{
		m_Callbacks->OnError(a_ErrCode, ErrorString(a_ErrCode));
		return;
	}

	// Call the success handler for each entry received:
	bool HasResolved = false;
	addrinfo * OrigAddr = a_Addr;
	for (;a_Addr != nullptr; a_Addr = a_Addr->ai_next)
	{
		char IP[128];
		switch (a_Addr->ai_family)
		{
			case AF_INET:  // IPv4
			{
				sockaddr_in * sin = reinterpret_cast<sockaddr_in *>(a_Addr->ai_addr);
				if (!m_Callbacks->OnNameResolvedV4(m_Hostname, sin))
				{
					// Callback indicated that the IP shouldn't be serialized to a string, just continue with the next address:
					HasResolved = true;
					continue;
				}
				evutil_inet_ntop(AF_INET, &(sin->sin_addr), IP, sizeof(IP));
				break;
			}
			case AF_INET6:  // IPv6
			{
				sockaddr_in6 * sin = reinterpret_cast<sockaddr_in6 *>(a_Addr->ai_addr);
				if (!m_Callbacks->OnNameResolvedV6(m_Hostname, sin))
				{
					// Callback indicated that the IP shouldn't be serialized to a string, just continue with the next address:
					HasResolved = true;
					continue;
				}
				evutil_inet_ntop(AF_INET6, &(sin->sin6_addr), IP, sizeof(IP));
				break;
			}
			default:
			{
				// Unknown address family, handle as if this entry wasn't received
				continue;  // for (a_Addr)
			}
		}
		m_Callbacks->OnNameResolved(m_Hostname, IP);
		HasResolved = true;
	}  // for (a_Addr)

	// If only unsupported families were reported, call the Error handler:
	if (!HasResolved)
	{
		m_Callbacks->OnError(EAI_NONAME, ErrorString(EAI_NONAME));
	}
	else
	{
		m_Callbacks->OnFinished();
	}
	freeaddrinfo(OrigAddr);
}





////////////////////////////////////////////////////////////////////////////////
// cNetwork API:

bool cNetwork::HostnameToIP(
	const AString & a_Hostname,
	cNetwork::cResolveNameCallbacksPtr a_Callbacks
)
{
	cHostnameLookup::Lookup(a_Hostname, std::move(a_Callbacks));
	return true;
}




