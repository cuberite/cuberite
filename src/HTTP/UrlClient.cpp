
// UrlClient.cpp

// Implements the cUrlClient class for high-level URL interaction

#include "Globals.h"
#include "UrlClient.h"
#include "UrlParser.h"
#include "HTTPMessageParser.h"
#include "../mbedTLS++/X509Cert.h"
#include "../mbedTLS++/CryptoKey.h"





// fwd:
class cSchemeHandler;
typedef std::shared_ptr<cSchemeHandler> cSchemeHandlerPtr;





class cUrlClientRequest:
	public cNetwork::cConnectCallbacks,
	public cTCPLink::cCallbacks
{
	friend class cHttpSchemeHandler;

public:
	static std::pair<bool, AString> Request(
		const AString & a_Method,
		const AString & a_URL,
		cUrlClient::cCallbacksPtr && a_Callbacks,
		AStringMap && a_Headers,
		const AString & a_Body,
		AStringMap && a_Options
	)
	{
		// Create a new instance of cUrlClientRequest, wrapped in a SharedPtr, so that it has a controlled lifetime.
		// Cannot use std::make_shared, because the constructor is not public
		std::shared_ptr<cUrlClientRequest> ptr (new cUrlClientRequest(
			a_Method, a_URL, std::move(a_Callbacks), std::move(a_Headers), a_Body, std::move(a_Options)
		));
		return ptr->DoRequest(ptr);
	}


	/** Calls the error callback with the specified message, if it exists, and terminates the request. */
	void CallErrorCallback(const AString & a_ErrorMessage)
	{
		// Call the error callback:
		m_Callbacks->OnError(a_ErrorMessage);

		// Terminate the request's TCP link:
		if (auto link = m_Link.lock())
		{
			link->Close();
		}
	}


	cUrlClient::cCallbacks & GetCallbacks() { return *m_Callbacks; }

	void RedirectTo(const AString & a_RedirectUrl);

	bool ShouldAllowRedirects() const;

	cX509CertPtr GetOwnCert() const
	{
		auto itr = m_Options.find("OwnCert");
		if (itr == m_Options.end())
		{
			return nullptr;
		}
		cX509CertPtr cert = std::make_shared<cX509Cert>();
		if (!cert->Parse(itr->second.data(), itr->second.size()))
		{
			LOGD("OwnCert failed to parse");
			return nullptr;
		}
		return cert;
	}

	cCryptoKeyPtr GetOwnPrivKey() const
	{
		auto itr = m_Options.find("OwnPrivKey");
		if (itr == m_Options.end())
		{
			return nullptr;
		}
		cCryptoKeyPtr key = std::make_shared<cCryptoKey>();
		auto passItr = m_Options.find("OwnPrivKeyPassword");
		auto pass = (passItr == m_Options.end()) ? AString() : passItr->second;
		if (!key->ParsePrivate(itr->second.data(), itr->second.size(), pass))
		{
			return nullptr;
		}
		return key;
	}

protected:

	/** Method to be used for the request */
	AString m_Method;

	/** URL that will be requested. */
	AString m_Url;

	/** Individual components of the URL that will be requested. */
	AString m_UrlScheme, m_UrlUsername, m_UrlPassword, m_UrlHost, m_UrlPath, m_UrlQuery, m_UrlFragment;
	UInt16 m_UrlPort;

	/** Callbacks that report progress and results of the request. */
	cUrlClient::cCallbacksPtr m_Callbacks;

	/** Extra headers to be sent with the request (besides the normal ones). */
	AStringMap m_Headers;

	/** Body to be sent with the request, if any. */
	AString m_Body;

	/** Extra options to be used for the request. */
	AStringMap m_Options;

	/** weak_ptr to self, so that this object can keep itself alive as needed by calling lock(),
	and pass self as callbacks to cNetwork functions. */
	std::weak_ptr<cUrlClientRequest> m_Self;

	/** The handler that "talks" the protocol specified in m_UrlScheme, handles all the sending and receiving. */
	std::shared_ptr<cSchemeHandler> m_SchemeHandler;

	/** The link handling the request. */
	std::weak_ptr<cTCPLink> m_Link;

	/** The number of redirect attempts that will still be followed.
	If the response specifies a redirect and this is nonzero, the redirect is followed.
	If the response specifies a redirect and this is zero, a redirect loop is reported as an error. */
	int m_NumRemainingRedirects;


	cUrlClientRequest(
		const AString & a_Method,
		const AString & a_Url,
		cUrlClient::cCallbacksPtr && a_Callbacks,
		AStringMap && a_Headers,
		const AString & a_Body,
		AStringMap && a_Options
	):
		m_Method(a_Method),
		m_Url(a_Url),
		m_Callbacks(std::move(a_Callbacks)),
		m_Headers(std::move(a_Headers)),
		m_Body(a_Body),
		m_Options(std::move(a_Options))
	{
		m_NumRemainingRedirects = GetStringMapInteger(m_Options, "MaxRedirects", 30);
	}


	std::pair<bool, AString> DoRequest(const std::shared_ptr<cUrlClientRequest> & a_Self);


	// cNetwork::cConnectCallbacks override: TCP link connected:
	virtual void OnConnected(cTCPLink & a_Link) override;

	// cNetwork::cConnectCallbacks override: An error has occurred:
	virtual void OnError(int a_ErrorCode, const AString & a_ErrorMsg) override
	{
		m_Callbacks->OnError(Printf("Network error %d (%s)", a_ErrorCode, a_ErrorMsg.c_str()));
	}


	// cTCPLink::cCallbacks override: TCP link created
	virtual void OnLinkCreated(cTCPLinkPtr a_Link) override
	{
		m_Link = a_Link;
	}


	// cTCPLink::cCallbacks override: TLS handshake completed on the link:
	virtual void OnTlsHandshakeCompleted(void) override;

	/** Called when there's data incoming from the remote peer. */
	virtual void OnReceivedData(const char * a_Data, size_t a_Length) override;


	/** Called when the remote end closes the connection.
	The link is still available for connection information query (IP / port).
	Sending data on the link is not an error, but the data won't be delivered. */
	virtual void OnRemoteClosed(void) override;
};





