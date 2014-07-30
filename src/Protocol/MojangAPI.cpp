
// MojangAPI.cpp

// Implements the cMojangAPI class representing the various API points provided by Mojang's webservices, and a cache for their results

#include "Globals.h"
#include "MojangAPI.h"
#include "SQLiteCpp/Database.h"
#include "SQLiteCpp/Statement.h"
#include "inifile/iniFile.h"
#include "json/json.h"
#include "PolarSSL++/BlockingSslClientSocket.h"





/** The maximum age for items to be kept in the cache. Any item older than this will be removed. */
const Int64 MAX_AGE = 7 * 24 * 60 * 60;  // 7 days ago

/** The maximum number of names to send in a single query */
const int MAX_PER_QUERY = 100;





#define DEFAULT_NAME_TO_UUID_SERVER "api.mojang.com"
#define DEFAULT_NAME_TO_UUID_ADDRESS "/profiles/minecraft"






/** This is the data of the root certs for Starfield Technologies, the CA that signed sessionserver.mojang.com's cert:
Downloaded from http://certs.starfieldtech.com/repository/ */
static const AString & StarfieldCACert(void)
{
	static const AString Cert(
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
	
	return Cert;
}





////////////////////////////////////////////////////////////////////////////////
// cMojangAPI:

cMojangAPI::cMojangAPI(void) :
	m_NameToUUIDServer(DEFAULT_NAME_TO_UUID_SERVER),
	m_NameToUUIDAddress(DEFAULT_NAME_TO_UUID_ADDRESS)
{
}





cMojangAPI::~cMojangAPI()
{
	SaveCachesToDisk();
}





void cMojangAPI::Start(cIniFile & a_SettingsIni)
{
	m_NameToUUIDServer   = a_SettingsIni.GetValueSet("Authentication", "NameToUUIDServer",  DEFAULT_NAME_TO_UUID_SERVER);
	m_NameToUUIDAddress  = a_SettingsIni.GetValueSet("Authentication", "NameToUUIDAddress", DEFAULT_NAME_TO_UUID_ADDRESS);
	LoadCachesFromDisk();
}





AStringVector cMojangAPI::GetUUIDsFromPlayerNames(const AStringVector & a_PlayerNames)
{
	// Convert all playernames to lowercase:
	AStringVector PlayerNames;
	for (AStringVector::const_iterator itr = a_PlayerNames.begin(), end = a_PlayerNames.end(); itr != end; ++itr)
	{
		AString Lower(*itr);
		PlayerNames.push_back(StrToLower(Lower));
	}  // for itr - a_PlayerNames[]
	
	// Request the cache to populate any names not yet contained:
	CacheNamesToUUIDs(PlayerNames);
	
	// Retrieve from cache:
	size_t idx = 0;
	AStringVector res;
	res.resize(PlayerNames.size());
	cCSLock Lock(m_CSNameToUUID);
	for (AStringVector::const_iterator itr = PlayerNames.begin(), end = PlayerNames.end(); itr != end; ++itr, ++idx)
	{
		cNameToUUIDMap::const_iterator itrN = m_NameToUUID.find(*itr);
		if (itrN != m_NameToUUID.end())
		{
			res[idx] = itrN->second.m_UUID;
		}
	}  // for itr - PlayerNames[]
	return res;
}



	

void cMojangAPI::AddPlayerNameToUUIDMapping(const AString & a_PlayerName, const AString & a_UUID)
{
	AString lcName(a_PlayerName);
	AString UUID = MakeUUIDShort(a_UUID);
	Int64 Now = time(NULL);
	cCSLock Lock(m_CSNameToUUID);
	m_NameToUUID[StrToLower(lcName)] = sUUIDRecord(a_PlayerName, UUID, Now);
}





bool cMojangAPI::SecureRequest(const AString & a_ServerName, const AString & a_Request, AString & a_Response)
{
	// Connect the socket:
	cBlockingSslClientSocket Socket;
	Socket.SetTrustedRootCertsFromString(StarfieldCACert(), a_ServerName);
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
	int ret;
	unsigned char buf[1024];

	for (;;)
	{
		ret = Socket.Receive(buf, sizeof(buf));

		if ((ret == POLARSSL_ERR_NET_WANT_READ) || (ret == POLARSSL_ERR_NET_WANT_WRITE))
		{
			// This value should never be returned, it is handled internally by cBlockingSslClientSocket
			LOGWARNING("%s: SSL reading failed internally", __FUNCTION__);
			return false;
		}
		if (ret == POLARSSL_ERR_SSL_PEER_CLOSE_NOTIFY)
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

		a_Response.append((const char *)buf, (size_t)ret);
	}

	Socket.Disconnect();
	return true;
}





AString cMojangAPI::MakeUUIDShort(const AString & a_UUID)
{
	// Note: we only check the string's length, not the actual content
	switch (a_UUID.size())
	{
		case 32:
		{
			// Already is a short UUID
			return a_UUID;
		}
		
		case 36: 
		{
			// Remove the dashes from the string:
			AString res;
			res.reserve(32);
			res.append(a_UUID, 0, 8);
			res.append(a_UUID, 9, 4);
			res.append(a_UUID, 14, 4);
			res.append(a_UUID, 19, 4);
			res.append(a_UUID, 24, 12);
			return res;
		}
	}
	LOGWARNING("%s: Not an UUID: \"%s\".", __FUNCTION__, a_UUID.c_str());
	return "";
}





AString cMojangAPI::MakeUUIDDashed(const AString & a_UUID)
{
	// Note: we only check the string's length, not the actual content
	switch (a_UUID.size())
	{
		case 36:
		{
			// Already is a dashed UUID
			return a_UUID;
		}
		
		case 32: 
		{
			// Insert dashes at the proper positions:
			AString res;
			res.reserve(36);
			res.append(a_UUID, 0, 8);
			res.push_back('-');
			res.append(a_UUID, 8, 4);
			res.push_back('-');
			res.append(a_UUID, 12, 4);
			res.push_back('-');
			res.append(a_UUID, 16, 4);
			res.push_back('-');
			res.append(a_UUID, 20, 12);
			return res;
		}
	}
	LOGWARNING("%s: Not an UUID: \"%s\".", __FUNCTION__, a_UUID.c_str());
	return "";
}





void cMojangAPI::LoadCachesFromDisk(void)
{
	try
	{
		// Open up the SQLite DB:
		SQLite::Database db("NameToUUID.sqlite", SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE);
		db.exec("CREATE TABLE IF NOT EXISTS PlayerNameToUUID (PlayerName, UUID, DateTime)");
		
		// Clean up old entries:
		{
			SQLite::Statement stmt(db, "DELETE FROM PlayerNameToUUID WHERE DateTime < ?");
			Int64 LimitDateTime = time(NULL) - MAX_AGE;
			stmt.bind(1, LimitDateTime);
			stmt.exec();
		}
		
		// Retrieve all remaining entries::
		SQLite::Statement stmt(db, "SELECT PlayerName, UUID, DateTime FROM PlayerNameToUUID");
		while (stmt.executeStep())
		{
			AString PlayerName = stmt.getColumn(0);
			AString UUID       = stmt.getColumn(1);
			Int64 DateTime     = stmt.getColumn(2);
			AString lcPlayerName = PlayerName;
			m_NameToUUID[StrToLower(lcPlayerName)] = sUUIDRecord(PlayerName, UUID, DateTime);
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
		SQLite::Database db("NameToUUID.sqlite", SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE);
		db.exec("CREATE TABLE IF NOT EXISTS PlayerNameToUUID (PlayerName, UUID, DateTime)");
		
		// Remove all entries:
		db.exec("DELETE FROM PlayerNameToUUID");
		
		// Save all cache entries:
		SQLite::Statement stmt(db, "INSERT INTO PlayerNameToUUID(PlayerName, UUID, DateTime) VALUES (?, ?, ?)");
		Int64 LimitDateTime = time(NULL) - MAX_AGE;
		cCSLock Lock(m_CSNameToUUID);
		for (cNameToUUIDMap::const_iterator itr = m_NameToUUID.begin(), end = m_NameToUUID.end(); itr != end; ++itr)
		{
			if (itr->second.m_DateTime < LimitDateTime)
			{
				// This item is too old, do not save
				continue;
			}
			stmt.bind(1, itr->second.m_PlayerName);
			stmt.bind(2, itr->second.m_UUID);
			stmt.bind(3, itr->second.m_DateTime);
			stmt.exec();
			stmt.reset();
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
	
	while (!NamesToQuery.empty())
	{
		// Create the request body - a JSON containing up to MAX_PER_QUERY playernames:
		Json::Value root;
		int Count = 0;
		AStringVector::iterator itr = NamesToQuery.begin(), end = NamesToQuery.end();
		for (; (itr != end) && (Count < MAX_PER_QUERY); ++itr, ++Count)
		{
			Json::Value req(*itr);
			root.append(req);
		}  // for itr - a_PlayerNames[]
		NamesToQuery.erase(NamesToQuery.begin(), itr);
		Json::FastWriter Writer;
		AString RequestBody = Writer.write(root);
	
		// Create the HTTP request:
		AString Request;
		Request += "POST " + m_NameToUUIDAddress + " HTTP/1.0\r\n";  // We need to use HTTP 1.0 because we don't handle Chunked transfer encoding
		Request += "Host: " + m_NameToUUIDServer + "\r\n";
		Request += "User-Agent: MCServer\r\n";
		Request += "Connection: close\r\n";
		Request += "Content-Type: application/json\r\n";
		Request += Printf("Content-Length: %u\r\n", (unsigned)RequestBody.length());
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
		Json::Reader reader;
		if (!reader.parse(Response, root, false) || !root.isArray())
		{
			LOGWARNING("%s failed: Cannot parse received data (NameToUUID) to JSON!", __FUNCTION__);
			LOGD("Response body:\n%s", CreateHexDump(HexDump, Response.data(), Response.size(), 16).c_str());
			continue;
		}
	
		// Store the returned results into cache:
		size_t JsonCount = root.size();
		Int64 Now = time(NULL);
		cCSLock Lock(m_CSNameToUUID);
		for (size_t idx = 0; idx < JsonCount; ++idx)
		{
			Json::Value & Val = root[idx];
			AString JsonName = Val.get("name", "").asString();
			AString JsonUUID = MakeUUIDShort(Val.get("id", "").asString());
			if (JsonUUID.empty())
			{
				continue;
			}
			AString lcName = JsonName;
			m_NameToUUID[StrToLower(lcName)] = sUUIDRecord(JsonName, JsonUUID, Now);
		}  // for idx - root[]
	}  // while (!NamesToQuery.empty())
}




