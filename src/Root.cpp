
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Root.h"
#include "Server.h"
#include "World.h"
#include "WebAdmin.h"
#include "FurnaceRecipe.h"
#include "GroupManager.h"
#include "CraftingRecipes.h"
#include "Bindings/PluginManager.h"
#include "MonsterConfig.h"
#include "Entities/Player.h"
#include "Blocks/BlockHandler.h"
#include "Items/ItemHandler.h"
#include "Chunk.h"
#include "Protocol/ProtocolRecognizer.h"  // for protocol version constants
#include "CommandOutput.h"
#include "DeadlockDetect.h"
#include "OSSupport/Timer.h"

#include "inifile/iniFile.h"

#ifdef _WIN32
	#include "conio.h"
	#include <psapi.h>
#elif defined(__linux__)
	#include <fstream>
#elif defined(__APPLE__)
	#include <mach/mach.h>
#endif





cRoot* cRoot::s_Root = NULL;





cRoot::cRoot(void) :
	m_PrimaryServerVersion(cProtocolRecognizer::PROTO_VERSION_LATEST),
	m_pDefaultWorld(NULL),
	m_InputThread(NULL),
	m_Server(NULL),
	m_MonsterConfig(NULL),
	m_GroupManager(NULL),
	m_CraftingRecipes(NULL),
	m_FurnaceRecipe(NULL),
	m_WebAdmin(NULL),
	m_PluginManager(NULL),
	m_Log(NULL),
	m_bStop(false),
	m_bRestart(false)
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

	cLogCommandOutputCallback Output;
	
	while (!self.m_bStop && !self.m_bRestart && !m_TerminateEventRaised && std::cin.good())
	{
		AString Command;
		std::getline(std::cin, Command);
		if (!Command.empty())
		{
			self.ExecuteConsoleCommand(TrimString(Command), Output);
		}
	}

	if (m_TerminateEventRaised || !std::cin.good())
	{
		// We have come here because the std::cin has received an EOF / a terminate signal has been sent, and the server is still running; stop the server:
		self.m_bStop = true;
	}
}





