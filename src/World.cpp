
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "World.h"
#include "BlockInfo.h"
#include "ClientHandle.h"
#include "Physics/Explodinator.h"
#include "Server.h"
#include "Root.h"
#include "IniFile.h"
#include "Generating/ChunkDesc.h"
#include "Generating/ComposableGenerator.h"
#include "SetChunkData.h"
#include "DeadlockDetect.h"
#include "LineBlockTracer.h"
#include "UUID.h"
#include "BlockInServerPluginInterface.h"

// Serializers
#include "WorldStorage/ScoreboardSerializer.h"

// Entities (except mobs):
#include "Entities/EnderCrystal.h"
#include "Entities/ExpOrb.h"
#include "Entities/FallingBlock.h"
#include "Entities/Minecart.h"
#include "Entities/Pickup.h"
#include "Entities/Player.h"
#include "Entities/TNTEntity.h"

#include "BlockEntities/CommandBlockEntity.h"
#include "BlockEntities/SignEntity.h"

// Simulators:
#include "Simulator/FloodyFluidSimulator.h"
#include "Simulator/FluidSimulator.h"
#include "Simulator/FireSimulator.h"
#include "Simulator/NoopFluidSimulator.h"
#include "Simulator/NoopRedstoneSimulator.h"
#include "Simulator/IncrementalRedstoneSimulator/IncrementalRedstoneSimulator.h"
#include "Simulator/SandSimulator.h"
#include "Simulator/VanillaFluidSimulator.h"
#include "Simulator/VaporizeFluidSimulator.h"

// Mobs:
#include "Mobs/IncludeAllMonsters.h"
#include "MobCensus.h"
#include "MobSpawner.h"

#include "Generating/Trees.h"
#include "Bindings/PluginManager.h"
#include "Blocks/BlockHandler.h"

// BlockHandler
#include "Blocks/BlockAir.h"
#include "Blocks/BlockCommandBlock.h"
#include "Blocks/BlockLog.h"
#include "Blocks/BlockSapling.h"
#include "Blocks/BlockSignPost.h"
#include "Blocks/BlockTrapdoor.h"
#include "Blocks/BlockWallSign.h"


#ifndef _WIN32
	#include <stdlib.h>
#endif

#include "SpawnPrepare.h"
#include "FastRandom.h"
#include "OpaqueWorld.h"





namespace World
{
	// Implement conversion functions from OpaqueWorld.h
	cBroadcastInterface * GetBroadcastInterface(cWorld * a_World) { return a_World; }
	cForEachChunkProvider * GetFECProvider     (cWorld * a_World) { return a_World; }
	cWorldInterface * GetWorldInterface        (cWorld * a_World) { return a_World; }

	cChunkInterface GetChunkInterface(cWorld & a_World)
	{
		return { a_World.GetChunkMap() };
	}
}





////////////////////////////////////////////////////////////////////////////////
// cWorld::cLock:

cWorld::cLock::cLock(const cWorld & a_World) :
	Super(&(a_World.m_ChunkMap.GetCS()))
{
}





////////////////////////////////////////////////////////////////////////////////
// cWorld::cTickThread:

cWorld::cTickThread::cTickThread(cWorld & a_World):
	Super(fmt::format(FMT_STRING("World Ticker ({})"), a_World.GetName())),
	m_World(a_World)
{
}





void cWorld::cTickThread::Execute()
{
	auto LastTime = std::chrono::steady_clock::now();
	auto TickTime = std::chrono::duration_cast<std::chrono::milliseconds>(1_tick);

	while (!m_ShouldTerminate)
	{
		auto NowTime = std::chrono::steady_clock::now();
		auto WaitTime = std::chrono::duration_cast<std::chrono::milliseconds>(NowTime - LastTime);
		m_World.Tick(WaitTime, TickTime);
		TickTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - NowTime);

		if (TickTime < 1_tick)
		{
			// Stretch tick time until it's at least 1 tick:
			std::this_thread::sleep_for(1_tick - TickTime);
		}

		LastTime = NowTime;
	}
}





////////////////////////////////////////////////////////////////////////////////
// cWorld:

cWorld::cWorld(
	const AString & a_WorldName, const AString & a_DataPath,
	cDeadlockDetect & a_DeadlockDetect, const AStringVector & a_WorldNames,
	eDimension a_Dimension, const AString & a_LinkedOverworldName
):
	m_WorldName(a_WorldName),
	m_DataPath(a_DataPath),
	m_LinkedOverworldName(a_LinkedOverworldName),
	m_IniFileName(m_DataPath + "/world.ini"),
	m_StorageSchema("Default"),
#ifdef __arm__
	m_StorageCompressionFactor(0),
#else
	m_StorageCompressionFactor(6),
#endif
	m_IsSavingEnabled(true),
	m_Dimension(a_Dimension),
	m_IsSpawnExplicitlySet(false),
	m_SpawnX(0),
	m_SpawnY(cChunkDef::Height),
	m_SpawnZ(0),
	m_BroadcastDeathMessages(true),
	m_BroadcastAchievementMessages(true),
	m_IsDaylightCycleEnabled(true),
	m_WorldAge(0),
	m_WorldDate(0),
	m_WorldTickAge(0),
	m_LastChunkCheck(0),
	m_LastSave(0),
	m_SkyDarkness(0),
	m_GameMode(gmSurvival),
	m_bEnabledPVP(false),
	m_bFarmlandTramplingEnabled(false),
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
	m_ChunkMap(this),
	m_bAnimals(true),
	m_Weather(eWeather_Sunny),
	m_WeatherInterval(24000),  // Guaranteed 1 game-day of sunshine at server start :)
	m_MaxSunnyTicks(180000),        // 150 real-world minutes -+
	m_MinSunnyTicks(12000),         // 10 real-world minutes   |
	m_MaxRainTicks(24000),          // 20 real-world minutes   +- all values adapted from Vanilla 1.7.2
	m_MinRainTicks(12000),          // 10 real-world minutes   |
	m_MaxThunderStormTicks(15600),  // 13 real-world minutes   |
	m_MinThunderStormTicks(3600),   // 3 real-world minutes   -+
	m_MaxCactusHeight(3),
	m_MaxSugarcaneHeight(4),
	/* TODO: Enable when functionality exists again
	m_IsBeetrootsBonemealable(true),
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
	m_IsBigFlowerBonemealable(true),
	m_IsTallGrassBonemealable(true),
	*/
	m_bCommandBlocksEnabled(true),
	m_bUseChatPrefixes(false),
	m_TNTShrapnelLevel(slNone),
	m_MaxViewDistance(12),
	m_Scoreboard(this),
	m_MapManager(this),
	m_GeneratorCallbacks(*this),
	m_ChunkSender(*this),
	m_Lighting(*this),
	m_TickThread(*this)
{
	LOGD("cWorld::cWorld(\"%s\")", a_WorldName);

	cFile::CreateFolderRecursive(m_DataPath);

	m_ChunkMap.TrackInDeadlockDetect(a_DeadlockDetect, m_WorldName);

	// Load the scoreboard
	cScoreboardSerializer Serializer(m_DataPath, &m_Scoreboard);
	Serializer.Load();

	// Track the CSs used by this world in the deadlock detector:
	a_DeadlockDetect.TrackCriticalSection(m_CSTasks, fmt::format(FMT_STRING("World {} tasks"), m_WorldName));

	// Load world settings from the ini file
	cIniFile IniFile;
	if (!IniFile.ReadFile(m_IniFileName))
	{
		LOGWARNING("Cannot read world settings from \"%s\", defaults will be used.", m_IniFileName);

		// TODO: More descriptions for each key
		IniFile.AddHeaderComment(" This is the per-world configuration file, managing settings such as generators, simulators, and spawn points");
		IniFile.AddKeyComment(" LinkedWorlds", "This section governs portal world linkage; leave a value blank to disabled that associated method of teleportation");
	}

	// The presence of a configuration value overrides everything
	// If no configuration value is found, GetDimension() is written to file and the variable is written to again to ensure that cosmic rays haven't sneakily changed its value
	m_Dimension = StringToDimension(IniFile.GetValueSet("General", "Dimension", DimensionToString(GetDimension())));
	int UnusedDirtyChunksCap = IniFile.GetValueSetI("General", "UnusedChunkCap", 1000);
	if (UnusedDirtyChunksCap < 0)
	{
		UnusedDirtyChunksCap *= -1;
		IniFile.SetValueI("General", "UnusedChunkCap", UnusedDirtyChunksCap);
	}
	m_UnusedDirtyChunksCap = static_cast<size_t>(UnusedDirtyChunksCap);

	m_BroadcastDeathMessages = IniFile.GetValueSetB("Broadcasting", "BroadcastDeathMessages", true);
	m_BroadcastAchievementMessages = IniFile.GetValueSetB("Broadcasting", "BroadcastAchievementMessages", true);

	SetMaxViewDistance(IniFile.GetValueSetI("SpawnPosition", "MaxViewDistance", cClientHandle::DEFAULT_VIEW_DISTANCE));

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
		m_SpawnX = IniFile.GetValueI("SpawnPosition", "X", m_SpawnX);
		m_SpawnY = IniFile.GetValueI("SpawnPosition", "Y", m_SpawnY);
		m_SpawnZ = IniFile.GetValueI("SpawnPosition", "Z", m_SpawnZ);
	}

	m_StorageSchema               = IniFile.GetValueSet ("Storage",       "Schema",                      m_StorageSchema);
	m_StorageCompressionFactor    = IniFile.GetValueSetI("Storage",       "CompressionFactor",           m_StorageCompressionFactor);
	m_MaxCactusHeight             = IniFile.GetValueSetI("Plants",        "MaxCactusHeight",             3);
	m_MaxSugarcaneHeight          = IniFile.GetValueSetI("Plants",        "MaxSugarcaneHeight",          3);
	/* TODO: Enable when functionality exists again
	m_IsBeetrootsBonemealable     = IniFile.GetValueSetB("Plants",        "IsBeetrootsBonemealable",     true);
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
	m_IsBigFlowerBonemealable     = IniFile.GetValueSetB("Plants",        "IsBigFlowerBonemealable",     true);
	m_IsTallGrassBonemealable     = IniFile.GetValueSetB("Plants",        "IsTallGrassBonemealable",     true);
	*/
	m_IsDeepSnowEnabled           = IniFile.GetValueSetB("Physics",       "DeepSnow",                    true);
	m_ShouldLavaSpawnFire         = IniFile.GetValueSetB("Physics",       "ShouldLavaSpawnFire",         true);
	int TNTShrapnelLevel          = IniFile.GetValueSetI("Physics",       "TNTShrapnelLevel",            static_cast<int>(slAll));
	m_bCommandBlocksEnabled       = IniFile.GetValueSetB("Mechanics",     "CommandBlocksEnabled",        false);
	m_bEnabledPVP                 = IniFile.GetValueSetB("Mechanics",     "PVPEnabled",                  true);
	m_bFarmlandTramplingEnabled   = IniFile.GetValueSetB("Mechanics",     "FarmlandTramplingEnabled",    true);
	m_bUseChatPrefixes            = IniFile.GetValueSetB("Mechanics",     "UseChatPrefixes",             true);
	m_MinNetherPortalWidth        = IniFile.GetValueSetI("Mechanics",     "MinNetherPortalWidth",        2);
	m_MaxNetherPortalWidth        = IniFile.GetValueSetI("Mechanics",     "MaxNetherPortalWidth",        21);
	m_MinNetherPortalHeight       = IniFile.GetValueSetI("Mechanics",     "MinNetherPortalHeight",       3);
	m_MaxNetherPortalHeight       = IniFile.GetValueSetI("Mechanics",     "MaxNetherPortalHeight",       21);
	m_VillagersShouldHarvestCrops = IniFile.GetValueSetB("Monsters",      "VillagersShouldHarvestCrops", true);
	m_IsDaylightCycleEnabled      = IniFile.GetValueSetB("General",       "IsDaylightCycleEnabled",      true);
	int GameMode                  = IniFile.GetValueSetI("General",       "Gamemode",                    static_cast<int>(m_GameMode));
	int Weather                   = IniFile.GetValueSetI("General",       "Weather",                     static_cast<int>(m_Weather));

	m_WorldAge = std::chrono::milliseconds(IniFile.GetValueSetI("General", "WorldAgeMS", 0LL));

	// Load the weather frequency data:
	if (m_Dimension == dimOverworld)
	{
		m_MaxSunnyTicks        = IniFile.GetValueSetI("Weather", "MaxSunnyTicks",        m_MaxSunnyTicks);
		m_MinSunnyTicks        = IniFile.GetValueSetI("Weather", "MinSunnyTicks",        m_MinSunnyTicks);
		m_MaxRainTicks         = IniFile.GetValueSetI("Weather", "MaxRainTicks",         m_MaxRainTicks);
		m_MinRainTicks         = IniFile.GetValueSetI("Weather", "MinRainTicks",         m_MinRainTicks);
		m_MaxThunderStormTicks = IniFile.GetValueSetI("Weather", "MaxThunderStormTicks", m_MaxThunderStormTicks);
		m_MinThunderStormTicks = IniFile.GetValueSetI("Weather", "MinThunderStormTicks", m_MinThunderStormTicks);
		if (m_MaxSunnyTicks < m_MinSunnyTicks)
		{
			std::swap(m_MaxSunnyTicks, m_MinSunnyTicks);
		}
		if (m_MaxRainTicks < m_MinRainTicks)
		{
			std::swap(m_MaxRainTicks, m_MinRainTicks);
		}
		if (m_MaxThunderStormTicks < m_MinThunderStormTicks)
		{
			std::swap(m_MaxThunderStormTicks, m_MinThunderStormTicks);
		}
	}

	auto WorldExists = [&](const AString & a_CheckWorldName)
	{
		return (std::find(a_WorldNames.begin(), a_WorldNames.end(), a_CheckWorldName) != a_WorldNames.end());
	};

	if (a_Dimension == dimOverworld)
	{
		AString MyNetherName = GetName() + "_nether";
		AString MyEndName = GetName() + "_the_end";
		if (!WorldExists(MyNetherName))
		{
			MyNetherName.clear();
		}
		if (!WorldExists(MyEndName))
		{
			MyEndName = GetName() + "_end";
			if (!WorldExists(MyEndName))
			{
				MyEndName.clear();
			}
		}

		m_LinkedNetherWorldName = IniFile.GetValueSet("LinkedWorlds", "NetherWorldName", MyNetherName);
		m_LinkedEndWorldName    = IniFile.GetValueSet("LinkedWorlds", "EndWorldName",    MyEndName);
	}
	else
	{
		m_LinkedOverworldName = IniFile.GetValueSet("LinkedWorlds", "OverworldName", GetLinkedOverworldName());
	}

	// If we are linked to one or more worlds that do not exist, unlink them
	if (a_Dimension == dimOverworld)
	{
		if (!m_LinkedNetherWorldName.empty() && !WorldExists(m_LinkedNetherWorldName))
		{
			IniFile.SetValue("LinkedWorlds", "NetherWorldName", "");
			LOG("%s Is linked to a nonexisting nether world called \"%s\". The server has modified \"%s/world.ini\" and removed this invalid link.",
				GetName().c_str(), m_LinkedNetherWorldName.c_str(), GetName().c_str());
			m_LinkedNetherWorldName.clear();
		}
		if (!m_LinkedEndWorldName.empty() && !WorldExists(m_LinkedEndWorldName))
		{
			IniFile.SetValue("LinkedWorlds", "EndWorldName", "");
			LOG("%s Is linked to a nonexisting end world called \"%s\". The server has modified \"%s/world.ini\" and removed this invalid link.",
				GetName().c_str(), m_LinkedEndWorldName.c_str(), GetName().c_str());
			m_LinkedEndWorldName.clear();
		}
	}
	else
	{
		if (!m_LinkedOverworldName.empty() && !WorldExists(m_LinkedOverworldName))
		{
			IniFile.SetValue("LinkedWorlds", "OverworldName", "");
			LOG("%s Is linked to a nonexisting overworld called \"%s\". The server has modified \"%s/world.ini\" and removed this invalid link.",
				GetName().c_str(), m_LinkedOverworldName.c_str(), GetName().c_str());
			m_LinkedOverworldName.clear();
		}
	}



	// Adjust the enum-backed variables into their respective bounds:
	m_GameMode         = static_cast<eGameMode>     (Clamp<int>(GameMode,         gmSurvival, gmSpectator));
	m_TNTShrapnelLevel = static_cast<eShrapnelLevel>(Clamp<int>(TNTShrapnelLevel, slNone,     slAll));
	m_Weather          = static_cast<eWeather>      (Clamp<int>(Weather,          wSunny,     wStorm));

	cComposableGenerator::InitializeGeneratorDefaults(IniFile, m_Dimension);

	InitializeAndLoadMobSpawningValues(IniFile);
	m_WorldDate = cTickTime(IniFile.GetValueSetI("General", "TimeInTicks", GetWorldDate().count()));

	// preallocate some memory for ticking blocks so we don't need to allocate that often
	m_BlockTickQueue.reserve(1000);
	m_BlockTickQueueCopy.reserve(1000);

	// Simulators:
	m_SimulatorManager  = std::make_unique<cSimulatorManager>(*this);
	m_WaterSimulator    = InitializeFluidSimulator(IniFile, "Water", BlockType::Water);
	m_LavaSimulator     = InitializeFluidSimulator(IniFile, "Lava",  BlockType::Lava);
	m_SandSimulator     = std::make_unique<cSandSimulator>(*this, IniFile);
	m_FireSimulator     = std::make_unique<cFireSimulator>(*this, IniFile);
	m_RedstoneSimulator = InitializeRedstoneSimulator(IniFile);

	// Water, Lava and Redstone simulators get registered in their initialize function.
	m_SimulatorManager->RegisterSimulator(m_SandSimulator.get(), 1);
	m_SimulatorManager->RegisterSimulator(m_FireSimulator.get(), 1);

	m_Storage.Initialize(*this, m_StorageSchema, m_StorageCompressionFactor);
	m_Generator.Initialize(m_GeneratorCallbacks, m_GeneratorCallbacks, IniFile);

	m_MapManager.LoadMapData();

	// Save any changes that the defaults may have done to the ini file:
	if (!IniFile.WriteFile(m_IniFileName))
	{
		LOGWARNING("Could not write world config to %s", m_IniFileName.c_str());
	}

	// Init of the spawn monster time (as they are supposed to have different spawn rate)
	m_LastSpawnMonster.emplace(cMonster::mfHostile, 0_tick);
	m_LastSpawnMonster.emplace(cMonster::mfPassive, 0_tick);
	m_LastSpawnMonster.emplace(cMonster::mfAmbient, 0_tick);
	m_LastSpawnMonster.emplace(cMonster::mfWater,   0_tick);
}





