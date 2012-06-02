
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "cRoot.h"
#include "cServer.h"
#include "cWorld.h"
#include "cWebAdmin.h"
#include "cFurnaceRecipe.h"
#include "cGroupManager.h"
#include "cRecipeChecker.h"
#include "cPluginManager.h"
#include "cMonsterConfig.h"
#include "cSleep.h"
#include "cThread.h"
#include "cFileFormatUpdater.h"
#include "cRedstone.h"

#include "../iniFile/iniFile.h"

#include <iostream>





cRoot* cRoot::s_Root = 0;

typedef std::map< AString, cWorld* > WorldMap;
struct cRoot::sRootState
{
	cWorld* pDefaultWorld;
	WorldMap WorldsByName;
};





cRoot::cRoot()
	: m_Server( 0 )
	, m_MonsterConfig( 0 )
	, m_GroupManager( 0 )
	, m_RecipeChecker( 0 )
	, m_FurnaceRecipe( 0 )
	, m_WebAdmin( 0 )
	, m_PluginManager( 0 )
	, m_Log( 0 )
	, m_bStop( false )
	, m_bRestart( false )
	, m_InputThread( 0 )
	, m_pState( new sRootState )
{
	s_Root = this;
}





cRoot::~cRoot()
{
	s_Root = 0;
	delete m_pState;
}





void cRoot::InputThread(void* a_Params)
{
	cRoot& self = *(cRoot*)a_Params;

	while( !(self.m_bStop || self.m_bRestart) )
	{
		std::string Command;
		std::getline(std::cin, Command);
		self.ServerCommand( Command.c_str() );
	}
}





void cRoot::Start()
{
	if( m_Log ) delete m_Log, m_Log = 0;
	m_Log = new cMCLogger();

	m_bStop = false;
	while(!m_bStop)
	{
		m_bRestart = false;

		LoadGlobalSettings();

		cFileFormatUpdater::UpdateFileFormat();

		LOG("Creating new server instance...");
		m_Server = new cServer();

		LOG("Starting server...");
		cIniFile IniFile("settings.ini"); IniFile.ReadFile();
		int Port = IniFile.GetValueI("Server", "Port", 25565 );
		if(!m_Server->InitServer( Port ))
		{
			LOG("Failed to start server, shutting down.");
			return;
		}

		cIniFile WebIniFile("webadmin.ini");
		if( WebIniFile.ReadFile() )
		{
			if( WebIniFile.GetValueB("WebAdmin", "Enabled", false ) == true )
			{
				LOG("Creating WebAdmin...");
				m_WebAdmin = new cWebAdmin(8080);
			}
		}

		LOG("Loading settings...");
		m_GroupManager	= new cGroupManager();
		m_RecipeChecker = new cRecipeChecker();
		m_FurnaceRecipe = new cFurnaceRecipe();
		
		LOG("Loading worlds...");
		LoadWorlds();

		LOG("Loading plugin manager...");
		m_PluginManager = new cPluginManager(); // This should be last
		m_PluginManager->ReloadPluginsNow();
		
		LOG("Loading MonsterConfig...");
		m_MonsterConfig = new cMonsterConfig;

		// This sets stuff in motion
		LOG("Starting Authenticator...");
		m_Authenticator.Start();
		
		LOG("Starting server...");
		m_Server->StartListenThread();
		//cHeartBeat* HeartBeat = new cHeartBeat();

		LOG("Starting InputThread...");
		m_InputThread = new cThread( InputThread, this, "cRoot::InputThread" );
		m_InputThread->Start( true );

		LOG("Initialization done, server running now.");
		while( !m_bStop && !m_bRestart ) // These are modified by external threads
		{
			cSleep::MilliSleep( 1000 );
		}

		delete m_InputThread; m_InputThread = 0;

		// Deallocate stuffs
		LOG("Shutting down server...");
		m_Server->Shutdown(); // This waits for threads to stop and d/c clients
		LOG("Stopping authenticator...");
		m_Authenticator.Stop();
		LOG("Stopping plugin manager...");
		delete m_PluginManager; m_PluginManager = 0;  // This should be first
		LOG("Freeing MonsterConfig...");
		delete m_MonsterConfig; m_MonsterConfig = 0;
		LOG("Stopping WebAdmin...");
		delete m_WebAdmin; m_WebAdmin = 0;
		LOG("Unloading recipes...");
		delete m_FurnaceRecipe; m_FurnaceRecipe = 0;
		delete m_RecipeChecker; m_RecipeChecker = 0;
		LOG("Forgetting groups...");
		delete m_GroupManager; m_GroupManager = 0;
		LOG("Unloading worlds...");
		UnloadWorlds();
		LOG("Destroying server...");
		//delete HeartBeat; HeartBeat = 0;
		delete m_Server; m_Server = 0;
		LOG("Shutdown done.");
	}

	delete m_Log; m_Log = 0;
}





