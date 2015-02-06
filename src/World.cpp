
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "BlockID.h"
#include "World.h"
#include "ChunkDef.h"
#include "ClientHandle.h"
#include "Server.h"
#include "Item.h"
#include "Root.h"
#include "IniFile.h"
#include "ChunkMap.h"
#include "Generating/ChunkDesc.h"
#include "SetChunkData.h"

// Serializers
#include "WorldStorage/ScoreboardSerializer.h"

// Entities (except mobs):
#include "Entities/ExpOrb.h"
#include "Entities/FallingBlock.h"
#include "Entities/Minecart.h"
#include "Entities/Pickup.h"
#include "Entities/Player.h"
#include "Entities/TNTEntity.h"

#include "BlockEntities/CommandBlockEntity.h"
#include "BlockEntities/BeaconEntity.h"

// Simulators:
#include "Simulator/SimulatorManager.h"
#include "Simulator/FloodyFluidSimulator.h"
#include "Simulator/FluidSimulator.h"
#include "Simulator/FireSimulator.h"
#include "Simulator/NoopFluidSimulator.h"
#include "Simulator/NoopRedstoneSimulator.h"
#include "Simulator/SandSimulator.h"
#include "Simulator/IncrementalRedstoneSimulator.h"
#include "Simulator/VanillaFluidSimulator.h"
#include "Simulator/VaporizeFluidSimulator.h"

// Mobs:
#include "Mobs/IncludeAllMonsters.h"
#include "MobCensus.h"
#include "MobSpawner.h"

#include "Generating/Trees.h"
#include "Bindings/PluginManager.h"
#include "Blocks/BlockHandler.h"

#include "Tracer.h"

// DEBUG: Test out the cLineBlockTracer class by tracing a few lines:
#include "LineBlockTracer.h"

#ifndef _WIN32
	#include <stdlib.h>
#endif





const int TIME_SUNSET        = 12000;
const int TIME_NIGHT_START   = 13187;
const int TIME_NIGHT_END     = 22812;
const int TIME_SUNRISE       = 23999;
const int TIME_SPAWN_DIVISOR =   148;





////////////////////////////////////////////////////////////////////////////////
// cSpawnPrepare:

/** Generates and lights the spawn area of the world. Runs as a separate thread. */
class cSpawnPrepare:
	public cIsThread,
	public cChunkCoordCallback
{
	typedef cIsThread super;

public:
	cSpawnPrepare(cWorld & a_World, int a_SpawnChunkX, int a_SpawnChunkZ, int a_PrepareDistance):
		super("SpawnPrepare"),
		m_World(a_World),
		m_SpawnChunkX(a_SpawnChunkX),
		m_SpawnChunkZ(a_SpawnChunkZ),
		m_PrepareDistance(a_PrepareDistance),
		m_MaxIdx(a_PrepareDistance * a_PrepareDistance),
		m_NumPrepared(0),
		m_LastReportChunkCount(0)
	{
		// Start the thread:
		Start();

		// Wait for start confirmation, so that the thread can be waited-upon after the constructor returns:
		m_EvtStarted.Wait();
	}


	// cIsThread override:
	virtual void Execute(void) override
	{
		// Confirm thread start:
		m_EvtStarted.Set();

		// Queue the initial chunks:
		m_MaxIdx = m_PrepareDistance * m_PrepareDistance;
		int maxQueue = std::min(m_MaxIdx - 1, 100);  // Number of chunks to queue at once
		m_NextIdx = maxQueue;
		m_LastReportTime = std::chrono::steady_clock::now();
		for (int i = 0; i < maxQueue; i++)
		{
			int chunkX, chunkZ;
			DecodeChunkCoords(i, chunkX, chunkZ);
			m_World.PrepareChunk(chunkX, chunkZ, this);
		}  // for i

		// Wait for the lighting thread to prepare everything. Event is set in the Call() callback:
		m_EvtFinished.Wait();
	}

protected:
	cWorld & m_World;
	int m_SpawnChunkX;
	int m_SpawnChunkZ;
	int m_PrepareDistance;

	/** The index of the next chunk to be queued in the lighting thread. */
	int m_NextIdx;

	/** The maximum index of the prepared chunks. Queueing stops when m_NextIdx reaches this number. */
	int m_MaxIdx;

	/** Total number of chunks already finished preparing. Preparation finishes when this number reaches m_MaxIdx. */
	int m_NumPrepared;

	/** Event used to signal that the thread has started. */
	cEvent m_EvtStarted;

	/** Event used to signal that the preparation is finished. */
	cEvent m_EvtFinished;

	/** The timestamp of the last progress report emitted. */
	std::chrono::steady_clock::time_point m_LastReportTime;

	/** Number of chunks prepared when the last progress report was emitted. */
	int m_LastReportChunkCount;

	// cChunkCoordCallback override:
	virtual void Call(int a_ChunkX, int a_ChunkZ)
	{
		// Check if this was the last chunk:
		m_NumPrepared += 1;
		if (m_NumPrepared >= m_MaxIdx)
		{
			m_EvtFinished.Set();
			// Must return here, because "this" may have gotten deleted by the previous line
			return;
		}

		// Queue another chunk, if appropriate:
		if (m_NextIdx < m_MaxIdx)
		{
			int chunkX, chunkZ;
			DecodeChunkCoords(m_NextIdx, chunkX, chunkZ);
			m_World.GetLightingThread().QueueChunk(chunkX, chunkZ, this);
			m_NextIdx += 1;
		}

		// Report progress every 1 second:
		auto Now = std::chrono::steady_clock::now();
		if (Now - m_LastReportTime > std::chrono::seconds(1))
		{
			float PercentDone = static_cast<float>(m_NumPrepared * 100) / m_MaxIdx;
			float ChunkSpeed = static_cast<float>((m_NumPrepared - m_LastReportChunkCount) * 1000) / std::chrono::duration_cast<std::chrono::milliseconds>(Now - m_LastReportTime).count();
			LOG("Preparing spawn (%s): %.02f%% (%d/%d; %.02f chunks/s)",
				m_World.GetName().c_str(), PercentDone, m_NumPrepared, m_MaxIdx, ChunkSpeed
			);
			m_LastReportTime = Now;
			m_LastReportChunkCount = m_NumPrepared;
		}
	}


	/** Decodes the index into chunk coords. Provides the specific chunk ordering. */
	void DecodeChunkCoords(int a_Idx, int & a_ChunkX, int & a_ChunkZ)
	{
		// A zigzag pattern from the top to bottom, each row alternating between forward-x and backward-x:
		int z = a_Idx / m_PrepareDistance;
		int x = a_Idx % m_PrepareDistance;
		if ((z & 1) == 0)
		{
			// Reverse every second row:
			x = m_PrepareDistance - 1 - x;
		}
		a_ChunkZ = m_SpawnChunkZ + z - m_PrepareDistance / 2;
		a_ChunkX = m_SpawnChunkX + x - m_PrepareDistance / 2;
	}
};





////////////////////////////////////////////////////////////////////////////////
// cWorld::cLock:

cWorld::cLock::cLock(cWorld & a_World) :
	super(&(a_World.m_ChunkMap->GetCS()))
{
}





////////////////////////////////////////////////////////////////////////////////
// cWorld::cTickThread:

cWorld::cTickThread::cTickThread(cWorld & a_World) :
	super(Printf("WorldTickThread: %s", a_World.GetName().c_str())),
	m_World(a_World)
{
}





void cWorld::cTickThread::Execute(void)
{
	auto LastTime = std::chrono::steady_clock::now();
	auto TickTime = std::chrono::duration_cast<std::chrono::milliseconds>(cTickTime(1));

	while (!m_ShouldTerminate)
	{
		auto NowTime = std::chrono::steady_clock::now();
		auto WaitTime = std::chrono::duration_cast<std::chrono::milliseconds>(NowTime - LastTime);
		m_World.Tick(WaitTime, TickTime);
		TickTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - NowTime);
		
		if (TickTime < cTickTime(1))
		{
			// Stretch tick time until it's at least 1 tick
			std::this_thread::sleep_for(cTickTime(1) - TickTime);
		}

		LastTime = NowTime;
	}
}





////////////////////////////////////////////////////////////////////////////////
// cWorld:

cWorld::cWorld(const AString & a_WorldName, eDimension a_Dimension, const AString & a_OverworldName) :
	m_WorldName(a_WorldName),
	m_OverworldName(a_OverworldName),
	m_IniFileName(m_WorldName + "/world.ini"),
	m_StorageSchema("Default"),
#ifdef __arm__
	m_StorageCompressionFactor(0),
#else
	m_StorageCompressionFactor(6),
#endif
	m_Dimension(a_Dimension),
	m_IsSpawnExplicitlySet(false),
	m_SpawnX(0),
	m_SpawnY(0),
	m_SpawnZ(0),
	m_BroadcastDeathMessages(true),
	m_BroadcastAchievementMessages(true),
	m_IsDaylightCycleEnabled(true),
	m_WorldAge(0),
	m_TimeOfDay(0),
	m_LastTimeUpdate(0),
	m_LastUnload(0),
	m_LastSave(0),
	m_SkyDarkness(0),
	m_GameMode(gmNotSet),
	m_bEnabledPVP(false),
	m_IsDeepSnowEnabled(false),
	m_ShouldLavaSpawnFire(true),
	m_VillagersShouldHarvestCrops(true),
	m_SimulatorManager(),
	m_SandSimulator(),
	m_WaterSimulator(nullptr),
	m_LavaSimulator(nullptr),
	m_FireSimulator(),
	m_RedstoneSimulator(nullptr),
	m_MaxPlayers(10),
	m_ChunkMap(),
	m_bAnimals(true),
	m_Weather(eWeather_Sunny),
	m_WeatherInterval(24000),  // Guaranteed 1 day of sunshine at server start :)
	m_MaxCactusHeight(3),
	m_MaxSugarcaneHeight(4),
	m_IsCactusBonemealable(false),
	m_IsCarrotsBonemealable(true),
	m_IsCropsBonemealable(true),
	m_IsGrassBonemealable(true),
	m_IsMelonStemBonemealable(true),
	m_IsMelonBonemealable(true),
	m_IsPotatoesBonemealable(true),
	m_IsPumpkinStemBonemealable(true),
	m_IsPumpkinBonemealable(true),
	m_IsSaplingBonemealable(true),
	m_IsSugarcaneBonemealable(false),
	m_bCommandBlocksEnabled(true),
	m_bUseChatPrefixes(false),
	m_TNTShrapnelLevel(slNone),
	m_MaxViewDistance(12),
	m_Scoreboard(this),
	m_MapManager(this),
	m_GeneratorCallbacks(*this),
	m_TickThread(*this)
{
	LOGD("cWorld::cWorld(\"%s\")", a_WorldName.c_str());

	cFile::CreateFolder(FILE_IO_PREFIX + m_WorldName);

	// Load the scoreboard
	cScoreboardSerializer Serializer(m_WorldName, &m_Scoreboard);
	Serializer.Load();
}





cWorld::~cWorld()
{
	delete m_WaterSimulator;     m_WaterSimulator    = nullptr;
	delete m_LavaSimulator;      m_LavaSimulator     = nullptr;
	delete m_RedstoneSimulator;  m_RedstoneSimulator = nullptr;

	UnloadUnusedChunks();
	
	m_Storage.WaitForFinish();

	// Unload the scoreboard
	cScoreboardSerializer Serializer(m_WorldName, &m_Scoreboard);
	Serializer.Save();

	m_MapManager.SaveMapData();

	// Explicitly destroy the chunkmap, so that it's guaranteed to be destroyed before the other internals
	// This fixes crashes on stopping the server, because chunk destructor deletes entities and those access the world.
	m_ChunkMap.reset();
}





void cWorld::CastThunderbolt (int a_BlockX, int a_BlockY, int a_BlockZ)
{
	BroadcastThunderbolt(a_BlockX, a_BlockY, a_BlockZ);
}





int cWorld::GetDefaultWeatherInterval(eWeather a_Weather)
{
	switch (a_Weather)
	{
		case eWeather_Sunny:
		{
			return 14400 + (m_TickRand.randInt() % 4800);  // 12 - 16 minutes
		}
		case eWeather_Rain:
		{
			return 9600 + (m_TickRand.randInt() % 7200);   // 8 - 14 minutes
		}
		case eWeather_ThunderStorm:
		{
			return 2400 + (m_TickRand.randInt() % 4800);   // 2 - 6 minutes
		}
	}
	LOGWARNING("%s: Missing default weather interval for weather %d.", __FUNCTION__, a_Weather);
	return -1;
}





void cWorld::SetWeather(eWeather a_NewWeather)
{
	// Do the plugins agree? Do they want a different weather?
	if (cRoot::Get()->GetPluginManager()->CallHookWeatherChanging(*this, a_NewWeather))
	{
		m_WeatherInterval = GetDefaultWeatherInterval(m_Weather);
		return;
	}
	
	// Set new period for the selected weather:
	m_WeatherInterval = GetDefaultWeatherInterval(a_NewWeather);
	
	// The weather can't be found:
	if (m_WeatherInterval < 0)
	{
		return;
	}
	
	m_Weather = a_NewWeather;

	BroadcastWeather(m_Weather);

	// Send rain density to avoid blinking weather
	BroadcastRainDensity(m_RainDensity);

	// Let the plugins know about the change:
	cPluginManager::Get()->CallHookWeatherChanged(*this);
}


void cWorld::UpdateWeather(void)
{
	float LastRainDensity = m_RainDensity;
	// Changing speed
	float RainDensityAddend = IsWeatherRain() ? 0.01f : -0.01f;
	m_RainDensity = std::max(0.0f, std::min(1.0f, m_RainDensity + RainDensityAddend));

	// Send rain density when it changed
	if (m_RainDensity != LastRainDensity)
	{
		BroadcastRainDensity(m_RainDensity);
	}

	// TODO: when is thundering, smooth sky darkness as well
	// Light level falls to 10 during daytime. Like rain, thunderstorms won't reduce the light level below 4 at night. [minecraft.gamepedia.com/Thunder]
}


void cWorld::ChangeWeather(void)
{
	// In the next tick the weather will be changed
	m_WeatherInterval = 0;
}





void cWorld::SetNextBlockTick(int a_BlockX, int a_BlockY, int a_BlockZ)
{
	return m_ChunkMap->SetNextBlockTick(a_BlockX, a_BlockY, a_BlockZ);
}





