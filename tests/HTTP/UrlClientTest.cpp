
#include "Globals.h"
#include "../TestHelpers.h"
#include "HTTP/UrlClient.h"
#include "OSSupport/NetworkSingleton.h"




namespace
{





// When under debugger, set timeouts to practically "never" so that there's time to step through the code:
#ifdef NDEBUG
	static const UInt32 TIMEOUT = 10000;  // 10 seconds
#else
	static const UInt32 TIMEOUT = 0xffffffff;  // ~4.3M seconds / ~49 days
#endif





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





/** The trusted root CAs for individual servers. */
namespace TrustedCAs
{
	// DigiCert Global Root CA (sessionserver.mojang.com)
	// Downloaded from https://www.digicert.com/kb/digicert-root-certificates.htm
	// DigiCert Global Root CA
	static const char MojangComSessionServer[] =
		"-----BEGIN CERTIFICATE-----\n"
		"MIIDjjCCAnagAwIBAgIQAzrx5qcRqaC7KGSxHQn65TANBgkqhkiG9w0BAQsFADBh\n"
		"MQswCQYDVQQGEwJVUzEVMBMGA1UEChMMRGlnaUNlcnQgSW5jMRkwFwYDVQQLExB3\n"
		"d3cuZGlnaWNlcnQuY29tMSAwHgYDVQQDExdEaWdpQ2VydCBHbG9iYWwgUm9vdCBH\n"
		"MjAeFw0xMzA4MDExMjAwMDBaFw0zODAxMTUxMjAwMDBaMGExCzAJBgNVBAYTAlVT\n"
		"MRUwEwYDVQQKEwxEaWdpQ2VydCBJbmMxGTAXBgNVBAsTEHd3dy5kaWdpY2VydC5j\n"
		"b20xIDAeBgNVBAMTF0RpZ2lDZXJ0IEdsb2JhbCBSb290IEcyMIIBIjANBgkqhkiG\n"
		"9w0BAQEFAAOCAQ8AMIIBCgKCAQEAuzfNNNx7a8myaJCtSnX/RrohCgiN9RlUyfuI\n"
		"2/Ou8jqJkTx65qsGGmvPrC3oXgkkRLpimn7Wo6h+4FR1IAWsULecYxpsMNzaHxmx\n"
		"1x7e/dfgy5SDN67sH0NO3Xss0r0upS/kqbitOtSZpLYl6ZtrAGCSYP9PIUkY92eQ\n"
		"q2EGnI/yuum06ZIya7XzV+hdG82MHauVBJVJ8zUtluNJbd134/tJS7SsVQepj5Wz\n"
		"tCO7TG1F8PapspUwtP1MVYwnSlcUfIKdzXOS0xZKBgyMUNGPHgm+F6HmIcr9g+UQ\n"
		"vIOlCsRnKPZzFBQ9RnbDhxSJITRNrw9FDKZJobq7nMWxM4MphQIDAQABo0IwQDAP\n"
		"BgNVHRMBAf8EBTADAQH/MA4GA1UdDwEB/wQEAwIBhjAdBgNVHQ4EFgQUTiJUIBiV\n"
		"5uNu5g/6+rkS7QYXjzkwDQYJKoZIhvcNAQELBQADggEBAGBnKJRvDkhj6zHd6mcY\n"
		"1Yl9PMWLSn/pvtsrF9+wX3N3KjITOYFnQoQj8kVnNeyIv/iPsGEMNKSuIEyExtv4\n"
		"NeF22d+mQrvHRAiGfzZ0JFrabA0UWTW98kndth/Jsw1HKj2ZL7tcu7XUIOGZX1NG\n"
		"Fdtom/DzMNU+MeKNhJ7jitralj41E6Vf8PlwUHBHQRFXGU7Aj64GxJUTFy8bJZ91\n"
		"8rGOmaFvE7FBcf6IKshPECBV1/MUReXgRPTqh5Uykw7+U0b6LJ3/iyK5S9kJRaTe\n"
		"pLiaWN0bfVKfjllDiIGknibVb63dDcY3fe0Dkhvld1927jyNxF1WW6LZZm6zNTfl\n"
		"MrY=\n"
		"-----END CERTIFICATE-----\n";

