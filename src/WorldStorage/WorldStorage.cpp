
// WorldStorage.cpp

// Implements the cWorldStorage class representing the chunk loading / saving thread

// To add a new storage schema, implement a cWSSchema descendant and add it to cWorldStorage::InitSchemas()

#include "Globals.h"
#include "WorldStorage.h"
#include "WSSCompact.h"
#include "WSSAnvil.h"
#include "../World.h"
#include "../Generating/ChunkGenerator.h"
#include "../Entities/Entity.h"
#include "../BlockEntities/BlockEntity.h"




/// Example storage schema - forgets all chunks ;)
class cWSSForgetful :
	public cWSSchema
{
public:
	cWSSForgetful(cWorld * a_World) : cWSSchema(a_World) {}
	
protected:
	// cWSSchema overrides:
	virtual bool LoadChunk(const cChunkCoords & a_Chunk) override {return false; }
	virtual bool SaveChunk(const cChunkCoords & a_Chunk) override {return true; }
	virtual const AString GetName(void) const override {return "forgetful"; }
} ;





////////////////////////////////////////////////////////////////////////////////
// cWorldStorage:

cWorldStorage::cWorldStorage(void) :
	super("cWorldStorage"),
	m_World(NULL),
	m_SaveSchema(NULL)
{
}





cWorldStorage::~cWorldStorage()
{
	for (cWSSchemaList::iterator itr = m_Schemas.begin(); itr != m_Schemas.end(); ++itr)
	{
		delete *itr;
	}  // for itr - m_Schemas[]
}





bool cWorldStorage::Start(cWorld * a_World, const AString & a_StorageSchemaName, int a_StorageCompressionFactor)
{
	m_World = a_World;
	m_StorageSchemaName = a_StorageSchemaName;
	InitSchemas(a_StorageCompressionFactor);
	
	return super::Start();
}





void cWorldStorage::Stop(void)
{
	WaitForFinish();
}





void cWorldStorage::WaitForFinish(void)
{
	LOG("Waiting for the world storage to finish saving");
	
	{
		m_LoadQueue.Clear();
	}
	
	// Wait for the saving to finish:
	WaitForSaveQueueEmpty();
	
	// Wait for the thread to finish:
	m_ShouldTerminate = true;
	m_Event.Set();  // Wake up the thread if waiting
	super::Wait();
	LOG("World storage thread finished");
}





void cWorldStorage::WaitForLoadQueueEmpty(void)
{
	m_LoadQueue.BlockTillEmpty();
}





void cWorldStorage::WaitForSaveQueueEmpty(void)
{
	m_SaveQueue.BlockTillEmpty();
}





size_t cWorldStorage::GetLoadQueueLength(void)
{
	return m_LoadQueue.Size();
}





size_t cWorldStorage::GetSaveQueueLength(void)
{
	return m_SaveQueue.Size();
}





void cWorldStorage::QueueLoadChunk(int a_ChunkX, int a_ChunkY, int a_ChunkZ, bool a_Generate)
{
	m_LoadQueue.EnqueueItem(sChunkLoad(a_ChunkX, a_ChunkY, a_ChunkZ, a_Generate));
	m_Event.Set();
}





void cWorldStorage::QueueSaveChunk(int a_ChunkX, int a_ChunkY, int a_ChunkZ)
{
	m_SaveQueue.EnqueueItemIfNotPresent(cChunkCoords(a_ChunkX, a_ChunkY, a_ChunkZ));
	m_Event.Set();
}





void cWorldStorage::UnqueueLoad(int a_ChunkX, int a_ChunkY, int a_ChunkZ)
{
	m_LoadQueue.Remove(sChunkLoad(a_ChunkX, a_ChunkY, a_ChunkZ, true));
}





void cWorldStorage::UnqueueSave(const cChunkCoords & a_Chunk)
{
	m_SaveQueue.Remove(a_Chunk);
}





