
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
#include "BoundingBox.h"
#include "SetChunkData.h"
#include "Blocks/ChunkInterface.h"
#include "Entities/Pickup.h"

#ifndef _WIN32
	#include <cstdlib>  // abs
#endif

#include "zlib/zlib.h"
#include "json/json.h"





////////////////////////////////////////////////////////////////////////////////
// cChunkMap:

cChunkMap::cChunkMap(cWorld * a_World) :
	m_World(a_World),
	m_Pool(
		new cListAllocationPool<cChunkData::sChunkSection, 1600>(
			std::auto_ptr<cAllocationPool<cChunkData::sChunkSection>::cStarvationCallbacks>(
				new cStarvationCallbacks()
			)
		)
	)
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





void cChunkMap::RemoveLayer(cChunkLayer * a_Layer)
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
	cChunkLayer * Layer = new cChunkLayer(a_LayerX, a_LayerZ, this, *m_Pool);
	if (Layer == nullptr)
	{
		LOGERROR("cChunkMap: Cannot create new layer, server out of memory?");
		return nullptr;
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
	return nullptr;
}





cChunkMap::cChunkLayer * cChunkMap::GetLayerForChunk(int a_ChunkX, int a_ChunkZ)
{
	const int LayerX = FAST_FLOOR_DIV(a_ChunkX, LAYER_SIZE);
	const int LayerZ = FAST_FLOOR_DIV(a_ChunkZ, LAYER_SIZE);
	return GetLayer(LayerX, LayerZ);
}





cChunkPtr cChunkMap::GetChunk(int a_ChunkX, int a_ChunkZ)
{
	ASSERT(m_CSLayers.IsLockedByCurrentThread());  // m_CSLayers should already be locked by the operation that called us

	cChunkLayer * Layer = GetLayerForChunk(a_ChunkX, a_ChunkZ);
	if (Layer == nullptr)
	{
		// An error must have occurred, since layers are automatically created if they don't exist
		return nullptr;
	}
	
	cChunkPtr Chunk = Layer->GetChunk(a_ChunkX, a_ChunkZ);
	if (Chunk == nullptr)
	{
		return nullptr;
	}
	if (!Chunk->IsValid() && !Chunk->IsQueued())
	{
		Chunk->SetPresence(cChunk::cpQueued);
		Chunk->SetShouldGenerateIfLoadFailed(true);
		m_World->GetStorage().QueueLoadChunk(a_ChunkX, a_ChunkZ);
	}
	return Chunk;
}





cChunkPtr cChunkMap::GetChunkNoGen(int a_ChunkX, int a_ChunkZ)
{
	ASSERT(m_CSLayers.IsLockedByCurrentThread());  // m_CSLayers should already be locked by the operation that called us

	cChunkLayer * Layer = GetLayerForChunk(a_ChunkX, a_ChunkZ);
	if (Layer == nullptr)
	{
		// An error must have occurred, since layers are automatically created if they don't exist
		return nullptr;
	}
	
	cChunkPtr Chunk = Layer->GetChunk(a_ChunkX, a_ChunkZ);
	if (Chunk == nullptr)
	{
		return nullptr;
	}
	if (!Chunk->IsValid() && !Chunk->IsQueued())
	{
		Chunk->SetPresence(cChunk::cpQueued);
		m_World->GetStorage().QueueLoadChunk(a_ChunkX, a_ChunkZ);
	}
	
	return Chunk;
}





cChunkPtr cChunkMap::GetChunkNoLoad( int a_ChunkX, int a_ChunkZ)
{
	ASSERT(m_CSLayers.IsLockedByCurrentThread());  // m_CSLayers should already be locked by the operation that called us

	cChunkLayer * Layer = GetLayerForChunk( a_ChunkX, a_ChunkZ);
	if (Layer == nullptr)
	{
		// An error must have occurred, since layers are automatically created if they don't exist
		return nullptr;
	}
	
	return Layer->GetChunk(a_ChunkX, a_ChunkZ);
}





bool cChunkMap::LockedGetBlock(int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE & a_BlockType, NIBBLETYPE & a_BlockMeta)
{
	// We already have m_CSLayers locked since this can be called only from within the tick thread
	ASSERT(m_CSLayers.IsLockedByCurrentThread());

	int ChunkX, ChunkZ;
	cChunkDef::AbsoluteToRelative(a_BlockX, a_BlockY, a_BlockZ, ChunkX, ChunkZ);
	cChunkPtr Chunk = GetChunkNoLoad(ChunkX, ChunkZ);
	if (Chunk == nullptr)
	{
		return false;
	}
	
	a_BlockType = Chunk->GetBlock(a_BlockX, a_BlockY, a_BlockZ);
	a_BlockMeta = Chunk->GetMeta(a_BlockX, a_BlockY, a_BlockZ);
	return true;
}





bool cChunkMap::LockedGetBlockType(int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE & a_BlockType)
{
	// We already have m_CSLayers locked since this can be called only from within the tick thread
	ASSERT(m_CSLayers.IsLockedByCurrentThread());

	int ChunkX, ChunkZ;
	cChunkDef::AbsoluteToRelative(a_BlockX, a_BlockY, a_BlockZ, ChunkX, ChunkZ);
	cChunkPtr Chunk = GetChunkNoLoad(ChunkX, ChunkZ);
	if (Chunk == nullptr)
	{
		return false;
	}
	
	a_BlockType = Chunk->GetBlock(a_BlockX, a_BlockY, a_BlockZ);
	return true;
}





bool cChunkMap::LockedGetBlockMeta(int a_BlockX, int a_BlockY, int a_BlockZ, NIBBLETYPE & a_BlockMeta)
{
	// We already have m_CSLayers locked since this can be called only from within the tick thread
	ASSERT(m_CSLayers.IsLockedByCurrentThread());
	
	int ChunkX, ChunkZ;
	cChunkDef::AbsoluteToRelative(a_BlockX, a_BlockY, a_BlockZ, ChunkX, ChunkZ);
	cChunkPtr Chunk = GetChunkNoLoad(ChunkX, ChunkZ);
	if (Chunk == nullptr)
	{
		return false;
	}
	
	a_BlockMeta = Chunk->GetMeta(a_BlockX, a_BlockY, a_BlockZ);
	return true;
}





bool cChunkMap::LockedSetBlock(int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE   a_BlockType, NIBBLETYPE   a_BlockMeta)
{
	// We already have m_CSLayers locked since this can be called only from within the tick thread
	int ChunkX, ChunkZ;
	cChunkDef::AbsoluteToRelative(a_BlockX, a_BlockY, a_BlockZ, ChunkX, ChunkZ);
	cChunkPtr Chunk = GetChunkNoLoad(ChunkX, ChunkZ);
	if (Chunk == nullptr)
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
	cChunkPtr Chunk = GetChunkNoLoad(ChunkX, ChunkZ);
	if (Chunk == nullptr)
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
	if (Layer == nullptr)
	{
		return nullptr;
	}
	return Layer->FindChunk(a_ChunkX, a_ChunkZ);
}





void cChunkMap::BroadcastAttachEntity(const cEntity & a_Entity, const cEntity * a_Vehicle)
{
	cCSLock Lock(m_CSLayers);
	cChunkPtr Chunk = GetChunkNoGen(a_Entity.GetChunkX(), a_Entity.GetChunkZ());
	if (Chunk == nullptr)
	{
		return;
	}
	// It's perfectly legal to broadcast packets even to invalid chunks!
	Chunk->BroadcastAttachEntity(a_Entity, a_Vehicle);
}





void cChunkMap::BroadcastBlockAction(int a_BlockX, int a_BlockY, int a_BlockZ, char a_Byte1, char a_Byte2, BLOCKTYPE a_BlockType, const cClientHandle * a_Exclude)
{
	cCSLock Lock(m_CSLayers);
	int x, z, ChunkX, ChunkZ;
	x = a_BlockX;
	z = a_BlockZ;
	cChunkDef::BlockToChunk(x, z, ChunkX, ChunkZ);
	cChunkPtr Chunk = GetChunkNoGen(ChunkX, ChunkZ);
	if (Chunk == nullptr)
	{
		return;
	}
	// It's perfectly legal to broadcast packets even to invalid chunks!
	Chunk->BroadcastBlockAction(a_BlockX, a_BlockY, a_BlockZ, a_Byte1, a_Byte2, a_BlockType, a_Exclude);
}





void cChunkMap::BroadcastBlockBreakAnimation(UInt32 a_EntityID, int a_BlockX, int a_BlockY, int a_BlockZ, char a_Stage, const cClientHandle * a_Exclude)
{
	cCSLock Lock(m_CSLayers);
	int ChunkX, ChunkZ;

	cChunkDef::BlockToChunk(a_BlockX, a_BlockZ, ChunkX, ChunkZ);
	cChunkPtr Chunk = GetChunkNoGen(ChunkX, ChunkZ);
	if (Chunk == nullptr)
	{
		return;
	}
	// It's perfectly legal to broadcast packets even to invalid chunks!
	Chunk->BroadcastBlockBreakAnimation(a_EntityID, a_BlockX, a_BlockY, a_BlockZ, a_Stage, a_Exclude);
}





void cChunkMap::BroadcastBlockEntity(int a_BlockX, int a_BlockY, int a_BlockZ, const cClientHandle * a_Exclude)
{
	cCSLock Lock(m_CSLayers);
	int ChunkX, ChunkZ;
	cChunkDef::BlockToChunk(a_BlockX, a_BlockZ, ChunkX, ChunkZ);
	cChunkPtr Chunk = GetChunkNoGen(ChunkX, ChunkZ);
	if ((Chunk == nullptr) || !Chunk->IsValid())
	{
		return;
	}
	Chunk->BroadcastBlockEntity(a_BlockX, a_BlockY, a_BlockZ, a_Exclude);
}





void cChunkMap::BroadcastChunkData(int a_ChunkX, int a_ChunkZ, cChunkDataSerializer & a_Serializer, const cClientHandle * a_Exclude)
{
	cCSLock Lock(m_CSLayers);
	cChunkPtr Chunk = GetChunkNoGen(a_ChunkX, a_ChunkZ);
	if (Chunk == nullptr)
	{
		return;
	}
	// It's perfectly legal to broadcast packets even to invalid chunks!
	Chunk->BroadcastChunkData(a_Serializer, a_Exclude);
}





void cChunkMap::BroadcastCollectEntity(const cEntity & a_Entity, const cPlayer & a_Player, const cClientHandle * a_Exclude)
{
	cCSLock Lock(m_CSLayers);
	cChunkPtr Chunk = GetChunkNoGen(a_Entity.GetChunkX(), a_Entity.GetChunkZ());
	if (Chunk == nullptr)
	{
		return;
	}
	// It's perfectly legal to broadcast packets even to invalid chunks!
	Chunk->BroadcastCollectEntity(a_Entity, a_Player, a_Exclude);
}





