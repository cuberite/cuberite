
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
		// GeoTrust root CA cert
		// Currently used for signing *.mojang.com's cert
		// Exported from Mozilla Firefox's built-in CA repository
		"-----BEGIN CERTIFICATE-----\n"
		"MIIDVDCCAjygAwIBAgIDAjRWMA0GCSqGSIb3DQEBBQUAMEIxCzAJBgNVBAYTAlVT\n"
		"MRYwFAYDVQQKEw1HZW9UcnVzdCBJbmMuMRswGQYDVQQDExJHZW9UcnVzdCBHbG9i\n"
		"YWwgQ0EwHhcNMDIwNTIxMDQwMDAwWhcNMjIwNTIxMDQwMDAwWjBCMQswCQYDVQQG\n"
		"EwJVUzEWMBQGA1UEChMNR2VvVHJ1c3QgSW5jLjEbMBkGA1UEAxMSR2VvVHJ1c3Qg\n"
		"R2xvYmFsIENBMIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEA2swYYzD9\n"
		"9BcjGlZ+W988bDjkcbd4kdS8odhM+KhDtgPpTSEHCIjaWC9mOSm9BXiLnTjoBbdq\n"
		"fnGk5sRgprDvgOSJKA+eJdbtg/OtppHHmMlCGDUUna2YRpIuT8rxh0PBFpVXLVDv\n"
		"iS2Aelet8u5fa9IAjbkU+BQVNdnARqN7csiRv8lVK83Qlz6cJmTM386DGXHKTubU\n"
		"1XupGc1V3sjs0l44U+VcT4wt/lAjNvxm5suOpDkZALeVAjmRCw7+OC7RHQWa9k0+\n"
		"bw8HHa8sHo9gOeL6NlMTOdReJivbPagUvTLrGAMoUgRx5aszPeE4uwc2hGKceeoW\n"
		"MPRfwCvocWvk+QIDAQABo1MwUTAPBgNVHRMBAf8EBTADAQH/MB0GA1UdDgQWBBTA\n"
		"ephojYn7qwVkDBF9qn1luMrMTjAfBgNVHSMEGDAWgBTAephojYn7qwVkDBF9qn1l\n"
		"uMrMTjANBgkqhkiG9w0BAQUFAAOCAQEANeMpauUvXVSOKVCUn5kaFOSPeCpilKIn\n"
		"Z57QzxpeR+nBsqTP3UEaBU6bS+5Kb1VSsyShNwrrZHYqLizz/Tt1kL/6cdjHPTfS\n"
		"tQWVYrmm3ok9Nns4d0iXrKYgjy6myQzCsplFAMfOEVEiIuCl6rYVSAlk6l5PdPcF\n"
		"PseKUgzbFbS9bZvlxrFUaKnjaZC2mqUPuLk/IH2uSrW4nOQdtqvmlKXBx4Ot2/Un\n"
		"hw4EbNX/3aBd7YdStysVAq45pmp06drE57xNNB6pXE0zX5IJL4hmXXeXxx12E6nV\n"
		"5fEWCRE11azbJHFwLJhWC9kXtNHjUStedejV0NxPNO3CBWaAocvmMw==\n"
		"-----END CERTIFICATE-----\n\n"

		// Equifax root CA cert
		// Exported from Mozilla Firefox's built-in CA repository
		"-----BEGIN CERTIFICATE-----\n"
		"MIIDIDCCAomgAwIBAgIENd70zzANBgkqhkiG9w0BAQUFADBOMQswCQYDVQQGEwJV\n"
		"UzEQMA4GA1UEChMHRXF1aWZheDEtMCsGA1UECxMkRXF1aWZheCBTZWN1cmUgQ2Vy\n"
		"dGlmaWNhdGUgQXV0aG9yaXR5MB4XDTk4MDgyMjE2NDE1MVoXDTE4MDgyMjE2NDE1\n"
		"MVowTjELMAkGA1UEBhMCVVMxEDAOBgNVBAoTB0VxdWlmYXgxLTArBgNVBAsTJEVx\n"
		"dWlmYXggU2VjdXJlIENlcnRpZmljYXRlIEF1dGhvcml0eTCBnzANBgkqhkiG9w0B\n"
		"AQEFAAOBjQAwgYkCgYEAwV2xWGcIYu6gmi0fCG2RFGiYCh7+2gRvE4RiIcPRfM6f\n"
		"BeC4AfBONOziipUEZKzxa1NfBbPLZ4C/QgKO/t0BCezhABRP/PvwDN1Dulsr4R+A\n"
		"cJkVV5MW8Q+XarfCaCMczE1ZMKxRHjuvK9buY0V7xdlfUNLjUA86iOe/FP3gx7kC\n"
		"AwEAAaOCAQkwggEFMHAGA1UdHwRpMGcwZaBjoGGkXzBdMQswCQYDVQQGEwJVUzEQ\n"
		"MA4GA1UEChMHRXF1aWZheDEtMCsGA1UECxMkRXF1aWZheCBTZWN1cmUgQ2VydGlm\n"
		"aWNhdGUgQXV0aG9yaXR5MQ0wCwYDVQQDEwRDUkwxMBoGA1UdEAQTMBGBDzIwMTgw\n"
		"ODIyMTY0MTUxWjALBgNVHQ8EBAMCAQYwHwYDVR0jBBgwFoAUSOZo+SvSspXXR9gj\n"
		"IBBPM5iQn9QwHQYDVR0OBBYEFEjmaPkr0rKV10fYIyAQTzOYkJ/UMAwGA1UdEwQF\n"
		"MAMBAf8wGgYJKoZIhvZ9B0EABA0wCxsFVjMuMGMDAgbAMA0GCSqGSIb3DQEBBQUA\n"
		"A4GBAFjOKer89961zgK5F7WF0bnj4JXMJTENAKaSbn+2kmOeUJXRmm/kEd5jhW6Y\n"
		"7qj/WsjTVbJmcVfewCHrPSqnI0kBBIZCe/zuf6IWUrVnZ9NA2zsmWLIodz2uFHdh\n"
		"1voqZiegDfqnc1zqcPGUIWVEX/r87yloqaKHee9570+sB3c4\n"
		"-----END CERTIFICATE-----\n\n"

		// Starfield G2 cert
		// This is the data of the root certs for Starfield Technologies, the CA that used to sign sessionserver.mojang.com's cert
		// Downloaded from https://certs.starfieldtech.com/repository/
		"-----BEGIN CERTIFICATE-----\n"
		"MIID3TCCAsWgAwIBAgIBADANBgkqhkiG9w0BAQsFADCBjzELMAkGA1UEBhMCVVMx\n"
		"EDAOBgNVBAgTB0FyaXpvbmExEzARBgNVBAcTClNjb3R0c2RhbGUxJTAjBgNVBAoT\n"
		"HFN0YXJmaWVsZCBUZWNobm9sb2dpZXMsIEluYy4xMjAwBgNVBAMTKVN0YXJmaWVs\n"
		"ZCBSb290IENlcnRpZmljYXRlIEF1dGhvcml0eSAtIEcyMB4XDTA5MDkwMTAwMDAw\n"
		"MFoXDTM3MTIzMTIzNTk1OVowgY8xCzAJBgNVBAYTAlVTMRAwDgYDVQQIEwdBcml6\n"
		"b25hMRMwEQYDVQQHEwpTY290dHNkYWxlMSUwIwYDVQQKExxTdGFyZmllbGQgVGVj\n"
		"aG5vbG9naWVzLCBJbmMuMTIwMAYDVQQDEylTdGFyZmllbGQgUm9vdCBDZXJ0aWZp\n"
		"Y2F0ZSBBdXRob3JpdHkgLSBHMjCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoC\n"
		"ggEBAL3twQP89o/8ArFvW59I2Z154qK3A2FWGMNHttfKPTUuiUP3oWmb3ooa/RMg\n"
		"nLRJdzIpVv257IzdIvpy3Cdhl+72WoTsbhm5iSzchFvVdPtrX8WJpRBSiUZV9Lh1\n"
		"HOZ/5FSuS/hVclcCGfgXcVnrHigHdMWdSL5stPSksPNkN3mSwOxGXn/hbVNMYq/N\n"
		"Hwtjuzqd+/x5AJhhdM8mgkBj87JyahkNmcrUDnXMN/uLicFZ8WJ/X7NfZTD4p7dN\n"
		"dloedl40wOiWVpmKs/B/pM293DIxfJHP4F8R+GuqSVzRmZTRouNjWwl2tVZi4Ut0\n"
		"HZbUJtQIBFnQmA4O5t78w+wfkPECAwEAAaNCMEAwDwYDVR0TAQH/BAUwAwEB/zAO\n"
		"BgNVHQ8BAf8EBAMCAQYwHQYDVR0OBBYEFHwMMh+n2TB/xH1oo2Kooc6rB1snMA0G\n"
		"CSqGSIb3DQEBCwUAA4IBAQARWfolTwNvlJk7mh+ChTnUdgWUXuEok21iXQnCoKjU\n"
		"sHU48TRqneSfioYmUeYs0cYtbpUgSpIB7LiKZ3sx4mcujJUDJi5DnUox9g61DLu3\n"
		"4jd/IroAow57UvtruzvE03lRTs2Q9GcHGcg8RnoNAX3FWOdt5oUwF5okxBDgBPfg\n"
		"8n/Uqgr/Qh037ZTlZFkSIHc40zI+OIF1lnP6aI+xy84fxez6nH7PfrHxBy22/L/K\n"
		"pL/QlwVKvOoYKAKQvVR4CSFx09F9HdkWsKlhPdAKACL8x3vLCWRFCztAgfd9fDL1\n"
		"mMpYjn0q7pBZc2T5NnReJaH1ZgUufzkVqSr7UIuOhWn0\n"
		"-----END CERTIFICATE-----\n\n"

		// Starfield original (G1) cert:
		"-----BEGIN CERTIFICATE-----\n"
		"MIIEDzCCAvegAwIBAgIBADANBgkqhkiG9w0BAQUFADBoMQswCQYDVQQGEwJVUzEl\n"
		"MCMGA1UEChMcU3RhcmZpZWxkIFRlY2hub2xvZ2llcywgSW5jLjEyMDAGA1UECxMp\n"
		"U3RhcmZpZWxkIENsYXNzIDIgQ2VydGlmaWNhdGlvbiBBdXRob3JpdHkwHhcNMDQw\n"
		"NjI5MTczOTE2WhcNMzQwNjI5MTczOTE2WjBoMQswCQYDVQQGEwJVUzElMCMGA1UE\n"
		"ChMcU3RhcmZpZWxkIFRlY2hub2xvZ2llcywgSW5jLjEyMDAGA1UECxMpU3RhcmZp\n"
		"ZWxkIENsYXNzIDIgQ2VydGlmaWNhdGlvbiBBdXRob3JpdHkwggEgMA0GCSqGSIb3\n"
		"DQEBAQUAA4IBDQAwggEIAoIBAQC3Msj+6XGmBIWtDBFk385N78gDGIc/oav7PKaf\n"
		"8MOh2tTYbitTkPskpD6E8J7oX+zlJ0T1KKY/e97gKvDIr1MvnsoFAZMej2YcOadN\n"
		"+lq2cwQlZut3f+dZxkqZJRRU6ybH838Z1TBwj6+wRir/resp7defqgSHo9T5iaU0\n"
		"X9tDkYI22WY8sbi5gv2cOj4QyDvvBmVmepsZGD3/cVE8MC5fvj13c7JdBmzDI1aa\n"
		"K4UmkhynArPkPw2vCHmCuDY96pzTNbO8acr1zJ3o/WSNF4Azbl5KXZnJHoe0nRrA\n"
		"1W4TNSNe35tfPe/W93bC6j67eA0cQmdrBNj41tpvi/JEoAGrAgEDo4HFMIHCMB0G\n"
		"A1UdDgQWBBS/X7fRzt0fhvRbVazc1xDCDqmI5zCBkgYDVR0jBIGKMIGHgBS/X7fR\n"
		"zt0fhvRbVazc1xDCDqmI56FspGowaDELMAkGA1UEBhMCVVMxJTAjBgNVBAoTHFN0\n"
		"YXJmaWVsZCBUZWNobm9sb2dpZXMsIEluYy4xMjAwBgNVBAsTKVN0YXJmaWVsZCBD\n"
		"bGFzcyAyIENlcnRpZmljYXRpb24gQXV0aG9yaXR5ggEAMAwGA1UdEwQFMAMBAf8w\n"
		"DQYJKoZIhvcNAQEFBQADggEBAAWdP4id0ckaVaGsafPzWdqbAYcaT1epoXkJKtv3\n"
		"L7IezMdeatiDh6GX70k1PncGQVhiv45YuApnP+yz3SFmH8lU+nLMPUxA2IGvd56D\n"
		"eruix/U0F47ZEUD0/CwqTRV/p2JdLiXTAAsgGh1o+Re49L2L7ShZ3U0WixeDyLJl\n"
		"xy16paq8U4Zt3VekyvggQQto8PT7dL5WXXp59fkdheMtlb71cZBDzI0fmgAKhynp\n"
		"VSJYACPq4xJDKVtHCN2MQWplBqjlIapBtJUhlbl90TSrE9atvNziPTnNvT51cKEY\n"
		"WQPJIrSPnNVeKtelttQKbfi3QBFGmh95DmK/D5fs4C8fF5Q=\n"
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
		Super("cMojangAPI::cUpdateThread"),
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
		LOGWARNING("%s: Can't connect to %s: %s", __FUNCTION__, a_ServerName.c_str(), Socket.GetLastErrorText().c_str());
		return false;
	}

	if (!Socket.Send(a_Request.c_str(), a_Request.size()))
	{
		LOGWARNING("%s: Writing SSL data failed: %s", __FUNCTION__, Socket.GetLastErrorText().c_str());
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
			LOGWARNING("%s: SSL reading failed internally", __FUNCTION__);
			return false;
		}
		if (ret == MBEDTLS_ERR_SSL_PEER_CLOSE_NOTIFY)
		{
			break;
		}
		if (ret < 0)
		{
			LOGWARNING("%s: SSL reading failed: -0x%x", __FUNCTION__, -ret);
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
			LOGINFO("%s failed: bad HTTP status line received", __FUNCTION__);
			LOGD("Response: \n%s", CreateHexDump(HexDump, Response.data(), Response.size(), 16).c_str());
			continue;
		}

		// Erase the HTTP headers from the response:
		size_t idxHeadersEnd = Response.find("\r\n\r\n");
		if (idxHeadersEnd == AString::npos)
		{
			LOGINFO("%s failed: bad HTTP response header received", __FUNCTION__);
			LOGD("Response: \n%s", CreateHexDump(HexDump, Response.data(), Response.size(), 16).c_str());
			continue;
		}
		Response.erase(0, idxHeadersEnd + 4);

		// Parse the returned string into Json:
		AString ParseError;
		if (!JsonUtils::ParseString(Response, root, &ParseError) || !root.isArray())
		{
			LOGWARNING("%s failed: Cannot parse received data (NameToUUID) to JSON: \"%s\"", __FUNCTION__, ParseError);
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
	ReplaceString(Address, "%UUID%", a_UUID.ToShortString());

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
		LOGINFO("%s failed: bad HTTP status line received", __FUNCTION__);
		LOGD("Response: \n%s", CreateHexDump(HexDump, Response.data(), Response.size(), 16).c_str());
		return;
	}

	// Erase the HTTP headers from the response:
	size_t idxHeadersEnd = Response.find("\r\n\r\n");
	if (idxHeadersEnd == AString::npos)
	{
		LOGINFO("%s failed: bad HTTP response header received", __FUNCTION__);
		LOGD("Response: \n%s", CreateHexDump(HexDump, Response.data(), Response.size(), 16).c_str());
		return;
	}
	Response.erase(0, idxHeadersEnd + 4);

	// Parse the returned string into Json:
	Json::Value root;
	AString ParseError;
	if (!JsonUtils::ParseString(Response, root, &ParseError) || !root.isObject())
	{
		LOGWARNING("%s failed: Cannot parse received data (NameToUUID) to JSON: \"%s\"", __FUNCTION__, ParseError);
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
		LOG("cMojangAPI: Updating name-to-uuid cache for %u names", static_cast<unsigned>(PlayerNames.size()));
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
		LOG("cMojangAPI: Updating uuid-to-profile cache for %u uuids", static_cast<unsigned>(ProfileUUIDs.size()));
		for (const auto & UUID : ProfileUUIDs)
		{
			QueryUUIDToProfile(UUID);
		}
	}
}
