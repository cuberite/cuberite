
// WorldStorage.h

// Interfaces to the cWorldStorage class representing the chunk loading / saving thread
// This class decides which storage schema to use for saving; it queries all available schemas for loading
// Also declares the base class for all storage schemas, cWSSchema





#pragma once
#ifndef WORLDSTORAGE_H_INCLUDED
#define WORLDSTORAGE_H_INCLUDED

#include "cChunk.h"
#include "cIsThread.h"





/// Interface that all the world storage schemas need to implement
class cWSSchema ABSTRACT
{
public:
	cWSSchema(cWorld * a_World) : m_World(a_World) {}
	virtual ~cWSSchema() {}  // Force the descendants' destructors to be virtual
	
	virtual bool LoadChunk(const cChunkPtr & a_Chunk) = 0;
	virtual bool SaveChunk(const cChunkPtr & a_Chunk) = 0;
	virtual const AString GetName(void) const = 0;
	
protected:

	cWorld * m_World;
} ;

typedef std::list<cWSSchema *> cWSSchemaList;





class cWorldStorage :
	public cIsThread
{
	typedef cIsThread super;
	
public:

	cWorldStorage(void);
	~cWorldStorage();
	
	void QueueLoadChunk(cChunkPtr & a_Chunk);  // Queues the chunk for loading; if not loaded, the chunk will be generated
	void QueueSaveChunk(cChunkPtr & a_Chunk);
	
	void UnqueueLoad(const cChunkPtr & a_Chunk);
	void UnqueueSave(const cChunkPtr & a_Chunk);
	
	bool Start(cWorld * a_World, const AString & a_StorageSchemaName);  // Hide the cIsThread's Start() method, we need to provide args
	void WaitForFinish(void);
	
protected:

	cWorld * m_World;
	AString  m_StorageSchemaName;
	
	cCriticalSection m_CSLoadQueue;
	cChunkPtrList    m_LoadQueue;
	
	cCriticalSection m_CSSaveQueue;
	cChunkPtrList    m_SaveQueue;
	
	cEvent m_Event;  // Set when there's any addition to the queues
	
	cWSSchemaList m_Schemas;
	cWSSchema * m_SaveSchema;
	
	void InitSchemas(void);
	
	virtual void Execute(void) override;
	bool LoadChunk(const cChunkPtr & a_Chunk);
} ;





#endif  // WORLDSTORAGE_H_INCLUDED