cWorld::~cWorld()
{
	delete m_WaterSimulator;     m_WaterSimulator    = nullptr;
	delete m_LavaSimulator;      m_LavaSimulator     = nullptr;
	delete m_RedstoneSimulator;  m_RedstoneSimulator = nullptr;
}





void cWorld::CastThunderbolt(int a_BlockX, int a_BlockY, int a_BlockZ)
{
	LOG("CastThunderbolt(int, int, int) is deprecated, use CastThunderbolt(Vector3i) instead");
	CastThunderbolt({a_BlockX, a_BlockY, a_BlockZ});
}





void cWorld::CastThunderbolt(Vector3i a_Block)
{
	BroadcastThunderbolt(a_Block);
	BroadcastSoundEffect("entity.lightning.thunder", a_Block, 50, 1);
}





cTickTime cWorld::GetTimeOfDay(void) const
{
	using namespace std::chrono_literals;

	return std::chrono::duration_cast<cTickTime>(m_WorldDate % 20min);
}





cTickTimeLong cWorld::GetWorldAge(void) const
{
	return std::chrono::duration_cast<cTickTimeLong>(m_WorldAge);
}





cTickTimeLong cWorld::GetWorldDate() const
{
	return std::chrono::duration_cast<cTickTimeLong>(m_WorldDate);
}





cTickTimeLong cWorld::GetWorldTickAge() const
{
	return m_WorldTickAge;
}





void cWorld::SetTimeOfDay(const cTickTime a_TimeOfDay)
{
	using namespace std::chrono_literals;

	m_WorldDate = (m_WorldDate / 20min) * 20min + a_TimeOfDay;
	UpdateSkyDarkness();
	BroadcastTimeUpdate();
}





int cWorld::GetDefaultWeatherInterval(eWeather a_Weather) const
{
	auto & Random = GetRandomProvider();
	switch (a_Weather)
	{
		case eWeather_Sunny:
		{
			return Random.RandInt(m_MinSunnyTicks, m_MaxSunnyTicks);
		}
		case eWeather_Rain:
		{
			return Random.RandInt(m_MinRainTicks, m_MaxRainTicks);
		}
		case eWeather_ThunderStorm:
		{
			return Random.RandInt(m_MinThunderStormTicks, m_MaxThunderStormTicks);
		}
	}
	UNREACHABLE("Unsupported weather");
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

	// Let the plugins know about the change:
	cPluginManager::Get()->CallHookWeatherChanged(*this);
}





void cWorld::ChangeWeather(void)
{
	// In the next tick the weather will be changed
	m_WeatherInterval = 0;
}





bool cWorld::IsWeatherSunnyAt(int a_BlockX, int a_BlockZ) const
{
	return m_ChunkMap.IsWeatherSunnyAt(a_BlockX, a_BlockZ);
}





bool cWorld::IsWeatherWetAt(int a_BlockX, int a_BlockZ)
{
	return m_ChunkMap.IsWeatherWetAt(a_BlockX, a_BlockZ);
}





bool cWorld::IsWeatherWetAtXYZ(const Vector3i a_Position)
{
	return m_ChunkMap.IsWeatherWetAt(a_Position);
}





void cWorld::SetNextBlockToTick(const Vector3i a_BlockPos)
{
	return m_ChunkMap.SetNextBlockToTick(a_BlockPos);
}





bool cWorld::SetSpawn(int a_X, int a_Y, int a_Z)
{
	cIniFile IniFile;

	IniFile.ReadFile(m_IniFileName);
	IniFile.SetValueI("SpawnPosition", "X", a_X);
	IniFile.SetValueI("SpawnPosition", "Y", a_Y);
	IniFile.SetValueI("SpawnPosition", "Z", a_Z);
	if (IniFile.WriteFile(m_IniFileName))
	{
		m_SpawnX = a_X;
		m_SpawnY = a_Y;
		m_SpawnZ = a_Z;
		FLOGD("Spawn set at {}", Vector3i{m_SpawnX, m_SpawnY, m_SpawnZ});
		return true;
	}
	else
	{
		LOGWARNING("Couldn't write new spawn settings to \"%s\".", m_IniFileName.c_str());
	}
	return false;
}





void cWorld::InitializeSpawn(void)
{
	// For the debugging builds, don't make the server build too much world upon start:
	#if !defined(NDEBUG) || defined(ANDROID)
		const int DefaultViewDist = 9;
	#else
		const int DefaultViewDist = 20;  // Always prepare an area 20 chunks across, no matter what the actual cClientHandle::VIEWDISTANCE is
	#endif  // !NDEBUG

	if (!m_IsSpawnExplicitlySet)
	{
		// Spawn position wasn't already explicitly set, enumerate random solid-land coordinate and then write it to the world configuration:
		GenerateRandomSpawn(DefaultViewDist);
	}

	cIniFile IniFile;
	IniFile.ReadFile(m_IniFileName);
	int ViewDist = IniFile.GetValueSetI("SpawnPosition", "PregenerateDistance", DefaultViewDist);
	IniFile.WriteFile(m_IniFileName);

	int ChunkX = 0, ChunkZ = 0;
	cChunkDef::BlockToChunk(m_SpawnX, m_SpawnZ, ChunkX, ChunkZ);
	cSpawnPrepare::PrepareChunks(*this, ChunkX, ChunkZ, ViewDist);
}





