#include "BlockID.h"
#include "cWorld.h"
#include "cRedstone.h"
#include "cChunk.h"
#include "cClientHandle.h"
#include "cPickup.h"
#include "cBlockToPickup.h"
#include "cMCLogger.h"
#include "cPlayer.h"
#include "cServer.h"
#include "cCriticalSection.h"
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


#include "packets/cPacket_TimeUpdate.h"
#include "packets/cPacket_NewInvalidState.h"
#include "packets/cPacket_Thunderbolt.h"

#include "Vector3d.h"

#include <time.h>

#include "tolua++.h"

#ifndef _WIN32
#include <stdlib.h>
//#include <sys/stat.h>   // for mkdir
#include <sys/types.h>
#endif



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

struct sSetBlockData
{
	sSetBlockData( int a_X, int a_Y, int a_Z, char a_BlockID, char a_BlockMeta )
		: x( a_X )
		, y( a_Y )
		, z( a_Z )
		, BlockID( a_BlockID )
		, BlockMeta( a_BlockMeta )
	{}
	int x, y, z;
	char BlockID, BlockMeta;
};

typedef std::list< sSetBlockData > FastSetBlockList;

struct cWorld::sWorldState
{
	cWorld::EntityList RemoveEntityQueue;
	cWorld::EntityList AllEntities;
	cWorld::ClientList Clients;
	cWorld::PlayerList Players;

	cWorld::ChunkList SpreadQueue;

	FastSetBlockList FastSetBlockQueue;

	cChunkGenerator* pChunkGenerator;

	std::string WorldName;
};

cWorld* cWorld::GetWorld()
{
	LOGWARN("WARNING: Using deprecated function cWorld::GetWorld() use cRoot::Get()->GetWorld() instead!");
	return cRoot::Get()->GetWorld();
}

cWorld::~cWorld()
{
	LockEntities();
	while( m_pState->AllEntities.begin() != m_pState->AllEntities.end() )
	{
		cEntity* Entity = *m_pState->AllEntities.begin();
		m_pState->AllEntities.remove( Entity );
		if( !Entity->IsDestroyed() ) Entity->Destroy();
		RemoveEntity( Entity );
	}
	UnlockEntities();

	delete m_SimulatorManager;
	delete m_SandSimulator;
	delete m_WaterSimulator;
	delete m_LavaSimulator;
	delete m_FireSimulator;

	UnloadUnusedChunks();
	delete m_pState->pChunkGenerator;
	delete m_ChunkMap;

	delete m_ClientHandleCriticalSection;	m_ClientHandleCriticalSection = 0;
	delete m_EntitiesCriticalSection;		m_EntitiesCriticalSection = 0;
	delete m_ChunksCriticalSection;			m_ChunksCriticalSection = 0;
	delete m_pState;

	delete m_WorldGenerator;
}

cWorld::cWorld( const char* a_WorldName )
	: m_pState( new sWorldState )
	, m_SpawnMonsterTime( 0.f )
	, m_RSList ( 0 )
	, m_Weather ( 0 )
	, m_WorldGenerator( 0 )
{
	LOG("cWorld::cWorld(%s)", a_WorldName);
	m_pState->WorldName = a_WorldName;

	cMakeDir::MakeDir(m_pState->WorldName.c_str());

	MTRand r1;
	m_SpawnX = (double)((r1.randInt()%1000)-500);
	m_SpawnY = 128;
	m_SpawnZ = (double)((r1.randInt()%1000)-500);
	m_WorldSeed = r1.randInt();
	m_GameMode = 0;

	std::string WorldGeneratorName;

	cIniFile IniFile( m_pState->WorldName + "/world.ini");
	if( IniFile.ReadFile() )
	{
		m_SpawnX = IniFile.GetValueF("SpawnPosition", "X", m_SpawnX );
		m_SpawnY = IniFile.GetValueF("SpawnPosition", "Y", m_SpawnY );
		m_SpawnZ = IniFile.GetValueF("SpawnPosition", "Z", m_SpawnZ );
		m_WorldSeed = IniFile.GetValueI("Seed", "Seed", m_WorldSeed );
		m_GameMode = IniFile.GetValueI("GameMode", "GameMode", m_GameMode );
		WorldGeneratorName = IniFile.GetValue("Generator", "GeneratorName", "Default");
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
			LOG("WARNING: Could not write to %s/world.ini", a_WorldName);
		}
	}
	LOGINFO("Seed: %i", m_WorldSeed );

	if( WorldGeneratorName.compare("Test") == 0 )
		m_WorldGenerator = new cWorldGenerator_Test();
	else	// Default
		m_WorldGenerator = new cWorldGenerator();

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

	m_ChunkMap = new cChunkMap( 32, 32, this );
	m_pState->pChunkGenerator = new cChunkGenerator( m_ChunkMap );

	m_Time = 0;
	m_WorldTimeFraction = 0.f;
	m_WorldTime = 0;
	m_LastSave = 0;
	m_LastUnload = 0;
	m_ClientHandleCriticalSection = new cCriticalSection();
	m_EntitiesCriticalSection = new cCriticalSection();
	m_ChunksCriticalSection = new cCriticalSection();

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