void cChunkMap::BroadcastDestroyEntity(const cEntity & a_Entity, const cClientHandle * a_Exclude)
{
	cCSLock Lock(m_CSLayers);
	cChunkPtr Chunk = GetChunkNoGen(a_Entity.GetChunkX(), a_Entity.GetChunkZ());
	if (Chunk == nullptr)
	{
		return;
	}
	// It's perfectly legal to broadcast packets even to invalid chunks!
	Chunk->BroadcastDestroyEntity(a_Entity, a_Exclude);
}





void cChunkMap::BroadcastEntityEffect(const cEntity & a_Entity, int a_EffectID, int a_Amplifier, short a_Duration, const cClientHandle * a_Exclude)
{
	cCSLock Lock(m_CSLayers);
	cChunkPtr Chunk = GetChunkNoGen(a_Entity.GetChunkX(), a_Entity.GetChunkZ());
	if (Chunk == nullptr)
	{
		return;
	}
	// It's perfectly legal to broadcast packets even to invalid chunks!
	Chunk->BroadcastEntityEffect(a_Entity, a_EffectID, a_Amplifier, a_Duration);
}





void cChunkMap::BroadcastEntityEquipment(const cEntity & a_Entity, short a_SlotNum, const cItem & a_Item, const cClientHandle * a_Exclude)
{
	cCSLock Lock(m_CSLayers);
	cChunkPtr Chunk = GetChunkNoGen(a_Entity.GetChunkX(), a_Entity.GetChunkZ());
	if (Chunk == nullptr)
	{
		return;
	}
	// It's perfectly legal to broadcast packets even to invalid chunks!
	Chunk->BroadcastEntityEquipment(a_Entity, a_SlotNum, a_Item, a_Exclude);
}





void cChunkMap::BroadcastEntityHeadLook(const cEntity & a_Entity, const cClientHandle * a_Exclude)
{
	cCSLock Lock(m_CSLayers);
	cChunkPtr Chunk = GetChunkNoGen(a_Entity.GetChunkX(), a_Entity.GetChunkZ());
	if (Chunk == nullptr)
	{
		return;
	}
	// It's perfectly legal to broadcast packets even to invalid chunks!
	Chunk->BroadcastEntityHeadLook(a_Entity, a_Exclude);
}





void cChunkMap::BroadcastEntityLook(const cEntity & a_Entity, const cClientHandle * a_Exclude)
{
	cCSLock Lock(m_CSLayers);
	cChunkPtr Chunk = GetChunkNoGen(a_Entity.GetChunkX(), a_Entity.GetChunkZ());
	if (Chunk == nullptr)
	{
		return;
	}
	// It's perfectly legal to broadcast packets even to invalid chunks!
	Chunk->BroadcastEntityLook(a_Entity, a_Exclude);
}





void cChunkMap::BroadcastEntityMetadata(const cEntity & a_Entity, const cClientHandle * a_Exclude)
{
	cCSLock Lock(m_CSLayers);
	cChunkPtr Chunk = GetChunkNoGen(a_Entity.GetChunkX(), a_Entity.GetChunkZ());
	if (Chunk == nullptr)
	{
		return;
	}
	// It's perfectly legal to broadcast packets even to invalid chunks!
	Chunk->BroadcastEntityMetadata(a_Entity, a_Exclude);
}





void cChunkMap::BroadcastEntityRelMove(const cEntity & a_Entity, char a_RelX, char a_RelY, char a_RelZ, const cClientHandle * a_Exclude)
{
	cCSLock Lock(m_CSLayers);
	cChunkPtr Chunk = GetChunkNoGen(a_Entity.GetChunkX(), a_Entity.GetChunkZ());
	if (Chunk == nullptr)
	{
		return;
	}
	// It's perfectly legal to broadcast packets even to invalid chunks!
	Chunk->BroadcastEntityRelMove(a_Entity, a_RelX, a_RelY, a_RelZ, a_Exclude);
}





void cChunkMap::BroadcastEntityRelMoveLook(const cEntity & a_Entity, char a_RelX, char a_RelY, char a_RelZ, const cClientHandle * a_Exclude)
{
	cCSLock Lock(m_CSLayers);
	cChunkPtr Chunk = GetChunkNoGen(a_Entity.GetChunkX(), a_Entity.GetChunkZ());
	if (Chunk == nullptr)
	{
		return;
	}
	// It's perfectly legal to broadcast packets even to invalid chunks!
	Chunk->BroadcastEntityRelMoveLook(a_Entity, a_RelX, a_RelY, a_RelZ, a_Exclude);
}





void cChunkMap::BroadcastEntityStatus(const cEntity & a_Entity, char a_Status, const cClientHandle * a_Exclude)
{
	cCSLock Lock(m_CSLayers);
	cChunkPtr Chunk = GetChunkNoGen(a_Entity.GetChunkX(), a_Entity.GetChunkZ());
	if (Chunk == nullptr)
	{
		return;
	}
	// It's perfectly legal to broadcast packets even to invalid chunks!
	Chunk->BroadcastEntityStatus(a_Entity, a_Status, a_Exclude);
}





void cChunkMap::BroadcastEntityVelocity(const cEntity & a_Entity, const cClientHandle * a_Exclude)
{
	cCSLock Lock(m_CSLayers);
	cChunkPtr Chunk = GetChunkNoGen(a_Entity.GetChunkX(), a_Entity.GetChunkZ());
	if (Chunk == nullptr)
	{
		return;
	}
	// It's perfectly legal to broadcast packets even to invalid chunks!
	Chunk->BroadcastEntityVelocity(a_Entity, a_Exclude);
}





void cChunkMap::BroadcastEntityAnimation(const cEntity & a_Entity, char a_Animation, const cClientHandle * a_Exclude)
{
	cCSLock Lock(m_CSLayers);
	cChunkPtr Chunk = GetChunkNoGen(a_Entity.GetChunkX(), a_Entity.GetChunkZ());
	if (Chunk == nullptr)
	{
		return;
	}
	// It's perfectly legal to broadcast packets even to invalid chunks!
	Chunk->BroadcastEntityAnimation(a_Entity, a_Animation, a_Exclude);
}





void cChunkMap::BroadcastParticleEffect(const AString & a_ParticleName, float a_SrcX, float a_SrcY, float a_SrcZ, float a_OffsetX, float a_OffsetY, float a_OffsetZ, float a_ParticleData, int a_ParticleAmount, cClientHandle * a_Exclude)
{
	cCSLock Lock(m_CSLayers);
	int ChunkX, ChunkZ;

	cChunkDef::BlockToChunk((int) a_SrcX, (int) a_SrcZ, ChunkX, ChunkZ);
	cChunkPtr Chunk = GetChunkNoGen(ChunkX, ChunkZ);
	if (Chunk == nullptr)
	{
		return;
	}
	// It's perfectly legal to broadcast packets even to invalid chunks!
	Chunk->BroadcastParticleEffect(a_ParticleName, a_SrcX, a_SrcY, a_SrcZ, a_OffsetX, a_OffsetY, a_OffsetZ, a_ParticleData, a_ParticleAmount, a_Exclude);
}





void cChunkMap::BroadcastRemoveEntityEffect(const cEntity & a_Entity, int a_EffectID, const cClientHandle * a_Exclude)
{
	cCSLock Lock(m_CSLayers);
	
	cChunkPtr Chunk = GetChunkNoGen(a_Entity.GetChunkX(), a_Entity.GetChunkZ());
	if (Chunk == nullptr)
	{
		return;
	}
	// It's perfectly legal to broadcast packets even to invalid chunks!
	Chunk->BroadcastRemoveEntityEffect(a_Entity, a_EffectID, a_Exclude);
}





void cChunkMap::BroadcastSoundEffect(const AString & a_SoundName, double a_X, double a_Y, double a_Z, float a_Volume, float a_Pitch, const cClientHandle * a_Exclude)
{
	cCSLock Lock(m_CSLayers);
	int ChunkX, ChunkZ;

	cChunkDef::BlockToChunk((int)std::floor(a_X), (int)std::floor(a_Z), ChunkX, ChunkZ);
	cChunkPtr Chunk = GetChunkNoGen(ChunkX, ChunkZ);
	if (Chunk == nullptr)
	{
		return;
	}
	// It's perfectly legal to broadcast packets even to invalid chunks!
	Chunk->BroadcastSoundEffect(a_SoundName, a_X, a_Y, a_Z, a_Volume, a_Pitch, a_Exclude);
}





void cChunkMap::BroadcastSoundParticleEffect(int a_EffectID, int a_SrcX, int a_SrcY, int a_SrcZ, int a_Data, const cClientHandle * a_Exclude)
{
	cCSLock Lock(m_CSLayers);
	int ChunkX, ChunkZ;

	cChunkDef::BlockToChunk(a_SrcX, a_SrcZ, ChunkX, ChunkZ);
	cChunkPtr Chunk = GetChunkNoGen(ChunkX, ChunkZ);
	if (Chunk == nullptr)
	{
		return;
	}
	// It's perfectly legal to broadcast packets even to invalid chunks!
	Chunk->BroadcastSoundParticleEffect(a_EffectID, a_SrcX, a_SrcY, a_SrcZ, a_Data, a_Exclude);
}





void cChunkMap::BroadcastSpawnEntity(cEntity & a_Entity, const cClientHandle * a_Exclude)
{
	cCSLock Lock(m_CSLayers);
	cChunkPtr Chunk = GetChunkNoGen(a_Entity.GetChunkX(), a_Entity.GetChunkZ());
	if (Chunk == nullptr)
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
	cChunkPtr Chunk = GetChunkNoGen(ChunkX, ChunkZ);
	if (Chunk == nullptr)
	{
		return;
	}
	// It's perfectly legal to broadcast packets even to invalid chunks!
	Chunk->BroadcastThunderbolt(a_BlockX, a_BlockY, a_BlockZ, a_Exclude);
}





void cChunkMap::BroadcastUseBed(const cEntity & a_Entity, int a_BlockX, int a_BlockY, int a_BlockZ)
{
	cCSLock Lock(m_CSLayers);
	int ChunkX, ChunkZ;

	cChunkDef::BlockToChunk(a_BlockX, a_BlockZ, ChunkX, ChunkZ);
	cChunkPtr Chunk = GetChunkNoGen(ChunkX, ChunkZ);
	if (Chunk == nullptr)
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
	cChunkPtr Chunk = GetChunkNoGen(ChunkX, ChunkZ);
	if ((Chunk == nullptr) || !Chunk->IsValid())
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
	cChunkPtr Chunk = GetChunkNoGen(ChunkX, ChunkZ);
	if ((Chunk == nullptr) || !Chunk->IsValid())
	{
		return;
	}
	Chunk->UseBlockEntity(a_Player, a_BlockX, a_BlockY, a_BlockZ);
}





bool cChunkMap::DoWithChunk(int a_ChunkX, int a_ChunkZ, cChunkCallback & a_Callback)
{
	cCSLock Lock(m_CSLayers);
	cChunkPtr Chunk = GetChunkNoLoad(a_ChunkX, a_ChunkZ);
	if (Chunk == nullptr)
	{
		return false;
	}
	return a_Callback.Item(Chunk);
}