void cWorld::Start()
{
	m_Lighting.Start();
	m_Storage.Start();
	m_Generator.Start();
	m_ChunkSender.Start();
	m_TickThread.Start();
}





void cWorld::GenerateRandomSpawn(int a_MaxSpawnRadius)
{
	LOGD("Generating random spawnpoint...");

	// Number of checks to make sure we have a valid biome
	// 100 checks will check across 400 chunks, we should have
	// a valid biome by then.
	static const int BiomeCheckCount = 100;

	// Make sure we are in a valid biome
	Vector3i BiomeOffset = Vector3i(0, 0, 0);
	for (int BiomeCheckIndex = 0; BiomeCheckIndex < BiomeCheckCount; ++BiomeCheckIndex)
	{
		EMCSBiome Biome = GetBiomeAt(BiomeOffset.x, BiomeOffset.z);
		if ((Biome != EMCSBiome::biOcean) && (Biome != EMCSBiome::biFrozenOcean))
		{
			// Found a usable biome
			break;
		}

		BiomeOffset.x += cChunkDef::Width * 4;
	}

	// Check 0, 0 first.
	Vector3i BiomeSpawn = BiomeOffset;
	if (CanSpawnAt(BiomeSpawn.x, BiomeSpawn.y, BiomeSpawn.z))
	{
		SetSpawn(BiomeSpawn.x, BiomeSpawn.y, BiomeSpawn.z);
		auto ChunkPos = cChunkDef::BlockToChunk(BiomeSpawn);
		cSpawnPrepare::PrepareChunks(*this, ChunkPos, a_MaxSpawnRadius);
		FLOGINFO("World \"{}\": Generated spawnpoint position at {}", m_WorldName, BiomeSpawn);
		return;
	}

	// A search grid (searches clockwise around the origin)
	static const int HalfChunk = cChunkDef::Width / 2;
	static const Vector3i ChunkOffset[] =
	{
		Vector3i(0, 0, HalfChunk),
		Vector3i(HalfChunk, 0, HalfChunk),
		Vector3i(HalfChunk, 0, 0),
		Vector3i(HalfChunk, 0, -HalfChunk),
		Vector3i(0, 0, -HalfChunk),
		Vector3i(-HalfChunk, 0, -HalfChunk),
		Vector3i(-HalfChunk, 0, 0),
		Vector3i(-HalfChunk, 0, HalfChunk),
	};

	static const int PerRadiSearchCount = ARRAYCOUNT(ChunkOffset);

	for (int RadiusOffset = 1; RadiusOffset < (a_MaxSpawnRadius * 2); ++RadiusOffset)
	{
		for (int SearchGridIndex = 0; SearchGridIndex < PerRadiSearchCount; ++SearchGridIndex)
		{
			auto PotentialSpawn = BiomeOffset + (ChunkOffset[SearchGridIndex] * RadiusOffset);

			if (CanSpawnAt(PotentialSpawn.x, PotentialSpawn.y, PotentialSpawn.z))
			{
				SetSpawn(PotentialSpawn.x, PotentialSpawn.y, PotentialSpawn.z);

				auto ChunkPos = cChunkDef::BlockToChunk(PotentialSpawn);
				cSpawnPrepare::PrepareChunks(*this, ChunkPos, a_MaxSpawnRadius);

				FLOGINFO("World \"{}\":Generated spawnpoint position at {}", m_WorldName, PotentialSpawn);
				return;
			}
		}
	}

	SetSpawn(BiomeSpawn.x, BiomeSpawn.y, BiomeSpawn.z);
	auto ChunkPos = cChunkDef::BlockToChunk(BiomeSpawn);
	cSpawnPrepare::PrepareChunks(*this, ChunkPos, a_MaxSpawnRadius);
	FLOGWARNING("World \"{}\": Did not find an acceptable spawnpoint. Generated a random spawnpoint position at {}", m_WorldName, BiomeSpawn);
}





void cWorld::FlushPendingBlockChanges()
{
	m_ChunkMap.FlushPendingBlockChanges();
};





bool cWorld::CanSpawnAt(int a_X, int & a_Y, int a_Z)
{
	// All this blocks can only be found above ground.
	// Apart from netherrack (as the Nether is technically a massive cave)
	static constexpr std::array<BlockType, 6> ValidSpawnBlocks =
	{
		BlockType::ShortGrass,
		BlockType::Sand,
		BlockType::Snow,
		BlockType::SnowBlock,
		BlockType::Netherrack,
		BlockType::EndStone
	};


	class cCanSpawnChunkStay:
		public cChunkStay
	{
		cEvent m_ChunksReady;
	public:

		cCanSpawnChunkStay(double a_WorldX, double a_WorldZ)
		{
			auto Chunk = cChunkDef::BlockToChunk(Vector3d{a_WorldX, 0.0, a_WorldZ}.Floor());
			for (int XOffset = -1; XOffset != 2; ++XOffset)
			{
				for (int ZOffset = -1; ZOffset != 2; ++ZOffset)
				{
					Add(Chunk.m_ChunkX + XOffset, Chunk.m_ChunkZ + ZOffset);
				}
			}
		}

		virtual ~cCanSpawnChunkStay() override
		{
			Disable();
		}

		virtual bool OnAllChunksAvailable() override
		{
			m_ChunksReady.Set();
			return false;  // Keep chunk stay active
		}

		virtual void OnChunkAvailable(int, int) override {}
		virtual void OnDisabled() override {}

		void Wait()
		{
			m_ChunksReady.Wait();
		}
	};

	// Use chunk stay to load 3x3 chunk area around a_X, a_Z
	cCanSpawnChunkStay ChunkStay(a_X, a_Z);
	ChunkStay.Enable(m_ChunkMap);
	ChunkStay.Wait();

	// Increase this by two, because we need two more blocks for body and head
	auto Height = GetHeight(static_cast<int>(a_X), static_cast<int>(a_Z));
	ASSERT(Height.has_value());
	static const int HighestSpawnPoint = *Height + 2;
	static const int LowestSpawnPoint = static_cast<int>(HighestSpawnPoint / 2.0f);

	for (int PotentialY = HighestSpawnPoint; PotentialY > LowestSpawnPoint; --PotentialY)
	{
		auto HeadBlock = GetBlock({ a_X, PotentialY, a_Z });

		// Is this block safe for spawning
		if (!cBlockAirHandler::IsBlockAir(HeadBlock))
		{
			continue;
		}

		auto BodyBlock = GetBlock({ a_X, PotentialY - 1, a_Z });

		// Is this block safe for spawning
		if (!cBlockAirHandler::IsBlockAir(BodyBlock))
		{
			continue;
		}

		auto FloorBlock = GetBlock({ a_X, PotentialY - 2, a_Z });

		// Early out - Is the floor block air
		if (!cBlockAirHandler::IsBlockAir(FloorBlock))
		{
			continue;
		}

		// Is the floor block ok
		bool IsValidSpawnBlock = false;
		for (const auto & ValidSpawnBlock : ValidSpawnBlocks)
		{
			IsValidSpawnBlock |= (ValidSpawnBlock == FloorBlock.Type());
		}

		if (!IsValidSpawnBlock)
		{
			continue;
		}

		if (!CheckPlayerSpawnPoint(a_X, PotentialY - 1, a_Z))
		{
			continue;
		}

		a_Y = PotentialY - 1;
		return true;
	}

	// Always set a_Y so it can be used when we fail to find any spawn point
	a_Y = HighestSpawnPoint - 2;
	return false;
}





bool cWorld::CheckPlayerSpawnPoint(Vector3i a_Pos)
{
	// Check height bounds
	if (!cChunkDef::IsValidHeight(a_Pos))
	{
		return false;
	}

	// Check that surrounding blocks are neither solid or liquid
	constexpr std::array<Vector3i, 8> SurroundingCoords =
	{
		{
			{0,  0,  1},
			{1,  0,  1},
			{1,  0,  0},
			{1,  0, -1},
			{0,  0, -1},
			{-1, 0, -1},
			{-1, 0,  0},
			{-1, 0,  1},
		}
	};

	for (const auto & Offset : SurroundingCoords)
	{
		auto BlockType = GetBlock(a_Pos + Offset);
		if (cBlockInfo::IsSolid(BlockType) || IsBlockLiquid(BlockType))
		{
			return true;
		}
	}

	return false;
}





eWeather cWorld::ChooseNewWeather()
{
	// Pick a new weather. Only reasonable transitions allowed:
	switch (m_Weather)
	{
		case eWeather_Sunny:
		case eWeather_ThunderStorm:
		{
			return eWeather_Rain;
		}
		case eWeather_Rain:
		{
			// 1 / 8 chance of turning into a thunderstorm
			return GetRandomProvider().RandBool(0.125) ? eWeather_ThunderStorm : eWeather_Sunny;
		}
	}
	UNREACHABLE("Unsupported weather");
}





