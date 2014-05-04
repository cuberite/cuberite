
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Authenticator.h"
#include "../Root.h"
#include "../Server.h"
#include "../ClientHandle.h"

#include "inifile/iniFile.h"
#include "json/json.h"

#include "PolarSSL++/BlockingSslClientSocket.h"

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
	
	int ret;
	unsigned char buf[1024];

	/* Initialize certificates */
	// This is the data of the root certs for Starfield Technologies, the CA that signed sessionserver.mojang.com's cert:
	// Downloaded from http://certs.starfieldtech.com/repository/
	static const AString StarfieldCACert(
		// G2 cert
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
		// Original (G1) cert:
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
	);

	// Connect the socket:
	cBlockingSslClientSocket Socket;
	Socket.SetTrustedRootCertsFromString(StarfieldCACert, m_Server);
	if (!Socket.Connect(m_Server, 443))
	{
		LOGWARNING("cAuthenticator: Can't connect to %s: %s", m_Server.c_str(), Socket.GetLastErrorText().c_str());
		return false;
	}

	// Create the GET request:
	AString ActualAddress = m_Address;
	ReplaceString(ActualAddress, "%USERNAME%", a_UserName);
	ReplaceString(ActualAddress, "%SERVERID%", a_ServerId);

	AString Request;
	Request += "GET " + ActualAddress + " HTTP/1.0\r\n";
	Request += "Host: " + m_Server + "\r\n";
	Request += "User-Agent: MCServer\r\n";
	Request += "Connection: close\r\n";
	Request += "\r\n";

	if (!Socket.Send(Request.c_str(), Request.size()))
	{
		LOGWARNING("cAuthenticator: Writing SSL data failed: %s", Socket.GetLastErrorText().c_str());
		return false;
	}

	// Read the HTTP response:
	std::string Response;
	for (;;)
	{
		ret = Socket.Receive(buf, sizeof(buf));

		if ((ret == POLARSSL_ERR_NET_WANT_READ) || (ret == POLARSSL_ERR_NET_WANT_WRITE))
		{
			// This value should never be returned, it is handled internally by cBlockingSslClientSocket
			LOGWARNING("cAuthenticator: SSL reading failed internally.");
			return false;
		}
		if (ret == POLARSSL_ERR_SSL_PEER_CLOSE_NOTIFY)
		{
			break;
		}
		if (ret < 0)
		{
			LOGWARNING("cAuthenticator: SSL reading failed: -0x%x", -ret);
			return false;
		}
		if (ret == 0)
		{
			break;
		}

		Response.append((const char *)buf, (size_t)ret);
	}

	Socket.Disconnect();

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





