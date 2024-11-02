
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Protocol/Authenticator.h"

#include "ClientHandle.h"
#include "HTTP/UrlClient.h"
#include "HTTP/UrlParser.h"
#include "IniFile.h"
#include "JsonUtils.h"
#include "json/json.h"
#include "Protocol/MojangAPI.h"
#include "Root.h"
#include "Server.h"
#include "UUID.h"





constexpr char DEFAULT_AUTH_SERVER[] = "sessionserver.mojang.com";
constexpr char DEFAULT_AUTH_ADDRESS[] = "/session/minecraft/hasJoined?username=%USERNAME%&serverId=%SERVERID%";





cAuthenticator::cAuthenticator(void) :
	Super("Authenticator"), m_Server(DEFAULT_AUTH_SERVER), m_Address(DEFAULT_AUTH_ADDRESS), m_ShouldAuthenticate(true)
{
}





cAuthenticator::~cAuthenticator()
{
	Stop();
}





void cAuthenticator::ReadSettings(cSettingsRepositoryInterface & a_Settings)
{
	m_Server = a_Settings.GetValueSet("Authentication", "Server", DEFAULT_AUTH_SERVER);
	m_Address = a_Settings.GetValueSet("Authentication", "Address", DEFAULT_AUTH_ADDRESS);
	m_ShouldAuthenticate = a_Settings.GetValueSetB("Authentication", "Authenticate", true);

	// prepend https:// if missing
	constexpr std::string_view HttpPrefix = "http://";
	constexpr std::string_view HttpsPrefix = "https://";

	if ((std::string_view(m_Server).substr(0, HttpPrefix.size()) != HttpPrefix) &&
		(std::string_view(m_Server).substr(0, HttpsPrefix.size()) != HttpsPrefix))
	{
		m_Server = "https://" + m_Server;
	}

	{
		auto [IsSuccessful, ErrorMessage] = cUrlParser::Validate(m_Server);
		if (!IsSuccessful)
		{
			LOGWARNING(
				"%s %d: Supplied invalid URL for configuration value [Authentication: Server]: \"%s\", using default! "
				"Error: %s",
				__FUNCTION__,
				__LINE__,
				m_Server.c_str(),
				ErrorMessage.c_str()
			);
			m_Server = DEFAULT_AUTH_SERVER;
		}
	}

	{
		auto [IsSuccessful, ErrorMessage] = cUrlParser::Validate(m_Server);
		if (!IsSuccessful)
		{
			LOGWARNING(
				"%s %d: Supplied invalid URL for configuration value [Authentication: Address]: \"%s\", using default! "
				"Error: %s",
				__FUNCTION__,
				__LINE__,
				m_Address.c_str(),
				ErrorMessage.c_str()
			);
			m_Address = DEFAULT_AUTH_ADDRESS;
		}
	}
}





void cAuthenticator::Authenticate(
	int a_ClientID,
	const std::string_view a_Username,
	const std::string_view a_ServerHash
)
{
	if (!m_ShouldAuthenticate)
	{
		// An "authenticated" username, which is just what the client sent since auth is off.
		std::string OfflineUsername(a_Username);

		// A specially constructed UUID based wholly on the username.
		const auto OfflineUUID = cClientHandle::GenerateOfflineUUID(OfflineUsername);

		// "Authenticate" the user based on what little information we have:
		cRoot::Get()->GetServer()->AuthenticateUser(a_ClientID, std::move(OfflineUsername), OfflineUUID, Json::Value());
		return;
	}

	cCSLock Lock(m_CS);
	m_Queue.emplace_back(a_ClientID, a_Username, a_ServerHash);
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

		cAuthenticator::cUser User = std::move(m_Queue.front());
		int & ClientID = User.m_ClientID;
		AString & Username = User.m_Name;
		AString & ServerID = User.m_ServerID;
		m_Queue.pop_front();
		Lock.Unlock();

		cUUID UUID;
		Json::Value Properties;
		if (AuthWithYggdrasil(Username, ServerID, UUID, Properties))
		{
			LOGINFO("User %s authenticated with UUID %s", Username.c_str(), UUID.ToShortString().c_str());
			cRoot::Get()->GetServer()->AuthenticateUser(ClientID, std::move(Username), UUID, std::move(Properties));
		}
		else
		{
			cRoot::Get()->KickUser(ClientID, "Failed to authenticate account!");
		}
	}  // for (-ever)
}





bool cAuthenticator::AuthWithYggdrasil(
	AString & a_UserName,
	const AString & a_ServerId,
	cUUID & a_UUID,
	Json::Value & a_Properties
) const
{
	LOGD("Trying to authenticate user %s", a_UserName.c_str());

	// Create the GET request:
	AString ActualAddress = m_Address;
	ReplaceURL(ActualAddress, "%USERNAME%", a_UserName);
	ReplaceURL(ActualAddress, "%SERVERID%", a_ServerId);

	// Create and send the HTTP request
	auto [IsSuccessful, Response] = cUrlClient::BlockingGet(m_Server + ActualAddress);
	if (!IsSuccessful)
	{
		return false;
	}

	// Parse the Json response:
	if (Response.empty())
	{
		return false;
	}
	Json::Value root;
	if (!JsonUtils::ParseString(Response, root))
	{
		LOGWARNING("%s: Cannot parse received data (authentication) to JSON!", __FUNCTION__);
		return false;
	}
	a_UserName = root.get("name", "Unknown").asString();
	a_Properties = root["properties"];
	if (!a_UUID.FromString(root.get("id", "").asString()))
	{
		LOGWARNING("%s: Received invalid UUID format", __FUNCTION__);
		return false;
	}

	// Store the player's profile in the MojangAPI caches:
	cRoot::Get()->GetMojangAPI().AddPlayerProfile(a_UserName, a_UUID, a_Properties);

	return true;
}



#ifdef ENABLE_PROPERTIES

/* In case we want to export this function to the plugin API later - don't forget to add the relevant INI configuration
 * lines for DEFAULT_PROPERTIES_ADDRESS */

#define DEFAULT_PROPERTIES_ADDRESS "/session/minecraft/profile/%UUID%"

// Gets the properties, such as skin, of a player based on their UUID via Mojang's API
bool GetPlayerProperties(const AString & a_UUID, Json::Value & a_Properties);

bool cAuthenticator::GetPlayerProperties(const AString & a_UUID, Json::Value & a_Properties)
{
	LOGD("Trying to get properties for user %s", a_UUID.c_str());

	// Create and send the HTTP request
	auto [IsSuccessful, Response] = cUrlClient::BlockingGet(m_Server + ActualAddress);
	if (!IsSuccessful)
	{
		return false;
	}

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
#endif
