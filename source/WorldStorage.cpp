
// WorldStorage.cpp

// Implements the cWorldStorage class representing the chunk loading / saving thread

// To add a new storage schema, implement a cWSSchema descendant and add it to cWorldStorage::InitSchemas()

#include "Globals.h"
#include "WorldStorage.h"
#include "WSSCompact.h"
#include "cWorld.h"
#include "cChunkGenerator.h"





/// Example storage schema - forgets all chunks ;)
class cWSSForgetful :
	public cWSSchema
{
public:
	cWSSForgetful(cWorld * a_World) : cWSSchema(a_World) {}
	
protected:
	// cWSSchema overrides:
	virtual bool LoadChunk(const cChunkPtr & a_Chunk) override {return false; }
	virtual bool SaveChunk(const cChunkPtr & a_Chunk) override {return true; }
	virtual const AString GetName(void) const override {return "forgetful"; }
} ;





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





void cWorldStorage::QueueLoadChunk(cChunkPtr & a_Chunk)
{
	// Queues the chunk for loading; if not loaded, the chunk will be generated
	cCSLock Lock(m_CSLoadQueue);
	m_LoadQueue.remove(a_Chunk);  // Don't add twice
	m_LoadQueue.push_back(a_Chunk);
	m_Event.Set();
}





void cWorldStorage::QueueSaveChunk(cChunkPtr & a_Chunk)
{
	cCSLock Lock(m_CSSaveQueue);
	m_SaveQueue.remove(a_Chunk);  // Don't add twice
	m_SaveQueue.push_back(a_Chunk);
	m_Event.Set();
}





void cWorldStorage::UnqueueLoad(const cChunkPtr & a_Chunk)
{
	cCSLock Lock(m_CSLoadQueue);
	m_LoadQueue.remove(a_Chunk);
}





void cWorldStorage::UnqueueSave(const cChunkPtr & a_Chunk)
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
			
			// Load 1 chunk:
			cChunkPtr ToLoad;
			{
				cCSLock Lock(m_CSLoadQueue);
				if (m_LoadQueue.size() > 0)
				{
					ToLoad = m_LoadQueue.front();
					m_LoadQueue.pop_front();
				}
				HasMore = (m_LoadQueue.size() > 0);
			}
			if ((ToLoad != NULL) && !LoadChunk(ToLoad))
			{
				// The chunk couldn't be loaded, generate it:
				m_World->GetGenerator().GenerateChunk(ToLoad->GetPosX(), ToLoad->GetPosZ());
			}
			
			// Save 1 chunk:
			cChunkPtr Save;
			{
				cCSLock Lock(m_CSSaveQueue);
				if (m_SaveQueue.size() > 0)
				{
					Save = m_SaveQueue.front();
					m_SaveQueue.pop_front();
				}
				HasMore = HasMore || (m_SaveQueue.size() > 0);
			}
			if ((Save != NULL) && (!m_SaveSchema->SaveChunk(Save)))
			{
				LOGWARNING("Cannot save chunk [%d, %d]", Save->GetPosX(), Save->GetPosZ());
			}
		} while (HasMore);
	}
}





bool cWorldStorage::LoadChunk(const cChunkPtr & a_Chunk)
{
	if (a_Chunk->IsValid())
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
		if ((*itr)->LoadChunk(a_Chunk))
		{
			return true;
		}
	}
	
	return false;
}