bool cChunkMap::DoWithChunkAt(Vector3i a_BlockPos, std::function<bool(cChunk &)> a_Callback)
{
	int ChunkX, ChunkZ;
	cChunkDef::BlockToChunk(a_BlockPos.x, a_BlockPos.z, ChunkX, ChunkZ);
	struct cCallBackWrapper : cChunkCallback
	{
		cCallBackWrapper(std::function<bool(cChunk &)> a_InnerCallback) :
			m_Callback(a_InnerCallback)
		{
		}
		
		virtual bool Item(cChunk * a_Chunk)
		{
			return m_Callback(*a_Chunk);
		}

	private:
		std::function<bool(cChunk &)> m_Callback;
	} callback(a_Callback);
	return DoWithChunk(ChunkX, ChunkZ, callback);
}




void cChunkMap::WakeUpSimulators(int a_BlockX, int a_BlockY, int a_BlockZ)
{
	cCSLock Lock(m_CSLayers);
	int ChunkX, ChunkZ;
	cChunkDef::BlockToChunk(a_BlockX, a_BlockZ, ChunkX, ChunkZ);
	cChunkPtr Chunk = GetChunkNoGen(ChunkX, ChunkZ);
	if ((Chunk == nullptr) || !Chunk->IsValid())
	{
		return;
	}
	m_World->GetSimulatorManager()->WakeUp(a_BlockX, a_BlockY, a_BlockZ, Chunk);
}





