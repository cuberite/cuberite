#include "BlockID.h"
#include "cWorld.h"
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
#include "cWaterSimulator.h"
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


#include "packets/cPacket_TimeUpdate.h"

#include "Vector3d.h"

#include <time.h>

#include "tolua++.h"

#ifndef _WIN32
#include <stdlib.h>
#include <sys/stat.h>   // for mkdir
#include <sys/types.h>
#endif

float cWorld::m_Time = 0.f;

char g_BlockLightValue[128];
char g_BlockSpreadLightFalloff[128];
bool g_BlockTransparent[128];
bool g_BlockOneHitDig[128];

#define RECI_RAND_MAX (1.f/RAND_MAX)
inline float fRadRand( float a_Radius )
{
	return ((float)rand() * RECI_RAND_MAX)*a_Radius - a_Radius*0.5f;
}

struct cWorld::sWorldState
{
	cWorld::EntityList m_RemoveEntityQueue;
	cWorld::EntityList m_AllEntities;
	cWorld::ClientList m_Clients;
	cWorld::PlayerList m_Players;

	static const unsigned int CHUNKBUFFER_SIZE = 5;
	std::vector< unsigned int > m_ChunkBuffer;

	cWorld::ChunkList m_SpreadQueue;
};

cWorld* cWorld::GetWorld()
{
	LOGWARN("WARNING: Using deprecated function cWorld::GetWorld() use cRoot::Get()->GetWorld() instead!");
	return cRoot::Get()->GetWorld();
}

cWorld::~cWorld()
{
	LockEntities();
	while( m_pState->m_AllEntities.begin() != m_pState->m_AllEntities.end() )
	{
		cEntity* Entity = *m_pState->m_AllEntities.begin();
		m_pState->m_AllEntities.remove( Entity );
		RemoveEntity( Entity );
	}
	UnlockEntities();

	delete m_WaterSimulator;

	UnloadUnusedChunks();
	delete m_ChunkMap;

	delete m_ClientHandleCriticalSection;	m_ClientHandleCriticalSection = 0;
	delete m_EntitiesCriticalSection;		m_EntitiesCriticalSection = 0;
	delete m_ChunksCriticalSection;			m_ChunksCriticalSection = 0;
	delete m_pState;
}

