
// WorldStorage.h

// Interfaces to the cWorldStorage class representing the chunk loading / saving thread
// This class decides which storage schema to use for saving; it queries all available schemas for loading
// Also declares the base class for all storage schemas, cWSSchema
// Helper serialization class cJsonChunkSerializer is declared as well





#pragma once
#ifndef WORLDSTORAGE_H_INCLUDED
#define WORLDSTORAGE_H_INCLUDED

#include "cChunk.h"
#include "cIsThread.h"
#include <json/json.h>





// fwd:
class cEntity;
class cBlockEntity;





/// Interface that all the world storage schemas need to implement
class cWSSchema ABSTRACT
{
public:
	cWSSchema(cWorld * a_World) : m_World(a_World) {}
	virtual ~cWSSchema() {}  // Force the descendants' destructors to be virtual
	
	virtual bool LoadChunk(const cChunkCoords & a_Chunk) = 0;
	virtual bool SaveChunk(const cChunkCoords & a_Chunk) = 0;
	virtual const AString GetName(void) const = 0;
	
protected:

	cWorld * m_World;
} ;

typedef std::list<cWSSchema *> cWSSchemaList;





/// Helper class for serializing a chunk into Json
class cJsonChunkSerializer :
	public cChunkDataCallback
{
public:

	cJsonChunkSerializer(void);
	
	Json::Value & GetRoot     (void) {return m_Root; }
	AString &     GetBlockData(void) {return m_BlockData; }
	bool          HasJsonData (void) const {return m_HasJsonData; }
	
protected:

	// BlockData is serialized into string
	AString m_BlockData;
	
	// Entities and BlockEntities are serialized to Json
	Json::Value m_Root;
	Json::Value m_AllChests;
	Json::Value m_AllFurnaces;
	Json::Value m_AllSigns;
	bool m_HasJsonData;
	
	// cChunkDataCallback overrides:
	virtual void BlockData  (const char * a_Data) override;
	virtual void Entity     (cEntity * a_Entity) override;
	virtual void BlockEntity(cBlockEntity * a_Entity) override;
} ;





/// The actual world storage class
class cWorldStorage :
	public cIsThread
{
	typedef cIsThread super;
	
public:

	cWorldStorage(void);
	~cWorldStorage();
	
	void QueueLoadChunk(int a_ChunkX, int a_ChunkY, int a_ChunkZ);  // Queues the chunk for loading; if not loaded, the chunk will be generated
	void QueueSaveChunk(int a_ChunkX, int a_ChunkY, int a_ChunkZ);
	
	void UnqueueLoad(const cChunkCoords & a_Chunk);
	void UnqueueSave(const cChunkCoords & a_Chunk);
	
	bool Start(cWorld * a_World, const AString & a_StorageSchemaName);  // Hide the cIsThread's Start() method, we need to provide args
	void WaitForFinish(void);
	
protected:

	cWorld * m_World;
	AString  m_StorageSchemaName;
	
	cCriticalSection m_CSLoadQueue;
	cChunkCoordsList    m_LoadQueue;
	
	cCriticalSection m_CSSaveQueue;
	cChunkCoordsList    m_SaveQueue;
	
	cEvent m_Event;  // Set when there's any addition to the queues
	
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
	
	/// Loads the chunk specified; returns true on success, false on failure
	bool LoadChunk(const cChunkCoords & a_Chunk);
} ;





#endif  // WORLDSTORAGE_H_INCLUDED