/** Represents a base class for an object that "talks" a specified URL protocol, such as HTTP or FTP.
Also provides a static factory method for creating an instance based on the scheme.
A descendant of this class is created for each request and handles all of the request's aspects,
from right after connecting to the TCP link till the link is closed.
For an example of a specific handler, see the cHttpSchemeHandler class. */
class cSchemeHandler abstract
{
public:
	cSchemeHandler(cUrlClientRequest & a_ParentRequest):
		m_ParentRequest(a_ParentRequest)
	{
	}

	// Force a virtual destructor in all descendants
	virtual ~cSchemeHandler() {}

	/** Creates and returns a new handler for the specified scheme.
	a_ParentRequest is the request which is to be handled by the handler. */
	static cSchemeHandlerPtr Create(const AString & a_Scheme, cUrlClientRequest & a_ParentRequest);

	/** Called when the link gets established. */
	virtual void OnConnected(cTCPLink & a_Link) = 0;

	/** Called when there's data incoming from the remote peer. */
	virtual void OnReceivedData(const char * a_Data, size_t a_Length) = 0;

	/** Called when the TLS handshake has completed on the underlying link. */
	virtual void OnTlsHandshakeCompleted(void) = 0;

	/** Called when the remote end closes the connection.
	The link is still available for connection information query (IP / port).
	Sending data on the link is not an error, but the data won't be delivered. */
	virtual void OnRemoteClosed(void) = 0;

protected:
	cUrlClientRequest & m_ParentRequest;
};





