
// ReDucTor is an awesome guy who helped me a lot

#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "cServer.h"
#include "cClientHandle.h"
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

#include "MersenneTwister.h"

#include "../iniFile/iniFile.h"
#include "Vector3f.h"

#include "packets/cPacket_Chat.h"

#include <fstream>
#include <sstream>
#include <iostream>





extern "C" {
	#include "zlib.h"
}



bool g_bWaterPhysics = false;

typedef std::list< cClientHandle* > ClientList;





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

	ClientList Clients;

	cEvent RestartEvent;
	std::string ServerID;
};





cServer * cServer::GetServer()
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





void cServer::ClientDestroying(const cClientHandle * a_Client)
{
	m_SocketThreads.RemoveClient(a_Client);
}





bool cServer::InitServer( int a_Port )
{
	if( m_bIsConnected )
	{
		LOGERROR("ERROR: Trying to initialize server while server is already running!");
		return false;
	}

	printf("/============================\\\n");
	printf("|   Custom Minecraft Server  |\n");
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

	if( cSocket::WSAStartup() != 0 ) // Only does anything on Windows, but whatever
	{
		LOGERROR("WSAStartup() != 0");
		return false;
	}

	m_pState->SListenClient = cSocket::CreateSocket();

	if( !m_pState->SListenClient.IsValid() )
	{
		LOGERROR("m_SListenClient==INVALID_SOCKET (%s)", cSocket::GetErrorString( cSocket::GetLastError() ).c_str() );
		return false;
	}

	if( m_pState->SListenClient.SetReuseAddress() == -1 )
	{
		LOGERROR("setsockopt == -1");
		return false;
	}

	cSocket::SockAddr_In local;
	local.Family = cSocket::ADDRESS_FAMILY_INTERNET;
	local.Address = cSocket::INTERNET_ADDRESS_ANY;
	local.Port = (unsigned short)a_Port; // 25565

	if( m_pState->SListenClient.Bind( local ) != 0 )
	{
		LOGERROR("bind fail (%s)", cSocket::GetErrorString( cSocket::GetLastError() ).c_str() );
		return false;
	}
	
	if( m_pState->SListenClient.Listen( 10 ) != 0)
	{
		LOGERROR("listen fail (%s)", cSocket::GetErrorString( cSocket::GetLastError() ).c_str() );
		return false;
	}

	m_iServerPort = a_Port;
	LOG("Port %i has been bound, server is open for connections", m_iServerPort);
	m_bIsConnected = true;

	cIniFile IniFile("settings.ini");
	if( IniFile.ReadFile() )
	{
		g_bWaterPhysics = IniFile.GetValueB("Physics", "Water", false );
		
		/* Replaced below with 1.0.0 compatible ServerID generation

		std::string ServerID = IniFile.GetValue("Server", "ServerID");
		if( ServerID.empty() )
		{
			ServerID = "MCServer";
			IniFile.SetValue("Server", "ServerID", ServerID, true );
			IniFile.WriteFile();
		}
		*/
		m_pState->ServerID = "-";
		if (IniFile.GetValueB("Authentication", "Authenticate"))
		{
			MTRand mtrand1;
			unsigned int r1 = (mtrand1.randInt()%1147483647) + 1000000000;
			unsigned int r2 = (mtrand1.randInt()%1147483647) + 1000000000;
			std::ostringstream sid;
			sid << std::hex << r1;
			sid << std::hex << r2;
			std::string ServerID = sid.str();
			ServerID.resize(16, '0');
			m_pState->ServerID = ServerID;
		}
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
    if( m_pState->SListenClient ) m_pState->SListenClient.CloseSocket();
	m_pState->SListenClient = 0;

	m_pState->bStopListenThread = true;
	delete m_pState->pListenThread;	m_pState->pListenThread = 0;
	m_pState->bStopTickThread = true;
	delete m_pState->pTickThread;	m_pState->pTickThread = 0;

	delete m_pState;
}





// TODO - Need to modify this or something, so it broadcasts to all worlds? And move this to cWorld?
void cServer::Broadcast( const cPacket * a_Packet, cClientHandle* a_Exclude /* = 0 */ )
{
	for( ClientList::iterator itr = m_pState->Clients.begin(); itr != m_pState->Clients.end(); ++itr)
	{
		if( *itr == a_Exclude || !(*itr)->IsLoggedIn() ) continue;
		(*itr)->Send( a_Packet );
	}
}





void cServer::StartListenClient()
{
	cSocket SClient = m_pState->SListenClient.Accept();

	if (!SClient.IsValid())
	{
		return;
	}
	
	const AString & ClientIP = SClient.GetIPString();
	if (ClientIP.empty())
	{
		LOGWARN("cServer: A client connected, but didn't present its IP, disconnecting.");
		SClient.CloseSocket();
		return;
	}

	LOG("Client \"%s\" connected!", ClientIP.c_str());

	cClientHandle *NewHandle = new cClientHandle(SClient);
	if (!m_SocketThreads.AddClient(&(NewHandle->GetSocket()), NewHandle))
	{
		// For some reason SocketThreads have rejected the handle, clean it up
		LOGERROR("Client \"%s\" cannot be handled, server probably unstable", SClient.GetIPString().c_str());
		SClient.CloseSocket();
		delete NewHandle;
		return;
	}
	m_pState->Clients.push_back( NewHandle );	// TODO - lock list
}





bool cServer::Tick(float a_Dt)
{
    //LOG("1. Tick %0.2f", a_Dt);
	if( a_Dt > 100.f ) a_Dt = 100.f; // Don't go over 1/10 second

	m_Millisecondsf += a_Dt;
	if( m_Millisecondsf > 1.f )
	{
		m_Milliseconds += (int)m_Millisecondsf;
		m_Millisecondsf = m_Millisecondsf - (int)m_Millisecondsf;
	}

	cRoot::Get()->TickWorlds( a_Dt ); // TODO - Maybe give all worlds their own thread?

	//World->LockClientHandle(); // TODO - Lock client list
	for( ClientList::iterator itr = m_pState->Clients.begin(); itr != m_pState->Clients.end();)
	{
		if( (*itr)->IsDestroyed() )
		{
			cClientHandle* RemoveMe = *itr;
			++itr;
			m_pState->Clients.remove( RemoveMe );
			delete RemoveMe;
			continue;
		}
		(*itr)->Tick(a_Dt);
		++itr;
	}
	//World->UnlockClientHandle();

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

	long long msPerTick = 50;	// TODO - Put this in server config file
	long long LastTime = Timer.GetNowTime();

	bool bKeepGoing = true;
	while( bKeepGoing )
	{
		long long NowTime = Timer.GetNowTime();
		float DeltaTime = (float)(NowTime-LastTime);
		bKeepGoing = CServerObj->Tick( DeltaTime );
		long long TickTime = Timer.GetNowTime() - NowTime;
		
		if( TickTime < msPerTick )	// Stretch tick time until it's at least msPerTick
		{
			cSleep::MilliSleep( (unsigned int)( msPerTick - TickTime ) );
		}

		LastTime = NowTime;
	}

	LOG("TICK THREAD STOPPED");
}





void cServer::StartListenThread()
{
	m_pState->pListenThread = new cThread( ServerListenThread, this, "cServer::ServerListenThread" );
	m_pState->pTickThread = new cThread( ServerTickThread, this, "cServer::ServerTickThread" );
	m_pState->pListenThread->Start( true );
	m_pState->pTickThread->Start( true );
}





template <class T>
bool from_string(
	T& t,
	const std::string& s,
	std::ios_base& (*f)(std::ios_base&)
)
{
	std::istringstream iss(s);
	return !(iss >> f >> t).fail();
}





bool cServer::Command( cClientHandle & a_Client, const char* a_Cmd )
{
    cPluginManager* PM = cRoot::Get()->GetPluginManager();
	if( PM->CallHook( cPluginManager::E_PLUGIN_CHAT, 2, a_Cmd, a_Client.GetPlayer() ) )
		return true;

	std::string Command( a_Cmd );
	if( Command.length() <= 0 ) return false;
	if( Command[0] != '/' ) return false;

	AStringVector split = StringSplit( Command, " " );
	if( split.size() == 0 )
		return false;

	if (split[0].compare("/coords") == 0)
	{
		AString Pos;
		Printf(Pos, "[X:%0.2f] [Y:%0.2f] [Z:%0.2f]", a_Client.GetPlayer()->GetPosX(), a_Client.GetPlayer()->GetPosY(), a_Client.GetPlayer()->GetPosZ() );
		a_Client.Send( cPacket_Chat(cChatColor::Green + Pos));
		return true;
	}
	return false;
}





void cServer::ServerCommand( const char * a_Cmd )
{
	AString Command( a_Cmd );
	AStringVector split = StringSplit( Command, " " );
	if( split.size() > 0 )
	{
		if( split[0].compare( "help" ) == 0 )
		{
			printf("================== ALL COMMANDS ===================\n");
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
			cRoot::Get()->GetWorld()->SaveAllChunks();	// TODO - Force ALL worlds to save their chunks
			return;
		}
		if( split[0].compare( "list" ) == 0 )
		{
			class cPlayerLogger : public cPlayerListCallback
			{
				virtual bool Item(cPlayer * a_Player) override
				{
					LOG("\t%s @ %s", a_Player->GetName().c_str(), a_Player->GetClientHandle()->GetSocket().GetIPString());
					return false;
				}
			} Logger;
			cRoot::Get()->GetWorld()->ForEachPlayer(&Logger);
			return;
		}
		if( split[0].compare( "numchunks" ) == 0 )
		{
			printf("Num loaded chunks: %i\n", cRoot::Get()->GetTotalChunkCount() );
			return;
		}
		
		if(split[0].compare("monsters") == 0 )
		{
			// TODO: cWorld::ListMonsters();
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

	//cWorld* World = cRoot::Get()->GetWorld();
	//World->LockClientHandle();	// TODO - Lock client list
	for( ClientList::iterator itr = m_pState->Clients.begin(); itr != m_pState->Clients.end(); ++itr )
	{
		delete *itr;
	}
	m_pState->Clients.clear();
	//World->UnlockClientHandle();
}





const AString & cServer::GetServerID(void) const
{
	return m_pState->ServerID;
}





void cServer::KickUser(const AString & iUserName, const AString & iReason)
{
	for (ClientList::iterator itr = m_pState->Clients.begin(); itr != m_pState->Clients.end(); ++itr)
	{
		if ((*itr)->GetUsername() == iUserName)
		{
			(*itr)->Kick(iReason);
		}
	}  // for itr - m_pState->Clients[]
}





void cServer::AuthenticateUser(const AString & iUserName)
{
	for (ClientList::iterator itr = m_pState->Clients.begin(); itr != m_pState->Clients.end(); ++itr)
	{
		if ((*itr)->GetUsername() == iUserName)
		{
			(*itr)->Authenticate();
		}
	}  // for itr - m_pState->Clients[]
}




