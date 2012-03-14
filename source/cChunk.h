
#pragma once

#include "cEntity.h"
#include "ChunkDef.h"





#define C_CHUNK_USE_INLINE 1

// Do not touch
#if C_CHUNK_USE_INLINE
	#define __C_CHUNK_INLINE__ inline
#else
	#define __C_CHUNK_INLINE__
#endif





namespace Json
{
	class Value;
};





class cWorld;
class cFurnaceEntity;
class cPacket;
class cClientHandle;
class cServer;
class MTRand;
class cPlayer;
class cChunkMap;

typedef std::list<cClientHandle *>  cClientHandleList;





// This class is not to be used directly
// Instead, call actions on cChunkMap (such as cChunkMap::SetBlock() etc.)
class cChunk :
	public cChunkDef  // The inheritance is "misused" here only to inherit the functions and constants defined in cChunkDef
{
public:
	cChunk(int a_X, int a_Y, int a_Z, cChunkMap * a_ChunkMap, cWorld * a_World);
	~cChunk();

	bool IsValid(void) const {return m_IsValid; }  // Returns true if the chunk is valid (loaded / generated)
	void SetValid(bool a_SendToClients = true);   // Also wakes up all clients attached to this chunk to let them finish logging in
	bool IsDirty(void) const {return m_IsDirty; }  // Returns true if the chunk has changed since it was last saved
	bool HasLoadFailed(void) const {return m_HasLoadFailed; }  // Returns true if the chunk failed to load and hasn't been generated since then
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
	void MarkLoadFailed(void);  // Marks the chunk as failed to load. Ignored is the chunk is already valid
	
	/// Gets all chunk data, calls the a_Callback's methods for each data type
	void GetAllData(cChunkDataCallback & a_Callback);
	
	/// Sets all chunk data
	void SetAllData(
		const BLOCKTYPE * a_BlockTypes, 
		const BLOCKTYPE * a_BlockMeta,
		const BLOCKTYPE * a_BlockLight,
		const BLOCKTYPE * a_BlockSkyLight,
		const cChunkDef::HeightMap * a_HeightMap,
		cEntityList & a_Entities, 
		cBlockEntityList & a_BlockEntities
	);
	
	/// Copies m_BlockData into a_BlockTypes, only the block types
	void GetBlockTypes(BLOCKTYPE  * a_BlockTypes);
	
	/// Copies entire block data into a_BlockData, the entire 4 arrays (Type, Meta, Light, SkyLight)
	void GetBlockData(BLOCKTYPE  * a_BlockData);
	
	/// Returns true if there is a block entity at the coords specified
	bool HasBlockEntityAt(int a_BlockX, int a_BlockY, int a_BlockZ);
	
	/// Sets or resets the internal flag that prevents chunk from being unloaded
	void Stay(bool a_Stay = true);
	
	void Tick(float a_Dt, MTRand & a_TickRandom);

	int GetPosX() { return m_PosX; }
	int GetPosY() { return m_PosY; }
	int GetPosZ() { return m_PosZ; }
	cWorld * GetWorld() { return m_World; }

	// OBSOLETE void SendTo( cClientHandle * a_Client );

	void SetBlock( int a_X, int a_Y, int a_Z, BLOCKTYPE a_BlockType, BLOCKTYPE a_BlockMeta );
	void SetBlock( const Vector3i & a_BlockPos, BLOCKTYPE a_BlockType, BLOCKTYPE a_BlockMeta ) { SetBlock( a_BlockPos.x, a_BlockPos.y, a_BlockPos.z, a_BlockType, a_BlockMeta ); }
	void FastSetBlock(int a_RelX, int a_RelY, int a_RelZ, BLOCKTYPE a_BlockType, BLOCKTYPE a_BlockMeta );  // Doesn't force block updates on neighbors, use for simple changes such as grass growing etc.
	BLOCKTYPE GetBlock( int a_X, int a_Y, int a_Z );
	BLOCKTYPE GetBlock( int a_BlockIdx );
	
	void CollectPickupsByPlayer(cPlayer * a_Player);
	void UpdateSign(int a_PosX, int a_PosY, int a_PosZ, const AString & a_Line1, const AString & a_Line2, const AString & a_Line3, const AString & a_Line4);  // Also sends update packets to all clients in the chunk

	int  GetHeight( int a_X, int a_Z );

	void SendBlockTo( int a_X, int a_Y, int a_Z, cClientHandle* a_Client );

	/// Adds a client to the chunk; returns true if added, false if already there
	bool AddClient    (cClientHandle* a_Client );
	
	void RemoveClient (cClientHandle* a_Client );
	bool HasClient    (cClientHandle* a_Client );
	bool HasAnyClients(void);  // Returns true if theres any client in the chunk; false otherwise

	void AddEntity( cEntity * a_Entity);
	void RemoveEntity( cEntity * a_Entity);
	
	void UseBlockEntity(cPlayer * a_Player, int a_X, int a_Y, int a_Z);  // [x, y, z] in world block coords

	inline void RecalculateLighting() { m_bCalculateLighting = true; } // Recalculate lighting next tick

	void CalculateLighting(); // Recalculate right now
	void CalculateHeightmap();

	// Broadcasts to all clients that have loaded this chunk
	void Broadcast( const cPacket & a_Packet, cClientHandle * a_Exclude = NULL) {Broadcast(&a_Packet, a_Exclude); }
	void Broadcast( const cPacket * a_Packet, cClientHandle * a_Exclude = NULL);

	//   Loaded(blockdata, lightdata, blockentities, entities),
	//   Generated(blockdata, lightdata, blockentities, entities),
	//   GetBlockData(blockdatadest) etc.
	/*
	BLOCKTYPE * GetBlockTypes   (void) { return m_BlockTypes; }
	BLOCKTYPE * GetBlockMeta    (void) { return m_BlockMeta; }
	BLOCKTYPE * GetBlockLight   (void) { return m_BlockLight; }
	BLOCKTYPE * GetBlockSkyLight(void) { return m_BlockSkyLight; }
	*/
	
	void PositionToWorldPosition(int a_ChunkX, int a_ChunkY, int a_ChunkZ, int & a_X, int & a_Y, int & a_Z);
	Vector3i PositionToWorldPosition( const Vector3i & a_InChunkPos ) { return PositionToWorldPosition( a_InChunkPos.x, a_InChunkPos.y, a_InChunkPos.z ); }
	Vector3i PositionToWorldPosition( int a_ChunkX, int a_ChunkY, int a_ChunkZ );

	inline void MarkDirty(void)
	{
		m_IsDirty = true;
		m_IsSaving = false;
	}
	
	inline void SpreadBlockSkyLight(void) {SpreadLight(m_BlockSkyLight); }
	inline void SpreadBlockLight   (void) {SpreadLight(m_BlockLight); }
	
	inline BLOCKTYPE GetMeta(int a_RelX, int a_RelY, int a_RelZ)                   {return cChunkDef::GetNibble(m_BlockMeta, a_RelX, a_RelY, a_RelZ); }
	inline void      SetMeta(int a_RelX, int a_RelY, int a_RelZ, BLOCKTYPE a_Meta) {       cChunkDef::SetNibble(m_BlockMeta, a_RelX, a_RelY, a_RelZ, a_Meta); }

private:

	friend class cChunkMap;
	
	bool m_IsValid;  // True if the chunk is loaded / generated
	bool m_IsDirty;  // True if the chunk has changed since it was last saved
	bool m_IsSaving;  // True if the chunk is being saved
	bool m_HasLoadFailed;  // True if chunk failed to load and hasn't been generated yet since then
	
	cCriticalSection            m_CSBlockLists;
	std::deque< unsigned int >  m_ToTickBlocks;
	std::vector< unsigned int > m_PendingSendBlocks;
	
	// A critical section is not needed, because all chunk access is protected by its parent ChunkMap's csLayers
	cClientHandleList  m_LoadedByClient;
	cClientHandleList  m_UnloadQuery;
	cEntityList        m_Entities;
	cBlockEntityList   m_BlockEntities;
	
	/// Number of times the chunk has been requested to stay (by various cChunkStay objects); if zero, the chunk can be unloaded
	int m_StayCount;

	bool m_bCalculateLighting;

	int m_PosX, m_PosY, m_PosZ;
	cWorld *    m_World;
	cChunkMap * m_ChunkMap;

	// TODO: Make these pointers and don't allocate what isn't needed
	BLOCKTYPE m_BlockTypes   [cChunkDef::NumBlocks];
	BLOCKTYPE m_BlockMeta    [cChunkDef::NumBlocks / 2];
	BLOCKTYPE m_BlockLight   [cChunkDef::NumBlocks / 2];
	BLOCKTYPE m_BlockSkyLight[cChunkDef::NumBlocks / 2];

	cChunkDef::HeightMap m_HeightMap;

	unsigned int m_BlockTickNum;
	unsigned int m_BlockTickX, m_BlockTickY, m_BlockTickZ;

	void RemoveBlockEntity( cBlockEntity* a_BlockEntity );
	void AddBlockEntity( cBlockEntity* a_BlockEntity );
	cBlockEntity * GetBlockEntity( int a_X, int a_Y, int a_Z );
	cBlockEntity * GetBlockEntity( const Vector3i & a_BlockPos ) { return GetBlockEntity( a_BlockPos.x, a_BlockPos.y, a_BlockPos.z ); }

	void SpreadLightOfBlock(BLOCKTYPE * a_LightBuffer, int a_X, int a_Y, int a_Z, BLOCKTYPE a_Falloff);

	void CreateBlockEntities(void);
	
	// Makes a copy of the list
	cClientHandleList GetAllClients(void) const {return m_LoadedByClient; }

	void SpreadLight(BLOCKTYPE * a_LightBuffer);
};

typedef cChunk * cChunkPtr;

typedef std::list<cChunkPtr> cChunkPtrList;





#if C_CHUNK_USE_INLINE
	#include "cChunk.inl.h"
#endif