void cRoot::LoadGlobalSettings()
{
	cIniFile IniFile("settings.ini"); 
	if( IniFile.ReadFile() )
	{
		cRedstone::s_UseRedstone = IniFile.GetValueB("Redstone", "SimulateRedstone", true );
	}
}





void cRoot::LoadWorlds()
{
	cIniFile IniFile("settings.ini"); IniFile.ReadFile();

	// First get the default world
	AString DefaultWorldName = IniFile.GetValue("Worlds", "DefaultWorld", "world");
	m_pState->pDefaultWorld = new cWorld( DefaultWorldName.c_str() );
	m_pState->pDefaultWorld->InitializeSpawn();
	m_pState->WorldsByName[ DefaultWorldName ] = m_pState->pDefaultWorld;

	// Then load the other worlds
	unsigned int KeyNum = IniFile.FindKey("Worlds");
	unsigned int NumWorlds = IniFile.GetNumValues( KeyNum );
	if ( NumWorlds > 0 )
	{
		for (unsigned int i = 0; i < NumWorlds; i++)
		{
			std::string ValueName = IniFile.GetValueName(KeyNum, i );
			if( ValueName.compare("World") == 0 )
			{
				std::string WorldName = IniFile.GetValue(KeyNum, i );
				if (!WorldName.empty())
				{
					cWorld* NewWorld = new cWorld( WorldName.c_str() );
					NewWorld->InitializeSpawn();
					m_pState->WorldsByName[ WorldName ] = NewWorld;
				}
			}
		}
	}
}





void cRoot::UnloadWorlds()
{
	for( WorldMap::iterator itr = m_pState->WorldsByName.begin(); itr != m_pState->WorldsByName.end(); ++itr )
	{
		delete itr->second;
	}
	m_pState->WorldsByName.clear();
}





cWorld* cRoot::GetDefaultWorld()
{
	return m_pState->pDefaultWorld;
}





cWorld* cRoot::GetWorld( const AString & a_WorldName )
{
	WorldMap::iterator itr = m_pState->WorldsByName.find( a_WorldName );
	if( itr != m_pState->WorldsByName.end() )
		return itr->second;
	return 0;
}





bool cRoot::ForEachWorld(cWorldListCallback & a_Callback)
{
	for( WorldMap::iterator itr = m_pState->WorldsByName.begin(); itr != m_pState->WorldsByName.end(); ++itr )
	{
		if (a_Callback.Item(itr->second))
		{
			return false;
		}
	}
	return true;
}





void cRoot::TickWorlds( float a_Dt )
{
	for( WorldMap::iterator itr = m_pState->WorldsByName.begin(); itr != m_pState->WorldsByName.end(); ++itr )
	{
		itr->second->Tick( a_Dt );
	}
}





void cRoot::ServerCommand( const char * a_Cmd )
{
	LOG("Server console command: \"%s\"", a_Cmd );
	m_Server->ServerCommand( a_Cmd );
	if( strcmp(a_Cmd, "stop") == 0 )
	{
		m_bStop = true;
	}
	else if( strcmp( a_Cmd, "restart") == 0 )
	{
		m_bRestart = true;
	}
}





void cRoot::KickUser(int a_ClientID, const AString & a_Reason)
{
	m_Server->KickUser(a_ClientID, a_Reason);
}





void cRoot::AuthenticateUser(int a_ClientID)
{
	m_Server->AuthenticateUser(a_ClientID);
}





int cRoot::GetTotalChunkCount(void)
{
	int res = 0;
	for ( WorldMap::iterator itr = m_pState->WorldsByName.begin(); itr != m_pState->WorldsByName.end(); ++itr )
	{
		res += itr->second->GetNumChunks();
	}
	return res;
}





void cRoot::SaveAllChunks(void)
{
	for (WorldMap::iterator itr = m_pState->WorldsByName.begin(); itr != m_pState->WorldsByName.end(); ++itr)
	{
		itr->second->SaveAllChunks();
	}
}





bool cRoot::ForEachPlayer(cPlayerListCallback & a_Callback)
{
	for (WorldMap::iterator itr = m_pState->WorldsByName.begin(); itr != m_pState->WorldsByName.end(); ++itr)
	{
		if (!itr->second->ForEachPlayer(a_Callback))
		{
			return false;
		}
	}
	return true;
}




