
// WorldStorage.cpp

// Implements the cWorldStorage class representing the chunk loading / saving thread

// To add a new storage schema, implement a cWSSchema descendant and add it to cWorldStorage::InitSchemas()

#include "Globals.h"
#include "WorldStorage.h"
#include "WSSCompact.h"
#include "cWorld.h"
#include "cChunkGenerator.h"
#include "cEntity.h"
#include "cBlockEntity.h"
#include "BlockID.h"





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





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cJsonChunkSerializer:

cJsonChunkSerializer::cJsonChunkSerializer(void) :
	m_HasJsonData(false)
{
	m_Root["Chests"]   = m_AllChests;
	m_Root["Furnaces"] = m_AllFurnaces;
	m_Root["Signs"]    = m_AllSigns;
}





void cJsonChunkSerializer::BlockData(const char * a_Data)
{
	m_BlockData.assign(a_Data, cChunk::c_BlockDataSize);
}





void cJsonChunkSerializer::Entity(cEntity * a_Entity)
{
	// TODO: a_Entity->SaveToJson(m_Root);
}





void cJsonChunkSerializer::BlockEntity(cBlockEntity * a_BlockEntity)
{
	const char * SaveInto = NULL;
	switch (a_BlockEntity->GetBlockType())
	{
		case E_BLOCK_CHEST:     SaveInto = "Chests";   break;
		case E_BLOCK_FURNACE:   SaveInto = "Furnaces"; break;
		case E_BLOCK_SIGN_POST: SaveInto = "Signs";    break;
		case E_BLOCK_WALLSIGN:  SaveInto = "Signs";    break;
		
		default:
		{
			assert(!"Unhandled blocktype in BlockEntities list while saving to JSON");
			break;
		}
	}  // switch (BlockEntity->GetBlockType())
	if (SaveInto == NULL)
	{
		return;
	}
	
	Json::Value val;
	a_BlockEntity->SaveToJson(val);
	m_Root[SaveInto].append(val);
	m_HasJsonData = true;
}





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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
	m_LoadQueue.clear();
	m_SaveQueue.clear();
}





bool cWorldStorage::Start(cWorld * a_World, const AString & a_StorageSchemaName)
{
	m_World = a_World;
	m_StorageSchemaName = a_StorageSchemaName;
	InitSchemas();
	
	return super::Start();
}





void cWorldStorage::WaitForFinish(void)
{
	LOG("Waiting for the world storage to finish saving");
	
	{
		// Cancel all loading requests:
		cCSLock Lock(m_CSLoadQueue);
		m_LoadQueue.clear();
	}
	
	// Wait for the thread to finish:
	mShouldTerminate = true;
	m_Event.Set();
	super::Wait();
}





void cWorldStorage::QueueLoadChunk(int a_ChunkX, int a_ChunkZ)
{
	// Queues the chunk for loading; if not loaded, the chunk will be generated
	cCSLock Lock(m_CSLoadQueue);
	m_LoadQueue.remove   (cChunkCoords(a_ChunkX, a_ChunkZ));  // Don't add twice
	m_LoadQueue.push_back(cChunkCoords(a_ChunkX, a_ChunkZ));
	m_Event.Set();
}





void cWorldStorage::QueueSaveChunk(int a_ChunkX, int a_ChunkZ)
{
	cCSLock Lock(m_CSSaveQueue);
	m_SaveQueue.remove   (cChunkCoords(a_ChunkX, a_ChunkZ));  // Don't add twice
	m_SaveQueue.push_back(cChunkCoords(a_ChunkX, a_ChunkZ));
	m_Event.Set();
}





void cWorldStorage::UnqueueLoad(const cChunkCoords & a_Chunk)
{
	cCSLock Lock(m_CSLoadQueue);
	m_LoadQueue.remove(a_Chunk);
}





void cWorldStorage::UnqueueSave(const cChunkCoords & a_Chunk)
{
	cCSLock Lock(m_CSSaveQueue);
	m_SaveQueue.remove(a_Chunk);
}