	// DigiCert Global Root G2 (api.mojang.com)
	// Downloaded from https://www.digicert.com/kb/digicert-root-certificates.htm
	// DigiCert Global Root G2
	static const char MojangComAPIMojang[] =
		"-----BEGIN CERTIFICATE-----\n"
		"MIIDjjCCAnagAwIBAgIQAzrx5qcRqaC7KGSxHQn65TANBgkqhkiG9w0BAQsFADBh\n"
		"MQswCQYDVQQGEwJVUzEVMBMGA1UEChMMRGlnaUNlcnQgSW5jMRkwFwYDVQQLExB3\n"
		"d3cuZGlnaWNlcnQuY29tMSAwHgYDVQQDExdEaWdpQ2VydCBHbG9iYWwgUm9vdCBH\n"
		"MjAeFw0xMzA4MDExMjAwMDBaFw0zODAxMTUxMjAwMDBaMGExCzAJBgNVBAYTAlVT\n"
		"MRUwEwYDVQQKEwxEaWdpQ2VydCBJbmMxGTAXBgNVBAsTEHd3dy5kaWdpY2VydC5j\n"
		"b20xIDAeBgNVBAMTF0RpZ2lDZXJ0IEdsb2JhbCBSb290IEcyMIIBIjANBgkqhkiG\n"
		"9w0BAQEFAAOCAQ8AMIIBCgKCAQEAuzfNNNx7a8myaJCtSnX/RrohCgiN9RlUyfuI\n"
		"2/Ou8jqJkTx65qsGGmvPrC3oXgkkRLpimn7Wo6h+4FR1IAWsULecYxpsMNzaHxmx\n"
		"1x7e/dfgy5SDN67sH0NO3Xss0r0upS/kqbitOtSZpLYl6ZtrAGCSYP9PIUkY92eQ\n"
		"q2EGnI/yuum06ZIya7XzV+hdG82MHauVBJVJ8zUtluNJbd134/tJS7SsVQepj5Wz\n"
		"tCO7TG1F8PapspUwtP1MVYwnSlcUfIKdzXOS0xZKBgyMUNGPHgm+F6HmIcr9g+UQ\n"
		"vIOlCsRnKPZzFBQ9RnbDhxSJITRNrw9FDKZJobq7nMWxM4MphQIDAQABo0IwQDAP\n"
		"BgNVHRMBAf8EBTADAQH/MA4GA1UdDwEB/wQEAwIBhjAdBgNVHQ4EFgQUTiJUIBiV\n"
		"5uNu5g/6+rkS7QYXjzkwDQYJKoZIhvcNAQELBQADggEBAGBnKJRvDkhj6zHd6mcY\n"
		"1Yl9PMWLSn/pvtsrF9+wX3N3KjITOYFnQoQj8kVnNeyIv/iPsGEMNKSuIEyExtv4\n"
		"NeF22d+mQrvHRAiGfzZ0JFrabA0UWTW98kndth/Jsw1HKj2ZL7tcu7XUIOGZX1NG\n"
		"Fdtom/DzMNU+MeKNhJ7jitralj41E6Vf8PlwUHBHQRFXGU7Aj64GxJUTFy8bJZ91\n"
		"8rGOmaFvE7FBcf6IKshPECBV1/MUReXgRPTqh5Uykw7+U0b6LJ3/iyK5S9kJRaTe\n"
		"pLiaWN0bfVKfjllDiIGknibVb63dDcY3fe0Dkhvld1927jyNxF1WW6LZZm6zNTfl\n"
		"MrY=\n"
		"-----END CERTIFICATE-----\n";

