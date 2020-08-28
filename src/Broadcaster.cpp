// Broadcaster.cpp

// Implements the broadcasting functions for cWorld

#include "Globals.h"
#include "World.h"
#include "Chunk.h"
#include "ClientHandle.h"
#include "Entities/Entity.h"
#include "Entities/Player.h"
#include "BlockEntities/BlockEntity.h"





namespace
{

	/** Calls the function object a_Func for every active client in the world
	\param a_World World the clients are in
	\param a_Exclude Client for which a_Func should not be called
	\param a_Func Function to be called with each non-excluded client */
	template <typename Func>
	void ForClientsInWorld(cWorld & a_World, const cClientHandle * a_Exclude, Func a_Func)
	{
		a_World.ForEachPlayer([&](cPlayer & a_Player)
			{
				cClientHandle * Client = a_Player.GetClientHandle();
				if ((Client != a_Exclude) && (Client != nullptr) && Client->IsLoggedIn() && !Client->IsDestroyed())
				{
					a_Func(*Client);
				}
				return false;
			}
		);
	}


	/** Calls the function object a_Func for every client who has the specified chunk
	\param a_ChunkCoords Coords of the chunk to query for clients
	\param a_World World that the chunk is in
	\param a_Exclude Client for which a_Func should not be called
	\param a_Func Function to be called with each non-excluded client */
	template <typename Func>
	void ForClientsWithChunk(const cChunkCoords a_ChunkCoords, cWorld & a_World, const cClientHandle * a_Exclude, Func a_Func)
	{
		a_World.DoWithChunk(a_ChunkCoords.m_ChunkX, a_ChunkCoords.m_ChunkZ,
			[&](cChunk & a_Chunk)
			{
				for (auto * Client : a_Chunk.GetAllClients())
				{
					if (Client != a_Exclude)
					{
						a_Func(*Client);
					}
				}
				return true;
			}
		);
	}



	/** Calls the function object a_Func for every client who has the chunk at the specified block position
	\param a_WorldPos Coordinates of the block to query for clients
	\param a_World World that the block is in
	\param a_Exclude Client for which a_Func should not be called
	\param a_Func Function to be called with each non-excluded client */
	template <typename Func>
	void ForClientsWithChunkAtPos(const Vector3i a_WorldPos, cWorld & a_World, const cClientHandle * a_Exclude, Func a_Func)
	{
		ForClientsWithChunk(cChunkDef::BlockToChunk(a_WorldPos), a_World, a_Exclude, std::move(a_Func));
	}



	/** Calls the function object a_Func for every client who has the specified entity
	\param a_Entity Entity to query for clients
	\param a_World World that the block is in
	\param a_Exclude Client for which a_Func should not be called
	\param a_Func Function to be called with each non-excluded client */
	template <typename Func>
	void ForClientsWithEntity(const cEntity & a_Entity, cWorld & a_World, const cClientHandle * a_Exclude, Func a_Func)
	{
		cWorld::cLock Lock(a_World);  // Lock world before accessing a_Entity
		auto Chunk = a_Entity.GetParentChunk();
		if (Chunk != nullptr)
		{
			for (auto * Client : Chunk->GetAllClients())
			{
				if (Client != a_Exclude)
				{
					a_Func(*Client);
				}
			}
		}
		else  // Some broadcasts happen before the entity's first tick sets its ParentChunk
		{
			ForClientsWithChunk({ a_Entity.GetChunkX(), a_Entity.GetChunkZ() }, a_World, a_Exclude, std::move(a_Func));
		}
	}
}  // namespace (anonymous)





void cWorld::BroadcastAttachEntity(const cEntity & a_Entity, const cEntity & a_Vehicle)
{
	ForClientsWithEntity(a_Entity, *this, nullptr, [&](cClientHandle & a_Client)
		{
			a_Client.SendAttachEntity(a_Entity, a_Vehicle);
		}
	);
}





void cWorld::BroadcastBlockAction(Vector3i a_BlockPos, Byte a_Byte1, Byte a_Byte2, BLOCKTYPE a_BlockType, const cClientHandle * a_Exclude)
{
	ForClientsWithChunkAtPos(a_BlockPos, *this, a_Exclude, [&](cClientHandle & a_Client)
		{
			a_Client.SendBlockAction(a_BlockPos.x, a_BlockPos.y, a_BlockPos.z, static_cast<char>(a_Byte1), static_cast<char>(a_Byte2), a_BlockType);
		}
	);
}