void cWorldStorage::InitSchemas(void)
{
	// The first schema added is considered the default
	m_Schemas.push_back(new cWSSCompact(m_World));
	m_Schemas.push_back(new cWSSForgetful(m_World));
	// Add new schemas here
	
	if (m_StorageSchemaName == "Default")
	{
		m_SaveSchema = m_Schemas.front();
		return;
	}
	for (cWSSchemaList::iterator itr = m_Schemas.begin(); itr != m_Schemas.end(); ++itr)
	{
		if ((*itr)->GetName() == m_StorageSchemaName)
		{
			m_SaveSchema = *itr;
			return;
		}
	}  // for itr - m_Schemas[]
	
	// Unknown schema selected, let the admin know:
	LOGWARNING("Unknown storage schema name \"%s\". Using default. Available schemas:", m_StorageSchemaName.c_str());
	for (cWSSchemaList::iterator itr = m_Schemas.begin(); itr != m_Schemas.end(); ++itr)
	{
		LOGWARNING("\t\"%s\"", (*itr)->GetName().c_str());
	}
	m_SaveSchema = m_Schemas.front();
}





void cWorldStorage::Execute(void)
{
	while (!mShouldTerminate)
	{
		m_Event.Wait();
		
		// Process both queues until they are empty again:
		bool HasMore;
		do
		{
			HasMore = false;
			if (mShouldTerminate)
			{
				return;
			}
			
			HasMore = LoadOneChunk();
			HasMore = HasMore | SaveOneChunk();
		} while (HasMore);
	}
}





bool cWorldStorage::LoadOneChunk(void)
{
	cChunkCoords ToLoad(0, 0);
	bool HasMore;
	bool ShouldLoad = false;
	{
		cCSLock Lock(m_CSLoadQueue);
		if (m_LoadQueue.size() > 0)
		{
			ToLoad = m_LoadQueue.front();
			m_LoadQueue.pop_front();
			ShouldLoad = true;
		}
		HasMore = (m_LoadQueue.size() > 0);
	}
	if (ShouldLoad && !LoadChunk(ToLoad))
	{
		// The chunk couldn't be loaded, generate it:
		m_World->GetGenerator().GenerateChunk(ToLoad.m_ChunkX, ToLoad.m_ChunkZ);
	}
	return HasMore;
}





bool cWorldStorage::SaveOneChunk(void)
{
	cChunkCoords Save(0, 0);
	bool HasMore;
	bool ShouldSave = false;
	{
		cCSLock Lock(m_CSSaveQueue);
		if (m_SaveQueue.size() > 0)
		{
			Save = m_SaveQueue.front();
			m_SaveQueue.pop_front();
			ShouldSave = true;
		}
		HasMore = (m_SaveQueue.size() > 0);
	}
	if (ShouldSave)
	{
		m_World->MarkChunkSaving(Save.m_ChunkX, 0, Save.m_ChunkZ);
		if (m_SaveSchema->SaveChunk(Save))
		{
			m_World->MarkChunkSaved(Save.m_ChunkX, 0, Save.m_ChunkZ);
		}
		else
		{
			LOGWARNING("Cannot save chunk [%d, %d]", Save.m_ChunkX, Save.m_ChunkZ);
		}
	}
	return HasMore;
}





bool cWorldStorage::LoadChunk(const cChunkCoords & a_Chunk)
{
	if (m_World->IsChunkValid(a_Chunk.m_ChunkX, 0, a_Chunk.m_ChunkZ))
	{
		// Already loaded (can happen, since the queue is async)
		return true;
	}
	
	if (m_SaveSchema->LoadChunk(a_Chunk))
	{
		return true;
	}
	
	for (cWSSchemaList::iterator itr = m_Schemas.begin(); itr != m_Schemas.end(); ++itr)
	{
		if (((*itr) != m_SaveSchema) && (*itr)->LoadChunk(a_Chunk))
		{
			return true;
		}
	}
	
	return false;
}