void cWorld::InitializeSpawn(void)
{
	if (!m_IsSpawnExplicitlySet)
	{
		// Spawn position wasn't already explicitly set, enerate random solid-land coordinate and then write it to the world configuration:
		GenerateRandomSpawn();
		cIniFile IniFile;
		IniFile.ReadFile(m_IniFileName);
		IniFile.SetValueF("SpawnPosition", "X", m_SpawnX);
		IniFile.SetValueF("SpawnPosition", "Y", m_SpawnY);
		IniFile.SetValueF("SpawnPosition", "Z", m_SpawnZ);
		IniFile.WriteFile(m_IniFileName);
	}

	int ChunkX = 0, ChunkZ = 0;
	cChunkDef::BlockToChunk((int)m_SpawnX, (int)m_SpawnZ, ChunkX, ChunkZ);
	
	// For the debugging builds, don't make the server build too much world upon start:
	#if defined(_DEBUG) || defined(ANDROID_NDK)
		const int DefaultViewDist = 9;
	#else
		const int DefaultViewDist = 20;  // Always prepare an area 20 chunks across, no matter what the actual cClientHandle::VIEWDISTANCE is
	#endif  // _DEBUG
	cIniFile IniFile;
	IniFile.ReadFile(m_IniFileName);
	int ViewDist = IniFile.GetValueSetI("SpawnPosition", "PregenerateDistance", DefaultViewDist);
	IniFile.WriteFile(m_IniFileName);

	cSpawnPrepare prep(*this, ChunkX, ChunkZ, ViewDist);
	prep.Wait();
	
	#ifdef TEST_LINEBLOCKTRACER
	// DEBUG: Test out the cLineBlockTracer class by tracing a few lines:
	class cTracerCallbacks :
		public cBlockTracer::cCallbacks
	{
		virtual bool OnNextBlock(int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta) override
		{
			LOGD("Block {%d, %d, %d}: %d:%d (%s)",
				a_BlockX, a_BlockY, a_BlockZ, a_BlockType, a_BlockMeta,
				ItemToString(cItem(a_BlockType, 1, a_BlockMeta)).c_str()
			);
			return false;
		}
		
		virtual bool OnNextBlockNoData(int a_BlockX, int a_BlockY, int a_BlockZ) override
		{
			LOGD("Block {%d, %d, %d}: no data available",
				a_BlockX, a_BlockY, a_BlockZ
			);
			return false;
		}

		virtual bool OnOutOfWorld(double a_BlockX, double a_BlockY, double a_BlockZ) override
		{
			LOGD("Out of world at {%f, %f, %f}", a_BlockX, a_BlockY, a_BlockZ);
			return false;
		}

		virtual bool OnIntoWorld(double a_BlockX, double a_BlockY, double a_BlockZ) override
		{
			LOGD("Into world at {%f, %f, %f}", a_BlockX, a_BlockY, a_BlockZ);
			return false;
		}

		virtual void OnNoMoreHits(void) override
		{
			LOGD("No more hits");
		}
	} Callbacks;
	LOGD("Spawn is at {%f, %f, %f}", m_SpawnX, m_SpawnY, m_SpawnZ);
	LOGD("Tracing a line along +X:");
	cLineBlockTracer::Trace(*this, Callbacks, m_SpawnX - 10, m_SpawnY, m_SpawnZ, m_SpawnX + 10, m_SpawnY, m_SpawnZ);
	LOGD("Tracing a line along -Z:");
	cLineBlockTracer::Trace(*this, Callbacks, m_SpawnX, m_SpawnY, m_SpawnZ + 10, m_SpawnX, m_SpawnY, m_SpawnZ - 10);
	LOGD("Tracing a line along -Y, out of world:");
	cLineBlockTracer::Trace(*this, Callbacks, m_SpawnX, 260, m_SpawnZ, m_SpawnX, -5, m_SpawnZ);
	LOGD("Tracing a line along XY:");
	cLineBlockTracer::Trace(*this, Callbacks, m_SpawnX - 10, m_SpawnY - 10, m_SpawnZ, m_SpawnX + 10, m_SpawnY + 10, m_SpawnZ);
	LOGD("Tracing a line in generic direction:");
	cLineBlockTracer::Trace(*this, Callbacks, m_SpawnX - 15, m_SpawnY - 5, m_SpawnZ + 7.5, m_SpawnX + 13, m_SpawnY - 10, m_SpawnZ + 8.5);
	LOGD("Tracing tests done");
	#endif  // TEST_LINEBLOCKTRACER
}





void cWorld::Start(void)
{
	m_SpawnX = 0;
	m_SpawnY = cChunkDef::Height;
	m_SpawnZ = 0;
	m_GameMode = eGameMode_Creative;

	cIniFile IniFile;
	if (!IniFile.ReadFile(m_IniFileName))
	{
		LOGWARNING("Cannot read world settings from \"%s\", defaults will be used.", m_IniFileName.c_str());

		// TODO: More descriptions for each key
		IniFile.AddHeaderComment(" This is the per-world configuration file, managing settings such as generators, simulators, and spawn points");
		IniFile.AddKeyComment(" LinkedWorlds", "This section governs portal world linkage; leave a value blank to disabled that associated method of teleportation");
	}

	// The presence of a configuration value overrides everything
	// If no configuration value is found, GetDimension() is written to file and the variable is written to again to ensure that cosmic rays haven't sneakily changed its value
	m_Dimension = StringToDimension(IniFile.GetValueSet("General", "Dimension", DimensionToString(GetDimension())));

	m_BroadcastDeathMessages = IniFile.GetValueSetB("Broadcasting", "BroadcastDeathMessages", true);
	m_BroadcastAchievementMessages = IniFile.GetValueSetB("Broadcasting", "BroadcastAchievementMessages", true);

	SetMaxViewDistance(IniFile.GetValueSetI("SpawnPosition", "MaxViewDistance", 12));

	// Try to find the "SpawnPosition" key and coord values in the world configuration, set the flag if found
	int KeyNum = IniFile.FindKey("SpawnPosition");
	m_IsSpawnExplicitlySet =
	(
		(KeyNum >= 0) &&
		(
			(IniFile.FindValue(KeyNum, "X") >= 0) &&
			(IniFile.FindValue(KeyNum, "Y") >= 0) &&
			(IniFile.FindValue(KeyNum, "Z") >= 0)
		)
	);

	if (m_IsSpawnExplicitlySet)
	{
		LOGD("Spawnpoint explicitly set!");
		m_SpawnX = IniFile.GetValueF("SpawnPosition", "X", m_SpawnX);
		m_SpawnY = IniFile.GetValueF("SpawnPosition", "Y", m_SpawnY);
		m_SpawnZ = IniFile.GetValueF("SpawnPosition", "Z", m_SpawnZ);
	}

	m_StorageSchema               = IniFile.GetValueSet ("Storage",       "Schema",                      m_StorageSchema);
	m_StorageCompressionFactor    = IniFile.GetValueSetI("Storage",       "CompressionFactor",           m_StorageCompressionFactor);
	m_MaxCactusHeight             = IniFile.GetValueSetI("Plants",        "MaxCactusHeight",             3);
	m_MaxSugarcaneHeight          = IniFile.GetValueSetI("Plants",        "MaxSugarcaneHeight",          3);
	m_IsCactusBonemealable        = IniFile.GetValueSetB("Plants",        "IsCactusBonemealable",        false);
	m_IsCarrotsBonemealable       = IniFile.GetValueSetB("Plants",        "IsCarrotsBonemealable",       true);
	m_IsCropsBonemealable         = IniFile.GetValueSetB("Plants",        "IsCropsBonemealable",         true);
	m_IsGrassBonemealable         = IniFile.GetValueSetB("Plants",        "IsGrassBonemealable",         true);
	m_IsMelonStemBonemealable     = IniFile.GetValueSetB("Plants",        "IsMelonStemBonemealable",     true);
	m_IsMelonBonemealable         = IniFile.GetValueSetB("Plants",        "IsMelonBonemealable",         false);
	m_IsPotatoesBonemealable      = IniFile.GetValueSetB("Plants",        "IsPotatoesBonemealable",      true);
	m_IsPumpkinStemBonemealable   = IniFile.GetValueSetB("Plants",        "IsPumpkinStemBonemealable",   true);
	m_IsPumpkinBonemealable       = IniFile.GetValueSetB("Plants",        "IsPumpkinBonemealable",       false);
	m_IsSaplingBonemealable       = IniFile.GetValueSetB("Plants",        "IsSaplingBonemealable",       true);
	m_IsSugarcaneBonemealable     = IniFile.GetValueSetB("Plants",        "IsSugarcaneBonemealable",     false);
	m_IsDeepSnowEnabled           = IniFile.GetValueSetB("Physics",       "DeepSnow",                    true);
	m_ShouldLavaSpawnFire         = IniFile.GetValueSetB("Physics",       "ShouldLavaSpawnFire",         true);
	int TNTShrapnelLevel          = IniFile.GetValueSetI("Physics",       "TNTShrapnelLevel",            (int)slAll);
	m_bCommandBlocksEnabled       = IniFile.GetValueSetB("Mechanics",     "CommandBlocksEnabled",        false);
	m_bEnabledPVP                 = IniFile.GetValueSetB("Mechanics",     "PVPEnabled",                  true);
	m_bUseChatPrefixes            = IniFile.GetValueSetB("Mechanics",     "UseChatPrefixes",             true);
	m_VillagersShouldHarvestCrops = IniFile.GetValueSetB("Monsters",      "VillagersShouldHarvestCrops", true);
	m_IsDaylightCycleEnabled      = IniFile.GetValueSetB("General",       "IsDaylightCycleEnabled",      true);
	int GameMode                  = IniFile.GetValueSetI("General",       "Gamemode",                    (int)m_GameMode);
	int Weather                   = IniFile.GetValueSetI("General",       "Weather",                     (int)m_Weather);
	
	if (GetDimension() == dimOverworld)
	{
		m_NetherWorldName = IniFile.GetValueSet("LinkedWorlds", "NetherWorldName", GetName() + "_nether");
		m_EndWorldName = IniFile.GetValueSet("LinkedWorlds", "EndWorldName", GetName() + "_end");
	}
	else
	{
		m_OverworldName = IniFile.GetValueSet("LinkedWorlds", "OverworldName", GetLinkedOverworldName());
	}
	
	// Adjust the enum-backed variables into their respective bounds:
	m_GameMode         = (eGameMode)     Clamp(GameMode,         (int)gmSurvival, (int)gmSpectator);
	m_TNTShrapnelLevel = (eShrapnelLevel)Clamp(TNTShrapnelLevel, (int)slNone,     (int)slAll);
	m_Weather          = (eWeather)      Clamp(Weather,          (int)wSunny,     (int)wStorm);

	InitialiseGeneratorDefaults(IniFile);
	InitialiseAndLoadMobSpawningValues(IniFile);
	SetTimeOfDay(IniFile.GetValueSetI("General", "TimeInTicks", GetTimeOfDay()));

	m_ChunkMap = make_unique<cChunkMap>(this);
	
	// preallocate some memory for ticking blocks so we don't need to allocate that often
	m_BlockTickQueue.reserve(1000);
	m_BlockTickQueueCopy.reserve(1000);

	// Simulators:
	m_SimulatorManager  = make_unique<cSimulatorManager>(*this);
	m_WaterSimulator    = InitializeFluidSimulator(IniFile, "Water", E_BLOCK_WATER, E_BLOCK_STATIONARY_WATER);
	m_LavaSimulator     = InitializeFluidSimulator(IniFile, "Lava",  E_BLOCK_LAVA,  E_BLOCK_STATIONARY_LAVA);
	m_SandSimulator     = make_unique<cSandSimulator>(*this, IniFile);
	m_FireSimulator     = make_unique<cFireSimulator>(*this, IniFile);
	m_RedstoneSimulator = InitializeRedstoneSimulator(IniFile);

	// Water, Lava and Redstone simulators get registered in their initialize function.
	m_SimulatorManager->RegisterSimulator(m_SandSimulator.get(), 1);
	m_SimulatorManager->RegisterSimulator(m_FireSimulator.get(), 1);

	m_Lighting.Start(this);
	m_Storage.Start(this, m_StorageSchema, m_StorageCompressionFactor);
	m_Generator.Start(m_GeneratorCallbacks, m_GeneratorCallbacks, IniFile);
	m_ChunkSender.Start(this);
	m_TickThread.Start();

	// Init of the spawn monster time (as they are supposed to have different spawn rate)
	m_LastSpawnMonster.insert(std::map<cMonster::eFamily, cTickTimeLong>::value_type(cMonster::mfHostile, cTickTimeLong(0)));
	m_LastSpawnMonster.insert(std::map<cMonster::eFamily, cTickTimeLong>::value_type(cMonster::mfPassive, cTickTimeLong(0)));
	m_LastSpawnMonster.insert(std::map<cMonster::eFamily, cTickTimeLong>::value_type(cMonster::mfAmbient, cTickTimeLong(0)));
	m_LastSpawnMonster.insert(std::map<cMonster::eFamily, cTickTimeLong>::value_type(cMonster::mfWater, cTickTimeLong(0)));

	m_MapManager.LoadMapData();

	// Save any changes that the defaults may have done to the ini file:
	if (!IniFile.WriteFile(m_IniFileName))
	{
		LOGWARNING("Could not write world config to %s", m_IniFileName.c_str());
	}
}





void cWorld::GenerateRandomSpawn(void)
{
	LOGD("Generating random spawnpoint...");

	// Look for a spawn point at most 100 chunks away from map center:
	for (int i = 0; i < 100; i++)
	{
		EMCSBiome biome = GetBiomeAt((int)m_SpawnX, (int)m_SpawnZ);
		if (
			(biome != biOcean) && (biome != biFrozenOcean) &&  // The biome is acceptable (don't want a small ocean island)
			!IsBlockWaterOrIce(GetBlock((int)m_SpawnX, GetHeight((int)m_SpawnX, (int)m_SpawnZ), (int)m_SpawnZ))  // The terrain is acceptable (don't want to spawn inside a lake / river)
		)
		{
			// A good spawnpoint was found
			break;
		}
		// Try a neighboring chunk:
		if ((GetTickRandomNumber(4) % 2) == 0)  // Randomise whether to increment X or Z coords
		{
			m_SpawnX += cChunkDef::Width;
		}
		else
		{
			m_SpawnZ += cChunkDef::Width;
		}
	}  // for i - 100*

	m_SpawnY = (double)GetHeight((int)m_SpawnX, (int)m_SpawnZ) + 1.6f;  // 1.6f to accomodate player height

	LOGINFO("Generated random spawnpoint position {%i, %i, %i}", (int)m_SpawnX, (int)m_SpawnY, (int)m_SpawnZ);
}





eWeather cWorld::ChooseNewWeather()
{
	// Pick a new weather. Only reasonable transitions allowed:
	switch (m_Weather)
	{
		case eWeather_Sunny:
		case eWeather_ThunderStorm: return eWeather_Rain;
			
		case eWeather_Rain:
		{
			// 1/8 chance of turning into a thunderstorm
			return ((m_TickRand.randInt() % 256) < 32) ? eWeather_ThunderStorm : eWeather_Sunny;
		}
	}
	
	LOGWARNING("Unknown current weather: %d. Setting sunny.", m_Weather);
	ASSERT(!"Unknown weather");
	return eWeather_Sunny;
}





void cWorld::InitialiseGeneratorDefaults(cIniFile & a_IniFile)
{
	switch (GetDimension())
	{
		case dimEnd:
		{
			a_IniFile.GetValueSet("Generator", "Generator",      "Composable");
			a_IniFile.GetValueSet("Generator", "BiomeGen",       "Constant");
			a_IniFile.GetValueSet("Generator", "ConstantBiome",  "End");
			a_IniFile.GetValueSet("Generator", "ShapeGen",       "End");
			a_IniFile.GetValueSet("Generator", "CompositionGen", "End");
			break;
		}
		case dimOverworld:
		{
			a_IniFile.GetValueSet("Generator", "Generator",      "Composable");
			a_IniFile.GetValueSet("Generator", "BiomeGen",       "Grown");
			a_IniFile.GetValueSet("Generator", "ShapeGen",       "BiomalNoise3D");
			a_IniFile.GetValueSet("Generator", "CompositionGen", "Biomal");
			a_IniFile.GetValueSet("Generator", "Finishers",      "Ravines, WormNestCaves, WaterLakes, WaterSprings, LavaLakes, LavaSprings, OreNests, Mineshafts, Trees, Villages, SprinkleFoliage, Ice, Snow, Lilypads, BottomLava, DeadBushes, NaturalPatches, PreSimulator, Animals");
			break;
		}
		case dimNether:
		{
			a_IniFile.GetValueSet("Generator", "Generator",        "Composable");
			a_IniFile.GetValueSet("Generator", "BiomeGen",         "Constant");
			a_IniFile.GetValueSet("Generator", "ConstantBiome",    "Nether");
			a_IniFile.GetValueSet("Generator", "ShapeGen",         "HeightMap");
			a_IniFile.GetValueSet("Generator", "HeightGen",        "Flat");
			a_IniFile.GetValueSet("Generator", "FlatHeight",       "128");
			a_IniFile.GetValueSet("Generator", "CompositionGen",   "Nether");
			a_IniFile.GetValueSet("Generator", "Finishers",        "SoulsandRims, WormNestCaves, BottomLava, LavaSprings, NetherClumpFoliage, NetherOreNests, NetherForts, PreSimulator");
			a_IniFile.GetValueSet("Generator", "BottomLavaHeight", "30");
			break;
		}
		case dimNotSet:
		{
			ASSERT(!"Dimension not set");
			break;
		}
	}
}