cWorld::cWorld()
	: m_pState( new sWorldState )
	, m_SpawnMonsterTime( 0.f )
{
	LOG("cWorld::cWorld()");

#ifdef _WIN32
	{
		SECURITY_ATTRIBUTES Attrib;
		Attrib.nLength = sizeof(SECURITY_ATTRIBUTES);
		Attrib.lpSecurityDescriptor = NULL;
		Attrib.bInheritHandle = false;
		::CreateDirectory("world", &Attrib);
	}
#else
	{
		mkdir("world", S_IRWXU | S_IRWXG | S_IRWXO);
	}
#endif

	srand( (unsigned int) time(0) );
	m_SpawnX = (double)((rand()%10000)-5000);
	m_SpawnY = 128;
	m_SpawnZ = (double)((rand()%10000)-5000);
	m_WorldSeed = rand();

	cIniFile IniFile("world/world.ini");
	if( IniFile.ReadFile() )
	{
		m_SpawnX = IniFile.GetValueF("SpawnPosition", "X", m_SpawnX );
		m_SpawnY = IniFile.GetValueF("SpawnPosition", "Y", m_SpawnY );
		m_SpawnZ = IniFile.GetValueF("SpawnPosition", "Z", m_SpawnZ );
		m_WorldSeed = IniFile.GetValueI("Seed", "Seed", m_WorldSeed );
	}
	else
	{
		IniFile.SetValueF("SpawnPosition", "X", m_SpawnX );
		IniFile.SetValueF("SpawnPosition", "Y", m_SpawnY );
		IniFile.SetValueF("SpawnPosition", "Z", m_SpawnZ );
		IniFile.SetValueI("Seed", "Seed", m_WorldSeed );
		if( !IniFile.WriteFile() )
		{
			LOG("WARNING: Could not write to world/world.ini");
		}
	}
	LOGINFO("Seed: %i", m_WorldSeed );

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
	}

	m_ChunkMap = new cChunkMap( 32, 32, this );

	m_Time = 0;
	m_WorldTimeFraction = 0.f;
	m_WorldTime = 0;
        m_GameMode = 1;
	m_LastSave = 0;
	m_LastUnload = 0;
	m_ClientHandleCriticalSection = new cCriticalSection();
	m_EntitiesCriticalSection = new cCriticalSection();
	m_ChunksCriticalSection = new cCriticalSection();

	m_WaterSimulator = new cWaterSimulator( this );

	memset( g_BlockLightValue, 0x0, 128 );
	memset( g_BlockSpreadLightFalloff, 0xf, 128 ); // 0xf means total falloff
	memset( g_BlockTransparent, 0x0, 128 );
	memset( g_BlockOneHitDig, 0x0, 128 );

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
	m_Time+=a_Dt/1000.f;

	bool bSendTime = false;
	m_WorldTimeFraction+=a_Dt/1000.f;
	while( m_WorldTimeFraction > 1.f )
	{
		m_WorldTimeFraction-=1.f;
		m_WorldTime+=20;
		m_WorldTime %= 24000; // 24000 units in a day
		bSendTime = true;
	}
	if( bSendTime ) cRoot::Get()->GetServer()->Broadcast( cPacket_TimeUpdate( (m_WorldTime) ) );

	LockEntities();
	for( cWorld::EntityList::iterator itr = GetEntities().begin(); itr != GetEntities().end();)
	{
		if( (*itr)->IsDestroyed() )
		{
			LOG("Destroy that entity! %i", (*itr)->GetUniqueID() );
			cEntity* RemoveMe = *itr;
			itr++;
			AddToRemoveEntityQueue( *RemoveMe );
			continue;
		}
		(*itr)->Tick(a_Dt);
		itr++;
	}
	UnlockEntities();

	LockChunks();

	while( !m_pState->m_SpreadQueue.empty() )
	{
		cChunk* Chunk = (*m_pState->m_SpreadQueue.begin());
		//LOG("Spreading: %p", Chunk );
		Chunk->SpreadLight( Chunk->pGetSkyLight() );
		Chunk->SpreadLight( Chunk->pGetLight() );
		m_pState->m_SpreadQueue.remove( &*Chunk );
	}

	m_ChunkMap->Tick(a_Dt);
	m_WaterSimulator->Simulate(a_Dt);
	UnlockChunks();

	if( m_Time - m_LastSave > 60*5 ) // Save each 5 minutes
	{
		SaveAllChunks();
	}

	if( m_Time - m_LastUnload > 10 ) // Unload each minute
	{
		UnloadUnusedChunks();
	}

	while( !m_pState->m_RemoveEntityQueue.empty() )
	{
		RemoveEntity( *m_pState->m_RemoveEntityQueue.begin() );
	}


	if( m_bAnimals && ( m_Time - m_SpawnMonsterTime > m_SpawnMonsterRate ) ) // 10 seconds
	{
		m_SpawnMonsterTime = m_Time;
		if( m_pState->m_Players.size() > 0 )
		{
			cMonster	*Monster = 0;

			//srand ( time(NULL) );			// Only seed random ONCE! Is already done in the cWorld constructor
			int dayRand   = rand() % 6;  //added mob code
			int nightRand = rand() % 10; //added mob code

			int RandomPlayerIdx = rand() & m_pState->m_Players.size();
			PlayerList::iterator itr = m_pState->m_Players.begin();
			for( int i = 1; i < RandomPlayerIdx; i++ )
				itr++;

			cPlayer* Player = *itr;
			Vector3d SpawnPos = Player->GetPosition();
			SpawnPos += Vector3d( (double)(rand()%64)-32, (double)(rand()%64)-32, (double)(rand()%64)-32 );
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
}

void cWorld::GrowTree( int a_X, int a_Y, int a_Z )
{
	// new tree code, looks much better
	// with help from seanj
	// converted from php to lua then lua to c++

	// build trunk
	int trunk = rand() % (7 - 5 + 1) + 5;
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
					if (k != a_Z || i != a_X && (rand() % 100 + 1) > 20) {
						
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
	m_ChunkMap->UnloadUnusedChunks();
	UnlockChunks();
}

cChunk* cWorld::GetChunk( int a_X, int a_Y, int a_Z )
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

	// This should never happen, but yeah
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
	m_WaterSimulator->WakeUp( a_X, a_Y, a_Z );

	int ChunkX, ChunkY, ChunkZ;
	AbsoluteToRelative( a_X, a_Y, a_Z, ChunkX, ChunkY, ChunkZ );

	GetChunk( ChunkX, ChunkY, ChunkZ )->SetBlock(a_X, a_Y, a_Z, a_BlockType, a_BlockMeta );
}

void cWorld::FastSetBlock( int a_X, int a_Y, int a_Z, char a_BlockType, char a_BlockMeta )
{
	int ChunkX, ChunkY, ChunkZ;

	AbsoluteToRelative( a_X, a_Y, a_Z, ChunkX, ChunkY, ChunkZ );

	GetChunk( ChunkX, ChunkY, ChunkZ )->FastSetBlock(a_X, a_Y, a_Z, a_BlockType, a_BlockMeta );
}

char cWorld::GetBlock( int a_X, int a_Y, int a_Z )
{
	int ChunkX, ChunkY, ChunkZ;

	AbsoluteToRelative( a_X, a_Y, a_Z, ChunkX, ChunkY, ChunkZ );

	return GetChunk( ChunkX, ChunkY, ChunkZ )->GetBlock(a_X, a_Y, a_Z);
}

char cWorld::GetBlockMeta( int a_X, int a_Y, int a_Z )
{
	int ChunkX, ChunkY, ChunkZ;

	AbsoluteToRelative( a_X, a_Y, a_Z, ChunkX, ChunkY, ChunkZ );

	cChunk* Chunk = GetChunk( ChunkX, ChunkY, ChunkZ );
	return Chunk->GetLight( Chunk->pGetMeta(), a_X, a_Y, a_Z );
}

void cWorld::SetBlockMeta( int a_X, int a_Y, int a_Z, char a_MetaData )
{
	int ChunkX, ChunkY, ChunkZ;

	AbsoluteToRelative( a_X, a_Y, a_Z, ChunkX, ChunkY, ChunkZ );

	cChunk* Chunk = GetChunk( ChunkX, ChunkY, ChunkZ );
	Chunk->SetLight( Chunk->pGetMeta(), a_X, a_Y, a_Z, a_MetaData );
	Chunk->SendBlockTo( a_X, a_Y, a_Z, 0 );
}

bool cWorld::DigBlock( int a_X, int a_Y, int a_Z, cItem & a_PickupItem )
{
	int PosX = a_X, PosY = a_Y, PosZ = a_Z, ChunkX, ChunkY, ChunkZ;

	AbsoluteToRelative( PosX, PosY, PosZ, ChunkX, ChunkY, ChunkZ );

	cChunk* DestChunk = GetChunk( ChunkX, ChunkY, ChunkZ );
	if(DestChunk)
	{
		DestChunk->SetBlock(PosX, PosY, PosZ, 0, 0 );
		m_WaterSimulator->WakeUp( a_X, a_Y, a_Z );

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
	return Chunk->GetHeight( PosX, PosZ );
}

const double & cWorld::GetSpawnY()
{
	m_SpawnY = (double)GetHeight( (int)m_SpawnX, (int)m_SpawnZ ) + 1.6f; // +1.6f eye height
	return m_SpawnY;
}

void cWorld::AddPlayer( cPlayer* a_Player )
{
	m_pState->m_Players.remove( a_Player );
	m_pState->m_Players.push_back( a_Player );
}

void cWorld::RemovePlayer( cPlayer* a_Player )
{
	m_pState->m_Players.remove( a_Player );
}

void cWorld::GetAllPlayers( lua_State* L )
{
	lua_createtable(L, m_pState->m_Players.size(), 0);
	int newTable = lua_gettop(L);
	int index = 1;
	PlayerList::const_iterator iter = m_pState->m_Players.begin();
	while(iter != m_pState->m_Players.end()) {
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
	for( PlayerList::iterator itr = m_pState->m_Players.begin(); itr != m_pState->m_Players.end(); itr++ )
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
	for( EntityList::iterator itr = m_pState->m_AllEntities.begin(); itr != m_pState->m_AllEntities.end(); ++itr )
	{
		if( (*itr)->GetUniqueID() == a_UniqueID )
			return *itr;
	}
	return 0;
}

void cWorld::RemoveClient( cClientHandle* a_Client )
{
	m_pState->m_Clients.remove( a_Client );
	if( a_Client )
	{
		delete a_Client;
		a_Client = 0;
	}
}

void cWorld::RemoveEntity( cEntity* a_Entity )
{
	m_pState->m_RemoveEntityQueue.remove( a_Entity );
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
	m_pState->m_SpreadQueue.remove( a_Chunk );
	m_pState->m_SpreadQueue.push_back( a_Chunk );
	UnlockChunks();
}

void cWorld::RemoveSpread( cChunk* a_Chunk )
{
	LockChunks();
	m_pState->m_SpreadQueue.remove( a_Chunk );
	UnlockChunks();
}


/************************************************************************/
/* Get and set                                                          */
/************************************************************************/
void cWorld::AddClient( cClientHandle* a_Client )
{
	m_pState->m_Clients.push_back( a_Client );
}
cWorld::ClientList & cWorld::GetClients()
{
	return m_pState->m_Clients;
}
cWorld::EntityList & cWorld::GetEntities()
{
	return m_pState->m_AllEntities;
}
void cWorld::AddEntity( cEntity* a_Entity )
{
	m_pState->m_AllEntities.push_back( a_Entity ); 
}
cWorld::PlayerList & cWorld::GetAllPlayers()
{
	return m_pState->m_Players; 
}
unsigned int cWorld::GetNumPlayers()
{
	return m_pState->m_Players.size(); 
}
void cWorld::AddToRemoveEntityQueue( cEntity & a_Entity )
{
	m_pState->m_AllEntities.remove( &a_Entity);
	m_pState->m_RemoveEntityQueue.push_back( &a_Entity ); 
}
