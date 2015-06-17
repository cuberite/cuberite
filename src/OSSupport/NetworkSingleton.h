
// NetworkSingleton.h

// Declares the cNetworkSingleton class representing the storage for global data pertaining to network API
// such as a list of all connections, all listening sockets and the LibEvent dispatch thread.

// This is an internal header, no-one outside OSSupport should need to include it; use Network.h instead;
// the only exception being the main app entrypoint that needs to call Terminate before quitting.





#pragma once

#include "Network.h"
#include "CriticalSection.h"
#include "Event.h"





// fwd:
struct event_base;
struct evdns_base;
class cTCPLinkImpl;
typedef SharedPtr<cTCPLinkImpl> cTCPLinkImplPtr;
typedef std::vector<cTCPLinkImplPtr> cTCPLinkImplPtrs;
class cServerHandleImpl;
typedef SharedPtr<cServerHandleImpl> cServerHandleImplPtr;
typedef std::vector<cServerHandleImplPtr> cServerHandleImplPtrs;
class cHostnameLookup;
typedef SharedPtr<cHostnameLookup> cHostnameLookupPtr;
typedef std::vector<cHostnameLookupPtr> cHostnameLookupPtrs;
class cIPLookup;
typedef SharedPtr<cIPLookup> cIPLookupPtr;
typedef std::vector<cIPLookupPtr> cIPLookupPtrs;





class cNetworkSingleton
{
public:
	cNetworkSingleton();
	~cNetworkSingleton();

	/** Returns the singleton instance of this class */
	static cNetworkSingleton & Get(void);

	/** Initialises all network-related threads.
	To be called on first run or after app restart. */
	void Initialise(void);

	/** Terminates all network-related threads.
	To be used only on app shutdown or restart.
	MSVC runtime requires that the LibEvent networking be shut down before the main() function is exitted; this is the way to do it. */
	void Terminate(void);

	/** Returns the main LibEvent handle for event registering. */
	event_base * GetEventBase(void) { return m_EventBase; }

	/** Returns the LibEvent handle for DNS lookups. */
	evdns_base * GetDNSBase(void) { return m_DNSBase; }

	/** Adds the specified hostname lookup to m_HostnameLookups.
	Used by the underlying lookup implementation when a new lookup is initiated. */
	void AddHostnameLookup(cHostnameLookupPtr a_HostnameLookup);

	/** Removes the specified hostname lookup from m_HostnameLookups.
	Used by the underlying lookup implementation when the lookup is finished. */
	void RemoveHostnameLookup(const cHostnameLookup * a_HostnameLookup);

	/** Adds the specified IP lookup to M_IPLookups.
	Used by the underlying lookup implementation when a new lookup is initiated. */
	void AddIPLookup(cIPLookupPtr a_IPLookup);

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

	/** Set to true if Terminate has been called. */
	volatile bool m_HasTerminated;

	/** The thread in which the main LibEvent loop runs. */
	std::thread m_EventLoopThread;

	/** Converts LibEvent-generated log events into log messages in MCS log. */
	static void LogCallback(int a_Severity, const char * a_Msg);

	/** Implements the thread that runs LibEvent's event dispatcher loop. */
	static void RunEventLoop(cNetworkSingleton * a_Self);
};