void cWorld::InitialiseAndLoadMobSpawningValues(cIniFile & a_IniFile)
{
	AString DefaultMonsters;
	switch (m_Dimension)
	{
		case dimOverworld: DefaultMonsters = "bat, cavespider, chicken, cow, creeper, enderman, horse, mooshroom, ocelot, pig, sheep, silverfish, skeleton, slime, spider, squid, wolf, zombie"; break;
		case dimNether:    DefaultMonsters = "blaze, ghast, magmacube, skeleton, zombie, zombiepigman"; break;
		case dimEnd:       DefaultMonsters = "enderman"; break;
		case dimNotSet:    ASSERT(!"Dimension not set"); break;
	}
	
	m_bAnimals = a_IniFile.GetValueSetB("Monsters", "AnimalsOn", true);
	AString AllMonsters = a_IniFile.GetValueSet("Monsters", "Types", DefaultMonsters);

	if (!m_bAnimals)
	{
		return;
	}

	AStringVector SplitList = StringSplitAndTrim(AllMonsters, ",");
	for (AStringVector::const_iterator itr = SplitList.begin(), end = SplitList.end(); itr != end; ++itr)
	{
		eMonsterType ToAdd = cMonster::StringToMobType(*itr);
		if (ToAdd != mtInvalidType)
		{
			m_AllowedMobs.insert(ToAdd);
			LOGD("Allowed mob: %s", itr->c_str());
		}
		else
		{
			LOG("World \"%s\": Unknown mob type: %s", m_WorldName.c_str(), itr->c_str());
		}
	}
}





void cWorld::Stop(void)
{
	// Delete the clients that have been in this world:
	{
		cCSLock Lock(m_CSClients);
		for (auto itr = m_Clients.begin(); itr != m_Clients.end(); ++itr)
		{
			(*itr)->Destroy();
		}  // for itr - m_Clients[]
		m_Clients.clear();
	}

	// Write settings to file; these are all plugin changeable values - keep updated!
	cIniFile IniFile;
	IniFile.ReadFile(m_IniFileName);
		if (GetDimension() == dimOverworld)
		{
			IniFile.SetValue("LinkedWorlds", "NetherWorldName", m_NetherWorldName);
			IniFile.SetValue("LinkedWorlds", "EndWorldName", m_EndWorldName);
		}
		else
		{
			IniFile.SetValue("LinkedWorlds", "OverworldName", m_OverworldName);
		}
		IniFile.SetValueI("Physics", "TNTShrapnelLevel", (int)m_TNTShrapnelLevel);
		IniFile.SetValueB("Mechanics", "CommandBlocksEnabled", m_bCommandBlocksEnabled);
		IniFile.SetValueB("Mechanics", "UseChatPrefixes", m_bUseChatPrefixes);
		IniFile.SetValueB("General", "IsDaylightCycleEnabled", m_IsDaylightCycleEnabled);
		IniFile.SetValueI("General", "Weather", (int)m_Weather);
		IniFile.SetValueI("General", "TimeInTicks", GetTimeOfDay());
	IniFile.WriteFile(m_IniFileName);
	
	m_TickThread.Stop();
	m_Lighting.Stop();
	m_Generator.Stop();
	m_ChunkSender.Stop();
	m_Storage.Stop();
}





void cWorld::Tick(std::chrono::milliseconds a_Dt, std::chrono::milliseconds a_LastTickDurationMSec)
{
	// Call the plugins
	cPluginManager::Get()->CallHookWorldTick(*this, a_Dt, a_LastTickDurationMSec);
	
	// Set any chunk data that has been queued for setting:
	cSetChunkDataPtrs SetChunkDataQueue;
	{
		cCSLock Lock(m_CSSetChunkDataQueue);
		std::swap(SetChunkDataQueue, m_SetChunkDataQueue);
	}
	for (cSetChunkDataPtrs::iterator itr = SetChunkDataQueue.begin(), end = SetChunkDataQueue.end(); itr != end; ++itr)
	{
		SetChunkData(**itr);
	}  // for itr - SetChunkDataQueue[]

	m_WorldAge += a_Dt;

	if (m_IsDaylightCycleEnabled)
	{
		// We need sub-tick precision here, that's why we store the time in milliseconds and calculate ticks off of it
		m_TimeOfDay += a_Dt;

		// Wrap time of day each 20 minutes (1200 seconds)
		if (m_TimeOfDay > std::chrono::minutes(20))
		{
			m_TimeOfDay -= std::chrono::minutes(20);
		}

		// Updates the sky darkness based on current time of day
		UpdateSkyDarkness();

		// Broadcast time update every 40 ticks (2 seconds)
		if (m_LastTimeUpdate < m_WorldAge - cTickTime(40))
		{
			BroadcastTimeUpdate();
			m_LastTimeUpdate = std::chrono::duration_cast<cTickTimeLong>(m_WorldAge);
		}
	}

	// Add entities waiting in the queue to be added:
	{
		cCSLock Lock(m_CSEntitiesToAdd);
		for (cEntityList::iterator itr = m_EntitiesToAdd.begin(), end = m_EntitiesToAdd.end(); itr != end; ++itr)
		{
			(*itr)->SetWorld(this);
			m_ChunkMap->AddEntity(*itr);
		}
		m_EntitiesToAdd.clear();
	}

	// Add players waiting in the queue to be added:
	AddQueuedPlayers();

	m_ChunkMap->Tick(a_Dt);

	TickClients(static_cast<float>(a_Dt.count()));
	TickQueuedBlocks();
	TickQueuedTasks();
	TickScheduledTasks();
	
	GetSimulatorManager()->Simulate(static_cast<float>(a_Dt.count()));

	TickWeather(static_cast<float>(a_Dt.count()));

	m_ChunkMap->FastSetQueuedBlocks();

	if (m_WorldAge - m_LastSave > std::chrono::minutes(5))  // Save each 5 minutes
	{
		SaveAllChunks();
	}

	if (m_WorldAge - m_LastUnload > std::chrono::minutes(5))  // Unload every 10 seconds
	{
		UnloadUnusedChunks();
	}

	TickMobs(a_Dt);
}





void cWorld::TickWeather(float a_Dt)
{
	UNUSED(a_Dt);

	UpdateWeather();

	// There are no weather changes anywhere but in the Overworld:
	if (GetDimension() != dimOverworld)
	{
		return;
	}

	if (m_WeatherInterval > 0)
	{
		// Not yet, wait for the weather period to end
		m_WeatherInterval--;
	}
	else
	{
		// Change weather:
		SetWeather(ChooseNewWeather());
	}

	if (m_Weather == eWeather_ThunderStorm)
	{
		// 0.5% chance per tick of thunderbolt
		if (m_TickRand.randInt() % 199 == 0)
		{
			CastThunderbolt(0, 0, 0);  // TODO: find random possitions near players to cast thunderbolts.
		}
	}
}





void cWorld::TickMobs(std::chrono::milliseconds a_Dt)
{
	// _X 2013_10_22: This is a quick fix for #283 - the world needs to be locked while ticking mobs
	cWorld::cLock Lock(*this);

	// before every Mob action, we have to count them depending on the distance to players, on their family ...
	cMobCensus MobCensus;
	m_ChunkMap->CollectMobCensus(MobCensus);
	if (m_bAnimals)
	{
		// Spawning is enabled, spawn now:
		static const cMonster::eFamily AllFamilies[] =
		{
			cMonster::mfHostile,
			cMonster::mfPassive,
			cMonster::mfAmbient,
			cMonster::mfWater,
		} ;
		for (size_t i = 0; i < ARRAYCOUNT(AllFamilies); i++)
		{
			cMonster::eFamily Family = AllFamilies[i];
			cTickTime SpawnDelay = cTickTime(cMonster::GetSpawnDelay(Family));
			if (
				(m_LastSpawnMonster[Family] > m_WorldAge - SpawnDelay) ||  // Not reached the needed ticks before the next round
				MobCensus.IsCapped(Family)
			)
			{
				continue;
			}
			m_LastSpawnMonster[Family] = std::chrono::duration_cast<cTickTimeLong>(m_WorldAge);
			cMobSpawner Spawner(Family, m_AllowedMobs);
			if (Spawner.CanSpawnAnything())
			{
				m_ChunkMap->SpawnMobs(Spawner);
				// do the spawn
				for (cMobSpawner::tSpawnedContainer::const_iterator itr2 = Spawner.getSpawned().begin(); itr2 != Spawner.getSpawned().end(); ++itr2)
				{
					SpawnMobFinalize(*itr2);
				}
			}
		}  // for i - AllFamilies[]
	}  // if (Spawning enabled)

	// move close mobs
	cMobProximityCounter::sIterablePair allCloseEnoughToMoveMobs = MobCensus.GetProximityCounter().getMobWithinThosesDistances(-1, 64 * 16);// MG TODO : deal with this magic number (the 16 is the size of a block)
	for (cMobProximityCounter::tDistanceToMonster::const_iterator itr = allCloseEnoughToMoveMobs.m_Begin; itr != allCloseEnoughToMoveMobs.m_End; ++itr)
	{
		itr->second.m_Monster.Tick(a_Dt, itr->second.m_Chunk);
	}

	// remove too far mobs
	cMobProximityCounter::sIterablePair allTooFarMobs = MobCensus.GetProximityCounter().getMobWithinThosesDistances(128 * 16, -1);// MG TODO : deal with this magic number (the 16 is the size of a block)
	for (cMobProximityCounter::tDistanceToMonster::const_iterator itr = allTooFarMobs.m_Begin; itr != allTooFarMobs.m_End; ++itr)
	{
		itr->second.m_Monster.Destroy(true);
	}
}





void cWorld::TickQueuedTasks(void)
{
	// Make a copy of the tasks to avoid deadlocks on accessing m_Tasks
	cTasks Tasks;
	{
		cCSLock Lock(m_CSTasks);
		std::swap(Tasks, m_Tasks);
	}

	// Execute and delete each task:
	for (cTasks::iterator itr = Tasks.begin(), end = Tasks.end(); itr != end; ++itr)
	{
		(*itr)->Run(*this);
	}  // for itr - m_Tasks[]
}





void cWorld::TickScheduledTasks(void)
{
	// Move the tasks to be executed to a seperate vector to avoid deadlocks on accessing m_Tasks
	cScheduledTasks Tasks;
	{
		cCSLock Lock(m_CSScheduledTasks);
		auto WorldAge = m_WorldAge;

		// Move all the due tasks from m_ScheduledTasks into Tasks:
		for (auto itr = m_ScheduledTasks.begin(); itr != m_ScheduledTasks.end();)  // Cannot use range-basd for, we're modifying the container
		{
			if ((*itr)->m_TargetTick < std::chrono::duration_cast<cTickTimeLong>(WorldAge).count())
			{
				auto next = itr;
				++next;
				Tasks.push_back(std::move(*itr));
				m_ScheduledTasks.erase(itr);
				itr = next;
			}
			else
			{
				// All the eligible tasks have been moved, bail out now
				break;
			}
		}
	}

	// Execute and delete each task:
	for (cScheduledTasks::iterator itr = Tasks.begin(), end = Tasks.end(); itr != end; ++itr)
	{
		(*itr)->m_Task->Run(*this);
	}  // for itr - m_Tasks[]
}




void cWorld::TickClients(float a_Dt)
{
	cClientHandlePtrs RemoveClients;
	{
		cCSLock Lock(m_CSClients);
		
		// Remove clients scheduled for removal:
		for (auto itr = m_ClientsToRemove.begin(), end = m_ClientsToRemove.end(); itr != end; ++itr)
		{
			for (auto itrC = m_Clients.begin(), endC = m_Clients.end(); itrC != endC; ++itrC)
			{
				if (itrC->get() == *itr)
				{
					m_Clients.erase(itrC);
					break;
				}
			}
		}  // for itr - m_ClientsToRemove[]
		m_ClientsToRemove.clear();
		
		// Add clients scheduled for adding:
		for (auto itr = m_ClientsToAdd.begin(), end = m_ClientsToAdd.end(); itr != end; ++itr)
		{
			ASSERT(std::find(m_Clients.begin(), m_Clients.end(), *itr) == m_Clients.end());
			m_Clients.push_back(*itr);
		}  // for itr - m_ClientsToRemove[]
		m_ClientsToAdd.clear();
		
		// Tick the clients, take out those that have been destroyed into RemoveClients
		for (auto itr = m_Clients.begin(); itr != m_Clients.end();)
		{
			if ((*itr)->IsDestroyed())
			{
				// Remove the client later, when CS is not held, to avoid deadlock
				RemoveClients.push_back(*itr);
				itr = m_Clients.erase(itr);
				continue;
			}
			(*itr)->Tick(a_Dt);
			++itr;
		}  // for itr - m_Clients[]
	}

	// Delete the clients queued for removal:
	RemoveClients.clear();
}





void cWorld::UpdateSkyDarkness(void)
{
	int TempTime = std::chrono::duration_cast<cTickTime>(m_TimeOfDay).count();
	if (TempTime <= TIME_SUNSET)
	{
		m_SkyDarkness = 0;
	}
	else if (TempTime <= TIME_NIGHT_START)
	{
		m_SkyDarkness = static_cast<NIBBLETYPE>((TIME_NIGHT_START - TempTime) / TIME_SPAWN_DIVISOR);
	}
	else if (TempTime <= TIME_NIGHT_END)
	{
		m_SkyDarkness = 8;
	}
	else
	{
		m_SkyDarkness = static_cast<NIBBLETYPE>((TIME_SUNRISE - TempTime) / TIME_SPAWN_DIVISOR);
	}

}





void cWorld::WakeUpSimulators(int a_BlockX, int a_BlockY, int a_BlockZ)
{
	return m_ChunkMap->WakeUpSimulators(a_BlockX, a_BlockY, a_BlockZ);
}





/// Wakes up the simulators for the specified area of blocks
void cWorld::WakeUpSimulatorsInArea(int a_MinBlockX, int a_MaxBlockX, int a_MinBlockY, int a_MaxBlockY, int a_MinBlockZ, int a_MaxBlockZ)
{
	return m_ChunkMap->WakeUpSimulatorsInArea(a_MinBlockX, a_MaxBlockX, a_MinBlockY, a_MaxBlockY, a_MinBlockZ, a_MaxBlockZ);
}





bool cWorld::ForEachBlockEntityInChunk(int a_ChunkX, int a_ChunkZ, cBlockEntityCallback & a_Callback)
{
	return m_ChunkMap->ForEachBlockEntityInChunk(a_ChunkX, a_ChunkZ, a_Callback);
}





bool cWorld::ForEachChestInChunk(int a_ChunkX, int a_ChunkZ, cChestCallback & a_Callback)
{
	return m_ChunkMap->ForEachChestInChunk(a_ChunkX, a_ChunkZ, a_Callback);
}





bool cWorld::ForEachDispenserInChunk(int a_ChunkX, int a_ChunkZ, cDispenserCallback & a_Callback)
{
	return m_ChunkMap->ForEachDispenserInChunk(a_ChunkX, a_ChunkZ, a_Callback);
}





