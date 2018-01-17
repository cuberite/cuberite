
// MojangAPI.h

// Declares the cMojangAPI class representing the various API points provided by Mojang's webservices, and a cache for their results





#pragma once

#include <time.h>

#include "../UUID.h"





// fwd: ../RankManager.h"
class cRankManager;

namespace Json
{
	class Value;
}



class cSettingsRepositoryInterface;

// tolua_begin
class cMojangAPI
{
public:
	// tolua_end

	cMojangAPI(void);
	~cMojangAPI();

	/** Initializes the API; reads the settings from the specified ini file.
	Loads cached results from disk. */
	void Start(cSettingsRepositoryInterface & a_Settings, bool a_ShouldAuth);

	/** Connects to the specified server using SSL, sends the given request and receives the response.
	Checks Mojang certificates using the hard-coded Starfield root CA certificate.
	Returns true if all was successful, false on failure. */
	static bool SecureRequest(const AString & a_ServerName, const AString & a_Request, AString & a_Response);

	/** Converts a player name into a UUID.
	The UUID will be nil on error.
	If a_UseOnlyCached is true, the function only consults the cached values.
	If a_UseOnlyCached is false and the name is not found in the cache, it is looked up online, which is a blocking
	operation, do not use this in world-tick thread!
	If you have multiple names to resolve, use the GetUUIDsFromPlayerNames() function, it uses a single request for multiple names. */
	cUUID GetUUIDFromPlayerName(const AString & a_PlayerName, bool a_UseOnlyCached = false);

	/** Converts a UUID into a playername.
	The returned playername will be empty on error.
	Uses both m_UUIDToName and m_UUIDToProfile to search for the value. Uses m_UUIDToProfile for cache.
	If a_UseOnlyCached is true, the function only consults the cached values.
	If a_UseOnlyCached is false and the name is not found in the cache, it is looked up online, which is a blocking
	operation, do not use this in world-tick thread! */
	AString GetPlayerNameFromUUID(const cUUID & a_UUID, bool a_UseOnlyCached = false);

	/** Converts the player names into UUIDs.
	a_PlayerName[idx] will be converted to UUID and returned as idx-th value
	The UUID will be nil on error.
	If a_UseOnlyCached is true, only the cached values are returned.
	If a_UseOnlyCached is false, the names not found in the cache are looked up online, which is a blocking
	operation, do not use this in world-tick thread! */
	std::vector<cUUID> GetUUIDsFromPlayerNames(const AStringVector & a_PlayerName, bool a_UseOnlyCached = false);

	/** Called by the Authenticator to add a PlayerName -> UUID mapping that it has received from
	authenticating a user. This adds the cache item and "refreshes" it if existing, adjusting its datetime
	stamp to now. */
	void AddPlayerNameToUUIDMapping(const AString & a_PlayerName, const cUUID & a_UUID);

	/** Called by the Authenticator to add a profile that it has received from authenticating a user. Adds
	the profile to the respective mapping caches and updtes their datetime stamp to now. */
	void AddPlayerProfile(const AString & a_PlayerName, const cUUID & a_UUID, const Json::Value & a_Properties);

	/** Sets the m_RankMgr that is used for name-uuid notifications. Accepts nullptr to remove the binding. */
	void SetRankManager(cRankManager * a_RankManager) { m_RankMgr = a_RankManager; }

protected:
	/** The thread that periodically checks for stale data and re-queries it from the server. */
	class cUpdateThread;


	/** Holds data for a single player profile. */
	struct sProfile
	{
		AString m_PlayerName;         // Case-correct playername
		cUUID   m_UUID;               // Player UUID
		AString m_Textures;           // The Textures field of the profile properties
		AString m_TexturesSignature;  // The signature of the Textures field of the profile properties
		Int64   m_DateTime;           // UNIXtime of the profile lookup

		/** Default constructor for the container's sake. */
		sProfile(void) :
			m_PlayerName(),
			m_UUID(),
			m_Textures(),
			m_TexturesSignature(),
			m_DateTime(time(nullptr))
		{
		}

		/** Constructor for the storage creation. */
		sProfile(
			const AString & a_PlayerName,
			const cUUID & a_UUID,
			const AString & a_Textures,
			const AString & a_TexturesSignature,
			Int64 a_DateTime
		) :
			m_PlayerName(a_PlayerName),
			m_UUID(a_UUID),
			m_Textures(a_Textures),
			m_TexturesSignature(a_TexturesSignature),
			m_DateTime(a_DateTime)
		{
		}

