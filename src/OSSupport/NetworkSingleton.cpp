
// NetworkSingleton.cpp

// Implements the cNetworkSingleton class representing the storage for global data pertaining to network API
// such as a list of all connections, all listening sockets and the LibEvent dispatch thread.

#include "Globals.h"
#include "NetworkSingleton.h"
#include <event2/event.h>
#include <event2/thread.h>
#include <event2/bufferevent.h>
#include <event2/dns.h>
#include <event2/listener.h>
#include "IPLookup.h"
#include "HostnameLookup.h"





cNetworkSingleton::cNetworkSingleton() :
	m_HasTerminated(true)
{
}





cNetworkSingleton::~cNetworkSingleton()
{
	// Check that Terminate has been called already:
	ASSERT(m_HasTerminated);
}





cNetworkSingleton & cNetworkSingleton::Get(void)
{
	static cNetworkSingleton Instance;
	return Instance;
}





void cNetworkSingleton::Initialise(void)
{
	// Windows: initialize networking:
	#ifdef _WIN32
		WSADATA wsaData;
		memset(&wsaData, 0, sizeof(wsaData));
		int res = WSAStartup (MAKEWORD(2, 2), &wsaData);
		if (res != 0)
		{
			int err = WSAGetLastError();
			LOGWARNING("WSAStartup failed: %d, WSAGLE = %d (%s)", res, err, evutil_socket_error_to_string(err));
			exit(1);
		}
	#endif  // _WIN32

	// Initialize LibEvent logging:
	event_set_log_callback(LogCallback);

	// Initialize threading:
	#if defined(EVTHREAD_USE_WINDOWS_THREADS_IMPLEMENTED)
		evthread_use_windows_threads();
	#elif defined(EVTHREAD_USE_PTHREADS_IMPLEMENTED)
		evthread_use_pthreads();
	#else
		#error No threading implemented for EVTHREAD
	#endif

	// Create the main event_base:
	m_EventBase = event_base_new();
	if (m_EventBase == nullptr)
	{
		LOGERROR("Failed to initialize LibEvent. The server will now terminate.");
		abort();
	}

	// Create the DNS lookup helper:
	m_DNSBase = evdns_base_new(m_EventBase, 1);
	if (m_DNSBase == nullptr)
	{
		LOGERROR("Failed to initialize LibEvent's DNS subsystem. The server will now terminate.");
		abort();
	}

	// Create the event loop thread:
	m_EventLoopThread = std::thread(RunEventLoop, this);
	m_HasTerminated = false;
}





void cNetworkSingleton::Terminate(void)
{
	ASSERT(!m_HasTerminated);

	// Wait for the LibEvent event loop to terminate:
	event_base_loopbreak(m_EventBase);
	m_EventLoopThread.join();

	// Remove all objects:
	{
		cCSLock Lock(m_CS);
		m_Connections.clear();
		m_Servers.clear();
		m_HostnameLookups.clear();
		m_IPLookups.clear();
	}

	// Free the underlying LibEvent objects:
	evdns_base_free(m_DNSBase, true);
	event_base_free(m_EventBase);

	libevent_global_shutdown();

	// Set the HasTerminated flag:
	// (Only set the flag after everything has been removed, to avoid the random failures in the Google-test, caused by links terminating after this flag was set)
	m_HasTerminated = true;
}





void cNetworkSingleton::LogCallback(int a_Severity, const char * a_Msg)
{
	switch (a_Severity)
	{
		case _EVENT_LOG_DEBUG: LOGD      ("LibEvent: %s", a_Msg); break;
		case _EVENT_LOG_MSG:   LOG       ("LibEvent: %s", a_Msg); break;
		case _EVENT_LOG_WARN:  LOGWARNING("LibEvent: %s", a_Msg); break;
		case _EVENT_LOG_ERR:   LOGERROR  ("LibEvent: %s", a_Msg); break;
		default:
		{
			LOGWARNING("LibEvent: Unknown log severity (%d): %s", a_Severity, a_Msg);
			break;
		}
	}
}





void cNetworkSingleton::RunEventLoop(cNetworkSingleton * a_Self)
{
	event_base_loop(a_Self->m_EventBase, EVLOOP_NO_EXIT_ON_EMPTY);
}





void cNetworkSingleton::AddHostnameLookup(cHostnameLookupPtr a_HostnameLookup)
{
	ASSERT(!m_HasTerminated);
	cCSLock Lock(m_CS);
	m_HostnameLookups.push_back(a_HostnameLookup);
}





void cNetworkSingleton::RemoveHostnameLookup(const cHostnameLookup * a_HostnameLookup)
{
	ASSERT(!m_HasTerminated);
	cCSLock Lock(m_CS);
	for (auto itr = m_HostnameLookups.begin(), end = m_HostnameLookups.end(); itr != end; ++itr)
	{
		if (itr->get() == a_HostnameLookup)
		{
			m_HostnameLookups.erase(itr);
			return;
		}
	}  // for itr - m_HostnameLookups[]
}





void cNetworkSingleton::AddIPLookup(cIPLookupPtr a_IPLookup)
{
	ASSERT(!m_HasTerminated);
	cCSLock Lock(m_CS);
	m_IPLookups.push_back(a_IPLookup);
}





void cNetworkSingleton::RemoveIPLookup(const cIPLookup * a_IPLookup)
{
	ASSERT(!m_HasTerminated);
	cCSLock Lock(m_CS);
	for (auto itr = m_IPLookups.begin(), end = m_IPLookups.end(); itr != end; ++itr)
	{
		if (itr->get() == a_IPLookup)
		{
			m_IPLookups.erase(itr);
			return;
		}
	}  // for itr - m_IPLookups[]
}





void cNetworkSingleton::AddLink(cTCPLinkImplPtr a_Link)
{
	ASSERT(!m_HasTerminated);
	cCSLock Lock(m_CS);
	m_Connections.push_back(a_Link);
}





void cNetworkSingleton::RemoveLink(const cTCPLinkImpl * a_Link)
{
	ASSERT(!m_HasTerminated);
	cCSLock Lock(m_CS);
	for (auto itr = m_Connections.begin(), end = m_Connections.end(); itr != end; ++itr)
	{
		if (itr->get() == a_Link)
		{
			m_Connections.erase(itr);
			return;
		}
	}  // for itr - m_Connections[]
}





void cNetworkSingleton::AddServer(cServerHandleImplPtr a_Server)
{
	ASSERT(!m_HasTerminated);
	cCSLock Lock(m_CS);
	m_Servers.push_back(a_Server);
}





void cNetworkSingleton::RemoveServer(const cServerHandleImpl * a_Server)
{
	ASSERT(!m_HasTerminated);
	cCSLock Lock(m_CS);
	for (auto itr = m_Servers.begin(), end = m_Servers.end(); itr != end; ++itr)
	{
		if (itr->get() == a_Server)
		{
			m_Servers.erase(itr);
			return;
		}
	}  // for itr - m_Servers[]
}