void cWorld::InitializeAndLoadMobSpawningValues(cIniFile & a_IniFile)
{
	AString DefaultMonsters;
	switch (m_Dimension)
	{
		case dimOverworld: DefaultMonsters = "bat, cavespider, chicken, cow, creeper, guardian, horse, mooshroom, ocelot, pig, rabbit, sheep, silverfish, skeleton, slime, spider, squid, wolf, zombie"; break;  // TODO Re-add Enderman when bugs are fixed
		case dimNether:    DefaultMonsters = "blaze, ghast, magmacube, witherskeleton, zombiepigman"; break;
		case dimEnd:       DefaultMonsters = ""; break;  // TODO Re-add Enderman when bugs are fixed
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





void cWorld::Stop(cDeadlockDetect & a_DeadlockDetect)
{
	// Write settings to file; these are all plugin changeable values - keep updated!
	cIniFile IniFile;
	IniFile.ReadFile(m_IniFileName);
		if (GetDimension() == dimOverworld)
		{
			IniFile.SetValue("LinkedWorlds", "NetherWorldName", m_LinkedNetherWorldName);
			IniFile.SetValue("LinkedWorlds", "EndWorldName",    m_LinkedEndWorldName);
		}
		else
		{
			IniFile.SetValue("LinkedWorlds", "OverworldName", m_LinkedOverworldName);
		}
		IniFile.SetValueI("Physics", "TNTShrapnelLevel", static_cast<int>(m_TNTShrapnelLevel));
		IniFile.SetValueB("Mechanics", "CommandBlocksEnabled", m_bCommandBlocksEnabled);
		IniFile.SetValueB("Mechanics", "UseChatPrefixes", m_bUseChatPrefixes);
		IniFile.SetValueB("General", "IsDaylightCycleEnabled", m_IsDaylightCycleEnabled);
		IniFile.SetValueI("General", "Weather", static_cast<int>(m_Weather));
		IniFile.SetValueI("General", "TimeInTicks", GetWorldDate().count());
		IniFile.SetValueI("General", "WorldAgeMS", static_cast<Int64>(m_WorldAge.count()));
	IniFile.WriteFile(m_IniFileName);

	m_TickThread.Stop();
	m_Lighting.Stop();
	m_Generator.Stop();
	m_ChunkSender.Stop();
	m_Storage.Stop();  // Waits for thread to finish

	a_DeadlockDetect.UntrackCriticalSection(m_CSTasks);
	m_ChunkMap.UntrackInDeadlockDetect(a_DeadlockDetect);

	if (IsSavingEnabled())
	{
		// Unload the scoreboard
		cScoreboardSerializer Serializer(m_DataPath, &m_Scoreboard);
		Serializer.Save();

		m_MapManager.SaveMapData();
	}
}





void cWorld::Tick(std::chrono::milliseconds a_Dt, std::chrono::milliseconds a_LastTickDurationMSec)
{
	// Notify the plugins:
	cPluginManager::Get()->CallHookWorldTick(*this, a_Dt, a_LastTickDurationMSec);

	m_WorldAge += a_Dt;
	m_WorldTickAge++;

	if (m_IsDaylightCycleEnabled)
	{
		m_WorldDate += a_Dt;

		// Updates the sky darkness based on current time of day:
		UpdateSkyDarkness();

		// Broadcast time update every 64 ticks (3.2 seconds):
		if ((m_WorldTickAge % 64_tick) == 0_tick)
		{
			BroadcastTimeUpdate();
		}
	}

	// Broadcast player list pings every 256 ticks (12.8 seconds):
	if ((m_WorldTickAge % 256_tick) == 0_tick)
	{
		BroadcastPlayerListUpdatePing();
	}

	// Process all clients' buffered actions:
	for (const auto Player : m_Players)
	{
		Player->GetClientHandle()->ProcessProtocolIn();
	}

	TickClients(a_Dt);
	TickQueuedChunkDataSets();
	TickQueuedBlocks();
	m_ChunkMap.Tick(a_Dt);
	TickMobs(a_Dt);
	TickQueuedEntityAdditions();
	m_MapManager.TickMaps();
	TickQueuedTasks();
	TickWeather(static_cast<float>(a_Dt.count()));

	GetSimulatorManager()->Simulate(static_cast<float>(a_Dt.count()));

	// Flush out all clients' buffered data:
	for (const auto Player : m_Players)
	{
		Player->GetClientHandle()->ProcessProtocolOut();
	}

	if (m_WorldAge - m_LastChunkCheck > std::chrono::seconds(10))
	{
		// Unload every 10 seconds
		UnloadUnusedChunks();

		if (m_WorldAge - m_LastSave > std::chrono::minutes(5))
		{
			// Save every 5 minutes
			SaveAllChunks();
		}
		else if (GetNumUnusedDirtyChunks() > m_UnusedDirtyChunksCap)
		{
			// Save if we have too many dirty unused chunks
			SaveAllChunks();
		}
	}
}





void cWorld::TickClients(const std::chrono::milliseconds a_Dt)
{
	for (const auto Player : m_Players)
	{
		Player->GetClientHandle()->Tick(a_Dt);
	}
}





void cWorld::TickWeather(float a_Dt)
{
	UNUSED(a_Dt);
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
	float oldrain = m_RainGradient;
	float oldthunder = m_ThunderGradient;
	if (m_Weather == eWeather_Rain)
	{
		m_RainGradient += 0.01f;
	}
	else
	{
		m_RainGradient -= 0.01f;
	}
	if (m_Weather == eWeather_ThunderStorm)
	{
		m_ThunderGradient += 0.01f;
	}
	else
	{
		m_ThunderGradient -= 0.01f;
	}
	m_RainGradient = std::clamp(m_RainGradient, 0.0F, 1.0F);
	m_ThunderGradient = std::clamp(m_ThunderGradient, 0.0F, 1.0F);
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wfloat-equal"
#endif
	if (oldrain != m_RainGradient)
	{
		BroadcastGameStateChange(eGameStateReason::RainGradientChanged, m_RainGradient);
	}
	if (oldthunder != m_ThunderGradient)
	{
		BroadcastGameStateChange(eGameStateReason::ThunderGradientChanged, m_ThunderGradient);
	}
#ifdef __clang__
#pragma clang diagnostic pop
#endif
	if (m_Weather == eWeather_ThunderStorm)
	{
		// 0.5% chance per tick of thunderbolt
		if (GetRandomProvider().RandBool(0.005))
		{
			// CastThunderbolt({0, 0, 0});  // TODO: find random positions near players to cast thunderbolts. Disabled because it crashes 1.13+ clients
		}
	}
}





void cWorld::TickMobs(std::chrono::milliseconds a_Dt)
{
	// _X 2013_10_22: This is a quick fix for #283 - the world needs to be locked while ticking mobs
	cWorld::cLock Lock(*this);

	// before every Mob action, we have to count them depending on the distance to players, on their family ...
	cMobCensus MobCensus;
	m_ChunkMap.CollectMobCensus(MobCensus);
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
			if (
				(m_LastSpawnMonster[Family] > (m_WorldTickAge - cMonster::GetSpawnDelay(Family))) ||  // Not reached the needed ticks before the next round
				MobCensus.IsCapped(Family)
			)
			{
				continue;
			}
			m_LastSpawnMonster[Family] = m_WorldTickAge;
			cMobSpawner Spawner(Family, m_AllowedMobs);
			if (Spawner.CanSpawnAnything())
			{
				m_ChunkMap.SpawnMobs(Spawner);
				// do the spawn
				for (auto & Mob : Spawner.getSpawned())
				{
					SpawnMobFinalize(std::move(Mob));
				}
			}
		}  // for i - AllFamilies[]
	}  // if (Spawning enabled)

	ForEachEntity([=](cEntity & a_Entity)
		{
			if (!a_Entity.IsMob())
			{
				return false;
			}
			if (!a_Entity.IsTicking())
			{
				return false;
			}

			auto & Monster = static_cast<cMonster &>(a_Entity);
			ASSERT(Monster.GetParentChunk() != nullptr);  // A ticking entity must have a valid parent chunk

			// Tick close mobs
			if (Monster.GetParentChunk()->HasAnyClients())
			{
				Monster.Tick(a_Dt, *(a_Entity.GetParentChunk()));
			}
			// Destroy far hostile mobs except if last target was a player
			else if ((Monster.GetMobFamily() == cMonster::eFamily::mfHostile) && !Monster.WasLastTargetAPlayer())
			{
				if (Monster.GetMobType() != eMonsterType::mtWolf)
				{
					Monster.Destroy();
				}
				else
				{
					auto & Wolf = static_cast<cWolf &>(Monster);
					if (!Wolf.IsAngry() && !Wolf.IsTame())
					{
						Monster.Destroy();
					}
				}
			}
			return false;
		}
	);
}





void cWorld::TickQueuedChunkDataSets()
{
	decltype(m_SetChunkDataQueue) SetChunkDataQueue;
	{
		cCSLock Lock(m_CSSetChunkDataQueue);
		SetChunkDataQueue = std::move(m_SetChunkDataQueue);
	}

	// Set any chunk data that has been queued for setting:
	for (auto & Item : SetChunkDataQueue)
	{
		// A copy of the chunk coordinates since we're moving Item.
		const auto Chunk = Item.Chunk;

		// Set the data:
		m_ChunkMap.SetChunkData(std::move(Item));

		// If a client is requesting this chunk, send it to them:
		cChunkSender & ChunkSender = m_ChunkSender;
		DoWithChunk(
			Chunk.m_ChunkX, Chunk.m_ChunkZ,
			[&ChunkSender](cChunk & a_Chunk)
			{
				if (a_Chunk.HasAnyClients())
				{
					ChunkSender.QueueSendChunkTo(
						a_Chunk.GetPosX(),
						a_Chunk.GetPosZ(),
						cChunkSender::Priority::Medium,
						a_Chunk.GetAllClients()
					);
				}
				return true;
			}
		);
	}  // for itr - SetChunkDataQueue[]
}





void cWorld::TickQueuedEntityAdditions(void)
{
	decltype(m_EntitiesToAdd) EntitiesToAdd;
	{
		cCSLock Lock(m_CSEntitiesToAdd);
		std::swap(EntitiesToAdd, m_EntitiesToAdd);
	}

	// Ensures m_Players manipulation happens under the chunkmap lock.
	cLock Lock(*this);

	// Add entities waiting in the queue to be added:
	for (auto & Item: EntitiesToAdd)
	{
		const auto Entity = Item.first.get();

		if (Entity->IsPlayer())
		{
			const auto Player = static_cast<cPlayer *>(Entity);

			LOGD("Adding player %s to world \"%s\".", Player->GetName().c_str(), m_WorldName.c_str());
			ASSERT(std::find(m_Players.begin(), m_Players.end(), Player) == m_Players.end());  // Is it already in the list? HOW?

			m_Players.push_back(Player);
		}

		m_ChunkMap.AddEntity(std::move(Item.first));

		if (const auto OldWorld = Item.second; OldWorld != nullptr)
		{
			cRoot::Get()->GetPluginManager()->CallHookEntityChangedWorld(*Entity, *OldWorld);
		}
	}
}





void cWorld::TickQueuedTasks(void)
{
	// Move the tasks to be executed to a seperate vector to avoid deadlocks on accessing m_Tasks
	decltype(m_Tasks) Tasks;
	{
		cCSLock Lock(m_CSTasks);
		if (m_Tasks.empty())
		{
			return;
		}

		// Partition everything to be executed by returning false to move to end of list if time reached
		auto MoveBeginIterator = std::partition(m_Tasks.begin(), m_Tasks.end(), [this](const decltype(m_Tasks)::value_type & a_Task)
		{
			return a_Task.first >= m_WorldAge;
		});

		// Cut all the due tasks from m_Tasks into Tasks:
		Tasks.insert(
			Tasks.end(),
			std::make_move_iterator(MoveBeginIterator),
			std::make_move_iterator(m_Tasks.end())
		);
		m_Tasks.erase(MoveBeginIterator, m_Tasks.end());
	}

	// Execute each task:
	for (const auto & Task : Tasks)
	{
		Task.second(*this);
	}  // for itr - m_Tasks[]
}





void cWorld::UpdateSkyDarkness(void)
{
	const auto TIME_SUNSET = 12000_tick;
	const auto TIME_NIGHT_START = 13187_tick;
	const auto TIME_NIGHT_END = 22812_tick;
	const auto TIME_SUNRISE = 23999_tick;
	const auto TIME_SPAWN_DIVISOR = 148_tick;

	const auto TempTime = GetTimeOfDay();
	if (TempTime <= TIME_SUNSET)
	{
		m_SkyDarkness = 0;
	}
	else if (TempTime <= TIME_NIGHT_START)
	{
		m_SkyDarkness = static_cast<LIGHTTYPE>((TIME_NIGHT_START - TempTime) / TIME_SPAWN_DIVISOR);
	}
	else if (TempTime <= TIME_NIGHT_END)
	{
		m_SkyDarkness = 8;
	}
	else
	{
		m_SkyDarkness = static_cast<LIGHTTYPE>((TIME_SUNRISE - TempTime) / TIME_SPAWN_DIVISOR);
	}
}





void cWorld::WakeUpSimulators(Vector3i a_Block)
{
	return m_ChunkMap.WakeUpSimulators(a_Block);
}





void cWorld::WakeUpSimulatorsInArea(const cCuboid & a_Area)
{
	m_SimulatorManager->WakeUp(a_Area);
}





bool cWorld::ForEachBlockEntityInChunk(int a_ChunkX, int a_ChunkZ, cBlockEntityCallback a_Callback)
{
	return m_ChunkMap.ForEachBlockEntityInChunk(a_ChunkX, a_ChunkZ, a_Callback);
}





void cWorld::DoExplosionAt(double a_ExplosionSize, double a_BlockX, double a_BlockY, double a_BlockZ, bool a_CanCauseFire, eExplosionSource a_Source, void * a_SourceData)
{
	cLock Lock(*this);
	if (!cPluginManager::Get()->CallHookExploding(*this, a_ExplosionSize, a_CanCauseFire, a_BlockX, a_BlockY, a_BlockZ, a_Source, a_SourceData) && (a_ExplosionSize > 0))
	{
		// TODO: CanCauseFire gets reset to false for some reason, (plugin has ability to change it, might be related)

		const cEntity * Entity;
		switch (a_Source)
		{
			case eExplosionSource::esEnderCrystal:
			case eExplosionSource::esGhastFireball:
			case eExplosionSource::esMonster:
			case eExplosionSource::esPrimedTNT:
			case eExplosionSource::esTNTMinecart:
			case eExplosionSource::esWitherBirth:
			case eExplosionSource::esWitherSkull:
			{
				Entity = static_cast<const cEntity *>(a_SourceData);
				break;
			}
			default:
			{
				Entity = nullptr;
			}
		}

		Explodinator::Kaboom(*this, Vector3d(a_BlockX, a_BlockY, a_BlockZ), FloorC(a_ExplosionSize), a_CanCauseFire, Entity);
		cPluginManager::Get()->CallHookExploded(*this, a_ExplosionSize, a_CanCauseFire, a_BlockX, a_BlockY, a_BlockZ, a_Source, a_SourceData);
	}
}





bool cWorld::DoWithBlockEntityAt(const Vector3i a_Position, cBlockEntityCallback a_Callback)
{
	return m_ChunkMap.DoWithBlockEntityAt(a_Position, a_Callback);
}





