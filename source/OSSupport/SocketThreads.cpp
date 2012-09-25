
// cSocketThreads.cpp

// Implements the cSocketThreads class representing the heart of MCS's client networking.
// This object takes care of network communication, groups sockets into threads and uses as little threads as possible for full read / write support
// For more detail, see http://forum.mc-server.org/showthread.php?tid=327

#include "Globals.h"
#include "SocketThreads.h"





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cSocketThreads:

cSocketThreads::cSocketThreads(void)
{
}





cSocketThreads::~cSocketThreads()
{
	for (cSocketThreadList::iterator itr = m_Threads.begin(); itr != m_Threads.end(); ++itr)
	{
		delete *itr;
	}  // for itr - m_Threads[]
	m_Threads.clear();
}






bool cSocketThreads::AddClient(const cSocket & a_Socket, cCallback * a_Client)
{
	// Add a (socket, client) pair for processing, data from a_Socket is to be sent to a_Client
	
	// Try to add to existing threads:
	cCSLock Lock(m_CS);
	for (cSocketThreadList::iterator itr = m_Threads.begin(); itr != m_Threads.end(); ++itr)
	{
		if ((*itr)->IsValid() && (*itr)->HasEmptySlot())
		{
			(*itr)->AddClient(a_Socket, a_Client);
			return true;
		}
	}
	
	// No thread has free space, create a new one:
	LOGD("Creating a new cSocketThread (currently have %d)", m_Threads.size());
	cSocketThread * Thread = new cSocketThread(this);
	if (!Thread->Start())
	{
		// There was an error launching the thread (but it was already logged along with the reason)
		LOGERROR("A new cSocketThread failed to start");
		delete Thread;
		return false;
	}
	Thread->AddClient(a_Socket, a_Client);
	m_Threads.push_back(Thread);
	return true;
}





/*
void cSocketThreads::RemoveClient(const cSocket * a_Socket)
{
	// Remove the socket (and associated client) from processing

	cCSLock Lock(m_CS);
	for (cSocketThreadList::iterator itr = m_Threads.begin(); itr != m_Threads.end(); ++itr)
	{
		if ((*itr)->RemoveSocket(a_Socket))
		{
			return;
		}
	}  // for itr - m_Threads[]
	
	// Cannot assert here, this may actually happen legally, since cClientHandle has to clean up the socket and it may have already closed in the meantime
	// ASSERT(!"Removing an unknown socket");
}
*/





void cSocketThreads::RemoveClient(const cCallback * a_Client)
{
	// Remove the associated socket and the client from processing

	cCSLock Lock(m_CS);
	for (cSocketThreadList::iterator itr = m_Threads.begin(); itr != m_Threads.end(); ++itr)
	{
		if ((*itr)->RemoveClient(a_Client))
		{
			return;
		}
	}  // for itr - m_Threads[]

	ASSERT(!"Removing an unknown client");
}





void cSocketThreads::NotifyWrite(const cCallback * a_Client)
{
	// Notifies the thread responsible for a_Client that the client has something to write

	cCSLock Lock(m_CS);
	for (cSocketThreadList::iterator itr = m_Threads.begin(); itr != m_Threads.end(); ++itr)
	{
		if ((*itr)->NotifyWrite(a_Client))
		{
			return;
		}
	}  // for itr - m_Threads[]
	
	// Cannot assert - this normally happens if a client disconnects and has pending packets, the cServer::cNotifyWriteThread will call this on invalid clients too
	// ASSERT(!"Notifying write to an unknown client");
}





void cSocketThreads::Write(const cCallback * a_Client, const AString & a_Data)
{
	// Puts a_Data into outgoing data queue for a_Client
	cCSLock Lock(m_CS);
	for (cSocketThreadList::iterator itr = m_Threads.begin(); itr != m_Threads.end(); ++itr)
	{
		if ((*itr)->Write(a_Client, a_Data))
		{
			return;
		}
	}  // for itr - m_Threads[]
	
	// This may be perfectly legal, if the socket has been destroyed and the client is finishing up
	// ASSERT(!"Writing to an unknown socket");
}





