
// NetworkInterfaceEnum.cpp

// Implements the cNetwork::EnumLocalIPAddresses() interface enumeration function

#include "Globals.h"
#include "Network.h"
#include "event2/util.h"

#if defined(_WIN32)
	#include <IPHlpApi.h>
	#pragma comment(lib, "IPHLPAPI.lib")
#elif !defined(ANDROID)  // _WIN32
	#include <sys/types.h>
	#include <ifaddrs.h>
	#include <netinet/in.h>
	#include <arpa/inet.h>
#endif  // else _WIN32





#if defined(_WIN32)

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
	return IP;
}

#elif !defined(ANDROID)  // _WIN32

static AString PrintAddress(ifaddrs * InterfaceAddress)
{
	switch (InterfaceAddress->ifa_addr->sa_family)
	{
		case AF_INET:
		{  // IPv4
			char AddressBuffer[INET_ADDRSTRLEN];
			sockaddr_in InternetSocket;

			std::memcpy(&InternetSocket, InterfaceAddress->ifa_addr, sizeof(InternetSocket));
			inet_ntop(AF_INET, &InternetSocket.sin_addr, AddressBuffer, INET_ADDRSTRLEN);
			return AddressBuffer;
		}
		case AF_INET6:
		{  // IPv6
			char AddressBuffer[INET6_ADDRSTRLEN];
			sockaddr_in6 InternetSocket;

			std::memcpy(&InternetSocket, InterfaceAddress->ifa_addr, sizeof(InternetSocket));
			inet_ntop(AF_INET6, &InternetSocket.sin6_addr, AddressBuffer, INET6_ADDRSTRLEN);
			return AddressBuffer;
		}
		default:
		{
			LOG("Unknown address family: %i", InterfaceAddress->ifa_addr->sa_family);
			return "";
		}
	}
}

#endif  // else !ANDROID





AStringVector cNetwork::EnumLocalIPAddresses(void)
{
	AStringVector res;

	#if defined(_WIN32)

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
			if (pCurrAddresses->OperStatus != IfOperStatusUp)
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

	#elif !defined(ANDROID)  // _WIN32

		struct ifaddrs * ifAddrStruct = nullptr;
		getifaddrs(&ifAddrStruct);

		for (auto ifa = ifAddrStruct; ifa != nullptr; ifa = ifa->ifa_next)
		{
			if (ifa->ifa_addr == nullptr)
			{
				continue;
			}

			auto Address = PrintAddress(ifa);
			if (!Address.empty())
			{
				res.emplace_back(Address);
			}
		}

		if (ifAddrStruct != nullptr)
		{
			freeifaddrs(ifAddrStruct);
		}

	#endif  // else _WIN32

	return res;
}




