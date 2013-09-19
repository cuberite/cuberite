
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "BlockID.h"
#include "World.h"
#include "ChunkDef.h"
#include "ClientHandle.h"
#include "Server.h"
#include "Item.h"
#include "Root.h"
#include "../iniFile/iniFile.h"
#include "ChunkMap.h"
#include "OSSupport/Timer.h"

// Entities (except mobs):
#include "Entities/Pickup.h"
#include "Entities/Player.h"
#include "Entities/TNTEntity.h"

// Simulators:
#include "Simulator/SimulatorManager.h"
#include "Simulator/FloodyFluidSimulator.h"
#include "Simulator/FluidSimulator.h"
#include "Simulator/FireSimulator.h"
#include "Simulator/NoopFluidSimulator.h"
#include "Simulator/SandSimulator.h"
#include "Simulator/RedstoneSimulator.h"
#include "Simulator/VaporizeFluidSimulator.h"

// Mobs:
#include "Mobs/Bat.h"
#include "Mobs/Blaze.h"
#include "Mobs/Cavespider.h"
#include "Mobs/Chicken.h"
#include "Mobs/Cow.h"
#include "Mobs/Creeper.h"
#include "Mobs/Enderman.h"
#include "Mobs/EnderDragon.h"
#include "Mobs/Ghast.h"
#include "Mobs/Giant.h"
#include "Mobs/Horse.h"
#include "Mobs/IronGolem.h"
#include "Mobs/MagmaCube.h"
#include "Mobs/Mooshroom.h"
#include "Mobs/Ocelot.h"
#include "Mobs/Pig.h"
#include "Mobs/Sheep.h"
#include "Mobs/Silverfish.h"
#include "Mobs/Skeleton.h"
#include "Mobs/Slime.h"
#include "Mobs/SnowGolem.h"
#include "Mobs/Spider.h"
#include "Mobs/Squid.h"
#include "Mobs/Villager.h"
#include "Mobs/Witch.h"
#include "Mobs/Wither.h"
#include "Mobs/Wolf.h"
#include "Mobs/Zombie.h"
#include "Mobs/ZombiePigman.h"

#include "OSSupport/MakeDir.h"
#include "MersenneTwister.h"
#include "Generating/Trees.h"
#include "PluginManager.h"
#include "Blocks/BlockHandler.h"
#include "Vector3d.h"

#include "Tracer.h"
#include "tolua++.h"

// DEBUG: Test out the cLineBlockTracer class by tracing a few lines:
#include "LineBlockTracer.h"

#ifndef _WIN32
	#include <stdlib.h>
#endif