void cRoot::Start(void)
{
	#ifdef _WIN32
	HWND hwnd = GetConsoleWindow();
	HMENU hmenu = GetSystemMenu(hwnd, FALSE);
	EnableMenuItem(hmenu, SC_CLOSE, MF_GRAYED);  // Disable close button when starting up; it causes problems with our CTRL-CLOSE handling
	#endif

	cDeadlockDetect dd;
	delete m_Log;
	m_Log = new cMCLogger();

	m_bStop = false;
	while (!m_bStop)
	{
		cTimer Time;
		long long mseconds = Time.GetNowTime();
		
		m_bRestart = false;

		LoadGlobalSettings();

		LOG("Creating new server instance...");
		m_Server = new cServer();

		LOG("Reading server config...");
		cIniFile IniFile;
		if (!IniFile.ReadFile("settings.ini"))
		{
			LOGWARN("Regenerating settings.ini, all settings will be reset");
			IniFile.AddHeaderComment(" This is the main server configuration");
			IniFile.AddHeaderComment(" Most of the settings here can be configured using the webadmin interface, if enabled in webadmin.ini");
			IniFile.AddHeaderComment(" See: http://wiki.mc-server.org/doku.php?id=configure:settings.ini for further configuration help");
		}

		m_PrimaryServerVersion = IniFile.GetValueI("Server", "PrimaryServerVersion", 0);
		if (m_PrimaryServerVersion == 0)
		{
			m_PrimaryServerVersion = cProtocolRecognizer::PROTO_VERSION_LATEST;
		}
		else
		{
			// Make a note in the log that the primary server version is explicitly set in the ini file
			LOGINFO("Primary server version set explicitly to %d.", m_PrimaryServerVersion);
		}

		LOG("Starting server...");
		if (!m_Server->InitServer(IniFile))
		{
			LOGERROR("Failure starting server, aborting...");
			return;
		}

		m_WebAdmin = new cWebAdmin();
		m_WebAdmin->Init();

		LOGD("Loading settings...");
		m_GroupManager    = new cGroupManager();
		m_CraftingRecipes = new cCraftingRecipes;
		m_FurnaceRecipe   = new cFurnaceRecipe();
		
		LOGD("Loading worlds...");
		LoadWorlds(IniFile);

		LOGD("Loading plugin manager...");
		m_PluginManager = new cPluginManager();
		m_PluginManager->ReloadPluginsNow(IniFile);
		
		LOGD("Loading MonsterConfig...");
		m_MonsterConfig = new cMonsterConfig;

		// This sets stuff in motion
		LOGD("Starting Authenticator...");
		m_Authenticator.Start(IniFile);
		
		LOGD("Starting worlds...");
		StartWorlds();
		
		if (IniFile.GetValueSetB("DeadlockDetect", "Enabled", true))
		{
			LOGD("Starting deadlock detector...");
			dd.Start(IniFile.GetValueSetI("DeadlockDetect", "IntervalSec", 20));
		}
		
		IniFile.WriteFile("settings.ini");

		LOGD("Finalising startup...");
		m_Server->Start();
		
		m_WebAdmin->Start();

		#if !defined(ANDROID_NDK)
		LOGD("Starting InputThread...");
		m_InputThread = new cThread( InputThread, this, "cRoot::InputThread");
		m_InputThread->Start( false);  // We should NOT wait? Otherwise we can't stop the server from other threads than the input thread
		#endif

		long long finishmseconds = Time.GetNowTime();
		finishmseconds -= mseconds;

		LOG("Startup complete, took %lld ms!", finishmseconds);
		#ifdef _WIN32
		EnableMenuItem(hmenu, SC_CLOSE, MF_ENABLED);  // Re-enable close button
		#endif

		while (!m_bStop && !m_bRestart && !m_TerminateEventRaised)  // These are modified by external threads
		{
			cSleep::MilliSleep(1000);
		}

		if (m_TerminateEventRaised)
		{
			m_bStop = true;
		}

		#if !defined(ANDROID_NDK)
		delete m_InputThread; m_InputThread = NULL;
		#endif

		// Stop the server:
		m_WebAdmin->Stop();
		LOG("Shutting down server...");
		m_Server->Shutdown();
		LOGD("Shutting down deadlock detector...");
		dd.Stop();
		LOGD("Stopping world threads...");
		StopWorlds();
		LOGD("Stopping authenticator...");
		m_Authenticator.Stop();

		LOGD("Freeing MonsterConfig...");
		delete m_MonsterConfig; m_MonsterConfig = NULL;
		delete m_WebAdmin; m_WebAdmin = NULL;
		LOGD("Unloading recipes...");
		delete m_FurnaceRecipe;   m_FurnaceRecipe = NULL;
		delete m_CraftingRecipes; m_CraftingRecipes = NULL;
		LOGD("Forgetting groups...");
		delete m_GroupManager; m_GroupManager = NULL;
		LOGD("Unloading worlds...");
		UnloadWorlds();
		
		LOGD("Stopping plugin manager...");
		delete m_PluginManager; m_PluginManager = NULL;

		cItemHandler::Deinit();

		LOG("Cleaning up...");
		delete m_Server; m_Server = NULL;
		LOG("Shutdown successful!");
	}

	delete m_Log; m_Log = NULL;
}





void cRoot::LoadGlobalSettings()
{
	// Nothing needed yet
}





void cRoot::LoadWorlds(cIniFile & IniFile)
{
	// First get the default world
	AString DefaultWorldName = IniFile.GetValueSet("Worlds", "DefaultWorld", "world");
	m_pDefaultWorld = new cWorld( DefaultWorldName.c_str());
	m_WorldsByName[ DefaultWorldName ] = m_pDefaultWorld;

	// Then load the other worlds
	unsigned int KeyNum = IniFile.FindKey("Worlds");
	unsigned int NumWorlds = IniFile.GetNumValues( KeyNum);
	if (NumWorlds <= 0)
	{
		return;
	}
	
	bool FoundAdditionalWorlds = false;
	for (unsigned int i = 0; i < NumWorlds; i++)
	{
		AString ValueName = IniFile.GetValueName(KeyNum, i);
		if (ValueName.compare("World") != 0)
		{
			continue;
		}
		AString WorldName = IniFile.GetValue(KeyNum, i);
		if (WorldName.empty())
		{
			continue;
		}
		FoundAdditionalWorlds = true;
		cWorld* NewWorld = new cWorld( WorldName.c_str());
		m_WorldsByName[ WorldName ] = NewWorld;
	}  // for i - Worlds

	if (!FoundAdditionalWorlds)
	{
		if (IniFile.GetKeyComment("Worlds", 0) != " World=secondworld")
		{
			IniFile.DeleteKeyComment("Worlds", 0);
			IniFile.AddKeyComment("Worlds", " World=secondworld");
		}
	}
}