void cWorld::CastThunderbolt ( int X, int Y, int Z ) {
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
	int ViewDist = cClientHandle::VIEWDISTANCE;
	LOG("Loading spawn area");
	for(int x = 0; x < ViewDist; x++)
	{
		for(int z = 0; z < ViewDist; z++)
		{
			GetChunk( x + ChunkX-(ViewDist-1)/2, 0, z + ChunkZ-(ViewDist-1)/2 );
		}
		LOG("Loaded %0.2f", ((float)x / (float)ViewDist)*100 );
	}
}

void cWorld::Tick(float a_Dt)
{
	int randWeather = 0;
	m_Time+=a_Dt/1000.f;

	CurrentTick++;

	bool bSendTime = false;
	m_WorldTimeFraction+=a_Dt/1000.f;
	while( m_WorldTimeFraction > 1.f )
	{
		m_WorldTimeFraction-=1.f;
		m_WorldTime+=20;
		m_WorldTime %= 24000; // 24000 units in a day
		bSendTime = true;
	}
	if( bSendTime ) Broadcast( cPacket_TimeUpdate( (m_WorldTime) ) );

	LockEntities();
	for( cWorld::EntityList::iterator itr = GetEntities().begin(); itr != GetEntities().end();)
	{
		if( (*itr)->IsDestroyed() )
		{
			LOG("Destroy that entity! %i", (*itr)->GetUniqueID() );
			cEntity* RemoveMe = *itr;
			itr = m_pState->AllEntities.erase( itr );
			m_pState->RemoveEntityQueue.push_back( RemoveMe ); 
			continue;
		}
		(*itr)->Tick(a_Dt);
		itr++;
	}
	UnlockEntities();

	LockChunks();


	int TimesSpreaded = 0;
	while( !m_pState->SpreadQueue.empty() && TimesSpreaded < 50 ) // Spread a max of 50 times each tick, otherwise server will hang
	{
		cChunk* Chunk = (*m_pState->SpreadQueue.begin());
		//LOG("Spreading: %p", Chunk );
		Chunk->SpreadLight( Chunk->pGetSkyLight() );
		Chunk->SpreadLight( Chunk->pGetLight() );
		m_pState->SpreadQueue.remove( Chunk );
		Chunk->RemoveReference();
		TimesSpreaded++;
	}
	if( TimesSpreaded >= 50 )
	{
		LOGWARN("Lots of lighting to do! At least %i chunks left!", m_pState->SpreadQueue.size() );
	}

	m_ChunkMap->Tick(a_Dt);
	
	GetSimulatorManager()->Simulate(a_Dt);

	UnlockChunks();

	MTRand r1;

////////////////Weather///////////////////////
	if ( GetWeather() == 0 ) { //if sunny
		if( CurrentTick % 19 == 0 ) { //every 20 ticks random weather
			randWeather = (r1.randInt() %10000);
			if (randWeather == 0) {
				LOG("Starting Rainstorm!");
				SetWeather ( 1 );
			} else if  (randWeather == 1) {
				LOG("Starting Thunderstorm!");
				SetWeather ( 2 );
			}
		}
	}

	if ( GetWeather() != 0 ) { //if raining or thunderstorm
		if( CurrentTick % 19 == 0 ) { //every 20 ticks random weather
			randWeather = (r1.randInt() %4999);
			if (randWeather == 0) { //2% chance per second
				LOG("Back to sunny!");
				SetWeather ( 0 );
			} else if ( (randWeather > 4000) && (GetWeather() != 2) ) { //random chance for rainstorm to turn into thunderstorm.
				LOG("Starting Thunderstorm!");
				SetWeather ( 2 );
			}
		}
	}

	if ( GetWeather() == 2 ) { //if thunderstorm
		if (r1.randInt() %199 == 0) { //0.5% chance per tick of thunderbolt
			CastThunderbolt ( 0, 0, 0 ); //todo: find random possitions near players to cast thunderbolts.
		}
	}
////////////////Weather///////////////////////

	// Asynchronously set blocks
	FastSetBlockList FastSetBlockQueueCopy = m_pState->FastSetBlockQueue;
	m_pState->FastSetBlockQueue.clear();
	for( FastSetBlockList::iterator itr = FastSetBlockQueueCopy.begin(); itr != FastSetBlockQueueCopy.end(); ++itr )
	{
		sSetBlockData & SetBlockData = *itr;
		FastSetBlock( SetBlockData.x, SetBlockData.y, SetBlockData.z, SetBlockData.BlockID, SetBlockData.BlockMeta );	// If unable to set block, it's added to FastSetBlockQueue again
	}
	if( FastSetBlockQueueCopy.size() != m_pState->FastSetBlockQueue.size() )
	LOG(" Before: %i, after %i" , FastSetBlockQueueCopy.size(), m_pState->FastSetBlockQueue.size() );

	if( m_Time - m_LastSave > 60*5 ) // Save each 5 minutes
	{
		SaveAllChunks();
	}

	if( m_Time - m_LastUnload > 10 ) // Unload each minute
	{
		UnloadUnusedChunks();
	}

	while( !m_pState->RemoveEntityQueue.empty() )
	{
		RemoveEntity( *m_pState->RemoveEntityQueue.begin() );
	}

	if( m_bAnimals && ( m_Time - m_SpawnMonsterTime > m_SpawnMonsterRate ) ) // 10 seconds
	{
		m_SpawnMonsterTime = m_Time;
		if( m_pState->Players.size() > 0 )
		{
			cMonster	*Monster = 0;

			//srand ( time(NULL) );			// Only seed random ONCE! Is already done in the cWorld constructor
			int dayRand   = r1.randInt() % 6;  //added mob code
			int nightRand = r1.randInt() % 10; //added mob code

			int RandomPlayerIdx = r1.randInt() & m_pState->Players.size();
			PlayerList::iterator itr = m_pState->Players.begin();
			for( int i = 1; i < RandomPlayerIdx; i++ )
				itr++;

			cPlayer* Player = *itr;
			Vector3d SpawnPos = Player->GetPosition();
			SpawnPos += Vector3d( (double)(r1.randInt()%64)-32, (double)(r1.randInt()%64)-32, (double)(r1.randInt()%64)-32 );
			char Height = GetHeight( (int)SpawnPos.x, (int)SpawnPos.z );

			if(m_WorldTime >= 12000 + 1000) {
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
			} else {
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
				Monster->TeleportTo( SpawnPos.x, (double)(Height)+2, SpawnPos.z );
				Monster->SpawnOn( 0 );
			}
		}
	}



	std::vector<int> m_RSList_copy(m_RSList);
	//copy(m_RSList.begin(), m_RSList.end(), m_RSList_copy.begin());
	m_RSList.erase(m_RSList.begin(),m_RSList.end());
	int tempX;	// FIXME - Keep the scope in mind, these variables are not used in this scope at all, move them down into the for loop
	int tempY;
	int tempZ;
	int state;

	std::vector<int>::const_iterator cii;	// FIXME - Please rename this variable, WTF is cii??? Use human readable variable names or common abbreviations (i, idx, itr, iter)
	for(cii=m_RSList_copy.begin(); cii!=m_RSList_copy.end();)
	{
		tempX = *cii;cii++;
		tempY = *cii;cii++;
		tempZ = *cii;cii++;
		state = *cii;cii++;
		
		//printf ("%i, %i, %i, %i\n",tempX,tempY,tempZ,state) ;
		if ( (state == 11111) && ( (int)GetBlock( tempX, tempY, tempZ ) == E_BLOCK_REDSTONE_TORCH_OFF ) ) {
			FastSetBlock( tempX, tempY, tempZ, E_BLOCK_REDSTONE_TORCH_ON, (int)GetBlockMeta( tempX, tempY, tempZ ) );
			cRedstone Redstone(this);
			Redstone.ChangeRedstone( tempX, tempY, tempZ, true );
		} else if ( (state == 00000) && ( (int)GetBlock( tempX, tempY, tempZ ) == E_BLOCK_REDSTONE_TORCH_ON ) ) {
			FastSetBlock( tempX, tempY, tempZ, E_BLOCK_REDSTONE_TORCH_OFF, (int)GetBlockMeta( tempX, tempY, tempZ ) );
			cRedstone Redstone(this);
			Redstone.ChangeRedstone( tempX, tempY, tempZ, false );
		}

	}
	m_RSList_copy.erase(m_RSList_copy.begin(),m_RSList_copy.end());

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

	LockChunks();
	LOGINFO("Unloading unused chunks");
	m_ChunkMap->UnloadUnusedChunks();
	UnlockChunks();
}

