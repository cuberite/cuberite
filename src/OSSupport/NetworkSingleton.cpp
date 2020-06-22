
// NetworkSingleton.cpp

// Implements the cNetworkSingleton class representing the storage for global data pertaining to network API
// such as a list of all connections, all listening sockets and the LibEvent dispatch thread.

#include "Globals.h"
#include "NetworkSingleton.h"
#include "Network.h"
#include <event2/thread.h>
#include <event2/bufferevent.h>
#include <event2/listener.h>





cNetworkSingleton::cNetworkSingleton() :
	m_HasTerminated(true)
{
}





cNetworkSingleton::~cNetworkSingleton() noexcept(false)
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
	// Start the lookup thread
	m_LookupThread.Start();

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
	event_config * config = event_config_new();
	event_config_set_flag(config, EVENT_BASE_FLAG_STARTUP_IOCP);
	m_EventBase = event_base_new_with_config(config);
	if (m_EventBase == nullptr)
	{
		LOGERROR("Failed to initialize LibEvent. The server will now terminate.");
		abort();
	}
	event_config_free(config);

	// Create the event loop thread:
	m_HasTerminated = false;
	m_EventLoopThread = std::thread(RunEventLoop, this);
	m_StartupEvent.Wait();  // Wait for the LibEvent loop to actually start running (otherwise calling Terminate too soon would hang, see #3228)
}





void cNetworkSingleton::Terminate(void)
{
	ASSERT(!m_HasTerminated);

	// Wait for the lookup thread to stop
	m_LookupThread.Stop();

	// Wait for the LibEvent event loop to terminate:
	event_base_loopbreak(m_EventBase);
	m_EventLoopThread.join();

	// Close all open connections:
	{
		cCSLock Lock(m_CS);
		// Must take copies because Close will modify lists
		auto Conns = m_Connections;
		for (auto & Conn : Conns)
		{
			Conn->Close();
		}

		auto Servers = m_Servers;
		for (auto & Server : Servers)
		{
			Server->Close();
		}

		// Closed handles should have removed themself
		ASSERT(m_Connections.empty());
		ASSERT(m_Servers.empty());
	}

	// Free the underlying LibEvent objects:
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
	auto timer = evtimer_new(a_Self->m_EventBase, SignalizeStartup, a_Self);
	timeval timeout{};  // Zero timeout - execute immediately
	evtimer_add(timer, &timeout);
	event_base_loop(a_Self->m_EventBase, EVLOOP_NO_EXIT_ON_EMPTY);
	event_free(timer);
}





void cNetworkSingleton::SignalizeStartup(evutil_socket_t a_Socket, short a_Events, void * a_Self)
{
	auto self = static_cast<cNetworkSingleton *>(a_Self);
	ASSERT(self != nullptr);
	self->m_StartupEvent.Set();
}





void cNetworkSingleton::AddLink(const cTCPLinkPtr & a_Link)
{
	ASSERT(!m_HasTerminated);
	cCSLock Lock(m_CS);
	m_Connections.push_back(a_Link);
}





void cNetworkSingleton::RemoveLink(const cTCPLink * a_Link)
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





void cNetworkSingleton::AddServer(const cServerHandlePtr & a_Server)
{
	ASSERT(!m_HasTerminated);
	cCSLock Lock(m_CS);
	m_Servers.push_back(a_Server);
}





void cNetworkSingleton::RemoveServer(const cServerHandle * a_Server)
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




