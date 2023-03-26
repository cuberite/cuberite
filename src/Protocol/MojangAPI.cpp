
// MojangAPI.cpp

// Implements the cMojangAPI class representing the various API points provided by Mojang's webservices, and a cache for their results

#include "Globals.h"
#include "MojangAPI.h"
#include "SQLiteCpp/Database.h"
#include "SQLiteCpp/Statement.h"
#include "../IniFile.h"
#include "../JsonUtils.h"
#include "json/json.h"
#include "../mbedTLS++/BlockingSslClientSocket.h"
#include "../mbedTLS++/SslConfig.h"
#include "../RankManager.h"
#include "../OSSupport/IsThread.h"
#include "../Root.h"





/** The maximum age for items to be kept in the cache. Any item older than this will be removed. */
const Int64 MAX_AGE = 7 * 24 * 60 * 60;  // 7 days ago

/** The maximum number of names to send in a single query */
const int MAX_PER_QUERY = 100;





#define DEFAULT_NAME_TO_UUID_SERVER     "api.mojang.com"
#define DEFAULT_NAME_TO_UUID_ADDRESS    "/profiles/minecraft"
#define DEFAULT_UUID_TO_PROFILE_SERVER  "sessionserver.mojang.com"
#define DEFAULT_UUID_TO_PROFILE_ADDRESS "/session/minecraft/profile/%UUID%?unsigned=false"