/// Wakes up the simulators for the specified area of blocks
void cChunkMap::WakeUpSimulatorsInArea(int a_MinBlockX, int a_MaxBlockX, int a_MinBlockY, int a_MaxBlockY, int a_MinBlockZ, int a_MaxBlockZ)
{
	// Limit the Y coords:
	a_MinBlockY = std::max(a_MinBlockY, 0);
	a_MaxBlockY = std::min(a_MaxBlockY, cChunkDef::Height - 1);
	
	cSimulatorManager * SimMgr = m_World->GetSimulatorManager();
	int MinChunkX, MinChunkZ, MaxChunkX, MaxChunkZ;
	cChunkDef::BlockToChunk(a_MinBlockX, a_MinBlockZ, MinChunkX, MinChunkZ);
	cChunkDef::BlockToChunk(a_MaxBlockX, a_MaxBlockZ, MaxChunkX, MaxChunkZ);
	cCSLock Lock(m_CSLayers);
	for (int z = MinChunkZ; z <= MaxChunkZ; z++)
	{
		int MinZ = std::max(a_MinBlockZ, z * cChunkDef::Width);
		int MaxZ = std::min(a_MaxBlockZ, z * cChunkDef::Width + cChunkDef::Width - 1);
		for (int x = MinChunkX; x <= MaxChunkX; x++)
		{
			cChunkPtr Chunk = GetChunkNoGen(x, z);
			if ((Chunk == nullptr) || !Chunk->IsValid())
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





void cChunkMap::MarkRedstoneDirty(int a_ChunkX, int a_ChunkZ)
{
	cCSLock Lock(m_CSLayers);
	cChunkPtr Chunk = GetChunkNoGen(a_ChunkX, a_ChunkZ);
	if ((Chunk == nullptr) || !Chunk->IsValid())
	{
		return;
	}
	Chunk->SetIsRedstoneDirty(true);
}





void cChunkMap::MarkChunkDirty(int a_ChunkX, int a_ChunkZ, bool a_MarkRedstoneDirty)
{
	cCSLock Lock(m_CSLayers);
	cChunkPtr Chunk = GetChunkNoGen(a_ChunkX, a_ChunkZ);
	if ((Chunk == nullptr) || !Chunk->IsValid())
	{
		return;
	}
	Chunk->MarkDirty();
	if (a_MarkRedstoneDirty)
	{
		Chunk->SetIsRedstoneDirty(true);
	}
}





void cChunkMap::MarkChunkSaving(int a_ChunkX, int a_ChunkZ)
{
	cCSLock Lock(m_CSLayers);
	cChunkPtr Chunk = GetChunkNoGen(a_ChunkX, a_ChunkZ);
	if ((Chunk == nullptr) || !Chunk->IsValid())
	{
		return;
	}
	Chunk->MarkSaving();
}





void cChunkMap::MarkChunkSaved (int a_ChunkX, int a_ChunkZ)
{
	cCSLock Lock(m_CSLayers);
	cChunkPtr Chunk = GetChunkNoGen(a_ChunkX, a_ChunkZ);
	if ((Chunk == nullptr) || !Chunk->IsValid())
	{
		return;
	}
	Chunk->MarkSaved();
}





void cChunkMap::SetChunkData(cSetChunkData & a_SetChunkData)
{
	int ChunkX = a_SetChunkData.GetChunkX();
	int ChunkZ = a_SetChunkData.GetChunkZ();
	{
		cCSLock Lock(m_CSLayers);
		cChunkPtr Chunk = GetChunkNoLoad(ChunkX, ChunkZ);
		if (Chunk == nullptr)
		{
			return;
		}
		Chunk->SetAllData(a_SetChunkData);
		
		if (a_SetChunkData.ShouldMarkDirty())
		{
			Chunk->MarkDirty();
		}
		
		// Notify relevant ChunkStays:
		cChunkStays ToBeDisabled;
		for (cChunkStays::iterator itr = m_ChunkStays.begin(), end = m_ChunkStays.end(); itr != end; ++itr)
		{
			if ((*itr)->ChunkAvailable(ChunkX, ChunkZ))
			{
				// The chunkstay wants to be disabled, add it to a list of to-be-disabled chunkstays for later processing:
				ToBeDisabled.push_back(*itr);
			}
		}  // for itr - m_ChunkStays[]
		
		// Disable (and possibly remove) the chunkstays that chose to get disabled:
		for (cChunkStays::iterator itr = ToBeDisabled.begin(), end = ToBeDisabled.end(); itr != end; ++itr)
		{
			(*itr)->Disable();
		}
	}

	// Notify plugins of the chunk becoming available
	cPluginManager::Get()->CallHookChunkAvailable(*m_World, ChunkX, ChunkZ);
}





void cChunkMap::ChunkLighted(
	int a_ChunkX, int a_ChunkZ,
	const cChunkDef::BlockNibbles & a_BlockLight,
	const cChunkDef::BlockNibbles & a_SkyLight
)
{
	cCSLock Lock(m_CSLayers);
	cChunkPtr Chunk = GetChunkNoLoad(a_ChunkX, a_ChunkZ);
	if (Chunk == nullptr)
	{
		return;
	}
	Chunk->SetLight(a_BlockLight, a_SkyLight);
	Chunk->MarkDirty();
}





bool cChunkMap::GetChunkData(int a_ChunkX, int a_ChunkZ, cChunkDataCallback & a_Callback)
{
	cCSLock Lock(m_CSLayers);
	cChunkPtr Chunk = GetChunkNoGen(a_ChunkX, a_ChunkZ);
	if ((Chunk == nullptr) || !Chunk->IsValid())
	{
		return false;
	}
	Chunk->GetAllData(a_Callback);
	return true;
}





bool cChunkMap::GetChunkBlockTypes(int a_ChunkX, int a_ChunkZ, BLOCKTYPE * a_BlockTypes)
{
	cCSLock Lock(m_CSLayers);
	cChunkPtr Chunk = GetChunkNoGen(a_ChunkX, a_ChunkZ);
	if ((Chunk == nullptr) || !Chunk->IsValid())
	{
		return false;
	}
	Chunk->GetBlockTypes(a_BlockTypes);
	return true;
}





bool cChunkMap::IsChunkQueued(int a_ChunkX, int a_ChunkZ)
{
	cCSLock Lock(m_CSLayers);
	cChunkPtr Chunk = GetChunkNoLoad(a_ChunkX, a_ChunkZ);
	return (Chunk != nullptr) && Chunk->IsQueued();
}





bool cChunkMap::IsChunkValid(int a_ChunkX, int a_ChunkZ)
{
	cCSLock Lock(m_CSLayers);
	cChunkPtr Chunk = GetChunkNoLoad(a_ChunkX, a_ChunkZ);
	return (Chunk != nullptr) && Chunk->IsValid();
}





bool cChunkMap::HasChunkAnyClients(int a_ChunkX, int a_ChunkZ)
{
	cCSLock Lock(m_CSLayers);
	cChunkPtr Chunk = GetChunkNoGen(a_ChunkX, a_ChunkZ);
	return (Chunk != nullptr) && Chunk->HasAnyClients();
}





int  cChunkMap::GetHeight(int a_BlockX, int a_BlockZ)
{
	for (;;)
	{
		cCSLock Lock(m_CSLayers);
		int ChunkX, ChunkZ, BlockY = 0;
		cChunkDef::AbsoluteToRelative(a_BlockX, BlockY, a_BlockZ, ChunkX, ChunkZ);
		cChunkPtr Chunk = GetChunk(ChunkX, ChunkZ);
		if (Chunk == nullptr)
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
	cChunkPtr Chunk = GetChunkNoLoad(ChunkX, ChunkZ);
	if ((Chunk == nullptr) || !Chunk->IsValid())
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
		int ChunkX = a_BlockList.front().m_ChunkX;
		int ChunkZ = a_BlockList.front().m_ChunkZ;
		cCSLock Lock(m_CSLayers);
		cChunkPtr Chunk = GetChunkNoGen(ChunkX, ChunkZ);
		if ((Chunk != nullptr) && Chunk->IsValid())
		{
			for (sSetBlockList::iterator itr = a_BlockList.begin(); itr != a_BlockList.end();)
			{
				if ((itr->m_ChunkX == ChunkX) && (itr->m_ChunkZ == ChunkZ))
				{
					Chunk->FastSetBlock(itr->m_RelX, itr->m_RelY, itr->m_RelZ, itr->m_BlockType, itr->m_BlockMeta);
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
				if ((itr->m_ChunkX == ChunkX) && (itr->m_ChunkZ == ChunkZ))
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





void cChunkMap::SetBlocks(const sSetBlockVector & a_Blocks)
{
	cCSLock lock(m_CSLayers);
	cChunkPtr chunk = nullptr;
	int lastChunkX = 0x7fffffff;  // Bogus coords so that chunk is updated on first pass
	int lastChunkZ = 0x7fffffff;
	for (auto block: a_Blocks)
	{
		// Update the chunk, if different from last time:
		if ((block.m_ChunkX != lastChunkX) || (block.m_ChunkZ != lastChunkZ))
		{
			lastChunkX = block.m_ChunkX;
			lastChunkZ = block.m_ChunkZ;
			chunk = GetChunk(lastChunkX, lastChunkZ);
		}

		// If the chunk is valid, set the block:
		if (chunk != nullptr)
		{
			chunk->SetBlock(block.m_RelX, block.m_RelY, block.m_RelZ, block.m_BlockType, block.m_BlockMeta);
		}
	}  // for block - a_Blocks[]
}





void cChunkMap::CollectPickupsByPlayer(cPlayer & a_Player)
{
	int BlockX = (int)(a_Player.GetPosX());  // Truncating doesn't matter much; we're scanning entire chunks anyway
	int BlockY = (int)(a_Player.GetPosY());
	int BlockZ = (int)(a_Player.GetPosZ());
	int ChunkX = 0, ChunkZ = 0;
	cChunkDef::AbsoluteToRelative(BlockX, BlockY, BlockZ, ChunkX, ChunkZ);
	int OtherChunkX = ChunkX + ((BlockX > 8) ? 1 : -1);
	int OtherChunkZ = ChunkZ + ((BlockZ > 8) ? 1 : -1);
	
	// We suppose that each player keeps their chunks in memory, therefore it makes little sense to try to re-load or even generate them.
	// The only time the chunks are not valid is when the player is downloading the initial world and they should not call this at that moment
	
	cCSLock Lock(m_CSLayers);
	GetChunkNoLoad(ChunkX, ChunkZ)->CollectPickupsByPlayer(a_Player);

	// Check the neighboring chunks as well:
	GetChunkNoLoad(OtherChunkX, ChunkZ)->CollectPickupsByPlayer     (a_Player);
	GetChunkNoLoad(OtherChunkX, OtherChunkZ)->CollectPickupsByPlayer(a_Player);
	GetChunkNoLoad(ChunkX,      ChunkZ)->CollectPickupsByPlayer     (a_Player);
	GetChunkNoLoad(ChunkX,      OtherChunkZ)->CollectPickupsByPlayer(a_Player);
}





BLOCKTYPE cChunkMap::GetBlock(int a_BlockX, int a_BlockY, int a_BlockZ)
{
	int X = a_BlockX, Y = a_BlockY, Z = a_BlockZ;
	int ChunkX, ChunkZ;
	cChunkDef::AbsoluteToRelative(X, Y, Z, ChunkX, ChunkZ);

	// First check if it isn't queued in the m_FastSetBlockQueue:
	{
		cCSLock Lock(m_CSFastSetBlock);
		for (sSetBlockList::iterator itr = m_FastSetBlockQueue.begin(); itr != m_FastSetBlockQueue.end(); ++itr)
		{
			if ((itr->m_RelX == X) && (itr->m_RelY == Y) && (itr->m_RelZ == Z) && (itr->m_ChunkX == ChunkX) && (itr->m_ChunkZ == ChunkZ))
			{
				return itr->m_BlockType;
			}
		}  // for itr - m_FastSetBlockQueue[]
	}

	// Not in the queue, query the chunk, if loaded:
	cCSLock Lock(m_CSLayers);
	cChunkPtr Chunk = GetChunk(ChunkX, ChunkZ);
	if ((Chunk != nullptr) && Chunk->IsValid())
	{
		return Chunk->GetBlock(X, Y, Z);
	}
	return 0;
}





NIBBLETYPE cChunkMap::GetBlockMeta(int a_BlockX, int a_BlockY, int a_BlockZ)
{
	int X = a_BlockX, Y = a_BlockY, Z = a_BlockZ;
	int ChunkX, ChunkZ;
	cChunkDef::AbsoluteToRelative(X, Y, Z, ChunkX, ChunkZ);

	// First check if it isn't queued in the m_FastSetBlockQueue:
	{
		cCSLock Lock(m_CSFastSetBlock);
		for (sSetBlockList::iterator itr = m_FastSetBlockQueue.begin(); itr != m_FastSetBlockQueue.end(); ++itr)
		{
			if ((itr->m_RelX == X) && (itr->m_RelY == Y) && (itr->m_RelZ == Z) && (itr->m_ChunkX == ChunkX) && (itr->m_ChunkZ == ChunkZ))
			{
				return itr->m_BlockMeta;
			}
		}  // for itr - m_FastSetBlockQueue[]
	}

	// Not in the queue, query the chunk, if loaded:
	cCSLock Lock(m_CSLayers);
	cChunkPtr Chunk = GetChunk(ChunkX, ChunkZ);
	if ((Chunk != nullptr) && Chunk->IsValid())
	{
		return Chunk->GetMeta(X, Y, Z);
	}
	return 0;
}





NIBBLETYPE cChunkMap::GetBlockSkyLight(int a_BlockX, int a_BlockY, int a_BlockZ)
{
	int ChunkX, ChunkZ;
	cChunkDef::AbsoluteToRelative(a_BlockX, a_BlockY, a_BlockZ, ChunkX, ChunkZ);

	cCSLock Lock(m_CSLayers);
	cChunkPtr Chunk = GetChunk( ChunkX, ChunkZ);
	if ((Chunk != nullptr) && Chunk->IsValid())
	{
		return Chunk->GetSkyLight(a_BlockX, a_BlockY, a_BlockZ);
	}
	return 0;
}





NIBBLETYPE cChunkMap::GetBlockBlockLight(int a_BlockX, int a_BlockY, int a_BlockZ)
{
	int ChunkX, ChunkZ;
	cChunkDef::AbsoluteToRelative(a_BlockX, a_BlockY, a_BlockZ, ChunkX, ChunkZ);

	cCSLock Lock(m_CSLayers);
	cChunkPtr Chunk = GetChunk( ChunkX, ChunkZ);
	if ((Chunk != nullptr) && Chunk->IsValid())
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
	cChunkPtr Chunk = GetChunk(ChunkX, ChunkZ);
	if ((Chunk != nullptr) && Chunk->IsValid())
	{
		Chunk->SetMeta(a_BlockX, a_BlockY, a_BlockZ, a_BlockMeta);
	}
}





void cChunkMap::SetBlock(int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, bool a_SendToClients)
{
	cChunkInterface ChunkInterface(this);
	BlockHandler(GetBlock(a_BlockX, a_BlockY, a_BlockZ))->OnDestroyed(ChunkInterface, *m_World, a_BlockX, a_BlockY, a_BlockZ);

	int ChunkX, ChunkZ, X = a_BlockX, Y = a_BlockY, Z = a_BlockZ;
	cChunkDef::AbsoluteToRelative( X, Y, Z, ChunkX, ChunkZ);

	cCSLock Lock(m_CSLayers);
	cChunkPtr Chunk = GetChunk( ChunkX, ChunkZ);
	if ((Chunk != nullptr) && Chunk->IsValid())
	{
		Chunk->SetBlock(X, Y, Z, a_BlockType, a_BlockMeta, a_SendToClients);
		m_World->GetSimulatorManager()->WakeUp(a_BlockX, a_BlockY, a_BlockZ, Chunk);
	}
	BlockHandler(a_BlockType)->OnPlaced(ChunkInterface, *m_World, a_BlockX, a_BlockY, a_BlockZ, a_BlockType, a_BlockMeta);
}





void cChunkMap::QueueSetBlock(int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, Int64 a_Tick, BLOCKTYPE a_PreviousBlockType)
{
	int ChunkX, ChunkZ, X = a_BlockX, Y = a_BlockY, Z = a_BlockZ;
	cChunkDef::AbsoluteToRelative(X, Y, Z, ChunkX, ChunkZ);

	cCSLock Lock(m_CSLayers);
	cChunkPtr Chunk = GetChunk(ChunkX, ChunkZ);
	if ((Chunk != nullptr) && Chunk->IsValid())
	{
		Chunk->QueueSetBlock(X, Y, Z, a_BlockType, a_BlockMeta, a_Tick, a_PreviousBlockType);
	}
}





bool cChunkMap::GetBlockTypeMeta(int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE & a_BlockType, NIBBLETYPE & a_BlockMeta)
{
	int ChunkX, ChunkZ, X = a_BlockX, Y = a_BlockY, Z = a_BlockZ;
	cChunkDef::AbsoluteToRelative( X, Y, Z, ChunkX, ChunkZ);

	cCSLock Lock(m_CSLayers);
	cChunkPtr Chunk = GetChunk( ChunkX, ChunkZ);
	if ((Chunk != nullptr) && Chunk->IsValid())
	{
		Chunk->GetBlockTypeMeta(X, Y, Z, a_BlockType, a_BlockMeta);
		return true;
	}
	return false;
}





bool cChunkMap::GetBlockInfo(int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE & a_BlockType, NIBBLETYPE & a_Meta, NIBBLETYPE & a_SkyLight, NIBBLETYPE & a_BlockLight)
{
	int ChunkX, ChunkZ, X = a_BlockX, Y = a_BlockY, Z = a_BlockZ;
	cChunkDef::AbsoluteToRelative( X, Y, Z, ChunkX, ChunkZ);

	cCSLock Lock(m_CSLayers);
	cChunkPtr Chunk = GetChunk( ChunkX, ChunkZ);
	if ((Chunk != nullptr) && Chunk->IsValid())
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
		cChunkPtr Chunk = GetChunk(itr->m_ChunkX, itr->m_ChunkZ);
		if ((Chunk == nullptr) || !Chunk->IsValid())
		{
			continue;
		}
		if (Chunk->GetBlock(itr->m_RelX, itr->m_RelY, itr->m_RelZ) == a_FilterBlockType)
		{
			Chunk->SetBlock(itr->m_RelX, itr->m_RelY, itr->m_RelZ, itr->m_BlockType, itr->m_BlockMeta);
		}
	}
}





void cChunkMap::ReplaceTreeBlocks(const sSetBlockVector & a_Blocks)
{
	cCSLock Lock(m_CSLayers);
	for (sSetBlockVector::const_iterator itr = a_Blocks.begin(); itr != a_Blocks.end(); ++itr)
	{
		cChunkPtr Chunk = GetChunk(itr->m_ChunkX, itr->m_ChunkZ);
		if ((Chunk == nullptr) || !Chunk->IsValid())
		{
			continue;
		}
		switch (Chunk->GetBlock(itr->m_RelX, itr->m_RelY, itr->m_RelZ))
		{
			CASE_TREE_OVERWRITTEN_BLOCKS:
			{
				Chunk->SetBlock(itr->m_RelX, itr->m_RelY, itr->m_RelZ, itr->m_BlockType, itr->m_BlockMeta);
				break;
			}
			case E_BLOCK_LEAVES:
			case E_BLOCK_NEW_LEAVES:
			{
				if ((itr->m_BlockType == E_BLOCK_LOG) || (itr->m_BlockType == E_BLOCK_NEW_LOG))
				{
					Chunk->SetBlock(itr->m_RelX, itr->m_RelY, itr->m_RelZ, itr->m_BlockType, itr->m_BlockMeta);
				}
				break;
			}
		}
	}  // for itr - a_Blocks[]
}





EMCSBiome cChunkMap::GetBiomeAt (int a_BlockX, int a_BlockZ)
{
	int ChunkX, ChunkZ, X = a_BlockX, Y = 0, Z = a_BlockZ;
	cChunkDef::AbsoluteToRelative(X, Y, Z, ChunkX, ChunkZ);

	cCSLock Lock(m_CSLayers);
	cChunkPtr Chunk = GetChunk(ChunkX, ChunkZ);
	if ((Chunk != nullptr) && Chunk->IsValid())
	{
		return Chunk->GetBiomeAt(X, Z);
	}
	else
	{
		return m_World->GetGenerator().GetBiomeAt(a_BlockX, a_BlockZ);
	}
}





bool cChunkMap::SetBiomeAt(int a_BlockX, int a_BlockZ, EMCSBiome a_Biome)
{
	int ChunkX, ChunkZ, X = a_BlockX, Y = 0, Z = a_BlockZ;
	cChunkDef::AbsoluteToRelative(X, Y, Z, ChunkX, ChunkZ);

	cCSLock Lock(m_CSLayers);
	cChunkPtr Chunk = GetChunk(ChunkX, ChunkZ);
	if ((Chunk != nullptr) && Chunk->IsValid())
	{
		Chunk->SetBiomeAt(X, Z, a_Biome);
		return true;
	}
	return false;
}





bool cChunkMap::SetAreaBiome(int a_MinX, int a_MaxX, int a_MinZ, int a_MaxZ, EMCSBiome a_Biome)
{
	// Translate coords to relative:
	int Y = 0;
	int MinChunkX, MinChunkZ, MinX = a_MinX, MinZ = a_MinZ;
	int MaxChunkX, MaxChunkZ, MaxX = a_MaxX, MaxZ = a_MaxZ;
	cChunkDef::AbsoluteToRelative(MinX, Y, MinZ, MinChunkX, MinChunkZ);
	cChunkDef::AbsoluteToRelative(MaxX, Y, MaxZ, MaxChunkX, MaxChunkZ);
	
	// Go through all chunks, set:
	bool res = true;
	cCSLock Lock(m_CSLayers);
	for (int x = MinChunkX; x <= MaxChunkX; x++)
	{
		int MinRelX = (x == MinChunkX) ? MinX : 0;
		int MaxRelX = (x == MaxChunkX) ? MaxX : cChunkDef::Width - 1;
		for (int z = MinChunkZ; z <= MaxChunkZ; z++)
		{
			int MinRelZ = (z == MinChunkZ) ? MinZ : 0;
			int MaxRelZ = (z == MaxChunkZ) ? MaxZ : cChunkDef::Width - 1;
			cChunkPtr Chunk = GetChunkNoLoad(x, z);
			if ((Chunk != nullptr) && Chunk->IsValid())
			{
				Chunk->SetAreaBiome(MinRelX, MaxRelX, MinRelZ, MaxRelZ, a_Biome);
			}
			else
			{
				res = false;
			}
		}  // for z
	}  // for x
	return res;
}





bool cChunkMap::GetBlocks(sSetBlockVector & a_Blocks, bool a_ContinueOnFailure)
{
	bool res = true;
	cCSLock Lock(m_CSLayers);
	for (sSetBlockVector::iterator itr = a_Blocks.begin(); itr != a_Blocks.end(); ++itr)
	{
		cChunkPtr Chunk = GetChunk(itr->m_ChunkX, itr->m_ChunkZ);
		if ((Chunk == nullptr) || !Chunk->IsValid())
		{
			if (!a_ContinueOnFailure)
			{
				return false;
			}
			res = false;
			continue;
		}
		itr->m_BlockType = Chunk->GetBlock(itr->m_RelX, itr->m_RelY, itr->m_RelZ);
		itr->m_BlockMeta = Chunk->GetMeta(itr->m_RelX, itr->m_RelY, itr->m_RelZ);
	}
	return res;
}





bool cChunkMap::DigBlock(int a_BlockX, int a_BlockY, int a_BlockZ)
{
	int PosX = a_BlockX, PosY = a_BlockY, PosZ = a_BlockZ, ChunkX, ChunkZ;

	cChunkDef::AbsoluteToRelative(PosX, PosY, PosZ, ChunkX, ChunkZ);

	{
		cCSLock Lock(m_CSLayers);
		cChunkPtr DestChunk = GetChunk( ChunkX, ChunkZ);
		if ((DestChunk == nullptr) || !DestChunk->IsValid())
		{
			return false;
		}
		
		DestChunk->SetBlock(PosX, PosY, PosZ, E_BLOCK_AIR, 0);
		m_World->GetSimulatorManager()->WakeUp(a_BlockX, a_BlockY, a_BlockZ, DestChunk);
	}

	return true;
}





void cChunkMap::SendBlockTo(int a_X, int a_Y, int a_Z, cPlayer * a_Player)
{
	int ChunkX, ChunkZ;
	cChunkDef::AbsoluteToRelative(a_X, a_Y, a_Z, ChunkX, ChunkZ);
	
	cCSLock Lock(m_CSLayers);
	cChunkPtr Chunk = GetChunk(ChunkX, ChunkZ);
	if ((Chunk != nullptr) && (Chunk->IsValid()))
	{
		Chunk->SendBlockTo(a_X, a_Y, a_Z, a_Player->GetClientHandle());
	}
}





void cChunkMap::CompareChunkClients(int a_ChunkX1, int a_ChunkZ1, int a_ChunkX2, int a_ChunkZ2, cClientDiffCallback & a_Callback)
{
	cCSLock Lock(m_CSLayers);
	cChunkPtr Chunk1 = GetChunkNoGen(a_ChunkX1, a_ChunkZ1);
	if (Chunk1 == nullptr)
	{
		return;
	}
	cChunkPtr Chunk2 = GetChunkNoGen(a_ChunkX2, a_ChunkZ2);
	if (Chunk2 == nullptr)
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
	cChunkPtr Chunk = GetChunk(a_ChunkX, a_ChunkZ);
	if (Chunk == nullptr)
	{
		return false;
	}
	return Chunk->AddClient(a_Client);
}





void cChunkMap::RemoveChunkClient(int a_ChunkX, int a_ChunkZ, cClientHandle * a_Client)
{
	cCSLock Lock(m_CSLayers);
	cChunkPtr Chunk = GetChunkNoGen(a_ChunkX, a_ChunkZ);
	if (Chunk == nullptr)
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
	cChunkPtr Chunk = GetChunkNoGen(a_Entity->GetChunkX(), a_Entity->GetChunkZ());
	if (
		(Chunk == nullptr) ||  // Chunk not present at all
		(!Chunk->IsValid() && !a_Entity->IsPlayer())  // Chunk present, but no valid data; players need to spawn in such chunks (#953)
	)
	{
		LOGWARNING("Entity at %p (%s, ID %d) spawning in a non-existent chunk, the entity is lost.",
			a_Entity, a_Entity->GetClass(), a_Entity->GetUniqueID()
		);
		return;
	}
	Chunk->AddEntity(a_Entity);
}





void cChunkMap::AddEntityIfNotPresent(cEntity * a_Entity)
{
	cCSLock Lock(m_CSLayers);
	cChunkPtr Chunk = GetChunkNoGen(a_Entity->GetChunkX(), a_Entity->GetChunkZ());
	if (
		(Chunk == nullptr) ||  // Chunk not present at all
		(!Chunk->IsValid() && !a_Entity->IsPlayer())  // Chunk present, but no valid data; players need to spawn in such chunks (#953)
	)
	{
		LOGWARNING("Entity at %p (%s, ID %d) spawning in a non-existent chunk, the entity is lost.",
			a_Entity, a_Entity->GetClass(), a_Entity->GetUniqueID()
		);
		return;
	}
	if (!Chunk->HasEntity(a_Entity->GetUniqueID()))
	{
		Chunk->AddEntity(a_Entity);
	}
}





bool cChunkMap::HasEntity(UInt32 a_UniqueID)
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
	cChunkPtr Chunk = GetChunkNoGen(a_Entity->GetChunkX(), a_Entity->GetChunkZ());

	// Even if a chunk is not valid, it may still contain entities such as players; make sure to remove them (#1190)
	if (Chunk == nullptr)
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
	cChunkPtr Chunk = GetChunkNoGen(a_ChunkX, a_ChunkZ);
	if ((Chunk == nullptr) || !Chunk->IsValid())
	{
		return false;
	}
	return Chunk->ForEachEntity(a_Callback);
}





bool cChunkMap::ForEachEntityInBox(const cBoundingBox & a_Box, cEntityCallback & a_Callback)
{
	// Calculate the chunk range for the box:
	int MinChunkX = (int)floor(a_Box.GetMinX() / cChunkDef::Width);
	int MinChunkZ = (int)floor(a_Box.GetMinZ() / cChunkDef::Width);
	int MaxChunkX = (int)floor((a_Box.GetMaxX() + cChunkDef::Width) / cChunkDef::Width);
	int MaxChunkZ = (int)floor((a_Box.GetMaxZ() + cChunkDef::Width) / cChunkDef::Width);
	
	// Iterate over each chunk in the range:
	cCSLock Lock(m_CSLayers);
	for (int z = MinChunkZ; z <= MaxChunkZ; z++)
	{
		for (int x = MinChunkX; x <= MaxChunkX; x++)
		{
			cChunkPtr Chunk = GetChunkNoGen(x, z);
			if ((Chunk == nullptr) || !Chunk->IsValid())
			{
				continue;
			}
			if (!Chunk->ForEachEntityInBox(a_Box, a_Callback))
			{
				return false;
			}
		}  // for x
	}  // for z
	return true;
}





void cChunkMap::DoExplosionAt(double a_ExplosionSize, double a_BlockX, double a_BlockY, double a_BlockZ, cVector3iArray & a_BlocksAffected)
{
	// Don't explode if outside of Y range (prevents the following test running into unallocated memory):
	if ((a_BlockY < 0) || (a_BlockY > cChunkDef::Height - 1))
	{
		return;
	}

	bool ShouldDestroyBlocks = true;

	// Don't explode if the explosion center is inside a liquid block:
	if (IsBlockLiquid(m_World->GetBlock((int)floor(a_BlockX), (int)floor(a_BlockY), (int)floor(a_BlockZ))))
	{
		ShouldDestroyBlocks = false;
	}

	int ExplosionSizeInt = (int)ceil(a_ExplosionSize);
	int ExplosionSizeSq = ExplosionSizeInt * ExplosionSizeInt;

	int bx = (int)floor(a_BlockX);
	int by = (int)floor(a_BlockY);
	int bz = (int)floor(a_BlockZ);

	int MinY = std::max((int)floor(a_BlockY - ExplosionSizeInt), 0);
	int MaxY = std::min((int)ceil(a_BlockY + ExplosionSizeInt), cChunkDef::Height - 1);

	if (ShouldDestroyBlocks)
	{
		cBlockArea area;
		a_BlocksAffected.reserve(8 * ExplosionSizeInt * ExplosionSizeInt * ExplosionSizeInt);
		if (!area.Read(m_World, bx - ExplosionSizeInt, (int)ceil(a_BlockX + ExplosionSizeInt), MinY, MaxY, bz - ExplosionSizeInt, (int)ceil(a_BlockZ + ExplosionSizeInt)))
		{
			return;
		}
		
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
							int FuseTime = 10 + m_World->GetTickRandomNumber(20);
							m_World->SpawnPrimedTNT(a_BlockX + x + 0.5, a_BlockY + y + 0.5, a_BlockZ + z + 0.5, FuseTime);
							area.SetBlockTypeMeta(bx + x, by + y, bz + z, E_BLOCK_AIR, 0);
							a_BlocksAffected.push_back(Vector3i(bx + x, by + y, bz + z));
							break;
						}
						
						case E_BLOCK_OBSIDIAN:
						case E_BLOCK_BEACON:
						case E_BLOCK_BEDROCK:
						case E_BLOCK_BARRIER:
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
							if (m_World->GetTickRandomNumber(100) <= 25)  // 25% chance of pickups
							{
								cItems Drops;
								cBlockHandler * Handler = BlockHandler(Block);

								Handler->ConvertToPickups(Drops, area.GetBlockMeta(bx + x, by + y, bz + z));  // Stone becomes cobblestone, coal ore becomes coal, etc.
								m_World->SpawnItemPickups(Drops, bx + x, by + y, bz + z);
							}
							else if ((m_World->GetTNTShrapnelLevel() > slNone) && (m_World->GetTickRandomNumber(100) < 20))  // 20% chance of flinging stuff around
							{
								// If the block is shrapnel-able, make a falling block entity out of it:
								if (
									((m_World->GetTNTShrapnelLevel() == slAll) && cBlockInfo::FullyOccupiesVoxel(Block)) ||
									((m_World->GetTNTShrapnelLevel() == slGravityAffectedOnly) && ((Block == E_BLOCK_SAND) || (Block == E_BLOCK_GRAVEL)))
								)
								{
									m_World->SpawnFallingBlock(bx + x, by + y + 5, bz + z, Block, area.GetBlockMeta(bx + x, by + y, bz + z));
								}
							}

							area.SetBlockTypeMeta(bx + x, by + y, bz + z, E_BLOCK_AIR, 0);
							a_BlocksAffected.push_back(Vector3i(bx + x, by + y, bz + z));
							break;
						}
					}  // switch (BlockType)
				}  // for z
			}  // for y
		}  // for x
		area.Write(m_World, bx - ExplosionSizeInt, MinY, bz - ExplosionSizeInt);
	}

	class cTNTDamageCallback :
		public cEntityCallback
	{
	public:
		cTNTDamageCallback(cBoundingBox & a_bbTNT, Vector3d a_ExplosionPos, int a_ExplosionSize) :
			m_bbTNT(a_bbTNT),
			m_ExplosionPos(a_ExplosionPos),
			m_ExplosionSize(a_ExplosionSize)
		{
		}

		virtual bool Item(cEntity * a_Entity) override
		{
			if (a_Entity->IsPickup() && (a_Entity->GetTicksAlive() < 20))
			{
				// If pickup age is smaller than one second, it is invincible (so we don't kill pickups that were just spawned)
				return false;
			}

			Vector3d DistanceFromExplosion = a_Entity->GetPosition() - m_ExplosionPos;
			
			if (!a_Entity->IsTNT() && !a_Entity->IsFallingBlock())  // Don't apply damage to other TNT entities and falling blocks, they should be invincible
			{
				cBoundingBox bbEntity(a_Entity->GetPosition(), a_Entity->GetWidth() / 2, a_Entity->GetHeight());

				if (!m_bbTNT.IsInside(bbEntity))  // If bbEntity is inside bbTNT, not vice versa!
				{
					return false;
				}

				// Ensure that the damage dealt is inversely proportional to the distance to the TNT centre - the closer a player is, the harder they are hit
				a_Entity->TakeDamage(dtExplosion, nullptr, (int)((1 / DistanceFromExplosion.Length()) * 6 * m_ExplosionSize), 0);
			}

			// Apply force to entities around the explosion - code modified from World.cpp DoExplosionAt()
			DistanceFromExplosion.Normalize();
			DistanceFromExplosion *= m_ExplosionSize * m_ExplosionSize;
			a_Entity->AddSpeed(DistanceFromExplosion);
			
			return false;
		}

	protected:
		cBoundingBox & m_bbTNT;
		Vector3d m_ExplosionPos;
		int m_ExplosionSize;
	};

	cBoundingBox bbTNT(Vector3d(a_BlockX, a_BlockY, a_BlockZ), 0.5, 1);
	bbTNT.Expand(ExplosionSizeInt * 2, ExplosionSizeInt * 2, ExplosionSizeInt * 2);


	cTNTDamageCallback TNTDamageCallback(bbTNT, Vector3d(a_BlockX, a_BlockY, a_BlockZ), ExplosionSizeInt);
	ForEachEntity(TNTDamageCallback);

	// Wake up all simulators for the area, so that water and lava flows and sand falls into the blasted holes (FS #391):
	WakeUpSimulatorsInArea(
		bx - ExplosionSizeInt - 1, bx + ExplosionSizeInt + 1,
		MinY, MaxY,
		bz - ExplosionSizeInt - 1, bz + ExplosionSizeInt + 1
	);
}