	// The root cert used by github.com
	static const char GithubCom[] =
		"-----BEGIN CERTIFICATE-----\n"
		"MIICjzCCAhWgAwIBAgIQXIuZxVqUxdJxVt7NiYDMJjAKBggqhkjOPQQDAzCBiDEL\n"
		"MAkGA1UEBhMCVVMxEzARBgNVBAgTCk5ldyBKZXJzZXkxFDASBgNVBAcTC0plcnNl\n"
		"eSBDaXR5MR4wHAYDVQQKExVUaGUgVVNFUlRSVVNUIE5ldHdvcmsxLjAsBgNVBAMT\n"
		"JVVTRVJUcnVzdCBFQ0MgQ2VydGlmaWNhdGlvbiBBdXRob3JpdHkwHhcNMTAwMjAx\n"
		"MDAwMDAwWhcNMzgwMTE4MjM1OTU5WjCBiDELMAkGA1UEBhMCVVMxEzARBgNVBAgT\n"
		"Ck5ldyBKZXJzZXkxFDASBgNVBAcTC0plcnNleSBDaXR5MR4wHAYDVQQKExVUaGUg\n"
		"VVNFUlRSVVNUIE5ldHdvcmsxLjAsBgNVBAMTJVVTRVJUcnVzdCBFQ0MgQ2VydGlm\n"
		"aWNhdGlvbiBBdXRob3JpdHkwdjAQBgcqhkjOPQIBBgUrgQQAIgNiAAQarFRaqflo\n"
		"I+d61SRvU8Za2EurxtW20eZzca7dnNYMYf3boIkDuAUU7FfO7l0/4iGzzvfUinng\n"
		"o4N+LZfQYcTxmdwlkWOrfzCjtHDix6EznPO/LlxTsV+zfTJ/ijTjeXmjQjBAMB0G\n"
		"A1UdDgQWBBQ64QmG1M8ZwpZ2dEl23OA1xmNjmjAOBgNVHQ8BAf8EBAMCAQYwDwYD\n"
		"VR0TAQH/BAUwAwEB/zAKBggqhkjOPQQDAwNoADBlAjA2Z6EWCNzklwBBHU6+4WMB\n"
		"zzuqQhFkoJ2UOQIReVx7Hfpkue4WQrO/isIJxOzksU0CMQDpKmFHjFJKS04YcPbW\n"
		"RNZu9YO6bVi9JNlWSOrvxKJGgYhqOkbRqZtNyWHa0V1Xahg=\n"
		"-----END CERTIFICATE-----\n";

