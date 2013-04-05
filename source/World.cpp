
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "BlockID.h"
#include "World.h"
#include "ChunkDef.h"
#include "ClientHandle.h"
#include "Pickup.h"
#include "Player.h"
#include "Server.h"
#include "Item.h"
#include "Root.h"
#include "../iniFile/iniFile.h"
#include "ChunkMap.h"

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
#include "Mobs/Ghast.h"
#include "Mobs/Magmacube.h"
#include "Mobs/Mooshroom.h"
#include "Mobs/Ocelot.h"
#include "Mobs/Pig.h"
#include "Mobs/Sheep.h"
#include "Mobs/Silverfish.h"
#include "Mobs/Skeleton.h"
#include "Mobs/Slime.h"
#include "Mobs/Spider.h"
#include "Mobs/Squid.h"
#include "Mobs/Villager.h"
#include "Mobs/Witch.h"
#include "Mobs/Wolf.h"
#include "Mobs/Zombie.h"
#include "Mobs/Zombiepigman.h"

#include "OSSupport/MakeDir.h"
#include "MersenneTwister.h"
#include "Tracer.h"
#include "Generating/Trees.h"
#include "PluginManager.h"
#include "Blocks/BlockHandler.h"
#include "Vector3d.h"

#include "tolua++.h"

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
// cWorld:

cWorld::cWorld(const AString & a_WorldName) :
	m_WorldAgeSecs(0),
	m_TimeOfDaySecs(0),
	m_WorldAge(0),
	m_TimeOfDay(0),
	m_LastTimeUpdate(0),
	m_LastSpawnMonster(0),
	m_RSList(0),
	m_Weather(eWeather_Sunny),
	m_WeatherInterval(24000)  // Guaranteed 1 day of sunshine at server start :)
{
	LOGD("cWorld::cWorld(%s)", a_WorldName.c_str());
	m_WorldName = a_WorldName;
	m_IniFileName = m_WorldName + "/world.ini";

	cMakeDir::MakeDir(m_WorldName.c_str());

	MTRand r1;
	m_SpawnX = (double)((r1.randInt() % 1000) - 500);
	m_SpawnY = cChunkDef::Height;
	m_SpawnZ = (double)((r1.randInt() % 1000) - 500);
	m_GameMode = eGameMode_Creative;

	AString StorageSchema("Default");

	cIniFile IniFile(m_IniFileName);
	IniFile.ReadFile();
	m_Dimension = (eDimension)(IniFile.GetValueSetI("General", "Dimension", 0));
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
			LOGWARNING("Unknown dimension: %d. Setting to Overworld (0)", m_Dimension);
			m_Dimension = dimOverworld;
			break;
		}
	}  // switch (m_Dimension)
	m_SpawnX                    = IniFile.GetValueSetF("SpawnPosition", "X",                         m_SpawnX);
	m_SpawnY                    = IniFile.GetValueSetF("SpawnPosition", "Y",                         m_SpawnY);
	m_SpawnZ                    = IniFile.GetValueSetF("SpawnPosition", "Z",                         m_SpawnZ);
	StorageSchema               = IniFile.GetValueSet ("Storage",       "Schema",                    StorageSchema);
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

	m_Lighting.Start(this);
	m_Storage.Start(this, StorageSchema);
	m_Generator.Start(this, IniFile);

	m_bAnimals = true;
	m_SpawnMonsterRate = 200;  // 1 mob each 10 seconds
	cIniFile IniFile2("settings.ini");
	if (IniFile2.ReadFile())
	{
		m_bAnimals = IniFile2.GetValueB("Monsters", "AnimalsOn", true);
		m_SpawnMonsterRate = (Int64)(IniFile2.GetValueF("Monsters", "AnimalSpawnInterval", 10) * 20);  // Convert from secs to ticks
		
		// TODO: Move this into cServer instead:
		SetMaxPlayers(IniFile2.GetValueI("Server", "MaxPlayers", 100));
		m_Description = IniFile2.GetValue("Server", "Description", "MCServer! - In C++!").c_str();
	}

	m_ChunkMap = new cChunkMap(this);
	
	m_ChunkSender.Start(this);

	m_LastSave = 0;
	m_LastUnload = 0;

	// preallocate some memory for ticking blocks so we don´t need to allocate that often
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

	// Save any changes that the defaults may have done to the ini file:
	if (!IniFile.WriteFile())
	{
		LOGWARNING("Could not write world config to %s", m_IniFileName.c_str());
	}
}