bool cWorld::ForEachDropperInChunk(int a_ChunkX, int a_ChunkZ, cDropperCallback & a_Callback)
{
	return m_ChunkMap->ForEachDropperInChunk(a_ChunkX, a_ChunkZ, a_Callback);
}





bool cWorld::ForEachDropSpenserInChunk(int a_ChunkX, int a_ChunkZ, cDropSpenserCallback & a_Callback)
{
	return m_ChunkMap->ForEachDropSpenserInChunk(a_ChunkX, a_ChunkZ, a_Callback);
}





bool cWorld::ForEachFurnaceInChunk(int a_ChunkX, int a_ChunkZ, cFurnaceCallback & a_Callback)
{
	return m_ChunkMap->ForEachFurnaceInChunk(a_ChunkX, a_ChunkZ, a_Callback);
}





void cWorld::DoExplosionAt(double a_ExplosionSize, double a_BlockX, double a_BlockY, double a_BlockZ, bool a_CanCauseFire, eExplosionSource a_Source, void * a_SourceData)
{
	if (cPluginManager::Get()->CallHookExploding(*this, a_ExplosionSize, a_CanCauseFire, a_BlockX, a_BlockY, a_BlockZ, a_Source, a_SourceData) || (a_ExplosionSize <= 0))
	{
		return;
	}
	
	// TODO: Implement block hardiness
	Vector3d explosion_pos = Vector3d(a_BlockX, a_BlockY, a_BlockZ);
	cVector3iArray BlocksAffected;
	m_ChunkMap->DoExplosionAt(a_ExplosionSize, a_BlockX, a_BlockY, a_BlockZ, BlocksAffected);
	BroadcastSoundEffect("random.explode", (double)a_BlockX, (double)a_BlockY, (double)a_BlockZ, 1.0f, 0.6f);

	{
		cCSLock Lock(m_CSPlayers);
		for (cPlayerList::iterator itr = m_Players.begin(); itr != m_Players.end(); ++itr)
		{
			cClientHandle * ch = (*itr)->GetClientHandle();
			if (ch == nullptr)
			{
				continue;
			}

			Vector3d distance_explosion = (*itr)->GetPosition() - explosion_pos;
			if (distance_explosion.SqrLength() < 4096.0)
			{
				double real_distance = std::max(0.004, distance_explosion.Length());
				double power = a_ExplosionSize / real_distance;
				if (power <= 1)
				{
					power = 0;
				}
				distance_explosion.Normalize();
				distance_explosion *= power;
				ch->SendExplosion(a_BlockX, a_BlockY, a_BlockZ, (float)a_ExplosionSize, BlocksAffected, distance_explosion);
			}
		}
	}

	cPluginManager::Get()->CallHookExploded(*this, a_ExplosionSize, a_CanCauseFire, a_BlockX, a_BlockY, a_BlockZ, a_Source, a_SourceData);
}





bool cWorld::DoWithBlockEntityAt(int a_BlockX, int a_BlockY, int a_BlockZ, cBlockEntityCallback & a_Callback)
{
	return m_ChunkMap->DoWithBlockEntityAt(a_BlockX, a_BlockY, a_BlockZ, a_Callback);
}





bool cWorld::DoWithBeaconAt(int a_BlockX, int a_BlockY, int a_BlockZ, cBeaconCallback& a_Callback)
{
	return m_ChunkMap->DoWithBeaconAt(a_BlockX, a_BlockY, a_BlockZ, a_Callback);
}





bool cWorld::DoWithChestAt(int a_BlockX, int a_BlockY, int a_BlockZ, cChestCallback & a_Callback)
{
	return m_ChunkMap->DoWithChestAt(a_BlockX, a_BlockY, a_BlockZ, a_Callback);
}





bool cWorld::DoWithDispenserAt(int a_BlockX, int a_BlockY, int a_BlockZ, cDispenserCallback & a_Callback)
{
	return m_ChunkMap->DoWithDispenserAt(a_BlockX, a_BlockY, a_BlockZ, a_Callback);
}





bool cWorld::DoWithDropperAt(int a_BlockX, int a_BlockY, int a_BlockZ, cDropperCallback & a_Callback)
{
	return m_ChunkMap->DoWithDropperAt(a_BlockX, a_BlockY, a_BlockZ, a_Callback);
}





bool cWorld::DoWithDropSpenserAt(int a_BlockX, int a_BlockY, int a_BlockZ, cDropSpenserCallback & a_Callback)
{
	return m_ChunkMap->DoWithDropSpenserAt(a_BlockX, a_BlockY, a_BlockZ, a_Callback);
}





bool cWorld::DoWithFurnaceAt(int a_BlockX, int a_BlockY, int a_BlockZ, cFurnaceCallback & a_Callback)
{
	return m_ChunkMap->DoWithFurnaceAt(a_BlockX, a_BlockY, a_BlockZ, a_Callback);
}





bool cWorld::DoWithNoteBlockAt(int a_BlockX, int a_BlockY, int a_BlockZ, cNoteBlockCallback & a_Callback)
{
	return m_ChunkMap->DoWithNoteBlockAt(a_BlockX, a_BlockY, a_BlockZ, a_Callback);
}





bool cWorld::DoWithCommandBlockAt(int a_BlockX, int a_BlockY, int a_BlockZ, cCommandBlockCallback & a_Callback)
{
	return m_ChunkMap->DoWithCommandBlockAt(a_BlockX, a_BlockY, a_BlockZ, a_Callback);
}





bool cWorld::DoWithMobHeadAt(int a_BlockX, int a_BlockY, int a_BlockZ, cMobHeadCallback & a_Callback)
{
	return m_ChunkMap->DoWithMobHeadAt(a_BlockX, a_BlockY, a_BlockZ, a_Callback);
}





bool cWorld::DoWithFlowerPotAt(int a_BlockX, int a_BlockY, int a_BlockZ, cFlowerPotCallback & a_Callback)
{
	return m_ChunkMap->DoWithFlowerPotAt(a_BlockX, a_BlockY, a_BlockZ, a_Callback);
}





bool cWorld::GetSignLines(int a_BlockX, int a_BlockY, int a_BlockZ, AString & a_Line1, AString & a_Line2, AString & a_Line3, AString & a_Line4)
{
	return m_ChunkMap->GetSignLines(a_BlockX, a_BlockY, a_BlockZ, a_Line1, a_Line2, a_Line3, a_Line4);
}





bool cWorld::DoWithChunk(int a_ChunkX, int a_ChunkZ, cChunkCallback & a_Callback)
{
	return m_ChunkMap->DoWithChunk(a_ChunkX, a_ChunkZ, a_Callback);
}





void cWorld::GrowTree(int a_X, int a_Y, int a_Z)
{
	if (GetBlock(a_X, a_Y, a_Z) == E_BLOCK_SAPLING)
	{
		// There is a sapling here, grow a tree according to its type:
		GrowTreeFromSapling(a_X, a_Y, a_Z, GetBlockMeta(a_X, a_Y, a_Z));
	}
	else
	{
		// There is nothing here, grow a tree based on the current biome here:
		GrowTreeByBiome(a_X, a_Y, a_Z);
	}
}





void cWorld::GrowTreeFromSapling(int a_X, int a_Y, int a_Z, NIBBLETYPE a_SaplingMeta)
{
	cNoise Noise(m_Generator.GetSeed());
	sSetBlockVector Logs, Other;
	auto WorldAge = (int)(std::chrono::duration_cast<cTickTimeLong>(m_WorldAge).count() & 0xffffffff);
	switch (a_SaplingMeta & 0x07)
	{
		case E_META_SAPLING_APPLE:    GetAppleTreeImage  (a_X, a_Y, a_Z, Noise, WorldAge, Logs, Other); break;
		case E_META_SAPLING_BIRCH:    GetBirchTreeImage  (a_X, a_Y, a_Z, Noise, WorldAge, Logs, Other); break;
		case E_META_SAPLING_CONIFER:  GetConiferTreeImage(a_X, a_Y, a_Z, Noise, WorldAge, Logs, Other); break;
		case E_META_SAPLING_JUNGLE:   GetJungleTreeImage (a_X, a_Y, a_Z, Noise, WorldAge, Logs, Other); break;
		case E_META_SAPLING_ACACIA:   GetAcaciaTreeImage (a_X, a_Y, a_Z, Noise, WorldAge, Logs, Other); break;
		case E_META_SAPLING_DARK_OAK: GetDarkoakTreeImage(a_X, a_Y, a_Z, Noise, WorldAge, Logs, Other); break;
	}
	Other.insert(Other.begin(), Logs.begin(), Logs.end());
	Logs.clear();
	GrowTreeImage(Other);
}





void cWorld::GrowTreeByBiome(int a_X, int a_Y, int a_Z)
{
	cNoise Noise(m_Generator.GetSeed());
	sSetBlockVector Logs, Other;
	GetTreeImageByBiome(a_X, a_Y, a_Z, Noise, (int)(std::chrono::duration_cast<cTickTimeLong>(m_WorldAge).count() & 0xffffffff), GetBiomeAt(a_X, a_Z), Logs, Other);
	Other.insert(Other.begin(), Logs.begin(), Logs.end());
	Logs.clear();
	GrowTreeImage(Other);
}





void cWorld::GrowTreeImage(const sSetBlockVector & a_Blocks)
{
	// Check that the tree has place to grow
	
	// Make a copy of the log blocks:
	sSetBlockVector b2;
	for (sSetBlockVector::const_iterator itr = a_Blocks.begin(); itr != a_Blocks.end(); ++itr)
	{
		if (itr->m_BlockType == E_BLOCK_LOG)
		{
			b2.push_back(*itr);
		}
	}  // for itr - a_Blocks[]
	
	// Query blocktypes and metas at those log blocks:
	if (!GetBlocks(b2, false))
	{
		return;
	}
	
	// Check that at each log's coord there's an block allowed to be overwritten:
	for (sSetBlockVector::const_iterator itr = b2.begin(); itr != b2.end(); ++itr)
	{
		switch (itr->m_BlockType)
		{
			CASE_TREE_ALLOWED_BLOCKS:
			{
				break;
			}
			default:
			{
				return;
			}
		}
	}  // for itr - b2[]
	
	// All ok, replace blocks with the tree image:
	m_ChunkMap->ReplaceTreeBlocks(a_Blocks);
}





bool cWorld::GrowRipePlant(int a_BlockX, int a_BlockY, int a_BlockZ, bool a_IsByBonemeal)
{
	BLOCKTYPE BlockType;
	NIBBLETYPE BlockMeta;
	GetBlockTypeMeta(a_BlockX, a_BlockY, a_BlockZ, BlockType, BlockMeta);
	switch (BlockType)
	{
		case E_BLOCK_CARROTS:
		{
			if (a_IsByBonemeal && !m_IsCarrotsBonemealable)
			{
				return false;
			}
			if (BlockMeta < 7)
			{
				FastSetBlock(a_BlockX, a_BlockY, a_BlockZ, BlockType, 7);
				BroadcastSoundParticleEffect(2005, a_BlockX, a_BlockY, a_BlockZ, 0);
			}
			return true;
		}

		case E_BLOCK_COCOA_POD:
		{
			NIBBLETYPE TypeMeta = BlockMeta & 0x03;
			int GrowState = BlockMeta >> 2;

			if (GrowState < 2)
			{
				GrowState++;
				FastSetBlock(a_BlockX, a_BlockY, a_BlockZ, BlockType, (NIBBLETYPE) (GrowState << 2 | TypeMeta));
				BroadcastSoundParticleEffect(2005, a_BlockX, a_BlockY, a_BlockZ, 0);
			}
			return true;
		}

		case E_BLOCK_CROPS:
		{
			if (a_IsByBonemeal && !m_IsCropsBonemealable)
			{
				return false;
			}
			if (BlockMeta < 7)
			{
				FastSetBlock(a_BlockX, a_BlockY, a_BlockZ, BlockType, 7);
				BroadcastSoundParticleEffect(2005, a_BlockX, a_BlockY, a_BlockZ, 0);
			}
			return true;
		}
		
		case E_BLOCK_MELON_STEM:
		{
			if (BlockMeta < 7)
			{
				if (a_IsByBonemeal && !m_IsMelonStemBonemealable)
				{
					return false;
				}
				FastSetBlock(a_BlockX, a_BlockY, a_BlockZ, BlockType, 7);
				BroadcastSoundParticleEffect(2005, a_BlockX, a_BlockY, a_BlockZ, 0);
			}
			else
			{
				if (a_IsByBonemeal && !m_IsMelonBonemealable)
				{
					return false;
				}
				GrowMelonPumpkin(a_BlockX, a_BlockY, a_BlockZ, BlockType);
			}
			return true;
		}
		
		case E_BLOCK_POTATOES:
		{
			if (a_IsByBonemeal && !m_IsPotatoesBonemealable)
			{
				return false;
			}
			if (BlockMeta < 7)
			{
				FastSetBlock(a_BlockX, a_BlockY, a_BlockZ, BlockType, 7);
				BroadcastSoundParticleEffect(2005, a_BlockX, a_BlockY, a_BlockZ, 0);
			}
			return true;
		}
		
		case E_BLOCK_PUMPKIN_STEM:
		{
			if (BlockMeta < 7)
			{
				if (a_IsByBonemeal && !m_IsPumpkinStemBonemealable)
				{
					return false;
				}
				FastSetBlock(a_BlockX, a_BlockY, a_BlockZ, BlockType, 7);
				BroadcastSoundParticleEffect(2005, a_BlockX, a_BlockY, a_BlockZ, 0);
			}
			else
			{
				if (a_IsByBonemeal && !m_IsPumpkinBonemealable)
				{
					return false;
				}
				GrowMelonPumpkin(a_BlockX, a_BlockY, a_BlockZ, BlockType);
			}
			return true;
		}
		
		case E_BLOCK_SAPLING:
		{
			if (a_IsByBonemeal && !m_IsSaplingBonemealable)
			{
				return false;
			}
			GrowTreeFromSapling(a_BlockX, a_BlockY, a_BlockZ, BlockMeta);
			return true;
		}
		
		case E_BLOCK_GRASS:
		{
			if (a_IsByBonemeal && !m_IsGrassBonemealable)
			{
				return false;
			}
			MTRand r1;
			for (int i = 0; i < 60; i++)
			{
				int OfsX = static_cast<int>(r1.randInt(3) + r1.randInt(3) + r1.randInt(3) + r1.randInt(3)) / 2 - 3;
				int OfsY = static_cast<int>(r1.randInt(3) + r1.randInt(3)) - 3;
				int OfsZ = static_cast<int>(r1.randInt(3) + r1.randInt(3) + r1.randInt(3) + r1.randInt(3)) / 2 - 3;
				BLOCKTYPE Ground = GetBlock(a_BlockX + OfsX, a_BlockY + OfsY, a_BlockZ + OfsZ);
				if (Ground != E_BLOCK_GRASS)
				{
					continue;
				}
				BLOCKTYPE Above  = GetBlock(a_BlockX + OfsX, a_BlockY + OfsY + 1, a_BlockZ + OfsZ);
				if (Above != E_BLOCK_AIR)
				{
					continue;
				}
				BLOCKTYPE  SpawnType;
				NIBBLETYPE SpawnMeta = 0;
				switch (r1.randInt(10))
				{
					case 0:  SpawnType = E_BLOCK_YELLOW_FLOWER; break;
					case 1:  SpawnType = E_BLOCK_RED_ROSE;      break;
					default:
					{
						SpawnType = E_BLOCK_TALL_GRASS;
						SpawnMeta = E_META_TALL_GRASS_GRASS;
						break;
					}
				}  // switch (random spawn block type)
				FastSetBlock(a_BlockX + OfsX, a_BlockY + OfsY + 1, a_BlockZ + OfsZ, SpawnType, SpawnMeta);
				BroadcastSoundParticleEffect(2005, a_BlockX + OfsX, a_BlockY + OfsY, a_BlockZ + OfsZ, 0);
			}  // for i - 50 times
			return true;
		}
		
		case E_BLOCK_SUGARCANE:
		{
			if (a_IsByBonemeal && !m_IsSugarcaneBonemealable)
			{
				return false;
			}
			m_ChunkMap->GrowSugarcane(a_BlockX, a_BlockY, a_BlockZ, m_MaxSugarcaneHeight);
			return true;
		}
		
		case E_BLOCK_CACTUS:
		{
			if (a_IsByBonemeal && !m_IsCactusBonemealable)
			{
				return false;
			}
			m_ChunkMap->GrowCactus(a_BlockX, a_BlockY, a_BlockZ, m_MaxCactusHeight);
			return true;
		}
	}  // switch (BlockType)
	return false;
}





