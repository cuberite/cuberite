
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
	cHTTPServer(void);
	
	bool Initialize(cIniFile & a_IniFile);
	
protected:
	friend class cHTTPConnection;
	
	cListenThread m_ListenThreadIPv4;
	cListenThread m_ListenThreadIPv6;
	
	cSocketThreads m_SocketThreads;

	cCriticalSection m_CSConnections;
	cHTTPConnections m_Connections;    ///< All the connections that are currently being serviced

	// cListenThread::cCallback overrides:
	virtual void OnConnectionAccepted(cSocket & a_Socket) override;
	
	/// Called by cHTTPConnection to close the connection (presumably due to an error)
	void CloseConnection(cHTTPConnection & a_Connection);
	
	/// Called by cHTTPConnection when it finishes parsing the request header
	void NewRequest(cHTTPConnection & a_Connection, cHTTPRequest & a_Request);
	
	/// Called by cHTTPConenction when it receives more data for the request body
	void RequestBody(cHTTPConnection & a_Connection, cHTTPRequest & a_Request);
	
	/// Called by cHTTPConnection when it detects that the request has finished (all of its body has been received)
	void RequestFinished(cHTTPConnection & a_Connection, cHTTPRequest & a_Request);
} ;