/** cSchemeHandler descendant that handles HTTP (and HTTPS) requests. */
class cHttpSchemeHandler:
	public cSchemeHandler,
	protected cHTTPMessageParser::cCallbacks
{
	using Super = cSchemeHandler;

public:

	cHttpSchemeHandler(cUrlClientRequest & a_ParentRequest, bool a_IsTls):
		Super(a_ParentRequest),
		m_Parser(*this),
		m_IsTls(a_IsTls),
		m_IsRedirect(false)
	{
	}


	virtual void OnConnected(cTCPLink & a_Link) override
	{
		m_Link = &a_Link;
		if (m_IsTls)
		{
			m_Link->StartTLSClient(m_ParentRequest.GetOwnCert(), m_ParentRequest.GetOwnPrivKey());
		}
		else
		{
			SendRequest();
		}
	}


	/** Sends the HTTP request over the link.
	Common code for both HTTP and HTTPS. */
	void SendRequest()
	{
		// Compose the request line:
		auto requestLine = m_ParentRequest.m_UrlPath;
		if (requestLine.empty())
		{
			requestLine = "/";
		}
		if (!m_ParentRequest.m_UrlQuery.empty())
		{
			requestLine.push_back('?');
			requestLine.append(m_ParentRequest.m_UrlQuery);
		}
		m_Link->Send(Printf("%s %s HTTP/1.1\r\n", m_ParentRequest.m_Method.c_str(), requestLine.c_str()));

		// Send the headers:
		m_Link->Send(Printf("Host: %s\r\n", m_ParentRequest.m_UrlHost.c_str()));
		m_Link->Send(Printf("Content-Length: %u\r\n", static_cast<unsigned>(m_ParentRequest.m_Body.size())));
		for (auto itr = m_ParentRequest.m_Headers.cbegin(), end = m_ParentRequest.m_Headers.cend(); itr != end; ++itr)
		{
			m_Link->Send(Printf("%s: %s\r\n", itr->first.c_str(), itr->second.c_str()));
		}  // for itr - m_Headers[]
		m_Link->Send("\r\n", 2);

		// Send the body:
		m_Link->Send(m_ParentRequest.m_Body);

		// Notify the callbacks that the request has been sent:
		m_ParentRequest.GetCallbacks().OnRequestSent();
	}


	virtual void OnReceivedData(const char * a_Data, size_t a_Length) override
	{
		auto res = m_Parser.Parse(a_Data, a_Length);
		if (res == AString::npos)
		{
			m_ParentRequest.CallErrorCallback("Failed to parse HTTP response");
			return;
		}
	}


	virtual void OnTlsHandshakeCompleted(void) override
	{
		SendRequest();
	}


	virtual void OnRemoteClosed(void) override
	{
		m_Link = nullptr;
	}


	// cHTTPResponseParser::cCallbacks overrides:
	virtual void OnError(const AString & a_ErrorDescription) override
	{
		m_Link = nullptr;
		m_ParentRequest.CallErrorCallback(a_ErrorDescription);
	}


	virtual void OnFirstLine(const AString & a_FirstLine) override
	{
		// Find the first space, parse the result code between it and the second space:
		auto idxFirstSpace = a_FirstLine.find(' ');
		if (idxFirstSpace == AString::npos)
		{
			m_ParentRequest.CallErrorCallback(Printf("Failed to parse HTTP status line \"%s\", no space delimiter.", a_FirstLine.c_str()));
			return;
		}
		auto idxSecondSpace = a_FirstLine.find(' ', idxFirstSpace + 1);
		if (idxSecondSpace == AString::npos)
		{
			m_ParentRequest.CallErrorCallback(Printf("Failed to parse HTTP status line \"%s\", missing second space delimiter.", a_FirstLine.c_str()));
			return;
		}
		int resultCode;
		auto resultCodeStr = a_FirstLine.substr(idxFirstSpace + 1, idxSecondSpace - idxFirstSpace - 1);
		if (!StringToInteger(resultCodeStr, resultCode))
		{
			m_ParentRequest.CallErrorCallback(Printf("Failed to parse HTTP result code from response \"%s\"", resultCodeStr.c_str()));
			return;
		}

		// Check for redirects, follow if allowed by the options:
		switch (resultCode)
		{
			case cUrlClient::HTTP_STATUS_MULTIPLE_CHOICES:
			case cUrlClient::HTTP_STATUS_MOVED_PERMANENTLY:
			case cUrlClient::HTTP_STATUS_FOUND:
			case cUrlClient::HTTP_STATUS_SEE_OTHER:
			case cUrlClient::HTTP_STATUS_TEMPORARY_REDIRECT:
			{
				m_IsRedirect = true;
				return;
			}
		}
		m_ParentRequest.GetCallbacks().OnStatusLine(a_FirstLine.substr(1, idxFirstSpace), resultCode, a_FirstLine.substr(idxSecondSpace + 1));
	}


	virtual void OnHeaderLine(const AString & a_Key, const AString & a_Value) override
	{
		if (m_IsRedirect)
		{
			if (a_Key == "Location")
			{
				m_RedirectLocation = a_Value;
			}
		}
		else
		{
			m_ParentRequest.GetCallbacks().OnHeader(a_Key, a_Value);
		}
	}


	/** Called when all the headers have been parsed. */
	virtual void OnHeadersFinished(void) override
	{
		if (!m_IsRedirect)
		{
			m_ParentRequest.GetCallbacks().OnHeadersFinished();
		}
	}


	/** Called for each chunk of the incoming body data. */
	virtual void OnBodyData(const void * a_Data, size_t a_Size) override
	{
		if (!m_IsRedirect)
		{
			m_ParentRequest.GetCallbacks().OnBodyData(a_Data, a_Size);
		}
	}


	/** Called when the entire body has been reported by OnBodyData(). */
	virtual void OnBodyFinished(void) override
	{
		if (m_IsRedirect)
		{
			if (m_RedirectLocation.empty())
			{
				m_ParentRequest.CallErrorCallback("Invalid redirect, there's no location to redirect to");
			}
			else
			{
				m_ParentRequest.RedirectTo(m_RedirectLocation);
			}
		}
		else
		{
			m_ParentRequest.GetCallbacks().OnBodyFinished();
			// Finished recieving data, shutdown the link
			m_Link->Shutdown();
		}
	}

protected:

	/** The network link. */
	cTCPLink * m_Link;

	/** Parser of the HTTP response message. */
	cHTTPMessageParser m_Parser;

	/** If true, the TLS should be started on the link before sending the request (used for https). */
	bool m_IsTls;

	/** Set to true if the first line contains a redirecting HTTP status code and the options specify to follow redirects.
	If true, and the parent request allows redirects, neither headers not the body contents are reported through the callbacks,
	and after the entire request is parsed, the redirect is attempted. */
	bool m_IsRedirect;

	/** The Location where the request should be redirected.
	Only used when m_IsRedirect is true. */
	AString m_RedirectLocation;
};





