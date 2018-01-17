// Broadcaster.cpp

// Implements the broadcasting interface for cWorld

#include "Globals.h"
#include "Broadcaster.h"
#include "World.h"
#include "Chunk.h"
#include "ClientHandle.h"





cBroadcaster::cBroadcaster(cWorld * a_World) :
	m_World(a_World)
{
}





void cBroadcaster::BroadcastParticleEffect(const AString & a_ParticleName, const Vector3f a_Src, const Vector3f a_Offset, float a_ParticleData, int a_ParticleAmount, cClientHandle * a_Exclude)
{
	m_World->DoWithChunkAt(a_Src,
		[=](cChunk & a_Chunk) -> bool
		{
			for (auto && client : a_Chunk.GetAllClients())
			{
				if (client == a_Exclude)
				{
					continue;
				}
				client->SendParticleEffect(a_ParticleName, a_Src.x, a_Src.y, a_Src.z, a_Offset.x, a_Offset.y, a_Offset.z, a_ParticleData, a_ParticleAmount);
			};
			return true;
		});
}





void cBroadcaster::BroadcastParticleEffect(const AString & a_ParticleName, const Vector3f a_Src, const Vector3f a_Offset, float a_ParticleData, int a_ParticleAmount, std::array<int, 2> a_Data, cClientHandle * a_Exclude)
{
	m_World->DoWithChunkAt(a_Src,
		[=](cChunk & a_Chunk) -> bool
		{
			for (auto && client : a_Chunk.GetAllClients())
			{
				if (client == a_Exclude)
				{
					continue;
				}
				client->SendParticleEffect(a_ParticleName, a_Src, a_Offset, a_ParticleData, a_ParticleAmount, a_Data);
			};
			return true;
		});
}