cWorld * cRoot::CreateAndInitializeWorld(const AString & a_WorldName)
{
	if (m_WorldsByName[a_WorldName] != NULL)
	{
		return NULL;
	}
	cWorld* NewWorld = new cWorld(a_WorldName.c_str());
	m_WorldsByName[a_WorldName] = NewWorld;
	NewWorld->Start();
	NewWorld->InitializeSpawn();
	m_PluginManager->CallHookWorldStarted(*NewWorld);
	return NewWorld;
}





void cRoot::StartWorlds(void)
{
	for (WorldMap::iterator itr = m_WorldsByName.begin(); itr != m_WorldsByName.end(); ++itr)
	{
		itr->second->Start();
		itr->second->InitializeSpawn();
		m_PluginManager->CallHookWorldStarted(*itr->second);
	}
}





void cRoot::StopWorlds(void)
{
	for (WorldMap::iterator itr = m_WorldsByName.begin(); itr != m_WorldsByName.end(); ++itr)
	{
		itr->second->Stop();
	}
}





void cRoot::UnloadWorlds(void)
{
	m_pDefaultWorld = NULL;
	for (WorldMap::iterator itr = m_WorldsByName.begin(); itr != m_WorldsByName.end(); ++itr)
	{
		delete itr->second;
	}
	m_WorldsByName.clear();
}





cWorld* cRoot::GetDefaultWorld()
{
	return m_pDefaultWorld;
}





cWorld* cRoot::GetWorld( const AString & a_WorldName)
{
	WorldMap::iterator itr = m_WorldsByName.find( a_WorldName);
	if (itr != m_WorldsByName.end())
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





void cRoot::TickCommands(void)
{
	// Execute any pending commands:
	cCommandQueue PendingCommands;
	{
		cCSLock Lock(m_CSPendingCommands);
		std::swap(PendingCommands, m_PendingCommands);
	}
	for (cCommandQueue::iterator itr = PendingCommands.begin(), end = PendingCommands.end(); itr != end; ++itr)
	{
		ExecuteConsoleCommand(itr->m_Command, *(itr->m_Output));
	}
}





void cRoot::QueueExecuteConsoleCommand(const AString & a_Cmd, cCommandOutputCallback & a_Output)
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
	m_PendingCommands.push_back(cCommand(a_Cmd, &a_Output));
}





void cRoot::QueueExecuteConsoleCommand(const AString & a_Cmd)
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
	m_PendingCommands.push_back(cCommand(a_Cmd, new cLogCommandDeleteSelfOutputCallback));
}





void cRoot::ExecuteConsoleCommand(const AString & a_Cmd, cCommandOutputCallback & a_Output)
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

	LOG("Executing console command: \"%s\"", a_Cmd.c_str());
	m_Server->ExecuteConsoleCommand(a_Cmd, a_Output);
}





void cRoot::KickUser(int a_ClientID, const AString & a_Reason)
{
	m_Server->KickUser(a_ClientID, a_Reason);
}





void cRoot::AuthenticateUser(int a_ClientID, const AString & a_Name, const AString & a_UUID, const Json::Value & a_Properties)
{
	m_Server->AuthenticateUser(a_ClientID, a_Name, a_UUID, a_Properties);
}





int cRoot::GetTotalChunkCount(void)
{
	int res = 0;
	for (WorldMap::iterator itr = m_WorldsByName.begin(); itr != m_WorldsByName.end(); ++itr)
	{
		res += itr->second->GetNumChunks();
	}
	return res;
}





void cRoot::SaveAllChunks(void)
{
	for (WorldMap::iterator itr = m_WorldsByName.begin(); itr != m_WorldsByName.end(); ++itr)
	{
		itr->second->QueueSaveAllChunks();
	}
}





void cRoot::ReloadGroups(void)
{
	LOG("Reload groups ...");
	m_GroupManager->LoadGroups();
	m_GroupManager->CheckUsers();
}





void cRoot::BroadcastChat(const AString & a_Message, eMessageType a_ChatPrefix)
{
	for (WorldMap::iterator itr = m_WorldsByName.begin(), end = m_WorldsByName.end(); itr != end; ++itr)
	{
		itr->second->BroadcastChat(a_Message, NULL, a_ChatPrefix);
	}  // for itr - m_WorldsByName[]
}





