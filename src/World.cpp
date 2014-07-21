#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "BlockID.h"
#include "World.h"
#include "ChunkDef.h"
#include "ClientHandle.h"
#include "Server.h"
#include "Item.h"
#include "Root.h"
#include "inifile/iniFile.h"
#include "ChunkMap.h"
#include "Generating/ChunkDesc.h"
#include "OSSupport/Timer.h"

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

#include "MersenneTwister.h"
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
// cWorldLoadProgress:

/// A simple thread that displays the progress of world loading / saving in cWorld::InitializeSpawn()
class cWorldLoadProgress :
	public cIsThread
{
public:
	cWorldLoadProgress(cWorld * a_World) :
		cIsThread("cWorldLoadProgress"),
		m_World(a_World)
	{
		Start();
	}
	
	void Stop(void)
	{
		m_ShouldTerminate = true;
		Wait();
	}
	
protected:

	cWorld * m_World;
	
	virtual void Execute(void) override
	{
		for (;;)
		{
			LOG("" SIZE_T_FMT  " chunks to load, %d chunks to generate",
				m_World->GetStorage().GetLoadQueueLength(),
				m_World->GetGenerator().GetQueueLength()
			);
			
			// Wait for 2 sec, but be "reasonably wakeable" when the thread is to finish
			for (int i = 0; i < 20; i++)
			{
				cSleep::MilliSleep(100);
				if (m_ShouldTerminate)
				{
					return;
				}
			}
		}  // for (-ever)
	}
	
} ;





////////////////////////////////////////////////////////////////////////////////
// cWorldLightingProgress:

/// A simple thread that displays the progress of world lighting in cWorld::InitializeSpawn()
class cWorldLightingProgress :
	public cIsThread
{
public:
	cWorldLightingProgress(cLightingThread * a_Lighting) :
		cIsThread("cWorldLightingProgress"),
		m_Lighting(a_Lighting)
	{
		Start();
	}
	
	void Stop(void)
	{
		m_ShouldTerminate = true;
		Wait();
	}
	
protected:

	cLightingThread * m_Lighting;
	
	virtual void Execute(void) override
	{
		for (;;)
		{
			LOG("" SIZE_T_FMT  " chunks remaining to light", m_Lighting->GetQueueLength()
			);
			
			// Wait for 2 sec, but be "reasonably wakeable" when the thread is to finish
			for (int i = 0; i < 20; i++)
			{
				cSleep::MilliSleep(100);
				if (m_ShouldTerminate)
				{
					return;
				}
			}
		}  // for (-ever)
	}
	
} ;





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
	cTimer Timer;

	const Int64 msPerTick = 50;
	Int64 LastTime = Timer.GetNowTime();

	Int64 TickDuration = 50;
	while (!m_ShouldTerminate)
	{
		Int64 NowTime = Timer.GetNowTime();
		float DeltaTime = (float)(NowTime - LastTime);
		m_World.Tick(DeltaTime, (int)TickDuration);
		TickDuration = Timer.GetNowTime() - NowTime;
		
		if (TickDuration < msPerTick)
		{
			// Stretch tick time until it's at least msPerTick
			cSleep::MilliSleep((unsigned int)(msPerTick - TickDuration));
		}

		LastTime = NowTime;
	}
}





////////////////////////////////////////////////////////////////////////////////
// cWorld:

cWorld::cWorld(const AString & a_WorldName) :
	m_WorldName(a_WorldName),
	m_IniFileName(m_WorldName + "/world.ini"),
	m_StorageSchema("Default"),
#ifdef __arm__
	m_StorageCompressionFactor(0),
#else
	m_StorageCompressionFactor(6),
