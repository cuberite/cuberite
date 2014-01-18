
// SocketThreads.h

// Interfaces to the cSocketThreads class representing the heart of MCS's client networking.
// This object takes care of network communication, groups sockets into threads and uses as little threads as possible for full read / write support
// For more detail, see http://forum.mc-server.org/showthread.php?tid=327

/*
Additional details:
When a client is terminating a connection:
- they call the StopReading() method to disable callbacks for the incoming data
- they call the Write() method to queue any outstanding outgoing data
- they call the QueueClose() method to queue the socket to close after outgoing data has been sent.
When a socket slot is marked as having no callback, it is kept alive until its outgoing data queue is empty and its m_ShouldClose flag is set.
This means that the socket can be written to several times before finally closing it via QueueClose()
*/





/// How many clients should one thread handle? (must be less than FD_SETSIZE for your platform)
#define MAX_SLOTS 63





#pragma once
#ifndef CSOCKETTHREADS_H_INCLUDED
#define CSOCKETTHREADS_H_INCLUDED

#include "Socket.h"
#include "IsThread.h"




// Check MAX_SLOTS:
#if MAX_SLOTS >= FD_SETSIZE
	#error "MAX_SLOTS must be less than FD_SETSIZE for your platform! (otherwise select() won't work)"
#endif





// fwd:
class cSocket;
class cClientHandle;





class cSocketThreads
{
public:

	// Clients of cSocketThreads must implement this interface to be able to communicate
	class cCallback
	{
	public:
		// Force a virtual destructor in all subclasses:
		virtual ~cCallback() {}
		
		/// Called when data is received from the remote party
		virtual void DataReceived(const char * a_Data, int a_Size) = 0;
		
		/// Called when data can be sent to remote party; the function is supposed to append outgoing data to a_Data
		virtual void GetOutgoingData(AString & a_Data) = 0;
		
		/// Called when the socket has been closed for any reason
		virtual void SocketClosed(void) = 0;
	} ;

	
	cSocketThreads(void);
	~cSocketThreads();
	
	/// Add a (socket, client) pair for processing, data from a_Socket is to be sent to a_Client; returns true if successful
	bool AddClient(const cSocket & a_Socket, cCallback * a_Client);
	
	/** Remove the associated socket and the client from processing.
	The socket is left to send its data and is removed only after all its m_OutgoingData is sent
	*/
	void RemoveClient(const cCallback * a_Client);
	
	/// Notify the thread responsible for a_Client that the client has something to write
	void NotifyWrite(const cCallback * a_Client);
	
	/// Puts a_Data into outgoing data queue for a_Client
	void Write(const cCallback * a_Client, const AString & a_Data);
	
	/// Stops reading from the client - when this call returns, no more calls to the callbacks are made
	void StopReading(const cCallback * a_Client);
	
	/// Queues the client for closing, as soon as its outgoing data is sent
	void QueueClose(const cCallback * a_Client);

private:

	class cSocketThread :
		public cIsThread
	{
		typedef cIsThread super;
		
	public:
	
		cSocketThread(cSocketThreads * a_Parent);
		~cSocketThread();
		
		// All these methods assume parent's m_CS is locked
		bool HasEmptySlot(void) const {return m_NumSlots < MAX_SLOTS; }
		bool IsEmpty     (void) const {return m_NumSlots == 0; }

		void AddClient   (const cSocket &   a_Socket, cCallback * a_Client);  // Takes ownership of the socket
		bool RemoveClient(const cCallback * a_Client);  // Returns true if removed, false if not found
		bool HasClient   (const cCallback * a_Client) const;
		bool HasSocket   (const cSocket *   a_Socket) const;
		bool NotifyWrite (const cCallback * a_Client);  // Returns true if client handled by this thread
		bool Write       (const cCallback * a_Client, const AString & a_Data);  // Returns true if client handled by this thread
		bool StopReading (const cCallback * a_Client);  // Returns true if client handled by this thread
		bool QueueClose  (const cCallback * a_Client);  // Returns true if client handled by this thread
		
		bool Start(void);  // Hide the cIsThread's Start method, we need to provide our own startup to create the control socket
		
		bool IsValid(void) const {return m_ControlSocket2.IsValid(); }  // If the Control socket dies, the thread is not valid anymore
		
	private:
	
		cSocketThreads * m_Parent;
	
		// Two ends of the control socket, the first is select()-ed, the second is written to for notifications
		cSocket m_ControlSocket1;
		cSocket m_ControlSocket2;
		
		// Socket-client-packetqueues triplets.
		// Manipulation with these assumes that the parent's m_CS is locked
		struct sSlot
		{
			cSocket     m_Socket;  // The socket is primarily owned by this
			cCallback * m_Client;
			AString     m_Outgoing;  // If sending writes only partial data, the rest is stored here for another send
			bool        m_ShouldClose;  // If true, the socket is to be closed after sending all outgoing data
			bool        m_ShouldCallClient;  // If true, the client callbacks are called. Set to false in StopReading()
		} ;
		sSlot m_Slots[MAX_SLOTS];
		int   m_NumSlots;  // Number of slots actually used
		
		virtual void Execute(void) override;
		
		void PrepareSet     (fd_set * a_Set, cSocket::xSocket & a_Highest);  // Puts all sockets into the set, along with m_ControlSocket1
		void ReadFromSockets(fd_set * a_Read);  // Reads from sockets indicated in a_Read
		void WriteToSockets (fd_set * a_Write);  // Writes to sockets indicated in a_Write
	} ;
	
	typedef std::list<cSocketThread *> cSocketThreadList;
	
	
	cCriticalSection  m_CS;
	cSocketThreadList m_Threads;
} ;





#endif  // CSOCKETTHREADS_H_INCLUDED




