
// HTTPMessage.h

// Declares the cHTTPMessage class representing the common ancestor for HTTP request and response classes





#pragma once





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
	virtual void AddHeader(const AString & a_Key, const AString & a_Value);

	void SetContentType  (const AString & a_ContentType) { m_ContentType   = a_ContentType; }
	void SetContentLength(size_t a_ContentLength)        { m_ContentLength = a_ContentLength; }

	const AString & GetContentType  (void) const { return m_ContentType; }
	size_t          GetContentLength(void) const { return m_ContentLength; }

protected:

	using cNameValueMap = std::map<AString, AString>;


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





/** Stores outgoing response headers and serializes them to an HTTP data stream. */
class cHTTPOutgoingResponse:
	public cHTTPMessage
{
	using Super = cHTTPMessage;

public:

	cHTTPOutgoingResponse(void);

	/** Appends the response to the specified datastream - response line and headers.
	The body will be sent later directly through cConnection::Send() */
	void AppendToData(AString & a_DataStream) const;
} ;





/** Provides storage for an incoming HTTP request. */
class cHTTPIncomingRequest:
	public cHTTPMessage
{
	using Super = cHTTPMessage;

public:

	/** Base class for anything that can be used as the UserData for the request. */
	class cUserData
	{
	public:
		// Force a virtual destructor in descendants:
		virtual ~cUserData() {}
	};
	using cUserDataPtr = std::shared_ptr<cUserData>;


	/** Creates a new instance of the class, containing the method and URL provided by the client. */
	cHTTPIncomingRequest(const AString & a_Method, const AString & a_URL);

	/** Returns the method used in the request */
	const AString & GetMethod(void) const { return m_Method; }

	/** Returns the URL used in the request */
	const AString & GetURL(void) const { return m_URL; }

	/** Returns the path part of the URL. */
	AString GetURLPath(void) const;

	/** Returns true if the request has had the Auth header present. */
	bool HasAuth(void) const { return m_HasAuth; }

	/** Returns the username that the request presented. Only valid if HasAuth() is true */
	const AString & GetAuthUsername(void) const { return m_AuthUsername; }

	/** Returns the password that the request presented. Only valid if HasAuth() is true */
	const AString & GetAuthPassword(void) const { return m_AuthPassword; }

	bool DoesAllowKeepAlive(void) const { return m_AllowKeepAlive; }

	/** Attaches any kind of data to this request, to be later retrieved by GetUserData(). */
	void SetUserData(cUserDataPtr a_UserData) { m_UserData = std::move(a_UserData); }

	/** Returns the data attached to this request by the class client. */
	cUserDataPtr GetUserData(void) { return m_UserData; }

	/** Adds the specified header into the internal list of headers.
	Overrides the parent to add recognizing additional headers: auth and keepalive. */
	virtual void AddHeader(const AString & a_Key, const AString & a_Value) override;

protected:

	/** Method of the request (GET / PUT / POST / ...) */
	AString m_Method;

	/** Full URL of the request */
	AString m_URL;

	/** Set to true if the request contains auth data that was understood by the parser */
	bool m_HasAuth;

	/** The username used for auth */
	AString m_AuthUsername;

	/** The password used for auth */
	AString m_AuthPassword;

	/** Set to true if the request indicated that it supports keepalives.
	If false, the server will close the connection once the request is finished */
	bool m_AllowKeepAlive;

	/** Any data attached to the request by the class client. */
	cUserDataPtr m_UserData;
};