/** Returns the CA certificates that should be trusted for Mojang-related connections. */
static cX509CertPtr GetCACerts(void)
{
	static const char CertString[] =
		// DigiCert Global Root CA (sessionserver.mojang.com)
		// Downloaded from https://www.digicert.com/kb/digicert-root-certificates.htm
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
		"-----END CERTIFICATE-----\n"

		// Amazon Root CA 1 (api.mojang.com)
		// Downloaded from https://www.amazontrust.com/repository/
		"-----BEGIN CERTIFICATE-----\n"
		"MIIDQTCCAimgAwIBAgITBmyfz5m/jAo54vB4ikPmljZbyjANBgkqhkiG9w0BAQsF\n"
		"ADA5MQswCQYDVQQGEwJVUzEPMA0GA1UEChMGQW1hem9uMRkwFwYDVQQDExBBbWF6\n"
		"b24gUm9vdCBDQSAxMB4XDTE1MDUyNjAwMDAwMFoXDTM4MDExNzAwMDAwMFowOTEL\n"
		"MAkGA1UEBhMCVVMxDzANBgNVBAoTBkFtYXpvbjEZMBcGA1UEAxMQQW1hem9uIFJv\n"
		"b3QgQ0EgMTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBALJ4gHHKeNXj\n"
		"ca9HgFB0fW7Y14h29Jlo91ghYPl0hAEvrAIthtOgQ3pOsqTQNroBvo3bSMgHFzZM\n"
		"9O6II8c+6zf1tRn4SWiw3te5djgdYZ6k/oI2peVKVuRF4fn9tBb6dNqcmzU5L/qw\n"
		"IFAGbHrQgLKm+a/sRxmPUDgH3KKHOVj4utWp+UhnMJbulHheb4mjUcAwhmahRWa6\n"
		"VOujw5H5SNz/0egwLX0tdHA114gk957EWW67c4cX8jJGKLhD+rcdqsq08p8kDi1L\n"
		"93FcXmn/6pUCyziKrlA4b9v7LWIbxcceVOF34GfID5yHI9Y/QCB/IIDEgEw+OyQm\n"
		"jgSubJrIqg0CAwEAAaNCMEAwDwYDVR0TAQH/BAUwAwEB/zAOBgNVHQ8BAf8EBAMC\n"
		"AYYwHQYDVR0OBBYEFIQYzIU07LwMlJQuCFmcx7IQTgoIMA0GCSqGSIb3DQEBCwUA\n"
		"A4IBAQCY8jdaQZChGsV2USggNiMOruYou6r4lK5IpDB/G/wkjUu0yKGX9rbxenDI\n"
		"U5PMCCjjmCXPI6T53iHTfIUJrU6adTrCC2qJeHZERxhlbI1Bjjt/msv0tadQ1wUs\n"
		"N+gDS63pYaACbvXy8MWy7Vu33PqUXHeeE6V/Uq2V8viTO96LXFvKWlJbYK8U90vv\n"
		"o/ufQJVtMVT8QtPHRh8jrdkPSHCa2XV4cdFyQzR1bldZwgJcJmApzyMZFo6IQ6XU\n"
		"5MsI+yMRQ+hDKXJioaldXgjUkK642M4UwtBV8ob2xJNDd2ZhwLnoQdeXeGADbkpy\n"
		"rqXRfboQnoZsG4q5WTP468SQvvG5\n"
		"-----END CERTIFICATE-----\n"

		// AAA Certificate Services (authserver.ely.by GH#4832)
		// Downloaded from https://www.tbs-certificates.co.uk/FAQ/en/Comodo_AAA_Certificate_Services.html
		"-----BEGIN CERTIFICATE-----\n"
		"MIIEMjCCAxqgAwIBAgIBATANBgkqhkiG9w0BAQUFADB7MQswCQYDVQQGEwJHQjEb\n"
		"MBkGA1UECAwSR3JlYXRlciBNYW5jaGVzdGVyMRAwDgYDVQQHDAdTYWxmb3JkMRow\n"
		"GAYDVQQKDBFDb21vZG8gQ0EgTGltaXRlZDEhMB8GA1UEAwwYQUFBIENlcnRpZmlj\n"
		"YXRlIFNlcnZpY2VzMB4XDTA0MDEwMTAwMDAwMFoXDTI4MTIzMTIzNTk1OVowezEL\n"
		"MAkGA1UEBhMCR0IxGzAZBgNVBAgMEkdyZWF0ZXIgTWFuY2hlc3RlcjEQMA4GA1UE\n"
		"BwwHU2FsZm9yZDEaMBgGA1UECgwRQ29tb2RvIENBIExpbWl0ZWQxITAfBgNVBAMM\n"
		"GEFBQSBDZXJ0aWZpY2F0ZSBTZXJ2aWNlczCCASIwDQYJKoZIhvcNAQEBBQADggEP\n"
		"ADCCAQoCggEBAL5AnfRu4ep2hxxNRUSOvkbIgwadwSr+GB+O5AL686tdUIoWMQua\n"
		"BtDFcCLNSS1UY8y2bmhGC1Pqy0wkwLxyTurxFa70VJoSCsN6sjNg4tqJVfMiWPPe\n"
		"3M/vg4aijJRPn2jymJBGhCfHdr/jzDUsi14HZGWCwEiwqJH5YZ92IFCokcdmtet4\n"
		"YgNW8IoaE+oxox6gmf049vYnMlhvB/VruPsUK6+3qszWY19zjNoFmag4qMsXeDZR\n"
		"rOme9Hg6jc8P2ULimAyrL58OAd7vn5lJ8S3frHRNG5i1R8XlKdH5kBjHYpy+g8cm\n"
		"ez6KJcfA3Z3mNWgQIJ2P2N7Sw4ScDV7oL8kCAwEAAaOBwDCBvTAdBgNVHQ4EFgQU\n"
		"oBEKIz6W8Qfs4q8p74Klf9AwpLQwDgYDVR0PAQH/BAQDAgEGMA8GA1UdEwEB/wQF\n"
		"MAMBAf8wewYDVR0fBHQwcjA4oDagNIYyaHR0cDovL2NybC5jb21vZG9jYS5jb20v\n"
		"QUFBQ2VydGlmaWNhdGVTZXJ2aWNlcy5jcmwwNqA0oDKGMGh0dHA6Ly9jcmwuY29t\n"
		"b2RvLm5ldC9BQUFDZXJ0aWZpY2F0ZVNlcnZpY2VzLmNybDANBgkqhkiG9w0BAQUF\n"
		"AAOCAQEACFb8AvCb6P+k+tZ7xkSAzk/ExfYAWMymtrwUSWgEdujm7l3sAg9g1o1Q\n"
		"GE8mTgHj5rCl7r+8dFRBv/38ErjHT1r0iWAFf2C3BUrz9vHCv8S5dIa2LX1rzNLz\n"
		"Rt0vxuBqw8M0Ayx9lt1awg6nCpnBBYurDC/zXDrPbDdVCYfeU0BsWO/8tqtlbgT2\n"
		"G9w84FoVxp7Z8VlIMCFlA2zs6SFz7JsDoeA3raAVGI/6ugLOpyypEBMs1OUIJqsi\n"
		"l2D4kF501KKaU73yqWjgom7C12yxow+ev+to51byrvLjKzg6CYG1a4XXvi3tPxq3\n"
		"smPi9WIsgtRqAEFQ8TmDn5XpNpaYbg==\n"
		"-----END CERTIFICATE-----\n"
	;

	static auto X509Cert = [&]()
	{
		auto Cert = std::make_shared<cX509Cert>();
		VERIFY(0 == Cert->Parse(CertString, sizeof(CertString)));
		return Cert;
	}();

	return X509Cert;
}





/** Returns the config to be used for secure requests. */
static std::shared_ptr<const cSslConfig> GetSslConfig()
{
	static const std::shared_ptr<const cSslConfig> Config = []()
	{
		auto Conf = cSslConfig::MakeDefaultConfig(true);
		Conf->SetCACerts(GetCACerts());
		Conf->SetAuthMode(eSslAuthMode::Required);
		return Conf;
	}();
	return Config;
}





////////////////////////////////////////////////////////////////////////////////
// cMojangAPI::sProfile:

cMojangAPI::sProfile::sProfile(
	const AString & a_PlayerName,
	const cUUID & a_UUID,
	const Json::Value & a_Properties,
	Int64 a_DateTime
) :
	m_PlayerName(a_PlayerName),
	m_UUID(a_UUID),
	m_Textures(),
	m_TexturesSignature(),
	m_DateTime(a_DateTime)
{
	/*
	Example a_Profile contents:
	"properties":
	[
		{
			"name": "textures",
			"value": "eyJ0aW1lc3RhbXAiOjE0MDcwNzAzMjEyNzEsInByb2ZpbGVJZCI6ImIxY2FmMjQyMDJhODQxYTc4MDU1YTA3OWM0NjBlZWU3IiwicHJvZmlsZU5hbWUiOiJ4b2Z0IiwiaXNQdWJsaWMiOnRydWUsInRleHR1cmVzIjp7IlNLSU4iOnsidXJsIjoiaHR0cDovL3RleHR1cmVzLm1pbmVjcmFmdC5uZXQvdGV4dHVyZS9iNzc5YmFiZjVhNTg3Zjk0OGFkNjc0N2VhOTEyNzU0MjliNjg4Mjk1YWUzYzA3YmQwZTJmNWJmNGQwNTIifX19",
			"signature": "XCty+jGEF39hEPrPhYNnCX087kPaoCjYruzYI/DS4nkL5hbjnkSM5Rh15hnUyv/FHhC8OF5rif3D1tQjtMI19KSVaXoUFXpbJM8/+PB8GDgEbX8Fc3u9nYkzOcM/xfxdYsFAdFhLQMkvase/BZLSuPhdy9DdI+TCrO7xuSTZfYmmwVuWo3w5gCY+mSIAnqltnOzaOOTcly75xvO0WYpVk7nJdnR2tvSi0wfrQPDrIg/uzhX7p0SnDqijmBU4QaNez/TNKiFxy69dAzt0RSotlQzqkDbyVKhhv9a4eY8h3pXi4UMftKEj4FAKczxLImkukJXuOn5NN15/Q+le0rJVBC60/xjKIVzltEsMN6qjWD0lQjey7WEL+4pGhCVuWY5KzuZjFvgqszuJTFz7lo+bcHiceldJtea8/fa02eTRObZvdLxbWC9ZfFY0IhpOVKfcLdno/ddDMNMQMi5kMrJ8MZZ/PcW1w5n7MMGWPGCla1kOaC55AL0QYSMGRVEZqgU9wXI5M7sHGZKGM4mWxkbEJYBkpI/p3GyxWgV6v33ZWlsz65TqlNrR1gCLaoFCm7Sif8NqPBZUAONHYon0roXhin/DyEanS93WV6i6FC1Wisscjq2AcvnOlgTo/5nN/1QsMbjNumuMGo37sqjRqlXoPb8zEUbAhhztYuJjEfQ2Rd8="
		}
	]
	*/

	// Parse the Textures and TexturesSignature from the Profile:
	if (!a_Properties.isArray())
	{
		// Properties is not a valid array, bail out
		return;
	}
	Json::UInt Size = a_Properties.size();
	for (Json::UInt i = 0; i < Size; i++)
	{
		const Json::Value & Prop = a_Properties[i];
		AString PropName = Prop.get("name", "").asString();
		if (PropName != "textures")
		{
			continue;
		}
		m_Textures = Prop.get("value", "").asString();
		m_TexturesSignature = Prop.get("signature", "").asString();
		break;
	}  // for i - Properties[]
}





////////////////////////////////////////////////////////////////////////////////
// cMojangAPI::cUpdateThread:

class cMojangAPI::cUpdateThread:
	public cIsThread
{
	using Super = cIsThread;

public:

	cUpdateThread(cMojangAPI & a_MojangAPI):
		Super("MojangAPI Updater"),
		m_MojangAPI(a_MojangAPI)
	{
	}

	virtual ~cUpdateThread() override
	{
		// Notify the thread that it should stop:
		m_ShouldTerminate = true;
		m_evtNotify.Set();

		// Wait for the thread to actually finish work:
		Stop();
	}

protected:

	/** The cMojangAPI instance to update. */
	cMojangAPI & m_MojangAPI;

	/** The event used for notifying that the thread should terminate, as well as timing. */
	cEvent m_evtNotify;


	// cIsThread override:
	virtual void Execute(void) override
	{
		do
		{
			m_MojangAPI.Update();
		} while (!m_ShouldTerminate && !m_evtNotify.Wait(60 * 60 * 1000));  // Repeat every 60 minutes until termination request
	}
} ;





////////////////////////////////////////////////////////////////////////////////
// cMojangAPI:

cMojangAPI::cMojangAPI(void) :
	m_NameToUUIDServer(DEFAULT_NAME_TO_UUID_SERVER),
	m_NameToUUIDAddress(DEFAULT_NAME_TO_UUID_ADDRESS),
	m_UUIDToProfileServer(DEFAULT_UUID_TO_PROFILE_SERVER),
	m_UUIDToProfileAddress(DEFAULT_UUID_TO_PROFILE_ADDRESS),
	m_RankMgr(nullptr),
	m_UpdateThread(new cUpdateThread(*this))
{
}