/// Stops reading from the socket - when this call returns, no more calls to the callbacks are made
void cSocketThreads::StopReading(const cCallback * a_Client)
{
	cCSLock Lock(m_CS);
	for (cSocketThreadList::iterator itr = m_Threads.begin(); itr != m_Threads.end(); ++itr)
	{
		if ((*itr)->StopReading(a_Client))
		{
			return;
		}
	}  // for itr - m_Threads[]
	
	// Cannot assert, this normally happens if the socket is closed before the client deinitializes
	// ASSERT(!"Stopping reading on an unknown client");
}





/// Queues the socket for closing, as soon as its outgoing data is sent
void cSocketThreads::QueueClose(const cCallback * a_Client)
{
	LOGD("QueueClose(client %p)", a_Client);
	
	cCSLock Lock(m_CS);
	for (cSocketThreadList::iterator itr = m_Threads.begin(); itr != m_Threads.end(); ++itr)
	{
		if ((*itr)->QueueClose(a_Client))
		{
			return;
		}
	}  // for itr - m_Threads[]
	
	ASSERT(!"Queueing close of an unknown client");
}





////////////////////////////////////////////////////////////////////////////////
// cSocketThreads::cSocketThread:

cSocketThreads::cSocketThread::cSocketThread(cSocketThreads * a_Parent) :
	cIsThread("cSocketThread"),
	m_Parent(a_Parent),
	m_NumSlots(0)
{
	// Nothing needed yet
}





cSocketThreads::cSocketThread::~cSocketThread()
{
	m_ShouldTerminate = true;

	// Notify the thread:
	ASSERT(m_ControlSocket2.IsValid());
	m_ControlSocket2.Send("a", 1);

	// Wait for the thread to finish:
	Wait();
	
	// Close the control sockets:
	m_ControlSocket1.CloseSocket();
	m_ControlSocket2.CloseSocket();
}





void cSocketThreads::cSocketThread::AddClient(const cSocket & a_Socket, cCallback * a_Client)
{
	ASSERT(m_NumSlots < MAX_SLOTS);  // Use HasEmptySlot() to check before adding
	
	m_Slots[m_NumSlots].m_Client = a_Client;
	m_Slots[m_NumSlots].m_Socket = a_Socket;
	m_Slots[m_NumSlots].m_Outgoing.clear();
	m_Slots[m_NumSlots].m_ShouldClose = false;
	m_Slots[m_NumSlots].m_ShouldCallClient = true;
	m_NumSlots++;
	
	// Notify the thread of the change:
	ASSERT(m_ControlSocket2.IsValid());
	m_ControlSocket2.Send("a", 1);
}





bool cSocketThreads::cSocketThread::RemoveClient(const cCallback * a_Client)
{
	// Returns true if removed, false if not found
	
	if (m_NumSlots == 0)
	{
		return false;
	}
	
	for (int i = m_NumSlots - 1; i >= 0 ; --i)
	{
		if (m_Slots[i].m_Client != a_Client)
		{
			continue;
		}
		
		// Found, remove it:
		m_Slots[i] = m_Slots[--m_NumSlots];
		
		// Notify the thread of the change:
		ASSERT(m_ControlSocket2.IsValid());
		m_ControlSocket2.Send("r", 1);
		return true;
	}  // for i - m_Slots[]
	
	// Not found
	return false;
}





bool cSocketThreads::cSocketThread::RemoveSocket(const cSocket * a_Socket)
{
	// Returns true if removed, false if not found

	for (int i = m_NumSlots - 1; i >= 0 ; --i)
	{
		if (m_Slots[i].m_Socket != *a_Socket)
		{
			continue;
		}
		
		// Found, remove it:
		m_Slots[i] = m_Slots[--m_NumSlots];
		
		// Notify the thread of the change:
		ASSERT(m_ControlSocket2.IsValid());
		m_ControlSocket2.Send("r", 1);
		return true;
	}  // for i - m_Slots[]
	
	// Not found
	return false;
}





