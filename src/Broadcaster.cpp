// Broadcaster.cpp

// Implements the broadcasting interface for cWorld

#include "Globals.h"
#include "Broadcaster.h"
#include "World.h"
#include "Chunk.h"
#include "ClientHandle.h"
#include "Entities/Player.h"





namespace
{
	template <typename Func>
	void ForClientsLoading(Vector3i a_WorldPos, cWorld & a_World, cClientHandle * a_Exclude, Func a_Func)
	{
		a_World.DoWithChunkAt(a_WorldPos,
			[&](cChunk & a_Chunk)
			{
				for (auto * Client : a_Chunk.GetAllClients())
				{
					if (Client != a_Exclude)
					{
						a_Func(*Client);
					}
				};
				return true;
			}
		);
	}
}  // namespace (anonymous)





cBroadcaster::cBroadcaster(cWorld * a_World) :
	m_World(a_World)
{
}





void cBroadcaster::BroadcastAttachEntity(const cEntity & a_Entity, const cEntity & a_Vehicle)
{
	cWorld::cLock Lock(*m_World);  // Lock before accessing a_Entity
	m_World->DoWithChunk(a_Entity.GetChunkX(), a_Entity.GetChunkZ(), [&](cChunk & a_Chunk)
		{
			a_Chunk.BroadcastAttachEntity(a_Entity, a_Vehicle);
			return true;
		}
	);
}





void cBroadcaster::BroadcastBlockAction(Vector3i a_BlockPos, Byte a_Byte1, Byte a_Byte2, BLOCKTYPE a_BlockType, const cClientHandle * a_Exclude)
{
	m_World->DoWithChunkAt(a_BlockPos, [&](cChunk & a_Chunk)
		{
			a_Chunk.BroadcastBlockAction(a_BlockPos, static_cast<char>(a_Byte1), static_cast<char>(a_Byte2), a_BlockType, a_Exclude);
			return true;
		}
	);
}





void cBroadcaster::BroadcastBlockBreakAnimation(UInt32 a_EntityID, Vector3i a_BlockPos, char a_Stage, const cClientHandle * a_Exclude)
{
	m_World->DoWithChunkAt(a_BlockPos, [&](cChunk & a_Chunk)
		{
			a_Chunk.BroadcastBlockBreakAnimation(a_EntityID, a_BlockPos, a_Stage, a_Exclude);
			return true;
		}
	);
}





void cBroadcaster::BroadcastBlockEntity(Vector3i a_BlockPos, const cClientHandle * a_Exclude)
{
	m_World->DoWithChunkAt(a_BlockPos, [&](cChunk & a_Chunk)
		{
			a_Chunk.BroadcastBlockEntity(a_BlockPos, a_Exclude);
			return true;
		}
	);
}





void cBroadcaster::BroadcastChat(const AString & a_Message, const cClientHandle * a_Exclude, eMessageType a_ChatPrefix)
{
	m_World->ForEachPlayer([&](cPlayer & a_Player)
		{
			cClientHandle * Ch = a_Player.GetClientHandle();
			if ((Ch != a_Exclude) && (Ch != nullptr) && Ch->IsLoggedIn() && !Ch->IsDestroyed())
			{
				Ch->SendChat(a_Message, a_ChatPrefix);
			}
			return false;
		}
	);
}





void cBroadcaster::BroadcastChat(const cCompositeChat & a_Message, const cClientHandle * a_Exclude)
{
	m_World->ForEachPlayer([&](cPlayer & a_Player)
		{
			cClientHandle * Ch = a_Player.GetClientHandle();
			if ((Ch != a_Exclude) && (Ch != nullptr) && Ch->IsLoggedIn() && !Ch->IsDestroyed())
			{
				Ch->SendChat(a_Message);
			}
			return false;
		}
	);
}





void cBroadcaster::BroadcastCollectEntity(const cEntity & a_Entity, const cPlayer & a_Player, int a_Count, const cClientHandle * a_Exclude)
{
	cWorld::cLock Lock(*m_World);  // Lock before accessing a_Entity
	m_World->DoWithChunk(a_Entity.GetChunkX(), a_Entity.GetChunkZ(), [&](cChunk & a_Chunk)
		{
			a_Chunk.BroadcastCollectEntity(a_Entity, a_Player, a_Count, a_Exclude);
			return true;
		}
	);
}





