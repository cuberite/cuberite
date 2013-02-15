
// ReDucTor is an awesome guy who helped me a lot

#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Server.h"
#include "ClientHandle.h"
#include "OSSupport/Timer.h"
#include "Mobs/Monster.h"
#include "OSSupport/Socket.h"
#include "Root.h"
#include "World.h"
#include "ChunkDef.h"
#include "PluginManager.h"
#include "GroupManager.h"
#include "ChatColor.h"
#include "Player.h"
#include "Inventory.h"
#include "Item.h"
#include "FurnaceRecipe.h"
#include "Tracer.h"
#include "WebAdmin.h"
#include "Protocol/ProtocolRecognizer.h"

#include "MersenneTwister.h"

#include "../iniFile/iniFile.h"
#include "Vector3f.h"

#include <fstream>
#include <sstream>
#include <iostream>

extern "C" {
	#include "zlib.h"
}




// For the "dumpmem" server command:
/// Synchronize this with main.cpp - the leak finder needs initialization before it can be used to dump memory
#define ENABLE_LEAK_FINDER

#if defined(_MSC_VER) && defined(_DEBUG) && defined(ENABLE_LEAK_FINDER)
	#pragma warning(push)
	#pragma warning(disable:4100)
	#include "LeakFinder.h"
	#pragma warning(pop)
#endif





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

	cEvent RestartEvent;
	std::string ServerID;
};





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
	m_SocketThreads.StopReading(a_Client);
}





void cServer::NotifyClientWrite(const cClientHandle * a_Client)
{
	m_NotifyWriteThread.NotifyClientWrite(a_Client);
}





void cServer::WriteToClient(const cClientHandle * a_Client, const AString & a_Data)
{
	m_SocketThreads.Write(a_Client, a_Data);
}





void cServer::QueueClientClose(const cClientHandle * a_Client)
{
	m_SocketThreads.QueueClose(a_Client);
}





void cServer::RemoveClient(const cClientHandle * a_Client)
{
	m_SocketThreads.RemoveClient(a_Client);
}





bool cServer::InitServer(cIniFile & a_SettingsIni)
{
	if (m_bIsConnected)
	{
		LOGERROR("ERROR: Trying to initialize server while server is already running!");
		return false;
	}

	printf("/============================\\\n");
	printf("|   Custom Minecraft Server  |\n");
	printf("|  Created by Kevin Bansberg |\n");
	printf("|       A.K.A. FakeTruth     |\n");
	printf("| Monsters by Alex Sonek     |\n");
	printf("|       A.K.A. Duralex       |\n");
	printf("| Stuff by Mattes D          |\n");
	printf("|       A.K.A. _Xoft(o)      |\n");
	printf("\\============================/\n");
	printf("More info: WWW.MC-SERVER.ORG\n");
	printf("           WWW.AE-C.NET\n");
	printf("           WWW.RBTHINKTANK.COM\n");
	printf("email: faketruth@gmail.com\n\n");

	LOG("Starting up server.");
	LOGINFO("Compatible clients: %s", MCS_CLIENT_VERSIONS);
	LOGINFO("Compatible protocol versions %s", MCS_PROTOCOL_VERSIONS);

	if (cSocket::WSAStartup() != 0) // Only does anything on Windows, but whatever
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

	int Port = a_SettingsIni.GetValueSetI("Server", "Port", 25565);

	cSocket::SockAddr_In local;
	local.Family = cSocket::ADDRESS_FAMILY_INTERNET;
	local.Address = cSocket::INTERNET_ADDRESS_ANY;
	local.Port = (unsigned short)Port;

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

	m_iServerPort = Port;
	LOG("Port %i has been bound", m_iServerPort);
	m_bIsConnected = true;

	m_pState->ServerID = "-";
	if (a_SettingsIni.GetValueSetB("Authentication", "Authenticate", true))
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
	
	m_ClientViewDistance = a_SettingsIni.GetValueSetI("Server", "DefaultViewDistance", cClientHandle::DEFAULT_VIEW_DISTANCE);
	if (m_ClientViewDistance < cClientHandle::MIN_VIEW_DISTANCE)
	{
		m_ClientViewDistance = cClientHandle::MIN_VIEW_DISTANCE;
		LOGINFO("Setting default viewdistance to the minimum of %d", m_ClientViewDistance);
	}
	if (m_ClientViewDistance > cClientHandle::MAX_VIEW_DISTANCE)
	{
		m_ClientViewDistance = cClientHandle::MAX_VIEW_DISTANCE;
		LOGINFO("Setting default viewdistance to the maximum of %d", m_ClientViewDistance);
	}
	
	m_NotifyWriteThread.Start(this);
	
	PrepareKeys();
	
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
	// TODO: Shut down the server gracefully
	if ( m_pState->SListenClient )
	{
		m_pState->SListenClient.CloseSocket();
	}
	m_pState->SListenClient = 0;

	m_pState->bStopListenThread = true;
	delete m_pState->pListenThread;	m_pState->pListenThread = NULL;
	m_pState->bStopTickThread = true;
	delete m_pState->pTickThread;	m_pState->pTickThread = NULL;

	delete m_pState;
}





