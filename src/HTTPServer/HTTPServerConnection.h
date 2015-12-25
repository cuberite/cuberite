
// HTTPConnection.h

// Declares the cHTTPConnection class representing a single persistent connection in the HTTP server.





#pragma once

#include "../OSSupport/Network.h"





// fwd:
class cHTTPServer;
class cHTTPResponse;
class cHTTPRequest;





class cHTTPServerConnection :
	public cTCPLink::cCallbacks
{
public:

	enum eState
	{
		wcsRecvHeaders,  ///< Receiving request headers (m_CurrentRequest is created if nullptr)
		wcsRecvBody,     ///< Receiving request body    (m_CurrentRequest is valid)
		wcsRecvIdle,     ///< Has received the entire body, waiting to send the response (m_CurrentRequest == nullptr)
		wcsSendingResp,  ///< Sending response body     (m_CurrentRequest == nullptr)
		wcsInvalid,      ///< The request was malformed, the connection is closing
	} ;

	cHTTPServerConnection(cHTTPServer & a_HTTPServer);
	virtual ~cHTTPServerConnection();

	/** Sends HTTP status code together with a_Reason (used for HTTP errors).
	Sends the a_Reason as the body as well, so that browsers display it. */
	void SendStatusAndReason(int a_StatusCode, const AString & a_Reason);

	/** Sends the "401 unauthorized" reply together with instructions on authorizing, using the specified realm */
	void SendNeedAuth(const AString & a_Realm);

	/** Sends the headers contained in a_Response */
	void Send(const cHTTPResponse & a_Response);

	/** Sends the data as the response (may be called multiple times) */
	void Send(const void * a_Data, size_t a_Size);

	/** Sends the data as the response (may be called multiple times) */
	void Send(const AString & a_Data) { Send(a_Data.data(), a_Data.size()); }

	/** Indicates that the current response is finished, gets ready for receiving another request (HTTP 1.1 keepalive) */
	void FinishResponse(void);

	/** Resets the internal connection state for a new request.
	Depending on the state, this will send an "InternalServerError" status or a "ResponseEnd" */
	void AwaitNextRequest(void);

	/** Terminates the connection; finishes any request being currently processed */
	void Terminate(void);

protected:
	typedef std::map<AString, AString> cNameValueMap;

	/** The parent webserver that is to be notified of events on this connection */
	cHTTPServer & m_HTTPServer;

	/** All the incoming data until the entire request header is parsed */
	AString m_IncomingHeaderData;

	/** Status in which the request currently is */
	eState m_State;

	/** The request being currently received
	Valid only between having parsed the headers and finishing receiving the body. */
	cHTTPRequest * m_CurrentRequest;

	/** Number of bytes that remain to read for the complete body of the message to be received.
	Valid only in wcsRecvBody */
	size_t m_CurrentRequestBodyRemaining;

	/** The network link attached to this connection. */
	cTCPLinkPtr m_Link;


	// cTCPLink::cCallbacks overrides:
	/** The link instance has been created, remember it. */
	virtual void OnLinkCreated(cTCPLinkPtr a_Link) override;

	/** Data is received from the client. */
	virtual void OnReceivedData(const char * a_Data, size_t a_Size) override;

	/** The socket has been closed for any reason. */
	virtual void OnRemoteClosed(void) override;

	/** An error has occurred on the socket. */
	virtual void OnError(int a_ErrorCode, const AString & a_ErrorMsg) override;

	// Overridable:
	/** Called to send raw data over the link. Descendants may provide data transformations (SSL etc.) */
	virtual void SendData(const void * a_Data, size_t a_Size);

	/** Sends the raw data over the link.
	Descendants may provide data transformations (SSL etc.) via the overridable SendData() function. */
	void SendData(const AString & a_Data)
	{
		SendData(a_Data.data(), a_Data.size());
	}
} ;

typedef std::vector<cHTTPServerConnection *> cHTTPServerConnections;





