
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "BlockID.h"
#include "cWorld.h"
#include "cRedstone.h"
#include "cChunk.h"
#include "cClientHandle.h"
#include "cPickup.h"
#include "cBlockToPickup.h"
#include "cPlayer.h"
#include "cServer.h"
#include "cItem.h"
#include "cRoot.h"
#include "../iniFile/iniFile.h"
#include "cChunkMap.h"
#include "cSimulatorManager.h"
#include "cWaterSimulator.h"
#include "cLavaSimulator.h"
#include "cFireSimulator.h"
#include "cSandSimulator.h"
#include "cChicken.h"
#include "cSpider.h"
#include "cCow.h" //cow
#include "cSquid.h" //Squid
#include "cWolf.h" //wolf
#include "cSlime.h" //slime
#include "cSkeleton.h" //Skeleton
#include "cSilverfish.h" //Silverfish
#include "cPig.h" //pig
#include "cSheep.h" //sheep
#include "cZombie.h" //zombie
#include "cEnderman.h" //enderman
#include "cCreeper.h" //creeper
#include "cCavespider.h" //cavespider
#include "cGhast.h" //Ghast
#include "cZombiepigman.h" //Zombiepigman
#include "cGenSettings.h"
#include "cMakeDir.h"
#include "cChunkGenerator.h"
#include "MersenneTwister.h"
#include "cWorldGenerator_Test.h"
#include "cTracer.h"


#include "packets/cPacket_TimeUpdate.h"
#include "packets/cPacket_NewInvalidState.h"
#include "packets/cPacket_Thunderbolt.h"

#include "Vector3d.h"

#include <time.h>

#include "tolua++.h"

#ifndef _WIN32
	#include <stdlib.h>
#endif





/// Up to this many m_SpreadQueue elements are handled each world tick
const int MAX_LIGHTING_SPREAD_PER_TICK = 10;





float cWorld::m_Time = 0.f;

char g_BlockLightValue[128];
char g_BlockSpreadLightFalloff[128];
bool g_BlockTransparent[128];
bool g_BlockOneHitDig[128];
bool g_BlockPistonBreakable[128];





#define RECI_RAND_MAX (1.f/RAND_MAX)
inline float fRadRand( float a_Radius )
{
	MTRand r1;
	return ((float)r1.rand() * RECI_RAND_MAX)*a_Radius - a_Radius*0.5f;
}





cWorld* cWorld::GetWorld()
{
	LOGWARN("WARNING: Using deprecated function cWorld::GetWorld() use cRoot::Get()->GetWorld() instead!");
	return cRoot::Get()->GetWorld();
}