bool cChunkMap::DoWithEntityByID(UInt32 a_UniqueID, cEntityCallback & a_Callback)
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
	cChunkPtr Chunk = GetChunkNoGen(a_ChunkX, a_ChunkZ);
	if ((Chunk == nullptr) || !Chunk->IsValid())
	{
		return false;
	}
	return Chunk->ForEachBlockEntity(a_Callback);
}





bool cChunkMap::ForEachChestInChunk(int a_ChunkX, int a_ChunkZ, cChestCallback & a_Callback)
{
	cCSLock Lock(m_CSLayers);
	cChunkPtr Chunk = GetChunkNoGen(a_ChunkX, a_ChunkZ);
	if ((Chunk == nullptr) || !Chunk->IsValid())
	{
		return false;
	}
	return Chunk->ForEachChest(a_Callback);
}





bool cChunkMap::ForEachDispenserInChunk(int a_ChunkX, int a_ChunkZ, cDispenserCallback & a_Callback)
{
	cCSLock Lock(m_CSLayers);
	cChunkPtr Chunk = GetChunkNoGen(a_ChunkX, a_ChunkZ);
	if ((Chunk == nullptr) || !Chunk->IsValid())
	{
		return false;
	}
	return Chunk->ForEachDispenser(a_Callback);
}





