
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "cRoot.h"
#include "cServer.h"
#include "cWorld.h"
#include "cWebAdmin.h"
#include "cFurnaceRecipe.h"
#include "cGroupManager.h"
#include "CraftingRecipes.h"
#include "cPluginManager.h"
#include "cMonsterConfig.h"
#include "cSleep.h"
#include "cThread.h"
#include "cFileFormatUpdater.h"
#include "cRedstone.h"
#include "cPlayer.h"
#include "blocks/Block.h"
#include "items/Item.h"
#include "cChunk.h"

#ifdef USE_SQUIRREL
	#include "squirrelbindings/SquirrelFunctions.h"
	#include "squirrelbindings/SquirrelBindings.h"
#endif

#include "../iniFile/iniFile.h"

#include <iostream>





cRoot* cRoot::s_Root = 0;





cRoot::cRoot()
	: m_Server( NULL )
	, m_MonsterConfig( NULL )
	, m_GroupManager( NULL )
	, m_CraftingRecipes(NULL)
	, m_FurnaceRecipe( NULL )
	, m_WebAdmin( NULL )
	, m_PluginManager( NULL )
	, m_Log( NULL )
	, m_bStop( false )
	, m_bRestart( false )
	, m_InputThread( NULL )
	, m_pDefaultWorld( NULL )
{
	s_Root = this;
}





cRoot::~cRoot()
{
	s_Root = 0;
}





void cRoot::InputThread(void * a_Params)
{
	cRoot & self = *(cRoot*)a_Params;

	while (!(self.m_bStop || self.m_bRestart))
	{
		std::string Command;
		std::getline(std::cin, Command);
		self.ServerCommand(Command);
	}
}





