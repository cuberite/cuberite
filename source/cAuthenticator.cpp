#include "cAuthenticator.h"
#include "cBlockingTCPLink.h"
#include "cMCLogger.h"

#include "../iniFile/iniFile.h"
#ifndef _WIN32
#include <cstring>
#endif

#include <string>
#include <sstream>

extern void ReplaceString( std::string & a_HayStack, const std::string & a_Needle, const std::string & a_ReplaceWith );

cAuthenticator::cAuthenticator()
{
}

cAuthenticator::~cAuthenticator()
{
}

bool cAuthenticator::Authenticate( const char* a_PlayerName, const char* a_ServerID )
{
	// Default values
	std::string Server = "www.minecraft.net";
	std::string Address = "/game/checkserver.jsp?user=%USERNAME%&serverId=%SERVERID%";
	bool bAuthenticate = true;

	// Read custom values from INI
	cIniFile IniFile("settings.ini");
	if( IniFile.ReadFile() )
	{
		std::string tServer = IniFile.GetValue("Authentication", "Server");
		std::string tAddress = IniFile.GetValue("Authentication", "Address");
		bAuthenticate = IniFile.GetValueB("Authentication", "Authenticate", true);
		bool bSave = false;
		if( tServer.length() == 0 )
		{
			IniFile.SetValue("Authentication", "Server", Server, true );
			bSave = true;
		}
		else
			Server = tServer;
		if( tAddress.length() == 0 )
		{
			IniFile.SetValue("Authentication", "Address", Address, true );
			bSave = true;
		}
		else
			Address = tAddress;

		if( bSave )
		{
			IniFile.SetValueB("Authentication", "Authenticate", bAuthenticate, true );
			IniFile.WriteFile();
		}
	}

	if( !bAuthenticate ) // If we don't want to authenticate.. just return true
	{
		return true;
	}

	ReplaceString( Address, "%USERNAME%", a_PlayerName );
	ReplaceString( Address, "%SERVERID%", a_ServerID );


	cBlockingTCPLink TCPLink;
	if( TCPLink.Connect( Server.c_str(), 80 ) )
	{
		//TCPLink.SendMessage( std::string( "GET /game/checkserver.jsp?user=" + std::string(a_PlayerName) + "&serverId=" + std::string(a_ServerID) + " HTTP/1.0\r\n\r\n" ).c_str() );
		TCPLink.SendMessage( std::string( "GET " + Address + " HTTP/1.0\r\n\r\n" ).c_str() );
		//LOGINFO("Successfully connected to mc.net");
		std::string Received = TCPLink.ReceiveData();
		//LOGINFO("Received data: %s", Received.c_str() );
		return ParseReceived( Received.c_str(), &TCPLink );
	}
	else
	{
		LOGERROR("Could not connect to %s to verify player name! (%s)", Server.c_str(), a_PlayerName );
		return false;
	}
}

bool cAuthenticator::ParseReceived( const char* a_Data, cBlockingTCPLink* a_TCPLink )
{
	std::stringstream ss(a_Data);

	std::string temp;
	ss >> temp;
	//LOGINFO("tmp: %s", temp.c_str() );

	bool bRedirect = false;
	bool bOK = false;

	if( temp.compare("HTTP/1.1") == 0 || temp.compare("HTTP/1.0") == 0 )
	{
		int code;
		ss >> code;
		if( code == 302 )
		{
			// redirect blabla
			LOGINFO("Need to redirect!");
			bRedirect = true;
		}
		else if( code == 200 )
		{
			LOGINFO("Got 200 OK :D");
			bOK = true;
		}
	}
	else
		return false;

	if( bRedirect )
	{
		std::string Location;
		// Search for "Location:"
		bool bFoundLocation = false;
		while( !bFoundLocation && ss.good() )
		{
			char c = 0;
			while( c != '\n' )
			{
				ss.get( c );
			}
			std::string Name;
			ss >> Name;
			if( Name.compare("Location:") == 0 )
			{
				bFoundLocation = true;
				ss >> Location;
			}
		}
		if( !bFoundLocation )
		{
			LOGERROR("Could not find location");
			return false;
		}

		Location = Location.substr( strlen("http://"), std::string::npos ); // Strip http://
		std::string Server = Location.substr( 0, Location.find( "/" ) ); // Only leave server address
		Location = Location.substr( Server.length(), std::string::npos );
		//LOGINFO("Got location:    (%s)", Location.c_str() );
		//LOGINFO("Got server addr: (%s)", Server.c_str() );
		a_TCPLink->CloseSocket();
		if( a_TCPLink->Connect( Server.c_str(), 80 ) )
		{
			LOGINFO("Successfully connected to %s", Server.c_str() );
			a_TCPLink->SendMessage( ( std::string("GET ") + Location + " HTTP/1.0\r\n\r\n").c_str() );
			std::string Received = a_TCPLink->ReceiveData();
			//LOGINFO("Received data: %s", Received.c_str() );
			return ParseReceived( Received.c_str(), a_TCPLink );
		}
		else
		{
			LOGERROR("Could not connect to %s to verify player name!", Server.c_str() );
		}
	}
	else if( bOK )
	{
		// Header says OK, so receive the rest.

		// Go past header, double \n means end of headers
		char c = 0;
		while( ss.good() )
		{
			while( c != '\n' )
			{
				ss.get( c );
			}
			ss.get( c );
			if( c == '\n' || c == '\r' || ss.peek() == '\r' || ss.peek() == '\n' )
				break;
		}
		if( !ss.good() ) return false;

		std::string Result;
		ss >> Result;
		LOGINFO("Got result: %s", Result.c_str() );
		if( Result.compare("YES") == 0 )
		{
			LOGINFO("Result was \"YES\", so player is authenticated!");
			return true;
		}
		else
		{
			LOGINFO("Result was \"%s\", so player is NOT authenticated!", Result.c_str() );
			return false;
		}
	}
	return false;
}