cMojangAPI::~cMojangAPI()
{
	SaveCachesToDisk();
}





void cMojangAPI::Start(cSettingsRepositoryInterface & a_Settings, bool a_ShouldAuth)
{
	m_NameToUUIDServer     = a_Settings.GetValueSet("MojangAPI", "NameToUUIDServer",     DEFAULT_NAME_TO_UUID_SERVER);
	m_NameToUUIDAddress    = a_Settings.GetValueSet("MojangAPI", "NameToUUIDAddress",    DEFAULT_NAME_TO_UUID_ADDRESS);
	m_UUIDToProfileServer  = a_Settings.GetValueSet("MojangAPI", "UUIDToProfileServer",  DEFAULT_UUID_TO_PROFILE_SERVER);
	m_UUIDToProfileAddress = a_Settings.GetValueSet("MojangAPI", "UUIDToProfileAddress", DEFAULT_UUID_TO_PROFILE_ADDRESS);
	LoadCachesFromDisk();
	if (a_ShouldAuth)
	{
		m_UpdateThread->Start();
	}
}





cUUID cMojangAPI::GetUUIDFromPlayerName(const AString & a_PlayerName, bool a_UseOnlyCached)
{
	// Convert the playername to lowercase:
	AString lcPlayerName = StrToLower(a_PlayerName);

	// Request the cache to query the name if not yet cached:
	if (!a_UseOnlyCached)
	{
		AStringVector PlayerNames{ lcPlayerName };
		CacheNamesToUUIDs(PlayerNames);
	}

	// Retrieve from cache:
	cCSLock Lock(m_CSNameToUUID);
	cProfileMap::const_iterator itr = m_NameToUUID.find(lcPlayerName);
	if (itr == m_NameToUUID.end())
	{
		// No UUID found
		return {};
	}
	return itr->second.m_UUID;
}





AString cMojangAPI::GetPlayerNameFromUUID(const cUUID & a_UUID, bool a_UseOnlyCached)
{
	// Retrieve from caches:
	{
		cCSLock Lock(m_CSUUIDToProfile);
		auto itr = m_UUIDToProfile.find(a_UUID);
		if (itr != m_UUIDToProfile.end())
		{
			return itr->second.m_PlayerName;
		}
	}
	{
		cCSLock Lock(m_CSUUIDToName);
		auto itr = m_UUIDToName.find(a_UUID);
		if (itr != m_UUIDToName.end())
		{
			return itr->second.m_PlayerName;
		}
	}

	// Name not yet cached, request cache and retry:
	if (!a_UseOnlyCached)
	{
		CacheUUIDToProfile(a_UUID);
		return GetPlayerNameFromUUID(a_UUID, true);
	}

	// No value found, none queried. Return an error:
	return {};
}





std::vector<cUUID> cMojangAPI::GetUUIDsFromPlayerNames(const AStringVector & a_PlayerNames, bool a_UseOnlyCached)
{
	// Convert all playernames to lowercase:
	AStringVector PlayerNames;
	for (AStringVector::const_iterator itr = a_PlayerNames.begin(), end = a_PlayerNames.end(); itr != end; ++itr)
	{
		PlayerNames.push_back(StrToLower(*itr));
	}  // for itr - a_PlayerNames[]

	// Request the cache to populate any names not yet contained:
	if (!a_UseOnlyCached)
	{
		CacheNamesToUUIDs(PlayerNames);
	}

	// Retrieve from cache:
	size_t idx = 0;
	std::vector<cUUID> res;
	res.resize(PlayerNames.size());
	cCSLock Lock(m_CSNameToUUID);
	for (AStringVector::const_iterator itr = PlayerNames.begin(), end = PlayerNames.end(); itr != end; ++itr, ++idx)
	{
		cProfileMap::const_iterator itrN = m_NameToUUID.find(*itr);
		if (itrN != m_NameToUUID.end())
		{
			res[idx] = itrN->second.m_UUID;
		}
	}  // for itr - PlayerNames[]
	return res;
}





void cMojangAPI::AddPlayerNameToUUIDMapping(const AString & a_PlayerName, const cUUID & a_UUID)
{
	Int64 Now = time(nullptr);
	{
		cCSLock Lock(m_CSNameToUUID);
		m_NameToUUID[StrToLower(a_PlayerName)] = sProfile(a_PlayerName, a_UUID, "", "", Now);
	}
	{
		cCSLock Lock(m_CSUUIDToName);
		m_UUIDToName[a_UUID] = sProfile(a_PlayerName, a_UUID, "", "", Now);
	}
	NotifyNameUUID(a_PlayerName, a_UUID);
}





