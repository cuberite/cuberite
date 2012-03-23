
// WorldStorage.h

// Interfaces to the cWorldStorage class representing the chunk loading / saving thread
// This class decides which storage schema to use for saving; it queries all available schemas for loading
// Also declares the base class for all storage schemas, cWSSchema
// Helper serialization class cJsonChunkSerializer is declared as well





#pragma once
#ifndef WORLDSTORAGE_H_INCLUDED
#define WORLDSTORAGE_H_INCLUDED

#include "ChunkDef.h"
#include "cIsThread.h"
#include <json/json.h>





/** Interface between cWorld and cWorldStorage, contains all calls into cWorld that cWorldStorage needs
Defining this as an interface lets us re-use the cWorldStorage outside of MC-Server's main executable,
for example for tools such as storage converters or chunk analytics
*/
class cWSInterface
{
public:
	/// Asks the world if the chunk is fully valid
	virtual bool WSIIsChunkValid(int a_ChunkX, int a_ChunkY, int a_ChunkZ) = 0;
	
	/// Marks the chunk as being saved
	virtual void WSIMarkChunkSaving(int a_ChunkX, int a_ChunkY, int a_ChunkZ) = 0;
	
	/// Marks the chunk as having been saved (if there was no change since the last MarkSaving)
	virtual void WSIMarkChunkSaved(int a_ChunkX, int a_ChunkY, int a_ChunkZ) = 0;
	
	/// Marks the chunk as unable to load
	virtual void WSIChunkLoadFailed(int a_ChunkX, int a_ChunkY, int a_ChunkZ) = 0;
	
	/// Called when chunk generation has been specified for a chunk that cannot be loaded
	virtual void WSIGenerateChunk(int a_ChunkX, int a_ChunkY, int a_ChunkZ) = 0;
	
	/// Marks the chunk as having been saved (if there was no change since the last MarkSaving)
	virtual bool WSIGetChunkData(int a_ChunkX, int a_ChunkY, int a_ChunkZ, cChunkDataCallback & a_Callback) = 0;

	/// Gets the folder where the world is saved
	virtual AString WSIGetFolder(void) = 0;

	virtual void WSIChunkDataLoaded(
		int a_ChunkX, int a_ChunkY, int a_ChunkZ, 
		const BLOCKTYPE * a_BlockTypes,
		const BLOCKTYPE * a_BlockMeta,
		const BLOCKTYPE * a_BlockLight,
		const BLOCKTYPE * a_BlockSkyLight,
		const cChunkDef::HeightMap * a_HeightMap,
		cEntityList & a_Entities,
		cBlockEntityList & a_BlockEntities
	) = 0;
} ;





/// Interface that all the world storage schemas need to implement
class cWSSchema abstract
{
public:
	cWSSchema(cWSInterface * a_WSI) : m_WSI(a_WSI) {}
	virtual ~cWSSchema() {}  // Force the descendants' destructors to be virtual
	
	virtual bool LoadChunk(const cChunkCoords & a_Chunk) = 0;
	virtual bool SaveChunk(const cChunkCoords & a_Chunk) = 0;
	virtual const AString GetName(void) const = 0;
	
protected:

	cWSInterface * m_WSI;
} ;

typedef std::list<cWSSchema *> cWSSchemaList;





/// Helper class for serializing a chunk into Json
class cJsonChunkSerializer :
	public cChunkDataCollector
{
public:

	cJsonChunkSerializer(void);
	
	Json::Value & GetRoot     (void) {return m_Root; }
	BLOCKTYPE *   GetBlockData(void) {return m_BlockData; }
	bool          HasJsonData (void) const {return m_HasJsonData; }
	
protected:
	
	// NOTE: block data is serialized into inherited cChunkDataCollector's m_BlockData[] array
	
	// Entities and BlockEntities are serialized to Json
	Json::Value m_Root;
	Json::Value m_AllChests;
	Json::Value m_AllFurnaces;
	Json::Value m_AllSigns;
	bool m_HasJsonData;
	
	// cChunkDataCollector overrides:
	virtual void Entity       (cEntity *      a_Entity) override;
	virtual void BlockEntity  (cBlockEntity * a_Entity) override;
} ;





/// The actual world storage class
class cWorldStorage :
	public cIsThread
{
	typedef cIsThread super;
	
public:

	cWorldStorage(void);
	~cWorldStorage();
	
	void QueueLoadChunk(int a_ChunkX, int a_ChunkY, int a_ChunkZ, bool a_Generate);  // Queues the chunk for loading; if not loaded, the chunk will be generated if a_Generate is true
	void QueueSaveChunk(int a_ChunkX, int a_ChunkY, int a_ChunkZ);
	
	/// Loads the chunk specified; returns true on success, false on failure
	bool LoadChunk(int a_ChunkX, int a_ChunkY, int a_ChunkZ);

	void UnqueueLoad(int a_ChunkX, int a_ChunkY, int a_ChunkZ);
	void UnqueueSave(const cChunkCoords & a_Chunk);
	
	bool Start(cWSInterface * a_WSI, const AString & a_StorageSchemaName);  // Hide the cIsThread's Start() method, we need to provide args
	void WaitForFinish(void);
	void WaitForQueuesEmpty(void);
	
	int GetLoadQueueLength(void);
	int GetSaveQueueLength(void);
	
protected:

	struct sChunkLoad
	{
		int m_ChunkX;
		int m_ChunkY;
		int m_ChunkZ;
		bool m_Generate;  // If true, the chunk will be generated if it cannot be loaded
		
		sChunkLoad(int a_ChunkX, int a_ChunkY, int a_ChunkZ, bool a_Generate) : m_ChunkX(a_ChunkX), m_ChunkY(a_ChunkY), m_ChunkZ(a_ChunkZ), m_Generate(a_Generate) {}
	} ;
	
	typedef std::list<sChunkLoad> sChunkLoadQueue;
	
	cWSInterface * m_WSI;
	
	AString  m_StorageSchemaName;
	
	// Both queues are locked by the same CS
	cCriticalSection m_CSQueues;
	sChunkLoadQueue  m_LoadQueue;
	cChunkCoordsList m_SaveQueue;
	
	cEvent m_Event;       // Set when there's any addition to the queues
	cEvent m_evtRemoved;  // Set when an item has been removed from the queue, either by the worker thread or the Unqueue methods
	
	/// All the storage schemas (all used for loading)
	cWSSchemaList m_Schemas;
	
	/// The one storage schema used for saving
	cWSSchema *   m_SaveSchema;
	
	void InitSchemas(void);
	
	virtual void Execute(void) override;
	
	/// Loads one chunk from the queue (if any queued); returns true if there are more chunks in the load queue
	bool LoadOneChunk(void);
	
	/// Saves one chunk from the queue (if any queued); returns true if there are more chunks in the save queue
	bool SaveOneChunk(void);
} ;





#endif  // WORLDSTORAGE_H_INCLUDED




