
#pragma once

#include "cEntity.h"





#define C_CHUNK_USE_INLINE 1

// Do not touch
#if C_CHUNK_USE_INLINE
	#define __C_CHUNK_INLINE__ inline
#else
	#define __C_CHUNK_INLINE__
#endif





/** This is really only a placeholder to be used in places where we need to "make up" a chunk's Y coord.
It will help us when the new chunk format comes out and we need to patch everything up for compatibility.
*/
#define ZERO_CHUNK_Y 0





namespace Json
{
	class Value;
};





class cWorld;
class cFurnaceEntity;
class cPacket;
class cBlockEntity;
class cClientHandle;
class cServer;
class MTRand;
class cPlayer;

typedef std::list<cClientHandle *>  cClientHandleList;
typedef std::list<cBlockEntity *>   cBlockEntityList;

/** Interface class used for getting data out of a chunk using the GetAllData() function.
Implementation must use the pointers immediately and NOT store any of them for later use
*/
class cChunkDataCallback
{
public:
	/// Called once to export blockdata
	virtual void BlockData(const char * a_Data) = 0;
	
	/// Called for each entity in the chunk
	virtual void Entity(cEntity * a_Entity) = 0;
	
	/// Called for each blockentity in the chunk
	virtual void BlockEntity(cBlockEntity * a_Entity) = 0;
} ;





class cChunk
{
public:
	cChunk(int a_X, int a_Y, int a_Z, cWorld* a_World);
	~cChunk();

	bool IsValid(void) const {return m_IsValid; }  // Returns true if the chunk is valid (loaded / generated)
	void SetValid(bool a_SendToClients = true);   // Also wakes up all clients attached to this chunk to let them finish logging in
	bool IsDirty(void) const {return m_IsDirty; }  // Returns true if the chunk has changed since it was last saved
	bool CanUnload(void);
	
	/*
	To save a chunk, the WSSchema must:
	1. Mark the chunk as being saved (MarkSaving() )
	2. Get the chunk's data using GetAllData()
	3. Mark the chunk as saved (MarkSaved() )
	If anywhere inside this sequence another thread mmodifies the chunk, the chunk will not get marked as saved in MarkSaved()
	*/
	void MarkSaving(void);  // Marks the chunk as being saved. 
	void MarkSaved(void);  // Marks the chunk as saved, if it didn't change from the last call to MarkSaving()
	void MarkLoaded(void);  // Marks the chunk as freshly loaded. Fails if the chunk is already valid
	
	/// Gets all chunk data, calls the a_Callback's methods for each data type
	void GetAllData(cChunkDataCallback * a_Callback);
	
	/// Sets all chunk data
	void SetAllData(const char * a_BlockData, cEntityList & a_Entities, cBlockEntityList & a_BlockEntities);
	
	/// Returns true if there is a block entity at the coords specified
	bool HasBlockEntityAt(int a_BlockX, int a_BlockY, int a_BlockZ);

	void Tick(float a_Dt, MTRand & a_TickRandom);

	int GetPosX() { return m_PosX; }
	int GetPosY() { return m_PosY; }
	int GetPosZ() { return m_PosZ; }
	cWorld * GetWorld() { return m_World; }

	void Send( cClientHandle* a_Client );
	void AsyncUnload( cClientHandle* a_Client );

	void SetBlock( int a_X, int a_Y, int a_Z, char a_BlockType, char a_BlockMeta );
	void FastSetBlock( int a_X, int a_Y, int a_Z, char a_BlockType, char a_BlockMeta );  // Doesn't force block updates on neighbors, use for simple changes such as grass growing etc.
	char GetBlock( int a_X, int a_Y, int a_Z );
	char GetBlock( int a_BlockIdx );
	
	void CollectPickupsByPlayer(cPlayer * a_Player);
	void UpdateSign(int a_PosX, int a_PosY, int a_PosZ, const AString & a_Line1, const AString & a_Line2, const AString & a_Line3, const AString & a_Line4);  // Also sends update packets to all clients in the chunk

	char GetHeight( int a_X, int a_Z );

	void SendBlockTo( int a_X, int a_Y, int a_Z, cClientHandle* a_Client );

	void AddClient    (cClientHandle* a_Client );
	void RemoveClient (cClientHandle* a_Client );
	bool HasClient    (cClientHandle* a_Client );
	bool HasAnyClients(void);  // Returns true if theres any client in the chunk; false otherwise

	void AddEntity( cEntity * a_Entity );
	void RemoveEntity( cEntity * a_Entity);
	
	void UseBlockEntity(cPlayer * a_Player, int a_X, int a_Y, int a_Z);  // [x, y, z] in world block coords

	inline void RecalculateLighting() { m_bCalculateLighting = true; } // Recalculate lighting next tick
	inline void RecalculateHeightmap() { m_bCalculateHeightmap = true; } // Recalculate heightmap next tick
	void SpreadLight(char* a_LightBuffer);
	void CalculateLighting(); // Recalculate right now
	void CalculateHeightmap();

	bool LoadFromDisk();

	// Broadcasts to all clients that have loaded this chunk
	void Broadcast( const cPacket & a_Packet, cClientHandle * a_Exclude = NULL) {Broadcast(&a_Packet, a_Exclude); }
	void Broadcast( const cPacket * a_Packet, cClientHandle * a_Exclude = NULL);

