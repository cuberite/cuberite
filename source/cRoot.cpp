#include "cRoot.h"
#include "cMCLogger.h"
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
#include <time.h>
#include <cstdio>

#ifndef _WIN32
#include <cstring>
#endif

cRoot* cRoot::s_Root = 0;

cRoot::cRoot()
	: m_Server( 0 )
	, m_World( 0 )
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
{
	s_Root = this;
}

cRoot::~cRoot()
{
	s_Root = 0;
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
		m_World = new cWorld();
		m_World->InitializeSpawn();

		m_PluginManager = new cPluginManager(); // This should be last
		m_PluginManager->ReloadPluginsNow();
		m_MonsterConfig = new cMonsterConfig(2);

		// This sets stuff in motion
		m_Server->StartListenThread();
		//cHeartBeat* HeartBeat = new cHeartBeat();

		m_InputThread = new cThread( InputThread, this );
		m_InputThread->Start( true );

		while( !m_bStop && !m_bRestart ) // These are modified by external threads
		{
			cSleep::MilliSleep( 1000 );
		}

		delete m_InputThread; m_InputThread = 0;

		// Deallocate stuffs
		m_Server->Shutdown(); // This waits for threads to stop and d/c clients
		delete m_PluginManager; m_PluginManager = 0;  // This should be first
		delete m_MonsterConfig; m_MonsterConfig = 0;
		if( m_WebAdmin ) { delete m_WebAdmin; m_WebAdmin = 0; }
		delete m_FurnaceRecipe; m_FurnaceRecipe = 0;
		delete m_RecipeChecker; m_RecipeChecker = 0;
		delete m_GroupManager; m_GroupManager = 0;
		delete m_World; m_World = 0;
		//delete HeartBeat; HeartBeat = 0;
		delete m_Server; m_Server = 0;
	}

	delete m_Log; m_Log = 0;
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
