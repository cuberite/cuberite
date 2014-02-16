
// ListenThread.cpp

// Implements the cListenThread class representing the thread that listens for client connections

#include "Globals.h"
#include "ListenThread.h"





cListenThread::cListenThread(cCallback & a_Callback,  const AString & a_ServiceName) :
	super(Printf("ListenThread %s", a_ServiceName.c_str())),
	m_Callback(a_Callback),
	m_Family(cSocket::INVALID_PROTOCOL),
	m_ShouldReuseAddr(false),
	m_ServiceName(a_ServiceName)
{
}





cListenThread::~cListenThread()
{
	Stop();
}





bool cListenThread::Initialize(cSocket::eFamily a_Family, const AString & a_PortsString)
{
	ASSERT(m_Sockets.empty());  // Not yet started
	m_Family = a_Family;
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
	
	// Close all the listening sockets:
	for (cSockets::iterator itr = m_Sockets.begin() + 1, end = m_Sockets.end(); itr != end; ++itr)
	{
		itr->CloseSocket();
	}  // for itr - m_Sockets[]
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
	
	AString FamilyStr = m_ServiceName;
	switch (m_Family)
	{
		case cSocket::IPv4:
		{
			FamilyStr.append(" IPv4");
			break;
		}
		case cSocket::IPv6:
		{
			FamilyStr.append(" IPv6");
			break;
		}
		case cSocket::IPDual:
		{
			FamilyStr.append(" IP Dual Stack");
			break;
		}
		case cSocket::INVALID_PROTOCOL:
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
			LOGWARNING("%s: Invalid port specified: \"%s\".", FamilyStr.c_str(), itr->c_str());
			continue;
		}
		m_Sockets.push_back(cSocket::CreateSocket(m_Family));
		if (!m_Sockets.back().IsValid())
		{
			LOGWARNING("%s: Cannot create listening socket for port %d: \"%s\"", FamilyStr.c_str(), Port, cSocket::GetLastErrorString().c_str());
			m_Sockets.pop_back();
			continue;
		}

		if (m_ShouldReuseAddr)
		{
			if (!m_Sockets.back().SetReuseAddress())
			{
				LOG("%s: Port %d cannot reuse addr, syscall failed: \"%s\".", FamilyStr.c_str(), Port, cSocket::GetLastErrorString().c_str());
			}
		}
		
		// Bind to port:
		bool res = false;
		switch (m_Family)
		{
			case cSocket::IPv4:
			case cSocket::IPv6:
			case cSocket::IPDual:
				res = m_Sockets.back().BindToAny(Port);
				break;
			case cSocket::INVALID_PROTOCOL:
				ASSERT(!"Invalid Protocol");
		}
		if (!res)
		{
			LOGWARNING("%s: Cannot bind port %d: \"%s\".", FamilyStr.c_str(), Port, cSocket::GetLastErrorString().c_str());
			m_Sockets.pop_back();
			continue;
		}
		
		if (!m_Sockets.back().Listen())
		{
			LOGWARNING("%s: Cannot listen on port %d: \"%s\".", FamilyStr.c_str(), Port, cSocket::GetLastErrorString().c_str());
			m_Sockets.pop_back();
			continue;
		}
		
		LOGINFO("%s: Port %d is open for connections", FamilyStr.c_str(), Port);
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
		
		timeval tv;  // On Linux select() doesn't seem to wake up when socket is closed, so let's kinda busy-wait:
		tv.tv_sec = 1;
		tv.tv_usec = 0;
		if (select(Highest + 1, &fdRead, NULL, NULL, &tv) == -1)
		{
			LOG("select(R) call failed in cListenThread: \"%s\"", cSocket::GetLastErrorString().c_str());
			continue;
		}
		for (cSockets::iterator itr = m_Sockets.begin(), end = m_Sockets.end(); itr != end; ++itr)
		{
			if (itr->IsValid() && FD_ISSET(itr->GetSocket(), &fdRead))
			{
				cSocket Client = itr->Accept();
				if (Client.IsValid())
				{
					m_Callback.OnConnectionAccepted(Client);
				}
			}
		}  // for itr - m_Sockets[]
	}  // while (!m_ShouldTerminate)
}




