// ReDucTor is an awesome guy who helped me a lot

#include "cServer.h"
#include "cClientHandle.h"
#include "cMCLogger.h"
#include "cThread.h"
#include "cEvent.h"
#include "cSleep.h"
#include "cTimer.h"
#include "cMonster.h"
#include "cSocket.h"
#include "cRoot.h"
#include "cWorld.h"
#include "cChunk.h"
#include "cPluginManager.h"
#include "cGroupManager.h"
#include "cChatColor.h"
#include "cPlayer.h"
#include "cInventory.h"
#include "cItem.h"
#include "cRecipeChecker.h"
#include "cFurnaceRecipe.h"
#include "cTracer.h"
#include "cWebAdmin.h"

#include "../iniFile/iniFile.h"
#include "Vector3f.h"

#include "packets/cPacket_Chat.h"

#ifndef _WIN32
#include <cstring>
#include <errno.h>
#include <semaphore.h>
#include "MCSocket.h"
#endif

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

extern "C" {
	#include "zlib.h"
}

#ifdef SendMessage
#undef SendMessage
#endif


bool g_bWaterPhysics = false;

struct cServer::sServerState
{
	sServerState()
		: pListenThread( 0 )
		, pTickThread( 0 )
		, bStopListenThread( false )
		, bStopTickThread( false )
	{}
	cSocket SListenClient; // socket listening for client calls

	cThread* pListenThread;	bool bStopListenThread;
	cThread* pTickThread;	bool bStopTickThread;

	cEvent RestartEvent;
	std::string ServerID;
};

cServer*cServer::GetServer()
{
	LOGWARN("WARNING: Using deprecated function cServer::GetServer() use cRoot::Get()->GetServer() instead!");
	return cRoot::Get()->GetServer();
}

void cServer::ServerListenThread( void *a_Args )
{
	LOG("ServerListenThread");
	cServer* self = (cServer*)a_Args;
	sServerState* m_pState = self->m_pState;
	while( !m_pState->bStopListenThread )
	{
		self->StartListenClient();
	}
}

std::string GetWSAError()
{
#ifdef _WIN32
	switch( WSAGetLastError() )
	{
	case WSANOTINITIALISED:
		return "WSANOTINITIALISED";
	case WSAENETDOWN:
		return "WSAENETDOWN";
	case WSAEFAULT:
		return "WSAEFAULT";
	case WSAENOTCONN:
		return "WSAENOTCONN";
	case WSAEINTR:
		return "WSAEINTR";
	case WSAEINPROGRESS:
		return "WSAEINPROGRESS";
	case WSAENETRESET:
		return "WSAENETRESET";
	case WSAENOTSOCK:
		return "WSAENOTSOCK";
	case WSAEOPNOTSUPP:
		return "WSAEOPNOTSUPP";
	case WSAESHUTDOWN:
		return "WSAESHUTDOWN";
	case WSAEWOULDBLOCK:
		return "WSAEWOULDBLOCK";
	case WSAEMSGSIZE:
		return "WSAEMSGSIZE";
	case WSAEINVAL:
		return "WSAEINVAL";
	case WSAECONNABORTED:
		return "WSAECONNABORTED";
	case WSAETIMEDOUT:
		return "WSAETIMEDOUT";
	case WSAECONNRESET:
		return "WSAECONNRESET";
	}
#endif
	return "No Error";
}