void cBroadcaster::BroadcastDestroyEntity(const cEntity & a_Entity, const cClientHandle * a_Exclude)
{
	cWorld::cLock Lock(*m_World);  // Lock before accessing a_Entity
	m_World->DoWithChunk(a_Entity.GetChunkX(), a_Entity.GetChunkZ(), [&](cChunk & a_Chunk)
		{
			a_Chunk.BroadcastDestroyEntity(a_Entity, a_Exclude);
			return true;
		}
	);
}





void cBroadcaster::BroadcastDetachEntity(const cEntity & a_Entity, const cEntity & a_PreviousVehicle)
{
	cWorld::cLock Lock(*m_World);  // Lock before accessing a_Entity
	m_World->DoWithChunk(a_Entity.GetChunkX(), a_Entity.GetChunkZ(), [&](cChunk & a_Chunk)
		{
			a_Chunk.BroadcastDetachEntity(a_Entity, a_PreviousVehicle);
			return true;
		}
	);
}





void cBroadcaster::BroadcastLeashEntity(const cEntity & a_Entity, const cEntity & a_EntityLeashedTo)
{
	cWorld::cLock Lock(*m_World);  // Lock before accessing a_Entity
	m_World->DoWithChunk(a_Entity.GetChunkX(), a_Entity.GetChunkZ(), [&](cChunk & a_Chunk)
		{
			a_Chunk.BroadcastLeashEntity(a_Entity, a_EntityLeashedTo);
			return true;
		}
	);
}





void cBroadcaster::BroadcastUnleashEntity(const cEntity & a_Entity)
{
	cWorld::cLock Lock(*m_World);  // Lock before accessing a_Entity
	m_World->DoWithChunk(a_Entity.GetChunkX(), a_Entity.GetChunkZ(), [&](cChunk & a_Chunk)
		{
			a_Chunk.BroadcastUnleashEntity(a_Entity);
			return true;
		}
	);
}





void cBroadcaster::BroadcastEntityEffect(const cEntity & a_Entity, int a_EffectID, int a_Amplifier, short a_Duration, const cClientHandle * a_Exclude)
{
	cWorld::cLock Lock(*m_World);  // Lock before accessing a_Entity
	m_World->DoWithChunk(a_Entity.GetChunkX(), a_Entity.GetChunkZ(), [&](cChunk & a_Chunk)
		{
			a_Chunk.BroadcastEntityEffect(a_Entity, a_EffectID, a_Amplifier, a_Duration, a_Exclude);
			return true;
		}
	);
}





void cBroadcaster::BroadcastEntityEquipment(const cEntity & a_Entity, short a_SlotNum, const cItem & a_Item, const cClientHandle * a_Exclude)
{
	cWorld::cLock Lock(*m_World);  // Lock before accessing a_Entity
	m_World->DoWithChunk(a_Entity.GetChunkX(), a_Entity.GetChunkZ(), [&](cChunk & a_Chunk)
		{
			a_Chunk.BroadcastEntityEquipment(a_Entity, a_SlotNum, a_Item, a_Exclude);
			return true;
		}
	);
}





void cBroadcaster::BroadcastEntityHeadLook(const cEntity & a_Entity, const cClientHandle * a_Exclude)
{
	cWorld::cLock Lock(*m_World);  // Lock before accessing a_Entity
	m_World->DoWithChunk(a_Entity.GetChunkX(), a_Entity.GetChunkZ(), [&](cChunk & a_Chunk)
		{
			a_Chunk.BroadcastEntityHeadLook(a_Entity, a_Exclude);
			return true;
		}
	);
}





void cBroadcaster::BroadcastEntityLook(const cEntity & a_Entity, const cClientHandle * a_Exclude)
{
	cWorld::cLock Lock(*m_World);  // Lock before accessing a_Entity
	m_World->DoWithChunk(a_Entity.GetChunkX(), a_Entity.GetChunkZ(), [&](cChunk & a_Chunk)
		{
			a_Chunk.BroadcastEntityLook(a_Entity, a_Exclude);
			return true;
		}
	);
}





