
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "cChunkMap.h"
#include "cWorld.h"
#include "cRoot.h"
#include "cMakeDir.h"
#include "cPlayer.h"
#include "BlockID.h"
#include "cItem.h"
#include "cPickup.h"
#include "cChunk.h"
#include "Trees.h"  // used in cChunkMap::ReplaceTreeBlocks() for tree block discrimination

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
	// No need to lock m_CSLayers, since it's already locked by the operation that called us
	cChunkLayer * Layer = GetLayerForChunk( a_ChunkX, a_ChunkZ );
	if (Layer == NULL)
	{
		// An error must have occurred, since layers are automatically created if they don't exist
		return NULL;
	}
	
	cChunkPtr Chunk = Layer->GetChunk(a_ChunkX, a_ChunkY, a_ChunkZ);
	if (Chunk == NULL)
	{
		return NULL;
	}
	if (!(Chunk->IsValid()))
	{
		m_World->GetStorage().QueueLoadChunk(a_ChunkX, a_ChunkY, a_ChunkZ, true);
	}
	return Chunk;
}





cChunkPtr cChunkMap::GetChunkNoGen( int a_ChunkX, int a_ChunkY, int a_ChunkZ )
{
	// No need to lock m_CSLayers, since it's already locked by the operation that called us
	cChunkLayer * Layer = GetLayerForChunk( a_ChunkX, a_ChunkZ );
	if (Layer == NULL)
	{
		// An error must have occurred, since layers are automatically created if they don't exist
		return NULL;
	}
	
	cChunkPtr Chunk = Layer->GetChunk(a_ChunkX, a_ChunkY, a_ChunkZ);
	if (Chunk == NULL)
	{
		return NULL;
	}
	if (!(Chunk->IsValid()))
	{
		m_World->GetStorage().QueueLoadChunk(a_ChunkX, a_ChunkY, a_ChunkZ, false);
	}
	
	return Chunk;
}





cChunkPtr cChunkMap::GetChunkNoLoad( int a_ChunkX, int a_ChunkY, int a_ChunkZ )
{
	// No need to lock m_CSLayers, since it's already locked by the operation that called us
	cChunkLayer * Layer = GetLayerForChunk( a_ChunkX, a_ChunkZ );
	if (Layer == NULL)
	{
		// An error must have occurred, since layers are automatically created if they don't exist
		return NULL;
	}
	
	return Layer->GetChunk(a_ChunkX, a_ChunkY, a_ChunkZ);
}





bool cChunkMap::LockedGetBlock(int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE & a_BlockType, NIBBLETYPE & a_BlockMeta)
{
	// We already have m_CSLayers locked since this can be called only from within the tick thread
	int ChunkX, ChunkZ;
	cChunkDef::AbsoluteToRelative(a_BlockX, a_BlockY, a_BlockZ, ChunkX, ChunkZ);
	cChunkPtr Chunk = GetChunkNoLoad(ChunkX, ZERO_CHUNK_Y, ChunkZ);
	if (Chunk == NULL)
	{
		return false;
	}
	
	int Index = cChunkDef::MakeIndexNoCheck(a_BlockX, a_BlockY, a_BlockZ);
	a_BlockType = Chunk->GetBlock(Index);
	a_BlockMeta = Chunk->GetMeta(Index);
	return true;
}





bool cChunkMap::LockedSetBlock(int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE   a_BlockType, NIBBLETYPE   a_BlockMeta)
{
	// We already have m_CSLayers locked since this can be called only from within the tick thread
	int ChunkX, ChunkZ;
	cChunkDef::AbsoluteToRelative(a_BlockX, a_BlockY, a_BlockZ, ChunkX, ChunkZ);
	cChunkPtr Chunk = GetChunkNoLoad(ChunkX, ZERO_CHUNK_Y, ChunkZ);
	if (Chunk == NULL)
	{
		return false;
	}
	
	Chunk->SetBlock(a_BlockX, a_BlockY, a_BlockZ, a_BlockType, a_BlockMeta);
	return true;
}





