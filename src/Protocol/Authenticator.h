
// cAuthenticator.h

// Interfaces to the cAuthenticator class representing the thread that authenticates users against the official Mojang servers
// Authentication prevents "hackers" from joining with an arbitrary username (possibly impersonating the server admins)
// For more info, see http://wiki.vg/Session
// In Cuberite, authentication is implemented as a single thread that receives queued auth requests and dispatches them one by one.





#pragma once

#include "../OSSupport/IsThread.h"

// fwd:
class cUUID;
class cSettingsRepositoryInterface;

namespace Json
{
	class Value;
}





class cAuthenticator:
	public cIsThread
{
	using Super = cIsThread;

public:

	cAuthenticator();
	virtual ~cAuthenticator() override;

	/** (Re-)read server and address from INI: */
	void ReadSettings(cSettingsRepositoryInterface & a_Settings);

	/** Queues a request for authenticating a user. If the auth fails, the user will be kicked */
	void Authenticate(int a_ClientID, const AString & a_UserName, const AString & a_ServerHash);

	/** Starts the authenticator thread. The thread may be started and stopped repeatedly */
	void Start(cSettingsRepositoryInterface & a_Settings);

	/** Stops the authenticator thread. The thread may be started and stopped repeatedly */
	void Stop(void);

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

	AString m_PropertiesAddress;
	bool    m_ShouldAuthenticate;

	/** cIsThread override: */
	virtual void Execute(void) override;

	/** Returns true if the user authenticated okay, false on error
	Returns the case-corrected username, UUID, and properties (eg. skin). */
	bool AuthWithYggdrasil(AString & a_UserName, const AString & a_ServerId, cUUID & a_UUID, Json::Value & a_Properties);
};




