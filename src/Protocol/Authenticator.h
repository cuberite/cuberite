
// cAuthenticator.h

// Interfaces to the cAuthenticator class representing the thread that authenticates users against the official MC server
// Authentication prevents "hackers" from joining with an arbitrary username (possibly impersonating the server admins)
// For more info, see http://wiki.vg/Session#Server_operation
// In MCS, authentication is implemented as a single thread that receives queued auth requests and dispatches them one by one.





#pragma once
#ifndef CAUTHENTICATOR_H_INCLUDED
#define CAUTHENTICATOR_H_INCLUDED

#include "../OSSupport/IsThread.h"





// fwd: "cRoot.h"
class cRoot;

namespace Json
{
	class Value;
}





class cAuthenticator :
	public cIsThread
{
	typedef cIsThread super;

public:
	cAuthenticator(void);
	~cAuthenticator();

	/** (Re-)read server and address from INI: */
	void ReadINI(cIniFile & IniFile);

	/** Queues a request for authenticating a user. If the auth fails, the user will be kicked */
	void Authenticate(int a_ClientID, const AString & a_UserName, const AString & a_ServerHash);

	/** Starts the authenticator thread. The thread may be started and stopped repeatedly */
	void Start(cIniFile & IniFile);

	/** Stops the authenticator thread. The thread may be started and stopped repeatedly */
	void Stop(void);
	
	/** Converts the player names into UUIDs.
	a_PlayerName[idx] will be converted to UUID and returned as idx-th value
	The UUID will be empty on error.
	Blocking operation, do not use in world-tick thread! */
	AStringVector GetUUIDsFromPlayerNames(const AStringVector & a_PlayerName);

private:

	class cUser
	{
	public:
		int     m_ClientID;
		AString m_Name;
		AString m_ServerID;

		cUser(int a_ClientID, const AString & a_Name, const AString & a_ServerID) :
			m_ClientID(a_ClientID),
			m_Name(a_Name),
			m_ServerID(a_ServerID)
		{
		}
	};

	typedef std::deque<cUser> cUserList;

	cCriticalSection m_CS;
	cUserList        m_Queue;
	cEvent           m_QueueNonempty;

	/** The server that is to be contacted for auth / UUID conversions */
	AString m_Server;
	
	/** The URL to use for auth, without server part.
	%USERNAME% will be replaced with actual user name.
	%SERVERID% will be replaced with server's ID.
	For example "/session/minecraft/hasJoined?username=%USERNAME%&serverId=%SERVERID%". */
	AString m_Address;
	
	/** The server to connect to when converting player names to UUIDs. For example "api.mojang.com". */
	AString m_NameToUUIDServer;
	
	/** The URL to use for converting player names to UUIDs, without server part.
	For example "/profiles/page/1". */
	AString m_NameToUUIDAddress;
	
	AString m_PropertiesAddress;
	bool    m_ShouldAuthenticate;

	/** cIsThread override: */
	virtual void Execute(void) override;

	/** Connects to a hostname using SSL, sends given data, and sets the response, returning whether all was successful or not */
	bool SecureGetFromAddress(const AString & a_CACerts, const AString & a_ExpectedPeerName, const AString & a_Request, AString & a_Response);

	/** Returns true if the user authenticated okay, false on error
	Sets the username, UUID, and properties (i.e. skin) fields
	*/
	bool AuthWithYggdrasil(AString & a_UserName, const AString & a_ServerId, AString & a_UUID, Json::Value & a_Properties);
};





#endif  // CAUTHENTICATOR_H_INCLUDED