bool cSocketThreads::cSocketThread::HasClient(const cCallback * a_Client) const
{
	for (int i = m_NumSlots - 1; i >= 0; --i)
	{
		if (m_Slots[i].m_Client == a_Client)
		{
			return true;
		}
	}  // for i - m_Slots[]
	return false;
}





bool cSocketThreads::cSocketThread::HasSocket(const cSocket * a_Socket) const
{
	for (int i = m_NumSlots - 1; i >= 0; --i)
	{
		if (m_Slots[i].m_Socket == *a_Socket)
		{
			return true;
		}
	}  // for i - m_Slots[]
	return false;
}





bool cSocketThreads::cSocketThread::NotifyWrite(const cCallback * a_Client)
{
	if (HasClient(a_Client))
	{
		// Notify the thread that there's another packet in the queue:
		ASSERT(m_ControlSocket2.IsValid());
		m_ControlSocket2.Send("q", 1);
		return true;
	}
	return false;
}





bool cSocketThreads::cSocketThread::Write(const cCallback * a_Client, const AString & a_Data)
{
	// Returns true if socket handled by this thread
	for (int i = m_NumSlots - 1; i >= 0; --i)
	{
		if (m_Slots[i].m_Client == a_Client)
		{
			m_Slots[i].m_Outgoing.append(a_Data);
			
			// Notify the thread that there's data in the queue:
			ASSERT(m_ControlSocket2.IsValid());
			m_ControlSocket2.Send("q", 1);
			
			return true;
		}
	}  // for i - m_Slots[]
	return false;
}





bool cSocketThreads::cSocketThread::StopReading (const cCallback * a_Client)
{
	// Returns true if client handled by this thread
	for (int i = m_NumSlots - 1; i >= 0; --i)
	{
		if (m_Slots[i].m_Client == a_Client)
		{
			m_Slots[i].m_ShouldCallClient = false;
			return true;
		}
	}  // for i - m_Slots[]
	return false;
}





bool cSocketThreads::cSocketThread::QueueClose(const cCallback * a_Client)
{
	// Returns true if socket handled by this thread
	for (int i = m_NumSlots - 1; i >= 0; --i)
	{
		if (m_Slots[i].m_Client == a_Client)
		{
			m_Slots[i].m_ShouldClose = true;

			// Notify the thread that there's a close queued (in case its conditions are already met):
			ASSERT(m_ControlSocket2.IsValid());
			m_ControlSocket2.Send("c", 1);
			
			return true;
		}
	}  // for i - m_Slots[]
	return false;
}





bool cSocketThreads::cSocketThread::Start(void)
{
	// Create the control socket listener
	m_ControlSocket2 = cSocket::CreateSocket();
	if (!m_ControlSocket2.IsValid())
	{
		LOGERROR("Cannot create a Control socket for a cSocketThread (\"%s\"); continuing, but server may be unreachable from now on.", cSocket::GetLastErrorString().c_str());
		return false;
	}
	cSocket::SockAddr_In Addr;
	Addr.Family = cSocket::ADDRESS_FAMILY_INTERNET;
	Addr.Address = cSocket::INTERNET_ADDRESS_LOCALHOST();
	Addr.Port = 0;  // Any free port is okay
	if (m_ControlSocket2.Bind(Addr) != 0)
	{
		LOGERROR("Cannot bind a Control socket for a cSocketThread (\"%s\"); continuing, but server may be unreachable from now on.", cSocket::GetLastErrorString().c_str());
		m_ControlSocket2.CloseSocket();
		return false;
	}
	if (m_ControlSocket2.Listen(1) != 0)
	{
		LOGERROR("Cannot listen on a Control socket for a cSocketThread (\"%s\"); continuing, but server may be unreachable from now on.", cSocket::GetLastErrorString().c_str());
		m_ControlSocket2.CloseSocket();
		return false;
	}
	if (m_ControlSocket2.GetPort() == 0)
	{
		LOGERROR("Cannot determine Control socket port (\"%s\"); conitnuing, but the server may be unreachable from now on.", cSocket::GetLastErrorString().c_str());
		m_ControlSocket2.CloseSocket();
		return false;
	}

	// Start the thread
	if (!super::Start())
	{
		LOGERROR("Cannot start new cSocketThread");
		m_ControlSocket2.CloseSocket();
		return false;
	}
	
	// Finish connecting the control socket by accepting connection from the thread's socket
	cSocket tmp = m_ControlSocket2.Accept();
	if (!tmp.IsValid())
	{
		LOGERROR("Cannot link Control sockets for a cSocketThread (\"%s\"); continuing, but server may be unreachable from now on.", cSocket::GetLastErrorString().c_str());
		m_ControlSocket2.CloseSocket();
		return false;
	}
	m_ControlSocket2.CloseSocket();
	m_ControlSocket2 = tmp;
	
	return true;
}