cChunk* cWorld::GetChunkReliable( int a_X, int a_Y, int a_Z )	// TODO - FIXME - WARNING - This can cause a duplicate chunk to be generated!!
{
	cChunk* Chunk = GetChunkUnreliable( a_X, a_Y, a_Z );
	if( Chunk )
	{
		return Chunk;
	}

	// Found nothing, create a chunk
	Chunk = new cChunk( a_X, a_Y, a_Z, this );
	if(Chunk)
	{
		LOGWARN("Created new chunk! %i %i", a_X, a_Z);
		LockChunks();
		m_ChunkMap->AddChunk( Chunk );
		UnlockChunks();
		Chunk->Initialize();
		return Chunk;
	}

	// This should never happen since it's reliable, but yeah
	return 0;
}

cChunk* cWorld::GetChunk( int a_X, int a_Y, int a_Z )
{
	// Get chunk from memory
	cChunk* Chunk = GetChunkUnreliable( a_X, a_Y, a_Z );
	if( Chunk )	return Chunk;

	// Generate new chunk asynchronously
	m_pState->pChunkGenerator->GenerateChunk( a_X, a_Z );

	// Could not find chunk, it's being generated, so return 0
	return 0;
}

cChunk* cWorld::GetChunkUnreliable( int a_X, int a_Y, int a_Z )
{
	LockChunks();
	cChunk* Chunk = m_ChunkMap->GetChunk( a_X, a_Y, a_Z );
	UnlockChunks();
	if( Chunk ) return Chunk;
	return 0;
}