void cRoot::BroadcastChat(const cCompositeChat & a_Message)
{
	for (WorldMap::iterator itr = m_WorldsByName.begin(), end = m_WorldsByName.end(); itr != end; ++itr)
	{
		itr->second->BroadcastChat(a_Message);
	}  // for itr - m_WorldsByName[]
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
		size_t        m_BestRating;
		size_t        m_NameLength;
		const AString m_PlayerName;

		virtual bool Item (cPlayer * a_pPlayer)
		{
			size_t Rating = RateCompareString (m_PlayerName, a_pPlayer->GetName());
			if ((Rating > 0) && (Rating >= m_BestRating))
			{
				m_BestMatch = a_pPlayer;
				if (Rating > m_BestRating)
				{
					m_NumMatches = 0;
				}
				m_BestRating = Rating;
				++m_NumMatches;
			}
			if (Rating == m_NameLength)  // Perfect match
			{
				return true;
			}
			return false;
		}

	public:
		cCallback (const AString & a_PlayerName) :
			m_BestRating(0),
			m_NameLength(a_PlayerName.length()),
			m_PlayerName(a_PlayerName),
			m_BestMatch(NULL),
			m_NumMatches(0)
		{}

		cPlayer * m_BestMatch;
		unsigned  m_NumMatches;
	} Callback (a_PlayerName);
	ForEachPlayer(Callback);

	if (Callback.m_NumMatches == 1)
	{
		return a_Callback.Item(Callback.m_BestMatch);
	}
	return false;
}





AString cRoot::GetProtocolVersionTextFromInt(int a_ProtocolVersion)
{
	return cProtocolRecognizer::GetVersionTextFromInt(a_ProtocolVersion);
}





int cRoot::GetVirtualRAMUsage(void)
{
	#ifdef _WIN32
		PROCESS_MEMORY_COUNTERS_EX pmc;
		if (GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS *)&pmc, sizeof(pmc)))
		{
			return (int)(pmc.PrivateUsage / 1024);
		}
		return -1;
	#elif defined(__linux__)
		// Code adapted from http://stackoverflow.com/questions/63166/how-to-determine-cpu-and-memory-consumption-from-inside-a-process
		std::ifstream StatFile("/proc/self/status");
		if (!StatFile.good())
		{
			return -1;
		}
		while (StatFile.good())
		{
			AString Line;
			std::getline(StatFile, Line);
			if (strncmp(Line.c_str(), "VmSize:", 7) == 0)
			{
				int res = atoi(Line.c_str() + 8);
				return (res == 0) ? -1 : res;  // If parsing failed, return -1
			}
		}
		return -1;
	#elif defined (__APPLE__)
		// Code adapted from http://stackoverflow.com/questions/63166/how-to-determine-cpu-and-memory-consumption-from-inside-a-process
		struct task_basic_info t_info;
		mach_msg_type_number_t t_info_count = TASK_BASIC_INFO_COUNT;

		if (KERN_SUCCESS == task_info(
			mach_task_self(),
			TASK_BASIC_INFO,
			(task_info_t)&t_info,
			&t_info_count
		))
		{
			return (int)(t_info.virtual_size / 1024);
		}
		return -1;
	#else
		LOGINFO("%s: Unknown platform, cannot query memory usage", __FUNCTION__);
		return -1;
	#endif
}





int cRoot::GetPhysicalRAMUsage(void)
{
	#ifdef _WIN32
		PROCESS_MEMORY_COUNTERS pmc;
		if (GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc)))
		{
			return (int)(pmc.WorkingSetSize / 1024);
		}
		return -1;
	#elif defined(__linux__)
		// Code adapted from http://stackoverflow.com/questions/63166/how-to-determine-cpu-and-memory-consumption-from-inside-a-process
		std::ifstream StatFile("/proc/self/status");
		if (!StatFile.good())
		{
			return -1;
		}
		while (StatFile.good())
		{
			AString Line;
			std::getline(StatFile, Line);
			if (strncmp(Line.c_str(), "VmRSS:", 6) == 0)
			{
				int res = atoi(Line.c_str() + 7);
				return (res == 0) ? -1 : res;  // If parsing failed, return -1
			}
		}
		return -1;
	#elif defined (__APPLE__)
		// Code adapted from http://stackoverflow.com/questions/63166/how-to-determine-cpu-and-memory-consumption-from-inside-a-process
		struct task_basic_info t_info;
		mach_msg_type_number_t t_info_count = TASK_BASIC_INFO_COUNT;

		if (KERN_SUCCESS == task_info(
			mach_task_self(),
			TASK_BASIC_INFO,
			(task_info_t)&t_info,
			&t_info_count
		))
		{
			return (int)(t_info.resident_size / 1024);
		}
		return -1;
	#else
		LOGINFO("%s: Unknown platform, cannot query memory usage", __FUNCTION__);
		return -1;
	#endif
}





