
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

#include "../iniFile/iniFile.h"

#include <iostream>





cRoot* cRoot::s_Root = 0;

typedef std::map< std::string, cWorld* > WorldMap;
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

		cFileFormatUpdater::UpdateFileFormat();

		m_Server = new cServer();

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
				m_WebAdmin = new cWebAdmin(8080);
			}
		}

		m_GroupManager	= new cGroupManager();
		m_RecipeChecker = new cRecipeChecker();
		m_FurnaceRecipe = new cFurnaceRecipe();
		LoadWorlds();

		m_PluginManager = new cPluginManager(); // This should be last
		m_PluginManager->ReloadPluginsNow();
		m_MonsterConfig = new cMonsterConfig(2);

		// This sets stuff in motion
		m_Authenticator.Start();
		m_Server->StartListenThread();
		//cHeartBeat* HeartBeat = new cHeartBeat();

		m_InputThread = new cThread( InputThread, this, "cRoot::InputThread" );
		m_InputThread->Start( true );

		while( !m_bStop && !m_bRestart ) // These are modified by external threads
		{
			cSleep::MilliSleep( 1000 );
		}

		delete m_InputThread; m_InputThread = 0;

		// Deallocate stuffs
		m_Server->Shutdown(); // This waits for threads to stop and d/c clients
		m_Authenticator.Stop();
		delete m_PluginManager; m_PluginManager = 0;  // This should be first
		delete m_MonsterConfig; m_MonsterConfig = 0;
		if( m_WebAdmin ) { delete m_WebAdmin; m_WebAdmin = 0; }
		delete m_FurnaceRecipe; m_FurnaceRecipe = 0;
		delete m_RecipeChecker; m_RecipeChecker = 0;
		delete m_GroupManager; m_GroupManager = 0;
		UnloadWorlds();
		//delete HeartBeat; HeartBeat = 0;
		delete m_Server; m_Server = 0;
	}

	delete m_Log; m_Log = 0;
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
	if( NumWorlds > 0 )
	{
		for(unsigned int i = 0; i < NumWorlds; i++)
		{
			std::string ValueName = IniFile.GetValueName(KeyNum, i );
			if( ValueName.compare("World") == 0 )
			{
				std::string WorldName = IniFile.GetValue(KeyNum, i );
				if( WorldName.size() > 0 )
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





cWorld* cRoot::GetWorld()
{
	return GetDefaultWorld();
}





cWorld* cRoot::GetDefaultWorld()
{
	return m_pState->pDefaultWorld;
}





cWorld* cRoot::GetWorld( const char* a_WorldName )
{
	WorldMap::iterator itr = m_pState->WorldsByName.find( a_WorldName );
	if( itr != m_pState->WorldsByName.end() )
		return itr->second;
	return 0;
}





void cRoot::TickWorlds( float a_Dt )
{
	for( WorldMap::iterator itr = m_pState->WorldsByName.begin(); itr != m_pState->WorldsByName.end(); ++itr )
	{
		itr->second->Tick( a_Dt );
	}
}





void cRoot::ServerCommand( const char* a_Cmd )
{
	//LOG("Command: %s", a_Cmd );
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





void cRoot::KickUser(const AString & iUserName, const AString & iReason)
{
	m_Server->KickUser(iUserName, iReason);
}





void cRoot::AuthenticateUser(const AString & iUserName)
{
	m_Server->AuthenticateUser(iUserName);
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




