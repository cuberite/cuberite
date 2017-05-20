
// HTTPConnection.h

// Declares the cHTTPConnection class representing a single persistent connection in the HTTP server.





#pragma once

#include "../OSSupport/Network.h"
#include "HTTPMessageParser.h"





// fwd:
class cHTTPServer;
class cHTTPOutgoingResponse;
class cHTTPIncomingRequest;




class cHTTPServerConnection :
	public cTCPLink::cCallbacks,
	public cHTTPMessageParser::cCallbacks
{
public:
	/** Creates a new instance, connected to the specified HTTP server instance */
	cHTTPServerConnection(cHTTPServer & a_HTTPServer);

	// Force a virtual destructor in all descendants
	virtual ~cHTTPServerConnection() override;

	/** Sends HTTP status code together with a_Reason (used for HTTP errors).
	Sends the a_Reason as the body as well, so that browsers display it.
	Clears the current request (since it's finished by this call). */
	void SendStatusAndReason(int a_StatusCode, const AString & a_Reason);

	/** Sends the "401 unauthorized" reply together with instructions on authorizing, using the specified realm.
	Clears the current request (since it's finished by this call). */
	void SendNeedAuth(const AString & a_Realm);

	/** Sends the headers contained in a_Response */
	void Send(const cHTTPOutgoingResponse & a_Response);

	/** Sends the data as the response (may be called multiple times) */
	void Send(const void * a_Data, size_t a_Size);

	/** Sends the data as the response (may be called multiple times) */
	void Send(const AString & a_Data) { Send(a_Data.data(), a_Data.size()); }

	/** Indicates that the current response is finished, gets ready for receiving another request (HTTP 1.1 keepalive).
	Clears the current request (since it's finished by this call). */
	void FinishResponse(void);

	/** Terminates the connection; finishes any request being currently processed */
	void Terminate(void);

protected:
	typedef std::map<AString, AString> cNameValueMap;

	/** The parent webserver that is to be notified of events on this connection */
	cHTTPServer & m_HTTPServer;

	/** The parser responsible for reading the requests. */
	cHTTPMessageParser m_Parser;

	/** The request being currently received
	Valid only between having parsed the headers and finishing receiving the body. */
	std::unique_ptr<cHTTPIncomingRequest> m_CurrentRequest;

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

	// cHTTPMessageParser::cCallbacks overrides:
	virtual void OnError(const AString & a_ErrorDescription) override;
	virtual void OnFirstLine(const AString & a_FirstLine) override;
	virtual void OnHeaderLine(const AString & a_Key, const AString & a_Value) override;
	virtual void OnHeadersFinished(void) override;
	virtual void OnBodyData(const void * a_Data, size_t a_Size) override;
	virtual void OnBodyFinished(void) override;

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