cWorld::~cWorld()
{
	{
		cCSLock Lock(m_CSEntities);
		while (m_AllEntities.begin() != m_AllEntities.end())
		{
			cEntity* Entity = *m_AllEntities.begin();
			m_AllEntities.remove(Entity);
			if (!Entity->IsDestroyed())
			{
				Entity->Destroy();
			}
			delete Entity;
		}
	}

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
}





void cWorld::StopThreads(void)
{
	m_Generator.Stop();
	m_ChunkSender.Stop();
}





void cWorld::Tick(float a_Dt)
{
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

	// Broadcase time update every 40 ticks (2 seconds)
	if (m_LastTimeUpdate < m_WorldAge - 40)
	{
		BroadcastTimeUpdate();
		m_LastTimeUpdate = m_WorldAge;
	}

	// Remove entities scheduled for removal:
	{
		cCSLock Lock(m_CSEntities);
		for (cEntityList::iterator itr = m_AllEntities.begin(); itr != m_AllEntities.end();)
		{
			if ((*itr)->IsDestroyed())
			{
				LOGD("Destroying entity #%i", (*itr)->GetUniqueID());
				cEntity * RemoveMe = *itr;
				itr = m_AllEntities.erase(itr);
				m_RemoveEntityQueue.push_back(RemoveMe); 
				continue;
			}
			(*itr)->Tick(a_Dt, m_TickRand);
			itr++;
		}
	}

	m_ChunkMap->Tick(a_Dt, m_TickRand);

	TickQueuedBlocks(a_Dt);
	
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

	// Delete entities queued for removal:
	for (cEntityList::iterator itr = m_RemoveEntityQueue.begin(); itr != m_RemoveEntityQueue.end(); ++itr)
	{
		delete *itr;
	}
	m_RemoveEntityQueue.clear();

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

	cMonster * Monster = NULL;
	int dayRand   = m_TickRand.randInt() % 6;
	int nightRand = m_TickRand.randInt() % 10;

	SpawnPos += Vector3d((double)(m_TickRand.randInt() % 64) - 32, (double)(m_TickRand.randInt() % 64) - 32, (double)(m_TickRand.randInt() % 64) - 32);
	int Height = GetHeight((int)SpawnPos.x, (int)SpawnPos.z);

	if (m_TimeOfDay >= 12000 + 1000)
	{
		if (GetBiomeAt((int)SpawnPos.x, (int)SpawnPos.z) == biHell) // Spawn nether mobs
		{
			if (nightRand == 1)
				Monster = new cZombie();
			else if (nightRand == 5)
				Monster = new cGhast();
			else if (nightRand == 6)
				Monster = new cZombiepigman();
		}
		else
		{
			if (nightRand == 0) //random percent to spawn for night
				Monster = new cSpider();
			else if (nightRand == 2)
				Monster = new cEnderman();
			else if (nightRand == 3)
				Monster = new cCreeper();
			else if (nightRand == 4)
				Monster = new cCavespider();
			else if (nightRand == 7)
				Monster = new cSlime();
			else if (nightRand == 8)
				Monster = new cSilverfish();
			else if (nightRand == 9)
				Monster = new cSkeleton();
		}
		//end random percent to spawn for night
	}
	else
	{
		if (dayRand == 0) //random percent to spawn for day
			Monster = new cChicken();
		else if (dayRand == 1)
			Monster = new cCow();
		else if (dayRand == 2)
			Monster = new cPig();
		else if (dayRand == 3)
			Monster = new cSheep();
		else if (dayRand == 4)
			Monster = new cSquid();
		else if (dayRand == 5)
			Monster = new cWolf();
		//end random percent to spawn for day
	}

	if (Monster)
	{
		Monster->Initialize(this);
		Monster->TeleportTo(SpawnPos.x, (double)(Height) + 2, SpawnPos.z);
		BroadcastSpawn(*Monster);
	}
}





void cWorld::WakeUpSimulators(int a_BlockX, int a_BlockY, int a_BlockZ)
{
	return m_ChunkMap->WakeUpSimulators(a_BlockX, a_BlockY, a_BlockZ);
}





