
// cSocketThreads.cpp

// Implements the cSocketThreads class representing the heart of MCS's client networking.
// This object takes care of network communication, groups sockets into threads and uses as little threads as possible for full read / write support
// For more detail, see http://forum.mc-server.org/showthread.php?tid=327

#include "Globals.h"
#include "SocketThreads.h"
#include "Errors.h"
#include "SocketSet.h"





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





////////////////////////////////////////////////////////////////////////////////
// cSocketThreads::cSocketThread:

cSocketThreads::cSocketThread::cSocketThread(cSocketThreads * a_Parent) :
	cIsThread("cSocketThread"),
	m_Parent(a_Parent),
	//m_NumSlots(0)
	m_Slots()
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
	ASSERT(m_Parent->m_CS.IsLockedByCurrentThread());
	ASSERT(HasEmptySlot());  // Use HasEmptySlot() to check before adding
	
	std::pair<cSocket, sSlot> slot(a_Socket, {a_Socket, a_Client, "", sSlot::ssNormal});
	
	m_Slots.insert(slot);
	
	/*m_Slots[m_NumSlots].m_Client = a_Client;
	m_Slots[m_NumSlots].m_Socket = a_Socket;
	m_Slots[m_NumSlots].m_Socket.SetNonBlocking();
	m_Slots[m_NumSlots].m_Outgoing.clear();
	m_Slots[m_NumSlots].m_State = sSlot::ssNormal;*/
	//m_NumSlots++;
	
	// Notify the thread of the change:
	ASSERT(m_ControlSocket2.IsValid());
	m_ControlSocket2.Send("a", 1);
}





bool cSocketThreads::cSocketThread::RemoveClient(const cCallback * a_Client)
{
	ASSERT(m_Parent->m_CS.IsLockedByCurrentThread());
	
	if (IsEmpty())
	{
		return false;
	}
	
	for (std::map<cSocket, sSlot>::iterator itr = m_Slots.begin(), end = m_Slots.end(); itr != end; itr++)
	{
		if (itr->second.m_Client != a_Client)
		{
			continue;
		}
		
		sSlot & slot = itr->second;
		// Found the slot:
		if (slot.m_State == sSlot::ssRemoteClosed)
		{
			// The remote has already closed the socket, remove the slot altogether:
			if (slot.m_Socket.IsValid())
			{
				slot.m_Socket.CloseSocket();
			}
			m_Slots.erase(itr);
		}
		else
		{
			// Query and queue the last batch of outgoing data:
			AString Data;
			slot.m_Client->GetOutgoingData(Data);
			slot.m_Outgoing.append(Data);
			if (itr->second.m_Outgoing.empty())
			{
				// No more outgoing data, shut the socket down immediately:
				slot.m_Socket.ShutdownReadWrite();
				slot.m_State = sSlot::ssShuttingDown;
			}
			else
			{
				// More data to send, shut down reading and wait for the rest to get sent:
				slot.m_State = sSlot::ssWritingRestOut;
			}
			slot.m_Client = NULL;
		}
		
		// Notify the thread of the change:
		ASSERT(m_ControlSocket2.IsValid());
		m_ControlSocket2.Send("r", 1);
		return true;
	}
	
	// Not found
	return false;
}





bool cSocketThreads::cSocketThread::HasClient(const cCallback * a_Client) const
{
	ASSERT(m_Parent->m_CS.IsLockedByCurrentThread());

	for (std::map<cSocket, sSlot>::const_iterator itr = m_Slots.begin(), end = m_Slots.end(); itr != end ; itr++)
	{
		if (itr->second.m_Client == a_Client)
		{
			return true;
		}
	}
	return false;
}





bool cSocketThreads::cSocketThread::HasSocket(const cSocket * a_Socket) const
{
	return(m_Slots.find(*a_Socket) != m_Slots.end());
}





bool cSocketThreads::cSocketThread::NotifyWrite(const cCallback * a_Client)
{
	ASSERT(m_Parent->m_CS.IsLockedByCurrentThread());

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
	ASSERT(m_Parent->m_CS.IsLockedByCurrentThread());
	for (std::map<cSocket, sSlot>::iterator itr = m_Slots.begin(), end = m_Slots.end(); itr != end ; itr++)
	{
		if (itr->second.m_Client == a_Client)
		{
			itr->second.m_Outgoing.append(a_Data);
			
			// Notify the thread that there's data in the queue:
			ASSERT(m_ControlSocket2.IsValid());
			m_ControlSocket2.Send("q", 1);
			
			return true;
		}
	}  // for i - m_Slots[]
	return false;
}





