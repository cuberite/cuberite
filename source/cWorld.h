#pragma once

#ifndef _WIN32
#include "BlockID.h"
#else
enum ENUM_ITEM_ID;
#endif

#define MAX_PLAYERS 65535

#include <list>
#include <vector>
#include <string>

#include "cSimulatorManager.h"

class cPacket;
class cRedstone;
class cWaterSimulator;
class cLavaSimulator;
class cSandSimulator;
class cChunkMap;
class cItem;
class cCriticalSection;
class cPlayer;
class cClientHandle;
class cChunk;
class cEntity;
class cBlockEntity;
class cWorldGenerator;


class cWorld													//tolua_export
{																//tolua_export
public:
	typedef std::list< cClientHandle* > ClientList;
	typedef std::list< cEntity* > EntityList;
	typedef std::list< cChunk* > ChunkList;
	typedef std::list< cPlayer* > PlayerList;
	std::vector<int> m_RSList;


	static cWorld* GetWorld();														//tolua_export

	// Return time in seconds
	inline static float GetTime()													//tolua_export
	{
		return m_Time;
	}
	long long GetWorldTime() { return m_WorldTime; }								//tolua_export

	int GetGameMode() { return m_GameMode; } //return gamemode for world

	void SetWorldTime(long long a_WorldTime) { m_WorldTime = a_WorldTime; }			//tolua_export

	cChunk* GetChunk( int a_X, int a_Y, int a_Z );
	cChunk* GetChunkReliable( int a_X, int a_Y, int a_Z );
	cChunk* GetChunkUnreliable( int a_X, int a_Y, int a_Z );
	cChunk* GetChunkOfBlock( int a_X, int a_Y, int a_Z );
	char GetHeight( int a_X, int a_Z );												//tolua_export

	//void AddClient( cClientHandle* a_Client );
	//void RemoveClient( cClientHandle* a_Client );
	//ClientList & GetClients();

	void Broadcast( const cPacket & a_Packet, cClientHandle* a_Exclude = 0 );
	
	// MOTD
	std::string GetDescription();

	// Max Players
	unsigned int GetMaxPlayers();
	void SetMaxPlayers(int iMax);

	void AddPlayer( cPlayer* a_Player );
	void RemovePlayer( cPlayer* a_Player );
	PlayerList & GetAllPlayers();
	typedef struct lua_State lua_State;
	void GetAllPlayers( lua_State* L );												// >> EXPORTED IN MANUALBINDINGS <<
	unsigned int GetNumPlayers();													//tolua_export
	cPlayer* GetPlayer( const char* a_PlayerName );									//tolua_export

	void AddEntity( cEntity* a_Entity );
	bool RemoveEntityFromChunk( cEntity & a_Entity, cChunk* a_CalledFrom = 0 );
	EntityList & GetEntities();

	cEntity* GetEntity( int a_UniqueID );											//tolua_export

	void SetBlock( int a_X, int a_Y, int a_Z, char a_BlockType, char a_BlockMeta );	//tolua_export
	void FastSetBlock( int a_X, int a_Y, int a_Z, char a_BlockType, char a_BlockMeta );	//tolua_export
	char GetBlock( int a_X, int a_Y, int a_Z );										//tolua_export
	char GetBlockMeta( int a_X, int a_Y, int a_Z );									//tolua_export
	void SetBlockMeta( int a_X, int a_Y, int a_Z, char a_MetaData );				//tolua_export
	bool DigBlock( int a_X, int a_Y, int a_Z, cItem & a_PickupItem );				//tolua_export
	void SendBlockTo( int a_X, int a_Y, int a_Z, cPlayer* a_Player );				//tolua_export

	const double & GetSpawnX() { return m_SpawnX; }									//tolua_export
	const double & GetSpawnY();														//tolua_export
	const double & GetSpawnZ() { return m_SpawnZ; }									//tolua_export

