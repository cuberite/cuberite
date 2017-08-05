
// IPLookup.h

// Declares the cIPLookup class representing an IP-to-hostname lookup in progress.

// This is an internal header, no-one outside OSSupport should need to include it; use Network.h instead





#pragma once

#include "Network.h"





/** Holds information about an in-progress IP-to-Hostname lookup. */
class cIPLookup
{
public:

	/** Creates a lookup object and schedules the lookup. */
	static void Lookup(const AString & a_IP, cNetwork::cResolveNameCallbacksPtr a_Callbacks);

protected:

	/** The callbacks to call for resolved names / errors. */
	cNetwork::cResolveNameCallbacksPtr m_Callbacks;

	/** The IP that was queried (needed for the callbacks). */
	AString m_IP;

	/** Creates the lookup object. Doesn't start the lookup yet. */
	cIPLookup(const AString & a_IP, cNetwork::cResolveNameCallbacksPtr a_Callbacks);

	/** Callback that is called by LibEvent when there's an event for the request. */
	void Callback(int a_Result, const char * a_Address);
};
typedef std::shared_ptr<cIPLookup> cIPLookupPtr;
typedef std::vector<cIPLookupPtr> cIPLookupPtrs;





