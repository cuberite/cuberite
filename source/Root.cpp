
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Root.h"
#include "Server.h"
#include "World.h"
#include "WebAdmin.h"
#include "FurnaceRecipe.h"
#include "GroupManager.h"
#include "CraftingRecipes.h"
#include "PluginManager.h"
#include "MonsterConfig.h"
#include "Player.h"
#include "Blocks/BlockHandler.h"
#include "Items/ItemHandler.h"
#include "Chunk.h"
#include "Protocol/ProtocolRecognizer.h"  // for protocol version constants

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
		self.ExecuteConsoleCommand(Command);
	}
}





void cRoot::Start(void)
{
	delete m_Log;
	m_Log = new cMCLogger();

	m_bStop = false;
	while (!m_bStop)
	{
		m_bRestart = false;

		LoadGlobalSettings();

		LOG("Creating new server instance...");
		m_Server = new cServer();

		LOG("Reading server config...");
		cIniFile IniFile("settings.ini");
		if (!IniFile.ReadFile())
		{
			LOGINFO("settings.ini inaccessible, using settings.example.ini for defaults!");
			IniFile.Path("settings.example.ini");
			IniFile.ReadFile();
			IniFile.Path("settings.ini");
		}
		m_PrimaryServerVersion = IniFile.GetValueI("Server", "PrimaryServerVersion", 0);
		if (m_PrimaryServerVersion == 0)
		{
			m_PrimaryServerVersion = cProtocolRecognizer::PROTO_VERSION_LATEST;
		}
		else
		{
			// Make a note in the log that the primary server version is explicitly set in the ini file
			LOGINFO("settings.ini: [Server].PrimaryServerVersion set to %d.", m_PrimaryServerVersion);
		}

		LOG("Starting server...");
		if (!m_Server->InitServer(IniFile))
		{
			LOGERROR("Failed to start server, shutting down.");
			return;
		}
		IniFile.WriteFile();

		cIniFile WebIniFile("webadmin.ini");
		if (!WebIniFile.ReadFile())
		{
			LOGINFO("webadmin.ini inaccessible, using webadmin.example.ini for defaults!");
			WebIniFile.Path("webadmin.example.ini");
			WebIniFile.ReadFile();
			WebIniFile.Path("webadmin.ini");
			WebIniFile.WriteFile();
		}

		if (WebIniFile.GetValueB("WebAdmin", "Enabled", false ))
		{
			LOG("Creating WebAdmin...");
			m_WebAdmin = new cWebAdmin(8080);
		}

		LOG("Loading settings...");
		m_GroupManager	= new cGroupManager();
		m_CraftingRecipes = new cCraftingRecipes;
		m_FurnaceRecipe = new cFurnaceRecipe();
		
		LOG("Loading worlds...");
		LoadWorlds();

		LOG("Loading plugin manager...");
		m_PluginManager = new cPluginManager();
		m_PluginManager->ReloadPluginsNow();
		
		LOG("Loading MonsterConfig...");
		m_MonsterConfig = new cMonsterConfig;

		// This sets stuff in motion
		LOG("Starting Authenticator...");
		m_Authenticator.Start();
		
		LOG("Starting worlds...");
		StartWorlds();
		
		LOG("Starting server...");
		m_Server->Start();

		#if !defined(ANDROID_NDK)
		LOG("Starting InputThread...");
		m_InputThread = new cThread( InputThread, this, "cRoot::InputThread" );
		m_InputThread->Start( false );	// We should NOT wait? Otherwise we can´t stop the server from other threads than the input thread
		#endif

		LOG("Initialization done, server running now.");
		while (!m_bStop && !m_bRestart)  // These are modified by external threads
		{
			cSleep::MilliSleep(1000);
		}

		#if !defined(ANDROID_NDK)
		delete m_InputThread; m_InputThread = NULL;
		#endif

		// Deallocate stuffs
		LOG("Shutting down server...");
		m_Server->Shutdown();  // This waits for threads to stop and d/c clients
		LOG("Stopping world threads...");
		StopWorlds();
		LOG("Stopping authenticator...");
		m_Authenticator.Stop();
		

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
		
		LOG("Stopping plugin manager...");
		delete m_PluginManager; m_PluginManager = NULL;

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
	// Nothing needed yet
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





void cRoot::TickWorlds(float a_Dt)
{
	// Execute any pending commands:
	AStringVector PendingCommands;
	{
		cCSLock Lock(m_CSPendingCommands);
		std::swap(PendingCommands, m_PendingCommands);
	}
	for (AStringVector::iterator itr = PendingCommands.begin(), end = PendingCommands.end(); itr != end; ++itr)
	{
		DoExecuteConsoleCommand(*itr);
	}
	
	// Tick the worlds:
	for (WorldMap::iterator itr = m_WorldsByName.begin(); itr != m_WorldsByName.end(); ++itr)
	{
		itr->second->Tick(a_Dt);
	}
}





void cRoot::ExecuteConsoleCommand(const AString & a_Cmd)
{
	// Some commands are built-in:
	if (a_Cmd == "stop")
	{
		m_bStop = true;
	}
	else if (a_Cmd == "restart")
	{
		m_bRestart = true;
	}

	// Put the command into a queue (Alleviates FS #363):
	cCSLock Lock(m_CSPendingCommands);
	m_PendingCommands.push_back(a_Cmd);
}





void cRoot::DoExecuteConsoleCommand(const AString & a_Cmd)
{
	LOG("Executing console command: \"%s\"", a_Cmd.c_str());
	m_Server->ExecuteConsoleCommand(a_Cmd);
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





AString cRoot::GetProtocolVersionTextFromInt(int a_ProtocolVersion)
{
	return cProtocolRecognizer::GetVersionTextFromInt(a_ProtocolVersion);
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