cChunk* cWorld::GetChunkOfBlock( int a_X, int a_Y, int a_Z )
{
	int ChunkX, ChunkY, ChunkZ;
	AbsoluteToRelative( a_X, a_Y, a_Z, ChunkX, ChunkY, ChunkZ );
	return GetChunk( ChunkX, ChunkY, ChunkZ );
}

void cWorld::SetBlock( int a_X, int a_Y, int a_Z, char a_BlockType, char a_BlockMeta )
{
	int ChunkX, ChunkY, ChunkZ, X = a_X, Y = a_Y, Z = a_Z;
	AbsoluteToRelative( X, Y, Z, ChunkX, ChunkY, ChunkZ );

	cChunk* Chunk = GetChunk( ChunkX, ChunkY, ChunkZ );
	if( Chunk )
	{
		Chunk->SetBlock(X, Y, Z, a_BlockType, a_BlockMeta );
		this->GetSimulatorManager()->WakeUp(a_X, a_Y, a_Z);
	}
}

void cWorld::FastSetBlock( int a_X, int a_Y, int a_Z, char a_BlockType, char a_BlockMeta )
{
	int ChunkX, ChunkY, ChunkZ, X = a_X, Y = a_Y, Z = a_Z;

	AbsoluteToRelative( X, Y, Z, ChunkX, ChunkY, ChunkZ );

	cChunk* Chunk = GetChunk( ChunkX, ChunkY, ChunkZ );
	if( Chunk )	
	{
		Chunk->FastSetBlock(X, Y, Z, a_BlockType, a_BlockMeta );
		return;
	}

	// Could not find chunk, so it has been pushed into the generate chunks queue
	// Check if currently generating the target chunk
	m_pState->pChunkGenerator->Lock();
	Chunk = m_pState->pChunkGenerator->GetCurrentlyGenerating();
	if( Chunk && Chunk->GetPosX() == ChunkX && Chunk->GetPosZ() == ChunkZ )
	{
		Chunk->FastSetBlock(X, Y, Z, a_BlockType, a_BlockMeta );
		m_pState->pChunkGenerator->Unlock();
		return;
	}
	m_pState->pChunkGenerator->Unlock();

	// Unable to set block right now, try again later
	m_pState->FastSetBlockQueue.push_back( sSetBlockData( a_X, a_Y, a_Z, a_BlockType, a_BlockMeta ) ); 
}