void cSocketThreads::cSocketThread::Execute(void)
{
	// Connect the "client" part of the Control socket:
	m_ControlSocket1 = cSocket::CreateSocket();
	cSocket::SockAddr_In Addr;
	Addr.Family = cSocket::ADDRESS_FAMILY_INTERNET;
	Addr.Address = cSocket::INTERNET_ADDRESS_LOCALHOST();
	Addr.Port = m_ControlSocket2.GetPort();
	ASSERT(Addr.Port != 0);  // We checked in the Start() method, but let's be sure
	if (m_ControlSocket1.Connect(Addr) != 0)
	{
		LOGERROR("Cannot connect Control sockets for a cSocketThread (\"%s\"); continuing, but the server may be unreachable from now on.", cSocket::GetLastErrorString().c_str());
		m_ControlSocket2.CloseSocket();
		return;
	}
	
	// The main thread loop:
	while (!m_ShouldTerminate)
	{
		// Put all sockets into the Read set:
		fd_set fdRead;
		cSocket::xSocket Highest = m_ControlSocket1.GetSocket();
		
		PrepareSet(&fdRead, Highest);
		
		// Wait for the sockets:
		if (select(Highest + 1, &fdRead, NULL, NULL, NULL) == -1)
		{
			LOG("select(R) call failed in cSocketThread: \"%s\"", cSocket::GetLastErrorString().c_str());
			continue;
		}
		
		ReadFromSockets(&fdRead);

		// Test sockets for writing:
		fd_set fdWrite;
		Highest = m_ControlSocket1.GetSocket();
		PrepareSet(&fdWrite, Highest);		
		timeval Timeout;
		Timeout.tv_sec = 0;
		Timeout.tv_usec = 0;
		if (select(Highest + 1, NULL, &fdWrite, NULL, &Timeout) == -1)
		{
			LOG("select(W) call failed in cSocketThread: \"%s\"", cSocket::GetLastErrorString().c_str());
			continue;
		}
		
		WriteToSockets(&fdWrite);
		
		RemoveClosedSockets();
	}  // while (!mShouldTerminate)
}





void cSocketThreads::cSocketThread::PrepareSet(fd_set * a_Set, cSocket::xSocket & a_Highest)
{
	FD_ZERO(a_Set);
	FD_SET(m_ControlSocket1.GetSocket(), a_Set);

	cCSLock Lock(m_Parent->m_CS);
	for (int i = m_NumSlots - 1; i >= 0; --i)
	{
		if (!m_Slots[i].m_Socket.IsValid())
		{
			continue;
		}
		cSocket::xSocket s = m_Slots[i].m_Socket.GetSocket();
		FD_SET(s, a_Set);
		if (s > a_Highest)
		{
			a_Highest = s;
		}
	}  // for i - m_Slots[]
}