bool cSocketThreads::cSocketThread::Start(void)
{
	// Create the control socket listener
	m_ControlSocket2 = cSocket::CreateSocket(cSocket::IPv4);
	if (!m_ControlSocket2.IsValid())
	{
		LOGERROR("Cannot create a Control socket for a cSocketThread (\"%s\"); continuing, but server may be unreachable from now on.", cSocket::GetLastErrorString().c_str());
		return false;
	}
	if (!m_ControlSocket2.BindToLocalhostIPv4(cSocket::ANY_PORT))
	{
		LOGERROR("Cannot bind a Control socket for a cSocketThread (\"%s\"); continuing, but server may be unreachable from now on.", cSocket::GetLastErrorString().c_str());
		m_ControlSocket2.CloseSocket();
		return false;
	}
	if (!m_ControlSocket2.Listen(1))
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
	m_ControlSocket1 = cSocket::CreateSocket(cSocket::IPv4);
	ASSERT(m_ControlSocket2.GetPort() != 0);  // We checked in the Start() method, but let's be sure
	if (!m_ControlSocket1.ConnectToLocalhostIPv4(m_ControlSocket2.GetPort()))
	{
		LOGERROR("Cannot connect Control sockets for a cSocketThread (\"%s\"); continuing, but the server may be unreachable from now on.", cSocket::GetLastErrorString().c_str());
		m_ControlSocket2.CloseSocket();
		return;
	}
	
	// The main thread loop:
	while (!m_ShouldTerminate)
	{
		// Read outgoing data from the clients:
		QueueOutgoingData();
		
		// Put sockets into the sets
		cSocketSet ReadSockets;
		cSocketSet WriteSockets;
		//fd_set fdRead;
		//fd_set fdWrite;
		//cSocket::xSocket Highest = m_ControlSocket1.GetSocket();
		PrepareSets(ReadSockets, WriteSockets);
		
		// Wait for the sockets:
		/*timeval Timeout;
		Timeout.tv_sec = 5;
		Timeout.tv_usec = 0;
		if (select(Highest + 1, &fdRead, &fdWrite, NULL, &Timeout) == -1)
		{
			
		}*/
		if (!cSocket::SelectReadWrite(ReadSockets,WriteSockets,5))
		{
			LOG("select() call failed in cSocketThread: \"%s\"", cSocket::GetLastErrorString().c_str());
			continue;
		}
		
		// Perform the IO:
		ReadFromSockets(ReadSockets);
		WriteToSockets(WriteSockets);
		CleanUpShutSockets();
	}  // while (!mShouldTerminate)
}





void cSocketThreads::cSocketThread::PrepareSets(cSocketSet & a_Read, cSocketSet & a_Write)
{
	//FD_ZERO(a_Read);
	//FD_ZERO(a_Write);
	//FD_SET(m_ControlSocket1.GetSocket(), a_Read);
	a_Read.Add(m_ControlSocket1);

	cCSLock Lock(m_Parent->m_CS);
	for (std::map<cSocket, sSlot>::iterator itr = m_Slots.begin(), end = m_Slots.end(); itr != end ; itr++)
	{
		if (!itr->second.m_Socket.IsValid())
		{
			continue;
		}
		if (itr->second.m_State == sSlot::ssRemoteClosed)
		{
			// This socket won't provide nor consume any data anymore, don't put it in the Set
			continue;
		}
		cSocket s = itr->second.m_Socket;
		a_Read.Add(s);
		//FD_SET(s, a_Read);
		//if (s > a_Highest)
		//{
		//	a_Highest = s;
		//}
		if (!itr->second.m_Outgoing.empty())
		{
			// There's outgoing data for the socket, put it in the Write set
			//FD_SET(s, a_Write);
			a_Write.Add(s);
		}
	} 
}





void cSocketThreads::cSocketThread::ReadFromSockets(cSocketSet & a_Read)
{
	// Read on available sockets:

	// Reset Control socket state:
	if (a_Read.IsSet(m_ControlSocket1))
	{
		char Dummy[128];
		m_ControlSocket1.Receive(Dummy, sizeof(Dummy), 0);
	}

	// Read from clients:
	cCSLock Lock(m_Parent->m_CS);
	for (cSocketSet::cEventIterator itr = a_Read.Eventbegin(), end = a_Read.Eventend(); itr != end; itr++)
	{
		//cSocket::xSocket Socket = m_Slots[i].m_Socket.GetSocket();
		if (!itr->IsValid())
		{
			continue;
		}
		char Buffer[1024];
		int Received = itr->Receive(Buffer, ARRAYCOUNT(Buffer), 0);
		sSlot & slot = m_Slots.find(*itr)->second;
		if (Received <= 0)
		{
			if (cSocket::GetLastError() != cSocket::ErrWouldBlock)
			{
				// The socket has been closed by the remote party
				switch (slot.m_State)
				{
					case sSlot::ssNormal:
					{
						// Notify the callback that the remote has closed the socket; keep the slot
						slot.m_Client->SocketClosed();
						slot.m_State = sSlot::ssRemoteClosed;
						slot.m_Socket.CloseSocket();
						break;
					}
					case sSlot::ssWritingRestOut:
					case sSlot::ssShuttingDown:
					case sSlot::ssShuttingDown2:
					{
						// Force-close the socket and remove the slot:
						slot.m_Socket.CloseSocket();
						m_Slots.erase(m_Slots.find(*itr));
						break;
					}
					default:
					{
						LOG("%s: Unexpected socket state: %d (%s)",
							__FUNCTION__, static_cast<int>(*itr), itr->GetIPString().c_str()
						);
						ASSERT(!"Unexpected socket state");
						break;
					}
				}  // switch (m_Slots[i].m_State)
			}
		}
		else
		{
			if (slot.m_Client != NULL)
			{
				slot.m_Client->DataReceived(Buffer, Received);
			}
		}
	}
}





