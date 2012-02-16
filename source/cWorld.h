
#pragma once

#ifndef _WIN32
	#include "BlockID.h"
#else
	enum ENUM_ITEM_ID;
#endif

#define MAX_PLAYERS 65535

#include "cSimulatorManager.h"
#include "MersenneTwister.h"
#include "cChunkMap.h"
#include "WorldStorage.h"
#include "cChunkGenerator.h"





class cPacket;
class cRedstone;
class cFireSimulator;
class cWaterSimulator;
class cLavaSimulator;
class cSandSimulator;
class cItem;
class cPlayer;
class cClientHandle;
class cEntity;
class cBlockEntity;
class cWorldGenerator;  // The generator that actually generates the chunks for a single world
class cChunkGenerator;  // The thread responsible for generating chunks
typedef std::list< cPlayer * > cPlayerList;
typedef cItemCallback<cPlayer> cPlayerListCallback;
typedef cItemCallback<cEntity> cEntityCallback;





class cWorld													//tolua_export
{																//tolua_export
public:

	static cWorld* GetWorld();														//tolua_export

	// Return time in seconds
	inline static float GetTime()													//tolua_export
	{
		return m_Time;
	}
	long long GetWorldTime(void) const { return m_WorldTime; }								//tolua_export

	int GetGameMode(void) const { return m_GameMode; } //return gamemode for world

	void SetWorldTime(long long a_WorldTime) { m_WorldTime = a_WorldTime; }			//tolua_export

	cChunkPtr GetChunk       ( int a_ChunkX, int a_ChunkY, int a_ChunkZ ) {return m_ChunkMap->GetChunk     (a_ChunkX, a_ChunkY, a_ChunkZ); }
	cChunkPtr GetChunkNoGen  ( int a_ChunkX, int a_ChunkY, int a_ChunkZ ) {return m_ChunkMap->GetChunkNoGen(a_ChunkX, a_ChunkY, a_ChunkZ); }
	cChunkPtr GetChunkOfBlock( int a_X, int a_Y, int a_Z );
	char GetHeight( int a_X, int a_Z );												//tolua_export

	//void AddClient( cClientHandle* a_Client );
	//void RemoveClient( cClientHandle* a_Client );
	//ClientList & GetClients();

	void Broadcast( const cPacket & a_Packet, cClientHandle* a_Exclude = 0 );
	
	void BroadcastToChunkOfBlock(int a_X, int a_Y, int a_Z, cPacket * a_Packet, cClientHandle * a_Exclude = NULL);
	
	void MarkChunkDirty    (int a_ChunkX, int a_ChunkY, int a_ChunkZ);
	void MarkChunkSaving   (int a_ChunkX, int a_ChunkY, int a_ChunkZ);
	void MarkChunkSaved    (int a_ChunkX, int a_ChunkY, int a_ChunkZ);
	void ChunkDataLoaded   (int a_ChunkX, int a_ChunkY, int a_ChunkZ, const char * a_BlockData, cEntityList & a_Entities, cBlockEntityList & a_BlockEntities);
	void SetChunkData      (int a_ChunkX, int a_ChunkY, int a_ChunkZ, const char * a_BlockData, cEntityList & a_Entities, cBlockEntityList & a_BlockEntities);
	void GetChunkData      (int a_ChunkX, int a_ChunkY, int a_ChunkZ, cChunkDataCallback * a_Callback);
	bool IsChunkValid      (int a_ChunkX, int a_ChunkY, int a_ChunkZ) const;
	bool HasChunkAnyClients(int a_ChunkX, int a_ChunkY, int a_ChunkZ) const;
	void UnloadUnusedChunks(void);

	// MOTD
	const AString & GetDescription(void) const {return m_Description; }

	// Max Players
	unsigned int GetMaxPlayers(void) const {return m_MaxPlayers; }
	void SetMaxPlayers(int iMax);

	void AddPlayer( cPlayer* a_Player );
	void RemovePlayer( cPlayer* a_Player );

	typedef struct lua_State lua_State;
	bool ForEachPlayer(cPlayerListCallback & a_Callback);	// Calls the callback for each player in the list
															// >> EXPORTED IN MANUALBINDINGS <<
	unsigned int GetNumPlayers();													//tolua_export
	
