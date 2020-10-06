
// HostnameLookup.cpp

// Implements the cHostnameLookup class representing an in-progress hostname-to-IP lookup

#include "Globals.h"
#include "HostnameLookup.h"
#include "NetworkSingleton.h"
#include "GetAddressInfoError.h"





////////////////////////////////////////////////////////////////////////////////
// cHostnameLookup:

void cHostnameLookup::Lookup(
	const AString & a_Hostname, cNetwork::cResolveNameCallbacksPtr a_Callbacks)
{
	// Note the Lookup object is owned solely by this lambda which is destroyed
	// after it runs
	cNetworkSingleton::Get().GetLookupThread().async_resolve(
		a_Hostname, "",
		[Callbacks = std::move(a_Callbacks)](const auto & a_Error, const auto & a_Results)
		{
			// If an error has occurred, notify the error callback:
			if (a_Error)
			{
				Callbacks->OnError(a_Error.value(), a_Error.message());
				return;
			}

			Callback(*Callbacks.get(), a_Results);
		}
	);
}





void cHostnameLookup::Callback(cNetwork::cResolveNameCallbacks & a_Callbacks, const asio::ip::tcp::resolver::results_type & a_Addr)
{
	// Call the success handler for each entry received:
	bool HasResolved = false;

	for (const auto & Addr : a_Addr)
	{
		const auto & Endpoint = Addr.endpoint();
		const auto & Address = Endpoint.address();
		const auto & Hostname = Addr.host_name();

		if (Address.is_v4())
		{
			const auto sin =
				reinterpret_cast<const sockaddr_in *>(Endpoint.data());
			if (!a_Callbacks.OnNameResolvedV4(Hostname, sin))
			{
				// Callback indicated that the IP shouldn't be serialized to
				// a string, just continue with the next address:
				HasResolved = true;
				continue;
			}
		}
		else if (Address.is_v6())
		{
			const auto sin =
				reinterpret_cast<const sockaddr_in6 *>(Endpoint.data());
			if (!a_Callbacks.OnNameResolvedV6(Hostname, sin))
			{
				// Callback indicated that the IP shouldn't be serialized to
				// a string, just continue with the next address:
				HasResolved = true;
				continue;
			}
		}
		else
		{
			// Unknown address family, handle as if this entry wasn't
			// received
			continue;  // for (a_Addr)
		}
		a_Callbacks.OnNameResolved(Hostname, Address.to_string());
		HasResolved = true;
	}  // for (a_Addr)

	// If only unsupported families were reported, call the Error handler:
	if (!HasResolved)
	{
		a_Callbacks.OnError(EAI_NONAME, ErrorString(EAI_NONAME));
	}
	else
	{
		a_Callbacks.OnFinished();
	}
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