void cWorld::BroadcastBlockBreakAnimation(UInt32 a_EntityID, Vector3i a_BlockPos, Int8 a_Stage, const cClientHandle * a_Exclude)
{
	ForClientsWithChunkAtPos(a_BlockPos, *this, a_Exclude, [&](cClientHandle & a_Client)
		{
			a_Client.SendBlockBreakAnim(a_EntityID, a_BlockPos.x, a_BlockPos.y, a_BlockPos.z, a_Stage);
		}
	);
}





void cWorld::BroadcastBlockEntity(Vector3i a_BlockPos, const cClientHandle * a_Exclude)
{
	DoWithChunkAt(a_BlockPos, [&](cChunk & a_Chunk)
		{
			cBlockEntity * Entity = a_Chunk.GetBlockEntity(a_BlockPos);
			if (Entity == nullptr)
			{
				return false;
			}

			for (auto * Client : a_Chunk.GetAllClients())
			{
				if (Client != a_Exclude)
				{
					Entity->SendTo(*Client);
				}
			}
			return true;
		}
	);
}





void cWorld::BroadcastChat(const AString & a_Message, const cClientHandle * a_Exclude, eMessageType a_ChatPrefix)
{
	ForClientsInWorld(*this, a_Exclude, [&](cClientHandle & a_Client)
		{
			a_Client.SendChat(a_Message, a_ChatPrefix);
		}
	);
}





void cWorld::BroadcastChat(const cCompositeChat & a_Message, const cClientHandle * a_Exclude)
{
	ForClientsInWorld(*this, a_Exclude, [&](cClientHandle & a_Client)
		{
			a_Client.SendChat(a_Message);
		}
	);
}





void cWorld::BroadcastCollectEntity(const cEntity & a_Collected, const cEntity & a_Collector, unsigned a_Count, const cClientHandle * a_Exclude)
{
	ForClientsWithEntity(a_Collected, *this, a_Exclude, [&](cClientHandle & a_Client)
		{
			a_Client.SendCollectEntity(a_Collected, a_Collector, a_Count);
		}
	);
}





void cWorld::BroadcastDestroyEntity(const cEntity & a_Entity, const cClientHandle * a_Exclude)
{
	ForClientsWithEntity(a_Entity, *this, a_Exclude, [&](cClientHandle & a_Client)
		{
			a_Client.SendDestroyEntity(a_Entity);
		}
	);
}





void cWorld::BroadcastDetachEntity(const cEntity & a_Entity, const cEntity & a_PreviousVehicle)
{
	ForClientsWithEntity(a_Entity, *this, nullptr, [&](cClientHandle & a_Client)
		{
			a_Client.SendDetachEntity(a_Entity, a_PreviousVehicle);
		}
	);
}





void cWorld::BroadcastDisplayObjective(const AString & a_Objective, cScoreboard::eDisplaySlot a_Display)
{
	ForClientsInWorld(*this, nullptr, [&](cClientHandle & a_Client)
		{
			a_Client.SendDisplayObjective(a_Objective, a_Display);
		}
	);
}





void cWorld::BroadcastEntityEffect(const cEntity & a_Entity, int a_EffectID, int a_Amplifier, int a_Duration, const cClientHandle * a_Exclude)
{
	ForClientsWithEntity(a_Entity, *this, a_Exclude, [&](cClientHandle & a_Client)
		{
			a_Client.SendEntityEffect(a_Entity, a_EffectID, a_Amplifier, a_Duration);
		}
	);
}





void cWorld::BroadcastEntityEquipment(const cEntity & a_Entity, short a_SlotNum, const cItem & a_Item, const cClientHandle * a_Exclude)
{
	ForClientsWithEntity(a_Entity, *this, a_Exclude, [&](cClientHandle & a_Client)
		{
			a_Client.SendEntityEquipment(a_Entity, a_SlotNum, a_Item);
		}
	);
}





void cWorld::BroadcastEntityHeadLook(const cEntity & a_Entity, const cClientHandle * a_Exclude)
{
	ForClientsWithEntity(a_Entity, *this, a_Exclude, [&](cClientHandle & a_Client)
		{
			a_Client.SendEntityHeadLook(a_Entity);
		}
	);
}





void cWorld::BroadcastEntityLook(const cEntity & a_Entity, const cClientHandle * a_Exclude)
{
	ForClientsWithEntity(a_Entity, *this, a_Exclude, [&](cClientHandle & a_Client)
		{
			a_Client.SendEntityLook(a_Entity);
		}
	);
}