		/** Constructor that parses the values from the Json profile. */
		sProfile(
			const AString & a_PlayerName,
			const cUUID & a_UUID,
			const Json::Value & a_Properties,
			Int64 a_DateTime
		);
	};
	typedef std::map<AString, sProfile> cProfileMap;
	typedef std::map<cUUID, sProfile> cUUIDProfileMap;


	/** The server to connect to when converting player names to UUIDs. For example "api.mojang.com". */
	AString m_NameToUUIDServer;

	/** The URL to use for converting player names to UUIDs, without server part.
	For example "/profiles/page/1". */
	AString m_NameToUUIDAddress;

	/** The server to connect to when converting UUID to profile. For example "sessionserver.mojang.com". */
	AString m_UUIDToProfileServer;

	/** The URL to use for converting UUID to profile, without the server part.
	Will replace %UUID% with the actual UUID. For example "session/minecraft/profile/%UUID%?unsigned=false". */
	AString m_UUIDToProfileAddress;

	/** Cache for the Name-to-UUID lookups. The map key is lowercased PlayerName. Protected by m_CSNameToUUID. */
	cProfileMap m_NameToUUID;

	/** Protects m_NameToUUID against simultaneous multi-threaded access. */
	cCriticalSection m_CSNameToUUID;

	/** Cache for the Name-to-UUID lookups. The map key is lowercased short UUID. Protected by m_CSUUIDToName. */
	cUUIDProfileMap m_UUIDToName;

	/** Protects m_UUIDToName against simultaneous multi-threaded access. */
	cCriticalSection m_CSUUIDToName;

	/** Cache for the UUID-to-profile lookups. The map key is lowercased short UUID.
	Protected by m_CSUUIDToProfile. */
	cUUIDProfileMap m_UUIDToProfile;

	/** Protects m_UUIDToProfile against simultaneous multi-threaded access. */
	cCriticalSection m_CSUUIDToProfile;

	/** The rank manager that is notified of the name-uuid pairings. May be nullptr. Protected by m_CSRankMgr. */
	cRankManager * m_RankMgr;

	/** Protects m_RankMgr agains simultaneous multi-threaded access. */
	cCriticalSection m_CSRankMgr;

	/** The thread that periodically updates the stale data in the DB from the Mojang servers. */
	std::shared_ptr<cUpdateThread> m_UpdateThread;


	/** Loads the caches from a disk storage. */
	void LoadCachesFromDisk(void);

	/** Saves the caches to a disk storage. */
	void SaveCachesToDisk(void);

	/** Makes sure all specified names are in the m_PlayerNameToUUID cache. Downloads any missing ones from Mojang API servers.
	Names that are not valid are not added into the cache.
	ASSUMEs that a_PlayerNames contains lowercased player names. */
	void CacheNamesToUUIDs(const AStringVector & a_PlayerNames);

	/** Queries all the specified names and stores them into the m_PlayerNameToUUID cache.
	Names that are not valid are not added into the cache.
	ASSUMEs that a_PlayerNames contans lowercased player names.
	For performance reasons takes a non-const reference and modifies the list given to it, until empty. */
	void QueryNamesToUUIDs(AStringVector & a_PlayerNames);

	/** Makes sure the specified UUID is in the m_UUIDToProfile cache. If missing, downloads it from Mojang API servers.
	UUIDs that are not valid will not be added into the cache. */
	void CacheUUIDToProfile(const cUUID & a_UUID);

	/** Queries the specified UUID's profile and stores it in the m_UUIDToProfile cache. If already present, updates the cache entry.
	UUIDs that are not valid will not be added into the cache. */
	void QueryUUIDToProfile(const cUUID & a_UUID);

	/** Called for each name-uuid pairing that is discovered.
	If assigned, notifies the m_RankManager of the event. */
	void NotifyNameUUID(const AString & a_PlayerName, const cUUID & a_PlayerUUID);

	/** Updates the stale values in the DB from the Mojang servers. Called from the cUpdateThread, blocks on the HTTPS API calls. */
	void Update(void);
} ;  // tolua_export




