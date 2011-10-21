#include "cHeartBeat.h"
#include "cMCLogger.h"
#include "md5/md5.h"

#include <stdio.h>

#include "cRoot.h"
#include "cServer.h"
#include "cSleep.h"

#ifndef _WIN32
#define sprintf_s(dst, size, format, ...) sprintf(dst, format, __VA_ARGS__ )
#endif

cHeartBeat::cHeartBeat()
{
	m_State = 0;
	Authenticate();
}

cHeartBeat::~cHeartBeat()
{
}

void cHeartBeat::ReceivedData( char a_Data[256], int a_Size )
{
	if( a_Size < 0 )	// Disconnected
		return;

	char MySalt[] = "1234567890";

	if( a_Size == 0 )
	{
		Authenticate();
		return;
	}

	bool bLoop = false;
	do
	{
		switch (m_State)
		{
		case 1:
			{
				m_ServerID = std::string( a_Data, a_Size );
				LOGINFO("Got server ID %s", m_ServerID.c_str() );
				std::string Hash = md5( m_ServerID + std::string( MySalt ) );
				CloseSocket();
				if( Connect( "mc-server.org", 80 ) )
				{
					SendMessage( (std::string("GET http://master.mc-server.org/?hash=") + Hash + std::string("&server=") + m_ServerID + "\n").c_str() );
					m_State = 2;
				}
			}
			break;
		case 2:
			{
				std::string ReturnedString = std::string( a_Data, a_Size );
				if( ReturnedString.compare("VALIDATED") == 0 )
				{
					LOGINFO("Successfully validated server on master server list");
				}
				else
				{
					LOGINFO("Could not validate server! Will try again later.");
					cSleep::MilliSleep( 10*1000 );
					Authenticate();
					return;
				}
				m_State = 3;
			}	// Don't break, but fall through and update server info
		case 3:
			{
				cSleep::MilliSleep( 10*1000 );
				SendUpdate();
				m_State = 4;
			}
			break;
		case 4:
			{
				if( a_Data[0] == '0' )
				{
					LOGINFO("Successfully updated server info!");
					cSleep::MilliSleep( 10*1000 );
					SendUpdate();
				}
				else
				{
					LOGINFO("Failed to update server info, reauthenticating");
					Authenticate();
				}
			}
		default:
			break;
		};
	} while( bLoop );
}

void cHeartBeat::SendUpdate()
{
	CloseSocket();
	if( Connect( "mc-server.org", 80 ) )
	{
		int Port = cRoot::Get()->GetServer()->GetPort();
		char c_Port[16];
		sprintf_s( c_Port, 16, "%i", Port );

		std::string sPort = std::string( c_Port );
		std::string sChecksum = md5( m_ServerID + sPort );
		SendMessage( (std::string("GET http://master.mc-server.org/?update=") + m_ServerID + std::string("&checksum=") + sChecksum + std::string("&port=") + sPort + "\n").c_str() );
	}
}

void cHeartBeat::Authenticate()
{
	CloseSocket();
	if( Connect( "mc-server.org", 80 ) )
	{
		m_State = 1;
		int RetVal = SendMessage( "GET http://master.mc-server.org/\r\n\r\n");
		LOGINFO("Returned %i", RetVal );
	}
}
