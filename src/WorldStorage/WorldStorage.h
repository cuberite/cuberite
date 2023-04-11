
// WorldStorage.h

// Interfaces to the cWorldStorage class representing the chunk loading / saving thread
// This class decides which storage schema to use for saving; it queries all available schemas for loading
// Also declares the base class for all storage schemas, cWSSchema
// Helper serialization class cJsonChunkSerializer is declared as well





#pragma once

#include "ChunkDef.h"
#include "OSSupport/IsThread.h"
#include "OSSupport/Queue.h"




// fwd:
class cWorld;





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

using cWSSchemaList = std::list<std::shared_ptr<cWSSchema>>;





/** The actual world storage class */
class cWorldStorage:
	public cIsThread
{
	using Super = cIsThread;

	static constexpr int MAX_CHUNK_COORD = 0x08000000;

public:

	cWorldStorage();

	/** Queues a chunk to be loaded, asynchronously. */
	void QueueLoadChunk(int a_ChunkX, int a_ChunkZ);

	/** Queues a chunk to be saved, asynchronously. */
	void QueueSaveChunk(int a_ChunkX, int a_ChunkZ);

	/** Initializes the storage schemas, ready to be started. */
	void Initialize(cWorld & a_World, const AString & a_StorageSchemaName, int a_StorageCompressionFactor);
	void Stop(void);  // Hide the cIsThread's Stop() method, we need to signal the event
	void WaitForFinish(void);
	void WaitForLoadQueueEmpty(void);
	void WaitForSaveQueueEmpty(void);

	size_t GetLoadQueueLength(void);
	size_t GetSaveQueueLength(void);

protected:

	cWorld * m_World;

	cQueue<cChunkCoords> m_LoadQueue;
	cQueue<cChunkCoords> m_SaveQueue;

	/** All the storage schemas (all used for loading) */
	cWSSchemaList m_Schemas;

	/** The one storage schema used for saving */
	std::shared_ptr<cWSSchema> m_SaveSchema = nullptr;

	/** Set when there's any addition to the queues */
	cEvent m_Event;

	void InitSchemas(int a_StorageCompressionFactor, const AString & a_StorageSchemaName);

	void Execute(void) override;

	/** Loads one chunk from the queue (if any queued); returns true if there was a chunk in the queue to load */
	bool LoadOneChunk(void);

	/** Saves one chunk from the queue (if any queued); returns true if there was a chunk in the queue to save */
	bool SaveOneChunk(void);

	/** Loads the chunk specified; returns true on success, false on failure */
	bool LoadChunk(int a_ChunkX, int a_ChunkZ);
} ;




