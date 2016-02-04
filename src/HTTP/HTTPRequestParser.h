
// HTTPRequestParser.h

// Declares the cHTTPRequestParser class representing the parser for incoming HTTP requests




#pragma once

#include "HTTPMessage.h"
#include "EnvelopeParser.h"





class cHTTPRequestParser :
	public cHTTPMessage,
	protected cEnvelopeParser::cCallbacks
{
	typedef cHTTPMessage super;
	
public:
	cHTTPRequestParser(void);
	
	/** Parses the request line and then headers from the received data.
	Returns the number of bytes consumed or AString::npos number for error.
	Once it has fully parsed all the headers, doesn't consume any more data. */
	size_t ParseHeaders(const char * a_Data, size_t a_Size);
	
	/** Returns true if the request did contain a Content-Length header */
	bool HasReceivedContentLength(void) const { return (m_ContentLength != AString::npos); }
	
	/** Returns the method used in the request */
	const AString & GetMethod(void) const { return m_Method; }
	
	/** Returns the URL used in the request */
	const AString & GetURL(void) const { return m_URL; }
	
	/** Returns the URL used in the request, without any parameters */
	AString GetBareURL(void) const;
	
	/** Sets the UserData pointer that is stored within this request.
	The request doesn't touch this data (doesn't delete it)! */
	void SetUserData(void * a_UserData) { m_UserData = a_UserData; }
	
	/** Retrieves the UserData pointer that has been stored within this request. */
	void * GetUserData(void) const { return m_UserData; }
	
	/** Returns true if more data is expected for the request headers */
	bool IsInHeaders(void) const { return m_EnvelopeParser.IsInHeaders(); }
	
	/** Returns true if the request did present auth data that was understood by the parser */
	bool HasAuth(void) const { return m_HasAuth; }
	
	/** Returns the username that the request presented. Only valid if HasAuth() is true */
	const AString & GetAuthUsername(void) const { return m_AuthUsername; }
	
	/** Returns the password that the request presented. Only valid if HasAuth() is true */
	const AString & GetAuthPassword(void) const { return m_AuthPassword; }
	
	bool DoesAllowKeepAlive(void) const { return m_AllowKeepAlive; }
	
protected:
	/** Parser for the envelope data */
	cEnvelopeParser m_EnvelopeParser;
	
	/** True if the data received so far is parsed successfully. When false, all further parsing is skipped */
	bool m_IsValid;
	
	/** Bufferred incoming data, while parsing for the request line */
	AString m_IncomingHeaderData;
	
	/** Method of the request (GET / PUT / POST / ...) */
	AString m_Method;
	
	/** Full URL of the request */
	AString m_URL;
	
	/** Data that the HTTPServer callbacks are allowed to store. */
	void * m_UserData;
	
	/** Set to true if the request contains auth data that was understood by the parser */
	bool m_HasAuth;
	
	/** The username used for auth */
	AString m_AuthUsername;
	
	/** The password used for auth */
	AString m_AuthPassword;
	
	/** Set to true if the request indicated that it supports keepalives.
	If false, the server will close the connection once the request is finished */
	bool m_AllowKeepAlive;
	
	
	/** Parses the incoming data for the first line (RequestLine)
	Returns the number of bytes consumed, or AString::npos for an error
	*/
	size_t ParseRequestLine(const char * a_Data, size_t a_Size);
	
	// cEnvelopeParser::cCallbacks overrides:
	virtual void OnHeaderLine(const AString & a_Key, const AString & a_Value) override;
} ;




