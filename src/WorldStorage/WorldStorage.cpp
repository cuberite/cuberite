
// WorldStorage.cpp

// Implements the cWorldStorage class representing the chunk loading / saving thread

// To add a new storage schema, implement a cWSSchema descendant and add it to cWorldStorage::InitSchemas()

#include "Globals.h"
#include "WorldStorage.h"
#include "WSSAnvil.h"
#include "../World.h"
#include "../Generating/ChunkGenerator.h"
#include "../Entities/Entity.h"
#include "../BlockEntities/BlockEntity.h"




/** Example storage schema - forgets all chunks */
class cWSSForgetful :
	public cWSSchema
{
public:

	using cWSSchema::cWSSchema;

protected:
	// cWSSchema overrides:
	bool LoadChunk(const cChunkCoords & a_Chunk) override { return false; }
	bool SaveChunk(const cChunkCoords & a_Chunk) override { return true; }
	const AString GetName(void) const override { return "forgetful"; }
} ;





////////////////////////////////////////////////////////////////////////////////
// cWorldStorage:

cWorldStorage::cWorldStorage(void) :
	Super("World Storage Executor")
{
}





void cWorldStorage::Initialize(cWorld & a_World, const AString & a_StorageSchemaName, int a_StorageCompressionFactor)
{
	m_World = &a_World;
	InitSchemas(a_StorageCompressionFactor, a_StorageSchemaName);
}





void cWorldStorage::Stop(void)
{
	WaitForFinish();
}





void cWorldStorage::WaitForFinish(void)
{
	LOGD("Waiting for the world storage to finish saving");

	{
		m_LoadQueue.Clear();
	}

	// Wait for the saving to finish:
	WaitForSaveQueueEmpty();

	// Wait for the thread to finish:
	m_ShouldTerminate = true;
	m_Event.Set();  // Wake up the thread if waiting
	Super::Stop();
	LOGD("World storage thread finished");
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





void cWorldStorage::QueueLoadChunk(int a_ChunkX, int a_ChunkZ)
{
	ASSERT((a_ChunkX > -MAX_CHUNK_COORD) && (a_ChunkX < MAX_CHUNK_COORD));
	ASSERT((a_ChunkZ > -MAX_CHUNK_COORD) && (a_ChunkZ < MAX_CHUNK_COORD));
	ASSERT(m_World->IsChunkQueued(a_ChunkX, a_ChunkZ));

	m_LoadQueue.EnqueueItem({ a_ChunkX, a_ChunkZ });
	m_Event.Set();
}





void cWorldStorage::QueueSaveChunk(int a_ChunkX, int a_ChunkZ)
{
	ASSERT(m_World->IsChunkValid(a_ChunkX, a_ChunkZ));

	m_SaveQueue.EnqueueItem({ a_ChunkX, a_ChunkZ });
	m_Event.Set();
}





void cWorldStorage::InitSchemas(int a_StorageCompressionFactor, const AString & a_StorageSchemaName)
{
	// The first schema added is considered the default
	m_Schemas.push_back(std::make_unique<cWSSAnvil>(m_World, a_StorageCompressionFactor));
	m_Schemas.push_back(std::make_unique<cWSSForgetful>(m_World));
	// Todo: Add new schemas here

	if (NoCaseCompare(a_StorageSchemaName, "default") == 0)
	{
		m_SaveSchema = m_Schemas.front();
		return;
	}

	for (const auto & Schema : m_Schemas)
	{
		if (NoCaseCompare(Schema->GetName(), a_StorageSchemaName) == 0)
		{
			m_SaveSchema = Schema;
			return;
		}
	}  // for itr - m_Schemas[]

	// Unknown schema selected, let the admin know:
	LOGWARNING("Unknown storage schema name \"%s\". Using default (\"%s\"). Available schemas:",
		a_StorageSchemaName.c_str(), m_SaveSchema->GetName().c_str()
	);
	for (const auto & Schema : m_Schemas)
	{
		LOGWARNING("\t\"%s\"", Schema->GetName().c_str());
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
	// Dequeue an item, bail out if there's none left:
	cChunkCoords ToLoad(0, 0);
	if (!m_LoadQueue.TryDequeueItem(ToLoad))
	{
		return false;
	}

	// Load the chunk:
	LoadChunk(ToLoad.m_ChunkX, ToLoad.m_ChunkZ);

	return true;
}





bool cWorldStorage::SaveOneChunk(void)
{
	// Dequeue one chunk to save:
	cChunkCoords ToSave(0, 0);
	if (!m_SaveQueue.TryDequeueItem(ToSave))
	{
		return false;
	}

	// Save the chunk, if it's valid:
	if (m_World->IsChunkValid(ToSave.m_ChunkX, ToSave.m_ChunkZ))
	{
		m_World->MarkChunkSaving(ToSave.m_ChunkX, ToSave.m_ChunkZ);
		if (m_SaveSchema->SaveChunk(cChunkCoords(ToSave.m_ChunkX, ToSave.m_ChunkZ)))
		{
			m_World->MarkChunkSaved(ToSave.m_ChunkX, ToSave.m_ChunkZ);
		}
	}

	return true;
}





bool cWorldStorage::LoadChunk(int a_ChunkX, int a_ChunkZ)
{
	ASSERT(m_World->IsChunkQueued(a_ChunkX, a_ChunkZ));

	cChunkCoords Coords(a_ChunkX, a_ChunkZ);

	// First try the schema that is used for saving
	if (m_SaveSchema->LoadChunk(Coords))
	{
		return true;
	}

	// If it didn't have the chunk, try all the other schemas:
	for (const auto & Schema : m_Schemas)
	{
		if ((Schema != m_SaveSchema) && Schema->LoadChunk(Coords))
		{
			return true;
		}
	}

	// Notify the chunk owner that the chunk failed to load (sets cChunk::m_HasLoadFailed to true):
	m_World->ChunkLoadFailed(a_ChunkX, a_ChunkZ);

	return false;
}