bool cWorld::GetSignLines(int a_BlockX, int a_BlockY, int a_BlockZ, AString & a_Line1, AString & a_Line2, AString & a_Line3, AString & a_Line4)
{
	return DoWithBlockEntityAt({ a_BlockX, a_BlockY, a_BlockZ }, [&a_Line1, &a_Line2, &a_Line3, &a_Line4](cBlockEntity & a_BlockEntity)
	{
		if (!cBlockWallSignHandler::IsBlockWallSign(a_BlockEntity.GetBlock()) && !cBlockSignPostHandler::IsBlockSignPost(a_BlockEntity.GetBlock()))
		{
			return false;  // Not a sign
		}

		const auto & Sign = static_cast<cSignEntity &>(a_BlockEntity);
		a_Line1 = Sign.GetLine(0);
		a_Line2 = Sign.GetLine(1);
		a_Line3 = Sign.GetLine(2);
		a_Line4 = Sign.GetLine(3);
		return true;
	});
}





bool cWorld::DoWithChunk(int a_ChunkX, int a_ChunkZ, cChunkCallback a_Callback)
{
	return m_ChunkMap.DoWithChunk(a_ChunkX, a_ChunkZ, a_Callback);
}





bool cWorld::DoWithChunkAt(Vector3i a_BlockPos, cChunkCallback a_Callback)
{
	return m_ChunkMap.DoWithChunkAt(a_BlockPos, a_Callback);
}





bool cWorld::GrowTree(const Vector3i a_BlockPos)
{
	if (cBlockSaplingHandler::IsBlockSapling(GetBlock(a_BlockPos)))
	{
		// There is a sapling here, grow a tree according to its type:
		return GrowTreeFromSapling(a_BlockPos);
	}
	else
	{
		// There is nothing here, grow a tree based on the current biome here:
		return GrowTreeByBiome(a_BlockPos);
	}
}





bool cWorld::GrowTreeFromSapling(Vector3i a_BlockPos)
{
	cNoise Noise(m_Generator.GetSeed());
	sSetBlockVector Logs, Other;
	auto WorldAge = static_cast<int>(m_WorldTickAge.count() & 0xffffffff);
	auto Sapling = GetBlock(a_BlockPos);
	switch (Sapling.Type())
	{
		case BlockType::OakSapling:    GetAppleTreeImage  (a_BlockPos, Noise, WorldAge, Logs, Other); break;
		case BlockType::BirchSapling:    GetBirchTreeImage  (a_BlockPos, Noise, WorldAge, Logs, Other); break;
		case BlockType::SpruceSapling:
		{
			bool IsLarge = GetLargeTreeAdjustment(a_BlockPos, Sapling);
			GetConiferTreeImage(a_BlockPos, Noise, WorldAge, Logs, Other, IsLarge);
			break;
		}
		case BlockType::AcaciaSapling:   GetAcaciaTreeImage (a_BlockPos, Noise, WorldAge, Logs, Other); break;
		case BlockType::JungleSapling:
		{
			bool IsLarge = GetLargeTreeAdjustment(a_BlockPos, Sapling);
			GetJungleTreeImage(a_BlockPos, Noise, WorldAge, Logs, Other, IsLarge);
			break;
		}
		case BlockType::DarkOakSapling:
		{
			if (!GetLargeTreeAdjustment(a_BlockPos, Sapling))
			{
				return false;
			}

			GetDarkoakTreeImage(a_BlockPos, Noise, WorldAge, Logs, Other);
			break;
		}
		default:
		{
			FLOGWARNING("{}: Got unknown block type: {}", __FUNCTION__, Sapling);
			break;
		}
	}
	Other.insert(Other.begin(), Logs.begin(), Logs.end());
	Logs.clear();
	return GrowTreeImage(Other);
}





bool cWorld::GetLargeTreeAdjustment(Vector3i & a_BlockPos, BlockState a_Block)
{
	bool IsLarge = true;

	// Check to see if we are the northwest corner
	for (int x = 0; x  < 2; ++x)
	{
		for (int z = 0; z < 2; ++z)
		{
			BlockState OtherSapling;
			if (GetBlock(a_BlockPos.addedXZ(x, z), OtherSapling))
			{
				return false;
			}
			IsLarge = IsLarge && (OtherSapling.Type() == a_Block.Type());
		}
	}

	if (IsLarge)
	{
		return true;
	}

	IsLarge = true;
	// Check to see if we are the southwest corner
	for (int x = 0; x  < 2; ++x)
	{
		for (int z = 0; z > -2; --z)
		{
			BlockState OtherSapling;
			if (GetBlock(a_BlockPos.addedXZ(x, z), OtherSapling))
			{
				return false;
			}
			IsLarge = IsLarge && (OtherSapling.Type() == a_Block.Type());
		}
	}

	if (IsLarge)
	{
		--a_BlockPos.z;
		return true;
	}

	IsLarge = true;
	// Check to see if we are the southeast corner
	for (int x = 0; x > -2; --x)
	{
		for (int z = 0; z > -2; --z)
		{
			BlockState OtherSapling;
			if (GetBlock(a_BlockPos.addedXZ(x, z), OtherSapling))
			{
				return false;
			}
			IsLarge = IsLarge && (OtherSapling.Type() == a_Block.Type());
		}
	}

	if (IsLarge)
	{
		--a_BlockPos.x;
		--a_BlockPos.z;
		return true;
	}

	IsLarge = true;
	// Check to see if we are the northeast corner
	for (int x = 0; x > -2; --x)
	{
		for (int z = 0; z < 2; ++z)
		{
			BlockState OtherSapling;
			if (GetBlock(a_BlockPos.addedXZ(x, z), OtherSapling))
			{
				return false;
			}
			IsLarge = IsLarge && (OtherSapling.Type() == a_Block.Type());
		}
	}

	if (IsLarge)
	{
		--a_BlockPos.x;
	}

	return IsLarge;
}





bool cWorld::GrowTreeByBiome(const Vector3i a_BlockPos)
{
	cNoise Noise(m_Generator.GetSeed());
	sSetBlockVector Logs, Other;
	auto seq = static_cast<int>(m_WorldTickAge.count() & 0xffffffff);
	GetTreeImageByBiome(a_BlockPos, Noise, seq, GetBiomeAt(a_BlockPos.x, a_BlockPos.z), Logs, Other);
	Other.insert(Other.begin(), Logs.begin(), Logs.end());
	Logs.clear();
	return GrowTreeImage(Other);
}





bool cWorld::GrowTreeImage(const sSetBlockVector & a_Blocks)
{
	// Check that the tree has place to grow

	// Make a copy of the log blocks:
	sSetBlockVector CopyBlocks;
	for (const auto & BlockToSet : a_Blocks)
	{
		if (cBlockLogHandler::IsBlockLog(BlockToSet.m_Block))
		{
			CopyBlocks.push_back(BlockToSet);
		}
	}

	// Query blocktypes and metas at those log blocks:
	if (!GetBlocks(CopyBlocks, false))
	{
		return false;
	}

	// Check that at each log's coord there's an block allowed to be overwritten:
	for (const auto & BlockToSet : CopyBlocks)
	{
		switch (BlockToSet.m_Block.Type())
		{
			CASE_TREE_ALLOWED_BLOCKS:
			{
				break;
			}
			default:
			{
				return false;
			}
		}
	}

	// All ok, replace blocks with the tree image:
	m_ChunkMap.ReplaceTreeBlocks(a_Blocks);
	return true;
}





int cWorld::GrowPlantAt(Vector3i a_BlockPos, char a_NumStages)
{
	return m_ChunkMap.GrowPlantAt(a_BlockPos, a_NumStages);
}





bool cWorld::GrowRipePlant(Vector3i a_BlockPos)
{
	return (GrowPlantAt(a_BlockPos, 16) > 0);
}





EMCSBiome cWorld::GetBiomeAt (int a_BlockX, int a_BlockZ)
{
	return m_ChunkMap.GetBiomeAt(a_BlockX, a_BlockZ);
}





bool cWorld::SetBiomeAt(int a_BlockX, int a_BlockZ, EMCSBiome a_Biome)
{
	return m_ChunkMap.SetBiomeAt(a_BlockX, a_BlockZ, a_Biome);
}





bool cWorld::SetAreaBiome(int a_MinX, int a_MaxX, int a_MinZ, int a_MaxZ, EMCSBiome a_Biome)
{
	return m_ChunkMap.SetAreaBiome(a_MinX, a_MaxX, a_MinZ, a_MaxZ, a_Biome);
}





bool cWorld::SetAreaBiome(const cCuboid & a_Area, EMCSBiome a_Biome)
{
	return SetAreaBiome(
		std::min(a_Area.p1.x, a_Area.p2.x), std::max(a_Area.p1.x, a_Area.p2.x),
		std::min(a_Area.p1.z, a_Area.p2.z), std::max(a_Area.p1.z, a_Area.p2.z),
		a_Biome
	);
}





void cWorld::SetMaxViewDistance(int a_MaxViewDistance)
{
	m_MaxViewDistance = Clamp(a_MaxViewDistance, cClientHandle::MIN_VIEW_DISTANCE, cClientHandle::MAX_VIEW_DISTANCE);
}





void cWorld::SetBlock(Vector3i a_BlockPos, BlockState a_Block)
{
	m_ChunkMap.SetBlock(a_BlockPos, a_Block);
}





LIGHTTYPE cWorld::GetBlockSkyLight(Vector3i a_BlockPos) const
{
	return m_ChunkMap.GetBlockSkyLight(a_BlockPos);
}





LIGHTTYPE cWorld::GetBlockBlockLight(Vector3i a_BlockPos) const
{
	return m_ChunkMap.GetBlockBlockLight(a_BlockPos);
}





bool cWorld::GetBlock(Vector3i a_BlockPos, BlockState & a_Block) const
{
	return m_ChunkMap.GetBlock(a_BlockPos, a_Block);
}





bool cWorld::GetBlockInfo(Vector3i a_BlockPos, BlockState & a_Block, LIGHTTYPE & a_SkyLight, LIGHTTYPE & a_BlockLight) const
{
	return m_ChunkMap.GetBlockInfo(a_BlockPos, a_Block, a_SkyLight, a_BlockLight);
}





bool cWorld::WriteBlockArea(cBlockArea & a_Area, int a_MinBlockX, int a_MinBlockY, int a_MinBlockZ, int a_DataTypes)
{
	return m_ChunkMap.WriteBlockArea(a_Area, a_MinBlockX, a_MinBlockY, a_MinBlockZ, a_DataTypes);
}





void cWorld::SpawnItemPickups(const cItems & a_Pickups, Vector3i a_BlockPos, double a_FlyAwaySpeed, bool a_IsPlayerCreated)
{
	auto & random = GetRandomProvider();
	auto microX = random.RandReal<double>(0, 1);
	auto microZ = random.RandReal<double>(0, 1);
	return SpawnItemPickups(a_Pickups, Vector3d(microX, 0, microZ) + a_BlockPos, a_FlyAwaySpeed, a_IsPlayerCreated);
}





void cWorld::SpawnItemPickups(const cItems & a_Pickups, Vector3d a_Pos, double a_FlyAwaySpeed, bool a_IsPlayerCreated)
{
	auto & Random = GetRandomProvider();
	a_FlyAwaySpeed /= 100;  // Pre-divide, so that we don't have to divide each time inside the loop
	for (const auto & Pickup : a_Pickups)
	{
		if (Pickup.m_ItemType == Item::Air)
		{
			// Don't spawn pickup if item isn't even valid; should prevent client crashing too
			continue;
		}

		float SpeedX = static_cast<float>(a_FlyAwaySpeed * Random.RandInt(-10, 10));
		float SpeedY = static_cast<float>(a_FlyAwaySpeed * Random.RandInt(40, 50));
		float SpeedZ = static_cast<float>(a_FlyAwaySpeed * Random.RandInt(-10, 10));

		auto PickupEntity = std::make_unique<cPickup>(a_Pos, Pickup, a_IsPlayerCreated, Vector3f{SpeedX, SpeedY, SpeedZ});
		auto PickupPtr = PickupEntity.get();
		PickupPtr->Initialize(std::move(PickupEntity), *this);
	}
}





