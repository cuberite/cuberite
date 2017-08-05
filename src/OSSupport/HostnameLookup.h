
// HostnameLookup.h

// Declares the cHostnameLookup class representing an in-progress hostname-to-IP lookup

// This is an internal header, no-one outside OSSupport should need to include it; use Network.h instead





#pragma once

#include "Network.h"





/** Holds information about an in-progress Hostname-to-IP lookup. */
class cHostnameLookup
{
public:
	/** Creates a lookup object and schedules the lookup. */
	static void Lookup(const AString & a_Hostname, cNetwork::cResolveNameCallbacksPtr a_Callbacks);

protected:

	/** Creates the lookup object. Doesn't start the lookup yet. */
	cHostnameLookup(const AString & a_Hostname, cNetwork::cResolveNameCallbacksPtr a_Callbacks);

	/** The callbacks to call for resolved names / errors. */
	cNetwork::cResolveNameCallbacksPtr m_Callbacks;

	/** The hostname that was queried (needed for the callbacks). */
	AString m_Hostname;

	void Callback(int a_ErrCode, struct addrinfo * a_Addr);
};
typedef std::shared_ptr<cHostnameLookup> cHostnameLookupPtr;
typedef std::vector<cHostnameLookupPtr> cHostnameLookupPtrs;





