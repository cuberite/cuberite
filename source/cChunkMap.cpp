
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "cChunkMap.h"
#include "cChunk.h"
#include "cWorld.h"
#include "cRoot.h"
#include "cMakeDir.h"

#ifndef _WIN32
	#include <cstdlib> // abs
#endif

#include "zlib.h"
#include <json/json.h>


#define USE_MEMCPY





////////////////////////////////////////////////////////////////////////////////
// cChunkMap:

cChunkMap::cChunkMap(cWorld * a_World )
	: m_World( a_World )
{
}





cChunkMap::~cChunkMap()
{
	cCSLock Lock(m_CSLayers);
	for (cChunkLayerList::iterator itr = m_Layers.begin(); itr != m_Layers.end(); ++itr)
	{
		delete *itr;
	}  // for itr - m_Layers[]
}





void cChunkMap::RemoveLayer( cChunkLayer* a_Layer )
{
	cCSLock Lock(m_CSLayers);
	m_Layers.remove(a_Layer);
}





cChunkMap::cChunkLayer * cChunkMap::GetLayer(int a_LayerX, int a_LayerZ)
{
	cCSLock Lock(m_CSLayers);
	for (cChunkLayerList::const_iterator itr = m_Layers.begin(); itr != m_Layers.end(); ++itr)
	{
		if (((*itr)->GetX() == a_LayerX) && ((*itr)->GetZ() == a_LayerZ))
		{
			return *itr;
		}
	}
	
	// Not found, create new:
	cChunkLayer * Layer = new cChunkLayer(a_LayerX, a_LayerZ, this);
	if (Layer == NULL)
	{
		LOGERROR("cChunkMap: Cannot create new layer, server out of memory?");
		return NULL;
	}
	m_Layers.push_back(Layer);
	return Layer;
}





cChunkMap::cChunkLayer * cChunkMap::GetLayerForChunk( int a_ChunkX, int a_ChunkZ )
{
	const int LayerX = (int)(floorf((float)a_ChunkX / (float)(LAYER_SIZE)));
	const int LayerZ = (int)(floorf((float)a_ChunkZ / (float)(LAYER_SIZE)));
	return GetLayer( LayerX, LayerZ );
}





cChunkPtr cChunkMap::GetChunk( int a_ChunkX, int a_ChunkY, int a_ChunkZ )
{
	cCSLock Lock(m_CSLayers);
	cChunkLayer * Layer = GetLayerForChunk( a_ChunkX, a_ChunkZ );
	if (Layer == NULL)
	{
		// An error must have occurred, since layers are automatically created if they don't exist
		return cChunkPtr();
	}
	
	cChunkPtr Chunk = Layer->GetChunk(a_ChunkX, a_ChunkZ);
	if (!(Chunk->IsValid()))
	{
		m_World->GetStorage().QueueLoadChunk(a_ChunkX, a_ChunkZ);
	}
	return Chunk;
}





cChunkPtr cChunkMap::GetChunkNoGen( int a_ChunkX, int a_ChunkY, int a_ChunkZ )
{
	cCSLock Lock(m_CSLayers);
	cChunkLayer * Layer = GetLayerForChunk( a_ChunkX, a_ChunkZ );
	if (Layer == NULL)
	{
		// An error must have occurred, since layers are automatically created if they don't exist
		return cChunkPtr();
	}
	
	cChunkPtr Chunk = Layer->GetChunk(a_ChunkX, a_ChunkZ);
	
	// TODO: Load, but do not generate, if not valid
	
	return Chunk;
}





void cChunkMap::BroadcastToChunkOfBlock(int a_X, int a_Y, int a_Z, cPacket * a_Packet, cClientHandle * a_Exclude)
{
	// Broadcasts a_Packet to all clients in the chunk where block [x, y, z] is, except to client a_Exclude
	
	cCSLock Lock(m_CSLayers);
	int ChunkX, ChunkZ;
	BlockToChunk(a_X, a_Y, a_Z, ChunkX, ChunkZ);
	cChunkPtr Chunk = GetChunkNoGen(ChunkX, 0, ChunkZ);
	if (Chunk == NULL)
	{
		return;
	}
	// Packets can be broadcasted even to invalid chunks!
	Chunk->Broadcast(a_Packet);
}





