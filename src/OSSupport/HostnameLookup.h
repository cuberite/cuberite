
// HostnameLookup.h

// Declares the cHostnameLookup class representing an in-progress hostname-to-IP lookup

// This is an internal header, no-one outside OSSupport should need to include it; use Network.h instead





#pragma once

#include "Network.h"
#include <asio/ip/tcp.hpp>





/** Holds information about an in-progress Hostname-to-IP lookup. */
class cHostnameLookup
{
public:
	/** Creates a lookup object and schedules the lookup. */
	static void Lookup(const AString & a_Hostname, cNetwork::cResolveNameCallbacksPtr a_Callbacks);

private:

	static void Callback(cNetwork::cResolveNameCallbacks & a_Callbacks, const asio::ip::tcp::resolver::results_type & a_Addr);
};