void cMojangAPI::AddPlayerProfile(const AString & a_PlayerName, const cUUID & a_UUID, const Json::Value & a_Properties)
{
	Int64 Now = time(nullptr);
	{
		cCSLock Lock(m_CSNameToUUID);
		m_NameToUUID[StrToLower(a_PlayerName)] = sProfile(a_PlayerName, a_UUID, "", "", Now);
	}
	{
		cCSLock Lock(m_CSUUIDToName);
		m_UUIDToName[a_UUID] = sProfile(a_PlayerName, a_UUID, "", "", Now);
	}
	{
		cCSLock Lock(m_CSUUIDToProfile);
		m_UUIDToProfile[a_UUID] = sProfile(a_PlayerName, a_UUID, a_Properties, Now);
	}
	NotifyNameUUID(a_PlayerName, a_UUID);
}





bool cMojangAPI::SecureRequest(const AString & a_ServerName, const AString & a_Request, AString & a_Response)
{
	// Connect the socket:
	cBlockingSslClientSocket Socket;
	Socket.SetSslConfig(GetSslConfig());
	Socket.SetExpectedPeerName(a_ServerName);
	if (!Socket.Connect(a_ServerName, 443))
	{
		LOGWARNING("%s: Can't connect to %s: %s", __METHOD_NAME__, a_ServerName.c_str(), Socket.GetLastErrorText().c_str());
		return false;
	}

	if (!Socket.Send(a_Request.c_str(), a_Request.size()))
	{
		LOGWARNING("%s: Writing SSL data failed: %s", __METHOD_NAME__, Socket.GetLastErrorText().c_str());
		return false;
	}

	// Read the HTTP response:
	unsigned char buf[1024];

	for (;;)
	{
		int ret = Socket.Receive(buf, sizeof(buf));

		if ((ret == MBEDTLS_ERR_SSL_WANT_READ) || (ret == MBEDTLS_ERR_SSL_WANT_WRITE))
		{
			// This value should never be returned, it is handled internally by cBlockingSslClientSocket
			LOGWARNING("%s: SSL reading failed internally", __METHOD_NAME__);
			return false;
		}
		if (ret == MBEDTLS_ERR_SSL_PEER_CLOSE_NOTIFY)
		{
			break;
		}
		if (ret < 0)
		{
			LOGWARNING("%s: SSL reading failed: -0x%x", __METHOD_NAME__, -ret);
			return false;
		}
		if (ret == 0)
		{
			break;
		}

		a_Response.append(reinterpret_cast<const char *>(buf), static_cast<size_t>(ret));
	}

	return true;
}





