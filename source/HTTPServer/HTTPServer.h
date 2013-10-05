
// HTTPServer.h

// Declares the cHTTPServer class representing a HTTP webserver that uses cListenThread and cSocketThreads for processing





#pragma once

#include "../OSSupport/ListenThread.h"
#include "../OSSupport/SocketThreads.h"
#include "../../iniFile/iniFile.h"





// fwd:
class cHTTPMessage;
class cHTTPRequest;
class cHTTPResponse;
class cHTTPConnection;

typedef std::vector<cHTTPConnection *> cHTTPConnections;






class cHTTPServer :
	public cListenThread::cCallback
{
public:
	class cCallbacks
	{
	public:
		/** Called when a new request arrives over a connection and its headers have been parsed.
		The request body needn't have arrived yet.
		*/
		virtual void OnRequestBegun(cHTTPConnection & a_Connection, cHTTPRequest & a_Request) = 0;
		
		/// Called when another part of request body has arrived.
		virtual void OnRequestBody(cHTTPConnection & a_Connection, cHTTPRequest & a_Request, const char * a_Data, int a_Size) = 0;
		
		/// Called when the request body has been fully received in previous calls to OnRequestBody()
		virtual void OnRequestFinished(cHTTPConnection & a_Connection, cHTTPRequest & a_Request) = 0;
	} ;
	
	cHTTPServer(void);
	
	/// Initializes the server on the specified ports
	bool Initialize(const AString & a_PortsIPv4, const AString & a_PortsIPv6);
	
	/// Starts the server and assigns the callbacks to use for incoming requests
	bool Start(cCallbacks & a_Callbacks);
	
	/// Stops the server, drops all current connections
	void Stop(void);
	
protected:
	friend class cHTTPConnection;
	
	cListenThread m_ListenThreadIPv4;
	cListenThread m_ListenThreadIPv6;
	
	cSocketThreads m_SocketThreads;

	cCriticalSection m_CSConnections;
	cHTTPConnections m_Connections;    ///< All the connections that are currently being serviced
	
	/// The callbacks to call for various events
	cCallbacks * m_Callbacks;
	

	// cListenThread::cCallback overrides:
	virtual void OnConnectionAccepted(cSocket & a_Socket) override;
	
	/// Called by cHTTPConnection to close the connection (presumably due to an error)
	void CloseConnection(cHTTPConnection & a_Connection);
	
	/// Called by cHTTPConnection to notify SocketThreads that there's data to be sent for the connection
	void NotifyConnectionWrite(cHTTPConnection & a_Connection);
	
	/// Called by cHTTPConnection when it finishes parsing the request header
	void NewRequest(cHTTPConnection & a_Connection, cHTTPRequest & a_Request);
	
	/// Called by cHTTPConenction when it receives more data for the request body
	void RequestBody(cHTTPConnection & a_Connection, cHTTPRequest & a_Request, const char * a_Data, int a_Size);
	
	/// Called by cHTTPConnection when it detects that the request has finished (all of its body has been received)
	void RequestFinished(cHTTPConnection & a_Connection, cHTTPRequest & a_Request);
} ;





