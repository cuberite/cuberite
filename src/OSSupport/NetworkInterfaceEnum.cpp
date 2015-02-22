
// NetworkInterfaceEnum.cpp

// Implements the cNetwork::EnumLocalIPAddresses() interface enumeration function

#include "Globals.h"
#include "Network.h"
#include "event2/util.h"
#ifdef _WIN32
	#include <IPHlpApi.h>
	#pragma comment(lib, "IPHLPAPI.lib")
#else  // _WIN32
#endif  // else _WIN32





#ifdef SELF_TEST

static class cEnumIPAddressTest
{
public:
	cEnumIPAddressTest(void)
	{
		printf("Enumerating all IP addresses...\n");
		auto IPs = cNetwork::EnumLocalIPAddresses();
		for (auto & ip: IPs)
		{
			printf("  %s\n", ip.c_str());
		}
		printf("done.\n");
	}
} g_EnumIPAddressTest;

#endif  // SELF_TEST





#ifdef _WIN32

/** Converts the SOCKET_ADDRESS structure received from the OS into an IP address string. */
static AString PrintAddress(SOCKET_ADDRESS & a_Addr)
{
	char IP[128];
	switch (a_Addr.lpSockaddr->sa_family)
	{
		case AF_INET:
		{
			auto sin = reinterpret_cast<const sockaddr_in *>(a_Addr.lpSockaddr);
			evutil_inet_ntop(a_Addr.lpSockaddr->sa_family, &(sin->sin_addr), IP, sizeof(IP));
			break;
		}
		case AF_INET6:
		{
			auto sin = reinterpret_cast<const sockaddr_in6 *>(a_Addr.lpSockaddr);
			evutil_inet_ntop(a_Addr.lpSockaddr->sa_family, &(sin->sin6_addr), IP, sizeof(IP));
			break;
		}
		default:
		{
			IP[0] = 0;
			break;
		}
	}
	return AString(IP);
}

#endif  // _WIN32





AStringVector cNetwork::EnumLocalIPAddresses(void)
{
	AStringVector res;

	#ifdef _WIN32

		// Query the OS for all adapters' addresses:
		char buffer[64 KiB];  // A buffer backing the address list
		PIP_ADAPTER_ADDRESSES pAddresses = reinterpret_cast<PIP_ADAPTER_ADDRESSES>(&buffer);
		ULONG outBufLen = sizeof(buffer);
		DWORD dwRetVal = GetAdaptersAddresses(
			AF_UNSPEC,
			GAA_FLAG_SKIP_ANYCAST | GAA_FLAG_SKIP_MULTICAST | GAA_FLAG_SKIP_DNS_SERVER | GAA_FLAG_SKIP_FRIENDLY_NAME, nullptr,
			pAddresses, &outBufLen
		);
		if (dwRetVal != ERROR_SUCCESS)
		{
			LOG("GetAdaptersAddresses() failed: %u", dwRetVal);
			return res;
		}

		// Enumerate all active adapters
		for (auto pCurrAddresses = pAddresses; pCurrAddresses != nullptr; pCurrAddresses = pCurrAddresses->Next)
		{
			if (pCurrAddresses->OperStatus != 1)
			{
				// Adapter not active, skip it:
				continue;
			}

			// Collect all IP addresses on this adapter:
			for (auto pUnicast = pCurrAddresses->FirstUnicastAddress; pUnicast != nullptr; pUnicast = pUnicast->Next)
			{
				auto Address = PrintAddress(pUnicast->Address);
				if (!Address.empty())
				{
					res.push_back(Address);
				}
			}  // for pUnicast
		}  // for pCurrAddresses

	#else  // _WIN32

		// TODO: Enumerate network interfaces on Linux

	#endif  // else _WIN32

	return res;
}




