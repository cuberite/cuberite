
#pragma once

#define C_CHUNK_USE_INLINE 1

// Do not touch
#if C_CHUNK_USE_INLINE
# define __C_CHUNK_INLINE__ inline
#else
# define __C_CHUNK_INLINE__
#endif

namespace Json
{
	class Value;
};





class cWorld;
class cFurnaceEntity;
class cPacket;
class cBlockEntity;
class cEntity;
class cClientHandle;
class cServer;
class MTRand;





class cChunk
{
public:
	cChunk(int a_X, int a_Y, int a_Z, cWorld* a_World);
	~cChunk();

	void Initialize();

	void Tick(float a_Dt, MTRand & a_TickRandom);

	int GetPosX() { return m_PosX; }
	int GetPosY() { return m_PosY; }
	int GetPosZ() { return m_PosZ; }
	cWorld* GetWorld() { return m_World; }

	void Send( cClientHandle* a_Client );
	void AsyncUnload( cClientHandle* a_Client );

	void SetBlock( int a_X, int a_Y, int a_Z, char a_BlockType, char a_BlockMeta );
	void FastSetBlock( int a_X, int a_Y, int a_Z, char a_BlockType, char a_BlockMeta );
	char GetBlock( int a_X, int a_Y, int a_Z );
	char GetBlock( int a_BlockIdx );
	cBlockEntity* GetBlockEntity( int a_X, int a_Y, int a_Z );
	void RemoveBlockEntity( cBlockEntity* a_BlockEntity );
	void AddBlockEntity( cBlockEntity* a_BlockEntity );

	char GetHeight( int a_X, int a_Z );

	void SendBlockTo( int a_X, int a_Y, int a_Z, cClientHandle* a_Client );

	void AddClient( cClientHandle* a_Client );
	void RemoveClient( cClientHandle* a_Client );

	std::list< cEntity* > & GetEntities();// { return m_Entities; }
	void AddEntity( cEntity & a_Entity );
	bool RemoveEntity( cEntity & a_Entity, cChunk* a_CalledFrom = 0 );
	void LockEntities();
	void UnlockEntities();

	const std::list< cClientHandle* > & GetClients();// { return m_LoadedByClient; }

	inline void RecalculateLighting() { m_bCalculateLighting = true; } // Recalculate lighting next tick
	inline void RecalculateHeightmap() { m_bCalculateHeightmap = true; } // Recalculate heightmap next tick
	void SpreadLight(char* a_LightBuffer);

	bool LoadFromDisk();

	// Broadcasts to all clients that have loaded this chunk
	void Broadcast( const cPacket & a_Packet, cClientHandle* a_Exclude = 0 ) const;

	char* pGetBlockData() { return m_BlockData; }
	char* pGetType() { return m_BlockType; }
	char* pGetMeta() { return m_BlockMeta; }
	char* pGetLight() { return m_BlockLight; }
	char* pGetSkyLight() { return m_BlockSkyLight; }

	char GetLight(char* a_Buffer, int a_BlockIdx);
	char GetLight(char* a_Buffer, int x, int y, int z);
	void SetLight(char* a_Buffer, int a_BlockIdx, char a_Light);
	void SetLight(char* a_Buffer, int x, int y, int z, char light);

	void PositionToWorldPosition(int a_ChunkX, int a_ChunkY, int a_ChunkZ, int & a_X, int & a_Y, int & a_Z);

	void AddTickBlockEntity( cFurnaceEntity* a_Entity );
	void RemoveTickBlockEntity( cFurnaceEntity* a_Entity );

	inline static unsigned int MakeIndex(int x, int y, int z )
	{
		if( x < 16 && x > -1 && y < 128 && y > -1 && z < 16 && z > -1 )
			return y + (z * 128) + (x * 128 * 16);
		return 0;
	}

	static const int c_NumBlocks = 16*128*16;
	static const int c_BlockDataSize = c_NumBlocks * 2 + (c_NumBlocks/2); // 2.5 * numblocks

	// Reference counting
	void AddReference();
	void RemoveReference();
	int GetReferenceCount();
private:
	struct sChunkState;
	sChunkState* m_pState;

	friend class cChunkMap; // So it has access to buffers and shit
	void LoadFromJson( const Json::Value & a_Value );
	void SaveToJson( Json::Value & a_Value );

	void CalculateLighting(); // Recalculate right now
	void CalculateHeightmap();
	void SpreadLightOfBlock(char* a_LightBuffer, int a_X, int a_Y, int a_Z, char a_Falloff);
	void SpreadLightOfBlockX(char* a_LightBuffer, int a_X, int a_Y, int a_Z);
	void SpreadLightOfBlockY(char* a_LightBuffer, int a_X, int a_Y, int a_Z);
	void SpreadLightOfBlockZ(char* a_LightBuffer, int a_X, int a_Y, int a_Z);

	void CreateBlockEntities();

	bool m_bCalculateLighting;
	bool m_bCalculateHeightmap;

	int m_PosX, m_PosY, m_PosZ;
	cWorld* m_World;

	char m_BlockData[c_BlockDataSize]; // Chunk data ready to be compressed and sent
	char *m_BlockType;		// Pointers to an element in m_BlockData
	char *m_BlockMeta;		// += NumBlocks
	char *m_BlockLight;		// += NumBlocks/2
	char *m_BlockSkyLight;	// += NumBlocks/2

	unsigned char m_HeightMap[16*16];

	unsigned int m_BlockTickNum;
	unsigned int m_BlockTickX, m_BlockTickY, m_BlockTickZ;

	cCriticalSection* m_EntitiesCriticalSection;
};






#if C_CHUNK_USE_INLINE
# include "cChunk.inl.h"
#endif