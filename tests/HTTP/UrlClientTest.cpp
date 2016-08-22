
#include "Globals.h"
#include "HTTP/UrlClient.h"
#include "OSSupport/NetworkSingleton.h"





class cCallbacks:
	public cUrlClient::cCallbacks
{
public:
	cCallbacks(cEvent & a_Event):
		m_Event(a_Event)
	{
	}

	virtual void OnConnected(cTCPLink & a_Link) override
	{
		LOG("Link connected to %s:%u", a_Link.GetRemoteIP().c_str(), a_Link.GetRemotePort());
	}

	virtual bool OnCertificateReceived() override
	{
		LOG("Server certificate received");
		return true;
	}

	virtual void OnRequestSent() override
	{
		LOG("Request has been sent");
	}

	virtual void OnHeader(const AString & a_Key, const AString & a_Value) override
	{
		LOG("HTTP Header: \"%s\" -> \"%s\"", a_Key.c_str(), a_Value.c_str());
	}

	virtual void OnHeadersFinished() override
	{
		LOG("HTTP headers finished.");
	}

	virtual void OnBodyData(const void * a_Data, size_t a_Size) override
	{
		AString body(reinterpret_cast<const char *>(a_Data), a_Size);
		LOG("Body part:\n%s", body.c_str());
	}

	/** Called after the response body has been fully reported by OnBody() calls.
	There will be no more OnBody() calls. */
	virtual void OnBodyFinished() override
	{
		LOG("Body finished.");
		m_Event.Set();
	}

	virtual void OnRedirecting(const AString & a_RedirectUrl) override
	{
		LOG("Redirecting to \"%s\".", a_RedirectUrl.c_str());
	}

	virtual void OnError(const AString & a_ErrorMsg) override
	{
		LOG("Error: %s", a_ErrorMsg.c_str());
		m_Event.Set();
	}

protected:
	cEvent & m_Event;
};





int TestRequest1()
{
	LOG("Running test 1");
	cEvent evtFinished;
	cCallbacks callbacks(evtFinished);
	AStringMap options;
	options["MaxRedirects"] = "0";
	auto res = cUrlClient::Get("http://github.com", callbacks, AStringMap(), AString(), options);
	if (res.first)
	{
		evtFinished.Wait();
	}
	else
	{
		LOG("Immediate error: %s", res.second.c_str());
		return 1;
	}
	return 0;
}





int TestRequest2()
{
	LOG("Running test 2");
	cEvent evtFinished;
	cCallbacks callbacks(evtFinished);
	auto res = cUrlClient::Get("http://github.com", callbacks);
	if (res.first)
	{
		evtFinished.Wait();
	}
	else
	{
		LOG("Immediate error: %s", res.second.c_str());
		return 1;
	}
	return 0;
}





int TestRequests()
{
	auto res = TestRequest1();
	if (res != 0)
	{
		return res;
	}
	res = TestRequest2();
	if (res != 0)
	{
		return res;
	}
	return 0;
}





int main()
{
	LOGD("Test started");

	LOGD("Initializing cNetwork...");
	cNetworkSingleton::Get().Initialise();

	LOGD("Testing...");
	auto res = TestRequests();

	LOGD("Terminating cNetwork...");
	cNetworkSingleton::Get().Terminate();
	LOGD("cUrlClient test finished");

	return res;
}