void cWorldStorage::InitSchemas(int a_StorageCompressionFactor)
{
	// The first schema added is considered the default
	m_Schemas.push_back(new cWSSAnvil    (m_World, a_StorageCompressionFactor));
	m_Schemas.push_back(new cWSSCompact  (m_World, a_StorageCompressionFactor));
	m_Schemas.push_back(new cWSSForgetful(m_World));
	// Add new schemas here
	
	if (NoCaseCompare(m_StorageSchemaName, "default") == 0)
	{
		m_SaveSchema = m_Schemas.front();
		return;
	}
	for (cWSSchemaList::iterator itr = m_Schemas.begin(); itr != m_Schemas.end(); ++itr)
	{
		if (NoCaseCompare((*itr)->GetName(), m_StorageSchemaName) == 0)
		{
			m_SaveSchema = *itr;
			return;
		}
	}  // for itr - m_Schemas[]
	
	// Unknown schema selected, let the admin know:
	LOGWARNING("Unknown storage schema name \"%s\". Using default (\"%s\"). Available schemas:",
		m_StorageSchemaName.c_str(), m_SaveSchema->GetName().c_str()
	);
	for (cWSSchemaList::iterator itr = m_Schemas.begin(); itr != m_Schemas.end(); ++itr)
	{
		LOGWARNING("\t\"%s\"", (*itr)->GetName().c_str());
	}
	m_SaveSchema = m_Schemas.front();
}





void cWorldStorage::Execute(void)
{
	while (!m_ShouldTerminate)
	{
		m_Event.Wait();
		// Process both queues until they are empty again:
		bool Success;
		do
		{
			if (m_ShouldTerminate)
			{
				return;
			}
			
			Success = LoadOneChunk();
			Success |= SaveOneChunk();
		} while (Success);
	}
}





bool cWorldStorage::LoadOneChunk(void)
{
	sChunkLoad ToLoad(0, 0, 0, false);
	bool ShouldLoad = m_LoadQueue.TryDequeueItem(ToLoad);
	if (ShouldLoad && !LoadChunk(ToLoad.m_ChunkX, ToLoad.m_ChunkY, ToLoad.m_ChunkZ))
	{
		if (ToLoad.m_Generate)
		{
			// The chunk couldn't be loaded, generate it:
			m_World->GetGenerator().QueueGenerateChunk(ToLoad.m_ChunkX, ToLoad.m_ChunkY, ToLoad.m_ChunkZ);
		}
		else
		{
			// TODO: Notify the world that the load has failed:
			// m_World->ChunkLoadFailed(ToLoad.m_ChunkX, ToLoad.m_ChunkY, ToLoad.m_ChunkZ);
		}
	}
	return ShouldLoad;
}





bool cWorldStorage::SaveOneChunk(void)
{
	cChunkCoords ToSave(0, 0, 0);
	bool ShouldSave = m_SaveQueue.TryDequeueItem(ToSave);
	if (ShouldSave && m_World->IsChunkValid(ToSave.m_ChunkX, ToSave.m_ChunkZ))
	{
		m_World->MarkChunkSaving(ToSave.m_ChunkX, ToSave.m_ChunkZ);
		if (m_SaveSchema->SaveChunk(ToSave))
		{
			m_World->MarkChunkSaved(ToSave.m_ChunkX, ToSave.m_ChunkZ);
		}
	}
	return ShouldSave;
}





bool cWorldStorage::LoadChunk(int a_ChunkX, int a_ChunkY, int a_ChunkZ)
{
	if (m_World->IsChunkValid(a_ChunkX, a_ChunkZ))
	{
		// Already loaded (can happen, since the queue is async)
		return true;
	}
	
	cChunkCoords Coords(a_ChunkX, a_ChunkY, a_ChunkZ);

	// First try the schema that is used for saving
	if (m_SaveSchema->LoadChunk(Coords))
	{
		return true;
	}
	
	// If it didn't have the chunk, try all the other schemas:
	for (cWSSchemaList::iterator itr = m_Schemas.begin(); itr != m_Schemas.end(); ++itr)
	{
		if (((*itr) != m_SaveSchema) && (*itr)->LoadChunk(Coords))
		{
			return true;
		}
	}
	
	// Notify the chunk owner that the chunk failed to load (sets cChunk::m_HasLoadFailed to true):
	m_World->ChunkLoadFailed(a_ChunkX, a_ChunkY, a_ChunkZ);
	
	return false;
}