void cBroadcaster::BroadcastEntityMetadata(const cEntity & a_Entity, const cClientHandle * a_Exclude)
{
	cWorld::cLock Lock(*m_World);  // Lock before accessing a_Entity
	m_World->DoWithChunk(a_Entity.GetChunkX(), a_Entity.GetChunkZ(), [&](cChunk & a_Chunk)
		{
			a_Chunk.BroadcastEntityMetadata(a_Entity, a_Exclude);
			return true;
		}
	);
}





void cBroadcaster::BroadcastEntityRelMove(const cEntity & a_Entity, Vector3<char> a_RelMove, const cClientHandle * a_Exclude)
{
	cWorld::cLock Lock(*m_World);  // Lock before accessing a_Entity
	m_World->DoWithChunk(a_Entity.GetChunkX(), a_Entity.GetChunkZ(), [&](cChunk & a_Chunk)
		{
			a_Chunk.BroadcastEntityRelMove(a_Entity, a_RelMove.x, a_RelMove.y, a_RelMove.z, a_Exclude);
			return true;
		}
	);
}





void cBroadcaster::BroadcastEntityRelMoveLook(const cEntity & a_Entity, Vector3<char> a_RelMove, const cClientHandle * a_Exclude)
{
	cWorld::cLock Lock(*m_World);  // Lock before accessing a_Entity
	m_World->DoWithChunk(a_Entity.GetChunkX(), a_Entity.GetChunkZ(), [&](cChunk & a_Chunk)
		{
			a_Chunk.BroadcastEntityRelMoveLook(a_Entity, a_RelMove.x, a_RelMove.y, a_RelMove.z, a_Exclude);
			return true;
		}
	);
}





void cBroadcaster::BroadcastEntityStatus(const cEntity & a_Entity, char a_Status, const cClientHandle * a_Exclude)
{
	cWorld::cLock Lock(*m_World);  // Lock before accessing a_Entity
	m_World->DoWithChunk(a_Entity.GetChunkX(), a_Entity.GetChunkZ(), [&](cChunk & a_Chunk)
		{
			a_Chunk.BroadcastEntityStatus(a_Entity, a_Status, a_Exclude);
			return true;
		}
	);
}





void cBroadcaster::BroadcastEntityVelocity(const cEntity & a_Entity, const cClientHandle * a_Exclude)
{
	cWorld::cLock Lock(*m_World);  // Lock before accessing a_Entity
	m_World->DoWithChunk(a_Entity.GetChunkX(), a_Entity.GetChunkZ(), [&](cChunk & a_Chunk)
		{
			a_Chunk.BroadcastEntityVelocity(a_Entity, a_Exclude);
			return true;
		}
	);
}




void cBroadcaster::BroadcastEntityAnimation(const cEntity & a_Entity, char a_Animation, const cClientHandle * a_Exclude)
{
	cWorld::cLock Lock(*m_World);  // Lock before accessing a_Entity
	m_World->DoWithChunk(a_Entity.GetChunkX(), a_Entity.GetChunkZ(), [&](cChunk & a_Chunk)
		{
			a_Chunk.BroadcastEntityAnimation(a_Entity, a_Animation, a_Exclude);
			return true;
		}
	);
}





void cBroadcaster::BroadcastParticleEffect(const AString & a_ParticleName, const Vector3f a_Src, const Vector3f a_Offset, float a_ParticleData, int a_ParticleAmount, cClientHandle * a_Exclude)
{
	ForClientsLoading(a_Src, *m_World, a_Exclude,
		[=](cClientHandle & a_Client)
		{
			a_Client.SendParticleEffect(a_ParticleName, a_Src.x, a_Src.y, a_Src.z, a_Offset.x, a_Offset.y, a_Offset.z, a_ParticleData, a_ParticleAmount);
		}
	);
}





void cBroadcaster::BroadcastParticleEffect(const AString & a_ParticleName, const Vector3f a_Src, const Vector3f a_Offset, float a_ParticleData, int a_ParticleAmount, std::array<int, 2> a_Data, cClientHandle * a_Exclude)
{
	ForClientsLoading(a_Src, *m_World, a_Exclude,
		[=](cClientHandle & a_Client)
		{
			a_Client.SendParticleEffect(a_ParticleName, a_Src, a_Offset, a_ParticleData, a_ParticleAmount, a_Data);
		}
	);
}