	// The root cert used by github.com in the past (no longer used)
	static const char GithubComOld[] =
		"-----BEGIN CERTIFICATE-----\n"
		"MIIEFzCCAv+gAwIBAgIQB/LzXIeod6967+lHmTUlvTANBgkqhkiG9w0BAQwFADBh\n"
		"MQswCQYDVQQGEwJVUzEVMBMGA1UEChMMRGlnaUNlcnQgSW5jMRkwFwYDVQQLExB3\n"
		"d3cuZGlnaWNlcnQuY29tMSAwHgYDVQQDExdEaWdpQ2VydCBHbG9iYWwgUm9vdCBD\n"
		"QTAeFw0yMTA0MTQwMDAwMDBaFw0zMTA0MTMyMzU5NTlaMFYxCzAJBgNVBAYTAlVT\n"
		"MRUwEwYDVQQKEwxEaWdpQ2VydCBJbmMxMDAuBgNVBAMTJ0RpZ2lDZXJ0IFRMUyBI\n"
		"eWJyaWQgRUNDIFNIQTM4NCAyMDIwIENBMTB2MBAGByqGSM49AgEGBSuBBAAiA2IA\n"
		"BMEbxppbmNmkKaDp1AS12+umsmxVwP/tmMZJLwYnUcu/cMEFesOxnYeJuq20ExfJ\n"
		"qLSDyLiQ0cx0NTY8g3KwtdD3ImnI8YDEe0CPz2iHJlw5ifFNkU3aiYvkA8ND5b8v\n"
		"c6OCAYIwggF+MBIGA1UdEwEB/wQIMAYBAf8CAQAwHQYDVR0OBBYEFAq8CCkXjKU5\n"
		"bXoOzjPHLrPt+8N6MB8GA1UdIwQYMBaAFAPeUDVW0Uy7ZvCj4hsbw5eyPdFVMA4G\n"
		"A1UdDwEB/wQEAwIBhjAdBgNVHSUEFjAUBggrBgEFBQcDAQYIKwYBBQUHAwIwdgYI\n"
		"KwYBBQUHAQEEajBoMCQGCCsGAQUFBzABhhhodHRwOi8vb2NzcC5kaWdpY2VydC5j\n"
		"b20wQAYIKwYBBQUHMAKGNGh0dHA6Ly9jYWNlcnRzLmRpZ2ljZXJ0LmNvbS9EaWdp\n"
		"Q2VydEdsb2JhbFJvb3RDQS5jcnQwQgYDVR0fBDswOTA3oDWgM4YxaHR0cDovL2Ny\n"
		"bDMuZGlnaWNlcnQuY29tL0RpZ2lDZXJ0R2xvYmFsUm9vdENBLmNybDA9BgNVHSAE\n"
		"NjA0MAsGCWCGSAGG/WwCATAHBgVngQwBATAIBgZngQwBAgEwCAYGZ4EMAQICMAgG\n"
		"BmeBDAECAzANBgkqhkiG9w0BAQwFAAOCAQEAR1mBf9QbH7Bx9phdGLqYR5iwfnYr\n"
		"6v8ai6wms0KNMeZK6BnQ79oU59cUkqGS8qcuLa/7Hfb7U7CKP/zYFgrpsC62pQsY\n"
		"kDUmotr2qLcy/JUjS8ZFucTP5Hzu5sn4kL1y45nDHQsFfGqXbbKrAjbYwrwsAZI/\n"
		"BKOLdRHHuSm8EdCGupK8JvllyDfNJvaGEwwEqonleLHBTnm8dqMLUeTF0J5q/hos\n"
		"Vq4GNiejcxwIfZMy0MJEGdqN9A57HSgDKwmKdsp33Id6rHtSJlWncg+d0ohP/rEh\n"
		"xRqhqjn1VtvChMQ1H3Dau0bwhr9kAMQ+959GG50jBbl9s08PqUU643QwmA==\n"
		"-----END CERTIFICATE-----\n";