#endif
	m_IsSpawnExplicitlySet(false),
	m_WorldAgeSecs(0),
	m_TimeOfDaySecs(0),
	m_WorldAge(0),
	m_TimeOfDay(0),
	m_LastTimeUpdate(0),
	m_SkyDarkness(0),
	m_Weather(eWeather_Sunny),
	m_WeatherInterval(24000),  // Guaranteed 1 day of sunshine at server start :)
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
	delete m_SimulatorManager;   m_SimulatorManager  = NULL;
	delete m_SandSimulator;      m_SandSimulator     = NULL;
	delete m_WaterSimulator;     m_WaterSimulator    = NULL;
	delete m_LavaSimulator;      m_LavaSimulator     = NULL;
	delete m_FireSimulator;      m_FireSimulator     = NULL;
	delete m_RedstoneSimulator;  m_RedstoneSimulator = NULL;

	UnloadUnusedChunks();
	
	m_Storage.WaitForFinish();

	// Unload the scoreboard
	cScoreboardSerializer Serializer(m_WorldName, &m_Scoreboard);
	Serializer.Save();

	m_MapManager.SaveMapData();

	delete m_ChunkMap;
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
	
	// Let the plugins know about the change:
	cPluginManager::Get()->CallHookWeatherChanged(*this);
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
	
	LOG("Preparing spawn area in world \"%s\"...", m_WorldName.c_str());
	for (int x = 0; x < ViewDist; x++)
	{
		for (int z = 0; z < ViewDist; z++)
		{
			m_ChunkMap->TouchChunk(x + ChunkX-(ViewDist - 1) / 2, ZERO_CHUNK_Y, z + ChunkZ-(ViewDist - 1) / 2);  // Queue the chunk in the generator / loader
		}
	}
	
	{
		// Display progress during this process:
		cWorldLoadProgress Progress(this);
		
		// Wait for the loader to finish loading
		m_Storage.WaitForLoadQueueEmpty();
		
		// Wait for the generator to finish generating
		m_Generator.WaitForQueueEmpty();

		// Wait for the loader to finish saving
		m_Storage.WaitForSaveQueueEmpty();
		
		Progress.Stop();
	}
	
	// Light all chunks that have been newly generated:
	LOG("Lighting spawn area in world \"%s\"...", m_WorldName.c_str());
	
	for (int x = 0; x < ViewDist; x++)
	{
		int ChX = x + ChunkX-(ViewDist - 1) / 2;
		for (int z = 0; z < ViewDist; z++)
		{
			int ChZ = z + ChunkZ-(ViewDist - 1) / 2;
			if (!m_ChunkMap->IsChunkLighted(ChX, ChZ))
			{
				m_Lighting.QueueChunk(ChX, ChZ);  // Queue the chunk in the lighting thread
			}
		}  // for z
	}  // for x
	
	{
		cWorldLightingProgress Progress(&m_Lighting);
		m_Lighting.WaitForQueueEmpty();
		Progress.Stop();
	}
	
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
	}
	AString Dimension = IniFile.GetValueSet("General", "Dimension", "Overworld");
	m_Dimension = StringToDimension(Dimension);

	// Try to find the "SpawnPosition" key and coord values in the world configuration, set the flag if found
	int KeyNum = IniFile.FindKey("SpawnPosition");
	m_IsSpawnExplicitlySet =
	(
		(KeyNum >= 0) &&
		(
			(IniFile.FindValue(KeyNum, "X") >= 0) ||
			(IniFile.FindValue(KeyNum, "Y") >= 0) ||
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
	int GameMode                  = IniFile.GetValueSetI("General",       "Gamemode",                    (int)m_GameMode);
	
	// Adjust the enum-backed variables into their respective bounds:
	m_GameMode         = (eGameMode)     Clamp(GameMode,         (int)gmSurvival, (int)gmAdventure);
	m_TNTShrapnelLevel = (eShrapnelLevel)Clamp(TNTShrapnelLevel, (int)slNone,     (int)slAll);

	// Load allowed mobs:
	AString DefaultMonsters;
	switch (m_Dimension)
	{
		case dimOverworld: DefaultMonsters = "bat, cavespider, chicken, cow, creeper, enderman, horse, mooshroom, ocelot, pig, sheep, silverfish, skeleton, slime, spider, squid, wolf, zombie"; break;
		case dimNether:    DefaultMonsters = "blaze, ghast, magmacube, skeleton, zombie, zombiepigman"; break;
		case dimEnd:       DefaultMonsters = "enderman"; break;
		case dimNotSet:    break;
	}
	m_bAnimals = IniFile.GetValueSetB("Monsters", "AnimalsOn", true);
	AString AllMonsters = IniFile.GetValueSet("Monsters", "Types", DefaultMonsters);
	AStringVector SplitList = StringSplitAndTrim(AllMonsters, ",");
	for (AStringVector::const_iterator itr = SplitList.begin(), end = SplitList.end(); itr != end; ++itr)
	{
		cMonster::eType ToAdd = cMonster::StringToMobType(*itr);
		if (ToAdd != cMonster::mtInvalidType)
		{
			m_AllowedMobs.insert(ToAdd);
			LOGD("Allowed mob: %s", itr->c_str());
		}
		else
		{
			LOG("World \"%s\": Unknown mob type: %s", m_WorldName.c_str(), itr->c_str());
		}
	}

	m_ChunkMap = new cChunkMap(this);
	
	m_LastSave = 0;
	m_LastUnload = 0;

	// preallocate some memory for ticking blocks so we don't need to allocate that often
	m_BlockTickQueue.reserve(1000);
	m_BlockTickQueueCopy.reserve(1000);

	// Simulators:
	m_SimulatorManager  = new cSimulatorManager(*this);
	m_WaterSimulator    = InitializeFluidSimulator(IniFile, "Water", E_BLOCK_WATER, E_BLOCK_STATIONARY_WATER);
	m_LavaSimulator     = InitializeFluidSimulator(IniFile, "Lava",  E_BLOCK_LAVA,  E_BLOCK_STATIONARY_LAVA);
	m_SandSimulator     = new cSandSimulator(*this, IniFile);
	m_FireSimulator     = new cFireSimulator(*this, IniFile);
	m_RedstoneSimulator = InitializeRedstoneSimulator(IniFile);

	// Water, Lava and Redstone simulators get registered in their initialize function.
	m_SimulatorManager->RegisterSimulator(m_SandSimulator, 1);
	m_SimulatorManager->RegisterSimulator(m_FireSimulator, 1);

	m_Lighting.Start(this);
	m_Storage.Start(this, m_StorageSchema, m_StorageCompressionFactor);
	m_Generator.Start(m_GeneratorCallbacks, m_GeneratorCallbacks, IniFile);
	m_ChunkSender.Start(this);
	m_TickThread.Start();

	// Init of the spawn monster time (as they are supposed to have different spawn rate)
	m_LastSpawnMonster.insert(std::map<cMonster::eFamily, Int64>::value_type(cMonster::mfHostile, 0));
	m_LastSpawnMonster.insert(std::map<cMonster::eFamily, Int64>::value_type(cMonster::mfPassive, 0));
	m_LastSpawnMonster.insert(std::map<cMonster::eFamily, Int64>::value_type(cMonster::mfAmbient, 0));
	m_LastSpawnMonster.insert(std::map<cMonster::eFamily, Int64>::value_type(cMonster::mfWater, 0));

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

	while (IsBlockWaterOrIce(GetBlock((int)m_SpawnX, GetHeight((int)m_SpawnX, (int)m_SpawnZ), (int)m_SpawnZ)))
	{
		if ((GetTickRandomNumber(4) % 2) == 0)  // Randomise whether to increment X or Z coords
		{
			m_SpawnX += cChunkDef::Width;
		}
		else
		{
			m_SpawnZ += cChunkDef::Width;
		}
	}

	m_SpawnY = (double)GetHeight((int)m_SpawnX, (int)m_SpawnZ) + 1.6f;  // 1.6f to accomodate player height

	LOGD("Generated random spawnpoint %i %i %i", (int)m_SpawnX, (int)m_SpawnY, (int)m_SpawnZ);
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





void cWorld::Stop(void)
{
	// Delete the clients that have been in this world:
	{
		cCSLock Lock(m_CSClients);
		for (cClientHandleList::iterator itr = m_Clients.begin(); itr != m_Clients.end(); ++itr)
		{
			(*itr)->Destroy();
			delete *itr;
		}  // for itr - m_Clients[]
		m_Clients.clear();
	}
	
	m_TickThread.Stop();
	m_Lighting.Stop();
	m_Generator.Stop();
	m_ChunkSender.Stop();
	m_Storage.Stop();
}





void cWorld::Tick(float a_Dt, int a_LastTickDurationMSec)
{
	// Call the plugins
	cPluginManager::Get()->CallHookWorldTick(*this, a_Dt, a_LastTickDurationMSec);
	
	// We need sub-tick precision here, that's why we store the time in seconds and calculate ticks off of it
	m_WorldAgeSecs  += (double)a_Dt / 1000.0;
	m_TimeOfDaySecs += (double)a_Dt / 1000.0;

	// Wrap time of day each 20 minutes (1200 seconds)
	if (m_TimeOfDaySecs > 1200.0)
	{
		m_TimeOfDaySecs -= 1200.0;
	}

	m_WorldAge  = (Int64)(m_WorldAgeSecs  * 20.0);
	m_TimeOfDay = (Int64)(m_TimeOfDaySecs * 20.0);

	// Updates the sky darkness based on current time of day
	UpdateSkyDarkness();

	// Broadcast time update every 40 ticks (2 seconds)
	if (m_LastTimeUpdate < m_WorldAge - 40)
	{
		BroadcastTimeUpdate();
		m_LastTimeUpdate = m_WorldAge;
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

	TickClients(a_Dt);
	TickQueuedBlocks();
	TickQueuedTasks();
	TickScheduledTasks();
	
	GetSimulatorManager()->Simulate(a_Dt);

	TickWeather(a_Dt);

	m_ChunkMap->FastSetQueuedBlocks();

	if (m_WorldAge - m_LastSave > 60 * 5 * 20)  // Save each 5 minutes
	{
		SaveAllChunks();
	}

	if (m_WorldAge - m_LastUnload > 10 * 20)  // Unload every 10 seconds
	{
		UnloadUnusedChunks();
	}

	TickMobs(a_Dt);
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

	if (m_Weather == eWeather_ThunderStorm)
	{
		// 0.5% chance per tick of thunderbolt
		if (m_TickRand.randInt() % 199 == 0)
		{
			CastThunderbolt(0, 0, 0);  // TODO: find random possitions near players to cast thunderbolts.
		}
	}
}





void cWorld::TickMobs(float a_Dt)
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
			int SpawnDelay = cMonster::GetSpawnDelay(Family);
			if (
				(m_LastSpawnMonster[Family] > m_WorldAge - SpawnDelay) ||  // Not reached the needed ticks before the next round
				MobCensus.IsCapped(Family)
			)
			{
				continue;
			}
			m_LastSpawnMonster[Family] = m_WorldAge;
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
		delete *itr;
	}  // for itr - m_Tasks[]
}





void cWorld::TickScheduledTasks(void)
{
	// Make a copy of the tasks to avoid deadlocks on accessing m_Tasks
	cScheduledTasks Tasks;
	{
		cCSLock Lock(m_CSScheduledTasks);
		while (!m_ScheduledTasks.empty() && (m_ScheduledTasks.front()->m_TargetTick < m_WorldAge))
		{
			Tasks.push_back(m_ScheduledTasks.front());
			m_ScheduledTasks.pop_front();
		}
	}

	// Execute and delete each task:
	for (cScheduledTasks::iterator itr = Tasks.begin(), end = Tasks.end(); itr != end; ++itr)
	{
		(*itr)->m_Task->Run(*this);
		delete *itr;
	}  // for itr - m_Tasks[]
}




void cWorld::TickClients(float a_Dt)
{
	cClientHandleList RemoveClients;
	{
		cCSLock Lock(m_CSClients);
		
		// Remove clients scheduled for removal:
		for (cClientHandleList::iterator itr = m_ClientsToRemove.begin(), end = m_ClientsToRemove.end(); itr != end; ++itr)
		{
			m_Clients.remove(*itr);
		}  // for itr - m_ClientsToRemove[]
		m_ClientsToRemove.clear();
		
		// Add clients scheduled for adding:
		for (cClientHandleList::iterator itr = m_ClientsToAdd.begin(), end = m_ClientsToAdd.end(); itr != end; ++itr)
		{
			if (std::find(m_Clients.begin(), m_Clients.end(), *itr) != m_Clients.end())
			{
				ASSERT(!"Adding a client that is already in the clientlist");
				continue;
			}
			m_Clients.push_back(*itr);
		}  // for itr - m_ClientsToRemove[]
		m_ClientsToAdd.clear();
		
		// Tick the clients, take out those that have been destroyed into RemoveClients
		for (cClientHandleList::iterator itr = m_Clients.begin(); itr != m_Clients.end();)
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
	
	// Delete the clients that have been destroyed
	for (cClientHandleList::iterator itr = RemoveClients.begin(); itr != RemoveClients.end(); ++itr)
	{
		delete *itr;
	}  // for itr - RemoveClients[]
}





void cWorld::UpdateSkyDarkness(void)
{
	int TempTime = (int)m_TimeOfDay;
	if (TempTime <= TIME_SUNSET)
	{
		m_SkyDarkness = 0;
	}
	else if (TempTime <= TIME_NIGHT_START)
	{
		m_SkyDarkness = (TIME_NIGHT_START - TempTime) / TIME_SPAWN_DIVISOR;
	}
	else if (TempTime <= TIME_NIGHT_END)
	{
		m_SkyDarkness = 8;
	}
	else
	{
		m_SkyDarkness = (TIME_SUNRISE - TempTime) / TIME_SPAWN_DIVISOR;
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
	
	// TODO: Add damage to entities and implement block hardiness
	Vector3d explosion_pos = Vector3d(a_BlockX, a_BlockY, a_BlockZ);
	cVector3iArray BlocksAffected;
	m_ChunkMap->DoExplosionAt(a_ExplosionSize, a_BlockX, a_BlockY, a_BlockZ, BlocksAffected);
	BroadcastSoundEffect("random.explode", (double)a_BlockX, (double)a_BlockY, (double)a_BlockZ, 1.0f, 0.6f);
	{
		cCSLock Lock(m_CSPlayers);
		for (cPlayerList::iterator itr = m_Players.begin(); itr != m_Players.end(); ++itr)
		{
			cClientHandle * ch = (*itr)->GetClientHandle();
			if ((ch == NULL) || !ch->IsLoggedIn() || ch->IsDestroyed())
			{
				continue;
			}
			Vector3d distance_explosion = (*itr)->GetPosition() - explosion_pos;
			if (distance_explosion.SqrLength() < 4096.0)
			{
				double real_distance = std::max(0.004, sqrt(distance_explosion.SqrLength()));
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
	switch (a_SaplingMeta & 0x07)
	{
		case E_META_SAPLING_APPLE:    GetAppleTreeImage  (a_X, a_Y, a_Z, Noise, (int)(m_WorldAge & 0xffffffff), Logs, Other); break;
		case E_META_SAPLING_BIRCH:    GetBirchTreeImage  (a_X, a_Y, a_Z, Noise, (int)(m_WorldAge & 0xffffffff), Logs, Other); break;
		case E_META_SAPLING_CONIFER:  GetConiferTreeImage(a_X, a_Y, a_Z, Noise, (int)(m_WorldAge & 0xffffffff), Logs, Other); break;
		case E_META_SAPLING_JUNGLE:   GetJungleTreeImage (a_X, a_Y, a_Z, Noise, (int)(m_WorldAge & 0xffffffff), Logs, Other); break;
		case E_META_SAPLING_ACACIA:   GetAcaciaTreeImage (a_X, a_Y, a_Z, Noise, (int)(m_WorldAge & 0xffffffff), Logs, Other); break;
		case E_META_SAPLING_DARK_OAK: GetDarkoakTreeImage(a_X, a_Y, a_Z, Noise, (int)(m_WorldAge & 0xffffffff), Logs, Other); break;
	}
	Other.insert(Other.begin(), Logs.begin(), Logs.end());
	Logs.clear();
	GrowTreeImage(Other);
}





void cWorld::GrowTreeByBiome(int a_X, int a_Y, int a_Z)
{
	cNoise Noise(m_Generator.GetSeed());
	sSetBlockVector Logs, Other;
	GetTreeImageByBiome(a_X, a_Y, a_Z, Noise, (int)(m_WorldAge & 0xffffffff), GetBiomeAt(a_X, a_Z), Logs, Other);
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
		if (itr->BlockType == E_BLOCK_LOG)
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
		switch (itr->BlockType)
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
				int OfsX = (r1.randInt(3) + r1.randInt(3) + r1.randInt(3) + r1.randInt(3)) / 2 - 3;
				int OfsY = r1.randInt(3) + r1.randInt(3) - 3;
				int OfsZ = (r1.randInt(3) + r1.randInt(3) + r1.randInt(3) + r1.randInt(3)) / 2 - 3;
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
	m_ChunkMap->SetBlock(*this, a_BlockX, a_BlockY, a_BlockZ, a_BlockType, a_BlockMeta, a_SendToClients);
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
		if (!IsValidItem(itr->m_ItemType) || itr->m_ItemType == E_BLOCK_AIR)
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
		if (!IsValidItem(itr->m_ItemType) || itr->m_ItemType == E_BLOCK_AIR)
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
		if ((ch == a_Exclude) || (ch == NULL) || !ch->IsLoggedIn() || ch->IsDestroyed())
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
		if ((ch == a_Exclude) || (ch == NULL) || !ch->IsLoggedIn() || ch->IsDestroyed())
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





void cWorld::BroadcastParticleEffect(const AString & a_ParticleName, float a_SrcX, float a_SrcY, float a_SrcZ, float a_OffsetX, float a_OffsetY, float a_OffsetZ, float a_ParticleData, int a_ParticleAmmount, cClientHandle * a_Exclude)
{
	m_ChunkMap->BroadcastParticleEffect(a_ParticleName, a_SrcX, a_SrcY, a_SrcZ, a_OffsetX, a_OffsetY, a_OffsetZ, a_ParticleData, a_ParticleAmmount, a_Exclude);
}





void cWorld::BroadcastPlayerListItem (const cPlayer & a_Player, bool a_IsOnline, const cClientHandle * a_Exclude)
{
	cCSLock Lock(m_CSPlayers);
	for (cPlayerList::iterator itr = m_Players.begin(); itr != m_Players.end(); ++itr)
	{
		cClientHandle * ch = (*itr)->GetClientHandle();
		if ((ch == a_Exclude) || (ch == NULL) || !ch->IsLoggedIn() || ch->IsDestroyed())
		{
			continue;
		}
		ch->SendPlayerListItem(a_Player, a_IsOnline);
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
		if ((ch == NULL) || !ch->IsLoggedIn() || ch->IsDestroyed())
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
		if ((ch == NULL) || !ch->IsLoggedIn() || ch->IsDestroyed())
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
		if ((ch == NULL) || !ch->IsLoggedIn() || ch->IsDestroyed())
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
		if ((ch == a_Exclude) || (ch == NULL) || !ch->IsLoggedIn() || ch->IsDestroyed())
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
		if ((ch == a_Exclude) || (ch == NULL) || !ch->IsLoggedIn() || ch->IsDestroyed())
		{
			continue;
		}
		ch->SendTimeUpdate(m_WorldAge, m_TimeOfDay);
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
		if ((ch == a_Exclude) || (ch == NULL) || !ch->IsLoggedIn() || ch->IsDestroyed())
		{
			continue;
		}
		ch->SendWeather(a_Weather);
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





void cWorld::SetChunkData(
	int a_ChunkX, int a_ChunkZ,
	const BLOCKTYPE * a_BlockTypes,
	const NIBBLETYPE * a_BlockMeta,
	const NIBBLETYPE * a_BlockLight,
	const NIBBLETYPE * a_BlockSkyLight,
	const cChunkDef::HeightMap * a_HeightMap,
	const cChunkDef::BiomeMap  * a_BiomeMap,
	cEntityList & a_Entities,
	cBlockEntityList & a_BlockEntities,
	bool a_MarkDirty
)
{
	// Validate biomes, if needed:
	cChunkDef::BiomeMap BiomeMap;
	const cChunkDef::BiomeMap * Biomes = a_BiomeMap;
	if (a_BiomeMap == NULL)
	{
		// The biomes are not assigned, get them from the generator:
		Biomes = &BiomeMap;
		m_Generator.GenerateBiomes(a_ChunkX, a_ChunkZ, BiomeMap);
	}
	
	m_ChunkMap->SetChunkData(
		a_ChunkX, a_ChunkZ,
		a_BlockTypes, a_BlockMeta, a_BlockLight, a_BlockSkyLight,
		a_HeightMap, *Biomes,
		a_BlockEntities,
		a_MarkDirty
	);
	
	// Initialize the entities (outside the m_ChunkMap's CS, to fix FS #347):
	for (cEntityList::iterator itr = a_Entities.begin(), end = a_Entities.end(); itr != end; ++itr)
	{
		(*itr)->Initialize(*this);
	}
	
	// If a client is requesting this chunk, send it to them:
	if (m_ChunkMap->HasChunkAnyClients(a_ChunkX, a_ChunkZ))
	{
		m_ChunkSender.ChunkReady(a_ChunkX, a_ChunkZ);
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
	m_LastUnload = m_WorldAge;
	m_ChunkMap->UnloadUnusedChunks();
}





void cWorld::QueueUnloadUnusedChunks(void)
{
	QueueTask(new cWorld::cTaskUnloadUnusedChunks);
}





void cWorld::CollectPickupsByPlayer(cPlayer * a_Player)
{
	m_ChunkMap->CollectPickupsByPlayer(a_Player);
}





void cWorld::AddPlayer(cPlayer * a_Player)
{
	cCSLock Lock(m_CSPlayersToAdd);
	m_PlayersToAdd.push_back(a_Player);
}





void cWorld::RemovePlayer(cPlayer * a_Player)
{

	m_ChunkMap->RemoveEntity(a_Player);
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
	if (Client != NULL)
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
	// Calls the callback for each player in the list
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
	cPlayer * BestMatch = NULL;
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

	if (BestMatch != NULL)
	{
		return a_Callback.Item (BestMatch);
	}
	return false;
}





// TODO: This interface is dangerous!
cPlayer * cWorld::FindClosestPlayer(const Vector3d & a_Pos, float a_SightLimit, bool a_CheckLineOfSight)
{
	cTracer LineOfSight(this);

	double ClosestDistance = a_SightLimit;
	cPlayer * ClosestPlayer = NULL;

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
		if ((ch != NULL) && !ch->IsDestroyed())
		{
			a_DestPlayer->GetClientHandle()->SendPlayerListItem(*(*itr), true);
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





void cWorld::SendChunkTo(int a_ChunkX, int a_ChunkZ, cClientHandle * a_Client)
{
	m_ChunkSender.QueueSendChunkTo(a_ChunkX, a_ChunkZ, a_Client);
}





void cWorld::ForceSendChunkTo(int a_ChunkX, int a_ChunkZ, cClientHandle * a_Client)
{
	a_Client->AddWantedChunk(a_ChunkX, a_ChunkZ);
	m_ChunkSender.QueueSendChunkTo(a_ChunkX, a_ChunkZ, a_Client);
}





void cWorld::RemoveClientFromChunkSender(cClientHandle * a_Client)
{
	m_ChunkSender.RemoveClient(a_Client);
}





void cWorld::TouchChunk(int a_ChunkX, int a_ChunkY, int a_ChunkZ)
{
	m_ChunkMap->TouchChunk(a_ChunkX, a_ChunkY, a_ChunkZ);
}





bool cWorld::LoadChunk(int a_ChunkX, int a_ChunkY, int a_ChunkZ)
{
	return m_ChunkMap->LoadChunk(a_ChunkX, a_ChunkY, a_ChunkZ);
}



	

void cWorld::LoadChunks(const cChunkCoordsList & a_Chunks)
{
	m_ChunkMap->LoadChunks(a_Chunks);
}





void cWorld::ChunkLoadFailed(int a_ChunkX, int a_ChunkY, int a_ChunkZ)
{
	m_ChunkMap->ChunkLoadFailed(a_ChunkX, a_ChunkY, a_ChunkZ);
}





bool cWorld::SetSignLines(int a_BlockX, int a_BlockY, int a_BlockZ, const AString & a_Line1, const AString & a_Line2, const AString & a_Line3, const AString & a_Line4, cPlayer * a_Player)
{
	AString Line1(a_Line1);
	AString Line2(a_Line2);
	AString Line3(a_Line3);
	AString Line4(a_Line4);
	if (cRoot::Get()->GetPluginManager()->CallHookUpdatingSign(this, a_BlockX, a_BlockY, a_BlockZ, Line1, Line2, Line3, Line4, a_Player))
	{
		return false;
	}
	if (m_ChunkMap->SetSignLines(a_BlockX, a_BlockY, a_BlockZ, Line1, Line2, Line3, Line4))
	{
		cRoot::Get()->GetPluginManager()->CallHookUpdatedSign(this, a_BlockX, a_BlockY, a_BlockZ, Line1, Line2, Line3, Line4, a_Player);
		return true;
	}
	return false;
}





bool cWorld::UpdateSign(int a_BlockX, int a_BlockY, int a_BlockZ, const AString & a_Line1, const AString & a_Line2, const AString & a_Line3, const AString & a_Line4, cPlayer * a_Player)
{
	return SetSignLines(a_BlockX, a_BlockY, a_BlockZ, a_Line1, a_Line2, a_Line3, a_Line4, a_Player);
}





bool cWorld::SetCommandBlockCommand(int a_BlockX, int a_BlockY, int a_BlockZ, const AString & a_Command)
{
	class cUpdateCommandBlock : public cCommandBlockCallback
	{
		AString m_Command;
	public:
		cUpdateCommandBlock(const AString & a_Command) : m_Command(a_Command) {}
			
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
	if (Block != E_BLOCK_TRAPDOOR)
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
	if (Block != E_BLOCK_TRAPDOOR)
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
	
	// Trick: use Y=1 to force the chunk generation even though the chunk data is already present
	m_Generator.QueueGenerateChunk(a_ChunkX, 1, a_ChunkZ);
}





void cWorld::GenerateChunk(int a_ChunkX, int a_ChunkZ)
{
	m_Generator.QueueGenerateChunk(a_ChunkX, ZERO_CHUNK_Y, a_ChunkZ);
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
	m_LastSave = m_WorldAge;
	m_ChunkMap->SaveAllChunks();
}





void cWorld::QueueSaveAllChunks(void)
{
	QueueTask(new cWorld::cTaskSaveAllChunks);
}





void cWorld::QueueTask(cTask * a_Task)
{
	cCSLock Lock(m_CSTasks);
	m_Tasks.push_back(a_Task);
}





void cWorld::ScheduleTask(int a_DelayTicks, cTask * a_Task)
{
	Int64 TargetTick = a_DelayTicks + m_WorldAge;
	
	// Insert the task into the list of scheduled tasks, ordered by its target tick
	cCSLock Lock(m_CSScheduledTasks);
	for (cScheduledTasks::iterator itr = m_ScheduledTasks.begin(), end = m_ScheduledTasks.end(); itr != end; ++itr)
	{
		if ((*itr)->m_TargetTick >= TargetTick)
		{
			m_ScheduledTasks.insert(itr, new cScheduledTask(TargetTick, a_Task));
			return;
		}
	}
	m_ScheduledTasks.push_back(new cScheduledTask(TargetTick, a_Task));
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
	return m_ChunkMap->HasEntity(a_UniqueID);
}





void cWorld::RemoveEntity(cEntity * a_Entity)
{
	m_ChunkMap->RemoveEntity(a_Entity);
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





int cWorld::SpawnMob(double a_PosX, double a_PosY, double a_PosZ, cMonster::eType a_MonsterType)
{
	cMonster * Monster = NULL;

	Monster = cMonster::NewMonsterFromType(a_MonsterType);
	if (Monster != NULL)
	{
		Monster->SetPosition(a_PosX, a_PosY, a_PosZ);
	}
	
	return SpawnMobFinalize(Monster);
}




int cWorld::SpawnMobFinalize(cMonster * a_Monster)
{
	if (!a_Monster)
		return -1;
	a_Monster->SetHealth(a_Monster->GetMaxHealth());
	if (cPluginManager::Get()->CallHookSpawningMonster(*this, *a_Monster))
	{
		delete a_Monster;
		a_Monster = NULL;
		return -1;
	}
	if (!a_Monster->Initialize(*this))
	{
		delete a_Monster;
		a_Monster = NULL;
		return -1;
	}
	BroadcastSpawnEntity(*a_Monster);
	cPluginManager::Get()->CallHookSpawnedMonster(*this, *a_Monster);

	return a_Monster->GetUniqueID();
}





int cWorld::CreateProjectile(double a_PosX, double a_PosY, double a_PosZ, cProjectileEntity::eKind a_Kind, cEntity * a_Creator, const cItem * a_Item, const Vector3d * a_Speed)
{
	cProjectileEntity * Projectile = cProjectileEntity::Create(a_Kind, a_Creator, a_PosX, a_PosY, a_PosZ, a_Item, a_Speed);
	if (Projectile == NULL)
	{
		return -1;
	}
	if (!Projectile->Initialize(*this))
	{
		delete Projectile;
		Projectile = NULL;
		return -1;
	}
	return Projectile->GetUniqueID();
}





void cWorld::TabCompleteUserName(const AString & a_Text, AStringVector & a_Results)
{
	cCSLock Lock(m_CSPlayers);
	for (cPlayerList::iterator itr = m_Players.begin(), end = m_Players.end(); itr != end; ++itr)
	{
		size_t LastSpace = a_Text.find_last_of(" ");  // Find the position of the last space
		
		AString LastWord = a_Text.substr(LastSpace + 1, a_Text.length());  // Find the last word
		AString PlayerName ((*itr)->GetName());
		size_t Found = PlayerName.find(LastWord);  // Try to find last word in playername
		
		if (Found == AString::npos)
		{
			continue;  // No match
		}
		
		a_Results.push_back(PlayerName);  // Match!
	}
}





void cWorld::SetChunkAlwaysTicked(int a_ChunkX, int a_ChunkZ, bool a_AlwaysTicked)
{
	m_ChunkMap->SetChunkAlwaysTicked(a_ChunkX, a_ChunkZ, a_AlwaysTicked);
}





cRedstoneSimulator * cWorld::InitializeRedstoneSimulator(cIniFile & a_IniFile)
{
	AString SimulatorName = a_IniFile.GetValueSet("Physics", "RedstoneSimulator", "");

	if (SimulatorName.empty())
	{
		LOGWARNING("[Physics] RedstoneSimulator not present or empty in %s, using the default of \"incremental\".", GetIniFileName().c_str());
		SimulatorName = "incremental";
	}
	
	cRedstoneSimulator * res = NULL;

	if (NoCaseCompare(SimulatorName, "incremental") == 0)
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
	AString SimulatorName = a_IniFile.GetValueSet("Physics", SimulatorNameKey, "");
	if (SimulatorName.empty())
	{
		LOGWARNING("[Physics] %s not present or empty in %s, using the default of \"Vanilla\".", SimulatorNameKey.c_str(), GetIniFileName().c_str());
		SimulatorName = "Vanilla";
	}
	
	cFluidSimulator * res = NULL;
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

		if (NoCaseCompare(SimulatorName, "floody") == 0)
		{
			res = new cFloodyFluidSimulator(*this, a_SimulateBlock, a_StationaryBlock, Falloff, TickDelay, NumNeighborsForSource);
		}
		else if (NoCaseCompare(SimulatorName, "vanilla") == 0)
		{
			res = new cVanillaFluidSimulator(*this, a_SimulateBlock, a_StationaryBlock, Falloff, TickDelay, NumNeighborsForSource);
		}
		else
		{
			// The simulator name doesn't match anything we have, issue a warning:
			LOGWARNING("%s [Physics]:%s specifies an unknown simulator, using the default \"Vanilla\".", GetIniFileName().c_str(), SimulatorNameKey.c_str());
			res = new cVanillaFluidSimulator(*this, a_SimulateBlock, a_StationaryBlock, Falloff, TickDelay, NumNeighborsForSource);
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
			cClientHandle * Client = (*itr)->GetClientHandle();
			if (Client != NULL)
			{
				m_Clients.push_back(Client);
			}
		}  // for itr - PlayersToAdd[]
	}  // Lock(m_CSClients)

	// Stream chunks to all eligible clients:
	for (cPlayerList::iterator itr = PlayersToAdd.begin(), end = PlayersToAdd.end(); itr != end; ++itr)
	{
		cClientHandle * Client = (*itr)->GetClientHandle();
		if (Client != NULL)
		{
			Client->StreamChunks();
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
		cPlayerCallback(std::vector<Vector3i> & a_SendQueue, cWorld & a_World) :
			m_SendQueue(a_SendQueue),
			m_World(a_World)
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

	m_World->SetChunkData(
		a_ChunkDesc.GetChunkX(), a_ChunkDesc.GetChunkZ(),
		a_ChunkDesc.GetBlockTypes(), BlockMetas,
		NULL, NULL,  // We don't have lighting, chunk will be lighted when needed
		&a_ChunkDesc.GetHeightMap(), &a_ChunkDesc.GetBiomeMap(),
		a_ChunkDesc.GetEntities(), a_ChunkDesc.GetBlockEntities(),
		true
	);

	// Save the chunk right after generating, so that we don't have to generate it again on next run
	m_World->GetStorage().QueueSaveChunk(a_ChunkDesc.GetChunkX(), 0, a_ChunkDesc.GetChunkZ());
}





bool cWorld::cChunkGeneratorCallbacks::IsChunkValid(int a_ChunkX, int a_ChunkZ)
{
	return m_World->IsChunkValid(a_ChunkX, a_ChunkZ);
}





bool cWorld::cChunkGeneratorCallbacks::HasChunkAnyClients(int a_ChunkX, int a_ChunkZ)
{
	return m_World->HasChunkAnyClients(a_ChunkX, a_ChunkZ);
}





void cWorld::cChunkGeneratorCallbacks::CallHookChunkGenerating(cChunkDesc & a_ChunkDesc)
{
	cPluginManager::Get()->CallHookChunkGenerating(
		m_World, a_ChunkDesc.GetChunkX(), a_ChunkDesc.GetChunkZ(), &a_ChunkDesc
	);
}





void cWorld::cChunkGeneratorCallbacks::CallHookChunkGenerated (cChunkDesc & a_ChunkDesc)
{
	cPluginManager::Get()->CallHookChunkGenerated(
		m_World, a_ChunkDesc.GetChunkX(), a_ChunkDesc.GetChunkZ(), &a_ChunkDesc
	);
}