char cWorld::GetBlock( int a_X, int a_Y, int a_Z )
{
	int ChunkX, ChunkY, ChunkZ;

	AbsoluteToRelative( a_X, a_Y, a_Z, ChunkX, ChunkY, ChunkZ );

	cChunk* Chunk = GetChunk( ChunkX, ChunkY, ChunkZ );
	if( Chunk )	return Chunk->GetBlock(a_X, a_Y, a_Z);
	return 0;
}

char cWorld::GetBlockMeta( int a_X, int a_Y, int a_Z )
{
	int ChunkX, ChunkY, ChunkZ;

	AbsoluteToRelative( a_X, a_Y, a_Z, ChunkX, ChunkY, ChunkZ );

	cChunk* Chunk = GetChunk( ChunkX, ChunkY, ChunkZ );
	if( Chunk )	return Chunk->GetLight( Chunk->pGetMeta(), a_X, a_Y, a_Z );
	return 0;
}

void cWorld::SetBlockMeta( int a_X, int a_Y, int a_Z, char a_MetaData )
{
	int ChunkX, ChunkY, ChunkZ;

	AbsoluteToRelative( a_X, a_Y, a_Z, ChunkX, ChunkY, ChunkZ );

	cChunk* Chunk = GetChunk( ChunkX, ChunkY, ChunkZ );
	if( Chunk )	
	{
		Chunk->SetLight( Chunk->pGetMeta(), a_X, a_Y, a_Z, a_MetaData );
		Chunk->SendBlockTo( a_X, a_Y, a_Z, 0 );
	}
}

bool cWorld::DigBlock( int a_X, int a_Y, int a_Z, cItem & a_PickupItem )
{
	int PosX = a_X, PosY = a_Y, PosZ = a_Z, ChunkX, ChunkY, ChunkZ;

	AbsoluteToRelative( PosX, PosY, PosZ, ChunkX, ChunkY, ChunkZ );

	cChunk* DestChunk = GetChunk( ChunkX, ChunkY, ChunkZ );
	if(DestChunk)
	{
		DestChunk->SetBlock(PosX, PosY, PosZ, E_BLOCK_AIR, 0 );

		GetSimulatorManager()->WakeUp(a_X, a_Y, a_Z);

		if( !a_PickupItem.IsEmpty() )
		{
			cPickup* Pickup = new cPickup( a_X*32 + 16 + (int)fRadRand(16.f), a_Y*32 + 16 + (int)fRadRand(16.f), a_Z*32 + 16 + (int)fRadRand(16.f), a_PickupItem );
			Pickup->Initialize( this );
		}
	}

	return true;
}

