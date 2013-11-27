
// HTTPConnection.h

// Declares the cHTTPConnection class representing a single persistent connection in the HTTP server.





#pragma once

#include "../OSSupport/SocketThreads.h"





// fwd:
class cHTTPServer;
class cHTTPResponse;
class cHTTPRequest;





class cHTTPConnection :
	public cSocketThreads::cCallback
{
public:
	
	enum eState
	{
		wcsRecvHeaders,  ///< Receiving request headers (m_CurrentRequest is created if NULL)
		wcsRecvBody,     ///< Receiving request body    (m_CurrentRequest is valid)
		wcsRecvIdle,     ///< Has received the entire body, waiting to send the response (m_CurrentRequest == NULL)
		wcsSendingResp,  ///< Sending response body     (m_CurrentRequest == NULL)
		wcsInvalid,      ///< The request was malformed, the connection is closing
	} ;
	
	cHTTPConnection(cHTTPServer & a_HTTPServer);
	~cHTTPConnection();
	
	/// Sends HTTP status code together with a_Reason (used for HTTP errors)
	void SendStatusAndReason(int a_StatusCode, const AString & a_Reason);
	
	/// Sends the "401 unauthorized" reply together with instructions on authorizing, using the specified realm
	void SendNeedAuth(const AString & a_Realm);
	
	/// Sends the headers contained in a_Response
	void Send(const cHTTPResponse & a_Response);
	
	/// Sends the data as the response (may be called multiple times)
	void Send(const void * a_Data, int a_Size);

	/// Sends the data as the response (may be called multiple times)
	void Send(const AString & a_Data) { Send(a_Data.data(), a_Data.size()); }
	
	/// Indicates that the current response is finished, gets ready for receiving another request (HTTP 1.1 keepalive)
	void FinishResponse(void);
	
	/// Resets the connection for a new request. Depending on the state, this will send an "InternalServerError" status or a "ResponseEnd"
	void AwaitNextRequest(void);
	
	/// Terminates the connection; finishes any request being currently processed
	void Terminate(void);
	
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

	/// Number of bytes that remain to read for the complete body of the message to be received. Valid only in wcsRecvBody
	int m_CurrentRequestBodyRemaining;
	
	
	// cSocketThreads::cCallback overrides:
	virtual void DataReceived   (const char * a_Data, int a_Size) override;  // Data is received from the client
	virtual void GetOutgoingData(AString & a_Data) override;  // Data can be sent to client
	virtual void SocketClosed   (void) override;  // The socket has been closed for any reason
} ;

typedef std::vector<cHTTPConnection *> cHTTPConnections;