void cChunkMap::UseBlockEntity(cPlayer * a_Player, int a_X, int a_Y, int a_Z)
{
	// a_Player rclked block entity at the coords specified, handle it
	cCSLock Lock(m_CSLayers);
	int ChunkX, ChunkZ;
	BlockToChunk(a_X, a_Y, a_Z, ChunkX, ChunkZ);
	cChunkPtr Chunk = GetChunkNoGen(ChunkX, 0, ChunkZ);
	if ((Chunk == NULL) || !Chunk->IsValid())
	{
		return;
	}
	Chunk->UseBlockEntity(a_Player, a_X, a_Y, a_Z);
}





void cChunkMap::MarkChunkDirty (int a_ChunkX, int a_ChunkY, int a_ChunkZ)
{
	cCSLock Lock(m_CSLayers);
	cChunkPtr Chunk = GetChunkNoGen(a_ChunkX, a_ChunkY, a_ChunkZ);
	if ((Chunk == NULL) || !Chunk->IsValid())
	{
		return;
	}
	Chunk->MarkDirty();
}





void cChunkMap::MarkChunkSaving(int a_ChunkX, int a_ChunkY, int a_ChunkZ)
{
	cCSLock Lock(m_CSLayers);
	cChunkPtr Chunk = GetChunkNoGen(a_ChunkX, a_ChunkY, a_ChunkZ);
	if ((Chunk == NULL) || !Chunk->IsValid())
	{
		return;
	}
	Chunk->MarkSaving();
}





void cChunkMap::MarkChunkSaved (int a_ChunkX, int a_ChunkY, int a_ChunkZ)
{
	cCSLock Lock(m_CSLayers);
	cChunkPtr Chunk = GetChunkNoGen(a_ChunkX, a_ChunkY, a_ChunkZ);
	if ((Chunk == NULL) || !Chunk->IsValid())
	{
		return;
	}
	Chunk->MarkSaved();
}





void cChunkMap::ChunkDataLoaded(int a_ChunkX, int a_ChunkY, int a_ChunkZ, const char * a_BlockData, cEntityList & a_Entities, cBlockEntityList & a_BlockEntities)
{
	cCSLock Lock(m_CSLayers);
	cChunkPtr Chunk = GetChunkNoGen(a_ChunkX, a_ChunkY, a_ChunkZ);
	if (Chunk == NULL)
	{
		return;
	}
	Chunk->SetAllData(a_BlockData, a_Entities, a_BlockEntities);
	Chunk->MarkLoaded();
}





void cChunkMap::SetChunkData(int a_ChunkX, int a_ChunkY, int a_ChunkZ, const char * a_BlockData, cEntityList & a_Entities, cBlockEntityList & a_BlockEntities)
{
	cCSLock Lock(m_CSLayers);
	cChunkPtr Chunk = GetChunkNoGen(a_ChunkX, a_ChunkY, a_ChunkZ);
	if (Chunk == NULL)
	{
		return;
	}
	Chunk->SetAllData(a_BlockData, a_Entities, a_BlockEntities);
}





void cChunkMap::GetChunkData(int a_ChunkX, int a_ChunkY, int a_ChunkZ, cChunkDataCallback * a_Callback)
{
	cCSLock Lock(m_CSLayers);
	cChunkPtr Chunk = GetChunkNoGen(a_ChunkX, a_ChunkY, a_ChunkZ);
	if ((Chunk == NULL) || !Chunk->IsValid())
	{
		return;
	}
	Chunk->GetAllData(a_Callback);
}





bool cChunkMap::IsChunkValid(int a_ChunkX, int a_ChunkY, int a_ChunkZ)
{
	cCSLock Lock(m_CSLayers);
	cChunkPtr Chunk = GetChunkNoGen(a_ChunkX, a_ChunkY, a_ChunkZ);
	return (Chunk != NULL) && Chunk->IsValid();
}





bool cChunkMap::HasChunkAnyClients(int a_ChunkX, int a_ChunkY, int a_ChunkZ)
{
	cCSLock Lock(m_CSLayers);
	cChunkPtr Chunk = GetChunkNoGen(a_ChunkX, a_ChunkY, a_ChunkZ);
	return (Chunk != NULL) && Chunk->HasAnyClients();
}





