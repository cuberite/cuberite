
// NetworkSingleton.h

// Declares the cNetworkSingleton class representing the storage for global data pertaining to network API
// such as a list of all connections, all listening sockets and the LibEvent dispatch thread.

#pragma once

#include "Network.h"
#include "CriticalSection.h"





// fwd:
struct event_base;
struct evdns_base;
class cServerHandleImpl;
class cTCPLinkImpl;
class cHostnameLookup;
class cIPLookup;
typedef SharedPtr<cTCPLinkImpl> cTCPLinkImplPtr;
typedef std::vector<cTCPLinkImplPtr> cTCPLinkImplPtrs;
typedef SharedPtr<cServerHandleImpl> cServerHandleImplPtr;
typedef std::vector<cServerHandleImplPtr> cServerHandleImplPtrs;
typedef SharedPtr<cHostnameLookup> cHostnameLookupPtr;
typedef std::vector<cHostnameLookupPtr> cHostnameLookupPtrs;
typedef SharedPtr<cIPLookup> cIPLookupPtr;
typedef std::vector<cIPLookupPtr> cIPLookupPtrs;





class cNetworkSingleton
{
public:
	/** Returns the singleton instance of this class */
	static cNetworkSingleton & Get(void);


	// The following functions are implementations for the cNetwork class

	/** Queues a DNS query to resolve the specified hostname to IP address.
	Calls one of the callbacks when the resolving succeeds, or when it fails.
	Returns true if queueing was successful, false if not.
	Note that the return value doesn't report the success of the actual lookup; the lookup happens asynchronously on the background.
	TODO: Move this out into a separate file with cHostnameLookup. */
	bool HostnameToIP(
		const AString & a_Hostname,
		cNetwork::cResolveNameCallbacksPtr a_Callbacks
	);


	/** Queues a DNS query to resolve the specified IP address to a hostname.
	Calls one of the callbacks when the resolving succeeds, or when it fails.
	Returns true if queueing was successful, false if not.
	Note that the return value doesn't report the success of the actual lookup; the lookup happens asynchronously on the background.
	TODO: Move this out into a separate file with cIPLookup. */
	bool IPToHostName(
		const AString & a_IP,
		cNetwork::cResolveNameCallbacksPtr a_Callbacks
	);

	/** Returns the main LibEvent handle for event registering. */
	event_base * GetEventBase(void) { return m_EventBase; }

	/** Returns the LibEvent handle for DNS lookups. */
	evdns_base * GetDNSBase(void) { return m_DNSBase; }

	/** Removes the specified hostname lookup from m_HostnameLookups.
	Used by the underlying lookup implementation when the lookup is finished. */
	void RemoveHostnameLookup(const cHostnameLookup * a_HostnameLookup);

	/** Removes the specified IP lookup from m_IPLookups.
	Used by the underlying lookup implementation when the lookup is finished. */
	void RemoveIPLookup(const cIPLookup * a_IPLookup);

	/** Adds the specified link to m_Connections.
	Used by the underlying link implementation when a new link is created. */
	void AddLink(cTCPLinkImplPtr a_Link);

	/** Removes the specified link from m_Connections.
	Used by the underlying link implementation when the link is closed / errored. */
	void RemoveLink(const cTCPLinkImpl * a_Link);

	/** Adds the specified link to m_Servers.
	Used by the underlying server handle implementation when a new listening server is created.
	Only servers that succeed in listening are added. */
	void AddServer(cServerHandleImplPtr a_Server);

	/** Removes the specified server from m_Servers.
	Used by the underlying server handle implementation when the server is closed. */
	void RemoveServer(const cServerHandleImpl * a_Server);

protected:

	/** The main LibEvent container for driving the event loop. */
	event_base * m_EventBase;

	/** The LibEvent handle for doing DNS lookups. */
	evdns_base * m_DNSBase;

	/** Container for all client connections, including ones with pending-connect. */
	cTCPLinkImplPtrs m_Connections;

	/** Container for all servers that are currently active. */
	cServerHandleImplPtrs m_Servers;

	/** Container for all pending hostname lookups. */
	cHostnameLookupPtrs m_HostnameLookups;

	/** Container for all pending IP lookups. */
	cIPLookupPtrs m_IPLookups;

	/** Mutex protecting all containers against multithreaded access. */
	cCriticalSection m_CS;


	/** Initializes the LibEvent internals. */
	cNetworkSingleton(void);

	/** Converts LibEvent-generated log events into log messages in MCS log. */
	static void LogCallback(int a_Severity, const char * a_Msg);

	/** Implements the thread that runs LibEvent's event dispatcher loop. */
	static void RunEventLoop(cNetworkSingleton * a_Self);
};