	// TODO: This interface is dangerous - rewrite to DoWithPlayer(playername, action)
	cPlayer * GetPlayer( const char * a_PlayerName );									//tolua_export
	
	// TODO: This interface is dangerous - rewrite to DoWithClosestPlayer(pos, sight, action)
	cPlayer * FindClosestPlayer(const Vector3f & a_Pos, float a_SightLimit);
	
	void SendPlayerList(cPlayer * a_DestPlayer);  // Sends playerlist to the player

	void AddEntity( cEntity* a_Entity );
	void RemoveEntityFromChunk( cEntity * a_Entity);

	// TODO: Export to Lua
	bool DoWithEntity( int a_UniqueID, cEntityCallback & a_Callback );

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

	// TODO: This interface is dangerous! Export as a set of specific action functions for Lua: GetChestItem, GetFurnaceItem, SetFurnaceItem, SetSignLines etc.
	cBlockEntity * GetBlockEntity( int a_X, int a_Y, int a_Z );						//tolua_export
	
	/// a_Player is using block entity at [x, y, z], handle that:
	void UseBlockEntity(cPlayer * a_Player, int a_X, int a_Y, int a_Z) {m_ChunkMap->UseBlockEntity(a_Player, a_X, a_Y, a_Z); }

	void GrowTree( int a_X, int a_Y, int a_Z );										//tolua_export

	unsigned int GetWorldSeed(void) const { return m_WorldSeed; }								//tolua_export
	const AString & GetName(void) const {return m_WorldName; }															//tolua_export

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

	void SaveAllChunks();	//tolua_export
	int GetNumChunks() const;		//tolua_export

	void Tick(float a_Dt);

	void ReSpreadLighting(const cChunkPtr & a_Chunk );
	void RemoveSpread(const cChunkPtr & a_Chunk );

	void InitializeSpawn();

	void CastThunderbolt ( int, int, int );									//tolua_export
	void SetWeather ( int );												//tolua_export
	int GetWeather() { return m_Weather; };									//tolua_export

	cChunkGenerator & GetGenerator(void) { return m_Generator; }
	cWorldStorage &   GetStorage  (void) { return m_Storage; }
	
private:

	friend class cRoot;
	
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

	// This random generator is to be used only in the Tick() method, and thus only in the World-Tick-thread (MTRand is not exactly thread-safe)
	MTRand m_TickRand;

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

	// The cRedstone class simulates redstone and needs access to m_RSList
	friend class cRedstone;
	std::vector<int> m_RSList;

	cSimulatorManager * m_SimulatorManager;
	cSandSimulator *    m_SandSimulator;
	cWaterSimulator *   m_WaterSimulator;
	cLavaSimulator *    m_LavaSimulator;
	cFireSimulator *    m_FireSimulator;
	
	cCriticalSection m_CSClients;
	cCriticalSection m_CSEntities;
	cCriticalSection m_CSPlayers;

	cWorldStorage     m_Storage;
	
	AString m_Description;
	
	unsigned int m_MaxPlayers;

	cChunkMap * m_ChunkMap;

	bool m_bAnimals;
	float m_SpawnMonsterTime;
	float m_SpawnMonsterRate;

	unsigned int m_WorldSeed;
	
	int	m_Weather;
	
	cEntityList       m_RemoveEntityQueue;
	cEntityList       m_AllEntities;
	cClientHandleList m_Clients;
	cPlayerList       m_Players;

	cCriticalSection  m_CSLighting;
	cChunkPtrList     m_SpreadQueue;

	cCriticalSection m_CSFastSetBlock;
	FastSetBlockList m_FastSetBlockQueue;

	cChunkGenerator  m_Generator;

	AString m_WorldName;

	cWorld(const AString & a_WorldName);
	~cWorld();

	void TickWeather(float a_Dt);  // Handles weather each tick
	void TickSpawnMobs(float a_Dt);  // Handles mob spawning each tick
	
	void RemoveEntity( cEntity * a_Entity );
}; //tolua_export




