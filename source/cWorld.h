
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
#include "Vector3i.h"
#include "ChunkSender.h"
#include "Defines.h"





class cPacket;
class cRedstone;
class cFireSimulator;
class cWaterSimulator;
class cLavaSimulator;
class cSandSimulator;
class cRedstoneSimulator;
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
	long long GetWorldTime(void) const { return m_WorldTime; }						//tolua_export

	eGameMode GetGameMode(void) const { return m_GameMode; }						//tolua_export

	void SetWorldTime(long long a_WorldTime) { m_WorldTime = a_WorldTime; }			//tolua_export

	int GetHeight( int a_X, int a_Z );												//tolua_export

	void Broadcast( const cPacket & a_Packet, cClientHandle* a_Exclude = 0 );
	void BroadcastToChunk(int a_ChunkX, int a_ChunkY, int a_ChunkZ, const cPacket & a_Packet, cClientHandle * a_Exclude = NULL);
	void BroadcastToChunkOfBlock(int a_X, int a_Y, int a_Z, cPacket * a_Packet, cClientHandle * a_Exclude = NULL);
	
	void MarkChunkDirty    (int a_ChunkX, int a_ChunkY, int a_ChunkZ);
	void MarkChunkSaving   (int a_ChunkX, int a_ChunkY, int a_ChunkZ);
	void MarkChunkSaved    (int a_ChunkX, int a_ChunkY, int a_ChunkZ);
	void ChunkDataLoaded   (int a_ChunkX, int a_ChunkY, int a_ChunkZ, const char * a_BlockData, cEntityList & a_Entities, cBlockEntityList & a_BlockEntities);
	void ChunkDataGenerated(int a_ChunkX, int a_ChunkY, int a_ChunkZ, const char * a_BlockData, cEntityList & a_Entities, cBlockEntityList & a_BlockEntities);
	bool GetChunkData      (int a_ChunkX, int a_ChunkY, int a_ChunkZ, cChunkDataCallback & a_Callback);
	
	/// Gets the chunk's blocks, only the block types
	bool GetChunkBlocks    (int a_ChunkX, int a_ChunkY, int a_ChunkZ, char * a_Blocks);
	
	/// Gets the chunk's blockdata, the entire array
	bool GetChunkBlockData (int a_ChunkX, int a_ChunkY, int a_ChunkZ, char * a_BlockData);
	
	bool IsChunkValid      (int a_ChunkX, int a_ChunkY, int a_ChunkZ) const;
	bool HasChunkAnyClients(int a_ChunkX, int a_ChunkY, int a_ChunkZ) const;
	void UnloadUnusedChunks(void);
	void CollectPickupsByPlayer(cPlayer * a_Player);

	// MOTD
	const AString & GetDescription(void) const {return m_Description; }	// FIXME: This should not be in cWorld

	// Max Players
	unsigned int GetMaxPlayers(void) const {return m_MaxPlayers; }					//tolua_export
	void SetMaxPlayers(int iMax);													//tolua_export

	void AddPlayer( cPlayer* a_Player );
	void RemovePlayer( cPlayer* a_Player );

	typedef struct lua_State lua_State;
	bool ForEachPlayer(cPlayerListCallback & a_Callback);	// Calls the callback for each player in the list
															// >> EXPORTED IN MANUALBINDINGS <<
	unsigned int GetNumPlayers();													//tolua_export
	
	// TODO: This interface is dangerous - rewrite to DoWithPlayer(playername, action)
	cPlayer * GetPlayer( const char * a_PlayerName );								//tolua_export
	
	// TODO: This interface is dangerous - rewrite to DoWithClosestPlayer(pos, sight, action)
	cPlayer * FindClosestPlayer(const Vector3f & a_Pos, float a_SightLimit);
	
	void SendPlayerList(cPlayer * a_DestPlayer);  // Sends playerlist to the player

	void AddEntity( cEntity* a_Entity );
	
	/// Add an entity to the chunk specified; broadcasts the a_SpawnPacket to all clients of that chunk
	void AddEntityToChunk(cEntity * a_Entity, int a_ChunkX, int a_ChunkY, int a_ChunkZ, cPacket * a_SpawnPacket);
	
	/// Removes the entity from the chunk specified
	void RemoveEntityFromChunk(cEntity * a_Entity, int a_ChunkX, int a_ChunkY, int a_ChunkZ);
	
	/// Moves the entity from its current chunk to the new chunk specified
	void MoveEntityToChunk(cEntity * a_Entity, int a_ChunkX, int a_ChunkY, int a_ChunkZ);

	/// Compares clients of two chunks, calls the callback accordingly
	void CompareChunkClients(int a_ChunkX1, int a_ChunkY1, int a_ChunkZ1, int a_ChunkX2, int a_ChunkY2, int a_ChunkZ2, cClientDiffCallback & a_Callback);
	
	/// Adds client to a chunk, if not already present; returns true if added, false if present
	bool AddChunkClient(int a_ChunkX, int a_ChunkY, int a_ChunkZ, cClientHandle * a_Client);
	
	/// Removes client from the chunk specified
	void RemoveChunkClient(int a_ChunkX, int a_ChunkY, int a_ChunkZ, cClientHandle * a_Client);
	
	/// Removes the client from all chunks specified
	void RemoveClientFromChunks(cClientHandle * a_Client, const cChunkCoordsList & a_Chunks);
	
	/// Sends the chunk to the client specified, if the chunk is valid. If not valid, the request is ignored (ChunkSender will send that chunk when it becomes valid)
	void SendChunkTo(int a_ChunkX, int a_ChunkY, int a_ChunkZ, cClientHandle * a_Client);
	
	/// Removes client from ChunkSender's queue of chunks to be sent
	void RemoveClientFromChunkSender(cClientHandle * a_Client);
	
	/// Touches the chunk, causing it to be loaded or generated
	void TouchChunk(int a_ChunkX, int a_ChunkY, int a_ChunkZ);
	
	/// Loads the chunk, if not already loaded. Doesn't generate. Returns true if chunk valid (even if already loaded before)
	bool LoadChunk(int a_ChunkX, int a_ChunkY, int a_ChunkZ);
	
	/// Loads the chunks specified. Doesn't report failure, other than chunks being !IsValid()
	void LoadChunks(const cChunkCoordsList & a_Chunks);
	
	/// Marks the chunk as failed-to-load:
	void ChunkLoadFailed(int a_ChunkX, int a_ChunkY, int a_ChunkZ);
	
	void UpdateSign(int a_X, int a_Y, int a_Z, const AString & a_Line1, const AString & a_Line2, const AString & a_Line3, const AString & a_Line4);	//tolua_export

	/// Marks (a_Stay == true) or unmarks (a_Stay == false) chunks as non-unloadable. To be used only by cChunkStay!
	void ChunksStay(const cChunkCoordsList & a_Chunks, bool a_Stay = true);

	// TODO: Export to Lua
	bool DoWithEntity( int a_UniqueID, cEntityCallback & a_Callback );

	void SetBlock( int a_X, int a_Y, int a_Z, char a_BlockType, char a_BlockMeta );						//tolua_export
	void FastSetBlock( int a_X, int a_Y, int a_Z, char a_BlockType, char a_BlockMeta );					//tolua_export
	char GetBlock( int a_X, int a_Y, int a_Z );															//tolua_export
	char GetBlock( const Vector3i & a_Pos ) { return GetBlock( a_Pos.x, a_Pos.y, a_Pos.z ); }			//tolua_export
	char GetBlockMeta( int a_X, int a_Y, int a_Z );														//tolua_export
	char GetBlockMeta( const Vector3i & a_Pos ) { return GetBlockMeta( a_Pos.x, a_Pos.y, a_Pos.z ); }	//tolua_export
	void SetBlockMeta( int a_X, int a_Y, int a_Z, char a_MetaData );									//tolua_export
	void SetBlockMeta( const Vector3i & a_Pos, char a_MetaData ) { SetBlockMeta( a_Pos.x, a_Pos.y, a_Pos.z, a_MetaData ); } //tolua_export
	bool DigBlock( int a_X, int a_Y, int a_Z, cItem & a_PickupItem );									//tolua_export
	void SendBlockTo( int a_X, int a_Y, int a_Z, cPlayer* a_Player );									//tolua_export

	const double & GetSpawnX() { return m_SpawnX; }														//tolua_export
	const double & GetSpawnY();																			//tolua_export
	const double & GetSpawnZ() { return m_SpawnZ; }														//tolua_export

	inline cSimulatorManager *GetSimulatorManager() { return m_SimulatorManager; }
	inline cWaterSimulator *GetWaterSimulator() { return m_WaterSimulator; }
	inline cLavaSimulator *GetLavaSimulator() { return m_LavaSimulator; }

	// TODO: This interface is dangerous! Export as a set of specific action functions for Lua: GetChestItem, GetFurnaceItem, SetFurnaceItem, SetSignLines etc.
	// _X 2012_02_21: This function always returns NULL
	OBSOLETE cBlockEntity * GetBlockEntity( int a_X, int a_Y, int a_Z );						//tolua_export
	
	/// a_Player is using block entity at [x, y, z], handle that:
	void UseBlockEntity(cPlayer * a_Player, int a_X, int a_Y, int a_Z) {m_ChunkMap->UseBlockEntity(a_Player, a_X, a_Y, a_Z); }

	void GrowTree( int a_X, int a_Y, int a_Z );													//tolua_export

	unsigned int GetWorldSeed(void) const { return m_WorldSeed; }								//tolua_export
	const AString & GetName(void) const { return m_WorldName; }									//tolua_export

	inline static void AbsoluteToRelative( int & a_X, int & a_Y, int & a_Z, int & a_ChunkX, int & a_ChunkY, int & a_ChunkZ )
	{
		// TODO: Use floor() instead of weird if statements
		// Also fix Y
		a_ChunkX = a_X/cChunk::c_ChunkWidth;
		if(a_X < 0 && a_X % cChunk::c_ChunkWidth != 0) a_ChunkX--;
		a_ChunkY = 0;
		a_ChunkZ = a_Z/cChunk::c_ChunkWidth;
		if(a_Z < 0 && a_Z % cChunk::c_ChunkWidth != 0) a_ChunkZ--;

		a_X = a_X - a_ChunkX*cChunk::c_ChunkWidth;
		a_Y = a_Y - a_ChunkY*cChunk::c_ChunkHeight;
		a_Z = a_Z - a_ChunkZ*cChunk::c_ChunkWidth;
	}
	
	inline static void BlockToChunk( int a_X, int a_Y, int a_Z, int & a_ChunkX, int & a_ChunkY, int & a_ChunkZ )
	{
		// TODO: Use floor() instead of weird if statements
		// Also fix Y
		(void)a_Y; // not unused anymore
		a_ChunkX = a_X/cChunk::c_ChunkWidth;
		if(a_X < 0 && a_X % cChunk::c_ChunkWidth != 0) a_ChunkX--;
		a_ChunkY = 0;
		a_ChunkZ = a_Z/cChunk::c_ChunkWidth;
		if(a_Z < 0 && a_Z % cChunk::c_ChunkWidth != 0) a_ChunkZ--;
	}

	void SaveAllChunks();			//tolua_export
	int GetNumChunks() const;		//tolua_export

	void Tick(float a_Dt);

	void ReSpreadLighting(int a_ChunkX, int a_ChunkY, int a_ChunkZ);
	void RemoveSpread(int a_ChunkX, int a_ChunkY, int a_ChunkZ);

	void InitializeSpawn();

	void CastThunderbolt (int a_X, int a_Y, int a_Z);						//tolua_export
	void SetWeather ( eWeather a_Weather );									//tolua_export
	eWeather GetWeather() { return m_Weather; };							//tolua_export

	cChunkGenerator & GetGenerator(void) { return m_Generator; }
	cWorldStorage &   GetStorage  (void) { return m_Storage; }
	cChunkMap *       GetChunkMap (void) { return m_ChunkMap; }
	
