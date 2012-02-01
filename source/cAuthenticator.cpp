
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "cAuthenticator.h"
#include "cBlockingTCPLink.h"
#include "cRoot.h"
#include "cServer.h"

#include "../iniFile/iniFile.h"

#include <sstream>





#define DEFAULT_AUTH_SERVER "session.minecraft.net"
#define DEFAULT_AUTH_ADDRESS "/game/checkserver.jsp?user=%USERNAME%&serverId=%SERVERID%"
#define MAX_REDIRECTS 10





cAuthenticator::cAuthenticator(void) :
	super("cAuthenticator"),
	mServer(DEFAULT_AUTH_SERVER),
	mAddress(DEFAULT_AUTH_ADDRESS),
	mShouldAuthenticate(true)
{
	ReadINI();
}





/// Read custom values from INI
void cAuthenticator::ReadINI(void)
{
	cIniFile IniFile("settings.ini");
	if (!IniFile.ReadFile())
	{
		return;
	}
	
	mServer  = IniFile.GetValue("Authentication", "Server");
	mAddress = IniFile.GetValue("Authentication", "Address");
	mShouldAuthenticate = IniFile.GetValueB("Authentication", "Authenticate", true);
	bool bSave = false;
	
	if (mServer.length() == 0)
	{
		mServer = DEFAULT_AUTH_SERVER;
		IniFile.SetValue("Authentication", "Server", mServer);
		bSave = true;
	}
	if (mAddress.length() == 0)
	{
		mAddress = DEFAULT_AUTH_ADDRESS;
		IniFile.SetValue("Authentication", "Address", mAddress);
		bSave = true;
	}

	if (bSave)
	{
		IniFile.SetValueB("Authentication", "Authenticate", mShouldAuthenticate);
		IniFile.WriteFile();
	}
}





/// Queues a request for authenticating a user. If the auth fails, the user is kicked
void cAuthenticator::Authenticate(const AString & iUserName, const AString & iServerID)
{
	if (!mShouldAuthenticate)
	{
		cRoot::Get()->AuthenticateUser(iUserName);
		return;
	}

	cCSLock Lock(mCS);
	mQueue.push_back(cUser(iUserName, iServerID));
	mQueueNonempty.Set();
}





void cAuthenticator::Execute(void)
{
	while (true)
	{
		cCSLock Lock(mCS);
		while (!mShouldTerminate && (mQueue.size() == 0))
		{
			cCSUnlock Unlock(Lock);
			mQueueNonempty.Wait();
		}
		if (mShouldTerminate)
		{
			return;
		}
		assert(mQueue.size() > 0);
		
		AString UserName = mQueue.front().mName;
		AString ActualAddress = mAddress;
		ReplaceString(ActualAddress, "%USERNAME%", UserName);
		ReplaceString(ActualAddress, "%SERVERID%", cRoot::Get()->GetServer()->GetServerID());
		mQueue.pop_front();
		Lock.Unlock();

		if (!AuthFromAddress(mServer, ActualAddress, UserName))
		{
			cRoot::Get()->KickUser(UserName, "auth failed");
		}
		else
		{
			cRoot::Get()->AuthenticateUser(UserName);
		}
	}
}





bool cAuthenticator::AuthFromAddress(const AString & iServer, const AString & iAddress, const AString & iUserName, int iLevel)
{
	// Returns true if the user authenticated okay, false on error; iLevel is the recursion deptht (bails out if too deep)

	cBlockingTCPLink Link;
	if (!Link.Connect(iServer.c_str(), 80))
	{
		LOGERROR("cAuthenticator: cannot connect to auth server \"%s\", kicking user \"%s\"", iServer.c_str(), iUserName.c_str());
		return false;
	}
	
	Link.SendMessage( AString( "GET " + iAddress + " HTTP/1.0\r\n\r\n" ).c_str());
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
			LOGINFO("Need to redirect!");
			if (iLevel > MAX_REDIRECTS)
			{
				LOGERROR("cAuthenticator: received too many levels of redirection from auth server \"%s\" for user \"%s\", bailing out and kicking the user", iServer.c_str(), iUserName.c_str());
				return false;
			}
			bRedirect = true;
		}
		else if (code == 200)
		{
			LOGINFO("Got 200 OK :D");
			bOK = true;
		}
	}
	else
	{
		LOGERROR("cAuthenticator: cannot parse auth reply from server \"%s\" for user \"%s\", kicking the user.", iServer.c_str(), iUserName.c_str());
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
			LOGERROR("cAuthenticator: received invalid redirection from auth server \"%s\" for user \"%s\", kicking user.", iServer.c_str(), iUserName.c_str());
			return false;
		}

		Location = Location.substr(strlen("http://"), std::string::npos); // Strip http://
		std::string Server = Location.substr( 0, Location.find( "/" ) ); // Only leave server address
		Location = Location.substr( Server.length(), std::string::npos);
		return AuthFromAddress(Server, Location, iUserName, iLevel + 1);
	}

	if (!bOK)
	{
		LOGERROR("cAuthenticator: received an error from auth server \"%s\" for user \"%s\", kicking user.", iServer.c_str(), iUserName.c_str());
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
		LOGERROR("cAuthenticator: error while parsing response body from auth server \"%s\" for user \"%s\", kicking user.", iServer.c_str(), iUserName.c_str());
		return false;
	}

	std::string Result;
	ss >> Result;
	LOGINFO("Got result: %s", Result.c_str());
	if (Result.compare("YES") == 0)
	{
		LOGINFO("Result was \"YES\", so player is authenticated!");
		return true;
	}
	LOGINFO("Result was \"%s\", so player is NOT authenticated!", Result.c_str());
	return false;
}