void cBroadcaster::BroadcastPlayerListAddPlayer(const cPlayer & a_Player, const cClientHandle * a_Exclude)
{
	m_World->ForEachPlayer([&](cPlayer & a_Player)
		{
			cClientHandle * Ch = a_Player.GetClientHandle();
			if ((Ch != a_Exclude) && (Ch != nullptr) && Ch->IsLoggedIn() && !Ch->IsDestroyed())
			{
				Ch->SendPlayerListAddPlayer(a_Player);
			}
			return false;
		}
	);
}





void cBroadcaster::BroadcastPlayerListRemovePlayer(const cPlayer & a_Player, const cClientHandle * a_Exclude)
{
	m_World->ForEachPlayer([&](cPlayer & a_Player)
		{
			cClientHandle * Ch = a_Player.GetClientHandle();
			if ((Ch != a_Exclude) && (Ch != nullptr) && Ch->IsLoggedIn() && !Ch->IsDestroyed())
			{
				Ch->SendPlayerListRemovePlayer(a_Player);
			}
			return false;
		}
	);
}





void cBroadcaster::BroadcastPlayerListUpdateGameMode(const cPlayer & a_Player, const cClientHandle * a_Exclude)
{
	m_World->ForEachPlayer([&](cPlayer & a_Player)
		{
			cClientHandle * Ch = a_Player.GetClientHandle();
			if ((Ch != a_Exclude) && (Ch != nullptr) && Ch->IsLoggedIn() && !Ch->IsDestroyed())
			{
				Ch->SendPlayerListUpdateGameMode(a_Player);
			}
			return false;
		}
	);
}





void cBroadcaster::BroadcastPlayerListUpdatePing(const cPlayer & a_Player, const cClientHandle * a_Exclude)
{
	m_World->ForEachPlayer([&](cPlayer & a_Player)
		{
			cClientHandle * Ch = a_Player.GetClientHandle();
			if ((Ch != a_Exclude) && (Ch != nullptr) && Ch->IsLoggedIn() && !Ch->IsDestroyed())
			{
				Ch->SendPlayerListUpdatePing(a_Player);
			}
			return false;
		}
	);
}





void cBroadcaster::BroadcastPlayerListUpdateDisplayName(const cPlayer & a_Player, const AString & a_CustomName, const cClientHandle * a_Exclude)
{
	m_World->ForEachPlayer([&](cPlayer & a_Player)
		{
			cClientHandle * Ch = a_Player.GetClientHandle();
			if ((Ch != a_Exclude) && (Ch != nullptr) && Ch->IsLoggedIn() && !Ch->IsDestroyed())
			{
				Ch->SendPlayerListUpdateDisplayName(a_Player, a_CustomName);
			}
			return false;
		}
	);
}





void cBroadcaster::BroadcastRemoveEntityEffect(const cEntity & a_Entity, int a_EffectID, const cClientHandle * a_Exclude)
{
	cWorld::cLock Lock(*m_World);  // Lock before accessing a_Entity
	m_World->DoWithChunk(a_Entity.GetChunkX(), a_Entity.GetChunkZ(), [&](cChunk & a_Chunk)
		{
			a_Chunk.BroadcastRemoveEntityEffect(a_Entity, a_EffectID, a_Exclude);
			return true;
		}
	);
}





void cBroadcaster::BroadcastScoreboardObjective(const AString & a_Name, const AString & a_DisplayName, Byte a_Mode)
{
	m_World->ForEachPlayer([&](cPlayer & a_Player)
		{
			cClientHandle * Ch = a_Player.GetClientHandle();
			if ((Ch != nullptr) && Ch->IsLoggedIn() && !Ch->IsDestroyed())
			{
				Ch->SendScoreboardObjective(a_Name, a_DisplayName, a_Mode);
			}
			return false;
		}
	);
}





void cBroadcaster::BroadcastScoreUpdate(const AString & a_Objective, const AString & a_PlayerName, cObjective::Score a_Score, Byte a_Mode)
{
	m_World->ForEachPlayer([&](cPlayer & a_Player)
		{
			cClientHandle * Ch = a_Player.GetClientHandle();
			if ((Ch != nullptr) && Ch->IsLoggedIn() && !Ch->IsDestroyed())
			{
				Ch->SendScoreUpdate(a_Objective, a_PlayerName, a_Score, a_Mode);
			}
			return false;
		}
	);
}





