
// WebServer.h

// Declares the cWebServer class representing a HTTP webserver that uses cListenThread and cSocketThreads for processing





#pragma once

#include "OSSupport/ListenThread.h"
#include "OSSupport/SocketThreads.h"
#include "../iniFile/iniFile.h"





// fwd:
class cWebServer;





class cWebRequest :
	public cSocketThreads::cCallback
{
public:
	enum
	{
		HTTP_OK = 200,
		HTTP_BAD_REQUEST = 400,
	} ;
	
	cWebRequest(cWebServer & a_WebServer);
	
	/// Sends HTTP status code together with a_Reason
	void SendStatusAndReason(int a_StatusCode, const AString & a_Reason);
	
protected:
	typedef std::map<AString, AString> cNameValueMap;
	
	cWebServer & m_WebServer;
	
	AString       m_Method;   ///< Method of the request (GET / PUT / POST / ...)
	AString       m_URL;      ///< Full URL of the request
	cNameValueMap m_Headers;  ///< All the headers the request has come with
	
	AString m_IncomingHeaderData;  ///< All the incoming data until the entire header is parsed
	
	/// Set to true when the header haven't been received yet. If false, receiving the optional body.
	bool m_IsReceivingHeaders;
	
	/// Data that is queued for sending, once the socket becomes writable
	AString m_OutgoingData;
	
	
	/// Parses the header in m_IncomingData until the specified end mark
	void ParseHeader(size_t a_IdxEnd);
	
	/** Parses the RequestLine out of m_IncomingHeaderData, up to index a_IdxEnd
	Returns the index to the next line, or npos if invalid request
	*/
	size_t ParseRequestLine(size_t a_IdxEnd);
	
	/** Parses one header field out of m_IncomingHeaderData, starting at the specified offset, up to offset a_IdxEnd.
	Returns the index to the next line, or npos if invalid request.
	a_Key is set to the key that was parsed (used for multi-line headers)
	*/
	size_t ParseHeaderField(size_t a_IdxStart, size_t a_IdxEnd, AString & a_Key);
	
	/** Parses one header field that is known to be a continuation of previous header.
	Returns the index to the next line, or npos if invalid request.
	*/
	size_t ParseHeaderFieldContinuation(size_t a_IdxStart, size_t a_IdxEnd, AString & a_Key);
	
	/// Adds a header into m_Headers; appends if key already exists
	void AddHeader(const AString & a_Key, const AString & a_Value);
	
	// cSocketThreads::cCallback overrides:
	virtual void DataReceived   (const char * a_Data, int a_Size) override;  // Data is received from the client
	virtual void GetOutgoingData(AString & a_Data) override;  // Data can be sent to client
	virtual void SocketClosed   (void) override;  // The socket has been closed for any reason
} ;

typedef std::vector<cWebRequest *> cWebRequests;




class cWebServer :
	public cListenThread::cCallback
{
public:
	cWebServer(void);
	
	bool Initialize(cIniFile & a_IniFile);
	
protected:
	friend class cWebRequest;
	
	cListenThread m_ListenThreadIPv4;
	cListenThread m_ListenThreadIPv6;
	
	cSocketThreads m_SocketThreads;

	cCriticalSection m_CSRequests;
	cWebRequests     m_Requests;    ///< All the requests that are currently being serviced

	// cListenThread::cCallback overrides:
	virtual void OnConnectionAccepted(cSocket & a_Socket) override;
	
	/// Called by cWebRequest when it finishes parsing its header
	void RequestReady(cWebRequest * a_Request);
} ;