void cWorld::SpawnItemPickups(const cItems & a_Pickups, Vector3d a_Pos, Vector3d a_Speed, bool a_IsPlayerCreated)
{
	for (const auto & Pickup : a_Pickups)
	{
		if (Pickup.m_ItemType == Item::Air)
		{
			continue;
		}

		auto PickupEntity = std::make_unique<cPickup>(a_Pos, Pickup, a_IsPlayerCreated, a_Speed);
		auto PickupPtr = PickupEntity.get();
		PickupPtr->Initialize(std::move(PickupEntity), *this);
	}
}





UInt32 cWorld::SpawnItemPickup(Vector3d a_Pos, const cItem & a_Item, Vector3f a_Speed, int a_LifetimeTicks, bool a_CanCombine)
{
	auto pickup = std::make_unique<cPickup>(a_Pos, a_Item, false, a_Speed, a_LifetimeTicks, a_CanCombine);
	auto pickupPtr = pickup.get();
	if (!pickupPtr->Initialize(std::move(pickup), *this))
	{
		return cEntity::INVALID_ID;
	}
	return pickupPtr->GetUniqueID();
}





UInt32 cWorld::SpawnFallingBlock(Vector3d a_Pos, BlockState a_Block)
{
	auto fallingBlock = std::make_unique<cFallingBlock>(a_Pos, a_Block);
	auto fallingBlockPtr = fallingBlock.get();
	auto ID = fallingBlock->GetUniqueID();
	if (!fallingBlockPtr->Initialize(std::move(fallingBlock), *this))
	{
		return cEntity::INVALID_ID;
	}
	return ID;
}





UInt32 cWorld::SpawnExperienceOrb(Vector3d a_Pos, int a_Reward)
{
	if (a_Reward < 1)
	{
		LOGWARNING("%s: Attempting to create an experience orb with non-positive reward!", __FUNCTION__);
		return cEntity::INVALID_ID;
	}

	auto expOrb = std::make_unique<cExpOrb>(a_Pos, a_Reward);
	auto expOrbPtr = expOrb.get();
	if (!expOrbPtr->Initialize(std::move(expOrb), *this))
	{
		return cEntity::INVALID_ID;
	}
	return expOrbPtr->GetUniqueID();
}





std::vector<UInt32> cWorld::SpawnSplitExperienceOrbs(Vector3d a_Pos, int a_Reward)
{
	std::vector<UInt32> OrbsID;

	if (a_Reward < 1)
	{
		LOGWARNING("%s: Attempting to create an experience orb with non-positive reward!", __FUNCTION__);
		return OrbsID;
	}

	std::vector<int> Rewards = cExpOrb::Split(a_Reward);

	// Check generate number to decide speed limit (distribute range)
	float SpeedLimit = static_cast<float>((Rewards.size() / 2) + 5);
	if (SpeedLimit > 10)
	{
		SpeedLimit = 10;
	}

	auto & Random = GetRandomProvider();
	for (auto Reward : Rewards)
	{
		auto ExpOrb = std::make_unique<cExpOrb>(a_Pos, Reward);
		auto ExpOrbPtr = ExpOrb.get();
		double SpeedX = Random.RandReal(-SpeedLimit, SpeedLimit);
		double SpeedY = Random.RandReal(0.5);
		double SpeedZ = Random.RandReal(-SpeedLimit, SpeedLimit);
		ExpOrbPtr->SetSpeed(SpeedX, SpeedY, SpeedZ);

		UInt32 Id = ExpOrbPtr->GetUniqueID();
		if (ExpOrbPtr->Initialize(std::move(ExpOrb), *this))
		{
			OrbsID.push_back(Id);
		}
	}

	return OrbsID;
}





UInt32 cWorld::SpawnMinecart(Vector3d a_Pos, Item a_MinecartType, const cItem & a_Content, int a_BlockHeight)
{
	std::unique_ptr<cMinecart> Minecart;
	switch (a_MinecartType)
	{
		case Item::Minecart:        Minecart = std::make_unique<cRideableMinecart>   (a_Pos, a_Content, a_BlockHeight); break;
		case Item::ChestMinecart:   Minecart = std::make_unique<cMinecartWithChest>  (a_Pos); break;
		case Item::FurnaceMinecart: Minecart = std::make_unique<cMinecartWithFurnace>(a_Pos); break;
		case Item::TNTMinecart:     Minecart = std::make_unique<cMinecartWithTNT>    (a_Pos); break;
		case Item::HopperMinecart:  Minecart = std::make_unique<cMinecartWithHopper> (a_Pos); break;
		default:
		{
			return cEntity::INVALID_ID;
		}
	}  // switch (a_MinecartType)

	auto MinecartPtr = Minecart.get();
	if (!MinecartPtr->Initialize(std::move(Minecart), *this))
	{
		return cEntity::INVALID_ID;
	}
	return MinecartPtr->GetUniqueID();
}





UInt32 cWorld::SpawnBoat(Vector3d a_Pos, cBoat::eMaterial a_Material)
{
	auto Boat = std::make_unique<cBoat>(a_Pos, a_Material);
	auto BoatPtr = Boat.get();
	if (!BoatPtr->Initialize(std::move(Boat), *this))
	{
		return cEntity::INVALID_ID;
	}
	return BoatPtr->GetUniqueID();
}





UInt32 cWorld::SpawnPrimedTNT(Vector3d a_Pos, int a_FuseTicks, double a_InitialVelocityCoeff, bool a_ShouldPlayFuseSound)
{
	auto TNT = std::make_unique<cTNTEntity>(a_Pos, a_FuseTicks);
	auto TNTPtr = TNT.get();
	if (!TNTPtr->Initialize(std::move(TNT), *this))
	{
		return cEntity::INVALID_ID;
	}

	if (a_ShouldPlayFuseSound)
	{
		BroadcastSoundEffect("entity.tnt.primed", a_Pos, 1.0f, 1.0f);
	}

	auto & Random = GetRandomProvider();
	TNTPtr->SetSpeed(
		a_InitialVelocityCoeff * Random.RandReal(-0.5f, 0.5f),
		a_InitialVelocityCoeff * 2,
		a_InitialVelocityCoeff * Random.RandReal(-0.5f, 0.5f)
	);
	return TNTPtr->GetUniqueID();
}





UInt32 cWorld::SpawnEnderCrystal(Vector3d a_Pos, bool a_ShowBottom)
{
	auto EnderCrystal = std::make_unique<cEnderCrystal>(a_Pos, a_ShowBottom);
	auto EnderCrystalPtr = EnderCrystal.get();
	if (!EnderCrystalPtr->Initialize(std::move(EnderCrystal), *this))
	{
		return cEntity::INVALID_ID;
	}
	return EnderCrystalPtr->GetUniqueID();
}





void cWorld::PlaceBlock(const Vector3i a_Position, BlockState a_Block)
{
	BlockState OldBlock;

	if (!GetBlock(a_Position, OldBlock))
	{
		return;
	}


	SetBlock(a_Position, a_Block);

	cChunkInterface ChunkInterface(GetChunkMap());
	cBlockHandler::For(OldBlock.Type()).OnBroken(ChunkInterface, *this, a_Position, OldBlock, nullptr);
	cBlockHandler::For(a_Block.Type()).OnPlaced(ChunkInterface, *this, a_Position, a_Block);
}





bool cWorld::GetBlocks(sSetBlockVector & a_Blocks, bool a_ContinueOnFailure)
{
	return m_ChunkMap.GetBlocks(a_Blocks, a_ContinueOnFailure);
}





bool cWorld::DigBlock(Vector3i a_BlockPos, const cEntity * a_Digger)
{
	BlockState Block;
	if (!GetBlock(a_BlockPos, Block))
	{
		return false;
	}

	if (!m_ChunkMap.DigBlock(a_BlockPos))
	{
		return false;
	}

	cChunkInterface ChunkInterface(GetChunkMap());
	cBlockHandler::For(Block.Type()).OnBroken(ChunkInterface, *this, a_BlockPos, Block, a_Digger);

	return true;
}





bool cWorld::DropBlockAsPickups(Vector3i a_BlockPos, const cEntity * a_Digger, const cItem * a_Tool)
{
	auto pickups = PickupsFromBlock(a_BlockPos, a_Digger, a_Tool);
	if (!DigBlock(a_BlockPos, a_Digger))
	{
		return false;
	}
	SpawnItemPickups(pickups, Vector3d(0.5, 0.5, 0.5) + a_BlockPos, 10);
	return true;
}





cItems cWorld::PickupsFromBlock(Vector3i a_BlockPos, const cEntity * a_Digger, const cItem * a_Tool)
{
	return m_ChunkMap.PickupsFromBlock(a_BlockPos, a_Digger, a_Tool);
}





void cWorld::SendBlockTo(int a_X, int a_Y, int a_Z, const cPlayer & a_Player)
{
	m_ChunkMap.SendBlockTo(a_X, a_Y, a_Z, a_Player);
}





std::optional<int> cWorld::GetHeight(int a_X, int a_Z)
{
	return m_ChunkMap.GetHeight(a_X, a_Z);
}





void cWorld::SendBlockEntity(int a_BlockX, int a_BlockY, int a_BlockZ, cClientHandle & a_Client)
{
	m_ChunkMap.SendBlockEntity(a_BlockX, a_BlockY, a_BlockZ, a_Client);
}





void cWorld::MarkChunkDirty(int a_ChunkX, int a_ChunkZ)
{
	m_ChunkMap.MarkChunkDirty(a_ChunkX, a_ChunkZ);
}





void cWorld::MarkChunkSaving(int a_ChunkX, int a_ChunkZ)
{
	m_ChunkMap.MarkChunkSaving(a_ChunkX, a_ChunkZ);
}





void cWorld::MarkChunkSaved (int a_ChunkX, int a_ChunkZ)
{
	m_ChunkMap.MarkChunkSaved (a_ChunkX, a_ChunkZ);
}





void cWorld::QueueSetChunkData(struct SetChunkData && a_SetChunkData)
{
	// Store a copy of the data in the queue:
	// TODO: If the queue is too large, wait for it to get processed. Not likely, though.
	cCSLock Lock(m_CSSetChunkDataQueue);
	m_SetChunkDataQueue.emplace_back(std::move(a_SetChunkData));
}





void cWorld::ChunkLighted(
	int a_ChunkX, int a_ChunkZ,
	const cChunkDef::LightNibbles & a_BlockLight,
	const cChunkDef::LightNibbles & a_SkyLight
)
{
	m_ChunkMap.ChunkLighted(a_ChunkX, a_ChunkZ, a_BlockLight, a_SkyLight);
}





bool cWorld::GetChunkData(cChunkCoords a_Coords, cChunkDataCallback & a_Callback) const
{
	return m_ChunkMap.GetChunkData(a_Coords, a_Callback);
}





bool cWorld::IsChunkQueued(int a_ChunkX, int a_ChunkZ) const
{
	return m_ChunkMap.IsChunkQueued(a_ChunkX, a_ChunkZ);
}





bool cWorld::IsChunkValid(int a_ChunkX, int a_ChunkZ) const
{
	return m_ChunkMap.IsChunkValid(a_ChunkX, a_ChunkZ);
}





bool cWorld::HasChunkAnyClients(int a_ChunkX, int a_ChunkZ) const
{
	return m_ChunkMap.HasChunkAnyClients(a_ChunkX, a_ChunkZ);
}





void cWorld::UnloadUnusedChunks(void)
{
	m_LastChunkCheck = m_WorldAge;
	m_ChunkMap.UnloadUnusedChunks();
}





void cWorld::QueueUnloadUnusedChunks(void)
{
	QueueTask([](cWorld & a_World) { a_World.UnloadUnusedChunks(); });
}





void cWorld::CollectPickupsByEntity(cEntity & a_Entity)
{
	m_ChunkMap.CollectPickupsByEntity(a_Entity);
}





