#include "Globals.h"
#include "EnvironmentComponent.h"
#include "../Monster.h"
#include "../../World.h"
#include "../../Chunk.h"

cEnvironmentComponent::cEnvironmentComponent(cMonster * a_Entity, int a_SightDistance) : m_Self(a_Entity), m_SightDistance(a_SightDistance){}





void cEnvironmentComponent::Tick(float a_Dt, cChunk & a_Chunk) {

	// Burning in daylight
	HandleDaylightBurning(a_Chunk);
}





void cEnvironmentComponent::HandleDaylightBurning(cChunk & a_Chunk)
{
	if (!m_BurnsInDaylight)
	{
		return;
	}
	
	int RelY = (int)floor(m_Self->GetPosY());
	if ((RelY < 0) || (RelY >= cChunkDef::Height))
	{
		// Outside the world
		return;
	}
	int PosX = (int)floor(m_Self->GetPosX());
	int PosZ = (int)floor(m_Self->GetPosX());
	int RelX = PosX - m_Self->GetChunkX() * cChunkDef::Width;
	int RelZ = PosZ - m_Self->GetChunkZ() * cChunkDef::Width;

	if (!a_Chunk.IsLightValid())
	{
		m_Self->GetWorld()->QueueLightChunk(m_Self->GetChunkX(), m_Self->GetChunkZ());
		return;
	}

	if (
		(a_Chunk.GetSkyLight(RelX, RelY, RelZ) == 15) &&				// In the daylight
		(a_Chunk.GetBlock(RelX, RelY, RelZ) != E_BLOCK_SOULSAND) &&		// Not on soulsand
		(m_Self->GetWorld()->GetTimeOfDay() < (12000 + 1000)) &&		// It is nighttime
		!m_Self->IsOnFire() &&											// Not already burning
		m_Self->GetWorld()->IsWeatherWetAt(PosX, PosZ)					// Not raining
	)
	{
		// Burn for 100 ticks, then decide again
		m_Self->StartBurning(100);
	}
}