bool cChunkMap::ForEachDropperInChunk(int a_ChunkX, int a_ChunkZ, cDropperCallback & a_Callback)
{
	cCSLock Lock(m_CSLayers);
	cChunkPtr Chunk = GetChunkNoGen(a_ChunkX, a_ChunkZ);
	if ((Chunk == nullptr) || !Chunk->IsValid())
	{
		return false;
	}
	return Chunk->ForEachDropper(a_Callback);
}





bool cChunkMap::ForEachDropSpenserInChunk(int a_ChunkX, int a_ChunkZ, cDropSpenserCallback & a_Callback)
{
	cCSLock Lock(m_CSLayers);
	cChunkPtr Chunk = GetChunkNoGen(a_ChunkX, a_ChunkZ);
	if ((Chunk == nullptr) || !Chunk->IsValid())
	{
		return false;
	}
	return Chunk->ForEachDropSpenser(a_Callback);
}





bool cChunkMap::ForEachFurnaceInChunk(int a_ChunkX, int a_ChunkZ, cFurnaceCallback & a_Callback)
{
	cCSLock Lock(m_CSLayers);
	cChunkPtr Chunk = GetChunkNoGen(a_ChunkX, a_ChunkZ);
	if ((Chunk == nullptr) || !Chunk->IsValid())
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
	cChunkPtr Chunk = GetChunkNoGen(ChunkX, ChunkZ);
	if ((Chunk == nullptr) || !Chunk->IsValid())
	{
		return false;
	}
	return Chunk->DoWithBlockEntityAt(a_BlockX, a_BlockY, a_BlockZ, a_Callback);
}





bool cChunkMap::DoWithBeaconAt(int a_BlockX, int a_BlockY, int a_BlockZ, cBeaconCallback & a_Callback)
{
	int ChunkX, ChunkZ;
	int BlockX = a_BlockX, BlockY = a_BlockY, BlockZ = a_BlockZ;
	cChunkDef::AbsoluteToRelative(BlockX, BlockY, BlockZ, ChunkX, ChunkZ);
	cCSLock Lock(m_CSLayers);
	cChunkPtr Chunk = GetChunkNoGen(ChunkX, ChunkZ);
	if ((Chunk == nullptr) || !Chunk->IsValid())
	{
		return false;
	}
	return Chunk->DoWithBeaconAt(a_BlockX, a_BlockY, a_BlockZ, a_Callback);
}





bool cChunkMap::DoWithChestAt(int a_BlockX, int a_BlockY, int a_BlockZ, cChestCallback & a_Callback)
{
	int ChunkX, ChunkZ;
	int BlockX = a_BlockX, BlockY = a_BlockY, BlockZ = a_BlockZ;
	cChunkDef::AbsoluteToRelative(BlockX, BlockY, BlockZ, ChunkX, ChunkZ);
	cCSLock Lock(m_CSLayers);
	cChunkPtr Chunk = GetChunkNoGen(ChunkX, ChunkZ);
	if ((Chunk == nullptr) || !Chunk->IsValid())
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
	cChunkPtr Chunk = GetChunkNoGen(ChunkX, ChunkZ);
	if ((Chunk == nullptr) || !Chunk->IsValid())
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
	cChunkPtr Chunk = GetChunkNoGen(ChunkX, ChunkZ);
	if ((Chunk == nullptr) || !Chunk->IsValid())
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
	cChunkPtr Chunk = GetChunkNoGen(ChunkX, ChunkZ);
	if ((Chunk == nullptr) || !Chunk->IsValid())
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
	cChunkPtr Chunk = GetChunkNoGen(ChunkX, ChunkZ);
	if ((Chunk == nullptr) || !Chunk->IsValid())
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
	cChunkPtr Chunk = GetChunkNoGen(ChunkX, ChunkZ);
	if ((Chunk == nullptr) || !Chunk->IsValid())
	{
		return false;
	}
	return Chunk->DoWithNoteBlockAt(a_BlockX, a_BlockY, a_BlockZ, a_Callback);
}




bool cChunkMap::DoWithCommandBlockAt(int a_BlockX, int a_BlockY, int a_BlockZ, cCommandBlockCallback & a_Callback)
{
	int ChunkX, ChunkZ;
	int BlockX = a_BlockX, BlockY = a_BlockY, BlockZ = a_BlockZ;
	cChunkDef::AbsoluteToRelative(BlockX, BlockY, BlockZ, ChunkX, ChunkZ);
	cCSLock Lock(m_CSLayers);
	cChunkPtr Chunk = GetChunkNoGen(ChunkX, ChunkZ);
	if ((Chunk == nullptr) || !Chunk->IsValid())
	{
		return false;
	}
	return Chunk->DoWithCommandBlockAt(a_BlockX, a_BlockY, a_BlockZ, a_Callback);
}