bool cWorld::ForEachPlayer(cPlayerListCallback a_Callback)
{
	// Calls the callback for each player in the list
	cLock Lock(*this);
	for (auto & Player : m_Players)
	{
		if (Player->IsTicking() && a_Callback(*Player))
		{
			return false;
		}
	}  // for itr - m_Players[]
	return true;
}





bool cWorld::DoWithPlayer(const AString & a_PlayerName, cPlayerListCallback a_Callback)
{
	// Calls the callback for the specified player in the list
	cLock Lock(*this);
	for (auto & Player : m_Players)
	{
		if (Player->IsTicking() && (NoCaseCompare(Player->GetName(), a_PlayerName) == 0))
		{
			a_Callback(*Player);
			return true;
		}
	}  // for itr - m_Players[]
	return false;
}





bool cWorld::FindAndDoWithPlayer(const AString & a_PlayerNameHint, cPlayerListCallback a_Callback)
{
	cPlayer * BestMatch = nullptr;
	size_t BestRating = 0;
	size_t NameLength = a_PlayerNameHint.length();

	cLock Lock(*this);
	for (const auto Player : m_Players)
	{
		if (!Player->IsTicking())
		{
			continue;
		}
		size_t Rating = RateCompareString (a_PlayerNameHint, Player->GetName());
		if (Rating >= BestRating)
		{
			BestMatch = Player;
			BestRating = Rating;
		}
		if (Rating == NameLength)  // Perfect match
		{
			break;
		}
	}  // for itr - m_Players[]

	if (BestMatch != nullptr)
	{
		return a_Callback(*BestMatch);
	}
	return false;
}





bool cWorld::DoWithPlayerByUUID(const cUUID & a_PlayerUUID, cPlayerListCallback a_Callback)
{
	cLock Lock(*this);
	for (auto & Player : m_Players)
	{
		if (Player->IsTicking() && (Player->GetUUID() == a_PlayerUUID))
		{
			return a_Callback(*Player);
		}
	}
	return false;
}





bool cWorld::DoWithNearestPlayer(Vector3d a_Pos, double a_RangeLimit, cPlayerListCallback a_Callback, bool a_CheckLineOfSight, bool a_IgnoreSpectator)
{
	double ClosestDistance = a_RangeLimit;
	cPlayer * ClosestPlayer = nullptr;

	cLock Lock(*this);
	for (const auto Player : m_Players)
	{
		if (!Player->IsTicking())
		{
			continue;
		}

		if (a_IgnoreSpectator && Player->IsGameModeSpectator())
		{
			continue;
		}

		Vector3f Pos = Player->GetPosition();
		double Distance = (Pos - a_Pos).Length();

		// If the player is too far, skip them:
		if (Distance > ClosestDistance)
		{
			continue;
		}

		// Check LineOfSight, if requested:
		if (
			a_CheckLineOfSight &&
			!cLineBlockTracer::LineOfSightTrace(*this, a_Pos, Pos, cLineBlockTracer::losAirWater)
		)
		{
			continue;
		}

		ClosestDistance = Distance;
		ClosestPlayer = Player;
	}

	if (ClosestPlayer)
	{
		return a_Callback(*ClosestPlayer);
	}
	else
	{
		return false;
	}
}





void cWorld::SendPlayerList(cPlayer * a_DestPlayer)
{
	// Sends the playerlist to a_DestPlayer
	cLock Lock(*this);
	for (const auto & Player : m_Players)
	{
		if (!Player->GetClientHandle()->IsDestroyed())
		{
			a_DestPlayer->GetClientHandle()->SendPlayerListAddPlayer(*Player);
		}
	}
}





bool cWorld::ForEachEntity(cEntityCallback a_Callback)
{
	return m_ChunkMap.ForEachEntity(a_Callback);
}





bool cWorld::ForEachEntityInChunk(int a_ChunkX, int a_ChunkZ, cEntityCallback a_Callback)
{
	return m_ChunkMap.ForEachEntityInChunk(a_ChunkX, a_ChunkZ, a_Callback);
}





bool cWorld::ForEachEntityInBox(const cBoundingBox & a_Box, cEntityCallback a_Callback)
{
	return m_ChunkMap.ForEachEntityInBox(a_Box, a_Callback);
}





size_t cWorld::GetPlayerCount() const
{
	cLock Lock(*this);
	return m_Players.size();
}





bool cWorld::DoWithEntityByID(UInt32 a_UniqueID, cEntityCallback a_Callback)
{
	// First check the entities-to-add:
	{
		cCSLock Lock(m_CSEntitiesToAdd);
		for (const auto & Item : m_EntitiesToAdd)
		{
			if (Item.first->GetUniqueID() == a_UniqueID)
			{
				a_Callback(*Item.first);
				return true;
			}
		}  // for ent - m_EntitiesToAdd[]
	}

	// Then check the chunkmap:
	return m_ChunkMap.DoWithEntityByID(a_UniqueID, a_Callback);
}





void cWorld::CompareChunkClients(int a_ChunkX1, int a_ChunkZ1, int a_ChunkX2, int a_ChunkZ2, cClientDiffCallback & a_Callback)
{
	m_ChunkMap.CompareChunkClients(a_ChunkX1, a_ChunkZ1, a_ChunkX2, a_ChunkZ2, a_Callback);
}





bool cWorld::AddChunkClient(int a_ChunkX, int a_ChunkZ, cClientHandle * a_Client)
{
	return m_ChunkMap.AddChunkClient(a_ChunkX, a_ChunkZ, a_Client);
}





void cWorld::RemoveChunkClient(int a_ChunkX, int a_ChunkZ, cClientHandle * a_Client)
{
	m_ChunkMap.RemoveChunkClient(a_ChunkX, a_ChunkZ, a_Client);
}





void cWorld::RemoveClientFromChunks(cClientHandle * a_Client)
{
	m_ChunkMap.RemoveClientFromChunks(a_Client);
}





void cWorld::SendChunkTo(int a_ChunkX, int a_ChunkZ, cChunkSender::Priority a_Priority, cClientHandle * a_Client)
{
	m_ChunkSender.QueueSendChunkTo(a_ChunkX, a_ChunkZ, a_Priority, a_Client);
}





void cWorld::ForceSendChunkTo(int a_ChunkX, int a_ChunkZ, cChunkSender::Priority a_Priority, cClientHandle * a_Client)
{
	a_Client->AddWantedChunk(a_ChunkX, a_ChunkZ);
	m_ChunkSender.QueueSendChunkTo(a_ChunkX, a_ChunkZ, a_Priority, a_Client);
}





void cWorld::PrepareChunk(int a_ChunkX, int a_ChunkZ, std::unique_ptr<cChunkCoordCallback> a_CallAfter)
{
	m_ChunkMap.PrepareChunk(a_ChunkX, a_ChunkZ, std::move(a_CallAfter));
}





void cWorld::ChunkLoadFailed(int a_ChunkX, int a_ChunkZ)
{
	m_ChunkMap.ChunkLoadFailed(a_ChunkX, a_ChunkZ);
}





bool cWorld::SetSignLines(Vector3i a_BlockPos, const AString & a_Line1, const AString & a_Line2, const AString & a_Line3, const AString & a_Line4, cPlayer * a_Player)
{
	// TODO: rvalue these strings

	AString Line1(a_Line1);
	AString Line2(a_Line2);
	AString Line3(a_Line3);
	AString Line4(a_Line4);

	if (cRoot::Get()->GetPluginManager()->CallHookUpdatingSign(*this, a_BlockPos, Line1, Line2, Line3, Line4, a_Player))
	{
		return false;
	}

	if (
		DoWithBlockEntityAt(a_BlockPos, [&Line1, &Line2, &Line3, &Line4](cBlockEntity & a_BlockEntity)
		{
			if (!cBlockWallSignHandler::IsBlockWallSign(a_BlockEntity.GetBlock()) && !cBlockSignPostHandler::IsBlockSignPost(a_BlockEntity.GetBlock()))
			{
				return false;  // Not a sign
			}

			static_cast<cSignEntity &>(a_BlockEntity).SetLines(Line1, Line2, Line3, Line4);
			return true;
		})
	)
	{
		cRoot::Get()->GetPluginManager()->CallHookUpdatedSign(*this, a_BlockPos, Line1, Line2, Line3, Line4, a_Player);
		return true;
	}

	return false;
}





bool cWorld::SetCommandBlockCommand(int a_BlockX, int a_BlockY, int a_BlockZ, const AString & a_Command)
{
	return DoWithBlockEntityAt({ a_BlockX, a_BlockY, a_BlockZ }, [&](cBlockEntity & a_BlockEntity)
	{
		if (cBlockCommandBlockHandler::IsBlockCommandBlock(a_BlockEntity.GetBlock()))
		{
			return false;
		}

		static_cast<cCommandBlockEntity &>(a_BlockEntity).SetCommand(a_Command);
		return true;
	});
}





bool cWorld::IsTrapdoorOpen(int a_BlockX, int a_BlockY, int a_BlockZ)
{
	BlockState Block;
	if (GetBlock({ a_BlockX, a_BlockY, a_BlockZ }, Block))
	{
		return false;
	}

	return cBlockTrapdoorHandler::IsTrapdoorOpen(GetBlock({ a_BlockX, a_BlockY, a_BlockZ }));
}





bool cWorld::SetTrapdoorOpen(int a_BlockX, int a_BlockY, int a_BlockZ, bool a_Open)
{
	BlockState Trapdoor;
	if (GetBlock({ a_BlockX, a_BlockY, a_BlockZ }, Trapdoor))
	{
		return false;
	}

	if (cBlockTrapdoorHandler::IsBlockTrapdoor(Trapdoor))
	{
		return false;
	}

	if (!cBlockTrapdoorHandler::IsTrapdoorOpen(Trapdoor))
	{
		FastSetBlock({ a_BlockX, a_BlockY, a_BlockZ }, cBlockTrapdoorHandler::ToggleTrapdoor(Trapdoor));
		BroadcastSoundParticleEffect(EffectID::SFX_RANDOM_WOODEN_TRAPDOOR_OPEN, { a_BlockX, a_BlockY, a_BlockZ }, 0);
		return true;
	}
	return false;
}





void cWorld::RegenerateChunk(int a_ChunkX, int a_ChunkZ)
{
	m_ChunkMap.MarkChunkRegenerating(a_ChunkX, a_ChunkZ);
	m_Generator.QueueGenerateChunk({a_ChunkX, a_ChunkZ}, true);
}





void cWorld::GenerateChunk(int a_ChunkX, int a_ChunkZ)
{
	m_ChunkMap.GenerateChunk(a_ChunkX, a_ChunkZ);
}





void cWorld::QueueLightChunk(int a_ChunkX, int a_ChunkZ, std::unique_ptr<cChunkCoordCallback> a_Callback)
{
	m_Lighting.QueueChunk(a_ChunkX, a_ChunkZ, std::move(a_Callback));
}





bool cWorld::IsChunkLighted(int a_ChunkX, int a_ChunkZ)
{
	return m_ChunkMap.IsChunkLighted(a_ChunkX, a_ChunkZ);
}





bool cWorld::ForEachChunkInRect(int a_MinChunkX, int a_MaxChunkX, int a_MinChunkZ, int a_MaxChunkZ, cChunkDataCallback & a_Callback)
{
	return m_ChunkMap.ForEachChunkInRect(a_MinChunkX, a_MaxChunkX, a_MinChunkZ, a_MaxChunkZ, a_Callback);
}





bool cWorld::ForEachLoadedChunk(cFunctionRef<bool(int, int)> a_Callback)
{
	return m_ChunkMap.ForEachLoadedChunk(a_Callback);
}





void cWorld::SaveAllChunks(void)
{
	if (IsSavingEnabled())
	{
		m_LastSave = m_WorldAge;
		m_ChunkMap.SaveAllChunks();
	}
}





void cWorld::QueueSaveAllChunks(void)
{
	QueueTask([](cWorld & a_World) { a_World.SaveAllChunks(); });
}





void cWorld::QueueTask(std::function<void(cWorld &)> a_Task)
{
	cCSLock Lock(m_CSTasks);
	m_Tasks.emplace_back(0, std::move(a_Task));
}