////////////////////////////////////////////////////////////////////////////////
// cSchemeHandler:

cSchemeHandlerPtr cSchemeHandler::Create(const AString & a_Scheme, cUrlClientRequest & a_ParentRequest)
{
	auto lowerScheme = StrToLower(a_Scheme);
	if (lowerScheme == "http")
	{
		return std::make_shared<cHttpSchemeHandler>(a_ParentRequest, false);
	}
	else if (lowerScheme == "https")
	{
		return std::make_shared<cHttpSchemeHandler>(a_ParentRequest, true);
	}

	return nullptr;
}





////////////////////////////////////////////////////////////////////////////////
// cUrlClientRequest:

void cUrlClientRequest::RedirectTo(const AString & a_RedirectUrl)
{
	// Check that redirection is allowed:
	m_Callbacks->OnRedirecting(a_RedirectUrl);
	if (!ShouldAllowRedirects())
	{
		CallErrorCallback(Printf("Redirect to \"%s\" not allowed", a_RedirectUrl.c_str()));
		return;
	}

	// Keep ourself alive while the link drops ownership
	auto Self = m_Self.lock();

	// Do the actual redirect:
	if (auto Link = m_Link.lock())
	{
		Link->Close();
	}

	m_Url = a_RedirectUrl;
	m_NumRemainingRedirects = m_NumRemainingRedirects - 1;
	auto res = DoRequest(Self);
	if (!res.first)
	{
		m_Callbacks->OnError(Printf("Redirection failed: %s", res.second.c_str()));
	}
}





