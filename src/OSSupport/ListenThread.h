
// ListenThread.h

// Declares the cListenThread class representing the thread that listens for client connections





#pragma once

#include "IsThread.h"
#include "Socket.h"





// fwd:
class cServer;





class cListenThread :
	public cIsThread
{
	typedef cIsThread super;
	
public:
	/// Used as the callback for connection events
	class cCallback
	{
	public:
		/// This callback is called whenever a socket connection is accepted
		virtual void OnConnectionAccepted(cSocket & a_Socket) = 0;
	} ;
	
	cListenThread(cCallback & a_Callback, const AString & a_ServiceName = "");
	~cListenThread();
	
	/// Creates all the sockets, returns trus if successful, false if not.
	bool Initialize(cSocket::eFamily a_Family, const AString & a_PortsString);
	
	bool Start(void);
	
	void Stop(void);
	
	/// Call before Initialize() to set the "reuse" flag on the sockets
	void SetReuseAddr(bool a_Reuse = true);
	
protected:
	typedef std::vector<cSocket> cSockets;

	/// The callback which to notify of incoming connections	
	cCallback & m_Callback;
	
	/// Socket address family to use
	cSocket::eFamily m_Family;
	
	/// Sockets that are being monitored
	cSockets m_Sockets;
	
	/// If set to true, the SO_REUSEADDR socket option is set to true
	bool m_ShouldReuseAddr;
	
	/// Name of the service that's listening on the ports; for logging purposes only
	AString m_ServiceName;
	
	
	/** Fills in m_Sockets with individual sockets, each for one port specified in a_PortsString.
	Returns true if successful and at least one socket has been created
	*/
	bool CreateSockets(const AString & a_PortsString);
	
	// cIsThread override:
	virtual void Execute(void) override;
} ;




