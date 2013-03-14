
// ListenThread.cpp

// Implements the cListenThread class representing the thread that listens for client connections

#include "Globals.h"
#include "ListenThread.h"





cListenThread::cListenThread(cCallback & a_Callback, cSocket::eFamily a_Family) :
	super("ListenThread"),
	m_Callback(a_Callback),
	m_Family(a_Family)
{
}





cListenThread::~cListenThread()
{
	// TODO
}





bool cListenThread::Initialize(const AString & a_PortsString)
{
	ASSERT(m_Sockets.empty());  // Not yet started
	
	if (!CreateSockets(a_PortsString))
	{
		return false;
	}
	
	return true;
}





bool cListenThread::Start(void)
{
	if (m_Sockets.empty())
	{
		// There are no sockets listening, either forgotten to initialize or the user specified no listening ports
		// Report as successful, though
		return true;
	}
	return super::Start();
}





void cListenThread::Stop(void)
{
	if (m_Sockets.empty())
	{
		// No sockets means no thread was running in the first place
		return;
	}

	m_ShouldTerminate = true;
	
	// Close one socket to wake the thread up from the select() call
	m_Sockets[0].CloseSocket();
	
	// Wait for the thread to finish
	super::Wait();
	
	// Clean up all sockets
	m_Sockets.clear();
}





void cListenThread::SetReuseAddr(bool a_Reuse)
{
	ASSERT(m_Sockets.empty());  // Must not have been Initialize()d yet
	
	m_ShouldReuseAddr = a_Reuse;
}





bool cListenThread::CreateSockets(const AString & a_PortsString)
{
	AStringVector Ports = StringSplitAndTrim(a_PortsString, ",");
	
	if (Ports.empty())
	{
		return false;
	}
	
	const char * FamilyStr = "";
	switch (m_Family)
	{
		case cSocket::IPv4: FamilyStr = "IPv4: "; break;
		case cSocket::IPv6: FamilyStr = "IPv6: "; break;
		default:
		{
			ASSERT(!"Unknown address family");
			break;
		}
	}
	
	for (AStringVector::const_iterator itr = Ports.begin(), end = Ports.end(); itr != end; ++itr)
	{
		int Port = atoi(itr->c_str());
		if ((Port <= 0) || (Port > 65535))
		{
			LOGWARNING("Invalid port specified: \"%s\".", itr->c_str());
			continue;
		}
		m_Sockets.push_back(cSocket::CreateSocket(m_Family));
		if (!m_Sockets.back().IsValid())
		{
			LOGERROR("Cannot create listening socket for port %d: \"%s\"", Port, cSocket::GetLastErrorString().c_str());
			m_Sockets.pop_back();
			continue;
		}

		if (m_ShouldReuseAddr)
		{
			if (!m_Sockets.back().SetReuseAddress())
			{
				LOG("Port %d cannot reuse addr, syscall failed: \"%s\".", Port, cSocket::GetLastErrorString().c_str());
			}
		}
		
		// Bind to port:
		bool res = false;
		switch (m_Family)
		{
			case cSocket::IPv4: res = m_Sockets.back().BindToAnyIPv4(Port); break;
			case cSocket::IPv6: res = m_Sockets.back().BindToAnyIPv6(Port); break;
			default:
			{
				ASSERT(!"Unknown address family");
				res = false;
			}
		}
		if (!res)
		{
			LOGWARNING("Cannot bind port %d: \"%s\".", Port, cSocket::GetLastErrorString().c_str());
			m_Sockets.pop_back();
			continue;
		}
		
		if (!m_Sockets.back().Listen())
		{
			LOGWARNING("Cannot listen on port %d: \"%s\".", Port, cSocket::GetLastErrorString().c_str());
			m_Sockets.pop_back();
			continue;
		}
		
		LOGINFO("%sPort %d is open for connections", FamilyStr, Port);
	}  // for itr - Ports[]
	
	return !(m_Sockets.empty());
}





void cListenThread::Execute(void)
{
	if (m_Sockets.empty())
	{
		LOGD("Empty cListenThread, ending thread now.");
		return;
	}
	
	// Find the highest socket number:
	cSocket::xSocket Highest = m_Sockets[0].GetSocket();
	for (cSockets::iterator itr = m_Sockets.begin(), end = m_Sockets.end(); itr != end; ++itr)
	{
		if (itr->GetSocket() > Highest)
		{
			Highest = itr->GetSocket();
		}
	}  // for itr - m_Sockets[]

	while (!m_ShouldTerminate)
	{
		// Put all sockets into a FD set:
		fd_set fdRead;
		FD_ZERO(&fdRead);
		for (cSockets::iterator itr = m_Sockets.begin(), end = m_Sockets.end(); itr != end; ++itr)
		{
			FD_SET(itr->GetSocket(), &fdRead);
		}  // for itr - m_Sockets[]
		
		if (select(Highest + 1, &fdRead, NULL, NULL, NULL) == -1)
		{
			LOG("select(R) call failed in cListenThread: \"%s\"", cSocket::GetLastErrorString().c_str());
			continue;
		}
		for (cSockets::iterator itr = m_Sockets.begin(), end = m_Sockets.end(); itr != end; ++itr)
		{
			if (FD_ISSET(itr->GetSocket(), &fdRead))
			{
				cSocket Client = (m_Family == cSocket::IPv4) ? itr->AcceptIPv4() : itr->AcceptIPv6();
				m_Callback.OnConnectionAccepted(Client);
			}
		}  // for itr - m_Sockets[]
	}  // while (!m_ShouldTerminate)
}