bool cWorld::ForEachChestInChunk(int a_ChunkX, int a_ChunkZ, cChestCallback & a_Callback)
{
	return m_ChunkMap->ForEachChestInChunk(a_ChunkX, a_ChunkZ, a_Callback);
}





bool cWorld::ForEachDispenserInChunk(int a_ChunkX, int a_ChunkZ, cDispenserCallback & a_Callback)
{
	return m_ChunkMap->ForEachDispenserInChunk(a_ChunkX, a_ChunkZ, a_Callback);
}





bool cWorld::ForEachFurnaceInChunk(int a_ChunkX, int a_ChunkZ, cFurnaceCallback & a_Callback)
{
	return m_ChunkMap->ForEachFurnaceInChunk(a_ChunkX, a_ChunkZ, a_Callback);
}





bool cWorld::DoWithChestAt(int a_BlockX, int a_BlockY, int a_BlockZ, cChestCallback & a_Callback)
{
	return m_ChunkMap->DoWithChestAt(a_BlockX, a_BlockY, a_BlockZ, a_Callback);
}





bool cWorld::DoWithDispenserAt(int a_BlockX, int a_BlockY, int a_BlockZ, cDispenserCallback & a_Callback)
{
	return m_ChunkMap->DoWithDispenserAt(a_BlockX, a_BlockY, a_BlockZ, a_Callback);
}





bool cWorld::DoWithFurnaceAt(int a_BlockX, int a_BlockY, int a_BlockZ, cFurnaceCallback & a_Callback)
{
	return m_ChunkMap->DoWithFurnaceAt(a_BlockX, a_BlockY, a_BlockZ, a_Callback);
}





bool cWorld::GetSignLines(int a_BlockX, int a_BlockY, int a_BlockZ, AString & a_Line1, AString & a_Line2, AString & a_Line3, AString & a_Line4)
{
	return m_ChunkMap->GetSignLines(a_BlockX, a_BlockY, a_BlockZ, a_Line1, a_Line2, a_Line3, a_Line4);
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





void cWorld::GrowTreeFromSapling(int a_X, int a_Y, int a_Z, char a_SaplingMeta)
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





void cWorld::GrowMelonPumpkin(int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockType)
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
		float SpeedY = (float)(a_FlyAwaySpeed *  r1.randInt(1000));
		float SpeedZ = (float)(a_FlyAwaySpeed * (r1.randInt(1000) - 500));
		cPickup * Pickup = new cPickup(
			(int)(a_BlockX * 32) + (r1.randInt(16) + r1.randInt(16) - 16),
			(int)(a_BlockY * 32) + (r1.randInt(16) + r1.randInt(16) - 16),
			(int)(a_BlockZ * 32) + (r1.randInt(16) + r1.randInt(16) - 16),
			*itr, SpeedX, SpeedY, SpeedZ
		);
		Pickup->Initialize(this);
	}
}