void cWorld::GrowCactus(int a_BlockX, int a_BlockY, int a_BlockZ, int a_NumBlocksToGrow)
{
	m_ChunkMap->GrowCactus(a_BlockX, a_BlockY, a_BlockZ, a_NumBlocksToGrow);
}





void cWorld::GrowMelonPumpkin(int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE a_BlockType)
{
	MTRand Rand;
	m_ChunkMap->GrowMelonPumpkin(a_BlockX, a_BlockY, a_BlockZ, a_BlockType, Rand);
}





void cWorld::GrowSugarcane(int a_BlockX, int a_BlockY, int a_BlockZ, int a_NumBlocksToGrow)
{
	m_ChunkMap->GrowSugarcane(a_BlockX, a_BlockY, a_BlockZ, a_NumBlocksToGrow);
}





EMCSBiome cWorld::GetBiomeAt (int a_BlockX, int a_BlockZ)
{
	return m_ChunkMap->GetBiomeAt(a_BlockX, a_BlockZ);
}





bool cWorld::SetBiomeAt(int a_BlockX, int a_BlockZ, EMCSBiome a_Biome)
{
	return m_ChunkMap->SetBiomeAt(a_BlockX, a_BlockZ, a_Biome);
}





bool cWorld::SetAreaBiome(int a_MinX, int a_MaxX, int a_MinZ, int a_MaxZ, EMCSBiome a_Biome)
{
	return m_ChunkMap->SetAreaBiome(a_MinX, a_MaxX, a_MinZ, a_MaxZ, a_Biome);
}





bool cWorld::SetAreaBiome(const cCuboid & a_Area, EMCSBiome a_Biome)
{
	return SetAreaBiome(
		std::min(a_Area.p1.x, a_Area.p2.x), std::max(a_Area.p1.x, a_Area.p2.x),
		std::min(a_Area.p1.z, a_Area.p2.z), std::max(a_Area.p1.z, a_Area.p2.z),
		a_Biome
	);
}





void cWorld::SetBlock(int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, bool a_SendToClients)
{
	m_ChunkMap->SetBlock(a_BlockX, a_BlockY, a_BlockZ, a_BlockType, a_BlockMeta, a_SendToClients);
}



void cWorld::SetBlockMeta(int a_X, int a_Y, int a_Z, NIBBLETYPE a_MetaData)
{
	m_ChunkMap->SetBlockMeta(a_X, a_Y, a_Z, a_MetaData);
}





NIBBLETYPE cWorld::GetBlockSkyLight(int a_X, int a_Y, int a_Z)
{
	return m_ChunkMap->GetBlockSkyLight(a_X, a_Y, a_Z);
}





NIBBLETYPE cWorld::GetBlockBlockLight(int a_BlockX, int a_BlockY, int a_BlockZ)
{
	return m_ChunkMap->GetBlockBlockLight(a_BlockX, a_BlockY, a_BlockZ);
}





bool cWorld::GetBlockTypeMeta(int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE & a_BlockType, NIBBLETYPE & a_BlockMeta)
{
	return m_ChunkMap->GetBlockTypeMeta(a_BlockX, a_BlockY, a_BlockZ, (BLOCKTYPE &)a_BlockType, (NIBBLETYPE &)a_BlockMeta);
}





bool cWorld::GetBlockInfo(int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE & a_BlockType, NIBBLETYPE & a_Meta, NIBBLETYPE & a_SkyLight, NIBBLETYPE & a_BlockLight)
{
	return m_ChunkMap->GetBlockInfo(a_BlockX, a_BlockY, a_BlockZ, a_BlockType, a_Meta, a_SkyLight, a_BlockLight);
}





bool cWorld::WriteBlockArea(cBlockArea & a_Area, int a_MinBlockX, int a_MinBlockY, int a_MinBlockZ, int a_DataTypes)
{
	return m_ChunkMap->WriteBlockArea(a_Area, a_MinBlockX, a_MinBlockY, a_MinBlockZ, a_DataTypes);
}





void cWorld::SpawnItemPickups(const cItems & a_Pickups, double a_BlockX, double a_BlockY, double a_BlockZ, double a_FlyAwaySpeed, bool IsPlayerCreated)
{
	a_FlyAwaySpeed /= 100;  // Pre-divide, so that we don't have to divide each time inside the loop
	for (cItems::const_iterator itr = a_Pickups.begin(); itr != a_Pickups.end(); ++itr)
	{
		if (!IsValidItem(itr->m_ItemType) || (itr->m_ItemType == E_BLOCK_AIR))
		{
			// Don't spawn pickup if item isn't even valid; should prevent client crashing too
			continue;
		}

		float SpeedX = (float)(a_FlyAwaySpeed * (GetTickRandomNumber(10) - 5));
		float SpeedY = (float)(a_FlyAwaySpeed * GetTickRandomNumber(50));
		float SpeedZ = (float)(a_FlyAwaySpeed * (GetTickRandomNumber(10) - 5));
		
		cPickup * Pickup = new cPickup(
			a_BlockX, a_BlockY, a_BlockZ,
			*itr, IsPlayerCreated, SpeedX, SpeedY, SpeedZ
		);
		Pickup->Initialize(*this);
	}
}





void cWorld::SpawnItemPickups(const cItems & a_Pickups, double a_BlockX, double a_BlockY, double a_BlockZ, double a_SpeedX, double a_SpeedY, double a_SpeedZ, bool IsPlayerCreated)
{
	for (cItems::const_iterator itr = a_Pickups.begin(); itr != a_Pickups.end(); ++itr)
	{
		if (!IsValidItem(itr->m_ItemType) || (itr->m_ItemType == E_BLOCK_AIR))
		{
			continue;
		}

		cPickup * Pickup = new cPickup(
			a_BlockX, a_BlockY, a_BlockZ,
			*itr, IsPlayerCreated, (float)a_SpeedX, (float)a_SpeedY, (float)a_SpeedZ
		);
		Pickup->Initialize(*this);
	}
}





int cWorld::SpawnFallingBlock(int a_X, int a_Y, int a_Z, BLOCKTYPE BlockType, NIBBLETYPE BlockMeta)
{
	cFallingBlock * FallingBlock = new cFallingBlock(Vector3i(a_X, a_Y, a_Z), BlockType, BlockMeta);
	FallingBlock->Initialize(*this);
	return FallingBlock->GetUniqueID();
}





int cWorld::SpawnExperienceOrb(double a_X, double a_Y, double a_Z, int a_Reward)
{
	if (a_Reward < 1)
	{
		return -1;
	}

	cExpOrb * ExpOrb = new cExpOrb(a_X, a_Y, a_Z, a_Reward);
	ExpOrb->Initialize(*this);
	return ExpOrb->GetUniqueID();
}





int cWorld::SpawnMinecart(double a_X, double a_Y, double a_Z, int a_MinecartType, const cItem & a_Content, int a_BlockHeight)
{
	cMinecart * Minecart;
	switch (a_MinecartType)
	{
		case E_ITEM_MINECART:             Minecart = new cRideableMinecart     (a_X, a_Y, a_Z, a_Content, a_BlockHeight); break;
		case E_ITEM_CHEST_MINECART:       Minecart = new cMinecartWithChest    (a_X, a_Y, a_Z); break;
		case E_ITEM_FURNACE_MINECART:     Minecart = new cMinecartWithFurnace  (a_X, a_Y, a_Z); break;
		case E_ITEM_MINECART_WITH_TNT:    Minecart = new cMinecartWithTNT      (a_X, a_Y, a_Z); break;
		case E_ITEM_MINECART_WITH_HOPPER: Minecart = new cMinecartWithHopper   (a_X, a_Y, a_Z); break;
		default:
		{
			return -1;
		}
	}  // switch (a_MinecartType)
	Minecart->Initialize(*this);
	return Minecart->GetUniqueID();
}





void cWorld::SpawnPrimedTNT(double a_X, double a_Y, double a_Z, int a_FuseTicks, double a_InitialVelocityCoeff)
{
	cTNTEntity * TNT = new cTNTEntity(a_X, a_Y, a_Z, a_FuseTicks);
	TNT->Initialize(*this);
	TNT->SetSpeed(
		a_InitialVelocityCoeff * (GetTickRandomNumber(2) - 1), /** -1, 0, 1 */
		a_InitialVelocityCoeff * 2,
		a_InitialVelocityCoeff * (GetTickRandomNumber(2) - 1)
	);
}





void cWorld::SetBlocks(const sSetBlockVector & a_Blocks)
{
	m_ChunkMap->SetBlocks(a_Blocks);
}





void cWorld::ReplaceBlocks(const sSetBlockVector & a_Blocks, BLOCKTYPE a_FilterBlockType)
{
	m_ChunkMap->ReplaceBlocks(a_Blocks, a_FilterBlockType);
}





bool cWorld::GetBlocks(sSetBlockVector & a_Blocks, bool a_ContinueOnFailure)
{
	return m_ChunkMap->GetBlocks(a_Blocks, a_ContinueOnFailure);
}





bool cWorld::DigBlock(int a_X, int a_Y, int a_Z)
{
	cBlockHandler * Handler = cBlockInfo::GetHandler(GetBlock(a_X, a_Y, a_Z));
	cChunkInterface ChunkInterface(GetChunkMap());
	Handler->OnDestroyed(ChunkInterface, *this, a_X, a_Y, a_Z);
	return m_ChunkMap->DigBlock(a_X, a_Y, a_Z);
}





void cWorld::SendBlockTo(int a_X, int a_Y, int a_Z, cPlayer * a_Player)
{
	m_ChunkMap->SendBlockTo(a_X, a_Y, a_Z, a_Player);
}





int cWorld::GetHeight(int a_X, int a_Z)
{
	return m_ChunkMap->GetHeight(a_X, a_Z);
}





bool cWorld::TryGetHeight(int a_BlockX, int a_BlockZ, int & a_Height)
{
	return m_ChunkMap->TryGetHeight(a_BlockX, a_BlockZ, a_Height);
}





void cWorld::BroadcastAttachEntity(const cEntity & a_Entity, const cEntity * a_Vehicle)
{
	return m_ChunkMap->BroadcastAttachEntity(a_Entity, a_Vehicle);
}





void cWorld::BroadcastBlockAction(int a_BlockX, int a_BlockY, int a_BlockZ, char a_Byte1, char a_Byte2, BLOCKTYPE a_BlockType, const cClientHandle * a_Exclude)
{
	m_ChunkMap->BroadcastBlockAction(a_BlockX, a_BlockY, a_BlockZ, a_Byte1, a_Byte2, a_BlockType, a_Exclude);
}





void cWorld::BroadcastBlockBreakAnimation(int a_EntityID, int a_BlockX, int a_BlockY, int a_BlockZ, char a_Stage, const cClientHandle * a_Exclude)
{
	m_ChunkMap->BroadcastBlockBreakAnimation(a_EntityID, a_BlockX, a_BlockY, a_BlockZ, a_Stage, a_Exclude);
}





void cWorld::BroadcastBlockEntity(int a_BlockX, int a_BlockY, int a_BlockZ, const cClientHandle * a_Exclude)
{
	m_ChunkMap->BroadcastBlockEntity(a_BlockX, a_BlockY, a_BlockZ, a_Exclude);
}





void cWorld::BroadcastChat(const AString & a_Message, const cClientHandle * a_Exclude, eMessageType a_ChatPrefix)
{
	cCSLock Lock(m_CSPlayers);
	for (cPlayerList::iterator itr = m_Players.begin(); itr != m_Players.end(); ++itr)
	{
		cClientHandle * ch = (*itr)->GetClientHandle();
		if ((ch == a_Exclude) || (ch == nullptr) || !ch->IsLoggedIn() || ch->IsDestroyed())
		{
			continue;
		}
		ch->SendChat(a_Message, a_ChatPrefix);
	}
}





void cWorld::BroadcastChat(const cCompositeChat & a_Message, const cClientHandle * a_Exclude)
{
	cCSLock Lock(m_CSPlayers);
	for (cPlayerList::iterator itr = m_Players.begin(); itr != m_Players.end(); ++itr)
	{
		cClientHandle * ch = (*itr)->GetClientHandle();
		if ((ch == a_Exclude) || (ch == nullptr) || !ch->IsLoggedIn() || ch->IsDestroyed())
		{
			continue;
		}
		ch->SendChat(a_Message);
	}
}





void cWorld::BroadcastChunkData(int a_ChunkX, int a_ChunkZ, cChunkDataSerializer & a_Serializer, const cClientHandle * a_Exclude)
{
	m_ChunkMap->BroadcastChunkData(a_ChunkX, a_ChunkZ, a_Serializer, a_Exclude);
}





void cWorld::BroadcastCollectEntity(const cEntity & a_Entity, const cPlayer & a_Player, const cClientHandle * a_Exclude)
{
	m_ChunkMap->BroadcastCollectEntity(a_Entity, a_Player, a_Exclude);
}





void cWorld::BroadcastDestroyEntity(const cEntity & a_Entity, const cClientHandle * a_Exclude)
{
	m_ChunkMap->BroadcastDestroyEntity(a_Entity, a_Exclude);
}





void cWorld::BroadcastEntityEffect(const cEntity & a_Entity, int a_EffectID, int a_Amplifier, short a_Duration, const cClientHandle * a_Exclude)
{
	m_ChunkMap->BroadcastEntityEffect(a_Entity, a_EffectID, a_Amplifier, a_Duration, a_Exclude);
}





void cWorld::BroadcastEntityEquipment(const cEntity & a_Entity, short a_SlotNum, const cItem & a_Item, const cClientHandle * a_Exclude)
{
	m_ChunkMap->BroadcastEntityEquipment(a_Entity, a_SlotNum, a_Item, a_Exclude);
}





void cWorld::BroadcastEntityHeadLook(const cEntity & a_Entity, const cClientHandle * a_Exclude)
{
	m_ChunkMap->BroadcastEntityHeadLook(a_Entity, a_Exclude);
}





void cWorld::BroadcastEntityLook(const cEntity & a_Entity, const cClientHandle * a_Exclude)
{
	m_ChunkMap->BroadcastEntityLook(a_Entity, a_Exclude);
}





void cWorld::BroadcastEntityMetadata(const cEntity & a_Entity, const cClientHandle * a_Exclude)
{
	m_ChunkMap->BroadcastEntityMetadata(a_Entity, a_Exclude);
}





void cWorld::BroadcastEntityRelMove(const cEntity & a_Entity, char a_RelX, char a_RelY, char a_RelZ, const cClientHandle * a_Exclude)
{
	m_ChunkMap->BroadcastEntityRelMove(a_Entity, a_RelX, a_RelY, a_RelZ, a_Exclude);
}





void cWorld::BroadcastEntityRelMoveLook(const cEntity & a_Entity, char a_RelX, char a_RelY, char a_RelZ, const cClientHandle * a_Exclude)
{
	m_ChunkMap->BroadcastEntityRelMoveLook(a_Entity, a_RelX, a_RelY, a_RelZ, a_Exclude);
}