	// TODO: These functions are dangerous - rewrite to:
	//   Loaded(blockdata, lightdata, blockentities, entities),
	//   Generated(blockdata, lightdata, blockentities, entities),
	//   GetBlockData(blockdatadest) etc.
	char* pGetBlockData() { return m_BlockData; }
	char* pGetType() { return m_BlockType; }
	char* pGetMeta() { return m_BlockMeta; }
	char* pGetLight() { return m_BlockLight; }
	char* pGetSkyLight() { return m_BlockSkyLight; }
	
	void CopyBlockDataFrom(const char * a_NewBlockData);  // Copies all blockdata, recalculates heightmap (used by chunk loaders)
	
	// TODO: Move this into the specific WSSchema:
	void LoadFromJson( const Json::Value & a_Value );
	void SaveToJson( Json::Value & a_Value );

	char GetLight(char* a_Buffer, int a_BlockIdx);
	char GetLight(char* a_Buffer, int x, int y, int z);
	void SetLight(char* a_Buffer, int a_BlockIdx, char a_Light);
	void SetLight(char* a_Buffer, int x, int y, int z, char light);

	void PositionToWorldPosition(int a_ChunkX, int a_ChunkY, int a_ChunkZ, int & a_X, int & a_Y, int & a_Z);

	inline static unsigned int MakeIndex(int x, int y, int z )
	{
		if( x < 16 && x > -1 && y < 128 && y > -1 && z < 16 && z > -1 )
		{
			return y + (z * 128) + (x * 128 * 16);
		}
		return 0;
	}
	
	inline void MarkDirty(void)
	{
		m_IsDirty = true;
		m_IsSaving = false;
	}

	static const int c_NumBlocks = 16*128*16;
	static const int c_BlockDataSize = c_NumBlocks * 2 + (c_NumBlocks/2); // 2.5 * numblocks
	
private:

	bool m_IsValid;  // True if the chunk is loaded / generated
	bool m_IsDirty;  // True if the chunk has changed since it was last saved
	bool m_IsSaving;  // True if the chunk is being saved
	
	cCriticalSection              m_CSBlockLists;
	std::map< unsigned int, int > m_ToTickBlocks;
	std::vector< unsigned int >   m_PendingSendBlocks;
	
	// TODO: This CS will soon not be needed, because all chunk access is protected by its parent ChunkMap's csLayers
	cCriticalSection  m_CSClients;
	cClientHandleList m_LoadedByClient;
	cClientHandleList m_UnloadQuery;

	// TODO: This CS will soon not be needed, because all chunk access is protected by its parent ChunkMap's csLayers
	cCriticalSection   m_CSEntities;
	cEntityList        m_Entities;
	cBlockEntityList   m_BlockEntities;

	bool m_bCalculateLighting;
	bool m_bCalculateHeightmap;

	int m_PosX, m_PosY, m_PosZ;
	cWorld * m_World;

	char m_BlockData[c_BlockDataSize]; // Chunk data ready to be compressed and sent
	char *m_BlockType;		// Pointers to an element in m_BlockData
	char *m_BlockMeta;		// += NumBlocks
	char *m_BlockLight;		// += NumBlocks/2
	char *m_BlockSkyLight;	// += NumBlocks/2

	unsigned char m_HeightMap[16 * 16];

	unsigned int m_BlockTickNum;
	unsigned int m_BlockTickX, m_BlockTickY, m_BlockTickZ;

	void RemoveBlockEntity( cBlockEntity* a_BlockEntity );
	void AddBlockEntity( cBlockEntity* a_BlockEntity );
	cBlockEntity * GetBlockEntity( int a_X, int a_Y, int a_Z );

	void SpreadLightOfBlock(char* a_LightBuffer, int a_X, int a_Y, int a_Z, char a_Falloff);
	void SpreadLightOfBlockX(char* a_LightBuffer, int a_X, int a_Y, int a_Z);
	void SpreadLightOfBlockY(char* a_LightBuffer, int a_X, int a_Y, int a_Z);
	void SpreadLightOfBlockZ(char* a_LightBuffer, int a_X, int a_Y, int a_Z);

	void CreateBlockEntities(void);
};

typedef std::tr1::shared_ptr<cChunk> cChunkPtr;

typedef std::list<cChunkPtr> cChunkPtrList;





class cChunkCoords
{
public:
	int m_ChunkX;
	int m_ChunkY;
	int m_ChunkZ;
	
	cChunkCoords(int a_ChunkX, int a_ChunkY, int a_ChunkZ) : m_ChunkX(a_ChunkX), m_ChunkY(a_ChunkY), m_ChunkZ(a_ChunkZ) {}
	
	bool operator == (const cChunkCoords & a_Other)
	{
		return ((m_ChunkX == a_Other.m_ChunkX) && (m_ChunkY == a_Other.m_ChunkY) && (m_ChunkZ == a_Other.m_ChunkZ));
	}
} ;

typedef std::list<cChunkCoords> cChunkCoordsList;





#if C_CHUNK_USE_INLINE
	#include "cChunk.inl.h"
#endif