void cWorld::SendBlockTo( int a_X, int a_Y, int a_Z, cPlayer* a_Player )
{
	int ChunkX, ChunkY, ChunkZ;
	AbsoluteToRelative( a_X, a_Y, a_Z, ChunkX, ChunkY, ChunkZ );
	cChunk* Chunk = GetChunk( ChunkX, ChunkY, ChunkZ );
	Chunk->SendBlockTo( a_X, a_Y, a_Z, a_Player->GetClientHandle() );
}

cBlockEntity* cWorld::GetBlockEntity( int a_X, int a_Y, int a_Z )
{
	int PosX = a_X, PosY = a_Y, PosZ = a_Z, ChunkX, ChunkY, ChunkZ;

	AbsoluteToRelative( PosX, PosY, PosZ, ChunkX, ChunkY, ChunkZ );

	cChunk* Chunk = GetChunk( ChunkX, ChunkY, ChunkZ );
	if( !Chunk ) return 0;

	return Chunk->GetBlockEntity( a_X, a_Y, a_Z );
}

char cWorld::GetHeight( int a_X, int a_Z )
{
	int PosX = a_X, PosY = 0, PosZ = a_Z, ChunkX, ChunkY, ChunkZ;
	AbsoluteToRelative( PosX, PosY, PosZ, ChunkX, ChunkY, ChunkZ );
	cChunk* Chunk = GetChunk( ChunkX, ChunkY, ChunkZ );
	if( Chunk )	return Chunk->GetHeight( PosX, PosZ );
	return 0;
}

const double & cWorld::GetSpawnY()
{
	m_SpawnY = (double)GetHeight( (int)m_SpawnX, (int)m_SpawnZ ) + 1.6f; // +1.6f eye height
	return m_SpawnY;
}

void cWorld::Broadcast( const cPacket & a_Packet, cClientHandle* a_Exclude /* = 0 */ )
{
	for( PlayerList::iterator itr = m_pState->Players.begin(); itr != m_pState->Players.end(); ++itr)
	{
		if( (*itr)->GetClientHandle() == a_Exclude || !(*itr)->GetClientHandle()->IsLoggedIn() ) continue;
		(*itr)->GetClientHandle()->Send( a_Packet );
	}
}

std::string cWorld::GetDescription()
{
	return this->m_Description;
}

unsigned int cWorld::GetMaxPlayers()
{
	return this->m_MaxPlayers;
}

void cWorld::SetMaxPlayers(int iMax)
{
	this->m_MaxPlayers = MAX_PLAYERS;
	if (iMax > 0 && iMax < MAX_PLAYERS)
	{
		this->m_MaxPlayers = iMax;
	}
}

void cWorld::AddPlayer( cPlayer* a_Player )
{
	m_pState->Players.remove( a_Player );
	m_pState->Players.push_back( a_Player );
}

void cWorld::RemovePlayer( cPlayer* a_Player )
{
	m_pState->Players.remove( a_Player );
}

void cWorld::GetAllPlayers( lua_State* L )
{
	lua_createtable(L, m_pState->Players.size(), 0);
	int newTable = lua_gettop(L);
	int index = 1;
	PlayerList::const_iterator iter = m_pState->Players.begin();
	while(iter != m_pState->Players.end()) {
		tolua_pushusertype( L, (*iter), "cPlayer" );
		lua_rawseti(L, newTable, index);
		++iter;
		++index;
	}
}

