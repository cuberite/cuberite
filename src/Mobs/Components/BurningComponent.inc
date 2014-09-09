
#include "BurningComponent.h"

template <class EntityType, class ChunkType>
void cBurningComponent<EntityType, ChunkType>::Tick(float a_Dt, ChunkType & a_Chunk)
{
	
	int RelY = (int)floor(m_Self.GetPosY());
	if ((RelY < 0) || (RelY >= cChunkDef::Height))
	{
		// Outside the world
		return;
	}
	int PosX = (int)floor(m_Self.GetPosX());
	int PosZ = (int)floor(m_Self.GetPosZ());
	int RelX = PosX - m_Self.GetChunkX() * cChunkDef::Width;
	int RelZ = PosZ - m_Self.GetChunkZ() * cChunkDef::Width;

	if (!a_Chunk.IsLightValid())
	{
		a_Chunk.GetWorld()->QueueLightChunk(m_Self.GetChunkX(), m_Self.GetChunkZ());
		return;
	}

	if (
		(a_Chunk.GetSkyLight(RelX, RelY, RelZ) == 15) &&				// In the daylight
		(a_Chunk.GetBlock(RelX, RelY, RelZ) != E_BLOCK_SOULSAND) &&		// Not on soulsand
		(a_Chunk.GetWorld()->GetTimeOfDay() < (12000 + 1000)) &&		// It is nighttime
		!m_Self.IsOnFire() &&											// Not already burning
		a_Chunk.GetWorld()->IsWeatherWetAt(PosX, PosZ)					// Not raining
	)
	{
		// Burn for 100 ticks, then decide again
		m_Self.StartBurning(100);
	}
}
