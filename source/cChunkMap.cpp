
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "cChunkMap.h"
#include "cChunk.h"
#include "cWorld.h"
#include "cRoot.h"
#include "cMakeDir.h"
#include "cPlayer.h"

#ifndef _WIN32
	#include <cstdlib> // abs
#endif

#include "zlib.h"
#include <json/json.h>





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
	
	cChunkPtr Chunk = Layer->GetChunk(a_ChunkX, a_ChunkY, a_ChunkZ);
	if (!(Chunk->IsValid()))
	{
		m_World->GetStorage().QueueLoadChunk(a_ChunkX, a_ChunkY, a_ChunkZ);
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
	
	cChunkPtr Chunk = Layer->GetChunk(a_ChunkX, a_ChunkY, a_ChunkZ);
	
	// TODO: Load, but do not generate, if not valid
	
	return Chunk;
}





void cChunkMap::BroadcastToChunk(int a_ChunkX, int a_ChunkY, int a_ChunkZ, cPacket & a_Packet, cClientHandle * a_Exclude)
{
	// Broadcasts a_Packet to all clients in the chunk where block [x, y, z] is, except to client a_Exclude
	
	cCSLock Lock(m_CSLayers);
	cChunkPtr Chunk = GetChunkNoGen(a_ChunkX, a_ChunkY, a_ChunkZ);
	if (Chunk == NULL)
	{
		return;
	}
	// It's perfectly legal to broadcast packets even to invalid chunks!
	Chunk->Broadcast(a_Packet, a_Exclude);
}