void cWorld::SpawnItemPickups(const cItems & a_Pickups, double a_BlockX, double a_BlockY, double a_BlockZ, double a_SpeedX, double a_SpeedY, double a_SpeedZ)
{
	MTRand r1;
	for (cItems::const_iterator itr = a_Pickups.begin(); itr != a_Pickups.end(); ++itr)
	{
		cPickup * Pickup = new cPickup(
			(int)(a_BlockX * 32) + r1.randInt(16) + r1.randInt(16), 
			(int)(a_BlockY * 32) + r1.randInt(16) + r1.randInt(16), 
			(int)(a_BlockZ * 32) + r1.randInt(16) + r1.randInt(16),
			*itr, (float)a_SpeedX, (float)a_SpeedY, (float)a_SpeedZ
		);
		Pickup->Initialize(this);
	}
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





void cWorld::BroadcastAttachEntity(const cEntity & a_Entity, const cEntity * a_Vehicle)
{
	return m_ChunkMap->BroadcastAttachEntity(a_Entity, a_Vehicle);
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





void cWorld::BroadcastPlayerAnimation(const cPlayer & a_Player, char a_Animation, const cClientHandle * a_Exclude)
{
	m_ChunkMap->BroadcastPlayerAnimation(a_Player, a_Animation, a_Exclude);
}





void cWorld::BroadcastEntityEquipment(const cEntity & a_Entity, short a_SlotNum, const cItem & a_Item, const cClientHandle * a_Exclude)
{
	m_ChunkMap->BroadcastEntityEquipment(a_Entity, a_SlotNum, a_Item, a_Exclude);
}





void cWorld::BroadcastEntVelocity(const cEntity & a_Entity, const cClientHandle * a_Exclude)
{
	m_ChunkMap->BroadcastEntVelocity(a_Entity, a_Exclude);
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





void cWorld::BroadcastEntRelMoveLook(const cEntity & a_Entity, char a_RelX, char a_RelY, char a_RelZ, const cClientHandle * a_Exclude)
{
	m_ChunkMap->BroadcastEntRelMoveLook(a_Entity, a_RelX, a_RelY, a_RelZ, a_Exclude);
}





void cWorld::BroadcastEntRelMove(const cEntity & a_Entity, char a_RelX, char a_RelY, char a_RelZ, const cClientHandle * a_Exclude)
{
	m_ChunkMap->BroadcastEntRelMove(a_Entity, a_RelX, a_RelY, a_RelZ, a_Exclude);
}





void cWorld::BroadcastEntLook(const cEntity & a_Entity, const cClientHandle * a_Exclude)
{
	m_ChunkMap->BroadcastEntLook(a_Entity, a_Exclude);
}





void cWorld::BroadcastEntHeadLook(const cEntity & a_Entity, const cClientHandle * a_Exclude)
{
	m_ChunkMap->BroadcastEntHeadLook(a_Entity, a_Exclude);
}





void cWorld::BroadcastBlockAction(int a_BlockX, int a_BlockY, int a_BlockZ, char a_Byte1, char a_Byte2, BLOCKTYPE a_BlockType, const cClientHandle * a_Exclude)
{
	m_ChunkMap->BroadcastBlockAction(a_BlockX, a_BlockY, a_BlockZ, a_Byte1, a_Byte2, a_BlockType, a_Exclude);
}





void cWorld::BroadcastDestroyEntity(const cEntity & a_Entity, const cClientHandle * a_Exclude)
{
	m_ChunkMap->BroadcastDestroyEntity(a_Entity, a_Exclude);
}





void cWorld::BroadcastEntityStatus(const cEntity & a_Entity, char a_Status, const cClientHandle * a_Exclude)
{
	m_ChunkMap->BroadcastEntityStatus(a_Entity, a_Status, a_Exclude);
}





void cWorld::BroadcastMetadata(const cPawn & a_Pawn, const cClientHandle * a_Exclude)
{
	m_ChunkMap->BroadcastMetadata(a_Pawn, a_Exclude);
}





void cWorld::BroadcastSpawn(cEntity & a_Entity, const cClientHandle * a_Exclude)
{
	m_ChunkMap->BroadcastSpawn(a_Entity, a_Exclude);
}





void cWorld::BroadcastCollectPickup(const cPickup & a_Pickup, const cPlayer & a_Player, const cClientHandle * a_Exclude)
{
	m_ChunkMap->BroadcastCollectPickup(a_Pickup, a_Player, a_Exclude);
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





void cWorld::BroadcastChunkData(int a_ChunkX, int a_ChunkZ, cChunkDataSerializer & a_Serializer, const cClientHandle * a_Exclude)
{
	m_ChunkMap->BroadcastChunkData(a_ChunkX, a_ChunkZ, a_Serializer, a_Exclude);
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





void cWorld::BroadcastBlockBreakAnimation(int a_EntityID, int a_BlockX, int a_BlockY, int a_BlockZ, char a_Stage, const cClientHandle * a_Exclude)
{
	m_ChunkMap->BroadcastBlockBreakAnimation(a_EntityID, a_BlockX, a_BlockY, a_BlockZ, a_Stage, a_Exclude);
}





void cWorld::BroadcastUseBed(const cEntity & a_Entity, int a_BlockX, int a_BlockY, int a_BlockZ)
{
	m_ChunkMap->BroadcastUseBed(a_Entity, a_BlockX, a_BlockY, a_BlockZ);
}





void cWorld::BroadcastBlockEntity(int a_BlockX, int a_BlockY, int a_BlockZ, const cClientHandle * a_Exclude)
{
	m_ChunkMap->BroadcastBlockEntity(a_BlockX, a_BlockY, a_BlockZ, a_Exclude);
}





void cWorld::SendBlockEntity(int a_BlockX, int a_BlockY, int a_BlockZ, cClientHandle & a_Client)
{
	m_ChunkMap->SendBlockEntity(a_BlockX, a_BlockY, a_BlockZ, a_Client);
}





void cWorld::MarkChunkDirty (int a_ChunkX, int a_ChunkY, int a_ChunkZ)
{
	m_ChunkMap->MarkChunkDirty (a_ChunkX, a_ChunkY, a_ChunkZ);
}





void cWorld::MarkChunkSaving(int a_ChunkX, int a_ChunkY, int a_ChunkZ)
{
	m_ChunkMap->MarkChunkSaving(a_ChunkX, a_ChunkY, a_ChunkZ);
}





void cWorld::MarkChunkSaved (int a_ChunkX, int a_ChunkY, int a_ChunkZ)
{
	m_ChunkMap->MarkChunkSaved (a_ChunkX, a_ChunkY, a_ChunkZ);
}





void cWorld::SetChunkData(
	int a_ChunkX, int a_ChunkY, int a_ChunkZ,
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
		a_ChunkX, a_ChunkY, a_ChunkZ, 
		a_BlockTypes, a_BlockMeta, a_BlockLight, a_BlockSkyLight,
		a_HeightMap, *Biomes,
		a_Entities, a_BlockEntities,
		a_MarkDirty
	);
	
	// Initialize the entities (outside the m_ChunkMap's CS, to fix FS #347):
	for (cEntityList::iterator itr = a_Entities.begin(), end = a_Entities.end(); itr != end; ++itr)
	{
		(*itr)->Initialize(this);
	}
	
	// If a client is requesting this chunk, send it to them:
	if (m_ChunkMap->HasChunkAnyClients(a_ChunkX, a_ChunkY, a_ChunkZ))
	{
		m_ChunkSender.ChunkReady(a_ChunkX, a_ChunkY, a_ChunkZ);
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





bool cWorld::GetChunkData(int a_ChunkX, int a_ChunkY, int a_ChunkZ, cChunkDataCallback & a_Callback)
{
	return m_ChunkMap->GetChunkData(a_ChunkX, a_ChunkY, a_ChunkZ, a_Callback);
}





bool cWorld::GetChunkBlockTypes(int a_ChunkX, int a_ChunkY, int a_ChunkZ, BLOCKTYPE * a_BlockTypes)
{
	return m_ChunkMap->GetChunkBlockTypes(a_ChunkX, a_ChunkY, a_ChunkZ, a_BlockTypes);
}





bool cWorld::IsChunkValid(int a_ChunkX, int a_ChunkY, int a_ChunkZ) const
{
	return m_ChunkMap->IsChunkValid(a_ChunkX, a_ChunkY, a_ChunkZ);
}





bool cWorld::HasChunkAnyClients(int a_ChunkX, int a_ChunkY, int a_ChunkZ) const
{
	return m_ChunkMap->HasChunkAnyClients(a_ChunkX, a_ChunkY, a_ChunkZ);
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





void cWorld::SetMaxPlayers(int iMax)
{
	m_MaxPlayers = MAX_PLAYERS;
	if (iMax > 0 && iMax < MAX_PLAYERS)
	{
		m_MaxPlayers = iMax;
	}
}





void cWorld::AddPlayer(cPlayer* a_Player)
{
	cCSLock Lock(m_CSPlayers);
	
	ASSERT(std::find(m_Players.begin(), m_Players.end(), a_Player) == m_Players.end());  // Is it already in the list? HOW?
	
	m_Players.remove(a_Player);  // Make sure the player is registered only once
	m_Players.push_back(a_Player);
}





void cWorld::RemovePlayer(cPlayer* a_Player)
{
	cCSLock Lock(m_CSPlayers);
	m_Players.remove(a_Player);
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

		if (Distance <= a_SightLimit)
		{
			if (!LineOfSight.Trace(a_Pos,(Pos - a_Pos),(int)(Pos - a_Pos).Length()))
			{
				if (Distance < ClosestDistance)
				{
					ClosestDistance = Distance;
					ClosestPlayer = *itr;
				}
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





void cWorld::RemoveEntityFromChunk(cEntity * a_Entity, int a_ChunkX, int a_ChunkY, int a_ChunkZ)
{
	m_ChunkMap->RemoveEntityFromChunk(a_Entity, a_ChunkX, a_ChunkY, a_ChunkZ);
}





void cWorld::MoveEntityToChunk(cEntity * a_Entity, int a_ChunkX, int a_ChunkY, int a_ChunkZ)
{
	m_ChunkMap->MoveEntityToChunk(a_Entity, a_ChunkX, a_ChunkY, a_ChunkZ);
}





bool cWorld::ForEachEntity(cEntityCallback & a_Callback)
{
	cCSLock Lock(m_CSEntities);
	for (cEntityList::iterator itr = m_AllEntities.begin(), itr2 = itr; itr != m_AllEntities.end(); itr = itr2)
	{
		++itr2;
		if (a_Callback.Item(*itr))
		{
			return false;
		}
	} // for itr - m_AllEntities[]
	return false;
}





bool cWorld::ForEachEntityInChunk(int a_ChunkX, int a_ChunkZ, cEntityCallback & a_Callback)
{
	return m_ChunkMap->ForEachEntityInChunk(a_ChunkX, a_ChunkZ, a_Callback);
}





bool cWorld::DoWithEntityByID(int a_UniqueID, cEntityCallback & a_Callback)
{
	cCSLock Lock(m_CSEntities);
	for (cEntityList::iterator itr = m_AllEntities.begin(), end = m_AllEntities.end(); itr != end; ++itr)
	{
		if ((*itr)->GetUniqueID() == a_UniqueID)
		{
			return a_Callback.Item(*itr);
		}
	} // for itr - m_AllEntities[]
	return false;
}





void cWorld::CompareChunkClients(int a_ChunkX1, int a_ChunkY1, int a_ChunkZ1, int a_ChunkX2, int a_ChunkY2, int a_ChunkZ2, cClientDiffCallback & a_Callback)
{
	m_ChunkMap->CompareChunkClients(a_ChunkX1, a_ChunkY1, a_ChunkZ1, a_ChunkX2, a_ChunkY2, a_ChunkZ2, a_Callback);
}





bool cWorld::AddChunkClient(int a_ChunkX, int a_ChunkY, int a_ChunkZ, cClientHandle * a_Client)
{
	return m_ChunkMap->AddChunkClient(a_ChunkX, a_ChunkY, a_ChunkZ, a_Client);
}





void cWorld::RemoveChunkClient(int a_ChunkX, int a_ChunkY, int a_ChunkZ, cClientHandle * a_Client)
{
	m_ChunkMap->RemoveChunkClient(a_ChunkX, a_ChunkY, a_ChunkZ, a_Client);
}





void cWorld::RemoveClientFromChunks(cClientHandle * a_Client)
{
	m_ChunkMap->RemoveClientFromChunks(a_Client);
}





void cWorld::SendChunkTo(int a_ChunkX, int a_ChunkY, int a_ChunkZ, cClientHandle * a_Client)
{
	m_ChunkSender.QueueSendChunkTo(a_ChunkX, a_ChunkY, a_ChunkZ, a_Client);
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





void cWorld::UpdateSign(int a_BlockX, int a_BlockY, int a_BlockZ, const AString & a_Line1, const AString & a_Line2, const AString & a_Line3, const AString & a_Line4, cPlayer * a_Player)
{
	AString Line1(a_Line1);
	AString Line2(a_Line2);
	AString Line3(a_Line3);
	AString Line4(a_Line4);
	if (cRoot::Get()->GetPluginManager()->CallHookUpdatingSign(this, a_BlockX, a_BlockY, a_BlockZ, Line1, Line2, Line3, Line4, a_Player))
	{
		return;
	}
	m_ChunkMap->UpdateSign(a_BlockX, a_BlockY, a_BlockZ, Line1, Line2, Line3, Line4);
	cRoot::Get()->GetPluginManager()->CallHookUpdatedSign(this, a_BlockX, a_BlockY, a_BlockZ, Line1, Line2, Line3, Line4, a_Player);
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





void cWorld::AddEntity(cEntity * a_Entity)
{
	cCSLock Lock(m_CSEntities);
	m_AllEntities.push_back(a_Entity); 
}





unsigned int cWorld::GetNumPlayers(void)
{
	cCSLock Lock(m_CSPlayers);
	return m_Players.size(); 
}





int cWorld::GetNumChunks(void) const
{
	return m_ChunkMap->GetNumChunks();
}





void cWorld::GetChunkStats(int & a_NumValid, int & a_NumDirty, int & a_NumInLightingQueue)
{
	m_ChunkMap->GetChunkStats(a_NumValid, a_NumDirty);
	a_NumInLightingQueue = (int) m_Lighting.GetQueueLength();
}





void cWorld::TickQueuedBlocks(float a_Dt)
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
		Block->ToWait -= a_Dt;
		if (Block->ToWait <= 0)
		{
			BlockHandler(GetBlock(Block->X, Block->Y, Block->Z))->OnUpdate(this, Block->X, Block->Y, Block->Z);
			delete Block;	//We don't have to remove it from the vector, this will happen automatically on the next tick
		}
		else
		{
			m_BlockTickQueue.push_back(Block);	//Keep the block in the queue
		}
	}  // for itr - m_BlockTickQueueCopy[]
}





void cWorld::QueueBlockForTick(int a_BlockX, int a_BlockY, int a_BlockZ, float a_TimeToWait)
{
	BlockTickQueueItem * Block = new BlockTickQueueItem;
	Block->X = a_BlockX;
	Block->Y = a_BlockY;
	Block->Z = a_BlockZ;
	Block->ToWait = a_TimeToWait;
	
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





int cWorld::SpawnMob(double a_PosX, double a_PosY, double a_PosZ, int a_EntityType)
{
	cMonster * Monster = NULL;

	switch (a_EntityType)
	{
		case E_ENTITY_TYPE_BAT:           Monster = new cBat();          break;
		case E_ENTITY_TYPE_BLAZE:         Monster = new cBlaze();        break;
		case E_ENTITY_TYPE_CAVE_SPIDER:   Monster = new cCavespider();   break;
		case E_ENTITY_TYPE_CHICKEN:       Monster = new cChicken();      break;
		case E_ENTITY_TYPE_COW:           Monster = new cCow();          break;
		case E_ENTITY_TYPE_CREEPER:       Monster = new cCreeper();      break;
		case E_ENTITY_TYPE_ENDERMAN:      Monster = new cEnderman();     break;
		case E_ENTITY_TYPE_GHAST:         Monster = new cGhast();        break;
		case E_ENTITY_TYPE_MAGMA_CUBE:    Monster = new cMagmacube();    break;
		case E_ENTITY_TYPE_MOOSHROOM:     Monster = new cMooshroom();    break;
		case E_ENTITY_TYPE_OCELOT:        Monster = new cOcelot();       break;
		case E_ENTITY_TYPE_PIG:           Monster = new cPig();          break;
		case E_ENTITY_TYPE_SHEEP:         Monster = new cSheep();        break;
		case E_ENTITY_TYPE_SILVERFISH:    Monster = new cSilverfish();   break;
		case E_ENTITY_TYPE_SKELETON:      Monster = new cSkeleton();     break;
		case E_ENTITY_TYPE_SLIME:         Monster = new cSlime();        break;
		case E_ENTITY_TYPE_SPIDER:        Monster = new cSpider();       break;
		case E_ENTITY_TYPE_SQUID:         Monster = new cSquid();        break;
		case E_ENTITY_TYPE_VILLAGER:      Monster = new cVillager();     break;
		case E_ENTITY_TYPE_WITCH:         Monster = new cWitch();        break;
		case E_ENTITY_TYPE_WOLF:          Monster = new cWolf();         break;
		case E_ENTITY_TYPE_ZOMBIE:        Monster = new cZombie();       break;
		case E_ENTITY_TYPE_ZOMBIE_PIGMAN: Monster = new cZombiepigman(); break;
		
		default:
		{
			LOGWARNING("cWorld::SpawnMob(): Unhandled entity type: %d. Not spawning.", a_EntityType);
			return -1;
		}
	}
	Monster->Initialize(this);
	Monster->TeleportTo(a_PosX, a_PosY, a_PosZ);
	BroadcastSpawn(*Monster);
	return Monster->GetUniqueID();
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
		LOGWARNING("%s [Physics]:%s not present or empty, using the default of \"Floody\".", GetIniFileName().c_str(), SimulatorNameKey.c_str());
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



