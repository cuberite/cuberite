
// WorldStorage.h

// Interfaces to the cWorldStorage class representing the chunk loading / saving thread
// This class decides which storage schema to use for saving; it queries all available schemas for loading
// Also declares the base class for all storage schemas, cWSSchema
// Helper serialization class cJsonChunkSerializer is declared as well





#pragma once
#ifndef WORLDSTORAGE_H_INCLUDED
#define WORLDSTORAGE_H_INCLUDED

#include "../ChunkDef.h"
#include "../OSSupport/IsThread.h"
#include "../OSSupport/Queue.h"





// fwd:
class cWorld;

typedef cQueue<cChunkCoordsWithCallback> cChunkCoordsQueue;





/** Interface that all the world storage schemas need to implement */
class cWSSchema abstract
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





/** The actual world storage class */
class cWorldStorage :
	public cIsThread
{
	typedef cIsThread super;
	
public:

	cWorldStorage(void);
	~cWorldStorage();
	
	void QueueLoadChunk(int a_ChunkX, int a_ChunkZ, cChunkCoordCallback * a_Callback = nullptr);
	void QueueSaveChunk(int a_ChunkX, int a_ChunkZ, cChunkCoordCallback * a_Callback = nullptr);
	
	void UnqueueLoad(int a_ChunkX, int a_ChunkZ);
	void UnqueueSave(const cChunkCoords & a_Chunk);
	
	bool Start(cWorld * a_World, const AString & a_StorageSchemaName, int a_StorageCompressionFactor);  // Hide the cIsThread's Start() method, we need to provide args
	void Stop(void);  // Hide the cIsThread's Stop() method, we need to signal the event
	void WaitForFinish(void);
	void WaitForLoadQueueEmpty(void);
	void WaitForSaveQueueEmpty(void);
	
	size_t GetLoadQueueLength(void);
	size_t GetSaveQueueLength(void);
	
protected:

	cWorld * m_World;
	AString  m_StorageSchemaName;

	cChunkCoordsQueue  m_LoadQueue;
	cChunkCoordsQueue m_SaveQueue;
	
	/** All the storage schemas (all used for loading) */
	cWSSchemaList m_Schemas;
	
	/** The one storage schema used for saving */
	cWSSchema * m_SaveSchema;

	/** Set when there's any addition to the queues */
	cEvent m_Event;

	
	/** Loads the chunk specified; returns true on success, false on failure */
	bool LoadChunk(int a_ChunkX, int a_ChunkZ);

	void InitSchemas(int a_StorageCompressionFactor);
	
	virtual void Execute(void) override;
	
	/** Loads one chunk from the queue (if any queued); returns true if there are more chunks in the load queue */
	bool LoadOneChunk(void);
	
	/** Saves one chunk from the queue (if any queued); returns true if there are more chunks in the save queue */
	bool SaveOneChunk(void);
} ;





#endif  // WORLDSTORAGE_H_INCLUDED