/// Up to this many m_SpreadQueue elements are handled each world tick
const int MAX_LIGHTING_SPREAD_PER_TICK = 10;





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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
			LOG("%d chunks to load, %d chunks to generate", 
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





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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
			LOG("%d chunks remaining to light", m_Lighting->GetQueueLength()
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





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cWorld::cLock:

cWorld::cLock::cLock(cWorld & a_World) :
	super(&(a_World.m_ChunkMap->GetCS()))
{
}





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cWorld::cTickThread:

cWorld::cTickThread::cTickThread(cWorld & a_World) :
	super(Printf("WorldTickThread: %s", a_World.GetName().c_str())),
	m_World(a_World)
{
}





void cWorld::cTickThread::Execute(void)
{
	cTimer Timer;

	long long msPerTick = 50;
	long long LastTime = Timer.GetNowTime();

	while (!m_ShouldTerminate)
	{
		long long NowTime = Timer.GetNowTime();
		float DeltaTime = (float)(NowTime - LastTime);
		m_World.Tick(DeltaTime);
		long long TickTime = Timer.GetNowTime() - NowTime;
		
		if (TickTime < msPerTick)
		{
			// Stretch tick time until it's at least msPerTick
			cSleep::MilliSleep((unsigned int)(msPerTick - TickTime));
		}

		LastTime = NowTime;
	}
}





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cWorld:

cWorld::cWorld(const AString & a_WorldName) :
	m_WorldName(a_WorldName),
	m_IniFileName(m_WorldName + "/world.ini"),
	m_StorageSchema("Default"),
	m_WorldAgeSecs(0),
	m_TimeOfDaySecs(0),
	m_WorldAge(0),
	m_TimeOfDay(0),
	m_LastTimeUpdate(0),
	m_LastSpawnMonster(0),
	m_RSList(0),
	m_Weather(eWeather_Sunny),
	m_WeatherInterval(24000),  // Guaranteed 1 day of sunshine at server start :)
	m_TickThread(*this)
{
	LOGD("cWorld::cWorld(\"%s\")", a_WorldName.c_str());

	cMakeDir::MakeDir(m_WorldName.c_str());
}





cWorld::~cWorld()
{
	delete m_SimulatorManager;
	delete m_SandSimulator;
	delete m_WaterSimulator;
	delete m_LavaSimulator;
	delete m_FireSimulator;
	delete m_RedstoneSimulator;

	UnloadUnusedChunks();
	
	m_Storage.WaitForFinish();

	delete m_ChunkMap;
}





void cWorld::CastThunderbolt (int a_BlockX, int a_BlockY, int a_BlockZ)
{
	BroadcastThunderbolt(a_BlockX, a_BlockY, a_BlockZ);
}





void cWorld::SetWeather(eWeather a_NewWeather)
{
	// Do the plugins agree? Do they want a different weather?
	cRoot::Get()->GetPluginManager()->CallHookWeatherChanging(*this, a_NewWeather);
	
	// Set new period for the selected weather:
	switch (a_NewWeather)
	{
		case eWeather_Sunny:        m_WeatherInterval = 14400 + (m_TickRand.randInt() % 4800); break;  // 12 - 16 minutes
		case eWeather_Rain:         m_WeatherInterval =  9600 + (m_TickRand.randInt() % 7200); break;  //  8 - 14 minutes
		case eWeather_ThunderStorm: m_WeatherInterval =  2400 + (m_TickRand.randInt() % 4800); break;  //  2 - 6 minutes
		default:
		{
			LOGWARNING("Requested unknown weather %d, setting sunny for a minute instead.", a_NewWeather);
			a_NewWeather = eWeather_Sunny;
			m_WeatherInterval = 1200;
			break;
		}
	}  // switch (NewWeather)
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
	int ChunkX = 0, ChunkY = 0, ChunkZ = 0;
	BlockToChunk((int)m_SpawnX, (int)m_SpawnY, (int)m_SpawnZ, ChunkX, ChunkY, ChunkZ);
	
	// For the debugging builds, don't make the server build too much world upon start:
	#if defined(_DEBUG) || defined(ANDROID_NDK)
	int ViewDist = 9;
	#else
	int ViewDist = 20;  // Always prepare an area 20 chunks across, no matter what the actual cClientHandle::VIEWDISTANCE is
	#endif  // _DEBUG
	
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
		m_Storage.WaitForQueuesEmpty();
		
		// Wait for the generator to finish generating
		m_Generator.WaitForQueueEmpty();
		
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
	
	// TODO: Better spawn detection - move spawn out of the water if it isn't set in the INI already
	m_SpawnY = (double)GetHeight((int)m_SpawnX, (int)m_SpawnZ) + 1.6f; // +1.6f eye height
	
	
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
	// TODO: Find a proper spawn location, based on the biomes (not in ocean)
	m_SpawnX = (double)((m_TickRand.randInt() % 1000) - 500);
	m_SpawnY = cChunkDef::Height;
	m_SpawnZ = (double)((m_TickRand.randInt() % 1000) - 500);
	m_GameMode = eGameMode_Creative;

	cIniFile IniFile(m_IniFileName);
	if (!IniFile.ReadFile())
	{
		LOGWARNING("Cannot read world settings from \"%s\", defaults will be used.", m_IniFileName.c_str());
	}
	AString Dimension = IniFile.GetValueSet("General", "Dimension", "Overworld");
	m_Dimension = StringToDimension(Dimension);
	switch (m_Dimension)
	{
		case dimNether:
		case dimOverworld:
		case dimEnd:
		{
			break;
		}
		default:
		{
			LOGWARNING("Unknown dimension: \"%s\". Setting to Overworld", Dimension.c_str());
			m_Dimension = dimOverworld;
			break;
		}
	}  // switch (m_Dimension)
	m_SpawnX                    = IniFile.GetValueSetF("SpawnPosition", "X",                         m_SpawnX);
	m_SpawnY                    = IniFile.GetValueSetF("SpawnPosition", "Y",                         m_SpawnY);
	m_SpawnZ                    = IniFile.GetValueSetF("SpawnPosition", "Z",                         m_SpawnZ);
	m_StorageSchema             = IniFile.GetValueSet ("Storage",       "Schema",                    m_StorageSchema);
	m_MaxCactusHeight           = IniFile.GetValueSetI("Plants",        "MaxCactusHeight",           3);
	m_MaxSugarcaneHeight        = IniFile.GetValueSetI("Plants",        "MaxSugarcaneHeight",        3);
	m_IsCactusBonemealable      = IniFile.GetValueSetB("Plants",        "IsCactusBonemealable",      false);
	m_IsCarrotsBonemealable     = IniFile.GetValueSetB("Plants",        "IsCarrotsBonemealable",     true);
	m_IsCropsBonemealable       = IniFile.GetValueSetB("Plants",        "IsCropsBonemealable",       true);
	m_IsGrassBonemealable       = IniFile.GetValueSetB("Plants",        "IsGrassBonemealable",       true);
	m_IsMelonStemBonemealable   = IniFile.GetValueSetB("Plants",        "IsMelonStemBonemealable",   true);
	m_IsMelonBonemealable       = IniFile.GetValueSetB("Plants",        "IsMelonBonemealable",       false);
	m_IsPotatoesBonemealable    = IniFile.GetValueSetB("Plants",        "IsPotatoesBonemealable",    true);
	m_IsPumpkinStemBonemealable = IniFile.GetValueSetB("Plants",        "IsPumpkinStemBonemealable", true);
	m_IsPumpkinBonemealable     = IniFile.GetValueSetB("Plants",        "IsPumpkinBonemealable",     false);
	m_IsSaplingBonemealable     = IniFile.GetValueSetB("Plants",        "IsSaplingBonemealable",     true);
	m_IsSugarcaneBonemealable   = IniFile.GetValueSetB("Plants",        "IsSugarcaneBonemealable",   false);
	m_bEnabledPVP               = IniFile.GetValueSetB("PVP",           "Enabled",                   true);
	m_IsDeepSnowEnabled         = IniFile.GetValueSetB("Physics",       "DeepSnow",                  false);

	m_GameMode = (eGameMode)IniFile.GetValueSetI("GameMode", "GameMode", m_GameMode);

	m_bAnimals = true;
	m_SpawnMonsterRate = 200;  // 1 mob each 10 seconds
	cIniFile IniFile2("settings.ini");
	if (IniFile2.ReadFile())
	{
		m_bAnimals = IniFile2.GetValueB("Monsters", "AnimalsOn", true);
		m_SpawnMonsterRate = (Int64)(IniFile2.GetValueF("Monsters", "AnimalSpawnInterval", 10) * 20);  // Convert from secs to ticks
		
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
	m_RedstoneSimulator = new cRedstoneSimulator(*this);

	// Water and Lava simulators get registered in InitializeFluidSimulator()
	m_SimulatorManager->RegisterSimulator(m_SandSimulator, 1);
	m_SimulatorManager->RegisterSimulator(m_FireSimulator, 1);
	m_SimulatorManager->RegisterSimulator(m_RedstoneSimulator, 1);

	m_Lighting.Start(this);
	m_Storage.Start(this, m_StorageSchema);
	m_Generator.Start(this, IniFile);
	m_ChunkSender.Start(this);
	m_TickThread.Start();

	// Save any changes that the defaults may have done to the ini file:
	if (!IniFile.WriteFile())
	{
		LOGWARNING("Could not write world config to %s", m_IniFileName.c_str());
	}

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





void cWorld::Tick(float a_Dt)
{
	// Call the plugins
	cPluginManager::Get()->CallHookWorldTick(*this, a_Dt);
	
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

	// Broadcast time update every 40 ticks (2 seconds)
	if (m_LastTimeUpdate < m_WorldAge - 40)
	{
		BroadcastTimeUpdate();
		m_LastTimeUpdate = m_WorldAge;
	}

	m_ChunkMap->Tick(a_Dt);

	TickClients(a_Dt);
	TickQueuedBlocks();
	TickQueuedTasks();
	
	GetSimulatorManager()->Simulate(a_Dt);

	TickWeather(a_Dt);

	// Asynchronously set blocks:
	sSetBlockList FastSetBlockQueueCopy;
	{
		cCSLock Lock(m_CSFastSetBlock);
		std::swap(FastSetBlockQueueCopy, m_FastSetBlockQueue);
	}
	m_ChunkMap->FastSetBlocks(FastSetBlockQueueCopy);
	if (!FastSetBlockQueueCopy.empty())
	{
		// Some blocks failed, store them for next tick:
		cCSLock Lock(m_CSFastSetBlock);
		m_FastSetBlockQueue.splice(m_FastSetBlockQueue.end(), FastSetBlockQueueCopy);
	}

	if (m_WorldAge - m_LastSave > 60 * 5 * 20) // Save each 5 minutes
	{
		SaveAllChunks();
	}

	if (m_WorldAge - m_LastUnload > 10 * 20) // Unload every 10 seconds
	{
		UnloadUnusedChunks();
	}

	TickSpawnMobs(a_Dt);

	std::vector<int> m_RSList_copy(m_RSList);
	
	m_RSList.clear();

	std::vector<int>::const_iterator cii;	// FIXME - Please rename this variable, WTF is cii??? Use human readable variable names or common abbreviations (i, idx, itr, iter)
	for (cii = m_RSList_copy.begin(); cii != m_RSList_copy.end();)
	{
		int tempX = *cii; cii++;
		int tempY = *cii; cii++;
		int tempZ = *cii; cii++;
		int state = *cii; cii++;
		
		if ((state == 11111) && ((int)GetBlock(tempX, tempY, tempZ) == E_BLOCK_REDSTONE_TORCH_OFF))
		{
			FastSetBlock(tempX, tempY, tempZ, E_BLOCK_REDSTONE_TORCH_ON, (int)GetBlockMeta(tempX, tempY, tempZ));
		}
		else if ((state == 00000) && ((int)GetBlock(tempX, tempY, tempZ) == E_BLOCK_REDSTONE_TORCH_ON))
		{
			FastSetBlock(tempX, tempY, tempZ, E_BLOCK_REDSTONE_TORCH_OFF, (int)GetBlockMeta(tempX, tempY, tempZ));
		}
	}
	m_RSList_copy.erase(m_RSList_copy.begin(),m_RSList_copy.end());
}





void cWorld::TickWeather(float a_Dt)
{
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
	
		// Pick a new weather. Only reasonable transitions allowed:
		eWeather NewWeather = m_Weather;
		switch (m_Weather)
		{
			case eWeather_Sunny:        NewWeather = eWeather_Rain; break;
			case eWeather_ThunderStorm: NewWeather = eWeather_Rain; break;
			case eWeather_Rain:
			{
				// 1/8 chance of turning into a thunderstorm
				NewWeather = ((m_TickRand.randInt() % 256) < 32) ? eWeather_ThunderStorm : eWeather_Sunny;
				break;
			}
			
			default:
			{
				LOGWARNING("Unknown current weather: %d. Setting sunny.", m_Weather);
				ASSERT(!"Unknown weather");
				NewWeather = eWeather_Sunny;
			}
		}
		
		SetWeather(NewWeather);
	}  // else (m_WeatherInterval > 0)

	if (m_Weather == eWeather_ThunderStorm)
	{
		// 0.5% chance per tick of thunderbolt
		if (m_TickRand.randInt() % 199 == 0)
		{
			CastThunderbolt(0, 0, 0);  // TODO: find random possitions near players to cast thunderbolts.
		}
	}
}





void cWorld::TickSpawnMobs(float a_Dt)
{
	if (!m_bAnimals || (m_WorldAge - m_LastSpawnMonster <= m_SpawnMonsterRate))
	{
		return;
	}
	
	m_LastSpawnMonster = m_WorldAge;
	Vector3d SpawnPos;
	{
		cCSLock Lock(m_CSPlayers);
		if (m_Players.size() <= 0)
		{
			return;
		}
		int RandomPlayerIdx = m_TickRand.randInt() & m_Players.size();
		cPlayerList::iterator itr = m_Players.begin();
		for (int i = 1; i < RandomPlayerIdx; i++)
		{
			itr++;
		}
		SpawnPos = (*itr)->GetPosition();
	}

	int dayRand   = (m_TickRand.randInt() /  7) %  6;
	int nightRand = (m_TickRand.randInt() / 11) % 10;

	SpawnPos += Vector3d((double)(m_TickRand.randInt() % 64) - 32, (double)(m_TickRand.randInt() % 64) - 32, (double)(m_TickRand.randInt() % 64) - 32);
	int Height = GetHeight((int)SpawnPos.x, (int)SpawnPos.z);

	int MobType = -1;
	int Biome = GetBiomeAt((int)SpawnPos.x, (int)SpawnPos.z);
	switch (Biome)
	{
		case biNether:
		{
			// Spawn nether mobs
			switch (nightRand)
			{
				case 5: MobType = cMonster::mtGhast;        break;
				case 6: MobType = cMonster::mtZombiePigman; break;
			}
			break;
		}
		
		case biEnd:
		{
			// Only endermen spawn in the End
			MobType = cMonster::mtEnderman;
			break;
		}
		
		case biMushroomIsland:
		case biMushroomShore:
		{
			// Mushroom land gets only mooshrooms
			MobType = cMonster::mtMooshroom;
			break;
		}
		
		default:
		{
			// Overworld biomes depend on whether it's night or day:
			if (m_TimeOfDay >= 12000 + 1000)
			{
				// Night mobs:
				switch (nightRand)
				{			
					case 0: MobType = cMonster::mtSpider;     break;
					case 1: MobType = cMonster::mtZombie;     break;				
					case 2: MobType = cMonster::mtEnderman;   break;
					case 3: MobType = cMonster::mtCreeper;    break;
					case 4: MobType = cMonster::mtCaveSpider; break;
					case 7: MobType = cMonster::mtSlime;      break;
					case 8: MobType = cMonster::mtSilverfish; break;
					case 9: MobType = cMonster::mtSkeleton;   break;
				}
			}  // if (night)
			else
			{
				// During the day:
				switch (dayRand)
				{
					case 0: MobType = cMonster::mtChicken; break;
					case 1: MobType = cMonster::mtCow;     break;
					case 2: MobType = cMonster::mtPig;     break;
					case 3: MobType = cMonster::mtSheep;   break;
					case 4: MobType = cMonster::mtSquid;   break;
					case 5: MobType = cMonster::mtWolf;    break;
				}
			}  // else (night)
		}  // case overworld biomes
	}  // switch (biome)

	if (MobType >= 0)
	{
		// A proper mob type was selected, now spawn the mob:
		SpawnMob(SpawnPos.x, SpawnPos.y, SpawnPos.z, (cMonster::eType)MobType);
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
	for (cTasks::iterator itr = m_Tasks.begin(), end = m_Tasks.end(); itr != end; ++itr)
	{
		(*itr)->Run(*this);
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
	} // for itr - RemoveClients[]
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
	
	// TODO: Add damage to entities, add support for pickups, and implement block hardiness
	Vector3d explosion_pos = Vector3d(a_BlockX, a_BlockY, a_BlockZ);
	cVector3iArray BlocksAffected;
	m_ChunkMap->DoExplosionAt(a_ExplosionSize, a_BlockX, a_BlockY, a_BlockZ, BlocksAffected);
	BroadcastSoundEffect("random.explode", (int)floor(a_BlockX * 8), (int)floor(a_BlockY * 8), (int)floor(a_BlockZ * 8), 1.0f, 0.6f);
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
		case E_META_SAPLING_APPLE:   GetAppleTreeImage  (a_X, a_Y, a_Z, Noise, (int)(m_WorldAge & 0xffffffff), Logs, Other); break;
		case E_META_SAPLING_BIRCH:   GetBirchTreeImage  (a_X, a_Y, a_Z, Noise, (int)(m_WorldAge & 0xffffffff), Logs, Other); break;
		case E_META_SAPLING_CONIFER: GetConiferTreeImage(a_X, a_Y, a_Z, Noise, (int)(m_WorldAge & 0xffffffff), Logs, Other); break;
		case E_META_SAPLING_JUNGLE:  GetJungleTreeImage (a_X, a_Y, a_Z, Noise, (int)(m_WorldAge & 0xffffffff), Logs, Other); break;
	}
	Other.insert(Other.begin(), Logs.begin(), Logs.end());
	Logs.clear();
	GrowTreeImage(Other);
}





void cWorld::GrowTreeByBiome(int a_X, int a_Y, int a_Z)
{
	cNoise Noise(m_Generator.GetSeed());
	sSetBlockVector Logs, Other;
	GetTreeImageByBiome(a_X, a_Y, a_Z, Noise, (int)(m_WorldAge & 0xffffffff), (EMCSBiome)GetBiomeAt(a_X, a_Z), Logs, Other);
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





int cWorld::GetBiomeAt (int a_BlockX, int a_BlockZ)
{
	return m_ChunkMap->GetBiomeAt(a_BlockX, a_BlockZ);
}





void cWorld::SetBlock(int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta)
{
	if (a_BlockType == E_BLOCK_AIR)
	{
		BlockHandler(GetBlock(a_BlockX, a_BlockY, a_BlockZ))->OnDestroyed(this, a_BlockX, a_BlockY, a_BlockZ);
	}
	m_ChunkMap->SetBlock(a_BlockX, a_BlockY, a_BlockZ, a_BlockType, a_BlockMeta);

	BlockHandler(a_BlockType)->OnPlaced(this, a_BlockX, a_BlockY, a_BlockZ, a_BlockType, a_BlockMeta);
}





void cWorld::FastSetBlock(int a_X, int a_Y, int a_Z, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta)
{
	cCSLock Lock(m_CSFastSetBlock);
	m_FastSetBlockQueue.push_back(sSetBlock(a_X, a_Y, a_Z, a_BlockType, a_BlockMeta)); 
}





void cWorld::QueueSetBlock(int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, int a_TickDelay)
{
	m_ChunkMap->QueueSetBlock(a_BlockX, a_BlockY, a_BlockZ, a_BlockType, a_BlockMeta, GetWorldAge() + a_TickDelay);
}





BLOCKTYPE cWorld::GetBlock(int a_X, int a_Y, int a_Z)
{
	// First check if it isn't queued in the m_FastSetBlockQueue:
	{
		int X = a_X, Y = a_Y, Z = a_Z;
		int ChunkX, ChunkY, ChunkZ;
		AbsoluteToRelative(X, Y, Z, ChunkX, ChunkY, ChunkZ);
		
		cCSLock Lock(m_CSFastSetBlock);
		for (sSetBlockList::iterator itr = m_FastSetBlockQueue.begin(); itr != m_FastSetBlockQueue.end(); ++itr)
		{
			if ((itr->x == X) && (itr->y == Y) && (itr->z == Z) && (itr->ChunkX == ChunkX) && (itr->ChunkZ == ChunkZ))
			{
				return itr->BlockType;
			}
		}  // for itr - m_FastSetBlockQueue[]
	}
	
	return m_ChunkMap->GetBlock(a_X, a_Y, a_Z);
}





NIBBLETYPE cWorld::GetBlockMeta(int a_X, int a_Y, int a_Z)
{
	// First check if it isn't queued in the m_FastSetBlockQueue:
	{
		cCSLock Lock(m_CSFastSetBlock);
		for (sSetBlockList::iterator itr = m_FastSetBlockQueue.begin(); itr != m_FastSetBlockQueue.end(); ++itr)
		{
			if ((itr->x == a_X) && (itr->y == a_Y) && (itr->y == a_Y))
			{
				return itr->BlockMeta;
			}
		}  // for itr - m_FastSetBlockQueue[]
	}
	
	return m_ChunkMap->GetBlockMeta(a_X, a_Y, a_Z);
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





void cWorld::SpawnItemPickups(const cItems & a_Pickups, double a_BlockX, double a_BlockY, double a_BlockZ, double a_FlyAwaySpeed)
{
	MTRand r1;
	a_FlyAwaySpeed /= 1000;  // Pre-divide, so that we don't have to divide each time inside the loop
	for (cItems::const_iterator itr = a_Pickups.begin(); itr != a_Pickups.end(); ++itr)
	{
		float SpeedX = (float)(a_FlyAwaySpeed * (r1.randInt(1000) - 500));
		float SpeedY = 1;
		float SpeedZ = (float)(a_FlyAwaySpeed * (r1.randInt(1000) - 500));
		
		cPickup * Pickup = new cPickup(
			a_BlockX, a_BlockY, a_BlockZ,
			*itr, SpeedX, SpeedY, SpeedZ
		);
		Pickup->Initialize(this);
	}
}





void cWorld::SpawnItemPickups(const cItems & a_Pickups, double a_BlockX, double a_BlockY, double a_BlockZ, double a_SpeedX, double a_SpeedY, double a_SpeedZ)
{
	for (cItems::const_iterator itr = a_Pickups.begin(); itr != a_Pickups.end(); ++itr)
	{
		cPickup * Pickup = new cPickup(
			a_BlockX, a_BlockY, a_BlockZ,
			*itr, (float)a_SpeedX, (float)a_SpeedY, (float)a_SpeedZ
		);
		Pickup->Initialize(this);
	}
}





void cWorld::SpawnPrimedTNT(double a_X, double a_Y, double a_Z, double a_FuseTimeInSec, double a_InitialVelocityCoeff)
{
	cTNTEntity * TNT = new cTNTEntity(a_X, a_Y, a_Z, a_FuseTimeInSec);
	TNT->Initialize(this);
	// TODO: Add a bit of speed in horiz and vert axes, based on the a_InitialVelocityCoeff
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
	cBlockHandler *Handler = cBlockHandler::GetBlockHandler(GetBlock(a_X, a_Y, a_Z));
	Handler->OnDestroyed(this, a_X, a_Y, a_Z);
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





void cWorld::BroadcastChat(const AString & a_Message, const cClientHandle * a_Exclude)
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





void cWorld::BroadcastCollectPickup(const cPickup & a_Pickup, const cPlayer & a_Player, const cClientHandle * a_Exclude)
{
	m_ChunkMap->BroadcastCollectPickup(a_Pickup, a_Player, a_Exclude);
}





void cWorld::BroadcastDestroyEntity(const cEntity & a_Entity, const cClientHandle * a_Exclude)
{
	m_ChunkMap->BroadcastDestroyEntity(a_Entity, a_Exclude);
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




void cWorld::BroadcastPlayerAnimation(const cPlayer & a_Player, char a_Animation, const cClientHandle * a_Exclude)
{
	m_ChunkMap->BroadcastPlayerAnimation(a_Player, a_Animation, a_Exclude);
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





void cWorld::BroadcastSoundEffect(const AString & a_SoundName, int a_SrcX, int a_SrcY, int a_SrcZ, float a_Volume, float a_Pitch, const cClientHandle * a_Exclude)
{
	m_ChunkMap->BroadcastSoundEffect(a_SoundName, a_SrcX, a_SrcY, a_SrcZ, a_Volume, a_Pitch, a_Exclude);
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





void cWorld::MarkChunkDirty (int a_ChunkX, int a_ChunkZ)
{
	m_ChunkMap->MarkChunkDirty (a_ChunkX, a_ChunkZ);
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
		(*itr)->Initialize(this);
	}
	
	// If a client is requesting this chunk, send it to them:
	if (m_ChunkMap->HasChunkAnyClients(a_ChunkX, a_ChunkZ))
	{
		m_ChunkSender.ChunkReady(a_ChunkX, a_ChunkZ);
	}
	
	// Notify the lighting thread that the chunk has become valid (in case it is a neighbor of a postponed chunk):
	m_Lighting.ChunkReady(a_ChunkX, a_ChunkZ);
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





void cWorld::CollectPickupsByPlayer(cPlayer * a_Player)
{
	m_ChunkMap->CollectPickupsByPlayer(a_Player);
}





void cWorld::AddPlayer(cPlayer * a_Player)
{
	{
		cCSLock Lock(m_CSPlayers);
		
		ASSERT(std::find(m_Players.begin(), m_Players.end(), a_Player) == m_Players.end());  // Is it already in the list? HOW?
		
		m_Players.remove(a_Player);  // Make sure the player is registered only once
		m_Players.push_back(a_Player);
	}
	
	// Add the player's client to the list of clients to be ticked:
	if (a_Player->GetClientHandle() != NULL)
	{
		cCSLock Lock(m_CSClients);
		m_ClientsToAdd.push_back(a_Player->GetClientHandle());
	}

	// The player has already been added to the chunkmap as the entity, do NOT add again!
}





void cWorld::RemovePlayer(cPlayer * a_Player)
{
	m_ChunkMap->RemoveEntity(a_Player);
	{
		cCSLock Lock(m_CSPlayers);
		m_Players.remove(a_Player);
	}
	
	// Remove the player's client from the list of clients to be ticked:
	if (a_Player->GetClientHandle() != NULL)
	{
		cCSLock Lock(m_CSClients);
		m_ClientsToRemove.push_back(a_Player->GetClientHandle());
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
	unsigned int BestRating = 0;
	unsigned int NameLength = a_PlayerNameHint.length();

	cCSLock Lock(m_CSPlayers);
	for (cPlayerList::iterator itr = m_Players.begin(); itr != m_Players.end(); ++itr)
	{
		unsigned int Rating = RateCompareString (a_PlayerNameHint, (*itr)->GetName());
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
		LOG("Compared %s and %s with rating %i", a_PlayerNameHint.c_str(), BestMatch->GetName().c_str(), BestRating);
		return a_Callback.Item (BestMatch);
	}
	return false;
}





// TODO: This interface is dangerous!
cPlayer * cWorld::FindClosestPlayer(const Vector3f & a_Pos, float a_SightLimit)
{
	cTracer LineOfSight(this);

	float ClosestDistance = a_SightLimit;
	cPlayer* ClosestPlayer = NULL;

	cCSLock Lock(m_CSPlayers);
	for (cPlayerList::const_iterator itr = m_Players.begin(); itr != m_Players.end(); ++itr)
	{
		Vector3f Pos = (*itr)->GetPosition();
		float Distance = (Pos - a_Pos).Length();

		if (Distance < ClosestDistance)
		{
			if (!LineOfSight.Trace(a_Pos,(Pos - a_Pos),(int)(Pos - a_Pos).Length()))
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





void cWorld::ChunksStay(const cChunkCoordsList & a_Chunks, bool a_Stay)
{
	m_ChunkMap->ChunksStay(a_Chunks, a_Stay);
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
	LOGINFO("Saving all chunks...");
	m_LastSave = m_WorldAge;
	m_ChunkMap->SaveAllChunks();
	m_Storage.QueueSavedMessage();
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





void cWorld::AddEntity(cEntity * a_Entity)
{
	m_ChunkMap->AddEntity(a_Entity);
}





bool cWorld::HasEntity(int a_UniqueID)
{
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

	for (std::vector<BlockTickQueueItem *>::iterator itr = m_BlockTickQueueCopy.begin(); itr != m_BlockTickQueueCopy.end(); itr++)
	{
		BlockTickQueueItem *Block = (*itr);
		Block->TicksToWait -= 1;
		if (Block->TicksToWait <= 0)
		{
			// TODO: Handle the case when the chunk is already unloaded
			BlockHandler(GetBlock(Block->X, Block->Y, Block->Z))->OnUpdate(this, Block->X, Block->Y, Block->Z);
			delete Block;	 // We don't have to remove it from the vector, this will happen automatically on the next tick
		}
		else
		{
			m_BlockTickQueue.push_back(Block);	// Keep the block in the queue
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

	int Size = GetTickRandomNumber(2) + 1;  // 1 .. 3
	
	switch (a_MonsterType)
	{
		case cMonster::mtBat:          Monster = new cBat();           break;
		case cMonster::mtBlaze:        Monster = new cBlaze();         break;
		case cMonster::mtCaveSpider:   Monster = new cCavespider();    break;
		case cMonster::mtChicken:      Monster = new cChicken();       break;
		case cMonster::mtCow:          Monster = new cCow();           break;
		case cMonster::mtCreeper:      Monster = new cCreeper();       break;
		case cMonster::mtEnderman:     Monster = new cEnderman();      break;
		case cMonster::mtEnderDragon:  Monster = new cEnderDragon();   break;
		case cMonster::mtGhast:        Monster = new cGhast();         break;
		case cMonster::mtGiant:        Monster = new cGiant();         break;
		case cMonster::mtHorse:        Monster = new cHorse();         break;
		case cMonster::mtIronGolem:    Monster = new cIronGolem();     break;
		case cMonster::mtMagmaCube:    Monster = new cMagmaCube(Size); break;
		case cMonster::mtMooshroom:    Monster = new cMooshroom();     break;
		case cMonster::mtOcelot:       Monster = new cOcelot();        break;
		case cMonster::mtPig:          Monster = new cPig();           break;
		case cMonster::mtSheep:        Monster = new cSheep();         break;
		case cMonster::mtSilverfish:   Monster = new cSilverfish();    break;
		case cMonster::mtSkeleton:     Monster = new cSkeleton();      break;
		case cMonster::mtSlime:        Monster = new cSlime(Size);     break;
		case cMonster::mtSnowGolem:    Monster = new cSnowGolem();     break;
		case cMonster::mtSpider:       Monster = new cSpider();        break;
		case cMonster::mtSquid:        Monster = new cSquid();         break;
		case cMonster::mtVillager:     Monster = new cVillager();      break;
		case cMonster::mtWitch:        Monster = new cWitch();         break;
		case cMonster::mtWither:       Monster = new cWither();        break;
		case cMonster::mtWolf:         Monster = new cWolf();          break;
		case cMonster::mtZombie:       Monster = new cZombie();        break;
		case cMonster::mtZombiePigman: Monster = new cZombiePigman();  break;
		
		default:
		{
			LOGWARNING("%s: Unhandled monster type: %d. Not spawning.", __FUNCTION__, a_MonsterType);
			return -1;
		}
	}
	Monster->SetPosition(a_PosX, a_PosY, a_PosZ);
	Monster->SetHealth(Monster->GetMaxHealth());
	if (cPluginManager::Get()->CallHookSpawningMonster(*this, *Monster))
	{
		delete Monster;
		return -1;
	}
	if (!Monster->Initialize(this))
	{
		delete Monster;
		return -1;
	}
	BroadcastSpawnEntity(*Monster);
	cPluginManager::Get()->CallHookSpawnedMonster(*this, *Monster);
	return Monster->GetUniqueID();
}





int cWorld::CreateProjectile(double a_PosX, double a_PosY, double a_PosZ, cProjectileEntity::eKind a_Kind, cEntity * a_Creator, const Vector3d * a_Speed)
{
	cProjectileEntity * Projectile = cProjectileEntity::Create(a_Kind, a_Creator, a_PosX, a_PosY, a_PosZ, a_Speed);
	if (Projectile == NULL)
	{
		return -1;
	}
	if (!Projectile->Initialize(this))
	{
		delete Projectile;
		return -1;
	}
	BroadcastSpawnEntity(*Projectile);
	return Projectile->GetUniqueID();
}





void cWorld::TabCompleteUserName(const AString & a_Text, AStringVector & a_Results)
{
	cCSLock Lock(m_CSPlayers);
	for (cPlayerList::iterator itr = m_Players.begin(), end = m_Players.end(); itr != end; ++itr)
	{
		size_t LastSpace = a_Text.find_last_of(" "); //Find the position of the last space
		
		std::string LastWord = a_Text.substr(LastSpace + 1, a_Text.length()); //Find the last word
		std::string PlayerName ((*itr)->GetName());
		std::size_t Found = PlayerName.find(LastWord); //Try to find last word in playername
		
		if (Found!=0)
		{
			continue; //No match
		}
		
		a_Results.push_back((*itr)->GetName()); //Match!
	}
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
		LOGWARNING("[Physics] %s not present or empty in %s, using the default of \"Floody\".", SimulatorNameKey.c_str(), GetIniFileName().c_str());
		SimulatorName = "Floody";
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
		if (NoCaseCompare(SimulatorName, "floody") != 0)
		{
			// The simulator name doesn't match anything we have, issue a warning:
			LOGWARNING("%s [Physics]:%s specifies an unknown simulator, using the default \"Floody\".", GetIniFileName().c_str(), SimulatorNameKey.c_str());
		}
		int Falloff               = a_IniFile.GetValueSetI(SimulatorSectionName, "Falloff",               IsWater ? 1 : 2);
		int TickDelay             = a_IniFile.GetValueSetI(SimulatorSectionName, "TickDelay",             IsWater ? 5 : 30);
		int NumNeighborsForSource = a_IniFile.GetValueSetI(SimulatorSectionName, "NumNeighborsForSource", IsWater ? 2 : -1);
		res = new cFloodyFluidSimulator(*this, a_SimulateBlock, a_StationaryBlock, Falloff, TickDelay, NumNeighborsForSource);
	}
	
	m_SimulatorManager->RegisterSimulator(res, Rate);

	return res;
}




///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cWorld::cTaskSaveAllChunks:

void cWorld::cTaskSaveAllChunks::Run(cWorld & a_World)
{
	a_World.SaveAllChunks();
}




