
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Authenticator.h"
#include "OSSupport/BlockingTCPLink.h"
#include "Root.h"
#include "Server.h"

#include "inifile/iniFile.h"

#include <sstream>





#define DEFAULT_AUTH_SERVER "session.minecraft.net"
#define DEFAULT_AUTH_ADDRESS "/game/checkserver.jsp?user=%USERNAME%&serverId=%SERVERID%"
#define MAX_REDIRECTS 10





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
	m_Server  = IniFile.GetValueSet("Authentication", "Server", DEFAULT_AUTH_SERVER);
	m_Address = IniFile.GetValueSet("Authentication", "Address", DEFAULT_AUTH_ADDRESS);
	m_ShouldAuthenticate = IniFile.GetValueSetB("Authentication", "Authenticate", true);
}





void cAuthenticator::Authenticate(int a_ClientID, const AString & a_UserName, const AString & a_ServerHash)
{
	if (!m_ShouldAuthenticate)
	{
		cRoot::Get()->AuthenticateUser(a_ClientID);
		return;
	}

	cCSLock Lock(m_CS);
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
		
		int ClientID = m_Queue.front().m_ClientID;
		AString UserName = m_Queue.front().m_Name;
		AString ActualAddress = m_Address;
		ReplaceString(ActualAddress, "%USERNAME%", UserName);
		ReplaceString(ActualAddress, "%SERVERID%", m_Queue.front().m_ServerID);
		m_Queue.pop_front();
		Lock.Unlock();

		if (!AuthFromAddress(m_Server, ActualAddress, UserName))
		{
			cRoot::Get()->KickUser(ClientID, "Failed to authenticate account!");
		}
		else
		{
			cRoot::Get()->AuthenticateUser(ClientID);
		}
	}  // for (-ever)
}





bool cAuthenticator::AuthFromAddress(const AString & a_Server, const AString & a_Address, const AString & a_UserName, int a_Level /* = 1 */)
{
	// Returns true if the user authenticated okay, false on error; iLevel is the recursion deptht (bails out if too deep)

	cBlockingTCPLink Link;
	if (!Link.Connect(a_Server.c_str(), 80))
	{
		LOGWARNING("%s: cannot connect to auth server \"%s\", kicking user \"%s\"",
			__FUNCTION__, a_Server.c_str(), a_UserName.c_str()
		);
		return false;
	}
	
	Link.SendMessage( AString( "GET " + a_Address + " HTTP/1.1\r\n" ).c_str());
	Link.SendMessage( AString( "User-Agent: MCServer\r\n" ).c_str());
	Link.SendMessage( AString( "Host: " + a_Server + "\r\n" ).c_str());
	//Link.SendMessage( AString( "Host: session.minecraft.net\r\n" ).c_str());
	Link.SendMessage( AString( "Accept: */*\r\n" ).c_str());
	Link.SendMessage( AString( "Connection: close\r\n" ).c_str());	//Close so we don´t have to mess with the Content-Length :)
	Link.SendMessage( AString( "\r\n" ).c_str());
	AString DataRecvd;
	Link.ReceiveData(DataRecvd);
	Link.CloseSocket();

	std::stringstream ss(DataRecvd);

	// Parse the data received:
	std::string temp;
	ss >> temp;
	bool bRedirect = false;
	bool bOK = false;
	if ((temp.compare("HTTP/1.1") == 0) || (temp.compare("HTTP/1.0") == 0))
	{
		int code;
		ss >> code;
		if (code == 302)
		{
			// redirect blabla
			LOGD("%s: Need to redirect, current level %d!", __FUNCTION__, a_Level);
			if (a_Level > MAX_REDIRECTS)
			{
				LOGERROR("cAuthenticator: received too many levels of redirection from auth server \"%s\" for user \"%s\", bailing out and kicking the user", a_Server.c_str(), a_UserName.c_str());
				return false;
			}
			bRedirect = true;
		}
		else if (code == 200)
		{
			LOGD("cAuthenticator: Received status 200 OK! :D");
			bOK = true;
		}
	}
	else
	{
		LOGERROR("cAuthenticator: cannot parse auth reply from server \"%s\" for user \"%s\", kicking the user.", a_Server.c_str(), a_UserName.c_str());
		return false;
	}

	if( bRedirect )
	{
		AString Location;
		// Search for "Location:"
		bool bFoundLocation = false;
		while( !bFoundLocation && ss.good() )
		{
			char c = 0;
			while( c != '\n' )
			{
				ss.get( c );
			}
			AString Name;
			ss >> Name;
			if (Name.compare("Location:") == 0)
			{
				bFoundLocation = true;
				ss >> Location;
			}
		}
		if (!bFoundLocation)
		{
			LOGERROR("cAuthenticator: received invalid redirection from auth server \"%s\" for user \"%s\", kicking user.", a_Server.c_str(), a_UserName.c_str());
			return false;
		}

		Location = Location.substr(strlen("http://"), std::string::npos); // Strip http://
		std::string Server = Location.substr( 0, Location.find( "/" ) ); // Only leave server address
		Location = Location.substr( Server.length(), std::string::npos);
		return AuthFromAddress(Server, Location, a_UserName, a_Level + 1);
	}

	if (!bOK)
	{
		LOGERROR("cAuthenticator: received an error from auth server \"%s\" for user \"%s\", kicking user.", a_Server.c_str(), a_UserName.c_str());
		return false;
	}

	// Header says OK, so receive the rest.
	// Go past header, double \n means end of headers
	char c = 0;
	while (ss.good())
	{
		while (c != '\n')
		{
			ss.get(c);
		}
		ss.get(c);
		if( c == '\n' || c == '\r' || ss.peek() == '\r' || ss.peek() == '\n' )
			break;
	}
	if (!ss.good())
	{
		LOGERROR("cAuthenticator: error while parsing response body from auth server \"%s\" for user \"%s\", kicking user.", a_Server.c_str(), a_UserName.c_str());
		return false;
	}

	std::string Result;
	ss >> Result;
	LOGD("cAuthenticator: Authentication result was %s", Result.c_str());

	if (Result.compare("YES") == 0)	//Works well
	{
		LOGINFO("Authentication result \"YES\", player authentication success!");
		return true;
	}


	LOGINFO("Authentication result was \"%s\", player authentication failure!", Result.c_str());
	return false;
}