void cWorld::BroadcastEntityStatus(const cEntity & a_Entity, char a_Status, const cClientHandle * a_Exclude)
{
	m_ChunkMap->BroadcastEntityStatus(a_Entity, a_Status, a_Exclude);
}





void cWorld::BroadcastEntityVelocity(const cEntity & a_Entity, const cClientHandle * a_Exclude)
{
	m_ChunkMap->BroadcastEntityVelocity(a_Entity, a_Exclude);
}




void cWorld::BroadcastEntityAnimation(const cEntity & a_Entity, char a_Animation, const cClientHandle * a_Exclude)
{
	m_ChunkMap->BroadcastEntityAnimation(a_Entity, a_Animation, a_Exclude);
}





void cWorld::BroadcastParticleEffect(const AString & a_ParticleName, float a_SrcX, float a_SrcY, float a_SrcZ, float a_OffsetX, float a_OffsetY, float a_OffsetZ, float a_ParticleData, int a_ParticleAmount, cClientHandle * a_Exclude)
{
	m_ChunkMap->BroadcastParticleEffect(a_ParticleName, a_SrcX, a_SrcY, a_SrcZ, a_OffsetX, a_OffsetY, a_OffsetZ, a_ParticleData, a_ParticleAmount, a_Exclude);
}





void cWorld::BroadcastPlayerListAddPlayer(const cPlayer & a_Player, const cClientHandle * a_Exclude)
{
	cCSLock Lock(m_CSPlayers);
	for (cPlayerList::iterator itr = m_Players.begin(); itr != m_Players.end(); ++itr)
	{
		cClientHandle * ch = (*itr)->GetClientHandle();
		if ((ch == a_Exclude) || (ch == nullptr) || !ch->IsLoggedIn() || ch->IsDestroyed())
		{
			continue;
		}
		ch->SendPlayerListAddPlayer(a_Player);
	}
}





void cWorld::BroadcastPlayerListRemovePlayer(const cPlayer & a_Player, const cClientHandle * a_Exclude)
{
	cCSLock Lock(m_CSPlayers);
	for (cPlayerList::iterator itr = m_Players.begin(); itr != m_Players.end(); ++itr)
	{
		cClientHandle * ch = (*itr)->GetClientHandle();
		if ((ch == a_Exclude) || (ch == nullptr) || !ch->IsLoggedIn() || ch->IsDestroyed())
		{
			continue;
		}
		ch->SendPlayerListRemovePlayer(a_Player);
	}
}





void cWorld::BroadcastPlayerListUpdateGameMode(const cPlayer & a_Player, const cClientHandle * a_Exclude)
{
	cCSLock Lock(m_CSPlayers);
	for (cPlayerList::iterator itr = m_Players.begin(); itr != m_Players.end(); ++itr)
	{
		cClientHandle * ch = (*itr)->GetClientHandle();
		if ((ch == a_Exclude) || (ch == nullptr) || !ch->IsLoggedIn() || ch->IsDestroyed())
		{
			continue;
		}
		ch->SendPlayerListUpdateGameMode(a_Player);
	}
}





void cWorld::BroadcastPlayerListUpdatePing(const cPlayer & a_Player, const cClientHandle * a_Exclude)
{
	cCSLock Lock(m_CSPlayers);
	for (cPlayerList::iterator itr = m_Players.begin(); itr != m_Players.end(); ++itr)
	{
		cClientHandle * ch = (*itr)->GetClientHandle();
		if ((ch == a_Exclude) || (ch == nullptr) || !ch->IsLoggedIn() || ch->IsDestroyed())
		{
			continue;
		}
		ch->SendPlayerListUpdatePing(a_Player);
	}
}





void cWorld::BroadcastPlayerListUpdateDisplayName(const cPlayer & a_Player, const AString & a_CustomName, const cClientHandle * a_Exclude)
{
	cCSLock Lock(m_CSPlayers);
	for (cPlayerList::iterator itr = m_Players.begin(); itr != m_Players.end(); ++itr)
	{
		cClientHandle * ch = (*itr)->GetClientHandle();
		if ((ch == a_Exclude) || (ch == nullptr) || !ch->IsLoggedIn() || ch->IsDestroyed())
		{
			continue;
		}
		ch->SendPlayerListUpdateDisplayName(a_Player, a_CustomName);
	}
}





void cWorld::BroadcastRemoveEntityEffect(const cEntity & a_Entity, int a_EffectID, const cClientHandle * a_Exclude)
{
	m_ChunkMap->BroadcastRemoveEntityEffect(a_Entity, a_EffectID, a_Exclude);
}





void cWorld::BroadcastScoreboardObjective(const AString & a_Name, const AString & a_DisplayName, Byte a_Mode)
{
	cCSLock Lock(m_CSPlayers);
	for (cPlayerList::iterator itr = m_Players.begin(); itr != m_Players.end(); ++itr)
	{
		cClientHandle * ch = (*itr)->GetClientHandle();
		if ((ch == nullptr) || !ch->IsLoggedIn() || ch->IsDestroyed())
		{
			continue;
		}
		ch->SendScoreboardObjective(a_Name, a_DisplayName, a_Mode);
	}
}





void cWorld::BroadcastScoreUpdate(const AString & a_Objective, const AString & a_Player, cObjective::Score a_Score, Byte a_Mode)
{
	cCSLock Lock(m_CSPlayers);
	for (cPlayerList::iterator itr = m_Players.begin(); itr != m_Players.end(); ++itr)
	{
		cClientHandle * ch = (*itr)->GetClientHandle();
		if ((ch == nullptr) || !ch->IsLoggedIn() || ch->IsDestroyed())
		{
			continue;
		}
		ch->SendScoreUpdate(a_Objective, a_Player, a_Score, a_Mode);
	}
}





void cWorld::BroadcastDisplayObjective(const AString & a_Objective, cScoreboard::eDisplaySlot a_Display)
{
	cCSLock Lock(m_CSPlayers);
	for (cPlayerList::iterator itr = m_Players.begin(); itr != m_Players.end(); ++itr)
	{
		cClientHandle * ch = (*itr)->GetClientHandle();
		if ((ch == nullptr) || !ch->IsLoggedIn() || ch->IsDestroyed())
		{
			continue;
		}
		ch->SendDisplayObjective(a_Objective, a_Display);
	}
}





void cWorld::BroadcastSoundEffect(const AString & a_SoundName, double a_X, double a_Y, double a_Z, float a_Volume, float a_Pitch, const cClientHandle * a_Exclude)
{
	m_ChunkMap->BroadcastSoundEffect(a_SoundName, a_X, a_Y, a_Z, a_Volume, a_Pitch, a_Exclude);
}





void cWorld::BroadcastSoundParticleEffect(int a_EffectID, int a_SrcX, int a_SrcY, int a_SrcZ, int a_Data, const cClientHandle * a_Exclude)
{
	m_ChunkMap->BroadcastSoundParticleEffect(a_EffectID, a_SrcX, a_SrcY, a_SrcZ, a_Data, a_Exclude);
}





void cWorld::BroadcastSpawnEntity(cEntity & a_Entity, const cClientHandle * a_Exclude)
{
	m_ChunkMap->BroadcastSpawnEntity(a_Entity, a_Exclude);
}





void cWorld::BroadcastTeleportEntity(const cEntity & a_Entity, const cClientHandle * a_Exclude)
{
	cCSLock Lock(m_CSPlayers);
	for (cPlayerList::iterator itr = m_Players.begin(); itr != m_Players.end(); ++itr)
	{
		cClientHandle * ch = (*itr)->GetClientHandle();
		if ((ch == a_Exclude) || (ch == nullptr) || !ch->IsLoggedIn() || ch->IsDestroyed())
		{
			continue;
		}
		ch->SendTeleportEntity(a_Entity);
	}
}





void cWorld::BroadcastThunderbolt(int a_BlockX, int a_BlockY, int a_BlockZ, const cClientHandle * a_Exclude)
{
	m_ChunkMap->BroadcastThunderbolt(a_BlockX, a_BlockY, a_BlockZ, a_Exclude);
}





void cWorld::BroadcastTimeUpdate(const cClientHandle * a_Exclude)
{
	cCSLock Lock(m_CSPlayers);
	for (cPlayerList::iterator itr = m_Players.begin(); itr != m_Players.end(); ++itr)
	{
		cClientHandle * ch = (*itr)->GetClientHandle();
		if ((ch == a_Exclude) || (ch == nullptr) || !ch->IsLoggedIn() || ch->IsDestroyed())
		{
			continue;
		}
		ch->SendTimeUpdate(std::chrono::duration_cast<cTickTimeLong>(m_WorldAge).count(), std::chrono::duration_cast<cTickTimeLong>(m_TimeOfDay).count(), m_IsDaylightCycleEnabled);
	}
}





void cWorld::BroadcastUseBed(const cEntity & a_Entity, int a_BlockX, int a_BlockY, int a_BlockZ)
{
	m_ChunkMap->BroadcastUseBed(a_Entity, a_BlockX, a_BlockY, a_BlockZ);
}





void cWorld::BroadcastWeather(eWeather a_Weather, const cClientHandle * a_Exclude)
{
	cCSLock Lock(m_CSPlayers);
	for (cPlayerList::iterator itr = m_Players.begin(); itr != m_Players.end(); ++itr)
	{
		cClientHandle * ch = (*itr)->GetClientHandle();
		if ((ch == a_Exclude) || (ch == nullptr) || !ch->IsLoggedIn() || ch->IsDestroyed())
		{
			continue;
		}
		ch->SendWeather(a_Weather);
	}
}

void cWorld::BroadcastRainDensity(float a_RainDensity, const cClientHandle * a_Exclude)
{
	cCSLock Lock(m_CSPlayers);
	for (cPlayerList::iterator itr = m_Players.begin(); itr != m_Players.end(); ++itr)
	{
		cClientHandle * ch = (*itr)->GetClientHandle();
		if ((ch == a_Exclude) || (ch == nullptr) || !ch->IsLoggedIn() || ch->IsDestroyed())
		{
			continue;
		}
		ch->SendRainDensity(a_RainDensity);
	}
}





void cWorld::SendBlockEntity(int a_BlockX, int a_BlockY, int a_BlockZ, cClientHandle & a_Client)
{
	m_ChunkMap->SendBlockEntity(a_BlockX, a_BlockY, a_BlockZ, a_Client);
}





void cWorld::MarkRedstoneDirty(int a_ChunkX, int a_ChunkZ)
{
	m_ChunkMap->MarkRedstoneDirty(a_ChunkX, a_ChunkZ);
}





void cWorld::MarkChunkDirty(int a_ChunkX, int a_ChunkZ, bool a_MarkRedstoneDirty)
{
	m_ChunkMap->MarkChunkDirty(a_ChunkX, a_ChunkZ, a_MarkRedstoneDirty);
}





void cWorld::MarkChunkSaving(int a_ChunkX, int a_ChunkZ)
{
	m_ChunkMap->MarkChunkSaving(a_ChunkX, a_ChunkZ);
}





void cWorld::MarkChunkSaved (int a_ChunkX, int a_ChunkZ)
{
	m_ChunkMap->MarkChunkSaved (a_ChunkX, a_ChunkZ);
}





void cWorld::QueueSetChunkData(const cSetChunkDataPtr & a_SetChunkData)
{
	ASSERT(IsChunkQueued(a_SetChunkData->GetChunkX(), a_SetChunkData->GetChunkZ()));

	// Validate biomes, if needed:
	if (!a_SetChunkData->AreBiomesValid())
	{
		// The biomes are not assigned, get them from the generator:
		m_Generator.GenerateBiomes(a_SetChunkData->GetChunkX(), a_SetChunkData->GetChunkZ(), a_SetChunkData->GetBiomes());
		a_SetChunkData->MarkBiomesValid();
	}
	
	// Validate heightmap, if needed:
	if (!a_SetChunkData->IsHeightMapValid())
	{
		a_SetChunkData->CalculateHeightMap();
	}
	
	// Store a copy of the data in the queue:
	// TODO: If the queue is too large, wait for it to get processed. Not likely, though.
	cCSLock Lock(m_CSSetChunkDataQueue);
	m_SetChunkDataQueue.push_back(a_SetChunkData);
}





void cWorld::SetChunkData(cSetChunkData & a_SetChunkData)
{
	ASSERT(a_SetChunkData.AreBiomesValid());
	ASSERT(a_SetChunkData.IsHeightMapValid());
	
	m_ChunkMap->SetChunkData(a_SetChunkData);
	
	// Initialize the entities (outside the m_ChunkMap's CS, to fix FS #347):
	cEntityList Entities;
	std::swap(a_SetChunkData.GetEntities(), Entities);
	for (cEntityList::iterator itr = Entities.begin(), end = Entities.end(); itr != end; ++itr)
	{
		(*itr)->Initialize(*this);
	}
	
	// If a client is requesting this chunk, send it to them:
	int ChunkX = a_SetChunkData.GetChunkX();
	int ChunkZ = a_SetChunkData.GetChunkZ();
	if (m_ChunkMap->HasChunkAnyClients(ChunkX, ChunkZ))
	{
		m_ChunkSender.ChunkReady(ChunkX, ChunkZ);
	}

	// Save the chunk right after generating, so that we don't have to generate it again on next run
	if (a_SetChunkData.ShouldMarkDirty())
	{
		m_Storage.QueueSaveChunk(ChunkX, ChunkZ);
	}
}





void cWorld::ChunkLighted(
	int a_ChunkX, int a_ChunkZ,
	const cChunkDef::BlockNibbles & a_BlockLight,
	const cChunkDef::BlockNibbles & a_SkyLight
)
{
	m_ChunkMap->ChunkLighted(a_ChunkX, a_ChunkZ, a_BlockLight, a_SkyLight);
}





bool cWorld::GetChunkData(int a_ChunkX, int a_ChunkZ, cChunkDataCallback & a_Callback)
{
	return m_ChunkMap->GetChunkData(a_ChunkX, a_ChunkZ, a_Callback);
}





bool cWorld::GetChunkBlockTypes(int a_ChunkX, int a_ChunkZ, BLOCKTYPE * a_BlockTypes)
{
	return m_ChunkMap->GetChunkBlockTypes(a_ChunkX, a_ChunkZ, a_BlockTypes);
}





bool cWorld::IsChunkQueued(int a_ChunkX, int a_ChunkZ) const
{
	return m_ChunkMap->IsChunkQueued(a_ChunkX, a_ChunkZ);
}





bool cWorld::IsChunkValid(int a_ChunkX, int a_ChunkZ) const
{
	return m_ChunkMap->IsChunkValid(a_ChunkX, a_ChunkZ);
}





bool cWorld::HasChunkAnyClients(int a_ChunkX, int a_ChunkZ) const
{
	return m_ChunkMap->HasChunkAnyClients(a_ChunkX, a_ChunkZ);
}





void cWorld::UnloadUnusedChunks(void)
{
	m_LastUnload = std::chrono::duration_cast<cTickTimeLong>(m_WorldAge);
	m_ChunkMap->UnloadUnusedChunks();
}





void cWorld::QueueUnloadUnusedChunks(void)
{
	QueueTask(make_unique<cWorld::cTaskUnloadUnusedChunks>());
}





void cWorld::CollectPickupsByPlayer(cPlayer & a_Player)
{
	m_ChunkMap->CollectPickupsByPlayer(a_Player);
}





void cWorld::AddPlayer(cPlayer * a_Player)
{
	cCSLock Lock(m_CSPlayersToAdd);
	m_PlayersToAdd.push_back(a_Player);
}