void cWorld::BroadcastEntityMetadata(const cEntity & a_Entity, const cClientHandle * a_Exclude)
{
	ForClientsWithEntity(a_Entity, *this, a_Exclude, [&](cClientHandle & a_Client)
		{
			a_Client.SendEntityMetadata(a_Entity);
		}
	);
}





void cWorld::BroadcastEntityPosition(const cEntity & a_Entity, const cClientHandle * a_Exclude)
{
	ForClientsWithEntity(a_Entity, *this, a_Exclude, [&](cClientHandle & a_Client)
		{
			a_Client.SendEntityPosition(a_Entity);
		}
	);
}





void cWorld::BroadcastEntityStatus(const cEntity & a_Entity, Int8 a_Status, const cClientHandle * a_Exclude)
{
	ForClientsWithEntity(a_Entity, *this, a_Exclude, [&](cClientHandle & a_Client)
		{
			a_Client.SendEntityStatus(a_Entity, a_Status);
		}
	);
}





void cWorld::BroadcastEntityVelocity(const cEntity & a_Entity, const cClientHandle * a_Exclude)
{
	ForClientsWithEntity(a_Entity, *this, a_Exclude, [&](cClientHandle & a_Client)
		{
			a_Client.SendEntityVelocity(a_Entity);
		}
	);
}





void cWorld::BroadcastEntityAnimation(const cEntity & a_Entity, Int8 a_Animation, const cClientHandle * a_Exclude)
{
	ForClientsWithEntity(a_Entity, *this, a_Exclude, [&](cClientHandle & a_Client)
		{
			a_Client.SendEntityAnimation(a_Entity, a_Animation);
		}
	);
}





void cWorld::BroadcastLeashEntity(const cEntity & a_Entity, const cEntity & a_EntityLeashedTo)
{
	ForClientsWithEntity(a_Entity, *this, nullptr, [&](cClientHandle & a_Client)
		{
			a_Client.SendLeashEntity(a_Entity, a_EntityLeashedTo);
		}
	);
}





void cWorld::BroadcastParticleEffect(const AString & a_ParticleName, const Vector3f a_Src, const Vector3f a_Offset, float a_ParticleData, int a_ParticleAmount, const cClientHandle * a_Exclude)
{
	ForClientsWithChunkAtPos(a_Src, *this, a_Exclude, [&](cClientHandle & a_Client)
		{
			a_Client.SendParticleEffect(a_ParticleName, a_Src.x, a_Src.y, a_Src.z, a_Offset.x, a_Offset.y, a_Offset.z, a_ParticleData, a_ParticleAmount);
		}
	);
}





void cWorld::BroadcastParticleEffect(const AString & a_ParticleName, const Vector3f a_Src, const Vector3f a_Offset, float a_ParticleData, int a_ParticleAmount, std::array<int, 2> a_Data, const cClientHandle * a_Exclude)
{
	ForClientsWithChunkAtPos(a_Src, *this, a_Exclude, [&](cClientHandle & a_Client)
		{
			a_Client.SendParticleEffect(a_ParticleName, a_Src, a_Offset, a_ParticleData, a_ParticleAmount, a_Data);
		}
	);
}





void cWorld::BroadcastPlayerListAddPlayer(const cPlayer & a_Player, const cClientHandle * a_Exclude)
{
	ForClientsInWorld(*this, a_Exclude, [&](cClientHandle & a_Client)
		{
			a_Client.SendPlayerListAddPlayer(a_Player);
		}
	);
}





void cWorld::BroadcastPlayerListRemovePlayer(const cPlayer & a_Player, const cClientHandle * a_Exclude)
{
	ForClientsInWorld(*this, a_Exclude, [&](cClientHandle & a_Client)
		{
			a_Client.SendPlayerListRemovePlayer(a_Player);
		}
	);
}





void cWorld::BroadcastPlayerListUpdateGameMode(const cPlayer & a_Player, const cClientHandle * a_Exclude)
{
	ForClientsInWorld(*this, a_Exclude, [&](cClientHandle & a_Client)
		{
			a_Client.SendPlayerListUpdateGameMode(a_Player);
		}
	);
}





void cWorld::BroadcastPlayerListUpdatePing(const cPlayer & a_Player, const cClientHandle * a_Exclude)
{
	ForClientsInWorld(*this, a_Exclude, [&](cClientHandle & a_Client)
		{
			a_Client.SendPlayerListUpdatePing(a_Player);
		}
	);
}





