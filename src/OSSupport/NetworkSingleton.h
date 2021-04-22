
// NetworkSingleton.h

// Declares the cNetworkSingleton class representing the storage for global data pertaining to network API
// such as a list of all connections, all listening sockets and the LibEvent dispatch thread.

// This is an internal header, no-one outside OSSupport should need to include it; use Network.h instead;
// the only exception being the main app entrypoint that needs to call Terminate before quitting.





#pragma once

#include <event2/event.h>
#include "NetworkLookup.h"
#include "CriticalSection.h"
#include "Event.h"





// fwd:
struct event_base;
class cTCPLink;
typedef std::shared_ptr<cTCPLink> cTCPLinkPtr;
typedef std::vector<cTCPLinkPtr> cTCPLinkPtrs;
class cServerHandle;
typedef std::shared_ptr<cServerHandle> cServerHandlePtr;
typedef std::vector<cServerHandlePtr> cServerHandlePtrs;





class cNetworkSingleton
{
public:
	cNetworkSingleton();
	~cNetworkSingleton() noexcept(false);

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

	/** Returns the thread used to perform hostname and IP lookups */
	cNetworkLookup & GetLookupThread() { return m_LookupThread; }

	/** Adds the specified link to m_Connections.
	Used by the underlying link implementation when a new link is created. */
	void AddLink(const cTCPLinkPtr & a_Link);

	/** Removes the specified link from m_Connections.
	Used by the underlying link implementation when the link is closed / errored. */
	void RemoveLink(const cTCPLink * a_Link);

	/** Adds the specified link to m_Servers.
	Used by the underlying server handle implementation when a new listening server is created.
	Only servers that succeed in listening are added. */
	void AddServer(const cServerHandlePtr & a_Server);

	/** Removes the specified server from m_Servers.
	Used by the underlying server handle implementation when the server is closed. */
	void RemoveServer(const cServerHandle * a_Server);

protected:

	/** The main LibEvent container for driving the event loop. */
	event_base * m_EventBase;

	/** Container for all client connections, including ones with pending-connect. */
	cTCPLinkPtrs m_Connections;

	/** Container for all servers that are currently active. */
	cServerHandlePtrs m_Servers;

	/** Mutex protecting all containers against multithreaded access. */
	cCriticalSection m_CS;

	/** Set to true if Terminate has been called. */
	std::atomic<bool> m_HasTerminated;

	/** The thread in which the main LibEvent loop runs. */
	std::thread m_EventLoopThread;

	/** Event that is signalled once the startup is finished and the LibEvent loop is running. */
	cEvent m_StartupEvent;

	/** The thread on which hostname and ip address lookup is performed. */
	cNetworkLookup m_LookupThread;


	/** Converts LibEvent-generated log events into log messages in MCS log. */
	static void LogCallback(int a_Severity, const char * a_Msg);

	/** Implements the thread that runs LibEvent's event dispatcher loop. */
	static void RunEventLoop(cNetworkSingleton * a_Self);

	/** Callback called by LibEvent when the event loop is started. */
	static void SignalizeStartup(evutil_socket_t a_Socket, short a_Events, void * a_Self);
};