	inline cSimulatorManager *GetSimulatorManager() { return m_SimulatorManager; }
	inline cWaterSimulator *GetWaterSimulator() { return m_WaterSimulator; }
	inline cLavaSimulator *GetLavaSimulator() { return m_LavaSimulator; }


	cBlockEntity* GetBlockEntity( int a_X, int a_Y, int a_Z );						//tolua_export

	void GrowTree( int a_X, int a_Y, int a_Z );										//tolua_export

	unsigned int GetWorldSeed() { return m_WorldSeed; }								//tolua_export
	const char* GetName();															//tolua_export

	inline static void AbsoluteToRelative( int & a_X, int & a_Y, int & a_Z, int & a_ChunkX, int & a_ChunkY, int & a_ChunkZ )
	{
		(void)a_Y; // not unused anymore
		a_ChunkX = a_X/16;
		if(a_X < 0 && a_X % 16 != 0) a_ChunkX--;
		a_ChunkY = 0;
		a_ChunkZ = a_Z/16;
		if(a_Z < 0 && a_Z % 16 != 0) a_ChunkZ--;

		a_X = a_X - a_ChunkX*16;
		//a_Y = a_Y - a_ChunkY*16;
		a_Z = a_Z - a_ChunkZ*16;
	}
	inline static void BlockToChunk( int a_X, int a_Y, int a_Z, int & a_ChunkX, int & a_ChunkY, int & a_ChunkZ )
	{
		(void)a_Y; // not unused anymore
		a_ChunkX = a_X/16;
		if(a_X < 0 && a_X % 16 != 0) a_ChunkX--;
		a_ChunkY = 0;
		a_ChunkZ = a_Z/16;
		if(a_Z < 0 && a_Z % 16 != 0) a_ChunkZ--;
	}

	void SaveAllChunks();

	void Tick(float a_Dt);

	void LockClientHandle();
	void UnlockClientHandle();

	void LockEntities();
	void UnlockEntities();

	void LockChunks();
	void UnlockChunks();

	void ReSpreadLighting( cChunk* a_Chunk );
	void RemoveSpread( cChunk* a_Chunk );

	void InitializeSpawn();

	void CastThunderbolt ( int, int, int );												//tolua_export
	void SetWeather ( int );												//tolua_export
	int GetWeather() { return m_Weather; };												//tolua_export

	cWorldGenerator* GetWorldGenerator() { return m_WorldGenerator; }
private:
	friend class cRoot;
	cWorld( const char* a_WorldName );
	~cWorld();

	struct sWorldState;
	sWorldState* m_pState;

	void AddToRemoveEntityQueue( cEntity & a_Entity );
	void RemoveEntity( cEntity* a_Entity );
	void UnloadUnusedChunks();

	double m_SpawnX;
	double m_SpawnY;
	double m_SpawnZ;

	float m_LastUnload;
	float m_LastSave;
	static float m_Time;	// Time in seconds
	long long m_WorldTime; // Time in seconds*20, this is sent to clients (is wrapped)
	unsigned long long CurrentTick;
	int m_GameMode;
	float m_WorldTimeFraction; // When this > 1.f m_WorldTime is incremented by 20

	cSimulatorManager *m_SimulatorManager;
	cSandSimulator *m_SandSimulator;
	cWaterSimulator* m_WaterSimulator;
	cLavaSimulator* m_LavaSimulator;
	
	cCriticalSection* m_ClientHandleCriticalSection;
	cCriticalSection* m_EntitiesCriticalSection;
	cCriticalSection* m_ChunksCriticalSection;

	cWorldGenerator* m_WorldGenerator;
	
	std::string m_Description;
	unsigned int m_MaxPlayers;

	cChunkMap* m_ChunkMap;

	bool m_bAnimals;
	float m_SpawnMonsterTime;
	float m_SpawnMonsterRate;

	unsigned int m_WorldSeed;
	int	m_Weather;
}; //tolua_export