void cWorld::RemovePlayer(cPlayer * a_Player, bool a_RemoveFromChunk)
{
	if (a_RemoveFromChunk)
	{
		// To prevent iterator invalidations when an entity goes through a portal and calls this function whilst being ticked by cChunk
		// we should not change cChunk's entity list if asked not to
		m_ChunkMap->RemoveEntity(a_Player);
	}
	{
		cCSLock Lock(m_CSPlayersToAdd);
		m_PlayersToAdd.remove(a_Player);
	}
	{
		cCSLock Lock(m_CSPlayers);
		LOGD("Removing player %s from world \"%s\"", a_Player->GetName().c_str(), m_WorldName.c_str());
		m_Players.remove(a_Player);
	}
	
	// Remove the player's client from the list of clients to be ticked:
	cClientHandle * Client = a_Player->GetClientHandle();
	if (Client != nullptr)
	{
		Client->RemoveFromWorld();
		m_ChunkMap->RemoveClientFromChunks(Client);
		cCSLock Lock(m_CSClients);
		m_ClientsToRemove.push_back(Client);
	}
}





bool cWorld::ForEachPlayer(cPlayerListCallback & a_Callback)
{
	// Calls the callback for each player in the list
	cCSLock Lock(m_CSPlayers);
	for (cPlayerList::iterator itr = m_Players.begin(), itr2 = itr; itr != m_Players.end(); itr = itr2)
	{
		++itr2;
		if (a_Callback.Item(*itr))
		{
			return false;
		}
	}  // for itr - m_Players[]
	return true;
}





bool cWorld::DoWithPlayer(const AString & a_PlayerName, cPlayerListCallback & a_Callback)
{
	// Calls the callback for the specified player in the list
	cCSLock Lock(m_CSPlayers);
	for (cPlayerList::iterator itr = m_Players.begin(); itr != m_Players.end(); ++itr)
	{
		if (NoCaseCompare((*itr)->GetName(), a_PlayerName) == 0)
		{
			a_Callback.Item(*itr);
			return true;
		}
	}  // for itr - m_Players[]
	return false;
}





bool cWorld::FindAndDoWithPlayer(const AString & a_PlayerNameHint, cPlayerListCallback & a_Callback)
{
	cPlayer * BestMatch = nullptr;
	size_t BestRating = 0;
	size_t NameLength = a_PlayerNameHint.length();

	cCSLock Lock(m_CSPlayers);
	for (cPlayerList::iterator itr = m_Players.begin(); itr != m_Players.end(); ++itr)
	{
		size_t Rating = RateCompareString (a_PlayerNameHint, (*itr)->GetName());
		if (Rating >= BestRating)
		{
			BestMatch = *itr;
			BestRating = Rating;
		}
		if (Rating == NameLength)  // Perfect match
		{
			break;
		}
	}  // for itr - m_Players[]

	if (BestMatch != nullptr)
	{
		return a_Callback.Item (BestMatch);
	}
	return false;
}





bool cWorld::DoWithPlayerByUUID(const AString & a_PlayerUUID, cPlayerListCallback & a_Callback)
{
	cCSLock Lock(m_CSPlayers);
	for (cPlayerList::iterator itr = m_Players.begin(); itr != m_Players.end(); ++itr)
	{
		if ((*itr)->GetUUID() == a_PlayerUUID)
		{
			return a_Callback.Item(*itr);
		}
	}
	return false;
}





// TODO: This interface is dangerous!
cPlayer * cWorld::FindClosestPlayer(const Vector3d & a_Pos, float a_SightLimit, bool a_CheckLineOfSight)
{
	cTracer LineOfSight(this);

	double ClosestDistance = a_SightLimit;
	cPlayer * ClosestPlayer = nullptr;

	cCSLock Lock(m_CSPlayers);
	for (cPlayerList::const_iterator itr = m_Players.begin(); itr != m_Players.end(); ++itr)
	{
		Vector3f Pos = (*itr)->GetPosition();
		double Distance = (Pos - a_Pos).Length();

		if (Distance < ClosestDistance)
		{
			if (a_CheckLineOfSight)
			{
				if (!LineOfSight.Trace(a_Pos, (Pos - a_Pos), (int)(Pos - a_Pos).Length()))
				{
					ClosestDistance = Distance;
					ClosestPlayer = *itr;
				}
			}
			else
			{
				ClosestDistance = Distance;
				ClosestPlayer = *itr;
			}
		}
	}
	return ClosestPlayer;
}





void cWorld::SendPlayerList(cPlayer * a_DestPlayer)
{
	// Sends the playerlist to a_DestPlayer
	cCSLock Lock(m_CSPlayers);
	for (cPlayerList::iterator itr = m_Players.begin(); itr != m_Players.end(); ++itr)
	{
		cClientHandle * ch = (*itr)->GetClientHandle();
		if ((ch != nullptr) && !ch->IsDestroyed())
		{
			a_DestPlayer->GetClientHandle()->SendPlayerListAddPlayer(*(*itr));
		}
	}
}





bool cWorld::ForEachEntity(cEntityCallback & a_Callback)
{
	return m_ChunkMap->ForEachEntity(a_Callback);
}





bool cWorld::ForEachEntityInChunk(int a_ChunkX, int a_ChunkZ, cEntityCallback & a_Callback)
{
	return m_ChunkMap->ForEachEntityInChunk(a_ChunkX, a_ChunkZ, a_Callback);
}





bool cWorld::ForEachEntityInBox(const cBoundingBox & a_Box, cEntityCallback & a_Callback)
{
	return m_ChunkMap->ForEachEntityInBox(a_Box, a_Callback);
}





bool cWorld::DoWithEntityByID(int a_UniqueID, cEntityCallback & a_Callback)
{
	return m_ChunkMap->DoWithEntityByID(a_UniqueID, a_Callback);
}





void cWorld::CompareChunkClients(int a_ChunkX1, int a_ChunkZ1, int a_ChunkX2, int a_ChunkZ2, cClientDiffCallback & a_Callback)
{
	m_ChunkMap->CompareChunkClients(a_ChunkX1, a_ChunkZ1, a_ChunkX2, a_ChunkZ2, a_Callback);
}





bool cWorld::AddChunkClient(int a_ChunkX, int a_ChunkZ, cClientHandle * a_Client)
{
	return m_ChunkMap->AddChunkClient(a_ChunkX, a_ChunkZ, a_Client);
}





void cWorld::RemoveChunkClient(int a_ChunkX, int a_ChunkZ, cClientHandle * a_Client)
{
	m_ChunkMap->RemoveChunkClient(a_ChunkX, a_ChunkZ, a_Client);
}





void cWorld::RemoveClientFromChunks(cClientHandle * a_Client)
{
	m_ChunkMap->RemoveClientFromChunks(a_Client);
}





void cWorld::SendChunkTo(int a_ChunkX, int a_ChunkZ, cChunkSender::eChunkPriority a_Priority, cClientHandle * a_Client)
{
	m_ChunkSender.QueueSendChunkTo(a_ChunkX, a_ChunkZ, a_Priority, a_Client);
}





void cWorld::ForceSendChunkTo(int a_ChunkX, int a_ChunkZ, cChunkSender::eChunkPriority a_Priority, cClientHandle * a_Client)
{
	a_Client->AddWantedChunk(a_ChunkX, a_ChunkZ);
	m_ChunkSender.QueueSendChunkTo(a_ChunkX, a_ChunkZ, a_Priority, a_Client);
}





void cWorld::RemoveClientFromChunkSender(cClientHandle * a_Client)
{
	m_ChunkSender.RemoveClient(a_Client);
}





void cWorld::TouchChunk(int a_ChunkX, int a_ChunkZ)
{
	m_ChunkMap->TouchChunk(a_ChunkX, a_ChunkZ);
}





void cWorld::PrepareChunk(int a_ChunkX, int a_ChunkZ, cChunkCoordCallback * a_CallAfter)
{
	m_ChunkMap->PrepareChunk(a_ChunkX, a_ChunkZ, a_CallAfter);
}





void cWorld::ChunkLoadFailed(int a_ChunkX, int a_ChunkZ)
{
	m_ChunkMap->ChunkLoadFailed(a_ChunkX, a_ChunkZ);
}





bool cWorld::SetSignLines(int a_BlockX, int a_BlockY, int a_BlockZ, const AString & a_Line1, const AString & a_Line2, const AString & a_Line3, const AString & a_Line4, cPlayer * a_Player)
{
	AString Line1(a_Line1);
	AString Line2(a_Line2);
	AString Line3(a_Line3);
	AString Line4(a_Line4);

	if (cRoot::Get()->GetPluginManager()->CallHookUpdatingSign(*this, a_BlockX, a_BlockY, a_BlockZ, Line1, Line2, Line3, Line4, a_Player))
	{
		return false;
	}

	if (m_ChunkMap->SetSignLines(a_BlockX, a_BlockY, a_BlockZ, Line1, Line2, Line3, Line4))
	{
		cRoot::Get()->GetPluginManager()->CallHookUpdatedSign(*this, a_BlockX, a_BlockY, a_BlockZ, Line1, Line2, Line3, Line4, a_Player);
		return true;
	}

	return false;
}





bool cWorld::SetCommandBlockCommand(int a_BlockX, int a_BlockY, int a_BlockZ, const AString & a_Command)
{
	class cUpdateCommandBlock : public cCommandBlockCallback
	{
		AString m_Command;
	public:
		cUpdateCommandBlock(const AString & a_CallbackCommand) : m_Command(a_CallbackCommand) {}
			
		virtual bool Item(cCommandBlockEntity * a_CommandBlock) override
		{
			a_CommandBlock->SetCommand(m_Command);
			return false;
		}
	} CmdBlockCB (a_Command);

	return DoWithCommandBlockAt(a_BlockX, a_BlockY, a_BlockZ, CmdBlockCB);
}





bool cWorld::IsTrapdoorOpen(int a_BlockX, int a_BlockY, int a_BlockZ)
{
	BLOCKTYPE Block;
	NIBBLETYPE Meta;
	GetBlockTypeMeta(a_BlockX, a_BlockY, a_BlockZ, Block, Meta);
	if ((Block != E_BLOCK_TRAPDOOR) && (Block != E_BLOCK_IRON_TRAPDOOR))
	{
		return false;
	}
	
	return (Meta & 0x4) > 0;
}





bool cWorld::SetTrapdoorOpen(int a_BlockX, int a_BlockY, int a_BlockZ, bool a_Open)
{
	BLOCKTYPE Block;
	NIBBLETYPE Meta;
	GetBlockTypeMeta(a_BlockX, a_BlockY, a_BlockZ, Block, Meta);
	if ((Block != E_BLOCK_TRAPDOOR) && (Block != E_BLOCK_IRON_TRAPDOOR))
	{
		return false;
	}
	
	bool IsOpen = (Meta & 0x4) > 0;
	if (a_Open != IsOpen)
	{
		SetBlockMeta(a_BlockX, a_BlockY, a_BlockZ, Meta ^ 0x4);
		BroadcastSoundParticleEffect(1003, a_BlockX, a_BlockY, a_BlockZ, 0);
		return true;
	}
	return false;
}





void cWorld::RegenerateChunk(int a_ChunkX, int a_ChunkZ)
{
	m_ChunkMap->MarkChunkRegenerating(a_ChunkX, a_ChunkZ);
	
	m_Generator.QueueGenerateChunk(a_ChunkX, a_ChunkZ, true);
}





void cWorld::GenerateChunk(int a_ChunkX, int a_ChunkZ)
{
	m_ChunkMap->GenerateChunk(a_ChunkX, a_ChunkZ);
}





void cWorld::QueueLightChunk(int a_ChunkX, int a_ChunkZ, cChunkCoordCallback * a_Callback)
{
	m_Lighting.QueueChunk(a_ChunkX, a_ChunkZ, a_Callback);
}





bool cWorld::IsChunkLighted(int a_ChunkX, int a_ChunkZ)
{
	return m_ChunkMap->IsChunkLighted(a_ChunkX, a_ChunkZ);
}





bool cWorld::ForEachChunkInRect(int a_MinChunkX, int a_MaxChunkX, int a_MinChunkZ, int a_MaxChunkZ, cChunkDataCallback & a_Callback)
{
	return m_ChunkMap->ForEachChunkInRect(a_MinChunkX, a_MaxChunkX, a_MinChunkZ, a_MaxChunkZ, a_Callback);
}





void cWorld::SaveAllChunks(void)
{
	m_LastSave = std::chrono::duration_cast<cTickTimeLong>(m_WorldAge);
	m_ChunkMap->SaveAllChunks();
}





void cWorld::QueueSaveAllChunks(void)
{
	QueueTask(make_unique<cWorld::cTaskSaveAllChunks>());
}





void cWorld::QueueTask(std::unique_ptr<cTask> a_Task)
{
	cCSLock Lock(m_CSTasks);
	m_Tasks.push_back(std::move(a_Task));
}





void cWorld::ScheduleTask(int a_DelayTicks, cTask * a_Task)
{
	Int64 TargetTick = a_DelayTicks + std::chrono::duration_cast<cTickTimeLong>(m_WorldAge).count();
	
	// Insert the task into the list of scheduled tasks, ordered by its target tick
	cCSLock Lock(m_CSScheduledTasks);
	for (cScheduledTasks::iterator itr = m_ScheduledTasks.begin(), end = m_ScheduledTasks.end(); itr != end; ++itr)
	{
		if ((*itr)->m_TargetTick >= TargetTick)
		{
			m_ScheduledTasks.insert(itr, make_unique<cScheduledTask>(TargetTick, a_Task));
			return;
		}
	}
	m_ScheduledTasks.push_back(make_unique<cScheduledTask>(TargetTick, a_Task));
}





void cWorld::AddEntity(cEntity * a_Entity)
{
	cCSLock Lock(m_CSEntitiesToAdd);
	m_EntitiesToAdd.push_back(a_Entity);
}





bool cWorld::HasEntity(int a_UniqueID)
{
	// Check if the entity is in the queue to be added to the world:
	{
		cCSLock Lock(m_CSEntitiesToAdd);
		for (cEntityList::const_iterator itr = m_EntitiesToAdd.begin(), end = m_EntitiesToAdd.end(); itr != end; ++itr)
		{
			if ((*itr)->GetUniqueID() == a_UniqueID)
			{
				return true;
			}
		}  // for itr - m_EntitiesToAdd[]
	}

	// Check if the entity is in the chunkmap:
	if (m_ChunkMap.get() == nullptr)
	{
		// Chunkmap has already been destroyed, there are no entities anymore.
		return false;
	}
	return m_ChunkMap->HasEntity(a_UniqueID);
}





/*
unsigned int cWorld::GetNumPlayers(void)
{
	cCSLock Lock(m_CSPlayers);
	return m_Players.size();
}
*/





int cWorld::GetNumChunks(void) const
{
	return m_ChunkMap->GetNumChunks();
}





void cWorld::GetChunkStats(int & a_NumValid, int & a_NumDirty, int & a_NumInLightingQueue)
{
	m_ChunkMap->GetChunkStats(a_NumValid, a_NumDirty);
	a_NumInLightingQueue = (int) m_Lighting.GetQueueLength();
}





void cWorld::TickQueuedBlocks(void)
{
	if (m_BlockTickQueue.empty())
	{
		return;
	}
	m_BlockTickQueueCopy.clear();
	m_BlockTickQueue.swap(m_BlockTickQueueCopy);

	for (std::vector<BlockTickQueueItem *>::iterator itr = m_BlockTickQueueCopy.begin(); itr != m_BlockTickQueueCopy.end(); ++itr)
	{
		BlockTickQueueItem * Block = (*itr);
		Block->TicksToWait -= 1;
		if (Block->TicksToWait <= 0)
		{
			// TODO: Handle the case when the chunk is already unloaded
			m_ChunkMap->TickBlock(Block->X, Block->Y, Block->Z);
			delete Block;  // We don't have to remove it from the vector, this will happen automatically on the next tick
		}
		else
		{
			m_BlockTickQueue.push_back(Block);  // Keep the block in the queue
		}
	}  // for itr - m_BlockTickQueueCopy[]
}





