
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

typedef cQueue<cChunkCoords> cChunkCoordsQueue;





/// Interface that all the world storage schemas need to implement
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
	
	bool Start(cWorld * a_World, const AString & a_StorageSchemaName, int a_StorageCompressionFactor);  // Hide the cIsThread's Start() method, we need to provide args
	void Stop(void);  // Hide the cIsThread's Stop() method, we need to signal the event
	void WaitForFinish(void);
	void WaitForLoadQueueEmpty(void);
	void WaitForSaveQueueEmpty(void);
	
	size_t GetLoadQueueLength(void);
	size_t GetSaveQueueLength(void);
	
protected:

	struct sChunkLoad
	{
		int m_ChunkX;
		int m_ChunkY;
		int m_ChunkZ;
		bool m_Generate;  // If true, the chunk will be generated if it cannot be loaded
		
		sChunkLoad(int a_ChunkX, int a_ChunkY, int a_ChunkZ, bool a_Generate) : m_ChunkX(a_ChunkX), m_ChunkY(a_ChunkY), m_ChunkZ(a_ChunkZ), m_Generate(a_Generate) {}

		bool operator==(const sChunkLoad other) const
		{
			return this->m_ChunkX == other.m_ChunkX &&
				this->m_ChunkY == other.m_ChunkY &&
				this->m_ChunkZ == other.m_ChunkZ;
		}
	} ;

	struct FuncTable
	{
		static void Delete(sChunkLoad) {};
		static void Combine(sChunkLoad & a_orig, const sChunkLoad a_new)
		{
			a_orig.m_Generate |= a_new.m_Generate;
		};
	};

	typedef cQueue<sChunkLoad, FuncTable> sChunkLoadQueue;
	
	cWorld * m_World;
	AString  m_StorageSchemaName;

	sChunkLoadQueue  m_LoadQueue;
	cChunkCoordsQueue m_SaveQueue;
	
	/// All the storage schemas (all used for loading)
	cWSSchemaList m_Schemas;
	
	/// The one storage schema used for saving
	cWSSchema *   m_SaveSchema;
	
	void InitSchemas(int a_StorageCompressionFactor);
	
	virtual void Execute(void) override;
	
	cEvent m_Event;       // Set when there's any addition to the queues

	/// Loads one chunk from the queue (if any queued); returns true if there are more chunks in the load queue
	bool LoadOneChunk(void);
	
	/// Saves one chunk from the queue (if any queued); returns true if there are more chunks in the save queue
	bool SaveOneChunk(void);
} ;





#endif  // WORLDSTORAGE_H_INCLUDED




