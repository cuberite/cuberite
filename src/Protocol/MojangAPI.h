
// MojangAPI.h

// Declares the cMojangAPI class representing the various API points provided by Mojang's webservices, and a cache for their results





#pragma once

#include <time.h>





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
	
	/** Converts the given UUID to its short form (32 bytes, no dashes).
	Logs a warning and returns empty string if not a UUID.
	Note: only checks the string's length, not the actual content. */
	static AString MakeUUIDShort(const AString & a_UUID);
	
	/** Converts the given UUID to its dashed form (36 bytes, 4 dashes).
	Logs a warning and returns empty string if not a UUID.
	Note: only checks the string's length, not the actual content. */
	static AString MakeUUIDDashed(const AString & a_UUID);

	/** Converts a player name into a UUID.
	The UUID will be empty on error.
	If a_UseOnlyCached is true, the function only consults the cached values.
	If a_UseOnlyCached is false and the name is not found in the cache, it is looked up online, which is a blocking
	operation, do not use this in world-tick thread! */
	AString GetUUIDFromPlayerName(const AString & a_PlayerName, bool a_UseOnlyCached = false);
	
	// tolua_end
	
	/** Converts the player names into UUIDs.
	a_PlayerName[idx] will be converted to UUID and returned as idx-th value
	The UUID will be empty on error.
	If a_UseOnlyCached is true, only the cached values are returned.
	If a_UseOnlyCached is false, the names not found in the cache are looked up online, which is a blocking
	operation, do not use this in world-tick thread! */
	AStringVector GetUUIDsFromPlayerNames(const AStringVector & a_PlayerName, bool a_UseOnlyCached = false);
	
	// tolua_begin
	
	/** Called by the Authenticator to add a PlayerName -> UUID mapping that it has received from
	authenticating a user. This adds the cache item and "refreshes" it if existing, adjusting its datetime
	stamp to now. */
	void AddPlayerNameToUUIDMapping(const AString & a_PlayerName, const AString & a_UUID);
	
	// tolua_end

protected:
	struct sUUIDRecord
	{
		AString m_PlayerName;  // Case-correct playername
		AString m_UUID;
		Int64   m_DateTime;  // UNIXtime of the UUID lookup
		
		sUUIDRecord(void) :
			m_UUID(),
			m_DateTime(time(NULL))
		{
		}
		
		sUUIDRecord(const AString & a_PlayerName, const AString & a_UUID, Int64 a_DateTime) :
			m_PlayerName(a_PlayerName),
			m_UUID(a_UUID),
			m_DateTime(a_DateTime)
		{
		}
	};
	typedef std::map<AString, sUUIDRecord> cNameToUUIDMap;  // maps Lowercased PlayerName to sUUIDRecord
	
	/** The server to connect to when converting player names to UUIDs. For example "api.mojang.com". */
	AString m_NameToUUIDServer;
	
	/** The URL to use for converting player names to UUIDs, without server part.
	For example "/profiles/page/1". */
	AString m_NameToUUIDAddress;
	
	/** Cache for the Name-to-UUID lookups. The map key is expected lowercased. Protected by m_CSNameToUUID. */
	cNameToUUIDMap m_NameToUUID;
	
	/** Protects m_NameToUUID against simultaneous multi-threaded access. */
	cCriticalSection m_CSNameToUUID;
	
	
	/** Loads the caches from a disk storage. */
	void LoadCachesFromDisk(void);
	
	/** Saves the caches to a disk storage. */
	void SaveCachesToDisk(void);
	
	/** Makes sure all specified names are in the cache. Downloads any missing ones from Mojang API servers.
	Names that are not valid are not added into the cache.
	ASSUMEs that a_PlayerNames contains lowercased player names. */
	void CacheNamesToUUIDs(const AStringVector & a_PlayerNames);
} ;  // tolua_export