void cChunkMap::Tick( float a_Dt, MTRand & a_TickRandom )
{
	cCSLock Lock(m_CSLayers);
	for (cChunkLayerList::iterator itr = m_Layers.begin(); itr != m_Layers.end(); ++itr)
	{
		(*itr)->Tick(a_Dt, a_TickRandom);
	}  // for itr - m_Layers
}





void cChunkMap::UnloadUnusedChunks()
{
	cCSLock Lock(m_CSLayers);
	for (cChunkLayerList::iterator itr = m_Layers.begin(); itr != m_Layers.end(); ++itr)
	{
		(*itr)->UnloadUnusedChunks();
	}  // for itr - m_Layers
}





void cChunkMap::SaveAllChunks(void)
{
	cCSLock Lock(m_CSLayers);
	for (cChunkLayerList::iterator itr = m_Layers.begin(); itr != m_Layers.end(); ++itr)
	{
		(*itr)->Save();
	}  // for itr - m_Layers[]
}





////////////////////////////////////////////////////////////////////////////////
// cChunkMap::cChunkLayer:

cChunkMap::cChunkLayer::cChunkLayer(int a_LayerX, int a_LayerZ, cChunkMap * a_Parent)
	: m_LayerX( a_LayerX )
	, m_LayerZ( a_LayerZ )
	, m_Parent( a_Parent )
	, m_NumChunksLoaded( 0 )
{
}





cChunkPtr cChunkMap::cChunkLayer::GetChunk( int a_ChunkX, int a_ChunkZ )
{
	// Always returns an assigned chunkptr, but the chunk needn't be valid (loaded / generated) - callers must check
	
	const int LocalX = a_ChunkX - m_LayerX * LAYER_SIZE;
	const int LocalZ = a_ChunkZ - m_LayerZ * LAYER_SIZE;
	
	
	if (!((LocalX < LAYER_SIZE) && (LocalZ < LAYER_SIZE) && (LocalX > -1) && (LocalZ > -1)))
	{
		assert(!"Asking a cChunkLayer for a chunk that doesn't belong to it!");
		return cChunkPtr();
	}
	
	int Index = LocalX + LocalZ * LAYER_SIZE;
	if (m_Chunks[Index].get() == NULL)
	{
		m_Chunks[Index].reset(new cChunk(a_ChunkX, 0, a_ChunkZ, m_Parent->GetWorld()));
	}
	return m_Chunks[Index];
}





void cChunkMap::cChunkLayer::Tick(float a_Dt, MTRand & a_TickRand)
{
	for (int i = 0; i < ARRAYCOUNT(m_Chunks); i++)
	{
		if ((m_Chunks[i] != NULL) && (m_Chunks[i]->IsValid()))
		{
			m_Chunks[i]->Tick(a_Dt, a_TickRand);
		}
	}  // for i - m_Chunks[]
}





void cChunkMap::cChunkLayer::Save(void)
{
	cWorld * World = m_Parent->GetWorld();
	for (int i = 0; i < ARRAYCOUNT(m_Chunks); ++i)
	{
		if ((m_Chunks[i] != NULL) && m_Chunks[i]->IsValid() && m_Chunks[i]->IsDirty())
		{
			World->GetStorage().QueueSaveChunk(m_Chunks[i]->GetPosX(), m_Chunks[i]->GetPosZ());
		}
	}  // for i - m_Chunks[]
}





void cChunkMap::cChunkLayer::UnloadUnusedChunks(void)
{
	for (int i = 0; i < ARRAYCOUNT(m_Chunks); i++)
	{
		if ((m_Chunks[i] != NULL) && (m_Chunks[i]->CanUnload()))
		{
			m_Chunks[i].reset();
		}
	}  // for i - m_Chunks[]
}





int cChunkMap::GetNumChunks(void)
{
	cCSLock Lock(m_CSLayers);
	int NumChunks = 0;
	for (cChunkLayerList::iterator itr = m_Layers.begin(); itr != m_Layers.end(); ++itr)
	{
		NumChunks += (*itr)->GetNumChunksLoaded();
	}
	return NumChunks;
}




