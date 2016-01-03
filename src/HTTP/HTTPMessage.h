
// HTTPMessage.h

// Declares the cHTTPMessage class representing the common ancestor for HTTP request and response classes





#pragma once

#include "EnvelopeParser.h"





class cHTTPMessage
{
public:
	enum eStatus
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

	// Force a virtual destructor in all descendants
	virtual ~cHTTPMessage() {}

	/** Adds a header into the internal map of headers. Recognizes special headers: Content-Type and Content-Length */
	void AddHeader(const AString & a_Key, const AString & a_Value);

	void SetContentType  (const AString & a_ContentType) { m_ContentType   = a_ContentType; }
	void SetContentLength(size_t a_ContentLength)        { m_ContentLength = a_ContentLength; }

	const AString & GetContentType  (void) const { return m_ContentType; }
	size_t          GetContentLength(void) const { return m_ContentLength; }

protected:
	typedef std::map<AString, AString> cNameValueMap;

	eKind m_Kind;

	/** Map of headers, with their keys lowercased. */
	AStringMap m_Headers;

	/** Type of the content; parsed by AddHeader(), set directly by SetContentLength() */
	AString m_ContentType;

	/** Length of the content that is to be received.
	AString::npos when the object is created.
	Parsed by AddHeader() or set directly by SetContentLength() */
	size_t m_ContentLength;
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