void cMojangAPI::LoadCachesFromDisk(void)
{
	try
	{
		// Open up the SQLite DB:
		SQLite::Database db("MojangAPI.sqlite", SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
		db.exec("CREATE TABLE IF NOT EXISTS PlayerNameToUUID (PlayerName, UUID, DateTime)");
		db.exec("CREATE TABLE IF NOT EXISTS UUIDToProfile    (UUID, PlayerName, Textures, TexturesSignature, DateTime)");

		// Retrieve all entries:
		{
			SQLite::Statement stmt(db, "SELECT PlayerName, UUID, DateTime FROM PlayerNameToUUID");
			while (stmt.executeStep())
			{
				AString PlayerName = stmt.getColumn(0);
				AString StringUUID = stmt.getColumn(1);
				Int64 DateTime     = stmt.getColumn(2);

				cUUID UUID;
				if (!UUID.FromString(StringUUID))
				{
					continue;  // Invalid UUID
				}

				m_NameToUUID[StrToLower(PlayerName)] = sProfile(PlayerName, UUID, "", "", DateTime);
				m_UUIDToName[UUID] = sProfile(PlayerName, UUID, "", "", DateTime);
			}
		}
		{
			SQLite::Statement stmt(db, "SELECT PlayerName, UUID, Textures, TexturesSignature, DateTime FROM UUIDToProfile");
			while (stmt.executeStep())
			{
				AString PlayerName        = stmt.getColumn(0);
				AString StringUUID        = stmt.getColumn(1);
				AString Textures          = stmt.getColumn(2);
				AString TexturesSignature = stmt.getColumn(2);
				Int64 DateTime            = stmt.getColumn(4);

				cUUID UUID;
				if (!UUID.FromString(StringUUID))
				{
					continue;  // Invalid UUID
				}

				m_UUIDToProfile[UUID] = sProfile(PlayerName, UUID, Textures, TexturesSignature, DateTime);
			}
		}
	}
	catch (const SQLite::Exception & ex)
	{
		LOGINFO("Loading MojangAPI cache failed: %s", ex.what());
	}
}





void cMojangAPI::SaveCachesToDisk(void)
{
	try
	{
		// Open up the SQLite DB:
		SQLite::Database db("MojangAPI.sqlite", SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
		db.exec("CREATE TABLE IF NOT EXISTS PlayerNameToUUID (PlayerName, UUID, DateTime)");
		db.exec("CREATE TABLE IF NOT EXISTS UUIDToProfile (UUID, PlayerName, Textures, TexturesSignature, DateTime)");

		// Remove all entries:
		db.exec("DELETE FROM PlayerNameToUUID");
		db.exec("DELETE FROM UUIDToProfile");

		// Save all cache entries - m_PlayerNameToUUID:
		Int64 LimitDateTime = time(nullptr) - MAX_AGE;
		{
			SQLite::Statement stmt(db, "INSERT INTO PlayerNameToUUID(PlayerName, UUID, DateTime) VALUES (?, ?, ?)");
			cCSLock Lock(m_CSNameToUUID);
			for (auto & NameToUUID : m_NameToUUID)
			{
				auto & Profile = NameToUUID.second;
				if (Profile.m_DateTime < LimitDateTime)
				{
					// This item is too old, do not save
					continue;
				}
				stmt.bind(1, Profile.m_PlayerName);
				stmt.bind(2, Profile.m_UUID.ToShortString());
				stmt.bind(3, Profile.m_DateTime);
				stmt.exec();
				stmt.reset();
			}
		}

		// Save all cache entries - m_UUIDToProfile:
		{
			SQLite::Statement stmt(db, "INSERT INTO UUIDToProfile(UUID, PlayerName, Textures, TexturesSignature, DateTime) VALUES (?, ?, ?, ?, ?)");
			cCSLock Lock(m_CSUUIDToProfile);
			for (auto & UUIDToProfile : m_UUIDToProfile)
			{
				auto & Profile = UUIDToProfile.second;
				if (Profile.m_DateTime < LimitDateTime)
				{
					// This item is too old, do not save
					continue;
				}
				stmt.bind(1, Profile.m_UUID.ToShortString());
				stmt.bind(2, Profile.m_PlayerName);
				stmt.bind(3, Profile.m_Textures);
				stmt.bind(4, Profile.m_TexturesSignature);
				stmt.bind(5, Profile.m_DateTime);
				stmt.exec();
				stmt.reset();
			}
		}
	}
	catch (const SQLite::Exception & ex)
	{
		LOGINFO("Saving MojangAPI cache failed: %s", ex.what());
	}
}





void cMojangAPI::CacheNamesToUUIDs(const AStringVector & a_PlayerNames)
{
	// Create a list of names to query, by removing those that are already cached:
	AStringVector NamesToQuery;
	NamesToQuery.reserve(a_PlayerNames.size());
	{
		cCSLock Lock(m_CSNameToUUID);
		for (AStringVector::const_iterator itr = a_PlayerNames.begin(), end = a_PlayerNames.end(); itr != end; ++itr)
		{
			if (m_NameToUUID.find(*itr) == m_NameToUUID.end())
			{
				NamesToQuery.push_back(*itr);
			}
		}  // for itr - a_PlayerNames[]
	}  // Lock(m_CSNameToUUID)

	QueryNamesToUUIDs(NamesToQuery);
}





void cMojangAPI::QueryNamesToUUIDs(AStringVector & a_NamesToQuery)
{
	while (!a_NamesToQuery.empty())
	{
		// Create the request body - a JSON containing up to MAX_PER_QUERY playernames:
		Json::Value root;
		int Count = 0;
		AStringVector::iterator itr = a_NamesToQuery.begin(), end = a_NamesToQuery.end();
		for (; (itr != end) && (Count < MAX_PER_QUERY); ++itr, ++Count)
		{
			Json::Value req(*itr);
			root.append(req);
		}  // for itr - a_PlayerNames[]
		a_NamesToQuery.erase(a_NamesToQuery.begin(), itr);
		auto RequestBody = JsonUtils::WriteFastString(root);

		// Create the HTTP request:
		AString Request;
		Request += "POST " + m_NameToUUIDAddress + " HTTP/1.0\r\n";  // We need to use HTTP 1.0 because we don't handle Chunked transfer encoding
		Request += "Host: " + m_NameToUUIDServer + "\r\n";
		Request += "User-Agent: Cuberite\r\n";
		Request += "Connection: close\r\n";
		Request += "Content-Type: application/json\r\n";
		Request += Printf("Content-Length: %u\r\n", static_cast<unsigned>(RequestBody.length()));
		Request += "\r\n";
		Request += RequestBody;

		// Get the response from the server:
		AString Response;
		if (!SecureRequest(m_NameToUUIDServer, Request, Response))
		{
			continue;
		}

		// Check the HTTP status line:
		const AString Prefix("HTTP/1.1 200 OK");
		AString HexDump;
		if (Response.compare(0, Prefix.size(), Prefix))
		{
			LOGINFO("%s failed: bad HTTP status line received", __METHOD_NAME__);
			LOGD("Response: \n%s", CreateHexDump(HexDump, Response.data(), Response.size(), 16).c_str());
			continue;
		}

		// Erase the HTTP headers from the response:
		size_t idxHeadersEnd = Response.find("\r\n\r\n");
		if (idxHeadersEnd == AString::npos)
		{
			LOGINFO("%s failed: bad HTTP response header received", __METHOD_NAME__);
			LOGD("Response: \n%s", CreateHexDump(HexDump, Response.data(), Response.size(), 16).c_str());
			continue;
		}
		Response.erase(0, idxHeadersEnd + 4);

		// Parse the returned string into Json:
		AString ParseError;
		if (!JsonUtils::ParseString(Response, root, &ParseError) || !root.isArray())
		{
			LOGWARNING("%s failed: Cannot parse received data (NameToUUID) to JSON: \"%s\"", __METHOD_NAME__, ParseError);
			LOGD("Response body:\n%s", CreateHexDump(HexDump, Response.data(), Response.size(), 16));
			continue;
		}

		// Store the returned results into cache:
		Json::Value::UInt JsonCount = root.size();
		Int64 Now = time(nullptr);
		{
			cCSLock Lock(m_CSNameToUUID);
			for (Json::Value::UInt idx = 0; idx < JsonCount; ++idx)
			{
				Json::Value & Val = root[idx];
				AString JsonName = Val.get("name", "").asString();
				cUUID JsonUUID;
				if (!JsonUUID.FromString(Val.get("id", "").asString()))
				{
					continue;
				}
				m_NameToUUID[StrToLower(JsonName)] = sProfile(JsonName, JsonUUID, "", "", Now);
				NotifyNameUUID(JsonName, JsonUUID);
			}  // for idx - root[]
		}  // cCSLock (m_CSNameToUUID)

		// Also cache the UUIDToName:
		{
			cCSLock Lock(m_CSUUIDToName);
			for (Json::Value::UInt idx = 0; idx < JsonCount; ++idx)
			{
				Json::Value & Val = root[idx];
				AString JsonName = Val.get("name", "").asString();
				cUUID JsonUUID;
				if (!JsonUUID.FromString(Val.get("id", "").asString()))
				{
					continue;
				}
				m_UUIDToName[JsonUUID] = sProfile(JsonName, JsonUUID, "", "", Now);
			}  // for idx - root[]
		}
	}  // while (!NamesToQuery.empty())
}





void cMojangAPI::CacheUUIDToProfile(const cUUID & a_UUID)
{
	// Check if already present:
	{
		cCSLock Lock(m_CSUUIDToProfile);
		if (m_UUIDToProfile.find(a_UUID) != m_UUIDToProfile.end())
		{
			return;
		}
	}

	QueryUUIDToProfile(a_UUID);
}





void cMojangAPI::QueryUUIDToProfile(const cUUID & a_UUID)
{
	// Create the request address:
	AString Address = m_UUIDToProfileAddress;
	ReplaceURL(Address, "%UUID%", a_UUID.ToShortString());

	// Create the HTTP request:
	AString Request;
	Request += "GET " + Address + " HTTP/1.0\r\n";  // We need to use HTTP 1.0 because we don't handle Chunked transfer encoding
	Request += "Host: " + m_UUIDToProfileServer + "\r\n";
	Request += "User-Agent: Cuberite\r\n";
	Request += "Connection: close\r\n";
	Request += "Content-Length: 0\r\n";
	Request += "\r\n";

	// Get the response from the server:
	AString Response;
	if (!SecureRequest(m_UUIDToProfileServer, Request, Response))
	{
		return;
	}

	// Check the HTTP status line:
	const AString Prefix("HTTP/1.1 200 OK");
	AString HexDump;
	if (Response.compare(0, Prefix.size(), Prefix))
	{
		LOGINFO("%s failed: bad HTTP status line received", __METHOD_NAME__);
		LOGD("Response: \n%s", CreateHexDump(HexDump, Response.data(), Response.size(), 16).c_str());
		return;
	}

	// Erase the HTTP headers from the response:
	size_t idxHeadersEnd = Response.find("\r\n\r\n");
	if (idxHeadersEnd == AString::npos)
	{
		LOGINFO("%s failed: bad HTTP response header received", __METHOD_NAME__);
		LOGD("Response: \n%s", CreateHexDump(HexDump, Response.data(), Response.size(), 16).c_str());
		return;
	}
	Response.erase(0, idxHeadersEnd + 4);

	// Parse the returned string into Json:
	Json::Value root;
	AString ParseError;
	if (!JsonUtils::ParseString(Response, root, &ParseError) || !root.isObject())
	{
		LOGWARNING("%s failed: Cannot parse received data (NameToUUID) to JSON: \"%s\"", __METHOD_NAME__, ParseError);
		LOGD("Response body:\n%s", CreateHexDump(HexDump, Response.data(), Response.size(), 16).c_str());
		return;
	}

	/* Example response:
	{
		"id": "b1caf24202a841a78055a079c460eee7",
		"name": "xoft",
		"properties":
		[
			{
				"name": "textures",
				"value": "eyJ0aW1lc3RhbXAiOjE0MDcwNzAzMjEyNzEsInByb2ZpbGVJZCI6ImIxY2FmMjQyMDJhODQxYTc4MDU1YTA3OWM0NjBlZWU3IiwicHJvZmlsZU5hbWUiOiJ4b2Z0IiwiaXNQdWJsaWMiOnRydWUsInRleHR1cmVzIjp7IlNLSU4iOnsidXJsIjoiaHR0cDovL3RleHR1cmVzLm1pbmVjcmFmdC5uZXQvdGV4dHVyZS9iNzc5YmFiZjVhNTg3Zjk0OGFkNjc0N2VhOTEyNzU0MjliNjg4Mjk1YWUzYzA3YmQwZTJmNWJmNGQwNTIifX19",
				"signature": "XCty+jGEF39hEPrPhYNnCX087kPaoCjYruzYI/DS4nkL5hbjnkSM5Rh15hnUyv/FHhC8OF5rif3D1tQjtMI19KSVaXoUFXpbJM8/+PB8GDgEbX8Fc3u9nYkzOcM/xfxdYsFAdFhLQMkvase/BZLSuPhdy9DdI+TCrO7xuSTZfYmmwVuWo3w5gCY+mSIAnqltnOzaOOTcly75xvO0WYpVk7nJdnR2tvSi0wfrQPDrIg/uzhX7p0SnDqijmBU4QaNez/TNKiFxy69dAzt0RSotlQzqkDbyVKhhv9a4eY8h3pXi4UMftKEj4FAKczxLImkukJXuOn5NN15/Q+le0rJVBC60/xjKIVzltEsMN6qjWD0lQjey7WEL+4pGhCVuWY5KzuZjFvgqszuJTFz7lo+bcHiceldJtea8/fa02eTRObZvdLxbWC9ZfFY0IhpOVKfcLdno/ddDMNMQMi5kMrJ8MZZ/PcW1w5n7MMGWPGCla1kOaC55AL0QYSMGRVEZqgU9wXI5M7sHGZKGM4mWxkbEJYBkpI/p3GyxWgV6v33ZWlsz65TqlNrR1gCLaoFCm7Sif8NqPBZUAONHYon0roXhin/DyEanS93WV6i6FC1Wisscjq2AcvnOlgTo/5nN/1QsMbjNumuMGo37sqjRqlXoPb8zEUbAhhztYuJjEfQ2Rd8="
			}
		]
	}
	*/

	// Store the returned result into caches:
	AString PlayerName = root.get("name", "").asString();
	if (PlayerName.empty())
	{
		// No valid playername, bail out
		return;
	}
	Json::Value Properties = root.get("properties", "");
	Int64 Now = time(nullptr);
	{
		cCSLock Lock(m_CSUUIDToProfile);
		m_UUIDToProfile[a_UUID] = sProfile(PlayerName, a_UUID, Properties, Now);
	}
	{
		cCSLock Lock(m_CSUUIDToName);
		m_UUIDToName[a_UUID] = sProfile(PlayerName, a_UUID, Properties, Now);
	}
	{
		cCSLock Lock(m_CSNameToUUID);
		m_NameToUUID[StrToLower(PlayerName)] = sProfile(PlayerName, a_UUID, Properties, Now);
	}
	NotifyNameUUID(PlayerName, a_UUID);
}





void cMojangAPI::NotifyNameUUID(const AString & a_PlayerName, const cUUID & a_UUID)
{
	// Notify the rank manager:
	cCSLock Lock(m_CSRankMgr);
	if (m_RankMgr != nullptr)
	{
		m_RankMgr->NotifyNameUUID(a_PlayerName, a_UUID);
	}
}





void cMojangAPI::Update(void)
{
	Int64 LimitDateTime = time(nullptr) - MAX_AGE;

	// Re-query all playernames that are stale:
	AStringVector PlayerNames;
	{
		cCSLock Lock(m_CSNameToUUID);
		for (const auto & NameToUUID : m_NameToUUID)
		{
			if (NameToUUID.second.m_DateTime < LimitDateTime)
			{
				PlayerNames.push_back(NameToUUID.first);
			}
		}  // for itr - m_NameToUUID[]
	}
	if (!PlayerNames.empty())
	{
		LOG("%s: Updating name-to-uuid cache for %u names", __METHOD_NAME__, static_cast<unsigned>(PlayerNames.size()));
		QueryNamesToUUIDs(PlayerNames);
	}

	// Re-query all profiles that are stale:
	std::vector<cUUID> ProfileUUIDs;
	{
		cCSLock Lock(m_CSUUIDToProfile);
		for (auto & UUIDToProfile : m_UUIDToProfile)
		{
			if (UUIDToProfile.second.m_DateTime < LimitDateTime)
			{
				ProfileUUIDs.push_back(UUIDToProfile.first);
			}
		}  // for itr - m_UUIDToProfile[]
	}
	if (!ProfileUUIDs.empty())
	{
		LOG("%s: Updating uuid-to-profile cache for %u uuids", __METHOD_NAME__, static_cast<unsigned>(ProfileUUIDs.size()));
		for (const auto & UUID : ProfileUUIDs)
		{
			QueryUUIDToProfile(UUID);
		}
	}
}
