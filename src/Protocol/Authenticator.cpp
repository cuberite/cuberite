
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Authenticator.h"
#include "MojangAPI.h"
#include "../Root.h"
#include "../Server.h"
#include "../ClientHandle.h"
#include "../UUID.h"

#include "../IniFile.h"
#include "../JsonUtils.h"
#include "json/json.h"

#include "../mbedTLS++/BlockingSslClientSocket.h"





#define DEFAULT_AUTH_SERVER "sessionserver.mojang.com"
#define DEFAULT_AUTH_ADDRESS "/session/minecraft/hasJoined?username=%USERNAME%&serverId=%SERVERID%"





cAuthenticator::cAuthenticator(void) :
	Super("cAuthenticator"),
	m_Server(DEFAULT_AUTH_SERVER),
	m_Address(DEFAULT_AUTH_ADDRESS),
	m_ShouldAuthenticate(true)
{
}





cAuthenticator::~cAuthenticator()
{
	Stop();
}





void cAuthenticator::ReadSettings(cSettingsRepositoryInterface & a_Settings)
{
	m_Server             = a_Settings.GetValueSet ("Authentication", "Server", DEFAULT_AUTH_SERVER);
	m_Address            = a_Settings.GetValueSet ("Authentication", "Address", DEFAULT_AUTH_ADDRESS);
	m_ShouldAuthenticate = a_Settings.GetValueSetB("Authentication", "Authenticate", true);
}





void cAuthenticator::Authenticate(int a_ClientID, const AString & a_UserName, const AString & a_ServerHash)
{
	if (!m_ShouldAuthenticate)
	{
		Json::Value Value;
		cRoot::Get()->AuthenticateUser(a_ClientID, a_UserName, cClientHandle::GenerateOfflineUUID(a_UserName), Value);
		return;
	}

	cCSLock LOCK(m_CS);
	m_Queue.push_back(cUser(a_ClientID, a_UserName, a_ServerHash));
	m_QueueNonempty.Set();
}





void cAuthenticator::Start(cSettingsRepositoryInterface & a_Settings)
{
	ReadSettings(a_Settings);
	Super::Start();
}





void cAuthenticator::Stop(void)
{
	m_ShouldTerminate = true;
	m_QueueNonempty.Set();
	Super::Stop();
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
		cUUID UUID;
		Json::Value Properties;
		if (AuthWithYggdrasil(NewUserName, ServerID, UUID, Properties))
		{
			LOGINFO("User %s authenticated with UUID %s", NewUserName.c_str(), UUID.ToShortString().c_str());
			cRoot::Get()->AuthenticateUser(ClientID, NewUserName, UUID, Properties);
		}
		else
		{
			cRoot::Get()->KickUser(ClientID, "Failed to authenticate account!");
		}
	}  // for (-ever)
}





bool cAuthenticator::AuthWithYggdrasil(AString & a_UserName, const AString & a_ServerId, cUUID & a_UUID, Json::Value & a_Properties)
{
	LOGD("Trying to authenticate user %s", a_UserName.c_str());

	// Create the GET request:
	AString ActualAddress = m_Address;
	ReplaceString(ActualAddress, "%USERNAME%", a_UserName);
	ReplaceString(ActualAddress, "%SERVERID%", a_ServerId);

	AString Request;
	Request += "GET " + ActualAddress + " HTTP/1.0\r\n";
	Request += "Host: " + m_Server + "\r\n";
	Request += "User-Agent: Cuberite\r\n";
	Request += "Connection: close\r\n";
	Request += "\r\n";

	AString Response;
	if (!cMojangAPI::SecureRequest(m_Server, Request, Response))
	{
		return false;
	}

	// Check the HTTP status line:
	const AString Prefix("HTTP/1.1 200 OK");
	AString HexDump;
	if (Response.compare(0, Prefix.size(), Prefix))
	{
		LOGINFO("User %s failed to auth, bad HTTP status line received", a_UserName.c_str());
		LOGD("Response: \n%s", CreateHexDump(HexDump, Response.data(), Response.size(), 16).c_str());
		return false;
	}

	// Erase the HTTP headers from the response:
	size_t idxHeadersEnd = Response.find("\r\n\r\n");
	if (idxHeadersEnd == AString::npos)
	{
		LOGINFO("User %s failed to authenticate, bad HTTP response header received", a_UserName.c_str());
		LOGD("Response: \n%s", CreateHexDump(HexDump, Response.data(), Response.size(), 16).c_str());
		return false;
	}
	Response.erase(0, idxHeadersEnd + 4);

	// Parse the Json response:
	if (Response.empty())
	{
		return false;
	}
	Json::Value root;
	if (!JsonUtils::ParseString(Response, root))
	{
		LOGWARNING("cAuthenticator: Cannot parse received data (authentication) to JSON!");
		return false;
	}
	a_UserName = root.get("name", "Unknown").asString();
	a_Properties = root["properties"];
	if (!a_UUID.FromString(root.get("id", "").asString()))
	{
		LOGWARNING("cAuthenticator: Recieved invalid UUID format");
		return false;
	}

	// Store the player's profile in the MojangAPI caches:
	cRoot::Get()->GetMojangAPI().AddPlayerProfile(a_UserName, a_UUID, a_Properties);

	return true;
}





/* In case we want to export this function to the plugin API later - don't forget to add the relevant INI configuration lines for DEFAULT_PROPERTIES_ADDRESS

#define DEFAULT_PROPERTIES_ADDRESS "/session/minecraft/profile/%UUID%"

// Gets the properties, such as skin, of a player based on their UUID via Mojang's API
bool GetPlayerProperties(const AString & a_UUID, Json::Value & a_Properties);

bool cAuthenticator::GetPlayerProperties(const AString & a_UUID, Json::Value & a_Properties)
{
	LOGD("Trying to get properties for user %s", a_UUID.c_str());

	// Create the GET request:
	AString ActualAddress = m_PropertiesAddress;
	ReplaceString(ActualAddress, "%UUID%", a_UUID);

	AString Request;
	Request += "GET " + ActualAddress + " HTTP/1.0\r\n";
	Request += "Host: " + m_Server + "\r\n";
	Request += "User-Agent: Cuberite\r\n";
	Request += "Connection: close\r\n";
	Request += "\r\n";

	AString Response;
	if (!ConnectSecurelyToAddress(StarfieldCACert(), m_Server, Request, Response))
	{
		return false;
	}

	// Check the HTTP status line:
	const AString Prefix("HTTP/1.1 200 OK");
	AString HexDump;
	if (Response.compare(0, Prefix.size(), Prefix))
	{
		LOGINFO("Failed to get properties for user %s, bad HTTP status line received", a_UUID.c_str());
		LOGD("Response: \n%s", CreateHexDump(HexDump, Response.data(), Response.size(), 16).c_str());
		return false;
	}

	// Erase the HTTP headers from the response:
	size_t idxHeadersEnd = Response.find("\r\n\r\n");
	if (idxHeadersEnd == AString::npos)
	{
		LOGINFO("Failed to get properties for user %s, bad HTTP response header received", a_UUID.c_str());
		LOGD("Response: \n%s", CreateHexDump(HexDump, Response.data(), Response.size(), 16).c_str());
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
		LOGWARNING("cAuthenticator: Cannot parse received properties data to JSON!");
		return false;
	}

	a_Properties = root["properties"];
	return true;
}
*/