bool cChunkMap::LockedFastSetBlock(int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta)
{
	// We already have m_CSLayers locked since this can be called only from within the tick thread
	int ChunkX, ChunkZ;
	cChunkDef::AbsoluteToRelative(a_BlockX, a_BlockY, a_BlockZ, ChunkX, ChunkZ);
	cChunkPtr Chunk = GetChunkNoLoad(ChunkX, ZERO_CHUNK_Y, ChunkZ);
	if (Chunk == NULL)
	{
		return false;
	}
	
	Chunk->FastSetBlock(a_BlockX, a_BlockY, a_BlockZ, a_BlockType, a_BlockMeta);
	return true;
}





void cChunkMap::BroadcastToChunk(int a_ChunkX, int a_ChunkY, int a_ChunkZ, const cPacket & a_Packet, cClientHandle * a_Exclude)
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





void cChunkMap::BroadcastToChunkOfBlock(int a_X, int a_Y, int a_Z, const cPacket * a_Packet, cClientHandle * a_Exclude)
{
	// Broadcasts a_Packet to all clients in the chunk where block [x, y, z] is, except to client a_Exclude
	
	cCSLock Lock(m_CSLayers);
	int ChunkX, ChunkZ;
	cChunkDef::BlockToChunk(a_X, a_Y, a_Z, ChunkX, ChunkZ);
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
	cChunkDef::BlockToChunk(a_X, a_Y, a_Z, ChunkX, ChunkZ);
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





void cChunkMap::SetChunkData(
	int a_ChunkX, int a_ChunkY, int a_ChunkZ,
	const BLOCKTYPE *  a_BlockTypes,
	const NIBBLETYPE * a_BlockMeta,
	const NIBBLETYPE * a_BlockLight,
	const NIBBLETYPE * a_BlockSkyLight,
	const cChunkDef::HeightMap * a_HeightMap,
	const cChunkDef::BiomeMap &  a_BiomeMap,
	cEntityList & a_Entities,
	cBlockEntityList & a_BlockEntities,
	bool a_MarkDirty
)
{
	cCSLock Lock(m_CSLayers);
	cChunkPtr Chunk = GetChunkNoLoad(a_ChunkX, a_ChunkY, a_ChunkZ);
	if (Chunk == NULL)
	{
		return;
	}
	Chunk->SetAllData(a_BlockTypes, a_BlockMeta, a_BlockLight, a_BlockSkyLight, a_HeightMap, a_BiomeMap, a_Entities, a_BlockEntities);
	Chunk->SetValid();
	
	if (a_MarkDirty)
	{
		Chunk->MarkDirty();
	}
}





void cChunkMap::ChunkLighted(
	int a_ChunkX, int a_ChunkZ,
	const cChunkDef::BlockNibbles & a_BlockLight,
	const cChunkDef::BlockNibbles & a_SkyLight
)
{
	cCSLock Lock(m_CSLayers);
	cChunkPtr Chunk = GetChunkNoLoad(a_ChunkX, ZERO_CHUNK_Y, a_ChunkZ);
	if (Chunk == NULL)
	{
		return;
	}
	Chunk->SetLight(a_BlockLight, a_SkyLight);
	Chunk->MarkDirty();
}





bool cChunkMap::GetChunkData(int a_ChunkX, int a_ChunkY, int a_ChunkZ, cChunkDataCallback & a_Callback)
{
	cCSLock Lock(m_CSLayers);
	cChunkPtr Chunk = GetChunkNoGen(a_ChunkX, a_ChunkY, a_ChunkZ);
	if ((Chunk == NULL) || !Chunk->IsValid())
	{
		return false;
	}
	Chunk->GetAllData(a_Callback);
	return true;
}





bool cChunkMap::GetChunkBlockTypes(int a_ChunkX, int a_ChunkY, int a_ChunkZ, BLOCKTYPE * a_BlockTypes)
{
	cCSLock Lock(m_CSLayers);
	cChunkPtr Chunk = GetChunkNoGen(a_ChunkX, a_ChunkY, a_ChunkZ);
	if ((Chunk == NULL) || !Chunk->IsValid())
	{
		return false;
	}
	Chunk->GetBlockTypes(a_BlockTypes);
	return true;
}





bool cChunkMap::GetChunkBlockData(int a_ChunkX, int a_ChunkY, int a_ChunkZ, BLOCKTYPE * a_BlockData)
{
	cCSLock Lock(m_CSLayers);
	cChunkPtr Chunk = GetChunkNoGen(a_ChunkX, a_ChunkY, a_ChunkZ);
	if ((Chunk == NULL) || !Chunk->IsValid())
	{
		return false;
	}
	Chunk->GetBlockData(a_BlockData);
	return true;
}





bool cChunkMap::IsChunkValid(int a_ChunkX, int a_ChunkY, int a_ChunkZ)
{
	cCSLock Lock(m_CSLayers);
	cChunkPtr Chunk = GetChunkNoLoad(a_ChunkX, a_ChunkY, a_ChunkZ);
	return (Chunk != NULL) && Chunk->IsValid();
}





bool cChunkMap::HasChunkAnyClients(int a_ChunkX, int a_ChunkY, int a_ChunkZ)
{
	cCSLock Lock(m_CSLayers);
	cChunkPtr Chunk = GetChunkNoGen(a_ChunkX, a_ChunkY, a_ChunkZ);
	return (Chunk != NULL) && Chunk->HasAnyClients();
}





int  cChunkMap::GetHeight(int a_BlockX, int a_BlockZ)
{
	cCSLock Lock(m_CSLayers);
	int ChunkX, ChunkZ, BlockY = 0;
	cChunkDef::AbsoluteToRelative(a_BlockX, BlockY, a_BlockZ, ChunkX, ChunkZ);
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
		cCSLock Lock(m_CSLayers);
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
	cChunkDef::AbsoluteToRelative(BlockX, BlockY, BlockZ, ChunkX, ChunkZ);
	int OtherChunkX = ChunkX + ((BlockX > 8) ? 1 : -1);
	int OtherChunkZ = ChunkZ + ((BlockZ > 8) ? 1 : -1);
	
	// We suppose that each player keeps their chunks in memory, therefore it makes little sense to try to re-load or even generate them.
	// The only time the chunks are not valid is when the player is downloading the initial world and they should not call this at that moment
	
	cCSLock Lock(m_CSLayers);
	GetChunkNoLoad(ChunkX, ChunkY, ChunkZ)->CollectPickupsByPlayer(a_Player);

	// Check the neighboring chunks as well:
	GetChunkNoLoad(OtherChunkX, ChunkY, ChunkZ     )->CollectPickupsByPlayer(a_Player);
	GetChunkNoLoad(OtherChunkX, ChunkY, OtherChunkZ)->CollectPickupsByPlayer(a_Player);
	GetChunkNoLoad(ChunkX,      ChunkY, ChunkZ     )->CollectPickupsByPlayer(a_Player);
	GetChunkNoLoad(ChunkX,      ChunkY, OtherChunkZ)->CollectPickupsByPlayer(a_Player);
}





BLOCKTYPE cChunkMap::GetBlock(int a_X, int a_Y, int a_Z)
{
	int ChunkX, ChunkZ;
	cChunkDef::AbsoluteToRelative( a_X, a_Y, a_Z, ChunkX, ChunkZ );
	
	cCSLock Lock(m_CSLayers);
	cChunkPtr Chunk = GetChunk(ChunkX, ZERO_CHUNK_Y, ChunkZ);
	if ((Chunk != NULL) && Chunk->IsValid())
	{
		return Chunk->GetBlock(a_X, a_Y, a_Z);
	}
	return 0;
}





BLOCKTYPE cChunkMap::GetBlockMeta(int a_X, int a_Y, int a_Z)
{
	int ChunkX, ChunkZ;
	cChunkDef::AbsoluteToRelative( a_X, a_Y, a_Z, ChunkX, ChunkZ );
	
	cCSLock Lock(m_CSLayers);
	cChunkPtr Chunk = GetChunk( ChunkX, ZERO_CHUNK_Y, ChunkZ );
	if ((Chunk != NULL) && Chunk->IsValid() )
	{
		return Chunk->GetMeta(a_X, a_Y, a_Z);
	}
	return 0;
}





BLOCKTYPE cChunkMap::GetBlockSkyLight(int a_X, int a_Y, int a_Z)
{
	int ChunkX, ChunkZ;
	cChunkDef::AbsoluteToRelative( a_X, a_Y, a_Z, ChunkX, ChunkZ );

	cCSLock Lock(m_CSLayers);
	cChunkPtr Chunk = GetChunk( ChunkX, ZERO_CHUNK_Y, ChunkZ );
	if ((Chunk != NULL) && Chunk->IsValid() )
	{
		return Chunk->GetSkyLight( a_X, a_Y, a_Z );
	}
	return 0;
}





void cChunkMap::SetBlockMeta(int a_X, int a_Y, int a_Z, char a_BlockMeta)
{
	int ChunkX, ChunkZ;
	cChunkDef::AbsoluteToRelative( a_X, a_Y, a_Z, ChunkX, ChunkZ );

	cCSLock Lock(m_CSLayers);
	cChunkPtr Chunk = GetChunk( ChunkX, ZERO_CHUNK_Y, ChunkZ );
	if ((Chunk != NULL) && Chunk->IsValid() )	
	{
		Chunk->SetMeta(a_X, a_Y, a_Z, a_BlockMeta);
		Chunk->MarkDirty();
		Chunk->SendBlockTo( a_X, a_Y, a_Z, NULL );
	}
}





void cChunkMap::SetBlock(int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE a_BlockType, BLOCKTYPE a_BlockMeta)
{
	int ChunkX, ChunkZ, X = a_BlockX, Y = a_BlockY, Z = a_BlockZ;
	cChunkDef::AbsoluteToRelative( X, Y, Z, ChunkX, ChunkZ );

	cCSLock Lock(m_CSLayers);
	cChunkPtr Chunk = GetChunk( ChunkX, ZERO_CHUNK_Y, ChunkZ );
	if ((Chunk != NULL) && Chunk->IsValid())
	{
		Chunk->SetBlock(X, Y, Z, a_BlockType, a_BlockMeta );
	}
}





void cChunkMap::GetBlockTypeMeta(int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE & a_BlockType, NIBBLETYPE & a_BlockMeta)
{
	int ChunkX, ChunkZ, X = a_BlockX, Y = a_BlockY, Z = a_BlockZ;
	cChunkDef::AbsoluteToRelative( X, Y, Z, ChunkX, ChunkZ );

	cCSLock Lock(m_CSLayers);
	cChunkPtr Chunk = GetChunk( ChunkX, ZERO_CHUNK_Y, ChunkZ );
	if ((Chunk != NULL) && Chunk->IsValid())
	{
		Chunk->GetBlockTypeMeta(X, Y, Z, a_BlockType, a_BlockMeta);
	}
}





void cChunkMap::ReplaceBlocks(const sSetBlockVector & a_Blocks, BLOCKTYPE a_FilterBlockType)
{
	cCSLock Lock(m_CSLayers);
	for (sSetBlockVector::const_iterator itr = a_Blocks.begin(); itr != a_Blocks.end(); ++itr)
	{
		cChunkPtr Chunk = GetChunk(itr->ChunkX, ZERO_CHUNK_Y, itr->ChunkZ );
		if ((Chunk == NULL) || !Chunk->IsValid())
		{
			continue;
		}
		if (Chunk->GetBlock(itr->x, itr->y, itr->z) == a_FilterBlockType)
		{
			Chunk->SetBlock(itr->x, itr->y, itr->z, itr->BlockType, itr->BlockMeta);
		}
	}
}





void cChunkMap::ReplaceTreeBlocks(const sSetBlockVector & a_Blocks)
{
	cCSLock Lock(m_CSLayers);
	for (sSetBlockVector::const_iterator itr = a_Blocks.begin(); itr != a_Blocks.end(); ++itr)
	{
		cChunkPtr Chunk = GetChunk(itr->ChunkX, ZERO_CHUNK_Y, itr->ChunkZ );
		if ((Chunk == NULL) || !Chunk->IsValid())
		{
			continue;
		}
		switch (Chunk->GetBlock(itr->x, itr->y, itr->z))
		{
			CASE_TREE_OVERWRITTEN_BLOCKS:
			{
				Chunk->SetBlock(itr->x, itr->y, itr->z, itr->BlockType, itr->BlockMeta);
				break;
			}
		}
	}  // for itr - a_Blocks[]
}





EMCSBiome cChunkMap::GetBiomeAt (int a_BlockX, int a_BlockZ)
{
	int ChunkX, ChunkZ, X = a_BlockX, Y = 0, Z = a_BlockZ;
	cChunkDef::AbsoluteToRelative( X, Y, Z, ChunkX, ChunkZ );

	cCSLock Lock(m_CSLayers);
	cChunkPtr Chunk = GetChunk( ChunkX, ZERO_CHUNK_Y, ChunkZ );
	if ((Chunk != NULL) && Chunk->IsValid())
	{
		return Chunk->GetBiomeAt(X, Z);
	}
	else
	{
		return m_World->GetGenerator().GetBiomeAt(a_BlockX, a_BlockZ);
	}
}





bool cChunkMap::GetBlocks(sSetBlockVector & a_Blocks, bool a_ContinueOnFailure)
{
	bool res = true;
	cCSLock Lock(m_CSLayers);
	for (sSetBlockVector::iterator itr = a_Blocks.begin(); itr != a_Blocks.end(); ++itr)
	{
		cChunkPtr Chunk = GetChunk(itr->ChunkX, ZERO_CHUNK_Y, itr->ChunkZ );
		if ((Chunk == NULL) || !Chunk->IsValid())
		{
			if (!a_ContinueOnFailure)
			{
				return false;
			}
			res = false;
			continue;
		}
		int idx = cChunkDef::MakeIndexNoCheck(itr->x, itr->y, itr->z);
		itr->BlockType = Chunk->GetBlock(idx);
		itr->BlockMeta = Chunk->GetMeta(idx);
	}
	return res;
}





bool cChunkMap::DigBlock(int a_X, int a_Y, int a_Z)
{
	int PosX = a_X, PosY = a_Y, PosZ = a_Z, ChunkX, ChunkZ;

	cChunkDef::AbsoluteToRelative( PosX, PosY, PosZ, ChunkX, ChunkZ );

	{
		cCSLock Lock(m_CSLayers);
		cChunkPtr DestChunk = GetChunk( ChunkX, ZERO_CHUNK_Y, ChunkZ );
		if ((DestChunk == NULL) || !DestChunk->IsValid())
		{
			return false;
		}
		
		DestChunk->SetBlock(PosX, PosY, PosZ, E_BLOCK_AIR, 0 );
	}

	m_World->GetSimulatorManager()->WakeUp(a_X, a_Y, a_Z);

	return true;
}





void cChunkMap::SendBlockTo(int a_X, int a_Y, int a_Z, cPlayer * a_Player)
{
	int ChunkX, ChunkZ;
	cChunkDef::AbsoluteToRelative(a_X, a_Y, a_Z, ChunkX, ChunkZ);
	
	cCSLock Lock(m_CSLayers);
	cChunkPtr Chunk = GetChunk(ChunkX, ZERO_CHUNK_Y, ChunkZ);
	if (Chunk->IsValid())
	{
		Chunk->SendBlockTo(a_X, a_Y, a_Z, a_Player->GetClientHandle());
	}
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





bool cChunkMap::AddChunkClient(int a_ChunkX, int a_ChunkY, int a_ChunkZ, cClientHandle * a_Client)
{
	cCSLock Lock(m_CSLayers);
	cChunkPtr Chunk = GetChunk(a_ChunkX, a_ChunkY, a_ChunkZ);
	if (Chunk == NULL)
	{
		return false;
	}
	return Chunk->AddClient(a_Client);
}





void cChunkMap::RemoveChunkClient(int a_ChunkX, int a_ChunkY, int a_ChunkZ, cClientHandle * a_Client)
{
	cCSLock Lock(m_CSLayers);
	cChunkPtr Chunk = GetChunkNoGen(a_ChunkX, a_ChunkY, a_ChunkZ);
	if (Chunk == NULL)
	{
		return;
	}
	Chunk->RemoveClient(a_Client);
}





void cChunkMap::RemoveClientFromChunks(cClientHandle * a_Client)
{
	cCSLock Lock(m_CSLayers);
	
	for (cChunkLayerList::const_iterator itr = m_Layers.begin(); itr != m_Layers.end(); ++itr)
	{
		(*itr)->RemoveClient(a_Client);
	}  // for itr - m_Layers[]
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





void cChunkMap::TouchChunk(int a_ChunkX, int a_ChunkY, int a_ChunkZ)
{
	cCSLock Lock(m_CSLayers);
	GetChunk(a_ChunkX, a_ChunkY, a_ChunkZ);
}





/// Loads the chunk synchronously, if not already loaded. Doesn't generate. Returns true if chunk valid (even if already loaded before)
bool cChunkMap::LoadChunk(int a_ChunkX, int a_ChunkY, int a_ChunkZ)
{
	{
		cCSLock Lock(m_CSLayers);
		cChunkPtr Chunk = GetChunkNoGen(a_ChunkX, a_ChunkY, a_ChunkZ);
		if (Chunk == NULL)
		{
			// Internal error
			return false;
		}
		if (Chunk->IsValid())
		{
			// Already loaded
			return true;
		}
		if (Chunk->HasLoadFailed())
		{
			// Already tried loading and it failed
			return false;
		}
	}
	return m_World->GetStorage().LoadChunk(a_ChunkX, a_ChunkY, a_ChunkZ);
}





/// Loads the chunks specified. Doesn't report failure, other than chunks being !IsValid()
void cChunkMap::LoadChunks(const cChunkCoordsList & a_Chunks)
{
	for (cChunkCoordsList::const_iterator itr = a_Chunks.begin(); itr != a_Chunks.end(); ++itr)
	{
		LoadChunk(itr->m_ChunkX, itr->m_ChunkY, itr->m_ChunkZ);
	}  // for itr - a_Chunks[]
}





void cChunkMap::ChunkLoadFailed(int a_ChunkX, int a_ChunkY, int a_ChunkZ)
{
	cCSLock Lock(m_CSLayers);
	cChunkPtr Chunk = GetChunkNoLoad(a_ChunkX, a_ChunkY, a_ChunkZ);
	if (Chunk == NULL)
	{
		return;
	}
	Chunk->MarkLoadFailed();
}





void cChunkMap::UpdateSign(int a_X, int a_Y, int a_Z, const AString & a_Line1, const AString & a_Line2, const AString & a_Line3, const AString & a_Line4)
{
	cCSLock Lock(m_CSLayers);
	int ChunkX, ChunkZ;
	cChunkDef::BlockToChunk(a_X, a_Y, a_Z, ChunkX, ChunkZ);
	cChunkPtr Chunk = GetChunkNoGen(ChunkX, ZERO_CHUNK_Y, ChunkZ);
	if ((Chunk == NULL) || !Chunk->IsValid())
	{
		return;
	}
	Chunk->UpdateSign(a_X, a_Y, a_Z, a_Line1, a_Line2, a_Line3, a_Line4);
}





void cChunkMap::ChunksStay(const cChunkCoordsList & a_Chunks, bool a_Stay)
{
	cCSLock Lock(m_CSLayers);
	for (cChunkCoordsList::const_iterator itr = a_Chunks.begin(); itr != a_Chunks.end(); ++itr)
	{
		cChunkPtr Chunk = GetChunkNoLoad(itr->m_ChunkX, itr->m_ChunkY, itr->m_ChunkZ);
		if (Chunk == NULL)
		{
			continue;
		}
		Chunk->Stay(a_Stay);
	}
}





void cChunkMap::MarkChunkRegenerating(int a_ChunkX, int a_ChunkZ)
{
	cCSLock Lock(m_CSLayers);
	cChunkPtr Chunk = GetChunkNoLoad(a_ChunkX, ZERO_CHUNK_Y, a_ChunkZ);
	if (Chunk == NULL)
	{
		// Not present
		return;
	}
	Chunk->MarkRegenerating();
}





bool cChunkMap::IsChunkLighted(int a_ChunkX, int a_ChunkZ)
{
	cCSLock Lock(m_CSLayers);
	cChunkPtr Chunk = GetChunkNoLoad(a_ChunkX, ZERO_CHUNK_Y, a_ChunkZ);
	if (Chunk == NULL)
	{
		// Not present
		return false;
	}
	return Chunk->IsLightValid();
}





void cChunkMap::GetChunkStats(int & a_NumChunksValid, int & a_NumChunksDirty)
{
	a_NumChunksValid = 0;
	a_NumChunksDirty = 0;
	cCSLock Lock(m_CSLayers);
	for (cChunkLayerList::const_iterator itr = m_Layers.begin(); itr != m_Layers.end(); ++itr)
	{
		int NumValid = 0, NumDirty = 0;
		(*itr)->GetChunkStats(NumValid, NumDirty);
		a_NumChunksValid += NumValid;
		a_NumChunksDirty += NumDirty;
	}  // for itr - m_Layers[]
}





void cChunkMap::GrowMelonPumpkin(int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE a_BlockType, MTRand & a_Rand)
{
	int ChunkX, ChunkZ;
	cChunkDef::AbsoluteToRelative(a_BlockX, a_BlockY, a_BlockZ, ChunkX, ChunkZ);
	
	cCSLock Lock(m_CSLayers);
	cChunkPtr Chunk = GetChunkNoLoad(ChunkX, ZERO_CHUNK_Y, ChunkZ);
	if (Chunk != NULL)
	{
		Chunk->GrowMelonPumpkin(a_BlockX, a_BlockY, a_BlockZ, a_BlockType, a_Rand);
	}
}





void cChunkMap::GrowSugarcane(int a_BlockX, int a_BlockY, int a_BlockZ, int a_NumBlocksToGrow)
{
	int ChunkX, ChunkZ;
	cChunkDef::AbsoluteToRelative(a_BlockX, a_BlockY, a_BlockZ, ChunkX, ChunkZ);
	
	cCSLock Lock(m_CSLayers);
	cChunkPtr Chunk = GetChunkNoLoad(ChunkX, ZERO_CHUNK_Y, ChunkZ);
	if (Chunk != NULL)
	{
		Chunk->GrowSugarcane(a_BlockX, a_BlockY, a_BlockZ, a_NumBlocksToGrow);
	}
}





void cChunkMap::GrowCactus(int a_BlockX, int a_BlockY, int a_BlockZ, int a_NumBlocksToGrow)
{
	int ChunkX, ChunkZ;
	cChunkDef::AbsoluteToRelative(a_BlockX, a_BlockY, a_BlockZ, ChunkX, ChunkZ);
	
	cCSLock Lock(m_CSLayers);
	cChunkPtr Chunk = GetChunkNoLoad(ChunkX, ZERO_CHUNK_Y, ChunkZ);
	if (Chunk != NULL)
	{
		Chunk->GrowCactus(a_BlockX, a_BlockY, a_BlockZ, a_NumBlocksToGrow);
	}
}





void cChunkMap::SetNextBlockTick(int a_BlockX, int a_BlockY, int a_BlockZ)
{
	int ChunkX, ChunkZ;
	cChunkDef::AbsoluteToRelative(a_BlockX, a_BlockY, a_BlockZ, ChunkX, ChunkZ);
	
	cCSLock Lock(m_CSLayers);
	cChunkPtr Chunk = GetChunkNoLoad(ChunkX, ZERO_CHUNK_Y, ChunkZ);
	if (Chunk != NULL)
	{
		Chunk->SetNextBlockTick(a_BlockX, a_BlockY, a_BlockZ);
	}
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
	memset(m_Chunks, 0, sizeof(m_Chunks));
}





cChunkMap::cChunkLayer::~cChunkLayer()
{
	for (int i = 0; i < ARRAYCOUNT(m_Chunks); ++i)
	{
		delete m_Chunks[i];
	}  // for i - m_Chunks[]
}





cChunkPtr cChunkMap::cChunkLayer::GetChunk( int a_ChunkX, int a_ChunkY, int a_ChunkZ )
{
	// Always returns an assigned chunkptr, but the chunk needn't be valid (loaded / generated) - callers must check

	const int LocalX = a_ChunkX - m_LayerX * LAYER_SIZE;
	const int LocalZ = a_ChunkZ - m_LayerZ * LAYER_SIZE;
	
	if (!((LocalX < LAYER_SIZE) && (LocalZ < LAYER_SIZE) && (LocalX > -1) && (LocalZ > -1)))
	{
		ASSERT(!"Asking a cChunkLayer for a chunk that doesn't belong to it!");
		return NULL;
	}
	
	int Index = LocalX + LocalZ * LAYER_SIZE;
	if (m_Chunks[Index] == NULL)
	{
		m_Chunks[Index] = new cChunk(a_ChunkX, 0, a_ChunkZ, m_Parent, m_Parent->GetWorld());
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





void cChunkMap::cChunkLayer::RemoveClient(cClientHandle * a_Client)
{
	for (int i = 0; i < ARRAYCOUNT(m_Chunks); i++)
	{
		if (m_Chunks[i] != NULL)
		{
			m_Chunks[i]->RemoveClient(a_Client);
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





void cChunkMap::cChunkLayer::GetChunkStats(int & a_NumChunksValid, int & a_NumChunksDirty) const
{
	int NumValid = 0;
	int NumDirty = 0;
	for ( int i = 0; i < ARRAYCOUNT(m_Chunks); ++i )
	{
		if (m_Chunks[i] == NULL)
		{
			continue;
		}
		NumValid++;
		if (m_Chunks[i]->IsDirty())
		{
			NumDirty++;
		}
	}  // for i - m_Chunks[]
	a_NumChunksValid = NumValid;
	a_NumChunksDirty = NumDirty;
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
			// The cChunk destructor calls our GetChunk() while removing its entities
			// so we still need to be able to return the chunk. Therefore we first delete, then NULLify
			// Doing otherwise results in bug http://forum.mc-server.org/showthread.php?tid=355
			delete m_Chunks[i];
			m_Chunks[i] = NULL;
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





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cChunkStay:

cChunkStay::cChunkStay(cWorld * a_World) :
	m_World(a_World),
	m_IsEnabled(false)
{
}





cChunkStay::~cChunkStay()
{
	Clear();
}





void cChunkStay::Clear(void)
{
	if (m_IsEnabled)
	{
		Disable();
	}
	m_Chunks.clear();
}





void cChunkStay::Add(int a_ChunkX, int a_ChunkY, int a_ChunkZ)
{
	ASSERT(!m_IsEnabled);

	for (cChunkCoordsList::const_iterator itr = m_Chunks.begin(); itr != m_Chunks.end(); ++itr)
	{
		if ((itr->m_ChunkX == a_ChunkX) && (itr->m_ChunkY == a_ChunkY) && (itr->m_ChunkZ == a_ChunkZ))
		{
			// Already present
			return;
		}
	}  // for itr - Chunks[]
	m_Chunks.push_back(cChunkCoords(a_ChunkX, a_ChunkY, a_ChunkZ));
}





void cChunkStay::Remove(int a_ChunkX, int a_ChunkY, int a_ChunkZ)
{
	ASSERT(!m_IsEnabled);

	for (cChunkCoordsList::iterator itr = m_Chunks.begin(); itr != m_Chunks.end(); ++itr)
	{
		if ((itr->m_ChunkX == a_ChunkX) && (itr->m_ChunkY == a_ChunkY) && (itr->m_ChunkZ == a_ChunkZ))
		{
			// Found, un-"stay"
			m_Chunks.erase(itr);
			return;
		}
	}  // for itr - m_Chunks[]
}





void cChunkStay::Enable(void)
{
	ASSERT(!m_IsEnabled);
	
	m_World->ChunksStay(*this, true);
	m_IsEnabled = true;
}





void cChunkStay::Load(void)
{
	for (cChunkCoordsList::iterator itr = m_Chunks.begin(); itr != m_Chunks.end(); ++itr)
	{
		m_World->TouchChunk(itr->m_ChunkX, itr->m_ChunkY, itr->m_ChunkZ);
	}  // for itr - m_Chunks[]
}





void cChunkStay::Disable(void)
{
	ASSERT(m_IsEnabled);
	
	m_World->ChunksStay(*this, false);
	m_IsEnabled = false;
}