bool cChunkMap::DoWithMobHeadAt(int a_BlockX, int a_BlockY, int a_BlockZ, cMobHeadCallback & a_Callback)
{
	int ChunkX, ChunkZ;
	int BlockX = a_BlockX, BlockY = a_BlockY, BlockZ = a_BlockZ;
	cChunkDef::AbsoluteToRelative(BlockX, BlockY, BlockZ, ChunkX, ChunkZ);
	cCSLock Lock(m_CSLayers);
	cChunkPtr Chunk = GetChunkNoGen(ChunkX, ChunkZ);
	if ((Chunk == nullptr) || !Chunk->IsValid())
	{
		return false;
	}
	return Chunk->DoWithMobHeadAt(a_BlockX, a_BlockY, a_BlockZ, a_Callback);
}





bool cChunkMap::DoWithFlowerPotAt(int a_BlockX, int a_BlockY, int a_BlockZ, cFlowerPotCallback & a_Callback)
{
	int ChunkX, ChunkZ;
	int BlockX = a_BlockX, BlockY = a_BlockY, BlockZ = a_BlockZ;
	cChunkDef::AbsoluteToRelative(BlockX, BlockY, BlockZ, ChunkX, ChunkZ);
	cCSLock Lock(m_CSLayers);
	cChunkPtr Chunk = GetChunkNoGen(ChunkX, ChunkZ);
	if ((Chunk == nullptr) || !Chunk->IsValid())
	{
		return false;
	}
	return Chunk->DoWithFlowerPotAt(a_BlockX, a_BlockY, a_BlockZ, a_Callback);
}





bool cChunkMap::GetSignLines(int a_BlockX, int a_BlockY, int a_BlockZ, AString & a_Line1, AString & a_Line2, AString & a_Line3, AString & a_Line4)
{
	int ChunkX, ChunkZ;
	int BlockX = a_BlockX, BlockY = a_BlockY, BlockZ = a_BlockZ;
	cChunkDef::AbsoluteToRelative(BlockX, BlockY, BlockZ, ChunkX, ChunkZ);
	cCSLock Lock(m_CSLayers);
	cChunkPtr Chunk = GetChunkNoGen(ChunkX, ChunkZ);
	if ((Chunk == nullptr) || !Chunk->IsValid())
	{
		return false;
	}
	return Chunk->GetSignLines(a_BlockX, a_BlockY, a_BlockZ, a_Line1, a_Line2, a_Line3, a_Line4);
}





void cChunkMap::TouchChunk(int a_ChunkX, int a_ChunkZ)
{
	cCSLock Lock(m_CSLayers);
	GetChunk(a_ChunkX, a_ChunkZ);
}





void cChunkMap::PrepareChunk(int a_ChunkX, int a_ChunkZ, cChunkCoordCallback * a_Callback)
{
	cCSLock Lock(m_CSLayers);
	cChunkPtr Chunk = GetChunkNoLoad(a_ChunkX, a_ChunkZ);

	// If the chunk is not prepared, queue it in the lighting thread, that will do all the needed processing:
	if ((Chunk == nullptr) || !Chunk->IsValid() || !Chunk->IsLightValid())
	{
		m_World->GetLightingThread().QueueChunk(a_ChunkX, a_ChunkZ, a_Callback);
		return;
	}

	// The chunk is present and lit, just call the callback:
	if (a_Callback != nullptr)
	{
		a_Callback->Call(a_ChunkX, a_ChunkZ);
	}
}





bool cChunkMap::GenerateChunk(int a_ChunkX, int a_ChunkZ, cChunkCoordCallback * a_Callback)
{
	cCSLock Lock(m_CSLayers);
	cChunkPtr Chunk = GetChunkNoLoad(a_ChunkX, a_ChunkZ);
	if (Chunk == nullptr)
	{
		// Generic error while getting the chunk - out of memory?
		return false;
	}

	// Try loading the chunk:
	if ((Chunk == nullptr) || (!Chunk->IsValid()))
	{
		Chunk->SetPresence(cChunk::cpQueued);
		class cPrepareLoadCallback: public cChunkCoordCallback
		{
		public:
			cPrepareLoadCallback(cWorld & a_World, cChunkMap & a_ChunkMap, cChunkCoordCallback * a_Callback):
				m_World(a_World),
				m_ChunkMap(a_ChunkMap),
				m_Callback(a_Callback)
			{
			}

			// cChunkCoordCallback override:
			virtual void Call(int a_CBChunkX, int a_CBChunkZ) override
			{
				// The chunk has been loaded or an error occurred, check if it's valid now:
				cCSLock Lock(m_ChunkMap.m_CSLayers);
				cChunkPtr CBChunk = m_ChunkMap.GetChunkNoLoad(a_CBChunkX, a_CBChunkZ);

				if (CBChunk == nullptr)
				{
					// An error occurred, but we promised to call the callback, so call it even when there's no real chunk data:
					if (m_Callback != nullptr)
					{
						m_Callback->Call(a_CBChunkX, a_CBChunkZ);
					}
					return;
				}

				// If the chunk is not valid, queue it in the generator:
				if (!CBChunk->IsValid())
				{
					m_World.GetGenerator().QueueGenerateChunk(a_CBChunkX, a_CBChunkZ, false, m_Callback);
					return;
				}

				// The chunk was loaded, call the callback:
				if (m_Callback != nullptr)
				{
					m_Callback->Call(a_CBChunkX, a_CBChunkZ);
				}
			}

		protected:
			cWorld & m_World;
			cChunkMap & m_ChunkMap;
			cChunkCoordCallback * m_Callback;
		};
		m_World->GetStorage().QueueLoadChunk(a_ChunkX, a_ChunkZ, new cPrepareLoadCallback(*m_World, *this, a_Callback));
		return true;
	}

	// The chunk is valid, just call the callback:
	if (a_Callback != nullptr)
	{
		a_Callback->Call(a_ChunkX, a_ChunkZ);
	}
	return true;
}





void cChunkMap::ChunkLoadFailed(int a_ChunkX, int a_ChunkZ)
{
	cCSLock Lock(m_CSLayers);
	cChunkPtr Chunk = GetChunkNoLoad(a_ChunkX, a_ChunkZ);
	if (Chunk == nullptr)
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
	cChunkPtr Chunk = GetChunkNoGen(ChunkX, ChunkZ);
	if ((Chunk == nullptr) || !Chunk->IsValid())
	{
		return false;
	}
	return Chunk->SetSignLines(a_BlockX, a_BlockY, a_BlockZ, a_Line1, a_Line2, a_Line3, a_Line4);
}





void cChunkMap::MarkChunkRegenerating(int a_ChunkX, int a_ChunkZ)
{
	cCSLock Lock(m_CSLayers);
	cChunkPtr Chunk = GetChunkNoLoad(a_ChunkX, a_ChunkZ);
	if (Chunk == nullptr)
	{
		// Not present
		return;
	}
	Chunk->MarkRegenerating();
}





bool cChunkMap::IsChunkLighted(int a_ChunkX, int a_ChunkZ)
{
	cCSLock Lock(m_CSLayers);
	cChunkPtr Chunk = GetChunkNoLoad(a_ChunkX, a_ChunkZ);
	if (Chunk == nullptr)
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
			cChunkPtr Chunk = GetChunkNoLoad(x, z);
			if ((Chunk == nullptr) || (!Chunk->IsValid()))
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
			cChunkPtr Chunk = GetChunkNoLoad(x, z);
			if ((Chunk == nullptr) || (!Chunk->IsValid()))
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
	cChunkPtr Chunk = GetChunkNoLoad(ChunkX, ChunkZ);
	if (Chunk != nullptr)
	{
		Chunk->GrowMelonPumpkin(a_BlockX, a_BlockY, a_BlockZ, a_BlockType, a_Rand);
	}
}





void cChunkMap::GrowSugarcane(int a_BlockX, int a_BlockY, int a_BlockZ, int a_NumBlocksToGrow)
{
	int ChunkX, ChunkZ;
	cChunkDef::AbsoluteToRelative(a_BlockX, a_BlockY, a_BlockZ, ChunkX, ChunkZ);
	
	cCSLock Lock(m_CSLayers);
	cChunkPtr Chunk = GetChunkNoLoad(ChunkX, ChunkZ);
	if (Chunk != nullptr)
	{
		Chunk->GrowSugarcane(a_BlockX, a_BlockY, a_BlockZ, a_NumBlocksToGrow);
	}
}





void cChunkMap::GrowCactus(int a_BlockX, int a_BlockY, int a_BlockZ, int a_NumBlocksToGrow)
{
	int ChunkX, ChunkZ;
	cChunkDef::AbsoluteToRelative(a_BlockX, a_BlockY, a_BlockZ, ChunkX, ChunkZ);
	
	cCSLock Lock(m_CSLayers);
	cChunkPtr Chunk = GetChunkNoLoad(ChunkX, ChunkZ);
	if (Chunk != nullptr)
	{
		Chunk->GrowCactus(a_BlockX, a_BlockY, a_BlockZ, a_NumBlocksToGrow);
	}
}





void cChunkMap::SetNextBlockTick(int a_BlockX, int a_BlockY, int a_BlockZ)
{
	int ChunkX, ChunkZ;
	cChunkDef::AbsoluteToRelative(a_BlockX, a_BlockY, a_BlockZ, ChunkX, ChunkZ);
	
	cCSLock Lock(m_CSLayers);
	cChunkPtr Chunk = GetChunkNoLoad(ChunkX, ChunkZ);
	if (Chunk != nullptr)
	{
		Chunk->SetNextBlockTick(a_BlockX, a_BlockY, a_BlockZ);
	}
}




void cChunkMap::CollectMobCensus(cMobCensus & a_ToFill)
{
	cCSLock Lock(m_CSLayers);
	for (auto && layer: m_Layers)
	{
		layer->CollectMobCensus(a_ToFill);
	}  // for itr - m_Layers
}






void cChunkMap::SpawnMobs(cMobSpawner & a_MobSpawner)
{
	cCSLock Lock(m_CSLayers);
	for (auto && layer: m_Layers)
	{
		layer->SpawnMobs(a_MobSpawner);
	}  // for itr - m_Layers
}





void cChunkMap::Tick(std::chrono::milliseconds a_Dt)
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
	cChunkPtr Chunk = GetChunkNoLoad(ChunkX, ChunkZ);
	if ((Chunk == nullptr) || !Chunk->IsValid())
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
	cChunkPtr Chunk = GetChunkNoLoad(ChunkX, ChunkZ);
	if (Chunk != nullptr)
	{
		Chunk->QueueTickBlock(a_BlockX, a_BlockY, a_BlockZ);
	}
}