cPlayer* cWorld::GetPlayer( const char* a_PlayerName )
{
	cPlayer* BestMatch = 0;
	unsigned int MatchedLetters = 0;
	unsigned int NumMatches = 0;
	bool bPerfectMatch = false;

	unsigned int NameLength = strlen( a_PlayerName );
	for( PlayerList::iterator itr = m_pState->Players.begin(); itr != m_pState->Players.end(); itr++ )
	{
		std::string Name = (*itr)->GetName();
		if( NameLength > Name.length() ) continue; // Definitely not a match

		for(unsigned int i = 0; i < NameLength; i++)
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
	if( NumMatches == 1 )
		return BestMatch;

	// More than one matches, so it's undefined. Return 0 instead
	return 0;
}

cEntity* cWorld::GetEntity( int a_UniqueID )
{
	for( EntityList::iterator itr = m_pState->AllEntities.begin(); itr != m_pState->AllEntities.end(); ++itr )
	{
		if( (*itr)->GetUniqueID() == a_UniqueID )
			return *itr;
	}
	return 0;
}

// void cWorld::RemoveClient( cClientHandle* a_Client )
// {
// 	m_pState->m_Clients.remove( a_Client );
// 	if( a_Client )
// 	{
// 		delete a_Client;
// 		a_Client = 0;
// 	}
// }

void cWorld::RemoveEntity( cEntity* a_Entity )
{
	m_pState->RemoveEntityQueue.remove( a_Entity );
	if( a_Entity )
	{
		delete a_Entity;
		a_Entity = 0;
	}
}

bool cWorld::RemoveEntityFromChunk( cEntity & a_Entity, cChunk* a_CalledFrom /* = 0 */ )
{
	LockChunks();
	bool retVal = m_ChunkMap->RemoveEntityFromChunk( a_Entity, a_CalledFrom );
	UnlockChunks();
	return retVal;
}

void cWorld::SaveAllChunks()
{
	LOG("Saving all chunks...");
	m_LastSave = m_Time;
	LockChunks();
	m_ChunkMap->SaveAllChunks();
	UnlockChunks();
	LOG("Done saving chunks");
}

void cWorld::LockClientHandle()
{
	m_ClientHandleCriticalSection->Lock();
}

void cWorld::UnlockClientHandle()
{
	m_ClientHandleCriticalSection->Unlock();
}

void cWorld::LockEntities()
{
	m_EntitiesCriticalSection->Lock();
}

void cWorld::UnlockEntities()
{
	m_EntitiesCriticalSection->Unlock();
}

void cWorld::LockChunks()
{
	m_ChunksCriticalSection->Lock();
}

void cWorld::UnlockChunks()
{
	m_ChunksCriticalSection->Unlock();
}

void cWorld::ReSpreadLighting( cChunk* a_Chunk )
{
	LockChunks();
	m_pState->SpreadQueue.remove( a_Chunk );
	m_pState->SpreadQueue.push_back( a_Chunk );
#define STRINGIZE(x) #x
	a_Chunk->AddReference( __FILE__ ": " STRINGIZE(__LINE__) );
	UnlockChunks();
}

void cWorld::RemoveSpread( cChunk* a_Chunk )
{
	LockChunks();
	size_t SizeBefore = m_pState->SpreadQueue.size();
	m_pState->SpreadQueue.remove( a_Chunk );
	if( SizeBefore != m_pState->SpreadQueue.size() )
		a_Chunk->RemoveReference();
	UnlockChunks();
}


/************************************************************************/
/* Get and set                                                          */
/************************************************************************/
// void cWorld::AddClient( cClientHandle* a_Client )
// {
// 	m_pState->m_Clients.push_back( a_Client );
// }
// cWorld::ClientList & cWorld::GetClients()
// {
// 	return m_pState->m_Clients;
// }
cWorld::EntityList & cWorld::GetEntities()
{
	return m_pState->AllEntities;
}
void cWorld::AddEntity( cEntity* a_Entity )
{
	m_pState->AllEntities.push_back( a_Entity ); 
}
cWorld::PlayerList & cWorld::GetAllPlayers()
{
	return m_pState->Players; 
}
unsigned int cWorld::GetNumPlayers()
{
	return m_pState->Players.size(); 
}
void cWorld::AddToRemoveEntityQueue( cEntity & a_Entity )
{
	m_pState->AllEntities.remove( &a_Entity);
	m_pState->RemoveEntityQueue.push_back( &a_Entity ); 
}
const char* cWorld::GetName()
{
	return m_pState->WorldName.c_str();
}
int cWorld::GetNumChunks()
{
	LockChunks();
	int NumChunks = m_ChunkMap->GetNumChunks();
	UnlockChunks();
	return NumChunks;
}