void cChunkMap::BroadcastToChunkOfBlock(int a_X, int a_Y, int a_Z, cPacket * a_Packet, cClientHandle * a_Exclude)
{
	// Broadcasts a_Packet to all clients in the chunk where block [x, y, z] is, except to client a_Exclude
	
	cCSLock Lock(m_CSLayers);
	int ChunkX, ChunkZ;
	BlockToChunk(a_X, a_Y, a_Z, ChunkX, ChunkZ);
	cChunkPtr Chunk = GetChunkNoGen(ChunkX, ZERO_CHUNK_Y, ChunkZ);
	if (Chunk == NULL)
	{
		return;
	}
	// It's perfectly legal to broadcast packets even to invalid chunks!
	Chunk->Broadcast(a_Packet, a_Exclude);
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





void cChunkMap::ChunkDataGenerated(int a_ChunkX, int a_ChunkY, int a_ChunkZ, const char * a_BlockData, cEntityList & a_Entities, cBlockEntityList & a_BlockEntities)
{
	cCSLock Lock(m_CSLayers);
	cChunkPtr Chunk = GetChunkNoGen(a_ChunkX, a_ChunkY, a_ChunkZ);
	if (Chunk == NULL)
	{
		return;
	}
	Chunk->SetAllData(a_BlockData, a_Entities, a_BlockEntities);

	// TODO: This has to go - lighting takes way too long to execute in a locked ChunkMap!
	Chunk->CalculateLighting();
	
	Chunk->SetValid();
	Chunk->MarkDirty();
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





bool cChunkMap::GetChunkBlocks(int a_ChunkX, int a_ChunkY, int a_ChunkZ, char * a_Blocks)
{
	cCSLock Lock(m_CSLayers);
	cChunkPtr Chunk = GetChunkNoGen(a_ChunkX, a_ChunkY, a_ChunkZ);
	if ((Chunk == NULL) || !Chunk->IsValid())
	{
		return false;
	}
	Chunk->GetBlocks(a_Blocks);
	return true;
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





void cChunkMap::SpreadChunkLighting(int a_ChunkX, int a_ChunkY, int a_ChunkZ)
{
	cCSLock Lock(m_CSLayers);
	cChunkPtr Chunk = GetChunkNoGen(a_ChunkX, a_ChunkY, a_ChunkZ);
	if ((Chunk != NULL) && Chunk->IsValid())
	{
		// TODO: Rewrite this to call Chunk's lighting without any parameters
		Chunk->SpreadLight( Chunk->pGetSkyLight() );
		Chunk->SpreadLight( Chunk->pGetLight() );
	}
}





int  cChunkMap::GetHeight(int a_BlockX, int a_BlockZ)
{
	cCSLock Lock(m_CSLayers);
	int ChunkX, ChunkZ, BlockY = 0;
	AbsoluteToRelative(a_BlockX, BlockY, a_BlockZ, ChunkX, ChunkZ);
	cChunkPtr Chunk = GetChunk(ChunkX, ZERO_CHUNK_Y, ChunkZ);
	if (Chunk == NULL)
	{
		return 0;
	}
	
	// Wait for the chunk to become valid:
	while (!Chunk->IsValid())
	{
		GetChunk(ChunkX, ZERO_CHUNK_Y, ChunkZ);  // Re-queue (in case it managed to get unloaded before we caught it
		cCSUnlock Unlock(Lock);
		m_evtChunkValid.Wait();
	}
	
	return Chunk->GetHeight(a_BlockX, a_BlockZ);
}





void cChunkMap::FastSetBlocks(sSetBlockList & a_BlockList)
{
	sSetBlockList Failed;
	
	// Process all items from a_BlockList, either successfully or by placing into Failed
	while (!a_BlockList.empty())
	{
		int ChunkX = a_BlockList.front().ChunkX;
		int ChunkZ = a_BlockList.front().ChunkZ;
		cChunkPtr Chunk = GetChunkNoGen(ChunkX, ZERO_CHUNK_Y, ChunkZ);
		if ((Chunk != NULL) && Chunk->IsValid())
		{
			for (sSetBlockList::iterator itr = a_BlockList.begin(); itr != a_BlockList.end();)
			{
				if ((itr->ChunkX == ChunkX) && (itr->ChunkZ == ChunkZ))
				{
					Chunk->FastSetBlock(itr->x, itr->y, itr->z, itr->BlockType, itr->BlockMeta);
					itr = a_BlockList.erase(itr);
				}
				else
				{
					++itr;
				}
			}  // for itr - a_BlockList[]
		}
		else
		{
			// The chunk is not valid, move all blocks within this chunk to Failed
			for (sSetBlockList::iterator itr = a_BlockList.begin(); itr != a_BlockList.end();)
			{
				if ((itr->ChunkX == ChunkX) && (itr->ChunkZ == ChunkZ))
				{
					Failed.push_back(*itr);
					itr = a_BlockList.erase(itr);
				}
				else
				{
					++itr;
				}
			}  // for itr - a_BlockList[]
		}
	}
	
	// Return the failed:
	std::swap(Failed, a_BlockList);
}





void cChunkMap::CollectPickupsByPlayer(cPlayer * a_Player)
{
	int BlockX = (int)(a_Player->GetPosX());  // Truncating doesn't matter much; we're scanning entire chunks anyway
	int BlockY = (int)(a_Player->GetPosY());
	int BlockZ = (int)(a_Player->GetPosZ());
	int ChunkX, ChunkZ, ChunkY = ZERO_CHUNK_Y;
	AbsoluteToRelative(BlockX, BlockY, BlockZ, ChunkX, ChunkZ);
	int OtherChunkX = ChunkX + ((BlockX > 8) ? 1 : -1);
	int OtherChunkZ = ChunkZ + ((BlockZ > 8) ? 1 : -1);
	
	cCSLock Lock(m_CSLayers);
	GetChunkNoGen(ChunkX, ChunkY, ChunkZ)->CollectPickupsByPlayer(a_Player);

	// Check the neighboring chunks as well:
	GetChunkNoGen(OtherChunkX, ChunkY, ChunkZ     )->CollectPickupsByPlayer(a_Player);
	GetChunkNoGen(OtherChunkX, ChunkY, OtherChunkZ)->CollectPickupsByPlayer(a_Player);
	GetChunkNoGen(ChunkX,      ChunkY, ChunkZ     )->CollectPickupsByPlayer(a_Player);
	GetChunkNoGen(ChunkX,      ChunkY, OtherChunkZ)->CollectPickupsByPlayer(a_Player);
}





void cChunkMap::CompareChunkClients(int a_ChunkX1, int a_ChunkY1, int a_ChunkZ1, int a_ChunkX2, int a_ChunkY2, int a_ChunkZ2, cClientDiffCallback & a_Callback)
{
	cCSLock Lock(m_CSLayers);
	cChunkPtr Chunk1 = GetChunkNoGen(a_ChunkX1, a_ChunkY1, a_ChunkZ1);
	if (Chunk1 == NULL)
	{
		return;
	}
	cChunkPtr Chunk2 = GetChunkNoGen(a_ChunkX2, a_ChunkY2, a_ChunkZ2);
	if (Chunk2 == NULL)
	{
		return;
	}

	cClientHandleList Clients1(Chunk1->GetAllClients());
	cClientHandleList Clients2(Chunk2->GetAllClients());
	
	// Find "removed" clients:
	for (cClientHandleList::iterator itr1 = Clients1.begin(); itr1 != Clients1.end(); ++itr1)
	{
		bool Found = false;
		for (cClientHandleList::iterator itr2 = Clients2.begin(); itr2 != Clients2.end(); ++itr2)
		{
			if (*itr1 == *itr2)
			{
				Found = true;
				break;
			}
		}  // for itr2 - Clients2[]
		if (!Found)
		{
			a_Callback.Removed(*itr1);
		}
	}  // for itr1 - Clients1[]
	
	// Find "added" clients:
	for (cClientHandleList::iterator itr2 = Clients2.begin(); itr2 != Clients2.end(); ++itr2)
	{
		bool Found = false;
		for (cClientHandleList::iterator itr1 = Clients1.begin(); itr1 != Clients1.end(); ++itr1)
		{
			if (*itr1 == *itr2)
			{
				Found = true;
				break;
			}
		}  // for itr1 - Clients1[]
		if (!Found)
		{
			a_Callback.Added(*itr2);
		}
	}  // for itr2 - Clients2[]
}





void cChunkMap::MoveEntityToChunk(cEntity * a_Entity, int a_ChunkX, int a_ChunkY, int a_ChunkZ)
{
	cCSLock Lock(m_CSLayers);
	cChunkPtr OldChunk = GetChunkNoGen(a_Entity->GetChunkX(), a_Entity->GetChunkY(), a_Entity->GetChunkZ());
	if (OldChunk != NULL)
	{
		OldChunk->RemoveEntity(a_Entity);
	}
	cChunkPtr NewChunk = GetChunkNoGen(a_ChunkX, a_ChunkY, a_ChunkZ);
	if (NewChunk != NULL)
	{
		NewChunk->AddEntity(a_Entity);
	}
}





void cChunkMap::RemoveEntityFromChunk(cEntity * a_Entity, int a_ChunkX, int a_ChunkY, int a_ChunkZ)
{
	cCSLock Lock(m_CSLayers);
	cChunkPtr Chunk = GetChunkNoGen(a_ChunkX, a_ChunkY, a_ChunkZ);
	if ((Chunk == NULL) && !Chunk->IsValid())
	{
		return;
	}
	Chunk->RemoveEntity(a_Entity);
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





cChunkPtr cChunkMap::cChunkLayer::GetChunk( int a_ChunkX, int a_ChunkY, int a_ChunkZ )
{
	// Always returns an assigned chunkptr, but the chunk needn't be valid (loaded / generated) - callers must check
	
	const int LocalX = a_ChunkX - m_LayerX * LAYER_SIZE;
	const int LocalZ = a_ChunkZ - m_LayerZ * LAYER_SIZE;
	
	
	if (!((LocalX < LAYER_SIZE) && (LocalZ < LAYER_SIZE) && (LocalX > -1) && (LocalZ > -1)))
	{
		ASSERT(!"Asking a cChunkLayer for a chunk that doesn't belong to it!");
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
		// Only tick chunks that are valid and have clients:
		if ((m_Chunks[i] != NULL) && m_Chunks[i]->IsValid() && m_Chunks[i]->HasAnyClients())
		{
			m_Chunks[i]->Tick(a_Dt, a_TickRand);
		}
	}  // for i - m_Chunks[]
}





int cChunkMap::cChunkLayer::GetNumChunksLoaded(void) const
{
	int NumChunks = 0;
	for ( int i = 0; i < ARRAYCOUNT(m_Chunks); ++i )
	{
		if (m_Chunks[i] != NULL)
		{
			NumChunks++;
		}
	}  // for i - m_Chunks[]
	return NumChunks; 
}





void cChunkMap::cChunkLayer::Save(void)
{
	cWorld * World = m_Parent->GetWorld();
	for (int i = 0; i < ARRAYCOUNT(m_Chunks); ++i)
	{
		if ((m_Chunks[i] != NULL) && m_Chunks[i]->IsValid() && m_Chunks[i]->IsDirty())
		{
			World->GetStorage().QueueSaveChunk(m_Chunks[i]->GetPosX(), m_Chunks[i]->GetPosY(), m_Chunks[i]->GetPosZ());
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





void cChunkMap::ChunkValidated(void)
{
	m_evtChunkValid.Set();
}




