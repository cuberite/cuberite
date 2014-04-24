
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Authenticator.h"
#include "../OSSupport/BlockingTCPLink.h"
#include "../Root.h"
#include "../Server.h"
#include "../ClientHandle.h"

#include "inifile/iniFile.h"
#include "json/json.h"

#include "polarssl/config.h"
#include "polarssl/net.h"
#include "polarssl/ssl.h"
#include "polarssl/ctr_drbg.h"
#include "polarssl/error.h"

#include "PolarSSL++/EntropyContext.h"
#include "PolarSSL++/CtrDrbgContext.h"
#include "PolarSSL++/X509Cert.h"

#include <sstream>
#include <iomanip>





#define DEFAULT_AUTH_SERVER "sessionserver.mojang.com"
#define DEFAULT_AUTH_ADDRESS "/session/minecraft/hasJoined?username=%USERNAME%&serverId=%SERVERID%"





cAuthenticator::cAuthenticator(void) :
	super("cAuthenticator"),
	m_Server(DEFAULT_AUTH_SERVER),
	m_Address(DEFAULT_AUTH_ADDRESS),
	m_ShouldAuthenticate(true)
{
}





cAuthenticator::~cAuthenticator()
{
	Stop();
}





void cAuthenticator::ReadINI(cIniFile & IniFile)
{
	m_Server = IniFile.GetValueSet("Authentication", "Server", DEFAULT_AUTH_SERVER);
	m_Address = IniFile.GetValueSet("Authentication", "Address", DEFAULT_AUTH_ADDRESS);
	m_ShouldAuthenticate = IniFile.GetValueSetB("Authentication", "Authenticate", true);
}





void cAuthenticator::Authenticate(int a_ClientID, const AString & a_UserName, const AString & a_ServerHash)
{
	if (!m_ShouldAuthenticate)
	{
		cRoot::Get()->AuthenticateUser(a_ClientID, a_UserName, cClientHandle::GenerateOfflineUUID(a_UserName));
		return;
	}

	cCSLock LOCK(m_CS);
	m_Queue.push_back(cUser(a_ClientID, a_UserName, a_ServerHash));
	m_QueueNonempty.Set();
}





void cAuthenticator::Start(cIniFile & IniFile)
{
	ReadINI(IniFile);
	m_ShouldTerminate = false;
	super::Start();
}





void cAuthenticator::Stop(void)
{
	m_ShouldTerminate = true;
	m_QueueNonempty.Set();
	Wait();
}





void cAuthenticator::Execute(void)
{
	for (;;)
	{
		cCSLock Lock(m_CS);
		while (!m_ShouldTerminate && (m_Queue.size() == 0))
		{
			cCSUnlock Unlock(Lock);
			m_QueueNonempty.Wait();
		}
		if (m_ShouldTerminate)
		{
			return;
		}
		ASSERT(!m_Queue.empty());

		cAuthenticator::cUser & User = m_Queue.front();
		int ClientID = User.m_ClientID;
		AString UserName = User.m_Name;
		AString ServerID = User.m_ServerID;
		m_Queue.pop_front();
		Lock.Unlock();

		AString NewUserName = UserName;
		AString UUID;
		if (AuthWithYggdrasil(NewUserName, ServerID, UUID))
		{
			LOGINFO("User %s authenticated with UUID '%s'", NewUserName.c_str(), UUID.c_str());
			cRoot::Get()->AuthenticateUser(ClientID, NewUserName, UUID);
		}
		else
		{
			cRoot::Get()->KickUser(ClientID, "Failed to authenticate account!");
		}
	}  // for (-ever)
}