void cRoot::LogChunkStats(cCommandOutputCallback & a_Output)
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
		int NumInSaveQueue = (int)World->GetStorageSaveQueueLength();
		int NumInLoadQueue = (int)World->GetStorageLoadQueueLength();
		int NumValid = 0;
		int NumDirty = 0;
		int NumInLighting = 0;
		World->GetChunkStats(NumValid, NumDirty, NumInLighting);
		a_Output.Out("World %s:", World->GetName().c_str());
		a_Output.Out("  Num loaded chunks: %d", NumValid);
		a_Output.Out("  Num dirty chunks: %d", NumDirty);
		a_Output.Out("  Num chunks in lighting queue: %d", NumInLighting);
		a_Output.Out("  Num chunks in generator queue: %d", NumInGenerator);
		a_Output.Out("  Num chunks in storage load queue: %d", NumInLoadQueue);
		a_Output.Out("  Num chunks in storage save queue: %d", NumInSaveQueue);
		int Mem = NumValid * sizeof(cChunk);
		a_Output.Out("  Memory used by chunks: %d KiB (%d MiB)", (Mem + 1023) / 1024, (Mem + 1024 * 1024 - 1) / (1024 * 1024));
		a_Output.Out("  Per-chunk memory size breakdown:");
		a_Output.Out("    block types:    " SIZE_T_FMT_PRECISION(6)  " bytes (" SIZE_T_FMT_PRECISION(3)  " KiB)", sizeof(cChunkDef::BlockTypes), (sizeof(cChunkDef::BlockTypes) + 1023) / 1024);
		a_Output.Out("    block metadata: " SIZE_T_FMT_PRECISION(6)  " bytes (" SIZE_T_FMT_PRECISION(3)  " KiB)", sizeof(cChunkDef::BlockNibbles), (sizeof(cChunkDef::BlockNibbles) + 1023) / 1024);
		a_Output.Out("    block lighting: " SIZE_T_FMT_PRECISION(6)  " bytes (" SIZE_T_FMT_PRECISION(3)  " KiB)", 2 * sizeof(cChunkDef::BlockNibbles), (2 * sizeof(cChunkDef::BlockNibbles) + 1023) / 1024);
		a_Output.Out("    heightmap:      " SIZE_T_FMT_PRECISION(6)  " bytes (" SIZE_T_FMT_PRECISION(3)  " KiB)", sizeof(cChunkDef::HeightMap), (sizeof(cChunkDef::HeightMap) + 1023) / 1024);
		a_Output.Out("    biomemap:       " SIZE_T_FMT_PRECISION(6)  " bytes (" SIZE_T_FMT_PRECISION(3)  " KiB)", sizeof(cChunkDef::BiomeMap), (sizeof(cChunkDef::BiomeMap) + 1023) / 1024);
		SumNumValid += NumValid;
		SumNumDirty += NumDirty;
		SumNumInLighting += NumInLighting;
		SumNumInGenerator += NumInGenerator;
		SumMem += Mem;
	}
	a_Output.Out("Totals:");
	a_Output.Out("  Num loaded chunks: %d", SumNumValid);
	a_Output.Out("  Num dirty chunks: %d", SumNumDirty);
	a_Output.Out("  Num chunks in lighting queue: %d", SumNumInLighting);
	a_Output.Out("  Num chunks in generator queue: %d", SumNumInGenerator);
	a_Output.Out("  Memory used by chunks: %d KiB (%d MiB)", (SumMem + 1023) / 1024, (SumMem + 1024 * 1024 - 1) / (1024 * 1024));
}





int cRoot::GetFurnaceFuelBurnTime(const cItem & a_Fuel)
{
	cFurnaceRecipe * FR = Get()->GetFurnaceRecipe();
	return FR->GetBurnTime(a_Fuel);
}




