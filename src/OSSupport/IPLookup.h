
// IPLookup.h

// Declares the cIPLookup class representing an IP-to-hostname lookup in progress.

// This is an internal header, no-one outside OSSupport should need to include it; use Network.h instead





#pragma once

#include "Network.h"





/** Holds information about an in-progress IP-to-Hostname lookup. */
class cIPLookup
{
	/** The callbacks to call for resolved names / errors. */
	cNetwork::cResolveNameCallbacksPtr m_Callbacks;

	/** The IP that was queried (needed for the callbacks). */
	AString m_IP;

	static void Callback(int a_Result, char a_Type, int a_Count, int a_Ttl, void * a_Addresses, void * a_Self);

public:
	cIPLookup(const AString & a_IP, cNetwork::cResolveNameCallbacksPtr a_Callbacks);
};
typedef SharedPtr<cIPLookup> cIPLookupPtr;
typedef std::vector<cIPLookupPtr> cIPLookupPtrs;