void cBroadcaster::BroadcastDisplayObjective(const AString & a_Objective, cScoreboard::eDisplaySlot a_Display)
{
	m_World->ForEachPlayer([&](cPlayer & a_Player)
		{
			cClientHandle * Ch = a_Player.GetClientHandle();
			if ((Ch != nullptr) && Ch->IsLoggedIn() && !Ch->IsDestroyed())
			{
				Ch->SendDisplayObjective(a_Objective, a_Display);
			}
			return false;
		}
	);
}





void cBroadcaster::BroadcastSoundEffect(const AString & a_SoundName, Vector3d a_Position, float a_Volume, float a_Pitch, const cClientHandle * a_Exclude)
{
	m_World->DoWithChunkAt(a_Position, [&](cChunk & a_Chunk)
		{
			a_Chunk.BroadcastSoundEffect(a_SoundName, a_Position, a_Volume, a_Pitch, a_Exclude);
			return true;
		}
	);
}





void cBroadcaster::BroadcastSoundParticleEffect(const EffectID a_EffectID, Vector3i a_SrcPos, int a_Data, const cClientHandle * a_Exclude)
{
	m_World->DoWithChunkAt(a_SrcPos, [&](cChunk & a_Chunk)
		{
			a_Chunk.BroadcastSoundParticleEffect(a_EffectID, a_SrcPos.x, a_SrcPos.y, a_SrcPos.z, a_Data, a_Exclude);
			return true;
		}
	);
}





void cBroadcaster::BroadcastSpawnEntity(cEntity & a_Entity, const cClientHandle * a_Exclude)
{
	cWorld::cLock Lock(*m_World);  // Lock before accessing a_Entity
	m_World->DoWithChunk(a_Entity.GetChunkX(), a_Entity.GetChunkZ(), [&](cChunk & a_Chunk)
		{
			a_Chunk.BroadcastSpawnEntity(a_Entity, a_Exclude);
			return true;
		}
	);
}





void cBroadcaster::BroadcastTeleportEntity(const cEntity & a_Entity, const cClientHandle * a_Exclude)
{
	m_World->ForEachPlayer([&](cPlayer & a_Player)
		{
			cClientHandle * Ch = a_Player.GetClientHandle();
			if ((Ch != a_Exclude) && (Ch != nullptr) && Ch->IsLoggedIn() && !Ch->IsDestroyed())
			{
				Ch->SendTeleportEntity(a_Entity);
			}
			return false;
		}
	);
}





void cBroadcaster::BroadcastThunderbolt(Vector3i a_BlockPos, const cClientHandle * a_Exclude)
{
	m_World->DoWithChunkAt(a_BlockPos, [&](cChunk & a_Chunk)
		{
			a_Chunk.BroadcastThunderbolt(a_BlockPos, a_Exclude);
			return true;
		}
	);
}





void cBroadcaster::BroadcastTimeUpdate(const cClientHandle * a_Exclude)
{
	m_World->ForEachPlayer([&](cPlayer & a_Player)
		{
			cClientHandle * Ch = a_Player.GetClientHandle();
			if ((Ch != a_Exclude) && (Ch != nullptr) && Ch->IsLoggedIn() && !Ch->IsDestroyed())
			{
				Ch->SendTimeUpdate(m_World->GetWorldAge(), m_World->GetTimeOfDay(), m_World->IsDaylightCycleEnabled());
			}
			return false;
		}
	);
}





void cBroadcaster::BroadcastUseBed(const cEntity & a_Entity, Vector3i a_BedPos)
{
	m_World->DoWithChunkAt(a_BedPos, [&](cChunk & a_Chunk)
		{
			a_Chunk.BroadcastUseBed(a_Entity, a_BedPos.x, a_BedPos.y, a_BedPos.z);
			return true;
		}
	);
}





void cBroadcaster::BroadcastWeather(eWeather a_Weather, const cClientHandle * a_Exclude)
{
	m_World->ForEachPlayer([&](cPlayer & a_Player)
		{
			cClientHandle * Ch = a_Player.GetClientHandle();
			if ((Ch != a_Exclude) && (Ch != nullptr) && Ch->IsLoggedIn() && !Ch->IsDestroyed())
			{
				Ch->SendWeather(a_Weather);
			}
			return false;
		}
	);
}
