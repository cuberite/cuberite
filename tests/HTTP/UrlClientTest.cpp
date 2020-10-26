
#include "Globals.h"
#include "../TestHelpers.h"
#include "HTTP/UrlClient.h"
#include "OSSupport/NetworkSingleton.h"




namespace
{

/** Track number of cCallbacks instances alive. */
std::atomic<int> g_ActiveCallbacks{ 0 };

/** Simple callbacks that dump the events to the console and signalize a cEvent when the request is finished. */
class cCallbacks:
	public cUrlClient::cCallbacks
{
public:
	cCallbacks(std::shared_ptr<cEvent> a_Event):
		m_Event(std::move(a_Event))
	{
		++g_ActiveCallbacks;
		LOGD("Created a cCallbacks instance at %p", reinterpret_cast<void *>(this));
	}


	virtual ~cCallbacks() override
	{
		--g_ActiveCallbacks;
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
		m_Event->Set();
	}


	virtual void OnRedirecting(const AString & a_RedirectUrl) override
	{
		LOG("Redirecting to \"%s\".", a_RedirectUrl.c_str());
	}


	virtual void OnError(const AString & a_ErrorMsg) override
	{
		LOG("Error: %s", a_ErrorMsg.c_str());
		m_Event->Set();
	}

protected:
	std::shared_ptr<cEvent> m_Event;
};





int TestRequest1()
{
	LOG("Running test 1");
	auto evtFinished = std::make_shared<cEvent>();
	auto callbacks = std::make_unique<cCallbacks>(evtFinished);
	AStringMap options;
	options["MaxRedirects"] = "0";
	auto res = cUrlClient::Get("http://github.com", std::move(callbacks), AStringMap(), AString(), options);
	if (res.first)
	{
		evtFinished->Wait();
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
	auto evtFinished = std::make_shared<cEvent>();
	auto callbacks = std::make_unique<cCallbacks>(evtFinished);
	auto res = cUrlClient::Get("http://github.com", std::move(callbacks));
	if (res.first)
	{
		evtFinished->Wait();
	}
	else
	{
		LOG("Immediate error: %s", res.second.c_str());
		return 1;
	}
	return 0;
}





int TestRequest3()
{
	LOG("Running test 3");
	auto evtFinished = std::make_shared<cEvent>();
	auto callbacks = std::make_unique<cCallbacks>(evtFinished);
	AStringMap options;
	options["MaxRedirects"] = "0";
	auto res = cUrlClient::Get("https://github.com", std::move(callbacks), AStringMap(), AString(), options);
	if (res.first)
	{
		evtFinished->Wait();
	}
	else
	{
		LOG("Immediate error: %s", res.second.c_str());
		return 1;
	}
	return 0;
}





int TestRequest4()
{
	LOG("Running test 4");
	auto evtFinished = std::make_shared<cEvent>();
	auto callbacks = std::make_unique<cCallbacks>(evtFinished);
	auto res = cUrlClient::Get("https://github.com", std::move(callbacks));
	if (res.first)
	{
		evtFinished->Wait();
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
	using func_t = int(void);
	func_t * tests[] =
	{
		&TestRequest1,
		&TestRequest2,
		&TestRequest3,
		&TestRequest4,
	};
	for (auto test: tests)
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

}  // namespace (anonymous)





IMPLEMENT_TEST_MAIN("UrlClient",
	LOG("Initializing cNetwork...");
	cNetworkSingleton::Get().Initialise();
	LOG("Testing...");
	TEST_EQUAL(TestRequests(), 0);
	LOG("Terminating cNetwork...");
	cNetworkSingleton::Get().Terminate();

	// No leaked callback instances
	LOG("cCallback instances still alive: %d", g_ActiveCallbacks.load());
	TEST_EQUAL(g_ActiveCallbacks, 0);
)
