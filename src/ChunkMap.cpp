
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "ChunkMap.h"
#include "World.h"
#include "Root.h"
#include "Entities/Player.h"
#include "Item.h"
#include "Entities/Pickup.h"
#include "Chunk.h"
#include "Generating/Trees.h"  // used in cChunkMap::ReplaceTreeBlocks() for tree block discrimination
#include "BlockArea.h"
#include "Bindings/PluginManager.h"
#include "Entities/TNTEntity.h"
#include "Blocks/BlockHandler.h"
#include "MobCensus.h"
#include "MobSpawner.h"

#ifndef _WIN32
	#include <cstdlib> // abs
#endif

#include "zlib/zlib.h"
#include "json/json.h"





////////////////////////////////////////////////////////////////////////////////
// cChunkMap:

cChunkMap::cChunkMap(cWorld * a_World )
	: m_World( a_World )
{
}





cChunkMap::~cChunkMap()
{
	cCSLock Lock(m_CSLayers);
	while (!m_Layers.empty())
	{
		delete m_Layers.back();
		m_Layers.pop_back();  // Must pop, because further chunk deletions query the chunkmap for entities and that would touch deleted data
	}
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





cChunkMap::cChunkLayer * cChunkMap::FindLayerForChunk(int a_ChunkX, int a_ChunkZ)
{
	const int LayerX = FAST_FLOOR_DIV(a_ChunkX, LAYER_SIZE);
	const int LayerZ = FAST_FLOOR_DIV(a_ChunkZ, LAYER_SIZE);
	return FindLayer(LayerX, LayerZ);
}





cChunkMap::cChunkLayer * cChunkMap::FindLayer(int a_LayerX, int a_LayerZ)
{
	ASSERT(m_CSLayers.IsLockedByCurrentThread());

	for (cChunkLayerList::const_iterator itr = m_Layers.begin(); itr != m_Layers.end(); ++itr)
	{
		if (((*itr)->GetX() == a_LayerX) && ((*itr)->GetZ() == a_LayerZ))
		{
			return *itr;
		}
	}  // for itr - m_Layers[]
	
	// Not found
	return NULL;
}





cChunkMap::cChunkLayer * cChunkMap::GetLayerForChunk(int a_ChunkX, int a_ChunkZ)
{
	const int LayerX = FAST_FLOOR_DIV(a_ChunkX, LAYER_SIZE);
	const int LayerZ = FAST_FLOOR_DIV(a_ChunkZ, LAYER_SIZE);
	return GetLayer(LayerX, LayerZ);
}





cChunkPtr cChunkMap::GetChunk( int a_ChunkX, int a_ChunkY, int a_ChunkZ )
{
	// No need to lock m_CSLayers, since it's already locked by the operation that called us
	ASSERT(m_CSLayers.IsLockedByCurrentThread());

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
	ASSERT(m_CSLayers.IsLockedByCurrentThread());

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





bool cChunkMap::LockedGetBlockType(int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE & a_BlockType)
{
	// We already have m_CSLayers locked since this can be called only from within the tick thread
	ASSERT(m_CSLayers.IsLockedByCurrentThread());

	int ChunkX, ChunkZ;
	cChunkDef::AbsoluteToRelative(a_BlockX, a_BlockY, a_BlockZ, ChunkX, ChunkZ);
	cChunkPtr Chunk = GetChunkNoLoad(ChunkX, ZERO_CHUNK_Y, ChunkZ);
	if (Chunk == NULL)
	{
		return false;
	}
	
	int Index = cChunkDef::MakeIndexNoCheck(a_BlockX, a_BlockY, a_BlockZ);
	a_BlockType = Chunk->GetBlock(Index);
	return true;
}





bool cChunkMap::LockedGetBlockMeta(int a_BlockX, int a_BlockY, int a_BlockZ, NIBBLETYPE & a_BlockMeta)
{
	// We already have m_CSLayers locked since this can be called only from within the tick thread
	ASSERT(m_CSLayers.IsLockedByCurrentThread());
	
	int ChunkX, ChunkZ;
	cChunkDef::AbsoluteToRelative(a_BlockX, a_BlockY, a_BlockZ, ChunkX, ChunkZ);
	cChunkPtr Chunk = GetChunkNoLoad(ChunkX, ZERO_CHUNK_Y, ChunkZ);
	if (Chunk == NULL)
	{
		return false;
	}
	
	int Index = cChunkDef::MakeIndexNoCheck(a_BlockX, a_BlockY, a_BlockZ);
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





cChunk * cChunkMap::FindChunk(int a_ChunkX, int a_ChunkZ)
{
	ASSERT(m_CSLayers.IsLockedByCurrentThread());
	
	cChunkLayer * Layer = FindLayerForChunk(a_ChunkX, a_ChunkZ);
	if (Layer == NULL)
	{
		return NULL;
	}
	return Layer->FindChunk(a_ChunkX, a_ChunkZ);
}





void cChunkMap::BroadcastAttachEntity(const cEntity & a_Entity, const cEntity * a_Vehicle)
{
	cCSLock Lock(m_CSLayers);
	cChunkPtr Chunk = GetChunkNoGen(a_Entity.GetChunkX(), ZERO_CHUNK_Y, a_Entity.GetChunkZ());
	if (Chunk == NULL)
	{
		return;
	}
	// It's perfectly legal to broadcast packets even to invalid chunks!
	Chunk->BroadcastAttachEntity(a_Entity, a_Vehicle);
}





void cChunkMap::BroadcastBlockAction(int a_BlockX, int a_BlockY, int a_BlockZ, char a_Byte1, char a_Byte2, BLOCKTYPE a_BlockType, const cClientHandle * a_Exclude)
{
	cCSLock Lock(m_CSLayers);
	int x, y, z, ChunkX, ChunkZ;
	x = a_BlockX;
	y = a_BlockY;
	z = a_BlockZ;
	cChunkDef::BlockToChunk(x, z, ChunkX, ChunkZ);
	cChunkPtr Chunk = GetChunkNoGen(ChunkX, ZERO_CHUNK_Y, ChunkZ);
	if (Chunk == NULL)
	{
		return;
	}
	// It's perfectly legal to broadcast packets even to invalid chunks!
	Chunk->BroadcastBlockAction(a_BlockX, a_BlockY, a_BlockZ, a_Byte1, a_Byte2, a_BlockType, a_Exclude);
}





void cChunkMap::BroadcastBlockBreakAnimation(int a_entityID, int a_blockX, int a_blockY, int a_blockZ, char a_stage, const cClientHandle * a_Exclude)
{
	cCSLock Lock(m_CSLayers);
	int ChunkX, ChunkZ;

	cChunkDef::BlockToChunk(a_blockX, a_blockZ, ChunkX, ChunkZ);
	cChunkPtr Chunk = GetChunkNoGen(ChunkX, 0, ChunkZ);
	if (Chunk == NULL)
	{
		return;
	}
	// It's perfectly legal to broadcast packets even to invalid chunks!
	Chunk->BroadcastBlockBreakAnimation(a_entityID, a_blockX, a_blockY, a_blockZ, a_stage, a_Exclude);
}





void cChunkMap::BroadcastBlockEntity(int a_BlockX, int a_BlockY, int a_BlockZ, const cClientHandle * a_Exclude)
{
	cCSLock Lock(m_CSLayers);
	int ChunkX, ChunkZ;
	cChunkDef::BlockToChunk(a_BlockX, a_BlockZ, ChunkX, ChunkZ);
	cChunkPtr Chunk = GetChunkNoGen(ChunkX, 0, ChunkZ);
	if ((Chunk == NULL) || !Chunk->IsValid())
	{
		return;
	}
	Chunk->BroadcastBlockEntity(a_BlockX, a_BlockY, a_BlockZ, a_Exclude);
}





void cChunkMap::BroadcastChunkData(int a_ChunkX, int a_ChunkZ, cChunkDataSerializer & a_Serializer, const cClientHandle * a_Exclude)
{
	cCSLock Lock(m_CSLayers);
	cChunkPtr Chunk = GetChunkNoGen(a_ChunkX, 0, a_ChunkZ);
	if (Chunk == NULL)
	{
		return;
	}
	// It's perfectly legal to broadcast packets even to invalid chunks!
	Chunk->BroadcastChunkData(a_Serializer, a_Exclude);
}





void cChunkMap::BroadcastCollectPickup(const cPickup & a_Pickup, const cPlayer & a_Player, const cClientHandle * a_Exclude)
{
	cCSLock Lock(m_CSLayers);
	cChunkPtr Chunk = GetChunkNoGen(a_Pickup.GetChunkX(), ZERO_CHUNK_Y, a_Pickup.GetChunkZ());
	if (Chunk == NULL)
	{
		return;
	}
	// It's perfectly legal to broadcast packets even to invalid chunks!
	Chunk->BroadcastCollectPickup(a_Pickup, a_Player, a_Exclude);
}





void cChunkMap::BroadcastDestroyEntity(const cEntity & a_Entity, const cClientHandle * a_Exclude)
{
	cCSLock Lock(m_CSLayers);
	cChunkPtr Chunk = GetChunkNoGen(a_Entity.GetChunkX(), ZERO_CHUNK_Y, a_Entity.GetChunkZ());
	if (Chunk == NULL)
	{
		return;
	}
	// It's perfectly legal to broadcast packets even to invalid chunks!
	Chunk->BroadcastDestroyEntity(a_Entity, a_Exclude);
}





void cChunkMap::BroadcastEntityEffect(const cEntity & a_Entity, int a_EffectID, int a_Amplifier, short a_Duration, const cClientHandle * a_Exclude)
{
	cCSLock Lock(m_CSLayers);
	cChunkPtr Chunk = GetChunkNoGen(a_Entity.GetChunkX(), ZERO_CHUNK_Y, a_Entity.GetChunkZ());
	if (Chunk == NULL)
	{
		return;
	}
	// It's perfectly legal to broadcast packets even to invalid chunks!
	Chunk->BroadcastEntityEffect(a_Entity, a_EffectID, a_Amplifier, a_Duration);
}





void cChunkMap::BroadcastEntityEquipment(const cEntity & a_Entity, short a_SlotNum, const cItem & a_Item, const cClientHandle * a_Exclude)
{
	cCSLock Lock(m_CSLayers);
	cChunkPtr Chunk = GetChunkNoGen(a_Entity.GetChunkX(), ZERO_CHUNK_Y, a_Entity.GetChunkZ());
	if (Chunk == NULL)
	{
		return;
	}
	// It's perfectly legal to broadcast packets even to invalid chunks!
	Chunk->BroadcastEntityEquipment(a_Entity, a_SlotNum, a_Item, a_Exclude);
}





void cChunkMap::BroadcastEntityHeadLook(const cEntity & a_Entity, const cClientHandle * a_Exclude)
{
	cCSLock Lock(m_CSLayers);
	cChunkPtr Chunk = GetChunkNoGen(a_Entity.GetChunkX(), ZERO_CHUNK_Y, a_Entity.GetChunkZ());
	if (Chunk == NULL)
	{
		return;
	}
	// It's perfectly legal to broadcast packets even to invalid chunks!
	Chunk->BroadcastEntityHeadLook(a_Entity, a_Exclude);
}





void cChunkMap::BroadcastEntityLook(const cEntity & a_Entity, const cClientHandle * a_Exclude)
{
	cCSLock Lock(m_CSLayers);
	cChunkPtr Chunk = GetChunkNoGen(a_Entity.GetChunkX(), ZERO_CHUNK_Y, a_Entity.GetChunkZ());
	if (Chunk == NULL)
	{
		return;
	}
	// It's perfectly legal to broadcast packets even to invalid chunks!
	Chunk->BroadcastEntityLook(a_Entity, a_Exclude);
}





void cChunkMap::BroadcastEntityMetadata(const cEntity & a_Entity, const cClientHandle * a_Exclude)
{
	cCSLock Lock(m_CSLayers);
	cChunkPtr Chunk = GetChunkNoGen(a_Entity.GetChunkX(), ZERO_CHUNK_Y, a_Entity.GetChunkZ());
	if (Chunk == NULL)
	{
		return;
	}
	// It's perfectly legal to broadcast packets even to invalid chunks!
	Chunk->BroadcastEntityMetadata(a_Entity, a_Exclude);
}





void cChunkMap::BroadcastEntityRelMove(const cEntity & a_Entity, char a_RelX, char a_RelY, char a_RelZ, const cClientHandle * a_Exclude)
{
	cCSLock Lock(m_CSLayers);
	cChunkPtr Chunk = GetChunkNoGen(a_Entity.GetChunkX(), ZERO_CHUNK_Y, a_Entity.GetChunkZ());
	if (Chunk == NULL)
	{
		return;
	}
	// It's perfectly legal to broadcast packets even to invalid chunks!
	Chunk->BroadcastEntityRelMove(a_Entity, a_RelX, a_RelY, a_RelZ, a_Exclude);
}





void cChunkMap::BroadcastEntityRelMoveLook(const cEntity & a_Entity, char a_RelX, char a_RelY, char a_RelZ, const cClientHandle * a_Exclude)
{
	cCSLock Lock(m_CSLayers);
	cChunkPtr Chunk = GetChunkNoGen(a_Entity.GetChunkX(), ZERO_CHUNK_Y, a_Entity.GetChunkZ());
	if (Chunk == NULL)
	{
		return;
	}
	// It's perfectly legal to broadcast packets even to invalid chunks!
	Chunk->BroadcastEntityRelMoveLook(a_Entity, a_RelX, a_RelY, a_RelZ, a_Exclude);
}





void cChunkMap::BroadcastEntityStatus(const cEntity & a_Entity, char a_Status, const cClientHandle * a_Exclude)
{
	cCSLock Lock(m_CSLayers);
	cChunkPtr Chunk = GetChunkNoGen(a_Entity.GetChunkX(), ZERO_CHUNK_Y, a_Entity.GetChunkZ());
	if (Chunk == NULL)
	{
		return;
	}
	// It's perfectly legal to broadcast packets even to invalid chunks!
	Chunk->BroadcastEntityStatus(a_Entity, a_Status, a_Exclude);
}





void cChunkMap::BroadcastEntityVelocity(const cEntity & a_Entity, const cClientHandle * a_Exclude)
{
	cCSLock Lock(m_CSLayers);
	cChunkPtr Chunk = GetChunkNoGen(a_Entity.GetChunkX(), ZERO_CHUNK_Y, a_Entity.GetChunkZ());
	if (Chunk == NULL)
	{
		return;
	}
	// It's perfectly legal to broadcast packets even to invalid chunks!
	Chunk->BroadcastEntityVelocity(a_Entity, a_Exclude);
}





void cChunkMap::BroadcastEntityAnimation(const cEntity & a_Entity, char a_Animation, const cClientHandle * a_Exclude)
{
	cCSLock Lock(m_CSLayers);
	cChunkPtr Chunk = GetChunkNoGen(a_Entity.GetChunkX(), ZERO_CHUNK_Y, a_Entity.GetChunkZ());
	if (Chunk == NULL)
	{
		return;
	}
	// It's perfectly legal to broadcast packets even to invalid chunks!
	Chunk->BroadcastEntityAnimation(a_Entity, a_Animation, a_Exclude);
}





void cChunkMap::BroadcastParticleEffect(const AString & a_ParticleName, float a_SrcX, float a_SrcY, float a_SrcZ, float a_OffsetX, float a_OffsetY, float a_OffsetZ, float a_ParticleData, int a_ParticleAmmount, cClientHandle * a_Exclude)
{
	cCSLock Lock(m_CSLayers);
	int ChunkX, ChunkZ;

	cChunkDef::BlockToChunk((int) a_SrcX, (int) a_SrcZ, ChunkX, ChunkZ);
	cChunkPtr Chunk = GetChunkNoGen(ChunkX, 0, ChunkZ);
	if (Chunk == NULL)
	{
		return;
	}
	// It's perfectly legal to broadcast packets even to invalid chunks!
	Chunk->BroadcastParticleEffect(a_ParticleName, a_SrcX, a_SrcY, a_SrcZ, a_OffsetX, a_OffsetY, a_OffsetZ, a_ParticleData, a_ParticleAmmount, a_Exclude);
}





void cChunkMap::BroadcastRemoveEntityEffect(const cEntity & a_Entity, int a_EffectID, const cClientHandle * a_Exclude)
{
	cCSLock Lock(m_CSLayers);
	
	cChunkPtr Chunk = GetChunkNoGen(a_Entity.GetChunkX(), ZERO_CHUNK_Y, a_Entity.GetChunkZ());
	if (Chunk == NULL)
	{
		return;
	}
	// It's perfectly legal to broadcast packets even to invalid chunks!
	Chunk->BroadcastRemoveEntityEffect(a_Entity, a_EffectID, a_Exclude);
}





void cChunkMap::BroadcastSoundEffect(const AString & a_SoundName, int a_SrcX, int a_SrcY, int a_SrcZ, float a_Volume, float a_Pitch, const cClientHandle * a_Exclude)
{
	cCSLock Lock(m_CSLayers);
	int ChunkX, ChunkZ;

	cChunkDef::BlockToChunk(a_SrcX / 8, a_SrcZ / 8, ChunkX, ChunkZ);
	cChunkPtr Chunk = GetChunkNoGen(ChunkX, 0, ChunkZ);
	if (Chunk == NULL)
	{
		return;
	}
	// It's perfectly legal to broadcast packets even to invalid chunks!
	Chunk->BroadcastSoundEffect(a_SoundName, a_SrcX, a_SrcY, a_SrcZ, a_Volume, a_Pitch, a_Exclude);
}





void cChunkMap::BroadcastSoundParticleEffect(int a_EffectID, int a_SrcX, int a_SrcY, int a_SrcZ, int a_Data, const cClientHandle * a_Exclude)
{
	cCSLock Lock(m_CSLayers);
	int ChunkX, ChunkZ;

	cChunkDef::BlockToChunk(a_SrcX, a_SrcZ, ChunkX, ChunkZ);
	cChunkPtr Chunk = GetChunkNoGen(ChunkX, 0, ChunkZ);
	if (Chunk == NULL)
	{
		return;
	}
	// It's perfectly legal to broadcast packets even to invalid chunks!
	Chunk->BroadcastSoundParticleEffect(a_EffectID, a_SrcX, a_SrcY, a_SrcZ, a_Data, a_Exclude);
}





void cChunkMap::BroadcastSpawnEntity(cEntity & a_Entity, const cClientHandle * a_Exclude)
{
	cCSLock Lock(m_CSLayers);
	cChunkPtr Chunk = GetChunkNoGen(a_Entity.GetChunkX(), ZERO_CHUNK_Y, a_Entity.GetChunkZ());
	if (Chunk == NULL)
	{
		return;
	}
	// It's perfectly legal to broadcast packets even to invalid chunks!
	Chunk->BroadcastSpawnEntity(a_Entity, a_Exclude);
}





void cChunkMap::BroadcastThunderbolt(int a_BlockX, int a_BlockY, int a_BlockZ, const cClientHandle * a_Exclude)
{
	cCSLock Lock(m_CSLayers);
	int ChunkX, ChunkZ;
	cChunkDef::BlockToChunk(a_BlockX, a_BlockZ, ChunkX, ChunkZ);
	cChunkPtr Chunk = GetChunkNoGen(ChunkX, 0, ChunkZ);
	if (Chunk == NULL)
	{
		return;
	}
	// It's perfectly legal to broadcast packets even to invalid chunks!
	Chunk->BroadcastThunderbolt(a_BlockX, a_BlockY, a_BlockZ, a_Exclude);
}





void cChunkMap::BroadcastUseBed(const cEntity & a_Entity, int a_BlockX, int a_BlockY, int a_BlockZ )
{
	cCSLock Lock(m_CSLayers);
	int ChunkX, ChunkZ;

	cChunkDef::BlockToChunk(a_BlockX, a_BlockZ, ChunkX, ChunkZ);
	cChunkPtr Chunk = GetChunkNoGen(ChunkX, 0, ChunkZ);
	if (Chunk == NULL)
	{
		return;
	}
	// It's perfectly legal to broadcast packets even to invalid chunks!
	Chunk->BroadcastUseBed(a_Entity, a_BlockX, a_BlockY, a_BlockZ);
}





void cChunkMap::SendBlockEntity(int a_BlockX, int a_BlockY, int a_BlockZ, cClientHandle & a_Client)
{
	cCSLock Lock(m_CSLayers);
	int ChunkX, ChunkZ;
	cChunkDef::BlockToChunk(a_BlockX, a_BlockZ, ChunkX, ChunkZ);
	cChunkPtr Chunk = GetChunkNoGen(ChunkX, 0, ChunkZ);
	if ((Chunk == NULL) || !Chunk->IsValid())
	{
		return;
	}
	Chunk->SendBlockEntity(a_BlockX, a_BlockY, a_BlockZ, a_Client);
}





void cChunkMap::UseBlockEntity(cPlayer * a_Player, int a_BlockX, int a_BlockY, int a_BlockZ)
{
	// a_Player rclked block entity at the coords specified, handle it
	cCSLock Lock(m_CSLayers);
	int ChunkX, ChunkZ;
	cChunkDef::BlockToChunk(a_BlockX, a_BlockZ, ChunkX, ChunkZ);
	cChunkPtr Chunk = GetChunkNoGen(ChunkX, 0, ChunkZ);
	if ((Chunk == NULL) || !Chunk->IsValid())
	{
		return;
	}
	Chunk->UseBlockEntity(a_Player, a_BlockX, a_BlockY, a_BlockZ);
}





bool cChunkMap::DoWithChunk(int a_ChunkX, int a_ChunkZ, cChunkCallback & a_Callback)
{
	cCSLock Lock(m_CSLayers);
	cChunkPtr Chunk = GetChunkNoLoad(a_ChunkX, ZERO_CHUNK_Y, a_ChunkZ);
	if (Chunk == NULL)
	{
		return false;
	}
	return a_Callback.Item(Chunk);
}





void cChunkMap::WakeUpSimulators(int a_BlockX, int a_BlockY, int a_BlockZ)
{
	cCSLock Lock(m_CSLayers);
	int ChunkX, ChunkZ;
	cChunkDef::BlockToChunk(a_BlockX, a_BlockZ, ChunkX, ChunkZ);
	cChunkPtr Chunk = GetChunkNoGen(ChunkX, 0, ChunkZ);
	if ((Chunk == NULL) || !Chunk->IsValid())
	{
		return;
	}
	m_World->GetSimulatorManager()->WakeUp(a_BlockX, a_BlockY, a_BlockZ, Chunk);
}





/// Wakes up the simulators for the specified area of blocks
void cChunkMap::WakeUpSimulatorsInArea(int a_MinBlockX, int a_MaxBlockX, int a_MinBlockY, int a_MaxBlockY, int a_MinBlockZ, int a_MaxBlockZ)
{
	cSimulatorManager * SimMgr = m_World->GetSimulatorManager();
	int MinChunkX, MinChunkZ, MaxChunkX, MaxChunkZ;
	cChunkDef::BlockToChunk(a_MinBlockX, a_MinBlockZ, MinChunkX, MinChunkZ);
	cChunkDef::BlockToChunk(a_MaxBlockX, a_MaxBlockZ, MaxChunkX, MaxChunkZ);
	for (int z = MinChunkZ; z <= MaxChunkZ; z++)
	{
		int MinZ = std::max(a_MinBlockZ, z * cChunkDef::Width);
		int MaxZ = std::min(a_MaxBlockZ, z * cChunkDef::Width + cChunkDef::Width - 1);
		for (int x = MinChunkX; x <= MaxChunkX; x++)
		{
			cChunkPtr Chunk = GetChunkNoGen(x, 0, z);
			if ((Chunk == NULL) || !Chunk->IsValid())
			{
				continue;
			}
			int MinX = std::max(a_MinBlockX, x * cChunkDef::Width);
			int MaxX = std::min(a_MaxBlockX, x * cChunkDef::Width + cChunkDef::Width - 1);
			for (int BlockY = a_MinBlockY; BlockY <= a_MaxBlockY; BlockY++)
			{
				for (int BlockZ = MinZ; BlockZ <= MaxZ; BlockZ++)
				{
					for (int BlockX = MinX; BlockX <= MaxX; BlockX++)
					{
						SimMgr->WakeUp(BlockX, BlockY, BlockZ, Chunk);
					}  // for BlockX
				}  // for BlockZ
			}  // for BlockY
		}  // for x - chunks
	}  // for z = chunks
}





void cChunkMap::MarkChunkDirty (int a_ChunkX, int a_ChunkZ)
{
	cCSLock Lock(m_CSLayers);
	cChunkPtr Chunk = GetChunkNoGen(a_ChunkX, ZERO_CHUNK_Y, a_ChunkZ);
	if ((Chunk == NULL) || !Chunk->IsValid())
	{
		return;
	}
	Chunk->MarkDirty();
}





void cChunkMap::MarkChunkSaving(int a_ChunkX, int a_ChunkZ)
{
	cCSLock Lock(m_CSLayers);
	cChunkPtr Chunk = GetChunkNoGen(a_ChunkX, ZERO_CHUNK_Y, a_ChunkZ);
	if ((Chunk == NULL) || !Chunk->IsValid())
	{
		return;
	}
	Chunk->MarkSaving();
}





void cChunkMap::MarkChunkSaved (int a_ChunkX, int a_ChunkZ)
{
	cCSLock Lock(m_CSLayers);
	cChunkPtr Chunk = GetChunkNoGen(a_ChunkX, ZERO_CHUNK_Y, a_ChunkZ);
	if ((Chunk == NULL) || !Chunk->IsValid())
	{
		return;
	}
	Chunk->MarkSaved();
}





void cChunkMap::SetChunkData(
	int a_ChunkX, int a_ChunkZ,
	const BLOCKTYPE *  a_BlockTypes,
	const NIBBLETYPE * a_BlockMeta,
	const NIBBLETYPE * a_BlockLight,
	const NIBBLETYPE * a_BlockSkyLight,
	const cChunkDef::HeightMap * a_HeightMap,
	const cChunkDef::BiomeMap &  a_BiomeMap,
	cBlockEntityList & a_BlockEntities,
	bool a_MarkDirty
)
{
	cCSLock Lock(m_CSLayers);
	cChunkPtr Chunk = GetChunkNoLoad(a_ChunkX, ZERO_CHUNK_Y, a_ChunkZ);
	if (Chunk == NULL)
	{
		return;
	}
	Chunk->SetAllData(a_BlockTypes, a_BlockMeta, a_BlockLight, a_BlockSkyLight, a_HeightMap, a_BiomeMap, a_BlockEntities);
	
	if (a_MarkDirty)
	{
		Chunk->MarkDirty();
	}

	// Notify plugins of the chunk becoming available
	cPluginManager::Get()->CallHookChunkAvailable(m_World, a_ChunkX, a_ChunkZ);
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





bool cChunkMap::GetChunkData(int a_ChunkX, int a_ChunkZ, cChunkDataCallback & a_Callback)
{
	cCSLock Lock(m_CSLayers);
	cChunkPtr Chunk = GetChunkNoGen(a_ChunkX, ZERO_CHUNK_Y, a_ChunkZ);
	if ((Chunk == NULL) || !Chunk->IsValid())
	{
		return false;
	}
	Chunk->GetAllData(a_Callback);
	return true;
}





bool cChunkMap::GetChunkBlockTypes(int a_ChunkX, int a_ChunkZ, BLOCKTYPE * a_BlockTypes)
{
	cCSLock Lock(m_CSLayers);
	cChunkPtr Chunk = GetChunkNoGen(a_ChunkX, ZERO_CHUNK_Y, a_ChunkZ);
	if ((Chunk == NULL) || !Chunk->IsValid())
	{
		return false;
	}
	Chunk->GetBlockTypes(a_BlockTypes);
	return true;
}





bool cChunkMap::IsChunkValid(int a_ChunkX, int a_ChunkZ)
{
	cCSLock Lock(m_CSLayers);
	cChunkPtr Chunk = GetChunkNoLoad(a_ChunkX, ZERO_CHUNK_Y, a_ChunkZ);
	return (Chunk != NULL) && Chunk->IsValid();
}





bool cChunkMap::HasChunkAnyClients(int a_ChunkX, int a_ChunkZ)
{
	cCSLock Lock(m_CSLayers);
	cChunkPtr Chunk = GetChunkNoGen(a_ChunkX, ZERO_CHUNK_Y, a_ChunkZ);
	return (Chunk != NULL) && Chunk->HasAnyClients();
}





int  cChunkMap::GetHeight(int a_BlockX, int a_BlockZ)
{
	for (;;)
	{
		cCSLock Lock(m_CSLayers);
		int ChunkX, ChunkZ, BlockY = 0;
		cChunkDef::AbsoluteToRelative(a_BlockX, BlockY, a_BlockZ, ChunkX, ChunkZ);
		cChunkPtr Chunk = GetChunk(ChunkX, ZERO_CHUNK_Y, ChunkZ);
		if (Chunk == NULL)
		{
			return 0;
		}
		
		if (Chunk->IsValid())
		{
			return Chunk->GetHeight(a_BlockX, a_BlockZ);
		}

		// The chunk is not valid, wait for it to become valid:
		cCSUnlock Unlock(Lock);
		m_evtChunkValid.Wait();
	}  // while (true)
}





bool cChunkMap::TryGetHeight(int a_BlockX, int a_BlockZ, int & a_Height)
{
	// Returns false if chunk not loaded / generated
	cCSLock Lock(m_CSLayers);
	int ChunkX, ChunkZ, BlockY = 0;
	cChunkDef::AbsoluteToRelative(a_BlockX, BlockY, a_BlockZ, ChunkX, ChunkZ);
	cChunkPtr Chunk = GetChunkNoLoad(ChunkX, ZERO_CHUNK_Y, ChunkZ);
	if ((Chunk == NULL) || !Chunk->IsValid())
	{
		return false;
	}
	a_Height = Chunk->GetHeight(a_BlockX, a_BlockZ);
	return true;
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





BLOCKTYPE cChunkMap::GetBlock(int a_BlockX, int a_BlockY, int a_BlockZ)
{
	int ChunkX, ChunkZ;
	cChunkDef::AbsoluteToRelative(a_BlockX, a_BlockY, a_BlockZ, ChunkX, ChunkZ );
	
	cCSLock Lock(m_CSLayers);
	cChunkPtr Chunk = GetChunk(ChunkX, ZERO_CHUNK_Y, ChunkZ);
	if ((Chunk != NULL) && Chunk->IsValid())
	{
		return Chunk->GetBlock(a_BlockX, a_BlockY, a_BlockZ);
	}
	return 0;
}





NIBBLETYPE cChunkMap::GetBlockMeta(int a_BlockX, int a_BlockY, int a_BlockZ)
{
	int ChunkX, ChunkZ;
	cChunkDef::AbsoluteToRelative(a_BlockX, a_BlockY, a_BlockZ, ChunkX, ChunkZ );
	
	cCSLock Lock(m_CSLayers);
	cChunkPtr Chunk = GetChunk( ChunkX, ZERO_CHUNK_Y, ChunkZ );
	if ((Chunk != NULL) && Chunk->IsValid() )
	{
		return Chunk->GetMeta(a_BlockX, a_BlockY, a_BlockZ);
	}
	return 0;
}





NIBBLETYPE cChunkMap::GetBlockSkyLight(int a_BlockX, int a_BlockY, int a_BlockZ)
{
	int ChunkX, ChunkZ;
	cChunkDef::AbsoluteToRelative(a_BlockX, a_BlockY, a_BlockZ, ChunkX, ChunkZ );

	cCSLock Lock(m_CSLayers);
	cChunkPtr Chunk = GetChunk( ChunkX, ZERO_CHUNK_Y, ChunkZ );
	if ((Chunk != NULL) && Chunk->IsValid() )
	{
		return Chunk->GetSkyLight(a_BlockX, a_BlockY, a_BlockZ);
	}
	return 0;
}





NIBBLETYPE cChunkMap::GetBlockBlockLight(int a_BlockX, int a_BlockY, int a_BlockZ)
{
	int ChunkX, ChunkZ;
	cChunkDef::AbsoluteToRelative(a_BlockX, a_BlockY, a_BlockZ, ChunkX, ChunkZ );

	cCSLock Lock(m_CSLayers);
	cChunkPtr Chunk = GetChunk( ChunkX, ZERO_CHUNK_Y, ChunkZ );
	if ((Chunk != NULL) && Chunk->IsValid() )
	{
		return Chunk->GetBlockLight(a_BlockX, a_BlockY, a_BlockZ);
	}
	return 0;
}





void cChunkMap::SetBlockMeta(int a_BlockX, int a_BlockY, int a_BlockZ, NIBBLETYPE a_BlockMeta)
{
	int ChunkX, ChunkZ;
	cChunkDef::AbsoluteToRelative(a_BlockX, a_BlockY, a_BlockZ, ChunkX, ChunkZ);
	// a_BlockXYZ now contains relative coords!

	cCSLock Lock(m_CSLayers);
	cChunkPtr Chunk = GetChunk(ChunkX, ZERO_CHUNK_Y, ChunkZ);
	if ((Chunk != NULL) && Chunk->IsValid())
	{
		Chunk->SetMeta(a_BlockX, a_BlockY, a_BlockZ, a_BlockMeta);
		Chunk->MarkDirty();
		Chunk->SendBlockTo(a_BlockX, a_BlockY, a_BlockZ, NULL);
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
		m_World->GetSimulatorManager()->WakeUp(a_BlockX, a_BlockY, a_BlockZ, Chunk);
	}
}





void cChunkMap::QueueSetBlock(int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE a_BlockType, BLOCKTYPE a_BlockMeta, Int64 a_Tick, BLOCKTYPE a_PreviousBlockType)
{
	int ChunkX, ChunkZ, X = a_BlockX, Y = a_BlockY, Z = a_BlockZ;
	cChunkDef::AbsoluteToRelative(X, Y, Z, ChunkX, ChunkZ);

	cCSLock Lock(m_CSLayers);
	cChunkPtr Chunk = GetChunk(ChunkX, ZERO_CHUNK_Y, ChunkZ);
	if ((Chunk != NULL) && Chunk->IsValid())
	{
		Chunk->QueueSetBlock(X, Y, Z, a_BlockType, a_BlockMeta, a_Tick, a_PreviousBlockType);
	}
}





bool cChunkMap::GetBlockTypeMeta(int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE & a_BlockType, NIBBLETYPE & a_BlockMeta)
{
	int ChunkX, ChunkZ, X = a_BlockX, Y = a_BlockY, Z = a_BlockZ;
	cChunkDef::AbsoluteToRelative( X, Y, Z, ChunkX, ChunkZ );

	cCSLock Lock(m_CSLayers);
	cChunkPtr Chunk = GetChunk( ChunkX, ZERO_CHUNK_Y, ChunkZ );
	if ((Chunk != NULL) && Chunk->IsValid())
	{
		Chunk->GetBlockTypeMeta(X, Y, Z, a_BlockType, a_BlockMeta);
		return true;
	}
	return false;
}





bool cChunkMap::GetBlockInfo(int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE & a_BlockType, NIBBLETYPE & a_Meta, NIBBLETYPE & a_SkyLight, NIBBLETYPE & a_BlockLight)
{
	int ChunkX, ChunkZ, X = a_BlockX, Y = a_BlockY, Z = a_BlockZ;
	cChunkDef::AbsoluteToRelative( X, Y, Z, ChunkX, ChunkZ );

	cCSLock Lock(m_CSLayers);
	cChunkPtr Chunk = GetChunk( ChunkX, ZERO_CHUNK_Y, ChunkZ );
	if ((Chunk != NULL) && Chunk->IsValid())
	{
		Chunk->GetBlockInfo(X, Y, Z, a_BlockType, a_Meta, a_SkyLight, a_BlockLight);
		return true;
	}
	return false;
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
			case E_BLOCK_LEAVES:
			{
				if (itr->BlockType == E_BLOCK_LOG)
				{
					Chunk->SetBlock(itr->x, itr->y, itr->z, itr->BlockType, itr->BlockMeta);
				}
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
		m_World->GetSimulatorManager()->WakeUp(a_X, a_Y, a_Z, DestChunk);
	}

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





void cChunkMap::CompareChunkClients(int a_ChunkX1, int a_ChunkZ1, int a_ChunkX2, int a_ChunkZ2, cClientDiffCallback & a_Callback)
{
	cCSLock Lock(m_CSLayers);
	cChunkPtr Chunk1 = GetChunkNoGen(a_ChunkX1, ZERO_CHUNK_Y, a_ChunkZ1);
	if (Chunk1 == NULL)
	{
		return;
	}
	cChunkPtr Chunk2 = GetChunkNoGen(a_ChunkX2, ZERO_CHUNK_Y, a_ChunkZ2);
	if (Chunk2 == NULL)
	{
		return;
	}
	
	CompareChunkClients(Chunk1, Chunk2, a_Callback);
}





void cChunkMap::CompareChunkClients(cChunk * a_Chunk1, cChunk * a_Chunk2, cClientDiffCallback & a_Callback)
{
	cClientHandleList Clients1(a_Chunk1->GetAllClients());
	cClientHandleList Clients2(a_Chunk2->GetAllClients());
	
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





bool cChunkMap::AddChunkClient(int a_ChunkX, int a_ChunkZ, cClientHandle * a_Client)
{
	cCSLock Lock(m_CSLayers);
	cChunkPtr Chunk = GetChunk(a_ChunkX, ZERO_CHUNK_Y, a_ChunkZ);
	if (Chunk == NULL)
	{
		return false;
	}
	return Chunk->AddClient(a_Client);
}





void cChunkMap::RemoveChunkClient(int a_ChunkX, int a_ChunkZ, cClientHandle * a_Client)
{
	cCSLock Lock(m_CSLayers);
	cChunkPtr Chunk = GetChunkNoGen(a_ChunkX, ZERO_CHUNK_Y, a_ChunkZ);
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





void cChunkMap::AddEntity(cEntity * a_Entity)
{
	cCSLock Lock(m_CSLayers);
	cChunkPtr Chunk = GetChunkNoGen(a_Entity->GetChunkX(), ZERO_CHUNK_Y, a_Entity->GetChunkZ());
	if ((Chunk == NULL) && !Chunk->IsValid())
	{
		LOGWARNING("Entity at %p (%s, ID %d) spawning in a non-existent chunk, the entity is lost.",
			a_Entity, a_Entity->GetClass(), a_Entity->GetUniqueID()
		);
		return;
	}
	Chunk->AddEntity(a_Entity);
}





bool cChunkMap::HasEntity(int a_UniqueID)
{
	cCSLock Lock(m_CSLayers);
	for (cChunkLayerList::const_iterator itr = m_Layers.begin(); itr != m_Layers.end(); ++itr)
	{
		if ((*itr)->HasEntity(a_UniqueID))
		{
			return true;
		}
	}
	return false;
}





void cChunkMap::RemoveEntity(cEntity * a_Entity)
{
	cCSLock Lock(m_CSLayers);
	cChunkPtr Chunk = GetChunkNoGen(a_Entity->GetChunkX(), ZERO_CHUNK_Y, a_Entity->GetChunkZ());
	if ((Chunk == NULL) && !Chunk->IsValid())
	{
		return;
	}
	Chunk->RemoveEntity(a_Entity);
}





bool cChunkMap::ForEachEntity(cEntityCallback & a_Callback)
{
	cCSLock Lock(m_CSLayers);
	for (cChunkLayerList::const_iterator itr = m_Layers.begin(); itr != m_Layers.end(); ++itr)
	{
		if (!(*itr)->ForEachEntity(a_Callback))
		{
			return false;
		}
	}
	return true;
}





bool cChunkMap::ForEachEntityInChunk(int a_ChunkX, int a_ChunkZ, cEntityCallback & a_Callback)
{
	cCSLock Lock(m_CSLayers);
	cChunkPtr Chunk = GetChunkNoGen(a_ChunkX, ZERO_CHUNK_Y, a_ChunkZ);
	if ((Chunk == NULL) && !Chunk->IsValid())
	{
		return false;
	}
	return Chunk->ForEachEntity(a_Callback);
}





void cChunkMap::DoExplosionAt(double a_ExplosionSize, double a_BlockX, double a_BlockY, double a_BlockZ, cVector3iArray & a_BlocksAffected)
{
	// Don't explode if outside of Y range (prevents the following test running into unallocated memory):
	if ((a_BlockY < 0) || (a_BlockY > cChunkDef::Height - 1))
	{
		return;
	}
	
	// Don't explode if the explosion center is inside a liquid block:
	switch (m_World->GetBlock((int)floor(a_BlockX), (int)floor(a_BlockY), (int)floor(a_BlockZ)))
	{
		case E_BLOCK_WATER:
		case E_BLOCK_STATIONARY_WATER:
		case E_BLOCK_LAVA:
		case E_BLOCK_STATIONARY_LAVA:
		{
			return;
		}
	}
	
	cBlockArea area;
	int bx = (int)floor(a_BlockX);
	int by = (int)floor(a_BlockY);
	int bz = (int)floor(a_BlockZ);
	int ExplosionSizeInt = (int) ceil(a_ExplosionSize);
	int ExplosionSizeSq =  ExplosionSizeInt * ExplosionSizeInt;
	a_BlocksAffected.reserve(8 * ExplosionSizeInt * ExplosionSizeInt * ExplosionSizeInt);
	int MinY = std::max((int)floor(a_BlockY - ExplosionSizeInt), 0);
	int MaxY = std::min((int)ceil(a_BlockY + ExplosionSizeInt), cChunkDef::Height - 1);
	area.Read(m_World, bx - ExplosionSizeInt, (int)ceil(a_BlockX + ExplosionSizeInt), MinY, MaxY, bz - ExplosionSizeInt, (int)ceil(a_BlockZ + ExplosionSizeInt));
	for (int x = -ExplosionSizeInt; x < ExplosionSizeInt; x++)
	{
		for (int y = -ExplosionSizeInt; y < ExplosionSizeInt; y++)
		{
			if ((by + y >= cChunkDef::Height) || (by + y < 0))
			{
				// Outside of the world
				continue;
			}
			for (int z = -ExplosionSizeInt; z < ExplosionSizeInt; z++)
			{
				if ((x * x + y * y + z * z) > ExplosionSizeSq)
				{
					// Too far away
					continue;
				}

				BLOCKTYPE Block = area.GetBlockType(bx + x, by + y, bz + z);
				switch (Block)
				{
					case E_BLOCK_TNT:
					{
						// Activate the TNT, with a random fuse between 10 to 30 game ticks
						double FuseTime = (double)(10 + m_World->GetTickRandomNumber(20)) / 20;
						m_World->SpawnPrimedTNT(a_BlockX + x + 0.5, a_BlockY + y + 0.5, a_BlockZ + z + 0.5, FuseTime);
						area.SetBlockType(bx + x, by + y, bz + z, E_BLOCK_AIR);
						a_BlocksAffected.push_back(Vector3i(bx + x, by + y, bz + z));
						break;
					}
					case E_BLOCK_OBSIDIAN:
					case E_BLOCK_BEDROCK:
					case E_BLOCK_WATER:
					case E_BLOCK_LAVA:
					{
						// These blocks are not affected by explosions
						break;
					}

					case E_BLOCK_STATIONARY_WATER:
					{
						// Turn into simulated water:
						area.SetBlockType(bx + x, by + y, bz + z, E_BLOCK_WATER);
						break;
					}
					
					case E_BLOCK_STATIONARY_LAVA:
					{
						// Turn into simulated lava:
						area.SetBlockType(bx + x, by + y, bz + z, E_BLOCK_LAVA);
						break;
					}
					
					case E_BLOCK_AIR:
					{
						// No pickups for air
						break;
					}
					
					default:
					{
						if (m_World->GetTickRandomNumber(100) <= 25) // 25% chance of pickups
						{
							cItems Drops;
							cBlockHandler * Handler = BlockHandler(Block);

							Handler->ConvertToPickups(Drops, area.GetBlockMeta(bx + x, by + y, bz + z)); // Stone becomes cobblestone, coal ore becomes coal, etc.
							m_World->SpawnItemPickups(Drops, bx + x, by + y, bz + z);
						}
						area.SetBlockType(bx + x, by + y, bz + z, E_BLOCK_AIR);
						a_BlocksAffected.push_back(Vector3i(bx + x, by + y, bz + z));
					}
				}  // switch (BlockType)
			}  // for z
		}  // for y
	}  // for x
	area.Write(m_World, bx - ExplosionSizeInt, MinY, bz - ExplosionSizeInt);

	// Wake up all simulators for the area, so that water and lava flows and sand falls into the blasted holes (FS #391):
	WakeUpSimulatorsInArea(
		bx - ExplosionSizeInt - 1, bx + ExplosionSizeInt + 1,
		MinY, MaxY,
		bz - ExplosionSizeInt - 1, bz + ExplosionSizeInt + 1
	);
}





bool cChunkMap::DoWithEntityByID(int a_UniqueID, cEntityCallback & a_Callback)
{
	cCSLock Lock(m_CSLayers);
	bool res = false;
	for (cChunkLayerList::const_iterator itr = m_Layers.begin(); itr != m_Layers.end(); ++itr)
	{
		if ((*itr)->DoWithEntityByID(a_UniqueID, a_Callback, res))
		{
			return res;
		}
	}
	return false;
}





bool cChunkMap::ForEachBlockEntityInChunk(int a_ChunkX, int a_ChunkZ, cBlockEntityCallback & a_Callback)
{
	cCSLock Lock(m_CSLayers);
	cChunkPtr Chunk = GetChunkNoGen(a_ChunkX, ZERO_CHUNK_Y, a_ChunkZ);
	if ((Chunk == NULL) && !Chunk->IsValid())
	{
		return false;
	}
	return Chunk->ForEachBlockEntity(a_Callback);
}





bool cChunkMap::ForEachChestInChunk(int a_ChunkX, int a_ChunkZ, cChestCallback & a_Callback)
{
	cCSLock Lock(m_CSLayers);
	cChunkPtr Chunk = GetChunkNoGen(a_ChunkX, ZERO_CHUNK_Y, a_ChunkZ);
	if ((Chunk == NULL) && !Chunk->IsValid())
	{
		return false;
	}
	return Chunk->ForEachChest(a_Callback);
}





bool cChunkMap::ForEachDispenserInChunk(int a_ChunkX, int a_ChunkZ, cDispenserCallback & a_Callback)
{
	cCSLock Lock(m_CSLayers);
	cChunkPtr Chunk = GetChunkNoGen(a_ChunkX, ZERO_CHUNK_Y, a_ChunkZ);
	if ((Chunk == NULL) && !Chunk->IsValid())
	{
		return false;
	}
	return Chunk->ForEachDispenser(a_Callback);
}





bool cChunkMap::ForEachDropperInChunk(int a_ChunkX, int a_ChunkZ, cDropperCallback & a_Callback)
{
	cCSLock Lock(m_CSLayers);
	cChunkPtr Chunk = GetChunkNoGen(a_ChunkX, ZERO_CHUNK_Y, a_ChunkZ);
	if ((Chunk == NULL) && !Chunk->IsValid())
	{
		return false;
	}
	return Chunk->ForEachDropper(a_Callback);
}





bool cChunkMap::ForEachDropSpenserInChunk(int a_ChunkX, int a_ChunkZ, cDropSpenserCallback & a_Callback)
{
	cCSLock Lock(m_CSLayers);
	cChunkPtr Chunk = GetChunkNoGen(a_ChunkX, ZERO_CHUNK_Y, a_ChunkZ);
	if ((Chunk == NULL) && !Chunk->IsValid())
	{
		return false;
	}
	return Chunk->ForEachDropSpenser(a_Callback);
}





bool cChunkMap::ForEachFurnaceInChunk(int a_ChunkX, int a_ChunkZ, cFurnaceCallback & a_Callback)
{
	cCSLock Lock(m_CSLayers);
	cChunkPtr Chunk = GetChunkNoGen(a_ChunkX, ZERO_CHUNK_Y, a_ChunkZ);
	if ((Chunk == NULL) && !Chunk->IsValid())
	{
		return false;
	}
	return Chunk->ForEachFurnace(a_Callback);
}





bool cChunkMap::DoWithBlockEntityAt(int a_BlockX, int a_BlockY, int a_BlockZ, cBlockEntityCallback & a_Callback)
{
	int ChunkX, ChunkZ;
	int BlockX = a_BlockX, BlockY = a_BlockY, BlockZ = a_BlockZ;
	cChunkDef::AbsoluteToRelative(BlockX, BlockY, BlockZ, ChunkX, ChunkZ);
	cCSLock Lock(m_CSLayers);
	cChunkPtr Chunk = GetChunkNoGen(ChunkX, ZERO_CHUNK_Y, ChunkZ);
	if ((Chunk == NULL) && !Chunk->IsValid())
	{
		return false;
	}
	return Chunk->DoWithBlockEntityAt(a_BlockX, a_BlockY, a_BlockZ, a_Callback);
}





bool cChunkMap::DoWithChestAt(int a_BlockX, int a_BlockY, int a_BlockZ, cChestCallback & a_Callback)
{
	int ChunkX, ChunkZ;
	int BlockX = a_BlockX, BlockY = a_BlockY, BlockZ = a_BlockZ;
	cChunkDef::AbsoluteToRelative(BlockX, BlockY, BlockZ, ChunkX, ChunkZ);
	cCSLock Lock(m_CSLayers);
	cChunkPtr Chunk = GetChunkNoGen(ChunkX, ZERO_CHUNK_Y, ChunkZ);
	if ((Chunk == NULL) && !Chunk->IsValid())
	{
		return false;
	}
	return Chunk->DoWithChestAt(a_BlockX, a_BlockY, a_BlockZ, a_Callback);
}





bool cChunkMap::DoWithDispenserAt(int a_BlockX, int a_BlockY, int a_BlockZ, cDispenserCallback & a_Callback)
{
	int ChunkX, ChunkZ;
	int BlockX = a_BlockX, BlockY = a_BlockY, BlockZ = a_BlockZ;
	cChunkDef::AbsoluteToRelative(BlockX, BlockY, BlockZ, ChunkX, ChunkZ);
	cCSLock Lock(m_CSLayers);
	cChunkPtr Chunk = GetChunkNoGen(ChunkX, ZERO_CHUNK_Y, ChunkZ);
	if ((Chunk == NULL) && !Chunk->IsValid())
	{
		return false;
	}
	return Chunk->DoWithDispenserAt(a_BlockX, a_BlockY, a_BlockZ, a_Callback);
}





bool cChunkMap::DoWithDropperAt(int a_BlockX, int a_BlockY, int a_BlockZ, cDropperCallback & a_Callback)
{
	int ChunkX, ChunkZ;
	int BlockX = a_BlockX, BlockY = a_BlockY, BlockZ = a_BlockZ;
	cChunkDef::AbsoluteToRelative(BlockX, BlockY, BlockZ, ChunkX, ChunkZ);
	cCSLock Lock(m_CSLayers);
	cChunkPtr Chunk = GetChunkNoGen(ChunkX, ZERO_CHUNK_Y, ChunkZ);
	if ((Chunk == NULL) && !Chunk->IsValid())
	{
		return false;
	}
	return Chunk->DoWithDropperAt(a_BlockX, a_BlockY, a_BlockZ, a_Callback);
}





bool cChunkMap::DoWithDropSpenserAt(int a_BlockX, int a_BlockY, int a_BlockZ, cDropSpenserCallback & a_Callback)
{
	int ChunkX, ChunkZ;
	int BlockX = a_BlockX, BlockY = a_BlockY, BlockZ = a_BlockZ;
	cChunkDef::AbsoluteToRelative(BlockX, BlockY, BlockZ, ChunkX, ChunkZ);
	cCSLock Lock(m_CSLayers);
	cChunkPtr Chunk = GetChunkNoGen(ChunkX, ZERO_CHUNK_Y, ChunkZ);
	if ((Chunk == NULL) && !Chunk->IsValid())
	{
		return false;
	}
	return Chunk->DoWithDropSpenserAt(a_BlockX, a_BlockY, a_BlockZ, a_Callback);
}





bool cChunkMap::DoWithFurnaceAt(int a_BlockX, int a_BlockY, int a_BlockZ, cFurnaceCallback & a_Callback)
{
	int ChunkX, ChunkZ;
	int BlockX = a_BlockX, BlockY = a_BlockY, BlockZ = a_BlockZ;
	cChunkDef::AbsoluteToRelative(BlockX, BlockY, BlockZ, ChunkX, ChunkZ);
	cCSLock Lock(m_CSLayers);
	cChunkPtr Chunk = GetChunkNoGen(ChunkX, ZERO_CHUNK_Y, ChunkZ);
	if ((Chunk == NULL) && !Chunk->IsValid())
	{
		return false;
	}
	return Chunk->DoWithFurnaceAt(a_BlockX, a_BlockY, a_BlockZ, a_Callback);
}




bool cChunkMap::DoWithNoteBlockAt(int a_BlockX, int a_BlockY, int a_BlockZ, cNoteBlockCallback & a_Callback)
{
	int ChunkX, ChunkZ;
	int BlockX = a_BlockX, BlockY = a_BlockY, BlockZ = a_BlockZ;
	cChunkDef::AbsoluteToRelative(BlockX, BlockY, BlockZ, ChunkX, ChunkZ);
	cCSLock Lock(m_CSLayers);
	cChunkPtr Chunk = GetChunkNoGen(ChunkX, ZERO_CHUNK_Y, ChunkZ);
	if ((Chunk == NULL) && !Chunk->IsValid())
	{
		return false;
	}
	return Chunk->DoWithNoteBlockAt(a_BlockX, a_BlockY, a_BlockZ, a_Callback);
}





bool cChunkMap::GetSignLines(int a_BlockX, int a_BlockY, int a_BlockZ, AString & a_Line1, AString & a_Line2, AString & a_Line3, AString & a_Line4)
{
	int ChunkX, ChunkZ;
	int BlockX = a_BlockX, BlockY = a_BlockY, BlockZ = a_BlockZ;
	cChunkDef::AbsoluteToRelative(BlockX, BlockY, BlockZ, ChunkX, ChunkZ);
	cCSLock Lock(m_CSLayers);
	cChunkPtr Chunk = GetChunkNoGen(ChunkX, ZERO_CHUNK_Y, ChunkZ);
	if ((Chunk == NULL) && !Chunk->IsValid())
	{
		return false;
	}
	return Chunk->GetSignLines(a_BlockX, a_BlockY, a_BlockZ, a_Line1, a_Line2, a_Line3, a_Line4);
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





bool cChunkMap::SetSignLines(int a_BlockX, int a_BlockY, int a_BlockZ, const AString & a_Line1, const AString & a_Line2, const AString & a_Line3, const AString & a_Line4)
{
	cCSLock Lock(m_CSLayers);
	int ChunkX, ChunkZ;
	cChunkDef::BlockToChunk(a_BlockX, a_BlockZ, ChunkX, ChunkZ);
	cChunkPtr Chunk = GetChunkNoGen(ChunkX, ZERO_CHUNK_Y, ChunkZ);
	if ((Chunk == NULL) || !Chunk->IsValid())
	{
		return false;
	}
	return Chunk->SetSignLines(a_BlockX, a_BlockY, a_BlockZ, a_Line1, a_Line2, a_Line3, a_Line4);
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





bool cChunkMap::ForEachChunkInRect(int a_MinChunkX, int a_MaxChunkX, int a_MinChunkZ, int a_MaxChunkZ, cChunkDataCallback & a_Callback)
{
	bool Result = true;
	cCSLock Lock(m_CSLayers);
	for (int z = a_MinChunkZ; z <= a_MaxChunkZ; z++)
	{
		for (int x = a_MinChunkX; x <= a_MaxChunkX; x++)
		{
			cChunkPtr Chunk = GetChunkNoLoad(x, ZERO_CHUNK_Y, z);
			if ((Chunk == NULL) || (!Chunk->IsValid()))
			{
				// Not present / not valid
				Result = false;
				continue;
			}
			if (!a_Callback.Coords(x, z))
			{
				continue;
			}
			Chunk->GetAllData(a_Callback);
		}
	}
	return Result;
}





bool cChunkMap::WriteBlockArea(cBlockArea & a_Area, int a_MinBlockX, int a_MinBlockY, int a_MinBlockZ, int a_DataTypes)
{
	// Convert block coords to chunks coords:
	int MinChunkX, MaxChunkX;
	int MinChunkZ, MaxChunkZ;
	int MinBlockX = a_MinBlockX;
	int MinBlockY = a_MinBlockY;
	int MinBlockZ = a_MinBlockZ;
	int MaxBlockX = a_MinBlockX + a_Area.GetSizeX();
	int MaxBlockY = a_MinBlockY + a_Area.GetSizeY();
	int MaxBlockZ = a_MinBlockZ + a_Area.GetSizeZ();
	cChunkDef::AbsoluteToRelative(MinBlockX, MinBlockY, MinBlockZ, MinChunkX, MinChunkZ);
	cChunkDef::AbsoluteToRelative(MaxBlockX, MaxBlockY, MaxBlockZ, MaxChunkX, MaxChunkZ);
	
	// Iterate over chunks, write data into each:
	bool Result = true;
	cCSLock Lock(m_CSLayers);
	for (int z = MinChunkZ; z <= MaxChunkZ; z++)
	{
		for (int x = MinChunkX; x <= MaxChunkX; x++)
		{
			cChunkPtr Chunk = GetChunkNoLoad(x, ZERO_CHUNK_Y, z);
			if ((Chunk == NULL) || (!Chunk->IsValid()))
			{
				// Not present / not valid
				Result = false;
				continue;
			}
			Chunk->WriteBlockArea(a_Area, a_MinBlockX, a_MinBlockY, a_MinBlockZ, a_DataTypes);
		}  // for x
	}  // for z
	return Result;
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




void cChunkMap::CollectMobCensus(cMobCensus& a_ToFill)
{
	cCSLock Lock(m_CSLayers);
	for (cChunkLayerList::iterator itr = m_Layers.begin(); itr != m_Layers.end(); ++itr)
	{
		(*itr)->CollectMobCensus(a_ToFill);
	}  // for itr - m_Layers
}






void cChunkMap::SpawnMobs(cMobSpawner& a_MobSpawner)
{
	cCSLock Lock(m_CSLayers);
	for (cChunkLayerList::iterator itr = m_Layers.begin(); itr != m_Layers.end(); ++itr)
	{
		(*itr)->SpawnMobs(a_MobSpawner);
	}  // for itr - m_Layers
}





void cChunkMap::Tick(float a_Dt)
{
	cCSLock Lock(m_CSLayers);
	for (cChunkLayerList::iterator itr = m_Layers.begin(); itr != m_Layers.end(); ++itr)
	{
		(*itr)->Tick(a_Dt);
	}  // for itr - m_Layers
}





void cChunkMap::TickBlock(int a_BlockX, int a_BlockY, int a_BlockZ)
{
	cCSLock Lock(m_CSLayers);
	int ChunkX, ChunkZ;
	cChunkDef::AbsoluteToRelative(a_BlockX, a_BlockY, a_BlockZ, ChunkX, ChunkZ);
	cChunkPtr Chunk = GetChunkNoLoad(ChunkX, ZERO_CHUNK_Y, ChunkZ);
	if ((Chunk == NULL) || !Chunk->IsValid())
	{
		return;
	}
	Chunk->TickBlock(a_BlockX, a_BlockY, a_BlockZ);
}





void cChunkMap::UnloadUnusedChunks(void)
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





void cChunkMap::QueueTickBlock(int a_BlockX, int a_BlockY, int a_BlockZ)
{
	int ChunkX, ChunkZ;
	cChunkDef::AbsoluteToRelative(a_BlockX, a_BlockY, a_BlockZ, ChunkX, ChunkZ);
	// a_BlockXYZ now contains relative coords!

	cCSLock Lock(m_CSLayers);
	cChunkPtr Chunk = GetChunkNoLoad(ChunkX, ZERO_CHUNK_Y, ChunkZ);
	if (Chunk != NULL)
	{
		Chunk->QueueTickBlock(a_BlockX, a_BlockY, a_BlockZ);
	}
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
	for (size_t i = 0; i < ARRAYCOUNT(m_Chunks); ++i)
	{
		delete m_Chunks[i];
		m_Chunks[i] = NULL;  // // Must zero out, because further chunk deletions query the chunkmap for entities and that would touch deleted data
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
		cChunk * neixm = (LocalX > 0)              ? m_Chunks[Index - 1]          : m_Parent->FindChunk(a_ChunkX - 1, a_ChunkZ);
		cChunk * neixp = (LocalX < LAYER_SIZE - 1) ? m_Chunks[Index + 1]          : m_Parent->FindChunk(a_ChunkX + 1, a_ChunkZ);
		cChunk * neizm = (LocalZ > 0)              ? m_Chunks[Index - LAYER_SIZE] : m_Parent->FindChunk(a_ChunkX    , a_ChunkZ - 1);
		cChunk * neizp = (LocalZ < LAYER_SIZE - 1) ? m_Chunks[Index + LAYER_SIZE] : m_Parent->FindChunk(a_ChunkX    , a_ChunkZ + 1);
		m_Chunks[Index] = new cChunk(a_ChunkX, 0, a_ChunkZ, m_Parent, m_Parent->GetWorld(), neixm, neixp, neizm, neizp);
	}
	return m_Chunks[Index];
}





cChunk * cChunkMap::cChunkLayer::FindChunk(int a_ChunkX, int a_ChunkZ)
{
	const int LocalX = a_ChunkX - m_LayerX * LAYER_SIZE;
	const int LocalZ = a_ChunkZ - m_LayerZ * LAYER_SIZE;
	
	if (!((LocalX < LAYER_SIZE) && (LocalZ < LAYER_SIZE) && (LocalX > -1) && (LocalZ > -1)))
	{
		ASSERT(!"Asking a cChunkLayer for a chunk that doesn't belong to it!");
		return NULL;
	}
	
	int Index = LocalX + LocalZ * LAYER_SIZE;
	return m_Chunks[Index];
}




void cChunkMap::cChunkLayer::CollectMobCensus(cMobCensus& a_ToFill)
{
	for (size_t i = 0; i < ARRAYCOUNT(m_Chunks); i++)
	{
		// We do count every Mobs in the world. But we are assuming that every chunk not loaded by any client
		// doesn't affect us. Normally they should not have mobs because every "too far" mobs despawn
		// If they have (f.i. when player disconnect) we assume we don't have to make them live or despawn
		if ((m_Chunks[i] != NULL) && m_Chunks[i]->IsValid() && m_Chunks[i]->HasAnyClients())
		{
			m_Chunks[i]->CollectMobCensus(a_ToFill);
		}
	}  // for i - m_Chunks[]
}






void cChunkMap::cChunkLayer::SpawnMobs(cMobSpawner& a_MobSpawner)
{
	for (size_t i = 0; i < ARRAYCOUNT(m_Chunks); i++)
	{
		// We only spawn close to players
		if ((m_Chunks[i] != NULL) && m_Chunks[i]->IsValid() && m_Chunks[i]->HasAnyClients())
		{
			m_Chunks[i]->SpawnMobs(a_MobSpawner);
		}
	}  // for i - m_Chunks[]
}



void cChunkMap::cChunkLayer::Tick(float a_Dt)
{
	for (size_t i = 0; i < ARRAYCOUNT(m_Chunks); i++)
	{
		// Only tick chunks that are valid and have clients:
		if ((m_Chunks[i] != NULL) && m_Chunks[i]->IsValid() && m_Chunks[i]->HasAnyClients())
		{
			m_Chunks[i]->Tick(a_Dt);
		}
	}  // for i - m_Chunks[]
}





void cChunkMap::cChunkLayer::RemoveClient(cClientHandle * a_Client)
{
	for (size_t i = 0; i < ARRAYCOUNT(m_Chunks); i++)
	{
		if (m_Chunks[i] != NULL)
		{
			m_Chunks[i]->RemoveClient(a_Client);
		}
	}  // for i - m_Chunks[]
}





bool cChunkMap::cChunkLayer::ForEachEntity(cEntityCallback & a_Callback)
{
	// Calls the callback for each entity in the entire world; returns true if all entities processed, false if the callback aborted by returning true
	for (size_t i = 0; i < ARRAYCOUNT(m_Chunks); i++)
	{
		if ((m_Chunks[i] != NULL) && m_Chunks[i]->IsValid())
		{
			if (!m_Chunks[i]->ForEachEntity(a_Callback))
			{
				return false;
			}
		}
	}
	return true;
}





bool cChunkMap::cChunkLayer::DoWithEntityByID(int a_EntityID, cEntityCallback & a_Callback, bool & a_CallbackReturn)
{
	// Calls the callback if the entity with the specified ID is found, with the entity object as the callback param. Returns true if entity found.
	for (size_t i = 0; i < ARRAYCOUNT(m_Chunks); i++)
	{
		if ((m_Chunks[i] != NULL) && m_Chunks[i]->IsValid())
		{
			if (m_Chunks[i]->DoWithEntityByID(a_EntityID, a_Callback, a_CallbackReturn))
			{
				return true;
			}
		}
	}
	return false;
}





bool cChunkMap::cChunkLayer::HasEntity(int a_EntityID)
{
	for (size_t i = 0; i < ARRAYCOUNT(m_Chunks); i++)
	{
		if ((m_Chunks[i] != NULL) && m_Chunks[i]->IsValid())
		{
			if (m_Chunks[i]->HasEntity(a_EntityID))
			{
				return true;
			}
		}
	}
	return false;
}





int cChunkMap::cChunkLayer::GetNumChunksLoaded(void) const
{
	int NumChunks = 0;
	for (size_t i = 0; i < ARRAYCOUNT(m_Chunks); ++i)
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
	for (size_t i = 0; i < ARRAYCOUNT(m_Chunks); ++i)
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
	for (size_t i = 0; i < ARRAYCOUNT(m_Chunks); ++i)
	{
		if ((m_Chunks[i] != NULL) && m_Chunks[i]->IsValid() && m_Chunks[i]->IsDirty())
		{
			World->GetStorage().QueueSaveChunk(m_Chunks[i]->GetPosX(), m_Chunks[i]->GetPosY(), m_Chunks[i]->GetPosZ());
		}
	}  // for i - m_Chunks[]
}





void cChunkMap::cChunkLayer::UnloadUnusedChunks(void)
{
	for (size_t i = 0; i < ARRAYCOUNT(m_Chunks); i++)
	{
		if (
			(m_Chunks[i] != NULL) &&   // Is valid
			(m_Chunks[i]->CanUnload()) &&  // Can unload
			!cPluginManager::Get()->CallHookChunkUnloading(m_Parent->GetWorld(), m_Chunks[i]->GetPosX(), m_Chunks[i]->GetPosZ())  // Plugins agree
		)
		{
			// The cChunk destructor calls our GetChunk() while removing its entities
			// so we still need to be able to return the chunk. Therefore we first delete, then NULLify
			// Doing otherwise results in bug http://forum.mc-server.org/showthread.php?tid=355
			delete m_Chunks[i];
			m_Chunks[i] = NULL;
		}
	}  // for i - m_Chunks[]
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




