
// MojangAPI.h

// Declares the cMojangAPI class representing the various API points provided by Mojang's webservices, and a cache for their results





#pragma once

#include <time.h>

namespace Json
{
	class Value;
}





// tolua_begin
class cMojangAPI
{
public:
	// tolua_end
	
	cMojangAPI(void);
	~cMojangAPI();
	
	/** Initializes the API; reads the settings from the specified ini file.
	Loads cached results from disk. */
	void Start(cIniFile & a_SettingsIni);
	
	/** Connects to the specified server using SSL, sends the given request and receives the response.
	Checks Mojang certificates using the hard-coded Starfield root CA certificate.
	Returns true if all was successful, false on failure. */
	static bool SecureRequest(const AString & a_ServerName, const AString & a_Request, AString & a_Response);
	
	// tolua_begin
	
	/** Normalizes the given UUID to its short form (32 bytes, no dashes, lowercase).
	Logs a warning and returns empty string if not a UUID.
	Note: only checks the string's length, not the actual content. */
	static AString MakeUUIDShort(const AString & a_UUID);
	
	/** Normalizes the given UUID to its dashed form (36 bytes, 4 dashes, lowercase).
	Logs a warning and returns empty string if not a UUID.
	Note: only checks the string's length, not the actual content. */
	static AString MakeUUIDDashed(const AString & a_UUID);

	// tolua_end
	
	/** Converts a player name into a UUID.
	The UUID will be empty on error.
	If a_UseOnlyCached is true, the function only consults the cached values.
	If a_UseOnlyCached is false and the name is not found in the cache, it is looked up online, which is a blocking
	operation, do not use this in world-tick thread!
	If you have multiple names to resolve, use the GetUUIDsFromPlayerNames() function, it uses a single request for multiple names. */
	AString GetUUIDFromPlayerName(const AString & a_PlayerName, bool a_UseOnlyCached = false);
	
	/** Converts a UUID into a playername.
	The returned playername will be empty on error.
	Both short and dashed UUID formats are accepted.
	Uses both m_UUIDToName and m_UUIDToProfile to search for the value. Uses m_UUIDToProfile for cache.
	If a_UseOnlyCached is true, the function only consults the cached values.
	If a_UseOnlyCached is false and the name is not found in the cache, it is looked up online, which is a blocking
	operation, do not use this in world-tick thread! */
	AString GetPlayerNameFromUUID(const AString & a_UUID, bool a_UseOnlyCached = false);
	
	/** Converts the player names into UUIDs.
	a_PlayerName[idx] will be converted to UUID and returned as idx-th value
	The UUID will be empty on error.
	If a_UseOnlyCached is true, only the cached values are returned.
	If a_UseOnlyCached is false, the names not found in the cache are looked up online, which is a blocking
	operation, do not use this in world-tick thread! */
	AStringVector GetUUIDsFromPlayerNames(const AStringVector & a_PlayerName, bool a_UseOnlyCached = false);
	
	/** Called by the Authenticator to add a PlayerName -> UUID mapping that it has received from
	authenticating a user. This adds the cache item and "refreshes" it if existing, adjusting its datetime
	stamp to now. */
	void AddPlayerNameToUUIDMapping(const AString & a_PlayerName, const AString & a_UUID);
	
	/** Called by the Authenticator to add a profile that it has received from authenticating a user. Adds
	the profile to the respective mapping caches and updtes their datetime stamp to now. */
	void AddPlayerProfile(const AString & a_PlayerName, const AString & a_UUID, const Json::Value & a_Properties);
	
protected:
	/** Holds data for a single player profile. */
	struct sProfile
	{
		AString m_PlayerName;         // Case-correct playername
		AString m_UUID;               // Short lowercased UUID
		AString m_Textures;           // The Textures field of the profile properties
		AString m_TexturesSignature;  // The signature of the Textures field of the profile properties
		Int64   m_DateTime;           // UNIXtime of the profile lookup
		
		/** Default constructor for the container's sake. */
		sProfile(void) :
			m_PlayerName(),
			m_UUID(),
			m_Textures(),
			m_TexturesSignature(),
			m_DateTime(time(NULL))
		{
		}
		
		/** Constructor for the storage creation. */
		sProfile(
			const AString & a_PlayerName,
			const AString & a_UUID,
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
			const AString & a_UUID,
			const Json::Value & a_Properties,
			Int64 a_DateTime
		);
	};
	typedef std::map<AString, sProfile> cProfileMap;

	
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
	cProfileMap m_UUIDToName;
	
	/** Protects m_UUIDToName against simultaneous multi-threaded access. */
	cCriticalSection m_CSUUIDToName;
	
	/** Cache for the UUID-to-profile lookups. The map key is lowercased short UUID.
	Protected by m_CSUUIDToProfile. */
	cProfileMap m_UUIDToProfile;
	
	/** Protects m_UUIDToProfile against simultaneous multi-threaded access. */
	cCriticalSection m_CSUUIDToProfile;
	
	
	/** Loads the caches from a disk storage. */
	void LoadCachesFromDisk(void);
	
	/** Saves the caches to a disk storage. */
	void SaveCachesToDisk(void);
	
	/** Makes sure all specified names are in the m_PlayerNameToUUID cache. Downloads any missing ones from Mojang API servers.
	Names that are not valid are not added into the cache.
	ASSUMEs that a_PlayerNames contains lowercased player names. */
	void CacheNamesToUUIDs(const AStringVector & a_PlayerNames);
	
	/** Makes sure the specified UUID is in the m_UUIDToProfile cache. If missing, downloads it from Mojang API servers.
	UUIDs that are not valid will not be added into the cache.
	ASSUMEs that a_UUID is a lowercased short UUID. */
	void CacheUUIDToProfile(const AString & a_UUID);
} ;  // tolua_export




