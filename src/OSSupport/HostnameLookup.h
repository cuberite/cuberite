
// HostnameLookup.h

// Declares the cHostnameLookup class representing an in-progress hostname-to-IP lookup

// This is an internal header, no-one outside OSSupport should need to include it; use Network.h instead





#pragma once

#include "Network.h"
#include <event2/util.h>





/** Holds information about an in-progress Hostname-to-IP lookup. */
class cHostnameLookup
{
public:
	/** Creates the lookup object. Doesn't start the lookup yet. */
	cHostnameLookup(cNetwork::cResolveNameCallbacksPtr a_Callbacks);

	/** Starts the lookup. */
	void Lookup(const AString & a_Hostname);

protected:

	/** The callbacks to call for resolved names / errors. */
	cNetwork::cResolveNameCallbacksPtr m_Callbacks;

	/** The hostname that was queried (needed for the callbacks). */
	AString m_Hostname;

	static void Callback(int a_ErrCode, struct evutil_addrinfo * a_Addr, void * a_Self);
};
typedef SharedPtr<cHostnameLookup> cHostnameLookupPtr;
typedef std::vector<cHostnameLookupPtr> cHostnameLookupPtrs;