void cRoot::Start()
{
	delete m_Log;
	m_Log = new cMCLogger();

	m_bStop = false;
	while (!m_bStop)
	{
		m_bRestart = false;

		LoadGlobalSettings();

		cFileFormatUpdater::UpdateFileFormat();

		LOG("Creating new server instance...");
		m_Server = new cServer();

		LOG("Starting server...");
		cIniFile IniFile("settings.ini");
		IniFile.ReadFile();
		int Port = IniFile.GetValueSetI("Server", "Port", 25565 );
		if(!m_Server->InitServer( Port ))
		{
			LOG("Failed to start server, shutting down.");
			return;
		}
		IniFile.WriteFile();

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
		m_CraftingRecipes = new cCraftingRecipes;
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
		
		LOG("Starting worlds...");
		StartWorlds();
		
		LOG("Starting server...");
		m_Server->StartListenThread();
		//cHeartBeat* HeartBeat = new cHeartBeat();

#if !defined(ANDROID_NDK)
		LOG("Starting InputThread...");
		m_InputThread = new cThread( InputThread, this, "cRoot::InputThread" );
		m_InputThread->Start( false );	//we should NOT wait? Otherwise we can´t stop the server from other threads than the input thread
#endif

		LOG("Initialization done, server running now.");
		while( !m_bStop && !m_bRestart ) // These are modified by external threads
		{
			cSleep::MilliSleep( 1000 );
		}

#if !defined(ANDROID_NDK)
		delete m_InputThread; m_InputThread = 0;
#endif

		// Deallocate stuffs
		LOG("Shutting down server...");
		m_Server->Shutdown(); // This waits for threads to stop and d/c clients
		LOG("Stopping world threads...");
		StopWorlds();
		LOG("Stopping authenticator...");
		m_Authenticator.Stop();
		LOG("Stopping plugin manager...");
		delete m_PluginManager; m_PluginManager = 0;  // This should be first
		

		#ifdef USE_SQUIRREL
		CloseSquirrelVM();
		#endif
		LOG("Freeing MonsterConfig...");
		delete m_MonsterConfig; m_MonsterConfig = 0;
		LOG("Stopping WebAdmin...");
		delete m_WebAdmin; m_WebAdmin = 0;
		LOG("Unloading recipes...");
		delete m_FurnaceRecipe;   m_FurnaceRecipe = NULL;
		delete m_CraftingRecipes; m_CraftingRecipes = NULL;
		LOG("Forgetting groups...");
		delete m_GroupManager; m_GroupManager = 0;
		LOG("Unloading worlds...");
		UnloadWorlds();
		
		cItemHandler::Deinit();
		cBlockHandler::Deinit();

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





void cRoot::LoadWorlds(void)
{
	cIniFile IniFile("settings.ini"); IniFile.ReadFile();

	// First get the default world
	AString DefaultWorldName = IniFile.GetValueSet("Worlds", "DefaultWorld", "world");
	m_pDefaultWorld = new cWorld( DefaultWorldName.c_str() );
	m_WorldsByName[ DefaultWorldName ] = m_pDefaultWorld;

	// Then load the other worlds
	unsigned int KeyNum = IniFile.FindKey("Worlds");
	unsigned int NumWorlds = IniFile.GetNumValues( KeyNum );
	if (NumWorlds <= 0)
	{
		return;
	}
	
	for (unsigned int i = 0; i < NumWorlds; i++)
	{
		AString ValueName = IniFile.GetValueName(KeyNum, i );
		if (ValueName.compare("World") != 0)
		{
			continue;
		}
		AString WorldName = IniFile.GetValue(KeyNum, i );
		if (WorldName.empty())
		{
			continue;
		}
		cWorld* NewWorld = new cWorld( WorldName.c_str() );
		m_WorldsByName[ WorldName ] = NewWorld;
	}  // for i - Worlds
}





void cRoot::StartWorlds(void)
{
	for( WorldMap::iterator itr = m_WorldsByName.begin(); itr != m_WorldsByName.end(); ++itr )
	{
		itr->second->InitializeSpawn();
	}
}





void cRoot::StopWorlds(void)
{
	for( WorldMap::iterator itr = m_WorldsByName.begin(); itr != m_WorldsByName.end(); ++itr )
	{
		itr->second->StopThreads();
	}
}





void cRoot::UnloadWorlds(void)
{
	m_pDefaultWorld = NULL;
	for( WorldMap::iterator itr = m_WorldsByName.begin(); itr != m_WorldsByName.end(); ++itr )
	{
		delete itr->second;
	}
	m_WorldsByName.clear();
}





cWorld* cRoot::GetDefaultWorld()
{
	return m_pDefaultWorld;
}





cWorld* cRoot::GetWorld( const AString & a_WorldName )
{
	WorldMap::iterator itr = m_WorldsByName.find( a_WorldName );
	if( itr != m_WorldsByName.end() )
		return itr->second;
	return 0;
}





bool cRoot::ForEachWorld(cWorldListCallback & a_Callback)
{
	for (WorldMap::iterator itr = m_WorldsByName.begin(), itr2 = itr; itr != m_WorldsByName.end(); itr = itr2)
	{
		++itr2;
		if (a_Callback.Item(itr->second))
		{
			return false;
		}
	}
	return true;
}





void cRoot::TickWorlds( float a_Dt )
{
	for( WorldMap::iterator itr = m_WorldsByName.begin(); itr != m_WorldsByName.end(); ++itr )
	{
		itr->second->Tick( a_Dt );
	}
}





void cRoot::ServerCommand(const AString & a_Cmd)
{
	LOG("Server console command: \"%s\"", a_Cmd.c_str());
	m_Server->ServerCommand(a_Cmd);
	if (a_Cmd == "stop")
	{
		m_bStop = true;
	}
	else if (a_Cmd == "restart")
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
	for ( WorldMap::iterator itr = m_WorldsByName.begin(); itr != m_WorldsByName.end(); ++itr )
	{
		res += itr->second->GetNumChunks();
	}
	return res;
}





void cRoot::SaveAllChunks(void)
{
	for (WorldMap::iterator itr = m_WorldsByName.begin(); itr != m_WorldsByName.end(); ++itr)
	{
		itr->second->SaveAllChunks();
	}
}





bool cRoot::ForEachPlayer(cPlayerListCallback & a_Callback)
{
	for (WorldMap::iterator itr = m_WorldsByName.begin(), itr2 = itr; itr != m_WorldsByName.end(); itr = itr2)
	{
		++itr2;
		if (!itr->second->ForEachPlayer(a_Callback))
		{
			return false;
		}
	}
	return true;
}





bool cRoot::FindAndDoWithPlayer(const AString & a_PlayerName, cPlayerListCallback & a_Callback)
{
	class cCallback : public cPlayerListCallback
	{
		unsigned int BestRating;
		unsigned int NameLength;
		const AString PlayerName;

		cPlayerListCallback & m_Callback;
		virtual bool Item (cPlayer * a_pPlayer)
		{
			unsigned int Rating = RateCompareString (PlayerName, a_pPlayer->GetName());
			if (Rating > 0 && Rating >= BestRating)
			{
				BestMatch = a_pPlayer;
				if( Rating > BestRating ) NumMatches = 0;
				BestRating = Rating;
				++NumMatches;
			}
			if (Rating == NameLength) // Perfect match
			{
				return false;
			}
			return true;
		}

	public:
		cCallback (const AString & a_PlayerName, cPlayerListCallback & a_Callback) 
			: m_Callback( a_Callback )
			, BestMatch( NULL )
			, BestRating( 0 )
			, NumMatches( 0 )
			, NameLength( a_PlayerName.length() )
			, PlayerName( a_PlayerName )
		{}

		cPlayer * BestMatch;
		unsigned int NumMatches;
	} Callback (a_PlayerName, a_Callback);
	ForEachPlayer( Callback );

	if (Callback.NumMatches == 1)
	{
		return a_Callback.Item (Callback.BestMatch);
	}
	return false;
}





void cRoot::LogChunkStats(void)
{
	int SumNumValid = 0;
	int SumNumDirty = 0;
	int SumNumInLighting = 0;
	int SumNumInGenerator = 0;
	int SumMem = 0;
	for (WorldMap::iterator itr = m_WorldsByName.begin(), end = m_WorldsByName.end(); itr != end; ++itr)
	{
		cWorld * World = itr->second;
		int NumInGenerator = World->GetGeneratorQueueLength();
		int NumInSaveQueue = World->GetStorageSaveQueueLength();
		int NumInLoadQueue = World->GetStorageLoadQueueLength();
		int NumValid = 0;
		int NumDirty = 0;
		int NumInLighting = 0;
		World->GetChunkStats(NumValid, NumDirty, NumInLighting);
		LOG("World %s:", World->GetName().c_str());
		LOG("  Num loaded chunks: %d", NumValid);
		LOG("  Num dirty chunks: %d", NumDirty);
		LOG("  Num chunks in lighting queue: %d", NumInLighting);
		LOG("  Num chunks in generator queue: %d", NumInGenerator);
		LOG("  Num chunks in storage load queue: %d", NumInLoadQueue);
		LOG("  Num chunks in storage save queue: %d", NumInSaveQueue);
		int Mem = NumValid * sizeof(cChunk);
		LOG("  Memory used by chunks: %d KiB (%d MiB)", (Mem + 1023) / 1024, (Mem + 1024 * 1024 - 1) / (1024 * 1024));
		LOG("  Per-chunk memory size breakdown:");
		LOG("    block types:    %6d bytes (%3d KiB)", sizeof(cChunkDef::BlockTypes), (sizeof(cChunkDef::BlockTypes) + 1023) / 1024);
		LOG("    block metadata: %6d bytes (%3d KiB)", sizeof(cChunkDef::BlockNibbles), (sizeof(cChunkDef::BlockNibbles) + 1023) / 1024);
		LOG("    block lighting: %6d bytes (%3d KiB)", 2 * sizeof(cChunkDef::BlockNibbles), (2 * sizeof(cChunkDef::BlockNibbles) + 1023) / 1024);
		LOG("    heightmap:      %6d bytes (%3d KiB)", sizeof(cChunkDef::HeightMap), (sizeof(cChunkDef::HeightMap) + 1023) / 1024);
		LOG("    biomemap:       %6d bytes (%3d KiB)", sizeof(cChunkDef::BiomeMap), (sizeof(cChunkDef::BiomeMap) + 1023) / 1024);
		int Rest = sizeof(cChunk) - sizeof(cChunkDef::BlockTypes) - 3 * sizeof(cChunkDef::BlockNibbles) - sizeof(cChunkDef::HeightMap) - sizeof(cChunkDef::BiomeMap);
		LOG("    other:          %6d bytes (%3d KiB)", Rest, (Rest + 1023) / 1024);
		SumNumValid += NumValid;
		SumNumDirty += NumDirty;
		SumNumInLighting += NumInLighting;
		SumNumInGenerator += NumInGenerator;
		SumMem += Mem;
	}
	LOG("Totals:");
	LOG("  Num loaded chunks: %d", SumNumValid);
	LOG("  Num dirty chunks: %d", SumNumDirty);
	LOG("  Num chunks in lighting queue: %d", SumNumInLighting);
	LOG("  Num chunks in generator queue: %d", SumNumInGenerator);
	LOG("  Memory used by chunks: %d KiB (%d MiB)", (SumMem + 1023) / 1024, (SumMem + 1024 * 1024 - 1) / (1024 * 1024));
}