	// The root cert used by cuberite.org
	static const char CuberiteOrg[] =
		"-----BEGIN CERTIFICATE-----\n"
		"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\n"
		"-----END CERTIFICATE-----\n";
}





int TestRequest1()
{
	LOG("Running test 1 - fetch http://github.com without redirects");
	auto evtFinished = std::make_shared<cEvent>();
	auto callbacks = std::make_unique<cCallbacks>(evtFinished);
	AStringMap options;
	options["MaxRedirects"] = "0";
	auto res = cUrlClient::Get("http://github.com", std::move(callbacks), AStringMap(), AString(), std::move(options));
	if (res.first)
	{
		if (!evtFinished->Wait(TIMEOUT))
		{
			LOG("Aborting the wait for response; failing the test.");
			return 1;
		}
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
	LOG("Running test 2 - default fetch http://github.com");
	auto evtFinished = std::make_shared<cEvent>();
	auto callbacks = std::make_unique<cCallbacks>(evtFinished);
	auto res = cUrlClient::Get("http://github.com", std::move(callbacks));
	if (res.first)
	{
		if (!evtFinished->Wait(TIMEOUT))
		{
			LOG("Aborting the wait for response; failing the test.");
			return 1;
		}
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
	LOG("Running test 3 - fetch https://github.com without redirects");
	auto evtFinished = std::make_shared<cEvent>();
	auto callbacks = std::make_unique<cCallbacks>(evtFinished);
	AStringMap options;
	options["MaxRedirects"] = "0";
	auto res = cUrlClient::Get("https://github.com", std::move(callbacks), AStringMap(), AString(), std::move(options));
	if (res.first)
	{
		if (!evtFinished->Wait(TIMEOUT))
		{
			LOG("Aborting the wait for response; failing the test.");
			return 1;
		}
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
	LOG("Running test 4 - fetch https://github.com with GitHub trusted root CA");
	auto evtFinished = std::make_shared<cEvent>();
	auto callbacks = std::make_unique<cCallbacks>(evtFinished);
	AStringMap options;
	options["TrustedRootCAs"] = TrustedCAs::GithubCom;
	auto res = cUrlClient::Get("https://github.com", std::move(callbacks), {}, {}, options);
	if (res.first)
	{
		if (!evtFinished->Wait(TIMEOUT))
		{
			LOG("Aborting the wait for response; failing the test.");
			return 1;
		}
	}
	else
	{
		LOG("Immediate error: %s", res.second.c_str());
		return 1;
	}
	return 0;
}





int TestRequest5()
{
	LOG("Running test 5 - fetch https://cuberite.org with Cuberite trusted root CA");
	auto evtFinished = std::make_shared<cEvent>();
	auto callbacks = std::make_unique<cCallbacks>(evtFinished);
	AStringMap options;
	options["TrustedRootCAs"] = TrustedCAs::CuberiteOrg;
	auto res = cUrlClient::Get("https://cuberite.org", std::move(callbacks), {}, {}, options);
	if (res.first)
	{
		if (!evtFinished->Wait(TIMEOUT))
		{
			LOG("Aborting the wait for response; failing the test.");
			return 1;
		}
	}
	else
	{
		LOG("Immediate error: %s", res.second.c_str());
		return 1;
	}
	return 0;
}





int TestRequest6()
{
	LOG("Running test 6 - fetch https://sessionserver.mojang.com with Mojang trusted root CA");
	auto evtFinished = std::make_shared<cEvent>();
	auto callbacks = std::make_unique<cCallbacks>(evtFinished);
	AStringMap options;
	options["TrustedRootCAs"] = TrustedCAs::MojangComSessionServer;
	auto res = cUrlClient::Get("https://sessionserver.mojang.com", std::move(callbacks), {}, {}, options);
	if (res.first)
	{
		if (!evtFinished->Wait(TIMEOUT))
		{
			LOG("Aborting the wait for response; failing the test.");
			return 1;
		}
	}
	else
	{
		LOG("Immediate error: %s", res.second.c_str());
		return 1;
	}
	return 0;
}





int TestRequest7()
{
	LOG("Running test 7 - fetch https://api.mojang.com with Mojang trusted root CA");
	auto evtFinished = std::make_shared<cEvent>();
	auto callbacks = std::make_unique<cCallbacks>(evtFinished);
	AStringMap options;
	options["TrustedRootCAs"] = TrustedCAs::MojangComAPIMojang;
	auto res = cUrlClient::Get("https://api.mojang.com", std::move(callbacks), {}, {}, options);
	if (res.first)
	{
		if (!evtFinished->Wait(TIMEOUT))
		{
			LOG("Aborting the wait for response; failing the test.");
			return 1;
		}
	}
	else
	{
		LOG("Immediate error: %s", res.second.c_str());
		return 1;
	}
	return 0;
}





int TestRequest8()
{
	LOG("Running test 8 - fetch https://api.mojang.com with GitHub trusted root CA (testing CA verification rejection)");
	auto evtFinished = std::make_shared<cEvent>();
	auto callbacks = std::make_unique<cCallbacks>(evtFinished);
	AStringMap options;
	options["TrustedRootCAs"] = TrustedCAs::GithubCom;
	auto [isSuccess, response] = cUrlClient::BlockingGet("https://api.mojang.com", {}, {}, options);
	if (isSuccess)
	{
		LOG("CA verification failure, should have rejected the connection due to bad root CA.");
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
		&TestRequest5,
		&TestRequest6,
		&TestRequest7,
		&TestRequest8,
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