void cChunkMap::SetChunkAlwaysTicked(int a_ChunkX, int a_ChunkZ, bool a_AlwaysTicked)
{
	cCSLock Lock(m_CSLayers);
	cChunkPtr Chunk = GetChunkNoLoad(a_ChunkX, a_ChunkZ);
	if (Chunk != nullptr)
	{
		Chunk->SetAlwaysTicked(a_AlwaysTicked);
	}
}





////////////////////////////////////////////////////////////////////////////////
// cChunkMap::cChunkLayer:

cChunkMap::cChunkLayer::cChunkLayer(
	int a_LayerX, int a_LayerZ,
	cChunkMap * a_Parent,
	cAllocationPool<cChunkData::sChunkSection> & a_Pool
)
	: m_LayerX( a_LayerX)
	, m_LayerZ( a_LayerZ)
	, m_Parent( a_Parent)
	, m_NumChunksLoaded( 0)
	, m_Pool(a_Pool)
{
	memset(m_Chunks, 0, sizeof(m_Chunks));
}





cChunkMap::cChunkLayer::~cChunkLayer()
{
	for (size_t i = 0; i < ARRAYCOUNT(m_Chunks); ++i)
	{
		delete m_Chunks[i];
		m_Chunks[i] = nullptr;  // Must zero out, because further chunk deletions query the chunkmap for entities and that would touch deleted data
	}  // for i - m_Chunks[]
}





cChunkPtr cChunkMap::cChunkLayer::GetChunk( int a_ChunkX, int a_ChunkZ)
{
	// Always returns an assigned chunkptr, but the chunk needn't be valid (loaded / generated) - callers must check

	const int LocalX = a_ChunkX - m_LayerX * LAYER_SIZE;
	const int LocalZ = a_ChunkZ - m_LayerZ * LAYER_SIZE;
	
	if (!((LocalX < LAYER_SIZE) && (LocalZ < LAYER_SIZE) && (LocalX > -1) && (LocalZ > -1)))
	{
		ASSERT(!"Asking a cChunkLayer for a chunk that doesn't belong to it!");
		return nullptr;
	}
	
	int Index = LocalX + LocalZ * LAYER_SIZE;
	if (m_Chunks[Index] == nullptr)
	{
		cChunk * neixm = (LocalX > 0)              ? m_Chunks[Index - 1]          : m_Parent->FindChunk(a_ChunkX - 1, a_ChunkZ);
		cChunk * neixp = (LocalX < LAYER_SIZE - 1) ? m_Chunks[Index + 1]          : m_Parent->FindChunk(a_ChunkX + 1, a_ChunkZ);
		cChunk * neizm = (LocalZ > 0)              ? m_Chunks[Index - LAYER_SIZE] : m_Parent->FindChunk(a_ChunkX,     a_ChunkZ - 1);
		cChunk * neizp = (LocalZ < LAYER_SIZE - 1) ? m_Chunks[Index + LAYER_SIZE] : m_Parent->FindChunk(a_ChunkX,     a_ChunkZ + 1);
		m_Chunks[Index] = new cChunk(a_ChunkX, a_ChunkZ, m_Parent, m_Parent->GetWorld(), neixm, neixp, neizm, neizp, m_Pool);
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
		return nullptr;
	}
	
	int Index = LocalX + LocalZ * LAYER_SIZE;
	return m_Chunks[Index];
}




void cChunkMap::cChunkLayer::CollectMobCensus(cMobCensus & a_ToFill)
{
	for (size_t i = 0; i < ARRAYCOUNT(m_Chunks); i++)
	{
		// We do count every Mobs in the world. But we are assuming that every chunk not loaded by any client
		// doesn't affect us. Normally they should not have mobs because every "too far" mobs despawn
		// If they have (f.i. when player disconnect) we assume we don't have to make them live or despawn
		if ((m_Chunks[i] != nullptr) && m_Chunks[i]->IsValid() && m_Chunks[i]->HasAnyClients())
		{
			m_Chunks[i]->CollectMobCensus(a_ToFill);
		}
	}  // for i - m_Chunks[]
}






void cChunkMap::cChunkLayer::SpawnMobs(cMobSpawner & a_MobSpawner)
{
	for (size_t i = 0; i < ARRAYCOUNT(m_Chunks); i++)
	{
		// We only spawn close to players
		if ((m_Chunks[i] != nullptr) && m_Chunks[i]->IsValid() && m_Chunks[i]->HasAnyClients())
		{
			m_Chunks[i]->SpawnMobs(a_MobSpawner);
		}
	}  // for i - m_Chunks[]
}





void cChunkMap::cChunkLayer::Tick(std::chrono::milliseconds a_Dt)
{
	for (size_t i = 0; i < ARRAYCOUNT(m_Chunks); i++)
	{
		// Only tick chunks that are valid and should be ticked:
		if ((m_Chunks[i] != nullptr) && m_Chunks[i]->IsValid() && m_Chunks[i]->ShouldBeTicked())
		{
			m_Chunks[i]->Tick(a_Dt);
		}
	}  // for i - m_Chunks[]
}





void cChunkMap::cChunkLayer::RemoveClient(cClientHandle * a_Client)
{
	for (size_t i = 0; i < ARRAYCOUNT(m_Chunks); i++)
	{
		if (m_Chunks[i] != nullptr)
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
		if ((m_Chunks[i] != nullptr) && m_Chunks[i]->IsValid())
		{
			if (!m_Chunks[i]->ForEachEntity(a_Callback))
			{
				return false;
			}
		}
	}
	return true;
}





bool cChunkMap::cChunkLayer::DoWithEntityByID(UInt32 a_EntityID, cEntityCallback & a_Callback, bool & a_CallbackReturn)
{
	// Calls the callback if the entity with the specified ID is found, with the entity object as the callback param. Returns true if entity found.
	for (size_t i = 0; i < ARRAYCOUNT(m_Chunks); i++)
	{
		if ((m_Chunks[i] != nullptr) && m_Chunks[i]->IsValid())
		{
			if (m_Chunks[i]->DoWithEntityByID(a_EntityID, a_Callback, a_CallbackReturn))
			{
				return true;
			}
		}
	}
	return false;
}





bool cChunkMap::cChunkLayer::HasEntity(UInt32 a_EntityID)
{
	for (size_t i = 0; i < ARRAYCOUNT(m_Chunks); i++)
	{
		if ((m_Chunks[i] != nullptr) && m_Chunks[i]->IsValid())
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
		if (m_Chunks[i] != nullptr)
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
		if (m_Chunks[i] == nullptr)
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
		if ((m_Chunks[i] != nullptr) && m_Chunks[i]->IsValid() && m_Chunks[i]->IsDirty())
		{
			World->GetStorage().QueueSaveChunk(m_Chunks[i]->GetPosX(), m_Chunks[i]->GetPosZ());
		}
	}  // for i - m_Chunks[]
}





void cChunkMap::cChunkLayer::UnloadUnusedChunks(void)
{
	for (size_t i = 0; i < ARRAYCOUNT(m_Chunks); i++)
	{
		if (
			(m_Chunks[i] != nullptr) &&   // Is valid
			(m_Chunks[i]->CanUnload()) &&  // Can unload
			!cPluginManager::Get()->CallHookChunkUnloading(*(m_Parent->GetWorld()), m_Chunks[i]->GetPosX(), m_Chunks[i]->GetPosZ())  // Plugins agree
		)
		{
			// The cChunk destructor calls our GetChunk() while removing its entities
			// so we still need to be able to return the chunk. Therefore we first delete, then nullptrify
			// Doing otherwise results in bug http://forum.mc-server.org/showthread.php?tid=355
			delete m_Chunks[i];
			m_Chunks[i] = nullptr;
		}
	}  // for i - m_Chunks[]
}





void cChunkMap::FastSetBlock(int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta)
{
	cCSLock Lock(m_CSFastSetBlock);
	m_FastSetBlockQueue.push_back(sSetBlock(a_BlockX, a_BlockY, a_BlockZ, a_BlockType, a_BlockMeta));
}





void cChunkMap::FastSetQueuedBlocks()
{
	// Asynchronously set blocks:
	sSetBlockList FastSetBlockQueueCopy;
	{
		cCSLock Lock(m_CSFastSetBlock);
		std::swap(FastSetBlockQueueCopy, m_FastSetBlockQueue);
	}
	this->FastSetBlocks(FastSetBlockQueueCopy);
	if (!FastSetBlockQueueCopy.empty())
	{
		// Some blocks failed, store them for next tick:
		cCSLock Lock(m_CSFastSetBlock);
		m_FastSetBlockQueue.splice(m_FastSetBlockQueue.end(), FastSetBlockQueueCopy);
	}
}





void cChunkMap::AddChunkStay(cChunkStay & a_ChunkStay)
{
	cCSLock Lock(m_CSLayers);
	
	// Add it to the list:
	ASSERT(std::find(m_ChunkStays.begin(), m_ChunkStays.end(), &a_ChunkStay) == m_ChunkStays.end());  // Has not yet been added
	m_ChunkStays.push_back(&a_ChunkStay);
	
	// Schedule all chunks to be loaded / generated, and mark each as locked:
	const cChunkCoordsVector & WantedChunks = a_ChunkStay.GetChunks();
	for (cChunkCoordsVector::const_iterator itr = WantedChunks.begin(); itr != WantedChunks.end(); ++itr)
	{
		cChunkPtr Chunk = GetChunk(itr->m_ChunkX, itr->m_ChunkZ);
		if (Chunk == nullptr)
		{
			continue;
		}
		Chunk->Stay(true);
		if (Chunk->IsValid())
		{
			if (a_ChunkStay.ChunkAvailable(itr->m_ChunkX, itr->m_ChunkZ))
			{
				// The chunkstay wants to be deactivated, disable it and bail out:
				a_ChunkStay.Disable();
				return;
			}
		}
	}  // for itr - WantedChunks[]
}





/** Removes the specified cChunkStay descendant from the internal list of ChunkStays. */
void cChunkMap::DelChunkStay(cChunkStay & a_ChunkStay)
{
	cCSLock Lock(m_CSLayers);
	
	// Remove from the list of active chunkstays:
	bool HasFound = false;
	for (cChunkStays::iterator itr = m_ChunkStays.begin(), end = m_ChunkStays.end(); itr != end; ++itr)
	{
		if (*itr == &a_ChunkStay)
		{
			m_ChunkStays.erase(itr);
			HasFound = true;
			break;
		}
	}  // for itr - m_ChunkStays[]
	
	if (!HasFound)
	{
		ASSERT(!"Removing a cChunkStay that hasn't been added!");
		return;
	}
	
	// Unmark all contained chunks:
	const cChunkCoordsVector & Chunks = a_ChunkStay.GetChunks();
	for (cChunkCoordsVector::const_iterator itr = Chunks.begin(), end = Chunks.end(); itr != end; ++itr)
	{
		cChunkPtr Chunk = GetChunkNoLoad(itr->m_ChunkX, itr->m_ChunkZ);
		if (Chunk == nullptr)
		{
			continue;
		}
		Chunk->Stay(false);
	}  // for itr - Chunks[]
	a_ChunkStay.OnDisabled();
}