void cWorld::QueueBlockForTick(int a_BlockX, int a_BlockY, int a_BlockZ, int a_TicksToWait)
{
	BlockTickQueueItem * Block = new BlockTickQueueItem;
	Block->X = a_BlockX;
	Block->Y = a_BlockY;
	Block->Z = a_BlockZ;
	Block->TicksToWait = a_TicksToWait;
	
	m_BlockTickQueue.push_back(Block);
}





bool cWorld::IsBlockDirectlyWatered(int a_BlockX, int a_BlockY, int a_BlockZ)
{
	return (
		IsBlockWater(GetBlock(a_BlockX - 1, a_BlockY, a_BlockZ)) ||
		IsBlockWater(GetBlock(a_BlockX + 1, a_BlockY, a_BlockZ)) ||
		IsBlockWater(GetBlock(a_BlockX,     a_BlockY, a_BlockZ - 1)) ||
		IsBlockWater(GetBlock(a_BlockX,     a_BlockY, a_BlockZ + 1))
	);
}





int cWorld::SpawnMob(double a_PosX, double a_PosY, double a_PosZ, eMonsterType a_MonsterType)
{
	cMonster * Monster = nullptr;

	Monster = cMonster::NewMonsterFromType(a_MonsterType);
	if (Monster != nullptr)
	{
		Monster->SetPosition(a_PosX, a_PosY, a_PosZ);
	}
	
	return SpawnMobFinalize(Monster);
}




int cWorld::SpawnMobFinalize(cMonster * a_Monster)
{
	// Invalid cMonster object. Bail out.
	if (!a_Monster)
	{
		return -1;
	}

	// Give the mob  full health.
	a_Monster->SetHealth(a_Monster->GetMaxHealth());

	// A plugin doesn't agree with the spawn. bail out.
	if (cPluginManager::Get()->CallHookSpawningMonster(*this, *a_Monster))
	{
		delete a_Monster;
		a_Monster = nullptr;
		return -1;
	}

	// Initialize the monster into the current world.
	if (!a_Monster->Initialize(*this))
	{
		delete a_Monster;
		a_Monster = nullptr;
		return -1;
	}

	cPluginManager::Get()->CallHookSpawnedMonster(*this, *a_Monster);

	return a_Monster->GetUniqueID();
}





int cWorld::CreateProjectile(double a_PosX, double a_PosY, double a_PosZ, cProjectileEntity::eKind a_Kind, cEntity * a_Creator, const cItem * a_Item, const Vector3d * a_Speed)
{
	cProjectileEntity * Projectile = cProjectileEntity::Create(a_Kind, a_Creator, a_PosX, a_PosY, a_PosZ, a_Item, a_Speed);
	if (Projectile == nullptr)
	{
		return -1;
	}
	if (!Projectile->Initialize(*this))
	{
		delete Projectile;
		Projectile = nullptr;
		return -1;
	}
	return Projectile->GetUniqueID();
}





void cWorld::TabCompleteUserName(const AString & a_Text, AStringVector & a_Results)
{
	typedef std::pair<AString::size_type, AString> pair_t;
	size_t LastSpace = a_Text.find_last_of(" ");  // Find the position of the last space
	AString LastWord = a_Text.substr(LastSpace + 1, a_Text.length());  // Find the last word

	if (LastWord.empty())
	{
		return;
	}

	std::vector<pair_t> UsernamesByWeight;

	cCSLock Lock(m_CSPlayers);
	for (cPlayerList::iterator itr = m_Players.begin(), end = m_Players.end(); itr != end; ++itr)
	{
		AString PlayerName ((*itr)->GetName());
		if ((*itr)->HasCustomName())
		{
			PlayerName = (*itr)->GetCustomName();
		}

		AString::size_type Found = PlayerName.find(LastWord);  // Try to find last word in playername
		if (Found == AString::npos)
		{
			continue;  // No match
		}

		UsernamesByWeight.push_back(std::make_pair(Found, PlayerName));  // Match! Store it with the position of the match as a weight
	}
	Lock.Unlock();

	std::sort(UsernamesByWeight.begin(), UsernamesByWeight.end());  // Sort lexicographically (by the first value, then second), so higher weights (usernames with match closer to start) come first (#1274)

	/* TODO: Uncomment once migrated to C++11
	std::transform(
		UsernamesByWeight.begin(),
		UsernamesByWeight.end(),
		std::back_inserter(a_Results),
		[](const pair_t & p) { return p.first; }
	);
	*/

	a_Results.reserve(UsernamesByWeight.size());
	for (std::vector<pair_t>::const_iterator itr = UsernamesByWeight.begin(); itr != UsernamesByWeight.end(); ++itr)
	{
		a_Results.push_back(itr->second);
	}
}





void cWorld::SetChunkAlwaysTicked(int a_ChunkX, int a_ChunkZ, bool a_AlwaysTicked)
{
	m_ChunkMap->SetChunkAlwaysTicked(a_ChunkX, a_ChunkZ, a_AlwaysTicked);
}





cRedstoneSimulator * cWorld::InitializeRedstoneSimulator(cIniFile & a_IniFile)
{
	AString SimulatorName = a_IniFile.GetValueSet("Physics", "RedstoneSimulator", "Incremental");

	if (SimulatorName.empty())
	{
		LOGWARNING("[Physics] RedstoneSimulator not present or empty in %s, using the default of \"Incremental\".", GetIniFileName().c_str());
		SimulatorName = "Incremental";
	}
	
	cRedstoneSimulator * res = nullptr;

	if (NoCaseCompare(SimulatorName, "Incremental") == 0)
	{
		res = new cIncrementalRedstoneSimulator(*this);
	}
	else if (NoCaseCompare(SimulatorName, "noop") == 0)
	{
		res = new cRedstoneNoopSimulator(*this);
	}
	
	m_SimulatorManager->RegisterSimulator(res, 1);
	
	return res;
}





cFluidSimulator * cWorld::InitializeFluidSimulator(cIniFile & a_IniFile, const char * a_FluidName, BLOCKTYPE a_SimulateBlock, BLOCKTYPE a_StationaryBlock)
{
	AString SimulatorNameKey;
	Printf(SimulatorNameKey, "%sSimulator", a_FluidName);
	AString SimulatorSectionName;
	Printf(SimulatorSectionName, "%sSimulator", a_FluidName);
	AString SimulatorName = a_IniFile.GetValueSet("Physics", SimulatorNameKey, "Vanilla");
	if (SimulatorName.empty())
	{
		LOGWARNING("[Physics] %s not present or empty in %s, using the default of \"Vanilla\".", SimulatorNameKey.c_str(), GetIniFileName().c_str());
		SimulatorName = "Vanilla";
	}
	
	cFluidSimulator * res = nullptr;
	bool IsWater = (strcmp(a_FluidName, "Water") == 0);  // Used for defaults
	int Rate = 1;
	if (
		(NoCaseCompare(SimulatorName, "vaporize") == 0) ||
		(NoCaseCompare(SimulatorName, "vaporise") == 0)
	)
	{
		res = new cVaporizeFluidSimulator(*this, a_SimulateBlock, a_StationaryBlock);
	}
	else if (
		(NoCaseCompare(SimulatorName, "noop") == 0) ||
		(NoCaseCompare(SimulatorName, "nop") == 0) ||
		(NoCaseCompare(SimulatorName, "null") == 0) ||
		(NoCaseCompare(SimulatorName, "nil") == 0)
	)
	{
		res = new cNoopFluidSimulator(*this, a_SimulateBlock, a_StationaryBlock);
	}
	else
	{
		int Falloff               = a_IniFile.GetValueSetI(SimulatorSectionName, "Falloff",               IsWater ? 1 : 2);
		int TickDelay             = a_IniFile.GetValueSetI(SimulatorSectionName, "TickDelay",             IsWater ? 5 : 30);
		int NumNeighborsForSource = a_IniFile.GetValueSetI(SimulatorSectionName, "NumNeighborsForSource", IsWater ? 2 : -1);
		
		if ((Falloff > 15) || (Falloff < 0))
		{
			LOGWARNING("Falloff for %s simulator is out of range, assuming default of %d", a_FluidName, IsWater ? 1 : 2);
			Falloff = IsWater ? 1 : 2;
		}

		if (NoCaseCompare(SimulatorName, "floody") == 0)
		{
			res = new cFloodyFluidSimulator(*this, a_SimulateBlock, a_StationaryBlock, static_cast<NIBBLETYPE>(Falloff), TickDelay, NumNeighborsForSource);
		}
		else if (NoCaseCompare(SimulatorName, "vanilla") == 0)
		{
			res = new cVanillaFluidSimulator(*this, a_SimulateBlock, a_StationaryBlock, static_cast<NIBBLETYPE>(Falloff), TickDelay, NumNeighborsForSource);
		}
		else
		{
			// The simulator name doesn't match anything we have, issue a warning:
			LOGWARNING("%s [Physics]:%s specifies an unknown simulator, using the default \"Vanilla\".", GetIniFileName().c_str(), SimulatorNameKey.c_str());
			res = new cVanillaFluidSimulator(*this, a_SimulateBlock, a_StationaryBlock, static_cast<NIBBLETYPE>(Falloff), TickDelay, NumNeighborsForSource);
		}
	}
	
	m_SimulatorManager->RegisterSimulator(res, Rate);

	return res;
}






void cWorld::AddQueuedPlayers(void)
{
	ASSERT(m_TickThread.IsCurrentThread());

	// Grab the list of players to add, it has to be locked to access it:
	cPlayerList PlayersToAdd;
	{
		cCSLock Lock(m_CSPlayersToAdd);
		std::swap(PlayersToAdd, m_PlayersToAdd);
	}
	
	// Add all the players in the grabbed list:
	{
		cCSLock Lock(m_CSPlayers);
		for (cPlayerList::iterator itr = PlayersToAdd.begin(), end = PlayersToAdd.end(); itr != end; ++itr)
		{
			ASSERT(std::find(m_Players.begin(), m_Players.end(), *itr) == m_Players.end());  // Is it already in the list? HOW?
			LOGD("Adding player %s to world \"%s\".", (*itr)->GetName().c_str(), m_WorldName.c_str());

			m_Players.push_back(*itr);
			(*itr)->SetWorld(this);

			// Add to chunkmap, if not already there (Spawn vs MoveToWorld):
			m_ChunkMap->AddEntityIfNotPresent(*itr);
		}  // for itr - PlayersToAdd[]
	}  // Lock(m_CSPlayers)

	// Add all the players' clienthandles:
	{
		cCSLock Lock(m_CSClients);
		for (cPlayerList::iterator itr = PlayersToAdd.begin(), end = PlayersToAdd.end(); itr != end; ++itr)
		{
			cClientHandlePtr Client = (*itr)->GetClientHandlePtr();
			if (Client != nullptr)
			{
				m_Clients.push_back(Client);
			}
		}  // for itr - PlayersToAdd[]
	}  // Lock(m_CSClients)

	// Stream chunks to all eligible clients:
	for (cPlayerList::iterator itr = PlayersToAdd.begin(), end = PlayersToAdd.end(); itr != end; ++itr)
	{
		cClientHandle * Client = (*itr)->GetClientHandle();
		if (Client != nullptr)
		{
			Client->SendPlayerMoveLook();
			Client->SendHealth();
			Client->SendWholeInventory(*(*itr)->GetWindow());
		}
	}  // for itr - PlayersToAdd[]
}





////////////////////////////////////////////////////////////////////////////////
// cWorld::cTaskSaveAllChunks:

void cWorld::cTaskSaveAllChunks::Run(cWorld & a_World)
{
	a_World.SaveAllChunks();
}





////////////////////////////////////////////////////////////////////////////////
// cWorld::cTaskUnloadUnusedChunks

void cWorld::cTaskUnloadUnusedChunks::Run(cWorld & a_World)
{
	a_World.UnloadUnusedChunks();
}





////////////////////////////////////////////////////////////////////////////////
// cWorld::cTaskSendBlockTo

cWorld::cTaskSendBlockToAllPlayers::cTaskSendBlockToAllPlayers(std::vector<Vector3i> & a_SendQueue) :
	m_SendQueue(a_SendQueue)
{
}

void cWorld::cTaskSendBlockToAllPlayers::Run(cWorld & a_World)
{
	class cPlayerCallback :
		public cPlayerListCallback
	{
	public:
		cPlayerCallback(std::vector<Vector3i> & a_SendQueue, cWorld & a_CallbackWorld) :
			m_SendQueue(a_SendQueue),
			m_World(a_CallbackWorld)
		{
		}

		virtual bool Item(cPlayer * a_Player)
		{
			for (std::vector<Vector3i>::const_iterator itr = m_SendQueue.begin(); itr != m_SendQueue.end(); ++itr)
			{
				m_World.SendBlockTo(itr->x, itr->y, itr->z, a_Player);
			}
			return false;
		}

	private:

		std::vector<Vector3i> m_SendQueue;
		cWorld & m_World;

	} PlayerCallback(m_SendQueue, a_World);

	a_World.ForEachPlayer(PlayerCallback);
}





////////////////////////////////////////////////////////////////////////////////
// cWorld::cChunkGeneratorCallbacks:

cWorld::cChunkGeneratorCallbacks::cChunkGeneratorCallbacks(cWorld & a_World) :
	m_World(&a_World)
{
}





void cWorld::cChunkGeneratorCallbacks::OnChunkGenerated(cChunkDesc & a_ChunkDesc)
{
	cChunkDef::BlockNibbles BlockMetas;
	a_ChunkDesc.CompressBlockMetas(BlockMetas);

	cSetChunkDataPtr SetChunkData(new cSetChunkData(
		a_ChunkDesc.GetChunkX(), a_ChunkDesc.GetChunkZ(),
		a_ChunkDesc.GetBlockTypes(), BlockMetas,
		nullptr, nullptr,  // We don't have lighting, chunk will be lighted when needed
		&a_ChunkDesc.GetHeightMap(), &a_ChunkDesc.GetBiomeMap(),
		a_ChunkDesc.GetEntities(), a_ChunkDesc.GetBlockEntities(),
		true
	));
	SetChunkData->RemoveInvalidBlockEntities();
	m_World->QueueSetChunkData(SetChunkData);
}





bool cWorld::cChunkGeneratorCallbacks::IsChunkValid(int a_ChunkX, int a_ChunkZ)
{
	return m_World->IsChunkValid(a_ChunkX, a_ChunkZ);
}





bool cWorld::cChunkGeneratorCallbacks::IsChunkQueued(int a_ChunkX, int a_ChunkZ)
{
	return m_World->IsChunkQueued(a_ChunkX, a_ChunkZ);
}





bool cWorld::cChunkGeneratorCallbacks::HasChunkAnyClients(int a_ChunkX, int a_ChunkZ)
{
	return m_World->HasChunkAnyClients(a_ChunkX, a_ChunkZ);
}





void cWorld::cChunkGeneratorCallbacks::CallHookChunkGenerating(cChunkDesc & a_ChunkDesc)
{
	cPluginManager::Get()->CallHookChunkGenerating(
		*m_World, a_ChunkDesc.GetChunkX(), a_ChunkDesc.GetChunkZ(), &a_ChunkDesc
	);
}





void cWorld::cChunkGeneratorCallbacks::CallHookChunkGenerated (cChunkDesc & a_ChunkDesc)
{
	cPluginManager::Get()->CallHookChunkGenerated(
		*m_World, a_ChunkDesc.GetChunkX(), a_ChunkDesc.GetChunkZ(), &a_ChunkDesc
	);
}





