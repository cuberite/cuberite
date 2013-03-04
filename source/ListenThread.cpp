
// ListenThread.cpp

// Implements the cListenThread class representing the thread that listens for client connections

#include "Globals.h"
#include "ListenThread.h"





cListenThread::cListenThread(cCallback & a_Callback) :
	super("ListenThread"),
	m_Callback(a_Callback)
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
	ASSERT(!m_Sockets.empty());  // Has Initialize() been called?
	
	return super::Start();
}





void cListenThread::Stop(void)
{
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
	ASSERT(m_Sockets.empty());  // Must not be started
	
	m_ShouldReuseAddr = a_Reuse;
}





bool cListenThread::CreateSockets(const AString & a_PortsString)
{
	AStringVector Ports = StringSplit(a_PortsString, ",");
	
	if (Ports.empty())
	{
		return false;
	}
	
	for (AStringVector::const_iterator itr = Ports.begin(), end = Ports.end(); itr != end; ++itr)
	{
		int Port = atoi(Trim(*itr).c_str());
		if ((Port <= 0) || (Port > 65535))
		{
			LOGWARNING("Invalid port specified: \"%s\".", Trim(*itr).c_str());
			continue;
		}
		m_Sockets.push_back(cSocket::CreateSocket());
		if (!m_Sockets.back().IsValid())
		{
			LOGERROR("Cannot create listening socket for port %d: \"%s\"", Port, cSocket::GetLastErrorString().c_str());
			m_Sockets.pop_back();
			continue;
		}

		if (m_ShouldReuseAddr)
		{
			if (m_Sockets.back().SetReuseAddress() == -1)
			{
				LOG("Port %d cannot reuse addr, syscall failed: \"%s\".", Port, cSocket::GetLastErrorString().c_str());
			}
		}
		m_Sockets.back().BindToAny(Port);
		m_Sockets.back().Listen();
		LOGD("Port %d is open for connections", Port);
	}  // for itr - Ports[]
	
	return !(m_Sockets.empty());
}





void cListenThread::Execute(void)
{
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
				cSocket Client = itr->Accept();
				m_Callback.OnConnectionAccepted(Client);
			}
		}  // for itr - m_Sockets[]
	}  // while (!m_ShouldTerminate)
}