bool cUrlClientRequest::ShouldAllowRedirects() const
{
	return (m_NumRemainingRedirects > 0);
}





void cUrlClientRequest::OnConnected(cTCPLink & a_Link)
{
	m_Callbacks->OnConnected(a_Link);
	m_SchemeHandler->OnConnected(a_Link);
}





void cUrlClientRequest::OnTlsHandshakeCompleted(void)
{
	// Notify the scheme handler and the callbacks:
	m_SchemeHandler->OnTlsHandshakeCompleted();
	m_Callbacks->OnTlsHandshakeCompleted();
}





void cUrlClientRequest::OnReceivedData(const char * a_Data, size_t a_Length)
{
	auto handler = m_SchemeHandler;
	if (handler != nullptr)
	{
		handler->OnReceivedData(a_Data, a_Length);
	}
}





void cUrlClientRequest::OnRemoteClosed()
{
	// Notify the callback:
	auto handler = m_SchemeHandler;
	if (handler != nullptr)
	{
		handler->OnRemoteClosed();
	}
}





std::pair<bool, AString> cUrlClientRequest::DoRequest(const std::shared_ptr<cUrlClientRequest> & a_Self)
{
	// We need a shared pointer to self, care must be taken not to pass any other ptr:
	ASSERT(a_Self.get() == this);

	m_Self = a_Self;

	// Parse the URL:
	auto res = cUrlParser::Parse(m_Url, m_UrlScheme, m_UrlUsername, m_UrlPassword, m_UrlHost, m_UrlPort, m_UrlPath, m_UrlQuery, m_UrlFragment);
	if (!res.first)
	{
		return res;
	}

	// Get a handler that will work with the specified scheme:
	m_SchemeHandler = cSchemeHandler::Create(m_UrlScheme, *this);
	if (m_SchemeHandler == nullptr)
	{
		return std::make_pair(false, Printf("Unknown Url scheme: %s", m_UrlScheme.c_str()));
	}

	// Connect and transfer ownership to the link
	if (!cNetwork::Connect(m_UrlHost, m_UrlPort, a_Self, a_Self))
	{
		return std::make_pair(false, "Network connection failed");
	}
	return std::make_pair(true, AString());
}





////////////////////////////////////////////////////////////////////////////////
// cUrlClient:

std::pair<bool, AString> cUrlClient::Request(
	const AString & a_Method,
	const AString & a_URL,
	cCallbacksPtr && a_Callbacks,
	AStringMap && a_Headers,
	AString && a_Body,
	AStringMap && a_Options
)
{
	return cUrlClientRequest::Request(
		a_Method, a_URL, std::move(a_Callbacks), std::move(a_Headers), std::move(a_Body), std::move(a_Options)
	);
}





std::pair<bool, AString> cUrlClient::Get(
	const AString & a_URL,
	cCallbacksPtr && a_Callbacks,
	AStringMap a_Headers,
	const AString & a_Body,
	AStringMap a_Options
)
{
	return cUrlClientRequest::Request(
		"GET", a_URL, std::move(a_Callbacks), std::move(a_Headers), std::move(a_Body), std::move(a_Options)
	);
}





std::pair<bool, AString> cUrlClient::Post(
	const AString & a_URL,
	cCallbacksPtr && a_Callbacks,
	AStringMap && a_Headers,
	AString && a_Body,
	AStringMap && a_Options
)
{
	return cUrlClientRequest::Request(
		"POST", a_URL, std::move(a_Callbacks), std::move(a_Headers), std::move(a_Body), std::move(a_Options)
	);
}





std::pair<bool, AString> cUrlClient::Put(
	const AString & a_URL,
	cCallbacksPtr && a_Callbacks,
	AStringMap && a_Headers,
	AString && a_Body,
	AStringMap && a_Options
)
{
	return cUrlClientRequest::Request(
		"PUT", a_URL, std::move(a_Callbacks), std::move(a_Headers), std::move(a_Body), std::move(a_Options)
	);
}





