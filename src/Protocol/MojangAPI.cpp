
// MojangAPI.cpp

// Implements the cMojangAPI class representing the various API points provided by Mojang's webservices, and a cache for their results

#include "Globals.h"
#include "MojangAPI.h"

#include "HTTP/UrlClient.h"
#include "IniFile.h"
#include "JsonUtils.h"
#include "json/json.h"
#include "mbedTLS++/BlockingSslClientSocket.h"
#include "mbedTLS++/SslConfig.h"
#include "OSSupport/IsThread.h"
#include "RankManager.h"
#include "Root.h"
#include "SQLiteCpp/Database.h"
#include "SQLiteCpp/Statement.h"




/** The maximum age for items to be kept in the cache. Any item older than this will be removed. */
const Int64 MAX_AGE = 7 * 24 * 60 * 60;  // 7 days ago

/** The maximum number of names to send in a single query */
const int MAX_PER_QUERY = 100;





constexpr char DEFAULT_NAME_TO_UUID_SERVER[]     = "api.mojang.com";
constexpr char DEFAULT_NAME_TO_UUID_ADDRESS[]    = "/profiles/minecraft";
constexpr char DEFAULT_UUID_TO_PROFILE_SERVER[]  = "sessionserver.mojang.com";
constexpr char DEFAULT_UUID_TO_PROFILE_ADDRESS[] = "/session/minecraft/profile/%UUID%?unsigned=false";





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
		if (Prop.get("name", "").asString() != "textures")
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




class cCallbacks : public cUrlClient::cCallbacks
{
public:

	explicit cCallbacks(std::shared_ptr<cEvent> a_Event, AString & a_ResponseBody) : m_Event(std::move(a_Event)), m_ResponseBody(a_ResponseBody) {}

	void OnBodyFinished() override
	{
		m_Event->Set();
	}

	void OnError(const AString & a_ErrorMsg) override
	{
		LOGERROR("%s %d: HTTP Error: %s", __FILE__, __LINE__, a_ErrorMsg.c_str());
		m_Event->Set();
	}

	void OnBodyData(const void * a_Data, size_t a_Size) override
	{
		m_ResponseBody.append(static_cast<const char *>(a_Data), a_Size);
	}

	std::shared_ptr<cEvent> m_Event;

	/** The accumulator for the partial body data, so that OnBodyFinished() can send the entire thing at once. */
	AString & m_ResponseBody;
};





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
		auto itr = a_NamesToQuery.begin();
		auto end = a_NamesToQuery.end();
		for (; (itr != end) && (Count < MAX_PER_QUERY); ++itr, ++Count)
		{
			Json::Value req(*itr);
			root.append(req);
		}  // for itr - a_PlayerNames[]
		a_NamesToQuery.erase(a_NamesToQuery.begin(), itr);
		auto RequestBody = JsonUtils::WriteFastString(root);

		// Create and send the HTTP request
		auto [Success, Response] = cUrlClient::BlockingPost(m_NameToUUIDAddress, AStringMap(), std::move(RequestBody), AStringMap());
		if (!Success)
		{
			continue;
		}
		AString HexDump;

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
	ReplaceURL(Address, "%UUID%", a_UUID.ToShortString());

	// Create and send the HTTP request
	auto [Success, Response] = cUrlClient::BlockingGet(Address);
	if (!Success)
	{
		return;
	}

	// Parse the returned string into Json:
	Json::Value root;
	AString ParseError, HexDump;
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
		for (const auto & UUIDToProfile : m_UUIDToProfile)
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