private:

	friend class cRoot;
	
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
	eGameMode m_GameMode;
	float m_WorldTimeFraction; // When this > 1.f m_WorldTime is incremented by 20

	// The cRedstone class simulates redstone and needs access to m_RSList
	friend class cRedstone;
	std::vector<int> m_RSList;

	cSimulatorManager *  m_SimulatorManager;
	cSandSimulator *     m_SandSimulator;
	cWaterSimulator *    m_WaterSimulator;
	cLavaSimulator *     m_LavaSimulator;
	cFireSimulator *     m_FireSimulator;
	cRedstoneSimulator * m_RedstoneSimulator;
	
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
	
	eWeather m_Weather;
	
	cEntityList       m_RemoveEntityQueue;
	cEntityList       m_AllEntities;
	cClientHandleList m_Clients;
	cPlayerList       m_Players;

	cCriticalSection  m_CSLighting;
	cChunkCoordsList  m_SpreadQueue;

	cCriticalSection m_CSFastSetBlock;
	sSetBlockList    m_FastSetBlockQueue;

	cChunkGenerator  m_Generator;
	
	cChunkSender     m_ChunkSender;

	AString m_WorldName;

	cWorld(const AString & a_WorldName);
	~cWorld();

	void TickWeather(float a_Dt);  // Handles weather each tick
	void TickSpawnMobs(float a_Dt);  // Handles mob spawning each tick
	void TickLighting(void);  // Handles lighting re-spreading
	
	void RemoveEntity( cEntity * a_Entity );
}; //tolua_export