void cSocketThreads::cSocketThread::WriteToSockets(cSocketSet & a_Write)
{
	// Write to available client sockets:
	cCSLock Lock(m_Parent->m_CS);
	for (cSocketSet::cEventIterator itr = a_Write.Eventbegin(), end = a_Write.Eventend(); itr != end; itr++)
	{
		//cSocket::xSocket Socket = m_Slots[i].m_Socket.GetSocket();
		if (!itr->IsValid())
		{
			continue;
		}
		sSlot & slot = m_Slots.find(*itr)->second;
		if (slot.m_Outgoing.empty())
		{
			// Request another chunk of outgoing data:
			if (slot.m_Client != NULL)
			{
				AString Data;
				slot.m_Client->GetOutgoingData(Data);
				slot.m_Outgoing.append(Data);
			}
			if (slot.m_Outgoing.empty())
			{
				// No outgoing data is ready
				if (slot.m_State == sSlot::ssWritingRestOut)
				{
					slot.m_State = sSlot::ssShuttingDown;
					slot.m_Socket.ShutdownReadWrite();
				}
				continue;
			}
		}  // if (outgoing data is empty)
		
		if (slot.m_State == sSlot::ssRemoteClosed)
		{
			continue;
		}
		
		if (!SendDataThroughSocket(*itr, slot.m_Outgoing))
		{
			int Err = cSocket::GetLastError();
			LOGWARNING("Error %d while writing to client \"%s\", disconnecting. \"%s\"", Err, itr->GetIPString().c_str(), GetOSErrorString(Err).c_str());
			itr->CloseSocket();
			if (slot.m_Client != NULL)
			{
				slot.m_Client->SocketClosed();
			}
			continue;
		}
		
		if (slot.m_Outgoing.empty() && (slot.m_State == sSlot::ssWritingRestOut))
		{
			slot.m_State = sSlot::ssShuttingDown;
			slot.m_Socket.ShutdownReadWrite();
		}

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





bool cSocketThreads::cSocketThread::SendDataThroughSocket(cSocket & a_Socket, AString & a_Data)
{
	// Send data in smaller chunks, so that the OS send buffers aren't overflown easily
	while (!a_Data.empty())
	{
		size_t NumToSend = std::min(a_Data.size(), (size_t)1024);
		int Sent = a_Socket.Send(a_Data.data(), NumToSend);
		if (Sent < 0)
		{
			int Err = cSocket::GetLastError();
			if (Err == cSocket::ErrWouldBlock)
			{
				// The OS send buffer is full, leave the outgoing data for the next time
				return true;
			}
			// An error has occured
			return false;
		}
		if (Sent == 0)
		{
			a_Socket.CloseSocket();
			return true;
		}
		a_Data.erase(0, Sent);
	}
	return true;
}





void cSocketThreads::cSocketThread::CleanUpShutSockets(void)
{
	cCSLock Lock(m_Parent->m_CS);
	for (std::map<cSocket, sSlot>::iterator itr = m_Slots.begin(), end = m_Slots.end(); itr != end ;)
	{
		switch (itr->second.m_State)
		{
			case sSlot::ssShuttingDown2:
			{
				// The socket has reached the shutdown timeout, close it and clear its slot:
				// increment then erase the previous value
				m_Slots.erase(itr++);
				break;
			}
			case sSlot::ssShuttingDown:
			{
				// The socket has been shut down for a single thread loop, let it loop once more before closing:
				itr->second.m_State = sSlot::ssShuttingDown2;
				itr++;
				break;
			}
			default:
			{
				itr++;
				break;
			}
		}
	}  // for i - m_Slots[]
}




void cSocketThreads::cSocketThread::QueueOutgoingData(void)
{
	cCSLock Lock(m_Parent->m_CS);
	for (std::map<cSocket, sSlot>::iterator itr = m_Slots.begin(), end = m_Slots.end(); itr != end ; itr++)
	{
		sSlot & slot = itr->second;
		if (slot.m_Client != NULL)
		{
			AString Data;
			slot.m_Client->GetOutgoingData(Data);
			slot.m_Outgoing.append(Data);
		}
		if (slot.m_Outgoing.empty())
		{
			// No outgoing data is ready
			if (slot.m_State == sSlot::ssWritingRestOut)
			{
				// The socket doesn't want to be kept alive anymore, and doesn't have any remaining data to send.
				// Shut it down and then close it after a timeout, or when the other side agrees
				slot.m_State = sSlot::ssShuttingDown;
				slot.m_Socket.ShutdownReadWrite();
			}
			continue;
		}
	}
}