cWorld::~cWorld()
{
	{
		cCSLock Lock(m_CSEntities);
		while( m_AllEntities.begin() != m_AllEntities.end() )
		{
			cEntity* Entity = *m_AllEntities.begin();
			m_AllEntities.remove( Entity );
			if ( !Entity->IsDestroyed() )
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

	m_Generator.Stop();

	UnloadUnusedChunks();
	
	m_Storage.WaitForFinish();

	delete m_ChunkMap;
}





cWorld::cWorld( const AString & a_WorldName )
	: m_SpawnMonsterTime( 0.f )
	, m_RSList ( 0 )
	, m_Weather ( 0 )
{
	LOG("cWorld::cWorld(%s)", a_WorldName.c_str());
	m_WorldName = a_WorldName;

	cMakeDir::MakeDir(m_WorldName.c_str());

	MTRand r1;
	m_SpawnX = (double)((r1.randInt()%1000)-500);
	m_SpawnY = 128;
	m_SpawnZ = (double)((r1.randInt()%1000)-500);
	m_WorldSeed = r1.randInt();
	m_GameMode = 0;

	AString GeneratorName;
	AString StorageSchema;

	cIniFile IniFile( m_WorldName + "/world.ini");
	if( IniFile.ReadFile() )
	{
		m_SpawnX = IniFile.GetValueF("SpawnPosition", "X", m_SpawnX );
		m_SpawnY = IniFile.GetValueF("SpawnPosition", "Y", m_SpawnY );
		m_SpawnZ = IniFile.GetValueF("SpawnPosition", "Z", m_SpawnZ );
		m_WorldSeed = IniFile.GetValueI("Seed", "Seed", m_WorldSeed );
		m_GameMode = IniFile.GetValueI("GameMode", "GameMode", m_GameMode );
		GeneratorName = IniFile.GetValue("Generator", "GeneratorName", "Default");
		StorageSchema = IniFile.GetValue("Storage", "Schema", "Default");
	}
	else
	{
		IniFile.SetValueF("SpawnPosition", "X", m_SpawnX );
		IniFile.SetValueF("SpawnPosition", "Y", m_SpawnY );
		IniFile.SetValueF("SpawnPosition", "Z", m_SpawnZ );
		IniFile.SetValueI("Seed", "Seed", m_WorldSeed );
		IniFile.SetValueI("GameMode", "GameMode", m_GameMode );
		IniFile.SetValue("Generator", "GeneratorName", "Default" );
		if( !IniFile.WriteFile() )
		{
			LOG("WARNING: Could not write to %s/world.ini", a_WorldName.c_str());
		}
	}
	LOGINFO("Seed: %i", m_WorldSeed );

	m_Storage.Start(this, StorageSchema);
	m_Generator.Start(this, GeneratorName);
	cIniFile GenSettings("terrain.ini");
	if( GenSettings.ReadFile() )
	{
#define READ_INI_TERRAIN_VAL( var, type ) cGenSettings::var = (type)GenSettings.GetValueF("Terrain", #var, cGenSettings::var )
		READ_INI_TERRAIN_VAL( HeightFreq1, float );
		READ_INI_TERRAIN_VAL( HeightFreq2, float );
		READ_INI_TERRAIN_VAL( HeightFreq3, float );
		READ_INI_TERRAIN_VAL( HeightAmp1, float );
		READ_INI_TERRAIN_VAL( HeightAmp2, float );
		READ_INI_TERRAIN_VAL( HeightAmp3, float );
	}
	else
	{
#define SET_INI_TERRAIN_VAL( var ) GenSettings.SetValueF("Terrain", #var, cGenSettings::var )
		SET_INI_TERRAIN_VAL( HeightFreq1 );
		SET_INI_TERRAIN_VAL( HeightFreq2 );
		SET_INI_TERRAIN_VAL( HeightFreq3 );
		SET_INI_TERRAIN_VAL( HeightAmp1 );
		SET_INI_TERRAIN_VAL( HeightAmp2 );
		SET_INI_TERRAIN_VAL( HeightAmp3 );
		GenSettings.WriteFile();
	}

	m_bAnimals = true;
	m_SpawnMonsterRate = 10;
	cIniFile IniFile2("settings.ini");
	if( IniFile2.ReadFile() )
	{
		m_bAnimals = IniFile2.GetValueB("Monsters", "AnimalsOn", true );
		m_SpawnMonsterRate = (float)IniFile2.GetValueF("Monsters", "AnimalSpawnInterval", 10 );
		SetMaxPlayers(IniFile2.GetValueI("Server", "MaxPlayers", 9001));
		m_Description = IniFile2.GetValue("Server", "Description", "MCServer! - It's OVER 9000!").c_str();
	}

	m_ChunkMap = new cChunkMap(this );

	m_Time = 0;
	m_WorldTimeFraction = 0.f;
	m_WorldTime = 0;
	m_LastSave = 0;
	m_LastUnload = 0;

	//Simulators:
	m_WaterSimulator = new cWaterSimulator( this );
	m_LavaSimulator = new cLavaSimulator( this );
	m_SandSimulator = new cSandSimulator(this);
	m_FireSimulator = new cFireSimulator(this);

	m_SimulatorManager = new cSimulatorManager();
	m_SimulatorManager->RegisterSimulator(m_WaterSimulator, 6);
	m_SimulatorManager->RegisterSimulator(m_LavaSimulator, 12);
	m_SimulatorManager->RegisterSimulator(m_SandSimulator, 1);
	m_SimulatorManager->RegisterSimulator(m_FireSimulator, 10);

	memset( g_BlockLightValue, 0x0, 128 );
	memset( g_BlockSpreadLightFalloff, 0xf, 128 ); // 0xf means total falloff
	memset( g_BlockTransparent, 0x0, 128 );
	memset( g_BlockOneHitDig, 0x0, 128 );
	memset( g_BlockPistonBreakable, 0x0, 128 );

	// Emissive blocks
	g_BlockLightValue[ E_BLOCK_TORCH ] =			14;
	g_BlockLightValue[ E_BLOCK_FIRE ] =				15;
	g_BlockLightValue[ E_BLOCK_LAVA ] =				15;
	g_BlockLightValue[ E_BLOCK_STATIONARY_LAVA ] =	15;
	g_BlockLightValue[ E_BLOCK_GLOWSTONE ] =		15;

	// Spread blocks
	g_BlockSpreadLightFalloff[ E_BLOCK_AIR ]				= 1;
	g_BlockSpreadLightFalloff[ E_BLOCK_TORCH ]				= 1;
	g_BlockSpreadLightFalloff[ E_BLOCK_FIRE ]				= 1;
	g_BlockSpreadLightFalloff[ E_BLOCK_LAVA ]				= 1;
	g_BlockSpreadLightFalloff[ E_BLOCK_STATIONARY_LAVA ]	= 1;
	g_BlockSpreadLightFalloff[ E_BLOCK_WATER ]				= 4;				// Light in water dissapears faster
	g_BlockSpreadLightFalloff[ E_BLOCK_STATIONARY_WATER ]	= 4;
	g_BlockSpreadLightFalloff[ E_BLOCK_LEAVES ]				= 1;
	g_BlockSpreadLightFalloff[ E_BLOCK_GLASS ]				= 1;
	g_BlockSpreadLightFalloff[ E_BLOCK_GLOWSTONE ]			= 1;
	g_BlockSpreadLightFalloff[ E_BLOCK_SIGN_POST ]			= 1;
	g_BlockSpreadLightFalloff[ E_BLOCK_WALLSIGN ]			= 1;

	// Transparent blocks
	g_BlockTransparent[ E_BLOCK_AIR ]		= true;
	g_BlockTransparent[ E_BLOCK_GLASS ]		= true;
	g_BlockTransparent[ E_BLOCK_FIRE ]		= true;
	g_BlockTransparent[ E_BLOCK_ICE ] 		= true;
	g_BlockTransparent[ E_BLOCK_TORCH ]		= true;
	g_BlockTransparent[ E_BLOCK_SIGN_POST ] = true;
	g_BlockTransparent[ E_BLOCK_WALLSIGN ]	= true;

	// One hit break blocks
	g_BlockOneHitDig[ E_BLOCK_SAPLING ]				= true;
	g_BlockOneHitDig[ E_BLOCK_YELLOW_FLOWER ]		= true;
	g_BlockOneHitDig[ E_BLOCK_RED_ROSE ]			= true;
	g_BlockOneHitDig[ E_BLOCK_BROWN_MUSHROOM ]		= true;
	g_BlockOneHitDig[ E_BLOCK_RED_MUSHROOM ]		= true;
	g_BlockOneHitDig[ E_BLOCK_TNT ]					= true;
	g_BlockOneHitDig[ E_BLOCK_TORCH ]				= true;
	g_BlockOneHitDig[ E_BLOCK_REDSTONE_WIRE ]		= true;
	g_BlockOneHitDig[ E_BLOCK_CROPS ]				= true;
	g_BlockOneHitDig[ E_BLOCK_REDSTONE_TORCH_OFF ]	= true;
	g_BlockOneHitDig[ E_BLOCK_REDSTONE_TORCH_ON ]	= true;
	g_BlockOneHitDig[ E_BLOCK_REEDS ]				= true;
	g_BlockOneHitDig[ E_BLOCK_REDSTONE_WIRE ]		= true;
	g_BlockOneHitDig[ E_BLOCK_REDSTONE_REPEATER_OFF ]		= true;
	g_BlockOneHitDig[ E_BLOCK_REDSTONE_REPEATER_ON ]		= true;
	g_BlockOneHitDig[ E_BLOCK_LOCKED_CHEST ]		= true;
	g_BlockOneHitDig [ E_BLOCK_FIRE ]				= true;

	// Blocks that breaks when pushed by piston
	g_BlockPistonBreakable[ E_BLOCK_AIR ]				= true;
	g_BlockPistonBreakable[ E_BLOCK_STATIONARY_WATER ]	= false;		//This gave pistons the ability to drop water :D
	g_BlockPistonBreakable[ E_BLOCK_WATER ]				= false;
	g_BlockPistonBreakable[ E_BLOCK_STATIONARY_LAVA ]	= false;
	g_BlockPistonBreakable[ E_BLOCK_LAVA ]				= false;
	g_BlockPistonBreakable[ E_BLOCK_BED ]				= true;
	g_BlockPistonBreakable[ E_BLOCK_COBWEB ]			= true;
	g_BlockPistonBreakable[ E_BLOCK_TALL_GRASS ]		= true;
	g_BlockPistonBreakable[ E_BLOCK_YELLOW_FLOWER ]		= true;
	g_BlockPistonBreakable[ E_BLOCK_BROWN_MUSHROOM ]	= true;
	g_BlockPistonBreakable[ E_BLOCK_RED_ROSE ]			= true;
	g_BlockPistonBreakable[ E_BLOCK_RED_MUSHROOM ]		= true;
	g_BlockPistonBreakable[ E_BLOCK_DEAD_BUSH ]			= true;
	g_BlockPistonBreakable[ E_BLOCK_TORCH ]				= true;
	g_BlockPistonBreakable[ E_BLOCK_FIRE ]				= true;
	g_BlockPistonBreakable[ E_BLOCK_REDSTONE_WIRE ]		= true;
	g_BlockPistonBreakable[ E_BLOCK_CROPS ]				= true;
	g_BlockPistonBreakable[ E_BLOCK_LADDER ]			= true;
	g_BlockPistonBreakable[ E_BLOCK_WOODEN_DOOR ]		= true;
	g_BlockPistonBreakable[ E_BLOCK_IRON_DOOR ]			= true;
	g_BlockPistonBreakable[ E_BLOCK_LEVER ]				= true;
	g_BlockPistonBreakable[ E_BLOCK_STONE_BUTTON ]		= true;
	g_BlockPistonBreakable[ E_BLOCK_REDSTONE_TORCH_ON ]	= true;
	g_BlockPistonBreakable[ E_BLOCK_REDSTONE_TORCH_OFF ]= true;
	g_BlockPistonBreakable[ E_BLOCK_SNOW ]				= true;
	g_BlockPistonBreakable[ E_BLOCK_REEDS ]				= true;
	g_BlockPistonBreakable[ E_BLOCK_PUMPKIN_STEM ]		= true;
	g_BlockPistonBreakable[ E_BLOCK_MELON_STEM ]		= true;
	g_BlockPistonBreakable[ E_BLOCK_MELON ]				= true;
	g_BlockPistonBreakable[ E_BLOCK_PUMPKIN ]			= true;
	g_BlockPistonBreakable[ E_BLOCK_JACK_O_LANTERN ]	= true;
	g_BlockPistonBreakable[ E_BLOCK_VINES ]				= true;
	g_BlockPistonBreakable[ E_BLOCK_STONE_PRESSURE_PLATE ] = true;
	g_BlockPistonBreakable[ E_BLOCK_WOODEN_PRESSURE_PLATE ] = true;
}





void cWorld::SetWeather( int Weather )
{
	if (Weather == 2) { //thunder storm
		m_Weather = 2;
		cPacket_NewInvalidState WeatherPacket;
		WeatherPacket.m_Reason = 1; //begin rain
		Broadcast ( WeatherPacket );
		CastThunderbolt ( 0, 0, 0 ); //start thunderstorm with a lightning strike at 0, 0, 0. >:D
	}
	if (Weather == 1) { //rainstorm
		m_Weather = 1;
		cPacket_NewInvalidState WeatherPacket;
		WeatherPacket.m_Reason = 1; //begin rain
		Broadcast ( WeatherPacket );
	}
	if (Weather == 0) { //sunny
		m_Weather = 0;
		cPacket_NewInvalidState WeatherPacket;
		WeatherPacket.m_Reason = 2; //stop rain
		Broadcast ( WeatherPacket );
	}
}





void cWorld::CastThunderbolt ( int X, int Y, int Z )
{
	cPacket_Thunderbolt ThunderboltPacket;
	ThunderboltPacket.m_xLBPos = X;
	ThunderboltPacket.m_yLBPos = Y;
	ThunderboltPacket.m_zLBPos = Z;
	Broadcast( ThunderboltPacket );
}





void cWorld::InitializeSpawn()
{
	int ChunkX = 0, ChunkY = 0, ChunkZ = 0;
	BlockToChunk( (int)m_SpawnX, (int)m_SpawnY, (int)m_SpawnZ, ChunkX, ChunkY, ChunkZ );
	int ViewDist = 20;  // Always prepare an area 20 chunks across, no matter what the actual cClientHandle::VIEWDISTANCE is
	LOG("Preparing spawn area in world \"%s\"", m_WorldName.c_str());
	for (int x = 0; x < ViewDist; x++)
	{
		for (int z = 0; z < ViewDist; z++)
		{
			GetChunk( x + ChunkX-(ViewDist - 1) / 2, 0, z + ChunkZ-(ViewDist - 1) / 2 );  // Queue the chunk in the generator / loader
		}
		LOG("Queued %0.2f %% of spawn area", ((float)x / (float)ViewDist) * 100 );
	}
	
	// TODO: Wait for the generator to finish generating these chunks
}





void cWorld::Tick(float a_Dt)
{
	m_Time += a_Dt / 1000.f;

	CurrentTick++;

	bool bSendTime = false;
	m_WorldTimeFraction += a_Dt / 1000.f;
	while ( m_WorldTimeFraction > 1.f )
	{
		m_WorldTimeFraction -= 1.f;
		m_WorldTime += 20;
		bSendTime = true;
	}
	m_WorldTime %= 24000; // 24000 units in a day
	if ( bSendTime )
	{
		Broadcast( cPacket_TimeUpdate( (m_WorldTime) ) );
	}

	{
		cCSLock Lock(m_CSEntities);
		for (cEntityList::iterator itr = m_AllEntities.begin(); itr != m_AllEntities.end();)
		{
			if ((*itr)->IsDestroyed())
			{
				LOG("Destroying entity #%i", (*itr)->GetUniqueID());
				cEntity * RemoveMe = *itr;
				itr = m_AllEntities.erase( itr );
				m_RemoveEntityQueue.push_back( RemoveMe ); 
				continue;
			}
			(*itr)->Tick(a_Dt);
			itr++;
		}
	}

	{
		cCSLock Lock(m_CSLighting);
		if (m_SpreadQueue.size() >= 50 )
		{
			LOGWARN("cWorld: Lots of lighting to do! Still %i chunks left!", m_SpreadQueue.size() );
		}
		int TimesSpreaded = 0;
		while ( !m_SpreadQueue.empty() && TimesSpreaded < MAX_LIGHTING_SPREAD_PER_TICK ) // Do not choke the tick thread
		{
			cChunkPtr & Chunk = *m_SpreadQueue.begin();
			//LOG("Spreading: %p", Chunk );
			Chunk->SpreadLight( Chunk->pGetSkyLight() );
			Chunk->SpreadLight( Chunk->pGetLight() );
			m_SpreadQueue.pop_front();
			TimesSpreaded++;
		}
	}

	m_ChunkMap->Tick(a_Dt, m_TickRand);
	
	GetSimulatorManager()->Simulate(a_Dt);

	TickWeather(a_Dt);

	// Asynchronously set blocks
	FastSetBlockList FastSetBlockQueueCopy;
	{
		cCSLock Lock(m_CSFastSetBlock);
		FastSetBlockQueueCopy = m_FastSetBlockQueue;
		m_FastSetBlockQueue.clear();
	}
	for ( FastSetBlockList::iterator itr = FastSetBlockQueueCopy.begin(); itr != FastSetBlockQueueCopy.end(); ++itr )
	{
		sSetBlockData & SetBlockData = *itr;
		FastSetBlock( SetBlockData.x, SetBlockData.y, SetBlockData.z, SetBlockData.BlockID, SetBlockData.BlockMeta );	// If unable to set block, it's added to FastSetBlockQueue again
	}

	if( m_Time - m_LastSave > 60 * 5 ) // Save each 5 minutes
	{
		SaveAllChunks();
	}

	if( m_Time - m_LastUnload > 10 ) // Unload every 10 seconds
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
	for(cii=m_RSList_copy.begin(); cii!=m_RSList_copy.end();)
	{
		int tempX = *cii;cii++;
		int tempY = *cii;cii++;
		int tempZ = *cii;cii++;
		int state = *cii;cii++;
		
		if ( (state == 11111) && ( (int)GetBlock( tempX, tempY, tempZ ) == E_BLOCK_REDSTONE_TORCH_OFF ) )
		{
			FastSetBlock( tempX, tempY, tempZ, E_BLOCK_REDSTONE_TORCH_ON, (int)GetBlockMeta( tempX, tempY, tempZ ) );
			cRedstone Redstone(this);
			Redstone.ChangeRedstone( tempX, tempY, tempZ, true );
		}
		else if ( (state == 00000) && ( (int)GetBlock( tempX, tempY, tempZ ) == E_BLOCK_REDSTONE_TORCH_ON ) )
		{
			FastSetBlock( tempX, tempY, tempZ, E_BLOCK_REDSTONE_TORCH_OFF, (int)GetBlockMeta( tempX, tempY, tempZ ) );
			cRedstone Redstone(this);
			Redstone.ChangeRedstone( tempX, tempY, tempZ, false );
		}
	}
	m_RSList_copy.erase(m_RSList_copy.begin(),m_RSList_copy.end());
}





void cWorld::TickWeather(float a_Dt)
{
	if ( GetWeather() == 0 )  // if sunny
	{
		if( CurrentTick % 19 == 0 )  //every 20 ticks random weather
		{
			unsigned randWeather = (m_TickRand.randInt() % 10000);
			if (randWeather == 0)
			{
				LOG("Starting Rainstorm!");
				SetWeather ( 1 );
			}
			else if  (randWeather == 1)
			{
				LOG("Starting Thunderstorm!");
				SetWeather ( 2 );
			}
		}
	}

	if ( GetWeather() != 0 )  // if raining or thunderstorm
	{
		if ( CurrentTick % 19 == 0 ) // every 20 ticks random weather
		{
			unsigned randWeather = (m_TickRand.randInt() % 4999);
			if (randWeather == 0)  //2% chance per second
			{
				LOG("Back to sunny!");
				SetWeather ( 0 );
			}
			else if ( (randWeather > 4000) && (GetWeather() != 2) )  // random chance for rainstorm to turn into thunderstorm.
			{
				LOG("Starting Thunderstorm!");
				SetWeather ( 2 );
			}
		}
	}

	if ( GetWeather() == 2 )  // if thunderstorm
	{
		if (m_TickRand.randInt() % 199 == 0)  // 0.5% chance per tick of thunderbolt
		{
			CastThunderbolt ( 0, 0, 0 );  // todo: find random possitions near players to cast thunderbolts.
		}
	}
}





void cWorld::TickSpawnMobs(float a_Dt)
{
	if (!m_bAnimals || (m_Time - m_SpawnMonsterTime <= m_SpawnMonsterRate))
	{
		return;
	}
	
	m_SpawnMonsterTime = m_Time;
	Vector3d SpawnPos;
	{
		cCSLock Lock(m_CSPlayers);
		if ( m_Players.size() <= 0)
		{
			return;
		}
		int RandomPlayerIdx = m_TickRand.randInt() & m_Players.size();
		cPlayerList::iterator itr = m_Players.begin();
		for( int i = 1; i < RandomPlayerIdx; i++ )
		{
			itr++;
		}
		SpawnPos = (*itr)->GetPosition();
	}

	cMonster * Monster = NULL;
	int dayRand   = m_TickRand.randInt() % 6;
	int nightRand = m_TickRand.randInt() % 10;

	SpawnPos += Vector3d( (double)(m_TickRand.randInt() % 64) - 32, (double)(m_TickRand.randInt() % 64) - 32, (double)(m_TickRand.randInt() % 64) - 32 );
	char Height = GetHeight( (int)SpawnPos.x, (int)SpawnPos.z );

	if (m_WorldTime >= 12000 + 1000)
	{
		if (nightRand == 0) //random percent to spawn for night
			Monster = new cSpider();
		else if (nightRand == 1)
			Monster = new cZombie();
		else if (nightRand == 2)
			Monster = new cEnderman();
		else if (nightRand == 3)
			Monster = new cCreeper();
		else if (nightRand == 4)
			Monster = new cCavespider();
		else if (nightRand == 5)
			Monster = new cGhast();
		else if (nightRand == 6)
			Monster = new cZombiepigman();
		else if (nightRand == 7)
			Monster = new cSlime();
		else if (nightRand == 8)
			Monster = new cSilverfish();
		else if (nightRand == 9)
			Monster = new cSkeleton();
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

	if( Monster )
	{
		Monster->Initialize( this );
		Monster->TeleportTo( SpawnPos.x, (double)(Height) + 2, SpawnPos.z );
		Monster->SpawnOn(0);
	}
}





void cWorld::GrowTree( int a_X, int a_Y, int a_Z )
{
	// new tree code, looks much better
	// with help from seanj
	// converted from php to lua then lua to c++

	// build trunk
	MTRand r1;
	int trunk = r1.randInt() % (7 - 5 + 1) + 5;
	for (int i = 0; i < trunk; i++) 
	{
		if( GetBlock( a_X, a_Y + i, a_Z ) == E_BLOCK_AIR )
			FastSetBlock( a_X, a_Y + i, a_Z, E_BLOCK_LOG, 0 );
	}

	// build tree
	for (int j = 0; j < trunk; j++) {
		int radius = trunk - j;
		if (radius < 4) {
			if (radius > 2) {
				radius = 2;
			}
			for (int i = a_X - radius; i <= a_X + radius; i++) {
				for (int k = a_Z-radius; k <= a_Z + radius; k++) {
					// small chance to be missing a block to add a little random
					if (k != a_Z || i != a_X && (r1.randInt() % 100 + 1) > 20) {
						if( GetBlock( i, a_Y + j, k ) == E_BLOCK_AIR )
							FastSetBlock(i, a_Y+j, k, E_BLOCK_LEAVES, 0 );
					}
					else {
						//if( m_BlockType[ MakeIndex(i, TopY+j, k) ] == E_BLOCK_AIR )
						//	m_BlockType[ MakeIndex(i, TopY+j, k) ] = E_BLOCK_LEAVES;
					}
				}
			}
			if( GetBlock( a_X, a_Y+j, a_Z ) == E_BLOCK_AIR )
				FastSetBlock( a_X, a_Y+j, a_Z, E_BLOCK_LOG, 0 );
		}
	}

	// do the top
	if( GetBlock( a_X+1, a_Y+trunk, a_Z ) == E_BLOCK_AIR )
		FastSetBlock( a_X+1, a_Y+trunk, a_Z, E_BLOCK_LEAVES, 0 );

	if( GetBlock( a_X-1, a_Y+trunk, a_Z ) == E_BLOCK_AIR )
		FastSetBlock( a_X-1, a_Y+trunk, a_Z, E_BLOCK_LEAVES, 0 );

	if( GetBlock( a_X, a_Y+trunk, a_Z+1 ) == E_BLOCK_AIR )
		FastSetBlock( a_X, a_Y+trunk, a_Z+1, E_BLOCK_LEAVES, 0 );

	if( GetBlock( a_X, a_Y+trunk, a_Z-1 ) == E_BLOCK_AIR )
		FastSetBlock( a_X, a_Y+trunk, a_Z-1, E_BLOCK_LEAVES, 0 );

	if( GetBlock( a_X, a_Y+trunk, a_Z ) == E_BLOCK_AIR )
		FastSetBlock( a_X, a_Y+trunk, a_Z, E_BLOCK_LEAVES, 0 );

	// end new tree code
}





void cWorld::UnloadUnusedChunks()
{
	m_LastUnload = m_Time;
	m_ChunkMap->UnloadUnusedChunks();
}





cChunkPtr cWorld::GetChunkOfBlock( int a_X, int a_Y, int a_Z )
{
	int ChunkX, ChunkY, ChunkZ;
	AbsoluteToRelative( a_X, a_Y, a_Z, ChunkX, ChunkY, ChunkZ );
	return GetChunk( ChunkX, ChunkY, ChunkZ );
}





void cWorld::SetBlock( int a_X, int a_Y, int a_Z, char a_BlockType, char a_BlockMeta )
{
	int ChunkX, ChunkY, ChunkZ, X = a_X, Y = a_Y, Z = a_Z;
	AbsoluteToRelative( X, Y, Z, ChunkX, ChunkY, ChunkZ );

	cChunkPtr Chunk = GetChunk( ChunkX, ChunkY, ChunkZ );
	if ( Chunk->IsValid() )
	{
		Chunk->SetBlock(X, Y, Z, a_BlockType, a_BlockMeta );
		this->GetSimulatorManager()->WakeUp(a_X, a_Y, a_Z);
	}
	// The chunk is not yet initialized, so it's probably far away from all players, no need to store this Meta change
}





void cWorld::FastSetBlock( int a_X, int a_Y, int a_Z, char a_BlockType, char a_BlockMeta )
{
	int ChunkX, ChunkY, ChunkZ, X = a_X, Y = a_Y, Z = a_Z;

	AbsoluteToRelative( X, Y, Z, ChunkX, ChunkY, ChunkZ );

	cChunkPtr Chunk = GetChunkNoGen( ChunkX, ChunkY, ChunkZ );
	if (Chunk->IsValid())	
	{
		Chunk->FastSetBlock(X, Y, Z, a_BlockType, a_BlockMeta );
		return;
	}

	// Unable to set block right now, try again later
	cCSLock Lock(m_CSFastSetBlock);
	m_FastSetBlockQueue.push_back( sSetBlockData( a_X, a_Y, a_Z, a_BlockType, a_BlockMeta ) ); 
}





char cWorld::GetBlock(int a_X, int a_Y, int a_Z)
{
	int ChunkX, ChunkY, ChunkZ;

	AbsoluteToRelative( a_X, a_Y, a_Z, ChunkX, ChunkY, ChunkZ );

	cChunkPtr Chunk = GetChunk( ChunkX, ChunkY, ChunkZ );
	if ( Chunk->IsValid() )
	{
		return Chunk->GetBlock(a_X, a_Y, a_Z);
	}
	return 0;
}





char cWorld::GetBlockMeta( int a_X, int a_Y, int a_Z )
{
	int ChunkX, ChunkY, ChunkZ;

	AbsoluteToRelative( a_X, a_Y, a_Z, ChunkX, ChunkY, ChunkZ );

	cChunkPtr Chunk = GetChunk( ChunkX, ChunkY, ChunkZ );
	if ( Chunk->IsValid() )
	{
		return Chunk->GetLight( Chunk->pGetMeta(), a_X, a_Y, a_Z );
	}
	return 0;
}





void cWorld::SetBlockMeta( int a_X, int a_Y, int a_Z, char a_MetaData )
{
	int ChunkX, ChunkY, ChunkZ;

	AbsoluteToRelative( a_X, a_Y, a_Z, ChunkX, ChunkY, ChunkZ );

	cChunkPtr Chunk = GetChunk( ChunkX, ChunkY, ChunkZ );
	if ( Chunk->IsValid() )	
	{
		Chunk->SetLight( Chunk->pGetMeta(), a_X, a_Y, a_Z, a_MetaData );
		Chunk->SendBlockTo( a_X, a_Y, a_Z, NULL );
	}
	// The chunk is not yet initialized, so it's probably far away from all players, no need to store this Meta change
}





bool cWorld::DigBlock( int a_X, int a_Y, int a_Z, cItem & a_PickupItem )
{
	int PosX = a_X, PosY = a_Y, PosZ = a_Z, ChunkX, ChunkY, ChunkZ;

	AbsoluteToRelative( PosX, PosY, PosZ, ChunkX, ChunkY, ChunkZ );

	cChunkPtr DestChunk = GetChunk( ChunkX, ChunkY, ChunkZ );
	if (DestChunk->IsValid())
	{
		DestChunk->SetBlock(PosX, PosY, PosZ, E_BLOCK_AIR, 0 );

		GetSimulatorManager()->WakeUp(a_X, a_Y, a_Z);

		if ( !a_PickupItem.IsEmpty() )
		{
			cPickup * Pickup = new cPickup( a_X * 32 + 16 + (int)fRadRand(16.f), a_Y * 32 + 16 + (int)fRadRand(16.f), a_Z * 32 + 16 + (int)fRadRand(16.f), a_PickupItem );
			Pickup->Initialize( this );
		}
	}
	return true;
}





void cWorld::SendBlockTo( int a_X, int a_Y, int a_Z, cPlayer * a_Player )
{
	int ChunkX, ChunkY, ChunkZ;
	AbsoluteToRelative( a_X, a_Y, a_Z, ChunkX, ChunkY, ChunkZ );
	cChunkPtr Chunk = GetChunk( ChunkX, ChunkY, ChunkZ );
	if (Chunk->IsValid())
	{
		Chunk->SendBlockTo( a_X, a_Y, a_Z, a_Player->GetClientHandle() );
	}
}





// TODO: This interface is dangerous!
cBlockEntity * cWorld::GetBlockEntity( int a_X, int a_Y, int a_Z )
{
	int PosX = a_X, PosY = a_Y, PosZ = a_Z, ChunkX, ChunkY, ChunkZ;

	AbsoluteToRelative( PosX, PosY, PosZ, ChunkX, ChunkY, ChunkZ );

	cChunkPtr Chunk = GetChunk( ChunkX, ChunkY, ChunkZ );
	if (Chunk->IsValid())
	{
		// TODO: return Chunk->GetBlockEntity( a_X, a_Y, a_Z );
	}
	return NULL;
}





char cWorld::GetHeight( int a_X, int a_Z )
{
	int PosX = a_X, PosY = 0, PosZ = a_Z, ChunkX, ChunkY, ChunkZ;
	AbsoluteToRelative( PosX, PosY, PosZ, ChunkX, ChunkY, ChunkZ );
	cChunkPtr Chunk = GetChunk( ChunkX, ChunkY, ChunkZ );
	if ( Chunk->IsValid())
	{
		return Chunk->GetHeight( PosX, PosZ );
	}
	return 0;
}





const double & cWorld::GetSpawnY(void)
{
	m_SpawnY = (double)GetHeight( (int)m_SpawnX, (int)m_SpawnZ ) + 1.6f; // +1.6f eye height
	return m_SpawnY;
}




void cWorld::Broadcast( const cPacket & a_Packet, cClientHandle* a_Exclude)
{
	cCSLock Lock(m_CSPlayers);
	for (cPlayerList::iterator itr = m_Players.begin(); itr != m_Players.end(); ++itr)
	{
		if (((*itr)->GetClientHandle() == a_Exclude) || !(*itr)->GetClientHandle()->IsLoggedIn() )
		{
			continue;
		}
		(*itr)->GetClientHandle()->Send( a_Packet );
	}
}





void cWorld::BroadcastToChunkOfBlock(int a_X, int a_Y, int a_Z, cPacket * a_Packet, cClientHandle * a_Exclude)
{
	m_ChunkMap->BroadcastToChunkOfBlock(a_X, a_Y, a_Z, a_Packet, a_Exclude);
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





void cWorld::ChunkDataLoaded(int a_ChunkX, int a_ChunkY, int a_ChunkZ, const char * a_BlockData, cEntityList & a_Entities, cBlockEntityList & a_BlockEntities)
{
	m_ChunkMap->ChunkDataLoaded(a_ChunkX, a_ChunkY, a_ChunkZ, a_BlockData, a_Entities, a_BlockEntities);
}





void cWorld::SetChunkData(int a_ChunkX, int a_ChunkY, int a_ChunkZ, const char * a_BlockData, cEntityList & a_Entities, cBlockEntityList & a_BlockEntities)
{
	m_ChunkMap->SetChunkData(a_ChunkX, a_ChunkY, a_ChunkZ, a_BlockData, a_Entities, a_BlockEntities);
}





void cWorld::GetChunkData(int a_ChunkX, int a_ChunkY, int a_ChunkZ, cChunkDataCallback * a_Callback)
{
	m_ChunkMap->GetChunkData(a_ChunkX, a_ChunkY, a_ChunkZ, a_Callback);
}





bool cWorld::IsChunkValid(int a_ChunkX, int a_ChunkY, int a_ChunkZ) const
{
	return m_ChunkMap->IsChunkValid(a_ChunkX, a_ChunkY, a_ChunkZ);
}





bool cWorld::HasChunkAnyClients(int a_ChunkX, int a_ChunkY, int a_ChunkZ) const
{
	return m_ChunkMap->HasChunkAnyClients(a_ChunkX, a_ChunkY, a_ChunkZ);
}





void cWorld::SetMaxPlayers(int iMax)
{
	m_MaxPlayers = MAX_PLAYERS;
	if (iMax > 0 && iMax < MAX_PLAYERS)
	{
		m_MaxPlayers = iMax;
	}
}





void cWorld::AddPlayer( cPlayer* a_Player )
{
	cCSLock Lock(m_CSPlayers);
	m_Players.remove( a_Player );  // Make sure the player is registered only once
	m_Players.push_back( a_Player );
}





void cWorld::RemovePlayer( cPlayer* a_Player )
{
	cCSLock Lock(m_CSPlayers);
	m_Players.remove( a_Player );
}





bool cWorld::ForEachPlayer(cPlayerListCallback * a_Callback)
{
	// Calls the callback for each player in the list
	cCSLock Lock(m_CSPlayers);
	for (cPlayerList::iterator itr = m_Players.begin(); itr != m_Players.end(); ++itr)
	{
		if (a_Callback->Item(*itr))
		{
			return false;
		}
	}  // for itr - m_Players[]
	return true;
}




// TODO: This interface is dangerous!
cPlayer* cWorld::GetPlayer( const char* a_PlayerName )
{
	cPlayer* BestMatch = 0;
	unsigned int MatchedLetters = 0;
	unsigned int NumMatches = 0;
	bool bPerfectMatch = false;

	unsigned int NameLength = strlen( a_PlayerName );
	cCSLock Lock(m_CSPlayers);
	for (cPlayerList::iterator itr = m_Players.begin(); itr != m_Players.end(); itr++ )
	{
		std::string Name = (*itr)->GetName();
		if( NameLength > Name.length() ) continue; // Definitely not a match

		for (unsigned int i = 0; i < NameLength; i++)
		{
			char c1 = (char)toupper( a_PlayerName[i] );
			char c2 = (char)toupper( Name[i] );
			if( c1 == c2 )
			{
				if( i+1 > MatchedLetters )
				{
					MatchedLetters = i+1;
					BestMatch = *itr;
				}
				if( i+1 == NameLength )
				{
					NumMatches++;
					if( NameLength == Name.length() )
					{
						bPerfectMatch = true;
						break;
					}
				}
			}
			else
			{
				if( BestMatch == *itr ) BestMatch = 0;
				break;
			}
			if( bPerfectMatch )
				break;
		}
	}
	if ( NumMatches == 1 )
	{
		return BestMatch;
	}

	// More than one matches, so it's undefined. Return NULL instead
	return NULL;
}





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
	for ( cPlayerList::iterator itr = m_Players.begin(); itr != m_Players.end(); ++itr)
	{
		if (((*itr)->GetClientHandle() != NULL) && !((*itr)->GetClientHandle()->IsDestroyed()))
		{
			cPacket_PlayerListItem PlayerListItem((*itr)->GetColor() + (*itr)->GetName(), true, (*itr)->GetClientHandle()->GetPing());
			a_DestPlayer->GetClientHandle()->Send( PlayerListItem );
		}
	}
}





bool cWorld::DoWithEntity( int a_UniqueID, cEntityCallback & a_Callback )
{
	cCSLock Lock(m_CSEntities);
	for (cEntityList::iterator itr = m_AllEntities.begin(); itr != m_AllEntities.end(); ++itr )
	{
		if( (*itr)->GetUniqueID() == a_UniqueID )
		{
			return a_Callback.Item(*itr);
		}
	} // for itr - m_AllEntities[]
	return false;
}





void cWorld::RemoveEntityFromChunk(cEntity * a_Entity)
{
	cChunkPtr Chunk = GetChunkOfBlock((int)(a_Entity->GetPosX()), (int)(a_Entity->GetPosY()), (int)(a_Entity->GetPosZ()));
	Chunk->RemoveEntity(a_Entity);
}





void cWorld::SaveAllChunks()
{
	LOG("Saving all chunks...");
	m_LastSave = m_Time;
	m_ChunkMap->SaveAllChunks();
	LOG("Done saving chunks");
}





void cWorld::ReSpreadLighting( const cChunkPtr & a_Chunk )
{
	cCSLock Lock(m_CSLighting);
	m_SpreadQueue.remove( a_Chunk ); 
	m_SpreadQueue.push_back( a_Chunk );
}





void cWorld::RemoveSpread( const cChunkPtr & a_Chunk )
{
	cCSLock Lock(m_CSLighting);
	m_SpreadQueue.remove( a_Chunk );
}





/************************************************************************/
/* Get and set                                                          */
/************************************************************************/
// void cWorld::AddClient( cClientHandle* a_Client )
// {
// 	m_m_Clients.push_back( a_Client );
// }
// cWorld::ClientList & cWorld::GetClients()
// {
// 	return m_m_Clients;
// }





void cWorld::AddEntity( cEntity* a_Entity )
{
	cCSLock Lock(m_CSEntities);
	m_AllEntities.push_back( a_Entity ); 
}





unsigned int cWorld::GetNumPlayers()
{
	cCSLock Lock(m_CSPlayers);
	return m_Players.size(); 
}





int cWorld::GetNumChunks(void) const
{
	return m_ChunkMap->GetNumChunks();
}