void cWorld::BroadcastPlayerListUpdateDisplayName(const cPlayer & a_Player, const AString & a_CustomName, const cClientHandle * a_Exclude)
{
	ForClientsInWorld(*this, a_Exclude, [&](cClientHandle & a_Client)
		{
			a_Client.SendPlayerListUpdateDisplayName(a_Player, a_CustomName);
		}
	);
}





void cWorld::BroadcastRemoveEntityEffect(const cEntity & a_Entity, int a_EffectID, const cClientHandle * a_Exclude)
{
	ForClientsWithEntity(a_Entity, *this, a_Exclude, [&](cClientHandle & a_Client)
		{
			a_Client.SendRemoveEntityEffect(a_Entity, a_EffectID);
		}
	);
}





void cWorld::BroadcastScoreboardObjective(const AString & a_Name, const AString & a_DisplayName, Byte a_Mode)
{
	ForClientsInWorld(*this, nullptr, [&](cClientHandle & a_Client)
		{
			a_Client.SendScoreboardObjective(a_Name, a_DisplayName, a_Mode);
		}
	);
}





void cWorld::BroadcastScoreUpdate(const AString & a_Objective, const AString & a_PlayerName, cObjective::Score a_Score, Byte a_Mode)
{
	ForClientsInWorld(*this, nullptr, [&](cClientHandle & a_Client)
		{
			a_Client.SendScoreUpdate(a_Objective, a_PlayerName, a_Score, a_Mode);
		}
	);
}





void cWorld::BroadcastSoundEffect(const AString & a_SoundName, Vector3d a_Position, float a_Volume, float a_Pitch, const cClientHandle * a_Exclude)
{
	ForClientsWithChunkAtPos(a_Position, *this, a_Exclude, [&](cClientHandle & a_Client)
		{
			a_Client.SendSoundEffect(a_SoundName, a_Position, a_Volume, a_Pitch);
		}
	);
}





void cWorld::BroadcastSoundParticleEffect(const EffectID a_EffectID, Vector3i a_SrcPos, int a_Data, const cClientHandle * a_Exclude)
{
	ForClientsWithChunkAtPos(a_SrcPos, *this, a_Exclude, [&](cClientHandle & a_Client)
		{
			a_Client.SendSoundParticleEffect(a_EffectID, a_SrcPos.x, a_SrcPos.y, a_SrcPos.z, a_Data);
		}
	);
}





void cWorld::BroadcastSpawnEntity(cEntity & a_Entity, const cClientHandle * a_Exclude)
{
	ForClientsWithEntity(a_Entity, *this, a_Exclude, [&](cClientHandle & a_Client)
		{
			a_Entity.SpawnOn(a_Client);
		}
	);
}





void cWorld::BroadcastThunderbolt(Vector3i a_BlockPos, const cClientHandle * a_Exclude)
{
	ForClientsWithChunkAtPos(a_BlockPos, *this, a_Exclude, [&](cClientHandle & a_Client)
		{
			a_Client.SendThunderbolt(a_BlockPos.x, a_BlockPos.y, a_BlockPos.z);
		}
	);
}





void cWorld::BroadcastTimeUpdate(const cClientHandle * a_Exclude)
{
	ForClientsInWorld(*this, a_Exclude, [&](cClientHandle & a_Client)
		{
			a_Client.SendTimeUpdate(GetWorldAge(), GetTimeOfDay(), IsDaylightCycleEnabled());
		}
	);
}





void cWorld::BroadcastUnleashEntity(const cEntity & a_Entity)
{
	ForClientsWithEntity(a_Entity, *this, nullptr, [&](cClientHandle & a_Client)
		{
			a_Client.SendUnleashEntity(a_Entity);
		}
	);
}





void cWorld::BroadcastUseBed(const cEntity & a_Entity, Vector3i a_BedPos)
{
	ForClientsWithChunkAtPos(a_BedPos, *this, nullptr, [&](cClientHandle & a_Client)
		{
			a_Client.SendUseBed(a_Entity, a_BedPos.x, a_BedPos.y, a_BedPos.z);
		}
	);
}





void cWorld::BroadcastWeather(eWeather a_Weather, const cClientHandle * a_Exclude)
{
	ForClientsInWorld(*this, a_Exclude, [&](cClientHandle & a_Client)
		{
			a_Client.SendWeather(a_Weather);
		}
	);
}