bool cServer::InitServer( int a_Port )
{
	if( m_bIsConnected )
	{
		LOGERROR("ERROR: Trying to initialize server while server is already running!");
		return false;
	}

	printf("/============================\\\n");
	printf("|   Minecraft Alpha Server   |\n");
	printf("|  Created by Kevin Bansberg |\n");
	printf("|           A.K.A.           |\n");
	printf("|         FakeTruth          |\n");
	printf("| Monsters by Alex Sonek     |\n");
	printf("|           A.K.A. Duralex   |\n");
	printf("\\============================/\n");
	printf("More info: WWW.MC-SERVER.ORG\n");
	printf("           WWW.AE-C.NET\n");
	printf("           WWW.RBTHINKTANK.COM\n");
	printf("email: faketruth@gmail.com\n\n");

	LOG("Starting up server.");

#ifdef _WIN32
	WSADATA wsaData;
	memset( &wsaData, 0, sizeof( wsaData ) );
    int wsaret=WSAStartup(/*0x101*/ MAKEWORD(2, 2),&wsaData);

	if(wsaret!=0)
	{
		LOG("wsaret != 0");
		return false;
	}
#endif

	sockaddr_in local;

	local.sin_family=AF_INET;
	local.sin_addr.s_addr=INADDR_ANY;
	local.sin_port=htons((u_short)a_Port);  // 25565

	m_pState->SListenClient = socket(AF_INET,SOCK_STREAM,0);

	if( !m_pState->SListenClient.IsValid() )
	{
#ifdef _WIN32
		LOGERROR("m_SListenClient==INVALID_SOCKET (%s)", GetWSAError().c_str() );
#else
		LOGERROR("m_SListenClient==INVALID_SOCKET");
#endif
	}


#ifdef _WIN32
	char yes = 1;
#else
    int yes = 1;
#endif
    if (setsockopt( m_pState->SListenClient, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
        LOGERROR("setsockopt == -1");
        return false;
    }

	if( bind( m_pState->SListenClient, (sockaddr*)&local, sizeof(local)) != 0 )
	{
#ifdef _WIN32
		LOGERROR("bind fail (%s)", GetWSAError().c_str() );
#else
        LOGERROR("bind fail (%i)", errno);
#endif
		return false;
	}


	if( listen( m_pState->SListenClient , 10 ) != 0)
	{
#ifdef _WIN32
		LOGERROR("listen fail (%s)", GetWSAError().c_str() );
#else
		LOGERROR("listen fail (%i)", errno);
#endif
		return false;
	}

	m_iServerPort = a_Port;
	LOG("Port %i has been bound, server is open for connections", m_iServerPort);
	m_bIsConnected = true;

	cIniFile IniFile("settings.ini");
	if( IniFile.ReadFile() )
	{
		g_bWaterPhysics = IniFile.GetValueB("Physics", "Water", false );
		std::string ServerID = IniFile.GetValue("Server", "ServerID");
		if( ServerID.empty() )
		{
			ServerID = "MCServer";
			IniFile.SetValue("Server", "ServerID", ServerID, true );
			IniFile.WriteFile();
		}
		m_pState->ServerID = ServerID;
	}
	return true;
}

cServer::cServer()
	: m_pState( new sServerState )
	, m_Millisecondsf( 0 )
	, m_Milliseconds( 0 )
	, m_bIsConnected( false )
	, m_iServerPort( 0 )
	, m_bRestarting( false )
{
}

cServer::~cServer()
{
    if( m_pState->SListenClient ) closesocket( m_pState->SListenClient );
	m_pState->SListenClient = 0;

	m_pState->bStopListenThread = true;
	delete m_pState->pListenThread;	m_pState->pListenThread = 0;
	m_pState->bStopTickThread = true;
	delete m_pState->pTickThread;	m_pState->pTickThread = 0;

	delete m_pState;
}

void cServer::Broadcast( const cPacket & a_Packet, cClientHandle* a_Exclude /* = 0 */ )
{
	//m_World->LockClientHandle();
	cWorld* World = cRoot::Get()->GetWorld();
	for( cWorld::ClientList::iterator itr = World->GetClients().begin(); itr != World->GetClients().end(); ++itr)
	{
		if( *itr == a_Exclude || !(*itr)->IsLoggedIn() ) continue;
		(*itr)->Send( a_Packet );
	}
	//m_World->UnlockClientHandle();
}

void cServer::SendAllEntitiesTo(cClientHandle* a_Target)
{
	cWorld* World = cRoot::Get()->GetWorld();
	for( cWorld::EntityList::iterator itr = World->GetEntities().begin(); itr != World->GetEntities().end(); ++itr)
	{
		(*itr)->SpawnOn( a_Target );
	}
}

void cServer::StartListenClient()
{
    sockaddr_in from;
    socklen_t fromlen=sizeof(from);

	cSocket SClient = accept(
		m_pState->SListenClient,
        (sockaddr*)&from,
		&fromlen);

	if( from.sin_addr.s_addr && SClient.IsValid() )
	{
		char * ClientIP = 0;
		if((ClientIP = inet_ntoa(from.sin_addr)) == 0 )
			return;

		LOG("%s connected!", ClientIP);

		cClientHandle *NewHandle = 0;
		NewHandle = new cClientHandle( SClient );
		cWorld* World = cRoot::Get()->GetWorld();
		World->LockClientHandle();
		World->AddClient( NewHandle );
		World->UnlockClientHandle();
	}
}

bool cServer::Tick(float a_Dt)
{
    //LOG("Tick");
	if( a_Dt > 100.f ) a_Dt = 100.f; // Don't go over 1/10 second

	cSleep::MilliSleep( 50 ); // Don't tick too much

	m_Millisecondsf += a_Dt;
	if( m_Millisecondsf > 1.f )
	{
		m_Milliseconds += (int)m_Millisecondsf;
		m_Millisecondsf = m_Millisecondsf - (int)m_Millisecondsf;
	}

	cWorld* World = cRoot::Get()->GetWorld();
	World->Tick(a_Dt);

	World->LockClientHandle();
	for( cWorld::ClientList::iterator itr = World->GetClients().begin(); itr != World->GetClients().end();)
	{
		(*itr)->HandlePendingPackets();

		if( (*itr)->IsDestroyed() )
		{

			cClientHandle* RemoveMe = *itr;
			++itr;
			cRoot::Get()->GetWorld()->RemoveClient( RemoveMe );
			continue;
		}
		(*itr)->Tick(a_Dt);
		++itr;
	}
	World->UnlockClientHandle();

	cRoot::Get()->GetPluginManager()->Tick( a_Dt );

	if( !m_bRestarting )
		return true;
	else
	{
		m_bRestarting = false;
		m_pState->RestartEvent.Set();
		LOG("<<<<>>>>SIGNALLED SEMAPHORE");
		return false;
	}
}

void ServerTickThread( void * a_Param )
{
	LOG("ServerTickThread");
	cServer *CServerObj = (cServer*)a_Param;

	cTimer Timer;

	long long LastTime = Timer.GetNowTime();

	bool bKeepGoing = true;
	while( bKeepGoing )
	{
		long long NowTime = Timer.GetNowTime();
		float DeltaTime = (float)(NowTime-LastTime);
		bKeepGoing = CServerObj->Tick( DeltaTime );
		LastTime = NowTime;
	}

	LOG("TICK THREAD STOPPED");
}

void cServer::StartListenThread()
{
	m_pState->pListenThread = new cThread( ServerListenThread, this );
	m_pState->pTickThread = new cThread( ServerTickThread, this );
	m_pState->pListenThread->Start( true );
	m_pState->pTickThread->Start( true );
}

std::vector< std::string > StringSplit(std::string str, std::string delim)
{
	std::vector< std::string > results;
	size_t cutAt;
	while( (cutAt = str.find_first_of(delim)) != str.npos )
	{
		if(cutAt > 0)
		{
			results.push_back(str.substr(0,cutAt));
		}
		str = str.substr(cutAt+1);
	}
	if(str.length() > 0)
	{
		results.push_back(str);
	}
	return results;
}

template <class T>
bool from_string(T& t,
				 const std::string& s,
				 std::ios_base& (*f)(std::ios_base&))
{
	std::istringstream iss(s);
	return !(iss >> f >> t).fail();
}

std::string & StrToUpper(std::string& s)
{
		std::string::iterator i = s.begin();
		std::string::iterator end = s.end();

		while (i != end) {
				*i = (char)toupper(*i);
				++i;
		}
		return s;
}

int NoCaseCompare( std::string s1, std::string  s2 )
{
	return StrToUpper( s1 ).compare( StrToUpper( s2 ) );
}

bool cServer::Command( cClientHandle & a_Client, const char* a_Cmd )
{
    cPluginManager* PM = cRoot::Get()->GetPluginManager();
	if( PM->CallHook( cPluginManager::E_PLUGIN_CHAT, 2, a_Cmd, a_Client.GetPlayer() ) )
		return true;

	std::string Command( a_Cmd );
	if( Command.length() <= 0 ) return false;
	if( Command[0] != '/' ) return false;

	std::vector< std::string > split = StringSplit( Command, " " );
	if( split.size() == 0 )
		return false;

	if( split[0].compare("/coords") == 0 )
	{
		char c_Str[128];
		sprintf_s( c_Str, 128, "[X:%0.2f] [Y:%0.2f] [Z:%0.2f]", a_Client.GetPlayer()->GetPosX(), a_Client.GetPlayer()->GetPosY(), a_Client.GetPlayer()->GetPosZ() );
		a_Client.Send( cPacket_Chat( cChatColor::Green + c_Str ) );
		return true;
	}
	return false;
}

void cServer::ServerCommand( const char* a_Cmd )
{
	std::string Command( a_Cmd );
	std::vector< std::string > split = StringSplit( Command, " " );
	if( split.size() > 0 )
	{
		if( split[0].compare( "help" ) == 0 )
		{
			printf("===================ALL COMMANDS====================\n");
			printf("help      - Shows this message\n");
			printf("save-all  - Saves all loaded chunks to disk\n");
			printf("list      - Lists all players currently in server\n");
			printf("numchunks - Shows number of chunks currently loaded\n");
			printf("say       - Sends a chat message to all players\n");
			printf("restart   - Kicks all clients, and saves everything\n");
			printf("            and clears memory\n");
			printf("stop      - Saves everything and closes server\n");
			printf("===================================================\n");
			return;
		}
		if( split[0].compare( "stop" ) == 0 || split[0].compare( "restart" ) == 0 )
		{
			return;
		}
		if( split[0].compare( "save-all" ) == 0 )
		{
			cRoot::Get()->GetWorld()->SaveAllChunks();
			return;
		}
		if( split[0].compare( "list" ) == 0 )
		{
			cWorld::EntityList Entities = cRoot::Get()->GetWorld()->GetEntities();
			std::string PlayerString;
			int NumPlayers = 0;
			cRoot::Get()->GetWorld()->LockEntities();
			for( cWorld::EntityList::iterator itr = Entities.begin(); itr != Entities.end(); ++itr)
			{
				if( (*itr)->GetEntityType() != cEntity::E_PLAYER ) continue;
				PlayerString.push_back(' ');
				PlayerString += ((cPlayer*)*itr)->GetName();
				NumPlayers++;
			}
			cRoot::Get()->GetWorld()->UnlockEntities();
			printf( "Players (%i):%s\n", NumPlayers, PlayerString.c_str() );
			return;
		}
		if( split[0].compare( "numchunks" ) == 0 )
		{
			//printf("Num loaded chunks: %i\n", cRoot::Get()->GetWorld()->GetChunks().size() );
			return;
		}
		if(split[0].compare("monsters") == 0 ){
			cMonster::ListMonsters();
			return;
		}
		if(split.size() > 1)
		{
			if( split[0].compare( "say" ) == 0 )
			{
				std::string Message = cChatColor::Purple + "[SERVER] " + Command.substr( Command.find_first_of("say") + 4 );
				LOG("%s", Message.c_str() );
				Broadcast( cPacket_Chat(Message) );
				return;
			}
		}
		printf("Unknown command, type 'help' for all commands.\n");
	}
	//LOG("You didn't enter anything? (%s)", a_Cmd.c_str() );
}

void cServer::SendMessage( const char* a_Message, cPlayer* a_Player /* = 0 */, bool a_bExclude /* = false */ )
{
	cPacket_Chat Chat( a_Message );
	if( a_Player && !a_bExclude )
	{
		cClientHandle* Client = a_Player->GetClientHandle();
		if( Client ) Client->Send( Chat );
		return;
	}

	Broadcast( Chat, (a_Player)?a_Player->GetClientHandle():0 );
}

void cServer::Shutdown()
{
	m_bRestarting = true;
	m_pState->RestartEvent.Wait();

	cRoot::Get()->GetWorld()->SaveAllChunks();

	cWorld* World = cRoot::Get()->GetWorld();
	World->LockClientHandle();
	while( World->GetClients().begin() != World->GetClients().end() )
	{
		World->RemoveClient( *World->GetClients().begin() );
	}
	World->UnlockClientHandle();
}


const char* cServer::GetServerID()
{
	return m_pState->ServerID.c_str();
}