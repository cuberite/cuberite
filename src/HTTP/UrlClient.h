
// UrlClient.h

// Declares the cUrlClient class for high-level URL interaction

/*
Options that can be set via the Options parameter to the cUrlClient calls:
"MaxRedirects":       The maximum number of allowed redirects before the client refuses a redirect with an error
"OwnCert":            The client certificate to use, if requested by the server. Any string that can be parsed by cX509Cert.
"OwnPrivKey":         The private key appropriate for OwnCert. Any string that can be parsed by cCryptoKey.
"OwnPrivKeyPassword": The password for OwnPrivKey. If not present or empty, no password is assumed.

Behavior:
- If a redirect is received, and redirection is allowed, the redirection is reported via OnRedirecting() callback
and the request is restarted at the redirect URL, without reporting any of the redirect's headers nor body
- If a redirect is received and redirection is not allowed (maximum redirection attempts have been reached),
the OnRedirecting() callback is called with the redirect URL and then the request terminates with an OnError() callback,
without reporting the redirect's headers nor body.
*/





#pragma once

#include "../OSSupport/Network.h"





class cUrlClient
{
public:
	/** Callbacks that are used for progress and result reporting. */
	class cCallbacks
	{
	public:
		// Force a virtual destructor in descendants:
		virtual ~cCallbacks() {}

		/** Called when the TCP connection is established. */
		virtual void OnConnected(cTCPLink & a_Link) {}

		/** Called for TLS connections, when the server certificate is received.
		Return true to continue with the request, false to abort.
		The default implementation does nothing and continues with the request.
		TODO: The certificate parameter needs a representation! */
		virtual bool OnCertificateReceived() { return true; }

		/** Called for TLS connections, when the TLS handshake has been completed.
		An empty default implementation is provided so that clients don't need to reimplement it unless they are interested in the event. */
		virtual void OnTlsHandshakeCompleted() { }

		/** Called after the entire request has been sent to the remote peer. */
		virtual void OnRequestSent() {}

		/** Called after the first line of the response is parsed, unless the response is an allowed redirect. */
		virtual void OnStatusLine(const AString & a_HttpVersion, int a_StatusCode, const AString & a_Rest) {}

		/** Called when a single HTTP header is received and parsed, unless the response is an allowed redirect
		Called once for each incoming header. */
		virtual void OnHeader(const AString & a_Key, const AString & a_Value) {}

		/** Called when the HTTP headers have been fully parsed, unless the response is an allowed redirect.
		There will be no more OnHeader() calls. */
		virtual void OnHeadersFinished() {}

		/** Called when the next fragment of the response body is received, unless the response is an allowed redirect.
		This can be called multiple times, as data arrives over the network. */
		virtual void OnBodyData(const void * a_Data, size_t a_Size) {}

		/** Called after the response body has been fully reported by OnBody() calls, unless the response is an allowed redirect.
		There will be no more OnBody() calls. */
		virtual void OnBodyFinished() {}

		/** Called when an asynchronous error is encountered. */
		virtual void OnError(const AString & a_ErrorMsg) {}

		/** Called when a redirect is to be followed.
		This is called even if the redirecting is prohibited by the options; in such an event, this call will be
		followed by OnError().
		If a response indicates a redirect (and the request allows redirecting), the regular callbacks
		OnStatusLine(), OnHeader(), OnHeadersFinished(), OnBodyData() and OnBodyFinished() are not called
		for such a response; instead, the redirect is silently attempted. */
		virtual void OnRedirecting(const AString & a_NewLocation) {}
	};
	typedef std::unique_ptr<cCallbacks> cCallbacksPtr;


	/** Used for HTTP status codes. */
	enum eHTTPStatus
	{
		HTTP_STATUS_OK                 = 200,
		HTTP_STATUS_MULTIPLE_CHOICES   = 300,  // MAY have a redirect using the "Location" header
		HTTP_STATUS_MOVED_PERMANENTLY  = 301,  // redirect using the "Location" header
		HTTP_STATUS_FOUND              = 302,  // redirect using the "Location" header
		HTTP_STATUS_SEE_OTHER          = 303,  // redirect using the "Location" header
		HTTP_STATUS_TEMPORARY_REDIRECT = 307,  // redirect using the "Location" header
	};


	/** Makes a network request to the specified URL, using the specified method (if applicable).
	The response is reported via the a_ResponseCallback callback, in a single call.
	The metadata about the response (HTTP headers) are reported via a_InfoCallback before the a_ResponseCallback call.
	If there is an asynchronous error, it is reported in via the a_ErrorCallback.
	If there is an immediate error (misformatted URL etc.), the function returns false and an error message.
	a_Headers contains additional headers to use for the request.
	a_Body specifies optional body to include with the request, if applicable.
	a_Options contains various options for the request that govern the request behavior, but aren't sent to the server,
	such as the proxy server, whether to follow redirects, and client certificate for TLS. */
	static std::pair<bool, AString> Request(
		const AString & a_Method,
		const AString & a_URL,
		cCallbacksPtr && a_Callbacks,
		AStringMap && a_Headers,
		AString && a_Body,
		AStringMap && a_Options
	);

	/** Alias for Request("GET", ...) */
	static std::pair<bool, AString> Get(
		const AString & a_URL,
		cCallbacksPtr && a_Callbacks,
		AStringMap a_Headers = AStringMap(),
		const AString & a_Body = AString(),
		AStringMap a_Options = AStringMap()
	);

	/** Alias for Request("POST", ...) */
	static std::pair<bool, AString> Post(
		const AString & a_URL,
		cCallbacksPtr && a_Callbacks,
		AStringMap && a_Headers,
		AString && a_Body,
		AStringMap && a_Options
	);

	/** Alias for Request("PUT", ...) */
	static std::pair<bool, AString> Put(
		const AString & a_URL,
		cCallbacksPtr && a_Callbacks,
		AStringMap && a_Headers,
		AString && a_Body,
		AStringMap && a_Options
	);
};




