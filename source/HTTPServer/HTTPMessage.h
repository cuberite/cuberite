
// HTTPMessage.h

// Declares the cHTTPMessage class representing the common ancestor for HTTP request and response classes





#pragma once





class cHTTPMessage
{
public:
	enum
	{
		HTTP_OK = 200,
		HTTP_BAD_REQUEST = 400,
	} ;

	enum eKind
	{
		mkRequest,
		mkResponse,
	} ;
	
	cHTTPMessage(eKind a_Kind);

	/// Adds a header into the internal map of headers. Recognizes special headers: Content-Type and Content-Length
	void AddHeader(const AString & a_Key, const AString & a_Value);
	
	void SetContentType  (const AString & a_ContentType) { m_ContentType   = a_ContentType; }
	void SetContentLength(int a_ContentLength)           { m_ContentLength = a_ContentLength; }
	
	const AString & GetContentType  (void) const { return m_ContentType; }
	int             GetContentLength(void) const { return m_ContentLength; }

protected:
	typedef std::map<AString, AString> cNameValueMap;
	
	eKind m_Kind;
	
	cNameValueMap m_Headers;

	/// Type of the content; parsed by AddHeader(), set directly by SetContentLength()
	AString m_ContentType;

	/// Length of the content that is to be received. -1 when the object is created, parsed by AddHeader() or set directly by SetContentLength()
	int m_ContentLength;
} ;





class cHTTPRequest :
	public cHTTPMessage
{
	typedef cHTTPMessage super;
	
public:
	cHTTPRequest(void);
	
	/// Parses the headers information from the received data in the specified string of incoming data. Returns true if successful.
	bool ParseHeaders(const char * a_IncomingData, size_t a_idxEnd);
	
	/// Returns true if the request did contain a Content-Length header
	bool HasReceivedContentLength(void) const { return (m_ContentLength >= 0); }
	
	/// Sets the UserData pointer that is stored within this request. The request doesn't touch this data (doesn't delete it)!
	void SetUserData(void * a_UserData) { m_UserData = a_UserData; }
	
	/// Retrieves the UserData pointer that has been stored within this request.
	void * GetUserData(void) const { return m_UserData; }
	
protected:
	/// Method of the request (GET / PUT / POST / ...)
	AString m_Method;
	
	/// Full URL of the request
	AString m_URL;
	
	/// Data that the HTTPServer callbacks are allowed to store.
	void * m_UserData;
	
	
	/** Parses the RequestLine out of a_Data, up to index a_IdxEnd
	Returns the index to the next line, or npos if invalid request
	*/
	size_t ParseRequestLine(const char * a_Data, size_t a_IdxEnd);
	
	/** Parses one header field out of a_Data, up to offset a_IdxEnd.
	Returns the index to the next line (relative to a_Data), or npos if invalid request.
	a_Key is set to the key that was parsed (used for multi-line headers)
	*/
	size_t ParseHeaderField(const char * a_Data, size_t a_IdxEnd, AString & a_Key);
	
	/** Parses one header field that is known to be a continuation of previous header.
	Returns the index to the next line, or npos if invalid request.
	*/
	size_t ParseHeaderFieldContinuation(const char * a_Data, size_t a_IdxEnd, AString & a_Key);
} ;





class cHTTPResponse :
	public cHTTPMessage
{
	typedef cHTTPMessage super;

public:
	cHTTPResponse(void);
	
	/** Appends the response to the specified datastream - response line and headers.
	The body will be sent later directly through cConnection::Send()
	*/
	void AppendToData(AString & a_DataStream) const;
} ;




