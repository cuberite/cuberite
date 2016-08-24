
#include "Globals.h"
#include "HTTP/UrlClient.h"
#include "OSSupport/NetworkSingleton.h"





/** Simple callbacks that dump the events to the console and signalize a cEvent when the request is finished. */
class cCallbacks:
	public cUrlClient::cCallbacks
{
public:
	cCallbacks(cEvent & a_Event):
		m_Event(a_Event)
	{
		LOGD("Created a cCallbacks instance at %p", reinterpret_cast<void *>(this));
	}


	~cCallbacks()
	{
		LOGD("Deleting the cCallbacks instance at %p", reinterpret_cast<void *>(this));
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


	virtual void OnTlsHandshakeCompleted() override
	{
		LOG("TLS handshake has completed.");
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
		#if 0
			// Output the whole received data blob:
			AString body(reinterpret_cast<const char *>(a_Data), a_Size);
			LOG("Body part:\n%s", body.c_str());
		#else
			// Output only the data size, to keep the log short:
			LOG("Body part: %u bytes", static_cast<unsigned>(a_Size));
		#endif
	}


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





static int TestRequest1()
{
	LOG("Running test 1");
	cEvent evtFinished;
	auto callbacks = cpp14::make_unique<cCallbacks>(evtFinished);
	AStringMap options;
	options["MaxRedirects"] = "0";
	auto res = cUrlClient::Get("http://github.com", std::move(callbacks), AStringMap(), AString(), options);
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





static int TestRequest2()
{
	LOG("Running test 2");
	cEvent evtFinished;
	auto callbacks = cpp14::make_unique<cCallbacks>(evtFinished);
	auto res = cUrlClient::Get("http://github.com", std::move(callbacks));
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





static int TestRequest3()
{
	LOG("Running test 3");
	cEvent evtFinished;
	auto callbacks = cpp14::make_unique<cCallbacks>(evtFinished);
	AStringMap options;
	options["MaxRedirects"] = "0";
	auto res = cUrlClient::Get("https://github.com", std::move(callbacks), AStringMap(), AString(), options);
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





static int TestRequest4()
{
	LOG("Running test 4");
	cEvent evtFinished;
	auto callbacks = cpp14::make_unique<cCallbacks>(evtFinished);
	auto res = cUrlClient::Get("https://github.com", std::move(callbacks));
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





static int TestRequests()
{
	std::function<int(void)> tests[] =
	{
		&TestRequest1,
		&TestRequest2,
		&TestRequest3,
		&TestRequest4,
	};
	for (const auto & test: tests)
	{
		LOG("%s", AString(60, '-').c_str());
		auto res = test();
		if (res != 0)
		{
			return res;
		}
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




