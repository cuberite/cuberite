
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
	// DigiCert Global Root CA (sessionserver.mojang.com, api.mojang.com)
	// Downloaded from https://www.digicert.com/kb/digicert-root-certificates.htm
	// DigiCert Global Root CA
	static const char MojangCom[] =
		"-----BEGIN CERTIFICATE-----\n"
		"MIIDrzCCApegAwIBAgIQCDvgVpBCRrGhdWrJWZHHSjANBgkqhkiG9w0BAQUFADBh\n"
		"MQswCQYDVQQGEwJVUzEVMBMGA1UEChMMRGlnaUNlcnQgSW5jMRkwFwYDVQQLExB3\n"
		"d3cuZGlnaWNlcnQuY29tMSAwHgYDVQQDExdEaWdpQ2VydCBHbG9iYWwgUm9vdCBD\n"
		"QTAeFw0wNjExMTAwMDAwMDBaFw0zMTExMTAwMDAwMDBaMGExCzAJBgNVBAYTAlVT\n"
		"MRUwEwYDVQQKEwxEaWdpQ2VydCBJbmMxGTAXBgNVBAsTEHd3dy5kaWdpY2VydC5j\n"
		"b20xIDAeBgNVBAMTF0RpZ2lDZXJ0IEdsb2JhbCBSb290IENBMIIBIjANBgkqhkiG\n"
		"9w0BAQEFAAOCAQ8AMIIBCgKCAQEA4jvhEXLeqKTTo1eqUKKPC3eQyaKl7hLOllsB\n"
		"CSDMAZOnTjC3U/dDxGkAV53ijSLdhwZAAIEJzs4bg7/fzTtxRuLWZscFs3YnFo97\n"
		"nh6Vfe63SKMI2tavegw5BmV/Sl0fvBf4q77uKNd0f3p4mVmFaG5cIzJLv07A6Fpt\n"
		"43C/dxC//AH2hdmoRBBYMql1GNXRor5H4idq9Joz+EkIYIvUX7Q6hL+hqkpMfT7P\n"
		"T19sdl6gSzeRntwi5m3OFBqOasv+zbMUZBfHWymeMr/y7vrTC0LUq7dBMtoM1O/4\n"
		"gdW7jVg/tRvoSSiicNoxBN33shbyTApOB6jtSj1etX+jkMOvJwIDAQABo2MwYTAO\n"
		"BgNVHQ8BAf8EBAMCAYYwDwYDVR0TAQH/BAUwAwEB/zAdBgNVHQ4EFgQUA95QNVbR\n"
		"TLtm8KPiGxvDl7I90VUwHwYDVR0jBBgwFoAUA95QNVbRTLtm8KPiGxvDl7I90VUw\n"
		"DQYJKoZIhvcNAQEFBQADggEBAMucN6pIExIK+t1EnE9SsPTfrgT1eXkIoyQY/Esr\n"
		"hMAtudXH/vTBH1jLuG2cenTnmCmrEbXjcKChzUyImZOMkXDiqw8cvpOp/2PV5Adg\n"
		"06O/nVsJ8dWO41P0jmP6P6fbtGbfYmbW0W5BjfIttep3Sp+dWOIrWcBAI+0tKIJF\n"
		"PnlUkiaY4IBIqDfv8NZ5YBberOgOzW6sRBc4L0na4UU+Krk2U886UAb3LujEV0ls\n"
		"YSEY1QSteDwsOoBrp+uvFRTp2InBuThs4pFsiv9kuXclVzDAGySj4dzp30d8tbQk\n"
		"CAUw7C29C79Fv1C5qfPrmAESrciIxpg0X40KPMbp1ZWVbd4=\n"
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
		"MIIFYDCCBEigAwIBAgIQQAF3ITfU6UK47naqPGQKtzANBgkqhkiG9w0BAQsFADA/\n"
		"MSQwIgYDVQQKExtEaWdpdGFsIFNpZ25hdHVyZSBUcnVzdCBDby4xFzAVBgNVBAMT\n"
		"DkRTVCBSb290IENBIFgzMB4XDTIxMDEyMDE5MTQwM1oXDTI0MDkzMDE4MTQwM1ow\n"
		"TzELMAkGA1UEBhMCVVMxKTAnBgNVBAoTIEludGVybmV0IFNlY3VyaXR5IFJlc2Vh\n"
		"cmNoIEdyb3VwMRUwEwYDVQQDEwxJU1JHIFJvb3QgWDEwggIiMA0GCSqGSIb3DQEB\n"
		"AQUAA4ICDwAwggIKAoICAQCt6CRz9BQ385ueK1coHIe+3LffOJCMbjzmV6B493XC\n"
		"ov71am72AE8o295ohmxEk7axY/0UEmu/H9LqMZshftEzPLpI9d1537O4/xLxIZpL\n"
		"wYqGcWlKZmZsj348cL+tKSIG8+TA5oCu4kuPt5l+lAOf00eXfJlII1PoOK5PCm+D\n"
		"LtFJV4yAdLbaL9A4jXsDcCEbdfIwPPqPrt3aY6vrFk/CjhFLfs8L6P+1dy70sntK\n"
		"4EwSJQxwjQMpoOFTJOwT2e4ZvxCzSow/iaNhUd6shweU9GNx7C7ib1uYgeGJXDR5\n"
		"bHbvO5BieebbpJovJsXQEOEO3tkQjhb7t/eo98flAgeYjzYIlefiN5YNNnWe+w5y\n"
		"sR2bvAP5SQXYgd0FtCrWQemsAXaVCg/Y39W9Eh81LygXbNKYwagJZHduRze6zqxZ\n"
		"Xmidf3LWicUGQSk+WT7dJvUkyRGnWqNMQB9GoZm1pzpRboY7nn1ypxIFeFntPlF4\n"
		"FQsDj43QLwWyPntKHEtzBRL8xurgUBN8Q5N0s8p0544fAQjQMNRbcTa0B7rBMDBc\n"
		"SLeCO5imfWCKoqMpgsy6vYMEG6KDA0Gh1gXxG8K28Kh8hjtGqEgqiNx2mna/H2ql\n"
		"PRmP6zjzZN7IKw0KKP/32+IVQtQi0Cdd4Xn+GOdwiK1O5tmLOsbdJ1Fu/7xk9TND\n"
		"TwIDAQABo4IBRjCCAUIwDwYDVR0TAQH/BAUwAwEB/zAOBgNVHQ8BAf8EBAMCAQYw\n"
		"SwYIKwYBBQUHAQEEPzA9MDsGCCsGAQUFBzAChi9odHRwOi8vYXBwcy5pZGVudHJ1\n"
		"c3QuY29tL3Jvb3RzL2RzdHJvb3RjYXgzLnA3YzAfBgNVHSMEGDAWgBTEp7Gkeyxx\n"
		"+tvhS5B1/8QVYIWJEDBUBgNVHSAETTBLMAgGBmeBDAECATA/BgsrBgEEAYLfEwEB\n"
		"ATAwMC4GCCsGAQUFBwIBFiJodHRwOi8vY3BzLnJvb3QteDEubGV0c2VuY3J5cHQu\n"
		"b3JnMDwGA1UdHwQ1MDMwMaAvoC2GK2h0dHA6Ly9jcmwuaWRlbnRydXN0LmNvbS9E\n"
		"U1RST09UQ0FYM0NSTC5jcmwwHQYDVR0OBBYEFHm0WeZ7tuXkAXOACIjIGlj26Ztu\n"
		"MA0GCSqGSIb3DQEBCwUAA4IBAQAKcwBslm7/DlLQrt2M51oGrS+o44+/yQoDFVDC\n"
		"5WxCu2+b9LRPwkSICHXM6webFGJueN7sJ7o5XPWioW5WlHAQU7G75K/QosMrAdSW\n"
		"9MUgNTP52GE24HGNtLi1qoJFlcDyqSMo59ahy2cI2qBDLKobkx/J3vWraV0T9VuG\n"
		"WCLKTVXkcGdtwlfFRjlBz4pYg1htmf5X6DYO8A4jqv2Il9DjXA6USbW1FzXSLr9O\n"
		"he8Y4IWS6wY7bCkjCWDcRQJMEhg76fsO3txE+FiYruq9RUWhiF1myv4Q6W+CyBFC\n"
		"Dfvp7OOGAN6dEOM4+qR9sdjoSYKEBpsr6GtPAQw4dy753ec5\n"
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
	options["TrustedRootCAs"] = TrustedCAs::MojangCom;
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
	options["TrustedRootCAs"] = TrustedCAs::MojangCom;
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