void cServer::PrepareKeys(void)
{
	// TODO: Save and load key for persistence across sessions
	// But generating the key takes only a moment, do we even need that?
	
	LOG("Generating protocol encryption keypair...");
	
	time_t CurTime = time(NULL);
	CryptoPP::RandomPool rng;
	rng.Put((const byte *)&CurTime, sizeof(CurTime));
	m_PrivateKey.GenerateRandomWithKeySize(rng, 1024);
	CryptoPP::RSA::PublicKey pk(m_PrivateKey);
	m_PublicKey = pk;
}





void cServer::BroadcastChat(const AString & a_Message, const cClientHandle * a_Exclude)
{
	cCSLock Lock(m_CSClients);
	for (ClientList::iterator itr = m_Clients.begin(); itr != m_Clients.end(); ++itr)
	{
		if ((*itr == a_Exclude) || !(*itr)->IsLoggedIn())
		{
			continue;
		}
		(*itr)->SendChat(a_Message);
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

	cClientHandle * NewHandle = new cClientHandle(&SClient, m_ClientViewDistance);
	if (!m_SocketThreads.AddClient(SClient, NewHandle))
	{
		// For some reason SocketThreads have rejected the handle, clean it up
		LOGERROR("Client \"%s\" cannot be handled, server probably unstable", SClient.GetIPString().c_str());
		SClient.CloseSocket();
		delete NewHandle;
		return;
	}
	
	cCSLock Lock(m_CSClients);
	m_Clients.push_back( NewHandle );
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

	cClientHandleList RemoveClients;
	{
		cCSLock Lock(m_CSClients);
		for (cClientHandleList::iterator itr = m_Clients.begin(); itr != m_Clients.end();)
		{
			if ((*itr)->IsDestroyed())
			{
				RemoveClients.push_back(*itr);  // Remove the client later, when CS is not held, to avoid deadlock ( http://forum.mc-server.org/showthread.php?tid=374 )
				itr = m_Clients.erase(itr);
				continue;
			}
			(*itr)->Tick(a_Dt);
			++itr;
		}  // for itr - m_Clients[]
	}
	for (cClientHandleList::iterator itr = RemoveClients.begin(); itr != RemoveClients.end(); ++itr)
	{
		delete *itr;
	} // for itr - RemoveClients[]

	cRoot::Get()->GetPluginManager()->Tick( a_Dt );

	if( !m_bRestarting )
	{
		return true;
	}
	else
	{
		m_bRestarting = false;
		m_pState->RestartEvent.Set();
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





bool cServer::Command(cClientHandle & a_Client, const AString & a_Cmd)
{
	return cRoot::Get()->GetPluginManager()->CallHookChat(a_Client.GetPlayer(), a_Cmd);
}





void cServer::ExecuteConsoleCommand(const AString & a_Cmd)
{
	AStringVector split = StringSplit(a_Cmd, " ");
	if (split.empty())
	{
		return;
	}
	
	// Special handling: "stop" and "restart" are built in
	if ((split[0].compare("stop") == 0) || (split[0].compare("restart") == 0))
	{
		return;
	}
	
	// There is currently no way a plugin can do these (and probably won't ever be):
	if (split[0].compare("chunkstats") == 0)
	{
		cRoot::Get()->LogChunkStats();
		return;
	}
	#if defined(_MSC_VER) && defined(_DEBUG) && defined(ENABLE_LEAK_FINDER)
	if (split[0].compare("dumpmem") == 0)
	{
		LeakFinderXmlOutput Output("memdump.xml");
		DumpUsedMemory(&Output);
		return;
	}
	#endif
	
	if (cPluginManager::Get()->ExecuteConsoleCommand(split))
	{
		return;
	}
	
	LOG("Unknown command, type 'help' for all commands.\n");
}





void cServer::BindBuiltInConsoleCommands(void)
{
	cPluginManager * PlgMgr = cPluginManager::Get();
	PlgMgr->BindConsoleCommand("restart", NULL, "Restarts the server cleanly");
	PlgMgr->BindConsoleCommand("stop", NULL, "Stops the server cleanly");
	PlgMgr->BindConsoleCommand("chunkstats", NULL, "Displays detailed chunk memory statistics");
	#if defined(_MSC_VER) && defined(_DEBUG) && defined(ENABLE_LEAK_FINDER)
	PlgMgr->BindConsoleCommand("dumpmem", NULL, "Dumps all used memory blocks together with their callstacks into memdump.xml");
	#endif	
}





void cServer::SendMessage(const AString & a_Message, cPlayer * a_Player /* = NULL */, bool a_bExclude /* = false */ )
{
	if ((a_Player != NULL) && !a_bExclude)
	{
		cClientHandle * Client = a_Player->GetClientHandle();
		if (Client != NULL)
		{
			Client->SendChat(a_Message);
		}
		return;
	}

	BroadcastChat(a_Message, (a_Player != NULL) ? a_Player->GetClientHandle() : NULL);
}





void cServer::Shutdown()
{
	m_bRestarting = true;
	m_pState->RestartEvent.Wait();

	cRoot::Get()->SaveAllChunks();

	cCSLock Lock(m_CSClients);
	for( ClientList::iterator itr = m_Clients.begin(); itr != m_Clients.end(); ++itr )
	{
		(*itr)->Destroy();
		delete *itr;
	}
	m_Clients.clear();
}





const AString & cServer::GetServerID(void) const
{
	return m_pState->ServerID;
}





void cServer::KickUser(int a_ClientID, const AString & a_Reason)
{
	cCSLock Lock(m_CSClients);
	for (ClientList::iterator itr = m_Clients.begin(); itr != m_Clients.end(); ++itr)
	{
		if ((*itr)->GetUniqueID() == a_ClientID)
		{
			(*itr)->Kick(a_Reason);
		}
	}  // for itr - m_Clients[]
}





void cServer::AuthenticateUser(int a_ClientID)
{
	cCSLock Lock(m_CSClients);
	for (ClientList::iterator itr = m_Clients.begin(); itr != m_Clients.end(); ++itr)
	{
		if ((*itr)->GetUniqueID() == a_ClientID)
		{
			(*itr)->Authenticate();
		}
	}  // for itr - m_Clients[]
}





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cServer::cClientPacketThread:

cServer::cNotifyWriteThread::cNotifyWriteThread(void) :
	super("ClientPacketThread"),
	m_Server(NULL)
{
}





cServer::cNotifyWriteThread::~cNotifyWriteThread()
{
	m_ShouldTerminate = true;
	m_Event.Set();
	Wait();
}





bool cServer::cNotifyWriteThread::Start(cServer * a_Server)
{
	m_Server = a_Server;
	return super::Start();
}





void cServer::cNotifyWriteThread::Execute(void)
{
	cClientHandleList Clients;
	while (!m_ShouldTerminate)
	{
		cCSLock Lock(m_CS);
		while (m_Clients.size() == 0)
		{
			cCSUnlock Unlock(Lock);
			m_Event.Wait();
			if (m_ShouldTerminate)
			{
				return;
			}
		}
		
		// Copy the clients to notify and unlock the CS:
		Clients.splice(Clients.begin(), m_Clients);
		Lock.Unlock();
		
		for (cClientHandleList::iterator itr = Clients.begin(); itr != Clients.end(); ++itr)
		{
			m_Server->m_SocketThreads.NotifyWrite(*itr);
		}  // for itr - Clients[]
		Clients.clear();
	}  // while (!mShouldTerminate)
}





void cServer::cNotifyWriteThread::NotifyClientWrite(const cClientHandle * a_Client)
{
	{
		cCSLock Lock(m_CS);
		m_Clients.remove(const_cast<cClientHandle *>(a_Client));  // Put it there only once
		m_Clients.push_back(const_cast<cClientHandle *>(a_Client));
	}
	m_Event.Set();
}