bool cAuthenticator::AuthWithYggdrasil(AString & a_UserName, const AString & a_ServerId, AString & a_UUID)
{
	LOGD("Trying to auth user %s", a_UserName.c_str());
	
	int ret, server_fd = -1;
	unsigned char buf[1024];
	const char pers[] = "cAuthenticator";

	cCtrDrbgContext CtrDrbg;
	ssl_context ssl;
	cX509Cert CACert;

	/* Initialize the RNG and the session data */
	memset(&ssl, 0, sizeof(ssl_context));

	if ((ret = CtrDrbg.Initialize(pers, sizeof(pers))) != 0)
	{
		LOGWARNING("cAuthenticator: CtrDrbg.Initialize() returned %d", ret);
		return false;
	}

	/* Initialize certificates */
	// This is the root cert for Starfield Technologies, the CA that signed sessionserver.mojang.com's cert:
	static const char StarfieldCACert[] =
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
	"-----END CERTIFICATE-----";
	// Parse the Starfield CA and add it to "trusted root certs"
	ret = CACert.Parse(StarfieldCACert, sizeof(StarfieldCACert) - 1);
	if (ret < 0)
	{
		LOGWARNING("cAuthenticator: CACert.Parse returned -0x%x", -ret);
		return false;
	}

	/* Connect */
	if ((ret = net_connect(&server_fd, m_Server.c_str(), 443)) != 0)
	{
		LOGWARNING("cAuthenticator: Can't connect to %s: %d", m_Server.c_str(), ret);
		return false;
	}

	/* Setup */
	if ((ret = ssl_init(&ssl)) != 0)
	{
		LOGWARNING("cAuthenticator: ssl_init returned %d", ret);
		return false;
	}
	ssl_set_endpoint(&ssl, SSL_IS_CLIENT);
	ssl_set_authmode(&ssl, SSL_VERIFY_OPTIONAL);
	ssl_set_ca_chain(&ssl, CACert.Get(), NULL, "PolarSSL Server 1");
	ssl_set_rng(&ssl, ctr_drbg_random, CtrDrbg.Get());
	ssl_set_bio(&ssl, net_recv, &server_fd, net_send, &server_fd);

	/* Handshake */
	while ((ret = ssl_handshake(&ssl)) != 0)
	{
		if ((ret != POLARSSL_ERR_NET_WANT_READ) && (ret != POLARSSL_ERR_NET_WANT_WRITE))
		{
			LOGWARNING("cAuthenticator: ssl_handshake returned -0x%x", -ret);
			return false;
		}
	}

	/* Write the GET request */
	AString ActualAddress = m_Address;
	ReplaceString(ActualAddress, "%USERNAME%", a_UserName);
	ReplaceString(ActualAddress, "%SERVERID%", a_ServerId);

	AString Request;
	Request += "GET " + ActualAddress + " HTTP/1.1\r\n";
	Request += "Host: " + m_Server + "\r\n";
	Request += "User-Agent: MCServer\r\n";
	Request += "Connection: close\r\n";
	Request += "\r\n";

	ret = ssl_write(&ssl, (const unsigned char *)Request.c_str(), Request.size());
	if (ret <= 0)
	{
		LOGWARNING("cAuthenticator: ssl_write returned %d", ret);
		return false;
	}

	/* Read the HTTP response */
	std::string Response;
	for (;;)
	{
		memset(buf, 0, sizeof(buf));
		ret = ssl_read(&ssl, buf, sizeof(buf));

		if ((ret == POLARSSL_ERR_NET_WANT_READ) || (ret == POLARSSL_ERR_NET_WANT_WRITE))
		{
			continue;
		}
		if (ret == POLARSSL_ERR_SSL_PEER_CLOSE_NOTIFY)
		{
			break;
		}
		if (ret < 0)
		{
			LOGWARNING("cAuthenticator: ssl_read returned %d", ret);
			break;
		}
		if (ret == 0)
		{
			LOGWARNING("cAuthenticator: EOF");
			break;
		}

		Response.append((const char *)buf, ret);
	}

	ssl_close_notify(&ssl);
	net_close(server_fd);
	ssl_free(&ssl);
	memset(&ssl, 0, sizeof(ssl));

	// Check the HTTP status line:
	AString prefix("HTTP/1.1 200 OK");
	AString HexDump;
	if (Response.compare(0, prefix.size(), prefix))
	{
		LOGINFO("User \"%s\" failed to auth, bad http status line received", a_UserName.c_str());
		LOG("Response: \n%s", CreateHexDump(HexDump, Response.data(), Response.size(), 16).c_str());
		return false;
	}

	// Erase the HTTP headers from the response:
	size_t idxHeadersEnd = Response.find("\r\n\r\n");
	if (idxHeadersEnd == AString::npos)
	{
		LOGINFO("User \"%s\" failed to authenticate, bad http response header received", a_UserName.c_str());
		LOG("Response: \n%s", CreateHexDump(HexDump, Response.data(), Response.size(), 16).c_str());
		return false;
	}
	Response.erase(0, idxHeadersEnd + 4);

	// Parse the Json response:
	if (Response.empty())
	{
		return false;
	}
	Json::Value root;
	Json::Reader reader;
	if (!reader.parse(Response, root, false))
	{
		LOGWARNING("cAuthenticator: Cannot parse Received Data to json!");
		return false;
	}
	a_UserName = root.get("name", "Unknown").asString();
	a_UUID = root.get("id", "").asString();
	
	// If the UUID doesn't contain the hashes, insert them at the proper places:
	if (a_UUID.size() == 32)
	{
		a_UUID.insert(8, "-");
		a_UUID.insert(13, "-");
		a_UUID.insert(18, "-");
		a_UUID.insert(23, "-");
	}
	return true;
}





