
// HTTPServer.h

// Declares the cHTTPServer class representing a HTTP webserver that uses cListenThread and cSocketThreads for processing





#pragma once

#include "../OSSupport/ListenThread.h"
#include "../OSSupport/SocketThreads.h"
#include "../../iniFile/iniFile.h"





// fwd:
class cHTTPServer;
class cHTTPMessage;
class cHTTPRequest;
class cHTTPResponse;





class cHTTPConnection :
	public cSocketThreads::cCallback
{
public:
	
	enum eState
	{
		wcsRecvHeaders,  ///< Receiving request headers (m_CurrentRequest == NULL)
		wcsRecvBody,     ///< Receiving request body    (m_CurrentRequest is valid)
		wcsRecvIdle,     ///< Has received the entire body, waiting to send the response (m_CurrentRequest == NULL)
		wcsSendingResp,  ///< Sending response body     (m_CurrentRequest == NULL)
		wcsInvalid,      ///< The request was malformed, the connection is closing
	} ;
	
	cHTTPConnection(cHTTPServer & a_HTTPServer);
	
	/// Sends HTTP status code together with a_Reason (used for HTTP errors)
	void SendStatusAndReason(int a_StatusCode, const AString & a_Reason);
	
	/// Sends the headers contained in a_Response
	void Send(const cHTTPResponse & a_Response);
	
	/// Sends the data as the response (may be called multiple times)
	void Send(const void * a_Data, int a_Size);

	/// Sends the data as the response (may be called multiple times)
	void Send(const AString & a_Data) { Send(a_Data.data(), a_Data.size()); }
	
	/// Finishes sending current response, gets ready for receiving another request (HTTP 1.1 keepalive)
	void FinishResponse(void);
	
protected:
	typedef std::map<AString, AString> cNameValueMap;
	
	/// The parent webserver that is to be notified of events on this connection
	cHTTPServer & m_HTTPServer;
	
	/// All the incoming data until the entire request header is parsed
	AString m_IncomingHeaderData;
	
	/// Status in which the request currently is
	eState m_State;
	
	/// Data that is queued for sending, once the socket becomes writable
	AString m_OutgoingData;
	
	/// The request being currently received (valid only between having parsed the headers and finishing receiving the body)
	cHTTPRequest * m_CurrentRequest;
	
	
	/// Parses the header in m_IncomingData until the specified end mark
	void ParseHeader(size_t a_IdxEnd);
	
	/// Sends the response status and headers. Transition from wrsRecvBody to wrsSendingResp.
	void SendRespHeaders(void);
	
	// cSocketThreads::cCallback overrides:
	virtual void DataReceived   (const char * a_Data, int a_Size) override;  // Data is received from the client
	virtual void GetOutgoingData(AString & a_Data) override;  // Data can be sent to client
	virtual void SocketClosed   (void) override;  // The socket has been closed for any reason
} ;

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