void cSocketThreads::cSocketThread::ReadFromSockets(fd_set * a_Read)
{
	// Read on available sockets:

	// Reset Control socket state:
	if (FD_ISSET(m_ControlSocket1.GetSocket(), a_Read))
	{
		char Dummy[128];
		m_ControlSocket1.Receive(Dummy, sizeof(Dummy), 0);
	}

	// Read from clients:
	cCSLock Lock(m_Parent->m_CS);
	for (int i = m_NumSlots - 1; i >= 0; --i)
	{
		if (!FD_ISSET(m_Slots[i].m_Socket.GetSocket(), a_Read))
		{
			continue;
		}
		char Buffer[1024];
		int Received = m_Slots[i].m_Socket.Receive(Buffer, ARRAYCOUNT(Buffer), 0);
		if (Received == 0)
		{
			// The socket has been closed by the remote party, close our socket and let it be removed after we process all reading
			m_Slots[i].m_Socket.CloseSocket();
			if (m_Slots[i].m_ShouldCallClient)
			{
				m_Slots[i].m_Client->SocketClosed();
			}
		}
		else if (Received > 0)
		{
			if (m_Slots[i].m_ShouldCallClient)
			{
				m_Slots[i].m_Client->DataReceived(Buffer, Received);
			}
		}
		else
		{
			// The socket has encountered an error, close it and let it be removed after we process all reading
			m_Slots[i].m_Socket.CloseSocket();
			if (m_Slots[i].m_ShouldCallClient)
			{
				m_Slots[i].m_Client->SocketClosed();
			}
		}
	}  // for i - m_Slots[]
}





void cSocketThreads::cSocketThread::WriteToSockets(fd_set * a_Write)
{
	// Write to available client sockets:
	cCSLock Lock(m_Parent->m_CS);
	for (int i = m_NumSlots - 1; i >= 0; --i)
	{
		if (!m_Slots[i].m_Socket.IsValid() || !FD_ISSET(m_Slots[i].m_Socket.GetSocket(), a_Write))
		{
			continue;
		}
		if (m_Slots[i].m_Outgoing.empty())
		{
			// Request another chunk of outgoing data:
			if (m_Slots[i].m_ShouldCallClient)
			{
				m_Slots[i].m_Client->GetOutgoingData(m_Slots[i].m_Outgoing);
			}
			if (m_Slots[i].m_Outgoing.empty())
			{
				// Nothing ready
				if (m_Slots[i].m_ShouldClose)
				{
					// Socket was queued for closing and there's no more data to send, close it now:
					
					// DEBUG
					LOGD("Socket was queued for closing, closing now. Slot %d, client %p, socket %d", i, m_Slots[i].m_Client, m_Slots[i].m_Socket.GetSocket());
					
					m_Slots[i].m_Socket.CloseSocket();
					// The slot must be freed actively by the client, using RemoveClient()
				}
				continue;
			}
		}  // if (outgoing data is empty)
		
		int Sent = m_Slots[i].m_Socket.Send(m_Slots[i].m_Outgoing.data(), m_Slots[i].m_Outgoing.size());
		if (Sent < 0)
		{
			int Err = cSocket::GetLastError();
			LOGWARNING("Error %d while writing to client \"%s\", disconnecting. \"%s\"", Err, m_Slots[i].m_Socket.GetIPString().c_str(), cSocket::GetErrorString(Err).c_str());
			m_Slots[i].m_Socket.CloseSocket();
			if (m_Slots[i].m_ShouldCallClient)
			{
				m_Slots[i].m_Client->SocketClosed();
			}
			return;
		}
		m_Slots[i].m_Outgoing.erase(0, Sent);
		
		// _X: If there's data left, it means the client is not reading fast enough, the server would unnecessarily spin in the main loop with zero actions taken; so signalling is disabled
		// This means that if there's data left, it will be sent only when there's incoming data or someone queues another packet (for any socket handled by this thread)
		/*
		// If there's any data left, signalize the Control socket:
		if (!m_Slots[i].m_Outgoing.empty())
		{
			ASSERT(m_ControlSocket2.IsValid());
			m_ControlSocket2.Send("q", 1);
		}
		*/
	}  // for i - m_Slots[i]
}





void cSocketThreads::cSocketThread::RemoveClosedSockets(void)
{
	// Removes sockets that have been queued for closing from m_Slots[]
	
	cCSLock Lock(m_Parent->m_CS);
	for (int i = m_NumSlots - 1; i >= 0; --i)
	{
		if (!m_Slots[i].m_ShouldClose || !m_Slots[i].m_Outgoing.empty())
		{
			continue;
		}
		m_Slots[i] = m_Slots[--m_NumSlots];
	}  // for i - m_Slots[]
}




