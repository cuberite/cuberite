
// HTTPMessage.h

// Declares the cHTTPMessage class representing the common ancestor for HTTP request and response classes





#pragma once

#include "EnvelopeParser.h"





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
	public cHTTPMessage,
	protected cEnvelopeParser::cCallbacks
{
	typedef cHTTPMessage super;
	
public:
	cHTTPRequest(void);
	
	/** Parses the request line and then headers from the received data.
	Returns the number of bytes consumed or a negative number for error
	*/
	int ParseHeaders(const char * a_Data, int a_Size);
	
	/// Returns true if the request did contain a Content-Length header
	bool HasReceivedContentLength(void) const { return (m_ContentLength >= 0); }
	
	/// Returns the method used in the request
	const AString & GetMethod(void) const { return m_Method; }
	
	/// Returns the URL used in the request
	const AString & GetURL(void) const { return m_URL; }
	
	/// Returns the URL used in the request, without any parameters
	AString GetBareURL(void) const;
	
	/// Sets the UserData pointer that is stored within this request. The request doesn't touch this data (doesn't delete it)!
	void SetUserData(void * a_UserData) { m_UserData = a_UserData; }
	
	/// Retrieves the UserData pointer that has been stored within this request.
	void * GetUserData(void) const { return m_UserData; }
	
	/// Returns true if more data is expected for the request headers
	bool IsInHeaders(void) const { return m_EnvelopeParser.IsInHeaders(); }
	
	/// Returns true if the request did present auth data that was understood by the parser
	bool HasAuth(void) const { return m_HasAuth; }
	
	/// Returns the username that the request presented. Only valid if HasAuth() is true
	const AString & GetAuthUsername(void) const { return m_AuthUsername; }
	
	/// Returns the password that the request presented. Only valid if HasAuth() is true
	const AString & GetAuthPassword(void) const { return m_AuthPassword; }
	
protected:
	/// Parser for the envelope data
	cEnvelopeParser m_EnvelopeParser;
	
	/// True if the data received so far is parsed successfully. When false, all further parsing is skipped
	bool m_IsValid;
	
	/// Bufferred incoming data, while parsing for the request line
	AString m_IncomingHeaderData;
	
	/// Method of the request (GET / PUT / POST / ...)
	AString m_Method;
	
	/// Full URL of the request
	AString m_URL;
	
	/// Data that the HTTPServer callbacks are allowed to store.
	void * m_UserData;
	
	/// Set to true if the request contains auth data that was understood by the parser
	bool m_HasAuth;
	
	/// The username used for auth
	AString m_AuthUsername;
	
	/// The password used for auth
	AString m_AuthPassword;
	
	
	/** Parses the incoming data for the first line (RequestLine)
	Returns the number of bytes consumed, or -1 for an error
	*/
	int ParseRequestLine(const char * a_Data, int a_Size);
	
	// cEnvelopeParser::cCallbacks overrides:
	virtual void OnHeaderLine(const AString & a_Key, const AString & a_Value) override;
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




