
// ListenThread.cpp

// Implements the cListenThread class representing the thread that listens for client connections

#include "Globals.h"
#include "ListenThread.h"
#include "SocketSet.h"





cListenThread::cListenThread(cCallback & a_Callback,  const AString & a_ServiceName) :
	super(Printf("ListenThread %s", a_ServiceName.c_str())),
	m_Callback(a_Callback),
	m_ShouldReuseAddr(false),
	m_ServiceName(a_ServiceName)
{
}





cListenThread::~cListenThread()
{
	Stop();
}





bool cListenThread::Initialize(const AString & a_DualPortsString, AString a_v4PortsString, AString a_v6PortsString)
{
	ASSERT(m_Sockets.empty());  // Not yet started
	if(cSocket::SupportsFamily(cSocket::IPDual))
	{
		if (a_DualPortsString != "" && !CreateSockets(cSocket::IPDual, a_DualPortsString))
		{
			return false;
		}
	}
	else
	{
		// Listen to ports with seperate sockets
		a_v4PortsString += "," + a_DualPortsString;
		a_v6PortsString += "," + a_DualPortsString;
	}
	
	
	if (a_v4PortsString != "" && !CreateSockets(cSocket::IPv4, a_v4PortsString))
	{
		return false;
	}
	
	if (a_v6PortsString != "" && !CreateSockets(cSocket::IPv6, a_v6PortsString))
	{
		return false;
	}
	
	if (a_DualPortsString == "" && a_v4PortsString == "" && a_v6PortsString == "")
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





bool cListenThread::CreateSockets(cSocket::eFamily a_Family, const AString & a_PortsString)
{
	AStringVector Ports = StringSplitAndTrim(a_PortsString, ",");
	
	if (Ports.empty())
	{
		return false;
	}
	
	for (AStringVector::const_iterator itr = Ports.begin(), end = Ports.end(); itr != end; ++itr)
	{
		int Port = atoi(itr->c_str());
		if ((Port <= 0) || (Port > 65535))
		{
			LOGWARNING("%s: Invalid port specified: \"%s\".", m_ServiceName.c_str(), itr->c_str());
			continue;
		}
		m_Sockets.push_back(cSocket::CreateSocket(a_Family));
		if (!m_Sockets.back().IsValid())
		{
			LOGWARNING("%s: Cannot create listening socket for port %d: \"%s\"", m_ServiceName.c_str(), Port, cSocket::GetLastErrorString().c_str());
			m_Sockets.pop_back();
			continue;
		}

		if (m_ShouldReuseAddr)
		{
			if (!m_Sockets.back().SetReuseAddress())
			{
				LOG("%s: Port %d cannot reuse addr, syscall failed: \"%s\".", m_ServiceName.c_str(), Port, cSocket::GetLastErrorString().c_str());
			}
		}
		
		// Bind to port:
		bool res = false;
		res = m_Sockets.back().BindToAny(Port);
		if (!res)
		{
			LOGWARNING("%s: Cannot bind port %d: \"%s\".", m_ServiceName.c_str(), Port, cSocket::GetLastErrorString().c_str());
			m_Sockets.pop_back();
			continue;
		}
		
		if (!m_Sockets.back().Listen())
		{
			LOGWARNING("%s: Cannot listen on port %d: \"%s\".", m_ServiceName.c_str(), Port, cSocket::GetLastErrorString().c_str());
			m_Sockets.pop_back();
			continue;
		}
		
		LOGINFO("%s: Port %d is open for connections", m_ServiceName.c_str(), Port);
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
	
	cSocketSet SocketSet(m_Sockets);

	while (!m_ShouldTerminate)
	{
		
		if (!SocketSet.SelectRead(1))
		{
			LOG("select(R) call failed in cListenThread: \"%s\"", cSocket::GetLastErrorString().c_str());
			continue;
		}
		for (cSocketSet::cEventIterator itr = SocketSet.Eventbegin(), end = SocketSet.Eventend(); itr != end; ++itr)
		{
			if (itr->IsValid())
			{
				cSocket Client = itr->Accept();
				if (Client.IsValid())
				{
					m_Callback.OnConnectionAccepted(Client);
				}
			}
		}  // for itr - m_Sockets[]
		SocketSet.Reset();
	}  // while (!m_ShouldTerminate)
}