void cWorld::ScheduleTask(const cTickTime a_DelayTicks, std::function<void (cWorld &)> a_Task)
{
	const auto TargetTick = a_DelayTicks + m_WorldAge;

	// Insert the task into the list of scheduled tasks
	{
		cCSLock Lock(m_CSTasks);
		m_Tasks.emplace_back(TargetTick, std::move(a_Task));
	}
}





void cWorld::AddEntity(OwnedEntity a_Entity, cWorld * a_OldWorld)
{
	cCSLock Lock(m_CSEntitiesToAdd);
	m_EntitiesToAdd.emplace_back(std::move(a_Entity), a_OldWorld);
}





OwnedEntity cWorld::RemoveEntity(cEntity & a_Entity)
{
	// Remove players from the player list:
	if (a_Entity.IsPlayer())
	{
		cLock Lock(*this);
		const auto Player = static_cast<cPlayer *>(&a_Entity);
		LOGD("Removing player %s from world \"%s\"", Player->GetName().c_str(), m_WorldName.c_str());
		m_Players.erase(std::remove(m_Players.begin(), m_Players.end(), Player), m_Players.end());
	}

	// Check if the entity is in the chunkmap:
	auto Entity = m_ChunkMap.RemoveEntity(a_Entity);
	if (Entity != nullptr)
	{
		Entity->OnRemoveFromWorld(*this);
		return Entity;
	}

	// Check if the entity is in the queue to be added to the world:
	cCSLock Lock(m_CSEntitiesToAdd);
	auto itr = std::find_if(m_EntitiesToAdd.begin(), m_EntitiesToAdd.end(),
		[&a_Entity](const auto & Item)
		{
			return (Item.first.get() == &a_Entity);
		}
	);

	if (itr != m_EntitiesToAdd.end())
	{
		Entity = std::move(itr->first);
		m_EntitiesToAdd.erase(itr);
	}

	return Entity;
}





size_t cWorld::GetNumChunks(void) const
{
	return m_ChunkMap.GetNumChunks();
}





size_t cWorld::GetNumUnusedDirtyChunks(void) const
{
	return m_ChunkMap.GetNumUnusedDirtyChunks();
}





void cWorld::GetChunkStats(int & a_NumValid, int & a_NumDirty, int & a_NumInLightingQueue)
{
	m_ChunkMap.GetChunkStats(a_NumValid, a_NumDirty);
	a_NumInLightingQueue = static_cast<int>(m_Lighting.GetQueueLength());
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
			m_ChunkMap.TickBlock({Block->X, Block->Y, Block->Z});
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
		(GetBlock({ a_BlockX - 1, a_BlockY, a_BlockZ }).Type() == BlockType::Water) ||
		(GetBlock({ a_BlockX + 1, a_BlockY, a_BlockZ }).Type() == BlockType::Water) ||
		(GetBlock({ a_BlockX,     a_BlockY, a_BlockZ - 1 }).Type() == BlockType::Water) ||
		(GetBlock({ a_BlockX,     a_BlockY, a_BlockZ + 1 }).Type() == BlockType::Water)
	);
}





UInt32 cWorld::SpawnMob(double a_PosX, double a_PosY, double a_PosZ, eMonsterType a_MonsterType, bool a_Baby)
{
	auto Monster = cMonster::NewMonsterFromType(a_MonsterType);
	if (Monster == nullptr)
	{
		return cEntity::INVALID_ID;
	}
	Monster->SetPosition(a_PosX, a_PosY, a_PosZ);

	if (a_Baby)
	{
		Monster->SetAge(-1);
	}

	return SpawnMobFinalize(std::move(Monster));
}





UInt32 cWorld::SpawnMobFinalize(std::unique_ptr<cMonster> a_Monster)
{
	ASSERT(a_Monster != nullptr);

	// Give the mob  full health.
	a_Monster->SetHealth(a_Monster->GetMaxHealth());

	// A plugin doesn't agree with the spawn. bail out.
	if (cPluginManager::Get()->CallHookSpawningMonster(*this, *a_Monster))
	{
		return cEntity::INVALID_ID;
	}

	auto & Monster = *a_Monster;

	// Initialize the monster into the current world.
	if (!Monster.Initialize(std::move(a_Monster), *this))
	{
		return cEntity::INVALID_ID;
	}

	cPluginManager::Get()->CallHookSpawnedMonster(*this, Monster);

	return Monster.GetUniqueID();
}





UInt32 cWorld::CreateProjectile(Vector3d a_Pos, cProjectileEntity::eKind a_Kind, cEntity * a_Creator, const cItem * a_Item, const Vector3d * a_Speed)
{
	auto Projectile = cProjectileEntity::Create(a_Kind, a_Creator, a_Pos, a_Item, a_Speed);
	if (Projectile == nullptr)
	{
		return cEntity::INVALID_ID;
	}

	auto ProjectilePtr = Projectile.get();
	if (!ProjectilePtr->Initialize(std::move(Projectile), *this))
	{
		return cEntity::INVALID_ID;
	}

	return ProjectilePtr->GetUniqueID();
}





UInt32 cWorld::CreateProjectile(double a_PosX, double a_PosY, double a_PosZ, cProjectileEntity::eKind a_Kind, cEntity * a_Creator, const cItem * a_Item, const Vector3d * a_Speed)
{
	return CreateProjectile({a_PosX, a_PosY, a_PosZ}, a_Kind, a_Creator, a_Item, a_Speed);
}





int cWorld::GetTickRandomNumber(int a_Range)
{
	return GetRandomProvider().RandInt(a_Range);
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

	cLock Lock(*this);
	for (const auto Player : m_Players)
	{
		AString PlayerName = Player->HasCustomName() ? Player->GetCustomName() : Player->GetName();

		AString::size_type Found = StrToLower(PlayerName).find(StrToLower(LastWord));  // Try to find last word in playername
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
	m_ChunkMap.SetChunkAlwaysTicked(a_ChunkX, a_ChunkZ, a_AlwaysTicked);
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
	else
	{
		LOGWARNING("[Physics] Unknown RedstoneSimulator \"%s\" in %s, using the default of \"Incremental\".", SimulatorName.c_str(), GetIniFileName().c_str());
		res = new cIncrementalRedstoneSimulator(*this);
	}

	m_SimulatorManager->RegisterSimulator(res, 2 /* Two game ticks is a redstone tick */);

	return res;
}





cFluidSimulator * cWorld::InitializeFluidSimulator(cIniFile & a_IniFile, const char * a_FluidName, BlockType a_SimulateBlock)
{
	auto SimulatorNameKey = fmt::format(FMT_STRING("{}Simulator"), a_FluidName);
	auto SimulatorSectionName = fmt::format(FMT_STRING("{}Simulator"), a_FluidName);

	bool IsWater = (strcmp(a_FluidName, "Water") == 0);  // Used for defaults
	AString DefaultSimulatorName = ((GetDimension() == dimNether) && IsWater) ? "Vaporise" : "Vanilla";
	AString SimulatorName = a_IniFile.GetValueSet("Physics", SimulatorNameKey, DefaultSimulatorName);
	if (SimulatorName.empty())
	{
		LOGWARNING("[Physics] %s not present or empty in %s, using the default of \"%s\".", SimulatorNameKey, GetIniFileName(), DefaultSimulatorName);
		SimulatorName = DefaultSimulatorName;
	}
	cFluidSimulator * res = nullptr;
	int Rate = 1;
	if (
		(NoCaseCompare(SimulatorName, "vaporize") == 0) ||
		(NoCaseCompare(SimulatorName, "vaporise") == 0)
	)
	{
		res = new cVaporizeFluidSimulator(*this, a_SimulateBlock, 0);
	}
	else if (
		(NoCaseCompare(SimulatorName, "noop") == 0) ||
		(NoCaseCompare(SimulatorName, "nop") == 0) ||
		(NoCaseCompare(SimulatorName, "null") == 0) ||
		(NoCaseCompare(SimulatorName, "nil") == 0)
	)
	{
		res = new cNoopFluidSimulator(*this, a_SimulateBlock, 0);
	}
	else
	{
		int Falloff                          = a_IniFile.GetValueSetI(SimulatorSectionName, "Falloff",               IsWater ? 1 : 2);
		size_t TickDelay = static_cast<size_t>(a_IniFile.GetValueSetI(SimulatorSectionName, "TickDelay",             IsWater ? 5 : 30));
		int NumNeighborsForSource            = a_IniFile.GetValueSetI(SimulatorSectionName, "NumNeighborsForSource", IsWater ? 2 : -1);

		if ((Falloff > 15) || (Falloff < 0))
		{
			LOGWARNING("Falloff for %s simulator is out of range, assuming default of %d", a_FluidName, IsWater ? 1 : 2);
			Falloff = IsWater ? 1 : 2;
		}

		static constexpr unsigned char StationaryFalloffValue = 0;

		if (NoCaseCompare(SimulatorName, "floody") == 0)
		{
			res = new cFloodyFluidSimulator(*this, a_SimulateBlock, StationaryFalloffValue, static_cast<unsigned char>(Falloff), TickDelay, NumNeighborsForSource);
		}
		else if (NoCaseCompare(SimulatorName, "vanilla") == 0)
		{
			res = new cVanillaFluidSimulator(*this, a_SimulateBlock, StationaryFalloffValue, static_cast<unsigned char>(Falloff), TickDelay, NumNeighborsForSource);
		}
		else
		{
			// The simulator name doesn't match anything we have, issue a warning:
			LOGWARNING("%s [Physics]:%s specifies an unknown simulator, using the default \"Vanilla\".", GetIniFileName().c_str(), SimulatorNameKey.c_str());
			res = new cVanillaFluidSimulator(*this, a_SimulateBlock, StationaryFalloffValue, static_cast<unsigned char>(Falloff), TickDelay, NumNeighborsForSource);
		}
	}

	m_SimulatorManager->RegisterSimulator(res, Rate);

	return res;
}





////////////////////////////////////////////////////////////////////////////////
// cWorld::cChunkGeneratorCallbacks:

cWorld::cChunkGeneratorCallbacks::cChunkGeneratorCallbacks(cWorld & a_World) :
	m_World(&a_World)
{
}





void cWorld::cChunkGeneratorCallbacks::OnChunkGenerated(cChunkDesc & a_ChunkDesc)
{

	struct SetChunkData Data({ a_ChunkDesc.GetChunkX(), a_ChunkDesc.GetChunkZ()});
	{
		Data.BlockData.SetAll(a_ChunkDesc.GetBlocks());

		std::copy(a_ChunkDesc.GetBiomeMap().begin(),  a_ChunkDesc.GetBiomeMap().end(),  Data.BiomeMap.data());
		std::copy(a_ChunkDesc.GetHeightMap().begin(), a_ChunkDesc.GetHeightMap().end(), Data.HeightMap.data());

		Data.Entities = std::move(a_ChunkDesc.GetEntities());
		Data.BlockEntities = std::move(a_ChunkDesc.GetBlockEntities());

		Data.IsLightValid = false;
	}

	// LOG("Chunk gen disabled X %d Z %d", a_ChunkDesc.GetChunkX(),  a_ChunkDesc.GetChunkZ());
	m_World->QueueSetChunkData(std::move(Data));
}





bool cWorld::cChunkGeneratorCallbacks::IsChunkValid(cChunkCoords a_Coords)
{
	return m_World->IsChunkValid(a_Coords.m_ChunkX, a_Coords.m_ChunkZ);
}





bool cWorld::cChunkGeneratorCallbacks::IsChunkQueued(cChunkCoords a_Coords)
{
	return m_World->IsChunkQueued(a_Coords.m_ChunkX, a_Coords.m_ChunkZ);
}





bool cWorld::cChunkGeneratorCallbacks::HasChunkAnyClients(cChunkCoords a_Coords)
{
	return m_World->HasChunkAnyClients(a_Coords.m_ChunkX, a_Coords.m_ChunkZ);
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





bool cWorld::IsSlimeChunk(int a_ChunkX, int a_ChunkZ) const
{
	cNoise Noise(GetSeed());
	return (Noise.IntNoise2DInt(a_ChunkX, a_ChunkZ) / 8) % 10 == 0;  // 10% chance
}
